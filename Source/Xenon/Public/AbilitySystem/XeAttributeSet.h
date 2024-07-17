// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "XeAttributeSet.generated.h"

// Macros to create accessor functions (get, set, init) for Attributes.
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


/**
 * 
 */
UCLASS()
class XENON_API UXeAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UXeAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	//~ Vital Attributes
	UPROPERTY(BlueprintReadOnly, Category="Vital Attributes", ReplicatedUsing=OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UXeAttributeSet, Health);
	UPROPERTY(BlueprintReadOnly, Category="Vital Attributes", ReplicatedUsing=OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UXeAttributeSet, Mana);
	UPROPERTY(BlueprintReadOnly, Category="Vital Attributes", ReplicatedUsing=OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UXeAttributeSet, MaxHealth);
	UPROPERTY(BlueprintReadOnly, Category="Vital Attributes", ReplicatedUsing=OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UXeAttributeSet, MaxMana);
	UPROPERTY(BlueprintReadOnly, Category="Vital Attributes", ReplicatedUsing=OnRep_HealthRegen)
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(UXeAttributeSet, HealthRegen);
	UPROPERTY(BlueprintReadOnly, Category="Vital Attributes", ReplicatedUsing=OnRep_ManaRegen)
	FGameplayAttributeData ManaRegen;
	ATTRIBUTE_ACCESSORS(UXeAttributeSet, ManaRegen);

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
	UFUNCTION()
	void OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen) const;
	UFUNCTION()
	void OnRep_ManaRegen(const FGameplayAttributeData& OldManaRegen) const;
	//~ end Vital Attributes
	
};
