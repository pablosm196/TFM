// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackNode.h"

#include "MyGameInstance.h"
#include "BehaviorList.h"

UAttackNode::UAttackNode()
{
}

EBTNodeResult::Type UAttackNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FString behavior = "Attack";
	BehaviorList::Instance()->addBehavior(behavior);

	OwnerComp.GetWorld()->GetGameInstance<UMyGameInstance>()->setPlayerDead();

	return EBTNodeResult::Succeeded;
}
