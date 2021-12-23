#include "KVector.h"
#include "KRect.h"
class KObject
{
public:
	int      iNodeIndex;
	KVector pos;
	float   mass;
	KVector Velocity; //속도
	KVector Acceleration; //가속도
	KVector sumForces; //모든 힘 합산

	Rect    rect;
public:
	KObject();
};

