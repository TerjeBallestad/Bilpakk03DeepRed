// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HandControllerBase.h"
#include "Components/WidgetInteractionComponent.h"

#include "HandControllerUI.generated.h"

/**
 * 
 */
UCLASS()
class BILPAKK002_API AHandControllerUI : public AHandControllerBase
{
	GENERATED_BODY()

public:
	AHandControllerUI();

	virtual void TriggerPressed() override;
	virtual void TriggerReleased() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UWidgetInteractionComponent* Pointer;
};
