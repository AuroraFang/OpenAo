#include "StdAfx.h"
#include "GLogSourceDB.h"


CGLogSourceDB::CGLogSourceDB(void)
{
	this->Init();
}

CGLogSourceDB::~CGLogSourceDB(void)
{	
	// DB ���� ���� �� �ν��Ͻ� ����
	if(adStateClosed != m_pConnection->GetState())
	{
		m_pConnection->Close();
	}
	m_pConnection.Release();
	m_pConnection = NULL;

	// DB ����� ���� �� �ν��Ͻ� ����
	if(adStateClosed != m_pRecordSet->GetState())
	{
		m_pRecordSet->Close();
	}
	m_pRecordSet.Release();
	m_pRecordSet = NULL;

	// DB ��� ���� �� �ν��Ͻ� ����
	m_pCommand.Release();
	m_pCommand = NULL;

	// COM ��ȯ
	::CoUninitialize();
}

void CGLogSourceDB::Init()
{
	// COM �ʱ�ȭ
	::CoInitialize(NULL);

	// DB ���� �ν��Ͻ� ����
	m_pConnection	= NULL;
	m_pConnection.CreateInstance(__uuidof(Connection));

	// DB ����� �ν��Ͻ� ����
	m_pRecordSet	= NULL;
	m_pRecordSet.CreateInstance(__uuidof(Recordset));

	// DB ��� �ν��Ͻ� ����
	m_pCommand		= NULL;
	m_pCommand.CreateInstance(__uuidof(Command));
	
	m_bIsDBCnn = FALSE;
}
void CGLogSourceDB::DBDisCnn()
{
	m_pConnection->Close();
	m_bIsDBCnn = FALSE;
}
bool CGLogSourceDB::DBCnn(TCHAR * i_strSourceDBCnn) 
{
	if(m_bIsDBCnn)
	{
		return TRUE;
	}

	try
	{
		m_pConnection->Open(i_strSourceDBCnn, "", "", adConnectUnspecified);
		m_pCommand->ActiveConnection = m_pConnection;
		g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog SourceDB Connection Success!!! ==============\r\n",TRUE);
	}
	catch(_com_error& e)
	{
		// Print COM errors. 
		_bstr_t bstrSource(e.Source());
		_bstr_t bstrDescription(e.Description());
		TCHAR	szError[1024];
		wsprintf(szError, L"[ERROR] GLog SourceDB Connection Fail!!! Error = %08lx, Message = %s , Source = %s, Description =%s \r\n"
			, e.Error(), e.ErrorMessage(), (LPCWSTR)bstrSource, (LPCWSTR)bstrDescription);
		g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(szError, TRUE);
		m_bIsDBCnn	= FALSE;
		return FALSE;
	}
	m_bIsDBCnn	= TRUE;
	return TRUE;
}