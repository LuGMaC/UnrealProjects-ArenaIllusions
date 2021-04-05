// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Enums.h"
#include "ArenaSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class ARENAPROJECT_API UArenaSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UArenaSaveGame();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Context System")
	int WorldContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Context System")
	int LocalContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stance List")
	TMap<FString, FCharacterSkillsVariableData> CharacterSkillVariableInformationMap;
};
