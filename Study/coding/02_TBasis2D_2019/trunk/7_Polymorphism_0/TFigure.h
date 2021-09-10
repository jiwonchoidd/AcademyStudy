#pragma once
#include <iostream>
using namespace std;

class TFigure
{
public:	
	void Move(int x, int y);			// ���� �̵�
	void Resize(int width, int height);	// ���� ũ�� ����
	virtual void Draw();				// ���� �׸���
protected:
	int x;					// �߽��� x��ǥ
	int y;					// �߽��� x��ǥ
	int width;					// ���� ����
	int height;				// ���� ����
public:
	TFigure();
	TFigure(int x, int y, int width, int height);
	virtual ~TFigure();
};
