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

		Vector3& cameraPosition = context.Camera->GetPosition();
		Vector3& cameraRotation = context.Camera->GetRotation();

		switch (eventsBuffer[i].Event.KeyEvent.uChar.AsciiChar)
		{
			// Moving Camera
		case 'w':
			cameraPosition.Y -= SPEED;
			break;
		case 's':
			cameraPosition.Y += SPEED;
			break;
		case 'a':
			cameraPosition.X -= SPEED;
			break;
		case 'd':
			cameraPosition.X += SPEED;
			break;
		case 'q':
			cameraPosition.Z += SPEED;
			break;
		case 'z':
			cameraPosition.Z -= SPEED;
			break;

			// Rotating Camera
		case 'o':
			cameraRotation.Y += 1;
			break;
		case 'p':
			cameraRotation.Y -= 1;
			break;
		case 'i':
			cameraRotation.X += 1;
			break;
		case 'k':
			cameraRotation.X -= 1;
			break;

			// Other
		default:
			break;
		}
	}

	

}
