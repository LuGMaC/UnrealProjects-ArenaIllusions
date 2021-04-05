// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimationManager.h"
#include "BaseCharacter.h"
#include "Animation/AnimInstance.h"
#include "BattleSystemManager.h"
#include "Enums.h"

void UCharacterAnimationManager::NativeInitializeAnimation()
{
	if (Character == nullptr)
	{
		Character = Cast<ABaseCharacter>(TryGetPawnOwner());

	}

	bCharacterIsBeingHurt = false;
	bDamagedOrBrokenBodyPartAnimation = false;
	bAttackDodge = false;
	bCharacterDefeat = false;
	bCharacterSpecialDefeat = false;
	bCharacterRevival = false;
	bCharacterCover = false;
	bAlternativeHurtAnimation = false;
	
}

void UCharacterAnimationManager::UpdateAnimationProperties()
{
	if (Character == nullptr)
	{
		Character = Cast<ABaseCharacter>(TryGetPawnOwner());
		
	}

	if (Character)
	{
		FVector2D LateralSpeed(Character->GetVelocity().X, Character->GetVelocity().Y); 
		MovementSpeed = LateralSpeed.Size(); 
		

		if (SkillMontageReference == nullptr)
		{
			SkillMontageReference = Character->SkillMontage;
		}

		
	}

}

 void UCharacterAnimationManager::ResetAnimationBools()
{
	 bCharacterIsBeingHurt = false;
	 bDamagedOrBrokenBodyPartAnimation = false;
	 bAttackDodge = false;
	 bCharacterCover = false;
	 //bCharacterDefeat = false;
	 //bCharacterSpecialDefeat = false;
	 //bCharacterRevival = false;
	 
}

void UCharacterAnimationManager::CallChangeStanceAnimNotify()
{
	if (Character == nullptr)
	{
		Character = Cast<ABaseCharacter>(TryGetPawnOwner());

	}

	if (Character->CheckStanceChange(Character->SpecialAction))
	{
		//In case there's a need to change stances
		//Call function to stop animation
		AnimationCancelling_OnMontageStop();
	}

	else
	{
		//In case there's no need to change stances
		//Let the animation finish as normal
	
	}



	//Character->ChangeStanceAfterAnimNotify(false);
}


void UCharacterAnimationManager::AnimationCancelling_OnMontageStop()
{
	if (Character == nullptr)
	{
		Character = Cast<ABaseCharacter>(TryGetPawnOwner());

	}

	UE_LOG(LogTemp, Warning, TEXT(" Animation Stop function"));

	//Creates and set a Delegate for when the animation is cancelled (TO FIX)
	//FOnMontageEnded CompleteDelegate;
	//CompleteDelegate.BindUObject(this, &UCharacterAnimationManager::AnimationCancelling_OnMontageEnd);
	//Character->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(CompleteDelegate, Character->SkillMontage);

	//calls function to stop montage
	if (Character->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
		Character->GetMesh()->GetAnimInstance()->Montage_Stop(0.1f, Character->SkillMontage);
		UE_LOG(LogTemp, Warning, TEXT(" STOP animation"));
		
	}

	AnimationCancelling_OnMontageEnd(Character->SkillMontage, false);

}


void UCharacterAnimationManager::AnimationCancelling_OnMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	UE_LOG(LogTemp, Warning, TEXT(" Animation Cancel function"));
	Character->ChangeStanceAfterAnimNotify(false);
}

void UCharacterAnimationManager::SetupForBrokenBodyPartAnimation(EBodyPartTarget TargetBodyPart, EBodyPartStatus BodyPartNewCondition)
{
	//to implement properly if there's different animations for each broken/hurt body part
	bDamagedOrBrokenBodyPartAnimation = true;

}

void UCharacterAnimationManager::SetupAndPlayAnimationMontage(EBattleAnimationSetup MontageAnimToPlay, FName SpecialOrSkillName)
{
	UE_LOG(LogTemp, Warning, TEXT(" Base Character Anim Manager - Attack Montage Setup "));

	if (Character == nullptr)
	{
		Character = Cast<ABaseCharacter>(TryGetPawnOwner());
		
	}

	if (SkillMontageReference == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT(" Base Character Anim Manager - Skill Montage Nullptr in SetupAndPlay "));
		SkillMontageReference = Character->SkillMontage;

	}

	UE_LOG(LogTemp, Warning, TEXT(" Base Character Anim Manager - Attack Montage Play "));
	Montage_Play(SkillMontageReference, 1.0f);

	UE_LOG(LogTemp, Warning, TEXT(" Base Character Anim Manager - End Delegate setup "));
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &UCharacterAnimationManager::OnMontageAnimationEnd);

	//Character->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate);
	Montage_SetEndDelegate(EndDelegate, SkillMontageReference);

	switch (MontageAnimToPlay)
	{
		case EBattleAnimationSetup::EBAS_AttackHigh:
			Montage_JumpToSection("AttackHigh", SkillMontageReference);
			break;
		case EBattleAnimationSetup::EBAS_AttackMid:
			Montage_JumpToSection("AttackMid", SkillMontageReference);
			break;
		case EBattleAnimationSetup::EBAS_AttackLow:
			Montage_JumpToSection("AttackLow", SkillMontageReference);
			break;
		case EBattleAnimationSetup::EBAS_AttackRight:
			Montage_JumpToSection("AttackRight", SkillMontageReference);
			break;
		case EBattleAnimationSetup::EBAS_AttackLeft:
			Montage_JumpToSection("AttackLeft", SkillMontageReference);
			break;
		case EBattleAnimationSetup::EBAS_Skill:
		case EBattleAnimationSetup::EBAS_Special:
			Montage_JumpToSection(SpecialOrSkillName, SkillMontageReference);
			break;
		case EBattleAnimationSetup::EBAS_Item:
			Montage_JumpToSection("Item", SkillMontageReference);
			break;
		case EBattleAnimationSetup::EBAS_Counter:
			Montage_JumpToSection("Counter", SkillMontageReference);
			break;
		case EBattleAnimationSetup::EBAS_Run:
			Montage_JumpToSection("Run", SkillMontageReference);
			break;
		default:
			break;
	}

}

