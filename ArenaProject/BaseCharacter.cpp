// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimationManager.h"
#include "Components/SphereComponent.h"
#include "BaseStanceComponent.h"
#include "SkillSetComponent.h"
#include "SpecialSetComponent.h"
#include "MainPlayerController.h"
#include "NonCombatBaseCharacter.h"
#include "BattleSystemManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//Initializing base parameters
	MaxHealth = 100000;
	CurrentHealth = 100000;
	MaxMagic = 100;
	CurrentMagic = 100;
	MagicRegeneration = 10;
	SingleAttackMagicRegeneration = 1;
	Max_AP = 10;
	CurrentAP = 10;
	AP_Regeneration = 1;
	BaseATK = 100;
	BaseDEF = 100;
	BaseTEC = 100;
	BaseEVA = 5;
	BaseSPD = 0;
	BaseAGI = 100;
	WaitTime = 0;
	SPD_Regeneration = 100;
	CharacterMainStatus = ECharacterStatus::ECS_Normal;
	CharacterMainLimitation = ECharacterLimitation::ECL_None;

	ScallingATK = 1;
	ScallingDEF = 1;
	ScallingTEC = 1;
	ScallingEVA = 1;
	ScallingSPD = 1;
	ScallingAGI = 1;
	ChargeValue = 0;

	CharacterAction = ECharacterAction::ECA_Null;

	//Initializing base parameters for character parts

	RightArmStatus = EBodyPartStatus::EBPS_Normal;
	LeftArmStatus = EBodyPartStatus::EBPS_Normal;
	ChestStatus = EBodyPartStatus::EBPS_Normal;
	LegsStatus = EBodyPartStatus::EBPS_Normal;
	HeadStatus = EBodyPartStatus::EBPS_Normal;

	NormalBodyPartDamageMultiplier = 1;
	DamagedBodyPartDamageMultiplier = 1.5;
	BrokenBodyPartDamageMultiplier = 3;

	RightArmDmgThreshold = 3000;
	LeftArmDmgThreshold = 3000;
	ChestDmgThreshold = 3000;
	LegsDmgThreshold = 3000;
	HeadDmgThreshold = 3000;

	RightArmBreakDmgThreshold = 8000;
	LeftArmBreakDmgThreshold = 8000;
	ChestBreakDmgThreshold = 8000;
	LegsBreakDmgThreshold = 8000;
	HeadBreakDmgThreshold = 8000;

	RightArmComboRes = 12;
	LeftArmComboRes = 12;
	ChestComboRes = 12;
	LegsComboRes = 12;
	HeadComboRes = 12;
	ComboResRenewValue = 15;
	ResistanceAttackValue = 1;

	OverHealthMultiplier = 2;

	//Initializing a base stance object
	//ActiveStance = NewObject<UBaseStanceComponent>(this, TEXT("DefaultActiveStance"));
	//ActiveStance->RegisterComponent();
	//this->AddInstanceComponent(ActiveStance);

	ActiveStance = CreateDefaultSubobject<UBaseStanceComponent>(TEXT("DefaultActiveStance"));
	//ActiveStance->RegisterComponent();
	//AddInstanceComponent(ActiveStance);

	//ActiveStance->RegisterComponent();
	//this->AddInstanceComponent(ActiveStance);

	//Initializing a base Skill Set

	ActiveSkillList = CreateDefaultSubobject<USkillSetComponent>(TEXT("DefaultActiveSkillList"));
	//ActiveSkillList->RegisterComponent();
	//AddInstanceComponent(ActiveSkillList);

	//Initializing a base special art set

	ActiveSpecialArtList = CreateDefaultSubobject<USpecialSetComponent>(TEXT("Default Special Art List"));
	//ActiveSpecialArtList->RegisterComponent();
	//AddInstanceComponent(ActiveSpecialArtList);


	//Initializing Character Camera and Spring Arm
	BaseCharacterSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CharacterSpringArm"));
	BaseCharacterSpringArmComponent->SetupAttachment(GetRootComponent());
	BaseCharacterSpringArmComponent->TargetArmLength = 600; //Distance that camera will follow character
	BaseCharacterSpringArmComponent->bUsePawnControlRotation = true; //Rotate arm based on controller

	BaseCharacterCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CharacterCamera"));
	BaseCharacterCamera->SetupAttachment(BaseCharacterSpringArmComponent,USpringArmComponent::SocketName);
	BaseCharacterCamera->bUsePawnControlRotation = false;
	
	ActivateCharacterCamera = false;
	this->bFindCameraComponentWhenViewTarget = true;

	//Initializing Talk Sphere
	//Change collision to only accept Pawn type
	TalkRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TalkRangeSphere"));
	TalkRangeSphere->SetupAttachment(GetRootComponent());
	TalkRangeSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	TalkRangeSphere->InitSphereRadius(360.f);

	//Setting up base values for movement
	WalkSpeed = 650;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	//Setting up base values for camera turning functions when using keyboard keys (To implement)
	BaseTurnRate = 65.f;
	BaseLookUpRate = 65.f;

	//Character does not rotate along with the controller/camera
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	//Character automatically turns in the direction of character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//Rotation Rate will control how fast the character can turn
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f);

	//AutoPossessPlayer = EAutoReceiveInput::Player0;

	StanceNumber = 0;
	AllowTalk = false;

	InitialStance = "Boxing";

	//Sum of all these must be 1
	LightAttackPercenteage = 0.3;
	MediumAttackPercenteage = 0.4;
	HeavyAttackPercenteage = 0.2;
	StancePercenteage = 0.1;

	LightStrategySPDThreshold = 40;

	LightStrategyAttackNumberThreshold = 3;

	MediumStrategyAttackNumberThreshold = 5;

	VeryHeavyStrategySPDCost = 140;

	MediumStrategySPDCost = 50;

	HeavyStrategySPDCost = 60;

	AIStrategy = EAIStrategy::EAIS_None;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Setting up initial stance. Boxing is default
	if (StanceMap.Contains(InitialStance.ToString()))
	{
		ChangeStance(InitialStance);
	}

	//ActiveStance = CreateDefaultSubobject<UBaseStanceComponent>(TEXT("DefaultActiveStance"));
	ActiveStance->RegisterComponent();
	AddInstanceComponent(ActiveStance);

	//ActiveStance->RegisterComponent();
	//this->AddInstanceComponent(ActiveStance);

	//Initializing a base Skill Set

	//ActiveSkillList = CreateDefaultSubobject<USkillSetComponent>(TEXT("DefaultActiveSkillList"));
	ActiveSkillList->RegisterComponent();
	AddInstanceComponent(ActiveSkillList);

	//Initializing a base special art set

	//ActiveSpecialArtList = CreateDefaultSubobject<USpecialSetComponent>(TEXT("Default Special Art List"));
	ActiveSpecialArtList->RegisterComponent();
	AddInstanceComponent(ActiveSpecialArtList);
	
	MainPlayerController = Cast<AMainPlayerController>(GetController());
	TalkRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::TalkRangeSphereOnOverlapBegin);
	TalkRangeSphere->OnComponentEndOverlap.AddDynamic(this, &ABaseCharacter::TalkRangeSphereOnOverlapEnd);
	AllowTalk = false;
	StartingPosition = this->GetActorLocation();
	//check if BattleSystemReference works. Should be null in case of outside of battle. In case of battle, should work as the character spawns. 
	/*if (MainPlayerController) 
	{
		BattleSystemReference = MainPlayerController->BattleSystemManagerReference;
	}*/
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Binds keyboard inputs to character movement
	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);

	//Binds mouse control to camera
	PlayerInputComponent->BindAxis("CameraLookUp", this, &ABaseCharacter::LookUp);
	PlayerInputComponent->BindAxis("CameraTurnRight", this, &ABaseCharacter::Turn);

	//Bind keyboard input to interact function
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ABaseCharacter::Interact);
	PlayerInputComponent->BindAction("QuitMainMenu", IE_Pressed, this, &ABaseCharacter::MainMenuEscape);

}

/*
* ============================================================================================================================================
*								MOVEMENT AND CAMERA FUNCTIONS
* ============================================================================================================================================
*/

void ABaseCharacter::MoveForward(float value)
{
	//Find out which way is forward 
	const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, value);

}
void ABaseCharacter::MoveRight(float value)
{
	//Find out which way is forward 
	const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, value);

}

void ABaseCharacter::Turn(float value)
{
	AddControllerYawInput(value);
}
void ABaseCharacter::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void ABaseCharacter::ToggleBaseCharacterCamera()
{
	if (ActivateCharacterCamera)
	{
		BaseCharacterCamera->SetActive(false);
		ActivateCharacterCamera = false;
	}

	else
	{
		BaseCharacterCamera->SetActive(true);
		ActivateCharacterCamera = true;

		if (MainPlayerController)
		{
			MainPlayerController->SetViewTargetWithBlend(this, 0, EViewTargetBlendFunction::VTBlend_Linear, 0, false);
		}

		else
		{
			MainPlayerController = Cast<AMainPlayerController>(GetController());
			MainPlayerController->SetViewTargetWithBlend(this, 0, EViewTargetBlendFunction::VTBlend_Linear, 0, false);
		}

		
	}

}


/*
* ============================================================================================================================================
*								OUTSIDE OF COMBAT RELATED FUNCTIONS
* ============================================================================================================================================
*/

void ABaseCharacter::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT(" InteractButton"));
	if (MainPlayerController && AllowTalk)
	{
		MainPlayerController->ShowMainMenu();
		UE_LOG(LogTemp, Warning, TEXT(" ShowMainMenu"));
		//Setup UI Mode Only
	}

}


void ABaseCharacter::MainMenuEscape()
{

	UE_LOG(LogTemp, Warning, TEXT(" MainMenuEscapeButton"));
	if (MainPlayerController && AllowTalk)
	{
		MainPlayerController->HideMainMenu();
		UE_LOG(LogTemp, Warning, TEXT(" HideMainMenu"));
		//Setup Quit UI Mode
	}

}

void ABaseCharacter::TalkRangeSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Allow Talk
	//Once button pressed, pass MainMenu to MainPlayerController and add to Viewport
	//Talk, Combat Setup functions and etc. come after that.

	UE_LOG(LogTemp, Warning, TEXT("TalkRangeTriggered"));

	if (OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("OtherActorTriggered"));
		NPC = Cast<ANonCombatBaseCharacter>(OtherActor);
		if (NPC)
		{
			UE_LOG(LogTemp, Warning, TEXT("CastTriggered"));
			AllowTalk = true;

			if (MainPlayerController)
			{
				UE_LOG(LogTemp, Warning, TEXT("MainPlayerTriggered"));
				MainPlayerController->ShowInteract();
			}


		}


	}

}

void ABaseCharacter::TalkRangeSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	//Unnalow Talk
	AllowTalk = false;
	if (MainPlayerController)
	{
		MainPlayerController->HideInteract();
	}

	NPC = nullptr;


}

FDialogue* ABaseCharacter::GetDialogueFromNPC(int WorldContext, int LocalContext)
{
	if (NPC)
	{
		return NPC->GetDialogue(WorldContext, LocalContext);
	}

	else
	{
		return nullptr; 
	}
}


/*
* ============================================================================================================================================
*								STANCE FUNCTIONS
* ============================================================================================================================================
*/

void ABaseCharacter::NextStance()
{
	StanceNumber++;
	if (StanceNumber > 2) StanceNumber = 0;

	if (StanceNumber == 0)
	{
		ChangeStance(TEXT("Boxing"));

	}

	if (StanceNumber == 1)
	{
		ChangeStance(TEXT("Capoeira"));

	}

	if (StanceNumber == 2)
	{
		ChangeStance(TEXT("BarFight"));

	}
}
void ABaseCharacter::PreviousStance()
{
	StanceNumber--;
	if (StanceNumber < 0) StanceNumber = 2;

	if (StanceNumber == 0)
	{
		ChangeStance(TEXT("Boxing"));

	}

	if (StanceNumber == 1)
	{
		ChangeStance(TEXT("Capoeira"));

	}

	if (StanceNumber == 2)
	{
		ChangeStance(TEXT("BarFight"));

	}
}
void ABaseCharacter::ChangeStance(FName StanceName)
{
	
	if (!CheckStanceChange(StanceName) && CharacterAction != ECharacterAction::ECA_Stance)
	{
		//if check stance change returns false, there's no need to change stances
		//Change stance if it was selected in stance menu because of the special effects
		return; 
	}

	if (StanceMap.Contains(StanceName.ToString()))
	{
		//Report Stance already happens after changing stances
		/*if (BattleSystemReference != nullptr)
		{
			
			UE_LOG(LogTemp, Warning, TEXT("Change Stance function"));
			FString NextStance = StanceName.ToString();
			BattleSystemReference->BattleLogReport(Name + " changes stance to " + NextStance);
		}*/
		
		//Unregister the component and mark current component to be destroyed
		ActiveStance->DestroyComponent();

		//UE_LOG(LogTemp, Warning, TEXT(" Stance Found"));

		ActiveStance = NewObject<UBaseStanceComponent>(this, StanceMap[StanceName.ToString()]);
		ActiveStance->RegisterComponent();
		this->AddInstanceComponent(ActiveStance);

		//UE_LOG(LogTemp, Warning, TEXT(" New ActiveStance registered"));

		if (ActiveStance)
		{
			//Activate stance effets if they are imediate type
			if(BattleSystemReference)
			{
				if (ActiveStance->EffectATrigger == EEffectType::EET_Imediate)
				{
					ActiveStance->EffectAActivation(this);
				}

				if (ActiveStance->EffectBTrigger == EEffectType::EET_Imediate)
				{
					ActiveStance->EffectBActivation(this);
				}

				ReportStance();
			}

			UE_LOG(LogTemp, Warning, TEXT(" New Stance Set"));
			GetMesh()->SetAnimInstanceClass(ActiveStance->StanceAnimationBP);

			CharacterAnimationReference = Cast<UCharacterAnimationManager>(GetMesh()->GetAnimInstance());

			
			if (CharacterAnimationReference)
			{
				UE_LOG(LogTemp, Warning, TEXT(" Animation class CAST success"));
			}

			else
			{
				UE_LOG(LogTemp, Warning, TEXT(" Animation class CAST fail"));
			}

		}
		
		else
		{
			UE_LOG(LogTemp, Warning, TEXT(" ActiveStance is null. NewObject Failed"));
		}

		
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT(" Stance 404"));
	}

}

bool ABaseCharacter::CheckStanceChange(FName NextStance)
{
	//Check if there's a need to ChangeStances.

	if (ActiveStance->StanceName == NextStance)
	{
		//If both are equal, there's no need to change stances
		UE_LOG(LogTemp, Warning, TEXT(" Same Stance. No need to change"));
		return false;
	}

	else
	{
		//Need to change stances
		return true;
	}

}

void ABaseCharacter::ChangeStanceAfterAnimNotify(bool Critical)
{
	if (Commands[CurrentComboAttack] == EAttackType::EAT_Special)
	{
		//FTimerHandle UnusedHandle;
		//GetWorldTimerManager().SetTimer(UnusedHandle,this, &ABaseCharacter::ChangeStanceAfterTimer, 1, false, 0.2);
		ChangeStance(ActiveSkillList->ActiveSkill.StanceChange);
	}
}

void ABaseCharacter::ChangeStanceAfterTimer()
{
	UE_LOG(LogTemp, Warning, TEXT(" Timer Ended - Change Stance"));
	ChangeStance(ActiveSkillList->ActiveSkill.StanceChange);
}

/*
* ============================================================================================================================================
*								SKILL FUNCTIONS
* ============================================================================================================================================
*/

