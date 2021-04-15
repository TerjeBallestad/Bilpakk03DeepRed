// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "PackageSpawner.h"
#include "Blueprint/UserWidget.h"
#include "RouteMap.generated.h"

/**
 * 
 */
UCLASS()
class BILPAKK002_API URouteMap : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void Setup();
	
};
