// Fill out your copyright notice in the Description page of Project Settings.


#include "BTGeneratorComponent.h"

#include "EnemyControllerCode.h"

#include "Kismet/GameplayStatics.h"

#include "MyBTTask_RotateToFaceBBEntry.h"
#include "BTTask_ChasePlayer.h"
#include "MyBTTask_MoveTo.h"
#include "BTTask_FindRandomPatrol.h"
#include "BehaviorTree/Tasks/BTTask_Wait.h"
#include "PickFlowerTask.h"
#include "BTTask_ChooseFlower.h"

#include "HasLineOfSight_Decorator.h"

#include "BTConstructor.h"
#include "BTFactory.h"

#include <string>
#include <cstdlib>

#include<fstream>

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

	if (BehaviorList::Init(TCHAR_TO_UTF8(*_validationPath), this))
		_list = BehaviorList::Instance();
}

void UBTGeneratorComponent::GenerateBT()
{
	BTFactory::Init();
	BTFactory* f = BTFactory::Instance();

	f->RegisterBlackboardTask<UMyBTTask_RotateToFaceBBEntry>("RotateToFaceBBEntry", "Rotate until facing the target");
	f->RegisterTask<UBTTask_ChasePlayer>("ChasePlayer", "Chases the player");
	f->RegisterBlackboardTask<UMyBTTask_MoveTo>("MoveTo", "Moves to the target position or object");
	f->RegisterTask<UBTTask_FindRandomPatrol>("FindRandomPatrol", "Choose a random point from the map and stores in the target");
	f->RegisterTask<UBTTask_Wait>("Wait", "Does nothing for a while");
	f->RegisterTask<UPickRedFlowerTask>("PickRedFlower", "Pick a red flower");
	f->RegisterTask<UPickYellowFlowerTask>("PickYellowFlower", "Pick a yellow flower");
	f->RegisterTask<UPickBlueFlowerTask>("PickBlueFlower", "Pick a blue flower");
	f->RegisterTask<UPickBlackFlowerTask>("PickBlackFlower", "Pick a black flower");
	f->RegisterBlackboardTask<UBTTask_ChooseRedFlower>("ChooseRedFlower", "Choose one red flower as a target");
	f->RegisterBlackboardTask<UBTTask_ChooseYellowFlower>("ChooseYellowFlower", "Choose one yellow flower as a target");
	f->RegisterBlackboardTask<UBTTask_ChooseBlueFlower>("ChooseBlueFlower", "Choose one blue flower as a target");
	f->RegisterBlackboardTask<UBTTask_ChooseBlackFlower>("ChooseBlackFlower", "Choose one black flower as a target");

	f->RegisterDecorator<UHasLineOfSight_Decorator>("HasLineOfSight?", "True if the player is in the line of sight, false if not");

	BTConstructor::Init();

	//CallGenerator(_prompt);


#if WITH_EDITOR
	if (GEditor)
		GEditor->RequestPlaySession(FRequestPlaySessionParams());
#endif
}


void UBTGeneratorComponent::CallGenerator(FString prompt)
{
	BTFactory* f = BTFactory::Instance();

	FString t = f->getAllTasks().c_str();
	FString bbt = f->getAllBlackboardTasks().c_str();
	FString d = f->getAllTasks().c_str();
	FString p = _BTName;
	FString entries = "[{\"\"Flower\"\" : \"\"Object\"\"}]";

	FString UVExe = TEXT("python");
	//FString UVExe = TEXT("uv");

	FString ScriptPath = TEXT("C:/Users/pablo/Desktop/Uni/Master/TFM/llm-bt-generator/main.py");
	FString Args = FString::Printf(
		TEXT("%s \"%s\" \"%s\" \"%s\" \"%s\" %s \"%s\""),
		*ScriptPath,
		*t,
		*bbt,
		*d,
		*prompt,
		*p,
		*entries
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


}

// Called every frame
void UBTGeneratorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBTGeneratorComponent::OnValidationFails(FString message)
{
	UE_LOG(LogTemp, Warning, TEXT("VALIDACIÓN FALLIDA"));
	std::string route = std::string(TCHAR_TO_UTF8(*FPaths::ProjectContentDir())).append("/TFM/JSONs/").append(TCHAR_TO_UTF8(*_BTName)).append(".json");
	std::ifstream f(route);
	json data = json::parse(f);

	if (data["Tries"] > 3) {
		data["Failed"] = true;
		std::ofstream out(route);
		out << data.dump(4);
		out.close();

#if WITH_EDITOR
		if (GEditor)
		{
			GEditor->RequestEndPlayMap();
		}
#endif

		return; // FALLO DEL CONSTRUCTOR DE BTs
	}

	std::string pseudocode = data["code"];

	FString prompt = "With the following pseudocode:\n" +
		FString(pseudocode.c_str()) +
		"\nIt gave me the following error: " +
		message +
		"\nFix the pseudocode for the following action:\n" +
		_prompt;

	CallGenerator(prompt);

	FString LevelName = UGameplayStatics::GetCurrentLevelName(this, true);

	UGameplayStatics::OpenLevel(this, FName(*LevelName));
}

void UBTGeneratorComponent::OnValidationEnds()
{
	std::string route = std::string(TCHAR_TO_UTF8(*FPaths::ProjectContentDir())).append("/TFM/JSONs/").append(TCHAR_TO_UTF8(*_BTName)).append(".json");
	std::ifstream f(route);
	json data = json::parse(f);

	data["Failed"] = false;
	std::ofstream out(route);
	out << data.dump(4);
	out.close();

#if WITH_EDITOR
	if (GEditor)
	{
		GEditor->RequestEndPlayMap();
	}
#endif
}

