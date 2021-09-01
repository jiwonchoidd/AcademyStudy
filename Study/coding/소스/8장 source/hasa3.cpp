/*
   hasa3.cpp
*/
#include <iostream>
using std::endl;
using std::cout;

class Cudgel  //¸ùµÕÀÌ
{ 
public:
	void Swing(){ cout<<"Swing a cudgel!"<<endl; }
};


class Police //¸ùµÕÀÌ¸¦ ¼ÒÀ¯ÇÏ´Â °æÂû
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
