// Copyright CoNY. All rights reserved.


#include "AbilitySystem/CustomCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "XeGameplayTags.h"
#include "AbilitySystem/XeAttributeSet.h"
#include "Interface/CombatInterface.h"

// Raw Struct (not FStruct) - to store captured Attribute
struct XeDamageStatics 
{
	// Declare attribute captures with macro.
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage); 
	
	
	XeDamageStatics()
	{
		// Define attribute captures with macro (AttributeSetClass, Attribute, AttributeOrigin, bShouldSnapshot).
		DEFINE_ATTRIBUTE_CAPTUREDEF(UXeAttributeSet, Damage, Source, true); 
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
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	/// Get source and target Ability System Component from incoming Params.
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	// Get Avatar from Ability System Component
	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	// Get source Level.
	int32 SourceCombatLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourceCombatLevel = ICombatInterface::Execute_GetCombatLevel(SourceAvatar);
	}

	// Get target Level.
	int32 TargetCombatLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetCombatLevel = ICombatInterface::Execute_GetCombatLevel(TargetAvatar);
	}

	// Get Gameplay Effect Spec from incoming Params.
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Get Gameplay Effect Context Handle from Gameplay Effect Spec.
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	// Get Damage from Set By Caller Magnitude.
	float Damage = Spec.GetSetByCallerMagnitude(FXeGameplayTags::Get().Damage, false);

	// TODO: Modify Damage.
	
	// Apply modified Damage.
	const FGameplayModifierEvaluatedData EvaluatedData(UXeAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
