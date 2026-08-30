// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <list>
#include <string>

#include "ValidatorCallback.h"
#include "ObjectiveValidatorBase.h"

#include "json.hpp"
using json = nlohmann::json;

/**
 * (
 */
class BTGENERATOR_API BehaviorList
{
public:
	BehaviorList();
	~BehaviorList();

	static BehaviorList* Instance();
	static bool Init(std::string path, IValidatorCallback* callback, IObjectiveValidatorBase* objectiveValidator);
	static void Release();

	void addBehavior(FString& behavior);
	FString getBehaviors();
private:
	bool init(std::string path, IValidatorCallback* callback, IObjectiveValidatorBase* objectiveValidator);

	void addTask(json data, int parentID);

	static BehaviorList* _instance;

	IValidatorCallback* _callback;
	IObjectiveValidatorBase* _objectiveValidator;

	struct NodesDefinition {
		FString _task;
		int _id = INDEX_NONE;
		int _parentId = INDEX_NONE;
		TArray<int> _children;
	};


	/*TMap<FString, TArray<FString>> _dependenciesGraph;
	TMap<FString, FString> _immediateAfterGraph;
*/

	std::string BASE_ROUTE = "/TFM/JSONs/";

	TArray<FString> _behaviors;

	TArray<NodesDefinition> _nodes;
	TMultiMap<FString, int> _namesToNodes;
	TSet<int> _activeStates;

	bool _initialized;
};
