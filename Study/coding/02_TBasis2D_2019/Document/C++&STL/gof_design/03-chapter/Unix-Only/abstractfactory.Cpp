#include <iostream>
#include <strings.h>
#include <sys/utsname.h>
using namespace std;

#define HPUX "HPUX"
#define SUNOS "SunOS"

class Scanner {
  public : 
    virtual ~Scanner()=0;
};

class Parser {
  public : 
    virtual ~Parser()=0;
};

class CodeGenerator {
  public : 
    virtual ~CodeGenerator()=0;
};

class Optimizer {
  public : 
    virtual ~Optimizer()=0;
};

Scanner::~Scanner() {}
Parser::~Parser() {}
CodeGenerator::~CodeGenerator() {}
Optimizer::~Optimizer() {}

class HPScanner : public Scanner {};
class HPParser : public Parser {};
class HPCodeGenerator : public CodeGenerator {};
class HPOptimizer : public Optimizer {};

class SunScanner : public Scanner {};
class SunParser : public Parser {};
class SunCodeGenerator : public CodeGenerator {};
class SunOptimizer : public Optimizer {};

class CompilerFactory {  
  public :    
    virtual Scanner* CreateScanner() = 0;    
    virtual Parser* CreateParser() = 0;    
    virtual CodeGenerator* CreateCodeGenerator() = 0;    
    virtual Optimizer* CreateOptimizer() = 0;  
};

class HPCompilerFactory : public CompilerFactory {  
  public :    
    Scanner* CreateScanner() { new HPScanner; }    
    Parser* CreateParser() { new HPParser; }    
    CodeGenerator* CreateCodeGenerator() { new HPCodeGenerator; }    
    Optimizer* CreateOptimizer() { new HPOptimizer; }
};

class SunCompilerFactory : public CompilerFactory {  
  public :    
    Scanner* CreateScanner() { new SunScanner; }    
    Parser* CreateParser() { new SunParser; }    
    CodeGenerator* CreateCodeGenerator() { new SunCodeGenerator; }    
    Optimizer* CreateOptimizer() { new SunOptimizer; }
};

CompilerFactory *pFactory;  

int main() {  
  struct utsname sysInfo;

  // -- OS 버전 및 하드웨어 타입 정보 얻기 위한 시스템 함수  
  if (uname(&sysInfo) < 0) { 
    cout << "Error Occurred" << endl;
    return(-1);
  }

  if (strncasecmp(sysInfo.sysname, HPUX, strlen(HPUX)) == 0) {  
    // -- HP 용 객체 생성 및 사용
    pFactory = new HPCompilerFactory;
  }
  else if (strncasecmp(sysInfo.sysname, SUNOS, strlen(SUNOS)) == 0) {  
    // -- Sun 용 객체 생성 및 사용
    pFactory = new SunCompilerFactory;
  }
  else {  
    // -- 지원 안하는 시스템 환경  
    cout << sysInfo.sysname << endl;
    return(0);
  }

  Scanner *pScanner = pFactory->CreateScanner();
  Parser *pParser = pFactory->CreateParser();
  CodeGenerator *pCodeGenerator = pFactory->CreateCodeGenerator();
  Optimizer *pOptimizer = pFactory->CreateOptimizer();
}
