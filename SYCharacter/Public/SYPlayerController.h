// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SYCharacter.h"
#include "GameFramework/PlayerController.h"
#include "SYPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SYCHARACTER_API ASYPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ASYPlayerController();

	//얘들은 왜 필요한건지 모르겠음
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn)	override;

protected:
	virtual void BeginPlay() override;
};
