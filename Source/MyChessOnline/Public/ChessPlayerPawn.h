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

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
