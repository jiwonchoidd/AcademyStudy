/*
   hasa1.cpp
*/
#include <iostream>
using std::endl;
using std::cout;

class Cudgel  //������
{ 
public:
	void Swing(){ cout<<"Swing a cudgel!"<<endl; }
};

class Police : public Cudgel  //�����̸� �����ϴ� ����
{
public:
	void UseWeapon(){ Swing(); }
};

int main()
{
	Police pol;
	pol.UseWeapon();

	return 0;
}
