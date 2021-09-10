#include <iostream>
#include <strings.h>      
#include <sys/utsname.h>
using namespace std;

#define HPUX "HPUX"
#define SUNOS "SunOS"

class Scanner {};
class Parser {};
class CodeGenerator {};
class Optimizer {};

class HPScanner : public Scanner {};
class HPParser : public Parser {};
class HPCodeGenerator : public CodeGenerator {};
class HPOptimizer : public Optimizer {};

class SunScanner : public Scanner {};
class SunParser : public Parser {};
class SunCodeGenerator : public CodeGenerator {};
class SunOptimizer : public Optimizer {};

struct utsname sysInfo;

void ScanParse()
{
  if (strncasecmp(sysInfo.sysname, HPUX, strlen(HPUX)) == 0) {  
    // -- HP 용 객체 생성 및 사용
    HPScanner scanner;
    HPParser parser;
  }
  else if (strncasecmp(sysInfo.sysname, SUNOS, strlen(SUNOS)) == 0) {  
    // -- Sun 용 객체 생성 및 사용
    SunScanner scanner;
    SunParser parser;
  }
  else {  
    // -- 지원 안하는 시스템 환경  
    cout << sysInfo.sysname << "does not supported" << endl;
  }
}

void CodeGenOptimize()
{
  if (strncasecmp(sysInfo.sysname, HPUX, strlen(HPUX)) == 0) {  
    // -- HP 용 객체 생성 및 사용
    HPCodeGenerator codeGenerator;
    HPOptimizer optimizer;
  }
  else if (strncasecmp(sysInfo.sysname, SUNOS, strlen(SUNOS)) == 0) {  
    // -- Sun 용 객체 생성 및 사용
    SunCodeGenerator codeGenerator;
    SunOptimizer optimizer;
  }
  else {  
    // -- 지원 안하는 시스템 환경  
    cout << sysInfo.sysname << "does not supported" << endl;
  }
}

int main()
{
  // -- OS 버전 및 하드웨어 타입 정보 얻기 위한 시스템 함수  
  if (uname(&sysInfo) < 0) { 
    cout << "Error Occurred" << endl;
    return(-1);
  }

  ScanParse();
  CodeGenOptimize();
}
