#include <iostream>
#include <stdlib.h>
#include <string.h>
#define SPA " "

using namespace::std;

class tvalue{
	char* vlaue;
	tvalue *next;
	public:
		tvalue(char*);
		~tvalue();
		void put_next(tvalue*);
		tvalue* get_next();
		char * show_value();
};
tvalue::tvalue(char* data)
{
	vlaue = new char[strlen(data)+1];
	strcpy(vlaue,data);
	next =	0;
}
tvalue::~tvalue()
{
	delete vlaue;
}
void tvalue::put_next(tvalue * tnode)
{
	next = tnode;
}
tvalue* tvalue::get_next()
{
	return next;
}
char* tvalue::show_value()
{
	return vlaue;
}
class token{
	tvalue* toke;
	int size;

	public:

		token(char*);
		~token();
		char* nth_token(int);
		void show_all();
};
token::token(char* x)
{
	char* temp;

	if((temp = strtok(x,SPA)) == 0){
		cout<<"error condition\n";
		size = 0;
		exit(1);
	}

	toke = new tvalue(temp);
	
	tvalue* cursor = toke;
	
	for(int i = 1;(temp = strtok(0,SPA));i++){
		cursor->put_next(new tvalue(temp));
		cursor = cursor->get_next();
	}
	//size = i;
}
token::~token()
{
	tvalue *prev;
	tvalue *cursor = toke;
	while(cursor != 0){
			prev = cursor;
			cursor = cursor->get_next();
			delete prev;
	}
}
char* token::nth_token(int num)
{
	tvalue * cursor = toke;

	for(int i = 1;i<num;i++)
		if(cursor != 0)
			cursor = cursor->get_next();
		else 
			return 0;
	return cursor->show_value();
}
void token::show_all()
{
	tvalue* cursor = toke;

	for(int i = 1;i<size;i++){
		cout<<"==>"<<cursor->show_value()<<"\n";
		cursor = cursor->get_next();
	}
}
void main()
{
	token x("This is only a test line");

	x.show_all();
	cout<<"Show the 3rd token:"<<x.nth_token(3)<<"\n";
}


