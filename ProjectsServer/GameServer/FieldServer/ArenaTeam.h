// ArenaTeamManager.h: interface for the CArenaTeam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARENATEAMMANAGER_H__74974ECC_1390_44F4_9E23_D7DEE57C4486__INCLUDED_)
#define AFX_ARENATEAMMANAGER_H__74974ECC_1390_44F4_9E23_D7DEE57C4486__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CArenaTeam  
{
public:
	CArenaTeam();
	virtual ~CArenaTeam();

	void		InitArenaTeamManager();				// 2007-04-17 by dhjin, �ʱ�ȭ
	void		SetTeamNum(INT i_nTeamNum);			// 2007-04-18 by dhjin, �� ��ȣ �Ҵ�
	void		SetTeamSize(BYTE i_nTeamSize);		// 2007-04-18 by dhjin, �� �ִ� �ο� �Ҵ�
	void		SetTeamMode(BYTE i_nTeamMode);		// 2007-04-18 by dhjin, �Ʒ��� ��� �Ҵ�
	void		SetTeamInfl(BYTE i_nTeamInfl);		// 2007-04-18 by dhjin, �� ���� �Ҵ�
	void		SetTeamLevel(BYTE i_nTeamLevel);	// 2007-04-18 by dhjin, �� �� ��� �Ҵ�
	void		SetTeamState(BYTE i_nTeamState);	// 2007-04-18 by dhjin, �� Arena ���� �Ҵ�
	void		SetTeamPW(CHAR * i_szTeamPW);		// 2007-04-18 by dhjin, �� PW �Ҵ�
	void		SetTeamLock(BOOL i_bTeamLock);		// 2007-04-18 by dhjin, �� ���� ���� �Ҵ�
	//void		SetTeamServerID(INT i_nTeamServerID);	// 2008-01-03 by dhjin, �Ʒ��� ���� - �� ���� ID �Ҵ�
	void		SetTeamArenaInfl(BYTE i_nTeamArenaInfl);	// 2008-01-16 by dhjin, �Ʒ��� ���� - �Ʒ������� ���Ǵ� �� ���� �Ҵ�
	void		SetTeamEvent(BOOL i_bTeamEvent);	// 2008-03-10 by dhjin, �Ʒ��� ���� - �̺�Ʈ �Ӽ� �ο�
	void		SetTeamSelectArenaMapNum(MapIndex_t i_nTeamSelectArenaMapNum);	// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ������ �Ʒ��� ��

	INT			GetTeamNum();						// 2007-04-18 by dhjin, �� ��ȣ �������� 
	BYTE		GetTeamSize();						// 2007-04-18 by dhjin, �� �ִ� �ο� ��������
	BYTE		GetCurrentTeamSize();				// 2007-04-25 by dhjin, ���� �� �ο� ��������
	BYTE		GetTeamMode();						// 2007-04-18 by dhjin, �Ʒ��� ��� ��������
	BYTE		GetTeamInfl();						// 2007-04-18 by dhjin, �� ���� ��������
	BOOL		CheckTeamLevel(int i_nLevel);		// 2007-04-18 by dhjin, ������ �� ��޿� �´��� üũ
	BYTE		GetTeamState();						// 2007-04-18 by dhjin, �� Arena ���� ��������
	CHAR *		GetTeamPW();						// 2007-04-18 by dhjin, �� PW ��������
	BOOL		GetTeamLock();						// 2007-04-18 by dhjin, �� ���� ���� ��������
	BYTE		GetTeamStartLevel();				// 2007-04-19 by dhjin, �� �� ���� ���� ��������
	BYTE		GetTeamEndLevel();					// 2007-04-19 by dhjin, �� �� ���� ���� ��������
	ClientIndex_t GetTeamFirstCharacterClientIndex();	// 2007-04-23 by dhjin, ���� ù ��° ���� ��������
	//INT		GetTeamServerID();					// 2008-01-03 by dhjin, �Ʒ��� ���� - �� ���� ID ��������
	BYTE		GetTeamArenaInfl();					// 2008-01-16 by dhjin, �Ʒ��� ���� - �Ʒ������� ���Ǵ� �� ���� ��������
	BOOL		GetTeamEvent();						// 2008-03-10 by dhjin, �Ʒ��� ���� - �̺�Ʈ �Ӽ� ��������
	MapIndex_t	GetTeamSelectArenaMapNum();			// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ������ �Ʒ��� ��

	void		InsertTeamList(ClientIndex_t i_nClientIdx);				// 2007-04-17 by dhjin, ���� �߰�
	void		DeleteTeamList(ClientIndex_t i_nClientIdx);				// 2007-04-17 by dhjin, ���� ����
	vectorClientIndex *		GetTeamList();					// 2007-04-17 by dhjin, ���� ��������
	BOOL		CheckFullMember();				// 2007-04-17 by dhjin, ������ �ִ� �ο����� üũ�ϰ� �ִ� �ο��̸� ���� ��Ī ��Ų��.
	BOOL		CheckEmptyMember();				// 2007-04-17 by dhjin, ������ �� �� �������� �ʴ��� üũ�ϰ� �� �� �������� ������ �� Ŭ������ �����Ѵ�.

	void		SetTeamReadyFinishTime();		// 2007-05-11 by dhjin, ���� �غ� �Ϸ�Ǹ� �ð� ���� �����Ѵ�
	ATUM_DATE_TIME * GetTeamReadyFinishTime();	// 2007-05-11 by dhjin, ���� �غ� �Ϸ�� �ð��� �����´�

	void		SetTeamJoinRankTime();			// 2007-06-05 by dhjin, ���� ���� ���� �ð� ����
	ATUM_DATE_TIME * GetTeamJoinRankTime();		// 2007-06-05 by dhjin, ���� ���� ���� �ð��� �����´�

protected:
	INT			m_TeamNum;						// 2007-04-17 by dhjin, �� ��ȣ
	BYTE		m_TeamSize;						// 2007-04-17 by dhjin, ���� ���� �� �� �ִ� �ִ� �ο�
	BYTE		m_TeamMode;						// 2007-04-17 by dhjin, �Ʒ��� �� ���
	BYTE		m_TeamInfl;						// 2007-04-17 by dhjin, �� ����
	BYTE		m_TeamStartLevel;				// 2007-04-17 by dhjin, �� �� ���� ���
	BYTE		m_TeamEndLevel;					// 2007-04-17 by dhjin, �� �� ������ ���
	BYTE		m_TeamState;					// 2007-04-17 by dhjin, �� Arena ����(ARENA_STATE_XXX)
	vectorClientIndex	m_vectTeamList;			// 2007-04-17 by dhjin, ���� ����
	CHAR		m_TeamPW[SIZE_MAX_TEAM_PW];		// 2007-04-17 by dhjin, ���� ������ �� �ִ� PW
	BOOL		m_TeamLock;						// 2007-04-17 by dhjin, ���� �������� ��������� üũ
	ATUM_DATE_TIME		m_TeamReadyFinishTime;	// 2007-05-11 by dhjin, ���� �غ� �Ϸ�� �ð�
	ATUM_DATE_TIME		m_TeamJoinRankTime;		// 2007-06-05 by dhjin, ���� ���� ������ �����ϱ� ���� �ð�
	//INT			m_TeamServerID;					// 2008-01-03 by dhjin, �Ʒ��� ���� - �� ���� ID
	BYTE		m_TeamArenaInfl;				// 2008-01-16 by dhjin, �Ʒ��� ���� - �Ʒ������� ���Ǵ� �� ����
	BOOL		m_TeamEvent;					// 2008-03-10 by dhjin, �Ʒ��� ���� - �̺�Ʈ �Ӽ� �ο�, �̺�Ʈ �Ӽ� �ο� ������ ��Ī �����ϵ���
	MapIndex_t	m_TeamSelectArenaMapNum;		// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ������ �Ʒ��� ��
};

#endif // !defined(AFX_ARENATEAMMANAGER_H__74974ECC_1390_44F4_9E23_D7DEE57C4486__INCLUDED_)
