// Fill out your copyright notice in the Description page of Project Settings.
#include "EnemyControllerCode.h"

#include "EngineUtils.h"
#include "BTGeneratorComponent.h"

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

void AEnemyControllerCode::SetBT()
{
	if (_btPath.IsEmpty()) return;
	_root = BTConstructor::Instance()->CreateBT(TCHAR_TO_UTF8(*_btPath), _blackBoard);
	UseBlackboard(_root->BlackboardAsset, _blackBoard);
	RunBehaviorTree(_root);


	_btParsed = true;
}

void AEnemyControllerCode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Controller %s posee Pawn %s"),
		*GetName(), *GetPawn()->GetName());

	if (_perception) {
		_perception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyControllerCode::OnPerceptionUpdated);
	}

	_btParsed = false;

	FString name = "";

	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		if (It->Tags.Contains("Generator"))
		{
			if (UBTGeneratorComponent* Comp = It->FindComponentByClass<UBTGeneratorComponent>())
			{
				name = Comp->getBTName();
				break;
			}
		}
	}

	_btPath = name;

	GetWorld()->GetTimerManager().SetTimer(
		_timerHandle,
		this,
		&AEnemyControllerCode::SetBT,
		1.0f,
		false
	);
}

void AEnemyControllerCode::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor || !GetBlackboardComponent() || !_btParsed) return;

	if (Actor->ActorHasTag("Objective") && Stimulus.WasSuccessfullySensed()) {
		GetBlackboardComponent()->SetValueAsBool("HasLineOfSight", true);
		GetBlackboardComponent()->SetValueAsObject("Objective", Actor);

		GetWorld()->GetTimerManager().ClearTimer(_timerHandle);
		GetWorld()->GetTimerManager().SetTimer(_timerHandle, [this]() {
			GetBlackboardComponent()->SetValueAsBool("HasLineOfSight", false);
			GetBlackboardComponent()->SetValueAsObject("Objective", nullptr);
			}, 3.f, false);
	}
}
