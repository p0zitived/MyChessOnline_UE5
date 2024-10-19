#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ChessPlayerPawn.generated.h"

UCLASS()
class MYCHESSONLINE_API AChessPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	AChessPlayerPawn();
	UFUNCTION(BlueprintPure, Category="Chess Player Pawn")
	AChessPlayerController* GetChessController();
	void InvokeTurnStarted(bool isBlackTurn);

	UFUNCTION(BlueprintNativeEvent, Category = "Chess Player pawn")
	void TurnTick(float remainedSeconds);

protected:
	virtual void TurnTick_Implementation(float remainedSeconds);

	UFUNCTION(BlueprintNativeEvent, Category="Chess Player pawn")
	void OnTurnStarted(bool isBlackTurn);
	virtual void OnTurnStarted_Implementation(bool isBlackTurn);

	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
