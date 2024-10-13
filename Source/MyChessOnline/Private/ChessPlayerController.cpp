#include "ChessPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraActor.h"

void AChessPlayerController::Init(bool blackTeam)
{
	if (isInited)
		return;
	isInBlackTeam = blackTeam;
	isInited = true;

	// De schimbat aici
	// Numi place de facut prin delay
	// Dar problema este ca nustiu in ce ordine se executa setarea camerei, cineva ii face override la camera mea
	FTimerHandle updateCameraTimer;
	GetWorld()->GetTimerManager().SetTimer(updateCameraTimer,this,&AChessPlayerController::UpdateCamera,1);
}

void AChessPlayerController::UpdateCamera()
{
	TArray<AActor*> foundActors;
	if (isInBlackTeam)
	{
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACameraActor::StaticClass(), "BlackCamera", foundActors);
	}
	else
	{
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACameraActor::StaticClass(), "WhiteCamera", foundActors);
	}

	if (foundActors.Num() > 0)
	{
		cameraActor = Cast<ACameraActor>(foundActors[0]);
	}

	if (cameraActor)
	{
		SetViewTarget(cameraActor);
	}
}
