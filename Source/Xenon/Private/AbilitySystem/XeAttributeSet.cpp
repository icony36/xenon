// Copyright CoNY. All rights reserved.


#include "AbilitySystem/XeAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/XeAbilitySystemComponent.h"
#include "AbilitySystem/XeAbilitySystemLibrary.h"
#include "AbilitySystem/Data/LevelInfo.h"
#include "GameFramework/Character.h"
#include "Interface/CombatInterface.h"
#include "Interface/PlayerInterface.h"
#include "Net/UnrealNetwork.h"

UXeAttributeSet::UXeAttributeSet()
{
	// TODO: Setup TagsToAttributes
}

void UXeAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Register replicated variables.
	DOREPLIFETIME_CONDITION_NOTIFY(UXeAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UXeAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UXeAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UXeAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UXeAttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UXeAttributeSet, ManaRegen, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UXeAttributeSet, Damage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UXeAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UXeAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UXeAttributeSet, AttackSpeed, COND_None, REPNOTIFY_Always);
}

void UXeAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// Clamp Health.
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}

	// Clamp Mana.
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UXeAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// Create Effect Properties that holds effect related data (instigator and victim data).
	FEffectProperties Props;
	MakeEffectProperties(Data, Props); // * variables in Props might be null, check before using

	// Clamp Health.
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}

	// Clamp Mana.
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Props);
	}

	if (Data.EvaluatedData.Attribute == GetIncomingEXPAttribute())
	{
		HandleIncomingEXP(Props);
	}
}

void UXeAttributeSet::MakeEffectProperties(const FGameplayEffectModCallbackData& Data,
                                           FEffectProperties& OutProperties)
{
	/**
	 * Source = Causer of the effect
	 * Target = Target of the effect (owner of this Attribute Set)
	 */

	// Set Effect Context Handle from Effect Spec of incoming Data.
	OutProperties.EffectContextHandle = Data.EffectSpec.GetContext();

	// Set Source Ability System Component from Effect Context Handle.
	OutProperties.SourceASC = OutProperties.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	// Check if Source Ability System, its Ability Actor Info and its Avatar Actor is valid.
	if (IsValid(OutProperties.SourceASC) && OutProperties.SourceASC->AbilityActorInfo.IsValid() && OutProperties.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		// Set Source Avatar Actor from Source Ability System.
		OutProperties.SourceAvatarActor = OutProperties.SourceASC->AbilityActorInfo->AvatarActor.Get();

		// Set Source Controller from Source Ability System
		OutProperties.SourceController = OutProperties.SourceASC->AbilityActorInfo->PlayerController.Get();

		// If we can't get the Source Controller from Source Ability System, get it from Source Avatar Actor.
		if (OutProperties.SourceController == nullptr && OutProperties.SourceAvatarActor != nullptr)
		{
			const APawn* Pawn = Cast<APawn>(OutProperties.SourceAvatarActor);
			if (Pawn != nullptr)
			{
				OutProperties.SourceController = Pawn->GetController();
			}
		}

		// If Source Controller is not null, set the Source Character from Source Controller.
		if (OutProperties.SourceController != nullptr)
		{
			OutProperties.SourceCharacter = Cast<ACharacter>(OutProperties.SourceController->GetPawn());
		}
	}

	// Check if Target (owner of this Attribute Set) Ability Actor Info and its Avatar Actor is valid.
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		// Set Target Avatar Actor from incoming Data Target.
		OutProperties.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();

		// Set Target Controller from incoming Data Target.
		OutProperties.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();

		// Set Target Character from Target Avatar Actor.
		OutProperties.TargetCharacter = Cast<ACharacter>(OutProperties.TargetAvatarActor);

		// Set Target Ability System from Target Avatar.
		OutProperties.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OutProperties.TargetAvatarActor);
	}

	/** Variables in OutProperties might be null, check before using. */
}

void UXeAttributeSet::HandleIncomingDamage(const FEffectProperties& Properties)
{
	// Cached incoming damage.
	const float LocalIncomingDamage = GetIncomingDamage();

	// Zero out meta attribute after used.
	SetIncomingDamage(0.f);

	if (LocalIncomingDamage > 0.f)
	{
		// Reduce health based on incoming damage.
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

		// If it kills the character.
		if (NewHealth <= 0.f)
		{
			if (Properties.TargetCharacter->Implements<UCombatInterface>())
			{
				ICombatInterface::Execute_Die(Properties.TargetCharacter, Properties);
			}
		
			SendEXPEvent(Properties);
		}
	}
}

