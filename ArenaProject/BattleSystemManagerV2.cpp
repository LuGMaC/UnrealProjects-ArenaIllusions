// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleSystemManagerV2.h"
#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MainPlayerControllerV2.h"
#include "BaseCharacter.h"
#include "CombatCharacterV2.h"
#include "BaseStanceComponent.h"
#include "SkillSetComponentV2.h"
#include "NonCombatBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Pawn.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ABattleSystemManagerV2::ABattleSystemManagerV2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BattleArenaVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("BattleArenaVolume"));
	RootComponent = BattleArenaVolume;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(GetRootComponent());

	AllySpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AllySpawnBox"));
	AllySpawnBox->SetupAttachment(BattleArenaVolume);

	EnemySpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("EnemySpawnBox"));
	EnemySpawnBox->SetupAttachment(BattleArenaVolume);

	//Initializing Character Camera and Spring Arm
	BattleSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("BattleSpringArmComponent"));
	BattleSpringArmComponent->SetupAttachment(GetRootComponent());
	BattleSpringArmComponent->TargetArmLength = 250; //Distance that camera will follow character
	BattleSpringArmComponent->bUsePawnControlRotation = true; //Rotate arm based on controller

	BattleCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("BattleCamera"));
	BattleCamera->SetupAttachment(BattleSpringArmComponent, USpringArmComponent::SocketName);
	BattleCamera->bUsePawnControlRotation = false;

	BattleCameraActive = false;

	FieldEffect = EFieldEffect::EFE_OutsideOfBattle;
	BattleVictoryCondition = EVictoryCondition::EVC_DefeatAll;

	DistanceBetweenCombatants = 70;

	this->bFindCameraComponentWhenViewTarget = true;


}

// Called when the game starts or when spawned
void ABattleSystemManagerV2::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerReference = Cast<AMainPlayerControllerV2>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PlayerControllerReference == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT(" Failed Cast"));
	}

	else {
		PlayerControllerReference->SetBattleSystemManagerReference(this);
	}

	BattleArenaVolume->OnComponentBeginOverlap.AddDynamic(this, &ABattleSystemManagerV2::OnOverlapBegin);
	BattleArenaVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BattleArenaVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BattleArenaVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	
}

// Called every frame
void ABattleSystemManagerV2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*
* ============================================================================================================================================
*								MOVEMENT AND CAMERA FUNCTIONS
* ============================================================================================================================================
*/

void ABattleSystemManagerV2::ActivateBattleCamera()
{
	if (PlayerControllerReference)
	{
		UE_LOG(LogTemp, Warning, TEXT(" Player Controler Reference found"));
		PlayerControllerReference->SetViewTargetWithBlend(this, 0, EViewTargetBlendFunction::VTBlend_Linear, 0, false);
		BattleCamera->SetActive(true);
		BattleCameraActive = true;
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT(" Player Controler Reference not found"));
	}

}



/*
* ============================================================================================================================================
*								BATTLE SETUP FUNCTIONS
* ============================================================================================================================================
*/

void ABattleSystemManagerV2::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ABaseCharacter* PlayerCharacter = Cast<ABaseCharacter>(OtherActor);

		if (PlayerCharacter)
		{
			//if the player controller character touched the battle area
			//Cancel collision on BattleArenaVolume
			BattleArenaVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			if ((AlliesNames.Num() > 0) && (EnemiesNames.Num() > 0) && (FieldEffect != EFieldEffect::EFE_OutsideOfBattle))
			{
				//If there's at least one ally name, one enemy name and the field effect is set to battle
				//then start battle
				UE_LOG(LogTemp, Warning, TEXT(" Battle Setup Start"));

				//PlayerCharacter->ToggleBaseCharacterCamera();
				this->ActivateBattleCamera();
				UE_LOG(LogTemp, Warning, TEXT(" ActivateBattleCamera"));


				//Send outside of battle player character to beginning
				PlayerCharacter->SetActorLocation(PlayerCharacter->StartingPosition);
				UE_LOG(LogTemp, Warning, TEXT(" PlayerCharacter Sent to Starting Position"));

				//FActorSpawnParameters SpawnParameters;
				UE_LOG(LogTemp, Warning, TEXT(" SpawnFighter Begin"));
				this->SpawnFighters();
				UE_LOG(LogTemp, Warning, TEXT(" SpawnFighter End"));


				//Create Combat UI
				UE_LOG(LogTemp, Warning, TEXT(" Create Combat UI"));
				PlayerControllerReference->SetupUIForCombat();

				//Start Music
				PlayBattleMusic();

				//Start first turn
				TurnBegin();

			}

		}
	}




	//SetupBattle(FieldEffect, BattleVictoryCondition);

}

