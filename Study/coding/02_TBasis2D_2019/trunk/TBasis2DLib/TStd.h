#pragma once
#include "TBasisStd.h"

#if NDEBUG
#pragma comment (lib, "TBasis2DLib_R.lib")
#else
#pragma comment (lib, "TBasis2DLib_D.lib")
#endif

using namespace std;

namespace TBASIS_CORE_LIB
{
	struct TKeyMap
	{
		bool   m_bFront; // up, w
		bool   m_bLeft;  // left, a
		bool   m_bRight; // right. d
		bool   m_bBack;  // down, s
		bool   m_bAttack;// enter, lbutton
		bool   m_bJump;  // space, rbutton
		bool   m_bHangul;
		bool   m_bCapsLock;
		bool   m_bNumLock;
	};
	
	extern TKeyMap  g_KeyMap;
	extern float	g_fGameTimer;
	extern float    g_fSecondPerFrame;
	extern HWND     g_hWnd;
	extern HINSTANCE     g_hInstance;
	extern HDC		g_hScreenDC;
	extern HDC		g_hOffScreenDC;
	extern RECT		g_rtClient;
	extern bool		g_bActive;	
}

using namespace TBASIS_CORE_LIB;