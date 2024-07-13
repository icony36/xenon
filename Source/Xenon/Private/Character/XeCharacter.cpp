// Copyright CoNY. All rights reserved.


#include "Character/XeCharacter.h"

// Sets default values
AXeCharacter::AXeCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AXeCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AXeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AXeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

