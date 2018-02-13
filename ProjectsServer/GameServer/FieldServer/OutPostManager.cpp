// OutPostManager.cpp: implementation of the COutPostManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OutPostManager.h"
#include "OutPost.h"
#include "FieldIOCP.h"
#include "FieldGlobal.h"
#include "AtumLogSender.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COutPostManager::COutPostManager()
{
}

COutPostManager::~COutPostManager()
{
	mt_auto_lock mta(&m_mtvectOutPostManager);		// 2009-06-24 by cmkwon, Lock �߰���. - 

	for (auto x : m_mtvectOutPostManager) delete x;
	m_mtvectOutPostManager.clear();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::InitOutPostManager(CFieldIOCP * i_pFIOCP)
/// \brief		�ʱ�ȭ
/// \author		dhjin
/// \date		2007-08-16 ~ 2007-08-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::InitOutPostManager(CFieldIOCP * i_pFIOCP)
{
	m_pFieldIOCP18	= i_pFIOCP;
	m_mtvectOutPostManager.clearLock();		// 2009-06-24 by cmkwon, Lock �߰���. - 
	m_vectOutPostNextWarTimeInfo.clear();
}

COutPost * COutPostManager::GetOutPost(int i_nMapIndex)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);

	mtvectOutPost::iterator	itr = m_mtvectOutPostManager.begin();
	for(; itr != m_mtvectOutPostManager.end(); itr++)
	{
		if(i_nMapIndex == (*itr)->GetOutPostMapIndex()
			|| i_nMapIndex == (*itr)->GetOutPostCityMapIndex())		// 2007-09-28 by cmkwon, �߰���
		{
			return *itr;
		}
	}
	
	return NULL;
}

