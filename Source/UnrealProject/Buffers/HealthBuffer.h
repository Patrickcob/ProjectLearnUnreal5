// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BufferBase.h"
#include "HealthBuffer.generated.h"

UCLASS()
class UNREALPROJECT_API AHealthBuffer : public ABufferBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHealthBuffer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual bool ApplyBuff(AActor* Target) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
