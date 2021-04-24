// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "HandControllerBase.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Sound/SoundCue.h"

#include "HandController.generated.h"

UENUM(BlueprintType)
enum class EGripState : uint8
{
	Open,
	CanGrab,
	Grab,
};

UCLASS()
class BILPAKK002_API AHandController : public AHandControllerBase
{
	GENERATED_BODY()
	
public:	
	AHandController();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	bool bUpdatePreview = false;
	
	UPROPERTY(VisibleAnywhere)
	USphereComponent* InteractionCollider;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* MapWidget;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* GridTick;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* PackagePlacement;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* FinalPackagePlacement;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* GridTickError;

	UPROPERTY()
	FTransform PreviousPreviewTransform;
	
public:
	virtual void Tick(float DeltaTime) override;
	virtual void GripPressed() override;
	virtual void GripReleased() override;
	//virtual void TriggerPressed() override;
	//virtual void TriggerReleased() override;
	class AStackablePackage* FindClosestPackageWithinRange();
	void SetMapEnabled(bool Enabled);

	UPROPERTY(BlueprintReadOnly)
	bool bWantsToGrip = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EGripState GripState = EGripState::Open;

	UFUNCTION(BlueprintCallable)
	void SetupMap();

	UPROPERTY(BlueprintReadOnly)
	AStackablePackage*  PackageInGrip;

	UPROPERTY(VisibleAnywhere)
	TArray<AStackablePackage*> GrippablePackages;

	UPROPERTY(VisibleAnywhere)
	class APlayfieldContainer* GrippablePlayfield;	

	UFUNCTION()
    void ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	
	UFUNCTION()
    void ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);
};
