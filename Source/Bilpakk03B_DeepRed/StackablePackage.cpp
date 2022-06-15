// Fill out your copyright notice in the Description page of Project Settings.

#include "StackablePackage.h"

AStackablePackage::AStackablePackage()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(MeshComponent);
}

void AStackablePackage::ReturnToPackagePool()
{
	PackagePool->ReturnPackage(this);
}

/*FGridRange AStackablePackage::GetPackageRange(FRotator Rotation)
{
	//TODO 
	SizeE;
	FIntVector Min = FIntVector();
	FIntVector Max = FIntVector();
	FGridRange Range;
	Range.Min = Min;
	Range.Max = Max;
	return Range;
}*/

void AStackablePackage::Setup(FPackageParameters NewPackage)
{
	PackageParameters = NewPackage;
	
	MeshComponent->SetStaticMesh(NewPackage.Mesh);
}


void AStackablePackage:: StopInteract()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	if(PackagePool)
	{
		PackagePool->OnPackageInteracted.Broadcast();
		ReturnToPackagePool();
	}
	bIsGripped = false;
}










