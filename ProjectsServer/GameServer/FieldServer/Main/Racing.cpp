// Racing.cpp: implementation of the CRacing class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Racing.h"
#include "MTAutoCriticalSection.h"
#include "FieldIOCPSocket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRacing::CRacing()
{
	m_racingState					= RACINGSTATE_STANDBY;
	m_dwLastChangeStateTick			= 0;

	m_byRacingSystemStartHour		= 12;				// ���� �ð�����(���� 4�ð��� ���ӻ󿡼� 1����) ���̽� ���۽ð�(����:�ð�)
	m_byRacingSystemEndHour			= m_byRacingSystemStartHour+3;		//
	
	m_byMaxUserCounts				= 8;				// �ִ� �����ڼ�
	m_byMinUserCounts				= 6;				// �ּ� �����ڼ�
	m_byJoinedUserCounts			= 0;				// ���̽� ���ӿ� ������ ����ڼ�
	m_byTotalCheckPointCounts		= 5;				// ���̽� ���� üũ ����Ʈ ��
	m_byRacingGameCounts			= 3;				// �� ����� ���̽� ���� ī��Ʈ
	m_byRacingGameNumber			= 1;				// ����� Ȥ�� ����ǰ� �ִ� ���̽� ��ȣ
	m_byGameRanking					= 1;
	m_nEntryFee						= 1000;				// ������(����:spi)	
	m_nLimitTimeForRacing			= 5*60;				// 5��, Racing�� ����Ǵ� ���ѽð�(����:��)	
	m_nRamainTimeForJoinRacing		= 0;				// ���� ���̽��� ����Ǳ� ���� ���� �ð�, ���ǽð�(����:��)
	m_nCountdownCounts				= 3;

	m_vectorWaitingRoomUserList.clear();				// ���� ���ǿ� ���� ����� ����Ʈ
	m_vectorJoinedUserList.clear();
}

CRacing::~CRacing()
{
	
}

void CRacing::SetRacingState(RacingState i_rState)
{
	m_racingState			= i_rState;
	m_dwLastChangeStateTick	= timeGetTime();
}

void CRacing::PutWaitingRoomUserList(CFieldIOCPSocket *i_FSocket)
{
	CMTAutoCriticalSection autoCri(m_mtcsRacing.GetCriticalSectionPtr());
	
	vectorFieldIOCPSocketPtr::iterator itr(m_vectorWaitingRoomUserList.begin());
	for(; itr != m_vectorWaitingRoomUserList.end(); itr++)
	{
		if(i_FSocket == (*itr))
		{
			return;
		}
	}

	m_vectorWaitingRoomUserList.push_back(i_FSocket);
}

void CRacing::PopWaitingRoomUserList(CFieldIOCPSocket *i_FSocket)
{
	CMTAutoCriticalSection autoCri(m_mtcsRacing.GetCriticalSectionPtr());
	
	vectorFieldIOCPSocketPtr::iterator itr(m_vectorWaitingRoomUserList.begin());
	for(; itr != m_vectorWaitingRoomUserList.end(); itr++)
	{
		if(i_FSocket == (*itr))
		{
			m_vectorWaitingRoomUserList.erase(itr);
			break;
		}
	}
}

int CRacing::PutJoinedUserList(CFieldIOCPSocket *i_FSocket)
{
	CMTAutoCriticalSection autoCri(m_mtcsRacing.GetCriticalSectionPtr());

	if(m_vectorJoinedUserList.size() >= this->m_byMaxUserCounts)
	{
		return 0;
	}

	RacingJoinedUserInfo	tmUserInfo;
	tmUserInfo.pJoinedFieldIOCPSocket	= i_FSocket;
	util::strncpy(tmUserInfo.szJoinedCharacterName, i_FSocket->GetCharacter()->CharacterName, SIZE_MAX_CHARACTER_NAME);
	m_vectorJoinedUserList.push_back(tmUserInfo);

	if(m_vectorJoinedUserList.size() >= this->m_byMaxUserCounts)
	{
		this->SetRacingState(RACINGSTATE_INVITIED);
	}
	return m_vectorJoinedUserList.size();
}

