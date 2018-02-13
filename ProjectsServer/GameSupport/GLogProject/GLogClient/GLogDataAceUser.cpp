//////////////////////////////////
// 2011-12-12 by hskim, GLog 2��

#include "StdAfx.h"
#include "GLogDataAceUser.h"

CGLogDataAceUser::CGLogDataAceUser(_RecordsetPtr i_pSourceRecordSet,_CommandPtr i_pSourceCommand,_RecordsetPtr i_pDestRecordSet,_CommandPtr i_pDestCommand)
{
	this->Init(i_pSourceRecordSet,i_pSourceCommand,i_pDestRecordSet,i_pDestCommand);
}

CGLogDataAceUser::~CGLogDataAceUser(void)
{
}

void CGLogDataAceUser::ResetData()
{
	util::zero(m_arrAceUserData,sizeof(CAceUserData)*GLOG_DATA_MAX_CNT);
}

Err_t CGLogDataAceUser::GLogProcessing()
{
	int SelectCnt = 0;
	Err_t Err =	this->GLogDBSelect(&SelectCnt);
	if(ERR_NO_ERROR != Err)
	{
		return this->ErrorProcessing(Err);
	}
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog AceUser Select Fin!!! ==============\r\n",TRUE);

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
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog AceUser Update Fin!!! ==============\r\n",TRUE);

	this->ResetData();
	return ERR_NO_ERROR;
}

Err_t CGLogDataAceUser::GLogDBSelect(int * o_pSelectCnt)
{
	m_pSourceCommand->CommandText = QDB_GLOG_TB_USER_SELECT;
	try
	{
		m_pSourceRecordSet = m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);  // ����� select���� �����մϴ�.
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataAceUser",e);
	}

	if(m_pSourceRecordSet->EndOfFile)     
	{
		m_pSourceRecordSet->Close();   // ���ڵ���� �ݴ´�.(�׻� ���ڵ�� ���� �Ŀ� �ݾ��ִ� �۾��� �����ؾ� �մϴ�.)
		return ERR_DB_NO_DATA;
	}   

	int				nCnt = 0;
	CAceUserData	tmpAceUserData;
	// ��ȸ�� ���ڵ��� ������ ������ ���� �����͸� Ȯ���մϴ�.
	while(!m_pSourceRecordSet->EndOfFile)
	{
		tmpAceUserData.UID	= m_pSourceRecordSet->Fields->GetItem("UID")->GetValue();	// �ش� �ʵ带 ������ variant������ �����մϴ�.
		LSTRNCPY_OLEDB(tmpAceUserData.us_id,m_pSourceRecordSet->Fields->GetItem("us_id")->GetValue());
		tmpAceUserData.us_birthday = m_pSourceRecordSet->Fields->GetItem("us_birthday")->GetValue();
		LSTRNCPY_OLEDB(tmpAceUserData.us_sex,m_pSourceRecordSet->Fields->GetItem("us_sex")->GetValue());
		tmpAceUserData.us_regdate = m_pSourceRecordSet->Fields->GetItem("us_regdate")->GetValue();
		LSTRNCPY_OLEDB(tmpAceUserData.us_delete,m_pSourceRecordSet->Fields->GetItem("us_delete")->GetValue());
		LSTRNCPY_OLEDB(tmpAceUserData.us_ip,m_pSourceRecordSet->Fields->GetItem("us_ip")->GetValue());
		LSTRNCPY_OLEDB(tmpAceUserData.us_location,m_pSourceRecordSet->Fields->GetItem("us_location")->GetValue());
		LSTRNCPY_OLEDB(tmpAceUserData.us_country,m_pSourceRecordSet->Fields->GetItem("us_country")->GetValue());
		LSTRNCPY_OLEDB(tmpAceUserData.us_publisher,m_pSourceRecordSet->Fields->GetItem("us_publisher")->GetValue());
		LSTRNCPY_OLEDB(tmpAceUserData.us_gameserver,m_pSourceRecordSet->Fields->GetItem("us_gameserver")->GetValue());

		memcpy(&m_arrAceUserData[nCnt],&tmpAceUserData,sizeof(CAceUserData));
		util::zero(&tmpAceUserData,sizeof(CAceUserData));

		// ����Ʈ ��Ʈ���� �߰��ϼ̰ų� ȭ�鿡 ����Ͻ� ������ �����ôٸ� 
		// ������ ������ ���� ä���ִ� �۾��� �̺κп��� �Ͻø� �˴ϴ�.

		m_pSourceRecordSet->MoveNext();    // ���ڵ���� �������� �̵���ŵ�ϴ�.
		nCnt++;
	}

	*o_pSelectCnt = nCnt;
	m_pSourceRecordSet->Close();           // ���ڵ���� �ݾ��ݴϴ�.
	return ERR_NO_ERROR;
}

Err_t CGLogDataAceUser::GLogDBInsert(int i_nCnt)
{
	try
	{
		// ���ν��� ����
		m_pDestCommand->CommandText = QDB_GLOG_TB_USER_INSERT;

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
		return ErrorLog(L"CGLogDataAceUser",e);
	}
	return ERR_NO_ERROR;
}

