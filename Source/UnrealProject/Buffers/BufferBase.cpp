// Fill out your copyright notice in the Description page of Project Settings.


#include "BufferBase.h"

#include "TestProjectGameTypes.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"


// Sets default values
ABufferBase::ABufferBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	const ECollisionChannel Collision = COLLISION_BUFFERS;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABufferBase::OnSphereBeginOverlap);
	SphereComponent->SetCollisionProfileName("Buffers");
	SphereComponent->SetupAttachment(RootComponent);
	
	BufferMesh = CreateDefaultSubobject<UStaticMeshComponent>("BufferMesh");
	BufferMesh->SetupAttachment(SphereComponent);
}

// BufferBase.cpp
void ABufferBase::OnSphereBeginOverlap(UPrimitiveComponent*, AActor* OtherActor,
	UPrimitiveComponent*, int32, bool, const FHitResult&)
{
	if (OtherActor && OtherActor != this && ApplyBuff(OtherActor))
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation(), FRotator::ZeroRotator);
		Destroy();
	}
}

// Called when the game starts or when spawned
void ABufferBase::BeginPlay()
{
	Super::BeginPlay();
}

void ABufferBase::AnimateMesh()
{
	FVector NewLocation = BufferMesh->GetRelativeLocation();
	NewLocation.Z += FMath::Sin(GetWorld()->GetTimeSeconds() * FloatSpeed);
	BufferMesh->SetRelativeLocation(NewLocation);
}

// Called every frame
void ABufferBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//Make the mesh float up and down
	AnimateMesh();
}

