// Fill out your copyright notice in the Description page of Project Settings.


#include "HandControllerUI.h"

AHandControllerUI::AHandControllerUI()
{
	Pointer = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("Pointer"));
	Pointer->SetupAttachment(GetRootComponent());
}

void AHandControllerUI::TriggerPressed()
{
	Pointer->PressPointerKey(EKeys::LeftMouseButton);

}

void AHandControllerUI::TriggerReleased()
{
	Pointer->ReleasePointerKey(EKeys::LeftMouseButton);
}
