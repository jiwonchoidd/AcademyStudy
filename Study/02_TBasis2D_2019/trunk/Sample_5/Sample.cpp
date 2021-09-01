// compile with: /W3
// This program creates a file (or overwrites one if
// it exists), in text mode using Unicode encoding.
// It then writes two strings into the file
// and then closes the file.

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <wchar.h>
#include <tchar.h>

#define BUFFER_SIZE 50

int _tmain(int argc, _TCHAR* argv[])
{
	wchar_t str[BUFFER_SIZE];
	size_t  strSize;
	FILE*   fileHandle;
	int iRet = _wfopen_s(&fileHandle, L"_wfopen_test.xml", L"wt+,ccs=UNICODE"); 																	 
	if (fileHandle == NULL)
	{
		wprintf(L"_wfopen failed!\n");
		return(0);
	}

	// Write a string into the file.
	wcscpy_s(str, sizeof(str) / sizeof(wchar_t), L"<xmlTag>\n");
	strSize = wcslen(str);
	if (fwrite(str, sizeof(wchar_t), strSize, fileHandle) != strSize)
	{
		wprintf(L"fwrite failed!\n");
	}

	// Write a string into the file.
	wcscpy_s(str, sizeof(str) / sizeof(wchar_t), L"</xmlTag>");
	strSize = wcslen(str);
	if (fwrite(str, sizeof(wchar_t), strSize, fileHandle) != strSize)
	{
		wprintf(L"fwrite failed!\n");
	}

	// Close the file.
	if (fclose(fileHandle))
	{
		wprintf(L"fclose failed!\n");
	}
	return 0;
}
