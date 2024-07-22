// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class XENON_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	int32 FindCombatLevelWithEXP(int32 InEXP) const;
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetEXP() const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSkillPoint() const;
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetSkillPointReward(int32 Level) const;
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToEXP(int32 InEXP);

	UFUNCTION(BlueprintNativeEvent)
	void AddToCombatLevel(int32 InCombatLevel);

	UFUNCTION(BlueprintNativeEvent)
	void AddToSkillPoint(int32 InSkillPoint);

	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();
};