void UXeAttributeSet::HandleIncomingEXP(const FEffectProperties& Properties)
{
	// Cached incoming EXP.
	const float LocalIncomingEXP = GetIncomingEXP();

	// Zero out meta attribute after used.
	SetIncomingEXP(0.f);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Incoming EXP: %f"), LocalIncomingEXP));

	// Source character is the owner, GA_ListenForEvents applies GE_EventBasedEffect, adding to IncomingXP
	if (Properties.SourceCharacter->Implements<UPlayerInterface>() && Properties.SourceCharacter->Implements<UCombatInterface>())
	{
		// Get source character current level and current EXP.
		const int32 CurrentLevel = ICombatInterface::Execute_GetCombatLevel(Properties.SourceCharacter);
		const int32 CurrentEXP = IPlayerInterface::Execute_GetEXP(Properties.SourceCharacter);

		// Get new level with incoming EXP.
		const int32 NewLevel = IPlayerInterface::Execute_FindCombatLevelWithEXP(Properties.SourceCharacter, CurrentEXP + LocalIncomingEXP);

		// Check if there is difference between new level and current level.
		const int32 NumLevelUps = NewLevel - CurrentLevel;

		// If source character can level up.
		if (NumLevelUps > 0)
		{
			// Add level.
			IPlayerInterface::Execute_AddToCombatLevel(Properties.SourceCharacter, NumLevelUps);

			// Get level up rewards based on how many level ups.
			int32 SkillPointReward = 0;
			float DamageReward = 0.f;
			float ArmorReward = 0.f;
			float MaxHealthReward = 0.f;
			float MaxManaReward = 0.f;
			for (int32 i=0; i<NumLevelUps; i++)
			{
				FLevelUpProperties LevelUpProps = IPlayerInterface::Execute_GetLevelUpProperties(Properties.SourceCharacter, CurrentLevel + i);
				SkillPointReward += LevelUpProps.SkillPointReward;
				DamageReward += LevelUpProps.DamageReward;
				ArmorReward += LevelUpProps.ArmorReward;
				MaxHealthReward += LevelUpProps.MaxHealthReward;
				MaxManaReward += LevelUpProps.MaxManaReward;
			}

			// Add skill points.
			IPlayerInterface::Execute_AddToSkillPoint(Properties.SourceCharacter, SkillPointReward);

			// Add damage and armor.
			SetDamage(GetDamage() + DamageReward);
			SetArmor(GetArmor() + ArmorReward);

			// Add health and mana.
			SetMaxHealth(GetMaxHealth() + MaxHealthReward);
			SetHealth(GetHealth() + MaxHealthReward);
			SetMaxMana(GetMaxMana() + MaxManaReward);
			SetMana(GetMana() + MaxManaReward);

			// Call level up.
			IPlayerInterface::Execute_LevelUp(Properties.SourceCharacter);
		}

		// Add EXP.
		IPlayerInterface::Execute_AddToEXP(Properties.SourceCharacter, LocalIncomingEXP);
	}
}

void UXeAttributeSet::SendEXPEvent(const FEffectProperties& Properties)
{
	if (Properties.TargetCharacter->Implements<UCombatInterface>())
	{
		// Get target level.
		const int32 TargetLevel = ICombatInterface::Execute_GetCombatLevel(Properties.TargetCharacter);

		// Get target character tag.
		const FGameplayTag TargetTag = ICombatInterface::Execute_GetCharacterTag(Properties.TargetCharacter);

		// Get target character EXP reward.
		const int32 EXPReward = UXeAbilitySystemLibrary::GetEXPReward(Properties.TargetCharacter, TargetTag, TargetLevel);

		// Send EXP to source character (character that kills the target character).
		UXeAbilitySystemLibrary::SendEXP(Properties.TargetCharacter, Properties.SourceCharacter, EXPReward);
	}
}

void UXeAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	// Handle attributes that will be predicted.
	GAMEPLAYATTRIBUTE_REPNOTIFY(UXeAttributeSet, Health, OldHealth); 
}

void UXeAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	// Handle attributes that will be predicted.
	GAMEPLAYATTRIBUTE_REPNOTIFY(UXeAttributeSet, Mana, OldMana); 
}

void UXeAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	// Handle attributes that will be predicted.
	GAMEPLAYATTRIBUTE_REPNOTIFY(UXeAttributeSet, MaxHealth, OldMaxHealth);
}

void UXeAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	// Handle attributes that will be predicted.
	GAMEPLAYATTRIBUTE_REPNOTIFY(UXeAttributeSet, MaxMana, OldMaxMana);
}

void UXeAttributeSet::OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen) const
{
	// Handle attributes that will be predicted.
	GAMEPLAYATTRIBUTE_REPNOTIFY(UXeAttributeSet, HealthRegen, OldHealthRegen);
}

void UXeAttributeSet::OnRep_ManaRegen(const FGameplayAttributeData& OldManaRegen) const
{
	// Handle attributes that will be predicted.
	GAMEPLAYATTRIBUTE_REPNOTIFY(UXeAttributeSet, ManaRegen, OldManaRegen);
}

void UXeAttributeSet::OnRep_Damage(const FGameplayAttributeData& OldDamage) const
{
	// Handle attributes that will be predicted.
	GAMEPLAYATTRIBUTE_REPNOTIFY(UXeAttributeSet, Damage, OldDamage);
}

void UXeAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	// Handle attributes that will be predicted.
	GAMEPLAYATTRIBUTE_REPNOTIFY(UXeAttributeSet, Armor, OldArmor);
}

void UXeAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed) const
{
	// Handle attributes that will be predicted.
	GAMEPLAYATTRIBUTE_REPNOTIFY(UXeAttributeSet, MovementSpeed, OldMovementSpeed);
}

void UXeAttributeSet::OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed) const
{
	// Handle attributes that will be predicted.
	GAMEPLAYATTRIBUTE_REPNOTIFY(UXeAttributeSet, AttackSpeed, OldAttackSpeed);
}