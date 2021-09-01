[소스코드 컴파일 전 주의 사항]
>> 예제 소스코드는 Unix/Linux용과 VC++용으로 구분되어 있으나, 대부분 동일합니다. 
   다만 VC++에서 컴파일하기 위해 추가, 수정된 부분은 소스코드에 일일이 VC++이라는 
   코멘트가 되어 있습니다. 또한 VC++용 소스코드의 파일 확장자는 컴파일 편의를 위해 
   .cpp로 변경되어 있습니다. 
>> 3장 소스는 Unix/Linux 전용이므로, VC++로 컴파일되지 않습니다. 
>> 26장 소스코드는 제대로 실행되려면 실행하는 PC에 웹서버가 설치되어 있어야 합니다. 
>> VC++에서 컴파일시 Warning이 많이 뜨는데, 이는 무시해도 좋습니다. 


[마이크로소프트 Visual C++ 컴파일러에서 예제 소스코드 컴파일하는 방법]
마이크로소프트 Visual C++ 컴파일러로 “GoF 디자인 패턴! 이렇게 활용한다”책에 나오는 
예제 소스코드를 컴파일하기 위해서는 다음의 과정을 참고하면 됩니다. 
① 마이크로소프트 Visual C++ 컴파일러로 예제 소스코드를 컴파일하기 위해서는 먼저 다음의 
   Visual C++ 서비스 팩을 다운로드받아 설치해야 합니다. 
   >> Visual C++ 5.0의 경우 :
      http://msdn.microsoft.com/vstudio/downloads/updates/sp/vs97/default.aspx
   >> Visual C++ 6.0의 경우 :
      http://msdn.microsoft.com/vstudio/downloads/updates/sp/vs6/sp6/default.aspx
② 한빛미디어 사이트에서 마이크로소프트 Visual C++용 예제 소스코드를 다운로드 받습니다      (http://www.hanbitbook.co.kr/exam/1281에서 pattern_source.zip -> Windows)
③ 각 예제마다 프로젝트를 생성합니다. 
   프로젝트 생성은 [File] ? [New] 메뉴로 연 대화상자에서 [Projects] 탭을 선택하고, 
   'Win32 Console Application'을 선택한 후 [Project name]에 'An empty project'를 입력하고 
   [Finish] 버튼 -> [OK] 버튼 형태로 진행합니다.
④ 각 예제의 프로젝트마다 관련된 파일을 추가합니다.
   파일 추가는 왼쪽 트리에서 하단의 [FileView] 탭을 선택한 후, 프로젝트명 files에서 
   오른쪽 마우스 버튼을 클릭하여 [Add Files to project]를 선택하고 추가할 파일을 지정하면 됩니다. 
⑤ 예제 소스코드의 파일명 확장자가 .c 일 경우 컴파일 옵션에 /Tp를 추가합니다. 
   컴파일 옵션 추가는 [Project] 메뉴 -> [Settings] -> [C/C++] 탭 선택 -> 
   대화상자 하단의 [Project Options]에 텍스트로 ‘/Tp’를 입력하면 된다. 
⑥ dynamic_cast 등 RTTI(Runtime Type Information) 기능을 활용할 경우에는 컴파일 옵션에 /GR을 추가합니다. 
⑦ 컴파일(F7)을 수행합니다. 
⑧ 컴파일이 끝나면 실행(Ctrl+F5)을 수행합니다. 
⑨ 기타 마이크로소프트 Visual C++를 활용한 컴파일이나 실행시 문제가 발생하면          http://msdn.microsoft.com/developercenters/ 사이트를 참조하시기 바랍니다.