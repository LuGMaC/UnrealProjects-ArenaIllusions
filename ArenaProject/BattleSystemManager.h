// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums.h"
#include "Containers/Array.h"
#include "BattleSystemManager.generated.h"

UCLASS()
class ARENAPROJECT_API ABattleSystemManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABattleSystemManager();

	/*Parameters for all battle system related features
	*
	*/

	/*Enviromental System/Field Effect related variables
	* All variables will be initiated on a "normal battle" condition and should be changed by BP.
	* Effects are always active for both enemies and allies. 
	* Field effects act upon character final status, as in, Character Base Stats + Stance Stats + Equipment 
	* All variables are %, therefore their values should always be positive.
	* FName for the Name of the Field Effect in case it is of the Irregular type
	* Irregular Type will call a BP Native function to set parameters
	* 
	* 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Effect")
	int ChallengeNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Effect")
	EFieldEffect FieldEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Effect")
	FName FieldName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Effect")
	float MaxHealth_Modifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Effect")
	float CurrentHealth_Modifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Effect")
	float MaxMagic_Modifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Effect")
	float CurrentMagic_Modifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Effect")
	float Max_AP_Modifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Effect")
	float CurrentAP_Modifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Effect")
	float ATK_Modifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Effect")
	float DEF_Modifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Effect")
	float TEC_Modifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Effect")
	float EVA_Modifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Effect")
	float SPD_Modifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Effect")
	float AGI_Modifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Effect")
	float EXP_Modifier;

	/* Other parameters that might be used
	* In the moment they are not implemented
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Effect")
	float Buff_Quantity_Modifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Effect")
	float Buff_Duration_Modifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Effect")
	float Debuff_Value_Modifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Effect")
	float Debuff_Duration_Modifier;

	/* Hit Counter for Allies and Enemies
	* Total Damage for Allies and Enemies
	* Bonus Damage value based on hits for allies and enemies
	*
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bonus Effects")
	int AlliesHitCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bonus Effects")
	int EnemiesHitCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bonus Effects")
	int AlliesDamageCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bonus Effects")
	int EnemiesDamageCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bonus Effects")
	float AlliesExtraDmgModifier;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bonus Effects")
	float EnemiesExtraDmgModifier;

	/**Victory condition related parameters
	* TurnsToSurvive - Set 0 if the condition is not Survival
	* DamageToCause - Set to 0 if the condition is not CauseDamage
	* PartBreak - Leave it empty if the condition is not BreakPart
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Effect")
	int TurnsToSurvive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Effect")
	int32 DamageToCause;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Effect")
	FName PartToBreak; 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Field Effect")
	EBodyPartTarget TargetPart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Effect")
	EVictoryCondition BattleVictoryCondition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Effect")
	EDefeatCondition BattleDefeatCondition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Effect")
	int TurnNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Effect")
	int32 OverallDamageCausedByAllies;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Effect")
	int32 OverallDamageCausedByEnemies;

	bool bBattleEnded;
	bool bPlayerVictory;

	/** Components for transitioning into battle
	* BattleArenaVolume - BoxComponent that will trigger battle setup once Pawn reaches it
	* ArenaName - Name of Arena/Level. If it's not the same as current level, will change level
	* AllySpawnBox - Place where the first ally will spawn. Automatically adjusts for other allies.
	* EnemySpawnBox - Place where the first enemy will spawn. Automatically adjusts for other enemies.
	* EnemyMap/AlliesMap - List of all possible enemies/Allies in the game/level. The FString key is the Enemy name. 
	* Allies/Enemies - TArray with the reference to the Allies Object after they were spawned
	* AlliesName/EnemiesName - TArray to hold the Allies/Enemies names that will participate in battle before they were spawned
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle Transition")
	class UBoxComponent* BattleArenaVolume;

	class UBillboardComponent* Billboard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle Transition")
	FName ArenaName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle Transition")
	UBoxComponent* AllySpawnBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle Transition")
	UBoxComponent* EnemySpawnBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle Participants")
	TMap<FName, TSubclassOf<class ABaseCharacter>> AlliesMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle Participants")
	TArray<ABaseCharacter*> Allies;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle Participants")
	TArray<FName> AlliesNames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle Participants")
	TMap<FName, TSubclassOf<ABaseCharacter>> EnemyMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle Participants")
	TArray<ABaseCharacter*> Enemies;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle Participants")
	TArray<FName> EnemiesNames;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle Participants")
	class AMainPlayerController* PlayerControllerReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle Details")
	float DistanceBetweenCombatants;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle Turn Order")
	TArray<ABaseCharacter*> TurnOrder;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle Turn Order")
	int CurrentAction;
	/* Camera related Components and variables
	* Battle Camera and Spring Arm Component
	* boolean to activate/deactivate camera	(to implement)
	* Base values to control the Turn Rate for the camera when using keyboard keys or digital inputs (to implement)
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* BattleCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* BattleSpringArmComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	bool BattleCameraActive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera | Parameters")
	float BattleCameraTurnRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera | Parameters")
	float BattleCameraLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle Music")
	TMap<int, USoundBase*> BattleMusicMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle Music")
	USoundBase* ActiveBattleMusicReference;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**======================================================================================
	*							Setup Combat Functions
	* =======================================================================================

	/*Functions
	* Setup Battle - Always called. Setups basic parameters to Normal Battle. If it is not a Normal Battle, adjusts according to type
	* Setup Irregular Battle - BP Native Event. Receives parameters for Irregular Battle
	* AddAllies - Add allies that will fight to TArray
	* AddEnemies - Add enemies that will fight to TArray
	* SpawnFighters - Spawn allies and enemies
	* ActivateBattleCamera - Setups Battle Camera as active. 
	* OnOverlapBeing - Starts Battle once Character enters the Arena
	* 
	* 
	*/


	

	UFUNCTION(BlueprintCallable, Category = "Battle Setup")
	void SetupBattle(EFieldEffect ActiveFieldEffect, EVictoryCondition ActiveVictoryCondition);

	UFUNCTION(BlueprintCallable, Category = "Battle Setup")
	void SetupVictoryDetails(int DamageOrNumberOfTurns);

	FORCEINLINE	void SetPartToBreak(FName SetBodyPartToBreak) { PartToBreak = SetBodyPartToBreak; }

	//Modifiers should be set in BPs when using SetupIrregularBattle
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Battle Setup")
	void SetupIrregularBattle();

	UFUNCTION(BlueprintCallable, Category = "Battle Setup")
	void SetupAlliesNames(TArray<FName> AlliesNamesForBattleSetup);

	UFUNCTION(BlueprintCallable, Category = "Battle Setup")
	void SetupEnemiesNames(TArray<FName> EnemiesNamesForBattleSetup);

	UFUNCTION(BlueprintCallable, Category = "Battle Setup")
	void SetupChallengeNumber(int NumberOfChallenge);

	//Currently unused function
	UFUNCTION(BlueprintCallable, Category = "Battle Setup")
	void SetupCharacterStatus(ABaseCharacter* CharacterToSetupStatus);

	void SpawnFighters();

	UFUNCTION(BlueprintCallable, Category = "Battle Camera")
	void ActivateBattleCamera();

	UFUNCTION(BlueprintCallable, Category = "Battle End Setup")
	void ClearAllCombatants();

	UFUNCTION(BlueprintCallable, Category = "Battle End Setup")
	void EndBattle(bool BattleResult);

	UFUNCTION(BlueprintNativeEvent, Category = "Battle Music")
	void PlayBattleMusic();

	UFUNCTION(BlueprintNativeEvent, Category = "Battle Music")
	void EndBattleMusic();

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/**======================================================================================
	*							During Combat Functions
	* =======================================================================================
	* SetupTurnOrder - Decides the Turn Order between fighters. Allies have priority if wait time is the same. 
	* GetTurnOrder - Used to get the Turn Order to update Combat Widget - no need to implement because TurnOrder array is already edit anywhere
	* CombatStart - Once everything is set up, this function will start the combat
	* 
	*/

	UFUNCTION(BlueprintCallable, Category = "Combat Function")
	void SetupTurnOrder();

	UFUNCTION(BlueprintCallable, Category = "Combat Function")
	void TurnBegin();

	UFUNCTION(BlueprintCallable, Category = "Combat Function")
	void TurnEnd();

	UFUNCTION(BlueprintCallable, Category = "Combat Function")
	void CombatStart();

	UFUNCTION(BlueprintCallable, Category = "Combat Function")		
	void ActionStart(ABaseCharacter* CharacterToAct);

	UFUNCTION(BlueprintCallable, Category = "Combat Function")
	void ActionEnd();

	//Checks if there is a next action to be executed. If not, ends the turn
	UFUNCTION(BlueprintCallable, Category = "Combat Function")
	void NextAction();

	UFUNCTION(BlueprintCallable, Category = "Combat Function")
	bool AreAllAlliesAlive();

	UFUNCTION(BlueprintCallable, Category = "Combat Function")
	bool AreAllEnemiesAlive();

	UFUNCTION(BlueprintCallable, Category = "Combat Function")
	void UpdateHitAndDamageCount(bool IsAnAlly, int DamageCaused, int NumberOfHits);

	UFUNCTION(BlueprintCallable, Category = "Combat Function")
	void BreakHitAndDamageCount(bool IsAnAlly);

	UFUNCTION(BlueprintCallable, Category = "Combat Function")
	void AssignNewTarget(ABaseCharacter* CharacterToAssignNewTarget);

	UFUNCTION(BlueprintCallable, Category = "Combat Function")
	void CheckVictoryAndDefeatConditions();
	//Called after a montage animation ends and it will define the next action in the combat - Call AnimationEndDelayFunction if it crashes because of a change in stance
	UFUNCTION(BlueprintCallable, Category = "Combat Function")
	void AnimationEnd();

	//Sets a 0.2s timer to call AnimationEnd. Prevents a crash when changing stances due to a nullptr callback
	UFUNCTION(BlueprintCallable, Category = "Combat Function")
	void AnimationEndDelayFunction();

	//Event called after an animation ends to properly update all UI Widgets
	UFUNCTION(BlueprintNativeEvent, Category = "Combat Function")
	void UpdateWidgets();

	UFUNCTION(BlueprintNativeEvent, Category = "Combat Function")
	void WidgetCounterCall();

	UFUNCTION(BlueprintNativeEvent, Category = "Combat Function")
	void WidgetBattleResultAnnounce(bool BattleResult);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Combat Function")
	void BattleLogReport(const FString& ReportStringToLog);

	UFUNCTION(BlueprintCallable, Category = "Combat Function")
	void EnemiesActionSetup();

	UFUNCTION(BlueprintCallable, Category = "Combat Function")
	void SortRandomTarget(ABaseCharacter* CharacterToAssignNewTarget);


	/*This function will check if a counter or cover can be triggered when an special is about to be executed.
	* This function is only responsible for an initial check and, if it is successful, it will call the Effect Activation function on the Active Stance BP
	* However, the stance BP might have other conditions in it, so it is not guaranteed that a counter or cover will activate it.
	* This function determines when to call the BP function as follows:
	* If the target is enemy single, it will check for a counter activation on target character and it will check for a cover activation on everyone else
	* If the target is enemy all, it will check for counter activation on all target characters
	* Currently, there are no instance where ally single or ally all will use this function
	*/
	UFUNCTION(BlueprintCallable, Category = "Combat Function")
	void CheckAllOpponentsForCountersAndBlocks(ABaseCharacter* CharacterThatActivatedSkill);

	UFUNCTION(BlueprintCallable, Category = "Combat Function")
	void DamageAllOpponents(bool IsAnAlly, int CalculatedDamageFromAttackingCharacter, ABaseCharacter* AttackingCharacter);

	UFUNCTION(BlueprintCallable, Category = "Combat Function")
	void HealAllAllies(bool IsAnAlly, int CalculatedHealFromCharacter);

};
