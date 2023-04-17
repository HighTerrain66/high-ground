// Copyright Epic Games, Inc. All Rights Reserved.

#include "ReboundGameMode.h"
#include "UObject/ConstructorHelpers.h"

AReboundGameMode::AReboundGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/BP/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
