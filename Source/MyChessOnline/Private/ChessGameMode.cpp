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
		
		// check daca sa primit sa spawnez pawn-ul
		if (pawn)
		{
			pawn->SetTeam(pawnData[i].isBlack);

			// check ca sa nu dea crash in caz ca bagi index gresit prin ChessRules
			if (cells.IsValidIndex(pawnData[i].X) && cells[pawnData[i].X].IsValidIndex(pawnData[i].Y))
			{
				cells[pawnData[i].X][pawnData[i].Y]->SetPawn(pawn);
			}
		}
	}
}

void AChessGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	players.Add(Cast<AChessPlayerController>(NewPlayer));

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, "Player Connected");

	if (players.Num() == 1)
	{
		players[0]->Init(false);
	}
	else
	{
		players[1]->Init(true);
	}
}
