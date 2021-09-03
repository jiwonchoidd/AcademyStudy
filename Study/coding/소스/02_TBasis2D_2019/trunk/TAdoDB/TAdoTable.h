#pragma once

#include <stdio.h>
#include <iostream>
#include <comdef.h>
#include <conio.h>
#include <TCHAR.h>

using namespace std;


#define CATCHERROR(ptr,a)	catch(_com_error &e)\
							{\
								ErrorHandler(e,m_ErrStr);\
								ptr=NULL;\
								return a;\
							}

#define CATCHERRGET			catch(_com_error &e)\
							{\
								ErrorHandler(e,m_ErrStr);\
								sprintf(m_ErrStr,"%s\n**For Field Name:%s",m_ErrStr,FieldName);\
								return 0;\
							}




#import "c:\Program Files\Common Files\System\ADO\msado15.dll" \
rename("EOF", "EndOfFile")

typedef ADODB::_RecordsetPtr	LP_RECORD_SET;
typedef ADODB::_ConnectionPtr	LP_CONNECTION; 
typedef ADODB::_CommandPtr		LP_COMMAND; 

using namespace ADODB;

class TAdoTable
{
public:
	LP_RECORD_SET	m_lpRecordSet;
	char			m_ErrStr[500];
	
	void GetErrorErrStr(char* ErrStr);
	void ErrorHandler(_com_error &e, char* ErrStr);
	int ISEOF();
	HRESULT MoveNext();
	HRESULT MovePrevious();
	HRESULT MoveFirst();
	HRESULT MoveLast();
	int AddNew();
	int Update();
	int Add(char* FieldName, char* FieldValue);
	int Add(char* FieldName,int FieldValue);
	int Add(char* FieldName,float FieldValue);
	int Add(char* FieldName,double FieldValue);
	int Add(char* FieldName,long FieldValue);
	bool Get(char* FieldName, char* FieldValue);
	bool Get(char* FieldName,int& FieldValue);
	bool Get(char* FieldName,float& FieldValue);
	bool Get(char* FieldName,double& FieldValue);
	bool Get(char* FieldName,double& FieldValue,int Scale);
	bool Get(char* FieldName,long& FieldValue);
	bool GetTime(char* FieldName, char* FieldValue);

	TAdoTable();
	virtual ~TAdoTable();
};

