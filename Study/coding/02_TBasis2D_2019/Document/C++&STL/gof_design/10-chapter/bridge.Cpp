#include <iostream>
#include <string>
using namespace std;

class BTree { };

class SearchEngineImp {
  public :
    virtual bool Search(string s, string idxFn) = 0;
    virtual bool Search(string s, BTree& bTree) = 0;
};

class UnixSearchEngineImp : public SearchEngineImp {
  public :
    bool Search(string s, string idxFn) {
      // -- Unix 환경에 맞추어 idxFn 에서 문자열 검색
      return true;
    }
    bool Search(string s, BTree& bTree) {
      // -- Unix 환경에 맞추어 BTree 에서 문자열 검색
      return true;
    }
};

class WindowsSearchEngineImp : public SearchEngineImp {
  public :
    bool Search(string s, string idxFn) {
      // -- MS Windows 환경에 맞추어 idxFn 에서 문자열 검색
      return true;
    }
    bool Search(string s, BTree& bTree) {
      // -- MS Windows 환경에 맞추어 BTree 에서 문자열 검색
      return true;
    }
};

class SearchEngine {
  public :
    SearchEngine() { pImp_ = 0; }
    virtual bool Search(string s) = 0;

  protected :
    SearchEngineImp* GetSearchEngineImp() {
      if (pImp_ == 0) {
#ifdef  __WIN32__
        pImp_ = new WindowsSearchEngineImp;
#else
        pImp_ = new UnixSearchEngineImp;
#endif
      }
      return pImp_;
    }

  private :
    SearchEngineImp* pImp_;
};

class WebSearchEngine : public SearchEngine {
  public :
    WebSearchEngine(string idxFn) { indexFn_ = idxFn; }
    bool Search(string s) {
      return GetSearchEngineImp()->Search(s, indexFn_);
    }

  private :
    string indexFn_;
};

class DBSearchEngine : public SearchEngine {
  public : 
    bool Search(string s) {
      return GetSearchEngineImp()->Search(s, bTree_);
    }

  private :
    BTree bTree_;
};

void 
main() 
{
  WebSearchEngine finder("inverted_file4web.idx");
  finder.Search("디자인 패턴");
}
