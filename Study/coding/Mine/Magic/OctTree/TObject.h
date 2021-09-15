#include "TVector.h"
#include "TRect.h"
class TObject
{
public:
	int      iNodeIndex;
	TVector pos;
	float   mass;
	TVector Velocity; //속도
	TVector Acceleration; //가속도
	TVector sumForces; //모든 힘 합산

	Rect    rect;
public:
	TObject();
};

