#include <stdio.h>
#include <iostream>
#include <comdef.h>
#include <conio.h>
#include "TAdoDB.h"

_variant_t  vtEmpty (DISP_E_PARAMNOTFOUND, VT_ERROR); 

void TAdoDB::ErrorHandler(_com_error &e, char* ErrStr)
{
  
	sprintf(ErrStr,"Error:\n");
	sprintf(ErrStr,"%sCode = %08lx\n",ErrStr ,e.Error());
	sprintf(ErrStr,"%sCode meaning = %s\n", ErrStr, (char*) e.ErrorMessage());
	sprintf(ErrStr,"%sSource = %s\n", ErrStr, (char*) e.Source());
	sprintf(ErrStr,"%sDescription = %s",ErrStr, (char*) e.Description());
	cout<<"\n"<<ErrStr<<"\n";
}

void TAdoDB::GetErrorErrStr(char* ErrStr)
{
	sprintf(ErrStr,"%s",m_ErrStr);
}


bool TAdoDB::Open(char* UserName, char* Pwd,char* CnnStr)
{
	try
	{
		HRESULT hr;
		hr    = m_lpConnection.CreateInstance( __uuidof( ADODB::Connection ) );
		m_lpConnection->Open(CnnStr, UserName, Pwd, NULL);// adAsyncConnect
	}
	
	CATCHERROR(m_lpConnection,0)

	sprintf(m_ErrStr,"Success");
	return 1;
}

bool TAdoDB::OpenTbl(int Mode, char* CmdStr, TAdoTable &Tbl)
{
	int		iRetval = 0,result=1;
	if(m_lpConnection==NULL)
	{
		Tbl.m_lpRecordSet=NULL;
		sprintf(m_ErrStr,"Invalid Connection");
		return 0;
	}
	LP_RECORD_SET t_Rec=NULL;
	try
	{
		t_Rec.CreateInstance( __uuidof( ADODB::Recordset ) );
		t_Rec->Open(CmdStr,_variant_t((IDispatch *) m_lpConnection, true),ADODB::adOpenStatic,ADODB::adLockOptimistic,Mode);
	}
	
	CATCHERROR(Tbl.m_lpRecordSet,0)

	Tbl.m_lpRecordSet=t_Rec;
	sprintf(m_ErrStr,"Success");
	return iRetval;
}

int TAdoDB::TestCmdStoredProc( bool bRec, char* CmdStr, TAdoTable& Tbl)
{
	int		iRetval = 0,result=1;
	
	if(m_lpConnection==NULL)
	{
		Tbl.m_lpRecordSet=NULL;
		sprintf(m_ErrStr,"Invalid Connection");
		return 0;
	}
	LP_RECORD_SET t_Rec=NULL;
	try
	{
		t_Rec.CreateInstance( __uuidof( ADODB::Recordset ) );		
		LP_COMMAND				cmd;
		cmd.CreateInstance(__uuidof(Command)); 
		// 커맨트 객체 초기화 
        cmd->CommandType=adCmdStoredProc;
		cmd->CommandText=_bstr_t(CmdStr); 
        cmd->ActiveConnection=m_lpConnection; 

		// 저장프로시져의 리턴 값 및 파라메터값을 지정함. 
        _ParameterPtr  paramRet=cmd->CreateParameter(_bstr_t("return"),adInteger,adParamReturnValue,sizeof(int)) ; 
        _ParameterPtr  paramInput=cmd->CreateParameter(_bstr_t("inputData"),adChar,adParamInput,10, "090") ; 
		_ParameterPtr  paramInput2=cmd->CreateParameter(_bstr_t("inputData2"),adChar,adParamInput,10,"090") ; 
        	         
        // 파라미터값을 command객체에 추가( returnValue를 처음에 append해야 함.) 
        cmd->Parameters->Append(paramRet); 
        cmd->Parameters->Append(paramInput); 
		cmd->Parameters->Append(paramInput2); 

        // 명령실행 
        t_Rec=cmd->Execute(NULL,NULL,adCmdStoredProc); 
        iRetval = cmd->Parameters->GetItem("return")->Value; 		
	}
	
	catch(_com_error & e)
	{
		_bstr_t bstrSource(e.Source());
		_bstr_t bstrDescription(e.Description());
		printf("\n Source : %s \n Description : %s \n",(LPCSTR)bstrSource,(LPCSTR)bstrDescription);
	}	

	return iRetval;
}
LP_COMMAND TAdoDB::CreateCommand(char* CmdProcStr)
{
	if(m_lpConnection==NULL)
	{
		sprintf(m_ErrStr,"Invalid Connection");
		return 0;
	}
	LP_COMMAND				cmd;
	try
	{			
		cmd.CreateInstance(__uuidof(Command)); 

		// 커맨트 객체 초기화 
		cmd->CommandType		=	adCmdStoredProc;
		cmd->CommandText		=	_bstr_t(CmdProcStr); 
		cmd->ActiveConnection	=	m_lpConnection; 

		 _ParameterPtr  paramRet=cmd->CreateParameter(_bstr_t("return"),adInteger,adParamReturnValue,sizeof(int)) ; 
		 cmd->Parameters->Append(paramRet); 
	}
	catch(_com_error & e)
	{
		_bstr_t bstrSource(e.Source());
		_bstr_t bstrDescription(e.Description());
		printf("\n Source : %s \n Description : %s \n",(LPCSTR)bstrSource,(LPCSTR)bstrDescription);
	}	

	return cmd;
}
HRESULT TAdoDB::Addparameter(LP_COMMAND	pCmd, 
							 char* pIndex, 
							 ADODB::DataTypeEnum type, 
							 ADODB::ParameterDirectionEnum Dir, 
							 ADODB::ADO_LONGPTR size, 
							 char* pData )
{
	HRESULT hr;
	// 저장프로시져의 리턴 값 및 파라메터값을 지정함. 
    _ParameterPtr  param=pCmd->CreateParameter(_bstr_t(pIndex),type,Dir,size,pData) ; 
    hr = pCmd->Parameters->Append(param); 
	return hr;
}
int TAdoDB::CmdStoredProc(LP_COMMAND	pCmd, TAdoTable& Tbl)
{
	int		iRetval = 0;

	LP_RECORD_SET t_Rec=NULL;
	try
	{
		t_Rec.CreateInstance( __uuidof( ADODB::Recordset ) );		
	    // 명령실행 
        t_Rec=pCmd->Execute(NULL,NULL,adCmdStoredProc); 
		Tbl.m_lpRecordSet=t_Rec;
        iRetval = pCmd->Parameters->GetItem("return")->Value; 		
	}
	
	catch(_com_error & e)
	{
		_bstr_t bstrSource(e.Source());
		_bstr_t bstrDescription(e.Description());
		printf("\n Source : %s \n Description : %s \n",(LPCSTR)bstrSource,(LPCSTR)bstrDescription);
	}	

	return iRetval;
}
bool TAdoDB::Execute(char* CmdStr)
{
	HRESULT	hResult;
	try
	{
		hResult = m_lpConnection->Execute(CmdStr,NULL,ADODB::adExecuteNoRecords);
	}
	catch(_com_error &e)
	{
		ErrorHandler(e,m_ErrStr);
		return 0;
	}
	if (FAILED(hResult)) return false;
	return true;	
}

bool TAdoDB::Execute(char* CmdStr, TAdoTable& Tbl)
{
	LP_RECORD_SET t_Rec=NULL;
	try
	{
		t_Rec=m_lpConnection->Execute(CmdStr,NULL,1);
	}

	CATCHERROR(Tbl.m_lpRecordSet,0)

	sprintf(m_ErrStr,"Success");
	Tbl.m_lpRecordSet=t_Rec;
	sprintf(m_ErrStr,"Success");
	return 1;
}



TAdoDB::TAdoDB()
{
	m_lpConnection=NULL;
	sprintf(m_ErrStr,"LNULL POINTER");
}

TAdoDB::~TAdoDB()
{
}