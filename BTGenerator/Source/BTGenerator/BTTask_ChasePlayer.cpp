// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChasePlayer.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

UBTTask_ChasePlayer::UBTTask_ChasePlayer()
{
	NodeName = "Chase Player Task";
}

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return EBTNodeResult::Failed;

	APawn* Pawn = Controller->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;

	ACharacter* Character = Cast<ACharacter>(Pawn);
	if (!Character) return EBTNodeResult::Failed;

	UCharacterMovementComponent* CharacterMovementComponent = Character->GetCharacterMovement(); //Puedes acceder a cualquier componente mediante Pawn->GetComponentByClass<T>();
	if (!CharacterMovementComponent) return EBTNodeResult::Failed;

	CharacterMovementComponent->MaxWalkSpeed = NewWalkSpeed;

	return EBTNodeResult::Succeeded;
}
