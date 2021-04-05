// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums.h"
#include "CombatCharacterV2.h"
#include "SkillSetComponentV2.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENAPROJECT_API USkillSetComponentV2 : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillSetComponentV2();

	/*The SkillTable that contains the character skills
	* WARNING : The Skill Table MUST be sorted by cost from the lesser to the bigger.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	class UDataTable* SkillTable;

	//Result of Searching Skills by their inputs
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills Search")
	TArray<FSkillV2> PossibleSkills;

	//Skill that will be activated next
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills")
	FSkillV2 ActiveSkill;

	//Skill that is the result of a search in the data table.
	FSkillV2* FoundSkill;

	//TArray that contains the position and the Skills that can be activated
	TArray<FSkillV2> ActivatedSkills;

	//Create a copy of the TurnCommands array. Since some of the inputs might change mid operation
	TArray<EAttackType> TurnCommandsFinal;



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

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
	void SkillComparison(FSkillV2* SkillToCompare, TArray<EAttackType> Commands);

	TArray<EAttackType> SkillCommandsToArray(FSkillV2* Skill);

		
};