void ABaseCharacter::LoadSkillVariableData()
{
	if (BattleSystemReference->PlayerControllerReference)
	{
		//FString - character name
		//TMap<FString, FCharacterSkillsVariableData> CharacterSkillVariableInformationMap;
		//FString - skill name
		//TMap<FString, FSkillVariableData> SkillVariableInformationMap;

		if (BattleSystemReference->PlayerControllerReference->CharacterSkillVariableInformationMap.Contains(Name))
		{

			SkillVariableInformationMap = BattleSystemReference->PlayerControllerReference->CharacterSkillVariableInformationMap[Name].CharacterSkillVariableData;

			UE_LOG(LogTemp, Warning, TEXT(" Skill Variable Data loaded."));
		}


	}

}

void ABaseCharacter::ExecuteSkill(FName SkillName)
{
	ActiveSkillList->SetActiveSkill(SkillName);

	if (SkillMontage && (ActiveSkillList->ActiveSkill.SkillName != "None"))
	{
		UE_LOG(LogTemp, Warning, TEXT(" SkillMontage found. Playing Skill"));
		CharacterAnimationReference->SetupAndPlayAnimationMontage(EBattleAnimationSetup::EBAS_Skill, ActiveSkillList->ActiveSkill.SkillName);
	}

	else {
		UE_LOG(LogTemp, Warning, TEXT(" SkillMontage is null."));
	}

}

void ABaseCharacter::ReportSkill()
{
	EBodyPartTarget BodyPartToPrint = ActiveSkillList->ActiveSkill.BodyPartTarget;
	//FString SkillTargetBodyPartString = StaticEnum<EBodyPartTarget>()->GetNameByIndex(static_cast<int32>(BodyPartToPrint)).ToString();
	FString SkillTargetBodyPartString = StaticEnum<EBodyPartTarget>()->GetDisplayValueAsText(BodyPartToPrint).ToString();
	SkillTargetBodyPartString.RemoveAt(0, 18);

	ETargetDefinition TargetDefToPrint = ActiveSkillList->ActiveSkill.Target;
	//FString SkillTargetString = StaticEnum<ETargetDefinition>()->GetNameByIndex(static_cast<int32>(TargetDefToPrint)).ToString();
	FString SkillTargetString = StaticEnum<ETargetDefinition>()->GetDisplayValueAsText(TargetDefToPrint).ToString();
	SkillTargetString.RemoveAt(0, 8);

	FString SkillReport;

	if (bIsCharacterAnAlly)
	{
		SkillReport = Name + " used " + ActiveSkillList->ActiveSkill.SkillName.ToString() + ". Target: " + SkillTargetBodyPartString + " of " + SkillTargetString;

		if (MainPlayerController == nullptr)
		{
			MainPlayerController = Cast<AMainPlayerController>(GetController());
		}

		/*if (!MainPlayerController->bEnglishDialogueOption)
		{
			SkillReport = Name + "は技" + ActiveSkillList->ActiveSkill.SkillName.ToString() + "を使いました. ターゲット: " + SkillTargetBodyPartString + "," + SkillTargetString;
		}*/

	}

	else 
	{
		SkillReport = Name + " used " + ActiveSkillList->ActiveSkill.SkillName.ToString() + ". Hit: " + SkillTargetBodyPartString;

		if (MainPlayerController == nullptr)
		{
			MainPlayerController = Cast<AMainPlayerController>(GetController());
		}

		/*if (!MainPlayerController->bEnglishDialogueOption)
		{
			SkillReport = Name + "は技" + ActiveSkillList->ActiveSkill.SkillName.ToString() + "を使いました. ターゲット: " + SkillTargetBodyPartString;
		}*/
	}
	
	BattleSystemReference->BattleLogReport(SkillReport);
}

void ABaseCharacter::ReportStance()
{
	FString StanceReport;

	if (bIsCharacterAnAlly)
	{
		StanceReport = Name + ". Stance: " + ActiveStance->StanceName.ToString() + " which " + ActiveStance->EffectABattleLogDescription + " and " + ActiveStance->EffectBBattleLogDescription;
	}

	else
	{
		StanceReport = Name + " changed stance to " + ActiveStance->StanceName.ToString();
	}

	BattleSystemReference->BattleLogReport(StanceReport);

}


float ABaseCharacter::CalculateSkillDamage(ESkillParameter SkillParameter, float ParameterScaling)
{
	float CalculatedDamage = 0;

	switch (SkillParameter)
	{
		case ESkillParameter::ESP_ATK:
		CalculatedDamage = FinalATK * ParameterScaling;
		UE_LOG(LogTemp, Warning, TEXT(" Calculate Skill Damage - F.ATK: %d - Scalling: %f - Dmg: %f"), FinalATK, ParameterScaling, CalculatedDamage);
		break;

		case ESkillParameter::ESP_TEC:
		CalculatedDamage = FinalTEC * ParameterScaling;
		UE_LOG(LogTemp, Warning, TEXT(" Calculate Skill Damage - F.TEC: %d - Scalling: %f - Dmg: %f"), FinalTEC, ParameterScaling, CalculatedDamage);
		break;

		case ESkillParameter::ESP_AGI:
		CalculatedDamage = FinalAGI * ParameterScaling;
		UE_LOG(LogTemp, Warning, TEXT(" Calculate Skill Damage - F.AGI: %d - Scalling: %f - Dmg: %f"), FinalAGI, ParameterScaling, CalculatedDamage);
		break;

		case ESkillParameter::ESP_DEF:
		CalculatedDamage = FinalDEF * ParameterScaling;
		UE_LOG(LogTemp, Warning, TEXT(" Calculate Skill Damage - F.DEF: %d - Scalling: %f - Dmg: %f"), FinalDEF, ParameterScaling, CalculatedDamage);
		break;

		//SPD is used to define number of combo actions per turn
		//EVA is used for dodging 
		//Thus none of them will have any meaning in dealing damage
		case ESkillParameter::ESP_SPD:
		case ESkillParameter::ESP_EVA:
		case ESkillParameter::ESP_EMPTY:
		default:
			UE_LOG(LogTemp, Warning, TEXT(" Calculate Skill Damage - Default/SPD/EVA/Empty case. Dmg: 0"));
			return 0;
		break;
	}

	return CalculateDamageWithHitBonus(CalculatedDamage, bIsCharacterAnAlly);


}

void ABaseCharacter::ReceiveSkillDamage(float CalculatedDamageToReceive, EBodyPartTarget TargetBodyPart, ABaseCharacter* AttackerReference)
{
	UE_LOG(LogTemp, Warning, TEXT(" ========== Receive Skill Damage =========="));

	//Check if the Character can receive any damage (e.g. he ain't dead)

	if (CharacterMainStatus == ECharacterStatus::ECS_Defeated)
	{
		//Do nothing
		return;
	}

	/*First we need to check for the Body Part that will be targered
	* The damage will change depending on the stance and the status of the body part
	*/

	UE_LOG(LogTemp, Warning, TEXT("Damage To Receive: %f"), CalculatedDamageToReceive);

	float DamageTaken = 0;


	switch (TargetBodyPart)
	{
		case EBodyPartTarget::EBPT_Head:
			DamageTaken = CalculatedDamageToReceive * ActiveStance->HeadDmgModifier * GetBodyPartStatusDamageMultiplier(EBodyPartTarget::EBPT_Head);
			UE_LOG(LogTemp, Warning, TEXT("Head Dmg Modifier: %f"), ActiveStance->HeadDmgModifier);
			UE_LOG(LogTemp, Warning, TEXT("Body Condition Modifier: %f"), GetBodyPartStatusDamageMultiplier(EBodyPartTarget::EBPT_Head));
			break;

		case EBodyPartTarget::EBPT_Chest:
			DamageTaken = CalculatedDamageToReceive * ActiveStance->ChestDmgModifier * GetBodyPartStatusDamageMultiplier(EBodyPartTarget::EBPT_Chest);
			UE_LOG(LogTemp, Warning, TEXT("Chest Dmg Modifier: %f"), ActiveStance->ChestDmgModifier);
			UE_LOG(LogTemp, Warning, TEXT("Body Condition Modifier: %f"), GetBodyPartStatusDamageMultiplier(EBodyPartTarget::EBPT_Chest));
			break;

		case EBodyPartTarget::EBPT_LeftArm:
			DamageTaken = CalculatedDamageToReceive * ActiveStance->LeftArmDmgModifier * GetBodyPartStatusDamageMultiplier(EBodyPartTarget::EBPT_LeftArm);
			UE_LOG(LogTemp, Warning, TEXT("LeftArm Dmg Modifier: %f"), ActiveStance->LeftArmDmgModifier);
			UE_LOG(LogTemp, Warning, TEXT("Body Condition Modifier: %f"), GetBodyPartStatusDamageMultiplier(EBodyPartTarget::EBPT_LeftArm));
			break;

		case EBodyPartTarget::EBPT_RightArm:
			DamageTaken = CalculatedDamageToReceive * ActiveStance->RightArmDmgModifier * GetBodyPartStatusDamageMultiplier(EBodyPartTarget::EBPT_RightArm);
			UE_LOG(LogTemp, Warning, TEXT("RightArm Dmg Modifier: %f"), ActiveStance->RightArmDmgModifier);
			UE_LOG(LogTemp, Warning, TEXT("Body Condition Modifier: %f"), GetBodyPartStatusDamageMultiplier(EBodyPartTarget::EBPT_RightArm));
			break;

		case EBodyPartTarget::EBPT_Legs:
			DamageTaken = CalculatedDamageToReceive * ActiveStance->LegsDmgModifier * GetBodyPartStatusDamageMultiplier(EBodyPartTarget::EBPT_Legs);
			UE_LOG(LogTemp, Warning, TEXT("Legs Dmg Modifier: %f"), ActiveStance->LegsDmgModifier);
			UE_LOG(LogTemp, Warning, TEXT("Body Condition Modifier: %f"), GetBodyPartStatusDamageMultiplier(EBodyPartTarget::EBPT_Legs));
			break;

		default:
			break;

	}

	UE_LOG(LogTemp, Warning, TEXT("Damage Taken Calculation before DEF: %f"), DamageTaken);

	//Apply Defense
	DamageTaken -= FinalDEF;

	UE_LOG(LogTemp, Warning, TEXT("F.DEF: %d"), FinalDEF);
	
	if (DamageTaken < 1)
		DamageTaken = 1;

	UE_LOG(LogTemp, Warning, TEXT("Damage Taken Calculation after DEF: %f"), DamageTaken);

	//We need to check if any body parts were damaged or broken
	CheckForUpdateInBodyPartCondition(TargetBodyPart, DamageTaken, AttackerReference->ResistanceAttackValue);

	ShowReceivedDamage(false, DamageTaken = FMath::FloorToInt(DamageTaken), AttackerReference->ActiveSkillList->ActiveSkill.NumberOfHits);

}

void ABaseCharacter::AddStatusEffect(EEffectStatus StatusToAdd, float Quantity, int Turns)
{
	if (StatusToAdd == EEffectStatus::EES_Empty)
	{
		UE_LOG(LogTemp, Warning, TEXT("No status to add"));
		//Do Nothing

	}

	else
	{
		FStatus NewStatus;
		NewStatus.StatusEffect = StatusToAdd;
		NewStatus.StatusQuantity = Quantity;
		NewStatus.StatusTurn = Turns - 1;

		if (NewStatus.StatusTurn < 0)
		{
			NewStatus.StatusTurn = 0;
		}

		FString SkillName = StaticEnum<EEffectStatus>()->GetNameByIndex(static_cast<int32>(NewStatus.StatusEffect)).ToString();

		if(StatusToAdd == EEffectStatus::EES_DamageOverTime || StatusToAdd == EEffectStatus::EES_DamageOverTimeMax || SkillName.Contains("Debuff"))
		{
			//We need to Add the Status Effect to the Target rather than the character
			UE_LOG(LogTemp, Warning, TEXT("Add new status to target"));
			SingleTargetReference->CharacterStatusChanges.Add(NewStatus);
			SingleTargetReference->ApplyStatusEffect(StatusToAdd, Quantity);
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("Add new status to self"));
			CharacterStatusChanges.Add(NewStatus);
			ApplyStatusEffect(StatusToAdd, Quantity);
		}
		
	}
}

void ABaseCharacter::ApplyStatusEffect(EEffectStatus StatusToApply, float Quantity)
{
	UE_LOG(LogTemp, Warning, TEXT(" ========== Apply Status Effect =========="));

	int32 QuantityCapped = FMath::FloorToInt(Quantity);

	FString EffectToApply = StaticEnum<EEffectStatus>()->GetNameByIndex(static_cast<int32>(StatusToApply)).ToString();
	UE_LOG(LogTemp, Warning, TEXT("Status to Apply: %s"), *EffectToApply);
	UE_LOG(LogTemp, Warning, TEXT(" Quantity: %f"), Quantity);
	UE_LOG(LogTemp, Warning, TEXT(" QuantityCapped: %d"), QuantityCapped);


	switch (StatusToApply)
	{
	case EEffectStatus::EES_DamageOverTime:
		ReceiveDirectDamage(QuantityCapped);
		break;

	case EEffectStatus::EES_DamageOverTimeMax:
		ReceiveDirectDamage(MaxHealth * QuantityCapped);
		break;

	case EEffectStatus::EES_HealHP:
		CurrentHealth += QuantityCapped;

		if (CurrentHealth > MaxHealth)
			CurrentHealth = MaxHealth;
		
		break;

	case EEffectStatus::EES_HealMaxHP:
		CurrentHealth = CurrentHealth + (MaxHealth * QuantityCapped);
	
		if (CurrentHealth > MaxHealth)
			CurrentHealth =  MaxHealth;
		break;

	case EEffectStatus::EES_HealMP:
		CurrentMagic += QuantityCapped;
		if (CurrentMagic > MaxMagic)
			CurrentMagic = MaxMagic;
		break;

	case EEffectStatus::EES_HealMaxMP:
		CurrentMagic = CurrentMagic + (MaxMagic * QuantityCapped);
		if (CurrentMagic > MaxMagic)
			CurrentMagic = MaxMagic;
		break;

	case EEffectStatus::EES_HealAP:
		CurrentAP += QuantityCapped;
		if (CurrentAP > Max_AP)
			CurrentAP = Max_AP;
		break;

	case EEffectStatus::EES_HealMaxAP:
		CurrentAP = CurrentAP + (QuantityCapped * Max_AP);
		if (CurrentAP > Max_AP)
			CurrentAP = Max_AP;
		break;

	case EEffectStatus::EES_BuffATK:
		ScallingATK = ScallingATK * (1 + Quantity/100);
		UpdateFinalStatus(ESkillParameter::ESP_ATK);
		break;
	case EEffectStatus::EES_DebuffATK:
		ScallingATK = ScallingATK * (1 - Quantity / 100);
		UpdateFinalStatus(ESkillParameter::ESP_ATK);
		break;

	case EEffectStatus::EES_BuffDEF:
		ScallingDEF = ScallingDEF * (1 + Quantity / 100);
		UpdateFinalStatus(ESkillParameter::ESP_DEF);
		break;

	case EEffectStatus::EES_DebuffDEF:
		ScallingDEF = ScallingDEF * (1 - Quantity / 100);
		UpdateFinalStatus(ESkillParameter::ESP_DEF);
		break;


	case EEffectStatus::EES_BuffSPD:
		ScallingSPD = ScallingSPD * (1 + Quantity / 100);
		//Only apply to SPD once it calls an update to all other status
		//UpdateFinalStatus(ESkillParameter::ESP_SPD);
		break;
	case EEffectStatus::EES_DebuffSPD:
		ScallingSPD = ScallingSPD * (1 - Quantity / 100);
		//Only apply to SPD once it calls an update to all other status
		//UpdateFinalStatus(ESkillParameter::ESP_SPD);
		break;

	case EEffectStatus::EES_BuffTEC:
		ScallingTEC = ScallingTEC * (1 + Quantity / 100);
		UpdateFinalStatus(ESkillParameter::ESP_TEC);
		break;
	case EEffectStatus::EES_DebuffTEC:
		ScallingTEC = ScallingTEC * (1 - Quantity / 100);
		UpdateFinalStatus(ESkillParameter::ESP_TEC);
		break;

	case EEffectStatus::EES_BuffEVA:
		ScallingEVA = ScallingEVA * (1 + Quantity / 100);
		UpdateFinalStatus(ESkillParameter::ESP_EVA);
		break;
	case EEffectStatus::EES_DebuffEVA:
		ScallingEVA = ScallingEVA * (1 - Quantity / 100);
		UpdateFinalStatus(ESkillParameter::ESP_EVA);
		break;

	case EEffectStatus::EES_BuffAGI:
		ScallingAGI = ScallingAGI * (1 + Quantity / 100);
		UpdateFinalStatus(ESkillParameter::ESP_AGI);
		break;
	case EEffectStatus::EES_DebuffAGI:
		ScallingAGI = ScallingAGI * (1 - Quantity / 100);
		UpdateFinalStatus(ESkillParameter::ESP_AGI);
		break;

	case EEffectStatus::EES_BodyPartExtraDamage:
		ResistanceAttackValue++;
		break;

	case EEffectStatus::EES_Charge:
		ChargeValue += Quantity;
		//To implement Charge
		break;

	case EEffectStatus::EES_FollowUp:
		//ToImplement
		break;

	case EEffectStatus::EES_ComboSupport:
		//ToImplement
		break;

	case EEffectStatus::EES_ComboBreak:
		//ToImplement
		break;

	default:
		ScallingATK = 1;
		ScallingDEF = 1;
		ScallingTEC = 1;
		ScallingEVA = 1;
		ScallingSPD = 1;
		ScallingAGI = 1;
		ChargeValue = 0;
		ResistanceAttackValue = 1;
		break;


	}

}

