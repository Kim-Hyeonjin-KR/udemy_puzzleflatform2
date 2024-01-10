// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"

#include "MainMenu.generated.h"


/**
 * 
 */
UCLASS()
class MYPUZZLEPLATFORM_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer & ObjectInitializer);

	void SetServerList(TArray<FString> ServerNames);

protected:
	virtual bool Initialize();

private:
	
	TSubclassOf<class UUserWidget> ServerRowClass;

	//Button
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancleJoinMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinGameButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;


	//WidgetSwitcher
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MainMenuWidgetSwitcher;

	//Widget
	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	//EditableTextBox
	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ServerList;

	//UPROPERTY(meta = (BindWidget))
	//class UTextBox* SessionText;





	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void OpenJoinMenu();
	
	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void ExitGame();

};
