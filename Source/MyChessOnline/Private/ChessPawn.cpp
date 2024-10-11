#include "ChessPawn.h"

AChessPawn::AChessPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pawn Mesh"));

	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative,false);
	staticMesh->AttachToComponent(RootComponent,rules);
}

void AChessPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AChessPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChessPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

