#include "TToken.h"
void main()
{
	TCHAR buffer[] = L"kgca game academy";
	// ��� 1 ( �ݰ�� ���� ����)
	// �����ڰ� private�� �Ǿ� �־ ���� �� �� ����.
	// B_TYPE ��ũ�ΰ� Ȱ��ȭ �Ǹ� public �����ڰ��� ����Ѵ�.
	//TValue value(buffer);
	//TToken token;
	//token.Show(&value);

	// ��� 2 
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