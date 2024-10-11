#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ChessPawn.generated.h"

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Chess Logic")
	bool isBlack;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void SetTeam(bool black);
};
