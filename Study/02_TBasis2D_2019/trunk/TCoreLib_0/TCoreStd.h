#ifndef TCORE_STD
	#define TCORE_STD
		#include <winsock2.h>
		#include <windows.h>
		#include <process.h>
		#include <tchar.h>
		#include <math.h>
		#include <vector>
		#include <string>
		#include <map>
		#include <list>
		using namespace std;
		#pragma comment(lib, "ws2_32.lib")

		typedef basic_string<TCHAR> TCHAR_STRING;
		typedef basic_string<CHAR> C_STR;

		template<class T> class TSingleton
		{
		public:
			static T& GetInstance()
			{
				static T theSingleInstance;
				return theSingleInstance;
			}
		};
		// 할당 소멸 관련 매크로
	#ifndef SAFE_ZERO
			#define SAFE_ZERO(A)	{ A = 0; }
	#endif

	#ifndef SAFE_NEW
		#define SAFE_NEW(A, B)		{ if (!A) A = new B; }
	#endif 

	#ifndef SAFE_DEL
		#define SAFE_DEL(A)		{ if (A) delete A; (A)=NULL; }
	#endif 

	#ifndef SAFE_NEW_ARRAY
		#define SAFE_NEW_ARRAY(A, B, C)	{ if (!A && C) A = new B[C]; }
	#endif 

	#ifndef SAFE_DELETE_ARRAY
		#define SAFE_DELETE_ARRAY(A)	{ if (A) delete [] A; (A)=NULL; }
	#endif 

	#ifndef SAFE_RELEASE
		#define SAFE_RELEASE(A)		{ if(A) { (A)->Release(); (A)=NULL; } }
	#endif 

	#ifndef SAFE_NEW_CLEAR
		#define SAFE_NEW_CLEAR( A, B )	{ if (!A) A = new B; if(A) memset( A, 0, sizeof(B) ); };
	#endif 

	#ifndef SAFE_NEW_ARRAY_CLEAR
		#define NEW_ARRAY_CLEAR( A, B, C )	{ if (!A && C) A = new B[C]; if(A) memset( A, 0, sizeof(B)*C ); };
	#endif
#endif