void ABaseCharacter::ApplyAllStatusEffect()
{
	UE_LOG(LogTemp, Warning, TEXT("============= Base Character - ApplyAllStatusEffect ============"));
	UE_LOG(LogTemp, Warning, TEXT(" Character - %s "), *Name);

	this->ClearStatusEffect();

	for (int i = 0; i < CharacterStatusChanges.Num(); i++)
	{

		UE_LOG(LogTemp, Warning, TEXT("Status #%d"), i);
		UE_LOG(LogTemp, Warning, TEXT("Turns Remaining before applying: %d"), CharacterStatusChanges[i].StatusTurn);

		if (CharacterStatusChanges[i].StatusTurn <= 0)
		{
			//if the remaining turns is less or equal than 0, then delete the status without applying the status
			//CharacterStatusChanges.RemoveAt(i);
			//i--;
			CharacterStatusChanges.RemoveAt(i, 1, false);
		}

		else
		{
			//Apply the status and removes 1 from remaining turns
			this->ApplyStatusEffect(CharacterStatusChanges[i].StatusEffect, CharacterStatusChanges[i].StatusQuantity);
			CharacterStatusChanges[i].StatusTurn -= 1;
			UE_LOG(LogTemp, Warning, TEXT("Turns Remaining after applying: %d"), CharacterStatusChanges[i].StatusTurn);
		}


	}

	CharacterStatusChanges.Shrink();
	UpdateFinalStatus(ESkillParameter::ESP_EMPTY); //Updates all Final Status 


}

void ABaseCharacter::ClearStatusEffect()
{
	UE_LOG(LogTemp, Warning, TEXT("=========== Base Character - ClearStatusEffect ==========="));
	ScallingATK = 1;
	ScallingDEF = 1;
	ScallingTEC = 1;
	ScallingEVA = 1;
	ScallingSPD = 1;
	ScallingAGI = 1;
	ChargeValue = 0; //To implement
	ResistanceAttackValue = 1;

}


/*
* ============================================================================================================================================
*								SPECIAL ART FUNCTIONS
* ============================================================================================================================================
*/

void ABaseCharacter::ExecuteSpecialArt(FName SkillName)
{
	ActiveSpecialArtList->SetActiveSpecialArt(SkillName);

	if (ActiveSpecialArtList->ValidSpecialArt)
	{
		//execute special art
		DamageCaused = 0;
		FinalSPD = 0;

		//Calculated all damage from each parameter
		DamageCaused += CalculateSkillDamage(ActiveSpecialArtList->ActiveSpecialArt.Parameter1, ActiveSpecialArtList->ActiveSpecialArt.Scaling1);
		DamageCaused += CalculateSkillDamage(ActiveSpecialArtList->ActiveSpecialArt.Parameter2, ActiveSpecialArtList->ActiveSpecialArt.Scaling2);
		DamageCaused += CalculateSkillDamage(ActiveSpecialArtList->ActiveSpecialArt.Parameter3, ActiveSpecialArtList->ActiveSpecialArt.Scaling3);

		switch (ActiveSpecialArtList->ActiveSpecialArt.Target)
		{
			case ETargetDefinition::ETD_AllySingle:
			case ETargetDefinition::ETD_AllyAll:
				//Activates Effects from Special Art
				ActiveSpecialArtList->ActivateAllEffectsFromSpecialArt();
				break;

			case ETargetDefinition::ETD_EnemySingle:
				//calculates target def and avoid chance
				SingleTargetReference->ReceiveSkillDamage(DamageCaused, ActiveSpecialArtList->ActiveSpecialArt.BodyPartTarget, this);
				//Activates Effects from Special Art
				ActiveSpecialArtList->ActivateAllEffectsFromSpecialArt();
				//executes Special Art animation 
				CharacterAnimationReference->SetupAndPlayAnimationMontage(EBattleAnimationSetup::EBAS_Special, ActiveSpecialArtList->ActiveSpecialArt.SpecialArtName);

				break;

			case ETargetDefinition::ETD_EnemyAll:
				BattleSystemReference->DamageAllOpponents(bIsCharacterAnAlly, DamageCaused, this);
				ActiveSpecialArtList->ActivateAllEffectsFromSpecialArt();
				//executes Special art animation
				CharacterAnimationReference->SetupAndPlayAnimationMontage(EBattleAnimationSetup::EBAS_Special, ActiveSpecialArtList->ActiveSpecialArt.SpecialArtName);
				break;
			default:
				break;

		}




	}

}

/*
* ============================================================================================================================================
*								BATTLE COMMANDS FUNCTIONS
* ============================================================================================================================================
*/

void ABaseCharacter::SetupAttackCommands(TArray<EAttackType> InputtedCommands, ABaseCharacter* TargetToAttack)
{
	UE_LOG(LogTemp, Warning, TEXT(" Base Character - Setup Attack Command "));
	if (CanCharacterExecuteAction(ECharacterAction::ECA_Attack))
	{
		Commands = InputtedCommands;
		Commands.Shrink();
		WaitTime = Commands.Num() * 10;
		CharacterAction = ECharacterAction::ECA_Attack;
		SingleTargetReference = TargetToAttack;
		UE_LOG(LogTemp, Warning, TEXT(" Base Character - CommandsSize = %d "), Commands.Num());
		UE_LOG(LogTemp, Warning, TEXT(" Base Character - Wait Time = %d "), WaitTime);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(" Base Character - Setup Attack Command FAILED"));
		//He cannot attack
		CharacterAction = ECharacterAction::ECA_Null;
	}
}

void ABaseCharacter::SetupSpecialCommand(FName SpecialAttackName, int MPCost)
{
	UE_LOG(LogTemp, Warning, TEXT(" Base Character - Setup Special Command "));
	if (CanCharacterExecuteAction(ECharacterAction::ECA_Special))
	{
		SpecialAction = SpecialAttackName;
		WaitTime = MPCost;
		CharacterAction = ECharacterAction::ECA_Special;
		UE_LOG(LogTemp, Warning, TEXT(" Base Character - Skill = %s - Wait Time = %d "), *SpecialAction.ToString(), WaitTime);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(" Base Character - Setup Special Command FAILED "));
		//He cannot use Special Command
		CharacterAction = ECharacterAction::ECA_Null;
		
	}
	

}

void ABaseCharacter::SetupStanceChange(FName StanceToChange)
{
	UE_LOG(LogTemp, Warning, TEXT(" Base Character - Setup Stance Change "));
	if (CanCharacterExecuteAction(ECharacterAction::ECA_Stance))
	{
		SpecialAction = StanceToChange;
		WaitTime = 0;
		CharacterAction = ECharacterAction::ECA_Stance;
		UE_LOG(LogTemp, Warning, TEXT(" Base Character - Stance = %s - Wait Time = %d "), *SpecialAction.ToString(), WaitTime);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(" Base Character - Setup Stance Change FAILED"));
		//He cannot change stances
		CharacterAction = ECharacterAction::ECA_Null;
	}
	

}


void ABaseCharacter::SetupItemUse()
{
	//Do nothing because Items were not implemented at this point

}

void ABaseCharacter::NextComboAttack()
{
	//CharacterAnimationReference->ResetAnimationBools();

	/* Will be called when attack begins and when an attack animation in a combo ends.
	* In the end, it will check if it's the last attack in the combo and end the turn
	*/
	if (CurrentComboAttack >= Commands.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT(" Base Character - Current Combo attack finished "));
		//The last attack was already executed so just return to it's original position
		//and end the action. 
		BattleSystemReference->ActionEnd();
		return;
	}

	if (bWasActiveSkillCountered)
	{
		UE_LOG(LogTemp, Warning, TEXT(" Base Character - Active Skill was countered, thus turn is finished"));
		//Skill was countered,, thus we only end the turn
		BattleSystemReference->ActionEnd();
		return;
	}


	UE_LOG(LogTemp, Warning, TEXT(" Base Character - Current Combo: %d") , CurrentComboAttack);

	float CalculatedDamage = 0;
	bWasActiveSkillCountered = false;

	/* By the time the attack starts, we already know in Commands where to trigger specials
	*  First it should check if it's a skill or not
	*  If it's a skill, as in the current build, it will be an unavoidable attack
	* if it's a regular attack, then it should check if it was dodged before calculating any damage
	*/

	if (Commands[CurrentComboAttack] == EAttackType::EAT_Special)
	{
		//A Special was triggered
		UE_LOG(LogTemp, Warning, TEXT(" Base Character - SpecialAttack Triggered "));

		//Set Active Skill
		ActiveSkillList->SetActiveSkillByIndex(CurrentComboAttack);


		//First check if skill targets an enemy or allies
		if (ActiveSkillList->ActiveSkill.Target == ETargetDefinition::ETD_EnemySingle || ActiveSkillList->ActiveSkill.Target == ETargetDefinition::ETD_EnemyAll)
		{
			//First, calculates the damage that will be caused since this damage can be used for counters
			CalculatedDamage += CalculateSkillDamage(ActiveSkillList->ActiveSkill.Parameter1, ActiveSkillList->ActiveSkill.Scaling1);
			CalculatedDamage += CalculateSkillDamage(ActiveSkillList->ActiveSkill.Parameter2, ActiveSkillList->ActiveSkill.Scaling2);
			CalculatedDamage += CalculateSkillDamage(ActiveSkillList->ActiveSkill.Parameter3, ActiveSkillList->ActiveSkill.Scaling3);

			DamageCaused = CalculatedDamage;

			UE_LOG(LogTemp, Warning, TEXT(" Base Character - SpecialAttack Triggered - DamageCaused: %f"), DamageCaused);

			/*If it targets an Enemy
			* If it's a single enemy, we need to check if a counter was activated in the target and if cover was activated on target allies 
			* If it's all enemies as target, we need to check if any of them triggered a counter
			* 
			*/
			BattleSystemReference->CheckAllOpponentsForCountersAndBlocks(this);

			//This means the active skill was not countered, thus we proceed to apply the damage. 
			//If the active skill was countered, the bool bWasAtiveSkillCountered should be changed on activate counter from the character that countered it
			if (!bWasActiveSkillCountered)
			{
				UE_LOG(LogTemp, Warning, TEXT(" Base Character - SpecialAttack Triggered - No counter"));

				//Add the magic regeneration for the attack
				// 22/Nov, Currently specials help with magic regeneration
				CurrentMagic += SingleAttackMagicRegeneration;
				if (CurrentMagic > MaxMagic)
					CurrentMagic = MaxMagic;

				AddStatusEffect(ActiveSkillList->ActiveSkill.Effect1, ActiveSkillList->ActiveSkill.EQuantity1, ActiveSkillList->ActiveSkill.EDuration1);
				AddStatusEffect(ActiveSkillList->ActiveSkill.Effect2, ActiveSkillList->ActiveSkill.EQuantity2, ActiveSkillList->ActiveSkill.EDuration2);
				AddStatusEffect(ActiveSkillList->ActiveSkill.Effect3, ActiveSkillList->ActiveSkill.EQuantity3, ActiveSkillList->ActiveSkill.EDuration3);

				if (ActiveSkillList->ActiveSkill.Target == ETargetDefinition::ETD_EnemySingle)
				{
					SingleTargetReference->ReceiveSkillDamage(CalculatedDamage, ActiveSkillList->ActiveSkill.BodyPartTarget, this);
				}

				if (ActiveSkillList->ActiveSkill.Target == ETargetDefinition::ETD_EnemyAll)
				{
					//Call Battle System Manager to cause damage to all opponents
					BattleSystemReference->DamageAllOpponents(bIsCharacterAnAlly, CalculatedDamage, this);
				}

				//Report the use of skill so that we can fill the battle log with the necessary information
				ReportSkill();


				/*Not going to work because the object itself will be destroyed and it's impossible to update datatable during runtime*/
				//ActiveSkillList->ActiveSkill.UsageCount++;


				//Only needs to record skill usage for allies
				if (bIsCharacterAnAlly)
				{
					//If the skill variable map already contains information about the used skill, just update it 
					if (SkillVariableInformationMap.Contains(ActiveSkillList->ActiveSkill.SkillName.ToString()))
					{
						SkillVariableInformationMap[ActiveSkillList->ActiveSkill.SkillName.ToString()].UsageCount++;
					}

					//First time using it
					else 
					{
						FSkillVariableData NewVariableData;
						SkillVariableInformationMap.Emplace(ActiveSkillList->ActiveSkill.SkillName.ToString(), NewVariableData);

						SkillVariableInformationMap[ActiveSkillList->ActiveSkill.SkillName.ToString()].SkillName = ActiveSkillList->ActiveSkill.SkillName.ToString();
						SkillVariableInformationMap[ActiveSkillList->ActiveSkill.SkillName.ToString()].UsageCount = 1;

						/*Update 02/12
						* Increase the Usage of skill.
						* If current Usage is 0, play learned skill particle effect
						*/

						if (LearnSkillParticles)
						{
							UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LearnSkillParticles, GetActorLocation());
						}

					}


				}


				/* SKILL ANIMATION CALL
				*/

				CharacterAnimationReference->SetupAndPlayAnimationMontage(EBattleAnimationSetup::EBAS_Skill, ActiveSkillList->ActiveSkill.SkillName);


			}

			//In the case of a counter, this character that did the skill will do nothing else
			//The character that caused the counter, will cause damage to this character and end it's turn

		}

		else 
		{
			//Targets are allies

			CalculatedDamage += CalculateSkillDamage(ActiveSkillList->ActiveSkill.Parameter1, ActiveSkillList->ActiveSkill.Scaling1);
			CalculatedDamage += CalculateSkillDamage(ActiveSkillList->ActiveSkill.Parameter2, ActiveSkillList->ActiveSkill.Scaling2);
			CalculatedDamage += CalculateSkillDamage(ActiveSkillList->ActiveSkill.Parameter3, ActiveSkillList->ActiveSkill.Scaling3);

			if (ActiveSkillList->ActiveSkill.Target == ETargetDefinition::ETD_AllySingle)
			{
				/*In the current implementation, it will be a self heal
				* Change code if there's another use
				* If character health drops to 0, it should not even enter this function as EndTurn should be called imediately 
				*/

				ReceiveHPHeal(CalculatedDamage);

			}

			if (ActiveSkillList->ActiveSkill.Target == ETargetDefinition::ETD_AllyAll)
			{
				//Call Battle System Manager to heal all allies
				BattleSystemReference->HealAllAllies(bIsCharacterAnAlly, CalculatedDamage);
			}
		}
		
	}

	else
	{
		//A special was not triggered, thus it's a regular attack
		UE_LOG(LogTemp, Warning, TEXT(" Base Character - Regular Attack "));
		//First check if the attack will be dodged
		if (SingleTargetReference->bIsAttackAvoided(this))
		{
			//Animates attack
			CharacterAnimationReference->SetupNormalAttackAnimation(Commands[CurrentComboAttack]);

			//if the attack was dodged
			UE_LOG(LogTemp, Warning, TEXT(" Base Character - Attack Dodged "));
			SingleTargetReference->ShowReceivedDamage(true, 0, 0);
			
		}

		else
		{
			UE_LOG(LogTemp, Warning, TEXT(" Base Character - Attack Hit "));
			//If the attack was not dodged, calculate the damage normally
			CalculatedDamage = CalculateRegularDamage(Commands[CurrentComboAttack]);
			SingleTargetReference->ReceiveRegularDamage(CalculatedDamage, Commands[CurrentComboAttack], this);

			//Add the magic regeneration for the single attack
			CurrentMagic += SingleAttackMagicRegeneration;
			if (CurrentMagic > MaxMagic)
				CurrentMagic = MaxMagic;

			//Play attack animation
			UE_LOG(LogTemp, Warning, TEXT(" Base Character - Attack Animation Call "));
			CharacterAnimationReference->SetupNormalAttackAnimation(Commands[CurrentComboAttack]);
		}
	}

	//By last, update the CurrentCombo Attack (currently updated in character animation manager, after montage finishes playing)
	//CurrentComboAttack++;
}

