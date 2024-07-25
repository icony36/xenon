// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "Engine/DataAsset.h"
#include "CharacterInfo.generated.h"


USTRUCT()
struct FCharacterProperties
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category="Class Defaults")
	FScalableFloat EXPReward = FScalableFloat();
};

/**
 * 
 */
UCLASS()
class XENON_API UCharacterInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, FCharacterProperties> CharacterPool;

	FCharacterProperties GetCharacterProperties(const FGameplayTag& CharacterTag);
	
};
