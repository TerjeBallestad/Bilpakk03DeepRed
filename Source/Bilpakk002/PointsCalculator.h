// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ActorPool.h"
#include "PackageGrid.h"
#include "Components/ActorComponent.h"
#include "PointsCalculator.generated.h"

FORCEINLINE    bool operator==(const FGridRange &Lhs, const FGridRange& Rhs)
{
	return Lhs.Max == Rhs.Max && Lhs.Min == Rhs.Min;
}

USTRUCT(BlueprintType)
struct FPackageChunks
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Points Calculator")
	TArray<FGridRange> Chunks;

	bool Contains(FIntVector Position);
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BILPAKK002_API UPointsCalculator : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPointsCalculator();

	virtual void BeginPlay() override;

	void Setup(TArray<EPackageType> Colors, UPackageGrid* Grid, TArray<FGridRange> InDoors);
	void SpawnNegativeIndicator(FGridRange Package);

	UFUNCTION(BlueprintCallable)
	int32 CalculateEndGamePoints();

	UFUNCTION(BlueprintCallable)
	int32 CalculatePlacePackagePoints(FGridRange Range, EPackageType Color);

private:
	void DrawDebugRange(FIntVector Range);

	UPROPERTY()
	UActorPool* MeshPool;

	UFUNCTION()
	void SetMeshPoolInvisible();
	
	UPROPERTY()
	UPackageGrid* PackageGrid;

	UPROPERTY()
	TArray<FGridRange> Doors;
	
	UPROPERTY(VisibleAnywhere)
	TMap<EPackageType, FPackageChunks> PackageClusters;

	UPROPERTY()
	TArray<FGridRange> PositivePackages;

	UPROPERTY()
	TArray<FGridRange> NegativePackages;

	UPROPERTY(EditDefaultsOnly)
	UMaterial* NegativeMaterial;

	UPROPERTY(EditDefaultsOnly)
	UMaterial* PositiveMaterial;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* CubeMesh;
	
};


