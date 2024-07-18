// Copyright CoNY. All rights reserved.


#include "Character/XeCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/XeAbilitySystemComponent.h"

AXeCharacter::AXeCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

UAbilitySystemComponent* AXeCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AXeCharacter::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultBasicAttributes, 1.f);
	ApplyEffectToSelf(DefaultEssentialAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f); // * apply Vital Attributes last as they depend on Essential Attributes
}

void AXeCharacter::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, float Level) const
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	
	check(IsValid(ASC));
	check(GameplayEffectClass);
	
	// Apply effect to self.
	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),ASC);
}

void AXeCharacter::AddStartupAbilities() const
{
	if (!HasAuthority()) return;
	
	UXeAbilitySystemComponent* XeASC = CastChecked<UXeAbilitySystemComponent>(AbilitySystemComponent);

	XeASC->AddCharacterAbilities(StartupAbilityClasses);
}

void AXeCharacter::SetupCombatInfo()
{
}

