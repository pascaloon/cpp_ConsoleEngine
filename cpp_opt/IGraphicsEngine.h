#pragma once

#include <vector>
#include "Objects/Object.h"

//class ObjectsCollection;

struct DrawContext
{
	DrawContext(double DeltaTime, const ObjectsCollection& Objects)
		: DeltaTime(DeltaTime)
		, Objects(Objects)
	{ }
	double DeltaTime;
	const ObjectsCollection& Objects;
};

class IGraphicsEngine
{

public:
	virtual ~IGraphicsEngine() {};

	virtual void Draw(const DrawContext& context) = 0;
	virtual void Clear() = 0;

	virtual Object* GetCamera() = 0;
};

