#include "stdafx.h"
#include "FieldSkillManager.h"
#include "FieldIOCPSocket.h"
#include "AtumError.h"
#include "FieldParty.h"
#include "FieldGlobal.h"		// 2009-12-11 by cmkwon, ������ ��׷η� Ÿ���� �����ϴ� ���� ���� - 

#include "FreeWar.h" // 2015-11-03 DevX

////////////////////////////////////////
// class CChargingSkillApplier
////////////////////////////////////////
CChargingSkillApplier::CChargingSkillApplier(ITEM **o_ppItemforProcessSplashAttack, BOOL i_bSplashAttack, ITEM *i_pAttackItem, CFieldIOCPSocket *i_pFieldIOCPSocket)
{
	m_pFieldIOCPSocket		= i_pFieldIOCPSocket;
	m_ChargingSkillParamFactor.Reset();
	m_bChargingSkillApplied = i_pFieldIOCPSocket->m_SkillManager.ApplyChargingSkill(o_ppItemforProcessSplashAttack, i_bSplashAttack, i_pAttackItem, &m_ChargingSkillParamFactor);
}

CChargingSkillApplier::~CChargingSkillApplier()
{
	if (m_bChargingSkillApplied)
	{
		m_pFieldIOCPSocket->m_SkillManager.ReleaseChargingSkill(&m_ChargingSkillParamFactor);
	}
}

////////////////////////////////////////
// class CFieldSkillManager
////////////////////////////////////////
CFieldSkillManager::CFieldSkillManager()
{
	m_vectorAtivatedSkill.reserve(10);
	m_vecChargingSkill.reserve(10);

	m_bChargingSkill	= FALSE;		// 2007-04-02 by cmkwon
	m_bHyperShot		= FALSE;		// 2007-03-29 by cmkwon
	m_bPrepareUseBigBoom= FALSE;		// 2007-07-13 by dhjin

	m_nSkillConfirmUseUID	= 1;		// 2009-04-06 by cmkwon, �ݿ��� ����� ��ų �ý��� ���� - �ʱⰪ
}

void CFieldSkillManager::InitFieldDataManager(CFieldIOCPSocket *i_pFieldIOCPSocket)
{
	m_pItemManager = &i_pFieldIOCPSocket->m_ItemManager;

	CFieldDataManager::InitFieldDataManager(i_pFieldIOCPSocket);
}

