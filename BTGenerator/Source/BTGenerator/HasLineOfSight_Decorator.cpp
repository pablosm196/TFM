// Fill out your copyright notice in the Description page of Project Settings.


#include "HasLineOfSight_Decorator.h"
#include "BehaviorTree/BlackboardComponent.h"

UHasLineOfSight_Decorator::UHasLineOfSight_Decorator()
{
	NodeName = "Has Line Of Sight?";

	BlackboardKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UHasLineOfSight_Decorator, BlackboardKey));
	FlowAbortMode = EBTFlowAbortMode::Both;
}

bool UHasLineOfSight_Decorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool tmp = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BlackboardKey.SelectedKeyName);
	return tmp;
}
