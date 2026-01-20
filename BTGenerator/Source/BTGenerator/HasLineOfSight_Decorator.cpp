// Fill out your copyright notice in the Description page of Project Settings.


#include "HasLineOfSight_Decorator.h"
#include "BehaviorTree/BlackboardComponent.h"

UHasLineOfSight_Decorator::UHasLineOfSight_Decorator()
{
	NodeName = "Has Line Of Sight?";

	_blackboardKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UHasLineOfSight_Decorator, _blackboardKey));
	FlowAbortMode = EBTFlowAbortMode::Both;
}

bool UHasLineOfSight_Decorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool tmp = OwnerComp.GetBlackboardComponent()->GetValueAsBool(_blackboardKey.SelectedKeyName);
	return tmp;
}
