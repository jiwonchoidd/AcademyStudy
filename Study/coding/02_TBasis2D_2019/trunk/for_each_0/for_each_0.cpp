// for_each_0.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>


//#define mem_fun1 mem_fun
class Person {
private:
	std::string name;
public:
	//...
	void print() const {
		std::cout << name << std::endl;
	}
	void Show(int iData) const {
		std::cout << name << "[" << iData << "]"<<std::endl;
	}
	void printWithPrefix(std::string prefix) const {
		std::cout << prefix << name << std::endl;
	}
};

void foo(const std::vector<Person>& coll)
{
	using std::for_each;
	using std::bind2nd;
	using std::mem_fun_ref;
	// 일반 템플릿 객체의 멤버함수 호출(인자값 없는)
	for_each(coll.begin(), coll.end(),mem_fun_ref(&Person::print));
	// 일반 템플릿 객체의 멤버함수 호출(인자값 있는)
	for_each(coll.begin(), coll.end(),
		bind2nd(mem_fun_ref(&Person::printWithPrefix),	"person: "));
}
void ptrfoo(const std::vector<Person*>& coll)// ^^^ pointer !
{
	using std::for_each;
	using std::bind2nd;
	using std::mem_fun;
	// 포인터 템플릿 객체의 멤버 함수 호출(인자값 없는)
	for_each(coll.begin(), coll.end(),mem_fun(&Person::print));
	// 포인터 템플릿 객체의 멤버 함수 호출(인자값 있는)
	for_each(coll.begin(), coll.end(),
		bind2nd(mem_fun(&Person::printWithPrefix),	"person: "));
}
Person  SetData()
{
	Person ret;
	return ret;
}
void GlobalFun(Person& pData)
{
	pData.print();
}
int main()
{
	std::vector<Person> coll(5);
	// 전역함수 호출
	for_each(coll.begin(), coll.end(), GlobalFun);
	for_each(coll.begin(), coll.end(),
		std::bind2nd(std::mem_fun_ref(&Person::Show), 1));
	foo(coll);

	std::vector<Person*> coll2;
	coll2.push_back(new Person);
	ptrfoo(coll2);

	std::vector<Person> vecList;
	std::generate_n( std::back_inserter(vecList), 10, SetData);
	for_each(vecList.begin(), vecList.end(), 
		std::bind2nd(std::mem_fun_ref(&Person::Show), 100));

	//mem_fun(&A::F)(class, x)		==> class->F(x)
	//mem_fun_ref(&B::F)(class, x)	==> class.F(x)
    return 0;
}

