//////////////////////////////////
// 2011-12-12 by hskim, GLog 2��

#include "StdAfx.h"
#include "GLogDataAceConnectUser.h"

CGLogDataAceConnectUser::CGLogDataAceConnectUser(_RecordsetPtr i_pSourceRecordSet,_CommandPtr i_pSourceCommand,_RecordsetPtr i_pDestRecordSet,_CommandPtr i_pDestCommand)
{
	this->Init(i_pSourceRecordSet,i_pSourceCommand,i_pDestRecordSet,i_pDestCommand);
}

CGLogDataAceConnectUser::~CGLogDataAceConnectUser(void)
{
}

void CGLogDataAceConnectUser::ResetData()
{
	util::zero(m_arrAceConnectUserData,sizeof(CAceConnectUserData)*GLOG_DATA_MAX_CNT);
}

Err_t CGLogDataAceConnectUser::GLogProcessing()
{
	int SelectCnt = 0;
	Err_t Err =	this->GLogDBSelect(&SelectCnt);
	if(ERR_NO_ERROR != Err)
	{
		return this->ErrorProcessing(Err);
	}
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog AceConnectUser Select Fin!!! ==============\r\n",TRUE);

	//Insert & Delete�� ���྿ ó��.
	for(int Cnt = 0 ; Cnt < SelectCnt; Cnt++)
	{
		Err = this->GLogDBInsert(Cnt);
		if(ERR_NO_ERROR != Err
			&& ERR_DB_COM_DUPLICATE_INSERT != Err)
		{
			return this->ErrorProcessing(Err);
		}

		Err = this->GLogDBDelete(Cnt);
		if(ERR_NO_ERROR != Err)
		{
			return this->ErrorProcessing(Err);
		}
	}
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog AceConnectUser Update Fin!!! ==============\r\n",TRUE);

	this->ResetData();
	return ERR_NO_ERROR;
}

Err_t CGLogDataAceConnectUser::GLogDBSelect(int * o_pSelectCnt)
{
	m_pSourceCommand->CommandText = QDB_GLOG_TB_CONNECT_USER_SELECT;
	try
	{
		m_pSourceRecordSet = m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);  // ����� select���� �����մϴ�.
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataAceConnectUser",e);
	}

	if(m_pSourceRecordSet->EndOfFile)     
	{
		m_pSourceRecordSet->Close();   // ���ڵ���� �ݴ´�.(�׻� ���ڵ�� ���� �Ŀ� �ݾ��ִ� �۾��� �����ؾ� �մϴ�.)
		return ERR_DB_NO_DATA;
	}   

	int				nCnt = 0;
	CAceConnectUserData	tmpAceConnectUserData;
	// ��ȸ�� ���ڵ��� ������ ������ ���� �����͸� Ȯ���մϴ�.

	while(!m_pSourceRecordSet->EndOfFile)
	{
 		tmpAceConnectUserData.UID = m_pSourceRecordSet->Fields->GetItem("UID")->GetValue();
		tmpAceConnectUserData.c_date = m_pSourceRecordSet->Fields->GetItem("c_date")->GetValue();
		tmpAceConnectUserData.c_world = m_pSourceRecordSet->Fields->GetItem("c_world")->GetValue();
		tmpAceConnectUserData.c_unique = m_pSourceRecordSet->Fields->GetItem("c_unique")->GetValue();
		tmpAceConnectUserData.c_new = m_pSourceRecordSet->Fields->GetItem("c_new")->GetValue();
		tmpAceConnectUserData.c_times = m_pSourceRecordSet->Fields->GetItem("c_times")->GetValue();
		tmpAceConnectUserData.c_del = m_pSourceRecordSet->Fields->GetItem("c_del")->GetValue();
		LSTRNCPY_OLEDB(tmpAceConnectUserData.c_gameserver,m_pSourceRecordSet->Fields->GetItem("c_gameserver")->GetValue());
		LSTRNCPY_OLEDB(tmpAceConnectUserData.c_publisher,m_pSourceRecordSet->Fields->GetItem("c_publisher")->GetValue());

		memcpy(&m_arrAceConnectUserData[nCnt],&tmpAceConnectUserData,sizeof(CAceConnectUserData));
		util::zero(&tmpAceConnectUserData,sizeof(CAceConnectUserData));

		// ����Ʈ ��Ʈ���� �߰��ϼ̰ų� ȭ�鿡 ����Ͻ� ������ �����ôٸ� 
		// ������ ������ ���� ä���ִ� �۾��� �̺κп��� �Ͻø� �˴ϴ�.

		m_pSourceRecordSet->MoveNext();    // ���ڵ���� �������� �̵���ŵ�ϴ�.
		nCnt++;
	}

	*o_pSelectCnt = nCnt;
	m_pSourceRecordSet->Close();           // ���ڵ���� �ݾ��ݴϴ�.
	return ERR_NO_ERROR;
}

