#include <iostream>
#include <stdlib.h>
#include <string.h>
#define SPA "  "

using namespace::std;

class tvalue{
	char* value;
	tvalue* next;
	friend class token;
public :
	tvalue(char*);
	~tvalue(){delete value;}
};

tvalue::tvalue(char* data)
{
	value = new char[strlen(data)+1];
	strcpy(value,data);
	next = 0;
}

class token{
	tvalue *toke;
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

	if((temp = strtok(x,SPA)) ==0){
		cout<<"error condition\n";
		size = 0;
		exit(1);
	}
	toke = new tvalue(temp);
	tvalue* cursor = toke;
	for(int i = 1;(temp = strtok(0,SPA));i++){
		cursor->next = new tvalue(temp);
		cursor = cursor->next;
	}
	//size = i;
}
token::~token()
{
	tvalue *prev;
	tvalue *cursor = toke;

	while(cursor != 0){
		prev = cursor;
		cursor = cursor->next;
		delete prev;
	}
}
char* token::nth_token(int num)
{
	tvalue* cursor = toke;

	for(int i = 1;i<num;i++)
		if(cursor != 0)
			cursor = cursor->next;
		else
			return 0;
	return cursor->value;
}
void token::show_all()
{
	tvalue* cursor = toke;
	for(int i = 0;i<size;i++){
		cout <<"==>"<<cursor->value<<"\n";
		cursor = cursor->next;
	}
}
void main()
{
	token x("This is only a test line");

	x.show_all();
	cout<<"Show the 3rd token:"<<x.nth_token(3)<<"\n";
}