#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MainMenuGameMode.h"
#include <Kismet/GameplayStatics.h>

bool AMainMenuGameMode::CreateSession(FString hostName, int hostLogoIndx, FOnSessionCreated onCompleted)
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();

		TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());
		SessionSettings->bAllowJoinInProgress = true;
		SessionSettings->bIsDedicated = false;
		SessionSettings->bIsLANMatch = true;
		SessionSettings->bShouldAdvertise = true;
		SessionSettings->bUsesPresence = true;
		SessionSettings->NumPublicConnections = 2;
		SessionSettings->Set("HostLogo", hostLogoIndx, EOnlineDataAdvertisementType::ViaOnlineService);
		SessionSettings->Set("HostName", hostName, EOnlineDataAdvertisementType::ViaOnlineService);
		SessionSettings->Set("EnemyConnected", false, EOnlineDataAdvertisementType::ViaOnlineService);
		auxOnSessionCreated = onCompleted;

		const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
		return Session->CreateSession(*localPlayer->GetPreferredUniqueNetId(), FName(hostName), *SessionSettings);
	}

	return false;
}

void AMainMenuGameMode::OnSessionCreationCompleted(FName sessionName, bool success)
{
	auxOnSessionCreated.ExecuteIfBound(sessionName, success);

	if (success)
	{
		GetWorld()->ServerTravel("/Game/_Game/MainGameplay?listen");
	}
}

void AMainMenuGameMode::ConnectToSession(int sessionIndex, FString nickname, int logo)
{
	if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		if (IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface())
		{
			if (FoundSessions.IsValidIndex(sessionIndex))
			{
				auxPlayerName = nickname;
				auxPlayerLogo = logo;
				SessionInterface->JoinSession(0, FName(FoundSessions[sessionIndex].sessionName), SessionSearch->SearchResults[sessionIndex]);
			}
		}
	}
}

void AMainMenuGameMode::OnJoinedToSession(FName sessionName, EOnJoinSessionCompleteResult::Type result)
{
	if (APlayerController* pcController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FString joinAdress;

		if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
		{
			if (IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface())
			{
				SessionInterface->GetResolvedConnectString(sessionName, joinAdress);

				if (joinAdress != "")
				{
					FOnlineSessionSettings* newSettings = SessionInterface->GetSessionSettings(sessionName);
					newSettings->Set("EnemyConnected", true, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
					newSettings->Set("EnemyName", auxPlayerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
					newSettings->Set("EnemyLogo", auxPlayerLogo, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
					SessionInterface->UpdateSession(sessionName, *newSettings);
				}
			}
		}
	}
}

void AMainMenuGameMode::OnSessionUpdated(FName sessionName, bool success)
{
	GEngine->AddOnScreenDebugMessage(0, 10, FColor::Red, "Session Updated " + success);
	if (APlayerController* pcController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FString joinAdress;

		if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
		{
			if (IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface())
			{
				SessionInterface->GetResolvedConnectString(sessionName, joinAdress);

				if (joinAdress != "")
				{
					pcController->ClientTravel(joinAdress, ETravelType::TRAVEL_Absolute);
				}
			}
		}
	}
}

void AMainMenuGameMode::OnSessionSettingsUpdated(FName sesionName,const FOnlineSessionSettings &settings) {}

void AMainMenuGameMode::UpdateSessionsData(FOnSessionsUpdated onFinish)
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();

	if (OnlineSubsystem) 
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
		if (Session.IsValid())
		{
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			SessionSearch->bIsLanQuery = true;

			const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();

			Session->FindSessions(*localPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
			auxOnSessionsUpdated = onFinish;
		}
	}
}

void AMainMenuGameMode::OnSessionSearchCompleted(bool success)
{
	FoundSessions.Empty();
	if (success)
	{
		for (FOnlineSessionSearchResult result : SessionSearch->SearchResults)
		{
			int logoId;
			FString hostName;
			bool enemyConnected;
			FString enemyName;
			int enemyLogo;
			result.Session.SessionSettings.Get("HostLogo", logoId);
			result.Session.SessionSettings.Get("HostName", hostName);
			result.Session.SessionSettings.Get("EnemyConnected", enemyConnected);
			result.Session.SessionSettings.Get("EnemyName", enemyName);
			result.Session.SessionSettings.Get("EnemyLogo", enemyLogo);
			FoundSessions.Add(FSessionData(hostName, logoId, enemyConnected, enemyName, enemyLogo));
		}
	}

	if (auxOnSessionsUpdated.IsBound())
	{
		auxOnSessionsUpdated.Execute();
	}
}

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
		Session->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionCompleteDelegate::CreateUObject(this, &AMainMenuGameMode::OnSessionCreationCompleted));
		Session->OnFindSessionsCompleteDelegates.AddUObject(this, &AMainMenuGameMode::OnSessionSearchCompleted);
		Session->OnJoinSessionCompleteDelegates.AddUObject(this, &AMainMenuGameMode::OnJoinedToSession);
		Session->OnSessionSettingsUpdatedDelegates.AddUObject(this, &AMainMenuGameMode::OnSessionSettingsUpdated);
		Session->OnUpdateSessionCompleteDelegates.AddUObject(this, &AMainMenuGameMode::OnSessionUpdated);
	}
}


