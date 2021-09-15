#include "TObject.h"
TObject::TObject()
{
	pos = {10,10,10};
	mass = 1.0f;
	Acceleration.x = 1.0f;
	Acceleration.y = 1.0f;
	Acceleration.z = 1.0f;
}