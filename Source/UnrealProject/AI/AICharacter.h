// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

class UActionSystemComponent;

UCLASS()
class UNREALPROJECT_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AAICharacter();
	
	virtual void PostInitializeComponents() override;
	
	void OnHealthChanged(FGameplayTag InGameplayTag, float NewHealth, float OldHealth);
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UActionSystemComponent> ActionSystemComponent;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Death")
	TObjectPtr<UAnimMontage> DeathMontage;
};
