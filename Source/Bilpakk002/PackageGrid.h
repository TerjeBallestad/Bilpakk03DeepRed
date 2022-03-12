// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "StackablePackage.h"
#include "Components/ActorComponent.h"
#include "PackageGrid.generated.h"


USTRUCT(BlueprintType)
struct FGridRange
{
	GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FIntVector Min;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FIntVector Max;
};

USTRUCT(BlueprintType)
struct FGridParameters
{
	GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	FIntVector Size;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	float CellSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	TArray<FGridRange> DefaultOccupiedGridPositions;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BILPAKK002_API UPackageGrid : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPackageGrid();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	TMap<FIntVector, EPackageType> GetGrid() const { return Grid; } 
	FIntVector WorldToGridLocation(FVector WorldLocation) const;

	UFUNCTION(BlueprintCallable)
	FVector GridToWorldLocation(FIntVector GridLocation) const;

	FIntVector RoundFVectorToIntVector(FVector Vector);
	FVector SnapLocationToGrid(FVector WorldLocation);
	FIntVector CalculatePackageOffset(FGridRange Range);
	bool CheckRangeVacant(FGridRange Range);
	bool CheckRangeVacantOrColor(FGridRange Range, EPackageType Color);
	bool CheckCellVacantOrColor(FIntVector Position, EPackageType Color);

	UFUNCTION(BlueprintCallable)
	void SetStatus(FGridRange Range, EPackageType Status);
	
	UFUNCTION(BlueprintCallable)
	void Setup(FGridParameters Parameters, FTransform LocalOffset);

	UFUNCTION(BlueprintCallable)
	void CalculatePackageBounds(FTransform PackageTransform, FVector LocalBoundMin, FVector LocalBoundsMax,
	                            FGridRange& OutRange);

	UFUNCTION(BlueprintCallable)
	bool FindSpaceForPackage(FTransform PackageTransform, UPARAM(ref) FGridRange& OutRange,UPARAM(ref) FTransform& OutTransform);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FIntVector Size;

	UPROPERTY(VisibleAnywhere)
	float CellSize;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FTransform GridTransform;

	UFUNCTION(BlueprintCallable)
    void UpdateDebug();
	
    FQuat SnapRotationToGrid(FRotator Rotator);
	FQuat SnapRotationToGridLocal(FRotator Rotator);
	
private:
	static float SnapRotationAxis(float Degrees);
	FQuat SnapRotation(FRotator Rotation);
	bool FindAvailableGridPosition(FIntVector StartGridLocation, FIntVector& ResultPosition, FGridRange& OutRange);

	UPROPERTY(VisibleAnywhere)
	FColorLibrary ColorLibrary = FColorLibrary();
	
	UPROPERTY(EditAnywhere)
	bool bDebugGrid;
	
	UPROPERTY(VisibleAnywhere)
	TMap<FIntVector, EPackageType> Grid;
	
	

};
