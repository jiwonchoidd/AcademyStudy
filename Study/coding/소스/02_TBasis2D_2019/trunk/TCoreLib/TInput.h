#pragma once
#include "TStd.h"

class TInput : public TSingleton<TInput>
{
private:
	friend class TSingleton<TInput>;
public:
	std::vector<wstring>    m_strList;
	TCHAR		m_csBuffer[MAX_PATH];
	DWORD		m_dwKeyState[256];
	POINT		m_MousePos;
public:
	DWORD		KeyCheck(DWORD dwKey);
	// Ŭ���� ��� ���� �ʱ�ȭ
	bool		Init();
	// �� �����ӿ��� ����� ����Ѵ�.
	bool		Frame();
	// �� �����ӿ��� ��ο츦 ����Ѵ�.
	bool		Render();
	// Ŭ���� ��� ���� �Ҹ��� ����Ѵ�.
	bool		Release();
	void		MsgEvent(MSG msg);
protected:
	TInput();
	virtual ~TInput();
};
#define I_Input TInput::GetInstance()