COutPost * COutPostManager::GetOutPostCityMap(int i_nCityMapIndex)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);

	mtvectOutPost::iterator	itr = m_mtvectOutPostManager.begin();
	for(; itr != m_mtvectOutPostManager.end(); itr++)
	{
		if(i_nCityMapIndex == (*itr)->GetOutPostCityMapIndex())
		{
			return *itr;
		}
	}
	
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::SetOutPostManagerByDBLoad(vectSOutPostInfo * i_pVectOutPostInfo)	// �������� ���� ������ DB�� ���� �о� �������� ���� �� ��ŭ Class OutPost�� �����Ͽ� m_mtvectSOutPost�� �߰��Ѵ�.
/// \brief		�������� ���� ������ DB�� ���� �о� �������� ���� �� ��ŭ Class OutPost�� �����Ͽ� m_mtvectSOutPost�� �߰��Ѵ�.
/// \author		dhjin
/// \date		2007-08-16 ~ 2007-08-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::SetOutPostManagerByDBLoad(vectSOutPostInfo * i_pVectOutPostInfo)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);

	// 2007-08-16 by dhjin, �������� ���� ��ŭ COutPost ����
	vectSOutPostInfo::iterator	itr = i_pVectOutPostInfo->begin();
	for(; itr != i_pVectOutPostInfo->end(); itr++)
	{
		COutPost * tmOutPost = new COutPost(&*itr, m_pFieldIOCP18);
		m_mtvectOutPostManager.push_back(tmOutPost);
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::SetOutPostNextWarTimeInfoByDBLoad(vectATUM_DATE_TIME * i_pvectOutPostNextWarTimeInfo)
/// \brief		�������� ���� �ð� ���� ������ DB���� �о�´�.
/// \author		dhjin
/// \date		2007-08-21 ~ 2007-08-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::SetOutPostNextWarTimeInfoByDBLoad(vectATUM_DATE_TIME * i_pvectOutPostNextWarTimeInfo)
{
	vectATUM_DATE_TIME::iterator	itr = i_pvectOutPostNextWarTimeInfo->begin();
	for(; itr != i_pvectOutPostNextWarTimeInfo->end(); itr++)
	{
		m_vectOutPostNextWarTimeInfo.push_back(*itr);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::OnDoMinutelyWorkOutPostManager(ATUM_DATE_TIME *i_pCurDateTime)
/// \brief		�� ���� �������� ���� �̺�Ʈ üũ
/// \author		dhjin
/// \date		2007-08-16 ~ 2007-08-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::OnDoMinutelyWorkOutPostManager(ATUM_DATE_TIME *i_pCurDateTime)
{
	// 2010-03-02 by cmkwon, ArenaServer�� �ʿ���� - 
	if(g_pFieldGlobal->IsArenaServer()){				return;}

	///////////////////////////////////////////////////////////////////////////////	
	// 2009-07-08 by cmkwon, ���� ������ ���� ���� - 
	vectSOUTPOSTWAR_EV_RESET_SUCCESS	tmVectEvResetSuccessList;
	vectSOUTPOSTWAR_EV_END				tmVectEvEndList;


	mt_auto_lock mta(&m_mtvectOutPostManager);
	
	mtvectOutPost::iterator	itr = m_mtvectOutPostManager.begin();
	for(; itr != m_mtvectOutPostManager.end(); itr++)
	{
		if((*itr)->CheckSetOutPostWarDay(i_pCurDateTime))
		{// 2007-09-03 by dhjin, �������� ���� �ð� ������ �ϴ� ���ڰ� �������� �����Ѵ�.
			continue;
		}
		
		// 2007-08-17 by dhjin, �������� ���� �߻��� �ð��� üũ�Ѵ�.
		int	CheckRemainWarTimeAtMin = (*itr)->CheckOutPostWarTime(i_pCurDateTime);
		
		if(30 == CheckRemainWarTimeAtMin || 10 == CheckRemainWarTimeAtMin || 5 == CheckRemainWarTimeAtMin)
		{// 2007-08-17 by dhjin, ������� 30�� ��, 10�� ��, 5���� ���� �ð��� Ŭ���̾�Ʈ�� �����Ѵ�.
			this->SendOutPostWarRemainTime(CheckRemainWarTimeAtMin, (*itr)->GetOutPostInfluence(), (*itr)->GetOutPostMapIndex());
			continue;
		}

		if(0 >= CheckRemainWarTimeAtMin && OUTPOST_NORMAL == (*itr)->GetOutPostState())
		{// 2007-08-17 by dhjin, �������~!!
			this->OutPostWarStart(*itr);
		}
		else if(0 > CheckRemainWarTimeAtMin && IS_OUTPOST( (*itr)->GetOutPostState()) )
		{// 2007-08-20 by dhjin, �������� �� ���̶�� üũ �ؾߵ� ����
			
			// 2007-08-20 by dhjin, �������� ���� �ϰ� �� ���� ���� �ð��� ���ϱ� ���� ������ ����� ��ȯ�Ѵ�.
			CheckRemainWarTimeAtMin = abs(CheckRemainWarTimeAtMin);
			
			if(OUTPOST_WARTIME <= CheckRemainWarTimeAtMin
				|| (g_pFieldGlobal->IsTestServer() && OUTPOST_WARTIME_FOR_TESTSERVER <= CheckRemainWarTimeAtMin))		// 2008-10-29 by cmkwon, �׼� �ý��� ����(���������� 1�ð�(60��)���� ����) - 
			{// 2007-08-20 by dhjin, �������� �� �ð��� OUTPOST_WARTIME �����ٸ� �������� ������ ������.
				// 2009-07-08 by cmkwon, ���� ������ ���� ���� - �����߰�(, vectSOUTPOSTWAR_EV_END *io_pEvEndList)
				this->OutPostWarEnd(*itr, &tmVectEvEndList);
			}

			// 2007-08-20 by dhjin, �������� ���� �ð� üũ
			int CheckPassageResetTime = (*itr)->CheckOutPostResetTime(i_pCurDateTime);
			
			if(0 == CheckPassageResetTime && IS_OUTPOST_RESETING((*itr)->GetOutPostState()) )
			{// 2007-08-20 by dhjin, ���� ����~!!

				// 2009-07-08 by cmkwon, ���� ������ ���� ���� - �����߰�(, vectSOUTPOSTWAR_EV_RESET_SUCCESS *io_pVectEvResetSuccessList)
				this->OutPostResetSuccess(*itr, &tmVectEvResetSuccessList);
			}

		}

	}
	mta.auto_unlock_cancel();	// 2009-07-08 by cmkwon, ���� ������ ���� ���� - deadlock �� ���ϱ� ���� ���⿡�� ������

	///////////////////////////////////////////////////////////////////////////////
	// 2009-07-08 by cmkwon, ���� ������ ���� ���� - 
	vectSOUTPOSTWAR_EV_RESET_SUCCESS::iterator evResetItr(tmVectEvResetSuccessList.begin());
	for(; evResetItr != tmVectEvResetSuccessList.end(); evResetItr++)
	{
		SOUTPOSTWAR_EV_RESET_SUCCESS *pReset = &*evResetItr;
		if(NULL == pReset->pFMChanOfOutPostWarMap)
		{
			continue;
		}

		// 2009-07-08 by cmkwon, ���� ������ ���� ���� - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] OutPostWarNoti COutPostManager::OnDoMinutelyWorkOutPostManager# EvResetSuccess OutpostWarMapIndex(%d) \r\n"
			, pReset->pFMChanOfOutPostWarMap->GetMapChannelIndex().MapIndex);

		pReset->pFMChanOfOutPostWarMap->WarpToCityMapFieldMapChannelByInfluence(pReset->bySuccessInfluenceTy);
	}
	vectSOUTPOSTWAR_EV_END::iterator evEndItr(tmVectEvEndList.begin());
	for(; evEndItr != tmVectEvEndList.end(); evEndItr++)
	{
		SOUTPOSTWAR_EV_END *pEnd = &*evEndItr;
		if(NULL == pEnd->pFMChanOfOutPostWarMap)
		{
			continue;
		}

		// 2009-07-08 by cmkwon, ���� ������ ���� ���� - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] OutPostWarNoti COutPostManager::OnDoMinutelyWorkOutPostManager# EvEnd OutpostWarMapIndex(%d) WinInfl(%d) LossInfl(%d) WinGuildUID(%ld) CityMapIndexOfOutpost(%d) \r\n"
			, pEnd->pFMChanOfOutPostWarMap->GetMapChannelIndex().MapIndex, pEnd->byWinInfluenceTy, pEnd->byLossInfluenceTy, pEnd->GuildUIDPossessOutPost, pEnd->nCityMapIndexForWarp);

		// 2009-07-08 by cmkwon, 1. WarPoint, ������ ���� ó��
		if(COMPARE_INFLUENCE(pEnd->byWinInfluenceTy, INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI))
		{			
			pEnd->pFMChanOfOutPostWarMap->AddWarPointInMap(OUTPOST_PAY_WARPOINT, FALSE, pEnd->byWinInfluenceTy, WPUT_OUTPOSTWAR);				// 2009-01-22 by cmkwon, ���� ���� ���� �α� ���� - CFieldMapChannel::AddWarPointInMap()�� ����(, BYTE i_byWPUpdateTy=WPUT_GENERAL) �߰�, // 2008-05-20 by dhjin, �¸� ���� ���� ���� ����. pQParam �� ����ϴ� �κ� ����
			pEnd->pFMChanOfOutPostWarMap->AddWarPointInMap(OUTPOST_PAY_WARPOINT_BY_LOSE, FALSE, pEnd->byLossInfluenceTy, WPUT_OUTPOSTWAR);	// 2009-01-22 by cmkwon, ���� ���� ���� �α� ���� - CFieldMapChannel::AddWarPointInMap()�� ����(, BYTE i_byWPUpdateTy=WPUT_GENERAL) �߰�, // 2008-01-07 by dhjin, ���� ���� ���� - 
			pEnd->pFMChanOfOutPostWarMap->InsertItemInMap(OUTPOST_PAY_ITEM, OUTPOST_PAY_ITEM_COUNT, pEnd->byWinInfluenceTy);	// 2008-05-20 by dhjin, �¸� ���� ���� ���� ����. pQParam �� ����ϴ� �κ� ���� // 2007-11-06 by dhjin, ������ ���� �߰� 

			CFieldMapChannel		*pBackFMChann = m_pFieldIOCP18->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(pEnd->pFMChanOfOutPostWarMap->m_pFieldMapProject->m_nBeforeMapIndex, 0));
			if(pBackFMChann)
			{
				pBackFMChann->AddWarPointInMap(OUTPOST_PAY_WARPOINT, FALSE, pEnd->byWinInfluenceTy, WPUT_OUTPOSTWAR);				// 2009-01-22 by cmkwon, ���� ���� ���� �α� ���� - CFieldMapChannel::AddWarPointInMap()�� ����(, BYTE i_byWPUpdateTy=WPUT_GENERAL) �߰�// 2008-05-20 by dhjin, �¸� ���� ���� ���� ����. pQParam �� ����ϴ� �κ� ����
				pBackFMChann->AddWarPointInMap(OUTPOST_PAY_WARPOINT_BY_LOSE, FALSE, pEnd->byLossInfluenceTy, WPUT_OUTPOSTWAR);	// 2009-01-22 by cmkwon, ���� ���� ���� �α� ���� - CFieldMapChannel::AddWarPointInMap()�� ����(, BYTE i_byWPUpdateTy=WPUT_GENERAL) �߰�	// 2008-01-07 by dhjin, ���� ���� ���� - 
				pBackFMChann->InsertItemInMap(OUTPOST_PAY_ITEM, OUTPOST_PAY_ITEM_COUNT, pEnd->byWinInfluenceTy);	// 2008-05-20 by dhjin, �¸� ���� ���� ���� ����. pQParam �� ����ϴ� �κ� ����	// 2007-11-06 by dhjin, ������ ���� �߰� 
			}

			// 2008-06-20 by cmkwon, EP3 ��� �ý��� ����(�ʴ� ����� 2�� ���� �� �� �ִ�) - ���������� ����� ���2 ���� ����
			pBackFMChann = m_pFieldIOCP18->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(pEnd->pFMChanOfOutPostWarMap->m_pFieldMapProject->m_nBeforeMapIndex2, 0));
			if(pBackFMChann)
			{
				pBackFMChann->AddWarPointInMap(OUTPOST_PAY_WARPOINT, FALSE, pEnd->byWinInfluenceTy, WPUT_OUTPOSTWAR);				// 2009-01-22 by cmkwon, ���� ���� ���� �α� ���� - CFieldMapChannel::AddWarPointInMap()�� ����(, BYTE i_byWPUpdateTy=WPUT_GENERAL) �߰�// 2008-05-20 by dhjin, �¸� ���� ���� ���� ����. pQParam �� ����ϴ� �κ� ����
				pBackFMChann->AddWarPointInMap(OUTPOST_PAY_WARPOINT_BY_LOSE, FALSE, pEnd->byLossInfluenceTy, WPUT_OUTPOSTWAR);	// 2009-01-22 by cmkwon, ���� ���� ���� �α� ���� - CFieldMapChannel::AddWarPointInMap()�� ����(, BYTE i_byWPUpdateTy=WPUT_GENERAL) �߰�		// 2008-01-07 by dhjin, ���� ���� ���� - 
				pBackFMChann->InsertItemInMap(OUTPOST_PAY_ITEM, OUTPOST_PAY_ITEM_COUNT, pEnd->byWinInfluenceTy);	// 2008-05-20 by dhjin, �¸� ���� ���� ���� ����. pQParam �� ����ϴ� �κ� ����	// 2007-11-06 by dhjin, ������ ���� �߰� 
			}
		}

		// 2009-07-08 by cmkwon, 2. ���� ���� ó��
		m_pFieldIOCP18->PayCOutPostWarContributionCalculation(pEnd->pFMChanOfOutPostWarMap->GetMapChannelIndex().MapIndex, pEnd->byWinInfluenceTy);
		m_pFieldIOCP18->DeleteCOutPostWarContributionCalculation(pEnd->pFMChanOfOutPostWarMap->GetMapChannelIndex().MapIndex);	

		// 2009-07-08 by cmkwon, 3. ���� ���� ó��
		pEnd->pFMChanOfOutPostWarMap->WarpToCityMapFieldMapChannelByGuildUID(pEnd->GuildUIDPossessOutPost);
		if(0 < pEnd->nCityMapIndexForWarp)
		{
			pEnd->pFMChanOfOutPostWarMap->WarpToOutPostCityMapFieldMapChannelByGuildUID(pEnd->GuildUIDPossessOutPost, pEnd->nCityMapIndexForWarp);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::SendOutPostWarRemainTime(int i_nRemainTime, BYTE i_byInfluence, int i_nMapIndex)
/// \brief		������� ���� ���� ���� �ð��� Ŭ���̾�Ʈ���� �����Ѵ�.
/// \author		dhjin
/// \date		2007-08-17 ~ 2007-08-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::SendOutPostWarRemainTime(int i_nRemainTime, BYTE i_byInfluence, int i_nMapIndex)
{
	INIT_MSG_WITH_BUFFER(MSG_FC_OUTPOST_WAR_BEFORE, T_FC_OUTPOST_WAR_BEFORE, SendMsg, SendBuf);
	SendMsg->Influence		= i_byInfluence;
	SendMsg->MapIndex		= i_nMapIndex;
	SendMsg->RemainTime		= i_nRemainTime;		
	m_pFieldIOCP18->SendMessageToAllClients(SendBuf,MSG_SIZE(MSG_FC_OUTPOST_WAR_BEFORE),INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::OutPostWarStart(COutPost * i_pOutPost)
/// \brief		�������� �� ����
/// \author		dhjin
/// \date		2007-08-17 ~ 2007-08-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::OutPostWarStart(COutPost * i_pOutPost)
{
	//////////////////////////////////////////////////////////////////////////
	//  1. �������� ���°� ����	OUTPOST_WARSTART
	//	2. �������� ���� ��ȯ
	//  3. �������� ���� ����
	//  4. �������� ���°� ���� OUTPOST_WARING
	
	// 2007-08-17 by dhjin, �������� ���°� ����
	i_pOutPost->SetOutPostState(OUTPOST_WARSTART);

	// 2007-08-17 by dhjin, ���� ��ȯ
	CFieldMapChannel *pFMChann = m_pFieldIOCP18->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(i_pOutPost->GetOutPostMapIndex(), 0));
	if(NULL == pFMChann)
	{
		// 2009-07-08 by cmkwon, ���� ������ ���� ���� - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] OutPostWarNoti COutPostManager::OutPostWarStart# OutpostWarMapIndex(%d) pFMChann(0x%X) \r\n", i_pOutPost->GetOutPostMapIndex(), pFMChann);
		return;
	}

	// 2008-03-26 by dhjin, ���� ������ �ְ� ������ ���� ��� ó��
	if(FALSE != i_pOutPost->GetOutPostInfluence()
		&& FALSE == i_pOutPost->GetOutPostGuildUID())
	{
		i_pOutPost->SetOutPostPossessInfluence(INFLUENCE_TYPE_UNKNOWN);
	}

	INIT_MSG_WITH_BUFFER(MSG_FN_MONSTER_OUTPOST_INIT, T_FN_MONSTER_OUTPOST_INIT, pSummon, SendBuf);
	pSummon->mapChann	= pFMChann->GetMapChannelIndex();
	this->SetInitOutPostMonsterSummonByBell(i_pOutPost->GetOutPostInfluence(), &(pSummon->bell1), &(pSummon->bell2), &(pSummon->bell3) );
	pFMChann->Send2NPCServerW(SendBuf, MSG_SIZE(MSG_FN_MONSTER_OUTPOST_INIT));
	
	// 2007-10-02 by dhjin, �׽�Ʈ ��
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] OutPostWarNoti OutPostWarStart  OUTPOST_INIT bell1 : [%6d], bell2 : [%6d], bell3 : [%6d], NextWarTimeDB : [%s]\r\n"
		, pSummon->bell1, pSummon->bell2, pSummon->bell3, i_pOutPost->GetOutPostNextWarTime()->GetDateTimeString().GetBuffer());	// 2008-06-27 by dhjin, �α׿� ���۽ð��� ���� ������ �߰�

	// 2007-08-17 by dhjin, Ŭ���̾�Ʈ�� ���� ����
	INIT_MSG_WITH_BUFFER(MSG_FC_OUTPOST_WAR_START, T_FC_OUTPOST_WAR_START, SendMsg, SendBuf2);
	SendMsg->OutPostState	= i_pOutPost->GetOutPostState();
	SendMsg->Influence		= i_pOutPost->GetOutPostInfluence();
	SendMsg->MapIndex		= i_pOutPost->GetOutPostMapIndex();
	SendMsg->WarStartTime	= *(i_pOutPost->GetOutPostNextWarTime());
	SendMsg->WarEndTime		= SendMsg->WarStartTime;
	SendMsg->WarEndTime.AddDateTime(0,0,0,0,OUTPOST_WARTIME,0);
	if(g_pFieldGlobal->IsTestServer())
	{// 2008-10-29 by cmkwon, �׼� �ý��� ����(���������� 1�ð�(60��)���� ����) - 
		SendMsg->WarEndTime		= SendMsg->WarStartTime;	
		SendMsg->WarEndTime.AddDateTime(0,0,0,0,OUTPOST_WARTIME_FOR_TESTSERVER,0);
	}
	m_pFieldIOCP18->SendMessageToAllClients(SendBuf2,MSG_SIZE(MSG_FC_OUTPOST_WAR_START),INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);

	//////////////////////////////////////////////////////////////////////////
	// 2008-12-23 by dhjin, ���� ���� �߰���
	m_pFieldIOCP18->InsertCOutPostWarContributionCalculation(i_pOutPost->GetOutPostMapIndex());
	m_pFieldIOCP18->WarStartOtherInflStayTimeForAllUser();

	// 2007-08-17 by dhjin, �������� ���°� ����
	i_pOutPost->SetOutPostState(OUTPOST_WARING);

	// 2007-10-02 by dhjin, �α� �����
	// 2007-10-16 by cmkwon, �α� �߰� - SendLogMessageOutPostStart() �Լ� ���� ����
	CAtumLogSender::SendLogMessageOutPostStart(i_pOutPost->GetOutPostMapIndex(), i_pOutPost->GetOutPostInfluence(), i_pOutPost->GetOutPostGuildUID());
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::OnOutPostProtectorDestroy(int i_nMapIndex)
/// \brief		�������� ��ȣ�� �ı��� ó�� 
/// \author		dhjin
/// \date		2007-08-24 ~ 2007-08-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::OnOutPostProtectorDestroy(int i_nMapIndex)
{
	//////////////////////////////////////////////////////////////////////////
	// 1. �������� ���� ���� �� ���� ����
	// 2. ���õ� ���� ���� �����ϰ� ����
	// 3. ��ȣ�� �ı� ���� Ŭ���̾�Ʈ���� ����
	// 4. �������� ���� ����
	// 5. �������� ���°� ���� OUTPOST_RESETSTART
	
	mt_auto_lock mta(&m_mtvectOutPostManager);

	COutPost * tmOutPost = this->GetOutPost(i_nMapIndex);
	if(NULL == tmOutPost)
	{
		return;
	}

	CFieldMapChannel *pFMChann = m_pFieldIOCP18->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(tmOutPost->GetOutPostMapIndex(), 0));
	if(NULL == pFMChann)
	{
		// 2009-07-08 by cmkwon, ���� ������ ���� ���� - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] OutPostWarNoti COutPostManager::OnOutPostProtectorDestroy# OutpostWarMapIndex(%d) pFMChann(0x%X) \r\n", tmOutPost->GetOutPostMapIndex(), pFMChann);
		return;
	}

	mtvectSOutPostProtectorDamage tmvectDamageList;
	
	INIT_MSG_WITH_BUFFER(MSG_FC_OUTPOST_PROTECTOR_DESTROY, T_FC_OUTPOST_PROTECTOR_DESTROY, SendMsg, SendBuf);
	// 2007-08-24 by dhjin, �������� ���� ���� �� ���� ����, ���õ� ���� ���� �����ϰ� ����, ��ȣ�� �ı� ���� Ŭ���̾�Ʈ���� ����
	SendMsg->MapIndex	= pFMChann->GetMapChannelIndex().MapIndex;
	SendMsg->GuildUID	= tmOutPost->GetTopGuildOfProtectorDamage(&tmvectDamageList);		// 2007-10-16 by cmkwon, �α� �߰� - �����߰�(mtvectSOutPostProtectorDamage *o_pvectDamageList)

	// 2007-10-16 by cmkwon, �α� �߰� - 
	CAtumLogSender::SendLogMessageOutPostProtectorDestroy(tmOutPost->GetOutPostMapIndex(), tmOutPost->GetOutPostInfluence(), tmOutPost->GetOutPostGuildUID(), tmOutPost->GetOutPostResetPossibleInfluence(), SendMsg->GuildUID, &tmvectDamageList);

	if(0 == SendMsg->GuildUID)
	{// 2007-10-05 by dhjin, ���� �߾�ó����ġ ���� ������ ������ ���ٸ� �������͸� �ٽ� ��ȯ�Ѵ�.
		// 2007-10-01 by dhjin, �������� ���� ����
		tmOutPost->SetOutPostPossessInfluenceByReset();

		// 2007-10-01 by dhjin, ���¿� ���� ������ ���� ó��
		pFMChann->WarpToCityMapFieldMapChannelByInfluence(tmOutPost->GetOutPostInfluence());

		// 2007-08-24 by dhjin, �������� ���� �ٽ� ��ȯ
		INIT_MSG_WITH_BUFFER(MSG_FN_MONSTER_OUTPOST_INIT, T_FN_MONSTER_OUTPOST_INIT, pSummon, SendBuf2);
		pSummon->mapChann	= pFMChann->GetMapChannelIndex();
		this->SetInitOutPostMonsterSummonByBell(tmOutPost->GetOutPostInfluence(), &(pSummon->bell1), &(pSummon->bell2), &(pSummon->bell3) );
		pFMChann->Send2NPCServerW(SendBuf2, MSG_SIZE(MSG_FN_MONSTER_OUTPOST_INIT));
		
		// 2007-10-02 by dhjin, �׽�Ʈ ��
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] OutPostWarNoti OnOutPostProtectorDestroy  OUTPOST_INIT bell1 : [%6d], bell2 : [%6d], bell3 : [%6d]\r\n"
			, pSummon->bell1, pSummon->bell2, pSummon->bell3);
		
		m_pFieldIOCP18->SendMessageToAllClients(SendBuf,MSG_SIZE(MSG_FC_OUTPOST_PROTECTOR_DESTROY),INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);
		return;
	}
	SendMsg->Influence	= tmOutPost->GetOutPostResetPossibleInfluence();
	util::strncpy(SendMsg->GuildName, tmOutPost->GetOutPostResetPossibleGuildName(), SIZE_MAX_GUILD_NAME);
	m_pFieldIOCP18->SendMessageToAllClients(SendBuf,MSG_SIZE(MSG_FC_OUTPOST_PROTECTOR_DESTROY),INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);

	tmOutPost->InitProtectorDamage();

	// 2007-08-24 by dhjin, �������� ���� ����
	INIT_MSG_WITH_BUFFER(MSG_FN_MONSTER_OUTPOST_RESET_SUMMON, T_FN_MONSTER_OUTPOST_RESET_SUMMON, pSummon, SendBuf2);
	pSummon->mapChann	= pFMChann->GetMapChannelIndex();
	if(INFLUENCE_TYPE_VCN == tmOutPost->GetOutPostResetPossibleInfluence())
	{
		pSummon->bell1		= BELL_OUTPOST_RESET_VCN;
	}
	else
	{
		pSummon->bell1		= BELL_OUTPOST_RESET_ANI;
	}
	pFMChann->Send2NPCServerW(SendBuf2, MSG_SIZE(MSG_FN_MONSTER_OUTPOST_RESET_SUMMON));
	
	// 2007-08-24 by dhjin, �������� ���°� ���� OUTPOST_RESETSTART
	tmOutPost->SetOutPostState(OUTPOST_RESETSTART);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::OnOutPostResetDestroy(int i_nMapIndex)
