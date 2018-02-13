// ArenaDeathMatchRoomManager.cpp: implementation of the CArenaDeathMatchRoom class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ArenaDeathMatchRoom.h"
#include "FieldIOCP.h"
#include "FieldIOCPSocket.h"
#include "FieldGlobal.h"			// 2007-05-18 by cmkwon
#include "AtumLogSender.h"

#define	DEFINE_ARENA_MAX_PALY_COUNT 10	// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : �ӽ÷� �ھƵ� ����Ƚ�� ������ [0�� ������ = �⺻���� ������ �ְڴ�]
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CArenaDeathMatchRoom::CArenaDeathMatchRoom()
{

}

CArenaDeathMatchRoom::~CArenaDeathMatchRoom()
{

}

void CArenaDeathMatchRoom::InitArenaDeathMatchRoom(CFieldIOCP *i_pFIOCP)
{
	m_DeathMatchRoomNum = -1;
	util::zero(&m_map,sizeof(MAP_CHANNEL_INDEX));
	m_CreateRoomTime.Reset();
	m_VCNDeathCount		= 0;
	m_ANIDeathCount		= 0;
	m_mtvectVCNTeam.clear();
	m_mtvectANITeam.clear();
	m_pFieldIOCP17		= i_pFIOCP;
	util::zero(&m_PayInfo,sizeof(SARENA_PAY_INFO));
	m_PlayLimitedTime	= 0;
	m_VCNTeamNum		= 0;
	m_ANITeamNum		= 0;
	m_FinishRoomTime.Reset();
	m_DeathMatchState	= STATE_ERROR;
	m_InitDeathMatchRoom= FALSE;
	m_mtvectWatchUserList.clear();
	//m_VCNMFSID			= 0;		// 2008-02-19 by dhjin, �Ʒ��� ���� - 
	//m_ANIMFSID			= 0;		// 2008-02-19 by dhjin, �Ʒ��� ���� - 
	m_VCNMFSInfl		= 0;		// 2008-02-19 by dhjin, �Ʒ��� ���� - 
	m_ANIMFSInfl		= 0;		// 2008-02-19 by dhjin, �Ʒ��� ���� - 
	m_fVCNTeamPercent	= 0;		// 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ð��ʰ��� ���� VCN�� ��ü �ۼ�Ʈ �� 
	m_fANITeamPercent	= 0;		// 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ð��ʰ��� ���� ANI�� ��ü �ۼ�Ʈ �� 
	m_fVCNTeamDamage	= 0;		// 2012-07-10 by jhseol, �Ʒ��� ���� - ������ ����
	m_fANITeamDamage	= 0;		// 2012-07-10 by jhseol, �Ʒ��� ���� - ������ ����

}

void CArenaDeathMatchRoom::SetDeathMatchMap(MAP_CHANNEL_INDEX i_SMap)
{
	m_map	= i_SMap;
}

void CArenaDeathMatchRoom::SetVCNDeathCount(BYTE i_VCNDeathCount)
{
	m_VCNDeathCount		= i_VCNDeathCount;
	m_MaxVCNDeathCount	= i_VCNDeathCount;
}

void CArenaDeathMatchRoom::SetANIDeathCount(BYTE i_ANIDeathCount)
{
	m_ANIDeathCount		= i_ANIDeathCount;
	m_MaxANIDeathCount  = i_ANIDeathCount;
}

void CArenaDeathMatchRoom::SetVCNTeam(vectorClientIndex * i_pVCNTeam, int i_nTeamNum)
{
	mt_auto_lock mtA(&m_mtvectVCNTeam);
	m_VCNTeamNum	= i_nTeamNum;
	m_mtvectVCNTeam.assign(i_pVCNTeam->begin(), i_pVCNTeam->end());
}	

void CArenaDeathMatchRoom::SetANITeam(vectorClientIndex * i_pANITeam, int i_nTeamNum)
{
	mt_auto_lock mtA(&m_mtvectANITeam);
	m_ANITeamNum	= i_nTeamNum;
	m_mtvectANITeam.assign(i_pANITeam->begin(), i_pANITeam->end());
}

void CArenaDeathMatchRoom::SetCreateRoomTime()
{
	m_CreateRoomTime.SetCurrentDateTime();
	//////////////////////////////////////////////////////////////////////////
	// 2007-05-02 by dhjin, �� ������ ���߾� CArenaManager�� ����ȭ�� ��Ų��, ��� �ð��� ���� ��� ���� �ð��� �ȴ�
	m_CreateRoomTime.Second = 0;
	m_CreateRoomTime.AddDateTime(0,0,0,0,2,0);
}

void CArenaDeathMatchRoom::SetDeathMatchRoomNum(INT i_nDeathMatchRoomNum)
{
	m_DeathMatchRoomNum = i_nDeathMatchRoomNum;
	m_DeathMatchState	= ARENA_STATE_FIGHTING_WARREADY;
}

