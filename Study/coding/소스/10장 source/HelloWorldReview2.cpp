/*
   HelloWorldReview2.cpp
*/
#include<stdio.h>

namespace mystd  //mystd��� �̸����� ����.
{   
	char* endl="\n";
	class ostream // Ŭ���� ostream ����
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

	ostream cout;  //ostream ��ü ����
} // mystd �̸����� ��.

using mystd::cout;
using mystd::endl;

int main()
{
	cout<<"Hello World"<<endl<<3.14<<endl;
	return 0;
}
