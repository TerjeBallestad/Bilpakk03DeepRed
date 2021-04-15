// Fill out your copyright notice in the Description page of Project Settings.


#include "BilpakkSave.h"



#include "Kismet/GameplayStatics.h"

const FString UBilpakkSave::SLOT_NAME = "A";


UBilpakkSave* UBilpakkSave::Create()
{
	UBilpakkSave* NewSave = Cast<UBilpakkSave>(UGameplayStatics::CreateSaveGameObject(StaticClass()));
	if(!NewSave->Save()) return nullptr;
	return  NewSave;
}

UBilpakkSave* UBilpakkSave::Load()
{
	UBilpakkSave* Save = Cast<UBilpakkSave>(UGameplayStatics::LoadGameFromSlot(SLOT_NAME, 0));
	if(!Save)
	{
		Save = Create();
	}

	return Save;
}

bool UBilpakkSave::Save()
{
	return UGameplayStatics::SaveGameToSlot(this, SLOT_NAME, 0);
}

TArray<int32> UBilpakkSave::GetHighscores(FString InRowName)
{
	for (int i = 0; i < Highscores.Num(); ++i)
	{
		if(Highscores[i].RowName == InRowName)
		{
			return Highscores[i].Scores;
		}
	}
	TArray<int32> Scores;
	Scores.Init(0,5);
	FHighscore Highscore;
	Highscore.bComplete = false;
	Highscore.Scores = Scores;
	Highscore.RowName = InRowName;
	Highscores.Add(Highscore);
	return Scores;
}

void UBilpakkSave::AddHighscore(int32 InScore, FString InRowName)
{
	for (int32 i = 0; i < Highscores.Num(); ++i)
	{
		if(InRowName == Highscores[i].RowName)
		{
			for (int32 j = 0; j < Highscores[i].Scores.Num(); ++j)
			{
				if(Highscores[i].Scores[j] < InScore)
				{
					Highscores[i].Scores.Insert(InScore, j);
					Highscores[i].Scores.SetNum(5);
					Save();
					return;
				}
			}
			return;
		}
	}
	FHighscore NewHighscore;
	NewHighscore.bComplete = true;
	NewHighscore.RowName = InRowName;
	TArray<int32> Scores;
	Scores.Init(0,5);
	Scores[0] = InScore;
	NewHighscore.Scores = Scores;
	Highscores.Add(NewHighscore);
	Save();
	
}
