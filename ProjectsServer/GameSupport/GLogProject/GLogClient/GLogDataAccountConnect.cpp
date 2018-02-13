#include "StdAfx.h"
#include "GLogDataAccountConnect.h"

CGLogDataAccountConnect::CGLogDataAccountConnect(_RecordsetPtr i_pSourceRecordSet,_CommandPtr i_pSourceCommand,_RecordsetPtr i_pDestRecordSet,_CommandPtr i_pDestCommand)
{
	this->Init(i_pSourceRecordSet,i_pSourceCommand,i_pDestRecordSet,i_pDestCommand);
}

CGLogDataAccountConnect::~CGLogDataAccountConnect(void)
{
}

void CGLogDataAccountConnect::ResetData()
{
	util::zero(m_arrAccountConnectData,sizeof(CAccountConnectData)*GLOG_DATA_MAX_CNT);
}

Err_t CGLogDataAccountConnect::GLogProcessing()
{
	int SelectCnt = 0;
	Err_t Err =	this->GLogDBSelect(&SelectCnt);
	if(ERR_NO_ERROR != Err)
	{
		return this->ErrorProcessing(Err);
	}
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog CGLogDataAccountConnect Select Fin!!! ==============\r\n",TRUE);

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
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog CGLogDataAccountConnect Update Fin!!! ==============\r\n",TRUE);

	this->ResetData();
	return ERR_NO_ERROR;
}

Err_t CGLogDataAccountConnect::GLogDBSelect(int * o_pSelectCnt)
{
	m_pSourceCommand->CommandText = QDB_GLOG_ACCOUNTCONNECT_SELECT;
	try
	{
		m_pSourceRecordSet = m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);  // ����� select���� �����մϴ�.
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataAccountConnect",e);
	}

	if(m_pSourceRecordSet->EndOfFile)     
	{
		m_pSourceRecordSet->Close();   // ���ڵ���� �ݴ´�.(�׻� ���ڵ�� ���� �Ŀ� �ݾ��ִ� �۾��� �����ؾ� �մϴ�.)
		return ERR_DB_NO_DATA;
	}   

	int				nCnt = 0;
	CAccountConnectData	tmpAccountConnectData;
	// ��ȸ�� ���ڵ��� ������ ������ ���� �����͸� Ȯ���մϴ�.
	while(!m_pSourceRecordSet->EndOfFile)
	{
		tmpAccountConnectData.Number	= m_pSourceRecordSet->Fields->GetItem("Number")->GetValue();	// �ش� �ʵ带 ������ variant������ �����մϴ�.
		tmpAccountConnectData.LogDate = m_pSourceRecordSet->Fields->GetItem("LogDate")->GetValue();
		tmpAccountConnectData.LogType = m_pSourceRecordSet->Fields->GetItem("LogType")->GetValue();
		tmpAccountConnectData.UserSerial = m_pSourceRecordSet->Fields->GetItem("UserSerial")->GetValue();
		tmpAccountConnectData.IP = m_pSourceRecordSet->Fields->GetItem("IP")->GetValue();
		tmpAccountConnectData.GameServerID = m_pSourceRecordSet->Fields->GetItem("GameServerID")->GetValue();
		
		memcpy(&m_arrAccountConnectData[nCnt],&tmpAccountConnectData,sizeof(CAccountConnectData));
		util::zero(&tmpAccountConnectData,sizeof(CAccountConnectData));
		// ����Ʈ ��Ʈ���� �߰��ϼ̰ų� ȭ�鿡 ����Ͻ� ������ �����ôٸ� 
		// ������ ������ ���� ä���ִ� �۾��� �̺κп��� �Ͻø� �˴ϴ�.
		m_pSourceRecordSet->MoveNext();    // ���ڵ���� �������� �̵���ŵ�ϴ�.
		nCnt++;
	}

	*o_pSelectCnt = nCnt;
	m_pSourceRecordSet->Close();           // ���ڵ���� �ݾ��ݴϴ�.
	return ERR_NO_ERROR;
}

Err_t CGLogDataAccountConnect::GLogDBInsert(int i_nCnt)
{
	try
	{
		// ���ν��� ����
		m_pDestCommand->CommandText = QDB_GLOG_ACCOUNTCONNECT_INSERT;

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
		return ErrorLog(L"CGLogDataAccountConnect",e);
	}
	return ERR_NO_ERROR;
}

Err_t CGLogDataAccountConnect::GLogDBDelete(int i_nCnt)
{
	try
	{
		// ���ν��� ����
		m_pSourceCommand->CommandText = QDB_GLOG_ACCOUNTCONNECT_DELETE;

		// ���ν����� �� ���ڰ� ����
		m_pSourceCommand->Parameters->Append(m_pSourceCommand->CreateParameter(_bstr_t("Number"),adBigInt,adParamInput,8,_variant_t(m_arrAccountConnectData[i_nCnt].Number)));

		// ���ν��� ����
		m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);
		// ���ڰ� �ʱ�ȭ
		m_pSourceCommand->Parameters->Delete(_bstr_t("Number"));
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataAccountConnect",e);
	}
	return ERR_NO_ERROR;
}

void CGLogDataAccountConnect::DBInsertParametersAppend(int i_nCnt)
{
	//m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Number"),adInteger,adParamInput,0,_variant_t(m_arrAccountConnectData[i_nCnt].Number)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("LogDate"),adDate,adParamInput,4,_variant_t(m_arrAccountConnectData[i_nCnt].LogDate)));
	//m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("LogDate"),adDBTimeStamp,adParamInput,8,_variant_t(m_arrAccountConnectData[i_nCnt].LogDate)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("LogType"),adSmallInt,adParamInput,0,_variant_t(m_arrAccountConnectData[i_nCnt].LogType)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("UserSerial"),adBigInt,adParamInput,8,_variant_t(m_arrAccountConnectData[i_nCnt].UserSerial)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("IP"),adBigInt,adParamInput,8,_variant_t(m_arrAccountConnectData[i_nCnt].IP)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GameServerID"),adInteger,adParamInput,0,_variant_t(m_arrAccountConnectData[i_nCnt].GameServerID)));
}

void CGLogDataAccountConnect::DBInsertParametersDelete()	
{
	//m_pDestCommand->Parameters->Delete(_bstr_t("Number"));
	m_pDestCommand->Parameters->Delete(_bstr_t("LogDate"));		
	m_pDestCommand->Parameters->Delete(_bstr_t("LogType"));
	m_pDestCommand->Parameters->Delete(_bstr_t("UserSerial"));
	m_pDestCommand->Parameters->Delete(_bstr_t("IP"));
	m_pDestCommand->Parameters->Delete(_bstr_t("GameServerID"));
}


