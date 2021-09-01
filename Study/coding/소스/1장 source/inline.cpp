/* 
    inline.cpp
*/

#include <iostream>
inline int SQUARE(int x)
{
	return x*x;
}

int main(void)
{
	std::cout<<SQUARE(5)<<std::endl;
	return 0;
}
