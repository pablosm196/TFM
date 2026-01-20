// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "MyBTTask_MoveTo.generated.h"

/**
 * 
 */
UCLASS()
class BTGENERATOR_API UMyBTTask_MoveTo : public UBTTask_MoveTo
{
	GENERATED_BODY()
public:
	inline void SetKeyName(FName name) { BlackboardKey.SelectedKeyName = name; };
	inline void ResolveKey(UBlackboardData* BBAsset) { BlackboardKey.ResolveSelectedKey(*BBAsset); };
};
