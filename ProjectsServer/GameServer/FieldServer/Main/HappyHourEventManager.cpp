// HappyHourEventManager.cpp: implementation of the CHappyHourEventManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HappyHourEventManager.h"
#include "FieldIOCP.h"
#include "GameEventManager.h"
#include "FieldGlobal.h"				// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHappyHourEventManager::CHappyHourEventManager(CFieldIOCP *i_pFIOCP, CGameEventManager *i_pGameEventManager)
{
	m_pFieldIOCP13			= i_pFIOCP;
	m_pGameEventManager		= i_pGameEventManager;
// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - �ּ� ó����
//	m_nAllEventUniqueNumber	= 0;
	util::zero(m_arrPeriodEachInfluenceType, sizeof(m_arrPeriodEachInfluenceType[0]) * INFLUENCE_TYPE_COUNT);	// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - �ʱ�ȭ
	util::zero(m_pArrCurHappyHourEventEachInfluenceType, sizeof(m_pArrCurHappyHourEventEachInfluenceType[0]) * INFLUENCE_TYPE_COUNT);	// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - �ʱ�ȭ


	// 2007-11-01 by cmkwon, ���º� HappyHourEvent �Ⱓ �ʱ�ȭ
	m_arrPeriodEachInfluenceType[0].InitSHAPPY_HOUR_EVENT_PERIOD(INFLUENCE_TYPE_NORMAL);
	m_arrPeriodEachInfluenceType[1].InitSHAPPY_HOUR_EVENT_PERIOD(INFLUENCE_TYPE_VCN);
	m_arrPeriodEachInfluenceType[2].InitSHAPPY_HOUR_EVENT_PERIOD(INFLUENCE_TYPE_ANI);
	m_arrPeriodEachInfluenceType[3].InitSHAPPY_HOUR_EVENT_PERIOD(INFLUENCE_TYPE_ALL_MASK);

	this->ResetHappyHourEventManager();
}
CHappyHourEventManager::~CHappyHourEventManager()
{
}

BOOL CHappyHourEventManager::InitHappyHourEventManager(mtvectHAPPY_HOUR_EVENT *i_pvectHappyEventList)
{
	mt_auto_lock mtA(this->GetVectHappyHourEventListPtr());				// 2006-08-24 by cmkwon

	if(i_pvectHappyEventList->empty())
	{
		return FALSE;
	}

	m_mtvectHappyHourEventList.clear();
	ATUM_DATE_TIME atimeCur { true };
	for(int i=0; i < i_pvectHappyEventList->size(); i++)
	{
		SHAPPY_HOUR_EVENT *pHappyEvent = &(*i_pvectHappyEventList)[i];
		switch(pHappyEvent->DayOfWeek)
		{
		case DAY_OF_WEEK_SUMDAY:
		case DAY_OF_WEEK_MONDAY:
		case DAY_OF_WEEK_TUESDAY:
		case DAY_OF_WEEK_WEDNESDAY:
		case DAY_OF_WEEK_THURSDAY:	
		case DAY_OF_WEEK_FRIDAY:
		case DAY_OF_WEEK_SATURDAY:
			{
				pHappyEvent->atimeStartTime2.Year	= pHappyEvent->atimeEndTime2.Year	= atimeCur.Year;
				pHappyEvent->atimeStartTime2.Month	= pHappyEvent->atimeEndTime2.Month	= atimeCur.Month;
				pHappyEvent->atimeStartTime2.Day	= pHappyEvent->atimeEndTime2.Day	= atimeCur.Day;

				if(pHappyEvent->atimeStartTime2 >= pHappyEvent->atimeEndTime2)
				{// ���� �̺�Ʈ �ð� ���� ����
					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] HappyHourEvent time error !!, HappyHourEventUniqueNumber(%u) InfluenceType(%d) %s ServerGroupID(%d) StartTime(%s) EndTime(%s)\r\n"
						, pHappyEvent->EventUniqueNumber, pHappyEvent->InfluenceType3, GetDayOfWeekString(pHappyEvent->DayOfWeek)
						, pHappyEvent->atimeStartTime2.GetDateTimeString().GetBuffer()
						, pHappyEvent->atimeEndTime2.GetDateTimeString().GetBuffer());
				}
				else
				{
					m_mtvectHappyHourEventList.push_back(*pHappyEvent);
				}
			}
			break;
		case DAY_OF_WEEK_HAPPYHOUREVENT_PERIOD:
			{				
				this->SetHappyHourEventPeriod(pHappyEvent->InfluenceType3, pHappyEvent->atimeStartTime2, pHappyEvent->atimeEndTime2);
			}
			break;
		case DAY_OF_WEEK_PCBANG_HAPPYHOUREVENT:
			{
				m_byPCBangHappyEvState				= PCBANG_HAPPYEV_STATE_TYPE_END;
				m_PCBangHappyHourEvent				= *pHappyEvent;
			}
			break;
		default:			
			{// 2007-10-30 by cmkwon, DayOfWeek �ʵ尪�� ��ȿ���� ����
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] HappyHourEvent day of week error !!, HappyHourEventUniqueNumber(%u) InfluenceType(%d) %s ServerGroupID(%d) StartTime(%s) EndTime(%s)\r\n"
					, pHappyEvent->EventUniqueNumber, pHappyEvent->InfluenceType3, GetDayOfWeekString(pHappyEvent->DayOfWeek)
					, pHappyEvent->atimeStartTime2.GetDateTimeString().GetBuffer()
					, pHappyEvent->atimeEndTime2.GetDateTimeString().GetBuffer());
			}
		}
	}// END - for(int i=0; i < i_pvectHappyEventList->size(); i++)

	return TRUE;
}

// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - ���� �߰�(BOOL i_bNotify=FALSE)
void CHappyHourEventManager::ResetHappyHourEventManager(BOOL i_bNotify/*=FALSE*/)
{
	mt_auto_lock mtA(this->GetVectHappyHourEventListPtr());				// 2006-08-24 by cmkwon

	if(i_bNotify)
	{
		INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, pSHappyEvEnd, SendBuf);
		int i=0;
		for(i=0; i < INFLUENCE_TYPE_COUNT; i++)
		{
			if(NULL == m_pArrCurHappyHourEventEachInfluenceType[i])
			{
				continue;
			}

			SHAPPY_HOUR_EVENT *pHappyEv		= m_pArrCurHappyHourEventEachInfluenceType[i];

			pSHappyEvEnd->byStateType		= HAPPYEV_STATE_TYPE_END;
			pSHappyEvEnd->byInfluenceType4	= pHappyEv->InfluenceType3;
			pSHappyEvEnd->StartATime		= pHappyEv->atimeStartTime2;
			pSHappyEvEnd->EndATime			= pHappyEv->atimeEndTime2;
			pSHappyEvEnd->fEXPRate2			= pHappyEv->fEXPRate2;
			pSHappyEvEnd->fSPIRate2			= pHappyEv->fSPIRate2;
			pSHappyEvEnd->fEXPRepairRate2	= pHappyEv->fEXPRepairRate2;
			pSHappyEvEnd->fDropItemRate2	= pHappyEv->fDropItemRate2;
			pSHappyEvEnd->fDropRareRate2	= pHappyEv->fDropRareItemRate2;
			pSHappyEvEnd->fWarPointRate2	= pHappyEv->fWarPointRate2;
			m_pFieldIOCP13->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK), pHappyEv->InfluenceType3, pHappyEv->MinLevel, pHappyEv->MaxLevel);		// 2008-03-13 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� - 
		}

		if(PCBANG_HAPPYEV_STATE_TYPE_END != m_byPCBangHappyEvState)
		{
			SHAPPY_HOUR_EVENT *pHappyEv		= &m_PCBangHappyHourEvent;

			pSHappyEvEnd->byStateType		= PCBANG_HAPPYEV_STATE_TYPE_END;
			pSHappyEvEnd->byInfluenceType4	= pHappyEv->InfluenceType3;
			pSHappyEvEnd->StartATime		= pHappyEv->atimeStartTime2;
			pSHappyEvEnd->EndATime			= pHappyEv->atimeEndTime2;
			pSHappyEvEnd->fEXPRate2			= pHappyEv->fEXPRate2;
			pSHappyEvEnd->fSPIRate2			= pHappyEv->fSPIRate2;
			pSHappyEvEnd->fEXPRepairRate2	= pHappyEv->fEXPRepairRate2;
			pSHappyEvEnd->fDropItemRate2	= pHappyEv->fDropItemRate2;
			pSHappyEvEnd->fDropRareRate2	= pHappyEv->fDropRareItemRate2;
			pSHappyEvEnd->fWarPointRate2	= pHappyEv->fWarPointRate2;
			m_pFieldIOCP13->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK), INFLUENCE_TYPE_ALL_MASK, pHappyEv->MinLevel, pHappyEv->MaxLevel, NULL, TRUE);		// 2008-03-13 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� - 
		}
	}

// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - ���� ���� ��
//	m_pCurrentHappyHourEvent	= NULL;
//	m_atimeStartTime.Reset();
//	m_atimeEndTime.Reset();
	// 2007-11-01 by cmkwon, ���º� HappyHourEvent �Ⱓ �ʱ�ȭ

	m_arrPeriodEachInfluenceType[0].InitSHAPPY_HOUR_EVENT_PERIOD(INFLUENCE_TYPE_NORMAL);
	m_arrPeriodEachInfluenceType[1].InitSHAPPY_HOUR_EVENT_PERIOD(INFLUENCE_TYPE_VCN);
	m_arrPeriodEachInfluenceType[2].InitSHAPPY_HOUR_EVENT_PERIOD(INFLUENCE_TYPE_ANI);
	m_arrPeriodEachInfluenceType[3].InitSHAPPY_HOUR_EVENT_PERIOD(INFLUENCE_TYPE_ALL_MASK);
	util::zero(m_pArrCurHappyHourEventEachInfluenceType, sizeof(m_pArrCurHappyHourEventEachInfluenceType[0]) * INFLUENCE_TYPE_COUNT);	// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - �ʱ�ȭ

	m_mtvectHappyHourEventList.clear();
	m_byPCBangHappyEvState		= PCBANG_HAPPYEV_STATE_TYPE_END;
	util::zero(&m_PCBangHappyHourEvent, sizeof(m_PCBangHappyHourEvent));		// 2006-08-23 by cmkwon
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			mtvectHAPPY_HOUR_EVENT *CHappyHourEventManager::GetVectHappyHourEventListPtr(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-08-22 ~ 2006-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
mtvectHAPPY_HOUR_EVENT *CHappyHourEventManager::GetVectHappyHourEventListPtr(void)
{
	return &m_mtvectHappyHourEventList;
}

// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - �Լ� �ּ� ó����
//void CHappyHourEventManager::SetHappyHourEventTime(ATUM_DATE_TIME i_atimeStart
//												   , ATUM_DATE_TIME i_atimeEnd
//												   , BOOL i_bStoreDB/*=FALSE*/)
//{
//	m_atimeStartTime	= i_atimeStart;
//	m_atimeEndTime		= i_atimeEnd;
//
//	if(i_bStoreDB)
//	{
//		QPARAM_UPDATE_HAPPY_HOUR_EVENT_DATE *pQParam = new QPARAM_UPDATE_HAPPY_HOUR_EVENT_DATE;
//		pQParam->atimeStartTime2	= m_atimeStartTime;
//		pQParam->atimeEndTime2		= m_atimeEndTime;
//		m_pFieldIOCP13->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateHappyHourEventDate, NULL, 0, pQParam);
//	}
//}
//
//SHAPPY_HOUR_EVENT *CHappyHourEventManager::GetCurrentHappyHourEvent(void)
//{
//	return m_pCurrentHappyHourEvent;
//}

// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - �Ʒ��� ���� �Լ��� ������
//SHAPPY_HOUR_EVENT *CHappyHourEventManager::FindHappyHourEventByTime(ATUM_DATE_TIME *pCurDateTime, int i_CurDayOfWeek)
//{
//	for(int i=0; i < m_mtvectHappyHourEventList.size(); i++)
//	{
//		SHAPPY_HOUR_EVENT *pHappyEv = &m_mtvectHappyHourEventList[i];
//		if(pHappyEv->DayOfWeek == i_CurDayOfWeek)
//		{
//			pHappyEv->atimeStartTime2.Year	= pHappyEv->atimeEndTime2.Year	= pCurDateTime->Year;
//			pHappyEv->atimeStartTime2.Month	= pHappyEv->atimeEndTime2.Month	= pCurDateTime->Month;
//			pHappyEv->atimeStartTime2.Day	= pHappyEv->atimeEndTime2.Day	= pCurDateTime->Day;
//
//			if(pHappyEv->atimeStartTime2 <= *pCurDateTime
//				&& pHappyEv->atimeEndTime2 > *pCurDateTime)
//			{
//				return pHappyEv;
//			}
//		}
//	}
//
//	return NULL;
//}
//
/////////////////////////////////////////////////////////////////////////////////
///// \fn			BOOL CHappyHourEventManager::OnDoMinutelyWorkHappyHourEventManager(ATUM_DATE_TIME *pDateTime)
///// \brief		
///// \author		cmkwon
///// \date		
///// \warning	
/////
///// \param		
///// \return		FALSE	: GameEvent Time üũ�� �����Ѵ�.
/////				TRUE	: GameEvent Time üũ�� �������� �ʴ´�
/////////////////////////////////////////////////////////////////////////////////
//BOOL CHappyHourEventManager::OnDoMinutelyWorkHappyHourEventManager(ATUM_DATE_TIME *pDateTime)
//{
//	mt_auto_lock mtHappy(this->GetVectHappyHourEventListPtr());	// 2006-08-23 by cmkwon
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 2006-08-23 by cmkwon, check PCBang event
//	BOOL bSendPCBangMessage = FALSE;
//	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, pSPCBang, PCBangSendBuf);
//	if(m_PCBangHappyHourEvent.atimeStartTime2 >= *pDateTime
//		|| m_PCBangHappyHourEvent.atimeEndTime2 <= *pDateTime)
//	{// 2006-08-23 by cmkwon, not PCBang event time.
//
//		if(PCBANG_HAPPYEV_STATE_TYPE_END != m_byPCBangHappyEvState)
//		{
//			bSendPCBangMessage			= TRUE;
//			m_byPCBangHappyEvState		= PCBANG_HAPPYEV_STATE_TYPE_END;
//			pSPCBang->byStateType		= m_byPCBangHappyEvState;
//			pSPCBang->StartATime		= m_PCBangHappyHourEvent.atimeStartTime2;
//			pSPCBang->EndATime			= m_PCBangHappyHourEvent.atimeEndTime2;
//			pSPCBang->fEXPRate2			= m_PCBangHappyHourEvent.fEXPRate2;
//			pSPCBang->fSPIRate2			= m_PCBangHappyHourEvent.fSPIRate2;
//			pSPCBang->fEXPRepairRate2	= m_PCBangHappyHourEvent.fEXPRepairRate2;
//			pSPCBang->fDropItemRate2	= m_PCBangHappyHourEvent.fDropItemRate2;
//			pSPCBang->fDropRareRate2	= m_PCBangHappyHourEvent.fDropRareItemRate2;
//			pSPCBang->fWarPointRate2	= m_PCBangHappyHourEvent.fWarPointRate2;			// 2007-06-26 by dhjin, ������Ʈ �̺�Ʈ ���� �߰�
//			m_pGameEventManager->EndPCBangGameEvent();
//		}
//	}
//	else
//	{
//		if(PCBANG_HAPPYEV_STATE_TYPE_END == m_byPCBangHappyEvState)
//		{
//			int nEventTimeMinute	= m_PCBangHappyHourEvent.atimeEndTime2.GetTimeDiffTimeInMinutes(m_PCBangHappyHourEvent.atimeStartTime2);
//			if(nEventTimeMinute > 0)
//			{
//				bSendPCBangMessage			= TRUE;
//				m_byPCBangHappyEvState		= PCBANG_HAPPYEV_STATE_TYPE_START;
//				pSPCBang->byStateType		= m_byPCBangHappyEvState;
//				pSPCBang->StartATime		= m_PCBangHappyHourEvent.atimeStartTime2;
//				pSPCBang->EndATime			= m_PCBangHappyHourEvent.atimeEndTime2;
//				pSPCBang->fEXPRate2			= m_PCBangHappyHourEvent.fEXPRate2;
//				pSPCBang->fSPIRate2			= m_PCBangHappyHourEvent.fSPIRate2;
//				pSPCBang->fEXPRepairRate2	= m_PCBangHappyHourEvent.fEXPRepairRate2;
//				pSPCBang->fDropItemRate2	= m_PCBangHappyHourEvent.fDropItemRate2;
//				pSPCBang->fDropRareRate2	= m_PCBangHappyHourEvent.fDropRareItemRate2;
//				pSPCBang->fWarPointRate2	= m_PCBangHappyHourEvent.fWarPointRate2;			// 2007-06-26 by dhjin, ������Ʈ �̺�Ʈ ���� �߰�
//				
//				m_pGameEventManager->StartPCBangGameEvent(GAME_EVENT_TYPE_EXP, m_PCBangHappyHourEvent.fEXPRate2, nEventTimeMinute);
//				m_pGameEventManager->StartPCBangGameEvent(GAME_EVENT_TYPE_SPI, m_PCBangHappyHourEvent.fSPIRate2, nEventTimeMinute);
//				m_pGameEventManager->StartPCBangGameEvent(GAME_EVENT_TYPE_EXP_NO_DOWN, m_PCBangHappyHourEvent.fEXPRepairRate2, nEventTimeMinute);
//				m_pGameEventManager->StartPCBangGameEvent(GAME_EVENT_TYPE_DROP_ITEM, m_PCBangHappyHourEvent.fDropItemRate2, nEventTimeMinute);
//				m_pGameEventManager->StartPCBangGameEvent(GAME_EVENT_TYPE_DROP_REAR_ITEM, m_PCBangHappyHourEvent.fDropRareItemRate2, nEventTimeMinute);
//				m_pGameEventManager->StartPCBangGameEvent(GAME_EVENT_TYPE_WARPOINT, m_PCBangHappyHourEvent.fWarPointRate2, nEventTimeMinute);			// 2007-06-26 by dhjin, ������Ʈ �̺�Ʈ ���� �߰�	
//			}
//		}
//	}
//
//	SHAPPY_HOUR_EVENT *pHappyEv = GetCurrentHappyHourEvent();
//
//	if(m_atimeStartTime >= *pDateTime
//		|| m_atimeEndTime <= *pDateTime
//		|| m_mtvectHappyHourEventList.empty())
//	{// 2006-04-12 by cmkwon, HappyHourEvent�� �Ⱓ�� ���� �Ǿ��ų� �����̺�Ʈ ������ ����
//
//		if(pHappyEv)
//		{// �̺�Ʈ ����
//			SHAPPY_HOUR_EVENT tmHappyEv = *pHappyEv;				// 2006-08-23 by cmkwon, �ӽ� ����
//			mtHappy.auto_unlock_cancel();							// 2006-08-23 by cmkwon
//
//			m_pGameEventManager->EndAllGameEvent();
//			m_pCurrentHappyHourEvent		= NULL;
//
//			INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, pSHappy, SendBuf);
//			pSHappy->byStateType			= HAPPYEV_STATE_TYPE_END;
//			pSHappy->StartATime				= tmHappyEv.atimeStartTime2;
//			pSHappy->EndATime				= tmHappyEv.atimeEndTime2;
//			pSHappy->fEXPRate2				= tmHappyEv.fEXPRate2;
//			pSHappy->fSPIRate2				= tmHappyEv.fSPIRate2;
//			pSHappy->fEXPRepairRate2		= tmHappyEv.fEXPRepairRate2;
//			pSHappy->fDropItemRate2			= tmHappyEv.fDropItemRate2;
//			pSHappy->fDropRareRate2			= tmHappyEv.fDropRareItemRate2;
//			pSHappy->fWarPointRate2			= tmHappyEv.fWarPointRate2;					// 2007-06-26 by dhjin, ������Ʈ �̺�Ʈ ���� �߰�
//			m_pFieldIOCP13->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK));
//		}
//		mtHappy.auto_unlock_cancel();							// 2006-08-23 by cmkwon
//		if(bSendPCBangMessage)
//		{// 2006-08-23 by cmkwon, PCBang �̺�Ʈ ���� ���� ����
//			m_pFieldIOCP13->SendMessageToAllClients(PCBangSendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK), INFLUENCE_TYPE_ALL_MASK, 0, 0, NULL, TRUE);
//		}
//		return FALSE;
//	}
//
//	struct tm localTime;	
//	pDateTime->Convert(localTime);
//	if(pHappyEv)
//	{
//		pHappyEv->atimeStartTime2.Year	= pHappyEv->atimeEndTime2.Year	= pDateTime->Year;
//		pHappyEv->atimeStartTime2.Month	= pHappyEv->atimeEndTime2.Month	= pDateTime->Month;
//		pHappyEv->atimeStartTime2.Day	= pHappyEv->atimeEndTime2.Day	= pDateTime->Day;
//
//		if(pHappyEv->DayOfWeek != localTime.tm_wday
//			|| pHappyEv->atimeStartTime2 > *pDateTime
//			|| pHappyEv->atimeEndTime2 <= *pDateTime)
//		{// ���� �̺�Ʈ ����
//			SHAPPY_HOUR_EVENT tmHappyEv = *pHappyEv;				// 2006-08-23 by cmkwon, �ӽ� ����
//			mtHappy.auto_unlock_cancel();							// 2006-08-23 by cmkwon
//
//			m_pGameEventManager->EndAllGameEvent();
//			m_pCurrentHappyHourEvent		= NULL;
//
//			INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, pSHappy, SendBuf);
//			pSHappy->byStateType			= HAPPYEV_STATE_TYPE_END;
//			pSHappy->StartATime				= tmHappyEv.atimeStartTime2;
//			pSHappy->EndATime				= tmHappyEv.atimeEndTime2;
//			pSHappy->fEXPRate2				= tmHappyEv.fEXPRate2;
//			pSHappy->fSPIRate2				= tmHappyEv.fSPIRate2;
//			pSHappy->fEXPRepairRate2		= tmHappyEv.fEXPRepairRate2;
//			pSHappy->fDropItemRate2			= tmHappyEv.fDropItemRate2;
//			pSHappy->fDropRareRate2			= tmHappyEv.fDropRareItemRate2;
//			pSHappy->fWarPointRate2			= tmHappyEv.fWarPointRate2;					// 2007-06-26 by dhjin, ������Ʈ �̺�Ʈ ���� �߰�
//			m_pFieldIOCP13->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK));
//		}
//		mtHappy.auto_unlock_cancel();							// 2006-08-23 by cmkwon
//		if(bSendPCBangMessage)
//		{// 2006-08-23 by cmkwon, PCBang �̺�Ʈ ���� ���� ����
//			m_pFieldIOCP13->SendMessageToAllClients(PCBangSendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK), INFLUENCE_TYPE_ALL_MASK, 0, 0, NULL, TRUE);
//		}
//		return TRUE;
//	}
//
//	pHappyEv = FindHappyHourEventByTime(pDateTime, localTime.tm_wday);
//	if(NULL == pHappyEv)
//	{
//		mtHappy.auto_unlock_cancel();							// 2006-08-23 by cmkwon
//		if(bSendPCBangMessage)
//		{// 2006-08-23 by cmkwon, PCBang �̺�Ʈ ���� ���� ����
//			m_pFieldIOCP13->SendMessageToAllClients(PCBangSendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK), INFLUENCE_TYPE_ALL_MASK, 0, 0, NULL, TRUE);
//		}
//		return FALSE;
//	}
//
//	int nEventTimeMinute	= pHappyEv->atimeEndTime2.GetTimeDiffTimeInMinutes(pHappyEv->atimeStartTime2);
//	if(nEventTimeMinute <= 0)
//	{// �̺�Ʈ ���� �ð� ����
//		mtHappy.auto_unlock_cancel();							// 2006-08-23 by cmkwon
//		if(bSendPCBangMessage)
//		{// 2006-08-23 by cmkwon, PCBang �̺�Ʈ ���� ���� ����
//			m_pFieldIOCP13->SendMessageToAllClients(PCBangSendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK), INFLUENCE_TYPE_ALL_MASK, 0, 0, NULL, TRUE);
//		}
//		return FALSE;
//	}
//	m_pCurrentHappyHourEvent		= pHappyEv;
//	SHAPPY_HOUR_EVENT tmHappyEv		= *pHappyEv;				// 2006-08-23 by cmkwon, �ӽ� ����
//	mtHappy.auto_unlock_cancel();								// 2006-08-23 by cmkwon
//	if(bSendPCBangMessage)
//	{// 2006-08-23 by cmkwon, PCBang �̺�Ʈ ���� ���� ����
//		m_pFieldIOCP13->SendMessageToAllClients(PCBangSendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK), INFLUENCE_TYPE_ALL_MASK, 0, 0, NULL, TRUE);
//	}
//	
//	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, pSHappy, SendBuf);
//	pSHappy->byStateType			= HAPPYEV_STATE_TYPE_START;
//	pSHappy->StartATime				= tmHappyEv.atimeStartTime2;
//	pSHappy->EndATime				= tmHappyEv.atimeEndTime2;
//	pSHappy->fEXPRate2				= tmHappyEv.fEXPRate2;
//	pSHappy->fSPIRate2				= tmHappyEv.fSPIRate2;
//	pSHappy->fEXPRepairRate2		= tmHappyEv.fEXPRepairRate2;
//	pSHappy->fDropItemRate2			= tmHappyEv.fDropItemRate2;
//	pSHappy->fDropRareRate2			= tmHappyEv.fDropRareItemRate2;
//	pSHappy->fWarPointRate2			= tmHappyEv.fWarPointRate2;					// 2007-06-26 by dhjin, ������Ʈ �̺�Ʈ ���� �߰�
//	m_pFieldIOCP13->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK));
//
//	m_pGameEventManager->StartGameEvent(GAME_EVENT_TYPE_EXP, tmHappyEv.fEXPRate2, nEventTimeMinute);
//	m_pGameEventManager->StartGameEvent(GAME_EVENT_TYPE_SPI, tmHappyEv.fSPIRate2, nEventTimeMinute);
//	m_pGameEventManager->StartGameEvent(GAME_EVENT_TYPE_EXP_NO_DOWN, tmHappyEv.fEXPRepairRate2, nEventTimeMinute);
//	m_pGameEventManager->StartGameEvent(GAME_EVENT_TYPE_DROP_ITEM, tmHappyEv.fDropItemRate2, nEventTimeMinute);
//	m_pGameEventManager->StartGameEvent(GAME_EVENT_TYPE_DROP_REAR_ITEM, tmHappyEv.fDropRareItemRate2, nEventTimeMinute);
//	m_pGameEventManager->StartGameEvent(GAME_EVENT_TYPE_WARPOINT, tmHappyEv.fWarPointRate2, nEventTimeMinute);			// 2007-06-26 by dhjin, ������Ʈ �̺�Ʈ ���� �߰�
//	
//	return TRUE;
//}
//
/////////////////////////////////////////////////////////////////////////////////
///// \fn			BOOL CHappyHourEventManager::GetPCBangHappyHourEventInfo(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK *o_pHappyInfoOK)
///// \brief		
///// \author		cmkwon
///// \date		2006-08-23 ~ 2006-08-23
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//BOOL CHappyHourEventManager::GetPCBangHappyHourEventInfo(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK *o_pHappyInfoOK)
//{
//	if(PCBANG_HAPPYEV_STATE_TYPE_END == m_byPCBangHappyEvState)
//	{
//		return FALSE;
//	}
//
//	o_pHappyInfoOK->byStateType		= PCBANG_HAPPYEV_STATE_TYPE_STARTING;
//	o_pHappyInfoOK->StartATime		= m_PCBangHappyHourEvent.atimeStartTime2;
//	o_pHappyInfoOK->EndATime		= m_PCBangHappyHourEvent.atimeEndTime2;
//	o_pHappyInfoOK->fEXPRate2		= m_PCBangHappyHourEvent.fEXPRate2;
//	o_pHappyInfoOK->fSPIRate2		= m_PCBangHappyHourEvent.fSPIRate2;
//	o_pHappyInfoOK->fEXPRepairRate2	= m_PCBangHappyHourEvent.fEXPRepairRate2;
//	o_pHappyInfoOK->fDropItemRate2	= m_PCBangHappyHourEvent.fDropItemRate2;
//	o_pHappyInfoOK->fDropRareRate2	= m_PCBangHappyHourEvent.fDropRareItemRate2;
//	o_pHappyInfoOK->fWarPointRate2	= m_PCBangHappyHourEvent.fWarPointRate2;
//
//	return TRUE;
//}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CHappyHourEventManager::OnDoMinutelyWorkHappyHourEventManager(ATUM_DATE_TIME *pDateTime)
/// \brief		// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - �Լ��� ������
/// \author		cmkwon
/// \date		2007-10-31 ~ 2007-10-31
/// \warning	
///
/// \param		
/// \return		FALSE	: GameEvent Time üũ�� �����Ѵ�.
///				TRUE	: GameEvent Time üũ�� �������� �ʴ´�
///////////////////////////////////////////////////////////////////////////////
BOOL CHappyHourEventManager::OnDoMinutelyWorkHappyHourEventManager(ATUM_DATE_TIME *pDateTime)
{
	mt_auto_lock mtHappy(this->GetVectHappyHourEventListPtr());	// 2006-08-23 by cmkwon

	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - PCBang HappyHourEvent ó��
	this->OnDoMinutelyWorkHappyHourEventManagerOfPCBang(pDateTime);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - ��� ���º� HappyHourEvent ó��
	this->OnDoMinutelyWorkHappyHourEventManagerAllInfluenceType(pDateTime);
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CHappyHourEventManager::OnDoMinutelyWorkHappyHourEventManagerOfPCBang(ATUM_DATE_TIME *pDateTime)
/// \brief		// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - CHappyHourEventManager::OnDoMinutelyWorkHappyHourEventManagerOfPCBang() �Լ� �߰�
/// \author		cmkwon
/// \date		2007-10-31 ~ 2007-10-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CHappyHourEventManager::OnDoMinutelyWorkHappyHourEventManagerOfPCBang(ATUM_DATE_TIME *pDateTime)
{
	if(m_PCBangHappyHourEvent.atimeStartTime2 >= *pDateTime
		|| m_PCBangHappyHourEvent.atimeEndTime2 <= *pDateTime)
	{// 2007-10-31 by cmkwon, PCBang HappyHourEvent �Ⱓ�� �ƴϴ�

		if(PCBANG_HAPPYEV_STATE_TYPE_END != m_byPCBangHappyEvState)
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] PCBang HappyHourEvent End !!, Period(%s ~ %s) ExpRate(%4.2f) SPIRate(%4.2f) ExpRepireRate(%4.2f) DropItemRate(%4.2f) DropRareItemRate(%4.2f) WarPointRate(%4.2f) MinLevel(%d) MaxLevel(%d)\r\n"
				, m_PCBangHappyHourEvent.atimeStartTime2.GetDateTimeString().GetBuffer(), m_PCBangHappyHourEvent.atimeEndTime2.GetDateTimeString().GetBuffer()
				, m_PCBangHappyHourEvent.fEXPRate2, m_PCBangHappyHourEvent.fSPIRate2, m_PCBangHappyHourEvent.fEXPRepairRate2
				, m_PCBangHappyHourEvent.fDropItemRate2, m_PCBangHappyHourEvent.fDropRareItemRate2, m_PCBangHappyHourEvent.fWarPointRate2, m_PCBangHappyHourEvent.MinLevel, m_PCBangHappyHourEvent.MaxLevel);

			///////////////////////////////////////////////////////////////////////////////
			// 2007-10-31 by cmkwon, PCBang HappyHourEvent ����� ����
			m_byPCBangHappyEvState		= PCBANG_HAPPYEV_STATE_TYPE_END;

			///////////////////////////////////////////////////////////////////////////////
			// 2007-10-31 by cmkwon, GameEvent ���� PCBang HappyHourEvent �� ������
			m_pGameEventManager->EndPCBangGameEvent();

			///////////////////////////////////////////////////////////////////////////////
			// 2007-10-31 by cmkwon, ��� PCBang �������� ����
			SHAPPY_HOUR_EVENT *pHappyEv	= &m_PCBangHappyHourEvent;
			INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, pSPCBang, SendBuf);			
			pSPCBang->byStateType		= m_byPCBangHappyEvState;
			pSPCBang->byInfluenceType4	= pHappyEv->InfluenceType3;
			pSPCBang->StartATime		= pHappyEv->atimeStartTime2;
			pSPCBang->EndATime			= pHappyEv->atimeEndTime2;
			pSPCBang->fEXPRate2			= pHappyEv->fEXPRate2;
			pSPCBang->fSPIRate2			= pHappyEv->fSPIRate2;
			pSPCBang->fEXPRepairRate2	= pHappyEv->fEXPRepairRate2;
			pSPCBang->fDropItemRate2	= pHappyEv->fDropItemRate2;
			pSPCBang->fDropRareRate2	= pHappyEv->fDropRareItemRate2;
			pSPCBang->fWarPointRate2	= pHappyEv->fWarPointRate2;			// 2007-06-26 by dhjin, ������Ʈ �̺�Ʈ ���� �߰�
			m_pFieldIOCP13->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK), INFLUENCE_TYPE_ALL_MASK, pHappyEv->MinLevel, pHappyEv->MaxLevel, NULL, TRUE);			// 2008-03-13 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� - 
		}
		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-31 by cmkwon, PCBang HappyHourEvent �Ⱓ


	if(PCBANG_HAPPYEV_STATE_TYPE_END != m_byPCBangHappyEvState)
	{// 2007-10-31 by cmkwon, PCBang HappyHourEvent �� ���� ���̴�
		return;
	}

	int nEventTimeMinute	= m_PCBangHappyHourEvent.atimeEndTime2.GetTimeDiffTimeInMinutes(m_PCBangHappyHourEvent.atimeStartTime2);
	if(0 >= nEventTimeMinute)
	{// 2007-10-31 by cmkwon, PCBang HappyHourEvent �Ⱓ�� ������ �ִ�
		return;
	}

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] PCBang HappyHourEvent Start !!, Period(%s ~ %s) ExpRate(%4.2f) SPIRate(%4.2f) ExpRepireRate(%4.2f) DropItemRate(%4.2f) DropRareItemRate(%4.2f) WarPointRate(%4.2f) MinLevel(%d) MaxLevel(%d)\r\n"
		, m_PCBangHappyHourEvent.atimeStartTime2.GetDateTimeString().GetBuffer(), m_PCBangHappyHourEvent.atimeEndTime2.GetDateTimeString().GetBuffer()
		, m_PCBangHappyHourEvent.fEXPRate2, m_PCBangHappyHourEvent.fSPIRate2, m_PCBangHappyHourEvent.fEXPRepairRate2
		, m_PCBangHappyHourEvent.fDropItemRate2, m_PCBangHappyHourEvent.fDropRareItemRate2, m_PCBangHappyHourEvent.fWarPointRate2, m_PCBangHappyHourEvent.MinLevel, m_PCBangHappyHourEvent.MaxLevel);
	
	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-31 by cmkwon, PCBang HappyHourEvent ���� ���� ���� ����
	m_byPCBangHappyEvState		= PCBANG_HAPPYEV_STATE_TYPE_START;
	
	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-31 by cmkwon, GameEvent �� PCBang HappyHourEvent �� ������
	m_pGameEventManager->StartPCBangGameEvent(GAME_EVENT_TYPE_EXP, m_PCBangHappyHourEvent.fEXPRate2, nEventTimeMinute);
	m_pGameEventManager->StartPCBangGameEvent(GAME_EVENT_TYPE_SPI, m_PCBangHappyHourEvent.fSPIRate2, nEventTimeMinute);
	m_pGameEventManager->StartPCBangGameEvent(GAME_EVENT_TYPE_EXP_NO_DOWN, m_PCBangHappyHourEvent.fEXPRepairRate2, nEventTimeMinute);
	m_pGameEventManager->StartPCBangGameEvent(GAME_EVENT_TYPE_DROP_ITEM, m_PCBangHappyHourEvent.fDropItemRate2, nEventTimeMinute);
	m_pGameEventManager->StartPCBangGameEvent(GAME_EVENT_TYPE_DROP_REAR_ITEM, m_PCBangHappyHourEvent.fDropRareItemRate2, nEventTimeMinute);
	m_pGameEventManager->StartPCBangGameEvent(GAME_EVENT_TYPE_WARPOINT, m_PCBangHappyHourEvent.fWarPointRate2, nEventTimeMinute);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-31 by cmkwon, ��� PCBang �������� ����
	SHAPPY_HOUR_EVENT *pHappyEv	= &m_PCBangHappyHourEvent;
	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, pSPCBang, SendBuf);
	pSPCBang->byStateType		= m_byPCBangHappyEvState;
	pSPCBang->byInfluenceType4	= pHappyEv->InfluenceType3;
	pSPCBang->StartATime		= pHappyEv->atimeStartTime2;
	pSPCBang->EndATime			= pHappyEv->atimeEndTime2;
	pSPCBang->fEXPRate2			= pHappyEv->fEXPRate2;
	pSPCBang->fSPIRate2			= pHappyEv->fSPIRate2;
	pSPCBang->fEXPRepairRate2	= pHappyEv->fEXPRepairRate2;
	pSPCBang->fDropItemRate2	= pHappyEv->fDropItemRate2;
	pSPCBang->fDropRareRate2	= pHappyEv->fDropRareItemRate2;
	pSPCBang->fWarPointRate2	= pHappyEv->fWarPointRate2;			// 2007-06-26 by dhjin, ������Ʈ �̺�Ʈ ���� �߰�
	m_pFieldIOCP13->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK), INFLUENCE_TYPE_ALL_MASK, pHappyEv->MinLevel, pHappyEv->MaxLevel, NULL, TRUE);		// 2008-03-13 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� - 
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CHappyHourEventManager::OnDoMinutelyWorkHappyHourEventManagerAllInfluenceType(ATUM_DATE_TIME *pDateTime)
/// \brief		// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - CHappyHourEventManager::OnDoMinutelyWorkHappyHourEventManagerAllInfluenceType() �Լ� �߰�
/// \author		cmkwon
/// \date		2007-10-31 ~ 2007-10-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CHappyHourEventManager::OnDoMinutelyWorkHappyHourEventManagerAllInfluenceType(ATUM_DATE_TIME *pDateTime)
{
	int i=0;
	for(i=0; i < INFLUENCE_TYPE_COUNT; i++)
	{
		SHAPPY_HOUR_EVENT_PERIOD *pHappyEvPeriod = &m_arrPeriodEachInfluenceType[i];
		this->OnDoMinutelyWorkHappyHourEventManagerOfInfluenceType(pDateTime, pHappyEvPeriod);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CHappyHourEventManager::OnDoMinutelyWorkHappyHourEventManagerOfInfluenceType(ATUM_DATE_TIME *pDateTime, SHAPPY_HOUR_EVENT_PERIOD *i_pHappyEvPeriod)
/// \brief		// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - CHappyHourEventManager::OnDoMinutelyWorkHappyHourEventManagerOfInfluenceType() �Լ� �߰�
/// \author		cmkwon
/// \date		2007-10-31 ~ 2007-10-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CHappyHourEventManager::OnDoMinutelyWorkHappyHourEventManagerOfInfluenceType(ATUM_DATE_TIME *pDateTime, SHAPPY_HOUR_EVENT_PERIOD *i_pHappyEvPeriod)
{
	SHAPPY_HOUR_EVENT *pHappyEv = this->GetCurrentHappyHourEventByInfluenceType(i_pHappyEvPeriod->InfluenceType3);

	if(i_pHappyEvPeriod->atimeStartTime2 >= *pDateTime
		|| i_pHappyEvPeriod->atimeEndTime2 <= *pDateTime)
	{// 2007-10-31 by cmkwon, HappyHourEvent �Ⱓ�� �ƴϴ�

		if(pHappyEv)
		{// 2007-10-31 by cmkwon, �������̴� HappyHourEvnet �� �ִ�, HappyHourEvnet ���� ó���ؾ� ��

			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] HappyHourEvent End !!, InfluenceType(%3d) Day(%s:%d) Period(%s ~ %s) ExpRate(%4.2f) SPIRate(%4.2f) ExpRepireRate(%4.2f) DropItemRate(%4.2f) DropRareItemRate(%4.2f) WarPointRate(%4.2f) MinLevel(%3d) MaxLevel(%3d)\r\n"
				, pHappyEv->InfluenceType3, GetDayOfWeekString(pHappyEv->DayOfWeek), pHappyEv->DayOfWeek
				, pHappyEv->atimeStartTime2.GetDateTimeString().GetBuffer(), pHappyEv->atimeEndTime2.GetDateTimeString().GetBuffer()
				, pHappyEv->fEXPRate2, pHappyEv->fSPIRate2, pHappyEv->fEXPRepairRate2
				, pHappyEv->fDropItemRate2, pHappyEv->fDropRareItemRate2, pHappyEv->fWarPointRate2
				, pHappyEv->MinLevel, pHappyEv->MaxLevel);		// 2007-10-31 by cmkwon, �ش� ������ GameEvent �� �ʱ�ȭ �Ѵ�.

			///////////////////////////////////////////////////////////////////////////////
			// 2007-10-31 by cmkwon, �ش� �̺�Ʈ�� �ӽ� ������ �Ҵ�
			SHAPPY_HOUR_EVENT tmHappyEv = *pHappyEv;

			///////////////////////////////////////////////////////////////////////////////
			// 2007-10-31 by cmkwon, �ش� ������ Current HappyHourEvent �� NULL �� �ʱ�ȭ �Ѵ�., pHappyEv ������ ��ȿ���� ����
			this->SetCurrentHappyHourEventByInfluenceType(tmHappyEv.InfluenceType3, NULL);

			///////////////////////////////////////////////////////////////////////////////
			// 2007-10-31 by cmkwon, �ش� ������ GameEvent �� �ʱ�ȭ �Ѵ�.
			m_pGameEventManager->EndGameEventAllByInfluenceType(tmHappyEv.InfluenceType3);

			///////////////////////////////////////////////////////////////////////////////
			// 2007-10-31 by cmkwon, �ش� ������ ��� �������� �����Ѵ�.
			INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, pSHappy, SendBuf);
			pSHappy->byStateType			= HAPPYEV_STATE_TYPE_END;
			pSHappy->byInfluenceType4		= tmHappyEv.InfluenceType3;
			pSHappy->StartATime				= tmHappyEv.atimeStartTime2;
			pSHappy->EndATime				= tmHappyEv.atimeEndTime2;
			pSHappy->fEXPRate2				= tmHappyEv.fEXPRate2;
			pSHappy->fSPIRate2				= tmHappyEv.fSPIRate2;
			pSHappy->fEXPRepairRate2		= tmHappyEv.fEXPRepairRate2;
			pSHappy->fDropItemRate2			= tmHappyEv.fDropItemRate2;
			pSHappy->fDropRareRate2			= tmHappyEv.fDropRareItemRate2;
			pSHappy->fWarPointRate2			= tmHappyEv.fWarPointRate2;					// 2007-06-26 by dhjin, ������Ʈ �̺�Ʈ ���� �߰�
			m_pFieldIOCP13->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK), tmHappyEv.InfluenceType3, tmHappyEv.MinLevel, tmHappyEv.MaxLevel);	// 2008-03-13 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� - 
		}

		return;
	}

	auto localTime = tm(*pDateTime);
	if(pHappyEv)
	{// 2007-10-31 by cmkwon, ���� ���� HappyHourEvent �� �ִ�, ���� üũ �ؾ� ��
		
		pHappyEv->atimeStartTime2.Year	= pHappyEv->atimeEndTime2.Year	= pDateTime->Year;
		pHappyEv->atimeStartTime2.Month	= pHappyEv->atimeEndTime2.Month	= pDateTime->Month;
		pHappyEv->atimeStartTime2.Day	= pHappyEv->atimeEndTime2.Day	= pDateTime->Day;

		if(pHappyEv->DayOfWeek != localTime.tm_wday
			|| pHappyEv->atimeStartTime2 > *pDateTime
			|| pHappyEv->atimeEndTime2 <= *pDateTime)
		{// HappyHourEvent ���� ó�� �ؾ� ��

			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] HappyHourEvent End !!, InfluenceType(%3d) Day(%s:%d) Period(%s ~ %s) ExpRate(%4.2f) SPIRate(%4.2f) ExpRepireRate(%4.2f) DropItemRate(%4.2f) DropRareItemRate(%4.2f) WarPointRate(%4.2f) MinLevel(%3d) MaxLevel(%3d)\r\n"
				, pHappyEv->InfluenceType3, GetDayOfWeekString(pHappyEv->DayOfWeek), pHappyEv->DayOfWeek
				, pHappyEv->atimeStartTime2.GetDateTimeString().GetBuffer(), pHappyEv->atimeEndTime2.GetDateTimeString().GetBuffer()
				, pHappyEv->fEXPRate2, pHappyEv->fSPIRate2, pHappyEv->fEXPRepairRate2
				, pHappyEv->fDropItemRate2, pHappyEv->fDropRareItemRate2, pHappyEv->fWarPointRate2
				, pHappyEv->MinLevel, pHappyEv->MaxLevel);		// 2007-10-31 by cmkwon, �ش� ������ GameEvent �� �ʱ�ȭ �Ѵ�.

			///////////////////////////////////////////////////////////////////////////////
			// 2007-10-31 by cmkwon, �ش� �̺�Ʈ�� �ӽ� ������ �Ҵ�
			SHAPPY_HOUR_EVENT tmHappyEv = *pHappyEv;

			///////////////////////////////////////////////////////////////////////////////
			// 2007-10-31 by cmkwon, �ش� ������ Current HappyHourEvent �� NULL �� �ʱ�ȭ �Ѵ�., pHappyEv ������ ��ȿ���� ����
			this->SetCurrentHappyHourEventByInfluenceType(tmHappyEv.InfluenceType3, NULL);

			///////////////////////////////////////////////////////////////////////////////
			// 2007-10-31 by cmkwon, �ش� ������ GameEvent �� �ʱ�ȭ �Ѵ�.
			m_pGameEventManager->EndGameEventAllByInfluenceType(tmHappyEv.InfluenceType3);

			///////////////////////////////////////////////////////////////////////////////
			// 2007-10-31 by cmkwon, �ش� ������ ��� �������� �����Ѵ�.
			INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, pSHappy, SendBuf);
			pSHappy->byStateType			= HAPPYEV_STATE_TYPE_END;
			pSHappy->byInfluenceType4		= tmHappyEv.InfluenceType3;
			pSHappy->StartATime				= tmHappyEv.atimeStartTime2;
			pSHappy->EndATime				= tmHappyEv.atimeEndTime2;
			pSHappy->fEXPRate2				= tmHappyEv.fEXPRate2;
			pSHappy->fSPIRate2				= tmHappyEv.fSPIRate2;
			pSHappy->fEXPRepairRate2		= tmHappyEv.fEXPRepairRate2;
			pSHappy->fDropItemRate2			= tmHappyEv.fDropItemRate2;
			pSHappy->fDropRareRate2			= tmHappyEv.fDropRareItemRate2;
			pSHappy->fWarPointRate2			= tmHappyEv.fWarPointRate2;					// 2007-06-26 by dhjin, ������Ʈ �̺�Ʈ ���� �߰�
			m_pFieldIOCP13->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK), tmHappyEv.InfluenceType3, tmHappyEv.MinLevel, tmHappyEv.MaxLevel);	// 2008-03-13 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� - 
		}
		return;
	}

	pHappyEv = FindHappyHourEventByInfluenceType8Time(i_pHappyEvPeriod->InfluenceType3, pDateTime, localTime.tm_wday);
	if(NULL == pHappyEv)
	{// 2007-10-31 by cmkwon, ���� �� HappyHourEvent �� ����
		return;
	}

	int nEventTimeMinute	= pHappyEv->atimeEndTime2.GetTimeDiffTimeInMinutes(pHappyEv->atimeStartTime2);
	if(0 >= nEventTimeMinute)
	{// HappyHourEvent ���� �ð� ����
		return;
	}

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] HappyHourEvent Start !!, InfluenceType(%3d) Day(%s:%d) Period(%s ~ %s) ExpRate(%4.2f) SPIRate(%4.2f) ExpRepireRate(%4.2f) DropItemRate(%4.2f) DropRareItemRate(%4.2f) WarPointRate(%4.2f) MinLevel(%3d) MaxLevel(%3d)\r\n"
		, pHappyEv->InfluenceType3, GetDayOfWeekString(pHappyEv->DayOfWeek), pHappyEv->DayOfWeek
		, pHappyEv->atimeStartTime2.GetDateTimeString().GetBuffer(), pHappyEv->atimeEndTime2.GetDateTimeString().GetBuffer()
		, pHappyEv->fEXPRate2, pHappyEv->fSPIRate2, pHappyEv->fEXPRepairRate2
		, pHappyEv->fDropItemRate2, pHappyEv->fDropRareItemRate2, pHappyEv->fWarPointRate2
		, pHappyEv->MinLevel, pHappyEv->MaxLevel);		// 2007-10-31 by cmkwon, �ش� ������ GameEvent �� �ʱ�ȭ �Ѵ�.
	
	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-31 by cmkwon, �ش� ������ Current HappyHourEvent �� �����Ѵ�.
	this->SetCurrentHappyHourEventByInfluenceType(pHappyEv->InfluenceType3, pHappyEv);
	
	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-31 by cmkwon, �ش� ������ Current HappyHourEvent �� GameEvent �� ���� �Ѵ�.
	m_pGameEventManager->StartGameEvent(GAME_EVENT_TYPE_EXP, pHappyEv->fEXPRate2, nEventTimeMinute, pHappyEv->MinLevel, pHappyEv->MaxLevel, pHappyEv->InfluenceType3);		// 2008-03-14 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� -		
	m_pGameEventManager->StartGameEvent(GAME_EVENT_TYPE_SPI, pHappyEv->fSPIRate2, nEventTimeMinute, pHappyEv->MinLevel, pHappyEv->MaxLevel, pHappyEv->InfluenceType3);		// 2008-03-14 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� -
	m_pGameEventManager->StartGameEvent(GAME_EVENT_TYPE_EXP_NO_DOWN, pHappyEv->fEXPRepairRate2, nEventTimeMinute, pHappyEv->MinLevel, pHappyEv->MaxLevel, pHappyEv->InfluenceType3);		// 2008-03-14 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� -
	m_pGameEventManager->StartGameEvent(GAME_EVENT_TYPE_DROP_ITEM, pHappyEv->fDropItemRate2, nEventTimeMinute, pHappyEv->MinLevel, pHappyEv->MaxLevel, pHappyEv->InfluenceType3);			// 2008-03-14 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� -
	m_pGameEventManager->StartGameEvent(GAME_EVENT_TYPE_DROP_REAR_ITEM, pHappyEv->fDropRareItemRate2, nEventTimeMinute, pHappyEv->MinLevel, pHappyEv->MaxLevel, pHappyEv->InfluenceType3);	// 2008-03-14 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� -
	m_pGameEventManager->StartGameEvent(GAME_EVENT_TYPE_WARPOINT, pHappyEv->fWarPointRate2, nEventTimeMinute, pHappyEv->MinLevel, pHappyEv->MaxLevel, pHappyEv->InfluenceType3);			// 2008-03-14 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� -
	
	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-31 by cmkwon, �ش� ������ ��� �������� �����Ѵ�.
	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, pSHappy, SendBuf);
	pSHappy->byStateType			= HAPPYEV_STATE_TYPE_START;
	pSHappy->byInfluenceType4		= pHappyEv->InfluenceType3;
	pSHappy->StartATime				= pHappyEv->atimeStartTime2;
	pSHappy->EndATime				= pHappyEv->atimeEndTime2;
	pSHappy->fEXPRate2				= pHappyEv->fEXPRate2;
	pSHappy->fSPIRate2				= pHappyEv->fSPIRate2;
	pSHappy->fEXPRepairRate2		= pHappyEv->fEXPRepairRate2;
	pSHappy->fDropItemRate2			= pHappyEv->fDropItemRate2;
	pSHappy->fDropRareRate2			= pHappyEv->fDropRareItemRate2;
	pSHappy->fWarPointRate2			= pHappyEv->fWarPointRate2;
	m_pFieldIOCP13->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK), pHappyEv->InfluenceType3, pHappyEv->MinLevel, pHappyEv->MaxLevel);		// 2008-03-13 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� - 
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SHAPPY_HOUR_EVENT *CHappyHourEventManager::GetCurrentHappyHourEventByInfluenceType(BYTE i_byInflTy)
/// \brief		// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - CHappyHourEventManager::GetCurrentHappyHourEventByInfluenceType() �Լ� �߰� ��
/// \author		cmkwon
/// \date		2007-10-31 ~ 2007-10-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SHAPPY_HOUR_EVENT *CHappyHourEventManager::GetCurrentHappyHourEventByInfluenceType(BYTE i_byInflTy)
{
	int nArrIdx = GetArrayIndexByInfluenceType(i_byInflTy);
	if(0 > nArrIdx)
	{
		return NULL;
	}
	return m_pArrCurHappyHourEventEachInfluenceType[nArrIdx];
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CHappyHourEventManager::SetCurrentHappyHourEventByInfluenceType(BYTE i_byInflTy, SHAPPY_HOUR_EVENT *i_pHappyEv)
/// \brief		// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - CHappyHourEventManager::SetCurrentHappyHourEventByInfluenceType() �Լ� �߰� ��
/// \author		cmkwon
/// \date		2007-10-31 ~ 2007-10-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CHappyHourEventManager::SetCurrentHappyHourEventByInfluenceType(BYTE i_byInflTy, SHAPPY_HOUR_EVENT *i_pHappyEv)
{
	int nArrIdx = GetArrayIndexByInfluenceType(i_byInflTy);
	if(0 > nArrIdx)
	{
		return FALSE;
	}
	
	m_pArrCurHappyHourEventEachInfluenceType[nArrIdx] = i_pHappyEv;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SHAPPY_HOUR_EVENT *CHappyHourEventManager::FindHappyHourEventByInfluenceType8Time(BYTE i_byInflTy, ATUM_DATE_TIME *pCurDateTime, int i_CurDayOfWeek)
/// \brief		// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - CHappyHourEventManager::FindHappyHourEventByInfluenceType8Time() �Լ� �߰�
/// \author		cmkwon
/// \date		2007-10-31 ~ 2007-10-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SHAPPY_HOUR_EVENT *CHappyHourEventManager::FindHappyHourEventByInfluenceType8Time(BYTE i_byInflTy, ATUM_DATE_TIME *pCurDateTime, int i_CurDayOfWeek)
{
	for(int i=0; i < m_mtvectHappyHourEventList.size(); i++)
	{
		SHAPPY_HOUR_EVENT *pHappyEv = &m_mtvectHappyHourEventList[i];
		if(i_byInflTy != pHappyEv->InfluenceType3)
		{// 2007-10-31 by cmkwon, ���� üũ
			continue;
		}

		// 2007-10-31 by cmkwon, ���� ����(��~��)�� üũ �ؾ���
		switch(pHappyEv->DayOfWeek)
		{
		case DAY_OF_WEEK_SUMDAY:
		case DAY_OF_WEEK_MONDAY:
		case DAY_OF_WEEK_TUESDAY:
		case DAY_OF_WEEK_WEDNESDAY:
		case DAY_OF_WEEK_THURSDAY:	
		case DAY_OF_WEEK_FRIDAY:
		case DAY_OF_WEEK_SATURDAY:
			break;
		default:
			{
				continue;		// 2007-10-31 by cmkwon, ������ �ƴ� �����ʹ� üũ �ʿ� ����
			}
		}
		
		if(pHappyEv->DayOfWeek != i_CurDayOfWeek)
		{// 2007-10-31 by cmkwon, ������ ������ üũ
			continue;
		}

		pHappyEv->atimeStartTime2.Year	= pHappyEv->atimeEndTime2.Year	= pCurDateTime->Year;
		pHappyEv->atimeStartTime2.Month	= pHappyEv->atimeEndTime2.Month	= pCurDateTime->Month;
		pHappyEv->atimeStartTime2.Day	= pHappyEv->atimeEndTime2.Day	= pCurDateTime->Day;

		if(pHappyEv->atimeStartTime2 > *pCurDateTime
			|| pHappyEv->atimeEndTime2 <= *pCurDateTime)
		{// 2007-10-31 by cmkwon, �ð��� üũ
			continue;
		}

		return pHappyEv;	// 2007-10-31 by cmkwon, ��ȿ�� HappyHourEvent �� ����
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CHappyHourEventManager::SendHappyHourEventInfo(CFieldIOCPSocket *i_pToFISock, BYTE i_byInflTy, MessageType_t i_msgTy)
/// \brief		// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - CHappyHourEventManager::SendHappyHourEventInfo() �Լ� �߰� ��
///				// 2008-05-20 by cmkwon, ��� �̺�Ʈ(HappyHoure,MotherShip,Item) �׷� ���ÿ� �����ϰ� - CHappyHourEventManager::SendHappyHourEventInfo() ���� �߰�
/// \author		cmkwon
/// \date		2007-10-31 ~ 2007-10-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CHappyHourEventManager::SendHappyHourEventInfo(CFieldIOCPSocket *i_pToFISock, BYTE i_byInflTy, MessageType_t i_msgTy)
{
	mt_auto_lock mtHappy(this->GetVectHappyHourEventListPtr());	// 2006-08-23 by cmkwon

	if(FALSE == i_pToFISock->IsValidCharacter(FALSE))
	{
		return FALSE;
	}

	BOOL bRet = FALSE;
	
	// 2008-05-20 by cmkwon, ��� �̺�Ʈ(HappyHoure,MotherShip,Item) �׷� ���ÿ� �����ϰ� - �Ʒ��� ���� i_msgTy �� ���ڷ� �޾Ƽ� ���� �Ѵ�.
	//INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, pSHappyEv, SendBuf);
	
	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, 0, pSHappyEv, SendBuf);		// 2008-05-20 by cmkwon, ��� �̺�Ʈ(HappyHoure,MotherShip,Item) �׷� ���ÿ� �����ϰ� - 
	*(MessageType_t*)SendBuf = i_msgTy;

	//if(i_pToFISock->m_bIsPCBangClient
	//	&& PCBANG_HAPPYEV_STATE_TYPE_END != m_byPCBangHappyEvState)
	//{
	//	SHAPPY_HOUR_EVENT *pHappyEv		= &m_PCBangHappyHourEvent;		

	//	bRet							= TRUE;

	//	pSHappyEv->byStateType			= PCBANG_HAPPYEV_STATE_TYPE_STARTING;
	//	pSHappyEv->byInfluenceType4		= pHappyEv->InfluenceType3;
	//	pSHappyEv->StartATime			= pHappyEv->atimeStartTime2;
	//	pSHappyEv->EndATime				= pHappyEv->atimeEndTime2;
	//	pSHappyEv->fEXPRate2			= pHappyEv->fEXPRate2;
	//	pSHappyEv->fSPIRate2			= pHappyEv->fSPIRate2;
	//	pSHappyEv->fEXPRepairRate2		= pHappyEv->fEXPRepairRate2;
	//	pSHappyEv->fDropItemRate2		= pHappyEv->fDropItemRate2;
	//	pSHappyEv->fDropRareRate2		= pHappyEv->fDropRareItemRate2;
	//	pSHappyEv->fWarPointRate2		= pHappyEv->fWarPointRate2;
	//	i_pToFISock->SendAddData(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK));
	//}

	int i=0;
	for(i=0; i < INFLUENCE_TYPE_COUNT; i++)
	{
		SHAPPY_HOUR_EVENT *pHappyEv = m_pArrCurHappyHourEventEachInfluenceType[i];
		if(NULL == pHappyEv
			|| FALSE == util::in_range(pHappyEv->MinLevel, i_pToFISock->m_character.Level, pHappyEv->MaxLevel))
		{// 2008-03-13 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� - ���� üũ �߰�
			continue;
		}

		if(INFLUENCE_TYPE_ALL_MASK == pHappyEv->InfluenceType3
			|| i_pToFISock->GetCharacter()->InfluenceType == pHappyEv->InfluenceType3)
		{
			bRet							= TRUE;

			pSHappyEv->byStateType			= HAPPYEV_STATE_TYPE_STARTING;
			pSHappyEv->byInfluenceType4		= pHappyEv->InfluenceType3;
			pSHappyEv->StartATime			= pHappyEv->atimeStartTime2;
			pSHappyEv->EndATime				= pHappyEv->atimeEndTime2;
			pSHappyEv->fEXPRate2			= pHappyEv->fEXPRate2;
			pSHappyEv->fSPIRate2			= pHappyEv->fSPIRate2;
			pSHappyEv->fEXPRepairRate2		= pHappyEv->fEXPRepairRate2;
			pSHappyEv->fDropItemRate2		= pHappyEv->fDropItemRate2;
			pSHappyEv->fDropRareRate2		= pHappyEv->fDropRareItemRate2;
			pSHappyEv->fWarPointRate2		= pHappyEv->fWarPointRate2;
			i_pToFISock->SendAddData(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK));
		}
	}

	return bRet;
}


//////////////////////////////////////////////////////////////////////
// 2012-10-05 by jhseol, NPlay PC�� ��� - ���Ǿƿ� ���� ��������
///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CHappyHourEventManager::LoadHappyHourEventInfo(CFieldIOCPSocket *i_pToFISock, BYTE i_byInflTy, MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK* i_msg)
/// \brief		���Ǿƿ� ���� ��������
/// \author		jhseol
/// \date		2012-10-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CHappyHourEventManager::LoadHappyHourEventInfo(CFieldIOCPSocket *i_pToFISock, BYTE i_byInflTy, MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK* i_msg)
{
	mt_auto_lock mtHappy(this->GetVectHappyHourEventListPtr());	// 2006-08-23 by cmkwon
	
	if(FALSE == i_pToFISock->IsValidCharacter(FALSE))
	{
		return FALSE;
	}
	
	BOOL bRet = FALSE;
	
	//if(i_pToFISock->m_bIsPCBangClient)
	//{
	//	if( PCBANG_HAPPYEV_STATE_TYPE_END != m_byPCBangHappyEvState)
	//	{
	//		SHAPPY_HOUR_EVENT *pHappyEv		= &m_PCBangHappyHourEvent;		
	//		bRet							= TRUE;
	//		i_msg->byStateType			= PCBANG_HAPPYEV_STATE_TYPE_STARTING;
	//		i_msg->byInfluenceType4		= pHappyEv->InfluenceType3;
	//		i_msg->StartATime			= pHappyEv->atimeStartTime2;
	//		i_msg->EndATime				= pHappyEv->atimeEndTime2;
	//		i_msg->fEXPRate2			= pHappyEv->fEXPRate2;
	//		i_msg->fSPIRate2			= pHappyEv->fSPIRate2;
	//		i_msg->fEXPRepairRate2		= pHappyEv->fEXPRepairRate2;
	//		i_msg->fDropItemRate2		= pHappyEv->fDropItemRate2;
	//		i_msg->fDropRareRate2		= pHappyEv->fDropRareItemRate2;
	//		i_msg->fWarPointRate2		= pHappyEv->fWarPointRate2;
	//	}
	//	else
	//	{
	//		i_msg->byStateType			= PCBANG_HAPPYEV_STATE_TYPE_END;
	//	}
	//}
	//else
	//{
		i_msg->byStateType			= PCBANG_HAPPYEV_STATE_TYPE_END;
	//}
	
	return bRet;
}
// 2012-10-05 by jhseol, NPlay PC�� ��� - ���Ǿƿ� ���� ��������

// 2008-05-20 by cmkwon, ��� �̺�Ʈ(HappyHoure,MotherShip,Item) �׷� ���ÿ� �����ϰ� - CHappyHourEventManager::SendHappyHourEventInfo() �Լ��� i_msgTy ���ڸ� �߰��Ͽ� ó����. �� �Լ��� �ʿ� ����
// ///////////////////////////////////////////////////////////////////////////////
// /// \fn			BOOL CHappyHourEventManager::SendHappyHourEventInfoByLevel(CFieldIOCPSocket *i_pToFISock, BYTE i_byInflTy)
// /// \brief		Level�� ���Ǿƿ� �̺�Ʈ ���� -	
// /// \author		dhjin
// /// \date		2008-03-14 ~ 2008-03-14
// /// \warning	
// ///
// /// \param		
// /// \return		
// ///////////////////////////////////////////////////////////////////////////////
// BOOL CHappyHourEventManager::SendHappyHourEventInfoByLevel(CFieldIOCPSocket *i_pToFISock, BYTE i_byInflTy)
// {
// 	mt_auto_lock mtHappy(this->GetVectHappyHourEventListPtr());	// 2006-08-23 by cmkwon
// 
// 	if(FALSE == i_pToFISock->IsValidCharacter(FALSE))
// 	{
// 		return FALSE;
// 	}
// 
// 	BOOL bRet = FALSE;
// 	
// 	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_BY_LEVEL, pSHappyEv, SendBuf);
// 	if(i_pToFISock->m_bIsPCBangClient
// 		&& PCBANG_HAPPYEV_STATE_TYPE_END != m_byPCBangHappyEvState)
// 	{
// 		SHAPPY_HOUR_EVENT *pHappyEv		= &m_PCBangHappyHourEvent;		
// 
// 		bRet							= TRUE;
// 
// 		pSHappyEv->byStateType			= PCBANG_HAPPYEV_STATE_TYPE_STARTING;
// 		pSHappyEv->byInfluenceType4		= pHappyEv->InfluenceType3;
// 		pSHappyEv->StartATime			= pHappyEv->atimeStartTime2;
// 		pSHappyEv->EndATime				= pHappyEv->atimeEndTime2;
// 		pSHappyEv->fEXPRate2			= pHappyEv->fEXPRate2;
// 		pSHappyEv->fSPIRate2			= pHappyEv->fSPIRate2;
// 		pSHappyEv->fEXPRepairRate2		= pHappyEv->fEXPRepairRate2;
// 		pSHappyEv->fDropItemRate2		= pHappyEv->fDropItemRate2;
// 		pSHappyEv->fDropRareRate2		= pHappyEv->fDropRareItemRate2;
// 		pSHappyEv->fWarPointRate2		= pHappyEv->fWarPointRate2;
// 		i_pToFISock->SendAddData(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK));
// 	}
// 
// 	int i=0;
// 	for(i=0; i < INFLUENCE_TYPE_COUNT; i++)
// 	{
// 		SHAPPY_HOUR_EVENT *pHappyEv = m_pArrCurHappyHourEventEachInfluenceType[i];
// 		if(NULL == pHappyEv
// 			|| FALSE == util::in_range(pHappyEv->MinLevel, i_pToFISock->m_character.Level, pHappyEv->MaxLevel))
// 		{// 2008-03-13 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� - ���� üũ �߰�
// 			continue;
// 		}
// 
// 		if(INFLUENCE_TYPE_ALL_MASK == pHappyEv->InfluenceType3
// 			|| i_pToFISock->GetCharacter()->InfluenceType == pHappyEv->InfluenceType3)
// 		{
// 			bRet							= TRUE;
// 
// 			pSHappyEv->byStateType			= HAPPYEV_STATE_TYPE_STARTING;
// 			pSHappyEv->byInfluenceType4		= pHappyEv->InfluenceType3;
// 			pSHappyEv->StartATime			= pHappyEv->atimeStartTime2;
// 			pSHappyEv->EndATime				= pHappyEv->atimeEndTime2;
// 			pSHappyEv->fEXPRate2			= pHappyEv->fEXPRate2;
// 			pSHappyEv->fSPIRate2			= pHappyEv->fSPIRate2;
// 			pSHappyEv->fEXPRepairRate2		= pHappyEv->fEXPRepairRate2;
// 			pSHappyEv->fDropItemRate2		= pHappyEv->fDropItemRate2;
// 			pSHappyEv->fDropRareRate2		= pHappyEv->fDropRareItemRate2;
// 			pSHappyEv->fWarPointRate2		= pHappyEv->fWarPointRate2;
// 			i_pToFISock->SendAddData(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK));
// 		}
// 	}
// 
// 	return bRet;
// }

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CHappyHourEventManager::SetHappyHourEventPeriod(BYTE i_byInflTy, ATUM_DATE_TIME i_atimeStart, ATUM_DATE_TIME i_atimeEnd, BOOL i_bStoreDB/*=FALSE*/)
/// \brief		// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - CHappyHourEventManager::SetHappyHourEventPeriod() �Լ� �߰�
/// \author		cmkwon
/// \date		2007-11-01 ~ 2007-11-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CHappyHourEventManager::SetHappyHourEventPeriod(BYTE i_byInflTy, ATUM_DATE_TIME i_atimeStart, ATUM_DATE_TIME i_atimeEnd, BOOL i_bStoreDB/*=FALSE*/)
{
	int nArrIdx = GetArrayIndexByInfluenceType(i_byInflTy);
	if(0 > nArrIdx)
	{
		return FALSE;
	}
	
	SHAPPY_HOUR_EVENT_PERIOD *pHappyEvPeriod = &m_arrPeriodEachInfluenceType[nArrIdx];
	
	pHappyEvPeriod->atimeStartTime2			= i_atimeStart;
	pHappyEvPeriod->atimeEndTime2			= i_atimeEnd;


	if(i_bStoreDB)
	{
		QPARAM_UPDATE_HAPPY_HOUR_EVENT_DATE *pQParam = new QPARAM_UPDATE_HAPPY_HOUR_EVENT_DATE;
		pQParam->byInfluenceType0	= i_byInflTy;
		pQParam->atimeStartTime2	= i_atimeStart;
		pQParam->atimeEndTime2		= i_atimeStart;
		m_pFieldIOCP13->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateHappyHourEventDate, NULL, 0, pQParam);
	}
	return TRUE;
}