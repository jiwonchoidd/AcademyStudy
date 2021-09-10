#pragma once
#include "TStd.h"
#include "TShape.h"
#include <vector>

struct TRectUV
{
	RECT		rtOffset;
	RECT		Rect;
};

class xSprite : public TShape
{
public:
	T_STR			m_szName;
	T_STR			m_szPath;
	int				m_iIndex;
public:
	vector<int>				m_BitmapArray;
	vector<TRectUV>			m_RectList;
	int						m_iApplyIndex;
	int						m_iNumBitmap;
	float					m_fSecPerRender;
	float					m_fTime;
	float					m_fLifeTime;
	RECT					m_rtCurrent;
	RECT					RectSet;		
	
public:
	void		SetPos(tPOINT pos);
	void		Updata( tPOINT pos, float& pfCurrentTimer, int& iApplyIndex,  float fGlobalTime, float fElapsedTime);
	
	void		AnimationFrame(int iApplyIndex);
	float		GetTimer() { return m_fTime; }
	void		SetTimer(float fTime) { m_fTime=fTime; }
public:
	bool		Init();
	bool		Frame( float fGlobalTime, float fElapsedTime );
	bool		Render();
	bool		Release();
	void		SetRectArray(vector<RECT> rtArray, float fLifeTime=1.0f);
	void		SetBitmapArray( TCHAR_STRING_VECTOR FileList );	
	RECT		SetUV(RECT& Rect);
	void		SetLife(float fTime=1.0f);
	void		SetAttribute(	float fAnimTime = 1.0f, 
								int iWidth = 1, 
								int iWidthSize = 0, 
								int iHeight	= 1, 
								int iHeightSize = 0 );

public:
	xSprite(void);
	~xSprite(void);
};

