#include "ConsoleInputEngine.h"



ConsoleInputEngine::ConsoleInputEngine()
{
	_console = GetStdHandle(STD_INPUT_HANDLE);
}


ConsoleInputEngine::~ConsoleInputEngine()
{
}

void ConsoleInputEngine::ReadKeys(InputContext& context)
{
	

	DWORD eventsCountRead = 0;
	GetNumberOfConsoleInputEvents(_console, &eventsCountRead);

	if (eventsCountRead <= 0)
		return;

	DWORD inputsCountRead = 0;
	INPUT_RECORD* eventsBuffer = new INPUT_RECORD[eventsCountRead];
	ReadConsoleInputA(_console, eventsBuffer, eventsCountRead, &inputsCountRead);

	for (size_t i = 0; i < inputsCountRead; i++)
	{
		if (eventsBuffer[i].EventType != KEY_EVENT)
			continue;

		if (!eventsBuffer[i].Event.KeyEvent.bKeyDown)
			continue; // Nope

		const double SPEED = 250.0;

		switch (eventsBuffer[i].Event.KeyEvent.uChar.AsciiChar)
		{
		case 'w':
			context.Camera.Y -= SPEED;
			break;
		case 's':
			context.Camera.Y += SPEED;
			break;
		case 'a':
			context.Camera.X -= SPEED;
			break;
		case 'd':
			context.Camera.X += SPEED;
			break;
		case 'q':
			context.Camera.Z += SPEED;
			break;
		case 'z':
			context.Camera.Z -= SPEED;
			break;
		default:
			break;
		}
	}

	

}
