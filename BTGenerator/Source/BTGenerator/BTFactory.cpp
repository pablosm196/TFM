#include "BTFactory.h"

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

UBTTask_BlackboardBase* BTFactory::GetTask(std::string ID) {
	auto it = _currentTasks.find(ID);

	if (it == _currentTasks.end()) return nullptr;

	return it->second();
}

UBTDecorator* BTFactory::GetDecorator(std::string ID)
{
	auto it = _currentDecorators.find(ID);

	if (it == _currentDecorators.end()) return nullptr;

	return it->second();
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
