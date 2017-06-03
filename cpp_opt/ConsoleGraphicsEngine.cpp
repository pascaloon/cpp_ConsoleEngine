#include "ConsoleGraphicsEngine.h"

#include "Objects\Object.h"


ConsoleGraphicsEngine::ConsoleGraphicsEngine()
{
	_console = GetStdHandle(STD_OUTPUT_HANDLE);
	_usedCoords = new std::vector<COORD>;
	_lastUsedCoords = nullptr;
}


ConsoleGraphicsEngine::~ConsoleGraphicsEngine()
{
}

void ConsoleGraphicsEngine::Draw(const DrawContext& context)
{
	DrawObjects(context);
	Clear();
}

void ConsoleGraphicsEngine::Clear()
{
	//COORD topLeft = { 0, 0 };

	/*CONSOLE_SCREEN_BUFFER_INFO screen;
	GetConsoleScreenBufferInfo(_console, &screen);

	DWORD written;
	FillConsoleOutputCharacterA(_console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);

	FillConsoleOutputAttribute(_console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, 
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
	SetConsoleCursorPosition(_console, topLeft);*/

	if (_lastUsedCoords == nullptr)
	{
		_lastUsedCoords = _usedCoords;
		_usedCoords = new std::vector<COORD>;
		return;
	}

	for (size_t i = 0; i < _lastUsedCoords->size(); i++)
	{
		bool found = false;
		for (size_t j = 0; j < _usedCoords->size(); j++)
		{
			const COORD& A = _lastUsedCoords->at(i);
			const COORD& B = _usedCoords->at(j);
			if (A.X == B.X && A.Y == B.Y)
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			DWORD written;
			WriteConsoleOutputCharacterA(_console, " ", 1, _lastUsedCoords->at(i), &written);
		}

	}

	delete _lastUsedCoords;
	_lastUsedCoords = _usedCoords;
	_usedCoords = new std::vector<COORD>;
}

void ConsoleGraphicsEngine::DrawObjects(const DrawContext& context)
{
	for (size_t i = 0; i < context.Objects.size(); i++)
	{
		DrawObject(context.DeltaTime, context.Objects[i]);
	}
}

void ConsoleGraphicsEngine::DrawObject(double deltaTime, Object* object)
{
	const static Vector3 POINTS[]{ 		
		Vector3{ -600,  -600, -600  },
		Vector3{  600,  -600, -600  },
		Vector3{ -600,   600, -600  },
		Vector3{  600,   600, -600  },

		Vector3{ -600,  -600,  600  },
		Vector3{  600,  -600,  600  },
		Vector3{ -600,   600,  600  },
		Vector3{  600,   600,  600  }
	};


	const static size_t LINESCOUNT = 12;
	const static Line LINES[LINESCOUNT]{
		Line{ POINTS[0], POINTS[1] },
		Line{ POINTS[0], POINTS[2] },
		Line{ POINTS[3], POINTS[1] },
		Line{ POINTS[3], POINTS[2] },

		Line{ POINTS[4], POINTS[5] },
		Line{ POINTS[4], POINTS[6] },
		Line{ POINTS[7], POINTS[5] },
		Line{ POINTS[7], POINTS[6] },

		Line{ POINTS[0], POINTS[4] },
		Line{ POINTS[1], POINTS[5] },
		Line{ POINTS[2], POINTS[6] },
		Line{ POINTS[3], POINTS[7] }
	};

	const static size_t LINESPIXELS = 13;
	const static size_t PIXELSCOUNT = LINESCOUNT * LINESPIXELS;
	static Vector3 PIXELS[PIXELSCOUNT];
	for (size_t i = 0; i < LINESCOUNT; i++)
	{
		for (size_t j = 0; j <= LINESPIXELS; j++)
		{
			Vector3 dv = (LINES[i].B - LINES[i].A) * ((double)j / LINESPIXELS);
			PIXELS[(i * LINESPIXELS) + j] = LINES[i].A + dv;
		}
	}

	const Vector3& rot = object->GetRotation();
	double cosRotX = cos(FromDegrees(rot.X));
	double sinRotX = sin(FromDegrees(rot.X));
	double cosRotY = cos(FromDegrees(rot.Y));
	double sinRotY = sin(FromDegrees(rot.Y));
	double cosRotZ = cos(FromDegrees(rot.Z));
	double sinRotZ = sin(FromDegrees(rot.Z));
	Matrix3 rotX{
		Vector3{ 1,               0,	    0 },
		Vector3{ 0,         cosRotX, -sinRotX },
		Vector3{ 0,         sinRotX,  cosRotX }};
	Matrix3 rotY{
		Vector3{  cosRotY,	      0,  sinRotY },
		Vector3{        0,        1,        0 },
		Vector3{ -sinRotY,        0,  cosRotY }};
	Matrix3 rotZ{
		Vector3{ cosRotZ,  -sinRotZ,        0 },
		Vector3{ sinRotZ,   cosRotZ,        0 },
		Vector3{       0,         0,        1 }};

	//for (size_t i = 0; i < 9; i++)
	//{
	//	Vector3 rotated = rotZ * (rotY * (rotX * POINTS[i]));
	//	COORD coord = ProjectToConsoleCoord(object->GetPosition() + rotated);
	//	_usedCoords->push_back(coord);
	//	DWORD written;
	//	WriteConsoleOutputCharacterA(_console, "X", 1, coord, &written);
	//}

	for (size_t i = 0; i < PIXELSCOUNT; i++)
	{
		Vector3 rotated = rotZ * (rotY * (rotX * PIXELS[i]));
		COORD coord = ProjectToConsoleCoord(object->GetPosition() + rotated);
		_usedCoords->push_back(coord);
		DWORD written;
		WriteConsoleOutputCharacterA(_console, "X", 1, coord, &written);
	}

	
}

COORD ConsoleGraphicsEngine::ProjectToConsoleCoord(const Vector3& position)
{
	const int Scale = 100;
	return{ (SHORT) position.X / Scale, (SHORT) position.Y / Scale };
}