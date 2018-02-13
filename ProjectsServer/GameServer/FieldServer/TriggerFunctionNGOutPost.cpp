// TriggerFunctionNGOutPost.cpp: implementation of the CTriggerFunctionNGOutPost class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TriggerFunctionNGOutPost.h"
#include "FieldMapProject.h"
#include "FieldMapChannel.h"
#include "TriggerNGCInflWarMonsterGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define MAP_4303						4303		// �̷��� �ȵŴµ�... �̹� td_outpostinfo�� �������� �����ϴ�... �׳� �ڵ忡 ���� �Ф�
#define MAP_4303_OUTPOST_NEXT_CITY		2007		// �̷��� �ȵŴµ�... �̹� td_outpostinfo�� �������� �����ϴ�... �׳� �ڵ忡 ���� �Ф�

CTriggerFunctionNGOutPost::CTriggerFunctionNGOutPost()
{
	OnClear();
}

CTriggerFunctionNGOutPost::~CTriggerFunctionNGOutPost()
{
	OnDestroy();
}

void CTriggerFunctionNGOutPost::OnClear()
{
	m_pFieldIOCP = NULL;
	m_pFieldNextOutPostMapChannel = NULL;
	m_pFieldOutPostCityMapChannel = NULL;

	m_bTodayTriggerEventPlay = FALSE;
	m_bTriggerOn = FALSE;
	m_bIsOutPostWarRuning = FALSE;
	m_bCommendReset = FALSE;
	m_bPossibleBossAttack = FALSE;
	m_bBeforePossibleBossAttack = FALSE;
	m_bIsBossDestroy = FALSE;
	m_nSummonMonsterGroupID	= 0;		// ��ȯ ���� �׷� ID
	m_nStandardOutPostMap	= 0;		// ������ �� �������� ��, �ش� ���������� �߻��� ������ ���� �ð��� Ʈ���Ű� �ߵ� �ȴ�.
	m_nBossMonster			= 0;
	m_nWinInfluenceWP		= 0;
	m_nLossInfluenceWP		= 0;
	m_nNextOutPostMap		= 0;		// ���� �������� ��
	m_nDestroyCrystalcount	= 0;
	m_nFixedBuffItem		= 0;
	
	{
		mt_auto_lock mta(&m_mtvectTriggerAttackUserDamage);
		m_mtvectTriggerAttackUserDamage.clear();
	}

	m_vectorTriggerNGCOutPostSummonMonsterGroup.clear();
	m_vectTriggerOutPostBossKillInfo.clear();
	{
		mt_auto_lock mta(&m_mtvectTriggerOutPostMonsterKillInfo);
		m_mtvectTriggerOutPostMonsterKillInfo.clear();
	}

	CTriggerFunction::OnClear();
}

BOOL CTriggerFunctionNGOutPost::OnCreate(CMapTriggerManager *pMapTriggerManager, MapTriggerID_t MapTriggerID, MapIndex_t MapIndex, ChannelIndex_t MapChannel, FunctionID_t FunctionID, MapTriggerType_t TriggerType)
{
	return CTriggerFunction::OnCreate(pMapTriggerManager, MapTriggerID, MapIndex, MapChannel, FunctionID, TriggerType);
}

void CTriggerFunctionNGOutPost::SetNGCOutPostInfo(SummonMosterGroupID_t i_nSummonMonsterGroupID, MapIndex_t i_nStandardOutPostMap, MonIdx_t i_nBossMonster, INT i_nWinInfluenceWP, INT i_nLossInfluenceWP, MapIndex_t i_nNextOutPostMap, INT i_nDestroyCrystalcount)
{
	m_nSummonMonsterGroupID = i_nSummonMonsterGroupID;		// ��ȯ ���� �׷� ID
	m_nStandardOutPostMap	= i_nStandardOutPostMap;			// ������ �� �������� ��, �ش� ���������� �߻��� ������ ���� �ð��� Ʈ���Ű� �ߵ� �ȴ�.
	m_nBossMonster			= i_nBossMonster;
	m_nWinInfluenceWP		= i_nWinInfluenceWP;
	m_nLossInfluenceWP		= i_nLossInfluenceWP;
	m_nNextOutPostMap		= i_nNextOutPostMap;				// ���� �������� ��
	m_nDestroyCrystalcount	= i_nDestroyCrystalcount;			// �ı��ؾ� �ϴ� ũ����Ż�� ��

	CMapTriggerManager* pMapTriggerManager = GetMapTriggerManager();
	if ( NULL == pMapTriggerManager )
	{
		//			g_pFieldGlobal->WriteSystemLogEX(TRUE,"[ERROR] CTriggerFunctionNGOutPost::DoMinutelyWork, pMapTriggerManager IS NULL\r\n");
		return;
	}
	
	m_pFieldIOCP = pMapTriggerManager->GetFieldIOCP();
	if( NULL == m_pFieldIOCP )
	{
		//			g_pFieldGlobal->WriteSystemLogEX(TRUE,"[ERROR] CTriggerFunctionNGOutPost::DoMinutelyWork, pFieldIOCP IS NULL\r\n");
		return;
	}
}

