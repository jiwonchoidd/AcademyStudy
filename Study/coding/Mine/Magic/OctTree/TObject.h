#include "TVector.h"
#include "TRect.h"
class TObject
{
public:
	int      iNodeIndex;
	TVector pos;
	float   mass;
	TVector Velocity; //�ӵ�
	TVector Acceleration; //���ӵ�
	TVector sumForces; //��� �� �ջ�

	Rect    rect;
public:
	TObject();
};

