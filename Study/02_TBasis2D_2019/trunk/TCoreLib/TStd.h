#ifndef KGCA
	#define KGCA
	#include <WinSock2.h>
	#include "TVector2.h"
	#include <time.h>
	#include <tchar.h>
	#include <algorithm>
	#include <vector>
	#include <map>
	#include <list>
	#include <string>
	#include <functional>
	#include <set>
	#include <assert.h>
	#include <memory>
	#include <IO.H>
	#include <atlconv.h> // A2W
	using namespace std;
	#pragma comment( lib, "ws2_32.lib")
	#pragma comment( lib, "winmm.lib")
	#pragma comment( lib, "msimg32") ///__imp_TransparentBlt 
#ifdef _DEBUG
	#pragma comment( lib, "TCoreLib64_d.lib")
#else
	#pragma comment( lib, "TCoreLib64.lib")
#endif
#ifndef tMax
#define tMax(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef tMin
#define tMin(a,b)            (((a) < (b)) ? (a) : (b))
#endif

	typedef basic_string<TCHAR> T_STR;
	typedef basic_string<wchar_t> W_STG;
	typedef basic_string<char>  C_STR;
	typedef vector<basic_string<TCHAR>>		TCHAR_STRING_VECTOR;
	typedef vector<DWORD>					DWORD_VECTOR;
	// 멀티바이트 문자집합 사용경우
	// ABC홍길동 = 9
	// [A][B][C] [0]
	// 유니코드바이트 문자집합 사용경우
	// ABC홍길동 = 12
	// [A][0][B][0][C][0] [][][][][][]
	static std::wstring mtw(std::string str)
	{
		std::wstring ret = std::wstring(str.begin(), str.end());
		return  ret;
	}
	static std::string wtm(std::wstring str)
	{
		return  std::string(str.begin(), str.end());
	}


	static std::wstring to_mw(const std::string& _src)
	{
		USES_CONVERSION;
		return std::wstring(A2W(_src.c_str()));
	};

	static std::string to_wm(const std::wstring& _src)
	{
		USES_CONVERSION;
		return std::string(W2A(_src.c_str()));
	};
	static char* GetWtM(WCHAR* data)
	{
		char retData[4096] = { 0 };
		// 변형되는 문자열의 크기가 반환된다.
		int iLength = WideCharToMultiByte(CP_ACP, 0,
			data, -1, 0, 0, NULL, NULL);
		int iRet = WideCharToMultiByte(CP_ACP, 0,
			data, -1,  //  소스
			retData, iLength, // 대상
			NULL, NULL);
		return retData;
	}
	static bool GetWtM(WCHAR* src, char* pDest)
	{
		// 변형되는 문자열의 크기가 반환된다.
		int iLength = WideCharToMultiByte(CP_ACP, 0,
			src, -1, 0, 0, NULL, NULL);
		int iRet = WideCharToMultiByte(CP_ACP, 0,
			src, -1,  //  소스
			pDest, iLength, // 대상
			NULL, NULL);
		if (iRet == 0) return false;
		return true;
	}
	static WCHAR* GetMtW(char* data)
	{
		WCHAR retData[4096] = { 0 };
		// 변형되는 문자열의 크기가 반환된다.
		int iLength = MultiByteToWideChar(CP_ACP, 0, data, -1, 0, 0);
		int iRet = MultiByteToWideChar(CP_ACP, 0,
			data, -1,  //  소스
			retData, iLength); // 대상
		return retData;
	}
	static bool GetMtW(char* pSrc, WCHAR* pDest)
	{
		// 변형되는 문자열의 크기가 반환된다.
		int iLength = MultiByteToWideChar(CP_ACP, 0,
			pSrc, -1, 0, 0);
		int iRet = MultiByteToWideChar(CP_ACP, 0,
			pSrc, -1,  //  소스
			pDest, iLength); // 대상		
		if (iRet == 0) return false;
		return true;
	}
	static void PRINT(char* fmt, ...) // 나열연산자
	{
		va_list arg;
		va_start(arg, fmt);
		char buf[256] = { 0, };
		vsprintf_s(buf, fmt, arg);
		printf("\n=====> %s", buf);
		va_end(arg);
	}
	template<class T> class TSingleton
	{
	public:
		static T& GetInstance()
		{
			static T theSingleInstance;
			return theSingleInstance;
		}
	};
#define TCORE_START int WINAPI wWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPWSTR    lpCmdLine,int       nCmdShow){Sample sample;
#define TCORE_WIN(s,x,y) if (sample.SetWindow(hInstance, L#s, x, y))	{	sample.Run();	} }
#define TCORE_RUN(s,x,y) TCORE_START;TCORE_WIN(s,x,y);
#define TCORE_END }

