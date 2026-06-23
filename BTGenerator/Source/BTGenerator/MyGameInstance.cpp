// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance() : UGameInstance() {
	_redFlowers = TArray<TObjectPtr<AActor>>();
	_blueFlowers = TArray<TObjectPtr<AActor>>();
	_yellowFlowers = TArray<TObjectPtr<AActor>>();
	_blackFlowers = TArray<TObjectPtr<AActor>>();
	_waypoints = TArray<TObjectPtr<AActor>>();

	_numRed = 0;
	_numBlue = 0;
	_numYellow = 0;
	_numBlack = 0;

	_playerDied = false;
}

void UMyGameInstance::addRedFlower(AActor* flower)
{
	_redFlowers.Add(flower);
}

void UMyGameInstance::addBlueFlower(AActor* flower)
{
	_blueFlowers.Add(flower);
}

void UMyGameInstance::addYellowFlower(AActor* flower)
{
	_yellowFlowers.Add(flower);

}

void UMyGameInstance::addBlackFlower(AActor* flower)
{
	_blackFlowers.Add(flower);
}

void UMyGameInstance::addWaypoint(AActor* waypoint)
{
	_waypoints.Add(waypoint);
}

AActor* UMyGameInstance::getRedFlower()
{
	if (_redFlowers.Num() == 0) return nullptr;
	AActor* flower = _redFlowers[0];
	//_redFlowers.RemoveAt(0);
	return flower;
}

AActor* UMyGameInstance::getBlueFlower()
{
	if (_blueFlowers.Num() == 0) return nullptr;
	AActor* flower = _blueFlowers[0];
	//_blueFlowers.RemoveAt(0);
	return flower;
}

AActor* UMyGameInstance::getYellowFlower()
{
	if (_yellowFlowers.Num() == 0) return nullptr;
	AActor* flower = _yellowFlowers[0];
	//_yellowFlowers.RemoveAt(0);
	return flower;
}

AActor* UMyGameInstance::getBlackFlower()
{
	if (_blackFlowers.Num() == 0) return nullptr;
	AActor* flower = _blackFlowers[0];
	//_blackFlowers.RemoveAt(0);
	return flower;
}

AActor* UMyGameInstance::getRandomWaypoint()
{
	int i = FMath::RandRange(0, _waypoints.Num() - 1);

	return _waypoints[i];
}

void UMyGameInstance::flowerPicked(FString color)
{
	if (color == "red")
		_numRed++;
	else if (color == "blue")
		_numBlue++;
	else if (color == "yellow")
		_numYellow++;
	else if (color == "black")
		_numYellow++;
}

void UMyGameInstance::resetLists()
{
	_redFlowers.Empty();
	_blueFlowers.Empty();
	_yellowFlowers.Empty();
	_blackFlowers.Empty();
	_waypoints.Empty();

	_numRed = 0;
	_numBlue = 0;
	_numYellow = 0;
	_numBlack = 0;

	_playerDied = false;
}
