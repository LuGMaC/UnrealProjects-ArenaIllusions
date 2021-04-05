// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatCharacterV2.h"

// Sets default values
ACombatCharacterV2::ACombatCharacterV2()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACombatCharacterV2::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACombatCharacterV2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACombatCharacterV2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

