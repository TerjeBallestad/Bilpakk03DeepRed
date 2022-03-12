// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PackageSpawner.h"
#include "GameFramework/GameStateBase.h"
#include "BilpakkGameState.generated.h"

/**
 * 
 */
UCLASS()
class BILPAKK002_API ABilpakkGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	void SetPoints(int32 Amount);
	void AddBonusPoints(int32 Amount);
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void TogglePause();
		
	UFUNCTION(BlueprintCallable)
	void FinishGame();
	void PlayMusic();

	UFUNCTION(BlueprintCallable)
	void StartGame(FName Row);

	UPROPERTY(BlueprintReadWrite)
	bool IsPaused = false;

	UPROPERTY(BlueprintReadWrite)
	bool IsFinished = false;

	UPROPERTY(BlueprintAssignable)
	FComponentCustomStartSignature OnPausedGame;

	UPROPERTY(BlueprintAssignable)
	FComponentCustomStartSignature OnUnPausedGame;

	UPROPERTY(BlueprintAssignable)
	FComponentCustomStartSignature OnFinishedGame;

	UPROPERTY(BlueprintAssignable)
	FComponentCustomStartSignature OnNewGame;

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static int32 GetPoints(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static FString GetRowName(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static FBilpakkLevel GetLevelData(UObject* WorldContextObject);

	UPROPERTY(EditDefaultsOnly)
	USoundCue* Music;

	UPROPERTY(BlueprintReadOnly)
	UAudioComponent* AudioComponent;
	
	UPROPERTY(EditAnywhere)
	UDataTable* LevelDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString RowName;

	UPROPERTY(VisibleAnywhere)
	APlayerController* PC;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FBilpakkLevel LevelData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AVRCarpakPawn* UIPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AVRCarpakPawn> UIPawnClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class APackageSpawner *PackageSpawner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APackageSpawner> PackageSpawnerClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class APlayfieldContainer *Playfield;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APlayfieldContainer> PlayfieldClass;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AVRCarpakPawn* StackingPawn;
	
private:
	bool bFirstTime = true;
	int32 Points;
	int32 BonusPoints;	
};
