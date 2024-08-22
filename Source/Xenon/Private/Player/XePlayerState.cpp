// Copyright CoNY. All rights reserved.


#include "Player/XePlayerState.h"

#include "AbilitySystem/XeAbilitySystemComponent.h"
#include "AbilitySystem/XeAbilitySystemLibrary.h"
#include "AbilitySystem/XeAttributeSet.h"
#include "Net/UnrealNetwork.h"

AXePlayerState::AXePlayerState()
{
	// Set to update faster (this value is used by Fortnite and Lyra Project).
	NetUpdateFrequency = 100.f;
	
	// Setup Ability System Component.
	AbilitySystemComponent = CreateDefaultSubobject<UXeAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	// Setup Attribute Set.
	AttributeSet = CreateDefaultSubobject<UXeAttributeSet>("AttributeSet");
}

void AXePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Register replicated variables.
	DOREPLIFETIME(AXePlayerState, CombatLevel);
	DOREPLIFETIME(AXePlayerState, Experience);
	DOREPLIFETIME(AXePlayerState, SkillPoint);
}

UAbilitySystemComponent* AXePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AXePlayerState::AddToCombatLevel(int32 Value)
{
	CombatLevel += Value;
	OnCombatLevelChangedDelegate.Broadcast(CombatLevel);
}

void AXePlayerState::AddToExperience(int32 Value)
{
	Experience += Value;
	OnExperienceChangedDelegate.Broadcast(Experience);
}

void AXePlayerState::AddToSkillPoint(int32 Value)
{
	SkillPoint += Value;
	OnSkillPointChangedDelegate.Broadcast(SkillPoint);
}

void AXePlayerState::SetCombatLevel(int32 InCombatLevel)
{
	CombatLevel = InCombatLevel;
	OnCombatLevelChangedDelegate.Broadcast(CombatLevel);
}

void AXePlayerState::SetExperience(int32 InExperience)
{
	Experience = InExperience;
	OnExperienceChangedDelegate.Broadcast(Experience);
}

void AXePlayerState::SetSkillPoint(int32 InSkillPoint)
{
	SkillPoint = InSkillPoint;
	OnSkillPointChangedDelegate.Broadcast(SkillPoint);
}

void AXePlayerState::AddCriticalData(const FGameplayTag& AbilityTag, const float CriticalChance,
	const float CriticalRate)
{
	FCriticalData CriticalData;
	CriticalData.CriticalChance = CriticalChance;
	CriticalData.CriticalRate = CriticalRate;

	CriticalDataContainer.Add(AbilityTag, CriticalData);
}

void AXePlayerState::RemoveCriticalData(const FGameplayTag& AbilityTag)
{
	CriticalDataContainer.Remove(AbilityTag);
}

void AXePlayerState::AddBlockData(const FGameplayTag& AbilityTag, const float BlockChance, const float BlockRate,
								  const bool bIsBlockRatePercentage)
{
	FBlockData BlockData;
	BlockData.BlockChance = BlockChance;
	BlockData.BlockRate = BlockRate;
	BlockData.bIsBlockRatePercentage = bIsBlockRatePercentage;

	BlockDataContainer.Add(AbilityTag, BlockData);
}

void AXePlayerState::RemoveBlockData(const FGameplayTag& AbilityTag)
{
	BlockDataContainer.Remove(AbilityTag);
}

FGameplayTag AXePlayerState::GetChosenCriticalData(FCriticalData& OutData)
{
	FGameplayTag AbilityTag;
	FCriticalData CriticalData;
	
	for(TTuple<FGameplayTag, FCriticalData> Pair: CriticalDataContainer)
	{
		if (UXeAbilitySystemLibrary::GetIsChanceSuccess(Pair.Value.CriticalChance))
		{
			if (Pair.Value.CriticalRate > CriticalData.CriticalRate)
			{
				AbilityTag = Pair.Key;
				CriticalData = Pair.Value;
			}
		}
	}

	if (AbilityTag.IsValid())
	{
		OutData = CriticalData;
	}

	return AbilityTag;
}

FGameplayTag AXePlayerState::GetChosenBlockData(FBlockData& OutData)
{
	FGameplayTag AbilityTag;
	FBlockData BlockData;
	float HighestValue = 0.f;

	float CurrentMaxHealth = 0.f;

	if (AbilitySystemComponent->HasAttributeSetForAttribute(UXeAttributeSet::GetMaxHealthAttribute()))
	{
		CurrentMaxHealth = AbilitySystemComponent->GetNumericAttribute(UXeAttributeSet::GetMaxHealthAttribute());
	}

	for(TTuple<FGameplayTag, FBlockData> Pair: BlockDataContainer)
	{
		if (UXeAbilitySystemLibrary::GetIsChanceSuccess(Pair.Value.BlockChance))
		{
			if (Pair.Value.bIsBlockRatePercentage)
			{
				const float Value = CurrentMaxHealth * Pair.Value.BlockRate;
				if (Value > HighestValue)
				{
					HighestValue = Value;
					AbilityTag = Pair.Key;
					BlockData = Pair.Value;
				}
			}
			else
			{
				if (Pair.Value.BlockRate > HighestValue)
				{
					HighestValue = Pair.Value.BlockRate;
					AbilityTag = Pair.Key;
					BlockData = Pair.Value;
				}
			}
		}
	}

	if (AbilityTag.IsValid())
	{
		OutData = BlockData;
	}
	
	return AbilityTag;
}

void AXePlayerState::OnRep_CombatLevel(int32 OldValue) const
{
	OnCombatLevelChangedDelegate.Broadcast(CombatLevel);
}

void AXePlayerState::OnRep_Experience(int32 OldValue) const
{
	OnExperienceChangedDelegate.Broadcast(Experience);
}

void AXePlayerState::OnRep_SkillPoint(int32 OldValue) const
{
	OnSkillPointChangedDelegate.Broadcast(SkillPoint);
}
