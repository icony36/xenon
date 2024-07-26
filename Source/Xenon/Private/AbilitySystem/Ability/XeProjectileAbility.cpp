// Copyright CoNY. All rights reserved.


#include "AbilitySystem/Ability/XeProjectileAbility.h"

#include "AbilitySystem/Actor/XeProjectile.h"
#include "Interface/CombatInterface.h"

void UXeProjectileAbility::SpawnProjectile(FRotator& SpawnRotation, const FGameplayTag& SpawnSocketTag,
	bool bShouldOverridePitch, float PitchOverride)
{
	// Only server can spawn projectile.
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;

	// Get spawning location.
	const FVector SocketLocation = ICombatInterface::Execute_GetSocketLocation(GetAvatarActorFromActorInfo(), SpawnSocketTag);
	

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

