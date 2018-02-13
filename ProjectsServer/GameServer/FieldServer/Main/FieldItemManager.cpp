#include "stdafx.h"
#include "FieldItemManager.h"
#include "FieldIOCP.h"
#include "AtumError.h"
#include "AtumLogSender.h"
#include "FieldGlobal.h"
#include "BunchCountableItemCount.h"

#include "FieldParty.h"		// 2013-02-28 by bckim, �������� �����߰�

CFieldItemManager::CFieldItemManager()
{
	m_vectDelayStoredItemInfo.reserve(10);

	m_cashPremiumCardInfo.ResetCASH_PREMEIUM_CARD_INFO();

	m_dwTimeLimiteCardItemCheckCounts	= 0;

	m_uid32StoreCharacterUID			= 0;

	this->SetUsingFieldStore(FALSE);		// 2009-09-23 by cmkwon, �ʵ�â�� ĳ�� ������ ���� - CFieldItemManager::CFieldItemManager(), 
	this->SetUsingStore(FALSE);				// 2013-04-12 by jhseol, �ʵ�â�� ������� ������þƮ ���� ����

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - Main������ ������ ����ȭ�� ���� ó��
	m_mtInfinityInsertItemInfoList.clear();
	m_mtInfinityUpdateItemInfoList.clear();
	m_mtInfinityDeleteItemInfoList.clear();

	m_nInventoryHiddenItemCount = 0;		// 2011-10-21 by hskim, ��Ʈ�� �ý��� 2�� - ���� ������ �ִ� ������ ������ (POS_HIDDEN_ITEM) ����
}

void CFieldItemManager::ResetAllItemGenerals()
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	
	for (auto& x : m_mapItemGeneral) delete x.second;
	
	m_mapItemGeneral.clear();

	// 2010-06-15 by shcho&hslee ��ý��� - ���� �� ���� �ʱ�ȭ.
	mtvectPetCurrentInfo::iterator it_PetCurrentInfo = m_vectPetCurrentInfo.begin();

	for ( ; it_PetCurrentInfo != m_vectPetCurrentInfo.end(); ++it_PetCurrentInfo )
	{
		util::del( *it_PetCurrentInfo );
	}
	m_vectPetCurrentInfo.clear();
	
	// 2008-01-31 by cmkwon, �ð����� ������ DB �� �߰� �� üũ - �ʱ�ȭ�ϱ�
	m_vectInsertingItemNumList2DB.clear();

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - Main������ ������ ����ȭ�� ���� ó��
	m_mtInfinityInsertItemInfoList.clearLock();
	m_mtInfinityUpdateItemInfoList.clearLock();
	m_mtInfinityDeleteItemInfoList.clearLock();

	m_nInventoryHiddenItemCount = 0;		// 2011-10-21 by hskim, ��Ʈ�� �ý��� 2�� - ���� ������ �ִ� ������ ������ (POS_HIDDEN_ITEM) ����
}

// 2007-01-29 by cmkwon, ���� �߰�(BOOL i_bClearEnchat)
void CFieldItemManager::ResetAllItemGeneralsInStore(BOOL i_bClearEnchat/*=TRUE*/)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-29 by cmkwon, â�� �������� ������ ��æƮ�� �����Ѵ�
	if(i_bClearEnchat
		&& m_pFieldIOCPSocket)
	{
		mt_auto_lock mtE(&m_pFieldIOCPSocket->m_mapEnchant);

		mtmapUID2ItemGeneral::iterator itr(m_mapItemInStore.begin());
		for(; itr != m_mapItemInStore.end(); itr++)
		{
			m_pFieldIOCPSocket->DeleteAllEnchantToItem(itr->first);
		}
	}
	for (auto& x : m_mapItemInStore) delete x.second;
	
	m_mapItemInStore.clear();

	m_uid32StoreCharacterUID	= 0;
}
void CFieldItemManager::ResetAllItemSkills()
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	for (auto x : m_vectItemSkillPtr) delete x;
	m_vectItemSkillPtr.clear();
}
void CFieldItemManager::ResetUsingGeneralItems(void)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	vectITEM_GENERALPtr::iterator itr(m_vectUsingGeneralItem.begin());
	for(; itr != m_vectUsingGeneralItem.end(); itr++)
	{
		ITEM_GENERAL *pItemG = *itr;
// 2009-10-22 by cmkwon, �ð� �������� ������ �ý��� ���� - CFieldItemManager::ResetUsingGeneralItems#, �Ʒ��� ���� ������ �������� ����
//		if(ITEMKIND_ACCESSORY_TIMELIMIT != pItemG->Kind)
		if(FALSE == IS_ATTACHABLE_ITEM(pItemG->ItemInfo) || WEAR_NOT_ATTACHED == pItemG->Wear)
		{// 2006-10-11 by cmkwon, ITEMKIND_ACCESSORY_TIMELIMIT�� ���� ������ �̹Ƿ� ���⿡�� �޸𸮸� �������� �ʴ´�
			util::del(*itr);
		}
	}
	m_vectUsingGeneralItem.clear();
}

// start 2011-09-29 by hskim, ��Ʈ�� �ý��� 2�� - ���Ǵ�Ƽ���� ���ƿ� �� ���
void CFieldItemManager::ResetUsingSocketItems(void)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	vectITEM_GENERALPtr::iterator itr(m_vectUsingGeneralItem.begin());

	while( itr != m_vectUsingGeneralItem.end() )
	{
		ITEM_GENERAL *pItemG = *itr;

		if(ITEMKIND_PET_SOCKET_ITEM == pItemG->Kind)
		{
			DeleteInsertingItemNumList2DB(pItemG->ItemInfo->ItemNum);
			
			util::del(*itr);
			itr = m_vectUsingGeneralItem.erase(itr);

			continue;
		}

		itr++;
	}
}
// end 2011-09-29 by hskim, ��Ʈ�� �ý��� 2�� - ���Ǵ�Ƽ���� ���ƿ� �� ���

void CFieldItemManager::ResetPremiumCardInfo(void)
{
	m_cashPremiumCardInfo.ResetCASH_PREMEIUM_CARD_INFO();
}


// ĳ������ ���� ���� �� ȣ��
void CFieldItemManager::ResetAllCharacterItems()
{
	ResetUsingGeneralItems();
	ResetAllItemGenerals();
	ResetAllItemGeneralsInStore();
	ResetAllItemSkills();

	this->SetUsingFieldStore(FALSE);	// 2009-09-23 by cmkwon, �ʵ�â�� ĳ�� ������ ���� - CFieldItemManager::ResetAllCharacterItems, 
	this->SetUsingStore(FALSE);				// 2013-04-12 by jhseol, �ʵ�â�� ������� ������þƮ ���� ����
}

ITEM_GENERAL* CFieldItemManager::GetItemGeneralByUID(UID64_t i_nItemUID, BYTE i_enumStorage)
{
// 2008-07-25 by cmkwon, CFieldItemManager::GetItemGeneralByUID() ItemUID üũ �߰� - �Ʒ��� ���� ����
//	mt_auto_lock mtAuto(&m_mapItemGeneral);
// 	if (i_enumStorage == ITEM_IN_CHARACTER)
// 	{
// 		return m_mapItemGeneral.findNoLock(i_nItemUID);
// 	}
// 	else if (i_enumStorage == ITEM_IN_STORE
// 			&& m_pFieldIOCPSocket->m_pCurrentBuildingNPC != NULL
// 			&& m_pFieldIOCPSocket->m_pCurrentBuildingNPC->BuildingKind == BUILDINGKIND_STORE)
// 	{
// 		return m_mapItemInStore.findNoLock(i_nItemUID);
// 	}
//
//	return NULL;

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-25 by cmkwon, CFieldItemManager::GetItemGeneralByUID() ItemUID üũ �߰� - 
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	ITEM_GENERAL *pItemG = NULL;
	if (i_enumStorage == ITEM_IN_CHARACTER)
	{
		pItemG = m_mapItemGeneral.findNoLock(i_nItemUID);
	}
	else if (i_enumStorage == ITEM_IN_STORE)
// 2009-10-30 by cmkwon, â�� ������ �������� ���� ���� ���� - CFieldItemManager::GetItemGeneralByUID#, 
//			&& m_pFieldIOCPSocket->m_pCurrentBuildingNPC != NULL
//			&& m_pFieldIOCPSocket->m_pCurrentBuildingNPC->BuildingKind == BUILDINGKIND_STORE)
	{
		///////////////////////////////////////////////////////////////////////////////
		// 2009-10-30 by cmkwon, â�� ������ �������� ���� ���� ���� - CFieldItemManager::GetItemGeneralByUID#, 
		if(NULL == m_pFieldIOCPSocket->m_pCurrentBuildingNPC
			|| BUILDINGKIND_STORE != m_pFieldIOCPSocket->m_pCurrentBuildingNPC->BuildingKind)
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldItemManager::GetItemGeneralByUID State error !! %s %s pBuildingNPC(0x%X) BuildingKind(%d) \r\n"
				, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), GetItemGeneralString(pItemG, string()), m_pFieldIOCPSocket->m_pCurrentBuildingNPC, (NULL == m_pFieldIOCPSocket->m_pCurrentBuildingNPC)?-1:m_pFieldIOCPSocket->m_pCurrentBuildingNPC->BuildingKind);
			return NULL;
		}

		pItemG = m_mapItemInStore.findNoLock(i_nItemUID);
	}

	if(NULL == pItemG)
	{// 2008-07-25 by cmkwon, �������� ���� ���� �ʴ´�.
		return NULL;
	}

	if(pItemG->UniqueNumber != i_nItemUID)
	{// 2008-07-25 by cmkwon, ã�� �������� ItemUID �� ���Ѵ�.
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldItemManager::GetItemGeneralByUID ItemUID error !! %s %s %I64d\r\n"
			, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), GetItemGeneralString(pItemG, string()), i_nItemUID);
		return NULL;
	}

	return pItemG;
}

ITEM_GENERAL* CFieldItemManager::GetFirstItemGeneralByItemNum(INT i_nItemNum, BYTE i_enumItemStorage)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	if (i_enumItemStorage == ITEM_IN_CHARACTER)
	{
		mtmapUID2ItemGeneral::iterator itr = m_mapItemGeneral.begin();
		while (itr != m_mapItemGeneral.end())
		{
			if (itr->second->ItemNum == (int)i_nItemNum)
			{
				return itr->second;
			}
			itr++;
		}
	}
	else if (i_enumItemStorage == ITEM_IN_STORE)
// 2009-10-30 by cmkwon, â�� ������ �������� ���� ���� ���� - CFieldItemManager::GetFirstItemGeneralByItemNum#, 
//			&& m_pFieldIOCPSocket->m_pCurrentBuildingNPC != NULL
//			&& m_pFieldIOCPSocket->m_pCurrentBuildingNPC->BuildingKind == BUILDINGKIND_STORE)
	{
		///////////////////////////////////////////////////////////////////////////////
		// 2009-10-30 by cmkwon, â�� ������ �������� ���� ���� ���� - CFieldItemManager::GetFirstItemGeneralByItemNum, 
		if(NULL == m_pFieldIOCPSocket->m_pCurrentBuildingNPC
			|| BUILDINGKIND_STORE != m_pFieldIOCPSocket->m_pCurrentBuildingNPC->BuildingKind)
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldItemManager::GetFirstItemGeneralByItemNum# State error !! %s ItemNum(%d) pBuildingNPC(0x%X) BuildingKind(%d) \r\n"
				, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), i_nItemNum, m_pFieldIOCPSocket->m_pCurrentBuildingNPC, (NULL == m_pFieldIOCPSocket->m_pCurrentBuildingNPC)?-1:m_pFieldIOCPSocket->m_pCurrentBuildingNPC->BuildingKind);
			return NULL;
		}
		
		mtmapUID2ItemGeneral::iterator itr = m_mapItemInStore.begin();
		while (itr != m_mapItemInStore.end())
		{
			if (itr->second->ItemNum == (int)i_nItemNum)
			{
				return itr->second;
			}
			itr++;
		}
	}

	return NULL;
}

ITEM_GENERAL* CFieldItemManager::PopItemGeneralByUID(UID64_t i_nItemUID)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);	
	return m_mapItemGeneral.popLock(i_nItemUID);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� ItemNum�߿� �̹� ������� �ð��� �������� �ִ��� üũ
