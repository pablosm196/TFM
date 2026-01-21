// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BaseTask.generated.h"

/**
 * 
 */
UINTERFACE()
class UBaseTask : public UInterface
{
	GENERATED_BODY()
};

class IBaseTask
{
	GENERATED_BODY()

public:
	virtual void SetKeyName(FName Name) = 0;
	virtual void ResolveKey(UBlackboardData* BBAsset) = 0;
};
