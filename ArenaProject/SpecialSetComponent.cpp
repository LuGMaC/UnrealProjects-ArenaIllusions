// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialSetComponent.h"
#include "BaseCharacter.h"
#include "BattleSystemManager.h"
#include "Engine/DataTable.h"

// Sets default values for this component's properties
USpecialSetComponent::USpecialSetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpecialSetComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	CharacterReference = Cast<ABaseCharacter>(GetOwner()); 
	ValidSpecialArt = false;
	
}


// Called every frame
void USpecialSetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool USpecialSetComponent::ActivationConditionCheck(ESpecialArtActivationCondition ConditionToCheck, float ConditionQuantity)
{
	int Damage = 0;
	float DamagePercenteage = 0;

	switch(ConditionToCheck)
	{
		case ESpecialArtActivationCondition::ESAAC_Empty:
			return true;
			break;

		case ESpecialArtActivationCondition::ESAAC_DamageAboveX:
			Damage = (CharacterReference->MaxHealth) - (CharacterReference->CurrentHealth);
			if(Damage >= ConditionQuantity)
			{
				return true;
			}
			else
			{
				return false;
			}
			break;

		case ESpecialArtActivationCondition::ESAAC_DamageAboveMax:
			DamagePercenteage = (CharacterReference->CurrentHealth) / (CharacterReference->MaxHealth) * 100;
			if (DamagePercenteage >= ConditionQuantity)
			{
				return true;
			}
			else
			{
				return false;
			}
			break;

		case ESpecialArtActivationCondition::ESAAC_EnemyDamageAboveX:
			//This is considering the target enemy
			Damage = (CharacterReference->SingleTargetReference->MaxHealth) - (CharacterReference->SingleTargetReference->CurrentHealth);
			if (Damage >= ConditionQuantity)
			{
				return true;
			}
			else
			{
				return false;
			}
			break;
		
		case ESpecialArtActivationCondition::ESAAC_EnemyDamageAboveMax:
			DamagePercenteage = (CharacterReference->SingleTargetReference->CurrentHealth) / (CharacterReference->SingleTargetReference->MaxHealth) * 100;
			if (DamagePercenteage >= ConditionQuantity)
			{
				return true;
			}
			else
			{
				return false;
			}
			break;

		case ESpecialArtActivationCondition::ESAAC_DefeatedAlly:
			if (CharacterReference->BattleSystemReference->AreAllAlliesAlive())
			{
				//Are All Alies Alive returns true if there isn't one dead. Thus, Defeated Ally must return false.
				return false;
			}
			else
			{
				return true;
			}

			break;

		case ESpecialArtActivationCondition::ESAAC_DefeatedSelf:
			if (CharacterReference->CurrentHealth <= 0)
			{
				return true;
			}
			else
			{
				return false;
			}
			
			break;

		case ESpecialArtActivationCondition::ESAAC_AllAlliesAlive:
			//Are All Alies Alive returns true if there isn't one dead. 
			return CharacterReference->BattleSystemReference->AreAllAlliesAlive();
			break;

		case ESpecialArtActivationCondition::ESAAC_AllEnemiesAlive:
			//Are All Enemies Alive returns true if there isn't one dead. 
			return CharacterReference->BattleSystemReference->AreAllEnemiesAlive(); 
			break;

		default:
			return true;
			break;
	}
}


bool USpecialSetComponent::CanSpecialArtBeActivated(FSpecialArt* SpecialArtToActivate)
{
	bool bActiveCondition1 = ActivationConditionCheck(SpecialArtToActivate->ActivationCondition1, SpecialArtToActivate->ACQuantity1);
	bool bActiveCondition2 = ActivationConditionCheck(SpecialArtToActivate->ActivationCondition2, SpecialArtToActivate->ACQuantity2);
	bool bActiveCondition3 = ActivationConditionCheck(SpecialArtToActivate->ActivationCondition3, SpecialArtToActivate->ACQuantity3);

	return (bActiveCondition1 && bActiveCondition2 && bActiveCondition3);

}


void USpecialSetComponent::SetActiveSpecialArt(FName RowName)
{
	static const FString ContextString(TEXT("ContextString"));
	FSpecialArt* FoundSpecialArt = SpecialArtTable->FindRow<FSpecialArt>(RowName, ContextString);

	if (FoundSpecialArt)
	{
		UE_LOG(LogTemp, Warning, TEXT(" FoundSpecialArtFound"));

		bool bCanbeActivated = CanSpecialArtBeActivated(FoundSpecialArt);

		if (bCanbeActivated)
		{
			ActiveSpecialArt = (*(FoundSpecialArt));
			ValidSpecialArt = true;
		}

		else
		{
			FSpecialArt EmptySpecialArt;
			ActiveSpecialArt = EmptySpecialArt; 
			ValidSpecialArt = false;
		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(" FoundSpecialArt not Found"));
		ValidSpecialArt = false;

	}


}

void USpecialSetComponent::ActivateAllEffectsFromSpecialArt()
{
	if (ValidSpecialArt)
	{
		ActivateIndividualEffectFromSpecialArt(ActiveSpecialArt.Effect1, ActiveSpecialArt.EDuration1, ActiveSpecialArt.EQuantity1);
		ActivateIndividualEffectFromSpecialArt(ActiveSpecialArt.Effect2, ActiveSpecialArt.EDuration2, ActiveSpecialArt.EQuantity2);
		ActivateIndividualEffectFromSpecialArt(ActiveSpecialArt.Effect3, ActiveSpecialArt.EDuration3, ActiveSpecialArt.EQuantity3);

	}
	
}

void USpecialSetComponent::ActivateIndividualEffectFromSpecialArt(ESpecialArtEffectStatus EffectToActivate, uint8 EffectToActivateDuration, float EffectToActivateQuantity)
{
	//TO IMPLEMENT

}
