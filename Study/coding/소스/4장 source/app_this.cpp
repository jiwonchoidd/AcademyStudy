/*
   app_this.cpp
*/
#include <iostream>
using std::cout;
using std::endl;

class Data
{
	int aaa;
	int bbb;

public :
	Data(int aaa, int bbb) {
		//aaa=aaa;
		this->aaa=aaa;

		//bbb=bbb;
		this->bbb=bbb;
	}
	void printAll() {
	       cout<<aaa<<" "<<bbb<<endl;
	}
};

int main(void)
{
	Data d(100, 200);
	d.printAll();

	return 0;
}
