// Copyright CoNY. All rights reserved.


#include "Player/XePlayerState.h"

#include "AbilitySystem/XeAbilitySystemComponent.h"
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
