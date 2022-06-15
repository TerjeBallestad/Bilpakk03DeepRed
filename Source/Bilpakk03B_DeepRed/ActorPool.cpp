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
	UStaticMeshComponent* Mesh = ActorPool.Pop(false);
	if(!Mesh) return nullptr;
	Mesh->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	Mesh->SetVisibility(true);
	ActivePackages.Add(Mesh);
	
	return Mesh;
}

void UActorPool::InitializeNewPooledActor()
{
	UStaticMeshComponent* Mesh = NewObject<UStaticMeshComponent>(GetOwner());
	Mesh->RegisterComponent();
	Mesh->SetVisibility(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ActorPool.Add(Mesh);
	//UE_LOG(LogTemp, Warning, TEXT("%d elements from %s"), ActorPool.Num(), *GetOwner()->GetName())
}

void UActorPool::ClearStackedPackages()
{
	for (auto Mesh : ActivePackages)
	{
		Mesh->SetVisibility(false);
		Mesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
		ActorPool.Add(Mesh);
	}
	ActivePackages.Empty(ActorPool.Num());
}



