#ifndef _ATUM_GUILD_H_
#define _ATUM_GUILD_H_

#include "AtumParam.h"
#include <SQLTYPES.H>
#include "AtumProtocol.h"

class CGuild;
class CIMIOCPSocket;
class CIMIOCP;
typedef vector<CIMIOCPSocket*>		vectCIMIOCPSocketPtr;			// 2007-01-31 by cmkwon

///////////////////////////////////////////////////////////////////////////////
// Guild Member
class CGuildMember
{
public:
	CGuildMember()
	{
		m_CharacterUniqueNumber = INVALID_UNIQUE_NUMBER;
		m_pIMIOCPSocket = NULL;
	}

	CGuildMember(char *i_CharacterName, UID32_t i_CharacterUniqueNumber, CIMIOCPSocket *i_pSocket, UID32_t i_uidGuild, BYTE i_GuildRank, BOOL i_bIsOnline
				, INT i_UnitKind, BYTE i_MemberLevel,EN_CHECK_TYPE i_VoipType)		// 2008-05-20 by dhjin, EP3 - ���� ���� ����
		: m_CharacterUniqueNumber(i_CharacterUniqueNumber)
		, m_Rank(i_GuildRank)
		, m_uidGuild(i_uidGuild)
		, m_bIsOnline(i_bIsOnline)
		, m_UnitKind(i_UnitKind)				// 2008-05-20 by dhjin, EP3 - ���� ���� ����
		, m_MemberLevel(i_MemberLevel)			// 2008-05-20 by dhjin, EP3 - ���� ���� ����
		//, m_VoipType(i_VoipType)				// 2008-07-16 by dhjin, EP3 - Voip ����	
	{
		m_pIMIOCPSocket = i_pSocket;
		util::strncpy(m_CharacterName, i_CharacterName, SIZE_MAX_CHARACTER_NAME);
	}

	BOOL SetOnline(CIMIOCPSocket *i_pSocket);
	void SetOffline(UID32_t i_uidCharacter);

public:
	char				m_CharacterName[SIZE_MAX_CHARACTER_NAME];
	UID32_t				m_CharacterUniqueNumber;
	BYTE				m_Rank;						// ���: GUILD_RANK_XXX
	BOOL				m_bIsOnline;				// ���� ���̸� TRUE
	INT					m_UnitKind;				// 2008-05-20 by dhjin, EP3 - ���� ���� ����
	BYTE				m_MemberLevel;			// 2008-05-20 by dhjin, EP3 - ���� ���� ����	
	//EN_CHECK_TYPE		m_VoipType;				// 2008-07-16 by dhjin, EP3 - Voip ����	

	UID32_t				m_uidGuild;
	CIMIOCPSocket		*m_pIMIOCPSocket;
	
	static CIMIOCP		*ms_pIMIOCP1;
};

class CIMIOCP;

///////////////////////////////////////////////////////////////////////////////
// Guild Mark
struct GUILD_MARK
{
	INT		nGuildMarkVersion;
	INT		nGuildMarkSize;
	char	GuildMarkImage[SIZE_MAX_GUILD_MARK_IMAGE];
	BYTE	byGuildMarkState;	// 2007-08-02 by cmkwon, ���� ��ũ �ɻ� �ý��� ���� - ������� �߰�(GUILD_MARK_STATE_XXX)
};

typedef list<CGuildMember>		listGuildMember;

///////////////////////////////////////////////////////////////////////////////
// Guild
class CGuild
{
public:
	CGuild();
	CGuild(CGuild *i_pGuild);
	~CGuild() {};

	void ResetGuild(void);

	BOOL IsGuildCommander(UID32_t i_CharacterUniqueNumber);
	BOOL AddGuildMember(CGuildMember& i_GuildMember);
	BOOL DeleteGuildMember(UID32_t i_CharacterUniqueNumber);
	BOOL GetGuildMember(UID32_t i_CharacterUniqueNumber, CGuildMember*& o_pGuildMember);	
	BOOL GetGuildMemberByRank(BYTE i_byRank, CGuildMember*& o_pGuildMember);			// 2008-07-01 by dhjin, EP3 - ���� ���� ���� 

