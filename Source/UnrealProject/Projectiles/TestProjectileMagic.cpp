// Fill out your copyright notice in the Description page of Project Settings.


#include "TestProjectileMagic.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "ActionSystem/ActionEffect.h"
#include "ActionSystem/ActionSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Hearing.h"

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
	
	UAISense_Hearing::ReportNoiseEvent(this, Hit.ImpactPoint, 1.0f, GetInstigator());
	UGameplayStatics::ApplyPointDamage(OtherActor, AttackDamage, HitFromDirection, Hit, GetInstigatorController(), this, nullptr);

	if (OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(HitFromDirection * (AttackDamage * 100), Hit.ImpactPoint);
	}

	if (EffectOnHit && IsValid(OtherActor))
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