void ABattleSystemManagerV2::SpawnFighters()
{

	UWorld* World = GetWorld();
	//Spawn Allies
	UE_LOG(LogTemp, Warning, TEXT(" SpawnFighter Allies Begin"));
	FActorSpawnParameters SpawnParams;
	int PositionAdjust = 0;

	for (int i = 0; i < AlliesNames.Num(); i++)
	{
		if (AlliesMap.Contains(AlliesNames[i]) && AlliesNames[i] != "None")
		{
			UE_LOG(LogTemp, Warning, TEXT(" Found Ally Name"));
			//If the map contains the correct name, spawn actor
			//ABaseCharacter* AllyToSpawn = World->SpawnActor<ABaseCharacter>(AlliesMap[AlliesNames[i]]);



			UE_LOG(LogTemp, Warning, TEXT(" Spawn Ally. Setup Location"));

			//setup actor location
			//For allies: Odd members must have +X and -Y coordinates, while Even members have -X and +Y
			FVector NewAllyLocation = AllySpawnBox->GetComponentLocation();
			PositionAdjust = (DistanceBetweenCombatants * FMath::DivideAndRoundUp(i, 2));

			//NewAllyLocation.Y += (DistanceBetweenCombatants * FMath::DivideAndRoundUp(i, 2));

			//if odd -Y and +X coordinate
			if (i % 2 != 0)
			{
				NewAllyLocation.Y -= PositionAdjust;
				NewAllyLocation.X += PositionAdjust;
			}

			//if even, +Y and -X coordinate
			else
			{
				NewAllyLocation.Y += PositionAdjust;
				NewAllyLocation.X -= PositionAdjust;
			}

			//setup actor location according to center of battle system, that is, billboard
			//the first ally will determine the rotator for all of them
			UE_LOG(LogTemp, Warning, TEXT(" Spawn Ally. Setup Rotation"));
			FRotator NewAllyRotator;

			if (i == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT(" First Ally. Setup Rotation Variable"));
				NewAllyRotator = UKismetMathLibrary::FindLookAtRotation(NewAllyLocation, Billboard->GetComponentLocation());
			}

			UE_LOG(LogTemp, Warning, TEXT("Setup Rotation End"));

			ACombatCharacterV2* AllyToSpawn = World->SpawnActor<ACombatCharacterV2>(AlliesMap[AlliesNames[i]], NewAllyLocation, NewAllyRotator, SpawnParams);

			//Add Ally reference to TArray
			Allies.Emplace(AllyToSpawn);

			/*if (AllyToSpawn)
			{
				AllyToSpawn->SetActorLocation(NewAllyLocation);
				AllyToSpawn->SetActorRotation(NewAllyRotator);
			}*/

			AllyToSpawn->bIsCharacterAnAlly = true;
			AllyToSpawn->BattleSystemReference = this;

			/* Update 02/12
			* Adjust Character to correct status based on level
			*/

			AllyToSpawn->SetupCharacterStatus(PlayerControllerReference->WorldContext);

			AllyToSpawn->LoadSkillVariableData();

		}

		if (AlliesNames[i] == "None")
		{
			UE_LOG(LogTemp, Warning, TEXT("AllyyName Empty"));
		}
	}

	UE_LOG(LogTemp, Warning, TEXT(" SpawnFighter Allies End. Spawn Enemies Begin"));

	//Spawn Enemies
	for (int i = 0; i < EnemiesNames.Num(); i++)
	{
		if (EnemyMap.Contains(EnemiesNames[i]) && EnemiesNames[i] != "None")
		{
			UE_LOG(LogTemp, Warning, TEXT(" Found Enemy Name"));
			//If the map contains the correct name, spawn actor


			UE_LOG(LogTemp, Warning, TEXT(" Spawn Enemy. Setup Location"));
			//setup actor location
			//For enemies: Odd members must have -X and -Y coordinates, while Even members have +X and +Y
			FVector NewEnemyLocation = EnemySpawnBox->GetComponentLocation();
			//NewEnemyLocation.Y += (DistanceBetweenCombatants * FMath::DivideAndRoundUp(i, 2));

			PositionAdjust = (DistanceBetweenCombatants * FMath::DivideAndRoundUp(i, 2));

			//If odd, -X and -Y coordinates
			if (i % 2 != 0)
			{
				NewEnemyLocation.Y -= PositionAdjust;
				NewEnemyLocation.X -= PositionAdjust;
			}

			//if even, +X and +Y coordinates
			else
			{
				NewEnemyLocation.Y += PositionAdjust;
				NewEnemyLocation.X += PositionAdjust;
			}

			UE_LOG(LogTemp, Warning, TEXT(" Setup Location Complete"));

			//setup actor location according to center of battle system, that is, billboard
			//the first ally will determine the rotator for all of them
			UE_LOG(LogTemp, Warning, TEXT(" Spawn Enemy. Setup Rotation"));
			FRotator NewEnemyRotator;

			if (i == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT(" First Enemy. Setup Rotation Variable"));
				NewEnemyRotator = UKismetMathLibrary::FindLookAtRotation(NewEnemyLocation, Billboard->GetComponentLocation());
			}

			UE_LOG(LogTemp, Warning, TEXT("Setup Rotation End"));

			ACombatCharacterV2* EnemyToSpawn = World->SpawnActor<ACombatCharacterV2>(EnemyMap[EnemiesNames[i]], NewEnemyLocation, NewEnemyRotator, SpawnParams);


			//Add spawned enemy to tarray of enemies
			Enemies.Emplace(EnemyToSpawn);

			/*if (EnemyToSpawn)
			{
				EnemyToSpawn->SetActorLocation(NewEnemyLocation);
				EnemyToSpawn->SetActorRotation(NewEnemyRotator);
			}*/

			EnemyToSpawn->bIsCharacterAnAlly = false;
			EnemyToSpawn->BattleSystemReference = this;

			/* Update 02/12
			* Adjust Character to correct status based on level
			*/

			EnemyToSpawn->SetupCharacterStatus(PlayerControllerReference->WorldContext);

		}

		if (EnemiesNames[i] == "None")
		{
			UE_LOG(LogTemp, Warning, TEXT("EnemyName Empty"));
		}
	}

}

