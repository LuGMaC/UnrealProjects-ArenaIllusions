// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums.h"
#include "BaseStanceComponent.generated.h"


UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENAPROJECT_API UBaseStanceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseStanceComponent();

	/** Base Stance Component
	/*	This is a class made to create Stance for allies and enemies, which contains:
	/*	Base Values for Damage Modifiers in each body part
	/*	Attack Modifiers for when attacking 
	/*	Level and Growth changes (to implement)
	/*  Two Effects and states denoting when they should Trigger
	/*  Effect Execution (This should be unique to each Style so probably to be implemented as a BlueprintNativeEvent
	/*  Reference to character that owns/equips the object of the stance
	/*  Reference to animationBP for that stance
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stance Name")
	FName StanceName;

	/**Damage Modifiers for the Parts System
	* Each value should be between 0 and 2, meaning 0% as the lowest possible and 200% as the highest
	*/

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stance Status | Parts Damage Modifiers")
	float HeadDmgModifier;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stance Status | Parts Damage Modifiers")
	float ChestDmgModifier;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stance Status | Parts Damage Modifiers")
	float RightArmDmgModifier;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stance Status | Parts Damage Modifiers")
	float LeftArmDmgModifier;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stance Status | Parts Damage Modifiers")
	float LegsDmgModifier;

	/**Attack Modifiers for the Combo and Skill System
	* Each value should be between 0.01 and 2, meaning 1% as the lowest possible and 200% as the highest
	* SPD Retention Rate - Determines how much of remaining SPD can be used in the next turn. Value between 0 and 3, as in, 0% and 300%
	*/

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stance Status | Attack Damage Modifiers")
	float HighAtkModifier;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stance Status | Attack Damage Modifiers")
	float MidAtkModifier;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stance Status | Attack Damage Modifiers")
	float RightAtkModifier;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stance Status | Attack Damage Modifiers")
	float LeftAtkModifier;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stance Status | Attack Damage Modifiers")
	float LowAtkModifier;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stance Status | SPD Retention Rate")
	float SPDRetentionRate;

	/**Level and Growth Parameters
	*  To Implement
	*/

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stance Level")
	int StanceLevel;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stance Effects | Effect A")
	EEffectType EffectATrigger; 
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stance Effects | Effect A")
	FText EffectADescription;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stance Effects | Effect A")
	FString EffectABattleLogDescription;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stance Effects | Effect B")
	EEffectType EffectBTrigger;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stance Effects | Effect B")
	FText EffectBDescription;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Stance Effects | Effect B")
	FString EffectBBattleLogDescription;

	/*Reference to AnimationBP or class for the Stance
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	class TSubclassOf<UAnimInstance> StanceAnimationBP;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//FUNCTIONS

	FORCEINLINE void SetEffectAType(EEffectType TriggerA) { EffectATrigger = TriggerA; }
	FORCEINLINE void SetEffectBType(EEffectType TriggerB) { EffectBTrigger = TriggerB; }

	FORCEINLINE EEffectType GetEffectAType() { return EffectATrigger; }
	FORCEINLINE EEffectType GetEffectBType() { return EffectBTrigger; }

	//Receives a reference from the character that triggered the Effect activation
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stance Effects | Effect A")
	void EffectAActivation(ABaseCharacter* TriggerCharacter);
	//Receives a reference from the character that triggered the Effect activation
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stance Effects | Effect B")
	void EffectBActivation(ABaseCharacter* TriggerCharacter);

	
		
};
