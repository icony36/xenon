// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "XeAbility.generated.h"

class UNiagaraSystem;
class UAnimMontage;

USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag EventTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SocketTag;
};

/**
 * 
 */
UCLASS()
class XENON_API UXeAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	//~ Input
	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartupInputTag; // * only for start up
	//~ end Input


	//~ Ability
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability")
	bool bIsPassive = false; // * ability system component will activate passive skill differently
	//~ end Ability


	//~ Damage
	UFUNCTION(BlueprintCallable, Category="XeAbility")
	virtual void CauseDamage(AActor* TargetActor, const bool bShouldUseDamageAttribute = false) const;
	
	UFUNCTION(BlueprintPure)
	virtual float GetDamageAtLevel() const;
	//~ end Damage
	
protected:
	UPROPERTY(BlueprintReadOnly, Category="XeAbility")
	TObjectPtr<AActor> OwnerActor;
	
	//~ Visual and Sound
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Visual and Sound")
	TArray<FTaggedMontage> AbilityMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Visual and Sound")
	TArray<USoundBase*> AbilitySound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Visual and Sound")
	TArray<UNiagaraSystem*> AbilityEffects;
	//~ end Visual and Sound
	
	
	//~ Damage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FScalableFloat SkillDamage;
	//~ end Damage
};
