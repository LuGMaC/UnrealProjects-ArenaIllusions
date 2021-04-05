// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseStanceComponent.h"
#include "Enums.h"
#include "BaseCharacter.h"

// Sets default values for this component's properties
UBaseStanceComponent::UBaseStanceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	StanceName = FName(TEXT("Base Stance"));

	//Default Values for the DmgModifiers in the Parts System

	HeadDmgModifier = 1.2;	
	ChestDmgModifier = 1;
	RightArmDmgModifier = 1;
	LeftArmDmgModifier = 1;
	LegsDmgModifier = 1;

	/**Default Values for the Attack Modifiers for the Combo and Skill System
	*/

	HighAtkModifier = 1;
	MidAtkModifier = 1;
	RightAtkModifier = 1;
	LeftAtkModifier = 1;
	LowAtkModifier = 1;
	SPDRetentionRate = 1;

	/** Default Level and Effect Triggers
	*  
	*/
	StanceLevel = 1;

	EffectATrigger = EEffectType::EET_Imediate;
	EffectBTrigger = EEffectType::EET_Imediate;

	//StanceAnimationBlueprint = CreateDefaultSubobject< UAnimBlueprintGeneratedClass>(TEXT("DefaultAnimBP"));


}


// Called when the game starts
void UBaseStanceComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBaseStanceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBaseStanceComponent::EffectAActivation_Implementation(ABaseCharacter* TriggerCharacter)
{

}

void UBaseStanceComponent::EffectBActivation_Implementation(ABaseCharacter* TriggerCharacter)
{

}

