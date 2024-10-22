#pragma once

#include "CoreMinimal.h"
#include "ChessPawn.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
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
	UFUNCTION(Client,Reliable)
	void SetPawn(AChessPawn* p);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	UStaticMeshComponent* cellMesh;
	UPROPERTY(Replicated,EditAnywhere, BlueprintReadOnly)
	int cellVariationIndex = 0;

	void SetState(EChessCellState newState);
	EChessCellState GetState() const;

	UFUNCTION(Client, Reliable)
	void ClientSetState(EChessCellState newState);
	
	AChessPawn* GetPawn() const;

protected:
	void ClientSetState_Implementation(EChessCellState newState);

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void SetPawn_Implementation(AChessPawn* p);

	UFUNCTION(BlueprintNativeEvent)
	void OnStateChanged(EChessCellState newState);
	virtual void OnStateChanged_Implementation(EChessCellState newState);

	UPROPERTY(Replicated)
	AChessPawn* pawn;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Main Logic")
	EChessCellState currentState;

public:	
	virtual void Tick(float DeltaTime) override;

};
