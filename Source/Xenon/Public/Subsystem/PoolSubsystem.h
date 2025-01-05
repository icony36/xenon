// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interface/PoolableInterface.h"
#include "Subsystems/WorldSubsystem.h"
#include "PoolSubsystem.generated.h"

class UPoolableInterface;

USTRUCT()
struct FPoolArray
{
	GENERATED_BODY()

	UPROPERTY() // Use struct to store this to prevent garbage collection
	TArray<AActor*> ObjectPool;

	bool IsEmpty() const
	{
		return ObjectPool.IsEmpty();
	}

	AActor* Pop()
	{
		return ObjectPool.Pop();
	}

	void Add(AActor* ActorToAdd)
	{
		ObjectPool.Add(ActorToAdd);
	}
};

/**
 * 
 */
UCLASS()
class XENON_API UPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category="Pool Subsystem", meta=(DeterminesOutputType="PoolClass", DynamicOutputParam="SpawnedActor"))
	void SpawnFromPool(const TSubclassOf<AActor> PoolClass, const FVector Location, const FRotator Rotation, AActor*& SpawnedActor);

	// Template function to return the desired class.
	template<typename T>
	T* SpawnFromPool(TSubclassOf<AActor> PoolClass, FVector Location, FRotator Rotation);
	
	UFUNCTION(BlueprintCallable, Category="Pool Subsystem")
	void ReturnToPool(AActor* PoolObject);

	UPROPERTY()
	TMap<UClass*, FPoolArray> ObjectPools;
};

template<typename T>
T* UPoolSubsystem::SpawnFromPool(const TSubclassOf<AActor> PoolClass, const FVector Location, const FRotator Rotation)
{
	T* PooledObject = nullptr;

	if (PoolClass.Get()->ImplementsInterface(UPoolableInterface::StaticClass()))
	{
		FPoolArray& ObjectPool = ObjectPools.FindOrAdd(PoolClass);
		if (ObjectPool.IsEmpty())
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			PooledObject = GetWorld()->SpawnActor<T>(PoolClass, Location, Rotation, SpawnParams);
		}
		else
		{
			PooledObject = CastChecked<T>(ObjectPool.Pop());
			PooledObject->SetActorLocationAndRotation(Location, Rotation);
		}

		IPoolableInterface::Execute_OnSpawnFromPool(PooledObject);
	}

	return PooledObject;
}
