#include "ChessPawn.h"
#include "ChessCell.h"

AChessPawn::AChessPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pawn Mesh"));

	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative,false);
	staticMesh->AttachToComponent(RootComponent,rules);
	staticMesh->SetIsReplicated(true);
	bReplicates = true;
}

void AChessPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AChessPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChessPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AChessPawn::SetTeam(bool black)
{
	isBlack = black;

	if (isBlack)
	{
		staticMesh->SetStaticMesh(blackMesh);
		AddActorWorldRotation(FRotator(0, 180, 0));
	}
	else
	{
		staticMesh->SetStaticMesh(whiteMesh);
	}
}

TArray<AChessCell*> AChessPawn::GetAvaibleCells(TArray<TArray<AChessCell*>> cells, int pawnCoordX, int pawnCoordY)
{
	TArray<AChessCell*> result = TArray<AChessCell*>();

	if (!alreadyMovedOnce)
	{
		if (!isBlack)
		{
			// forward once
			if (cells[pawnCoordX].IsValidIndex(pawnCoordY + 1))
			{
				result.Add(cells[pawnCoordX][pawnCoordY + 1]);
			}
			// forward twice
			if (cells[pawnCoordX].IsValidIndex(pawnCoordY + 2))
			{
				result.Add(cells[pawnCoordX][pawnCoordY + 2]);
			}
		}
		else
		{
			// forward once
			if (cells[pawnCoordX].IsValidIndex(pawnCoordY - 1))
			{
				result.Add(cells[pawnCoordX][pawnCoordY - 1]);
			}
			// forward twice
			if (cells[pawnCoordX].IsValidIndex(pawnCoordY - 2))
			{
				result.Add(cells[pawnCoordX][pawnCoordY - 2]);
			}
		}
	}
	else
	{
		if (!isBlack)
		{
			// forward once
			if (cells[pawnCoordX].IsValidIndex(pawnCoordY + 1))
			{
				result.Add(cells[pawnCoordX][pawnCoordY + 1]);
			}
		}
		else
		{
			// forward once
			if (cells[pawnCoordX].IsValidIndex(pawnCoordY - 1))
			{
				result.Add(cells[pawnCoordX][pawnCoordY - 1]);
			}
		}
	}

	return result;
}

// NET CODING
void AChessPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AChessPawn, isBlack);
}