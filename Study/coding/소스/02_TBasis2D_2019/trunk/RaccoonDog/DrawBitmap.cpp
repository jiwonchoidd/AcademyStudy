#include <string.h>
#include "DrawBitmap.h"

void DrawBitmap(HDC hDC, int x, int y, HBITMAP hBit, BOOL bTrans,
				COLORREF crTransColor)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;

	MemDC = CreateCompatibleDC(hDC);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	// 특정 색깔을 투명하게 할 것인가?
	 if (bTrans)
		 TransBitBlt(hDC, x, y, bx, by, MemDC, 0, 0, crTransColor);
	 else 
		 BitBlt(hDC, x, y, bx, by, MemDC, 0, 0, SRCCOPY);	 

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}



void DrawBitmap(HDC hDC, int x, int y, HBITMAP hBit, BOOL bTrans , int s_x, int s_y,
	  int e_x , int e_y ,COLORREF crTransColor)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;

	MemDC = CreateCompatibleDC(hDC);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	// 특정 색깔을 투명하게 할 것인가?
	 if (bTrans) {
		 // 그릴 범위를 고려하여 이미지 크기 계산 
		 int DrawWidth = e_x-s_x;
		 int DrawHeight = e_y-s_y;
		 TransBitBlt(hDC, x, y, DrawWidth, DrawHeight, MemDC, s_x, s_y, crTransColor);
	 } else {
		int DrawWidth = e_x-s_x;
		int DrawHeight = e_y-s_y;
		BitBlt(hDC, x, y, DrawWidth, DrawHeight, MemDC, s_x, s_y, SRCCOPY);
	 }

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}


void DrawBitmapAlpha(HDC hDC, int x, int y, HBITMAP hBit, int alp )
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;
	BLENDFUNCTION bf;

	MemDC = CreateCompatibleDC(hDC);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = alp;
	bf.AlphaFormat = 0;
	
	AlphaBlend(hDC, x, y, bx, by, MemDC, 0, 0, bx, by, bf);		
	
	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}



void DrawBitmapAlpha(HDC hDC, int x, int y, HBITMAP hBit, int alp, int s_x, int s_y,
	  int e_x , int e_y)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;
	BLENDFUNCTION bf;

	MemDC = CreateCompatibleDC(hDC);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = alp;
	bf.AlphaFormat = 0;

	int DrawWidth = e_x-s_x;
	int DrawHeight = e_y-s_y;
	AlphaBlend(hDC, x, y, DrawWidth, DrawHeight, MemDC, s_x, s_y, DrawWidth, DrawHeight, bf);			 		

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}

void DrawDigit(HDC hDC, int x, int y, int score, HBITMAP hBit, int cipher,
			   COLORREF crTransColor)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;

	MemDC = CreateCompatibleDC(hDC);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth / 10;
	by = bit.bmHeight;

	int len;
	int blank = 0;
	char str[10];	

	itoa(score, str, 10);
	len = strlen(str);

	if (cipher) 
		blank = cipher - len - 1;

	//숫자를 자리수를 고려하여 공백을 넣고 숫자 표시  
	for (int i = 0; i < len; i++)
		TransBitBlt(hDC, x+(blank*bx)+(i*bx), y, bx, by, MemDC, bx*(str[i]-'0'), 0, crTransColor);
	
	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}


 
void TransBitBlt(HDC hdc, int x, int y, int width, int height, 
				 HDC MemDC, int mx, int my, COLORREF clrMask)
{
	COLORREF cColor;
	HBITMAP bmAndBack, bmAndObject, bmAndMem, bmSave;
	HBITMAP bmBackOld, bmObjectOld, bmMemOld, bmSaveOld;
	HDC hdcMem, hdcBack, hdcObject, hdcSave;

	hdcBack = CreateCompatibleDC(hdc);
	hdcObject = CreateCompatibleDC(hdc);
	hdcMem = CreateCompatibleDC(hdc);
	hdcSave = CreateCompatibleDC(hdc);

	bmAndBack = CreateBitmap(width, height, 1, 1, NULL);
	bmAndObject = CreateBitmap(width, height, 1, 1, NULL);
	bmAndMem = CreateCompatibleBitmap(hdc, width, height);
	bmSave = CreateCompatibleBitmap(hdc, width, height);

	bmBackOld = (HBITMAP) SelectObject(hdcBack, bmAndBack);
	bmObjectOld = (HBITMAP) SelectObject(hdcObject, bmAndObject);
	bmMemOld = (HBITMAP) SelectObject(hdcMem, bmAndMem);
	bmSaveOld = (HBITMAP) SelectObject(hdcSave, bmSave);

	SetMapMode(MemDC, GetMapMode(hdc));

	BitBlt(hdcSave, 0, 0, width, height, MemDC, mx, my, SRCCOPY);

	cColor = SetBkColor(MemDC, clrMask);

	BitBlt(hdcObject, 0, 0, width, height, MemDC, mx , my, SRCCOPY);

	SetBkColor(MemDC, cColor);

	BitBlt(hdcBack, 0, 0, width, height , hdcObject, 0, 0, NOTSRCCOPY);
	BitBlt(hdcMem , 0, 0, width, height , hdc , x, y, SRCCOPY);
	BitBlt(hdcMem , 0, 0, width, height , hdcObject, 0, 0, SRCAND);
	BitBlt(MemDC , mx, my, width, height , hdcBack , 0, 0, SRCAND);
	BitBlt(hdcMem , 0, 0, width, height, MemDC , mx, my, SRCPAINT);
	BitBlt(hdc , x, y, width, height, hdcMem , 0, 0, SRCCOPY);
	BitBlt(MemDC , mx, my, width, height, hdcSave , 0, 0, SRCCOPY);

	DeleteObject(SelectObject(hdcBack, bmBackOld));
	DeleteObject(SelectObject(hdcObject, bmObjectOld));
	DeleteObject(SelectObject(hdcMem, bmMemOld));
	DeleteObject(SelectObject(hdcSave, bmSaveOld));

	DeleteDC(hdcMem);
	DeleteDC(hdcBack);
	DeleteDC(hdcObject);
	DeleteDC(hdcSave);
}
