// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

#include "BTGeneratorComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BTGENERATOR_API UBTGeneratorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBTGeneratorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, meta = (MultiLine=true))
	FString _prompt;

	UPROPERTY(EditAnywhere)
	FString _BTName;

	UPROPERTY(EditAnywhere)
	APawn* _pawn;

	UFUNCTION(CallInEditor)
	void GenerateBT();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
};
