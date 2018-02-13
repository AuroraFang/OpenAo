// InfinityManager.cpp: implementation of the CInfinityManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InfinityManager.h"
#include "FieldGlobal.h"
#include "InfinityTickManager.h"
#include "FieldIOCPSocket.h"			// 2010-04-05 by cmkwon, ���� ������ ī�� ���� �ý��� ���� - 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInfinityManager::CInfinityManager()
{
	InitializeCriticalSection(&m_criticalSectionCreate);	// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
	this->InitInfinityManager();
}

CInfinityManager::~CInfinityManager()
{
	DeleteCriticalSection(&m_criticalSectionCreate);		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
	m_pTickManager->CleanThread();
	util::del(m_pTickManager);
}


void CInfinityManager::InitInfinityManager() 
{
	m_vectInfiModeInfo.clear();
	m_vectInfiMonsterInfo.clear();
	m_mtvectInfiBossRush.clear();
	m_mtvectInfiDefence.clear();
	m_mtvectInfiMShipBattle.clear();			// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
	m_nInfinityCreateUID = 1;
	m_mtInfinityDisConnectUserList.clearLock();		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��
	m_pTickManager = new CInfinityTickManager(this);
}

void CInfinityManager::SetInfinityMapManagerW(CFieldMapWorkspace * i_pFieldMapWorkspace) {
	this->m_InfiMapManager.SetInfinityMapManager(i_pFieldMapWorkspace);
}	

BOOL CInfinityManager::SetDBManager(CAtumFieldDBManager * i_pAtumDBManager) {
	if(NULL == i_pAtumDBManager) {
		return FALSE;
	}
	m_pAtumDBManager = i_pAtumDBManager;

	return TRUE;
}

void CInfinityManager::SetCinemaInfoW(vectorCinemaInfo * i_pVectCinemaInfo) {
	this->m_Cinema.SetCinemaInfo(i_pVectCinemaInfo);
}

void CInfinityManager::SetRevisionInfoW(vectorRevisionInfo * i_pVectRevisionInfo) {
	this->m_Revision.SetRevisionInfo(i_pVectRevisionInfo);
}

void CInfinityManager::SetDBInfinityModeInfo(vectorInfinityModeInfo * i_pVectInfiModeInfo) {
	if(i_pVectInfiModeInfo) {
		m_vectInfiModeInfo.clear();
		m_vectInfiModeInfo.assign(i_pVectInfiModeInfo->begin(), i_pVectInfiModeInfo->end());	
		
		m_InfiMapManager.InitInfinityMapInfoList(&m_vectInfiModeInfo);
	}
}

void CInfinityManager::SetDBInfinityMonsterInfo(vectorInfinityMonsterInfo * i_pVectInfiMonsterInfo) {
	if(i_pVectInfiMonsterInfo) {
		m_vectInfiMonsterInfo.clear();
		m_vectInfiMonsterInfo.assign(i_pVectInfiMonsterInfo->begin(), i_pVectInfiMonsterInfo->end());
	}
}

void CInfinityManager::GetInfinityModeInfo(vectorInfinityModeInfo * o_pCopyVectInfiModeInfo) {
	if(o_pCopyVectInfiModeInfo) {
		o_pCopyVectInfiModeInfo->clear();
		o_pCopyVectInfiModeInfo->assign(m_vectInfiModeInfo.begin(), m_vectInfiModeInfo.end());
	}
}

INT CInfinityManager::MakeMsgInfinityPlayingList(INFINITY_READY_LIST * o_pInfinityPlayingList, MapIndex_t i_nInfinityMapIdx, eINFINITY_MODE i_nInfinityMode, BYTE i_byInfluenceType) 
{
	if( NULL == o_pInfinityPlayingList ) 
	{
		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
		return FALSE;
	}

	INT PlayingListCount = 0;

	switch (i_nInfinityMode) 
	{
		case INFINITY_MODE_BOSSRUSH: 
			{
				mt_auto_lock mta(&m_mtvectInfiBossRush);
				mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();

				for( ; itr != m_mtvectInfiBossRush.end(); itr++ ) 
				{
					if(SIZE_MAX_PACKET < MSG_SIZE(MSG_FC_INFINITY_READY_LIST_OK) + sizeof(INFINITY_READY_LIST) * (PlayingListCount+1)) 
					{
						// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
						break;
					}

					if( i_nInfinityMapIdx == (*itr)->GetInfinityMapIndex() 
						&& INFINITY_STATE_UNPREPARED == (*itr)->GetInfinityState()
						&& i_byInfluenceType == (*itr)->GetInfluenceType() ) 
					{
						o_pInfinityPlayingList[PlayingListCount].InfinityCreateUID			= (*itr)->GetInfinityCreateUID();
						o_pInfinityPlayingList[PlayingListCount].PlayingRoomMemberCount		= (*itr)->GetPlayerListSize();
						o_pInfinityPlayingList[PlayingListCount].MaxMemberCount				= (*itr)->GetMaxPlayerSize();
						util::strncpy(o_pInfinityPlayingList[PlayingListCount].MasterName, (*itr)->GetMasterPlayerName(), SIZE_MAX_CHARACTER_NAME);
						util::strncpy(o_pInfinityPlayingList[PlayingListCount].InfinityTeamName, (*itr)->GetInfinityTeamName(), SIZE_MAX_PARTY_NAME);

						// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
						o_pInfinityPlayingList[PlayingListCount].DifficultLevel				= (*itr)->GetDifficultyLevel();

						PlayingListCount++;
					}
				}

				mta.auto_unlock_cancel();
			} // case INFINITY_MODE_BOSSRUSH
			break; 

		case INFINITY_MODE_DEFENCE: 
			{
				mt_auto_lock mta(&m_mtvectInfiDefence);
				mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();

				for( ; itr != m_mtvectInfiDefence.end(); itr++ ) 
				{
					if( SIZE_MAX_PACKET < MSG_SIZE(MSG_FC_INFINITY_READY_LIST_OK) + sizeof(INFINITY_READY_LIST) * (PlayingListCount+1)) 
					{
						// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
						break;
					}

					if( i_nInfinityMapIdx == (*itr)->GetInfinityMapIndex()
						&& INFINITY_STATE_UNPREPARED == (*itr)->GetInfinityState()
						&& i_byInfluenceType == (*itr)->GetInfluenceType() ) 
					{
						o_pInfinityPlayingList[PlayingListCount].InfinityCreateUID			= (*itr)->GetInfinityCreateUID();
						o_pInfinityPlayingList[PlayingListCount].PlayingRoomMemberCount		= (*itr)->GetPlayerListSize();
						o_pInfinityPlayingList[PlayingListCount].MaxMemberCount				= (*itr)->GetMaxPlayerSize();
						util::strncpy(o_pInfinityPlayingList[PlayingListCount].MasterName, (*itr)->GetMasterPlayerName(), SIZE_MAX_CHARACTER_NAME);
						util::strncpy(o_pInfinityPlayingList[PlayingListCount].InfinityTeamName, (*itr)->GetInfinityTeamName(), SIZE_MAX_PARTY_NAME);

						// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
						o_pInfinityPlayingList[PlayingListCount].DifficultLevel				= (*itr)->GetDifficultyLevel();

						PlayingListCount++;
					}
				}

				mta.auto_unlock_cancel();
			} // case INFINITY_MODE_DEFENCE
			break; 	   

        // 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾� 
		case INFINITY_MODE_MSHIPBATTLE:				
			{
				mt_auto_lock mta(&m_mtvectInfiMShipBattle);
				mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
				
				for( ; itr != m_mtvectInfiMShipBattle.end(); itr++ ) 
				{
					if( SIZE_MAX_PACKET < MSG_SIZE(MSG_FC_INFINITY_READY_LIST_OK) + sizeof(INFINITY_READY_LIST) * (PlayingListCount+1)) 
					{
						break;
					}
					
					if( i_nInfinityMapIdx == (*itr)->GetInfinityMapIndex()
						&& INFINITY_STATE_UNPREPARED == (*itr)->GetInfinityState()
						&& i_byInfluenceType == (*itr)->GetInfluenceType() ) 
					{
						o_pInfinityPlayingList[PlayingListCount].InfinityCreateUID			= (*itr)->GetInfinityCreateUID();
						o_pInfinityPlayingList[PlayingListCount].PlayingRoomMemberCount		= (*itr)->GetPlayerListSize();
						o_pInfinityPlayingList[PlayingListCount].MaxMemberCount				= (*itr)->GetMaxPlayerSize();
						util::strncpy(o_pInfinityPlayingList[PlayingListCount].MasterName, (*itr)->GetMasterPlayerName(), SIZE_MAX_CHARACTER_NAME);
						util::strncpy(o_pInfinityPlayingList[PlayingListCount].InfinityTeamName, (*itr)->GetInfinityTeamName(), SIZE_MAX_PARTY_NAME);

						o_pInfinityPlayingList[PlayingListCount].DifficultLevel				= (*itr)->GetDifficultyLevel();
						
						PlayingListCount++;
					}
				}
				
				mta.auto_unlock_cancel();
			} // case INFINITY_MODE_MSHIPBATTLE
			break; 										   
       // end 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
	}

	return PlayingListCount;
}

BOOL CInfinityManager::CheckInfinityModeLevel(InfiModeUID_t i_nInfinityModeUID, Lv_t i_Lv) {
	vectorInfinityModeInfo::iterator itr = m_vectInfiModeInfo.begin();
	for(; itr != m_vectInfiModeInfo.end(); itr++) {
		if(i_nInfinityModeUID == itr->InfinityModeUID) {
			if(i_Lv >= itr->MinLv && i_Lv <= itr->MaxLv) {
				return TRUE;
			}
			else {
				return FALSE;
			}
		}
	}
	return FALSE;
}
// 2010-03-23 by cmkwon, ���Ǵ�Ƽ ���� ĳ�� ������ ���� - 
BOOL CInfinityManager::CheckEntranceCount(InfiModeUID_t i_nInfinityModeUID, EntranceCount_t i_EntranceCount, int i_nAddLimiteCnt/*=0*/)
{
	vectorInfinityModeInfo::iterator itr = m_vectInfiModeInfo.begin();
	for(; itr != m_vectInfiModeInfo.end(); itr++) {
		if(i_nInfinityModeUID == itr->InfinityModeUID) {
// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ, �ذ� ���� ����
//			if(i_EntranceCount <= itr->EntranceCount) {
			if(i_EntranceCount < itr->EntranceCount + i_nAddLimiteCnt) // 2010-03-23 by cmkwon, ���Ǵ�Ƽ ���� ĳ�� ������ ���� - �߰� ī��Ʈ
			{	
				return TRUE;
			}
			else 
			{
				return FALSE;
			}
		}
	}
	return FALSE;
}


// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
/******************************************************************************************************************
**
**	���Ǵ�Ƽ �� ����.
**
**	Create Info :	??. ??. ??
**
**	Update Info : 2010. 05. 28. by hsLee. - ���Ǵ�Ƽ ���̵� ���� ����, �� ������ �Ѱ� ���� �� ���̵� ���� �߰�.
**
*******************************************************************************************************************/
//BOOL CInfinityManager::CreateInfinity(MSG_FC_INFINITY_CREATE * i_pInfinityCreateInfo, CFieldIOCPSocket * i_pFISoc, InfinityCreateUID_t * o_pCreateUID) 
BOOL CInfinityManager::CreateInfinity(MSG_FC_INFINITY_CREATE * i_pInfinityCreateInfo, CFieldIOCPSocket * i_pFISoc, InfinityCreateUID_t * o_pCreateUID , INT * o_nInfinityDifficultyLevel ) 
// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
{
	if(NULL == i_pInfinityCreateInfo
		|| NULL == i_pFISoc
		|| NULL == o_pCreateUID 

		// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
		|| NULL == o_nInfinityDifficultyLevel
		// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
		) 
	{
		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
		return FALSE;
	}

	BOOL CheckInvalidInputValue = FALSE;
	INFINITY_MODEINFO InfinityModeInfo;
	util::zero(&InfinityModeInfo, sizeof(INFINITY_MODEINFO));

	vectorInfinityModeInfo::iterator itr = m_vectInfiModeInfo.begin();
	for(; itr != m_vectInfiModeInfo.end(); itr++) 
	{
		// ��ȿ�� �Է� ������ üũ�ϰ� �����Ϸ��� �ϴ� ���� ��� ������ �����´�.
		if(itr->InfinityModeUID == i_pInfinityCreateInfo->InfinityModeUID
			&& itr->MapIdx == i_pInfinityCreateInfo->MapIndex
			&& itr->ModeTypeNum == i_pInfinityCreateInfo->InfinityMode)
		{
			InfinityModeInfo = *itr;
			CheckInvalidInputValue = TRUE;
			break;
		}
	}

	if(FALSE == CheckInvalidInputValue) 
	{
		return FALSE;
	}

	// Cinema, Revision ���� ���⼭ �����
	vectorCinemaInfo		CinemaInfoList;
	util::zero(&CinemaInfoList, sizeof(vectorCinemaInfo));
	
	EnterCriticalSection(&m_criticalSectionCreate);		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ


	switch (i_pInfinityCreateInfo->InfinityMode) 
	{
		case INFINITY_MODE_BOSSRUSH:
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] [Infinity][%I64d] CInfinityManager::CreateInfinity# BossRush Created !, MapIdx(%d) \r\n", m_nInfinityCreateUID);

				CInfinityBossrush * InfiBossRush = new CInfinityBossrush();
				InfiBossRush->SetInfinityCreateUID(m_nInfinityCreateUID);
				InfiBossRush->SetModeInfo(&InfinityModeInfo);
				InfiBossRush->SetCinemaInfo(this->m_Cinema.GetCinemaInfo(InfinityModeInfo.CinemaNum, &CinemaInfoList));
				InfiBossRush->SetInfinityTeamName(i_pInfinityCreateInfo->InfinityTeamName);
				InfiBossRush->InitMasterPlayer(i_pFISoc);

				// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ���� ���̵� ���� ����.
				InfiBossRush->SetDifficultyLevel ( i_pInfinityCreateInfo->InfinityDifficultyLevel );
				*o_nInfinityDifficultyLevel = InfiBossRush->GetDifficultyLevel();
				// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ���� ���̵� ���� ����.

				mt_auto_lock mta(&m_mtvectInfiBossRush);
				m_mtvectInfiBossRush.push_back(InfiBossRush);
				mta.auto_unlock_cancel();		
			} // case INFINITY_MODE_BOSSRUSH
			break; 

		case INFINITY_MODE_DEFENCE:
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] [Infinity][%I64d] CInfinityManager::CreateInfinity# Defence Created !, MapIdx(%d) \r\n", m_nInfinityCreateUID, InfinityModeInfo.MapIdx);

				CInfinityDefence * InfiDefence = new CInfinityDefence();
				InfiDefence->SetInfinityCreateUID(m_nInfinityCreateUID);
				InfiDefence->SetModeInfo(&InfinityModeInfo);
				InfiDefence->SetCinemaInfo(this->m_Cinema.GetCinemaInfo(InfinityModeInfo.CinemaNum, &CinemaInfoList));
				InfiDefence->SetInfinityTeamName(i_pInfinityCreateInfo->InfinityTeamName);
				InfiDefence->InitMasterPlayer(i_pFISoc);

				// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ���� ���̵� ���� ����.
				InfiDefence->SetDifficultyLevel ( i_pInfinityCreateInfo->InfinityDifficultyLevel );
				*o_nInfinityDifficultyLevel = InfiDefence->GetDifficultyLevel();
				// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ���� ���̵� ���� ����.

				mt_auto_lock mta(&m_mtvectInfiDefence);
				m_mtvectInfiDefence.push_back(InfiDefence);
				mta.auto_unlock_cancel();
			} // case INFINITY_MODE_DEFENCE
			break;

        // 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
		case INFINITY_MODE_MSHIPBATTLE:					
			{
#ifdef S_INFINITY3_HSKIM		// ON/OFF ��� ����
#else
				LeaveCriticalSection(&m_criticalSectionCreate);
				return FALSE;
#endif

				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] [Infinity][%I64d] CInfinityManager::CreateInfinity# MShipBattle Created !, MapIdx(%d) \r\n", m_nInfinityCreateUID, InfinityModeInfo.MapIdx);
				
				CInfinityMShipBattle * InfiMShipBattle = new CInfinityMShipBattle();
				InfiMShipBattle->SetInfinityCreateUID(m_nInfinityCreateUID);
				InfiMShipBattle->SetModeInfo(&InfinityModeInfo);
				InfiMShipBattle->SetCinemaInfo(this->m_Cinema.GetCinemaInfo(InfinityModeInfo.CinemaNum, &CinemaInfoList));
				InfiMShipBattle->SetInfinityTeamName(i_pInfinityCreateInfo->InfinityTeamName);
				InfiMShipBattle->InitMasterPlayer(i_pFISoc);
				
				InfiMShipBattle->SetDifficultyLevel ( i_pInfinityCreateInfo->InfinityDifficultyLevel );
				*o_nInfinityDifficultyLevel = InfiMShipBattle->GetDifficultyLevel();
				
				mt_auto_lock mta(&m_mtvectInfiMShipBattle);
				m_mtvectInfiMShipBattle.push_back(InfiMShipBattle);
				mta.auto_unlock_cancel();
			} // case INFINITY_MODE_DEFENCE
			break;
        // end 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�

		default : 
			{
				LeaveCriticalSection(&m_criticalSectionCreate);		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
				return FALSE;
			} // default
	}

	*o_pCreateUID = m_nInfinityCreateUID;
	m_nInfinityCreateUID++;

	LeaveCriticalSection(&m_criticalSectionCreate);		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ

	return TRUE;
}

