
#pragma once
#include "TNetStd.h"
#include "TNetUser.h"
#define MAX_WORKER_THREAD 3
class TServer;

class TIocp 
{
public:
	/// TODO : 서버인스턴스
	TServer* m_pServer;///< TServer point>	
	HANDLE   m_hIOCP;
	HANDLE   m_hWorkThread[MAX_WORKER_THREAD];
	DWORD    m_dwThreadID[MAX_WORKER_THREAD];
public:
	/**
		* 초기화
		*
		* @return bool
	*/
	bool	Init();
	bool	Run();
	bool	Release();
/// <A short one line description>
///
/// <Longer description>
/// <May span multiple lines or paragraphs as needed>
///
/// @param  클라이언트 소켓
/// @param  반환키
/// @return Description of the return value
	void    SetBind(SOCKET sock, ULONG_PTR key);
public:
	/**
		* 생성자
		* @param TServer*
		*/
	TIocp(TServer*);
	virtual ~TIocp();
};

