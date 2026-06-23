// Fill out your copyright notice in the Description page of Project Settings.


#include "PickFlowerTask.h"
#include "BehaviorList.h"

#include "MyGameInstance.h"

UPickFlowerTask::UPickFlowerTask()
{
	_color = "";
}

EBTNodeResult::Type UPickFlowerTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	std::string color = _color;
	color[0] = toupper(color[0]);
	FString behavior = ("Pick" + color + "Flower").c_str();

	OwnerComp.GetWorld()->GetGameInstance<UMyGameInstance>()->flowerPicked(_color.c_str());

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
