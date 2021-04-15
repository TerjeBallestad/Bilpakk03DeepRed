// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "BilpakkSave.generated.h"

USTRUCT(BlueprintType)
struct FHighscore
{
	GENERATED_BODY()

	UPROPERTY()
	FString RowName;

	UPROPERTY()
	bool bComplete = false;

	UPROPERTY()
	TArray<int32> Scores;
};

/**
 * 
 */
UCLASS()
class BILPAKK002_API UBilpakkSave : public USaveGame
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static UBilpakkSave* Create();

	UFUNCTION(BlueprintCallable)
	static UBilpakkSave* Load();

	UFUNCTION(BlueprintCallable)
	bool Save();
	
	UFUNCTION(BlueprintCallable)
	TArray<int32> GetHighscores(FString InRowName);

	UFUNCTION(BlueprintCallable)
	void AddHighscore(int32 InScore, FString InRowName);
	
private:
	static const FString SLOT_NAME;
	
	UPROPERTY()
	TArray<FHighscore> Highscores;
	
};
