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

/***** Temporary class **************/
class Temporary : public Employee
{
private:
	int time; //일한시간
	int pay; //시간당 급여.
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

/***** main function **************/
int main()
{
	//직원을 관리하는 CONTROL 클래스
	Department department;

	//직원 등록.
	department.AddEmployee(new Permanent("KIM", 1000));
	department.AddEmployee(new Permanent("LEE", 1500));
	department.AddEmployee(new Temporary("HAN", 10, 200));
	department.AddEmployee(new Temporary("JANG", 12, 300));

	//최종적으로 이번달에 지불해야할 급여는?
	department.ShowList();	
	return 0;
}