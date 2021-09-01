#pragma once
#include <iostream>
//// const 
//int dataA = 1;
//int dataB = 2;
//int dataC = 3;
//int dataD = 4;
	// ��� ������, ���� ������
	// a ������ �ּҿ� ���� ������ �� �ִ�.
//////////////// �߿� ////////////////////
// ��(�����ʿ� const�پ� ������) <- *(����) -> �ּ�(���ʿ� const�پ� ������)
////////////////////////////////////
//const int* a = &dataA;
//// a ������ �ּҿ� ���� ������ �� �ִ�.
//a = &dataB; // ����
//*a = 9;   // �Ұ���
//const int const* a1 = &dataA;
//a1 = &dataC; // ����
//*a1 = 9;     // �Ұ���
//int const* a2 = &dataA;
//a2 = &dataC; // ����
//*a2 = 9;     // �Ұ���
//int* const const a3 = &dataA;
//a3 = &dataD; // �Ұ���
//*a3 = 9;     // ����
//int* const a4 = &dataA;
//a4 = &dataD; // �Ұ���
//*a4 = 9;     // ����
//int const * const a5 = &dataA;
//a5 = &dataD; // �Ұ���
//*a5 = 9;     // �Ұ���
// void a() const {} //  �Լ����� ��� ���� �� ���� �Ұ���
// const �Լ������� const �Լ��� ȣ�Ⱑ��.


class TRegion
{
public:
	enum { POINT_POINT, POINT_SIZE };	// �������� ����, �������� ����
	int m_iWidth;
	int m_iHeight;
private:
	/////////////////////////////////////////////////////////////
	// �簢�� ������ �����ϱ� ���� ���������
	int m_iLeft;				// �������� x��ǥ
	int m_iTop;				// �������� y��ǥ
	int m_iRight;				// ������ x��ǥ
	int m_iBottom;				// ������ y��ǥ
	/////////////////////////////////////////////////////////////
	// ��¹���� �����ϱ� ���� �������
	static int notation;	// POINT_POINT �Ǵ� POINT_SIZE
public:
	const TRegion* Get() const { return this; }
	const TRegion& GetRef() const { return *this; }
	/////////////////////////////////////////////////////////////
	// �簢�� ������ ���� ������ ��� �Լ���
	const int GetLeft() { return m_iLeft; }	
	const int GetTop()  { return m_iTop; }
	const int* GetRight() { return &m_iRight; }
	const int& GetBottom() { return m_iBottom; }

	int GetWidth() const;						// ���α��� ���	
	const int GetHeight() const;				// ���α��� ���
	void GetStartPoint(int &x, int &y) const;	// ������ ���
	void GetEndPoint(int &x, int &y) const;		// ���� ���
	void GetCenterPoint(int &x, int &y) const;	// �߽��� ���
	bool IsPointInRegion(int x, int y) const;	// ���� ���� �ȿ� �ִ���
	/////////////////////////////////////////////////////////////
	// �簢�� ������ �����ϰ� ó���ϴ� �Լ���
	void SetRect(int l, int t, int r, int b);	// ���� ����
	void Move(int x, int y);					// ��ġ �̵�
	void Resize(int width, int height);			// ũ�� ����
	void UnionRegion(const TRegion &r1, const TRegion &r2);		// ������
	bool IntersectRegion(const TRegion &r1, const TRegion &r2);	// ������
	/////////////////////////////////////////////////////////////
	// �簢�� ������ ǥ���ϱ� ���� �Լ���
	void Show(void);						// ���� ���
	static void SetNotation(int notation);	// ��� ��� ����
public:
	TRegion(void);			// ������
	TRegion(int l, int t, int r, int b);	// ������
};