void ABaseCharacter::StartAction()
{
	UE_LOG(LogTemp, Warning, TEXT(" Base Character - Start Action"));

	if (!CanCharacterExecuteAction(CharacterAction))
	{
		//If he cannot execute his action.
		//Just end his action
		BattleSystemReference->ActionEnd();
	}

	else 
	{
		//Check if the character target needs to me adjusted
		if (CharacterAction == ECharacterAction::ECA_Attack || (CharacterAction == ECharacterAction::ECA_Special && AttackTarget == ETargetDefinition::ETD_EnemySingle))
		{
			if (SingleTargetReference == nullptr)
			{
				//Will assign a target if the Single Target Reference is null. Useful for enemy AI. 
				UE_LOG(LogTemp, Warning, TEXT(" Base Character - Assign new target because SingleTargetReference is null "));
				BattleSystemReference->AssignNewTarget(this);
			}

			else 
			{
				if (SingleTargetReference->CharacterMainStatus == ECharacterStatus::ECS_Defeated)
				{
					//If the target character is already defeated, then we need to check for another target
					UE_LOG(LogTemp, Warning, TEXT(" Base Character - Assign new target because SingleTargetReference is defeated "));
					BattleSystemReference->AssignNewTarget(this);
				}
			}
		}

		//This switch is the character action being executed
		UE_LOG(LogTemp, Warning, TEXT(" Base Character - Action execution "));
		int CommandSize = 0;

		switch (CharacterAction)
		{
			case ECharacterAction::ECA_Attack:
				UE_LOG(LogTemp, Warning, TEXT(" Base Character - AttackAction "));
				CommandSize = Commands.Num();

				//Decrease FinalSPD in according to the number of commmands inputted
				FinalSPD -= CommandSize * 10;


				UE_LOG(LogTemp, Warning, TEXT(" Base Character %s - CommandsSize: %d - FinalSPD: %d "),*Name,  CommandSize, FinalSPD);
				//The SetupTurnSkills will search for the skills that can be activated 
				//And update Commands with EAT_Special in the indexes where we should activate a skill
				if (ActiveSkillList)
				{
					UE_LOG(LogTemp, Warning, TEXT(" Base Character - AttackAction - CheckSkills "));
					Commands = ActiveSkillList->CheckTurnSkills(Commands);
				}
				

				//Moving to target - TO IMPLEMENT

				//Deal first attack
				UE_LOG(LogTemp, Warning, TEXT(" First Attack "));
				CurrentComboAttack = 0;
				NextComboAttack();

				break;

			case ECharacterAction::ECA_Special:
				UE_LOG(LogTemp, Warning, TEXT(" Base Character - Special Action "));
				ExecuteSpecialArt(SpecialAction);
				break;

			case ECharacterAction::ECA_Stance:
				UE_LOG(LogTemp, Warning, TEXT(" Base Character - Stance Action "));
				ChangeStance(SpecialAction);
				BattleSystemReference->ActionEnd();
				break;

			case ECharacterAction::ECA_Item:
				break;
			default:
				break;
		}

	}

	

}

void ABaseCharacter::EndAction()
{
	//CharacterAnimationReference->ResetAnimationBools();
	/*Function called after a character action ends
	* Checks if character is defeated
	* Checks for ressurection or other after defeated effects
	*/

	UE_LOG(LogTemp, Warning, TEXT(" Base Character - End Action "));

	if (CurrentHealth == 0)
	{
		bool bRevived = false;

		//Check for revive condition
		for (int i = 0; i < CharacterStatusChanges.Num(); i++)
		{
			if (CharacterStatusChanges[i].StatusEffect == EEffectStatus::EES_Revive)
			{
				CurrentHealth = CharacterStatusChanges[i].StatusQuantity * MaxHealth;
				bRevived = true;
				CharacterStatusChanges.RemoveAt(i, 1, true);
				//Revival Animation
				BattleSystemReference->BattleLogReport(Name + " revives");
				CharacterAnimationReference->PlayRegularAnimation(EBattleAnimationSetup::EBAS_Revive);
				break;
			}
		}

		if(!bRevived)
		{
			CharacterMainStatus = ECharacterStatus::ECS_Defeated;
			//Defeated Animation
			CharacterAnimationReference->PlayRegularAnimation(EBattleAnimationSetup::EBAS_Defeat);
		}
		
	}

}

void ABaseCharacter::BeginTurn()
{
	//All actions the character performs when a new turn begins
	UE_LOG(LogTemp, Warning, TEXT(" Base Character - BeginTurn "));
	//First, check if character is alive

	if (CharacterMainStatus != ECharacterStatus::ECS_Defeated)
	{
		UE_LOG(LogTemp, Warning, TEXT(" Base Character - BeginTurn %d - SPD Analysis "), BattleSystemReference->TurnNumber);
		UE_LOG(LogTemp, Warning, TEXT(" Character - %s "), *Name);
		UE_LOG(LogTemp, Warning, TEXT(" BaseSPD: %d "), BaseSPD);
		UE_LOG(LogTemp, Warning, TEXT(" FinalSPDRemaining: %d "), FinalSPD);
		UE_LOG(LogTemp, Warning, TEXT(" SPDRetentionRate: %f "), ActiveStance->SPDRetentionRate);
		UE_LOG(LogTemp, Warning, TEXT(" SPD Regeneration: %d "), SPD_Regeneration);

		if (BattleSystemReference->TurnNumber != 1)
		{
			//Regenerate SPD: The remaining SPD from the last turn + regeneration
			BaseSPD = (FinalSPD * (ActiveStance->SPDRetentionRate)) + SPD_Regeneration;
		}
		
		UE_LOG(LogTemp, Warning, TEXT(" New BaseSPD: %d "), BaseSPD);

		/*Clears all status effects and reapplies all status effects that are still active.
		* Removes status effects that have ended
		* Updates all Final Status
		* Final SPD will also be updated, considering the multipler made by battle sys manager and the character scalling SPD
		* Should be called ONCE and only when the turn starts.
		*/
		ApplyAllStatusEffect();

		if (FinalSPD > 200)
			FinalSPD = 200;
	}
	


}

void ABaseCharacter::EndTurn()
{
	CharacterAction = ECharacterAction::ECA_Null;
	Commands.Empty();
	SpecialAction = "";
	WaitTime = 0;
	bWasActiveSkillCountered = false;
	
	//Magic and AP regeneration happen at the end of turn

	if (CurrentHealth > 0)
	{
		CurrentMagic += MagicRegeneration;
		if (CurrentMagic > MaxMagic)
			CurrentMagic = MaxMagic;

		CurrentAP += AP_Regeneration;
		if (CurrentAP > Max_AP)
			CurrentAP = Max_AP;
	}

	//Ressurection is already checked in ActionEnd
	else
	{
		CharacterAction = ECharacterAction::ECA_Defeated;
	}

}


/*
* ============================================================================================================================================
*								OTHER BATTLE RELATED FUNCTIONS
* ============================================================================================================================================
*/


void ABaseCharacter::UpdateFinalStatus(ESkillParameter StatusToUpdate)
{
	/* Updates Final Status (Base * Scalling * BattleSysManagerModifier)
	* Can Update a single status (useful for buffs and debuffs) or all of them at once (Use ESP_MAX for this)
	*/

	if (BattleSystemReference == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT(" Base Character - UpdateFinalStatus - BattleSysReference Null "));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT(" ======Update Status for Character - %s ==========="), *Name);

	switch(StatusToUpdate)
	{
		case ESkillParameter::ESP_EMPTY:
		case ESkillParameter::ESP_MAX:
		default:
			UE_LOG(LogTemp, Warning, TEXT("=========== Base Character - UpdateFinalStatus - ALL =========="));
			FinalATK = FMath::FloorToInt(BaseATK * ScallingATK * BattleSystemReference->ATK_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("BaseATK: %d, ScallingATK: %f , ATKMod: %f"), BaseATK, ScallingATK, BattleSystemReference->ATK_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("FinalATK before FloorToInt: %f"), BaseATK * ScallingATK * BattleSystemReference->ATK_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("FinalATK: %d"), FinalATK);

			FinalDEF = FMath::FloorToInt(BaseDEF * ScallingDEF * BattleSystemReference->DEF_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("BaseDEF: %d, ScallingDEF: %f , DEFMod: %f"), BaseDEF, ScallingDEF, BattleSystemReference->DEF_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("FinalDEF before FloorToInt: %f"), BaseDEF * ScallingDEF * BattleSystemReference->DEF_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("FinalDEF: %d"), FinalDEF);

			FinalTEC = FMath::FloorToInt(BaseTEC * ScallingTEC * BattleSystemReference->TEC_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("BaseTEC: %d, ScallingTEC: %f , TECMod: %f"), BaseTEC, ScallingTEC, BattleSystemReference->TEC_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("FinalTEC before FloorToInt: %f"), BaseTEC * ScallingTEC * BattleSystemReference->TEC_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("FinalTEC: %d"), FinalTEC);

			FinalAGI = FMath::FloorToInt(BaseAGI * ScallingAGI * BattleSystemReference->AGI_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("BaseAGI: %d, ScallingAGI: %f , AGIMod: %f"), BaseAGI, ScallingAGI, BattleSystemReference->AGI_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("FinalAGI before FloorToInt: %f"), BaseAGI * ScallingAGI * BattleSystemReference->AGI_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("FinalAGI: %d"), FinalAGI);

			FinalSPD = FMath::FloorToInt(BaseSPD * ScallingSPD * BattleSystemReference->SPD_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("BaseSPD: %d, ScallingSPD: %f , SPDMod: %f"), BaseSPD, ScallingSPD, BattleSystemReference->SPD_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("FinalSPD before FloorToInt: %f"), BaseSPD * ScallingSPD * BattleSystemReference->SPD_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("FinalSPD: %d"), FinalSPD);

			FinalEVA = FMath::FloorToInt(BaseEVA * ScallingEVA * BattleSystemReference->EVA_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("BaseEVA: %d, ScallingEVA: %f , EVAMod: %f"), BaseEVA, ScallingEVA, BattleSystemReference->EVA_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("FinalEVA before FloorToInt: %f"), BaseEVA * ScallingEVA * BattleSystemReference->EVA_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("FinalEVA: %d"), FinalEVA);
			break;

		case ESkillParameter::ESP_ATK:
			UE_LOG(LogTemp, Warning, TEXT("=========== Base Character - UpdateFinalStatus - ATK =========="));
			FinalATK = FMath::FloorToInt(BaseATK * ScallingATK * BattleSystemReference->ATK_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("BaseATK: %d, ScallingATK: %f , ATKMod: %f"), BaseATK, ScallingATK, BattleSystemReference->ATK_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("FinalATK before FloorToInt: %f"), BaseATK * ScallingATK * BattleSystemReference->ATK_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("FinalATK: %d"), FinalATK);
			break;

		case ESkillParameter::ESP_DEF:
			UE_LOG(LogTemp, Warning, TEXT("=========== Base Character - UpdateFinalStatus - DEF =========="));
			FinalDEF = FMath::FloorToInt(BaseDEF * ScallingDEF * BattleSystemReference->DEF_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("BaseDEF: %d, ScallingDEF: %f , DEFMod: %f"), BaseDEF, ScallingDEF, BattleSystemReference->DEF_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("FinalDEF before FloorToInt: %f"), BaseDEF * ScallingDEF * BattleSystemReference->DEF_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("FinalDEF: %d"), FinalDEF);
			break;

		case ESkillParameter::ESP_TEC:
			UE_LOG(LogTemp, Warning, TEXT("=========== Base Character - UpdateFinalStatus - TEC =========="));
			FinalTEC = FMath::FloorToInt(BaseTEC * ScallingTEC * BattleSystemReference->TEC_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("BaseTEC: %d, ScallingTEC: %f , TECMod: %f"), BaseTEC, ScallingTEC, BattleSystemReference->TEC_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("FinalTEC before FloorToInt: %f"), BaseTEC * ScallingTEC * BattleSystemReference->TEC_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("FinalTEC: %d"), FinalTEC);
			break;

		case ESkillParameter::ESP_AGI:
			UE_LOG(LogTemp, Warning, TEXT("=========== Base Character - UpdateFinalStatus - AGI =========="));
			FinalAGI = FMath::FloorToInt(BaseAGI * ScallingAGI * BattleSystemReference->AGI_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("BaseAGI: %d, ScallingAGI: %f , AGIMod: %f"), BaseAGI, ScallingAGI, BattleSystemReference->AGI_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("FinalAGI before FloorToInt: %f"), BaseAGI * ScallingAGI * BattleSystemReference->AGI_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("FinalAGI: %d"), FinalAGI);
			break;

		case ESkillParameter::ESP_SPD:
			UE_LOG(LogTemp, Warning, TEXT("=========== Base Character - UpdateFinalStatus - SPD =========="));
			FinalSPD = FMath::FloorToInt(BaseSPD * ScallingSPD * BattleSystemReference->SPD_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("BaseSPD: %d, ScallingSPD: %f , SPDMod: %f"), BaseSPD, ScallingSPD, BattleSystemReference->SPD_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("FinalSPD before FloorToInt: %f"), BaseSPD * ScallingSPD * BattleSystemReference->SPD_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("FinalSPD: %d"), FinalSPD);
			break;

		case ESkillParameter::ESP_EVA:
			UE_LOG(LogTemp, Warning, TEXT("=========== Base Character - UpdateFinalStatus - EVA =========="));
			FinalEVA = FMath::FloorToInt(BaseEVA * ScallingEVA * BattleSystemReference->EVA_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("BaseEVA: %d, ScallingEVA: %f , EVAMod: %f"), BaseEVA, ScallingEVA, BattleSystemReference->EVA_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("FinalEVA before FloorToInt: %f"), BaseEVA * ScallingEVA * BattleSystemReference->EVA_Modifier);
			UE_LOG(LogTemp, Warning, TEXT("FinalEVA: %d"), FinalEVA);
			break;

	}
}


