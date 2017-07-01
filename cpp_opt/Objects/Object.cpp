#include "Object.h"



Object::Object()
	: _position {}
	, _rotation {}
	, _scale {1, 1, 1}
{
	_totalTime = 0.0;
}


Object::~Object()
{
}

void Object::Update(double deltaTime)
{
	if (_posInit.X == 0)
		_posInit = _position;

	const double SPEED = 0.25;
	_rotation.Y += SPEED * deltaTime;
	//_rotation.X += SPEED * deltaTime;
	//_rotation.Y += SPEED * deltaTime;

	//_position.X = _posInit.X + ( sin(_totalTime / 1000.0) * 2000.0);

	_totalTime += deltaTime;
}
