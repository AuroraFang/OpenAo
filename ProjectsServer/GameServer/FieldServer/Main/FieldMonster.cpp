// FieldMonster.cpp: implementation of the CFieldMonster class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FieldMonster.h"
#include "FieldIOCP.h"
#include "FieldGlobal.h"
#include "TriggerFunctionCrystal.h"		// 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFieldMonster::CFieldMonster()
{
	m_usSendCountsMonsterMoveOK			= 0;
	m_pCurrentFieldMapChannelMonster	= NULL;

	m_mtlistDropItem.lock();
	m_mtlistDropItem.clear();
	m_mtlistDropItem.unlock();

	m_mtVectCltIdxForMonsterMoveOK.reserve(100);
	m_mtVectCltIdxForExcludeMonsterMoveOK.reserve(20);

	m_nAttackerCliIdx						= 0;				// 2006-11-07 by cmkwon
	m_dwLastTickSetAttackerCliIdx			= 0;				// 2006-11-07 by cmkwon

	util::zero(&m_bBarrierInfo, sizeof(BARRIER_SKILL_INFO));	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �踮��
	util::zero(&m_ImmediateBarrier, sizeof(IMMEDIATE_BARRIER_SKILL_INFO));	// 2009-09-09 ~ 2010-01-15 by dhjin, ���Ǵ�Ƽ - ���� ��� ���� �踮�� ��ų(��޹��⸸�ߵ�)

	// start 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
	mt_auto_lock mtAuto(&m_mtVectTriggerFunctionPtr);
	m_mtVectTriggerFunctionPtr.clear();
	// end 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
}

CFieldMonster::~CFieldMonster()
{
	m_mtlistDropItem.lock();
	if(m_mtlistDropItem.empty() == false)
	{
		mtlistMonsterDropItem::iterator itr = m_mtlistDropItem.begin();
		while(itr != m_mtlistDropItem.end())
		{
			delete(itr->pItemGeneral);
			itr++;
		}
		m_mtlistDropItem.clear();
	}
	m_mtlistDropItem.unlock();
}	// Heap override (20 bytes)


//////////////////////////////////////////////////////////////////////
// Property
//////////////////////////////////////////////////////////////////////
USHORT CFieldMonster::GetSendCountsMonsterMoveOK(void)
{
	return m_usSendCountsMonsterMoveOK;
}

void CFieldMonster::SetCityWarFieldMonster(CCityWar *i_pWar)
{
	m_pCityWar2 = i_pWar;
}


//////////////////////////////////////////////////////////////////////
// Method /
//////////////////////////////////////////////////////////////////////
void CFieldMonster::LockDropItemList(void)
{
	m_mtlistDropItem.lock();
}
void CFieldMonster::UnlockDropItemList(void)
{
	m_mtlistDropItem.unlock();
}

