// ArenaDeathMatchRoomManager.h: interface for the CArenaDeathMatchRoom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARENADEATHMATCHROOMMANAGER_H__F7CC603F_62DA_4359_9489_93A4B3035E99__INCLUDED_)
#define AFX_ARENADEATHMATCHROOMMANAGER_H__F7CC603F_62DA_4359_9489_93A4B3035E99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFieldIOCP;
class CFieldIOCPSocket;
class CArenaDeathMatchRoom
{
public:
	CArenaDeathMatchRoom();
	virtual ~CArenaDeathMatchRoom();

	void				InitArenaDeathMatchRoom(CFieldIOCP * i_pFIOCP);		// 2007-04-19 by dhjin, ������ġ �ʱ�ȭ
	
	void				SetDeathMatchMap(MAP_CHANNEL_INDEX i_SMap);			// 2007-04-19 by dhjin, �� �Ҵ�
	void				SetVCNDeathCount(BYTE i_VCNDeathCount);				// 2007-04-19 by dhjin, ���������� DeathCount �Ҵ�
	void				SetANIDeathCount(BYTE i_ANIDeathCount);				// 2007-04-19 by dhjin, �˸��� DeathCount �Ҵ�
	void				SetVCNTeam(vectorClientIndex * i_pVCNTeam, int i_nTeamNum);		// 2007-04-19 by dhjin, ������������ ������ �Ҵ�
	void				SetANITeam(vectorClientIndex * i_pANITeam, int i_nTeamNum);		// 2007-04-19 by dhjin, ������������ ������ �Ҵ�
	void				SetCreateRoomTime();								// 2007-04-20 by dhjin, ����� �ð� ����
	void				SetDeathMatchRoomNum(INT i_nDeathMatchRoomNum);		// 2007-04-24 by dhjin, ������ġ �� ��ȣ �Ҵ�
	void				SetPayInfo(SARENA_PAY_INFO * i_PayInfo);				// 2007-04-24 by dhjin, ���� �� ����
	BYTE				GetDeathMatchState(void);							// 2007-05-21 by cmkwon
	INT					GetVCNTeamNum(void);								// 2007-05-21 by cmkwon
	INT					GetANITeamNum(void);								// 2007-05-21 by cmkwon

	void				DecreaseDeathCount(BYTE i_nInfl);					// 2007-04-17 by dhjin, DeathCount -1
	
	INT					GetDeathMatchRoomNum();								// 2007-04-24 by dhjin, ������ġ �� ��ȣ ��������
	ATUM_DATE_TIME*		GetCreateRoomTime();								// 2007-04-23 by dhjin,
	MAP_CHANNEL_INDEX*	GetMapInfo();										// 2007-04-24 by dhjin, �� ���� ������

	BOOL				CheckWinCondition();								// 2007-04-17 by dhjin, �¸� ���� üũ
	void				DoMinutelyWorkArenaDeathMatchRoom(ATUM_DATE_TIME *pDateTime);		// 2007-04-20 by dhjin,
	void				WarStart();											// 2007-04-20 by dhjin, ���� �������� 1���� ���� ���� ������ �Ѵ�.
	void				BeforeWarFinish();									// 2007-04-30 by dhjin, ������ �ð��� 1���� ������ ��� ����
	void				WarFinish(BYTE i_byEndType);						// 2007-04-20 by dhjin, ������ ���а� ������ ��쿡�� ó���Ѵ�
	void				WarFinishDraw(BYTE i_byEndType);					// 2007-04-24 by dhjin, ������ ����� ��
//	INT					MakeMSG_FC_ARENA_ROOM_WAR_FINISH(SARENA_ROOM_FINISH_INFO * o_pArenaRoomFinishInfo);		// 2007-04-24 by dhjin, ���� ���� ���� ������
	
	void				AllUserWarp();										// 2007-04-20 by dhjin, ������ ��� ���� ������Ų��
	void				ReadyUserWarp(CFieldIOCPSocket * i_pFISock);		// 2007-04-24 by dhjin, ������ ������Ű�� ���� ������ ���� �۾��� �Ѵ�

