#include <windows.h>
#include <iostream>
/*	CreateFile();
	WriteFile();
	ReadFile();
	CloseHandle();*/
//�񵿱� �����
//�۾��ض� �ϸ� �������� �۾��� �� 
//�۾��� ������ �˾Ƽ� �˷���.
int main()
{
#pragma region ���� ���� ����� 1
	setlocale(LC_ALL, ""); // Korean�ص� ������ ������ �ý��� ������������ ������ �Ѵ�.
	HANDLE hFile = CreateFile(L"test.txt", GENERIC_WRITE
		, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//CREATE_ALWAYS : ���� �������
	if (hFile != NULL)
	{
		WCHAR buffer[] = L"�����ڵ�� �̱۹���Ʈ�� �ؼ��Ҷ��� �߿��ϴ�.";
		DWORD dLength = sizeof(buffer);
		DWORD dWritten;
		bool bRet = WriteFile(hFile, buffer, dLength, &dWritten, NULL);

		if (bRet == true)
		{
			std::wcout << L"��¼���" << std::endl;
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
			std::wcout << L"�о���� ����" << std::endl;
		}
	}
	CloseHandle(hFile_Read);
#pragma endregion
}
//	#pragma region ���� ���� ����� 2
//	//�ѱ� window 
//	setlocale(LC_ALL, ""); // Korean�ص� ������ ������ �ý��� ������������ ������ �Ѵ�.
//	HANDLE hFile = CreateFile(L"main.jpg", GENERIC_WRITE | GENERIC_READ
//		, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//
//	LARGE_INTEGER fileSize;
//	//���� �뷮�̶�� ���� �ſ� ũ�� ������ Large_Interger�� ����.
//	// 
//	//CREATE_ALWAYS : ���� �������
//	if (hFile != NULL)
//	{
//		GetFileSizeEx(hFile, &fileSize);
//		WCHAR buffer[] = L"�����ڵ�� �̱۹���Ʈ�� �ؼ��Ҷ��� �߿��ϴ�.";
//		DWORD dLength = sizeof(buffer);
//		DWORD dWritten;
//		bool bRet = WriteFile(hFile, buffer, dLength, &dWritten, NULL);
//
//		if (bRet == true)
//		{
//			std::wcout << L"��¼���" << std::endl;
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
//			std::wcout << L"�о���� ����" << std::endl;
//		}
//	}
//	CloseHandle(hFile_Read);
//#pragma endregion
//
//#pragma region �񵿱� ���� ����� 1
//	/*OVERLAPPED ����ü ReadFile ������ ���ڿ� ����
//	�񵿱����� ������� �ϰԲ� �ȴ�, 
//	CreateFile FILE_FLAG_OVERLAPPED �÷��׸� ���ܾ���
//	
//	�񵿱�, �Լ� ȣ���ϸ� �ٷ� ���ϵȴ�. �������� �а� ����
//	�񵿱� �۾��ϴ� ����� ���μ����� �ִ�. �츮�� �������� �������
//	�ƴ����� �ƹ�ư �ִ�. ������ ���ϰ��� FALSE�� ���´�. �������� �ƴ���
//	üũ�� ������Ѵ�. GetLastError() == error_io_pending ���� �۾��ϰ� ������
//	��ٸ���� ��, �ε����̶�� ����ϸ�ȴ�. ���� �� ������ �ƴϸ� ������ ���̹Ƿ�
//	�ε� ����..
//	�ε� ���϶� �۾� �Ϸ�� ����� �˷��޶�...�񵿱� �۾��� ����� �� ���ִ� �Լ� GetOverlappedResult()
//	������ ��� �Լ��� �ƴϴ�. DWORD dwTrans ����� ������ �ɻ������� ����ϰ� �۾� ����
//	bWait TRUE �ϸ� �񵿱� �۾��� ������ ��ȯ��. 
//	bWail FALSE ��ٸ��� �ʰڴ�. 
//	��Ŷó�� �Ҷ� ������ ����� ������� �ƴ����� ��ŭ �о���. ��ŭ ���Ҵ�. 
//	���������� ���� ������, �� ���� �۾��� ������ hEvent�� �ñ׳εȴ�.
//	
//	����Ҷ��� OVERLAPPED���� �߰������� FILE_FLAG_WRITE_THROUGH �񵿱� ����� �ȵǼ�
//	�ϳ� �� �߰���
//	���ν����尡 �߰��߰� Ȯ���ϰ� �ִ� �ٵ� �ǹ̰� ���� WHILE(BPADING) �̺�Ʈ�� Ȯ�ΰ���
//
//	��ŭ �о���, ��ŭ ���ϸ� Ȯ���Ҽ�����. ����ٸ� ǥ���ؾ��ϴµ�,
//	15�ʰ� �Ѿ�� �ɸ������� ���Ե�, MS���� 15�ʰ� �Ѿ �Ǵµ� ȭ���� �������·� 15�ʴ� �ȵ�
//	�񵿱⸦ ����Ѵٴ� ���̴�. 
//	*/
//
//	//�ѱ� window 
//	setlocale(LC_ALL, ""); // Korean�ص� ������ ������ �ý��� ������������ ������ �Ѵ�.
//	HANDLE hFile = CreateFile(L"main.jpg", GENERIC_WRITE | GENERIC_READ
//		, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
//	//CREATE_ALWAYS : ���� �������
//	if (hFile != NULL)
//	{
//		WCHAR buffer[] = L"�����ڵ�� �̱۹���Ʈ�� �ؼ��Ҷ��� �߿��ϴ�.";
//		DWORD dLength = sizeof(buffer);
//		DWORD dWritten;
//		bool bRet = WriteFile(hFile, buffer, dLength, &dWritten, NULL);
//
//		if (bRet == true)
//		{
//			std::wcout << L"��¼���" << std::endl;
//		}
//	}
//	CloseHandle(hFile);
//
//#pragma endregion
//}