// 2007-12-07 by cmkwon, ������� �ʴ� �Լ� ��
//void CFieldMonster::CreateDropItem(CFieldIOCP *pFieldIOCP, CFieldIOCPSocket *i_pFISockGive2MonsterTopDamage)
//{
//	// ����Ʈ�� �������� �̸� �����صΰ�, ���� ���Ľ�, �������� ���� 
//	// �� �ִ� ����Ʈ�� ������ ������ �ְ�, �ƴ� �����ϴ� ������
//	// ó���ϱ�� ����, 20040220, kelovon
//
//	// ����Ʈ�� ��� ������ ������ ����Ʈ ���ε��� ���� ��ȿ
//	if (!pFieldIOCP->m_ServicePauseManager.GetQuestServiceState())
//	{
//		return;
//	}
//
//	multimap<int, MONSTER2ITEM*>*			pMapMonster2Item	= &pFieldIOCP->m_mapMonster2Item;
//	multimap<int, MONSTER2ITEM*>::iterator	itr					= pMapMonster2Item->find(MonsterInfoPtr->MonsterUnitKind);
//	if (itr == pMapMonster2Item->end())
//	{
//		return;
//	}
//	
//// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - �Ʒ��� ���� �Լ��� ������
////	float fPlusSPI			= pFieldIOCP->m_gameEventManager.GetSPIRate(INFLUENCE_TYPE_NORMAL, FALSE);
////	float fPlusDropItem		= pFieldIOCP->m_gameEventManager.GetDropItemRate(INFLUENCE_TYPE_NORMAL, FALSE) - 1.0f;
////	float fPlusDropRareItem	= pFieldIOCP->m_gameEventManager.GetDropRareItemRate(INFLUENCE_TYPE_NORMAL, FALSE) - 1.0f;
//	float fSPIRate			= 1.0f + pFieldIOCP->m_gameEventManager.GetSPIPlusRate(INFLUENCE_TYPE_NORMAL, FALSE);
//	float fPlusDropItem		= pFieldIOCP->m_gameEventManager.GetDropItemPlusRate(INFLUENCE_TYPE_NORMAL, FALSE);
//	float fPlusDropRareItem	= pFieldIOCP->m_gameEventManager.GetDropRareItemPlusRate(INFLUENCE_TYPE_NORMAL, FALSE);
//	if(i_pFISockGive2MonsterTopDamage
//		&& i_pFISockGive2MonsterTopDamage->IsValidCharacter())
//	{
//		fSPIRate			= i_pFISockGive2MonsterTopDamage->GetPlusRateSPI();
//		fPlusDropItem		= i_pFISockGive2MonsterTopDamage->GetPlusRateDropItem() - 1.0f;
//		fPlusDropRareItem	= i_pFISockGive2MonsterTopDamage->GetPlusRateDropRareItem() - 1.0f;
//	}
//
//	do
//	{
//		MONSTER2ITEM	*pMonster2Item	= itr->second;
//		ITEM			*pItem			= pFieldIOCP->GetItemInfo(pMonster2Item->ItemNumber);
//
//		if (pItem == NULL)
//		{
//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] Monster DropItem Error1, MonsterUnitKind(%8d) ItemNumber(%8d)\r\n"
//				, MonsterInfoPtr->MonsterUnitKind, pMonster2Item->ItemNumber);
//			itr++;
//			continue;
//		}
//
//		int pValue = RAND1000K();
//		if (pMonster2Item->Probability < pValue - (Prob1000K_t)(pMonster2Item->Probability*fPlusDropItem))
//		{// ���� ����
//			
//			itr++;
//			continue;
//		}
//
//		if(pMonster2Item->MinCount < 0
//			|| pMonster2Item->MaxCount < 0
//			|| pMonster2Item->MaxCount < pMonster2Item->MinCount)
//		{
//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] Monster DropItem Error2, MonsterUnitKind(%8d) ItemNumber(%8d) MinCount(%4d) MaxCount(%8d)\r\n"
//				, MonsterInfoPtr->MonsterUnitKind, pMonster2Item->ItemNumber, pMonster2Item->MinCount, pMonster2Item->MaxCount);
//			itr++;
//			continue;
//		}
//
//		INT nAmount = RANDI(pMonster2Item->MinCount, pMonster2Item->MaxCount);
//		if(0 >= nAmount)
//		{
//			itr++;
//			continue;
//		}
//
//		if(g_pFieldGlobal->InMasangServer())
//		{// 2006-01-02 by cmkwon, �׽�Ʈ �ڵ�
//			char szTemp[1024];
//			sprintf(szTemp, "	Mon(%s:%d) DromItem ==> Item(%s:%d), Counts(%d)\r\n"
//				, MonsterInfoPtr->MonsterName, MonsterIndex, pItem->ItemName, pItem->ItemNum, nAmount);
//			DBGOUT(szTemp);
//			g_pFieldGlobal->WriteSystemLog(szTemp);
//		}
//		
//		if (IS_COUNTABLE_ITEM(pItem->Kind))
//		{// Countable Item�� ���
//
//			ITEM_GENERAL *pNewItem = new ITEM_GENERAL(pItem);
//			if(MONEY_ITEM_NUMBER == pItem->ItemNum)
//			{
//				nAmount *= fSPIRate;
//			}
//
//			pNewItem->CurrentCount			= nAmount;
//			MONSTER_DROP_ITEM_GENERAL tmpMonsterDropItem;
//			tmpMonsterDropItem.pItemGeneral	= pNewItem;
//			tmpMonsterDropItem.QuestIndex	= pMonster2Item->QuestIndex;
//			m_mtlistDropItem.push_back(tmpMonsterDropItem);
//		}
//		else
//		{// ������ �Ϲ� �������� ���(Countable Item�� �ƴѰ��)
//			
//			for (int i = 0; i < nAmount; i++)
//			{
//				ITEM_GENERAL *pStoreItem = new ITEM_GENERAL(pItem);
//				CFieldIOCPSocket::ApplyRareItem(this, pMonster2Item, fPlusDropRareItem
//					, CFieldIOCPSocket::ms_pFieldIOCP, pStoreItem);
//
//				//DBGOUT("%d(%#x) ", pStoreItem->ItemNum, pStoreItem);
//				MONSTER_DROP_ITEM_GENERAL tmpMonsterDropItem;
//				tmpMonsterDropItem.pItemGeneral = pStoreItem;
//				tmpMonsterDropItem.QuestIndex = pMonster2Item->QuestIndex;
//
//				m_mtlistDropItem.push_back(tmpMonsterDropItem);
//			}
//		}// end
//
//		itr++;
//	} while(itr != pMapMonster2Item->upper_bound(MonsterInfoPtr->MonsterUnitKind));	
//}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldMonster::SetAttackerCliIdx(ClientIndex_t i_cliIdx)
/// \brief		
/// \author		cmkwon
/// \date		2006-11-07 ~ 2006-11-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldMonster::SetAttackerCliIdx(ClientIndex_t i_cliIdx)
{
	if(BELL_NOTATTACK == this->MonsterInfoPtr->Belligerence
		|| IS_STRATEGYPOINT_MONSTER(this->MonsterInfoPtr->Belligerence))	// 2006-12-13 by cmkwon, ��������Ʈ ���͵� ���� �������� �ʴ´�
	{// 2006-11-24 by cmkwon, ����� ����
		return FALSE;
	}

	if(0 != m_nTargetIndex)
	{
		return FALSE;
	}

	DWORD dwCur = timeGetTime();
	if(0 == m_nAttackerCliIdx
		|| dwCur - m_dwLastTickSetAttackerCliIdx > 10000)
	{
		m_nAttackerCliIdx				= i_cliIdx;
		m_dwLastTickSetAttackerCliIdx	= dwCur;
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ClientIndex_t CFieldMonster::GetAttackerCliIdx(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-11-07 ~ 2006-11-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ClientIndex_t CFieldMonster::GetAttackerCliIdx(void)
{
	if(0 != m_nTargetIndex)
	{
		return m_nTargetIndex;
	}

	DWORD dwCur = timeGetTime();
	if(0 != m_nAttackerCliIdx
		&& dwCur - m_dwLastTickSetAttackerCliIdx < 10000)
	{
		return m_nAttackerCliIdx;
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : CMonster::GetCurrentPosition
// ��ȯ�Ǵ� ��  : D3DXVECTOR3
// �� �� �� ��  : void
// �� �� �� ��  : inline �Լ�
//					������ ���� PositionVector�� �����ϴ� �Լ��� �ӵ��� ����
//					������ PositionVector ������ ���̱� ���� �Լ��� Attck�ÿ�
//					�¾Ҵ����� ������ ����
D3DXVECTOR3 CFieldMonster::GetCurrentPosition(void)
{
	if (m_BeforePosition == PositionVector)
	{
		return m_BeforePosition;
	}

	DWORD		dwCurrentGap = timeGetTime() - m_dwTimeLastMoved;
	D3DXVECTOR3 TempVector3;

	D3DXVec3Normalize(&TempVector3, &(PositionVector - m_BeforePosition));
	return m_BeforePosition + TempVector3 * dwCurrentGap/MONSTER_UPDATE_MOVE_TERM_TICK;
}


ITEM * CFieldMonster::FindItemPtrWithItemNum(INT i_uiItemNum)
{
	if(NULL == MonsterInfoPtr){				return NULL;}
	

	for(int i = 0; i < ARRAY_SIZE_MONSTER_ITEM; i++)
	{
		if(this->MonsterInfoPtr->ItemInfo[i].pItemInfo
			&& MonsterInfoPtr->ItemInfo[i].pItemInfo->ItemNum == i_uiItemNum)
		{
			return MonsterInfoPtr->ItemInfo[i].pItemInfo;
		}
	}
	return NULL;
}

ITEM * CFieldMonster::FindItemPtrByOrBitTyp(BYTE i_byOrBitType)
{
	if(NULL == MonsterInfoPtr){				return NULL;}
	

	for(int i = 0; i < ARRAY_SIZE_MONSTER_ITEM; i++)
	{
		if(this->MonsterInfoPtr->ItemInfo[i].pItemInfo
			&& MonsterInfoPtr->ItemInfo[i].pItemInfo->OrbitType == i_byOrBitType)
		{
			return MonsterInfoPtr->ItemInfo[i].pItemInfo;
		}
	}
	return NULL;
}

USHORT CFieldMonster::IncreaseSendCountsMonsterMoveOK(void)
{
	m_usSendCountsMonsterMoveOK++;
	return m_usSendCountsMonsterMoveOK;
}

float CFieldMonster::IncreaseMonsterCurrentHP(float i_fValue)
{
	mt_auto_lock mtAuto(&m_mtvectorAttackedInfoPtr);

	// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
	/*if(CurrentHP <= 0.0f
		|| FALSE == this->IsValidMonster())
	{
		return CurrentHP;
	}*/
	if( this->CurrentHP <= 0.0f
		|| FALSE == this->IsValidMonster())
	{
		return this->CurrentHP;
	}
	// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )


	float	BeforeHP = 0.0f;
	// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
	/*BeforeHP	= this->CurrentHP;
	CurrentHP	= min(MonsterInfoPtr->MonsterHP, CurrentHP + i_fValue);
	
	float AfterHP = 0.0f;
	AfterHP		= this->CurrentHP;*/

	BeforeHP		= this->CurrentHP;
	this->CurrentHP	= min(this->MonsterInfoExtend.fMaxHP , this->CurrentHP + i_fValue );
	
	float AfterHP = 0.0f;
	AfterHP			= this->CurrentHP;
	// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )

	mtAuto.auto_unlock_cancel();

	//////////////////////////////////////////////////////////////////////////
	// 2008-03-27 by dhjin, ���� ���� ǥ�� ��ȹ�� - 
	if(m_pCurrentFieldMapChannelMonster->m_pFieldIOCPMapChannel->m_InflWarManager.CheckSummonBoss()
		&& this->MSMonsterCheckWithoutMSBossMonster())
	{
		// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
		//this->HPRateCalcByMSWarInfoDisPlay(MonsterInfoPtr->MonsterHP, BeforeHP, AfterHP);
		this->HPRateCalcByMSWarInfoDisPlay( this->MonsterInfoExtend.fMaxHP , BeforeHP , AfterHP );
		// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
	}
	else if (IS_STRATEGYPOINT_MONSTER(MonsterInfoPtr->Belligerence))
	{
		INT AfterHPRate = CalcGradeByRate(this->MonsterInfoExtend.fMaxHP, AfterHP, MS_WAR_HP_RATE);
		if (AfterHPRate != CalcGradeByRate(this->MonsterInfoExtend.fMaxHP, BeforeHP, MS_WAR_HP_RATE))
		{
			m_pCurrentFieldMapChannelMonster->m_pFieldIOCPMapChannel->m_InflWarManager.UpdateSummonStrategicPointInfoHP(m_pCurrentFieldMapChannelMonster->GetMapChannelIndex().MapIndex, AfterHPRate);
		}
	}
	
	return AfterHP;
}

// 2010-03-18 by dhjin, ���� �� ���� ó�� ��ƾ ���� - DecreaseMonsterCurrentHP �� ���� - �ּ�ó��
// float CFieldMonster::DecreaseMonsterCurrentHPByCharacter(float i_fValue, CFieldIOCPSocket *i_AttackFISock, vectActionInfo *i_pAInfoAggroList)
// {
// 	mt_auto_lock mtAuto(&m_mtvectorAttackedInfoPtr);
// 	if(this->CurrentHP <= 0.0f
// 		|| TRUE == COMPARE_BODYCON_BIT(BodyCondition, BODYCON_DEAD_MASK)
// 		|| MS_PLAYING != this->m_enMonsterState)
// 	{
// 		return this->CurrentHP;
// 	}
// 
// 	// 2010-03-18 by dhjin, ���� �� ���� ó�� ��ƾ ���� - �ּ� ó�� ProcessingDamagedMonsterByCharacter �ű�
// // 	//////////////////////////////////////////////////////////////////////////
// // 	// 2008-12-23 by dhjin, ���� ���� �߰���
// // 	if(IS_INFLWAR_MONSTER(this->MonsterInfoPtr->Belligerence)
// // 		&& (i_AttackFISock->ms_pFieldIOCP->m_InflWarManager.CheckSummonBossAndStrategyPoint()
// // 			|| i_AttackFISock->ms_pFieldIOCP->m_OutPostManager.CheckALLOutPostWaring())
// // 		)
// // 	{
// // 		UID32_t WarContribution;
// // 		if(this->CurrentHP <= i_fValue)
// // 		{// 2008-12-23 by dhjin, ���Ķ�� ���� �Ƿ� �ִ´�.
// // 			WarContribution = (UID32_t)this->CurrentHP;
// // 		}
// // 		else
// // 		{
// // 			WarContribution = (UID32_t)i_fValue;
// // 		}
// // 		i_AttackFISock->ms_pFieldIOCP->WarContributionCalculation(i_AttackFISock, WarContribution);
// // 	}
// 		
// 	//////////////////////////////////////////////////////////////////////////
// 	// 2008-03-27 by dhjin, ���� ���� ǥ�� ��ȹ�� - 
// 	if(m_pCurrentFieldMapChannelMonster->m_pFieldIOCPMapChannel->m_InflWarManager.CheckSummonBoss()
// 		&& this->MSMonsterCheckWithoutMSBossMonster())
// 	{
// 		float	BeforeHP, AfterHP;
// 		BeforeHP = this->CurrentHP;
// 
// 		i_fValue = min(i_fValue, this->CurrentHP);
// 		this->CurrentHP -= i_fValue;
// 
// 		AfterHP = this->CurrentHP;
// 		this->HPRateCalcByMSWarInfoDisPlay(MonsterInfoPtr->MonsterHP, BeforeHP, AfterHP);
// 	}
// 	else
// 	{
// 		i_fValue = min(i_fValue, this->CurrentHP);
// 		this->CurrentHP -= i_fValue;
// 	}
// 
// 	// 2010-03-18 by dhjin, ���� �� ���� ó�� ��ƾ ���� - �ּ� ó�� ProcessingDamagedMonsterByCharacter �ű�
// // 	///////////////////////////////////////////////////////////////////////////////
// // 	// ���� ���� ������ �߰��Ѵ�
// // 	this->InserttoAttackedInfoList(&ActionInfo(i_AttackFISock->GetCharacter()->ClientIndex, timeGetTime(), i_fValue));
// // 
// // 	///////////////////////////////////////////////////////////////////////////////
// // 	// 2006-07-21 by cmkwon, Aggro �߰��ϱ�
// // 	for(int i=0; i < i_pAInfoAggroList->size(); i++)
// // 	{
// // 		this->InserttoAttackedInfoList( &((*i_pAInfoAggroList)[i]) );
// // 	}
// 	
// 	// 2010-03-18 by dhjin, ���� �� ���� ó�� ��ƾ ���� - ��� ���ϴ� �ý��� �ּ� ó��
// // 	CCityWar *ptmWar = m_pCityWar2;
// // 	if(ptmWar
// // 		&& CITYWAR_STATE_STARTED == ptmWar->GetCityWarState()
// // 		&& this->MonsterInfoPtr->MonsterUnitKind == ptmWar->CityWarQuestMonsterUID
// // 		&& IS_VALID_UNIQUE_NUMBER(i_AttackFISock->GetCharacter()->GuildUniqueNumber))
// // 	{
// // 		ptmWar->AddSumOfDamageCityWar(i_AttackFISock->GetCharacter()->GuildUniqueNumber, i_fValue);
// // 	}
// 
// 	// 2010-03-18 by dhjin, ���� �� ���� ó�� ��ƾ ���� - ��� ���ϴ� �ý��� �ּ� ó��
// // 	// 2007-08-24 by dhjin, �������� ��ȣ���̸� ���ܰ� �������� �����Ѵ�.
// // 	if((BELL_ATTACK_OUTPOST_PROTECTOR == this->MonsterInfoPtr->Belligerence
// // 		|| BELL_OUTPOST_PROTECTOR_VCN == this->MonsterInfoPtr->Belligerence
// // 		|| BELL_OUTPOST_PROTECTOR_ANI == this->MonsterInfoPtr->Belligerence)
// // 		&& IS_VALID_UNIQUE_NUMBER(i_AttackFISock->m_character.GuildUniqueNumber)		// 2008-04-02 by dhjin, �������� �������� ������ ��꿡 ������ �ĺ� ���� ���� - ��ȿ�� �������� üũ ��ƾ �߰�
// // 		&& (!i_AttackFISock->ms_pFieldIOCP->m_InflWarManager.CheckLeader_SubLeaderGuildUID(i_AttackFISock->m_character.GuildUniqueNumber)) // 2007-09-07 by dhjin, ������ ����, �������� ���������� ������ ����
// // 		&& (!i_AttackFISock->ms_pFieldIOCP->m_OutPostManager.CheckOutPostPossessByGuildUID(i_AttackFISock->m_character.GuildUniqueNumber)) // 2007-11-29 by dhjin, �������� ���� ������ �ٸ� ���������� ������ �� �����Ƿ� �����Ѵ�.	
// // 		&& (!i_AttackFISock->ms_pFieldIOCP->m_InflWarManager.CheckRegLeaderCandidateGuild(i_AttackFISock->m_character.GuildUniqueNumber))	// 2008-04-02 by dhjin, �������� �������� ������ ��꿡 ������ �ĺ� ���� ���� - ������ �ĺ� ����� �������� üũ
// // 		)
// // 	{
// // 		i_AttackFISock->ms_pFieldIOCP->m_OutPostManager.AddDamageOfProtectorW(i_AttackFISock->GetCharacter()->MapChannelIndex.MapIndex, i_AttackFISock->GetCharacter()->InfluenceType
// // 																				, i_AttackFISock->GetCharacter()->GuildUniqueNumber, i_AttackFISock->m_character.GuildName, i_fValue);
// // 	}
// 
// 	return this->CurrentHP;
// }

// 2010-03-18 by dhjin, ���� �� ���� ó�� ��ƾ ���� - DecreaseMonsterCurrentHP �� ���� - �ּ�ó��
///////////////////////////////////////////////////////////////////////////////
/// \fn			float CFieldMonster::DecreaseMonsterCurrentHPByMonster(float i_fValue)
/// \brief		���Ǵ�Ƽ - ���Ϳ��� ���� ���� 
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// float CFieldMonster::DecreaseMonsterCurrentHPByMonster(float i_fValue) {
// 	mt_auto_lock mtAuto(&m_mtvectorAttackedInfoPtr);
// 	if(this->CurrentHP <= 0.0f
// 		|| TRUE == COMPARE_BODYCON_BIT(BodyCondition, BODYCON_DEAD_MASK)
// 		|| MS_PLAYING != this->m_enMonsterState) {
// 		return this->CurrentHP;
// 	}
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	// 2008-03-27 by dhjin, ���� ���� ǥ�� ��ȹ�� - 
// 	if(m_pCurrentFieldMapChannelMonster->m_pFieldIOCPMapChannel->m_InflWarManager.CheckSummonBoss()
// 		&& this->MSMonsterCheckWithoutMSBossMonster()) {
// 		float	BeforeHP, AfterHP;
// 		BeforeHP = this->CurrentHP;
// 		
// 		i_fValue = min(i_fValue, this->CurrentHP);
// 		this->CurrentHP -= i_fValue;
// 		
// 		AfterHP = this->CurrentHP;
// 		this->HPRateCalcByMSWarInfoDisPlay(MonsterInfoPtr->MonsterHP, BeforeHP, AfterHP);
// 	}
// 	else {
// 		i_fValue = min(i_fValue, this->CurrentHP);
// 		this->CurrentHP -= i_fValue;
// 	}
// 	
// 	return this->CurrentHP;
// }

// 2010-03-18 by dhjin, ���� �� ���� ó�� ��ƾ ���� - DecreaseMonsterCurrentHP �� ���� - �ּ�ó��
///////////////////////////////////////////////////////////////////////////////
/// \fn			float CFieldMonster::DecreaseMonsterCurrentHPByPenalty(float i_fValue)
/// \brief		���Ǵ�Ƽ(�������) - �г�Ƽ ������ ���Ͽ� ���� HP ����
/// \author		dhjin
/// \date		2010-03-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// float CFieldMonster::DecreaseMonsterCurrentHPByPenalty(float i_fValue)
// {
// 	mt_auto_lock mtAuto(&m_mtvectorAttackedInfoPtr);
// 	if(this->CurrentHP <= 0.0f
// 		|| TRUE == COMPARE_BODYCON_BIT(BodyCondition, BODYCON_DEAD_MASK)
// 		|| MS_PLAYING != this->m_enMonsterState) {
// 		return this->CurrentHP;
// 	}
// 
// 	i_fValue = min(i_fValue, this->CurrentHP);
// 	this->CurrentHP -= i_fValue;
// 	this->CurrentHP = max(this->CurrentHP, PENALTY_REMAIN_HP); // �г�Ƽ�� ���� HP���Ҵ� �ּ� 1�� �������ش�.
// 
// 	return this->CurrentHP;
// }

///////////////////////////////////////////////////////////////////////////////
/// \fn			float CFieldMonster::DecreaseMonsterCurrentHP(float i_fValue, BOOL i_bPenalty/*=FALSE*/)
/// \brief		���� �� ���� ó�� ��ƾ ���� - DecreaseMonsterCurrentHP �� ����
/// \author		dhjin
/// \date		2010-03-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CFieldMonster::DecreaseMonsterCurrentHP(float i_fValue, BOOL i_bPenalty/*=FALSE*/)
{
	mt_auto_lock mtAuto(&m_mtvectorAttackedInfoPtr);
	if(this->CurrentHP <= 0.0f
		|| FALSE == this->IsValidMonster()) {
		return this->CurrentHP;
	}
	
	if(i_bPenalty)
	{
		// �г�Ƽ ó��
		i_fValue = min(i_fValue, this->CurrentHP);
		this->CurrentHP -= i_fValue;
		this->CurrentHP = max(this->CurrentHP, PENALTY_REMAIN_HP); // �г�Ƽ�� ���� HP���Ҵ� �ּ� PENALTY_REMAIN_HP�� �������ش�.
		
		return this->CurrentHP;
	}

	float	BeforeHP;

	BeforeHP = this->CurrentHP;	
	i_fValue = min(i_fValue, this->CurrentHP);
	this->CurrentHP -= i_fValue;
	
	float AfterHP = this->CurrentHP;
	mtAuto.auto_unlock_cancel();

	//////////////////////////////////////////////////////////////////////////
	// 2008-03-27 by dhjin, ���� ���� ǥ�� ��ȹ�� - 
	if(m_pCurrentFieldMapChannelMonster->m_pFieldIOCPMapChannel->m_InflWarManager.CheckSummonBoss()
		&& this->MSMonsterCheckWithoutMSBossMonster())
	{
		// 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.)
		//this->HPRateCalcByMSWarInfoDisPlay(MonsterInfoPtr->MonsterHP, BeforeHP, AfterHP);
		this->HPRateCalcByMSWarInfoDisPlay ( this->MonsterInfoExtend.fMaxHP , BeforeHP , AfterHP );
		// End 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.)
	}
	else if (IS_STRATEGYPOINT_MONSTER(MonsterInfoPtr->Belligerence))
	{
		INT AfterHPRate = CalcGradeByRate(this->MonsterInfoExtend.fMaxHP, AfterHP, MS_WAR_HP_RATE);
		if (AfterHPRate != CalcGradeByRate(this->MonsterInfoExtend.fMaxHP, BeforeHP, MS_WAR_HP_RATE))
		{
			m_pCurrentFieldMapChannelMonster->m_pFieldIOCPMapChannel->m_InflWarManager.UpdateSummonStrategicPointInfoHP(m_pCurrentFieldMapChannelMonster->GetMapChannelIndex().MapIndex, AfterHPRate);
		}
	}

	return AfterHP;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldMonster::ProcessingDamagedMonsterByCharacter(float i_fValue, CFieldIOCPSocket *i_AttackFISock, vectActionInfo *i_pAInfoAggroList)
/// \brief		���Ǵ�Ƽ(�������) - ���� �� ���� ó�� ��ƾ ���� -
/// \author		dhjin
/// \date		2010-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldMonster::ProcessingDamagedMonsterByCharacter(float i_fValue, CFieldIOCPSocket *i_AttackFISock, vectActionInfo *i_pAInfoAggroList)
{
	if(NULL == i_AttackFISock
		|| this->CurrentHP <= 0.0f
		|| TRUE == COMPARE_BODYCON_BIT(BodyCondition, BODYCON_DEAD_MASK)
		|| MS_PLAYING != this->m_enMonsterState)
	{
		return;
	}
	
	this->ProcessingInserttoAttackedInfoList(i_fValue, i_AttackFISock, i_pAInfoAggroList);
	
	//////////////////////////////////////////////////////////////////////////
	// 2008-12-23 by dhjin, ���� ���� �߰���
	if(IS_INFLWAR_MONSTER(this->MonsterInfoPtr->Belligerence)
		&& (	i_AttackFISock->ms_pFieldIOCP->m_InflWarManager.CheckSummonBossAndStrategyPoint()
				&& !IS_ONEY_ATTACK_MONSTER(this->MonsterInfoPtr->Belligerence)	// 2010-07-06 by jskim, �⿩�� ���� ���� ���� �߰�
				|| i_AttackFISock->ms_pFieldIOCP->m_OutPostManager.CheckALLOutPostWaring()	)
		)
	{
		UID32_t WarContribution;
		if(this->CurrentHP <= i_fValue)
		{// 2008-12-23 by dhjin, ���Ķ�� ���� �Ƿ� �ִ´�.
			WarContribution = (UID32_t)this->CurrentHP;
		}
		else
		{
			WarContribution = (UID32_t)i_fValue;
		}

		i_AttackFISock->ms_pFieldIOCP->WarContributionCalculation(i_AttackFISock, WarContribution);
	}
	
	// 2007-08-24 by dhjin, �������� ��ȣ���̸� ���ܰ� �������� �����Ѵ�.
	if((BELL_ATTACK_OUTPOST_PROTECTOR == this->MonsterInfoPtr->Belligerence
		|| BELL_OUTPOST_PROTECTOR_VCN == this->MonsterInfoPtr->Belligerence
		|| BELL_OUTPOST_PROTECTOR_ANI == this->MonsterInfoPtr->Belligerence)
		&& IS_VALID_UNIQUE_NUMBER(i_AttackFISock->m_character.GuildUniqueNumber)		// 2008-04-02 by dhjin, �������� �������� ������ ��꿡 ������ �ĺ� ���� ���� - ��ȿ�� �������� üũ ��ƾ �߰�
		&& (!i_AttackFISock->ms_pFieldIOCP->m_InflWarManager.CheckLeader_SubLeaderGuildUID(i_AttackFISock->m_character.GuildUniqueNumber)) // 2007-09-07 by dhjin, ������ ����, �������� ���������� ������ ����
		&& (!i_AttackFISock->ms_pFieldIOCP->m_OutPostManager.CheckOutPostPossessByGuildUID(i_AttackFISock->m_character.GuildUniqueNumber)) // 2007-11-29 by dhjin, �������� ���� ������ �ٸ� ���������� ������ �� �����Ƿ� �����Ѵ�.	
		&& (!i_AttackFISock->ms_pFieldIOCP->m_InflWarManager.CheckRegLeaderCandidateGuild(i_AttackFISock->m_character.GuildUniqueNumber))	// 2008-04-02 by dhjin, �������� �������� ������ ��꿡 ������ �ĺ� ���� ���� - ������ �ĺ� ����� �������� üũ
		)
	{
		i_AttackFISock->ms_pFieldIOCP->m_OutPostManager.AddDamageOfProtectorW(i_AttackFISock->GetCharacter()->MapChannelIndex.MapIndex, i_AttackFISock->GetCharacter()->InfluenceType
			, i_AttackFISock->GetCharacter()->GuildUniqueNumber, i_AttackFISock->m_character.GuildName, i_fValue);
	}

	// start 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
	if( TRUE == IsTriggerFunction() )
	{
		// �� ���ʹ� Ʈ���� �ý��۰� ����Ǿ� �ִ�.
		mt_auto_lock mtAuto(&m_mtVectTriggerFunctionPtr);
		
		for(int i=0; i<m_mtVectTriggerFunctionPtr.size(); i++)
		{
			CTriggerFunction *pTriggerFunction = m_mtVectTriggerFunctionPtr[i];
			pTriggerFunction->OnAddDamage(this->MonsterInfoPtr->MonsterUnitKind, i_AttackFISock->GetCharacter()->CharacterUniqueNumber, i_AttackFISock->GetCharacter()->GuildUniqueNumber, i_AttackFISock->GetCharacter()->InfluenceType, i_fValue);
		}
	}
	// end 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
}

///////////////////////////////////////////////////////////////////////////////
// virtual
///////////////////////////////////////////////////////////////////////////////
void CFieldMonster::ResetMonster(void)
{
	CMonster::ResetMonster();

	m_usSendCountsMonsterMoveOK	= 0;

	m_mtlistDropItem.lock();
	if(m_mtlistDropItem.empty() == false)
	{
		mtlistMonsterDropItem::iterator itr = m_mtlistDropItem.begin();
		while(itr != m_mtlistDropItem.end())
		{
			delete(itr->pItemGeneral);
			itr++;
		}
	}
	m_mtlistDropItem.clear();
	m_mtlistDropItem.unlock();

	m_dwTimeCreatedTick						= 0;
	m_pCityWar2								= NULL;
	m_dwLastTickInfluenceBossAttackedMsg	= 0;


	m_nAttackerCliIdx						= 0;				// 2006-11-07 by cmkwon
	m_dwLastTickSetAttackerCliIdx			= 0;				// 2006-11-07 by cmkwon

	m_mtvectSAttackParameterList.clearLock();

	// start 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
	mt_auto_lock mtAuto(&m_mtVectTriggerFunctionPtr);
	m_mtVectTriggerFunctionPtr.clear();
	// end 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
}

void CFieldMonster::InserttoAttackedInfoList(ActionInfo *i_pAttackedInfo)
{
	mt_auto_lock mtAuto(&m_mtvectorAttackedInfoPtr);
	if(MS_PLAYING != this->m_enMonsterState)
	{
		return;
	}

	mtvectorActionInfoPtr::iterator it = find_if(m_mtvectorAttackedInfoPtr.begin(), m_mtvectorAttackedInfoPtr.end(),
		listActionInfoPtr_UnitIndex_Find_Object(i_pAttackedInfo->UnitIndex));
	if(it != m_mtvectorAttackedInfoPtr.end())
	{	// ����Ʈ�� UnitIndex�� ���� ������ ����

		(*it)->dwCurTick = i_pAttackedInfo->dwCurTick;
		(*it)->fSumDamage += i_pAttackedInfo->fSumDamage;
	}
	else
	{
		ActionInfo *ptmAInfo = new ActionInfo;
		*ptmAInfo = *i_pAttackedInfo;
		m_mtvectorAttackedInfoPtr.push_back(ptmAInfo);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldMonster::APInsertAttackParameter(SATTACK_PARAMETER *i_pAttParam)
/// \brief		
/// \author		cmkwon
/// \date		2007-06-08 ~ 2007-06-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldMonster::APInsertAttackParameter(SATTACK_PARAMETER *i_pAttParam)
{
	mt_auto_lock mtA(&m_mtvectSAttackParameterList);

	if(APFindAttackParameterNoLock(i_pAttParam->WeaponIndex))
	{
		return FALSE;
	}

	m_mtvectSAttackParameterList.push_back(*i_pAttParam);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SATTACK_PARAMETER *CFieldMonster::APFindAttackParameterNoLock(UID16_t i_WeaponIndex)
/// \brief		
/// \author		cmkwon
/// \date		2007-06-08 ~ 2007-06-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SATTACK_PARAMETER *CFieldMonster::APFindAttackParameterNoLock(UID16_t i_WeaponIndex)
{
	mtvectSATTACK_PARAMETER::iterator itr(m_mtvectSAttackParameterList.begin());
	for(; itr != m_mtvectSAttackParameterList.end(); itr++)
	{
		if(itr->WeaponIndex == i_WeaponIndex)
		{
			return &*itr;
		}
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldMonster::APPopAttackParameter(SATTACK_PARAMETER *o_pAttParam, UID16_t i_WeaponIndex)
/// \brief		
/// \author		cmkwon
/// \date		2007-06-08 ~ 2007-06-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldMonster::APPopAttackParameter(SATTACK_PARAMETER *o_pAttParam, UID16_t i_WeaponIndex)
{
	mt_auto_lock mtA(&m_mtvectSAttackParameterList);

	DWORD dwCurTick = timeGetTime();
	mtvectSATTACK_PARAMETER::iterator itr(m_mtvectSAttackParameterList.begin());
	while(itr != m_mtvectSAttackParameterList.end())
	{
		// start 2011-06-04 by hskim, ���Ǵ�Ƽ 3�� - ������ �� �ð� 2�� ź�� ����
		DWORD dwLifeTime = TICKGAP_WARHEAD_LIFETIME;	// 2011-06-04 by hskim, ���Ǵ�Ƽ 3�� - ������ �� �ð� 2�� ź�� ����

		if( itr->pWeaponItemInfo->Kind == ITEMKIND_FOR_MON_MAIN_ARMAMENT  )
		{
			dwLifeTime = TICKGAP_WARHEAD_LIFETIME_LONG;
		}
		// end 2011-06-04 by hskim, ���Ǵ�Ƽ 3�� - ������ �� �ð� 2�� ź�� ����		
		
		if(dwCurTick - itr->dwShotTick > dwLifeTime)		// 2011-06-04 by hskim, ���Ǵ�Ƽ 3�� - ������ �� �ð� 2�� ź�� ���� - ���� ����
		{
			itr = m_mtvectSAttackParameterList.erase(itr);
			continue;
		}
		else if(itr->WeaponIndex == i_WeaponIndex)
		{
			*o_pAttParam = *itr;

			itr = m_mtvectSAttackParameterList.erase(itr);
			return TRUE;
		}

		itr++;
	}
	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldMonster::APCalcAttckParameter(SATTACK_PARAMETER *o_pAttParam, ITEM *i_pWeaponItemInfo, UID16_t i_WeaponIndex)
/// \brief		
/// \author		cmkwon
/// \date		2007-06-08 ~ 2007-06-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldMonster::APCalcAttckParameter(SATTACK_PARAMETER *o_pAttParam, ITEM *i_pWeaponItemInfo, UID16_t i_WeaponIndex)
{
	if(IS_PRIMARY_WEAPON_MONSTER(i_pWeaponItemInfo->Kind))
	{
		o_pAttParam->dwShotTick				= 0;
	}
	else
	{// 2007-06-08 by cmkwon, 2�� ź�θ� ó��
		o_pAttParam->dwShotTick				= timeGetTime();
	}
	o_pAttParam->WeaponIndex				= i_WeaponIndex;
	o_pAttParam->pWeaponItemInfo			= i_pWeaponItemInfo;

	// 2010. 06. 14 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (���� ���ݷ� �߰� ����, PierceȮ���� ���߷� ����. ) - ���� �뷱�� ���� �߰�.
	o_pAttParam->fAttackProbability			= CAtumSJ::GetAttackProbabilityM ( MonsterInfoPtr , i_pWeaponItemInfo , &this->MonsterInfoExtend );
	//o_pAttParam->fAttackProbability			= CAtumSJ::GetAttackProbabilityM(MonsterInfoPtr, i_pWeaponItemInfo);

	// 2010. 06. 14 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (���� ���ݷ� �߰� ����, PierceȮ���� ���߷� ����. ) - ���� �뷱�� ���� �߰�.
	o_pAttParam->fPierceAttackProbability	= CAtumSJ::GetPierceAttackProbabilityM ( MonsterInfoPtr->Level , i_pWeaponItemInfo , &this->MonsterInfoExtend );
	// 2009-12-17 by cmkwon, ������ ���� ������ �ʵ�(����,ȸ��,�Ǿ,Ȯ��)�� 255�̻� ���� ���� �����ϰ� ���� - �Ʒ��� ���� ���ݹ��⸦ ���ڷ� �ִ´�
	//o_pAttParam->fPierceAttackProbability	= CAtumSJ::GetPierceAttackProbabilityM(MonsterInfoPtr->Level) + i_pWeaponItemInfo->FractionResistance;		// 2009-02-13 by cmkwon, ���� ���� ���⿡ FractionResistance �� ���� - ��������� ���� ������Ʈ �� ����
	//o_pAttParam->fPierceAttackProbability	= CAtumSJ::GetPierceAttackProbabilityM(MonsterInfoPtr->Level, i_pWeaponItemInfo);

	// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
	//o_pAttParam->fAttack					= CAtumSJ::GetAttackM(i_pWeaponItemInfo);
	//o_pAttParam->fMaxAttack					= CAtumSJ::GetMaxAttackM(i_pWeaponItemInfo);

	o_pAttParam->fAttack					= CAtumSJ::GetAttackM(i_pWeaponItemInfo , &this->MonsterInfoExtend );
	o_pAttParam->fMaxAttack					= CAtumSJ::GetMaxAttackM(i_pWeaponItemInfo , &this->MonsterInfoExtend );
	// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )

	m_CurrentAttackItemNum	= i_pWeaponItemInfo->ItemNum;					// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-05 by cmkwon, ����2�� M2M 2�� ���� ���� ó�� - 
/// \author		cmkwon
/// \date		2010-04-05 ~ 2010-04-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldMonster::APPopOverLifeTimeAttackParameterList(vectSATTACK_PARAMETER *o_pOverTimeAttParamList)
{
	mt_auto_lock mtA(&m_mtvectSAttackParameterList);
	
	DWORD dwCurTick = timeGetTime();
	mtvectSATTACK_PARAMETER::iterator itr(m_mtvectSAttackParameterList.begin());
	while(itr != m_mtvectSAttackParameterList.end())
	{
		SATTACK_PARAMETER *pAttParam = &*itr;

		// start 2011-03-28 by hskim, ���Ǵ�Ƽ 3�� - ���� ��Ƽ Ÿ���� ��� �߰�
		DWORD dwStandardShotTick = TICKGAP_WARHEAD_LIFETIME; 

		if( pAttParam->pWeaponItemInfo->Kind == ITEMKIND_FOR_MON_MAIN_ARMAMENT )		// ���Ǵ�Ƽ 3�� �� ������ ���� ó�� - ���� 2������ LifeTime�� ���
		{
			dwStandardShotTick = TICKGAP_WARHEAD_LIFETIME_LONG;
		}
		// end 2011-03-28 by hskim, ���Ǵ�Ƽ 3�� - ���� ��Ƽ Ÿ���� ��� �߰�

		if(dwCurTick - pAttParam->dwShotTick > dwStandardShotTick)		// 2011-03-28 by hskim, ���Ǵ�Ƽ 3�� - ���� ��Ƽ Ÿ���� ��� �߰�
		{
			o_pOverTimeAttParamList->push_back(*pAttParam);

			itr = m_mtvectSAttackParameterList.erase(itr);
			continue;
		}		
		itr++;
	}

	return !o_pOverTimeAttParamList->empty();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldMonster::HPRateCalcByMSWarInfoDisPlay(float i_fMonsterMaxHP, float i_fMonsterBeforeHP, float i_fMonsterAfterHP)
/// \brief		���� ���� ǥ�� ��ȹ�� - ���� ���� ǥ�ÿ� ���õ� ���� HPRate����Ͽ� ���� ����
/// \author		dhjin
/// \date		2008-03-28 ~ 2008-03-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldMonster::HPRateCalcByMSWarInfoDisPlay(float i_fMonsterMaxHP, float i_fMonsterBeforeHP, float i_fMonsterAfterHP)
{
	INT		BeforeHPRate, AfterHPRate;

	BeforeHPRate = CalcGradeByRate(i_fMonsterMaxHP, i_fMonsterBeforeHP, MS_WAR_HP_RATE);
	AfterHPRate	 = CalcGradeByRate(i_fMonsterMaxHP, i_fMonsterAfterHP, MS_WAR_HP_RATE);
	if(BeforeHPRate != AfterHPRate)
	{
		m_pCurrentFieldMapChannelMonster->m_pFieldIOCPMapChannel->m_InflWarManager.SetMSWarInfo(m_pCurrentFieldMapChannelMonster->GetMapChannelIndex().MapIndex, T_MSWARINFO_MODIFY_HPRATE, AfterHPRate);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			���� ���� ǥ�� ��ȹ�� - ���� ���� ǥ�ÿ� ���õ� ���� ���� üũ
/// \brief		BOOL CFieldMonster::MSMonsterCheckWithoutMSBossMonster()
/// \author		dhjin
/// \date		2008-03-28 ~ 2008-03-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldMonster::MSMonsterCheckWithoutMSBossMonster()
{
	if(IS_STRATEGYPOINT_MONSTER(this->MonsterInfoPtr->Belligerence)
		|| IS_TELEPORT_MONSTER(this->MonsterInfoPtr->Belligerence)
		)
	{
		return TRUE;
	}
		
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CFieldMonster::MonsterSkillApply(MSG_FN_BATTLE_ATTACK_SKILL * i_pMsg, ITEM * i_nSkill)
/// \brief		���Ǵ�Ƽ - ���� ��ų ����
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldMonster::MonsterSkillApply(MSG_FN_BATTLE_ATTACK_SKILL * i_pMsg, ITEM * i_nSkill) {
	if(m_enMonsterState != MS_PLAYING
		|| TRUE == COMPARE_BODYCON_BIT(BodyCondition, BODYCON_DEAD_MASK)
		|| CurrentHP <= 0.0f) {
		return;
	}

	switch(i_nSkill->ArrDestParameter[0])
	{
		case DES_SKILL_FULL_RECOVERY: 
			{
				// ����ȸ��
				// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
				//this->CurrentHP = this->MonsterInfoPtr->MonsterHP;
				this->CurrentHP = this->MonsterInfoExtend.fMaxHP;
				// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
				m_pCurrentFieldMapChannelMonster->FieldSendMonsterChangeInfo(this, T_FN_MONSTER_CHANGE_HP);
			}
			break;

		case DES_WARP : 
			{
				// ���� �̵�
				this->m_nTargetIndex		= i_pMsg->MapInfo.ChannelIndex;
				D3DXVECTOR3	tmVector3Pos	= A2DX(i_pMsg->PositionVector);
				
				// update block position
				this->m_pCurrentFieldMapChannelMonster->UpdateBlockPosition(this->PositionVector.x, this->PositionVector.z,
					tmVector3Pos.x, tmVector3Pos.z, this->MonsterIndex);
				
				// update position
				this->PositionVector		= A2DX(i_pMsg->PositionVector);
				D3DXVec3Normalize(&this->TargetVector, &(A2DX(i_pMsg->TargetVector)));			
			}
			break;

		case DES_SKILL_INVINCIBLE : 
			{
				// �踮�� ��ų
				this->SetBarrier(i_nSkill);
			}
			break;

		case DES_SKILL_BARRIER : 
			{
				// 2009-09-09 ~ 2010-01-15 by dhjin, ���Ǵ�Ƽ - ���� ��� ���� �踮�� ��ų(��޹��⸸�ߵ�)
				this->SetImmediateBarrier(i_nSkill);
				INIT_MSG_WITH_BUFFER(MSG_FC_BATTLE_MONSTER_BARRIER_USE, T_FC_BATTLE_MONSTER_BARRIER_USE, pSendMsg, SendBuf);
				pSendMsg->MonsterIndex		= this->MonsterIndex;
				pSendMsg->SkillItemNum		= this->m_ImmediateBarrier.SkillNum;
				m_pCurrentFieldMapChannelMonster->SendMessageToAllInChannel(SendBuf,  MSG_SIZE(MSG_FC_BATTLE_MONSTER_BARRIER_USE));
			}
			break;

		case DES_IMMEDIATE_HP_OR_DP_UP : 
			{
				this->IncreaseMonsterCurrentHP(i_nSkill->ArrParameterValue[0]);
				m_pCurrentFieldMapChannelMonster->FieldSendMonsterChangeInfo(this, T_FN_MONSTER_CHANGE_HP);
			}
			break;

	}

}

void CFieldMonster::SetBarrier(ITEM * i_pSkill) {
	m_bBarrierInfo.bBarrierSkillHave	= TRUE;
	m_bBarrierInfo.bBarrierSkillUsing	= FALSE;
	m_bBarrierInfo.HitRate				= i_pSkill->Charging;
	m_bBarrierInfo.dwLastTick			= timeGetTime();
	m_bBarrierInfo.ReAttackTick			= i_pSkill->ReAttacktime;
	m_bBarrierInfo.UseTick				= i_pSkill->Time;
	m_bBarrierInfo.SkillNum				= i_pSkill->ItemNum;
}

BOOL CFieldMonster::CheckBarrierHave() {
	if(!m_bBarrierInfo.bBarrierSkillHave) {
		// ��ų ��� �Ұ� 
		return FALSE;
	}
	return TRUE;
}

BOOL CFieldMonster::CheckBarrierUsing() {
	DWORD dwCurTick = timeGetTime();
	DWORD dwElapseTick = dwCurTick - m_bBarrierInfo.dwLastTick;
	if(m_bBarrierInfo.bBarrierSkillUsing && dwElapseTick <= m_bBarrierInfo.UseTick)	{
		// ��ų ����� 
		return TRUE;
	}
	else {
		m_bBarrierInfo.bBarrierSkillUsing = FALSE;
	}

	return FALSE;
}

BOOL CFieldMonster::BarrierUse() {
	DWORD dwCurTick = timeGetTime();	
	DWORD dwElapseTick = dwCurTick - m_bBarrierInfo.dwLastTick;
	if(dwElapseTick <= m_bBarrierInfo.ReAttackTick+m_bBarrierInfo.UseTick) {
		// ��ų ���� �ð�
		return FALSE;
	}
	

	Prob100_t nRand = RAND100();
	if(m_bBarrierInfo.HitRate < nRand) return FALSE;
	
	m_bBarrierInfo.dwLastTick	= dwCurTick;
	m_bBarrierInfo.bBarrierSkillUsing	= TRUE;
	return TRUE;
}

void CFieldMonster::SetImmediateBarrier(ITEM * i_pSkill) {
	// 2009-09-09 ~ 2010-01-15 by dhjin, ���Ǵ�Ƽ - ���� ��� ���� �踮�� ��ų(��޹��⸸�ߵ�) ����
	m_ImmediateBarrier.bBarrierSkillUsing	= TRUE;
	m_ImmediateBarrier.dwLastTick			= timeGetTime();
	m_ImmediateBarrier.UseTick				= i_pSkill->Time;
	m_ImmediateBarrier.SkillNum				= i_pSkill->ItemNum;
}

BOOL CFieldMonster::CheckImmediateBarrierUsing() {
	// 2009-09-09 ~ 2010-01-15 by dhjin, ���Ǵ�Ƽ - ���� ��� ���� �踮�� ��ų(��޹��⸸�ߵ�) ����� üũ
	if(FALSE == m_ImmediateBarrier.bBarrierSkillUsing) {
		return FALSE;
	}

	DWORD dwCurTick = timeGetTime();
	DWORD dwElapseTick = dwCurTick - m_ImmediateBarrier.dwLastTick;
	if(m_ImmediateBarrier.bBarrierSkillUsing && dwElapseTick <= m_ImmediateBarrier.UseTick)	{
		// ��ų ����� 
		return TRUE;
	}
	else {
		m_ImmediateBarrier.bBarrierSkillUsing = FALSE;
	}
	
	return FALSE;
}

CFieldMapChannel * CFieldMonster::GetCurrentMapChannelMonster() {
	return this->m_pCurrentFieldMapChannelMonster;
}

ItemNum_t CFieldMonster::GetCurrentAttackItemNum() {
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�
	return m_CurrentAttackItemNum;
}	

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldMonster::ProcessingInserttoAttackedInfoList(float i_fValue, CFieldIOCPSocket *i_AttackFISock, vectActionInfo *i_pAInfoAggroList)
/// \brief		���� �� ���� ó�� ��ƾ ���� - 
/// \author		dhjin
/// \date		2010-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldMonster::ProcessingInserttoAttackedInfoList(float i_fValue, CFieldIOCPSocket *i_AttackFISock, vectActionInfo *i_pAInfoAggroList)
{
	if(NULL == i_AttackFISock
		|| NULL == i_pAInfoAggroList)
	{
		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// ���� ���� ������ �߰��Ѵ�
	this->InserttoAttackedInfoList(&ActionInfo(i_AttackFISock->GetCharacter()->ClientIndex, timeGetTime(), i_fValue));
	
	///////////////////////////////////////////////////////////////////////////////
	// 2006-07-21 by cmkwon, Aggro �߰��ϱ�
	for(int i=0; i < i_pAInfoAggroList->size(); i++)
	{
		this->InserttoAttackedInfoList( &((*i_pAInfoAggroList)[i]) );
	}
}

// start 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
void CFieldMonster::InsertTriggerFunction(CTriggerFunction *pTriggerFunction)
{
	mt_auto_lock mtAuto(&m_mtVectTriggerFunctionPtr);
	m_mtVectTriggerFunctionPtr.push_back(pTriggerFunction);
}

BOOL CFieldMonster::IsTriggerFunction()
{
	mt_auto_lock mtAuto(&m_mtVectTriggerFunctionPtr);
	return NULL != m_mtVectTriggerFunctionPtr.size();
}
// end 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���