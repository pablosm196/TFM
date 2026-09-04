// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "BehaviorList.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

#include "json.hpp"
using json = nlohmann::json;

#include "ValidatorCallback.h"

#include "Chaos/Pair.h"


#include "BTGeneratorComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BTGENERATOR_API UBTGeneratorComponent : public UActorComponent, public IValidatorCallback
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBTGeneratorComponent();

	inline FString getBTName() { return _BTName; }
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, meta = (MultiLine=true))
	FString _prompt;

	UPROPERTY(EditAnywhere)
	FString _BTName;

	UPROPERTY(EditAnywhere)
	FString _validationPath;

	UPROPERTY(EditAnywhere)
	TMap<FString, FString> _blackboardEntries;

	UPROPERTY(EditAnywhere)
	APawn* _pawn;

	UPROPERTY(EditAnywhere)
	FString _scriptPath;

	UFUNCTION(CallInEditor)
	void GenerateBT();

	BehaviorList* _list;

	FTimerHandle _timerHandle;


	void CallGenerator(FString prompt);


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OnValidationFails(FString message) override;
	void OnValidationEnds() override;
	void CheckValidationByTimer();
};
