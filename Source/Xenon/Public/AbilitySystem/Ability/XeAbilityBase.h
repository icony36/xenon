// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "XeAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class XENON_API UXeAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartupInputTag; // * only for start up

	FORCEINLINE bool GetIsPassiveAbility() const { return bPassiveAbility; }
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void ReceiveDeactivate(const FGameplayTag& AbilityTag);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability")
	bool bPassiveAbility = false;
};
