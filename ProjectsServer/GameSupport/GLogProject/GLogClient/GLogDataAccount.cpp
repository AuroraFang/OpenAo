#include "StdAfx.h"
#include "GLogDataAccount.h"

CGLogDataAccount::CGLogDataAccount(_RecordsetPtr i_pSourceRecordSet,_CommandPtr i_pSourceCommand,_RecordsetPtr i_pDestRecordSet,_CommandPtr i_pDestCommand)
{
	this->Init(i_pSourceRecordSet,i_pSourceCommand,i_pDestRecordSet,i_pDestCommand);
}

CGLogDataAccount::~CGLogDataAccount(void)
{
}

void CGLogDataAccount::ResetData()
{
	util::zero(m_arrAccountData,sizeof(CAccountData)*GLOG_DATA_MAX_CNT);
}

Err_t CGLogDataAccount::GLogProcessing()
{
	int SelectCnt = 0;
	Err_t Err =	this->GLogDBSelect(&SelectCnt);
	if(ERR_NO_ERROR != Err)
	{
		return this->ErrorProcessing(Err);
	}
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog Account Select Fin!!! ==============\r\n",TRUE);

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
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog Account Update Fin!!! ==============\r\n",TRUE);
	this->ResetData();
	return ERR_NO_ERROR;
}

Err_t CGLogDataAccount::GLogDBSelect(int * o_pSelectCnt)
{
	m_pSourceCommand->CommandText = QDB_GLOG_ACCOUNT_SELECT;
	try
	{
		m_pSourceRecordSet = m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);  // ����� select���� �����մϴ�.
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataAccount",e);
	}
	
	if(m_pSourceRecordSet->EndOfFile)     
	{
		m_pSourceRecordSet->Close();   // ���ڵ���� �ݴ´�.(�׻� ���ڵ�� ���� �Ŀ� �ݾ��ִ� �۾��� �����ؾ� �մϴ�.)
		return ERR_DB_NO_DATA;
	}   

	int				nCnt = 0;
	CAccountData	tmpAccountData;
	// ��ȸ�� ���ڵ��� ������ ������ ���� �����͸� Ȯ���մϴ�.

	while(!m_pSourceRecordSet->EndOfFile)
	{
		tmpAccountData.UID	= m_pSourceRecordSet->Fields->GetItem("UID")->GetValue();	// �ش� �ʵ带 ������ variant������ �����մϴ�.
		LSTRNCPY_OLEDB(tmpAccountData.AccountName,m_pSourceRecordSet->Fields->GetItem("AccountName")->GetValue());
		LSTRNCPY_OLEDB(tmpAccountData.GamePublisher,m_pSourceRecordSet->Fields->GetItem("GamePublisher")->GetValue());
		LSTRNCPY_OLEDB(tmpAccountData.GameName,m_pSourceRecordSet->Fields->GetItem("GameName")->GetValue());
		LSTRNCPY_OLEDB(tmpAccountData.GameServerIP,m_pSourceRecordSet->Fields->GetItem("GameServerIP")->GetValue());
		tmpAccountData.Sex = m_pSourceRecordSet->Fields->GetItem("Sex")->GetValue();
		LSTRNCPY_OLEDB(tmpAccountData.Birthday,m_pSourceRecordSet->Fields->GetItem("Birthday")->GetValue());
		tmpAccountData.RegDate = m_pSourceRecordSet->Fields->GetItem("RegDate")->GetValue();
		memcpy(&m_arrAccountData[nCnt],&tmpAccountData,sizeof(CAccountData));
		util::zero(&tmpAccountData,sizeof(CAccountData));
		// ����Ʈ ��Ʈ���� �߰��ϼ̰ų� ȭ�鿡 ����Ͻ� ������ �����ôٸ� 
		// ������ ������ ���� ä���ִ� �۾��� �̺κп��� �Ͻø� �˴ϴ�.
		m_pSourceRecordSet->MoveNext();    // ���ڵ���� �������� �̵���ŵ�ϴ�.
		nCnt++;
	}

	*o_pSelectCnt = nCnt;
	m_pSourceRecordSet->Close();           // ���ڵ���� �ݾ��ݴϴ�.
	return ERR_NO_ERROR;
}

