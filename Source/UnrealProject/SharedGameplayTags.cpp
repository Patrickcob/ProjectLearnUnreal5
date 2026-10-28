#include "SharedGameplayTags.h"

namespace SharedGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Action_PrimaryAttack, "Action.PrimaryAttack");
	UE_DEFINE_GAMEPLAY_TAG(Action_BlackHoleAttack, "Action.BlackHoleAttack");
	UE_DEFINE_GAMEPLAY_TAG(Action_TeleporterAttack, "Action.TeleporterAttack");
	UE_DEFINE_GAMEPLAY_TAG(Action_Sprint, "Action.Sprint");
	
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Health, "Attribute.Health");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_MaxHealth, "Attribute.MaxHealth");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Rage, "Attribute.Rage");
	UE_DEFINE_GAMEPLAY_TAG(Attribute_MaxRage, "Attribute.MaxRage");
	
	UE_DEFINE_GAMEPLAY_TAG(StatusEffect_Sprinting, "StatusEffect.Sprinting");
	UE_DEFINE_GAMEPLAY_TAG(StatusEffect_Stunned, "StatusEffect.Stunned");
}
