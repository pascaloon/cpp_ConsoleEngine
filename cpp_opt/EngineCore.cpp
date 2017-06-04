#include "EngineCore.h"

#include <chrono>
#include <ctime>

EngineCore::EngineCore()
{
}


EngineCore::~EngineCore()
{
}

void EngineCore::Start(IGraphicsEngine* graphicsEngine, IInputEngine* inputEngine)
{
	// Set middleware dependencies
	_graphicsEngine = graphicsEngine;
	_inputEngine = inputEngine;


	// Engine Looping
	auto lastLoopTime = std::chrono::high_resolution_clock::now();
	while (true)
	{
		auto now = std::chrono::high_resolution_clock::now();
		Tick(std::chrono::duration<double, std::milli>(now - lastLoopTime).count());
		lastLoopTime = now;
	}
}

void EngineCore::Tick(double deltaTime)
{
	// Read Input
	_inputEngine->ReadKeys(InputContext(_objects, _graphicsEngine->GetCamera()));

	// Update Objects
	UpdateObjects(deltaTime);

	// Draw Scene
	_graphicsEngine->Draw(DrawContext(deltaTime, _objects));
}

void EngineCore::AddObject(Object* inObject)
{
	_objects.push_back(inObject);
}

void EngineCore::UpdateObjects(double deltaTime)
{
	for (size_t i = 0; i < _objects.size(); i++)
	{
		_objects[i]->Update(deltaTime);
	}
}
