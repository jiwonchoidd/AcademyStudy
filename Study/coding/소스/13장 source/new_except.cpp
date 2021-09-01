/*
   new_except.cpp   
*/
#include <iostream>
#include <new>

using std::bad_alloc;
using std::endl;
using std::cout;

int main(void)
{
	try{
		int i=0;
		while(1){
			cout<<i++<<"번째 할당"<<endl;
			double(*arr)[10000]=new double[10000][10000];
		}

	}
	catch(bad_alloc ex){
		ex.what();
		cout<<endl<<"END"<<endl;
	}

	return 0;
}
