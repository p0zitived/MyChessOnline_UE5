#include "ChessCell.h"
#include "Net/UnrealNetwork.h"

AChessCell::AChessCell()
{
	PrimaryActorTick.bCanEverTick = true;
	cellMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CellMesh"));
	bReplicates = true;
}

void AChessCell::SetPawn(AChessPawn* p)
{
	pawn = p;
	pawn->SetActorLocation(GetActorLocation());
	pawn->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));
}

void AChessCell::SetState(EChessCellState newState)
{
	currentState = newState;
	OnStateChanged(newState);
}

EChessCellState AChessCell::GetState() const
{
	return currentState;
}

void AChessCell::ClientSetState_Implementation(EChessCellState newState)
{
	SetState(newState);
}

void AChessCell::BeginPlay()
{
	Super::BeginPlay();
}

void AChessCell::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AChessCell, cellVariationIndex);
	DOREPLIFETIME(AChessCell, currentState);
}

void AChessCell::OnStateChanged_Implementation(EChessCellState newState)
{

}


void AChessCell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

