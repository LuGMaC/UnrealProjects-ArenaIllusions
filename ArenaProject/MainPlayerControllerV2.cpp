// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerControllerV2.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "CombatCharacterV2.h"
#include "NonCombatBaseCharacter.h"
#include "BaseCharacter.h"
#include "BattleSystemManagerV2.h"
#include "ArenaSaveGame.h"


void AMainPlayerControllerV2::BeginPlay()
{
	Super::BeginPlay();

	this->SetupMainMenuUI();

	OutsideOfCombatCharacter = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	OutsideOfCombatCharacter->ToggleBaseCharacterCamera();
	WorldContext = 0;
	LocalContext = 0;
	PlayerControllerDialogueConversationID = 0;
	PlayerControllerDialogueLineID = 1;
	bAutoManageActiveCameraTarget = false;
	bChapterBattleLost = false;
	bEnglishDialogueOption = true;
	CutsceneDialogue = false;

	SetupInitialMenu();
}

void AMainPlayerControllerV2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}


/**======================================================================================
*						Outside of Combat - UI Related Functions
* =======================================================================================
*
*/


void AMainPlayerControllerV2::ShowDialogue_Implementation(const FText& TextToOutput)
{
	/*Receives the Dialogue from FText and shows it to the player
	*/
}

void AMainPlayerControllerV2::ShowInteract_Implementation()
{

}
void AMainPlayerControllerV2::HideInteract_Implementation()
{

}
void AMainPlayerControllerV2::ShowMainMenu_Implementation()
{

}
void AMainPlayerControllerV2::HideMainMenu_Implementation()
{

}

void AMainPlayerControllerV2::SetupReference_Implementation()
{

}

void AMainPlayerControllerV2::SetupAndPlayLoungeMusic_Implementation()
{

}

void AMainPlayerControllerV2::StopLoungeMusic_Implementation()
{

}

void AMainPlayerControllerV2::SetupCutscene_Implementation(int CutsceneNumber)
{

}

void AMainPlayerControllerV2::SetupInitialMenu_Implementation()
{

}

void AMainPlayerControllerV2::SetupSpecialUI_Implementation(EUserWidgetStatus WidgetSpecialShow)
{
	if (WidgetSpecialShow == EUserWidgetStatus::EUWS_CombatMainMenu)
	{
		if (CombatWidget)
		{
			/*If CombatWidget is valid, we only need to change it's state to Main Menu
			* That should be done in BPs
			*/
		}

		else
		{
			/*If the reference is not valid, then we need to recreate the Combat Widget
			*/

			SetupUIForCombat();
		}


	}

	if (WidgetSpecialShow == EUserWidgetStatus::EUWS_MainMenu)
	{
		if (MainMenuWidget)
		{
			/*If the Main Menu Widget is valid, we only need to change it's state to Dialogue
			* and send the correct text
			* That should be done in BPs
			*/
		}

		else
		{
			SetupMainMenuUI();
		}

	}

	if (WidgetSpecialShow == EUserWidgetStatus::EUWS_Dialogue)
	{
		if (DialogueWidget)
		{
			/*If the Main Menu Widget is valid, we only need to change it's state to Dialogue
			* and send the correct text
			* That should be done in BPs
			*/
		}

		else
		{
			SetupUIForDialogue();
		}

	}

}

void AMainPlayerControllerV2::SetupUIForCombat()
{
	if (WCombatWidgetAsset)
	{
		//If reference is found, create widget
		CombatWidget = CreateWidget<UUserWidget>(this, WCombatWidgetAsset);

		if (MainMenuWidget)
		{
			MainMenuWidget->RemoveFromParent();
		}

		if (DialogueWidget)
		{
			DialogueWidget->RemoveFromParent();
		}

		//Add to Viewport once battle begins
		CombatWidget->AddToViewport();
		CombatWidget->SetVisibility(ESlateVisibility::Visible);
		UIModeOnly();

	}

}

void AMainPlayerControllerV2::SetupMainMenuUI()
{
	if (WMainMenuWidgetAsset)
	{
		MainMenuWidget = CreateWidget<UUserWidget>(this, WMainMenuWidgetAsset);

		if (CombatWidget)
		{
			CombatWidget->RemoveFromParent();
		}

		if (DialogueWidget)
		{
			DialogueWidget->RemoveFromParent();
		}

		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();
			SetupReference();
			GameModeOnly();
		}
	}

}

