// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionEffect_StunMonster.h"

#include "SharedGameplayTags.h"

UActionEffect_StunMonster::UActionEffect_StunMonster()
{
	// Matches available animation duration for RangedMinion
	Duration = 2.5f;
	GrantsTags.AddTag(SharedGameplayTags::StatusEffect_Stunned);
}
