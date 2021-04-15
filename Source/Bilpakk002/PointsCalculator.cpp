// Fill out your copyright notice in the Description page of Project Settings.


#include "PointsCalculator.h"



#include "DrawDebugHelpers.h"
#include "PackageGrid.h"

bool FPackageChunks::Contains(FIntVector Position)
{
	for (auto Chunk: this->Chunks)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Comparing %s"), *Position.ToString());
		if(Chunk.Max.X > Position.X && Chunk.Min.X <= Position.X &&
			Chunk.Max.Y > Position.Y && Chunk.Min.Y <= Position.Y &&
            Chunk.Max.Z > Position.Z && Chunk.Min.Z <= Position.Z)
		{
			return true;
		}
	}
	return false;
}

UPointsCalculator::UPointsCalculator()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UPointsCalculator::Setup(TArray<EPackageType> Colors, UPackageGrid* Grid, TArray<FGridRange> InDoors)
{
	for (auto Color : Colors)
	{
		PackageClusters.Add(Color);
	}
	PackageGrid = Grid;
	Doors = InDoors;
}

int32 UPointsCalculator::CalculateEndGamePoints()
{
	int32 Points = 0;

	// Only for debug
	/*for (auto Door : Doors)
	{
		for (int X = Door.Min.X; X < Door.Max.X; ++X)
		{
			for (int Y = Door.Min.Y; Y < Door.Max.Y; ++Y)
			{
				for (int Z = Door.Min.Z; Z < Door.Max.Z; ++Z)
				{
					FIntVector CurrentPosition(X,Y,Z);
					DrawDebugBox(GetWorld(), PackageGrid->GridToWorldLocation(CurrentPosition), PackageGrid->GridTransform.GetScale3D() * FVector(0.5, 0.5, 0.5),GetOwner()->GetActorRotation().Quaternion(), FColor(250,50,55), false, 0, 2);	
				}
			}
		}
	}
	*/
	
	for(EPackageType Color: TEnumRange<EPackageType>())
	{
		for (auto Package : PackageClusters[Color].Chunks)
		{
			// Check above
			FGridRange AboveRange(Package);
			AboveRange.Min.Z = Package.Max.Z;
			AboveRange.Max.Z = PackageGrid->Size.Z;
			if(! PackageGrid->CheckRangeVacantOrColor(AboveRange, Color)) continue;
			
			// Find Closest door
			int32 ShortestDistance = 1000;
			FIntVector ClosestPosition;
			FGridRange ClosestDoor;
			for (int32 i = 0; i < Doors.Num(); ++i)
			{
				for (int32 X = Doors[i].Min.X; X < Doors[i].Max.X; ++X)
				{
					for (int32 Y = Doors[i].Min.Y; Y < Doors[i].Max.Y; ++Y)
					{
						int32 Distance = abs(X - Package.Min.X) + abs(Y - Package.Min.Y);
						if(Distance < ShortestDistance)
						{
							ShortestDistance = Distance;
							ClosestPosition = FIntVector(X, Y, Package.Min.Z);
							ClosestDoor = Doors[i];
						}
					}
				}	
			}
			//DrawDebugBox(GetWorld(), PackageGrid->GridToWorldLocation(ClosestPosition), PackageGrid->GridTransform.GetScale3D() * FVector(0.4, 0.4, 0.4),GetOwner()->GetActorRotation().Quaternion(), FColor(25,25,255), false, 0, 2);					
			//DrawDebugBox(GetWorld(), PackageGrid->GridToWorldLocation(Package.Min), PackageGrid->GridTransform.GetScale3D() * FVector(0.5, 0.5, 0.5),GetOwner()->GetActorRotation().Quaternion(), FColor(250,250,255), false, 0, 2);					
			FIntVector PackageSize = Package.Max - Package.Min;

			// Straight line 
			int32 WalkX = ClosestPosition.X - Package.Min.X;
			int32 WalkY = ClosestPosition.Y - Package.Min.Y;

			// TODO Probably need more offset states
			// offset x
			if(WalkX > 0 && ClosestPosition.X + PackageSize.X >= ClosestDoor.Max.X)
			{
				ClosestPosition.X -= PackageSize.X + ClosestPosition.X - ClosestDoor.Max.X;
			}
			else if(WalkX < 0 && ClosestPosition.X - PackageSize.X <= ClosestDoor.Min.X)
			{
				// Do nothing?
			}

			// offset y
			if(WalkY > 0 && ClosestPosition.Y + PackageSize.Y >= ClosestDoor.Max.Y)
			{
				ClosestPosition.Y -= PackageSize.Y + ClosestPosition.Y - ClosestDoor.Max.Y;
			}
			else if(WalkY < 0 && ClosestPosition.Y + PackageSize.Y >= ClosestDoor.Max.Y)
			{
				ClosestPosition.Y -= ClosestPosition.Y + PackageSize.Y - ClosestDoor.Max.Y;
			}

			WalkX = ClosestPosition.X - Package.Min.X;
			WalkY = ClosestPosition.Y - Package.Min.Y;
			
			//DrawDebugBox(GetWorld(), PackageGrid->GridToWorldLocation(ClosestPosition), PackageGrid->GridTransform.GetScale3D() * FVector(0.4, 0.4, 0.4),GetOwner()->GetActorRotation().Quaternion(), FColor(25,250,25), false, 0, 2);					

			//move up, max = pack height
			for (int32 Z = 0; Z <  PackageSize.Z + 1; ++Z)
			{
				FGridRange WalkRange = Package;
				WalkRange.Min.Z += Z;
				WalkRange.Max.Z += Z;
				if(WalkY > 0)
				{
					WalkRange.Max.Y += WalkY; 
					WalkRange.Min.Y += PackageSize.Y;
				} else
				{
					WalkRange.Min.Y += WalkY;
					WalkRange.Max.Y -= PackageSize.Y;
				}
				if	(!PackageGrid->CheckRangeVacantOrColor(WalkRange, Color)) continue;

				WalkRange = Package;
				WalkRange.Max.Y += WalkY;
				WalkRange.Min.Y += WalkY;
				WalkRange.Min.Z += Z;
				WalkRange.Max.Z += Z;

				if(WalkX > 0)
				{
					WalkRange.Max.X += WalkX;
					WalkRange.Min.X += PackageSize.X;
				} else
				{
					WalkRange.Min.X += WalkX;
					WalkRange.Max.X -= PackageSize.X;
				}

				if	(!PackageGrid->CheckRangeVacantOrColor(WalkRange, Color)) continue;

				Points += 10 * PackageSize.X * PackageSize.Y * PackageSize.Z;
				break;
			}
		}
	}
	return Points;
}

