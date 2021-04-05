// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enums.h"
#include "CharacterAnimationManager.generated.h"

/**
 * 
 */
UCLASS()
class ARENAPROJECT_API UCharacterAnimationManager : public UAnimInstance
{
	GENERATED_BODY()
	
	/**This class will be the basis for managing most animation related affairs for a blueprintable character
	* 
	*
	*/

public:

	/** Variables
	* Character - Reference to CharacterBP which will be animated
	*/

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Character")
	class ABaseCharacter* Character;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	UAnimMontage* SkillMontageReference;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bDamagedOrBrokenBodyPartAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bCharacterIsBeingHurt;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bAttackDodge;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bCharacterDefeat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bCharacterSpecialDefeat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bCharacterRevival;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bCharacterCover;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bAlternativeHurtAnimation;

	/** Functions
	* Constructor to get reference to CharacterBP
	* Update Animation properties - Character Condition, Status, etc.
	* CriticalConfirm - Defines if a skill was a critical or not based on timing of a button - To implement
	* CallChangeStanceAnimNotify - Calls BaseCharacter function to Change Stance. Use after skill animation is done
	* PlayAnimation_MontagePlay - Will play the correct skill and will bind functions for animation cancelling (implemented on baseCharacter)
	* AnimationCancelling_OnMontageStop - Will confirm that the animation has stopped
	* AnimationCancelling_OnMontageEnd - Will permit that the AnimBP can be changed
	*/

	virtual void NativeInitializeAnimation() override; 

	UFUNCTION(BlueprintCallable, Category = "AnimationProperties")
	void UpdateAnimationProperties();

	UFUNCTION(BlueprintCallable, Category = "AnimationProperties")
	void ResetAnimationBools();

	//UFUNCTION(BlueprintCallable, Category = "AnimationProperties")
	//void CriticalConfirm();

	UFUNCTION(BlueprintCallable, Category = "AnimationProperties")
	void CallChangeStanceAnimNotify();

	//UFUNCTION(BlueprintCallable, Category = "Animation")
	//void PlayAnimation_MontagePlay(UAnimMontage* Montage, FName SkillName);

	UFUNCTION(BlueprintCallable, Category = "AnimationCancelling")
	void AnimationCancelling_OnMontageStop();

	UFUNCTION(BlueprintCallable, Category = "AnimationCancelling")
	void AnimationCancelling_OnMontageEnd(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void SetupForBrokenBodyPartAnimation(EBodyPartTarget TargetBodyPart, EBodyPartStatus BodyPartNewCondition);

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void SetupAndPlayAnimationMontage(EBattleAnimationSetup MontageAnimToPlay, FName SpecialOrSkillName = "None");

	/*Plays an animation that does not call the BattleSystemManager to end the action
	* Hit animation, defeated animation, special defeat animation, revival animation and cover animation 
	* Any other types, this function will do nothing
	*/
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void PlayRegularAnimation(EBattleAnimationSetup AnimToPlay);

	//This function will receive the attack input and call SetupAndPlayAnimationMontage to animate the correct normal attack
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void SetupNormalAttackAnimation(EAttackType NormalAttackToAnimate);

	//Function that will be called upon any montage animation end
	void OnMontageAnimationEnd(UAnimMontage* Montage, bool bInterrupted);

};
