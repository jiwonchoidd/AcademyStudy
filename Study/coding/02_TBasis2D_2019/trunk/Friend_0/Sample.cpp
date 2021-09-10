#include "TToken.h"
void main()
{
	TCHAR buffer[] = L"kgca game academy";
	// 방법 1 ( 금고와 열쇠 관계)
	// 생성자가 private로 되어 있어서 접근 할 수 없다.
	// B_TYPE 매크로가 활성화 되면 public 생성자가를 사용한다.
	//TValue value(buffer);
	//TToken token;
	//token.Show(&value);

	// 방법 2 
	try {
		TToken token(buffer);
		token.Show();
#ifndef _UNICODE
		std::cout << _T("Show the 3rd token:") << x.nth_token(3) << _T("\n");
#else
		std::wcout << _T("Show the 3rd token:") << token.nth_token(3) << _T("\n");
#endif
	}
	catch (TCHAR* pData)
	{
		std::wcout << _T("Error!!!!");
	}

	//////////////////////////// stl ///////////////////////////////////
	std::vector<std::string> str = split("one:two::three", ':');

}