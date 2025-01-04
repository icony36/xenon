// Copyright CoNY. All rights reserved.


#include "AbilitySystem/Ability/XeAbility.h"

#include "AbilitySystemComponent.h"
#include "XeGameplayTags.h"
#include "AbilitySystem/XeAbilitySystemComponent.h"
#include "AbilitySystem/XeAbilitySystemLibrary.h"
#include "GameFramework/Character.h"

void UXeAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
	
	if (AbilityTags.HasTag(FXeGameplayTags::Get().Ability_Passive))
	{
		bIsPassiveAbility = true;
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}

void UXeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (bIsPassiveAbility)
	{
		if (UXeAbilitySystemComponent* XeAsc = Cast<UXeAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo()))
		{
			XeAsc->DeactivatePassiveAbility.AddUObject(this, &UXeAbility::ReceiveDeactivate);
		}
	}
}

void UXeAbility::ReceiveDeactivate(const FGameplayTag& AbilityTag)
{
	if (AbilityTags.HasTagExact(AbilityTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

FTaggedMontage UXeAbility::GetMontageToPlay(const bool bRandomOrder)
{
	const int32 MontagesNum = AbilityMontages.Num();

	if (MontagesNum <= 1)
	{
		 return AbilityMontages[0];
	}

	// Get random Montage.
	if (bRandomOrder)
	{
		const int32 RandomNum = FMath::RandRange(0, MontagesNum - 1);

		return AbilityMontages[RandomNum];
	}

	// Get Montage based on order.
	const int32 LocalIndex = CurrentMontageIndex;
	
	if (CurrentMontageIndex < MontagesNum - 1)
	{
		CurrentMontageIndex++;
	}
	else
	{
		CurrentMontageIndex = 0;
	}	

	return AbilityMontages[LocalIndex];
}

FName UXeAbility::GetCurrentMontageSection() const
{
	if (const ACharacter* AvatarCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		const UAnimInstance* AnimInstance = AvatarCharacter->GetMesh()->GetAnimInstance();
		return AnimInstance->Montage_GetCurrentSection();
	}

	return FName();
}

FVector UXeAbility::GetActorLocation() const
{
	return GetAvatarActorFromActorInfo()->GetActorLocation();
}

FVector UXeAbility::GetActorForwardVector() const
{
	return GetAvatarActorFromActorInfo()->GetActorForwardVector();
}

FVector UXeAbility::GetCastAtFrontLocation() const
{
	return GetActorLocation() + (GetActorForwardVector() * CastRange);
}

void UXeAbility::RotateToFace(AActor* TargetActor) const
{
	if (TargetActor == nullptr)
	{
		return;
	}

	// Get the actor current rotation.
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	const FRotator AvatarRotation = AvatarActor->GetActorRotation();

	// Calculate the direction to the target.
	FVector Direction = TargetActor->GetActorLocation() - AvatarActor->GetActorLocation();
	Direction.Z = 0.0f; // * ignore Z axis for horizontal rotation

	// Calculate the desired rotation to face the target
	const FRotator TargetRotation = Direction.Rotation();

	// Set the player's rotation to face the target
	AvatarActor->SetActorRotation(FRotator(AvatarRotation.Pitch, TargetRotation.Yaw, AvatarRotation.Roll));
}

void UXeAbility::RotateToFaceNearestCombatActor(const TArray<AActor*>& ActorsToIgnore, const bool bShowDebug, const float ShowDebugTime, const FLinearColor DebugColor) const
{
	const AActor* AvatarActor = GetAvatarActorFromActorInfo();

	// Get nearest actor to face.
	AActor* TargetActor = UXeAbilitySystemLibrary::GetNearestCombatActor(
		AvatarActor,
		ActorsToIgnore,
		CombatActorDetectRadius,
		bShowDebug,
		ShowDebugTime,
		DebugColor);

	// Rotate avatar actor.
	RotateToFace(TargetActor);
}