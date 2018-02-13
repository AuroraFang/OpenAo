//////////////////////////////////
// 2011-12-12 by hskim, GLog 2��

#include "StdAfx.h"
#include "GLogDataAceUserPlaytime.h"

CGLogDataAceUserPlaytime::CGLogDataAceUserPlaytime(_RecordsetPtr i_pSourceRecordSet,_CommandPtr i_pSourceCommand,_RecordsetPtr i_pDestRecordSet,_CommandPtr i_pDestCommand)
{
	this->Init(i_pSourceRecordSet,i_pSourceCommand,i_pDestRecordSet,i_pDestCommand);
}

CGLogDataAceUserPlaytime::~CGLogDataAceUserPlaytime(void)
{
}

void CGLogDataAceUserPlaytime::ResetData()
{
	util::zero(m_arrAceUserPlaytimeData,sizeof(CAceUserPlaytimeData)*GLOG_DATA_MAX_CNT);
}

Err_t CGLogDataAceUserPlaytime::GLogProcessing()
{
	int SelectCnt = 0;
	Err_t Err =	this->GLogDBSelect(&SelectCnt);
	if(ERR_NO_ERROR != Err)
	{
		return this->ErrorProcessing(Err);
	}
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog AceUserPlaytime Select Fin!!! ==============\r\n",TRUE);

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
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog AceUserPlaytime Update Fin!!! ==============\r\n",TRUE);

	this->ResetData();
	return ERR_NO_ERROR;
}

Err_t CGLogDataAceUserPlaytime::GLogDBSelect(int * o_pSelectCnt)
{
	m_pSourceCommand->CommandText = QDB_GLOG_TB_USER_PLAYTIME_SELECT;
	try
	{
		m_pSourceRecordSet = m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);  // ����� select���� �����մϴ�.
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataAceUserPlaytime",e);
	}

	if(m_pSourceRecordSet->EndOfFile)     
	{
		m_pSourceRecordSet->Close();   // ���ڵ���� �ݴ´�.(�׻� ���ڵ�� ���� �Ŀ� �ݾ��ִ� �۾��� �����ؾ� �մϴ�.)
		return ERR_DB_NO_DATA;
	}   

	int				nCnt = 0;
	CAceUserPlaytimeData	tmpAceUserPlaytimeData;
	// ��ȸ�� ���ڵ��� ������ ������ ���� �����͸� Ȯ���մϴ�.
	while(!m_pSourceRecordSet->EndOfFile)
	{
		tmpAceUserPlaytimeData.UID	= m_pSourceRecordSet->Fields->GetItem("UID")->GetValue();
		tmpAceUserPlaytimeData.up_login = m_pSourceRecordSet->Fields->GetItem("up_login")->GetValue();
		tmpAceUserPlaytimeData.up_logout = m_pSourceRecordSet->Fields->GetItem("up_logout")->GetValue();
		tmpAceUserPlaytimeData.up_time = m_pSourceRecordSet->Fields->GetItem("up_time")->GetValue();
		tmpAceUserPlaytimeData.us_no = m_pSourceRecordSet->Fields->GetItem("us_no")->GetValue();
		tmpAceUserPlaytimeData.up_charID = m_pSourceRecordSet->Fields->GetItem("up_charID")->GetValue();
		LSTRNCPY_OLEDB(tmpAceUserPlaytimeData.up_country,m_pSourceRecordSet->Fields->GetItem("up_country")->GetValue());
		LSTRNCPY_OLEDB(tmpAceUserPlaytimeData.up_race,m_pSourceRecordSet->Fields->GetItem("up_race")->GetValue());
		LSTRNCPY_OLEDB(tmpAceUserPlaytimeData.up_class,m_pSourceRecordSet->Fields->GetItem("up_class")->GetValue());
		tmpAceUserPlaytimeData.up_exp = m_pSourceRecordSet->Fields->GetItem("up_exp")->GetValue();
		LSTRNCPY_OLEDB(tmpAceUserPlaytimeData.up_gameserver,m_pSourceRecordSet->Fields->GetItem("up_gameserver")->GetValue());
		LSTRNCPY_OLEDB(tmpAceUserPlaytimeData.up_publisher,m_pSourceRecordSet->Fields->GetItem("up_publisher")->GetValue());

		memcpy(&m_arrAceUserPlaytimeData[nCnt],&tmpAceUserPlaytimeData,sizeof(CAceUserPlaytimeData));
		util::zero(&tmpAceUserPlaytimeData,sizeof(CAceUserPlaytimeData));

		// ����Ʈ ��Ʈ���� �߰��ϼ̰ų� ȭ�鿡 ����Ͻ� ������ �����ôٸ� 
		// ������ ������ ���� ä���ִ� �۾��� �̺κп��� �Ͻø� �˴ϴ�.

		m_pSourceRecordSet->MoveNext();    // ���ڵ���� �������� �̵���ŵ�ϴ�.
		nCnt++;
	}

	*o_pSelectCnt = nCnt;
	m_pSourceRecordSet->Close();           // ���ڵ���� �ݾ��ݴϴ�.
	return ERR_NO_ERROR;
}