void CTriggerFunctionNGOutPost::InsertBossMonsterKillInfo(STRIGGER_TRIGGER_OUTPOST_BOSS_KILL_INFO* i_BossKillInfo)
{
	if( NULL != i_BossKillInfo )
	{
		STRIGGER_TRIGGER_OUTPOST_BOSS_KILL_INFO tmBossKillInfo;
		util::zero(&tmBossKillInfo, sizeof(tmBossKillInfo));
		memcpy(&tmBossKillInfo, i_BossKillInfo, sizeof(tmBossKillInfo));
		m_vectTriggerOutPostBossKillInfo.push_back(tmBossKillInfo);			// �������� ���� ������ ���� ���Ϳ� �߰�.
		
		mt_auto_lock mta(&m_mtvectTriggerOutPostMonsterKillInfo);
		m_mtvectTriggerOutPostMonsterKillInfo.push_back(tmBossKillInfo);	// �̾ ����ܰ踦 ������ ���Ϳ� ������ ������ �߰�.
	}
}

BOOL CTriggerFunctionNGOutPost::InsertNGCOutPostSummonMonster(CTriggerNGCInflWarMonsterGroup *i_pTriggerFnctionNGCOutPostSummonMonster)
{
	if( NULL == i_pTriggerFnctionNGCOutPostSummonMonster )
	{
		return FALSE;
	}
	
	m_vectorTriggerNGCOutPostSummonMonsterGroup.push_back(i_pTriggerFnctionNGCOutPostSummonMonster);
	
	return TRUE;
}

void CTriggerFunctionNGOutPost::InsertCrystalBuff(STRIGGER_CRYSTAL_BUFF *i_CrystalBuff)
{
	if( NULL != i_CrystalBuff )
	{
		STRIGGER_CRYSTAL_BUFF tmCrystalBuff;
		util::zero(&tmCrystalBuff, sizeof(tmCrystalBuff));
		memcpy(&tmCrystalBuff, i_CrystalBuff, sizeof(tmCrystalBuff));
		m_vectCrystalBuff.push_back(tmCrystalBuff);
	}
}	