void ABaseCharacter::AddConditionEffect(ECharacterStatus ConditionToAdd, float Quantity, int Turns)
{

}

void ABaseCharacter::ApplyConditionEffect(ECharacterStatus ConditionToApply, float Quantity)
{

}

void ABaseCharacter::ApplyAllConditionEffect()
{

}

void ABaseCharacter::ClearConditionEffect()
{

}


float ABaseCharacter::CalculateRegularDamage(EAttackType InputtedCommand)
{
	UE_LOG(LogTemp, Warning, TEXT("=========== Base Character - Calculate Regular Damage =========="));
	UE_LOG(LogTemp, Warning, TEXT("Dmg = Final Status * Stance Modifier"));
	float CalculatedDamage = 0;

	switch(InputtedCommand)
	{
		case EAttackType::EAT_High:
			CalculatedDamage = (FinalATK * ActiveStance->HighAtkModifier);
			UE_LOG(LogTemp, Warning, TEXT("High - F.ATK: %d - StanceMod: %f - Dmg: %f"), FinalATK, ActiveStance->HighAtkModifier, CalculatedDamage);
			break;

		case EAttackType::EAT_Left:
			CalculatedDamage = (FinalATK * ActiveStance->LeftAtkModifier);
			UE_LOG(LogTemp, Warning, TEXT("Left - F.ATK: %d - StanceMod: %f - Dmg: %f"), FinalATK, ActiveStance->LeftAtkModifier, CalculatedDamage);
			break;

		case EAttackType::EAT_Right:
			CalculatedDamage = (FinalATK * ActiveStance->RightAtkModifier);
			UE_LOG(LogTemp, Warning, TEXT("Right - F.ATK: %d - StanceMod: %f - Dmg: %f"), FinalATK, ActiveStance->RightAtkModifier, CalculatedDamage);
			break;

		case EAttackType::EAT_Mid:
			CalculatedDamage = (FinalATK * ActiveStance->MidAtkModifier);
			UE_LOG(LogTemp, Warning, TEXT("Mid - F.ATK: %d - StanceMod: %f - Dmg: %f"), FinalATK, ActiveStance->MidAtkModifier, CalculatedDamage);
			break;

		case EAttackType::EAT_Low:
			CalculatedDamage = (FinalATK * ActiveStance->LowAtkModifier);
			UE_LOG(LogTemp, Warning, TEXT("Low - F.ATK: %d - StanceMod: %f - Dmg: %f"), FinalATK, ActiveStance->LowAtkModifier, CalculatedDamage);
			break;

		default:
			return 0;
			break;
	}

	if (CalculatedDamage <= 0)
	{
		return 1;
	}

	else
	{
		return CalculateDamageWithHitBonus(CalculatedDamage, bIsCharacterAnAlly);
	}

}

float ABaseCharacter::CalculateDamageWithHitBonus(float CalculatedDamage, bool bIsAnAlly)
{
	//BattleSystemManager HIT BONUS dmg
	
	if (bIsAnAlly)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ally Hit Bonus Damage is: %f"), BattleSystemReference->AlliesExtraDmgModifier);
		return (CalculatedDamage * BattleSystemReference->AlliesExtraDmgModifier);
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy Hit Bonus Damage is: %f"), BattleSystemReference->EnemiesExtraDmgModifier);
		return (CalculatedDamage * BattleSystemReference->EnemiesExtraDmgModifier);
	}


}

/*As the current build, the formula will be static
* DamageThatWasToBeCaused + Final ATK + Final TEC
* This damage will be applied to ReceiveCounter which will not use Final DEF to mitigate damage and it will multiply by the body part condition
*/
float ABaseCharacter::CalculateCounterDamage(float DamageThasWasToBeCaused)
{
	UE_LOG(LogTemp, Warning, TEXT("=========== Base Character - Calculate Counter Damage =========="));
	UE_LOG(LogTemp, Warning, TEXT("Damage That Was To Be Caused: %f"), DamageThasWasToBeCaused);
	UE_LOG(LogTemp, Warning, TEXT("F.ATK: %d"), FinalATK);
	UE_LOG(LogTemp, Warning, TEXT("F.TEC: %d"), FinalTEC);
	return DamageThasWasToBeCaused + FinalATK + FinalTEC;
}

void ABaseCharacter::ReceiveRegularDamage(float CalculatedDamageToReceive, EAttackType TypeOfAttack, ABaseCharacter* AttackerReference)
{
	UE_LOG(LogTemp, Warning, TEXT("=========== Base Character - Receive Regular Damage =========="));
	UE_LOG(LogTemp, Warning, TEXT("Calculated Damage To Receive: %f"), CalculatedDamageToReceive);
	//Check if the Character can receive any damage (e.g. he ain't dead). This check is only for security since a defeated character 
	//should not be able to be targeted 
	if (CharacterMainStatus == ECharacterStatus::ECS_Defeated)
	{
		//Do nothing
		return;
	}

	//Calculates the damage received by a single regular attack
	//And sends that information to BattleSystemManager so that the proper value can be shown on screen
	//CalculatedDamageToReceive is already the attack force considering all variables from attacker, so first we need to check the modifiers from stance and body part condition

	float DamageTaken = 0;

	switch(TypeOfAttack)
	{
		case EAttackType::EAT_High:
			DamageTaken = CalculatedDamageToReceive * ActiveStance->HeadDmgModifier * GetBodyPartStatusDamageMultiplier(EBodyPartTarget::EBPT_Head);
			UE_LOG(LogTemp, Warning, TEXT("Head StanceDmgMod: %f"), ActiveStance->HeadDmgModifier);
			UE_LOG(LogTemp, Warning, TEXT("BodyPartCondition Multiplier: %f"), GetBodyPartStatusDamageMultiplier(EBodyPartTarget::EBPT_Head));
			break;
	
		case EAttackType::EAT_Mid:
			DamageTaken = CalculatedDamageToReceive * ActiveStance->ChestDmgModifier * GetBodyPartStatusDamageMultiplier(EBodyPartTarget::EBPT_Chest);
			UE_LOG(LogTemp, Warning, TEXT("Chest StanceDmgMod: %f"), ActiveStance->ChestDmgModifier);
			UE_LOG(LogTemp, Warning, TEXT("BodyPartCondition Multiplier: %f"), GetBodyPartStatusDamageMultiplier(EBodyPartTarget::EBPT_Chest));
			break;

		case EAttackType::EAT_Right:
			//If someone punches with their Right Hand, it's the target left arm that will receive the damage
			DamageTaken = CalculatedDamageToReceive * ActiveStance->LeftArmDmgModifier * GetBodyPartStatusDamageMultiplier(EBodyPartTarget::EBPT_LeftArm);
			UE_LOG(LogTemp, Warning, TEXT("LeftArm StanceDmgMod: %f"), ActiveStance->LeftArmDmgModifier);
			UE_LOG(LogTemp, Warning, TEXT("BodyPartCondition Multiplier: %f"), GetBodyPartStatusDamageMultiplier(EBodyPartTarget::EBPT_LeftArm));
			break;

		case EAttackType::EAT_Left:
			//If someone punches with their Left Hand, it's the target right arm that will receive the damage
			DamageTaken = CalculatedDamageToReceive * ActiveStance->RightArmDmgModifier * GetBodyPartStatusDamageMultiplier(EBodyPartTarget::EBPT_RightArm);
			UE_LOG(LogTemp, Warning, TEXT("RightArm StanceDmgMod: %f"), ActiveStance->RightArmDmgModifier);
			UE_LOG(LogTemp, Warning, TEXT("BodyPartCondition Multiplier: %f"), GetBodyPartStatusDamageMultiplier(EBodyPartTarget::EBPT_RightArm));
			break;
				
		case EAttackType::EAT_Low:
			DamageTaken = CalculatedDamageToReceive * ActiveStance->LegsDmgModifier * GetBodyPartStatusDamageMultiplier(EBodyPartTarget::EBPT_Legs);
			UE_LOG(LogTemp, Warning, TEXT("Legs StanceDmgMod: %f"), ActiveStance->LegsDmgModifier);
			UE_LOG(LogTemp, Warning, TEXT("BodyPartCondition Multiplier: %f"), GetBodyPartStatusDamageMultiplier(EBodyPartTarget::EBPT_Legs));
			break;

		default:
			break;
	}

	UE_LOG(LogTemp, Warning, TEXT("Damage Taken Before DEF: %f"), DamageTaken);

	//After that we need to apply the defense
	DamageTaken -= FinalDEF;

	UE_LOG(LogTemp, Warning, TEXT("Damage Taken After DEF: %f"), DamageTaken);

	if (DamageTaken < 1)
		DamageTaken = 1; 

	ShowReceivedDamage(false, DamageTaken = FMath::FloorToInt(DamageTaken), 1);

}

void ABaseCharacter::ReceiveDirectDamage(int CalculatedDamageToReceive)
{
	//Health is already updated once ShowReceivedDamage is called
	ShowReceivedDamage(false, CalculatedDamageToReceive, 0);
}

void ABaseCharacter::ReceiveCounter(int DamageToBeReceived, EBodyPartTarget TargetBodyPart)
{
	UE_LOG(LogTemp, Warning, TEXT("===== Base Character - Receive Counter ========"));

	float CounterDamage = DamageToBeReceived * GetBodyPartStatusDamageMultiplier(TargetBodyPart);

	UE_LOG(LogTemp, Warning, TEXT("Dmg To Receive: %d"), DamageToBeReceived);
	UE_LOG(LogTemp, Warning, TEXT("BodyPart Multiplier: %f"), GetBodyPartStatusDamageMultiplier(TargetBodyPart));
	UE_LOG(LogTemp, Warning, TEXT("Counter Damage: %f"), CounterDamage);

	CheckForUpdateInBodyPartCondition(TargetBodyPart, CounterDamage, ResistanceAttackValue);

	bWasActiveSkillCountered = true;
	
	//Health will be updated once ShowReceivedDamage is called
	ShowReceivedDamage(false, CounterDamage, 1);

}

bool ABaseCharacter::bIsAttackAvoided(ABaseCharacter* AttackerReference)
{
	/*Makes an attempt to dodge the attack
	* Current formula will be: EVA + (AGI - Attacker TEC) 
	* AGI - TEC cannot be less than 0 
	* AvoidChance is then divided by 100 to bring it to a % and if the RandomNumber is lower than it, the attack was avoided
	*/

	UE_LOG(LogTemp, Warning, TEXT("===== Base Character -Is Attack Avoided ========"));

	//If character is already defeated, no  chance of dodging 
	if (CurrentHealth == 0)
		return false;

	float AvoidChance = (FinalAGI - AttackerReference->FinalTEC);

	UE_LOG(LogTemp, Warning, TEXT("Character F. AGI: %d , Attacker F.TEC: %d"), FinalAGI, AttackerReference->FinalTEC);
	UE_LOG(LogTemp, Warning, TEXT("AvoidChance before EVA: %f"), AvoidChance);

	if (AvoidChance < 0)
			AvoidChance = 0;

	AvoidChance = (FinalEVA+AvoidChance) / 100;

	UE_LOG(LogTemp, Warning, TEXT("AvoidChance after EVA: %f %"), AvoidChance);

	float RandomNumber = FMath::FRandRange(0,1);

	UE_LOG(LogTemp, Warning, TEXT("Random Number between 0 and 1: %f"), RandomNumber);

	if (RandomNumber > AvoidChance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack Hit"));
		return false;
	}
		

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack Dodged"));
		return true;
	}
		
}

void ABaseCharacter::ReceiveHPHeal(int QuantityToHeal)
{
	UE_LOG(LogTemp, Warning, TEXT("===== Base Character -Receive HP Heal ========"));

	CurrentHealth += QuantityToHeal;
		
	if (CurrentHealth >  MaxHealth)
	{
		int Overhealth = CurrentHealth - MaxHealth;
		CurrentHealth =  MaxHealth;
		ShowReceivedHeal(QuantityToHeal - Overhealth);
	}
			
	else
	{
		ShowReceivedHeal(QuantityToHeal);
	}
	
}

void ABaseCharacter::ShowReceivedHeal(int HealedQuantity)
{
	//Show a green number with the quantity that was healed
	FString HealText = FString::FromInt(HealedQuantity);
	FString HealReport = Name + " healed " + HealText;
	BattleSystemReference->BattleLogReport(HealReport);

}

void  ABaseCharacter::ShowReceivedDamage(bool IsAttackMissed, int DamageReceived, int NumberOfHitsTaken)
{
	UE_LOG(LogTemp, Warning, TEXT("===== Base Character - Show Received Damage ========"));

	if (!IsAttackMissed)
	{
		//If Attack Hit

		//show damage value on screen - to implement. DamageTakenToShowOnScreen will be the value shown on screen
		DamageTakenToShowOnScreen = DamageReceived;

		FString DamageText = FString::FromInt(DamageTakenToShowOnScreen);
		FString DmgReport = Name + " received " + DamageText + " damage";
		BattleSystemReference->BattleLogReport(DmgReport);

		UE_LOG(LogTemp, Warning, TEXT(" Base Character - ShowReceivedDamage - Health Before Dmg = %d "), CurrentHealth);
		UE_LOG(LogTemp, Warning, TEXT(" Base Character - ShowReceivedDamage - Damage = %d "), DamageTakenToShowOnScreen);

		//update health
		CurrentHealth -= DamageReceived;
		if (CurrentHealth < 0)
			CurrentHealth = 0;

		UE_LOG(LogTemp, Warning, TEXT(" Base Character - ShowReceivedDamage - CurrentHealth = %d "), CurrentHealth);

		//update battle system with hit and damage count
		BattleSystemReference->UpdateHitAndDamageCount(bIsCharacterAnAlly, DamageReceived, NumberOfHitsTaken);

		//play taking damage animation 
		CharacterAnimationReference->PlayRegularAnimation(EBattleAnimationSetup::EBAS_Hurt);

		//Particles
		if (HitParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, GetActorLocation());
		}

	}

	else
	{
		//If Attack Missed
		//play dodge animation
		UE_LOG(LogTemp, Warning, TEXT(" Base Character - Attack Dodge true "));
		BattleSystemReference->BattleLogReport(Name + " dodged attack.");
		CharacterAnimationReference->PlayRegularAnimation(EBattleAnimationSetup::EBAS_Dodge);
		//show MISS on screen - to implement


	}


}

