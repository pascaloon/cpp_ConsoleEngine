#pragma once
#include "Objects\Object.h"

struct InputContext
{
	InputContext(ObjectsCollection& inObjects, Vector3& inCamera)
		: Objects(inObjects)
		, Camera(inCamera)
	{ }
	ObjectsCollection& Objects;
	Vector3& Camera;
};

class IInputEngine
{
public:

	virtual ~IInputEngine() {}

	virtual void ReadKeys(InputContext& context) = 0;
};

