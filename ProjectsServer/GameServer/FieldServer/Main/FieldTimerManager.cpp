#include "stdafx.h"
#include "AtumProtocol.h"
#include "AtumError.h"
#include "FieldTimerManager.h"
#include "FieldIOCPSocket.h"
#include "FieldItemManager.h"
#include "FieldParty.h"

CFieldTimerManager::CFieldTimerManager()
{
	ResetTimerData();
}

void CFieldTimerManager::ResetTimerData()
{
	// Timer Events ����
	m_listWatingTimerEvents.clearLock();

	m_dwDoMinutelyCounts			= 0;
	
	m_bRecoverHPTimerRunning		= FALSE;
	m_dwRecoverHPRunningTick		= 0;
	m_uiHPTimerUID					= 0;			// 2006-09-27 by cmkwon

	m_bRecoverDPTimerRunning		= FALSE;
	m_dwRecoverDPRunningTick		= 0;
	m_uiDPTimerUID					= 0;			// 2006-09-27 by cmkwon

	m_bRecoverSPTimerRunning		= FALSE;
	m_dwRecoverSPRunningTick		= 0;
	m_uiSPTimerUID					= 0;			// 2006-09-27 by cmkwon
	
	m_bDoMinutelyWorkRunning		= FALSE;
	m_bImmediateHPTimerRunning		= FALSE;
	m_bImmediateDPTimerRunning		= FALSE;
	m_bImmediateSPTimerRunning		= FALSE;
	m_bImmediateEPTimerRunning		= FALSE;

	m_nRemainedTimeOfGradualHPUP	= 0;
	m_nRemainedTimeOfGradualDPUP	= 0;
	m_nRemainedTimeOfGradualSPUP	= 0;
	m_nRemainedTimeOfGradualEPUP	= 0;


	m_TEIDGradualHPUP = NULL;

	m_fSumDistanceForDPConsumption = 0.0f;
	m_fSumDistanceForEPConsumption = 0.0f;
	m_fDPRecoveryWeight = 1;
	m_EPConsumptionTimeStamp = 0;
}

#define AUTO_DP_RECOVERY_MULTIPLIER		2.5f
void CFieldTimerManager::HandleTimerEventRecoverHP(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime)
{
	///////////////////////////////////////////////////////////////////////////////
	// 2006-09-27 by cmkwon, TimerUID üũ
//	DBGOUT("HandleTimerEventRecoverHP uiTimerUID(%d) == pTimerEventUID(%d)\r\n", m_uiHPTimerUID, pTimerEvent->TimerUID);
	if(m_uiHPTimerUID != pTimerEvent->TimerUID){					return;}

	if(m_pCharacter->HP <= m_pCharacter->CurrentHP
		|| 0.0f >= m_pCharacter->CurrentHP
		|| COMPARE_BODYCON_BIT(m_pCharacter->BodyCondition, BODYCON_DEAD_MASK))
	{// check: HP�� ���̸� Ÿ�̸� ����
		m_bRecoverHPTimerRunning		= FALSE;		// 2006-09-27 by cmkwon
		return;
	}

	if(NULL == m_pFieldIOCPSocket->m_pCurrentFieldMapChannel)
	{
		m_bRecoverHPTimerRunning		= FALSE;		// 2006-09-27 by cmkwon
		m_pFieldIOCPSocket->SendString128(STRING_128_DEBUG_L3, STRMSG_S_F2NOTIFY_0142);
		return;
	}

	float fHPRepairRate = 1.0f + m_pFieldIOCPSocket->GetHPRepairRateFlighting();	// 2006-10-24 by cmkwon, ȸ������ �׻�(����, ����) ����Ǿ�� �Ѵ�.
	if(FALSE == m_pCharacter->CharacterMode)
	{// ĳ���͸�尡 �ƴ� ���

		if ( FALSE == COMPARE_BODYCON_BIT(m_pCharacter->BodyCondition, BODYCON_LANDED_MASK)
			|| (IS_AGEAR(m_pCharacter->UnitKind) && COMPARE_BODYCON_BIT(m_pCharacter->BodyCondition, BODYCON_SIEGE_ON_MASK))
			)
		{// ���� ���°� �ƴ�
			fHPRepairRate = m_pFieldIOCPSocket->GetHPRepairRateFlighting();
			if(0.0f >= fHPRepairRate)
			{// 2006-03-30 by cmkwon, ����� HP ȸ���� üũ
				m_bRecoverHPTimerRunning	= CheckRecoveryTimer(TE_TYPE_RECOVER_HP, FALSE);
				return;
			}
		}

		TILEINFO *pTile = m_pFieldIOCPSocket->m_pCurrentFieldMapChannel->m_pFieldMapProject->GetTileInfo(&m_pCharacter->PositionVector);
		if ( NULL == pTile
			|| (FALSE == m_pFieldIOCPSocket->m_pCurrentFieldMapChannel->m_pFieldMapProject->m_bAutoDPRecovery && FALSE == pTile->m_bEnableLand)
			)
		{// ���������� �ƴ�
			fHPRepairRate = m_pFieldIOCPSocket->GetHPRepairRateFlighting();
			if(0.0f >= fHPRepairRate)
			{// 2006-03-30 by cmkwon, ����� HP ȸ���� üũ
				m_bRecoverHPTimerRunning	= CheckRecoveryTimer(TE_TYPE_RECOVER_HP);
				return;
			}
		}
	}
	
	///////////////////////////////////////////////////////////////////////////////
	// 2005-11-09 by cmkwon, ���� ���¿��� 1�ʴ� 9�� ȸ��
	float	fChangeValueHP	= 27.0f * fHPRepairRate;		// 2006-03-30 by cmkwon, ����� HP ȸ���� ó�� �߰�

	m_pCharacter->CurrentHP = min(m_pCharacter->HP, m_pCharacter->CurrentHP+fChangeValueHP);
	if(m_pCharacter->CurrentHP >= m_pCharacter->HP)
	{
		m_pCharacter->CurrentHP			= m_pCharacter->HP;
		m_pFieldIOCPSocket->SendCharacterInfo(T_FC_CHARACTER_CHANGE_CURRENTHP);
		m_bRecoverHPTimerRunning		= FALSE;		// 2006-09-27 by cmkwon
		return;
	}
	
	m_pFieldIOCPSocket->SendCharacterInfo(T_FC_CHARACTER_CHANGE_CURRENTHP);
	m_bRecoverHPTimerRunning		= CheckRecoveryTimer(TE_TYPE_RECOVER_HP, FALSE);		// reset
}

void CFieldTimerManager::HandleTimerEventRecoverDP(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime)
{
	if (m_uiDPTimerUID != pTimerEvent->TimerUID) return;

	if (m_pCharacter->DP <= m_pCharacter->CurrentDP || COMPARE_BODYCON_BIT(m_pCharacter->BodyCondition, BODYCON_DEAD_MASK))
	{
		m_bRecoverDPTimerRunning		= FALSE;		// 2006-09-27 by cmkwon
		return;
	}

	if (!m_pFieldIOCPSocket->m_pCurrentFieldMapChannel)
	{
		m_bRecoverDPTimerRunning		= FALSE;		// 2006-09-27 by cmkwon
		return;
	}

	//float	fChangeValueDP	= 14.0f;
	//if(m_pCharacter->CharacterMode)
	//{// ĳ���͸�� �϶�
	//	fChangeValueDP	= 27.0f;
	//}
	//else if (COMPARE_BODYCON_BIT(m_pCharacter->BodyCondition, BODYCON_LANDED_MASK)
	//	&& FALSE == COMPARE_BODYCON_BIT(m_pCharacter->BodyCondition, BODYCON_SIEGE_ON_MASK))
	//{// �������� �̸鼭 �����尡 �ƴϸ�
	//	TILEINFO *pTile = m_pFieldIOCPSocket->m_pCurrentFieldMapChannel->m_pFieldMapProject->GetTileInfo(&m_pCharacter->PositionVector);
	//	if ( m_pFieldIOCPSocket->m_pCurrentFieldMapChannel->m_pFieldMapProject->m_bAutoDPRecovery
	//		|| (pTile && pTile->m_bEnableLand)
	//		)
	//	{// �ڵ�ȸ�� ���̰ų� �������� �ȿ� ���� ���
	//		fChangeValueDP	= 27.0f;
	//	}
	//}

	// todo : here we are, static shield regen

	m_pCharacter->CurrentDP += 135;

	m_pCharacter->CurrentDP = min(m_pCharacter->DP, m_pCharacter->CurrentDP);

	if (m_pCharacter->CurrentDP >= m_pCharacter->DP)
	{
		m_pCharacter->CurrentDP			= m_pCharacter->DP;
		m_pFieldIOCPSocket->SendCharacterInfo(T_FC_CHARACTER_CHANGE_CURRENTDP);
		m_bRecoverDPTimerRunning		= FALSE;		// 2006-09-27 by cmkwon
	}
	else
	{
		m_pFieldIOCPSocket->SendCharacterInfo(T_FC_CHARACTER_CHANGE_CURRENTDP);
		m_bRecoverDPTimerRunning = CheckRecoveryTimer(TE_TYPE_RECOVER_DP, FALSE);		// reset
	}
}

