// IMIOCP.h: interface for the CIMIOCP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMIOCP_H__89992FF9_ADF7_4FFF_A639_F9BC79F85619__INCLUDED_)
#define AFX_IMIOCP_H__89992FF9_ADF7_4FFF_A639_F9BC79F85619__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IOCP.h"
#include "ServerGroup.h"
#include "Guild.h"
#include "IMTickManager.h"
#include "ChatBlockManager.h"
//#include "MersenneTwister.h"					// 2008-06-04 by dhjin, EP3 ���� �Լ� �߰�
#include "ChatRoomManager.h"					// 2008-06-09 by dhjin, EP3 ä�ù� - 

///////////////////////////////////////////////////////////////////////////////
// Party(��� ����)
struct BelongedPartyInfo	// �Ҽӵ� ����� ����
{
	UID32_t		CharacterUniqueNumber;		// �ش� ĳ���� ��Ƽ ����
	UID32_t		PartyMasterUniqueNumber;	// ��Ƽ(��) ���� ��ȣ
	TimeUnit_t	PartyCreationTime;			// ��Ƽ ���� �ð�
};

class CAtumIMDBManager;
class CIMIOCPSocket;
class CPreIMWinSocket;
class CIMParty;
class CChatRoomManager;						// 2008-06-09 by dhjin, EP3 ä�ù� - 
class CANoticeManager;				// 2009-01-14 by cmkwon, ��� �ڵ� ���� �ý��� ���� - 
//class CSecurityManager;						// 2011-06-22 by hskim, �缳 ���� ����

class CIMIOCP : public CIOCP
{
public:
	CIMIOCP(int nPort, char *szLocalIP = "127.0.0.1");
	virtual ~CIMIOCP();

public:
	CAtumIMDBManager				*m_pAtumIMDBManager;			// DB manager
	CIMTickManager					*m_pIMTickManager;				// Tick manager
	//CSecurityManager				*m_pSecurityManager;			// 2011-06-22 by hskim, �缳 ���� ����
	mt_map<string, CIMIOCPSocket*>	m_mAccountName;					// AccountName�� Ű�� �̷���� Client Map
	mt_map<UID32_t, CIMIOCPSocket*>	m_MapAccountUniqueNumber;
	mt_map<string, CIMIOCPSocket*>	m_mCharacterName;				// CharacterName�� Ű�� �̷���� Client Map
	mt_map<UID32_t, CIMIOCPSocket*>	m_MapChactarterUniqueNumber;
	CPreIMWinSocket					*m_pPreWinSocket;
public:
	CIMIOCPSocket* GetIMIOCPSocket(int idx);
	CIMIOCPSocket* GetIMIOCPSocketByCharacterUID(UID32_t i_characterUID);
	CIMIOCPSocket* GetIMIOCPSocketByCharacterName(char *i_szCharacterName);
	CIMIOCPSocket* GetIMIOCPSocketByAccountUID(UID32_t i_accountUID);
	CHARACTER* GetCharacterByArrayIndex(int idx);
	BOOL IsConnectedUser(char* AccountName, CIMIOCPSocket **ppSock);
	BOOL SendMessageToFieldServer(BYTE *buffer, int size);

	///////////////////////////////////////////////////////////////////////////
	// �����ڿ� ���� ����Ʈ ���� �Լ�
	void SendMessageToAllAdmin(BYTE *buffer, int size);
	void SendString128ToAllAdmin(BYTE i_byString128Ty, char *i_szString);		// 2007-11-19 by cmkwon, �����ý��� ������Ʈ - 
	BOOL AddToAdminCharacter(UID32_t AdminCharacterUniqueNumber);
	BOOL DeleteFromAdminCharacter(UID32_t AdminCharacterUniqueNumber);

