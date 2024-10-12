#include "ChessPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"

void AChessPlayerController::Init(bool blackTeam)
{
	isInBlackTeam = blackTeam;

	TArray<AActor*> foundActors;
	if (isInBlackTeam)
	{
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACameraActor::StaticClass(), "BlackCamera", foundActors);
		if (foundActors.Num() > 0)
		{
			cameraActor = Cast<ACameraActor>(foundActors[0]);
		}
	}
	else
	{
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACameraActor::StaticClass(), "WhiteCamera", foundActors);
		if (foundActors.Num() > 0)
		{
			cameraActor = Cast<ACameraActor>(foundActors[0]);
		}
	}

	if (cameraActor)
	{
		SetViewTarget(cameraActor);
	}

	isInited = true;
}

void AChessPlayerController::BeginPlay()
{

}
