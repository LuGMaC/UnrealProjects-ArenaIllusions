// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Enums.h"
#include "BattleSystemManager.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ARENAPROJECT_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:


	

	/**======================================================================================
	*							Outside of Combat
	* =======================================================================================
	*Outside of Combat Variables and functions
	*These declarations are for Widget and Variables outside of Battle
	*WidgetToShow - Widget that will be shown to player. Will be setup by the character 
	*SetupTextForDialogue - Setup the dialogue to show to the player and uses the widget to add it to viewport
	*SetupForCombat - Display widgets for equipment, challenge choice
	*SetupEnemies - Clears the TArray and insert the Enemies that will fight 
	*SetupAllies - Same as SetupEnemies, but for allies
	*SaveData - TO IMPLEMENT
	*LoadData - TO IMPLEMENT
	* 
	*/

	/**Reference to the UMG Asset in the Editor*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> WMainMenuWidgetAsset;

	/*Variable to hold reference after creating the Widget*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	class UUserWidget* MainMenuWidget;

	FORCEINLINE void SetMainMenuWidget(UUserWidget* MainMenuWidgetReference) { MainMenuWidget = MainMenuWidgetReference; }
	FORCEINLINE UUserWidget* GetMainMenuWidget() { return MainMenuWidget; }

	ABaseCharacter* OutsideOfCombatCharacter; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Context System")
	int WorldContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Context System")
	int LocalContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Context System")
	int PlayerControllerDialogueLineID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Context System")
	int PlayerControllerDialogueConversationID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Context System")
	bool CutsceneDialogue;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
	FName CharacterName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
	FText EnglishTextToShow;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
	FText JapaneseTextToShow;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
	bool bLastDialogueToShow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	bool bChapterBattleLost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	bool bEnglishDialogueOption;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User Widget")
	EUserWidgetStatus UserWidgetToShow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lounge Music")
	TMap<int, USoundBase*> LoungeMusicMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lounge Music")
	USoundBase* ActiveLoungeMusicReference;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	class UDataTable* DialogueTable;

	//UFUNCTION(BlueprintNativeEvent, Category = "Text")
	//void SetupTextForDialogue(TSubclassOf<UUserWidget> DialogueWidget, FText TextToOutput);


	/**======================================================================================
	*							Outside of Combat UI Related Functions
	* =======================================================================================
	*
	*/


	UFUNCTION(BlueprintCallable, Category = "Text")
	void SetupDialogue();

	//Starts the cutscene dialogue
	UFUNCTION(BlueprintCallable, Category = "Text")
	void SetupCutsceneStart(int CutsceneWorldContext, int CutsceneLocalContext, int CutsceneDialogueID);


	UFUNCTION(BlueprintCallable, Category = "Text")
	void GetCutsceneDialogue(int CutsceneWorldContext, int CutsceneLocalContext, int CutsceneDialogueID, int CutsceneLineID);


	UFUNCTION(BlueprintNativeEvent, Category = "Text")
	void ShowDialogue(const FText& TextToOutput);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UI")
	void ShowInteract();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UI")
	void HideInteract();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UI")
	void ShowMainMenu();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UI")
	void HideMainMenu();
	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void SetupReference();

	/*This function is for when an external class (e.g.: Battle Sys Manager or an event) needs an specific UI
	* such as dialogue screen for events or the combat main menu for the battle system manager
	* The Parent function (e.g. C++ function) MUST be called before any actions are taken in BPs
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UI")
	void SetupSpecialUI(EUserWidgetStatus WidgetSpecialShow);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "UI")
	void SetupInitialMenu();

	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void SetupCutscene(int CutsceneNumber);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Music")
	void SetupAndPlayLoungeMusic();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Music")
	void StopLoungeMusic();

	/**======================================================================================
	*							Combat Related Functions and Variables
	* =======================================================================================
	*	
	* SetupUIForCombat - Kills the main menu widget, creates the combat widget and adds it to viewport
	* 
	* 
	*/

	//SetupForCombat - Widget already in MainPlayerController. Function will kill MainMenuWidget and create combat one
	//SetupEnemies, EnemiesToFight, SetupAllies, AlliesToFight, SetupBattleConditions do to Battle System Manager
	//Create


	/**Reference to the UMG Asset in the Editor*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> WCombatWidgetAsset;

	/*Variable to hold reference after creating the Widget*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	class UUserWidget* CombatWidget;

	FORCEINLINE void SetCombatWidget(UUserWidget* CombatWidgetReference) { CombatWidget = CombatWidgetReference; }
	FORCEINLINE UUserWidget* GetSetCombatWidget() { return CombatWidget; }

	//Hold Character Widget asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AllyWidgets")
	TSubclassOf<class UUserWidget> WCharacterWidget;

	/*Variable to hold reference after creating the Widget*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AllyWidgets")
	class UUserWidget* CharacterAWidgetReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AllyWidgets")
	class UUserWidget* CharacterBWidgetReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AllyWidgets")
	class UUserWidget* CharacterCWidgetReference;

	//Hold Enemy Widget asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AllyWidgets")
	TSubclassOf<class UUserWidget> WEnemyWidget;

	/*Variable to hold reference after creating the Widget*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AllyWidgets")
	class UUserWidget* EnemyAWidgetReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AllyWidgets")
	class UUserWidget* EnemyBWidgetReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AllyWidgets")
	class UUserWidget* EnemyCWidgetReference;

	//Battle System
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Manager Reference")
	class ABattleSystemManager* BattleSystemManagerReference;

	//First FString is name of the character
	//Second FString is name of the Skill

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stance List")
	TMap<FString, FCharacterSkillsVariableData> CharacterSkillVariableInformationMap;


	FORCEINLINE void SetBattleSystemManagerReference(ABattleSystemManager* ObjectToSet) { BattleSystemManagerReference = ObjectToSet; }


	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetupMainMenuUI();
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetupUIForCombat();

	UFUNCTION(BlueprintCallable, Category = "Battle Setup")
	void SetupChallengeNumber(int ChallengeNumber);

	UFUNCTION(BlueprintCallable, Category = "Battle Setup")
	void SetupCombatantsNames(TArray<FName> AlliesNames, TArray<FName> EnemiesNames);

	UFUNCTION(BlueprintCallable, Category = "Battle Setup")
	void SetupBattleConditions(EFieldEffect SetupFieldEffect, EVictoryCondition SetupVictoryCondition);

	UFUNCTION(BlueprintCallable, Category = "Battle Setup")
	void SetupVictoryDetails(int Quantity);

	UFUNCTION(BlueprintCallable, Category = "Battle Setup")
	void SetupVictoryDetailsForPartBreak(FName PartBreak);

	UFUNCTION(BlueprintCallable, Category = "Skill Variable Information")
	void SaveSkillVariableInformation(FString AllyName, TMap<FString, FSkillVariableData> SkillVariableInformationTMap);


	/**====================================================================================== 
	*							Inside of Combat 
	* =======================================================================================
	*These declarations are for Widget and Variables inside of Battle
	* SkillUsageCount - TO IMPLEMENT
	* 
	* 
	* 
	*/

	/**======================================================================================
	*							Other Functions and Variables
	* =======================================================================================
	*/

	void GameModeOnly();
	void UIModeOnly();

	UFUNCTION(BlueprintCallable, Category = "World Context System")
	void UpdateWorldSettings(bool UpdateWorldContext, bool UpdateLocalContext);

	UFUNCTION(BlueprintCallable, Category = "Save/Load")
	void Save();

	UFUNCTION(BlueprintCallable, Category = "Save/Load")
	void Load();


protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

};
