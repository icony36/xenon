// Copyright CoNY. All rights reserved.


#include "Character/XeAICharacter.h"

#include "AbilitySystem/XeAbilitySystemComponent.h"
#include "AbilitySystem/XeAttributeSet.h"

AXeAICharacter::AXeAICharacter()
{
	// Setup Ability System Component.
	AbilitySystemComponent = CreateDefaultSubobject<UXeAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	// Setup Attribute Set.
	AttributeSet = CreateDefaultSubobject<UXeAttributeSet>("AttributeSet");
}

void AXeAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void AXeAICharacter::SetupCombatInfo()
{
	Super::SetupCombatInfo();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}
