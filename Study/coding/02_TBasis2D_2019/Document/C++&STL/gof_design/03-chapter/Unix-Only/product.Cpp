#include <iostream>
#include <strings.h>
#include <sys/utsname.h>
using namespace std;

#define HPUX "HPUX"
#define SUNOS "SunOS"

class Product {
  public :
    virtual ~Product()=0;
};

Product::~Product() {}

class HPScanner : public Product {};
class HPParser : public Product {};
class HPCodeGenerator : public Product {};
class HPOptimizer : public Product {};
class HPErrorHandler : public Product {}; // -- 추가 필요

class SunScanner : public Product {};
class SunParser : public Product {};
class SunCodeGenerator : public Product {};
class SunOptimizer : public Product {};
class SunErrorHandler : public Product {}; // -- 추가 필요

class CompilerFactory {  
  public :    
    virtual Product* CreateProduct(int type) = 0;  
};

#define SCANNER         1
#define PARSER          2
#define CODEGENERATOR   3
#define OPTIMIZER       4
#define ERRORHANDLER    5   // -- 추가 필요

class HPCompilerFactory : public CompilerFactory {  
  public :    
    Product* CreateProduct(int type) { 
      switch (type) {
        case SCANNER : return new HPScanner;
        case PARSER : return new HPParser;
        case CODEGENERATOR : return new HPCodeGenerator;
        case OPTIMIZER : return new HPOptimizer;
        case ERRORHANDLER : return new HPErrorHandler; // -- 추가 필요
      }
    }
};

class SunCompilerFactory : public CompilerFactory {  
  public :    
    Product* CreateProduct(int type) { 
      switch (type) {
        case SCANNER : return new SunScanner;
        case PARSER : return new SunParser;
        case CODEGENERATOR : return new SunCodeGenerator;
        case OPTIMIZER : return new SunOptimizer;
        case ERRORHANDLER : return new SunErrorHandler; // -- 추가 필요
      }
    }
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

  Product *pScanner = pFactory->CreateProduct(SCANNER);
  Product *pParser = pFactory->CreateProduct(PARSER);
  Product *pCodeGenerator = pFactory->CreateProduct(CODEGENERATOR);
  Product *pOptimizer = pFactory->CreateProduct(OPTIMIZER);
  // -- Product *pErrorHandler = pFactory->CreateProduct(ERRORHANDLER); 
}
