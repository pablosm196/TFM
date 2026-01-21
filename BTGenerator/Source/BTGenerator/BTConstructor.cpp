// Fill out your copyright notice in the Description page of Project Settings.


#include "BTConstructor.h"

#include <fstream>

#include <BehaviorTree/Blackboard/BlackboardKeyType_Object.h>
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_String.h"

#include "BehaviorTree/Composites/BTComposite_Sequence.h"
#include "BehaviorTree/Composites/BTComposite_Selector.h"
#include "BehaviorTree/Composites/BTComposite_SimpleParallel.h"

#include "BTFactory.h"
#include "BaseTask.h"
#include "BaseDecorator.h"



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
	if (BBAsset == nullptr)
		return nullptr;

	FBTCompositeChild RootNode = CreateNode(data["Node"], BBAsset);
	UBehaviorTree* Root = NewObject<UBehaviorTree>();

	Root->RootNode = RootNode.ChildComposite;
	Root->BlackboardAsset = BBAsset;
	return Root;
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

FBTCompositeChild BTConstructor::CreateNode(json data, UBlackboardData* BBAsset)
{
	FBTCompositeChild composite;

	if (data["Type"] != "Task") {
		UBTCompositeNode* compositeNode = nullptr;
		if (data["Type"] == "Selector")
			compositeNode = NewObject<UBTComposite_Selector>();
		else if (data["Type"] == "Sequence")
			compositeNode = NewObject<UBTComposite_Sequence>();
		else if (data["Type"] == "Parallel")
			compositeNode = NewObject<UBTComposite_SimpleParallel>();
		else FBTCompositeChild();

		for (auto it : data["Nodes"]) {
			FBTCompositeChild child = CreateNode(it["Node"], BBAsset);
			compositeNode->Children.Add(child);
		}

		composite = FBTCompositeChild(compositeNode);
	}
	else {
		UBTTaskNode* task = BTFactory::Instance()->GetTask(data["Task"]);

		if(task == nullptr)
			return FBTCompositeChild();

		composite.ChildTask = task;

		if (data.contains("BlackboardEntries")) {
			for (auto& it : data["BlackboardEntries"]) {
				dynamic_cast<IBaseTask*>(task)->SetKeyName(FName(std::string(it).c_str()));
				dynamic_cast<IBaseTask*>(task)->ResolveKey(BBAsset);
			}
		}
	}

	for (auto& it : data["Decorators"]) {
		for (auto& ot : it.items()) {

			std::string key = ot.key();
			std::string value = ot.value();

			UBTDecorator* decorator = BTFactory::Instance()->GetDecorator(key);
			if (value != "") {
				if (dynamic_cast<UBaseDecorator*>(decorator) != nullptr) {
					dynamic_cast<UBaseDecorator*>(decorator)->SetKeyName(FName(std::string(value).c_str()));
					dynamic_cast<UBaseDecorator*>(decorator)->ResolveKey(BBAsset);
				}
			}
			composite.Decorators.Add(decorator);
		}
	}

	return composite;
}