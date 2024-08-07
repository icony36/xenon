// Copyright CoNY. All rights reserved.


#include "AbilitySystem/XeAbilitySystemLibrary.h"

#include "AbilitySystem/XeAttributeSet.h"
#include "Engine/OverlapResult.h"
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

	// Make one WidgetController Params.
	if (MakeWidgetControllerParams(WorldContextObject, WidgetControllerParams, XeHUD))
	{
		// Return OverlayWidgetController from HUD. HUD will create one if it is null.
		return XeHUD->GetOverlayWidgetController(WidgetControllerParams);
	}

	return nullptr;
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

		// Get overlap actors from World.
		World->OverlapMultiByObjectType(
			Overlaps,
			SphereOrigin,
			FQuat::Identity,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			FCollisionShape::MakeSphere(Radius),
			SphereParams
		);

		// Draw debug sphere
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

		// Add non-dead overlapped actors to out array.
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
