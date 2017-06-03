#pragma once

#include "Objects/Object.h"
#include "IGraphicsEngine.h"
#include "IInputEngine.h"

class EngineCore
{
public:
	EngineCore();
	~EngineCore();

	void Start(IGraphicsEngine* graphicsEngine, IInputEngine* inputEngine);
	void Tick(double deltaTime);
	void AddObject(Object* inObject);
	void UpdateObjects(double deltaTime);

private:

	IGraphicsEngine* _graphicsEngine;
	IInputEngine* _inputEngine;

	ObjectsCollection _objects;

};