/// \brief		�������� �߾�ó�� �ı��� ó�� 
/// \author		dhjin
/// \date		2007-08-24 ~ 2007-08-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::OnOutPostResetDestroy(int i_nMapIndex)
{
	//////////////////////////////////////////////////////////////////////////
	// 1. �߾�ó�� �ı� ���� ����
	// 2. �������� ���°� ���� OUTPOST_RESETEND
	// 3. �������� NPC���� ó��
	// 4. �������� �� ���� ��� ������ ���� ó��
	// 5. �������� ���� �ٽ� ��ȯ
	// 6. �������� ���°��� �����Ѵ�. OUTPOST_WARING

	mt_auto_lock mta(&m_mtvectOutPostManager);

	COutPost * tmOutPost = this->GetOutPost(i_nMapIndex);
	if(NULL == tmOutPost)
	{
		return;
	}

	CFieldMapChannel *pFMChann = m_pFieldIOCP18->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(tmOutPost->GetOutPostMapIndex(), 0));
	if(NULL == pFMChann)
	{
		// 2009-07-08 by cmkwon, ���� ������ ���� ���� - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] OutPostWarNoti COutPostManager::OnOutPostResetDestroy# OutpostWarMapIndex(%d) pFMChann(0x%X) \r\n", tmOutPost->GetOutPostMapIndex(), pFMChann);
		return;
	}		
	
	INIT_MSG_WITH_BUFFER(MSG_FC_OUTPOST_RESET_DESTROY, T_FC_OUTPOST_RESET_DESTROY, SendMsg, SendBuf);
	// 2007-08-24 by dhjin, �߾�ó�� �ı� ���� ����
	SendMsg->MapIndex	= i_nMapIndex;
	m_pFieldIOCP18->SendMessageToAllClients(SendBuf,MSG_SIZE(MSG_FC_OUTPOST_RESET_DESTROY),INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);

	// 2007-08-24 by dhjin, �������� ���°� ���� OUTPOST_RESETEND
	tmOutPost->SetOutPostState(OUTPOST_RESETEND);

	//////////////////////////////////////////////////////////////////////////
	// 2007-10-01 by dhjin, ���� �����ϴ� ������ NPC�̸� NPC �����̴�.
	if(FALSE == tmOutPost->GetOutPostInfluence())
	{
		// 2007-08-24 by dhjin, �������� NPC���� ó��
		tmOutPost->SetOutPostPossessNPCByReset();

		// 2007-08-24 by dhjin, �� ���� ��� ������ ���� ó��
		pFMChann->WarpToCityMapFieldMapChannelByGuildUID(tmOutPost->GetOutPostGuildUID());
	}
	else
	{// 2007-10-01 by dhjin, VCN, ANI ������ ���� ���� ��� �״�� �д�.
		// 2007-10-01 by dhjin, �������� ���� ����
		tmOutPost->SetOutPostPossessInfluenceByReset();

		// 2007-10-01 by dhjin, ���¿� ���� ������ ���� ó��
		pFMChann->WarpToCityMapFieldMapChannelByInfluence(tmOutPost->GetOutPostInfluence());

		m_pFieldIOCP18->m_InflWarManager.CalcConsecutiveVictoriesPoint(tmOutPost->GetOutPostInfluence());		// 2013-05-09 by hskim, ���� ����Ʈ ����	
	}

	// 2007-08-24 by dhjin, �������� ���� �ٽ� ��ȯ
	INIT_MSG_WITH_BUFFER(MSG_FN_MONSTER_OUTPOST_INIT, T_FN_MONSTER_OUTPOST_INIT, pSummon, SendBuf2);
	pSummon->mapChann	= pFMChann->GetMapChannelIndex();
	this->SetInitOutPostMonsterSummonByBell(tmOutPost->GetOutPostInfluence(), &(pSummon->bell1), &(pSummon->bell2), &(pSummon->bell3) );
	pFMChann->Send2NPCServerW(SendBuf2, MSG_SIZE(MSG_FN_MONSTER_OUTPOST_INIT));

	// 2007-10-02 by dhjin, �׽�Ʈ ��
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] OutPostWarNoti OnOutPostResetDestroy  OUTPOST_INIT bell1 : [%6d], bell2 : [%6d], bell3 : [%6d]\r\n"
		, pSummon->bell1, pSummon->bell2, pSummon->bell3);

	// 2007-08-24 by dhjin, �������� ���°� ���� OUTPOST_WARING
	tmOutPost->SetOutPostState(OUTPOST_WARING);

	// 2007-10-16 by cmkwon, �α� �߰� - 
	CAtumLogSender::SendLogMessageOutPostResetDestroy(tmOutPost->GetOutPostMapIndex(), tmOutPost->GetOutPostInfluence(), tmOutPost->GetOutPostGuildUID(), tmOutPost->GetOutPostResetPossibleInfluence(), tmOutPost->GetOutPostResetPossibleGuildUID());
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL COutPostManager::CheckValidGuildUID(int i_nMapIndex, UID32_t i_nGuildUID)
/// \brief		��ȿ�� �������� üũ
/// \author		dhjin
/// \date		2007-08-22 ~ 2007-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL COutPostManager::CheckValidGuildUID(int i_nMapIndex, UID32_t i_nGuildUID)
{
	// 2007-09-21 by cmkwon, ���� UID�� ��ȿ���� �̸� üũ
	if(FALSE == IS_VALID_UNIQUE_NUMBER(i_nGuildUID))
	{
		return FALSE;
	}

	mt_auto_lock mta(&m_mtvectOutPostManager);

	// 2007-08-20 by dhjin, ��ȿ�� ������� üũ
	mtvectOutPost::iterator	itr = m_mtvectOutPostManager.begin();
	for(; itr != m_mtvectOutPostManager.end(); itr++)
	{
		if(i_nMapIndex == (*itr)->GetOutPostMapIndex()
			&& i_nGuildUID == (*itr)->GetOutPostGuildUID())
		{
			return TRUE;
		}
	}	

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT	COutPostManager::GetOutPostCityMapIndexByGuildUID(UID32_t i_nGuildUID)
/// \brief		��ȿ�� �����̸� �������� ���� �� ��ȣ�� �����Ѵ�.
/// \author		dhjin
/// \date		2007-09-05 ~ 2007-09-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT	COutPostManager::GetOutPostCityMapIndexByGuildUID(UID32_t i_nGuildUID)
{
	// 2007-09-21 by cmkwon, üũ �߰�
	if(FALSE == IS_VALID_UNIQUE_NUMBER(i_nGuildUID))
	{
		return 0;
	}

	mt_auto_lock mta(&m_mtvectOutPostManager);

	mtvectOutPost::iterator	itr = m_mtvectOutPostManager.begin();
	for(; itr != m_mtvectOutPostManager.end(); itr++)
	{
		if(i_nGuildUID == (*itr)->GetOutPostGuildUID())
		{
			return (*itr)->GetOutPostCityMapIndex();
		}
	}	

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT	COutPostManager::GetOutPostMapIndexByGuildUID(UID32_t i_nGuildUID)
/// \brief		��ȿ�� �����̸� �������� �� ��ȣ�� �����Ѵ�.
/// \author		dhjin
/// \date		2007-09-15 ~ 2007-09-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT	COutPostManager::GetOutPostMapIndexByGuildUID(UID32_t i_nGuildUID)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);

	mtvectOutPost::iterator	itr = m_mtvectOutPostManager.begin();
	for(; itr != m_mtvectOutPostManager.end(); itr++)
	{
		if(i_nGuildUID == (*itr)->GetOutPostGuildUID())
		{
			return (*itr)->GetOutPostMapIndex();
		}
	}	

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL COutPostManager::CheckResetValidGuildUID(int i_nMapIndex, UID32_t i_nGuildUID)
/// \brief		������ ��ȿ�� �������� üũ
/// \author		dhjin
/// \date		2007-08-20 ~ 2007-08-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL COutPostManager::CheckResetValidGuildUID(int i_nMapIndex, UID32_t i_nGuildUID)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);

	// 2007-08-20 by dhjin, �������� ���� ������ ������� üũ
	mtvectOutPost::iterator	itr = m_mtvectOutPostManager.begin();
	for(; itr != m_mtvectOutPostManager.end(); itr++)
	{
		if(i_nMapIndex == (*itr)->GetOutPostMapIndex()
			&& i_nGuildUID == (*itr)->GetOutPostResetPossibleGuildUID())
		{
			return TRUE;
		}
	}	

	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BYTE COutPostManager::GetOutPostInfluenceByMapIndex(int i_nMapIdx)
/// \brief		// 2007-09-28 by cmkwon, ���������� ���� üũ ���� - �ش� ���������� ���� ������ �����Ѵ�.
/// \author		cmkwon
/// \date		2007-09-28 ~ 2007-09-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE COutPostManager::GetOutPostInfluenceByMapIndex(int i_nMapIdx)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);

	mtvectOutPost::iterator	itr = m_mtvectOutPostManager.begin();
	for(; itr != m_mtvectOutPostManager.end(); itr++)
	{
		COutPost *pOutPost = *itr;
		if(i_nMapIdx == pOutPost->GetOutPostMapIndex()
			|| i_nMapIdx == pOutPost->GetOutPostCityMapIndex())
		{// 2007-09-28 by cmkwon, ���������ʰ� �����������ø��� �δ� üũ�Ѵ�
			return pOutPost->GetOutPostInfluence();
		}
	}

	return INFLUENCE_TYPE_UNKNOWN;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL COutPostManager::SendOutPostResetStartByGuildCommander(int i_nMapIndex, UID32_t i_guildUID, UID32_t i_charUID)
/// \brief		// 2007-10-16 by cmkwon, �α� �߰� - �����߰�(, UID32_t i_guildUID, UID32_t i_charUID), �������� ������ ���� ��
/// \author		dhjin
/// \date		2007-08-20 ~ 2007-08-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL COutPostManager::SendOutPostResetStartByGuildCommander(int i_nMapIndex, UID32_t i_guildUID, UID32_t i_charUID)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);

	ATUM_DATE_TIME ResetStartTime;

	COutPost * tmOutPost = this->GetOutPost(i_nMapIndex);
	if(NULL == tmOutPost)
	{
		return FALSE;
	}

	if(IS_OUTPOST_RESETING(tmOutPost->GetOutPostState()) )
	{// 2007-09-11 by dhjin, �̹� ���� ���̸� �����Ѵ�.
		return FALSE;
	}

	tmOutPost->SetOutPostState(OUTPOST_RESETING);
	tmOutPost->SetOutPostResetStartTime( &(ResetStartTime.GetCurrentDateTime()) );

	// 2007-10-16 by cmkwon, �α� �߰� -
	CAtumLogSender::SendLogMessageOutPostResetStart(tmOutPost->GetOutPostMapIndex(), tmOutPost->GetOutPostInfluence(), tmOutPost->GetOutPostGuildUID(), tmOutPost->GetOutPostResetPossibleInfluence(), tmOutPost->GetOutPostResetPossibleGuildUID(), i_guildUID, i_charUID);

	// 2007-08-20 by dhjin, Ŭ���̾�Ʈ�� ���� ����
	INIT_MSG_WITH_BUFFER(MSG_FC_OUTPOST_RESET_START_OK, T_FC_OUTPOST_RESET_START_OK, SendMsg, SendBuf);
	SendMsg->Influence		= tmOutPost->GetOutPostResetPossibleInfluence();
	SendMsg->MapIndex		= tmOutPost->GetOutPostMapIndex();
	SendMsg->GuildUID		= tmOutPost->GetOutPostResetPossibleGuildUID();
	util::strncpy(SendMsg->GuildName, tmOutPost->GetOutPostResetPossibleGuildName(), SIZE_MAX_GUILD_NAME);
	SendMsg->ResetStartTime = *(tmOutPost->GetOutPostResetStartTime());
	SendMsg->ResetEndTime = *(tmOutPost->GetOutPostResetEndTime());
	m_pFieldIOCP18->SendMessageToAllClients(SendBuf,MSG_SIZE(MSG_FC_OUTPOST_RESET_START_OK),INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);

	return	TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::OutPostResetSuccess(COutPost * i_pOutPost)
/// \brief		�������� ���� ����
///				// 2009-07-08 by cmkwon, ���� ������ ���� ���� - �����߰�(, vectSOUTPOSTWAR_EV_RESET_SUCCESS *io_pVectEvResetSuccessList)
/// \author		dhjin
/// \date		2007-08-20 ~ 2007-08-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::OutPostResetSuccess(COutPost * i_pOutPost, vectSOUTPOSTWAR_EV_RESET_SUCCESS *io_pVectEvResetSuccessList)
{
	//////////////////////////////////////////////////////////////////////////
	// 1. �������� ���°��� �����Ѵ�. OUTPOST_RESETEND
	// 2. �������� ������ ���� ������ ����, �������� �����Ѵ�.
	// 3. �������� ���� ���Ḧ Ŭ���̾�Ʈ���� ����
	// 4. �������� ���� ������ �ƴ� ������ ������ ������ ����.
	// 5. �������� ���� ���ͷ� �ٽ� ��ȯ�Ѵ�.
	// 6. �������� ���°��� �����Ѵ�. OUTPOST_WARING

	// 2007-08-20 by dhjin, �������� ���°��� �����Ѵ�. OUTPOST_RESETEND
	i_pOutPost->SetOutPostState(OUTPOST_RESETEND);

	// 2007-08-20 by dhjin, �������� ������ ���� ������ ����, �������� �����Ѵ�.
	i_pOutPost->SetOutPostPossessGuildAndInfluenceByResetSuccess();

	// 2007-08-20 by dhjin, �������� ���� ���Ḧ Ŭ���̾�Ʈ���� ����
	INIT_MSG_WITH_BUFFER(MSG_FC_OUTPOST_RESET_SUCCESS, T_FC_OUTPOST_RESET_SUCCESS, SendMsg, SendBuf);
	SendMsg->Influence		= i_pOutPost->GetOutPostInfluence();
	SendMsg->MapIndex		= i_pOutPost->GetOutPostMapIndex();
	SendMsg->GuildUID		= i_pOutPost->GetOutPostGuildUID();
	util::strncpy(SendMsg->GuildName, i_pOutPost->GetOutPostGuildName(), SIZE_MAX_GUILD_NAME);
	m_pFieldIOCP18->SendMessageToAllClients(SendBuf,MSG_SIZE(MSG_FC_OUTPOST_RESET_SUCCESS),INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);

	// 2007-08-20 by dhjin, �������� ���� ������ �ƴ� ������ ������ ������ ����.
	CFieldMapChannel *pFMChann = m_pFieldIOCP18->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(i_pOutPost->GetOutPostMapIndex(), 0));
	if(NULL == pFMChann)
	{
		// 2009-07-08 by cmkwon, ���� ������ ���� ���� - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] OutPostWarNoti COutPostManager::OutPostResetSuccess# OutpostWarMapIndex(%d) pFMChann(0x%X) \r\n", i_pOutPost->GetOutPostMapIndex(), pFMChann);
		return;
	}

// 2009-07-08 by cmkwon, ���� ������ ���� ���� - COutPostManager::OnDoMinutelyWorkOutPostManager()���� ó���ϵ��� ������.
//	pFMChann->WarpToCityMapFieldMapChannelByInfluence(i_pOutPost->GetOutPostInfluence());
	///////////////////////////////////////////////////////////////////////////////
	// 2009-07-08 by cmkwon, ���� ������ ���� ���� - �Ʒ��� ���� ����� �����Ͽ� ó���ϵ��� ������.
	SOUTPOSTWAR_EV_RESET_SUCCESS tmReset;
	util::zero(&tmReset, sizeof(tmReset));
	tmReset.bySuccessInfluenceTy	= i_pOutPost->GetOutPostInfluence();
	tmReset.pFMChanOfOutPostWarMap	= pFMChann;
	io_pVectEvResetSuccessList->push_back(tmReset);

	pFMChann->m_ChangeBodyConMonsterIndex	= 0;					// 2007-09-10 by dhjin, �߾�ó�� ��ġ �ٵ������ �ٲٴ� ���� Index�� 0���� �����Ѵ�.	

	// 2007-08-20 by dhjin, �������� ���� ���ͷ� �ٽ� ��ȯ�Ѵ�.
	INIT_MSG_WITH_BUFFER(MSG_FN_MONSTER_OUTPOST_INIT, T_FN_MONSTER_OUTPOST_INIT, pSummon, SendBuf2);
	pSummon->mapChann	= pFMChann->GetMapChannelIndex();
	this->SetInitOutPostMonsterSummonByBell(i_pOutPost->GetOutPostInfluence(), &(pSummon->bell1), &(pSummon->bell2), &(pSummon->bell3) );
	pFMChann->Send2NPCServerW(SendBuf2, MSG_SIZE(MSG_FN_MONSTER_OUTPOST_INIT));

	// 2007-10-02 by dhjin, �׽�Ʈ ��
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] OutPostWarNoti OutPostResetSuccess  OUTPOST_INIT bell1 : [%6d], bell2 : [%6d], bell3 : [%6d]\r\n"
		, pSummon->bell1, pSummon->bell2, pSummon->bell3);

	// 2007-08-20 by dhjin, �������� ���°��� �����Ѵ�. OUTPOST_WARING
	i_pOutPost->SetOutPostState(OUTPOST_WARING);
	
	// 2007-10-16 by cmkwon, �α� �߰� - 
	CAtumLogSender::SendLogMessageOutPostResetComplete(i_pOutPost->GetOutPostMapIndex(), i_pOutPost->GetOutPostInfluence(), i_pOutPost->GetOutPostGuildUID());
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::OutPostWarEnd(COutPost * i_pOutPost)
/// \brief		�������� �� ����	
///				// 2009-07-08 by cmkwon, ���� ������ ���� ���� - �����߰�(, vectSOUTPOSTWAR_EV_END *io_pEvEndList)
/// \author		dhjin
/// \date		2007-08-20 ~ 2007-08-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::OutPostWarEnd(COutPost * i_pOutPost, vectSOUTPOSTWAR_EV_END *io_pEvEndList)
{
	//////////////////////////////////////////////////////////////////////////
	// 1. �������� ���°��� �����Ѵ�. OUTPOST_WAREND
	// 2. �������� ���͸� �ʱ�ȭ�Ѵ�.
	// 3. �������� ���� �����ڸ� �����Ѵ�.
	// 4. ���������� ���Ḧ Ŭ���̾�Ʈ���� �����Ѵ�.
	// 5. �������� ���°��� �����Ѵ�. OUTPOST_NORMAL
	// 6. �������� �ʿ� �ִ� ������ ���ǿ� �°� ������Ų��. (���� ������ �ִٸ� ���� ���ܿ��� �������� ���� ������ ����)
	// 7. �ǰ��� �����Ѵ�.

	///////////////////////////////////////////////////////////////////////////////
	// 2009-07-08 by cmkwon, ���� ������ ���� ���� - 
	SOUTPOSTWAR_EV_END	tmEvEnd;
	util::zero(&tmEvEnd, sizeof(tmEvEnd));
	

	// 2007-08-20 by dhjin, �������� ���°��� �����Ѵ�. OUTPOST_WAREND
	i_pOutPost->SetOutPostState(OUTPOST_WAREND);
	
	
	// 2007-08-20 by dhjin, �������� ���͸� �ʱ�ȭ�Ѵ�.
	CFieldMapChannel *pFMChann = m_pFieldIOCP18->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(i_pOutPost->GetOutPostMapIndex(), 0));
	if(NULL == pFMChann)
	{
		// 2009-07-08 by cmkwon, ���� ������ ���� ���� - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] OutPostWarNoti COutPostManager::OutPostWarEnd# OutpostWarMapIndex(%d) pFMChann(0x%X) \r\n", i_pOutPost->GetOutPostMapIndex(), pFMChann);
		return;
	}

	INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL, T_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL, pMonster, SendBuf);
	pMonster->mapChann = pFMChann->GetMapChannelIndex();
	pMonster->bAllFlag = TRUE;
	m_pFieldIOCP18->Send2NPCServerByTCP(SendBuf, MSG_SIZE(MSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL));

	
	// 2007-08-20 by dhjin, �������� ���� �����ڸ� �����Ѵ�.
	// 1. �켱 �������� ���� ���� �����ϰ�
	// 2. NPC���� ���� ���� ���� ���� üũ�Ͽ� ���� ���� �ð� ������ �����Ѵ�.
	i_pOutPost->SetOutPostPossessByWarEnd();
	if(FALSE == i_pOutPost->GetOutPostGuildUID()
		&& FALSE == i_pOutPost->GetOutPostInfluence())
	{// 2007-08-20 by dhjin, NPC������ �� ���������� ���� ���� �ð��� �����Ѵ�.
		if(0 == m_vectOutPostNextWarTimeInfo.size())
		{// 2007-08-20 by dhjin, DB���� ���� �Ǿ� ���� ������ ���Ƿ� �����Ѵ�.
			ATUM_DATE_TIME tmNextTime;
			tmNextTime.SetDateTime(0,0,0,20,0,0);
			i_pOutPost->SetOutPostNextWarTime(&tmNextTime);
		}
		else
		{
			// 2007-09-03 by dhjin, 2��° �����ص� ���� ���� �� �ð����� ����.
			vectATUM_DATE_TIME::iterator itr = m_vectOutPostNextWarTimeInfo.begin();
			itr++;
			i_pOutPost->SetOutPostNextWarTime(&*itr);
		}
		i_pOutPost->SetOutPostNextWarTimeSet(TRUE);
		// 2007-08-21 by dhjin, DB ������Ʈ
		QPARAM_SET_NPCPOSSESSION_OUTPOST *pQParam = new QPARAM_SET_NPCPOSSESSION_OUTPOST;
		pQParam->MapIndex			= i_pOutPost->GetOutPostMapIndex();
		pQParam->GetTime			= *(i_pOutPost->GetPotPostGetTime());
		pQParam->NextWarTime		= *(i_pOutPost->GetOutPostNextWarTime());
		m_pFieldIOCP18->m_pAtumDBManager->MakeAndEnqueueQuery(QT_SetNPCPossessionOutPost, NULL, NULL, pQParam);

	}
	else
	{// 2007-08-21 by dhjin, ���� ���� ���������̴�.
		// 2007-09-03 by dhjin, 2��° �����ص� ���� ���� �� �ð����� ����.
		vectATUM_DATE_TIME::iterator itr = m_vectOutPostNextWarTimeInfo.begin();
		itr++;
		i_pOutPost->SetOutPostNextWarTime(&*itr);
		i_pOutPost->SetOutPostNextWarTimeSet(FALSE);
		// 2007-08-21 by dhjin, DB ������Ʈ
		QPARAM_SET_GUILDPOSSESSION_OUTPOST *pQParam = new QPARAM_SET_GUILDPOSSESSION_OUTPOST;
		pQParam->MapIndex			= i_pOutPost->GetOutPostMapIndex();
		pQParam->Influence			= i_pOutPost->GetOutPostInfluence();
		pQParam->GuildUID			= i_pOutPost->GetOutPostGuildUID();
		pQParam->GetTime			= *(i_pOutPost->GetPotPostGetTime());
		pQParam->NextWarTime		= *(i_pOutPost->GetOutPostNextWarTime());
		m_pFieldIOCP18->m_pAtumDBManager->MakeAndEnqueueQuery(QT_SetGuildPossessionOutPost, NULL, NULL, pQParam);

#ifdef _OUTPOST_BUFFS
		m_pFieldIOCP18->DeleteBuffSkillAllCharacter(INFLUENCE_TYPE_ANI | INFLUENCE_TYPE_VCN, DES_SKILLTYPE_OUTPOST_BUFF);

		int itemNum = 0;
		switch (i_pOutPost->GetOutPostMapIndex())
		{
			case 6016:
				itemNum = OUTPOST_BUFF_SKILL_AKRON;
				break;
			case 6017:
				itemNum = OUTPOST_BUFF_SKILL_RAKION;
				break;
			case 6018:
				itemNum = OUTPOST_BUFF_SKILL_CORONADO;
				break;
			case 4303:
				itemNum = OUTPOST_BUFF_SKILL_NGC_MILITARY_BASE;
				break;
		}
		
		if (itemNum != 0)
		{
			m_pFieldIOCP18->ApplyBuffSkillToGuild(itemNum, i_pOutPost->GetOutPostGuildUID());
		}
#endif
		// 2008-01-07 by dhjin, ���� ���� ���� - 
		BYTE LoseInfluence = INFLUENCE_TYPE_UNKNOWN;
		BYTE WinInfluence = i_pOutPost->GetOutPostInfluence();		// 2008-05-20 by dhjin, �¸� ���� ���� ���� ����. pQParam �� ����ϴ� �κ� ����
		if(INFLUENCE_TYPE_VCN == WinInfluence)			// 2008-05-20 by dhjin, �¸� ���� ���� ���� ����. pQParam �� ����ϴ� �κ� ����
		{
			LoseInfluence = INFLUENCE_TYPE_ANI;
		}
		else
		{
			LoseInfluence = INFLUENCE_TYPE_VCN;
		}

// 2009-07-08 by cmkwon, ���� ������ ���� ���� - ���� ó���� COutPostManager::OnDoMinutelyWorkOutPostManager()���� ó���ϰ� ����
// 		// 2007-09-14 by dhjin, �¸� ���¿��� WarPoint�� �����Ѵ�.
// 		pFMChann->AddWarPointInMap(OUTPOST_PAY_WARPOINT, FALSE, WinInfluence, WPUT_OUTPOSTWAR);				// 2009-01-22 by cmkwon, ���� ���� ���� �α� ���� - CFieldMapChannel::AddWarPointInMap()�� ����(, BYTE i_byWPUpdateTy=WPUT_GENERAL) �߰�, // 2008-05-20 by dhjin, �¸� ���� ���� ���� ����. pQParam �� ����ϴ� �κ� ����
// 		pFMChann->AddWarPointInMap(OUTPOST_PAY_WARPOINT_BY_LOSE, FALSE, LoseInfluence, WPUT_OUTPOSTWAR);	// 2009-01-22 by cmkwon, ���� ���� ���� �α� ���� - CFieldMapChannel::AddWarPointInMap()�� ����(, BYTE i_byWPUpdateTy=WPUT_GENERAL) �߰�, // 2008-01-07 by dhjin, ���� ���� ���� - 
// 		pFMChann->InsertItemInMap(OUTPOST_PAY_ITEM, OUTPOST_PAY_ITEM_COUNT, WinInfluence);	// 2008-05-20 by dhjin, �¸� ���� ���� ���� ����. pQParam �� ����ϴ� �κ� ���� // 2007-11-06 by dhjin, ������ ���� �߰� 
// 
// 		CFieldMapChannel		*pBackFMChann = m_pFieldIOCP18->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(pFMChann->m_pFieldMapProject->m_nBeforeMapIndex, 0));
// 		if(pBackFMChann)
// 		{
// 			pBackFMChann->AddWarPointInMap(OUTPOST_PAY_WARPOINT, FALSE, WinInfluence, WPUT_OUTPOSTWAR);				// 2009-01-22 by cmkwon, ���� ���� ���� �α� ���� - CFieldMapChannel::AddWarPointInMap()�� ����(, BYTE i_byWPUpdateTy=WPUT_GENERAL) �߰�// 2008-05-20 by dhjin, �¸� ���� ���� ���� ����. pQParam �� ����ϴ� �κ� ����
// 			pBackFMChann->AddWarPointInMap(OUTPOST_PAY_WARPOINT_BY_LOSE, FALSE, LoseInfluence, WPUT_OUTPOSTWAR);	// 2009-01-22 by cmkwon, ���� ���� ���� �α� ���� - CFieldMapChannel::AddWarPointInMap()�� ����(, BYTE i_byWPUpdateTy=WPUT_GENERAL) �߰�	// 2008-01-07 by dhjin, ���� ���� ���� - 
// 			pBackFMChann->InsertItemInMap(OUTPOST_PAY_ITEM, OUTPOST_PAY_ITEM_COUNT, WinInfluence);	// 2008-05-20 by dhjin, �¸� ���� ���� ���� ����. pQParam �� ����ϴ� �κ� ����	// 2007-11-06 by dhjin, ������ ���� �߰� 
// 		}
// 
// 		// 2008-06-20 by cmkwon, EP3 ��� �ý��� ����(�ʴ� ����� 2�� ���� �� �� �ִ�) - ���������� ����� ���2 ���� ����
// 		pBackFMChann = m_pFieldIOCP18->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(pFMChann->m_pFieldMapProject->m_nBeforeMapIndex2, 0));
// 		if(pBackFMChann)
// 		{
// 			pBackFMChann->AddWarPointInMap(OUTPOST_PAY_WARPOINT, FALSE, WinInfluence, WPUT_OUTPOSTWAR);				// 2009-01-22 by cmkwon, ���� ���� ���� �α� ���� - CFieldMapChannel::AddWarPointInMap()�� ����(, BYTE i_byWPUpdateTy=WPUT_GENERAL) �߰�// 2008-05-20 by dhjin, �¸� ���� ���� ���� ����. pQParam �� ����ϴ� �κ� ����
// 			pBackFMChann->AddWarPointInMap(OUTPOST_PAY_WARPOINT_BY_LOSE, FALSE, LoseInfluence, WPUT_OUTPOSTWAR);	// 2009-01-22 by cmkwon, ���� ���� ���� �α� ���� - CFieldMapChannel::AddWarPointInMap()�� ����(, BYTE i_byWPUpdateTy=WPUT_GENERAL) �߰�		// 2008-01-07 by dhjin, ���� ���� ���� - 
// 			pBackFMChann->InsertItemInMap(OUTPOST_PAY_ITEM, OUTPOST_PAY_ITEM_COUNT, WinInfluence);	// 2008-05-20 by dhjin, �¸� ���� ���� ���� ����. pQParam �� ����ϴ� �κ� ����	// 2007-11-06 by dhjin, ������ ���� �߰� 
// 		}
		///////////////////////////////////////////////////////////////////////////////
		// 2009-07-08 by cmkwon, ���� ������ ���� ���� - ���� ���� ����
		tmEvEnd.byWinInfluenceTy	= WinInfluence;
		tmEvEnd.byLossInfluenceTy	= LoseInfluence;
	}

	// 2007-08-21 by dhjin, ���������� ���Ḧ Ŭ���̾�Ʈ���� �����Ѵ�.
	INIT_MSG_WITH_BUFFER(MSG_FC_OUTPOST_WAR_END, T_FC_OUTPOST_WAR_END, SendMsg, SendBuf2);
	SendMsg->Influence		= i_pOutPost->GetOutPostInfluence();
	SendMsg->MapIndex		= i_pOutPost->GetOutPostMapIndex();
	SendMsg->GuildUID		= i_pOutPost->GetOutPostGuildUID();
	util::strncpy(SendMsg->GuildName, i_pOutPost->GetOutPostGuildName(), SIZE_MAX_GUILD_NAME);
	m_pFieldIOCP18->SendMessageToAllClients(SendBuf2,MSG_SIZE(MSG_FC_OUTPOST_RESET_SUCCESS),INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);

