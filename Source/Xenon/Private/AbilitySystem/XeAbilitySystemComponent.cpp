// Copyright CoNY. All rights reserved.


#include "AbilitySystem/XeAbilitySystemComponent.h"

#include "AbilitySystem/Ability/XeAbility.h"
#include "AbilitySystem/Ability/XeAttackModifierAbility.h"

FGameplayAbilitySpecHandle UXeAbilitySystemComponent::AddAbility(const TSubclassOf<UGameplayAbility>& AbilityClass)
{
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
	if (UXeAbility* XeAbility = Cast<UXeAbility>(AbilitySpec.Ability))
	{
		AbilitySpec.DynamicAbilityTags.AddTag(XeAbility->StartupInputTag);

		if (UXeAttackModifierAbility* AttackModifierAbility = Cast<UXeAttackModifierAbility>(XeAbility))
		{
			AttackModifierAbilities.Add(AttackModifierAbility);
		}
	}

	return GiveAbility(AbilitySpec);
}

void UXeAbilitySystemComponent::RemoveAbility(const FGameplayAbilitySpecHandle& AbilitySpecHandle)
{
	const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(AbilitySpecHandle);
	if (UXeAttackModifierAbility* AttackModifierAbility = Cast<UXeAttackModifierAbility>(AbilitySpec->Ability))
	{
		AttackModifierAbilities.Remove(AttackModifierAbility);
	}

	DeactivatePassiveAbility.Broadcast(AbilitySpec->Ability->AbilityTags.First());
	
	ClearAbility(AbilitySpecHandle);
}

void UXeAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	// Lock Active Scope to prevent modification of Ability.
	FScopedAbilityListLock ActiveScopeLoc(*this);
	
	// Loop through all available Abilities.
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// If Ability has the Input Tag.
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) 
		{
			// Tell Ability Spec the input is pressed.
			AbilitySpecInputPressed(AbilitySpec); 

			// If Ability is activated. 
			if (AbilitySpec.IsActive()) 
			{
				// Tell server about the input pressed so WaitForInputPressed will work.
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			}

			break;
		}
	}
}

void UXeAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	// Lock Active Scope to prevent modification of Ability.
	FScopedAbilityListLock ActiveScopeLoc(*this);
	
	// Loop through all available Abilities.
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// If Ability has the Input Tag.
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) 
		{
			// Tell Ability Spec the input is pressed.
			AbilitySpecInputPressed(AbilitySpec);

			// If Ability is not activated yet. 
			if (!AbilitySpec.IsActive()) 
			{
				// Activate ability.
				TryActivateAbility(AbilitySpec.Handle); 
			}

			break;
		}
	}
}

void UXeAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	// Lock Active Scope to prevent modification of Ability.
	FScopedAbilityListLock ActiveScopeLoc(*this);
	
	// Loop through all available abilities.
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// If Ability has the Input Tag.
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) && AbilitySpec.IsActive()) 
		{
			// Tell Ability Spec the input is released.
			AbilitySpecInputReleased(AbilitySpec); 

			// Tell server about the input released so WaitForInputReleased will work.
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());

			break;
		}
	}
}

bool UXeAbilitySystemComponent::HasAbilityWithTag(const FGameplayTag& AbilityTag)
{
	if (!AbilityTag.IsValid()) return false;

	// Lock Active Scope to prevent modification of Ability.
	FScopedAbilityListLock ActiveScopeLoc(*this);
	
	// Loop through all available abilities.
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		const FGameplayTagContainer AbilityTags = AbilitySpec.Ability->AbilityTags;
		if (AbilityTags.IsValid() && AbilityTags.HasTagExact(AbilityTag) ) 
		{
			return true;
		}
	}

	return false;
}
