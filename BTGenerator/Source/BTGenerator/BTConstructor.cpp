// Fill out your copyright notice in the Description page of Project Settings.


#include "BTConstructor.h"

#include <fstream>

#include "BehaviorTree/BlackboardComponent.h"

#include <BehaviorTree/Blackboard/BlackboardKeyType_Object.h>
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_String.h"



BTConstructor* BTConstructor::_instance = nullptr;

BTConstructor::BTConstructor()
{

}

BTConstructor::~BTConstructor()
{

}

BTConstructor* BTConstructor::Instance() {
	return _instance;
}

bool BTConstructor::Init() {
	if (_instance == nullptr) {
		_instance = new BTConstructor();
		return _instance->init();
	}
	return true;
}

void BTConstructor::Release() {
	delete _instance;
}

UBehaviorTree* BTConstructor::CreateBT(std::string file, UBlackboardComponent* blackboard) {
	std::string route = std::string(TCHAR_TO_UTF8(*FPaths::ProjectContentDir())).append(BASE_ROUTE).append(file);
	std::ifstream f(route);
	json data = json::parse(f);
	
	UBlackboardData* BBAsset = CreateBlackboardAsset(data["Blackboard"], blackboard);

	return nullptr;
}

bool BTConstructor::init() {
	return true;
}

UBlackboardData* BTConstructor::CreateBlackboardAsset(json data, UBlackboardComponent* blackboard) {
	UBlackboardData* BBAsset = NewObject<UBlackboardData>();

	for (auto& it : data) {

		FBlackboardEntry entry;

		for (auto& ot : it.items()) {

			std::string key = ot.key();
			std::string value = ot.value();

			entry.EntryName = FName(key.c_str());

			if(value == "Bool")
				entry.KeyType = NewObject<UBlackboardKeyType_Bool>();
			else if(value == "Int")
				entry.KeyType = NewObject<UBlackboardKeyType_Int>();
			else if (value == "Float")
				entry.KeyType = NewObject<UBlackboardKeyType_Float>();
			else if (value == "Vector")
				entry.KeyType = NewObject<UBlackboardKeyType_Vector>();
			else if (value == "Object")
				entry.KeyType = NewObject<UBlackboardKeyType_Object>();
			else if (value == "String")
				entry.KeyType = NewObject<UBlackboardKeyType_String>();
		}

		BBAsset->Keys.Add(entry);
	}

	if (!blackboard->InitializeBlackboard(*BBAsset))
		return nullptr;

	return BBAsset;
}