// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "Components/Button.h"




bool UMainMenu::Initialize()
{
   bool Success = Super::Initialize();
   if (!Success) return false;

   if (!ensure(HostButton != nullptr)) return false;
   HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

   if (!ensure(JoinButton != nullptr)) return false;
   JoinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);


   UE_LOG(LogTemp, Warning, TEXT("Hostingggg21314124ggg~"));

   return true;
}

void UMainMenu::SetMenuInterface(IMenuInterface* _MenuInterface)
{
	this->MenuInterface = _MenuInterface;
}


void UMainMenu::HostServer()
{
	if (!ensure(MenuInterface != nullptr)) return;
	MenuInterface->Host();

	UE_LOG(LogTemp, Warning, TEXT("Hosting~"));
}


void UMainMenu::JoinServer()
{
	if (!ensure(MenuInterface != nullptr)) return;
	//MenuInterface->Join(const FString & Address);

	UE_LOG(LogTemp, Warning, TEXT("Joing~"));
}
