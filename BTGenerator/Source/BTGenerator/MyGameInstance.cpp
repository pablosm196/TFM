// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance() : UGameInstance() {
	_redFlowers = TArray<TObjectPtr<AActor>>();
	_blueFlowers = TArray<TObjectPtr<AActor>>();
	_yellowFlowers = TArray<TObjectPtr<AActor>>();
	_blackFlowers = TArray<TObjectPtr<AActor>>();
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

void UMyGameInstance::resetLists()
{
	_redFlowers.Empty();
	_blueFlowers.Empty();
	_yellowFlowers.Empty();
	_blackFlowers.Empty();
}
