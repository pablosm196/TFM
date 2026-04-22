// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorList.h"

BehaviorList* BehaviorList::_instance = nullptr;

BehaviorList::BehaviorList()
{
	_behaviors = std::list<std::string>();
}

BehaviorList::~BehaviorList()
{
}

BehaviorList* BehaviorList::Instance() {
	return _instance;
}

bool BehaviorList::Init() {
	if (_instance == nullptr) {
		_instance = new BehaviorList();
		return _instance->init();
	}
	return true;
}

void BehaviorList::Release() {
	delete _instance;
	_instance = nullptr;
}

void BehaviorList::addBehavior(std::string behavior)
{
	_behaviors.push_back(behavior);
}

std::string BehaviorList::getBehaviors()
{
	std::string behaviors = "";

	for (std::string b : _behaviors)
		behaviors += b + "\n";

	return behaviors;
}

bool BehaviorList::init() {
	return true;
}
