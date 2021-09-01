#pragma once
#include "TStd.h"
// 1, 게임 실행 시간 누적 기능
// 2, 1 Frame 경과 시간 저장
// 3, 1초 동안 프레임 카운터
// ********* 모든 클래스는 반드시 다음과 같은 함수를 구현한다. *******

class TTimer : public TSingleton<TTimer>
{
public:
	// 1, 게임 실행 시간 누적 기능
	float	m_fGameTimer;
	// 2, 1 Frame 경과 시간 저장
	float   m_fSecondPerFrame;
	// 3, 1초 동안 프레임 카운터
	int		m_iFPS;

	WCHAR	m_szBuffer[256] = { 0, };
	float   m_fBeforeTime;
public:
	bool  Reset();
	// 초기화 및 준비 작업 구현
	bool  Init();
	// 프레임 단위로 계산작업
	bool  Frame();
	// 프레임 단위로 드로우(렌더링)
	bool  Render();
	// 소멸 및 삭제 기능 구현
	bool  Release();
public:
	TTimer();
	virtual ~TTimer();
};
#define g_Timer TTimer::GetInstance()
