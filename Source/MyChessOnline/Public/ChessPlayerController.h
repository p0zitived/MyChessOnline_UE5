#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraActor.h"
#include "ChessPlayerPawn.h"
#include "ChessCell.h"
#include "ChessPlayerController.generated.h"

class AChessGameMode;

UCLASS()
class MYCHESSONLINE_API AChessPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AChessPlayerController();

	UFUNCTION(Client,Reliable)
	virtual void Init(bool blackTeam, TArray<TArray<AChessCell*>> cells);
	void UpdateCamera();
	
	UFUNCTION(BlueprintPure, Category="Chess Player Controller")
	AChessPlayerPawn* GetControlledPlayerPawn();

	UFUNCTION(Client,Reliable)
	void InvokeTurnStarted(bool isBlackTurn);

	UFUNCTION(Client,Reliable)
	void TurnTick(float remainedTime);

	UFUNCTION(BlueprintNativeEvent, Category="Chess Player Controller")
	void TurnTickEvent(float remainedTime);

	void GetCellCoords(AChessCell* targetCell, int& coordX, int& coordY);

protected:
	// FUNCTIONS
	UFUNCTION(Server,Reliable,BlueprintCallable)
	void ServerFinishTurn();
	virtual void ServerFinishTurn_Implementation();

	UFUNCTION(BlueprintCallable, Category= "Chess Player Controller")
	virtual FString OnMouseClicked();
	virtual void Tick(float DeltaTime) override;
	virtual void TurnTickEvent_Implementation(float remainedTime);
	virtual void Init_Implementation(bool blackTeam, TArray<TArray<AChessCell*>> cells);
	virtual void TurnTick_Implementation(float remainedTime);
	virtual void InvokeTurnStarted_Implementation(bool isBlackTurn);
	void SelectCell(AChessCell* targetCell);
	AChessGameMode* GetChessGameMode();

	UFUNCTION(Server,Reliable)
	void ServerSetCellState(AChessPlayerController* caller,AChessCell* targetCell, EChessCellState newState);
	void ServerSetCellState_Implementation(AChessPlayerController* caller,AChessCell* targetCell, EChessCellState newState);

	UFUNCTION(Client, Reliable)
	void ClientSetCellState(AChessCell* targetCell, EChessCellState newState);
	void ClientSetCellState_Implementation(AChessCell* targetCell, EChessCellState newState);

	UFUNCTION(BlueprintNativeEvent, Category="Chess Events")
	void OnTurnStarted(bool isBlackTurn);
	virtual void OnTurnStarted_Implementation(bool isBlackTurn);

	void OnCellHovered(AChessCell* hoveredCell);

	// VARIABLES
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Chess Player Controller")
	bool isInBlackTeam;
	bool isInited;
	bool isMyTurn;
	TArray<TArray<AChessCell*>> boardCells;
	AChessCell* currentHoveredCell;
	AChessCell* previousHoveredCell;
	AChessCell* currentSelectedCell;
	ACameraActor* cameraActor;
};
