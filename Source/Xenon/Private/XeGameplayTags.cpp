// Copyright CoNY. All rights reserved.


#include "XeGameplayTags.h"

#include "GameplayTagsManager.h"

// Declare explicitly for static variable.
FXeGameplayTags FXeGameplayTags::GameplayTags;

void FXeGameplayTags::InitializeNativeGameplayTags()
{
	/** Input Tags - Movement */
	GameplayTags.Input_Combat_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputAction.Combat.Attack"),
		FString("Input tag for Attack."));
}
