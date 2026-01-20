// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "HasLineOfSight_Decorator.generated.h"

/**
 * 
 */
UCLASS()
class BTGENERATOR_API UHasLineOfSight_Decorator : public UBTDecorator
{
	GENERATED_BODY()
public:
	UHasLineOfSight_Decorator();
	inline void SetKeyName(FName name) { _blackboardKey.SelectedKeyName = name; };
	inline void ResolveKey(UBlackboardData* BBAsset) { _blackboardKey.ResolveSelectedKey(*BBAsset); };
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector _blackboardKey;
};
