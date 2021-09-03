#include <iostream>
#include <strings.h>
#include <sys/utsname.h>
using namespace std;

#define HPUX "HPUX"
#define SUNOS "SunOS"

class Scanner {
  public : 
    virtual Scanner * Clone()=0;
};

class Parser {
  public : 
    virtual Parser * Clone()=0;
};

class CodeGenerator {
  public : 
    virtual CodeGenerator * Clone()=0;
};

class Optimizer {
  public : 
    virtual Optimizer * Clone()=0;
};

class HPScanner : public Scanner {
  public :
    Scanner * Clone() { return new HPScanner(*this); }
};

class HPParser : public Parser {
  public :
    Parser * Clone() { return new HPParser(*this); }
};

class HPCodeGenerator : public CodeGenerator {
  public :
    CodeGenerator * Clone() { return new HPCodeGenerator(*this); }
};

class HPOptimizer : public Optimizer {
  public :
    Optimizer * Clone() { return new HPOptimizer(*this); }
};

class SunScanner : public Scanner {
  public :
    Scanner * Clone() { return new SunScanner(*this); }
};

class SunParser : public Parser {
  public :
    Parser * Clone() { return new SunParser(*this); }
};

class SunCodeGenerator : public CodeGenerator {
  public :
    CodeGenerator * Clone() { return new SunCodeGenerator(*this); }
};

class SunOptimizer : public Optimizer {
  public :
    Optimizer * Clone() { return new SunOptimizer(*this); }
};

class CompilerFactory {  
  public :    
    CompilerFactory(Scanner* pScanner, Parser* pParser, 
                    CodeGenerator* pCodeGenerator, Optimizer* pOptimizer)
        : pScanner_(pScanner), pParser_(pParser), 
          pCodeGenerator_(pCodeGenerator), pOptimizer_(pOptimizer) {}

    Scanner* CreateScanner() { return pScanner_->Clone(); }
    Parser* CreateParser() { return pParser_->Clone(); }
    CodeGenerator* CreateCodeGenerator() { return pCodeGenerator_->Clone(); }
    Optimizer* CreateOptimizer() { return pOptimizer_->Clone(); }

  private :
    Scanner* pScanner_;
    Parser* pParser_;
    CodeGenerator* pCodeGenerator_;
    Optimizer* pOptimizer_;
};


int 
main() {  
  CompilerFactory *pFactory;
  struct utsname sysInfo;

  // -- OS 버전 및 하드웨어 타입 정보 얻기 위한 시스템 함수  
  if (uname(&sysInfo) < 0) { 
    cout << "Error Occurred" << endl;
    return(-1);
  }

  if (strncasecmp(sysInfo.sysname, HPUX, strlen(HPUX)) == 0) {  
    // -- HP 용 객체 생성 및 사용
    HPScanner scanner;
    HPParser parser;
    HPCodeGenerator codeGenerator;
    HPOptimizer optimizer;

    pFactory = new CompilerFactory(&scanner, 
                                    &parser, &codeGenerator, &optimizer);  
  }
  else if (strncasecmp(sysInfo.sysname, SUNOS, strlen(SUNOS)) == 0) {  
    // -- Sun 용 객체 생성 및 사용
    SunScanner scanner;
    SunParser parser;
    SunCodeGenerator codeGenerator;
    SunOptimizer optimizer;

    pFactory = new CompilerFactory(&scanner, 
                                    &parser, &codeGenerator, &optimizer);  
  }
  else {  
    // -- 지원 안하는 시스템 환경  
    cout << sysInfo.sysname << endl;
    return(0);
  }

  Scanner *pScanner;
  pScanner = pFactory->CreateScanner();

  Parser *pParser;
  pParser = pFactory->CreateParser();

  CodeGenerator *pCodeGenerator;
  pCodeGenerator = pFactory->CreateCodeGenerator();

  Optimizer *pOptimizer;
  pOptimizer = pFactory->CreateOptimizer();
}
