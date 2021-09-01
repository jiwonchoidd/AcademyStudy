#pragma once
#include "TStd.h"
class TTimer
{
public:
	TCHAR		m_csBuffer[MAX_PATH];
	int			m_iFPS;// 1초당 프레임 개수
	float		m_fSecondPerFrame; // 1프레임의 경과시간
	float		m_fAccumulation;// 실행된 경과 시간

	DWORD		m_dwBeforeTick;
	float		m_fFrameTime; // 임시변수
	DWORD		m_dwFrameCnt;
public:
	// 클래스 멤버 변수 초기화
	bool		Init();
	// 매 프레임에서 계산을 담당한다.
	bool		Frame();
	// 매 프레임에서 드로우를 담당한다.
	bool		Render();
	// 클래스 멤버 변수 소멸을 담당한다.
	bool		Release();
public:
	TTimer();
	virtual ~TTimer();
};

