// Fill out your copyright notice in the Description page of Project Settings.


#include "PickFlowerTask.h"
#include "BehaviorList.h"

UPickFlowerTask::UPickFlowerTask()
{
	_color = "";
}

EBTNodeResult::Type UPickFlowerTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	std::string behavior = "Pick flower " + _color;

	BehaviorList::Instance()->addBehavior(behavior);

	return EBTNodeResult::Succeeded;
}

UPickRedFlowerTask::UPickRedFlowerTask()
{
	_color = "red";
}

UPickYellowFlowerTask::UPickYellowFlowerTask()
{
	_color = "yellow";
}

UPickBlueFlowerTask::UPickBlueFlowerTask()
{
	_color = "blue";
}

UPickBlackFlowerTask::UPickBlackFlowerTask()
{
	_color = "black";
}
