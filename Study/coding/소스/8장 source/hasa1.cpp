/*
   hasa1.cpp
*/
#include <iostream>
using std::endl;
using std::cout;

class Cudgel  //¸ùµÕÀÌ
{ 
public:
	void Swing(){ cout<<"Swing a cudgel!"<<endl; }
};

class Police : public Cudgel  //¸ùµÕÀÌ¸¦ ¼ÒÀ¯ÇÏ´Â °æÂû
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
