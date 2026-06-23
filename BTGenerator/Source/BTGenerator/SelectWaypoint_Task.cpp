// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectWaypoint_Task.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "MyGameInstance.h"
#include "BehaviorList.h"

EBTNodeResult::Type USelectWaypoint_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UWorld* World = OwnerComp.GetWorld();
	if (!World) return EBTNodeResult::Failed;

	UMyGameInstance* GI = World->GetGameInstance<UMyGameInstance>();
	if (!GI) return EBTNodeResult::Failed;

	AActor* Waypoint = GI->getRandomWaypoint();
	if (!Waypoint) return EBTNodeResult::Failed;

	setBBKey(OwnerComp, Waypoint);

	FString behavior = "SelectWaypoint";
	BehaviorList::Instance()->addBehavior(behavior);

	return EBTNodeResult::Succeeded;
}

void USelectWaypoint_Task::setBBKey(UBehaviorTreeComponent& OwnerComp, AActor* waypoint)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BlackboardKey.SelectedKeyName, waypoint);
}