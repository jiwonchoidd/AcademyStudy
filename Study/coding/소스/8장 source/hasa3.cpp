/*
   hasa3.cpp
*/
#include <iostream>
using std::endl;
using std::cout;

class Cudgel  //������
{ 
public:
	void Swing(){ cout<<"Swing a cudgel!"<<endl; }
};


class Police //�����̸� �����ϴ� ����
{
	Cudgel* cud;
public:
	Police(){
		cud=new Cudgel;
	}
	~Police(){
		delete cud;
	}
	void UseWeapon(){ cud->Swing(); }
};

int main()
{
	Police pol;
	pol.UseWeapon();

	return 0;
}
