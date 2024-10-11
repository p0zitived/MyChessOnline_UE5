#include "ChessGameMode.h"

void AChessGameMode::BeginPlay() 
{
	GenerateBoard();
	SetUpBoard();
}

void AChessGameMode::GenerateBoard()
{
	cells.SetNum(chessRules->BoardLengthX);
	for (int i = 0; i < chessRules->BoardLengthX; i++)
	{
		cells[i].SetNum(chessRules->BoardLengthY);
	}

	for (int x = 0; x < chessRules->BoardLengthX; x++) 
	{
		for (int y = 0; y < chessRules->BoardLengthY; y++)
		{
			FVector targetLocation = FVector(
				x * (chessRules->CellSize.X + chessRules->CellSpace.X) + chessRules->StartLocation.X,
				y * (chessRules->CellSize.Y + chessRules->CellSpace.Y) + chessRules->StartLocation.Y,
				chessRules->StartLocation.Z);

			FRotator targetRotation = FRotator(0, 0, 0);

			cells[x][y] = GetWorld()->SpawnActor<AChessCell>(chessRules->CellClass,targetLocation,targetRotation);
		}
	}
}

void AChessGameMode::SetUpBoard()
{
	TArray<FPawnData> pawnData = chessRules->InitialBoardSetUp.pawnData;
	for (int i = 0; i < pawnData.Num(); i++)
	{
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AChessPawn* pawn = GetWorld()->SpawnActor<AChessPawn>(pawnData[i].pawnClass,params);
		if (pawn)
		{
			cells[pawnData[i].X][pawnData[i].Y]->SetPawn(pawn);
		}
	}
}
