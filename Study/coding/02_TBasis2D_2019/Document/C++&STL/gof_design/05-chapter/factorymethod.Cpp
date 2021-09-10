#include <iostream>
#include <fstream>
#include <string>		// added in VC++
#include <map>
using namespace std;

class Document {
  public :
    virtual bool Open(char* pFileName) = 0;
};

class HwpDocument : public Document {
  public :
    bool Open(char* pFileName) {
      ifstream ifs(pFileName);
      if (!ifs)
        return false;
      // -- HWP 고유 프로세싱
      return true;
    }
};

class MsWordDocument : public Document {
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
    void NewDocument(char* pFileName) {
      Document *pDoc = CreateDocument();
      docs_[pFileName] = pDoc;
      pDoc->Open(pFileName);
    }

    virtual Document* CreateDocument() = 0;

  private :
    map<string, Document *> docs_;
};

class HwpApplication : public Application {
  public :
    Document* CreateDocument() {
      return new HwpDocument;
    }
};

class MsWordApplication : public Application {
  public :
    Document* CreateDocument() {
      return new MsWordDocument;
    }
};

void		
main()
{
  HwpApplication hwp;
  hwp.NewDocument("input.hwp");
}
