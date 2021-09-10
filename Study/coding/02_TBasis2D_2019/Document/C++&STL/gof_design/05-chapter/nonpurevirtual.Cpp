#include <iostream>
#include <fstream>
#include <string>			// added in VC++
#include <map>
using namespace std;

#define UNDEF_DOCUMENT        0
#define HWP_DOCUMENT          1
#define MSWORD_DOCUMENT       2
#define ANOTHER_DOCUMENT      3

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

class AnotherDocument : public Document {
  public :
    bool Open(char* pFileName) {
      ifstream ifs(pFileName);
      if (!ifs)
        return false;
      // -- 알맞은 프로세싱
      return true;
    }
};

class Application {
  public :
    void NewDocument(char* pFileName) {
      Document *pDoc = CreateDocument(GetDocType(pFileName));
	  if (pDoc == NULL) exit(0);
      docs_[pFileName] = pDoc;
      pDoc->Open(pFileName);
    }

    virtual Document* CreateDocument(int docType) {
      switch (docType) {
        case HWP_DOCUMENT : 
          return new HwpDocument;
        case MSWORD_DOCUMENT : 
          return new MsWordDocument;
      }
      return 0;
    }

  protected : 
    int GetDocType(char *pFileName) {
      char *pExt = strrchr(pFileName, '.');
      if (strcmp(pExt, ".hwp") == 0)
        return HWP_DOCUMENT;
      else if (strcmp(pExt, ".doc") == 0)
        return MSWORD_DOCUMENT;
      else
        return UNDEF_DOCUMENT;
    }

  private :
    map<string, Document *> docs_;
};

class HwpApplication : public Application { };

class MsWordApplication : public Application { };

class AnotherApplication : public Application {
  public :
    Document* CreateDocument(int docType) {
      if (docType == ANOTHER_DOCUMENT)
        return new AnotherDocument;
      else // -- 자신이 생성할 수 없는 객체는 상위 클래스가 생성토록 위임
        return Application::CreateDocument(docType);
    }
};

void		
main()
{
  AnotherApplication another;
  another.NewDocument("input.dat");
}
