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

  protected :
    virtual Document* CreateDocument() = 0;

  private :
    map<string, Document *> docs_;
};

template <class DocType>
class ConcreteApplication : public Application {
  protected :
    Document* CreateDocument() {
      return new DocType;
    }
};

void
main()
{
  ConcreteApplication<HwpDocument> hwp;
  hwp.NewDocument("input.hwp");
}
