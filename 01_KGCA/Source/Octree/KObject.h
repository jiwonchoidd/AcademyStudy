#include "KVector3.h"
#include "KRect.h"
class KObject
{
public:
	int      iNodeIndex;
	KVector3 pos;
	float   mass;
	KVector3 Velocity; //�ӵ�
	KVector3 Acceleration; //���ӵ�
	KVector3 sumForces; //��� �� �ջ�
	float	dir;
	Rect    rect;
public:
	KObject();
};

