/*
   TempObj.cpp   
*/
#include <iostream>
using std::endl;
using std::cout;

class AAA{
	char name[20];
public:
	AAA(char* _name){
		strcpy(name, _name);
		cout<<name<<" ��ü ����"<<endl;
	}
	~AAA(){
		cout<<name<<" ��ü �Ҹ�"<<endl;
	}
};

int main(void)
{
	AAA aaa("aaa Obj");
	cout<<"--------�ӽ� ��ü ���� ��---------"<<endl;
	AAA("Temp Obj");
	cout<<"--------�ӽ� ��ü ���� ��---------"<<endl;
	return 0;
}