// 2013-05-09 by hskim, ���� ����Ʈ ����
#ifdef S_UPGRADE_INFLUENCE_POINT_HSKIM
	m_pFieldIOCP18->m_InflWarManager.CalcContributionPoint(i_pOutPost->GetOutPostInfluence(), CONTRIBUTIONPOINT_OUTPOST_WAR_WINNER, CONTRIBUTIONPOINT_OUTPOST_WAR_LOSER);
#endif
// end 2013-05-09 by hskim, ���� ����Ʈ ����	

// 2009-07-08 by cmkwon, ���� ������ ���� ���� - ���� ó���� COutPostManager::OnDoMinutelyWorkOutPostManager()���� ó���ϰ� ����	
// 	//////////////////////////////////////////////////////////////////////////
// 	// 2008-12-23 by dhjin, ���� ���� �߰���
// 	m_pFieldIOCP18->PayCOutPostWarContributionCalculation(i_pOutPost->GetOutPostMapIndex(), i_pOutPost->GetOutPostInfluence());
// 	m_pFieldIOCP18->DeleteCOutPostWarContributionCalculation(i_pOutPost->GetOutPostMapIndex());	

	// 2009-02-11 by dhjin, �ý��� �α� ����
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] OutPostWarNoti COutPostManager::OutPostWarEnd# 10000 OutPostMapIndex(%6d) \r\n", i_pOutPost->GetOutPostMapIndex());

	// 2007-08-21 by dhjin, �������� ���°��� �����Ѵ�. OUTPOST_NORMAL
	i_pOutPost->SetOutPostState(OUTPOST_NORMAL);
	m_pFieldIOCP18->m_InflWarManager.SetDoingInfluenceWar();	// 2012-11-26 by jhseol, ���� �ý��� ������ - ����� Ʈ���� ��������(�Ͻ�����), ���� ���Ḧ �ǽð����� Ȯ�� �ϱ� ���� �Լ�ȣ��(���������� ����)

	//////////////////////////////////////////////////////////////////////////
	// 2007-08-21 by dhjin, �������� �ʿ� �ִ� ������ ���ǿ� �°� ������Ų��. (���� ������ �ִٸ� ���� ���ܿ��� �������� ���� ������ ����)
	// 1. �켱 ���� ����UID�� ������ ��� ���� ������ ���� (NPC�� �����ϸ� ���� ��ȣ�� 0�̱� ������ ������� ��� �����ȴ�.)
	// 2. ���� ������ NPC�� �ƴ϶�� ���� ���ܿ��� �������� ���� ������ ������Ų��.

