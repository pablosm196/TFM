// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "BaseTask.h"

#include "SelectWaypoint_Task.generated.h"

/**
 * 
 */
UCLASS()
class BTGENERATOR_API UBTTask_SelectWaypoint : public UBTTask_BlackboardBase, public IBaseTask
{
	GENERATED_BODY()
public:
	inline void SetKeyName(FName name) override { BlackboardKey.SelectedKeyName = name; };
	inline void ResolveKey(UBlackboardData* BBAsset) override { BlackboardKey.ResolveSelectedKey(*BBAsset); };
protected:
	void setBBKey(UBehaviorTreeComponent& OwnerComp, AActor* waypoint);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
