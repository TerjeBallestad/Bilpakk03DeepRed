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

	check(PlayerInputComponent);
	if(PlayerInputComponent)
	{
		/*PlayerInputComponent->BindAxis(TEXT("LeftVertical"), this, &AVRCarpakPawn::UpdateVertical);
		PlayerInputComponent->BindAxis(TEXT("LeftHorizontal"), this, &AVRCarpakPawn::UpdateHorizontal);*/
		PlayerInputComponent->BindAction(TEXT("MoveLeft"), IE_Pressed,this, &AMachinePawn::MoveLeft);
		PlayerInputComponent->BindAction(TEXT("MoveRight"), IE_Pressed,this, &AMachinePawn::MoveRight);
		PlayerInputComponent->BindAction(TEXT("MoveUp"), IE_Pressed,this, &AMachinePawn::MoveUp);
		PlayerInputComponent->BindAction(TEXT("MoveDown"), IE_Pressed,this, &AMachinePawn::MoveDown);
		PlayerInputComponent->BindAction(TEXT("RotateLeft"), IE_Pressed,this, &AMachinePawn::RotateLeft);
		PlayerInputComponent->BindAction(TEXT("RotateRight"), IE_Pressed,this, &AMachinePawn::RotateRight);
		PlayerInputComponent->BindAction(TEXT("FlipForward"), IE_Pressed,this, &AMachinePawn::FlipForward);
		PlayerInputComponent->BindAction(TEXT("FlipBackward"), IE_Pressed,this, &AMachinePawn::FlipBackward);
	}
}

void AMachinePawn::MoveLeft()
{
	UE_LOG(LogTemp, Warning, TEXT("Moving left"))
}

void AMachinePawn::MoveRight()
{
	UE_LOG(LogTemp, Warning, TEXT("Moving right"))
}

void AMachinePawn::MoveUp()
{
	UE_LOG(LogTemp, Warning, TEXT("Moving up "))
}

void AMachinePawn::MoveDown()
{
	UE_LOG(LogTemp, Warning, TEXT("Moving down "))
}

void AMachinePawn::RotateLeft()
{
	UE_LOG(LogTemp, Warning, TEXT("Rotating left "))
}

void AMachinePawn::RotateRight()
{
	UE_LOG(LogTemp, Warning, TEXT("Rotating right "))
}

void AMachinePawn::FlipForward()
{
	UE_LOG(LogTemp, Warning, TEXT("Flipping forward "))
}

void AMachinePawn::FlipBackward()
{
	UE_LOG(LogTemp, Warning, TEXT("Flipping backward "))
}

