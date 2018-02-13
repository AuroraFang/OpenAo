// InfinityDefence.cpp: implementation of the CInfinityDefence class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InfinityDefence.h"
#include "FieldGlobal.h"
#include "FieldIOCPSocket.h"
#include "InfinityTickManager.h"
#include "FieldMapChannel.h"
#include "AtumLogSender.h"				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInfinityDefence::CInfinityDefence()
{
	this->InitInfinityDefence();
}

CInfinityDefence::~CInfinityDefence()
{

}

void CInfinityDefence::InitInfinityDefence() {

}

void CInfinityDefence::DoSecondlyWorkInfinity(ATUM_DATE_TIME *pDateTime) 
{
	if(FALSE == this->DoSecondlyWorkInfinityBaseIsContinue()) 
	{
		return;
	}

	DWORD dwCurTick = timeGetTime();
	DWORD dwPassTick = dwCurTick - m_InfinityCreatedTick;	
	if(dwPassTick > this->m_InfinityModeInfo.LimitTime)
	{// 2010-03-29 by cmkwon, ���ѽð��� ��� �Ǹ� ���� ó��
		this->Fin(TRUE, INFINITY_FIN_SUCCESS_BY_TIMEOVER);
		return;
	}

	if(FALSE == this->CheckEmptyMonster_AliveForGameClearAlive()) 
	{// 2010-03-29 by cmkwon, ���� ���� �ı��Ǹ� ���� ó��
		// ���� ����
		this->Fin(FALSE, INFINITY_FIN_FAIL_BY_DEAD_KEYMONSTER);
		return;
	}

	///////////////////////////////////////////////////////////////////////////////

	// 2010-04-09 by cmkwon, ����2��(���� ���� �߰�) - 
	if( 0 >= m_CinemaInfoList.size() )
	{
		if( TRUE == this->CheckEmptyMonster_DeadForNextStepAlive() )
		{
			this->Fin(TRUE, INFINITY_FIN_SUCCESS_BY_DEAD_ALL_KEY_MONSTER_FOR_NEXTSTEP);
			return;
		}
	}

	this->ProcessingCinema(dwCurTick, dwPassTick);
}


