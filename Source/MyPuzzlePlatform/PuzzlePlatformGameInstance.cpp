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
	//클래스 파인더
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Construct"));

	//변수명 WBP_MainMenuClass
	//ConstructorHelpers::FClassFinder<UUserWidget>으로 경로에서 인스턴스or클래스를 찾아서 변수에 넣어준다.
	
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

	//온라인 서브시스템을 모듈에서 가져옴
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Online Subsystem Name : %s"), *Subsystem->GetSubsystemName().ToString());

		//세션포인터를 선언하고 서브시스템에서 세션인터페이스를 가져와서 넣음.
		SessionInterface = Subsystem->GetSessionInterface();

		if (SessionInterface.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Found Session Interface"));

			//UObject에 바인딩 하는 이유는 가비지 컬랙션에서 자동으로 관리해주는 것을 노리기 때문이다.
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
		//랜 서버로 매칭 여부
		SessionSettings.NumPublicConnections = 2;
		//온라인 세션에서 검색할 경우 공개될 지 여부
		SessionSettings.bShouldAdvertise = true;
		//인터넷 세션 (LobbySession, InternetSession)을 구성하기 위해 필요한 설정. 위의 SessionSettings.bIsLANMatch이 false일 경우에 작동하도록 설계되어 있으므로 반드시 꺼주자.
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

//DestroySession이 완료되면 자동으로 콜백을 받고 함수가 실행됨.
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

	//힙에서 공유 포인터에 바로 넣을 수 없게 만들어져서 MakeShareable 함수로 감싸줘야 한다.
	//오브젝트의 생성자가 private이더라도 접근이 가능하고 소멸시 커스텀 비헤이비어가 지원된다.
	//MakeShareable은 스마트 포인터에 대한 헬퍼 함수이다.
	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	if (SessionSearch.IsValid())
	{
		//SessionSearch->bIsLanQuery = true;	// LAN사용 여부
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
			Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;	// SearchResult.Session.NumOpenPublicConnections == 접속 가능한 비어있는 세션의 수

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
