// Fill out your copyright notice in the Description page of Project Settings.


#include "HandControllerBase.h"

AHandControllerBase::AHandControllerBase()
{
	PrimaryActorTick.bCanEverTick = false;

	MotionControllerComponent = CreateDefaultSubobject<UMotionControllerComponent> (TEXT("MotionController"));
	SetRootComponent(MotionControllerComponent);
	MotionControllerComponent->SetShowDeviceModel(true);
}

