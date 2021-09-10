#pragma once
#include "TCore.h"

class Sample  : public TCore
{
public:
	OVERLAPPED	m_ov; // �񵿱� ������� ������ �����ϰ� ����.
	HANDLE		m_hFile;
	char*		m_buf;
	TCHAR		m_strBuffer[256];
	int			m_iCount;
	DWORD		m_dwLength;
public:
	bool	Init();
	bool	Frame();
	bool	Release();
	void	T_ERROR();
public:
	Sample(void);
	~Sample(void);
};
