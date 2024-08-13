// Copyright CoNY. All rights reserved.


#include "AbilitySystem/XeAbilitySystemLibrary.h"

#include "AbilitySystem/XeAttributeSet.h"
#include "Engine/OverlapResult.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/CombatInterface.h"
#include "Player/XePlayerState.h"
#include "UI/HUD/XeHUD.h"
#include "UI/WidgetController/XeWidgetController.h"

void UXeAbilitySystemLibrary::ShowDebugSphere(const UObject* WorldContextObject, const FVector& Center, const float Radius,
	const int32 Segments, const FLinearColor Color, const float LifeTime)
{
	DrawDebugSphere(
			GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull),
			Center,
			Radius,
			Segments,
			Color.ToFColor(true),
			false,
			LifeTime);
}

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

void UXeAbilitySystemLibrary::GetLiveCombatActorsWithinRadius(const UObject* WorldContextObject,
                                                         TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, const FVector& Center,
                                                         const float Radius, const bool bShowDebug, const float ShowDebugTime, const FLinearColor DebugColor)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;

		// Get overlap actors from World.
		World->OverlapMultiByObjectType(
			Overlaps,
			Center,
			FQuat::Identity,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			FCollisionShape::MakeSphere(Radius),
			SphereParams);

		// Draw debug sphere
		if (bShowDebug)
		{
			ShowDebugSphere(
				World,
				Center,
				Radius,
				24,
				DebugColor,
				ShowDebugTime);
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


AActor* UXeAbilitySystemLibrary::GetNearestCombatActor(const AActor* CenterActor, const TArray<AActor*>& ActorsToIgnore,
	const float Radius, const bool bShowDebug, const float ShowDebugTime, const FLinearColor DebugColor)
{
	// Get center location.
	const FVector Center = CenterActor->GetActorLocation();
	
	// Get all overlapping actors that is Combat Interface and not dead.
	TArray<AActor*> OverlappingActors;
	GetLiveCombatActorsWithinRadius(CenterActor, OverlappingActors, ActorsToIgnore, Center, Radius, bShowDebug, ShowDebugTime, DebugColor);

	// Get nearest actor.
	AActor* NearestActor = nullptr;
	float MinDistance = MAX_FLT;
	for (AActor* Actor : OverlappingActors)
	{
		// Calculate the distance between the center and the actor.
		float Distance = FVector::Dist(Center, Actor->GetActorLocation());

		// Check if this is the closest actor so far.
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			NearestActor = Actor;
		}
	}

	// Show debug on nearest actor.
	if (bShowDebug && NearestActor != nullptr)
	{
		ShowDebugSphere(
			GEngine->GetWorldFromContextObject(CenterActor, EGetWorldErrorMode::LogAndReturnNull),
			NearestActor->GetActorLocation(),
			50,
			8,
			DebugColor,
			ShowDebugTime);
	}
	
	return NearestActor;
}
