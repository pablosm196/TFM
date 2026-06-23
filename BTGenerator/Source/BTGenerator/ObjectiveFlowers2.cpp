// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectiveFlowers2.h"

#include "MyGameInstance.h"

// Sets default values for this component's properties
UObjectiveFlowers2::UObjectiveFlowers2()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UObjectiveFlowers2::CheckObjectiveCompleted()
{
	UMyGameInstance* GI = GetWorld()->GetGameInstance<UMyGameInstance>();

	return GI->getNumRed() == 1 && GI->getNumBlue() == 3;
}


// Called when the game starts
void UObjectiveFlowers2::BeginPlay()
{
	Super::BeginPlay();

	// ...
	_failedMessage = "It hasn't collected the required flowers (three blue and one red)";

	
}


// Called every frame
void UObjectiveFlowers2::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

