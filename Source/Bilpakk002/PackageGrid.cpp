// Fill out your copyright notice in the Description page of Project Settings.


#include "PackageGrid.h"

#include "DrawDebugHelpers.h"
#include "StackablePackage.h"

UPackageGrid::UPackageGrid()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UPackageGrid::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GridTransform.SetRotation(GetOwner()->GetActorTransform().GetRotation());
	GridTransform.SetLocation(GetOwner()->GetActorTransform().TransformPosition(FVector(Size.X -1, Size.Y-1, Size.Z-1) * CellSize * -0.5));

	if(bDebugGrid)
	{
		UpdateDebug();
	}
}	

void UPackageGrid::Setup(FGridParameters Parameters, FTransform LocalOffset = FTransform::Identity)
{
	Size.X = Parameters.Size.X;
	Size.Y = Parameters.Size.Y;
	Size.Z = Parameters.Size.Z;
	CellSize = Parameters.CellSize;
	
	for (int x = 0; x < Size.X; ++x)
	{
		for (int y = 0; y < Size.Y; ++y)
		{
			for (int z = 0; z < Size.Z; ++z)
			{
				FIntVector GridLocation = FIntVector(x, y, z);
				Grid.Add(GridLocation, EPackageType::Vacant);				
			}
		}
	}
	GridTransform.SetScale3D(FVector::OneVector * CellSize);
	GridTransform.SetRotation(GetOwner()->GetActorTransform().GetRotation() + LocalOffset.GetRotation());
	GridTransform.SetLocation(LocalOffset.GetLocation() + GetOwner()->GetActorTransform().TransformPosition(FVector(Size.X -1, Size.Y-1, Size.Z-1) * CellSize * -0.5));
	for (FGridRange Range : Parameters.DefaultOccupiedGridPositions)
	{
		SetStatus(Range, EPackageType::Occupied);	
	}
}

bool UPackageGrid::FindSpaceForPackage(FTransform PackageTransform, FGridRange& OutRange, FTransform& OutTransform)
{
	

	// DrawDebugPoint(GetWorld(), GridToWorldLocation(IntMin), 20.0f, FColor(255,0,255,255), false, 0, 1);	
	// DrawDebugPoint(GetWorld(), GridToWorldLocation(IntMax), 20.0f, FColor(0,255,255,255), false, 0, 1);	

	const FIntVector Offset = CalculatePackageOffset(OutRange);
	OutRange.Min += Offset;
	OutRange.Max += Offset;
		
	FIntVector FoundLocation;

	if (FindAvailableGridPosition(WorldToGridLocation(PackageTransform.GetLocation()) + Offset, FoundLocation,OutRange))
	{
		const FVector PreviewLocation = GridToWorldLocation(FoundLocation);
		OutTransform.SetLocation(PreviewLocation);
		OutTransform.SetRotation(SnapRotationToGrid(PackageTransform.GetRotation().Rotator()));
		return true;
	}
	return false;
}

bool UPackageGrid::FindAvailableGridPosition(FIntVector StartGridLocation, FIntVector& ResultPosition, FGridRange& OutRange)
{
	const FIntVector pSize = OutRange.Max - OutRange.Min;
	const int32 zSize = StartGridLocation.Z - OutRange.Min.Z;
	for (int32 z = 0; z < Size.Z - pSize.Z + 1; ++z)
	{
		OutRange.Min.Z = z;
		OutRange.Max.Z = z + pSize.Z;

		if(!CheckRangeVacant(OutRange))
		{
			continue;
		}
		
		ResultPosition = StartGridLocation;
		ResultPosition.Z = OutRange.Min.Z + zSize;
		return true;
	}
	return false;
}

FVector UPackageGrid::SnapLocationToGrid(FVector WorldLocation)
{
	FVector LocalLocation = GridTransform.InverseTransformPosition(WorldLocation);
	int NewX = FMath::Clamp(FGenericPlatformMath::RoundToInt(LocalLocation.X), 0, Size.X);
	int NewY = FMath::Clamp(FGenericPlatformMath::RoundToInt(LocalLocation.Y), 0, Size.Y);
	int NewZ = FMath::Clamp(FGenericPlatformMath::RoundToInt(LocalLocation.Z), 0, Size.Z);
	
	return GridTransform.TransformPosition(FVector(NewX, NewY, NewZ));
}

FQuat UPackageGrid::SnapRotationToGrid(FRotator Rotator)
{
	FQuat LocalRotation = GridTransform.InverseTransformRotation(Rotator.Quaternion());
	LocalRotation = SnapRotation(LocalRotation.Rotator());
	return GridTransform.TransformRotation(LocalRotation);
}