void CFieldSkillManager::ResetSkillData()
{
	m_vectorAtivatedSkill.clearLock();
	m_vecChargingSkill.clearLock();
	m_bChargingSkill	= FALSE;		// 2007-04-02 by cmkwon
	m_bHyperShot		= FALSE;		// 2007-03-29 by cmkwon
	m_bPrepareUseBigBoom= FALSE;		// 2007-07-13 by dhjin

	m_mtvectSkillConfirmUseList.clearLock();	// 2009-04-06 by cmkwon, �ݿ��� ����� ��ų �ý��� ���� - 
	m_nSkillConfirmUseUID	= 1;				// 2009-04-06 by cmkwon, �ݿ��� ����� ��ų �ý��� ���� - 
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldSkillManager::ResetTimeLimitSkillData()
/// \brief		�Ʒ��� ���� - �ð������� ��ų �ʱ�ȭ
/// \author		dhjin
/// \date		2008-02-25 ~ 2008-02-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldSkillManager::ResetTimeLimitSkillData()
{
	mt_auto_lock alock(&m_vectorAtivatedSkill);

	mtvectorItemID::iterator itr = m_vectorAtivatedSkill.begin();
	while (itr != m_vectorAtivatedSkill.end())
	{
		ITEM *pSkillItemInfo = m_pFieldIOCPSocket->ms_pFieldIOCP->GetItemInfo(itr->ItemNum);

		if(NULL != pSkillItemInfo
			&& SKILLTYPE_TIMELIMIT == pSkillItemInfo->SkillType)
		{
			this->UnsetSkillParamFactor(pSkillItemInfo);

			itr = m_vectorAtivatedSkill.erase(itr);
			continue;
		}

		itr++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldSkillManager::ResetToggleSkillData()
/// \brief		�Ʒ��� ���� - ��� ��ų�� �ʱ�ȭ
/// \author		dhjin
/// \date		2008-02-25 ~ 2008-02-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldSkillManager::ResetToggleSkillData()
{
	mt_auto_lock alock(&m_vectorAtivatedSkill);

	mtvectorItemID::iterator itr = m_vectorAtivatedSkill.begin();
	while (itr != m_vectorAtivatedSkill.end())
	{
		ITEM *pSkillItemInfo = m_pFieldIOCPSocket->ms_pFieldIOCP->GetItemInfo(itr->ItemNum);

		if(NULL != pSkillItemInfo
			&& SKILLTYPE_TOGGLE == pSkillItemInfo->SkillType)
		{
			this->UnsetSkillParamFactor(pSkillItemInfo);

			itr = m_vectorAtivatedSkill.erase(itr);

			const MEX_TIMER_EVENT *pTimerEvent = m_pFieldIOCPSocket->m_TimerManager.GetTimerEventByItemNum(pSkillItemInfo->ItemNum);
			if (pTimerEvent != NULL)
			{
				m_pFieldIOCPSocket->m_TimerManager.StopTimerField(pTimerEvent->TimerUID);
			}
			continue;
		}

		itr++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldSkillManager::ResetChargingSkillData()
/// \brief		�Ʒ��� ���� - ��¡ ��ų�� �ʱ�ȭ
/// \author		dhjin
/// \date		2008-02-25 ~ 2008-02-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldSkillManager::ResetChargingSkillData()
{
	mt_auto_lock mtA(&m_vecChargingSkill);
	
	m_bChargingSkill	= FALSE;
	m_bHyperShot		= FALSE;		
	m_vecChargingSkill.clear();
}

BOOL CFieldSkillManager::IsSkillActivatedByUID(UID64_t i_nSkillUID)
{
	mt_auto_lock alock(&m_vectorAtivatedSkill);

	mtvectorItemID::iterator itr = m_vectorAtivatedSkill.begin();
	while (itr != m_vectorAtivatedSkill.end())
	{
		if (i_nSkillUID == itr->ItemUID)
		{
			return TRUE;
		}

		itr++;
	}

	return FALSE;
}

BOOL CFieldSkillManager::IsSkillActivatedByItemNum(INT i_nSkillItemNum)
{
	mt_auto_lock alock(&m_vectorAtivatedSkill);

	mtvectorItemID::iterator itr = m_vectorAtivatedSkill.begin();
	while (itr != m_vectorAtivatedSkill.end())
	{
		if (i_nSkillItemNum == itr->ItemNum)
		{
			return TRUE;
		}

		itr++;
	}

	return FALSE;
}

// returns: 0�̸� ����, 0�� �ƴϿ� �ش� ��ų�� ItemNum
INT CFieldSkillManager::IsSkillActivatedBySkillBaseNum(INT i_nSkillItemNum)
{
	mt_auto_lock alock(&m_vectorAtivatedSkill);

	mtvectorItemID::iterator itr = m_vectorAtivatedSkill.begin();
	while (itr != m_vectorAtivatedSkill.end())
	{
		// 2013-03-12 by jhseol, ��ų �ѹ��� �ý��� ����
#ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
		ITEM *pItemInfo = m_pFieldIOCPSocket->ms_pFieldIOCP->GetItemInfo(itr->ItemNum);
		if ( NULL == pItemInfo )
		{
			itr++;
			continue;
		}
		if ( i_nSkillItemNum == pItemInfo->NextSkill)
		{
			return itr->ItemNum;
		}
#else	// #ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
		if (SKILL_BASE_NUM(i_nSkillItemNum) == SKILL_BASE_NUM(itr->ItemNum))
		{
			return itr->ItemNum;
		}
#endif	// #ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
		// end 2013-03-12 by jhseol, ��ų �ѹ��� �ý��� ����

		itr++;
	}

	return 0;
}

// 2013-03-12 by jhseol, ��ų �ѹ��� �ý��� ����
BYTE CFieldSkillManager::GetSkillActivatedLevelBySkillBaseNum(INT i_nSkillItemNum)
{
#ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
	mt_auto_lock alock(&m_vectorAtivatedSkill);
	
	mtvectorItemID::iterator itr = m_vectorAtivatedSkill.begin();
	while (itr != m_vectorAtivatedSkill.end())
	{
		ITEM *pItemInfo = m_pFieldIOCPSocket->ms_pFieldIOCP->GetItemInfo(itr->ItemNum);
		if ( NULL == pItemInfo )
		{
			itr++;
			continue;
		}
		if ( i_nSkillItemNum == pItemInfo->NextSkill )
		{
			return pItemInfo->SkillLevel;
		}
		
		itr++;
	}
#endif	// #ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
	
	return 0;
}
// end 2013-03-12 by jhseol, ��ų �ѹ��� �ý��� ����

BOOL CFieldSkillManager::ApplyAllPermanentAndAtivatedSkills()
{
	// �нú� ��ų ����
	mt_auto_lock mtAuto(&m_pItemManager->m_mapItemGeneral);
	for(int i=0; i < m_pItemManager->m_vectItemSkillPtr.size(); i++)
	{
		ITEM_SKILL *pTmpSkill = m_pItemManager->m_vectItemSkillPtr[i];
		if (pTmpSkill->ItemInfo->SkillType == SKILLTYPE_PERMANENT)
		{
			SetSkillParamFactor(pTmpSkill->ItemInfo);
		}
	}
	mtAuto.auto_unlock_cancel();

	// ��� �� ��ų ����
	m_vectorAtivatedSkill.lock();
	mtvectorItemID::iterator itr = m_vectorAtivatedSkill.begin();
	while (itr != m_vectorAtivatedSkill.end())
	{
		ItemID_t itemID = *itr;
		ITEM *pItemInfo = ms_pFieldIOCP2->GetItemInfo(itemID.ItemNum);
		if (pItemInfo != NULL)
		{
			// 2013-05-07 by jhseol,bckim ���� �г�Ƽ
			float tmBuffPenalty = 1.0f;
#ifdef SC_BUFF_PENALTY_JHSEOL_BCKIM
			if ( TRUE == pItemInfo->IsExistDesParam(DES_BUFF_PENALTY_RATIO) )					// ���� �г�Ƽ DES�� ������
			{
				USHORT tmApplirdUnitKind = (USHORT)pItemInfo->GetParameterValue(DES_BUFF_PENALTY_APPLIED_UNIT_KIND);
				if ( 0 != (m_pFieldIOCPSocket->m_character.UnitKind & tmApplirdUnitKind) )		// ������ �� ���Ͽ�
				{
					tmBuffPenalty = pItemInfo->GetParameterValue(DES_BUFF_PENALTY_RATIO);		// �г�Ƽ�� ���� ��Ų��.
				}
			}
#endif
			SetSkillParamFactor(pItemInfo, 0, tmBuffPenalty);
			// end 2013-05-07 by jhseol,bckim ���� �г�Ƽ
		}

		itr++;
	}
	m_vectorAtivatedSkill.unlock();

	return TRUE;
}

// check: ���� �ִ� ������ �츮��
ITEM_SKILL* CFieldSkillManager::GetActivatedSkillByUID(UID64_t i_nSkillUID)
{
	ASSERT_NOT_IMPLEMENTED_YET();
	return NULL;
}

// check: ���� �ִ� ������ �츮��
ITEM_SKILL* CFieldSkillManager::GetActivatedSkillByItemNum(INT i_nSkillItemNum)
{
	ASSERT_NOT_IMPLEMENTED_YET();
	return NULL;
}

// 2013-05-09 by hskim, ���� ����Ʈ ����
void CFieldSkillManager::GetActiveatedSkillByDesParam(mtvectorItemID *pvecItemID, DestParam_t desSame)
{
	mt_auto_lock alock(&m_vectorAtivatedSkill);

	mtvectorItemID::iterator itr = m_vectorAtivatedSkill.begin();
	while (itr != m_vectorAtivatedSkill.end())
	{
		ITEM *pItemInfo = ms_pFieldIOCP2->GetItemInfo(itr->ItemNum);
		if (pItemInfo != NULL)
		{
			if( pItemInfo->IsExistDesParam(desSame))
			{
				pvecItemID->push_back(*itr);
			}
		}

		itr++;
	}
}
// 2013-05-09 by hskim, ���� ����Ʈ ����

BOOL CFieldSkillManager::UseSkill(ITEM_SKILL* i_pItemSkill, MSG_FC_SKILL_USE_SKILL *i_pMsgUseSkill)
{
	if (i_pItemSkill == NULL) return FALSE;
	
	// get skill targets
	// 2009-12-11 by cmkwon, ��� ������ ó���� �ʿ� ���� - �Ʒ��� ���� ������.
	// 2015-11-03 DevX added different method for free war maps
	//	It allows party skills to be used without a party and blocks the usage of banned skills
	vectClientIndex_t	vectTargetList;

	if (!isFreeForAllMap(m_pFieldIOCPSocket->m_character.MapChannelIndex.MapIndex))

		GetSkillTargets(&vectTargetList, i_pMsgUseSkill->TargetIndex, i_pMsgUseSkill->TargetCharUID, i_pItemSkill->ItemInfo);

	else
	{
		GetSkillTargetsFreeWar(&vectTargetList, i_pMsgUseSkill->TargetIndex, i_pMsgUseSkill->TargetCharUID, i_pItemSkill->ItemInfo);

		if (vectTargetList.empty()) return FALSE;

		if (isBannedSkillWarMap(i_pItemSkill->ItemInfo->ItemNum)) return FALSE;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2009-12-11 by cmkwon, ������ ��׷η� Ÿ���� �����ϴ� ���� ���� - 
	setClientIndex_t	monsterIdxListForSkillAggro;

	BOOL bRet;
	switch(i_pItemSkill->ItemInfo->SkillType)
	{
	case SKILLTYPE_CLICK:
		bRet = UseSkillTypeClick(i_pItemSkill, i_pMsgUseSkill, &vectTargetList, &monsterIdxListForSkillAggro);			// 2009-12-11 by cmkwon, ������ ��׷η� Ÿ���� �����ϴ� ���� ���� - 
		break;
	case SKILLTYPE_TIMELIMIT:
		bRet = UseSkillTypeTimeLimit(i_pItemSkill, i_pMsgUseSkill, &vectTargetList, &monsterIdxListForSkillAggro);		// 2009-12-11 by cmkwon, ������ ��׷η� Ÿ���� �����ϴ� ���� ���� - 
		break;
	case SKILLTYPE_TOGGLE:
		bRet = UseSkillTypeToggle(i_pItemSkill, i_pMsgUseSkill, &monsterIdxListForSkillAggro);
		break;
	case SKILLTYPE_CHARGING:
		bRet = UseSkillTypeCharging(i_pItemSkill, i_pMsgUseSkill, &monsterIdxListForSkillAggro);
		break;
	default:
#ifdef _DEBUG
		m_pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, "ATTACKPROBABILITY01: %d @UseSkill", m_pFieldIOCPSocket->m_ParamFactor.pfp_ATTACKPROBABILITY_01);
#endif
		m_pFieldIOCPSocket->SendErrorMessage(T_FC_SKILL_USE_SKILL, ERR_PROTOCOL_CANNOT_USE_SKILL, i_pItemSkill->ItemNum);
		return FALSE;
	}

	// 2010-04-01 by cmkwon, ��ų ��� ���� ó�� - 
	if (bRet == FALSE) return FALSE;

	///////////////////////////////////////////////////////////////////////////////
	// 2009-12-11 by cmkwon, ������ ��׷η� Ÿ���� �����ϴ� ���� ���� - 
	float fSkillDamageForAggro = i_pItemSkill->ItemInfo->GetSkillDamageForAggro();
	if (0.0f < fSkillDamageForAggro && false == monsterIdxListForSkillAggro.empty())
	{
// 2009-12-10 by cmkwon, TEMP 
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] 091210 %s monstercountforAggro(%d) before unique \r\n"
//			, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), monsterIdxListForSkillAggro.size());

		CFieldMapChannel *pCurFMChann = m_pFieldIOCPSocket->GetCurrentFieldMapChannel();
		if(pCurFMChann)
		{
			int nSentCnt = 0;
			setClientIndex_t::iterator itr(monsterIdxListForSkillAggro.begin());
			for(; itr != monsterIdxListForSkillAggro.end(); itr++)
			{
				ClientIndex_t monIdx = *itr;
				CFieldMonster *pFMon = pCurFMChann->GetFieldMonster(monIdx, 10000);
				if(NULL == pFMon
					|| FALSE == pFMon->IsValidMonster()
					|| ATTACKOBJ_AGGRO != pFMon->MonsterInfoPtr->AttackObject)
				{
					continue;
				}

// 2009-12-10 by cmkwon, TEMP 
//				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] 091210 %s monstercountforAggro(%d) MonsterIndex(%d) \r\n"
//					, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), monsterIdxListForSkillAggro.size(), pFMon->MonsterIndex);
			
				///////////////////////////////////////////////////////////////////////
				// Attack ������ NPC���� ����
				INIT_MSG_WITH_BUFFER(MSG_FN_BATTLE_SET_ATTACK_CHARACTER, T_FN_BATTLE_SET_ATTACK_CHARACTER, pSetAttackChar, SendBuf);
				pSetAttackChar->ChannelIndex	= pCurFMChann->GetMapChannelIndex().ChannelIndex;
				pSetAttackChar->AttackIndex		= m_pFieldIOCPSocket->GetCharacter()->ClientIndex;
				pSetAttackChar->TargetIndex		= monIdx;
				pSetAttackChar->DamageAmount	= fSkillDamageForAggro;
				pSetAttackChar->ItemKind		= i_pItemSkill->Kind;
				pCurFMChann->Send2NPCServerW(SendBuf, MSG_SIZE(MSG_FN_BATTLE_SET_ATTACK_CHARACTER));

				nSentCnt++;
				if(10 <= nSentCnt)
				{
					break;
				}
			}

// 2009-12-10 by cmkwon, TEMP 
//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] 091210 %s monstercountforAggro(%d) after unique, nSentCnt(%d) \r\n"
//				, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), monsterIdxListForSkillAggro.size(), nSentCnt);
		}
	}


#ifdef _DEBUG
	m_pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, "ATTACKPROBABILITY01: %d @UseSkill", m_pFieldIOCPSocket->m_ParamFactor.pfp_ATTACKPROBABILITY_01);
#endif
	
	return bRet;
}

// Ŭ���� ��ų ó��
// 2009-12-11 by cmkwon, ������ ��׷η� Ÿ���� �����ϴ� ���� ���� - �Ʒ��� �Լ��� �����߰�(, setClientIndex_t *o_pMonListForSkillAggro)
BOOL CFieldSkillManager::UseSkillTypeClick(ITEM_SKILL *i_pItemSkill, MSG_FC_SKILL_USE_SKILL *i_pMsgUseSkill, vector<ClientIndex_t> *i_pVectorClientIndex, setClientIndex_t *o_pMonListForSkillAggro)
{
	INIT_MSG_OF_SIZE(MSG_FC_SKILL_USE_SKILL_OK, T_FC_SKILL_USE_SKILL_OK, msgUseSkillOK, SendBuf);
	int nSuccessCnt	= 0;
	for (int i = 0; i < i_pVectorClientIndex->size(); i++)
	{
		ClientIndex_t nTmpClientIndex = (*i_pVectorClientIndex)[i];

		CFieldIOCPSocket *pTargetSocket = ms_pFieldIOCP2->GetFieldIOCPSocket(nTmpClientIndex);

		// 2011-06-27 by shcho, �Ƹ���Ƽ�� ������ ���� - �ݿ��� ����� ���� ����
		// 2013-03-12 by jhseol, ��ų �ѹ��� �ý��� ����
#ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
		if( MGEAR_SKILL_BASENUM_CALLOFHERO == i_pItemSkill->ItemInfo->NextSkill )
#else	// #ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
		if(i_pItemSkill->ItemNum == 7813031)
#endif	// #ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
		// end 2013-03-12 by jhseol, ��ų �ѹ��� �ý��� ����
		{
			if(pTargetSocket->GetCharacter()->CurrentHP <= 0)
			{
				continue; // �ǰ� 0�̰ų�
			}
			if(COMPARE_BODYCON_BIT(pTargetSocket->GetCharacter()->BodyCondition,BODYCON_DEAD_MASK))
			{
				continue; // �ٵ�������� ����
			}
		}
		// end 2011-06-27 by shcho, �Ƹ���Ƽ�� ������ ���� - �ݿ��� ����� ���� ����

		// 2010-12-29 by shcho, �� �ε����� �ٸ��� ��� �Ұ�
		if (NULL == pTargetSocket
			|| FALSE == pTargetSocket->IsValidCharacter()
 			|| pTargetSocket->IsInvisibleCharacter()		// 2007-10-02 by cmkwon, ���� ������ ������ ��ų ��� �� ���� �Ұ�)
			// 2013-03-12 by jhseol, ��ų �ѹ��� �ý��� ����
#ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
			|| MGEAR_SKILL_BASENUM_CALLOFHERO != i_pItemSkill->ItemInfo->NextSkill )
#else	// #ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
			|| i_pItemSkill->ItemNum !=	7813031)	// 7813031 : �� ���� ��������� üũ
#endif	// #ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
			// end 2013-03-12 by jhseol, ��ų �ѹ��� �ý��� ����
		{
			// �� ���� ����ΰ� �ƴ� ��쿡�� �� üũ�� �ؾ��Ѵ�.
			if(m_pFieldIOCPSocket->GetCurrentFieldMapChannel()->GetMapChannelIndex() != pTargetSocket->GetCurrentFieldMapChannel()->GetMapChannelIndex())
			{
				continue;
			}
			
			if (!pTargetSocket->m_SkillManager.CheckSkillReqItemKind(i_pItemSkill->ItemInfo))
			{
				continue;
			}
			// 2011-03-28 by hsson, �̹� ���� ������ ��ü���� ���� Ÿ�� ������ ���� ����
			if(COMPARE_BODYCON_BIT(pTargetSocket->GetCharacter()->BodyCondition,BODYCON_DEAD_MASK))
			{
				continue;
			}
			// END 2011-03-28 by hsson, �̹� ���� ������ ��ü���� ���� Ÿ�� ������ ���� ����
		}
		// end 2010-12-29 by shcho, �� �ε����� �ٸ��� ��� �Ұ�


		////////////////////////////////////////////////////////////////////////////////
		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ������ ��� üũ
		if(COMPARE_BIT_FLAG(i_pItemSkill->ItemInfo->ItemAttribute, ITEM_ATTR_CHECK_SKILL_RESISTANCE_STATE) 
			&& TRUE == this->CheckResistance(pTargetSocket)) {
			// ���� ������ ��������~!!
			INIT_MSG_WITH_BUFFER(MSG_FC_INFO_APPLY_RESISTANCE_ITEM, T_FC_INFO_APPLY_RESISTANCE_ITEM, ResistanceMsg, ResistanceSendBuf);
			ResistanceMsg->ResistanceUserIdx		= pTargetSocket->m_character.ClientIndex;
			ResistanceMsg->SkillNum					= i_pItemSkill->ItemInfo->ItemNum;	// 2009-09-09 ~ 2010-01-19 by dhjin, ���Ǵ�Ƽ - ���׵� ��ų ���� �߰�
			ResistanceMsg->OnceResistanceItem		= FALSE;		// 2009-09-09 ~ 2010-01-19 by dhjin, ���Ǵ�Ƽ - �� ���� �����ϰ� �������� ���� ������ �߰�
			pTargetSocket->SendInRangeMessageAroundMe(ResistanceSendBuf, MSG_SIZE(MSG_FC_INFO_APPLY_RESISTANCE_ITEM), TRUE);
			continue;
		}
		////////////////////////////////////////////////////////////////////////////////
		// 2009-09-09 ~ 2010-01-19 by dhjin, ���Ǵ�Ƽ - �� ���� �����ϰ� �������� ���� ������ �߰�
		if(COMPARE_BIT_FLAG(i_pItemSkill->ItemInfo->ItemAttribute, ITEM_ATTR_CHECK_SKILL_RESISTANCE_STATE) 
			&& TRUE == this->CheckOnceResistance(pTargetSocket)) {
			// ���� ������ ��������~!!
			INIT_MSG_WITH_BUFFER(MSG_FC_INFO_APPLY_RESISTANCE_ITEM, T_FC_INFO_APPLY_RESISTANCE_ITEM, ResistanceMsg, ResistanceSendBuf);
			ResistanceMsg->ResistanceUserIdx		= pTargetSocket->m_character.ClientIndex;
			ResistanceMsg->SkillNum					= i_pItemSkill->ItemInfo->ItemNum;	// 2009-09-09 ~ 2010-01-19 by dhjin, ���Ǵ�Ƽ - ���׵� ��ų ���� �߰�
			ResistanceMsg->OnceResistanceItem		= TRUE;			// 2009-09-09 ~ 2010-01-19 by dhjin, ���Ǵ�Ƽ - �� ���� �����ϰ� �������� ���� ������ �߰�
			pTargetSocket->SendInRangeMessageAroundMe(ResistanceSendBuf, MSG_SIZE(MSG_FC_INFO_APPLY_RESISTANCE_ITEM), TRUE);
			continue;
		}

		// 2007-07-11 by dhjin, �ɸ��� �����̸� BigBoom ��� �Ұ� ����
		// 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - ITEM::IsExistDesParam() �Լ��� ������
		//if(IS_EXIST_DES_PARAM(i_pItemSkill->ItemInfo, DES_SKILL_BIG_BOOM))
		if(i_pItemSkill->ItemInfo->IsExistDesParam(DES_SKILL_BIG_BOOM))
		{
			if(m_pFieldIOCPSocket->m_character.CharacterMode)
			{
				return FALSE;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// 2008-05-20 by dhjin, ��� ���� ��� ��ȭ ��ų ���� ��ȯ ��Ż ��� �ȵǴ� ���� ����
		// 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - ITEM::IsExistDesParam() �Լ��� ������
		//if(IS_EXIST_DES_PARAM(i_pItemSkill->ItemInfo, DES_SKILL_CANCELALL))
		if(i_pItemSkill->ItemInfo->IsExistDesParam(DES_SKILL_CANCELALL))
		{
			pTargetSocket->m_SkillManager.SetPrepareUseBigBoom(FALSE);
		}

		///////////////////////////////////////////////////////////////////////////////
		// 2005-11-24 by cmkwon, ���� ��ȯ ��ų ó��
		// 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - ITEM::IsExistDesParam() �Լ��� ������
		//if(IS_EXIST_DES_PARAM(i_pItemSkill->ItemInfo, DES_SKILL_SUMMON_FORMATION_MEMBER))
		if(i_pItemSkill->ItemInfo->IsExistDesParam(DES_SKILL_SUMMON_FORMATION_MEMBER))
		{// ���� ��ȯ ��ų�̸�

			// 2009-09-09 ~ 2010-01-20 by dhjin, ���Ǵ�Ƽ - ���� ���� ���� ���μ����� �����Ѵ�. ���� ���� üũ
			if(FALSE == g_pFieldGlobal->IsArenaServer()
				&& INFINITY_STATE_NONE != pTargetSocket->m_InfinityPlayingInfo.InfinityState ) {
				// ���μ������� ���� �������� ������ ��ȯ �Ұ����ϴ�.
				return FALSE;
			}
			
			if(m_pFieldIOCPSocket->GetCharacter()->InfluenceType != pTargetSocket->GetCharacter()->InfluenceType)
			{// 2007-05-10 by cmkwon, ���� ��ȯ ��ų�� ������ �ٸ��� ��� �Ұ�
				return FALSE;
			}

			CFieldMapProject *pFMPro = m_pFieldIOCPSocket->GetCurrentFieldMapProject();
			if(NULL == pFMPro)
			{
				pTargetSocket->SendErrorMessage(T_FC_SKILL_USE_SKILL, ERR_INVALID_CHARACTER, m_pFieldIOCPSocket->GetCharacter()->CharacterUniqueNumber, i_pItemSkill->ItemNum);
				return FALSE;
			}

// 2008-02-12 by cmkwon, IS_CITY_MAP_INDEX() ��ũ�θ� �������� ���ø����� ���θ� ���� - �Ʒ��� ���� ����			
// 			if(IS_CITY_MAP_INDEX(pFMPro->m_nMapIndex)
// 				|| IS_BAZAAR_MAP_INDEX(pFMPro->m_nMapIndex)
// 				|| IS_OUTPOST_CITY_MAP_INDEX(pFMPro->m_nMapIndex)				// 2007-09-28 by cmkwon, �������� ���øʿ����� �ݿ�������� �Ұ�
// 				|| IS_MAP_INFLUENCE_EVENT_AREA(pFMPro->m_nMapInfluenceType))	// 2006-11-17 by cmkwon, �̺�Ʈ�ʿ����� �ݿ�������� �Ұ�
			if(IS_CITY_MAP_INDEX(pFMPro->m_nMapIndex)
				|| IS_MAP_INFLUENCE_EVENT_AREA(pFMPro->m_nMapInfluenceType))	// 2006-11-17 by cmkwon, �̺�Ʈ�ʿ����� �ݿ�������� �Ұ�
			{// 2006-07-20 by cmkwon, 
				return FALSE;
			}

			// start 2011-12-19 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
			if( 0 != pFMPro->m_EntranceCondition.size() )						// �� ���� ���ǿ� �������� �ִ� ��쿡�� �ݿ�������� ��� �Ұ�
			{
				return FALSE;
			}
			// end 2011-12-19 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���

			//////////////////////////////////////////////////////////////////////////
			// 2007-05-02 by dhjin, �Ʒ��� �ʿ����� �Ʒ��� �� �ȿ����� �ݿ�������� ��ų�� ��밡���ϴ�.
			if(IS_MAP_INFLUENCE_ARENA(pFMPro->GetMapInfluenceType())
				&& m_pFieldIOCPSocket->GetCurrentFieldMapChannel() != pTargetSocket->GetCurrentFieldMapChannel())
			{
				return FALSE;
			}

			// 2010. 11. 24. by hsLee.	�����ڰ� ����Ÿ�� ���� ��� Ÿ���� ����Ÿ�� �� ���� ���� üũ.
			if ( NULL != ms_pFieldIOCP2->GetBurningMapInfo ( m_pFieldIOCPSocket->GetCharacter()->MapChannelIndex.MapIndex ) 
				&& ERR_NO_ERROR != pTargetSocket->CheckPossibleBurningMapWarp ( m_pFieldIOCPSocket->GetCharacter()->MapChannelIndex.MapIndex ) )
			{
				m_pFieldIOCPSocket->SendErrorMessage ( T_FC_SKILL_USE_SKILL , ERR_PROTOCOL_NOT_WARP_BURNING_MAP , m_pFieldIOCPSocket->GetCharacter()->CharacterUniqueNumber , i_pItemSkill->ItemNum );
				return FALSE;
			}

			// 2016-01-04 Future, CoH cannot be used on warp restricted users
#ifdef SC_LEADER_WARP_RESTRICTION
			if (pTargetSocket->IsWarpRestricted())
			{
				m_pFieldIOCPSocket->SendErrorMessage(T_FC_SKILL_USE_SKILL, ERR_WARP_RESTRICTED);
				return FALSE;
			}
#endif // SC_LEADER_WARP_RESTRICTION

			//////////////////////////////////////////////////////////////////////////
			// 2007-08-28 by dhjin, �������� �� üũ
			if(IS_MAP_INFLUENCE_OUTPOST(pFMPro->GetMapInfluenceType()) )
			{
				if(FALSE == ms_pFieldIOCP2->m_OutPostManager.CheckOutPostWaring(m_pFieldIOCPSocket->m_character.MapChannelIndex.MapIndex))
				{
					return FALSE;
				}
			}

			Err_t nRetErr = pTargetSocket->IsEnableWarp(pFMPro->GetMapInfluenceType(), pFMPro->IsConflictAreaMap(), pFMPro->m_nQuestIndexForWarp);
			if(ERR_NO_ERROR != nRetErr)
			{// 2005-12-02 by cmkwon, ���� �Ұ�
				pTargetSocket->SendErrorMessage(T_FC_SKILL_USE_SKILL, nRetErr, m_pFieldIOCPSocket->GetCharacter()->CharacterUniqueNumber, i_pItemSkill->ItemNum);
				return FALSE;
			}

			// 2009-04-06 by cmkwon, �ݿ��� ����� ��ų �ý��� ���� - �Ʒ��� ���� ���� ��.
			//EventResult_t nRetedResult = pTargetSocket->WarpToMap(m_pFieldIOCPSocket->GetCharacter()->MapChannelIndex, &(m_pFieldIOCPSocket->m_character.PositionVector)); 
			//if(EVENT_RESULT_CONTINUE != nRetedResult)
			//{// ���� ����
			//	return FALSE;
			//}
			// 2009-04-06 by cmkwon, �ݿ��� ����� ��ų �ý��� ���� - �Ʒ��� ���� ó�� ��. ��ȿ�ð� 15��
			INIT_MSG_WITH_BUFFER(MSG_FC_SKILL_CONFIRM_USE, T_FC_SKILL_CONFIRM_USE, pSMsg, SendBuf);
			util::strncpy(pSMsg->szAttackCharacterName, m_pFieldIOCPSocket->GetCharacter()->CharacterName, SIZE_MAX_CHARACTER_NAME);
			pSMsg->AttackCharacterUID	= m_pFieldIOCPSocket->GetCharacter()->CharacterUniqueNumber;
			pSMsg->TargetCharacterUID	= pTargetSocket->GetCharacter()->CharacterUniqueNumber;
			pSMsg->UsingSkillItemNum	= i_pItemSkill->ItemNum;
			pSMsg->MapChannelIndex		= m_pFieldIOCPSocket->GetCharacter()->MapChannelIndex;
			pSMsg->SkillConfirmUseUID	= this->AddSkillConfirmUse(pSMsg->TargetCharacterUID, pSMsg->UsingSkillItemNum, 10+5, pSMsg->MapChannelIndex, m_pFieldIOCPSocket->GetCharacter()->PositionVector);
			pTargetSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_SKILL_CONFIRM_USE));
		}

		//////////////////////////////////////////////////////////////////////////
		// 2006-11-17 by dhjin, 171 - ��ü�� HP, ����, ����, SP�� ����ȸ��
		// 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - ITEM::IsExistDesParam() �Լ��� ������
		//if(IS_EXIST_DES_PARAM(i_pItemSkill->ItemInfo, DES_SKILL_FULL_RECOVERY))
		if(i_pItemSkill->ItemInfo->IsExistDesParam(DES_SKILL_FULL_RECOVERY))
		{
			pTargetSocket->m_character.CurrentHP	= pTargetSocket->m_character.HP;
			pTargetSocket->m_character.CurrentDP	= pTargetSocket->m_character.DP;
			pTargetSocket->m_character.CurrentSP	= pTargetSocket->m_character.SP;
			pTargetSocket->m_character.CurrentEP	= pTargetSocket->m_character.EP;
			pTargetSocket->SendCharacterInfo(T_FC_CHARACTER_CHANGE_CURRENTHPDPSPEP);
		}

		///////////////////////////////////////////////////////////////////////////////
		// 2006-12-18 by cmkwon, DP ȸ�� �Ұ�
		// 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - ITEM::IsExistDesParam() �Լ��� ������
		//if(IS_EXIST_DES_PARAM(i_pItemSkill->ItemInfo, DES_IMMEDIATE_DP_UP)
		if(i_pItemSkill->ItemInfo->IsExistDesParam(DES_IMMEDIATE_DP_UP)
			&& FALSE == pTargetSocket->IsEnableRepairDP())
		{
			continue;
		}
		
		nSuccessCnt++;

		// check: Ŭ���� ��ų�� ParamFactor ������ ����, 20041125, kelovon w/ dhkwon, sjmin
		// 2004-12-06 by cmkwon, ��������/��������� ���(������ Ŭ���� ��ų���� �ʿ����-DesParam�� �ʱ�ȭ �Ǿ� �־����)
		// set paramfactor
		float tmBuffPenalty = 1.0f;
#ifdef NEMERIAN_NATION_BALANCE_BUFF_PENALITY
		int diff = g_diffVictoryWarByroo;
		if ((diff < 0 && TRUE == COMPARE_INFLUENCE(pTargetSocket->m_character.InfluenceType, INFLUENCE_TYPE_ANI)) ||
			(diff > 0 && TRUE == COMPARE_INFLUENCE(pTargetSocket->m_character.InfluenceType, INFLUENCE_TYPE_VCN))) {
			int levelPenality = abs(diff) / 22;
			tmBuffPenalty = 1.0 - (0.05f * levelPenality);
			//char szSystem[1024];
			//sprintf(szSystem, "[NEMERIAN DEBUG] penality applied to skill: %f, level penality: %d", tmBuffPenalty, levelPenality);
			//g_pFieldGlobal->WriteSystemLog(szSystem);
		}
#endif /*NEMERIAN_NATION_BALANCE_BUFF_PENALITY*/
 		pTargetSocket->m_SkillManager.SetSkillParamFactor(i_pItemSkill->ItemInfo, m_pFieldIOCPSocket->GetCharacter()->ClientIndex, tmBuffPenalty);

		////////////////////////////////////////////////////////////////////////////////
		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - OrbitSkill ����
		if(TRUE == IS_ORBIT_SKILL(i_pItemSkill->ItemInfo->OrbitType)) {
			pTargetSocket->m_SkillManager.ApplyOrbitSkill(i_pItemSkill->ItemInfo, m_pFieldIOCPSocket->GetCharacter()->ClientIndex);
		}

		msgUseSkillOK->SkillItemID	= i_pMsgUseSkill->SkillItemID;
		msgUseSkillOK->AttackIndex	= i_pMsgUseSkill->AttackIndex;
		msgUseSkillOK->TargetIndex	= nTmpClientIndex;
		msgUseSkillOK->UseTime		= i_pItemSkill->UseTime;			// 2006-12-05 by dhjin, ��ų ��� �ð�
#ifdef NEMERIAN_NATION_BALANCE_BUFF_PENALITY
		msgUseSkillOK->penality = tmBuffPenalty;
#endif /* NEMERIAN_NATION_BALANCE_BUFF_PENALITY */

//		// 2007-02-05 by dhjin, ��ĵ ��ų 10�� ���������� ����
//		// �ڽŰ� ������ �Ʊ׵鿡�Ե� ����
//		if(IS_EXIST_DES_PARAM(i_pItemSkill->ItemInfo, DES_SKILL_SCANNING))
//		{// 2006-11-27 by dhjin, ��Ȯ�� �Ÿ��� üũ�Ͽ� ����
//			pTargetSocket->SendInRangeMessageExactAroundMe(SendBuf, MSG_SIZE(MSG_FC_SKILL_USE_SKILL_OK), i_pItemSkill->ItemInfo->Range, TRUE);
//		}
//		else
//		{// 2006-11-27 by dhjin
//			pTargetSocket->SendInRangeMessageAroundMe(SendBuf, MSG_SIZE(MSG_FC_SKILL_USE_SKILL_OK), TRUE);
//		}
		pTargetSocket->SendInRangeMessageAroundMe(SendBuf, MSG_SIZE(MSG_FC_SKILL_USE_SKILL_OK), TRUE);

		///////////////////////////////////////////////////////////////////////////////
		// 2009-12-11 by cmkwon, ������ ��׷η� Ÿ���� �����ϴ� ���� ���� - 
		if(0.0f < i_pItemSkill->ItemInfo->GetSkillDamageForAggro())
		{
			pTargetSocket->GetAttackedMonsterIndex(o_pMonListForSkillAggro);
		}

#ifdef _DEBUG
		m_pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, "SKILL Target: %s",
			GetCharacterString(ms_pFieldIOCP2->GetCharacterByArrayIndex(nTmpClientIndex), string()));
#endif
	}
	// 2005-11-24 by cmkwon, ��ų�� ����� ����� ���ٸ� ��ų ��� ����
	if(0 == nSuccessCnt)
	{
		return FALSE;
	}
	return TRUE;
}

// 2009-12-11 by cmkwon, ������ ��׷η� Ÿ���� �����ϴ� ���� ���� - �Ʒ��� �Լ��� �����߰�(, setClientIndex_t *o_pMonListForSkillAggro)
BOOL CFieldSkillManager::UseSkillTypeTimeLimit(ITEM_SKILL *i_pItemSkill, MSG_FC_SKILL_USE_SKILL *i_pMsgUseSkill, vector<ClientIndex_t> *i_pVectorClientIndex, setClientIndex_t *o_pMonListForSkillAggro)
{
	// ������ ���ؼ��� Ŭ���̾�Ʈ���� �׻� cancel_skill�� �̸� ������, 20041015, kelovon with jschoi
// 2006-12-28 by cmkwon, �ڽ��� ����� ��ų�� üũ�ؾ���
//	BOOL bSkillActivated = IsSkillActivatedByItemNum(i_pItemSkill->ItemNum);
	
	// 2013-05-09 by hskim, ���� ����Ʈ ����
	BOOL bSkillActivated = FALSE;
	
	if( 0 == i_pItemSkill->UniqueNumber )
	{
		bSkillActivated = IsSkillActivatedByItemNum(i_pItemSkill->ItemNum);
		// 2013-08-01 by jhseol, ������ ���� ������ - �������� ������ ������ ������ ��ġ�� ������ �ش�
		if ( TRUE == bSkillActivated )
		{
			for (int i = 0; i < i_pVectorClientIndex->size(); i++)
			{
				ClientIndex_t nTmpClientIndex = (*i_pVectorClientIndex)[i];
				
				CFieldIOCPSocket *pTargetSocket = ms_pFieldIOCP2->GetFieldIOCPSocket(nTmpClientIndex);
				if (pTargetSocket == NULL || FALSE == pTargetSocket->IsValidCharacter())
				{
					continue;
				}
				float tmBuffPenalty = 1.0f;
				ITEM *pItemInfo = i_pItemSkill->ItemInfo;
				if ( NULL != pItemInfo )
				{
					int CurrentDiffContributionPoint = ms_pFieldIOCP2->m_InflWarManager.GetTurnAroundPoint();
					if( (CurrentDiffContributionPoint < 0 && TRUE == COMPARE_INFLUENCE(pTargetSocket->m_character.InfluenceType, INFLUENCE_TYPE_ANI)) ||
						(CurrentDiffContributionPoint > 0 && TRUE == COMPARE_INFLUENCE(pTargetSocket->m_character.InfluenceType, INFLUENCE_TYPE_VCN)) )
					{
						tmBuffPenalty = ms_pFieldIOCP2->m_InflWarManager.GetPVPBuffPercent(CurrentDiffContributionPoint);
					}
					else
					{
						tmBuffPenalty = 0.0f;
					}
					pTargetSocket->m_SkillManager.SetSkillParamFactor(pItemInfo, m_pFieldIOCPSocket->GetCharacter()->ClientIndex, tmBuffPenalty);
				}
			}
		}
		// end 2013-08-01 by jhseol, ������ ���� ������ - �������� ������ ������ ������ ��ġ�� ������ �ش�
	}
	else bSkillActivated = IsSkillActivatedByUID(i_pItemSkill->UniqueNumber);
	// end 2013-05-09 by hskim, ���� ����Ʈ ����

	if (bSkillActivated)
	{
		return FALSE;
	}

	BOOL bBreak = FALSE;		// 2006-12-04 by dhjin
	INIT_MSG_OF_SIZE(MSG_FC_SKILL_USE_SKILL_OK, T_FC_SKILL_USE_SKILL_OK, msgUseSkillOK, SendBuf);
	int nSuccessCnt	= 0;
	for (int i = 0; i < i_pVectorClientIndex->size(); i++)
	{
		ClientIndex_t nTmpClientIndex = (*i_pVectorClientIndex)[i];

		CFieldIOCPSocket *pTargetSocket = ms_pFieldIOCP2->GetFieldIOCPSocket(nTmpClientIndex);
		if (pTargetSocket == NULL
			|| FALSE == pTargetSocket->IsValidCharacter()
			|| pTargetSocket->IsInvisibleCharacter())		// 2007-10-02 by cmkwon, ���� ������ ������ ��ų ��� �� ���� �Ұ�
		{
			continue;
		}

		if (!pTargetSocket->m_SkillManager.CheckSkillReqItemKind(i_pItemSkill->ItemInfo))
		{
			continue;
		}

		ITEM *pItemInfo = i_pItemSkill->ItemInfo;

		// ������ ���� BaseNum�� ��ų�� ����ϰ� �ִ��� Ȯ��
		// 2013-03-12 by jhseol, ��ų �ѹ��� �ý��� ����
#ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
		INT nOldSkillItemNum = pTargetSocket->m_SkillManager.IsSkillActivatedBySkillBaseNum(i_pItemSkill->ItemInfo->NextSkill);
		BYTE nOldSkillLevel = pTargetSocket->m_SkillManager.GetSkillActivatedLevelBySkillBaseNum(i_pItemSkill->ItemInfo->NextSkill);
		if (nOldSkillLevel != 0)
		{
			if ( nOldSkillLevel <= i_pItemSkill->ItemInfo->SkillLevel )
			{
				// �̹� ����ϰ� �ִ� ���
				pTargetSocket->m_SkillManager.DeactivateSkillByItemNum(nOldSkillItemNum, FALSE);
			}
			else
			{
				// check: ������ �� ����, ���߿� �ʿ��ϸ� �츲, kelovon with jschoi, 20041018
				continue;
			}
		}
#else	// #ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
		INT nOldSkillItemNum = pTargetSocket->m_SkillManager.IsSkillActivatedBySkillBaseNum(i_pItemSkill->ItemNum);
		if (nOldSkillItemNum != 0)
		{
			if (SKILL_LEVEL(nOldSkillItemNum) <= SKILL_LEVEL(i_pItemSkill->ItemNum))
			{
				// �̹� ����ϰ� �ִ� ���
				pTargetSocket->m_SkillManager.DeactivateSkillByItemNum(nOldSkillItemNum, FALSE);
			}
			else
			{
				// check: ������ �� ����, ���߿� �ʿ��ϸ� �츲, kelovon with jschoi, 20041018
				continue;
			}
		}
#endif	// #ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
		// end 2013-03-12 by jhseol, ��ų �ѹ��� �ý��� ����

		// set paramfactor
		// 2013-05-07 by jhseol,bckim ���� �г�Ƽ
		float tmBuffPenalty = 1.0f;
#ifdef SC_BUFF_PENALTY_JHSEOL_BCKIM
		if ( TRUE == pItemInfo->IsExistDesParam(DES_BUFF_PENALTY_RATIO) )					// ���� �г�Ƽ DES�� ������
		{
			USHORT tmApplirdUnitKind = (USHORT)pItemInfo->GetParameterValue(DES_BUFF_PENALTY_APPLIED_UNIT_KIND);
			if ( 0 != (pTargetSocket->m_character.UnitKind & tmApplirdUnitKind) )		// ������ �� ���Ͽ�
			{
				tmBuffPenalty = pItemInfo->GetParameterValue(DES_BUFF_PENALTY_RATIO);		// �г�Ƽ�� ���� ��Ų��.
			}
		}
#endif
		// end 2013-05-07 by jhseol,bckim ���� �г�Ƽ
		// 2013-08-01 by jhseol, ������ ���� ������
#ifdef S_UPGRADE_INFLUENCE_POINT_HSKIM
		if ( TRUE == pItemInfo->IsExistDesParam(DES_SKILLTYPE_TRUN_AROUND) )	// ������ ���� ���� �ϴ�
		{
			int CurrentDiffContributionPoint = ms_pFieldIOCP2->m_InflWarManager.GetTurnAroundPoint();
			if( (CurrentDiffContributionPoint < 0 && TRUE == COMPARE_INFLUENCE(pTargetSocket->m_character.InfluenceType, INFLUENCE_TYPE_ANI)) ||
				(CurrentDiffContributionPoint > 0 && TRUE == COMPARE_INFLUENCE(pTargetSocket->m_character.InfluenceType, INFLUENCE_TYPE_VCN)) )
			{
				tmBuffPenalty = ms_pFieldIOCP2->m_InflWarManager.GetPVPBuffPercent(CurrentDiffContributionPoint);
			}
			else
			{
				tmBuffPenalty = 0.0f;
			}
		}
#endif
#ifdef NEMERIAN_NATION_BALANCE_BUFF_PENALITY
		if (i_pItemSkill->ItemInfo->SkillType == SKILLTYPE_TIMELIMIT) {
			int diff = g_diffVictoryWarByroo;
			if ((diff < 0 && TRUE == COMPARE_INFLUENCE(pTargetSocket->m_character.InfluenceType, INFLUENCE_TYPE_ANI)) ||
				(diff > 0 && TRUE == COMPARE_INFLUENCE(pTargetSocket->m_character.InfluenceType, INFLUENCE_TYPE_VCN))) {
				int levelPenality = abs(diff) / 22;
				tmBuffPenalty = 1.0 - (0.05f * levelPenality);
			}
		}

#endif
		// end 2013-08-01 by jhseol, ������ ���� ������
//		pTargetSocket->m_SkillManager.SetSkillParamFactor(pItemInfo, m_pFieldIOCPSocket->GetCharacter()->ClientIndex);
		pTargetSocket->m_SkillManager.SetSkillParamFactor(pItemInfo, m_pFieldIOCPSocket->GetCharacter()->ClientIndex, tmBuffPenalty);

		////////////////////////////////////////////////////////////////////////////////
		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - OrbitSkill ����
		if(TRUE == IS_ORBIT_SKILL(i_pItemSkill->ItemInfo->OrbitType)) {
			pTargetSocket->m_SkillManager.ApplyOrbitSkill(i_pItemSkill->ItemInfo, m_pFieldIOCPSocket->GetCharacter()->ClientIndex);
		}

// check: �׳� ��� �ϱ�, 20041030, kelovon with ClientTeam
//		// check: shotnum�� ������Ű�� ��ų�̸� ���� �����ϱ�, 20040902, kelovon
//		//        ���� ���� ��� shotnum ���� ��ų ����ϸ� �Ѿ� �� ������ ���� ������ ����
//		if (pItemInfo->DestParameter1 == DES_SHOTNUM_01 || pItemInfo->DestParameter2 == DES_SHOTNUM_01 ||
//			pItemInfo->DestParameter3 == DES_SHOTNUM_01 || pItemInfo->DestParameter4 == DES_SHOTNUM_01)
//		{
//			pTargetSocket->m_PrimaryShotNumCount
//				= CAtumSJ::GetTotalPrimaryShotCountPerReattackTime(&pTargetSocket->m_ItemProwOut, &pTargetSocket->m_ParamFactor);
//		}

		//////////////////////////////////////////////////////////////////////////
		// 81, 2006-12-11 by dhjin - A����� ����óġ�� ���
		// 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - ITEM::IsExistDesParam() �Լ��� ������
		//if (IS_EXIST_DES_PARAM(pItemInfo, DES_GRADUAL_DP_UP))
		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 
		if (pItemInfo->IsExistDesParam(DES_GRADUAL_DP_UP)
			&& FALSE == pTargetSocket->m_SkillManager.m_FieldDebuff.CheckApplyingDebuff(DES_SKILL_MON_FREEZE_DP))
		{
			pTargetSocket->m_TimerManager.m_nRemainedTimeOfGradualDPUP		= pItemInfo->Time;
			pTargetSocket->m_TimerManager.StartTimerField(TE_TYPE_GRADUAL_DP_UP, TIMER_GRADUAL_UP_INTERVAL, (UINT)pItemInfo);
		}
		
		//////////////////////////////////////////////////////////////////////////
		// 124, 2006-12-04 by dhjin - B����� ü�� ����
		// 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - ITEM::IsExistDesParam() �Լ��� ������
		//if (IS_EXIST_DES_PARAM(pItemInfo, DES_SKILL_HALLUCINATION))
		if (pItemInfo->IsExistDesParam(DES_SKILL_HALLUCINATION))
		{			
			mt_auto_lock dummylock(&pTargetSocket->m_mtvectFieldDummy);
			if(false == pTargetSocket->m_mtvectFieldDummy.empty())
			{// 2006-12-04 by dhjin, ���Ȱ��� ���� ������� ����
				return FALSE;
			}
// 2007-06-21 by cmkwon, ü�� �ϳ��� �޽����� ��� ���� - �Ʒ��� ���� ����			
//			int nSendByte = 0;
//			INIT_MSG_WITH_BUFFER(MSG_FC_BATTLE_DROP_DUMMY_OK, T_FC_BATTLE_DROP_DUMMY_OK, pMsgDropDummyOK, SendBuf);
//			for (int i = 0; i < pItemInfo->GetParameterValue(DES_SKILL_HALLUCINATION); i++)
//			{
//				if(SIZE_MAX_PACKET < nSendByte + MSG_SIZE(MSG_FC_BATTLE_DROP_DUMMY_OK))
//				{
//					break;
//				}
//				FIELD_DUMMY *pFieldDummy = new FIELD_DUMMY(pTargetSocket->m_uiDummyItemFieldIndex++, pItemInfo);
//				pTargetSocket->m_mtvectFieldDummy.push_back(pFieldDummy);					
//				
//				pMsgDropDummyOK->ItemNum		= pItemInfo->ItemNum;
//				pMsgDropDummyOK->AttackIndex	= pTargetSocket->m_character.ClientIndex;
//				pMsgDropDummyOK->ItemFieldIndex = pFieldDummy->ItemFieldIndex;
//				
//				pTargetSocket->m_TimerManager.StartTimerField(TE_TYPE_DELETE_DUMMY, pItemInfo->Time, pFieldDummy->ItemFieldIndex);
//
//				nSendByte		+= MSG_SIZE(MSG_FC_BATTLE_DROP_DUMMY_OK);
//				*(MessageType_t*)(SendBuf+nSendByte)	= T_FC_BATTLE_DROP_DUMMY_OK;
//				pMsgDropDummyOK							= (MSG_FC_BATTLE_DROP_DUMMY_OK*)(SendBuf + nSendByte + SIZE_FIELD_TYPE_HEADER);
//			}
			pTargetSocket->m_bDummyCheck	= TRUE;
			for (int i = 0; i < pItemInfo->GetParameterValue(DES_SKILL_HALLUCINATION); i++)
			{
				FIELD_DUMMY *pFieldDummy = new FIELD_DUMMY(pTargetSocket->m_uiDummyItemFieldIndex++, pItemInfo);
				pTargetSocket->m_mtvectFieldDummy.push_back(pFieldDummy);					
				pTargetSocket->m_TimerManager.StartTimerField(TE_TYPE_DELETE_DUMMY, pItemInfo->Time, pFieldDummy->ItemFieldIndex);
			}
			int		nSendByte = 0;
			BYTE	SendBuf[SIZE_MAX_PACKET];
			if(pTargetSocket->Make_MSG_FC_BATTLE_DROP_DUMMY_OK(SendBuf, &nSendByte, SIZE_MAX_PACKET))
			{
				CFieldIOCPSocket::ms_pFieldIOCP->SendInRangeMessageAroundCharacter(pTargetSocket->m_character.ClientIndex, SendBuf, nSendByte
						, pTargetSocket->GetCurrentFieldMapChannel(), TRUE, pTargetSocket->GetCurrentFieldMapChannel()->GetUserVisibleDiameterW());
			}
			pTargetSocket->m_uiDummyItemFieldIndex += 100;			
			bBreak							= TRUE;
		}		
		pTargetSocket->m_SkillManager.m_vectorAtivatedSkill.pushBackLock(ItemID_t(i_pItemSkill->UniqueNumber, i_pItemSkill->ItemNum));

		// MSG ����
		msgUseSkillOK->SkillItemID	= i_pMsgUseSkill->SkillItemID;
		msgUseSkillOK->AttackIndex	= i_pMsgUseSkill->AttackIndex;
		msgUseSkillOK->TargetIndex	= nTmpClientIndex;
		msgUseSkillOK->UseTime		= i_pItemSkill->UseTime;			// 2006-12-05 by dhjin, ��ų ��� �ð�
#ifdef NEMERIAN_NATION_BALANCE_BUFF_PENALITY
		msgUseSkillOK->penality = tmBuffPenalty;
#endif
		///////////////////////////////////////////////////////////////////////////////
		// 2007-02-07 by cmkwon
		// 1. ��ĳ�� ��ų�� ��� ��ü �������Դ� �ѹ��� ������ Ÿ�� ���� ��� ������ Ÿ�� �������Ը� �����Ѵ�.
		// 2. ������ ��ų�� ��� ������ ��� �������� �����Ѵ�.
		// 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - ITEM::IsExistDesParam() �Լ��� ������
		//if(IS_EXIST_DES_PARAM(i_pItemSkill->ItemInfo, DES_SKILL_SCANNING)
		if(i_pItemSkill->ItemInfo->IsExistDesParam(DES_SKILL_SCANNING)
			&& i_pMsgUseSkill->AttackIndex != nTmpClientIndex)
		{
			pTargetSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_SKILL_USE_SKILL_OK));
		}
		else
		{
			pTargetSocket->SendInRangeMessageAroundMe(SendBuf, MSG_SIZE(MSG_FC_SKILL_USE_SKILL_OK), TRUE);
		}

		nSuccessCnt++;		// 2006-12-04 by dhjin

		///////////////////////////////////////////////////////////////////////////////
		// 2009-12-11 by cmkwon, ������ ��׷η� Ÿ���� �����ϴ� ���� ���� - 
		if(0.0f < i_pItemSkill->ItemInfo->GetSkillDamageForAggro())
		{
			pTargetSocket->GetAttackedMonsterIndex(o_pMonListForSkillAggro);
		}

		if(bBreak)
		{// 2006-12-04 by dhjin
			break;
		}
	}
	// 2005-11-24 by cmkwon, ��ų�� ����� ����� ���ٸ� ��ų ��� ����
	if(0 == nSuccessCnt)
	{
		return FALSE;
	}
	return TRUE;
}

