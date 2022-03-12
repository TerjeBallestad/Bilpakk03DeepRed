// Fill out your copyright notice in the Description page of Project Settings.


#include "BilpakkGameState.h"


#include "BilpakkGameModeBase.h"
#include "BilpakkSave.h"
#include "PlayfieldContainer.h"
#include "VRCarpakPawn.h"
#include "OculusMRFunctionLibrary.h"
#include "AndroidPermissionFunctionLibrary.h"
#include "Components/AudioComponent.h"

void ABilpakkGameState::TogglePause()
{
	if(IsFinished) return;
	if(IsPaused)
	{
		// Unpause
		UIPawn->SetActorHiddenInGame(true);
		StackingPawn->SetActorHiddenInGame(false);
		IsPaused = false;
		PC->Possess(StackingPawn);
		OnUnPausedGame.Broadcast();
	} else
	{
		// Pause
		UIPawn->SetActorTransform(StackingPawn->GetActorTransform());
		UIPawn->SetActorHiddenInGame(false);
		StackingPawn->SetActorHiddenInGame(true);
		IsPaused = true;
		PC->Possess(UIPawn);
		OnPausedGame.Broadcast();
	}
}

void ABilpakkGameState::FinishGame()
{
	//UAndroidPermissionFunctionLibrary::AcquirePermissions(MyPermissionArray);
	UBilpakkSave* Save = UBilpakkSave::Load();
	Save->AddHighscore(ABilpakkGameState::GetPoints(GetWorld()), RowName);
	Save->Save();
	UIPawn->SetActorTransform(StackingPawn->GetActorTransform());
	UIPawn->SetActorHiddenInGame(false);
	StackingPawn->SetActorHiddenInGame(true);
	IsFinished = true;
	PC->Possess(UIPawn);
	AudioComponent->Stop();
	OnFinishedGame.Broadcast();
}

void ABilpakkGameState::PlayMusic()
{
	if(AudioComponent && !AudioComponent->IsPlaying())
	{
		AudioComponent->SetSound(Music);
		AudioComponent->Play();
	}
		
}

void ABilpakkGameState::StartGame(FName Row)
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
	if(!UIPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("No UI pawn"))
		UIPawn = GetWorld()->SpawnActor<AVRCarpakPawn>(UIPawnClass);
		UIPawn->InitializeEvents(this);
		UIPawn->Setup();
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
	
	UIPawn->SetActorHiddenInGame(true);
	StackingPawn->SetActorHiddenInGame(false);
	StackingPawn->SetupMap();
	IsPaused = false;
	PC->Possess(StackingPawn);
	OnNewGame.Broadcast();
	if(bFirstTime)
	{
		TogglePause();
		bFirstTime = false;
		IsFinished = true;
	} else
	{
		IsFinished = false;
		FTimerHandle Handle;
		GetWorldTimerManager().SetTimer(Handle, this, &ABilpakkGameState::PlayMusic, 10);
	}
}

void ABilpakkGameState::SetPoints(int32 Amount)
{
	Points = Amount + BonusPoints;
}

void ABilpakkGameState::AddBonusPoints(int32 Amount)
{
	BonusPoints += Amount;
}

void ABilpakkGameState::BeginPlay()
{
	Super::BeginPlay();

	UOculusMRFunctionLibrary::IsMrcEnabled();
	
	
	AActor* MusicActor = GetWorld()->SpawnActor<AActor>();
	AudioComponent = NewObject<UAudioComponent>(MusicActor);
	AudioComponent->RegisterComponent();
	AudioComponent->SetSound(Music);
}

int32 ABilpakkGameState::GetPoints(UObject* WorldContextObject) 
{
	if((!GEngine) && (!WorldContextObject)) return -1;

	ABilpakkGameState* State = Cast<ABilpakkGameState>(GEngine->
        GetWorldFromContextObjectChecked(WorldContextObject)->GetAuthGameMode()->GameState);
	if(State)
		return State->Points;
	else
		return -1;
}

FString ABilpakkGameState::GetRowName(UObject* WorldContextObject)
{
	if((!GEngine) && (!WorldContextObject)) return FString();

	ABilpakkGameState* State = Cast<ABilpakkGameState>(GEngine->
        GetWorldFromContextObjectChecked(WorldContextObject)->GetAuthGameMode()->GameState);
	if(State)
		return State->RowName;
	else
		return FString();
}

FBilpakkLevel ABilpakkGameState::GetLevelData(UObject * WorldContextObject)
{
	if((!GEngine) && (!WorldContextObject)) return FBilpakkLevel();

	ABilpakkGameState* State = Cast<ABilpakkGameState>(GEngine->
		GetWorldFromContextObjectChecked(WorldContextObject)->GetAuthGameMode()->GameState);
	if(State)
		return State->LevelData;
	else
		return FBilpakkLevel();
	
}
