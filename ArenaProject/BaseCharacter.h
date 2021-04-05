// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"



//Struct for various buffs and debuffs
USTRUCT(BlueprintType)
struct FStatus
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status")
	EEffectStatus StatusEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status")
	float StatusQuantity;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status")
	int StatusTurn;

};

//Struct for various conditions

USTRUCT(BlueprintType)
struct FCharacterCondition
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status")
	ECharacterStatus CharacterStatusEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status")
	float CharacterStatusQuantity;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status")
	int CharacterStatusTurn;

};


UCLASS()
class ARENAPROJECT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FString Name;

	/** Base Character
	/*	This is a class made to create characters (allies and enemies) which contains:
	/*	Base Values for Parameters (Health, Magic, etc.)
	/*	Skeletal Mesh and Equipment Socket Names
	/*	Damage information for Parts System
	/*  Base Weakness and Strenghts values
	/*  Execution for commands
	/*  Access to Skills, Stances, Equipment data
	*/

	/**	Base Parameters for Characters. Base Paramenters do not take equipment or external influences into account.
	/* Base Status : Health, Magic, Ability Points(AP), Attack(ATK), Defense(DEF), Technique(TEC), Evasion(EVA), Speed(SPD) and Agility(AGI)  
	/* StatusScaling : Multiplier for the stats. Used for buffs and debuffs
	/* Parts System(Damage to change part to damaged status) : RightArmDmgThreshold, LeftArmDmgThreshold, ChestDmgThreshold, LegsDmgThreshold, HeadDmgThreshold
	/* Parts System(Damage to change part to broken status) :RightArmBreakDmgThreshold, LeftArmBreakDmgThreshold, ChestBreakDmgThreshold, LegsBreakDmgThreshold, HeadBreakDmgThreshold
	/* Parts System(Number of combos to damage and/or break a part) :RightArmComboRes, LeftArmComboRes, ChestComboRes, LegsComboRes, HeadComboRes
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	int32 MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	int32 CurrentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	int MaxMagic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	int CurrentMagic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	int MagicRegeneration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	int SingleAttackMagicRegeneration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	int Max_AP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	int CurrentAP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	int AP_Regeneration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	int BaseATK;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	int BaseDEF;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	int BaseTEC;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	int BaseEVA;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	int BaseSPD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	int BaseAGI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	int WaitTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	int SPD_Regeneration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	ECharacterStatus CharacterMainStatus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	ECharacterLimitation CharacterMainLimitation;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Stats Scalling")
	float ScallingATK;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Stats Scalling")
	float ScallingDEF;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Stats Scalling")
	float ScallingTEC;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Stats Scalling")
	float ScallingEVA;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Stats Scalling")
	float ScallingSPD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Stats Scalling")
	float ScallingAGI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Stats Scalling")
	float ChargeValue;

	//These status are the ones after considering Scalling and the BattleSystemManager changes.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Final Stats")
	int FinalATK;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Final Stats")
	int FinalDEF;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Final Stats")
	int FinalTEC;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Final Stats")
	int FinalEVA;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Final Stats")
	int FinalSPD;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Final Stats")
	int FinalAGI;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Other Stats")
	bool bIsCharacterAnAlly;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Other Stats")
	bool bWasActiveSkillCountered;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Other Stats")
	float OverHealthMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Parts Stats")
	EBodyPartStatus RightArmStatus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Parts Stats")
	EBodyPartStatus LeftArmStatus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Parts Stats")
	EBodyPartStatus ChestStatus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Parts Stats")
	EBodyPartStatus LegsStatus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Parts Stats")
	EBodyPartStatus HeadStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Parts Stats | Dmg Threshold")
	int32 RightArmDmgThreshold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Parts Stats | Dmg Threshold")
	int32 LeftArmDmgThreshold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Parts Stats | Dmg Threshold")
	int32 ChestDmgThreshold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Parts Stats | Dmg Threshold")
	int32 LegsDmgThreshold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Parts Stats | Dmg Threshold")
	int32 HeadDmgThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Parts Stats | Break Dmg Threshold")
	int32 RightArmBreakDmgThreshold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Parts Stats | Break Dmg Threshold")
	int32 LeftArmBreakDmgThreshold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Parts Stats | Break Dmg Threshold")
	int32 ChestBreakDmgThreshold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Parts Stats | Break Dmg Threshold")
	int32 LegsBreakDmgThreshold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Parts Stats | Break Dmg Threshold")
	int32 HeadBreakDmgThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Parts Stats | Dmg Bonus")
	float NormalBodyPartDamageMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Parts Stats | Dmg Bonus")
	float DamagedBodyPartDamageMultiplier;
	//This cannot be equal to Normal Body Part Damage Multiplier or Damaged Body Part Damage Multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Parts Stats | Dmg Bonus")
	float BrokenBodyPartDamageMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Parts Stats | ComboResistance")
	int ResistanceAttackValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Parts Stats | ComboResistance")
	int RightArmComboRes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Parts Stats | ComboResistance")
	int LeftArmComboRes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Parts Stats | ComboResistance")
	int ChestComboRes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Parts Stats | ComboResistance")
	int LegsComboRes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Parts Stats | ComboResistance")
	int HeadComboRes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Parts Stats | ComboResistance")
	int ComboResRenewValue;


	/*Texts used to fill the battle log with hints about how the enemy characters might act
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Action Preview Text")
	FText StancePreviewText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Action Preview Text")
	FText LightAttackPreviewText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Action Preview Text")
	FText MediumAttackPreviewText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Action Preview Text")
	FText HeavyAttackPreviewText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Action Preview Text")
	FText VeryHeavyAttackPreviewText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Action Preview Text | JPN")
	FText StancePreviewTextJPN;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Action Preview Text | JPN")
	FText LightAttackPreviewTextJPN;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Action Preview Text | JPN")
	FText MediumAttackPreviewTextJPN;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Action Preview Text | JPN")
	FText HeavyAttackPreviewTextJPN;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Action Preview Text | JPN")
	FText VeryHeavyAttackPreviewTextJPN;


	/**Animation related variables and structures
	/* SocketNames = RightArmSocket, LeftArmSocket, HeadSocket, ChestSocket, LegsSocket. 
	/* SkillMontage = Montage that contains animation for skills and magic
	/* Hit Particles = Particles played when hit by enemy
	/* Hit Sound = SE when hit by enemy
	* AttackParticles = Particles played when hitting enemy
	* AttackSound = SE when attacking enemy
	*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	class UAnimMontage* SkillMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	class UParticleSystem* HitParticles; 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	UParticleSystem* AttackParticles;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	UParticleSystem* LearnSkillParticles;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	class USoundCue* HitSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	USoundCue* AttackSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	FName AnimationToPlay;

	/* Camera related Components and variables
	* Character Camera and Spring Arm Component
	* boolean to activate/deactivate camera	(to implement)
	* Base values to control the Turn Rate for the camera when using keyboard keys or digital inputs (to implement)
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* BaseCharacterCamera; 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* BaseCharacterSpringArmComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera | Parameters")
	bool ActivateCharacterCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera | Parameters")
	float BaseTurnRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera | Parameters")
	float BaseLookUpRate;

	/*Movement and Other Outside of Combat related variables
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int WalkSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	class AMainPlayerController* MainPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
	class USphereComponent* TalkRangeSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
	bool AllowTalk;

	class ANonCombatBaseCharacter* NPC;

	FVector StartingPosition;

	/*Stance related Variables 
	* TMap Holds the Reference to all Stances that can be used by the character
	* ActiveStance is the object of the current stance
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stance List")
	class UBaseStanceComponent* ActiveStance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stance List")
	TMap<FString, TSubclassOf<UBaseStanceComponent>> StanceMap;

	//Boxing will be used as initial stance if not specified
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stance List")
	FName InitialStance;


	/*Variables related to battle actions and conditions
	*/

	//Battle System Manager Reference
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle System Reference")
	class ABattleSystemManager* BattleSystemReference;

	//Chain of Commands
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	TArray<EAttackType> Commands;

	//TArray to hold all status changes from buffs, debuffs and other. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	TArray<FStatus> CharacterStatusChanges;

	//TArray to hold all condition changes from special skills.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Condition")
	TArray<FCharacterCondition> CharacterConditionChanges;
	
	//ENUM that holds which action the character will take in his turn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Action")
	ECharacterAction CharacterAction; 

	//Name of the Special Action or Stance to change
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Action")
	FName SpecialAction;

	//The type of target (single or all. Ally or enemy)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Action")
	ETargetDefinition AttackTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Action")
	ABaseCharacter* SingleTargetReference;

	//Damage to be caused to enemy 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Action")
	float DamageCaused;


	//Damage taken that will be shown on screen 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Action")
	int DamageTakenToShowOnScreen;

	//Which attack is being executed during a combo
	int CurrentComboAttack;

	//Reference to the character animation reference class 
	class UCharacterAnimationManager* CharacterAnimationReference;

	//Reference to a table that will contain the base stats in accord to the WorldConcext variable in the player controller 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	class UDataTable* LevelTable;


	/*Skill Related Variables
	* ActiveSkillList - List of Skills that belongs to the character. This is the BP that contains the Spreadsheet
	* ActiveSkill - This data will remain on ActiveSkillList as a Struct. ActiveSkillList->ActiveSkill
	* 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	class USkillSetComponent* ActiveSkillList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Art")
	class USpecialSetComponent* ActiveSpecialArtList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stance List")
	TMap<FString, FSkillVariableData> SkillVariableInformationMap;

	/*AI behavior related variables
	*/

	//Percenteage of using a combo that consumes little SPD (30% of available). Value must be between 0 and 1. Sum must be 1 with other Percenteages.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Behavior")
	float LightAttackPercenteage;

	//Minimum SPD required to use Light Attack Strategy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Behavior")
	int LightStrategySPDThreshold;

	//Maximum number of special attacks that can be used in Light Strategy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Behavior")
	int LightStrategyAttackNumberThreshold;

	//Percenteage of using a combo that consumes medium SPD (50% of available). Value must be between 0 and 1. Sum must be 1 with other Percenteages.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Behavior")
	float MediumAttackPercenteage;

	//Minimum SPD required to use Medium Attack Strategy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Behavior")
	int MediumStrategySPDCost;

	//Maximum number of special attacks that can be used in Medium Strategy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Behavior")
	int MediumStrategyAttackNumberThreshold;

	//Percenteage of using a combo that consumes most SPD (80% of available). Value must be between 0 and 1. Sum must be 1 with other Percenteages.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Behavior")
	float HeavyAttackPercenteage;

	//Minimum SPD required to use Heavy Attack Strategy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Behavior")
	int HeavyStrategySPDCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Behavior")
	int VeryHeavyStrategySPDCost;

	//Percenteage of choosing a change in stance. Value must be between 0 and 1. Sum must be 1 with other Percenteages.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Behavior")
	float StancePercenteage;

	//Enum that defines which strategy AI will use
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Behavior")
	EAIStrategy AIStrategy;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Behavior")
	float SpecialPercenteage;
	*/

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//FUNCTIONS

	/*Basic Movement functions
	* Forward and side to side input functions
	*/
	void MoveForward(float value);
	void MoveRight(float value);

	/*Outside of combat related functions
	*/

	UFUNCTION(BlueprintCallable, Category = "Character Interaction")
	void Interact();

	UFUNCTION(BlueprintCallable, Category = "Character Interaction")
	void MainMenuEscape();

	UFUNCTION()
	virtual void TalkRangeSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void TalkRangeSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	struct FDialogue* GetDialogueFromNPC(int WorldContext, int LocalContext);

	/*Basic Camera Movement functions
	* Yaw and Pitch input functions
	*/

	void Turn(float value);
	void LookUp(float value);
	void ToggleBaseCharacterCamera();

	/*Stance related functions
	* Cycle to next stance - Function for debugging
	* Cycle to previous stance - Function for debugging
	* StanceNumber - int for debugging
	* ChangeStance - Changes to specific Stance based on the Name
	* ChangeStanceAfterAnimNotify - Changes to Stance based on ActiveSkill and determines if Critical should be used
	* CheckStanceChange - Checks if there's a need to change stances. Returns a bool
	*/

	int StanceNumber;
	
	UFUNCTION(BlueprintCallable)
	void NextStance();

	UFUNCTION(BlueprintCallable)
	void PreviousStance();

	UFUNCTION(BlueprintCallable)
	void ChangeStance(FName StanceName);

	UFUNCTION(BlueprintCallable)
	void ChangeStanceAfterAnimNotify(bool Critical);


	UFUNCTION(BlueprintCallable)
	bool CheckStanceChange(FName NextStance);

	/*Skill related functions
	* Execute Skill
	* 
	*/
	UFUNCTION(BlueprintCallable)
	void ExecuteSkill(FName RowName);

	/*Special Art related functions
	*/

	UFUNCTION(BlueprintCallable)
	void ExecuteSpecialArt(FName SkillName);

	UFUNCTION(BlueprintCallable)
	void LoadSkillVariableData();

	/* Battle Action related functions
	* CalculateSkillDamage(ESkillParameter, Scaling) - calculates the damage that a skill/special will cause BEFORE considering target Defense
	* CalculateRegularDamage() - calculates the damage that a normal attack will cause BEFORE considering target Defense
	* NextComboAttack() - call to execute the next attack in a combo
	* BreakTurn() - call to end the attacking character turn. Either by defeat or a counter
	*/

	UFUNCTION(BlueprintCallable)
	void SetupAttackCommands(TArray<EAttackType> InputtedCommands, ABaseCharacter* TargetToAttack);


	/*Remember to setup SingleTargetReference if the special targets a single character
	* This is, in this build, done in after target selection in combat widget
	*/
	UFUNCTION(BlueprintCallable)
	void SetupSpecialCommand(FName SpecialAttackName, int MPCost);

	UFUNCTION(BlueprintCallable)
	void SetupStanceChange(FName StanceToChange);

	UFUNCTION(BlueprintCallable)
	void SetupItemUse();

	UFUNCTION(BlueprintCallable)
	void StartAction();

	UFUNCTION(BlueprintCallable)
	void NextComboAttack();

	UFUNCTION(BlueprintCallable)
	void EndAction();

	UFUNCTION(BlueprintCallable)
	void BeginTurn();

	UFUNCTION(BlueprintCallable)
	void EndTurn();

	/* Use this function to fill the battle log with information about when skills or specials are used
	*/
	UFUNCTION(BlueprintCallable)
	void ReportSkill();

	/* Use this function to fill the battle log with information about the active stance
	*/
	UFUNCTION(BlueprintCallable)
	void ReportStance();

	UFUNCTION(BlueprintCallable)
	float CalculateSkillDamage(ESkillParameter SkillParameter, float ParameterScaling);
	
	UFUNCTION(BlueprintCallable)
	float CalculateRegularDamage(EAttackType InputtedCommand);

	UFUNCTION(BlueprintCallable)
	float CalculateDamageWithHitBonus(float CalculatedDamage, bool bIsAnAlly);

	/*As the current build, the formula will be static
	* DamageThatWasToBeCaused + Final ATK + Final TEC
	* This damage will be applied to ReceiveCounter which will not use Final DEF to mitigate damage
	* It will also be multiplied by the condition of the body part
	*/
	UFUNCTION(BlueprintCallable)
	float CalculateCounterDamage(float DamageThasWasToBeCaused);

	UFUNCTION(BlueprintCallable)
	void ReceiveSkillDamage(float CalculatedDamageToReceive, EBodyPartTarget TargetBodyPart, ABaseCharacter* AttackerReference);

	UFUNCTION(BlueprintCallable)
	void ReceiveRegularDamage(float CalculatedDamageToReceive, EAttackType TypeOfAttack, ABaseCharacter* AttackerReference);

	UFUNCTION(BlueprintCallable)
	void ReceiveDirectDamage(int CalculatedDamageToReceive);


	/*Calculates the damage a counter will cause
	* Updates bWasActiveSkillCountered to true 
	* Shows Received Damage
	* This function does not call End Turn, the character making the Counter must call it
	*/
	UFUNCTION(BlueprintCallable)
	void ReceiveCounter(int DamageToBeReceived, EBodyPartTarget TargetBodyPart);

	UFUNCTION(BlueprintCallable)
	void ShowReceivedDamage(bool IsAttackMissed, int DamageReceived, int NumberOfHitsTaken);

	UFUNCTION(BlueprintCallable)
	void ReceiveHPHeal(int QuantityToHeal);

	UFUNCTION(BlueprintCallable)
	void ShowReceivedHeal(int HealedQuantity);

	/*Returns false if character CANNOT make that action
	* Returns true if character can make that action
	*/
	UFUNCTION(BlueprintCallable)
	bool CanCharacterExecuteAction(ECharacterAction ActionToBeTaken);

	bool bIsAttackAvoided(ABaseCharacter* AttackerReference);

	float GetBodyPartStatusDamageMultiplier(EBodyPartTarget TargetBodyPart);

	//Checks if a Body Part was damaged or broken based on the DamageToBodyPart and DamageToResistance
	void CheckForUpdateInBodyPartCondition(EBodyPartTarget TargetBodyPart, int DamageToBodyPart, int DamageToResistance);

	/*Other battle related functions such as damage and status calculations
	* and UI related functions
	* AddStatusEffect - Adds a single status effect and triggers it's effect once. Adds the status effect to the CharacterStatusChange array
	* ApplyStatusEffect - Switch/Case with the logic of each status
	* ApplyAllStatusEffect - Loops in CharacterStatusChange array and apply all the status effects in there. Eliminates the ones that ended. Must be used in the beginning of the turn.
	* ClearStatusEffect - Returns scalling back to normal so that effects from different turns do not stack upon each other.
	*/

	UFUNCTION(BlueprintCallable)
	void UpdateFinalStatus(ESkillParameter StatusToUpdate);
	
	UFUNCTION(BlueprintCallable)
	void AddStatusEffect(EEffectStatus StatusToAdd, float Quantity, int Turns);

	UFUNCTION(BlueprintCallable)
	void ApplyStatusEffect(EEffectStatus StatusToApply, float Quantity);

	UFUNCTION(BlueprintCallable)
	void ApplyAllStatusEffect();

	UFUNCTION(BlueprintCallable)
	void ClearStatusEffect();

	UFUNCTION(BlueprintCallable)
	void AddConditionEffect(ECharacterStatus ConditionToAdd, float Quantity, int Turns);

	UFUNCTION(BlueprintCallable)
	void ApplyConditionEffect(ECharacterStatus ConditionToApply, float Quantity);

	UFUNCTION(BlueprintCallable)
	void ApplyAllConditionEffect();

	UFUNCTION(BlueprintCallable)
	void ClearConditionEffect();

	UFUNCTION(BlueprintCallable)
	void ActivateCounter(ABaseCharacter* CharacterToBeCountered, EBodyPartTarget TargetBodyPart);

	UFUNCTION(BlueprintCallable)
	void ActivateCover(ABaseCharacter* CharacterThatWillTargetCover);

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetupAIAction();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI")
	void SetupAICustomAction();


	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetupAIChangeStance();

	/*Setups the Combo Attack for AI
	* 0 for Light Strategy
	* 1 for Medium
	* 2 for Heavy
	* 3 for Very Heavy
	*/
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetupAIComboAttack(EAIStrategy Choice);

	UFUNCTION(BlueprintCallable, Category = "AI")
	void AIActionReport(EAIStrategy ActionTaken);

	UFUNCTION(BlueprintCallable)
	void ChangeStanceAfterTimer();

	UFUNCTION(BlueprintCallable)
	void SetupCharacterStatus(int CharacterLevel);

};
