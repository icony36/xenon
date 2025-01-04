// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "XeAbilitySystemComponent.generated.h"

class UXeAttackModifierAbility;
DECLARE_MULTICAST_DELEGATE_OneParam(FDeactivatePassiveAbility, const FGameplayTag& /*AbilityTag*/);

/**
 * 
 */
UCLASS()
class XENON_API UXeAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	FDeactivatePassiveAbility DeactivatePassiveAbility;
	
	FGameplayAbilitySpecHandle AddAbility(const TSubclassOf<UGameplayAbility>& AbilityClass);
	void RemoveAbility(const FGameplayAbilitySpecHandle& AbilitySpecHandle);

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	bool HasAbilityWithTag(const FGameplayTag& AbilityTag);

	TArray<TWeakObjectPtr<UXeAttackModifierAbility>> AttackModifierAbilities;
};
