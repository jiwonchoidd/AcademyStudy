//새로운 작업의 추가, 변경이 쉽게 작업 대상과 작업 항목을 각각 별도의 
//클래스 상속 구조로 분리해서 정의하고 Double - dispatch방식에 따라 작업 수행이
//일어나게 클래스를 설계한 것을 비지터 패턴이라 한다.
//비지터 패턴(visitor pattern; 방문자 패턴)은 알고리즘을 객체 구조에서 분리시키는 디자인 패턴이다.
//이렇게 분리를 하면 구조를 수정하지 않고도 실질적으로 새로운 동작을 기존의 객체 구조에 
//추가할 수 있게 된다.개방 - 폐쇄 원칙을 적용하는 방법의 하나이다.
#include "reporter.h"

void DoReport(list<Account *>& accountList, Reporter& rpt) 
{
	list<Account *>::iterator iter;
	for (iter = accountList.begin(); iter != accountList.end(); iter++)
		(*iter)->Accept(rpt);
}

void main()
{
	PassbookAccount a("영희"), b("철수");
	CheckingAccount c("아빠"), d("엄마");

	list<Account *> accountList;
	accountList.push_front(&a);
	accountList.push_front(&b);
	accountList.push_front(&c);
	accountList.push_front(&d);

	TotalSumReporter sumRpt;
	HistoryReporter  histRpt;
	// 예금처리
	a.Deposit(1000);
	b.Deposit(2000);
	c.Deposit(3000);
	d.Deposit(4000);

	DoReport(accountList, sumRpt);
	cout << endl;

	// 출금처리
	a.Withdraw(500);
	c.Withdraw(1000);

	DoReport(accountList, histRpt);
	cout << endl;
	DoReport(accountList, sumRpt);
}