void CFieldTimerManager::HandleTimerEventSequential(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime)
{
	if (COMPARE_BODYCON_BIT(m_pCharacter->BodyCondition, BODYCON_DEAD_MASK)
		|| m_pCharacter->CurrentHP <= 0.0f)
	{// �ʱ�ȭ
		m_nRemainedTimeOfGradualHPUP	= 0;		
		m_TEIDGradualHPUP				= NULL;
		return;
	}

	if (pTimerEvent->Type == TE_TYPE_GRADUAL_HP_UP)
	{
		if (m_pCharacter->HP <= m_pCharacter->CurrentHP)
		{
			m_nRemainedTimeOfGradualHPUP = 0;
			// �ʱ�ȭ
			m_TEIDGradualHPUP = NULL;
			return;
		}

//		if (COMPARE_BODYCON_BIT(m_pCharacter->BodyCondition, BODYCON_BOOSTER3_MASK))
//		{
//			m_nRemainedTimeOfGradualHPUP = 0;
//			// �ʱ�ȭ
//			m_TEIDGradualHPUP = NULL;
//#ifdef _DEBUG
//			m_pFieldIOCPSocket->SendString128(STRING_128_DEBUG_L3, "Gradual HP UP ����, by BOOSTER3");
//#endif
//			return;
//		}
//
//		// �ܾ�(������, �ν���, ����)�� ���� �ν��� ����
//		if (m_nRemainedTimeOfGradualHPUP <= 0)
//		{
//			m_pFieldIOCPSocket->SendString128(STRING_128_DEBUG_L3, "�ܾп� ���� Gradual HP UP ����(���� �ð�: %d)", m_nRemainedTimeOfGradualHPUP);
//			m_nRemainedTimeOfGradualHPUP = 0;	// �ʱ�ȭ
//			// �ʱ�ȭ
//			m_TEIDGradualHPUP = NULL;
//			return;
//		}

		// ���� �ð� ����
		m_nRemainedTimeOfGradualHPUP -= TIMER_GRADUAL_UP_INTERVAL;

		// HP UP
		ITEM *pItemInfo = (ITEM*)pTimerEvent->nGeneralParam1;
// 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - �Ʒ��� ���� ����
//		m_pCharacter->CurrentHP += (pItemInfo->ParameterValue1) / (pItemInfo->Time/TIMER_GRADUAL_UP_INTERVAL);
		m_pCharacter->CurrentHP += (pItemInfo->ArrParameterValue[0]) / (pItemInfo->Time/TIMER_GRADUAL_UP_INTERVAL);

		if (m_pCharacter->CurrentHP >= m_pCharacter->HP)
		{
			m_pCharacter->CurrentHP = m_pCharacter->HP;
		}
		m_pFieldIOCPSocket->SendCharacterInfo(T_FC_CHARACTER_CHANGE_CURRENTHP);
#ifdef _DEBUG
// 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - �Ʒ��� ���� ����
//		m_pFieldIOCPSocket->SendString128(STRING_128_DEBUG_L3, STRMSG_S_F2NOTIFY_0144, (pItemInfo->ParameterValue1) / (pItemInfo->Time/TIMER_GRADUAL_UP_INTERVAL));
		m_pFieldIOCPSocket->SendString128(STRING_128_DEBUG_L3, STRMSG_S_F2NOTIFY_0144, (pItemInfo->ArrParameterValue[0]) / (pItemInfo->Time/TIMER_GRADUAL_UP_INTERVAL));
#endif

		if (m_nRemainedTimeOfGradualHPUP <= 0)
		{
			m_pFieldIOCPSocket->SendString128(STRING_128_DEBUG_L3, STRMSG_S_F2NOTIFY_0145, m_nRemainedTimeOfGradualHPUP);
			m_nRemainedTimeOfGradualHPUP = 0;	// �ʱ�ȭ
			// �ʱ�ȭ
			m_TEIDGradualHPUP = NULL;
			return;
		}
		else
		{
			m_TEIDGradualHPUP = StartTimerField(TE_TYPE_GRADUAL_HP_UP, TIMER_GRADUAL_UP_INTERVAL, (UINT)pItemInfo);
		}
	}
	else if (pTimerEvent->Type == TE_TYPE_GRADUAL_DP_UP)
	{
		if (m_pCharacter->DP <= m_pCharacter->CurrentDP)
		{
			m_nRemainedTimeOfGradualDPUP = 0;
			return;
		}

		// ���� �ð� ����
		m_nRemainedTimeOfGradualDPUP -= TIMER_GRADUAL_UP_INTERVAL;

		// DP UP
		ITEM *pItemInfo = (ITEM*)pTimerEvent->nGeneralParam1;
// 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - �Ʒ��� ���� ����
//		m_pCharacter->CurrentDP += (pItemInfo->ParameterValue1) / (pItemInfo->Time/TIMER_GRADUAL_UP_INTERVAL);
		m_pCharacter->CurrentDP += (pItemInfo->ArrParameterValue[0]) / (pItemInfo->Time/TIMER_GRADUAL_UP_INTERVAL);

		if (m_pCharacter->CurrentDP >= m_pCharacter->DP)
		{
			m_pCharacter->CurrentDP = m_pCharacter->DP;
		}
		m_pFieldIOCPSocket->SendCharacterInfo(T_FC_CHARACTER_CHANGE_CURRENTDP);

		if (m_nRemainedTimeOfGradualDPUP <= 0)
		{
			m_pFieldIOCPSocket->SendString128(STRING_128_DEBUG_L3, STRMSG_S_F2NOTIFY_0146, m_nRemainedTimeOfGradualDPUP);
			m_nRemainedTimeOfGradualDPUP = 0;	// �ʱ�ȭ
			return;
		}
		else
		{
			StartTimerField(TE_TYPE_GRADUAL_DP_UP, TIMER_GRADUAL_UP_INTERVAL, (UINT)pItemInfo);
		}
	}
	else if (pTimerEvent->Type == TE_TYPE_GRADUAL_EP_UP)
	{
		if (m_pCharacter->EP <= m_pCharacter->CurrentEP)
		{
			m_nRemainedTimeOfGradualEPUP = 0;
			return;
		}

		// ���� �ð� ����
		m_nRemainedTimeOfGradualEPUP -= TIMER_GRADUAL_UP_INTERVAL;

		// EP UP
		ITEM *pItemInfo = (ITEM*)pTimerEvent->nGeneralParam1;
// 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - �Ʒ��� ���� ����
//		m_pFieldIOCPSocket->ChangeCurrentEP((pItemInfo->ParameterValue1) / (pItemInfo->Time/TIMER_GRADUAL_UP_INTERVAL));
		m_pFieldIOCPSocket->ChangeCurrentEP((pItemInfo->ArrParameterValue[0]) / (pItemInfo->Time/TIMER_GRADUAL_UP_INTERVAL));

		if (m_nRemainedTimeOfGradualEPUP <= 0)
		{
			m_pFieldIOCPSocket->SendString128(STRING_128_DEBUG_L3, STRMSG_S_F2NOTIFY_0147, m_nRemainedTimeOfGradualEPUP);
			m_nRemainedTimeOfGradualEPUP = 0;	// �ʱ�ȭ
			return;
		}
		else
		{
			StartTimerField(TE_TYPE_GRADUAL_EP_UP, TIMER_GRADUAL_UP_INTERVAL, (UINT)pItemInfo);
		}
	}
	else if (pTimerEvent->Type = TE_TYPE_GRADUAL_SHIELD_UP)
	{
		if (!COMPARE_BODYCON_BIT(m_pCharacter->BodyCondition, BODYCON_SHIELD_MASK))
		{
			return;
		}

		if (m_pFieldIOCPSocket->m_ItemWingOut.Kind != ITEMKIND_SHIELD)
		{
			m_pFieldIOCPSocket->StopItemKindShield(NULL);
			return;
		}

		// lock m_mapItemGeneral, @HandleTimerEventSequential
		mt_auto_lock igLock(&m_pFieldIOCPSocket->m_ItemManager.m_mapItemGeneral);

		ITEM_GENERAL *pItemGeneral
			= m_pFieldIOCPSocket->m_ItemManager.GetItemGeneralByUID(MAKEUINT64(pTimerEvent->nGeneralParam2, pTimerEvent->nGeneralParam1));
		if (pItemGeneral == NULL)
		{
			m_pFieldIOCPSocket->StopItemKindShield(NULL);
			return;
		}

		ITEM *pItemInfo = pItemGeneral->ItemInfo;
// 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - �ּ� ó����.
//#ifdef _DEBUG
//		ASSERT_ASSERT(pItemInfo->DestParameter1 == DES_GRADUAL_DP_UP);
//		ASSERT_ASSERT(pItemInfo->Kind == ITEMKIND_SHIELD);
//#endif

		// DP�� �� á���� ������ϰ� ����
		if (m_pCharacter->DP <= m_pCharacter->CurrentDP)
		{
			// �����
			StartTimerField(TE_TYPE_GRADUAL_SHIELD_UP, TIMER_GRADUAL_UP_INTERVAL, pTimerEvent->nGeneralParam1, pTimerEvent->nGeneralParam2, pTimerEvent->nGeneralParam3);
			return;
		}

		// ���� �������� ���� �����ϸ� ����
		if (pItemGeneral->CurrentCount <= 0)
		{
			m_pFieldIOCPSocket->StopItemKindShield(pItemInfo);
			return;
		}

		// �������� ���� �� ����
// 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - �Ʒ��� ���� ����
//		pItemGeneral->CurrentCount -= min(pItemInfo->ParameterValue1, pItemGeneral->CurrentCount);
		pItemGeneral->CurrentCount -= min(pItemInfo->ArrParameterValue[0], pItemGeneral->CurrentCount);

#ifdef _DEBUG
		m_pFieldIOCPSocket->SendString128Static(STRING_128_ADMIN_CMD, "%s: %d",
							pItemInfo->ItemName, pItemGeneral->CurrentCount);
#endif

		INIT_MSG_WITH_BUFFER(MSG_FC_STORE_UPDATE_ITEM_COUNT, T_FC_STORE_UPDATE_ITEM_COUNT, msgUpdate, msgUpdateBuf);
		msgUpdate->ItemUniqueNumber = pItemGeneral->UniqueNumber;
		msgUpdate->NewCount = pItemGeneral->CurrentCount;
		msgUpdate->ItemUpdateType = IUT_GENERAL;
		m_pFieldIOCPSocket->SendAddData(msgUpdateBuf, MSG_SIZE(MSG_FC_STORE_UPDATE_ITEM_COUNT));

		// DP UP
// 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - �Ʒ��� ���� ����
//		m_pCharacter->CurrentDP += min(pItemInfo->ParameterValue1, pItemGeneral->CurrentCount);
		m_pCharacter->CurrentDP += min(pItemInfo->ArrParameterValue[0], pItemGeneral->CurrentCount);

		if (m_pCharacter->CurrentDP >= m_pCharacter->DP)
		{
			m_pCharacter->CurrentDP = m_pCharacter->DP;
			m_pFieldIOCPSocket->SendCharacterInfo(T_FC_CHARACTER_CHANGE_CURRENTDP);
			// �����
			StartTimerField(TE_TYPE_GRADUAL_SHIELD_UP, TIMER_GRADUAL_UP_INTERVAL, pTimerEvent->nGeneralParam1, pTimerEvent->nGeneralParam2, pTimerEvent->nGeneralParam3);
			return;
		}
		m_pFieldIOCPSocket->SendCharacterInfo(T_FC_CHARACTER_CHANGE_CURRENTDP);

		// �����
		StartTimerField(TE_TYPE_GRADUAL_SHIELD_UP, TIMER_GRADUAL_UP_INTERVAL, pTimerEvent->nGeneralParam1, pTimerEvent->nGeneralParam2, pTimerEvent->nGeneralParam3);
	}
}

