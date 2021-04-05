// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillSetComponentV2.h"
#include "Enums.h"
#include "Engine/DataTable.h"

// Sets default values for this component's properties
USkillSetComponentV2::USkillSetComponentV2()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USkillSetComponentV2::BeginPlay()
{
	Super::BeginPlay();

	// ...

	if (SkillTable)
	{
		UE_LOG(LogTemp, Warning, TEXT(" SkillTable exists - SkillSet V2"));
	}

	else {

		UE_LOG(LogTemp, Warning, TEXT(" SkillTable does not exist - SkillSet V2"));
	}
	
}


// Called every frame
void USkillSetComponentV2::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void USkillSetComponentV2::SetActiveSkill(FName RowName)
{
	static const FString ContextString(TEXT("ContextString"));
	FoundSkill = SkillTable->FindRow<FSkillV2>(RowName, ContextString);

	UE_LOG(LogTemp, Warning, TEXT(" NewContextStringv2"));

	if (FoundSkill)
	{
		UE_LOG(LogTemp, Warning, TEXT(" SkillFound"));
		ActiveSkill = (*(FoundSkill));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(" SkillNOTFound"));

	}

}

void USkillSetComponentV2::SetActiveSkillByIndex(uint8 Index)
{
	ActiveSkill = ActivatedSkills[Index];
}

TArray<EAttackType> USkillSetComponentV2::CheckTurnSkills(TArray<EAttackType> TurnCommands)
{
	/*This functions will search the TurnCommands array to see if a skill was trigerred.
	* If there's a match to a skills command, the last input of the match in TurnCommands will change to EAT_Special
	* The skill found will have it's struct saved in ActivatedSkills to be quickly called once the action starts
	* WARNING : The Skill Database MUST be sorted by cost from the lesser to the bigger.
	*/

	UE_LOG(LogTemp, Warning, TEXT(" SkillSetComponent - Check Turn Skills "));

	//Create a copy of the TurnCommands array. Since some of the inputs might change mid operation
	TurnCommandsFinal = TurnCommands;

	if (TurnCommands.Num() > 2)
	{
		//ActivatedSkills is a TArray that contains the Skills that can be activated
		//The Size of it must be equal to TurnCommands because the index will act as a pointer to when the skill was activated
		ActivatedSkills.Empty();
		ActivatedSkills.SetNum(TurnCommands.Num());

		//Get AllRowNames from the Data Table
		TArray<FName> RowNames;
		RowNames = SkillTable->GetRowNames();


		for (int i = 0; i < RowNames.Num(); i++)
		{
			//Iterate in the entire data table until it ends OR the cost of a skill is bigger than the command input
			static const FString ContextString(TEXT("ContextString"));
			FoundSkill = SkillTable->FindRow<FSkillV2>(RowNames[i], ContextString);

			FString SName = FoundSkill->SkillName.ToString();

			UE_LOG(LogTemp, Warning, TEXT(" SkillSetComponent - Check Skill: %s "), *SName);

			if (FoundSkill->Cost > (TurnCommands.Num() * 10))
			{
				//If this is true, it means the skills to be search are always bigger than the command array
				//Thus we can stop our search
				break;
			}

			SkillComparison(FoundSkill, TurnCommands);
		}

		UE_LOG(LogTemp, Warning, TEXT(" SkillSetComponent - Finished sorting skills "));

		//Special enum value check
		/**for (int i = 0; i < TurnCommandsFinal.Num(); i++)
		{

			EAttackType AttackToPrint = TurnCommandsFinal[i];

			//UEnum* EnumType = FindObject<UEnum>(ANY_PACKAGE, TEXT("EAttackType"), true);
			FString AttackString = StaticEnum<EAttackType>()->GetNameByIndex(static_cast<int32>(AttackToPrint)).ToString();

			UE_LOG(LogTemp, Warning, TEXT("Command:%d - %s"), i, *AttackString);

		}*/

		return TurnCommandsFinal;
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT(" SkillSetComponent - No need to sort skills "));

		/*for (int i = 0; i < TurnCommandsFinal.Num(); i++)
		{
			//FString message = TEXT("Command: ", i, " - ") + TurnCommandsFinal[i]->GetValueAsString();
		}*/


		//If TurnCommands only has 2 or less inputs, there are no skills to be activated
		return TurnCommandsFinal;
	}

}


