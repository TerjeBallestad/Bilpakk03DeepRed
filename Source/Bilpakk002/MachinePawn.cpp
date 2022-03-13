// Fill out your copyright notice in the Description page of Project Settings.


#include "MachinePawn.h"

AMachinePawn::AMachinePawn()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AMachinePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMachinePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMachinePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

