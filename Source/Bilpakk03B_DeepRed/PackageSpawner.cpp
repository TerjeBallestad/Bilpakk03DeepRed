// Fill out your copyright notice in the Description page of Project Settings.


#include "PackageSpawner.h"

#include "BilpakkGameState.h"
#include "PlayfieldContainer.h"
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
	PackageSpawnLocation.SetLocation(PackageSpawnLocation.GetLocation() + GetActorLocation());

	if(Materials.Num() < 1)
	{
		for (EPackageType Color : Colors)
		{
			UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(MaterialClass, this);
			Material->SetVectorParameterValue("Color", ColorLibrary.Colors[Color]);
			Materials.Add(Color, Material);
		}

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

int32 APackageSpawner::GetRemainingPackageAmount(UObject* WorldContextObject)
{
	if((!GEngine) && (!WorldContextObject)) return -1;

	ABilpakkGameState* State = Cast<ABilpakkGameState>(GEngine->
		GetWorldFromContextObjectChecked(WorldContextObject)->GetAuthGameMode()->GameState);
	if(State)
		return State->PackageSpawner->SpawnQueue.Num();
	else
		return -1;
}

void APackageSpawner::UpdateNextPackage()
{
	if(SpawnQueue.Num() < 1)
	{
		NextPackage = nullptr;
		return;
	}

	AStackablePackage* Package = PackagePool->GetPackage();
	Package->Setup(SpawnQueue[0]);
	Package->MeshComponent->SetMaterial(0, SpawnQueue[0].Material);
	FVector NewSpawnLocation = PackageSpawnLocation.GetLocation();
	NewSpawnLocation.X += Package->PackageParameters.SizeInt.X * 2.5 - 2.5;
	NewSpawnLocation.Y += Package->PackageParameters.SizeInt.Y * 2.5 - 2.5;
	NewSpawnLocation.Z -= Package->PackageParameters.SizeInt.Z * 2.5 - 2.5;
	Package->SetActorLocationAndRotation(NewSpawnLocation, PackageSpawnLocation.GetRotation());
	RemoveFirstPackageFromQueue();
	NextPackage = Package;
}

AStackablePackage* APackageSpawner::GetNextPackage()
{
	if(!NextPackage)
	{
		FTimerHandle Handle;
		GetWorldTimerManager().SetTimer(Handle, GameState, &ABilpakkGameState::FinishGame, 2, false);
	}
	return NextPackage;
}

bool APackageSpawner::RemoveFirstPackageFromQueue()
{
	if(SpawnQueue.Num() > 0)
	{
		SpawnQueue.RemoveAt(0, 1, true);
		//UE_LOG(LogTemp, Warning, TEXT("Count: %s"), SpawnQueue.Num());
		return true;
	}
	return false;
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

void APackageSpawner::ShuffleArray(TArray<FPackageParameters>& Array) const
{
	if (Array.Num() > 0)
	{
		const int32 LastIndex = Array.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			const int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				Array.Swap(i, Index);
			}
		}
	}
}
 
