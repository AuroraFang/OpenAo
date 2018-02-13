// ArenaManager.h: interface for the CArenaManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARENAMANAGER_H__79160014_CA51_4F71_9ABC_E7D25DD5EFC7__INCLUDED_)
#define AFX_ARENAMANAGER_H__79160014_CA51_4F71_9ABC_E7D25DD5EFC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CArenaManager  
{
public:
	CArenaManager();
	virtual ~CArenaManager();
	
	// ���̵� �� ����
	void ConevertArenaRenderUserName(int nArenaState, char* pSrcDst);

	// �Ʒ��� ���� ������Ʈ
	void SetArenaState(int nState);
	int GetArenaState();	

	// �Ʒ��� ���� ����
	void SetArenaServerInfo(MSG_FC_CONNECT_ARENASERVER_INFO* pMsg);
	MSG_FC_CONNECT_ARENASERVER_INFO* GetArenaServerInfo();

	// �Ʒ��� ĳ���� ����
	void SetArenaCharacterInfo(CHARACTER	 *pAFSCharacter);
	CHARACTER*	GetArenaCharacterInfo();

	// ���� ���� ���� ����
	void SetMFSMyShuttleInfo(CHARACTER *pMyShuttleInfo);
	CHARACTER* GetMFSMyShuttleInfo();	

	// GM�� �Ʒ��� ������ ���ӽõ���?
	void SetGmModeConnect(BOOL bGmModeConnect);
	BOOL IsGmModeConnect();
	
	// �Ʒ��� ���� ����� �����Ѵ�.
	void FieldSocketArenaServerSServerGroupForClient(MSG_FC_CONNECT_ARENASERVER_SSERVER_GROUP_FOR_CLIENT* pMsg);
	// �Ʒ��� ���� ����� �˷��ش�.
	BOOL GetArenaId_To_ArenaServerName(char* pArenaId, char* pDstServerName);

	// �Ʒ��� ������ ������ �ݴ´�.
	void SetForceCloseArenaSocket(BOOL bCloseArenaSocket);
	BOOL IsForceCloseArenaSocket();

	// �Ʒ��� ��������Ʈ
	void SetInflPoint(int nInflPoint);
	int GetInflPoint();

private:
	// �Ʒ��� ����
	int					m_nArenaState;	// �Ʒ��� ����
	MSG_FC_CONNECT_ARENASERVER_INFO*	m_pInfoArenaServer;	// �Ʒ��� ���� ��������
	CHARACTER*			m_pAFSCharacter;	// �Ʒ��� ĳ���� ����	
	CHARACTER*			m_pMFSCharacter;	// ���μ��� ĳ���� ����
	BOOL				m_bGmModeConnect;	// Gm�� �Ʒ��� ������ ���ӽõ�
	vector<SSERVER_GROUP_FOR_CLIENT>		m_vecArenaServerInfo;	 // �Ʒ��� �������
	BOOL				m_bForceCloseArenaSocket;	// �Ʒ��� ������ ������ �ݴ´�.

	// 2008-04-15 by bhsohn �Ʒ��� ������Ʈ ��� �ý��� ����
	int					m_nInfPoint;
	
};

#endif // !defined(AFX_ARENAMANAGER_H__79160014_CA51_4F71_9ABC_E7D25DD5EFC7__INCLUDED_)
