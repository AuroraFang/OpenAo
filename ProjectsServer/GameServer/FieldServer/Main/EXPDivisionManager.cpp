// EXPDivisionManager.cpp: implementation of the CEXPDivisionManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EXPDivisionManager.h"
#include "FieldIOCPSocket.h"
#include "FieldParty.h"
#include "FieldGlobal.h"		// 2009-12-14 by cmkwon, 2009 ũ���������̺�Ʈ(�Ѹ�� ������ �ϳ���) - 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEXPDivisionManager::CEXPDivisionManager(CFieldIOCP *i_pFIOCP, CFieldMapChannel *i_pFMChann)
{
	m_pFieldIOCP15		= i_pFIOCP;
	m_pFieldMapChannel1	= i_pFMChann;

	m_fTotalOfDamage	= 0.0f;

}

CEXPDivisionManager::~CEXPDivisionManager()
{
	listMonsterDropItem::iterator itr(m_litMonsterDropItem.begin());
	for(; itr != m_litMonsterDropItem.end(); itr++)
	{
		MONSTER_DROP_ITEM_GENERAL tmpMonsterDropItemGeneral = *itr;
		util::del(tmpMonsterDropItemGeneral.pItemGeneral);		
	}
	m_litMonsterDropItem.clear();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			float CEXPDivisionManager::GetTotalOfDamage(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-13 ~ 2006-07-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CEXPDivisionManager::GetTotalOfDamage(void)
{
	return m_fTotalOfDamage;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CEXPDivisionManager::GetvectSEXP_DIVISIONListSize(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-13 ~ 2006-07-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CEXPDivisionManager::GetvectSEXP_DIVISIONListSize(void)
{
	return m_vectSEXP_DIVISIONList.size();
};


///////////////////////////////////////////////////////////////////////////////
/// \fn			listMonsterDropItem *CEXPDivisionManager::GetlitMonsterDropItemPtr(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-13 ~ 2006-07-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
listMonsterDropItem *CEXPDivisionManager::GetlitMonsterDropItemPtr(void)
{
	return &m_litMonsterDropItem;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CEXPDivisionManager::AddEXPDivision(CFieldIOCPSocket *i_pFISock, float i_fDamage, float i_fAggro)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-13 ~ 2006-07-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CEXPDivisionManager::AddEXPDivision(CFieldIOCPSocket *i_pFISock, float i_fDamage, float i_fAggro)
{
	m_fTotalOfDamage	+= i_fDamage;

	CFieldParty *pFParty = i_pFISock->m_pFieldParty;
	SEXP_DIVISION tmEXPDivision(i_pFISock, pFParty, i_fDamage, i_fAggro);
	if(NULL == pFParty)
	{
		m_vectSEXP_DIVISIONList.push_back(tmEXPDivision);
		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2007-06-27 by cmkwon, ��Ƽ�� ����ġ �й� ��� �ý��� �߰� - �й� Ÿ�Կ� ���� ó��
	switch(pFParty->GetExpDistributeType())
	{
	case EXP_DISTRIBUTE_TYPE_DAMAGE:
		{
			tmEXPDivision.bIsApplyPartyBonus	= TRUE;		// 2007-06-27 by cmkwon, ��Ƽ�� ����ġ �й� ��� �ý��� �߰� - �׻� TRUE
			SEXP_DIVISION *pPartyExpDivision = FindEXPDivisionByFPartyPtr(pFParty);
			if(NULL == pPartyExpDivision)
			{
				m_vectSEXP_DIVISIONList.push_back(tmEXPDivision);
				return;
			}

			pPartyExpDivision->AddPartyMember(&tmEXPDivision);
		}
		break;
	case EXP_DISTRIBUTE_TYPE_EQUALITY:
		{
			BYTE byPMaxLevel = pFParty->GetMaxMemberLevelInSameMap(m_pFieldMapChannel1);
			if(MAX_LEVEL_GAP_APPLY_PARTY_BONUS >= byPMaxLevel - i_pFISock->GetCharacter()->Level)
			{// 2007-06-27 by cmkwon, ��Ƽ�� ����ġ �й� ��� �ý��� �߰� - ��Ƽ ���ʽ� ��ȿ �����

				tmEXPDivision.bIsApplyPartyBonus	= TRUE;		// 2007-06-27 by cmkwon, ��Ƽ�� ����ġ �й� ��� �ý��� �߰� -
				SEXP_DIVISION *pPartyExpDivision = FindEXPDivisionByFPartyPtr(pFParty);
				if(NULL == pPartyExpDivision)
				{
					m_vectSEXP_DIVISIONList.push_back(tmEXPDivision);
					return;
				}

				pPartyExpDivision->AddPartyMember(&tmEXPDivision);
			}
			else
			{// 2007-06-27 by cmkwon, ��Ƽ�� ����ġ �й� ��� �ý��� �߰� - ��Ƽ ���ʽ� ���� �����
			
				tmEXPDivision.bIsApplyPartyBonus	= FALSE;		// 2007-06-27 by cmkwon, ��Ƽ�� ����ġ �й� ��� �ý��� �߰� -
				m_vectSEXP_DIVISIONList.push_back(tmEXPDivision);	// 2007-06-27 by cmkwon, ��Ƽ �˻� ���� �߰��� ó��
			}
		}
	default:
		{
			return;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SEXP_DIVISION *CEXPDivisionManager::FindEXPDivisionByFPartyPtr(CFieldParty *i_pFParty)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-13 ~ 2006-07-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SEXP_DIVISION *CEXPDivisionManager::FindEXPDivisionByFPartyPtr(CFieldParty *i_pFParty)
{
	if(NULL == i_pFParty){					return NULL;}

	int nCnts = m_vectSEXP_DIVISIONList.size();
	for(int i=0; i < nCnts; i++)
	{
		// 2007-06-27 by cmkwon, ��Ƽ�� ����ġ �й� ��� �ý��� �߰� - �Ʒ��� ���� ����, ��Ƽ ����ġ ���ʽ��� ��ȿ�� �͸� ������
		//if(i_pFParty == m_vectSEXP_DIVISIONList[i].pFParty0)
		if(i_pFParty == m_vectSEXP_DIVISIONList[i].pFParty0
			&& m_vectSEXP_DIVISIONList[i].bIsApplyPartyBonus)
		{
			return &m_vectSEXP_DIVISIONList[i];
		}
	}

	return NULL;
}


struct SSort_SEXP_DIVISION_BY_DAMAGE: binary_function<SEXP_DIVISION, SEXP_DIVISION, bool>
{
	bool operator()(SEXP_DIVISION expDivision1, SEXP_DIVISION expDivision2)
	{
        return expDivision1.fSumOfDamage > expDivision2.fSumOfDamage;		// ���� ���� ������ ���� �ݴ�� ��
    };
};
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CEXPDivisionManager::SortByDamageDecrease(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-13 ~ 2006-07-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CEXPDivisionManager::SortByDamageDecrease(void)
{
	sort(m_vectSEXP_DIVISIONList.begin(), m_vectSEXP_DIVISIONList.end(), SSort_SEXP_DIVISION_BY_DAMAGE());	
}

struct SSort_SEXP_PARTYMEMBER_BY_DAMAGE: binary_function<SEXP_PARTYMEMBER, SEXP_PARTYMEMBER, bool>
{
	bool operator()(SEXP_PARTYMEMBER param1, SEXP_PARTYMEMBER param2)
	{
        return param1.fDamage > param2.fDamage;		// ���� ���� ������ ���� �ݴ�� ��
    };
};

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CEXPDivisionManager::GetSEXP_DIVISIONOfTopDamage(SEXP_DIVISION *o_pExpDivision)
/// \brief		
/// \author		cmkwon
/// \date		2006-08-25 ~ 2006-08-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CEXPDivisionManager::GetSEXP_DIVISIONOfTopDamage(SEXP_DIVISION *o_pExpDivision)
{
	if(m_vectSEXP_DIVISIONList.empty())
	{
		return FALSE;
	}

	SEXP_DIVISION *pExpDivision = &m_vectSEXP_DIVISIONList[0];
	if(pExpDivision->pFParty0
		&& 2 <= pExpDivision->nPartyMemberCnts)
	{
		sort(&(pExpDivision->ArrSEXP_PARTYMEMBER[0]), &(pExpDivision->ArrSEXP_PARTYMEMBER[pExpDivision->nPartyMemberCnts]), SSort_SEXP_PARTYMEMBER_BY_DAMAGE());
	}
	
	*o_pExpDivision	= m_vectSEXP_DIVISIONList[0];
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CEXPDivisionManager::ProcessEXPDivision(DROPITEM *o_pDropItem, MSG_FL_LOG_MONSTER_BOSS *o_pLogMonsterBoss, Experience_t i_exp, BOOL i_bBossMonster, CFieldMonster *pTargetMonster/*=NULL*/)
/// \brief		�������� ����ġ �й� ó��, ��Ƽ ���ʽ��� ��� �Ϸ�
/// \author		cmkwon
/// \date		2006-07-13 ~ 2006-07-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
void CEXPDivisionManager::ProcessEXPDivision(DROPITEM *o_pDropItem, MSG_FL_LOG_MONSTER_BOSS *o_pLogMonsterBoss, Experience_t i_exp, BOOL i_bBossMonster,CFieldMonster *pTargetMonster/*=NULL*/)
// end 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
{
	int nCountForBossMonsterLog		= 0;
	float fTempDelayRatio			= 0.0f;
	int nCnt						= m_vectSEXP_DIVISIONList.size();
	for(int i=0; i < nCnt; i++)	
	{
		SEXP_DIVISION *pExpDivision = &m_vectSEXP_DIVISIONList[i];
		CFieldIOCPSocket *pFISock	= pExpDivision->ArrSEXP_PARTYMEMBER[0].pFISock0;

		// ����ġ ���
		Experience_t fExp			= i_exp * pExpDivision->fSumOfDamage/m_fTotalOfDamage;
		if (NULL == pExpDivision->pFParty0 
			|| 1 >= pExpDivision->pFParty0->m_ListPartyMember.size()) //ȥ�� ������϶�
		{
			// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
			pFISock->ChangeExperience(fExp, TRUE, TRUE, TRUE,1,pTargetMonster);		// 2011-09-02 by hskim, ��Ʈ�� �ý��� 2��
			// end 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
			
			
			// Drop Item Delay����
			if (o_pDropItem->priorityInfoCount < SIZE_MAX_DROP_ITEM_PRIORITY_INFO)
			{
				o_pDropItem->priorityInfoArray[o_pDropItem->priorityInfoCount].CharacterUniqueNumber	= pFISock->GetCharacter()->CharacterUniqueNumber;
				o_pDropItem->priorityInfoArray[o_pDropItem->priorityInfoCount].dwDelay					= (DWORD)((float)MAX_DROPITEM_DELAY * fTempDelayRatio);
				o_pDropItem->priorityInfoCount++;
			}

			if (i_bBossMonster
				&& pFISock
				&& pFISock->IsValidCharacter()
				&& nCountForBossMonsterLog < SIZE_MAX_USERLIST_ON_BOSS_MONSTER_DEAD)
			{
				util::strncpy(o_pLogMonsterBoss->AttackUser[nCountForBossMonsterLog], pFISock->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
				nCountForBossMonsterLog++;		// 2007-01-16 by cmkwon, ���� ����
			}
		}
		else	// ��Ƽ ������϶�
		{
			// ��Ƽ�� ����ġ �й�
			CFieldIOCPSocket *ArrayPartyMember[SIZE_MAX_PARTY_MEMBER];
			util::zero(ArrayPartyMember, sizeof(CFieldIOCPSocket*)*SIZE_MAX_PARTY_MEMBER);
			
			int nPartyBonusMemberCnts	= 0;
			BYTE byPartyMaxLevel		= 1;
			int nMemberCnts				= 0;
			
			// 2007-06-26 by cmkwon, ��Ƽ ����ġ ���� ���� - �Ʒ��� ���� ���� ����� ����
			//nMemberCnts = pExpDivision->pFParty0->GetMembersInSameMap(ArrayPartyMember, m_pFieldMapChannel1, NULL, 0.0f, &byPartyMaxLevel);
			nMemberCnts = pExpDivision->pFParty0->GetMembersInSameMap(ArrayPartyMember, m_pFieldMapChannel1, FALSE, &byPartyMaxLevel);
			int j;
			for(j=0; j < nMemberCnts; j++)
			{
				CFieldIOCPSocket *p2FISock	= ArrayPartyMember[j];
				if(NULL == p2FISock
					|| FALSE == p2FISock->IsValidCharacter())
				{// 2006-07-18 by cmkwon, ��ȿ ��Ƽ�� üũ
					continue;
				}

				///////////////////////////////////////////////////////////////////////////////
				// 2006-07-18 by cmkwon, ��Ƽ���� �ο��� üũ
				// 2007-06-27 by cmkwon, ��Ƽ�� ����ġ �й� ��� �ý��� �߰� - �Ʒ��� ���� ����
				//if(byPartyMaxLevel-7 <= p2FISock->GetCharacter()->Level)
				if(MAX_LEVEL_GAP_APPLY_PARTY_BONUS >= byPartyMaxLevel-p2FISock->GetCharacter()->Level)
				{
					nPartyBonusMemberCnts++;
				}

				///////////////////////////////////////////////////////////////////////////////
				// Drop Item Delay���� - Delay�� ��� ��Ƽ������ �����ϰ� ����
				if (o_pDropItem->priorityInfoCount < SIZE_MAX_DROP_ITEM_PRIORITY_INFO)
				{
					o_pDropItem->priorityInfoArray[o_pDropItem->priorityInfoCount].CharacterUniqueNumber	= p2FISock->GetCharacter()->CharacterUniqueNumber;
					o_pDropItem->priorityInfoArray[o_pDropItem->priorityInfoCount].dwDelay					= (DWORD)((float)MAX_DROPITEM_DELAY * fTempDelayRatio);
					o_pDropItem->priorityInfoCount++;
				}
			}

			///////////////////////////////////////////////////////////////////////////////
			// 2007-06-27 by cmkwon, ��Ƽ�� ����ġ �й� ��� �ý��� �߰� - ����ġ �й� Ÿ�Ժ��� ����ġ �й� ó��
			if(pExpDivision->bIsApplyPartyBonus) //��Ƽ�� ���ʽ� ����ġ�� ��� �� ���
			{
				pExpDivision->fPartyBonus	= CAtumSJ::GetPartyBonusExpWeight(nPartyBonusMemberCnts);
				// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
#else
				fExp						*= pExpDivision->fPartyBonus;
#endif //S_BONUSEXPSYSTEM_RENEWAL
				// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
				switch(pExpDivision->pFParty0->GetExpDistributeType())
				{
				case EXP_DISTRIBUTE_TYPE_DAMAGE:
					{
						float fPartySumOfDamage		= pExpDivision->fSumOfDamage + pExpDivision->fSumOfAggro;		// 2006-07-21 by cmkwon, �������հ� ��׷��� ��
						// Drop Item ����
						for (j = 0; j < pExpDivision->nPartyMemberCnts; j++)
						{
							CFieldIOCPSocket *p2FISock = pExpDivision->ArrSEXP_PARTYMEMBER[j].pFISock0;

							if(NULL == p2FISock
								|| FALSE == p2FISock->IsValidCharacter())
							{// 2006-07-18 by cmkwon, ��ȿ ��Ƽ�� üũ
								continue;
							}

							///////////////////////////////////////////////////////////////////////////////
							// 2006-07-18 by cmkwon, ����ġ ����
							// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
							p2FISock->ChangeExperience(fExp * (pExpDivision->ArrSEXP_PARTYMEMBER[j].GetPartyDamage()/fPartySumOfDamage), TRUE, TRUE, TRUE, nMemberCnts, pTargetMonster);		// 2011-09-02 by hskim, ��Ʈ�� �ý��� 2��
							// end 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������

							///////////////////////////////////////////////////////////////////////////////
							// 2006-07-18 by cmkwon, �������� ���� �α�
							if (i_bBossMonster
								&& nCountForBossMonsterLog < SIZE_MAX_USERLIST_ON_BOSS_MONSTER_DEAD)
							{
								util::strncpy(o_pLogMonsterBoss->AttackUser[nCountForBossMonsterLog], p2FISock->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
								nCountForBossMonsterLog++;		// 2007-01-16 by cmkwon, ���� ����
							}
						}
					}
					break;
				case EXP_DISTRIBUTE_TYPE_EQUALITY:
					{
						for(int j=0; j < nMemberCnts; j++)
						{
							CFieldIOCPSocket *p2FISock = ArrayPartyMember[j];
							if(NULL == p2FISock
								|| FALSE == p2FISock->IsValidCharacter()
								|| MAX_LEVEL_GAP_APPLY_PARTY_BONUS < byPartyMaxLevel-p2FISock->GetCharacter()->Level)
							{// 2006-07-18 by cmkwon, ��ȿ ��Ƽ�� üũ
								continue;
							}

							///////////////////////////////////////////////////////////////////////////////
							// 2006-07-18 by cmkwon, ����ġ ����
							// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
							p2FISock->ChangeExperience(fExp / max(nPartyBonusMemberCnts,1), TRUE, TRUE, TRUE, nMemberCnts, pTargetMonster);		// 2011-09-02 by hskim, ��Ʈ�� �ý��� 2��
							// end 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������

							///////////////////////////////////////////////////////////////////////////////
							// 2006-07-18 by cmkwon, �������� ���� �α�
							if (i_bBossMonster
								&& nCountForBossMonsterLog < SIZE_MAX_USERLIST_ON_BOSS_MONSTER_DEAD)
							{
								util::strncpy(o_pLogMonsterBoss->AttackUser[nCountForBossMonsterLog], p2FISock->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
								nCountForBossMonsterLog++;		// 2007-01-16 by cmkwon, ���� ����
							}
						}
					}
					break;
				}
			}
			else
			{
				// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
				pFISock->ChangeExperience(fExp, TRUE, TRUE, TRUE, 1, pTargetMonster);		// 2011-09-02 by hskim, ��Ʈ�� �ý��� 2��
				// end 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
				///////////////////////////////////////////////////////////////////////////////
				// 2006-07-18 by cmkwon, �������� ���� �α�
				if (i_bBossMonster
					&& nCountForBossMonsterLog < SIZE_MAX_USERLIST_ON_BOSS_MONSTER_DEAD)
				{
					util::strncpy(o_pLogMonsterBoss->AttackUser[nCountForBossMonsterLog], pFISock->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
					nCountForBossMonsterLog++;		// 2007-01-16 by cmkwon, ���� ����
				}
			}
		}

		fTempDelayRatio += (pExpDivision->fSumOfDamage/m_fTotalOfDamage);		// Delay ������ ���� �ջ�
	}// END - for(int i=0; i < nCnt; i++)
}


struct DROP_ITEM_PRIORITY
{
	ITEM_GENERAL* pDropItemGeneral;
	int iPriority;
};

struct SORT_DROP_ITEM_PRIORITY
{
	bool operator()(DROP_ITEM_PRIORITY op1, DROP_ITEM_PRIORITY op2)
	{
		return op1.iPriority < op2.iPriority;
	}
};	

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CEXPDivisionManager::ProcessPickUpDropItems(string *o_pStrDropItemlist, DROPITEM *i_pDropItem, CFieldMonster *i_pDeadFMonster, CFieldIOCPSocket *i_pFISockTopDamage)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-13 ~ 2006-07-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CEXPDivisionManager::ProcessPickUpDropItems(string *o_pStrDropItemlist, DROPITEM *i_pDropItem, CFieldMonster *i_pDeadFMonster, CFieldIOCPSocket *i_pFISockTopDamage)
{
	if(1 > m_litMonsterDropItem.size())
	{
		return;
	}

	// 2010-04-09 by cmkwon, ����2�� �߰� ����(�ܰ躰 ���� �߰�) - 
	D3DXVECTOR3 vec3MonPos = i_pDeadFMonster->PositionVector;
	if(0.0f > vec3MonPos.x || 0.0f > vec3MonPos.z)
	{		
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] DropItem error !! MonsterIdx(%d) Pos(%f, %f, %f) \r\n", i_pDeadFMonster->MonsterIndex, i_pDeadFMonster->PositionVector.x, i_pDeadFMonster->PositionVector.y, i_pDeadFMonster->PositionVector.z);
		return;
	}

	MONSTER_INFO *pMonsterInfo = i_pDeadFMonster->MonsterInfoPtr;
	if(NULL == pMonsterInfo)
	{
		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2009-12-14 by cmkwon, 2009 ũ���������̺�Ʈ(�Ѹ�� ������ �ϳ���) - ũ������������(2060600), ������ AutoPickup
	BOOL bAutoPickUpOfXMasDrop	= FALSE;
	vector<DROP_ITEM_PRIORITY> v_DropItemGeneral;
	// 2010-06-21 by jskim, 2010 �̺�Ʈ(�Ѹ�� ������ �ϳ���) - ������(2098200), ������ AutoPickup
	BOOL bAutoPickUpOfEventDrop	= FALSE;
	if(2060600 == pMonsterInfo->MonsterUnitKind)
	{
		bAutoPickUpOfXMasDrop	= TRUE;
	}
	// 2010-06-21 by jskim, 2010 �̺�Ʈ(�Ѹ�� ������ �ϳ���) - ������(2098200), ������ AutoPickup
	if(2098200 == pMonsterInfo->MonsterUnitKind)
	{
		bAutoPickUpOfEventDrop	= TRUE;
	}

	mtlistMonsterDropItem::iterator itrDropItem = m_litMonsterDropItem.begin();
	for(; itrDropItem != m_litMonsterDropItem.end(); itrDropItem++)
	{
		MONSTER_DROP_ITEM_GENERAL tmpMonsterDropItemGeneral = *itrDropItem;
		ITEM_GENERAL *pDropItemGeneral		= tmpMonsterDropItemGeneral.pItemGeneral;
		ITEM *pDropItemInfo					= tmpMonsterDropItemGeneral.pItemGeneral->ItemInfo;
		
		if (COMPARE_MPOPTION_BIT(pMonsterInfo->MPOption, MPOPTION_BIT_BOSS_MONSTER))
		{
			*o_pStrDropItemlist += pDropItemInfo->ItemName;
			*o_pStrDropItemlist += "|";
		}

		BYTE ItemUpdateType = IUT_DROP_ITEM;
		if (COMPARE_BIT_FLAG(pDropItemInfo->ItemAttribute, ITEM_ATTR_AUTO_PICK_UP)
			//|| bAutoPickUpOfXMasDrop)
			|| bAutoPickUpOfXMasDrop || bAutoPickUpOfEventDrop)		// 2009-12-14 by cmkwon, 2009 ũ���������̺�Ʈ(�Ѹ�� ������ �ϳ���) - // 2010-06-21 by jskim, 2010 �̺�Ʈ(�Ѹ�� ������ �ϳ���) - ������(2098200)
		{
			///////////////////////////////////////////////////////////////
			// auto pickup�� ���
			///////////////////////////////////////////////////////////////
			if (NULL == i_pFISockTopDamage
				|| FALSE == i_pFISockTopDamage->IsValidCharacter())
// 2006-11-16 by cmkwon, �ý��� �������� �ʿ� ����
//				|| FALSE == i_pFISockTopDamage->IsValidGameUser())		// 2006-06-14 by cmkwon, ��Ʈ�� ������ üũ - �ڵ� ���� ������
			{
				util::del(pDropItemGeneral);		// just delete item
				continue;
			}

			BOOL bPickUP = FALSE;
			if (COMPARE_BIT_FLAG(pDropItemInfo->ItemAttribute, ITEM_ATTR_QUEST_ITEM) || pDropItemInfo->Kind == ITEMKIND_QUEST)
			{
				///////////////////////////////////////////////////////
				// quest item�� ���
				bPickUP = TRUE;
				ItemUpdateType = IUT_QUEST;
			}
			else
			{
				///////////////////////////////////////////////////////
				// quest item�� �ƴ� ���
				if (pDropItemGeneral->ItemNum == MONEY_ITEM_NUMBER
					&& FALSE == CAtumSJ::CanPickUpDropMoney(i_pFISockTopDamage->m_character.Level, pMonsterInfo))
				{
					bPickUP = FALSE;
				}
				else
				{
					bPickUP = TRUE;
					ItemUpdateType = IUT_DROP_ITEM;
				}
			}

			if (FALSE == bPickUP)
			{// 2006-03-17 by cmkwon, 
				util::del(pDropItemGeneral);		// just delete item
				continue;
			}

			pDropItemGeneral->AccountUniqueNumber	= i_pFISockTopDamage->m_character.AccountUniqueNumber;
			pDropItemGeneral->Possess				= i_pFISockTopDamage->m_character.CharacterUniqueNumber;
// 2009-08-25 by cmkwon, ������� �ʴ� �ʵ� ����(td_Store.ScarcityNumber) - 
//			pDropItemGeneral->ScarcityNumber		= 0;
			pDropItemGeneral->Wear					= WEAR_NOT_ATTACHED;
			pDropItemGeneral->ItemWindowIndex		= POS_INVALID_POSITION;	// ���������� ������ â�� ù slot�� �־�ΰ�, ������ client���� �Ѵ�.
			pDropItemGeneral->NumOfEnchants			= 0;

			if (pDropItemInfo->ItemNum == MONEY_ITEM_NUMBER)
			{
				CFieldParty *pFirstAttackCharacterFParty = i_pFISockTopDamage->m_pFieldParty;
				if(NULL == pFirstAttackCharacterFParty)
				{
					i_pFISockTopDamage->m_ItemManager.InsertItemBaseByItemNum(MONEY_ITEM_NUMBER, pDropItemGeneral->CurrentCount, ItemUpdateType);
				}
				else
				{
					SEXP_DIVISION *pExpDivision = this->FindEXPDivisionByFPartyPtr(pFirstAttackCharacterFParty);
					if(NULL == pExpDivision
						|| 1 >= pExpDivision->nPartyMemberCnts
						|| FALSE == pExpDivision->bIsApplyPartyBonus)	// 2007-06-27 by cmkwon, ��Ƽ�� ����ġ �й� ��� �ý��� �߰� - ��Ƽ ���ʽ� ���� ���� üũ �߰�
					{
						i_pFISockTopDamage->m_ItemManager.InsertItemBaseByItemNum(MONEY_ITEM_NUMBER, pDropItemGeneral->CurrentCount, ItemUpdateType);
					}
					else
					{
						///////////////////////////////////////////////////////////////////////////////					
						// 2007-06-27 by cmkwon, ��Ƽ�� ����ġ �й� ��� �ý��� �߰� - ����ġ �й� Ÿ�Ժ��� SPI �й� ó��
						switch(pFirstAttackCharacterFParty->GetExpDistributeType())
						{
						case EXP_DISTRIBUTE_TYPE_DAMAGE:
							{
								for (int i = 0; i < pExpDivision->nPartyMemberCnts; i++)
								{
									if(pExpDivision->ArrSEXP_PARTYMEMBER[i].pFISock0
										&& pExpDivision->ArrSEXP_PARTYMEMBER[i].pFISock0->IsValidCharacter())
									{
										pExpDivision->ArrSEXP_PARTYMEMBER[i].pFISock0->m_ItemManager.InsertItemBaseByItemNum(MONEY_ITEM_NUMBER, pDropItemGeneral->CurrentCount * pExpDivision->ArrSEXP_PARTYMEMBER[i].fDamage/pExpDivision->fSumOfDamage, ItemUpdateType);
									}
								}
							}
							break;
						case EXP_DISTRIBUTE_TYPE_EQUALITY:
							{
								// 2007-06-27 by cmkwon, ��ȿ�� ��Ƽ�� ��ο��� �յ��ϰ� �й�
								CFieldIOCPSocket *arrFPMem[SIZE_MAX_PARTY_MEMBER] = {NULL,};
								int nPFMemCnts = pFirstAttackCharacterFParty->GetMembersInSameMap(arrFPMem, m_pFieldMapChannel1, TRUE);
								for(int i=0; i < nPFMemCnts; i++)
								{
									CFieldIOCPSocket *pFISoc = arrFPMem[i];
									if(NULL == pFISoc
										|| FALSE == pFISoc->IsValidCharacter())
									{
										continue;
									}
									pFISoc->m_ItemManager.InsertItemBaseByItemNum(MONEY_ITEM_NUMBER, pDropItemGeneral->CurrentCount / nPFMemCnts, ItemUpdateType);
								}
							}
							break;
 						}
					}
				}
				util::del(pDropItemGeneral);
				continue;
			}

			if (tmpMonsterDropItemGeneral.QuestIndex == 0)
			{// ����Ʈ �������� �ƴ� �Ϲ� �������� ���

				///////////////////////////////////////////////////////////////////////////////
				// 2009-12-14 by cmkwon, 2009 ũ���������̺�Ʈ(�Ѹ�� ������ �ϳ���) - �Ѹ�� �ϳ��� �������� �����ϰ� ó��
				if(bAutoPickUpOfXMasDrop)
				{
					//g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] 091214 %s Item(%d:%d)\r\n"
					//	, GetCharacterString(i_pFISockTopDamage->GetCharacter(), string()), pDropItemGeneral->ItemNum, pDropItemGeneral->CurrentCount);
					
					if(FALSE == i_pFISockTopDamage->m_ItemManager.InsertItemGeneralByPointer(pDropItemGeneral, ItemUpdateType))
					{
						util::del(pDropItemGeneral);
					}
					
					i_pFISockTopDamage = this->GetNextUser(i_pFISockTopDamage, FALSE);
					continue;
				}
				if(bAutoPickUpOfEventDrop)	 // 2010-06-21 by jskim, 2010 �̺�Ʈ(�Ѹ�� ������ �ϳ���) - ������(2098200)
				{
					DROP_ITEM_PRIORITY TempPriority;
					
					switch(pDropItemGeneral->ItemNum)
					{
					case 7029820 :
						{
							TempPriority.iPriority = 1;
						}
						break;
					case 7029830 :
						{
							TempPriority.iPriority = 2;
						}
						break;
					case 7029910 :
						{
							TempPriority.iPriority = 3;
						}
						break;
					case 7029920 :
						{
							TempPriority.iPriority = 4;
						}
						break;
					case 7029840 :
						{
							TempPriority.iPriority = 5;
						}
						break;
					case 7029930 :
						{
							TempPriority.iPriority = 6;
						}
						break;
					case 7029940 :
						{
							TempPriority.iPriority = 7;
						}
						break;
					case 7029850 :
						{
							TempPriority.iPriority = 8;
						}
						break;
					case 7029950 :
						{
							TempPriority.iPriority = 9;
						}
						break;
					case 7029960 :
						{
							TempPriority.iPriority = 10;
						}
						break;
					default:
						continue;
					}

					TempPriority.pDropItemGeneral = pDropItemGeneral;
					v_DropItemGeneral.push_back(TempPriority);
					continue;
				}


				//////////////////////////////////////////////////////////////////////////
				// 2008-06-03 by dhjin, EP3 ��� ���� -	������ �й� ��� �߰��� �ذ� ���� ����
				CFieldParty *pFirstAttackCharacterFParty = i_pFISockTopDamage->m_pFieldParty;

				// 2009-01-21 by dhjin, ������ �ü��� �����Ƿ� �����Ѵ�.
				if(NULL == pFirstAttackCharacterFParty)
				{
					if(FALSE == i_pFISockTopDamage->m_ItemManager.InsertItemGeneralByPointer(pDropItemGeneral, ItemUpdateType))
					{
						util::del(pDropItemGeneral);
					}
					continue;
				}

				switch(pFirstAttackCharacterFParty->GetItemDistributeType())
				{
				case ITEM_DISTRIBUTE_TYPE_DAMAGE:
					{
						// 2006-09-27 by cmkwon, ����Ʈ �������� �κ��� �������� ���� �� �ֵ��� ����
						if(FALSE == i_pFISockTopDamage->m_ItemManager.InsertItemGeneralByPointer(pDropItemGeneral, ItemUpdateType))
						{
							util::del(pDropItemGeneral);
						}
					}
					break;
				case ITEM_DISTRIBUTE_TYPE_ORDER:
					{
						BYTE tmPivotPartyMember = 0;
						CFieldIOCPSocket *pFISoc = pFirstAttackCharacterFParty->GetItemByOrderDistributeType(m_pFieldMapChannel1, &tmPivotPartyMember);
						if(NULL == pFISoc
							|| FALSE == pFISoc->IsValidCharacter())
						{
							util::del(pDropItemGeneral);
							continue;
						}
						// 2006-09-27 by cmkwon, ����Ʈ �������� �κ��� �������� ���� �� �ֵ��� ����
						if(FALSE == pFISoc->m_ItemManager.InsertItemGeneralByPointer(pDropItemGeneral, ItemUpdateType))
						{
							util::del(pDropItemGeneral);
						}						
		
						pFirstAttackCharacterFParty->IncreasePivotPartyMember(tmPivotPartyMember);
					}
					break;
				case ITEM_DISTRIBUTE_TYPE_RANDOM:
					{
						CFieldIOCPSocket *pFISoc = pFirstAttackCharacterFParty->GetItemByRandomDistributeType(m_pFieldMapChannel1);
						if(NULL == pFISoc
							|| FALSE == pFISoc->IsValidCharacter())
						{
							util::del(pDropItemGeneral);
							continue;
						}
						// 2006-09-27 by cmkwon, ����Ʈ �������� �κ��� �������� ���� �� �ֵ��� ����
						if(FALSE == pFISoc->m_ItemManager.InsertItemGeneralByPointer(pDropItemGeneral, ItemUpdateType))
						{
							util::del(pDropItemGeneral);
						}
					}
					break;		
				}				
				continue;
			}
			
			///////////////////////////////////////////////////////////////////////////////
			// ����Ʈ �������� ���
			CQuest *pQuestInfo	= m_pFieldIOCP15->m_mapQuestInfoFromScript.findEZ(tmpMonsterDropItemGeneral.QuestIndex);
			if(NULL == pQuestInfo)
			{
				util::del(pDropItemGeneral);
				continue;
			}

			if (pQuestInfo->DropItemToAllAttackCharacter == FALSE)
			{// �ְ� ������ ĳ�����Ը� �ִ� ���
				
				if (FALSE == i_pFISockTopDamage->IsQuestStateInProgressByQuestIndex(tmpMonsterDropItemGeneral.QuestIndex))
				{
					util::del(pDropItemGeneral);
					continue;
				}

				if(FALSE == i_pFISockTopDamage->m_ItemManager.InsertItemGeneralByPointer(pDropItemGeneral, ItemUpdateType, TRUE))
				{
					util::del(pDropItemGeneral);
				}
				continue;				
			}

			///////////////////////////////////////////////////////////////////////////////
			// ��Ƽ�� ��ο��� �� �ִ� ����Ʈ ������
			// �ϴ� �������� �Ҵ� �� ����� ����
			INT nQuestItemNum		= pDropItemGeneral->ItemNum;
			INT nQuestItemCount		= pDropItemGeneral->CurrentCount;
			ITEM *pQuestItemInfo	= pDropItemInfo;
			util::del(pDropItemGeneral);

			// ������ �ֱ�
			for(int i=0; i < m_vectSEXP_DIVISIONList.size(); i++)
			{
				SEXP_DIVISION *pExpDivision = &m_vectSEXP_DIVISIONList[i];
				CFieldIOCPSocket *pFISock	= pExpDivision->ArrSEXP_PARTYMEMBER[0].pFISock0;
				
				// 2008-01-18 by cmkwon, S_F: ����Ʈ ������ ���� ��� ���� ���� - ����Ʈ ���ʽ��ϰ�� ��� ���� �κ���, bIsApplyPartyBonus �� üũ �ʿ� ����
				//if(NULL == pExpDivision->pFParty0
				//	|| FALSE == pExpDivision->bIsApplyPartyBonus)		// 2007-06-27 by cmkwon, ��Ƽ�� ����ġ �й� ��� �ý��� �߰� - ��Ƽ ���ʽ� ���� ���� üũ �߰�
				if(NULL == pExpDivision->pFParty0)
				{
					if(pFISock
						&& pFISock->IsValidCharacter())
					{
						if(FALSE == pQuestInfo->IsPartyQuest())
						{// 2006-03-24 by cmkwon, ��Ƽ����Ʈ�� �ƴҶ��� �������� �߰��ȴ�.

							if (pFISock->IsQuestStateInProgressByQuestIndex(tmpMonsterDropItemGeneral.QuestIndex))
							{
								pFISock->m_ItemManager.InsertItemBaseByItemNum(nQuestItemNum, nQuestItemCount, IUT_QUEST, TRUE);
							}
						}
					}
				}
				else
				{
					CFieldIOCPSocket *ArrayPartyMember[SIZE_MAX_PARTY_MEMBER];
					util::zero(ArrayPartyMember, sizeof(ArrayPartyMember[0])*SIZE_MAX_PARTY_MEMBER);
					// 2007-06-26 by cmkwon, ��Ƽ ����ġ ���� ���� - ��� �ִ� ĳ���͸� ó��
					int nMemCount = pExpDivision->pFParty0->GetMembersInSameMap(ArrayPartyMember, pFISock->m_pCurrentFieldMapChannel, TRUE);
					for(int j=0; j < nMemCount; j++)
					{
						if (ArrayPartyMember[j]->IsQuestStateInProgressByQuestIndex(tmpMonsterDropItemGeneral.QuestIndex))
						{
							ArrayPartyMember[j]->m_ItemManager.InsertItemBaseByItemNum(nQuestItemNum, nQuestItemCount, IUT_QUEST, TRUE);
						}
					}
				}
			}// end_for(int i=0; i < m_vectSEXP_DIVISIONList.size(); i++)
		}// end_if (COMPARE_BIT_FLAG(pDropItemInfo->ItemAttribute, ITEM_ATTR_AUTO_PICK_UP))
		else
		{
			///////////////////////////////////////////////////////////////
			// auto pickup�� �ƴ� ���
			///////////////////////////////////////////////////////////////
			if (NULL == i_pFISockTopDamage
				|| FALSE == i_pFISockTopDamage->IsValidCharacter()
				|| FALSE == CAtumSJ::CanPickUpDropItem(i_pFISockTopDamage->m_character.Level, pMonsterInfo))
			{
				util::del(pDropItemGeneral);		// just delete item
			}
			else
			{
				// Rafix ���� �˻�
				if (FALSE == CAtumSJ::CanPickUpDropRare(i_pFISockTopDamage->m_character.Level, pMonsterInfo))
				{
					pDropItemGeneral->PrefixCodeNum = 0;
					pDropItemGeneral->SuffixCodeNum = 0;
				}

// 2010-04-09 by cmkwon, ����2�� �߰� ����(�ܰ躰 ���� �߰�) - 
// 				if (i_pDeadFMonster->PositionVector.x == 0.0f)
// 				{	// check: monster�� ��ǥ�� (0,0,0)�� ��� ������ ã�ƾ� ��, �켱 ����...
// 
// 					//DbgOut("Monster's Position is 0: (%d)\n", i_pDeadFMonster->MonsterIndex);
// 					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] DropItem error !! MonsterIdx(%d) Pos(%f, %f, %f) \r\n", i_pDeadFMonster->MonsterIndex, i_pDeadFMonster->PositionVector.x, i_pDeadFMonster->PositionVector.y, i_pDeadFMonster->PositionVector.z);
// 					break;
// 				}
 				D3DXVECTOR3 pos;			

				CMapBlock *pMapBlock = NULL;
				m_pFieldMapChannel1->m_pFieldMapProject->GetRandomPositionInRadius(&vec3MonPos, &pos, DROP_ITEM_RADIUS);
				pMapBlock						= m_pFieldMapChannel1->GetBlock(pos.x, pos.z);
				i_pDropItem->Position			= pos;
				i_pDropItem->dwStartTick		= m_pFieldIOCP15->GetCurrentServerTimeInMilliSeconds();
				i_pDropItem->pItem				= pDropItemGeneral;
				i_pDropItem->FirstCharacterUID1	= i_pFISockTopDamage->m_character.CharacterUniqueNumber;
				pMapBlock->InsertDropItem(pDropItemGeneral, i_pDropItem);

				INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_SHOW_ITEM, T_FC_ITEM_SHOW_ITEM, pMsgShowItem, pMsgShowItemBuf);
				pMsgShowItem->ItemFieldIndex	= (UINT)i_pDropItem->pItem;
				pMsgShowItem->ItemNum			= i_pDropItem->pItem->ItemNum;
				pMsgShowItem->FirstCharacterUID	= i_pDropItem->FirstCharacterUID1;
				pMsgShowItem->Amount			= i_pDropItem->pItem->CurrentCount;
				pMsgShowItem->Position			= i_pDropItem->Position;
				pMsgShowItem->DropItemType		= DROP_ITEM_TYPE_PICKUP;

// 2007-06-18 by cmkwon, �׽�Ʈ				DBGOUT("ShowItem(%d): %#x[%d][%d]\n", i_pDropItem->pItem->ItemNum, pMsgShowItem->ItemFieldIndex, pMapBlock->m_x, pMapBlock->m_z);

				if(IS_TUTORIAL_MAP_INDEX(m_pFieldMapChannel1->GetMapChannelIndex().MapIndex))
				{
					mt_auto_lock mtCT(&i_pDeadFMonster->m_mtvectClientIdxForTargetType);
					m_pFieldIOCP15->SendInRangeClientIndexVectorAroundPosition(pos, &i_pDeadFMonster->m_mtvectClientIdxForTargetType
						, pMsgShowItemBuf, MSG_SIZE(MSG_FC_ITEM_SHOW_ITEM), m_pFieldMapChannel1, m_pFieldMapChannel1->GetUserVisibleDiameterW());
				}
				else
				{
					m_pFieldIOCP15->SendInRangeMessageAroundPosition(pos, pMsgShowItemBuf, MSG_SIZE(MSG_FC_ITEM_SHOW_ITEM), m_pFieldMapChannel1);
				}
			}
		} // END - NOT AUTO PICKUP
	} // END - for(; itrDropItem != m_litMonsterDropItem.end(); itrDropItem++)
	// 2010-06-21 by jskim, 2010 �̺�Ʈ(�Ѹ�� ������ �ϳ���) - ������(2098200)
	if(bAutoPickUpOfEventDrop && !v_DropItemGeneral.empty())
	{
		sort( v_DropItemGeneral.begin(), v_DropItemGeneral.end(), SORT_DROP_ITEM_PRIORITY() );
		for(int i=0; i < v_DropItemGeneral.size(); i++)
		{
			if (NULL != i_pFISockTopDamage)
			{
				v_DropItemGeneral[i].pDropItemGeneral->AccountUniqueNumber	= i_pFISockTopDamage->m_character.AccountUniqueNumber;
				v_DropItemGeneral[i].pDropItemGeneral->Possess				= i_pFISockTopDamage->m_character.CharacterUniqueNumber;
				if(FALSE == i_pFISockTopDamage->m_ItemManager.InsertItemGeneralByPointer(v_DropItemGeneral[i].pDropItemGeneral, IUT_DROP_ITEM))
				{
					util::del(v_DropItemGeneral[i].pDropItemGeneral);
				}
				i_pFISockTopDamage = this->GetNextUser(i_pFISockTopDamage, FALSE);
			}
		}		
	}
	m_litMonsterDropItem.clear();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CEXPDivisionManager::ProcessEXPDivisionInfinityMap(DROPITEM *o_pDropItem, MSG_FL_LOG_MONSTER_BOSS *o_pLogMonsterBoss, Experience_t i_exp, BOOL i_bBossMonster, CFieldMonster *pTargetMonster/*=NULL*/)
/// \brief		���Ǵ�Ƽ - ���� �ʿ��� ��Ƽ ������� �� ��� ���� ����ġ �յ�й�.
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
void CEXPDivisionManager::ProcessEXPDivisionInfinityMap(DROPITEM *o_pDropItem, MSG_FL_LOG_MONSTER_BOSS *o_pLogMonsterBoss, Experience_t i_exp, BOOL i_bBossMonster, CFieldMonster *pTargetMonster /*=NULL*/)
// end 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
{
	if(NULL == o_pDropItem
		|| NULL == o_pLogMonsterBoss) {
		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
		return;
	}

	int nCountForBossMonsterLog		= 0;

	// ����ġ ���
	if(0 >= m_pFieldMapChannel1->m_mapCharacterUniqueNumberMapProject.size()) {
		return;
	}
	Experience_t fExp			= i_exp / m_pFieldMapChannel1->m_mapCharacterUniqueNumberMapProject.size();

	mt_auto_lock mta(&m_pFieldMapChannel1->m_mapCharacterUniqueNumberMapProject);
	mtmapUID2FieldIOCPSocket::iterator itrChar = m_pFieldMapChannel1->m_mapCharacterUniqueNumberMapProject.begin();
	for (; itrChar != m_pFieldMapChannel1->m_mapCharacterUniqueNumberMapProject.end(); itrChar++) {
		
		CFieldIOCPSocket *pFISock = itrChar->second;

		// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
		pFISock->ChangeExperience(fExp, TRUE, TRUE, TRUE, 1, pTargetMonster);		// 2011-09-02 by hskim, ��Ʈ�� �ý��� 2��
		// end 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
		
		// Drop Item Delay����
		if (o_pDropItem->priorityInfoCount < SIZE_MAX_DROP_ITEM_PRIORITY_INFO)
		{
			o_pDropItem->priorityInfoArray[o_pDropItem->priorityInfoCount].CharacterUniqueNumber	= pFISock->GetCharacter()->CharacterUniqueNumber;
			o_pDropItem->priorityInfoArray[o_pDropItem->priorityInfoCount].dwDelay					= 0;
			o_pDropItem->priorityInfoCount++;
		}

		if (i_bBossMonster
			&& pFISock
			&& pFISock->IsValidCharacter()
			&& nCountForBossMonsterLog < SIZE_MAX_USERLIST_ON_BOSS_MONSTER_DEAD)
		{
			util::strncpy(o_pLogMonsterBoss->AttackUser[nCountForBossMonsterLog], pFISock->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
			nCountForBossMonsterLog++;		// 2007-01-16 by cmkwon, ���� ����
		}
	}// END - for(int i=0; i < nCnt; i++)
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CEXPDivisionManager::ProcessEXPDivisionInfinityMap(string *o_pStrDropItemlist, DROPITEM *i_pDropItem, CFieldMonster *i_pDeadFMonster, mtTenderItemList * o_pTenderItemList)
/// \brief		���Ǵ�Ƽ - ���� �ʿ��� ������ ó��, Tender�� InfinityBase���� ó���Ѵ�.
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CEXPDivisionManager::ProcessPickUpDropItemsInfinityMap(string *o_pStrDropItemlist, DROPITEM *i_pDropItem, CFieldMonster *i_pDeadFMonster, CInfinityManager *i_pInfiMan)
{
	if(NULL == o_pStrDropItemlist
		|| NULL == i_pDropItem
		|| NULL == i_pDeadFMonster) {
		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
		return;
	}

	if(1 > m_litMonsterDropItem.size())
	{
		return;
	}

	// 2010-04-09 by cmkwon, ����2�� �߰� ����(�ܰ躰 ���� �߰�) - 
	D3DXVECTOR3 vec3MonPos = i_pDeadFMonster->PositionVector;
	if(0.0f > vec3MonPos.x || 0.0f > vec3MonPos.z)
	{		
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] DropItem error !! MonsterIdx(%d) Pos(%f, %f, %f) \r\n", i_pDeadFMonster->MonsterIndex, i_pDeadFMonster->PositionVector.x, i_pDeadFMonster->PositionVector.y, i_pDeadFMonster->PositionVector.z);
		return;
	}

	MONSTER_INFO *pMonsterInfo = i_pDeadFMonster->MonsterInfoPtr;
	if(NULL == pMonsterInfo)
	{
		return;
	}

	mtlistMonsterDropItem::iterator itrDropItem = m_litMonsterDropItem.begin();
	for(; itrDropItem != m_litMonsterDropItem.end(); itrDropItem++)
	{
		MONSTER_DROP_ITEM_GENERAL tmpMonsterDropItemGeneral = *itrDropItem;
		ITEM_GENERAL *pDropItemGeneral		= tmpMonsterDropItemGeneral.pItemGeneral;
		ITEM *pDropItemInfo					= tmpMonsterDropItemGeneral.pItemGeneral->ItemInfo;
		
		if (COMPARE_MPOPTION_BIT(pMonsterInfo->MPOption, MPOPTION_BIT_BOSS_MONSTER))
		{
			*o_pStrDropItemlist += pDropItemInfo->ItemName;
			*o_pStrDropItemlist += "|";
		}

		BYTE ItemUpdateType = IUT_DROP_ITEM;

		pDropItemGeneral->Wear					= WEAR_NOT_ATTACHED;
		pDropItemGeneral->ItemWindowIndex		= POS_INVALID_POSITION;	// ���������� ������ â�� ù slot�� �־�ΰ�, ������ client���� �Ѵ�.
		pDropItemGeneral->NumOfEnchants			= 0;

		// 2010-04-09 by cmkwon, ����2�� �߰� ����(�ܰ躰 ���� �߰�) - 
		//if (TRUE == m_pFieldIOCP15->CheckTenderItem(pDropItemInfo->ItemNum))
		if(pDropItemInfo->IsTenderDropItem)
		{
			// Tender Item ó��
			D3DXVECTOR3 pos;			
			
			CMapBlock *pMapBlock = NULL;
			m_pFieldMapChannel1->m_pFieldMapProject->GetRandomPositionInRadius(&vec3MonPos, &pos, DROP_ITEM_RADIUS);
			pMapBlock						= m_pFieldMapChannel1->GetBlock(pos.x, pos.z);
			i_pDropItem->Position			= pos;
			i_pDropItem->dwStartTick		= m_pFieldIOCP15->GetCurrentServerTimeInMilliSeconds();
			i_pDropItem->pItem				= pDropItemGeneral;
			i_pDropItem->FirstCharacterUID1	= 0;
			i_pDropItem->bTendering			= TRUE;				// Tender ���� ���������� ����
			pMapBlock->InsertDropItem(pDropItemGeneral, i_pDropItem);

			CTenderItemInfo InsertTenderInfo;
			InsertTenderInfo.m_ItemFieldIndex		= (UINT)i_pDropItem->pItem;
			InsertTenderInfo.m_TenderItemPosition	= pos;
			InsertTenderInfo.m_TenderItemNum		= pDropItemGeneral->ItemNum;
			InsertTenderInfo.m_TenderItemCount		= pDropItemGeneral->CurrentCount;
			
			if(ERR_NO_ERROR != i_pInfiMan->PushTenderItemW(m_pFieldMapChannel1->GetMapChannelIndex().MapIndex, m_pFieldMapChannel1->GetMapChannelIndex().ChannelIndex, &InsertTenderInfo))
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CEXPDivisionManager::ProcessPickUpDropItemsInfinityMap# 10000 ! Map(%s) Tender(ItemNum:%s ItemCnt:%d) \r\n"
					, GetMapString(m_pFieldMapChannel1->GetMapChannelIndex(), string()), InsertTenderInfo.m_TenderItemNum, InsertTenderInfo.m_TenderItemCount);
			}

			// Tender Item ���� ����
			INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_TENDER_DROPITEM_INFO, T_FC_INFINITY_TENDER_DROPITEM_INFO, pMsgTenderItem, pMsgTenderItemBuf);
			pMsgTenderItem->ItemFieldIndex		= (UINT)i_pDropItem->pItem;
			pMsgTenderItem->TenderDropItemNum	= i_pDropItem->pItem->ItemNum;
			pMsgTenderItem->Count				= i_pDropItem->pItem->CurrentCount;
			pMsgTenderItem->PrefixCodeNum		= pDropItemGeneral->PrefixCodeNum;
			pMsgTenderItem->SuffixCodeNum		= pDropItemGeneral->SuffixCodeNum;
			pMsgTenderItem->TenderStartTime = InsertTenderInfo.m_TenderStartTime;
			m_pFieldMapChannel1->SendMessageToAllInChannel(pMsgTenderItemBuf, MSG_SIZE(MSG_FC_INFINITY_TENDER_DROPITEM_INFO));


			INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_SHOW_ITEM, T_FC_ITEM_SHOW_ITEM, pMsgShowItem, pMsgShowItemBuf);
			pMsgShowItem->ItemFieldIndex	= (UINT)i_pDropItem->pItem;
			pMsgShowItem->ItemNum			= i_pDropItem->pItem->ItemNum;
			pMsgShowItem->FirstCharacterUID	= 0;
			pMsgShowItem->Amount			= i_pDropItem->pItem->CurrentCount;
			pMsgShowItem->Position			= i_pDropItem->Position;
			pMsgShowItem->DropItemType		= DROP_ITEM_TYPE_PICKUP;
			m_pFieldIOCP15->SendInRangeMessageAroundPosition(pos, pMsgShowItemBuf, MSG_SIZE(MSG_FC_ITEM_SHOW_ITEM), m_pFieldMapChannel1);
			continue;
		}
		else if (MONEY_ITEM_NUMBER == pDropItemGeneral->ItemNum)
		{
			// �� ó�� - �����ϰ� 1/n
			pDropItemGeneral->CurrentCount = (pDropItemGeneral->CurrentCount) / max(1, (m_pFieldMapChannel1->m_mapCharacterUniqueNumberMapProject.size()));

			mt_auto_lock mta(&m_pFieldMapChannel1->m_mapCharacterUniqueNumberMapProject);
			mtmapUID2FieldIOCPSocket::iterator itrChar = m_pFieldMapChannel1->m_mapCharacterUniqueNumberMapProject.begin();
			for (; itrChar != m_pFieldMapChannel1->m_mapCharacterUniqueNumberMapProject.end(); itrChar++) {
				CFieldIOCPSocket *pFISock = itrChar->second;
				if(NULL != pFISock) {
					pDropItemGeneral->AccountUniqueNumber	= pFISock->m_character.AccountUniqueNumber;
					pDropItemGeneral->Possess				= pFISock->m_character.CharacterUniqueNumber;
					pFISock->m_ItemManager.InsertItemBaseByItemNum(MONEY_ITEM_NUMBER, pDropItemGeneral->CurrentCount, ItemUpdateType);
				}
			}// END - for(int i=0; i < nCnt; i++)
			util::del(pDropItemGeneral);
			continue;
		}
		else
		{
			// �����ϰ� �� ��� �������� ����
			mt_auto_lock mta(&m_pFieldMapChannel1->m_mapCharacterUniqueNumberMapProject);
			mtmapUID2FieldIOCPSocket::iterator itrChar = m_pFieldMapChannel1->m_mapCharacterUniqueNumberMapProject.begin();
			for (; itrChar != m_pFieldMapChannel1->m_mapCharacterUniqueNumberMapProject.end(); itrChar++) {
				CFieldIOCPSocket *pFISock = itrChar->second;
				if(NULL != pFISock) {
					if (IS_COUNTABLE_ITEM(pDropItemGeneral->Kind)) {
						// 2009-09-09 ~ 2010-02-23 by dhjin, ���Ǵ�Ƽ - ���Ǵ�Ƽ �� �Ϲ� ��� ������ ���� ���� ����
						pFISock->m_ItemManager.InsertItemBaseByItemNum(pDropItemGeneral->ItemNum, pDropItemGeneral->CurrentCount, ItemUpdateType);
					}
					else {
						pFISock->m_ItemManager.InsertItemBaseByItemNum(pDropItemGeneral->ItemNum, 1, ItemUpdateType);
					}
				}
			}// END - for(int i=0; i < nCnt; i++)
			util::del(pDropItemGeneral);
			continue;
		}
	}
	m_litMonsterDropItem.clear();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-12-14 by cmkwon, 2009 ũ���������̺�Ʈ(�Ѹ�� ������ �ϳ���) - 
/// \author		cmkwon
/// \date		2009-12-14 ~ 2009-12-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CFieldIOCPSocket * CEXPDivisionManager::GetNextUser(CFieldIOCPSocket *i_pCurFISoc, BOOL i_bLiveCheck/*=TRUE*/)
{
	if(NULL == i_pCurFISoc
		|| m_vectSEXP_DIVISIONList.empty())
	{
		return NULL;
	}

	BOOL bFindedCurUser = FALSE;
	vectSEXP_DIVISION::iterator itr(m_vectSEXP_DIVISIONList.begin());
	for(; itr != m_vectSEXP_DIVISIONList.end(); itr++)
	{
		SEXP_DIVISION *pExpDiv = &*itr;
		if(bFindedCurUser
			&& pExpDiv->ArrSEXP_PARTYMEMBER[0].pFISock0
			&& pExpDiv->ArrSEXP_PARTYMEMBER[0].pFISock0->IsValidCharacter(i_bLiveCheck))
		{
			return pExpDiv->ArrSEXP_PARTYMEMBER[0].pFISock0;
		}

		if(NULL == pExpDiv->pFParty0
			|| 1 >= pExpDiv->nPartyMemberCnts)
		{
			if(i_pCurFISoc == pExpDiv->ArrSEXP_PARTYMEMBER[0].pFISock0)
			{
				bFindedCurUser = TRUE;
			}
		}
		else
		{
			for (int j = 0; j < pExpDiv->nPartyMemberCnts; j++)
			{
				CFieldIOCPSocket *p2FISock = pExpDiv->ArrSEXP_PARTYMEMBER[j].pFISock0;
				
				if(NULL == p2FISock
					|| FALSE == p2FISock->IsValidCharacter(i_bLiveCheck))
				{// 2006-07-18 by cmkwon, ��ȿ ��Ƽ�� üũ
					continue;
				}
				
				if(bFindedCurUser)
				{
					return p2FISock;
				}

				if(i_pCurFISoc == p2FISock)
				{
					bFindedCurUser = TRUE;
				}
			}
		}
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		cmkwon
/// \date		2010-04-12 ~ 2010-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CEXPDivisionManager::ProcessPickUpTenderItemsInInfinityMap(string *o_pStrDropItemlist, CFieldMonster *i_pDeadFMonster, CInfinityManager *i_pInfiMan)
{
	if(NULL == m_pFieldMapChannel1 
		|| m_litMonsterDropItem.empty())
	{
		return;
	}

	// 2010-04-09 by cmkwon, ����2�� �߰� ����(�ܰ躰 ���� �߰�) - 
	D3DXVECTOR3 vec3MonPos = i_pDeadFMonster->PositionVector;
	if(0.0f > vec3MonPos.x || 0.0f > vec3MonPos.z)
	{		
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] DropItem error !! MonsterIdx(%d) Pos(%f, %f, %f) \r\n", i_pDeadFMonster->MonsterIndex, i_pDeadFMonster->PositionVector.x, i_pDeadFMonster->PositionVector.y, i_pDeadFMonster->PositionVector.z);
		return;
	}
	MONSTER_INFO *pMonsterInfo = i_pDeadFMonster->MonsterInfoPtr;
	if(NULL == pMonsterInfo)
	{
		return;
	}

	mtlistMonsterDropItem::iterator itrDropItem = m_litMonsterDropItem.begin();
	for(; itrDropItem != m_litMonsterDropItem.end(); itrDropItem++)
	{
		MONSTER_DROP_ITEM_GENERAL tmpMonsterDropItemGeneral = *itrDropItem;
		ITEM_GENERAL *pDropItemGeneral		= tmpMonsterDropItemGeneral.pItemGeneral;
		ITEM *pDropItemInfo					= tmpMonsterDropItemGeneral.pItemGeneral->ItemInfo;
		
		if (COMPARE_MPOPTION_BIT(pMonsterInfo->MPOption, MPOPTION_BIT_BOSS_MONSTER|MPOPTION_BIT_KEY_MONSTER|MPOPTION_BIT_KEY_MONSTER_ALIVE_FOR_GAMECLEAR))
		{
			*o_pStrDropItemlist += pDropItemInfo->ItemName;
			*o_pStrDropItemlist += "|";
		}

		pDropItemGeneral->Wear					= WEAR_NOT_ATTACHED;
		pDropItemGeneral->ItemWindowIndex		= POS_INVALID_POSITION;	// ���������� ������ â�� ù slot�� �־�ΰ�, ������ client���� �Ѵ�.
		pDropItemGeneral->NumOfEnchants			= 0;

		
		DROPITEM			dropItem;
		CMapBlock *pMapBlock = NULL;
		D3DXVECTOR3 pos;			
		m_pFieldMapChannel1->m_pFieldMapProject->GetRandomPositionInRadius(&vec3MonPos, &pos, DROP_ITEM_RADIUS);
		pMapBlock					= m_pFieldMapChannel1->GetBlock(pos.x, pos.z);
		dropItem.Position			= pos;
		dropItem.dwStartTick		= m_pFieldIOCP15->GetCurrentServerTimeInMilliSeconds();
		dropItem.pItem				= pDropItemGeneral;
		dropItem.bProcessing		= FALSE;
		dropItem.FirstCharacterUID1	= 0;
		dropItem.bTendering			= TRUE;				// Tender ���� ���������� ����
		pMapBlock->InsertDropItem(pDropItemGeneral, &dropItem);

		CTenderItemInfo InsertTenderInfo;
		InsertTenderInfo.m_ItemFieldIndex		= (UINT)(dropItem.pItem);
		InsertTenderInfo.m_TenderItemPosition	= pos;
		InsertTenderInfo.m_TenderItemNum		= pDropItemGeneral->ItemNum;
		InsertTenderInfo.m_TenderItemCount		= pDropItemGeneral->CurrentCount;
		if(ERR_NO_ERROR != i_pInfiMan->PushTenderItemW(m_pFieldMapChannel1->GetMapChannelIndex().MapIndex, m_pFieldMapChannel1->GetMapChannelIndex().ChannelIndex, &InsertTenderInfo))
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CEXPDivisionManager::ProcessPickUpDropItemsInfinityMap# 10000 ! Map(%s) Tender(ItemNum:%s ItemCnt:%d) \r\n"
				, GetMapString(m_pFieldMapChannel1->GetMapChannelIndex(), string()), InsertTenderInfo.m_TenderItemNum, InsertTenderInfo.m_TenderItemCount);
		}

		// Tender Item ���� ����
		INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_TENDER_DROPITEM_INFO, T_FC_INFINITY_TENDER_DROPITEM_INFO, pMsgTenderItem, pMsgTenderItemBuf);
		pMsgTenderItem->ItemFieldIndex		= (UINT)(dropItem.pItem);
		pMsgTenderItem->TenderDropItemNum	= dropItem.pItem->ItemNum;
		pMsgTenderItem->Count				= dropItem.pItem->CurrentCount;
		pMsgTenderItem->PrefixCodeNum		= pDropItemGeneral->PrefixCodeNum;
		pMsgTenderItem->SuffixCodeNum		= pDropItemGeneral->SuffixCodeNum;
		pMsgTenderItem->TenderStartTime = InsertTenderInfo.m_TenderStartTime;
		m_pFieldMapChannel1->SendMessageToAllInChannel(pMsgTenderItemBuf, MSG_SIZE(MSG_FC_INFINITY_TENDER_DROPITEM_INFO));

		INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_SHOW_ITEM, T_FC_ITEM_SHOW_ITEM, pMsgShowItem, pMsgShowItemBuf);
		pMsgShowItem->ItemFieldIndex	= (UINT)(dropItem.pItem);
		pMsgShowItem->ItemNum			= dropItem.pItem->ItemNum;
		pMsgShowItem->FirstCharacterUID	= 0;
		pMsgShowItem->Amount			= dropItem.pItem->CurrentCount;
		pMsgShowItem->Position			= dropItem.Position;
		pMsgShowItem->DropItemType		= DROP_ITEM_TYPE_PICKUP;
		m_pFieldIOCP15->SendInRangeMessageAroundPosition(pos, pMsgShowItemBuf, MSG_SIZE(MSG_FC_ITEM_SHOW_ITEM), m_pFieldMapChannel1);
	}
	m_litMonsterDropItem.clear();
}