/// \author		cmkwon
/// \date		2005-03-10 ~ 2005-03-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::IsExistAlreadyUsingItemByDestParameter1(DestParam_t i_DestParameter)		// 2011-08-01 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ��� ���� (DestParameter - 255 -> 32767 ����)
{
	if(DES_NULL == i_DestParameter)
	{// 2009-09-25 by cmkwon, üũ �߰�
		return FALSE;
	}

	mt_auto_lock mtAuto(&m_mapItemGeneral);
	for(int i=0; i < m_vectUsingGeneralItem.size(); i++)
	{
		// 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - �Ʒ��� ���� ����
		//if(m_vectUsingGeneralItem[i]->ItemInfo->DestParameter1 == i_DestParameter)
		// 2009-09-23 by cmkwon, �ʵ�â�� ĳ�� ������ ���� - �Ʒ��� ���� 8���� ��� üũ�ϰ� ����
		//if(m_vectUsingGeneralItem[i]->ItemInfo->ArrDestParameter[0] == i_DestParameter)
		if(m_vectUsingGeneralItem[i]->ItemInfo->IsExistDesParam(i_DestParameter))
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldItemManager::IsExistAlreadyUsingItemByItemNum(int i_nItemNum)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-21 ~ 2006-04-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::IsExistAlreadyUsingItemByItemNum(int i_nItemNum)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	for(int i=0; i < m_vectUsingGeneralItem.size(); i++)
	{
		if(m_vectUsingGeneralItem[i]->ItemNum == i_nItemNum)
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldItemManager::DeleteUsingTimeLimitedItemList(int i_nItemNum)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-21 ~ 2006-04-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::DeleteUsingTimeLimitedItemList(int i_nItemNum)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	for(int i=0; i < m_vectUsingGeneralItem.size(); i++)
	{
		if(m_vectUsingGeneralItem[i]->ItemNum == i_nItemNum)
		{
			///////////////////////////////////////////////////////////////////////////////
			// 1. ������ ��� ����
			m_pFieldIOCPSocket->ReleaseCardItem(m_vectUsingGeneralItem[i]->ItemInfo);

			///////////////////////////////////////////////////////////////////////////////
			// 2. Ŭ���̾�Ʈ�� ����
			INIT_MSG_WITH_BUFFER(MSG_FC_STORE_DELETE_USINGITEM, T_FC_STORE_DELETE_USINGITEM, pSDUsingItem, SendBuf);
			pSDUsingItem->ItemNum			= m_vectUsingGeneralItem[i]->ItemNum;
			pSDUsingItem->ItemDeletionType	= IUT_EXPIRE_CARD_ITEM;
			m_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_STORE_DELETE_USINGITEM));

			///////////////////////////////////////////////////////////////////////////////
			// 3. DB���� ������ ����			
			QPARAM_DELETESTOREITEM *pQParam	= new QPARAM_DELETESTOREITEM;
			pQParam->AccountUniqueNumber	= m_pFieldIOCPSocket->m_character.AccountUniqueNumber;
			pQParam->ItemUniqueNumber		= m_vectUsingGeneralItem[i]->UniqueNumber;
			ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_DeleteStoreItem, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);

//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENACHECK] DeleteUsingTimeLimitedItemList ItemUniqueNumber = %d\r\n"
//				, m_vectUsingGeneralItem[i]->UniqueNumber);

			util::del(m_vectUsingGeneralItem[i]);

			m_vectUsingGeneralItem.erase(m_vectUsingGeneralItem.begin() + i);

			return TRUE;
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldItemManager::ApplyParamFactorWithAllUsingTimeLimitedOnlyCardItem(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-24 ~ 2006-04-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::ApplyParamFactorWithAllUsingTimeLimitedOnlyCardItem(void)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	for(int i=0; i < m_vectUsingGeneralItem.size(); i++)
	{
		ITEM_GENERAL *pUsingItemG = m_vectUsingGeneralItem[i];
// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
// 		if(ITEMKIND_ACCESSORY_TIMELIMIT != pUsingItemG->ItemInfo->Kind)
// 		{// 2006-10-11 by cmkwon, ITEMKIND_ACCESSORY_TIMELIMIT�� ���� �������̹Ƿ� ���� ó���ȴ�
// // 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - �Ʒ��� ���� ����
// // 			m_pFieldIOCPSocket->SetParamFactor(pUsingItemG->ItemInfo->DestParameter1, pUsingItemG->ItemInfo->ParameterValue1);
// // 			m_pFieldIOCPSocket->SetParamFactor(pUsingItemG->ItemInfo->DestParameter2, pUsingItemG->ItemInfo->ParameterValue2);
// // 			m_pFieldIOCPSocket->SetParamFactor(pUsingItemG->ItemInfo->DestParameter3, pUsingItemG->ItemInfo->ParameterValue3);
// // 			m_pFieldIOCPSocket->SetParamFactor(pUsingItemG->ItemInfo->DestParameter4, pUsingItemG->ItemInfo->ParameterValue4);
// 			for(int j=0; j < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; j++)
// 			{
// 				m_pFieldIOCPSocket->SetParamFactor(pUsingItemG->ItemInfo->ArrDestParameter[j], pUsingItemG->ItemInfo->ArrParameterValue[j]);
// 			}
// 		}
		///////////////////////////////////////////////////////////////////////////////
		// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������, �������� �������� �ƴѰ͸� ��� ����
		if(FALSE == IS_ATTACHABLE_ITEM(pUsingItemG->ItemInfo))
		{
			// 2013-07-22 by bckim, ���ڽ� ī�� ���� ����  
			if ( FALSE == pUsingItemG->ItemInfo->IsExistDesParam(DES_CASH_STEALTH) )//&& pUsingItemG->ItemInfo->Kind == ITEMKIND_CARD)// 2013-08-20 by bckim, �޸��� ���� ���� 
			{
				m_pFieldIOCPSocket->ApplyItemDesParam(pUsingItemG->ItemInfo);
			}
			//m_pFieldIOCPSocket->ApplyItemDesParam(pUsingItemG->ItemInfo);
			// End. 2013-07-22 by bckim, ���ڽ� ī�� ���� ���� 
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ITEM_GENERAL *CFieldItemManager::FindUsingItemByItemNumNoLock(int i_nItemNum)
/// \brief		// 2007-06-15 by cmkwon, ��� �ð� ���� ������ �ý��� ����
/// \author		cmkwon
/// \date		2007-06-15 ~ 2007-06-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ITEM_GENERAL *CFieldItemManager::FindUsingItemByItemNumNoLock(int i_nItemNum)
{
	mt_auto_lock igLock(&m_mapItemGeneral);		// 2012-11-21 by hskim, ĳ���� ���� �״� ���� ����

	for(int i=0; i < m_vectUsingGeneralItem.size(); i++)
	{
		if(m_vectUsingGeneralItem[i]->ItemNum == i_nItemNum)
		{
			return m_vectUsingGeneralItem[i];
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-05 by cmkwon, ���ͺ��Ű��� ���� ī�� ���� - 2010-04-16 by cmkwon, �������� �߰� ����
/// \author		cmkwon
/// \date		2010-04-16 ~ 2010-04-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////			
ITEM_GENERAL *CFieldItemManager::FindUsingItemByDestParameterNoLock(DestParam_t i_DestParameter)		// 2011-08-01 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ��� ���� (DestParameter - 255 -> 32767 ����)
{
	mt_auto_lock igLock(&m_mapItemGeneral);		// 2012-11-21 by hskim, ĳ���� ���� �״� ���� ����

	for(int i=0; i < m_vectUsingGeneralItem.size(); i++)
	{
		if(m_vectUsingGeneralItem[i]->ItemInfo->IsExistDesParam(i_DestParameter))
		{
			return m_vectUsingGeneralItem[i];
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-05 by cmkwon, ���ͺ��Ű��� ���� ī�� ���� - 2010-04-16 by cmkwon, �������� �߰� ����
/// \author		cmkwon
/// \date		2010-04-16 ~ 2010-04-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::DeleteUsingTimeLimitedItemListByDestParameter(DestParam_t i_DestParameter)		// 2011-08-01 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ��� ���� (DestParameter - 255 -> 32767 ����)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);

	ITEM_GENERAL *pItemG = this->FindUsingItemByDestParameterNoLock(i_DestParameter);
	if(NULL == pItemG)
	{
		return FALSE;
	}

	return this->DeleteUsingTimeLimitedItemList(pItemG->ItemNum);
}


////////////////////////////////////////////////////////////////////////////
// 2013-02-28 by bckim, �������� �����߰�
//
////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::SetReturnItemDesParamInParty(CFieldIOCPSocket *i_pUseItemFISoc, ITEM* i_pItem)
{

	if( NULL == i_pUseItemFISoc || NULL == i_pItem )
	{	return FALSE;	}

	i_pUseItemFISoc->m_character.bUsingReturnItem = RETURN_USER_USING_ITEM0;		// RETURN_USER_USING_ITEM0 = 1
	i_pUseItemFISoc->SendCharacterOtherInfo( i_pUseItemFISoc->m_character.ClientIndex, TRUE );
	i_pUseItemFISoc->ApplyItemDesParam(i_pItem);
	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-01-18 by cmkwon, ������ ���� Parameter �ߺ� üũ �ý��� ���� - 
/// \author		cmkwon
/// \date		2010-01-18 ~ 2010-01-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ITEM *CFieldItemManager::CheckItemParamOverlapOfUsingItem(ITEM *i_pToUseItemInfo, INT i_nExcludeItemNum)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	for(int i=0; i < m_vectUsingGeneralItem.size(); i++)
	{
		ITEM_GENERAL *pUsingItemG = m_vectUsingGeneralItem[i];
		if(pUsingItemG->ItemNum == i_nExcludeItemNum)
		{// 2010-01-18 by cmkwon, üũ ���� ������
			continue;
		}

		if(FALSE == i_pToUseItemInfo->CheckParamOverlap(pUsingItemG->ItemInfo))
		{
			return pUsingItemG->ItemInfo;
		}
	}

	return NULL;
}
  

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldItemManager::InsertInsertingItemNumList2DB(int i_nItemNum)
/// \brief		// 2008-01-31 by cmkwon, �ð����� ������ DB �� �߰� �� üũ - CFieldItemManager::InsertInsertingItemNumList2DB() �߰�
/// \author		cmkwon
/// \date		2008-01-31 ~ 2008-01-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::InsertInsertingItemNumList2DB(int i_nItemNum)
{
	vectINT::iterator itr = find_if(m_vectInsertingItemNumList2DB.begin(), m_vectInsertingItemNumList2DB.end(), Sfind_if_INT(i_nItemNum));
	if(itr != m_vectInsertingItemNumList2DB.end())
	{
		return;
	}

	m_vectInsertingItemNumList2DB.push_back(i_nItemNum);
}


/****************************************************************************************************************************************************
**
**	DB ��� ���� �����۸�� üũ.
**
**	Create Info :	2008-01-31 ~ 2008-01-31	cmkwon.
**
**
**	Update Info :	- // 2008-01-31 by cmkwon, �ð����� ������ DB �� �߰� �� üũ - CFieldItemManager::IsExistInsertingItemNumList2DB() �߰�
**
**					- ������ ��Ͽ��� ȿ��(Ex:DESPARAM)��ġ�� �����۵� �߰��� üũ.	2010. 09. 17. by hsLee.
**
*****************************************************************************************************************************************************/
BOOL CFieldItemManager::IsExistInsertingItemNumList2DB(int i_nItemNum)
{

	// 2010. 09. 17. by hsLee.
	ITEM *pcUsingItem = ms_pFieldIOCP2->GetItemInfo ( i_nItemNum );
	
	if ( NULL == pcUsingItem )
		return FALSE;
	
	for(int i=0; i < m_vectInsertingItemNumList2DB.size(); i++)
	{
		ITEM *pcItem = ms_pFieldIOCP2->GetItemInfo( m_vectInsertingItemNumList2DB[i] ); 
		
		if ( NULL == pcItem || pcItem->ItemNum == i_nItemNum )
			continue;
		
		if( FALSE == pcItem->CheckParamOverlap( pcUsingItem ) )
		{
			return TRUE;
		}
	}
	// End 2010. 09. 17. by hsLee.
	
	vectINT::iterator itr = find_if(m_vectInsertingItemNumList2DB.begin(), m_vectInsertingItemNumList2DB.end(), Sfind_if_INT(i_nItemNum));
	if(itr == m_vectInsertingItemNumList2DB.end())
	{
		return FALSE;
	}

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldItemManager::DeleteInsertingItemNumList2DB(int i_nItemNum)
/// \brief		// 2008-01-31 by cmkwon, �ð����� ������ DB �� �߰� �� üũ - CFieldItemManager::DeleteInsertingItemNumList2DB() �߰�
/// \author		cmkwon
/// \date		2008-01-31 ~ 2008-01-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::DeleteInsertingItemNumList2DB(int i_nItemNum)
{
	vectINT::iterator itr = find_if(m_vectInsertingItemNumList2DB.begin(), m_vectInsertingItemNumList2DB.end(), Sfind_if_INT(i_nItemNum));
	if(itr == m_vectInsertingItemNumList2DB.end())
	{
		return;
	}

	itr = m_vectInsertingItemNumList2DB.erase(itr);
}

ITEM_BASE* CFieldItemManager::MakeNewItem(INT ItemNum,
										BYTE i_enumStorage,
										ITEM *pItemInfo, UID64_t ItemUniqueNumber)
{
	ITEM_BASE *pRetItem = NULL;

	if (pItemInfo == NULL)
	{
		pItemInfo = ms_pFieldIOCP2->GetItemInfo(ItemNum);
		if (pItemInfo == NULL)
		{
			DBGOUT("Error! No such item(%d) at MakeNewItem()\n", ItemNum);
			return NULL;
		}
	}

	if (IS_GENERAL_ITEM(pItemInfo->Kind))
	{
		ITEM_GENERAL *pItem			= new ITEM_GENERAL(pItemInfo);
		pItem->UniqueNumber			= ItemUniqueNumber;
		pItem->AccountUniqueNumber	= m_pFieldIOCPSocket->m_character.AccountUniqueNumber;
		pItem->Possess				= m_pFieldIOCPSocket->m_character.CharacterUniqueNumber;
		pItem->ItemStorage			= i_enumStorage;
		pItem->Wear					= WEAR_NOT_ATTACHED;
		pItem->CurrentCount			= (IS_CHARGABLE_ITEM(pItemInfo->Kind)?pItemInfo->Charging:1);
// 2009-08-25 by cmkwon, ������� �ʴ� �ʵ� ����(td_Store.ScarcityNumber) - 
//		pItem->ScarcityNumber		= 0;
		pItem->ItemWindowIndex		= POS_INVALID_POSITION;
		pItem->NumOfEnchants		= 0;
		
		pRetItem					= pItem;
	}
	else if (IS_SKILL_ITEM(pItemInfo->Kind))
	{
		ITEM_SKILL *pItem			= new ITEM_SKILL(pItemInfo);
		pItem->UniqueNumber			= ItemUniqueNumber;
		pItem->AccountUniqueNumber	= m_pFieldIOCPSocket->m_character.AccountUniqueNumber;
		pItem->Possess				= m_pFieldIOCPSocket->m_character.CharacterUniqueNumber;
		pItem->ItemWindowIndex		= POS_INVALID_POSITION;

		pRetItem					= pItem;
	}
	else
	{
		// check: error
		DBGOUT("Error! No such item Type(%s, %d) --> type: %d, at MakeNewItem()\n",
			pItemInfo->ItemName, pItemInfo->ItemNum, pItemInfo->Kind
		);
		return NULL;
	}

	return pRetItem;
}

// 2010-06-15 by shcho&hslee ��ý��� - 
/********************************************************************************************
**
**	�� ������ ����.
**
**	Create Info :	2010-06-15 by shcho&hslee
**
*********************************************************************************************/
tPET_CURRENTINFO* CFieldItemManager :: MakeNewPetData ( UID64_t i_nPetUID , const tPET_BASEDATA *a_pPetBaseData , const tPET_LEVEL_DATA *a_pPetLevelData /*= NULL*/ )
{

	tPET_CURRENTINFO *RetPetData = NULL;

	if ( NULL == a_pPetBaseData )
		return NULL;

	tPET_CURRENTINFO *pInputPetData = new tPET_CURRENTINFO(NULL);

	if ( pInputPetData )
	{

// 		pInputPetData->CreatedPetUID = i_nPetUID;
// 
// 		pInputPetData->EnableEditPetName = true;
// 		strcpy ( pInputPetData->szPetName , a_pPetBaseData->szPetName );
// 
// 		pInputPetData->PetIndex = a_pPetBaseData->PetIndex;
// 		pInputPetData->PetLevel = a_pPetBaseData->PetLevel;
// 
// 		pInputPetData->PetExp = a_pPetLevelData != NULL ? a_pPetLevelData->NeedExp : 0;
// 
// 		pInputPetData->Stamina = a_pPetBaseData->Stamina;

// 		ITEM *psItem = NULL;
// 
// 		for ( int i = 0; i < SIZE_MAX_PETSKILLITEM; ++i )
// 		{
// 
// 			psItem = ms_pFieldIOCP2->GetItemInfo( a_pPetBaseData->PetSkillItemIndex[i] );
// 
// 			if ( NULL == psItem )
// 				continue;
// 
// 		}

		RetPetData = pInputPetData;

	}
	

	return RetPetData;

}


BOOL CFieldItemManager::InsertItemGeneralByPointer(ITEM_GENERAL *i_pItemGeneral
												   , BYTE i_nItemUpdateType
												   , BOOL i_bCheckGeneralItemCounts/*=FALSE*/
												   , BYTE i_enumStorage/*=ITEM_IN_CHARACTER*/
												   , BOOL i_bUpdateDB/*=TRUE*/
												   , BOOL i_bSendMSG/*=TRUE*/
												   , UID32_t i_tradeCharacterUID/*=INVALID_UNIQUE_NUMBER*/		// 2006-05-15 by cmkwon
												   , BOOL i_bCheckGamePlayTime/*=TRUE*/							// 2008-03-28 by cmkwon, PlayTime ���� ���� ���� ���� - 
												   , int  i_nGLogItemtype/*=0*/									// 2012-11-21 by khkim, GLog 2��
												   )
{
// 2008-04-08 by cmkwon, ���� ���� �ý��� ���� - �Ʒ��� �ڵ�� �ʿ� ����, ������� ó���ϰ� �������� �׻� 100%
// 	// 2008-03-28 by cmkwon, PlayTime ���� ���� ���� ���� - ���⿡���� �ʿ�� üũ ����� ��
// 	if( i_bCheckGamePlayTime
// 		&& (IUT_DROP_ITEM == i_nItemUpdateType || IUT_QUEST == i_nItemUpdateType) )
// 	{
// 		float	fApplyRate		= 0.0f;
// 		BOOL	bRestriction	= m_pFieldIOCPSocket->GPGetApplyRateByGamePlayingTime(&fApplyRate);
// 		if(bRestriction)
// 		{
// 			if(0.0f >= fApplyRate)
// 			{
// 				// 2008-03-28 by cmkwon, �������� �������.
// 				util::del(i_pItemGeneral);
// 				return TRUE;
// 			}
// 
// 			if(IS_COUNTABLE_ITEM(i_pItemGeneral->Kind))
// 			{
// 				i_pItemGeneral->CurrentCount	= (i_pItemGeneral->CurrentCount+1)*fApplyRate;
// 				if(0 >= i_pItemGeneral->CurrentCount)
// 				{					
// 					// 2008-03-28 by cmkwon, �������� �������.
// 					util::del(i_pItemGeneral);
// 					return TRUE;
// 				}
// 			}
// 		}
//	}

	// lock m_mapItemGeneral, @InsertItemGeneralByPointer
	mt_auto_lock igLock(&m_mapItemGeneral);

// 2006-11-17 by cmkwon, �ý��� �������� �ʿ� ����
//	if(FALSE == m_pFieldIOCPSocket->IsValidGameUser()
//		&& (IUT_DROP_ITEM == i_nItemUpdateType || IUT_QUEST == i_nItemUpdateType))
//	{// 2006-06-14 by cmkwon, ��Ʈ�� ������ üũ - ��� ������ �߰�
//		return FALSE;
//	}

	if (!IS_GENERAL_ITEM(i_pItemGeneral->ItemInfo->Kind))
	{
		// ITEM_GENERAL�� �ƴϸ� ������ �� ����.
		return FALSE;
	}

	if (IS_COUNTABLE_ITEM(i_pItemGeneral->Kind)
		&& i_pItemGeneral->CurrentCount < 0)
	{
		// ������ 0���� ������ error��
		return FALSE;
	}

//#ifdef _DEBUG
//	if (IS_COUNTABLE_ITEM(i_pItemGeneral->ItemInfo->Kind))
//	{
//		m_pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD,
//				"������ \'%s\'��(��) %d�� %s�߰��Ǿ����ϴ�",
//				i_pItemGeneral->ItemInfo->ItemName, i_pItemGeneral->CurrentCount,
//				((i_enumStorage==ITEM_IN_STORE)?"â�� ":""));
//	}
//	else
//	{
//		m_pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, "������ \'%s\'��(��) 1�� %s�߰��Ǿ����ϴ�",
//				i_pItemGeneral->ItemInfo->ItemName,
//				((i_enumStorage==ITEM_IN_STORE)?"â�� ":""));
//	}
//#endif

	// start 2012-11-21 by khkim, GLog 2��
	if(!COMPARE_RACE(this->m_pCharacter->Race, RACE_OPERATION|RACE_GAMEMASTER|RACE_MONITOR|RACE_GUEST|RACE_DEMO))		// 2013-01-18 by khkim, GLog (�Ϲ������� ���)	
	{
		if( NULL != m_pFieldIOCPSocket )
		{
			switch((FALSE == i_nGLogItemtype)? i_nItemUpdateType : i_nGLogItemtype)
			{
			case IUT_DROP_ITEM:
				{
					SetGLogData(GLOG_IL_LT_DROP,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case IUT_QUEST:
				{
					SetGLogData(GLOG_IL_LT_QUEST,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case IUT_SHOP:
				{
					SetGLogData(GLOG_IL_LT_SHOP_BUY,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case GLOG_WARPOINTSHOP:
				{
					SetGLogData(GLOG_IL_LT_WARPOINTSHOP_BUY,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case GLOG_INFINITYOBSHOP:
				{
					SetGLogData(GLOG_IL_LT_INFINITYOBSHOP_BUY,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case GLOG_CASHSHOP:
				{
					SetGLogData(GLOG_IL_LT_CASHSHOP_BUY,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case IUT_LUCKY_ITEM:
				{
					SetGLogData(GLOG_IL_LT_LUCKYMACHINE_GET,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case IUT_BAZAAR_SELL:
				{
					SetGLogData(GLOG_IL_LT_BAZAARSELL_GET,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case IUT_BAZAAR_BUY:
				{
					SetGLogData(GLOG_IL_LT_BAZAARBUY_GET,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case IUT_TRADE:
				{
					SetGLogData(GLOG_IL_LT_TREADGET,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case IUT_MIXING:
				{
					SetGLogData(GLOG_IL_LT_MIXING_GET,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case IUT_ENCHANT:
				{
					SetGLogData(GLOG_IL_LT_ENCHANT_UP,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case IUT_DISSOLUTION_ITEM:
				{
					SetGLogData(GLOG_IL_LT_DISSOLUTION_GETITEM,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case IUT_RANDOMBOX:
 				{
					SetGLogData(GLOG_IL_LT_RANDOMBOX,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
 				}
				// 2013-05-22 by jhseol, �ҽ����� - �ּ�����
			default:
				{
					break;
				}
			}
		}
	}
	// end 2012-11-21 by khkim, GLog 2��


	///////////////////////////////////////////////////////////////////////////
	// countable item�̰� �̹� �����ϴ� ���� update count
	///////////////////////////////////////////////////////////////////////////
	if (IS_COUNTABLE_ITEM(i_pItemGeneral->ItemInfo->Kind)
		&& (// 2006-09-29 by cmkwon, ITEM_ATTR_TIME_LIMITE�� üũ�ϵ��� ���� ITEMKIND_CARD != i_pItemGeneral->Kind
			FALSE == COMPARE_BIT_FLAG(i_pItemGeneral->ItemInfo->ItemAttribute, ITEM_ATTR_TIME_LIMITE|ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED)	// 2008-11-26 by cmkwon, ��� �� �ð�(����ð�) ���� ������ ���� - 
			|| ITEM_NOT_USING == i_pItemGeneral->UsingTimeStamp)
		)
	{
		// Countable Item �� ���
		ITEM_GENERAL *pItemCountableFound = GetFirstItemGeneralByItemNum(i_pItemGeneral->ItemNum, i_enumStorage);

		if (pItemCountableFound != NULL)
		{
			// �̹� �����ϸ�
			UpdateItemCountByPointer(pItemCountableFound, i_pItemGeneral->CurrentCount, i_nItemUpdateType, i_enumStorage, i_bUpdateDB, i_bSendMSG);

// 2008-01-30 by cmkwon, ���ӷα� �ι� ����Ǵ� ���� ���� - ����� �ּ� ó���ϰ� UpdateItemCountByPointer() �Լ��ȿ��� ó���Ѵ�.
// 			///////////////////////////////////////////////////////////////////////////////
// 			// 2007-01-11 by cmkwon, ī���ͺ� �������� ���⿡�� �α׸� ���ܾ� �Ѵ�.
// 			switch(i_nItemUpdateType)
// 			{
// 			case IUT_DROP_ITEM:
// 			case IUT_QUEST:			// 2007-10-30 by cmkwon, ����Ʈ ������ ���� ���� �α׿� �߰�
// 				CAtumLogSender::SendLogMessageITEMGetITEM(m_pFieldIOCPSocket, pItemCountableFound, i_pItemGeneral->CurrentCount);
// 				break;
// 			case IUT_GIVEEVENTITEM:		// 2006-08-28 by dhjin
// 			case IUT_BONUS_ITEM:		// 2007-09-11 by cmkwon, ��ɾ� �̺�Ʈ ������ ���� ���ӷα׿� �߰��ϱ�
// 			case IUT_GIVEEVENTITEM_COUPONEVENT:		// 2008-01-10 by cmkwon, ������ �̺�Ʈ �ý��ۿ� �� ���� �ý��� �߰� - 
// 				CAtumLogSender::SendLogMessageITEMGetItemByItemEvent(m_pFieldIOCPSocket, pItemCountableFound, i_pItemGeneral->CurrentCount);
// 				break;
// 			}

			// check: �ϴ� ����� �ɷ� ���������� �̰� ���� ���� �Ǵ��ϱ�? 20031029, kelovon
			util::del(i_pItemGeneral);
			return TRUE;
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// �� �̿��� ��쿡�� ���� - UniqueNumber�� �޾ƾ� �ϱ� ������ QP_InsertStoreItem() ���Ŀ� ó��
	///////////////////////////////////////////////////////////////////////////
	// 2009-11-02 by cmkwon, ĳ��(�κ�/â�� Ȯ��) ������ �߰� ���� - 
	if(i_bCheckGeneralItemCounts
		&& this->GetInventoryItemCounts() >= this->GetMaxInventoryCount())
	{// �κ��丮�� �� ��

		// 2007-01-04 by cmkwon, �ý��۷α׿� ���� �ʵ��� ����
		m_pFieldIOCPSocket->SendErrorMessage(T_FIELD_IOCP, ERR_PROTOCOL_INVENTORY_FULL_ITEM, this->GetMaxInventoryCount(), this->GetInventoryItemCounts(), NULL, FALSE, FALSE);
		return FALSE;
	}

	if (i_bUpdateDB)
	{
		m_pFieldIOCPSocket->IncrementInsertingStoreCounts();
		
		if (ITEM_IN_STORE == i_enumStorage)
		{// â�� ��� ���̶��

			if(FALSE == IS_VALID_UNIQUE_NUMBER(m_uid32StoreCharacterUID))
			{// 2006-09-20 by dhjin, üũ
				i_pItemGeneral->Possess		= m_pCharacter->CharacterUniqueNumber;
			}
			else
			{
				i_pItemGeneral->Possess		= m_uid32StoreCharacterUID;				// 2006-09-20 by dhjin, �ɸ��Ͱ� â�� ���� -> ���õ� �ɸ��� UID
			}
		}

		m_pFieldDBManager->MakeAndEnqueueQuery(QT_InsertStoreItem, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, i_pItemGeneral, NULL, i_nItemUpdateType, i_tradeCharacterUID);
	}
	else
	{// 2006-05-11 by cmkwon, DB�� ������Ʈ ���� ������ �ٷ� m_mapItemGeneral�� �߰��Ѵ�.
		i_pItemGeneral->Possess		= m_pCharacter->CharacterUniqueNumber;		// 2006-09-20 by dhjin, �ɸ��Ͱ� â�� ���� -> ���õ� �ɸ��� UID
		m_mapItemGeneral.insertLock(i_pItemGeneral->UniqueNumber, i_pItemGeneral);
		// 2013-06-25 by jhseol, ������ UID '0' ������ ����� �α�
#ifdef S_ITEMUID_ZERO_DEBUG_LOG_JHSEOL
		if (IS_COUNTABLE_ITEM(i_pItemGeneral->Kind))
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DEBUG] SPI ItemUID Zero : CFieldItemManager::InsertItemGeneralByPointer - CUID(%d), ItemNum(%d), Count(%d), ItemUID(%d)\r\n"
				, i_pItemGeneral->Possess, i_pItemGeneral->ItemNum, i_pItemGeneral->CurrentCount, i_pItemGeneral->UniqueNumber);
		}
#endif
		// end 2013-06-25 by jhseol, ������ UID '0' ������ ����� �α�
		this->ItemInsertByInfinityPlaying(i_pItemGeneral);			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� �� ������ ����
		m_pFieldIOCPSocket->Check8SendAFSUpdateItem(i_pItemGeneral);			// 2010-03-31 by dhjin, ���Ǵ�Ƽ ���� ĳ�� ������ ���� - 
		if(i_bSendMSG)
		{
			INIT_MSG_WITH_BUFFER(MSG_FC_STORE_INSERT_ITEM, T_FC_STORE_INSERT_ITEM, pInsertItem, SendBuf);
			pInsertItem->FromCharacterUniqueNumber	= 0;
			pInsertItem->ItemInsertionType			= i_nItemUpdateType;
			pInsertItem->ItemGeneral				= *i_pItemGeneral;
			m_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_STORE_INSERT_ITEM));
		}
	}

	return TRUE;
}

// 2010-06-15 by shcho&hslee ��ý��� - ���� �� ���� ����
BOOL CFieldItemManager::InsertItemPetByPointer( tPET_CURRENTINFO *i_pItemPet )
{

	if ( NULL == i_pItemPet ) 
		return FALSE;

	mt_auto_lock mtAuto(&m_vectPetCurrentInfo);
	if(this->GetItemPetByUID(i_pItemPet->CreatedPetUID))
	{
		return FALSE;
	}
	m_vectPetCurrentInfo.push_back(i_pItemPet);
	return TRUE;

}
// ��ų�ε���, ��ų���Խ� ����
BOOL CFieldItemManager::InsertItemSkillByPointer(UID64_t i_SkillItemUID, ITEM_SKILL *i_pItemSkill)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	if(this->GetItemSkillByUID(i_pItemSkill->UniqueNumber))
	{
		return FALSE;
	}
	m_vectItemSkillPtr.push_back(i_pItemSkill);
	return TRUE;
}

BOOL CFieldItemManager::InsertStoreItemByPointer(UID64_t i_n64ItemUID, ITEM_GENERAL *i_pItemG)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	if(i_pItemG->Possess != m_uid32StoreCharacterUID)
	{
		return FALSE;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2009-10-30 by cmkwon, â�� ������ �������� ���� ���� ���� - CFieldItemManager::InsertStoreItemByPointer#,
	if(NULL == m_pFieldIOCPSocket->m_pCurrentBuildingNPC
		|| BUILDINGKIND_STORE != m_pFieldIOCPSocket->m_pCurrentBuildingNPC->BuildingKind)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldItemManager::InsertStoreItemByPointer State error !! %s %s pBuildingNPC(0x%X) BuildingKind(%d) \r\n"
			, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), GetItemGeneralString(i_pItemG, string()), m_pFieldIOCPSocket->m_pCurrentBuildingNPC, (NULL == m_pFieldIOCPSocket->m_pCurrentBuildingNPC)?-1:m_pFieldIOCPSocket->m_pCurrentBuildingNPC->BuildingKind);
		return FALSE;
	}

	m_mapItemInStore.insertNoLock(i_n64ItemUID, i_pItemG);
	return TRUE;
}

// 2007-07-25 by cmkwon, �̺�Ʈ �������� ����/���� ���� ���� - CFieldItemManager::InsertItemBaseByItemNum()�Լ� ���� �߰�(, int i_nPrefixCodeNum/*=0*/, int i_nSuffixCodeNum/*=0*/)
// 2010-02-11 by cmkwon, ���ս� ����,����Ʈ ���� �ý��� ���� - �����߰�(, INT i_nShapeItemNum=0, INT i_nEffectItemNum=0)
BOOL CFieldItemManager::InsertItemBaseByItemNum(INT i_nItemNum, INT i_nItemCount
												, BYTE i_nItemUpdateType
												, BOOL i_bCheckGeneralItemCounts/*=FALSE*/
												, BYTE i_enumStorage/*=ITEM_IN_CHARACTER*/
												, BOOL i_bUpdateDB/*=TRUE*/
												, BOOL i_bSendMSG/*=TRUE*/
												, INT i_nPrefixCodeNum/*=0*/
												, INT i_nSuffixCodeNum/*=0*/
												, INT i_nShapeItemNum/*=0*/
												, INT i_nEffectItemNum/*=0*/
												, int i_nGLogItemtype/*=0*/)			//  2012-11-21 by khkim, GLog 2��
{
	ITEM *pItemInfo = ms_pFieldIOCP2->GetItemInfo(i_nItemNum);
	if (pItemInfo == NULL)
	{
		DBGOUT("FATAL ERROR@CFieldItemManager::InsertItemBaseByItemNum(): T_FIELD_IOCP, ERR_PROTOCOL_NO_SUCH_ITEM, ItemNum, 0\r\n");
		return FALSE;
	}

	if (IS_GENERAL_ITEM(pItemInfo->Kind))
	{
		if (IS_COUNTABLE_ITEM(pItemInfo->Kind))
		{
			ITEM_GENERAL *pItemGeneral = (ITEM_GENERAL*)MakeNewItem(pItemInfo->ItemNum);
			if (pItemGeneral == NULL) { return FALSE; }
			pItemGeneral->ItemStorage	= i_enumStorage;
			pItemGeneral->CurrentCount	= i_nItemCount;

			// 2008-03-28 by cmkwon, PlayTime ���� ���� ���� ���� - �Ʒ��� ���� ���� ������, PlayTime ���� ���� üũ ���� �ʾƾ� ��, �� ������ �̹� ó���� �Ǿ� ����
			//BOOL bRet = InsertItemGeneralByPointer(pItemGeneral, i_nItemUpdateType, i_bCheckGeneralItemCounts, i_enumStorage, i_bUpdateDB, i_bSendMSG);
			BOOL bRet = InsertItemGeneralByPointer(pItemGeneral, i_nItemUpdateType, i_bCheckGeneralItemCounts, i_enumStorage, i_bUpdateDB, i_bSendMSG, INVALID_UNIQUE_NUMBER, FALSE, i_nGLogItemtype);	//  2012-11-21 by khkim, GLog 2��
			if (!bRet)
			{
				util::del(pItemGeneral);		// 2006-06-14 by cmkwon
				return FALSE;
			}
		}
		else
		{
			for (int i = 0; i < i_nItemCount; i++)
			{
				ITEM_GENERAL *pItemGeneral = (ITEM_GENERAL*)MakeNewItem(pItemInfo->ItemNum);
				if (pItemGeneral == NULL) { return FALSE; }
				pItemGeneral->ItemStorage	= i_enumStorage;

				//////////////////////////////////////////////////////////////////////////
				// 2007-07-25 by cmkwon, �̺�Ʈ �������� ����/���� ���� ���� - ���ڷ� ���� ���� �����ϱ�
				if(0 != i_nPrefixCodeNum)
				{
					if(ms_pFieldIOCP2->m_mapRareItemInfo.findEZ_ptr(i_nPrefixCodeNum))					
					{
						pItemGeneral->PrefixCodeNum = i_nPrefixCodeNum;
					}
				}
				if(0 != i_nSuffixCodeNum)
				{
					if(ms_pFieldIOCP2->m_mapRareItemInfo.findEZ_ptr(i_nSuffixCodeNum))
					{
						pItemGeneral->SuffixCodeNum = i_nSuffixCodeNum;
					}
				}

				///////////////////////////////////////////////////////////////////////////////
				// 2010-02-11 by cmkwon, ���ս� ����,����Ʈ ���� �ý��� ���� - 
				if(0 != i_nShapeItemNum)
				{
					if(ms_pFieldIOCP2->GetItemInfo(i_nShapeItemNum))
					{
						pItemGeneral->ShapeItemNum	= i_nShapeItemNum;
					}
				}
				if(0 != i_nEffectItemNum)
				{
					if(ms_pFieldIOCP2->GetItemInfo(i_nEffectItemNum))
					{
						pItemGeneral->ColorCode		= i_nEffectItemNum;
					}
				}

				// 2008-03-28 by cmkwon, PlayTime ���� ���� ���� ���� - �Ʒ��� ���� ���� ������, PlayTime ���� ���� üũ ���� �ʾƾ� ��, �� ������ �̹� ó���� �Ǿ� ����
				//BOOL bRet = InsertItemGeneralByPointer(pItemGeneral, i_nItemUpdateType, i_bCheckGeneralItemCounts, i_enumStorage, i_bUpdateDB, i_bSendMSG);
				BOOL bRet = InsertItemGeneralByPointer(pItemGeneral, i_nItemUpdateType, i_bCheckGeneralItemCounts, i_enumStorage, i_bUpdateDB, i_bSendMSG, INVALID_UNIQUE_NUMBER, FALSE, i_nGLogItemtype); //  2012-11-21 by khkim, GLog 2��
				if (!bRet)
				{
					util::del(pItemGeneral);		// 2006-06-14 by cmkwon
					return FALSE;
				}
			}
		}
	}
	else if (IS_SKILL_ITEM(pItemInfo->Kind))
	{
		// ���� ��ų�� �ְ� ������ ������ ���
		// 2013-03-12 by jhseol, ��ų �ѹ��� �ý��� ����
#ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
		ITEM_SKILL *pItemSkillOld = GetFirstSkillBySkillBaseNum(pItemInfo->NextSkill);
		if (pItemSkillOld != NULL && pItemSkillOld->ItemInfo->SkillLevel > pItemInfo->SkillLevel)
		{
			return TRUE;
		}
#else	// #ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
		ITEM_SKILL *pItemSkillOld = GetFirstSkillBySkillBaseNum(pItemInfo->ItemNum);
		if (pItemSkillOld != NULL && SKILL_LEVEL(pItemSkillOld->ItemNum) > SKILL_LEVEL(pItemInfo->ItemNum))
		{
			return TRUE;
		}
#endif	// #ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
		// end 2013-03-12 by jhseol, ��ų �ѹ��� �ý��� ����

		// ��ų �߰�
		ITEM_SKILL *pItemSkill = (ITEM_SKILL*)MakeNewItem(pItemInfo->ItemNum, ITEM_IN_CHARACTER, pItemInfo);
		if (pItemSkill == NULL || (pItemSkill->ItemInfo->ReqUnitKind&m_pFieldIOCPSocket->m_character.UnitKind) == 0)
		{
			m_pFieldIOCPSocket->SendErrorMessage(T_FIELD_IOCP, ERR_PROTOCOL_REQ_UNITKIND_NOT_MATCHED);
			return FALSE;
		}
		m_pFieldDBManager->MakeAndEnqueueQuery(QT_InsertStoreItemSkill, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pItemSkill, NULL, i_nItemUpdateType);
	}

	return TRUE;
}

BOOL CFieldItemManager::InsertItemGeneralByItemNum(INT i_nItemNum, INT i_nItemCount,
							BYTE i_nItemUpdateType, BYTE i_enumStorage,
							BOOL i_bUpdateDB, BOOL i_bSendMSG
							, UID32_t i_tradeCharacterUID/*=INVALID_UNIQUE_NUMBER*/		// 2006-05-15 by cmkwon
							)
{
	ITEM_GENERAL *pItemGeneral = (ITEM_GENERAL*)MakeNewItem(i_nItemNum, i_enumStorage);

	if (pItemGeneral == NULL || !IS_GENERAL_ITEM(pItemGeneral->Kind))
	{
		util::del(pItemGeneral);
		return FALSE;
	}

	if (IS_COUNTABLE_ITEM(pItemGeneral->Kind))
	{
		pItemGeneral->CurrentCount = i_nItemCount;
	}
	else
	{
		ASSERT_NOT_IMPLEMENTED_YET();
		util::del(pItemGeneral);
		return FALSE;
	}

	if(FALSE == InsertItemGeneralByPointer(pItemGeneral, i_nItemUpdateType, FALSE, i_enumStorage, i_bUpdateDB, i_bSendMSG, i_tradeCharacterUID))
	{
		util::del(pItemGeneral);
		return FALSE;
	}

	return TRUE;
}

BOOL CFieldItemManager::UpdateItemCountByPointer(ITEM_GENERAL *i_pItemGeneral,
							INT i_nChangeCount,
							BYTE i_nItemUpdateType, BYTE i_enumStorage,
							BOOL i_bUpdateDB, BOOL i_bSendMSG, BOOL i_bDeleteMemory,
							int i_nGLogItemtype/*=0*/ )			//  2012-11-21 by khkim, GLog 2��
	{
	if(NULL == i_pItemGeneral) {
		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ����, NULL �� üũ �߰�
		return FALSE;
	}
	///////////////////////////////////////////////////////////////////////////
	// ����� �� Countable Item�� ���ؼ��� �����, nCount��ŭ CurrentCount�� ��ȭ��Ŵ
	// nCount < 0�̸� ������ ������ ���̰�, nCount > 0�̸� ������ ���δ�
	// ���� ���� 0���� ������ �������� �����Ѵ�
	///////////////////////////////////////////////////////////////////////////
	if(0 == i_pItemGeneral->UniqueNumber)
	{
		char szSysLog[1024];
		sprintf(szSysLog, "[ERROR] CFieldItemManager::UpdateItemCountByPointer_ ItemUID Error, %s %s ChangeCount(%d) ItemUpdateType(%d)\r\n"
			, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), GetItemGeneralString(i_pItemGeneral, string())
			, i_nChangeCount, i_nItemUpdateType);
		g_pFieldGlobal->WriteSystemLog(szSysLog);
		DbgOut(szSysLog);
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
	{
		// countable item�� ���
		if (i_pItemGeneral->CurrentCount <= 0 && i_pItemGeneral->ItemNum != MONEY_ITEM_NUMBER)
		{
			m_pFieldIOCPSocket->SendErrorMessage(T_FIELD_IOCP, ERR_PROTOCOL_INVALID_ITEM_COUNT, i_pItemGeneral->UniqueNumber, i_pItemGeneral->CurrentCount, i_pItemGeneral->ItemInfo->ItemName);
			m_pFieldIOCPSocket->DBGOUT2(DO2_ALL, "FATAL ERROR: Item Count �̻�! %s %s\r\n"
					, GetCharacterString(&m_pFieldIOCPSocket->m_character, string())
					, GetItemGeneralString(i_pItemGeneral, string())
			);
			return FALSE;
		}

		///////////////////////////////////////////////////////////////////////////////
		// 2005-09-13 by cmkwon, ī���ͺ� ������ �ִ� ī��Ʈ üũ
		INT64 tmCurrentCount = (INT64)i_pItemGeneral->CurrentCount + (INT64)i_nChangeCount;
		if(tmCurrentCount > MAX_NOTIFY_ITEM_COUNTS)
		{
			m_pFieldIOCPSocket->SendErrorMessage(T_FIELD_IOCP, ERR_PROTOCOL_NOTIFY_MAX_ITEM_COUNTS_OVER, 0, 0);
		}
	}

	// start 2012-01-16 by hskim, ��� - ȭ��
	if( NULL != ms_pFieldIOCP2->m_pStatisticsManager )
	{
		if( MONEY_ITEM_NUMBER == i_pItemGeneral->ItemNum )
		{
			if( i_nChangeCount < 0 )
			{
				ms_pFieldIOCP2->m_pStatisticsManager->Sub(STATISTICS_SPI, i_nChangeCount);
			}
			else if( i_nChangeCount > 0 )
			{
				ms_pFieldIOCP2->m_pStatisticsManager->Add(STATISTICS_SPI, i_nChangeCount);
			}
		}
	}
	// end 2012-01-16 by hskim, ��� - ȭ��

	// Item Count �����ϱ�
	INT64 n64Counts = i_pItemGeneral->CurrentCount;
	n64Counts = min((INT64)n64Counts + (INT64)i_nChangeCount, MAX_ITEM_COUNTS);
	if (n64Counts <= 0
		&& IS_COUNTABLE_ITEM(i_pItemGeneral->ItemInfo->Kind)
		&& i_pItemGeneral->ItemNum != MONEY_ITEM_NUMBER)
	{
		// �����
		if(CFieldItemManager::IsDelayStoreItem(i_pItemGeneral->ItemInfo))
		{
			this->DeleteDelayStoredItemInfoByItemUID(i_pItemGeneral->UniqueNumber);
		}
		DeleteItemFromCharacterByPointer(i_pItemGeneral, i_nItemUpdateType, i_enumStorage, i_bUpdateDB, i_bSendMSG, i_bDeleteMemory, i_nGLogItemtype);  //  2012-11-21 by khkim, GLog 2��
	}
	else if(i_pItemGeneral->CurrentCount != n64Counts)
	{
		// ���� ������Ʈ
		i_pItemGeneral->CurrentCount = max(0, n64Counts);

		// Update DB
		if (i_bUpdateDB)
		{
			if(0 == i_pItemGeneral->CurrentCount		// 2006-05-21 by cmkwon, SPI�� ���
				|| (IUT_USE_ENERGY != i_nItemUpdateType && IUT_DROP_ITEM != i_nItemUpdateType)
				|| FALSE == CFieldItemManager::IsDelayStoreItem(i_pItemGeneral->ItemInfo)	// 2006-05-21 by cmkwon, ī��Ʈ ��纯�� �������� ó��
				)
			{
				////////////////////////////////////////////////////////////////////////////////
				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ذ� ���� ����, ���� ���� �� ������ ����
//				QPARAM_UPDATEITEMCOUNT *pQParam	= new QPARAM_UPDATEITEMCOUNT;
//				pQParam->ItemUniqueNumber		= i_pItemGeneral->UniqueNumber;
//				pQParam->Count					= i_pItemGeneral->CurrentCount;
//				ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemCount, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
				if(FALSE == g_pFieldGlobal->IsArenaServer()) {
					QPARAM_UPDATEITEMCOUNT *pQParam	= new QPARAM_UPDATEITEMCOUNT;
					pQParam->ItemUniqueNumber		= i_pItemGeneral->UniqueNumber;
					pQParam->Count					= i_pItemGeneral->CurrentCount;
					ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemCount, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
				}
				else {
					this->ItemUpdateByInfinityPlaying(i_pItemGeneral);
				}
			}
		}

		// start 2012-11-21 by khkim, GLog 2�� 
		if(!COMPARE_RACE(this->m_pCharacter->Race, RACE_OPERATION|RACE_GAMEMASTER|RACE_MONITOR|RACE_GUEST|RACE_DEMO))		// 2013-01-18 by khkim, GLog (�Ϲ������� ���)	
		{
			if( NULL != m_pFieldIOCPSocket )
			{
				if( 0 > i_nChangeCount )					
				{ 
					switch((FALSE == i_nGLogItemtype)? i_nItemUpdateType : i_nGLogItemtype)
					{
					case IUT_SHOP:
						{
							SetGLogData(GLOG_IL_LT_SHOP_SELL, i_pItemGeneral, m_pFieldIOCPSocket->m_character.Level, i_nChangeCount);
							break;
						}
					case IUT_BAZAAR_SELL:
						{
							SetGLogData( GLOG_IL_LT_BAZAARSELL_GIVE, i_pItemGeneral, m_pFieldIOCPSocket->m_character.Level, i_nChangeCount);
							break;
						}
					case IUT_BAZAAR_BUY:
						{
							SetGLogData(GLOG_IL_LT_BAZAARBUY_GIVE, i_pItemGeneral, m_pFieldIOCPSocket->m_character.Level, i_nChangeCount);
							break;
						}
					case IUT_TRADE:
						{
							SetGLogData(GLOG_IL_LT_TREADGIVE, i_pItemGeneral, m_pFieldIOCPSocket->m_character.Level, i_nChangeCount);
							break;
						}
					case IUT_LUCKY_ITEM:
						{
							SetGLogData(GLOG_IL_LT_LUCKYMACHINE_USE, i_pItemGeneral, m_pFieldIOCPSocket->m_character.Level, i_nChangeCount);
							break;
						}
					case IUT_GENERAL:
						{
							SetGLogData(GLOG_IL_LT_THROWAWAY, i_pItemGeneral, m_pFieldIOCPSocket->m_character.Level, i_nChangeCount);
							break;
						}
					case IUT_USE_ITEM:
						{
							SetGLogData(GLOG_IL_LT_USE_USEITEM, i_pItemGeneral, m_pFieldIOCPSocket->m_character.Level, i_nChangeCount);
							break;
						}
					case IUT_ENCHANT:
						{
							SetGLogData(GLOG_IL_LT_ENCHANT_USEITEM, i_pItemGeneral, m_pFieldIOCPSocket->m_character.Level, i_nChangeCount);
							break;
						}
					case IUT_MIXING:
						{
							SetGLogData(GLOG_IL_LT_MIXING_USE, i_pItemGeneral, m_pFieldIOCPSocket->m_character.Level, i_nChangeCount);
							break;
						}
					default:
						{
							break;
						}
					}
				}
			}
		}
		// end 2012-11-21 by khkim, GLog 2��


		// 2007-10-30 by cmkwon, ����Ʈ ������ ���� ���� �α׿� �߰�  - �Ʒ��� ���� ������
		//// �α� �����, 20040820, kelovon
		//if (i_nItemUpdateType == IUT_DROP_ITEM)
		//{
		//	CAtumLogSender::SendLogMessageITEMGetITEM(m_pFieldIOCPSocket, i_pItemGeneral, i_nChangeCount);
		//}
// 2008-01-30 by cmkwon, ���ӷα� �ι� ����Ǵ� ���� ���� - �Ʒ��� ���� �����ϴ� �α� i_nItemUpdateType �� �߰���
// 		switch(i_nItemUpdateType)
// 		{
// 		case IUT_DROP_ITEM:
// 		case IUT_QUEST:			// 2007-10-30 by cmkwon, ����Ʈ ������ ���� ���� �α׿� �߰�
// 			CAtumLogSender::SendLogMessageITEMGetITEM(m_pFieldIOCPSocket, i_pItemGeneral, i_nChangeCount);
// 			break;
//		}

		// 2008-01-30 by cmkwon, ���ӷα� �ι� ����Ǵ� ���� ���� - CFieldItemManager::InsertItemGeneralByPointer() �Լ����� �����ϴ� ���� �ּ� ó���ϰ� ���⿡�� ó���Ѵ�.
		switch(i_nItemUpdateType)
		{
		case IUT_DROP_ITEM:
		case IUT_QUEST:			// 2007-10-30 by cmkwon, ����Ʈ ������ ���� ���� �α׿� �߰�
		case IUT_MIXING:		// 2008-09-26 by cmkwon, ���ս� GameLog ���� - 
		case IUT_DISSOLUTION_ITEM : // 2010-08-31 by shcho&jskim �����ۿ��� �ý��� -
		case IUT_RANDOMBOX :	// 2013-06-10 by jhseol, GameLog �߰� - ĸ���� ��� �� ȹ���ϴ� �����۵� ���ӷα׸� ���⵵�� �߰�
			// 2008-09-26 by cmkwon, ���ս� GameLog ���� - 
			// 2012-08-30 by jhseol, Ǯ�α� �����
#ifdef S_FULL_LOG_JHSEOL
			//CAtumLogSender::SendLogMessageITEMGetITEM(m_pFieldIOCPSocket, i_pItemGeneral, i_nChangeCount);	// 2012-08-30 by jhseol, Ǯ�α� ����� - ���� ���� �Լ� ������ �ּ� ���� ����.
#endif	//#ifdef S_FULL_LOG_JHSEOL
			// end 2012-08-30 by jhseol, Ǯ�α� �����
			CAtumLogSender::SendLogMessageITEMGetITEM(m_pFieldIOCPSocket, i_pItemGeneral, i_nChangeCount, i_nItemUpdateType);		// 2008-09-26 by cmkwon, ���ս� GameLog ���� - 
			break;
		case IUT_GIVEEVENTITEM:		// 2006-08-28 by dhjin
		case IUT_BONUS_ITEM:		// 2007-09-11 by cmkwon, ��ɾ� �̺�Ʈ ������ ���� ���ӷα׿� �߰��ϱ�
		case IUT_GIVEEVENTITEM_COUPONEVENT:		// 2008-01-10 by cmkwon, ������ �̺�Ʈ �ý��ۿ� �� ���� �ý��� �߰� - 
			CAtumLogSender::SendLogMessageITEMGetItemByItemEvent(m_pFieldIOCPSocket, i_pItemGeneral, i_nChangeCount);
			break;
		case IUT_STORE:		// 2008-02-15 by cmkwon, �κ�<->â�� �̵� �α� ���� - ���� ���� �α� �����ϱ� �߰�
			{
				// 2012-12-27 by bckim, ���ϰ���, ������ �ɸ��Ͱ��� ������ �̵��� �α��߰�
				CFieldIOCPSocket tempCharacterSocket;
				tempCharacterSocket.m_character.CharacterUniqueNumber = i_pItemGeneral->Possess;
				tempCharacterSocket.m_character.MapChannelIndex.MapIndex =this->m_pCharacter->MapChannelIndex.MapIndex;
				// 2012-12-27 by bckim, ���ϰ���, ������ �ɸ��Ͱ��� ������ �̵��� �α��߰�.End
				
				if(0 < i_nChangeCount)
				{// 2008-02-15 by cmkwon, i_nChangeCount �� 0���� �۴ٸ� â�� �α׷� �̹� ����ǰ� �ִ�.
					if(ITEM_IN_CHARACTER == i_enumStorage)
					{
						CAtumLogSender::SendLogMessageITEMCharacterITEM(m_pFieldIOCPSocket, i_pItemGeneral, i_nChangeCount);
						// 2012-12-27 by bckim, ���ϰ���, ������ �ɸ��Ͱ��� ������ �̵��� �α��߰�
						if( i_pItemGeneral->Possess != this->m_uid32StoreCharacterUID)
						{
							CAtumLogSender::SendLogMessageITEMGetWithSameAccount(&tempCharacterSocket, i_pItemGeneral, i_nChangeCount, this->m_uid32StoreCharacterUID);
						}
						// 2012-12-27 by bckim, ���ϰ���, ������ �ɸ��Ͱ��� ������ �̵��� �α��߰�.End
					}
					else
					{
						CAtumLogSender::SendLogMessageITEMStoreITEM(m_pFieldIOCPSocket, i_pItemGeneral, i_nChangeCount);
						// 2012-12-27 by bckim, ���ϰ���, ������ �ɸ��Ͱ��� ������ �̵��� �α��߰�
						if( i_pItemGeneral->Possess != m_pFieldIOCPSocket->m_character.CharacterUniqueNumber)
						{
							CAtumLogSender::SendLogMessageITEMGetWithSameAccount(&tempCharacterSocket, i_pItemGeneral, i_nChangeCount, m_pFieldIOCPSocket->m_character.CharacterUniqueNumber);											
						}
						// 2012-12-27 by bckim, ���ϰ���, ������ �ɸ��Ͱ��� ������ �̵��� �α��߰�.End
					}
				}
			}
			break;

			// 2012-12-17 by bckim, ���â�� ������ �̵� ���� �߰��α�
		case IUT_GUILD_STORE:
			{
				CAtumLogSender::SendLogMessageITEMCharacterToFromGuildstoreResult(m_pFieldIOCPSocket, i_pItemGeneral, i_nChangeCount);
			}
		break;
		// 2012-12-17 by bckim, ���â�� ������ �̵� ���� �߰��α�. End
		}

		// MSG ����
		if (i_bSendMSG)
		{
			INIT_MSG_WITH_BUFFER(MSG_FC_STORE_UPDATE_ITEM_COUNT, T_FC_STORE_UPDATE_ITEM_COUNT, msgUpdate, msgUpdateBuf);
			msgUpdate->ItemUniqueNumber	= i_pItemGeneral->UniqueNumber;
			msgUpdate->NewCount			= i_pItemGeneral->CurrentCount;
			msgUpdate->ItemUpdateType	= i_nItemUpdateType;
			m_pFieldIOCPSocket->SendAddData(msgUpdateBuf, MSG_SIZE(MSG_FC_STORE_UPDATE_ITEM_COUNT));
		}
	}

	return TRUE;
}




///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldItemManager::UpdateItemNumByPointer(ITEM_GENERAL *i_pItemGeneral, ITEM *i_pChangeITEM, BYTE i_nItemUpdateType, BOOL i_bSendMSG/*=TRUE*/)
/// \brief		
/// \author		cmkwon
/// \date		2006-06-14 ~ 2006-06-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::UpdateItemNumByPointer(ITEM_GENERAL *i_pItemGeneral, ITEM *i_pChangeITEM, BYTE i_nItemUpdateType, BOOL i_bSendMSG/*=TRUE*/)
{
	if(i_pItemGeneral->ItemInfo->Kind != i_pChangeITEM->Kind)
	{
		return FALSE;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2006-06-14 by cmkwon, ����
	i_pItemGeneral->ItemNum			= i_pChangeITEM->ItemNum;
	i_pItemGeneral->ItemInfo		= i_pChangeITEM;

	///////////////////////////////////////////////////////////////////////////////
	// 2006-06-14 by cmkwon
	QPARAM_UPDATEITEMNUM *pQParam	= new QPARAM_UPDATEITEMNUM;
	pQParam->ItemUniqueNumber		= i_pItemGeneral->UniqueNumber;
	pQParam->ItemNum				= i_pChangeITEM->ItemNum;
	ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemNum, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-06-14 by cmkwon
	if(i_bSendMSG)
	{
		INIT_MSG_WITH_BUFFER(MSG_FC_STORE_UPDATE_ITEMNUM, T_FC_STORE_UPDATE_ITEMNUM, pSUItemNum, SendBuf);
		pSUItemNum->ItemUniqueNumber	= i_pItemGeneral->UniqueNumber;
		pSUItemNum->ItemNum				= i_pItemGeneral->ItemNum;
		pSUItemNum->ItemUpdateType		= i_nItemUpdateType;
		m_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_STORE_UPDATE_ITEMNUM));
	}

	return TRUE;
}



///////////////////////////////////////////////////////////////////////////////
// ������ ��ü�� ����, Countable �������̴��� ������ �����ϰ� ����
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::DeleteItemFromCharacterByPointer(ITEM_GENERAL *i_pItemGeneral,
							BYTE i_nItemUpdateType, BYTE i_enumStorage,
							BOOL i_bUpdateDB, BOOL i_bSendMSG, BOOL i_bDeleteMemory,
							int	 i_nGLogItemtype/*=0*/)	// 2012-11-21 by khkim, GLog 2��

{
	if (i_pItemGeneral == NULL)
	{
		return FALSE;
	}

	mt_auto_lock mtAuto(&m_mapItemGeneral);
	BOOL bRet = FALSE;

	if (i_enumStorage == ITEM_IN_CHARACTER)
	{// �̺������� ����Ʈ���� ����
		bRet = m_mapItemGeneral.deleteNoLock(i_pItemGeneral->UniqueNumber);
	}
	else if (i_enumStorage == ITEM_IN_STORE)
	{// â�� ������ ����Ʈ���� ����
		bRet = m_mapItemInStore.deleteNoLock(i_pItemGeneral->UniqueNumber);
	}
	else
	{
		return FALSE;
	}

	if(FALSE == bRet)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldItemManager::DeleteItemFromCharacterByPointer() Failed: Type(%d) ItemInfo(%s) CharInfo(%s)\r\n"
			, i_enumStorage, GetItemGeneralString(i_pItemGeneral, string()), GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()));
		return FALSE;
	}
	mtAuto.auto_unlock_cancel();

	this->ItemDeleteByInfinityPlaying(i_pItemGeneral);		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� �� ������ ����

	// start 2012-11-21 by khkim, GLog 2�� 
	if(!COMPARE_RACE(this->m_pCharacter->Race, RACE_OPERATION|RACE_GAMEMASTER|RACE_MONITOR|RACE_GUEST|RACE_DEMO))		// 2013-01-18 by khkim, GLog (�Ϲ������� ���)	
	{
		if( NULL != m_pFieldIOCPSocket )
		{
			switch((FALSE == i_nGLogItemtype)? i_nItemUpdateType : i_nGLogItemtype)
			{
			case IUT_SHOP:
				{
					SetGLogData(GLOG_IL_LT_SHOP_SELL,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case IUT_BAZAAR_SELL:
				{
					SetGLogData(GLOG_IL_LT_BAZAARSELL_GIVE,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case IUT_BAZAAR_BUY:
				{
					SetGLogData(GLOG_IL_LT_BAZAARBUY_GIVE,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case IUT_TRADE:
				{
					SetGLogData(GLOG_IL_LT_TREADGIVE,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case IUT_LUCKY_ITEM:
				{
					SetGLogData(GLOG_IL_LT_LUCKYMACHINE_GET,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}	
			case IUT_GENERAL:
				{
					SetGLogData(GLOG_IL_LT_THROWAWAY,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			default:
				{
					break;
				}
			}
		}
	}
	// end 2012-11-21 by khkim, GLog 2��


	// DB���� ����
	if (i_bUpdateDB)
	{
		QPARAM_DELETESTOREITEM *pQParam = new QPARAM_DELETESTOREITEM;
		pQParam->AccountUniqueNumber = m_pFieldIOCPSocket->m_character.AccountUniqueNumber;
		pQParam->ItemUniqueNumber = i_pItemGeneral->UniqueNumber;
		ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_DeleteStoreItem, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
// 2004-12-31 by cmkwon, �ʿ����
//#ifdef _DEBUG
//		// ������ ������� ���� �߰��ϱ� ���� �α� ����
//		m_pFieldIOCPSocket->DBGOUT2(DO2_SYSLOG, "DeleteStoreItem: %s %s PF(%d) SF(%d) IUT: %d\r\n",
//						GetCharacterString(m_pCharacter, string()),
//						GetItemGeneralString(i_pItemGeneral, string()),
//						i_pItemGeneral->PrefixCodeNum, i_pItemGeneral->SuffixCodeNum,
//						i_nItemUpdateType);
//		m_pFieldIOCPSocket->PrintEnchant(i_pItemGeneral->UniqueNumber, TRUE);
//#endif
	}

	// MSG ������
	if (i_bSendMSG)
	{
		INIT_MSG_WITH_BUFFER(MSG_FC_STORE_DELETE_ITEM, T_FC_STORE_DELETE_ITEM, msgDelete, msgDeleteBuf);
		msgDelete->ItemUniqueNumber = i_pItemGeneral->UniqueNumber;
		msgDelete->ItemDeletionType = i_nItemUpdateType;
		m_pFieldIOCPSocket->SendAddData(msgDeleteBuf, MSG_SIZE(MSG_FC_STORE_DELETE_ITEM));
		switch(i_nItemUpdateType)
		{
		case IUT_TRADE:
		case IUT_BAZAAR_SELL:
		case IUT_BAZAAR_BUY:
			break;
		default:
			{
				m_pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_S_F2NOTIFY_0137, i_pItemGeneral->ItemInfo->ItemName);
			}
		}
	}

	// 2010-06-15 by shcho&hslee ��ý��� - ���� �� ���� �޸� ����.
	if ( i_pItemGeneral && i_pItemGeneral->ItemInfo->Kind == ITEMKIND_PET_ITEM )
	{
		DeleteItemPetFromCharacterByUID ( i_pItemGeneral->UniqueNumber , i_bSendMSG , i_bDeleteMemory );
	}

	// free memory
	if (i_bDeleteMemory)
	{
		util::del(i_pItemGeneral);
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// ������ ��ü�� ����, Countable �������̴��� ������ �����ϰ� ����
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::DeleteItemFromCharacterByUID(UID64_t i_nItemUID,
							BYTE i_nItemUpdateType, BYTE i_enumStorage,
							BOOL i_bUpdateDB, BOOL i_bSendMSG, BOOL i_bDeleteMemory,
							int i_nGLogItemtype/*=0*/)			//  2012-11-21 by khkim, GLog 2��
{
	ITEM_GENERAL *i_pItemGeneral = GetItemGeneralByUID(i_nItemUID, i_enumStorage);
	if (i_pItemGeneral == NULL)
	{
		return FALSE;
	}

	return DeleteItemFromCharacterByPointer(i_pItemGeneral, i_nItemUpdateType, i_enumStorage, i_bUpdateDB, i_bSendMSG, i_bDeleteMemory, i_nGLogItemtype);	//  2012-11-21 by khkim, GLog 2��
}

BOOL CFieldItemManager::DeleteItemFromCharacterByItemNum(INT i_nItemNum,
							INT i_nDelCount,
							BYTE i_nItemUpdateType, BYTE i_enumStorage,
							BOOL i_bUpdateDB, BOOL i_bSendMSG, BOOL i_bDeleteMemory,
							int  i_nGLogItemtype/*=0*/ )			//  2012-11-21 by khkim, GLog 2��
{
	ITEM_GENERAL *pItemGeneral
		= GetFirstItemGeneralByItemNum(i_nItemNum, i_enumStorage);

	if (pItemGeneral == NULL)
	{
		return FALSE;
	}

	if (IS_COUNTABLE_ITEM(pItemGeneral->Kind))
	{
		// Countable Item�� ���
		UpdateItemCountByPointer(pItemGeneral, -i_nDelCount, i_nItemUpdateType, i_enumStorage, i_bUpdateDB, i_bSendMSG, i_bDeleteMemory, i_nGLogItemtype);		//  2012-11-21 by khkim, GLog 2��

		return TRUE;
	}

	// NON-Countable Item�� ���
	INT nRetDelCount = 0;
	DeleteItemFromCharacterByUID(pItemGeneral->UniqueNumber, i_nItemUpdateType, i_enumStorage, i_bUpdateDB, i_bSendMSG, i_bDeleteMemory, i_nGLogItemtype);		//  2012-11-21 by khkim, GLog 2��
	nRetDelCount++;

	while (i_nDelCount > nRetDelCount)
	{
		ITEM_GENERAL *pItemToDelete = GetFirstItemGeneralByItemNum(i_nItemNum, i_enumStorage);
		if (pItemToDelete == NULL)
		{
			return TRUE;
		}
		DeleteItemFromCharacterByUID(pItemToDelete->UniqueNumber, i_nItemUpdateType, i_enumStorage, i_bUpdateDB, i_bSendMSG, i_bDeleteMemory, i_nGLogItemtype);	//  2012-11-21 by khkim, GLog 2��
		nRetDelCount++;
	}

	return TRUE;
}


// 2010-06-15 by shcho&hslee ��ý��� - ���� �� ���� �޸� ����.
/**************************************************************************************
**
**	���� �� ���� �޸� ����. 
**
**	Create Info : 2010-06-15 by shcho&hslee.
**
***************************************************************************************/
BOOL CFieldItemManager :: DeleteItemPetFromCharacterByUID ( UID64_t i_ItemUID , BOOL i_bSendMSG /*= TRUE*/ , BOOL i_bDeleteMemory /*= TRUE*/ )
{

	tPET_CURRENTINFO *pPetCurInfo = GetItemPetByUID ( i_ItemUID );

	if ( NULL == pPetCurInfo )
		return FALSE;

	if ( i_bSendMSG )
	{	// Ŭ���̾�Ʈ�� ���� ���� ����.
		INIT_MSG_WITH_BUFFER(MSG_TRADE_PET_DATA, T_FC_TRADE_DELETE_CURRENT_PET_DATA, Petcurrentmsg, sendPetbuff);

		///////////////////////////////////////////////////////////////////////////////////////
		// start 2011-08-17 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ� ���� ����
		//
		// ���� 
		//

// 		Petcurrentmsg->CreatedPetUID		= pPetCurInfo->CreatedPetUID;
// 		Petcurrentmsg->EnableEditPetName	= pPetCurInfo->EnableEditPetName;
// 		Petcurrentmsg->EnableLevelUp		= pPetCurInfo->EnableLevelUp;
// 		Petcurrentmsg->PetExp				= pPetCurInfo->PetExp;
// 		Petcurrentmsg->PetIndex				= pPetCurInfo->PetIndex;
// 		Petcurrentmsg->PetLevel				= pPetCurInfo->PetLevel;
// 
// 		memcpy( Petcurrentmsg->szPetName, pPetCurInfo->szPetName, SIZE_MAX_PET_NAME );

		/////////
		// ����

		Petcurrentmsg->CreatedPetUID		= pPetCurInfo->CreatedPetUID;
		Petcurrentmsg->PetExp				= pPetCurInfo->PetExp;
		Petcurrentmsg->PetIndex				= pPetCurInfo->PetIndex;
		Petcurrentmsg->PetLevel				= pPetCurInfo->PetLevel;

		memcpy(Petcurrentmsg->PetName, pPetCurInfo->PetName, SIZE_MAX_PET_NAME);

		// end 2011-08-17 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ� ���� ����
		///////////////////////////////////////////////////////////////////////////////////////

		m_pFieldIOCPSocket->SendAddData( sendPetbuff, MSG_SIZE(MSG_TRADE_PET_DATA) ); // �� ���� ����
	}

	if ( i_bDeleteMemory )
	{
		DeleteItemPetByUID ( i_ItemUID );
	}

	return TRUE;

}
// End 2010-06-15 by shcho&hslee ��ý��� - ���� �� ���� �޸� ����.

BOOL CFieldItemManager::DeleteAllItemGeneralFromCharacter(BOOL i_bOnlyNotAttachedItems /* = TRUE */)
{
	BOOL bDeleted = FALSE;

	m_mapItemGeneral.lock();
	while (TRUE)
	{
		bDeleted = FALSE;
		mtmapUID2ItemGeneral::iterator itrItemGeneral = m_mapItemGeneral.begin();
		while (itrItemGeneral != m_mapItemGeneral.end())
		{
			ITEM_GENERAL *pItem = itrItemGeneral->second;
			if (pItem->ItemNum != MONEY_ITEM_NUMBER
				&& IS_GENERAL_ITEM(pItem->Kind)
				&& POS_HIDDEN_ITEM != pItem->ItemWindowIndex	// // 2011-09-30 by hskim, ��Ʈ�� �ý��� 2�� - ������ �������� ���� ���� (����� �������� �����ɶ� ���� ������)
				&& (i_bOnlyNotAttachedItems?(pItem->Wear == WEAR_NOT_ATTACHED):TRUE)
			)
			{
				bDeleted = DeleteItemFromCharacterByUID(pItem->UniqueNumber, IUT_ADMIN);
				break;
			}
			
			itrItemGeneral++;
		}
		if (!bDeleted) break;
	}
	m_mapItemGeneral.unlock();

	return bDeleted;
}

BOOL CFieldItemManager::DeleteAllItemSkillFromCharacter()
{
	ASSERT_NOT_IMPLEMENTED_YET();
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldItemManager::DeleteAllArenaItem()
/// \brief		�Ʒ��� ���� �������� �����Ѵ�.
/// \author		dhjin
/// \date		2007-06-04 ~ 2007-06-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::DeleteAllArenaItem()
{
	m_mapItemGeneral.lock();

	BOOL bDeleted = FALSE;
	while (TRUE)
	{
		bDeleted = FALSE;
		mtmapUID2ItemGeneral::iterator itrItemGeneral = m_mapItemGeneral.begin();
		while (itrItemGeneral != m_mapItemGeneral.end())
		{
			ITEM_GENERAL *pItem = itrItemGeneral->second;
			if (COMPARE_BIT_FLAG(pItem->ItemInfo->ItemAttribute, ITEM_ATTR_ARENA_ITEM))
			{
				bDeleted = DeleteItemFromCharacterByUID(pItem->UniqueNumber, IUT_ARENA_ITEM);
				break;
			}
			
			itrItemGeneral++;
		}
		if (!bDeleted) break;
	}

	m_mapItemGeneral.unlock();
}

BOOL CFieldItemManager::LoadEnchantItem(UID64_t i_nItemUID, BOOL i_bLoadServer, BOOL i_bSendClient)
{
	ASSERT_NOT_IMPLEMENTED_YET();

	return FALSE;
}


 
/**********************************************************************************************************************************************************
**
**	������ �� �ִ� ī���ͺ� �����۵� ���� ó��.
**	
**	Create Info : ??.??.??.
**
**	Update Info :	// 2008-02-18 by cmkwon, ī���ͺ� �������� ������ ����� �ν��Ͻ� �������Ǵ� ���� ���� - CFieldItemManager::RearrangeCountableItems() �Լ� ����
**
**					// Ŭ���̾�Ʈ���� ��Ű �ӽ� ������ ���� ������Ʈ ��Ŷ�� �����ϴ� ��� Ȯ�εǾ� i_byItemUpdateType���� ���ڷ� �Ѱܹ޾� ó���ϵ��� ����. 2010. 08. 02 by hsLee.
**
***********************************************************************************************************************************************************/
void CFieldItemManager::RearrangeCountableItems(BYTE i_enumStorage, int i_nItemNum/*=0*/ , BYTE i_byItemUpdateType /*= IUT_GENERAL*/ )
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);

	if (i_enumStorage == ITEM_IN_CHARACTER)
	{
		mtmapUID2ItemGeneral::iterator itrItemGeneral = m_mapItemGeneral.begin();
		while (itrItemGeneral != m_mapItemGeneral.end())
		{
			ITEM_GENERAL *pItemGeneral = itrItemGeneral->second;

			// 2008-02-18 by cmkwon, ī���ͺ� �������� ������ ����� �ν��Ͻ� �������Ǵ� ���� ���� - i_nItemNum �� üũ�Ѵ�.			
			if(0 != i_nItemNum && pItemGeneral->ItemNum != i_nItemNum)
			{
				itrItemGeneral++;
				continue;
			}

			if (IS_COUNTABLE_ITEM(pItemGeneral->Kind))
			{
				ITEM_GENERAL *pItemGeneralFound = GetFirstItemGeneralByItemNum(pItemGeneral->ItemNum, ITEM_IN_CHARACTER);
				if (pItemGeneralFound != NULL && pItemGeneralFound->UniqueNumber != pItemGeneral->UniqueNumber)
				{
					// 2007-11-29 by cmkwon, �ý��� �α� ����
					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] RearrangeItems %s: %s <- %s\r\n",
														GetCharacterString(&(m_pFieldIOCPSocket->m_character), string()),
														GetItemGeneralString(pItemGeneralFound, string()),
														GetItemGeneralString(pItemGeneral, string()));

					// �ߺ��� �����
					//UpdateItemCountByPointer(pItemGeneralFound, pItemGeneral->CurrentCount, IUT_GENERAL, ITEM_IN_CHARACTER);
					UpdateItemCountByPointer(pItemGeneralFound , pItemGeneral->CurrentCount , i_byItemUpdateType , ITEM_IN_CHARACTER );

					if(g_pFieldGlobal->IsArenaServer())
					{
						// 2010-04-02 by cmkwon, ����2�� �߰� ���� - 
						this->ItemUpdateByInfinityPlaying(pItemGeneralFound);
						this->ItemDeleteByInfinityPlaying(pItemGeneral);
					}

					///////////////////////////////////////////////////////////////////////////////
					// 2007-11-29 by cmkwon, ī���ͺ������ �������� ���� �α� ����� - �κ�
					int nChangeCnts				= pItemGeneral->CurrentCount;
					pItemGeneral->CurrentCount	= 0;
					CAtumLogSender::SendLogMessageRearrangeDeleteItem(m_pFieldIOCPSocket, pItemGeneral, nChangeCnts);
					CAtumLogSender::SendLogMessageRearrangeItem(m_pFieldIOCPSocket, pItemGeneralFound, nChangeCnts);

					// DB���� ����
					QPARAM_DELETESTOREITEM *pQParam = new QPARAM_DELETESTOREITEM;
					pQParam->AccountUniqueNumber = m_pFieldIOCPSocket->m_character.AccountUniqueNumber;
					pQParam->ItemUniqueNumber = pItemGeneral->UniqueNumber;
					ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_DeleteStoreItem, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);

					// MSG ������
					INIT_MSG_WITH_BUFFER(MSG_FC_STORE_DELETE_ITEM, T_FC_STORE_DELETE_ITEM, msgDelete, msgDeleteBuf);
					msgDelete->ItemUniqueNumber = pItemGeneral->UniqueNumber;
					//msgDelete->ItemDeletionType = IUT_GENERAL;
					msgDelete->ItemDeletionType = i_byItemUpdateType;
					m_pFieldIOCPSocket->SendAddData(msgDeleteBuf, MSG_SIZE(MSG_FC_STORE_DELETE_ITEM));
					m_pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_S_F2NOTIFY_0137, pItemGeneral->ItemInfo->ItemName);

					// map���� ����
					m_mapItemGeneral.erase(itrItemGeneral++);

					// memory���� ����
					util::del(pItemGeneral);

					continue;
				}
			}

			itrItemGeneral++;
		}
	}
	else if (i_enumStorage == ITEM_IN_STORE)
	{
		mtmapUID2ItemGeneral::iterator itrItemInStore = m_mapItemInStore.begin();
		while (itrItemInStore != m_mapItemInStore.end())
		{
			ITEM_GENERAL *pItemInStore = itrItemInStore->second;

			// 2008-02-18 by cmkwon, ī���ͺ� �������� ������ ����� �ν��Ͻ� �������Ǵ� ���� ���� - i_nItemNum �� üũ�Ѵ�.			
			if(0 != i_nItemNum && pItemInStore->ItemNum != i_nItemNum)
			{
				itrItemInStore++;
				continue;
			}

			if (IS_COUNTABLE_ITEM(pItemInStore->Kind))
			{
				ITEM_GENERAL *pItemInStoreFound = GetFirstItemGeneralByItemNum(pItemInStore->ItemNum, ITEM_IN_STORE);
				if (pItemInStoreFound != NULL && pItemInStoreFound->UniqueNumber != pItemInStore->UniqueNumber)
				{
					// 2007-11-29 by cmkwon, �ý��� �α� ����
					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] RearrangeItems %s: %s <- %s\r\n",
														GetCharacterString(&(m_pFieldIOCPSocket->m_character), string()),
														GetItemGeneralString(pItemInStoreFound, string()),
														GetItemGeneralString(pItemInStore, string()));

					// �ߺ��� �����
					//UpdateItemCountByPointer(pItemInStoreFound, pItemInStore->CurrentCount, IUT_GENERAL, ITEM_IN_STORE);
					UpdateItemCountByPointer(pItemInStoreFound , pItemInStore->CurrentCount , i_byItemUpdateType , ITEM_IN_STORE );

					///////////////////////////////////////////////////////////////////////////////
					// 2007-11-29 by cmkwon, ī���ͺ������ �������� ���� �α� ����� - â��
					int nChangeCnts				= pItemInStore->CurrentCount;
					pItemInStore->CurrentCount	= 0;
					CAtumLogSender::SendLogMessageRearrangeDeleteItem(m_pFieldIOCPSocket, pItemInStore, nChangeCnts, TRUE);
					CAtumLogSender::SendLogMessageRearrangeItem(m_pFieldIOCPSocket, pItemInStoreFound, nChangeCnts, TRUE);

					// DB���� ����
					QPARAM_DELETESTOREITEM *pQParam = new QPARAM_DELETESTOREITEM;
					pQParam->AccountUniqueNumber = m_pFieldIOCPSocket->m_character.AccountUniqueNumber;
					pQParam->ItemUniqueNumber = pItemInStore->UniqueNumber;
					ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_DeleteStoreItem, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);

					// MSG ������
					INIT_MSG_WITH_BUFFER(MSG_FC_STORE_DELETE_ITEM, T_FC_STORE_DELETE_ITEM, msgDelete, msgDeleteBuf);
					msgDelete->ItemUniqueNumber = pItemInStore->UniqueNumber;
					//msgDelete->ItemDeletionType = IUT_GENERAL;
					msgDelete->ItemDeletionType = i_byItemUpdateType;
					m_pFieldIOCPSocket->SendAddData(msgDeleteBuf, MSG_SIZE(MSG_FC_STORE_DELETE_ITEM));
					m_pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_S_F2NOTIFY_0137, pItemInStore->ItemInfo->ItemName);

					// map���� ����
					m_mapItemInStore.erase(itrItemInStore++);

					// memory���� ����
					util::del(pItemInStore);
					continue;
				}
			}// end_if (IS_COUNTABLE_ITEM(pItemInStore->Kind))
			
			itrItemInStore++;
		}// end_while (itrItemInStore != m_mapItemInStore.end())
	}
	return;
}

// 2006-01-24 by cmkwon, �����ۿ� �߷� ��æƮ ����
// 2006-10-13 by cmkwon, ���ϰ� �ڷ��� ����(float-->int)
// 2007-11-02 by cmkwon, �߷� ���ϴ� �ý��� ���� - ���ϰ� �ڷ��� ����(<--int)
float CFieldItemManager::GetCurrentCarryingItemWeight()
{
	float retTotalWeight = 0.0f;

	m_mapItemGeneral.lock();

	map<UID64_t, ITEM_GENERAL*>::iterator itr = m_mapItemGeneral.begin();

	while (itr != m_mapItemGeneral.end())
	{
		ITEM_GENERAL *pItemGeneral = itr->second;

// 2005-09-27 by cmkwon, ���԰������ ������ �����۸� �����ϵ��� ������
//		retTotalWeight += CAtumSJ::GetItemWeight(pItemGeneral->ItemInfo, pItemGeneral->CurrentCount);
		if(WEAR_ATTACHED == pItemGeneral->Wear)
		{
			retTotalWeight += CAtumSJ::GetItemWeight(pItemGeneral->ItemInfo, pItemGeneral->CurrentCount, pItemGeneral->DesWeight);
		}
		itr++;
	}

	m_mapItemGeneral.unlock();

	return retTotalWeight;
}

void CFieldItemManager::DeleteItemSkillBySkillBaseNum(INT i_nSkillItemNum, BOOL i_bSendMsg)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	
	vectItemSkillPtr::iterator itr = m_vectItemSkillPtr.begin();
	while (itr != m_vectItemSkillPtr.end())
	{
		ITEM_SKILL *pItemSkill = *itr;
		// 2013-03-12 by jhseol, ��ų �ѹ��� �ý��� ����
#ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
		if ( pItemSkill->ItemInfo->NextSkill == i_nSkillItemNum )
#else	// #ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
		if (SKILL_BASE_NUM(pItemSkill->ItemNum) == SKILL_BASE_NUM(i_nSkillItemNum))
#endif	// #ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
		// end 2013-03-12 by jhseol, ��ų �ѹ��� �ý��� ����
		{			
			if (i_bSendMsg)
			{// send msg

				INIT_MSG_WITH_BUFFER(MSG_FC_STORE_DELETE_ITEM, T_FC_STORE_DELETE_ITEM, pDelSkill, pDelSkillBuf);
				pDelSkill->ItemUniqueNumber = pItemSkill->UniqueNumber;
				pDelSkill->ItemDeletionType = IUT_SKILL;
				m_pFieldIOCPSocket->SendAddData(pDelSkillBuf, MSG_SIZE(MSG_FC_STORE_DELETE_ITEM));
			}

			itr = m_vectItemSkillPtr.erase(itr);			
			util::del(pItemSkill);			// release memory

			continue;							// continue searching
		}
		itr++;
	}
}

ITEM_SKILL *CFieldItemManager::GetFirstSkillBySkillBaseNum(INT i_nSkillItemNum)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	vectItemSkillPtr::iterator itr = m_vectItemSkillPtr.begin();
	while (itr != m_vectItemSkillPtr.end())
	{
		ITEM_SKILL *pItemSkill = *itr;
		// 2013-03-12 by jhseol, ��ų �ѹ��� �ý��� ����
#ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
		if ( pItemSkill->ItemInfo->NextSkill == i_nSkillItemNum )
#else	// #ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
		if (SKILL_BASE_NUM(pItemSkill->ItemNum) == SKILL_BASE_NUM(i_nSkillItemNum))
#endif	// #ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
		// end 2013-03-12 by jhseol, ��ų �ѹ��� �ý��� ����
		{
			return pItemSkill;
		}

		itr++;
	}
	return NULL;
}

ITEM_SKILL *CFieldItemManager::GetItemSkillByUID(UID64_t i_nSkillUID)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	for(int i=0; i < m_vectItemSkillPtr.size(); i++)
	{
		if(m_vectItemSkillPtr[i]->UniqueNumber == i_nSkillUID)
		{
			return m_vectItemSkillPtr[i];
		}
	}

	return NULL;
}


// 2010-06-15 by shcho&hslee ��ý���

// ITEM_PET ����.
void CFieldItemManager :: DeleteItemPetByUID ( UID64_t i_nPetUID )
{
	mt_auto_lock mtauto(&m_vectPetCurrentInfo);

	mtvectPetCurrentInfo::iterator itr = m_vectPetCurrentInfo.begin();

	for ( ; itr != m_vectPetCurrentInfo.end(); ++itr )
	{
		if ( (*itr)->CreatedPetUID == i_nPetUID )
		{
			if((*itr) != NULL)
			{
				/////////////////////////////////////////////////
				// start 2011-09-20 by hskim, ��Ʈ�� �ý��� 2��
				// �������� ���� ������ �����
				for(int a=0; a<SIZE_MAX_PETSOCKET; a++)
				{
					if( 0 != (*itr)->PetSocketItemUID[a] )
					{
						DeleteItemFromCharacterByUID((*itr)->PetSocketItemUID[a], IUT_DEPENDENCY_ITEM);
					}
				}
				// end 2011-09-20 by hskim, ��Ʈ�� �ý��� 2��
				/////////////////////////////////////////////////
				
			util::del ( *itr );
			}
			m_vectPetCurrentInfo.erase( itr );
			break;
		}
	}

	mtauto.auto_unlock_cancel();
}

tPET_CURRENTINFO *CFieldItemManager :: GetItemPetByUID ( UID64_t i_nPetUID )
{
	mt_auto_lock mtauto ( &m_vectPetCurrentInfo );

	mtvectPetCurrentInfo::iterator itr = m_vectPetCurrentInfo.begin();

	//for ( ; itr != m_vectPetCurrentInfo.end(); ++itr )
	while ( itr != m_vectPetCurrentInfo.end() )
	{
		if ( (*itr)->CreatedPetUID == i_nPetUID )
			return (*itr);

		++itr;
	}

	mtauto.auto_unlock_cancel();

	return NULL;
}


///////////////////////////////////////////////////////////////////////////////////////
// start 2011-09-20 by hskim, ��Ʈ�� �ý��� 2��

BOOL CFieldItemManager::IsBelongPetSocketItemByUID(UID64_t i_nPetID, UID64_t i_nSocketID)
{
	mt_auto_lock mtauto(&m_vectPetCurrentInfo);

	tPET_CURRENTINFO *pCurrentPet = GetItemPetByUID(i_nPetID);
	
	if( NULL != pCurrentPet )
	{
		for(int a=0; a<pCurrentPet->PetEnableSocketCount; a++)
		{
			if( i_nSocketID == pCurrentPet->PetSocketItemUID[a] )
			{
				return TRUE;
			}
		}
	}

	mtauto.auto_unlock_cancel();

	return FALSE;
}

// end 2011-09-20 by hskim, ��Ʈ�� �ý��� 2��
///////////////////////////////////////////////////////////////////////////////////////

// End 2010-06-15 by shcho&hslee ��ý���

void CFieldItemManager::CheckRequirementAllGeneralItems(void)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);

	mtmapUID2ItemGeneral::iterator itr = m_mapItemGeneral.begin();
	for(; m_mapItemGeneral.end() != itr; itr++)
	{
		ITEM_GENERAL *pItemG = itr->second;
		if(pItemG->Wear == WEAR_ATTACHED)
		{			
			Err_t nErr = m_pFieldIOCPSocket->CheckGeneralItemRequirements(pItemG);
			if ( nErr != ERR_NO_ERROR
				|| (ITEMKIND_SUPPORT != pItemG->Kind && m_pFieldIOCPSocket->GetCurrentLoadagePercentage() > 1.0f) )
			{
				int nTmpWindowIndex		= pItemG->ItemWindowIndex;	// ���� ��ġ

				///////////////////////////////////////////////////////////////////////////////
				// 2006-10-23 by cmkwon, ���� ���� ó����
				m_pFieldIOCPSocket->UpdateWearItemPointer(pItemG->UniqueNumber, pItemG->UniqueNumber, pItemG->ItemWindowIndex, POS_ITEMWINDOW_OFFSET + nTmpWindowIndex);
				m_pFieldIOCPSocket->OnDetachItem(pItemG, pItemG->ItemWindowIndex, FALSE);

				// 2010-03-17 by cmkwon, ����1�� �ߵ��� ������ ���� ���� ���� - ������ ���� ���������� ó��
				m_pFieldIOCPSocket->OnUpdateWearItem(pItemG->UniqueNumber, NULL, pItemG->ItemWindowIndex, POS_ITEMWINDOW_OFFSET + nTmpWindowIndex);

				pItemG->Wear			= WEAR_NOT_ATTACHED;
				pItemG->ItemWindowIndex	= POS_ITEMWINDOW_OFFSET + nTmpWindowIndex;

				///////////////////////////////////////////////////////////////////////////////
				// Ŭ���̾�Ʈ�� â�� ���� ����
				INIT_MSG_OF_SIZE(MSG_FC_ITEM_CHANGE_WINDOW_POSITION_OK, T_FC_ITEM_CHANGE_WINDOW_POSITION_OK, pMsgChangeOK, pMsgChangeOKBuf);
				pMsgChangeOK->UniqueNumber			= pItemG->UniqueNumber;
				pMsgChangeOK->ItemWindowIndex		= pItemG->ItemWindowIndex;
				pMsgChangeOK->Wear					= pItemG->Wear;
				pMsgChangeOK->UniqueNumberDest		= 0;
				pMsgChangeOK->ItemWindowIndexDest	= nTmpWindowIndex;
				pMsgChangeOK->WearDest				= 0;
				m_pFieldIOCPSocket->SendAddData(pMsgChangeOKBuf, MSG_SIZE(MSG_FC_ITEM_CHANGE_WINDOW_POSITION_OK));

				///////////////////////////////////////////////////////////////////////////////
				// DB�� ���� ���� ����
				QPARAM_CHANGE_ITEM_WINDOW_POSITION *pQParamChangeWindowPosition	= new QPARAM_CHANGE_ITEM_WINDOW_POSITION;
				pQParamChangeWindowPosition->AccountUID			= m_pFieldIOCPSocket->m_character.AccountUniqueNumber;
				pQParamChangeWindowPosition->CharacterUID		= m_pFieldIOCPSocket->m_character.CharacterUniqueNumber;
				pQParamChangeWindowPosition->ItemUID1			= pItemG->UniqueNumber;
				pQParamChangeWindowPosition->ItemWear1			= pItemG->Wear;
				pQParamChangeWindowPosition->ItemWindowIndex1	= pItemG->ItemWindowIndex;
				pQParamChangeWindowPosition->ItemUID2			= 0;
				pQParamChangeWindowPosition->ItemWindowIndex2	= POS_INVALID_POSITION;
				pQParamChangeWindowPosition->ItemWear2			= WEAR_NOT_ATTACHED;
				ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_ChangeItemPosition, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParamChangeWindowPosition);
			}
		}
	}
}

INT CFieldItemManager::GetInventoryItemCounts(void)		// 2011-10-21 by hskim, ��Ʈ�� �ý��� 2��
{
	///////////////////////////////////////////////////////////////////////////////////////
	// start 2011-09-20 by hskim, ��Ʈ�� �ý��� 2��
	//
	// ���� 

	//return m_mapItemGeneral.size();

	/////////
	// ����

	return m_mapItemGeneral.size() - m_nInventoryHiddenItemCount;

	// end 2011-09-20 by hskim, ��Ʈ�� �ý��� 2��
	///////////////////////////////////////////////////////////////////////////////////////
}

// start 2011-10-21 by hskim, ��Ʈ�� �ý��� 2�� - ���� ������ �ִ� ������ ������ (POS_HIDDEN_ITEM) ����
INT CFieldItemManager::GetInventoryHiddenItemCounts(void)
{
	return m_nInventoryHiddenItemCount;
}

INT CFieldItemManager::CalcInventoryHiddenItemCounts(void)
{
	INT nItemCount = 0;
	mt_auto_lock igLock(&m_mapItemGeneral);

	mtmapUID2ItemGeneral::iterator itr = m_mapItemGeneral.begin();
	while(itr != m_mapItemGeneral.end())
	{
		ITEM_GENERAL *pItemG = itr->second;

		if( NULL != pItemG->ItemInfo && pItemG->ItemWindowIndex == POS_HIDDEN_ITEM )
		{
			nItemCount++;
		}

		itr++;
	}

	m_nInventoryHiddenItemCount = nItemCount;

	igLock.auto_unlock_cancel();

	return nItemCount;
}
// end 2011-10-21 by hskim, ��Ʈ�� �ý��� 2�� - ���� ������ �ִ� ������ ������ (POS_HIDDEN_ITEM) ����

// 2013-05-22 by jhseol, �ҽ����� - �ּ�����

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldItemManager::UpdateItemStorage(ITEM_GENERAL *i_pItemGeneral, BYTE i_enFromStorage, BYTE i_enToStorage)
/// \brief		
/// \author		cmkwon
/// \date		2005-12-07 ~ 2005-12-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::UpdateItemStorage(ITEM_GENERAL *i_pItemGeneral, BYTE i_enFromStorage, BYTE i_enToStorage)
{
	if (m_pFieldIOCPSocket->m_pCurrentBuildingNPC == NULL
		|| m_pFieldIOCPSocket->m_pCurrentBuildingNPC->BuildingKind != BUILDINGKIND_STORE)
	{
		return FALSE;
	}

	mt_auto_lock mtAuto(&m_mapItemGeneral);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-09-23 by dhjin, ���� â�� <--> ĳ�����κ� �̵� ó��
	// ĳ���� �κ�ó���� �ϰ� DB Update�� ���� ����
	if(ITEM_IN_GUILD_STORE == i_enFromStorage
		||ITEM_IN_GUILD_STORE == i_enToStorage)
	{
		if (ITEM_IN_CHARACTER == i_enFromStorage
			&& ITEM_IN_GUILD_STORE == i_enToStorage)
		{// ĳ�����κ�->����â��
			i_pItemGeneral->ItemStorage = i_enToStorage;
			m_mapItemGeneral.deleteNoLock(i_pItemGeneral->UniqueNumber);

			RemoveTimerFixedTermShape(i_pItemGeneral);			// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)

			return TRUE;
		}
		else if (ITEM_IN_GUILD_STORE == i_enFromStorage
				&& ITEM_IN_CHARACTER == i_enToStorage)
		{// ����â��->�ɸ����κ�
			i_pItemGeneral->ItemStorage = i_enToStorage;
			m_mapItemGeneral.insertNoLock(i_pItemGeneral->UniqueNumber, i_pItemGeneral);
			// 2013-06-25 by jhseol, ������ UID '0' ������ ����� �α�
#ifdef S_ITEMUID_ZERO_DEBUG_LOG_JHSEOL
			if (IS_COUNTABLE_ITEM(i_pItemGeneral->Kind))
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DEBUG] SPI ItemUID Zero : CFieldItemManager::UpdateItemStorage_1 - CUID(%d), ItemNum(%d), Count(%d), ItemUID(%d)\r\n"
					, m_pFieldIOCPSocket->m_character.CharacterUniqueNumber, i_pItemGeneral->ItemNum, i_pItemGeneral->CurrentCount, i_pItemGeneral->UniqueNumber);
			}
#endif
			// end 2013-06-25 by jhseol, ������ UID '0' ������ ����� �α�

			RegisterTimerFixedTermShape(i_pItemGeneral);		// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)

			return TRUE;
		}
	}

	if (m_pFieldIOCPSocket->m_character.CharacterUniqueNumber == m_uid32StoreCharacterUID)
	{// 2006-09-18 by dhjin, �ڱ� �ڽ� �ɸ��� �϶�
		if (ITEM_IN_CHARACTER == i_enToStorage)
		{// â��->ĳ�����κ�
			i_pItemGeneral->ItemStorage = i_enToStorage;
			m_mapItemInStore.deleteNoLock(i_pItemGeneral->UniqueNumber);
			m_mapItemGeneral.insertNoLock(i_pItemGeneral->UniqueNumber, i_pItemGeneral);
			// 2013-06-25 by jhseol, ������ UID '0' ������ ����� �α�
#ifdef S_ITEMUID_ZERO_DEBUG_LOG_JHSEOL
			if (IS_COUNTABLE_ITEM(i_pItemGeneral->Kind))
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DEBUG] SPI ItemUID Zero : CFieldItemManager::UpdateItemStorage_2 - CUID(%d), ItemNum(%d), Count(%d), ItemUID(%d)\r\n"
					, m_pFieldIOCPSocket->m_character.CharacterUniqueNumber, i_pItemGeneral->ItemNum, i_pItemGeneral->CurrentCount, i_pItemGeneral->UniqueNumber);
			}
#endif
			// end 2013-06-25 by jhseol, ������ UID '0' ������ ����� �α�

			RegisterTimerFixedTermShape(i_pItemGeneral);		// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)
		}
		else if (ITEM_IN_STORE == i_enToStorage)
		{// ĳ�����κ�->â��
			i_pItemGeneral->ItemStorage = i_enToStorage;
			m_mapItemInStore.insertNoLock(i_pItemGeneral->UniqueNumber, i_pItemGeneral);
			m_mapItemGeneral.deleteNoLock(i_pItemGeneral->UniqueNumber);

			RemoveTimerFixedTermShape(i_pItemGeneral);			// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)
		}
		else
		{
			return FALSE;
		}
	}
	else
	{// 2006-09-18 by dhjin, �ٸ� �ɸ��� �϶�

		// start 2012-01-30 by hskim, ��Ʈ�� �ý��� 2�� - ���� â�� �̵�	
		if( i_pItemGeneral && i_pItemGeneral->ItemInfo->Kind == ITEMKIND_PET_ITEM )
		{
			UpdateItemStorageSubItem(i_pItemGeneral, i_enFromStorage, i_enToStorage);
		}
		// end 2012-01-30 by hskim, ��Ʈ�� �ý��� 2�� - ���� â�� �̵�

		if (ITEM_IN_CHARACTER == i_enToStorage)
		{// â��->ĳ�����κ�
			i_pItemGeneral->ItemStorage = i_enToStorage;
			m_mapItemInStore.deleteNoLock(i_pItemGeneral->UniqueNumber);
			i_pItemGeneral->Possess		= m_pCharacter->CharacterUniqueNumber;
			m_mapItemGeneral.insertNoLock(i_pItemGeneral->UniqueNumber, i_pItemGeneral);
			// 2013-06-25 by jhseol, ������ UID '0' ������ ����� �α�
#ifdef S_ITEMUID_ZERO_DEBUG_LOG_JHSEOL
			if (IS_COUNTABLE_ITEM(i_pItemGeneral->Kind))
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DEBUG] SPI ItemUID Zero : CFieldItemManager::UpdateItemStorage_3 - CUID(%d), ItemNum(%d), Count(%d), ItemUID(%d)\r\n"
					, m_pFieldIOCPSocket->m_character.CharacterUniqueNumber, i_pItemGeneral->ItemNum, i_pItemGeneral->CurrentCount, i_pItemGeneral->UniqueNumber);
			}
#endif
			// end 2013-06-25 by jhseol, ������ UID '0' ������ ����� �α�

			RegisterTimerFixedTermShape(i_pItemGeneral);		// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)
		}
		else if (ITEM_IN_STORE == i_enToStorage)
		{// ĳ�����κ�->â��
			i_pItemGeneral->ItemStorage = i_enToStorage;
			i_pItemGeneral->Possess		= m_uid32StoreCharacterUID;
			m_mapItemInStore.insertNoLock(i_pItemGeneral->UniqueNumber, i_pItemGeneral);
			m_mapItemGeneral.deleteNoLock(i_pItemGeneral->UniqueNumber);

			RemoveTimerFixedTermShape(i_pItemGeneral);			// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)
		}
		else
		{
			return FALSE;
		}	
	}
	
	QPARAM_STORE_UPDATE *qQParamStoreUpdate = new QPARAM_STORE_UPDATE;
	qQParamStoreUpdate->ItemUniqueNumber	= i_pItemGeneral->UniqueNumber;
	qQParamStoreUpdate->CharacterUID		= i_pItemGeneral->Possess;
	qQParamStoreUpdate->ItemStorage			= i_enToStorage;
	ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemStorage, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, qQParamStoreUpdate);
	
	INIT_MSG_WITH_BUFFER(MSG_FC_STORE_UPDATE_ITEMSTORAGE, T_FC_STORE_UPDATE_ITEMSTORAGE, pMsgUpdate, SendBuf);
	pMsgUpdate->ItemUniqueNumber	= i_pItemGeneral->UniqueNumber;
	pMsgUpdate->FromItemStorage		= i_enFromStorage;
	pMsgUpdate->ToItemStorage		= i_enToStorage;
	m_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_STORE_UPDATE_ITEMSTORAGE));
	
	// 2011-02-07 by shcho, �� ������ ���� ������ �߰�
	// �� �� �������� ������ �ִ� �� ������ ������ ã�Ƽ� ������ �Ѵ�.

	tPET_CURRENTINFO* pSendPetCurrentData;
	pSendPetCurrentData = m_pFieldIOCPSocket->m_ItemManager.GetItemPetByUID(i_pItemGeneral->UniqueNumber);
	// ���� ��쿡 �������� ������.
	if(NULL!=pSendPetCurrentData)
	{
		m_pFieldIOCPSocket->SendInsertPetItemInfo(ITEM_IN_CHARACTER,pSendPetCurrentData);
	}
	// end 2011-02-07 by shcho, �� ������ ���� ������ �߰�

	return TRUE;
}

// start 2012-01-30 by hskim, ��Ʈ�� �ý��� 2�� - ���� â�� �̵�
BOOL CFieldItemManager::UpdateItemStorageSubItem(ITEM_GENERAL *i_pItemGeneral, BYTE i_enFromStorage, BYTE i_enToStorage)
{
	// ��Ʈ�� �������� â�� ������ �̵�(�����)�� ��� ����
	// ��Ʈ�ʿ� ������ ���Ͼ����۵� ���� ���� �ϱ� ���� �۾�

	tPET_CURRENTINFO *pPetCurrentData = NULL;
	pPetCurrentData = m_pFieldIOCPSocket->m_ItemManager.GetItemPetByUID(i_pItemGeneral->UniqueNumber);

	if( NULL != pPetCurrentData )
	{
		mt_auto_lock mtAuto(&m_mapItemGeneral);

		for(int i=0; i<SIZE_MAX_PETSOCKET; i++)
		{
			UID32_t Possess = 0;

			if( 0 != pPetCurrentData->PetSocketItemUID[i] )
			{
				// DB�� ��Ʈ�� ���� ��ų ���� �ʱ�ȭ

				QPARAM_PET_SET_AUTOSKILL_SLOT *pQParamClearAutoSkill = new QPARAM_PET_SET_AUTOSKILL_SLOT;
				pQParamClearAutoSkill->ItemUniqueNumber = i_pItemGeneral->UniqueNumber;
				memset(&pQParamClearAutoSkill->PetAutoSkill, 0, sizeof(pQParamClearAutoSkill->PetAutoSkill));
				ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_PetSetAutoSkillSlot, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParamClearAutoSkill);

				// �޸𸮻��� ��Ʈ�� ���� ��ų ���� �ʱ�ȭ
				
				memset(&pPetCurrentData->PetAutoSkill, 0, sizeof(pPetCurrentData->PetAutoSkill));

				if( ITEM_IN_CHARACTER == i_enToStorage )
				{
					// â��->ĳ�����κ�

					Possess = m_pCharacter->CharacterUniqueNumber;

					QPARAM_STORE_GET_ITEM_ONE *qQParamStoreUpdate = new QPARAM_STORE_GET_ITEM_ONE;
					qQParamStoreUpdate->ItemUniqueNumber	= pPetCurrentData->PetSocketItemUID[i];
					qQParamStoreUpdate->bSendToClient	= TRUE;
					ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_StoreGetItemOne, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, qQParamStoreUpdate);
				}
				else if( ITEM_IN_STORE == i_enToStorage )
				{
					// ĳ�����κ�->â��

					Possess = m_uid32StoreCharacterUID;

					ITEM_GENERAL *pItemGDel = GetItemGeneralByUID(pPetCurrentData->PetSocketItemUID[i]);
					
					if( NULL != pItemGDel )
					{
						m_mapItemGeneral.deleteNoLock(pPetCurrentData->PetSocketItemUID[i]);
						util::del(pItemGDel);
					}

					INIT_MSG_WITH_BUFFER(MSG_FC_STORE_DELETE_ITEM, T_FC_STORE_DELETE_ITEM, msgDelete, msgDeleteBuf);
					msgDelete->ItemUniqueNumber = pPetCurrentData->PetSocketItemUID[i];
					msgDelete->ItemDeletionType = IUT_DEPENDENCY_ITEM;
					m_pFieldIOCPSocket->SendAddData(msgDeleteBuf, MSG_SIZE(MSG_FC_STORE_DELETE_ITEM));
				}

				// ��Ʈ�� ������ ���� �Ҽ� ���� ����

				QPARAM_PET_CHANGE_SOCKET_OWNER *qQParamStoreUpdate = new QPARAM_PET_CHANGE_SOCKET_OWNER;
				qQParamStoreUpdate->ItemUniqueNumber	= pPetCurrentData->PetSocketItemUID[i];
				qQParamStoreUpdate->CharacterUID		= Possess;
				ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_PetChangeSocketOwner, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, qQParamStoreUpdate);
			}
		}
	}

	return TRUE;
}
// end 2012-01-30 by hskim, ��Ʈ�� �ý��� 2�� - ���� â�� �̵�

void CFieldItemManager::DBG_PRINT_ITEM()
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	int nCnts = 1;
	mtmapUID2ItemGeneral::iterator tmpItr = m_mapItemGeneral.begin();
	while (m_mapItemGeneral.end() != tmpItr)
	{
		ITEM_GENERAL *pItemG = tmpItr->second;

		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] %30s Inventory Item: %s ItemCount(%d)\r\n",
				GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()),
				GetItemGeneralString(pItemG, string()), nCnts);

		nCnts++;
		tmpItr++;
	}

	for(int i=0; i < m_vectItemSkillPtr.size(); i++)	
	{
		ITEM_SKILL *pSkill = m_vectItemSkillPtr[i];

		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] %30s Skill: %s SkillCount(%d)\r\n",
				GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()),
				GetItemSkillString(pSkill, string()), i+1);
	}


	m_pFieldIOCPSocket->m_mapTradeItem.lock();
	nCnts = 1;
	mtmapUID2TradeItem::iterator tmpItrTrade = m_pFieldIOCPSocket->m_mapTradeItem.begin();
	while (m_pFieldIOCPSocket->m_mapTradeItem.end() != tmpItrTrade)
	{
		ITEM_GENERAL *pTrdItemG = tmpItrTrade->second.pStoreItem;

		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] %30s Trade Item: %s ItemCount(%d)\r\n",
				GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()),
				GetItemGeneralString(pTrdItemG, string()), nCnts);

		nCnts++;
		tmpItrTrade++;
	}
	m_pFieldIOCPSocket->m_mapTradeItem.unlock();

	mtAuto.auto_unlock_cancel();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-03-31 by cmkwon, �����ʱ�ȭ �ý��� ���� - 