Err_t CGLogDataAccount::GLogDBInsert(int i_nCnt)
{
	try
	{
		// ���ν��� ����
		m_pDestCommand->CommandText = QDB_GLOG_ACCOUNT_INSERT;

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
		return ErrorLog(L"CGLogDataAccount",e);
	}
	return ERR_NO_ERROR;
}

Err_t CGLogDataAccount::GLogDBDelete(int i_nCnt)
{
	try
	{
		// ���ν��� ����
		m_pSourceCommand->CommandText = QDB_GLOG_ACCOUNT_DELETE;

		// ���ν����� �� ���ڰ� ����
		m_pSourceCommand->Parameters->Append(m_pSourceCommand->CreateParameter(_bstr_t("UID"),adBigInt,adParamInput,8,_variant_t(m_arrAccountData[i_nCnt].UID)));

		// ���ν��� ����
		m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);
		// ���ڰ� �ʱ�ȭ
		m_pSourceCommand->Parameters->Delete(_bstr_t("UID"));
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataAccount",e);
	}
	return ERR_NO_ERROR;
}

void CGLogDataAccount::DBInsertParametersAppend(int i_nCnt)
{
	VARIANT vAccountName;
	vAccountName.vt = VT_BSTR;
	vAccountName.bstrVal = _bstr_t(m_arrAccountData[i_nCnt].AccountName); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("AccountName"),adWChar,adParamInput,SIZE_MAX_ACCOUNT_NAME+1,vAccountName));
	VARIANT vGamePublisher;
	vGamePublisher.vt = VT_BSTR;
	vGamePublisher.bstrVal = _bstr_t(m_arrAccountData[i_nCnt].GamePublisher); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GamePublisher"),adWChar,adParamInput,SIZE_MAX_GAME_PUBLISHER_NAME+1,vGamePublisher));
	VARIANT vGameName;
	vGameName.vt = VT_BSTR;
	vGameName.bstrVal = _bstr_t(m_arrAccountData[i_nCnt].GameName); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GameName"),adWChar,adParamInput,SIZE_MAX_GAME_NAME+1,vGameName));
	VARIANT vGameServerIP;
	vGameServerIP.vt = VT_BSTR;
	vGameServerIP.bstrVal = _bstr_t(m_arrAccountData[i_nCnt].GameServerIP); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GameServerIP"),adWChar,adParamInput,SIZE_MAX_GAME_SERVER_IP+1,vGameServerIP));
	VARIANT vSex;
	vSex.vt = VT_BOOL;
	vSex.boolVal = m_arrAccountData[i_nCnt].Sex; 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Sex"),adBoolean,adParamInput,0,vSex));
	VARIANT vBirthday;
	vBirthday.vt = VT_BSTR;
	vBirthday.bstrVal = _bstr_t(m_arrAccountData[i_nCnt].Birthday); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Birthday"),adWChar,adParamInput,SIZE_MAX_BIRTHDAY+1,vBirthday));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("RegDate"),adDate,adParamInput,8,_variant_t(m_arrAccountData[i_nCnt].RegDate)));
}

void CGLogDataAccount::DBInsertParametersDelete()
{
	m_pDestCommand->Parameters->Delete(_bstr_t("AccountName"));
	m_pDestCommand->Parameters->Delete(_bstr_t("GamePublisher"));		
	m_pDestCommand->Parameters->Delete(_bstr_t("GameName"));
	m_pDestCommand->Parameters->Delete(_bstr_t("GameServerIP"));
	m_pDestCommand->Parameters->Delete(_bstr_t("Sex"));
	m_pDestCommand->Parameters->Delete(_bstr_t("Birthday"));
	m_pDestCommand->Parameters->Delete(_bstr_t("RegDate"));
}
