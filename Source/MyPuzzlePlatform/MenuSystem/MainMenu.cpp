// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

//�÷��� ��Ŭ����
#include "UObject/ConstructorHelpers.h"


//���� ��Ŭ����
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

//������ ���� ��Ŭ����
#include "ServerRow.h"



// �Ű������� �� ������ �𸮾󿡼� �����ϴ� �Լ��� �ʱ�ȭ �� �� �𸮾��� �ʱ�ȭ ����� �����ڰ� ���Ƿ� ������� ���ϵ��� ���� �����̴�.
UMainMenu::UMainMenu(const FObjectInitializer & ObjectIniterlizer)
{
	ConstructorHelpers::FClassFinder<UUserWidget>ServerRow_BPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ensure(ServerRow_BPClass.Class != nullptr)) return;

	ServerRowClass = ServerRow_BPClass.Class;


}




bool UMainMenu::Initialize()
{
   bool Success = Super::Initialize();
   if (!Success) return false;

   if (!ensure(HostButton != nullptr)) return false;
   HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

   if (!ensure(JoinGameButton != nullptr)) return false;
   JoinGameButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

   if (!ensure(JoinButton != nullptr)) return false;
   JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

   if (!ensure(CancleJoinMenuButton != nullptr)) return false;
   CancleJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

   if (!ensure(ExitButton != nullptr)) return false;
   ExitButton->OnClicked.AddDynamic(this, &UMainMenu::ExitGame);


   UE_LOG(LogTemp, Warning, TEXT("MainMenu Initialize"));

   return true;
}



void UMainMenu::HostServer()
{
	if (!ensure(MenuInterface != nullptr)) return;
	MenuInterface->Host();

	UE_LOG(LogTemp, Warning, TEXT("Hosting Server"));
}

void UMainMenu::SetServerList(TArray<FString> ServerNames)
{
	UWorld* Temp_World = this->GetWorld();
	if (!ensure(Temp_World != nullptr)) return;

	ServerList->ClearChildren();

	for (const FString& ServerName : ServerNames)
	{
		UServerRow* Child_ServerRow = CreateWidget<UServerRow>(Temp_World, ServerRowClass);
		if (!ensure(Child_ServerRow != nullptr)) return;

		Child_ServerRow->ServerName->SetText(FText::FromString(ServerName));

		ServerList->AddChild(Child_ServerRow);

	}



}

void UMainMenu::JoinServer()
{
	if (!ensure(MenuInterface != nullptr)) return;
	{
		//if (!ensure(IPAdressField != nullptr)) return;
		//const FString& Adress = IPAdressField->GetText().ToString();
		MenuInterface->Join("");
	}
	
}



void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuInterface != nullptr)) return;
	//MenuInterface->Join(const FString & Address);

	if (!ensure(MainMenuWidgetSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;
	
	MainMenuWidgetSwitcher->SetActiveWidget(JoinMenu);


	UE_LOG(LogTemp, Warning, TEXT("Open Join Menu"));
}


void UMainMenu::OpenMainMenu()
{
	if (!ensure(MainMenuWidgetSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;
	MainMenuWidgetSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::ExitGame()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ConsoleCommand("quit");
}
