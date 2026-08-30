// Fill out your copyright notice in the Description page of Project Settings.


#include "HasLineOfSight_Decorator.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorList.h"

UDecorator_HasLineOfSight::UDecorator_HasLineOfSight()
{
	NodeName = "Has Line Of Sight?";

	BlackboardKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UDecorator_HasLineOfSight, BlackboardKey));
	FlowAbortMode = EBTFlowAbortMode::Both;
}

bool UDecorator_HasLineOfSight::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	FString behavior = "HasLineOfSight?";
	BehaviorList::Instance()->addBehavior(behavior);

	bool tmp = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BlackboardKey.SelectedKeyName);
	return tmp;
}
