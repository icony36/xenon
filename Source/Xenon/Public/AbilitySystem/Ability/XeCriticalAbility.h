// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/XeAbilityBase.h"
#include "XeCriticalAbility.generated.h"

/**
 * 
 */
UCLASS()
class XENON_API UXeCriticalAbility : public UXeAbilityBase
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void ReceiveDeactivate(const FGameplayTag& AbilityTag) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin = "0.0", ClampMax = "1.0"), Category="Critical")
	float CriticalChance = 0.f; // TODO: Make it scalable float

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin = "1.0"), Category="Critical")
	float CriticalRate = 1.f; // TODO: Make it scalable float
};
