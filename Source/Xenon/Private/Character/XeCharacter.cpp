// Copyright CoNY. All rights reserved.


#include "Character/XeCharacter.h"

#include "AbilitySystemComponent.h"
#include "XeGameplayTags.h"
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

AActor* AXeCharacter::GetAvatar_Implementation() 
{
	return this;
}

FVector AXeCharacter::GetSocketLocation_Implementation(const FGameplayTag& SocketTag)
{
	const FXeGameplayTags& GameplayTags = FXeGameplayTags::Get();
	if (SocketTag.MatchesTagExact(GameplayTags.Socket_WeaponTip))
	{
		return GetMesh()->GetSocketLocation("WeaponTip");
	}

	if (SocketTag.MatchesTagExact(GameplayTags.Socket_LeftHand))
	{
		return GetMesh()->GetSocketLocation("LeftHand");
	}

	if (SocketTag.MatchesTagExact(GameplayTags.Socket_RightHand))
	{
		return GetMesh()->GetSocketLocation("RightHand");
	}

	if (SocketTag.MatchesTagExact(GameplayTags.Socket_Tail))
	{
		return GetMesh()->GetSocketLocation("Tail");
	}

	return FVector();
}

bool AXeCharacter::GetIsDead_Implementation() const
{
	return IsDead;
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