void ABattleSystemManagerV2::SetupCharacterStatus(ACombatCharacterV2* CharacterToSetupStatus)
{
	if (PlayerControllerReference == nullptr)
	{
		PlayerControllerReference = Cast<AMainPlayerControllerV2>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	}

	FCharacterLevelAndStatusV2* CharacterCurrentStatus;
	FString WorldInt = FString::FromInt(PlayerControllerReference->WorldContext);
	FName CurrentWorldContext = (*WorldInt);

	//Iterate in the entire data table until it ends OR the cost of a skill is bigger than the command input
	static const FString ContextString(TEXT("ContextString"));
	CharacterCurrentStatus = CharacterToSetupStatus->LevelTable->FindRow<FCharacterLevelAndStatusV2>(CurrentWorldContext, ContextString);

	CharacterToSetupStatus->Level = CharacterCurrentStatus->Level;
	CharacterToSetupStatus->MaxHealth = CharacterCurrentStatus->MaxHealth;
	CharacterToSetupStatus->CurrentHealth = CharacterCurrentStatus->CurrentHealth;

	if (CharacterToSetupStatus->CurrentHealth > (CharacterToSetupStatus->MaxHealth))
	{
		CharacterToSetupStatus->CurrentHealth = CharacterToSetupStatus->MaxHealth;
	}

	CharacterToSetupStatus->MaxSP = CharacterCurrentStatus->MaxSP;
	CharacterToSetupStatus->CurrentSP = CharacterCurrentStatus->CurrentSP;
	CharacterToSetupStatus->SP_Regeneration_Skill = CharacterCurrentStatus->SP_Regeneration_Skill;
	CharacterToSetupStatus->SP_Regeneration_SingleAttack = CharacterCurrentStatus->SP_Regeneration_SingleAttack;

	if (CharacterToSetupStatus->CurrentSP > CharacterToSetupStatus->MaxSP)
	{
		CharacterToSetupStatus->CurrentSP = CharacterToSetupStatus->MaxSP;
	}

	CharacterToSetupStatus->Max_AP = CharacterCurrentStatus->Max_AP;
	CharacterToSetupStatus->CurrentAP = CharacterCurrentStatus->CurrentAP;
	CharacterToSetupStatus->AP_Regeneration = CharacterCurrentStatus->AP_Regeneration;

	if (CharacterToSetupStatus->CurrentAP > CharacterToSetupStatus->Max_AP)
	{
		CharacterToSetupStatus->CurrentAP = CharacterToSetupStatus->Max_AP;
	}

	CharacterToSetupStatus->BaseATK = CharacterCurrentStatus->BaseATK;
	CharacterToSetupStatus->BaseDEF = CharacterCurrentStatus->BaseDEF;
	CharacterToSetupStatus->BaseTEC = CharacterCurrentStatus->BaseTEC;
	CharacterToSetupStatus->BaseRES = CharacterCurrentStatus->BaseRES;
	CharacterToSetupStatus->BaseEVA = CharacterCurrentStatus->BaseEVA;
	CharacterToSetupStatus->BaseSPD = CharacterCurrentStatus->BaseSPD;
	CharacterToSetupStatus->BaseAGI = CharacterCurrentStatus->BaseAGI;
	CharacterToSetupStatus->SPD_Regeneration = CharacterCurrentStatus->SPD_Regeneration;


	CharacterToSetupStatus->RightArmDmgThreshold = CharacterCurrentStatus->RightArmDmgThreshold;
	CharacterToSetupStatus->LeftArmDmgThreshold = CharacterCurrentStatus->LeftArmDmgThreshold;
	CharacterToSetupStatus->ChestDmgThreshold = CharacterCurrentStatus->ChestDmgThreshold;
	CharacterToSetupStatus->LegsDmgThreshold = CharacterCurrentStatus->LegsDmgThreshold;
	CharacterToSetupStatus->HeadDmgThreshold = CharacterCurrentStatus->HeadDmgThreshold;

	CharacterToSetupStatus->RightArmBreakDmgThreshold = CharacterCurrentStatus->RightArmBreakDmgThreshold;
	CharacterToSetupStatus->LeftArmBreakDmgThreshold = CharacterCurrentStatus->LeftArmBreakDmgThreshold;
	CharacterToSetupStatus->ChestBreakDmgThreshold = CharacterCurrentStatus->ChestBreakDmgThreshold;
	CharacterToSetupStatus->LegsBreakDmgThreshold = CharacterCurrentStatus->LegsBreakDmgThreshold;
	CharacterToSetupStatus->HeadBreakDmgThreshold = CharacterCurrentStatus->HeadBreakDmgThreshold;

	CharacterToSetupStatus->NormalBodyPartDamageMultiplier = CharacterCurrentStatus->NormalBodyPartDamageMultiplier;
	CharacterToSetupStatus->DamagedBodyPartDamageMultiplier = CharacterCurrentStatus->DamagedBodyPartDamageMultiplier;
	CharacterToSetupStatus->BrokenBodyPartDamageMultiplier = CharacterCurrentStatus->BrokenBodyPartDamageMultiplier;

	/*A check for if the BrokenBodyPart is the same as the other values.
	bool IsBodyMultiplierValuesAnIssue = false;

	do
	{
		IsBodyMultiplierValuesAnIssue = false;

		CharacterToSetupStatus->NormalBodyPartDamageMultiplier = CharacterCurrentStatus->NormalBodyPartDamageMultiplier;
		CharacterToSetupStatus->DamagedBodyPartDamageMultiplier = CharacterCurrentStatus->DamagedBodyPartDamageMultiplier;
		CharacterToSetupStatus->BrokenBodyPartDamageMultiplier = CharacterCurrentStatus->BrokenBodyPartDamageMultiplier;

		if ((CharacterToSetupStatus->BrokenBodyPartDamageMultiplier == CharacterToSetupStatus->NormalBodyPartDamageMultiplier) || (CharacterToSetupStatus->BrokenBodyPartDamageMultiplier == CharacterToSetupStatus->DamagedBodyPartDamageMultiplier))
		{
			UE_LOG(LogTemp, Warning, TEXT("WARNING: Broken Body Part Multiplier is equal to Normal/Damaged. Changing to a new value"));
			IsBodyMultiplierValuesAnIssue = true;
			float InMin = 0.01;
			float InMax = 1.99;
			CharacterToSetupStatus->BrokenBodyPartDamageMultiplier += FMath::RandRange(InMin, InMax);
		}


	} while (IsBodyMultiplierValuesAnIssue);
	*/

	CharacterToSetupStatus->ResistanceAttackValue = CharacterCurrentStatus->ResistanceAttackValue;
	CharacterToSetupStatus->RightArmComboRes = CharacterCurrentStatus->RightArmComboRes;
	CharacterToSetupStatus->LeftArmComboRes = CharacterCurrentStatus->LeftArmComboRes;
	CharacterToSetupStatus->ChestComboRes = CharacterCurrentStatus->ChestComboRes;
	CharacterToSetupStatus->LegsComboRes = CharacterCurrentStatus->LegsComboRes;
	CharacterToSetupStatus->HeadComboRes = CharacterCurrentStatus->HeadComboRes;
	CharacterToSetupStatus->ComboResRenewValue = CharacterCurrentStatus->ComboResRenewValue;

	CharacterToSetupStatus->InitialStance = CharacterCurrentStatus->InitialStance;

	if (CharacterToSetupStatus->StanceMap.Contains(CharacterToSetupStatus->InitialStance.ToString()))
	{
		CharacterToSetupStatus->ChangeStance(CharacterToSetupStatus->InitialStance);
	}


	/*The rest of the values should be standard
	* Scaling should be 1 and only change due to buffs and debuffs
	* Status should be all normal, but they can be changed via irregular implementation in battle condition
	*/

	//Implement Parameters for AP System
}


