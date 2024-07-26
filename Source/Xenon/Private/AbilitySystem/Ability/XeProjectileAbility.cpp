// Copyright CoNY. All rights reserved.


#include "AbilitySystem/Ability/XeProjectileAbility.h"

#include "AbilitySystem/Actor/XeProjectile.h"
#include "Interface/CombatInterface.h"


void UXeProjectileAbility::SpawnProjectile(const FGameplayTag& SpawnSocketTag,
                                           bool bShouldOverridePitch, float PitchOverride)
{
	AActor* OwnerActor = GetAvatarActorFromActorInfo();
	
	// Only server can spawn projectile.
	if (!OwnerActor->HasAuthority()) return;

	// Get spawning location.
	const FVector SocketLocation = ICombatInterface::Execute_GetSocketLocation(OwnerActor, SpawnSocketTag);

	// Get spawning rotation.
	FRotator SpawnRotation = OwnerActor->GetActorForwardVector().Rotation();
	
	// Override projectile pitch if needed.
	if (bShouldOverridePitch)
	{
		SpawnRotation.Pitch = PitchOverride;
	}

	// Set spawn transform.
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(SpawnRotation.Quaternion());

	// Create projectile.
	AXeProjectile* Projectile = GetWorld()->SpawnActorDeferred<AXeProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	// TODO: add damage
	
	// Spawn projectile.
	Projectile->FinishSpawning(SpawnTransform);
}

