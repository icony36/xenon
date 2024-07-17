// Copyright CoNY. All rights reserved.


#include "Character/XeAICharacter.h"

#include "AbilitySystem/XeAbilitySystemComponent.h"
#include "AbilitySystem/XeAttributeSet.h"

AXeAICharacter::AXeAICharacter()
{
	// Setup Ability System Component.
	AbilitySystemComponent = CreateDefaultSubobject<UXeAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	// Setup Attribute Set.
	AttributeSet = CreateDefaultSubobject<UXeAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AXeAICharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
