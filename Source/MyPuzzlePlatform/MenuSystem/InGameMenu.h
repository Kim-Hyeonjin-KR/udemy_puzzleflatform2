// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"




#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class MYPUZZLEPLATFORM_API UInGameMenu : public UMenuWidget
{
	GENERATED_BODY()

public:

protected:
	virtual bool Initialize();


private:
	UPROPERTY(meta =(BindWidget))
	class UButton* InGameMenu_Cancle;

	UPROPERTY(meta = (BindWidget))
	class UButton* InGameMenu_Quit;

	UFUNCTION()
	void CloseInGameMenu();

	UFUNCTION()
	void QuitInGame();

	UFUNCTION(Blueprintcallable)
	void LoadInGameMenu();

};
