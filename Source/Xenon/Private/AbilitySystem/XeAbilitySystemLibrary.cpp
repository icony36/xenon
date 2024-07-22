// Copyright CoNY. All rights reserved.


#include "AbilitySystem/XeAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "XeGameplayTags.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystem/Data/CharacterInfo.h"
#include "Engine/OverlapResult.h"
#include "Game/XeGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/CombatInterface.h"
#include "Player/XePlayerState.h"
#include "UI/HUD/XeHUD.h"
#include "UI/WidgetController/XeWidgetController.h"

bool UXeAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject,
	FWidgetControllerParams& OutWidgetControllerParams, AXeHUD*& OutXeHUD)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutXeHUD = PlayerController->GetHUD<AXeHUD>();
		if (OutXeHUD)
		{
			AXePlayerState* XePlayerState = PlayerController->GetPlayerState<AXePlayerState>();
			UAbilitySystemComponent* AbilitySystemComponent = XePlayerState->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = XePlayerState->GetAttributeSet();

			OutWidgetControllerParams.PlayerState = XePlayerState;
			OutWidgetControllerParams.PlayerController = PlayerController;
			OutWidgetControllerParams.AbilitySystemComponent = AbilitySystemComponent;
			OutWidgetControllerParams.AttributeSet = AttributeSet;
			
			return true;
		}
	}

	return false;
}

UXeOverlayWidgetController* UXeAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WidgetControllerParams;
	AXeHUD* XeHUD = nullptr;

	if (MakeWidgetControllerParams(WorldContextObject, WidgetControllerParams, XeHUD))
	{
		return XeHUD->GetOverlayWidgetController(WidgetControllerParams);
	}

	return nullptr;
}

UCharacterInfo* UXeAbilitySystemLibrary::GetCharacterInfo(const UObject* WorldContextObject)
{
	const AXeGameModeBase* XeGameModeBase = Cast<AXeGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (XeGameModeBase == nullptr) return nullptr;

	return XeGameModeBase->CharacterInfo;
}

int32 UXeAbilitySystemLibrary::GetEXPReward(const UObject* WorldContextObject, const FGameplayTag& CharacterTag,
	int32 CharacterLevel)
{
	UCharacterInfo* CharacterInfo = GetCharacterInfo(WorldContextObject);
	if (CharacterInfo == nullptr) return 0;

	const FCharacterProperties Info = CharacterInfo->GetCharacterProperties(CharacterTag);
	const float EXPReward = Info.EXPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(EXPReward);
}

void UXeAbilitySystemLibrary::SendEXP(const UObject* WorldContextObject, AActor* Recipient, float InEXP)
{
	// Setup payload to send (EXP tag, EXP reward amount).
	const FXeGameplayTags& GameplayTags = FXeGameplayTags::Get();
	FGameplayEventData Payload;
	Payload.EventTag = GameplayTags.Attribute_Meta_IncomingEXP;
	Payload.EventMagnitude = InEXP;

	// Send EXP to target actor through Gameplay Event.
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Recipient, GameplayTags.Attribute_Meta_IncomingEXP, Payload);
}

void UXeAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
                                                         TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, const FVector& SphereOrigin,
                                                         float Radius, bool bShowDebug)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;

		World->OverlapMultiByObjectType(
			Overlaps,
			SphereOrigin,
			FQuat::Identity,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			FCollisionShape::MakeSphere(Radius),
			SphereParams
		);

		if (bShowDebug)
		{
			DrawDebugSphere(
				World,
				SphereOrigin,
				Radius,
				12,
				FColor::Red,
				false,
				2
			);
		}

		for (FOverlapResult& Overlap : Overlaps)
		{
			// Check if the overlapped actor is Combat Interface and not dead.
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_GetIsDead(Overlap.GetActor())) // use UCombatInterface in Implements
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}
