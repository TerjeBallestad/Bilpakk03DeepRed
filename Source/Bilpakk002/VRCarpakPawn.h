// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"



#include "Camera/CameraComponent.h"
#include "HandControllerBase.h"
#include "GameFramework/Pawn.h"
#include "VRCarpakPawn.generated.h"

UCLASS()
class BILPAKK002_API AVRCarpakPawn : public APawn
{
	GENERATED_BODY()

public:
	AVRCarpakPawn();
	
	UFUNCTION(BlueprintCallable)
	void SetupMap();
	
	void Setup();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void Finish();

	UFUNCTION(BlueprintImplementableEvent)
	void NewGame();

	UFUNCTION(BlueprintImplementableEvent)
    void Pause();

	UFUNCTION(BlueprintImplementableEvent)
    void UnPause();

public:	

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void SetActorHiddenInGame(bool bNewHidden) override;

	UPROPERTY(BlueprintReadOnly)
	UCameraComponent* VRCamera;

	UFUNCTION(BlueprintNativeEvent)
	void PauseGame();
	
	UPROPERTY(BlueprintReadOnly)
	class ABilpakkGameState* GameState;

	UFUNCTION(BlueprintNativeEvent)
	void InitializeEvents(class ABilpakkGameState *State);
	
private:

	bool bShouldUpdateCar = false;
	float Horizontal;
	float Vertical;
	void GripLeft() { LeftHandController->GripPressed(); }
	void TriggerLeft() { LeftHandController->TriggerPressed(); }
	void GripReleaseLeft() { LeftHandController->GripReleased(); }
	void TriggerReleaseLeft() { LeftHandController->TriggerReleased(); }
	
	void GripRight() {RightHandController->GripPressed(); }
	void TriggerRight() { RightHandController->TriggerPressed(); }
	void GripReleaseRight() { RightHandController->GripReleased(); }
	void TriggerReleaseRight() { RightHandController->TriggerReleased(); }

	UPROPERTY(VisibleAnywhere)
	USceneComponent* VRRoot;
	
	UPROPERTY()
	bool bSetup = false;
	
	UPROPERTY(VisibleAnywhere)
	AHandControllerBase* RightHandController;

	UPROPERTY(VisibleAnywhere)
	AHandControllerBase* LeftHandController;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHandControllerBase> HandControllerClass;


};