Err_t CGLogDataAceConnectUser::GLogDBInsert(int i_nCnt)
{
	try
	{
		// ���ν��� ����
		m_pDestCommand->CommandText = QDB_GLOG_TB_CONNECT_USER_INSERT;

		// ���ν����� �� ���ڰ� ����
		this->DBInsertParametersAppend(i_nCnt);

		// ���ν��� ����
		m_pDestCommand->Execute(NULL,NULL,adCmdStoredProc);
		
		// ���ڰ� �ʱ�ȭ
		this->DBInsertParametersDelete();
	}
	catch(_com_error& e)
	{
		// ���ڰ� �ʱ�ȭ
		this->DBInsertParametersDelete();
		return ErrorLog(L"CGLogDataAceConnectUser",e);
	}
	return ERR_NO_ERROR;
}

Err_t CGLogDataAceConnectUser::GLogDBDelete(int i_nCnt)
{
	try
	{
		// ���ν��� ����
		m_pSourceCommand->CommandText = QDB_GLOG_TB_CONNECT_USER_DELETE;

		// ���ν����� �� ���ڰ� ����
		m_pSourceCommand->Parameters->Append(m_pSourceCommand->CreateParameter(_bstr_t("UID"),adBigInt,adParamInput,8,_variant_t(m_arrAceConnectUserData[i_nCnt].UID)));

		// ���ν��� ����
		m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);

		// ���ڰ� �ʱ�ȭ
		m_pSourceCommand->Parameters->Delete(_bstr_t("UID"));
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataAceConnectUser",e);
	}
	return ERR_NO_ERROR;
}

void CGLogDataAceConnectUser::DBInsertParametersAppend(int i_nCnt)
{
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("c_date"),adDBTimeStamp,adParamInput,4,_variant_t(m_arrAceConnectUserData[i_nCnt].c_date)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("c_world"),adTinyInt,adParamInput,0,_variant_t(m_arrAceConnectUserData[i_nCnt].c_world)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("c_unique"),adInteger,adParamInput,0,_variant_t(m_arrAceConnectUserData[i_nCnt].c_unique)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("c_new"),adInteger,adParamInput,0,_variant_t(m_arrAceConnectUserData[i_nCnt].c_new)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("c_times"),adInteger,adParamInput,0,_variant_t(m_arrAceConnectUserData[i_nCnt].c_times)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("c_del"),adInteger,adParamInput,0,_variant_t(m_arrAceConnectUserData[i_nCnt].c_del)));

	VARIANT vGameServer;
	vGameServer.vt = VT_BSTR;
	vGameServer.bstrVal = _bstr_t(m_arrAceConnectUserData[i_nCnt].c_gameserver); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("c_gameserver"),adWChar,adParamInput,SIZE_MAX_GAME_SERVER_INFO+1,vGameServer));

	VARIANT vPublisher;
	vPublisher.vt = VT_BSTR;
	vPublisher.bstrVal = _bstr_t(m_arrAceConnectUserData[i_nCnt].c_publisher); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("c_publisher"),adWChar,adParamInput,SIZE_MAX_PUBLISHER+1,vPublisher));
}

void CGLogDataAceConnectUser::DBInsertParametersDelete()
{
	m_pDestCommand->Parameters->Delete(_bstr_t("c_date"));
	m_pDestCommand->Parameters->Delete(_bstr_t("c_world"));
	m_pDestCommand->Parameters->Delete(_bstr_t("c_unique"));
	m_pDestCommand->Parameters->Delete(_bstr_t("c_new"));
	m_pDestCommand->Parameters->Delete(_bstr_t("c_times"));
	m_pDestCommand->Parameters->Delete(_bstr_t("c_del"));
	m_pDestCommand->Parameters->Delete(_bstr_t("c_gameserver"));
	m_pDestCommand->Parameters->Delete(_bstr_t("c_publisher"));
}