void AMainPlayerControllerV2::SetupUIForDialogue()
{
	if (WDialogueWidgetAsset)
	{
		DialogueWidget = CreateWidget<UUserWidget>(this, WDialogueWidgetAsset);

		if (CombatWidget)
		{
			CombatWidget->RemoveFromParent();
		}

		if (MainMenuWidget)
		{
			MainMenuWidget->RemoveFromParent();
		}

		if (DialogueWidget)
		{
			DialogueWidget->AddToViewport();
			UIModeOnly();

			//Make a function to fetch the Dialogue that needs to be shown
		}
	}


}

void AMainPlayerControllerV2::GameModeOnly()
{
	//Call this function to return to Visitor character

	FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);
	bShowMouseCursor = false;

	if (OutsideOfCombatCharacter)
	{
		OutsideOfCombatCharacter->EnableInput(this);
		Possess(OutsideOfCombatCharacter);
		SetViewTargetWithBlend(OutsideOfCombatCharacter, 0, EViewTargetBlendFunction::VTBlend_Linear, 0, false);
	}

}


void AMainPlayerControllerV2::UIModeOnly()
{
	OutsideOfCombatCharacter->ConsumeMovementInputVector();
	OutsideOfCombatCharacter->DisableInput(this);
	FInputModeUIOnly UIModeOnly;
	SetInputMode(UIModeOnly);
	bShowMouseCursor = true;
}

/**======================================================================================
*							Dialogue Related Functions
* =======================================================================================
*
*/

void AMainPlayerControllerV2::SetupCutsceneStart(int CutsceneWorldContext, int CutsceneLocalContext, int CutsceneDialogueID)
{
	CutsceneDialogue = true;
	PlayerControllerDialogueConversationID = CutsceneDialogueID;
	PlayerControllerDialogueLineID = 1;
	bLastDialogueToShow = false;
	//GetCutsceneDialogue(WorldContext, LocalContext, CutsceneDialogueID, PlayerControllerDialogueLineID);
	SetupSpecialUI(EUserWidgetStatus::EUWS_Dialogue);

}


void AMainPlayerControllerV2::GetCutsceneDialogue(int CutsceneWorldContext, int CutsceneLocalContext, int CutsceneDialogueID, int CutsceneLineID)
{
	FDialogue* CutsceneDialogueText;
	FString DialogueRowNameString = FString::FromInt(CutsceneWorldContext) + "." + FString::FromInt(CutsceneLocalContext) + "." + FString::FromInt(CutsceneDialogueID) + "." + FString::FromInt(CutsceneLineID);
	FName DialogueRowName = FName(*DialogueRowNameString);

	static const FString DialogueContextString(TEXT("DialogueContextString"));
	CutsceneDialogueText = DialogueTable->FindRow<FDialogue>(DialogueRowName, DialogueContextString);

	if (CutsceneDialogueText != nullptr)
	{
		CharacterName = CutsceneDialogueText->TalkPersonName;
		EnglishTextToShow = CutsceneDialogueText->EnglishText;
		JapaneseTextToShow = CutsceneDialogueText->JapaneseText;

		if (CutsceneDialogueText->SpecialEventID != 0)
		{
			SetupCutscene(CutsceneDialogueText->SpecialEventID);
		}

		if (CutsceneDialogueText->LinesRemaining == 0)
		{
			bLastDialogueToShow = true;
			//Changed in BP
			//CutsceneDialogue = false; 
			PlayerControllerDialogueConversationID++;
			PlayerControllerDialogueLineID = 1;
		}

		else
		{
			PlayerControllerDialogueLineID++;
		}

	}

	else
	{
		CharacterName = "VOID";
		EnglishTextToShow = FText::FromString("404");
		JapaneseTextToShow = FText::FromString("Dame Dame Dameno damenanoyo");
	}

}

void AMainPlayerControllerV2::SetupDialogue()
{
	/*Calls Player Character which, in turn, will get the closest NPC and, from the NPC,
	* will get the necessary FDialogue Struct that will be used for the dialogue.
	* This function will update TalkPersonName, EnglishText and JapaneseText and then
	* a Get function from BPs should be used to get the text
	*/

	bLastDialogueToShow = false;

	if (OutsideOfCombatCharacter)
	{
		FDialogue* CurrentDialogue = OutsideOfCombatCharacter->GetDialogueFromNPC(WorldContext, LocalContext);

		UE_LOG(LogTemp, Warning, TEXT(" Get Dialogue from %s - World: %d - Local: %d"), *OutsideOfCombatCharacter->Name, WorldContext, LocalContext);

		if (CurrentDialogue != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT(" Get Dialogue from NPC - Dialogue found"));
			CharacterName = CurrentDialogue->TalkPersonName;
			EnglishTextToShow = CurrentDialogue->EnglishText;
			JapaneseTextToShow = CurrentDialogue->JapaneseText;

			if (CurrentDialogue->SpecialEventID != 0)
			{
				SetupCutscene(CurrentDialogue->SpecialEventID);
			}

			if (CurrentDialogue->LinesRemaining == 0)
			{
				bLastDialogueToShow = true;
			}


		}

		else
		{

			CharacterName = "VOID";
			EnglishTextToShow = FText::FromString("404");
			JapaneseTextToShow = FText::FromString("Dame Dame Dameno damenanoyo");
		}

	}

}

