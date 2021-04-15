// Fill out your copyright notice in the Description page of Project Settings.


#include "HandController.h"


#include "PlayfieldContainer.h"
#include "RouteMap.h"

AHandController::AHandController()
{
	PrimaryActorTick.bCanEverTick = false;

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

void AHandController::GripPressed()
{
	if (!GrippablePlayfield) return;

	if(GrippablePlayfield->StartPanning(this))
	{
		PlayfieldInGrip = GrippablePlayfield;
	}
}

void AHandController::GripReleased()
{
	if(PlayfieldInGrip)
	{
		PlayfieldInGrip->EndPanning(this);
		PlayfieldInGrip = nullptr;
	}
}

void AHandController::TriggerPressed()
{
	ActivePackage = FindClosestInteractableActor();
	if(!ActivePackage) return;
	UE_LOG(LogTemp, Warning, TEXT("Triggering %s"), *ActivePackage->GetName());
	ActivePackage->StartInteract(this);
}

void AHandController::TriggerReleased()
{
	if(!ActivePackage) return;

	if(GrippablePlayfield)
	{
		GrippablePlayfield->PlacePackage();
		ActivePackage->StopInteract();
		ActivePackage = nullptr;
	} else
	{
		//TODO
		//InteractingActor->PackageSpawner->SpawnQueue.Add(InteractingActor->PackageParameters);
		ActivePackage->StopInteract();
		ActivePackage = nullptr;
	}
}

AStackablePackage* AHandController::FindClosestInteractableActor()
{
	float ShortestDistance = INFINITY;
	AStackablePackage* ClosestActor = nullptr;
	for (AStackablePackage* InteractableActor : InteractableActors)
	{
		float Distance = FVector::Dist(InteractionCollider->GetComponentLocation(), InteractableActor->GetActorLocation());
		if(Distance < ShortestDistance)
		{
			ClosestActor = InteractableActor;
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
	if(AStackablePackage* Package = Cast<AStackablePackage>(OtherActor))
	{
		InteractableActors.Add(Package);
	}

	if(APlayfieldContainer* PlayfieldContainer = Cast<APlayfieldContainer>(OtherActor))
	{
		GrippablePlayfield = PlayfieldContainer;
	}
}

void AHandController::ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if(InteractableActors.Contains(OtherActor))
	{
		InteractableActors.Remove(Cast<AStackablePackage>(OtherActor));
	}

	if (OtherActor == GrippablePlayfield)
	{
		GrippablePlayfield = nullptr;
	}
}



