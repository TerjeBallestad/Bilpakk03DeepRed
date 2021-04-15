// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "VRCarpakPawn.h"
#include "GameFramework/GameModeBase.h"
#include "BilpakkGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BILPAKK002_API ABilpakkGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ABilpakkGameModeBase();
	virtual void BeginPlay() override;
	virtual  void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	UPROPERTY(VisibleAnywhere)
	FString RowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<AVRCarpakPawn> DefaultUIPawnClass;
};
