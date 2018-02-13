#include "StdAfx.h"
#include "GLogDataCharData.h"

CGLogDataCharData::CGLogDataCharData(_RecordsetPtr i_pSourceRecordSet,_CommandPtr i_pSourceCommand,_RecordsetPtr i_pDestRecordSet,_CommandPtr i_pDestCommand)
{
	this->Init(i_pSourceRecordSet,i_pSourceCommand,i_pDestRecordSet,i_pDestCommand);
}

CGLogDataCharData::~CGLogDataCharData(void)
{
}

void CGLogDataCharData::ResetData()
{
	util::zero(m_arrCharData,sizeof(CCharData)*GLOG_DATA_MAX_CNT);
}

Err_t CGLogDataCharData::GLogProcessing()
{
	int SelectCnt = 0;
	Err_t Err =	this->GLogDBSelect(&SelectCnt);
	if(ERR_NO_ERROR != Err)
	{
		return this->ErrorProcessing(Err);
	}
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog CGLogDataCharData Select Fin!!! ==============\r\n",TRUE);

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
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog CGLogDataCharData Update Fin!!! ==============\r\n",TRUE);

	this->ResetData();
	return ERR_NO_ERROR;
}

Err_t CGLogDataCharData::GLogDBSelect(int * o_pSelectCnt)
{
	m_pSourceCommand->CommandText = QDB_GLOG_CHARDATA_SELECT;
	try
	{
		m_pSourceRecordSet = m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);  // ����� select���� �����մϴ�.
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataCharData",e);
	}

	if(m_pSourceRecordSet->EndOfFile)     
	{
		m_pSourceRecordSet->Close();   // ���ڵ���� �ݴ´�.(�׻� ���ڵ�� ���� �Ŀ� �ݾ��ִ� �۾��� �����ؾ� �մϴ�.)
		return ERR_DB_NO_DATA;
	}   

	int				nCnt = 0;
	CCharData	tmpCharData;
	// ��ȸ�� ���ڵ��� ������ ������ ���� �����͸� Ȯ���մϴ�.
	while(!m_pSourceRecordSet->EndOfFile)
	{
		tmpCharData.Number	= m_pSourceRecordSet->Fields->GetItem("Number")->GetValue();	// �ش� �ʵ带 ������ variant������ �����մϴ�.
		tmpCharData.LogDate = m_pSourceRecordSet->Fields->GetItem("LogDate")->GetValue();
		tmpCharData.LogType = m_pSourceRecordSet->Fields->GetItem("LogType")->GetValue();
		tmpCharData.UserSerial = m_pSourceRecordSet->Fields->GetItem("UserSerial")->GetValue();
		tmpCharData.CharSerial = m_pSourceRecordSet->Fields->GetItem("CharSerial")->GetValue();
		tmpCharData.Class = m_pSourceRecordSet->Fields->GetItem("Class")->GetValue();
		tmpCharData.Lv = m_pSourceRecordSet->Fields->GetItem("Lv")->GetValue();
		tmpCharData.Exp = m_pSourceRecordSet->Fields->GetItem("Exp")->GetValue();
		tmpCharData.GameMoney = m_pSourceRecordSet->Fields->GetItem("GameMoney")->GetValue();
		tmpCharData.Playtime = m_pSourceRecordSet->Fields->GetItem("Playtime")->GetValue();
		tmpCharData.GameServerID = m_pSourceRecordSet->Fields->GetItem("GameServerID")->GetValue();

		memcpy(&m_arrCharData[nCnt],&tmpCharData,sizeof(CCharData));
		util::zero(&tmpCharData,sizeof(CCharData));
		// ����Ʈ ��Ʈ���� �߰��ϼ̰ų� ȭ�鿡 ����Ͻ� ������ �����ôٸ� 
		// ������ ������ ���� ä���ִ� �۾��� �̺κп��� �Ͻø� �˴ϴ�.
		m_pSourceRecordSet->MoveNext();    // ���ڵ���� �������� �̵���ŵ�ϴ�.
		nCnt++;
	}

	*o_pSelectCnt = nCnt;
	m_pSourceRecordSet->Close();           // ���ڵ���� �ݾ��ݴϴ�.
	return ERR_NO_ERROR;
}

