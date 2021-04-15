// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayfieldContainer.h"

#include "ActorPool.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "PackageSpawner.h"
#include "StackablePackage.h"

APlayfieldContainer::APlayfieldContainer()
{
	PrimaryActorTick.bCanEverTick = false;

	CarModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Car"));
	SetRootComponent(CarModel);

	Grid = CreateDefaultSubobject<UPackageGrid>(TEXT("Grid"));
	PointsCalculator = CreateDefaultSubobject<UPointsCalculator>(TEXT("Points Calculator"));
	StaticMeshPool = CreateDefaultSubobject<UActorPool>(TEXT("StaticMeshPool"));
}

void APlayfieldContainer::BeginPlay()
{
	Super::BeginPlay();

	PreviewActor = GetWorld()->SpawnActor<AActor>();
	PreviewMesh = NewObject<UStaticMeshComponent>(PreviewActor);
	if(PreviewMesh)
	{
		PreviewMesh->RegisterComponent();
		PreviewMesh->SetMobility(EComponentMobility::Movable);
		PreviewMesh->SetVisibility(false);
		PreviewActor->SetRootComponent(PreviewMesh);
	}
}

void APlayfieldContainer::PlacePackage(AStackablePackage* ActivePackage)
{
	UStaticMeshComponent* NewPackage = StaticMeshPool->GetPooledActor();
	if(!NewPackage) return;
	NewPackage->AttachToComponent(CarModel, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	NewPackage->SetStaticMesh(PreviewMesh->GetStaticMesh());
	NewPackage->SetMaterial(0,ActivePackage->PackageParameters.Material);
	NewPackage->SetWorldLocationAndRotation(PreviewActor->GetActorLocation(), PreviewActor->GetActorRotation());
	Grid->SetStatus(PreviewRange, ActivePackage->PackageParameters.Type);
	GameState->AddBonusPoints(PointsCalculator->CalculatePlacePackagePoints(PreviewRange, ActivePackage->PackageParameters.Type));
	GameState->SetPoints(PointsCalculator->CalculateEndGamePoints());
	OnPointsAdded.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("Returned Points: %d"), ABilpakkGameState::GetPoints(GetWorld()));
	PreviewMesh->SetVisibility(false);
	//TODO Disable collision
}

void APlayfieldContainer::StartUpdatingPreview(UMaterialInstance* Material, UStaticMesh* Mesh)
{
	PreviewMesh->SetVisibility(true);
	PreviewMesh->SetMaterial(0, Material);
	PreviewMesh->SetStaticMesh(Mesh);
	PreviewMesh->SetVisibility(true);
	bUpdatePreview = true;
}

void APlayfieldContainer::StopUpdatingPreview()
{
	PreviewMesh->SetVisibility(false);
	bUpdatePreview = false;
}

void APlayfieldContainer::InitializeEvents_Implementation(ABilpakkGameState* State)
{
	GameState = State;
	SetActorTransform(SpawnLocation);
	GameState->OnNewGame.AddDynamic(this, &APlayfieldContainer::NewGame);
	GameState->OnFinishedGame.AddDynamic(this, &APlayfieldContainer::Finish);
	GameState->OnPausedGame.AddDynamic(this, &APlayfieldContainer::Pause);
	GameState->OnUnPausedGame.AddDynamic(this, &APlayfieldContainer::UnPause);
}

void APlayfieldContainer::Setup(FBilpakkLevel LevelData)
{
	TArray<EPackageType> Colors;
	LevelDataRowName = FName(LevelData.LevelName.ToString());
	FColorLibrary().Colors.GetKeys(Colors); 
	Grid->Setup(LevelData.GridParameters);
	PointsCalculator->Setup(Colors, Grid, LevelData.Doors);
}

void APlayfieldContainer::PanPlayfield()
{
	const FVector CurrentLoc = PanHand->GetActorLocation();
	float Degrees = FMath::Atan2(CurrentLoc.X, CurrentLoc.Y) - FMath::Atan2(InitialHandLocation.X, InitialHandLocation.Y); 
	Degrees *=10;
	const FRotator Delta = FRotator(0,Degrees,0);
	CarModel->AddRelativeRotation(Delta);
}

void APlayfieldContainer::UpdatePreview(AStackablePackage* ActivePackage)
{
	FTransform PreviewTransform;

	
	CalculatePackageBounds(ActivePackage, PreviewRange);
	if(Grid->FindSpaceForPackage(ActivePackage, PreviewRange, PreviewTransform))
	{
		PreviewActor->SetActorTransform(PreviewTransform);
	}
}

void APlayfieldContainer::CalculatePackageBounds(AStackablePackage* ActivePackage, FGridRange& OutRange)
{
	FTransform ActivePackageTransform = ActivePackage->GetTransform();
	ActivePackageTransform.SetLocation( Grid->SnapLocationToGrid(ActivePackageTransform.GetLocation()));
	ActivePackageTransform.SetRotation(Grid->SnapRotationToGridLocal(ActivePackageTransform.GetRotation().Rotator()));
	//DrawDebugCoordinateSystem(GetWorld(), Grid->SnapLocationToGrid(ActivePackageTransform.GetLocation()),ActivePackageTransform.GetRotation().Rotator(), 10, false, 0, 2, .2);
	//ActivePackage->GetPackageRange(ActivePackageTransform.Rotator());
	FVector Min;
	FVector Max;
	PreviewMesh->GetLocalBounds(Min, Max);
		
	Min = ActivePackageTransform.TransformVector(Min);
	Max = ActivePackageTransform.TransformVector(Max);

	if (Min.X > Max.X)
	{
		float Temp = Min.X;
		Min.X = Max.X;
		Max.X = Temp;
	}

	if (Min.Y > Max.Y)
	{
		float Temp = Min.Y;
		Min.Y = Max.Y;
		Max.Y = Temp;
	}

	if (Min.Z > Max.Z)
	{
		float Temp = Min.Z;
		Min.Z = Max.Z;
		Max.Z = Temp;
	}
	 //DrawDebugBox(GetWorld(), ActivePackage->GetActorLocation() + FVector::UpVector * 20, (Min - Max) * .5,ActivePackageTransform.GetRotation(), FColor(250,50,55), false, 0, 2);

	OutRange.Min = Grid->RoundFVectorToIntVector(Min / Grid->CellSize);
	OutRange.Max = Grid->RoundFVectorToIntVector(Max / Grid->CellSize);

	OutRange.Min += Grid->WorldToGridLocation(ActivePackage->GetActorLocation());
	OutRange.Max += Grid->WorldToGridLocation(ActivePackage->GetActorLocation());
}