// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectiveFlowers1.h"
#include "MyGameInstance.h"

// Sets default values for this component's properties
UObjectiveFlowers1::UObjectiveFlowers1()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UObjectiveFlowers1::CheckObjectiveCompleted()
{
	UMyGameInstance* GI = GetWorld()->GetGameInstance<UMyGameInstance>();

	return GI->getNumRed() == 1 && GI->getNumBlue() == 1 && GI->getNumYellow() == 1;
}


// Called when the game starts
void UObjectiveFlowers1::BeginPlay()
{
	Super::BeginPlay();

	_failedMessage = "It hasn't collected the required flowers (red, blue and yellow)";
	
}


// Called every frame
void UObjectiveFlowers1::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

