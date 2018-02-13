//////////////////////////////////
// 2011-12-12 by hskim, GLog 2��

#include "StdAfx.h"
#include "GLogDataAceItemBuy.h"

CGLogDataAceItemBuy::CGLogDataAceItemBuy(_RecordsetPtr i_pSourceRecordSet,_CommandPtr i_pSourceCommand,_RecordsetPtr i_pDestRecordSet,_CommandPtr i_pDestCommand)
{
	this->Init(i_pSourceRecordSet,i_pSourceCommand,i_pDestRecordSet,i_pDestCommand);
}

CGLogDataAceItemBuy::~CGLogDataAceItemBuy(void)
{
}

void CGLogDataAceItemBuy::ResetData()
{
	util::zero(m_arrAceItemBuyData,sizeof(CAceItemBuyData)*GLOG_DATA_MAX_CNT);
}

Err_t CGLogDataAceItemBuy::GLogProcessing()
{
	int SelectCnt = 0;
	Err_t Err =	this->GLogDBSelect(&SelectCnt);
	if(ERR_NO_ERROR != Err)
	{
		return this->ErrorProcessing(Err);
	}
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog AceItemBuy Select Fin!!! ==============\r\n",TRUE);

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
	g_pLogGlobal->m_pGlobalLogManager->WriteSystemLog(L"============== GLog AceItemBuy Update Fin!!! ==============\r\n",TRUE);

	this->ResetData();
	return ERR_NO_ERROR;
}

Err_t CGLogDataAceItemBuy::GLogDBSelect(int * o_pSelectCnt)
{
	m_pSourceCommand->CommandText = QDB_GLOG_TB_ITEM_BUY_USER_SELECT;
	try
	{
		m_pSourceRecordSet = m_pSourceCommand->Execute(NULL,NULL,adCmdStoredProc);  // ����� select���� �����մϴ�.
	}
	catch(_com_error& e)
	{
		return ErrorLog(L"CGLogDataAceItemBuy",e);
	}

	if(m_pSourceRecordSet->EndOfFile)     
	{
		m_pSourceRecordSet->Close();   // ���ڵ���� �ݴ´�.(�׻� ���ڵ�� ���� �Ŀ� �ݾ��ִ� �۾��� �����ؾ� �մϴ�.)
		return ERR_DB_NO_DATA;
	}   

	int				nCnt = 0;
	CAceItemBuyData	tmpAceItemBuyData;
	// ��ȸ�� ���ڵ��� ������ ������ ���� �����͸� Ȯ���մϴ�.
	while(!m_pSourceRecordSet->EndOfFile)
	{
		tmpAceItemBuyData.UID = m_pSourceRecordSet->Fields->GetItem("UID")->GetValue();
		tmpAceItemBuyData.ib_date = m_pSourceRecordSet->Fields->GetItem("ib_date")->GetValue();
		tmpAceItemBuyData.ib_totalCost = m_pSourceRecordSet->Fields->GetItem("ib_totalCost")->GetValue();
		tmpAceItemBuyData.ib_CashCost = m_pSourceRecordSet->Fields->GetItem("ib_CashCost")->GetValue();
		tmpAceItemBuyData.us_no = m_pSourceRecordSet->Fields->GetItem("us_no")->GetValue();
		LSTRNCPY_OLEDB(tmpAceItemBuyData.us_id,m_pSourceRecordSet->Fields->GetItem("us_id")->GetValue());
		tmpAceItemBuyData.ib_good_id = m_pSourceRecordSet->Fields->GetItem("ib_good_id")->GetValue();
		LSTRNCPY_OLEDB(tmpAceItemBuyData.ib_good_name,m_pSourceRecordSet->Fields->GetItem("ib_good_name")->GetValue());
		LSTRNCPY_OLEDB(tmpAceItemBuyData.ib_publisher,m_pSourceRecordSet->Fields->GetItem("ib_publisher")->GetValue());
		LSTRNCPY_OLEDB(tmpAceItemBuyData.ib_charName,m_pSourceRecordSet->Fields->GetItem("ib_charName")->GetValue());
		tmpAceItemBuyData.ib_charLevel = m_pSourceRecordSet->Fields->GetItem("ib_charLevel")->GetValue();
		tmpAceItemBuyData.ib_itemCount = m_pSourceRecordSet->Fields->GetItem("ib_itemCount")->GetValue();
		LSTRNCPY_OLEDB(tmpAceItemBuyData.ib_gameserver,m_pSourceRecordSet->Fields->GetItem("ib_gameserver")->GetValue());

		memcpy(&m_arrAceItemBuyData[nCnt],&tmpAceItemBuyData,sizeof(CAceItemBuyData));
		util::zero(&tmpAceItemBuyData,sizeof(CAceItemBuyData));

		// ����Ʈ ��Ʈ���� �߰��ϼ̰ų� ȭ�鿡 ����Ͻ� ������ �����ôٸ� 
		// ������ ������ ���� ä���ִ� �۾��� �̺κп��� �Ͻø� �˴ϴ�.

		m_pSourceRecordSet->MoveNext();    // ���ڵ���� �������� �̵���ŵ�ϴ�.
		nCnt++;
	}

	*o_pSelectCnt = nCnt;
	m_pSourceRecordSet->Close();           // ���ڵ���� �ݾ��ݴϴ�.
	return ERR_NO_ERROR;
}

