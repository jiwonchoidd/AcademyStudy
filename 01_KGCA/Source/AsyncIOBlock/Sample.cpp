/*10mb �а� 10mb ���, �ٽ� 10mb �а��ؾ���
�� �۾��� �ٽ� 10mb ���, 
�װ� �񵿱�� ������ ������ �����ʴ´�.
�д� ���۰� 10���� ���ư��� offset 0 10 20 30
���� ���۰� �˾Ƽ� 10�� �Ǹ� �о��

�б� ������ ���� ������ ���� �Ѹ�

�������� ���� ���� ������ ����, 
���� ���۸� ���������� ���� ���

��ϴ����� �Ѵ�. 2048 512 ���
�񵿱� ��� Offset�� �� ����ϴ��� �ƴ°� �߿�
*/
#define BlockSizes 2048
#include <windows.h>
#include <iostream>

void main()
{
	setlocale(LC_ALL, "");
	WCHAR* pwbuffer = NULL;
	LARGE_INTEGER fileSize;
	OVERLAPPED ol_Read = { 0, };
	OVERLAPPED ol_Write = { 0, };

	//�ڵ� ��ȯ FILE_FLAG_OVERLAPPED���� �񵿱� ���� ����� 
	//������ �ִ� ������ �о��
	HANDLE hFile = CreateFile(L"test.JPG", GENERIC_READ | GENERIC_WRITE, 0,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	//�����ϸ� �ʱⰪ�� ����
	if (hFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		return;
	}
	// �ڵ� ��ȯ FILE_FLAG_OVERLAPPED���� �񵿱� ���� �����
	// �����ϴ� �ڵ�
	HANDLE hFileCopy = CreateFile(L"test_copy.JPG", GENERIC_WRITE, 0, 
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	//�����ϸ� �ʱⰪ�� ����
	if (hFileCopy == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFileCopy);
		return;
	}

	//�о�� ���� ����� üũ
	GetFileSizeEx(hFile, &fileSize);

	DWORD dwRead;
	DWORD dwWritten;
	DWORD dwTotalWrite = 0;

	//sector�� ũ�� ��� �Ϲ������� 512
	pwbuffer = new WCHAR[BlockSizes];
	int i = 0;
	while(1)
	{
		//Temp ����
		ZeroMemory(pwbuffer, BlockSizes);
		
		//�񵿱� �ε�
		//Overlapped ����ü ������
		ol_Read.Offset = i * BlockSizes;
		ol_Read.OffsetHigh = 0;

		BOOL ret = ReadFile(hFile, pwbuffer, BlockSizes, &dwRead, &ol_Read);
		BOOL bPending = FALSE;
		if (ret == FALSE)
		{
			DWORD dwError = GetLastError();
			if (dwError == ERROR_IO_PENDING)
			{
				std::wcout << L"ReadFile";
			}
			else
			{

			}
		}
		BOOL bReturn = GetOverlappedResult(hFile, &ol_Read, &dwRead, FALSE);
		if (bReturn == TRUE)
		{
			std::wcout << L"�ε� �Ϸ�";
			bPending = FALSE;
		}
		else
		{
			DWORD dwError = GetLastError();
			if (dwError == ERROR_IO_INCOMPLETE)
			{
				std::wcout << L".";
			}
			else
			{
				std::wcout << L"False";
				bPending = FALSE;
			}
		}
		//�񵿱� ���
		//Overlapped ����ü ������
		ol_Write.Offset = i * BlockSizes;
		ol_Write.OffsetHigh = 0;

		ret = WriteFile(hFileCopy, pwbuffer, dwRead, &dwWritten, &ol_Write);
		if (ret == FALSE)
		{
			DWORD dwError = GetLastError();
			if (dwError == ERROR_IO_PENDING)
			{
				std::wcout << L"��� ��";
			}
			else
			{

			}
		}
		bReturn = GetOverlappedResult(hFileCopy, &ol_Write, &dwWritten, FALSE);
		if (bReturn == TRUE)
		{
			std::wcout << L"�ε� �Ϸ�";
			bPending = FALSE;
		}
		else
		{
			DWORD dwError = GetLastError();
			if (dwError == ERROR_IO_INCOMPLETE)
			{
				std::wcout << L".";
			}
			else
			{
				std::wcout << L"False";
				bPending = FALSE;
			}
		}
		dwTotalWrite += dwWritten;

		if (fileSize.LowPart == dwTotalWrite)
		{
			break;
		}
		i++;
	}
	CloseHandle(hFile);
	CloseHandle(hFileCopy);
}
