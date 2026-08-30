// Fill out your copyright notice in the Description page of Project Settings.


#include "IsItNear_Decorator.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BehaviorList.h"

UDecorator_IsItNear::UDecorator_IsItNear()
{
	FlowAbortMode = EBTFlowAbortMode::Both;
}

bool UDecorator_IsItNear::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	FString behavior = "IsItNear?";
	BehaviorList::Instance()->addBehavior(behavior);

	double distance = FVector::Distance(OwnerComp.GetOwner()->GetActorLocation(), Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BlackboardKey.SelectedKeyName))->GetActorLocation());

	return distance < 5;
}
