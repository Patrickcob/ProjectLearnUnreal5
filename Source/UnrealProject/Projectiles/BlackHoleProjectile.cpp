// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHoleProjectile.h"

#include "Components/SphereComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"


// Sets default values
ABlackHoleProjectile::ABlackHoleProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	SphereComponent->SetSphereRadius(300.0f);
	SphereComponent->SetCollisionProfileName("BlackHole");
	SphereComponent->SetGenerateOverlapEvents(true);
	

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForceComponent");
	RadialForceComponent->SetupAttachment(SphereComponent);
	RadialForceComponent->Radius = 300.0f;
	RadialForceComponent->ForceStrength = -10000000.0f;
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->bAutoActivate = true;
	RadialForceComponent->bIgnoreOwningActor = true;
	
	InitialLifeSpan = 5.0f;
}

void ABlackHoleProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABlackHoleProjectile::OnSphereBeginOverlap);
}

void ABlackHoleProjectile::Destroy()
{
	Super::Destroy();
}

// Called when the game starts or when spawned
void ABlackHoleProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABlackHoleProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABlackHoleProjectile::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->IsSimulatingPhysics())
	{
		OtherActor->Destroy();
	}
}