void CRacing::PopJoinedUserList(CFieldIOCPSocket *i_FSocket)
{
	CMTAutoCriticalSection autoCri(m_mtcsRacing.GetCriticalSectionPtr());
	
	vectorJoinedUserInfo::iterator itr(m_vectorJoinedUserList.begin());
	for(; itr != m_vectorJoinedUserList.end(); itr++)
	{
		if(i_FSocket == itr->pJoinedFieldIOCPSocket)
		{
			itr->pJoinedFieldIOCPSocket = NULL;
			break;
		}
	}
}

void CRacing::Send2WaitingRoomUserList(BYTE *pData, int nDataSize)
{
	this->LockRacing();
	vectorFieldIOCPSocketPtr	tmWaitingRoomUserList(m_vectorWaitingRoomUserList);
	this->UnlockRacing();

	vectorFieldIOCPSocketPtr::iterator itr(tmWaitingRoomUserList.begin());
	for(; itr != tmWaitingRoomUserList.end(); itr++)
	{
		(*itr)->SendAddData(pData, nDataSize);
	}
}
void CRacing::Send2JoinedRacingUserList(BYTE *pData, int nDataSize)
{
	this->LockRacing();
	vectorJoinedUserInfo	tmJoinedUserList(m_vectorJoinedUserList);
	this->UnlockRacing();

	vectorJoinedUserInfo::iterator itr(tmJoinedUserList.begin());
	for(; itr != tmJoinedUserList.end(); itr++)
	{
		if(itr->pJoinedFieldIOCPSocket)
		{
			itr->pJoinedFieldIOCPSocket->SendAddData(pData, nDataSize);
		}
	}
}

BOOL CRacing::CheckPoint(CFieldIOCPSocket *i_FSocket, BYTE i_byCheckPoint)
{
	CMTAutoCriticalSection autoCri(m_mtcsRacing.GetCriticalSectionPtr());
	
	vectorJoinedUserInfo::iterator itr(m_vectorJoinedUserList.begin());
	for(; itr != m_vectorJoinedUserList.end(); itr++)
	{
		if(i_FSocket == itr->pJoinedFieldIOCPSocket)
		{
			if(i_FSocket->m_byRacingNextCheckPointNumber == i_byCheckPoint)
			{
				i_FSocket->m_byRacingNextCheckPointNumber++;				
				if(i_FSocket->m_byRacingNextCheckPointNumber > this->m_byTotalCheckPointCounts)
				{
					i_FSocket->m_byRacingRanking = this->m_byGameRanking;
					this->m_byGameRanking++;

					if(this->m_byGameRanking > this->m_vectorJoinedUserList.size())
					{
						this->SetRacingState(RACINGSTATE_ENDING);
					}
				}
				return TRUE;
			}
			break;
		}
	}
	return FALSE;
}

BOOL CRacing::IsJoinedUserList(CFieldIOCPSocket *i_FSocket)
{
	CMTAutoCriticalSection autoCri(m_mtcsRacing.GetCriticalSectionPtr());
	
	vectorJoinedUserInfo::iterator itr(m_vectorJoinedUserList.begin());
	for(; itr != m_vectorJoinedUserList.end(); itr++)
	{
		if(i_FSocket == itr->pJoinedFieldIOCPSocket)
		{
			return TRUE;			
		}
	}

	return FALSE;
}

void CRacing::EndRacingGame(void)
{
	///////////////////////////////////////////////////////////////////////////////
	//
	INIT_MSG_WITH_BUFFER(MSG_FC_RACING_FINALRANKING, T_FC_RACING_FINALRANKING, pSMsg, SendBuf);
	pSMsg->byRankingInfoCounts = 0;
	RankingInfo *pRankkinInfo = (RankingInfo*)((BYTE*)pSMsg + sizeof(MSG_FC_RACING_FINALRANKING));

	m_mtcsRacing.Enter();
	vectorJoinedUserInfo::iterator itr(m_vectorJoinedUserList.begin());
	for(; itr != m_vectorJoinedUserList.end(); itr++)
	{
		//
	}

	m_mtcsRacing.Leave();




	///////////////////////////////////////////////////////////////////////////////
	// ���� ���̰̽����� ����ǰų� ���̽� ������ ��� ���·� �ȴ�
	m_byRacingGameNumber++;
	if(m_byRacingGameCounts < m_byRacingGameNumber)
	{
		this->SetRacingState(RACINGSTATE_STANDBY);
	}
	else
	{
		this->SetRacingState(RACINGSTATE_READY);
	}
}