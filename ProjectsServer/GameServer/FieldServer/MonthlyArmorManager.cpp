// MonthlyArmorManager.cpp: implementation of the CMonthlyArmorManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FieldIOCP.h"
#include "FieldIOCPSocket.h"
#include "MonthlyArmorManager.h"
#include "FieldGlobal.h"			// 2008-01-17 by cmkwon, S_F: �ý��� �α� �߰� - ������� �߰�

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMonthlyArmorManager::CMonthlyArmorManager(CFieldIOCP *i_pFieldIOCP)
{
	ms_pFieldIOCP = i_pFieldIOCP;
	m_mtMonthlyArmorEventList.reserve(10);
}

CMonthlyArmorManager::~CMonthlyArmorManager()
{
	m_mtMonthlyArmorEventList.clear();
}

mtvectMONTHLY_ARMOR_EVNET_INFO * CMonthlyArmorManager::GetVectMonthlyArmorEventListPtr()
{
	return &m_mtMonthlyArmorEventList;
}

BOOL CMonthlyArmorManager::InitMonthlyArmorEventManager(vectMONTHLY_ARMOR_EVNET_INFO *i_pMonthlyArmorEventList)
{
	// 2008-01-17 by cmkwon, S_F: �ý��� �α� �߰�
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] MonthlyArmorEvent reloaded !!, MonthlyArmorEvent Count(%d)\r\n", i_pMonthlyArmorEventList->size());

	mt_auto_lock mtA(this->GetVectMonthlyArmorEventListPtr());				// 2006-08-24 by cmkwon

	m_mtMonthlyArmorEventList.clear();			// 2006-09-12 by cmkwon, ������ �ʱ�ȭ ó���ؾ���

	if(i_pMonthlyArmorEventList->empty())
	{
		return FALSE;
	}
	
	m_mtMonthlyArmorEventList.assign(i_pMonthlyArmorEventList->begin(), i_pMonthlyArmorEventList->end());
	SendMonthlyArmorEventListToAllClients();
	return TRUE;
}

void CMonthlyArmorManager::CheckMonthlyArmorEventItem(ITEM_GENERAL* io_pItemG, CFieldIOCPSocket* i_pSock /*= NULL*/)
{
	if ( NULL == io_pItemG )
	{
		return;
	}

	if ( ITEMKIND_DEFENSE != io_pItemG->Kind && FALSE == io_pItemG->ItemInfo->IsExistDesParam(DES_SHAPE_ITEM) )
	{
		return;
	}

	if ( (ITEMKIND_DEFENSE == io_pItemG->Kind && io_pItemG->FixedTermShape.nStatLevel > SHAPE_STAT_INIT_LEVEL ) ||		// ��þƮ �ܺ�Ŷ�� ����� �Ƹ� ���� üũ
		(TRUE == io_pItemG->ItemInfo->IsExistDesParam(DES_COLLECTION_ARMOR_INDEX) && io_pItemG->ItemInfo->SkillLevel > 0) )		// ��þƮ �ܺ�Ŷ ���� üũ
	{
		mt_auto_lock mtA(this->GetVectMonthlyArmorEventListPtr());
		
		ATUM_DATE_TIME atCurrentDate { true };
		ItemNum_t tmBeforeOptionItemNum		= io_pItemG->nMonthlyOptionItemNum;
		ATUM_DATE_TIME tmBeforeEventDate	= io_pItemG->atMonthlyEventEndDate;
		
		io_pItemG->nMonthlyOptionItemNum	= 0;
		io_pItemG->atMonthlyEventEndDate.Reset();
		
		mtvectMONTHLY_ARMOR_EVNET_INFO::iterator itrEvent = m_mtMonthlyArmorEventList.begin();
		for ( ; itrEvent != m_mtMonthlyArmorEventList.end() ; itrEvent++ )
		{
			if ( atCurrentDate >= itrEvent->atStartDate && atCurrentDate < itrEvent->atEndDate )
			{
				if ( (ITEMKIND_DEFENSE == io_pItemG->Kind && io_pItemG->ShapeItemNum == itrEvent->nArmorSourceIndex) ||		// �Ƹ� �̸鼭 ���������� ������ üũ
					(io_pItemG->ItemInfo->IsExistDesParam(DES_SHAPE_ITEM) && io_pItemG->ItemInfo->LinkItem == itrEvent->nArmorSourceIndex) )	// �ܺ�Ŷ ������ �̸� ������ ������ ������ üũ
				{
					io_pItemG->nMonthlyOptionItemNum = itrEvent->nOptionItemNum;
					io_pItemG->atMonthlyEventEndDate = itrEvent->atEndDate;
					break;	// �� �������� ������ �׳� �����ϰ� ���⼭ stop, ���� ù��° �̺�Ʈ�� ����
				}
			}
		}
		mtA.auto_unlock_cancel();
		
		if ( NULL != i_pSock )
		{
			if ( tmBeforeOptionItemNum != io_pItemG->nMonthlyOptionItemNum && tmBeforeEventDate != io_pItemG->atMonthlyEventEndDate )
			{
				if ( WEAR_ATTACHED == io_pItemG->Wear )
				{
					ITEM *pBeforeItem = ms_pFieldIOCP->GetItemInfo(tmBeforeOptionItemNum);
					ITEM *pAfterItem = ms_pFieldIOCP->GetItemInfo(io_pItemG->nMonthlyOptionItemNum);
					if ( NULL != pBeforeItem )
					{
						i_pSock->ReleaseItemDesParam(pBeforeItem);
					}
					if ( NULL != pAfterItem )
					{
						i_pSock->ApplyItemDesParam(pAfterItem);
					}
				}
				INIT_MSG_OF_SIZE(MSG_FC_STORE_UPDATE_ARMOR_ITEM_EVENT_INFO, T_FC_STORE_UPDATE_ARMOR_ITEM_EVENT_INFO, pUpdateArmorEventInfo, pUpdateArmorEventInfoBuf);
				pUpdateArmorEventInfo->ItemUID			= io_pItemG->UniqueNumber;
				pUpdateArmorEventInfo->nOptionItemNum	= io_pItemG->nMonthlyOptionItemNum;
				pUpdateArmorEventInfo->atEndDate		= io_pItemG->atMonthlyEventEndDate;
				i_pSock->SendAddData(pUpdateArmorEventInfoBuf, MSG_SIZE(MSG_FC_STORE_UPDATE_ARMOR_ITEM_EVENT_INFO));
			}
		}
	}
}