// 2009-07-08 by cmkwon, ���� ������ ���� ���� - ���� ó���� COutPostManager::OnDoMinutelyWorkOutPostManager()���� ó���ϰ� ����
//	pFMChann->WarpToCityMapFieldMapChannelByGuildUID(i_pOutPost->GetOutPostGuildUID());
// 	if(FALSE != i_pOutPost->GetOutPostGuildUID()
// 		&& FALSE != i_pOutPost->GetOutPostInfluence()
// 		&& FALSE == i_pOutPost->GetOutPostNextWarTimeSet())
// 	{
// 		pFMChann->WarpToOutPostCityMapFieldMapChannelByGuildUID(i_pOutPost->GetOutPostGuildUID(), i_pOutPost->GetOutPostCityMapIndex());
// 	}
	///////////////////////////////////////////////////////////////////////////////
	// 2009-07-08 by cmkwon, ���� ������ ���� ���� - �Ʒ��� ���� ����� �Ѱܼ� ó���ϵ��� �Ѵ�.
	tmEvEnd.GuildUIDPossessOutPost		= i_pOutPost->GetOutPostGuildUID();
	tmEvEnd.pFMChanOfOutPostWarMap		= pFMChann;
	if(FALSE != i_pOutPost->GetOutPostGuildUID()
		&& FALSE != i_pOutPost->GetOutPostInfluence()
		&& FALSE == i_pOutPost->GetOutPostNextWarTimeSet())
	{
		tmEvEnd.nCityMapIndexForWarp	= i_pOutPost->GetOutPostCityMapIndex();
	}
	io_pEvEndList->push_back(tmEvEnd);


	// 2009-02-11 by dhjin, �ý��� �α� ����
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] OutPostWarNoti COutPostManager::OutPostWarEnd# 10100 OutPostMapIndex(%6d) \r\n", i_pOutPost->GetOutPostMapIndex());

	//////////////////////////////////////////////////////////////////////////
	// 2007-08-27 by dhjin, �ǰ��� �����Ѵ�.
	this->SetExpediencyFundByOutPostWarEnd(i_pOutPost->GetOutPostMapIndex(), i_pOutPost->GetOutPostCityMapIndex(), i_pOutPost->GetOutPostInfluence(), i_pOutPost->GetOutPostGuildUID());

	// 2009-02-11 by dhjin, �ý��� �α� ����
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] OutPostWarNoti COutPostManager::OutPostWarEnd# 10200 OutPostMapIndex(%6d) \r\n", i_pOutPost->GetOutPostMapIndex());

	// 2007-10-02 by dhjin, �α� �����
	// 2007-10-16 by cmkwon, �α� �߰� - SendLogMessageOutPostEnd() �Լ� ���� ����
	CAtumLogSender::SendLogMessageOutPostEnd(i_pOutPost->GetOutPostMapIndex(), i_pOutPost->GetOutPostInfluence(), i_pOutPost->GetOutPostGuildUID(), i_pOutPost->GetOutPostGuildName());

	//////////////////////////////////////////////////////////////////////////
	// 2008-05-21 by dhjin, EP3 - ���� ���� ���� - �������� ����
	// 2007-10-16 by cmkwon, �α� �߰� - SendLogMessageOutPostEnd() �Լ� ���� ����
	INIT_MSG_WITH_BUFFER(MSG_FI_GUILD_OUTPOST, T_FI_GUILD_OUTPOST, pGuildOutPost, GuildOutPostSendBuf);
	pGuildOutPost->GuildUID			= i_pOutPost->GetOutPostGuildUID();
	pGuildOutPost->MapIndex			= i_pOutPost->GetOutPostCityMapIndex();
	m_pFieldIOCP18->Send2IMServer(GuildOutPostSendBuf, MSG_SIZE(MSG_FI_GUILD_OUTPOST));


}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::SetExpediencyFundByOutPostWarEnd(int i_nMapindex, int i_nCityMapindex, byte i_byInfluence, UID32_t i_nGuildUID)
/// \brief		�ǰ��� ����
/// \author		dhjin
/// \date		2007-08-27 ~ 2007-08-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::SetExpediencyFundByOutPostWarEnd(int i_nMapindex, int i_nCityMapindex, byte i_byInfluence, UID32_t i_nGuildUID)
{
	//////////////////////////////////////////////////////////////////////////
	// 1. �ǰ����� ���� ����(����� �����ڸ� ��ȭ�Ѵ�.)
	// 2. �ǰ��� ���� �ʱ�ȭ
	UID32_t GuildCommanderUID = 0;

	if(FALSE != i_nGuildUID)
	{// 2007-08-27 by dhjin, NPC������ �ƴϸ� ������ UID�� �����´�.
		GuildCommanderUID = m_pFieldIOCP18->m_GuildWarManager.GetGuildCommanderUID(i_nGuildUID);
	}

	if(FALSE == i_nGuildUID
		&& FALSE == i_byInfluence)
	{// 2007-08-27 by dhjin, NPC������ �� ���������� �ǰ��� ����.
		// 1. ������ ���� ����
		SCITYLEADER_INFO	tmCityLeaderInfo;
		util::zero(&tmCityLeaderInfo, sizeof(SCITYLEADER_INFO));
		m_pFieldIOCP18->m_CityLeader.GetExpediencyfund(i_nCityMapindex, &tmCityLeaderInfo);
		if(INFLUENCE_TYPE_VCN == tmCityLeaderInfo.Influence)
		{// 2007-08-27 by dhjin, NPC���� ���� ���������� ������ �����ϰ� �־��ٸ� ���������� ������ ������ ���δ�.
			this->UpdateExpediencyfundRateLeader(VCN_CITY_MAP_INDEX, INFLUENCE_TYPE_VCN, -EXPEDIENCYFUND_LEADER_OUTPOST);
		}
		else if(INFLUENCE_TYPE_ANI == tmCityLeaderInfo.Influence)
		{// 2007-08-27 by dhjin, NPC���� ���� �˸��� ������ �����ϰ� �־��ٸ� �˸��� ������ ������ ���δ�.
			this->UpdateExpediencyfundRateLeader(ANI_CITY_MAP_INDEX, INFLUENCE_TYPE_ANI, -EXPEDIENCYFUND_LEADER_OUTPOST);
		}

		// 2. �������� �ǰ��� �ʱ�ȭ
		m_pFieldIOCP18->m_CityLeader.InitExpediencyfund(i_nCityMapindex, i_byInfluence, GuildCommanderUID, EXPEDIENCYFUND_GUILDCOMMANDER);
		QPARAM_INIT_EXPEDIENCY_FUND *pQParam = new QPARAM_INIT_EXPEDIENCY_FUND;
		pQParam->MapIndex					= i_nCityMapindex;
		pQParam->Influence					= i_byInfluence;
		pQParam->CharacterUID				= GuildCommanderUID;
		pQParam->ExpediencyFundCumulative	= 0;
		pQParam->ExpediencyFund				= 0;
		pQParam->ExpediencyRate				= EXPEDIENCYFUND_GUILDCOMMANDER;
		m_pFieldIOCP18->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InitExpediencyFund, NULL, NULL, pQParam);
	}
	else
	{// 2007-08-21 by dhjin, ���� ���� ���������̴�.
		// 1. ������ ������ �����Ѵ�.
		SCITYLEADER_INFO	tmCityLeaderInfo;
		util::zero(&tmCityLeaderInfo, sizeof(SCITYLEADER_INFO));
		m_pFieldIOCP18->m_CityLeader.GetExpediencyfund(i_nCityMapindex, &tmCityLeaderInfo);
		
		// 2007-08-27 by dhjin, ���� ������ ���� ������ ������ �����Ͽ��ٸ� ������ ���ÿ� ��ȭ�� ����.
		if(INFLUENCE_TYPE_UNKNOWN == tmCityLeaderInfo.Influence && INFLUENCE_TYPE_ANI == i_byInfluence) 	// 2007-10-22 by dhjin, �ڽ��� ���� �����϶��� ���þ��� ����.
		{// 2007-08-27 by dhjin, ���� ������ ������ NPC�̰� ���� ������ ������ �˸����̸� �˸��� ������ ������ ������Ų��.
//			this->UpdateExpediencyfundRateLeader(VCN_CITY_MAP_INDEX, INFLUENCE_TYPE_ANI, EXPEDIENCYFUND_LEADER_OUTPOST);
			this->UpdateExpediencyfundRateLeader(ANI_CITY_MAP_INDEX, INFLUENCE_TYPE_ANI, EXPEDIENCYFUND_LEADER_OUTPOST);	// 2009-02-17 by dhjin, �ǰ��� ���� ����(K0001196) 
		}
		else if(INFLUENCE_TYPE_UNKNOWN == tmCityLeaderInfo.Influence && INFLUENCE_TYPE_VCN == i_byInfluence) 	// 2007-10-22 by dhjin, �ڽ��� ���� �����϶��� ���þ��� ����.
		{// 2007-08-27 by dhjin, ���� ������ ������ NPC�̰� ���� ������ ������ �����������̸� ���������� ������ ������ ������Ų��.
			this->UpdateExpediencyfundRateLeader(VCN_CITY_MAP_INDEX, INFLUENCE_TYPE_VCN, EXPEDIENCYFUND_LEADER_OUTPOST);
		}
		else if(INFLUENCE_TYPE_VCN == tmCityLeaderInfo.Influence && INFLUENCE_TYPE_ANI == i_byInfluence)	// 2007-10-22 by dhjin, �ڽ��� ���� �����϶��� ���þ��� ����.
		{// 2007-08-27 by dhjin, ���� ������ ������ �����������̰� ���� ������ ������ �˸����̸� ���������� ������ ������ ���̰� �˸��� ������ ������ ������Ų��.
			this->UpdateExpediencyfundRateLeader(VCN_CITY_MAP_INDEX, INFLUENCE_TYPE_VCN, -EXPEDIENCYFUND_LEADER_OUTPOST);
			this->UpdateExpediencyfundRateLeader(ANI_CITY_MAP_INDEX, INFLUENCE_TYPE_ANI, EXPEDIENCYFUND_LEADER_OUTPOST);
		}
		else if(INFLUENCE_TYPE_ANI == tmCityLeaderInfo.Influence && INFLUENCE_TYPE_VCN == i_byInfluence) 	// 2007-10-22 by dhjin, �ڽ��� ���� �����϶��� ���þ��� ����.
		{// 2007-08-27 by dhjin, ���� ������ ������ �����������̰� ���� ������ ������  �����������̸� �˸��� ������ ������ ���̰� ���������� ������ ������ ������Ų��.
			this->UpdateExpediencyfundRateLeader(VCN_CITY_MAP_INDEX, INFLUENCE_TYPE_VCN, EXPEDIENCYFUND_LEADER_OUTPOST);
			this->UpdateExpediencyfundRateLeader(ANI_CITY_MAP_INDEX, INFLUENCE_TYPE_ANI, -EXPEDIENCYFUND_LEADER_OUTPOST);
		}

		// 2. �������� �ǰ��� �ʱ�ȭ
		m_pFieldIOCP18->m_CityLeader.InitExpediencyfund(i_nCityMapindex, i_byInfluence, GuildCommanderUID, EXPEDIENCYFUND_GUILDCOMMANDER);
		QPARAM_INIT_EXPEDIENCY_FUND *pQParam = new QPARAM_INIT_EXPEDIENCY_FUND;
		pQParam->MapIndex					= i_nCityMapindex;
		pQParam->Influence					= i_byInfluence;
		pQParam->CharacterUID				= GuildCommanderUID;
		pQParam->ExpediencyFundCumulative	= 0;
		pQParam->ExpediencyFund				= 0;
		pQParam->ExpediencyRate				= EXPEDIENCYFUND_GUILDCOMMANDER;
		m_pFieldIOCP18->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InitExpediencyFund, NULL, NULL, pQParam);
	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::UpdateExpediencyfundRateLeader(int i_nMapIndx, byte i_byInfluence, int i_nChangeExpediencyFundRate)
