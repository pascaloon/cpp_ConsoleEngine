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

		const double MOVEMENT_SPEED = 250.0;
		const double ROTATION_SPEED = 5.0;

		Vector3& cameraPosition = context.Camera->GetPosition();
		Vector3& cameraRotation = context.Camera->GetRotation();

		switch (eventsBuffer[i].Event.KeyEvent.uChar.AsciiChar)
		{
			// Moving Camera
		case 'w':
			cameraPosition.Y -= MOVEMENT_SPEED;
			break;
		case 's':
			cameraPosition.Y += MOVEMENT_SPEED;
			break;
		case 'a':
			cameraPosition.X -= MOVEMENT_SPEED;
			break;
		case 'd':
			cameraPosition.X += MOVEMENT_SPEED;
			break;
		case 'q':
			cameraPosition.Z += MOVEMENT_SPEED;
			break;
		case 'z':
			cameraPosition.Z -= MOVEMENT_SPEED;
			break;

			// Rotating Camera
		case 'o':
			cameraRotation.Y += ROTATION_SPEED;
			break;
		case 'p':
			cameraRotation.Y -= ROTATION_SPEED;
			break;
		case 'i':
			cameraRotation.X += ROTATION_SPEED;
			break;
		case 'k':
			cameraRotation.X -= ROTATION_SPEED;
			break;

			// Other
		default:
			break;
		}
	}

	

}
