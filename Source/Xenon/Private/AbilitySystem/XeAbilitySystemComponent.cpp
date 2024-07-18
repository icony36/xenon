// Copyright CoNY. All rights reserved.


#include "AbilitySystem/XeAbilitySystemComponent.h"

#include "AbilitySystem/Ability/XeAbility.h"

void UXeAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AbilityClasses)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass: AbilityClasses)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UXeAbility* XeAbility = Cast<UXeAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(XeAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
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
