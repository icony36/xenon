// Copyright CoNY. All rights reserved.


#include "Character/XeCharacter.h"

AXeCharacter::AXeCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

UAbilitySystemComponent* AXeCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AXeCharacter::SetupCombatInfo()
{
}
