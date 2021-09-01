/*
   EmployeeManager3.cpp
*/
#include <iostream>
using std::endl;
using std::cout;

/***** Employee class **************/
class Employee
{
protected:
	char name[20];
public:
	Employee(char* _name);
	const char* GetName();
};
Employee::Employee(char* _name)
{
	strcpy(name, _name);
}	
const char* Employee::GetName()
{
	return name;
}	

/***** Permanent class **************/
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

/***** Temporary class **************/
class Temporary : public Employee
{
private:
	int time; //���ѽð�
	int pay; //�ð��� �޿�.
public:
	Temporary(char* _name, int _time, int _pay);
	int GetPay();
};
Temporary::Temporary(char* _name, int _time, int _pay)
: Employee(_name)
{
	time=_time;
	pay=_pay;
}
int Temporary::GetPay()
{
	return time*pay;
}

/***** Department class **************/
class Department
{
private:
	Employee* empList[10];
	int index;
public:
	Department(): index(0) { };
	void AddEmployee(Employee* emp= new Permanent("KIM", 1000));
	void AddEmployee(Employee* emp= new Temporary("JANG", 12, 300));
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

/***** main function **************/
int main()
{
	//������ �����ϴ� CONTROL Ŭ����
	Department department;

	//���� ���.
	department.AddEmployee(new Permanent("KIM", 1000));
	department.AddEmployee(new Permanent("LEE", 1500));
	department.AddEmployee(new Temporary("HAN", 10, 200));
	department.AddEmployee(new Temporary("JANG", 12, 300));

	//���������� �̹��޿� �����ؾ��� �޿���?
	department.ShowList();	
	return 0;
}