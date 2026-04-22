// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

#include <string>

#include "PickFlowerTask.generated.h"

/**
 * 
 */
UCLASS()
class BTGENERATOR_API UPickFlowerTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UPickFlowerTask();
protected:
	std::string _color;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

UCLASS()
class BTGENERATOR_API UPickRedFlowerTask : public UPickFlowerTask
{
	GENERATED_BODY()
public:
	UPickRedFlowerTask();
};

UCLASS()
class BTGENERATOR_API UPickYellowFlowerTask : public UPickFlowerTask
{
	GENERATED_BODY()
public:
	UPickYellowFlowerTask();
};

UCLASS()
class BTGENERATOR_API UPickBlueFlowerTask : public UPickFlowerTask
{
	GENERATED_BODY()
public:
	UPickBlueFlowerTask();
};

UCLASS()
class BTGENERATOR_API UPickBlackFlowerTask : public UPickFlowerTask
{
	GENERATED_BODY()
public:
	UPickBlackFlowerTask();
};
