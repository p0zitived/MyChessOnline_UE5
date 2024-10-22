#include "ChessPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraActor.h"
#include <ChessGameMode.h>
#include "ChessCell.h"

AChessPlayerController::AChessPlayerController()
{
	bReplicates = true;
}

void AChessPlayerController::Init_Implementation(bool blackTeam, TArray<TArray<AChessCell*>> cells)
{
	if (isInited)
		return;
	isInBlackTeam = blackTeam;
	isInited = true;
	boardCells = cells;
	// De schimbat aici
	// Numi place de facut prin delay
	// Dar problema este ca nustiu in ce ordine se executa setarea camerei, cineva ii face override la camera mea
	FTimerHandle updateCameraTimer;
	GetWorld()->GetTimerManager().SetTimer(updateCameraTimer,this,&AChessPlayerController::UpdateCamera,2);
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

void AChessPlayerController::GetCellCoords(AChessCell* targetCell, int& coordX, int& coordY)
{
	for (int i = 0; i < boardCells.Num(); i++)
	{
		for (int j = 0; j < boardCells[i].Num(); j++)
		{
			if (boardCells[i][j] == targetCell)
			{
				coordX = i;
				coordY = j;
				return;
			}
		}
	}
}

void AChessPlayerController::ServerFinishTurn_Implementation()
{
	if (AChessGameMode* gm = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode()))
	{
		gm->FinishTurn();
	}
}
void AChessPlayerController::Tick(float DeltaTime)
{
	bShowMouseCursor = true; // ALWAYS SHOW MOUSE

	float mousePosX, mousePosY;
	if (GetMousePosition(mousePosX, mousePosY))
	{
		FVector worldLocation;
		FVector worldDir;

		if (DeprojectScreenPositionToWorld(mousePosX, mousePosY, worldLocation, worldDir))
		{
			FHitResult hitResult;

			bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, worldLocation, worldLocation + worldDir * 10000, ECollisionChannel::ECC_Camera);
			if (bHit)
			{
				AChessCell* cell = Cast<AChessCell>(hitResult.GetActor());
				if (cell)
				{
					OnCellHovered(cell);
				}
				else
				{
					cell = Cast<AChessCell>(hitResult.GetActor()->GetAttachParentActor());
					if (cell)
					{
						OnCellHovered(cell);
					}
				}
			}
			else
			{
				OnCellHovered(nullptr);
			}
		}
	}
}

void AChessPlayerController::TurnTickEvent_Implementation(float remainedTime)
{
}

void AChessPlayerController::OnCellHovered(AChessCell* hoveredCell)
{
	// if we hovered another cell
	if (currentHoveredCell && currentHoveredCell != hoveredCell)
	{
		// if previouse cell was selected, do not change it's state
		if (currentHoveredCell->GetState() == EChessCellState::Hovered)
		{
			currentHoveredCell->SetState(EChessCellState::Default);
			ServerSetCellState(this,currentHoveredCell, EChessCellState::Default);
		}
	}

	previousHoveredCell = currentHoveredCell;
	currentHoveredCell = hoveredCell;

	// if hovered cell isn't nullptr
	if (currentHoveredCell)
	{
		// change cell state only if is not selected
		if (currentHoveredCell->GetState() == EChessCellState::Default)
		{
			currentHoveredCell->SetState(EChessCellState::Hovered);
			ServerSetCellState(this,currentHoveredCell, EChessCellState::Hovered);
		}
	}
}

void AChessPlayerController::ServerSetCellState_Implementation(AChessPlayerController* caller,AChessCell* targetCell, EChessCellState newState)
{
	AChessGameMode* gm = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	if (gm)
	{
		for (AChessPlayerController* player : gm->GetPlayers())
		{
			if (player != caller)
				player->ClientSetCellState(targetCell, newState);
		}
	}
}

void AChessPlayerController::ClientSetCellState_Implementation(AChessCell* targetCell, EChessCellState newState)
{
	targetCell->SetState(newState);
}

// MOUSE CLICK LOGIC
FString AChessPlayerController::OnMouseClicked()
{
	if (!currentHoveredCell)
		return "Current Hovered cell is null";

	if (!isMyTurn)
		return "Is not my turn";

	switch (currentHoveredCell->GetState())
	{
		case EChessCellState::Default:
			SelectCell(currentHoveredCell);
			return "Cell State Was : Default";
		case EChessCellState::Hovered:
			SelectCell(currentHoveredCell);
			return "Cell State Was : Hovered";
		case EChessCellState::Selected:
			if (currentSelectedCell == currentHoveredCell)
			{
				ServerSetCellState(this, currentSelectedCell, EChessCellState::Hovered);
				currentSelectedCell->SetState(EChessCellState::Hovered);
				currentSelectedCell = nullptr;
			}
			return "Cell State Was : Selected";
		case EChessCellState::Avaible:
			return "Cell State Was : Avaible";
		case EChessCellState::Unavaible:
			return "Cell State Was : Unavaible";
	}

	return "Nothing passed";
}

void AChessPlayerController::SelectCell(AChessCell* targetCell)
{
	if (!targetCell)
		return;

	// deselect previouse selected cell
	if (currentSelectedCell)
	{
		ServerSetCellState(this, currentSelectedCell, EChessCellState::Default);
		currentSelectedCell->SetState(EChessCellState::Default);
	}

	currentSelectedCell = targetCell;
	ServerSetCellState(this, currentSelectedCell, EChessCellState::Selected);
	currentSelectedCell->SetState(EChessCellState::Selected);

	if (AChessPawn* pawn = currentSelectedCell->GetPawn())
	{
		int x, y;
		GetCellCoords(currentSelectedCell, x, y);
		TArray<AChessCell*> avaibleCells = pawn->GetAvaibleCells(boardCells, x, y);

		for (AChessCell* cell : avaibleCells)
		{
			cell->SetState(EChessCellState::Avaible);
			ServerSetCellState(this, cell, EChessCellState::Avaible);
		}

		// clear all avaible cells
		for (int i = 0; i < boardCells.Num(); i++)
		{
			for (int j = 0; j < boardCells[i].Num(); j++)
			{
				if ((boardCells[i][j]->GetState() == EChessCellState::Avaible ||
					boardCells[i][j]->GetState() == EChessCellState::Unavaible) &&
					!avaibleCells.Contains(boardCells[i][j]))
				{
					boardCells[i][j]->SetState(EChessCellState::Default);
					ServerSetCellState(this, boardCells[i][j], EChessCellState::Default);
				}
			}
		}
	}
}

AChessGameMode* AChessPlayerController::GetChessGameMode()
{
	return Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
}