/// \author		cmkwon
/// \date		2009-04-01 ~ 2009-04-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CFieldItemManager::ChangeItemNum(INT i_nFromItemNum, INT i_nToItemNum)
{
	int nCnt = 0;
	mt_auto_lock igLock(&m_mapItemGeneral);
	mtmapUID2ItemGeneral::iterator itr = m_mapItemGeneral.begin();
	for(; itr != m_mapItemGeneral.end(); itr++)
	{
		ITEM_GENERAL *pItemG = itr->second;
		if (pItemG->ItemNum == i_nFromItemNum)
		{
			nCnt++;

			///////////////////////////////////////////////////////////////////////////////
			// ����� ������ �����Ѵ�.
			pItemG->ItemNum					= i_nToItemNum;
			pItemG->ItemInfo				= ms_pFieldIOCP2->GetItemInfo(i_nToItemNum);

			///////////////////////////////////////////////////////////////////////////////
			// ����� ItemNum�� DB���� �����Ѵ�.
			QPARAM_UPDATEITEMNUM *pQParam	= new QPARAM_UPDATEITEMNUM;
			pQParam->ItemUniqueNumber		= pItemG->UniqueNumber;
			pQParam->ItemNum				= i_nToItemNum;
			ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemNum, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);			

			///////////////////////////////////////////////////////////////////////////////
			// 2009-04-06 by cmkwon, �����ʱ�ȭ �ý��� �߰� - ����� ItemNum ������ ���� �����Ѵ�.
			INIT_MSG_WITH_BUFFER(MSG_FC_STORE_UPDATE_ITEMNUM, T_FC_STORE_UPDATE_ITEMNUM, pSUItemNum, SendBuf);
			pSUItemNum->ItemUniqueNumber	= pItemG->UniqueNumber;
			pSUItemNum->ItemNum				= pItemG->ItemNum;
			pSUItemNum->ItemUpdateType		= IUT_GENERAL;
			m_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_STORE_UPDATE_ITEMNUM));
		}
	}
	
	return nCnt;
}

