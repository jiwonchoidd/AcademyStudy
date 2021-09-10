#include <iostream>
#include <string>
using namespace std;

class SearchEngine {
  public :
    bool Search(string s);
};

bool
SearchEngine::Search(string s) {

#ifdef __WIN32__
  // --- MS Windows 환경에 알맞은 소스 코드
#else 
  // --- Unix 환경에 알맞은 소스 코드
#endif

}
