// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChooseFlower.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "MyGameInstance.h"


EBTNodeResult::Type UBTTask_ChooseRedFlower::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UWorld* World = OwnerComp.GetWorld();
	if (!World) return EBTNodeResult::Failed;

	UMyGameInstance* GI = World->GetGameInstance<UMyGameInstance>();
	if (!GI) return EBTNodeResult::Failed;

	AActor* Flower = GI->getRedFlower();
	if (!Flower) return EBTNodeResult::Failed;

	setBBKey(OwnerComp, Flower);
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UBTTask_ChooseYellowFlower::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UWorld* World = OwnerComp.GetWorld();
	if (!World) return EBTNodeResult::Failed;

	UMyGameInstance* GI = World->GetGameInstance<UMyGameInstance>();
	if (!GI) return EBTNodeResult::Failed;

	AActor* Flower = GI->getYellowFlower();
	if (!Flower) return EBTNodeResult::Failed;

	setBBKey(OwnerComp, Flower);
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UBTTask_ChooseBlueFlower::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UWorld* World = OwnerComp.GetWorld();
	if (!World) return EBTNodeResult::Failed;

	UMyGameInstance* GI = World->GetGameInstance<UMyGameInstance>();
	if (!GI) return EBTNodeResult::Failed;

	AActor* Flower = GI->getBlueFlower();
	if (!Flower) return EBTNodeResult::Failed;

	setBBKey(OwnerComp, Flower);
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UBTTask_ChooseBlackFlower::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UWorld* World = OwnerComp.GetWorld();
	if (!World) return EBTNodeResult::Failed;

	UMyGameInstance* GI = World->GetGameInstance<UMyGameInstance>();
	if (!GI) return EBTNodeResult::Failed;

	AActor* Flower = GI->getBlackFlower();
	if (!Flower) return EBTNodeResult::Failed;

	setBBKey(OwnerComp, Flower);
	return EBTNodeResult::Succeeded;
}

void UBTTask_ChooseFlower::setBBKey(UBehaviorTreeComponent& OwnerComp, AActor* flower)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BlackboardKey.SelectedKeyName, flower);
}
