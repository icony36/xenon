// Copyright CoNY. All rights reserved.


#include "XeGameplayTags.h"

#include "GameplayTagsManager.h"

// Declare explicitly for static variable.
FXeGameplayTags FXeGameplayTags::GameplayTags;

void FXeGameplayTags::InitializeNativeGameplayTags()
{
	/** Input Tags - Movement */
	GameplayTags.InputAction_Movement_Forward = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputAction.Movement.Forward"),
		FString("Input action tag for Forward action."));

	GameplayTags.InputAction_Movement_Backward = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputAction.Movement.Backward"),
		FString("Input action tag for Backward action."));

	GameplayTags.InputAction_Movement_Left = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputAction.Movement.Left"),
		FString("Input action tag for Left action."));

	GameplayTags.InputAction_Movement_Right = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputAction.Movement.Right"),
		FString("Input action tag for Right action."));
}
