#pragma once

#include <unordered_map>
#include <string>

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BTDecorator.h"

#include "CoreMinimal.h"


/**
 * 
 */
class BTGENERATOR_API BTFactory
{
public:
	BTFactory();
	~BTFactory();

	static BTFactory* Instance();
	static bool Init();
	static void Release();

	template<typename T>
	void RegisterTask(std::string ID) {
		if (!TaskExists(ID))
			_currentTasks.emplace(ID, &BTFactory::CreateObject<T>);
	}

	template <typename T>
	void RegisterDecorator(std::string ID) {
		if (!DecoratorExists(ID))
			_currentDecorators.emplace(ID, &BTFactory::CreateObject<T>);
	}

	UBTTask_BlackboardBase* GetTask(std::string ID);
	UBTDecorator* GetDecorator(std::string ID);
private:
	static BTFactory* _instance;

	std::unordered_map<std::string, UBTTask_BlackboardBase* (*)()> _currentTasks;
	std::unordered_map<std::string, UBTDecorator* (*)()> _currentDecorators;

	static bool init();

	bool TaskExists(std::string ID);
	bool DecoratorExists(std::string ID);

	template <typename T>
	static UBTTask_BlackboardBase* CreateObject() {
		return NewObject<T>();
	}
};
