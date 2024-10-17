#include "ChessPlayerPawn.h"
#include "ChessPlayerController.h"

AChessPlayerPawn::AChessPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

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

