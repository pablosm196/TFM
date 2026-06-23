// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectiveAttack.h"

#include "MyGameInstance.h"

// Sets default values for this component's properties
UObjectiveAttack::UObjectiveAttack()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UObjectiveAttack::CheckObjectiveCompleted()
{
	UMyGameInstance* GI = GetWorld()->GetGameInstance<UMyGameInstance>();

	return GI->isPlayerDead();
}


// Called when the game starts
void UObjectiveAttack::BeginPlay()
{
	Super::BeginPlay();

	// ...
	_failedMessage = "It hasn't killed the objective";
}


// Called every frame
void UObjectiveAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

