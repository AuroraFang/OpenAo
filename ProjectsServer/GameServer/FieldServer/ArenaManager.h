// ArenaManager.h: interface for the CArenaManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARENAMANAGER_H__B0380168_ECFE_43F2_8AA8_DA0828F3EE57__INCLUDED_)
#define AFX_ARENAMANAGER_H__B0380168_ECFE_43F2_8AA8_DA0828F3EE57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumProtocol.h"

class CFieldIOCP;
class CFieldIOCPSocket;
class CArenaTeam;
class CArenaDeathMatchRoom;
class CArenaTickManager;

typedef mt_map<INT, CArenaTeam*>								mtmapINT2ArenaTeamManager;
typedef mt_map<INT, CArenaDeathMatchRoom*>						mtmapINT2ArenaDeathMatchRoom;

class CArenaManager  
{
public:
	CArenaManager();
	virtual ~CArenaManager();
	
	void	InitArenaManager(CFieldIOCP *i_pFIOCP);															// 2007-04-17 by dhjin, �ʱ�ȭ
	void	InitDBArenaInfo(vectorSDBARENA_INFO* i_pvectorDBArenaInfo);										// 2007-04-17 by dhjin, ArenaInfo�� ����
	void	InitDBArenaMapInfo(vectorSDBARENA_MAPINFO* i_pvectorDBArenaMapInfo);							// 2007-04-17 by dhjin, ArenaMapInfo�� ����
	
	void	SetTotalMapCount(int i_nTotalMapCount);															// 2007-04-30 by dhjin, �Ʒ��� �� �� ��
	void	SetTeamState(int i_nTeamNum, BYTE i_nTeamState);												// 2007-05-04 by dhjin, �Ʒ��� ���� ���¸� �����Ѵ�

	void	SendRequestTeamInfo(CFieldIOCPSocket * i_pFis, BYTE i_nTeamMode
								, int i_nStartNum, BYTE i_byRequetTeamState									// 2007-04-23 by dhjin, �� ��� ��û
								, MapIndex_t i_byRequestArenaMap);											// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ������ �Ʒ��� �� �Ķ���� �߰�
								
	INT		MakeMSG_FC_ARENA_REQUEST_TEAMLIST_OK(SARENA_TEAM_INFO *o_pArenaTeamInfo
												, CFieldIOCPSocket * i_pFis, BYTE i_nTeamMode
												, int i_nStartNum, int * o_nTotalCount
												, BYTE i_byRequetTeamState									// 2007-04-23 by dhjin,
												, MapIndex_t i_byRequestArenaMap);							// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ������ �Ʒ��� �� �Ķ���� �߰�

	INT		MakeMSG_FC_ARENA_TEAM_MEMBERLIST_OK(SARENA_TEAM_CHARACTERINFO * o_pArenaTeamMemberInfo
												, vectorClientIndex *i_pvectClient);						// 2007-04-23 by dhjin,

	void	SendCreatTeamInfo(CFieldIOCPSocket * i_pFis);													// 2007-04-26 by dhjin, �� ������ ���� ��û�� DB������ �����Ѵ�
	INT		MakeMSG_FC_ARENA_REQUEST_CREATE_TEAMINFO(SARENA_REQUEST_CREATE_TEAMINFO *o_pArenaRequestTeamInfo);	// 2007-04-26 by dhjin, �� ������ ���� ��û�� DB���� �����
	INT		CreateTeam(MSG_FC_ARENA_CREATE_TEAM *i_pSMSG, CFieldIOCPSocket * i_pFISock);					// 2007-04-17 by dhjin, ������
	BOOL	DBCheckCreateTeam(MSG_FC_ARENA_CREATE_TEAM *i_pSMSG);											// 2007-04-25 by dhjin, �� �����ϱ����� DB���ǿ� ���������� üũ�Ѵ�
	BOOL	DeleteTeam(CArenaTeam * i_pArenaTeam);															// 2007-04-17 by dhjin, ����ü
	INT		FindEmptyTeam();																				// 2007-04-17 by dhjin, �� �� ��ȣ ã�Ƽ� ����
	CArenaTeam *	FindTeam(INT i_nTeamNum);																// 2007-04-17 by dhjin, �� ��ȣ ã�Ƽ� ����
	