void CFieldTimerManager::HandleTimerEventRequestTimeOut(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime)
{
	if(REQUEST_TYPE_P2P_PK == pTimerEvent->nGeneralParam1)
	{
		if(INVALID_CLIENT_INDEX == m_pFieldIOCPSocket->m_peerP2PPKClientIndex
			|| m_pFieldIOCPSocket->m_peerP2PPKClientIndex != pTimerEvent->nGeneralParam3
			|| m_pFieldIOCPSocket->m_bP2PPKStarted)
		{// �ٸ������ P2PK �������̰ų� �̹� ������

			return;
		}

		INIT_MSG_WITH_BUFFER(MSG_FC_BATTLE_REJECT_REQUEST_P2P_PK_OK, T_FC_BATTLE_REJECT_REQUEST_P2P_PK_OK, pMsgRejectOK, RejectOKBuf);
		
		// PK reset
		m_pFieldIOCPSocket->m_peerP2PPKClientIndex	= INVALID_CLIENT_INDEX;
		m_pFieldIOCPSocket->m_bP2PPKStarted			= FALSE;
		pMsgRejectOK->TargetClientIndex				= pTimerEvent->nGeneralParam3;
		m_pFieldIOCPSocket->SendAddData(RejectOKBuf, MSG_SIZE(MSG_FC_BATTLE_REJECT_REQUEST_P2P_PK_OK));

		CFieldIOCPSocket *pPeerFISocket = ms_pFieldIOCP2->GetFieldIOCPSocket(pTimerEvent->nGeneralParam3);
		if(pPeerFISocket 
			&& pPeerFISocket->IsUsing()
			&& pPeerFISocket->m_peerP2PPKClientIndex == pTimerEvent->nGeneralParam2)
		{
			pPeerFISocket->m_peerP2PPKClientIndex	= INVALID_CLIENT_INDEX;
			pPeerFISocket->m_bP2PPKStarted			= FALSE;
			pMsgRejectOK->TargetClientIndex			= pTimerEvent->nGeneralParam2;
			pPeerFISocket->SendAddData(RejectOKBuf, MSG_SIZE(MSG_FC_BATTLE_REJECT_REQUEST_P2P_PK_OK));
		}
	}
	else if (pTimerEvent->nGeneralParam1 == REQUEST_TYPE_PARTY_BATTLE)
	{
		// ��Ƽ���� Ż���ϰų� ƨ�� ��쿡�� ����
		CFieldParty *pFParty = m_pFieldIOCPSocket->m_pFieldParty;			// 2006-07-04 by cmkwon
		if (pFParty == NULL)
		{
//			m_pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, "aaa");
			return;
		}

		PartyID_t tmpPartyID = MAKEUINT64(pTimerEvent->nGeneralParam2, pTimerEvent->nGeneralParam3);

		// ��û�� ����� ��Ƽ�� ������ Ȯ��
		if (pFParty->m_PartyID != tmpPartyID)
		{
//			m_pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, "%I64d != %I64d", m_pFieldParty->m_PartyID, tmpPartyID);
			return;
		}

		if (pFParty->m_PeerBattleRequesterPartyID != 0
			&& pFParty->m_PeerBattlePartyID == 0)
		{
			pFParty->m_PeerBattleRequesterPartyID = 0;

			INIT_MSG_WITH_BUFFER(MSG_FC_REQUEST_CANCEL_REQUEST, T_FC_REQUEST_CANCEL_REQUEST, pCancelRequest, pCancelRequestBuf);
			pCancelRequest->PeerClientIndex = 0;
			pCancelRequest->RequestType = REQUEST_TYPE_PARTY_BATTLE;
			m_pFieldIOCPSocket->SendAddData(pCancelRequestBuf, MSG_SIZE(MSG_FC_REQUEST_CANCEL_REQUEST));
			return;
		}
		else
		{
//			m_pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, "%I64d == 0 || %I64d != 0",
//				m_pFieldParty->m_PeerBattleRequesterPartyID,
//				m_pFieldParty->m_PeerBattlePartyID);

			return;
		}
	}
	else if (pTimerEvent->nGeneralParam1 == REQUEST_TYPE_GUILD_WAR)
	{
		m_pFieldIOCPSocket->m_RequestHandler.SendCancelRequsetAndReset();
	}
}

