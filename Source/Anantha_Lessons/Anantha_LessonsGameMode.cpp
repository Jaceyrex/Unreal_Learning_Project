// Copyright Epic Games, Inc. All Rights Reserved.

#include "Anantha_LessonsGameMode.h"
#include "Anantha_LessonsCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAnantha_LessonsGameMode::AAnantha_LessonsGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