// 2013-04-12 by jhseol, �ʵ�â�� ������� ������þƮ ���� ����
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�Ϲ� â�� ��� ���ε� ������ ����� ���� üũ�ؾ���.
/// \author		jhseol
/// \date		2013-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::GetUsingStore(void)
{
	mt_auto_lock igLock(&m_mapItemGeneral);
	return m_bUsingStore;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�Ϲ� â�� ��� ���ε� ������ ����� ���� üũ�ؾ���.
/// \author		jhseol
/// \date		2013-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::SetUsingStore(BOOL i_bFlag)
{
	mt_auto_lock igLock(&m_mapItemGeneral);
	m_bUsingStore		= i_bFlag;
}
// end 2013-04-12 by jhseol, �ʵ�â�� ������� ������þƮ ���� ����

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-23 by cmkwon, �ʵ�â�� ĳ�� ������ ���� - 
/// \author		cmkwon
/// \date		2009-09-25 ~ 2009-09-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::GetUsingFieldStore(void)
{
	mt_auto_lock igLock(&m_mapItemGeneral);
	return m_bUsingFieldStore;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-23 by cmkwon, �ʵ�â�� ĳ�� ������ ���� - 
/// \author		cmkwon
/// \date		2009-09-25 ~ 2009-09-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::SetUsingFieldStore(BOOL i_bFlag)
{
	mt_auto_lock igLock(&m_mapItemGeneral);
	m_bUsingFieldStore		= i_bFlag;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-23 by cmkwon, �ʵ�â�� ĳ�� ������ ���� - 
/// \author		cmkwon
/// \date		2009-09-25 ~ 2009-09-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::IsUseableFieldStore(void)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);

	ITEM_GENERAL *pItemG = GetItemGeneralByDestParam(DES_FIELD_STORE);

	if(NULL == pItemG)
	{
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-23 by cmkwon, �ʵ�â�� ĳ�� ������ ���� - 
/// \author		cmkwon
/// \date		2009-09-25 ~ 2009-09-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ITEM_GENERAL * CFieldItemManager::GetItemGeneralByDestParam(DestParam_t i_byDestParam, BYTE i_enumStorage/*=ITEM_IN_CHARACTER*/)		// 2011-08-01 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ��� ���� (DestParameter - 255 -> 32767 ����)
{
	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-25 by cmkwon, CFieldItemManager::GetItemGeneralByUID() ItemUID üũ �߰� - 
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	ITEM_GENERAL *pItemG = NULL;
	if (i_enumStorage == ITEM_IN_CHARACTER)
	{
		mtmapUID2ItemGeneral::iterator itr(m_mapItemGeneral.begin());
		for(; itr != m_mapItemGeneral.end(); itr++)
		{
			pItemG = itr->second;
			if(pItemG->ItemInfo->IsExistDesParam(i_byDestParam))
			{
				return pItemG;
			}
		}

		return NULL;
	}

	if (i_enumStorage == ITEM_IN_STORE)
// 2009-10-30 by cmkwon, â�� ������ �������� ���� ���� ���� - CFieldItemManager::GetItemGeneralByDestParam#,
//		&& m_pFieldIOCPSocket->m_pCurrentBuildingNPC != NULL
//		&& m_pFieldIOCPSocket->m_pCurrentBuildingNPC->BuildingKind == BUILDINGKIND_STORE)
	{
		///////////////////////////////////////////////////////////////////////////////
		// 2009-10-30 by cmkwon, â�� ������ �������� ���� ���� ���� - CFieldItemManager::GetItemGeneralByDestParam#, 
		if(NULL == m_pFieldIOCPSocket->m_pCurrentBuildingNPC
			|| BUILDINGKIND_STORE != m_pFieldIOCPSocket->m_pCurrentBuildingNPC->BuildingKind)
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldItemManager::GetItemGeneralByDestParam# State error !! %s DestParam(%d) pBuildingNPC(0x%X) BuildingKind(%d) \r\n"
				, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), i_byDestParam, m_pFieldIOCPSocket->m_pCurrentBuildingNPC, (NULL == m_pFieldIOCPSocket->m_pCurrentBuildingNPC)?-1:m_pFieldIOCPSocket->m_pCurrentBuildingNPC->BuildingKind);
			return NULL;
		}

		mtmapUID2ItemGeneral::iterator itr(m_mapItemInStore.begin());
		for(; itr != m_mapItemInStore.end(); itr++)
		{
			pItemG = itr->second;
			if(pItemG->ItemInfo->IsExistDesParam(i_byDestParam))
			{
				return pItemG;
			}
		}
		
		return NULL;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-11-02 by cmkwon, ĳ��(�κ�/â�� Ȯ��) ������ �߰� ���� - 
/// \author		cmkwon
/// \date		2009-11-02 ~ 2009-11-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CFieldItemManager::GetMaxInventoryCount(BYTE i_enStorage/*=ITEM_IN_CHARACTER*/)
{
	switch(i_enStorage)
	{
	case ITEM_IN_CHARACTER:		return CAtumSJ::GetMaxInventorySize(this->IsExistPremiumCard(), m_pFieldIOCPSocket->m_character.GetAddedPermanentInventoryCount(i_enStorage));
	case ITEM_IN_STORE:
		{
			if(m_uid32StoreCharacterUID == m_pCharacter->CharacterUniqueNumber)
			{
				return CAtumSJ::GetMaxStoreSize(this->IsExistPremiumCard(), m_pFieldIOCPSocket->m_character.GetAddedPermanentInventoryCount(i_enStorage));
			}
			return CAtumSJ::GetMaxStoreSize(this->IsExistPremiumCard(), m_pFieldIOCPSocket->GetAddedPermanentInventoryCountOfOthersCharacter(m_uid32StoreCharacterUID));
		}
	}
	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldItemManager::Add2UsingTimeLimitedItemList(ITEM_GENERAL *i_pItemGen)
/// \brief		
///				// 2010-03-17 by cmkwon, ����1�� �ߵ��� ������ ���� ���� ���� - ���� ������ �Ķ� ���� ���� ���� ����
/// \author		cmkwon
/// \date		2006-03-29 ~ 2006-03-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::Add2UsingTimeLimitedItemList(ITEM_GENERAL *i_pItemGen, BOOL i_bResetAllSkillAndEnchant/*=FALSE*/)
{
	mt_auto_lock igLock(&m_mapItemGeneral);
	for(int i=0; i < m_vectUsingGeneralItem.size(); i++)
	{
		if(i_pItemGen == m_vectUsingGeneralItem[i])
		{
			return FALSE;
		}
	}

	i_pItemGen->UsingStartTime = ATUM_DATE_TIME { true };			// 2006-03-30 by cmkwon, ������ ��� ���۽ð��� �����Ѵ�.
	m_vectUsingGeneralItem.push_back(i_pItemGen);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-17 by cmkwon, ����1�� �ߵ��� ������ ���� ���� ���� - ���� ������ �Ķ� ���� ���� ���� ����
	if(FALSE == IS_ATTACHABLE_ITEM(i_pItemGen->ItemInfo)
		&& i_bResetAllSkillAndEnchant)
	{
		m_pFieldIOCPSocket->ResetAllSkillAndEnchant();
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ITEM_GENERAL * CFieldItemManager::Stop8StoreFromUsingTimeLimitedItemList(ITEM_GENERAL *i_pItemGen, BOOL i_bDeleteUsingTimeLimitedItem)
/// \brief		������� �ð����� �����ۿ��� ���ڷ� ���� �������� ã�Ƽ� ��븮��Ʈ���� �����ϰ� ���� �ð��� ����Ͽ� DB�� �����Ѵ�.
/// \author		cmkwon
/// \date		2006-03-29 ~ 2006-03-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ITEM_GENERAL * CFieldItemManager::Stop8StoreFromUsingTimeLimitedItemList(ITEM_GENERAL *i_pItemGen, BOOL i_bDeleteUsingTimeLimitedItem)
{
	mt_auto_lock igLock(&m_mapItemGeneral);
	for(int i=0; i < m_vectUsingGeneralItem.size(); i++)
	{
		if(i_pItemGen == m_vectUsingGeneralItem[i])
		{
			///////////////////////////////////////////////////////////////////////////////
			// 2006-03-29 by cmkwon, ���� �ð��� ����ϰ� DB�� �����Ѵ�.
			ATUM_DATE_TIME atimeCur { true };
			i_pItemGen->UsingTimeStamp	= i_pItemGen->UsingTimeStamp + atimeCur.GetTimeDiffTimeInSeconds(i_pItemGen->UsingStartTime);
			i_pItemGen->UsingStartTime	= atimeCur;
	
			QPARAM_UPDATE_ITEM_USINGTIMESTAMP *pQParam = new QPARAM_UPDATE_ITEM_USINGTIMESTAMP;
			pQParam->ItemUniqueNumber	= i_pItemGen->UniqueNumber;
			pQParam->UsingTimeStamp1	= i_pItemGen->UsingTimeStamp;
			ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemUsingTimeStamp, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);

			if(i_bDeleteUsingTimeLimitedItem)
			{
				///////////////////////////////////////////////////////////////////////////////
				// 2006-03-29 by cmkwon, ����� ������ ���� ����Ʈ���� �����Ѵ�.                                                         
//				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENACHECK] Stop8StoreFromUsingTimeLimitedItemList ItemUniqueNumber = %d\r\n"
//					, m_vectUsingGeneralItem[i]->UniqueNumber);
				m_vectUsingGeneralItem.erase(m_vectUsingGeneralItem.begin() + i);
			}
			return i_pItemGen;
		}
	}
	return NULL;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldItemManager::ResetTimeStampUsingTimeLimitedItemList(BOOL i_bAfterArena = FALSE)
/// \brief		��� ����� �ð����� �����ۿ� ��� ���� �ð��� �����ϰ� ������ ������ Ŭ���̾�Ʈ�� �����Ѵ�., 2008-02-01 by dhjin, �Ʒ��� ���� - �Ʒ��� ������ ���Ӽ������� ���۽� üũ ��Ʈ �߰�
/// \author		cmkwon
/// \date		2005-XX-XX ~ 2005-XX-XX
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::ResetTimeStampUsingTimeLimitedItemList(BOOL i_bAfterArena /* = FALSE*/)
{
	mt_auto_lock igLock(&m_mapItemGeneral);

	ATUM_DATE_TIME atimeCur { true };
	for(int i=0; i < m_vectUsingGeneralItem.size(); i++)
	{
		if ( NULL == m_vectUsingGeneralItem[i]->ItemInfo )
		{
			g_pFieldGlobal->WriteSystemLogEX ( FALSE , " [ERROR] CFieldItemManager::ResetTimeStampUsingTimeLimitedItemList# ITEM Info NULL!! ItemNum(%d) Loop i(%d)\r\n" ,
				m_vectUsingGeneralItem[i]->ItemNum , i );
			
			m_vectUsingGeneralItem[i]->ItemInfo = this->ms_pFieldIOCP2->GetItemInfo( m_vectUsingGeneralItem[i]->ItemNum );
			
			if ( NULL == m_vectUsingGeneralItem[i]->ItemInfo )
				continue;
		}
		
		///////////////////////////////////////////////////////////////////////////////
		// ���� �ð��� �����Ѵ�
		m_vectUsingGeneralItem[i]->UsingStartTime = atimeCur;
		
		// 2012-11-21 by hskim, ĳ���� ���� �״� ���� ���� - ���� �����Ǹ� �ش� �ڵ� ���� ����
		if( NULL == m_pFieldIOCPSocket )
		{
			g_pFieldGlobal->WriteSystemLogEX ( FALSE , " [DEBUG] CFieldItemManager::ResetTimeStampUsingTimeLimitedItemList# m_pFieldIOCPSocket is NULL!! ItemNum(%d) Loop i(%d) Loop size (%d)\r\n" ,
				m_vectUsingGeneralItem[i]->ItemNum , i, m_vectUsingGeneralItem.size());

			continue;		// �ǹ̾��� -- ��¥�� ��������
		}
		// end 2012-11-21 by hskim, ĳ���� ���� �״� ���� ����

		if(i_bAfterArena)
		{// 2008-03-05 by dhjin, �Ʒ��� ��� ���� �� setparam�� �Ǿ� �����Ƿ� ���ڽ� ī�� �κи� ���� ó���Ѵ�.
			// 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - �Ʒ��� ���� ����
			//if(DES_CASH_STEALTH == m_vectUsingGeneralItem[i]->ItemInfo->DestParameter1)
			if(DES_CASH_STEALTH == m_vectUsingGeneralItem[i]->ItemInfo->ArrDestParameter[0])
			{
				m_pFieldIOCPSocket->m_bStealthState	= TRUE;
			}
		}
		else
		{
			m_pFieldIOCPSocket->ApplyCardItem(m_vectUsingGeneralItem[i]->ItemInfo);
		}
		
		INIT_MSG_WITH_BUFFER(MSG_FC_STORE_INSERT_USINGITEM, T_FC_STORE_INSERT_USINGITEM, pSUsingItem, SendBuf);
		pSUsingItem->ItemNum				= m_vectUsingGeneralItem[i]->ItemNum;
		pSUsingItem->ItemUID				= m_vectUsingGeneralItem[i]->UniqueNumber;
		pSUsingItem->ItemInsertionType		= IUT_LOADING;
		// 2008-11-26 by cmkwon, ��� �� �ð�(����ð�) ���� ������ ���� - �Ʒ��� ���� ���� ��.
		//pSUsingItem->nRemainSecond			= m_vectUsingGeneralItem[i]->ItemInfo->Time/1000 - m_vectUsingGeneralItem[i]->UsingTimeStamp;
		// 2013-01-02 by jhseol, ĳ���� �Ʒ��� ���� ũ���� ����� �α� ���� ���� ����
#if defined(S_CAN_SERVER_SETTING_HSSON) || defined(S_DA_SERVER_SETTING_HSSON) || defined(S_ETRS_SERVER_SETTING_HSSON)
		BOOL IsArenaServerCheck = g_pFieldGlobal->IsArenaServer();
		UID32_t	tempCUID = 0;
		if( IsArenaServerCheck )
		{
			if( NULL == m_pFieldIOCPSocket )
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DEBUG_SEOL_E1] CUID(%8d) :: m_pFieldIOCPSocket Pointer is NULL \r\n", tempCUID);
				if( NULL != this )
				{
					if( NULL != this->m_pCharacter )
					{
						tempCUID = this->m_pCharacter->CharacterUniqueNumber;
						g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DEBUG_SEOL_E2] CUID(%8d) :: CUID is this Pointer Find \r\n", tempCUID);
					}
				}
				if( 0 == tempCUID )
				{
					tempCUID = GetTickCount();
					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DEBUG_SEOL_E3] CUID(%8d) :: CUID is GetTickCount \r\n", tempCUID);
				}
			}
			else
			{
				tempCUID = m_pFieldIOCPSocket->m_character.CharacterUniqueNumber;
			}
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DEBUG_SEOL_01] CUID(%8d) :: VectorSize(%d), ForloopIndex(%d) \r\n", tempCUID, m_vectUsingGeneralItem.size(), i);
			if( NULL == this )
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DEBUG_SEOL_E4] CUID(%8d) :: This Pointer is NULL \r\n", tempCUID);
			}
			if( NULL == m_vectUsingGeneralItem[i]->ItemInfo )
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DEBUG_SEOL_E5] CUID(%8d) :: ForloopIndex ItemInfo Pointer is NULL \r\n", tempCUID);
			}
			if( i < m_vectUsingGeneralItem.size() )
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DEBUG_SEOL_02] CUID(%8d) :: ItemNum(%d), ForloopIndex Item UsingTimeStamp(%d) \r\n"
					, tempCUID, m_vectUsingGeneralItem[i]->ItemNum, m_vectUsingGeneralItem[i]->UsingTimeStamp);
			}
		}
		pSUsingItem->nRemainSecond			= CAtumSJ::GetTimeSecondByItemKind8ItemAttribute(m_vectUsingGeneralItem[i]->ItemInfo) - m_vectUsingGeneralItem[i]->UsingTimeStamp;
		if( IsArenaServerCheck )
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DEBUG_SEOL_03] CUID(%8d) :: Pass to Result ItemNum(%d), pSUsingItem->nRemainSecond(%d) \r\n", tempCUID, pSUsingItem->nRemainSecond);
		}
