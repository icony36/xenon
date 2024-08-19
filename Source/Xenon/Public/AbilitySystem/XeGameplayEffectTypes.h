#pragma once

#include "GameplayEffectTypes.h"
#include "XeGameplayEffectTypes.generated.h"

USTRUCT(BlueprintType)
struct FXeGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FXeGameplayEffectContext* Duplicate() const override
	{
		FXeGameplayEffectContext* NewContext = new FXeGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}


	/** Getter */
	TSharedPtr<FGameplayTag> GetHitReactTag() const { return HitReactTag;}

	/** Setter */
	void SetHitReactTag(TSharedPtr<FGameplayTag> InHitReactTag) { HitReactTag = InHitReactTag; }
	
protected:
	// Implement additional data for Gameplay Effect Context here. (e.g.: IsBlockedHit, IsCriticalHit)
	TSharedPtr<FGameplayTag> HitReactTag; // No UPROPERTY as TSharedPtr handles the GC itself.
	
};

// Define what this Script Struct can do for reflection system and serialization.
template<>
struct TStructOpsTypeTraits< FXeGameplayEffectContext > : public TStructOpsTypeTraitsBase2< FXeGameplayEffectContext >
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};