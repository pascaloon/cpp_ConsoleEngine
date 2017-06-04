#include "ConsoleInputEngine.h"



ConsoleInputEngine::ConsoleInputEngine()
{
	_console = GetStdHandle(STD_INPUT_HANDLE);
}


ConsoleInputEngine::~ConsoleInputEngine()
{
}

void ConsoleInputEngine::ReadKeys(ObjectsCollection& Objects)
{
	//LPCSTR readCharacter = nullptr;
	//DWORD readCharactersCount = 0;
	//ReadConsoleA(_console, (LPVOID)readCharacter, 1, &readCharactersCount, NULL);

	//if (readCharactersCount != 0)
	//{
	//	return;
	//}

	//if (readCharactersCount == 1 && *readCharacter == 'w')
	//	Objects[0]->GetPosition().Y -= 10;
	//if (readCharactersCount == 1 && *readCharacter == 's')
	//	Objects[0]->GetPosition().Y += 10;


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
			Objects[0]->GetPosition().Y -= SPEED;
			break;
		case 's':
			Objects[0]->GetPosition().Y += SPEED;
			break;
		case 'a':
			Objects[0]->GetPosition().X -= SPEED;
			break;
		case 'd':
			Objects[0]->GetPosition().X += SPEED;
			break;
		default:
			break;
		}
	}

	

}
