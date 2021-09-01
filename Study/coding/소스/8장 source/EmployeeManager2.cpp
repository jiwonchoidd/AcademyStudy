/*
   EmployeeManager2.cpp
*/
#include <iostream>
using std::endl;
using std::cout;

class Employee
{
protected:
	char name[20];
public:
	Employee(char* _name){
		strcpy(name, _name);
	}	
	const char* GetName();
};
const char* Employee::GetName()
{
	return name;
}	

class Permanent : public Employee
{
private:
	int salary;  // 기본급여
public:
	Permanent(char* _name, int sal);
	int GetPay();
};

Permanent::Permanent(char* _name, int sal)
: Employee(_name)
{
	salary=sal;
}

int Permanent::GetPay()
{
	return salary;
}

class Department
{
private:
	Employee* empList[10];
	int index;
public:
	Department(): index(0) { };
	void AddEmployee(Employee* emp);
	void ShowList(); // 급여 리스트 출력.
};

void Department::AddEmployee(Employee* emp)
{
	empList[index++]=emp;
}
void Department::ShowList() // 급여 리스트 출력.
{
	for(int i=0; i<index; i++)
	{
		cout<<"name: "<<empList[i]->GetName()<<endl;
		//cout<<"salary: "<<empList[i]->GetPay()<<endl;
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