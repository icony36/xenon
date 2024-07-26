// Copyright CoNY. All rights reserved.


#include "AbilitySystem/Actor/XeProjectile.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Xenon/Xenon.h"


AXeProjectile::AXeProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	// Setup sphere component for overlap.
	SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(SphereComponent);
	// SphereComponent->SetCollisionObjectType(ECC_Projectile);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovementComponent->InitialSpeed = 550.f;
	ProjectileMovementComponent->MaxSpeed = 550.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void AXeProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Set 
	SetReplicateMovement(true);
	SetLifeSpan(LifeSpan);

	// Bind to sphere overlap event.
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereOverlap);

	// Play looping sound.
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
}

void AXeProjectile::Destroyed()
{
	// In case destroyed before overlapping, e.g: life span ended, server destroyed before client overlapped.
	if (!bHasPlayImpact)
	{
		PlayImpact();
	}
	
	// Stop looping sound.
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	
	Super::Destroyed();
}

void AXeProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bHasPlayImpact)
	{
		PlayImpact();
	}
	
	// Server destroy.
	if (HasAuthority())
	{
		// Destroy();
	}
}

void AXeProjectile::PlayImpact()
{
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	}

	if (ImpactEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	}

	bHasPlayImpact = true;
}

