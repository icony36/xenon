// Copyright CoNY. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XeProjectile.generated.h"

class UNiagaraSystem;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class XENON_API AXeProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AXeProjectile();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;

protected:
	virtual void BeginPlay() override;
	
	virtual void Destroyed() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category="XeAbility|Projectile")
	virtual void PlayImpact();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAudioComponent> LoopingSoundComponent;

private:
	bool bHasPlayImpact = false;
	
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	float LifeSpan = 5.f;

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<USoundBase> LoopingSound;
};
