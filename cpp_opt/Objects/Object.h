#pragma once

#include <vector>
#include "../Primitives.h"


class Object
{
public:
	Object();
	~Object();

	void Update(double deltaTime);

	const Vector3& GetPosition() const { return _position; }
	Vector3& GetPosition() { return _position; }
	void SetPosition(const Vector3& inPosition) { _position = inPosition; }

	const Vector3& GetRotation() const { return _rotation; }
	Vector3& GetRotation() { return _rotation; }
	void SetRotation(const Vector3& inRotation) { _rotation = inRotation; }

	const Vector3& GetScale() const { return _scale; }
	Vector3& GetScale() { return _scale; }
	void SetScale(const Vector3& inScale) { _scale = inScale; }

private:
	Vector3 _position;
	Vector3 _rotation;
	Vector3 _scale;

	Vector3 _posInit;
	double _totalTime;

};

typedef std::vector<Object*> ObjectsCollection;
