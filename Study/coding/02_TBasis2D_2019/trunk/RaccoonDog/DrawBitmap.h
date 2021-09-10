#pragma once
#include <windows.h>

// ��Ʈ�� �̹��� ��ü�� �׸���.
// 5��° ���ڴ� 6��°���ڰ� ������ ������ �������� �׸� ���ΰ�?
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBlt, BOOL bTrans, 
				COLORREF crTransColor = RGB(0,0,0));

// ��Ʈ�� �̹����� �κи��� �׸���.
// s_x : start_x
// e_x : end_x
// ���� ����ϴ� �Լ��� �ٸ��ϴ�.
// �� �Լ��� �̹����� Ư�� ������ ���ؼ� �� �κ��� �׸��ϴ�.
// ���� å�� �ִ� �Լ��� ���� �ٸ��ϴ�. 
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBlt, BOOL bTrans, int s_x, int s_y,
	  int e_x, int e_y, COLORREF crTransColor = RGB(0,0,0));

// ��Ʈ�� �̹��� ��ü�� ���� ȿ���� �༭ �׸���.
void DrawBitmapAlpha(HDC hdc, int x, int y, HBITMAP hBlt, int alp);

// ��Ʈ�� �̹��� �κ��� ���� ȿ���� �༭ �׸���.
void DrawBitmapAlpha(HDC hdc, int x, int y, HBITMAP hBlt, int alp, int s_x, int s_y,
	  int e_x, int e_y);

// ���� ǥ�� �Լ� 
// �ִ� 10�ڸ� ���ڱ����� ǥ�õ� 
// cipher�� �ڸ���, �ڸ����� �����ϸ� ������ ��ŭ �տ� ������ ����, �� ���� ���� 
void DrawDigit(HDC hdc, int x, int y, int score, HBITMAP hBit, int cipher = 0,
			   COLORREF crTransColor = RGB(0,0,0));

// TransparentBlt ���� ���� ���� 
void TransBitBlt(HDC hdc, int x, int y, int width, int height, HDC MemDC,
				 int mx, int my, COLORREF clrMask);
