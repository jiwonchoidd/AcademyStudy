#define _CRT_SECURE_NO_WARNINGS
#include "KODBC.h"
//SQLHANDLE = void* 임 이름만 다름

void main()
{
	setlocale(LC_ALL, "");

	KODBC	odbc;

	odbc.Init();

	if (odbc.Connect(L"\\..\\..\\data\\databases\\User_DB.dsn"))
	{
		odbc.Execute_TableSet(L"user_table");
	}

	odbc.Release();
}
