// Fill out your copyright notice in the Description page of Project Settings.


#include "PackageSpawner.h"

#include "BilpakkGameState.h"
#include "Kismet/KismetArrayLibrary.h"
#include "StackablePackage.h"


APackageSpawner::APackageSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	PackagePool = CreateDefaultSubobject<UPackagePool>(TEXT("PackagePool"));
}

void APackageSpawner::Setup(FBilpakkLevel Data)
{
	SpawnQueue.Empty();
	
	TArray<EPackageType> Colors;
	ColorLibrary.Colors.GetKeys(Colors);

	for (EPackageType Color : Colors)
	{
		UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(MaterialClass, this);
		Material->SetVectorParameterValue("Color", ColorLibrary.Colors[Color]);
		Materials.Add(Color, Material);
	}
	
	PackageLibrary = Data.PackageLibrary;
	TArray<EPackageType> PackageTypes;
	Data.PackageAmounts.GenerateKeyArray(PackageTypes);
	for (EPackageType Pt : PackageTypes)
	{
		TArray<EPackageSize> Sizes;
		Data.PackageAmounts[Pt].Amounts.GetKeys(Sizes);
		for (EPackageSize Size : Sizes)
		{
			for (int i = 0; i < Data.PackageAmounts[Pt].Amounts[Size]; ++i)
			{
				FPackageParameters Package;
				Package.Material = Materials[Pt];
				Package.Mesh = PackageLibrary[Size].Mesh;
				Package.SizeInt = PackageLibrary[Size].SizeInt;
				Package.Type = Pt;
				Package.SizeE = Size;
				SpawnQueue.Add(Package);	
			}
		}
	}
	ShuffleArray(SpawnQueue);
}

AStackablePackage* APackageSpawner::GetNextPackage()
{
	if(SpawnQueue.Num() < 1)
	{
		GameState->FinishGame();
		return nullptr;
	}
	
	AStackablePackage* Package = PackagePool->GetPackage();
	Package->Setup(SpawnQueue[0]);
	Package->MeshComponent->SetMaterial(0, SpawnQueue[0].Material);
	SpawnQueue.RemoveAt(0);
	return Package;
}

void APackageSpawner::InitializeEvents_Implementation(ABilpakkGameState* State)
{
	GameState = State;
	SetActorTransform(SpawnLocation);
	GameState->OnNewGame.AddDynamic(this, &APackageSpawner::NewGame);
	GameState->OnFinishedGame.AddDynamic(this, &APackageSpawner::Finish);
	GameState->OnPausedGame.AddDynamic(this, &APackageSpawner::Pause);
	GameState->OnUnPausedGame.AddDynamic(this, &APackageSpawner::UnPause);
}

void APackageSpawner::ShuffleArray(TArray<FPackageParameters>& Array)
{
	if (Array.Num() > 0)
	{
		int32 LastIndex = Array.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				Array.Swap(i, Index);
			}
		}
	}
}
 
