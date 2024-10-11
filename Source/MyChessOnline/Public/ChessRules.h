#pragma once

#include "CoreMinimal.h"
#include "ChessCell.h"
#include "ChessPawn.h"
#include "Engine/DataAsset.h"
#include "ChessRules.generated.h"

USTRUCT(BlueprintType)
struct FPawnData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PawnData")
	TSubclassOf<AChessPawn> pawnClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PawnData")
	int X;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PawnData")
	int Y;
};

USTRUCT(BlueprintType)
struct FChessBoardData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoardData")
	TArray<FPawnData> pawnData;
};

UCLASS()
class MYCHESSONLINE_API UChessRules : public UDataAsset
{
	GENERATED_BODY()

public:
	UChessRules();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Board Settings")
	int BoardLengthX = 8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Board Settings")
	int BoardLengthY = 8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board Settings")
	FVector StartLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board Settings")
	FVector CellSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board Settings")
	FVector CellSpace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board Settings")
	TSubclassOf<AChessCell> CellClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Set Up")
	FChessBoardData InitialBoardSetUp;
};
