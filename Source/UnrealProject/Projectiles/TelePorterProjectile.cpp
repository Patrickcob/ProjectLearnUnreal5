// Fill out your copyright notice in the Description page of Project Settings.


#include "TelePorterProjectile.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ATelePorterProjectile::ATelePorterProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
}

// Called when the game starts or when spawned
void ATelePorterProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle TeleporterTimerHandle;
	GetWorldTimerManager().SetTimer(TeleporterTimerHandle, [this]()
	{
		TeleportPlayerToLocation(FVector::ZeroVector);
	}, TeleportDelay, false);
}

// Called every frame
void ATelePorterProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATelePorterProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FTimerHandle TeleporterTimerHandle;
	GetWorldTimerManager().SetTimer(TeleporterTimerHandle, [this, Hit]()
	{
		TeleportPlayerToLocation(Hit.ImpactPoint);
	}, TeleportDelay, false);
}

void ATelePorterProjectile::TeleportPlayerToLocation(const FVector& Location)
{
	if (bHasTeleported)
	{
		return;
	}
	
	bHasTeleported = true;
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, TeleporterExitEffect, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this, TeleporterExplosionSound, GetActorLocation(), FRotator::ZeroRotator);
	
	FVector TeleporterLocation = Location;
	if (TeleporterLocation == FVector::ZeroVector)
	{
		TeleporterLocation = GetActorLocation();
	}
	
	FTimerHandle TeleporterTimerHandle;
	GetWorldTimerManager().SetTimer(TeleporterTimerHandle, [this, TeleporterLocation]()
	{
		Teleport(TeleporterLocation);
	}, TeleportDelay, false);
}

void ATelePorterProjectile::Teleport(const FVector& Location)
{
	APawn* Player = GetInstigator();
	if (Player)
	{
		//Play Animation to enter the teleporter at player's location
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(Player, TeleporterExitEffect, GetActorLocation());
		Player->TeleportTo(Location, Player->GetActorRotation());
	}
	
	Destroy();
}

void ATelePorterProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->OnComponentHit.AddDynamic(this, &ATelePorterProjectile::OnHit);
}