FQuat UPackageGrid::SnapRotationToGridLocal(FRotator Rotator)
{
	FQuat LocalRotation = GridTransform.InverseTransformRotation(Rotator.Quaternion());
	return SnapRotation(LocalRotation.Rotator());
}


FQuat UPackageGrid::SnapRotation(FRotator Rotation)
{
	const float Roll = SnapRotationAxis(Rotation.Roll);
	const float Pitch = SnapRotationAxis(Rotation.Pitch);
	const float Yaw = SnapRotationAxis(Rotation.Yaw);
	return FRotator(Pitch, Yaw, Roll).Quaternion(); 
}

float UPackageGrid::SnapRotationAxis(const float Degrees)
{
	return round(Degrees / 90) * 90; 
}

FIntVector UPackageGrid::WorldToGridLocation(const FVector WorldLocation) const
{
	const FVector LocalLocation = GridTransform.InverseTransformPosition(WorldLocation);

	const int NewX = FMath::Clamp(FMath::RoundToInt(LocalLocation.X), 0, Size.X - 1);
	const int NewY = FMath::Clamp(FMath::RoundToInt(LocalLocation.Y), 0, Size.Y - 1);
	const int NewZ = FMath::Clamp(FMath::RoundToInt(LocalLocation.Z), 0, Size.Z - 1);
	
	return FIntVector(NewX, NewY, NewZ);
}

FVector UPackageGrid::GridToWorldLocation(const FIntVector GridLocation) const
{
	return GridTransform.TransformPosition(FVector(GridLocation));
}

FIntVector UPackageGrid::RoundFVectorToIntVector(FVector Vector)
{
	FIntVector IntVector;
	IntVector.X = FMath::CeilToInt(Vector.X);
	IntVector.Y = FMath::CeilToInt(Vector.Y);
	IntVector.Z = FMath::CeilToInt(Vector.Z);
	return IntVector;
}

FIntVector UPackageGrid::CalculatePackageOffset(FGridRange Range)
{
	FIntVector Offset = FIntVector(0,0,0);

	// offset x
	if(Range.Min.X < 0)
	{
		Offset.X = FMath::Abs(Range.Min.X);
	}
	else if(Range.Max.X >= Size.X)
	{
		Offset.X -= Range.Max.X - Size.X;
	}
		
	// offset y
	if(Range.Min.Y < 0)
	{
		Offset.Y = FMath::Abs(Range.Min.Y);
	}
	else if(Range.Max.Y >= Size.Y)
	{
		Offset.Y -= Range.Max.Y - Size.Y;
	}
		
	// offset z
	if(Range.Min.Z < 0)
	{
		Offset.Z = FMath::Abs(Range.Min.Z);
	}
	else if(Range.Max.Z >= Size.Z)
	{
		Offset.Z -= Range.Max.Z - Size.Z;
	}
	return Offset;
}

bool UPackageGrid::CheckRangeVacant(FGridRange Range)
{
	for (int X = Range.Min.X; X < Range.Max.X; ++X)
	{
		for (int Y = Range.Min.Y; Y < Range.Max.Y; ++Y)
		{
			for (int Z = Range.Min.Z; Z < Range.Max.Z; ++Z)
			{
				FIntVector GridLocation(X,Y,Z);
				FVector PreviewLocation = GridToWorldLocation(GridLocation);
				if(!Grid.Contains(GridLocation))
				{
					if(bDebugGrid) DrawDebugBox(GetWorld(), PreviewLocation, GridTransform.GetScale3D() * FVector(0.5, 0.5, 0.5), GetOwner()->GetActorRotation().Quaternion(), FColor(250,50,55), false, 0, 2);	
					return false;
				}
				if(Grid[GridLocation] != EPackageType::Vacant)
				{
					if(bDebugGrid) DrawDebugBox(GetWorld(), PreviewLocation, GridTransform.GetScale3D() * FVector(0.5, 0.5, 0.5),GetOwner()->GetActorRotation().Quaternion(), FColor(250,50,55), false, 0, 2);	
					return false;
				}
				if(bDebugGrid) DrawDebugBox(GetWorld(), PreviewLocation, GridTransform.GetScale3D() * FVector(0.5, 0.5, 0.5),GetOwner()->GetActorRotation().Quaternion(), FColor(50,50,255), false, 0, 2);	
			}
		}
	}
	return true;
}

