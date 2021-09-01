/*
	reffunc.cpp
*/
#include <iostream>
using std::cout;
using std::endl;
using std::cin;

struct _Person {
	int age;		// ���� 
	char name[20];	// �̸� 
	char personalID[20]; // �ֹε�� ��ȣ.
};

typedef struct _Person Person;

void ShowData(const Person &p)
{
	cout<<"******* ���� ���� ��� *******"<<endl;
	cout<<"��    �� : "<<p.name<<endl;
	cout<<"�ֹι�ȣ : "<<p.personalID<<endl;
	cout<<"��    �� : "<<p.age<<endl;
}

int main(void)
{
	Person man;

	cout<<"��    �� : ";
	cin>>man.name;

	cout<<"��    �� : ";
	cin>>man.age;

	cout<<"�ֹι�ȣ : ";
	cin>>man.personalID;

	ShowData(man);
	return 0;
}