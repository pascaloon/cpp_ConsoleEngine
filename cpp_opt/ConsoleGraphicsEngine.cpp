#include "ConsoleGraphicsEngine.h"

#include "Objects\Object.h"



ConsoleGraphicsEngine::ConsoleGraphicsEngine()
{
	_console = GetStdHandle(STD_OUTPUT_HANDLE);
	_refreshTimer = 0.0;

	_camera = new Object;
	_camera->SetPosition(Vector3{ 0000, 0000, -8000 });

}


ConsoleGraphicsEngine::~ConsoleGraphicsEngine()
{
}

void ConsoleGraphicsEngine::Draw(const DrawContext& context)
{

#if DIAG
	LARGE_INTEGER freq, start, draw, clear, end;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);
#endif // DIAG

	_refreshTimer += context.DeltaTime;
	DrawObjects(context);

#if DIAG
	QueryPerformanceCounter(&draw);
#endif // DIAG

	Clear();

#if DIAG
	QueryPerformanceCounter(&clear);
#endif // DIAG

#if DIAG
	QueryPerformanceCounter(&end);
	double drawTime = (static_cast<double>(draw.QuadPart - start.QuadPart) / freq.QuadPart);
	double clearTime = (static_cast<double>(clear.QuadPart - draw.QuadPart) / freq.QuadPart);
	double fps = 1.0 / (static_cast<double>(end.QuadPart - start.QuadPart) / freq.QuadPart);
	printf("General:\n\tFPS: %fs\n\tDraw time: %fs\n\tClear time: %fs\n\n", fps, drawTime, clearTime);

	Vector3& cameraPos = _camera->GetPosition();
	Vector3& cameraRot = _camera->GetRotation();
	printf("Camera:\n\tPosition: (%f, %f, %f)\n\tRotation: (%f, %f, %f)\n\n", 
		cameraPos.X, cameraPos.Y, cameraPos.Z,
		cameraRot.X, cameraRot.Y, cameraRot.Z);
#endif // DIAG

}

void ConsoleGraphicsEngine::Clear()
{
	if (_refreshTimer >= 5000.0)
	{
		TotalClear();
		_refreshTimer = 0.0;
		return;
	}

#if DIAGDETAILS
	LARGE_INTEGER freq, start, end;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);
#endif // DIAGDETAILS

	for (auto it = _pixelsToClear.begin(); it != _pixelsToClear.end(); ++it)
	{
		DWORD written;
		WriteConsoleOutputCharacterA(_console, " ", 1, *it, &written);
	}

#if DIAGDETAILS
	QueryPerformanceCounter(&end);
	double totalTime = static_cast<double>(end.QuadPart - start.QuadPart) / freq.QuadPart;
	printf("Clear:\n\tTotal time: %fs\n\n", totalTime);
#endif // DIAGDETAILS

	

	SetConsoleCursorPosition(_console, { 0, 0 });

}

