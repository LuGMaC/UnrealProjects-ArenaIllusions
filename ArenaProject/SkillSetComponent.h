// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Enums.h"
#include "BaseCharacter.h"
#include "SkillSetComponent.generated.h"




USTRUCT(BlueprintType)
struct FSkill : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data")
	FName SkillName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data | Commands")
	uint8 Cost;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data | Commands")
	EAttackType Command1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data | Commands")
	EAttackType Command2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data | Commands")
	EAttackType Command3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data | Commands")
	EAttackType Command4;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data | Commands")
	EAttackType Command5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data | Commands")
	EAttackType Command6;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data | Commands")
	EAttackType Command7;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data | Commands")
	EAttackType Command8;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data | Commands")
	EAttackType Command9;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data | Commands")
	EAttackType Command10;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data | Parameter 1")
	ESkillParameter Parameter1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data | Parameter 1")
	float Scaling1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data | Parameter 2")
	ESkillParameter Parameter2;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data | Parameter 2")
	float Scaling2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data | Parameter 3")
	ESkillParameter Parameter3;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data | Parameter 3")
	float Scaling3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data")
	ETargetDefinition Target;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data")
	EBodyPartTarget BodyPartTarget;

	/*Effects
	* Status Effect: Determines which effect will take place
	* Duration: Number of turns that the effect will last. 0 will mean it's instantaneous.
	* Quantity: Scaling Parameter as follows:
	* HealHP, HealMP, HealAP : Quantity to be restored
	* Buff and Debuffs: % that will change the status
	* BodyPartExtraDamage: How much damage will be added to the body part when the combo hits
	* Damage: How much damage will be added when the combo hits
	* Charge: % that will change the damage on the next combo
	* FollowUp and ComboSupport: Do not use Scaling
	* 
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data | Effect1")
	EEffectStatus Effect1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data | Effect1")
	uint8 EDuration1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data | Effect1")
	float EQuantity1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data | Effect2")
	EEffectStatus Effect2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data | Effect2")
	uint8 EDuration2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data | Effect2")
	float EQuantity2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data | Effect3")
	EEffectStatus Effect3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data | Effect3")
	uint8 EDuration3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data | Effect3")
	float EQuantity3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data")
	FName StanceChange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data")
	int NumberOfHits;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data")
	int32 UsageCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data")
	FText Description;

};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENAPROJECT_API USkillSetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillSetComponent();

	/*The SkillTable that contains the character skills
	* WARNING : The Skill Table MUST be sorted by cost from the lesser to the bigger. 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	class UDataTable* SkillTable; 

	//Result of Searching Skills by their inputs
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills Search")
	TArray<FSkill> PossibleSkills; 

	//Skill that will be activated next
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills")
	FSkill ActiveSkill;

	//Skill that is the result of a search in the data table.
	FSkill* FoundSkill;

	//TArray that contains the position and the Skills that can be activated
	TArray<FSkill> ActivatedSkills;

	//Create a copy of the TurnCommands array. Since some of the inputs might change mid operation
	TArray<EAttackType> TurnCommandsFinal;




protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	//Function that will return all possible executable skills based on the inputted commands
	//TArray<FSkill> SearchPossibleSkills(TArray<enum EAttackType> Commands);

	//Function that will update PossibleSkills TArray based on inputted commands
	//This function should be used as a search when the player is choosing which commands to input - To implement
	void SearchPossibleSkills(TArray<EAttackType> Commands);

	//Searches the RowName and Creates the FSkill struct
	void SetActiveSkill(FName RowName);

	//Receives an Index, searches the ActivatedSkills TArray and sets that as the ActiveSkill
	void SetActiveSkillByIndex(uint8 Index);

	/*This functions will search the TurnCommands array to see if a skill was trigerred.
	* If there's a match to a skills command, the last input of the match in TurnCommands will change to EAT_Special
	* The skill found will have it's struct saved in ActivatedSkills to be quickly called once the action starts
	* WARNING : The Skill Database MUST be sorted by cost from the lesser to the bigger.
	*/
	TArray<EAttackType> CheckTurnSkills(TArray<EAttackType> TurnCommands);

	/* Searches the entire Commands array to see if the SkillToCompare was activated at some point
	* Then the Skill was activated, it writes their name in the ActivatedSkills TArray in the position where they can be activated
	* For example, if a Skill named Uppercut was activated at the 5th command
	* It would write the FName Uppercut on the index 4 (5th position) of the ActivatedSkills TArray
	* Caution: The Skill Comparison will overwrite if two skills activate at the same time so it should be called respecting cost order
	* Caution2: This function will not compare the cost of the SkillToCompare and the Commands so this should be done before calling this function.
	*/
	void SkillComparison(FSkill* SkillToCompare, TArray<EAttackType> Commands);

	TArray<EAttackType> SkillCommandsToArray(FSkill* Skill);

			
};
