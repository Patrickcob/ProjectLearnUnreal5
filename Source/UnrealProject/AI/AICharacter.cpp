// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"

#include "SharedGameplayTags.h"
#include "ActionSystem/ActionSystemComponent.h"
#include "GameFramework/PawnMovementComponent.h"


// Sets default values
AAICharacter::AAICharacter()
{
	ActionSystemComponent = CreateDefaultSubobject<UActionSystemComponent>("ActionSystemComponent");
	ActionSystemComponent->SetDefaultAttributes(UAIAttributeSet::StaticClass());
}

void AAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ActionSystemComponent->GetAttributeChangedDelegate(SharedGameplayTags::Attribute_Health).AddUObject(this, &ThisClass::OnHealthChanged);
}

void AAICharacter::OnHealthChanged(FGameplayTag InAttributeTag, float NewHealth, float OldHealth)
{
	UE_LOG(LogTemp, Warning, TEXT("AICharacter Health Changed: OldHealth=%f, NewHealth=%f"), OldHealth, NewHealth);

	if (FMath::IsNearlyZero(NewHealth))
	{
		UE_LOG(LogTemp, Warning, TEXT("AICharacter Died"));
	}
}

float AAICharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("AICharacter TakeDamage: DamageAmount=%f"), DamageAmount);
	
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ActionSystemComponent->ApplyAttributeChange(SharedGameplayTags::Attribute_Health, -ActualDamage, Base);
	return ActualDamage;
}