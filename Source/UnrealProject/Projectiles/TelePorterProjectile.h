// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "TelePorterProjectile.generated.h"

class UNiagaraSystem;
class USoundBase;

UCLASS()
class UNREALPROJECT_API ATelePorterProjectile : public AProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATelePorterProjectile();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<UNiagaraSystem> TeleporterExitEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<UNiagaraSystem> TeleporterEnterEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> TeleporterExplosionSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float TeleportDelay = 0.2f;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	bool bHasTeleported = false;
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	void TeleportPlayerToLocation(const FVector& Location);
	
	void Teleport(const FVector& Location);
	
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

};
