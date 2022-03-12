// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"



#include "BilpakkGameState.h"
#include "PackageGrid.h"
#include "PointsCalculator.h"


#include "PlayfieldContainer.generated.h"


UCLASS()
class BILPAKK002_API APlayfieldContainer : public AActor
{
	GENERATED_BODY()
	
public:	
	APlayfieldContainer();

	UPROPERTY(BlueprintReadOnly)
	ABilpakkGameState* GameState;


protected:
	virtual void BeginPlay() override;
	void PanPlayfield();

	UPROPERTY(EditDefaultsOnly)
	FTransform SpawnLocation;

	UPROPERTY()
	bool bUpdatePreview = false;
	
	UPROPERTY()
	AActor* PreviewActor;

	UPROPERTY()
	FTransform PreviousPreviewTransform;

	UPROPERTY()
	FGridRange PreviewRange;

	UPROPERTY(BlueprintReadOnly)
	UStaticMeshComponent* PreviewMesh;

	UPROPERTY(BlueprintReadWrite)
	bool bPanCylinder = false;

	UPROPERTY(BlueprintReadWrite)
	AActor* PanHand;

	UPROPERTY(BlueprintReadWrite)
	FVector InitialHandLocation;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UActorPool* StaticMeshPool;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* CarModel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPackageGrid* Grid;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetComponent* FloatingTextWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetComponent* NegativeFloatingTextWidget;

	//UPROPERTY()
	//class AStackablePackage* ActivePackage;

	UPROPERTY(VisibleAnywhere)
	UPointsCalculator* PointsCalculator;
	
public:	
	//virtual void Tick(float DeltaTime) override;
	bool PlacePackage(AStackablePackage* ActivePackage);
	void StartUpdatingPreview(UMaterialInstance* Material, UStaticMesh* Mesh);
	void StopUpdatingPreview();
	bool UpdatePreview(AStackablePackage* ActivePackage, FTransform& InOutPreviewTransform);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FPoints Points;

	UFUNCTION(BlueprintNativeEvent)
	void InitializeEvents(class ABilpakkGameState *State);

	UPROPERTY(EditDefaultsOnly)
	float PreviewMovementThreshold = 0.1;

	UFUNCTION(BlueprintCallable)
	void Setup(FBilpakkLevel LevelData);

	UPROPERTY(BlueprintAssignable)
	FComponentCustomStartSignature OnPointsAdded;

	UPROPERTY(EditAnywhere)
	FName LevelDataRowName;

	/*UFUNCTION(BlueprintNativeEvent)
	bool StartPanning(AActor* Hand);

	UFUNCTION(BlueprintNativeEvent)
	void EndPanning(AActor* Hand);*/

	//UFUNCTION()
	//void ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	/*
	UFUNCTION()
    void ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);
    */

	UFUNCTION(BlueprintImplementableEvent)
	void Finish();

	UFUNCTION(BlueprintImplementableEvent)
    void NewGame();

	UFUNCTION(BlueprintImplementableEvent)
    void Pause();

	UFUNCTION(BlueprintImplementableEvent)
    void UnPause();
};


