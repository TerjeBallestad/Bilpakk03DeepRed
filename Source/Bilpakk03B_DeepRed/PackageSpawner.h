// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StackablePackage.h"
#include "PackageGrid.h"
#include "Components/Image.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "PackageSpawner.generated.h"
USTRUCT(BlueprintType)
struct FBilpakkLevel: public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bilpakk Level")
	FText LevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bilpakk Level")
	int32 Difficulty;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bilpakk Level")
	FGridParameters GridParameters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bilpakk Level")
	TMap<EPackageType, FPackageAmounts> PackageAmounts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bilpakk Level")
	UStaticMesh* CarMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bilpakk Level")
	TMap<EPackageSize, FPackageParameters> PackageLibrary;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bilpakk Level")
	TArray<FGridRange> Doors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bilpakk Level")
	TAssetPtr<UTexture2D> MapImage;

	
};
UCLASS()
class BILPAKK03B_DEEPRED_API APackageSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	APackageSpawner();

	UFUNCTION(BlueprintNativeEvent)
	void InitializeEvents(class ABilpakkGameState *State);
	
	UPROPERTY(BlueprintReadOnly)
	ABilpakkGameState* GameState;

	UFUNCTION(BlueprintCallable)
	void Setup(FBilpakkLevel Data);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FPackageParameters> SpawnQueue;

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static int32 GetRemainingPackageAmount(UObject* WorldContextObject);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UPackagePool* PackagePool;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AStackablePackage *NextPackage;
	
	UFUNCTION(BlueprintImplementableEvent)
	void Finish();

	UFUNCTION(BlueprintImplementableEvent)
    void NewGame();

	UFUNCTION(BlueprintImplementableEvent)
    void Pause();

	UFUNCTION(BlueprintImplementableEvent)
    void UnPause();

	UFUNCTION(BlueprintCallable)
	void UpdateNextPackage();
	
	UFUNCTION(BlueprintCallable)
	class AStackablePackage* GetNextPackage();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FTransform PackageSpawnLocation;
	
private:
	
	UPROPERTY(EditDefaultsOnly)
	FTransform SpawnLocation;

	UFUNCTION(BlueprintCallable)
	bool RemoveFirstPackageFromQueue();

	// Components
	
	UPROPERTY(VisibleAnywhere)
	TMap<EPackageType, UMaterialInstance*> Materials;

	UPROPERTY(VisibleAnywhere)
	FColorLibrary ColorLibrary = FColorLibrary();

	UPROPERTY(EditAnywhere)
	UMaterialInterface* MaterialClass;

	UPROPERTY(VisibleAnywhere)
	TMap<EPackageSize, FPackageParameters> PackageLibrary;
	
	
	void ShuffleArray(TArray<FPackageParameters>& Array) const;
};
