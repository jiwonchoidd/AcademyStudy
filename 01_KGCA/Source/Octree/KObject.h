#include "KVector.h"
#include "KRect.h"
class KObject
{
public:
	int      iNodeIndex;
	KVector pos;
	float   mass;
	KVector Velocity; //�ӵ�
	KVector Acceleration; //���ӵ�
	KVector sumForces; //��� �� �ջ�

	Rect    rect;
public:
	KObject();
};

