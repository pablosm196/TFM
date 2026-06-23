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

	UFUNCTION(BlueprintCallable)
	void addWaypoint(AActor* waypoint);

	UFUNCTION()
	AActor* getRedFlower();
	UFUNCTION()
	AActor* getBlueFlower();
	UFUNCTION()
	AActor* getYellowFlower();
	UFUNCTION()
	AActor* getBlackFlower();

	UFUNCTION()
	AActor* getRandomWaypoint();

	void flowerPicked(FString color);

	inline int getNumRed() { return _numRed; }
	inline int getNumBlue() { return _numBlue; }
	inline int getNumYellow() { return _numYellow; }
	inline int getNumBlack() { return _numBlack; }

	inline void setPlayerDead() { _playerDied = true; }
	inline bool isPlayerDead() { return _playerDied; }

	void resetLists();
protected:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> _redFlowers;
	UPROPERTY()
	TArray<TObjectPtr<AActor>> _blueFlowers;
	UPROPERTY()
	TArray<TObjectPtr<AActor>> _yellowFlowers;
	UPROPERTY()
	TArray<TObjectPtr<AActor>> _blackFlowers;
	UPROPERTY()
	TArray<TObjectPtr<AActor>> _waypoints;

	int _numRed;
	int _numBlue;
	int _numYellow;
	int _numBlack;

	bool _playerDied;
};
