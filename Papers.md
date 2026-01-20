# LLM-BRAIn: AI-driven Fast Generation of Robot Behaviour Tree based on Large Language Model

# BB y BT en Unreal C++
````cpp
// BehaviorTreeBuilder.h
#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTComposite_Sequence.h"
#include "BehaviorTree/BTTaskNode.h"

class FBehaviorTreeBuilder
{
public:
    FBehaviorTreeBuilder(UObject* InOwner);

    // Blackboard
    void AddVectorKey(FName KeyName);

    // Nodos
    UBTComposite_Sequence* CreateSequence();
    UBTTaskNode* AddTask(UClass* TaskClass);
    UBTTaskNode* AddWait(float Seconds);
    UBTTaskNode* AddMoveTo(FName BlackboardKey);

    // Finalizar
    void BuildAndRun(class AAIController* Controller);

private:
    UObject* Owner;
    UBlackboardComponent* BlackboardComp;
    UBehaviorTreeComponent* BehaviorComp;
    UBTCompositeNode* RootNode;
};
````
````cpp
// BehaviorTreeBuilder.cpp
#include "BehaviorTreeBuilder.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/BTTask_MoveTo.h"
#include "BehaviorTree/BTTask_Wait.h"

FBehaviorTreeBuilder::FBehaviorTreeBuilder(UObject* InOwner)
{
    Owner = InOwner;
    BlackboardComp = NewObject<UBlackboardComponent>(Owner);
    BehaviorComp   = NewObject<UBehaviorTreeComponent>(Owner);

    // Crear Blackboard vacío
    BlackboardComp->InitializeBlackboard(*NewObject<UBlackboardData>());
}

void FBehaviorTreeBuilder::AddVectorKey(FName KeyName)
{
    FBlackboardEntry Entry;
    Entry.EntryName = KeyName;
    Entry.KeyType = NewObject<UBlackboardKeyType_Vector>();
    BlackboardComp->GetBlackboardAsset()->Keys.Add(Entry);
}

UBTComposite_Sequence* FBehaviorTreeBuilder::CreateSequence()
{
    RootNode = NewObject<UBTComposite_Sequence>(Owner);
    return Cast<UBTComposite_Sequence>(RootNode);
}

UBTTaskNode* FBehaviorTreeBuilder::AddTask(UClass* TaskClass)
{
    UBTTaskNode* Task = NewObject<UBTTaskNode>(Owner, TaskClass);
    RootNode->Children.Add(FBTCompositeChild(Task));
    return Task;
}

UBTTaskNode* FBehaviorTreeBuilder::AddMoveTo(FName BlackboardKey)
{
    UBTTask_MoveTo* MoveToTask = NewObject<UBTTask_MoveTo>(Owner);
    MoveToTask->BlackboardKey.SelectedKeyName = BlackboardKey;
    RootNode->Children.Add(FBTCompositeChild(MoveToTask));
    return MoveToTask;
}

UBTTaskNode* FBehaviorTreeBuilder::AddWait(float Seconds)
{
    UBTTask_Wait* WaitTask = NewObject<UBTTask_Wait>(Owner);
    WaitTask->WaitTime = Seconds;
    RootNode->Children.Add(FBTCompositeChild(WaitTask));
    return WaitTask;
}

void FBehaviorTreeBuilder::BuildAndRun(AAIController* Controller)
{
    BehaviorComp->StartTree(*RootNode);
}
````
`````cpp
#include "MyAIController.h"
#include "BehaviorTreeBuilder.h"
#include "BTTask_FindRandomLocation.h"

void AMyAIController::BeginPlay()
{
    Super::BeginPlay();

    FBehaviorTreeBuilder Builder(this);

    // Blackboard
    Builder.AddVectorKey("TargetLocation");

    // Secuencia
    UBTComposite_Sequence* Seq = Builder.CreateSequence();
    Builder.AddTask(UBTTask_FindRandomLocation::StaticClass());
    Builder.AddMoveTo("TargetLocation");
    Builder.AddWait(4.0f);

    // Ejecutar
    Builder.BuildAndRun(this);
}
``````
