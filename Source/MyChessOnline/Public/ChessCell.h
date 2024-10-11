#pragma once

#include "CoreMinimal.h"
#include "ChessPawn.h"
#include "GameFramework/Actor.h"
#include "ChessCell.generated.h"

UCLASS()
class MYCHESSONLINE_API AChessCell : public AActor
{
	GENERATED_BODY()
	
public:	
	AChessCell();
	void SetPawn(AChessPawn* p);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	UStaticMeshComponent* cellMesh;

protected:
	virtual void BeginPlay() override;
	AChessPawn* pawn;

public:	
	virtual void Tick(float DeltaTime) override;

};
