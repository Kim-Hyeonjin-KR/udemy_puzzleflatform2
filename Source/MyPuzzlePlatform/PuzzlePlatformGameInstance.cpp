// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "Online/OnlineSessionNames.h"


#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"

const static FName SESSION_NAME = TEXT("My Session Game");



UPuzzlePlatformGameInstance::UPuzzlePlatformGameInstance(const FObjectInitializer& ObjectInitializer)
{
	//Ŭ���� ���δ�
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Construct"));

	//������ WBP_MainMenuClass
	//ConstructorHelpers::FClassFinder<UUserWidget>���� ��ο��� �ν��Ͻ�orŬ������ ã�Ƽ� ������ �־��ش�.
	
	static ConstructorHelpers::FClassFinder<UUserWidget> WBP_MainMenuClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(WBP_MainMenuClass.Class != nullptr)) return;

	MainMenu_Class = WBP_MainMenuClass.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> WBP_InGameMenuClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!ensure(WBP_InGameMenuClass.Class != nullptr)) return;

	InGameMenu_Class = WBP_InGameMenuClass.Class;
}

void UPuzzlePlatformGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Init"));

	//�¶��� ����ý����� ��⿡�� ������
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Online Subsystem Name : %s"), *Subsystem->GetSubsystemName().ToString());

		//���������͸� �����ϰ� ����ý��ۿ��� �����������̽��� �����ͼ� ����.
		SessionInterface = Subsystem->GetSessionInterface();

		if (SessionInterface.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Found Session Interface"));

			//UObject�� ���ε� �ϴ� ������ ������ �÷��ǿ��� �ڵ����� �������ִ� ���� �븮�� �����̴�.
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnJoinSessionsComplete);

			//RefreshServerList();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Found Online Subsystem"));
	}

	//UE_LOG(LogTemp, Warning, TEXT("Found Class Name : %s"), *MainMenu_Class->GetName());

	//TEST CODE
	LoadMenuWidget();
}

void UPuzzlePlatformGameInstance::LoadMenuWidget()
{

	UE_LOG(LogTemp, Warning, TEXT("Starting Load Menu Widget Function"));

	if (!ensure(MainMenu_Class != nullptr))
	{
		UE_LOG(LogTemp, Warning, TEXT("Nullptr MainMenu_Class Load Menu Widget Function"));
		return;
	}
	Menu = CreateWidget<UMainMenu>(this, MainMenu_Class);
	if (!ensure(Menu != nullptr)) return;
	
	Menu->Setup();
	Menu->SetMenuInterface(this);
}

void UPuzzlePlatformGameInstance::LoadInGameMenu()
{
	if (!ensure(InGameMenu_Class != nullptr)) return;
	
	UMenuWidget* InGameMenu = CreateWidget<UMenuWidget>(this, InGameMenu_Class);
	if (!ensure(InGameMenu != nullptr)) return;
	
	InGameMenu->Setup();
	InGameMenu->SetMenuInterface(this);
}

void UPuzzlePlatformGameInstance::Host()
{
	if (SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Destroy Session Starting"));
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Create Session Starting"));
			CreateSession();
		}
	}
}

void UPuzzlePlatformGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			SessionSettings.bIsLANMatch = true;
		}
		else
		{
			SessionSettings.bIsLANMatch = false;
		}
		//�� ������ ��Ī ����
		SessionSettings.NumPublicConnections = 2;
		//�¶��� ���ǿ��� �˻��� ��� ������ �� ����
		SessionSettings.bShouldAdvertise = true;
		//���ͳ� ���� (LobbySession, InternetSession)�� �����ϱ� ���� �ʿ��� ����. ���� SessionSettings.bIsLANMatch�� false�� ��쿡 �۵��ϵ��� ����Ǿ� �����Ƿ� �ݵ�� ������.
		SessionSettings.bUsesPresence = true;

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UPuzzlePlatformGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not creat session"));
		return;
	}

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

//DestroySession�� �Ϸ�Ǹ� �ڵ����� �ݹ��� �ް� �Լ��� �����.
void UPuzzlePlatformGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		CreateSession();
	}
}

void UPuzzlePlatformGameInstance::RefreshServerList()
{
	UE_LOG(LogTemp, Warning, TEXT("Refresh Server List Starting"));

	//������ ���� �����Ϳ� �ٷ� ���� �� ���� ��������� MakeShareable �Լ��� ������� �Ѵ�.
	//������Ʈ�� �����ڰ� private�̴��� ������ �����ϰ� �Ҹ�� Ŀ���� �����̺� �����ȴ�.
	//MakeShareable�� ����Ʈ �����Ϳ� ���� ���� �Լ��̴�.
	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	if (SessionSearch.IsValid())
	{
		//SessionSearch->bIsLanQuery = true;	// LAN��� ����
		SessionSearch->MaxSearchResults = 1000;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

		UE_LOG(LogTemp, Warning, TEXT("Starting Find Session"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UPuzzlePlatformGameInstance::OnFindSessionsComplete(bool Success)
{
	UE_LOG(LogTemp, Warning, TEXT("OnFindSessionsComplete Funtion Start"));


	if (Success && SessionSearch.IsValid() && (Menu != nullptr))
	{
		UE_LOG(LogTemp, Warning, TEXT("Finished Find Session"));

		TArray<FServerData> ServerNames;

		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Find Session is %s"), *SearchResult.GetSessionIdStr());

			FServerData Data;
			Data.Name = SearchResult.GetSessionIdStr();
			Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;	// SearchResult.Session.NumOpenPublicConnections == ���� ������ ����ִ� ������ ��

			Data.HostUserName = SearchResult.Session.OwningUserName;

			ServerNames.Add(Data);
		}

		Menu->SetServerList(ServerNames);
	}
}

void UPuzzlePlatformGameInstance::Join(uint32 _index)
{

	if (!SessionInterface.IsValid()) return;
	if (!SessionSearch.IsValid()) return;

	if (Menu != nullptr)
	{

		Menu->Teardown();
	}

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[_index]);
}

void UPuzzlePlatformGameInstance::OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) return;

	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed Get Resolved Connect String"));
		return;
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 6, FColor::Green, FString::Printf(TEXT("Join %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}
