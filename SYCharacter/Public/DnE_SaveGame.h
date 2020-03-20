// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DnE_SaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SYCHARACTER_API UDnE_SaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UDnE_SaveGame();

	UPROPERTY()
		float CurrentHP;
	UPROPERTY()
		bool IsBowavailable;
	UPROPERTY()
		bool IsBombavailable;
	UPROPERTY()
		FString DneLevelName;
	UPROPERTY()
		int iPotionNum;
	//UIPart SaveData
	UPROPERTY()
		TMap<int, bool> AchievementDataMap;

public:
	UFUNCTION(BlueprintCallable)
	bool GetCurrentPlayerAchievement(int AchievementID);

	UFUNCTION(BlueprintCallable)
	bool SetCurrentPlayerAchievement(int AchievementID);


};
