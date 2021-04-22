// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorPool.h"

UActorPool::UActorPool()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UActorPool::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < PreWarmed; ++i)
	{
		InitializeNewPooledActor();
	}
}

UStaticMeshComponent* UActorPool::GetPooledActor()
{
	if(ActorPool.Num() < 1)
	{
		InitializeNewPooledActor();
	}
	UStaticMeshComponent* NewActor = ActorPool.Pop();
	if(!NewActor) return nullptr;
	NewActor->SetVisibility(true);
	NewActor->Activate();
	ActivePackages.Add(NewActor);
	
	return NewActor;
}

void UActorPool::InitializeNewPooledActor()
{
	UStaticMeshComponent* NewComponent = NewObject<UStaticMeshComponent>(GetOwner());
	NewComponent->RegisterComponent();
	NewComponent->SetVisibility(false);
	NewComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ActorPool.Add(NewComponent);
}

void UActorPool::ClearStackedPackages()
{
	for (auto Mesh : ActivePackages)
	{
		Mesh->SetVisibility(false);
		Mesh->Deactivate();
		Mesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		ActorPool.Add(Mesh);
	}
	ActivePackages.Empty();
}



