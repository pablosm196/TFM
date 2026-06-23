// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectiveValidatorBase.h"

#include "ObjectiveFlowers1.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BTGENERATOR_API UObjectiveFlowers1 : public UActorComponent, public IObjectiveValidatorBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UObjectiveFlowers1();
	bool CheckObjectiveCompleted() override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