	BOOL SendMsgToGuildMembers(BYTE *buffer, int size, UID32_t CharacterUniqueNumberToExclude = INVALID_UNIQUE_NUMBER, MSG_IC_COUNTDOWN_START *i_pCountdownStart=NULL);
//	BOOL SendChatMsgToGuildMembers(BYTE *buffer, int size, BitFlag8_t i_chatFlag8);
	// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
	BOOL SendGuildBonusExp();
	BOOL SendFieldToGuildMembers(BYTE *buffer, int size, UID32_t CharacterUniqueNumberToExclude = INVALID_UNIQUE_NUMBER, MSG_IC_COUNTDOWN_START *i_pCountdownStart=NULL);
	// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
	BOOL SendChatMsgToGuildMembers(BYTE *buffer, int size, BitFlag16_t i_chatFlag8);		// 2008-05-15 by dhjin, EP3 - ä�� �ý��� ����
	int Send_MSG_IC_GUILD_GET_GUILD_INFO_OK(CIMIOCPSocket *i_pIISock);

	void SetOnlineGuildMember(CIMIOCPSocket *i_pSocket, BOOL i_bSendMSG=TRUE);
	void SetOfflineGuildMember(UID32_t i_uidCharacter, BOOL i_bSendMSG=TRUE);
	int GetOnlineMemberCount(UID32_t i_CharacterUniqueNumberToExclude = INVALID_UNIQUE_NUMBER);
	int GetLiveMemberCountByMapChannelIndex(MAP_CHANNEL_INDEX i_mapChannIdx);

	void StartWar(UID32_t i_uidPeerGuild, MAP_CHANNEL_INDEX i_mapChannIdxWar, DWORD i_dwWarTimerUniqueNumber);
	void WarResultWin(void);
	void WarResultLose(void);

	BOOL CheckRank(UID32_t i_CharacterUniqueNumber, BYTE i_nNewRank);

	void PrintGuildInfo(CIMIOCPSocket *pCharacterSocket);

	void AddGuildFame(int i_nAddValues);

// 2007-01-31 by cmkwon, �Ʒ��� �Լ��� ��ü(int GetGuildMemberIISocket(vectCIMIOCPSocketPtr *o_pvectIISockPtr))
//	int GetGuildMemberIISocket(CIMIOCPSocket **pArrIISockPtr);
	int GetGuildMemberIISocket(vectCIMIOCPSocketPtr *o_pvectIISockPtr);

	BOOL SetMemberLevel(UID32_t i_CharacterUniqueNumber, BYTE i_byLevel);		// 2008-05-20 by dhjin, EP3 - ���� ���� ���� - ���� ���� �� ���� ����

	void InitGuildIntroduction();									// 2008-05-27 by dhjin, EP3 - ���� ���� ���� - ���� �Ұ� �ʱ�ȭ 
	void SetGuildIntroduction(SGUILD_INTRODUCTION * i_pGuildIntroduction);	// 2008-05-27 by dhjin, EP3 - ���� ���� ���� - ���� �Ұ� ���� 
	char * GetGuildIntroduction();									// 2008-05-27 by dhjin, EP3 - ���� ���� ���� - ���� �Ұ� ��������	
	void CheckOldIntroductionContent();								// 2008-05-27 by dhjin, EP3 - ���� ���� ���� - ������ ���� �Ұ� äũ 
	
	void SetGuildMemberShip(BOOL i_bMemberShip, ATUM_DATE_TIME * i_pMemberShipExpireTime = NULL);	// 2008-06-20 by dhjin, EP3 - ���� ���� ���� - �����忡 �ɹ��� ����	

