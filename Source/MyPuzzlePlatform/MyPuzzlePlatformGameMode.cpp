// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyPuzzlePlatformGameMode.h"
#include "MyPuzzlePlatformCharacter.h"
#include "UObject/ConstructorHelpers.h"



AMyPuzzlePlatformGameMode::AMyPuzzlePlatformGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}



}
