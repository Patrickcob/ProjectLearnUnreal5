// Fill out your copyright notice in the Description page of Project Settings.


#include "TestProjectInteractionComponent.h"

#include "TestProjectGameTypes.h"
#include "Engine/OverlapResult.h"
#include "Core/TestProjectInteraction.h"


TAutoConsoleVariable<bool> CVarInteractionDebugDrawing(
	TEXT("game.interaction.debugdrawing"),
	true,
	TEXT("Enable debug drawing for interaction component."),
	ECVF_Cheat
);

UTestProjectInteractionComponent::UTestProjectInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTestProjectInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	const bool bEnabledDebugDraw = CVarInteractionDebugDrawing.GetValueOnGameThread();

	APlayerController* PC = CastChecked<APlayerController>(GetOwner());
	
	FVector PlayerLocation = PC->GetPawn()->GetActorLocation();
	FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();
	
	ECollisionChannel CollisionChannel = COLLISION_INTERACTION;
	TArray<FOverlapResult> OutOverlaps;
	GetWorld()->OverlapMultiByChannel(
		OutOverlaps,
		PlayerLocation,
		FQuat::Identity,
		CollisionChannel,
		FCollisionShape::MakeSphere(InteractionRadius)
	);

	if (bEnabledDebugDraw)
	{
		DrawDebugSphere(GetWorld(), PlayerLocation, InteractionRadius, 32, FColor::White);
	}

	
	float InteractionRadiusSquared = InteractionRadius * InteractionRadius;
	
	float HighestWeight = 0.0f;
	AActor* BestActor = nullptr;
	for (FOverlapResult& OutOverlap : OutOverlaps)
	{
		FVector Origin;
		FVector BoxExtents;
		OutOverlap.GetActor()->GetActorBounds(true, Origin, BoxExtents);
		FVector OverlapDirection = (Origin - CameraLocation).GetSafeNormal();
		
		float NormalizedDistanceTo = (1.0f - FVector::Distance(Origin, PlayerLocation) / InteractionRadiusSquared) * DistanceToWeightScale;
		double NormalizedDotProduct = (FVector::DotProduct(OverlapDirection, PC->GetControlRotation().Vector()) * 0.5 + 0.5) * DirectionWeightScale;
		
		float Weight = NormalizedDotProduct + NormalizedDistanceTo;
		DrawDebugString(GetWorld(), Origin, FString::Printf(TEXT("Weight: %f, Dot: %.2f"), Weight, NormalizedDotProduct), nullptr, FColor::Yellow, 0.f, true);
		if (Weight > HighestWeight)
		{
			HighestWeight = Weight;
			BestActor = OutOverlap.GetActor();
		}
	}
		
	if (BestActor)
	{
		SelectedActor = BestActor;
		if (bEnabledDebugDraw)
		{
			DrawDebugBox(GetWorld(), BestActor->GetActorLocation(), FVector(50.f, 50.f, 50.f), FColor::Green);
		}
		
		//TODO: Move this to a UI Widget soon
		//DrawDebugString(GetWorld(), BestActor->GetActorLocation(), FString::Printf(TEXT("F to Interact")), nullptr, FColor::White, 0.f, true);
	}
}

void UTestProjectInteractionComponent::Interact()
{
	if (SelectedActor != nullptr)
	{
		ITestProjectInteraction::Execute_Interact(SelectedActor);
	}
	
	
}

