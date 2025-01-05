// Copyright CoNY. All rights reserved.


#include "Subsystem/PoolSubsystem.h"

void UPoolSubsystem::SpawnFromPool(const TSubclassOf<AActor> PoolClass, const FVector Location, const FRotator Rotation,
	AActor*& SpawnedActor)
{
	SpawnedActor = SpawnFromPool<AActor>(PoolClass, Location, Rotation);
}

void UPoolSubsystem::ReturnToPool(AActor* PoolObject)
{
	const UClass* PoolableClass = PoolObject->GetClass();
	if (PoolableClass->ImplementsInterface(UPoolableInterface::StaticClass()))
	{
		IPoolableInterface::Execute_OnReturnToPool(PoolObject);
		FPoolArray* ObjectPool = ObjectPools.Find(PoolableClass);
		ObjectPool->Add(PoolObject);
	}
	else
	{
		PoolObject->Destroy();
	}
}
