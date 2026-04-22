// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChooseFlower.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "MyGameInstance.h"


EBTNodeResult::Type UBTTask_ChooseRedFlower::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActor* flower = Cast<UMyGameInstance>(GetWorld()->GetGameInstance())->getRedFlower();
	setBBKey(OwnerComp, flower);
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UBTTask_ChooseYellowFlower::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActor* flower = Cast<UMyGameInstance>(GetWorld()->GetGameInstance())->getYellowFlower();
	setBBKey(OwnerComp, flower);
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UBTTask_ChooseBlueFlower::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActor* flower = Cast<UMyGameInstance>(GetWorld()->GetGameInstance())->getBlueFlower();
	setBBKey(OwnerComp, flower);
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UBTTask_ChooseBlackFlower::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActor* flower = Cast<UMyGameInstance>(GetWorld()->GetGameInstance())->getBlackFlower();
	setBBKey(OwnerComp, flower);
	return EBTNodeResult::Succeeded;
}

void UBTTask_ChooseFlower::setBBKey(UBehaviorTreeComponent& OwnerComp, AActor* flower)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BlackboardKey.SelectedKeyName, flower);
}