#else
		pSUsingItem->nRemainSecond			= CAtumSJ::GetTimeSecondByItemKind8ItemAttribute(m_vectUsingGeneralItem[i]->ItemInfo) - m_vectUsingGeneralItem[i]->UsingTimeStamp;
#endif	// #ifdef S_CAN_SERVER_SETTING_HSSON || S_DA_SERVER_SETTING_HSSON || S_ETRS_SERVER_SETTING_HSSON
		// end 2013-01-02 by jhseol, ĳ���� �Ʒ��� ���� ũ���� ����� �α� ���� ���� ����
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENACHECK] T_FC_STORE_INSERT_USINGITEM nRemainSecond = %d, Min = %d, S = %d\r\n"
//					, pSUsingItem->nRemainSecond, m_vectUsingGeneralItem[i]->UsingStartTime.Minute, m_vectUsingGeneralItem[i]->UsingStartTime.Second);		
		m_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_STORE_INSERT_USINGITEM));
	}

	///////////////////////////////////////////////////////////////////////////////
	// ��ȿ �ð� üũ
	this->CheckExpireUsingTimeLimitedItemList();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldItemManager::CheckExpireUsingTimeLimitedItemList(UID64_t i_itemUID/*=INVALID_UID64*/)
/// \brief		��� ����� �ð����� �������� �����ð��� üũ�ϰ� ���ð��� ����� �������� ���� ó���ϰ� Ŭ���̾�Ʈ�� ������ �����Ѵ�.
///				// 2006-10-11 by cmkwon, ���� �߰�(UID64_t i_itemUID/*=INVALID_UID64*/)
///				// 2009-10-22 by cmkwon, �ð� �������� ������ �ý��� ���� - CFieldItemManager::CheckExpireUsingTimeLimitedItemList#, �ʿ� ���� �ּ� ���� �� ����
/// \author		cmkwon
/// \date		2005-XX-XX ~ 2005-XX-XX
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::CheckExpireUsingTimeLimitedItemList(UID64_t i_itemUID/*=INVALID_UID64*/)
{
	// 2008-03-05 by dhjin, �Ʒ��� ���� - �Ʒ��� ��� �߿��� �������� �ʴ´�.
	if(CS_ARENASERVER_PLAYING == m_pFieldIOCPSocket->GetClientState())		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - CS_ARENA_PLAYING => CS_ARENASERVER_PLAYING ����
	{
		return;
	}

	// 2010-03-17 by cmkwon, ����1�� �ߵ��� ������ ���� ���� ���� - ���� ������ ����� �Ķ� ó��
	BOOL bCallResetAllSkillAndEnchant = FALSE;

	mt_auto_lock igLock(&m_mapItemGeneral);

	ATUM_DATE_TIME atimeCur { true };
	vectITEM_GENERALPtr::iterator itr = m_vectUsingGeneralItem.begin();
	while (itr != m_vectUsingGeneralItem.end())
	{
		ITEM_GENERAL *pItemGen = *itr;
		if(IS_VALID_UID64(i_itemUID)
			&& pItemGen->UniqueNumber != i_itemUID)
		{// 2006-10-11 by cmkwon, �ϳ��� �����۸� üũ�Ѵ�
			itr++;
			continue;
		}

		int nCurUsingSec	= pItemGen->UsingTimeStamp + atimeCur.GetTimeDiffTimeInSeconds(pItemGen->UsingStartTime);
		int temp1 = pItemGen->UsingTimeStamp;
		int temp2 = atimeCur.GetTimeDiffTimeInSeconds(pItemGen->UsingStartTime);

		// 2010-12-21 by jskim, ���� �̵� �ӵ� ���� ������ ����
		//if(CAtumSJ::GetTimeSecondByItemKind8ItemAttribute(pItemGen->ItemInfo) <= nCurUsingSec)
		if(CAtumSJ::GetTimeSecondByItemKind8ItemAttribute(pItemGen->ItemInfo) <= nCurUsingSec && pItemGen->ItemInfo->Time != NULL)
		// end 2010-12-21 by jskim, ���� �̵� �ӵ� ���� ������ ����
		
		{
			// 2009-10-22 by cmkwon, �ð� �������� ������ �ý��� ���� - CFieldItemManager::CheckExpireUsingTimeLimitedItemList#, �������� üũ 			
			if(WEAR_ATTACHED == pItemGen->Wear)
			{
				MSG_FC_ITEM_CHANGE_WINDOW_POSITION tmItemChangePos;
				util::zero(&tmItemChangePos, sizeof(MSG_FC_ITEM_CHANGE_WINDOW_POSITION));
				tmItemChangePos.CharacterUniqueNumber	= m_pFieldIOCPSocket->m_character.CharacterUniqueNumber;
				tmItemChangePos.FromItemUniqueNumber	= pItemGen->UniqueNumber;
				tmItemChangePos.FromItemWindowIndex		= pItemGen->ItemWindowIndex;
				tmItemChangePos.ToItemUniqueNumber		= 0;
				tmItemChangePos.ToItemWindowIndex		= POS_ITEMWINDOW_OFFSET;
				m_pFieldIOCPSocket->ProcessItemChangeWindowPosition(&tmItemChangePos, FALSE, FALSE);	// 2009-10-22 by cmkwon, �ð� �������� ������ �ý��� ���� - CFieldIOCPSocket::ProcessItemChangeWindowPosition# ����

				// 2008-01-23 by cmkwon, S_F, S_L: ����/�������� ���� �α׿� �߰� - 
				// 2009-11-17 by cmkwon, �������� ���� ������ ������ �α� ���� �߰� - CFieldItemManager::CheckExpireUsingTimeLimitedItemList#
				CAtumLogSender::SendLogMessageITEMDelete(m_pFieldIOCPSocket, pItemGen, IDT_EXPIRE_TIME, nCurUsingSec);
				this->DeleteItemFromCharacterByPointer(pItemGen, IUT_EXPIRE_CARD_ITEM);
			}
			else
			{
				// 2008-01-23 by cmkwon, S_F, S_L: ����/�������� ���� �α׿� �߰� - 
				// 2009-11-17 by cmkwon, �������� ���� ������ ������ �α� ���� �߰� - CFieldItemManager::CheckExpireUsingTimeLimitedItemList#
				CAtumLogSender::SendLogMessageITEMDelete(m_pFieldIOCPSocket, pItemGen, IDT_EXPIRE_TIME, nCurUsingSec);

				///////////////////////////////////////////////////////////////////////////////
				// 1. ������ ��� ����
				m_pFieldIOCPSocket->ReleaseCardItem(pItemGen->ItemInfo);

				///////////////////////////////////////////////////////////////////////////////
				// 2. Ŭ���̾�Ʈ�� ����
				INIT_MSG_WITH_BUFFER(MSG_FC_STORE_DELETE_USINGITEM, T_FC_STORE_DELETE_USINGITEM, pSDUsingItem, SendBuf);
				pSDUsingItem->ItemNum			= pItemGen->ItemNum;
				pSDUsingItem->ItemDeletionType	= IUT_EXPIRE_CARD_ITEM;
				m_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_STORE_DELETE_USINGITEM));

				///////////////////////////////////////////////////////////////////////////////
				// 3. DB���� ������ ����			
				QPARAM_DELETESTOREITEM *pQParam	= new QPARAM_DELETESTOREITEM;
				pQParam->AccountUniqueNumber	= m_pFieldIOCPSocket->m_character.AccountUniqueNumber;
				pQParam->ItemUniqueNumber		= pItemGen->UniqueNumber;
				ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_DeleteStoreItem, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
				
				this->ItemDeleteByInfinityPlaying(pItemGen);		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� �� ������ ����

				///////////////////////////////////////////////////////////////////////////////
				// 4. ITEM_GENERAL�� �޸� ����
				util::del(pItemGen);

				// 2010-03-17 by cmkwon, ����1�� �ߵ��� ������ ���� ���� ���� - ���� ������ ����� �Ķ� ó��
				if(FALSE == bCallResetAllSkillAndEnchant)
				{
					bCallResetAllSkillAndEnchant = TRUE;
				}
			}
			itr = m_vectUsingGeneralItem.erase(itr);

#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, �������� �����߰�
			if( NULL != m_pFieldIOCPSocket )		//	2013-07-07 by bckim, ���� ����� ���� 
			{
			CFieldParty *pFParty = m_pFieldIOCPSocket->m_pFieldParty;
				if( NULL != pFParty )				//	2013-07-07 by bckim, ���� ����� ���� 
			{
				pFParty->SetReturnUserBuffSkill(m_pFieldIOCPSocket); // ������ �Ⱓ����� �̹� ������ �����ǰ� DB ���� �Ǿ���
				}			
			}			
#endif
			
			if(IS_VALID_UID64(i_itemUID))
			{// 2006-10-11 by cmkwon, ������ �ϳ��� üũ
				return;
			}
			
			continue;
		}
		itr++;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-17 by cmkwon, ����1�� �ߵ��� ������ ���� ���� ���� - ���� ������ ����� �Ķ� ó��
	if(bCallResetAllSkillAndEnchant)
	{
		m_pFieldIOCPSocket->ResetAllSkillAndEnchant();
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2006-09-29 by cmkwon, ������ ���� �� �ڵ� ���� ������ ���� üũ
	mtmapUID2ItemGeneral::iterator itrG(m_mapItemGeneral.begin());
	while(itrG != m_mapItemGeneral.end())
	{
		ITEM_GENERAL *pItemGen = itrG->second;		
		if(IS_VALID_UID64(i_itemUID)
			&& pItemGen->UniqueNumber != i_itemUID)
		{// 2006-10-11 by cmkwon, �ϳ��� �����۸� üũ�Ѵ�

			itrG++;
			continue;
		}

		if(COMPARE_BIT_FLAG(pItemGen->ItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE))
		{
			ATUM_DATE_TIME expireATime;
			expireATime			= pItemGen->CreatedTime;
			// 2009-10-22 by cmkwon, �ð� �������� ������ �ý��� ���� - CFieldItemManager::CheckExpireUsingTimeLimitedItemList#, 
			//expireATime.AddDateTime(0, 0, 0, pItemGen->ItemInfo->Endurance, 0, 0);
			expireATime.AddDateTime(0, 0, 0, 0, CAtumSJ::GetTimeMinuteByItemKind8ItemAttribute(pItemGen->ItemInfo, TIME_TERM_DELETE_ITEM));	// 2009-10-22 by cmkwon, �ð� �������� ������ �ý��� ���� - CFieldItemManager::CheckExpireUsingTimeLimitedItemList#,

			if(atimeCur > expireATime)
			{// 2006-09-29 by cmkwon, �Ⱓ ����, �����ؾ���

				// 2009-10-22 by cmkwon, �ð� �������� ������ �ý��� ���� - CFieldItemManager::CheckExpireUsingTimeLimitedItemList#, �Ʒ��� ���� ó��
				if(IS_ATTACHABLE_ITEM(pItemGen->ItemInfo))
				{// 2009-10-22 by cmkwon, ���� ���� �����۷�
					if(WEAR_ATTACHED == pItemGen->Wear)
					{
						///////////////////////////////////////////////////////////////////////////////
						// 2007-09-07 by cmkwon, �Ⱓ ����� ������ ������ ���� ���� ó�� - ��ũ �������� �Ⱓ ���� �����ۿ� �߰���
						MSG_FC_ITEM_CHANGE_WINDOW_POSITION tmItemChangePos;
						util::zero(&tmItemChangePos, sizeof(MSG_FC_ITEM_CHANGE_WINDOW_POSITION));
						tmItemChangePos.CharacterUniqueNumber	= m_pFieldIOCPSocket->m_character.CharacterUniqueNumber;
						tmItemChangePos.FromItemUniqueNumber	= pItemGen->UniqueNumber;
						tmItemChangePos.FromItemWindowIndex		= pItemGen->ItemWindowIndex;
						tmItemChangePos.ToItemUniqueNumber		= 0;
						tmItemChangePos.ToItemWindowIndex		= POS_ITEMWINDOW_OFFSET;
						m_pFieldIOCPSocket->ProcessItemChangeWindowPosition(&tmItemChangePos, FALSE, FALSE);	// 2009-10-22 by cmkwon, �ð� �������� ������ �ý��� ���� - CFieldIOCPSocket::ProcessItemChangeWindowPosition# ����
					}
				}

				// 2008-01-23 by cmkwon, S_F, S_L: ����/�������� ���� �α׿� �߰� - �Ʒ��� ���� CAtumLogSender::SendLogMessageITEMDelete() �� ��ü ��
				// 2012-08-30 by jhseol, Ǯ�α� �����
#ifdef S_FULL_LOG_JHSEOL
				CAtumLogSender::SendLogMessageITEMUseITEM(m_pFieldIOCPSocket, pItemGen);						// ��� ������ �α� �߰�
#endif	//#ifdef S_FULL_LOG_JHSEOL
				// end 2012-08-30 by jhseol, Ǯ�α� �����
				// 2008-01-23 by cmkwon, S_F, S_L: ����/�������� ���� �α׿� �߰� - �Ⱓ ����� ������
				CAtumLogSender::SendLogMessageITEMDelete(m_pFieldIOCPSocket, pItemGen, IDT_EXPIRE_TIME);

				// 1. m_mapItemGeneral���� ����
				itrG = m_mapItemGeneral.erase(itrG);

				// 2. DB���� ������ ����
				QPARAM_DELETESTOREITEM *pQParam = new QPARAM_DELETESTOREITEM;
				pQParam->AccountUniqueNumber	= m_pFieldIOCPSocket->m_character.AccountUniqueNumber;
				pQParam->ItemUniqueNumber		= pItemGen->UniqueNumber;
				ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_DeleteStoreItem, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);

				// 3. Ŭ���̾�Ʈ�� ����
				INIT_MSG_WITH_BUFFER(MSG_FC_STORE_DELETE_ITEM, T_FC_STORE_DELETE_ITEM, msgDelete, msgDeleteBuf);
				msgDelete->ItemUniqueNumber		= pItemGen->UniqueNumber;
				msgDelete->ItemDeletionType		= IUT_EXPIRE_ITEM;
				m_pFieldIOCPSocket->SendAddData(msgDeleteBuf, MSG_SIZE(MSG_FC_STORE_DELETE_ITEM));

				util::del(pItemGen);

				if(IS_VALID_UID64(i_itemUID))
				{// 2006-10-11 by cmkwon, ������ �ϳ��� üũ
					return;
				}
				
				continue;
			}
		}
		itrG++;
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldItemManager::StoreTimeStampUsingTimeLimitedIteList(void)
/// \brief		������� ��� ���ð����� �������� ���� �ð��� ����Ͽ� DB�� �����Ѵ�
/// \author		cmkwon
/// \date		2005-XX-XX ~ 2005-XX-XX
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::StoreTimeStampUsingTimeLimitedIteList(void)
{
	mt_auto_lock igLock(&m_mapItemGeneral);

	ATUM_DATE_TIME atimeCur { true };
	for(int i=0; i < m_vectUsingGeneralItem.size(); i++)
	{
		ITEM_GENERAL *pItemGen		= m_vectUsingGeneralItem[i];
		pItemGen->UsingTimeStamp	= pItemGen->UsingTimeStamp + atimeCur.GetTimeDiffTimeInSeconds(pItemGen->UsingStartTime);
		pItemGen->UsingStartTime	= atimeCur;
		
		////////////////////////////////////////////////////////////////////////////////
		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ذ� ���� ����, ���� ���� �� ������ ����
// 		QPARAM_UPDATE_ITEM_USINGTIMESTAMP *pQParam = new QPARAM_UPDATE_ITEM_USINGTIMESTAMP;
// 		pQParam->ItemUniqueNumber	= pItemGen->UniqueNumber;
// 		pQParam->UsingTimeStamp1	= pItemGen->UsingTimeStamp;
// 		ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemUsingTimeStamp, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
		if(FALSE == g_pFieldGlobal->IsArenaServer()) {
			QPARAM_UPDATE_ITEM_USINGTIMESTAMP *pQParam = new QPARAM_UPDATE_ITEM_USINGTIMESTAMP;
			pQParam->ItemUniqueNumber	= pItemGen->UniqueNumber;
			pQParam->UsingTimeStamp1	= pItemGen->UsingTimeStamp;
			ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemUsingTimeStamp, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
		}
		else {
//			if(pItemGen->ItemInfo->Kind == ITEMKIND_CARD && COMPARE_BIT_FLAG(pItemGen->ItemInfo->ItemAttribute, ITEM_ATTR_TIME_LIMITE|ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED)) {
			if(IS_ATTACHABLE_ITEM(pItemGen->ItemInfo)) {
				this->ItemUpdateByInfinityPlaying(pItemGen);
			}
			else {

				// ���Ǵ�Ƽ ���� ��� ������ ���� ���� ó��. 2010. 08. 09. by hsLee. - ���� ó������ ����� �����۸� ���� ���ϵ��� ����.
				if ( ITEM_NOT_USING != pItemGen->UsingTimeStamp
					&& COMPARE_BIT_FLAG(pItemGen->ItemInfo->ItemAttribute, ITEM_ATTR_ONLY_USE_INFINITY ) ) 
				{
					// 2009-09-09 ~ 2010-01-28 by dhjin, ���Ǵ�Ƽ - ���ǿ����� ��� ������ ������ ������ ���ƿö� ����
					continue;
				}

				// ���� �ٷ� �����Ǵ� �ð��� ������
				CIOCPSocket	*	MFSSock;
				MFSSock = this->ms_pFieldIOCP2->GetMFSSockForArenaServer();
				if(NULL == MFSSock) {// 2007-12-28 by dhjin, �ʵ� ������ ���� ������ ������ ����
					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR][Infinity][%I64d] MFS NULL 'StoreTimeStampUsingTimeLimitedIteList()', CharacterName = %s \r\n", m_pFieldIOCPSocket->m_InfinityPlayingInfo.InfinityCreateUID, m_pFieldIOCPSocket->m_character.CharacterName);
					break;
				}
				INIT_MSG_WITH_BUFFER(MSG_FtoA_INFINITY_USING_TIME_LIMIT_ITEM, T_FtoA_INFINITY_USING_TIME_LIMIT_ITEM, pUpdateItemSendMsg, UpdateItemSendBuf);
				pUpdateItemSendMsg->AccountUID			= m_pFieldIOCPSocket->m_character.AccountUniqueNumber;
				pUpdateItemSendMsg->MFSCharacterUID		= m_pFieldIOCPSocket->m_AfsNeedMfsInfo.MFSCharacterUID;
				pUpdateItemSendMsg->MFSClientIndex		= m_pFieldIOCPSocket->m_AfsNeedMfsInfo.MFSClientIdx;
				pUpdateItemSendMsg->CurrentCount		= pItemGen->CurrentCount;
				pUpdateItemSendMsg->ItemWindowIndex		= pItemGen->ItemWindowIndex;
				pUpdateItemSendMsg->MainSvrItemUID		= pItemGen->MainSvrItemUID;
				pUpdateItemSendMsg->Wear				= pItemGen->Wear;
				pUpdateItemSendMsg->UsingTimeStamp		= pItemGen->UsingTimeStamp;
				pUpdateItemSendMsg->CreatedTime			= pItemGen->CreatedTime;
				pUpdateItemSendMsg->ItemNum				= pItemGen->ItemNum;
				pUpdateItemSendMsg->CoolingTimeStamp	= pItemGen->CoolingTimeStamp;		// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
				MFSSock->SendAddData(UpdateItemSendBuf, MSG_SIZE(MSG_FtoA_INFINITY_USING_TIME_LIMIT_ITEM));
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ITEM][Infinity][%I64d] MSG_FtoA_INFINITY_USING_TIME_LIMIT_ITEM, CharacterName = %s, ItemNum = %d, CurrentCnt = %d, Wear = %d, UsingTimeStamp = %d \r\n"
					, m_pFieldIOCPSocket->m_InfinityPlayingInfo.InfinityCreateUID, m_pFieldIOCPSocket->m_character.CharacterName, pItemGen->ItemInfo->ItemNum
					, pItemGen->CurrentCount, pItemGen->Wear, pItemGen->UsingTimeStamp);
			} // if(IS_ATTACHABLE_ITEM(pItemGen->ItemInfo)) {} else {
		} // if(FALSE == g_pFieldGlobal->IsArenaServer()) {} else {
	} // for(int i=0; i < m_vectUsingGeneralItem.size(); i++)
}