bool UPackageGrid::CheckRangeVacantOrColor(FGridRange Range, EPackageType Color)
{
	for (int X = Range.Min.X; X < Range.Max.X; ++X)
	{
		for (int Y = Range.Min.Y; Y < Range.Max.Y; ++Y)
		{
			for (int Z = Range.Min.Z; Z < Range.Max.Z; ++Z)
			{
				FIntVector GridLocation = FIntVector(X, Y, Z);
				if(!CheckCellVacantOrColor(GridLocation, Color))
				{	
					return false;
				}
			}
		}
	}
	return true;
}

bool UPackageGrid::CheckCellVacantOrColor(FIntVector Position, EPackageType Color)
{
	// The enum order is also the package order, so lower (int) enum will be removed first
	if(Grid.Contains(Position) && Grid[Position] > Color)
	{
		if(bDebugGrid) DrawDebugBox(GetWorld(), GridToWorldLocation(Position), GridTransform.GetScale3D() * FVector(0.4, 0.4, 0.4), GetOwner()->GetActorRotation().Quaternion(), FColor(250,50,55), false, 10, 2);	
		return false;
	}
	if(bDebugGrid) DrawDebugBox(GetWorld(), GridToWorldLocation(Position), GridTransform.GetScale3D() * FVector(0.4, 0.4, 0.4),GetOwner()->GetActorRotation().Quaternion(), FColor(50,50,255), false, 10, 2);	
	return true;
}

void UPackageGrid::SetStatus(FGridRange Range, EPackageType Status)
{
	for (int X = Range.Min.X; X < Range.Max.X; ++X)
	{
		for (int Y = Range.Min.Y; Y < Range.Max.Y; ++Y)
		{
			for (int Z = Range.Min.Z; Z < Range.Max.Z; ++Z)
			{
				FIntVector CurrentPosition(X,Y,Z);
				if(Grid.Contains(CurrentPosition))
					Grid[CurrentPosition] = Status;
			}
		}
	}
}

void UPackageGrid::CalculatePackageBounds(FTransform PackageTransform, FVector LocalBoundMin,FVector LocalBoundsMax, FGridRange& OutRange)
{
	FTransform ActivePackageTransform = PackageTransform;
	ActivePackageTransform.SetLocation( SnapLocationToGrid(ActivePackageTransform.GetLocation()));
	ActivePackageTransform.SetRotation(SnapRotationToGridLocal(ActivePackageTransform.GetRotation().Rotator()));
		
	LocalBoundMin = ActivePackageTransform.TransformVector(LocalBoundMin);
	LocalBoundsMax = ActivePackageTransform.TransformVector(LocalBoundsMax);

	if (LocalBoundMin.X > LocalBoundsMax.X)
	{
		float Temp = LocalBoundMin.X;
		LocalBoundMin.X = LocalBoundsMax.X;
		LocalBoundsMax.X = Temp;
	}

	if (LocalBoundMin.Y > LocalBoundsMax.Y)
	{
		float Temp = LocalBoundMin.Y;
		LocalBoundMin.Y = LocalBoundsMax.Y;
		LocalBoundsMax.Y = Temp;
	}

	if (LocalBoundMin.Z > LocalBoundsMax.Z)
	{
		float Temp = LocalBoundMin.Z;
		LocalBoundMin.Z = LocalBoundsMax.Z;
		LocalBoundsMax.Z = Temp;
	}
	
	OutRange.Min = RoundFVectorToIntVector(LocalBoundMin / CellSize);
	OutRange.Max = RoundFVectorToIntVector(LocalBoundsMax / CellSize);

	OutRange.Min += WorldToGridLocation(PackageTransform.GetLocation());
	OutRange.Max += WorldToGridLocation(PackageTransform.GetLocation());
}

void UPackageGrid::UpdateDebug()
{	
	for (int x = 0; x < Size.X; ++x)
	{
		for (int y = 0; y < Size.Y; ++y)
		{
			for (int z = 0; z < Size.Z; ++z)
			{
				FIntVector GridPosition(x, y, z);
				FVector WorldPosition = GridToWorldLocation(GridPosition);


				if (Grid[GridPosition] == EPackageType::Vacant)
				{
					DrawDebugPoint(GetWorld(), WorldPosition, 4.0f, FColor(255,255,255,50), false, 0, 1);	
				}
				else
				{
					DrawDebugSolidBox(GetWorld(), WorldPosition, GridTransform.GetScale3D() * FVector(0.25, 0.25, 0.25),GetOwner()->GetActorRotation().Quaternion(), ColorLibrary.Colors[Grid[GridPosition]], false, 0, 2);
				}
			}
		}
	}
}