/**************************************************************************************************************************
**
**	��Ƽ�Ǵ� ������� - �ó׸� ���� ó��.
**
**	Create Info :	2010-03-31 by dhjin
**
**	Update Info :	2010. 05. 19 by hsLee - ���� �뷱�� ���� ����.
**
**					2010. 05. 31 by hsLee - ���� �߰� Count�� '0'�� �� �� ������ ����ó�� �߰�.
**
***************************************************************************************************************************/
void CInfinityDefence::ProcessingCinema(DWORD i_dwCurTick, DWORD i_dwPassTick) 
{	// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - ���� ����
	vectorCinemaInfo::iterator itrCinema = m_CinemaInfoList.begin();
	while(itrCinema != m_CinemaInfoList.end()) 
	{
		// �ó׸� üũ
		if(i_dwPassTick >= itrCinema->StartTime) 
		{		// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - 
			// �ó׸� �ɷȾ�~!! ���� ó�� �� �����
			INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_CINEMA, T_FC_INFINITY_CINEMA, pSMsg, SendBuf);
			*pSMsg = *itrCinema;	
				this->SendAddDataPlayerList(SendBuf, MSG_SIZE(MSG_FC_INFINITY_CINEMA));
			
			if ( itrCinema->ChangeObjectIdx )
			{
				INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_NEW_CHANGE_OBJECT, T_FN_NPCSERVER_NEW_CHANGE_OBJECT, pChObjMsg, ChObjSendBuf);
				pChObjMsg->mapChann			= m_pInfinityMapChannel->GetMapChannelIndex();
				pChObjMsg->DeleteObjectUID	= itrCinema->ObjectIdx;
				pChObjMsg->NewObjectNum		= itrCinema->ChangeObjectIdx;
				((CFieldIOCP*)(m_pInfinityMapChannel->m_pFieldMapWorkspace->m_pIOCPServer))->Send2NPCServerByTCP(ChObjSendBuf, MSG_SIZE(MSG_FN_NPCSERVER_NEW_CHANGE_OBJECT));
			}

			if( itrCinema->MonsterIdx ) 
			{
				// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - // ���� ��ȯ
				INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE, T_FN_NPCSERVER_CINEMA_MONSTER_CREATE, pSummon, SendBuf2);
				pSummon->mapChann			= m_pInfinityMapChannel->GetMapChannelIndex();
				pSummon->MonsterUnitKind	= itrCinema->MonsterIdx;

				// 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.) - ���� �뷱�� �Ʊ� ���� ���� ����ó��.
				bool bDefaultBalance = false;
				bool bKey_BossMonster = false;

				// 2010. 06. 14 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (�Ʊ� ���� �뷱�� ���Ѽ� ����.) - ������� ���� �Ʊ� ���� �뷱�� ����ó��.
				bool bLimitBalance = false;

				MONSTER_INFO *pMonsterInfo = ms_pFieldIOCP4->GetMonsterInfo ( itrCinema->MonsterIdx );
				
 				if ( ms_pFieldIOCP4 )
 				{
 					pMonsterInfo = ms_pFieldIOCP4->GetMonsterInfo ( itrCinema->MonsterIdx );
 					
					// 2010. 06. 14 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (�Ʊ� ���� �뷱�� ���Ѽ� ����.) - ������� ���� �Ʊ� ���� �뷱�� ����ó��.
					if ( pMonsterInfo && pMonsterInfo->Belligerence == BELL_INFINITY_DEFENSE_MONSTER )
						bLimitBalance = true;

//  					if ( pMonsterInfo && pMonsterInfo->Belligerence != BELL_INFINITY_DEFENSE_MONSTER )
// 						bDefaultBalance = false;
 				 
 					if ( COMPARE_MPOPTION_BIT(pMonsterInfo->MPOption, MPOPTION_BIT_KEY_MONSTER_ALIVE_FOR_GAMECLEAR) )
 						bKey_BossMonster = true;
 				}
				// 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.) - ���� �뷱�� �Ʊ� ���� ���� ����ó��.

				// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ���� �뷱�� ���� �߰�.
				// 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.) - ���� ��ȯ ī��Ʈ '0'�� �ɼ� ���� ����ó��.
				if ( bDefaultBalance || bKey_BossMonster )
					pSummon->MonsterSummonCount = itrCinema->SummonCount;
				else
					pSummon->MonsterSummonCount	= itrCinema->SummonCount * this->m_InfinityDifficultyData.fSummonCountRatio;
				
				pSummon->MonsterSummonCount = max ( pSummon->MonsterSummonCount , 1 );
				// End 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.)
				// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ���� �뷱�� ���� �߰�.

				pSummon->MaxRandomDistance	= itrCinema->SummonRandomPos;

				// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ���� �뷱�� ���� �߰�.
				// 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.) - ���� �뷱�� �Ʊ� ���� ���� ����ó��.
				if ( bDefaultBalance )
				{
					pSummon->MonsterBalanceInfo.Init(0);
				}
				else
				{
					// 2010. 06. 14 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (�Ʊ� ���� �뷱�� ���Ѽ� ����.) - ������� ���� �Ʊ� ���� �뷱�� ����ó��.
					if ( bLimitBalance && this->m_InfinityDifficultyLevel > DEFAULT_LEVEL_INFINITY_DIFFICULTY )
					{
						if ( ms_pFieldIOCP4 )
						{
							MONSTER_BALANCE_DATA sUpdateBalanceData ( DEFAULT_LEVEL_INFINITY_DIFFICULTY );

							const INFINITY_DIFFICULTY_BONUS_INFO *a_pInfinity_BonusInfo = ms_pFieldIOCP4->m_InfinityManager.Get_Difficulty_BonusInfo( sUpdateBalanceData.DifficultyStep );
							const INFINITY_DIFFICULTY_MONSTER_SETTING_INFO *a_pInfinity_MonsterInfo = ms_pFieldIOCP4->m_InfinityManager.Get_Difficulty_MonsterInfo ( sUpdateBalanceData.DifficultyStep );

							if ( a_pInfinity_BonusInfo )
								sUpdateBalanceData += *a_pInfinity_BonusInfo;

							if ( a_pInfinity_MonsterInfo )
								sUpdateBalanceData += *a_pInfinity_MonsterInfo;

							pSummon->MonsterBalanceInfo = sUpdateBalanceData;
						}
						else
						{
							pSummon->MonsterBalanceInfo.Init(0);
						}
					}
					else
					{
						pSummon->MonsterBalanceInfo = this->m_InfinityDifficultyData;
					}
				}
				// End 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.)
				// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ���� �뷱�� ���� �߰�.

				((CFieldIOCP*)(m_pInfinityMapChannel->m_pFieldMapWorkspace->m_pIOCPServer))->Send2NPCServerByTCP(SendBuf2, MSG_SIZE(MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE));
			}

			this->SetDeleteCinemaInfo(&*itrCinema);		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��
			CAtumLogSender::SendLogMessageInfinityCinema(&*itrCinema, this->GetInfinityCreateUID());			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� ���� �α�
			itrCinema = m_CinemaInfoList.erase(itrCinema);

			continue;
		}
		break;
	}
	
}


