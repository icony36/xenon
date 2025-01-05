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

	// Get Gameplay Effect Spec from incoming Params.
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	// Get Gameplay Effect Context Handle from Gameplay Effect Spec.
	const FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	// Create Evaluation Parameters for captured Attributes.
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// Get Damage from Set By Caller Magnitude.
	float Damage = Spec.GetSetByCallerMagnitude(FXeGameplayTags::Get().Data_Damage, false);
	
	// TODO: Get floating text from execution param
	// TODO: Spawn floating text here (critical or block)
	
	/** Modify normal attack damage with Attack Modifier Ability. */
	FGameplayTag AttackModifierEvent;
	if (UXeAbilitySystemLibrary::GetAbilityTag(EffectContextHandle).MatchesTag(FXeGameplayTags::Get().Ability_NormalAttack))
	{
		
		// TODO: Determine attack modifier application sequence logic
		for(const TWeakObjectPtr<UXeAttackModifierAbility> AttackModifierAbility: SourceAsc->AttackModifierAbilities)
		{
			if (AttackModifierAbility->ApplyAttackModifier(Damage, Damage))
			{
				AttackModifierEvent = UXeAbilitySystemLibrary::GetTagWithParent(AttackModifierAbility->AbilityTags, FXeGameplayTags::Get().Ability);;
				break;
			}
		}
	}

	// TODO: Execute DefenseModifiers from TargetAsc
	/** Modify all damage with Defensive Modifier Ability. */
	
	/** Modify damage with Block. */
	// TODO: Modify Damage - Armor
	
	// Capture Armor on target.
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, TargetArmor);

	// Get Armor Coefficient from curve table.
	/***/

	// Send Events
	if (AttackModifierEvent.IsValid())
	{
		FGameplayEventData EventData;
		EventData.EventTag = AttackModifierEvent;
		EventData.EventMagnitude = Damage;
		EventData.Instigator = SourceAvatar;
		EventData.Target = TargetAvatar;
		SourceAsc->HandleGameplayEvent(EventData.EventTag, &EventData);
	}

	// Apply modified Damage.
	if (Damage > 0.f)
	{
		// Send Damage Dealt Event to attacker for abilities that need to do post-damage dealt effect.
		FGameplayEventData SourceEventData;
		SourceEventData.EventTag = FXeGameplayTags::Get().Event_DamageDealt;
		SourceEventData.EventMagnitude = Damage;
		SourceEventData.Instigator = SourceAvatar;
		SourceEventData.Target = TargetAvatar;
		SourceAsc->HandleGameplayEvent(SourceEventData.EventTag, &SourceEventData);

		// Send Damage Taken Event to victim for abilities that need to do post-damage received effect.
		FGameplayEventData TargetEventData;
		TargetEventData.EventTag = FXeGameplayTags::Get().Event_DamageTaken;
		TargetEventData.EventMagnitude = Damage;
		TargetEventData.Instigator = SourceAvatar;
		TargetEventData.Target = TargetAvatar;
		TargetAsc->HandleGameplayEvent(TargetEventData.EventTag, &TargetEventData);

		// Apply Damage.
		const FGameplayModifierEvaluatedData EvaluatedData(UXeAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
		OutExecutionOutput.AddOutputModifier(EvaluatedData);
	}
}
