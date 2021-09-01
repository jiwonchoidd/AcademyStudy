/*
   DataTemplate1.cpp
*/
#include <iostream>
using std::endl;
using std::cout;

class Data
{
	int data;
public:
	Data(int d){
		data=d;
	}
	void SetData(int d){
		data=d;
	}
	int GetData(){
		return data;
	}
};

int main(void)
{
	Data d1(0);
	d1.SetData(10);
	Data d2(100);

	cout<<d1.GetData()<<endl;
	cout<<d2.GetData()<<endl;

	return 0;
}