void ConsoleGraphicsEngine::TotalClear() const
{
	COORD topLeft = { 0, 0 };

	CONSOLE_SCREEN_BUFFER_INFO screen;
	GetConsoleScreenBufferInfo(_console, &screen);

	DWORD written;
	FillConsoleOutputCharacterA(_console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);

	FillConsoleOutputAttribute(_console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
	screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
	SetConsoleCursorPosition(_console, topLeft);
}

void ConsoleGraphicsEngine::DrawObjects(const DrawContext& context)
{
	_pixelsToClear = _pixels;
	_pixels.clear();
	for (size_t i = 0; i < context.Objects.size(); i++)
	{
		DrawObject(context.DeltaTime, context.Objects[i]);
	}
}

void ConsoleGraphicsEngine::DrawObject(double deltaTime, Object* object)
{

#if DIAGDETAILS
	LARGE_INTEGER freq, startTimePoint, linesTimePoint, rotTimePoint, endTimePoint;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&startTimePoint);
#endif //DIAGDETAILS


	const Vector3 PositionRelativeToCamera = object->GetPosition() - _camera->GetPosition();


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

#if DIAGDETAILS
	QueryPerformanceCounter(&linesTimePoint);
#endif //DIAGDETAILS
		
	// P x TRSc x TRSv x V
	
	// Sv
	Matrix4 scaleV = Matrix4::CreateScaleMatrix(object->GetScale());

	// Rv
	const Vector3& rotv = object->GetRotation();
	Matrix4 rotVX = Matrix4::CreateRotationMatrixX(rotv.X);
	Matrix4 rotVY = Matrix4::CreateRotationMatrixY(rotv.Y);
	Matrix4 rotVZ = Matrix4::CreateRotationMatrixZ(rotv.Z);

	// Tv
	Matrix4 transV = Matrix4::CreateTranslationMatrix(object->GetPosition());
	//Matrix4 transV = Matrix4::CreateTranslationMatrix(PositionRelativeToCamera);


	// Sc
	Matrix4 scaleC = Matrix4::CreateScaleMatrix(_camera->GetScale());

	// Rc
	const Vector3& rotc = _camera->GetRotation();
	Matrix4 rotCX = Matrix4::CreateRotationMatrixX(rotc.X);
	Matrix4 rotCY = Matrix4::CreateRotationMatrixY(rotc.Y);
	Matrix4 rotCZ = Matrix4::CreateRotationMatrixZ(rotc.Z);

	// Tc
	Matrix4 transC = Matrix4::CreateTranslationMatrix(_camera->GetPosition());


	

	

#if DIAGDETAILS
	QueryPerformanceCounter(&rotTimePoint);
#endif //DIAGDETAILS

	const double ez = 1600;
	CONSOLE_SCREEN_BUFFER_INFO screen;
	GetConsoleScreenBufferInfo(_console, &screen);
	double width = screen.dwSize.X * 100.0;
	double height = screen.dwSize.Y * 100.0;


	//const Matrix4 viewMatrix = (-1.0 * transC) * (rotCZ * (rotCY * (rotCX)));
	const Vector4 viewDirection = rotCZ * (rotCY * (rotCX * Vector4{ 0, 0, 1, 1 }));
	const Matrix4 viewMatrix = Matrix4::LookAtMatrix(
		_camera->GetPosition(), 
		_camera->GetPosition() + viewDirection.XYZ(),
		Vector3{ 0, 1, 0 }
	);

	const double FOV = 90.0;
	const double aspect = 1;

	const double nearp = 100.0;
	const double farp = 1000.0;
	const double top = nearp * (FromDegrees(FOV) / 2);
	const double bottom = -top;
	const double right = top * aspect;
	const double left = -right;

	const Matrix4 perspectiveMatrix = Matrix4::PerspectiveMatrix(
		nearp, farp,
		top, bottom,
		left, right
	);

	for (size_t i = 0; i < PIXELSCOUNT; i++)
	{

		const Vector4 vertexPosition = Vector4{ PIXELS[i], 1 };

		const Matrix4 modelMatrix = transV * (rotVZ * (rotVY * (rotVX * (scaleV))));
		

		

		const Vector4 transformedPosition = perspectiveMatrix * (viewMatrix * (modelMatrix * vertexPosition));

		const double ratioZ = (ez / transformedPosition.Z) / 0.1;
		Vector3 projectedPosition{
			ratioZ * transformedPosition.X,
			ratioZ * transformedPosition.Y,
			0.0 };
		projectedPosition.X += width / 2;
		projectedPosition.Y += height / 8;
		COORD coord = ProjectToConsoleCoord(projectedPosition);


		_pixels.emplace(coord);
		auto pos = _pixelsToClear.find(coord);
		if (pos != _pixelsToClear.end())
		{
			_pixelsToClear.erase(coord);
		}
		DWORD written;
		WriteConsoleOutputCharacterA(_console, "X", 1, coord, &written);
	}

#if DIAGDETAILS
	QueryPerformanceCounter(&endTimePoint);

	double linesTime = static_cast<double>(linesTimePoint.QuadPart - startTimePoint.QuadPart) / freq.QuadPart;
	double rotStaticTime = static_cast<double>(rotTimePoint.QuadPart - linesTimePoint.QuadPart) / freq.QuadPart;
	double drawTime = static_cast<double>(endTimePoint.QuadPart - rotTimePoint.QuadPart) / freq.QuadPart;
	double totalTime = static_cast<double>(endTimePoint.QuadPart - startTimePoint.QuadPart) / freq.QuadPart;
	
	printf("Draw:\n\tLines time: %fs\n\tStatic Rot time: %fs\n\tDraw time: %fs\n\tTotal time: %fs\n\n", linesTime, rotStaticTime, drawTime, totalTime);
#endif //DIAGDETAILS

}

COORD ConsoleGraphicsEngine::ProjectToConsoleCoord(const Vector3& position)
{
	static const double Scale = 100.0;
	return{ (SHORT) (position.X / Scale), (SHORT) (position.Y / Scale) };
}