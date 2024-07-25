// Copyright CoNY. All rights reserved.


#include "UI/WidgetController/XeOverlayWidgetController.h"

#include "AbilitySystem/Data/LevelInfo.h"
#include "AbilitySystem/XeAbilitySystemComponent.h"
#include "AbilitySystem/XeAttributeSet.h"
#include "Player/XePlayerState.h"

void UXeOverlayWidgetController::BindCallbacksToDependencies()
{
	/** Bind to Attribute Changed Delegate */
	XeAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(XeAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChangedDelegate.Broadcast(Data.NewValue);
		}
	);

	XeAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(XeAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChangedDelegate.Broadcast(Data.NewValue);
		}
	);

	XeAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(XeAttributeSet->GetHealthRegenAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthRegenChangedDelegate.Broadcast(Data.NewValue);
		}
	);

	XeAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(XeAttributeSet->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChangedDelegate.Broadcast(Data.NewValue);
		}
	);

	XeAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(XeAttributeSet->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChangedDelegate.Broadcast(Data.NewValue);
		}
	);

	XeAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(XeAttributeSet->GetManaRegenAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaRegenChangedDelegate.Broadcast(Data.NewValue);
		}
	);

	XeAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(XeAttributeSet->GetDamageAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnDamageChangedDelegate.Broadcast(Data.NewValue);
		}
	);

	XeAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(XeAttributeSet->GetArmorAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnArmorChangedDelegate.Broadcast(Data.NewValue);
		}
	);

	XeAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(XeAttributeSet->GetMovementSpeedAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMovementSpeedChangedDelegate.Broadcast(Data.NewValue);
		}
	);
	
	
	/** Bind to Player State Changed Delegate */
	XePlayerState->OnExperienceChangedDelegate.AddLambda(
		[this](const int32 NewEXP)
		{
			OnEXPPercentChangedDelegate.Broadcast(GetEXPPercent(NewEXP));
		}
	);
	
	XePlayerState->OnCombatLevelChangedDelegate.AddLambda(
		[this](const int32 NewLevel)
		{
			OnCombatLevelChangedDelegate.Broadcast(NewLevel);
		}
	);

	XePlayerState->OnSkillPointChangedDelegate.AddLambda(
		[this](const int32 SkillPoint)
		{
			OnSkillPointChangedDelegate.Broadcast(SkillPoint);
		}
	);
}
	

void UXeOverlayWidgetController::BroadcastInitialValues()
{
	// Broadcast the initial values of attributes.
	OnHealthChangedDelegate.Broadcast(XeAttributeSet->GetHealth());
	OnMaxHealthChangedDelegate.Broadcast(XeAttributeSet->GetMaxHealth());
	OnHealthRegenChangedDelegate.Broadcast(XeAttributeSet->GetHealthRegen());
	OnManaChangedDelegate.Broadcast(XeAttributeSet->GetMana());
	OnMaxManaChangedDelegate.Broadcast(XeAttributeSet->GetMaxMana());
	OnManaRegenChangedDelegate.Broadcast(XeAttributeSet->GetManaRegen());
	OnDamageChangedDelegate.Broadcast(XeAttributeSet->GetDamage());
	OnArmorChangedDelegate.Broadcast(XeAttributeSet->GetArmor());
	OnMovementSpeedChangedDelegate.Broadcast(XeAttributeSet->GetMovementSpeed());

	// Broadcast the initial values of player state.
	OnCombatLevelChangedDelegate.Broadcast(XePlayerState->GetCombatLevel());
	OnEXPPercentChangedDelegate.Broadcast(GetEXPPercent(XePlayerState->GetExperience()));
	OnSkillPointChangedDelegate.Broadcast(XePlayerState->GetSkillPoint());
}

float UXeOverlayWidgetController::GetEXPPercent(const int32 NewEXP) const
{
	const ULevelInfo* LevelInfo = XePlayerState->GetLevelInfo();
	
	checkf(LevelInfo, TEXT("PlayerLevelInfo is not set in BP_XePlayerState."));

	// Get current level with accumulated EXP.
	const int32 CurrentLevel = LevelInfo->FindLevelWithEXP(NewEXP);

	// Check if current level is between max level and 0.
	if (CurrentLevel <= LevelInfo->LevelUpInformation.Num() && CurrentLevel > 0)
	{
		// Get current level up requirement.
		const int32 CurrentLevelUpRequirement = LevelInfo->LevelUpInformation[CurrentLevel].LevelUpRequirement;

		// Get level up requirement of previous level.
		const int32 PreviousLevelUpRequirement = LevelInfo->LevelUpInformation[CurrentLevel-1].LevelUpRequirement;

		// Calculate the difference of previous level requirement and current level requirement.
		const int32 DeltaLevelUpRequirement = CurrentLevelUpRequirement - PreviousLevelUpRequirement;

		// Calculate the current EXP.
		const int32 EXPForThisLevel = NewEXP - PreviousLevelUpRequirement;

		// Get EXP percent
		const float EXPBarPercent = static_cast<float>(EXPForThisLevel) / static_cast<float>(DeltaLevelUpRequirement);
	
		return EXPBarPercent;
	}

	return 0.f;
}
