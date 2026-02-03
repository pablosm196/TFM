#include "BTFactory.h"
#include <string>

BTFactory* BTFactory::_instance = nullptr;

BTFactory::BTFactory()
{
}

BTFactory::~BTFactory()
{
}

BTFactory* BTFactory::Instance() {
	return _instance;
}

bool BTFactory::Init() {
	if (_instance == nullptr) {
		_instance = new BTFactory();
		return _instance->init();
	}
	return true;
}

void BTFactory::Release() {
	delete _instance;
	_instance = nullptr;
}

UBTTaskNode* BTFactory::GetTask(std::string ID) {
	auto it = _currentTasks.find(ID);

	if (it == _currentTasks.end()) return nullptr;

	return static_cast<UBTTaskNode*>(it->second());
}

UBTDecorator* BTFactory::GetDecorator(std::string ID)
{
	auto it = _currentDecorators.find(ID);

	if (it == _currentDecorators.end()) return nullptr;

	return static_cast<UBTDecorator*>(it->second());
}

std::string BTFactory::getAllTasks()
{
	std::string tasks = "";

	for (auto it = _currentTasks.begin(); it != _currentTasks.end(); it++) {
		tasks += it->first + ",";
	}

	if (!tasks.empty())
		tasks.pop_back();

	return tasks;
}

std::string BTFactory::getAllDecorators()
{
	std::string decorators = "";

	for (auto it = _currentDecorators.begin(); it != _currentDecorators.end(); it++) {
		decorators += it->first + ",";
	}

	if (!decorators.empty())
		decorators.pop_back();

	return decorators;
}

bool BTFactory::init() {
	return true;
}

bool BTFactory::TaskExists(std::string ID)
{
	auto it = _currentTasks.find(ID);

	return it != _currentTasks.end();
}

bool BTFactory::DecoratorExists(std::string ID)
{
	auto it = _currentDecorators.find(ID);

	return it != _currentDecorators.end();
}
