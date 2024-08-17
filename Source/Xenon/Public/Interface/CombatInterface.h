// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

struct FEffectProperties;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, DeadActor);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class XENON_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	// BlueprintNativeEvent allow executes without casting, it automatically generates virtual native version that exist in C++ that can be overriden in C++.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)  
	AActor* GetAvatar();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) 
	FGameplayTag GetCharacterTag(); 
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) 
	FVector GetSocketLocation(const FGameplayTag& SocketTag);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetCombatLevel();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) 
	bool GetIsDead() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) 
	void ActivateHitReact(const FGameplayTag& HitReactTag, const FVector& Direction);

	/**
	 * @param	RespawnTime		Time delay for character respawn. If < 0, no respawn.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) 
	void Die(float RespawnTime = -1.f); 
};
