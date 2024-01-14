// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "MenuSystem/MenuInterface.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "PuzzlePlatformGameInstance.generated.h"


/**
 * 
 */
UCLASS(Blueprintable)
class MYPUZZLEPLATFORM_API UPuzzlePlatformGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
	
public:
	UPuzzlePlatformGameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init();
	
	UFUNCTION(BlueprintCallable)
	void LoadMenuWidget();

	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();

	UFUNCTION(Exec)
	void Host() override;

	UFUNCTION(Exec)
	void Join(uint32 _index) override;

	virtual void LoadMainMenu() override;
	void RefreshServerList() override;

private:

	TSubclassOf<class UUserWidget> MainMenu_Class;
	TSubclassOf<class UUserWidget> InGameMenu_Class;

	IOnlineSessionPtr SessionInterface;

	//FOnlineSessionSearch 클래스를 전방선언
	TSharedPtr< class FOnlineSessionSearch > SessionSearch;

	class UMainMenu* Menu;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void CreateSession();
};
