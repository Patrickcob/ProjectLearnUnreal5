// Fill out your copyright notice in the Description page of Project Settings.


#include "TestProjectileMagic.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "ActionSystem/ActionEffect.h"
#include "ActionSystem/ActionSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATestProjectileMagic::ATestProjectileMagic()
{	

}

void ATestProjectileMagic::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	SphereComponent->OnComponentHit.AddDynamic(this, &ATestProjectileMagic::OnHit);
}

void ATestProjectileMagic::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FVector HitFromDirection = GetActorRotation().Vector();
	
	UGameplayStatics::ApplyPointDamage(OtherActor, 10.0f, HitFromDirection, Hit, GetInstigatorController(), this, DamageTypeClass);

	if (EffectOnHit)
	{
		UActionSystemComponent* ActionComponent = OtherActor->FindComponentByClass<UActionSystemComponent>();
		if (ActionComponent)
		{
			ActionComponent->GrantAction(EffectOnHit);
		}
	}
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionSystem, GetActorLocation());
	
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation(), FRotator::ZeroRotator);
	
	Destroy();
}