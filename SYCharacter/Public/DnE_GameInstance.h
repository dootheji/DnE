// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DnE_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SYCHARACTER_API UDnE_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Save)
	FString SlotName;
	virtual void Init() override;
	
};
