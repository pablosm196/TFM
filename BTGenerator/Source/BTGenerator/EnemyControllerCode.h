// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include <string>

#include "EnemyControllerCode.generated.h"
/**
 * 
 */
UCLASS()
class BTGENERATOR_API AEnemyControllerCode : public AAIController
{
	GENERATED_BODY()
public: 
	AEnemyControllerCode();

	void SetBT(std::string path);
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
private:
	UPROPERTY()
	UBlackboardComponent* _blackBoard = nullptr;

	UPROPERTY()
	UBehaviorTreeComponent* _behaviorTree = nullptr;

	UPROPERTY()
	UBehaviorTree* _root;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UAIPerceptionComponent* _perception;

	UPROPERTY()
	UAISenseConfig_Sight* _sightConfig;

	FTimerHandle _timerHandle;
};
