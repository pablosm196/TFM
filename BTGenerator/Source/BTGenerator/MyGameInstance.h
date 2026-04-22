// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include <list>

#include "MyGameInstance.generated.h"
/**
 * 
 */
UCLASS()
class BTGENERATOR_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UMyGameInstance();

	UFUNCTION(BlueprintCallable)
	void addRedFlower(AActor* flower);
	UFUNCTION(BlueprintCallable)
	void addBlueFlower(AActor* flower);
	UFUNCTION(BlueprintCallable)
	void addYellowFlower(AActor* flower);
	UFUNCTION(BlueprintCallable)
	void addBlackFlower(AActor* flower);

	UFUNCTION()
	AActor* getRedFlower();
	UFUNCTION()
	AActor* getBlueFlower();
	UFUNCTION()
	AActor* getYellowFlower();
	UFUNCTION()
	AActor* getBlackFlower();

	void resetLists();
protected:
	std::list<AActor*> _redFlowers;
	std::list<AActor*> _blueFlowers;
	std::list<AActor*> _yellowFlowers;
	std::list<AActor*> _blackFlowers;
};
