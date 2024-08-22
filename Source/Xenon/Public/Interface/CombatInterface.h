// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, DeadActor);

// Struct to store all critical ability related data.
USTRUCT(BlueprintType)
struct FCriticalData
{
	GENERATED_BODY()

	FCriticalData(){}

	UPROPERTY()
	float CriticalChance = 0.f;

	UPROPERTY()
	float CriticalRate = 1.f;
};

// Struct to store all block ability related data.
USTRUCT(BlueprintType)
struct FBlockData
{
	GENERATED_BODY()
	
	FBlockData(){}

	UPROPERTY()
	float BlockChance = 0.f;

	UPROPERTY()
	float BlockRate = 0.f;

	UPROPERTY()
	bool bIsBlockRatePercentage = true;
};

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
	UAnimMontage* GetHitReactMontage(const FGameplayTag& HitReactTag);

	/**
	 * @param	RespawnTime		Time delay for character respawn. If < 0, no respawn.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) 
	void Die(float RespawnTime = -1.f);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddCriticalData(const FGameplayTag& AbilityTag, const float CriticalChance, const float CriticalRate);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddBlockData(const FGameplayTag& AbilityTag, const float BlockChance, const float BlockRate, const bool bIsBlockRatePercentage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RemoveCriticalData(const FGameplayTag& AbilityTag);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RemoveBlockData(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FGameplayTag GetChosenCriticalData(FCriticalData& OutData);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FGameplayTag GetChosenBlockData(FBlockData& OutData);
};