	Err_t	InsertMember(MSG_FC_ARENA_ENTER_TEAM *i_pSMSG, CFieldIOCPSocket * i_pFISock);					// 2007-04-17 by dhjin, �����߰�
	Err_t	QuickInsertMember(CFieldIOCPSocket * i_pFISock, BYTE * o_byArenaMode
								, BYTE * o_byTeamSize, INT * o_nTeamNum);									// 2007-06-05 by dhjin, ���� ������ �����߰�

	Err_t	DeleteMember(CFieldIOCPSocket * i_pFISocket);													// 2007-04-17 by dhjin, ��������
	void	SendTeamMemberList(int i_nTeamNum);																// 2007-04-23 by dhjin, ���� ����
	vectorClientIndex * GetMemberList(int i_nTeamNum);														// 2007-04-18 by dhjin, ���� ����Ʈ �޾ƿ��� 
	void	SendTeamMemberState(int i_nTeamNum, BYTE i_nState);												// 2007-04-23 by dhjin, �� ���� ���� �� ���� ���� ����

	BYTE	GetTeamState(int i_nTeamNum);																	// 2007-04-19 by dhjin, �� ���� ������
	BOOL	ChectTeamFullReadyFinish(int i_nTeamNum);														// 2007-05-02 by dhjin, ������ ���� ARENA_STATE_READY_FINISH���� üũ	

	BOOL	TeamMatch(int i_nTeamNum);																		// 2007-04-17 by dhjin, ������ Ready ���°� �Ǹ� ������� ��ġ
	BOOL	FindEmptyRoom(MapIndex_t * o_nMapIndex, ChannelIndex_t * o_nChannelIndex						// 2007-04-19 by dhjin, �� �� ����
							, MapIndex_t i_nTeamAMapIndex, MapIndex_t i_nTeamBMapIndex);					// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ������ �Ʒ��� �� �Ķ���� �߰�

	BOOL	GetPayInfo(BYTE i_nTeamMode, BYTE i_nTeamSize, SARENA_PAY_INFO * o_sPayInfo);					// 2007-04-24 by dhjin, DB �������� ���� ������ ���´�

	
	BOOL    GetDeathMatchDBInfo(BYTE i_nTeamMode, BYTE i_nTeamSize
								, BYTE * o_nDeathMatchCount, BYTE * o_nPlayLimitedTime);					// 2007-04-20 by dhjin, ������ġ ���� ó�� ������ġ ī��Ʈ�� ��� ���� �ð��� ����
	BOOL	CreateDeathMatchRoom(MAP_CHANNEL_INDEX i_SMapInfo, CArenaTeam * i_pATeam
								, CArenaTeam * i_pBTeam);													// 2007-04-17 by dhjin, �Ʒ��� ������ġ ��� �� ����
	void	DeleteDeathMatchRoom(INT i_nDeathMatchRoomNum);													// 2007-04-17 by dhjin, �Ʒ��� ������ġ ��� �� ���� 
	BOOL	DeleteDeathMatchRoomByRoomNum(INT i_nDeathMatchRoomNum, MAP_CHANNEL_INDEX * o_pMapInfo);		// 2007-04-27 by dhjin, DeleteDeathMatchRoom()���� ȣ��, �Ʒ��� ������ġ ��� �� ���� �� �濡 MapIndex����.
	BOOL	DeleteArenaMapManager(MAP_CHANNEL_INDEX *  i_pMapIndex);										// 2007-04-27 by dhjin, �Ʒ��� ���� ����� m_mtvectArenaMapManager�� ���� �����Ѵ�
	
