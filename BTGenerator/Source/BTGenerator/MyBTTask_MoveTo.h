// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BaseTask.h"
#include "MyBTTask_MoveTo.generated.h"
/**
 * 
 */
UCLASS()
class BTGENERATOR_API UMyBTTask_MoveTo : public UBTTask_MoveTo, public IBaseTask
{
	GENERATED_BODY()
public:
	inline void SetKeyName(FName name) override { BlackboardKey.SelectedKeyName = name; } ;
	inline void ResolveKey(UBlackboardData* BBAsset) override { BlackboardKey.ResolveSelectedKey(*BBAsset); } ;
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