Err_t CGLogDataAceItemBuy::GLogDBInsert(int i_nCnt)
{
	try
	{
		// ���ν��� ����
		m_pDestCommand->CommandText = QDB_GLOG_TB_ITEM_BUY_USER_INSERT;

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
		return ErrorLog(L"CGLogDataAceItemBuy",e);
	}
	return ERR_NO_ERROR;
}

Err_t CGLogDataAceItemBuy::GLogDBDelete(int i_nCnt)
{
	try
	{
		// ���ν��� ����
		m_pSourceCommand->CommandText = QDB_GLOG_TB_ITEM_BUY_USER_DELETE;

		// ���ν����� �� ���ڰ� ����
		m_pSourceCommand->Parameters->Append(m_pSourceCommand->CreateParameter(_bstr_t("UID"),adBigInt,adParamInput,8,_variant_t(m_arrAceItemBuyData[i_nCnt].UID)));

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

void CGLogDataAceItemBuy::DBInsertParametersAppend(int i_nCnt)
{
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ib_date"),adDate,adParamInput,8,_variant_t(m_arrAceItemBuyData[i_nCnt].ib_date)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ib_totalCost"),adInteger,adParamInput,0,_variant_t(m_arrAceItemBuyData[i_nCnt].ib_totalCost)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ib_CashCost"),adInteger,adParamInput,0,_variant_t(m_arrAceItemBuyData[i_nCnt].ib_CashCost)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("us_no"),adInteger,adParamInput,0,_variant_t(m_arrAceItemBuyData[i_nCnt].us_no)));

	VARIANT vIPAddress;
	vIPAddress.vt = VT_BSTR;
	vIPAddress.bstrVal = _bstr_t(m_arrAceItemBuyData[i_nCnt].us_id); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("us_id"),adWChar,adParamInput,SIZE_MAX_USER_ID+1,vIPAddress));

	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ib_good_id"),adInteger,adParamInput,0,_variant_t(m_arrAceItemBuyData[i_nCnt].ib_good_id)));

	VARIANT vGoodName;
	vGoodName.vt = VT_BSTR;
	vGoodName.bstrVal = _bstr_t(m_arrAceItemBuyData[i_nCnt].ib_good_name); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ib_good_name"),adWChar,adParamInput,SIZE_MAX_GOOD_NAME+1,vGoodName));

	VARIANT vPublisher;
	vPublisher.vt = VT_BSTR;
	vPublisher.bstrVal = _bstr_t(m_arrAceItemBuyData[i_nCnt].ib_publisher); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ib_publisher"),adWChar,adParamInput,SIZE_MAX_GAME_PUBLISHER_NAME+1,vPublisher));

	VARIANT vCharName;
	vCharName.vt = VT_BSTR;
	vCharName.bstrVal = _bstr_t(m_arrAceItemBuyData[i_nCnt].ib_charName); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ib_charName"),adWChar,adParamInput,SIZE_MAX_CHAR_NAME+1,vCharName));

	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ib_charLevel"),adInteger,adParamInput,0,_variant_t(m_arrAceItemBuyData[i_nCnt].ib_charLevel)));
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ib_itemCount"),adInteger,adParamInput,0,_variant_t(m_arrAceItemBuyData[i_nCnt].ib_itemCount)));

	VARIANT vGameServer;
	vGameServer.vt = VT_BSTR;
	vGameServer.bstrVal = _bstr_t(m_arrAceItemBuyData[i_nCnt].ib_gameserver); 
	m_pDestCommand->Parameters->Append(m_pDestCommand->CreateParameter(_bstr_t("ib_gameserver"),adWChar,adParamInput,SIZE_MAX_GAME_SERVER_INFO+1,vGameServer));
}

void CGLogDataAceItemBuy::DBInsertParametersDelete()
{
	m_pDestCommand->Parameters->Delete(_bstr_t("ib_date"));		
	m_pDestCommand->Parameters->Delete(_bstr_t("ib_totalCost"));
	m_pDestCommand->Parameters->Delete(_bstr_t("ib_CashCost"));
	m_pDestCommand->Parameters->Delete(_bstr_t("us_no"));
	m_pDestCommand->Parameters->Delete(_bstr_t("us_id"));
	m_pDestCommand->Parameters->Delete(_bstr_t("ib_good_id"));
	m_pDestCommand->Parameters->Delete(_bstr_t("ib_good_name"));
	m_pDestCommand->Parameters->Delete(_bstr_t("ib_publisher"));
	m_pDestCommand->Parameters->Delete(_bstr_t("ib_charName"));
	m_pDestCommand->Parameters->Delete(_bstr_t("ib_charLevel"));
	m_pDestCommand->Parameters->Delete(_bstr_t("ib_itemCount"));
	m_pDestCommand->Parameters->Delete(_bstr_t("ib_gameserver"));
}
