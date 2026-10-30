// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "TestProjectileMagic.generated.h"

class UActionEffect;
class UNiagaraSystem;

UCLASS(Abstract)
class UNREALPROJECT_API ATestProjectileMagic : public AProjectile
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	TSubclassOf<UDamageType> DamageTypeClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<UNiagaraSystem> ExplosionSystem;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> ExplosionSound;
	
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	TSubclassOf<UActionEffect> EffectOnHit;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
public:
	
	// Called when the game starts or when spawned
	virtual void PostInitializeComponents() override;
	
	// Sets default values for this actor's properties
	ATestProjectileMagic();
};
