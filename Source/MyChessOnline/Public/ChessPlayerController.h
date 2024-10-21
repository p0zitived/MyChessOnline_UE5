#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraActor.h"
#include "ChessPlayerPawn.h"
#include "ChessCell.h"
#include "ChessPlayerController.generated.h"

UCLASS()
class MYCHESSONLINE_API AChessPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AChessPlayerController();

	virtual void Init(bool blackTeam);
	void UpdateCamera();
	
	UFUNCTION(BlueprintPure, Category="Chess Player Controller")
	AChessPlayerPawn* GetControlledPlayerPawn();

	UFUNCTION(Client,Reliable)
	void InvokeTurnStarted(bool isBlackTurn);

	UFUNCTION(Client,Reliable)
	void TurnTick(float remainedTime);

	UFUNCTION(BlueprintNativeEvent, Category="Chess Player Controller")
	void TurnTickEvent(float remainedTime);

protected:
	// FUNCTIONS
	UFUNCTION(Server,Reliable,BlueprintCallable)
	void ServerFinishTurn();
	virtual void ServerFinishTurn_Implementation();
	virtual void Tick(float DeltaTime) override;
	virtual void TurnTickEvent_Implementation(float remainedTime);
	virtual void TurnTick_Implementation(float remainedTime);
	virtual void InvokeTurnStarted_Implementation(bool isBlackTurn);

	UFUNCTION(Server,Reliable)
	void ServerSetCellState(AChessCell* targetCell, EChessCellState newState);
	void ServerSetCellState_Implementation(AChessCell* targetCell, EChessCellState newState);

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
	AChessCell* currentHoveredCell;
	AChessCell* previousHoveredCell;
	ACameraActor* cameraActor;
};