/// \brief		������ �ǰ����� ������ �����Ѵ�.
/// \author		dhjin
/// \date		2007-08-28 ~ 2007-08-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::UpdateExpediencyfundRateLeader(int i_nMapIndx, byte i_byInfluence, int i_nChangeExpediencyFundRate)
{
	SCITYLEADER_INFO tmCityLeaderInfo;
	util::zero(&tmCityLeaderInfo, sizeof(SCITYLEADER_INFO));
	m_pFieldIOCP18->m_CityLeader.UpdateExpediencyfundRateATLeader(i_nMapIndx, i_nChangeExpediencyFundRate, &tmCityLeaderInfo);
	
	QPARAM_SET_EXPEDIENCY_FUNDRATE *pQParam = new QPARAM_SET_EXPEDIENCY_FUNDRATE;
	pQParam->MapIndex					= i_nMapIndx;
	pQParam->Influence					= i_byInfluence;
	pQParam->CharacterUID				= tmCityLeaderInfo.CharacterUID;
	pQParam->ExpediencyRate				= tmCityLeaderInfo.ExpediencyFundRate;
	m_pFieldIOCP18->m_pAtumDBManager->MakeAndEnqueueQuery(QT_SetExpediencyFundRate, NULL, NULL, pQParam);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::SetInitOutPostMonsterSummonByBell(byte i_byInfl, int * o_byBell1, int * o_byBell2, int * o_byBell3)
/// \brief		�������� �ʱ�ȭ �� �� ���͸� �����ϱ� ���� Bell�� ����
/// \author		dhjin
/// \date		2007-08-24 ~ 2007-08-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::SetInitOutPostMonsterSummonByBell(byte i_byInfl, int * o_byBell1, int * o_byBell2, int * o_byBell3)
{
	// 2007-08-24 by dhjin, Bell �⺻�� ���� -1 �̾�� �Ѵ�.
	*o_byBell1 = -1;
	*o_byBell2 = -1;
	*o_byBell3 = -1;

	// 2007-08-24 by dhjin, NPC �����϶�
	if(INFLUENCE_TYPE_UNKNOWN == i_byInfl)
	{
		*o_byBell1 = BELL_ATATTACK;
		*o_byBell2 = BELL_ATTACK_OUTPOST_PROTECTOR;

	}
	else if(INFLUENCE_TYPE_VCN == i_byInfl)
	{
		*o_byBell1 = BELL_INFLUENCE_VCN;
		*o_byBell2 = BELL_OUTPOST_PROTECTOR_VCN;
	}
	else if(INFLUENCE_TYPE_ANI== i_byInfl)
	{
		*o_byBell1 = BELL_INFLUENCE_ANI;
		*o_byBell2 = BELL_OUTPOST_PROTECTOR_ANI;
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::AddDamageOfProtectorW(int i_nMapIndex, BYTE i_byInfluence, UID32_t i_nGuildUID, CHAR * i_szGuildName, float i_fDamage)
/// \brief		�������� ��ȣ���� �������� �� ���ܰ� ������ ���� �����Ѵ�.
/// \author		dhjin
/// \date		2007-08-24 ~ 2007-08-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::AddDamageOfProtectorW(int i_nMapIndex, BYTE i_byInfluence, UID32_t i_nGuildUID, CHAR * i_szGuildName, float i_fDamage)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);

	COutPost * tmOutPost = this->GetOutPost(i_nMapIndex);
	if(NULL == tmOutPost)
	{
		return;
	}

	tmOutPost->AddDamageOfProtector(i_byInfluence, i_nGuildUID, i_szGuildName, i_fDamage);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::MakeMsgOutPostNextWarInfoRequest(int i_nMapIndex, BOOL i_bCheckGuildCommander, BYTE * o_pData, int * o_nSize)
/// \brief		�������� �ð� ���� ���� ����
/// \author		dhjin
/// \date		2007-08-28 ~ 2007-08-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::MakeMsgOutPostNextWarInfoRequest(int i_nMapIndex, BOOL i_bCheckGuildCommander, BYTE * o_pData, int * o_nSize)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);

	COutPost * tmOutPost = this->GetOutPostCityMap(i_nMapIndex);
	if(NULL == tmOutPost)
	{
		return;
	}
	
	INIT_MSG_WITH_BUFFER(MSG_FC_OUTPOST_NEXTWAR_INFO_REQUEST_OK, T_FC_OUTPOST_NEXTWAR_INFO_REQUEST_OK, SendMsg, SendBuf);
	SendMsg->OutPostNextWarTime		= *(tmOutPost->GetOutPostNextWarTime());
	if(i_bCheckGuildCommander)
	{// 2007-08-28 by dhjin, �������̸� �����ϴ� �ð� �� ����� �����Ѵ�.
		SendMsg->OutPostNextWarTimeListCount = MakeMSG_FC_OUTPOST_NEXTWAR_INFO_REQUEST_OK((ATUM_DATE_TIME*)(SendBuf+MSG_SIZE(MSG_FC_OUTPOST_NEXTWAR_INFO_REQUEST_OK)), &(SendMsg->OutPostNextWarSelectTimeChoice), &(SendMsg->OutPostNextWarTime));
		if(0 > SendMsg->OutPostNextWarTimeListCount)
		{
			return;
		}
		
	}

	*o_nSize = MSG_SIZE(MSG_FC_OUTPOST_NEXTWAR_INFO_REQUEST_OK)+SendMsg->OutPostNextWarTimeListCount*sizeof(ATUM_DATE_TIME);
	memcpy(o_pData, SendBuf, *o_nSize);
	
}

INT	COutPostManager::MakeMSG_FC_OUTPOST_NEXTWAR_INFO_REQUEST_OK(ATUM_DATE_TIME * o_pOutPostNextWarTimeListInfo, int * o_nSelectTimeChoice, ATUM_DATE_TIME * i_OutPostNextWarTime)
{
	INT	OutPostNextWarTimeListCount = 0;

	vectATUM_DATE_TIME::iterator itr(m_vectOutPostNextWarTimeInfo.begin());
	for(; itr != m_vectOutPostNextWarTimeInfo.end(); itr++)
	{
		if(i_OutPostNextWarTime->Hour == itr->Hour
			&& i_OutPostNextWarTime->Minute == itr->Minute
			&& i_OutPostNextWarTime->Second == itr->Second)
		{// 2007-08-28 by dhjin, ������ �ð��� ������ �ð��� ���õ� �ε��� ���� �����Ѵ�.
			*o_nSelectTimeChoice = OutPostNextWarTimeListCount;
		}
		o_pOutPostNextWarTimeListInfo[OutPostNextWarTimeListCount]		= *itr;
		OutPostNextWarTimeListCount++;
	}

	return OutPostNextWarTimeListCount;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			Err_t COutPostManager::OutPostNextWarTimeSetByChoiceUserW(int i_nMapIndex, int i_nSelectTimeChoice, UID32_t i_guildUID, UID32_t i_charUID)
/// \brief		// 2007-10-16 by cmkwon, �α� �߰� - �����߰�(, UID32_t i_guildUID, UID32_t i_charUID), �������� ���� ���� �ð� ����
/// \author		dhjin
/// \date		2007-08-28 ~ 2007-08-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
Err_t COutPostManager::OutPostNextWarTimeSetByChoiceUserW(int i_nMapIndex, int i_nSelectTimeChoice, UID32_t i_guildUID, UID32_t i_charUID)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);

	COutPost * tmOutPost = this->GetOutPostCityMap(i_nMapIndex);
	if(NULL == tmOutPost)
	{
		return ERR_CANNOT_SEARCH_OUTPOST;
	}

	if(tmOutPost->GetOutPostNextWarTimeSet())
	{
		return ERR_ALREADY_SET_OUTPOST_NEXTWARTIME;
	}
	
	int TempCount = 0;			// 2007-08-28 by dhjin, Ŭ���̾�Ʈ�� ������ �ð� ������°�� ã�� ���� 
	vectATUM_DATE_TIME::iterator itr(m_vectOutPostNextWarTimeInfo.begin());
	for(; itr != m_vectOutPostNextWarTimeInfo.end(); itr++)
	{
		if(i_nSelectTimeChoice == TempCount)
		{// 2007-08-28 by dhjin, Ŭ���̾�Ʈ�� ������ �ð� ������°�� ���� �����̸� ���� ���� �ð����� �����Ѵ�.
			tmOutPost->SetOutPostNextWarTimeByChoiceUser(&*itr);
	
			// 2007-09-11 by dhjin, �������� ���� ���� �ð� ���� �Ұ��� ����
			tmOutPost->SetOutPostNextWarTimeSet(TRUE);
			
			// 2007-09-11 by dhjin, DB�� ����
			QPARAM_SET_OUTPOST_NEXTWAR_TIME *pQParam = new QPARAM_SET_OUTPOST_NEXTWAR_TIME;
			pQParam->MapIndex					= i_nMapIndex;
			pQParam->Influence					= tmOutPost->GetOutPostInfluence();
			pQParam->GuildUID					= tmOutPost->GetOutPostGuildUID();
			pQParam->NextWarTime				= *(tmOutPost->GetOutPostNextWarTime());
			m_pFieldIOCP18->m_pAtumDBManager->MakeAndEnqueueQuery(QT_SetOutPostNextWarTime, NULL, NULL, pQParam);

			// 2007-10-16 by cmkwon, �α� �߰� - 
			CAtumLogSender::SendLogMessageOutPostSetNextTime(tmOutPost->GetOutPostMapIndex(), tmOutPost->GetOutPostInfluence(), tmOutPost->GetOutPostGuildUID(), i_guildUID, i_charUID, tmOutPost->GetOutPostNextWarTime());

			return ERR_NO_ERROR;
		}
		TempCount++;
	}	
	
	return ERR_CANNOT_SEARCH_OUTPOST_NEXTWARTIME;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL COutPostManager::CheckOutPostWarTimeStrategyPoint(ATUM_DATE_TIME *i_pStrategyPointSummonTime)
/// \brief		��������Ʈ ��ȯ �� �������� ���� �ð��� �ɸ����� üũ
/// \author		dhjin
/// \date		2007-09-03 ~ 2007-09-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL COutPostManager::CheckOutPostWarTimeStrategyPoint(ATUM_DATE_TIME *i_pStrategyPointSummonTime)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);
	
	mtvectOutPost::iterator	itr = m_mtvectOutPostManager.begin();
	for(; itr != m_mtvectOutPostManager.end(); itr++)
	{
		ATUM_DATE_TIME	tmOutPostWarStartTime; 
		ATUM_DATE_TIME	tmOutPostWarEndTime;
		tmOutPostWarStartTime = *((*itr)->GetOutPostNextWarTime());
		tmOutPostWarStartTime.AddDateTime(0,0,0,-1,0,0);	// 2007-09-03 by dhjin, �������� ���� �յڷ� 1�ð��� ������ �д�.
		tmOutPostWarEndTime = tmOutPostWarStartTime;
		tmOutPostWarEndTime.AddDateTime(0,0,0,4,0,0);
		if(tmOutPostWarStartTime <= *i_pStrategyPointSummonTime
			&& tmOutPostWarEndTime >= *i_pStrategyPointSummonTime)
		{// 2007-09-03 by dhjin, �������� ������ �߻��ϴ� �ð��̸� ��������Ʈ�� �������� �ʴ´�.
			return FALSE;
		}
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ATUM_DATE_TIME * COutPostManager::GetOutPostNextWarTime(int i_nMapIndex)
/// \brief		�������� ���� ���� �ð� ������
/// \author		dhjin
/// \date		2007-09-06 ~ 2007-09-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ATUM_DATE_TIME * COutPostManager::GetOutPostNextWarTime(int i_nMapIndex)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);
	
	COutPost * tmOutPost = this->GetOutPost(i_nMapIndex);
	if(NULL == tmOutPost)
	{
		return NULL;
	}

	return tmOutPost->GetOutPostNextWarTime();
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ATUM_DATE_TIME * COutPostManager::GetOutPostNextWarTimeByCityMapIndex(int i_nCityMapIndex)
/// \brief		���� ������ �������� ���� ���� �ð� ������
/// \author		dhjin
/// \date		2007-09-11 ~ 2007-09-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ATUM_DATE_TIME * COutPostManager::GetOutPostNextWarTimeByCityMapIndex(int i_nCityMapIndex)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);
	
	COutPost * tmOutPost = this->GetOutPostCityMap(i_nCityMapIndex);
	if(NULL == tmOutPost)
	{
		return NULL;
	}

	return tmOutPost->GetOutPostNextWarTime();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL COutPostManager::CheckOutPostWaring(int i_nMapIndex)
/// \brief		�������� ������ ���������� üũ
/// \author		dhjin
/// \date		2007-09-07 ~ 2007-09-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL COutPostManager::CheckOutPostWaring(int i_nMapIndex)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);
	
	COutPost * tmOutPost = this->GetOutPost(i_nMapIndex);
	if(NULL == tmOutPost)
	{
		return FALSE;
	}

	if(IS_OUTPOST(tmOutPost->GetOutPostState()))
	{
		return TRUE;
	}
	
	return FALSE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL COutPostManager::CheckALLOutPostWaring()
/// \brief		�������� ������ ���������� üũ ��� ��
/// \author		dhjin
/// \date		2007-11-09 ~ 2007-11-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL COutPostManager::CheckALLOutPostWaring()
{
	mt_auto_lock mta(&m_mtvectOutPostManager);
	
	mtvectOutPost::iterator itr = m_mtvectOutPostManager.begin();
	for(; itr != m_mtvectOutPostManager.end(); itr++)
	{
		if(IS_OUTPOST((*itr)->GetOutPostState()))
		{
			return TRUE;
		}
	}	

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL COutPostManager::CheckOutPostPossessByGuildUID(UID32_t i_nGuildUID)
/// \brief		���������� ������ �������� üũ
/// \author		dhjin
/// \date		2007-11-09 ~ 2007-11-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL COutPostManager::CheckOutPostPossessByGuildUID(UID32_t i_nGuildUID)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);
	
	mtvectOutPost::iterator itr = m_mtvectOutPostManager.begin();
	for(; itr != m_mtvectOutPostManager.end(); itr++)
	{
		if(i_nGuildUID == (*itr)->GetOutPostGuildUID())
		{
			return TRUE;
		}
	}	

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL COutPostManager::CheckOutPostResetByGuildUID(UID32_t i_nGuildUID)
/// \brief		���������� �������� �������� üũ
/// \author		dhjin
/// \date		2007-12-13 ~ 2007-12-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL COutPostManager::CheckOutPostResetByGuildUID(UID32_t i_nGuildUID)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);
	
	mtvectOutPost::iterator itr = m_mtvectOutPostManager.begin();
	for(; itr != m_mtvectOutPostManager.end(); itr++)
	{
		if(i_nGuildUID == (*itr)->GetOutPostResetPossibleGuildUID())
		{
			return TRUE;
		}
	}	

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT	COutPostManager::CheckAllOutPostWaringAndSendClient(SOUTPOST_WAR_INFO *o_pOutPostWarInfo)
/// \brief		�������� ������ �߹��ߴ��� üũ�Ͽ� Ŭ���̾�Ʈ���� ������ �����Ѵ�.
/// \author		dhjin
/// \date		2007-09-14 ~ 2007-09-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT	COutPostManager::CheckAllOutPostWaringAndSendClient(SOUTPOST_WAR_INFO *o_pOutPostWarInfo)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);
	
	int	OutPostWarInfoCount = 0;

	ATUM_DATE_TIME tmCurrentTime;
	tmCurrentTime.SetCurrentDateTime();

	mtvectOutPost::iterator itr = m_mtvectOutPostManager.begin();
	for(; itr != m_mtvectOutPostManager.end(); itr++)
	{
		if(IS_OUTPOST((*itr)->GetOutPostState()))
		{
			o_pOutPostWarInfo[OutPostWarInfoCount].OutPostState			= (*itr)->GetOutPostState();
			o_pOutPostWarInfo[OutPostWarInfoCount].OutPostResetIngInfluence		= (*itr)->GetOutPostResetPossibleInfluence();
			o_pOutPostWarInfo[OutPostWarInfoCount].MapIndex				= (*itr)->GetOutPostMapIndex();
			o_pOutPostWarInfo[OutPostWarInfoCount].OutPostWarStartTime	= *((*itr)->GetOutPostNextWarTime());
			o_pOutPostWarInfo[OutPostWarInfoCount].OutPostWarEndTime	= o_pOutPostWarInfo[OutPostWarInfoCount].OutPostWarStartTime;
			o_pOutPostWarInfo[OutPostWarInfoCount].OutPostWarEndTime.AddDateTime(0,0,0,0,OUTPOST_WARTIME,0);
			if(g_pFieldGlobal->IsTestServer())
			{// 2008-10-29 by cmkwon, �׼� �ý��� ����(���������� 1�ð�(60��)���� ����) - 
				o_pOutPostWarInfo[OutPostWarInfoCount].OutPostWarEndTime	= o_pOutPostWarInfo[OutPostWarInfoCount].OutPostWarStartTime;
				o_pOutPostWarInfo[OutPostWarInfoCount].OutPostWarEndTime.AddDateTime(0,0,0,0,OUTPOST_WARTIME_FOR_TESTSERVER,0);
			}
			o_pOutPostWarInfo[OutPostWarInfoCount].OutPostWarResetRamainSecondTime = (*itr)->GetOutPostResetEndTime()->GetTimeDiffTimeInSeconds(tmCurrentTime);
//			o_pOutPostWarInfo[OutPostWarInfoCount].OutPostWarResetStartTime = *((*itr)->GetOutPostResetStartTime());
//			o_pOutPostWarInfo[OutPostWarInfoCount].OutPostWarResetEndTime = *((*itr)->GetOutPostResetEndTime());
			OutPostWarInfoCount++;
		}
	}
	
	return OutPostWarInfoCount;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void COutPostManager::OutPostPossessNPCByDeleteGuild(UID32_t i_nGuildUID)
/// \brief		���� ���� ��û���� �������� ���� �ϰ� �ִ� �����̾��� ��� NPC������ ����
/// \author		dhjin
/// \date		2007-11-09 ~ 2007-11-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void COutPostManager::OutPostPossessNPCByDeleteGuild(UID32_t i_nGuildUID)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);

	mtvectOutPost::iterator	itr = m_mtvectOutPostManager.begin();
	for(; itr != m_mtvectOutPostManager.end(); itr++)
	{
		if(i_nGuildUID == (*itr)->GetOutPostGuildUID())
		{
			// 2007-08-24 by dhjin, �������� NPC���� ó��
			(*itr)->SetOutPostPossessNPCByReset();
			QPARAM_SET_NPCPOSSESSION_OUTPOST *pQParamNPC = new QPARAM_SET_NPCPOSSESSION_OUTPOST;
			pQParamNPC->MapIndex			= (*itr)->GetOutPostMapIndex();
			pQParamNPC->GetTime			= *((*itr)->GetPotPostGetTime());
			pQParamNPC->NextWarTime		= *((*itr)->GetOutPostNextWarTime());
			m_pFieldIOCP18->m_pAtumDBManager->MakeAndEnqueueQuery(QT_SetNPCPossessionOutPost, NULL, NULL, pQParamNPC);
			
			
			INT CityMapIndex = (*itr)->GetOutPostCityMapIndex();

			SCITYLEADER_INFO	tmCityLeaderInfo;
			util::zero(&tmCityLeaderInfo, sizeof(SCITYLEADER_INFO));
			m_pFieldIOCP18->m_CityLeader.GetExpediencyfund(CityMapIndex, &tmCityLeaderInfo);
			BYTE	tmInfluence = tmCityLeaderInfo.Influence;
			if(INFLUENCE_TYPE_VCN == tmCityLeaderInfo.Influence)
			{// 2007-08-27 by dhjin, NPC���� ���� ���������� ������ �����ϰ� �־��ٸ� ���������� ������ ������ ���δ�.
				this->UpdateExpediencyfundRateLeader(VCN_CITY_MAP_INDEX, INFLUENCE_TYPE_VCN, -EXPEDIENCYFUND_LEADER_OUTPOST);
			}
			else if(INFLUENCE_TYPE_ANI == tmCityLeaderInfo.Influence)
			{// 2007-08-27 by dhjin, NPC���� ���� �˸��� ������ �����ϰ� �־��ٸ� �˸��� ������ ������ ���δ�.
				this->UpdateExpediencyfundRateLeader(ANI_CITY_MAP_INDEX, INFLUENCE_TYPE_ANI, -EXPEDIENCYFUND_LEADER_OUTPOST);
			}

			// 2. �������� �ǰ��� �ʱ�ȭ
			m_pFieldIOCP18->m_CityLeader.InitExpediencyfund(CityMapIndex, 0, 0, EXPEDIENCYFUND_GUILDCOMMANDER);
			QPARAM_INIT_EXPEDIENCY_FUND *pQParam = new QPARAM_INIT_EXPEDIENCY_FUND;
			pQParam->MapIndex					= CityMapIndex;
			pQParam->Influence					= 0;
			pQParam->CharacterUID				= 0;
			pQParam->ExpediencyFundCumulative	= 0;
			pQParam->ExpediencyFund				= 0;
			pQParam->ExpediencyRate				= EXPEDIENCYFUND_GUILDCOMMANDER;
			m_pFieldIOCP18->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InitExpediencyFund, NULL, NULL, pQParam);	

			return;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT	COutPostManager::MakeAllOutPostInfoMsg(SCITY_WARINFO_OUTPOST *o_pOutPostWarInfo)
/// \brief		�������� ��� ���� ����Ʈ �����
/// \author		dhjin
/// \date		2007-12-03 ~ 2007-12-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT	COutPostManager::MakeAllOutPostInfoMsg(SCITY_WARINFO_OUTPOST *o_pOutPostWarInfo)
{
	mt_auto_lock mta(&m_mtvectOutPostManager);
	
	int	OutPostWarInfoCount = 0;

	mtvectOutPost::iterator itr = m_mtvectOutPostManager.begin();
	for(; itr != m_mtvectOutPostManager.end(); itr++)
	{
		o_pOutPostWarInfo[OutPostWarInfoCount].Influence	= (*itr)->GetOutPostInfluence();
		o_pOutPostWarInfo[OutPostWarInfoCount].MapIndex		= (*itr)->GetOutPostCityMapIndex();
		o_pOutPostWarInfo[OutPostWarInfoCount].GuildUID		= (*itr)->GetOutPostGuildUID();
		m_pFieldIOCP18->m_GuildWarManager.GetGuildCommanderName(o_pOutPostWarInfo[OutPostWarInfoCount].GuildUID, o_pOutPostWarInfo[OutPostWarInfoCount].GuildCommander);
		util::strncpy(o_pOutPostWarInfo[OutPostWarInfoCount].GuildName, (*itr)->GetOutPostGuildName(), SIZE_MAX_GUILD_NAME);
		o_pOutPostWarInfo[OutPostWarInfoCount].OutpostDate	= *((*itr)->GetOutPostNextWarTime());
		OutPostWarInfoCount++;
	}
	
	return OutPostWarInfoCount;
}

