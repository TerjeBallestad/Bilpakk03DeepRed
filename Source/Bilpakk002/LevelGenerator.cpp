// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"

#include <string>

#include "BilpakkGameState.h"
#include "PackageSpawner.h"


ULevelGenerator::ULevelGenerator()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void ULevelGenerator::BeginPlay()
{
	Super::BeginPlay();
}


void ULevelGenerator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void ULevelGenerator::Setup(UPackageGrid *InGrid)
{
	Grid = InGrid;
}

EPackageSize ULevelGenerator::RandomPackageSize()
{
	return static_cast<EPackageSize>(FMath::RandRange(0, static_cast<uint8>(EPackageSize::Count) - 1));
}

EPackageType ULevelGenerator::RandomPackageType()
{
	return static_cast<EPackageType>(FMath::RandRange(2, static_cast<uint8>(EPackageType::Count) - 1));
}

FBilpakkLevel ULevelGenerator::GenerateLevel()
{
	FBilpakkLevel NewLevel = ABilpakkGameState::GetLevelData(GetWorld());
	int RemainingCells = NewLevel.GridParameters.Size.X * NewLevel.GridParameters.Size.Y * NewLevel.GridParameters.Size.Z;
	for (FGridRange Range : NewLevel.GridParameters.DefaultOccupiedGridPositions)
	{
		int RangeAreaX = Range.Max.X - Range.Min.X;
		int RangeAreaY = Range.Max.Y - Range.Min.Y;
		RemainingCells -= RangeAreaX * RangeAreaY;
	}
	NewLevel.LevelName = FText::FromString(TEXT("Generated Level"));
	NewLevel.Difficulty = 3;
	NewLevel.PackageAmounts = TMap<EPackageType, FPackageAmounts>();
	for (int i = 2; i < static_cast<uint8>(EPackageType::Count) ; ++i)
	{
		NewLevel.PackageAmounts.Add(static_cast<EPackageType>(i));
		for (int j = 0; j < static_cast<uint8>(EPackageSize::Count) ; ++j)
		{
			NewLevel.PackageAmounts[static_cast<EPackageType>(i)].Amounts.Add(static_cast<EPackageSize>(j), 0);
		}
	}
	
		UE_LOG(LogTemp, Warning, TEXT("Remaining cells %d"), RemainingCells)
	while (RemainingCells > 100)
	{
		FPackageParameters NewPackage = FPackageParameters();
		NewPackage.Type = RandomPackageType();
		NewPackage.SizeE = RandomPackageSize();
		NewPackage.SizeInt = NewLevel.PackageLibrary[NewPackage.SizeE].SizeInt;
		NewLevel.PackageAmounts[NewPackage.Type].Amounts[NewPackage.SizeE] ++;
		RemainingCells -= NewPackage.SizeInt.X * NewPackage.SizeInt.Y * NewPackage.SizeInt.Z;
		UE_LOG(LogTemp, Warning, TEXT("Remaining cells %d"), RemainingCells)
	}
	return  NewLevel;
}