void UCharacterAnimationManager::SetupNormalAttackAnimation(EAttackType NormalAttackToAnimate)
{
	UE_LOG(LogTemp, Warning, TEXT(" Base Character Anim Manager - Attack Anim Setup "));
	switch (NormalAttackToAnimate)
	{
		case EAttackType::EAT_High:
			SetupAndPlayAnimationMontage(EBattleAnimationSetup::EBAS_AttackHigh);
			break;
		case EAttackType::EAT_Mid:
			SetupAndPlayAnimationMontage(EBattleAnimationSetup::EBAS_AttackMid);
			break;
		case EAttackType::EAT_Low:
			SetupAndPlayAnimationMontage(EBattleAnimationSetup::EBAS_AttackLow);
			break;
		case EAttackType::EAT_Right:
			SetupAndPlayAnimationMontage(EBattleAnimationSetup::EBAS_AttackRight);
			break;
		case EAttackType::EAT_Left:
			SetupAndPlayAnimationMontage(EBattleAnimationSetup::EBAS_AttackLeft);
			break;
		default:
			break;
	}
}

void UCharacterAnimationManager::PlayRegularAnimation(EBattleAnimationSetup AnimToPlay)
{
	UE_LOG(LogTemp, Warning, TEXT(" Base Character Anim Manager - Play Regular Animation "));

	if (Character == nullptr)
	{
		Character = Cast<ABaseCharacter>(TryGetPawnOwner());

	}

	if (SkillMontageReference == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT(" Base Character Anim Manager - Skill Montage Nullptr in Play Regular Animation "));
		SkillMontageReference = Character->SkillMontage;

	}

	UE_LOG(LogTemp, Warning, TEXT(" Base Character Anim Manager -Regular Animation Play "));

	switch (AnimToPlay)
	{
		case EBattleAnimationSetup::EBAS_Hurt:
			if (!bAlternativeHurtAnimation)
			{
				Montage_Play(SkillMontageReference, 1.0f);
				Montage_JumpToSection("Hurt", SkillMontageReference);
				bAlternativeHurtAnimation = true;
			}
			else 
			{
				Montage_Play(SkillMontageReference, 1.0f);
				Montage_JumpToSection("Hurt2", SkillMontageReference);
				bAlternativeHurtAnimation = false;
			}
			break;
		case EBattleAnimationSetup::EBAS_Defeat:
		case EBattleAnimationSetup::EBAS_SpecialDefeat:
			//Defeat state is continuous, thus we need to remain in the correct state. 
			bCharacterDefeat = true;
			bCharacterRevival = false;
			break;
		case EBattleAnimationSetup::EBAS_Cover:
			Montage_Play(SkillMontageReference, 1.0f);
			Montage_JumpToSection("Cover", SkillMontageReference);
			break;
		case EBattleAnimationSetup::EBAS_Revive:
			bCharacterRevival = true; 
			bCharacterDefeat = false;
			break;

		case EBattleAnimationSetup::EBAS_Dodge:
			Montage_Play(SkillMontageReference, 1.0f);
			Montage_JumpToSection("Dodge", SkillMontageReference);
			break;

		default:

			break;
	}

}

void UCharacterAnimationManager::OnMontageAnimationEnd(UAnimMontage* Montage, bool bInterrupted)
{

	UE_LOG(LogTemp, Warning, TEXT(" Base Character Anim Manager - Attack Montage End "));

	if (Character == nullptr)
	{
		Character = Cast<ABaseCharacter>(TryGetPawnOwner());

	}

	if (Character)
	{
		if (!Montage_IsPlaying(Montage))
		{
			Character->BattleSystemReference->AnimationEndDelayFunction();
			//Character->BattleSystemReference->AnimationEnd();
		}
		

		/* LEGACY CODE

		//If the character is doing a combo, go to the next combo attack
		if (Character->CharacterAction == ECharacterAction::ECA_Attack)
		{
			UE_LOG(LogTemp, Warning, TEXT(" Base Character Anim Manager - Combo Continue "));

			//Stance change if necessary
			Character->ChangeStanceAfterAnimNotify(false);

			Character->CurrentComboAttack++;
			Character->NextComboAttack();
		}

		//if it isn't a combo, it is a single action. We can end the action
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT(" Base Character Anim Manager - Action End "));

			Character->BattleSystemReference->ActionEnd();
		}

		*/
	}



}