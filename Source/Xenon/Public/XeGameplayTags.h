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
 FGameplayTag InputAction_Movement_Forward;
 FGameplayTag InputAction_Movement_Backward;
 FGameplayTag InputAction_Movement_Left;
 FGameplayTag InputAction_Movement_Right;
 FGameplayTag InputAction_Combat_Attack;
 FGameplayTag InputAction_Combat_Skill1;
 FGameplayTag InputAction_Combat_Skill2;
 FGameplayTag InputAction_Combat_Skill3;
 FGameplayTag InputAction_Combat_Skill4;
 //~ end Input Tags

private:
 static FXeGameplayTags GameplayTags;
};