	void	SendDeathMatchEnterRoom(MSG_FC_ARENA_ENTER_ROOM *i_pSMSG
									, vectorClientIndex *i_pvectClient, INT i_nDeathMatchRoomNum);			// 2007-04-23 by dhjin, �Ʒ��� ������ġ �� ���� ���� ����
	
	void	SendDeathMatchRoomInfo(int i_nTeamNum, int i_nDeathMatchRoomNum, BYTE i_nInfl);					// 2007-04-24 by dhjin, �Ʒ��� ������ġ �� ���� ����
	void	SendDeathMatchUserWarpCity(int i_nDeathMatchRoomNum, CFieldIOCPSocket * i_pFis);				// 2007-04-24 by dhjin, �Ʒ��� ������ġ ������ ���� ���� ���� ����
	void	SendTeamChat(BYTE * i_pDATA, int i_nSize, int i_nTeamNum);										// 2007-05-02 by dhjin, �Ʒ��� ä�� ����

	void	Pay();																							// 2007-04-17 by dhjin, �Ʒ��� ���� ����

	void	DoMinutelyWorkArena(ATUM_DATE_TIME *pDateTime);													// 2007-04-20 by dhjin,
	void	WarpToDeathMatchRoom(CFieldIOCPSocket * i_pFISocket);											// 2007-05-04 by dhjin, ������ġ ������ ������ ������Ų��.

	void	CheckFullMemberByFinishRoom(int i_nTeam);														// 2007-05-04 by dhjin, �Ʒ��� ���� ������ ���� ���¸� �����ϱ� ���� ������ Ǯ���� �ƴ��� üũ�Ͽ� ���°��� �����Ѵ�
	void	SendTeamMatchIngState(int i_nTeamNum);															// 2007-05-07 by dhjin, �Ʒ��� �� ��Ī ���¸� �������� �����Ѵ�.

	CArenaTeam * GetArenaTeam(int i_nTeamNum);																// 2007-05-08 by dhjin, �� ��ȣ�� m_pArenaTeamManager�� ���� �����´�.
	CArenaDeathMatchRoom * GetArenaDeathMatchRoom(int i_nRoomNum);											// 2007-05-08 by dhjin, �� ��ȣ�� m_pArenaDeathMatchRoom�� ���� �����´�.

	BOOL	CheckTeamFullMember(int i_nTeamNum);															// 2007-05-10 by dhjin, �� �������� Ǯ���� üũ�Ѵ�.

	BYTE	GetTeamSize(int i_nTeamNum);																	// 2007-04-19 by dhjin, �� �ִ� �ο� ��������.
	
	void	SendOtherTeamInfo(CFieldIOCPSocket * i_pFISoc);													// 2007-05-23 by dhjin, ��� �� ������ �����Ѵ�.
	void	SendArenaTeamInfoByGMCommand(CFieldIOCPSocket * i_pFISoc, BYTE i_byInfl);						// 2007-05-23 by dhjin, GM��ɾ� /�Ʒ���
	
	INT		MakeMSG_FC_ARENA_REQUEST_OTHER_TEAM_INFO_OK(SARENA_REQUEST_OTHER_TEAM_INFO_OK * o_pArenaOtherTeamInfo
														, vectSARENA_OTHER_TEAM_INFO *i_pvectTeam, BYTE i_byArenaMode);	// 2007-05-25 by dhjin, ��� �� �˻�
	void	SetMSGArenaUseUser(BYTE i_byArenaState, BYTE i_byMemberCount, INT * o_ReadyUserTotalCount
								, INT * o_MatchIngUserTotalCount, INT * o_ArenaFightIngUserTotalCount);		// 2007-05-25 by dhjin, �Ʒ��� �̿��ϴ� �� �������� ���Ѵ�.

