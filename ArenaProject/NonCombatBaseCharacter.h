// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "Enums.h"
#include "NonCombatBaseCharacter.generated.h"



UCLASS()
class ARENAPROJECT_API ANonCombatBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANonCombatBaseCharacter();


	//Be sure that the Name written the same as TalkPersonName in dialogue sheet
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FName Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StartPosition")
	FVector StartingPosition;

	/* World Context variables. Used for Dialogue system.
	* 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	class UDataTable* DialogueTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	int ConversationID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	int LineID;


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


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//FUNCTIONS

	/*Basic Camera Movement functions
	* Yaw and Pitch input functions
	*/

	void Turn(float value);
	void LookUp(float value);

	//Function to return correct dialogue
	FDialogue* GetDialogue(int WorldContext, int LocalContext);

	//Function to correct conversation ID. 
	FORCEINLINE void SetConversationID(int NewConversationID) { ConversationID = NewConversationID; };

	void AdjustConversationID(int NewConversationID);

	void ToggleNonCombatBaseCharacterCamera();

};
