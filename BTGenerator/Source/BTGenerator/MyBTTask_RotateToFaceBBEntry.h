// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_RotateToFaceBBEntry.h"
#include "BaseTask.h"
#include "MyBTTask_RotateToFaceBBEntry.generated.h"

/**
 * 
 */
UCLASS()
class BTGENERATOR_API UMyBTTask_RotateToFaceBBEntry : public UBTTask_RotateToFaceBBEntry, public IBaseTask
{
	GENERATED_BODY()
public:
	inline void SetPrecision(float precision) { Precision = precision; };
	inline void SetKeyName(FName name) override { BlackboardKey.SelectedKeyName = name; };
	inline void ResolveKey(UBlackboardData* BBAsset) override { BlackboardKey.ResolveSelectedKey(*BBAsset); };
};
