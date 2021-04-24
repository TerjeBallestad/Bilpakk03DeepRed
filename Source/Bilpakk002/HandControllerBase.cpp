// Fill out your copyright notice in the Description page of Project Settings.


#include "HandControllerBase.h"

#include "Components/AudioComponent.h"

AHandControllerBase::AHandControllerBase()
{
	PrimaryActorTick.bCanEverTick = true;

	MotionControllerComponent = CreateDefaultSubobject<UMotionControllerComponent> (TEXT("MotionController"));
	SetRootComponent(MotionControllerComponent);
	MotionControllerComponent->SetShowDeviceModel(true);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(MotionControllerComponent);
	
}