void CTriggerFunctionNGOutPost::DoMinutelyWork(ATUM_DATE_TIME *pDateTime)
{
	if ( NULL == m_pFieldNextOutPostMapChannel )
	{
		if ( NULL != m_pFieldIOCP)
		{
			CFieldMapProject *pFMProOutPost = m_pFieldIOCP->GetFieldMapProjectByMapIndex(m_nNextOutPostMap);
			if( NULL != pFMProOutPost )
			{
				m_pFieldNextOutPostMapChannel = (CFieldMapChannel*)(pFMProOutPost->GetMapChannelByIndex(0));		// �⺻ ä�η� �̵�
			}
		}
	}

	if ( NULL == m_pFieldOutPostCityMapChannel )
	{
		if ( NULL != m_pFieldIOCP)
		{
			if ( MAP_4303 == m_nNextOutPostMap )
			{
				CFieldMapProject *pFMProCity = m_pFieldIOCP->GetFieldMapProjectByMapIndex(MAP_4303_OUTPOST_NEXT_CITY);
				if ( NULL != pFMProCity )
				{
					m_pFieldOutPostCityMapChannel = (CFieldMapChannel*)(pFMProCity->GetMapChannelByIndex(0));
				}
			}
		}
	}

	if ( FALSE == m_bTriggerOn && FALSE == m_bIsOutPostWarRuning )
	{
		// Ʈ���� �ߵ� ������ �� �������� ������ �����´�.
		COutPost* pStandardOutPost = m_pFieldIOCP->m_OutPostManager.GetOutPost(m_nStandardOutPostMap);
		if ( NULL == pStandardOutPost )
		{
//			g_pFieldGlobal->WriteSystemLogEX(TRUE,"[ERROR] CTriggerFunctionNGOutPost::DoMinutelyWork, pStandardOutPost IS NULL\r\n");
			return;
		}

		// �������� �ð��� �����´�.
		ATUM_DATE_TIME* pStandardOutPostTime = pStandardOutPost->GetOutPostNextWarTime();
		if ( NULL == pStandardOutPostTime )
		{
//			g_pFieldGlobal->WriteSystemLogEX(TRUE,"[ERROR] CTriggerFunctionNGOutPost::DoMinutelyWork, pStandardOutPostTime IS NULL\r\n");
			return;
		}
		
		ATUM_DATE_TIME CurrentTime;
		CurrentTime.SetCurrentDateTime(TRUE);
		CurrentTime.AddDateTime(0,0,4,0,0,0);	// ���� �ð��� 4���� ������ ������ �� �������� ��¥�� �񱳸� �ؾ� ������ �Ǵ� ���������� �������� Ʈ���Ű� �ߵ��� �� �ִ�.
		if( CurrentTime.Year == pStandardOutPostTime->Year && CurrentTime.Month == pStandardOutPostTime->Month && CurrentTime.Day == pStandardOutPostTime->Day )
		{
			if ( FALSE == m_bTodayTriggerEventPlay )
			{
				COutPost* pOutPost = m_pFieldIOCP->m_OutPostManager.GetOutPost(m_nNextOutPostMap);
				if ( NULL != pOutPost )
				{
					pOutPost->SetOutPostNextWarTime(pStandardOutPost->GetOutPostNextWarTime());
				}
				
				int	CheckRemainWarTimeAtMin = pStandardOutPostTime->GetTimeDiffTimeInMinutes(CurrentTime);
				
				if(30 == CheckRemainWarTimeAtMin || 10 == CheckRemainWarTimeAtMin || 5 == CheckRemainWarTimeAtMin)
				{
					if ( NULL != m_pFieldIOCP )
					{
						if( 10 == CheckRemainWarTimeAtMin )
						{
							if ( NULL != pOutPost && NULL != m_pFieldNextOutPostMapChannel && NULL != m_pFieldOutPostCityMapChannel )
							{
								m_pFieldIOCP->SendString128ToAllClients(STRING_128_USER_NOTICE, STRMSG_130121_0001);
								pOutPost->InitOutPostInfluence();
								pOutPost->InitOutPostGuildUID();
								pOutPost->InitOutPostGuildName();
								m_pFieldNextOutPostMapChannel->WarpToCityMap(0);
								m_pFieldOutPostCityMapChannel->WarpToCityMap(0);
								return;
							}
						}
						m_pFieldIOCP->m_OutPostManager.SendOutPostWarRemainTime(CheckRemainWarTimeAtMin, pOutPost->GetOutPostInfluence(), pOutPost->GetOutPostMapIndex());
					}
					return;
				}
				else if(0 >= CheckRemainWarTimeAtMin )
				{
					m_bTriggerOn = TRUE;				// Ʈ���� �ߵ�
				}
			}
		}
		else
		{
			m_bTodayTriggerEventPlay = FALSE;
		}
	}

	if( TRUE == m_bTriggerOn || TRUE == m_bCommendReset )
	{	
		if( NULL != m_pFieldMapChannel && NULL != m_pFieldMapChannel->m_pFieldMapProject )
		{	
			StopEvent();				// �������� ���� ����� ���� �ʱ�ȭ
			InitBossKillPlayStep();		// ���� �ܰ� �ʱ�ȭ

			if( TRUE == m_pFieldMapChannel->m_pFieldMapProject->GetNPCServerStartFlag() && TRUE == m_pFieldMapChannel->m_pFieldMapProject->IsNPCServerMonsterCreated() )	// NPC ������ ����Ǿ� ���� ������ �����Ѱ�?
			{
				CreateAllMonster();
				m_pFieldIOCP->SetNextMapWarpPossible(m_nNextOutPostMap, FALSE);
				m_bIsOutPostWarRuning = TRUE;
				if ( TRUE == m_bTriggerOn )
				{
					m_bTriggerOn = FALSE;
					m_bTodayTriggerEventPlay = TRUE;	// ���� Ʈ���Ű� �ߵ��ߴٰ� üũ�� �Ѵ�.
				}
				if ( TRUE == m_bCommendReset )
				{
					m_bCommendReset = FALSE;
				}

				if ( NULL != m_pFieldIOCP )
				{
					m_pFieldIOCP->SetNextMapWarpPossible(m_nNextOutPostMap, FALSE);
					COutPost* pOutPost = m_pFieldIOCP->m_OutPostManager.GetOutPost(m_nNextOutPostMap);
					if ( NULL != pOutPost )
					{
						pOutPost->SetOutPostWarStartTime();
						m_pFieldIOCP->m_OutPostManager.OutPostWarStart(pOutPost);
						m_pFieldNextOutPostMapChannel->WarpToCityMap(0);
						return;
					}
				}
			}
			else
			{
#ifdef _DEBUG
				if( NULL != m_pFieldMapChannel )
				{
					char szString[256] = { 0, };
					sprintf(szString, "\\y[NGC �������� �ý���]\\y NPC ���� �ʱ�ȭ ��� ��");
					m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, szString);
				}
#endif
			}
		}
	}


	if ( TRUE == m_bIsOutPostWarRuning )
	{
		if ( NULL != m_pFieldIOCP )
		{
			COutPost* pOutPost = m_pFieldIOCP->m_OutPostManager.GetOutPost(m_nNextOutPostMap);
			if ( NULL != pOutPost )
			{
				if ( OUTPOST_NORMAL == pOutPost->GetOutPostState() )
				{
					m_bIsOutPostWarRuning= FALSE;
					DestroyAllMonster();
				}
				else
				{
					CrystalRegen();
				}
			}
		}
	}

}

