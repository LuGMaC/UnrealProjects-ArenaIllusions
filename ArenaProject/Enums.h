// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Enums.generated.h"

/**
 * Non Combat Character Enums
 */

UENUM(BlueprintType)
enum class EDialogueType : uint8
{
	//All possible Parameters that can be used on dialogues
	EDT_Normal	UMETA(DisplayName = "Normal Dialogue"),
	EDT_Loop	UMETA(DisplayName = "Loop Dialogue"),
	EDT_MAX		UMETA(DisplayName = "DefaultMAX")
};

USTRUCT(BlueprintType)
struct FDialogue : public FTableRowBase
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, Category = "World Context")
		FName DialogueID;

	UPROPERTY(VisibleAnywhere, Category = "World Context")
		int WorldContext;

	UPROPERTY(VisibleAnywhere, Category = "World Context")
		int LocalContext;

	UPROPERTY(VisibleAnywhere, Category = "World Context")
		int ConversationID;

	UPROPERTY(VisibleAnywhere, Category = "Conversation")
		int LineID;

	UPROPERTY(VisibleAnywhere, Category = "Conversation")
		int LinesRemaining;

	UPROPERTY(VisibleAnywhere, Category = "Conversation")
		FName TalkPersonName;

	UPROPERTY(VisibleAnywhere, Category = "Text")
		FText EnglishText;

	UPROPERTY(VisibleAnywhere, Category = "Text")
		FText JapaneseText;

	UPROPERTY(VisibleAnywhere, Category = "Text")
		EDialogueType DialogueType;

	UPROPERTY(VisibleAnywhere, Category = "Description")
		FText DialogueDescription;

	UPROPERTY(VisibleAnywhere, Category = "Special Event ID")
		int SpecialEventID;

};

/**
 *  Character Animation Manager Enums
 */

UENUM(BlueprintType)
enum class EBattleAnimationSetup : uint8
{
	//All possible Parameters that can be used on battle animations
	//Stance change is not here because it changes the animBP itself
	//Damage taken is by flipping a bool, so it's not here either
	EBAS_AttackHigh		UMETA(DisplayName = "Regular Attack: High"),
	EBAS_AttackMid		UMETA(DisplayName = "Regular Attack: Mid"),
	EBAS_AttackLow		UMETA(DisplayName = "Regular Attack: Low"),
	EBAS_AttackRight	UMETA(DisplayName = "Regular Attack: Right"),
	EBAS_AttackLeft		UMETA(DisplayName = "Regular Attack: Left"),
	EBAS_Skill			UMETA(DisplayName = "Skill"),
	EBAS_Special		UMETA(DisplayName = "Special"),
	EBAS_Item			UMETA(DisplayName = "Item"),
	EBAS_Counter		UMETA(DisplayName = "Counter"),
	EBAS_Run			UMETA(DisplayName = "Run"),
	EBAS_Revive			UMETA(DisplayName = "Revive"),
	EBAS_Defeat			UMETA(DisplayName = "Defeat"),
	EBAS_SpecialDefeat	UMETA(DisplayName = "SpecialDefeat"),
	EBAS_Cover			UMETA(DisplayName = "Cover"),
	EBAS_Hurt			UMETA(DisplayName = "Hurt"),
	EBAS_Dodge			UMETA(DisplayName = "Dodge"),
	EBAS_MAX			UMETA(DisplayName = "DefaultMAX")

	//Unused enums

};



/**
 * Base Character Enums
 */

UENUM(BlueprintType)
enum class EBodyPartStatus : uint8
{
	EBPS_Normal		UMETA(DisplayName = "Normal"),
	EBPS_Damaged	UMETA(DisplayName = "Damaged"),
	EBPS_Broken		UMETA(DisplayName = "Broken"),
	EBPS_MAX		UMETA(DisplayName = "DefaultMAX")

};

