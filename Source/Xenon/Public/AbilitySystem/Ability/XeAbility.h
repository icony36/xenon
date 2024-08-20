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
	FGameplayTag CastSocket;
};

/**
 * 
 */
UCLASS()
class XENON_API UXeAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	//~ Input
	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartupInputTag; // * only for start up
	//~ end Input


	//~ Ability
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability")
	bool bIsPassive = false; // * ability system component will activate passive skill differently
	//~ end Ability
	

	//~ Montage
	UFUNCTION(BlueprintPure, Category="XeAbility")
	FTaggedMontage GetMontageToPlay(const bool bRandomOrder = false);

	UFUNCTION(BlueprintPure, Category="XeAbility")
	FName GetCurrentMontageSection() const;
	//~ end Montage 


	//~ Transform
	UFUNCTION(BlueprintPure, Category="XeAbility")
	FVector GetActorLocation() const;

	UFUNCTION(BlueprintPure, Category="XeAbility")
	FVector GetActorForwardVector() const;

	UFUNCTION(BlueprintPure, Category="XeAbility")
	FVector GetCastAtFrontLocation() const;

	UFUNCTION(BlueprintCallable, Category="XeAbility")
	void RotateToFace(AActor* TargetActor) const;
	//~ end Transform
	
protected:
	//~ Cast
	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="XeAbility")
	void RotateToFaceNearestCombatActor(const TArray<AActor*>& ActorsToIgnore, const bool bShowDebug = false, const float ShowDebugTime = 1.f, const FLinearColor DebugColor = FLinearColor::Green) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability")
	float CombatActorDetectRadius = 500.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability")
	float CastRadius = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability")
	float CastRange = 100.f;
	//~ end Cast

	
	//~ Montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation")
	TArray<FTaggedMontage> AbilityMontages;
	//~ end Montage

	
	//~ VFX and SFX
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="VFX & SFX")
	TArray<USoundBase*> AbilitySFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="VFX & SFX")
	TArray<UNiagaraSystem*> AbilityVFX;
	//~ VFX and SFX

private:
	//~ Montage
	int32 CurrentMontageIndex = 0;
	//~ end Montage
};