void ABattleSystemManagerV2::SetupBattle(EFieldEffect ActiveFieldEffect, EVictoryCondition ActiveVictoryCondition)
{

	//Turn on BattleArena Collision to start fight
	BattleArenaVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	//Normal Parameters for Battle
	MaxHealth_Modifier = 1;
	CurrentHealth_Modifier = 1;
	MaxSP_Modifier = 1;
	CurrentSP_Modifier = 1;
	Max_AP_Modifier = 1;
	CurrentAP_Modifier = 1;
	ATK_Modifier = 1;
	DEF_Modifier = 1;
	TEC_Modifier = 1;
	EVA_Modifier = 1;
	SPD_Modifier = 1;
	AGI_Modifier = 1;
	EXP_Modifier = 1;
	AlliesHitCount = 0;
	EnemiesHitCount = 0;
	AlliesDamageCount = 0;
	EnemiesDamageCount = 0;
	AlliesExtraDmgModifier = 1;
	EnemiesExtraDmgModifier = 1;
	TurnsToSurvive = 0;
	DamageToCause = 0;
	PartToBreak = " ";
	BattleVictoryCondition = ActiveVictoryCondition;
	BattleDefeatCondition = EDefeatCondition::EDC_AllDefeated;
	FieldEffect = ActiveFieldEffect;
	TurnNumber = 1;
	OverallDamageCausedByAllies = 0;
	OverallDamageCausedByEnemies = 0;
	bBattleEnded = false;
	bPlayerVictory = false;

	if (BattleVictoryCondition == EVictoryCondition::EVC_BreakPart)
	{
		if (PartToBreak == "Head")
		{
			TargetPart = EBodyPartTarget::EBPT_Head;
		}

		if (PartToBreak == "Chest")
		{
			TargetPart = EBodyPartTarget::EBPT_Chest;
		}

		if (PartToBreak == "Legs")
		{
			TargetPart = EBodyPartTarget::EBPT_Legs;
		}

		if (PartToBreak == "RightArm")
		{
			TargetPart = EBodyPartTarget::EBPT_RightArm;
		}

		if (PartToBreak == "LeftArm")
		{
			TargetPart = EBodyPartTarget::EBPT_LeftArm;
		}

	}

	if (FieldEffect != EFieldEffect::EFE_NormalBattle)
	{

		switch (FieldEffect)
		{
		case EFieldEffect::EFE_Freeze:
			AGI_Modifier = 0.8;
			break;

		case EFieldEffect::EFE_Heat:
			SPD_Modifier = 0.8;
			break;

		case EFieldEffect::EFE_HeavyGravity:
			MaxHealth_Modifier = 0.8;
			break;

		case EFieldEffect::EFE_MiasmaField:
			CurrentHealth_Modifier = 0.8;
			break;

		case EFieldEffect::EFE_Irregular:
			SetupIrregularBattle();
			break;

		default:
			break;

		}

	}


}

void ABattleSystemManagerV2::SetupIrregularBattle_Implementation()
{
	//Setups battle data for irregular battle

	//Turn on BattleArena Collision to start fight
	BattleArenaVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	//Data should be set on BPs

}

void ABattleSystemManagerV2::SetupAlliesNames(TArray<FName> AlliesNamesForBattleSetup)
{
	if (AlliesNames.Num() > 0)
	{
		AlliesNames.Empty();
	}

	AlliesNames = AlliesNamesForBattleSetup;

}



void ABattleSystemManagerV2::SetupEnemiesNames(TArray<FName>EnemiesNamesForBattleSetup)
{
	if (EnemiesNames.Num() > 0)
	{
		EnemiesNames.Empty();
	}

	EnemiesNames = EnemiesNamesForBattleSetup;

}

void ABattleSystemManagerV2::SetupVictoryDetails(int DamageOrNumberOfTurns)
{
	if (BattleVictoryCondition == EVictoryCondition::EVC_Survive)
	{
		TurnsToSurvive = DamageOrNumberOfTurns;
	}

	if (BattleVictoryCondition == EVictoryCondition::EVC_CauseDamage)
	{
		DamageToCause = DamageOrNumberOfTurns;
	}

}

void ABattleSystemManagerV2::ClearAllCombatants()
{

	if (Allies.Num() > 0)
	{

		for (int i = 0; i < Allies.Num(); i++)
		{
			//Saves to Player Controller Reference the Skill Variable Information
			PlayerControllerReference->SaveSkillVariableInformation(Allies[i]->Name, Allies[i]->SkillVariableInformationMap);

			//Allies[i]->ConditionalBeginDestroy();
			Allies[i]->Destroy();
		}

		Allies.Empty();
		AlliesNames.Empty();
	}

	if (Enemies.Num() > 0)
	{
		for (int i = 0; i < Enemies.Num(); i++)
		{
			//Enemies[i]->ConditionalBeginDestroy();
			Enemies[i]->Destroy();
		}

		Enemies.Empty();
		EnemiesNames.Empty();
	}



}

