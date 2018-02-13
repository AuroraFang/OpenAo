// GiveItemManager.cpp: implementation of the CGiveItemManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FieldIOCPSocket.h"
#include "GiveItemManager.h"
#include "FieldGlobal.h"			// 2008-01-17 by cmkwon, S_F: �ý��� �α� �߰� - ������� �߰�

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGiveItemManager::CGiveItemManager()
{
	m_mtEventItemList.reserve(10);
}

CGiveItemManager::~CGiveItemManager()
{
	m_mtEventItemList.clear();
}

// 2007-07-24 by dhjin, INT i_nUnitkind �߰�
// 2008-02-01 by cmkwon, ItemEvent �� LastGameEndDate üũ ��ƾ �߰� - 
//void CGiveItemManager::GetActEventItemGameStart(vectItemEventInfo *o_ItemEventInfo, BOOL i_bPCBangCheck, BYTE i_byInfluenecType, BOOL i_bMemberShip, ATUM_DATE_TIME *i_Time, INT i_nUnitkind, ATUM_DATE_TIME *i_patLastGameEndDate)
// 2010-06-16 by jskim, ������ �̺�Ʈ üũ ��� -
//void CGiveItemManager::GetActEventItemGameStart(vectItemEventInfo *o_ItemEventInfo, BOOL i_bMemberShip, BOOL i_bPCBangUser, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME *i_patRegisteredDate, ATUM_DATE_TIME *i_patLastGameEndDate)
// start 2011-08-25 by shcho, Ƚ���� ������ ���ޱ�� ���� - ���� �߰�
void CGiveItemManager::GetActEventItemGameStart(vectItemEventInfo *o_ItemEventInfo, BOOL i_bMemberShip, BOOL i_bPCBangUser, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME *i_patRegisteredDate, ATUM_DATE_TIME *i_patLastGameEndDate, BYTE b_Level, UID32_t i_AccountUniqueNumber, INT i_LoginCheckNum)
//end 2010-06-16 by jskim, ������ �̺�Ʈ üũ ��� -
{
	mt_auto_lock mtA { GetVectItemEventListPtr() };
	
	ATUM_DATE_TIME CurTime { true };

	for (auto& info : m_mtEventItemList)
	{
		//auto pItemEvInfo = &(m_mtEventItemList[i]);		// 2008-02-01 by cmkwon, ItemEvent �� LastGameEndDate üũ ��ƾ �߰� - 

		if ( info.StartTime < CurTime && info.EndTime > CurTime)
		{

			if (ITEM_EVENT_TYPE_INFLCHANGE == info.ItemEventType || ITEM_EVENT_TYPE_LEVELUP == info.ItemEventType
				|| ITEM_EVENT_TYPE_FIRST_PURCHASE == info.ItemEventType)	// 2013-03-29 by jhseol, ������ �̺�Ʈ - �ſ� ù ����� ��������

				continue;

			if(info.iLevelMin && info.iLevelMax &&
				(b_Level < info.iLevelMin || b_Level > info.iLevelMax))
			{
				continue;
			}

			if (!IsEnableItemEvent(&info, i_bMemberShip, i_bPCBangUser, i_byInfluenecType, i_nUnitkind, i_patRegisteredDate,i_LoginCheckNum,i_AccountUniqueNumber))
				
				continue;

			// 2008-04-23 by cmkwon, ���� ����, ITEM_EVENT_TYPE_ONLYONE Ÿ�� �� ������ atLastGameEndDate �� üũ�ؾ� �Ѵ�.
			if (ITEM_EVENT_TYPE_ONLYONE == info.ItemEventType && info.UseLastGameEndDate && info.atLastGameEndDate.IsValid())
			{
				auto patCheckDate = *i_patLastGameEndDate;

				// 2008-02-01 by cmkwon, LastGameEndDate�� ��ȿ���� �����Ƿ� RegisteredDate �� üũ�Ѵ�.
				if (!patCheckDate.IsValid()) patCheckDate = *i_patRegisteredDate;

				// 2008-02-01 by cmkwon, ��ȿ���� �Ȱų� ���� ��¥ ���� �ð� �̸� ���� �̺�Ʈ�� �ƴϴ�.
				if (!patCheckDate.IsValid() || patCheckDate > info.atLastGameEndDate) continue;
			}

// 2013-02-28 by bckim, �������� �����߰�
#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				
			if (ITEM_EVENT_TYPE_ONLYONE == info.ItemEventType && info.UseFixedPeriod && info.FixedPeriod_DAY)
			{
				auto patCheckDate = i_patLastGameEndDate;
				if (!i_patLastGameEndDate->IsValid())
				{				
					patCheckDate = i_patRegisteredDate;
				}
				auto tmpCurrentDate = ATUM_DATE_TIME::GetCurrentDateTime();

				// ������ ���ӽð��� ���� �ð����� �����̸� ������ ����
				tmpCurrentDate.AddDateTime(0,0, -info.FixedPeriod_DAY , 0, 0, 0);
				if (!patCheckDate->IsValid() || *patCheckDate > tmpCurrentDate) continue;
			}			
#endif

			o_ItemEventInfo->push_back(info);
		}
	}
}