	///////////////////////////////////////////////////////////////////////////
	// ���� ����
	void SendMessageToAllClients(BYTE *buffer, int size);
//	void SendChatMsgToAllClients(BitFlag8_t i_chatType, BYTE *i_pChat, int i_nSize, BYTE i_byInfluenceMask=INFLUENCE_TYPE_ALL_MASK);
	void SendChatMsgToAllClients(BitFlag16_t i_chatType, BYTE *i_pChat, int i_nSize, BYTE i_byInfluenceMask=INFLUENCE_TYPE_ALL_MASK);	// 2008-05-15 by dhjin, EP3 - ä�� �ý��� ����
	int SendMsg2MultiUser(vectCIMIOCPSocketPtr *i_pvectCIISockPtrList, BYTE *i_pbyBuf, int i_nLen);
	int SendMsg2MultiUser(vectClientIndex_t *i_pvectClientIdxList, BYTE *i_pbyBuf, int i_nLen);
//	int SendChatMsg2MultiUser(vectCIMIOCPSocketPtr *i_pvectCIISockPtrList, BYTE *i_pbyBuf, int i_nLen, BitFlag8_t i_chatType);
//	int SendChatMsg2MultiUser(vectClientIndex_t *i_pvectClientIdxList, BYTE *i_pbyBuf, int i_nLen, BitFlag8_t i_chatType);
	int SendChatMsg2MultiUser(vectCIMIOCPSocketPtr *i_pvectCIISockPtrList, BYTE *i_pbyBuf, int i_nLen, BitFlag16_t i_chatType);		// 2008-05-15 by dhjin, EP3 - ä�� �ý��� ����
	int SendChatMsg2MultiUser(vectClientIndex_t *i_pvectClientIdxList, BYTE *i_pbyBuf, int i_nLen, BitFlag16_t i_chatType);			// 2008-05-15 by dhjin, EP3 - ä�� �ý��� ����

	// 2009-01-14 by cmkwon, ��� �ڵ� ���� �ý��� ���� - 
	int SendString128ToAllClients(BYTE i_byStr128Ty, char *i_szString, BOOL i_bWriteLog=FALSE, BYTE i_byInfluenceMask=INFLUENCE_TYPE_ALL_MASK);
	
	///////////////////////////////////////////////////////////////////////////
	// ���
	BOOL OnGuildLoading(CGuild *i_pGuild, CIMIOCPSocket *i_pIISock);
	CGuild* GetGuildByName(const char *i_szGuildName);
	void DismemberAllPendingGuild(ATUM_DATE_TIME *pCurrentDateTime);
// 2004-10-20 by cmkwon, ������� �ʴ� �Լ���
//	void ReadyGuildWar(UID32_t DefenderGuildUID, UID32_t ChallengerGuildUID, INT ReadyTimeInSeconds);
// 2004-10-20 by cmkwon, ī��Ʈ ��� ����
//	void StartGuildWar(UID32_t DefenderGuildUID, UID32_t ChallengerGuildUID);
	void SendMsgToGuildMembersByGuildUID(UID32_t i_uidGuild, BYTE *buffer, int size, UID32_t CharacterUniqueNumberToExclude = INVALID_UNIQUE_NUMBER);
	void GuildWarCheck(UID32_t i_uidGuildUID, MAP_CHANNEL_INDEX i_mapChannIdxCurrent, UID32_t i_uidAttackerGuildUID=INVALID_GUILD_UID);
	void GuildWarEnd(USHORT i_usWarEndType, UID32_t i_uidGuildUID, DWORD i_dwTimerUniqueNumber=0);
	void GuildSetGuildFame(UID32_t i_guildUID, int i_nTotalFame, int i_nMonthlyFame);
	int WarpGulid(char *i_szGuildName, MAP_CHANNEL_INDEX i_wapMapChann, D3DXVECTOR3 i_vWarpPosition);
	BOOL IsGuildCommanderW(UID32_t i_guildUID, UID32_t i_charUID);	// 2007-10-06 by cmkwon, ������ üũ


	void SetFieldServerSocket(CIMIOCPSocket *i_pFieldServerSocket){	m_pFieldServerSocket = i_pFieldServerSocket;};

