// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance() : UGameInstance() {
	_redFlowers = std::list<AActor*>();
	_blueFlowers = std::list<AActor*>();
	_yellowFlowers = std::list<AActor*>();
	_blackFlowers = std::list<AActor*>();
}

void UMyGameInstance::addRedFlower(AActor* flower)
{
	_redFlowers.push_back(flower);
}

void UMyGameInstance::addBlueFlower(AActor* flower)
{
	_blueFlowers.push_back(flower);
}

void UMyGameInstance::addYellowFlower(AActor* flower)
{
	_yellowFlowers.push_back(flower);

}

void UMyGameInstance::addBlackFlower(AActor* flower)
{
	_blackFlowers.push_back(flower);
}

AActor* UMyGameInstance::getRedFlower()
{
	AActor* flower = _redFlowers.front();
	_redFlowers.pop_front();
	return flower;
}

AActor* UMyGameInstance::getBlueFlower()
{
	AActor* flower = _blueFlowers.front();
	_blueFlowers.pop_front();
	return flower;
}

AActor* UMyGameInstance::getYellowFlower()
{
	AActor* flower = _yellowFlowers.front();
	_yellowFlowers.pop_front();
	return flower;
}

AActor* UMyGameInstance::getBlackFlower()
{
	AActor* flower = _blackFlowers.front();
	_blackFlowers.pop_front();
	return flower;
}

void UMyGameInstance::resetLists()
{
	_redFlowers.clear();
	_blueFlowers.clear();
	_yellowFlowers.clear();
	_blackFlowers.clear();
}