UENUM(BlueprintType)
enum class ECharacterAction : uint8
{
	ECA_Attack		UMETA(DisplayName = "Attack"),
	ECA_Skill		UMETA(DisplayName = "Skill"),
	ECA_Special		UMETA(DisplayName = "Special"),
	ECA_Stance		UMETA(DisplayName = "Stance"),
	ECA_Item		UMETA(DisplayName = "Item"),
	ECA_Null		UMETA(DisplayName = "Null"),
	ECA_Defeated	UMETA(DisplayName = "Defeated"),
	ECA_Counter 	UMETA(DisplayName = "Counter"),
	ECA_Cover		UMETA(DisplayName = "Cover"),
	EBPS_MAX		UMETA(DisplayName = "DefaultMAX")

};

UENUM(BlueprintType)
enum class ECharacterStatus : uint8
{
	ECS_Normal		UMETA(DisplayName = "Normal"),
	ECS_Defeated	UMETA(DisplayName = "Defeated"),
	ECS_Poisoned	UMETA(DisplayName = "Poisoned"),
	ECS_Cold		UMETA(DisplayName = "Cold"),
	ECS_Burn		UMETA(DisplayName = "Burn"),
	ECS_Tired		UMETA(DisplayName = "Tired"),
	ECS_Paralyzed	UMETA(DisplayName = "Paralyzed"),
	ECS_Blind		UMETA(DisplayName = "Blind"),
	ECS_Angry		UMETA(DisplayName = "Angry"),
	ECS_Revive		UMETA(DisplayName = "Revive"),
	ECS_Aura		UMETA(DisplayName = "Aura"),
	ECS_Fast		UMETA(DisplayName = "Fast"),
	ECS_LastResort	UMETA(DisplayName = "LastResort"),
	ECS_Death		UMETA(DisplayName = "Death"),
	ECS_MAX			UMETA(DisplayName = "DefaultMAX")

};

UENUM(BlueprintType)
enum class ECharacterLimitation : uint8
{
	ECL_None			UMETA(DisplayName = "None"),
	ECL_NoAttacks		UMETA(DisplayName = "No Attacks can be done"),
	ECL_NoSpecial		UMETA(DisplayName = "No Specials can be done"),
	ECL_NoStanceChange	UMETA(DisplayName = "No Stance can be changed"),
	ECL_NoSkill			UMETA(DisplayName = "No Skills can be done"),
	ECL_All				UMETA(DisplayName = "Character can't do shit"),
	ECL_MAX				UMETA(DisplayName = "DefaultMAX")

};



