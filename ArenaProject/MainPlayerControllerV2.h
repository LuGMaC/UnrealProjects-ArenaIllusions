// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Enums.h"
#include "BattleSystemManagerV2.h"
#include "MainPlayerControllerV2.generated.h"

/**
 * 
 */
UCLASS()
class ARENAPROJECT_API AMainPlayerControllerV2 : public APlayerController
{
	GENERATED_BODY()

public:

	/*===========================================================
	*				Widgets and References
	*============================================================
	*/

	/**Reference to the UMG Asset in the Editor - Outside of Combat Widget*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> WMainMenuWidgetAsset;

	/*Variable to hold reference after creating the Widget*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	class UUserWidget* MainMenuWidget;

	FORCEINLINE void SetMainMenuWidget(UUserWidget* MainMenuWidgetReference) { MainMenuWidget = MainMenuWidgetReference; }
	FORCEINLINE UUserWidget* GetMainMenuWidget() { return MainMenuWidget; }

	/**Reference to the UMG Asset in the Editor - Combat Widget*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> WCombatWidgetAsset;

	/*Variable to hold reference after creating the Widget*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	class UUserWidget* CombatWidget;

	FORCEINLINE void SetCombatWidget(UUserWidget* CombatWidgetReference) { CombatWidget = CombatWidgetReference; }
	FORCEINLINE UUserWidget* GetCombatWidget() { return CombatWidget; }

	/**Reference to the UMG Asset in the Editor - Dialogue Widget*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> WDialogueWidgetAsset;

	/*Variable to hold reference after creating the Widget*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	class UUserWidget* DialogueWidget;

	FORCEINLINE void SetDialogueWidget(UUserWidget* DialogueWidgetReference) { DialogueWidget = DialogueWidgetReference; }
	FORCEINLINE UUserWidget* GetDialogueWidget() { return DialogueWidget; }

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

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetupMainMenuUI();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetupUIForCombat();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetupUIForDialogue();

	/*===========================================================
	*		Battle System Related Variables and Functions
	*============================================================
	*/

	//Battle System
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Manager Reference")
	class ABattleSystemManagerV2* BattleSystemManagerReference;

	FORCEINLINE void SetBattleSystemManagerReference(ABattleSystemManagerV2* ObjectToSet) { BattleSystemManagerReference = ObjectToSet; }

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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UI")
	void SetupInitialMenu();

	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void SetupCutscene(int CutsceneNumber);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Music")
	void SetupAndPlayLoungeMusic();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Music")
	void StopLoungeMusic();


	/**======================================================================================
	*						Outside of Combat - Variables and Functions
	* =======================================================================================
	*/

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

	UFUNCTION(BlueprintCallable, Category = "World Context System")
	void UpdateWorldSettings(bool UpdateWorldContext, bool UpdateLocalContext);

	ABaseCharacter* OutsideOfCombatCharacter;


	/**======================================================================================
	*						Save Data Related Variable and Functions
	* =======================================================================================
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stance List")
	TMap<FString, FCharacterSkillsVariableData> CharacterSkillVariableInformationMap;

	UFUNCTION(BlueprintCallable, Category = "Skill Variable Information")
	void SaveSkillVariableInformation(FString AllyName, TMap<FString, FSkillVariableData> SkillVariableInformationTMap);

	UFUNCTION(BlueprintCallable, Category = "Save/Load")
	void Save();

	UFUNCTION(BlueprintCallable, Category = "Save/Load")
	void Load();


	/**======================================================================================
	*							Other Functions and Variables
	* =======================================================================================
	*/

	void GameModeOnly();
	void UIModeOnly();




};
