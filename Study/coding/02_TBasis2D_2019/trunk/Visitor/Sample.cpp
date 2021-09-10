//���ο� �۾��� �߰�, ������ ���� �۾� ���� �۾� �׸��� ���� ������ 
//Ŭ���� ��� ������ �и��ؼ� �����ϰ� Double - dispatch��Ŀ� ���� �۾� ������
//�Ͼ�� Ŭ������ ������ ���� ������ �����̶� �Ѵ�.
//������ ����(visitor pattern; �湮�� ����)�� �˰����� ��ü �������� �и���Ű�� ������ �����̴�.
//�̷��� �и��� �ϸ� ������ �������� �ʰ� ���������� ���ο� ������ ������ ��ü ������ 
//�߰��� �� �ְ� �ȴ�.���� - ��� ��Ģ�� �����ϴ� ����� �ϳ��̴�.
#include "reporter.h"

void DoReport(list<Account *>& accountList, Reporter& rpt) 
{
	list<Account *>::iterator iter;
	for (iter = accountList.begin(); iter != accountList.end(); iter++)
		(*iter)->Accept(rpt);
}

void main()
{
	PassbookAccount a("����"), b("ö��");
	CheckingAccount c("�ƺ�"), d("����");

	list<Account *> accountList;
	accountList.push_front(&a);
	accountList.push_front(&b);
	accountList.push_front(&c);
	accountList.push_front(&d);

	TotalSumReporter sumRpt;
	HistoryReporter  histRpt;
	// ����ó��
	a.Deposit(1000);
	b.Deposit(2000);
	c.Deposit(3000);
	d.Deposit(4000);

	DoReport(accountList, sumRpt);
	cout << endl;

	// ���ó��
	a.Withdraw(500);
	c.Withdraw(1000);

	DoReport(accountList, histRpt);
	cout << endl;
	DoReport(accountList, sumRpt);
}
