// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorList.h"

#include <fstream>


BehaviorList* BehaviorList::_instance = nullptr;

BehaviorList::BehaviorList()
{
	
}

BehaviorList::~BehaviorList()
{
}

BehaviorList* BehaviorList::Instance() {
	return _instance;
}

bool BehaviorList::Init(std::string path, IValidatorCallback* callback) {
	if (_instance == nullptr) {
		_instance = new BehaviorList();
		return _instance->init(path, callback);
	}
	return true;
}

void BehaviorList::Release() {
	delete _instance;
	_instance = nullptr;
}

void BehaviorList::addBehavior(FString& behavior)
{
	if (behavior.Equals("Final")) {
		_callback->OnValidationEnds();
		return;
	}

	_behaviors.Add(behavior);

	TSet<int> newStates;

	TArray<int> candidates;

	_namesToNodes.MultiFind(behavior, candidates);

	if (candidates.Num() == 0) //NODO IRRELEVANTE.
		return;

	for (int c : candidates) {
		if (_nodes[c]._parentId == INDEX_NONE) {
			newStates.Add(c);
		}
	}

	for (int activeNodeId : _activeStates) {
		const NodesDefinition& activeNode = _nodes[activeNodeId];

		for (int childId : activeNode._children) {
			if (_nodes[childId]._task.Equals(behavior)) {
				newStates.Add(childId);
			}
		}
	}

	if (newStates.IsEmpty()) {

		FString message = "The node order has not been fulfilled. The list of executed tasks is as follows: " +
			getBehaviors() +
			"\nThe execution failed on task " +
			behavior;

		_callback->OnValidationFails(message);
	}

	_activeStates = MoveTemp(newStates);
}

FString BehaviorList::getBehaviors()
{
	FString behaviors = "";

	for (FString b : _behaviors)
		behaviors += b + ", ";

	return behaviors;
}

bool BehaviorList::init(std::string path, IValidatorCallback* callback)
{
	_callback = callback;

	std::string route = std::string(TCHAR_TO_UTF8(*FPaths::ProjectContentDir())).append(BASE_ROUTE).append(path).append(".json");
	std::ifstream f(route);
	json data = json::parse(f);

	addTask(data["Tasks"], INDEX_NONE);

	return true;
}

void BehaviorList::addTask(json data, int parentID)
{
	for (auto it : data) {
		std::string name = it["Name"];

		int newID = _nodes.Num();

		NodesDefinition node;
		node._id = newID;
		node._task = name.c_str();
		node._parentId = parentID;

		_nodes.Add(node);

		_namesToNodes.Add(name.c_str(), newID);

		if (parentID != INDEX_NONE) {
			_nodes[parentID]._children.Add(newID);
		}

		addTask(it["Childs"], newID);
	}
}