Err_t CInfinityManager::JoinCheckInfinity(MSG_FC_INFINITY_JOIN * i_pInfinityJoinRequestInfo,  CFieldIOCPSocket * i_pFISoc) {
	if(NULL == i_pInfinityJoinRequestInfo
		|| NULL == i_pFISoc) {
		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
		return ERR_INFINITY_NULL_VALUE;
	}

	// ���� ���� �� üũ ���� 
	switch (i_pInfinityJoinRequestInfo->InfinityMode) {
	case INFINITY_MODE_BOSSRUSH: {
		mt_auto_lock mta(&m_mtvectInfiBossRush);
		mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
		for(; itr != m_mtvectInfiBossRush.end(); itr++) { 
			if(i_pInfinityJoinRequestInfo->InfinityCreateUID == (*itr)->GetInfinityCreateUID()) {
				Err_t errCode = (*itr)->JoinCheck(i_pFISoc);
				return errCode;
			}
		}
		mta.auto_unlock_cancel();
								 } // case INFINITY_MODE_BOSSRUSH
		break; 
	case INFINITY_MODE_DEFENCE: {
		mt_auto_lock mta(&m_mtvectInfiDefence);
		mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();
		for(; itr != m_mtvectInfiDefence.end(); itr++) { 
			if(i_pInfinityJoinRequestInfo->InfinityCreateUID == (*itr)->GetInfinityCreateUID()) {
				Err_t errCode = (*itr)->JoinCheck(i_pFISoc);
				return errCode;
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_DEFENCE
		break;

	case INFINITY_MODE_MSHIPBATTLE: {				// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
		mt_auto_lock mta(&m_mtvectInfiMShipBattle);
		mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
		for(; itr != m_mtvectInfiMShipBattle.end(); itr++) { 
			if(i_pInfinityJoinRequestInfo->InfinityCreateUID == (*itr)->GetInfinityCreateUID()) {
				Err_t errCode = (*itr)->JoinCheck(i_pFISoc);
				return errCode;
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_MSHIPBATTLE
		break;

	default : {
		return ERR_INFINITY_MODE;
			  } // default
	}	
	
	return ERR_INFINITY_CREATEUID;
}

Err_t CInfinityManager::JoinInfinity(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID,  CFieldIOCPSocket * i_pFISoc) 
{
	if(NULL == i_pFISoc) 
	{
		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
		return ERR_INFINITY_NULL_VALUE;
	}

	// ���� ����
	switch (i_eInfiMode) 
	{
		case INFINITY_MODE_BOSSRUSH: 
			{
				mt_auto_lock mta(&m_mtvectInfiBossRush);
				mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
				for(; itr != m_mtvectInfiBossRush.end(); itr++) 
				{ 
					if(i_CreateUID == (*itr)->GetInfinityCreateUID()) 
					{
						Err_t errCode = (*itr)->Join(i_pFISoc);
						return errCode;
					}
				}
				mta.auto_unlock_cancel();
			} // case INFINITY_MODE_BOSSRUSH
			break; 

		case INFINITY_MODE_DEFENCE: 
			{
				mt_auto_lock mta(&m_mtvectInfiDefence);
				mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();
				for(; itr != m_mtvectInfiDefence.end(); itr++) 
				{ 
					if(i_CreateUID == (*itr)->GetInfinityCreateUID()) 
					{
						Err_t errCode = (*itr)->Join(i_pFISoc);
						return errCode;
					}
				}
				mta.auto_unlock_cancel();
			} // case INFINITY_MODE_DEFENCE
			break;

		case INFINITY_MODE_MSHIPBATTLE:			// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
			{
				mt_auto_lock mta(&m_mtvectInfiMShipBattle);
				mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
				for(; itr != m_mtvectInfiMShipBattle.end(); itr++) 
				{ 
					if(i_CreateUID == (*itr)->GetInfinityCreateUID()) 
					{
						Err_t errCode = (*itr)->Join(i_pFISoc);
						return errCode;
					}
				}
				mta.auto_unlock_cancel();
			} // case INFINITY_MODE_MSHIPBATTLE
			break;

		default : 
			{
				return ERR_INFINITY_MODE;
			} // default
	}	
	
	return ERR_INFINITY_CREATEUID;
}

void CInfinityManager::GetPlayerListW(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, vectCFieldIOCPSocket * o_pInfinityMemberList, ClientIndex_t * o_pMasterUserClientIdx) {
	if(NULL == o_pInfinityMemberList
		|| NULL == o_pMasterUserClientIdx) {
		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
		return;
	}

	switch (i_eInfiMode) {
	case INFINITY_MODE_BOSSRUSH: {
		mt_auto_lock mta(&m_mtvectInfiBossRush);
		mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
		for(; itr != m_mtvectInfiBossRush.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				(*itr)->GetPlayerList(o_pInfinityMemberList, o_pMasterUserClientIdx);
				return;
			}
		}
		mta.auto_unlock_cancel();
								 } // case INFINITY_MODE_BOSSRUSH
		break; 
	case INFINITY_MODE_DEFENCE: {
		mt_auto_lock mta(&m_mtvectInfiDefence);
		mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();
		for(; itr != m_mtvectInfiDefence.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				(*itr)->GetPlayerList(o_pInfinityMemberList, o_pMasterUserClientIdx);
				return;
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_DEFENCE
		break;
	case INFINITY_MODE_MSHIPBATTLE: {		// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
		mt_auto_lock mta(&m_mtvectInfiMShipBattle);
		mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
		for(; itr != m_mtvectInfiMShipBattle.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				(*itr)->GetPlayerList(o_pInfinityMemberList, o_pMasterUserClientIdx);
				return;
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_MSHIPBATTLE
		break;
	default : {
		return;
			  } // default
	}	
	
	return;
}

Err_t CInfinityManager::ChangeMasterUserW(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, CFieldIOCPSocket * i_pChangeMasterUserFISoc) {
	if(NULL == i_pChangeMasterUserFISoc) {
		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
		return ERR_INFINITY_NULL_VALUE;
	}

	switch (i_eInfiMode) {
	case INFINITY_MODE_BOSSRUSH: {
		mt_auto_lock mta(&m_mtvectInfiBossRush);
		mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
		for(; itr != m_mtvectInfiBossRush.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				Err_t errCode = (*itr)->ChangeMasterUser(i_pChangeMasterUserFISoc);
				return errCode;
			}
		}
		mta.auto_unlock_cancel();
								 } // case INFINITY_MODE_BOSSRUSH
		break; 
	case INFINITY_MODE_DEFENCE: {
		mt_auto_lock mta(&m_mtvectInfiDefence);
		mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();
		for(; itr != m_mtvectInfiDefence.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				Err_t errCode = (*itr)->ChangeMasterUser(i_pChangeMasterUserFISoc);
				return errCode;
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_DEFENCE
		break;
	case INFINITY_MODE_MSHIPBATTLE: {			// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
		mt_auto_lock mta(&m_mtvectInfiMShipBattle);
		mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
		for(; itr != m_mtvectInfiMShipBattle.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				Err_t errCode = (*itr)->ChangeMasterUser(i_pChangeMasterUserFISoc);
				return errCode;
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_MSHIPBATTLE
		break;
	default : {
		return ERR_INFINITY_MODE;
			  } // default
	}	
	
	return ERR_INFINITY_CREATEUID;
}

Err_t CInfinityManager::LeaveInfinity(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, CFieldIOCPSocket * i_pFISoc) {
	if(NULL == i_pFISoc) {
		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
		return ERR_INFINITY_NULL_VALUE;
	}

	// ���� Ż��
	switch (i_eInfiMode) {
	case INFINITY_MODE_BOSSRUSH: {
		mt_auto_lock mta(&m_mtvectInfiBossRush);
		mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
		for(; itr != m_mtvectInfiBossRush.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				Err_t errCode = (*itr)->Leave(i_pFISoc);
				if(0 >= (*itr)->GetPlayerListSize())
				{
					// 2010-04-02 by cmkwon, ����2�� �߰� ���� - 
					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] [Infinity][%I64d] CInfinityManager::LeaveInfinity# BossRush Call CInfinityMapManager::ResetInfinityMap# !, MapInfo<%d(%d)> \r\n"
						, (*itr)->GetInfinityCreateUID(), (*itr)->GetInfinityMapIndex(), (*itr)->GetInfinityChannelIndex());

					// ������ �������ٸ� �� ����
					this->m_InfiMapManager.ResetInfinityMap((*itr)->GetInfinityMapIndex(), (*itr)->GetInfinityChannelIndex());
					util::del(*itr);
					itr = m_mtvectInfiBossRush.erase(itr);
				}
				return errCode;
			}
		}
		mta.auto_unlock_cancel();
								 } // case INFINITY_MODE_BOSSRUSH
		break; 
	case INFINITY_MODE_DEFENCE: {
		mt_auto_lock mta(&m_mtvectInfiDefence);
		mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();
		for(; itr != m_mtvectInfiDefence.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				Err_t errCode = (*itr)->Leave(i_pFISoc);
				if(0 >= (*itr)->GetPlayerListSize())
				{
					// 2010-04-02 by cmkwon, ����2�� �߰� ���� - 
					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] [Infinity][%I64d] CInfinityManager::LeaveInfinity# Defence Call CInfinityMapManager::ResetInfinityMap# !, MapInfo<%d(%d)> \r\n"
						, (*itr)->GetInfinityCreateUID(), (*itr)->GetInfinityMapIndex(), (*itr)->GetInfinityChannelIndex());

					// ������ �������ٸ� �� ����
					this->m_InfiMapManager.ResetInfinityMap((*itr)->GetInfinityMapIndex(), (*itr)->GetInfinityChannelIndex());
					util::del(*itr);
					itr = m_mtvectInfiDefence.erase(itr);
				}
				return errCode;
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_DEFENCE
		break;
	case INFINITY_MODE_MSHIPBATTLE: {					// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
		mt_auto_lock mta(&m_mtvectInfiMShipBattle);
		mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
		for(; itr != m_mtvectInfiMShipBattle.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				Err_t errCode = (*itr)->Leave(i_pFISoc);
				if(0 >= (*itr)->GetPlayerListSize())
				{
					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] [Infinity][%I64d] CInfinityManager::LeaveInfinity# MShipBattle Call CInfinityMapManager::ResetInfinityMap# !, MapInfo<%d(%d)> \r\n"
						, (*itr)->GetInfinityCreateUID(), (*itr)->GetInfinityMapIndex(), (*itr)->GetInfinityChannelIndex());

					this->m_InfiMapManager.ResetInfinityMap((*itr)->GetInfinityMapIndex(), (*itr)->GetInfinityChannelIndex());
					util::del(*itr);
					itr = m_mtvectInfiMShipBattle.erase(itr);
				}
				return errCode;
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_MSHIPBATTLE
		break;
	default : {
		return ERR_INFINITY_MODE;
			  } // default
	}	
	
	return ERR_INFINITY_CREATEUID;
}

Err_t CInfinityManager::BanInfinity(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, CFieldIOCPSocket * i_pBanFISoc) {
	if(NULL == i_pBanFISoc) {
		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
		return ERR_INFINITY_NULL_VALUE;
	}

	// ���� �߹�
	switch (i_eInfiMode) {
	case INFINITY_MODE_BOSSRUSH: {
		mt_auto_lock mta(&m_mtvectInfiBossRush);
		mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
		for(; itr != m_mtvectInfiBossRush.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				Err_t errCode = (*itr)->Ban(i_pBanFISoc);
				return errCode;
			}
		}
		mta.auto_unlock_cancel();
								 } // case INFINITY_MODE_BOSSRUSH
		break; 
	case INFINITY_MODE_DEFENCE: {
		mt_auto_lock mta(&m_mtvectInfiDefence);
		mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();
		for(; itr != m_mtvectInfiDefence.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				Err_t errCode = (*itr)->Ban(i_pBanFISoc);
				return errCode;
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_DEFENCE
		break;
	case INFINITY_MODE_MSHIPBATTLE: {			// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
		mt_auto_lock mta(&m_mtvectInfiMShipBattle);
		mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
		for(; itr != m_mtvectInfiMShipBattle.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				Err_t errCode = (*itr)->Ban(i_pBanFISoc);
				return errCode;
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_MSHIPBATTLE
		break;
	default : {
		return ERR_INFINITY_MODE;
			  } // default
	}	
	
	return ERR_INFINITY_CREATEUID;
}

Err_t CInfinityManager::StartInfinity(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, CFieldIOCPSocket * i_pFISoc)
{
	if(NULL == i_pFISoc 
		|| FALSE == i_pFISoc->IsValidCharacter(FALSE))
	{
		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
		return ERR_INFINITY_NULL_VALUE;
	}

	// ���� ����
	switch (i_eInfiMode)
	{
	case INFINITY_MODE_BOSSRUSH:
		{
			mt_auto_lock mta(&m_mtvectInfiBossRush);
			///////////////////////////////////////////////////////////////////////////////
			// 2010-04-05 by cmkwon, ���� ������ ī�� ���� �ý��� ���� - 
			CInfinityBossrush *pInfinity = this->FindBossrushNoLock(i_CreateUID);
			if(pInfinity)
			{				
				Err_t errCode = pInfinity->Start(i_pFISoc, this->m_InfiMapManager.CreateInfinityMap(pInfinity->GetInfinityMapIndex()));
				return errCode;
			}
		} // case INFINITY_MODE_BOSSRUSH
		break; 
	case INFINITY_MODE_DEFENCE:
		{
			mt_auto_lock mta(&m_mtvectInfiDefence);
			///////////////////////////////////////////////////////////////////////////////
			// 2010-04-05 by cmkwon, ���� ������ ī�� ���� �ý��� ���� - 
			CInfinityDefence *pInfinity = this->FindDefenceNoLock(i_CreateUID);
			if(pInfinity)
			{
				Err_t errCode = pInfinity->Start(i_pFISoc, this->m_InfiMapManager.CreateInfinityMap(pInfinity->GetInfinityMapIndex()));
				return errCode;
			}
		} // case INFINITY_MODE_DEFENCE
		break;
	case INFINITY_MODE_MSHIPBATTLE:		// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
		{
			mt_auto_lock mta(&m_mtvectInfiMShipBattle);

			CInfinityMShipBattle *pInfinity = this->FindMShipBattleNoLock(i_CreateUID);
			if(pInfinity)
			{
   				Err_t errCode = pInfinity->Start(i_pFISoc, this->m_InfiMapManager.CreateInfinityMap(pInfinity->GetInfinityMapIndex()));
				return errCode;
			}
		} // case INFINITY_MODE_MSHIPBATTLE
		break;
	default :
		{
			return ERR_INFINITY_MODE;
		} // default
	}
	
	return ERR_INFINITY_CREATEUID;
}

BOOL CInfinityManager::GetRevisionInfoW(REVISIONINFO * o_pRevisionInfo, InfiModeUID_t i_InfinityModeUID, INT i_nUnitKind) {
	if(NULL == o_pRevisionInfo) {
		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
		return FALSE;
	}

	// ����ġ ��������
	vectorInfinityModeInfo::iterator itr = m_vectInfiModeInfo.begin();
	for(; itr != m_vectInfiModeInfo.end(); itr++) {
		if(i_InfinityModeUID == itr->InfinityModeUID) {
			if(this->m_Revision.GetRevisionInfo(o_pRevisionInfo, itr->RevisionNum, i_nUnitKind)) {
				return TRUE;			
			}
			return FALSE;
		}
	}

	return FALSE;
}

Err_t CInfinityManager::UserMapLoadedComplete(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, MapIndex_t i_MapIndex)
{
	// ���� �� ����
	switch (i_eInfiMode)
	{
	case INFINITY_MODE_BOSSRUSH:
		{
			mt_auto_lock mta(&m_mtvectInfiBossRush);
			///////////////////////////////////////////////////////////////////////////////
			// 2010-04-05 by cmkwon, ���� ������ ī�� ���� �ý��� ���� - 
			CInfinityBossrush *pInfinity = this->FindBossrushNoLock(i_CreateUID);
			if(pInfinity)
			{
				return pInfinity->CreateMap(NULL);
			}

		} // case INFINITY_MODE_BOSSRUSH
		break; 
	case INFINITY_MODE_DEFENCE:
		{
			mt_auto_lock mta(&m_mtvectInfiDefence);
			///////////////////////////////////////////////////////////////////////////////
			// 2010-04-05 by cmkwon, ���� ������ ī�� ���� �ý��� ���� - 
			CInfinityDefence *pInfinity = this->FindDefenceNoLock(i_CreateUID);
			if(pInfinity)
			{
				return pInfinity->CreateMap(NULL);
			}
		} // case INFINITY_MODE_DEFENCE
		break;
	case INFINITY_MODE_MSHIPBATTLE:			// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
		{
			mt_auto_lock mta(&m_mtvectInfiMShipBattle);

			CInfinityMShipBattle *pInfinity = this->FindMShipBattleNoLock(i_CreateUID);
			if(pInfinity)
			{
				return pInfinity->CreateMap(NULL);
			}
		} // case INFINITY_MODE_MSHIPBATTLE
		break;
	default :
		{
			return ERR_INFINITY_MODE;
		} // default
	}
	
	return ERR_INFINITY_CREATEUID;
}


void CInfinityManager::StartTick() {
	m_pTickManager->InitThread();
}

void CInfinityManager::DoSecondlyWorkInfinity(ATUM_DATE_TIME *pDateTime) {
	// Tick
	
	{
		mt_auto_lock mta(&m_mtvectInfiBossRush);
		mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
		while (itr != m_mtvectInfiBossRush.end())
		{
			if((*itr)->CheckDestory())
			{
				// 2010-04-02 by cmkwon, ����2�� �߰� ���� - 
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] [Infinity][%I64d] CInfinityManager::DoSecondlyWorkInfinity# BossRush Call CInfinityMapManager::ResetInfinityMap# !, MapInfo<%d(%d)> \r\n"
					, (*itr)->GetInfinityCreateUID(), (*itr)->GetInfinityMapIndex(), (*itr)->GetInfinityChannelIndex());

				this->m_InfiMapManager.ResetInfinityMap((*itr)->GetInfinityMapIndex(), (*itr)->GetInfinityChannelIndex());
				util::del(*itr);
				itr = m_mtvectInfiBossRush.erase(itr);
				continue;
			}
			(*itr)->DoSecondlyWorkInfinity(pDateTime);
			itr++;
		}
		mta.auto_unlock_cancel();
	}

	mt_auto_lock mtD(&m_mtvectInfiDefence);
	mtvectInfiDefence::iterator itrD = m_mtvectInfiDefence.begin();
	while (itrD != m_mtvectInfiDefence.end()) {
		if((*itrD)->CheckDestory())
		{
			// 2010-04-02 by cmkwon, ����2�� �߰� ���� - 
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] [Infinity][%I64d] CInfinityManager::DoSecondlyWorkInfinity# Defence Call CInfinityMapManager::ResetInfinityMap# !, MapInfo<%d(%d)> \r\n"
				, (*itrD)->GetInfinityCreateUID(), (*itrD)->GetInfinityMapIndex(), (*itrD)->GetInfinityChannelIndex());

			this->m_InfiMapManager.ResetInfinityMap((*itrD)->GetInfinityMapIndex(), (*itrD)->GetInfinityChannelIndex());			
			util::del(*itrD);
			itrD = m_mtvectInfiDefence.erase(itrD);
			continue;
		}
		(*itrD)->DoSecondlyWorkInfinity(pDateTime);
		itrD++;
	}
	mtD.auto_unlock_cancel();

	{		// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
		mt_auto_lock mta(&m_mtvectInfiMShipBattle);
		mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
		while (itr != m_mtvectInfiMShipBattle.end())
		{
			if((*itr)->CheckDestory())
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] [Infinity][%I64d] CInfinityManager::DoSecondlyWorkInfinity# MShipBattle Call CInfinityMapManager::ResetInfinityMap# !, MapInfo<%d(%d)> \r\n"
					, (*itr)->GetInfinityCreateUID(), (*itr)->GetInfinityMapIndex(), (*itr)->GetInfinityChannelIndex());
				
				this->m_InfiMapManager.ResetInfinityMap((*itr)->GetInfinityMapIndex(), (*itr)->GetInfinityChannelIndex());
				util::del(*itr);
				itr = m_mtvectInfiMShipBattle.erase(itr);
				continue;
			}
			(*itr)->DoSecondlyWorkInfinity(pDateTime);
			itr++;
		}
		mta.auto_unlock_cancel();
	}
}


void CInfinityManager::CreateKeyMonster_DeadForNextStepW(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx, MonIdx_t i_CreateMonsterIdx) {
	mt_auto_lock mta(&m_mtvectInfiBossRush);
	mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
	for(; itr != m_mtvectInfiBossRush.end(); itr++) {
		if(i_MapIndex == (*itr)->GetInfinityMapIndex() 
			&& i_ChannelIdx == (*itr)->GetInfinityChannelIndex()) {
			(*itr)->CreateKeyMonster_DeadForNextStep(i_CreateMonsterIdx);
			return;
		}
	}
	mta.auto_unlock_cancel();
	
	mt_auto_lock mtD(&m_mtvectInfiDefence);
	mtvectInfiDefence::iterator itrD = m_mtvectInfiDefence.begin();
	for(; itrD != m_mtvectInfiDefence.end(); itrD++) {
		if(i_MapIndex == (*itrD)->GetInfinityMapIndex() 
			&& i_ChannelIdx == (*itrD)->GetInfinityChannelIndex()) {
			(*itrD)->CreateKeyMonster_DeadForNextStep(i_CreateMonsterIdx);
			return;
		}
	}
	mtD.auto_unlock_cancel();
	
	mt_auto_lock mtT(&m_mtvectInfiMShipBattle);			// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
	mtvectInfiMShipBattle::iterator itrT = m_mtvectInfiMShipBattle.begin();
	for(; itrT != m_mtvectInfiMShipBattle.end(); itrT++) {
		if(i_MapIndex == (*itrT)->GetInfinityMapIndex() 
			&& i_ChannelIdx == (*itrT)->GetInfinityChannelIndex()) {
			(*itrT)->CreateKeyMonster_DeadForNextStep(i_CreateMonsterIdx);
			return;
		}
	}
	mtT.auto_unlock_cancel();
}

// 2010-04-09 by cmkwon, ����2�� �߰� ����(�ܰ躰 ���� �߰�) - ���ϰ��� Ű���� ���� ó�� ���� �÷���
void CInfinityManager::DeleteKeyMonster_DeadForNextStepW(BOOL *o_pbCompensationFlag, MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx, MonIdx_t i_DeadMonsterIdx)
{
// 2010-04-09 by cmkwon, ����2�� �߰� ����(�ܰ躰 ���� �߰�) - 
// 	mt_auto_lock mta(&m_mtvectInfiBossRush);
// 	mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
// 	for(; itr != m_mtvectInfiBossRush.end(); itr++) {
// 		if(i_MapIndex == (*itr)->GetInfinityMapIndex() 
// 			&& i_ChannelIdx == (*itr)->GetInfinityChannelIndex()) {
// 			(*itr)->DeleteKeyMonster_DeadForNextStep(i_DeadMonsterIdx);
// 			return;
// 		}
// 	}
// 	mta.auto_unlock_cancel();
// 	
// 	mt_auto_lock mtD(&m_mtvectInfiDefence);
// 	mtvectInfiDefence::iterator itrD = m_mtvectInfiDefence.begin();
// 	for(; itrD != m_mtvectInfiDefence.end(); itrD++) {
// 		if(i_MapIndex == (*itrD)->GetInfinityMapIndex() 
// 			&& i_ChannelIdx == (*itrD)->GetInfinityChannelIndex()) {
// 			(*itrD)->DeleteKeyMonster_DeadForNextStep(i_DeadMonsterIdx);
// 			return;
// 		}
// 	}
// 	mtD.auto_unlock_cancel();
	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-09 by cmkwon, ����2�� �߰� ����(�ܰ躰 ���� �߰�) - 
	{
		mt_auto_lock mtA(&m_mtvectInfiBossRush);
		CInfinityBossrush *pInfinity = this->FindBossrushNoLock(i_MapIndex, i_ChannelIdx);
		if(pInfinity)
		{
			pInfinity->DeleteKeyMonster_DeadForNextStep(o_pbCompensationFlag, i_DeadMonsterIdx);
			return;
		}
	}
	{
		mt_auto_lock mtA(&m_mtvectInfiDefence);
		CInfinityDefence *pInfinity = this->FindDefenceNoLock(i_MapIndex, i_ChannelIdx);
		if(pInfinity)
		{
			pInfinity->DeleteKeyMonster_DeadForNextStep(o_pbCompensationFlag, i_DeadMonsterIdx);
			return;
		}
	}
	{			// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
		mt_auto_lock mtA(&m_mtvectInfiMShipBattle);
		CInfinityMShipBattle *pInfinity = this->FindMShipBattleNoLock(i_MapIndex, i_ChannelIdx);
		if(pInfinity)
		{
			pInfinity->DeleteKeyMonster_DeadForNextStep(o_pbCompensationFlag, i_DeadMonsterIdx);
			return;
		}
	}
}

Err_t CInfinityManager::ChoiceTenderItemW(DiceCnt_t *o_pDiceResult, MSG_FC_INFINITY_TENDER_PUT_IN_TENDER * i_pPutInTenderInfo, ClientIndex_t i_PlayerClientIdx)
{
	switch (i_pPutInTenderInfo->InfinityMode)
	{
	case INFINITY_MODE_BOSSRUSH:
		{
			mt_auto_lock mta(&m_mtvectInfiBossRush);
			CInfinityBossrush *pInfinity = this->FindBossrushNoLock(i_pPutInTenderInfo->InfinityCreateUID);
			if(pInfinity)
			{
				return pInfinity->ChoiceTenderItem(o_pDiceResult, i_PlayerClientIdx, i_pPutInTenderInfo->ItemFieldIndex, i_pPutInTenderInfo->GiveUp);
			}
		} // case INFINITY_MODE_BOSSRUSH
		break; 
	case INFINITY_MODE_DEFENCE:
		{
			mt_auto_lock mta(&m_mtvectInfiDefence);
			CInfinityDefence *pInfinity = this->FindDefenceNoLock(i_pPutInTenderInfo->InfinityCreateUID);
			if(pInfinity)
			{
				return pInfinity->ChoiceTenderItem(o_pDiceResult, i_PlayerClientIdx, i_pPutInTenderInfo->ItemFieldIndex, i_pPutInTenderInfo->GiveUp);
			}
		} // case INFINITY_MODE_DEFENCE
		break;
	case INFINITY_MODE_MSHIPBATTLE:			// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
		{
			mt_auto_lock mta(&m_mtvectInfiMShipBattle);
			CInfinityMShipBattle *pInfinity = this->FindMShipBattleNoLock(i_pPutInTenderInfo->InfinityCreateUID);
			if(pInfinity)
			{
				return pInfinity->ChoiceTenderItem(o_pDiceResult, i_PlayerClientIdx, i_pPutInTenderInfo->ItemFieldIndex, i_pPutInTenderInfo->GiveUp);
			}
		} // case INFINITY_MODE_MSHIPBATTLE
		break;
	default :
		{
			return ERR_INFINITY_MODE;
		} // default
	}	
	
	return ERR_INFINITY_CREATEUID;
}

void CInfinityManager::ProcessingInfinityPenalty(char * i_szCharacterName, eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID) {
	if(NULL == i_szCharacterName) {
		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
		return;
	}
	
	// 2009-09-09 ~ 2010-01-13 by dhjin, ���Ǵ�Ƽ - ���� ���� �̸� ���� ���� �߰�, // ���Ǵ�Ƽ - ���� ����� �г�Ƽ �߰�
	switch (i_eInfiMode) {
	case INFINITY_MODE_BOSSRUSH: {
		mt_auto_lock mta(&m_mtvectInfiBossRush);
		mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
		for(; itr != m_mtvectInfiBossRush.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				(*itr)->CalcLimitTimeByUserDeath(i_szCharacterName);
				return;
			}
		}
		mta.auto_unlock_cancel();
								 } // case INFINITY_MODE_BOSSRUSH
		break; 
 	case INFINITY_MODE_DEFENCE: {
 		mt_auto_lock mta(&m_mtvectInfiDefence);
 		mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();
 		for(; itr != m_mtvectInfiDefence.end(); itr++) { 
 			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
 				(*itr)->CalcAliveForGameClearMonsterHPByUserDeath(i_szCharacterName);	
				return;
 			}
 		}
 		mta.auto_unlock_cancel();
 								} // case INFINITY_MODE_DEFENCE
 		break;
	case INFINITY_MODE_MSHIPBATTLE: {				// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
		mt_auto_lock mta(&m_mtvectInfiMShipBattle);
		mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
		for(; itr != m_mtvectInfiMShipBattle.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				(*itr)->CalcLimitTimeByUserDeath(i_szCharacterName);					// �г�Ƽ ���� (���� �ð�)
				(*itr)->CalcAliveForGameClearMonsterHPByUserDeath(i_szCharacterName);	// �г�Ƽ ���� (�Ʊ� �� HP)
				return;
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_MSHIPBATTLE
		break;
	default : {
		return;
			  } // default
	}	
	
	return;
}

void CInfinityManager::SendInfinityTeamChatW(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, BYTE * i_pDATA, int i_nSize) {
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ä��
	switch (i_eInfiMode) {
	case INFINITY_MODE_BOSSRUSH: {
		mt_auto_lock mta(&m_mtvectInfiBossRush);
		mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
		for(; itr != m_mtvectInfiBossRush.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				(*itr)->SendInfinityTeamChat(i_pDATA, i_nSize);
				return;
			}
		}
		mta.auto_unlock_cancel();
								 } // case INFINITY_MODE_BOSSRUSH
		break; 
	case INFINITY_MODE_DEFENCE: {
		mt_auto_lock mta(&m_mtvectInfiDefence);
		mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();
		for(; itr != m_mtvectInfiDefence.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				(*itr)->SendInfinityTeamChat(i_pDATA, i_nSize);
				return;
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_DEFENCE
		break;
	case INFINITY_MODE_MSHIPBATTLE: {			// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
		mt_auto_lock mta(&m_mtvectInfiMShipBattle);
		mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
		for(; itr != m_mtvectInfiMShipBattle.end(); itr++) { 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID()) {
				(*itr)->SendInfinityTeamChat(i_pDATA, i_nSize);
				return;
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_MSHIPBATTLE
		break;
	default : {
		return;
			  } // default
	}	
	
	return;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-05 by cmkwon, ���� ������ ī�� ���� �ý��� ���� - 
/// \author		cmkwon
/// \date		2010-04-05 ~ 2010-04-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
Err_t CInfinityManager::ImputeInfinityW(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID)
{
	// ���� �� ����
	switch (i_eInfiMode)
	{
	case INFINITY_MODE_BOSSRUSH:
		{
			mt_auto_lock mta(&m_mtvectInfiBossRush);
			///////////////////////////////////////////////////////////////////////////////
			// 2010-04-05 by cmkwon, ���� ������ ī�� ���� �ý��� ���� - 
			CInfinityBossrush *pInfinity = this->FindBossrushNoLock(i_CreateUID);
			if(pInfinity)
			{
				return pInfinity->ImputeInfinity();
			}
			
		} // case INFINITY_MODE_BOSSRUSH
		break; 
	case INFINITY_MODE_DEFENCE:
		{
			mt_auto_lock mta(&m_mtvectInfiDefence);
			///////////////////////////////////////////////////////////////////////////////
			// 2010-04-05 by cmkwon, ���� ������ ī�� ���� �ý��� ���� - 
			CInfinityDefence *pInfinity = this->FindDefenceNoLock(i_CreateUID);
			if(pInfinity)
			{
				return pInfinity->ImputeInfinity();
			}
		} // case INFINITY_MODE_DEFENCE
		break;
	case INFINITY_MODE_MSHIPBATTLE:			// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
		{
			mt_auto_lock mta(&m_mtvectInfiMShipBattle);

			CInfinityMShipBattle *pInfinity = this->FindMShipBattleNoLock(i_CreateUID);
			if(pInfinity)
			{
				return pInfinity->ImputeInfinity();
			}
		} // case INFINITY_MODE_MSHIPBATTLE
		break;
	default :
		{
			return ERR_INFINITY_MODE;
		} // default
	}
	
	return ERR_INFINITY_CREATEUID;
}


// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
/******************************************************************************************************************
**
**	���Ǵ�Ƽ - �濡 ���̵� ���� ����.
**
**	Create Info :	2010. 05. 26. by hsLee.
**
**	Update Info :	
**
*******************************************************************************************************************/
Err_t CInfinityManager :: ChangeInfinityDifficultyLevel ( const INT i_cst_ChangeDifficultyLevel , eINFINITY_MODE i_eInfiMode , InfinityCreateUID_t i_CreateUID , CFieldIOCPSocket * i_pMasterUserFISoc )
{

	if( NULL == i_pMasterUserFISoc )
		return ERR_INFINITY_NULL_VALUE;
	
	switch ( i_eInfiMode )
	{

		case INFINITY_MODE_BOSSRUSH: 
			{
				mt_auto_lock mta ( &m_mtvectInfiBossRush );
				mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
				for( ; itr != m_mtvectInfiBossRush.end(); itr++ ) 
				{ 
					if ( i_CreateUID == (*itr)->GetInfinityCreateUID() )
					{
						return (*itr)->ChangeDifficultyLevel( i_pMasterUserFISoc , i_cst_ChangeDifficultyLevel );
					}
				}
				mta.auto_unlock_cancel();
			} 
			break; 

		case INFINITY_MODE_DEFENCE: 
			{
				mt_auto_lock mta ( &m_mtvectInfiDefence );
				mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();
				for( ; itr != m_mtvectInfiDefence.end(); itr++ )
				{ 
					if ( i_CreateUID == (*itr)->GetInfinityCreateUID() ) 
					{
						return (*itr)->ChangeDifficultyLevel ( i_pMasterUserFISoc , i_cst_ChangeDifficultyLevel );
					}
				}
				mta.auto_unlock_cancel();
			}
			break;

		case INFINITY_MODE_MSHIPBATTLE:			// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
			{
				mt_auto_lock mta ( &m_mtvectInfiMShipBattle );
				mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
				for( ; itr != m_mtvectInfiMShipBattle.end(); itr++ )
				{ 
					if ( i_CreateUID == (*itr)->GetInfinityCreateUID() ) 
					{
						return (*itr)->ChangeDifficultyLevel ( i_pMasterUserFISoc , i_cst_ChangeDifficultyLevel );
					}
				}
				mta.auto_unlock_cancel();
			}
			break;
	}	
	
	return ERR_INFINITY_DIFFICULTY_LEVEL_INVALID;

}

// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )


// 2010. 07. 27 by hsLee ���Ǵ�Ƽ 2�� ���� ��� �ó׸� ���� ��ŵ ó��.
/******************************************************************************************************************************
**
**	���Ǵ�Ƽ ���� �ó׸� ���� ��ŵ Ȱ��ȭ. 
**
**	Create Info :	2010. 07. 27.	by hsLee.
**
**	Update Info :	�ó׸� ���� ��ŵ ó���� ���� ���� ���ῡ ���� ó�� �߰�. 'a_bNormalEnding'	2010. 08. 26. by hsLee.
**
*******************************************************************************************************************************/
Err_t CInfinityManager :: InfinitySkipEndingCinema ( eINFINITY_MODE i_eInfiMode , InfinityCreateUID_t i_CreateUID , CFieldIOCPSocket * i_pPlayerFISoc , const bool a_bNormalEnding /*= false*/ )
{

	if( NULL == i_pPlayerFISoc )
		return ERR_INFINITY_NULL_VALUE;
	
	switch ( i_eInfiMode )
	{
		
		case INFINITY_MODE_BOSSRUSH: 
			{
				mt_auto_lock mta ( &m_mtvectInfiBossRush );
				mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
				for( ; itr != m_mtvectInfiBossRush.end(); itr++ ) 
				{ 
					if ( i_CreateUID == (*itr)->GetInfinityCreateUID() )
					{
						return (*itr)->SkipEndingCinema ( i_pPlayerFISoc , a_bNormalEnding );
					}
				}
				mta.auto_unlock_cancel();
			} 
			break; 
			
		case INFINITY_MODE_DEFENCE: 
			{
				mt_auto_lock mta ( &m_mtvectInfiDefence );
				mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();
				for( ; itr != m_mtvectInfiDefence.end(); itr++ )
				{ 
					if ( i_CreateUID == (*itr)->GetInfinityCreateUID() ) 
					{
						return (*itr)->SkipEndingCinema ( i_pPlayerFISoc , a_bNormalEnding );
					}
				}
				mta.auto_unlock_cancel();
			}
			break;

		case INFINITY_MODE_MSHIPBATTLE:			// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
			{
				mt_auto_lock mta ( &m_mtvectInfiMShipBattle );
				mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
				for( ; itr != m_mtvectInfiMShipBattle.end(); itr++ )
				{ 
					if ( i_CreateUID == (*itr)->GetInfinityCreateUID() ) 
					{
						return (*itr)->SkipEndingCinema ( i_pPlayerFISoc , a_bNormalEnding );
					}
				}
				mta.auto_unlock_cancel();
			}
			break;
	}
	
	return ERR_INFINITY_MISMATCH_CREATEUID;

}


void CInfinityManager::SetDisConnectUserInfo(INFINITY_DISCONNECTUSER_INFO * i_pDisConnectUserInfo)
{
	if ( NULL == i_pDisConnectUserInfo ) 
	{
		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
		return;
	}

	// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
	mt_auto_lock mta(&m_mtInfinityDisConnectUserList);
	mtvectorInfinityDisConnectUser::iterator itr = m_mtInfinityDisConnectUserList.begin();

	for(; itr != m_mtInfinityDisConnectUserList.end(); itr++)
	{
		if ( 0 == strnicmp(i_pDisConnectUserInfo->CharacterName, itr->CharacterName, SIZE_MAX_CHARACTER_NAME) )
		{
			// ���� ������ �ñ� ������ �����Ѵٸ�!! ����������~~!~!!!
			itr->InfinityCreateUID	= i_pDisConnectUserInfo->InfinityCreateUID;
			itr->InfinityMode		= i_pDisConnectUserInfo->InfinityMode;
			return;
		}
	}

	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��, ���� ƨ�� ������ ��� ���� ����.
	m_mtInfinityDisConnectUserList.push_back(*i_pDisConnectUserInfo);
}

Err_t CInfinityManager::DisConnectUserReStart(char * i_DisConnectUserName, INFINITY_PLAYING_INFO * o_pInfinityPlayingInfo)
{
	if(	NULL == i_DisConnectUserName
		|| NULL == o_pInfinityPlayingInfo ) 
	{
		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
		return ERR_INFINITY_NULL_VALUE;
	}

	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��, ���� ƨ�� ���� ���� ó��
	mt_auto_lock mta(&m_mtInfinityDisConnectUserList);
	mtvectorInfinityDisConnectUser::iterator itr = m_mtInfinityDisConnectUserList.begin();

	for(; itr != m_mtInfinityDisConnectUserList.end(); itr++)
	{
		if ( 0 == strnicmp(i_DisConnectUserName, itr->CharacterName, SIZE_MAX_CHARACTER_NAME) )
		{
			o_pInfinityPlayingInfo->InfinityCreateUID	= itr->InfinityCreateUID;
			o_pInfinityPlayingInfo->ModeTypeNum			= itr->InfinityMode;
			return this->CheckIsCreateInfinityUID(o_pInfinityPlayingInfo);
		}
	}

	return ERR_INFINITY_CANNOT_SUCH_TEAM;
}


Err_t CInfinityManager::CheckIsCreateInfinityUID(INFINITY_PLAYING_INFO * o_pInfinityPlayingInfo) 
{
	if ( NULL == o_pInfinityPlayingInfo ) 
	{
		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
		return ERR_INFINITY_NULL_VALUE;
	}

	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��
	switch ( o_pInfinityPlayingInfo->ModeTypeNum ) 
	{
		case INFINITY_MODE_BOSSRUSH: 
			{
				mt_auto_lock mta ( &m_mtvectInfiBossRush );
				mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();

				for(; itr != m_mtvectInfiBossRush.end(); itr++) 
				{ 
					if ( o_pInfinityPlayingInfo->InfinityCreateUID == (*itr)->GetInfinityCreateUID() ) 
					{
						o_pInfinityPlayingInfo->MapIdx = (*itr)->GetInfinityMapIndex();
						return ERR_NO_ERROR;
					}
				}

				mta.auto_unlock_cancel();

			} // case INFINITY_MODE_BOSSRUSH
			break; 

		case INFINITY_MODE_DEFENCE: 
			{
				mt_auto_lock mta ( &m_mtvectInfiDefence );
				mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();

				for(; itr != m_mtvectInfiDefence.end(); itr++) 
				{ 
					if( o_pInfinityPlayingInfo->InfinityCreateUID == (*itr)->GetInfinityCreateUID() ) 
					{
						o_pInfinityPlayingInfo->MapIdx = (*itr)->GetInfinityMapIndex();
						return ERR_NO_ERROR;
					}
				}

				mta.auto_unlock_cancel();

			} // case INFINITY_MODE_DEFENCE
			break;

		case INFINITY_MODE_MSHIPBATTLE:			// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
			{
				mt_auto_lock mta ( &m_mtvectInfiMShipBattle );
				mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
				
				for(; itr != m_mtvectInfiMShipBattle.end(); itr++) 
				{ 
					if( o_pInfinityPlayingInfo->InfinityCreateUID == (*itr)->GetInfinityCreateUID() ) 
					{
						o_pInfinityPlayingInfo->MapIdx = (*itr)->GetInfinityMapIndex();
						return ERR_NO_ERROR;
					}
				}
				
				mta.auto_unlock_cancel();
				
			} // case INFINITY_MODE_MSHIPBATTLE
			break;

		default : 
			return ERR_INFINITY_CANNOT_SUCH_TEAM;
	}

	
	return ERR_INFINITY_CANNOT_SUCH_TEAM;
}


BOOL CInfinityManager::DeleteDisConnectUserInfo(char * i_DisConnectUserName)
{
	if ( NULL == i_DisConnectUserName ) 
	{
		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
		return FALSE;
	}

	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��, ���� ƨ�� ������ ��� ���� ����.
	mt_auto_lock mta(&m_mtInfinityDisConnectUserList);

	mtvectorInfinityDisConnectUser::iterator itr = m_mtInfinityDisConnectUserList.begin();

	for(; itr != m_mtInfinityDisConnectUserList.end(); itr++)
	{
		if( 0 == strnicmp(i_DisConnectUserName, itr->CharacterName, SIZE_MAX_CHARACTER_NAME) )
		{
			m_mtInfinityDisConnectUserList.erase(itr);
			return TRUE;
		}
	}
	return FALSE;
}

void CInfinityManager::ReStartDisConnectUserW(InfinityCreateUID_t i_nInfinityCreateUID, eINFINITY_MODE i_eInfiMode, CFieldIOCPSocket * i_pUserFISoc) 
{

	if ( NULL == i_pUserFISoc )
	{
		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
		return;
	}
	
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó�� ����
	switch ( i_eInfiMode )
	{
		case INFINITY_MODE_BOSSRUSH: 
			{
				mt_auto_lock mta( &m_mtvectInfiBossRush );
				mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
				for(; itr != m_mtvectInfiBossRush.end(); itr++) 
				{ 
					if( i_nInfinityCreateUID == (*itr)->GetInfinityCreateUID() ) 
					{
						(*itr)->ReStartDisConnectUser(i_pUserFISoc);
						return;
					}
				}
				mta.auto_unlock_cancel();
			} // case INFINITY_MODE_BOSSRUSH
			break; 

		case INFINITY_MODE_DEFENCE: 
			{
				mt_auto_lock mta( &m_mtvectInfiDefence );
				mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();
				for(; itr != m_mtvectInfiDefence.end(); itr++) 
				{ 
					if( i_nInfinityCreateUID == (*itr)->GetInfinityCreateUID() ) 
					{
						(*itr)->ReStartDisConnectUser( i_pUserFISoc );
						return;
					}
				}
				mta.auto_unlock_cancel();
			} // case INFINITY_MODE_DEFENCE
			break;

		case INFINITY_MODE_MSHIPBATTLE:				// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
			{
				mt_auto_lock mta( &m_mtvectInfiMShipBattle );
				mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
				for(; itr != m_mtvectInfiMShipBattle.end(); itr++) 
				{ 
					if( i_nInfinityCreateUID == (*itr)->GetInfinityCreateUID() ) 
					{
						(*itr)->ReStartDisConnectUser( i_pUserFISoc );
						return;
					}
				}
				mta.auto_unlock_cancel();
			} // case INFINITY_MODE_DEFENCE
			break;

		default : 
			return;
	}	
	
	return;
}


void CInfinityManager::CreateKeyMonster_AliveForGameClearW(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx, MonIdx_t i_CreateMonsterIdx)
{
	mt_auto_lock mtD(&m_mtvectInfiDefence);
	mtvectInfiDefence::iterator itrD = m_mtvectInfiDefence.begin();
	for(; itrD != m_mtvectInfiDefence.end(); itrD++) {
		if(i_MapIndex == (*itrD)->GetInfinityMapIndex() 
			&& i_ChannelIdx == (*itrD)->GetInfinityChannelIndex()) {
			(*itrD)->CreateKeyMonster_AliveForGameClear(i_CreateMonsterIdx);
			return;
		}
	}
	mtD.auto_unlock_cancel();

	// start 2011-06-05 by hskim, ���Ǵ�Ƽ 3�� - Ű���� ������ ���� ó��
	mt_auto_lock mtT(&m_mtvectInfiMShipBattle);
	mtvectInfiMShipBattle::iterator itrT = m_mtvectInfiMShipBattle.begin();
	for(; itrT != m_mtvectInfiMShipBattle.end(); itrT++) {
		if(i_MapIndex == (*itrT)->GetInfinityMapIndex() 
			&& i_ChannelIdx == (*itrT)->GetInfinityChannelIndex()) {
			(*itrT)->CreateKeyMonster_AliveForGameClear(i_CreateMonsterIdx);
			return;
		}
	}
	mtT.auto_unlock_cancel();
	// end 2011-06-05 by hskim, ���Ǵ�Ƽ 3�� - Ű���� ������ ���� ó��
}

void CInfinityManager::DeleteKeyMonster_AliveForGameClearW(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx, MonIdx_t i_DeadMonsterIdx)
{
	mt_auto_lock mtD(&m_mtvectInfiDefence);
	mtvectInfiDefence::iterator itrD = m_mtvectInfiDefence.begin();
	for(; itrD != m_mtvectInfiDefence.end(); itrD++) {
		if(i_MapIndex == (*itrD)->GetInfinityMapIndex() 
			&& i_ChannelIdx == (*itrD)->GetInfinityChannelIndex()) {
			(*itrD)->DeleteKeyMonster_AliveForGameClear(i_DeadMonsterIdx);
			return;
		}
	}
	mtD.auto_unlock_cancel();

	// start 2011-06-05 by hskim, ���Ǵ�Ƽ 3�� - Ű���� ������ ���� ó��
	mt_auto_lock mtT(&m_mtvectInfiMShipBattle);
	mtvectInfiMShipBattle::iterator itrT = m_mtvectInfiMShipBattle.begin();
	for(; itrT != m_mtvectInfiMShipBattle.end(); itrT++) {
		if(i_MapIndex == (*itrT)->GetInfinityMapIndex()
			&& i_ChannelIdx == (*itrT)->GetInfinityChannelIndex()) {
			(*itrT)->DeleteKeyMonster_AliveForGameClear(i_DeadMonsterIdx);
			return;
		}
	}
	mtT.auto_unlock_cancel();
	// end 2011-06-05 by hskim, ���Ǵ�Ƽ 3�� - Ű���� ������ ���� ó��
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-03-23 by cmkwon, ���Ǵ�Ƽ ���� ĳ�� ������ ���� - 
/// \author		cmkwon
/// \date		2010-03-23 ~ 2010-03-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
Err_t CInfinityManager::SendFtoA_INFINITY_START_CHECK_W(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID)
{	
	switch (i_eInfiMode)
	{
	case INFINITY_MODE_BOSSRUSH:
		{
			mt_auto_lock mta(&m_mtvectInfiBossRush);
			mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
			for(; itr != m_mtvectInfiBossRush.end(); itr++)
			{ 
				if(i_CreateUID == (*itr)->GetInfinityCreateUID())
				{
					return (*itr)->SendFtoA_INFINITY_START_CHECK();
				}
			}
			mta.auto_unlock_cancel();
		}// case INFINITY_MODE_BOSSRUSH
		break; 
	case INFINITY_MODE_DEFENCE: {
		mt_auto_lock mta(&m_mtvectInfiDefence);
		mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();
		for(; itr != m_mtvectInfiDefence.end(); itr++)
		{ 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID())
			{
				return (*itr)->SendFtoA_INFINITY_START_CHECK();
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_DEFENCE
		break;
	case INFINITY_MODE_MSHIPBATTLE: {					// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
		mt_auto_lock mta(&m_mtvectInfiMShipBattle);
		mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
		for(; itr != m_mtvectInfiMShipBattle.end(); itr++)
		{ 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID())
			{
				return (*itr)->SendFtoA_INFINITY_START_CHECK();
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_MSHIPBATTLE
		break;
	default : {
		return ERR_INFINITY_MODE;
			  } // default
	}	
	
	return ERR_INFINITY_CREATEUID;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-03-23 by cmkwon, ���Ǵ�Ƽ ���� ĳ�� ������ ���� - 
/// \author		cmkwon
/// \date		2010-03-23 ~ 2010-03-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CFieldIOCPSocket *CInfinityManager::GetMasterPlayerW(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID)
{
	switch (i_eInfiMode)
	{
	case INFINITY_MODE_BOSSRUSH:
		{
			mt_auto_lock mta(&m_mtvectInfiBossRush);
			mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
			for(; itr != m_mtvectInfiBossRush.end(); itr++)
			{ 
				if(i_CreateUID == (*itr)->GetInfinityCreateUID())
				{
					return (*itr)->GetMasterPlayer();
				}
			}
			mta.auto_unlock_cancel();
		}// case INFINITY_MODE_BOSSRUSH
		break; 
	case INFINITY_MODE_DEFENCE: {
		mt_auto_lock mta(&m_mtvectInfiDefence);
		mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();
		for(; itr != m_mtvectInfiDefence.end(); itr++)
		{ 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID())
			{
				return (*itr)->GetMasterPlayer();
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_DEFENCE
		break;
	case INFINITY_MODE_MSHIPBATTLE: {				// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
		mt_auto_lock mta(&m_mtvectInfiMShipBattle);
		mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
		for(; itr != m_mtvectInfiMShipBattle.end(); itr++)
		{ 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID())
			{
				return (*itr)->GetMasterPlayer();
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_MSHIPBATTLE
		break;
	default : {
		return NULL;
			  } // default
	}	
	
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-03-23 by cmkwon, ���Ǵ�Ƽ ���� ĳ�� ������ ���� - 
/// \author		cmkwon
/// \date		2010-03-23 ~ 2010-03-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInfinityManager::CheckInfinityStartCheckAckW(eINFINITY_STATE *o_pInfiRoomState, MSG_FtoA_INFINITY_START_CHECK_ACK *i_pStartCheckAck)
{
	switch (i_pStartCheckAck->InfinityMode)
	{
	case INFINITY_MODE_BOSSRUSH:
		{
			mt_auto_lock mta(&m_mtvectInfiBossRush);
			mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
			for(; itr != m_mtvectInfiBossRush.end(); itr++)
			{ 
				if(i_pStartCheckAck->InfinityCreateUID == (*itr)->GetInfinityCreateUID())
				{
					return (*itr)->CheckInfinityStartCheckAck(o_pInfiRoomState, i_pStartCheckAck);
				}
			}
			mta.auto_unlock_cancel();
		}// case INFINITY_MODE_BOSSRUSH
		break; 
	case INFINITY_MODE_DEFENCE:
		{
			mt_auto_lock mta(&m_mtvectInfiDefence);
			mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();
			for(; itr != m_mtvectInfiDefence.end(); itr++)
			{ 
				if(i_pStartCheckAck->InfinityCreateUID == (*itr)->GetInfinityCreateUID())
				{
					return (*itr)->CheckInfinityStartCheckAck(o_pInfiRoomState, i_pStartCheckAck);
				}
			}
			mta.auto_unlock_cancel();
		} // case INFINITY_MODE_DEFENCE
		break;
	case INFINITY_MODE_MSHIPBATTLE:			// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
		{
			mt_auto_lock mta(&m_mtvectInfiMShipBattle);
			mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
			for(; itr != m_mtvectInfiMShipBattle.end(); itr++)
			{ 
				if(i_pStartCheckAck->InfinityCreateUID == (*itr)->GetInfinityCreateUID())
				{
					return (*itr)->CheckInfinityStartCheckAck(o_pInfiRoomState, i_pStartCheckAck);
				}
			}
			mta.auto_unlock_cancel();
		} // case INFINITY_MODE_MSHIPBATTLE
		break;
	default :
		{
			return FALSE;
		} // default
	}	
	
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-03-23 by cmkwon, ���Ǵ�Ƽ ���� ĳ�� ������ ���� - 
/// \author		cmkwon
/// \date		2010-03-23 ~ 2010-03-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInfinityManager::SetAllPlayerStateW(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, eINFINITY_STATE i_InfiState)
{
	switch (i_eInfiMode)
	{
	case INFINITY_MODE_BOSSRUSH:
		{
			mt_auto_lock mta(&m_mtvectInfiBossRush);
			mtvectInfiBossRush::iterator itr = m_mtvectInfiBossRush.begin();
			for(; itr != m_mtvectInfiBossRush.end(); itr++)
			{ 
				if(i_CreateUID == (*itr)->GetInfinityCreateUID())
				{
					(*itr)->SetAllPlayerState(i_InfiState);
					return;
				}
			}
			mta.auto_unlock_cancel();
		}// case INFINITY_MODE_BOSSRUSH
		break; 
	case INFINITY_MODE_DEFENCE: {
		mt_auto_lock mta(&m_mtvectInfiDefence);
		mtvectInfiDefence::iterator itr = m_mtvectInfiDefence.begin();
		for(; itr != m_mtvectInfiDefence.end(); itr++)
		{ 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID())
			{
				(*itr)->SetAllPlayerState(i_InfiState);
				return;
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_DEFENCE
		break;
	case INFINITY_MODE_MSHIPBATTLE: {				// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
		mt_auto_lock mta(&m_mtvectInfiMShipBattle);
		mtvectInfiMShipBattle::iterator itr = m_mtvectInfiMShipBattle.begin();
		for(; itr != m_mtvectInfiMShipBattle.end(); itr++)
		{ 
			if(i_CreateUID == (*itr)->GetInfinityCreateUID())
			{
				(*itr)->SetAllPlayerState(i_InfiState);
				return;
			}
		}
		mta.auto_unlock_cancel();
								} // case INFINITY_MODE_MSHIPBATTLE
		break;
	default : {
		return;
			  } // default
	}	
	
	return;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		cmkwon
/// \date		2010-04-05 ~ 2010-04-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CInfinityBossrush *CInfinityManager::FindBossrushNoLock(InfinityCreateUID_t i_CreateUID)
{
	mtvectInfiBossRush::iterator itr(m_mtvectInfiBossRush.begin());
	for(; itr != m_mtvectInfiBossRush.end(); itr++)
	{
		CInfinityBossrush *pInfinity = *itr;
		if(i_CreateUID == pInfinity->GetInfinityCreateUID())
		{			
			return pInfinity;
		}
	}	
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		cmkwon
/// \date		2010-04-05 ~ 2010-04-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CInfinityDefence *CInfinityManager::FindDefenceNoLock(InfinityCreateUID_t i_CreateUID)
{
	mtvectInfiDefence::iterator itr(m_mtvectInfiDefence.begin());
	for(; itr != m_mtvectInfiDefence.end(); itr++)
	{
		CInfinityDefence *pInfinity = *itr;
		if(i_CreateUID == pInfinity->GetInfinityCreateUID())
		{			
			return pInfinity;
		}
	}	
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
/// \author		hskim
/// \date		2011-02-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CInfinityMShipBattle *CInfinityManager::FindMShipBattleNoLock(InfinityCreateUID_t i_CreateUID)			
{
	// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
	
	mtvectInfiMShipBattle::iterator itr(m_mtvectInfiMShipBattle.begin());
	for(; itr != m_mtvectInfiMShipBattle.end(); itr++)
	{
		CInfinityMShipBattle *pInfinity = *itr;
		if(i_CreateUID == pInfinity->GetInfinityCreateUID())
		{			
			return pInfinity;
		}
	}	
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-09 by cmkwon, ����2�� �߰� ����(�ܰ躰 ���� �߰�) - 
/// \author		cmkwon
/// \date		2010-04-09 ~ 2010-04-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CInfinityBossrush *CInfinityManager::FindBossrushNoLock(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx)
{
	mtvectInfiBossRush::iterator itr(m_mtvectInfiBossRush.begin());
	for(; itr != m_mtvectInfiBossRush.end(); itr++)
	{
		CInfinityBossrush *pInfinity = *itr;
		if(i_MapIndex == pInfinity->GetInfinityMapIndex()
			&& i_ChannelIdx == pInfinity->GetInfinityChannelIndex())
		{
			return pInfinity;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-09 by cmkwon, ����2�� �߰� ����(�ܰ躰 ���� �߰�) - 
/// \author		cmkwon
/// \date		2010-04-09 ~ 2010-04-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CInfinityDefence *CInfinityManager::FindDefenceNoLock(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx)
{
	mtvectInfiDefence::iterator itr(m_mtvectInfiDefence.begin());
	for(; itr != m_mtvectInfiDefence.end(); itr++)
	{
		CInfinityDefence *pInfinity = *itr;
		if(i_MapIndex == pInfinity->GetInfinityMapIndex()
			&& i_ChannelIdx == pInfinity->GetInfinityChannelIndex())
		{			
			return pInfinity;
		}
	}	
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-09 by cmkwon, ����2�� �߰� ����(�ܰ躰 ���� �߰�) -> 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
/// \author		hskim
/// \date		2011-02-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CInfinityMShipBattle *CInfinityManager::FindMShipBattleNoLock(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx)
{
	// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�

	mtvectInfiMShipBattle::iterator itr(m_mtvectInfiMShipBattle.begin());
	for(; itr != m_mtvectInfiMShipBattle.end(); itr++)
	{
		CInfinityMShipBattle *pInfinity = *itr;
		if(i_MapIndex == pInfinity->GetInfinityMapIndex()
			&& i_ChannelIdx == pInfinity->GetInfinityChannelIndex())
		{			
			return pInfinity;
		}
	}	
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-09 by cmkwon, ����2�� �߰� ����(�ܰ躰 ���� �߰�) - 
/// \author		cmkwon
/// \date		2010-04-12 ~ 2010-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
Err_t CInfinityManager::PushTenderItemW(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx, CTenderItemInfo *i_pTenderItemInfo)
{
	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-09 by cmkwon, ����2�� �߰� ����(�ܰ躰 ���� �߰�) - 
	{
		mt_auto_lock mtA(&m_mtvectInfiBossRush);
		CInfinityBossrush *pInfinity = this->FindBossrushNoLock(i_MapIndex, i_ChannelIdx);
		if(pInfinity)
		{
			return pInfinity->PushTenderItem(i_pTenderItemInfo);
		}
	}
	{
		mt_auto_lock mtA(&m_mtvectInfiDefence);
		CInfinityDefence *pInfinity = this->FindDefenceNoLock(i_MapIndex, i_ChannelIdx);
		if(pInfinity)
		{
			return pInfinity->PushTenderItem(i_pTenderItemInfo);
		}
	}
	{			// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
		mt_auto_lock mtA(&m_mtvectInfiMShipBattle);
		CInfinityMShipBattle *pInfinity = this->FindMShipBattleNoLock(i_MapIndex, i_ChannelIdx);
		if(pInfinity)
		{
			return pInfinity->PushTenderItem(i_pTenderItemInfo);
		}
	}
	return ERR_INFINITY_CREATEUID;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-06 by cmkwon, ����2�� �߰� ���� - 
/// \author		cmkwon
/// \date		2010-04-06 ~ 2010-04-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
Err_t CInfinityManager::CheckInfinityAllPlayerStateW(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, eINFINITY_STATE i_infiState)
{
	// ���� ����
	switch (i_eInfiMode)
	{
	case INFINITY_MODE_BOSSRUSH:
		{
			mt_auto_lock mta(&m_mtvectInfiBossRush);
			///////////////////////////////////////////////////////////////////////////////
			// 2010-04-05 by cmkwon, ���� ������ ī�� ���� �ý��� ���� - 
			CInfinityBossrush *pInfinity = this->FindBossrushNoLock(i_CreateUID);
			if(pInfinity)
			{
//				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CInfinityManager::CheckInfinityAllPlayerStateW# 10000 !, InfiState(%d) Cnt(%d) \r\n"
//					, pInfinity->GetInfinityCreateUID(), pInfinity->GetInfinityState(), pInfinity->GetPlayerListSize());

				// 2012-07-17 by hskim, ���Ǵ�Ƽ �ο��� �����Ҵ� START �Ұ�� ���� ó��
				if( FALSE == pInfinity->CheckInfinityMinAdmission() )
				{
					return ERR_INFINITY_MIN_ADMISSIONCNT;
				}
				// end 2012-07-17 by hskim, ���Ǵ�Ƽ �ο��� �����Ҵ� START �Ұ�� ���� ó��

				if(FALSE == pInfinity->CheckInfinityAllPlayerState(i_infiState))
				{
					return ERR_INFINITY_NOT_ALL_READY;
				}
				return ERR_NO_ERROR;
			}
		} // case INFINITY_MODE_BOSSRUSH
		break; 
	case INFINITY_MODE_DEFENCE:
		{
			mt_auto_lock mta(&m_mtvectInfiDefence);
			///////////////////////////////////////////////////////////////////////////////
			// 2010-04-05 by cmkwon, ���� ������ ī�� ���� �ý��� ���� - 
			CInfinityDefence *pInfinity = this->FindDefenceNoLock(i_CreateUID);
			if(pInfinity)
			{
//				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CInfinityManager::CheckInfinityAllPlayerStateW# 20000 !, InfiState(%d) Cnt(%d) \r\n"
//					, pInfinity->GetInfinityCreateUID(), pInfinity->GetInfinityState(), pInfinity->GetPlayerListSize());

				// 2012-07-17 by hskim, ���Ǵ�Ƽ �ο��� �����Ҵ� START �Ұ�� ���� ó��
				if( FALSE == pInfinity->CheckInfinityMinAdmission() )
				{
					return ERR_INFINITY_MIN_ADMISSIONCNT;
				}
				// end 2012-07-17 by hskim, ���Ǵ�Ƽ �ο��� �����Ҵ� START �Ұ�� ���� ó��

				if(FALSE == pInfinity->CheckInfinityAllPlayerState(i_infiState))
				{
					return ERR_INFINITY_NOT_ALL_READY;
				}
				return ERR_NO_ERROR;
			}
		} // case INFINITY_MODE_DEFENCE
		break;
	case INFINITY_MODE_MSHIPBATTLE:				// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
		{
			mt_auto_lock mta(&m_mtvectInfiMShipBattle);

			CInfinityMShipBattle *pInfinity = this->FindMShipBattleNoLock(i_CreateUID);
			if(pInfinity)
			{				
				// 2012-07-17 by hskim, ���Ǵ�Ƽ �ο��� �����Ҵ� START �Ұ�� ���� ó��
				if( FALSE == pInfinity->CheckInfinityMinAdmission() )
				{
					return ERR_INFINITY_MIN_ADMISSIONCNT;
				}
				// end 2012-07-17 by hskim, ���Ǵ�Ƽ �ο��� �����Ҵ� START �Ұ�� ���� ó��

				if(FALSE == pInfinity->CheckInfinityAllPlayerState(i_infiState))
				{
					return ERR_INFINITY_NOT_ALL_READY;
				}
				return ERR_NO_ERROR;
			}
		} // case INFINITY_MODE_MSHIPBATTLE
		break;
	default :
		{
			return ERR_INFINITY_MODE;
		} // default
	}
	
	return ERR_INFINITY_CREATEUID;
}


// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
/************************************************************************************************
**
**	���Ǵ�Ƽ ���̵� ���� - �������� ������ ���̵� ���� ��������
**
**	Create Info :	2010-05-13 by shcho
**
**	Update Infi :	���� �����ͷ� ����, ��� �� �����ϵ��� ����.	2010. 05. 24. by hsLee.
**
*************************************************************************************************/
BOOL CInfinityManager::Get_Difficulty_BonusInfo_ListData(vectorInfinity_DifficultyInfo_Bonus *p_vecInfinity_DifficulytList) 
{
	if ( NULL == p_vecInfinity_DifficulytList )
		return FALSE;

	p_vecInfinity_DifficulytList->clear();
	p_vecInfinity_DifficulytList->assign( m_vecInfinityDifficultyBonusInfo.begin() , m_vecInfinityDifficultyBonusInfo.end() );

	return TRUE;
}


/**********************************************************************************************
**
**	���Ǵ�Ƽ ���̵� ���� - ���ʽ� ������ �����ϱ�
**
**	Create Info :	2010-05-13 by shcho
**
**	Update Info :	���ڸ� ����, ��� �� �����ϵ��� ����.	2010. 05. 24. by hsLee.
**
***********************************************************************************************/
BOOL CInfinityManager::Set_Difficulty_BonusInfo_ListData(vectorInfinity_DifficultyInfo_Bonus *p_vecListInfo)
{
	if ( NULL == p_vecListInfo )
		return FALSE;

	m_vecInfinityDifficultyBonusInfo.clear();
	m_vecInfinityDifficultyBonusInfo.assign( p_vecListInfo->begin(), p_vecListInfo->end() );

	return TRUE;
}


/**********************************************************************************************
**
**	���Ǵ�Ƽ ���̵� ���� - ���ʽ� ������ ã�´�.
**
**	Create Info :	2010-05-13 by shcho
**
**	Update Info :	���ڸ� ����, ���� ó�� ����.	2010. 05. 24. by hsLee.
**
***********************************************************************************************/
const INFINITY_DIFFICULTY_BONUS_INFO *CInfinityManager :: Get_Difficulty_BonusInfo ( const int a_iStep )
{

	vectorInfinity_DifficultyInfo_Bonus::iterator it = m_vecInfinityDifficultyBonusInfo.begin();

	while ( it != m_vecInfinityDifficultyBonusInfo.end() )
	{
		if ( it->iIncreaseStep == a_iStep )
			return &*it;

		++it;
	}

	return NULL;

}


/**********************************************************************************************
**
**	���Ǵ�Ƽ ���̵� ���� - ���� �뷱�� ������ ã�´�.
**
**	Create Info :	2010-05-13 by shcho
**
**	Update Info :	���ڸ� ����, ���� ó�� ����.	2010. 05. 24. by hsLee.
**
***********************************************************************************************/
const INFINITY_DIFFICULTY_MONSTER_SETTING_INFO *CInfinityManager::Get_Difficulty_MonsterInfo(int iStep) 
{
	vectorInfinity_DifficultyInfo_Monster::iterator iter = m_vecInfinityDifficultyMonsterInfo.begin(); // ���ܿ� ���� ������ ã�´�.
	
	while ( iter != m_vecInfinityDifficultyMonsterInfo.end() )
	{
		if ( iter->iIncreaseStep == iStep )
			return &*iter;

		++iter;
	}

	return NULL;
}


/**********************************************************************************************
**
**	���Ǵ�Ƽ ���̵� ���� - ���� �뷱�� ������ �����Ѵ�.
**
**	Create Info :	2010-05-13 by shcho
**
**	Update Info :	���ڸ� ����.	2010. 05. 24. by hsLee.
**
***********************************************************************************************/
BOOL CInfinityManager::Set_Difficulty_MonsterInfo_ListData(vectorInfinity_DifficultyInfo_Monster* p_vecListInfo)
{
	if ( NULL == p_vecListInfo )
		return FALSE;

	m_vecInfinityDifficultyMonsterInfo.clear();
	m_vecInfinityDifficultyMonsterInfo.assign( p_vecListInfo->begin() , p_vecListInfo->end() );

	return TRUE;
}

// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )


// 2010. 06. 04 by hsLee ��Ƽ�Ǵ� �ʵ� 2�� ���̵� ����. (GM ��ɾ� �߰�. /nextscene(���� �ó׸� �� ȣ��.) ) - GM ���� ���׸� �ٷ� ȣ��.
/**************************************************************************************************************************************************
**
**	���Ǵ�Ƽ : GM�� ���� �ó׸� �ٷ� ȣ�� ó��.
**
**	Create Info :	2010. 06. 04 by hsLee.
**
**	Update Info :	���ڸ� ����.	2010. 05. 24. by hsLee.
**
**					�ѹ� ȣ��� ���� ������ �ó׸� ���� ���� ���� ó��. ( nUpdateSceneCount )	2010. 08. 20. by hsLee.
**
***************************************************************************************************************************************************/
void CInfinityManager :: UpdateNextSceneProc ( eINFINITY_MODE i_eInfiMode , InfinityCreateUID_t i_CreateUID , CFieldIOCPSocket * i_pMasterFISoc , int nUpdateSceneCount /*= 1*/ )
{

	if (!i_pMasterFISoc) return;

	nUpdateSceneCount = max ( nUpdateSceneCount , 1 );

	switch ( i_eInfiMode )
	{
		case INFINITY_MODE_BOSSRUSH :
			{
				mt_auto_lock mtR(&m_mtvectInfiBossRush);

				mtvectInfiBossRush::iterator itrR = m_mtvectInfiBossRush.begin();

				while ( itrR != m_mtvectInfiBossRush.end() )
				{
					if ( (*itrR)->GetInfinityCreateUID() == i_CreateUID )
					{
						(*itrR)->ProcessingCinema( i_pMasterFISoc , nUpdateSceneCount );

						break;
					}

					itrR++;
				}

				mtR.auto_unlock_cancel();
			}
			break;

		case INFINITY_MODE_DEFENCE :
			{
				mt_auto_lock mtD(&m_mtvectInfiDefence);

				mtvectInfiDefence::iterator itrD = m_mtvectInfiDefence.begin();

				while ( itrD != m_mtvectInfiDefence.end() )
				{
					if ( (*itrD)->GetInfinityCreateUID() == i_CreateUID )
					{
						(*itrD)->ProcessingCinema ( i_pMasterFISoc , nUpdateSceneCount );

						break;
					}
					
					itrD++;
				}

				mtD.auto_unlock_cancel();
			}
			break;

		case INFINITY_MODE_MSHIPBATTLE :				// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
			{
				mt_auto_lock mtD(&m_mtvectInfiMShipBattle);
				
				mtvectInfiMShipBattle::iterator itrD = m_mtvectInfiMShipBattle.begin();
				
				while ( itrD != m_mtvectInfiMShipBattle.end() )
				{
					if ( (*itrD)->GetInfinityCreateUID() == i_CreateUID )
					{
						(*itrD)->ProcessingCinema ( i_pMasterFISoc , nUpdateSceneCount );
						
						break;
					}
					
					itrD++;
				}
				
				mtD.auto_unlock_cancel();
			}
			break;
	}
	
}
// End 2010. 06. 04 by hsLee ��Ƽ�Ǵ� �ʵ� 2�� ���̵� ����. (GM ��ɾ� �߰�. /nextscene(���� �ó׸� �� ȣ��.) ) - GM ���� ���׸� �ٷ� ȣ��.