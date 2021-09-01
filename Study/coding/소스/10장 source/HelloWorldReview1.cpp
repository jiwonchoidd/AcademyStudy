/*
   HelloWorldReview1.cpp
*/
#include<stdio.h>

namespace mystd  //mystd라는 이름공간 시작.
{   
	char* endl="\n";
	class ostream // 클래스 ostream 정의
	{
	public:
		void operator<<(char * str) {
			printf("%s", str);
		}
		void operator<<(int i) {
			printf("%d", i);
		}	
		void operator<<(double i) {
			printf("%e", i);
		}	
	};

	ostream cout;  //ostream 객체 생성
} // mystd 이름공간 끝.

using mystd::cout;
using mystd::endl;

int main()
{
	cout<<"Hello World \n";
	cout<<3.14;
	cout<<endl;
	cout<<1;
	cout<<endl;
	
	return 0;
}

