/*
   handling.cpp   
*/
#include <iostream>
using std::endl;
using std::cout;
using std::cin;

int main(void)
{
	int a, b;
	
	cout<<"�ΰ��� ���� �Է� : ";
	cin>>a>>b;
	if(b==0){
		cout<<"�Է¿���! �ٽ� ���� �ϼ���."<<endl;
	}
	else {
		cout<<"a/b�� �� : "<<a/b<<endl;
		cout<<"a/b�� ������ : "<<a%b<<endl;
	}
	
	return 0;
}