void CFieldTimerManager::HandleTimerEventTest(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime)
{
//	m_pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, "TimerEventTest(cnt: %d, interval: %d, gap: %d)",
//		pTimerEvent->nRemainedRepeatCount, (INT)pTimerEvent->TimeInterval,
//		(INT)(currentTime - pTimerEvent->ExpireTime));
}



void CFieldTimerManager::HandleTimerEventDoMinutelyWork(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime)
{
	m_dwDoMinutelyCounts++;

	if(m_pFieldIOCPSocket->GetClientArrayIndex()%5 == m_dwDoMinutelyCounts%5)
	{// 5�п� �ѹ��� ī���ͺ� ������ ������ �����Ѵ�

		m_pFieldIOCPSocket->m_ItemManager.StoreCountableItem();

		m_pFieldIOCPSocket->StoreExperienceDelayStore();			// 2006-05-21 by cmkwon
	}

	///////////////////////////////////////////////////////////////////////////////
	// 1�и��� �����̾� ī�� üũ
	m_pFieldIOCPSocket->m_ItemManager.CashCheckPremiumCardExpire();

	///////////////////////////////////////////////////////////////////////////////
	// 1�и��� �ð������� �������� ���� �ð��� üũ�Ѵ�
	m_pFieldIOCPSocket->m_ItemManager.CheckExpireUsingTimeLimitedItemList();

	///////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
	m_pFieldIOCPSocket->m_ItemManager.SaveInvokingItemList(FALSE);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-09-27 by cmkwon
	if(m_pCharacter->HP > m_pCharacter->CurrentHP)
	{
		this->CheckRecoveryTimer(TE_TYPE_RECOVER_HP);
	}
	if(m_pCharacter->DP > m_pCharacter->CurrentDP)
	{
		this->CheckRecoveryTimer(TE_TYPE_RECOVER_DP);
	}
	if(m_pCharacter->SP > m_pCharacter->CurrentSP)
	{
		this->CheckRecoveryTimer(TE_TYPE_RECOVER_SP);
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2007-06-13 by cmkwon, ���� 7�� PC�� ���θ��(���̺�Ʈ ����) - �����������
	m_pFieldIOCPSocket->OnDoMinutelyWorkByTimerManager();

	///////////////////////////////////////////////////////////////////////////////
	// Flag�� �ʱ�ȭ�ϰ� Ÿ�̸Ӹ� �߰��Ѵ�	
	m_bDoMinutelyWorkRunning = FALSE;
	this->CheckRecoveryTimer(TE_TYPE_DO_MINUTELY_WORK);
}

void CFieldTimerManager::HandleTimerEventRecoverSP(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime)
{
	///////////////////////////////////////////////////////////////////////////////
	// 2006-09-27 by cmkwon, TimerUID üũ
//	DBGOUT("HandleTimerEventRecoverSP uiTimerUID(%d) == pTimerEventUID(%d)\r\n", m_uiSPTimerUID, pTimerEvent->TimerUID);
	if(pTimerEvent->TimerUID != m_uiSPTimerUID){						return;}

	if(m_pCharacter->SP <= m_pCharacter->CurrentSP
		|| COMPARE_BODYCON_BIT(m_pCharacter->BodyCondition, BODYCON_DEAD_MASK))
	{
		m_bRecoverSPTimerRunning	= FALSE;		// 2006-09-27 by cmkwon
		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// SP ȸ��
	SHORT	sAddValue	= 3.0f;								// 2006-09-27 by cmkwon, ����(3�ʴ� 2�� ȸ��-->3�ʴ� 3�� ȸ��)
	sAddValue			+= (SHORT)(m_pCharacter->SP/500);	// 2006-09-27 by cmkwon, SP���� ���� �߰� ȸ�� SP���� ���ȴ�.
// 2005-11-09 by cmkwon, ���¿� ������� 3�ʴ� 2��
//	if (COMPARE_BODYCON_BIT(m_pCharacter->BodyCondition, BODYCON_LANDED_MASK)
//		&& FALSE == COMPARE_BODYCON_BIT(m_pCharacter->BodyCondition, BODYCON_SIEGE_ON_MASK))
//	{// �������� �̸鼭 �����尡 �ƴϸ�
//		TILEINFO *pTile = m_pFieldIOCPSocket->m_pCurrentFieldMapChannel->m_pFieldMapProject->GetTileInfo(&m_pCharacter->PositionVector);
//		if ( m_pFieldIOCPSocket->m_pCurrentFieldMapChannel->m_pFieldMapProject->m_bAutoDPRecovery
//			|| (pTile && pTile->m_bEnableLand)
//			)
//		{// �ڵ�ȸ�� ���̰ų� �������� �ȿ� ���� ���
//			sAddValue = 4.0f;
//		}
//	}

	m_pCharacter->CurrentSP += sAddValue;	
	if(m_pCharacter->CurrentSP >= m_pCharacter->SP)
	{
		m_pCharacter->CurrentSP = m_pCharacter->SP;
		m_pFieldIOCPSocket->SendCharacterInfo(T_FC_CHARACTER_CHANGE_CURRENTSP);
		m_bRecoverSPTimerRunning	= FALSE;		// 2006-09-27 by cmkwon
		return;
	}
	
	m_pFieldIOCPSocket->SendCharacterInfo(T_FC_CHARACTER_CHANGE_CURRENTSP);	
	m_bRecoverSPTimerRunning	= CheckRecoveryTimer(TE_TYPE_RECOVER_SP, FALSE);		// reset
}

void CFieldTimerManager::HandleTimerEventDecreaseSP(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime)
{
	ITEM *pItemInfo = ms_pFieldIOCP2->GetItemInfo(pTimerEvent->nGeneralParam3);

	if (pItemInfo == NULL)
	{
		// fatal error
		StopTimerField(pTimerEvent->TimerUID);
		return;
	}

	m_pCharacter->CurrentSP = max(m_pCharacter->CurrentSP-pItemInfo->ReqSP, 0);

	CheckRecoveryTimer(TE_TYPE_RECOVER_SP);

	m_pFieldIOCPSocket->SendCharacterInfo(T_FC_CHARACTER_CHANGE_CURRENTSP);
}


void CFieldTimerManager::HandleTimerEventDeleteMine(MEX_TIMER_EVENT* pTimerEvent, TimeUnit_t currentTime)
{
	CMapBlock *pMapBlock = (CMapBlock*)pTimerEvent->nGeneralParam1;
	DROPMINE *pDropMine = (DROPMINE*)pTimerEvent->nGeneralParam2;

	// lock m_setCharacDropMine
	mt_auto_lock mineLock(&m_pFieldIOCPSocket->m_setCharacDropMine);

	if (!m_pFieldIOCPSocket->IsValidDropMine(pDropMine, pMapBlock) || pMapBlock == NULL)
	{
		// error
		return;
	}

	INIT_MSG_OF_SIZE(MSG_FC_ITEM_HIDE_ITEM, T_FC_ITEM_HIDE_ITEM, pMsgHideItem, pMsgHideItemBuf);
	pMsgHideItem->ClientIndex = 0;
	pMsgHideItem->ItemFieldIndex = (UINT)pDropMine;
	ms_pFieldIOCP2->SendInRangeMessageAroundPosition(pDropMine->Position,
										pMsgHideItemBuf, MSG_SIZE(MSG_FC_ITEM_HIDE_ITEM),
										(CFieldMapChannel*)pMapBlock->m_pMapChannel);

	m_pFieldIOCPSocket->m_setCharacDropMine.deleteLock(pDropMine);	// ĳ������ drop mine list������ ����

	pMapBlock->m_setMapBlockDropMine.deleteLock(pDropMine);

	util::del(pDropMine);
}

void CFieldTimerManager::HandleTimerEventDeleteDummy(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime)
{
// 2006-12-04 by dhjin, ���̸� ��ų üũ�� ����
//	INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_HIDE_ITEM, T_FC_ITEM_HIDE_ITEM, pHideItem, pHideItemBuf);
//	pHideItem->ClientIndex = m_pCharacter->ClientIndex;
//	pHideItem->ItemFieldIndex = pTimerEvent->nGeneralParam1;
//	m_pFieldIOCPSocket->SendInRangeMessageAroundMe(pHideItemBuf, MSG_SIZE(MSG_FC_ITEM_HIDE_ITEM), TRUE);
//
//	// lock m_mapFieldDummy
//	mt_auto_lock dummyLock(&m_pFieldIOCPSocket->m_mapFieldDummy);
//
//	if (m_pFieldIOCPSocket->m_mapFieldDummy.find(pHideItem->ItemFieldIndex) == m_pFieldIOCPSocket->m_mapFieldDummy.end())
//	{
//		// �̹� �������ų� ������ ���, do nothing
//		m_pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, "OnDeleteDummy, Not Exists![%#08x], dummyCount(%d)\r\n",
//						pHideItem->ItemFieldIndex, m_pFieldIOCPSocket->m_mapFieldDummy.size());
//	}
//	else
//	{
//		m_pFieldIOCPSocket->m_mapFieldDummy.deleteLock(pHideItem->ItemFieldIndex);
//		delete((FIELD_DUMMY*)pHideItem->ItemFieldIndex);
//
//		m_pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, "OnDeleteDummy [%#08x], dummyCount(%d)\r\n",
//						pHideItem->ItemFieldIndex, m_pFieldIOCPSocket->m_mapFieldDummy.size());
//	}

	if(FALSE == m_pFieldIOCPSocket->DeleteFieldDummy(pTimerEvent->nGeneralParam1))
	{// 2006-12-04 by dhjin, ã�� �� ����
		return;
	}
	
	INIT_MSG_WITH_BUFFER(MSG_FC_BATTLE_DELETE_DUMMY_OK, T_FC_BATTLE_DELETE_DUMMY_OK, pSMsg, SendBuf);
	pSMsg->AttackIndex		= m_pCharacter->ClientIndex;
	pSMsg->ItemFieldIndex	= pTimerEvent->nGeneralParam1;
	m_pFieldIOCPSocket->SendInRangeMessageAroundMe(SendBuf, MSG_SIZE(MSG_FC_BATTLE_DELETE_DUMMY_OK), TRUE);
}

// TE_TYPE_DELETE_FIXER
void CFieldTimerManager::HandleTimerEventDeleteFixer(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime)
{
	// send msg
	INIT_MSG_WITH_BUFFER(MSG_FC_BATTLE_ATTACK_HIDE_ITEM_W_KIND, T_FC_BATTLE_ATTACK_HIDE_ITEM_W_KIND, pMsgHideItem, pMsgHideItemBuf);
	pMsgHideItem->TargetIndex = pTimerEvent->nGeneralParam1;
	pMsgHideItem->ItemKind = pTimerEvent->nGeneralParam2;
	pMsgHideItem->TargetItemFieldIndex = (UINT)pTimerEvent->TimerUID;

	m_pFieldIOCPSocket->SendInRangeMessageAroundMe(pMsgHideItemBuf, MSG_SIZE(MSG_FC_BATTLE_ATTACK_HIDE_ITEM_W_KIND), TRUE);
}

void CFieldTimerManager::HandleTimerEventDeleteDecoy(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime)
{
	ITEM_GENERAL *pItemGeneral = (ITEM_GENERAL*)pTimerEvent->nGeneralParam1;
	ITEM *pItemInfo = pItemGeneral->ItemInfo;

#ifdef _DEBUG
	m_pFieldIOCPSocket->SendString128Static(STRING_128_ADMIN_CMD, "%s: %d", pItemInfo->ItemName, pItemGeneral->CurrentCount);
#endif

	if (!COMPARE_BODYCON_BIT(m_pCharacter->BodyCondition, BODYCON_DECOY_MASK))
	{
		return;
	}

	// ���� �ð� ����
	pItemGeneral->CurrentCount -= TIMER_DECOY_TIME_CHECK_INTERVAL/1000;

	// Count ������Ʈ
	INIT_MSG_WITH_BUFFER(MSG_FC_STORE_UPDATE_ITEM_COUNT, T_FC_STORE_UPDATE_ITEM_COUNT, msgUpdate, msgUpdateBuf);
	msgUpdate->ItemUniqueNumber = pItemGeneral->UniqueNumber;
	msgUpdate->NewCount = pItemGeneral->CurrentCount;
	msgUpdate->ItemUpdateType = IUT_GENERAL;
	m_pFieldIOCPSocket->SendAddData(msgUpdateBuf, MSG_SIZE(MSG_FC_STORE_UPDATE_ITEM_COUNT));

	if (pItemGeneral->CurrentCount <= 0)
	{
		pItemGeneral->CurrentCount = 0;
		m_pFieldIOCPSocket->StopItemKindDecoy(pItemInfo);
		return;
	}
	else
	{
		StartTimerField(TE_TYPE_DELETE_DECOY, TIMER_DECOY_TIME_CHECK_INTERVAL, pTimerEvent->nGeneralParam1);
	}

	return;
}

const MEX_TIMER_EVENT* CFieldTimerManager::GetTimerEventByEventID(UID32_t i_nTEID)
{
	MEX_TIMER_EVENT *ret = NULL;

	m_listWatingTimerEvents.lock();
	mtlistMexTimerEvent::iterator itr = m_listWatingTimerEvents.begin();
	while(itr != m_listWatingTimerEvents.end())
	{
		if (itr->TimerUID == i_nTEID)
		{
			ret = &(*itr);
			break;
		}
		itr++;
	}
	m_listWatingTimerEvents.unlock();

	return ret;
}

const MEX_TIMER_EVENT* CFieldTimerManager::GetTimerEventByItemNum(INT i_nItemNum)
{
	MEX_TIMER_EVENT *ret = NULL;

	m_listWatingTimerEvents.lock();
	mtlistMexTimerEvent::iterator itr = m_listWatingTimerEvents.begin();
	while(itr != m_listWatingTimerEvents.end())
	{
//#ifdef _DEBUG
//		pTimer->print();
//#endif
		MEX_TIMER_EVENT *pTimerEvent = &(*itr);
		if ( pTimerEvent->Type == TE_TYPE_DECREASE_SP
			&& pTimerEvent->nGeneralParam3 == i_nItemNum)
		{
			ret = &(*itr);
			break;
		}
		itr++;
	}
	m_listWatingTimerEvents.unlock();

	return ret;
}

BOOL CFieldTimerManager::DeleteTimerEventByTimerUID(UID32_t i_nTimerUID)
{
	BOOL bRet = FALSE;

	m_listWatingTimerEvents.lock();
	mtlistMexTimerEvent::iterator itr = m_listWatingTimerEvents.begin();
	while(itr != m_listWatingTimerEvents.end())
	{
		MEX_TIMER_EVENT *pTimerEvent = &(*itr);
		if (pTimerEvent->TimerUID == i_nTimerUID)
		{
			m_listWatingTimerEvents.erase(itr);
			bRet = TRUE;
			break;
		}
		itr++;
	}
	m_listWatingTimerEvents.unlock();

	return bRet;
}

// 2006-09-27 by cmkwon, ���� �߰�(BOOL i_bFlagCheck/*=TRUE*/)
BOOL CFieldTimerManager::CheckRecoveryTimer(TimerEventType TEType, BOOL i_bFlagCheck/*=TRUE*/)
{
	DWORD dwCur = timeGetTime();
	if (TEType == TE_TYPE_RECOVER_HP)
	{

		m_pFieldIOCPSocket->CheckDPLoop();
		return FALSE;

		////////////////////////////////////////////////////////////////////////////////
		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 
		if(m_pFieldIOCPSocket->m_SkillManager.m_FieldDebuff.CheckApplyingDebuff(DES_SKILL_MON_FREEZE_HP)) {
			// ������ ��ų �߿� HP,DP,SP ȸ�� �Ұ�
			return FALSE;
		}

		if(i_bFlagCheck
			&& m_bRecoverHPTimerRunning)
		{// 2005-12-01 by cmkwon

			if(dwCur-m_dwRecoverHPRunningTick < 3*HP_RECOVERY_INTERVAL)
			{
				return FALSE;
			}
		}
		m_dwRecoverHPRunningTick	= dwCur;

		if(m_pCharacter->CurrentHP >= m_pCharacter->HP)
		{// 2005-10-07 by cmkwon, �̹� HP�� �����̴�.
			return FALSE;
		}
		// ���ø��̰ų� ���� ������ ���� ä��
		if (NULL == m_pFieldIOCPSocket->m_pCurrentFieldMapChannel)
		{
			return FALSE;
		}

		if(FALSE == m_pCharacter->CharacterMode)
		{
			float fHPRepairRate = m_pFieldIOCPSocket->GetHPRepairRateFlighting();
			if (FALSE == COMPARE_BODYCON_BIT(m_pCharacter->BodyCondition, BODYCON_LANDED_MASK)
				&& 0.0f >= fHPRepairRate)
			{// ���� ���°� �ƴϴ�
				return FALSE;
			}

			// ���� ���� ������ ���� ä��(A����)
			TILEINFO *pTile = m_pFieldIOCPSocket->m_pCurrentFieldMapChannel->m_pFieldMapProject->GetTileInfo(&m_pCharacter->PositionVector);
			if (NULL == pTile
				|| (FALSE == m_pFieldIOCPSocket->m_pCurrentFieldMapChannel->m_pFieldMapProject->m_bAutoDPRecovery && FALSE == pTile->m_bEnableLand)
				)
			{
				if(0.0f >= fHPRepairRate)
				{
					return FALSE;
				}
			}
		}
		m_bRecoverHPTimerRunning	= TRUE;
		m_uiHPTimerUID				= StartTimerField(TE_TYPE_RECOVER_HP, max(HP_MIN_RECOVERY_INTERVAL, HP_RECOVERY_INTERVAL - HP_RECOVERY_INTERVAL*m_pFieldIOCPSocket->GetHPRepairTimeSubtractRate()));
		return TRUE;
	}
	else if (TEType == TE_TYPE_RECOVER_DP)
	{
		m_pFieldIOCPSocket->CheckDPLoop();
		return FALSE;

		if (i_bFlagCheck && m_bRecoverDPTimerRunning)
			
			if (dwCur - m_dwRecoverDPRunningTick < 3 * DP_RECOVERY_INTERVAL)
				
				return FALSE;

		m_dwRecoverDPRunningTick		= dwCur;
		
		if (!m_pFieldIOCPSocket->IsEnableRepairDP()) return FALSE;

		if (m_pCharacter->CurrentDP >= m_pCharacter->DP) return	FALSE;

		m_bRecoverDPTimerRunning		= TRUE;
		//m_uiDPTimerUID					= StartTimerField(TE_TYPE_RECOVER_DP, max(DP_MIN_RECOVERY_INTERVAL, DP_RECOVERY_INTERVAL - DP_RECOVERY_INTERVAL*m_pFieldIOCPSocket->GetDPRepairTimeSubtractRate()));	// 2006-12-13 by cmkwon, �ְ��� �����Ѵ�.
		m_uiDPTimerUID = StartTimerField(TE_TYPE_RECOVER_DP, 600);
		m_fDPRecoveryWeight				= 1;
		m_fSumDistanceForDPConsumption	= 0;
		return TRUE;
	}
	else if (TEType == TE_TYPE_RECOVER_SP)
	{
		////////////////////////////////////////////////////////////////////////////////
		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 
		if(m_pFieldIOCPSocket->m_SkillManager.m_FieldDebuff.CheckApplyingDebuff(DES_SKILL_MON_FREEZE_SP)) {
			// ������ ��ų �߿� HP,DP,SP ȸ�� �Ұ�
			return FALSE;
		}

		if(i_bFlagCheck
			&& m_bRecoverSPTimerRunning)
		{// 2005-12-01 by cmkwon

			if(dwCur-m_dwRecoverSPRunningTick < 3*SP_RECOVERY_INTERVAL)
			{
				return FALSE;
			}
		}
		m_dwRecoverSPRunningTick	= dwCur;

		if(m_pCharacter->CurrentSP >= m_pCharacter->SP)
		{// 2005-10-07 by cmkwon, �̹� SP�� �����̴�.
			return FALSE;
		}
		
		m_bRecoverSPTimerRunning	= TRUE;
		m_uiSPTimerUID				= StartTimerField(TE_TYPE_RECOVER_SP, max(SP_MIN_RECOVERY_INTERVAL, SP_RECOVERY_INTERVAL - SP_RECOVERY_INTERVAL*m_pFieldIOCPSocket->GetSPRepairTimeSubtractRate()));	// 2006-12-13 by cmkwon, �ּҰ� ����
		return TRUE;
	}
	else if(TE_TYPE_DO_MINUTELY_WORK == TEType && FALSE == m_bDoMinutelyWorkRunning)
	{
		StartTimerField(TE_TYPE_DO_MINUTELY_WORK, TIMER_DO_MINUTELY_WORK);
		m_bDoMinutelyWorkRunning = TRUE;
		return TRUE;
	}

	return FALSE;
}


int CFieldTimerManager::PauseAllTimerEvents(TIMER_EVENT_4_EXCHANGE *ArrTimerEvent4Exchange)
{
//	ASSERT_NOT_IMPLEMENTED_YET();	// check: Ÿ�̸� ������, 20041004, kelovon
	return 0;
}

int CFieldTimerManager::ContinueAllTimerEvents()
{
//	ASSERT_NOT_IMPLEMENTED_YET();	// check: Ÿ�̸� ������, 20041004, kelovon
	return 0;
}

UID32_t CFieldTimerManager::StartTimerField(TimerEventType i_type, TimeUnit_t i_nInterval
											, UINT i_nParam1 /*=0*/
											, UINT i_nParam2 /*=0*/
											, UINT i_nParam3 /*=0*/
											, INT i_nRepeatCount /*=0*/)
{
	INIT_MSG_WITH_BUFFER(MSG_FC_TIMER_START_TIMER, T_FC_TIMER_START_TIMER, msgStartTimer, msgStartTimerBuf);
	msgStartTimer->TimerEvent.Type					= i_type;
	msgStartTimer->TimerEvent.TimerUID				= m_TimerUIDGenerator.GetNext();
	msgStartTimer->TimerEvent.StartTimeStamp		= ms_pFieldIOCP2->GetCurrentServerTimeInMilliSeconds();
	msgStartTimer->TimerEvent.ExpireTime			= msgStartTimer->TimerEvent.StartTimeStamp + i_nInterval;
	msgStartTimer->TimerEvent.TimeInterval			= i_nInterval;
	msgStartTimer->TimerEvent.CharacterUniqueNumber	= m_pCharacter->CharacterUniqueNumber;
	msgStartTimer->TimerEvent.ClientIndex			= m_pCharacter->ClientIndex;
	msgStartTimer->TimerEvent.nGeneralParam1		= i_nParam1;
	msgStartTimer->TimerEvent.nGeneralParam2		= i_nParam2;
	msgStartTimer->TimerEvent.nGeneralParam3		= i_nParam3;
	msgStartTimer->TimerEvent.bRepeatable			= ((i_nRepeatCount==0)?FALSE:TRUE);	// check: �ٽ� �� Ȯ���غ���, 20041001, kelovon
	msgStartTimer->TimerEvent.nRemainedRepeatCount	= i_nRepeatCount;					// check: �ٽ� �� Ȯ���غ���, 20041001, kelovon
	m_pFieldIOCPSocket->SendAddData(msgStartTimerBuf, MSG_SIZE(MSG_FC_TIMER_START_TIMER));

	m_listWatingTimerEvents.pushBackLock(msgStartTimer->TimerEvent);

	m_pFieldIOCPSocket->SendString128(STRING_128_DEBUG_L3, "START %s uid(%d) interval(%d) time(%u) expire(%u)",
		CAtumSJ::GetTimerTypeString(msgStartTimer->TimerEvent.Type),
		(INT)msgStartTimer->TimerEvent.TimerUID,
		(INT)msgStartTimer->TimerEvent.TimeInterval,
		(UINT)msgStartTimer->TimerEvent.StartTimeStamp,
		(UINT)msgStartTimer->TimerEvent.ExpireTime);

	return msgStartTimer->TimerEvent.TimerUID;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldTimerManager::StartTimerFieldNotCreateTimerUID(TimerEventType i_type, TimeUnit_t i_nInterval, UID32_t i_nTimerUID, UINT i_nParam1 = 0, UINT i_nParam2 = 0, UINT i_nParam3 = 0, INT i_nRepeatCount = 0)
/// \brief		���Ǵ�Ƽ - �ܺο��� TimerUID �Է� �޾� ����ϴ� �Լ� �߰�
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldTimerManager::StartTimerFieldNotCreateTimerUID(TimerEventType i_type, TimeUnit_t i_nInterval, UID32_t i_nTimerUID											, UINT i_nParam1 /*=0*/
														  , UINT i_nParam2 /*=0*/
														  , UINT i_nParam3 /*=0*/
														  , INT i_nRepeatCount /*=0*/) {
	INIT_MSG_WITH_BUFFER(MSG_FC_TIMER_START_TIMER, T_FC_TIMER_START_TIMER, msgStartTimer, msgStartTimerBuf);
	msgStartTimer->TimerEvent.Type					= i_type;
	msgStartTimer->TimerEvent.TimerUID				= i_nTimerUID;
	msgStartTimer->TimerEvent.StartTimeStamp		= ms_pFieldIOCP2->GetCurrentServerTimeInMilliSeconds();
	msgStartTimer->TimerEvent.ExpireTime			= msgStartTimer->TimerEvent.StartTimeStamp + i_nInterval;
	msgStartTimer->TimerEvent.TimeInterval			= i_nInterval;
	msgStartTimer->TimerEvent.CharacterUniqueNumber	= m_pCharacter->CharacterUniqueNumber;
	msgStartTimer->TimerEvent.ClientIndex			= m_pCharacter->ClientIndex;
	msgStartTimer->TimerEvent.nGeneralParam1		= i_nParam1;
	msgStartTimer->TimerEvent.nGeneralParam2		= i_nParam2;
	msgStartTimer->TimerEvent.nGeneralParam3		= i_nParam3;
	msgStartTimer->TimerEvent.bRepeatable			= ((i_nRepeatCount==0)?FALSE:TRUE);	// check: �ٽ� �� Ȯ���غ���, 20041001, kelovon
	msgStartTimer->TimerEvent.nRemainedRepeatCount	= i_nRepeatCount;					// check: �ٽ� �� Ȯ���غ���, 20041001, kelovon
	m_pFieldIOCPSocket->SendAddData(msgStartTimerBuf, MSG_SIZE(MSG_FC_TIMER_START_TIMER));
	
	m_listWatingTimerEvents.pushBackLock(msgStartTimer->TimerEvent);
	
	m_pFieldIOCPSocket->SendString128(STRING_128_DEBUG_L3, "START %s uid(%d) interval(%d)",
		CAtumSJ::GetTimerTypeString(msgStartTimer->TimerEvent.Type),
		(INT)msgStartTimer->TimerEvent.TimerUID,
		(INT)msgStartTimer->TimerEvent.TimeInterval);
}

BOOL CFieldTimerManager::StopTimerField(UID32_t i_nTimerUID)
{
	if (i_nTimerUID == 0)
	{
		return TRUE;
	}

	m_listWatingTimerEvents.lock();

	int nDeleted = 0;
	mtlistMexTimerEvent::iterator itrTimer = m_listWatingTimerEvents.begin();
	while (itrTimer != m_listWatingTimerEvents.end())
	{
		if (itrTimer->TimerUID == i_nTimerUID)
		{
			m_listWatingTimerEvents.erase(itrTimer);
			nDeleted = 1;
			break;
		}

		itrTimer++;
	}

	if (nDeleted != 1)
	{
		// �̹� ���� �̺�Ʈ�� ���� ��� �������� ���� �� ���� -> ���� ��Ȳ��
		m_listWatingTimerEvents.unlock();
		return FALSE;
	}

	m_pFieldIOCPSocket->SendString128(STRING_128_DEBUG_L3, "T_FC_TIMER_STOP_TIMER UID:%d", i_nTimerUID);
	
	INIT_MSG_WITH_BUFFER(MSG_FC_TIMER_STOP_TIMER, T_FC_TIMER_STOP_TIMER, msgStopTimer, msgStopTimerBuf);
	msgStopTimer->TimerUID = i_nTimerUID;
	m_pFieldIOCPSocket->SendAddData(msgStopTimerBuf, MSG_SIZE(MSG_FC_TIMER_STOP_TIMER));

	m_listWatingTimerEvents.unlock();
	return TRUE;
}

UID32_t CFieldTimerManager::UpdateTimerField(UID32_t i_nTimerUID)
{
	// lock
	mt_auto_lock alock(&m_listWatingTimerEvents);

	MEX_TIMER_EVENT *pTimerEvent = NULL;

	mtlistMexTimerEvent::iterator itrTimer = m_listWatingTimerEvents.begin();
	while (itrTimer != m_listWatingTimerEvents.end())
	{
		if (itrTimer->TimerUID == i_nTimerUID)
		{
			pTimerEvent = &(*itrTimer);
			break;
		}

		itrTimer++;
	}

	if (pTimerEvent == NULL)
	{
		return 0;
	}

	// �ð� ������Ʈ
	pTimerEvent->StartTimeStamp = ms_pFieldIOCP2->GetCurrentServerTimeInMilliSeconds();
	pTimerEvent->ExpireTime = pTimerEvent->StartTimeStamp + pTimerEvent->TimeInterval;

	INIT_MSG_WITH_BUFFER(MSG_FC_TIMER_UPDATE_TIMER, T_FC_TIMER_UPDATE_TIMER, msgUpdateTimer, msgUpdateTimerBuf);
	msgUpdateTimer->TimerEvent = *pTimerEvent;
	m_pFieldIOCPSocket->SendAddData(msgUpdateTimerBuf, MSG_SIZE(MSG_FC_TIMER_UPDATE_TIMER));

	return pTimerEvent->TimerUID;
}

UID32_t CFieldTimerManager::PauseTimerField(UID32_t i_nTimerUID)
{
	INIT_MSG_WITH_BUFFER(MSG_FC_TIMER_PAUSE_TIMER, T_FC_TIMER_PAUSE_TIMER, msgPauseTimer, msgPauseTimerBuf);
	msgPauseTimer->TimerUID = i_nTimerUID;
	m_pFieldIOCPSocket->SendAddData(msgPauseTimerBuf, MSG_SIZE(MSG_FC_TIMER_PAUSE_TIMER));

	return i_nTimerUID;
}

UID32_t CFieldTimerManager::ContinueTimerField(UID32_t i_nTimerUID)
{
	INIT_MSG_WITH_BUFFER(MSG_FC_TIMER_CONTINUE_TIMER, T_FC_TIMER_CONTINUE_TIMER, msgContinueTimer, msgContinueTimerBuf);
	msgContinueTimer->TimerUID = i_nTimerUID;
	m_pFieldIOCPSocket->SendAddData(msgContinueTimerBuf, MSG_SIZE(MSG_FC_TIMER_CONTINUE_TIMER));

	return i_nTimerUID;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldTimerManager::HandleTimerEventDotSkillStealing(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime)
/// \brief		���Ǵ�Ƽ - // Stealing ��ų - SP���� (SP�� ���� �Ǿ ��� ���� �Ǿ�� �Ѵ�.)
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldTimerManager::HandleTimerEventDotSkillStealing(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime) {
	if(!m_pFieldIOCPSocket->m_SkillManager.m_FieldDot.CheckApplyingDot(pTimerEvent->nGeneralParam1, DES_SKILL_MON_STEALING)) {
		return;
	}

	DOTINFO CopyDotInfo;
	util::zero(&CopyDotInfo, sizeof(DOTINFO));
	m_pFieldIOCPSocket->m_SkillManager.m_FieldDot.CopyDotInfo(pTimerEvent->nGeneralParam1, &CopyDotInfo);

	m_pCharacter->CurrentSP = max(m_pCharacter->CurrentSP-CopyDotInfo.DesParamValue, 0);		
	m_pFieldIOCPSocket->SendCharacterInfo(T_FC_CHARACTER_CHANGE_CURRENTSP);

	// ���� �ð� ���� �� ����
	if(m_pFieldIOCPSocket->m_SkillManager.m_FieldDot.DecreaseApplyingDotTime(pTimerEvent->nGeneralParam1, DES_SKILL_MON_STEALING)) {
		StartTimerField(TE_TYPE_DOTSKILL_STEALING, TIMER_DOT_INTERVAL, pTimerEvent->nGeneralParam1);
	}
	else
	{
		m_pFieldIOCPSocket->m_SkillManager.m_FieldDot.ReleaseDot(pTimerEvent->nGeneralParam1);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldTimerManager::HandleTimerEventDotSkillDrain(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime)
/// \brief		���Ǵ�Ƽ - // Drain ��ų - �ǰݴ�� HP���ҽ��� ���ݴ�󿡰� ä���ش�.
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldTimerManager::HandleTimerEventDotSkillDrain(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime) {
	if(!m_pFieldIOCPSocket->m_SkillManager.m_FieldDot.CheckApplyingDot(pTimerEvent->nGeneralParam1, DES_SKILL_MON_DRAIN)) {
		return;
	}
	
	DOTINFO CopyDotInfo;
	util::zero(&CopyDotInfo, sizeof(DOTINFO));
	m_pFieldIOCPSocket->m_SkillManager.m_FieldDot.CopyDotInfo(pTimerEvent->nGeneralParam1, &CopyDotInfo);
	
	m_pCharacter->CurrentHP = max(m_pCharacter->CurrentHP-CopyDotInfo.DesParamValue, 0);	
	if(m_pCharacter->CurrentHP <= 0) {
		// ���� ó��
		m_pFieldIOCPSocket->CharacterDeadRoutine(DAMAGE_BY_NA);
		return;
	}
	m_pFieldIOCPSocket->SendCharacterInfo(T_FC_CHARACTER_CHANGE_CURRENTHP);
	CFieldMonster *ptmFMon = m_pFieldIOCPSocket->m_pCurrentFieldMapChannel->GetFieldMonster(CopyDotInfo.ClientIndex, 50);
	if(NULL == ptmFMon
		|| ptmFMon->m_enMonsterState != MS_PLAYING
		|| TRUE == COMPARE_BODYCON_BIT(ptmFMon->BodyCondition, BODYCON_DEAD_MASK))
	{
		return;
	}

	// 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.)
// 	if(ptmFMon->CurrentHP < ptmFMon->MonsterInfoPtr->MonsterHP) {
// 		ptmFMon->CurrentHP = min(ptmFMon->CurrentHP += CopyDotInfo.DesParamValue, ptmFMon->MonsterInfoPtr->MonsterHP);
// 		m_pFieldIOCPSocket->m_pCurrentFieldMapChannel->FieldSendMonsterChangeInfo(ptmFMon, T_FN_MONSTER_CHANGE_HP);	
// 	}
	if( ptmFMon->CurrentHP < ptmFMon->MonsterInfoExtend.fMaxHP ) 
	{
		ptmFMon->CurrentHP = min ( ptmFMon->CurrentHP += CopyDotInfo.DesParamValue , ptmFMon->MonsterInfoExtend.fMaxHP );
		m_pFieldIOCPSocket->m_pCurrentFieldMapChannel->FieldSendMonsterChangeInfo(ptmFMon, T_FN_MONSTER_CHANGE_HP);	
	}

	// End 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.)

	// ���� �ð� ���� �� ����
	if(m_pFieldIOCPSocket->m_SkillManager.m_FieldDot.DecreaseApplyingDotTime(pTimerEvent->nGeneralParam1, DES_SKILL_MON_DRAIN)) {
		StartTimerField(TE_TYPE_DOTSKILL_DRAIN, TIMER_DOT_INTERVAL, pTimerEvent->nGeneralParam1);
	}
	else
	{
		m_pFieldIOCPSocket->m_SkillManager.m_FieldDot.ReleaseDot(pTimerEvent->nGeneralParam1);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldTimerManager::HandleTimerEventDotSkillTimeBomb(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime)
/// \brief		���Ǵ�Ƽ(�������) - TimeBomb ��ų
/// \author		dhjin
/// \date		2010-03-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldTimerManager::HandleTimerEventDotSkillTimeBomb(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime)
{
	if(!m_pFieldIOCPSocket->m_SkillManager.m_FieldDot.CheckApplyingDot(pTimerEvent->nGeneralParam1, DES_TIME_BOMB)) {
		return;
	}
	
	DOTINFO CopyDotInfo;
	util::zero(&CopyDotInfo, sizeof(DOTINFO));
	m_pFieldIOCPSocket->m_SkillManager.m_FieldDot.CopyDotInfo(pTimerEvent->nGeneralParam1, &CopyDotInfo);
	
	// ���� �ð� ���� �� ����
	if(m_pFieldIOCPSocket->m_SkillManager.m_FieldDot.DecreaseApplyingDotTime(pTimerEvent->nGeneralParam1, DES_TIME_BOMB)) {
		StartTimerField(TE_TYPE_DOTSKILL_TIME_BOMB, TIMER_DOT_INTERVAL, pTimerEvent->nGeneralParam1);
	}
	else
	{
		m_pFieldIOCPSocket->m_SkillManager.m_FieldDot.ReleaseDot(pTimerEvent->nGeneralParam1);
		m_pFieldIOCPSocket->m_SkillManager.ProcessingTimeBombSkill(CopyDotInfo.DesParamValue);
	}

}

// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)
void CFieldTimerManager::HandleTimerFixedTermShape(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime)
{
	// ������ ã�Ƽ� ���� ���� ���� ó��
	BOOL bResult = FALSE;

	bResult = m_pFieldIOCPSocket->m_ItemManager.DeleteFixedTermShapeByTimerUID(pTimerEvent->TimerUID);

	if( TRUE == bResult )
	{
		// ��ü �� Ŭ�󿡰� ���� ���� �˸� ���� �� �Ʒ��� ���� ó�� ����

		ms_pFieldIOCP2->SendInRangeCharacterOtherInfo(m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_pCurrentFieldMapChannel);
	}
}
// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)