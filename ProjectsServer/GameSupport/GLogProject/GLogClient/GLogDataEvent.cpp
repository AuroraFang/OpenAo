#include "StdAfx.h"
#include "GLogDataEvent.h"


CGLogDataEvent::CGLogDataEvent(_RecordsetPtr i_pSourceRecordSet,_CommandPtr i_pSourceCommand,_RecordsetPtr i_pDestRecordSet,_CommandPtr i_pDestCommand)
{
	this->Init(i_pSourceRecordSet,i_pSourceCommand,i_pDestRecordSet,i_pDestCommand);
}

CGLogDataEvent::~CGLogDataEvent(void)
{
}

void CGLogDataEvent::ResetData()
{
	util::zero(m_arrEventData,sizeof(CEventData)*GLOG_DATA_MAX_CNT);
}

Err_t CGLogDataEvent::GLogProcessing()
{
	int SelectCnt = 0;
	Err_t Err =	this->GLogDBSelect(&SelectCnt);
	if(ERR_NO_ERROR != Err)
	{
		return this->ErrorProcessing(Err);
	}
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog Event Select Fin!!! ==============\r\n",TRUE);

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
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog Event Update Fin!!! ==============\r\n",TRUE);

	this->ResetData();
	return ERR_NO_ERROR;
}

Err_t CGLogDataEvent::GLogDBSelect(int * o_pSelectCnt)
{
	m_pSourceCommand->CommandText = QDB_GLOG_EVENT_SELECT;
	try
	{
		m_pSourceRecordSet = m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);  // ����� select���� �����մϴ�.
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataEvent",e);
	}

	if(m_pSourceRecordSet->EndOfFile)     
	{
		m_pSourceRecordSet->Close();   // ���ڵ���� �ݴ´�.(�׻� ���ڵ�� ���� �Ŀ� �ݾ��ִ� �۾��� �����ؾ� �մϴ�.)
		return ERR_DB_NO_DATA;
	}   

	int				nCnt = 0;
	CEventData	tmpEventData;
	// ��ȸ�� ���ڵ��� ������ ������ ���� �����͸� Ȯ���մϴ�.
	while(!m_pSourceRecordSet->EndOfFile)
	{
		tmpEventData.UID	= m_pSourceRecordSet->Fields->GetItem("UID")->GetValue();	// �ش� �ʵ带 ������ variant������ �����մϴ�.
		LSTRNCPY_OLEDB(tmpEventData.GamePublisher,m_pSourceRecordSet->Fields->GetItem("GamePublisher")->GetValue());
		LSTRNCPY_OLEDB(tmpEventData.GameName,m_pSourceRecordSet->Fields->GetItem("GameName")->GetValue());
		LSTRNCPY_OLEDB(tmpEventData.GameServerName,m_pSourceRecordSet->Fields->GetItem("GameServerName")->GetValue());
		tmpEventData.StartDate = m_pSourceRecordSet->Fields->GetItem("StartDate")->GetValue();
		tmpEventData.EndDate = m_pSourceRecordSet->Fields->GetItem("EndDate")->GetValue();
		LSTRNCPY_OLEDB(tmpEventData.Description,m_pSourceRecordSet->Fields->GetItem("Description")->GetValue());
		memcpy(&m_arrEventData[nCnt],&tmpEventData,sizeof(CEventData));
		util::zero(&tmpEventData,sizeof(CEventData));
		// ����Ʈ ��Ʈ���� �߰��ϼ̰ų� ȭ�鿡 ����Ͻ� ������ �����ôٸ� 
		// ������ ������ ���� ä���ִ� �۾��� �̺κп��� �Ͻø� �˴ϴ�.
		m_pSourceRecordSet->MoveNext();    // ���ڵ���� �������� �̵���ŵ�ϴ�.
		nCnt++;
	}

	*o_pSelectCnt = nCnt;
	m_pSourceRecordSet->Close();           // ���ڵ���� �ݾ��ݴϴ�.
	return ERR_NO_ERROR;
}

Err_t CGLogDataEvent::GLogDBInsert(int i_nCnt)
{
	try
	{
		// ���ν��� ����
		m_pDestCommand->CommandText = QDB_GLOG_EVENT_INSERT;

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
		return ErrorLog(L"CGLogDataEvent",e);
	}
	return ERR_NO_ERROR;
}

Err_t CGLogDataEvent::GLogDBDelete(int i_nCnt)
{
	try
	{
		// ���ν��� ����
		m_pSourceCommand->CommandText = QDB_GLOG_EVENT_DELETE;

		// ���ν����� �� ���ڰ� ����
		m_pSourceCommand->Parameters->Append(m_pSourceCommand->CreateParameter(_bstr_t("UID"),adBigInt,adParamInput,8,_variant_t(m_arrEventData[i_nCnt].UID)));

		// ���ν��� ����
		m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);
		// ���ڰ� �ʱ�ȭ
		m_pSourceCommand->Parameters->Delete(_bstr_t("UID"));
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataEvent",e);
	}
	return ERR_NO_ERROR;
}

void CGLogDataEvent::DBInsertParametersAppend(int i_nCnt)
{
	VARIANT vGamePublisher;
	vGamePublisher.vt = VT_BSTR;
	vGamePublisher.bstrVal = _bstr_t(m_arrEventData[i_nCnt].GamePublisher); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GamePublisher"),adWChar,adParamInput,SIZE_MAX_GAME_PUBLISHER_NAME+1,vGamePublisher));
	VARIANT vGameName;
	vGameName.vt = VT_BSTR;
	vGameName.bstrVal = _bstr_t(m_arrEventData[i_nCnt].GameName); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GameName"),adWChar,adParamInput,SIZE_MAX_GAME_NAME+1,vGameName));
	VARIANT vGameServerName;
	vGameServerName.vt = VT_BSTR;
	vGameServerName.bstrVal = _bstr_t(m_arrEventData[i_nCnt].GameServerName); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GameServerName"),adWChar,adParamInput,SIZE_MAX_GAME_SERVER_NAME+1,vGameServerName));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("StartDate"),adDate,adParamInput,8,_variant_t(m_arrEventData[i_nCnt].StartDate)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("EndDate"),adDate,adParamInput,8,_variant_t(m_arrEventData[i_nCnt].EndDate)));
	VARIANT vDescription;
	vDescription.vt = VT_BSTR;
	vDescription.bstrVal = _bstr_t(m_arrEventData[i_nCnt].Description); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Description"),adWChar,adParamInput,SIZE_MAX_DESCRIPTION+1,vDescription));
}

void CGLogDataEvent::DBInsertParametersDelete()
{
	m_pDestCommand->Parameters->Delete(_bstr_t("GamePublisher"));
	m_pDestCommand->Parameters->Delete(_bstr_t("GameName"));		
	m_pDestCommand->Parameters->Delete(_bstr_t("GameServerName"));
	m_pDestCommand->Parameters->Delete(_bstr_t("StartDate"));
	m_pDestCommand->Parameters->Delete(_bstr_t("EndDate"));
	m_pDestCommand->Parameters->Delete(_bstr_t("Description"));
}