void CTriggerFunctionNGOutPost::CrystalRegen()
{
	ATUM_DATE_TIME tmCurrentDateTime { true };
	mt_auto_lock mta(&m_mtvectTriggerOutPostMonsterKillInfo);
	mtvectorTriggerOutPostMonsterKillInfo::iterator itrKillMonsterInfo = m_mtvectTriggerOutPostMonsterKillInfo.begin();
	for (; itrKillMonsterInfo != m_mtvectTriggerOutPostMonsterKillInfo.end() ; itrKillMonsterInfo++ )
	{
		if ( TRUE == itrKillMonsterInfo->IsDestroy )
		{
			if ( RANDI(itrKillMonsterInfo->RegenMinTimeMinutes*60, itrKillMonsterInfo->RegenMaxTimeMinutes*60) < tmCurrentDateTime.GetTimeDiffTimeInSeconds(itrKillMonsterInfo->DestroyTime) )
			{
				if( NULL != m_pFieldMapChannel )
				{
					itrKillMonsterInfo->IsDestroy = FALSE;
					INIT_MSG_WITH_BUFFER(MSG_FN_ADMIN_SUMMON_MONSTER, T_FN_ADMIN_SUMMON_MONSTER, pMsgSummonMonster, SendBuf);
					pMsgSummonMonster->ChannelIndex			= m_pFieldMapChannel->GetMapChannelIndex().ChannelIndex;
					util::strncpy(pMsgSummonMonster->CharacterName, "CrystalMonster", SIZE_MAX_CHARACTER_NAME);
					pMsgSummonMonster->MonsterUnitKind		= itrKillMonsterInfo->CrystalNum;
					pMsgSummonMonster->MonsterTargetType1	= MONSTER_TARGETTYPE_NORMAL;
					pMsgSummonMonster->TargetTypeData1		= 0;
					pMsgSummonMonster->CltIdxForTargetType1	= 0;
					pMsgSummonMonster->NumOfMonster			= 1;
					pMsgSummonMonster->Position				= AVECTOR3(0, 0, 0);
					pMsgSummonMonster->MonsterBalanceData.Init(0);
					m_pFieldMapChannel->Send2NPCServerW(SendBuf, MSG_SIZE(MSG_FN_ADMIN_SUMMON_MONSTER));

					m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, STRMSG_130121_0002);
					if ( NULL != m_pFieldNextOutPostMapChannel )
					{
						m_pFieldNextOutPostMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, STRMSG_130121_0002);
					}
				}
			}
		}
	}
	m_bPossibleBossAttack = IsPossibleBossAttack();
}

