// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayfieldContainer.h"

#include "ActorPool.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "PackageSpawner.h"
#include "StackablePackage.h"
#include "VRCarpakPawn.h"
#include "Kismet/KismetMathLibrary.h"

APlayfieldContainer::APlayfieldContainer()
{
	PrimaryActorTick.bCanEverTick = false;

	CarModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Car"));
	SetRootComponent(CarModel);

	Grid = CreateDefaultSubobject<UPackageGrid>(TEXT("Grid"));
	PointsCalculator = CreateDefaultSubobject<UPointsCalculator>(TEXT("Points Calculator"));
	StaticMeshPool = CreateDefaultSubobject<UActorPool>(TEXT("StaticMeshPool"));
	FloatingTextWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("FloatingTextWidget"));
	FloatingTextWidget->SetupAttachment(CarModel);
	NegativeFloatingTextWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("NegativeFloatingTextWidget"));
	NegativeFloatingTextWidget->SetupAttachment(CarModel);
}

void APlayfieldContainer::BeginPlay()
{
	Super::BeginPlay();

	PreviewActor = GetWorld()->SpawnActor<AActor>();
	PreviewMesh = NewObject<UStaticMeshComponent>(PreviewActor);
	if( PreviewMesh)
	{
		PreviewMesh->RegisterComponent();
		PreviewMesh->SetMobility(EComponentMobility::Movable);
		PreviewMesh->SetVisibility(false);
		PreviewActor->SetRootComponent(PreviewMesh);
	}
}

bool APlayfieldContainer::PlacePackage(AStackablePackage* Package)
{
	UStaticMeshComponent* NewPackage = StaticMeshPool->GetPooledActor();
	if(!NewPackage || !PreviewMesh->IsVisible()) return false;
	
	//NewPackage->AttachToComponent(CarModel, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	NewPackage->SetStaticMesh(PreviewMesh->GetStaticMesh());
	NewPackage->SetMaterial(0,Package->PackageParameters.Material);
	NewPackage->SetWorldLocationAndRotation(PreviewActor->GetActorLocation(), PreviewActor->GetActorRotation());
	Grid->SetStatus(PreviewRange, Package->PackageParameters.Type);
	int32 Bonus = PointsCalculator->CalculatePlacePackagePoints(PreviewRange, Package->PackageParameters.Type);
	GameState->AddBonusPoints(Bonus);
	Points = PointsCalculator->CalculateEndGamePoints();
	Points.Diff += Bonus;
	Points.Bonus = Bonus;
	if(Points.Diff == abs(Points.Negative))
	{
		Points.Diff = 0;
		Points.Negative = 0;
	}
	GameState->SetPoints(Points.Total);
	FloatingTextWidget->SetWorldLocation(PreviewMesh->Bounds.Origin + FVector::UpVector * PreviewMesh->Bounds.BoxExtent.Z);
	NegativeFloatingTextWidget->SetWorldLocation(PreviewMesh->Bounds.Origin + FVector::UpVector * PreviewMesh->Bounds.BoxExtent.Z);
	
	FRotator WidgetRotation = UKismetMathLibrary::FindLookAtRotation(FloatingTextWidget->GetComponentLocation(),GameState->MLPawn->PlayerCamera->GetComponentLocation());
	WidgetRotation.Pitch = 0;
	FloatingTextWidget->SetWorldRotation(WidgetRotation);
	NegativeFloatingTextWidget->SetWorldRotation(WidgetRotation);
	OnPointsAdded.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("Returned Points: %d, and %d negative points, %d bonus, %d diff, %d from gamestate"), Points.Total, Points.Negative, Points.Bonus, Points.Diff, ABilpakkGameState::GetPoints(GetWorld()));
	PreviewMesh->SetVisibility(false);
	GetNewActivePackage();
	GameState->PackageSpawner->PackagePool->ReturnPackage(Package);

	return true;
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

void APlayfieldContainer::InitializeEvents_Implementation(AMachineGameState* State)
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
	Grid->Setup(LevelData.GridParameters, FTransform::Identity);
	PointsCalculator->Setup(Colors, Grid, LevelData.Doors);
	PackagePreviewGridPosition = FIntVector(4,4,4);
	GetNewActivePackage();
	//ActivePackage->MeshComponent->SetVisibility(false);
/*	ActivePackage = GameState->PackageSpawner->GetNextPackage();
	//ActivePackage->MeshComponent->SetVisibility(false);
	const FVector PreviewLocation = Grid->GridToWorldLocation(PackagePreviewGridPosition);
	ActivePackage->SetActorLocation(PreviewLocation);
	ActivePackageRotator = GetWorld()->SpawnActor<AActor>();
	UStaticMeshComponent *NewMesh = NewObject<UStaticMeshComponent>(ActivePackageRotator);
	NewMesh->RegisterComponent();
	NewMesh->SetMobility(EComponentMobility::Movable);
	NewMesh->SetVisibility(false);
	ActivePackageRotator->SetRootComponent(NewMesh);

	FVector Min;
	FVector Max;
	ActivePackage->MeshComponent->GetLocalBounds(Min, Max);

	const FVector Middle = (Max + Min) / 2 + PreviewLocation;
	UE_LOG(LogTemp, Warning, TEXT("min: %s "), *((Max + Min) / 2).ToString());
	DrawDebugBox(GetWorld(), PreviewLocation, FVector::OneVector, FColor::Cyan, true, 10,0,1);
	ActivePackageRotator->SetActorLocation(Middle);
	ActivePackage->AttachToActor(ActivePackageRotator, FAttachmentTransformRules::KeepWorldTransform);
	ActivePackageRotator->SetActorLocation(PreviewLocation);
	StartUpdatingPreview(ActivePackage->PackageParameters.Material, ActivePackage->MeshComponent->GetStaticMesh());
	FTransform outTransform;
	UpdatePreview(ActivePackage, outTransform);*/
}

