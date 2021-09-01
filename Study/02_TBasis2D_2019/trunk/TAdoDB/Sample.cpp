// TAdoDB.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "TAdoDB.h"

using namespace std;
/*
���� ���ڿ� �ɼ� �����ϱ�
���� ���ڿ����� �� ���� ���α׷��� �����ͺ��̽��� �����ϴ� �� �ʿ��� ��� ������ ���ԵǾ� �־�� �մϴ�. �� ���ڿ��� �ش� �������� ������ ��ũ��Ʈ�� ���ԵǾ� ���߿� ���� ���α׷� �������� ó���˴ϴ�. 

Microsoft Access �� SQL Server �����ͺ��̽��� ���� ���ڿ��� ���� �Ű� ������ �����ݷ����� ������ �������� �����˴ϴ�.

Provider�� �����ͺ��̽��� OLE DB �����ڸ� �����մϴ�. ���� ���, ������ ���� Access, SQL Server �� Oracle �����ͺ��̽��� �Ϲ� OLE DB �����ڿ� ���� �Ű� �����Դϴ�.

Provider=Microsoft.Jet.OLEDB.4.0;...
Provider=SQLOLEDB;...
Provider=OraOLEDB;...

OLE DB ������ �Ű� ���� ���� ���� �ڼ��� ������ ����̹� ���޾�ü�� ������ �����ϰų� �ý��� �����ڿ��� �����Ͻʽÿ�.

Provider �Ű� ������ ���Խ�Ű�� ������ ODBC�� �⺻ OLE DB �����ڰ� ���ǹǷ� �����ͺ��̽��� ���� ������ ODBC ����̹��� �����ؾ� �մϴ�.

Driver�� �����ͺ��̽��� ���� OLE DB �����ڸ� �������� ���� ��� ����� ODBC ����̹��� �����մϴ�.

Server�� �� ���� ���α׷��� �ٸ� �������� ����Ǵ� ��� SQL Server �����ͺ��̽��� ȣ�����ϴ� ������ �����մϴ�.

Database�� SQL Server �����ͺ��̽��� �̸��Դϴ�.

DBQ�� Microsoft Access���� ���� �Ͱ� ���� ���� ��� �����ͺ��̽��� ����Դϴ�. ��δ� �����ͺ��̽� ������ ȣ�����ϴ� ������ ����Դϴ�.

UID������ ����� �̸��� �����մϴ�.

PWD������ ����� ��ȣ�� �����մϴ�.

DSN�� ������ �ҽ� �̸��� ����� ��� ������ �ҽ� �̸��Դϴ�. �������� DSN�� �����ϴ� ����� ���� ���� ���ڿ��� �ٸ� �Ű� ������ ������ �� �ֽ��ϴ�. ���� ���, DSN�� ���� �� �ٸ� �Ű� ������ �����ϸ� DSN=Results�� ��ȿ�� ���� ���ڿ��� �� �� �ֽ��ϴ�(DSN ��� ����).

�ٸ� �����ͺ��̽� ������ ���� ���ڿ��� ���� ������ �Ű� ������ ������� �ʰų� �Ű� ������ �ٸ� �̸� �Ǵ� �ٸ� �뵵�� ����� ���� �ֽ��ϴ�. �ڼ��� ������ �����ͺ��̽� ���޾�ü�� ������ �����ϰų� �ý��� �����ڿ��� �����Ͻʽÿ�.

������ trees.mdb��� Access �����ͺ��̽��� ODBC ������ ����� ���� ���ڿ��� �����Դϴ�.

Driver={Microsoft Access Driver (*.mdb)};
DBQ=C:\Inetpub\wwwroot\Research\trees.mdb

������ Gojira��� ������ ��ġ�� Mothra��� SQL Server �����ͺ��̽��� OLE DB ������ ����� ���� ���ڿ��� �����Դϴ�.

Provider=SQLOLEDB;Server=Gojira;Database=Mothra;UID=jsmith;
PWD=orlando8

< ��Ʈ�� ���� ��� >
1.�ƹ� txt������ ����ð� �̸��� XXXX.udl�� ������ �ֽʽÿ�.
2.�������� ��ǻ�Ϳ� ����������� ����Ǿ����� ����Ŭ��
3.������ ���� �Ӽ�â���� �����ڸ� ����(�� Microsoft OLE DB Provider for SQL Server) �� ���� ��ư
4.���������� �̵��Ǿ��ٸ� ������� ��ȣ �����ͺ��̽��� ������ �� �����׽�Ʈ
5.�����׽�Ʈ�� �����̶�� Ȯ�� ��ư�� ������ ��� �ۼ��� XXXX.udl������ �ؽ�Ʈ ������� �о���ø�
6.����� Ȯ���Ͻø� �˴ϴ�.
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
	if( iReturnValue == 0 ) cout<<"\n ���� �α��� �Ϸ�"<<"\n";
	if( iReturnValue == 1 ) cout<<"���� ���̵� �߸� �Ǿ����ϴ�."<<"\n";
	if( iReturnValue == 2 ) cout<<"�н����尡 ��ġ���� �ʽ��ϴ�."<<"\n";
	


	/* Test DB 
	int iReturnValue = db.TestCmdStoredProc(false, "usp_LoginAccount",tbl);
	if( iReturnValue == 0 ) cout<<"\n ���� �α��� �Ϸ�"<<"\n";
	if( iReturnValue == 1 ) cout<<"���� ���̵� �߸� �Ǿ����ϴ�."<<"\n";
	if( iReturnValue == 2 ) cout<<"�н����尡 ��ġ���� �ʽ��ϴ�."<<"\n";	*/

	::CoUninitialize();
	return 0;
}