	//BOOL SetMemberVoipType(UID32_t i_CharacterUniqueNumber, EN_CHECK_TYPE i_VoipType);	// 2008-07-16 by dhjin, EP3 - Voip ����

	// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
	void	CalGuildExpBonusRate();
	INT		GetGuildExpBonusRate();
	void	RefreshGuildOnlineMember();
	// end 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������

public:
	listGuildMember		m_ListGuildMember;

	// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
	INT					m_nOnlineGuildMember;
	INT					m_nBonusExpRate;
	// end 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������

	char				m_GuildName[SIZE_MAX_GUILD_NAME];
	UID32_t				m_GuildUniqueNumber;
	UID32_t				m_CommanderUniqueNumber;
	UID32_t				m_uidWarPeerGuildUID;
	INT					m_nGuildMemberCapacity;

	BYTE				m_GuildState;		// GUILD_STATE_XXX
	ATUM_DATE_TIME		m_DismemberDate;

	GUILD_MARK			m_GuildMark;

	INT					m_nWarWinPoint;
	INT					m_nWarLossPoint;

	MAP_CHANNEL_INDEX	m_WarMapChannelIndex;
	DWORD				m_WarTimerUniqueNumber;
	INT					m_WarKillCounts;

	INT					m_nTotalFame;				// 2005-12-27 by cmkwon, ���� �� ���� ��
	INT					m_nMonthlyFame;				// 2005-12-27 by cmkwon, ���� �Ѵ� ���� ��

	char				m_Notice[SIZE_MAX_NOTICE];		// 2008-05-20 by dhjin, EP3 - ���� ���� ����
	MapIndex_t			m_OutPostCityMapIndex;			// 2008-05-20 by dhjin, EP3 - ���� ���� ���� - �������� ����
	SGUILD_INTRODUCTION	m_GuildIntroduction;			// 2008-05-27 by dhjin, EP3 - ���� ���� ���� - ���� �Ұ�

	INT					m_nTotalFameRank;				// 2008-06-05 by dhjin, EP3 - ���� ���� ����
	INT					m_nMonthlyFameRank;				// 2008-06-05 by dhjin, EP3 - ���� ���� ����
	BOOL				m_bMemberShip;					// 2008-06-20 by dhjin, EP3 - ���� ���� ����
	ATUM_DATE_TIME		m_MemberShipExpireTime;			// 2008-06-20 by dhjin, EP3 - ���� ���� ����	

public:
	static DWORD		ms_dwGuildWarUniqueNumber;
	static CIMIOCP		*ms_pIMIOCP3;
};

inline MEX_GUILD_MEMBER_INFO& MEX_GUILD_MEMBER_INFO::operator=(const CGuildMember& rhs)
{
	util::strncpy(this->MemberName, rhs.m_CharacterName, SIZE_MAX_CHARACTER_NAME);
	this->MemberUniqueNumber = rhs.m_CharacterUniqueNumber;
	this->GuildRank = rhs.m_Rank;
	this->IsOnline = rhs.m_bIsOnline;
	this->UnitKind = rhs.m_UnitKind;		// 2008-05-20 by dhjin, EP3 - ���� ���� ����
	this->MemberLevel = rhs.m_MemberLevel;	// 2008-05-20 by dhjin, EP3 - ���� ���� ����
	//this->VoipType	= rhs.m_VoipType;		// 2008-07-16 by dhjin, EP3 - Voip ����

	return *this;
}

inline MEX_OTHER_GUILD_INFO& MEX_OTHER_GUILD_INFO::operator=(const CGuild& rhs)
{
	util::strncpy(this->GuildName, rhs.m_GuildName, SIZE_MAX_GUILD_NAME);
	this->GuildUniqueNumber = rhs.m_GuildUniqueNumber;
	this->GuildMarkVersion = rhs.m_GuildMark.nGuildMarkVersion;

	return *this;
}

#endif // _ATUM_GUILD_H_
