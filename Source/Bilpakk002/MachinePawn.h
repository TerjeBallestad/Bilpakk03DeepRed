// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BilpakkGameState.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "MachinePawn.generated.h"

UCLASS()
class BILPAKK002_API AMachinePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMachinePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly)
	UCameraComponent *PlayerCamera;

	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent *CameraArm;

private:
	void PlacePackage();
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();
	void RotateLeft();
	void RotateRight();
	void FlipForward();
	void FlipBackward();

	UPROPERTY()
	ABilpakkGameState* GameState;
};