void ABaseCharacter::CheckForUpdateInBodyPartCondition(EBodyPartTarget TargetBodyPart, int DamageToBodyPart, int DamageToResistance)
{
	switch (TargetBodyPart)
	{
		case EBodyPartTarget::EBPT_Head:
			//First, update the Resistance damage
			HeadComboRes -= DamageToResistance;

			//if it's broken already, there's no need to do anything
			if (HeadStatus != EBodyPartStatus::EBPS_Broken)
			{
				if (DamageToBodyPart > HeadBreakDmgThreshold)
				{
					//This case means the damage was so high that it broke the body part
					HeadStatus = EBodyPartStatus::EBPS_Broken;

					//Makes a call to anim instance so that the animation will be played out later
					CharacterAnimationReference->SetupForBrokenBodyPartAnimation(TargetBodyPart, HeadStatus);

					return;
				}

				if (HeadStatus == EBodyPartStatus::EBPS_Damaged)
				{
					//In this case, we need to check if the damage to the resistance was enough to break the body part
					if (HeadComboRes <= 0)
					{
						//This case means the body part was broken by a lack of resistance resistance
						HeadStatus = EBodyPartStatus::EBPS_Broken;

						//Updates the resistance value to the renewed value
						HeadComboRes = ComboResRenewValue;

						//Makes a call to anim instance so that the animation will be played out later
						CharacterAnimationReference->SetupForBrokenBodyPartAnimation(TargetBodyPart, HeadStatus);

						return;

					}

					//if it was not broken, just return.
					return;

				}

				else 
				{
					//This case is where the condition of the body part is normal so we need to check
					//if the body part was damaged by the damage taken or by a lack of resistance or if nothing happened
					
					if (DamageToBodyPart > HeadDmgThreshold)
					{
						//This means the damage was high enough to damage the body part, but not break it
						HeadStatus = EBodyPartStatus::EBPS_Damaged;

						//Updates the resistance value to the renewed value
						HeadComboRes = ComboResRenewValue;

						//Makes a call to anim instance so that the animation will be played out later
						CharacterAnimationReference->SetupForBrokenBodyPartAnimation(TargetBodyPart, HeadStatus);

						return;
					}

					else 
					{
						//If the damage was not enough, check for the resistance

						if (HeadComboRes <= 0)
						{
							//This case means the body part was broken by a lack of resistance resistance
							HeadStatus = EBodyPartStatus::EBPS_Damaged;

							//Updates the resistance value to the renewed value
							HeadComboRes = ComboResRenewValue;

							//Makes a call to anim instance so that the animation will be played out later
							CharacterAnimationReference->SetupForBrokenBodyPartAnimation(TargetBodyPart, HeadStatus);

							return;

						}

						//If the resistance was not enough, nothing happens. So returns
						return;

					}
				}
			}
		break;

	case EBodyPartTarget::EBPT_Chest:
		//First, update the Resistance damage
		ChestComboRes -= DamageToResistance;

		//if it's broken already, there's no need to do anything
		if (ChestStatus != EBodyPartStatus::EBPS_Broken)
		{
			if (DamageToBodyPart > ChestBreakDmgThreshold)
			{
				//This case means the damage was so high that it broke the body part
				ChestStatus = EBodyPartStatus::EBPS_Broken;

				//Makes a call to anim instance so that the animation will be played out later
				CharacterAnimationReference->SetupForBrokenBodyPartAnimation(TargetBodyPart, ChestStatus);

				return;
			}

			if (ChestStatus == EBodyPartStatus::EBPS_Damaged)
			{
				//In this case, we need to check if the damage to the resistance was enough to break the body part
				if (ChestComboRes <= 0)
				{
					//This case means the body part was broken by a lack of resistance resistance
					ChestStatus = EBodyPartStatus::EBPS_Broken;

					//Updates the resistance value to the renewed value
					ChestComboRes = ComboResRenewValue;

					//Makes a call to anim instance so that the animation will be played out later
					CharacterAnimationReference->SetupForBrokenBodyPartAnimation(TargetBodyPart, ChestStatus);

					return;

				}

				//if it was not broken, just return.
				return;

			}

			else
			{
				//This case is where the condition of the body part is normal so we need to check
				//if the body part was damaged by the damage taken or by a lack of resistance or if nothing happened

				if (DamageToBodyPart > ChestDmgThreshold)
				{
					//This means the damage was high enough to damage the body part, but not break it
					ChestStatus = EBodyPartStatus::EBPS_Damaged;

					//Updates the resistance value to the renewed value
					ChestComboRes = ComboResRenewValue;

					//Makes a call to anim instance so that the animation will be played out later
					CharacterAnimationReference->SetupForBrokenBodyPartAnimation(TargetBodyPart, ChestStatus);

					return;
				}

				else
				{
					//If the damage was not enough, check for the resistance

					if (ChestComboRes <= 0)
					{
						//This case means the body part was broken by a lack of resistance resistance
						ChestStatus = EBodyPartStatus::EBPS_Damaged;

						//Updates the resistance value to the renewed value
						ChestComboRes = ComboResRenewValue;

						//Makes a call to anim instance so that the animation will be played out later
						CharacterAnimationReference->SetupForBrokenBodyPartAnimation(TargetBodyPart, ChestStatus);

						return;

					}

					//If the resistance was not enough, nothing happens. So returns
					return;

				}
			}
		}
		
		break;

	case EBodyPartTarget::EBPT_LeftArm:

		//First, update the Resistance damage
		LeftArmComboRes -= DamageToResistance;

		//if it's broken already, there's no need to do anything
		if (LeftArmStatus != EBodyPartStatus::EBPS_Broken)
		{
			if (DamageToBodyPart > LeftArmBreakDmgThreshold)
			{
				//This case means the damage was so high that it broke the body part
				LeftArmStatus = EBodyPartStatus::EBPS_Broken;

				//Makes a call to anim instance so that the animation will be played out later
				CharacterAnimationReference->SetupForBrokenBodyPartAnimation(TargetBodyPart, LeftArmStatus);

				return;
			}

			if (LeftArmStatus == EBodyPartStatus::EBPS_Damaged)
			{
				//In this case, we need to check if the damage to the resistance was enough to break the body part
				if (LeftArmComboRes <= 0)
				{
					//This case means the body part was broken by a lack of resistance resistance
					LeftArmStatus = EBodyPartStatus::EBPS_Broken;

					//Updates the resistance value to the renewed value
					LeftArmComboRes = ComboResRenewValue;

					//Makes a call to anim instance so that the animation will be played out later
					CharacterAnimationReference->SetupForBrokenBodyPartAnimation(TargetBodyPart, LeftArmStatus);

					return;

				}

				//if it was not broken, just return.
				return;

			}

			else
			{
				//This case is where the condition of the body part is normal so we need to check
				//if the body part was damaged by the damage taken or by a lack of resistance or if nothing happened

				if (DamageToBodyPart > LeftArmDmgThreshold)
				{
					//This means the damage was high enough to damage the body part, but not break it
					LeftArmStatus = EBodyPartStatus::EBPS_Damaged;

					//Updates the resistance value to the renewed value
					LeftArmComboRes = ComboResRenewValue;

					//Makes a call to anim instance so that the animation will be played out later
					CharacterAnimationReference->SetupForBrokenBodyPartAnimation(TargetBodyPart, LeftArmStatus);

					return;
				}

				else
				{
					//If the damage was not enough, check for the resistance

					if (LeftArmComboRes <= 0)
					{
						//This case means the body part was broken by a lack of resistance resistance
						LeftArmStatus = EBodyPartStatus::EBPS_Damaged;

						//Updates the resistance value to the renewed value
						LeftArmComboRes = ComboResRenewValue;

						//Makes a call to anim instance so that the animation will be played out later
						CharacterAnimationReference->SetupForBrokenBodyPartAnimation(TargetBodyPart, LeftArmStatus);

						return;

					}

					//If the resistance was not enough, nothing happens. So returns
					return;

				}
			}
		}
		
		break;

	case EBodyPartTarget::EBPT_RightArm:

		//First, update the Resistance damage
		RightArmComboRes -= DamageToResistance;

		//if it's broken already, there's no need to do anything
		if (RightArmStatus != EBodyPartStatus::EBPS_Broken)
		{
			if (DamageToBodyPart > RightArmBreakDmgThreshold)
			{
				//This case means the damage was so high that it broke the body part
				RightArmStatus = EBodyPartStatus::EBPS_Broken;

				//Makes a call to anim instance so that the animation will be played out later
				CharacterAnimationReference->SetupForBrokenBodyPartAnimation(TargetBodyPart, RightArmStatus);

				return;
			}

			if (RightArmStatus == EBodyPartStatus::EBPS_Damaged)
			{
				//In this case, we need to check if the damage to the resistance was enough to break the body part
				if (RightArmComboRes <= 0)
				{
					//This case means the body part was broken by a lack of resistance resistance
					RightArmStatus = EBodyPartStatus::EBPS_Broken;

					//Updates the resistance value to the renewed value
					RightArmComboRes = ComboResRenewValue;

					//Makes a call to anim instance so that the animation will be played out later
					CharacterAnimationReference->SetupForBrokenBodyPartAnimation(TargetBodyPart, RightArmStatus);

					return;

				}

				//if it was not broken, just return.
				return;

			}

			else
			{
				//This case is where the condition of the body part is normal so we need to check
				//if the body part was damaged by the damage taken or by a lack of resistance or if nothing happened

				if (DamageToBodyPart > RightArmDmgThreshold)
				{
					//This means the damage was high enough to damage the body part, but not break it
					RightArmStatus = EBodyPartStatus::EBPS_Damaged;

					//Updates the resistance value to the renewed value
					RightArmComboRes = ComboResRenewValue;

					//Makes a call to anim instance so that the animation will be played out later
					CharacterAnimationReference->SetupForBrokenBodyPartAnimation(TargetBodyPart, RightArmStatus);

					return;
				}

				else
				{
					//If the damage was not enough, check for the resistance

					if (RightArmComboRes <= 0)
					{
						//This case means the body part was broken by a lack of resistance resistance
						RightArmStatus = EBodyPartStatus::EBPS_Damaged;

						//Updates the resistance value to the renewed value
						RightArmComboRes = ComboResRenewValue;

						//Makes a call to anim instance so that the animation will be played out later
						CharacterAnimationReference->SetupForBrokenBodyPartAnimation(TargetBodyPart, RightArmStatus);

						return;

					}

					//If the resistance was not enough, nothing happens. So returns
					return;

				}
			}
		}

		
		break;

	case EBodyPartTarget::EBPT_Legs:

		//First, update the Resistance damage
		LegsComboRes -= DamageToResistance;

		//if it's broken already, there's no need to do anything
		if (LegsStatus != EBodyPartStatus::EBPS_Broken)
		{
			if (DamageToBodyPart > LegsBreakDmgThreshold)
			{
				//This case means the damage was so high that it broke the body part
				LegsStatus = EBodyPartStatus::EBPS_Broken;

				//Makes a call to anim instance so that the animation will be played out later
				CharacterAnimationReference->SetupForBrokenBodyPartAnimation(TargetBodyPart, LegsStatus);

				return;
			}

			if (LegsStatus == EBodyPartStatus::EBPS_Damaged)
			{
				//In this case, we need to check if the damage to the resistance was enough to break the body part
				if (LegsComboRes <= 0)
				{
					//This case means the body part was broken by a lack of resistance resistance
					LegsStatus = EBodyPartStatus::EBPS_Broken;

					//Updates the resistance value to the renewed value
					LegsComboRes = ComboResRenewValue;

					//Makes a call to anim instance so that the animation will be played out later
					CharacterAnimationReference->SetupForBrokenBodyPartAnimation(TargetBodyPart, LegsStatus);

					return;

				}

				//if it was not broken, just return.
				return;

			}

			else
			{
				//This case is where the condition of the body part is normal so we need to check
				//if the body part was damaged by the damage taken or by a lack of resistance or if nothing happened

				if (DamageToBodyPart > LegsDmgThreshold)
				{
					//This means the damage was high enough to damage the body part, but not break it
					LegsStatus = EBodyPartStatus::EBPS_Damaged;

					//Updates the resistance value to the renewed value
					LegsComboRes = ComboResRenewValue;

					//Makes a call to anim instance so that the animation will be played out later
					CharacterAnimationReference->SetupForBrokenBodyPartAnimation(TargetBodyPart, LegsStatus);

					return;
				}

				else
				{
					//If the damage was not enough, check for the resistance

					if (LegsComboRes <= 0)
					{
						//This case means the body part was broken by a lack of resistance resistance
						LegsStatus = EBodyPartStatus::EBPS_Damaged;

						//Updates the resistance value to the renewed value
						LegsComboRes = ComboResRenewValue;

						//Makes a call to anim instance so that the animation will be played out later
						CharacterAnimationReference->SetupForBrokenBodyPartAnimation(TargetBodyPart, LegsStatus);

						return;

					}

					//If the resistance was not enough, nothing happens. So returns
					return;

				}
			}
		}
		
		break;

	default:
		break;

	}


}

float ABaseCharacter::GetBodyPartStatusDamageMultiplier(EBodyPartTarget TargetBodyPart)
{
	switch (TargetBodyPart)
	{
		case EBodyPartTarget::EBPT_Head:
			if (HeadStatus == EBodyPartStatus::EBPS_Normal)	return NormalBodyPartDamageMultiplier;
			if (HeadStatus == EBodyPartStatus::EBPS_Damaged) return DamagedBodyPartDamageMultiplier;
			if (HeadStatus == EBodyPartStatus::EBPS_Broken) return BrokenBodyPartDamageMultiplier;
			break;

		case EBodyPartTarget::EBPT_Chest:
			if (ChestStatus == EBodyPartStatus::EBPS_Normal) return NormalBodyPartDamageMultiplier;
			if (ChestStatus == EBodyPartStatus::EBPS_Damaged) return DamagedBodyPartDamageMultiplier;
			if (ChestStatus == EBodyPartStatus::EBPS_Broken) return BrokenBodyPartDamageMultiplier;
			break;

		case EBodyPartTarget::EBPT_RightArm:
			if (RightArmStatus == EBodyPartStatus::EBPS_Normal)	return NormalBodyPartDamageMultiplier;
			if (RightArmStatus == EBodyPartStatus::EBPS_Damaged) return DamagedBodyPartDamageMultiplier;
			if (RightArmStatus == EBodyPartStatus::EBPS_Broken) return BrokenBodyPartDamageMultiplier;
			break;

		case EBodyPartTarget::EBPT_LeftArm:
			if (LeftArmStatus == EBodyPartStatus::EBPS_Normal)	return NormalBodyPartDamageMultiplier;
			if (LeftArmStatus == EBodyPartStatus::EBPS_Damaged) return DamagedBodyPartDamageMultiplier;
			if (LeftArmStatus == EBodyPartStatus::EBPS_Broken) return BrokenBodyPartDamageMultiplier;
			break;

		case EBodyPartTarget::EBPT_Legs:
			if (LegsStatus == EBodyPartStatus::EBPS_Normal)	return NormalBodyPartDamageMultiplier;
			if (LegsStatus == EBodyPartStatus::EBPS_Damaged) return DamagedBodyPartDamageMultiplier;
			if (LegsStatus == EBodyPartStatus::EBPS_Broken) return BrokenBodyPartDamageMultiplier;
			break;

		default:
			return NormalBodyPartDamageMultiplier;
			break;

	}

	return 1;
}


bool ABaseCharacter::CanCharacterExecuteAction(ECharacterAction ActionToBeTaken)
{
	if (CharacterMainStatus == ECharacterStatus::ECS_Defeated || CharacterMainLimitation == ECharacterLimitation::ECL_All || CurrentHealth <= 0)
	{
		return false;
	}

	switch (ActionToBeTaken)
	{
		case ECharacterAction::ECA_Attack:

			if (CharacterMainLimitation != ECharacterLimitation::ECL_NoAttacks)
			{
				return true;
			}

			else
			{
				return false;
			}

			break;

		case ECharacterAction::ECA_Special:

			if (CharacterMainLimitation != ECharacterLimitation::ECL_NoSpecial)
			{
				return true;
			}

			else
			{
				return false;
			}
			break;

		case ECharacterAction::ECA_Stance:
			if (CharacterMainLimitation != ECharacterLimitation::ECL_NoStanceChange)
			{
				return true;
			}

			else
			{
				return false;
			}
			break;

		case ECharacterAction::ECA_Item:
			return false;
			break;

		default:
			return true;
			break;
	}

}