BOOL CTriggerFunctionNGOutPost::OnMonsterCreate(CFieldMonster *pMonster)
{
	if( NULL == pMonster || NULL == pMonster->MonsterInfoPtr )
	{
		return FALSE;
	}
	
	vectorTriggerOutPostBossKillInfo::iterator itrBossKillInfo = m_vectTriggerOutPostBossKillInfo.begin();		// ���� ���͸� ��� ���� ���� ��ƾ� �ϴ� ���� ���� ������ ���� ����
	for (; itrBossKillInfo != m_vectTriggerOutPostBossKillInfo.end() ; itrBossKillInfo++ )
	{
		if ( pMonster->MonsterInfoPtr->MonsterUnitKind == itrBossKillInfo->BossMonster || pMonster->MonsterInfoPtr->MonsterUnitKind == itrBossKillInfo->CrystalNum )
		{
			pMonster->InsertTriggerFunction(this);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CTriggerFunctionNGOutPost::OnMonsterDead(CFieldMonster *pMonster)
{
	if( NULL == pMonster || NULL == pMonster->MonsterInfoPtr )
	{
		return FALSE;
	}

	if( m_nBossMonster == pMonster->MonsterInfoPtr->MonsterUnitKind )
	{
		RunBossKillEvent();
		return TRUE;
	}
	else
	{
		BOOL Rect = FALSE;
		mt_auto_lock mta(&m_mtvectTriggerOutPostMonsterKillInfo);
		mtvectorTriggerOutPostMonsterKillInfo::iterator itrKillMonsterInfo = m_mtvectTriggerOutPostMonsterKillInfo.begin();
		for (; itrKillMonsterInfo != m_mtvectTriggerOutPostMonsterKillInfo.end() ; itrKillMonsterInfo++ )
		{
			if ( itrKillMonsterInfo->CrystalNum == pMonster->MonsterInfoPtr->MonsterUnitKind )
			{
				Rect = TRUE;
				itrKillMonsterInfo->IsDestroy = TRUE;
				itrKillMonsterInfo->DestroyTime.SetCurrentDateTime();

				if ( TRUE == itrKillMonsterInfo->IsDestroy )
				{
					RunKeyMonsterKillEvent(itrKillMonsterInfo->CrystalNum);
				}
				break;
			}
		}

		m_bPossibleBossAttack = IsPossibleBossAttack();
		return Rect;
	}
}

void CTriggerFunctionNGOutPost::RunKeyMonsterKillEvent(MonIdx_t i_nMonsterNum)
{
	m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, STRMSG_130121_0003);
	if ( NULL != m_pFieldNextOutPostMapChannel )
	{
		m_pFieldNextOutPostMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, STRMSG_130121_0002);
	}

	BYTE tmWinInfluenceType, tmDefeatInfluenceType;
	if ( INFLUENCE_TYPE_VCN == FindWinInfluenc(i_nMonsterNum) )					// �̱� ���� ã��
	{
		tmWinInfluenceType		= INFLUENCE_TYPE_VCN;
		tmDefeatInfluenceType	= INFLUENCE_TYPE_ANI;
	}
	else
	{
		tmWinInfluenceType		= INFLUENCE_TYPE_ANI;
		tmDefeatInfluenceType	= INFLUENCE_TYPE_VCN;
	}

	if ( 0 != m_nFixedBuffItem && NULL != m_pFieldMapChannel  )
	{
//		m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_ADMIN_CMD, "\\y[NGC �������� �ý���]\\y ������ ������ ���� �Դϴ�.");
		m_pFieldMapChannel->ApplySkillToAllInChannel(m_nFixedBuffItem, tmWinInfluenceType);
		if ( NULL != m_pFieldNextOutPostMapChannel )
		{
//			m_pFieldNextOutPostMapChannel->SendString128ToAllInChannel(STRING_128_ADMIN_CMD, "\\y[NGC �������� �ý���]\\y ������ ������ ���� �Դϴ�.");
			m_pFieldNextOutPostMapChannel->ApplySkillToAllInChannel(m_nFixedBuffItem, tmWinInfluenceType);
		}
	}
	else if ( TRUE != m_vectCrystalBuff.empty() && NULL != m_pFieldMapChannel )
	{
		random_shuffle(m_vectCrystalBuff.begin(), m_vectCrystalBuff.end());
		vectorTriggerCrystalBuff::iterator itrCrystalBuff = m_vectCrystalBuff.begin();
		m_pFieldMapChannel->ApplySkillToAllInChannel(itrCrystalBuff->SkillItemNum, tmWinInfluenceType);
		if ( NULL != m_pFieldNextOutPostMapChannel )
		{
			m_pFieldNextOutPostMapChannel->ApplySkillToAllInChannel(itrCrystalBuff->SkillItemNum, tmWinInfluenceType);
		}
	}

	return;
}

void CTriggerFunctionNGOutPost::RunBossKillEvent()
{
	m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, STRMSG_130121_0006);

	m_bIsBossDestroy = TRUE;

	BYTE tmWinInfluenceType, tmDefeatInfluenceType;
	if ( INFLUENCE_TYPE_VCN == FindWinInfluenc(m_nBossMonster) )					// �̱� ���� ã��
	{
		tmWinInfluenceType		= INFLUENCE_TYPE_VCN;
		tmDefeatInfluenceType	= INFLUENCE_TYPE_ANI;
	}
	else
	{
		tmWinInfluenceType		= INFLUENCE_TYPE_ANI;
		tmDefeatInfluenceType	= INFLUENCE_TYPE_VCN;
	}
	
	if( NULL != m_pFieldMapChannel )
	{
		// ���� ����
		m_pFieldMapChannel->AddWarPointInMap(m_nWinInfluenceWP, TRUE, tmWinInfluenceType, WPUT_TRIGGER);
		m_pFieldMapChannel->AddWarPointInMap(m_nLossInfluenceWP, TRUE, tmDefeatInfluenceType, WPUT_TRIGGER);
	}

	m_pFieldIOCP->SetNextMapWarpPossible(m_nNextOutPostMap, TRUE);
	m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_OUTPOST_1ST_VALUE_BOSS_PROTECTIVE_FILM_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_OUTPOST_1ST_VALUE_BOSS_PROTECTIVE_FILM_OBJECT_NUM_DEFAULT);
}

