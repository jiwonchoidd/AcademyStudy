// TAdoDB.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "TAdoDB.h"

using namespace std;
/*
연결 문자열 옵션 설정하기
연결 문자열에는 웹 응용 프로그램이 데이터베이스에 연결하는 데 필요한 모든 정보가 포함되어 있어야 합니다. 이 문자열은 해당 페이지의 서버측 스크립트에 삽입되어 나중에 응용 프로그램 서버에서 처리됩니다. 

Microsoft Access 및 SQL Server 데이터베이스용 연결 문자열은 다음 매개 변수를 세미콜론으로 구분한 조합으로 구성됩니다.

Provider는 데이터베이스의 OLE DB 공급자를 지정합니다. 예를 들어, 다음은 각각 Access, SQL Server 및 Oracle 데이터베이스의 일반 OLE DB 공급자에 대한 매개 변수입니다.

Provider=Microsoft.Jet.OLEDB.4.0;...
Provider=SQLOLEDB;...
Provider=OraOLEDB;...

OLE DB 공급자 매개 변수 값에 대한 자세한 내용은 드라이버 공급업체의 설명서를 참조하거나 시스템 관리자에게 문의하십시오.

Provider 매개 변수를 포함시키지 않으면 ODBC용 기본 OLE DB 공급자가 사용되므로 데이터베이스에 대한 적절한 ODBC 드라이버를 지정해야 합니다.

Driver는 데이터베이스에 대한 OLE DB 공급자를 지정하지 않은 경우 사용할 ODBC 드라이버를 지정합니다.

Server는 웹 응용 프로그램이 다른 서버에서 실행되는 경우 SQL Server 데이터베이스를 호스팅하는 서버를 지정합니다.

Database는 SQL Server 데이터베이스의 이름입니다.

DBQ는 Microsoft Access에서 만든 것과 같은 파일 기반 데이터베이스의 경로입니다. 경로는 데이터베이스 파일을 호스팅하는 서버의 경로입니다.

UID에서는 사용자 이름을 지정합니다.

PWD에서는 사용자 암호를 지정합니다.

DSN은 데이터 소스 이름을 사용할 경우 데이터 소스 이름입니다. 서버에서 DSN을 정의하는 방법에 따라 연결 문자열의 다른 매개 변수를 생략할 수 있습니다. 예를 들어, DSN을 만들 때 다른 매개 변수를 정의하면 DSN=Results가 유효한 연결 문자열이 될 수 있습니다(DSN 사용 참조).

다른 데이터베이스 유형의 연결 문자열은 위에 나열된 매개 변수를 사용하지 않거나 매개 변수를 다른 이름 또는 다른 용도로 사용할 수도 있습니다. 자세한 내용은 데이터베이스 공급업체의 설명서를 참조하거나 시스템 관리자에게 문의하십시오.

다음은 trees.mdb라는 Access 데이터베이스에 ODBC 연결을 만드는 연결 문자열의 예제입니다.

Driver={Microsoft Access Driver (*.mdb)};
DBQ=C:\Inetpub\wwwroot\Research\trees.mdb

다음은 Gojira라는 서버에 위치한 Mothra라는 SQL Server 데이터베이스에 OLE DB 연결을 만드는 연결 문자열의 예제입니다.

Provider=SQLOLEDB;Server=Gojira;Database=Mothra;UID=jsmith;
PWD=orlando8

< 스트링 생성 방법 >
1.아무 txt파일을 만드시고 이름을 XXXX.udl로 변경해 주십시요.
2.아이콘이 컴퓨터와 폴더모양으로 변경되었으면 더블클릭
3.데이터 연결 속성창에서 공급자를 선택(전 Microsoft OLE DB Provider for SQL Server) 후 다음 버튼
4.연결탭으로 이동되었다면 서버명과 암호 데이터베이스를 선택한 후 연결테스트
5.연결테스트가 성공이라면 확인 버튼을 누른후 방금 작성된 XXXX.udl파일을 텍스트 편집기로 읽어오시면
6.결과를 확인하시면 됩니다.
*/
//char CnnStr[200]="Provider=SQLOLEDB.1;Persist Security Info=False;User ID=sa;Initial Catalog=WildPlanetDB;Data Source=server.kgcaschool.co.kr";
//char CnnStr[200]="Provider=SQLOLEDB.1;Persist Security Info=False;User ID=sa;Initial Catalog=WildPlanetDB;Data Source=printer";
char CnnStr[200] = "Provider=SQLOLEDB.1;Persist Security Info=False;User ID=sa;Initial Catalog=WildPlanetDB;Data Source=KGCA3D\\KGCA3D";
char ErrStr[200];

