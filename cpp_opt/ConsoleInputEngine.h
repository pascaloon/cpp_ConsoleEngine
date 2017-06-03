#pragma once
#include "IInputEngine.h"
#include <Windows.h>


class ConsoleInputEngine : public IInputEngine
{
public:
	ConsoleInputEngine();
	~ConsoleInputEngine();


	void ReadKeys(ObjectsCollection& Objects) override;

private:
	HANDLE _console;

};

