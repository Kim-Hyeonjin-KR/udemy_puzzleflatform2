// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"


UPuzzlePlatformGameInstance::UPuzzlePlatformGameInstance(const FObjectInitializer& ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Construct"));

	static ConstructorHelpers::FClassFinder<UUserWidget> WBP_MainMenuClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(WBP_MainMenuClass.Class != nullptr)) return;

	MainMenu_Class = WBP_MainMenuClass.Class;
}

void UPuzzlePlatformGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Initttttttttttttt"));

	UE_LOG(LogTemp, Warning, TEXT("Found Class Name : %s"), *MainMenu_Class->GetName());
}

void UPuzzlePlatformGameInstance::LoadMenu()
{
	if (!ensure(MainMenu_Class != nullptr)) return; 
	
	Menu = CreateWidget<UMainMenu>(this, MainMenu_Class);
	if (!ensure(Menu != nullptr)) return;

	Menu->Setup();
	Menu->SetMenuInterface(this);
}

void UPuzzlePlatformGameInstance::Host()
{
	if (Menu != nullptr)
	{
		Menu->Teardown();
	}


	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = UGameInstance::GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");


}

void UPuzzlePlatformGameInstance::Join(const FString& Address)
{
	if (Menu != nullptr)
	{
		Menu->Teardown();
	}




	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 6, FColor::Green, FString::Printf(TEXT("Join %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);

}
