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

int32 AXeAICharacter::GetCombatLevel_Implementation()
{
	return CombatLevel;
}

void AXeAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SetupCombatInfo();

	// Add startup abilities only on server.
	if (HasAuthority())
	{
		AddStartupAbilities();
	}
}

void AXeAICharacter::SetupCombatInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	// Initialize default Attributes.
	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
}
