// Copyright CoNY. All rights reserved.


#include "AbilitySystem/Ability/XeAbilityBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/XeAbilitySystemComponent.h"

void UXeAbilityBase::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (bPassiveAbility)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}

void UXeAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (bPassiveAbility)
	{
		if (UXeAbilitySystemComponent* XeASC = Cast<UXeAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo()))
		{
			XeASC->DeactivatePassiveAbility.AddUObject(this, &UXeAbilityBase::ReceiveDeactivate);
		}
	}
}

void UXeAbilityBase::ReceiveDeactivate(const FGameplayTag& AbilityTag)
{
	if (AbilityTags.HasTagExact(AbilityTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}
