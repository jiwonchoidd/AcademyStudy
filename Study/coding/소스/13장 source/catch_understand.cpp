/*
   catch_understand.cpp   
*/
#include <iostream>
using std::endl;
using std::cout;
using std::cin;

int main(void)
{
	int num;

	cout<<"input number: ";
	cin>>num;

	try{
		if(num>0)
			throw 10;  // int형 예외 전달.
		else
			throw 'm';  // char형 예외 전달. 
	}
	catch(int exp){
		cout<<"int형 예외 발생"<<endl;
	}
	catch(char exp){
		cout<<"char형 예외 발생"<<endl;
	}
	return 0;
}