UENUM(BlueprintType)
enum class EAttackType : uint8
{
	EAT_Empty	UMETA(DisplayName = " "),
	EAT_High	UMETA(DisplayName = "High"),
	EAT_Mid		UMETA(DisplayName = "Mid"),
	EAT_Low		UMETA(DisplayName = "Low"),
	EAT_Left	UMETA(DisplayName = "Left"),
	EAT_Right	UMETA(DisplayName = "Right"),
	EAT_Special UMETA(DisplayName = "Special"),
	EAT_MAX		UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EAIStrategy : uint8
{
	EAIS_None		UMETA(DisplayName = "None"),
	EAIS_Stance		UMETA(DisplayName = "Stance"),
	EAIS_Light		UMETA(DisplayName = "Light Combo"),
	EAIS_Medium		UMETA(DisplayName = "Medium Combo"),
	EAIS_Heavy		UMETA(DisplayName = "Heavy Combo"),
	EAIS_VeryHeavy	UMETA(DisplayName = "Very Heavy Combo"),
	EAIS_MAX		UMETA(DisplayName = "DefaultMAX")

};


/**
 * BaseStanceComponent Component Enums
 */

UENUM(BlueprintType)
enum class EEffectType : uint8
{
	/**Possible triggers for the first Effect
	* Imediate - Execute as soon as stance is equipped
	* Counter - Execute when enemy is about to attack character. The effect happens before enemy attack.
	* Cover - Executes when enemy is about to attack character. Changes the target attack. Effect happens before enemy attack.
	* FollowUp - Execute when ally attacks with a combo. Effect happens after ally ends combo.
	* Revenge - Execute when enemy attacks character. The Effect happens after the enemy attack.
	*/
	EET_Imediate	UMETA(DisplayName = "Imediate"),
	EET_Counter		UMETA(DisplayName = "Counter"),
	EET_Cover		UMETA(DisplayName = "Cover"),
	EET_FollowUp	UMETA(DisplayName = "FollowUp"),
	EET_Revenge		UMETA(DisplayName = "Revenge"),
	EET_MAX			UMETA(DisplayName = "DefaultMAX")
};

/**
 * SkillSet Component Enums
 */

UENUM(BlueprintType)
enum class EEffectStatus : uint8
{
	//All possible effects from skills
	EES_Empty					UMETA(DisplayName = " "),
	EES_DamageOverTime			UMETA(DisplayName = "Skill Effect: DamageOverTime(+)"),
	EES_DamageOverTimeMax		UMETA(DisplayName = "Skill Effect: DamageOverTime(%)"),
	EES_HealHP					UMETA(DisplayName = "Skill Effect: HealHP(+)"),
	EES_HealMaxHP				UMETA(DisplayName = "Skill Effect: HealHP(%)"),
	EES_HealMP					UMETA(DisplayName = "Skill Effect: HealMP(+)"),
	EES_HealMaxMP				UMETA(DisplayName = "Skill Effect: HealMP(%)"),
	EES_HealAP					UMETA(DisplayName = "Skill Effect: HealAP(+)"),
	EES_HealMaxAP				UMETA(DisplayName = "Skill Effect: HealAP(%)"),
	EES_BuffATK					UMETA(DisplayName = "Skill Effect: BuffATK"),
	EES_DebuffATK				UMETA(DisplayName = "Skill Effect: DebuffATK"),
	EES_BuffDEF					UMETA(DisplayName = "Skill Effect: BuffDEF"),
	EES_DebuffDEF				UMETA(DisplayName = "Skill Effect: DebuffDEF"),
	EES_BuffSPD					UMETA(DisplayName = "Skill Effect: BuffSPD"),
	EES_DebuffSPD				UMETA(DisplayName = "Skill Effect: DebuffSPD"),
	EES_BuffTEC					UMETA(DisplayName = "Skill Effect: BuffTEC"),
	EES_DebuffTEC				UMETA(DisplayName = "Skill Effect: DebuffTEC"),
	EES_BuffEVA					UMETA(DisplayName = "Skill Effect: BuffEVA"),
	EES_DebuffEVA				UMETA(DisplayName = "Skill Effect: DebuffEVA"),
	EES_BuffAGI					UMETA(DisplayName = "Skill Effect: BuffAGI"),
	EES_DebuffAGI				UMETA(DisplayName = "Skill Effect: DebuffAGI"),
	EES_BodyPartExtraDamage		UMETA(DisplayName = "Skill Effect: BPExtraDamage"),
	EES_Charge					UMETA(DisplayName = "Skill Effect: Charge"),
	EES_FollowUp				UMETA(DisplayName = "Skill Effect: FollowUp"),
	EES_ComboSupport			UMETA(DisplayName = "Skill Effect: ComboSupport"),
	EES_ComboBreak				UMETA(DisplayName = "Skill Effect: ComboBreak"),
	EES_Revive					UMETA(DisplayName = "Skill Effect: Revive"),
	EES_MAX						UMETA(DisplayName = "Skill Effect: DefaultMAX")
};

UENUM(BlueprintType)
enum class ETargetDefinition : uint8
{
	//All Possible Target possibilities for skills
	ETD_AllySingle		UMETA(DisplayName = "Target: AllySingle"),
	ETD_EnemySingle		UMETA(DisplayName = "Target: EnemySingle"),
	ETD_AllyAll			UMETA(DisplayName = "Target: AllyAll"),
	ETD_EnemyAll		UMETA(DisplayName = "Target: EnemyAll"),
	ETD_MAX				UMETA(DisplayName = "Target: DefaultMAX")
};

UENUM(BlueprintType)
enum class EBodyPartTarget : uint8
{
	//All Possible Body Parts than can be targeted by skill
	EBPT_Head		UMETA(DisplayName = "Body Part Target: Head"),
	EBPT_Chest		UMETA(DisplayName = "Body Part Target: Chest"),
	EBPT_Legs		UMETA(DisplayName = "Body Part Target: Legs"),
	EBPT_LeftArm	UMETA(DisplayName = "Body Part Target: LeftArm"),
	EBPT_RightArm	UMETA(DisplayName = "Body Part Target: RightArm"),
	EBPT_MAX		UMETA(DisplayName = "Body Part Target: DefaultMAX")
};

UENUM(BlueprintType)
enum class ESkillParameter : uint8
{
	//All possible Parameters that can be used when calculating skill damage/heal
	ESP_EMPTY	UMETA(DisplayName = " "),
	ESP_ATK		UMETA(DisplayName = "Skill Parameter: ATK"),
	ESP_DEF		UMETA(DisplayName = "Skill Parameter: DEF"),
	ESP_SPD		UMETA(DisplayName = "Skill Parameter: SPD"),
	ESP_TEC		UMETA(DisplayName = "Skill Parameter: TEC"),
	ESP_EVA		UMETA(DisplayName = "Skill Parameter: EVA"),
	ESP_AGI		UMETA(DisplayName = "Skill Parameter: AGI"),
	ESP_MAX		UMETA(DisplayName = "Skill Parameter: DefaultMAX")
};


USTRUCT(BlueprintType)
struct FSkillCustomCell
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Custom Data")
	EEffectStatus CustomEffect1;


};

