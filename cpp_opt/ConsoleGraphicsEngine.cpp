#include "ConsoleGraphicsEngine.h"

#include "Objects\Object.h"



ConsoleGraphicsEngine::ConsoleGraphicsEngine()
{
    _console = GetStdHandle(STD_OUTPUT_HANDLE);
    _refreshTimer = 0.0;
}


ConsoleGraphicsEngine::~ConsoleGraphicsEngine()
{
}

void ConsoleGraphicsEngine::Draw(const DrawContext& context)
{
    _refreshTimer += context.DeltaTime;
    DrawObjects(context);
    Clear();
}

void ConsoleGraphicsEngine::Clear()
{
    if (_refreshTimer >= 5000.0)
    {
        TotalClear();
        _refreshTimer = 0.0;
        return;
    }

#if DIAG
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
#endif // DIAG

    for (auto it = _pixelsToClear.begin(); it != _pixelsToClear.end(); ++it)
    {
        DWORD written;
        WriteConsoleOutputCharacterA(_console, " ", 1, *it, &written);
    }

#if DIAG
    QueryPerformanceCounter(&end);
    double totalTime = static_cast<double>(end.QuadPart - start.QuadPart) / freq.QuadPart;
    printf("Clear:\n\tTotal time: %fs\n\n", totalTime);
#endif // DIAG

    

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

#if DIAG
    LARGE_INTEGER freq, startTimePoint, linesTimePoint, rotTimePoint, endTimePoint;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&startTimePoint);
#endif //DIAG


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

#if DIAG
    QueryPerformanceCounter(&linesTimePoint);
#endif //DIAG

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

#if DIAG
    QueryPerformanceCounter(&rotTimePoint);
#endif //DIAG

    for (size_t i = 0; i < PIXELSCOUNT; i++)
    {
        Vector3 rotated = rotZ * (rotY * (rotX * PIXELS[i]));
        COORD coord = ProjectToConsoleCoord(object->GetPosition() + rotated);
        _pixels.emplace(coord);
        auto pos = _pixelsToClear.find(coord);
        if (pos != _pixelsToClear.end())
        {
            _pixelsToClear.erase(coord);
        }
        DWORD written;
        WriteConsoleOutputCharacterA(_console, "X", 1, coord, &written);
    }

#if DIAG
    QueryPerformanceCounter(&endTimePoint);

    double linesTime = static_cast<double>(linesTimePoint.QuadPart - startTimePoint.QuadPart) / freq.QuadPart;
    double rotStaticTime = static_cast<double>(rotTimePoint.QuadPart - linesTimePoint.QuadPart) / freq.QuadPart;
    double drawTime = static_cast<double>(endTimePoint.QuadPart - rotTimePoint.QuadPart) / freq.QuadPart;
    double totalTime = static_cast<double>(endTimePoint.QuadPart - startTimePoint.QuadPart) / freq.QuadPart;
    
    printf("Draw:\n\tLines time: %fs\n\tStatic Rot time: %fs\n\tDraw time: %fs\n\tTotal time: %fs\n\n", linesTime, rotStaticTime, drawTime, totalTime);
#endif //DIAG

}

COORD ConsoleGraphicsEngine::ProjectToConsoleCoord(const Vector3& position)
{
    const int Scale = 100;
    return{ (SHORT) position.X / Scale, (SHORT) position.Y / Scale };
}