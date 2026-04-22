// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BTGeneratorGameMode.generated.h"

/**
 *  Simple GameMode for a third person game
 */
UCLASS()
class ABTGeneratorGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	/** Constructor */
	ABTGeneratorGameMode();

protected:
	virtual void BeginPlay() override;
};