void CMonthlyArmorManager::SendMonthlyArmorEventListToAllClients()
{
	CFieldIOCPSocket *pSock = NULL;

	for (int i = (CLIENT_INDEX_START_NUM); i < ms_pFieldIOCP->GetArrayClientSize(); i++)
	{
		pSock = (CFieldIOCPSocket*)ms_pFieldIOCP->GetIOCPSocket(i);
		if ( NULL != pSock )
		{
			if ( TRUE == pSock->IsValidCharacter(FALSE) )
			{
				mt_auto_lock igLock(&(pSock->m_ItemManager.m_mapItemGeneral));
				mtmapUID2ItemGeneral::iterator itrItem = pSock->m_ItemManager.m_mapItemGeneral.begin();
				for(; pSock->m_ItemManager.m_mapItemGeneral.end() != itrItem; itrItem++)
				{
					ITEM_GENERAL *pItemG = itrItem->second;
					CheckMonthlyArmorEventItem(pItemG, pSock);
				}
				SendMonthlyArmorEventAllCollectionList(pSock);	// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - �̴��� ��� �̺�Ʈ ���� ������
			}
		}
	}
}

// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - �̴��� ��� �̺�Ʈ ���� ������
void CMonthlyArmorManager::CheckMonthlyArmorEventCollection(CFieldIOCPSocket* i_pSock, ItemNum_t i_nShapeItemNum)
{
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM	// - �ϳ��� �÷��ǿ� ���Ͽ� �̴��� ��� �̺�Ʈ ���� ������
	if ( NULL == i_pSock )
	{
		// ����
		return;
	}
	mt_auto_lock mtA(this->GetVectMonthlyArmorEventListPtr());

	COLLECTION_INFO *tmCollectionPtr = i_pSock->m_ArmorCollection.GetCollectionInfoPtr(i_nShapeItemNum);
	if ( NULL != tmCollectionPtr )
	{
		ATUM_DATE_TIME atCurrentDate { true };
		mtvectMONTHLY_ARMOR_EVNET_INFO::iterator itrEvent = m_mtMonthlyArmorEventList.begin();
		for ( ; itrEvent != m_mtMonthlyArmorEventList.end() ; itrEvent++ )
		{
			if ( atCurrentDate >= itrEvent->atStartDate && atCurrentDate < itrEvent->atEndDate && tmCollectionPtr->ShapeNum == itrEvent->nArmorSourceIndex )
			{
				INIT_MSG_WITH_BUFFER(MSG_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO, T_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO, pSend, SendBuf);
				pSend->CollectionShapeNum	= tmCollectionPtr->ShapeNum;
				pSend->nOptionItemNum		= itrEvent->nOptionItemNum;
				pSend->atEndDate			= itrEvent->atEndDate;
				i_pSock->SendAddData(SendBuf, MSG_SIZE(MSG_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO));
			}
		}
	}

	mtA.auto_unlock_cancel();
#endif
}

void CMonthlyArmorManager::SendMonthlyArmorEventAllCollectionList(CFieldIOCPSocket* i_pSock)
{
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM	// - ��� �÷��ǿ� ���Ͽ� �̴��� ��� �̺�Ʈ ���� ������
	if ( NULL == i_pSock )
	{
		// ����
		return;
	}
	mt_auto_lock mtA(this->GetVectMonthlyArmorEventListPtr());
	
	i_pSock->SendAddMessageType(T_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INIT);
	
	ATUM_DATE_TIME atCurrentDate { true };
	mtvectMONTHLY_ARMOR_EVNET_INFO::iterator itrEvent = m_mtMonthlyArmorEventList.begin();
	for ( ; itrEvent != m_mtMonthlyArmorEventList.end() ; itrEvent++ )
	{
		if ( atCurrentDate >= itrEvent->atStartDate && atCurrentDate < itrEvent->atEndDate )
		{
			COLLECTION_INFO *tmCollectionPtr = i_pSock->m_ArmorCollection.GetCollectionInfoPtr(itrEvent->nArmorSourceIndex);
			if ( NULL != tmCollectionPtr )
			{
				INIT_MSG_WITH_BUFFER(MSG_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO, T_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO, pSend, SendBuf);
				pSend->CollectionShapeNum	= tmCollectionPtr->ShapeNum;
				pSend->nOptionItemNum		= itrEvent->nOptionItemNum;
				pSend->atEndDate			= itrEvent->atEndDate;
				i_pSock->SendAddData(SendBuf, MSG_SIZE(MSG_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO));
			}
		}
	}
	
	mtA.auto_unlock_cancel();
#endif
}
// end 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - �̴��� ��� �̺�Ʈ ���� ������