void CArenaDeathMatchRoom::SetPayInfo(SARENA_PAY_INFO * i_PayInfo)
{
	m_PayInfo.PayInfluencePointWIN	= i_PayInfo->PayInfluencePointWIN;
	m_PayInfo.PayInfluencePointLOSE	= i_PayInfo->PayInfluencePointLOSE;
	m_PayInfo.PayWarPointWIN		= i_PayInfo->PayWarPointWIN;
	m_PayInfo.PayWarPointLOSE		= i_PayInfo->PayWarPointLOSE;
	//////////////////////////////////////////////////////////////////////////////
	// 2012-05-23 by jhseol, �Ʒ��� �߰����� - �ּ� ���� ���� �ʵ� �߰�
	m_PayInfo.DefaultPayWarPointWIN	= i_PayInfo->DefaultPayWarPointWIN;
	m_PayInfo.DefaultPayWarPointLOSE= i_PayInfo->DefaultPayWarPointLOSE;
	// end 2012-05-23 by jhseol, �Ʒ��� �߰����� - �ּ� ���� ���� �ʵ� �߰�

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BYTE CArenaDeathMatchRoom::GetDeathMatchState(void)
/// \brief		
/// \author		cmkwon
/// \date		2007-05-21 ~ 2007-05-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE CArenaDeathMatchRoom::GetDeathMatchState(void)
{
	return m_DeathMatchState;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT CArenaDeathMatchRoom::GetVCNTeamNum(void)
/// \brief		
/// \author		cmkwon
/// \date		2007-05-21 ~ 2007-05-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CArenaDeathMatchRoom::GetVCNTeamNum(void)
{
	return m_VCNTeamNum;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT CArenaDeathMatchRoom::GetANITeamNum(void)
/// \brief		
/// \author		cmkwon
/// \date		2007-05-21 ~ 2007-05-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CArenaDeathMatchRoom::GetANITeamNum(void)
{
	return m_ANITeamNum;
}


INT	CArenaDeathMatchRoom::GetDeathMatchRoomNum()
{
	return m_DeathMatchRoomNum;
}

ATUM_DATE_TIME*	CArenaDeathMatchRoom::GetCreateRoomTime()
{
	return &m_CreateRoomTime;
}

MAP_CHANNEL_INDEX* CArenaDeathMatchRoom::GetMapInfo()
{
	return &m_map;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::DoMinutelyWorkArenaDeathMatchRoom(ATUM_DATE_TIME *pDateTime)
/// \brief		�ʸ��� üũ �ؾ� �Ǵ� �۾�
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::DoMinutelyWorkArenaDeathMatchRoom(ATUM_DATE_TIME *pDateTime)
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-05-04 by dhjin, ���и� �Ǵ��ϴ� �κ��̹Ƿ� ���� ����ȭ ó���� ���� ��ü���� Lock�� ��´�
	mt_auto_lock mtW(&m_mtvectWatchUserList);
	mt_auto_lock mtB(&m_mtvectVCNTeam);
	mt_auto_lock mtA(&m_mtvectANITeam);
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-05-04 by dhjin, �������� ��ȿ���� üũ�Ѵ�.
	this->CheckLiveMemberByDoMinutelyWorkArena();

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-08 by dhjin, �� ������ ���ٸ� ���� �ʱ�ȭ �Ѵ�.
	if(this->CheckEmptyTeamSize())
	{
		m_InitDeathMatchRoom = TRUE;
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-20 by dhjin, ���� �������� 1���� �������� üũ
// 2009-09-10 by jwlee, �Ʒ��� ���� �ð� ���� ���� ���� - 
//	if(ARENA_STATE_FIGHTING_WARREADY == m_DeathMatchState
//		&& 1 <= pDateTime->GetTimeDiffTimeInMinutes(m_CreateRoomTime))
	// 2009-09-10 by jwlee, �Ʒ��� ���� �ð� ���� ���� ���� - �Ʒ��� ���� ������.
	if(ARENA_STATE_FIGHTING_WARREADY == m_DeathMatchState
		&& 0 <= pDateTime->GetTimeDiffTimeInMinutes(m_CreateRoomTime))
	{
		m_DeathMatchState = ARENA_STATE_FIGHTING_WARING;
		this->WarStart();
		
		//////////////////////////////////////////////////////////////////////////
		// 2007-06-18 by dhjin, �����ڿ��� ���� ������ ������.
		this->SendWatchStartInfo();
		
//		// 2007-05-18 by cmkwon, �׽�Ʈ �α�
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] DMRoomStart(2-1) - DRoomNum(%4d) AState(%30s) BTeamNum(%4d) ATeamNum(%4d) MapInfo(%s)\r\n" 
//				, m_DeathMatchRoomNum, GET_ARENA_STATE_STRING(m_DeathMatchState), m_VCNTeamNum, m_ANITeamNum, GET_MAP_STRING(m_map));
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-24 by dhjin, ������ ���� ���� �ʰ� �� �� ���̶� �� �������
	if(ARENA_STATE_FIGHTING_WARREADY == m_DeathMatchState
		&& (0 == this->GetVCNTeamSize()
			|| 0 == this->GetANITeamSize() ) )
	{
		this->WarFinishDraw(ARENA_END_TYPE_GIVEUP);

//		// 2007-05-18 by cmkwon, �׽�Ʈ �α�
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] DMRoomEnd(4-1) - DRoomNum(%4d) AState(%30s) BTeamNum(%4d) ATeamNum(%4d) MapInfo(%s), ������ ���� ���� �ʰ� �� �� ���̶� �� �������\r\n" 
//				, m_DeathMatchRoomNum, GET_ARENA_STATE_STRING(m_DeathMatchState), m_VCNTeamNum, m_ANITeamNum, GET_MAP_STRING(m_map));
		return;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-24 by dhjin, ������ ���� �Ǿ����� ����Ʈ�� �߻����� �ʰ� �� ���� ��� ���� ��� 
	if(ARENA_STATE_FIGHTING_WARING == m_DeathMatchState
		&& m_VCNDeathCount == m_MaxVCNDeathCount
		&& m_ANIDeathCount == m_MaxANIDeathCount
		&& (0 == this->GetVCNTeamSize()
			|| 0 == this->GetANITeamSize() ) )
	{
		this->WarFinishDraw(ARENA_END_TYPE_GIVEUP);

//		// 2007-05-18 by cmkwon, �׽�Ʈ �α�
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] DMRoomEnd(4-2) - DRoomNum(%4d) AState(%30s) BTeamNum(%4d) ATeamNum(%4d) MapInfo(%s), ������ ���� �Ǿ����� ����Ʈ�� �߻����� �ʰ� �� ���� ��� ���� ���\r\n" 
//				, m_DeathMatchRoomNum, GET_ARENA_STATE_STRING(m_DeathMatchState), m_VCNTeamNum, m_ANITeamNum, GET_MAP_STRING(m_map));
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-24 by dhjin, ���� ���� �� ����Ʈ�� �߻��� �������� �� ���� ��� ���� ���
	if(ARENA_STATE_FIGHTING_WARING == m_DeathMatchState
		&& (m_VCNDeathCount != m_MaxVCNDeathCount
			|| m_ANIDeathCount != m_MaxANIDeathCount)
		&& (0 == this->GetVCNTeamSize()
			|| 0 == this->GetANITeamSize() ) )
	{
		if(0 == this->GetVCNTeamSize()
			&& 0 == this->GetANITeamSize())
		{
			this->WarFinishDraw(ARENA_END_TYPE_GIVEUP);
		}
		else if(0 == this->GetVCNTeamSize())
		{
			m_ANIDeathCount = 0;
		}
		else if(0 == this->GetANITeamSize())
		{
			m_VCNDeathCount = 0;
		}
		this->WarFinish(ARENA_END_TYPE_GIVEUP);

//		// 2007-05-18 by cmkwon, �׽�Ʈ �α�
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] DMRoomEnd(4-3) - DRoomNum(%4d) AState(%30s) BTeamNum(%4d) ATeamNum(%4d) MapInfo(%s), ���� ���� �� ����Ʈ�� �߻��� �������� �� ���� ��� ���� ���\r\n" 
//				, m_DeathMatchRoomNum, GET_ARENA_STATE_STRING(m_DeathMatchState), m_VCNTeamNum, m_ANITeamNum, GET_MAP_STRING(m_map));
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-30 by dhjin, ������ ����Ǳ� 1������ Ŭ���̾�Ʈ���� ���� ����
	if(ARENA_STATE_FIGHTING_WARING == m_DeathMatchState
		&& ( (m_PlayLimitedTime - 1) <= pDateTime->GetTimeDiffTimeInMinutes(m_CreateRoomTime) )
		&& (m_PlayLimitedTime > pDateTime->GetTimeDiffTimeInMinutes(m_CreateRoomTime)))
	{
		this->BeforeWarFinish();

//		// 2007-05-18 by cmkwon, �׽�Ʈ �α�
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] DMRoomNotify(3-1) - DRoomNum(%4d) AState(%30s) BTeamNum(%4d) ATeamNum(%4d) MapInfo(%s), ������ ����Ǳ� 1������ Ŭ���̾�Ʈ���� ���� ����\r\n" 
//				, m_DeathMatchRoomNum, GET_ARENA_STATE_STRING(m_DeathMatchState), m_VCNTeamNum, m_ANITeamNum, GET_MAP_STRING(m_map));
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-24 by dhjin, ����Ʈ�� �߻� �Ͽ�����, ��ǥ ����Ʈ�� ȹ������ ���ϰ� �ð��� �ʰ��� ���
	if(ARENA_STATE_FIGHTING_WARING == m_DeathMatchState
		&& (m_VCNDeathCount != m_MaxVCNDeathCount
			|| m_ANIDeathCount != m_MaxANIDeathCount)
		&& (m_PlayLimitedTime <= pDateTime->GetTimeDiffTimeInMinutes(m_CreateRoomTime)))
	{
		if(m_VCNDeathCount == m_ANIDeathCount)
		{
			///////////////////////////////////////////////////////////////////////////////
			// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ������� ��ȭ
#ifndef S_ARENA_EX_1ST_JHSEOL
			this->WarFinishDraw(ARENA_END_TYPE_TIME);
			return;
#else

			// 2012-07-10 by jhseol, �Ʒ��� ���� - ������ ���� �������� �������� ��� ����
/*
			// 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ð��ʰ��� ���� VCN�� ������, �ǵ� ��� 
			mtvectorClientIndex_t::iterator itrA = m_mtvectVCNTeam.begin();
			while(itrA != m_mtvectVCNTeam.end())
			{
				CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrA);
				if(NULL == tm_pFISocket)
				{
					itrA++;
					continue;
				}
				if( (float)tm_pFISocket->m_character.HP + (float)tm_pFISocket->m_character.DP != FALSE )
				{
					m_fVCNTeamPercent	+= ((float)tm_pFISocket->m_character.CurrentHP + (float)tm_pFISocket->m_character.CurrentDP) / ((float)tm_pFISocket->m_character.HP + (float)tm_pFISocket->m_character.DP);
				}
				else
				{
					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] ���� HP/DP ��ġ ����!! CUID:%d, HP:%d, DP:%d\r\n",tm_pFISocket->m_character.CharacterUniqueNumber, tm_pFISocket->m_character.HP, tm_pFISocket->m_character.DP );
				}
				itrA++;
			}	
			// end 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ð��ʰ��� ���� VCN�� ������, �˵?��� 

			// 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ð��ʰ��� ���� ANI�� ������, �ǵ� ��� 
			mtvectorClientIndex_t::iterator itrB = m_mtvectANITeam.begin();
			while(itrB != m_mtvectANITeam.end())
			{
				CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrB);
				if(NULL == tm_pFISocket)
				{
					itrB++;
					continue;
				}
				if( (float)tm_pFISocket->m_character.HP + (float)tm_pFISocket->m_character.DP != FALSE )
				{
					m_fANITeamPercent	+= ((float)tm_pFISocket->m_character.CurrentHP + (float)tm_pFISocket->m_character.CurrentDP) / ((float)tm_pFISocket->m_character.HP + (float)tm_pFISocket->m_character.DP);
				}
				else
				{
					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] ���� HP/DP ��ġ ����!! CUID:%d, HP:%d, DP:%d\r\n",tm_pFISocket->m_character.CharacterUniqueNumber, tm_pFISocket->m_character.HP, tm_pFISocket->m_character.DP );
				}
				itrB++;
			}	
			// end 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ð��ʰ��� ���� ANI�� ������, �˵?��� 
*/
			// end 2012-07-10 by jhseol, �Ʒ��� ���� - ������ ���� �������� �������� ��� ����

			// 2012-07-10 by jhseol, �Ʒ��� ���� - ������ ���� ���
			if( 0 != m_fVCNTeamDamage + m_fANITeamDamage )
			{
				m_fVCNTeamPercent = ( m_fVCNTeamDamage / ( m_fVCNTeamDamage + m_fANITeamDamage ) )*100;
				m_fANITeamPercent = 100 - m_fVCNTeamPercent;
				if( m_fVCNTeamPercent > 50)
				{
					m_fVCNTeamPercent = m_fVCNTeamPercent + 1;
					if( m_fVCNTeamPercent > 100)
					{
						m_fVCNTeamPercent = 100;
					}
				}
				else
				{
					m_fANITeamPercent = m_fANITeamPercent + 1;
					if( m_fANITeamPercent > 100)
					{
						m_fANITeamPercent = 100;
					}
				}
			}
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] Arena Result | DeathMatchRoomNum[%d], BCUTeamNum[%d], ANITeamNum[%d] | BCUTeamDamage[%f], ANITeamDamage[%f] : BCUTeamPercent[%f], ANITeamPercent[%f]\r\n"
				,m_DeathMatchRoomNum, m_VCNTeamNum, m_ANITeamNum, m_fVCNTeamDamage, m_fANITeamDamage, m_fVCNTeamPercent, m_fANITeamPercent );
			// end 2012-07-10 by jhseol, �Ʒ��� ���� - ������ ���� ���

			// 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ð��ʰ��� ���� ������ �� Percent�� ���Ͽ� ���� ū ���� �¸� 
			if(m_fVCNTeamPercent == m_fANITeamPercent)
			{
				this->WarFinishDraw(ARENA_END_TYPE_TIME);
				return;
			}

			if(m_fVCNTeamPercent < m_fANITeamPercent)
			{
				m_ANIDeathCount = 0;
			}
			else
			{
				m_VCNDeathCount = 0;
			}
			this->WarFinish(ARENA_END_TYPE_TIME);
			return;
#endif
			// end 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ð��ʰ��� ���� ������ �� Percent�� ���Ͽ� ���� ū ���� �¸�
			// end 2012-04-12 by jhseol, �Ʒ��� �߰����� - ������� ��ȭ

//			// 2007-05-18 by cmkwon, �׽�Ʈ �α�
//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] DMRoomEnd(4-4) - DRoomNum(%4d) AState(%30s) BTeamNum(%4d) ATeamNum(%4d) MapInfo(%s), ����Ʈ�� �߻� �Ͽ�����, ��ǥ ����Ʈ�� ȹ������ ���ϰ� �ð��� �ʰ��� ���\r\n" 
//					, m_DeathMatchRoomNum, GET_ARENA_STATE_STRING(m_DeathMatchState), m_VCNTeamNum, m_ANITeamNum, GET_MAP_STRING(m_map));			
		}
		
		if(m_VCNDeathCount > m_ANIDeathCount)
		{
			m_ANIDeathCount = 0;
		}
		else
		{
			m_VCNDeathCount = 0;
		}
		this->WarFinish(ARENA_END_TYPE_TIME);
		
//		// 2007-05-18 by cmkwon, �׽�Ʈ �α�
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] DMRoomEnd(4-5) - DRoomNum(%4d) AState(%30s) BTeamNum(%4d) ATeamNum(%4d) MapInfo(%s), ����Ʈ�� �߻� �Ͽ�����, ��ǥ ����Ʈ�� ȹ������ ���ϰ� �ð��� �ʰ��� ���\r\n" 
//				, m_DeathMatchRoomNum, GET_ARENA_STATE_STRING(m_DeathMatchState), m_VCNTeamNum, m_ANITeamNum, GET_MAP_STRING(m_map));			
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-24 by dhjin, ����Ʈ�� �߻����� ���� ���¿��� �ð��� �ʰ��� ���
	if(ARENA_STATE_FIGHTING_WARING == m_DeathMatchState
		&& m_VCNDeathCount == m_MaxVCNDeathCount
		&& m_ANIDeathCount == m_MaxANIDeathCount
		&& (m_PlayLimitedTime <= pDateTime->GetTimeDiffTimeInMinutes(m_CreateRoomTime)))
	{
		if(m_VCNDeathCount == m_ANIDeathCount)
		{
			///////////////////////////////////////////////////////////////////////////////
			// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ������� ��ȭ
#ifndef S_ARENA_EX_1ST_JHSEOL
		this->WarFinishDraw(ARENA_END_TYPE_TIME);
			return;
#else
		
			// 2012-07-10 by jhseol, �Ʒ��� ���� - ������ ���� �������� �������� ��� ����
/*
			// start 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ð��ʰ��� ���� VCN�� ������, �ǵ� ��� 
			mtvectorClientIndex_t::iterator itrA = m_mtvectVCNTeam.begin();
			while(itrA != m_mtvectVCNTeam.end())
			{
				CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrA);
				if(NULL == tm_pFISocket)
				{
					itrA++;
					continue;
				}
				if( (float)tm_pFISocket->m_character.HP + (float)tm_pFISocket->m_character.DP != FALSE )
				{
					m_fVCNTeamPercent	+= ((float)tm_pFISocket->m_character.CurrentHP + (float)tm_pFISocket->m_character.CurrentDP) / ((float)tm_pFISocket->m_character.HP + (float)tm_pFISocket->m_character.DP);
				}
				else
				{
					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] ���� HP/DP ��ġ ����!! CUID:%d, HP:%d, DP:%d\r\n",tm_pFISocket->m_character.CharacterUniqueNumber, tm_pFISocket->m_character.HP, tm_pFISocket->m_character.DP );
				}
				itrA++;
			}	
			// end 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ð��ʰ��� ���� VCN�� ������, �˵?��� 
			
			// 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ð��ʰ��� ���� ANI�� ������, �ǵ� ��� 
			mtvectorClientIndex_t::iterator itrB = m_mtvectANITeam.begin();
			while(itrB != m_mtvectANITeam.end())
			{
				CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrB);
				if(NULL == tm_pFISocket)
				{
					itrB++;
					continue;
				}
				if( (float)tm_pFISocket->m_character.HP + (float)tm_pFISocket->m_character.DP != FALSE )
				{
					m_fANITeamPercent	+= ((float)tm_pFISocket->m_character.CurrentHP + (float)tm_pFISocket->m_character.CurrentDP) / ((float)tm_pFISocket->m_character.HP + (float)tm_pFISocket->m_character.DP);
				}
				else
				{
					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] ���� HP/DP ��ġ ����!! CUID:%d, HP:%d, DP:%d\r\n",tm_pFISocket->m_character.CharacterUniqueNumber, tm_pFISocket->m_character.HP, tm_pFISocket->m_character.DP );
				}
				itrB++;
			}	
			// end 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ð��ʰ��� ���� ANI�� ������, �˵?��� 
*/
			// end 2012-07-10 by jhseol, �Ʒ��� ���� - ������ ���� �������� �������� ��� ����
			
			// 2012-07-10 by jhseol, �Ʒ��� ���� - ������ ���� ���
			if( 0 != m_fVCNTeamDamage + m_fANITeamDamage )
			{
				m_fVCNTeamPercent = ( m_fVCNTeamDamage / ( m_fVCNTeamDamage + m_fANITeamDamage ) )*100;
				m_fANITeamPercent = 100 - m_fVCNTeamPercent;
				if( m_fVCNTeamPercent > 50)
				{
					m_fVCNTeamPercent = m_fVCNTeamPercent + 1;
					if( m_fVCNTeamPercent > 100)
					{
						m_fVCNTeamPercent = 100;
					}
				}
				else
				{
					m_fANITeamPercent = m_fANITeamPercent + 1;
					if( m_fANITeamPercent > 100)
					{
						m_fANITeamPercent = 100;
					}
				}
			}
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] Arena Result | DeathMatchRoomNum[%d], BCUTeamNum[%d], ANITeamNum[%d] | BCUTeamDamage[%f], ANITeamDamage[%f] : BCUTeamPercent[%f], ANITeamPercent[%f]\r\n"
				,m_DeathMatchRoomNum, m_VCNTeamNum, m_ANITeamNum, m_fVCNTeamDamage, m_fANITeamDamage, m_fVCNTeamPercent, m_fANITeamPercent );
			// end 2012-07-10 by jhseol, �Ʒ��� ���� - ������ ���� ���
			
			// 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ð��ʰ��� ���� ������ �� Percent�� ���Ͽ� ���� ū ���� �¸� 
			if(m_fVCNTeamPercent == m_fANITeamPercent)
			{
				this->WarFinishDraw(ARENA_END_TYPE_TIME);
				return;
			}
			
			if(m_fVCNTeamPercent > m_fANITeamPercent)
			{
				m_VCNDeathCount = 0;
			}
			else
			{
				m_ANIDeathCount = 0;
			}
			this->WarFinish(ARENA_END_TYPE_TIME);
			return;
#endif
			// end 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ð��ʰ��� ���� ������ �� Percent�� ���Ͽ� ���� ū ���� �¸� 
			// end 2012-04-12 by jhseol, �Ʒ��� �߰����� - ������� ��ȭ

		}

//		// 2007-05-18 by cmkwon, �׽�Ʈ �α�
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] DMRoomEnd(4-6) - DRoomNum(%4d) AState(%30s) BTeamNum(%4d) ATeamNum(%4d) MapInfo(%s), ����Ʈ�� �߻����� ���� ���¿��� �ð��� �ʰ��� ���\r\n" 
//				, m_DeathMatchRoomNum, GET_ARENA_STATE_STRING(m_DeathMatchState), m_VCNTeamNum, m_ANITeamNum, GET_MAP_STRING(m_map));
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-24 by dhjin, �Ʒ��� �� ���ᰡ �ǰ� 3���� ���� ��� ������ ������ ������Ű�� ���� �ʱ�ȭ �Ѵ�.
	if(ARENA_STATE_FIGHTING_WARFINISH == m_DeathMatchState
		&& (3 <= pDateTime->GetTimeDiffTimeInMinutes(m_FinishRoomTime)))
	{
		this->AllUserWarp();
		m_InitDeathMatchRoom = TRUE;

//		// 2007-05-18 by cmkwon, �׽�Ʈ �α�
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENA] DMRoomEnd(4-7) - DRoomNum(%4d) AState(%30s) BTeamNum(%4d) ATeamNum(%4d) MapInfo(%s), �Ʒ��� �� ���ᰡ �ǰ� 3���� ���� ��� ������ ������ ������Ű�� ���� �ʱ�ȭ �Ѵ�.\r\n" 
//				, m_DeathMatchRoomNum, GET_ARENA_STATE_STRING(m_DeathMatchState), m_VCNTeamNum, m_ANITeamNum, GET_MAP_STRING(m_map));
		return;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::WarStart()
/// \brief		1�а� ��� ���°� ������ ���� ���� ���¸� �˸���.
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::WarStart()
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, �޼��� �����
	INIT_MSG_WITH_BUFFER(MSG_FC_ARENA_ROOM_WAR_START, T_FC_ARENA_ROOM_WAR_START, pArenaRoomWarStart, SendBuf);
	pArenaRoomWarStart->ArenaMode		= ARENAMODE_DEATHMATCH;
	pArenaRoomWarStart->ArenaState		= ARENA_STATE_FIGHTING_WARING;
	pArenaRoomWarStart->Time.GetCurrentDateTime();

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, ���� ��ο��� ���� ����
//	mtvectorClientIndex_t *pmtVectCliIdx =  GetVectByInfo(VCN);
	Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARREADY, &m_mtvectVCNTeam, SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_START), ARENA_STATE_FIGHTING_WARING);
	Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARREADY, &m_mtvectANITeam, SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_START), ARENA_STATE_FIGHTING_WARING);
	Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARREADY, &m_mtvectWatchUserList, SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_START), ARENA_STATE_FIGHTING_WARING);	// 2007-06-18 by dhjin, ����

//	mtvectorClientIndex_t::iterator itrA = m_mtvectVCNTeam.begin();
//	while(itrA != m_mtvectVCNTeam.end())
//	{
//		CFieldIOCPSocket *tm_pFieldIOCPSocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrA);
//		// 2007-05-03 by dhjin, ������ ��ȿ���� üũ�Ѵ�.
//		if(!CheckCFieldIOCPSocketValid(tm_pFieldIOCPSocket, ARENA_STATE_WARREADY))
//		{
//			itrA = m_mtvectVCNTeam.erase(itrA);
//			continue;
//		}
//		tm_pFieldIOCPSocket->m_ArenaInfo.State	=	ARENA_STATE_WARING;
//		tm_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_START));
//		itrA++;
//	}
//	mtvectorClientIndex_t::iterator itrB = m_mtvectANITeam.begin();
//	while(itrB != m_mtvectANITeam.end())
//	{
//		CFieldIOCPSocket *tm_pFieldIOCPSocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrB);
//		if(!CheckCFieldIOCPSocketValid(tm_pFieldIOCPSocket, ARENA_STATE_WARREADY))
//		{
//			itrB = m_mtvectANITeam.erase(itrB);
//			continue;
//		}
//		tm_pFieldIOCPSocket->m_ArenaInfo.State	=	ARENA_STATE_WARING;
//		tm_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_START));
//		itrB++;
//	}		
	// 2012-06-14 by jhseol, �Ʒ��� �߰�����part2 - ���� : ���۽� ���� ä���
#ifdef S_ARENA_EX_1ST_JHSEOL
	{
		mt_auto_lock mtA(&m_mtvectVCNTeam);			// 2012-09-24 by jhseol, �ҽ����� - Lock ����
	mtvectorClientIndex_t::iterator itrA = m_mtvectVCNTeam.begin();
	while(itrA != m_mtvectVCNTeam.end())
	{
		CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrA);
		if(NULL == tm_pFISocket)
		{
			itrA++;
			continue;
		}
		tm_pFISocket->m_character.CurrentHP = tm_pFISocket->m_character.HP;
		tm_pFISocket->m_character.CurrentDP = tm_pFISocket->m_character.DP;
		tm_pFISocket->m_character.CurrentSP = tm_pFISocket->m_character.SP;
		tm_pFISocket->m_character.CurrentEP = tm_pFISocket->m_character.EP;
		tm_pFISocket->SendCharacterInfo(T_FC_CHARACTER_CHANGE_CURRENTHPDPSPEP);
		itrA++;
	}	
	}
	
	{
		mt_auto_lock mtA(&m_mtvectANITeam);			// 2012-09-24 by jhseol, �ҽ����� - Lock ����
	mtvectorClientIndex_t::iterator itrB = m_mtvectANITeam.begin();
	while(itrB != m_mtvectANITeam.end())
	{
		CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrB);
		if(NULL == tm_pFISocket)
		{
			itrB++;
			continue;
		}
		tm_pFISocket->m_character.CurrentHP = tm_pFISocket->m_character.HP;
		tm_pFISocket->m_character.CurrentDP = tm_pFISocket->m_character.DP;
		tm_pFISocket->m_character.CurrentSP = tm_pFISocket->m_character.SP;
		tm_pFISocket->m_character.CurrentEP = tm_pFISocket->m_character.EP;
		tm_pFISocket->SendCharacterInfo(T_FC_CHARACTER_CHANGE_CURRENTHPDPSPEP);
		itrB++;
		}
	}
#endif
	// end 2012-06-14 by jhseol, �Ʒ��� �߰�����part2 - ���� : ���۽� ���� ä���

	m_DeathMatchState	= ARENA_STATE_FIGHTING_WARING;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::DecreaseDeathCount(BYTE i_nInfl)
/// \brief		������ġ ����Ʈ�� �����Ѵ�.
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::DecreaseDeathCount(BYTE i_nInfl)
{
	///////////////////////////////////////////////////////////////////////////////
	// 2007-06-01 by cmkwon, �̹� ����� ������
	if(this->CheckWinCondition())
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, DeathCount ���� �� �� �����Ͽ� ����
	if(INFLUENCE_TYPE_VCN == i_nInfl)
	{
		m_ANIDeathCount--;
	}
	else
	{
		m_VCNDeathCount--;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, �޼��� �����
	INIT_MSG_WITH_BUFFER(MSG_FC_ARENA_ROOM_WAR_INFO, T_FC_ARENA_ROOM_WAR_INFO, pArenaRoomWarInfo, SendBuf);
	pArenaRoomWarInfo->ArenaMode		= ARENAMODE_DEATHMATCH;
	pArenaRoomWarInfo->VCNDeathCount	= m_VCNDeathCount;
	pArenaRoomWarInfo->ANIDeathCount	= m_ANIDeathCount;
//	pArenaRoomWarInfo->Time.SetCurrentDateTime();

	Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &m_mtvectVCNTeam, SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_INFO));
	Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &m_mtvectANITeam, SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_INFO));
	Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &m_mtvectWatchUserList, SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_INFO)); // 2007-06-18 by dhjin, ����
//	mtvectorClientIndex_t::iterator itrA = m_mtvectVCNTeam.begin();
//	while(itrA != m_mtvectVCNTeam.end())
//	{
//		CFieldIOCPSocket * tm_pFieldIOCPSocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrA);
//		if(!CheckCFieldIOCPSocketValid(tm_pFieldIOCPSocket, ARENA_STATE_WARING))
//		{
//			itrA = m_mtvectVCNTeam.erase(itrA);
//			continue;
//		}
//		tm_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_INFO));
//		itrA++;
//	}
//	mtvectorClientIndex_t::iterator itrB = m_mtvectANITeam.begin();
//	while(itrB != m_mtvectANITeam.end())
//	{
//		CFieldIOCPSocket * tm_pFieldIOCPSocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrB);
//		if(!CheckCFieldIOCPSocketValid(tm_pFieldIOCPSocket, ARENA_STATE_WARING))
//		{
//			itrB = m_mtvectANITeam.erase(itrB);
//			continue;
//		}
//		tm_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_INFO));
//		itrB++;
//	}
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-25 by dhjin, ���� �� ������ġ ����Ʈ�� 0�̶�� ������ �����Ѵ�
	if(this->CheckWinCondition())
	{
		this->WarFinish(ARENA_END_TYPE_POINT);
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CArenaDeathMatchRoom::CheckWinCondition()
/// \brief		��������Ʈ�� 0���� üũ
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CArenaDeathMatchRoom::CheckWinCondition()
{
	if(0 >= m_VCNDeathCount
		|| 0 >= m_ANIDeathCount)
	{
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::BeforeWarFinish()
/// \brief		��Ⱑ ���� �Ǳ� 1������ Ŭ���̾�Ʈ���� ������ �����Ѵ�.
/// \author		dhjin
/// \date		2007-04-30 ~ 2007-04-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::BeforeWarFinish()
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin,
	Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &m_mtvectVCNTeam, T_FC_ARENA_BEFORE_ROOM_WAR_FINISH);
	Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &m_mtvectANITeam, T_FC_ARENA_BEFORE_ROOM_WAR_FINISH);
	Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &m_mtvectWatchUserList, T_FC_ARENA_BEFORE_ROOM_WAR_FINISH); // 2007-06-18 by dhjin, ����

//	mtvectorClientIndex_t::iterator itrA = m_mtvectVCNTeam.begin();
//	while(itrA != m_mtvectVCNTeam.end())
//	{
//		CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrA);
//		if(!CheckCFieldIOCPSocketValid(tm_pFISocket, ARENA_STATE_WARING))
//		{
//			itrA = m_mtvectVCNTeam.erase(itrA);
//			continue;
//		}
//		tm_pFISocket->SendAddMessageType(T_FC_ARENA_BEFORE_ROOM_WAR_FINISH);
//		itrA++;
//	}	
//	mtvectorClientIndex_t::iterator itrB = m_mtvectANITeam.begin();
//	while(itrB != m_mtvectANITeam.end())
//	{
//		CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrB);
//		if(!CheckCFieldIOCPSocketValid(tm_pFISocket, ARENA_STATE_WARING))
//		{
//			itrB = m_mtvectANITeam.erase(itrB);
//			continue;
//		}
//		tm_pFISocket->SendAddMessageType(T_FC_ARENA_BEFORE_ROOM_WAR_FINISH);
//		itrB++;
//	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::WarFinish(BYTE i_byEndType)
/// \brief		���� ����� ���� �� ���� ����, ���а� ���� �Ǿ��� ��
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::WarFinish(BYTE i_byEndType)
{
	int			nSendBytes = 0;
	BYTE		tmVCNWatchIssue = 0;	// 2007-06-20 by dhjin
	BYTE		tmANIWatchIssue = 0;	// 2007-06-20 by dhjin
	///////////////////////////////////////////////////////////////////////////////
	// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : �ӽ÷� �ھƵ� ����Ƚ�� ������ [0�� ������ = �⺻���� ������ �ְڴ�]
#ifdef S_ARENA_EX_1ST_JHSEOL
	INT			Arena_Max_Paly_Count = DEFINE_ARENA_MAX_PALY_COUNT;
#else
	INT			Arena_Max_Paly_Count = FALSE;
#endif
	// end 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : �ӽ÷� �ھƵ� ����Ƚ�� ������ [0�� ������ = �⺻���� ������ �ְڴ�]

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-11 by dhjin, iterator �ߺ��� ���ϱ� ���� ���纻�� ����� �޼��� ���� �� ����Ѵ�.
	vectorClientIndex tmVectVCNTeam;
	{
		mt_auto_lock mtA(&m_mtvectVCNTeam);			// 2012-09-24 by jhseol, �ҽ����� - Lock ����
	tmVectVCNTeam.clear();
	tmVectVCNTeam.assign(m_mtvectVCNTeam.begin(), m_mtvectVCNTeam.end());
	}

	vectorClientIndex tmVectANITeam;
	{
		mt_auto_lock mtB(&m_mtvectANITeam);			// 2012-09-24 by jhseol, �ҽ����� - Lock ����
	tmVectANITeam.clear();
	tmVectANITeam.assign(m_mtvectANITeam.begin(), m_mtvectANITeam.end());
	}

	vectorClientIndex tmVectWatchUser;		// 2007-06-18 by dhjin, ���� 
	{
		mt_auto_lock mtC(&m_mtvectWatchUserList);			// 2012-09-24 by jhseol, �ҽ����� - Lock ����
	tmVectWatchUser.clear();
	tmVectWatchUser.assign(m_mtvectWatchUserList.begin(), m_mtvectWatchUserList.end());
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-24 by dhjin, ���� ���� �°� �޼��� ���� �ۼ��ϰ� ������
	INIT_MSG_WITH_BUFFER(MSG_FC_ARENA_ROOM_WAR_FINISH_HEADER, T_FC_ARENA_ROOM_WAR_FINISH_HEADER, pSWarFinInfoHeader, SendBuf);
	pSWarFinInfoHeader->ArenaMode			= ARENAMODE_DEATHMATCH;
	pSWarFinInfoHeader->ArenaState			= ARENA_STATE_FIGHTING_WARFINISH;
	pSWarFinInfoHeader->FinishTime.GetCurrentDateTime();
	nSendBytes = MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_FINISH_HEADER);		
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-05-08 by dhjin, �켱 ���������� ���� ���� �����Ѵ�.
	mtvectorClientIndex_t *pmtVectCliIdx = GetVectByInfl(INFLUENCE_TYPE_VCN);

	{
		mt_auto_lock mtD(pmtVectCliIdx);		// 2012-09-24 by jhseol, �ҽ����� - Lock ����

	mtvectorClientIndex_t::iterator itr = pmtVectCliIdx->begin();
	while(itr != pmtVectCliIdx->end())
	{
		CFieldIOCPSocket *pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(!CheckCFieldIOCPSocketValid(pFISocket, ARENA_STATE_FIGHTING_WARING))
		{
//			this->SendWatchRemoveUser(*itr);		// 2007-06-18 by dhjin, �Ʒ������� ���� ���� ���� ����.
			itr = pmtVectCliIdx->erase(itr);
			continue;
		}
		
		// 2007-05-08 by dhjin, �޼��� ����
		if(nSendBytes + MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_FINISH) > SIZE_MAX_PACKET)
		{

			//////////////////////////////////////////////////////////////////////////
			// 2007-05-08 by dhjin, ����Ÿ ���� �� ����...
			Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &tmVectVCNTeam, SendBuf, nSendBytes);
			Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &tmVectANITeam, SendBuf, nSendBytes);
			Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &tmVectWatchUser, SendBuf, nSendBytes);	// 2007-06-18 by dhjin, ����

			nSendBytes					= 0;
		}

		*((MessageType_t*)(SendBuf + nSendBytes))	= T_FC_ARENA_ROOM_WAR_FINISH;
		nSendBytes									+= SIZE_FIELD_TYPE_HEADER;
		MSG_FC_ARENA_ROOM_WAR_FINISH *pSWarFinInfo	= (MSG_FC_ARENA_ROOM_WAR_FINISH*)(SendBuf + nSendBytes);
		pSWarFinInfo->Influence				= pFISocket->m_character.InfluenceType;
		pSWarFinInfo->ShootingDown			= pFISocket->m_ArenaInfo.ShootingDown;
		pSWarFinInfo->SufferingAttack		= pFISocket->m_ArenaInfo.SufferingAttack;
		pSWarFinInfo->LostPoint				= pFISocket->m_ArenaInfo.LostPoint;
		pSWarFinInfo->UnitKind				= pFISocket->m_character.UnitKind;
		// 2012-07-10 by jhseol, �Ʒ��� ���� - ������ ���� �������� �������� ��� ����
/*
		// 2012-06-08 by jhseol, �Ʒ��� �߰�����part2 - HP/DP ����
		if( (float)pFISocket->m_character.HP + (float)pFISocket->m_character.DP != FALSE )
		{
			pSWarFinInfo->Average			= ((float)pFISocket->m_character.CurrentHP + (float)pFISocket->m_character.CurrentDP) / ((float)pFISocket->m_character.HP + (float)pFISocket->m_character.DP);
			pSWarFinInfo->Average			= pSWarFinInfo->Average*100;
		}
		float playercount = 0.0f;
		if(GetVCNTeamSize() > GetANITeamSize() ? playercount = (float)GetVCNTeamSize() : playercount = (float)GetANITeamSize())
		{
			if(FALSE != playercount)
			{
				if(FALSE != (m_fVCNTeamPercent / playercount) + (m_fANITeamPercent / playercount))
				{
					if(pFISocket->m_ArenaInfo.TeamNum == m_VCNTeamNum)
					{
						pSWarFinInfo->TeamAverage = (  (m_fVCNTeamPercent / playercount) / ( (m_fVCNTeamPercent / playercount) + (m_fANITeamPercent / playercount) ) )*100;
					}
					else if(pFISocket->m_ArenaInfo.TeamNum == m_ANITeamNum)
					{
						pSWarFinInfo->TeamAverage = (  (m_fANITeamPercent / playercount) / ( (m_fVCNTeamPercent / playercount) + (m_fANITeamPercent / playercount) ) )*100;
					}
					// 2012-06-08 by jhseol, �Ʒ��� �߰�����part2 - HP/DP ���� �ݿø� �˰���
					if(pSWarFinInfo->TeamAverage > 50)
					{
						pSWarFinInfo->TeamAverage = pSWarFinInfo->TeamAverage + 1;
					}
					// end 2012-06-08 by jhseol, �Ʒ��� �߰�����part2 - HP/DP ���� �ݿø� �˰���
				}
			}
		}
		// end 2012-06-08 by jhseol, �Ʒ��� �߰�����part2 - HP/DP ����
*/
		// end 2012-07-10 by jhseol, �Ʒ��� ���� - ������ ���� �������� �������� ��� ����
		// 2012-07-10 by jhseol, �Ʒ��� ���� - ������ ���� ���� ������
		if(pFISocket->m_ArenaInfo.TeamNum == m_VCNTeamNum)
		{
			pSWarFinInfo->TeamAverage = m_fVCNTeamPercent;
		}
		else if(pFISocket->m_ArenaInfo.TeamNum == m_ANITeamNum)
		{
			pSWarFinInfo->TeamAverage = m_fANITeamPercent;
		}
		// end 2012-07-10 by jhseol, �Ʒ��� ���� - ������ ���� ���� ������
		util::strncpy(pSWarFinInfo->CharacterLevelRank, pFISocket->GetCharacterLevelRank(), SIZE_MAX_LEVELRANK);
		util::strncpy(pSWarFinInfo->CharacterName, pFISocket->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
		nSendBytes									+= sizeof(MSG_FC_ARENA_ROOM_WAR_FINISH);
		itr++;
	}
	}

	if(nSendBytes > 0)
	{
		//////////////////////////////////////////////////////////////////////////
		// 2007-05-08 by dhjin, ����Ÿ ���� �� ����...
		Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &tmVectVCNTeam, SendBuf, nSendBytes);
		Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &tmVectANITeam, SendBuf, nSendBytes);
		Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &tmVectWatchUser, SendBuf, nSendBytes);	// 2007-06-18 by dhjin, ����

		nSendBytes					= 0;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-08 by dhjin, �˸��� ���� �����Ѵ�.
	pmtVectCliIdx = GetVectByInfl(INFLUENCE_TYPE_ANI);

	{
		mt_auto_lock mtE(pmtVectCliIdx);		// 2012-09-24 by jhseol, �ҽ����� - Lock ����

		mtvectorClientIndex_t::iterator itr = pmtVectCliIdx->begin();
	while(itr != pmtVectCliIdx->end())
	{
		CFieldIOCPSocket *pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(!CheckCFieldIOCPSocketValid(pFISocket, ARENA_STATE_FIGHTING_WARING))
		{
//			this->SendWatchRemoveUser(*itr);		// 2007-06-18 by dhjin, �Ʒ������� ���� ���� ���� ����.
			itr = pmtVectCliIdx->erase(itr);
			continue;
		}
		
		if(nSendBytes + MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_FINISH) > SIZE_MAX_PACKET)
		{
			//////////////////////////////////////////////////////////////////////////
			// 2007-05-08 by dhjin, ����Ÿ ���� �� ����...
			Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &tmVectVCNTeam, SendBuf, nSendBytes);
			Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &tmVectANITeam, SendBuf, nSendBytes);
			Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &tmVectWatchUser, SendBuf, nSendBytes);	// 2007-06-18 by dhjin, ����
			nSendBytes					= 0;
		}

		*((MessageType_t*)(SendBuf + nSendBytes))	= T_FC_ARENA_ROOM_WAR_FINISH;
		nSendBytes									+= SIZE_FIELD_TYPE_HEADER;
		MSG_FC_ARENA_ROOM_WAR_FINISH *pSWarFinInfo	= (MSG_FC_ARENA_ROOM_WAR_FINISH*)(SendBuf + nSendBytes);
		pSWarFinInfo->Influence				= pFISocket->m_character.InfluenceType;
		pSWarFinInfo->ShootingDown			= pFISocket->m_ArenaInfo.ShootingDown;
		pSWarFinInfo->SufferingAttack		= pFISocket->m_ArenaInfo.SufferingAttack;
		pSWarFinInfo->LostPoint				= pFISocket->m_ArenaInfo.LostPoint;
		pSWarFinInfo->UnitKind				= pFISocket->m_character.UnitKind;
		// 2012-07-10 by jhseol, �Ʒ��� ���� - ������ ���� �������� �������� ��� ����
