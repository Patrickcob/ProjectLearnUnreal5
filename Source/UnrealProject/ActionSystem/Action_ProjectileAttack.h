// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "Projectiles/Projectile.h"
#include "Action_ProjectileAttack.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UAction_ProjectileAttack : public UAction
{
	GENERATED_BODY()
	
	virtual void StartAction_Implementation() override;
	
	UAction_ProjectileAttack();
protected:
	void SpawnProjectile(ACharacter* InstigatorCharacter);
	void AttackDelayElapsed(ACharacter* InstigatorCharacter);
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AProjectile> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TObjectPtr<UAnimMontage> AttackMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TObjectPtr<USoundBase> AttackSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	FName MuzzleSocketName;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TObjectPtr<UNiagaraSystem> AttackEffect;
	
};
