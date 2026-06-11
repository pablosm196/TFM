// Fill out your copyright notice in the Description page of Project Settings.


#include "FinalTask.h"
#include "BehaviorList.h"

EBTNodeResult::Type UFinalTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FString behavior = "Final";
	BehaviorList::Instance()->addBehavior(behavior);

	return EBTNodeResult::InProgress;
}
