// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BaseDecorator.h"
#include "IsItNear_Decorator.generated.h"

/**
 * 
 */
UCLASS()
class BTGENERATOR_API UIsItNear_Decorator : public UBaseDecorator
{
	GENERATED_BODY()
public:
	UIsItNear_Decorator();
	inline void SetKeyName(FName name) { BlackboardKey.SelectedKeyName = name; };
	inline void ResolveKey(UBlackboardData* BBAsset) { BlackboardKey.ResolveSelectedKey(*BBAsset); };
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
