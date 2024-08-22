// Copyright CoNY. All rights reserved.


#include "AbilitySystem/Ability/XePlayerAttack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "XeGameplayTags.h"
#include "AbilitySystem/XeAbilitySystemComponent.h"

void UXePlayerAttack::CauseDamage(AActor* TargetActor, const float Damage, const bool bShouldReactToHit,
                                  const FGameplayTag& HitReactTag) const
{
	if (TargetActor == nullptr) return;
	
	// Get ability system component of this ability.
	UXeAbilitySystemComponent* XeASC = Cast<UXeAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());

	// Send gameplay event to trigger other abilities that will modifier normal attack (e.g.: slow, poison).
	FGameplayEventData EventData;
	EventData.EventTag = FXeGameplayTags::Get().Event_Attack;
	EventData.Target = TargetActor;
	XeASC->HandleGameplayEvent(EventData.EventTag, &EventData);
	
	Super::CauseDamage(TargetActor, Damage, bShouldReactToHit, HitReactTag);
}
