#include <iostream>
#include <string>
#include <map>
#include <list>
#include <iterator>
using namespace std;

class SearchCond {
  public :
    SearchCond(map<string, string> nvList) {
      condList_ = nvList;
    }

    bool CheckCond() {
      // -- name, value 쌍들의 리스트에 대해 검색 조건을 검사한다.
      // 예를 들어, age에 대해 숫자가 아닌 값이 설정되면 안되며, 
      // 음수값등이 설정되면 안 된다. 
      return true;
    }
  private :
    map<string, string> condList_;
};

class ListData {
  // -- 데이터베이스에서 검색되는 결과 레코드가 저장되기 위한 자료구조
};

class ListDBResult {
  public :
    int GetCount() {
      return result_.size();
    }

    void InitIterator() {
      iter_ = result_.begin();
    }

    ListData GetNextData() {
      return *iter_++;
    }
  private:
    list<ListData> result_;
    list<ListData>::iterator iter_;
};

class Database {
  public :
    Database() {
      // -- 데이터베이스 관리 시스템과 연결을 수행함.
    }

    bool Execute(string sql, ListDBResult& result) {
      // -- sql 수행 결과를 result에 설정하되, sql 수행 과정에서 
      // 문제가 있으면 false를 return하며, 그렇지 않은 경우에는 
      // true 를 return한다.
      return true;
    }
};

class SQLGenerator {
  public :
    string GenerateSQL(SearchCond cond) {
      string sql;

      // -- 검색 조건에 따라 sql 문장을 생성
      return sql;
    }
};


class DBHandler {
  public :
    bool Search(map<string, string> nvList, ListDBResult& result) {
      SearchCond cond(nvList);
      if (! cond.CheckCond()) {
        cout << "잘못된 검색조건" << endl;
        return false;
      }

      SQLGenerator generator;
      string sql = generator.GenerateSQL(cond);

      return db_.Execute(sql, result);
    }
  private :
    Database db_;
};

void
main()
{
  map<string, string> nvList;
  ListDBResult result;

  DBHandler handler;
  handler.Search(nvList, result);
}
