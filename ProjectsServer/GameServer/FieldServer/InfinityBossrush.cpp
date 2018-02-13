// InfinityBossrush.cpp: implementation of the CInfinityBossrush class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InfinityBossrush.h"
#include "FieldGlobal.h"
#include "FieldIOCPSocket.h"
#include "InfinityTickManager.h"
#include "FieldMapChannel.h"
#include "AtumLogSender.h"				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInfinityBossrush::CInfinityBossrush()
{
	this->InitInfinityBossrush();
}

CInfinityBossrush::~CInfinityBossrush()
{	

}

void CInfinityBossrush::InitInfinityBossrush() 
{
	m_CheckDelayForcedCreateMonsterCount = 0;
}

void CInfinityBossrush::DoSecondlyWorkInfinity(ATUM_DATE_TIME *pDateTime) {
	
	// 2009-09-09 ~ 2010-01-15 by dhjin, ���Ǵ�Ƽ - ���� ���� ���� �Ǵ� �Ŀ��� ���� �����ϰ� ����. this->TenderProcessing(); ������ �ű�
// 	if(INFINITY_STATE_PLAYING != m_eInfinityState) {
// 		return;
// 	}

	// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - �ذ� ���� ����
// 	// ���� üũ
// 	this->TenderProcessing();
// 
// 	if(INFINITY_STATE_PLAYING != m_eInfinityState) {
// 		// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -
// 		if(INFINITY_STATE_DONE_SUCCESS == m_eInfinityState) {
// 			// ���� ���� ���� ó��
// 			this->ProcessingCinemaFinSuccess();
// 		}
// 		else if(INFINITY_STATE_DONE_FAIL == m_eInfinityState) {
// 			// ���� ���� ���� ó��
// 			this->ProcessingCinemaFinFail();
// 		}
// 		return;
// 	}
	if(FALSE == this->DoSecondlyWorkInfinityBaseIsContinue()) 
	{
		return;
	}

	if(0 < m_CheckDelayForcedCreateMonsterCount)
	{
		m_CheckDelayForcedCreateMonsterCount--;
	}

	DWORD dwCurTick = timeGetTime();
	DWORD dwPassTick = dwCurTick - m_InfinityCreatedTick;
	
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ����� �г�Ƽ �߰�, �ذ� ���� ����
//	if(dwPassTick > this->m_InfinityModeInfo.LimitTime) {
	if(dwPassTick > this->m_InfinityModeInfo.LimitTime + m_InfinityLimitTick) {
		// ���� ����
		this->Fin(FALSE, INFINITY_FIN_FAIL_BY_TIMEOVER);
		return;
	}

	if(0 >= m_CinemaInfoList.size()
		&& TRUE == this->CheckEmptyMonster_DeadForNextStepAlive()) {
		// ���� ����
		this->Fin(TRUE, INFINITY_FIN_SUCCESS_BY_PASS_ALL_STEP);
		return;
	}

	if(0 >= m_CinemaInfoList.size()) {
		return;
	}

	this->ProcessingCinema(dwCurTick, dwPassTick);
}

