#pragma once

#include "CoreMinimal.h"
#include "ChessRules.h"
#include "ChessCell.h"
#include "ChessPlayerController.h"
#include "GameFramework/GameModeBase.h"
#include "ChessGameMode.generated.h"

UCLASS()
class MYCHESSONLINE_API AChessGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AChessGameMode();
	void FinishTurn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game Settings")
	UChessRules* chessRules;

protected:
	TArray<TArray<AChessCell*>> cells;
	TArray<AChessPlayerController*> players;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void ServerFinishTurn_Implementation();

private:
	void GenerateBoard();
	void SetUpBoard();
	void PostLogin(APlayerController* NewPlayer);
	void StartGame();
	void TurnTick(float DeltaTime);

	bool isBlackTurn;
	float blackTurnRemainedTime;
	float whiteTurnRemainedTime;
};