BOOL CTriggerFunctionNGOutPost::IsPossibleBossAttack()
{
	if ( TRUE == m_bIsBossDestroy )
	{
		return FALSE;
	}
	int DestoryCrystalCount = 0;
	mt_auto_lock mta(&m_mtvectTriggerOutPostMonsterKillInfo);
	mtvectorTriggerOutPostMonsterKillInfo::iterator itrKillMonsterInfo = m_mtvectTriggerOutPostMonsterKillInfo.begin();
	for (; itrKillMonsterInfo != m_mtvectTriggerOutPostMonsterKillInfo.end() ; itrKillMonsterInfo++ )
	{
		if ( TRUE == itrKillMonsterInfo->IsDestroy )
		{
			DestoryCrystalCount++;
		}
	}
	
	if ( m_nDestroyCrystalcount <= DestoryCrystalCount )
	{
		if ( FALSE == m_bBeforePossibleBossAttack && NULL != m_pFieldMapChannel )
		{
			m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, STRMSG_130121_0005);
			m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_OUTPOST_1ST_VALUE_BOSS_PROTECTIVE_FILM_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_OUTPOST_1ST_VALUE_BOSS_PROTECTIVE_FILM_OBJECT_NUM_DEFAULT);
			m_bBeforePossibleBossAttack = TRUE;
		}
		return TRUE;
	}
	else
	{
		if ( TRUE == m_bBeforePossibleBossAttack  && NULL != m_pFieldMapChannel )
		{
			m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, STRMSG_130121_0004);
			m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_OUTPOST_1ST_VALUE_BOSS_PROTECTIVE_FILM_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_OUTPOST_1ST_VALUE_BOSS_PROTECTIVE_FILM_OBJECT_NUM_CHANGE);
			m_bBeforePossibleBossAttack = FALSE;
		}
		return FALSE;
	}
}

BOOL CTriggerFunctionNGOutPost::OnIsPossibleBossAttack(MonIdx_t MonsterIdx)
{
	if ( m_nBossMonster == MonsterIdx )
	{	// �������͸� ���θ� Ȯ���ϰ�
		return m_bPossibleBossAttack;
	}
	else
	{	// �ƴ϶�� ������ ������ �����ϴ�
		return TRUE;
	}
}

BOOL CTriggerFunctionNGOutPost::OnAddDamage(MonIdx_t MonsterIdx, UID32_t CharacterUniqueNumber, UID32_t GuildUniqueNumber, BYTE InfluenceType, float fDamage)
{
	mt_auto_lock mta(&m_mtvectTriggerAttackUserDamage);

	mtvectTriggerAttackUserDamage::iterator itr = m_mtvectTriggerAttackUserDamage.begin();
	while(itr != m_mtvectTriggerAttackUserDamage.end())
	{
		if( itr->MonsterIdx == MonsterIdx && itr->CharacterUniqueNumber == CharacterUniqueNumber )
		{
			itr->fDamage += fDamage;
			itr->GuildUniqueNumber = GuildUniqueNumber;
			itr->InfluenceType = InfluenceType;

			return TRUE;
		}
		itr++;
	}

	STRIGGER_ATTACK_USER_DAMAGE	tmCrystalDamage;
	tmCrystalDamage.MonsterIdx = MonsterIdx;
	tmCrystalDamage.CharacterUniqueNumber = CharacterUniqueNumber;
	tmCrystalDamage.GuildUniqueNumber = GuildUniqueNumber;
	tmCrystalDamage.InfluenceType = InfluenceType;
	tmCrystalDamage.fDamage = fDamage;
	m_mtvectTriggerAttackUserDamage.push_back(tmCrystalDamage);

	return TRUE;
}

void CTriggerFunctionNGOutPost::OnEventSkip(BOOL i_bCheckCreateCrystal)
{
	if( FALSE == i_bCheckCreateCrystal )
	{
		return;
	}
	DestroyAllMonster();
	m_bCommendReset = TRUE;
	if ( NULL != m_pFieldMapChannel )
	{
//		m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_ADMIN_CMD, "\\y[NGC �������� �ý���]\\y ��б��� Ʈ���� �ʱ�ȭ");
		
		if ( NULL != m_pFieldIOCP )
		{
			COutPost* pOutPost = m_pFieldIOCP->m_OutPostManager.GetOutPost(m_nNextOutPostMap);
			if ( NULL != pOutPost && NULL != m_pFieldNextOutPostMapChannel && NULL != m_pFieldOutPostCityMapChannel )
			{
				pOutPost->InitOutPostInfluence();
				pOutPost->InitOutPostGuildUID();
				pOutPost->InitOutPostGuildName();
				m_pFieldNextOutPostMapChannel->WarpToCityMap(0);
				m_pFieldOutPostCityMapChannel->WarpToCityMap(0);
				if ( OUTPOST_NORMAL != pOutPost->GetOutPostState() )
				{
					vectSOUTPOSTWAR_EV_END				tmVectEvEndList;
					m_pFieldIOCP->m_OutPostManager.OutPostWarEnd(pOutPost, &tmVectEvEndList);
				}
			}
		}
	}
}