	///////////////////////////////////////////////////////////////////////////
	// ������
	void InsertParty(CIMParty *i_pParty);
	CIMParty *FindParty(PartyID_t i_partyID);
	void DeleteParty(PartyID_t i_partyID);
	void AdjustDeletedParties(void);				// ������ ��Ƽ�� �����Ѵ�

	//////////////////////////////////////////////////////////////////////////
	// 2006-12-08 by dhjin, ���� ����
	void SetInflAllLeader(CHARACTER *o_m_character);

	// 2006-05-08 by cmkwon, callGM �ý���
	int CallGMInsertRequest(UID32_t i_characterUID, BYTE i_byInflTy, char *i_szCharName, char *i_szCallDes);	// 2007-11-19 by cmkwon, �����ý��� ������Ʈ - �����߰�(, BYTE i_byInflTy)
	void CallGMOnRequest(int i_nCallGMRequestCount);
	BOOL CallGMMake_MSG_IC_ADMIN_CALLGM_VIEW_OK(MSG_IC_ADMIN_CALLGM_VIEW_OK *o_pCallGMViewOK, int i_nReqCount);
	BOOL CallGMMake_MSG_IC_ADMIN_CALLGM_BRING_OK(MSG_IC_ADMIN_CALLGM_BRING_OK *o_pCallGMBringOK, int i_nReqCount);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-06 by cmkwon, �������� 2���� ȣĪ�� �ٸ��� ���� -
	BOOL CheckInfluenceLeader(BYTE i_byInflTy, UID32_t i_uidCharacterUID);
	BOOL CheckInfluenceSubLeader(BYTE i_byInflTy, UID32_t i_uidCharacterUID);
	BOOL CheckInfluenceSubLeader1(BYTE i_byInflTy, UID32_t i_uidCharacterUID);
	BOOL CheckInfluenceSubLeader2(BYTE i_byInflTy, UID32_t i_uidCharacterUID);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-11-19 by cmkwon, �����ý��� ������Ʈ -
	bool IsOnCallGMSystem() const;
	void SetCallGMSystemTime(ATUM_DATE_TIME *i_patStartTime, ATUM_DATE_TIME *i_patEndTime);

	void OnDoMinutelyWorkIMIOCP(ATUM_DATE_TIME *pDateTime);		// 2007-11-19 by cmkwon, �����ý��� ������Ʈ -

	// 2009-01-14 by cmkwon, ��� �ڵ� ���� �ý��� ���� - 
	void OnDoSecondlyWorkIMIOCP(ATUM_DATE_TIME *pDateTime);

	//////////////////////////////////////////////////////////////////////////
	// 2008-05-21 by dhjin, EP3 - ���� ���� ����
	void UpdateGuildCommander(UID32_t i_GuildUID, UID32_t i_Commander, UID32_t i_NewCommander);	// 2008-05-21 by dhjin, EP3 - ���� ���� ���� - ������ ����
	BOOL CheckGuildLoaded(CGuild *i_pGuild, CIMIOCPSocket *i_pIISock);							// 2008-05-30 by dhjin, EP3 - ���� ���� ���� - ������ �ε��Ǿ� �ִ��� üũ
	void SetGuildMemberShipW(UID32_t i_GuildUID, BOOL i_bMemberShip, ATUM_DATE_TIME * i_pMemberShipDate = NULL);		// 2008-06-20 by dhjin, EP3 - ���� ���� ���� - ������ �ɹ��� ���� ����

	static int GetRandInt32(UINT i_uiMin, UINT i_uiMax) { return RANDI(i_uiMin, i_uiMax); }				// 2008-06-04 by dhjin, EP3 ���� �Լ� �߰�

public:
	virtual BOOL IOCPInit(void);
	virtual void IOCPClean(void);
	virtual SThreadInfo *CheckIOCPThread(DWORD i_dwThreadIdToExclude);

public:
	///////////////////////////////////////////////////////////////////////////
	// Party
	mt_map<PartyID_t, CIMParty*>		m_MapParties;
	mt_vector<CIMParty *>				m_mtvectorDeletedParties;
	
