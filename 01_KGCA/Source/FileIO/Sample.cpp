#include "Sample.h"
#include <iostream>
int main()
{
	Sample s;
	//파일의 존재 유무
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