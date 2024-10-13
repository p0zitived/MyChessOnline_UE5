#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
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

	FSessionData(){}

	FSessionData(FString name,int logoIndx)
	{
		sessionName = name;
		hostLogoIndx = logoIndx;
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
	bool ConnectToSession(int sessionIndex);
	UFUNCTION(BlueprintCallable, Category = "HD Online")
	void UpdateSessionsData(FOnSessionsUpdated onFinish);
	// PROPERTIES
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HD Online");
	TArray<FSessionData> FoundSessions;

protected:
	virtual void BeginPlay() override;
	void OnSessionSearchCompleted(bool success);
	void OnSessionCreationCompleted(FName sessionName, bool success);
	FOnSessionsUpdated auxOnSessionsUpdated;
	FOnSessionCreated auxOnSessionCreated;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
};
