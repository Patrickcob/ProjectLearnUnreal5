// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BufferBase.generated.h"

class UNiagaraComponent;
class USphereComponent;

UCLASS()
class UNREALPROJECT_API ABufferBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABufferBase();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<USphereComponent> SphereComponent;
	
	//Buffer mesh
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> BufferMesh;
	
	//Pickup sound
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> PickupSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float FloatSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "Buff")
	float BuffAmount;
	
	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	virtual bool ApplyBuff(AActor* OtherActor) PURE_VIRTUAL(ABufferBase::ApplyBuff, return false;);
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void AnimateMesh();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
