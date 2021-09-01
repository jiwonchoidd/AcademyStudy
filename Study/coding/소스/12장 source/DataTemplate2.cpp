/*
   DataTemplate2.cpp
*/
#include <iostream>
using std::endl;
using std::cout;

template <typename T>
class Data
{
	T data;
public:
	Data(T d){
		data=d;
	}
	void SetData(T d){
		data=d;
	}
	T GetData(){
		return data;
	}
};

int main(void)
{
	Data<int> d1(0);
	d1.SetData(10);

	Data<char> d2('a');

	cout<<d1.GetData()<<endl;
	cout<<d2.GetData()<<endl;

	return 0;
}

