/* 
 using2.cpp
*/
	
#include <iostream>

namespace A_COM
{
	void function(void)
	{
		std::cout<<"A.com에서 정의한 함수"<<std::endl;
	}
}

using namespace A_COM;

int main(void)
{
	function();
	return 0;
}
