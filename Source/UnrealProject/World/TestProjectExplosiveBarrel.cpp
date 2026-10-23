// Fill out your copyright notice in the Description page of Project Settings.


#include "TestProjectExplosiveBarrel.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"


// Sets default values
ATestProjectExplosiveBarrel::ATestProjectExplosiveBarrel()
{
	ExplosiveBarrelStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("ExplosiveBarrelStaticMesh");
	ExplosiveBarrelStaticMesh->SetSimulatePhysics(true);
	RootComponent = ExplosiveBarrelStaticMesh;
	
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForceComponent");
	RadialForceComponent->SetupAttachment(RootComponent);
	RadialForceComponent->ImpulseStrength = 150000.0f;
	RadialForceComponent->Radius = 750.0f;
	RadialForceComponent->bAutoActivate = false;
	RadialForceComponent->bIgnoreOwningActor = true;
}


void ATestProjectExplosiveBarrel::TakeDamageTimerElapsed()
{
	bExploded = true;

	if (ActiveBurningEffectComp)
	{
		ActiveBurningEffectComp->Deactivate();
	}

	if (ActiveBurningSoundComp)
	{
		ActiveBurningSoundComp->Stop();
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionEffect, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
	RadialForceComponent->FireImpulse();
	
	ExplosiveBarrelStaticMesh->AddImpulse(FVector::UpVector * 1000, NAME_None, true);
	ExplosiveBarrelStaticMesh->AddAngularImpulseInDegrees(FVector::RightVector * 1000, NAME_None, true);
}

float ATestProjectExplosiveBarrel::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float Applied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (bExploded || GetWorldTimerManager().IsTimerActive(ExplosionTimerHandle))
	{
		return Applied;
	}
	
	FVector ImpactLocation = GetActorLocation();
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		ImpactLocation = PointEvent->HitInfo.ImpactPoint;
	}
	
	ActiveBurningEffectComp = UNiagaraFunctionLibrary::SpawnSystemAttached(BurningEffect, ExplosiveBarrelStaticMesh, NAME_None, ImpactLocation, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);
	ActiveBurningSoundComp = UGameplayStatics::SpawnSoundAttached(BurningSound, ExplosiveBarrelStaticMesh);
	
	GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &ATestProjectExplosiveBarrel::TakeDamageTimerElapsed, ExplosionDelay, false);
	
	return Applied;
}