void ABaseCharacter::ActivateCounter(ABaseCharacter* CharacterToBeCountered, EBodyPartTarget TargetBodyPart)
{
	//bWasActiveSkillCountered is updated in Receive Counter
	//CharacterToBeCountered->bWasActiveSkillCountered = true;
	
	if (CanCharacterExecuteAction(ECharacterAction::ECA_Counter))
	{
		BattleSystemReference->WidgetCounterCall();
		float CounterDamage = CalculateCounterDamage(CharacterToBeCountered->DamageCaused);
		UE_LOG(LogTemp, Warning, TEXT("Final Counter Damage: %f"), CounterDamage);
		FString CounterDamageString = FString::FromInt(CounterDamage);
		//BattleSystemReference->BattleLogReport(Name + " used counter and caused " + CounterDamageString + " damage");
		BattleSystemReference->BattleLogReport(Name + " used counter");
		CharacterToBeCountered->ReceiveCounter(CounterDamage, TargetBodyPart);
		CharacterAnimationReference->SetupAndPlayAnimationMontage(EBattleAnimationSetup::EBAS_Counter);
	}

	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("Activate Counter - Counter could not be performed"));
	}
	
}


void ABaseCharacter::ActivateCover(ABaseCharacter* CharacterThatWillTargetCover)
{
	UE_LOG(LogTemp, Warning, TEXT("====== Base Character - Activate Cover ========"));

	CharacterThatWillTargetCover->SingleTargetReference = this;
	//play cover animation - movement is to implement
	//CharacterAnimationReference->bCharacterCover = true;
	BattleSystemReference->BattleLogReport(Name + " covers his allies.");
	CharacterAnimationReference->PlayRegularAnimation(EBattleAnimationSetup::EBAS_Cover);
	
}

void ABaseCharacter::SetupAICustomAction_Implementation()
{

}

void ABaseCharacter::SetupAIAction()
{
	UE_LOG(LogTemp, Warning, TEXT("====== Base Character - Decide AI Action ========"));

	//First we need to determine how much SPD the current character has and can use
	int NumberOfCommands = FinalSPD/10;
	float RandomNumber = 0;
	float RateToAdjust = 0;
	float LightAttackPercenteageAdjusted = LightAttackPercenteage;
	float MediumAttackPercenteageAdjusted = MediumAttackPercenteage;
	float StancePercenteageAdjusted = StancePercenteage;

	UE_LOG(LogTemp, Warning, TEXT("%s has F.SPD: %d"), *Name, FinalSPD);

	if (FinalSPD < LightStrategySPDThreshold)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough SPD for Light Action. Change Stance"));
		//If Final SPD is less than LightStrategySPDThreshold, then we should just change stances
		SetupAIChangeStance();
		return;
	}

	if (FinalSPD < MediumStrategySPDCost)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough SPD for Medium Action. Stance or Light"));
		//If Final SPD is less than MediumStrategySPDThreshold, then we should just choose between stance or light attack
		//Adjust rates
		//RateToAdjust = 1 - LightAttackPercenteage - StancePercenteage;
		RateToAdjust = (MediumAttackPercenteage + HeavyAttackPercenteage);
		LightAttackPercenteageAdjusted = LightAttackPercenteage + (RateToAdjust / 2);
		StancePercenteageAdjusted = StancePercenteage + (RateToAdjust / 2);

		RandomNumber = FMath::FRandRange(0, 1);

		UE_LOG(LogTemp, Warning, TEXT("Stance%: %f"), StancePercenteageAdjusted);
		UE_LOG(LogTemp, Warning, TEXT("Light%: %f"), LightAttackPercenteageAdjusted);
		UE_LOG(LogTemp, Warning, TEXT("RandomNumber: %f"), RandomNumber);

		if (RandomNumber <= StancePercenteageAdjusted)
		{
			UE_LOG(LogTemp, Warning, TEXT("Stance Chosen"));
			SetupAIChangeStance();
		}

		else
		{
			//Light Combo attack setup
			UE_LOG(LogTemp, Warning, TEXT("Light Attack Chosen"));
			SetupAIComboAttack(EAIStrategy::EAIS_Light);
		}

		return;
	}

	if (FinalSPD < HeavyStrategySPDCost)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough SPD for Heavy Action. Stance/Light/Medium"));
		//If Final SPD is less than HeavyStrategySPDThreshold, then we should just choose between stance, light attack or medium
		//Adjust rates
		RateToAdjust = (HeavyAttackPercenteage/3);
		LightAttackPercenteageAdjusted += RateToAdjust;
		MediumAttackPercenteageAdjusted += RateToAdjust;
		StancePercenteageAdjusted += RateToAdjust;

		RandomNumber = FMath::FRandRange(0, 1);

		UE_LOG(LogTemp, Warning, TEXT("Stance%: %f"), StancePercenteageAdjusted);
		UE_LOG(LogTemp, Warning, TEXT("Light%: %f"), LightAttackPercenteageAdjusted);
		UE_LOG(LogTemp, Warning, TEXT("Medium%: %f"), MediumAttackPercenteageAdjusted);
		UE_LOG(LogTemp, Warning, TEXT("RandomNumber: %f"), RandomNumber);

		if (RandomNumber <= StancePercenteageAdjusted)
		{
			UE_LOG(LogTemp, Warning, TEXT("Stance Chosen"));
			SetupAIChangeStance();
		}

		else 
		{
			RandomNumber -= StancePercenteageAdjusted;

			if (RandomNumber <= LightAttackPercenteageAdjusted)
			{
				//Light Combo attack setup
				UE_LOG(LogTemp, Warning, TEXT("Light Attack Chosen"));
				SetupAIComboAttack(EAIStrategy::EAIS_Light);
			}

			else 
			{
				//Medium Combo attack setup
				UE_LOG(LogTemp, Warning, TEXT("Medium Attack Chosen"));
				SetupAIComboAttack(EAIStrategy::EAIS_Medium);
			}

		}

		return;

	}


	if (FinalSPD < VeryHeavyStrategySPDCost)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough SPD for Very Heavy Action. Stance/Light/Medium/Heavy"));
		//Stance or light/medium/heavy Attack will occur
		//no need to adjust rates

		RandomNumber = FMath::FRandRange(0, 1);

		UE_LOG(LogTemp, Warning, TEXT("Not ajusted values."));
		UE_LOG(LogTemp, Warning, TEXT("Stance%: %f"), StancePercenteage);
		UE_LOG(LogTemp, Warning, TEXT("Light%: %f"), LightAttackPercenteage);
		UE_LOG(LogTemp, Warning, TEXT("Medium%: %f"), MediumAttackPercenteage);
		UE_LOG(LogTemp, Warning, TEXT("Heavy%: %f"), HeavyAttackPercenteage);
		UE_LOG(LogTemp, Warning, TEXT("RandomNumber: %f"), RandomNumber);

		if (RandomNumber <= StancePercenteage)
		{
			UE_LOG(LogTemp, Warning, TEXT("Stance Chosen"));
			SetupAIChangeStance();
		}

		else
		{
			RandomNumber -= StancePercenteage;

			if (RandomNumber <= LightAttackPercenteage)
			{
				//Light Combo attack setup
				UE_LOG(LogTemp, Warning, TEXT("Light Attack Chosen"));
				SetupAIComboAttack(EAIStrategy::EAIS_Light);
			}

			else
			{
				RandomNumber -= LightAttackPercenteage;
				
				if (RandomNumber <= MediumAttackPercenteage)
				{
					//Medium Combo attack setup
					UE_LOG(LogTemp, Warning, TEXT("Medium Attack Chosen"));
					SetupAIComboAttack(EAIStrategy::EAIS_Medium);
				}
				
				else
				{
					//Heavy Combo attack setup
					UE_LOG(LogTemp, Warning, TEXT("Heavy Attack Chosen"));
					SetupAIComboAttack(EAIStrategy::EAIS_Heavy);
				}

			}

		}

		return;
		
	}

	if (FinalSPD >= VeryHeavyStrategySPDCost)
	{
		UE_LOG(LogTemp, Warning, TEXT("Final SPD above or equal Very Heavy Strategy SPD Cost"));
		//Heavy Attack will occur
		UE_LOG(LogTemp, Warning, TEXT("Very Heavy Attack Chosen"));
		SetupAIComboAttack(EAIStrategy::EAIS_VeryHeavy);
	}


}

void ABaseCharacter::SetupAIChangeStance()
{
	//Stance change was sorted
	//Get AllKeys from StanceMap

	TArray<FString> StanceNames;
	int NumberOfStances;
	NumberOfStances = StanceMap.GetKeys(StanceNames);

	//int RandomNumber = FMath::RandHelper(StanceNames.Num());
	int RandomNumber = FMath::RandRange(0, StanceNames.Num()-1);


	FName StanceToChange(StanceNames[RandomNumber]);
	UE_LOG(LogTemp, Warning, TEXT(" Base Character - AI Setup - Stance: %s "), *StanceToChange.ToString());
	AIActionReport(EAIStrategy::EAIS_Stance);
	SetupStanceChange(StanceToChange);

}