void CInfinityBossrush::ProcessingCinema(DWORD i_dwCurTick, DWORD i_dwPassTick) 
{	// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - ���� ����

	vectorCinemaInfo::iterator itrCinema = m_CinemaInfoList.begin();
	while(itrCinema != m_CinemaInfoList.end())
	{
		CINEMAINFO *pCinemaInfo = &*itrCinema;

		if(TICK_FIRST_FORCED_CREATE_MONSTER_TERM < i_dwPassTick
			&& 0 == m_CheckDelayForcedCreateMonsterCount  
			&& FALSE == this->CheckKeyMonster_DeadForNextStepAlive()
			&& pCinemaInfo->CinemaOrder != m_CinemaOrder )  
		{
//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CInfinityBossrush::ProcessingCinema# 10000 ! PassTick(%6d) Cinema<Num(%d) Order(%d) StartTime(%6d) m_CinemaOrder(%d) \r\n"
//				, m_nInfinityCreateUID, i_dwPassTick, pCinemaInfo->CinemaNum, pCinemaInfo->CinemaOrder, pCinemaInfo->StartTime, m_CinemaOrder);

			// Ű �� ����. ��ȯ�ϱ� ���� ����
			m_CinemaOrder	= pCinemaInfo->CinemaOrder;
			i_dwPassTick	= pCinemaInfo->StartTime;		// �������� ���� ��ȯ�� �ִٸ� �ð� �񱳰��� ������ �ٲ� ��ȯ �����ϰ� �Ѵ�.
		}
		
		if(pCinemaInfo->CinemaOrder != m_CinemaOrder )
		{
			return;
		}

		// �ó׸� üũ
		if(i_dwPassTick >= pCinemaInfo->StartTime)
		{// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - 

//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CInfinityBossrush::ProcessingCinema# 20000 ! PassTick(%6d) Cinema<Num(%d) Order(%d) StartTime(%6d) m_CinemaOrder(%d) \r\n"
//				, m_nInfinityCreateUID, i_dwPassTick, pCinemaInfo->CinemaNum, pCinemaInfo->CinemaOrder, pCinemaInfo->StartTime, m_CinemaOrder);

			// �ó׸� �ɷȾ�~!! ���� ó�� �� �����
			INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_CINEMA, T_FC_INFINITY_CINEMA, pSMsg, SendBuf);
			*pSMsg = *pCinemaInfo;	
			this->SendAddDataPlayerList(SendBuf, MSG_SIZE(MSG_FC_INFINITY_CINEMA));

			if(pCinemaInfo->ChangeObjectIdx)
			{
				INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_NEW_CHANGE_OBJECT, T_FN_NPCSERVER_NEW_CHANGE_OBJECT, pChObjMsg, ChObjSendBuf);
				pChObjMsg->mapChann			= m_pInfinityMapChannel->GetMapChannelIndex();
				pChObjMsg->DeleteObjectUID	= pCinemaInfo->ObjectIdx;
				pChObjMsg->NewObjectNum		= pCinemaInfo->ChangeObjectIdx;
				ms_pFieldIOCP4->Send2NPCServerByTCP(ChObjSendBuf, MSG_SIZE(MSG_FN_NPCSERVER_NEW_CHANGE_OBJECT));
			}
			if(pCinemaInfo->MonsterIdx)
			{
				// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - // ���� ��ȯ
				INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE, T_FN_NPCSERVER_CINEMA_MONSTER_CREATE, pSummon, SendBuf2);
				pSummon->mapChann			= m_pInfinityMapChannel->GetMapChannelIndex();
				pSummon->MonsterUnitKind	= pCinemaInfo->MonsterIdx;

				// 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.) - ���� �뷱�� �Ʊ� ���� ���� ����ó��.
				bool bDefaultBalance = false;
				bool bKey_BossMonster = false;
				
				if ( ms_pFieldIOCP4 )
				{
					MONSTER_INFO *pMonsterInfo = ms_pFieldIOCP4->GetMonsterInfo ( itrCinema->MonsterIdx );
					
// 					if ( pMonsterInfo && pMonsterInfo->Belligerence != BELL_INFINITY_DEFENSE_MONSTER )
// 						bDefaultBalance = false;

					// 2010. 06. 10 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (�������� ���� ��ü �� ����ó��.)
					if ( NULL != GetKeyMonsterInfoFromKeyMonsterDeadForNextStep( pMonsterInfo->MonsterUnitKind ) 
						|| COMPARE_MPOPTION_BIT(pMonsterInfo->MPOption, MPOPTION_BIT_KEY_MONSTER_ALIVE_FOR_GAMECLEAR) )
					{
						bKey_BossMonster = true;
					}

				}
				// End 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.)

				// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ���� �뷱�� ���� �߰�.
				// 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.) - ���� �뷱�� �Ʊ� ���� ���� ����ó��.
				if ( bDefaultBalance || bKey_BossMonster )
					pSummon->MonsterSummonCount = pCinemaInfo->SummonCount;
				else
					pSummon->MonsterSummonCount	= pCinemaInfo->SummonCount * ( this->m_InfinityDifficultyData.fSummonCountRatio <= 0.0f ? 1.0f : this->m_InfinityDifficultyData.fSummonCountRatio );

				pSummon->MonsterSummonCount = max ( pSummon->MonsterSummonCount , 1 );
				// End 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.) - ���� �뷱�� �Ʊ� ���� ���� ����ó��.
				// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ���� �뷱�� ���� �߰�.

				pSummon->MaxRandomDistance	= pCinemaInfo->SummonRandomPos;

				// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ���� �뷱�� ���� �߰�.
				// 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.) - ���� �뷱�� �Ʊ� ���� ���� ����ó��.
				if ( bDefaultBalance )
					pSummon->MonsterBalanceInfo.Init(0);
				else
					pSummon->MonsterBalanceInfo	= this->m_InfinityDifficultyData;
				// End 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.)
				// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ���� �뷱�� ���� �߰�.
							
				ms_pFieldIOCP4->Send2NPCServerByTCP(SendBuf2, MSG_SIZE(MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE));
			}

			this->SetDeleteCinemaInfo(pCinemaInfo);		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��
			CAtumLogSender::SendLogMessageInfinityCinema(pCinemaInfo, this->GetInfinityCreateUID());			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� ���� �α�
			itrCinema = m_CinemaInfoList.erase(itrCinema);	
			
