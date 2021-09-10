#pragma once
#include "TAdoTable.h"

class TAdoDB
{
public:
	LP_CONNECTION			m_lpConnection;
	LP_RECORD_SET			m_lpRecordSet;
	LP_COMMAND				m_lpCommand;

	char					m_ErrStr[500];
	
	bool Open(char* UserName, char* Pwd,char* CnnStr);
	bool OpenTbl(int Mode, char* CmdStr, TAdoTable& Tbl);
	bool Execute(char* CmdStr);
	bool Execute(char* CmdStr, TAdoTable& Tbl);
	void GetErrorErrStr(char* ErrStr);
	void ErrorHandler(_com_error &e, char* ErrStr);

	// StoredProc
	LP_COMMAND	CreateCommand(char* CmdProcStr);
	HRESULT		Addparameter(LP_COMMAND	pCmd, 
							 char* pIndex, 
							 ADODB::DataTypeEnum type, 
							 ADODB::ParameterDirectionEnum Dir, 
							 ADODB::ADO_LONGPTR size, 
							 char* pData );
	int			CmdStoredProc(LP_COMMAND	pCmd, TAdoTable& Tbl);


	int			 TestCmdStoredProc( bool bRec, char* CmdStr, TAdoTable &Tbl);

	TAdoDB();
	virtual ~TAdoDB();
};

