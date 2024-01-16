// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "ServerRow.generated.h"

/**
 * 
 */
UCLASS()
class MYPUZZLEPLATFORM_API UServerRow : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HostUser;

	
	//UPROPERTY(meta = (BindWidget))
	//class UTextBlock* ConnectionFraction;	//Fraction 당원, 조직의 일원, 분수
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentUser;


	UPROPERTY(BlueprintReadOnly)
	bool Selected = false;

	void Setup(class UMainMenu* _Parent, uint32 _Index);

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* ServerRowButton;

	UPROPERTY()
	class UMainMenu* Parent;

	uint32 Index;


	UFUNCTION()
	void OnClicked();

};