USTRUCT(BlueprintType)
struct FSkillVariableData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Variable Data")
	FString SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Variable Data")
	int UsageCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Variable Data")
	FSkillCustomCell Customize01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Variable Data")
	FSkillCustomCell Customize02;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Variable Data")
	FSkillCustomCell Customize03;

};

USTRUCT(BlueprintType)
struct FCharacterSkillsVariableData
{
	GENERATED_BODY()

	//TMap that uses the name of the Skill as key and saves the struct variables of it.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Custom Data")
	TMap<FString, FSkillVariableData> CharacterSkillVariableData;
};

/**
 * SpecialArtSet Component Enums
 */

UENUM(BlueprintType)
enum class ESpecialArtEffectStatus : uint8
{
	//All possible effects from skills
	ESAES_Empty						UMETA(DisplayName = " "),
	ESAES_Damage					UMETA(DisplayName = "Special Art Effect: Damage"),
	ESAES_DmgBodyPart				UMETA(DisplayName = "Special Art Effect: DmgBodyPart"),
	ESAES_Burn						UMETA(DisplayName = "Special Art Effect: Burn"),
	ESAES_Freeze					UMETA(DisplayName = "Special Art Effect: Freeze"),
	ESAES_Bind						UMETA(DisplayName = "Special Art Effect: Bind"),
	ESAES_Death						UMETA(DisplayName = "Special Art Effect: Death"),
	ESAES_ProtectAllies				UMETA(DisplayName = "Special Art Effect: ProtectAllies"),
	ESAES_HealSelf					UMETA(DisplayName = "Special Art Effect: HealSelf"),
	ESAES_HealAll					UMETA(DisplayName = "Special Art Effect: HealAll"),
	ESAES_ReviveSelf				UMETA(DisplayName = "Special Art Effect: ReviveSelf"),
	ESAES_ReviveAll					UMETA(DisplayName = "Special Art Effect: ReviveAll"),
	ESAES_Awaken					UMETA(DisplayName = "Special Art Effect: Awaken"),
	ESAES_Desperate					UMETA(DisplayName = "Special Art Effect: Desperate"),
	ESAES_Illuminated				UMETA(DisplayName = "Special Art Effect: Illuminated"),
	ESAES_AfterImage				UMETA(DisplayName = "Special Art Effect: AfterImage"),
	ESAES_CleanseEnemy				UMETA(DisplayName = "Special Art Effect: Cleanse Enemy"),
	ESAES_CleanseSelf				UMETA(DisplayName = "Special Art Effect: Cleanse Self"),
	ESAES_ReleaseAll				UMETA(DisplayName = "Special Art Effect: ReleaseAll"),
	ESAES_MAX						UMETA(DisplayName = "Special Art Effect: DefaultMAX")

};

