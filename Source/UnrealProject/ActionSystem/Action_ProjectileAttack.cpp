// Fill out your copyright notice in the Description page of Project Settings.


#include "Action_ProjectileAttack.h"

#include "ActionSystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "TestProjectGameTypes.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Projectiles/Projectile.h"

void UAction_ProjectileAttack::StartAction_Implementation()
{
	Super::StartAction_Implementation();
	
	UActionSystemComponent* ActionComp = GetOwningComponent();
	ACharacter* Character = Cast<ACharacter>(ActionComp->GetOwner());

	Character->PlayAnimMontage(AttackMontage);
	
	UNiagaraFunctionLibrary::SpawnSystemAttached(AttackEffect, Character->GetMesh(), MuzzleSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);
	
	UGameplayStatics::PlaySound2D(this, AttackSound);
	
	FTimerHandle AttackTimerHandle;
	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &UAction_ProjectileAttack::AttackDelayElapsed, Character);
	Character->GetWorldTimerManager().SetTimer(AttackTimerHandle, Delegate, 0.2f, false);
}

UAction_ProjectileAttack::UAction_ProjectileAttack()
{
	CoolDownTime = 0.5f;
}

void UAction_ProjectileAttack::AttackDelayElapsed(ACharacter* InstigatorCharacter)
{
	SpawnProjectile(InstigatorCharacter);
	StopAction();
}


void UAction_ProjectileAttack::SpawnProjectile(ACharacter* InstigatorCharacter)
{
	if (!ensure(ProjectileClass))
	{
		return;
	}

	const FVector SpawnLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(MuzzleSocketName);

	FVector EyeLocation;
	FRotator EyeRotation;
	InstigatorCharacter->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	const FVector TraceEnd = EyeLocation + EyeRotation.Vector() * 10000.f;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(InstigatorCharacter);

	FHitResult Hit;
	const FVector Target = GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_PROJECTILE, QueryParams) ? Hit.ImpactPoint : TraceEnd;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = InstigatorCharacter;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* Projectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, (Target - SpawnLocation).Rotation(), SpawnParams);
	InstigatorCharacter->MoveIgnoreActorAdd(Projectile);
}
