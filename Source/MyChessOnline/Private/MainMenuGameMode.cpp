#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MainMenuGameMode.h"

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

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
		Session->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionCompleteDelegate::CreateUObject(this, &AMainMenuGameMode::OnSessionCreationCompleted));
		Session->OnFindSessionsCompleteDelegates.AddUObject(this, &AMainMenuGameMode::OnSessionSearchCompleted);
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
			result.Session.SessionSettings.Get("HostLogo", logoId);
			result.Session.SessionSettings.Get("HostName", hostName);
			FoundSessions.Add(FSessionData(hostName, logoId));
		}
	}

	if (auxOnSessionsUpdated.IsBound())
	{
		auxOnSessionsUpdated.Execute();
	}
}


bool AMainMenuGameMode::ConnectToSession(int sessionIndex)
{
	return false;
}
