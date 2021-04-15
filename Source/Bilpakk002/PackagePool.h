// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "PackagePool.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FComponentCustomStartSignature);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BILPAKK002_API UPackagePool : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	UPackagePool();

protected:
	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintAssignable)
	FComponentCustomStartSignature OnPackageInteracted;

	UFUNCTION(BlueprintCallable)
	class AStackablePackage* GetPackage();

	UFUNCTION(BlueprintCallable)
	void ReturnAllPackages();
	
	void ReturnPackage(AStackablePackage* Package);

private:
	UPROPERTY()
	TArray<class AStackablePackage*> PackagePool;

	UPROPERTY()
	TArray<class AStackablePackage*> ActivePackages;

	UPROPERTY(EditAnywhere)
	int PoolSize =4;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AStackablePackage> PackageClass;
};
