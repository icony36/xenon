// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 *  Singleton 
 */

struct FXeGameplayTags
{
public:
 // Singleton Getter
 static const FXeGameplayTags& Get() { return GameplayTags; }

 // Initialize all native gameplay tags.
 static void InitializeNativeGameplayTags();

 
 //~ Input Tags
 FGameplayTag Input_Combat_Attack;
 FGameplayTag Input_Combat_Skill1;
 FGameplayTag Input_Combat_Skill2;
 FGameplayTag Input_Combat_Skill3;
 FGameplayTag Input_Combat_Skill4;
 //~ end Input Tags

private:
 static FXeGameplayTags GameplayTags;
}; 
