// NPCIOCP.h: interface for the CNPCIOCP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCIOCP_H__89992FF9_ADF7_4FFF_A639_F9BC79F85619__INCLUDED_)
#define AFX_NPCIOCP_H__89992FF9_ADF7_4FFF_A639_F9BC79F85619__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IOCP.h"
#include "AtumDBManager.h"
#include "NPCIOCPSocket.h"
#include "MapWorkspace.h"
#include "NPCMapChannel.h"		// 2008-12-02 by cmkwon, NPCServer Ŭ���̾�Ʈ ���� ���� ���� - 

// start 2011-05-23 by hskim, ���Ǵ�Ƽ 3�� - ��������Ʈ ����
#include "WayPointManager.h"

class CWayPointManager;
// end 2011-05-23 by hskim, ���Ǵ�Ƽ 3�� - ��������Ʈ ����

class CNPCTickManager;
class CNPCIOCP : public CIOCP
{
	friend class CNPCIOCPSocket;

public:
	CNPCIOCP(int nPort, char *szLocalIP = "127.0.0.1");
	virtual ~CNPCIOCP();
	ez_map<INT, ITEM*> * GetPtrMapItemInfo(void){		return &m_mapItemInfo;}

	inline CNPCMapWorkspace* GetMapWorkspace();
public:
	inline CNPCIOCPSocket* GetNPCIOCPSocket(int idx);
	BOOL OpenUDPPortForOtherServer(void);

	void LoadItemInfo();
	ITEM* GetItemInfo(int ItemNum);
	MONSTER_INFO *GetMonsterInfo(int i_nMonsterUnitKind);
	void SetSummonJacoMonster(BYTE i_byBelligerence, BOOL i_bIsSummon);
	BOOL CheckSummonJacoMonster(BYTE i_byBelligerence);

	// Monitor ���� �Լ�
	CNPCMapChannel * GetNPCMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX MapChannIndex);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-04-16 by cmkwon, ���� ��� �� ���� ��ȯ �̺�Ʈ �ý��� ���� - 
	mtvectSEVENT_MONSTER	m_EventMonsterList;				// 2008-04-16 by cmkwon, ���� ��� �� ���� ��ȯ �̺�Ʈ �ý��� ���� - 
	void SetEventMonsterList(mtvectSEVENT_MONSTER *i_pEvMonList);
	BOOL AddCheckSummonEventMonster(SEVENT_MONSTER *i_pEventMonster, MAP_CHANNEL_INDEX i_mapChannIdx, MONSTER_INFO *i_pMonInfo);
	BOOL GetSummonEventMonsterListAfterDead(mtvectSSUMMON_EVENT_MONSTER *o_pSummonEvMonList, MAP_CHANNEL_INDEX i_mapChannIdx, MONSTER_INFO *i_pMonInfo);

protected:
	CNPCMapWorkspace		*m_pNPCMapWorkspace;

	// 2006-04-18 by cmkwon
	bool					m_bSummonVCNJacoMonster;		// 2006-04-18 by cmkwon
	bool					m_bSummonANIJacoMonster;		// 2006-04-18 by cmkwon


public:
	ez_map<int, MONSTER_INFO>	m_mapMonsterParameter;
	ez_map<INT, ITEM*>		m_mapItemInfo;

	vectorCLIENT_INFO			m_vectorClientInfo;			// 2008-12-02 by cmkwon, NPCServer Ŭ���̾�Ʈ ���� ���� ���� - CNPCMapChannel �� �ִ� ���� �̰����� �̵���.
	CLIENT_INFO* GetClientInfoO(int i_Characteridx);		// 2008-12-02 by cmkwon, NPCServer Ŭ���̾�Ʈ ���� ���� ���� - 

	BOOL IsExistClient(MAP_CHANNEL_INDEX i_mapChannIdx);	// 2008-12-03 by cmkwon, NPCServer Ŭ���̾�Ʈ ���� ��ä�� ó�� ���� - 

	ez_map<INT, vectHPAction>	m_mapHPAction;					// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - HPAction �ε�
	
	// start 2011-05-23 by hskim, ���Ǵ�Ƽ 3�� - ��������Ʈ ����
	CWayPointManager		*m_pWayPointManager;
	// end 2011-05-23 by hskim, ���Ǵ�Ƽ 3�� - ��������Ʈ ����
	
public:
	virtual BOOL IOCPInit(void);
	virtual void IOCPClean(void);
	virtual SThreadInfo *CheckIOCPThread(DWORD i_dwThreadIdToExclude);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-08 by cmkwon, �������� �ʷε��� AlivePacket ���� �߰� - 
	virtual void SendAlivePacket2OtherServers(void);
};

CNPCIOCPSocket* CNPCIOCP::GetNPCIOCPSocket(int idx)
{
	return (CNPCIOCPSocket*)GetIOCPSocket(idx);
}

CNPCMapWorkspace* CNPCIOCP::GetMapWorkspace()
{
	return m_pNPCMapWorkspace;
}
#endif // !defined(AFX_NPCIOCP_H__89992FF9_ADF7_4FFF_A639_F9BC79F85619__INCLUDED_)
