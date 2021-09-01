/*
   UnderObj2.cpp   
*/
#include <iostream>
using std::endl;
using std::cout;

/****** Ŭ���� Data�� �䳻�� ���� *********/
struct Data
{
	int data;
	void (*ShowData)(Data*);
	void (*Add)(Data*, int);
};

void ShowData(Data* THIS){
	cout<<"Data: "<<THIS->data<<endl;
}
void Add(Data* THIS, int num){
	THIS->data+=num;
}


/***** ������ ����� main �Լ� *********/
int main(void)
{
	//Data ddd1(10);
	Data ddd1={10, ShowData, Add};

	//ddd1.Add(10);
	ddd1.Add(&ddd1, 10);

	//ddd1.ShowData();
	ddd1.ShowData(&ddd1);

	Data ddd2={1, ShowData, Add};
	ddd2.Add(&ddd2, 1);
	ddd2.ShowData(&ddd2);

	return 0;
};