SDelayStoreItemInfo *CFieldItemManager::FindDelayStoredItemInfoByItemUID(UID64_t i_n64ItemUID)
{
	for(int i=0; i < m_vectDelayStoredItemInfo.size(); i++)
	{
		if(i_n64ItemUID == m_vectDelayStoredItemInfo[i].n64ItemUID)
		{
			return &m_vectDelayStoredItemInfo[i];
		}
	}
	
	return NULL;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		DB�� ����� ī��Ʈ�� ������ �д�
/// \author		cmkwon
/// \date		2006-06-01 ~ 2006-06-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::SetDelayStoredItemCounts(ITEM_GENERAL *i_pItemG)
{
	if(FALSE == CFieldItemManager::IsDelayStoreItem(i_pItemG->ItemInfo))
	{
		return FALSE;
	}

	SDelayStoreItemInfo *pSItemInfo = FindDelayStoredItemInfoByItemUID(i_pItemG->UniqueNumber);
	if(pSItemInfo)
	{
		pSItemInfo->nStoredItemCounts		= i_pItemG->CurrentCount;
		return TRUE;
	}
	
	m_vectDelayStoredItemInfo.push_back(SDelayStoreItemInfo(i_pItemG->UniqueNumber, i_pItemG->CurrentCount));
	return FALSE;
}

BOOL CFieldItemManager::DeleteDelayStoredItemInfoByItemUID(UID64_t i_n64ItemUID)
{
	vectDelayStoreItemInfo::iterator itr(m_vectDelayStoredItemInfo.begin());

	for(; itr != m_vectDelayStoredItemInfo.end(); itr++)
	{
		if(i_n64ItemUID == itr->n64ItemUID)
		{
			m_vectDelayStoredItemInfo.erase(itr);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CFieldItemManager::StoreCountableItem(void)
{
	mt_auto_lock igLock(&m_mapItemGeneral);
	
	mtmapUID2ItemGeneral::iterator itr(m_mapItemGeneral.begin());
	for(; itr != m_mapItemGeneral.end(); itr++)
	{
		ITEM_GENERAL *pItemGen = itr->second;		
		if(IsDelayStoreItem(pItemGen->ItemInfo))
		{
			BOOL bStore2DB = FALSE;
			SDelayStoreItemInfo *pStoredInfo = FindDelayStoredItemInfoByItemUID(pItemGen->UniqueNumber);
			
			if(pStoredInfo
				&& pStoredInfo->nStoredItemCounts != pItemGen->CurrentCount)
			{
				pStoredInfo->nStoredItemCounts	= pItemGen->CurrentCount;
				bStore2DB	= TRUE;
			}
			else if(NULL == pStoredInfo)
			{
				SetDelayStoredItemCounts(pItemGen);
				pStoredInfo	= FindDelayStoredItemInfoByItemUID(pItemGen->UniqueNumber);
				pStoredInfo->nStoredItemCounts	= pItemGen->CurrentCount;
				bStore2DB	= TRUE;
			}
			
			if(bStore2DB)
			{
				////////////////////////////////////////////////////////////////////////////////
				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ذ� ���� ����, ���� ���� �� ������ ����
// 				QPARAM_UPDATEITEMCOUNT *pQParam	= new QPARAM_UPDATEITEMCOUNT;
// 				pQParam->ItemUniqueNumber		= pItemGen->UniqueNumber;
// 				pQParam->Count					= pItemGen->CurrentCount;
// 				ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemCount, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
				if(FALSE == g_pFieldGlobal->IsArenaServer()) {
					QPARAM_UPDATEITEMCOUNT *pQParam	= new QPARAM_UPDATEITEMCOUNT;
					pQParam->ItemUniqueNumber		= pItemGen->UniqueNumber;
					pQParam->Count					= pItemGen->CurrentCount;
					ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemCount, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
				}
				else {
					this->ItemUpdateByInfinityPlaying(pItemGen);
				}
			}
		}
	}

	return TRUE;
}

BOOL CFieldItemManager::IsDelayStoreItem(ITEM *i_pItem)
{
	if(IS_COUNTABLE_ITEM(i_pItem->Kind))
	{
		return FALSE;
	}
	
	if(MONEY_ITEM_NUMBER == i_pItem->ItemNum)
	{// 2006-05-21 by cmkwon, ���� ���� ������Ʈ ���������� ó���Ѵ�.
		return TRUE;
	}

	if(ITEMKIND_ENERGY != i_pItem->Kind)
	{
		return FALSE;
	}

	// 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - �Ʒ��� ���� ����
	//switch(i_pItem->DestParameter1)
	switch(i_pItem->ArrDestParameter[0])
	{
	case DES_GRADUAL_HP_UP:
	case DES_GRADUAL_DP_UP:
	case DES_GRADUAL_EP_UP:
	case DES_IN_TIME_HP_UP:
	case DES_IN_TIME_DP_UP:
	case DES_IN_TIME_EP_UP:
	case DES_IMMEDIATE_HP_UP:
	case DES_IMMEDIATE_DP_UP:
	case DES_IMMEDIATE_EP_UP:
		{
		}
		return TRUE;
	}

	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldItemManager::IsExistPremiumCard(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-03-30 ~ 2006-03-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::IsExistPremiumCard(void)
{
	if(CASH_PREMIUM_CARD_STATE_NORMAL != m_cashPremiumCardInfo.enCardState
		|| 0 == m_cashPremiumCardInfo.n64UniqueNumber10
		|| m_pFieldIOCPSocket->m_character.AccountUniqueNumber != m_cashPremiumCardInfo.nAccountUID10)
	{
		return FALSE;
	}

	return TRUE;
}

void CFieldItemManager::CashInitPremiumCardInfo(SCASH_PREMEIUM_CARD_INFO *i_pCardInfo)
{
	mt_auto_lock at(&m_mapItemGeneral);
	if(i_pCardInfo->nAccountUID10 != m_pFieldIOCPSocket->m_character.AccountUniqueNumber)
	{
		return;
	}

	m_cashPremiumCardInfo				= *i_pCardInfo;
	m_cashPremiumCardInfo.enCardState	= CASH_PREMIUM_CARD_STATE_NORMAL;
	m_cashPremiumCardInfo.ResetAllPlusRateByPremiumCard();		// 2006-10-11 by cmkwon
}

Err_t CFieldItemManager::CashInsertPremiumCard(ITEM *i_pItem, BOOL i_bOnlyCheck/*=FALSE*/)
{
	if(NULL == i_pItem){			return ERR_PROTOCOL_NO_SUCH_ITEM_INFO;}		// 2006-04-25 by cmkwon
// 2008-10-20 by cmkwon, ����� ������ üũ ItemAttribute �� ó�� - �Ʒ��� ���� ���� 
//	if(CASH_ITEMKIND_PREMIUM_CARD != i_pItem->SummonMonster)
	if(FALSE == COMPARE_BIT_FLAG(i_pItem->ItemAttribute, ITEM_ATTR_CASH_ITEM_PREMIUM_CARD))
	{
		return ERR_CASH_PREMIUM_CARD_INVALID_ITEMNUM;
	}

	mt_auto_lock at(&m_mapItemGeneral);
	if(FALSE == m_pFieldIOCPSocket->IsUsing())
	{
		return ERR_COMMON_SOCKET_CLOSED;
	}
	else if(CASH_PREMIUM_CARD_STATE_NOT_EXIST != m_cashPremiumCardInfo.enCardState
		&& CASH_PREMIUM_CARD_STATE_NORMAL != m_cashPremiumCardInfo.enCardState)
	{
		return ERR_CASH_PREMIUM_CARD_ALREADY_USING;
	}

	if(CASH_PREMIUM_CARD_STATE_NOT_EXIST == m_cashPremiumCardInfo.enCardState)
	{// ������� �����̾�ī�尡 ����

// 2006-07-05 by cmkwon, �����̾� ī�� �ý����� MonthlyPay�� ����
// 		if(ITEM_NUM_CASH_UPGRADE_PREMIUM_CARD == i_pItem->ItemNum)
// 		{
// 			return ERR_CASH_PREMIUM_CARD_NOT_MATCHED;
// 		}

		if(FALSE == i_bOnlyCheck)
		{
			m_cashPremiumCardInfo.enCardState			= CASH_PREMIUM_CARD_STATE_INSERTING;		
			m_cashPremiumCardInfo.n64UniqueNumber10		= 0;
			m_cashPremiumCardInfo.nAccountUID10			= m_pFieldIOCPSocket->m_character.AccountUniqueNumber;
			m_cashPremiumCardInfo.nCardItemNum			= i_pItem->ItemNum;
			m_cashPremiumCardInfo.atumTimeUpdatedTime.SetCurrentDateTime();
			m_cashPremiumCardInfo.atumTimeUpdatedTime.Second = 0;
			m_cashPremiumCardInfo.atumTimeCreatedTime	= m_cashPremiumCardInfo.atumTimeUpdatedTime;			
			m_cashPremiumCardInfo.atumTimeExpireTime	= m_cashPremiumCardInfo.atumTimeUpdatedTime;
			m_cashPremiumCardInfo.atumTimeExpireTime.AddDateTime(0, 0, i_pItem->Time, 0, 0, 0);

			QPARAM_CASH_INSERT_PREMIUM_CARD *pQParam = new QPARAM_CASH_INSERT_PREMIUM_CARD;
			util::zero(pQParam, sizeof(QPARAM_CASH_INSERT_PREMIUM_CARD));

			pQParam->nAccountUID			= m_cashPremiumCardInfo.nAccountUID10;
			pQParam->nItemNum				= m_cashPremiumCardInfo.nCardItemNum;
			pQParam->atumTimeCurrentTime	= m_cashPremiumCardInfo.atumTimeUpdatedTime;
			pQParam->atumTimeExpireTime		= m_cashPremiumCardInfo.atumTimeExpireTime;
			ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_CashInsertPremiumCard, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
		}		
	}
	else
	{// �̹� ī�尡 �����Ѵ� - 

		ATUM_DATE_TIME atimeMaxDate(true);

		// 2007-10-01 by cmkwon, ����� ���� ���� �ִ� �ϼ� - �� ������ �ִ� �ϼ��� �����´�
		atimeMaxDate.AddDateTime(0, 0, CFieldIOCPSocket::GetMaxBuyableDayOfMembership());
		ATUM_DATE_TIME atimeExpireDate = m_cashPremiumCardInfo.atumTimeExpireTime;
		atimeExpireDate.AddDateTime(0, 0, i_pItem->Time, 0, 0, 0);

		if(atimeMaxDate < atimeExpireDate)
		{// ���� ���� �Ⱓ�� �����Ѵ�

			return ERR_CASH_PREMIUM_CARD_NOT_YET_BUY_STATE;
		}

		if(FALSE == i_bOnlyCheck)
		{
			m_cashPremiumCardInfo.enCardState			= CASH_PREMIUM_CARD_STATE_UPDATING;			
			m_cashPremiumCardInfo.atumTimeUpdatedTime.SetCurrentDateTime();
			m_cashPremiumCardInfo.atumTimeUpdatedTime.Second = 0;
			m_cashPremiumCardInfo.atumTimeExpireTime.AddDateTime(0, 0, i_pItem->Time);

			QPARAM_CASH_UPDATE_PREMIUM_CARD *pQParam = new QPARAM_CASH_UPDATE_PREMIUM_CARD;
			util::zero(pQParam, sizeof(QPARAM_CASH_UPDATE_PREMIUM_CARD));

			pQParam->nAccountUID			= m_cashPremiumCardInfo.nAccountUID10;
			pQParam->nItemNum				= m_cashPremiumCardInfo.nCardItemNum;
			pQParam->atumTimeCurrentTime	= m_cashPremiumCardInfo.atumTimeUpdatedTime;
			pQParam->atumTimeExpireTime		= m_cashPremiumCardInfo.atumTimeExpireTime;
			ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_CashUpdatePremiumCard, m_pFieldIOCPSocket
				, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
		}

// 2006-07-05 by cmkwon, �����̾� ī�� �ý����� MonthlyPay�� �����ϸ鼭 ���� ���� �����Ǿ����ϴ�.
// 		if(m_cashPremiumCardInfo.nCardItemNum == i_pItem->ItemNum)
// 		{// ���� ������ ī�带 �����Ѵ�
// 			
// 			ATUM_DATE_TIME tmAtumTime(true);
// 			tmAtumTime.AddDateTime(0, 0, i_pItem->Time*TERM_ENABLE_CASH_PREMIUM_CARD_BUY_MONTH, 0, 0, 0);
// 
// 			if(tmAtumTime <= m_cashPremiumCardInfo.atumTimeExpireTime)
// 			{// ���� ���� �Ⱓ�� �����Ѵ�
// 				
// 				return ERR_CASH_PREMIUM_CARD_NOT_YET_BUY_STATE;
// 			}
// 
// 			if(FALSE == i_bOnlyCheck)
// 			{
// 				m_cashPremiumCardInfo.enCardState			= CASH_PREMIUM_CARD_STATE_UPDATING;			
// 				m_cashPremiumCardInfo.atumTimeUpdatedTime.SetCurrentDateTime();
// 				m_cashPremiumCardInfo.atumTimeUpdatedTime.Second = 0;
// 				m_cashPremiumCardInfo.atumTimeExpireTime.AddDateTime(0, 0, i_pItem->Time, 0, 0, 0);
// 
// 				QPARAM_CASH_UPDATE_PREMIUM_CARD *pQParam = new QPARAM_CASH_UPDATE_PREMIUM_CARD;
// 				util::zero(pQParam, sizeof(QPARAM_CASH_UPDATE_PREMIUM_CARD));
// 
// 				pQParam->nAccountUID			= m_cashPremiumCardInfo.nAccountUID10;
// 				pQParam->nItemNum				= m_cashPremiumCardInfo.nCardItemNum;
// 				pQParam->atumTimeCurrentTime	= m_cashPremiumCardInfo.atumTimeUpdatedTime;
// 				pQParam->atumTimeExpireTime		= m_cashPremiumCardInfo.atumTimeExpireTime;
// 				ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_CashUpdatePremiumCard, m_pFieldIOCPSocket
// 					, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
// 			}
// 		}
// 		else
// 		{// ���׷��̵� ī�� ����
// 
// 			if(ITEM_NUM_CASH_NORMAL_PREMIUM_CARD != m_cashPremiumCardInfo.nCardItemNum
// 				|| ITEM_NUM_CASH_UPGRADE_PREMIUM_CARD != i_pItem->ItemNum)
// 			{
// 				return ERR_CASH_PREMIUM_CARD_NOT_MATCHED;
// 			}
// 
// 			if(FALSE == i_bOnlyCheck)
// 			{
// 				m_cashPremiumCardInfo.enCardState			= CASH_PREMIUM_CARD_STATE_UPDATING;
// 				m_cashPremiumCardInfo.nCardItemNum			= ITEM_NUM_CASH_SUPER_PREMIUM_CARD;
// 				m_cashPremiumCardInfo.atumTimeUpdatedTime.SetCurrentDateTime();
// 				m_cashPremiumCardInfo.atumTimeUpdatedTime.Second = 0;
// 
// 				QPARAM_CASH_UPDATE_PREMIUM_CARD *pQParam = new QPARAM_CASH_UPDATE_PREMIUM_CARD;
// 				util::zero(pQParam, sizeof(QPARAM_CASH_UPDATE_PREMIUM_CARD));
// 
// 				pQParam->nAccountUID			= m_cashPremiumCardInfo.nAccountUID10;
// 				pQParam->nItemNum				= m_cashPremiumCardInfo.nCardItemNum;
// 				pQParam->atumTimeCurrentTime	= m_cashPremiumCardInfo.atumTimeUpdatedTime;
// 				pQParam->atumTimeExpireTime		= m_cashPremiumCardInfo.atumTimeExpireTime;
// 				ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_CashUpdatePremiumCard, m_pFieldIOCPSocket
// 					, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
// 			}
// 		}
	}

	if(FALSE == i_bOnlyCheck)
	{
		///////////////////////////////////////////////////////////////////////////////
		// ���ʽ� ������ ����
		this->CashInsertBonusItemPremiumCard(i_pItem);

		///////////////////////////////////////////////////////////////////////////////
		// �����̾� ������ ���� ����
		INIT_MSG_WITH_BUFFER(MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO, T_FC_CHARACTER_CASH_PREMIUM_CARD_INFO, pSCardInfo, SendBuf);
		this->Make_MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO(pSCardInfo);
		m_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO));	
		INIT_MSG(MSG_FI_CASH_PREMIUM_CARD_INFO, T_FI_CASH_PREMIUM_CARD_INFO, pIMSCardInfo, SendBuf);// 2006-09-14 by dhjin, IM������ �ɹ��� �Ⱓ ������ �˸���
		this->Make_MSG_FI_CASH_PREMIUM_CARD_INFO(pIMSCardInfo);
		m_pFieldIOCPSocket->ms_pFieldIOCP->Send2IMServer(SendBuf, MSG_SIZE(MSG_FI_CASH_PREMIUM_CARD_INFO));
	}
	return ERR_NO_ERROR;
}


BOOL CFieldItemManager::CashOnEventPremiumCardFromDB(int i_queryType, UID32_t i_AccUID
													 , UID64_t i_n64CardUID, Err_t i_errNum/*=0*/)
{
	if(m_pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber != i_AccUID)
	{
		return FALSE;
	}

	mt_auto_lock at(&m_mapItemGeneral);
	
	if(ERR_NO_ERROR == i_errNum)
	{// ���� ����

		switch(i_queryType)
		{
		case QT_CashInsertPremiumCard:
		case QT_CashUpdatePremiumCard:
			{
				m_cashPremiumCardInfo.enCardState		= CASH_PREMIUM_CARD_STATE_NORMAL;
				m_cashPremiumCardInfo.n64UniqueNumber10	= i_n64CardUID;
#ifdef NEMERIAN_UPGRADED_MEMBERSHIP
				m_pFieldIOCPSocket->ChangeHP(m_pFieldIOCPSocket->GetCharacterTotalHP());
				m_pFieldIOCPSocket->ChangeDP(m_pFieldIOCPSocket->GetCharacterTotalDP());
				m_pFieldIOCPSocket->ChangeEP(m_pFieldIOCPSocket->GetCharacterTotalEP());
#endif
			}
			break;
		case QT_CashDeletePremiumCard:
			{
				m_cashPremiumCardInfo.ResetCASH_PREMEIUM_CARD_INFO();
#ifdef NEMERIAN_UPGRADED_MEMBERSHIP
				m_pFieldIOCPSocket->ChangeHP(m_pFieldIOCPSocket->GetCharacterTotalHP());
				m_pFieldIOCPSocket->ChangeDP(m_pFieldIOCPSocket->GetCharacterTotalDP());
				m_pFieldIOCPSocket->ChangeEP(m_pFieldIOCPSocket->GetCharacterTotalEP());
#endif
			}
			break;		
		}	
		
		m_cashPremiumCardInfo.ResetAllPlusRateByPremiumCard();			// 2006-10-11 by cmkwon
		return TRUE;
	}
	
	return TRUE;
}


BOOL CFieldItemManager::CashCheckPremiumCardExpire(BOOL i_bUnconditionalDeleteFlag/*=FALSE*/)
{
	mt_auto_lock at(&m_mapItemGeneral);
	if(CASH_PREMIUM_CARD_STATE_NORMAL != m_cashPremiumCardInfo.enCardState)
	{// ����ȭ �����̾� ī�� ���ų� �ٸ� ó����

		return FALSE;
	}
	
	if(FALSE == i_bUnconditionalDeleteFlag)
	{
		ATUM_DATE_TIME	tmAtumCurTime(true);
		if(tmAtumCurTime < m_cashPremiumCardInfo.atumTimeExpireTime)
		{// ��� �Ⱓ�� ���� ������

			return FALSE;
		}
	}
	
	m_cashPremiumCardInfo.enCardState			= CASH_PREMIUM_CARD_STATE_DELETING;
	m_cashPremiumCardInfo.nCardItemNum			= 0;
	
	///////////////////////////////////////////////////////////////////////////////
	// DB���� ����
	QPARAM_CASH_DELETE_PREMIUM_CARD *pQParam = new QPARAM_CASH_DELETE_PREMIUM_CARD;
	util::zero(pQParam, sizeof(QPARAM_CASH_DELETE_PREMIUM_CARD));
	pQParam->nAccountUID			= m_cashPremiumCardInfo.nAccountUID10;
	ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_CashDeletePremiumCard, m_pFieldIOCPSocket
		, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);

	///////////////////////////////////////////////////////////////////////////////
	// Ŭ���̾�Ʈ�� ���� ����
	INIT_MSG_WITH_BUFFER(MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO, T_FC_CHARACTER_CASH_PREMIUM_CARD_INFO, pSCardInfo, SendBuf);
	this->Make_MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO(pSCardInfo);
	m_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO));
	
	INIT_MSG(MSG_FI_CASH_PREMIUM_CARD_INFO, T_FI_CASH_PREMIUM_CARD_INFO, pIMSCardInfo, SendBuf);// 2006-09-14 by dhjin, IM������ �ɹ��� �Ⱓ ������ �˸���
	this->Make_MSG_FI_CASH_PREMIUM_CARD_INFO(pIMSCardInfo);
	m_pFieldIOCPSocket->ms_pFieldIOCP->Send2IMServer(SendBuf, MSG_SIZE(MSG_FI_CASH_PREMIUM_CARD_INFO));
	return TRUE;
}

void CFieldItemManager::CashInsertBonusItemPremiumCard(ITEM *i_pItemInfo)
{
	mt_auto_lock at(&m_mapItemGeneral);
	
	for(int i=0; i < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; i++)
	{
		if(0.0f < i_pItemInfo->ArrParameterValue[i]
			&& 0 < i_pItemInfo->ArrDestParameter[i])
		{
			ITEM *pLinkItemInfo = ms_pFieldIOCP2->GetItemInfo(i_pItemInfo->ArrParameterValue[i]);
			if (pLinkItemInfo)
			{
				this->InsertItemBaseByItemNum(pLinkItemInfo->ItemNum, i_pItemInfo->ArrDestParameter[i], IUT_SHOP);
			}
		}	
	}
}


void CFieldItemManager::Make_MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO(MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO *o_pCardInfo)
{
	o_pCardInfo->nCardItemNum1			= m_cashPremiumCardInfo.nCardItemNum;	
	o_pCardInfo->atumTimeUpdatedTime1	= m_cashPremiumCardInfo.atumTimeUpdatedTime;
	o_pCardInfo->atumTimeExpireTime1	= m_cashPremiumCardInfo.atumTimeExpireTime;
	o_pCardInfo->fExpRate1				= m_cashPremiumCardInfo.fExpRate;
// 2005-03-23 by cmkwon, �������ݷδ� ���� �������� ���� ��(�ʿ�� �߰�)
//	o_pCardInfo->fSPIRate1				= m_cashPremiumCardInfo.fSPIRate;
	o_pCardInfo->fDropRate1				= m_cashPremiumCardInfo.fDropRate;
	o_pCardInfo->fDropRareRate1			= m_cashPremiumCardInfo.fDropRareRate;
	o_pCardInfo->fExpRepairRate1		= m_cashPremiumCardInfo.fExpRepairRate;
}

void CFieldItemManager::Make_MSG_FI_CASH_PREMIUM_CARD_INFO(MSG_FI_CASH_PREMIUM_CARD_INFO *o_pCardInfo)
{
	o_pCardInfo->AccountUID				= m_pFieldIOCPSocket->m_character.AccountUniqueNumber;
	o_pCardInfo->nCardItemNum1			= m_cashPremiumCardInfo.nCardItemNum;	
	o_pCardInfo->atumTimeExpireTime1	= m_cashPremiumCardInfo.atumTimeExpireTime;	// 2008-06-20 by dhjin, EP3 ���� ���� ���� - ���� �ð� �ʿ�
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldItemManager::AllDetachTimeLimitItem()
/// \brief		�Ʒ��� ���� - �������� �ð����� �������� ���´�.
/// \author		dhjin
/// \date		2008-03-03 ~ 2008-03-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::AllDetachTimeLimitItem()
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);

	vectITEM_GENERALPtr::iterator itr = m_vectUsingGeneralItem.begin();
	while (itr != m_vectUsingGeneralItem.end())
	{
		ITEM_GENERAL *pUsingItemG = *itr;

		if(ITEMKIND_ACCESSORY_TIMELIMIT == pUsingItemG->ItemInfo->Kind
			&& WEAR_ATTACHED == pUsingItemG->Wear)
		{
			int nTmpWindowIndex		= pUsingItemG->ItemWindowIndex;	// ���� ��ġ

			// 2008-03-03 by dhjin, ���� ���� ó����
			m_pFieldIOCPSocket->UpdateWearItemPointer(pUsingItemG->UniqueNumber, pUsingItemG->UniqueNumber, pUsingItemG->ItemWindowIndex, POS_ITEMWINDOW_OFFSET + nTmpWindowIndex);
			
// 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - �Ʒ��� ���� ����
// 			m_pFieldIOCPSocket->UnsetParamFactor(pUsingItemG->ItemInfo->DestParameter1, pUsingItemG->ItemInfo->ParameterValue1);
// 			m_pFieldIOCPSocket->UnsetParamFactor(pUsingItemG->ItemInfo->DestParameter2, pUsingItemG->ItemInfo->ParameterValue2);
// 			m_pFieldIOCPSocket->UnsetParamFactor(pUsingItemG->ItemInfo->DestParameter3, pUsingItemG->ItemInfo->ParameterValue3);
// 			m_pFieldIOCPSocket->UnsetParamFactor(pUsingItemG->ItemInfo->DestParameter4, pUsingItemG->ItemInfo->ParameterValue4);
			for(int i=0; i < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; i++)
			{
				m_pFieldIOCPSocket->UnsetParamFactor(pUsingItemG->ItemInfo->ArrDestParameter[i], pUsingItemG->ItemInfo->ArrParameterValue[i]);
			}

			ATUM_DATE_TIME atimeCur { true };
			pUsingItemG->UsingTimeStamp	= pUsingItemG->UsingTimeStamp + atimeCur.GetTimeDiffTimeInSeconds(pUsingItemG->UsingStartTime);
			pUsingItemG->UsingStartTime	= atimeCur;
			
			QPARAM_UPDATE_ITEM_USINGTIMESTAMP *pQParam = new QPARAM_UPDATE_ITEM_USINGTIMESTAMP;
			pQParam->ItemUniqueNumber	= pUsingItemG->UniqueNumber;
			pQParam->UsingTimeStamp1	= pUsingItemG->UsingTimeStamp;
			ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemUsingTimeStamp, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);

			// 2008-01-23 by cmkwon, S_F, S_L: ����/�������� ���� �α׿� �߰� - ����� �ð����ѾǼ��縮 �����۸� �α׿� �����
			CAtumLogSender::SendLogMessageITEMDetachItem(m_pFieldIOCPSocket, pUsingItemG);

			pUsingItemG->Wear				= WEAR_NOT_ATTACHED;
			pUsingItemG->ItemWindowIndex	= POS_ITEMWINDOW_OFFSET + nTmpWindowIndex;
	
			QPARAM_CHANGE_ITEM_WINDOW_POSITION *pQParamChangeWindowPosition	= new QPARAM_CHANGE_ITEM_WINDOW_POSITION;
			pQParamChangeWindowPosition->AccountUID			= m_pFieldIOCPSocket->m_character.AccountUniqueNumber;
			pQParamChangeWindowPosition->CharacterUID		= m_pFieldIOCPSocket->m_character.CharacterUniqueNumber;
			pQParamChangeWindowPosition->ItemUID1			= pUsingItemG->UniqueNumber;
			pQParamChangeWindowPosition->ItemWear1			= pUsingItemG->Wear;
			pQParamChangeWindowPosition->ItemWindowIndex1	= pUsingItemG->ItemWindowIndex;
			pQParamChangeWindowPosition->ItemUID2			= 0;
			pQParamChangeWindowPosition->ItemWindowIndex2	= POS_INVALID_POSITION;
			pQParamChangeWindowPosition->ItemWear2			= WEAR_NOT_ATTACHED;
			ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_ChangeItemPosition, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParamChangeWindowPosition);

//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENACHECK] AllDetachTimeLimitItem ItemUniqueNumber = %d\r\n"
//				, pUsingItemG->UniqueNumber);			
			itr = m_vectUsingGeneralItem.erase(itr);
			continue;
		}
		
		itr++;		
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldItemManager::ItemInsertByInfinityPlaying(ITEM_GENERAL *i_pItemGeneral)
/// \brief		���Ǵ�Ƽ - ���� ���� �� ������ ����
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::ItemInsertByInfinityPlaying(ITEM_GENERAL *i_pItemGeneral)
{
	if(FALSE == g_pFieldGlobal->IsArenaServer()
		|| NULL == i_pItemGeneral) {
		return;
	}

	mt_auto_lock mtA(&m_mtInfinityInsertItemInfoList);

	INFINITY_INSERT_ITEM_INFO InsertItemInfo;
	util::zero(&InsertItemInfo, sizeof(INFINITY_INSERT_ITEM_INFO));
	InsertItemInfo.ItemUID				= i_pItemGeneral->UniqueNumber;
	InsertItemInfo.ItemNum				= i_pItemGeneral->ItemNum;
	InsertItemInfo.CurrentCount			= i_pItemGeneral->CurrentCount;
	InsertItemInfo.ItemWindowIndex		= i_pItemGeneral->ItemWindowIndex;
	InsertItemInfo.PrefixCodeNum		= i_pItemGeneral->PrefixCodeNum;
	InsertItemInfo.SuffixCodeNum		= i_pItemGeneral->SuffixCodeNum;
	InsertItemInfo.Wear					= i_pItemGeneral->Wear;
	InsertItemInfo.UsingTimeStamp		= i_pItemGeneral->UsingTimeStamp;
	InsertItemInfo.CreatedTime			= i_pItemGeneral->CreatedTime;
	InsertItemInfo.CoolingTimeStamp		= i_pItemGeneral->CoolingTimeStamp;	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������

	m_mtInfinityInsertItemInfoList.push_back(InsertItemInfo);

//	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CFieldItemManager::ItemInsertByInfinityPlaying# Inserted Item!, %s %s \r\n"
//		, m_pFieldIOCPSocket->m_InfinityPlayingInfo.InfinityCreateUID, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), GetItemGeneralString(i_pItemGeneral, string()));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldItemManager::ItemUpdateByInfinityPlaying(ITEM_GENERAL *i_pItemGeneral)
/// \brief		���Ǵ�Ƽ - ���� ���� �� ������ ����
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::ItemUpdateByInfinityPlaying(ITEM_GENERAL *i_pItemGeneral) {
	if(FALSE == g_pFieldGlobal->IsArenaServer()
		|| NULL == i_pItemGeneral)
	{
		return;
	}

	{
		// ���� ���� �� ������ ��ǰ�� �ִ��� üũ�ϰ� �ִٸ� ������ ��ǰ ���� ����, ���ǲ� ���ǰ� �˾Ƽ� ó��
		mt_auto_lock mta(&m_mtInfinityInsertItemInfoList);
		INFINITY_INSERT_ITEM_INFO *pInsertItemInfo = this->InfinityFindItemInfoFromInsertItemListNoLock(i_pItemGeneral->UniqueNumber);
		if(pInsertItemInfo)
		{
			pInsertItemInfo->CurrentCount		= i_pItemGeneral->CurrentCount;
			pInsertItemInfo->ItemWindowIndex	= i_pItemGeneral->ItemWindowIndex;
			pInsertItemInfo->PrefixCodeNum		= i_pItemGeneral->PrefixCodeNum;
			pInsertItemInfo->SuffixCodeNum		= i_pItemGeneral->SuffixCodeNum;
			pInsertItemInfo->Wear				= i_pItemGeneral->Wear;
			pInsertItemInfo->UsingTimeStamp		= i_pItemGeneral->UsingTimeStamp;
			pInsertItemInfo->CoolingTimeStamp	= i_pItemGeneral->CoolingTimeStamp;	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
			
//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CFieldItemManager::ItemUpdateByInfinityPlaying# Update Item from InsertList, %s %s, Wear = %d, UsingTimeStamp(%d) CoolingTimeStamp(%d) MFS(%I64d) \r\n"
//				, m_pFieldIOCPSocket->m_InfinityPlayingInfo.InfinityCreateUID, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), GetItemGeneralString(i_pItemGeneral, string()), i_pItemGeneral->Wear, i_pItemGeneral->UsingTimeStamp, i_pItemGeneral->CoolingTimeStamp, i_pItemGeneral->MainSvrItemUID);
			return;
		}
	}

	{
		mt_auto_lock mtu(&m_mtInfinityUpdateItemInfoList);
		INFINITY_UPDATE_ITEM_INFO *pUpdateItemInfo = this->InfinityFindItemInfoFromUpdateItemListNoLock(i_pItemGeneral->UniqueNumber);
		if(pUpdateItemInfo)
		{
			pUpdateItemInfo->CurrentCount		= i_pItemGeneral->CurrentCount;
			pUpdateItemInfo->ItemWindowIndex	= i_pItemGeneral->ItemWindowIndex;
			pUpdateItemInfo->Wear				= i_pItemGeneral->Wear;
			pUpdateItemInfo->UsingTimeStamp		= i_pItemGeneral->UsingTimeStamp;
			pUpdateItemInfo->CoolingTimeStamp	= i_pItemGeneral->CoolingTimeStamp;	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������

//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CFieldItemManager::ItemUpdateByInfinityPlaying# Update Item from UpdateList, %s %s, Wear = %d, UsingTimeStamp(%d) CoolingTimeStamp(%d) MFS(%I64d) \r\n"
//				, m_pFieldIOCPSocket->m_InfinityPlayingInfo.InfinityCreateUID, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), GetItemGeneralString(i_pItemGeneral, string()), i_pItemGeneral->Wear, i_pItemGeneral->UsingTimeStamp, i_pItemGeneral->CoolingTimeStamp, i_pItemGeneral->MainSvrItemUID);
			return;
		}

		INFINITY_UPDATE_ITEM_INFO UpdateItemInfo;
		util::zero(&UpdateItemInfo, sizeof(INFINITY_UPDATE_ITEM_INFO));
		UpdateItemInfo.ItemUID				= i_pItemGeneral->UniqueNumber;
		UpdateItemInfo.CurrentCount			= i_pItemGeneral->CurrentCount;
		UpdateItemInfo.ItemWindowIndex		= i_pItemGeneral->ItemWindowIndex;
		UpdateItemInfo.Wear					= i_pItemGeneral->Wear;
		UpdateItemInfo.MainSvrItemUID		= i_pItemGeneral->MainSvrItemUID;
		UpdateItemInfo.UsingTimeStamp		= i_pItemGeneral->UsingTimeStamp;
		UpdateItemInfo.CoolingTimeStamp		= i_pItemGeneral->CoolingTimeStamp;	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
		m_mtInfinityUpdateItemInfoList.push_back(UpdateItemInfo);

//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CFieldItemManager::ItemUpdateByInfinityPlaying# Insert Item to UpdateList, %s %s, Wear = %d, UsingTimeStamp(%d) CoolingTimeStamp(%d) MFS(%I64d) \r\n"
//			, m_pFieldIOCPSocket->m_InfinityPlayingInfo.InfinityCreateUID, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), GetItemGeneralString(i_pItemGeneral, string()), i_pItemGeneral->Wear, i_pItemGeneral->UsingTimeStamp, i_pItemGeneral->CoolingTimeStamp, i_pItemGeneral->MainSvrItemUID);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldItemManager::ItemDeleteByInfinityPlaying(ITEM_GENERAL *i_pItemGeneral)
/// \brief		���Ǵ�Ƽ - ���� ���� �� ������ ����
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::ItemDeleteByInfinityPlaying(ITEM_GENERAL *i_pItemGeneral) {
	if(FALSE == g_pFieldGlobal->IsArenaServer()
		|| NULL == i_pItemGeneral) {
		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-02 by cmkwon, ����2�� �߰� ���� - 
	if(this->InfinityDeleteItemInfoFromInsertItemList(i_pItemGeneral->UniqueNumber))
	{
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CFieldItemManager::ItemDeleteByInfinityPlaying# deleted from InsertList !, %s %s \r\n"
//			, m_pFieldIOCPSocket->m_InfinityPlayingInfo.InfinityCreateUID, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), GetItemGeneralString(i_pItemGeneral, string()));
		return;
	}

	// 2010-07-16 by jskim ���Ǵ�Ƽ ����� ������ ������� �����Ǵ� ���� ���� 
	/*
	if(this->InfinityDeleteItemInfoFromUpdateItemList(i_pItemGeneral->UniqueNumber))
	{		
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CFieldItemManager::ItemDeleteByInfinityPlaying# deleted from UpdateList !, %s %s \r\n"
//			, m_pFieldIOCPSocket->m_InfinityPlayingInfo.InfinityCreateUID, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), GetItemGeneralString(i_pItemGeneral, string()));
		return;
	}
	*/
	this->InfinityDeleteItemInfoFromUpdateItemList(i_pItemGeneral->UniqueNumber);

	if(0 == i_pItemGeneral->MainSvrItemUID)
	{
		// 2009-09-09 ~ 2010-01-27 by dhjin, ���Ǵ�Ƽ - MainServerUID = 0 �̸� ���� �������� �ƴϴ�. �̰� �� �� �׸񿡼� �ɷ����� �ʴ��� üũ�ؾ���!!!�ӽù����ڵ�
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] [Infinity][%I64d] CFieldItemManager::ItemDeleteByInfinityPlaying# %s %s MainSvrUID = 0 \r\n"
			, m_pFieldIOCPSocket->m_InfinityPlayingInfo.InfinityCreateUID, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), GetItemGeneralString(i_pItemGeneral, string()));
		return;
	}

	BOOL bReted = this->InfinityInsertItemInfoFromDeleteItemList(i_pItemGeneral->MainSvrItemUID);
//	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CFieldItemManager::ItemDeleteByInfinityPlaying# inserted ItemList to delete item On MFS !, %s %s bReted(%d) \r\n"
//		, m_pFieldIOCPSocket->m_InfinityPlayingInfo.InfinityCreateUID, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), GetItemGeneralString(i_pItemGeneral, string()), bReted);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldItemManager::UpdateMainSvrUsingTimeLimitedItemByInfinity(MSG_FtoA_INFINITY_USING_TIME_LIMIT_ITEM *i_pItemInfo)
/// \brief		���Ǵ�Ƽ - ���ǿ��� ������ �ð��� ������ ����!
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::UpdateMainSvrUsingTimeLimitedItemByInfinity(MSG_FtoA_INFINITY_USING_TIME_LIMIT_ITEM *i_pItemInfo)	{
	if(NULL == i_pItemInfo) {
		return FALSE;
	}

	mt_auto_lock igLock(&m_mapItemGeneral);
	for(int i=0; i < m_vectUsingGeneralItem.size(); i++) {
		if(i_pItemInfo->MainSvrItemUID == m_vectUsingGeneralItem[i]->UniqueNumber) {
			// �����ϸ� ����!
			m_vectUsingGeneralItem[i]->UsingTimeStamp	= i_pItemInfo->UsingTimeStamp;
			m_vectUsingGeneralItem[i]->Wear				= i_pItemInfo->Wear;
			return TRUE;
		}
	}

	return FALSE;
} 

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldItemManager::DeleteUsingTimeLimitedItemByInfinity(UID64_t i_MainSvrItemUID)
/// \brief		���Ǵ�Ƽ - ���ǿ��� ������ �ð��� ������ ó��
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::DeleteUsingTimeLimitedItemByInfinity(UID64_t i_MainSvrItemUID) {

	mt_auto_lock igLock(&m_mapItemGeneral);

	ATUM_DATE_TIME atimeCur { true };
	vectITEM_GENERALPtr::iterator itr = m_vectUsingGeneralItem.begin();
	while (itr != m_vectUsingGeneralItem.end())
	{
		ITEM_GENERAL *pItemGen = *itr;
		if(IS_VALID_UID64(i_MainSvrItemUID)
			&& pItemGen->UniqueNumber != i_MainSvrItemUID)
		{// 2006-10-11 by cmkwon, �ϳ��� �����۸� üũ�Ѵ�
			itr++;
			continue;
		}

		int nCurUsingSec	= pItemGen->UsingTimeStamp + atimeCur.GetTimeDiffTimeInSeconds(pItemGen->UsingStartTime);
		int temp1 = pItemGen->UsingTimeStamp;
		int temp2 = atimeCur.GetTimeDiffTimeInSeconds(pItemGen->UsingStartTime);

		// if(pItemGen->ItemInfo->Kind == ITEMKIND_ACCESSORY_TIMELIMIT)
		if(WEAR_ATTACHED == pItemGen->Wear)
		{
			MSG_FC_ITEM_CHANGE_WINDOW_POSITION tmItemChangePos;
			util::zero(&tmItemChangePos, sizeof(MSG_FC_ITEM_CHANGE_WINDOW_POSITION));
			tmItemChangePos.CharacterUniqueNumber	= m_pFieldIOCPSocket->m_character.CharacterUniqueNumber;
			tmItemChangePos.FromItemUniqueNumber	= pItemGen->UniqueNumber;
			tmItemChangePos.FromItemWindowIndex		= pItemGen->ItemWindowIndex;
			tmItemChangePos.ToItemUniqueNumber		= 0;
			tmItemChangePos.ToItemWindowIndex		= POS_ITEMWINDOW_OFFSET;
			m_pFieldIOCPSocket->ProcessItemChangeWindowPosition(&tmItemChangePos, FALSE, FALSE);

			// 2008-01-23 by cmkwon, S_F, S_L: ����/�������� ���� �α׿� �߰� - 
			CAtumLogSender::SendLogMessageITEMDelete(m_pFieldIOCPSocket, pItemGen, IDT_EXPIRE_TIME, nCurUsingSec);

			this->DeleteItemFromCharacterByPointer(pItemGen, IUT_EXPIRE_CARD_ITEM);
		}
		else
		{
			// 2008-01-23 by cmkwon, S_F, S_L: ����/�������� ���� �α׿� �߰� - 
			CAtumLogSender::SendLogMessageITEMDelete(m_pFieldIOCPSocket, pItemGen, IDT_EXPIRE_TIME, nCurUsingSec);
			///////////////////////////////////////////////////////////////////////////////
			// 1. ������ ��� ����
			m_pFieldIOCPSocket->ReleaseCardItem(pItemGen->ItemInfo);

			///////////////////////////////////////////////////////////////////////////////
			// 3. DB���� ������ ����			
			QPARAM_DELETESTOREITEM *pQParam	= new QPARAM_DELETESTOREITEM;
			pQParam->AccountUniqueNumber	= m_pFieldIOCPSocket->m_character.AccountUniqueNumber;
			pQParam->ItemUniqueNumber		= pItemGen->UniqueNumber;
			ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_DeleteStoreItem, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
			
			///////////////////////////////////////////////////////////////////////////////
			// 4. ITEM_GENERAL�� �޸� ����
			util::del(pItemGen);
		}

		itr = m_vectUsingGeneralItem.erase(itr);
	
		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ, �ذ� ���� ����
//		if(IS_VALID_UID64(i_MainSvrItemUID))
//		{// 2006-10-11 by cmkwon, ������ �ϳ��� üũ
//			return TRUE;
//		}
// 2010-01-21 by cmkwon, ���� ���� �Ǿ�� ��.
//		itr++;
		return TRUE;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2006-09-29 by cmkwon, ������ ���� �� �ڵ� ���� ������ ���� üũ
	mtmapUID2ItemGeneral::iterator itrG(m_mapItemGeneral.begin());
	while(itrG != m_mapItemGeneral.end())
	{
		ITEM_GENERAL *pItemGen = itrG->second;		
		if(IS_VALID_UID64(i_MainSvrItemUID)
			&& pItemGen->UniqueNumber != i_MainSvrItemUID)
		{// 2006-10-11 by cmkwon, �ϳ��� �����۸� üũ�Ѵ�

			itrG++;
			continue;
		}

		if(COMPARE_BIT_FLAG(pItemGen->ItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE))
		{
			if(IS_ATTACHABLE_ITEM(pItemGen->ItemInfo))
			{// 2009-10-22 by cmkwon, ���� ���� �����۷�
				if(WEAR_ATTACHED == pItemGen->Wear)	{
				///////////////////////////////////////////////////////////////////////////////
				// 2007-09-07 by cmkwon, �Ⱓ ����� ������ ������ ���� ���� ó�� - ��ũ �������� �Ⱓ ���� �����ۿ� �߰���
				MSG_FC_ITEM_CHANGE_WINDOW_POSITION tmItemChangePos;
				util::zero(&tmItemChangePos, sizeof(MSG_FC_ITEM_CHANGE_WINDOW_POSITION));
				tmItemChangePos.CharacterUniqueNumber	= m_pFieldIOCPSocket->m_character.CharacterUniqueNumber;
				tmItemChangePos.FromItemUniqueNumber	= pItemGen->UniqueNumber;
				tmItemChangePos.FromItemWindowIndex		= pItemGen->ItemWindowIndex;
				tmItemChangePos.ToItemUniqueNumber		= 0;
				tmItemChangePos.ToItemWindowIndex		= POS_ITEMWINDOW_OFFSET;
				m_pFieldIOCPSocket->ProcessItemChangeWindowPosition(&tmItemChangePos, FALSE, FALSE);
				}
			}

			// 2008-01-23 by cmkwon, S_F, S_L: ����/�������� ���� �α׿� �߰� - �Ʒ��� ���� CAtumLogSender::SendLogMessageITEMDelete() �� ��ü ��
			// 2012-08-30 by jhseol, Ǯ�α� �����
#ifdef S_FULL_LOG_JHSEOL
			CAtumLogSender::SendLogMessageITEMUseITEM(m_pFieldIOCPSocket, pItemGen);						// ��� ������ �α� �߰�
#endif	//#ifdef S_FULL_LOG_JHSEOL
			// end 2012-08-30 by jhseol, Ǯ�α� �����
			// 2008-01-23 by cmkwon, S_F, S_L: ����/�������� ���� �α׿� �߰� - �Ⱓ ����� ������
			CAtumLogSender::SendLogMessageITEMDelete(m_pFieldIOCPSocket, pItemGen, IDT_EXPIRE_TIME);

			// 1. m_mapItemGeneral���� ����
			itrG = m_mapItemGeneral.erase(itrG);

			// 2. DB���� ������ ����
			QPARAM_DELETESTOREITEM *pQParam = new QPARAM_DELETESTOREITEM;
			pQParam->AccountUniqueNumber	= m_pFieldIOCPSocket->m_character.AccountUniqueNumber;
			pQParam->ItemUniqueNumber		= pItemGen->UniqueNumber;
			ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_DeleteStoreItem, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
		
			util::del(pItemGen);

			// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ, �ذ� ���� ����
//			if(IS_VALID_UID64(i_MainSvrItemUID))
//			{// 2006-10-11 by cmkwon, ������ �ϳ��� üũ
//				return TRUE;
//			}
			return TRUE;

		}
		itrG++;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldItemManager::LogInfinityUsingItem()
/// \brief		���Ǵ�Ƽ - ���� ���ӷα� �߰�
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::LogInfinityUsingItem() {
	mt_auto_lock igLock(&m_mapItemGeneral);
	vectITEM_GENERALPtr::iterator itr = m_vectUsingGeneralItem.begin();
	for(; itr != m_vectUsingGeneralItem.end(); itr++) {
		CAtumLogSender::SendLogMessageInfinityFinItem(m_pFieldIOCPSocket, *itr);
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-01-26 by cmkwon, ĳ�� ������ �����Ǹ� �ý��� ���� - 
/// \author		cmkwon
/// \date		2010-01-26 ~ 2010-01-26
/// \warning	
///
/// \param		
/// \return		FALSE	==> ���� �Ұ�( ���� or ERROR)
///				TRUE	==> ���� ����
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::CheckBuyLimitedEditionShopItem(INT *o_pRemainCount, INT i_nItemNum)
{
	CODBCStatement mainDBOdbcStmt;
	BOOL bRet = mainDBOdbcStmt.Init(g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (char*)g_pGlobal->GetODBCUID(), (char*)g_pGlobal->GetODBCPASSWORD(), g_pGlobal->GetMainWndHandle());
	if (FALSE == bRet)
	{		
		g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] fail to connect DBServer(%s:%d %s %s %s) !! CFieldItemManager::CheckBuyLimitedEditionShopItem# %s %d \r\n"
			, g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (char*)g_pGlobal->GetODBCUID(), (char*)g_pGlobal->GetODBCPASSWORD()
			, GetCharacterString(m_pCharacter, string()), i_nItemNum);
		return FALSE;
	}

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_check_Buy_LimitedEdtion_shopItem
	-- DESC				: // 2010-01-26 by cmkwon, ĳ�� ������ �����Ǹ� �ý��� ���� - 
	--					
	-- Result set		: [ErrorCode] [RemainCountForLimitedEdition]
	--------------------------------------------------------------------------------
	CALL dbo.atum_check_Buy_LimitedEdtion_shopItem
	**************************************************************************/
	SQLHSTMT mDBstmt = mainDBOdbcStmt.GetSTMTHandle();
	SQLINTEGER arrCB[3] = {SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(mDBstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0,	&i_nItemNum, 0, &arrCB[1]);
	
	RETCODE ret = mainDBOdbcStmt.ExecuteQuery(PROCEDURE_100127_0545);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] fail to ExecuteQuery !! PROCEDURE_100127_0545 %s %d \r\n", GetCharacterString(m_pCharacter, string()), i_nItemNum);
		mainDBOdbcStmt.ProcessLogMessages(SQL_HANDLE_STMT, mDBstmt,"@PROCEDURE_100127_0545 1 Failed!\r\n", TRUE);
		mainDBOdbcStmt.FreeStatement();
		return FALSE;
	}

	int nErrCode = 1;					// 2010-01-27 by cmkwon, �⺻�� Error
	arrCB[1] = arrCB[2] = SQL_NTS;
	SQLBindCol(mDBstmt, 1, SQL_C_LONG, &nErrCode,	0,			&arrCB[1]);
	SQLBindCol(mDBstmt, 2, SQL_C_LONG, o_pRemainCount,	0,		&arrCB[2]);
	
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(mDBstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 �̸� ����� RowCount �̴� 
			ret = SQLMoreResults(mDBstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(mDBstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(mDBstmt);                 // 2008-07-25 by cmkwon, ���� Results �� �ִ����� �ѹ��� üũ�Ѵ�.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}
	mainDBOdbcStmt.FreeStatement();	
	if(ERR_NO_ERROR != nErrCode)
	{
		g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] fail to Fetch !! PROCEDURE_100127_0545 %s %d ErrCode(%d) %d \r\n"
			, GetCharacterString(m_pCharacter, string()), i_nItemNum, nErrCode, *o_pRemainCount);
		return FALSE;
	}

	// 2010-02-02 by cmkwon, ����� ������ ���� ����
	ms_pFieldIOCP2->LimitedEUpdateItemCount(i_nItemNum, *o_pRemainCount);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldItemManager::AddInvokingItemList(ItemUID_t i_ItemUID)
/// \brief		���Ǵ�Ƽ - �ߵ�������������, ��ߵ� ���� �� �߰�
/// \author		dhjin
/// \date		2009-09-09 ~ 2010-02-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::AddInvokingItemList(ItemUID_t i_ItemUID)
{
	mt_auto_lock igLock(&m_mapItemGeneral);
	ITEM_GENERAL *pItemG = this->GetItemGeneralByUID(i_ItemUID);
	if(NULL == pItemG || 0 == pItemG->ItemInfo->ReAttacktime)
	{
		return FALSE;
	}

	if(NULL != this->InvokingFindItemGNoLock(i_ItemUID))
	{// 2010-03-04 by cmkwon, �̹� ����Ʈ�� ����

		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldItemManager::AddInvokingItemList# %s %s %d %s\r\n"
			, GetCharacterString(m_pCharacter, string()), GetItemString(pItemG, string()), pItemG->CoolingTimeStamp, pItemG->CoolingStartTime.GetDateTimeString().GetBuffer());
		return FALSE;
	}
	
	pItemG->CoolingStartTime	= ATUM_DATE_TIME { true };	// 2006-03-30 by cmkwon, ������ ��� ���۽ð����� ����, CoolingTimeStamp�� �ʱ�ȭ �ϸ� �ȵ�
	m_InvokingItemList.push_back(pItemG);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldItemManager::CheckCoolingTimeInvokingItem(ItemUID_t i_ItemUID)
/// \brief		���Ǵ�Ƽ - �ߵ�������������, ��Ÿ���� ���� �ִ��� üũ
/// \author		dhjin
/// \date		2009-09-09 ~ 2010-02-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::CheckCoolingTimeInvokingItem(ItemUID_t i_ItemUID, BOOL i_bSendElapsedSec/*=FALSE*/)
{
	mt_auto_lock igLock(&m_mapItemGeneral);
	ITEM_GENERAL *pItemG = this->InvokingFindItemGNoLock(i_ItemUID);
	if(NULL == pItemG)
	{
		return FALSE;
	}

	ATUM_DATE_TIME atimeCur { true };
	int nElapsedSec	= pItemG->CoolingTimeStamp + atimeCur.GetTimeDiffTimeInSeconds(pItemG->CoolingStartTime);	
	if(nElapsedSec < pItemG->ItemInfo->ReAttacktime/1000)
	{
		if(i_bSendElapsedSec)
		{
			// �������� ����
			INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_GET_COOLINGTIME_INFO_OK, T_FC_ITEM_GET_COOLINGTIME_INFO_OK, pSMsg, SendBuf);
			pSMsg->ItemUID			= pItemG->UniqueNumber;
			pSMsg->CoolingTimeStamp	= nElapsedSec;
			m_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ITEM_GET_COOLINGTIME_INFO_OK));
		}
		return TRUE;
	}
	this->InvokingDeleteItemGNoLock(i_ItemUID);		// ��Ÿ�Ӹ���Ʈ���� ����

	pItemG->CoolingTimeStamp	= 0;				// 

	//�� Ÿ�� �Ϸ�!!
	QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP *pQParam = new QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP;
	pQParam->ItemUniqueNumber	= pItemG->UniqueNumber;
	pQParam->CoolingTimeStamp	= pItemG->CoolingTimeStamp;
	ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemCoolingTimeStamp, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
	
	// �������� ����
	INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_END_COOLINGTIME_ITEM_OK, T_FC_ITEM_END_COOLINGTIME_ITEM_OK, pMsg, pBuf);
	pMsg->ItemUID	= i_ItemUID;
	m_pFieldIOCPSocket->SendAddData(pBuf, MSG_SIZE(MSG_FC_ITEM_END_COOLINGTIME_ITEM_OK));
	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldItemManager::StopInvokingItemList(ITEM_GENERAL *i_pItemGen)
/// \brief		���Ǵ�Ƽ - �ߵ�������������, ��Ÿ���� ����
/// \author		dhjin
/// \date		2009-09-09 ~ 2010-02-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::StopInvokingItemList(ITEM_GENERAL *i_pItemGen) {
	if(NULL == i_pItemGen) {
		return;
	}

	mt_auto_lock igLock(&m_mapItemGeneral);
	
	if(NULL == this->InvokingFindItemGNoLock(i_pItemGen->UniqueNumber))
	{
		return;
	}

	this->InvokingDeleteItemGNoLock(i_pItemGen->UniqueNumber);		// ��Ÿ�Ӹ���Ʈ���� ����
	
	ATUM_DATE_TIME atimeCur { true };
	int nElapsedSec	= i_pItemGen->CoolingTimeStamp + atimeCur.GetTimeDiffTimeInSeconds(i_pItemGen->CoolingStartTime);	
	if(nElapsedSec < i_pItemGen->ItemInfo->ReAttacktime/1000)
	{		
		i_pItemGen->CoolingTimeStamp	= nElapsedSec;
		i_pItemGen->CoolingStartTime	= atimeCur;

		// �������� ����
		INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_GET_COOLINGTIME_INFO_OK, T_FC_ITEM_GET_COOLINGTIME_INFO_OK, pSMsg, SendBuf);
		pSMsg->ItemUID			= i_pItemGen->UniqueNumber;
		pSMsg->CoolingTimeStamp	= nElapsedSec;
		m_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ITEM_GET_COOLINGTIME_INFO_OK));
			
		QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP *pQParam = new QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP;
		pQParam->ItemUniqueNumber	= i_pItemGen->UniqueNumber;
		pQParam->CoolingTimeStamp	= nElapsedSec;
		ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemCoolingTimeStamp, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);		
	}
	else
	{
		i_pItemGen->CoolingTimeStamp = 0;

		//�� Ÿ�� �Ϸ�!!
		QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP *pQParam = new QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP;
		pQParam->ItemUniqueNumber	= i_pItemGen->UniqueNumber;
		pQParam->CoolingTimeStamp	= i_pItemGen->CoolingTimeStamp;
		ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemCoolingTimeStamp, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
		
		// �������� ����
		INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_END_COOLINGTIME_ITEM_OK, T_FC_ITEM_END_COOLINGTIME_ITEM_OK, pMsg, pBuf);
		pMsg->ItemUID	= i_pItemGen->UniqueNumber;
		m_pFieldIOCPSocket->SendAddData(pBuf, MSG_SIZE(MSG_FC_ITEM_END_COOLINGTIME_ITEM_OK));
	}	
	ITEM_GENERAL tmItemG = *i_pItemGen;
	igLock.auto_unlock_cancel();

	this->ItemUpdateByInfinityPlaying(&tmItemG);// ���Ǵ�Ƽ ����ȭ
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldItemManager::SaveInvokingItemList()
/// \brief		���Ǵ�Ƽ - �ߵ�������������, ��Ÿ�� ����
/// \author		dhjin
/// \date		2009-09-09 ~ 2010-02-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::SaveInvokingItemList(BOOL i_bSaveCoolingItem/*=TRUE*/)
 {
	mt_auto_lock igLock(&m_mapItemGeneral);
	vectITEM_GENERALPtr::iterator itr = m_InvokingItemList.begin();
	while(itr != m_InvokingItemList.end())
	{
		ITEM_GENERAL *pItemG = *itr;
		
		ATUM_DATE_TIME atimeCur { true };
		int nElapsedSec	= pItemG->CoolingTimeStamp + atimeCur.GetTimeDiffTimeInSeconds(pItemG->CoolingStartTime);			
		if(nElapsedSec < pItemG->ItemInfo->ReAttacktime/1000)
		{
			if(i_bSaveCoolingItem)
			{
				QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP *pQParam = new QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP;
				pQParam->ItemUniqueNumber	= pItemG->UniqueNumber;
				pQParam->CoolingTimeStamp	= nElapsedSec;
				ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemCoolingTimeStamp, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);

				// �������� ����
				INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_GET_COOLINGTIME_INFO_OK, T_FC_ITEM_GET_COOLINGTIME_INFO_OK, pSMsg, SendBuf);
				pSMsg->ItemUID			= pItemG->UniqueNumber;
				pSMsg->CoolingTimeStamp	= nElapsedSec;
				m_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ITEM_GET_COOLINGTIME_INFO_OK));
			}
		}
		else
		{
			pItemG->CoolingTimeStamp	= 0;
			
			QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP *pQParam = new QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP;
			pQParam->ItemUniqueNumber	= pItemG->UniqueNumber;
			pQParam->CoolingTimeStamp	= pItemG->CoolingTimeStamp;
			ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemCoolingTimeStamp, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);

			// �������� ����
			INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_END_COOLINGTIME_ITEM_OK, T_FC_ITEM_END_COOLINGTIME_ITEM_OK, pMsg, pBuf);
			pMsg->ItemUID	= pItemG->UniqueNumber;
			m_pFieldIOCPSocket->SendAddData(pBuf, MSG_SIZE(MSG_FC_ITEM_END_COOLINGTIME_ITEM_OK));
			
			// ����Ʈ���� ����
			itr = m_InvokingItemList.erase(itr);
			continue;
		}
		itr++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		cmkwon
