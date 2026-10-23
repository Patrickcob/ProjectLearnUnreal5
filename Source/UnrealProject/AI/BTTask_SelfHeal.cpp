// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SelfHeal.h"

#include "AICharacter.h"
#include "AIController.h"
#include "SharedGameplayTags.h"
#include "ActionSystem/ActionSystemComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_SelfHeal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAICharacter* OwningPawn = Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	check(OwningPawn);
	
	OwningPawn->ActionSystemComponent->ApplyAttributeChange(SharedGameplayTags::Attribute_Health, SelfHealAmount, Base);
	
	UE_LOG(LogTemp, Warning, TEXT("AI Character SelfHeal applied."));
	
	return EBTNodeResult::Succeeded;
}
