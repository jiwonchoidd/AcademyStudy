// TCoreDll.cpp : DLL ���� ���α׷��� ���� ������ �Լ��� �����մϴ�.
//

#include "stdafx.h"
#include "TCoreDll.h"


// ������ ������ �����Դϴ�.
TCOREDLL_API int nTCoreDll=0;

// ������ �Լ��� �����Դϴ�.
TCOREDLL_API int fnTCoreDll(void)
{
	return 42;
}

// ������ Ŭ������ �������Դϴ�.
// Ŭ���� ���Ǹ� ������ TCoreDll.h�� �����Ͻʽÿ�.
CTCoreDll::CTCoreDll()
{
	return;
}
