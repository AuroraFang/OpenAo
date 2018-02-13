#include "StdAfx.h"
#include "GLogDataCharacterPlayTime.h"

CGLogDataCharacterPlayTime::CGLogDataCharacterPlayTime(_RecordsetPtr i_pSourceRecordSet,_CommandPtr i_pSourceCommand,_RecordsetPtr i_pDestRecordSet,_CommandPtr i_pDestCommand)
{
	this->Init(i_pSourceRecordSet,i_pSourceCommand,i_pDestRecordSet,i_pDestCommand);
}

CGLogDataCharacterPlayTime::~CGLogDataCharacterPlayTime(void)
{
}

void CGLogDataCharacterPlayTime::ResetData()
{
	util::zero(m_arrCharacterPlayTimeData,sizeof(CCharacterPlayTimeData)*GLOG_DATA_MAX_CNT);
}

Err_t CGLogDataCharacterPlayTime::GLogProcessing()
{
	int SelectCnt = 0;
	Err_t Err =	this->GLogDBSelect(&SelectCnt);
	if(ERR_NO_ERROR != Err)
	{
		return this->ErrorProcessing(Err);
	}
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog CharacterPlayTime Select Fin!!! ==============\r\n",TRUE);

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
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog CharacterPlayTime Update Fin!!! ==============\r\n",TRUE);

	this->ResetData();
	return ERR_NO_ERROR;
}

Err_t CGLogDataCharacterPlayTime::GLogDBSelect(int * o_pSelectCnt)
{
	m_pSourceCommand->CommandText = QDB_GLOG_CHARACTER_PLAY_TIME_SELECT;
	try
	{
		m_pSourceRecordSet = m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);  // ����� select���� �����մϴ�.
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataCharacterPlayTime",e);
	}

	if(m_pSourceRecordSet->EndOfFile)     
	{
		m_pSourceRecordSet->Close();   // ���ڵ���� �ݴ´�.(�׻� ���ڵ�� ���� �Ŀ� �ݾ��ִ� �۾��� �����ؾ� �մϴ�.)
		return ERR_DB_NO_DATA;
	}   

	int				nCnt = 0;
	CCharacterPlayTimeData	tmpCharacterPlayTimeData;
	// ��ȸ�� ���ڵ��� ������ ������ ���� �����͸� Ȯ���մϴ�.
	while(!m_pSourceRecordSet->EndOfFile)
	{
		tmpCharacterPlayTimeData.UID	= m_pSourceRecordSet->Fields->GetItem("UID")->GetValue();	// �ش� �ʵ带 ������ variant������ �����մϴ�.
		LSTRNCPY_OLEDB(tmpCharacterPlayTimeData.AccountName,m_pSourceRecordSet->Fields->GetItem("AccountName")->GetValue());
		LSTRNCPY_OLEDB(tmpCharacterPlayTimeData.GamePublisher,m_pSourceRecordSet->Fields->GetItem("GamePublisher")->GetValue());
		LSTRNCPY_OLEDB(tmpCharacterPlayTimeData.GameName,m_pSourceRecordSet->Fields->GetItem("GameName")->GetValue());
		LSTRNCPY_OLEDB(tmpCharacterPlayTimeData.GameServerName,m_pSourceRecordSet->Fields->GetItem("GameServerName")->GetValue());
		LSTRNCPY_OLEDB(tmpCharacterPlayTimeData.CharacterName,m_pSourceRecordSet->Fields->GetItem("CharacterName")->GetValue());
		tmpCharacterPlayTimeData.LoginDate = m_pSourceRecordSet->Fields->GetItem("LoginDate")->GetValue();
		tmpCharacterPlayTimeData.LogoutDate = m_pSourceRecordSet->Fields->GetItem("LogoutDate")->GetValue();
		tmpCharacterPlayTimeData.TotalPlayTime = m_pSourceRecordSet->Fields->GetItem("TotalPlayTime")->GetValue();
		tmpCharacterPlayTimeData.PlayExp = m_pSourceRecordSet->Fields->GetItem("PlayExp")->GetValue();
		tmpCharacterPlayTimeData.TotalExp = m_pSourceRecordSet->Fields->GetItem("TotalExp")->GetValue();
		LSTRNCPY_OLEDB(tmpCharacterPlayTimeData.Race,m_pSourceRecordSet->Fields->GetItem("Race")->GetValue());
		LSTRNCPY_OLEDB(tmpCharacterPlayTimeData.Class,m_pSourceRecordSet->Fields->GetItem("Class")->GetValue());
		tmpCharacterPlayTimeData.Level = m_pSourceRecordSet->Fields->GetItem("Level")->GetValue();
		LSTRNCPY_OLEDB(tmpCharacterPlayTimeData.MostStayedInZoneName,m_pSourceRecordSet->Fields->GetItem("MostStayedInZoneName")->GetValue());
		memcpy(&m_arrCharacterPlayTimeData[nCnt],&tmpCharacterPlayTimeData,sizeof(CCharacterPlayTimeData));
		util::zero(&tmpCharacterPlayTimeData,sizeof(CCharacterPlayTimeData));
		// ����Ʈ ��Ʈ���� �߰��ϼ̰ų� ȭ�鿡 ����Ͻ� ������ �����ôٸ� 
		// ������ ������ ���� ä���ִ� �۾��� �̺κп��� �Ͻø� �˴ϴ�.
		m_pSourceRecordSet->MoveNext();    // ���ڵ���� �������� �̵���ŵ�ϴ�.
		nCnt++;
	}

	*o_pSelectCnt = nCnt;
	m_pSourceRecordSet->Close();           // ���ڵ���� �ݾ��ݴϴ�.
	return ERR_NO_ERROR;
}

