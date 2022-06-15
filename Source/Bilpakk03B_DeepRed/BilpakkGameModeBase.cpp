// Copyright Epic Games, Inc. All Rights Reserved.


#include "BilpakkGameModeBase.h"


#include "BilpakkGameState.h"
#include "Kismet/GameplayStatics.h"


ABilpakkGameModeBase::ABilpakkGameModeBase()
{
	GameStateClass = ABilpakkGameState::StaticClass();
}

void ABilpakkGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	ABilpakkGameState* Gs = Cast<ABilpakkGameState>(GameState);
	if(Gs)
	{
		Gs->StartGame(FName(RowName));
		UE_LOG(LogTemp, Warning, TEXT("Row Name: %s --- Game Manager"), *RowName);
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("Game mode cast failed"));
	}
}

//This runs when loading a level
void ABilpakkGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	RowName = UGameplayStatics::ParseOption(Options, "RowName");
	
}
