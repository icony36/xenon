// Copyright CoNY. All rights reserved.


#include "AbilitySystem/XeAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
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