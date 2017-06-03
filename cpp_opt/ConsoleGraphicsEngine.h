#pragma once

#include "IGraphicsEngine.h"
#include <Windows.h>

struct Line
{
	Line(const Vector3& a, const Vector3& b): A(a), B(b) {	}

	const Vector3& A;
	const Vector3& B;
};


class ConsoleGraphicsEngine : public IGraphicsEngine
{
public:
	ConsoleGraphicsEngine();
	~ConsoleGraphicsEngine();
	
	void Draw(const DrawContext& context) override;
	void Clear() override;

private:
	void DrawObjects(const DrawContext& context);
	void DrawObject(double deltaTime, Object* object);
	COORD ProjectToConsoleCoord(const Vector3& position);
private:
	HANDLE _console;
	std::vector<COORD>* _usedCoords;
	std::vector<COORD>* _lastUsedCoords;

};