Err_t CGLogDataCharData::GLogDBInsert(int i_nCnt)
{
	try
	{
		// ���ν��� ����
		m_pDestCommand->CommandText = QDB_GLOG_CHARDATA_INSERT;

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
		return ErrorLog(L"CGLogDataCharData",e);
	}
	return ERR_NO_ERROR;
}

Err_t CGLogDataCharData::GLogDBDelete(int i_nCnt)
{
	try
	{
		// ���ν��� ����
		m_pSourceCommand->CommandText = QDB_GLOG_CHARDATA_DELETE;

		// ���ν����� �� ���ڰ� ����
		m_pSourceCommand->Parameters->Append(m_pSourceCommand->CreateParameter(_bstr_t("Number"),adBigInt,adParamInput,8,_variant_t(m_arrCharData[i_nCnt].Number)));

		// ���ν��� ����
		m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);
		// ���ڰ� �ʱ�ȭ
		m_pSourceCommand->Parameters->Delete(_bstr_t("Number"));
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataCharacterPlayTime",e);
	}
	return ERR_NO_ERROR;
}

void CGLogDataCharData::DBInsertParametersAppend(int i_nCnt)
{
	//m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Number"),adInteger,adParamInput,8,_variant_t(m_arrCharData[i_nCnt].Number)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("LogDate"),adDate,adParamInput,4,_variant_t(m_arrCharData[i_nCnt].LogDate)));
	//m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("LogDate"),adDBTime,adParamInput,8,_variant_t(m_arrCharData[i_nCnt].LogDate)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("LogType"),adSmallInt,adParamInput,0,_variant_t(m_arrCharData[i_nCnt].LogType)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("UserSerial"),adBigInt,adParamInput,8,_variant_t(m_arrCharData[i_nCnt].UserSerial)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("CharSerial"),adBigInt,adParamInput,8,_variant_t(m_arrCharData[i_nCnt].CharSerial)));

	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Class"),adSmallInt,adParamInput,0,_variant_t(m_arrCharData[i_nCnt].Class)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Lv"),adSmallInt,adParamInput,0,_variant_t(m_arrCharData[i_nCnt].Lv)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Exp"),adBigInt,adParamInput,8,_variant_t(m_arrCharData[i_nCnt].Exp)));
	/*m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GameMoney"),adBigInt,adParamInput,8,_variant_t(m_arrCharData[i_nCnt].GameMoney)));*/
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Playtime"),adBigInt,adParamInput,8,_variant_t(m_arrCharData[i_nCnt].Playtime)));

	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GameServerID"),adInteger,adParamInput,0,_variant_t(m_arrCharData[i_nCnt].GameServerID)));

}

void CGLogDataCharData::DBInsertParametersDelete()
{
	//m_pDestCommand->Parameters->Delete(_bstr_t("Number"));
	m_pDestCommand->Parameters->Delete(_bstr_t("LogDate"));		
	m_pDestCommand->Parameters->Delete(_bstr_t("LogType"));
	m_pDestCommand->Parameters->Delete(_bstr_t("UserSerial"));
	m_pDestCommand->Parameters->Delete(_bstr_t("CharSerial"));

	m_pDestCommand->Parameters->Delete(_bstr_t("Class"));
	m_pDestCommand->Parameters->Delete(_bstr_t("Lv"));
	m_pDestCommand->Parameters->Delete(_bstr_t("Exp"));
/*	m_pDestCommand->Parameters->Delete(_bstr_t("GameMoney"));*/
	m_pDestCommand->Parameters->Delete(_bstr_t("Playtime"));

	m_pDestCommand->Parameters->Delete(_bstr_t("GameServerID"));
}