int32 UPointsCalculator::CalculatePlacePackagePoints(FGridRange Range, EPackageType Color)
{
	int32 Points = 0;
	FIntVector CurrentPositon;
	for (int32 X = Range.Min.X; X < Range.Max.X; ++X)
	{
		for (int32 Y = Range.Min.Y; Y < Range.Max.Y; ++Y)
		{
			CurrentPositon = FIntVector(X,Y, Range.Min.Z -1);
			
			//UE_LOG(LogTemp, Warning, TEXT("Current position: %s"), *CurrentPositon.ToString());

			if(PackageClusters[Color].Contains(CurrentPositon))
			{
				//UE_LOG(LogTemp, Warning, TEXT("Add points"));
				//DrawDebugBox(GetWorld(), PackageGridGrid->GridToWorldLocation(CurrentPositon), PackageGridGrid->GridTransform.GetScale3D() * FVector(0.5, 0.5, 0.5),GetOwner()->GetActorRotation().Quaternion(), FColor(250,50,55), false, 5, 2);	
				Points += 10;
			}
			CurrentPositon = FIntVector(X,Y, Range.Max.Z);
			//UE_LOG(LogTemp, Warning, TEXT("Current position: %s"), *CurrentPositon.ToString());
			if(PackageClusters[Color].Contains(CurrentPositon))
			{
				//UE_LOG(LogTemp, Warning, TEXT("Add points"));
				//DrawDebugBox(GetWorld(), PackageGridGrid->GridToWorldLocation(CurrentPositon), PackageGridGrid->GridTransform.GetScale3D() * FVector(0.5, 0.5, 0.5),GetOwner()->GetActorRotation().Quaternion(), FColor(250,50,55), false, 5, 2);	
				Points += 10;
			}
		}
	}
	for (int32 X = Range.Min.X; X < Range.Max.X; ++X)
	{
		for (int32 Z = Range.Min.Z; Z < Range.Max.Z; ++Z)
		{
			CurrentPositon = FIntVector(X,Range.Min.Y - 1, Z);
			//UE_LOG(LogTemp, Warning, TEXT("Current position: %s"), *CurrentPositon.ToString());

			if(PackageClusters[Color].Contains(CurrentPositon))
			{
				//UE_LOG(LogTemp, Warning, TEXT("Add points"));
				//DrawDebugBox(GetWorld(), PackageGridGrid->GridToWorldLocation(CurrentPositon), PackageGridGrid->GridTransform.GetScale3D() * FVector(0.5, 0.5, 0.5),GetOwner()->GetActorRotation().Quaternion(), FColor(250,50,55), false, 5, 2);	
				Points += 10;
			}
			CurrentPositon = FIntVector(X,Range.Max.Y, Z);
			//UE_LOG(LogTemp, Warning, TEXT("Current position: %s"), *CurrentPositon.ToString());
			if(PackageClusters[Color].Contains(CurrentPositon))
			{
				//UE_LOG(LogTemp, Warning, TEXT("Add points"));
				//DrawDebugBox(GetWorld(), PackageGridGrid->GridToWorldLocation(CurrentPositon), PackageGridGrid->GridTransform.GetScale3D() * FVector(0.5, 0.5, 0.5),GetOwner()->GetActorRotation().Quaternion(), FColor(250,50,55), false, 5, 2);	
				Points += 10;
			}
		}
	}
	for (int32 Y = Range.Min.Y; Y < Range.Max.Y; ++Y)
	{
		for (int32 Z = Range.Min.Z; Z < Range.Max.Z; ++Z)
		{
			CurrentPositon = FIntVector(Range.Min.X - 1,Y, Z);
			//UE_LOG(LogTemp, Warning, TEXT("Current position: %s"), *CurrentPositon.ToString());
			if(PackageClusters[Color].Contains(CurrentPositon))
			{
				//UE_LOG(LogTemp, Warning, TEXT("Add points"));
				//DrawDebugBox(GetWorld(), PackageGridGrid->GridToWorldLocation(CurrentPositon), PackageGridGrid->GridTransform.GetScale3D() * FVector(0.5, 0.5, 0.5),GetOwner()->GetActorRotation().Quaternion(), FColor(250,50,55), false, 5, 2);	
				Points += 10;
			}
			CurrentPositon = FIntVector(Range.Max.X, Y, Z);
			//UE_LOG(LogTemp, Warning, TEXT("Current position: %s"), *CurrentPositon.ToString());
			if(PackageClusters[Color].Contains(CurrentPositon))
			{
				//UE_LOG(LogTemp, Warning, TEXT("Add points"));
				//DrawDebugBox(GetWorld(), PackageGridGrid->GridToWorldLocation(CurrentPositon), PackageGridGrid->GridTransform.GetScale3D() * FVector(0.5, 0.5, 0.5),GetOwner()->GetActorRotation().Quaternion(), FColor(250,50,55), false, 5, 2);	
				Points += 10;
			}
		}
	}
	//DrawDebugBox(GetWorld(), PackageGridGrid->GridToWorldLocation(Range.Min), PackageGridGrid->GridTransform.GetScale3D() * FVector(0.5, 0.5, 0.5),GetOwner()->GetActorRotation().Quaternion(), FColor(50,250,55), false, 5, 2);	
	//DrawDebugBox(GetWorld(), PackageGridGrid->GridToWorldLocation(Range.Max), PackageGridGrid->GridTransform.GetScale3D() * FVector(0.5, 0.5, 0.5),GetOwner()->GetActorRotation().Quaternion(), FColor(50,250,55), false, 5, 2);	

	//UE_LOG(LogTemp, Warning, TEXT("Adding range: min %s  max %s"), *Range.Min.ToString(), *Range.Max.ToString());
	PackageClusters[Color].Chunks.Add(Range);
	
	return Points;
}

void UPointsCalculator::DrawDebugRange(FIntVector Range)
{
	
}





