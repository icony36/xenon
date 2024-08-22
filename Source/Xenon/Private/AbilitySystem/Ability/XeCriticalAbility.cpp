// Copyright CoNY. All rights reserved.


#include "AbilitySystem/Ability/XeCriticalAbility.h"

#include "AbilitySystem/XeAbilitySystemComponent.h"
#include "Interface/CombatInterface.h"

UXeCriticalAbility::UXeCriticalAbility()
{
	bPassiveAbility = true;
}

void UXeCriticalAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* AvatarCharacter = GetAvatarActorFromActorInfo();
	
	if (AvatarCharacter->Implements<UCombatInterface>())
	{
		ICombatInterface::Execute_AddCriticalData(AvatarCharacter, AbilityTags.First(), CriticalChance, CriticalRate);
	}
}

void UXeCriticalAbility::ReceiveDeactivate(const FGameplayTag& AbilityTag)
{
	Super::ReceiveDeactivate(AbilityTag);

	if (AbilityTags.HasTagExact(AbilityTag))
	{
		AActor* AvatarCharacter = GetAvatarActorFromActorInfo();
		
		if (AvatarCharacter->Implements<UCombatInterface>())
		{
			ICombatInterface::Execute_RemoveCriticalData(AvatarCharacter, AbilityTags.First());
		}
	}
}

