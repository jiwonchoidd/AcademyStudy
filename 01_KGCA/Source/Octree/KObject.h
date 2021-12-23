#include "KVector3.h"
#include "KRect.h"
class KObject
{
public:
	int      iNodeIndex;
	KVector3 pos;
	float   mass;
	KVector3 Velocity; //속도
	KVector3 Acceleration; //가속도
	KVector3 sumForces; //모든 힘 합산
	float	dir;
	Rect    rect;
public:
	KObject();
};

