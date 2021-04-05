// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Enums.h"
#include "SpecialSetComponent.generated.h"

USTRUCT(BlueprintType)
struct FSpecialArt : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Special Art Data")
	FName SpecialArtName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Special Art Data")
	uint8 MPCost;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Special Art Data | Parameter 1")
	ESkillParameter Parameter1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Special Art Data | Parameter 1")
	float Scaling1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Special Art Data | Parameter 2")
	ESkillParameter Parameter2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Special Art Data | Parameter 2")
	float Scaling2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Special Art Data | Parameter 3")
	ESkillParameter Parameter3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Special Art Data | Parameter 3")
	float Scaling3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Special Art Data")
	ETargetDefinition Target;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Special Art Data")
	EBodyPartTarget BodyPartTarget;

	/*Effects
	* Special Art Effect: Determines which effect will take place
	* Duration: Number of turns that the effect will last. 0 will mean it's instantaneous.
	* Quantity: Scaling Parameter as follows:
	* 
	* HealSelf,HeallAll,ReviveAll,ReviveSelf - % of HP to be restored
	* Awaken, Desperate, Illuminated, AfterImage - Will scale different parameters according to Quantity
	* DmgBodyPart: How much damage will be added to the body part when the combo hits
	* Damage: How much damage will be added when the combo hits
	* ReleaseAll : Extra Bonus multiplier per buff released
	* CleanseEnemy, CleanseSelf, Burn, Freeze, Bind, Death, ProtectAllies: Quantity will not apply	
	* 
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Special Art Data | Effect1")
	ESpecialArtEffectStatus Effect1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Special Art Data | Effect1")
	uint8 EDuration1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Special Art Data | Effect1")
	float EQuantity1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Special Art Data | Effect2")
	ESpecialArtEffectStatus Effect2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Special Art Data | Effect2")
	uint8 EDuration2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Special Art Data | Effect2")
	float EQuantity2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Special Art Data | Effect3")
	ESpecialArtEffectStatus Effect3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Special Art Data | Effect3")
	uint8 EDuration3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Special Art Data | Effect3")
	float EQuantity3;

	/*ActivationConditions
	* DamageAboveX : Damage above X value
	* DamageAboveMax: Damage above x % of max life
	* EnemyDamageAboveX and EnemyDamageAboveMax : Same as above, applied to enemy target
	* DefeatedAlly : There's X defeated allies
	* DefeatedSelf: The character that uses the special attack will be defeated in X turns
	* AllAlliesAlive, AllEnemiesAlive : Self explanatory 
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Special Art Data | Activation Condition")
	ESpecialArtActivationCondition ActivationCondition1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Special Art Data | Activation Condition")
	float ACQuantity1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Special Art Data | Activation Condition")
	ESpecialArtActivationCondition ActivationCondition2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Special Art Data | Activation Condition")
	float ACQuantity2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Special Art Data | Activation Condition")
	ESpecialArtActivationCondition ActivationCondition3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Special Art Data | Activation Condition")
	float ACQuantity3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Special Art Data")
	FText Description;



};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENAPROJECT_API USpecialSetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpecialSetComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialArt")
	class UDataTable* SpecialArtTable;

	//Skill that will be activated
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills")
	FSpecialArt ActiveSpecialArt;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skills")
	bool ValidSpecialArt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner")
	class ABaseCharacter* CharacterReference;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	bool ActivationConditionCheck(ESpecialArtActivationCondition ConditionToCheck, float ConditionQuantity);

	bool CanSpecialArtBeActivated(FSpecialArt* SpecialArtToActivate);

	//Searches the RowName, Checks for Activation Condition and Creates the FSkill struct
	void SetActiveSpecialArt(FName RowName);

	void ActivateAllEffectsFromSpecialArt();

	void ActivateIndividualEffectFromSpecialArt(ESpecialArtEffectStatus EffectToActivate, uint8 EffectToActivateDuration, float EffectToActivateQuantity);
		
};
