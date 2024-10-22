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

void AChessPlayerController::Init(bool blackTeam)
{
	if (isInited)
		return;
	isInBlackTeam = blackTeam;
	isInited = true;

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
		if (currentHoveredCell->GetState() != EChessCellState::Selected)
		{
			ServerSetCellState(currentHoveredCell, EChessCellState::Default);
		}
	}

	previousHoveredCell = currentHoveredCell;
	currentHoveredCell = hoveredCell;

	// if hovered cell isn't nullptr
	if (currentHoveredCell)
	{
		// change cell state only if is not selected
		if (currentHoveredCell->GetState() != EChessCellState::Selected)
		{
			ServerSetCellState(currentHoveredCell, EChessCellState::Hovered);
		}
	}
}

void AChessPlayerController::ServerSetCellState_Implementation(AChessCell* targetCell, EChessCellState newState)
{
	AChessGameMode* gm = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	if (gm)
	{
		for (AChessPlayerController* player : gm->GetPlayers())
		{
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

	switch (currentHoveredCell->GetState())
	{
		case EChessCellState::Default:
			// deselect previouse selected cell
			if (currentSelectedCell)
			{
				ServerSetCellState(currentSelectedCell, EChessCellState::Default);
			}

			currentSelectedCell = currentHoveredCell;
			ServerSetCellState(currentSelectedCell, EChessCellState::Selected);
			return "Cell State Was : Default";
		break;
		case EChessCellState::Hovered:
			// deselect previouse selected cell
			if (currentSelectedCell)
			{
				ServerSetCellState(currentSelectedCell, EChessCellState::Default);
			}

			currentSelectedCell = currentHoveredCell;
			ServerSetCellState(currentSelectedCell, EChessCellState::Selected);
			return "Cell State Was : Hovered";
		break;
		case EChessCellState::Selected:
			if (currentSelectedCell == currentHoveredCell)
			{
				ServerSetCellState(currentSelectedCell, EChessCellState::Hovered);
				currentSelectedCell = nullptr;
			}
			return "Cell State Was : Selected";
		break;
		case EChessCellState::Avaible:
			return "Cell State Was : Avaible";
		break;
		case EChessCellState::Unavaible:
			return "Cell State Was : Unavaible";
		break;
	}

	return "Nothing passed";
}