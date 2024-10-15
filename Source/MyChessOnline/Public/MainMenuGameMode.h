#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MainMenuGameMode.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnSessionsUpdated);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnSessionCreated,FName,sessionName,bool,success);

USTRUCT(BlueprintType)
struct FSessionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString sessionName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int hostLogoIndx;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool enemyConnected;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString enemyName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int enemyLogoIndx;
	

	FSessionData(){}

	FSessionData(FString name,int logoIndx,bool enemyConn,FString enemyN,int enemyLogo)
	{
		sessionName = name;
		hostLogoIndx = logoIndx;
		enemyName = enemyN;
		enemyLogoIndx = enemyLogoIndx;
		enemyConnected = enemyConn;
	}
};

UCLASS()
class MYCHESSONLINE_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	// FUNCTIONS
	UFUNCTION(BlueprintCallable, Category = "HD Online")
	bool CreateSession(FString hostName, int hostLogoIndx, FOnSessionCreated onCompleted);
	UFUNCTION(BlueprintCallable, Category = "HD Online")
	void ConnectToSession(int sessionIndex, FString nickname, int logo);
	UFUNCTION(BlueprintCallable, Category = "HD Online")
	void UpdateSessionsData(FOnSessionsUpdated onFinish);
	// PROPERTIES
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HD Online");
	TArray<FSessionData> FoundSessions;

protected:
	virtual void BeginPlay() override;
	void OnSessionSearchCompleted(bool success);
	void OnSessionCreationCompleted(FName sessionName, bool success);
	void OnJoinedToSession(FName sessionName, EOnJoinSessionCompleteResult::Type result);
	void OnSessionSettingsUpdated(FName sesionName,const FOnlineSessionSettings &settings);
	void OnSessionUpdated(FName sessionName, bool success);
	UFUNCTION(Server,Reliable,BlueprintCallable, Category = "HD Online")
	void InformSessionAboutMe(FName sessionName,const FString& myNickname,int muLogo);
	void InformSessionAboutMe_Implementation(FName sessionName, const FString& myNickname,int muLogo);
	FOnSessionsUpdated auxOnSessionsUpdated;
	FOnSessionCreated auxOnSessionCreated;
	FOnSessionsUpdated auxOnJoinedToSession;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	FString auxPlayerName;
	int auxPlayerLogo;
};
