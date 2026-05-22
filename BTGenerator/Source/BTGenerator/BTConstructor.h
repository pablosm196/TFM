#pragma once

#include <string>
#include "json.hpp"
using json = nlohmann::json;

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "CoreMinimal.h"

/**
 * 
 */
class BTGENERATOR_API BTConstructor
{
public:
	BTConstructor();
	~BTConstructor();

	static BTConstructor* Instance();
	static bool Init();
	static void Release();

	UBehaviorTree* CreateBT(std::string file, UBlackboardComponent* blackboard);
private:
	static BTConstructor* _instance;
	std::string BASE_ROUTE = "/TFM/JSONs/";

	static bool init();
	UBlackboardData* CreateBlackboardAsset(json data, UBlackboardComponent* blackboard, UObject* outer);
	FBTCompositeChild CreateNode(json data, UBlackboardData* BBAsset, UBehaviorTree* ownerTree);
};
