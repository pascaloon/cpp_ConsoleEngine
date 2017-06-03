#pragma once

#include "Objects/Object.h"
#include "IGraphicsEngine.h"

class EngineCore
{
public:
	EngineCore();
	~EngineCore();

	void Start(IGraphicsEngine* graphicsEngine);
	void Tick(double deltaTime);
	void AddObject(Object* inObject);
	void UpdateObjects(double deltaTime);

private:

	IGraphicsEngine* _graphicsEngine;

	ObjectsCollection _objects;

};