mtvectItemEventInfo * CGiveItemManager::GetVectItemEventListPtr()
{
	return &m_mtEventItemList;
}

BOOL CGiveItemManager::InitItemEventManager(vectItemEventInfo *i_pItemEventList)
{
	// 2008-01-17 by cmkwon, S_F: �ý��� �α� �߰�
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] ItemEvent reloaded !!, ItemEvent Count(%d)\r\n", i_pItemEventList->size());

	mt_auto_lock mtA(this->GetVectItemEventListPtr());				// 2006-08-24 by cmkwon

	m_mtEventItemList.clear();			// 2006-09-12 by cmkwon, ������ �ʱ�ȭ ó���ؾ���

	if(i_pItemEventList->empty())
	{
		return FALSE;
	}
	
	m_mtEventItemList.assign(i_pItemEventList->begin(), i_pItemEventList->end());	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CGiveItemManager::GetActEventItemInflChange(vectItemEventInfo *o_ItemEventInfo, BOOL i_bMemberShip, BOOL i_bPCBangUser, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME *i_patRegisteredDate)
/// \brief		ITEM_EVENT_TYPE_NEWMEMBER_INFLCHANGE �̺�Ʈ ���� üũ
///				// 2008-02-01 by cmkwon, ItemEvent �� LastGameEndDate üũ ��ƾ �߰� - 
/// \author		dhjin
/// \date		2007-07-20 ~ 2007-07-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CGiveItemManager::GetActEventItemInflChange(vectItemEventInfo *o_ItemEventInfo, BOOL i_bMemberShip, BOOL i_bPCBangUser, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME *i_patRegisteredDate)
{
	mt_auto_lock mtA(this->GetVectItemEventListPtr());

	int v_size = m_mtEventItemList.size();
	for (int i=0; i < v_size;i++)
	{
		SITEM_EVENT_INFO *pItemEvInfo = &(m_mtEventItemList[i]);		// 2008-02-01 by cmkwon, ItemEvent �� LastGameEndDate üũ ��ƾ �߰� - 

		if (ITEM_EVENT_TYPE_INFLCHANGE != m_mtEventItemList[i].ItemEventType)
		{
			continue;
		}

// 2008-02-01 by cmkwon, ItemEvent �� LastGameEndDate üũ ��ƾ �߰� - �Ʒ��� ���� IsEnableItemEvent() �� üũ
// 		if (FALSE == (i_nUnitkind & m_mtEventItemList[i].UnitKind))
// 		{// 2007-07-24 by dhjin, ���� ����� �ƴ� �������� üũ
// 			continue;	
// 		}
// 
// 		if(m_mtEventItemList[i].NewMember
// 			&& (((m_mtEventItemList[i].StartTime) > *i_Time) 
// 				|| ((m_mtEventItemList[i].EndTime) < *i_Time)) )
// 		{// 2007-07-23 by dhjin, �̺�Ʈ �Ⱓ�ȿ� ������ ���� ������� üũ
// 			continue;
// 		}
		if(FALSE == this->IsEnableItemEvent(pItemEvInfo, i_bMemberShip, i_bPCBangUser, i_byInfluenecType, i_nUnitkind, i_patRegisteredDate))
		{// 2008-02-01 by cmkwon, IsEnableItemEvent() ���� üũ
			continue;
		}

		o_ItemEventInfo->push_back(m_mtEventItemList[i]);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CGiveItemManager::GetActEventItemLevelUp(vectItemEventInfo *o_ItemEventInfo, BOOL i_bMemberShip, BOOL i_bPCBangUser, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME *i_patRegisteredDate, BYTE i_nLevel)
/// \brief		ITEM_EVENT_TYPE_NEWMEMBER_LEVELUP �̺�Ʈ ���� üũ
///				// 2008-02-01 by cmkwon, ItemEvent �� LastGameEndDate üũ ��ƾ �߰� - 
/// \author		dhjin
/// \date		2007-07-20 ~ 2007-07-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CGiveItemManager::GetActEventItemLevelUp(vectItemEventInfo *o_ItemEventInfo, BOOL i_bMemberShip, BOOL i_bPCBangUser, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME *i_patRegisteredDate, BYTE i_nLevel)
{
	mt_auto_lock mtA(this->GetVectItemEventListPtr());

	int v_size = m_mtEventItemList.size();
	for (int i=0; i < v_size;i++)
	{
		SITEM_EVENT_INFO *pItemEvInfo = &(m_mtEventItemList[i]);		// 2008-02-01 by cmkwon, ItemEvent �� LastGameEndDate üũ ��ƾ �߰� - 

		if (ITEM_EVENT_TYPE_LEVELUP != m_mtEventItemList[i].ItemEventType
			|| i_nLevel != m_mtEventItemList[i].ReqLevel)
		{
			continue;
		}
// 2008-02-01 by cmkwon, ItemEvent �� LastGameEndDate üũ ��ƾ �߰� - �Ʒ��� ���� IsEnableItemEvent() �� üũ
// 		if (FALSE == (i_nUnitkind & m_mtEventItemList[i].UnitKind))
// 		{// 2007-07-24 by dhjin, ���� ����� �ƴ� �������� üũ
// 			continue;	
// 		}
// 
// 		if(m_mtEventItemList[i].NewMember
// 			&& (((m_mtEventItemList[i].StartTime) > *i_Time) 
// 				|| ((m_mtEventItemList[i].EndTime) < *i_Time)) )
// 		{// 2007-07-23 by dhjin, �̺�Ʈ �Ⱓ�ȿ� ������ ���� ������� üũ
// 			continue;
// 		}
		if(FALSE == this->IsEnableItemEvent(pItemEvInfo, i_bMemberShip, i_bPCBangUser, i_byInfluenecType, i_nUnitkind, i_patRegisteredDate))
		{// 2008-02-01 by cmkwon, IsEnableItemEvent() ���� üũ
			continue;
		}

		o_ItemEventInfo->push_back(m_mtEventItemList[i]);
	}
}

// 2013-03-29 by jhseol, ������ �̺�Ʈ - �ſ� ù ����� ��������
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CGiveItemManager::GetActEventItemFirstPurchase(vectItemEventInfo *o_ItemEventInfo, BOOL i_bMemberShip, BOOL i_bPCBangUser, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME i_atAccountLastBuyDate, ATUM_DATE_TIME i_atCharacterLastBuyDate)
/// \brief		�ſ� ù ����� ��������
/// \author		jhseol
/// \date		2013-03-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CGiveItemManager::GetActEventItemFirstPurchase(vectItemEventInfo *o_ItemEventInfo, BOOL i_bMemberShip, BOOL i_bPCBangUser, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME i_atAccountLastBuyDate, ATUM_DATE_TIME i_atCharacterLastBuyDate)
{
	mt_auto_lock mtA(this->GetVectItemEventListPtr());
	
	int v_size = m_mtEventItemList.size();
	for (int i=0; i < v_size;i++)
	{
		SITEM_EVENT_INFO *pItemEvInfo = &(m_mtEventItemList[i]);		// 2008-02-01 by cmkwon, ItemEvent �� LastGameEndDate üũ ��ƾ �߰� - 
		
		if (ITEM_EVENT_TYPE_FIRST_PURCHASE != pItemEvInfo->ItemEventType) continue;

		ATUM_DATE_TIME CurrentTime { true };
		if ( CurrentTime < pItemEvInfo->StartTime || CurrentTime > pItemEvInfo->EndTime )
		{
			continue;
		}
		if ( TRUE == pItemEvInfo->CheckWithCharacterUID )
		{
			if ( i_atCharacterLastBuyDate > pItemEvInfo->StartTime )
			{
				continue;
			}
		}
		else
		{
			if ( i_atAccountLastBuyDate > pItemEvInfo->StartTime )
			{
				continue;
			}
		}
		if(FALSE == this->IsEnableItemEvent(pItemEvInfo, i_bMemberShip, i_bPCBangUser, i_byInfluenecType, i_nUnitkind, &CurrentTime))
		{
			continue;
		}
		
		o_ItemEventInfo->push_back(m_mtEventItemList[i]);
	}
}
// end 2013-03-29 by jhseol, ������ �̺�Ʈ - �ſ� ù ����� ��������

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CGiveItemManager::IsEnableItemEvent(SITEM_EVENT_INFO *i_pItemEvInfo, BOOL i_bMemberShip, BOOL i_bPCBangCheck, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME *i_patRegisteredDate)
/// \brief		// 2008-02-01 by cmkwon, ItemEvent �� LastGameEndDate üũ ��ƾ �߰� - CGiveItemManager::IsEnableItemEvent() �߰�
///				// 2011-08-25 by shcho, Ƚ���� ������ ���ޱ�� ���� - üũ�ѹ� �����߰�
/// \author		cmkwon
/// \date		2008-02-01 ~ 2008-02-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CGiveItemManager::IsEnableItemEvent(SITEM_EVENT_INFO *i_pItemEvInfo, BOOL i_bMemberShip, BOOL i_bPCBangCheck, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME *i_patRegisteredDate, INT i_LoginEventCheckNumber, UID32_t i_AccountUniqueNumber/*=0*/)
{
	if(i_pItemEvInfo->OnlyPCBang
		&& FALSE == i_bPCBangCheck)
	{
		return FALSE;
	}

	if (i_pItemEvInfo->MemberShip
		&& FALSE == i_bMemberShip)
	{
		return FALSE;
	}

	if (INFLUENCE_TYPE_ALL_MASK != i_pItemEvInfo->byInfluenceType
		&& i_pItemEvInfo->byInfluenceType != i_byInfluenecType)
	{
		return FALSE;
	}

	if (FALSE == (i_nUnitkind & i_pItemEvInfo->UnitKind))
	{
		return FALSE;
	}

	if( i_pItemEvInfo->NewMember
		&& (i_pItemEvInfo->StartTime > *i_patRegisteredDate || i_pItemEvInfo->EndTime < *i_patRegisteredDate) )
	{
		return FALSE;
	}
	// start 2011-08-25 by shcho, Ƚ���� ������ ���ޱ�� ���� - ���ڷ� ���� üũ�ѹ��� üũ

	if( ITEM_EVENT_TYPE_LOGIN == i_pItemEvInfo->ItemEventType
		|| 0 != i_pItemEvInfo->LoginCheckNumber )
	{
		// �ѹ��� ����Ǿ�� �Ѵ�.
		if(i_LoginEventCheckNumber == i_pItemEvInfo->LoginCheckNumber) // ������ �Ϸ�� ������ ó��
		{
			// �̺�Ʈ ���� �α״� �����Ƿ� ���� �ȳ��ܵ� �ȴ�.
			// g_pFieldGlobal->WriteSystemLogEX(TRUE,"Login Event Success!! CheckEventNumber[%d], AccountUniqueNumber[%d]\r\n",i_LoginEventCheckNumber,i_AccountUniqueNumber);
			return TRUE;
		}
		return FALSE;	// �α����̺�Ʈ�̰� ���� �� ������ �������� ������ ������ ����
	}
	// end 2011-08-25 by shcho, Ƚ���� ������ ���ޱ�� ���� - ���ڷ� ���� üũ�ѹ��� üũ
	return TRUE;
}

// 2011-08-25 by shcho, Ƚ���� ������ ���ޱ�� ���� - üũ�Լ� �߰�
BOOL CGiveItemManager::IsEnableCheckLoginEvent()
{
	mt_auto_lock mtA(this->GetVectItemEventListPtr());
	
	int v_size = m_mtEventItemList.size();
	for (int i=0; i < v_size;i++)
	{
		SITEM_EVENT_INFO *pItemEvInfo = &(m_mtEventItemList[i]);		// 2008-02-01 by cmkwon, ItemEvent �� LastGameEndDate üũ ��ƾ �߰� - 
		
		if (ITEM_EVENT_TYPE_LEVELUP != m_mtEventItemList[i].ItemEventType) return TRUE;
	}
	return FALSE;
}

// 2013-03-29 by jhseol, ������ �̺�Ʈ - �ſ� ù ����� ��������
SITEM_EVENT_INFO* CGiveItemManager::GetItemEventInfoByEventUID(int i_nItemEventUID)
{
	mt_auto_lock mtA(this->GetVectItemEventListPtr());
	
	int v_size = m_mtEventItemList.size();
	for (int i=0; i < v_size;i++)
	{
		SITEM_EVENT_INFO *pItemEvInfo = &(m_mtEventItemList[i]);
		
		if ( i_nItemEventUID == pItemEvInfo->ItemEventUID )
		{
			return pItemEvInfo;
		}
	}
	return NULL;
}
// end 2013-03-29 by jhseol, ������ �̺�Ʈ - �ſ� ù ����� ��������
