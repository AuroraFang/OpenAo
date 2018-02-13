#include "StdAfx.h"
#include "GLogDataConnectZoneUserCount.h"

CGLogDataConnectZoneUserCount::CGLogDataConnectZoneUserCount(_RecordsetPtr i_pSourceRecordSet,_CommandPtr i_pSourceCommand,_RecordsetPtr i_pDestRecordSet,_CommandPtr i_pDestCommand)
{
	this->Init(i_pSourceRecordSet,i_pSourceCommand,i_pDestRecordSet,i_pDestCommand);
}

CGLogDataConnectZoneUserCount::~CGLogDataConnectZoneUserCount(void)
{
}

void CGLogDataConnectZoneUserCount::ResetData()
{
	util::zero(m_arrConnectZoneUserCountData,sizeof(CConnectZoneUserCountData)*GLOG_DATA_MAX_CNT);
}

Err_t CGLogDataConnectZoneUserCount::GLogProcessing()
{
	int SelectCnt = 0;
	Err_t Err =	this->GLogDBSelect(&SelectCnt);
	if(ERR_NO_ERROR != Err)
	{
		return this->ErrorProcessing(Err);
	}
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog ConnectZoneUserCount Select Fin!!! ==============\r\n",TRUE);

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
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog ConnectZoneUserCount Update Fin!!! ==============\r\n",TRUE);

	this->ResetData();
	return ERR_NO_ERROR;
}

Err_t CGLogDataConnectZoneUserCount::GLogDBSelect(int * o_pSelectCnt)
{
	m_pSourceCommand->CommandText = QDB_GLOG_CONNECT_ZONE_USER_COUNT_SELECT;
	try
	{
		m_pSourceRecordSet = m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);  // ����� select���� �����մϴ�.
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataConnectZoneUserCount",e);
	}

	if(m_pSourceRecordSet->EndOfFile)     
	{
		m_pSourceRecordSet->Close();   // ���ڵ���� �ݴ´�.(�׻� ���ڵ�� ���� �Ŀ� �ݾ��ִ� �۾��� �����ؾ� �մϴ�.)
		return ERR_DB_NO_DATA;
	}   

	int				nCnt = 0;
	CConnectZoneUserCountData	tmpConnectZoneUserCountData;
	// ��ȸ�� ���ڵ��� ������ ������ ���� �����͸� Ȯ���մϴ�.
	while(!m_pSourceRecordSet->EndOfFile)
	{
		tmpConnectZoneUserCountData.UID	= m_pSourceRecordSet->Fields->GetItem("UID")->GetValue();	// �ش� �ʵ带 ������ variant������ �����մϴ�.
		tmpConnectZoneUserCountData.Date = m_pSourceRecordSet->Fields->GetItem("Date")->GetValue();
		LSTRNCPY_OLEDB(tmpConnectZoneUserCountData.GamePublisher,m_pSourceRecordSet->Fields->GetItem("GamePublisher")->GetValue());
		LSTRNCPY_OLEDB(tmpConnectZoneUserCountData.GameName,m_pSourceRecordSet->Fields->GetItem("GameName")->GetValue());
		LSTRNCPY_OLEDB(tmpConnectZoneUserCountData.GameServerName,m_pSourceRecordSet->Fields->GetItem("GameServerName")->GetValue());
		LSTRNCPY_OLEDB(tmpConnectZoneUserCountData.ZoneName,m_pSourceRecordSet->Fields->GetItem("ZoneName")->GetValue());
		tmpConnectZoneUserCountData.ZoneUserCount = m_pSourceRecordSet->Fields->GetItem("ZoneUserCount")->GetValue();
		memcpy(&m_arrConnectZoneUserCountData[nCnt],&tmpConnectZoneUserCountData,sizeof(CConnectZoneUserCountData));
		util::zero(&tmpConnectZoneUserCountData,sizeof(CConnectZoneUserCountData));
		// ����Ʈ ��Ʈ���� �߰��ϼ̰ų� ȭ�鿡 ����Ͻ� ������ �����ôٸ� 
		// ������ ������ ���� ä���ִ� �۾��� �̺κп��� �Ͻø� �˴ϴ�.
		m_pSourceRecordSet->MoveNext();    // ���ڵ���� �������� �̵���ŵ�ϴ�.
		nCnt++;
	}

	*o_pSelectCnt = nCnt;
	m_pSourceRecordSet->Close();           // ���ڵ���� �ݾ��ݴϴ�.
	return ERR_NO_ERROR;
}

