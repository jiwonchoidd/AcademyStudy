/*
   re_throw.cpp
*/
#include <iostream>
using std::cout;
using std::endl;

class Exception 
{
public:
	void what(){ 
		cout<<"Simple Exception"<<endl;
	}
};

void ThrowException() 
{	
	try{
	    throw Exception();	
	}
	catch(Exception& t){
	    t.what();
	    throw;
	}
}

int main()
{	
	try{
	    ThrowException();
	}
	catch(Exception& t){
		t.what();
	}

	return 0;
}
