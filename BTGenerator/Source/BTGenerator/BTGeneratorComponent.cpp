// Fill out your copyright notice in the Description page of Project Settings.


#include "BTGeneratorComponent.h"

#include "EnemyControllerCode.h"

#include "MyBTTask_RotateToFaceBBEntry.h"
#include "BTTask_ChasePlayer.h"
#include "MyBTTask_MoveTo.h"
#include "BTTask_FindRandomPatrol.h"
#include "BehaviorTree/Tasks/BTTask_Wait.h"

#include "HasLineOfSight_Decorator.h"
#include "PickFlowerTask.h"
#include "BTTask_ChooseFlower.h"

#include "BTConstructor.h"
#include "BTFactory.h"

#include <string>
#include <cstdlib>

// Sets default values for this component's properties
UBTGeneratorComponent::UBTGeneratorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBTGeneratorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	//BTFactory::Init();
	//BTFactory* f = BTFactory::Instance();

	//f->RegisterTask<UMyBTTask_RotateToFaceBBEntry>("RotateToFaceBBEntry");
	//f->RegisterTask<UBTTask_ChasePlayer>("ChasePlayer");
	//f->RegisterTask<UMyBTTask_MoveTo>("MoveTo");
	//f->RegisterTask<UBTTask_FindRandomPatrol>("FindRandomPatrol");
	//f->RegisterTask<UBTTask_Wait>("Wait");

	//f->RegisterDecorator<UHasLineOfSight_Decorator>("HasLineOfSight?");

	//BTConstructor::Init();
}

void UBTGeneratorComponent::GenerateBT()
{
	BTFactory::Init();
	BTFactory* f = BTFactory::Instance();

	f->RegisterBlackboardTask<UMyBTTask_RotateToFaceBBEntry>("RotateToFaceBBEntry");
	f->RegisterTask<UBTTask_ChasePlayer>("ChasePlayer");
	f->RegisterBlackboardTask<UMyBTTask_MoveTo>("MoveTo");
	f->RegisterTask<UBTTask_FindRandomPatrol>("FindRandomPatrol");
	f->RegisterTask<UBTTask_Wait>("Wait");
	f->RegisterTask<UPickRedFlowerTask>("PickRedFlower");
	f->RegisterTask<UPickYellowFlowerTask>("PickYellowFlower");
	f->RegisterTask<UPickBlueFlowerTask>("PickBlueFlower");
	f->RegisterTask<UPickBlackFlowerTask>("PickBlackFlower");
	f->RegisterBlackboardTask<UBTTask_ChooseRedFlower>("ChooseRedFlower");
	f->RegisterBlackboardTask<UBTTask_ChooseYellowFlower>("ChooseYellowFlower");
	f->RegisterBlackboardTask<UBTTask_ChooseBlueFlower>("ChooseBlueFlower");
	f->RegisterBlackboardTask<UBTTask_ChooseBlackFlower>("ChooseBlackFlower");

	f->RegisterDecorator<UHasLineOfSight_Decorator>("HasLineOfSight?");

	BTConstructor::Init();

	FString t = f->getAllTasks().c_str();
	FString bbt = f->getAllBlackboardTasks().c_str();
	FString d = f->getAllTasks().c_str();
	FString p = _BTName;

	FString UVExe = TEXT("python");
	//FString UVExe = TEXT("uv");

	FString ScriptPath = TEXT("C:/Users/pablo/Desktop/Uni/Master/TFM/llm-bt-generator/main.py");
	FString Args = FString::Printf(
		TEXT("%s %s %s %s \"%s\" %s"),
		*ScriptPath,
		*t,
		*bbt,
		*d,
		*_prompt,
		*p
	);

	int32 ReturnCode = 0;
	FString StdOut;
	FString StdErr;

	FPlatformProcess::ExecProcess(
		*UVExe,
		*Args,
		&ReturnCode,
		&StdOut,
		&StdErr
	);

	UE_LOG(LogTemp, Warning, TEXT("Salida: %s"), *StdOut);
	UE_LOG(LogTemp, Error, TEXT("Errores: %s"), *StdErr);
	UE_LOG(LogTemp, Warning, TEXT("Código retorno: %d"), ReturnCode);

#if WITH_EDITOR
	if (GEditor)
		GEditor->RequestPlaySession(FRequestPlaySessionParams());
#endif

	//AEnemyControllerCode* controller = Cast<AEnemyControllerCode>(_pawn->GetController());
}


// Called every frame
void UBTGeneratorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