	//////////////////////////////////////////////////////////////////////////
	// 2007-06-04 by dhjin, �Ʒ��� ���� ������ ����
	void	ArenaItemSupply(CFieldIOCPSocket * i_pFISoc, INT i_nMapIndex);									// 2007-06-04 by dhjin, �Ʒ��� ���� ������ ���� ����.
	INT		GetTeamSupplyItemNum(INT i_nitemNum, BYTE i_nTeamStartLevel);									// 2007-06-04 by dhjin, �� ��޿� �´� �Ʒ��� ���� ������ ��ȣ�� �����Ѵ�.

	BOOL	InsertWatchUserListW(CFieldIOCPSocket * i_pFISoc, MAP_CHANNEL_INDEX i_MapInfo);					// 2007-06-15 by dhjin, �� ������ �ش��ϴ� �Ʒ��� �濡 ���� ������ �߰� �Ѵ�.
	void	DeleteWatchUserListW(CFieldIOCPSocket * i_pFISoc);												// 2007-06-15 by dhjin, �� ������ �ش��ϴ� �Ʒ��� �濡 ���� ������ ���� �Ѵ�.
	void	SendWatchUserATRoomInfo(CFieldIOCPSocket * i_pFISoc, MAP_CHANNEL_INDEX i_MapInfo);				// 2007-06-18 by dhjin, �Ʒ��� �ʿ� �ִ� ������ ������ �����ϴ� �������� �����Ѵ�.

	// 2008-01-09 by dhjin, �Ʒ��� ���� -
	MAP_CHANNEL_INDEX * GetArenaDeathMatchRoomMapInfo(int i_nRoomNum);						// 2008-01-09 by dhjin, �Ʒ��� ���� - ������ġ �� �� ���� ������
	BYTE	GetTeamArenaInflW(INT i_nTeamNum);							// 2008-01-16 by dhjin, �Ʒ��� ���� - �� �Ʒ��� ���� ������ ���� ��������
	BYTE	GetTeamInflW(INT i_nTeamNum);								// 2008-01-16 by dhjin, �Ʒ��� ���� - �� ���� ��������
	BOOL	SetTeamEventW(INT i_nTeamNum, BOOL i_bSet);								// 2008-03-10 by dhjin, �Ʒ��� ���� - �̺�Ʈ �Ӽ� �Ҵ��ϱ�	

protected:
	CArenaTickManager*				m_pTickManager;															// 2007-04-20 by dhjin, 

	CFieldIOCP						*m_pFieldIOCP16;
	
	mtmapINT2ArenaTeamManager		m_mtmapArenaTeamManager;
	CArenaTeam						*m_pArenaTeamManager;

	mtmapINT2ArenaDeathMatchRoom	m_mtmapArenaDeathMatchRoom;
	CArenaDeathMatchRoom			*m_pArenaDeathMatchRoom;

	mtvectSARENA_MAP_MANAGER		m_mtvectArenaMapManager;												// 2007-04-19 by dhjin, �Ʒ��� ���� ���¸� ����

	vectorSDBARENA_INFO				m_vectorDBArenaInfo;													// 2007-04-17 by dhjin, ti_ArenaInfo �� 
	vectorSDBARENA_MAPINFO			m_vectorDBArenaMapInfo;													// 2007-04-17 by dhjin, ti_ArenaMapInfo ��	

	INT								m_ReserveTeamNum;														// 2007-04-18 by dhjin, �� ��ȣ �Ҵ� ���� ��
	BYTE							m_MaxChannelPerMap;														// 2007-04-19 by dhjin, �� �ʿ� �Ҵ� �� �� �ִ� ä�� ��

	INT								m_TotalArenaMapCount;													// 2007-04-30 by dhjin, �Ʒ��� �� �� ��
};

#endif // !defined(AFX_ARENAMANAGER_H__B0380168_ECFE_43F2_8AA8_DA0828F3EE57__INCLUDED_)
