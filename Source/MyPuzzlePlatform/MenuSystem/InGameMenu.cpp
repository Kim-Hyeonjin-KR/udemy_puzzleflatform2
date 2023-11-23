// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "Components/Button.h"

bool UInGameMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;


	if (!ensure(InGameMenu_Cancle != nullptr)) return false;
	InGameMenu_Cancle->OnClicked.AddDynamic(this, &UInGameMenu::CloseInGameMenu);

	
	if (!ensure(InGameMenu_Quit != nullptr)) return false;
	InGameMenu_Quit->OnClicked.AddDynamic(this, &UInGameMenu::QuitInGame);




	return true;
}

void UInGameMenu::CloseInGameMenu()
{
	Teardown();
	UE_LOG(LogTemp, Warning, TEXT("CloseInGameMenu"));
}

void UInGameMenu::QuitInGame()
{

	UE_LOG(LogTemp, Warning, TEXT("QuitInGame"));
}

void UInGameMenu::LoadInGameMenu()
{
	this->Setup();
}
