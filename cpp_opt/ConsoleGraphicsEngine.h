#pragma once

#include "IGraphicsEngine.h"
#include <Windows.h>
#include <unordered_map>
#include <unordered_set>

struct Line
{
	Line(const Vector3& a, const Vector3& b): A(a), B(b) {	}

	const Vector3& A;
	const Vector3& B;
};



struct COORDKeyHasher
{
	std::size_t operator()(const COORD& k) const
	{
		return (k.Y * 1000000) + k.X;
	}
};
inline bool operator==(const COORD& lhs, const COORD& rhs)
{
	return lhs.X == rhs.X
		&& lhs.Y == rhs.Y;
}


class ConsoleGraphicsEngine : public IGraphicsEngine
{
public:
	ConsoleGraphicsEngine();
	~ConsoleGraphicsEngine();
	
	void Draw(const DrawContext& context) override;
	Object* GetCamera() override { return _camera; };
	void Clear() override;
	void TotalClear() const;

private:
	void DrawObjects(const DrawContext& context);
	void DrawObject(double deltaTime, Object* object);
	COORD ProjectToConsoleCoord(const Vector3& position);
private:
	HANDLE _console;

	std::unordered_set<COORD, COORDKeyHasher> _pixels;
	std::unordered_set<COORD, COORDKeyHasher> _pixelsToClear;

	double _refreshTimer;
	Object* _camera;
};

