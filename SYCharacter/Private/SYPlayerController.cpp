// Fill out your copyright notice in the Description page of Project Settings.


#include "SYPlayerController.h"

ASYPlayerController::ASYPlayerController()
{

}

void ASYPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ASYPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void ASYPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}