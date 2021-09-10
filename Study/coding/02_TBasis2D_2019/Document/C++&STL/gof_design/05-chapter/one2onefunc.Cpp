#include <iostream>
#include <fstream>
#include <string>			// added in VC++
#include <map>
using namespace std;

class HwpDocument {
  public :
    bool Open(char* pFileName) {
      ifstream ifs(pFileName);
      if (!ifs)
        return false;
      // -- HWP 고유 프로세싱
      return true;
    }
};

class MsWordDocument {
  public :
    bool Open(char* pFileName) {
      ifstream ifs(pFileName);
      if (!ifs)
        return false;
      // -- MS-Word 고유 프로세싱
      return true;
    }
};

class Application {
  public :
    virtual void NewDocument(char* pFileName) = 0;
};

class HwpApplication : public Application {
  public :
    void NewDocument(char* pFileName) {
      HwpDocument *pDoc = new HwpDocument;
      docs_[pFileName] = pDoc;
      pDoc->Open(pFileName);
    }

  private :
    map<string, HwpDocument *> docs_;
};

class MsWordApplication : public Application {
  public :
    void NewDocument(char* pFileName) {
      MsWordDocument *pDoc = new MsWordDocument;
      docs_[pFileName] = pDoc;
      pDoc->Open(pFileName);
    }

  private :
    map<string, MsWordDocument *> docs_;
};

void		
main()
{
  HwpApplication hwp;
  hwp.NewDocument("input.hwp");
}
