// Racing.h: interface for the CRacing class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RACING_H__D43339AC_2F2D_474C_84C1_EAE90C6E3AA8__INCLUDED_)
#define AFX_RACING_H__D43339AC_2F2D_474C_84C1_EAE90C6E3AA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MTCriticalSection.h"

typedef enum
{
	RACINGSTATE_STANDBY		= 0,			// �ܼ��� ���̰̽����� �����ϴ»���
	RACINGSTATE_READY		= 1,			// �ܼ��� ���̰̽����� ����Ǵ� ����
	RACINGSTATE_INVITING	= 2,			// ��û�ڸ� �޴� ����
	RACINGSTATE_INVITIED	= 3,			// ��û�� �Ϸ��, ��� ���̸̽ʿ� ���� �Ϸ�Ǹ� RACINGSTATE_COUNTDOWN �� �����
	RACINGSTATE_COUNTDOWN	= 4,			// ��û�� �Ϸ�ǰ� ī��Ʈ �ٿ��� ���۵� ����
	RACINGSTATE_STARTED		= 5,			// ������ ���۵� ����
	RACINGSTATE_ENDING		= 6				// ������ ����ó�� ����
} RacingState;

class CFieldIOCPSocket;
typedef vector<CFieldIOCPSocket*>			vectorFieldIOCPSocketPtr;

typedef struct
{
	char				szJoinedCharacterName[SIZE_MAX_CHARACTER_NAME];
	CFieldIOCPSocket	*pJoinedFieldIOCPSocket;
} RacingJoinedUserInfo;
typedef vector<RacingJoinedUserInfo>		vectorJoinedUserInfo;

class CFieldMapChannel;
class CRacing  
{
	friend CFieldMapChannel;
public:
	CRacing();
	virtual ~CRacing();

	void SetRacingState(RacingState i_rState);
	
	void LockRacing(void){		m_mtcsRacing.Enter();}
	void UnlockRacing(void){	m_mtcsRacing.Leave();}

	void PutWaitingRoomUserList(CFieldIOCPSocket *i_FSocket);
	void PopWaitingRoomUserList(CFieldIOCPSocket *i_FSocket);

	int PutJoinedUserList(CFieldIOCPSocket *i_FSocket);
	void PopJoinedUserList(CFieldIOCPSocket *i_FSocket);

	void Send2WaitingRoomUserList(BYTE *pData, int nDataSize);
	void Send2JoinedRacingUserList(BYTE *pData, int nDataSize);

	BOOL CheckPoint(CFieldIOCPSocket *i_FSocket, BYTE i_byCheckPoint);
	BOOL IsJoinedUserList(CFieldIOCPSocket *i_FSocket);
	void EndRacingGame(void);

protected:
	CMTCriticalSection			m_mtcsRacing;
	RacingState					m_racingState;							// 
	DWORD						m_dwLastChangeStateTick;
	
	BYTE						m_byRacingSystemStartHour;				// ���� �ð�����(���� 4�ð��� ���ӻ󿡼� 1����) ���̽� ���۽ð�(����:�ð�)
	BYTE						m_byRacingSystemEndHour;				//
	
	BYTE						m_byMaxUserCounts;						// �ִ� �����ڼ�
	BYTE						m_byMinUserCounts;						// �ּ� �����ڼ�
	BYTE						m_byJoinedUserCounts;					// ���̽� ���ӿ� ������ ����ڼ�
	BYTE						m_byTotalCheckPointCounts;				// ���̽� ���� üũ ����Ʈ ��
	BYTE						m_byRacingGameCounts;					// �� ����� ���̽� ���� ī��Ʈ
	BYTE						m_byRacingGameNumber;					// ����� Ȥ�� ����ǰ� �ִ� ���̽� ��ȣ
	BYTE						m_byGameRanking;
	int							m_nEntryFee;							// ������(����:spi)	
	int							m_nLimitTimeForRacing;					// Racing�� ����Ǵ� ���ѽð�(����:��)
	int							m_nRamainTimeForJoinRacing;				// ���� ���̽��� ����Ǳ� ���� ���� �ð�, ���ǽð�(����:��)	
	int							m_nCountdownCounts;

	vectorFieldIOCPSocketPtr	m_vectorWaitingRoomUserList;			// ���� ���ǿ� ���� ����� ����Ʈ
	vectorJoinedUserInfo		m_vectorJoinedUserList;					// ���� ������ ����� ����Ʈ
};

#endif // !defined(AFX_RACING_H__D43339AC_2F2D_474C_84C1_EAE90C6E3AA8__INCLUDED_)
