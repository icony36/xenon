// Copyright CoNY. All rights reserved.


#include "AbilitySystem/CustomCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "XeGameplayTags.h"
#include "AbilitySystem/XeAbilitySystemComponent.h"
#include "AbilitySystem/XeAbilitySystemLibrary.h"
#include "AbilitySystem/XeAttributeSet.h"
#include "AbilitySystem/XeGameplayEffectTypes.h"
#include "AbilitySystem/Ability/XeAttackModifierAbility.h"
#include "Interface/CombatInterface.h"

// Raw Struct (not FStruct) - to store captured Attribute
struct XeDamageStatics 
{
	// Declare attribute captures with macro.
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor); 
	
	
	XeDamageStatics()
	{
		// Define attribute captures with macro (AttributeSetClass, Attribute, AttributeOrigin, bShouldSnapshot).
		DEFINE_ATTRIBUTE_CAPTUREDEF(UXeAttributeSet, Armor, Target, true); 
	}
};

// Static function for getting XeDamageStatics.
static const XeDamageStatics& DamageStatics()
{
	// Create static XeDamageStatics variable.
	static XeDamageStatics DStatics;

	return DStatics; // * it will always return the same DStatics
}

UExecCalc_Damage::UExecCalc_Damage()
{
	// Add Attributes to Capture List.
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	/// Get source and target Ability System Component from incoming Params.
	UXeAbilitySystemComponent* SourceAsc = StaticCast<UXeAbilitySystemComponent*>(ExecutionParams.GetSourceAbilitySystemComponent());
	UXeAbilitySystemComponent* TargetAsc = StaticCast<UXeAbilitySystemComponent*>(ExecutionParams.GetTargetAbilitySystemComponent());

	// Get Avatar from Ability System Component
	AActor* SourceAvatar = SourceAsc ? SourceAsc->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetAsc ? TargetAsc->GetAvatarActor() : nullptr;

	// Get source Level and Critical Data.
	int32 SourceLevel = 1;
	// FCriticalData CriticalData;
	// FGameplayTag CriticalAbilityTag;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourceLevel = ICombatInterface::Execute_GetCombatLevel(SourceAvatar);
		// CriticalAbilityTag = ICombatInterface::Execute_GetChosenCriticalData(SourceAvatar, CriticalData);
	}

	// Get target Level and Block Data.
	int32 TargetLevel = 1;
	// FBlockData BlockData;
	// FGameplayTag BlockAbilityTag;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetLevel = ICombatInterface::Execute_GetCombatLevel(TargetAvatar);
		// BlockAbilityTag = ICombatInterface::Execute_GetChosenBlockData(TargetAvatar, BlockData);
	}

	// Get Gameplay Effect Spec from incoming Params.
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	// Get Gameplay Effect Context Handle from Gameplay Effect Spec.
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	// Create Evaluation Parameters for captured Attributes.
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// Get Damage from Set By Caller Magnitude.
	float Damage = Spec.GetSetByCallerMagnitude(FXeGameplayTags::Get().Data_Damage, false);

	
	// /** Modify damage with Critical Ability. */
	// if (CriticalAbilityTag.IsValid())
	// {
	// 	Damage *= CriticalData.CriticalRate;
	// 	UXeAbilitySystemLibrary::SetCriticalAbilityTag(EffectContextHandle, CriticalAbilityTag);
	// }
	// /***/
	//
	//
	// /** Modify damage with Block Ability. */
	// if (BlockAbilityTag.IsValid())
	// {
	// 	if (BlockData.bIsBlockRatePercentage)
	// 	{
	// 		Damage *= (1-BlockData.BlockRate);
	// 	}
	// 	else
	// 	{
	// 		Damage -= BlockData.BlockRate;
	// 	}
	//
	// 	UXeAbilitySystemLibrary::SetBlockAbilityTag(EffectContextHandle, BlockAbilityTag);
	// }

	// TODO: Removed Critical and Block Attribute
	// TODO: Get floating text from execution param
	// TODO: Spawn floating text here (critical or block)

	
	if (UXeAbilitySystemLibrary::GetAbilityTag(EffectContextHandle).MatchesTag(FXeGameplayTags::Get().Ability_NormalAttack))
	{
		for(const TWeakObjectPtr<UXeAttackModifierAbility> AttackModifierAbility: SourceAsc->AttackModifierAbilities)
		{
			if (AttackModifierAbility->ApplyAttackModifier(Damage, Damage))
			{
				FGameplayEventData EventData;
				EventData.EventTag = AttackModifierAbility->AbilityTags.First();
				EventData.EventMagnitude = Damage;
				SourceAsc->HandleGameplayEvent(EventData.EventTag, &EventData);
				break;
			}
		}
	}

	// TODO: Execute DefenseModifiers from TargetAsc

	
	/** Modify damage with Block. */
	// TODO: Modify Damage - Armor
	
	// Capture Armor on target.
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, TargetArmor);

	// Get Armor Coefficient from curve table.
	/***/
	

	// Apply modified Damage.
	if (Damage > 0.f)
	{
		const FGameplayModifierEvaluatedData EvaluatedData(UXeAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
		OutExecutionOutput.AddOutputModifier(EvaluatedData);
	}
}
