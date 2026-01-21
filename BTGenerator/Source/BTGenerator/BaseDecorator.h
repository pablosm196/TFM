// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BaseDecorator.generated.h"

/**
 * 
 */
UCLASS()
class BTGENERATOR_API UBaseDecorator : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
public:
	inline void SetKeyName(FName name) { BlackboardKey.SelectedKeyName = name; };
	inline void ResolveKey(UBlackboardData* BBAsset) { BlackboardKey.ResolveSelectedKey(*BBAsset); };
};
