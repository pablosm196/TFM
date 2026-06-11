// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_RotateToFaceBBEntry.h"
#include "BehaviorList.h"

EBTNodeResult::Type UMyBTTask_RotateToFaceBBEntry::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

    FString behavior = "RotateToFaceBBEntry";

    BehaviorList::Instance()->addBehavior(behavior);

    return UBTTask_RotateToFaceBBEntry::ExecuteTask(OwnerComp, NodeMemory);
}
