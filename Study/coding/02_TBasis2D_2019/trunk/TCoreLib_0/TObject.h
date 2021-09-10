#pragma once
#include "TCoreStd.h"
#include "TImage.h"
class TObject
{
public:
	float		m_fPosition[2];
	float		m_fRadius;
	float		m_fAngle;
	bool		m_bRotation;
	bool		m_bCollide;
	int		    m_iIndex;
	int		    m_iIndexMask;
public:
	HDC      m_hDrawDC;
	HDC		 GetDrawDC() { return m_hDrawDC; }
	TImage*  m_pImage;
	TImage*  m_pImageMask;
	HBITMAP  m_hOldBitmap;
	HBITMAP  m_OldBitmap;
	RECT	 m_SrcRect;
	RECT	 m_DeskRect;	
	RECT	 m_rtCollision;	
	BLENDFUNCTION m_AlphaBlend;
	int			  m_iAlpha;
public:
	bool	 Init();
	bool	 Frame();
	bool	 Render();
	bool	 Release();
	void	 SetPos( float iX, float iY);
	bool	 Load(	TCHAR* strLoadFile,
					TCHAR* strMaskLoadFile);
	void	 CreateOffScreenDC(	HDC hdc,
								int iWidth, 
								int iHeight);
	void	 SetObjDC(	HDC hdc );
	void DrawBitmap(	HDC hdc, 
						int iX, int iY,
						int iWidth, int iHeight,
						int iStartX, int iStartY,
						HBITMAP bitmap,
						DWORD rop = SRCCOPY);
	void Draw(	HDC hdc, 
						int iX, int iY,
						int iWidth, int iHeight,
						HDC srchdc,
						int iStartX, int iStartY,						
						DWORD rop = SRCCOPY);
	HBITMAP RotateBitmap( HDC hdc, float fAngle, int iSizeWidth, int iSizeHeight);
public:
	TObject(void);
	~TObject(void);
};

