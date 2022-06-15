// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorPool.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BILPAKK03B_DEEPRED_API UActorPool : public UActorComponent
{
	GENERATED_BODY()

public:	
	UActorPool();

protected:
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetPooledActor();

	void InitializeNewPooledActor();

	UFUNCTION(BlueprintCallable)
	void ClearStackedPackages();

private:
	UPROPERTY(VisibleAnywhere)
	TArray<UStaticMeshComponent*> ActorPool;

	UPROPERTY(VisibleAnywhere)
	TArray<UStaticMeshComponent*> ActivePackages;

	UPROPERTY(EditAnywhere)
	int PreWarmed = 4;
};