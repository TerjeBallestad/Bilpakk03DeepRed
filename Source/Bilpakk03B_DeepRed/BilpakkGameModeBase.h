// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"
#include "BilpakkGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BILPAKK03B_DEEPRED_API ABilpakkGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ABilpakkGameModeBase();
	virtual void BeginPlay() override;
	virtual  void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	UPROPERTY(VisibleAnywhere)
	FString RowName;
	
};
