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

	
	/** Bind to Player State Changed Delegate */
	XePlayerState->OnExperienceChangedDelegate.AddUObject(this, &UXeOverlayWidgetController::OnEXPChanged);
	XePlayerState->OnCombatLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel)
		{
			OnCombatLevelChangedDelegate.Broadcast(NewLevel);
		}
	);
}
	

void UXeOverlayWidgetController::BroadcastInitialValues()
{
	// Broadcast the initial values of attributes.
	OnHealthChangedDelegate.Broadcast(XeAttributeSet->GetHealth());
	OnMaxHealthChangedDelegate.Broadcast(XeAttributeSet->GetMaxHealth());
	OnManaChangedDelegate.Broadcast(XeAttributeSet->GetMana());
	OnMaxManaChangedDelegate.Broadcast(XeAttributeSet->GetMaxMana());
}

void UXeOverlayWidgetController::OnEXPChanged(const int32 NewEXP) const
{
	// const ULevelUpInfo* LevelUpInfo = GetAuraPS()->LevelUpInfo;
	//
	// checkf(LevelUpInfo, TEXT("Unabled to find LevelUpInfo. Please fill out AuraPlayerState Blueprint"));

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
	// 	OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	// }

	const float EXPPercent = static_cast<float>(NewEXP); 
	OnEXPPercentChangedDelegate.Broadcast(EXPPercent);
}
