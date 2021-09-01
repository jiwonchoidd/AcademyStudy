/*
   HelloWorldReview2.cpp
*/
#include<stdio.h>

namespace mystd  //mystd라는 이름공간 시작.
{   
	char* endl="\n";
	class ostream // 클래스 ostream 정의
	{
	public:
		ostream& operator<<(char * str) {
			printf("%s", str);
			return *this;
		}
		ostream& operator<<(int i) {
			printf("%d", i);
			return *this;
		}	
		ostream& operator<<(double i) {
			printf("%e", i);
			return *this;
		}	
	};

	ostream cout;  //ostream 객체 생성
} // mystd 이름공간 끝.

using mystd::cout;
using mystd::endl;

int main()
{
	cout<<"Hello World"<<endl<<3.14<<endl;
	return 0;
}