void ABattleSystemManagerV2::CheckVictoryAndDefeatConditions()
{
	//Defines if bPlayerVictory should be true or false
	//and if bBattleEnd should be true or false

	//Checks first if all allies or enemies were defeated

	bool bAreAllAlliesDefeated = true;

	for (int i = 0; i < Allies.Num(); i++)
	{
		if (Allies[i]->CurrentHealth > 0)
		{
			bAreAllAlliesDefeated = false;
			break;
		}
	}

	if (bAreAllAlliesDefeated)
	{
		bBattleEnded = true;
		WidgetBattleResultAnnounce(false);
		//EndBattle(false);
		return;
	}

	bool bAreAllEnemiesDefeated = true;

	for (int i = 0; i < Enemies.Num(); i++)
	{
		if (Enemies[i]->CurrentHealth > 0)
		{
			bAreAllEnemiesDefeated = false;
			break;
		}
	}

	if (bAreAllEnemiesDefeated)
	{
		bBattleEnded = true;
		WidgetBattleResultAnnounce(true);
		//EndBattle(true);
		return;
	}


	//Check for defeat conditions first
	switch (BattleDefeatCondition)
	{
	case EDefeatCondition::EDC_AllDefeated:
		//Already checked
	default:
		break;
	}

	switch (BattleVictoryCondition)
	{
	case EVictoryCondition::EVC_DefeatAll:
		//Already checked
		break;

	case EVictoryCondition::EVC_CauseDamage:

		UE_LOG(LogTemp, Warning, TEXT(" Battle System Manager - Victory Check - CauseDamage"));
		UE_LOG(LogTemp, Warning, TEXT(" DamageCaused: %d"), OverallDamageCausedByAllies);
		UE_LOG(LogTemp, Warning, TEXT(" DamageToCause: %d"), DamageToCause);

		if (OverallDamageCausedByAllies > DamageToCause)
		{
			bBattleEnded = true;
			WidgetBattleResultAnnounce(true);
			//EndBattle(true);
		}

		break;

	case EVictoryCondition::EVC_Survive:

		UE_LOG(LogTemp, Warning, TEXT(" Battle System Manager - Victory Check - Survive Turns"));
		UE_LOG(LogTemp, Warning, TEXT(" CurrentTurn: %d"), TurnNumber);
		UE_LOG(LogTemp, Warning, TEXT(" Turns To Survive: %d"), TurnsToSurvive);

		if (TurnNumber > TurnsToSurvive)
		{
			bBattleEnded = true;
			WidgetBattleResultAnnounce(true);
			//EndBattle(true);
		}
		break;

	case EVictoryCondition::EVC_BreakPart:
		for (int i = 0; i < Enemies.Num(); i++)
		{
			if (Enemies[i]->GetBodyPartStatusDamageMultiplier(TargetPart) == Enemies[i]->BrokenBodyPartDamageMultiplier)
			{
				bBattleEnded = true;
				WidgetBattleResultAnnounce(true);
				//EndBattle(true);
				break;
			}
		}
		break;

	}
}

void ABattleSystemManagerV2::EndBattle(bool BattleResult)
{
	//Show result animation
	//WidgetBattleResultAnnounce(bool result) is in charge of that. Afterwards, the BP will call endbattle

	ClearAllCombatants();

	if (PlayerControllerReference)
	{
		if (ChallengeNumber < PlayerControllerReference->WorldContext)
		{
			//This means the fight was not the one that will advance the chapter, thus do nothing.
		}

		else
		{
			if (BattleResult)
			{
				//Gives the winning result to player controller reference
				PlayerControllerReference->UpdateWorldSettings(true, false);
			}
			else
			{
				//Gives the losing result to player controller reference
				PlayerControllerReference->UpdateWorldSettings(false, true);
			}

		}

		//rebuilds main menu UI
		PlayerControllerReference->SetupMainMenuUI();

		//Finishes Battle Music and setup lounge music
		EndBattleMusic();

	}

}

void ABattleSystemManagerV2::SetupChallengeNumber(int NumberOfChallenge)
{
	ChallengeNumber = NumberOfChallenge;
}

/*
* ============================================================================================================================================
*								DURING BATTLE FUNCTIONS
* ============================================================================================================================================
*/

void ABattleSystemManagerV2::UpdateWidgets_Implementation()
{

}

void ABattleSystemManagerV2::WidgetCounterCall_Implementation()
{

}

void ABattleSystemManagerV2::WidgetBattleResultAnnounce_Implementation(bool BattleResult)
{

}

void ABattleSystemManagerV2::BattleLogReport_Implementation(const FString& ReportStringToLog)
{

}

void ABattleSystemManagerV2::PlayBattleMusic_Implementation()
{

}

void ABattleSystemManagerV2::EndBattleMusic_Implementation()
{

}


void ABattleSystemManagerV2::EnemiesActionSetup()
{
	for (int i = 0; i != Enemies.Num(); i++)
	{
		Enemies[i]->SetupAIAction();
	}
}

void ABattleSystemManagerV2::AnimationEndDelayFunction()
{
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ABattleSystemManagerV2::AnimationEnd, 1, false, 0.2);
}

void ABattleSystemManagerV2::AnimationEnd()
{

	UE_LOG(LogTemp, Warning, TEXT("Battle Sys Manager - AnimationEnd"));

	UpdateWidgets();

	switch (TurnOrder[CurrentAction]->CharacterAction)
	{
	case ECharacterAction::ECA_Attack:
	case ECharacterAction::ECA_Skill:
		//The change stance is the last action and it will only occur if a skill was triggered
		TurnOrder[CurrentAction]->ChangeStanceAfterAnimNotify(false);
		//After that we change the current combo attack
		TurnOrder[CurrentAction]->CurrentComboAttack++;
		UE_LOG(LogTemp, Warning, TEXT("Battle Sys Manager - NextComboAttack: %d"), TurnOrder[CurrentAction]->CurrentComboAttack);
		TurnOrder[CurrentAction]->NextComboAttack();
		break;

	case ECharacterAction::ECA_Counter:
		ActionEnd();
		break;

	case ECharacterAction::ECA_Cover:
		break;

	case ECharacterAction::ECA_Item:
		ActionEnd();
		break;

	case ECharacterAction::ECA_Special:
	case ECharacterAction::ECA_Stance:
	case ECharacterAction::ECA_Defeated:
	case ECharacterAction::ECA_Null:
	default:
		ActionEnd();
		break;

	}


}


void ABattleSystemManagerV2::SetupTurnOrder()
{
	UE_LOG(LogTemp, Warning, TEXT(" Setup Turn Order"));

	TurnOrder = Allies;
	TurnOrder.Append(Enemies);

	TurnOrder.StableSort([](const ACombatCharacterV2& A, const ACombatCharacterV2& B) {
		return (A.WaitTime) < (B.WaitTime);

		});

	for (int i = 0; i < TurnOrder.Num(); i++)
	{
		FString CharName = TurnOrder[i]->Name;
		int CharWait = TurnOrder[i]->WaitTime;
		UE_LOG(LogTemp, Warning, TEXT(" Turn Order: %d - %s - WT: %d"), i, *CharName, CharWait);

	}

}


