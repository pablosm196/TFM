// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindRandomPatrol.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomPatrol::UBTTask_FindRandomPatrol()
{
	NodeName = "Find Random Location";
}

EBTNodeResult::Type UBTTask_FindRandomPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return EBTNodeResult::Failed;

	APawn* Pawn = Controller->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;

	FVector position = Pawn->GetActorLocation();

	FNavLocation RandomPoint;
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(Pawn->GetWorld());

	if (NavSys && NavSys->GetRandomReachablePointInRadius(position, PatrolRadius, RandomPoint))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector("PatrolLocation", RandomPoint.Location);
		return EBTNodeResult::Succeeded;
	}
	else {
		OwnerComp.GetBlackboardComponent()->SetValueAsVector("PatrolLocation", position);
		return EBTNodeResult::Succeeded;
	}
}
