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
			throw 10;  // int�� ���� ����.
		else
			throw 'm';  // char�� ���� ����. 
	}
	catch(int exp){
		cout<<"int�� ���� �߻�"<<endl;
	}
	catch(char exp){
		cout<<"char�� ���� �߻�"<<endl;
	}
	return 0;
}