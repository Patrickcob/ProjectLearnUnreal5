// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBuffer.h"

#include "SharedGameplayTags.h"
#include "ActionSystem/ActionSystemComponent.h"
#include "Core/TestProjectGameplayStatics.h"


// Sets default values
AHealthBuffer::AHealthBuffer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHealthBuffer::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AHealthBuffer::ApplyBuff(AActor* Target)
{
	UActionSystemComponent* ActionSystemComponent = Target->FindComponentByClass<UActionSystemComponent>();
	if (!ActionSystemComponent || UTestProjectGameplayStatics::IsFullHealth(ActionSystemComponent))
	{
		return false;
	}
	
	ActionSystemComponent->ApplyAttributeChange(SharedGameplayTags::Attribute_Health, BuffAmount, Base);
	return true;
}

// Called every frame
void AHealthBuffer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

