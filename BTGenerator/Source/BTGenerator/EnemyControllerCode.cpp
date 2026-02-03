// Fill out your copyright notice in the Description page of Project Settings.
#include "EnemyControllerCode.h"

//Clases base
#include "BehaviorTree/BehaviorTree.h"

//Secuencias
#include "BehaviorTree/Composites/BTComposite_Sequence.h"
#include "BehaviorTree/Composites/BTComposite_Selector.h"

//Tasks
#include "MyBTTask_RotateToFaceBBEntry.h"
#include "BTTask_ChasePlayer.h"
#include "MyBTTask_MoveTo.h"
#include "BTTask_FindRandomPatrol.h"
#include "BehaviorTree/Tasks/BTTask_Wait.h"

//KeyTypes
#include <BehaviorTree/Blackboard/BlackboardKeyType_Object.h>
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

//Decorators
#include "HasLineOfSight_Decorator.h"

#include "BTConstructor.h"
#include "BTFactory.h"

#include <string>
#include <cstdlib>


AEnemyControllerCode::AEnemyControllerCode()
{
	_blackBoard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoard")); //Crea el componente de pizarra
	_behaviorTree = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree")); //Crea el componente de árbol de comportamiento

	_perception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception")); //Crea el componente de Percepción

	_sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight")); //Crea la configuración de la vista

	_sightConfig->SightRadius = 3000.f;
	_sightConfig->LoseSightRadius = 3500.f;

	_sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	_sightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	_perception->ConfigureSense(*_sightConfig);
	_perception->SetDominantSense(_sightConfig->GetSenseImplementation());
}