// 2009-12-11 by cmkwon, ������ ��׷η� Ÿ���� �����ϴ� ���� ���� - �Ʒ��� �Լ��� �����߰�(, setClientIndex_t *o_pMonListForSkillAggro)
BOOL CFieldSkillManager::UseSkillTypeToggle(ITEM_SKILL *i_pItemSkill, MSG_FC_SKILL_USE_SKILL *i_pMsgUseSkill, setClientIndex_t *o_pMonListForSkillAggro)
{
	// ������ ���ؼ��� Ŭ���̾�Ʈ���� �׻� cancel_skill�� �̸� ������, 20041015, kelovon with jschoi
	BOOL bSkillActivated = IsSkillActivatedByItemNum(i_pItemSkill->ItemNum);
	if (bSkillActivated)
	{
		return FALSE;
	}

	ITEM *pItemInfo = i_pItemSkill->ItemInfo;
	
	// set paramfactor
	m_pFieldIOCPSocket->m_SkillManager.SetSkillParamFactor(pItemInfo);

// check: �׳� ��� �ϱ�, 20041030, kelovon with ClientTeam
//	// check: shotnum�� ������Ű�� ��ų�̸� ���� �����ϱ�, 20040902, kelovon
//	//        ���� ���� ��� shotnum ���� ��ų ����ϸ� �Ѿ� �� ������ ���� ������ ����
//	if (pItemInfo->DestParameter1 == DES_SHOTNUM_01 || pItemInfo->DestParameter2 == DES_SHOTNUM_01 ||
//		pItemInfo->DestParameter3 == DES_SHOTNUM_01 || pItemInfo->DestParameter4 == DES_SHOTNUM_01)
//	{
//		m_pFieldIOCPSocket->m_PrimaryShotNumCount
//			= CAtumSJ::GetTotalPrimaryShotCountPerReattackTime(&m_pFieldIOCPSocket->m_ItemProwOut, &m_pFieldIOCPSocket->m_ParamFactor);
//	}

	// Ÿ�̸� ����
	m_pFieldIOCPSocket->m_TimerManager.StartTimerField(TE_TYPE_DECREASE_SP, TIMER_DECREASE_SP_INTERVAL,
				HIDWORD(i_pItemSkill->UniqueNumber), LODWORD(i_pItemSkill->UniqueNumber), pItemInfo->ItemNum, TIMER_REPEAT_INFINITE);

	m_vectorAtivatedSkill.pushBackLock(ItemID_t(i_pItemSkill->UniqueNumber, i_pItemSkill->ItemNum));

	INIT_MSG_OF_SIZE(MSG_FC_SKILL_USE_SKILL_OK, T_FC_SKILL_USE_SKILL_OK, msgUseSkillOK, SendBuf);
	msgUseSkillOK->SkillItemID	= i_pMsgUseSkill->SkillItemID;
	msgUseSkillOK->AttackIndex	= i_pMsgUseSkill->AttackIndex;
	msgUseSkillOK->TargetIndex	= i_pMsgUseSkill->TargetIndex;
	msgUseSkillOK->UseTime		= i_pItemSkill->UseTime;			// 2006-12-05 by dhjin, ��ų ��� �ð�

	// �ڽŰ� ������ �Ʊ׵鿡�Ե� ����
	m_pFieldIOCPSocket->SendInRangeMessageAroundMe(SendBuf, MSG_SIZE(MSG_FC_SKILL_USE_SKILL_OK), TRUE);
	
	///////////////////////////////////////////////////////////////////////////////
	// 2009-12-11 by cmkwon, ������ ��׷η� Ÿ���� �����ϴ� ���� ���� - 
	if(0.0f < i_pItemSkill->ItemInfo->GetSkillDamageForAggro())
	{
		m_pFieldIOCPSocket->GetAttackedMonsterIndex(o_pMonListForSkillAggro);
	}

	return TRUE;
}

