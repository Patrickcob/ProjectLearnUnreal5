// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RangedAttack.h"

#include "AIController.h"
#include "TestProjectGameTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Projectiles/TestProjectileMagic.h"

UBTTask_RangedAttack::UBTTask_RangedAttack()
{
	TargetActorKey.SelectedKeyName = NAME_TargetActor;
}

EBTNodeResult::Type UBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* OwningPawn = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
	check(OwningPawn);
	
	FVector MuzzleLocation = OwningPawn->GetMesh()->GetSocketLocation(MuzzleSocketName);
	
	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!IsValid(TargetActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetActor is not valid"));
		return EBTNodeResult::Failed;
	}
	
	AAIController* Controller = OwnerComp.GetAIOwner();
	bool bHasLOS = Controller->LineOfSightTo(TargetActor);
	if (!bHasLOS)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetActor is not in line of sight, Moving..."));
		return EBTNodeResult::Succeeded;
	}
	
	FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
	FRotator SpawnRotation = Direction.Rotation();
	
	SpawnRotation.Pitch += FMath::RandRange(-SpreadAngle, SpreadAngle);
	SpawnRotation.Yaw += FMath::RandRange(-SpreadAngle, SpreadAngle);
	
	FActorSpawnParameters SpawnParam;
	SpawnParam.Instigator = OwningPawn;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, SpawnRotation, SpawnParam);	
	return NewProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