UENUM(BlueprintType)
enum class ESpecialArtActivationCondition : uint8
{
	//All possible effects from skills
	ESAAC_Empty						UMETA(DisplayName = " "),
	ESAAC_DamageAboveX				UMETA(DisplayName = "Special Art Condition: Damage Above X"),
	ESAAC_DamageAboveMax			UMETA(DisplayName = "Special Art Condition: Damage Above X %"),
	ESAAC_EnemyDamageAboveX			UMETA(DisplayName = "Special Art Condition: Enemy Damage Above X"),
	ESAAC_EnemyDamageAboveMax		UMETA(DisplayName = "Special Art Condition: Enemy Damage Above X %"),
	ESAAC_DefeatedAlly				UMETA(DisplayName = "Special Art Condition: Defeated Ally"),
	ESAAC_DefeatedSelf				UMETA(DisplayName = "Special Art Condition: Defeated Self"),
	ESAAC_AllAlliesAlive			UMETA(DisplayName = "Special Art Condition: All Allies Alive"),
	ESAAC_AllEnemiesAlive			UMETA(DisplayName = "Special Art Condition: All Enemies Alive"),
	ESAAC_MAX						UMETA(DisplayName = "Special Art Condition: DefaultMAX")

};



/**
 * BattleSystem Manager Enums & Struct
 */

USTRUCT(BlueprintType)
struct FCharacterLevelAndStatus : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Level Status")
	int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	int32 MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	int32 CurrentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Other Stats")
	float OverHealthMultiplier;
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Base Stats")
	int SPD_Regeneration;


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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Initial Stance")
	FName InitialStance;
};

UENUM(BlueprintType)
enum class EVictoryCondition : uint8
{
	//All Possible Conditions for Winning a fight
	EVC_DefeatAll		UMETA(DisplayName = "Victory Condition: Defeat All"),
	EVC_Survive			UMETA(DisplayName = "Victory Condition: Survive X Turns"),
	EVC_CauseDamage		UMETA(DisplayName = "Victory Condition: Cause X Damage"),
	EVC_BreakPart		UMETA(DisplayName = "Victory Condition: Break X Body Part"),
	EVC_MAX				UMETA(DisplayName = "Victory Condition: DefaultMAX")
};

UENUM(BlueprintType)
enum class EDefeatCondition : uint8
{
	//All Possible Conditions for losing a fight
	EDC_AllDefeated		UMETA(DisplayName = "Losing Condition: All Defeated"),
	EDC_TurnLimit		UMETA(DisplayName = "Losing Condition: X Turns have passed"),
	EDC_TakeDamage		UMETA(DisplayName = "Losing Condition: Take X Damage"),
	EDC_BrokenPart		UMETA(DisplayName = "Losing Condition: Break X Body Part"),
	EDC_MAX				UMETA(DisplayName = "Losing Condition: DefaultMAX")
};