void AEnemyControllerCode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Controller %s posee Pawn %s"),
		*GetName(), *GetPawn()->GetName());

	if (_perception) {
		_perception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyControllerCode::OnPerceptionUpdated);
	}

	BTFactory::Init();
	BTFactory* f = BTFactory::Instance();

	f->RegisterTask<UMyBTTask_RotateToFaceBBEntry>("RotateToFaceBBEntry");
	f->RegisterTask<UBTTask_ChasePlayer>("ChasePlayer");
	f->RegisterTask<UMyBTTask_MoveTo>("MoveTo");
	f->RegisterTask<UBTTask_FindRandomPatrol>("FindRandomPatrol");
	f->RegisterTask<UBTTask_Wait>("Wait");

	f->RegisterDecorator<UHasLineOfSight_Decorator>("HasLineOfSight?");

	BTConstructor::Init();

	/*std::string command = "uv run C:/Users/pablo/Desktop/Uni/Master/TFM/llm-bt-generator/main.py ";
	command += f->getAllTasks();
	command += " ";
	command += f->getAllDecorators();
	command += " ";
	command += "\"NPC that, constantly, select a random point from the map, goes to the point and waits a time.\"";
	command += " ";
	command += "Ejemplo_1";*/

	FString t = f->getAllTasks().c_str();
	FString d = f->getAllTasks().c_str();
	FString prompt = TEXT("NPC that, constantly, select a random point from the map, goes to the point and waits a time.");
	FString p = TEXT("Ejemplo_1");

	FString UVExe = TEXT("C:/Users/pablo/Desktop/Uni/Master/TFM/llm-bt-generator/.venv/Scripts/uv.exe");

	FString ScriptPath = TEXT("C:/Users/pablo/Desktop/Uni/Master/TFM/llm-bt-generator/main.py");
	FString Args = FString::Printf(
		TEXT("run \"%s\" %s %s \"%s\" %s"),
		*ScriptPath,
		*t,
		*d,
		*prompt,
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



	UBehaviorTree* Root = BTConstructor::Instance()->CreateBT("Ejemplo1.json", _blackBoard);
	UseBlackboard(Root->BlackboardAsset, _blackBoard);
	RunBehaviorTree(Root);
//
//#pragma region BLACKBOARD
//	UBlackboardData* BBAsset = NewObject<UBlackboardData>(this);
//
//	FBlackboardEntry entry;
//	entry.EntryName = "EnemyActor";
//	entry.KeyType = NewObject<UBlackboardKeyType_Object>();
//	BBAsset->Keys.Add(entry);
//
//	entry = FBlackboardEntry();
//	entry.EntryName = "HasLineOfSight";
//	entry.KeyType = NewObject<UBlackboardKeyType_Bool>();
//	BBAsset->Keys.Add(entry);
//
//	entry = FBlackboardEntry();
//	entry.EntryName = "PatrolLocation";
//	entry.KeyType = NewObject<UBlackboardKeyType_Vector>();
//	BBAsset->Keys.Add(entry);
//
//	if (!_blackBoard->InitializeBlackboard(*BBAsset)) //Inicializa la pizarra. 
//		return;
//
//#pragma endregion
//
//#pragma region CHASE
//	UBTComposite_Sequence* ChasePlayerNode = NewObject<UBTComposite_Sequence>(this);
//	ChasePlayerNode->NodeName = "Chase Player";
//
//	UMyBTTask_RotateToFaceBBEntry* RotateToEnemyNode = NewObject<UMyBTTask_RotateToFaceBBEntry>(this);
//	RotateToEnemyNode->SetPrecision(10.f);
//	RotateToEnemyNode->SetKeyName("EnemyActor");
//	RotateToEnemyNode->ResolveKey(BBAsset);
//	FBTCompositeChild RotateChild;
//	RotateChild.ChildTask = RotateToEnemyNode;
//
//	UBTTask_ChasePlayer* ChasePlayerTaskNode = NewObject<UBTTask_ChasePlayer>(this);
//	ChasePlayerTaskNode->SetWalkSpeed(500.f);
//	FBTCompositeChild ChasePlayerChild;
//	ChasePlayerChild.ChildTask = ChasePlayerTaskNode;
//
//	UMyBTTask_MoveTo* MoveToNode = NewObject<UMyBTTask_MoveTo>(this);
//	MoveToNode->SetKeyName("EnemyActor");
//	MoveToNode->ResolveKey(BBAsset);
//	FBTCompositeChild MoveToChild;
//	MoveToChild.ChildTask = MoveToNode;
//
//	ChasePlayerNode->Children.Add(RotateChild);
//	ChasePlayerNode->Children.Add(ChasePlayerChild);
//	ChasePlayerNode->Children.Add(MoveToChild);
//
//	FBTCompositeChild ChasePlayer(ChasePlayerNode);
//	UHasLineOfSight_Decorator* SightDecorator = NewObject<UHasLineOfSight_Decorator>(this);
//	SightDecorator->SetKeyName("HasLineOfSight");
//	SightDecorator->ResolveKey(BBAsset);
//	ChasePlayer.Decorators.Add(SightDecorator);
//#pragma endregion
//
//#pragma region PATROL
//	UBTComposite_Sequence* PatrolNode = NewObject<UBTComposite_Sequence>(this);
//	PatrolNode->NodeName = "Patrol";
//
//	UBTTask_FindRandomPatrol* FindRandomNode = NewObject<UBTTask_FindRandomPatrol>(this);
//	FindRandomNode->SetPatrolRadius(1000.f);
//	FBTCompositeChild FindRandomChild;
//	FindRandomChild.ChildTask = FindRandomNode;
//
//	UMyBTTask_MoveTo* MoveToPatrolNode = NewObject<UMyBTTask_MoveTo>(this);
//	MoveToPatrolNode->SetKeyName("PatrolLocation");
//	MoveToPatrolNode->ResolveKey(BBAsset);
//	FBTCompositeChild MoveToPatrolChild;
//	MoveToPatrolChild.ChildTask = MoveToPatrolNode;
//
//	UBTTask_Wait* WaitPatrolNode = NewObject<UBTTask_Wait>(this);
//	WaitPatrolNode->WaitTime = 4.f;
//	WaitPatrolNode->RandomDeviation = 1.f;
//	FBTCompositeChild WaitPatrolChild;
//	WaitPatrolChild.ChildTask = WaitPatrolNode;
//
//	PatrolNode->Children.Add(FindRandomChild);
//	PatrolNode->Children.Add(MoveToPatrolChild);
//	PatrolNode->Children.Add(WaitPatrolChild);
//
//	FBTCompositeChild Patrol(PatrolNode);
//#pragma endregion
//
//#pragma region WAIT
//	UBTTask_Wait* WaitNode = NewObject<UBTTask_Wait>(this);
//	WaitNode->WaitTime = 1.f;
//
//	FBTCompositeChild Wait;
//	Wait.ChildTask = WaitNode;
//#pragma endregion
//
//	UBTComposite_Selector* RootNode = NewObject<UBTComposite_Selector>(this);
//	RootNode->NodeName = "Root";
//
//	RootNode->Children.Add(ChasePlayer);
//	RootNode->Children.Add(Patrol);
//	RootNode->Children.Add(Wait);
//
//	UBehaviorTree* Root = NewObject<UBehaviorTree>(this);
//	Root->RootNode = RootNode;
//	Root->BlackboardAsset = BBAsset;
//
//	UseBlackboard(BBAsset, _blackBoard);
//	RunBehaviorTree(Root);
}

void AEnemyControllerCode::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor || !GetBlackboardComponent()) return;

	if (Actor->ActorHasTag("Player") && Stimulus.WasSuccessfullySensed()) {
		GetBlackboardComponent()->SetValueAsBool("HasLineOfSight", true);
		GetBlackboardComponent()->SetValueAsObject("EnemyActor", Actor);

		GetWorld()->GetTimerManager().ClearTimer(_timerHandle);
		GetWorld()->GetTimerManager().SetTimer(_timerHandle, [this]() {
			GetBlackboardComponent()->SetValueAsBool("HasLineOfSight", false);
			GetBlackboardComponent()->SetValueAsObject("EnemyActor", nullptr);
			}, 3.f, false);
	}
}
