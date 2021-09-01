#include "TDepartment.h"

int main()
{
	//직원을 관리하는 CONTROL 클래스
	TDepartment department;
	//직원 등록.
	department.AddEmployee(new TPermanent("KIM", 1000));
	department.AddEmployee(new TPermanent("LEE", 1500));
	department.AddEmployee(new TTemporary("HAN", 10, 200));
	department.AddEmployee(new TTemporary("JANG", 12, 300));
	//최종적으로 이번달에 지불해야할 급여는?
	department.ShowList();
	department.Release();
	return 0;
}