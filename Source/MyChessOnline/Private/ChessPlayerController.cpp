#include "ChessPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraActor.h"
#include <ChessGameMode.h>

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
	bShowMouseCursor = true;
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

AChessPlayerPawn* AChessPlayerController::GetControlledPlayerPawn()
{
	if (APawn* controlledPawn = GetPawn())
	{
		return Cast<AChessPlayerPawn>(controlledPawn);
	}
	return nullptr;
}


void AChessPlayerController::TurnTick_Implementation(float remainedTime)
{
	if (AChessPlayerPawn* playerPawn = GetControlledPlayerPawn())
	{
		UE_LOG(LogTemp, Log, TEXT("ChessPlayerController : TurnTick called"));
		TurnTickEvent(remainedTime);
		playerPawn->TurnTick(remainedTime);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ChessPlayerController : controlled player pawn is Null"));
	}
}

void AChessPlayerController::InvokeTurnStarted_Implementation(bool isBlackTurn)
{
	isMyTurn = (isBlackTurn && isInBlackTeam) || (!isBlackTurn && !isInBlackTeam);
	if (AChessPlayerPawn* playerPawn = GetControlledPlayerPawn())
	{
		playerPawn->InvokeTurnStarted(isBlackTurn);
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("ChessPlayerController : controlled player pawn is Null"))
	}
	
	OnTurnStarted(isBlackTurn);
}

void AChessPlayerController::OnTurnStarted_Implementation(bool isBlackTurn)
{
}
void AChessPlayerController::ServerFinishTurn_Implementation()
{
	if (AChessGameMode* gm = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode()))
	{
		gm->FinishTurn();
	}
}
void AChessPlayerController::TurnTickEvent_Implementation(float remainedTime)
{
}