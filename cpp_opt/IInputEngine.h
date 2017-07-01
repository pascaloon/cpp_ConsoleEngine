#pragma once
#include "Objects\Object.h"

struct InputContext
{
	InputContext(ObjectsCollection& inObjects, Object* inCamera)
		: Objects(inObjects)
		, Camera(inCamera)
	{ }
	ObjectsCollection& Objects;
	Object* Camera;
};

class IInputEngine
{
public:

	virtual ~IInputEngine() {}

	virtual void ReadKeys(InputContext& context) = 0;
};

