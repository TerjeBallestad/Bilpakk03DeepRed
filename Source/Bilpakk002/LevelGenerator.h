// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PackageGrid.h"
#include "PackageSpawner.h"
#include "Components/ActorComponent.h"
#include "LevelGenerator.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BILPAKK002_API ULevelGenerator : public UActorComponent
{
	GENERATED_BODY()

public:	
	ULevelGenerator();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Setup(UPackageGrid *InGrid);
	FBilpakkLevel GenerateLevel();

private:
	EPackageSize RandomPackageSize();
	EPackageType RandomPackageType();
	
	UPROPERTY()
	UPackageGrid *Grid;

	UPROPERTY()
	FColorLibrary ColorLibrary = FColorLibrary();


		
};