// 2010. 06. 04 by hsLee ��Ƽ�Ǵ� �ʵ� 2�� ���̵� ����. (GM ��ɾ� �߰�. /nextscene(���� �ó׸� �� ȣ��.) )
/**********************************************************************************************
**
**	��Ƽ�Ǵ� ���� �� �ѱ�� ó��.
**
**	Create Info : 2010. 06. 04 by hsLee
**
**	Update Info : ������ Scene Count ���ڷ� �߰�. 'nUpdateScene' 2010. 08. 25. by hsLee.
**
***********************************************************************************************/
void CInfinityDefence :: ProcessingCinema ( CFieldIOCPSocket * i_pMasterFISoc , int nUpdateScene /*= 1*/ )
{

	if ( NULL == i_pMasterFISoc || this->GetMasterPlayerClientIdx() != i_pMasterFISoc->m_character.ClientIndex )
		return;

	nUpdateScene = max ( nUpdateScene , 1 );

	// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - ���� ����
	vectorCinemaInfo::iterator itrCinema = m_CinemaInfoList.begin();

	int nExquteCnt = 0;

	while ( itrCinema != m_CinemaInfoList.end() )
	{	// �ó׸� üũ

		// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - 

		// �ó׸� �ɷȾ�~!! ���� ó�� �� �����
		INIT_MSG_WITH_BUFFER ( MSG_FC_INFINITY_CINEMA , T_FC_INFINITY_CINEMA , pSMsg , SendBuf );
			*pSMsg = *itrCinema;

		this->SendAddDataPlayerList ( SendBuf , MSG_SIZE( MSG_FC_INFINITY_CINEMA ) );
		
		if ( itrCinema->ChangeObjectIdx )
		{
			INIT_MSG_WITH_BUFFER ( MSG_FN_NPCSERVER_NEW_CHANGE_OBJECT , T_FN_NPCSERVER_NEW_CHANGE_OBJECT , pChObjMsg , ChObjSendBuf );
				pChObjMsg->mapChann			= m_pInfinityMapChannel->GetMapChannelIndex();
				pChObjMsg->DeleteObjectUID	= itrCinema->ObjectIdx;
				pChObjMsg->NewObjectNum		= itrCinema->ChangeObjectIdx;

			((CFieldIOCP*)(m_pInfinityMapChannel->m_pFieldMapWorkspace->m_pIOCPServer))->Send2NPCServerByTCP ( ChObjSendBuf , MSG_SIZE(MSG_FN_NPCSERVER_NEW_CHANGE_OBJECT) );
		}
		
		if ( itrCinema->MonsterIdx ) 
		{
			// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - // ���� ��ȯ
			INIT_MSG_WITH_BUFFER ( MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE , T_FN_NPCSERVER_CINEMA_MONSTER_CREATE , pSummon , SendBuf2 );
				pSummon->mapChann			= m_pInfinityMapChannel->GetMapChannelIndex();
				pSummon->MonsterUnitKind	= itrCinema->MonsterIdx;
			
			// 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.) - ���� �뷱�� �Ʊ� ���� ���� ����ó��.
			bool bDefaultBalance = false;
			bool bKey_BossMonster = false;

			// 2010. 06. 14 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (�Ʊ� ���� �뷱�� ���Ѽ� ����.) - ������� ���� �Ʊ� ���� �뷱�� ����ó��.
			bool bLimitBalance = false;
			
			// 2010. 06. 08 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (�Ʊ� ���� �뷱�� ����.) - ���� ���� ó���� �κ� ����.
 			if ( ms_pFieldIOCP4 )
 			{
 				MONSTER_INFO *pMonsterInfo = ms_pFieldIOCP4->GetMonsterInfo ( itrCinema->MonsterIdx );
 				
// 				if ( pMonsterInfo && pMonsterInfo->Belligerence != BELL_INFINITY_DEFENSE_MONSTER )
// 					bDefaultBalance = false;

				// 2010. 06. 14 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (�Ʊ� ���� �뷱�� ���Ѽ� ����.) - ������� ���� �Ʊ� ���� �뷱�� ����ó��.
				if ( pMonsterInfo && pMonsterInfo->Belligerence == BELL_INFINITY_DEFENSE_MONSTER )
					bLimitBalance = true;

 				if ( COMPARE_MPOPTION_BIT(pMonsterInfo->MPOption, MPOPTION_BIT_KEY_MONSTER_ALIVE_FOR_GAMECLEAR) )
 					bKey_BossMonster = true;
 			}

			// 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.) - ���� �뷱�� �Ʊ� ���� ���� ����ó��.
			
			// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ���� �뷱�� ���� �߰�.
			// 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.) - ���� ��ȯ ī��Ʈ '0'�� �ɼ� ���� ����ó��.
			if ( bDefaultBalance || bKey_BossMonster )
				pSummon->MonsterSummonCount = itrCinema->SummonCount;
			else
				pSummon->MonsterSummonCount	= itrCinema->SummonCount * this->m_InfinityDifficultyData.fSummonCountRatio;
			
			pSummon->MonsterSummonCount = max ( pSummon->MonsterSummonCount , 1 );
			// End 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.)
			// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ���� �뷱�� ���� �߰�.
			
			pSummon->MaxRandomDistance	= itrCinema->SummonRandomPos;
			
			// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ���� �뷱�� ���� �߰�.
			// 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.) - ���� �뷱�� �Ʊ� ���� ���� ����ó��.
			if ( bDefaultBalance )
				pSummon->MonsterBalanceInfo.Init(0);
			else
			{

				// 2010. 06. 14 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (�Ʊ� ���� �뷱�� ���Ѽ� ����.) - ������� ���� �Ʊ� ���� �뷱�� ����ó��.
				if ( bLimitBalance && this->m_InfinityDifficultyLevel > DEFAULT_LEVEL_INFINITY_DIFFICULTY )
				{
					if ( ms_pFieldIOCP4 )
					{
						MONSTER_BALANCE_DATA sUpdateBalanceData ( DEFAULT_LEVEL_INFINITY_DIFFICULTY );
						
						const INFINITY_DIFFICULTY_BONUS_INFO *a_pInfinity_BonusInfo = ms_pFieldIOCP4->m_InfinityManager.Get_Difficulty_BonusInfo( sUpdateBalanceData.DifficultyStep );
						const INFINITY_DIFFICULTY_MONSTER_SETTING_INFO *a_pInfinity_MonsterInfo = ms_pFieldIOCP4->m_InfinityManager.Get_Difficulty_MonsterInfo ( sUpdateBalanceData.DifficultyStep );
						
						if ( a_pInfinity_BonusInfo )
							sUpdateBalanceData += *a_pInfinity_BonusInfo;
						
						if ( a_pInfinity_MonsterInfo )
							sUpdateBalanceData += *a_pInfinity_MonsterInfo;
						
						pSummon->MonsterBalanceInfo = sUpdateBalanceData;
					}
					else
					{
						pSummon->MonsterBalanceInfo.Init(0);
					}
				}
				else
				{
					pSummon->MonsterBalanceInfo = this->m_InfinityDifficultyData;
				}
			}
			// End 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.)
			// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ���� �뷱�� ���� �߰�.
			
			((CFieldIOCP*)(m_pInfinityMapChannel->m_pFieldMapWorkspace->m_pIOCPServer))->Send2NPCServerByTCP ( SendBuf2 , MSG_SIZE ( MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE ) );
		}
		
		this->SetDeleteCinemaInfo ( &*itrCinema );		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��

		CAtumLogSender::SendLogMessageInfinityCinema( &*itrCinema , this->GetInfinityCreateUID() );			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� ���� �α�

		itrCinema = m_CinemaInfoList.erase(itrCinema);

		nExquteCnt++;

		if ( nExquteCnt >= nUpdateScene )
			break;
	}

}
// End 2010. 06. 04 by hsLee ��Ƽ�Ǵ� �ʵ� 2�� ���̵� ����. (GM ��ɾ� �߰�. /nextscene(���� �ó׸� �� ȣ��.) )