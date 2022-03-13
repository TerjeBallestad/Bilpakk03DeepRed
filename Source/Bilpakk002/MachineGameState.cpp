// Fill out your copyright notice in the Description page of Project Settings.


#include "MachineGameState.h"
#include "PlayfieldContainer.h"


void AMachineGameState::StartGame(FName Row)
{
	Points = 0;
	BonusPoints = 0;
	if(Row == "")
	{
		UE_LOG(LogTemp, Warning, TEXT("No Row Name in Start Game"))
		Row = LevelDataTable->GetRowNames()[0];
	}
	UE_LOG(LogTemp, Warning, TEXT("Starting row: %s"), *Row.ToString());
	
	LevelData = *LevelDataTable->FindRow<FBilpakkLevel>(Row, "");
	RowName = Row.ToString();
	
	if(!PC)
	{
		PC = GetWorld() != nullptr ? GetWorld()->GetFirstPlayerController<APlayerController>() : nullptr;
		if(PC == nullptr) UE_LOG(LogTemp, Error, TEXT("Player controller is null --- Game State"))
	}
	if(!StackingPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Stacking pawn"))
		StackingPawn = Cast<AVRCarpakPawn>(PC->GetPawn());
		StackingPawn->InitializeEvents(this);
		StackingPawn->Setup();
	}
	if(!PackageSpawner)
	{
		PackageSpawner = GetWorld()->SpawnActor<APackageSpawner>(PackageSpawnerClass);
		PackageSpawner->InitializeEvents(this);
	}
	if(!Playfield)
	{
		Playfield = GetWorld()->SpawnActor<APlayfieldContainer>(PlayfieldClass);
		Playfield->InitializeEvents(this);
	}

	//StackingPawn->SetupMap();
	IsPaused = false;
	OnNewGame.Broadcast();
}
