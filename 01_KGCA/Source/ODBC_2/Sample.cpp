#define _CRT_SECURE_NO_WARNINGS
#include "KODBC.h"
//SQLHANDLE = void* 임 이름만 다름

void main()
{
	setlocale(LC_ALL, "");

	KODBC	odbc;

	odbc.Init();

	SQLWCHAR dir[MAX_PATH] = { 0, };
	GetCurrentDirectory(MAX_PATH, dir);

	std::wstring dbpath = dir;
	dbpath += L"\\GameDB.dsn";
	if (odbc.Connect(4, dbpath.c_str()))
	{
		odbc.Execute_TableSet(L"usertbl");
	}

	odbc.Release();
}