	BOOL				CheckCFieldIOCPSocketValid(CFieldIOCPSocket * i_pFISock, BYTE i_nArenaState);		// 2007-05-03 by dhjin, ��ȿ�� �������� üũ�Ѵ�.
	void				SetPlayLimitedTime(BYTE i_nPlayLimitedTime);		// 2007-05-04 by dhjin, ��� ���� �ð� ����

	BYTE				GetVCNTeamSize();									// 2007-05-04 by dhjin, ������������ �ο���
	BYTE				GetANITeamSize();									// 2007-05-04 by dhjin, �˸����� �ο���
	BYTE				GetWatchUserSize();									// 2007-07-04 by dhjin, ������ �ο���

	void				CheckLiveMemberByDoMinutelyWorkArena();				// 2007-05-04 by dhjin, 1�� ���� ��ȿ�� �������� üũ�Ѵ�
	BOOL				InitDeathMatchRoomState();							// 2007-05-07 by dhjin, �ʱ�ȭ ���� ���� ����

	BOOL				CheckEmptyTeamSize();								// 2007-05-08 by dhjin, �� �� ��� ���� ������ üũ
	int					Send2ArenaDeathRoom(BYTE i_nArenaState, vectorClientIndex *i_pVectCIdx, BYTE *i_pSendBuf, int i_nSendBytes, BYTE i_nUpdateArenaState = FALSE);
	int					Send2ArenaDeathRoom(BYTE i_nArenaState, vectorClientIndex *i_pVectCIdx, MessageType_t i_msgTy);
	mtvectorClientIndex_t * GetVectByInfl(BYTE i_nInfl);					// 2007-05-08 by dhjin, ���¿� ���� �� ������ ������

	void				InsertWatchUserList(ClientIndex_t i_nClientIdx);	// 2007-06-15 by dhjin, �����ϴ� ���� ����Ʈ�� �߰�
	void				DeleteWatchUserList(ClientIndex_t i_nClientIdx);	// 2007-06-15 by dhjin, �����ϴ� ���� ����Ʈ�� ����
	void				PopAllWatchUserObserver();							// 2007-06-18 by dhjin, �Ʒ����� ������ ��� ������ ���������� �����ڸ� �����Ѵ�.
	void				SendWatchStartInfo();								// 2007-06-18 by dhjin, �����ϴ� ���� ���� ������
	INT					MakeMSG_FC_ARENA_WATCH_START(SARENA_WATCH_START_USER_INFO * o_pArenaWatchStartUserInfo);
	void				SendWatchRemoveUser(ClientIndex_t i_nClientIdx);	// 2007-06-18 by dhjin, �Ʒ������� ���� ���� ������ �����ڿ��� �����Ѵ�.
	void				PopWatchUserObserver(CFieldIOCPSocket * i_pFISoc, vectorClientIndex *i_pVectCIdx);			// 2007-06-21 by dhjin, ������ ����
	void				ADDWatchUserObserver(CFieldIOCPSocket * i_pFISoc, vectorClientIndex *i_pVectCIdx);			// 2007-06-21 by dhjin, ������ �߰�

	//void				SetVCNMFSID(USHORT i_VCNMFSID);						// 2008-01-10 by dhjin, �Ʒ��� ���� - VCN�ʵ弭��ID ���� 	
	//void				SetANIMFSID(USHORT i_ANIMFSID);						// 2008-01-10 by dhjin, �Ʒ��� ���� - ANI�ʵ弭��ID ���� 	
	//USHORT			GetVCNMFSID();										// 2008-01-10 by dhjin, �Ʒ��� ���� - VCN�ʵ弭��ID	�������� 
	//USHORT			GetANIMFSID();										// 2008-01-10 by dhjin, �Ʒ��� ���� - ANI�ʵ弭��ID	��������
	void				SetVCNMFSInfl(BYTE i_VCNMFSInfl);					// 2008-02-19 by dhjin, �Ʒ��� ���� - VCN�� MFS Influence ���� 	
	void				SetANIMFSInfl(BYTE i_ANIMFSInfl);					// 2008-02-19 by dhjin, �Ʒ��� ���� - ANI�� MFS Influence ���� 	
	BYTE				GetVCNMFSInfl();									// 2008-02-19 by dhjin, �Ʒ��� ���� - VCN�� MFS Influence �������� 
	BYTE				GetANIMFSInfl();									// 2008-02-19 by dhjin, �Ʒ��� ���� - ANI�� MFS Influence ��������

	float				m_fVCNTeamDamage;		// 2012-07-10 by jhseol, �Ʒ��� ���� - ������ ����
	float				m_fANITeamDamage;		// 2012-07-10 by jhseol, �Ʒ��� ���� - ������ ����

	void				ArenaFinish();			// 2012-07-11 by jhseol, �Ʒ��� �������� ��ɾ� �߰�

protected:
	CFieldIOCP			*m_pFieldIOCP17;
	INT					m_DeathMatchRoomNum;								// 2007-04-24 by dhjin, ������ġ �� ��ȣ 
	MAP_CHANNEL_INDEX	m_map;												// 2007-04-17 by dhjin, ������
	ATUM_DATE_TIME		m_CreateRoomTime;									// 2007-04-17 by dhjin, �� ���� �ð�
	ATUM_DATE_TIME		m_FinishRoomTime;									// 2007-04-17 by dhjin, �� ���� �ð�
	BYTE				m_VCNDeathCount;									// 2007-04-17 by dhjin, ���������� ���� DeathCount
	BYTE				m_ANIDeathCount;									// 2007-04-17 by dhjin, �˸��� ���� DeathCount
	BYTE				m_MaxVCNDeathCount;									// 2007-04-17 by dhjin, ���������� �ְ� DeathCount
	BYTE				m_MaxANIDeathCount;									// 2007-04-17 by dhjin, �˸��� �ְ� DeathCount
	mtvectorClientIndex_t	m_mtvectVCNTeam;								// 2007-04-17 by dhjin, ������������
	mtvectorClientIndex_t	m_mtvectANITeam;								// 2007-04-17 by dhjin, �˸�����
	INT					m_VCNTeamNum;										// 2007-05-04 by dhjin,	���������� �� ��ȣ
	INT					m_ANITeamNum;										// 2007-05-04 by dhjin, �˸��� �� ��ȣ

	BYTE				m_DeathMatchState;									// 2007-04-20 by dhjin, ������ġ�� ����

	SARENA_PAY_INFO		m_PayInfo;											// 2007-04-24 by dhjin, ���� ������

	BYTE				m_PlayLimitedTime;									// 2007-05-04 by dhjin, ��� ���� �ð�
	BOOL				m_InitDeathMatchRoom;								// 2007-05-07 by dhjin, �ʱ�ȭ ���� ����, ��Ⱑ ������ TRUE�� �����Ͽ� �ʱ�ȭ �Ѵ�

	mtvectorClientIndex_t	m_mtvectWatchUserList;							// 2007-06-15 by dhjin, �����ϴ� ���� ����Ʈ
	
	//USHORT			m_VCNMFSID;											// 2008-01-10 by dhjin, �Ʒ��� ���� - VCN�ʵ弭��ID	
	//USHORT			m_ANIMFSID;											// 2008-01-10 by dhjin, �Ʒ��� ���� - ANI�ʵ弭��ID	
	USHORT				m_VCNMFSInfl;										// 2008-02-19 by dhjin, �Ʒ��� ���� - VCN�� MFS Influence	
	USHORT				m_ANIMFSInfl;										// 2008-02-19 by dhjin, �Ʒ��� ���� - ANI�� MFS Influence

	float				m_fVCNTeamPercent;		// 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ð��ʰ��� ���� VCN�� ��ü �ۼ�Ʈ �� 
	float				m_fANITeamPercent;		// 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ð��ʰ��� ���� ANI�� ��ü �ۼ�Ʈ �� 

};

#endif // !defined(AFX_ARENADEATHMATCHROOMMANAGER_H__F7CC603F_62DA_4359_9489_93A4B3035E99__INCLUDED_)