// 2009-12-11 by cmkwon, ������ ��׷η� Ÿ���� �����ϴ� ���� ���� - �Ʒ��� �Լ��� �����߰�(, setClientIndex_t *o_pMonListForSkillAggro)
BOOL CFieldSkillManager::UseSkillTypeCharging(ITEM_SKILL *i_pItemSkill, MSG_FC_SKILL_USE_SKILL *i_pMsgUseSkill, setClientIndex_t *o_pMonListForSkillAggro)
{
	// ������ ���ؼ��� Ŭ���̾�Ʈ���� �׻� cancel_skill�� �̸� ������, 20041015, kelovon with jschoi
	BOOL bSkillActivated = IsSkillActivatedByItemNum(i_pItemSkill->ItemNum);
	if (bSkillActivated)
	{
		return FALSE;
	}

	mt_auto_lock mtAuto(&m_vecChargingSkill);
	for(int i=0; i < m_vecChargingSkill.size(); i++)
	{
		if(m_vecChargingSkill[i]->ItemNum == i_pItemSkill->ItemInfo->ItemNum)
		{// �̹� ���� ��¡��ų�� ������̴�

			return FALSE;
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2007-03-29 by cmkwon, MSG_FC_MOVE_OK�� ������ ��Ʈ������ ����
	// 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - ITEM::IsExistDesParam() �Լ��� ������
	//if(IS_EXIST_DES_PARAM(i_pItemSkill->ItemInfo, DES_SKILL_HYPERSHOT))
	if(i_pItemSkill->ItemInfo->IsExistDesParam(DES_SKILL_HYPERSHOT))
	{
		m_bHyperShot		= TRUE;		// 2007-03-29 by cmkwon
	}
	else
	{
		m_bChargingSkill	= TRUE;		// 2007-04-02 by cmkwon
	}
	
	// check: charging �ð��� �������� Ȯ���ϴ� ��ƾ �ʿ��ѵ�... ITEM::AttackTime�� ������
	m_vecChargingSkill.push_back(i_pItemSkill->ItemInfo);
	mtAuto.auto_unlock_cancel();

	INIT_MSG_OF_SIZE(MSG_FC_SKILL_USE_SKILL_OK, T_FC_SKILL_USE_SKILL_OK, msgUseSkillOK, SendBuf);
	msgUseSkillOK->SkillItemID	= i_pMsgUseSkill->SkillItemID;
	msgUseSkillOK->AttackIndex	= i_pMsgUseSkill->AttackIndex;
	msgUseSkillOK->TargetIndex	= i_pMsgUseSkill->TargetIndex;
	msgUseSkillOK->UseTime		= i_pItemSkill->UseTime;			// 2006-12-05 by dhjin, ��ų ��� �ð�

	// �ڽŰ� ������ �Ʊ׵鿡�Ե� ����
	m_pFieldIOCPSocket->SendInRangeMessageAroundMe(SendBuf, MSG_SIZE(MSG_FC_SKILL_USE_SKILL_OK), TRUE);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-12-11 by cmkwon, ������ ��׷η� Ÿ���� �����ϴ� ���� ���� - 
	if(0.0f < i_pItemSkill->ItemInfo->GetSkillDamageForAggro())
	{
		m_pFieldIOCPSocket->GetAttackedMonsterIndex(o_pMonListForSkillAggro);
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldSkillManager::CheckUseSkillTime(ITEM_SKILL* i_pItemSkill, ATUM_DATE_TIME *i_patCurTime)
/// \brief		
/// \author		dhjin
/// \date		2006-12-05 ~ 2006-12-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldSkillManager::CheckUseSkillTime(ITEM_SKILL* i_pItemSkill, ATUM_DATE_TIME *i_patCurTime)
{
// 2009-04-06 by cmkwon, ��ų ���� �Ӽ� �߰��� ����(��� �ð� ����) - �Ʒ��� ���� ��ų�Ӽ����� üũ
//	if (i_pItemSkill->ItemInfo->ReAttacktime >= TERM_TICK_MUST_SERVER_CHECK_SKILL_REATTACK_TIME)  // 10�� = 10 * 60 * 1000
//	{// ���� �ð��� 10���� �̻��̸� üũ
	if(COMPARE_BIT_FLAG(i_pItemSkill->ItemInfo->ItemAttribute, SKILL_ATTR_STORE_USING_TIME))
	{// 2009-04-06 by cmkwon, ��ų ���� �Ӽ� �߰��� ����(��� �ð� ����) - ��ų�Ӽ����� üũ
		int GapTime = i_patCurTime->GetTimeDiffTimeInSeconds(i_pItemSkill->UseTime);
		if (GapTime > (i_pItemSkill->ItemInfo->ReAttacktime/1000))
		{
			return TRUE;
		}
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldSkillManager::SaveUseSkillTime(ITEM_SKILL* i_pItemSkill)
/// \brief		
/// \author		dhjin
/// \date		2006-12-06 ~ 2006-12-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldSkillManager::SaveUseSkillTime(ITEM_SKILL* i_pItemSkill)
{
// 2009-04-06 by cmkwon, ��ų ���� �Ӽ� �߰��� ����(��� �ð� ����) - �Ʒ��� ���� ������.
//	if (i_pItemSkill->ItemInfo->ReAttacktime < TERM_TICK_MUST_SERVER_CHECK_SKILL_REATTACK_TIME)  // 10�� = 10 * 60 * 1000
//	{// 2006-12-20 by cmkwon, 10�� �̸��� �������� ����
	if(FALSE == COMPARE_BIT_FLAG(i_pItemSkill->ItemInfo->ItemAttribute, SKILL_ATTR_STORE_USING_TIME))
	{// 2009-04-06 by cmkwon, ��ų ���� �Ӽ� �߰��� ����(��� �ð� ����) - ��ų ���ð� ���� ���� üũ
		return FALSE;
	}

	QPARAM_UPDATE_SKILLUSETIME *pQParam = new QPARAM_UPDATE_SKILLUSETIME;
	pQParam->ItemUID					= i_pItemSkill->UniqueNumber;
	pQParam->UseTime					= i_pItemSkill->UseTime;
	m_pFieldDBManager->MakeAndEnqueueQuery(QT_UpdateStoreItemSkillUseTime, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
	return TRUE;
}

BOOL CFieldSkillManager::DeactivateSkill(ITEM* i_pItemInfo, BOOL i_bSendMSG)
{
	if (i_pItemInfo == NULL)
	{
		return FALSE;
	}

	if (i_pItemInfo->SkillType != SKILLTYPE_TIMELIMIT && i_pItemInfo->SkillType != SKILLTYPE_TOGGLE)
	{
		return FALSE;
	}

	// unset paramfactor
	m_pFieldIOCPSocket->m_SkillManager.UnsetSkillParamFactor(i_pItemInfo);

	// AtivatedSkill���� ����
	if (!DeleteActivatedSkillByItemNum(i_pItemInfo->ItemNum))
	{
		return FALSE;
	}

	if (i_bSendMSG)
	{
		// send msg
		INIT_MSG_OF_SIZE(MSG_FC_SKILL_INVALIDATE_SKILL, T_FC_SKILL_INVALIDATE_SKILL, msgInvalidateSkill, SendBuf);
		msgInvalidateSkill->SkillItemID.ItemNum = i_pItemInfo->ItemNum;
		msgInvalidateSkill->SkillItemID.ItemUID = 0;	// �� ��쿡�� Ŭ���̾�Ʈ ���ʿ���, �ʿ��ϸ� ����� �Ҵ��ϱ�, 20041018, kelovon
		msgInvalidateSkill->ClientIndex = m_pFieldIOCPSocket->m_character.ClientIndex;
		m_pFieldIOCPSocket->SendInRangeMessageAroundMe(SendBuf, MSG_SIZE(MSG_FC_SKILL_INVALIDATE_SKILL), TRUE);
	}

	// TOGGLE���̸� Ÿ�̸� ����
	if (SKILLTYPE_TOGGLE == i_pItemInfo->SkillType)
	{
		const MEX_TIMER_EVENT *pTimerEvent = m_pFieldIOCPSocket->m_TimerManager.GetTimerEventByItemNum(i_pItemInfo->ItemNum);
		if (pTimerEvent != NULL)
		{
			m_pFieldIOCPSocket->m_TimerManager.StopTimerField(pTimerEvent->TimerUID);
		}
		else
		{
			m_pFieldIOCPSocket->SendErrorMessage(T_FC_SKILL_CANCEL_SKILL, ERR_PROTOCOL_NO_SUCH_SKILL, i_pItemInfo->ItemNum);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CFieldSkillManager::DeactivateSkillByUID(UID64_t i_nSkillUID, BOOL i_bSendMSG)
{
	ASSERT_NOT_IMPLEMENTED_YET();
	return FALSE;
}

BOOL CFieldSkillManager::DeactivateSkillByItemNum(INT i_nSkillItemNum, BOOL i_bSendMSG)
{
	ITEM *pItemInfo = ms_pFieldIOCP2->GetItemInfo(i_nSkillItemNum);

	if (pItemInfo == NULL)
	{
		return FALSE;
	}

	return DeactivateSkill(pItemInfo, i_bSendMSG);
}

BOOL CFieldSkillManager::ApplyChargingSkill(ITEM **o_ppItemforProcessSplashAttack, BOOL i_bSplashAttack, ITEM *i_pAttackItem, CParamFactor *i_pParamFactorForChargingSkill)
{
	BOOL bChargingSkillApplied = FALSE;

	// Chaging Skill ����
	if (!m_vecChargingSkill.empty())
	{
		m_vecChargingSkill.lock();
		mtvectorItemPtr::iterator itrChargingSkill = m_vecChargingSkill.begin();
		while (itrChargingSkill != m_vecChargingSkill.end()) 
		{
			ITEM *pChagingSkillItem = *itrChargingSkill;
			
			if (COMPARE_ITEMKIND(pChagingSkillItem->ReqItemKind, i_pAttackItem->Kind))
			{
				if(FALSE == i_bSplashAttack
					// 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - ITEM::IsExistDesParam() �Լ��� ������
					//&& IS_EXIST_DES_PARAM(pChagingSkillItem, DES_SKILL_HYPERSHOT))
					&& pChagingSkillItem->IsExistDesParam(DES_SKILL_HYPERSHOT))
				{// 2006-12-01 by dhjin, SplashAttack ó���� �ƴϸ鼭 �����ۻ� ��ų�̸� ProcessSplashAttack�� TRUE�� �����Ѵ�
					*o_ppItemforProcessSplashAttack		= pChagingSkillItem;		// 2006-12-01 by dhjin
				}
				else
				{
					// 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - ITEM::IsExistDesParam() �Լ��� ������
					//if(IS_EXIST_DES_PARAM(pChagingSkillItem, DES_SKILL_HYPERSHOT))
					if(pChagingSkillItem->IsExistDesParam(DES_SKILL_HYPERSHOT))
					{
						*o_ppItemforProcessSplashAttack	= pChagingSkillItem;	// 2006-12-01 by dhjin
						m_bHyperShot		= FALSE;		// 2007-03-29 by cmkwon, MSG_FC_MOVE_OK�� ������ ��Ʈ������ ���� ����
					}
					else
					{
						m_bChargingSkill	= FALSE;		// 2007-04-02 by cmkwon, MSG_FC_MOVE_OK�� ������ ��Ʈ������ ���� ����
					}

// 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - �Ʒ��� ���� ����
// 					m_pFieldIOCPSocket->SetParamFactor(i_pParamFactorForChargingSkill, pChagingSkillItem->DestParameter1, pChagingSkillItem->ParameterValue1);
// 					m_pFieldIOCPSocket->SetParamFactor(i_pParamFactorForChargingSkill, pChagingSkillItem->DestParameter2, pChagingSkillItem->ParameterValue2);
// 					m_pFieldIOCPSocket->SetParamFactor(i_pParamFactorForChargingSkill, pChagingSkillItem->DestParameter3, pChagingSkillItem->ParameterValue3);
// 					m_pFieldIOCPSocket->SetParamFactor(i_pParamFactorForChargingSkill, pChagingSkillItem->DestParameter4, pChagingSkillItem->ParameterValue4);
					for(int i=0; i < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; i++)
					{
						m_pFieldIOCPSocket->SetParamFactor(i_pParamFactorForChargingSkill, pChagingSkillItem->ArrDestParameter[i], pChagingSkillItem->ArrParameterValue[i] );
					}

					// Chaging Skill�� ���� �Ӽ� �Ͻ� ����
					m_pFieldIOCPSocket->m_ParamFactor += *i_pParamFactorForChargingSkill;
					bChargingSkillApplied = TRUE;

					itrChargingSkill = m_vecChargingSkill.erase(itrChargingSkill);
					continue;
				}
			}

			itrChargingSkill++;
		}
		m_vecChargingSkill.unlock();
	}

	return bChargingSkillApplied;
}

BOOL CFieldSkillManager::ReleaseChargingSkill(CParamFactor *i_pParamFactorForChargingSkill)
{
	m_pFieldIOCPSocket->m_ParamFactor -= *i_pParamFactorForChargingSkill;
	i_pParamFactorForChargingSkill->Reset();
	return TRUE;
}

// 2006-12-06 by cmkwon, ���� �߰�(ITEM *i_pSkillItem/*=NULL*/), NULL�̸� ��� �ʱ�ȭ, NULL�� �ƴϸ� Ư�� ��ų�� �ʱ�ȭ
BOOL CFieldSkillManager::ClearChargingSkill(ITEM *i_pSkillItem/*=NULL*/)
{
	mt_auto_lock mtA(&m_vecChargingSkill);
	if (m_vecChargingSkill.empty())
	{
		m_bChargingSkill	= FALSE;		// 2007-04-02 by cmkwon
		m_bHyperShot		= FALSE;		// 2007-03-29 by cmkwon
		return FALSE;
	}

	if(NULL == i_pSkillItem)
	{// 2006-12-06 by cmkwon, ��� ��¡ �ʱ�ȭ
		m_bChargingSkill	= FALSE;		// 2007-04-02 by cmkwon
		m_bHyperShot		= FALSE;		// 2007-03-29 by cmkwon
		m_vecChargingSkill.clear();
	}
	else
	{// 2006-12-06 by cmkwon, Ư�� ��¡�� �ʱ�ȭ
		mtvectorItemPtr::iterator itrChargingSkill = m_vecChargingSkill.begin();
		while (itrChargingSkill != m_vecChargingSkill.end()) 
		{
			if(i_pSkillItem->ItemNum == (*itrChargingSkill)->ItemNum)
			{
				///////////////////////////////////////////////////////////////////////////////
				// 2007-03-29 by cmkwon, MSG_FC_MOVE_OK�� ������ ��Ʈ������ ���� ����
				// 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - ITEM::IsExistDesParam() �Լ��� ������
				//if(IS_EXIST_DES_PARAM(i_pSkillItem, DES_SKILL_HYPERSHOT))
				if(i_pSkillItem->IsExistDesParam(DES_SKILL_HYPERSHOT))
				{
					m_bHyperShot		= FALSE;		// 2007-03-29 by cmkwon
				}
				else
				{
					m_bChargingSkill	= FALSE;		// 2007-04-02 by cmkwon
				}

				itrChargingSkill = m_vecChargingSkill.erase(itrChargingSkill);
				continue;
			}
			itrChargingSkill++;
		}
	}
	return TRUE;
}

void CFieldSkillManager::GetSkillTargets(vector<ClientIndex_t> *i_pVectorClientIndex
										 , ClientIndex_t i_nTargetIndex
										 , UID32_t i_nTargetCharUID
										 , ITEM *i_pItemInfo)
{
	switch(i_pItemInfo->SkillTargetType)
	{
	case SKILLTARGETTYPE_ME:
		{
			i_pVectorClientIndex->push_back(m_pFieldIOCPSocket->m_character.ClientIndex);
		}
		break;
	case SKILLTARGETTYPE_ONE:
		{
			if (IS_VALID_CLIENT_INDEX(i_nTargetIndex))
			{
				i_pVectorClientIndex->push_back(i_nTargetIndex);
			}
			else if(0 != i_nTargetCharUID)
			{
				CFieldIOCPSocket *pFSock = m_pFieldIOCPSocket->ms_pFieldIOCP->GetFieldIOCPSocketByCharacterUniqueNumber(i_nTargetCharUID);
				if(pFSock
					&& pFSock->IsValidCharacter())
				{
					i_pVectorClientIndex->push_back(pFSock->GetCharacter()->ClientIndex);
				}
			}
		}
		break;
	case SKILLTARGETTYPE_ONE_EXCLUDE_ME:
		{
			if (IS_VALID_CLIENT_INDEX(i_nTargetIndex)
				&& m_pFieldIOCPSocket->GetCharacter()->ClientIndex != i_nTargetIndex)
			{
				i_pVectorClientIndex->push_back(i_nTargetIndex);
			}
			else if(0 != i_nTargetCharUID
				&& m_pFieldIOCPSocket->GetCharacter()->CharacterUniqueNumber != i_nTargetCharUID)
			{
				CFieldIOCPSocket *pFSock = m_pFieldIOCPSocket->ms_pFieldIOCP->GetFieldIOCPSocketByCharacterUniqueNumber(i_nTargetCharUID);
				if(pFSock
					&& pFSock->IsValidCharacter())
				{
					i_pVectorClientIndex->push_back(pFSock->GetCharacter()->ClientIndex);
				}
			}
		}
		break;
	case SKILLTARGETTYPE_PARTY_WITH_ME:
		{
			CFieldParty *pFParty = m_pFieldIOCPSocket->m_pFieldParty;			// 2006-07-04 by cmkwon
			if (pFParty != NULL)
			{
				i_pVectorClientIndex->push_back(m_pFieldIOCPSocket->m_character.ClientIndex);
				pFParty->GetMembersInSameMapToVector(i_pVectorClientIndex,
								m_pFieldIOCPSocket->m_pCurrentFieldMapChannel,
								&m_pFieldIOCPSocket->m_character.PositionVector,
								i_pItemInfo->Range, m_pFieldIOCPSocket->m_character.ClientIndex);
			}
		}
		break;
	case SKILLTARGETTYPE_PARTY_ONE:	// 21	// �ƹ��� �ϳ��� ��Ƽ��, // 2005-12-05 by cmkwon
		{
			if (m_pFieldIOCPSocket->m_pFieldParty != NULL)
			{
				if (IS_VALID_CLIENT_INDEX(i_nTargetIndex))
				{
					CFieldIOCPSocket *pFSock = m_pFieldIOCPSocket->ms_pFieldIOCP->GetFieldIOCPSocket(i_nTargetIndex);
					if(pFSock
						&& pFSock->IsValidCharacter()
						&& pFSock->m_pFieldParty == m_pFieldIOCPSocket->m_pFieldParty)
					{
						i_pVectorClientIndex->push_back(i_nTargetIndex);
					}
				}
				else if(0 != i_nTargetCharUID)
				{
					CFieldIOCPSocket *pFSock = m_pFieldIOCPSocket->ms_pFieldIOCP->GetFieldIOCPSocketByCharacterUniqueNumber(i_nTargetCharUID);
					if(pFSock
						&& pFSock->IsValidCharacter()
						&& pFSock->m_pFieldParty == m_pFieldIOCPSocket->m_pFieldParty)
					{
						i_pVectorClientIndex->push_back(pFSock->GetCharacter()->ClientIndex);
					}
				}
			}
		}
		break;
	case SKILLTARGETTYPE_PARTY_ONE_EXCLUDE_ME:	// 22	// �ڽ��� ������ �ϳ��� ��Ƽ��, // 2005-12-05 by cmkwon
		{
			if (m_pFieldIOCPSocket->m_pFieldParty != NULL)
			{
				if (IS_VALID_CLIENT_INDEX(i_nTargetIndex)
					&& m_pFieldIOCPSocket->GetCharacter()->ClientIndex != i_nTargetIndex)
				{
					CFieldIOCPSocket *pFSock = m_pFieldIOCPSocket->ms_pFieldIOCP->GetFieldIOCPSocket(i_nTargetIndex);
					if(pFSock
						&& pFSock->IsValidCharacter()
						&& pFSock->m_pFieldParty == m_pFieldIOCPSocket->m_pFieldParty)
					{
						i_pVectorClientIndex->push_back(i_nTargetIndex);
					}
				}
				else if(0 != i_nTargetCharUID
					&& m_pFieldIOCPSocket->GetCharacter()->CharacterUniqueNumber != i_nTargetCharUID)
				{
					CFieldIOCPSocket *pFSock = m_pFieldIOCPSocket->ms_pFieldIOCP->GetFieldIOCPSocketByCharacterUniqueNumber(i_nTargetCharUID);
					if(pFSock
						&& pFSock->IsValidCharacter()
						&& pFSock->m_pFieldParty == m_pFieldIOCPSocket->m_pFieldParty)
					{
						i_pVectorClientIndex->push_back(pFSock->GetCharacter()->ClientIndex);
					}
				}
			}
		}
		break;
	case SKILLTARGETTYPE_INRANGE_WITH_ME:
		{
			i_pVectorClientIndex->push_back(m_pFieldIOCPSocket->m_character.ClientIndex);
			m_pFieldIOCPSocket->m_pCurrentFieldMapChannel->GetAdjacentCharacterIndexes(
								&m_pFieldIOCPSocket->m_character.PositionVector,
								i_pItemInfo->Range,
								i_pVectorClientIndex, m_pFieldIOCPSocket->m_character.ClientIndex);

		}
		break;
	case SKILLTARGETTYPE_PARTY_WITHOUT_ME:
		{
			CFieldParty *pFParty = m_pFieldIOCPSocket->m_pFieldParty;			// 2006-07-04 by cmkwon
			if (pFParty != NULL)
			{
				pFParty->GetMembersInSameMapToVector(i_pVectorClientIndex,
								m_pFieldIOCPSocket->m_pCurrentFieldMapChannel,
								&m_pFieldIOCPSocket->m_character.PositionVector,
								i_pItemInfo->Range, m_pFieldIOCPSocket->m_character.ClientIndex);
			}
		}
		break;
	case SKILLTARGETTYPE_INRANGE_WITHOUT_ME:
		{
			m_pFieldIOCPSocket->m_pCurrentFieldMapChannel->GetAdjacentCharacterIndexes(
								&m_pFieldIOCPSocket->m_character.PositionVector,
								i_pItemInfo->Range,
								i_pVectorClientIndex, m_pFieldIOCPSocket->m_character.ClientIndex);
		}
		break;
	case SKILLTARGETTYPE_ONE_EXCEPT_OURS:		// 2006-11-23 by cmkwon, �ϴ� �������� üũ�� ���� �ʴ´�, Ŭ���̾�Ʈ���� üũ - ��뼼��, PvP ���, ��Ƽ�� ���, ������ ���
		{
			CFieldIOCPSocket *pFISock = m_pFieldIOCPSocket->ms_pFieldIOCP->GetFieldIOCPSocket(i_nTargetIndex);
			if(pFISock
				&& pFISock->IsValidCharacter())
			{
				i_pVectorClientIndex->push_back(i_nTargetIndex);
			}
		}
		break;
	case SKILLTARGETTYPE_ALL_ENEMY:			// 2006-11-30 by dhjin, �ڽ��� �߽����� �� ����
		{
			i_pVectorClientIndex->push_back(m_pFieldIOCPSocket->m_character.ClientIndex);	// 2007-02-07 by dhjin, ��ų ��� ���� Ȯ�� �� ����
		}
		break;
	case SKILLTARGETTYPE_ALL_OURS:			// 2007-02-06 by dhjin, �ڽ��� �߽����� �� �Ʊ���
		{
			m_pFieldIOCPSocket->m_pCurrentFieldMapChannel->GetExactCharacterIndexes(i_pVectorClientIndex, &m_pFieldIOCPSocket->m_character.PositionVector, i_pItemInfo->Range);
			vectClientIndex_t::iterator itr = i_pVectorClientIndex->begin();
			while(itr != i_pVectorClientIndex->end())
			{
				CFieldIOCPSocket	*pTargetCSock = m_pFieldIOCPSocket->ms_pFieldIOCP->GetFieldIOCPSocket(*itr);
				if ( NULL == pTargetCSock
					|| FALSE == pTargetCSock->IsValidCharacter()
					|| COMPARE_INFLUENCE(pTargetCSock->GetCharacter()->InfluenceType, INFLUENCE_TYPE_NORMAL)
					|| FALSE == CFieldIOCPSocket::IsValidCharacterFriend(m_pFieldIOCPSocket, pTargetCSock) )
				{// 2007-03-07 by cmkwon, �Ϲ� ������ ����, �Ʊ��� �ƴϸ� ����
					itr = i_pVectorClientIndex->erase(itr);
					continue;
				}
				itr++;
			}
		}
		break;
	default:
		{
		}
	}
}

// 2015-11-03 DevX this function defines which skills are enabled in free war maps
void CFieldSkillManager::GetSkillTargetsFreeWar(vector<ClientIndex_t> *i_pVectorClientIndex,
												ClientIndex_t i_nTargetIndex, UID32_t i_nTargetCharUID, ITEM *i_pItemInfo)
{
	switch (i_pItemInfo->SkillTargetType)
	{

	case SKILLTARGETTYPE_ME:
	case SKILLTARGETTYPE_PARTY_WITH_ME:
	case SKILLTARGETTYPE_PARTY_ONE:
	case SKILLTARGETTYPE_ALL_ENEMY:

		i_pVectorClientIndex->push_back(m_pFieldIOCPSocket->m_character.ClientIndex);

		break;


	case SKILLTARGETTYPE_ONE_EXCLUDE_ME:

		if (m_pFieldIOCPSocket->GetCharacter()->CharacterUniqueNumber == i_nTargetCharUID) break;

		// 2015-11-03 DevX Beware, intentionally omitted break; execution continues to next label

	case SKILLTARGETTYPE_ONE:

		if (IS_VALID_CLIENT_INDEX(i_nTargetIndex)) i_pVectorClientIndex->push_back(i_nTargetIndex);

		else if (i_nTargetCharUID != 0)
		{
			CFieldIOCPSocket *pFSock = m_pFieldIOCPSocket->ms_pFieldIOCP->GetFieldIOCPSocketByCharacterUniqueNumber(i_nTargetCharUID);
			if (pFSock && pFSock->IsValidCharacter()) i_pVectorClientIndex->push_back(pFSock->GetCharacter()->ClientIndex);
		}

		break;


	case SKILLTARGETTYPE_INRANGE_WITH_ME:

		i_pVectorClientIndex->push_back(m_pFieldIOCPSocket->m_character.ClientIndex);

		// 2015-11-03 DevX Beware, intentionally omitted break; execution continues to next label

	case SKILLTARGETTYPE_INRANGE_WITHOUT_ME:

		m_pFieldIOCPSocket->m_pCurrentFieldMapChannel->
			GetAdjacentCharacterIndexes(&m_pFieldIOCPSocket->m_character.PositionVector, i_pItemInfo->Range, i_pVectorClientIndex, m_pFieldIOCPSocket->m_character.ClientIndex);

		break;


	case SKILLTARGETTYPE_ONE_EXCEPT_OURS:
	{
		auto pFISock = m_pFieldIOCPSocket->ms_pFieldIOCP->GetFieldIOCPSocket(i_nTargetIndex);

		if (pFISock && pFISock->IsValidCharacter()) i_pVectorClientIndex->push_back(i_nTargetIndex);

		break;
	}

	case SKILLTARGETTYPE_ALL_OURS:			// 2007-02-06 by dhjin, �ڽ��� �߽����� �� �Ʊ���
	{
		m_pFieldIOCPSocket->m_pCurrentFieldMapChannel->GetExactCharacterIndexes(i_pVectorClientIndex, &m_pFieldIOCPSocket->m_character.PositionVector, i_pItemInfo->Range);
		vectClientIndex_t::iterator itr = i_pVectorClientIndex->begin();
		while (itr != i_pVectorClientIndex->end())
		{
			CFieldIOCPSocket	*pTargetCSock = m_pFieldIOCPSocket->ms_pFieldIOCP->GetFieldIOCPSocket(*itr);
			if (NULL == pTargetCSock
				|| FALSE == pTargetCSock->IsValidCharacter())
			{// 2007-03-07 by cmkwon, �Ϲ� ������ ����, �Ʊ��� �ƴϸ� ����
				itr = i_pVectorClientIndex->erase(itr);
				continue;
			}
			itr++;
		}
	}
	break;
	default: break;

	}
}

/******************************************************************************
**
**	��ų�� �� �Ķ���� ����.
**
**	Create Info : ??. ??. ??.
**
**	Update Info :	// 2006-07-21 by cmkwon, ���� �߰��� - ClientIndex_t i_SkillUseClientIdx
**
**					// 2010. 06. 08 by hsLee. ���� �߰� - i_fIncreasePowerRatio
**
*******************************************************************************/
BOOL CFieldSkillManager::SetSkillParamFactor(ITEM *i_pItemInfo, ClientIndex_t i_SkillUseClientIdx/*=0*/ , float i_fIncreasePowerRatio /*= 1.0f*/ )
{
// 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - �Ʒ��� ���� ����
// 	m_pFieldIOCPSocket->SetParamFactor(i_pItemInfo->DestParameter1, i_pItemInfo->ParameterValue1, i_SkillUseClientIdx);
// 	m_pFieldIOCPSocket->SetParamFactor(i_pItemInfo->DestParameter2, i_pItemInfo->ParameterValue2, i_SkillUseClientIdx);
// 	m_pFieldIOCPSocket->SetParamFactor(i_pItemInfo->DestParameter3, i_pItemInfo->ParameterValue3, i_SkillUseClientIdx);
// 	m_pFieldIOCPSocket->SetParamFactor(i_pItemInfo->DestParameter4, i_pItemInfo->ParameterValue4, i_SkillUseClientIdx);
	for(int i=0; i < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; i++)
	{
		// 2010. 06. 08 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (�Ʊ� ���� �뷱�� ����.) - ��ų�� �뷱�� ����.
		//m_pFieldIOCPSocket->SetParamFactor(i_pItemInfo->ArrDestParameter[i], i_pItemInfo->ArrParameterValue[i], i_SkillUseClientIdx);
#ifdef NEMERIAN_NATION_BALANCE_BUFF_PENALITY
		if (i_pItemInfo->ArrDestParameter[i] == DES_DEFENSE_01
			|| i_pItemInfo->ArrDestParameter[i] == DES_DEFENSE_02
			|| i_pItemInfo->ArrDestParameter[i] == DES_DEFENSEPROBABILITY_01
			|| i_pItemInfo->ArrDestParameter[i] == DES_DEFENSEPROBABILITY_02
			|| i_pItemInfo->ArrDestParameter[i] == DES_MINATTACK_01
			|| i_pItemInfo->ArrDestParameter[i] == DES_MAXATTACK_01
			|| i_pItemInfo->ArrDestParameter[i] == DES_MINATTACK_02
			|| i_pItemInfo->ArrDestParameter[i] == DES_MAXATTACK_02
			|| i_pItemInfo->ArrDestParameter[i] == DES_ATTACKPROBABILITY_01
			|| i_pItemInfo->ArrDestParameter[i] == DES_ATTACKPROBABILITY_02) {
			m_pFieldIOCPSocket->SetParamFactor(i_pItemInfo->ArrDestParameter[i], (float)(i_pItemInfo->ArrParameterValue[i] * i_fIncreasePowerRatio), i_SkillUseClientIdx);
		}
		else {
			m_pFieldIOCPSocket->SetParamFactor(i_pItemInfo->ArrDestParameter[i], (float)(i_pItemInfo->ArrParameterValue[i]), i_SkillUseClientIdx);
		}
#else
		m_pFieldIOCPSocket->SetParamFactor(i_pItemInfo->ArrDestParameter[i], (float)(i_pItemInfo->ArrParameterValue[i] * i_fIncreasePowerRatio), i_SkillUseClientIdx);
#endif
	}

	return TRUE;
}

BOOL CFieldSkillManager::UnsetSkillParamFactor(ITEM *i_pItemInfo)
{
// 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - �Ʒ��� ���� ����
// 	m_pFieldIOCPSocket->UnsetParamFactor(i_pItemInfo->DestParameter1, i_pItemInfo->ParameterValue1);
// 	m_pFieldIOCPSocket->UnsetParamFactor(i_pItemInfo->DestParameter2, i_pItemInfo->ParameterValue2);
// 	m_pFieldIOCPSocket->UnsetParamFactor(i_pItemInfo->DestParameter3, i_pItemInfo->ParameterValue3);
// 	m_pFieldIOCPSocket->UnsetParamFactor(i_pItemInfo->DestParameter4, i_pItemInfo->ParameterValue4);
	// 2013-05-07 by jhseol,bckim ���� �г�Ƽ
	float tmBuffPenalty = 1.0f;
#ifdef SC_BUFF_PENALTY_JHSEOL_BCKIM
	if ( TRUE == i_pItemInfo->IsExistDesParam(DES_BUFF_PENALTY_RATIO) )					// ���� �г�Ƽ DES�� ������
	{
		USHORT tmApplirdUnitKind = (USHORT)i_pItemInfo->GetParameterValue(DES_BUFF_PENALTY_APPLIED_UNIT_KIND);
		if ( 0 != (m_pFieldIOCPSocket->m_character.UnitKind & tmApplirdUnitKind) )		// ������ �� ���Ͽ�
		{
			tmBuffPenalty = i_pItemInfo->GetParameterValue(DES_BUFF_PENALTY_RATIO);		// �г�Ƽ�� ���� ��Ų��.
		}
	}
#endif
#ifdef NEMERIAN_NATION_BALANCE_BUFF_PENALITY
	if (i_pItemInfo->SkillType == SKILLTYPE_TIMELIMIT) {
		int diff = g_diffVictoryWarByroo;
		if ((diff < 0 && TRUE == COMPARE_INFLUENCE(m_pFieldIOCPSocket->m_character.InfluenceType, INFLUENCE_TYPE_ANI)) ||
			(diff > 0 && TRUE == COMPARE_INFLUENCE(m_pFieldIOCPSocket->m_character.InfluenceType, INFLUENCE_TYPE_VCN))) {
			int levelPenality = abs(diff) / 22;
			tmBuffPenalty = 1.0 - (0.05f * levelPenality);
		}
	}
#endif
	// end 2013-05-07 by jhseol,bckim ���� �г�Ƽ
	for(int i=0; i < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; i++)
	{
#ifdef NEMERIAN_NATION_BALANCE_BUFF_PENALITY
		if (i_pItemInfo->ArrDestParameter[i] == DES_DEFENSE_01
			|| i_pItemInfo->ArrDestParameter[i] == DES_DEFENSE_02
			|| i_pItemInfo->ArrDestParameter[i] == DES_DEFENSEPROBABILITY_01
			|| i_pItemInfo->ArrDestParameter[i] == DES_DEFENSEPROBABILITY_02
			|| i_pItemInfo->ArrDestParameter[i] == DES_MINATTACK_01
			|| i_pItemInfo->ArrDestParameter[i] == DES_MAXATTACK_01
			|| i_pItemInfo->ArrDestParameter[i] == DES_MINATTACK_02
			|| i_pItemInfo->ArrDestParameter[i] == DES_MAXATTACK_02
			|| i_pItemInfo->ArrDestParameter[i] == DES_ATTACKPROBABILITY_01
			|| i_pItemInfo->ArrDestParameter[i] == DES_ATTACKPROBABILITY_02) {
			m_pFieldIOCPSocket->UnsetParamFactor(i_pItemInfo->ArrDestParameter[i], i_pItemInfo->ArrParameterValue[i] * tmBuffPenalty);	// 2013-05-07 by jhseol,bckim ���� �г�Ƽ
		}
		else {
			m_pFieldIOCPSocket->UnsetParamFactor(i_pItemInfo->ArrDestParameter[i], i_pItemInfo->ArrParameterValue[i]);	// 2013-05-07 by jhseol,bckim ���� �г�Ƽ
		}
#else
		m_pFieldIOCPSocket->UnsetParamFactor(i_pItemInfo->ArrDestParameter[i], i_pItemInfo->ArrParameterValue[i] * tmBuffPenalty);	// 2013-05-07 by jhseol,bckim ���� �г�Ƽ
#endif
	}

	return TRUE;
}

BOOL CFieldSkillManager::CheckSkillReqItemKind(ITEM *i_pItemInfo)
{
	for (int i = 0; i < SIZE_MAX_POS; i++)
	{
		if (COMPARE_ITEMKIND(i_pItemInfo->ReqItemKind, m_pFieldIOCPSocket->m_arrAttachItems[i].Kind))
		{
			return TRUE;
		}
	}

	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldSkillManager::GetPrepareUseBigBoom(void)
/// \brief		// 2007-07-11 by cmkwon, BigBoom�� PrepareUse ���� ����
/// \author		cmkwon
/// \date		2007-07-11 ~ 2007-07-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldSkillManager::GetPrepareUseBigBoom(void)
{
	return m_bPrepareUseBigBoom;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldSkillManager::SetPrepareUseBigBoom(BOOL i_bPrepareUseBigBoom)
/// \brief		// 2007-07-11 by cmkwon, BigBoom�� PrepareUse ���� ����
/// \author		cmkwon
/// \date		2007-07-11 ~ 2007-07-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldSkillManager::SetPrepareUseBigBoom(BOOL i_bPrepareUseBigBoom)
{
	m_bPrepareUseBigBoom	= i_bPrepareUseBigBoom;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-04-06 by cmkwon, �ݿ��� ����� ��ų �ý��� ���� - 
/// \author		cmkwon
/// \date		2009-04-06 ~ 2009-04-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CFieldSkillManager::AddSkillConfirmUse(UID32_t i_targetCharcUID, int i_nUsingSkillNum, DWORD i_dwAvailableSec, MAP_CHANNEL_INDEX i_mapChannIdx, D3DXVECTOR3 i_posVec3)
{
	mt_auto_lock maA(&m_mtvectSkillConfirmUseList);

	this->CheckExpireAvaibleTick();

	SSKILL_CONFIRM_USE tmSkillConUse;
	tmSkillConUse.SkillConfirmUseUID	= m_nSkillConfirmUseUID++;
	tmSkillConUse.ExpireTick			= timeGetTime() + i_dwAvailableSec * 1000;
	tmSkillConUse.TargetCharacterUID	= i_targetCharcUID;
	tmSkillConUse.UsingSkillItemNum		= i_nUsingSkillNum;
	tmSkillConUse.MapChannelIndex		= i_mapChannIdx;
	tmSkillConUse.PositionVec3			= i_posVec3;

	m_mtvectSkillConfirmUseList.push_back(tmSkillConUse);

	return tmSkillConUse.SkillConfirmUseUID;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-04-06 by cmkwon, �ݿ��� ����� ��ų �ý��� ���� - 
/// \author		cmkwon
/// \date		2009-04-06 ~ 2009-04-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldSkillManager::PopSkillConfirmUse(INT i_skillConfirmUseUID, UID32_t i_targetCharcUID, int i_nUsingSkillNum, SSKILL_CONFIRM_USE *o_pSkillConfirmUse)
{
	mt_auto_lock maA(&m_mtvectSkillConfirmUseList);

	mtvectSSKILL_CONFIRM_USE::iterator itr(m_mtvectSkillConfirmUseList.begin());
	for(; itr != m_mtvectSkillConfirmUseList.end(); itr++)
	{
		if(itr->SkillConfirmUseUID == i_skillConfirmUseUID
			&& itr->TargetCharacterUID == i_targetCharcUID
			&& itr->UsingSkillItemNum == i_nUsingSkillNum)
		{
			*o_pSkillConfirmUse		= *itr;
			m_mtvectSkillConfirmUseList.erase(itr);
			return TRUE;
		}
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-04-06 by cmkwon, �ݿ��� ����� ��ų �ý��� ���� - 
/// \author		cmkwon
/// \date		2009-04-06 ~ 2009-04-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldSkillManager::CheckExpireAvaibleTick(void)
{
	mt_auto_lock maA(&m_mtvectSkillConfirmUseList);
	DWORD dwCurTick = timeGetTime();

	mtvectSSKILL_CONFIRM_USE::iterator itr(m_mtvectSkillConfirmUseList.begin());
	while(itr != m_mtvectSkillConfirmUseList.end())
	{
		if(itr->ExpireTick < dwCurTick)
		{
			itr = m_mtvectSkillConfirmUseList.erase(itr);
			continue;
		}
		itr++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-08-03 by cmkwon, EP3-4 ��� ���� ��ų ���� - 
///					CFieldIOCPSocket::Process_FC_SKILL_CANCEL_SKILL()���� ó���ϴ� ���� �Լ��� ����
/// \author		cmkwon
/// \date		2009-08-05 ~ 2009-08-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
Err_t CFieldSkillManager::CancelSkill(ITEM *i_pSkillInfo, MSG_FC_SKILL_CANCEL_SKILL *i_pCancelSkill)
{
	// CANCEL_SKILL�� �ð���(SKILLTYPE_TIMELIMIT), �����(SKILLTYPE_TOGGLE) ��ų���� ����� �� �ִ�.
	switch(i_pSkillInfo->SkillType)
	{
	case SKILLTYPE_TIMELIMIT:
		{
			if (this->IsSkillActivatedByItemNum(i_pCancelSkill->SkillItemID.ItemNum))
			{
				if (!this->DeactivateSkill(i_pSkillInfo, TRUE))
				{					
					return ERR_PROTOCOL_NO_SUCH_SKILL;
				}
			}
		}
		break;
	case SKILLTYPE_TOGGLE:
		{
			if (this->IsSkillActivatedByItemNum(i_pCancelSkill->SkillItemID.ItemNum))
			{
				if (!this->DeactivateSkill(i_pSkillInfo, TRUE))
				{
					return ERR_PROTOCOL_NO_SUCH_SKILL;
				}
			}
		}
		break;
	case SKILLTYPE_CLICK:
		{
			// 2013-03-12 by jhseol, ��ų �ѹ��� �ý��� ����
#ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
			if (i_pSkillInfo->Kind == SKILL_KIND_CONTROL)
#else	// #ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
			if (SKILL_KIND(i_pSkillInfo->ItemNum) == SKILL_KIND_CONTROL)
#endif	// #ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
			// end 2013-03-12 by jhseol, ��ų �ѹ��� �ý��� ����
			{
				// �ڽ��� ������ ���� ���ֿ��� �ѷ��ֱ⸸ �Ѵ�.
				INIT_MSG_OF_SIZE(MSG_FC_SKILL_INVALIDATE_SKILL, T_FC_SKILL_INVALIDATE_SKILL, msgInvalidateSkill, msgInvalidateSkillBuf);
				msgInvalidateSkill->SkillItemID.ItemNum = i_pSkillInfo->ItemNum;
				msgInvalidateSkill->SkillItemID.ItemUID = 0;	// Ŭ���̾�Ʈ ���ʿ���
				msgInvalidateSkill->ClientIndex = m_pCharacter->ClientIndex;
				m_pFieldIOCPSocket->SendInRangeMessageAroundMe(msgInvalidateSkillBuf, MSG_SIZE(MSG_FC_SKILL_INVALIDATE_SKILL), TRUE);
			}
		}
		break;
	case SKILLTYPE_CHARGING:
		{// 2006-11-30 by dhjin, ��¡ ��ҵ� Ŭ���̾�Ʈ�� �����Ѵ�
			this->ClearChargingSkill(i_pSkillInfo);			// 2006-12-06 by cmkwon, ��ҵ� ��¡��ų�� �ʱ�ȭ �Ѵ�.
		}
		break;
	default:
		{
			return ERR_PROTOCOL_INVALID_SKILLTYPE;
		}
	}

	return ERR_NO_ERROR;
}


BOOL CFieldSkillManager::DeleteActivatedSkillByItemNum(INT i_nSkillItemNum)
{
	mt_auto_lock alock(&m_vectorAtivatedSkill);

	mtvectorItemID::iterator itr = m_vectorAtivatedSkill.begin();
	while (itr != m_vectorAtivatedSkill.end())
	{
		if (i_nSkillItemNum == itr->ItemNum)
		{
			m_vectorAtivatedSkill.erase(itr);
			return TRUE;
		}

		itr++;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldSkillManager::ApplyMonSkill(ITEM *i_pSkill, MSG_FN_BATTLE_ATTACK_SKILL * i_pMsg)
/// \brief		���Ǵ�Ƽ - ���� ���� ��ų�� �ִٸ� ����
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldSkillManager::ApplyMonSkill(ITEM *i_pSkill, MSG_FN_BATTLE_ATTACK_SKILL * i_pMsg) 
{
	if ( TRUE == IS_ORBIT_SKILL ( i_pSkill->OrbitType ) ) 
	{
		// OrbitSkill ����
		this->ApplyOrbitSkill ( i_pSkill , i_pMsg->ClientIndex );
		return;		// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -
	}

	// 2010. 06. 08 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (�Ʊ� ���� �뷱�� ����.) - ��ų �뷱�� ����.
	//this->SetSkillParamFactor(i_pSkill, i_pMsg->ClientIndex);		// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -
	this->SetSkillParamFactor ( i_pSkill , i_pMsg->ClientIndex , i_pMsg->fIncreasePowerRatio );		// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldSkillManager::ApplyOrbitSkill(ITEM *i_pSkill, ClientIndex_t i_ClientIdx)
/// \brief		���Ǵ�Ƽ - OrbitSkill ����
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2010. 06. 08 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (�Ʊ� ���� �뷱�� ����.) - (i_fIncreasePowerRatio) ���� �߰�.
void CFieldSkillManager::ApplyOrbitSkill( ITEM *i_pSkill , ClientIndex_t i_ClientIdx , float i_fIncreasePowerRatio /*= 1.0f*/ ) 
{
	switch(i_pSkill->OrbitType) 
	{
		case ORBIT_SKILL_DEBUFF_TIMELIMIT: 
			{
				this->ApplyDebuffSkill(i_pSkill , i_fIncreasePowerRatio );
			}
			break;

		case ORBIT_SKILL_DOT_TIMELIMIT: 
			{
				this->ApplyDotSkill(i_pSkill, i_ClientIdx , i_fIncreasePowerRatio );
			}
			break;
	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldSkillManager::ApplyDebuffSkill(ITEM *i_pSkill)
/// \brief		���Ǵ�Ƽ - Debuff ����
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2010. 06. 08 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (�Ʊ� ���� �뷱�� ����.) - (i_fIncreasePowerRatio) ���� �߰�.
void CFieldSkillManager::ApplyDebuffSkill(ITEM *i_pSkill , float i_fIncreasePowerRatio /*= 1.0f*/ ) {
	for(int i=0; i < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; i++) {
		switch(i_pSkill->ArrDestParameter[i]) {
			case DES_SKILL_NO_WARNING: {
				m_FieldDebuff.SetDebuff(i_pSkill, i , i_fIncreasePowerRatio );
			}break;
			case DES_SKILL_SLOWMOVING: {
				m_FieldDebuff.SetDebuff(i_pSkill, i , i_fIncreasePowerRatio );
			}break;
			case DES_SKILL_SHIELD_PARALYZE: {
				m_FieldDebuff.SetDebuff(i_pSkill, i , i_fIncreasePowerRatio );
			}break;
			case DES_SKILL_MON_SILENCE_PRIMARY: {
				m_FieldDebuff.SetDebuff(i_pSkill, i , i_fIncreasePowerRatio );
			}break;
			case DES_SKILL_MON_SILENCE_SECOND: {
				m_FieldDebuff.SetDebuff(i_pSkill, i , i_fIncreasePowerRatio );
			}break;
			case DES_SKILL_MON_FREEZE_HP: {
				m_FieldDebuff.SetDebuff(i_pSkill, i , i_fIncreasePowerRatio );
			}break;
			case DES_SKILL_MON_FREEZE_DP: {
				m_FieldDebuff.SetDebuff(i_pSkill, i , i_fIncreasePowerRatio );
			}break;
			case DES_SKILL_MON_FREEZE_SP: {
				m_FieldDebuff.SetDebuff(i_pSkill, i , i_fIncreasePowerRatio );
			}break;
			case DES_SKILL_MON_HOLD: {
				m_FieldDebuff.SetDebuff(i_pSkill, i , i_fIncreasePowerRatio );
			}break;
			case DES_SKILL_MON_SILENCE_SKILL: {
				m_FieldDebuff.SetDebuff(i_pSkill, i , i_fIncreasePowerRatio );
			}break;		
			// start 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ��ų �߰�
			case DES_SKILL_STOPMAGIC: {
				m_FieldDebuff.SetDebuff(i_pSkill, i , i_fIncreasePowerRatio );
			}break;
			// end 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ��ų �߰�
		}	
	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldSkillManager::ApplyDotSkill(ITEM *i_pSkill, ClientIndex_t i_ClientIdx)
/// \brief		���Ǵ�Ƽ - Dot ����
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2010. 06. 08 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (�Ʊ� ���� �뷱�� ����.) - (i_fIncreasePowerRatio) ���� �߰�.
void CFieldSkillManager::ApplyDotSkill(ITEM *i_pSkill, ClientIndex_t i_ClientIdx , float i_fIncreasePowerRatio /*= 1.0f*/ ) 
{
	for(int i=0; i < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; i++) 
	{
		switch(i_pSkill->ArrDestParameter[i]) 
		{
			case DES_SKILL_MON_STEALING: 
				{
					UID32_t TimerUID = m_pFieldIOCPSocket->m_TimerManager.m_TimerUIDGenerator.GetNext();
					m_pFieldIOCPSocket->m_TimerManager.StartTimerFieldNotCreateTimerUID(TE_TYPE_DOTSKILL_STEALING, TIMER_DOT_INTERVAL, TimerUID, TimerUID);

					// 2010. 06. 08 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (�Ʊ� ���� �뷱�� ����.) - ��ų �뷱�� ����.
					//m_FieldDot.SetDot(i_pSkill, i, i_ClientIdx, TimerUID );
					m_FieldDot.SetDot( i_pSkill , i , i_ClientIdx , TimerUID , i_fIncreasePowerRatio );
				}
				break;

			case DES_SKILL_MON_DRAIN: 
				{
					UID32_t TimerUID = m_pFieldIOCPSocket->m_TimerManager.m_TimerUIDGenerator.GetNext();
					m_pFieldIOCPSocket->m_TimerManager.StartTimerFieldNotCreateTimerUID(TE_TYPE_DOTSKILL_DRAIN, TIMER_DOT_INTERVAL, TimerUID, TimerUID);

					// 2010. 06. 08 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (�Ʊ� ���� �뷱�� ����.) - ��ų �뷱�� ����.
					//m_FieldDot.SetDot(i_pSkill, i, i_ClientIdx, TimerUID);
					m_FieldDot.SetDot( i_pSkill , i , i_ClientIdx , TimerUID , i_fIncreasePowerRatio );
				}
				break;

			case DES_TIME_BOMB: 
				{	// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -
					UID32_t TimerUID = m_pFieldIOCPSocket->m_TimerManager.m_TimerUIDGenerator.GetNext();
					m_pFieldIOCPSocket->m_TimerManager.StartTimerFieldNotCreateTimerUID(TE_TYPE_DOTSKILL_TIME_BOMB, TIMER_DOT_INTERVAL, TimerUID, TimerUID);
					
					// 2010. 06. 08 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (�Ʊ� ���� �뷱�� ����.) - ��ų �뷱�� ����.
					//m_FieldDot.SetDot(i_pSkill, i, i_ClientIdx, TimerUID);
					m_FieldDot.SetDot( i_pSkill , i , i_ClientIdx , TimerUID , i_fIncreasePowerRatio );
				}
				break;
		}
	}	
}

void CFieldSkillManager::ResetDebuffSkill() {
	this->m_FieldDebuff.InitFieldDebuff();
}

void CFieldSkillManager::ResetDotSkill() {
	this->m_FieldDot.InitFieldDot();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldSkillManager::ReleaseOrbitSkill(ITEM *i_pSkill, BOOL i_bTimeCheck/* = TRUE*/)
/// \brief		���Ǵ�Ƽ - OrbitSkill ����
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldSkillManager::ReleaseOrbitSkill(ITEM *i_pSkill, BOOL i_bTimeCheck/* = TRUE*/) {
	switch(i_pSkill->OrbitType) {
		case ORBIT_SKILL_DEBUFF_TIMELIMIT: {
			this->ReleaseDebuffSkill(i_pSkill, i_bTimeCheck);
		}break;
		case ORBIT_SKILL_DOT_TIMELIMIT: {
			this->ReleaseDotSkill(i_pSkill);
		}break;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldSkillManager::ReleaseDebuffSkill(ITEM *i_pSkill, BOOL i_bTimeCheck/* = TRUE*/)
/// \brief		���Ǵ�Ƽ - DebuffSkill ����
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldSkillManager::ReleaseDebuffSkill(ITEM *i_pSkill, BOOL i_bTimeCheck/* = TRUE*/) {
	for(int i=0; i < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; i++) {	
		switch(i_pSkill->ArrDestParameter[i]) {
			case DES_SKILL_NO_WARNING: {
				m_FieldDebuff.ReleaseDebuff(i_pSkill->ItemNum, DES_SKILL_NO_WARNING, i_bTimeCheck);
			}break;
			case DES_SKILL_SLOWMOVING: {
				m_FieldDebuff.ReleaseDebuff(i_pSkill->ItemNum, DES_SKILL_SLOWMOVING, i_bTimeCheck);
			}break;
			case DES_SKILL_SHIELD_PARALYZE: {
				m_FieldDebuff.ReleaseDebuff(i_pSkill->ItemNum, DES_SKILL_SHIELD_PARALYZE, i_bTimeCheck);
			}break;
			case DES_SKILL_MON_SILENCE_PRIMARY: {
				m_FieldDebuff.ReleaseDebuff(i_pSkill->ItemNum, DES_SKILL_MON_SILENCE_PRIMARY, i_bTimeCheck);
			}break;
			case DES_SKILL_MON_SILENCE_SECOND: {
				m_FieldDebuff.ReleaseDebuff(i_pSkill->ItemNum, DES_SKILL_MON_SILENCE_SECOND, i_bTimeCheck);
			}break;
			case DES_SKILL_MON_FREEZE_HP: {
				m_FieldDebuff.ReleaseDebuff(i_pSkill->ItemNum, DES_SKILL_MON_FREEZE_HP, i_bTimeCheck);
			}break;
			case DES_SKILL_MON_FREEZE_DP: {
				m_FieldDebuff.ReleaseDebuff(i_pSkill->ItemNum, DES_SKILL_MON_FREEZE_DP, i_bTimeCheck);
			}break;
			case DES_SKILL_MON_FREEZE_SP: {
				m_FieldDebuff.ReleaseDebuff(i_pSkill->ItemNum, DES_SKILL_MON_FREEZE_SP, i_bTimeCheck);
			}break;
			case DES_SKILL_MON_HOLD: {
				m_FieldDebuff.ReleaseDebuff(i_pSkill->ItemNum, DES_SKILL_MON_HOLD, i_bTimeCheck);
			}break;
			case DES_SKILL_MON_SILENCE_SKILL: {
				m_FieldDebuff.ReleaseDebuff(i_pSkill->ItemNum, DES_SKILL_MON_SILENCE_SKILL, i_bTimeCheck);
			}break;	
			// start 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ��ų �߰�
			case DES_SKILL_STOPMAGIC: {
				m_FieldDebuff.ReleaseDebuff(i_pSkill->ItemNum, DES_SKILL_STOPMAGIC, i_bTimeCheck);
			}break;	
			// end 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ��ų �߰�
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldSkillManager::ReleaseDotSkill(ITEM *i_pSkill)
/// \brief		���Ǵ�Ƽ - DotSkill ����
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldSkillManager::ReleaseDotSkill(ITEM *i_pSkill) {
	for(int i=0; i < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; i++) {	
		switch(i_pSkill->ArrDestParameter[i]) {
			case DES_SKILL_MON_STEALING: {
				m_FieldDot.ReleaseDotByCancelSkill(i_pSkill, DES_SKILL_MON_STEALING);
			}break;
			case DES_SKILL_MON_DRAIN: {
				m_FieldDot.ReleaseDotByCancelSkill(i_pSkill, DES_SKILL_MON_DRAIN);
			}break;	
			case DES_TIME_BOMB: {	// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -
				m_FieldDot.ReleaseDotByCancelSkill(i_pSkill, DES_TIME_BOMB);
			}break;	
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldSkillManager::CheckResistance(CFieldIOCPSocket * i_pTargetUser)
/// \brief		���Ǵ�Ƽ - ���� ������ ��� üũ
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldSkillManager::CheckResistance(CFieldIOCPSocket * i_pTargetUser) {
	if(NULL == i_pTargetUser) {
		return FALSE;
	}

	ParamValue_t	ResistanceValue = i_pTargetUser->GetParamResistance();
	if(RAND100() <= ResistanceValue) {
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldSkillManager::CheckResistance(CFieldIOCPSocket * i_pTargetUser)
/// \brief		���Ǵ�Ƽ - �� ���� �����ϰ� �������� ���� ������ �߰�
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldSkillManager::CheckOnceResistance(CFieldIOCPSocket * i_pTargetUser) {
	if(NULL == i_pTargetUser) {
		return FALSE;
	}
	
	ParamValue_t	ResistanceValue = i_pTargetUser->GetParamOnceResistance();
	if(RAND100() <= ResistanceValue) {
		return TRUE;
	}
	
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldSkillManager::ProcessingTimeBombSkill(float i_DamageRate)
/// \brief		���Ǵ�Ƽ(�������) - Ÿ�ӹ㽺ų ó��
/// \author		dhjin
/// \date		2010-03-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldSkillManager::ProcessingTimeBombSkill(float i_DamageRate)
{
	if (m_pFieldIOCPSocket->m_bNoDamage
		|| FALSE == m_pFieldIOCPSocket->IsValidCharacter())
	{	// ���� ����
		return;
	}

	float Damage = m_pFieldIOCPSocket->m_character.CurrentHP * i_DamageRate;
	if(0 >= (int)Damage)
	{	// Ÿ�ӹ� ��ų�δ� ���� ���ϼ�����!!
		return;
	}
	m_pFieldIOCPSocket->ChangeCurrentHP(-Damage);
}


// 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý���
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldSkillManager::MapBuffUse(ITEM_SKILL *i_pSkillItem)
/// \brief		�� ���� ����
/// \author		jhseol
/// \date		2013-03-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldSkillManager::MapBuffUse(ITEM_SKILL *i_pSkillItem)
{
	if ( NULL == i_pSkillItem || NULL == m_pFieldIOCPSocket )
	{
		return;
	}

	switch(i_pSkillItem->ItemInfo->SkillType)
	{
	case SKILLTYPE_CLICK:
		{
			//////////////////////////////
			/// ���� ȸ��
			if(i_pSkillItem->ItemInfo->IsExistDesParam(DES_SKILL_FULL_RECOVERY))
			{
				m_pFieldIOCPSocket->m_character.CurrentHP	= m_pFieldIOCPSocket->m_character.HP;
				m_pFieldIOCPSocket->m_character.CurrentDP	= m_pFieldIOCPSocket->m_character.DP;
				m_pFieldIOCPSocket->m_character.CurrentSP	= m_pFieldIOCPSocket->m_character.SP;
				m_pFieldIOCPSocket->m_character.CurrentEP	= m_pFieldIOCPSocket->m_character.EP;
				m_pFieldIOCPSocket->SendCharacterInfo(T_FC_CHARACTER_CHANGE_CURRENTHPDPSPEP);
			}
			//////////////////////////////
			/// ��ȭ
			else if(i_pSkillItem->ItemInfo->IsExistDesParam(DES_SKILL_CANCELALL))
			{
				SetPrepareUseBigBoom(FALSE);
			}

		}
		break;
	case SKILLTYPE_TIMELIMIT:
		{
			// ������ ���� BaseNum�� ��ų�� ����ϰ� �ִ��� Ȯ��
			// 2013-03-12 by jhseol, ��ų �ѹ��� �ý��� ����
#ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
			INT nOldSkillItemNum = IsSkillActivatedBySkillBaseNum(i_pSkillItem->ItemInfo->NextSkill);
#else	// #ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
			INT nOldSkillItemNum = IsSkillActivatedBySkillBaseNum(i_pSkillItem->ItemNum);
#endif	// #ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
			// end 2013-03-12 by jhseol, ��ų �ѹ��� �ý��� ����
			BYTE nOldSkillLevel = GetSkillActivatedLevelBySkillBaseNum(i_pSkillItem->ItemInfo->NextSkill);
			if (nOldSkillLevel != 0)
			{
				if ( nOldSkillLevel <= i_pSkillItem->ItemInfo->SkillLevel )
				{
					// �̹� ����ϰ� �ִ� ���
					DeactivateSkillByItemNum(nOldSkillItemNum, FALSE);

					//////////////////////////////
					// ü������ �߰�ó��
					ITEM *pItemInfo = ms_pFieldIOCP2->GetItemInfo(nOldSkillItemNum);
					if ( NULL != pItemInfo )
					{
						if(pItemInfo->IsExistDesParam(DES_SKILL_HALLUCINATION))
						{
							mt_auto_lock mtA(&m_pFieldIOCPSocket->m_mtvectFieldDummy);
							m_pFieldIOCPSocket->m_bDummyCheck			= FALSE;
							if(false == m_pFieldIOCPSocket->m_mtvectFieldDummy.empty())
							{
								INIT_MSG_WITH_BUFFER(MSG_FC_BATTLE_DELETE_DUMMY_OK, T_FC_BATTLE_DELETE_DUMMY_OK, pSMsg, SendBuf);
								pSMsg->AttackIndex		= m_pFieldIOCPSocket->m_character.ClientIndex;
								mtvectFieldDummy::iterator itr = m_pFieldIOCPSocket->m_mtvectFieldDummy.begin();
								for(; itr != m_pFieldIOCPSocket->m_mtvectFieldDummy.end(); itr++)
								{
									FIELD_DUMMY *pFDummy	= *itr;
									pSMsg->ItemFieldIndex	= pFDummy->ItemFieldIndex;
									
									// 2006-12-06 by cmkwon, ������ ��� ĳ���Ϳ��� ����
									m_pFieldIOCPSocket->SendInRangeMessageAroundMe(SendBuf, MSG_SIZE(MSG_FC_BATTLE_DELETE_DUMMY_OK), TRUE);
								}
								m_pFieldIOCPSocket->DeleteAllFieldDummy();			// 2006-12-06 by cmkwon, ��� ü���� �����Ѵ�.
							}
						}
					}
				}
				else
				{
					return;
				}
			}

			SetSkillParamFactor(i_pSkillItem->ItemInfo, m_pFieldIOCPSocket->m_character.ClientIndex);

			//////////////////////////////
			// ü������ �߰�ó��
			if (i_pSkillItem->ItemInfo->IsExistDesParam(DES_SKILL_HALLUCINATION))
			{			
				mt_auto_lock dummylock(&m_pFieldIOCPSocket->m_mtvectFieldDummy);
				if( TRUE == m_pFieldIOCPSocket->m_mtvectFieldDummy.empty())
				{
					m_pFieldIOCPSocket->m_bDummyCheck	= TRUE;
					for (int i = 0; i < i_pSkillItem->ItemInfo->GetParameterValue(DES_SKILL_HALLUCINATION); i++)
					{
						FIELD_DUMMY *pFieldDummy = new FIELD_DUMMY(m_pFieldIOCPSocket->m_uiDummyItemFieldIndex++, i_pSkillItem->ItemInfo);
						m_pFieldIOCPSocket->m_mtvectFieldDummy.push_back(pFieldDummy);					
						m_pFieldIOCPSocket->m_TimerManager.StartTimerField(TE_TYPE_DELETE_DUMMY, i_pSkillItem->ItemInfo->Time, pFieldDummy->ItemFieldIndex);
					}
					int		nSendByte = 0;
					BYTE	SendBuf[SIZE_MAX_PACKET];
					if(m_pFieldIOCPSocket->Make_MSG_FC_BATTLE_DROP_DUMMY_OK(SendBuf, &nSendByte, SIZE_MAX_PACKET))
					{
						CFieldIOCPSocket::ms_pFieldIOCP->SendInRangeMessageAroundCharacter(m_pFieldIOCPSocket->m_character.ClientIndex, SendBuf, nSendByte
							, m_pFieldIOCPSocket->GetCurrentFieldMapChannel(), TRUE, m_pFieldIOCPSocket->GetCurrentFieldMapChannel()->GetUserVisibleDiameterW());
					}
					m_pFieldIOCPSocket->m_uiDummyItemFieldIndex += 100;			
				}
			}		
			m_vectorAtivatedSkill.pushBackLock(ItemID_t(i_pSkillItem->UniqueNumber, i_pSkillItem->ItemNum));
		}
		break;
	}

	//////////////////////////////
	/// �׿��� ��ų�� Ŭ�󿡰� ��ų�� ����ߴٰ� �˷���.

	MessageData<T_FC_SKILL_USE_SKILL_OK> msg;

	msg->SkillItemID.ItemNum	= i_pSkillItem->ItemNum;
	msg->AttackIndex			= 0;
	msg->TargetIndex			= m_pFieldIOCPSocket->m_character.ClientIndex;
	msg->UseTime				= ATUM_DATE_TIME { true };

	m_pFieldIOCPSocket->SendInRangeMessageAroundMe(msg, true);
}
// end 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý���