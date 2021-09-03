#include "TAdoTable.h"

void TAdoTable::ErrorHandler(_com_error &e, char* ErrStr)
{
  
	sprintf(ErrStr,"Error:\n");
	sprintf(ErrStr,"%sCode = %08lx\n",ErrStr ,e.Error());
	sprintf(ErrStr,"%sCode meaning = %s\n", ErrStr, (char*) e.ErrorMessage());
	sprintf(ErrStr,"%sSource = %s\n", ErrStr, (char*) e.Source());
	sprintf(ErrStr,"%sDescription = %s",ErrStr, (char*) e.Description());
}


int TAdoTable::ISEOF()
{
	int rs;
	if(m_lpRecordSet==NULL)
	{
		sprintf(m_ErrStr,"Invalid Record");
		return -1;
	}
	try{
		rs=m_lpRecordSet->EndOfFile;
	}
	
	CATCHERROR(m_lpRecordSet,-2)

	sprintf(m_ErrStr,"Success");
	return rs;
}

bool TAdoTable::Get(char* FieldName, char* FieldValue)
{
	try
	{
		_variant_t  vtValue;
		vtValue = m_lpRecordSet->Fields->GetItem(FieldName)->GetValue();
		sprintf(FieldValue,"%s",(LPCSTR)((_bstr_t)vtValue.bstrVal));
	}

	CATCHERRGET

	sprintf(m_ErrStr,"Success");
	return 1;
}
bool TAdoTable::GetTime(char* FieldName, char* FieldValue)
{
	try
	{
		_variant_t  vtValue;
		vtValue = m_lpRecordSet->Fields->GetItem(FieldName)->GetValue();
		sprintf(FieldValue,"%s",(LPCSTR)((_bstr_t)vtValue));
	}

	CATCHERRGET

	sprintf(m_ErrStr,"Success");
	return 1;
}
bool TAdoTable::Get(char* FieldName,int& FieldValue)
{
	try
	{
		_variant_t  vtValue;//     _bstr_t val = field->GetValue();

		vtValue = m_lpRecordSet->Fields->GetItem(FieldName)->GetValue();
		FieldValue=vtValue.intVal;
	}

	CATCHERRGET

	sprintf(m_ErrStr,"Success");
	return 1;
}

bool TAdoTable::Get(char* FieldName,float& FieldValue)
{
	try
	{
		_variant_t  vtValue;
		vtValue = m_lpRecordSet->Fields->GetItem(FieldName)->GetValue();
		FieldValue=vtValue.fltVal;
	}

	CATCHERRGET

	sprintf(m_ErrStr,"Success");
	return 1;
}

bool TAdoTable::Get(char* FieldName,double& FieldValue)
{
	try
	{
		_variant_t  vtValue;
		vtValue = m_lpRecordSet->Fields->GetItem(FieldName)->GetValue();
		FieldValue=vtValue.dblVal;
		//GetDec(vtValue,FieldValue,3);
	}

	CATCHERRGET

	sprintf(m_ErrStr,"Success");
	return 1;
}

HRESULT TAdoTable::MoveNext()
{
	HRESULT hr;
	try
	{
		hr=m_lpRecordSet->MoveNext();
	}
	catch(_com_error &e)
	{
		ErrorHandler(e,m_ErrStr);
		//m_lpRecordSet=NULL;
		return -2;
	}
	sprintf(m_ErrStr,"Success");
	return hr;
}

HRESULT TAdoTable::MovePrevious()
{
	HRESULT hr;
	try
	{
		hr=m_lpRecordSet->MovePrevious();
	}
	catch(_com_error &e)
	{
		ErrorHandler(e,m_ErrStr);
		//m_lpRecordSet=NULL;
		return -2;
	}
	sprintf(m_ErrStr,"Success");
	return hr;
}

HRESULT TAdoTable::MoveFirst()
{
	HRESULT hr;
	try
	{
		hr=m_lpRecordSet->MoveFirst();
	}
	catch(_com_error &e)
	{
		ErrorHandler(e,m_ErrStr);
		//m_lpRecordSet=NULL;
		return -2;
	}
	sprintf(m_ErrStr,"Success");
	return hr;
}

HRESULT TAdoTable::MoveLast()
{
	HRESULT hr;
	try
	{
		hr=m_lpRecordSet->MoveLast();
	}
	catch(_com_error &e)
	{
		ErrorHandler(e,m_ErrStr);
		//m_lpRecordSet=NULL;
		return -2;
	}
	sprintf(m_ErrStr,"Success");
	return hr;
}
void TAdoTable::GetErrorErrStr(char* ErrStr)
{
	sprintf(ErrStr,"%s",m_ErrStr);
}


TAdoTable::TAdoTable()
{
	m_lpRecordSet=NULL;
}

TAdoTable::~TAdoTable()
{
	m_lpRecordSet=NULL;
}