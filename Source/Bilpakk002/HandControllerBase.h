// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MotionControllerComponent.h"
#include "HandControllerBase.generated.h"

UCLASS()
class BILPAKK002_API AHandControllerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AHandControllerBase();

	virtual void SetHand(const EControllerHand Hand) const { MotionControllerComponent->SetTrackingSource(Hand); };
	virtual void GripPressed() {};
	virtual void GripReleased() {};
	virtual void TriggerPressed() {};
	virtual void TriggerReleased() {};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMotionControllerComponent* MotionControllerComponent;
};