Err_t CGLogDataCharacterPlayTime::GLogDBInsert(int i_nCnt)
{
	try
	{
		// ���ν��� ����
		m_pDestCommand->CommandText = QDB_GLOG_CHARACTER_PLAY_TIME_INSERT;

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
		return ErrorLog(L"CGLogDataCharacterPlayTime",e);
	}
	return ERR_NO_ERROR;
}

Err_t CGLogDataCharacterPlayTime::GLogDBDelete(int i_nCnt)
{
	try
	{
		// ���ν��� ����
		m_pSourceCommand->CommandText = QDB_GLOG_CHARACTER_PLAY_TIME_DELETE;

		// ���ν����� �� ���ڰ� ����
		m_pSourceCommand->Parameters->Append(m_pSourceCommand->CreateParameter(_bstr_t("UID"),adBigInt,adParamInput,8,_variant_t(m_arrCharacterPlayTimeData[i_nCnt].UID)));

		// ���ν��� ����
		m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);
		// ���ڰ� �ʱ�ȭ
		m_pSourceCommand->Parameters->Delete(_bstr_t("UID"));
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataCharacterPlayTime",e);
	}
	return ERR_NO_ERROR;
}

void CGLogDataCharacterPlayTime::DBInsertParametersAppend(int i_nCnt)
{
	VARIANT vAccountName;
	vAccountName.vt = VT_BSTR;
	vAccountName.bstrVal = _bstr_t(m_arrCharacterPlayTimeData[i_nCnt].AccountName); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("AccountName"),adWChar,adParamInput,SIZE_MAX_ACCOUNT_NAME+1,vAccountName));
	VARIANT vGamePublisher;
	vGamePublisher.vt = VT_BSTR;
	vGamePublisher.bstrVal = _bstr_t(m_arrCharacterPlayTimeData[i_nCnt].GamePublisher); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GamePublisher"),adWChar,adParamInput,SIZE_MAX_GAME_PUBLISHER_NAME+1,vGamePublisher));
	VARIANT vGameName;
	vGameName.vt = VT_BSTR;
	vGameName.bstrVal = _bstr_t(m_arrCharacterPlayTimeData[i_nCnt].GameName); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GameName"),adWChar,adParamInput,SIZE_MAX_GAME_NAME+1,vGameName));
	VARIANT vGameServerName;
	vGameServerName.vt = VT_BSTR;
	vGameServerName.bstrVal = _bstr_t(m_arrCharacterPlayTimeData[i_nCnt].GameServerName); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("GameServerName"),adWChar,adParamInput,SIZE_MAX_GAME_SERVER_NAME+1,vGameServerName));
	VARIANT vCharacterName;
	vCharacterName.vt = VT_BSTR;
	vCharacterName.bstrVal = _bstr_t(m_arrCharacterPlayTimeData[i_nCnt].CharacterName); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("CharacterName"),adWChar,adParamInput,SIZE_MAX_GAME_NAME+1,vCharacterName));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("LoginDate"),adDate,adParamInput,8,_variant_t(m_arrCharacterPlayTimeData[i_nCnt].LoginDate)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("LogoutDate"),adDate,adParamInput,8,_variant_t(m_arrCharacterPlayTimeData[i_nCnt].LogoutDate)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("TotalPlayTime"),adBigInt,adParamInput,8,_variant_t(m_arrCharacterPlayTimeData[i_nCnt].TotalPlayTime)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("PlayExp"),adInteger,adParamInput,0,_variant_t(m_arrCharacterPlayTimeData[i_nCnt].PlayExp)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("TotalExp"),adBigInt,adParamInput,8,_variant_t(m_arrCharacterPlayTimeData[i_nCnt].TotalExp)));
	VARIANT vRace;
	vRace.vt = VT_BSTR;
	vRace.bstrVal = _bstr_t(m_arrCharacterPlayTimeData[i_nCnt].Race); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Race"),adWChar,adParamInput,SIZE_MAX_RACE_NAME+1,vRace));
	VARIANT vClass;
	vClass.vt = VT_BSTR;
	vClass.bstrVal = _bstr_t(m_arrCharacterPlayTimeData[i_nCnt].Class); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Class"),adWChar,adParamInput,SIZE_MAX_CLASS_NAME+1,vClass));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("Level"),adInteger,adParamInput,0,_variant_t(m_arrCharacterPlayTimeData[i_nCnt].Level)));
	VARIANT vMostStayedInZoneName;
	vMostStayedInZoneName.vt = VT_BSTR;
	vMostStayedInZoneName.bstrVal = _bstr_t(m_arrCharacterPlayTimeData[i_nCnt].MostStayedInZoneName); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("MostStayedInZoneName"),adWChar,adParamInput,SIZE_MAX_MOST_STAYED_IN_ZONE_NAME+1,vMostStayedInZoneName));
}

void CGLogDataCharacterPlayTime::DBInsertParametersDelete()
{
	m_pDestCommand->Parameters->Delete(_bstr_t("AccountName"));
	m_pDestCommand->Parameters->Delete(_bstr_t("GamePublisher"));		
	m_pDestCommand->Parameters->Delete(_bstr_t("GameName"));
	m_pDestCommand->Parameters->Delete(_bstr_t("GameServerName"));
	m_pDestCommand->Parameters->Delete(_bstr_t("CharacterName"));
	m_pDestCommand->Parameters->Delete(_bstr_t("LoginDate"));
	m_pDestCommand->Parameters->Delete(_bstr_t("LogoutDate"));
	m_pDestCommand->Parameters->Delete(_bstr_t("TotalPlayTime"));
	m_pDestCommand->Parameters->Delete(_bstr_t("PlayExp"));
	m_pDestCommand->Parameters->Delete(_bstr_t("TotalExp"));
	m_pDestCommand->Parameters->Delete(_bstr_t("Race"));
	m_pDestCommand->Parameters->Delete(_bstr_t("Class"));
	m_pDestCommand->Parameters->Delete(_bstr_t("Level"));
	m_pDestCommand->Parameters->Delete(_bstr_t("MostStayedInZoneName"));
}
