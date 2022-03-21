// Fill out your copyright notice in the Description page of Project Settings.


#include "PackagePool.h"
#include "StackablePackage.h"
#include "PackageSpawner.h"

UPackagePool::UPackagePool()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UPackagePool::BeginPlay()
{
	Super::BeginPlay();

	if(!PackageClass) return;
	for (int i = 0; i < PoolSize; ++i)
	{
		AStackablePackage* Package = GetWorld()->SpawnActor<AStackablePackage>(PackageClass, FVector(), FRotator());
		Package->SetActorHiddenInGame(true);
		Package->SetActorEnableCollision(false);
		Package->PackagePool = this;
		Package->PackageSpawner = GetOwner<APackageSpawner>();
		Package->SetActorLocation(GetOwner()->GetActorLocation());
		PackagePool.Add(Package);
	}
}


AStackablePackage* UPackagePool::GetPackage()
{
	if(PackagePool.Num() == 0) return nullptr;
	AStackablePackage* Package = PackagePool.Pop();
	if(!Package) return nullptr;

	ActivePackages.Add(Package);
	Package->bActive = true;
	Package->SetActorHiddenInGame(false);
	Package->SetActorEnableCollision(true);

	return Package;
}

void UPackagePool::ReturnAllPackages()
{
	TArray<AStackablePackage*> TempArray = ActivePackages;
	for (const auto Package : TempArray)
	{
		ReturnPackage(Package);
	}
}

void UPackagePool::ReturnPackage(AStackablePackage* Package)
{
	Package->SetActorHiddenInGame(true);
	Package->SetActorEnableCollision(false);
	ActivePackages.Remove(Package);
	PackagePool.Push(Package);
}




