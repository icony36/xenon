// Copyright CoNY. All rights reserved.


#include "XeGameplayTags.h"

#include "GameplayTagsManager.h"

// Declare explicitly for static variable.
FXeGameplayTags FXeGameplayTags::GameplayTags;

void FXeGameplayTags::InitializeNativeGameplayTags()
{
	/** Input Tags */
	GameplayTags.Input_Combat_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Combat.Attack"),
		FString("Input tag for Attack."));

	GameplayTags.Input_Combat_Skill1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Combat.Skill1"),
		FString("Input tag for Skill1."));

	GameplayTags.Input_Combat_Skill2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Combat.Skill2"),
		FString("Input tag for Skill2."));
	
	GameplayTags.Input_Combat_Skill3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Combat.Skill3"),
		FString("Input tag for Skill3."));

	GameplayTags.Input_Combat_Skill4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Combat.Skill4"),
		FString("Input tag for Skill4."));
	
}
