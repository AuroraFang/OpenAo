//////////////////////////////////
// 2011-12-12 by hskim, GLog 2��

#include "StdAfx.h"
#include "GLogDataAceConcurrentUser.h"

CGLogDataAceConcurrentUser::CGLogDataAceConcurrentUser(_RecordsetPtr i_pSourceRecordSet,_CommandPtr i_pSourceCommand,_RecordsetPtr i_pDestRecordSet,_CommandPtr i_pDestCommand)
{
	this->Init(i_pSourceRecordSet,i_pSourceCommand,i_pDestRecordSet,i_pDestCommand);
}

CGLogDataAceConcurrentUser::~CGLogDataAceConcurrentUser(void)
{
}

void CGLogDataAceConcurrentUser::ResetData()
{
	util::zero(m_arrAceConcurrentUserData,sizeof(CAceConcurrentUserData)*GLOG_DATA_MAX_CNT);
}

Err_t CGLogDataAceConcurrentUser::GLogProcessing()
{
	int SelectCnt = 0;
	Err_t Err =	this->GLogDBSelect(&SelectCnt);
	if(ERR_NO_ERROR != Err)
	{
		return this->ErrorProcessing(Err);
	}
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog AceConcurrentUser Select Fin!!! ==============\r\n",TRUE);

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
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog AceConcurrentUser Update Fin!!! ==============\r\n",TRUE);

	this->ResetData();
	return ERR_NO_ERROR;
}

Err_t CGLogDataAceConcurrentUser::GLogDBSelect(int * o_pSelectCnt)
{
	m_pSourceCommand->CommandText = QDB_GLOG_TB_CONCURRENT_USER_SELECT;
	try
	{
		m_pSourceRecordSet = m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);  // ����� select���� �����մϴ�.
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataAceConcurrentUser",e);
	}

	if(m_pSourceRecordSet->EndOfFile)     
	{
		m_pSourceRecordSet->Close();   // ���ڵ���� �ݴ´�.(�׻� ���ڵ�� ���� �Ŀ� �ݾ��ִ� �۾��� �����ؾ� �մϴ�.)
		return ERR_DB_NO_DATA;
	}   

	int				nCnt = 0;
	CAceConcurrentUserData	tmpAceConcurrentUserData;
	// ��ȸ�� ���ڵ��� ������ ������ ���� �����͸� Ȯ���մϴ�.

	while(!m_pSourceRecordSet->EndOfFile)
	{
		tmpAceConcurrentUserData.UID = m_pSourceRecordSet->Fields->GetItem("UID")->GetValue();
		tmpAceConcurrentUserData.cu_world = m_pSourceRecordSet->Fields->GetItem("cu_world")->GetValue();
		tmpAceConcurrentUserData.cu_date = m_pSourceRecordSet->Fields->GetItem("cu_date")->GetValue();
		tmpAceConcurrentUserData.cu_count = m_pSourceRecordSet->Fields->GetItem("cu_count")->GetValue();
		LSTRNCPY_OLEDB(tmpAceConcurrentUserData.cu_publisher,m_pSourceRecordSet->Fields->GetItem("cu_publisher")->GetValue());
		LSTRNCPY_OLEDB(tmpAceConcurrentUserData.cu_gameServer,m_pSourceRecordSet->Fields->GetItem("cu_gameServer")->GetValue());

		memcpy(&m_arrAceConcurrentUserData[nCnt],&tmpAceConcurrentUserData,sizeof(CAceConcurrentUserData));
		util::zero(&tmpAceConcurrentUserData,sizeof(CAceConcurrentUserData));

		// ����Ʈ ��Ʈ���� �߰��ϼ̰ų� ȭ�鿡 ����Ͻ� ������ �����ôٸ� 
		// ������ ������ ���� ä���ִ� �۾��� �̺κп��� �Ͻø� �˴ϴ�.

		m_pSourceRecordSet->MoveNext();    // ���ڵ���� �������� �̵���ŵ�ϴ�.
		nCnt++;
	}

	*o_pSelectCnt = nCnt;
	m_pSourceRecordSet->Close();           // ���ڵ���� �ݾ��ݴϴ�.
	return ERR_NO_ERROR;
}

Err_t CGLogDataAceConcurrentUser::GLogDBInsert(int i_nCnt)
{
	try
	{
		// ���ν��� ����
		m_pDestCommand->CommandText = QDB_GLOG_TB_CONCURRENT_USER_INSERT;

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
		return ErrorLog(L"CGLogDataAceConcurrentUser",e);
	}
	return ERR_NO_ERROR;
}

Err_t CGLogDataAceConcurrentUser::GLogDBDelete(int i_nCnt)
{
	try
	{
		// ���ν��� ����
		m_pSourceCommand->CommandText = QDB_GLOG_TB_CONCURRENT_USER_DELETE;

		// ���ν����� �� ���ڰ� ����
		m_pSourceCommand->Parameters->Append(m_pSourceCommand->CreateParameter(_bstr_t("UID"),adBigInt,adParamInput,8,_variant_t(m_arrAceConcurrentUserData[i_nCnt].UID)));

		// ���ν��� ����
		m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);

		// ���ڰ� �ʱ�ȭ
		m_pSourceCommand->Parameters->Delete(_bstr_t("UID"));
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataAceConcurrentUser",e);
	}
	return ERR_NO_ERROR;
}

void CGLogDataAceConcurrentUser::DBInsertParametersAppend(int i_nCnt)
{
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("cu_world"),adTinyInt,adParamInput,0,_variant_t(m_arrAceConcurrentUserData[i_nCnt].cu_world)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("cu_date"),adDate,adParamInput,8,_variant_t(m_arrAceConcurrentUserData[i_nCnt].cu_date)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("cu_count"),adInteger,adParamInput,0,_variant_t(m_arrAceConcurrentUserData[i_nCnt].cu_count)));

	VARIANT vPublisher;
	vPublisher.vt = VT_BSTR;
	vPublisher.bstrVal = _bstr_t(m_arrAceConcurrentUserData[i_nCnt].cu_publisher); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("cu_publisher"),adWChar,adParamInput,SIZE_MAX_PUBLISHER+1,vPublisher));

	VARIANT vGameServer;
	vGameServer.vt = VT_BSTR;
	vGameServer.bstrVal = _bstr_t(m_arrAceConcurrentUserData[i_nCnt].cu_gameServer); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("cu_gameServer"),adWChar,adParamInput,SIZE_MAX_GAME_SERVER_INFO+1,vGameServer));
}

void CGLogDataAceConcurrentUser::DBInsertParametersDelete()
{
	m_pDestCommand->Parameters->Delete(_bstr_t("cu_world"));		
	m_pDestCommand->Parameters->Delete(_bstr_t("cu_date"));
	m_pDestCommand->Parameters->Delete(_bstr_t("cu_count"));
	m_pDestCommand->Parameters->Delete(_bstr_t("cu_publisher"));
	m_pDestCommand->Parameters->Delete(_bstr_t("cu_gameServer"));
}
