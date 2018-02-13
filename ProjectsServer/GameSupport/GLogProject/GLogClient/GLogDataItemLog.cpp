#include "StdAfx.h"
#include "GLogDataItemLog.h"

CGLogDataItemLog::CGLogDataItemLog(_RecordsetPtr i_pSourceRecordSet,_CommandPtr i_pSourceCommand,_RecordsetPtr i_pDestRecordSet,_CommandPtr i_pDestCommand)
{
	this->Init(i_pSourceRecordSet,i_pSourceCommand,i_pDestRecordSet,i_pDestCommand);
}

CGLogDataItemLog::~CGLogDataItemLog(void)
{
}

void CGLogDataItemLog::ResetData()
{
	util::zero(m_arrItemLog,sizeof(CItemLog)*GLOG_DATA_MAX_CNT);
}

Err_t CGLogDataItemLog::GLogProcessing()
{
	int SelectCnt = 0;
	Err_t Err =	this->GLogDBSelect(&SelectCnt);
	if(ERR_NO_ERROR != Err)
	{
		return this->ErrorProcessing(Err);
	}
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog CGLogDataItemLog Select Fin!!! ==============\r\n",TRUE);

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
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog CGLogDataItemLog Update Fin!!! ==============\r\n",TRUE);

	this->ResetData();
	return ERR_NO_ERROR;
}

Err_t CGLogDataItemLog::GLogDBSelect(int * o_pSelectCnt)
{
	m_pSourceCommand->CommandText = QDB_GLOG_ITEMLOG_SELECT;
	try
	{
		m_pSourceRecordSet = m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);  // ����� select���� �����մϴ�.
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataItemLog",e);
	}

	if(m_pSourceRecordSet->EndOfFile)     
	{
		m_pSourceRecordSet->Close();   // ���ڵ���� �ݴ´�.(�׻� ���ڵ�� ���� �Ŀ� �ݾ��ִ� �۾��� �����ؾ� �մϴ�.)
		return ERR_DB_NO_DATA;
	}   

	int				nCnt = 0;
	CItemLog	tmpItemLog;
	// ��ȸ�� ���ڵ��� ������ ������ ���� �����͸� Ȯ���մϴ�.
	while(!m_pSourceRecordSet->EndOfFile)
	{
		tmpItemLog.Number = m_pSourceRecordSet->Fields->GetItem("Number")->GetValue();	// �ش� �ʵ带 ������ variant������ �����մϴ�.
		tmpItemLog.LogDate = m_pSourceRecordSet->Fields->GetItem("LogDate")->GetValue();
		tmpItemLog.LogType = m_pSourceRecordSet->Fields->GetItem("LogType")->GetValue();
		tmpItemLog.ItemSerial = m_pSourceRecordSet->Fields->GetItem("ItemSerial")->GetValue();
		tmpItemLog.Count1 = m_pSourceRecordSet->Fields->GetItem("Count1")->GetValue();

		tmpItemLog.Count2 = m_pSourceRecordSet->Fields->GetItem("Count2")->GetValue();
		tmpItemLog.CharSerial = m_pSourceRecordSet->Fields->GetItem("CharSerial")->GetValue();
		tmpItemLog.SCharSerial = m_pSourceRecordSet->Fields->GetItem("SCharSerial")->GetValue();
		tmpItemLog.GameServerID = m_pSourceRecordSet->Fields->GetItem("GameServerID")->GetValue();

		memcpy(&m_arrItemLog[nCnt],&tmpItemLog,sizeof(CItemLog));
		util::zero(&tmpItemLog,sizeof(CItemLog));
		// ����Ʈ ��Ʈ���� �߰��ϼ̰ų� ȭ�鿡 ����Ͻ� ������ �����ôٸ� 
		// ������ ������ ���� ä���ִ� �۾��� �̺κп��� �Ͻø� �˴ϴ�.
		m_pSourceRecordSet->MoveNext();    // ���ڵ���� �������� �̵���ŵ�ϴ�.
		nCnt++;
	}

	*o_pSelectCnt = nCnt;
	m_pSourceRecordSet->Close();           // ���ڵ���� �ݾ��ݴϴ�.
	return ERR_NO_ERROR;
}

Err_t CGLogDataItemLog::GLogDBInsert(int i_nCnt)
{
	try
	{
		// ���ν��� ����
		m_pDestCommand->CommandText = QDB_GLOG_ITEMLOG_INSERT;

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
		return ErrorLog(L"CGLogDataItemLog",e);
	}
	return ERR_NO_ERROR;
}

Err_t CGLogDataItemLog::GLogDBDelete(int i_nCnt)
{
	try
	{
		// ���ν��� ����
		m_pSourceCommand->CommandText = QDB_GLOG_ITEMLOG_DELETE;

		// ���ν����� �� ���ڰ� ����
		m_pSourceCommand->Parameters->Append(m_pSourceCommand->CreateParameter(_bstr_t("Number"),adBigInt,adParamInput,8,_variant_t(m_arrItemLog[i_nCnt].Number)));

		// ���ν��� ����
		m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);
		// ���ڰ� �ʱ�ȭ
		m_pSourceCommand->Parameters->Delete(_bstr_t("Number"));
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataItemLog",e);
	}
	return ERR_NO_ERROR;
}

void CGLogDataItemLog::DBInsertParametersAppend(int i_nCnt)
{
	//m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Number"),adInteger,adParamInput,0,_variant_t(m_arrItemLog[i_nCnt].Number)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("LogDate"),adDate,adParamInput,4,_variant_t(m_arrItemLog[i_nCnt].LogDate)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("LogType"),adSmallInt,adParamInput,0,_variant_t(m_arrItemLog[i_nCnt].LogType)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ItemSerial"),adBigInt,adParamInput,8,_variant_t(m_arrItemLog[i_nCnt].ItemSerial)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Count1"),adBigInt,adParamInput,8,_variant_t(m_arrItemLog[i_nCnt].Count1)));

	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Count2"),adBigInt,adParamInput,8,_variant_t(m_arrItemLog[i_nCnt].Count2)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("CharSerial"),adBigInt,adParamInput,8,_variant_t(m_arrItemLog[i_nCnt].CharSerial)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("SCharSerial"),adBigInt,adParamInput,8,_variant_t(m_arrItemLog[i_nCnt].SCharSerial)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GameServerID"),adInteger,adParamInput,0,_variant_t(m_arrItemLog[i_nCnt].GameServerID)));

}

void CGLogDataItemLog::DBInsertParametersDelete()
{
	//m_pDestCommand->Parameters->Delete(_bstr_t("Number"));
	m_pDestCommand->Parameters->Delete(_bstr_t("LogDate"));		
	m_pDestCommand->Parameters->Delete(_bstr_t("LogType"));
	m_pDestCommand->Parameters->Delete(_bstr_t("ItemSerial"));
	m_pDestCommand->Parameters->Delete(_bstr_t("Count1"));

	m_pDestCommand->Parameters->Delete(_bstr_t("Count2"));
	m_pDestCommand->Parameters->Delete(_bstr_t("CharSerial"));
	m_pDestCommand->Parameters->Delete(_bstr_t("SCharSerial"));
	m_pDestCommand->Parameters->Delete(_bstr_t("GameServerID"));

}


