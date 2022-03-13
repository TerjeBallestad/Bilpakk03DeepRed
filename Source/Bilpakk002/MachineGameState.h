// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BilpakkGameState.h"
#include "MachineGameState.generated.h"

/**
 * 
 */
UCLASS()
class BILPAKK002_API AMachineGameState : public ABilpakkGameState
{
	GENERATED_BODY()

	
	void StartGame(FName Row) override;
	
};