/// \date		2010-03-04 ~ 2010-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ITEM_GENERAL *CFieldItemManager::InvokingFindItemGNoLock(ItemUID_t i_ItemUID)
{
	vectITEM_GENERALPtr::iterator itr = m_InvokingItemList.begin();
	for(; itr != m_InvokingItemList.end(); itr++)
	{
		ITEM_GENERAL *pItemG = *itr;
		if(i_ItemUID == pItemG->UniqueNumber)
		{
			return pItemG;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		cmkwon
/// \date		2010-03-04 ~ 2010-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::InvokingDeleteItemGNoLock(ItemUID_t i_ItemUID)
{
	vectITEM_GENERALPtr::iterator itr = m_InvokingItemList.begin();
	for(; itr != m_InvokingItemList.end(); itr++)
	{
		ITEM_GENERAL *pItemG = *itr;
		if(i_ItemUID == pItemG->UniqueNumber)
		{
			m_InvokingItemList.erase(itr);
			return TRUE;
		}
	}
	return FALSE;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
/// \author		cmkwon
/// \date		2010-03-04 ~ 2010-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::InvokingCheckExpire(void)
{
	mt_auto_lock igLock(&m_mapItemGeneral);
	vectITEM_GENERALPtr::iterator itr = m_InvokingItemList.begin();
	while(itr != m_InvokingItemList.end())
	{
		ITEM_GENERAL *pItemG = *itr;
		
		ATUM_DATE_TIME atimeCur { true };
		int nElapsedSec	= pItemG->CoolingTimeStamp + atimeCur.GetTimeDiffTimeInSeconds(pItemG->CoolingStartTime);			
		if(nElapsedSec < pItemG->ItemInfo->ReAttacktime/1000)
		{
			QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP *pQParam = new QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP;
			pQParam->ItemUniqueNumber	= pItemG->UniqueNumber;
			pQParam->CoolingTimeStamp	= nElapsedSec;
			ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemCoolingTimeStamp, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
			
			// �������� ����
			INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_GET_COOLINGTIME_INFO_OK, T_FC_ITEM_GET_COOLINGTIME_INFO_OK, pSMsg, SendBuf);
			pSMsg->ItemUID			= pItemG->UniqueNumber;
			pSMsg->CoolingTimeStamp	= nElapsedSec;
			m_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ITEM_GET_COOLINGTIME_INFO_OK));
		}
		else
		{
			pItemG->CoolingTimeStamp	= 0;
			
			QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP *pQParam = new QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP;
			pQParam->ItemUniqueNumber	= pItemG->UniqueNumber;
			pQParam->CoolingTimeStamp	= pItemG->CoolingTimeStamp;
			ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemCoolingTimeStamp, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
			
			// �������� ����
			INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_END_COOLINGTIME_ITEM_OK, T_FC_ITEM_END_COOLINGTIME_ITEM_OK, pMsg, pBuf);
			pMsg->ItemUID	= pItemG->UniqueNumber;
			m_pFieldIOCPSocket->SendAddData(pBuf, MSG_SIZE(MSG_FC_ITEM_END_COOLINGTIME_ITEM_OK));
			
			// ����Ʈ���� ����
			itr = m_InvokingItemList.erase(itr);
			continue;
		}
		
		itr++;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���Ǵ�Ƽ ���� ĳ�� ������ ���� -
/// \author		cmkwon
/// \date		2010-03-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::UpdateItemCountByPointer(DestParam_t i_byDestParam, BYTE i_enumStorage/*=ITEM_IN_CHARACTER*/)		// 2011-08-01 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ��� ���� (DestParameter - 255 -> 32767 ����)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	
	ITEM_GENERAL *pItemG = this->GetItemGeneralByDestParam(i_byDestParam, i_enumStorage);
	
	if(NULL == pItemG
		|| FALSE == IS_COUNTABLE_ITEM(pItemG->Kind))
	{// 2010-03-23 by cmkwon, ����� ī���ͺ� �����۸� ó��
		return FALSE;
	}
	
	this->UpdateItemCountByPointer(pItemG, -1, IUT_USE_ITEM);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-02 by cmkwon, ����2�� �߰� ���� - 
/// \author		cmkwon
/// \date		2010-04-02 ~ 2010-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INFINITY_INSERT_ITEM_INFO *CFieldItemManager::InfinityFindItemInfoFromInsertItemListNoLock(INT i_ItemNum)
{
	mtvectorInfinityInsertItemInfo::iterator itr(m_mtInfinityInsertItemInfoList.begin());
	for(; itr != m_mtInfinityInsertItemInfoList.end(); itr++)
	{
		INFINITY_INSERT_ITEM_INFO *pInfiItemInfo = &*itr;
		if(i_ItemNum == pInfiItemInfo->ItemNum)
		{
			return pInfiItemInfo;
		}
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-02 by cmkwon, ����2�� �߰� ���� - 
/// \author		cmkwon
/// \date		2010-04-02 ~ 2010-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INFINITY_INSERT_ITEM_INFO *CFieldItemManager::InfinityFindItemInfoFromInsertItemListNoLock(ItemUID_t i_ItemUID)
{
	mtvectorInfinityInsertItemInfo::iterator itr(m_mtInfinityInsertItemInfoList.begin());
	for(; itr != m_mtInfinityInsertItemInfoList.end(); itr++)
	{
		INFINITY_INSERT_ITEM_INFO *pInfiItemInfo = &*itr;
		if(i_ItemUID == pInfiItemInfo->ItemUID)
		{
			return pInfiItemInfo;
		}
	}
	
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-02 by cmkwon, ����2�� �߰� ���� - 
/// \author		cmkwon
/// \date		2010-04-02 ~ 2010-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::InfinityDeleteItemInfoFromInsertItemList(ItemUID_t i_ItemUID)
{
	mt_auto_lock mtA(&m_mtInfinityInsertItemInfoList);
	mtvectorInfinityInsertItemInfo::iterator itr(m_mtInfinityInsertItemInfoList.begin());
	for(; itr != m_mtInfinityInsertItemInfoList.end(); itr++)
	{
		INFINITY_INSERT_ITEM_INFO *pInfiItemInfo = &*itr;
		if(i_ItemUID == pInfiItemInfo->ItemUID)
		{
			m_mtInfinityInsertItemInfoList.erase(itr);
			return TRUE;
		}
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-02 by cmkwon, ����2�� �߰� ���� - 
/// \author		cmkwon
/// \date		2010-04-02 ~ 2010-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::InfinitySendInsertItemList(CFieldIOCPSocket *i_pFISoc4MFS)
{
	mt_auto_lock mtA(&m_mtInfinityInsertItemInfoList);
	mtvectorInfinityInsertItemInfo::iterator itr(m_mtInfinityInsertItemInfoList.begin());
	for(; itr != m_mtInfinityInsertItemInfoList.end(); itr++)
	{
		INFINITY_INSERT_ITEM_INFO *pInfiItemInfo = &*itr;

		INIT_MSG_WITH_BUFFER(MSG_FtoA_INFINITY_INSERT_ITEM, T_FtoA_INFINITY_INSERT_ITEM, pInsertItemSendMsg, InsertItemSendBuf);
		pInsertItemSendMsg->AccountUID			= m_pCharacter->AccountUniqueNumber;
		pInsertItemSendMsg->MFSCharacterUID		= m_pFieldIOCPSocket->m_AfsNeedMfsInfo.MFSCharacterUID;
		pInsertItemSendMsg->MFSClientIndex		= m_pFieldIOCPSocket->m_AfsNeedMfsInfo.MFSClientIdx;
		pInsertItemSendMsg->CreatedTime			= pInfiItemInfo->CreatedTime;
		pInsertItemSendMsg->CurrentCount		= pInfiItemInfo->CurrentCount;
		pInsertItemSendMsg->ItemNum				= pInfiItemInfo->ItemNum;
		pInsertItemSendMsg->ItemWindowIndex		= pInfiItemInfo->ItemWindowIndex;
		pInsertItemSendMsg->PrefixCodeNum		= pInfiItemInfo->PrefixCodeNum;
		pInsertItemSendMsg->SuffixCodeNum		= pInfiItemInfo->SuffixCodeNum;
		pInsertItemSendMsg->UsingTimeStamp		= pInfiItemInfo->UsingTimeStamp;
		pInsertItemSendMsg->Wear				= pInfiItemInfo->Wear;
		pInsertItemSendMsg->ShapeItemNum		= pInfiItemInfo->ShapeItemNum;
		pInsertItemSendMsg->CoolingTimeStamp	= pInfiItemInfo->CoolingTimeStamp;	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
		i_pFISoc4MFS->SendAddData(InsertItemSendBuf, MSG_SIZE(MSG_FtoA_INFINITY_INSERT_ITEM));

//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CFieldItemManager::InfinitySendInsertItemList# !, %s, ItemNum(%d) CurrentCount(%8d) Wear(%d) UsingTimeStamp(%d) CoolingTimeStamp(%d) \r\n"
//			, m_pFieldIOCPSocket->m_InfinityPlayingInfo.InfinityCreateUID, GetCharacterString(m_pCharacter, string()), pInfiItemInfo->ItemNum, pInfiItemInfo->CurrentCount, pInfiItemInfo->Wear, pInfiItemInfo->UsingTimeStamp, pInfiItemInfo->CoolingTimeStamp);
	}

	// 2010-07-15 by jskim ���Ǵ�Ƽ ����� ������ ���� �Ǵ� ���� ����
	m_mtInfinityInsertItemInfoList.clear();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-02 by cmkwon, ����2�� �߰� ���� - 
/// \author		cmkwon
/// \date		2010-04-02 ~ 2010-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INFINITY_UPDATE_ITEM_INFO *CFieldItemManager::InfinityFindItemInfoFromUpdateItemListNoLock(ItemUID_t i_ItemUID)
{
	mtvectorInfinityUpdateItemInfo::iterator itr(m_mtInfinityUpdateItemInfoList.begin());
	for(; itr != m_mtInfinityUpdateItemInfoList.end(); itr++)
	{
		INFINITY_UPDATE_ITEM_INFO *pInfiItemInfo = &*itr;
		if(i_ItemUID == pInfiItemInfo->ItemUID)
		{
			return pInfiItemInfo;
		}
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-02 by cmkwon, ����2�� �߰� ���� - 
/// \author		cmkwon
/// \date		2010-04-02 ~ 2010-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::InfinityDeleteItemInfoFromUpdateItemList(ItemUID_t i_ItemUID)
{
	mt_auto_lock mtA(&m_mtInfinityUpdateItemInfoList);
	mtvectorInfinityUpdateItemInfo::iterator itr(m_mtInfinityUpdateItemInfoList.begin());
	for(; itr != m_mtInfinityUpdateItemInfoList.end(); itr++)
	{
		INFINITY_UPDATE_ITEM_INFO *pInfiItemInfo = &*itr;
		if(i_ItemUID == pInfiItemInfo->ItemUID)
		{
			m_mtInfinityUpdateItemInfoList.erase(itr);
			return TRUE;
		}
	}
	
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-02 by cmkwon, ����2�� �߰� ���� - 
/// \author		cmkwon
/// \date		2010-04-02 ~ 2010-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::InfinitySendUpdateItemList(CFieldIOCPSocket *i_pFISoc4MFS)
{
	mt_auto_lock mtA(&m_mtInfinityUpdateItemInfoList);
	mtvectorInfinityUpdateItemInfo::iterator itr(m_mtInfinityUpdateItemInfoList.begin());
	for(; itr != m_mtInfinityUpdateItemInfoList.end(); itr++)
	{
		INFINITY_UPDATE_ITEM_INFO *pInfiItemInfo = &*itr;

		INIT_MSG_WITH_BUFFER(MSG_FtoA_INFINITY_UPDATE_ITEM, T_FtoA_INFINITY_UPDATE_ITEM, pUpdateItemSendMsg, UpdateItemSendBuf);
		pUpdateItemSendMsg->AccountUID			= m_pCharacter->AccountUniqueNumber;
		pUpdateItemSendMsg->MFSCharacterUID		= m_pFieldIOCPSocket->m_AfsNeedMfsInfo.MFSCharacterUID;
		pUpdateItemSendMsg->MFSClientIndex		= m_pFieldIOCPSocket->m_AfsNeedMfsInfo.MFSClientIdx;
		pUpdateItemSendMsg->CurrentCount		= pInfiItemInfo->CurrentCount;
		pUpdateItemSendMsg->ItemWindowIndex		= pInfiItemInfo->ItemWindowIndex;
		pUpdateItemSendMsg->MainSvrItemUID		= pInfiItemInfo->MainSvrItemUID;
		pUpdateItemSendMsg->Wear				= pInfiItemInfo->Wear;
		pUpdateItemSendMsg->UsingTimeStamp		= pInfiItemInfo->UsingTimeStamp;
		pUpdateItemSendMsg->CoolingTimeStamp	= pInfiItemInfo->CoolingTimeStamp;	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
		i_pFISoc4MFS->SendAddData(UpdateItemSendBuf, MSG_SIZE(MSG_FtoA_INFINITY_UPDATE_ITEM));

//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CFieldItemManager::InfinitySendUpdateItemList# !, %s, ItemUID4MFS(%I64d) CurrentCount(%8d) Wear(%d) UsingTimeStamp(%d) CoolingTimeStamp(%d) \r\n"
//			, m_pFieldIOCPSocket->m_InfinityPlayingInfo.InfinityCreateUID, GetCharacterString(m_pCharacter, string()), pInfiItemInfo->MainSvrItemUID, pInfiItemInfo->CurrentCount, pInfiItemInfo->Wear, pInfiItemInfo->UsingTimeStamp, pInfiItemInfo->CoolingTimeStamp);
	}

	// 2010-07-15 by jskim ���Ǵ�Ƽ ����� ������ ���� �Ǵ� ���� ����
	m_mtInfinityUpdateItemInfoList.clear();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-02 by cmkwon, ����2�� �߰� ���� - 
/// \author		cmkwon
/// \date		2010-04-02 ~ 2010-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INFINITY_DELETE_ITEM_INFO *CFieldItemManager::InfinityFindItemInfoFromDeleteItemListNoLock(ItemUID_t i_ItemUID4MFS)
{
	mtvectorInfinityDeleteItemInfo::iterator itr(m_mtInfinityDeleteItemInfoList.begin());
	for(; itr != m_mtInfinityDeleteItemInfoList.end(); itr++)
	{
		INFINITY_DELETE_ITEM_INFO *pInfiItemInfo = &*itr;
		if(i_ItemUID4MFS == pInfiItemInfo->MainSvrItemUID)
		{
			return pInfiItemInfo;
		}
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-02 by cmkwon, ����2�� �߰� ���� - 
/// \author		cmkwon
/// \date		2010-04-02 ~ 2010-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::InfinityDeleteItemInfoFromDeleteItemList(ItemUID_t i_ItemUID4MFS)
{
	mt_auto_lock mtA(&m_mtInfinityDeleteItemInfoList);
	mtvectorInfinityDeleteItemInfo::iterator itr(m_mtInfinityDeleteItemInfoList.begin());
	for(; itr != m_mtInfinityDeleteItemInfoList.end(); itr++)
	{
		INFINITY_DELETE_ITEM_INFO *pInfiItemInfo = &*itr;
		if(i_ItemUID4MFS == pInfiItemInfo->MainSvrItemUID)
		{
			m_mtInfinityDeleteItemInfoList.erase(itr);
			return TRUE;
		}
	}
	
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-02 by cmkwon, ����2�� �߰� ���� - 
/// \author		cmkwon
/// \date		2010-04-02 ~ 2010-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::InfinityInsertItemInfoFromDeleteItemList(ItemUID_t i_ItemUID4MFS)
{
	mt_auto_lock mtA(&m_mtInfinityDeleteItemInfoList);
	INFINITY_DELETE_ITEM_INFO *pDelItemInfo = this->InfinityFindItemInfoFromDeleteItemListNoLock(i_ItemUID4MFS);
	if(pDelItemInfo)
	{
		return FALSE;
	}

	INFINITY_DELETE_ITEM_INFO tmItemInfo;	util::zero(&tmItemInfo, sizeof(tmItemInfo));
	tmItemInfo.MainSvrItemUID	= i_ItemUID4MFS;
	m_mtInfinityDeleteItemInfoList.push_back(tmItemInfo);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-02 by cmkwon, ����2�� �߰� ���� - 
/// \author		cmkwon
/// \date		2010-04-02 ~ 2010-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::InfinitySendDeleteItemList(CFieldIOCPSocket *i_pFISoc4MFS)
{
	mt_auto_lock mtA(&m_mtInfinityDeleteItemInfoList);
	mtvectorInfinityDeleteItemInfo::iterator itr(m_mtInfinityDeleteItemInfoList.begin());
	for(; itr != m_mtInfinityDeleteItemInfoList.end(); itr++)
	{
		INFINITY_DELETE_ITEM_INFO *pInfiItemInfo = &*itr;

		INIT_MSG_WITH_BUFFER(MSG_FtoA_INFINITY_DELETE_ITEM, T_FtoA_INFINITY_DELETE_ITEM, pDeleteItemSendMsg, DeleteItemSendBuf);
		pDeleteItemSendMsg->AccountUID			= m_pCharacter->AccountUniqueNumber;
		pDeleteItemSendMsg->MFSCharacterUID		= m_pFieldIOCPSocket->m_AfsNeedMfsInfo.MFSCharacterUID;
		pDeleteItemSendMsg->MFSClientIndex		= m_pFieldIOCPSocket->m_AfsNeedMfsInfo.MFSClientIdx;
		pDeleteItemSendMsg->MainSvrItemUID		= pInfiItemInfo->MainSvrItemUID;
		i_pFISoc4MFS->SendAddData(DeleteItemSendBuf, MSG_SIZE(MSG_FtoA_INFINITY_DELETE_ITEM));

//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CFieldItemManager::InfinitySendDeleteItemList# !, %s DeleteItemMainSvrUID(%I64d) \r\n"
//			, m_pFieldIOCPSocket->m_InfinityPlayingInfo.InfinityCreateUID, GetCharacterString(m_pCharacter, string()), pInfiItemInfo->MainSvrItemUID);
	}

	// 2010-07-15 by jskim ���Ǵ�Ƽ ����� ������ ���� �Ǵ� ���� ����
	m_mtInfinityDeleteItemInfoList.clear();
}

// start 2011-09-30 by hskim, ��Ʈ�� �ý��� 2��
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// start 2011-09-30 by hskim, ��Ʈ�� �ý��� 2�� - ��Ʈ�� Current ���� ���� (���� ������ �ִ� td_Pet ���� ����)
/// \author		hskim
/// \date		2010-09-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::InfinitySendPetItemInfo(CFieldIOCPSocket *i_pFISoc4MFS)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);

	mtmapUID2ItemGeneral::iterator itr = m_mapItemGeneral.begin();

	for( ; itr != m_mapItemGeneral.end(); itr++)
	{
		if( ITEMKIND_PET_ITEM == itr->second->Kind )
		{
			// �� ���� �о�ͼ� ����
			ITEM_GENERAL *pItemG = itr->second;
			ITEM *pItemInfo = itr->second->ItemInfo;
			tPET_CURRENTINFO *pPetCurrentData = NULL;

			if( NULL == pItemG ) continue;
			if( NULL == pItemInfo) continue;
			if( 0 == pItemG->MainSvrItemUID )
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Error] [Infinity][%I64d] CFieldItemManager::InfinitySendPetItemInfo MainSvrItemUID is Zero!!, %s, ItemNum(%d) CurrentCount(%8d) Wear(%d) UsingTimeStamp(%d) CoolingTimeStamp(%d) \r\n"
					, m_pFieldIOCPSocket->m_InfinityPlayingInfo.InfinityCreateUID, GetCharacterString(m_pCharacter, string()), pItemG->ItemNum, pItemG->CurrentCount, pItemG->Wear, pItemG->UsingTimeStamp, pItemG->CoolingTimeStamp);

				continue;
			}

			pPetCurrentData = GetItemPetByUID(pItemG->UniqueNumber);

			if( NULL != pPetCurrentData )
			{
				INIT_MSG_WITH_BUFFER(MSG_FtoA_INFINITY_UPDATE_ITEM_PET, T_FtoA_INFINITY_UPDATE_ITEM_PET, pPetSendMsg, PetSendBuf);
				pPetSendMsg->AccountUID			= m_pCharacter->AccountUniqueNumber;
				pPetSendMsg->MFSCharacterUID	= m_pFieldIOCPSocket->m_AfsNeedMfsInfo.MFSCharacterUID;
				pPetSendMsg->MFSClientIndex		= m_pFieldIOCPSocket->m_AfsNeedMfsInfo.MFSClientIdx;
				pPetSendMsg->MainSvrItemUID		= pItemG->MainSvrItemUID;		// ���Ǵ�Ƽ�� �Ʒ��������� ��Ʈ�� ������ �ȵ�, �� UID�� 0�� ���� ������ ����

				memcpy(&pPetSendMsg->PetCurrentData, pPetCurrentData, sizeof(pPetSendMsg->PetCurrentData));

				i_pFISoc4MFS->SendAddData(PetSendBuf, MSG_SIZE(MSG_FtoA_INFINITY_UPDATE_ITEM_PET));
			}
		}
	}

	return TRUE;
}
// end 2011-09-30 by hskim, ��Ʈ�� �ý��� 2��

// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)
BOOL CFieldItemManager::RegisterTimerFixedTermShape(ITEM_GENERAL *i_pItemGeneral)
{
	if( NULL == i_pItemGeneral )
	{
		return FALSE;
	}

	if( IS_ENABLE_CHANGE_ShapeItemNum(i_pItemGeneral->Kind) )
	{
		if( 0 != i_pItemGeneral->ShapeItemNum && TRUE == i_pItemGeneral->FixedTermShape.bActive )
		{
			INT nCalcMin = 0;
			ATUM_DATE_TIME CurrentDate { true };

			nCalcMin = i_pItemGeneral->FixedTermShape.EndDate.GetTimeDiffTimeInMinutes(CurrentDate);

			if( nCalcMin > 0 && nCalcMin < MANAGE_MAX_TIME_FIXED_TERM_MINUTELY )
			{
				i_pItemGeneral->FixedTermShape.TimerUID = m_pFieldIOCPSocket->m_TimerManager.StartTimerField(TE_TYPE_FIXED_TERM_SHAPE, nCalcMin * TIMER_DO_MINUTELY_WORK, 0, 0);
			}
		}
	}

	return TRUE;
}

BOOL CFieldItemManager::RemoveTimerFixedTermShape(ITEM_GENERAL *i_pItemGeneral)
{
	if( NULL == i_pItemGeneral )
	{
		return FALSE;
	}	

	if( IS_ENABLE_CHANGE_ShapeItemNum(i_pItemGeneral->Kind) )
	{
		if( 0 != i_pItemGeneral->ShapeItemNum && TRUE == i_pItemGeneral->FixedTermShape.bActive )
		{
			m_pFieldIOCPSocket->m_TimerManager.StopTimerField(i_pItemGeneral->FixedTermShape.TimerUID);
			i_pItemGeneral->FixedTermShape.TimerUID = 0;
		}
	}
	
	return TRUE;
}

BOOL CFieldItemManager::DeleteFixedTermShapeByTimerUID(UID32_t TimerUID)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);

	mtmapUID2ItemGeneral::iterator itr = m_mapItemGeneral.begin();

	for( ; itr != m_mapItemGeneral.end(); itr++)
	{
		if( IS_ENABLE_CHANGE_ShapeItemNum(itr->second->Kind) )		// ���� ���� ������ �����۵鸸 Ȯ��
		{
			ITEM_GENERAL *pItemG = itr->second;
			ITEM *pItemInfo = itr->second->ItemInfo;

			if( pItemG->FixedTermShape.TimerUID == TimerUID )
			{
				INT nCalcMin = 0;
				ATUM_DATE_TIME CurrentDate { true };

				nCalcMin = pItemG->FixedTermShape.EndDate.GetTimeDiffTimeInMinutes(CurrentDate);

				if( nCalcMin > 0 )
				{
					if( nCalcMin < MANAGE_MAX_TIME_FIXED_TERM_MINUTELY )
					{
						pItemG->FixedTermShape.TimerUID = m_pFieldIOCPSocket->m_TimerManager.StartTimerField(TE_TYPE_FIXED_TERM_SHAPE, nCalcMin * TIMER_DO_MINUTELY_WORK, 0, 0);
					}

					return FALSE;
				}
				
				INT nDelShapeItemNum = pItemG->ShapeItemNum;
				FIXED_TERM_INFO DelFixedTermShape;
				memcpy(&DelFixedTermShape, &pItemG->FixedTermShape, sizeof(FIXED_TERM_INFO));

				// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - �Ⱓ ����
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM	// - �Ⱓ ����
				if ( ITEMKIND_DEFENSE == pItemG->Kind )	// �Ƹӷ���
				{
					COLLECTION_INFO *tmCollectiobPtr = m_pFieldIOCPSocket->m_ArmorCollection.ChangeActivedCount(pItemG->ShapeItemNum, FALSE);
					if ( NULL != tmCollectiobPtr && 0 == tmCollectiobPtr->ActivedCount && 0 >= tmCollectiobPtr->RemainSeconds )
					{
						ITEM *tmShapeItem = ms_pFieldIOCP2->GetItemInfo(pItemG->ShapeItemNum);
						if ( NULL != tmShapeItem )
						{
							m_pFieldIOCPSocket->SendString128(STRING_128_USER_NOTICE, STRMSG_130531_0008, tmShapeItem->ItemName);
						}
					}
					if ( WEAR_ATTACHED == pItemG->Wear )
					{
						ITEM *pShapeKitItem = ms_pFieldIOCP2->GetItemInfo(pItemG->FixedTermShape.nStatShapeItemNum);
						if ( NULL != pShapeKitItem )
						{
							if ( TRUE == pShapeKitItem->IsExistDesParam(DES_ALL_PART) )
							{
								m_pFieldIOCPSocket->CheckAllState(pShapeKitItem->GetParameterValue(DES_ALL_PART)*min(pItemG->FixedTermShape.nStatLevel, SHAPE_STAT_MAX_LEVEL), FALSE);
							}
						}
					}
					////////////////////////////////////////////
					// �������� ������ ���� ã�´�,
					ITEM_GENERAL *pBeforeEngineItem = m_pFieldIOCPSocket->GetAttachItemGeneralByPosition(POS_REAR);
					m_pFieldIOCPSocket->m_ItemManager.CheckRequirementAllGeneralItems();
					ITEM_GENERAL *pAfterEngineItem = m_pFieldIOCPSocket->GetAttachItemGeneralByPosition(POS_REAR);
					if ( NULL != pBeforeEngineItem && NULL == pAfterEngineItem )		// ���� ������ ����.
					{
						m_pFieldIOCPSocket->SendString128(STRING_128_USER_POPUP, STRMSG_130531_0004);
// 						if ( FALSE == IS_CITY_MAP_INDEX(m_pFieldIOCPSocket->m_character.MapChannelIndex.MapIndex) )		// ���ø��� �ƴϸ� ���� ����
// 						{
// 							m_pFieldIOCPSocket->SendString128(STRING_128_USER_NOTICE, STRMSG_130531_0005);
// 							m_pFieldIOCPSocket->WarpToCityMap();
// 						}
					}
					pItemG->FixedTermShape.nStatLevel			= SHAPE_STAT_INIT_LEVEL;
					pItemG->FixedTermShape.nStatShapeItemNum	= 0;
					
					// DB ���� �κ� 
					QPARAM_SHAPE_STATLEVEL	*pQParamSS = new QPARAM_SHAPE_STATLEVEL;
					pQParamSS->ItemKind				= pItemG->ItemInfo->Kind;			// ������ kind
					pQParamSS->ItemUID				= pItemG->UniqueNumber;
					pQParamSS->nStatLevel			= pItemG->FixedTermShape.nStatLevel;
					pQParamSS->nStatShapeItemNum	= pItemG->FixedTermShape.nStatShapeItemNum;
					ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InsertShapeStatLevel, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParamSS);
				}
#endif
				// end 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - �Ⱓ ����
				pItemG->ShapeItemNum = 0;

				QPARAM_UPDATE_SHAPEITEMNUM *pQParam = new QPARAM_UPDATE_SHAPEITEMNUM;
				pQParam->ItemUID		= pItemG->UniqueNumber;
				pQParam->nShapeItemNum	= pItemG->ShapeItemNum;
				// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)
				pQParam->FixedTermShape.bActive = FALSE;
				pQParam->FixedTermShape.StartDate.Reset();
				pQParam->FixedTermShape.EndDate.Reset();
				pQParam->FixedTermShape.TimerUID = 0;
				// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)
				
				// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - �Ⱓ ����
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM	// - �Ⱓ ����
				pQParam->FixedTermShape.nStatLevel			= pItemG->FixedTermShape.nStatLevel;
				pQParam->FixedTermShape.nStatShapeItemNum	= pItemG->FixedTermShape.nStatShapeItemNum;
#endif
				// end 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - �Ⱓ ����
	
				ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateShapeItemNum, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);

				if( FALSE == g_pFieldGlobal->IsArenaServer() )
				{
					QPARAM_FIXED_TERM_SHAPE	*pQParamFT = new QPARAM_FIXED_TERM_SHAPE;
					pQParamFT->ItemUID = pItemG->UniqueNumber;
					pQParamFT->TermType = FIXED_TERM_SHAPE;
					pQParamFT->AppliedItemNum = pItemG->ShapeItemNum;
					pQParamFT->StartDate = pItemG->FixedTermShape.StartDate;
					pQParamFT->EndDate = pItemG->FixedTermShape.EndDate;
					ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_DeleteFixedTerm, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParamFT);

					CAtumLogSender::SendLogMessageFixedTermShapeEnd(m_pFieldIOCPSocket, pItemG, nDelShapeItemNum, DelFixedTermShape);
				}

				// 2013-04-18 by jhseol,bckim �̴��� �Ƹ� - ���� �Ⱓ ���ῡ ���� �̺�Ʈ Ȯ��
#ifdef S_MONTHL_ARMOR_EVENT_JHSEOL_BCKIM		// ���� �Ⱓ ���ῡ ���� �̺�Ʈ Ȯ��
				if ( NULL != ms_pFieldIOCP2->m_pMonthlyArmorManager )
				{
					ms_pFieldIOCP2->m_pMonthlyArmorManager->CheckMonthlyArmorEventItem(pItemG, m_pFieldIOCPSocket);
				}
#endif
				// end 2013-04-18 by jhseol,bckim �̴��� �Ƹ� - ���� �Ⱓ ���ῡ ���� �̺�Ʈ Ȯ��

				return TRUE;
			}
		}
	}

	return FALSE;
}
// end 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)

// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�κ��丮 ���� Ư�� �������� ã�� �Լ�
/// \author		jhjang
/// \date		2012-10-13 ~ 2012-10-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ITEM_GENERAL *CFieldItemManager::FindGeneralItemByItemNum(INT i_nItemNum)
{
	mt_auto_lock igLock(&m_mapItemGeneral);

	mtmapUID2ItemGeneral::iterator itr = m_mapItemGeneral.begin();
	for(; itr != m_mapItemGeneral.end(); itr++)
	{
		ITEM_GENERAL *pItemG = itr->second;
		if(i_nItemNum == pItemG->ItemNum)
		{
			return pItemG;
		}
	}
	return NULL;
}
// end 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������

// start 2012-11-21 by khkim, GLog 2��
void CFieldItemManager::SetGLogData(BYTE i_nLogType, ITEM_GENERAL *i_pItemGeneral, BYTE i_nCharLv, int i_nChangeCount, int i_nCharSerial)
{
	if(NULL != i_pItemGeneral && NULL != m_pFieldIOCPSocket )	// 2013-05-30 by jhseol, GLog �ý��� ����
	{
		if( FALSE == i_pItemGeneral->CurrentCount )
		{// SPI 0���� ���NO
			return;	
		}

		MSG_FL_LOG_ITEM LogItemParameter; 
		util::zero(&LogItemParameter, sizeof(LogItemParameter));	// 2013-05-30 by jhseol, GLog �ý��� ���� - �ʱ�ȭ
		LogItemParameter.LogType			= i_nLogType; 
		LogItemParameter.ItemSerial			= (UID64_t)i_pItemGeneral->ItemNum;	// 2013-05-30 by jhseol, GLog �ý��� ���� - ĳ����
		LogItemParameter.Count1				= (UID64_t)i_pItemGeneral->CurrentCount;	// 2013-05-30 by jhseol, GLog �ý��� ���� - ĳ����
		// 2013-05-30 by jhseol, GLog �ý��� ����
//		LogItemParameter.CharSerial			= i_nCharSerial;
//		LogItemParameter.CharLv				= i_nCharLv;
		LogItemParameter.CharSerial			= (UID64_t)m_pFieldIOCPSocket->m_character.CharacterUniqueNumber;
		LogItemParameter.CharLv				= (int)m_pFieldIOCPSocket->m_character.Level;
		// end 2013-05-30 by jhseol, GLog �ý��� ����
		//LogItemParameter.GameServerID		= g_pFieldGlobal->GetMGameServerID();

		if( NULL != i_nChangeCount )
		{// �������� ������ ���(UpdateItemCountByPointer�Լ� ���ø� ����) 
			LogItemParameter.Count1			= -i_nChangeCount;	
		}	

		// 2013-01-16 by khkim, �������� ���� ���Ž� �Ѿ˼� 1�� ���� ����
		if( FALSE == IS_COUNTABLE_ITEM(i_pItemGeneral->Kind) )
		{
			LogItemParameter.Count1 = 1;
		}
		// end 2013-01-16 by khkim, �������� ���� ���Ž� �Ѿ˼� 1�� ���� ����
		
		CAtumLogSender::SendLogItem(&LogItemParameter);
	}
}
// end 2012-11-21 by khkim, GLog 2��

// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - ReturnShapeChangeKitItem �Լ� ������
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldItemManager::ReturnShapeChangeKitItem(ITEM_GENERAL *i_pItemG)
/// \brief		�����ۿ� �������� ������ ������ Ȯ���ϰ� �ǵ��� �ִ� �Լ�
/// \author		jhseol
/// \date		2013-06-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::ReturnShapeChangeKitItem(ITEM_GENERAL *i_pItemG)
{
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM	// - �����ۿ� �������� ������ ������ Ȯ���ϰ� �ǵ��� �ִ� �Լ�
	if ( NULL == i_pItemG )
	{
		return;
	}
	if ( ITEMKIND_DEFENSE == i_pItemG->Kind					// �Ƹӷ� �̰�
		&& 0 != i_pItemG->ShapeItemNum						// �������� ������ �ְ�
		&& FALSE == i_pItemG->FixedTermShape.bActive		// ������ �� ���
		&& SHAPE_STAT_INIT_LEVEL >= i_pItemG->FixedTermShape.nStatLevel )		// �Ϲ� �ܺ�Ŷ���� ����� �༮�鸸.
	{
		ez_map<INT, ITEM*>::iterator itrKitItem = ms_pFieldIOCP2->m_mapItemInfo.begin();
		while (itrKitItem != ms_pFieldIOCP2->m_mapItemInfo.end())
		{
			ITEM *pKitItem = itrKitItem->second;
			if ( ITEMKIND_INGOT == pKitItem->Kind									// �Ϲ� �ܺ�Ŷ�� KIND �̾�� �ϸ�
				&& i_pItemG->ShapeItemNum == pKitItem->LinkItem						// ���� �����̾�� �ϸ�
				&& TRUE == IS_SAME_UNITKIND(i_pItemG->ItemInfo->ReqUnitKind, pKitItem->ReqUnitKind)	// ���� �� ReqUnitKind �̾�� �ϸ�
				&& TRUE == pKitItem->IsExistDesParam(DES_SHAPE_ITEM)				// ���� ���� DES�� ������ �־�� �ϸ�
				&& FALSE == pKitItem->IsExistDesParam(DES_FIXED_TERM_SHAPE_TIME) )	// �Ⱓ���� �ƴϾ�� �Ѵ�.
			{
				InsertItemBaseByItemNum(pKitItem->ItemNum, 1, IUT_BONUS_ITEM);
				return;
			}
			itrKitItem++;
		}
	}
#endif
}

bool CFieldItemManager::UpgradePet(UID64_t oldPetUID, ITEM* targetPetItem)
{
	if (oldPetUID < 0 || !targetPetItem)
		return false;

	// Query the Field DB Manager with the upgrade procedure of the Pet
	QPARAM_UPGRADE_PET* upgradePetParams = new QPARAM_UPGRADE_PET;
	upgradePetParams->OldItemUniqueNumber = oldPetUID;
	upgradePetParams->UpgradedPetItemUID = targetPetItem->ItemNum;

	m_pFieldDBManager->MakeAndEnqueueQuery(QT_UpgradePet
		, m_pFieldIOCPSocket
		, m_pFieldIOCPSocket->m_character.AccountUniqueNumber
		, upgradePetParams);

	return true;
}

void CFieldItemManager::InsertUpgradedPet(UID64_t oldPetUID, ITEM_GENERAL* newItemData, tPET_CURRENTINFO* newPetInfo)
{
	if (!newItemData || !newPetInfo || !m_pFieldIOCPSocket)
		return;

	// Delete old PET (without removing its dependent sockets)
	ITEM_GENERAL *i_pItemGeneral = GetItemGeneralByUID(oldPetUID, ITEM_IN_CHARACTER);
	if (!i_pItemGeneral)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "ERROR: Failed to upgrade the Pet. The old Pet was not found. Old Pet UID (%d), new Pet UID (%d)", oldPetUID, newItemData->UniqueNumber);
		return;
	}
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	m_mapItemGeneral.deleteNoLock(oldPetUID);
	mtAuto.auto_unlock_cancel();
	// Remove from IF Item List
	this->ItemDeleteByInfinityPlaying(i_pItemGeneral);

	// Send deletion of the Item to client
	INIT_MSG_WITH_BUFFER(MSG_FC_STORE_DELETE_ITEM, T_FC_STORE_DELETE_ITEM, msgDelete, msgDeleteBuf);
	msgDelete->ItemUniqueNumber = i_pItemGeneral->UniqueNumber;
	msgDelete->ItemDeletionType = IUT_MIXING;
	m_pFieldIOCPSocket->SendAddData(msgDeleteBuf, MSG_SIZE(MSG_FC_STORE_DELETE_ITEM));

	// Delete from PET Data
	mt_auto_lock mtauto(&m_vectPetCurrentInfo);
	for (mtvectPetCurrentInfo::iterator itr = m_vectPetCurrentInfo.begin(); itr != m_vectPetCurrentInfo.end(); itr++)
	{
		if ((*itr)->CreatedPetUID == i_pItemGeneral->UniqueNumber)
		{
			// Send deletion of the Pet to client
			INIT_MSG_WITH_BUFFER(MSG_TRADE_PET_DATA, T_FC_TRADE_DELETE_CURRENT_PET_DATA, Petcurrentmsg, sendPetbuff);
			Petcurrentmsg->CreatedPetUID = (*itr)->CreatedPetUID;
			Petcurrentmsg->PetExp = (*itr)->PetExp;
			Petcurrentmsg->PetIndex = (*itr)->PetIndex;
			Petcurrentmsg->PetLevel = (*itr)->PetLevel;
			memcpy(Petcurrentmsg->PetName, (*itr)->PetName, SIZE_MAX_PET_NAME);
			m_pFieldIOCPSocket->SendAddData(sendPetbuff, MSG_SIZE(MSG_TRADE_PET_DATA));

			// Remove from Memory
			util::del(*itr);
			m_vectPetCurrentInfo.erase(itr);
			break;
		}
	}
	mtauto.auto_unlock_cancel();

	// Delete old Item Memory
	util::del(i_pItemGeneral);

	// Copy the Items to Heap
	ITEM_GENERAL* newStoreItem = new ITEM_GENERAL;
	memcpy_s(newStoreItem, sizeof(ITEM_GENERAL), newItemData, sizeof(ITEM_GENERAL));
	tPET_CURRENTINFO* newPetItem = new tPET_CURRENTINFO(newPetInfo);

	// Add Store item (without adding to the DB)
	InsertItemGeneralByPointer(newStoreItem
		, IUT_MIXING
		, FALSE
		, ITEM_IN_CHARACTER
		, FALSE
		, TRUE);

	// Insert the new Pet
	InsertItemPetByPointer(newPetItem);

	// Update the Clients Pet Data
	m_pFieldIOCPSocket->SendPetItemList();
}

// end 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - ReturnShapeChangeKitItem �Լ� ������


