void CTriggerFunctionNGOutPost::OnFixedBuffItem(INT i_nFixedBuffItem)
{
	if ( 0 != i_nFixedBuffItem )
	{
		vectorTriggerCrystalBuff::iterator itrBuffItem = m_vectCrystalBuff.begin();
		for ( ; itrBuffItem != m_vectCrystalBuff.end() ; itrBuffItem++ )
		{
			if ( i_nFixedBuffItem == itrBuffItem->SkillItemNum )
			{
				m_nFixedBuffItem = i_nFixedBuffItem;
//				m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_ADMIN_CMD, "\\y[NGC �������� �ý���]\\y ������ �����Ǿ����ϴ�.");
				return;
			}
		}
	}
	else
	{
		m_nFixedBuffItem = i_nFixedBuffItem;
//		m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_ADMIN_CMD, "\\y[NGC �������� �ý���]\\y �������� ���");
		return;
	}
	m_nFixedBuffItem = i_nFixedBuffItem;
//	m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_ADMIN_CMD, "\\y[NGC �������� �ý���]\\y �߸��� ���� ������ �Դϴ�. ���������� ��� �մϴ�.");
}

void CTriggerFunctionNGOutPost::StopEvent()
{
	mt_auto_lock mta(&m_mtvectTriggerAttackUserDamage);
	m_mtvectTriggerAttackUserDamage.clear();
}

void CTriggerFunctionNGOutPost::InitBossKillPlayStep()
{
	m_bPossibleBossAttack = FALSE;
	
	mt_auto_lock mta(&m_mtvectTriggerOutPostMonsterKillInfo);
	m_mtvectTriggerOutPostMonsterKillInfo.clear();
	
	vectorTriggerOutPostBossKillInfo::iterator itrBossKillInfo =  m_vectTriggerOutPostBossKillInfo.begin();
	for (; itrBossKillInfo != m_vectTriggerOutPostBossKillInfo.end() ; itrBossKillInfo++ )
	{
		STRIGGER_TRIGGER_OUTPOST_BOSS_KILL_INFO tmBossKillInfo;
		util::zero(&tmBossKillInfo, sizeof(tmBossKillInfo));
		memcpy(&tmBossKillInfo, &*itrBossKillInfo, sizeof(tmBossKillInfo));
		m_mtvectTriggerOutPostMonsterKillInfo.push_back(tmBossKillInfo);
	}
}

void CTriggerFunctionNGOutPost::CreateAllMonster()
{
	for(int i=0; i<m_vectorTriggerNGCOutPostSummonMonsterGroup.size(); i++)
	{
		CTriggerNGCInflWarMonsterGroup *pNGCOutPostSummonMonsterGroup = m_vectorTriggerNGCOutPostSummonMonsterGroup[i];
		pNGCOutPostSummonMonsterGroup->CreateMonster();
	}

	CreateCrystalMonster();
	m_bIsBossDestroy = FALSE;
	m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_OUTPOST_1ST_VALUE_BOSS_PROTECTIVE_FILM_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_OUTPOST_1ST_VALUE_BOSS_PROTECTIVE_FILM_OBJECT_NUM_CHANGE);
}

void CTriggerFunctionNGOutPost::CreateCrystalMonster()
{
	vectorTriggerOutPostBossKillInfo::iterator itrBossKillInfo = m_vectTriggerOutPostBossKillInfo.begin();
	for (; itrBossKillInfo != m_vectTriggerOutPostBossKillInfo.end() ; itrBossKillInfo++ )
	{
		if( NULL != m_pFieldMapChannel )
		{
			INIT_MSG_WITH_BUFFER(MSG_FN_ADMIN_SUMMON_MONSTER, T_FN_ADMIN_SUMMON_MONSTER, pMsgSummonMonster, SendBuf);
			pMsgSummonMonster->ChannelIndex			= m_pFieldMapChannel->GetMapChannelIndex().ChannelIndex;
			util::strncpy(pMsgSummonMonster->CharacterName, "CrystalMonster", SIZE_MAX_CHARACTER_NAME);
			pMsgSummonMonster->MonsterUnitKind		= itrBossKillInfo->CrystalNum;
			pMsgSummonMonster->MonsterTargetType1	= MONSTER_TARGETTYPE_NORMAL;
			pMsgSummonMonster->TargetTypeData1		= 0;
			pMsgSummonMonster->CltIdxForTargetType1	= 0;
			pMsgSummonMonster->NumOfMonster			= 1;
			pMsgSummonMonster->Position				= AVECTOR3(0, 0, 0);
			pMsgSummonMonster->MonsterBalanceData.Init(0);
			m_pFieldMapChannel->Send2NPCServerW(SendBuf, MSG_SIZE(MSG_FN_ADMIN_SUMMON_MONSTER));
		}
	}
	m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, STRMSG_130121_0002);
	m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, STRMSG_130121_0004);
	if ( NULL != m_pFieldNextOutPostMapChannel )
	{
		m_pFieldNextOutPostMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, STRMSG_130121_0002);
		m_pFieldNextOutPostMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, STRMSG_130121_0004);
	}
}