void ABattleSystemManagerV2::TurnBegin()
{


	UE_LOG(LogTemp, Warning, TEXT(" Battle Sys Manager - TurnBegin -  Turn: %d"), TurnNumber);

	for (int i = 0; i != Allies.Num(); i++)
	{
		Allies[i]->BeginTurn();
	}

	for (int i = 0; i != Enemies.Num(); i++)
	{
		Enemies[i]->BeginTurn();
	}

	//Call so that we can update enemies with it's actions
	EnemiesActionSetup();

	//Call so that Combat UI goes to MainMenu mode
	PlayerControllerReference->SetupSpecialUI(EUserWidgetStatus::EUWS_CombatMainMenu);

}

void ABattleSystemManagerV2::TurnEnd()
{
	UE_LOG(LogTemp, Warning, TEXT(" Battle Sys Manager - TurnEnd "));
	for (int i = 0; i != Allies.Num(); i++)
	{
		Allies[i]->EndTurn();
	}

	for (int i = 0; i != Enemies.Num(); i++)
	{
		Enemies[i]->EndTurn();
	}

	TurnNumber++;

	//Check for victory conditions
	CheckVictoryAndDefeatConditions();

	if (!bBattleEnded)
	{
		TurnBegin();
	}

}

void ABattleSystemManagerV2::ActionStart(ACombatCharacterV2* CharacterToAct)
{
	//Checks if the target for the next action needs to be adjusted
	TurnOrder[CurrentAction]->StartAction();

}

void ABattleSystemManagerV2::AssignNewTarget(ACombatCharacterV2* CharacterToAssignNewTarget)
{

	if (CharacterToAssignNewTarget->bIsCharacterAnAlly)
	{
		for (int i = 0; i != Enemies.Num(); i++)
		{
			if (CharacterToAssignNewTarget->SingleTargetReference != Enemies[i])
			{
				CharacterToAssignNewTarget->SingleTargetReference = Enemies[i];
				return;
			}

		}
	}

	else
	{
		for (int i = 0; i != Allies.Num(); i++)
		{
			if (CharacterToAssignNewTarget->SingleTargetReference != Allies[i])
			{
				CharacterToAssignNewTarget->SingleTargetReference = Allies[i];
				return;
			}

		}
	}

}

void ABattleSystemManagerV2::SortRandomTarget(ACombatCharacterV2* CharacterToAssignNewTarget)
{
	UE_LOG(LogTemp, Warning, TEXT(" Battle Sys Manager - Sort Random Target "));

	int NumberOfBattleCombatants = 0;
	int RandomChoice;
	TArray<int>FailedChoices;
	bool AcceptableRandomChoice = false;

	FailedChoices.Empty();

	if (CharacterToAssignNewTarget->bIsCharacterAnAlly)
	{
		UE_LOG(LogTemp, Warning, TEXT(" Battle Sys Manager - Sort Enemy Target for Ally"));
		NumberOfBattleCombatants = Enemies.Num();
		RandomChoice = FMath::RandRange(0, NumberOfBattleCombatants - 1);

		if (Enemies[RandomChoice]->CurrentHealth > 0)
		{
			CharacterToAssignNewTarget->SingleTargetReference = Enemies[RandomChoice];
			return;
		}

		else
		{
			//The first chosen character is defeated already
			FailedChoices.Emplace(RandomChoice);

			do
			{
				RandomChoice = FMath::RandRange(0, NumberOfBattleCombatants - 1);

				for (int i = 0; i < FailedChoices.Num(); i++)
				{
					if (RandomChoice == FailedChoices[i])
					{
						//We need to sort another number
						break;
					}

					if (i == (FailedChoices.Num() - 1))
					{
						//Checked the whole array and no copies came
						if (Enemies[RandomChoice]->CurrentHealth > 0)
						{
							AcceptableRandomChoice = true;
							CharacterToAssignNewTarget->SingleTargetReference = Enemies[RandomChoice];
							return;
						}

						else
						{
							FailedChoices.Emplace(RandomChoice);
						}
					}

				}


			} while (!AcceptableRandomChoice);
		}

	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT(" Battle Sys Manager - Sort Ally Target for Enemy"));
		NumberOfBattleCombatants = Allies.Num();
		RandomChoice = FMath::RandRange(0, NumberOfBattleCombatants - 1);

		UE_LOG(LogTemp, Warning, TEXT(" Random: %d"), RandomChoice);

		if (Allies[RandomChoice]->CurrentHealth > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT(" Ally %s chosen"), *Allies[RandomChoice]->Name);
			CharacterToAssignNewTarget->SingleTargetReference = Allies[RandomChoice];
			return;
		}

		else
		{
			//The first chosen character is defeated already
			FailedChoices.Emplace(RandomChoice);

			do
			{
				UE_LOG(LogTemp, Warning, TEXT(" Failed Choice"));
				RandomChoice = FMath::RandRange(0, NumberOfBattleCombatants - 1);
				UE_LOG(LogTemp, Warning, TEXT(" Random: %d"), RandomChoice);

				for (int i = 0; i < FailedChoices.Num(); i++)
				{
					if (RandomChoice == FailedChoices[i])
					{
						//We need to sort another number
						break;
					}

					if (i == (FailedChoices.Num() - 1))
					{
						//Checked the whole array and no copies came
						if (Allies[RandomChoice]->CurrentHealth > 0)
						{
							AcceptableRandomChoice = true;
							UE_LOG(LogTemp, Warning, TEXT(" Ally %s chosen"), *Allies[RandomChoice]->Name);
							CharacterToAssignNewTarget->SingleTargetReference = Allies[RandomChoice];
							return;
						}

						else
						{
							FailedChoices.Emplace(RandomChoice);
						}
					}

				}


			} while (!AcceptableRandomChoice);

		}
	}

}


