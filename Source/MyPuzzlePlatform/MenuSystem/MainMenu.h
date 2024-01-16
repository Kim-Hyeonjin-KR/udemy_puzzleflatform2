// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"

#include "MainMenu.generated.h"


/**
 * 
 */


USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUserName;

};



UCLASS()
class MYPUZZLEPLATFORM_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer & ObjectInitializer);

	void SetServerList(TArray<FServerData> ServerNames);

	void SelectIndex(uint32 Index);

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


	//TOptional�� ����ü�̴�
	//IsSet, Emplace, GetValue ���� �Լ��� Ȱ���ϸ� ���ڰ��� �ʱ�ȭ ���γ� �߰��� ���� ��Ȯ�ϰ� �ڵ������ ó���� �� �ִٴ� ������ �ִ� 
	TOptional<uint32> SelectedIndex;

	void UpdateChildren();
};
