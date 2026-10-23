// Fill out your copyright notice in the Description page of Project Settings.


#include "TestProjectItemChest.h"


// Sets default values
ATestProjectItemChest::ATestProjectItemChest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	ChestStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("ChestStaticMesh");
	ChestStaticMesh->SetCollisionProfileName("Interaction");
	RootComponent = ChestStaticMesh;
	
	LidStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidStaticMesh");
	LidStaticMesh->SetCollisionProfileName("Interaction");
	LidStaticMesh->SetupAttachment(ChestStaticMesh);
}

void ATestProjectItemChest::Interact_Implementation()
{
	SetActorTickEnabled(true);
}

void ATestProjectItemChest::OpenChest(float DeltaTime)
{
	CurrentLidPitch = FMath::FInterpConstantTo(CurrentLidPitch, TargetLidPitch, DeltaTime, LidInterpolationSpeed);
	
	LidStaticMesh->SetRelativeRotation(FRotator(CurrentLidPitch, 0, 0.0f));
	
	if (FMath::IsNearlyEqual(CurrentLidPitch, TargetLidPitch))
	{
		ChestAnimationComplete();
		SetActorTickEnabled(false);
	}
}

// Called every frame
//TODO: Move the logic here since Tick is not the place for it, but for now it's fine
void ATestProjectItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	OpenChest(DeltaTime);
}

