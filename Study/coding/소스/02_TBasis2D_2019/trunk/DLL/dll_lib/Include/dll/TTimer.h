#pragma once
#include "TCoreStd.h"

class TCOREDLL_API TTimer : public TSingleton<TTimer>
{
public:		
//	friend  class TSingleton<TTimer>;
	TCHAR		m_strBuffer[MAX_PATH];
private:
	// 1�ʴ� ������ Ƚ��
	int			m_iFPS; 
	// 1�������� ����� �ð�
	float		m_fSecPerFrame; 
	// ���������� ����� �ð�
	float		m_fAccumulation; 
	// �ӽú���
	DWORD		m_dwBeforeTick;
	float		m_fFrameTime;
	DWORD		m_dwFrameCnt;
public:	
	bool		Init();	
	bool		Frame();
	bool		Render();
	bool		Release();
public:
	TTimer(void);
	~TTimer(void);
};
#define I_Timer TTimer::GetInstance()
