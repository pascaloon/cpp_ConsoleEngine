
#include "EngineCore.h"
#include "ConsoleGraphicsEngine.h"
#include <cstdlib>
#include <time.h>
#include "ConsoleInputEngine.h"

int randNumber()
{
	return rand() % 1000 + 1001;
}

int main()
{
	srand(time(0));

	ConsoleGraphicsEngine ConsoleGraphics;
	ConsoleInputEngine ConsoleInput;
	EngineCore Engine;

	//for (size_t i = 0; i < 3; i++)
	//{
	//	Object* o = new Object;
	//	o->SetPosition(Vector3(randNumber(), randNumber(), 0));
	//	o->SetRotation(Vector3(20.0, 0.0, 0.0));
	//	Engine.AddObject(o);
	//}
	Object* o1 = new Object;
	o1->SetPosition(Vector3(8000, 2000,   0));
	o1->SetRotation(Vector3(  15,   30,   0));
	Engine.AddObject(o1);

	Object* o2 = new Object;
	o2->SetPosition(Vector3(12000, 2000, 0));
	o2->SetRotation(Vector3(15, 30, 0));
	Engine.AddObject(o2);

	Object* o3 = new Object;
	o3->SetPosition(Vector3(16000, 2000, 0));
	o3->SetRotation(Vector3(15, 30, 0));
	Engine.AddObject(o3);

	//Object* o2 = new Object;
	//o2->SetPosition(Vector3(6000, 1000, 0));
	//o2->SetRotation(Vector3(15, 30, 0));
	//Engine.AddObject(o2);

	Engine.Start(&ConsoleGraphics, &ConsoleInput);

	
}