/*
   HelloWorldReview1.cpp
*/
#include<stdio.h>

namespace mystd  //mystd��� �̸����� ����.
{   
	char* endl="\n";
	class ostream // Ŭ���� ostream ����
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

	ostream cout;  //ostream ��ü ����
} // mystd �̸����� ��.

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