void USkillSetComponentV2::SkillComparison(FSkillV2* SkillToCompare, TArray<EAttackType> Commands)
{
	/* Searches the entire Commands array to see if the SkillToCompare was activated at some point
	* Then the Skill was activated, it writes their name in the ActivatedSkills TArray in the position where they can be activated
	* For example, if a Skill named Uppercut was activated at the 5th command
	* It would write the FName Uppercut on the index 4 (5th position) of the ActivatedSkills TArray
	* Caution: The Skill Comparison will overwrite if two skills activate at the same time so it should be called respecting cost order
	* Caution2: This function will not compare the cost of the SkillToCompare and the Commands so this should be done before calling this function.
	*/

	//Gets each individual command from the skill and puts them in an TArray
	TArray<EAttackType> SkillCommands = SkillCommandsToArray(SkillToCompare);

	//Special Enum name UELOG
	/*for (int i = 0; i < SkillCommands.Num(); i++)
	{
		FString SkillCommand = StaticEnum<EAttackType>()->GetNameByIndex(static_cast<int32>(SkillCommands[i])).ToString();
		UE_LOG(LogTemp, Warning, TEXT("Command:%d - %s"), i, *SkillCommand);
	}*/


	//Determines the LastIndex in the commands array that will be searched
	//If the skill has 5 inputs, we don't need to check an array with only that last 4
	int LastIndexToSearch = (Commands.Num() - SkillCommands.Num());

	//UE_LOG(LogTemp, Warning, TEXT("Last Index: %d"), LastIndexToSearch);

	for (int i = 0; i <= LastIndexToSearch; i++)
	{
		//The Mid attack can act as a Joker Input only once
		bool bJokerInput = false;

		for (int j = 0; j < SkillCommands.Num(); j++)
		{
			/*Update on 02/12
			* Mid command can accept any input
			*/
			if ((SkillCommands[j] == Commands[i + j]) || (SkillCommands[j] == EAttackType::EAT_Mid))
			{
				//UE_LOG(LogTemp, Warning, TEXT(" SkillComparison - Equal Input OR Mid"));
				if (j == (SkillCommands.Num() - 1))
				{
					//UE_LOG(LogTemp, Warning, TEXT(" SkillComparison - Skill Trigger on:  %d"), i+j);
					//If this triggers, all commands were checked and it does have a match within the commands inputted
					ActivatedSkills[i + j] = (*(SkillToCompare));
					TurnCommandsFinal[i + j] = EAttackType::EAT_Special;
				}
			}

			else
			{
				//If the inputs are different, we need to check if the joker input was used
				//UE_LOG(LogTemp, Warning, TEXT(" SkillComparison - Different Input "));

				if ((Commands[i + j] == EAttackType::EAT_Mid) && (!bJokerInput))
				{
					//UE_LOG(LogTemp, Warning, TEXT(" SkillComparison - Joker Used "));
					//It will enter this if when the input on commands is Mid AND the joker input hasn't been used
					//If the mid input is also on the Skill, it won't fail the previous if check
					bJokerInput = true;

					//If this is the last input to be checked, the skill was activated by the joker
					if (j == (SkillCommands.Num() - 1))
					{
						//UE_LOG(LogTemp, Warning, TEXT(" SkillComparison - Joker - Skill Trigger on:  %d"), i + j);
						//If this triggers, all commands were checked and it does have a match within the commands inputted
						ActivatedSkills[i + j] = (*(SkillToCompare));
						TurnCommandsFinal[i + j] = EAttackType::EAT_Special;

					}
				}

				else
				{
					//If the inputs are different and the joker system does not trigger, it means that this skill will not activate at this time
					UE_LOG(LogTemp, Warning, TEXT(" SkillComparison - Skill will not activate "));
					break;
				}
			}

		}


	}


}


TArray<EAttackType> USkillSetComponentV2::SkillCommandsToArray(FSkillV2* Skill)
{
	/*The commands in the Skill spreadsheet are separated in different collumns
	* and to make comparisons viable, we need to make all of them in a single TArray
	* Thus, this function will read each Commands from the skill and input them on an array
	* The output is a TArray with all the commands
	*/

	TArray<EAttackType> OutputArray;

	//A Skill has, at minimum 3 inputs. 
	OutputArray.Emplace(Skill->Command1);
	OutputArray.Emplace(Skill->Command2);
	OutputArray.Emplace(Skill->Command3);

	//4th Command
	if (Skill->Command4 == EAttackType::EAT_Empty)
	{
		return OutputArray;
	}

	OutputArray.Emplace(Skill->Command4);

	//5th Command
	if (Skill->Command5 == EAttackType::EAT_Empty)
	{
		return OutputArray;
	}

	OutputArray.Emplace(Skill->Command5);

	//6th Command
	if (Skill->Command6 == EAttackType::EAT_Empty)
	{
		return OutputArray;
	}

	OutputArray.Emplace(Skill->Command6);

	//7th Command
	if (Skill->Command7 == EAttackType::EAT_Empty)
	{
		return OutputArray;
	}

	OutputArray.Emplace(Skill->Command7);

	//8th Command
	if (Skill->Command8 == EAttackType::EAT_Empty)
	{
		return OutputArray;
	}

	OutputArray.Emplace(Skill->Command8);

	//9th Command
	if (Skill->Command9 == EAttackType::EAT_Empty)
	{
		return OutputArray;
	}

	OutputArray.Emplace(Skill->Command9);

	//10th Command
	if (Skill->Command10 == EAttackType::EAT_Empty)
	{
		return OutputArray;
	}

	OutputArray.Emplace(Skill->Command10);

	return OutputArray;

}

