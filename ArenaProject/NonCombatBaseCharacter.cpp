// Fill out your copyright notice in the Description page of Project Settings.


#include "NonCombatBaseCharacter.h"
#include "Components/SphereComponent.h"
#include "Enums.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/DataTable.h"

// Sets default values
ANonCombatBaseCharacter::ANonCombatBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ActivateCharacterCamera = false;

	BaseCharacterSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CharacterSpringArm"));
	BaseCharacterSpringArmComponent->SetupAttachment(GetRootComponent());
	BaseCharacterSpringArmComponent->TargetArmLength = 600; //Distance that camera will follow character
	BaseCharacterSpringArmComponent->bUsePawnControlRotation = true; //Rotate arm based on controller

	BaseCharacterCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CharacterCamera"));
	BaseCharacterCamera->SetupAttachment(BaseCharacterSpringArmComponent, USpringArmComponent::SocketName);
	BaseCharacterCamera->bUsePawnControlRotation = false;


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

	ConversationID = 0;
	LineID = 1;
	ActivateCharacterCamera = false;

}

// Called when the game starts or when spawned
void ANonCombatBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	//this->ToggleNonCombatBaseCharacterCamera();
	StartingPosition = this->GetActorLocation();

}

// Called every frame
void ANonCombatBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANonCombatBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Binds mouse control to camera
	PlayerInputComponent->BindAxis("CameraLookUp", this, &ANonCombatBaseCharacter::LookUp);
	PlayerInputComponent->BindAxis("CameraTurnRight", this, &ANonCombatBaseCharacter::Turn);

}


void ANonCombatBaseCharacter::Turn(float value)
{
	AddControllerYawInput(value);
}
void ANonCombatBaseCharacter::LookUp(float value)
{
	AddControllerPitchInput(value);
}

FDialogue* ANonCombatBaseCharacter::GetDialogue(int WorldContext, int LocalContext)
{
	if(DialogueTable)
	{
		FString DialogueRowNameString = FString::FromInt(WorldContext) + "." + FString::FromInt(LocalContext) + "." + FString::FromInt(ConversationID) + "." + FString::FromInt(LineID);
		FName DialogueRowName = FName(*DialogueRowNameString);

		static const FString DialogueContextString(TEXT("DialogueContextString"));
		FDialogue* FoundDialogue = DialogueTable->FindRow<FDialogue>(DialogueRowName, DialogueContextString);

		if (FoundDialogue == nullptr)
		{
			DialogueRowNameString = "ErrorDialogueRow";
			DialogueRowName = FName(*DialogueRowNameString);
			FoundDialogue = DialogueTable->FindRow<FDialogue>(DialogueRowName, DialogueContextString);
			/*FoundDialogue->TalkPersonName = "VoiceFromNowhere";
			FoundDialogue->EnglishText = FText::FromString("Text Not Found");
			FoundDialogue->JapaneseText = FText::FromString("Text Not Found");*/
		}

		else 
		{
			if (FoundDialogue->LinesRemaining != 0)
			{
				LineID++;
			}

			else 
			{
				//It's the last line in a dialogue
				if (FoundDialogue->DialogueType != EDialogueType::EDT_Loop)
				{
					//If the dialogue is not a loop
					ConversationID++;
					LineID = 1;
				}

				//If it is a loop, the LineID and ConversationID should remain the same so that the same line can be repeated.
				//To get out of the loop, it needs to change 
				

			}

		}

		return FoundDialogue;
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT(" DialogueTable not founde. 404. MIA. "));
		return nullptr;
	}

}

void ANonCombatBaseCharacter::AdjustConversationID(int NewConversationID)
{
	ConversationID = NewConversationID;
	LineID = 1;
}

void ANonCombatBaseCharacter::ToggleNonCombatBaseCharacterCamera()
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
	}
	
}