void ABaseCharacter::SetupAIComboAttack(EAIStrategy Choice)
{
	//Get AllRowNames from the Data Table
	TArray<FName> RowNames;
	TArray<EAttackType> AICommands;
	TArray<EAttackType> SkillCommands;

	int SPDToUse = FinalSPD;
	int SkillSPDCost;
	int NumberOfSkillsToRandomlyChoose;
	int RandomSkillChoice;
	int MinSPDCost = 30;
	int MediumOrHeavySPDCost;
	int IndexToChange = 0;
	int NumberOfSkillsUsed = 0;

	
	bool MidInputUsed = false;
	bool RandomSkillsSelection = true;

	FSkill* SkillToSearch;
	static const FString ContextString(TEXT("ContextString"));

	RowNames = ActiveSkillList->SkillTable->GetRowNames();
	AICommands.Empty();
	NumberOfSkillsUsed = 0;

	UE_LOG(LogTemp, Warning, TEXT("==========SETUP AI Combo=========="));

	AIActionReport(Choice);

	switch (Choice)
	{
		case EAIStrategy::EAIS_Light:
			//Light attack choice
			UE_LOG(LogTemp, Warning, TEXT("========== Light Combo Setup =========="));
			for (int i = 0; i < RowNames.Num(); i++)
			{
				SkillToSearch = ActiveSkillList->SkillTable->FindRow<FSkill>(RowNames[i], ContextString);
				SkillCommands.Empty();
				SkillCommands = ActiveSkillList->SkillCommandsToArray(SkillToSearch);
			
				SkillSPDCost = SkillToSearch->Cost;
				
				if (MidInputUsed)
				{
					//If the mid input was used, the cost to add the skill will be 10 less than normal
					SkillSPDCost = SkillSPDCost - 10;
				}

				if (SPDToUse >= SkillSPDCost)
				{
					//We have enough SPD to add the current Skill

					//Adjust SPDToUse
					SPDToUse -= SkillSPDCost;

					//If the mid input was not used, we need to append the SkillCommand to the AICommand TArray and change the last input to mid
					//If the SkillCommand first input and the AICommand last input are equal, there will be no need to add the first input of SkillCommand
					//By last, once SkillCommand is added, the last input will be changed to mid
					if (!MidInputUsed)
					{
						
						//First we need to check if AICommands is empty. If a comparison occurs while empty, it crashes
						if (AICommands.Num() > 0)
						{
							//We need to check if the last input of AICommands is the same as SkillCommands

							if (AICommands[AICommands.Num() - 1] == SkillCommands[0])
							{
								//If the last input in AICommands and the first input in SkillCommands are the same, we remove the first in SkillCommands
								SkillCommands.RemoveAt(0);
							}

							//if they are different, we just need to append the skill commands array normally

						}

						//It won't crash even if it's the first skill being inputted
						//First we append the skill commands
						AICommands.Append(SkillCommands);
						NumberOfSkillsUsed++;

						UE_LOG(LogTemp, Warning, TEXT("Light Combo Setup - Skills Used: %d"), NumberOfSkillsUsed);

						//Then we change the last input to mid
						AICommands[AICommands.Num() - 1] = EAttackType::EAT_Mid;
						MidInputUsed = true;

						if (NumberOfSkillsUsed > LightStrategyAttackNumberThreshold)
						{
							//We already used the maximum number of skills
							break;
						}

					}

					//If the mid input was used, we need to remove the first input of skillcommands
					//and then add it to the AICommands TArray
					else 
					{
						//Remove first element of skill array
						SkillCommands.RemoveAt(0);
						AICommands.Append(SkillCommands);
						MidInputUsed = false;
						NumberOfSkillsUsed++;

						if (NumberOfSkillsUsed > LightStrategyAttackNumberThreshold)
						{
							//We already used the maximum number of skills
							UE_LOG(LogTemp, Warning, TEXT("Light Combo Setup - Max Skill Number Used"));
							break;
						}
					}
				}

				else 
				{
					//We do not have enough SPD
					//Since the datasheet is in a crescent order of SPD, we can stop the loop here once it doesn't have anymore SPD
					break;
				}

			}

			//Special Enum name UELOG
			for (int i = 0; i < AICommands.Num(); i++)
			{
				FString FinalCommandsAI = StaticEnum<EAttackType>()->GetNameByIndex(static_cast<int32>(AICommands[i])).ToString();
				UE_LOG(LogTemp, Warning, TEXT("AI - Command:%d - %s"), i, *FinalCommandsAI);
			}

			//SortRandomTarget will give a proper reference
			BattleSystemReference->SortRandomTarget(this);
			SetupAttackCommands(AICommands, SingleTargetReference);

			break;

		case EAIStrategy::EAIS_Medium:
		case EAIStrategy::EAIS_Heavy:
			//Medium attack choice
			//Heavy attack choice

			if (Choice == EAIStrategy::EAIS_Medium)
			{
				UE_LOG(LogTemp, Warning, TEXT("========== Medium Combo Setup =========="));
				MediumOrHeavySPDCost = MediumStrategySPDCost;
			}

			else 
			{
				UE_LOG(LogTemp, Warning, TEXT("========== Heavy Combo Setup =========="));
				MediumOrHeavySPDCost = HeavyStrategySPDCost;
			}

			//First skill should have the appropriate strategy spd cost, at least

			for (int i = 0; i < RowNames.Num(); i++)
			{
				SkillToSearch = ActiveSkillList->SkillTable->FindRow<FSkill>(RowNames[i], ContextString);

				if (MediumOrHeavySPDCost <= SkillToSearch->Cost)
				{
					//We found an appropriate skill to start
					SkillCommands.Empty();
					SkillCommands = ActiveSkillList->SkillCommandsToArray(SkillToSearch);

					if (SPDToUse >= SkillToSearch->Cost)
					{
						UE_LOG(LogTemp, Warning, TEXT(" Base Character - AI Setup - MidHeavyStrategy - First Skill"));
						//We have enough SPD to execute
						SPDToUse -= SkillToSearch->Cost;
						
						//We add the first skill to the AICommands TArray
						AICommands.Append(SkillCommands);

						//Add to the number of skills used as medium strategy can have a limitation
						NumberOfSkillsUsed++;

						UE_LOG(LogTemp, Warning, TEXT("Medium Combo Setup - Skills Used: %d"), NumberOfSkillsUsed);

						//And change the last input to mid
						AICommands[AICommands.Num() - 1] = EAttackType::EAT_Mid;
						MidInputUsed = true;

						//First skill found, no need to continue loop
						break;
					}

					else 
					{
						//Should not happpen due to SPD checks before, but as a precaution, change stance
						UE_LOG(LogTemp, Warning, TEXT(" Base Character - AI Setup - MidHeavyStrategy SPD Error. ChangeStance"));
						SetupAIChangeStance();
						return;
					}

				}

				if ((AICommands.Num() == 0) && (i == RowNames.Num() - 1))
				{
					//If we searched all skills and couldn't find a good one to start
					UE_LOG(LogTemp, Warning, TEXT(" Base Character - AI Setup - MidHeavyStrategy No Skill. ChangeStance"));
					SetupAIChangeStance();
					return;
				}
			}

			//After first skill was found, we need to fill the AICommand array with other skills selected at random

			//Define min SPD for skills
			SkillToSearch = ActiveSkillList->SkillTable->FindRow<FSkill>(RowNames[0], ContextString);
			MinSPDCost = SkillToSearch->Cost;

			//Use the remaining SPD to randomly sort skills
			NumberOfSkillsToRandomlyChoose = RowNames.Num();

			//From here on, we are putting random skills on the AICommand TArray
			do 
			{	
				//Choose a random skill
				RandomSkillChoice = FMath::RandRange(0, NumberOfSkillsToRandomlyChoose-1);

				SkillToSearch = ActiveSkillList->SkillTable->FindRow<FSkill>(RowNames[RandomSkillChoice], ContextString);
				SkillCommands.Empty();
				SkillCommands = ActiveSkillList->SkillCommandsToArray(SkillToSearch);

				//Check the cost of inputting the skill
				SkillSPDCost = SkillToSearch->Cost;

				if (MidInputUsed)
				{
					//If the mid input was used, the cost to add the skill will be 10 less than normal
					SkillSPDCost = SkillSPDCost - 10;
					//And we can remove the first input from the SkillCommands TArray
					SkillCommands.RemoveAt(0);

					if (SPDToUse >= SkillSPDCost)
					{
						//Adjust SPDToUse

						SPDToUse -= SkillSPDCost;

						//if we have enough SPD to add the skill
						AICommands.Append(SkillCommands);
						MidInputUsed = false;
						NumberOfSkillsUsed++;

						UE_LOG(LogTemp, Warning, TEXT("Medium Combo Setup - Skills Used: %d"), NumberOfSkillsUsed);

						if (Choice == EAIStrategy::EAIS_Medium)
						{
							if (NumberOfSkillsUsed > MediumStrategyAttackNumberThreshold)
							{
								//We added all skills we could while in the medium strategy
								UE_LOG(LogTemp, Warning, TEXT("Medium Combo Setup - Max Skill Number Used"));
								RandomSkillsSelection = false;
								break;
							}
						}
						
					}

					else 
					{
						//Not enough SPD to add skill
						//We need to decide if it's possible or not to continue adding skills
						if (SPDToUse < (MinSPDCost - 10))
						{
							//There are no more skills that can be added
							RandomSkillsSelection = false;
							break;
						}

						else 
						{
							//We can still add skills, thus we need to search for skills that have SPD cost below the current one
							//No need to decrease 1 because it is already done when making the random
							NumberOfSkillsToRandomlyChoose = RandomSkillChoice;
						}

					}

				}

				/*In this case, the mid input was not used.
				*First we need to check if the last input in AICommands and the first input in SkillCommands TArray are the same
				*If they are the same, the cost of the skill will be reduced by 10 and we will remove the first input from SkillCommands
				*Then check if there's enough SPD to use the skill
				*Then append the SkillCommands TArray to AICommands 
				*Adjust the SPDToUse and NumberOfSkills used
				*And, by last, change the last input of AICommands to mid
				*/
				else 
				{
					if (AICommands[AICommands.Num() - 1] == SkillCommands[0])
					{
						//If the last input of AICommands and the first of SkillCommands are the same, no need to add the first and, thus, we decrease the cost
						SkillCommands.RemoveAt(0);
						SkillSPDCost -= 10;
					}

					if (SPDToUse >= SkillSPDCost)
					{
						//If we have enough SPD to add the skill

						//then append skillcommands to aicommands
						AICommands.Append(SkillCommands);

						//Adjust SPD
						SPDToUse -= SkillSPDCost;

						//Change last input to mid
						AICommands[AICommands.Num() - 1] = EAttackType::EAT_Mid;

						MidInputUsed = true;
						NumberOfSkillsUsed++;

						if (Choice == EAIStrategy::EAIS_Medium)
						{
							if (NumberOfSkillsUsed > MediumStrategyAttackNumberThreshold)
							{
								//We added all skills we could while in the medium strategy
								RandomSkillsSelection = false;
								break;
							}
						}

					}


					else
					{
						//Not enough SPD to add skill
						//We do not have enough SPD to add the current skill
						//Thus all skills with higher cost won't be able to be added
						NumberOfSkillsToRandomlyChoose = RandomSkillChoice;

						if (SPDToUse < (MinSPDCost-10))
						{
							//There is a chance that a lower SPD cost skill can be used if their first input is the same as the last in the current AICommand TArray
							//There are no more skills that can be added
							RandomSkillsSelection = false;
							break;
						}

					}



				}

			} while (RandomSkillsSelection);

			//Special Enum name UELOG
			for (int i = 0; i < AICommands.Num(); i++)
			{
				FString FinalCommandsAI = StaticEnum<EAttackType>()->GetNameByIndex(static_cast<int32>(AICommands[i])).ToString();
				UE_LOG(LogTemp, Warning, TEXT("AI - Command:%d - %s"), i, *FinalCommandsAI);
			}

			//SortRandomTarget will give a proper reference
			BattleSystemReference->SortRandomTarget(this);
			SetupAttackCommands(AICommands, SingleTargetReference);
			break;

		case EAIStrategy::EAIS_VeryHeavy:
			//Very heavy attack choice. Will add as many attacks as possible starting from higher cost
			UE_LOG(LogTemp, Warning, TEXT("========== Very Heavy Combo Setup =========="));

			for (int i = RowNames.Num()-1; i >= 0; i--)
			{
				SkillToSearch = ActiveSkillList->SkillTable->FindRow<FSkill>(RowNames[i], ContextString);
				SkillCommands.Empty();
				SkillCommands = ActiveSkillList->SkillCommandsToArray(SkillToSearch);

				SkillSPDCost = SkillToSearch->Cost;

				if (MidInputUsed)
				{
					//If the mid input was used, the cost to add the skill will be 10 less than normal
					SkillSPDCost = SkillSPDCost - 10;

					//And we can remove the first input from the SkillCommands TArray
					SkillCommands.RemoveAt(0);

					if (SPDToUse >= SkillSPDCost)
					{
						//We have enough SPD to add the current Skill

						//Adjust SPDToUse
						SPDToUse -= SkillSPDCost;

						AICommands.Append(SkillCommands);
						MidInputUsed = false;

					}

					//Regardless if we do not have enough SPD to add the skill, we need to determine if we continue the loop.
					
					if (SPDToUse < (MinSPDCost-10))
					{
						//The next skill can have the same initial input as the current last one, thus decreasing the cost by 10
						//If there's no more SPD, break from loop
						break;
					}

				}

				/*In this case, the last input of AICommands TArray is not a mid
				* Thus we need to check if the last input of AICommands and the first input of SkillCommands match
				* If they do, decrease the cost of skill by 10 and remove first input of SkillCommands
				* The append array, decrease SPDToUse and change last input to mid
				*/

				else 
				{
					//If this is the first input, the next if will lead to a crash
					if (AICommands.Num() > 0)
					{
						if (AICommands[AICommands.Num() - 1] == SkillCommands[0])
						{
							//If the last command and the first command match case
							SkillCommands.RemoveAt(0);
							SkillSPDCost -= 10;
						}
					}

					if (SPDToUse >= SkillSPDCost)
					{
						//We have enough SPD to add the current Skill
						//Adjust SPDToUse
						SPDToUse -= SkillSPDCost;

						AICommands.Append(SkillCommands);
						AICommands[AICommands.Num() - 1] = EAttackType::EAT_Mid;
						MidInputUsed = true;

					}

					//Regardless if we do not have enough SPD to add the skill, we need to determine if we continue the loop.

					if (SPDToUse < (MinSPDCost - 10))
					{
						//The next skill can have the same initial input as the current last one, thus decreasing the cost by 10
						//If there's no more SPD, break from loop
						break;
					}

					
				}



			}

			//Special Enum name UELOG
			for (int i = 0; i < AICommands.Num(); i++)
			{
				FString FinalCommandsAI = StaticEnum<EAttackType>()->GetNameByIndex(static_cast<int32>(AICommands[i])).ToString();
				UE_LOG(LogTemp, Warning, TEXT("AI - Command:%d - %s"), i, *FinalCommandsAI);
			}

			//SortRandomTarget will give a proper reference
			BattleSystemReference->SortRandomTarget(this);
			SetupAttackCommands(AICommands, SingleTargetReference);
			break;

		default:
			//Should not happpen, but as a precaution, change stance
			UE_LOG(LogTemp, Warning, TEXT(" Base Character - AI Setup - Choice Error. ChangeStance"));
			SetupAIChangeStance();
			break;
	}

}

void ABaseCharacter::AIActionReport(EAIStrategy ActionTaken)
{
	if (BattleSystemReference == nullptr)
	{
		//Do nothing at this point
		UE_LOG(LogTemp, Warning, TEXT(" Base Character - AI Action Report - BattleSysReference nullptr"));
		return;
	}

	if (MainPlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT(" Base Character - AI Action Report - MainPlayerController nullptr"));
		MainPlayerController = Cast<AMainPlayerController>(GetController());
	}

	FString TextToReport;

	switch (ActionTaken)
	{
		case EAIStrategy::EAIS_Stance:
			
				TextToReport = StancePreviewText.ToString();
				//BattleSystemReference->BattleLogReport(StancePreviewText.ToString());
			
				//TextToReport = StancePreviewTextJPN.ToString();
				//BattleSystemReference->BattleLogReport(StancePreviewTextJPN.ToString());
			
			break;
		case EAIStrategy::EAIS_Light:
			
				TextToReport = LightAttackPreviewText.ToString();
				//BattleSystemReference->BattleLogReport(LightAttackPreviewText.ToString());
			
				//TextToReport = LightAttackPreviewTextJPN.ToString();
				//BattleSystemReference->BattleLogReport(LightAttackPreviewTextJPN.ToString());
			
			break;
		case EAIStrategy::EAIS_Medium:
			
				TextToReport = MediumAttackPreviewText.ToString();
				//BattleSystemReference->BattleLogReport(MediumAttackPreviewText.ToString());
			
				//TextToReport = MediumAttackPreviewTextJPN.ToString();
				//BattleSystemReference->BattleLogReport(MediumAttackPreviewTextJPN.ToString());
			
			break;
		case EAIStrategy::EAIS_Heavy:
			
				TextToReport = HeavyAttackPreviewText.ToString();
				//BattleSystemReference->BattleLogReport(HeavyAttackPreviewText.ToString());
			
				//TextToReport = HeavyAttackPreviewTextJPN.ToString();
				//BattleSystemReference->BattleLogReport(HeavyAttackPreviewTextJPN.ToString());
			
			break;
		case EAIStrategy::EAIS_VeryHeavy:
			
				TextToReport = VeryHeavyAttackPreviewText.ToString();
				//BattleSystemReference->BattleLogReport(VeryHeavyAttackPreviewText.ToString());
		
				//TextToReport = VeryHeavyAttackPreviewTextJPN.ToString();
				//BattleSystemReference->BattleLogReport(VeryHeavyAttackPreviewTextJPN.ToString());
			
			break;

		default:
			TextToReport = "default";
			break;
	}

	BattleSystemReference->BattleLogReport(TextToReport);

}

void ABaseCharacter::SetupCharacterStatus(int CharacterLevel)
{
	FCharacterLevelAndStatus* CharacterCurrentStatus;
	static const FString ContextString(TEXT("ContextString"));

	FString CharLvl = FString::FromInt(CharacterLevel);

	FName LevelToSearch = FName(*CharLvl);

	if (LevelTable)
	{
		CharacterCurrentStatus = LevelTable->FindRow<FCharacterLevelAndStatus>(LevelToSearch, ContextString);

		if (CharacterCurrentStatus == nullptr)
		{
			//Could not find row
			UE_LOG(LogTemp, Warning, TEXT(" Base Character - Setup Character Status - Could not find row - %s"), *CharLvl);
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT(" Base Character - Setup Character Status - Row Found -LVL: %s"), *CharLvl);

		Level = CharacterCurrentStatus->Level;
		MaxHealth = CharacterCurrentStatus->MaxHealth;
		CurrentHealth = CharacterCurrentStatus->CurrentHealth;
		OverHealthMultiplier = CharacterCurrentStatus->OverHealthMultiplier;

		if (CurrentHealth > MaxHealth)
		{
			CurrentHealth = MaxHealth;
		}

		MaxMagic = CharacterCurrentStatus->MaxMagic;
		CurrentMagic = CharacterCurrentStatus->CurrentMagic;
		MagicRegeneration = CharacterCurrentStatus->MagicRegeneration;
		SingleAttackMagicRegeneration = CharacterCurrentStatus->SingleAttackMagicRegeneration;

		if (CurrentMagic > MaxMagic)
		{
			CurrentMagic = MaxMagic;
		}

		Max_AP = CharacterCurrentStatus->Max_AP;
		CurrentAP = CharacterCurrentStatus->CurrentAP;
		AP_Regeneration = CharacterCurrentStatus->AP_Regeneration;

		if (CurrentAP > Max_AP)
		{
			CurrentAP = Max_AP;
		}

		BaseATK = CharacterCurrentStatus->BaseATK;
		BaseDEF = CharacterCurrentStatus->BaseDEF;
		BaseTEC = CharacterCurrentStatus->BaseTEC;
		BaseEVA = CharacterCurrentStatus->BaseEVA;
		BaseSPD = CharacterCurrentStatus->BaseSPD;
		BaseAGI = CharacterCurrentStatus->BaseAGI;
		SPD_Regeneration = CharacterCurrentStatus->SPD_Regeneration;


		RightArmDmgThreshold = CharacterCurrentStatus->RightArmDmgThreshold;
		LeftArmDmgThreshold = CharacterCurrentStatus->LeftArmDmgThreshold;
		ChestDmgThreshold = CharacterCurrentStatus->ChestDmgThreshold;
		LegsDmgThreshold = CharacterCurrentStatus->LegsDmgThreshold;
		HeadDmgThreshold = CharacterCurrentStatus->HeadDmgThreshold;

		RightArmBreakDmgThreshold = CharacterCurrentStatus->RightArmBreakDmgThreshold;
		LeftArmBreakDmgThreshold = CharacterCurrentStatus->LeftArmBreakDmgThreshold;
		ChestBreakDmgThreshold = CharacterCurrentStatus->ChestBreakDmgThreshold;
		LegsBreakDmgThreshold = CharacterCurrentStatus->LegsBreakDmgThreshold;
		HeadBreakDmgThreshold = CharacterCurrentStatus->HeadBreakDmgThreshold;

		NormalBodyPartDamageMultiplier = CharacterCurrentStatus->NormalBodyPartDamageMultiplier;
		DamagedBodyPartDamageMultiplier = CharacterCurrentStatus->DamagedBodyPartDamageMultiplier;
		BrokenBodyPartDamageMultiplier = CharacterCurrentStatus->BrokenBodyPartDamageMultiplier;

		ResistanceAttackValue = CharacterCurrentStatus->ResistanceAttackValue;
		RightArmComboRes = CharacterCurrentStatus->RightArmComboRes;
		LeftArmComboRes = CharacterCurrentStatus->LeftArmComboRes;
		ChestComboRes = CharacterCurrentStatus->ChestComboRes;
		LegsComboRes = CharacterCurrentStatus->LegsComboRes;
		HeadComboRes = CharacterCurrentStatus->HeadComboRes;
		ComboResRenewValue = CharacterCurrentStatus->ComboResRenewValue;

		InitialStance = CharacterCurrentStatus->InitialStance;

		if (StanceMap.Contains(InitialStance.ToString()))
		{
			ChangeStance(InitialStance);
		}
	}
	
}