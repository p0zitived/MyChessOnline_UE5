#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Pawn.h"
#include "ChessPawn.generated.h"

class AChessCell;

UCLASS()
class MYCHESSONLINE_API AChessPawn : public APawn
{
	GENERATED_BODY()

public:
	AChessPawn();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visual Settings")
	UStaticMeshComponent* staticMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Visaul Settings")
	UStaticMesh* whiteMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Visaul Settings")
	UStaticMesh* blackMesh;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Chess Logic")
	bool isBlack;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	bool alreadyMovedOnce;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void SetTeam(bool black);
	virtual TArray<AChessCell*> GetAvaibleCells(TArray<TArray<AChessCell*>> cells, int pawnCoordX,int pawnCoordY);
};
