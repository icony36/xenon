// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "XeAbilitySystemComponent.generated.h"

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
	
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AbilityClasses);

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	bool HasAbilityWithTag(const FGameplayTag& AbilityTag);
};
