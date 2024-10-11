#include "ChessCell.h"

AChessCell::AChessCell()
{
	PrimaryActorTick.bCanEverTick = true;
	cellMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CellMesh"));
}

void AChessCell::SetPawn(AChessPawn* p)
{
	pawn = p;
	pawn->SetActorLocation(GetActorLocation());
	pawn->SetActorRotation(GetActorRotation());
}

void AChessCell::BeginPlay()
{
	Super::BeginPlay();
	
}

void AChessCell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