int _tmain(int argc, _TCHAR* argv[])
{
	::CoInitialize(NULL);
	TAdoDB db;
	TAdoTable tbl;
	if(!db.Open("sa","kgca!@34",CnnStr))
	{
		db.GetErrorErrStr(ErrStr);
		cout<<ErrStr<<"\n";
	}
		
	/*if(!db.Execute("select * from Account order by id,pw",tbl))
	{
		db.GetErrorErrStr(ErrStr);		
	}
	if(!db.Execute("EXEC OPENUSER",tbl))
	{
		db.GetErrorErrStr(ErrStr);
		cout<<ErrStr<<"\n";
	}*/

	
	if(!db.OpenTbl(ADODB::adCmdText,"select * from Account order by id,personName",tbl))
	{
		db.GetErrorErrStr(ErrStr);		
	}

	char id[100]={0,};
	if(!tbl.ISEOF())	tbl.MoveFirst();
	
	while(!tbl.ISEOF())
	{
		if(tbl.Get("id",id))			cout<<"\nID:"<<id;
		else{ tbl.GetErrorErrStr(ErrStr);	break;	}

		if(tbl.Get("pw",id))			cout<<" PW:"<<id;
		else{ tbl.GetErrorErrStr(ErrStr);	break;	}

		if(tbl.Get("PersonName",id))	cout<<" PersonName:"<<id;
		else{ tbl.GetErrorErrStr(ErrStr);	break;	}
		
		if(tbl.Get("PersonID",id))		cout<<" PersonID:"<<id;
		else{ tbl.GetErrorErrStr(ErrStr);	break;	}

		if(tbl.Get("eMail",id))			cout<<" EMail:"<<id;
		else{ tbl.GetErrorErrStr(ErrStr);	break;	}
		
		if(tbl.GetTime("accountCreate",id))	cout<<" AccountTime:"<<id;
		else{ tbl.GetErrorErrStr(ErrStr);	break;	}		

		tbl.MoveNext();
	}

	LP_COMMAND pCmd = db.CreateCommand("usp_LoginAccount");
	db.Addparameter( pCmd, "id", adChar,adParamInput, 10, "090");
	db.Addparameter( pCmd, "pw", adChar,adParamInput, 10, "090");
	
	int iReturnValue = db.CmdStoredProc( pCmd, tbl );
	if( iReturnValue == 0 ) cout<<"\n 정상 로그인 완료"<<"\n";
	if( iReturnValue == 1 ) cout<<"접속 아이디가 잘못 되었습니다."<<"\n";
	if( iReturnValue == 2 ) cout<<"패스워드가 일치하지 않습니다."<<"\n";
	


	/* Test DB 
	int iReturnValue = db.TestCmdStoredProc(false, "usp_LoginAccount",tbl);
	if( iReturnValue == 0 ) cout<<"\n 정상 로그인 완료"<<"\n";
	if( iReturnValue == 1 ) cout<<"접속 아이디가 잘못 되었습니다."<<"\n";
	if( iReturnValue == 2 ) cout<<"패스워드가 일치하지 않습니다."<<"\n";	*/

	::CoUninitialize();
	return 0;
}
