// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

class AAIController;
class UAIDataAsset;
class UActionSystemComponent;

UCLASS(Abstract)
class UNREALPROJECT_API AAICharacter : public ACharacter,  public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AAICharacter();

	virtual FGenericTeamId GetGenericTeamId() const override;
	
	UAIDataAsset* GetMonsterAsset() const { return MonsterAsset; }
	
	void SetMonsterAsset(UAIDataAsset* InMonsterAsset)
	{
		check(InMonsterAsset != nullptr);
		MonsterAsset = InMonsterAsset;
	}

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;
	
	void OnHealthChanged(FGameplayTag InGameplayTag, float NewHealth, float OldHealth);
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UActionSystemComponent> ActionSystemComponent;


	UFUNCTION()
	void OnGameplayTagUpdated(FGameplayTag UpdatedTag, int32 NewCount);
	
	UPROPERTY(EditDefaultsOnly, Category=Animation)
	TObjectPtr<UAnimMontage> StunnedAnimation;
protected:
	UPROPERTY(Transient)
	TObjectPtr<UAIDataAsset> MonsterAsset;
	
	UPROPERTY(EditDefaultsOnly, Category = "Death")
	TObjectPtr<UAnimMontage> DeathMontage;
	
	bool bIsDead = false;
	
	FTimerHandle OverlayTimerHandle;
};
