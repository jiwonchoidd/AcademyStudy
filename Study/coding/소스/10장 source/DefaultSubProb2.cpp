/*
   DefaultSubProb2.cpp   
*/
#include <iostream>
using std::endl;
using std::cout;
using std::ostream;

class Person {
private:
	char* name;
public:
	Person(char* _name);
	Person(const Person& p);
	~Person();
	Person& operator=(const Person& p);
	friend ostream& operator<<(ostream& os, const Person& p);
};

Person::Person(char* _name){
	name= new char[strlen(_name)+1];
	strcpy(name, _name);
}
Person::Person(const Person& p){
	name= new char[strlen(p.name)+1];
	strcpy(name, p.name);
}
Person::~Person(){
	delete[] name;
}
Person& Person::operator=(const Person& p){
	delete []name;
	name= new char[strlen(p.name)+1];
	strcpy(name, p.name);
	return *this;
}

ostream& operator<<(ostream& os, const Person& p)
{
	os<<p.name;
	return os;
}

int main()
{
	Person p1("LEE JUNE");
	Person p2("HONG KEN");	

	cout<<p1<<endl;
	cout<<p2<<endl;

	p1=p2;  // 문제의 원인.

	cout<<p1<<endl;
	return 0;
}
