#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraActor.h"
#include "ChessPlayerController.generated.h"

UCLASS()
class MYCHESSONLINE_API AChessPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void Init(bool blackTeam);

protected:
	// FUNCTIONS
	virtual void BeginPlay() override;
	// VARIABLES
	bool isInBlackTeam;
	bool isInited;
	ACameraActor* cameraActor;
};
