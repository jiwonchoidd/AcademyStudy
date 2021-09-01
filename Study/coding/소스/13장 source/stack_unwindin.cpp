/*
   stack_unwindin.cpp   
*/
#include <iostream>
using std::endl;
using std::cout;
using std::cin;

void fct1();
void fct2();
void fct3();

int main(void)
{
	try{
		fct1();
	}
	catch(int ex){
		cout<<"¿¹¿Ü: "<<ex<<endl;
	}

	return 0;
}

void fct1(){ 
	fct2();
}
void fct2(){
	fct3();
}
void fct3(){
	throw 100;
}