void ABattleSystemManagerV2::ActionEnd()
{
	/*Function called after a character action ends
	*/
	UE_LOG(LogTemp, Warning, TEXT(" Battle Sys Manager - ActionEnd "));

	for (int i = 0; i != Allies.Num(); i++)
	{
		Allies[i]->EndAction();
	}

	for (int i = 0; i != Enemies.Num(); i++)
	{
		Enemies[i]->EndAction();
	}

	//Check for Victory condition
	CheckVictoryAndDefeatConditions();

	if (!bBattleEnded)
	{
		NextAction();
	}

}

void ABattleSystemManagerV2::NextAction()
{
	//Updates the Current Action
	CurrentAction++;

	UE_LOG(LogTemp, Warning, TEXT(" Battle Sys Manager - NextAction - CurrentAction: %d "), CurrentAction);

	if (CurrentAction >= TurnOrder.Num())
	{
		//All characters have attacked 
		TurnEnd();
	}

	else
	{
		ActionStart(TurnOrder[CurrentAction]);
	}

}

void ABattleSystemManagerV2::CombatStart()
{
	//Starts the combat with the first on the turn order
	CurrentAction = 0;

	UE_LOG(LogTemp, Warning, TEXT(" Battle System Manager - Combat Start - CurrentAction = %d"), CurrentAction);

	if (TurnOrder[CurrentAction] == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT(" Battle System Manager - Combat Start - TurnOrder Empty"));
	}

	else
	{
		ActionStart(TurnOrder[CurrentAction]);
	}


}


bool ABattleSystemManagerV2::AreAllAlliesAlive()
{
	for (int i = 0; i != Allies.Num(); i++)
	{
		//if one ally has current health less or equal to 0, return false. Meaning there's at least one ally dead. 
		if (Allies[i]->CurrentHealth <= 0)
			return false;
	}

	//if the loop ends, there are no dead allies. 
	return true;

}


bool ABattleSystemManagerV2::AreAllEnemiesAlive()
{
	for (int i = 0; i != Enemies.Num(); i++)
	{
		//if one enemy has current health less or equal to 0, return false. Meaning there's at least one enemy dead. 
		if (Enemies[i]->CurrentHealth <= 0)
			return false;
	}

	//if the loop ends, there are no dead enemies. 
	return true;

}

void ABattleSystemManagerV2::UpdateHitAndDamageCount(bool IsAnAlly, int DamageCaused, int NumberOfHits)
{

	UE_LOG(LogTemp, Warning, TEXT("===== Battle Sys Manager - Update Hit and Damage ========"));
	UE_LOG(LogTemp, Warning, TEXT("Damage: %d"), DamageCaused);
	UE_LOG(LogTemp, Warning, TEXT("Hits: %d"), NumberOfHits);
	/* The update happens once the damage will be shown on screen.
	* Thus the one calling this method is the target of the attack.
	* If the IsAnAlly bool is true, it means it's an ally as target. If false, it's an enemy as target.
	* Thus, the ally as target should update hit count and total damage of the enemy side and vice-versa
	*/
	if (IsAnAlly)
	{
		//target is an ally
		UE_LOG(LogTemp, Warning, TEXT("Ally Target"));
		EnemiesHitCount += NumberOfHits;
		EnemiesDamageCount += DamageCaused;
		OverallDamageCausedByEnemies += DamageCaused;
		EnemiesExtraDmgModifier = 1 + (EnemiesHitCount / 100);
	}

	else
	{
		//Target is an enemy
		UE_LOG(LogTemp, Warning, TEXT("Enemy Target"));
		AlliesHitCount += NumberOfHits;
		AlliesDamageCount += DamageCaused;
		OverallDamageCausedByAllies += DamageCaused;
		AlliesExtraDmgModifier = 1 + (AlliesHitCount / 100);

	}

}

void ABattleSystemManagerV2::BreakHitAndDamageCount(bool IsAnAlly)
{
	UE_LOG(LogTemp, Warning, TEXT("===== Battle Sys Manager - Break Hit and Damage Count ========"));
	/* This will be called as an Effect of a skill
	* Thus the one calling this method is the one executing the skill
	* If the IsAnAlly bool is true, it means it's an ally as the attacker. If false, it's an enemy as attacker
	* Thus, the ally as attacker should break the hit count and total damage of the enemy side and vice-versa
	*/

	if (IsAnAlly)
	{
		//Attacker is an ally
		EnemiesHitCount = 0;
		//EnemiesDamageCount = 0;
		EnemiesExtraDmgModifier = 1;
	}

	else
	{
		//Attacker is an enemy
		AlliesHitCount = 0;
		//AlliesDamageCount = 0;
		AlliesExtraDmgModifier = 1;

	}

}