UENUM(BlueprintType)
enum class EFieldEffect : uint8
{
	//Some possible conditions for Field Effects
	EFE_OutsideOfBattle		UMETA(DisplayName = "Outside of Battle"),
	EFE_NormalBattle		UMETA(DisplayName = "Field Effect: Normal"),
	EFE_Heat				UMETA(DisplayName = "Field Effect: Heat"),
	EFE_Freeze				UMETA(DisplayName = "Field Effect: Freeze"),
	EFE_HeavyGravity		UMETA(DisplayName = "Field Effect: HeavyGravity"),
	EFE_MiasmaField			UMETA(DisplayName = "Field Effect: MiasmaField"),
	EFE_Irregular			UMETA(DisplayName = "Field Effect: Irregular"),
	EFE_MAX				UMETA(DisplayName = "Field Effect: DefaultMAX")
};

/**
 * Player Controller Enums
 */

UENUM(BlueprintType)
enum class EUserWidgetStatus : uint8
{
	/* Player Controller enums in oder to call either the dialogue screen
	* or the combat main menu or other UIs in the future
	*/
	EUWS_MainMenu			UMETA(DisplayName = "Main Menu Widget"),
	EUWS_Dialogue			UMETA(DisplayName = "Dialogue Widget"),
	EUWS_CombatMainMenu		UMETA(DisplayName = "Combat: Main Menu"),
	EUWS_MAX				UMETA(DisplayName = "User Widget Status: DefaultMAX")
};

/*=========================================================================================================================================
* 
*												Version 2 Enums and Struts 
* 
* =========================================================================================================================================
*/

UENUM(BlueprintType)
enum class EAttackAlignment : uint8
{
	//All possible Parameters for the Combat Character
	EAA_Order		UMETA(DisplayName = "Attack Alignment: Order"),
	EAA_Chaos		UMETA(DisplayName = "Attack Alignment: Chaos"),
	EAA_Neutral		UMETA(DisplayName = "Attack Alignment: Neutral"),
	EAA_MAX			UMETA(DisplayName = "Attack Alignment: MAX")
};

UENUM(BlueprintType)
enum class ECombatCharacterParameter : uint8
{
	//All possible Parameters for the Combat Character
	ECCP_ATK		UMETA(DisplayName = "Parameter: ATK"),
	ECCP_DEF		UMETA(DisplayName = "Parameter: DEF"),
	ECCP_TEC		UMETA(DisplayName = "Parameter: TEC"),
	ECCP_RES		UMETA(DisplayName = "Parameter: RES"),
	ECCP_EVA		UMETA(DisplayName = "Parameter: EVA"),
	ECCP_AGI		UMETA(DisplayName = "Parameter: AGI"),
	ECCP_SPD		UMETA(DisplayName = "Parameter: SPD"),
	ECCP_MAX		UMETA(DisplayName = "Parameter: DefaultMAX")
};


USTRUCT(BlueprintType)
struct FSkillV2 : public FTableRowBase
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
	float Scaling1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data | Parameter 2")
	float Scaling2;

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
	ECombatCharacterParameter OrderEffect;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data | Effect1")
	float EQuantity1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data | Effect2")
	ECombatCharacterParameter ChaosEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data | Effect2")
	float EQuantity2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data")
	int NeutralSPCost;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data")
	FName StanceChange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data")
	int NumberOfHits;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data")
	int32 UsageCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Data")
	FText Description;

};

USTRUCT(BlueprintType)
struct FCharacterLevelAndStatusV2 : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Level Status")
	int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	int32 MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	int32 CurrentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	int MaxSP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	int CurrentSP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	int SP_Regeneration_Skill;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	int SP_Regeneration_SingleAttack;
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
	int BaseRES;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	int BaseEVA;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	int BaseAGI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Base Stats")
	int BaseSPD;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Base Stats")
	int SPD_Regeneration;


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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Initial Stance")
	FName InitialStance;

	//Add AP Mechanics parameters


};


class ARENAPROJECT_API Enums
{
public:
	Enums();
	~Enums();
};
