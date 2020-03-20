// Fill out your copyright notice in the Description page of Project Settings.


#include "DnE_SaveGame.h"

UDnE_SaveGame::UDnE_SaveGame()
{
	DneLevelName = TEXT("Title");
	CurrentHP = 100;
	IsBowavailable = false;
	IsBombavailable = false;
	iPotionNum = 0;
	///UI PART///

	AchievementDataMap.Add(500, false);
	AchievementDataMap.Add(501, false);
	AchievementDataMap.Add(502, false);
	AchievementDataMap.Add(503, false);
	AchievementDataMap.Add(504, false);
	AchievementDataMap.Add(505, false);
	AchievementDataMap.Add(506, false);

	///UI PART END///


}

///UI PART///


bool UDnE_SaveGame::GetCurrentPlayerAchievement(int AchievementID)
{
	if (AchievementDataMap.Contains(AchievementID))
	{
		return AchievementDataMap[AchievementID];
	}
	else
	{
		return false;
	}
}

bool UDnE_SaveGame::SetCurrentPlayerAchievement(int AchievementID)
{
	if (AchievementDataMap.Contains(AchievementID))
	{
		AchievementDataMap.Emplace(AchievementID, true);
		return true;
	}
	else
	{
		return false;
	}
}

///UI PART END///
