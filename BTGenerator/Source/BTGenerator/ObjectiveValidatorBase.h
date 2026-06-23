// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ObjectiveValidatorBase.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UObjectiveValidatorBase : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BTGENERATOR_API IObjectiveValidatorBase
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool CheckObjectiveCompleted() = 0;
	inline FString getMessage() { return _failedMessage; }
protected:
	FString _failedMessage;
};
