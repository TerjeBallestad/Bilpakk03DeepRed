// Fill out your copyright notice in the Description page of Project Settings.


#include "PackagePool.h"
#include "StackablePackage.h"

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
		Package->PackagePool = this;
		Packages.Add(Package);
	}
}


AStackablePackage* UPackagePool::GetPackage()
{
	if(Packages.Num() == 0) return nullptr;
	AStackablePackage* Package = Packages.Pop();
	if(!Package) return nullptr;

	Package->bActive = true;
	Package->SetActorHiddenInGame(false);

	return Package;
}

void UPackagePool::ReturnPackage(AStackablePackage* Package)
{
	Package->SetActorHiddenInGame(true);
	Packages.Add(Package);
}




