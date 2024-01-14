// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

//플랫폼 인클루드들
#include "UObject/ConstructorHelpers.h"


//엔진 인클루드들
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

//개발자 생성 인클루드들
#include "ServerRow.h"



// 매개변수로 들어간 내용은 언리얼에서 제공하는 함수를 초기화 할 때 언리얼의 초기화 방식을 개발자가 임의로 덮어씌우지 못하도록 막기 위함이다.
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

	uint32 i = 0;
	for (const FString& ServerName : ServerNames)
	{
		UServerRow* Child_ServerRow = CreateWidget<UServerRow>(Temp_World, ServerRowClass);
		if (!ensure(Child_ServerRow != nullptr)) return;

		Child_ServerRow->ServerName->SetText(FText::FromString(ServerName));
		Child_ServerRow->Setup(this, i);
		++i;

		ServerList->AddChild(Child_ServerRow);
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;

}

void UMainMenu::JoinServer()
{
	if (SelectedIndex.IsSet() && (MenuInterface != nullptr))
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Index is %d"), SelectedIndex.GetValue());
		MenuInterface->Join(SelectedIndex.GetValue());

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Index is not set"));
	}
}



void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MainMenuWidgetSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;
	
	MainMenuWidgetSwitcher->SetActiveWidget(JoinMenu);
	UE_LOG(LogTemp, Warning, TEXT("Open Join Menu"));

	if (!ensure(MenuInterface != nullptr)) return;
	MenuInterface->RefreshServerList();
	UE_LOG(LogTemp, Warning, TEXT("Call Refresh Server List Function On MainMenu.Cpp"));
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
