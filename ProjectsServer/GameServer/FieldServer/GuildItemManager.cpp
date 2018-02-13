// GuildItemManager.cpp: implementation of the CGuildItemManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildItemManager.h"
#include "AtumFieldDBManager.h"				// 2006-09-25 by cmkwon
#include "FieldIOCP.h"						// 2006-09-25 by cmkwon
#include "FieldGlobal.h"					// 2006-09-25 by cmkwon

CFieldIOCP *CGuildItemManager::ms_pFieldIOCP3		= NULL;			// 2006-09-25 by cmkwon


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuildItemManager::CGuildItemManager()
{
	m_vectItemGeneralList.clear();
	m_vectItemGeneralList.reserve(100);

	m_uid32GuildUID			= 0;

	m_bIsLoaded				= FALSE;
	m_dwReqLoadLastTick		= 0;
}

CGuildItemManager::~CGuildItemManager()
{
	ResetGuildItemManager();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		dhjin
/// \date		2006-09-23 ~ 2006-09-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CGuildItemManager::SetGuildUID(UID32_t i_guildUID)
{
	m_uid32GuildUID		= i_guildUID;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			UID32_t CGuildItemManager::GetGuildUID(void)
/// \brief		
/// \author		dhjin
/// \date		2006-09-23 ~ 2006-09-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
UID32_t CGuildItemManager::GetGuildUID(void)
{
	return m_uid32GuildUID;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CGuildItemManager::GetIsLoaded(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-09-25 ~ 2006-09-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CGuildItemManager::GetIsLoaded(void)
{
	return m_bIsLoaded;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			DWORD CGuildItemManager::GetReqLoadLastTick(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-09-25 ~ 2006-09-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
DWORD CGuildItemManager::GetReqLoadLastTick(void)
{
	return m_dwReqLoadLastTick;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		cmkwon
/// \date		2006-09-25 ~ 2006-09-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CGuildItemManager::SetReqLoadLastTick(DWORD i_dwCurTick)
{
	m_dwReqLoadLastTick		= i_dwCurTick;
}


struct sort_vectENCANT_By_UID
{
	bool operator()(ENCHANT op1, ENCHANT op2)
	{
		return op1.TargetItemUniqueNumber < op2.TargetItemUniqueNumber;		// �������� ����
	}
};
///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CGuildItemManager::InitGuildItemManager(vectITEM_GENERALPtr *i_pGuildItem, vectENCHANT *i_pVectEnchant)
/// \brief		
/// \author		dhjin
/// \date		2006-09-25 ~ 2006-09-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CGuildItemManager::InitGuildItemManager(vectITEM_GENERALPtr *i_pGuildItem, vectENCHANT *i_pVectEnchant)
{
	this->ResetGuildItemManager();

	m_bIsLoaded					= TRUE;			// 2006-09-25 by cmkwon, DB�� ������ �ε���


	if(false == i_pGuildItem->empty())
	{
		m_vectItemGeneralList	= *i_pGuildItem;
	}

	if(false == i_pVectEnchant->empty())
	{
		sort(i_pVectEnchant->begin(), i_pVectEnchant->end(), sort_vectENCANT_By_UID());
		m_vectEnchantList		= *i_pVectEnchant;
	}

	i_pGuildItem->clear();				// 2007-01-30 by cmkwon
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CGuildItemManager::ResetGuildItemManager(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-09-25 ~ 2006-09-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CGuildItemManager::ResetGuildItemManager(void)
{
	for (auto x : m_vectItemGeneralList) delete x;
	m_vectItemGeneralList.clear();

	m_vectEnchantList.clear();

	m_bIsLoaded				= FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			ITEM_GENERAL* CGuildItemManager::GetItemGeneralByUID(UID64_t i_itemUID)
/// \brief		
/// \author		cmkwon
/// \date		2006-09-25 ~ 2006-09-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ITEM_GENERAL* CGuildItemManager::GetItemGeneralByUID(UID64_t i_itemUID)
{
	int nCnt = m_vectItemGeneralList.size(); 
	for(int i=0; i < nCnt; i++)
	{
		if(i_itemUID == m_vectItemGeneralList[i]->UniqueNumber)
		{
			return m_vectItemGeneralList[i];
		}
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ITEM_GENERAL* CGuildItemManager::GetItemGeneralByItemNum(int i_nItemNum)
/// \brief		
/// \author		cmkwon
/// \date		2006-09-25 ~ 2006-09-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ITEM_GENERAL* CGuildItemManager::GetItemGeneralByItemNum(int i_nItemNum)
{
	int nCnt = m_vectItemGeneralList.size(); 
	for(int i=0; i < nCnt; i++)
	{
		if(i_nItemNum == m_vectItemGeneralList[i]->ItemNum)
		{
			return m_vectItemGeneralList[i];
		}
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CGuildItemManager::GetGuildItemListSize(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-09-25 ~ 2006-09-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CGuildItemManager::GetGuildItemListSize(void)
{
	return m_vectItemGeneralList.size();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			vectITEM_GENERALPtr * CGuildItemManager::GetItemGeneralListPtr(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-09-25 ~ 2006-09-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
vectITEM_GENERALPtr * CGuildItemManager::GetItemGeneralListPtr(void)
{
	return &m_vectItemGeneralList;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CGuildItemManager::PushBackGuildStoreItemByPointer(ITEM_GENERAL *i_pItemG)
/// \brief		DB �߰� �Ŀ� �������� CGuildItemManager�� �߰��ϴ� �Լ�
/// \author		cmkwon
/// \date		2006-09-25 ~ 2006-09-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CGuildItemManager::PushBackGuildStoreItemByPointer(ITEM_GENERAL *i_pItemG)
{
	if(GetItemGeneralByUID(i_pItemG->UniqueNumber))
	{// 2006-09-25 by cmkwon, �̹� ���� UID�� �������� ���� �Ѵ�
		return FALSE;
	}

	m_vectItemGeneralList.push_back(i_pItemG);
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CGuildItemManager::DeleteItemGeneralByPointer(ITEM_GENERAL *i_pItemG, BOOL i_bDeleteMemory/*=TRUE*/)
/// \brief		
/// \author		cmkwon
/// \date		2006-09-25 ~ 2006-09-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CGuildItemManager::DeleteItemGeneralByPointer(ITEM_GENERAL *i_pItemG, BOOL i_bDeleteMemory/*=TRUE*/)
{
	for (int i=0; i < m_vectItemGeneralList.size();i++)
	{
		if (m_vectItemGeneralList[i]->UniqueNumber == i_pItemG->UniqueNumber)
		{
			m_vectItemGeneralList.erase(m_vectItemGeneralList.begin() + i);
			if(i_bDeleteMemory)
			{
				util::del(i_pItemG);						// release Memory
			}
			return TRUE;
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CGuildItemManager::UpdateGuildStoreItemStorage(CFieldIOCPSocket *i_pFISock, UID32_t i_AccountUID, UID32_t i_charUID, ITEM_GENERAL *i_pItemG, BYTE i_enFromStorage, BYTE i_enToStorage)
/// \brief		
/// \author		cmkwon
/// \date		2006-09-25 ~ 2006-09-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CGuildItemManager::UpdateGuildStoreItemStorage(CFieldIOCPSocket *i_pFISock, UID32_t i_AccountUID, UID32_t i_charUID
													, ITEM_GENERAL *i_pItemG, BYTE i_enFromStorage, BYTE i_enToStorage)
{
	if (ITEM_IN_GUILD_STORE == i_enToStorage)
	{// ĳ�����κ�->����â��
		if(FALSE == this->PushBackGuildStoreItemByPointer(i_pItemG))
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] PushBackGuildStoreItemByPointer error !! %s GuildUID(%d) %s %d->%d\r\n"
				, GetItemGeneralString(i_pItemG, string()), i_pItemG->Possess, GetCharacterString(i_pFISock->GetCharacter(), string()), i_enFromStorage, i_enToStorage);
			return FALSE;
		}
		
		i_AccountUID	= m_uid32GuildUID;
		i_charUID		= m_uid32GuildUID;
	}
	else if (ITEM_IN_CHARACTER == i_enToStorage)
	{// ����â��->�ɸ����κ�

		if(FALSE == this->DeleteItemGeneralByPointer(i_pItemG, FALSE))
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
	
	QPARAM_UPDATE_GUILDSTORE *qQParamStoreUpdate = new QPARAM_UPDATE_GUILDSTORE;
	qQParamStoreUpdate->ItemUID			= i_pItemG->UniqueNumber;
	qQParamStoreUpdate->AccountUID		= i_AccountUID;
	qQParamStoreUpdate->Possess			= i_charUID;
	qQParamStoreUpdate->ItemStorage		= i_enToStorage;
	qQParamStoreUpdate->Count			= i_pItemG->CurrentCount;
	ms_pFieldIOCP3->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateGuildStoreItem, i_pFISock, i_pFISock->m_character.AccountUniqueNumber, qQParamStoreUpdate);
	
	INIT_MSG_WITH_BUFFER(MSG_FC_STORE_UPDATE_ITEMSTORAGE, T_FC_STORE_UPDATE_ITEMSTORAGE, pMsgUpdate, SendBuf);
	pMsgUpdate->ItemUniqueNumber	= i_pItemG->UniqueNumber;
	pMsgUpdate->FromItemStorage		= i_enFromStorage;
	pMsgUpdate->ToItemStorage		= i_enToStorage;
	i_pFISock->SendAddData(SendBuf, MSG_SIZE(MSG_FC_STORE_UPDATE_ITEMSTORAGE));
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CGuildItemManager::InsertGuildStoreItemByItemNum(CFieldIOCPSocket *i_pFISock, INT i_nItemNum, INT i_nItemCount, BYTE i_nItemUpdateType, BYTE i_nItemStorage)
/// \brief		
/// \author		cmkwon
/// \date		2006-09-25 ~ 2006-09-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CGuildItemManager::InsertGuildStoreItemByItemNum(CFieldIOCPSocket *i_pFISock, INT i_nItemNum
													  , INT i_nItemCount, BYTE i_nItemUpdateType, BYTE i_nItemStorage)
{
	ITEM* pItemInfo = ms_pFieldIOCP3->GetItemInfo(i_nItemNum);
	if (pItemInfo == NULL)
	{
		char szErr[1024];
		sprintf(szErr, "InsertGuildStoreItemByItemNum iteminfo error !!, GuildUID(%d), ItemNum(%d), ItemCount(%d)\r\n"
			, m_uid32GuildUID, i_nItemNum, i_nItemCount);
		g_pFieldGlobal->WriteSystemLog(szErr);
		DbgOut(szErr);
		return FALSE;
	}
	
	ITEM_GENERAL *pItemGeneral			= new ITEM_GENERAL(pItemInfo);
	pItemGeneral->Kind					= pItemInfo->Kind;				// 2006-09-26 by cmkwon
	pItemGeneral->UniqueNumber			= 0;
	pItemGeneral->AccountUniqueNumber	= m_uid32GuildUID;
	pItemGeneral->Possess				= m_uid32GuildUID;
	pItemGeneral->ItemStorage			= ITEM_IN_GUILD_STORE;
	pItemGeneral->Wear					= WEAR_NOT_ATTACHED;
	pItemGeneral->CurrentCount			= (IS_CHARGABLE_ITEM(pItemInfo->Kind)?pItemInfo->Charging:1);
// 2009-08-25 by cmkwon, ������� �ʴ� �ʵ� ����(td_Store.ScarcityNumber) - 
//	pItemGeneral->ScarcityNumber		= 0;
	pItemGeneral->ItemWindowIndex		= POS_INVALID_POSITION;
	pItemGeneral->NumOfEnchants			= 0;
	
	if (pItemGeneral == NULL || !IS_GENERAL_ITEM(pItemGeneral->Kind))
	{
		util::del(pItemGeneral);
		return FALSE;
	}

	if (IS_COUNTABLE_ITEM(pItemGeneral->Kind))
	{
		pItemGeneral->CurrentCount = i_nItemCount;
	}

	if(FALSE == InsertGuildStoreItemByPointer(i_pFISock, pItemGeneral, i_nItemUpdateType))
	{
		util::del(pItemGeneral);
		return FALSE;
	}

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CGuildItemManager::InsertGuildStoreItemByPointer(CFieldIOCPSocket *i_pFISock, ITEM_GENERAL *i_pItemGeneral, BYTE i_nItemUpdateType, BOOL i_bSendToClient/*=TRUE*/)
/// \brief		
/// \author		cmkwon
/// \date		2006-09-25 ~ 2006-09-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CGuildItemManager::InsertGuildStoreItemByPointer(CFieldIOCPSocket *i_pFISock, ITEM_GENERAL *i_pItemGeneral
													  , BYTE i_nItemUpdateType, BOOL i_bSendToClient/*=TRUE*/)
{
	UID32_t	GuildUID0	= i_pItemGeneral->AccountUniqueNumber;
	if (FALSE == IS_GENERAL_ITEM(i_pItemGeneral->ItemInfo->Kind))
	{// ITEM_GENERAL�� �ƴϸ� ������ �� ����.

		return FALSE;
	}

	if (IS_COUNTABLE_ITEM(i_pItemGeneral->Kind)
		&& i_pItemGeneral->CurrentCount < 0)
	{// ī���ͺ� �������϶� ������ 0���� ������ error��

		return FALSE;
	}

	///////////////////////////////////////////////////////////////////////////
	// countable item�̰� �̹� �����ϴ� ���� update count
	///////////////////////////////////////////////////////////////////////////
	if (IS_COUNTABLE_ITEM(i_pItemGeneral->ItemInfo->Kind))
	{// Countable Item �� ���

		ITEM_GENERAL *pItemCountableFound = this->GetItemGeneralByItemNum(i_pItemGeneral->ItemNum);
		if (pItemCountableFound)
		{
			// �̹� �����ϸ�
			UpdateGuildStoreItemCountByPointer(i_pFISock, pItemCountableFound, i_pItemGeneral->CurrentCount
				, i_nItemUpdateType, i_bSendToClient);
			util::del(i_pItemGeneral);
			return TRUE;
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// �� �̿��� ��쿡�� ���� - UniqueNumber�� �޾ƾ� �ϱ� ������ QP_InsertGuildStoreItem() ���Ŀ� ó��
	ms_pFieldIOCP3->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InsertGuildStoreItem, i_pFISock, i_pFISock->m_character.AccountUniqueNumber
		, i_pItemGeneral, NULL);
	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CGuildItemManager::UpdateGuildStoreItemCountByPointer(CFieldIOCPSocket *i_pFISock, ITEM_GENERAL *i_pItemGeneral, INT i_nChangeCount, BYTE i_nItemUpdateType, BOOL i_bSendToClient/*=TRUE*/)
/// \brief		
/// \author		cmkwon
/// \date		2006-09-25 ~ 2006-09-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CGuildItemManager::UpdateGuildStoreItemCountByPointer(CFieldIOCPSocket *i_pFISock, ITEM_GENERAL *i_pItemGeneral
														   , INT i_nChangeCount, BYTE i_nItemUpdateType, BOOL i_bSendToClient/*=TRUE*/)
{
	///////////////////////////////////////////////////////////////////////////
	// ����� �� Countable Item�� ���ؼ��� �����, nCount��ŭ CurrentCount�� ��ȭ��Ŵ
	// nCount < 0�̸� ������ ������ ���̰�, nCount > 0�̸� ������ ���δ�
	// ���� ���� 0���� ������ �������� �����Ѵ�
	///////////////////////////////////////////////////////////////////////////
	if(0 == i_pItemGeneral->UniqueNumber)
	{
		return FALSE;
	}

	if (IS_CHARGABLE_ITEM(i_pItemGeneral->ItemInfo->Kind))
	{
		// do nothing
	}
	else if (!IS_COUNTABLE_ITEM(i_pItemGeneral->ItemInfo->Kind))
	{
		return FALSE;
	}
	else
	{// countable item�� ���
		
		if (i_pItemGeneral->CurrentCount <= 0 && i_pItemGeneral->ItemNum != MONEY_ITEM_NUMBER)
		{
			return FALSE;
		}

		///////////////////////////////////////////////////////////////////////////////
		// 2005-09-13 by cmkwon, ī���ͺ� ������ �ִ� ī��Ʈ üũ
		INT64 tmCurrentCount = (INT64)i_pItemGeneral->CurrentCount + (INT64)i_nChangeCount;
		if(tmCurrentCount > MAX_NOTIFY_ITEM_COUNTS)
		{
			i_pFISock->SendErrorMessage(T_FIELD_IOCP, ERR_PROTOCOL_NOTIFY_MAX_ITEM_COUNTS_OVER, 0, 0);

		}
		if(tmCurrentCount > MAX_ITEM_COUNTS)
		{// 2008-12-29 by dhjin, ���� ���� �߰���
			char szErr[1024];
			sprintf(szErr, "UpdateGuildStoreItem MaxCount Exceed !!, GuildUID(%d), ItemNum(%d), ItemCurCount(%d), ItemUpdateCount(%d)\r\n"
				, m_uid32GuildUID, i_pItemGeneral->ItemNum, i_pItemGeneral->CurrentCount, i_nChangeCount);
			g_pFieldGlobal->WriteSystemLog(szErr);
			DbgOut(szErr);
		}
	}

	// Item Count �����ϱ�
	INT64 n64Counts = i_pItemGeneral->CurrentCount;
	n64Counts = min((INT64)n64Counts + (INT64)i_nChangeCount, MAX_ITEM_COUNTS);
	if (n64Counts <= 0
		&& IS_COUNTABLE_ITEM(i_pItemGeneral->ItemInfo->Kind)
		&& i_pItemGeneral->ItemNum != MONEY_ITEM_NUMBER)
	{
		i_pFISock->m_bProcessingStoreGetItem = TRUE;		// 2013-02-21 by hskim, ��Ʈ�� ������ ���� ���� ���� (DB ó���� ���������� ���� ��Ŷ�� ����)

		QPARAM_DELETE_GUILDSTORE	*qQParamGuildStoreDelete =  new QPARAM_DELETE_GUILDSTORE;
		qQParamGuildStoreDelete->ItemUID		= i_pItemGeneral->UniqueNumber;
		qQParamGuildStoreDelete->GuildUID		= m_uid32GuildUID;
		qQParamGuildStoreDelete->byItemStorage	= ITEM_IN_GUILD_STORE;
		qQParamGuildStoreDelete->SendMSG		= i_bSendToClient;
		ms_pFieldIOCP3->m_pAtumDBManager->MakeAndEnqueueQuery(QT_DeleteGuildStoreItem, i_pFISock, i_pFISock->m_character.AccountUniqueNumber, qQParamGuildStoreDelete);

		DeleteItemGeneralByPointer(i_pItemGeneral);			// �����, �޸� ����
	}
	else if(i_pItemGeneral->CurrentCount != n64Counts)
	{
		i_pItemGeneral->CurrentCount = max(0, n64Counts);				// ���� ������Ʈ

		i_pFISock->m_bProcessingStoreGetItem = TRUE;		// 2013-02-21 by hskim, ��Ʈ�� ������ ���� ���� ���� (DB ó���� ���������� ���� ��Ŷ�� ����)

		// Update DB and send to client
		QPARAM_UPDATE_GUILDSTORE *qQParamStoreUpdate = new QPARAM_UPDATE_GUILDSTORE;
		qQParamStoreUpdate->ItemUID			= i_pItemGeneral->UniqueNumber;
		qQParamStoreUpdate->AccountUID		= i_pItemGeneral->AccountUniqueNumber;
		qQParamStoreUpdate->Possess			= i_pItemGeneral->Possess;
		qQParamStoreUpdate->ItemStorage		= ITEM_IN_GUILD_STORE;
		qQParamStoreUpdate->Count			= i_pItemGeneral->CurrentCount;
		ms_pFieldIOCP3->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateGuildStoreItem, i_pFISock, i_pFISock->m_character.AccountUniqueNumber, qQParamStoreUpdate);
		
		if(i_bSendToClient)
		{
			INIT_MSG_WITH_BUFFER(MSG_FC_STORE_UPDATE_ITEM_COUNT, T_FC_STORE_UPDATE_ITEM_COUNT, msgUpdate, msgUpdateBuf);
			msgUpdate->ItemUniqueNumber	= i_pItemGeneral->UniqueNumber;
			msgUpdate->NewCount			= i_pItemGeneral->CurrentCount;
			msgUpdate->ItemUpdateType	= i_nItemUpdateType;
			i_pFISock->SendAddData(msgUpdateBuf, MSG_SIZE(MSG_FC_STORE_UPDATE_ITEM_COUNT));
		}
	}
	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CGuildItemManager::SendGuildStoreItemList(CFieldIOCPSocket *i_pFISock)
/// \brief		
/// \author		cmkwon
/// \date		2006-09-25 ~ 2006-09-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CGuildItemManager::SendGuildStoreItemList(CFieldIOCPSocket *i_pFISock)
{
	if(FALSE == i_pFISock->IsValidCharacter())
	{
		return;
	}

	int			nSendBytes = 0;
	// PUT_ITEM_HEADER ����
	INIT_MSG_WITH_BUFFER(MSG_FC_STORE_PUT_ITEM_HEADER, T_FC_STORE_PUT_ITEM_HEADER, pSPutItemHeader, SendBuf);
	pSPutItemHeader->PossessCharacter	= i_pFISock->GetCharacter()->CharacterUniqueNumber;
	pSPutItemHeader->ItemStorage0		= ITEM_IN_GUILD_STORE;
	nSendBytes							= MSG_SIZE(MSG_FC_STORE_PUT_ITEM_HEADER);
	
	int i;
	for (i=0; i < m_vectItemGeneralList.size(); i++)
	{
		if(nSendBytes + MSG_SIZE(MSG_FC_STORE_INSERT_ITEM) > SIZE_MAX_PACKET)
		{
			i_pFISock->SendAddData(SendBuf, nSendBytes);
			nSendBytes					= 0;
		}

		*((MessageType_t*)(SendBuf + nSendBytes))	= T_FC_STORE_INSERT_ITEM;
		nSendBytes									+= SIZE_FIELD_TYPE_HEADER;
		MSG_FC_STORE_INSERT_ITEM *pStoreItem		= (MSG_FC_STORE_INSERT_ITEM*)(SendBuf + nSendBytes);
		pStoreItem->FromCharacterUniqueNumber		= i_pFISock->m_character.CharacterUniqueNumber;
		pStoreItem->ItemInsertionType				= IUT_LOADING;
		pStoreItem->ItemGeneral						= *(m_vectItemGeneralList[i]);
		nSendBytes									+= sizeof(MSG_FC_STORE_INSERT_ITEM);
	}

	if(nSendBytes > 0)
	{
		i_pFISock->SendAddData(SendBuf, nSendBytes);
		nSendBytes						= 0;
	}

	// PUT_ITEM_DONE ����
	INIT_MSG(MSG_FC_STORE_PUT_ITEM_DONE, T_FC_STORE_PUT_ITEM_DONE, pMsgPutItemDone, SendBuf);
	pMsgPutItemDone->NumOfItem		= 0;
	pMsgPutItemDone->ItemStorage0	= ITEM_IN_GUILD_STORE;
	i_pFISock->SendAddData(SendBuf, MSG_SIZE(MSG_FC_STORE_PUT_ITEM_DONE));


	///////////////////////////////////////////////////////////////////////////////
	// 2006-09-25 by cmkwon, ��æƮ ����
	// Init MSG
	INIT_MSG(MSG_FC_ITEM_PUT_ENCHANT, T_FC_ITEM_PUT_ENCHANT, pPutEnchant, SendBuf);
	nSendBytes						= 0;
	for(i=0; i < m_vectEnchantList.size(); i++)
	{
		if(nSendBytes + MSG_SIZE(MSG_FC_ITEM_PUT_ENCHANT) > SIZE_MAX_PACKET)
		{
			i_pFISock->SendAddData(SendBuf, nSendBytes);
			nSendBytes				= 0;
		}
		
		*((MessageType_t*)(SendBuf + nSendBytes))	= T_FC_ITEM_PUT_ENCHANT;
		nSendBytes									+= SIZE_FIELD_TYPE_HEADER;
		MSG_FC_ITEM_PUT_ENCHANT *pPutEnchat			= (MSG_FC_ITEM_PUT_ENCHANT*)(SendBuf + nSendBytes);
		pPutEnchat->Enchant							= m_vectEnchantList[i];
		nSendBytes									+= sizeof(MSG_FC_ITEM_PUT_ENCHANT);

	}// END - for(i=0; i < i_pVectEnchant->size(); i++)

	if(nSendBytes > 0)
	{
		i_pFISock->SendAddData(SendBuf, nSendBytes);
		nSendBytes					= 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CGuildItemManager::RearrangeGuildCountableItems(CFieldIOCPSocket *i_pFISock)
/// \brief		
/// \author		dhjin
/// \date		2006-09-25 ~ 2006-09-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CGuildItemManager::RearrangeGuildCountableItems(CFieldIOCPSocket *i_pFISock)
{
	vectITEM_GENERALPtr::iterator itr = m_vectItemGeneralList.begin(); 
	while(itr != m_vectItemGeneralList.end())
	{
		ITEM_GENERAL *pCurItemG = *itr;
		if(IS_COUNTABLE_ITEM(pCurItemG->Kind))
		{
			ITEM_GENERAL *pFirstItemG = GetItemGeneralByItemNum(pCurItemG->ItemNum);
			if (pFirstItemG
				&& pCurItemG->UniqueNumber != pFirstItemG->UniqueNumber)
			{
				UpdateGuildStoreItemCountByPointer(i_pFISock, pFirstItemG, pCurItemG->CurrentCount, IUT_GUILD_STORE, FALSE);

				QPARAM_DELETE_GUILDSTORE	*qQParamGuildStoreDelete =  new QPARAM_DELETE_GUILDSTORE;
				qQParamGuildStoreDelete->ItemUID		= pCurItemG->UniqueNumber;
				qQParamGuildStoreDelete->GuildUID		= m_uid32GuildUID;
				qQParamGuildStoreDelete->byItemStorage	= ITEM_IN_GUILD_STORE;
				qQParamGuildStoreDelete->SendMSG		= FALSE;
				ms_pFieldIOCP3->m_pAtumDBManager->MakeAndEnqueueQuery(QT_DeleteGuildStoreItem, i_pFISock, i_pFISock->m_character.AccountUniqueNumber, qQParamGuildStoreDelete);

				itr = m_vectItemGeneralList.erase(itr);

				util::del(pCurItemG);
				continue;
			}			
		}

		itr++;
	}
}

BOOL CGuildItemManager::PopGuildEnchantByItemUID(vectENCHANT *o_pvectEnchat, UID64_t i_itemUID)
{
	vectENCHANT::iterator itr = m_vectEnchantList.begin();
	while(itr != m_vectEnchantList.end())
	{
		if (i_itemUID == itr->TargetItemUniqueNumber)
		{
			o_pvectEnchat->push_back(*itr);
			itr = m_vectEnchantList.erase(itr);		// �����Ѵ�.
			continue;	// 2006-10-23 by cmkwon
		}
		itr++;	
	}
	return TRUE;
}

BOOL CGuildItemManager::InsertGuildEnchant(vectENCHANT *i_pvectEnchant)
{
	m_vectEnchantList.insert(m_vectEnchantList.end(), i_pvectEnchant->begin(), i_pvectEnchant->end());	
	return TRUE;
}