	CUID32Generator						m_UID32GenPartyID;		// PartyID�� UID32 Generator

	///////////////////////////////////////////////////////////////////////////
	// Guild
	mt_map<UID32_t, CGuild*>				m_MapGuild;				// main guild data

	///////////////////////////////////////////////////////////////////////////
	// ���� �ٿ� Ȯ�ο� ����
	USHORT			m_nServerDownVerifyNumber;

	///////////////////////////////////////////////////////////////////////////
	// �����ڿ� ���� ����Ʈ
	mtsetUID32		m_setAdminCharacterUniqueNumbers;

	UINT								m_uiIMAccumulatedTotalUserCounts;
	UINT								m_uiIMMaxTotalUserCounts;

	///////////////////////////////////////////////////////////////////////////
	// FieldServer�� ����� IMIOCPSocket
	CIMIOCPSocket						*m_pFieldServerSocket;
	FieldServerInfo						m_FieldServerInfo;

	///////////////////////////////////////////////////////////////////////////////
	// ä�� ����
	CChatBlockManager					m_chatBlockManager;

	CChatBlockManager					m_chatBlockManagerByLeader;		// 2008-12-30 by cmkwon, ������ ä�� ���� ī�� ���� - 


	///////////////////////////////////////////////////////////////////////////////
	// 2005-11-17 by cmkwon, ��ɾ� ����Ʈ
	vectString							m_vectstringCommandList;				// ������ ��ɾ� ����Ʈ
	vectString							m_vectstringGameMasterCommandList;		// 2007-11-08 by cmkwon, GM ��ɾ� ���� - ��� ��ɾ� ����Ʈ
	vectString							m_vectstringCommandListforUser;			// �Ϲ����� ��ɾ� ����Ʈ

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-20 by cmkwon, ���������� CharacterUID
	UID32_t								m_VCNLeaderCharacterUID;
	UID32_t								m_ANILeaderCharacterUID;
	UID32_t								m_VCNSub1LeaderCharacterUID;
	UID32_t								m_ANISub1LeaderCharacterUID;
	UID32_t								m_VCNSub2LeaderCharacterUID;
	UID32_t								m_ANISub2LeaderCharacterUID;

	// 2006-05-08 by cmkwon, CallGM �ý���
	mtvectSCALLGM					m_mtvectSCallGMList;			// 2006-05-08 by cmkwon

	///////////////////////////////////////////////////////////////////////////////
	// 2007-11-19 by cmkwon, �����ý��� ������Ʈ - ����, ���� �ð� ���� �߰�
	BOOL							m_bOnCallGMSystem;		
	ATUM_DATE_TIME					m_atCallGMStartTime;
	ATUM_DATE_TIME					m_atCallGMEndTime;


	BOOL							m_bArenaServerCheck;				// 2008-02-28 by dhjin, �Ʒ��� ���� - �Ʒ��� �������� üũ ��Ʈ 0:False, 1:TRUE

	//MTRand							m_mtRandManager;	// 2008-06-04 by dhjin, EP3 ���� �Լ� �߰�
	mt_lock							m_lockRandManager;	// 2008-06-04 by dhjin, EP3 ���� �Լ� �߰�

	//////////////////////////////////////////////////////////////////////////
	// 2008-06-09 by dhjin, EP3 ä�ù� - 
	CChatRoomManager				m_ChatRoomManager;	// 2008-06-09 by dhjin, EP3 ä�ù� - ä�ù� ������

	///////////////////////////////////////////////////////////////////////////////
	// 2009-01-14 by cmkwon, ��� �ڵ� ���� �ý��� ���� - 
	CANoticeManager *				m_pANoticeManager;	// 2009-01-14 by cmkwon, ��� �ڵ� ���� �ý��� ���� - 

	BOOL							m_bEnableCNC;
};

#endif // !defined(AFX_IMIOCP_H__89992FF9_ADF7_4FFF_A639_F9BC79F85619__INCLUDED_)
