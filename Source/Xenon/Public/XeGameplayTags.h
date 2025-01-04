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


  //~ Character Tags
  FGameplayTag Character_Player;
  //~ end Character Tags
  
  
  //~ Input Tags
  FGameplayTag Input_Combat_Attack;
  FGameplayTag Input_Combat_Skill1;
  FGameplayTag Input_Combat_Skill2;
  FGameplayTag Input_Combat_Skill3;
  FGameplayTag Input_Combat_Skill4;
  //~ end Input Tags
  
  
  //~ Sockets
  FGameplayTag Socket_LeftHand;
  FGameplayTag Socket_RightHand;
  FGameplayTag Socket_Tail;
  FGameplayTag Socket_WeaponTip;
  //~ end Sockets

  
  //~ Data Tags
  FGameplayTag Data_Damage;
  //~ end Data Tags


  //~ Attributes (for Listen For Event Gameplay Ability)
  FGameplayTag Attribute_Meta_IncomingEXP; // Data_Attribute_IncomingEXP
  //~ end Attributes


  //~ Event Tags
  FGameplayTag Event_HitReact;
  FGameplayTag Event_Attack;
  //~ end Event Tags


  //~ Ability Tags
   FGameplayTag Ability_Active;
   FGameplayTag Ability_Passive;
   FGameplayTag Ability_NormalAttack;
  //~ end Ability Tags
 
private:
 static FXeGameplayTags GameplayTags;
}; 
