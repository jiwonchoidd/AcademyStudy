/*
	AccessControl.cpp
*/
#include <iostream>
using std::cout;
using std::endl;

const int OPEN=1;
const int CLOSE=2;

class Door{	
private:
	int state;

public:
	void Open(){
		state=OPEN;
	}
	void Close(){
		state=CLOSE;
	}
	void ShowState(){
	    cout<<"���� ���� ���� : ";
		cout<<((state==OPEN)? "OPEN" : "CLOSE")<<endl;	
	}
};

int main()
{
	Door d;
	//d.state=OPEN;
	
	d.Open();
	d.ShowState();

	return 0;
}
