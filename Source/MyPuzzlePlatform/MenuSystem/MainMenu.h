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


protected:
	virtual bool Initialize();

private:

	//Button
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancleJoinMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinGame;


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
	class UEditableTextBox* IPAdressField;



	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void OpenJoinMenu();
	
	UFUNCTION()
	void OpenMainMenu();



};
