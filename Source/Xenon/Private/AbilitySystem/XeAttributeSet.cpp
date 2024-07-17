// Copyright CoNY. All rights reserved.


#include "AbilitySystem/XeAttributeSet.h"

#include "Net/UnrealNetwork.h"

UXeAttributeSet::UXeAttributeSet()
{
	// TODO: Setup TagsToAttributes
}

void UXeAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Register replicated variables.
	DOREPLIFETIME_CONDITION_NOTIFY(UXeAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UXeAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UXeAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UXeAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UXeAttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UXeAttributeSet, ManaRegen, COND_None, REPNOTIFY_Always);
}

void UXeAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	// Handle attributes that will be predicted.
	GAMEPLAYATTRIBUTE_REPNOTIFY(UXeAttributeSet, Health, OldHealth); 
}

void UXeAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	// Handle attributes that will be predicted.
	GAMEPLAYATTRIBUTE_REPNOTIFY(UXeAttributeSet, Mana, OldMana); 
}

void UXeAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	// Handle attributes that will be predicted.
	GAMEPLAYATTRIBUTE_REPNOTIFY(UXeAttributeSet, MaxHealth, OldMaxHealth);
}

void UXeAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	// Handle attributes that will be predicted.
	GAMEPLAYATTRIBUTE_REPNOTIFY(UXeAttributeSet, MaxMana, OldMaxMana);
}

void UXeAttributeSet::OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen) const
{
	// Handle attributes that will be predicted.
	GAMEPLAYATTRIBUTE_REPNOTIFY(UXeAttributeSet, HealthRegen, OldHealthRegen);
}

void UXeAttributeSet::OnRep_ManaRegen(const FGameplayAttributeData& OldManaRegen) const
{
	// Handle attributes that will be predicted.
	GAMEPLAYATTRIBUTE_REPNOTIFY(UXeAttributeSet, ManaRegen, OldManaRegen);
}
