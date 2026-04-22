// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <list>
#include <string>

/**
 * (
 */
class BTGENERATOR_API BehaviorList
{
public:
	BehaviorList();
	~BehaviorList();

	static BehaviorList* Instance();
	static bool Init();
	static void Release();

	void addBehavior(std::string behavior);
	std::string getBehaviors();
private:
	bool init();
	static BehaviorList* _instance;

	std::list<std::string> _behaviors;
};