void APlayfieldContainer::PanPlayfield()
{
	const FVector CurrentLoc = PanHand->GetActorLocation();
	float Degrees = FMath::Atan2(CurrentLoc.X, CurrentLoc.Y) - FMath::Atan2(InitialHandLocation.X, InitialHandLocation.Y); 
	Degrees *=10;
	const FRotator Delta = FRotator(0,Degrees,0);
	CarModel->AddRelativeRotation(Delta);
}

void APlayfieldContainer::GetNewActivePackage()
{
	if(ActivePackage) ActivePackage->StopInteract();
	ActivePackage = GameState->PackageSpawner->GetNextPackage();
	
	const FVector PreviewLocation = Grid->GridToWorldLocation(PackagePreviewGridPosition);

	if(!ActivePackageRotator)
	{
		ActivePackageRotator = GetWorld()->SpawnActor<AActor>();
		UStaticMeshComponent *NewMesh = NewObject<UStaticMeshComponent>(ActivePackageRotator);
		NewMesh->RegisterComponent();
		NewMesh->SetMobility(EComponentMobility::Movable);
		//NewMesh->SetVisibility(false);
		ActivePackageRotator->SetRootComponent(NewMesh);
		ActivePackageRotator->SetActorLocation(PreviewLocation);
	} 

	//FVector Min, Max;
	//ActivePackage->MeshComponent->GetLocalBounds(Min, Max);
	

	const FVector PackagePosition = PreviewLocation + (FVector( ActivePackage->PackageParameters.SizeInt ) - Grid->CellSize / 2);
	//const FVector Middle = (Max + Min) / 2 - 2.5 + PreviewLocation;
	ActivePackage->SetActorLocation(PackagePosition);
	//UE_LOG(LogTemp, Warning, TEXT("min: %s "), *((Max + Min) / 2).ToString());
	DrawDebugBox(GetWorld(), PreviewLocation, FVector::OneVector * 0.1, FColor::Cyan, true, 10,3,1);
	DrawDebugBox(GetWorld(), PackagePosition, FVector::OneVector *0.1, FColor::Emerald, true, 10,2,1);
	//ActivePackageRotator->SetActorLocation(Middle);
	ActivePackage->AttachToActor(ActivePackageRotator, FAttachmentTransformRules::KeepWorldTransform);
	//ActivePackageRotator->SetActorLocation(PreviewLocation);
	StartUpdatingPreview(ActivePackage->PackageParameters.Material, ActivePackage->MeshComponent->GetStaticMesh());
	FTransform outTransform;
	UpdatePreview(ActivePackage, outTransform);
	
}

bool APlayfieldContainer::UpdatePreview(const AStackablePackage* Package, FTransform& InOutPreviewTransform)
{
	DrawDebugCoordinateSystem(GetWorld(), ActivePackageRotator->GetActorLocation(), ActivePackageRotator->GetActorRotation(), 10, true, 10);
	FVector Min;
	FVector Max;
	PreviewMesh->GetLocalBounds(Min, Max);
	Grid->CalculatePackageBounds(Package->GetTransform(), Min, Max, PreviewRange);
	if(Grid->FindSpaceForPackage(Package->GetTransform() , PreviewRange, InOutPreviewTransform))
	{
		//if(PreviewMovementThreshold < FVector::Distance(PreviewActor->GetActorLocation(), InOutPreviewTransform.GetLocation()))
		//{
			PreviewActor->SetActorTransform(InOutPreviewTransform);
		//}
		PreviewMesh->SetVisibility(true);
		return true;
	} 
	PreviewMesh->SetVisibility(false);
	return false;
}

void APlayfieldContainer::MovePreviewBlock(FIntVector MovementDelta)
{
	const FIntVector GridLocationToCheck = Grid->SnapLocationToGrid( PackagePreviewGridPosition + MovementDelta);
	FVector Min;
	FVector Max;
	FTransform FoundTransform;
	ActivePackage->MeshComponent->GetLocalBounds(Min, Max);
	//const FVector Location = (Max + Min) / 2 - 2.5 + Grid->GridToWorldLocation(GridLocationToCheck);
	ActivePackageRotator->SetActorLocation(Grid->GridToWorldLocation(GridLocationToCheck));
	Grid->CalculatePackageBounds(ActivePackage->GetTransform(), Min, Max, PreviewRange);
	if (Grid->FindSpaceForPackage(ActivePackage->GetTransform(), PreviewRange,  FoundTransform))
	{
		DrawDebugCoordinateSystem(GetWorld(), ActivePackage->GetActorLocation(), ActivePackageRotator->GetActorRotation(), 15, false, 10);
		PackagePreviewGridPosition = GridLocationToCheck;
		PreviewActor->SetActorTransform(FoundTransform);
	}
}

void APlayfieldContainer::RotatePreviewBlock(FRotator RotationDelta)
{
	//FRotator NewRotation(ActivePackageRotator->GetActorRotation().Add(RotationDelta.Pitch, RotationDelta.Yaw, RotationDelta.Roll));
	ActivePackageRotator->AddActorWorldRotation(RotationDelta);
	FTransform outTransform;
	UpdatePreview(ActivePackage, outTransform);
}

void APlayfieldContainer::PlaceActivePackage()
{
	PlacePackage(ActivePackage);	
}