#define TBASIS_EPSILON		((FLOAT)  0.001f)
#define TBASIS_PI			((FLOAT)  3.141592654f)
#define DegreeToRadian( degree ) ((degree) * (TBASIS_PI / 180.0f))
#define RadianToDegree( radian ) ((radian) * (180.0f / TBASIS_PI))
#define MAKECOLOR_ARGB(a, r, g, b)			(((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)
#define IS_IN_RANGE(value,r0,r1) (( ((r0) <= (value)) && ((value) <= (r1)) ) ? 1 : 0)
#define randf(x) (x*rand()/(float)RAND_MAX)
#define randf2(x,off) (off+x*rand()/(float)RAND_MAX)
#define randstep(fMin,fMax) (fMin+((float)fMax-(float)fMin)*rand()/(float)RAND_MAX)
#define clamp(x,MinX,MaxX) if (x>MaxX) x=MaxX; else if (x<MinX) x=MinX;
	//////////////////////////////////////////////
	// 객체 및 배열 할당과 삭제 및 소멸 매크로
	//////////////////////////////////////////////
#ifndef SAFE_ZERO
#define SAFE_ZERO(A)				{ A = 0; }
#endif

#ifndef SAFE_NEW
#define SAFE_NEW(A, B)				{ if (!A) A = new B; }
#endif 

#ifndef SAFE_DEL
#define SAFE_DEL(A)					{ if (A) delete A; (A)=NULL; }
#endif 

#ifndef SAFE_NEW_ARRAY
#define SAFE_NEW_ARRAY(A, B, C)		{ if (!A && C) A = new B[C]; }
#endif 

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(A)		{ if (A) delete [] A; (A)=NULL; }
#endif 

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(A)				{ if(A) { (A)->Release(); (A)=NULL; } }
#endif 

#ifndef SAFE_NEW_CLEAR
#define SAFE_NEW_CLEAR( A, B )			{ if (!A) A = new B; if(A) memset( A, 0, sizeof(B) ); };
#endif 

#ifndef SAFE_NEW_ARRAY_CLEAR
#define SAFE_NEW_ARRAY_CLEAR( A, B, C )	{ if (!A && C) A = new B[C]; if(A) memset( A, 0, sizeof(B)*C ); };
#endif
	// 추가
#define COUNTOF(a) ( sizeof( a ) / sizeof( ( a )[0] ) )

	static void T_ERR_EXIT(TCHAR *msg)
	{
		LPVOID lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(TCHAR*)&lpMsgBuf, 0, NULL);
		MessageBox(NULL, (TCHAR*)lpMsgBuf, msg, MB_ICONERROR);
		LocalFree(lpMsgBuf);
		exit(-1);
	}
	static void T_ERR(TCHAR *msg)
	{
		LPVOID lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(TCHAR*)&lpMsgBuf, 0, NULL);
		MessageBox(NULL, (TCHAR*)lpMsgBuf, msg, MB_ICONERROR);
		LocalFree(lpMsgBuf);
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////
	// 디버그 메세지( 오류가 예상되는 곳에서 사용함 ) : 파일명과 라인 및 문자열이 출력되며 강제 종료됨.
	///////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define	DEBUGMSG(lpText)															\
	{																						\
		TCHAR szBuffer[256];															\
		_stprintf_s(szBuffer, _T("[File: %s][Line: %d]\n[Note : %s]"), __FILE__, __LINE__, lpText);	\
		MessageBox(NULL, szBuffer, _T("ERROR"), MB_ICONERROR);									\
		_ASSERT(1);																			\
		_asm {																				\
		int 3																			\
		}																					\
	}
#else
#define DEBUGMSG(lpText)
#endif
	struct TRect
	{
		float    left;
		float    top;
		float    right;
		float    bottom;
		void operator = (const RECT rt)
		{
			left = (float)rt.left;
			top = (float)rt.top;
			right = (float)rt.right;
			bottom = (float)rt.bottom;
		}
	};
	
	struct TIndex
	{
		int x, y;
		bool operator == (const TIndex& pos)
		{
			return (x == pos.x && y == pos.y);
		}
		TIndex operator + (const TIndex& right_)
		{
			return{ x + right_.x, y + right_.y };
		}
		TIndex(int fX, int fY)
		{
			x = fX;
			y = fY;
		}
		TIndex()
		{
			x = y = 0;
		}
	};
	struct TSphere
	{
		TPoint vCenter;
		float  fRadius;
	};

	struct TInputActionMap
	{
		DWORD   bWKey;
		DWORD   bSKey;
		DWORD   bAKey;
		DWORD   bDKey;
		DWORD   bLeftClick;
		DWORD   bRightClick;
		DWORD   bMiddleClick;
		DWORD   bExit;
		DWORD   bSpace; // left mouse, A key
	};
	enum { KEY_FREE, KEY_PUSH, KEY_HOLD, KEY_UP };
	extern TInputActionMap  g_ActionInput;
	extern POINT			g_MousePos;
	extern HWND			g_hWnd;
	extern HINSTANCE	g_hInstance;
	extern RECT			g_rtClient;
	extern int			g_iFPS;// 1초당 프레임 개수
	extern float		g_fSecPerFrame; // 1프레임의 경과시간
	extern float		g_fAccumulation;// 실행된 경과 시간
	extern HDC			g_hScreenDC;
	extern HDC			g_hOffScreenDC;
	extern DWORD		g_dwFixedFrame;
	extern DWORD		g_dwStepTick;
#endif