Err_t CGLogDataAceUser::GLogDBDelete(int i_nCnt)
{
	try
	{
		// ���ν��� ����
		m_pSourceCommand->CommandText = QDB_GLOG_TB_USER_DELETE;

		// ���ν����� �� ���ڰ� ����
		m_pSourceCommand->Parameters->Append(m_pSourceCommand->CreateParameter(_bstr_t("UID"),adBigInt,adParamInput,8,_variant_t(m_arrAceUserData[i_nCnt].UID)));

		// ���ν��� ����
		m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);

		// ���ڰ� �ʱ�ȭ
		m_pSourceCommand->Parameters->Delete(_bstr_t("UID"));
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataAceUser",e);
	}
	return ERR_NO_ERROR;
}

void CGLogDataAceUser::DBInsertParametersAppend(int i_nCnt)
{
	VARIANT vAccountName;
	vAccountName.vt = VT_BSTR;
	vAccountName.bstrVal = _bstr_t(m_arrAceUserData[i_nCnt].us_id);
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("us_id"),adWChar,adParamInput,SIZE_MAX_USER_ID+1,vAccountName));
	
	if( m_arrAceUserData[i_nCnt].us_birthday.GetStatus() != COleDateTime::valid )		// �ڵ尡 ������ �ȵ����� �켱 ���ѵ���...
	{
		VARIANT vBirthday;
		vBirthday.vt = VT_NULL;
		m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("us_birthday"),adDate,adParamInput,8,vBirthday));	
	}
	else
	{
		m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("us_birthday"),adDate,adParamInput,8,_variant_t(m_arrAceUserData[i_nCnt].us_birthday)));
	}

	VARIANT vSex;
	if( 0 != _tcslen(m_arrAceUserData[i_nCnt].us_sex) )									// �ڵ尡 ������ �ȵ����� �켱 ���ѵ���...
	{
		vSex.vt = VT_BSTR;
		vSex.bstrVal = m_arrAceUserData[i_nCnt].us_sex;
	}
	else
	{
		vSex.vt = VT_NULL;
	}
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("us_sex"),adWChar,adParamInput,SIZE_MAX_CHAR_ONE+1,vSex));

	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("us_regdate"),adDate,adParamInput,8,_variant_t(m_arrAceUserData[i_nCnt].us_regdate)));

	VARIANT vDelete;
	vDelete.vt = VT_BSTR;
	vDelete.bstrVal = m_arrAceUserData[i_nCnt].us_delete;
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("us_delete"),adWChar,adParamInput,SIZE_MAX_CHAR_ONE+1,vDelete));

	VARIANT vGameServerIP;
	vGameServerIP.vt = VT_BSTR;
	vGameServerIP.bstrVal = _bstr_t(m_arrAceUserData[i_nCnt].us_ip); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("us_ip"),adWChar,adParamInput,SIZE_MAX_IP_ADDRESS+1, vGameServerIP));

	VARIANT vLocation;
	if( 0 != _tcslen(m_arrAceUserData[i_nCnt].us_location) )									// �ڵ尡 ������ �ȵ����� �켱 ���ѵ���...
	{
		vLocation.vt = VT_BSTR;
		vLocation.bstrVal = _bstr_t(m_arrAceUserData[i_nCnt].us_location); 
	}
	else
	{
		vLocation.vt = VT_NULL;
	}
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("us_location"),adWChar,adParamInput,SIZE_MAX_LOCATION+1, vLocation));

	VARIANT vCountry;
	vCountry.vt = VT_BSTR;
	vCountry.bstrVal = _bstr_t(m_arrAceUserData[i_nCnt].us_country); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("us_country"),adWChar,adParamInput,SIZE_MAX_COUNTRY+1, vCountry));

	VARIANT vGamePublisher;
	vGamePublisher.vt = VT_BSTR;
	vGamePublisher.bstrVal = _bstr_t(m_arrAceUserData[i_nCnt].us_publisher); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("us_publisher"),adWChar,adParamInput,SIZE_MAX_PUBLISHER+1,vGamePublisher));

	VARIANT vGameServer;
	vGameServer.vt = VT_BSTR;
	vGameServer.bstrVal = _bstr_t(m_arrAceUserData[i_nCnt].us_gameserver); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("us_gameserver"),adWChar,adParamInput,SIZE_MAX_GAME_SERVER_INFO+1,vGameServer));
}

void CGLogDataAceUser::DBInsertParametersDelete()
{
	m_pDestCommand->Parameters->Delete(_bstr_t("us_id"));
	m_pDestCommand->Parameters->Delete(_bstr_t("us_birthday"));
	m_pDestCommand->Parameters->Delete(_bstr_t("us_sex"));
	m_pDestCommand->Parameters->Delete(_bstr_t("us_regdate"));
	m_pDestCommand->Parameters->Delete(_bstr_t("us_delete"));
	m_pDestCommand->Parameters->Delete(_bstr_t("us_ip"));
	m_pDestCommand->Parameters->Delete(_bstr_t("us_location"));
	m_pDestCommand->Parameters->Delete(_bstr_t("us_country"));
	m_pDestCommand->Parameters->Delete(_bstr_t("us_publisher"));
	m_pDestCommand->Parameters->Delete(_bstr_t("us_gameserver"));
}