/*
		// 2012-06-08 by jhseol, �Ʒ��� �߰�����part2 - HP/DP ����
		if( (float)pFISocket->m_character.HP + (float)pFISocket->m_character.DP != FALSE )
		{
			pSWarFinInfo->Average			= ((float)pFISocket->m_character.CurrentHP + (float)pFISocket->m_character.CurrentDP) / ((float)pFISocket->m_character.HP + (float)pFISocket->m_character.DP);
			pSWarFinInfo->Average			= pSWarFinInfo->Average*100;
		}
		float playercount = 0.0f;
		if(GetVCNTeamSize() > GetANITeamSize() ? playercount = (float)GetVCNTeamSize() : playercount = (float)GetANITeamSize())
		{
			if(FALSE != playercount)
			{
				if(FALSE != (m_fVCNTeamPercent / playercount) + (m_fANITeamPercent / playercount))
				{
					if(pFISocket->m_ArenaInfo.TeamNum == m_VCNTeamNum)
					{
						pSWarFinInfo->TeamAverage = (  (m_fVCNTeamPercent / playercount) / ( (m_fVCNTeamPercent / playercount) + (m_fANITeamPercent / playercount) ) )*100;
					}
					else if(pFISocket->m_ArenaInfo.TeamNum == m_ANITeamNum)
					{
						pSWarFinInfo->TeamAverage = (  (m_fANITeamPercent / playercount) / ( (m_fVCNTeamPercent / playercount) + (m_fANITeamPercent / playercount) ) )*100;
					}
					// 2012-06-08 by jhseol, �Ʒ��� �߰�����part2 - HP/DP ���� �ݿø� �˰���
					if(pSWarFinInfo->TeamAverage > 50)
					{
						pSWarFinInfo->TeamAverage = pSWarFinInfo->TeamAverage + 1;
					}
					// end 2012-06-08 by jhseol, �Ʒ��� �߰�����part2 - HP/DP ���� �ݿø� �˰���
				}
			}
		}
		// end 2012-06-08 by jhseol, �Ʒ��� �߰�����part2 - HP/DP ����
*/
		// end 2012-07-10 by jhseol, �Ʒ��� ���� - ������ ���� �������� �������� ��� ����
		// 2012-07-10 by jhseol, �Ʒ��� ���� - ������ ���� ���� ������
		if(pFISocket->m_ArenaInfo.TeamNum == m_VCNTeamNum)
		{
			pSWarFinInfo->TeamAverage = m_fVCNTeamPercent;
		}
		else if(pFISocket->m_ArenaInfo.TeamNum == m_ANITeamNum)
		{
			pSWarFinInfo->TeamAverage = m_fANITeamPercent;
		}
		// end 2012-07-10 by jhseol, �Ʒ��� ���� - ������ ���� ���� ������
		util::strncpy(pSWarFinInfo->CharacterLevelRank, pFISocket->GetCharacterLevelRank(), SIZE_MAX_LEVELRANK);
		util::strncpy(pSWarFinInfo->CharacterName, pFISocket->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
		nSendBytes									+= sizeof(MSG_FC_ARENA_ROOM_WAR_FINISH);
		itr++;
		}
	}

	if(nSendBytes > 0)
	{
		//////////////////////////////////////////////////////////////////////////
		// 2007-05-08 by dhjin, ����Ÿ ���� �� ����...
		Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &tmVectVCNTeam, SendBuf, nSendBytes);
		Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &tmVectANITeam, SendBuf, nSendBytes);
		Send2ArenaDeathRoom(ARENA_STATE_FIGHTING_WARING, &tmVectWatchUser, SendBuf, nSendBytes);	// 2007-06-18 by dhjin, ����

		nSendBytes					= 0;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-08 by dhjin, T_FC_ARENA_ROOM_WAR_FINISH_DONE ����, ����������
	INIT_MSG(MSG_FC_ARENA_ROOM_WAR_FINISH_DONE, T_FC_ARENA_ROOM_WAR_FINISH_DONE, pSWarFinInfoDoneVCN, SendBuf);
	pSWarFinInfoDoneVCN->EndState				= i_byEndType;
	if(0 < m_VCNDeathCount)
	{
		pSWarFinInfoDoneVCN->Issue				= ARENA_ISSUE_LOSE;
		pSWarFinInfoDoneVCN->InflPoint			= m_PayInfo.PayInfluencePointLOSE;
		pSWarFinInfoDoneVCN->WarPoint			= m_PayInfo.PayWarPointLOSE;
// 2008-01-10 by dhjin, �Ʒ��� ���� - �ʵ� ������ ���� ���� ����
//		this->m_pFieldIOCP17->m_InflWarManager.AddContributionPoint(INFLUENCE_TYPE_VCN, m_PayInfo.PayInfluencePointLOSE);
		this->m_pFieldIOCP17->SendArenaServerPay(GetVCNMFSInfl(), m_PayInfo.PayInfluencePointLOSE);
	}
	else
	{
		pSWarFinInfoDoneVCN->Issue				= ARENA_ISSUE_WIN;
		pSWarFinInfoDoneVCN->InflPoint			= m_PayInfo.PayInfluencePointWIN;
		pSWarFinInfoDoneVCN->WarPoint			= m_PayInfo.PayWarPointWIN;
// 2008-01-10 by dhjin, �Ʒ��� ���� - �ʵ� ������ ���� ���� ����
//		this->m_pFieldIOCP17->m_InflWarManager.AddContributionPoint(INFLUENCE_TYPE_VCN, m_PayInfo.PayInfluencePointWIN);
		this->m_pFieldIOCP17->SendArenaServerPay(GetVCNMFSInfl(), m_PayInfo.PayInfluencePointWIN);
	}
	tmVCNWatchIssue = pSWarFinInfoDoneVCN->Issue;			// 2007-06-20 by dhjin, ���������� �����ڿ� ���� ����

	// 2007-07-11 by dhjin, WarPoint �⺻��
	INT	VCNBaseWarPoint	= pSWarFinInfoDoneVCN->WarPoint;

	pmtVectCliIdx = GetVectByInfl(INFLUENCE_TYPE_VCN);
	{
		mt_auto_lock mtD(pmtVectCliIdx);		// 2012-09-24 by jhseol, �ҽ����� - Lock ����
		
		mtvectorClientIndex_t::iterator itr = pmtVectCliIdx->begin();
	while(itr != pmtVectCliIdx->end())
	{
		CFieldIOCPSocket *pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(NULL == pFISocket)
		{
			continue;
		}
		if(!CheckCFieldIOCPSocketValid(pFISocket, ARENA_STATE_FIGHTING_WARING))
		{
//			this->SendWatchRemoveUser(*itr);		// 2007-06-18 by dhjin, �Ʒ������� ���� ���� ���� ����.
			itr = pmtVectCliIdx->erase(itr);
			continue;
		}	
		pFISocket->m_ArenaInfo.State	= ARENA_STATE_FIGHTING_WARFINISH;


		//////////////////////////////////////////////////////////////////////////////
		// 2012-05-23 by jhseol, �Ʒ��� �߰����� - ����Ƚ�� �̻��̸� �⺻ ���� ������ �����Ѵ�.
		INT PlusWarPoint = 0;
		if(pFISocket->m_character.ArenaPlayInfo.nPlayCount > Arena_Max_Paly_Count || pFISocket->m_character.ArenaPlayInfo.nPlayCount <= FALSE || Arena_Max_Paly_Count == FALSE)
		{
			if(ARENA_ISSUE_WIN == pSWarFinInfoDoneVCN->Issue)
			{
				PlusWarPoint = pFISocket->CalcWarPoint(m_PayInfo.DefaultPayWarPointWIN, TRUE); // 2012-05-23 by jhseol, �Ʒ��� �߰����� - ����Ƚ�� �ʰ� ������ �⺻ ���޷����� ����
			}
			else
			{
				PlusWarPoint = pFISocket->CalcWarPoint(m_PayInfo.DefaultPayWarPointLOSE, TRUE); // 2012-05-23 by jhseol, �Ʒ��� �߰����� - ����Ƚ�� �ʰ� ������ �⺻ ���޷����� ����
			}
		}
		else
		{
			PlusWarPoint = pFISocket->CalcWarPoint(VCNBaseWarPoint, TRUE);
		}
		// end 2012-05-23 by jhseol, �Ʒ��� �߰����� - ����Ƚ�� �̻��̸� �⺻ ���� ������ �����Ѵ�.


		// 2007-08-13 by cmkwon, WarPoint�� ����뿡 �����ϱ�(����,�������� ����) - �Ʒ��� VCN ó��
//		float	fApplyRate		= 0.0f;
//		BOOL	bRestriction	= pFISocket->GPGetApplyRateByGamePlayingTime(&fApplyRate);
//		if(bRestriction)
//		{
//			if(0.0f >= fApplyRate)
//			{
//				PlusWarPoint = 0;
//			}
//			else
//			{
//				PlusWarPoint = PlusWarPoint * fApplyRate;
//			}
//		}
//		
//		pFISocket->m_character.WarPoint += PlusWarPoint;
//		pFISocket->m_character.CumulativeWarPoint += PlusWarPoint;
		pSWarFinInfoDoneVCN->WarPoint	= PlusWarPoint;

		//////////////////////////////////////////////////////////////////////////
		// 2007-06-07 by dhjin, ���� �����.
		if(ARENA_ISSUE_WIN == pSWarFinInfoDoneVCN->Issue)
		{
			pFISocket->m_character.ArenaWin++;
		}
		else
		{
			pFISocket->m_character.ArenaLose++;
		}

		// 2009-01-22 by cmkwon, ���� ���� ���� �α� ���� - CAtumLogSender::SendLogMessageGetWarPoint()�� ����(, BYTE i_byWPUpdateTy) �߰�
		CAtumLogSender::SendLogMessageGetWarPoint(pFISocket, PlusWarPoint, pFISocket->m_character.WarPoint, pFISocket->m_character.CumulativeWarPoint, WPUT_ARENA);
//////////////////////////////////////////////////////////////////////////
// 2008-01-10 by dhjin,  �Ʒ��� ���� - �ʵ� �������� ���۹޾� ó��
//		QPARAM_UPDATE_ARENA_RESULT *pQparam = new QPARAM_UPDATE_ARENA_RESULT;
//		pQparam->AccountUID			= pFISocket->m_character.AccountUniqueNumber;
//		pQparam->CharacterUID		= pFISocket->m_character.CharacterUniqueNumber;
//		pQparam->ArenaWin			= pFISocket->m_character.ArenaWin;
//		pQparam->ArenaLose			= pFISocket->m_character.ArenaLose;
//		pQparam->TotalWarPoint		= pFISocket->m_character.WarPoint;
//		pQparam->CumulativeWarPoint = pFISocket->m_character.CumulativeWarPoint;
//		this->m_pFieldIOCP17->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateArenaResult, pFISocket, 0, pQparam);
		// 2008-01-10 by dhjin, �Ʒ��� ���� - �ʵ� �������� ����
		pFISocket->SendArenaCharacterPay(pSWarFinInfoDoneVCN->Issue, PlusWarPoint);
		
		pFISocket->SendUserInfoForArenaEnding();		// 2011-10-17 by hskim, ��Ʈ�� �ý��� 2��
		pFISocket->InitUserInfoForArenaEnding();

		pSWarFinInfoDoneVCN->TotalWarPoint		= pFISocket->m_character.WarPoint;
		pSWarFinInfoDoneVCN->CumulativeWarPoint = pFISocket->m_character.CumulativeWarPoint;
		pSWarFinInfoDoneVCN->ArenaWin			= pFISocket->m_character.ArenaWin;
		pSWarFinInfoDoneVCN->ArenaLose			= pFISocket->m_character.ArenaLose;
		pFISocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_FINISH_DONE));
		itr++;
	}
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-08 by dhjin, T_FC_ARENA_ROOM_WAR_FINISH_DONE ����, �˸���
	INIT_MSG(MSG_FC_ARENA_ROOM_WAR_FINISH_DONE, T_FC_ARENA_ROOM_WAR_FINISH_DONE, pSWarFinInfoDoneANI, SendBuf);
	pSWarFinInfoDoneANI->EndState				= i_byEndType;
	if(0 < m_ANIDeathCount)
	{
		pSWarFinInfoDoneANI->Issue				= ARENA_ISSUE_LOSE;
		pSWarFinInfoDoneANI->InflPoint			= m_PayInfo.PayInfluencePointLOSE;
		pSWarFinInfoDoneANI->WarPoint			= m_PayInfo.PayWarPointLOSE;
// 2008-01-10 by dhjin, �Ʒ��� ���� - �ʵ� ������ ���� ���� ����
//		this->m_pFieldIOCP17->m_InflWarManager.AddContributionPoint(INFLUENCE_TYPE_ANI, m_PayInfo.PayInfluencePointLOSE);
		this->m_pFieldIOCP17->SendArenaServerPay(GetANIMFSInfl(), m_PayInfo.PayInfluencePointLOSE);
	}
	else
	{
		pSWarFinInfoDoneANI->Issue				= ARENA_ISSUE_WIN;
		pSWarFinInfoDoneANI->InflPoint			= m_PayInfo.PayInfluencePointWIN;
		pSWarFinInfoDoneANI->WarPoint			= m_PayInfo.PayWarPointWIN;
// 2008-01-10 by dhjin, �Ʒ��� ���� - �ʵ� ������ ���� ���� ����
//		this->m_pFieldIOCP17->m_InflWarManager.AddContributionPoint(INFLUENCE_TYPE_ANI, m_PayInfo.PayInfluencePointWIN);
		this->m_pFieldIOCP17->SendArenaServerPay(GetANIMFSInfl(), m_PayInfo.PayInfluencePointWIN);
	}
	tmANIWatchIssue = pSWarFinInfoDoneANI->Issue;			// 2007-06-20 by dhjin, �˸��� �����ڿ� ���� ����

	// 2007-07-11 by dhjin, WarPoint �⺻��
	INT	ANIBaseWarPoint	= pSWarFinInfoDoneANI->WarPoint;

	pmtVectCliIdx = GetVectByInfl(INFLUENCE_TYPE_ANI);
	{
		mt_auto_lock mtD(pmtVectCliIdx);		// 2012-09-24 by jhseol, �ҽ����� - Lock ����
		
		mtvectorClientIndex_t::iterator itr = pmtVectCliIdx->begin();
	while(itr != pmtVectCliIdx->end())
	{
		CFieldIOCPSocket *pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(NULL == pFISocket)
		{
			continue;
		}
		if(!CheckCFieldIOCPSocketValid(pFISocket, ARENA_STATE_FIGHTING_WARING))
		{
//			this->SendWatchRemoveUser(*itr);		// 2007-06-18 by dhjin, �Ʒ������� ���� ���� ���� ����.
			itr = pmtVectCliIdx->erase(itr);
			continue;
		}	
		pFISocket->m_ArenaInfo.State	= ARENA_STATE_FIGHTING_WARFINISH;


		//////////////////////////////////////////////////////////////////////////////
		// 2012-05-23 by jhseol, �Ʒ��� �߰����� - ����Ƚ�� �̻��̸� �⺻ ���� ������ �����Ѵ�.
		INT PlusWarPoint = 0;
		if(pFISocket->m_character.ArenaPlayInfo.nPlayCount > Arena_Max_Paly_Count || pFISocket->m_character.ArenaPlayInfo.nPlayCount <= FALSE || Arena_Max_Paly_Count == FALSE)
		{
			if(ARENA_ISSUE_WIN == pSWarFinInfoDoneANI->Issue)
			{
				PlusWarPoint = pFISocket->CalcWarPoint(m_PayInfo.DefaultPayWarPointWIN, TRUE); // 2012-05-23 by jhseol, �Ʒ��� �߰����� - ����Ƚ�� �ʰ� ������ �⺻ ���޷����� ����
			}
			else
			{
				PlusWarPoint = pFISocket->CalcWarPoint(m_PayInfo.DefaultPayWarPointLOSE, TRUE); // 2012-05-23 by jhseol, �Ʒ��� �߰����� - ����Ƚ�� �ʰ� ������ �⺻ ���޷����� ����
			}
		}
		else
		{
			PlusWarPoint = pFISocket->CalcWarPoint(ANIBaseWarPoint, TRUE);
		}
		// end 2012-05-23 by jhseol, �Ʒ��� �߰����� - ����Ƚ�� �̻��̸� �⺻ ���� ������ �����Ѵ�.

		// 2007-08-13 by cmkwon, WarPoint�� ����뿡 �����ϱ�(����,�������� ����) - �Ʒ��� ANI ó��
//		float	fApplyRate		= 0.0f;
//		BOOL	bRestriction	= pFISocket->GPGetApplyRateByGamePlayingTime(&fApplyRate);
//		if(bRestriction)
//		{
//			if(0.0f >= fApplyRate)
//			{
//				PlusWarPoint = 0;
//			}
//			else
//			{
//				PlusWarPoint = PlusWarPoint * fApplyRate;
//			}
//		}
//		
//		pFISocket->m_character.WarPoint += PlusWarPoint;
//		pFISocket->m_character.CumulativeWarPoint += PlusWarPoint;
		pSWarFinInfoDoneANI->WarPoint	= PlusWarPoint;

		//////////////////////////////////////////////////////////////////////////
		// 2007-06-07 by dhjin, ���� �����.
		if(ARENA_ISSUE_WIN == pSWarFinInfoDoneANI->Issue)
		{
			pFISocket->m_character.ArenaWin++;
		}
		else
		{
			pFISocket->m_character.ArenaLose++;
		}

		// 2009-01-22 by cmkwon, ���� ���� ���� �α� ���� - CAtumLogSender::SendLogMessageGetWarPoint()�� ����(, BYTE i_byWPUpdateTy) �߰�
		CAtumLogSender::SendLogMessageGetWarPoint(pFISocket, PlusWarPoint, pFISocket->m_character.WarPoint, pFISocket->m_character.CumulativeWarPoint, WPUT_ARENA);
//////////////////////////////////////////////////////////////////////////
// 2008-01-10 by dhjin,  �Ʒ��� ���� - �ʵ� �������� ���۹޾� ó��
//		QPARAM_UPDATE_ARENA_RESULT *pQparam = new QPARAM_UPDATE_ARENA_RESULT;
//		pQparam->AccountUID			= pFISocket->m_character.AccountUniqueNumber;
//		pQparam->CharacterUID		= pFISocket->m_character.CharacterUniqueNumber;
//		pQparam->ArenaWin			= pFISocket->m_character.ArenaWin;
//		pQparam->ArenaLose			= pFISocket->m_character.ArenaLose;
//		pQparam->TotalWarPoint		= pFISocket->m_character.WarPoint;
//		pQparam->CumulativeWarPoint = pFISocket->m_character.CumulativeWarPoint;
//		this->m_pFieldIOCP17->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateArenaResult, pFISocket, 0, pQparam);
		// 2008-01-10 by dhjin, �Ʒ��� ���� - �ʵ� �������� ����
		pFISocket->SendArenaCharacterPay(pSWarFinInfoDoneANI->Issue, PlusWarPoint);
		
		pFISocket->SendUserInfoForArenaEnding();		// 2011-10-17 by hskim, ��Ʈ�� �ý��� 2��
		pFISocket->InitUserInfoForArenaEnding();

		pSWarFinInfoDoneANI->TotalWarPoint		= pFISocket->m_character.WarPoint;
		pSWarFinInfoDoneANI->CumulativeWarPoint = pFISocket->m_character.CumulativeWarPoint;
		pSWarFinInfoDoneANI->ArenaWin			= pFISocket->m_character.ArenaWin;
		pSWarFinInfoDoneANI->ArenaLose			= pFISocket->m_character.ArenaLose;
		pFISocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_FINISH_DONE));
		itr++;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-06-18 by dhjin, ������ ����. �����ڴ� ���п��δ� ������ �ʴ´�.
	INIT_MSG(MSG_FC_ARENA_ROOM_WAR_FINISH_DONE, T_FC_ARENA_ROOM_WAR_FINISH_DONE, pSWarFinInfoDoneWatch, SendBuf);
	pSWarFinInfoDoneWatch->EndState				= i_byEndType;
	pSWarFinInfoDoneWatch->InflPoint			= 0;
	pSWarFinInfoDoneWatch->WarPoint				= 0;

	mtvectorClientIndex_t::iterator itrW = tmVectWatchUser.begin();
	for(; itrW != tmVectWatchUser.end(); itrW++)
	{
		CFieldIOCPSocket *pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrW);
		if(!CheckCFieldIOCPSocketValid(pFISocket, ARENA_STATE_FIGHTING_WARING))
		{
			continue;
		}

		if(COMPARE_INFLUENCE(pFISocket->m_character.InfluenceType, INFLUENCE_TYPE_VCN))
		{
			pSWarFinInfoDoneWatch->Issue				= tmVCNWatchIssue;	
		}
		else
		{
			pSWarFinInfoDoneWatch->Issue				= tmANIWatchIssue;
		}
		
		pFISocket->m_ArenaInfo.State	= ARENA_STATE_FIGHTING_WARFINISH;
		pSWarFinInfoDoneANI->TotalWarPoint		= pFISocket->m_character.WarPoint;
		pSWarFinInfoDoneANI->CumulativeWarPoint = pFISocket->m_character.CumulativeWarPoint;
		pSWarFinInfoDoneANI->ArenaWin			= pFISocket->m_character.ArenaWin;
		pSWarFinInfoDoneANI->ArenaLose			= pFISocket->m_character.ArenaLose;
		pFISocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_FINISH_DONE));
	}

	m_FinishRoomTime.SetCurrentDateTime();	
	m_DeathMatchState	= ARENA_STATE_FIGHTING_WARFINISH;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::WarFinishDraw(BYTE i_byEndType)
/// \brief		���� ����, ���º� �� �� 
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::WarFinishDraw(BYTE i_byEndType)
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, �޼��� �����
	INIT_MSG_WITH_BUFFER(MSG_FC_ARENA_ROOM_WAR_FINISH_DRAW, T_FC_ARENA_ROOM_WAR_FINISH_DRAW, pSArenaRoomWarFinishList, SendBuf);
	pSArenaRoomWarFinishList->ArenaMode			= ARENAMODE_DEATHMATCH;
	pSArenaRoomWarFinishList->ArenaState		= ARENA_STATE_FIGHTING_WARFINISH;
	pSArenaRoomWarFinishList->FinishTime.GetCurrentDateTime();
	pSArenaRoomWarFinishList->Issue				= ARENA_ISSUE_DRAW;
	pSArenaRoomWarFinishList->EndState			= i_byEndType;
	pSArenaRoomWarFinishList->InflPoint			= 0;
	pSArenaRoomWarFinishList->WarPoint			= 0;

	if(ARENA_STATE_FIGHTING_WARREADY == m_DeathMatchState
		|| ARENA_STATE_FIGHTING_WARING == m_DeathMatchState)
	{
		Send2ArenaDeathRoom(m_DeathMatchState, &m_mtvectVCNTeam, SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_FINISH_DRAW), ARENA_STATE_FIGHTING_WARFINISH);
		Send2ArenaDeathRoom(m_DeathMatchState, &m_mtvectANITeam, SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_FINISH_DRAW), ARENA_STATE_FIGHTING_WARFINISH);
		Send2ArenaDeathRoom(m_DeathMatchState, &m_mtvectWatchUserList, SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_FINISH_DRAW), ARENA_STATE_FIGHTING_WARFINISH);
	}
