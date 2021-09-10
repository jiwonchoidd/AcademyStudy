#include <iostream>
#include <strings.h>
#include <sys/utsname.h>
using namespace std;

#define HPUX "HPUX"
#define SUNOS "SunOS"

class Product {
  public :
    virtual Product* Clone()=0;
};

class HPScanner : public Product {
  public :
    Product* Clone() { return new HPScanner(*this); }
};

class HPParser : public Product {
  public :
    Product* Clone() { return new HPParser(*this); }
};

class HPCodeGenerator : public Product {
  public :
    Product* Clone() { return new HPCodeGenerator(*this); }
};

class HPOptimizer : public Product {
  public :
    Product* Clone() { return new HPOptimizer(*this); }
};

class SunScanner : public Product {
  public :
    Product* Clone() { return new SunScanner(*this); }
};

class SunParser : public Product {
  public :
    Product* Clone() { return new SunParser(*this); }
};

class SunCodeGenerator : public Product {
  public :
    Product* Clone() { return new SunCodeGenerator(*this); }
};

class SunOptimizer : public Product {
  public :
    Product* Clone() { return new SunOptimizer(*this); }
};

// -- 추가 필요 부분 시작
class HPErrorHandler : public Product {
  public :
    Product* Clone() { return new HPErrorHandler(*this); }
};

class SunErrorHandler : public Product {
  public :
    Product* Clone() { return new SunErrorHandler(*this); }
};
// -- 추가 필요 부분 끝

class CompilerFactory {  
  public :    
    virtual Product* CreateProduct(Product *p) = 0;  
};

class HPCompilerFactory : public CompilerFactory {  
  public :    
    Product* CreateProduct(Product *p) { 
       return p->Clone();
    }
};

class SunCompilerFactory : public CompilerFactory {  
  public :    
    Product* CreateProduct(Product *p) { 
      return p->Clone();
    }
};

CompilerFactory *pFactory;  

int main() {  
  Product *pScanner, *pParser, *pCodeGenerator, *pOptimizer;
  Product *pErrorHandler; // -- 추가 필요
  struct utsname sysInfo;

  // -- OS 버전 및 하드웨어 타입 정보 얻기 위한 시스템 함수  
  if (uname(&sysInfo) < 0) { 
    cout << "Error Occurred" << endl;
    return(-1);
  }

  if (strncasecmp(sysInfo.sysname, HPUX, strlen(HPUX)) == 0) {  
    // -- HP 용 객체 생성 및 사용
    pScanner = new HPScanner;
    pParser = new HPParser;
    pCodeGenerator = new HPCodeGenerator;
    pOptimizer = new HPOptimizer;
    pErrorHandler = new HPErrorHandler; // -- 추가 필요

    pFactory = new HPCompilerFactory;
  }
  else if (strncasecmp(sysInfo.sysname, SUNOS, strlen(SUNOS)) == 0) {  
    // -- Sun 용 객체 생성 및 사용
    pScanner = new SunScanner;
    pParser = new SunParser;
    pCodeGenerator = new SunCodeGenerator;
    pOptimizer = new SunOptimizer;
    pErrorHandler = new SunErrorHandler; // -- 추가 필요

    pFactory = new SunCompilerFactory;
  }
  else {  
    // -- 지원 안하는 시스템 환경  
    cout << sysInfo.sysname << endl;
    return(0);
  }

  Product *pNewScanner = pFactory->CreateProduct(pScanner);
  Product *pNewParser = pFactory->CreateProduct(pParser);
  Product *pNewCodeGenerator = pFactory->CreateProduct(pCodeGenerator);
  Product *pNewOptimizer = pFactory->CreateProduct(pOptimizer);
  // -- Product *pNewErrorHandler = pFactory->CreateProduct(pErrorHandler);
}
