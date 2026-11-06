// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"

#include "AIController.h"
#include "SharedGameplayTags.h"
#include "ActionSystem/ActionSystemComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Core/TestProjectGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISense_Damage.h"


// Sets default values
AAICharacter::AAICharacter()
{
	ActionSystemComponent = CreateDefaultSubobject<UActionSystemComponent>("ActionSystemComponent");
	ActionSystemComponent->SetDefaultAttributes(UAIAttributeSet::StaticClass());
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	ActionSystemComponent->OnGameplayTagCountChanged.AddDynamic(this, &ThisClass::OnGameplayTagUpdated);
}

FGenericTeamId AAICharacter::GetGenericTeamId() const
{
	if (auto AIController = GetController<AAIController>())
	{
		return AIController->GetGenericTeamId();
	}
	
	return FGenericTeamId::NoTeam;
	
}

void AAICharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		UTestProjectGameInstance* GI = GetGameInstance<UTestProjectGameInstance>();
		if (GI)	
		{
			GI->AliveMonsters.RemoveSingleSwap(this, EAllowShrinking::No);
		}
	}
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	UTestProjectGameInstance* GI = GetGameInstance<UTestProjectGameInstance>();
	if (GI)
	{
		GI->AliveMonsters.Add(this);
	}
}

void AAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	GetMesh()->SetOverlayMaterialMaxDrawDistance(1);

	ActionSystemComponent->GetAttributeChangedDelegate(SharedGameplayTags::Attribute_Health).AddUObject(this, &ThisClass::OnHealthChanged);
}

void AAICharacter::OnHealthChanged(FGameplayTag InAttributeTag, float NewHealth, float OldHealth)
{
	UE_LOG(LogTemp, Warning, TEXT("AICharacter Health Changed: OldHealth=%f, NewHealth=%f"), OldHealth, NewHealth);

	if (FMath::IsNearlyZero(NewHealth) && !bIsDead)
	{
		bIsDead = true;

		AAIController* AIController = GetController<AAIController>();
		AIController->GetBrainComponent()->StopLogic("Dead");
		
		GetMesh()->bPauseAnims = true;
		
		GetMesh()->SetCollisionProfileName("Ragdoll");
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		
		GetCharacterMovement()->DisableMovement();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		UTestProjectGameInstance* GI = GetGameInstance<UTestProjectGameInstance>();
		if (GI)	
		{
			GI->AliveMonsters.RemoveSingleSwap(this, EAllowShrinking::No);
		}
		
		SetLifeSpan(5.0f);
	}
}

float AAICharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("AICharacter TakeDamage: DamageAmount=%f"), DamageAmount);

	const ETeamAttitude::Type TeamAttitude = GetTeamAttitudeTowards(*EventInstigator);
	
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ActionSystemComponent->ApplyAttributeChange(SharedGameplayTags::Attribute_Health, -ActualDamage, Base);
	
	GetMesh()->SetOverlayMaterialMaxDrawDistance(0);
	GetMesh()->SetCustomPrimitiveDataFloat(0, GetWorld()->TimeSeconds);
	
	GetWorldTimerManager().SetTimer(OverlayTimerHandle, [this]()
	{
		if (GetMesh())
		{
			GetMesh()->SetOverlayMaterialMaxDrawDistance(1);
		}
	}, 1.0f, false);

	if (IsValid(EventInstigator) && EventInstigator->GetPawn() != this && TeamAttitude != ETeamAttitude::Friendly)
	{
		UAISense_Damage::ReportDamageEvent(this, this, EventInstigator->GetPawn(), FMath::Abs(ActualDamage), EventInstigator->GetPawn()->GetActorLocation(), GetActorLocation());
	}
	
	return ActualDamage;
}

void AAICharacter::OnGameplayTagUpdated(FGameplayTag UpdatedTag, int32 NewCount)
{
	// Block/Allow movement
	if (UpdatedTag.MatchesTag(SharedGameplayTags::StatusEffect_Stunned))
	{
		const bool bWasAdded = NewCount > 0;

		EMovementMode NewMoveMode = bWasAdded ? MOVE_None : MOVE_Walking;
		GetCharacterMovement()->SetMovementMode(NewMoveMode);
		
		// Pause ALL logic for Monster
		AAIController* AIC = Cast<AAIController>(GetController());
		// Could be nullptr for dead monsters, in that case we shouldn't reach here anymore
		check(AIC);
		UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(AIC->GetBrainComponent());
		check(BTComp);
	
		// Alternatively you could restart the logic to skip any 'waits' it might be in and get out of the stunned
		// picking a new thing to do right away (BTComp->StartLogic() BTComp->StopLogic())
		if (bWasAdded)
		{
			BTComp->PauseLogic("StunApplied");
		}
		else
		{
			BTComp->ResumeLogic("StunRemoved");
		}
		
		// Animation
		if (bWasAdded)
		{
			PlayAnimMontage(StunnedAnimation);
		}
	}
}