void CTriggerFunctionNGOutPost::DestroyAllMonster()
{
	for(int i=0; i<m_vectorTriggerNGCOutPostSummonMonsterGroup.size(); i++)
	{
		CTriggerNGCInflWarMonsterGroup *pNGCOutPostSummonMonsterGroup = m_vectorTriggerNGCOutPostSummonMonsterGroup[i];

		pNGCOutPostSummonMonsterGroup->DestroyMonster();
	}

	DestroyCrystalMonster();
	m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_OUTPOST_1ST_VALUE_BOSS_PROTECTIVE_FILM_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_OUTPOST_1ST_VALUE_BOSS_PROTECTIVE_FILM_OBJECT_NUM_DEFAULT);
}

void CTriggerFunctionNGOutPost::DestroyCrystalMonster()
{
	vectorTriggerOutPostBossKillInfo::iterator itrBossKillInfo = m_vectTriggerOutPostBossKillInfo.begin();
	for (; itrBossKillInfo != m_vectTriggerOutPostBossKillInfo.end() ; itrBossKillInfo++ )
	{
		if( NULL != m_pFieldMapChannel )
		{
			INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_CINEMA_MONSTER_DESTROY, T_FN_NPCSERVER_CINEMA_MONSTER_DESTROY, pDestroy, SendBufDestroy);
			pDestroy->mapChann			= m_pFieldMapChannel->GetMapChannelIndex();
			pDestroy->MonsterUnitKind	= itrBossKillInfo->CrystalNum;
			((CFieldIOCP*)(m_pFieldMapChannel->m_pFieldMapWorkspace->m_pIOCPServer))->Send2NPCServerByTCP(SendBufDestroy, MSG_SIZE(MSG_FN_NPCSERVER_CINEMA_MONSTER_DESTROY));
		}
	}
}

BYTE CTriggerFunctionNGOutPost::FindWinInfluenc(MonIdx_t i_nMonsterNum)
{
	BOOL bWinBCU = FALSE;
	float fBCUDamage = 0.0f;
	float fANIDamage = 0.0f;

	mt_auto_lock mta(&m_mtvectTriggerAttackUserDamage);

	for(int a=0; a<m_mtvectTriggerAttackUserDamage.size(); a++)
	{
		if( m_mtvectTriggerAttackUserDamage[a].MonsterIdx == i_nMonsterNum )
		{
			if( TRUE == IS_VCN_INFLUENCE_TYPE(m_mtvectTriggerAttackUserDamage[a].InfluenceType) )
			{
				fBCUDamage += m_mtvectTriggerAttackUserDamage[a].fDamage;
			}

			if( TRUE == IS_ANI_INFLUENCE_TYPE(m_mtvectTriggerAttackUserDamage[a].InfluenceType) )
			{
				fANIDamage += m_mtvectTriggerAttackUserDamage[a].fDamage;
			}
			m_mtvectTriggerAttackUserDamage[a].fDamage = 0.0f;		// ����� �ջ��� ������ ���� ������� �ʱ�ȭ ���ش�.
		}
	}

	if( fBCUDamage == fANIDamage )		// ���� ����?
	{
		if( ((int)fBCUDamage % 2) == 0 )		// ������ ������ �̱似��
		{
			bWinBCU = TRUE;
		}
	}
	else
	{
		if( fBCUDamage > fANIDamage )
		{
			bWinBCU = TRUE;
		}
	}
#ifdef _DEBUG
	char szString[256] = { 0, };
	if( TRUE == bWinBCU )
	{
		sprintf(szString, "\\y[NGC �������� �ý���]\\y �̱� ���� [BCU] �й� ���� [ANI]");
	}
	else
	{
		sprintf(szString, "\\y[NGC �������� �ý���]\\y �̱� ���� [ANI] �й� ���� [BCU]");
	}
	//m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, szString);		
#endif

	if( TRUE == bWinBCU )
	{
		return INFLUENCE_TYPE_VCN;
	}
	else
	{
		return INFLUENCE_TYPE_ANI;
	}

}



void CTriggerFunctionNGOutPost::OnDestroy()
{
	for (auto x : m_vectorTriggerNGCOutPostSummonMonsterGroup) delete x;
	
	CTriggerFunction::OnDestroy();
}