//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CInfinityBossrush::ProcessingCinema# 21000 ! PassTick(%6d) Cinema<Num(%d) Order(%d) StartTime(%6d) m_CinemaOrder(%d) \r\n"
//				, m_nInfinityCreateUID, i_dwPassTick, itrCinema->CinemaNum, itrCinema->CinemaOrder, itrCinema->StartTime, m_CinemaOrder);

			if(FALSE != m_CinemaOrder
				&& 0 < m_CinemaInfoList.size())
			{
				if(m_CinemaOrder != itrCinema->CinemaOrder)
				{
					// ���� ���� ��ȯ�� ���ٸ� ���� ��ȯ ���� ���� ���� ����
					m_CheckDelayForcedCreateMonsterCount = COUNT_DELAY_FORCED_CREATE_MONSTER_TERM;
					return;	// ���� �ܰ��̱� ������ �� üũ���� �ʰ� ���� ��Ų��.

				}
			}
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
void CInfinityBossrush :: ProcessingCinema ( CFieldIOCPSocket * i_MasterFISoc , int nUpdateScene /*= 1*/ )
{

	if ( NULL == i_MasterFISoc || this->GetMasterPlayerClientIdx() != i_MasterFISoc->m_character.ClientIndex )
		return;

	nUpdateScene = max ( nUpdateScene , 1 );

	vectorCinemaInfo::iterator itrCinema = m_CinemaInfoList.begin();

	int nExquteCnt = 0;

	while ( itrCinema != m_CinemaInfoList.end() )
	{
		CINEMAINFO *pCinemaInfo = &*itrCinema;

		// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - 

		// �ó׸� �ɷȾ�~!! ���� ó�� �� �����
		INIT_MSG_WITH_BUFFER ( MSG_FC_INFINITY_CINEMA , T_FC_INFINITY_CINEMA , pSMsg , SendBuf );
			*pSMsg = *pCinemaInfo;

		this->SendAddDataPlayerList ( SendBuf , MSG_SIZE ( MSG_FC_INFINITY_CINEMA ) );

		if ( pCinemaInfo->ChangeObjectIdx )
		{
			INIT_MSG_WITH_BUFFER ( MSG_FN_NPCSERVER_NEW_CHANGE_OBJECT, T_FN_NPCSERVER_NEW_CHANGE_OBJECT, pChObjMsg, ChObjSendBuf );

			pChObjMsg->mapChann			= m_pInfinityMapChannel->GetMapChannelIndex();
			pChObjMsg->DeleteObjectUID	= pCinemaInfo->ObjectIdx;
			pChObjMsg->NewObjectNum		= pCinemaInfo->ChangeObjectIdx;

			ms_pFieldIOCP4->Send2NPCServerByTCP ( ChObjSendBuf , MSG_SIZE ( MSG_FN_NPCSERVER_NEW_CHANGE_OBJECT ) );
		}

		if ( pCinemaInfo->MonsterIdx )
		{
			// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - // ���� ��ȯ
			INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE, T_FN_NPCSERVER_CINEMA_MONSTER_CREATE, pSummon, SendBuf2);
				pSummon->mapChann			= m_pInfinityMapChannel->GetMapChannelIndex();
				pSummon->MonsterUnitKind	= pCinemaInfo->MonsterIdx;

			// 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.) - ���� �뷱�� �Ʊ� ���� ���� ����ó��.
			bool bDefaultBalance = false;
			bool bKey_BossMonster = false;
			
			// 2010. 06. 08 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (�Ʊ� ���� �뷱�� ����.)
 			if ( ms_pFieldIOCP4 )
 			{
 				MONSTER_INFO *pMonsterInfo = ms_pFieldIOCP4->GetMonsterInfo ( itrCinema->MonsterIdx );

// 				if ( pMonsterInfo && pMonsterInfo->Belligerence != BELL_INFINITY_DEFENSE_MONSTER )
// 					bDefaultBalance = false;

 				// 2010. 06. 10 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (�������� ���� ��ü �� ����ó��.)
				if ( NULL != GetKeyMonsterInfoFromKeyMonsterDeadForNextStep( pMonsterInfo->MonsterUnitKind ) 
					|| COMPARE_MPOPTION_BIT(pMonsterInfo->MPOption, MPOPTION_BIT_KEY_MONSTER_ALIVE_FOR_GAMECLEAR) )
				{
					bKey_BossMonster = true;
				}
 
 			}

			// End 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.)

			// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ���� �뷱�� ���� �߰�.
			// 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.) - ���� �뷱�� �Ʊ� ���� ���� ����ó��.
			if ( bDefaultBalance || bKey_BossMonster )
				pSummon->MonsterSummonCount = pCinemaInfo->SummonCount;
			else
				pSummon->MonsterSummonCount	= pCinemaInfo->SummonCount * ( this->m_InfinityDifficultyData.fSummonCountRatio <= 0.0f ? 1.0f : this->m_InfinityDifficultyData.fSummonCountRatio );

			pSummon->MonsterSummonCount = max ( pSummon->MonsterSummonCount , 1 );
			// End 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.) - ���� �뷱�� �Ʊ� ���� ���� ����ó��.
			// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ���� �뷱�� ���� �߰�.

			pSummon->MaxRandomDistance	= pCinemaInfo->SummonRandomPos;

			// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ���� �뷱�� ���� �߰�.
			// 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.) - ���� �뷱�� �Ʊ� ���� ���� ����ó��.
			if ( bDefaultBalance )
				pSummon->MonsterBalanceInfo.Init(0);
			else
				pSummon->MonsterBalanceInfo	= this->m_InfinityDifficultyData;
			// End 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.)
			// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ���� �뷱�� ���� �߰�.
						
			ms_pFieldIOCP4->Send2NPCServerByTCP ( SendBuf2 , MSG_SIZE( MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE ) );
		}

		this->SetDeleteCinemaInfo( pCinemaInfo );		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��

		CAtumLogSender::SendLogMessageInfinityCinema ( pCinemaInfo , this->GetInfinityCreateUID() );			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� ���� �α�

		itrCinema = m_CinemaInfoList.erase( itrCinema );

		nExquteCnt++;

		if ( nExquteCnt >= nUpdateScene )
			break;

	}

}

// End 2010. 06. 04 by hsLee ��Ƽ�Ǵ� �ʵ� 2�� ���̵� ����. (GM ��ɾ� �߰�. /nextscene(���� �ó׸� �� ȣ��.) )