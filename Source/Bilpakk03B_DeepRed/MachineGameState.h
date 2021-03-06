// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BilpakkGameState.h"
#include "MachinePawn.h"
#include "LevelGenerator.h"
#include "MachineGameState.generated.h"

/**
 * 
 */
UCLASS()
class BILPAKK03B_DEEPRED_API AMachineGameState : public ABilpakkGameState
{
	GENERATED_BODY()


	virtual void StartGame(FName Row) override;

	

public:
	AMachineGameState();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AMachinePawn * MLPawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ULevelGenerator *LevelGenerator;
	
};
