#include "StdAfx.h"
#include "GLogDataItemStateLog.h"

CGLogDataItemStateLog::CGLogDataItemStateLog(_RecordsetPtr i_pSourceRecordSet,_CommandPtr i_pSourceCommand,_RecordsetPtr i_pDestRecordSet,_CommandPtr i_pDestCommand)
{
	this->Init(i_pSourceRecordSet,i_pSourceCommand,i_pDestRecordSet,i_pDestCommand);
}

CGLogDataItemStateLog::~CGLogDataItemStateLog(void)
{
}

void CGLogDataItemStateLog::ResetData()
{
	util::zero(m_arrItemStateLog,sizeof(CItemStateLog)*GLOG_DATA_MAX_CNT);
}

Err_t CGLogDataItemStateLog::GLogProcessing()
{
	int SelectCnt = 0;
	Err_t Err =	this->GLogDBSelect(&SelectCnt);
	if(ERR_NO_ERROR != Err)
	{
		return this->ErrorProcessing(Err);
	}
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog CGLogDataItemStateLog Select Fin!!! ==============\r\n",TRUE);

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
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog CGLogDataItemStateLog Update Fin!!! ==============\r\n",TRUE);

	this->ResetData();
	return ERR_NO_ERROR;
}

Err_t CGLogDataItemStateLog::GLogDBSelect(int * o_pSelectCnt)
{
	m_pSourceCommand->CommandText = QDB_GLOG_ITEMSTATELOG_SELECT;
	try
	{
		m_pSourceRecordSet = m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);  // ����� select���� �����մϴ�.
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataItemStateLog",e);
	}

	if(m_pSourceRecordSet->EndOfFile)     
	{
		m_pSourceRecordSet->Close();   // ���ڵ���� �ݴ´�.(�׻� ���ڵ�� ���� �Ŀ� �ݾ��ִ� �۾��� �����ؾ� �մϴ�.)
		return ERR_DB_NO_DATA;
	}   

	int				nCnt = 0;
	CItemStateLog	tmpItemStateLog;
	// ��ȸ�� ���ڵ��� ������ ������ ���� �����͸� Ȯ���մϴ�.
	while(!m_pSourceRecordSet->EndOfFile)
	{
		tmpItemStateLog.Number = m_pSourceRecordSet->Fields->GetItem("Number")->GetValue();	// �ش� �ʵ带 ������ variant������ �����մϴ�.
		tmpItemStateLog.LogDate = m_pSourceRecordSet->Fields->GetItem("LogDate")->GetValue();
		tmpItemStateLog.LogType = m_pSourceRecordSet->Fields->GetItem("LogType")->GetValue();
		tmpItemStateLog.ItemSerial = m_pSourceRecordSet->Fields->GetItem("ItemSerial")->GetValue();
		tmpItemStateLog.Count = m_pSourceRecordSet->Fields->GetItem("Count")->GetValue();

		tmpItemStateLog.UserSerial = m_pSourceRecordSet->Fields->GetItem("UserSerial")->GetValue();
		tmpItemStateLog.CharSerial = m_pSourceRecordSet->Fields->GetItem("CharSerial")->GetValue();
		tmpItemStateLog.GameServerID = m_pSourceRecordSet->Fields->GetItem("GameServerID")->GetValue();

		memcpy(&m_arrItemStateLog[nCnt],&tmpItemStateLog,sizeof(CItemStateLog));
		util::zero(&tmpItemStateLog,sizeof(CItemStateLog));
		// ����Ʈ ��Ʈ���� �߰��ϼ̰ų� ȭ�鿡 ����Ͻ� ������ �����ôٸ� 
		// ������ ������ ���� ä���ִ� �۾��� �̺κп��� �Ͻø� �˴ϴ�.
		m_pSourceRecordSet->MoveNext();    // ���ڵ���� �������� �̵���ŵ�ϴ�.
		nCnt++;
	}

	*o_pSelectCnt = nCnt;
	m_pSourceRecordSet->Close();           // ���ڵ���� �ݾ��ݴϴ�.
	return ERR_NO_ERROR;
}

Err_t CGLogDataItemStateLog::GLogDBInsert(int i_nCnt)
{
	try
	{
		// ���ν��� ����
		m_pDestCommand->CommandText = QDB_GLOG_ITEMSTATELOG_INSERT;

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
		return ErrorLog(L"CGLogDataItemStateLog",e);
	}
	return ERR_NO_ERROR;
}

Err_t CGLogDataItemStateLog::GLogDBDelete(int i_nCnt)
{
	try
	{
		// ���ν��� ����
		m_pSourceCommand->CommandText = QDB_GLOG_ITEMSTATELOG_DELETE;

		// ���ν����� �� ���ڰ� ����
		m_pSourceCommand->Parameters->Append(m_pSourceCommand->CreateParameter(_bstr_t("Number"),adBigInt,adParamInput,8,_variant_t(m_arrItemStateLog[i_nCnt].Number)));

		// ���ν��� ����
		m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);
		// ���ڰ� �ʱ�ȭ
		m_pSourceCommand->Parameters->Delete(_bstr_t("Number"));
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataItemStateLog",e);
	}
	return ERR_NO_ERROR;
}

void CGLogDataItemStateLog::DBInsertParametersAppend(int i_nCnt)
{
	//m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Number"),adInteger,adParamInput,0,_variant_t(m_arrItemStateLog[i_nCnt].Number)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("LogDate"),adDate,adParamInput,4,_variant_t(m_arrItemStateLog[i_nCnt].LogDate)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("LogType"),adSmallInt,adParamInput,0,_variant_t(m_arrItemStateLog[i_nCnt].LogType)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ItemSerial"),adBigInt,adParamInput,8,_variant_t(m_arrItemStateLog[i_nCnt].ItemSerial)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Count"),adBigInt,adParamInput,8,_variant_t(m_arrItemStateLog[i_nCnt].Count)));

	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("UserSerial"),adBigInt,adParamInput,8,_variant_t(m_arrItemStateLog[i_nCnt].UserSerial)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("CharSerial"),adBigInt,adParamInput,8,_variant_t(m_arrItemStateLog[i_nCnt].CharSerial)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GameServerID"),adInteger,adParamInput,0,_variant_t(m_arrItemStateLog[i_nCnt].GameServerID)));
}

void CGLogDataItemStateLog::DBInsertParametersDelete()
{
	//m_pDestCommand->Parameters->Delete(_bstr_t("Number"));
	m_pDestCommand->Parameters->Delete(_bstr_t("LogDate"));		
	m_pDestCommand->Parameters->Delete(_bstr_t("LogType"));
	m_pDestCommand->Parameters->Delete(_bstr_t("ItemSerial"));
	m_pDestCommand->Parameters->Delete(_bstr_t("Count"));

	m_pDestCommand->Parameters->Delete(_bstr_t("UserSerial"));
	m_pDestCommand->Parameters->Delete(_bstr_t("CharSerial"));
	m_pDestCommand->Parameters->Delete(_bstr_t("GameServerID"));
}


