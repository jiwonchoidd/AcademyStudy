//facade는 '건물의 앞쪽 정면(전면)'이란 뜻이다
//몇 개의 클라이언트 클래스와 서브시스템의 클라이언트 사이에 facade라는 객체를 세워놓음으로써 복잡한 관계를 정리(구조화)한 것이다.
//즉 모든 관계가 전면에 세워진 facade 객체를 통해서만 이루어질 수 있게 단순한 인터페이스를 제공(단순한 창구 역할)하는 것이다.
//facade 패턴을 이용하면 서브시스템 내부에서 작동하고 있는 많은 클래스들의 관계나 사용법을 의식하지 않고
//facade에서 제공하는 단순화된 하나의 인터페이스만 사용하므로, 
//클래스 간의 의존 관계가 줄어들고 복잡성 또한 낮아지는 효과를 볼 수 있다.

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
		// -- name, value 쌍들의 리스트에 대해 검색 조건을 검사한다.
		// 예를 들어, age에 대해 숫자가 아닌 값이 설정되면 안되며, 
		// 음수값등이 설정되면 안 된다. 
		return true;
	}
private:
	map<string, string> condList_;
};

class ListData 
{
	// -- 데이터베이스에서 검색되는 결과 레코드가 저장되기 위한 자료구조
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
		// -- 데이터베이스 관리 시스템과 연결을 수행함.
	}

	bool Execute(string sql, ListDBResult& result) 
	{
		// -- sql 수행 결과를 result에 설정하되, sql 수행 과정에서 
		// 문제가 있으면 false를 return하며, 그렇지 않은 경우에는 
		// true 를 return한다.
		return true;
	}
};

class SQLGenerator 
{

public:
	string GenerateSQL(SearchCond cond) 
	{
		string sql;
		// -- 검색 조건에 따라 sql 문장을 생성
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
			cout << "잘못된 검색조건" << endl;
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