void AMainPlayerControllerV2::UpdateWorldSettings(bool UpdateWorldContext, bool UpdateLocalContext)
{
	PlayerControllerDialogueConversationID = 0;
	PlayerControllerDialogueLineID = 1;

	if (UpdateWorldContext)
	{
		WorldContext++;
		LocalContext = 0;
		bChapterBattleLost = false;
	}

	if (UpdateLocalContext && !bChapterBattleLost)
	{
		LocalContext++;
		bChapterBattleLost = true;
	}

	TArray<AActor*> FoundNPCs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANonCombatBaseCharacter::StaticClass(), FoundNPCs);

	for (AActor* Actor : FoundNPCs)
	{
		ANonCombatBaseCharacter* CastActor = Cast<ANonCombatBaseCharacter>(Actor);
		CastActor->AdjustConversationID(0);
	}

	Save();

	//TSubclassOf<ANonCombatBaseCharacter> NPCs;
	//TArray<ANonCombatBaseCharacter*> FoundNPCs;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), NPCs, FoundNPCs);


}


/**======================================================================================
*							Combat Related Functions
* =======================================================================================
*
*/


void AMainPlayerControllerV2::SetupCombatantsNames(TArray<FName> AlliesNames, TArray<FName> EnemiesNames)
{
	BattleSystemManagerReference->SetupAlliesNames(AlliesNames);
	BattleSystemManagerReference->SetupEnemiesNames(EnemiesNames);
}

void AMainPlayerControllerV2::SetupBattleConditions(EFieldEffect SetupFieldEffect, EVictoryCondition SetupVictoryCondition)
{
	BattleSystemManagerReference->SetupBattle(SetupFieldEffect, SetupVictoryCondition);
}

void AMainPlayerControllerV2::SetupVictoryDetails(int Quantity)
{
	BattleSystemManagerReference->SetupVictoryDetails(Quantity);
}

void AMainPlayerControllerV2::SetupVictoryDetailsForPartBreak(FName PartBreak)
{
	BattleSystemManagerReference->SetPartToBreak(PartBreak);
}

void AMainPlayerControllerV2::SetupChallengeNumber(int ChallengeNumber)
{
	BattleSystemManagerReference->SetupChallengeNumber(ChallengeNumber);
}


void AMainPlayerControllerV2::SaveSkillVariableInformation(FString AllyName, TMap<FString, FSkillVariableData> SkillVariableInformationTMap)
{
	//TMap<FString, FCharacterSkillsVariableData> CharacterSkillVariableInformationMap;

	FCharacterSkillsVariableData SkillVariableData;
	SkillVariableData.CharacterSkillVariableData = SkillVariableInformationTMap;

	CharacterSkillVariableInformationMap.Emplace(AllyName, SkillVariableData);
}

/**======================================================================================
*							Inside of Combat
* =======================================================================================
*/




/**======================================================================================
*							Other Functions
* =======================================================================================
*/

void AMainPlayerControllerV2::Save()
{
	UArenaSaveGame* SaveGameInstance = Cast<UArenaSaveGame>(UGameplayStatics::CreateSaveGameObject(UArenaSaveGame::StaticClass()));
	SaveGameInstance->WorldContext = WorldContext;
	SaveGameInstance->LocalContext = LocalContext;
	SaveGameInstance->CharacterSkillVariableInformationMap = CharacterSkillVariableInformationMap;

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, "Default", 0);
}

void AMainPlayerControllerV2::Load()
{
	UArenaSaveGame* LoadGameInstance = Cast<UArenaSaveGame>(UGameplayStatics::CreateSaveGameObject(UArenaSaveGame::StaticClass()));

	LoadGameInstance = Cast<UArenaSaveGame>(UGameplayStatics::LoadGameFromSlot("Default", 0));

	WorldContext = LoadGameInstance->WorldContext;
	LocalContext = LoadGameInstance->LocalContext;
	if (LocalContext != 0)
	{
		bChapterBattleLost = true;
	}
	CharacterSkillVariableInformationMap = LoadGameInstance->CharacterSkillVariableInformationMap;

}