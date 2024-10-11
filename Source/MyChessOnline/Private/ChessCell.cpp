#include "ChessCell.h"

AChessCell::AChessCell()
{
	PrimaryActorTick.bCanEverTick = true;
	cellMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CellMesh"));
}

void AChessCell::SetState(EChessCellState newState)
{
	currentState = newState;
	onStateChanged.Broadcast(newState);
}

void AChessCell::SetPawn(AChessPawn* p)
{
	pawn = p;
	pawn->SetActorLocation(GetActorLocation());
}

void AChessCell::BeginPlay()
{
	Super::BeginPlay();
	
}


void AChessCell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

