#include "Sample.h"
#include <iostream>
int main()
{
	Sample s;
	//������ ���� ����
	if (s.io.FileLoad())
	{

	}
	else
	{
		std::cout << ":::::::::::::::::No File:::::::::::::::::" << std::endl;
		std::cout << ":::::::::::::Create New File:::::::::::::" << std::endl;

		s.io.FileCreate();
		s.io.FileLoad();
	}
}