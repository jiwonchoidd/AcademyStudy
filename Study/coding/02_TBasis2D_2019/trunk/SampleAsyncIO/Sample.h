#pragma once
#include "TCore.h"

class Sample  : public TCore
{
public:
	OVERLAPPED	m_ov; // 비동기 입출력의 개수와 동일하게 생성.
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
