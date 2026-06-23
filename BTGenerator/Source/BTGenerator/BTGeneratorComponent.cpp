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
#include "AttackNode.h"
#include "SelectWaypoint_Task.h"

#include "HasLineOfSight_Decorator.h"
#include "IsItNear_Decorator.h"

#include "BTConstructor.h"
#include "BTFactory.h"

#include "ObjectiveValidatorBase.h"

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

	IObjectiveValidatorBase* validator = nullptr;

	for (UActorComponent* C : GetOwner()->GetComponents())
	{
		if (C->GetClass()->ImplementsInterface(UObjectiveValidatorBase::StaticClass()))
		{
			validator = Cast<IObjectiveValidatorBase>(C);
			break;
		}
	}

	if (BehaviorList::Init(TCHAR_TO_UTF8(*_validationPath), this, validator))
		_list = BehaviorList::Instance();

	//_timerHandle = FTimerHandle();

	GetWorld()->GetTimerManager().SetTimer(
		_timerHandle,
		this,
		&UBTGeneratorComponent::CheckValidationByTimer,
		10.0f,
		false
	);
}

void UBTGeneratorComponent::GenerateBT()
{
	BTFactory::Init();
	BTFactory* f = BTFactory::Instance();

	f->RegisterBlackboardTask<UMyBTTask_RotateToFaceBBEntry>("RotateToFaceBBEntry", "Rotate until facing the target");
	f->RegisterTask<UBTTask_ChasePlayer>("ChasePlayer", "Chases the player");
	f->RegisterBlackboardTask<UMyBTTask_MoveTo>("MoveTo", "Moves to the target position or object. The target position MUST BE an Object type.");
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
	f->RegisterTask<UAttackNode>("Attack", "It launches an attack on whoever is nearby");
	f->RegisterBlackboardTask<USelectWaypoint_Task>("SelectWaypoint", "Choose a random waypoint as a target. It needs and Object type");

	f->RegisterDecorator<UHasLineOfSight_Decorator>("HasLineOfSight?", "True if the player is seeing the target, false if not. It NEEDS a to check only a bool type");
	f->RegisterDecorator<UHasLineOfSight_Decorator>("IsItNear?", "True if the distance between the executor and the objective is less or equal than a certain number, false if not. The objective NEEDS to be an Object type");

	BTConstructor::Init();

	CallGenerator(_prompt);


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
	FString d = f->getAllDecorators().c_str();
	FString p = _BTName;
	FString entries = "[";

	for (auto i = _blackboardEntries.begin(); i != _blackboardEntries.end(); ++i) {
		entries += "{\"\"" + i->Key + "\"\" : \"\"" + i->Value + "\"\"},";
	}

	entries.RemoveAt(entries.Len() - 1);

	entries += "]";

	UE_LOG(LogTemp, Warning, TEXT("Salida: %s"), *entries);

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

void UBTGeneratorComponent::CheckValidationByTimer()
{
	IObjectiveValidatorBase* validator = nullptr;

	for (UActorComponent* C : GetOwner()->GetComponents())
	{
		if (C->GetClass()->ImplementsInterface(UObjectiveValidatorBase::StaticClass()))
		{
			validator = Cast<IObjectiveValidatorBase>(C);
			break;
		}
	}

	if (validator == nullptr) return;

	if (!validator->CheckObjectiveCompleted()) {
		FString message = validator->getMessage() +
			"The list of executed tasks is as follows: " +
			BehaviorList::Instance()->getBehaviors();

		OnValidationFails(message);
	}
	else {
		OnValidationEnds();
	}
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

