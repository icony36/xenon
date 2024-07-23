// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelInfo.generated.h"

USTRUCT(BlueprintType)
struct FLevelUpProperties
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement = 0;
	
	UPROPERTY(EditDefaultsOnly)
	int32 SkillPointReward = 1;

	UPROPERTY(EditDefaultsOnly)
	float DamageReward  = 5;
	
	UPROPERTY(EditDefaultsOnly)
	float ArmorReward  = 1;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealthReward  = 10;
	
	UPROPERTY(EditDefaultsOnly)
	float MaxManaReward  = 10;
};


/**
 * 
 */
UCLASS()
class XENON_API ULevelInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FLevelUpProperties> LevelUpInformation;

	int32 FindLevelWithEXP(int32 EXP) const;
};
