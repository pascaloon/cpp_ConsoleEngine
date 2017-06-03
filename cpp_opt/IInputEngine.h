#pragma once
#include "Objects\Object.h"

class IInputEngine
{
public:

	virtual ~IInputEngine() {}

	virtual void ReadKeys(ObjectsCollection& Objects) = 0;
};

