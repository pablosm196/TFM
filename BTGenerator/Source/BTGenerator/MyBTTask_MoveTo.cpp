// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_MoveTo.h"
#include "BehaviorList.h"

EBTNodeResult::Type UMyBTTask_MoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    FString behavior = "MoveTo";
    BehaviorList::Instance()->addBehavior(behavior);
    return UBTTask_MoveTo::ExecuteTask(OwnerComp, NodeMemory);
}
