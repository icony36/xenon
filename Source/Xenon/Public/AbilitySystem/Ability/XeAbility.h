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
	// Input Tag only for startup.
	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartupInputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effects")
	TArray<FTaggedMontage> AbilityMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effects")
	TArray<USoundBase*> AbilitySound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effects")
	TArray<UNiagaraSystem*> AbilityEffects;
};
