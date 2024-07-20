// Copyright CoNY. All rights reserved.


#include "Character/XeCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/XeAbilitySystemComponent.h"
#include "AbilitySystem/XeAttributeSet.h"
#include "Components/WidgetComponent.h"

AXeCharacter::AXeCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Setup status widget above head.
	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>("OverheadWidget");
	OverheadWidget->SetupAttachment(GetRootComponent());
}

UAbilitySystemComponent* AXeCharacter::GetAbilitySystemComponent() const
{
	return XeAbilitySystemComponent;
}

UAttributeSet* AXeCharacter::GetAttributeSet() const
{
	return XeAttributeSet;
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
	
	// Apply effect.
	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),ASC);
}

void AXeCharacter::AddStartupAbilities() const
{
	if (!HasAuthority()) return;
	
	XeAbilitySystemComponent->AddCharacterAbilities(StartupAbilityClasses);
}

void AXeCharacter::SetupCombatInfo()
{
	// Implement in child class.
}

void AXeCharacter::SetupOverheadWidget()
{
	// Implement in child class.
}

void AXeCharacter::BindCallbacksToDependencies()
{
	// Implement in child class.
}

