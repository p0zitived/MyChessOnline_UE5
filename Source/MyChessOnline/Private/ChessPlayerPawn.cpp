#include "ChessPlayerPawn.h"

AChessPlayerPawn::AChessPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AChessPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AChessPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChessPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

