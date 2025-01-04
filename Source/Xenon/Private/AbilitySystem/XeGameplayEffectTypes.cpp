#include "AbilitySystem/XeGameplayEffectTypes.h"

bool FXeGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	// Initialize RepBits with 32 bits to store data.
	uint32 RepBits = 0;

	// Store data to RepBits with bitwise operations (OR & Shift Left).
	if (Ar.IsSaving())
	{
		if (bReplicateInstigator && Instigator.IsValid())
		{
			RepBits |= 1 << 0;
		}
		if (bReplicateEffectCauser && EffectCauser.IsValid() )
		{
			RepBits |= 1 << 1;
		}
		if (AbilityCDO.IsValid())
		{
			RepBits |= 1 << 2;
		}
		if (bReplicateSourceObject && SourceObject.IsValid())
		{
			RepBits |= 1 << 3;
		}
		if (Actors.Num() > 0)
		{
			RepBits |= 1 << 4;
		}
		if (HitResult.IsValid())
		{
			RepBits |= 1 << 5;
		}
		if (bHasWorldOrigin)
		{
			RepBits |= 1 << 6;
		}
		if (AbilityTag.IsValid())
		{
			RepBits |= 1 << 7;
		}
		if (HitReactTag.IsValid())
		{
			RepBits |= 1 << 8;
		}
	}

	// Serialize RepBits.
	Ar.SerializeBits(&RepBits, 9);

	// Store data to archive.
	if (RepBits & (1 << 0))
	{
		Ar << Instigator;
	}
	if (RepBits & (1 << 1))
	{
		Ar << EffectCauser;
	}
	if (RepBits & (1 << 2))
	{
		Ar << AbilityCDO;
	}
	if (RepBits & (1 << 3))
	{
		Ar << SourceObject;
	}
	if (RepBits & (1 << 4))
	{
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (RepBits & (1 << 5))
	{
		if (Ar.IsLoading())
		{
			if (!HitResult.IsValid())
			{
				HitResult = TSharedPtr<FHitResult>(new FHitResult());
			}
		}
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6))
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}
	if (RepBits & (1 << 7))
	{
		if (Ar.IsLoading())
		{
			if (!AbilityTag.IsValid())
			{
				AbilityTag = TSharedPtr<FGameplayTag>(new FGameplayTag());
			}
		}
		AbilityTag->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 8))
	{
		if (Ar.IsLoading())
		{
			if (!HitReactTag.IsValid())
			{
				HitReactTag = TSharedPtr<FGameplayTag>(new FGameplayTag());
			}
		}
		HitReactTag->NetSerialize(Ar, Map, bOutSuccess);
	}


	// Add instigator when archive is loading.
	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
	}	

	bOutSuccess = true;
	return true;
}