void ABattleSystemManagerV2::CheckAllOpponentsForCountersAndBlocks(ACombatCharacterV2* CharacterThatActivatedSkill)
{
	if (CharacterThatActivatedSkill->bIsCharacterAnAlly)
	{
		//Character is an Ally, thus we need to check who the skill will target

		if (CharacterThatActivatedSkill->ActiveSkillList->ActiveSkill.Target == ETargetDefinition::ETD_EnemySingle)
		{
			//The target is a single enemy
			//Thus we need to check for a counter in target enemy and cover on everyone else

			for (int i = 0; i < Enemies.Num(); i++)
			{
				if (Enemies[i] == CharacterThatActivatedSkill->SingleTargetReference)
				{
					//Determine if there is an limitation for him to counter (e.g. already defeated)
					if (Enemies[i]->CanCharacterExecuteAction(ECharacterAction::ECA_Counter))
					{
						//If it's the target enemy, check for counter condition on effectA and then effectB
						if (Enemies[i]->ActiveStance->EffectATrigger == EEffectType::EET_Counter)
						{
							Enemies[i]->ActiveStance->EffectAActivation(CharacterThatActivatedSkill);
						}

						if (Enemies[i]->ActiveStance->EffectBTrigger == EEffectType::EET_Counter)
						{
							Enemies[i]->ActiveStance->EffectBActivation(CharacterThatActivatedSkill);
						}
					}
				}

				else
				{
					//Determine if there is an limitation for him to block (e.g. already defeated)
					if (Enemies[i]->CanCharacterExecuteAction(ECharacterAction::ECA_Cover))
					{
						//If it's not the target enemy, check for cover condition on effectA and then effectB
						if (Enemies[i]->ActiveStance->EffectATrigger == EEffectType::EET_Cover)
						{
							Enemies[i]->ActiveStance->EffectAActivation(CharacterThatActivatedSkill);
						}

						if (Enemies[i]->ActiveStance->EffectBTrigger == EEffectType::EET_Cover)
						{
							Enemies[i]->ActiveStance->EffectBActivation(CharacterThatActivatedSkill);
						}
					}

				}

			}


		}

		if (CharacterThatActivatedSkill->ActiveSkillList->ActiveSkill.Target == ETargetDefinition::ETD_EnemyAll)
		{
			//If target is all enemies
			//Check for counter condition on all enemies

			for (int i = 0; i < Enemies.Num(); i++)
			{
				//Determine if there is an limitation for him to counter (e.g. already defeated)
				if (Enemies[i]->CanCharacterExecuteAction(ECharacterAction::ECA_Counter))
				{
					if (Enemies[i]->ActiveStance->EffectATrigger == EEffectType::EET_Counter)
					{
						//This will lead to what is implemented in each style blueprint. There might be further checks in it
						Enemies[i]->ActiveStance->EffectAActivation(CharacterThatActivatedSkill);
					}

					if (Enemies[i]->ActiveStance->EffectBTrigger == EEffectType::EET_Counter)
					{
						//This will lead to what is implemented in each style blueprint. There might be further checks in it
						Enemies[i]->ActiveStance->EffectBActivation(CharacterThatActivatedSkill);
					}
				}

			}

		}

		//if, somehow, the target is an ally, it will do nothing
	}

	else
	{
		//Character is an enemy, thus we need to check all allies. The functions are the same as above, just checking the allies TArray
		if (CharacterThatActivatedSkill->ActiveSkillList->ActiveSkill.Target == ETargetDefinition::ETD_EnemySingle)
		{
			//The target is a single enemy
			//Thus we need to check for a counter in target enemy and cover on everyone else

			for (int i = 0; i < Allies.Num(); i++)
			{
				if (Allies[i] == CharacterThatActivatedSkill->SingleTargetReference)
				{
					//Determine if there is an limitation for him to counter (e.g. already defeated)
					if (Allies[i]->CanCharacterExecuteAction(ECharacterAction::ECA_Counter))
					{
						//If it's the target enemy, check for counter condition on effectA and then effectB
						if (Allies[i]->ActiveStance->EffectATrigger == EEffectType::EET_Counter)
						{
							Allies[i]->ActiveStance->EffectAActivation(CharacterThatActivatedSkill);
						}

						if (Allies[i]->ActiveStance->EffectBTrigger == EEffectType::EET_Counter)
						{
							Allies[i]->ActiveStance->EffectBActivation(CharacterThatActivatedSkill);
						}
					}

				}

				else
				{
					//Determine if there is an limitation for him to cover (e.g. already defeated)
					if (Allies[i]->CanCharacterExecuteAction(ECharacterAction::ECA_Cover))
					{
						//If it's not the target enemy, check for cover condition on effectA and then effectB
						if (Allies[i]->ActiveStance->EffectATrigger == EEffectType::EET_Cover)
						{
							Allies[i]->ActiveStance->EffectAActivation(CharacterThatActivatedSkill);
						}

						if (Allies[i]->ActiveStance->EffectBTrigger == EEffectType::EET_Cover)
						{
							Allies[i]->ActiveStance->EffectBActivation(CharacterThatActivatedSkill);
						}
					}


				}

			}


		}

		if (CharacterThatActivatedSkill->ActiveSkillList->ActiveSkill.Target == ETargetDefinition::ETD_EnemyAll)
		{
			//If target is all enemies
			//Check for counter condition on all allies since in this case, bIsAnAlly should be false

			for (int i = 0; i < Allies.Num(); i++)
			{
				//Determine if there is an limitation for him to counter (e.g. already defeated)
				if (Allies[i]->CanCharacterExecuteAction(ECharacterAction::ECA_Counter))
				{
					if (Allies[i]->ActiveStance->EffectATrigger == EEffectType::EET_Counter)
					{
						//This will lead to what is implemented in each style blueprint. There might be further checks in it
						Allies[i]->ActiveStance->EffectAActivation(CharacterThatActivatedSkill);
					}

					if (Allies[i]->ActiveStance->EffectBTrigger == EEffectType::EET_Counter)
					{
						//This will lead to what is implemented in each style blueprint. There might be further checks in it
						Allies[i]->ActiveStance->EffectBActivation(CharacterThatActivatedSkill);
					}
				}

			}


		}
	}

}

void ABattleSystemManagerV2::DamageAllOpponents(bool IsAnAlly, int CalculatedDamageFromAttackingCharacter, ABaseCharacter* AttackingCharacter)
{
	if (IsAnAlly)
	{
		//Attacking Character is an Ally, thus we need to Damage all enemies
		for (int i = 0; i < Enemies.Num(); i++)
		{
			Enemies[i]->ReceiveSkillDamage(CalculatedDamageFromAttackingCharacter, AttackingCharacter->ActiveSkillList->ActiveSkill.BodyPartTarget, AttackingCharacter);

		}
	}

	else
	{
		//Attacking Character is an Enemy, thus we need to Damage all allies
		for (int i = 0; i < Allies.Num(); i++)
		{
			Allies[i]->ReceiveSkillDamage(CalculatedDamageFromAttackingCharacter, AttackingCharacter->ActiveSkillList->ActiveSkill.BodyPartTarget, AttackingCharacter);

		}

	}

}


void ABattleSystemManagerV2::HealAllAllies(bool IsAnAlly, int CalculatedHealFromCharacter)
{
	if (IsAnAlly)
	{
		//Attacking Character is an Ally, thus we need to heal all allies
		for (int i = 0; i < Allies.Num(); i++)
		{
			Allies[i]->ReceiveHPHeal(CalculatedHealFromCharacter);

		}
	}

	else
	{
		//Attacking Character is an Enemy, thus we need to heal all enemies
		for (int i = 0; i < Enemies.Num(); i++)
		{
			Enemies[i]->ReceiveHPHeal(CalculatedHealFromCharacter);

		}

	}

}

