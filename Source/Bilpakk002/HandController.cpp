// Fill out your copyright notice in the Description page of Project Settings.


#include "HandController.h"


#include "PlayfieldContainer.h"
#include "RouteMap.h"

AHandController::AHandController()
{
	PrimaryActorTick.bCanEverTick = true;

	InteractionCollider = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionCollider"));
	InteractionCollider->SetupAttachment(MotionControllerComponent);
	InteractionCollider->InitSphereRadius(5);
	InteractionCollider->SetRelativeLocation(FVector(10,0,0));
	MapWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Map"));
	MapWidget->SetupAttachment(MotionControllerComponent);
	MapWidget->SetVisibility(false);
}

void AHandController::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &AHandController::ActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AHandController::ActorEndOverlap);
}

void AHandController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(ActivePackage && GrippablePlayfield)
	{
		GrippablePlayfield->UpdatePreview(ActivePackage);
	}
}

void AHandController::GripPressed()
{
	ActivePackage = FindClosestPackageWithinRange();
	if(!ActivePackage) return;
	UE_LOG(LogTemp, Warning, TEXT("Triggering %s"), *ActivePackage->GetName());
	ActivePackage->StartInteract(this);
}

void AHandController::GripReleased()
{
	if(!ActivePackage) return;

	if(GrippablePlayfield)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trigger released in playfield"));
		GrippablePlayfield->PlacePackage(ActivePackage);
		ActivePackage->StopInteract();
		ActivePackage = nullptr;
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Trigger released outside playfield"));
		//TODO
		ActivePackage->PackageSpawner->SpawnQueue.Insert(ActivePackage->PackageParameters, 0);
		ActivePackage->StopInteract();
		ActivePackage = nullptr;
	}
}

AStackablePackage* AHandController::FindClosestPackageWithinRange()
{
	TArray<AActor*> Packages;
	InteractionCollider->GetOverlappingActors(Packages, AStackablePackage::StaticClass());
	float ShortestDistance = FLT_MAX;
	AStackablePackage* ClosestActor = nullptr;
	for (AActor* Package : Packages)
	{
		float Distance = FVector::Dist(InteractionCollider->GetComponentLocation(), Package->GetActorLocation());
		if(Distance < ShortestDistance)
		{
			ClosestActor = Cast<AStackablePackage>(Package);
			ShortestDistance = Distance;
		}	
	}
	return ClosestActor;
}

void AHandController::SetMapEnabled(bool Enabled)
{
	MapWidget->SetVisibility(Enabled);
}

void AHandController::SetupMap()
{
	MapWidget->SetVisibility(true);
	URouteMap* Map = Cast<URouteMap>(MapWidget->GetUserWidgetObject());
	if(Map)
	{
		Map->Setup();
	}
}

void AHandController::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if(APlayfieldContainer* PlayfieldContainer = Cast<APlayfieldContainer>(OtherActor))
	{
		GrippablePlayfield = PlayfieldContainer;
		if(ActivePackage)
		{
			GrippablePlayfield->StartUpdatingPreview(ActivePackage->PackageParameters.Material, ActivePackage->MeshComponent->GetStaticMesh());
			ActivePackage->MeshComponent->SetMaterial(0, ActivePackage->HoloMaterial);
			
		}
	}
}

void AHandController::ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor == GrippablePlayfield)
	{
		GrippablePlayfield->StopUpdatingPreview();
		GrippablePlayfield = nullptr;
	}
}



