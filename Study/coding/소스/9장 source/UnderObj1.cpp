/*
   UnderObj1.cpp   
*/
#include <iostream>
using std::endl;
using std::cout;

class Data
{
	int data;
public:
	Data(int num){
		data=num;
	}
	void ShowData(){
		cout<<"Data: "<<data<<endl;
	}
	void Add(int num){
		data+=num;
	}
};

int main(void)
{
	Data ddd(10);
	ddd.Add(10);
	ddd.ShowData();

	return 0;
}