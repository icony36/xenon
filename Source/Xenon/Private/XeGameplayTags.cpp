// Copyright CoNY. All rights reserved.


#include "XeGameplayTags.h"

#include "GameplayTagsManager.h"

// Declare explicitly for static variable.
FXeGameplayTags FXeGameplayTags::GameplayTags;

void FXeGameplayTags::InitializeNativeGameplayTags()
{
	/** Character Tags */
	GameplayTags.Character_Player= UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Character.Player"),
		FString("Character tag for Player."));


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



	/** Sockets */
	GameplayTags.Socket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
    	FName("Socket.LeftHand"),
    	FString("Socket tag for LeftHand."));

	GameplayTags.Socket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Socket.RightHand"),
		FString("Socket tag for RightHand."));

	GameplayTags.Socket_Tail = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Socket.Tail"),
		FString("Socket tag for Tail."));

	GameplayTags.Socket_WeaponTip = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Socket.WeaponTip"),
		FString("Socket tag for WeaponTip."));
	

	/** Data */
	GameplayTags.Data_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Data.Damage"),
		FString("Data Damage tag for Set By Caller Magnitude."));
	
	
	/** Attributes */
	GameplayTags.Attribute_Meta_IncomingEXP = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attribute.Meta.IncomingEXP"),
		FString("Attribute tag for IncomingEXP."));


	/** Events */
	GameplayTags.Event_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.HitReact"),
		FString("Event tag to trigger Hit React."));
	
	GameplayTags.Event_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.Attack"),
		FString("Event tag for attack triggered."));


	/** Abilities */
	GameplayTags.Ability_Active = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Active"),
		FString("Ability tag for Active Abilities."));

	GameplayTags.Ability_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Ability.Passive"),
		FString("Ability tag for Passive Abilities."));
		
    GameplayTags.Ability_NormalAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Ability.Active.NormalAttack"),
        FString("Ability tag for Normal Attack Ability."));
}