//
//	mtvectorClientIndex_t::iterator itrA = m_mtvectVCNTeam.begin();
//	while(itrA != m_mtvectVCNTeam.end())
//	{
//		CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrA);
//		if(!CheckCFieldIOCPSocketValid(tm_pFISocket, ARENA_STATE_WARING))
//		{
//			itrA = m_mtvectVCNTeam.erase(itrA);
//			continue;
//		}
//		tm_pFISocket->m_ArenaInfo.State	= ARENA_STATE_WARFINISH;
//		tm_pFISocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_FINISH)+pSArenaRoomWarFinishList->ArenaRoomFinishInfoDeathMatchListCount*sizeof(SARENA_ROOM_FINISH_INFO));
//		itrA++;
//	}	
//	mtvectorClientIndex_t::iterator itrB = m_mtvectANITeam.begin();
//	while(itrB != m_mtvectANITeam.end())
//	{
//		CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrB);
//		if(!CheckCFieldIOCPSocketValid(tm_pFISocket, ARENA_STATE_WARING))
//		{
//			itrB = m_mtvectANITeam.erase(itrB);
//			continue;
//		}
//		tm_pFISocket->m_ArenaInfo.State	= ARENA_STATE_WARFINISH;
//		tm_pFISocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ARENA_ROOM_WAR_FINISH)+pSArenaRoomWarFinishList->ArenaRoomFinishInfoDeathMatchListCount*sizeof(SARENA_ROOM_FINISH_INFO));
//		itrB++;
//	}

	// 2008-01-16 by dhjin, �Ʒ��� ���� - �ӽ÷� �����ߴ� ���� ������ �������� ����
	{
		mt_auto_lock mtA(&m_mtvectVCNTeam);			// 2012-09-24 by jhseol, �ҽ����� - Lock ����
	mtvectorClientIndex_t::iterator itrA = m_mtvectVCNTeam.begin();
	while(itrA != m_mtvectVCNTeam.end())
	{
		CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrA);
		if(NULL == tm_pFISocket)
		{
			itrA++;
			continue;
		}

		tm_pFISocket->SendUserInfoForArenaEnding();		// 2011-10-17 by hskim, ��Ʈ�� �ý��� 2��
		tm_pFISocket->InitUserInfoForArenaEnding();
		//////////////////////////////////////////////////////////////////////
		// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : ��ο� �϶� ������ �������� ����Ƚ���� �ٽ� ���� ��Ų�� MF�� ������Ʈ ��û
		tm_pFISocket->m_character.ArenaPlayInfo.nPlayCount--;
		if(tm_pFISocket->m_character.ArenaPlayInfo.nPlayCount < 0)
		{
			tm_pFISocket->m_character.ArenaPlayInfo.nPlayCount = 0;
		}
		tm_pFISocket->ArenaPlayInfoUpdataToMF();
		// end 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : ��ο� �϶� ������ �������� ����Ƚ���� �ٽ� ���� ��Ų�� MF�� ������Ʈ ��û

		itrA++;
	}	
	}


	{
		mt_auto_lock mtA(&m_mtvectANITeam);			// 2012-09-24 by jhseol, �ҽ����� - Lock ����
	mtvectorClientIndex_t::iterator itrB = m_mtvectANITeam.begin();
	while(itrB != m_mtvectANITeam.end())
	{
		CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrB);
		if(NULL == tm_pFISocket)
		{
			itrB++;
			continue;
		}

		tm_pFISocket->SendUserInfoForArenaEnding();		// 2011-10-17 by hskim, ��Ʈ�� �ý��� 2��
		tm_pFISocket->InitUserInfoForArenaEnding();
		//////////////////////////////////////////////////////////////////////
		// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : ��ο� �϶� ������ �������� ����Ƚ���� �ٽ� ���� ��Ų�� MF�� ������Ʈ ��û
		tm_pFISocket->m_character.ArenaPlayInfo.nPlayCount--;
		if(tm_pFISocket->m_character.ArenaPlayInfo.nPlayCount < 0)
		{
			tm_pFISocket->m_character.ArenaPlayInfo.nPlayCount = 0;
		}
		tm_pFISocket->ArenaPlayInfoUpdataToMF();
		// end 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : ��ο� �϶� ������ �������� ����Ƚ���� �ٽ� ���� ��Ų�� MF�� ������Ʈ ��û

		itrB++;
		}
	}

	m_FinishRoomTime.SetCurrentDateTime();	
	m_DeathMatchState	= ARENA_STATE_FIGHTING_WARFINISH;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			INT	CArenaDeathMatchRoom::MakeMSG_FC_ARENA_ROOM_WAR_FINISH(SARENA_ROOM_FINISH_INFO * o_pArenaRoomFinishInfo)
