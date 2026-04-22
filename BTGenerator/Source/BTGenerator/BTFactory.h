#pragma once

#include <unordered_map>
#include <string>

#include "BehaviorTree/BTTaskNode.h"
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
			_currentTasks.emplace(ID, &BTFactory::CreateTask<T>);
	}
	template<typename T>
	void RegisterBlackboardTask(std::string ID) {
		if (!TaskExists(ID))
			_currentBBTasks.emplace(ID, &BTFactory::CreateTask<T>);
	}

	template <typename T>
	void RegisterDecorator(std::string ID) {
		if (!DecoratorExists(ID))
			_currentDecorators.emplace(ID, &BTFactory::CreateDecorator<T>);
	}

	UBTTaskNode* GetTask(std::string ID);
	UBTDecorator* GetDecorator(std::string ID);

	std::string getAllTasks();
	std::string getAllBlackboardTasks();
	std::string getAllDecorators();
private:
	static BTFactory* _instance;

	std::unordered_map<std::string, UBTTaskNode* (*)()> _currentTasks;
	std::unordered_map<std::string, UBTTaskNode* (*)()> _currentBBTasks;
	std::unordered_map<std::string, UBTDecorator* (*)()> _currentDecorators;

	static bool init();

	bool TaskExists(std::string ID);
	bool DecoratorExists(std::string ID);

	template <typename T>
	static UBTTaskNode* CreateTask() {
		return NewObject<T>();
	}
	template <typename T>
	static UBTDecorator* CreateDecorator() {
		return NewObject<T>();
	}
};
