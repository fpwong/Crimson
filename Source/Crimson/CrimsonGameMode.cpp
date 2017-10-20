// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Crimson.h"
#include "CrimsonGameMode.h"
#include "CrimsonPawn.h"
#include "CrimsonController.h"

ACrimsonGameMode::ACrimsonGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = ACrimsonPawn::StaticClass();
	PlayerControllerClass = ACrimsonController::StaticClass();
}
