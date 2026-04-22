// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "BaseTask.h"

#include "BTTask_ChooseFlower.generated.h"

/**
 * 
 */
UCLASS()
class BTGENERATOR_API UBTTask_ChooseFlower : public UBTTask_BlackboardBase, public IBaseTask
{
	GENERATED_BODY()
public:
	inline void SetKeyName(FName name) override { BlackboardKey.SelectedKeyName = name; };
	inline void ResolveKey(UBlackboardData* BBAsset) override { BlackboardKey.ResolveSelectedKey(*BBAsset); };
protected:
	void setBBKey(UBehaviorTreeComponent& OwnerComp, AActor* flower);
};


UCLASS()
class BTGENERATOR_API UBTTask_ChooseRedFlower : public UBTTask_ChooseFlower 
{
	GENERATED_BODY()
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

UCLASS()
class BTGENERATOR_API UBTTask_ChooseYellowFlower : public UBTTask_ChooseFlower
{
	GENERATED_BODY()
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

UCLASS()
class BTGENERATOR_API UBTTask_ChooseBlueFlower : public UBTTask_ChooseFlower
{
	GENERATED_BODY()
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

UCLASS()
class BTGENERATOR_API UBTTask_ChooseBlackFlower : public UBTTask_ChooseFlower
{
	GENERATED_BODY()
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