Err_t CGLogDataConnectZoneUserCount::GLogDBInsert(int i_nCnt)
{
	try
	{
		// ���ν��� ����
		m_pDestCommand->CommandText = QDB_GLOG_CONNECT_ZONE_USER_COUNT_INSERT;

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
		return ErrorLog(L"CGLogDataConnectZoneUserCount",e);
	}
	return ERR_NO_ERROR;
}

Err_t CGLogDataConnectZoneUserCount::GLogDBDelete(int i_nCnt)
{
	try
	{
		// ���ν��� ����
		m_pSourceCommand->CommandText = QDB_GLOG_CONNECT_ZONE_USER_COUNT_DELETE;

		// ���ν����� �� ���ڰ� ����
		m_pSourceCommand->Parameters->Append(m_pSourceCommand->CreateParameter(_bstr_t("UID"),adBigInt,adParamInput,8,_variant_t(m_arrConnectZoneUserCountData[i_nCnt].UID)));

		// ���ν��� ����
		m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);
		// ���ڰ� �ʱ�ȭ
		m_pSourceCommand->Parameters->Delete(_bstr_t("UID"));
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataConnectZoneUserCount",e);
	}
	return ERR_NO_ERROR;
}

void CGLogDataConnectZoneUserCount::DBInsertParametersAppend(int i_nCnt)
{
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Date"),adDate,adParamInput,8,_variant_t(m_arrConnectZoneUserCountData[i_nCnt].Date)));
	VARIANT vGamePublisher;
	vGamePublisher.vt = VT_BSTR;
	vGamePublisher.bstrVal = _bstr_t(m_arrConnectZoneUserCountData[i_nCnt].GamePublisher); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GamePublisher"),adWChar,adParamInput,SIZE_MAX_GAME_PUBLISHER_NAME+1,vGamePublisher));
	VARIANT vGameName;
	vGameName.vt = VT_BSTR;
	vGameName.bstrVal = _bstr_t(m_arrConnectZoneUserCountData[i_nCnt].GameName); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GameName"),adWChar,adParamInput,SIZE_MAX_GAME_NAME+1,vGameName));
	VARIANT vGameServerName;
	vGameServerName.vt = VT_BSTR;
	vGameServerName.bstrVal = _bstr_t(m_arrConnectZoneUserCountData[i_nCnt].GameServerName); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GameServerName"),adWChar,adParamInput,SIZE_MAX_GAME_SERVER_NAME+1,vGameServerName));
	VARIANT vZoneName;
	vZoneName.vt = VT_BSTR;
	vZoneName.bstrVal = _bstr_t(m_arrConnectZoneUserCountData[i_nCnt].ZoneName); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ZoneName"),adWChar,adParamInput,SIZE_MAX_ZONE_NAME+1,vZoneName));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ZoneUserCount"),adInteger,adParamInput,0,_variant_t(m_arrConnectZoneUserCountData[i_nCnt].ZoneUserCount)));
}

void CGLogDataConnectZoneUserCount::DBInsertParametersDelete()
{
	m_pDestCommand->Parameters->Delete(_bstr_t("Date"));
	m_pDestCommand->Parameters->Delete(_bstr_t("GamePublisher"));		
	m_pDestCommand->Parameters->Delete(_bstr_t("GameName"));
	m_pDestCommand->Parameters->Delete(_bstr_t("GameServerName"));
	m_pDestCommand->Parameters->Delete(_bstr_t("ZoneName"));
	m_pDestCommand->Parameters->Delete(_bstr_t("ZoneUserCount"));
}
