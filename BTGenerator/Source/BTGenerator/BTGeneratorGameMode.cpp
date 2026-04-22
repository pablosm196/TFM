// Copyright Epic Games, Inc. All Rights Reserved.

#include "BTGeneratorGameMode.h"

#include "MyGameInstance.h"

ABTGeneratorGameMode::ABTGeneratorGameMode()
{
	// stub
}

void ABTGeneratorGameMode::BeginPlay()
{
	if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance())) {
		GI->resetLists();
	}
}
