#pragma once

#include "CoreMinimal.h"
#include "ChessPawn.h"
#include "GameFramework/Actor.h"
#include "ChessCell.generated.h"

UENUM(BlueprintType)
enum class EChessCellState : uint8
{
	Default			UMETA(DisplayName = "Default"),
	Hovered			UMETA(DisplayName = "Hovered"),
	Selected		UMETA(DisplayName = "Selected"),
	Avaible			UMETA(DisplayName = "Avaible"),
	Unavaible		UMETA(DisplayName = "Unavaible"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOneParamDelegate,EChessCellState, newState);

UCLASS()
class MYCHESSONLINE_API AChessCell : public AActor
{
	GENERATED_BODY()
	
public:	
	AChessCell();
	void SetState(EChessCellState newState);
	void SetPawn(AChessPawn* p);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	UStaticMeshComponent* cellMesh;

	// EVENTS
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOneParamDelegate onStateChanged;

protected:
	virtual void BeginPlay() override;

	AChessPawn* pawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Logic")
	EChessCellState currentState;

public:	
	virtual void Tick(float DeltaTime) override;

};
