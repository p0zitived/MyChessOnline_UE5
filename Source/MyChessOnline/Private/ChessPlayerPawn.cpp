#include "ChessPlayerPawn.h"
#include "ChessPlayerController.h"

AChessPlayerPawn::AChessPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

}

AChessPlayerController* AChessPlayerPawn::GetChessController()
{
	if (AController* controller = GetController())
	{
		return Cast<AChessPlayerController>(controller);
	}
	return nullptr;
}

void AChessPlayerPawn::InvokeTurnStarted(bool isBlackTurn)
{
	OnTurnStarted(isBlackTurn);
}

void AChessPlayerPawn::TurnTick_Implementation(float remainedSecodns)
{
}

void AChessPlayerPawn::OnTurnStarted_Implementation(bool isBlackTurn)
{
}

void AChessPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
}

void AChessPlayerPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AChessPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChessPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

