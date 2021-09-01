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
	int salary;  // �⺻�޿�
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
	void ShowList(); // �޿� ����Ʈ ���.
};

void Department::AddEmployee(Employee* emp)
{
	empList[index++]=emp;
}
void Department::ShowList() // �޿� ����Ʈ ���.
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
	//������ �����ϴ� CONTROL Ŭ����
	Department department;

	//���� ���.
	department.AddEmployee(new Permanent("KIM", 1000));
	department.AddEmployee(new Permanent("LEE", 1500));
	department.AddEmployee(new Permanent("JUN", 2000));

	//���������� �̹��޿� �����ؾ��� �޿���?
	department.ShowList();	
	return 0;
}