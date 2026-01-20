// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindRandomPatrol.generated.h"

/**
 * 
 */
UCLASS()
class BTGENERATOR_API UBTTask_FindRandomPatrol : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_FindRandomPatrol();
	inline void SetPatrolRadius(float newRadius) { PatrolRadius = newRadius; };
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Search")
	float PatrolRadius = 1000.f;
};
