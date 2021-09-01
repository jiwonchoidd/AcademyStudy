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
		cout<<name<<" 按眉 积己"<<endl;
	}
	~AAA(){
		cout<<name<<" 按眉 家戈"<<endl;
	}
};

int main(void)
{
	AAA aaa("aaa Obj");
	cout<<"--------烙矫 按眉 积己 傈---------"<<endl;
	AAA("Temp Obj");
	cout<<"--------烙矫 按眉 积己 饶---------"<<endl;
	return 0;
}