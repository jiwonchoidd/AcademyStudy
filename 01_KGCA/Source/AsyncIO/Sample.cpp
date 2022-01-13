#include <windows.h>
#include <iostream>
/*	CreateFile();
	WriteFile();
	ReadFile();
	CloseHandle();*/
//비동기 입출력
//작업해라 하면 누군가는 작업을 해 
//작업이 끝나면 알아서 알려줌.
int main()
{
#pragma region 동기 파일 입출력 1
	setlocale(LC_ALL, ""); // Korean해도 되지만 공백은 시스템 지역설정으로 따르게 한다.
	HANDLE hFile = CreateFile(L"test.txt", GENERIC_WRITE
		, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//CREATE_ALWAYS : 파일 덮어쓸건지
	if (hFile != NULL)
	{
		WCHAR buffer[] = L"유니코드든 싱글바이트든 해석할때가 중요하다.";
		DWORD dLength = sizeof(buffer);
		DWORD dWritten;
		bool bRet = WriteFile(hFile, buffer, dLength, &dWritten, NULL);

		if (bRet == true)
		{
			std::wcout << L"출력성공" << std::endl;
		}
	}
	CloseHandle(hFile);


	HANDLE hFile_Read = CreateFile(L"test.txt", GENERIC_READ
		, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile_Read != NULL)
	{
		WCHAR buffer[512] = { 0, };
		DWORD dLength = sizeof(buffer);
		DWORD dWritten;
		bool bRet = ReadFile(hFile_Read, buffer, dLength, &dWritten, NULL);

		if (bRet == true)
		{
			std::wcout << L"읽어오기 성공" << std::endl;
		}
	}
	CloseHandle(hFile_Read);
#pragma endregion
}
//	#pragma region 동기 파일 입출력 2
//	//한글 window 
//	setlocale(LC_ALL, ""); // Korean해도 되지만 공백은 시스템 지역설정으로 따르게 한다.
//	HANDLE hFile = CreateFile(L"main.jpg", GENERIC_WRITE | GENERIC_READ
//		, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//
//	LARGE_INTEGER fileSize;
//	//파일 용량이라는 것은 매우 크기 때문에 Large_Interger를 쓴다.
//	// 
//	//CREATE_ALWAYS : 파일 덮어쓸건지
//	if (hFile != NULL)
//	{
//		GetFileSizeEx(hFile, &fileSize);
//		WCHAR buffer[] = L"유니코드든 싱글바이트든 해석할때가 중요하다.";
//		DWORD dLength = sizeof(buffer);
//		DWORD dWritten;
//		bool bRet = WriteFile(hFile, buffer, dLength, &dWritten, NULL);
//
//		if (bRet == true)
//		{
//			std::wcout << L"출력성공" << std::endl;
//		}
//	}
//	CloseHandle(hFile);
//
//
//	HANDLE hFile_Read = CreateFile(L"test.txt", GENERIC_READ
//		, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//	if (hFile_Read != NULL)
//	{
//		WCHAR buffer[512] = { 0, };
//		DWORD dLength = sizeof(buffer);
//		DWORD dWritten;
//		bool bRet = ReadFile(hFile_Read, buffer, dLength, &dWritten, NULL);
//
//		if (bRet == true)
//		{
//			std::wcout << L"읽어오기 성공" << std::endl;
//		}
//	}
//	CloseHandle(hFile_Read);
//#pragma endregion
//
//#pragma region 비동기 파일 입출력 1
//	/*OVERLAPPED 구조체 ReadFile 마지막 인자에 넣음
//	비동기적인 입출력을 하게끔 된다, 
//	CreateFile FILE_FLAG_OVERLAPPED 플래그를 남겨야함
//	
//	비동기, 함수 호출하면 바로 리턴된다. 누군가는 읽고 있음
//	비동기 작업하는 기능이 프로세스에 있다. 우리가 직접만든 스레드는
//	아니지만 아무튼 있다. 문제는 리턴값이 FALSE가 나온다. 오류인지 아닌지
//	체크를 해줘야한다. GetLastError() == error_io_pending 지금 작업하고 있으니
//	기다리라는 뜻, 로드중이라고 출력하면된다. 만약 이 조건이 아니면 실패한 것이므로
//	로드 실패..
//	로딩 중일때 작업 완료된 결과를 알려달라...비동기 작업의 결과를 알 수있는 함수 GetOverlappedResult()
//	과정을 얻는 함수는 아니다. DWORD dwTrans 결과가 리턴이 될뿐이지만 비슷하게 작업 가능
//	bWait TRUE 하면 비동기 작업이 끝나여 반환함. 
//	bWail FALSE 기다리지 않겠다. 
//	패킷처리 할때 오프셋 썼듯이 쓰레드는 아니지만 얼만큼 읽었다. 얼만큼 남았다. 
//	내부적으로 쓰는 데이터, 이 파일 작업이 끝나면 hEvent이 시그널된다.
//	
//	출력할때는 OVERLAPPED에서 추가적으로 FILE_FLAG_WRITE_THROUGH 비동기 출력이 안되서
//	하나 더 추가함
//	메인스레드가 중간중간 확인하고 있다 근데 의미가 없음 WHILE(BPADING) 이벤트로 확인가능
//
//	얼만큼 읽었니, 얼만큼 썻니를 확인할수없음. 진행바를 표시해야하는데,
//	15초가 넘어가면 심리적으로 끄게됨, MS에서 15초가 넘어도 되는데 화면이 정지상태로 15초는 안됨
//	비동기를 써야한다는 뜻이다. 
//	*/
//
//	//한글 window 
//	setlocale(LC_ALL, ""); // Korean해도 되지만 공백은 시스템 지역설정으로 따르게 한다.
//	HANDLE hFile = CreateFile(L"main.jpg", GENERIC_WRITE | GENERIC_READ
//		, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
//	//CREATE_ALWAYS : 파일 덮어쓸건지
//	if (hFile != NULL)
//	{
//		WCHAR buffer[] = L"유니코드든 싱글바이트든 해석할때가 중요하다.";
//		DWORD dLength = sizeof(buffer);
//		DWORD dWritten;
//		bool bRet = WriteFile(hFile, buffer, dLength, &dWritten, NULL);
//
//		if (bRet == true)
//		{
//			std::wcout << L"출력성공" << std::endl;
//		}
//	}
//	CloseHandle(hFile);
//
//#pragma endregion
//}