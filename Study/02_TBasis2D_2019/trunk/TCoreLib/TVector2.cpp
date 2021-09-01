#include "TVector2.h"

Vector2::Vector2()
{
	x = y = 0.0f;
}

Vector2::Vector2(const Vector2& v0)
{
	x = v0.x;
	y = v0.y;
}

Vector2::Vector2(float fX, float fY)
{
	x = fX;
	y = fY;
}
Vector2::Vector2(TPoint p)
{
	x = p.x;
	y = p.y;
}
/*////////////////////////////////////////////////////////////////////////////////////////////////////////
//   ������ ������ �� ��Ģ
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//������ ������ �ÿ� ���׿����� ��ġ�� ��ġ ������ ���
a.operator++() (->++a) / a.operator++(int) (->a++)
//������ ������ �ÿ� ���׿����ڸ� ��ġ�� �����(++a),
���۷����� ��ü�� �����(call by reference), *this�� �����Ѵ�.
//������ ������ �ÿ� ���׿����ڸ� ��ġ�� �����(a++), call by value�̵� const�� �����ؾ��Ѵ�.
�������� �����ϰ� �ڱ��ڽ� ���� ������Ų��.
//�������� ����Ű�� ���ο� ī���� ������ �ذ��ϱ� ���Ͽ�
���۷����� ���� �Լ��� ������ �ؾ��Ѵ�.
//�������ڸ� ������ٸ�
Int32 & operator=(const Int32 &r)�� �Բ� �������Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////////*/
// ��ġ ������ ������ 
Vector2  Vector2::operator-()
{
	Vector2 temp = *this;
	temp = temp * -1.0f;
	return temp;
}

Vector2 & Vector2::operator++()
{
	*this = *this + *this;
	return *this;
}
// ��ġ ������ ������ 
const Vector2  Vector2::operator++(int)
{
	Vector2 temp = *this;
	++(*this);
	return temp;
}

Vector2 Vector2::operator + (Vector2 const &v0)
{
	return Vector2(v0.x + x, v0.y + y);
}

Vector2 Vector2::operator - (Vector2 const &v0)
{
	return Vector2(x - v0.x, y - v0.y);
}

Vector2 Vector2::operator + (float const &fScala)
{
	return Vector2(fScala + x, fScala + y);
}
Vector2 Vector2::operator * (float const &fScala)
{
	return Vector2(fScala*x, fScala*y);
}
Vector2 Vector2::operator / (float const &fScala)
{
	float fInvert = 1.0f / fScala;
	return Vector2(x*fInvert, y*fInvert);
}

// Dot Product
float Vector2::operator | (Vector2 const &v0)
{
	return x * v0.x + y * v0.y;
}

bool	Vector2::operator == (Vector2 const &v0)
{
	if (fabs(x - v0.x) < TBASIS_EPSILON)
	{
		if (fabs(y - v0.y) < TBASIS_EPSILON)
		{
			return true;
		}
	}
	return false;
}

// ����
float Vector2::LengthSquared()
{
	return (x*x + y * y);
}

// �������� ������ �Ÿ�
float Vector2::Length()
{
	return (float)sqrt(LengthSquared());
}

Vector2 Vector2::Normal()
{	
	float invertLength = 1.0f / (Length()+0.0001f);
	return Vector2(x*invertLength, y*invertLength);
}

float Vector2::Angle(Vector2& v0)
{
	Vector2 vA = Normal();
	Vector2 vB = v0.Normal();
	float fDot = (vA | vB);
	if (fDot > 1.0f) fDot = 1.0f;
	float fRadian = acos(fDot);
	float fAngle = (float)RadianToDegree(fRadian);
	if (fDot < 0.0f)
	{
		fAngle = 180.0f + (180.0f - fAngle);
	}
	else
	{
		fAngle = fAngle;
	}
	return fAngle;
}
// Cross Product
Vector2 Vector2::operator ^ (Vector2 const &v0)
{
	if (*this == v0)
	{
		return v0;
	}
	return Vector2(0, (x*v0.y - y * v0.x));
}

bool   Vector2::CCW(Vector2 vLook, Vector2 vDir)
{
	//Vector2 pTarget, pPos;
	//// Y���� �������� �Ʒ��� �����ϵ��� Ŭ���̾�Ʈ ��ǥ�� ���� ��ǥ�� ��ȯ�Ѵ�.
	//pTarget.y *= -1.0f;
	//pPos.y *= -1.0f;
	//// ���� ���� �������� ���Ϳ� ������ ������ ���� ����Ѵ�.
	//Vector2 vDir;
	//vDir.x = pTarget.x - pPos.x;
	//vDir.y = pTarget.y - pPos.y;
	//vDir = vDir.Normal();
	//float fAngle = vDir.Angle(vLook);

	// �ð� �ݴ�������� �����̸� ������ ����.
	Vector2 vA, vB, vC, vD;
	vA.x = vLook.x;	vA.y = vLook.y;
	vB.x = vDir.x;	vB.y = vDir.y;
	vC = (vA ^ vB).Normal();
	float fDot = vC | Vector2(0, -1);
	if (fDot > 0.0f)
	{
		return false;
	}
	return true;
}
/*Vector3 vA, vB, vC, vD;
vA.x = m_pOwner->m_vLook.x;	vA.y = m_pOwner->m_vLook.y;	vA.z = 0.0f;
vB.x = vDir.x;	vB.y = vDir.y;	vB.z = 0.0f;
vC = (vA ^ vB).Normal();
float fDot = vC | Vector3(0,0,-1);*/