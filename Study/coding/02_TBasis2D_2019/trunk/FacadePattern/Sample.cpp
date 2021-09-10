//facade�� '�ǹ��� ���� ����(����)'�̶� ���̴�
//�� ���� Ŭ���̾�Ʈ Ŭ������ ����ý����� Ŭ���̾�Ʈ ���̿� facade��� ��ü�� �����������ν� ������ ���踦 ����(����ȭ)�� ���̴�.
//�� ��� ���谡 ���鿡 ������ facade ��ü�� ���ؼ��� �̷���� �� �ְ� �ܼ��� �������̽��� ����(�ܼ��� â�� ����)�ϴ� ���̴�.
//facade ������ �̿��ϸ� ����ý��� ���ο��� �۵��ϰ� �ִ� ���� Ŭ�������� ���質 ������ �ǽ����� �ʰ�
//facade���� �����ϴ� �ܼ�ȭ�� �ϳ��� �������̽��� ����ϹǷ�, 
//Ŭ���� ���� ���� ���谡 �پ��� ���⼺ ���� �������� ȿ���� �� �� �ִ�.

#include <iostream>
#include <string>
#include <map>
#include <list>
#include <iterator>
using namespace std;

class SearchCond 
{
public:
	SearchCond(map<string, string> nvList) 
	{
		condList_ = nvList;
	}	
	bool CheckCond() 
	{
		// -- name, value �ֵ��� ����Ʈ�� ���� �˻� ������ �˻��Ѵ�.
		// ���� ���, age�� ���� ���ڰ� �ƴ� ���� �����Ǹ� �ȵǸ�, 
		// ���������� �����Ǹ� �� �ȴ�. 
		return true;
	}
private:
	map<string, string> condList_;
};

class ListData 
{
	// -- �����ͺ��̽����� �˻��Ǵ� ��� ���ڵ尡 ����Ǳ� ���� �ڷᱸ��
};

class ListDBResult 
{
public:
	int GetCount() 
	{
		return result_.size();
	}
	void InitIterator() 
	{
		iter_ = result_.begin();
	}
	ListData GetNextData() 
	{
		return *iter_++;
	}
private:
	list<ListData> result_;
	list<ListData>::iterator iter_;
};

class Database 
{
public:
	Database() 
	{
		// -- �����ͺ��̽� ���� �ý��۰� ������ ������.
	}

	bool Execute(string sql, ListDBResult& result) 
	{
		// -- sql ���� ����� result�� �����ϵ�, sql ���� �������� 
		// ������ ������ false�� return�ϸ�, �׷��� ���� ��쿡�� 
		// true �� return�Ѵ�.
		return true;
	}
};

class SQLGenerator 
{

public:
	string GenerateSQL(SearchCond cond) 
	{
		string sql;
		// -- �˻� ���ǿ� ���� sql ������ ����
		return sql;
	}
};


class DBHandler 
{
public:
	bool Search(map<string, string> nvList, ListDBResult& result) 
	{
		SearchCond cond(nvList);
		if (!cond.CheckCond()) 
		{
			cout << "�߸��� �˻�����" << endl;
			return false;
		}
		SQLGenerator generator;
		string sql = generator.GenerateSQL(cond);
		return db_.Execute(sql, result);
	}
private:
	Database db_;
};

void main()
{
	map<string, string> nvList;
	ListDBResult result;

	DBHandler handler;
	handler.Search(nvList, result);
}