Err_t CGLogDataAceUserPlaytime::GLogDBInsert(int i_nCnt)
{
	try
	{
		// ���ν��� ����
		m_pDestCommand->CommandText = QDB_GLOG_TB_USER_PLAYTIME_INSERT;

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
		return ErrorLog(L"CGLogDataAceUserPlaytime",e);
	}
	return ERR_NO_ERROR;
}

Err_t CGLogDataAceUserPlaytime::GLogDBDelete(int i_nCnt)
{
	try
	{
		// ���ν��� ����
		m_pSourceCommand->CommandText = QDB_GLOG_TB_USER_PLAYTIME_DELETE;

		// ���ν����� �� ���ڰ� ����
		m_pSourceCommand->Parameters->Append(m_pSourceCommand->CreateParameter(_bstr_t("UID"),adBigInt,adParamInput,8,_variant_t(m_arrAceUserPlaytimeData[i_nCnt].UID)));

		// ���ν��� ����
		m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);

		// ���ڰ� �ʱ�ȭ
		m_pSourceCommand->Parameters->Delete(_bstr_t("UID"));
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataAceUserPlaytime",e);
	}
	return ERR_NO_ERROR;
}

void CGLogDataAceUserPlaytime::DBInsertParametersAppend(int i_nCnt)
{
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("up_login"),adDate,adParamInput,8,_variant_t(m_arrAceUserPlaytimeData[i_nCnt].up_login)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("up_logout"),adDate,adParamInput,8,_variant_t(m_arrAceUserPlaytimeData[i_nCnt].up_logout)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("up_time"),adInteger,adParamInput,0,_variant_t(m_arrAceUserPlaytimeData[i_nCnt].up_time)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("us_no"),adInteger,adParamInput,0,_variant_t(m_arrAceUserPlaytimeData[i_nCnt].us_no)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("up_charID"),adInteger,adParamInput,0,_variant_t(m_arrAceUserPlaytimeData[i_nCnt].up_charID)));

	VARIANT vCountry;
	vCountry.vt = VT_BSTR;
	vCountry.bstrVal = _bstr_t(m_arrAceUserPlaytimeData[i_nCnt].up_country); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("up_country"),adWChar,adParamInput,SIZE_MAX_CHAR_ONE+1,vCountry));

	VARIANT vRace;
	vRace.vt = VT_BSTR;
	vRace.bstrVal = _bstr_t(m_arrAceUserPlaytimeData[i_nCnt].up_race); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("up_race"),adWChar,adParamInput,SIZE_MAX_CHAR_ONE+1,vRace));

	VARIANT vClass;
	vClass.vt = VT_BSTR;
	vClass.bstrVal = _bstr_t(m_arrAceUserPlaytimeData[i_nCnt].up_class); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("up_class"),adWChar,adParamInput,SIZE_MAX_CHAR_ONE+1,vClass));

	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("up_exp"),adDouble,adParamInput,0,_variant_t(m_arrAceUserPlaytimeData[i_nCnt].up_exp)));

	VARIANT vPublisher;
	vPublisher.vt = VT_BSTR;
	vPublisher.bstrVal = _bstr_t(m_arrAceUserPlaytimeData[i_nCnt].up_publisher); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("up_publisher"),adWChar,adParamInput,SIZE_MAX_PUBLISHER+1,vPublisher));

	VARIANT vGameServer;
	vGameServer.vt = VT_BSTR;
	vGameServer.bstrVal = _bstr_t(m_arrAceUserPlaytimeData[i_nCnt].up_gameserver); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("up_gameserver"),adWChar,adParamInput,SIZE_MAX_GAME_SERVER_INFO+1,vGameServer));
}

void CGLogDataAceUserPlaytime::DBInsertParametersDelete()
{
	m_pDestCommand->Parameters->Delete(_bstr_t("up_login"));		
	m_pDestCommand->Parameters->Delete(_bstr_t("up_logout"));
	m_pDestCommand->Parameters->Delete(_bstr_t("up_time"));
	m_pDestCommand->Parameters->Delete(_bstr_t("us_no"));
	m_pDestCommand->Parameters->Delete(_bstr_t("up_charID"));
	m_pDestCommand->Parameters->Delete(_bstr_t("up_country"));
	m_pDestCommand->Parameters->Delete(_bstr_t("up_race"));
	m_pDestCommand->Parameters->Delete(_bstr_t("up_class"));
	m_pDestCommand->Parameters->Delete(_bstr_t("up_exp"));
	m_pDestCommand->Parameters->Delete(_bstr_t("up_publisher"));
	m_pDestCommand->Parameters->Delete(_bstr_t("up_gameserver"));
}