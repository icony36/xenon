// Copyright CoNY. All rights reserved.


#include "UI/WidgetController/XeOverlayWidgetController.h"

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
	OnEXPPercentChangedDelegate.Broadcast(0.f);
	OnSkillPointChangedDelegate.Broadcast(XePlayerState->GetSkillPoint());
}

float UXeOverlayWidgetController::GetEXPPercent(const int32 NewEXP)
{
	// const ULevelUpInfo* LevelUpInfo = GetAuraPS()->LevelUpInfo;
	//
	// checkf(LevelUpInfo, TEXT("Unable to find LevelUpInfo. Please fill out AuraPlayerState Blueprint"));

	// const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);

	// if (Level <= LevelUpInfo->LevelUpInformation.Num() && Level > 0)
	// {
	// 	const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
	// 	const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level-1].LevelUpRequirement;
	// 	const int32 DeltaLevelUpRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
	//
	// 	const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;
	//
	// 	const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelUpRequirement);
	//
	// 	return XPBarPercent;
	// }

	return 0.f;
}
