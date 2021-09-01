#pragma once
#include "TStd.h"
#include "TImage.h"
#include "TVector2.h"

struct TObjectInfo
{
	TPoint	InitPos;
	int		iNumFrame;
	float	fTime;
	float	fLifeTime;
	int		iBitmapID;
	int		iMaskID;
	bool	bLoop;
	TPoint	pos;
	RECT	rtClient;
	float	fSpeed;
	std::vector<RECT> rtList;
	TObjectInfo() 
	{
		iBitmapID = -1;
		iMaskID = -1;
		bLoop = false;
	}
	TObjectInfo(TPoint p, RECT r, float f)
	{
		pos = p;
		rtClient = r;
		fSpeed = f;
		iBitmapID = -1;
		iMaskID = -1;
		bLoop = false;
	}
};
class TObject
{
public:
	TObjectInfo m_info;
	float		m_fDeltaTime;// = 3.0f / iNumSprite;
	bool		m_bDead;
	int			m_iSpriteID;
	int			m_iCurrentFrame;
	float       m_fTmpTime;
	
public:
	TPoint		m_vPos;
	float		m_fRadius;
	float		m_fAngle;
	bool		m_bRotation;
	bool		m_bCollide;
	int		    m_iIndex;
	int		    m_iIndexMask;
	float		m_fAlpha;
	float		m_fSpeed;
	HDC			m_hDrawDC;
	HDC			m_hColorDC;
	HDC			m_hMaskDC;
	TImage*		m_pImage;
	TImage*		m_pImageMask;
	HBITMAP		m_hOldBitmap;
	HBITMAP		m_OldBitmap;
	RECT		m_SrcRect;
	RECT		m_DeskRect;	
	RECT		m_rtCollision;	
	TSphere		m_Sphere;
	BLENDFUNCTION m_AlphaBlend;
	int			  m_iAlpha;
	bool		  m_bDebugRect;
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
public:
	HDC			GetDrawDC() { return m_hDrawDC; }
	void		Move(TPoint pos);
	void		SetPos(POINT pos);
	void		SetPos(TPoint pos);
	void		SetPos(float x, float y);
	void		SetUpdateData();
	bool		Set(TObjectInfo info, int id, int mask);
	bool		SetSourceRect(float iX, float iY, RECT rt, float fSpeed = 300);
	bool		Load(	const TCHAR* strLoadFile, const TCHAR* strMaskLoadFile=0);
	void		DrawBitmap(	HDC hdc, 
						int iX, int iY,
						int iWidth, int iHeight,
						int iStartX, int iStartY,
						HBITMAP bitmap,
						DWORD rop = SRCCOPY);
	void  Draw(DWORD dwMode);
	void  Draw(int x, int y, DWORD dwMode);
	void  Draw(	HDC hdc, 
						int iX, int iY,
						int iWidth, int iHeight,
						HDC srchdc,
						int iStartX, int iStartY,						
						DWORD rop = SRCCOPY);
	void DrawColorKey(HDC hdc, int iX, int iY, int iWidth, int iHeight,
					HDC srchdc, int iStartX, int iStartY, int iSrcWidth, int iSrcHeight,
					DWORD rop = SRCCOPY, DWORD dwColorKey = RGB(255, 255, 255));
	void DrawAlphaBlend(HDC hdc, int iX, int iY, int iWidth, int iHeight,
			HDC srchdc, int iStartX, int iStartY, int iSrcWidth, int iSrcHeight, float fAlpha=255.0f);
	void Draw(HDC hdc,
		int iX, int iY,
		int iWidth, int iHeight,
		HDC srchdc,
		int iStartX, int iStartY, int iSrcWidth, int iSrcHeight,
		DWORD rop = SRCCOPY);
	// clrMask 픽셀을 투명처리한다.
	void		TransBitBlt(HDC hdc, int x, int y, int width, int height,
							HDC MemDC, int mx, int my, COLORREF clrMask);
public:
	HBITMAP		MakeDIBSection(HDC hdc, int width, int height, int iBitCount=24);
	void		RotateBlt(float iAngle, float fAlpha = 255);
	void		GetRotationBimap(HDC hdc, HDC srdDC,float fAngle,int iWidth,int iHeight);
	bool		AlphaBlend(	HDC dcDest, int x, int y, int cx, int cy,
							HDC dcSrc,
							HDC dcMaskSrc, int sx, int sy, int scx, int scy, 
							int alpha, DWORD opMode, COLORREF rgbMask);
public:
	TObject(void);
	virtual ~TObject(void);
};

