#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace::std;

class node{
	void* info;
	node* next;

public:
	node(void* v){info = v; next = 0;}
	void put_next(node* n){next = n;}
	node* get_next(){return next;}
	void* get_info(){return info;}
};
class list{
	node* head;
	int node_num;
public:
	list(){node_num =0;head = 0;}
	void remove(int);
	void insert(void*,int);
	void append(void*v){insert(v,node_num+1);}
	void* find(int);
	void display();
};

void list::remove(int pos)
{
	node *prev,*cursor = head;
	if(pos ==1){
		head = cursor->get_next();
		delete cursor;
	}
	else{
		for(int i =1;i<pos;i++){
			prev  = cursor;
			cursor = cursor->get_next();
		}
		prev->put_next(cursor->get_next());
		delete cursor;
		node_num--;
	}
}

void list::insert(void *val, int pos)
{
	node *prev, *cursor = head, *temp = new node(val);

	if(!head)head = temp;
	else if(pos ==0){
		temp->put_next(head);
		head = temp;
	}
	else{
		for(int i = 1;(i<pos)&&cursor->get_next();i++){
			prev = cursor->get_next();

		}
		if(!cursor->get_next())
			cursor->put_next(temp);
		else{
			temp->put_next(cursor);
			prev->put_next(temp);
		}
	}
	node_num++;
}
void* list::find(int pos)
{
	if(!head)return 0;
	node* cursor = head;

	for(int i =0;i<pos;i++)
		cursor = cursor->get_next();

	if(cursor)
		return cursor->get_info();
	return 0;
}
void list::display()
{
	void * temp;

	for(int i =0; ; i++){
		if(!(temp = find(i)))break;
		cout<<"*"<<(char*)temp<<"\n";
	}
}
class stack : private list{
	int top;

public :
	stack(){cout<<"Buliding a stack object...\n";top = -1;}
	void display();
	void push(double);
	double pop();
};

void stack::display()
{
	void* temp;

	for(int i =0; ; i++)
	{
		if(!(temp = find(i)))break;
	}
}
void stack::push(double x)
{
	double* ptr = new double;
	*ptr =x;

	top++;
	append((void*)ptr);
}

double stack::pop()
{
	void* temp = find(top);
	remove(top+1);
	top--;
	return *((double*)temp);
}
int main()
{
	stack x;
	cout <<"=============================\n";
	x.push(3.2);
	x.push(6.4);
	x.push(12.8);
	x.display();
	cout<<"==============================\n";
	cout<<"q = "<<x.pop()<<"\n";
	cout<<"==============================\n";
	x.display();
	cout<<"==============================\n";
	cout<< "q= "<<x.pop()<<"\n";
	cout<<"==============================\n";
}