/// \brief		������ġ ����� ���Ͽ� ���� ���� �����
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
//INT	CArenaDeathMatchRoom::MakeMSG_FC_ARENA_ROOM_WAR_FINISH(SARENA_ROOM_FINISH_INFO * o_pArenaRoomFinishInfo)
//{
//
//	INT	nRoomInfoListCount	= 0;
//
//	//////////////////////////////////////////////////////////////////////////
//	// 2007-04-24 by dhjin, ���� A���� ������ �ִ´�.
//	mtvectorClientIndex_t *pmtVectCliIdx = GetVectByInfl(INFLUENCE_TYPE_VCN);
//	mtvectorClientIndex_t::iterator itr = pmtVectCliIdx->begin();
//	while(itr != pmtVectCliIdx->end())
//	{
//		CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
//		if(!CheckCFieldIOCPSocketValid(tm_pFISocket, ARENA_STATE_WARING))
//		{
//			itr = pmtVectCliIdx->erase(itr);
//			continue;
//		}
//		o_pArenaRoomFinishInfo[nRoomInfoListCount].Influence			= tm_pFISocket->m_character.InfluenceType;
//		o_pArenaRoomFinishInfo[nRoomInfoListCount].ShootingDown			= tm_pFISocket->m_ArenaInfo.ShootingDown;
//		o_pArenaRoomFinishInfo[nRoomInfoListCount].SufferingAttack		= tm_pFISocket->m_ArenaInfo.SufferingAttack;
//		o_pArenaRoomFinishInfo[nRoomInfoListCount].UnitKind				= tm_pFISocket->m_character.UnitKind;
//		util::strncpy(o_pArenaRoomFinishInfo[nRoomInfoListCount].CharacterLevelRank, tm_pFISocket->GetCharacterLevelRank(), SIZE_MAX_LEVELRANK);
//		util::strncpy(o_pArenaRoomFinishInfo[nRoomInfoListCount].CharacterName, tm_pFISocket->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
//		nRoomInfoListCount++;
//		itr++;
//	}
//
//	//////////////////////////////////////////////////////////////////////////
//	// 2007-04-24 by dhjin, B���� ���� �ۼ�.
//	pmtVectCliIdx = GetVectByInfl(INFLUENCE_TYPE_ANI);
//	itr = pmtVectCliIdx->begin();
//	while(itr != pmtVectCliIdx->end())
//	{
//		CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
//		if(!CheckCFieldIOCPSocketValid(tm_pFISocket, ARENA_STATE_WARING))
//		{
//			itr = pmtVectCliIdx->erase(itr);
//			continue;
//		}
//		o_pArenaRoomFinishInfo[nRoomInfoListCount].Influence			= tm_pFISocket->m_character.InfluenceType;
//		o_pArenaRoomFinishInfo[nRoomInfoListCount].ShootingDown			= tm_pFISocket->m_ArenaInfo.ShootingDown;
//		o_pArenaRoomFinishInfo[nRoomInfoListCount].SufferingAttack		= tm_pFISocket->m_ArenaInfo.SufferingAttack;
//		o_pArenaRoomFinishInfo[nRoomInfoListCount].UnitKind				= tm_pFISocket->m_character.UnitKind;
//		util::strncpy(o_pArenaRoomFinishInfo[nRoomInfoListCount].CharacterLevelRank, tm_pFISocket->GetCharacterLevelRank(), SIZE_MAX_LEVELRANK);
//		util::strncpy(o_pArenaRoomFinishInfo[nRoomInfoListCount].CharacterName, tm_pFISocket->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
//		nRoomInfoListCount++;
//		itr++;
//	}
//
////	mtvectorClientIndex_t::iterator itrA = m_mtvectVCNTeam.begin();
////	while(itrA != m_mtvectVCNTeam.end())
////	{
////		CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrA);
////		if(!CheckCFieldIOCPSocketValid(tm_pFISocket, ARENA_STATE_WARING))
////		{
////			itrA = m_mtvectVCNTeam.erase(itrA);
////			continue;
////		}
////		o_pArenaRoomFinishInfo[nRoomInfoListCount].Influence			= tm_pFISocket->m_character.InfluenceType;
////		o_pArenaRoomFinishInfo[nRoomInfoListCount].ShootingDown			= tm_pFISocket->m_ArenaInfo.ShootingDown;
////		o_pArenaRoomFinishInfo[nRoomInfoListCount].SufferingAttack		= tm_pFISocket->m_ArenaInfo.SufferingAttack;
////		o_pArenaRoomFinishInfo[nRoomInfoListCount].UnitKind				= tm_pFISocket->m_character.UnitKind;
////		util::strncpy(o_pArenaRoomFinishInfo[nRoomInfoListCount].CharacterLevelRank, tm_pFISocket->GetCharacterLevelRank(), SIZE_MAX_LEVELRANK);
////		util::strncpy(o_pArenaRoomFinishInfo[nRoomInfoListCount].CharacterName, tm_pFISocket->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
////		nRoomInfoListCount++;
////		itrA++;
////	}
////
////	//////////////////////////////////////////////////////////////////////////
////	// 2007-04-24 by dhjin, B���� ���� �ۼ�.
////	mtvectorClientIndex_t::iterator itrB = m_mtvectANITeam.begin();
////	while(itrB != m_mtvectANITeam.end())
////	{
////		CFieldIOCPSocket *tm_pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrB);
////		if(!CheckCFieldIOCPSocketValid(tm_pFISocket, ARENA_STATE_WARING))
////		{
////			itrB = m_mtvectANITeam.erase(itrB);
////			continue;
////		}
////		o_pArenaRoomFinishInfo[nRoomInfoListCount].Influence			= tm_pFISocket->m_character.InfluenceType;
////		o_pArenaRoomFinishInfo[nRoomInfoListCount].ShootingDown			= tm_pFISocket->m_ArenaInfo.ShootingDown;
////		o_pArenaRoomFinishInfo[nRoomInfoListCount].SufferingAttack		= tm_pFISocket->m_ArenaInfo.SufferingAttack;
////		o_pArenaRoomFinishInfo[nRoomInfoListCount].UnitKind				= tm_pFISocket->m_character.UnitKind;
////		util::strncpy(o_pArenaRoomFinishInfo[nRoomInfoListCount].CharacterLevelRank, tm_pFISocket->GetCharacterLevelRank(), SIZE_MAX_LEVELRANK);
////		util::strncpy(o_pArenaRoomFinishInfo[nRoomInfoListCount].CharacterName, tm_pFISocket->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
////		nRoomInfoListCount++;
////		itrB++;
////	}
//
//	return nRoomInfoListCount;	
//}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::AllUserWarp()
/// \brief		������ġ�� ����ǰ� 30�ʰ� ���� ������ ������ ������Ų��
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::AllUserWarp()
{	
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, ���� ��� ������ ����
	mtvectorClientIndex_t *pmtVectCliIdx = GetVectByInfl(INFLUENCE_TYPE_VCN);
	mtvectorClientIndex_t::iterator itr = pmtVectCliIdx->begin();
	while(itr != pmtVectCliIdx->end())
	{
		CFieldIOCPSocket *pFISock = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(!CheckCFieldIOCPSocketValid(pFISock, ARENA_STATE_FIGHTING_WARFINISH))
		{
//			this->SendWatchRemoveUser(*itr);		// 2007-06-18 by dhjin, �Ʒ������� ���� ���� ���� ����.
			itr = pmtVectCliIdx->erase(itr);
			continue;
		}
		// 2007-05-11 by dhjin, ������ ���� �� �Ʒ��� ������ �̵��ϱ� �� HP, DP, SP, EP�� �����Ѵ�.
		pFISock->ArenaWarpCitySetHPDPSPEP();

		pFISock->WarpToCityMap();
		pFISock->m_ArenaInfo.State = ARENA_STATE_READY;
		// 2007-05-09 by dhjin, ����/�ǰ�/������ġ �� ���� �ʱ�ȭ
		pFISock->m_ArenaInfo.ShootingDown		= 0;
		pFISock->m_ArenaInfo.SufferingAttack	= 0;
		pFISock->m_ArenaInfo.LostPoint			= 0;
		pFISock->m_ArenaInfo.DeathMatchRoomNum	= -1;

		itr++;
	}

	pmtVectCliIdx = GetVectByInfl(INFLUENCE_TYPE_ANI);
	itr = pmtVectCliIdx->begin();
	while(itr != pmtVectCliIdx->end())
	{
		CFieldIOCPSocket *pFISock = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(!CheckCFieldIOCPSocketValid(pFISock, ARENA_STATE_FIGHTING_WARFINISH))
		{
//			this->SendWatchRemoveUser(*itr);		// 2007-06-18 by dhjin, �Ʒ������� ���� ���� ���� ����.
			itr = pmtVectCliIdx->erase(itr);
			continue;
		}
		// 2007-05-11 by dhjin, ������ ���� �� �Ʒ��� ������ �̵��ϱ� �� HP, DP, SP, EP�� �����Ѵ�.
		pFISock->ArenaWarpCitySetHPDPSPEP();

		pFISock->WarpToCityMap();
		pFISock->m_ArenaInfo.State = ARENA_STATE_READY;
		// 2007-05-09 by dhjin, ����/�ǰ�/������ġ �� ���� �ʱ�ȭ
		pFISock->m_ArenaInfo.ShootingDown		= 0;
		pFISock->m_ArenaInfo.SufferingAttack	= 0;
		pFISock->m_ArenaInfo.LostPoint			= 0;
		pFISock->m_ArenaInfo.DeathMatchRoomNum	= -1;

		itr++;
	}	

	//////////////////////////////////////////////////////////////////////////
	// 2007-06-18 by dhjin, �����ڵ� ��� ������ ������.
	mtvectorClientIndex_t::iterator itrW = m_mtvectWatchUserList.begin();
	for(; itrW != m_mtvectWatchUserList.end(); itrW++)
	{
		CFieldIOCPSocket *pFISock = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrW);
		if(!CheckCFieldIOCPSocketValid(pFISock, ARENA_STATE_FIGHTING_WARFINISH))
		{
			continue;
		}

		pFISock->WarpToCityMap();
		pFISock->m_ArenaInfo.WatchState = FALSE;
		pFISock->m_ArenaInfo.State = ARENA_STATE_NONE;
		// 2007-05-09 by dhjin, ����/�ǰ�/������ġ �� ���� �ʱ�ȭ
		pFISock->m_ArenaInfo.ShootingDown		= 0;
		pFISock->m_ArenaInfo.SufferingAttack	= 0;
		pFISock->m_ArenaInfo.LostPoint			= 0;
		pFISock->m_ArenaInfo.DeathMatchRoomNum	= -1;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-06-18 by dhjin, �Ʒ��� �� ������ �������� �����Ѵ�.
	this->PopAllWatchUserObserver();


//	mtvectorClientIndex_t::iterator itrA = m_mtvectVCNTeam.begin();
//	while(itrA != m_mtvectVCNTeam.end())
//	{
//		CFieldIOCPSocket *tm_pFieldIOCPSocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrA);
//		if(!CheckCFieldIOCPSocketValid(tm_pFieldIOCPSocket, ARENA_STATE_WARFINISH))
//		{
//			itrA = m_mtvectVCNTeam.erase(itrA);
//			continue;
//		}
//		tm_pFieldIOCPSocket->WarpToCityMap();
//		tm_pFieldIOCPSocket->m_ArenaInfo.State = ARENA_STATE_READY;
//		itrA++;
//	}
//	mtvectorClientIndex_t::iterator itrB = m_mtvectANITeam.begin();
//	while(itrB != m_mtvectANITeam.end())
//	{
//		CFieldIOCPSocket *tm_pFieldIOCPSocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrB);
//		if(!CheckCFieldIOCPSocketValid(tm_pFieldIOCPSocket, ARENA_STATE_WARFINISH))
//		{
//			itrB = m_mtvectANITeam.erase(itrB);
//			continue;
//		}
//		tm_pFieldIOCPSocket->WarpToCityMap();
//		tm_pFieldIOCPSocket->m_ArenaInfo.State = ARENA_STATE_READY;
//		itrB++;
//	}	

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-04 by dhjin, ���� ARENA���°��� �����Ѵ�
	this->m_pFieldIOCP17->m_ArenaManager.CheckFullMemberByFinishRoom(m_VCNTeamNum);
	this->m_pFieldIOCP17->m_ArenaManager.CheckFullMemberByFinishRoom(m_ANITeamNum);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::ReadyUserWarp(CFieldIOCPSocket * i_pFISock)
/// \brief		Ŭ���̾�Ʈ���� ���� ��û�� �޾� ������ ������Ű�� ���� ������ ���� �۾��� �Ѵ�
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::ReadyUserWarp(CFieldIOCPSocket * i_pFISock)
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, ���� ������ ����
	if(INFLUENCE_TYPE_VCN == i_pFISock->m_character.InfluenceType)
	{
		mt_auto_lock mtA(&m_mtvectVCNTeam);
		mtvectorClientIndex_t::iterator itrA = find(m_mtvectVCNTeam.begin(), m_mtvectVCNTeam.end(), i_pFISock->m_character.ClientIndex);
		if(itrA == m_mtvectVCNTeam.end())
		{
			return;
		}

		if(!CheckCFieldIOCPSocketValid(i_pFISock, ARENA_STATE_FIGHTING_WARFINISH))
		{
			return;
		}

//		this->SendWatchRemoveUser(*itrA);		// 2007-06-18 by dhjin, �Ʒ������� ���� ���� ���� ����.
		itrA = m_mtvectVCNTeam.erase(itrA);	
	}
	else
	{
		mt_auto_lock mtA(&m_mtvectANITeam);
		mtvectorClientIndex_t::iterator itrB = find(m_mtvectANITeam.begin(), m_mtvectANITeam.end(), i_pFISock->m_character.ClientIndex);
		if(itrB == m_mtvectANITeam.end())
		{
			return;
		}

		if(!CheckCFieldIOCPSocketValid(i_pFISock, ARENA_STATE_FIGHTING_WARFINISH))
		{
			return;
		}

//		this->SendWatchRemoveUser(*itrB);		// 2007-06-18 by dhjin, �Ʒ������� ���� ���� ���� ����.
		itrB = m_mtvectANITeam.erase(itrB);	
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CArenaDeathMatchRoom::CheckCFieldIOCPSocketValid(CFieldIOCPSocket * i_pFISock, BYTE i_nArenaState)
/// \brief		��ȿ�� �������� üũ�Ѵ�.
/// \author		dhjin
/// \date		2007-05-03 ~ 2007-05-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CArenaDeathMatchRoom::CheckCFieldIOCPSocketValid(CFieldIOCPSocket * i_pFISock, BYTE i_nArenaState)
{
	if(NULL == i_pFISock
		|| FALSE == i_pFISock->IsValidCharacter(FALSE)
		|| i_nArenaState != i_pFISock->m_ArenaInfo.State
		|| ARENAMODE_DEATHMATCH != i_pFISock->m_ArenaInfo.ArenaMode
		|| m_DeathMatchRoomNum != i_pFISock->m_ArenaInfo.DeathMatchRoomNum)
	{
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::SetPlayLimitedTime(BYTE i_nPlayLimitedTime)
/// \brief		��� ���� �ð� ����
/// \author		dhjin
/// \date		2007-05-04 ~ 2007-05-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::SetPlayLimitedTime(BYTE i_nPlayLimitedTime)
{
	m_PlayLimitedTime = i_nPlayLimitedTime;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BYTE CArenaDeathMatchRoom::GetVCNTeamSize()
/// \brief		������������ �ο���
/// \author		dhjin
/// \date		2007-05-04 ~ 2007-05-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE CArenaDeathMatchRoom::GetVCNTeamSize()
{
	return m_mtvectVCNTeam.size();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BYTE CArenaDeathMatchRoom::GetANITeamSize()
/// \brief		�˸����� �ο���
/// \author		dhjin
/// \date		2007-05-04 ~ 2007-05-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////	
BYTE CArenaDeathMatchRoom::GetANITeamSize()
{
	return m_mtvectANITeam.size();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BYTE CArenaDeathMatchRoom::GetWatchUserSize()
/// \brief		
/// \author		dhjin
/// \date		2007-07-04 ~ 2007-07-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE CArenaDeathMatchRoom::GetWatchUserSize()
{
	return m_mtvectWatchUserList.size();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::CheckLiveMemberByDoMinutelyWorkArena()
/// \brief		1�� ���� ��ȿ�� �������� üũ�Ѵ�
/// \author		dhjin
/// \date		2007-05-04 ~ 2007-05-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::CheckLiveMemberByDoMinutelyWorkArena()
{
	mtvectorClientIndex_t *pmtVectCliIdx = GetVectByInfl(INFLUENCE_TYPE_VCN);
	mtvectorClientIndex_t::iterator itr = pmtVectCliIdx->begin();
	while(itr != pmtVectCliIdx->end())
	{
		CFieldIOCPSocket *pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(NULL == pFISocket
			|| FALSE == pFISocket->IsValidCharacter(FALSE)
			|| ARENAMODE_DEATHMATCH != pFISocket->m_ArenaInfo.ArenaMode
			|| m_DeathMatchRoomNum != pFISocket->m_ArenaInfo.DeathMatchRoomNum)	
		{
			if(FALSE == pFISocket->m_ArenaInfo.WatchState)
			{// 2007-06-19 by dhjin, �����ڴ� ���� ������ ������ �ʴ´�.
				this->SendWatchRemoveUser(*itr);		// 2007-06-18 by dhjin, �Ʒ������� ���� ���� ���� ����.
			}
			itr = pmtVectCliIdx->erase(itr);
			continue;
		}
		itr++;
	}

	pmtVectCliIdx = GetVectByInfl(INFLUENCE_TYPE_ANI);
	itr = pmtVectCliIdx->begin();
	while(itr != pmtVectCliIdx->end())
	{
		CFieldIOCPSocket *pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(NULL == pFISocket
			|| FALSE == pFISocket->IsValidCharacter(FALSE)
			|| ARENAMODE_DEATHMATCH != pFISocket->m_ArenaInfo.ArenaMode
			|| m_DeathMatchRoomNum != pFISocket->m_ArenaInfo.DeathMatchRoomNum)	
		{
			if(FALSE == pFISocket->m_ArenaInfo.WatchState)
			{// 2007-06-19 by dhjin, �����ڴ� ���� ������ ������ �ʴ´�.
				this->SendWatchRemoveUser(*itr);		// 2007-06-18 by dhjin, �Ʒ������� ���� ���� ���� ����.
			}
			itr = pmtVectCliIdx->erase(itr);
			continue;
		}
		itr++;
	}
	
	itr = m_mtvectWatchUserList.begin();
	while(itr != m_mtvectWatchUserList.end())
	{
		CFieldIOCPSocket *pFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(NULL == pFISoc
			|| FALSE == pFISoc->IsValidCharacter(FALSE)
			|| ARENAMODE_DEATHMATCH != pFISoc->m_ArenaInfo.ArenaMode
			|| m_DeathMatchRoomNum != pFISoc->m_ArenaInfo.DeathMatchRoomNum)
		{
			itr = m_mtvectWatchUserList.erase(itr);
			continue;
		}
		itr++;
	}

//	mtvectorClientIndex_t::iterator itrA = m_mtvectVCNTeam.begin();
//	while(itrA != m_mtvectVCNTeam.end())
//	{
//		CFieldIOCPSocket *pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrA);
//		if(NULL == pFISocket
//			|| FALSE == pFISocket->IsValidCharacter(FALSE)
//			|| ARENAMODE_DEATHMATCH != pFISocket->m_ArenaInfo.ArenaMode
//			|| m_DeathMatchRoomNum != pFISocket->m_ArenaInfo.DeathMatchRoomNum)	
//		{
//			itrA = m_mtvectVCNTeam.erase(itrA);
//			continue;
//		}
//		itrA++;
//	}
//
//	mtvectorClientIndex_t::iterator itrB = m_mtvectANITeam.begin();
//	while(itrB != m_mtvectANITeam.end())
//	{
//		CFieldIOCPSocket *pFISocket = this->m_pFieldIOCP17->GetFieldIOCPSocket(*itrB);
//		if(NULL == pFISocket
//			|| FALSE == pFISocket->IsValidCharacter(FALSE)
//			|| ARENAMODE_DEATHMATCH != pFISocket->m_ArenaInfo.ArenaMode
//			|| m_DeathMatchRoomNum != pFISocket->m_ArenaInfo.DeathMatchRoomNum)	
//		{
//			itrB = m_mtvectANITeam.erase(itrB);
//			continue;
//		}
//		itrB++;
//	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CArenaDeathMatchRoom::InitDeathMatchRoomState()
/// \brief		�ʱ�ȭ ���� ���� ����
/// \author		dhjin
/// \date		2007-05-07 ~ 2007-05-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CArenaDeathMatchRoom::InitDeathMatchRoomState()
{
	return m_InitDeathMatchRoom;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CArenaDeathMatchRoom::CheckEmptyTeamSize()
/// \brief		�� �� ��� ���� ������ üũ
/// \author		dhjin
/// \date		2007-05-08 ~ 2007-05-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CArenaDeathMatchRoom::CheckEmptyTeamSize()
{
	if(0 == m_mtvectVCNTeam.size()
		&& 0 == m_mtvectANITeam.size())
	{
		return TRUE;
	}

	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int CArenaDeathMatchRoom::Send2ArenaDeathRoom(BYTE i_nArenaState, vectorClientIndex *i_pVectCIdx, BYTE *i_pSendBuf, int i_nSendBytes, BYTE i_nUpdateArenaState /* = FALSE */)
/// \brief		
/// \author		cmkwon
/// \date		2007-05-08 ~ 2007-05-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CArenaDeathMatchRoom::Send2ArenaDeathRoom(BYTE i_nArenaState, vectorClientIndex *i_pVectCIdx, BYTE *i_pSendBuf, int i_nSendBytes, BYTE i_nUpdateArenaState /* = FALSE */)
{
	int nSendCnt = 0;
	vectorClientIndex::iterator itr(i_pVectCIdx->begin());
	while(itr != i_pVectCIdx->end())
	{
		CFieldIOCPSocket *pFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(!CheckCFieldIOCPSocketValid(pFISoc, i_nArenaState))
		{
			if(FALSE == pFISoc->m_ArenaInfo.WatchState)
			{// 2007-06-19 by dhjin, �����ڴ� ���� ������ ������ �ʴ´�.
				this->SendWatchRemoveUser(*itr);		// 2007-06-18 by dhjin, �Ʒ������� ���� ���� ���� ����.
			}
			itr = i_pVectCIdx->erase(itr);
			continue;
		}

		if(FALSE != i_nUpdateArenaState)
		{
			pFISoc->m_ArenaInfo.State = i_nUpdateArenaState;
		}
		
		pFISoc->SendAddData(i_pSendBuf, i_nSendBytes);
		nSendCnt++;
		itr++;
	}

	return nSendCnt;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CArenaDeathMatchRoom::Send2ArenaDeathRoom(BYTE i_nArenaState, vectorClientIndex *i_pVectCIdx, MessageType_t i_msgTy)
/// \brief		
/// \author		cmkwon
/// \date		2007-05-08 ~ 2007-05-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CArenaDeathMatchRoom::Send2ArenaDeathRoom(BYTE i_nArenaState, vectorClientIndex *i_pVectCIdx, MessageType_t i_msgTy)
{
	return this->Send2ArenaDeathRoom(i_nArenaState, i_pVectCIdx, (BYTE*)(&i_msgTy), sizeof(MessageType_t));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			mtvectorClientIndex_t * CArenaDeathMatchRoom::GetVectByInfl(BYTE i_nInfl)
/// \brief		
/// \author		dhjin
/// \date		2007-05-08 ~ 2007-05-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
mtvectorClientIndex_t * CArenaDeathMatchRoom::GetVectByInfl(BYTE i_nInfl)
{
	if(INFLUENCE_TYPE_VCN == i_nInfl)
	{
		return &m_mtvectVCNTeam;
	}
	else
	{
		return &m_mtvectANITeam;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::InsertWatchUserList(ClientIndex_t i_nClientIdx)
/// \brief		�����ϴ� ���� ����Ʈ�� �߰�
/// \author		dhjin
/// \date		2007-06-15 ~ 2007-06-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::InsertWatchUserList(ClientIndex_t i_nClientIdx)
{
	mt_auto_lock mta(&m_mtvectWatchUserList);

	m_mtvectWatchUserList.push_back(i_nClientIdx);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::DeleteWatchUserList(ClientIndex_t i_nClientIdx)
/// \brief		�����ϴ� ���� ����Ʈ�� ����
/// \author		dhjin
/// \date		2007-06-15 ~ 2007-06-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::DeleteWatchUserList(ClientIndex_t i_nClientIdx)
{
	mt_auto_lock mtW(&m_mtvectWatchUserList);
	mt_auto_lock mtB(&m_mtvectVCNTeam);
	mt_auto_lock mtA(&m_mtvectANITeam);	

	mtvectorClientIndex_t::iterator itr = find(m_mtvectWatchUserList.begin(), m_mtvectWatchUserList.end(), i_nClientIdx);
	if(itr == m_mtvectWatchUserList.end())
	{
		return;
	}

	CFieldIOCPSocket *pFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
	if(NULL == pFISoc
		|| FALSE == pFISoc->IsValidCharacter(FALSE))
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-06-18 by dhjin, ������ �����ڸ� �����Ѵ�.
	this->PopWatchUserObserver(pFISoc, &m_mtvectVCNTeam);
	this->PopWatchUserObserver(pFISoc, &m_mtvectANITeam);

//	mtvectorClientIndex_t::iterator itrV = m_mtvectVCNTeam.begin();
//	for(; itrV != m_mtvectVCNTeam.end(); itrV++)
//	{
//		CFieldIOCPSocket *pVCNFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itrV);
//		if(NULL == pVCNFISoc
//			|| FALSE == pVCNFISoc->IsValidCharacter(FALSE))
//		{
//			continue;
//		}
//		pVCNFISoc->PopWatchUser(pFISoc);
//	}
//	mtvectorClientIndex_t::iterator itrA = m_mtvectANITeam.begin();
//	for(; itrA != m_mtvectANITeam.end(); itrA++)
//	{
//		CFieldIOCPSocket *pANIFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itrA);
//		if(NULL == pANIFISoc
//			|| FALSE == pANIFISoc->IsValidCharacter(FALSE))
//		{
//			continue;
//		}
//		pANIFISoc->PopWatchUser(pFISoc);
//	}

	pFISoc->SendAddMessageType(T_FC_ARENA_WATCH_END);

	//////////////////////////////////////////////////////////////////////////
	// 2007-06-18 by dhjin, �����ϴ� ���� ����Ʈ���� ����.
	itr = m_mtvectWatchUserList.erase(itr);
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::PopAllWatchUserObserver()
/// \brief		�Ʒ����� ������ ��� ������ ���������� �����ڸ� �����Ѵ�.
/// \author		dhjin
/// \date		2007-06-18 ~ 2007-06-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::PopAllWatchUserObserver()
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-06-18 by dhjin, ������ ��ο��� ���� ����
	mtvectorClientIndex_t::iterator itr = m_mtvectWatchUserList.begin();
	for(; itr != m_mtvectWatchUserList.end(); itr++)
	{
		CFieldIOCPSocket *pFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(NULL == pFISoc
			|| FALSE == pFISoc->IsValidCharacter(FALSE))
		{
			continue;
		}
		
		if(ARENA_STATE_FIGHTING_WARFINISH == pFISoc->m_ArenaInfo.State)
		{// 2007-06-18 by dhjin, �Ʒ��� ���� ������ �����ڿ��Ը� ������.

			//////////////////////////////////////////////////////////////////////////
			// 2007-06-18 by dhjin, ������ �����ڸ� �����Ѵ�.
			this->PopWatchUserObserver(pFISoc, &m_mtvectVCNTeam);
			this->PopWatchUserObserver(pFISoc, &m_mtvectANITeam);

//			mtvectorClientIndex_t::iterator itrV = m_mtvectVCNTeam.begin();
//			for(; itrV != m_mtvectVCNTeam.end(); itrV++)
//			{
//				CFieldIOCPSocket *pVCNFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itrV);
//				if(NULL == pVCNFISoc
//					|| FALSE == pVCNFISoc->IsValidCharacter(FALSE))
//				{
//					continue;
//				}
//				pVCNFISoc->PopWatchUser(pFISoc);
//			}
//			mtvectorClientIndex_t::iterator itrA = m_mtvectANITeam.begin();
//			for(; itrA != m_mtvectANITeam.end(); itrA++)
//			{
//				CFieldIOCPSocket *pANIFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itrA);
//				if(NULL == pANIFISoc
//					|| FALSE == pANIFISoc->IsValidCharacter(FALSE))
//				{
//					continue;
//				}
//				pANIFISoc->PopWatchUser(pFISoc);
//			}
		}
		
		pFISoc->m_ArenaInfo.ResetArenaInfo();
		pFISoc->SendAddMessageType(T_FC_ARENA_WATCH_END);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::SendWatchStartInfo()
/// \brief		�����ϴ� ���� ���� ������
/// \author		dhjin
/// \date		2007-06-18 ~ 2007-06-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::SendWatchStartInfo()
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-07-04 by dhjin, �����ڰ� ���ų� 12�� �� �ʰ��� �����Ѵ�. (�������� 12��������.)
	if(0 == this->GetWatchUserSize()
		|| 12 < this->GetANITeamSize()
		|| 12 < this->GetVCNTeamSize())
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-06-18 by dhjin, �޼��� �����
	INIT_MSG_WITH_BUFFER(MSG_FC_ARENA_WATCH_START, T_FC_ARENA_WATCH_START, pArenaWatchStartUserInfo, SendBuf);
	pArenaWatchStartUserInfo->VCNDeathCount						= m_VCNDeathCount;
	pArenaWatchStartUserInfo->ANIDeathCount						= m_ANIDeathCount;
	pArenaWatchStartUserInfo->Time								= (m_PlayLimitedTime * 60)  - (m_CreateRoomTime.GetTimeDiffToCurrentTimeInSeconds());
	pArenaWatchStartUserInfo->ArenaWatchStartUserInfoListCount	= this->MakeMSG_FC_ARENA_WATCH_START((SARENA_WATCH_START_USER_INFO*)(SendBuf+MSG_SIZE(MSG_FC_ARENA_WATCH_START)));
	if(0 >= pArenaWatchStartUserInfo->ArenaWatchStartUserInfoListCount)
	{
		return;
	}

	mt_auto_lock mtW(&m_mtvectWatchUserList);
	mt_auto_lock mtV(&m_mtvectVCNTeam);
	mt_auto_lock mtA(&m_mtvectANITeam);
	//////////////////////////////////////////////////////////////////////////
	// 2007-06-18 by dhjin, ������ ��ο��� ���� ����
	mtvectorClientIndex_t::iterator itr = m_mtvectWatchUserList.begin();
	for(; itr != m_mtvectWatchUserList.end(); itr++)
	{
		CFieldIOCPSocket *pFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(NULL == pFISoc
			|| FALSE == pFISoc->IsValidCharacter(FALSE))
		{
			continue;
		}
		
		if(ARENA_STATE_FIGHTING_WARING == pFISoc->m_ArenaInfo.State)
		{// 2007-06-18 by dhjin, �Ʒ��� ���� ������ �����ڿ��Ը� ������.
			pFISoc->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ARENA_WATCH_START)+pArenaWatchStartUserInfo->ArenaWatchStartUserInfoListCount*sizeof(SARENA_WATCH_START_USER_INFO));
			//////////////////////////////////////////////////////////////////////////
			// 2007-06-18 by dhjin, �������� �����ڸ� ���δ�.
			this->ADDWatchUserObserver(pFISoc, &m_mtvectVCNTeam);
			this->ADDWatchUserObserver(pFISoc, &m_mtvectANITeam);

//			mtvectorClientIndex_t::iterator itrV = m_mtvectVCNTeam.begin();
//			for(; itrV != m_mtvectVCNTeam.end(); itrV++)
//			{
//				CFieldIOCPSocket *pVCNFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itrV);
//				if(NULL == pVCNFISoc
//					|| FALSE == pVCNFISoc->IsValidCharacter(FALSE))
//				{
//					continue;
//				}
//				pVCNFISoc->AddWatchUser(pFISoc);
//			}
//			mtvectorClientIndex_t::iterator itrA = m_mtvectANITeam.begin();
//			for(; itrA != m_mtvectANITeam.end(); itrA++)
//			{
//				CFieldIOCPSocket *pANIFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itrA);
//				if(NULL == pANIFISoc
//					|| FALSE == pANIFISoc->IsValidCharacter(FALSE))
//				{
//					continue;
//				}
//				pANIFISoc->AddWatchUser(pFISoc);
// 			}
		}
	}
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT	CArenaDeathMatchRoom::MakeMSG_FC_ARENA_WATCH_START(SARENA_WATCH_START_USER_INFO * o_pArenaWatchStartUserInfo)
/// \brief		
/// \author		dhjin
/// \date		2007-06-18 ~ 2007-06-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT	CArenaDeathMatchRoom::MakeMSG_FC_ARENA_WATCH_START(SARENA_WATCH_START_USER_INFO * o_pArenaWatchStartUserInfo)
{
	INT	nTeamListCount = 0;

	mt_auto_lock mtV(&m_mtvectVCNTeam);
	mtvectorClientIndex_t::iterator itr = m_mtvectVCNTeam.begin();
	for(; itr != m_mtvectVCNTeam.end(); itr++)
	{
		// 2007-07-04 by dhjin, SIZE_MAX_PACKET ũ�⸦ ���� �ʾƾ��Ѵ�. 24��� ��� ũ�⸦ �Ѿ��.
		if(SIZE_MAX_PACKET < MSG_SIZE(MSG_FC_ARENA_WATCH_START) + sizeof(SARENA_WATCH_START_USER_INFO) * (nTeamListCount+1))
		{
			break;
		}
		CFieldIOCPSocket *pFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(NULL == pFISoc
			|| FALSE == pFISoc->IsValidCharacter(FALSE))
		{
			continue;
		}
		
		o_pArenaWatchStartUserInfo[nTeamListCount].HP			= pFISoc->m_character.HP;
		o_pArenaWatchStartUserInfo[nTeamListCount].CurrentHP	= pFISoc->m_character.CurrentHP;
		o_pArenaWatchStartUserInfo[nTeamListCount].DP			= pFISoc->m_character.DP;
		o_pArenaWatchStartUserInfo[nTeamListCount].CurrentDP	= pFISoc->m_character.CurrentDP;
		o_pArenaWatchStartUserInfo[nTeamListCount].Influence	= pFISoc->m_character.InfluenceType;
		o_pArenaWatchStartUserInfo[nTeamListCount].ClientIndex	= pFISoc->m_character.ClientIndex;
		util::strncpy(o_pArenaWatchStartUserInfo[nTeamListCount].CharacterName, pFISoc->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
		nTeamListCount++;
	}
	mtV.auto_unlock_cancel();

	mt_auto_lock mtA(&m_mtvectANITeam);
	itr = m_mtvectANITeam.begin();
	for(; itr != m_mtvectANITeam.end(); itr++)
	{
		// 2007-07-04 by dhjin, SIZE_MAX_PACKET ũ�⸦ ���� �ʾƾ��Ѵ�. 24��� ��� ũ�⸦ �Ѿ��.
		if(SIZE_MAX_PACKET < MSG_SIZE(MSG_FC_ARENA_WATCH_START) + sizeof(SARENA_WATCH_START_USER_INFO) * (nTeamListCount+1))
		{
			break;
		}
		CFieldIOCPSocket *pFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(NULL == pFISoc
			|| FALSE == pFISoc->IsValidCharacter(FALSE))
		{
			continue;
		}
		
		o_pArenaWatchStartUserInfo[nTeamListCount].HP			= pFISoc->m_character.HP;
		o_pArenaWatchStartUserInfo[nTeamListCount].CurrentHP	= pFISoc->m_character.CurrentHP;
		o_pArenaWatchStartUserInfo[nTeamListCount].DP			= pFISoc->m_character.DP;
		o_pArenaWatchStartUserInfo[nTeamListCount].CurrentDP	= pFISoc->m_character.CurrentDP;
		o_pArenaWatchStartUserInfo[nTeamListCount].Influence	= pFISoc->m_character.InfluenceType;
		o_pArenaWatchStartUserInfo[nTeamListCount].ClientIndex	= pFISoc->m_character.ClientIndex;
		util::strncpy(o_pArenaWatchStartUserInfo[nTeamListCount].CharacterName, pFISoc->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
		nTeamListCount++;
	}
	mtA.auto_unlock_cancel();

	return nTeamListCount;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::SendWatchRemoveUser(ClientIndex_t i_nClientIdx)
/// \brief		�Ʒ������� ���� ���� ������ �����ڿ��� �����Ѵ�.
/// \author		dhjin
/// \date		2007-06-18 ~ 2007-06-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::SendWatchRemoveUser(ClientIndex_t i_nClientIdx)
{
	mtvectorClientIndex_t::iterator itr = m_mtvectWatchUserList.begin();
	for(; itr != m_mtvectWatchUserList.end(); itr++)
	{
		CFieldIOCPSocket *pFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(NULL == pFISoc
			|| FALSE == pFISoc->IsValidCharacter(FALSE)
			|| i_nClientIdx == pFISoc->m_character.ClientIndex)		// 2007-06-19 by dhjin, �Ʒ������� ���� ������ �������̹Ƿ� ������ �ʴ´�.
		{
			continue;
		}
		
		if(ARENA_STATE_FIGHTING_WARING == pFISoc->m_ArenaInfo.State)
		{// 2007-06-18 by dhjin, �Ʒ��� ���� ������ �����ڿ��Ը� ������.
			INIT_MSG_WITH_BUFFER(MSG_FC_ARENA_WATCH_REMOVE_USER, T_FC_ARENA_WATCH_REMOVE_USER, pMSG, SendBuf);
			pMSG->ClientIndex			= i_nClientIdx;
			pFISoc->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ARENA_WATCH_REMOVE_USER));
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::PopWatchUserObserver(CFieldIOCPSocket * i_pFISoc, vectorClientIndex *i_pVectCIdx)
/// \brief		������ ����
/// \author		dhjin
/// \date		2007-06-21 ~ 2007-06-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::PopWatchUserObserver(CFieldIOCPSocket * i_pFISoc, vectorClientIndex *i_pVectCIdx)
{
	vectorClientIndex::iterator itr(i_pVectCIdx->begin());
	for(; itr != i_pVectCIdx->end(); itr++)
	{
		CFieldIOCPSocket *pFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(NULL == pFISoc
			|| FALSE == pFISoc->IsValidCharacter(FALSE))
		{
			continue;
		}
		pFISoc->PopWatchUser(i_pFISoc);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::ADDWatchUserObserver(CFieldIOCPSocket * i_pFISoc, vectorClientIndex *i_pVectCIdx)
/// \brief		������ �߰�
/// \author		dhjin
/// \date		2007-06-21 ~ 2007-06-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::ADDWatchUserObserver(CFieldIOCPSocket * i_pFISoc, vectorClientIndex *i_pVectCIdx)
{
    vectorClientIndex::iterator itr(i_pVectCIdx->begin());
	for(; itr != i_pVectCIdx->end(); itr++)
	{
		CFieldIOCPSocket *pFISoc = m_pFieldIOCP17->GetFieldIOCPSocket(*itr);
		if(NULL == pFISoc
			|| FALSE == pFISoc->IsValidCharacter(FALSE))
		{
			continue;
		}
		pFISoc->AddWatchUser(i_pFISoc);
	}
}




///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::SetVCNMFSInfl(BYTE i_VCNMFSInfl)
/// \brief		�Ʒ��� ���� - VCN�� MFS Influence ���� 	
/// \author		dhjin
/// \date		2008-02-19 ~ 2008-02-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::SetVCNMFSInfl(BYTE i_VCNMFSInfl)
{
	m_VCNMFSInfl = i_VCNMFSInfl;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::SetANIMFSInfl(BYTE i_ANIMFSInfl)
/// \brief		�Ʒ��� ���� - ANI�� MFS Influence ���� 	
/// \author		dhjin
/// \date		2008-02-19 ~ 2008-02-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::SetANIMFSInfl(BYTE i_ANIMFSInfl)
{
	m_ANIMFSInfl = i_ANIMFSInfl;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BYTE CArenaDeathMatchRoom::GetVCNMFSInfl()
/// \brief		�Ʒ��� ���� - VCN�� MFS Influence �������� 
/// \author		dhjin
/// \date		2008-02-19 ~ 2008-02-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE CArenaDeathMatchRoom::GetVCNMFSInfl()
{
	return m_VCNMFSInfl;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BYTE CArenaDeathMatchRoom::GetANIMFSInfl()
/// \brief		�Ʒ��� ���� - ANI�� MFS Influence ��������
/// \author		dhjin
/// \date		2008-02-19 ~ 2008-02-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE CArenaDeathMatchRoom::GetANIMFSInfl()
{
	return m_ANIMFSInfl;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaDeathMatchRoom::ArenaFinish();
/// \brief		// 2012-07-11 by jhseol, �Ʒ��� �������� ��ɾ� �߰�
/// \author		jhseol
/// \date		2012-07-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaDeathMatchRoom::ArenaFinish()
{
	m_PlayLimitedTime = 0;
}
