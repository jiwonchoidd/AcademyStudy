/*
   EmployeeManager1.cpp
*/
#include <iostream>
using std::endl;
using std::cout;

class Permanent
{
private:
	char name[20];	
	int salary;
public:
	Permanent(char* _name, int sal);
	const char* GetName();
	int GetPay();
};

Permanent::Permanent(char* _name, int sal) {
	strcpy(name, _name);
	salary=sal;
}
const char* Permanent::GetName()
{
	return name;
}	
int Permanent::GetPay()
{
	return salary;
}

class Department
{
private:
	Permanent* empList[10];
	int index;
public:
	Department(): index(0) { };
	void AddEmployee(Permanent* emp);
	void ShowList(); // 급여 리스트 출력.
};

void Department::AddEmployee(Permanent* emp)
{
	empList[index++]=emp;
}
void Department::ShowList() // 급여 리스트 출력.
{
	for(int i=0; i<index; i++)
	{
		cout<<"name: "<<empList[i]->GetName()<<endl;
		cout<<"salary: "<<empList[i]->GetPay()<<endl;
		cout<<endl;
	}
}

int main()
{
	//직원을 관리하는 CONTROL 클래스
	Department department;

	//직원 등록.
	department.AddEmployee(new Permanent("KIM", 1000));
	department.AddEmployee(new Permanent("LEE", 1500));
	department.AddEmployee(new Permanent("JUN", 2000));

	//최종적으로 이번달에 지불해야할 급여는?
	department.ShowList();	
	return 0;
}