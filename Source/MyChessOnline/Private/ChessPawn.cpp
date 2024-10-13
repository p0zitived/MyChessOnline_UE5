#include "ChessPawn.h"

AChessPawn::AChessPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pawn Mesh"));

	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative,false);
	staticMesh->AttachToComponent(RootComponent,rules);
	staticMesh->SetIsReplicated(true);
	bReplicates = true;
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

void AChessPawn::SetTeam(bool black)
{
	isBlack = black;

	if (isBlack)
	{
		staticMesh->SetStaticMesh(blackMesh);
		AddActorWorldRotation(FRotator(0, 180, 0));
	}
	else
	{
		staticMesh->SetStaticMesh(whiteMesh);
	}
}

// NET CODING
void AChessPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AChessPawn, isBlack);
}