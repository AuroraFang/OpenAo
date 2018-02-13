// FieldIOCPSocket.h: interface for the CFieldIOCPSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIELDIOCPSOCKET_H__6FB6A18E_0C74_4E46_AB75_2ECA21C4204B__INCLUDED_)
#define AFX_FIELDIOCPSOCKET_H__6FB6A18E_0C74_4E46_AB75_2ECA21C4204B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IOCP.h"
#include "FieldMapChannel.h"
#include "AtumSJ.h"
#include "Quest.h"
#include "GeneralRequestHandler.h"
#include "FieldItemManager.h"
#include "MTCriticalSection.h"
#include "FieldSkillManager.h"
#include "FieldTimerManager.h"
#include "CountdownChecker.h"
#include "FieldCharacterQuest.h"
#include "GuildDamageManager.h"

#include "BazaarManager.h"				// 2006-07-26 by cmkwon
#include "MemoryHackHistory.h"			// 2012-12-14 by hskim, �޸��� �ڵ� �� ��� ����
#include "SpeedHackHistory.h"			// 2013-01-29 by hskim, ���ǵ��� �ڵ� �� ��� ����

#include "BillingWikigames.h"				// 2008-08-07 by cmkwon, Wikigames_Eng ���� ���� - 

#include "LetterManager.h"				// 2008-04-23 by dhjin, EP3 ���� �ý��� - 

#include "InvokingItemManager.h"			// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������


#include "ArenaManager.h"
#include "ArenaTeam.h"

#include "ArenaDeathMatchRoom.h"	// 2012-07-10 by jhseol, �Ʒ��� ���� - ������ ����

#include "ArmorCollection.h"		// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - #include "ArmorCollection.h" �߰�

#ifdef NEMERIAN_PVP_AWARD_EFFORT
#include "LastHitManager.h"
#endif

enum CS_State
{
	CS_NOTCONNECT		= 0,			// Client�� �������� ���� ����
	CS_CONNECTED		= 1,			// Client�� Socket ������ ����
	CS_RECVLOGIN		= 2,			// Client�� AccountName�� ������ Login�õ���(Pre Server���� Ȯ����)
	CS_LOGINED			= 3,			// Client�� AccountName�� ������ Login �� ����
	CS_RECVGETCHARACTER	= 4,			// Client�� Login�� �ϰ� ������ Character ������ �ޱ� ���� ó����(DB Ȯ����)
	CS_GOTCHARACTER		= 5,			// Client�� Login�� �ϰ� ������ Character ������ ���� ����
	CS_PLAYING			= 6,			// Client�� ������ Character�� Game�� ������ ����
	CS_WARP_PROCESSING	= 7,			// Client�� ���� ó����
	CS_ARENASERVER_PLAYING	= 8,		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - CS_ARENA_PLAYING => CS_ARENASERVER_PLAYING ���� // 2008-01-04 by dhjin, �Ʒ��� ���� - �Ʒ��� ���� ���̴�.
};

enum EnumWarpType
{
	WT_SAME_MAP					= 0,
	WT_DIFF_MAP_SAME_SERVER		= 1,
	WT_DIFF_MAP_DIFF_SERVER		= 2
};

struct TradeItem
{
	UID64_t			ItemUniqueNumber;
	int				Count;
	ITEM_GENERAL	*pStoreItem;

	TradeItem(): ItemUniqueNumber(0), Count(0), pStoreItem(NULL) {}

#ifdef _ATUM_SERVER
	void* operator new(size_t size) { return VMemPool::vmObjectNew(size); }

	void operator delete(void* p) { VMemPool::vmObjectDelete(p, sizeof(TradeItem)); };
#endif
};

struct SPEED_HACK_DATA
{
	int			nServerTimeGap;
	int			nSumClientTimeGap;
	int			nRecvMoveCounts;
	int			nSumServerDistanceGap;
	int			nCurrentSpeed;

	void ResetSPEED_HACK_DATA()
	{
		nServerTimeGap			= 0;
		nSumClientTimeGap		= 0;
		nRecvMoveCounts			= 0;
		nSumServerDistanceGap	= 0;
		nCurrentSpeed			= 0;
	}
};

constexpr auto	ARRAYSIZE_SPEED_HACK_DATA = 3;

constexpr auto CHARACTER_SPEED_HACK_CHECK_TERM = 10000;


class CFieldIOCP;


struct FIELD_DUMMY
{
	UINT		ItemFieldIndex;
	float		Endurance1;			// ���� ������
	INT			ItemNum;
	ITEM		*pItemInfo;			// �������� ����

	FIELD_DUMMY(UINT i_uiItemFieldIdx, ITEM *i_pItemInfo)
	{
		ItemFieldIndex	= i_uiItemFieldIdx;

		if (i_pItemInfo)
		{
			Endurance1	= i_pItemInfo->Endurance;
			ItemNum		= i_pItemInfo->ItemNum;
			pItemInfo	= i_pItemInfo;
		}
	}

#ifdef _ATUM_SERVER
	void * operator new(size_t size) { return VMemPool::vmObjectNew(size); }

	void operator delete(void* p) { VMemPool::vmObjectDelete(p, sizeof(FIELD_DUMMY)); };
#endif
};

enum EnumQueryType;

enum EnumResetVarType
{
	RVT_CONSTRUCTOR = 0,
	RVT_GAME_END = 1,
	RVT_ON_CLOSE = 2
};

// FMTT_XXX, Fieldserver Message Transfer Target
constexpr BitFlag16_t FMTT_CHARACTER_IN_RANGE = 0x0001;	// ������ ĳ����
constexpr BitFlag16_t FMTT_PARTY_IN_RANGE = 0x0002;	// ���� ���� ������ ��Ƽ��
constexpr BitFlag16_t FMTT_PARTY_IN_MAP = 0x0004;	// ���� ���� ��� ��Ƽ��, FMTT_PARTY_IN_RANGE ����
constexpr BitFlag16_t FMTT_GUILD_IN_RANGE = 0x0008;	// ���� ���� ������ ����
constexpr BitFlag16_t FMTT_GUILD_IN_MAP = 0x0010;	// ���� ���� ��� ����, FMTT_GUILD_IN_RANGE ����
constexpr BitFlag16_t FMTT_P2P_PK = 0x0020;	// 1:1 ���� ���

class CFieldMonster;
class CGuildItemManager;
class CPCBangIPManager;

typedef mt_map<UINT, FIELD_DUMMY*>			mtmapItemFieldIndex2FieldDummy;
typedef mt_vector<FIELD_DUMMY*>				mtvectFieldDummy;
typedef mt_set<DROPMINE*>					mtsetDropMine;
typedef mt_map<UID64_t, TradeItem>			mtmapUID2TradeItem;
typedef mt_vector<CHARACTER>				mtvectCharacter;
typedef mt_map<INT, CFieldCharacterQuest>	mtmapQuestIndex2FieldCharacterQuest;
typedef mt_vector<CFieldIOCPSocket*>		mtvectObersverCharacter;
typedef mt_vector<CFieldIOCPSocket*>		mtvectWatchCharacter;
typedef mt_vector<STUTORIAL_LIST_INFO>		mtvectTutorialList;
typedef mt_map<MapIndex_t, MapIndex_t>		mtmapMapIdx2MapIdx; 
typedef vector<CFieldIOCPSocket*>			vectCFieldIOCPSocket;
typedef mt_vector<CFieldIOCPSocket*>		mtvectCFieldIOCPSocket;
typedef vector<MOSTSTAYED_MAP_INFO>			vectStayedMapInfo;

class CFieldIOCPSocket : public CIOCPSocket
{
	friend class CPreWinSocket;
	friend class CIMWinSocket;
	friend class CArenaFieldWinSocket;	
	friend class CFieldIOCP;
	friend class CAtumFieldDBManager;
	friend class CFieldParty;
	friend class CAtumLogSender;
	friend class CGeneralRequestHandler;
	friend class CFieldItemManager;
	friend class CRacing;
	friend class CFieldTimerManager;
	friend class CArenaDeathMatchRoom;
	friend class CArmorCollection;

public:
	CFieldIOCPSocket();
	virtual ~CFieldIOCPSocket();

	CMTCriticalSection	m_mtCritSecForGameEndRoutine;
	CHARACTER			m_character;						// ������ Client�� Character���� ����ü
	UID32_t				m_nArenaCharacterUID;				// 2012-10-21 by jhseol, �Ʒ��� ���׼��� - �Ʒ��� ����� �ɸ��� ���� ����.
	char				m_szMACAddress[SIZE_MAX_MAC_ADDRESS];	// 2015-09-23 Future, adding of MAC Address Logging

	INT					m_nPlayCharacterType;	// 2012-06-08 by jhseol, �Ʒ��� �߰�����part2 - �ɸ��� : Ÿ�� ����
	UsingItemBitFlag	m_structUsingItemBitFlag;			// 2012-12-06 by jhseol, ��ġ���� ���� ĳ���� �߽����� ����
	SARENA_INFO			m_ArenaInfo;						// 2007-04-17 by dhjin, Arena ���� ��
	BYTE				m_GuildRank;							// 2006-09-25 by dhjin, ������ ��� ��ŷ ����
	char				m_szCharacterMent[SIZE_STRING_32];	// ĳ���� ��Ʈ
	BOOL				m_bStealthState;
	BOOL				m_bStealthStateBySkill;				// 2013-05-09 by hskim, ���� ����Ʈ ����						
	INT					m_nPlusHP;
	INT					m_nPlusDP;
	BOOL				m_bDeadReasonByPK;					// 1:1 ���� ���� ���� ����
	BYTE				m_byCityWarTeamType;				// ������������ ������, �����, ������ ����
	BYTE				m_bySelectableInfluenceMask;		// ���� ������ ����Mask, �ϳ��� �������� �ϳ��� ���¸� ���ð���
	Experience_t		m_ExperienceDelayStore;				// 2006-05-21 by cmkwon, ����ġ ���� ���� �ý���
	ATUM_DATE_TIME		m_AccountRegisteredDate;			// 2006-06-02 by cmkwon
	char				m_AccountPasswordFromDB[SIZE_MAX_PASSWORD_MD5_STRING];	// 2006-06-02 by cmkwon
	BOOL				m_bPreServerAuthenticated;			// 2007-11-06 by cmkwon, ���� �α� DB ���� ���� �����ϱ� - ���Ӽ��� �α���/�α׾ƿ� ó��, PreServer ���� �÷��� ���� �߰�

	mtvectFC_CONNECT_LOGIN_INFO m_mtvectAllCharacterInfo;   // 2006-09-15 by dhjin, ������ ��� �ɸ��� ����
	mtvectCharacter		m_mtvectCharacterCache;		// GetCharacter�� ������ �����ϱ� ���� vector
	int					m_nNumCharacters;			// ������ Client�� ��� Character ��(�ִ� 3, SIZE_MAX_NUM_CHARACTER)
	CS_State			m_CSClientState;			// ������ Client�� ���� ����
	BOOL				m_bFlagDBStore;				// ����ÿ� DB�� Client�� ���� ��ġ������ DB�� �����ϱ� ���� �÷���
	DWORD				m_dwConnectTime;			// Client�� ������ �ð�(�ʴ���)
	//DWORD				m_dwTickLastAttackPrimary;	// Primary ������ ���� �ð�(Tick����)
	//DWORD				m_dwTickLastAttackSecondary;// Secondary ������ ���� �ð�.
	BOOL				m_bNotifyCloseFlag;
	EVENTINFO			*m_pCurrentEventInfo;		// �ٸ� �ʵ� �������� ���� �� ������ �����ϱ� ���� ��� -> used @ CPreWinSocket::OnRecvdPacket()�� case T_FP_EVENT_NOTIFY_WARP_OK
	USHORT				m_nCurrentWarpAreaIndex;	// �ٸ� �ʵ� �������� ���� �� WarpArea ������ �����ϱ� ���� ��� -> used @ ResEventWarpConnect, check: ����ϴ°�? Ȯ���ϱ�...
	CFieldParty			*m_pFieldParty;
	BOOL				m_bFieldServerChangeFlag;	// �ٸ� �ʵ弭������ ������ ConnectGameStart�� close���� ����ϱ� ���� ����

	CRITICAL_SECTION	m_criticalLevelAndExperience;	// ĳ������ ����ġ�� ������ ����ȭ�� ����

	INT					m_nMapMoveCount;				// 2011-10-10 by hskim, EP4 [Ʈ���� �ý���] - ȭ���� / �� ��ǳ

	// Map ���� ���� �߰�, added by ywlee 20021029
	CFieldMapChannel	*m_pCurrentFieldMapChannel;		// Character�� ���� �ִ� �ʿ� ���� pointer, initialize�� �Ҵ�
	CFieldMapProject	*m_pFieldMapProjectForNPC;

	int					m_nOnlineEachOtherFriendCnts;			// 2006-07-18 by cmkwon, ���� ��ϵ� ģ������Ʈ���� �¶��� ģ�� ī��Ʈ

	int					m_nGuildBonusExpRate;			// 2012-10-04 by jhjang, ���ܿ� ���� �������� �¶��� �ܿ� ī��Ʈ
	int					m_nKillCount;					// 2012-10-04 by jhjang, ųī��Ʈ ���ʽ� ī��Ʈ
	bool				m_bIsPayKillCountBonusItem;		// 2012-10-04 by jhjang, ųī��Ʈ ���ʽ� ī��Ʈ
	int					m_nInSamePartyCount;			// ��� ����ġ ���ʽ�.


	CFieldItemManager			m_ItemManager;
	ITEM						m_arrAttachItems[SIZE_MAX_POS];
	ITEM						&m_ItemProw;			// 2008-09-23 by dhjin, ���̴� // ���� (����Ʈ�迭 + ���迭)
	ITEM						&m_ItemProwIn;			// Primary ����, ������ ����, (��ǻ�� �迭)
	ITEM						&m_ItemProwOut;			// Primary ����, ������ �ٱ���, ���� (���迭 or ĳ��迭)
	ITEM						&m_ItemWingIn;			// Secondary ����, ������ ���� , (���� �� ����, 20041111, kelovon)
	ITEM						&m_ItemWingOut;			// Secondary ����, ������ �ٱ���, ����(���ϰ迭 or �̻��ϰ迭)
	ITEM						&m_ItemCenter;			// �Ƹ� ����
	ITEM						&m_ItemRear;			// �����迭


	ITEM						&m_ItemAccessoryUnLimited;	// 2006-03-30 by cmkwon, ������ �Ǽ��縮 - ������(�Ĺ� ����-������ũ|�����̳ʰ迭)
	ITEM						&m_ItemAccessoryTimeLimit;	// 2006-03-30 by cmkwon, �ð����� �Ǽ��縮(�Ĺ� ����)

	ITEM						&m_ItemPet;				// 2010-06-15 by shcho&hslee ��ý��� - ������ �� ����

	CUID16Generator				m_WeaponIndexGenerator;	// MSG_FC_BATTLE_ATTACK_OK�� WeaponIndex �Ҵ�
	
	// ��ų ����
	CFieldSkillManager			m_SkillManager;

	// Ÿ�̸� ����
	CFieldTimerManager			m_TimerManager;
	
	CArmorCollection			m_ArmorCollection;		// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - m_ArmorCollection ����
	
	Experience_t				m_InfCurrentAllEXP;
	
	LONG						m_nInsertingStoreItemCounts;
	DWORD						m_dwLastTickInsertingStoreItem;


	mtmapItemFieldIndex2FieldDummy	m_mapFieldDummy;

	// ���η� ������
	mtsetDropMine				m_setCharacDropMine;

	// ��ü�� �׸��� ���� ���� ���� - ���� ������ ��
	CHARACTER_RENDER_INFO		m_UnitRenderInfo;

	// ���� �� �ִ� �ǹ� ����
	BUILDINGNPC					*m_pCurrentBuildingNPC;


	int							m_nCrystalAttackCount;				// 2012-12-15 by jhseol, ���� �ý��� ������ - ����� Ʈ���� ��������(�Ͻ�����) ũ����Ż ����Ƚ�� ����

	ATUM_DATE_TIME				m_atAccountLastBuyDate;				// 2013-03-29 by jhseol, ������ �̺�Ʈ - �ſ� ù ����� ��������
	ATUM_DATE_TIME				m_atCharacterLastBuyDate;			// 2013-03-29 by jhseol, ������ �̺�Ʈ - �ſ� ù ����� ��������
	ATUM_DATE_TIME				m_atLoginDate;						// 2013-03-29 by jhseol, ������ �̺�Ʈ - ����Ÿ�� �߰������� ����
	
	BOOL						m_bProcessingStoreGetItem;			// 2012-12-11 by hskim, ��Ʈ�� ������ ���� ���� ���� (DB ó���� ���������� ���� ��Ŷ�� ����)
																	// �ش� m_bProcessingStoreGetItem �� QT_GetStoreItem / QT_GetGuildStoreItem ����ÿ� �׻� TRUE ����Ǿ�� ��
																	// �ش� ������ FALSE ������ DB �� ó���� �ٳ����� ��쿡 ������
																	// 2012-12-11 by hskim, ��Ʈ�� ������ ���� ���� ���� (DB ó���� ���������� ���� ��Ŷ�� ����)		
																	// ��� ���� ���װ� �߰��� �߰ߵ�
																	// �ش� ��� �������� �κ��丮�� �������� �κ��� QT_DeleteGuildStoreItem / QT_UpdateGuildStoreItem ���� �ش� �κ� TRUE ���� �ǵ��� ó����

	CParamFactor				m_ParamFactor;

private:
	// DB Log��
	DWORD						m_nGameStartTimeInSeconds;			// �ʴ���

	// PK ��� ����
	BOOL						m_bPKMode;					// PK ���� ����, PK ���� ������ �� Ŭ���̾�Ʈ�� ��û�� ���� TRUE�� ����
	ClientIndex_t				m_peerP2PPKClientIndex;		// 1��1 PK ����� �Ҵ�, �⺻��: INVALID_CLIENT_INDEX
	BOOL						m_bP2PPKStarted;			// 1��1 PK�� ���۵Ǿ����� ����, (PK ��û �� ���´� ����!)
	INT							m_nP2PPKPreferredMode;		// 1��1 PK�� ���۵Ǿ����� ����, (PK ��û �� ���´� ����!)
	INT							m_nP2PPKRequestedMode;		// 1��1 PK�� ���۵Ǿ����� ����, (PK ��û �� ���´� ����!)

	// �ŷ�
	UID32_t						m_peerTraderCharacterUniqueNumber;	// �ŷ��ϴ� ����, ���� INVALID_UNIQUE_NUMBER�̸� �ŷ��� �ƴ�
	BOOL						m_bOKTradeChecked;					// �ŷ� �Ϸᰡ ���� �� TRUE, �� �ŷ��ڰ� ��� TRUE�̸� �ŷ� ����
	mt_lock						m_lockTrade;						// �ŷ��� ���� lock, ���� ��� �� ��
	mt_lock						*m_pCurrentLockTrade;				// �ŷ��� ������ ���Ǵ� lock
	mtmapUID2TradeItem			m_mapTradeItem;

	mtvectClientIndex_t			m_mtvectAttackTargetMonsterIndex;		// �ڽ��� ���� �ߴ� MonsterIndex


	BitFlag16_t					m_enableChatFlag16;

	BYTE						m_STRING_128_print_level;

	BOOL						m_bSendMessgeTypeDBG;

	DWORD						m_nOldTimeStampDBG;

	mtmapUID2EnchantVector		m_mapEnchant;		// (ItemUniqueNumber) -> (ENCHANT List)


	mtmapQuestIndex2FieldCharacterQuest	m_mapQuest;	// lock ó�� Ȯ����!


	USHORT						m_backupRace;


	DWORD						m_dwSendCountsMoveOK;
	DWORD						m_dwSendCountsAttack;


	DWORD						m_dwLastTickSpeedHackCheck;			// Speed Hack üũ�� �� ������ Tick
	SPEED_HACK_DATA				m_arrSpeedHackCheck[ARRAYSIZE_SPEED_HACK_DATA];

	CGeneralRequestHandler		m_RequestHandler;


	CFieldMapChannel			*m_pRacingFieldMapChannel;
	BYTE						m_byRacingNextCheckPointNumber;
	BYTE						m_byRacingRanking;


	CCountdownChecker			m_countdownChecker;


	UID32_t						m_uidAttackerGuildUID;


	DWORD						m_dwLastTickRolling;		// �Ѹ��� ����� tick


	ITEM						*m_pGiveKillMarkItemPtr2Killer;		// 2006-02-09 by cmkwon, Ÿ���� ������ ���� �������� �� �������� ����� ų��ũ

	int							m_nDeletedPacketCount;	// 2008-03-13 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - �ӽÿ�

	vectCFieldIOCPSocket		m_VecMissionMasterIOCPSocket;		// 2008-12-22 by dhjin, �̼Ǹ�����

#if S_BONUSEXPSYSTEM_RENEWAL
	INT							m_nRestExperienceCount;				// 2012-10-07 by hskim, �޽� ����ġ
#endif

	CMemoryHackHistory			m_MemoryHackHistory;				// 2012-12-14 by hskim, �޸��� �ڵ� �� ��� ����
	CSpeedHackHistory			m_SpeedHackHistory;					// 2013-01-29 by hskim, ���ǵ��� �ڵ� �� ��� ����



// 2015-10-04 Future, decking detection
#ifdef S_ANTI_DECK
	mt_lock			m_lockLastHit;
	ENEMY_LAST_HIT	m_LastHit;
#endif 

public:
	static CFieldIOCP			*ms_pFieldIOCP;



	void InitFieldIOCPSocket();

	BOOL IsTrivialMessageType(MessageType_t msgType);

#ifdef _DEBUG
	BOOL SendAddData(BYTE* pData, int nSize);  // check: EVENT ���� MSG �� ������ ���� ���� ����! ���� ��� ã�ƾ� ��.
#endif
	BOOL SendAddDataBuffer(GBUFFER_SIZE_MAX_PACKET &i_gbuffer, EN_PACKET_PRIORITY i_enPacketPriority = EN_PACKET_PRIORITY_HIGHEST);

	// Property Functions
	void SetCurrentFieldMapChannel(CFieldMapChannel *i_pCurrentFieldMapChannel);
	CFieldMapChannel* GetCurrentFieldMapChannel();
	CFieldMapProject * GetCurrentFieldMapProject(void);
	CFieldMapChannel *GetFieldMapChannel(MAP_CHANNEL_INDEX *i_pMapChannelIndex, BOOL i_bFindOtherChannel = FALSE, BOOL i_bFindDefaultMapChannel = FALSE);
	void SetSelectableInfluenceMask(BYTE i_byMask);
	BYTE GetSelectableInfluenceMask(void);
	void SetAccountInfo(MSG_FP_CONNECT_AUTH_USER_OK *i_pFPAuthUserOK);


	void ResetAllVariables(EnumResetVarType type);										// ������, OnClose, GameEnd ��� ��� ��� ���� �����ϱ�

	inline BOOL IsUsingRestrict(ClientIndex_t ClientIndex, UID32_t CharacterUniqueNumber);	// CFieldIOCPSocket�� �ν��Ͻ��� ��� �������� ���θ� ����, restrict checking by kelovon

	// ĳ���� 
	// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
	float GetPlusRateExp(INT nInSamePartyCount = 1);
	// end 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
	float GetPlusRateSPI(void);
	float GetPlusRateDropItem(void);
	float GetPlusRateDropRareItem(void);
	float GetPlusRateExpRepair(void);
	float GetHPRepairRateFlighting(void);
	float GetHPRepairTimeSubtractRate(void);
	float GetDPRepairTimeSubtractRate(void);
	float GetSPRepairTimeSubtractRate(void);
	float GetPlusWarPointRate(void);						// 2007-06-26 by dhjin, ������Ʈ �̺�Ʈ ���� �߰�		

	// ĳ���� ����
	const CHARACTER* GetCharacter() const { return &m_character; }
	CS_State GetClientState() const { return m_CSClientState; }

	inline void SetClientState(CS_State csState);
	void CharacterGameEndRoutine(void);
	BOOL CharacterSaveCriticalData(BOOL i_bSynchExec = FALSE);
	BOOL CharacterGameStartRoutine(GameStartType type);
	BOOL CheckCurrentMapOnGameStart(void);	// 2007-12-26 by cmkwon, CurrentMap üũ �ý��� ���� - CFieldIOCPSocket::CheckCurrentMapOnGameStart() �߰�
	void CharacterDeadRoutine(BYTE damageType, CFieldMonster *i_pAttackMonster=NULL, CFieldIOCPSocket *i_pAttackUser=NULL);	// ĳ���Ͱ� �װ� GameEndRoutine() Ȥ�� DEAD_GAMESTART �ϱ� ����(����)�� ó���� ���׵�
	BOOL CharacterDeadGameStartRoutine(BOOL i_bNotify, int i_nRecoverHP=0, int i_nRecoverDP=0, int i_nRecoverSP=0, int i_nRecoverEP=0);		// ĳ���� ��Ȱ ó��
	void OnWarpDone(EnumWarpType warpType);		// ���� �Ϸ� �� ���� ���� �� ó�� ����
	LONGLONG GetTotalPlayTimeInSeconds();		// �� ���ӽð� ��ȯ
	LONGLONG GetCurrentPlayTimeInSeconds();		// ���� ���� ���� ���� ��������� ���� �ð� ��ȯ
	LONGLONG GetPCBangTotalPlayTimeInSeconds(); // 2007-06-07 by dhjin, PC�� �� �÷��� �ð� ��ȯ
	BOOL CompareCharacterName(char *i_szCharacterName);
	CFieldMapProject *GetDefaultCityMapProject(void);
	INT GetDownSPICountOnDead(void);
	void CharacterChangeModeRoutine(void);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-10-12 by cmkwon, ������ī ���� ��� ���� - �����߰�(MapIndex_t i_nStartCityMapIdx)
	CFieldMapProject *GetCityFieldMapProjectPtr(BYTE i_byInfluence=INFLUENCE_TYPE_UNKNOWN, MapIndex_t i_nStartCityMapIdx=VCN_CITY_MAP_INDEX);
	CFieldMapChannel *GetCityFieldMapChannelPtr(BYTE i_byInfluence=INFLUENCE_TYPE_UNKNOWN, MapIndex_t i_nStartCityMapIdx=VCN_CITY_MAP_INDEX);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-10-12 by cmkwon, ������ī ���� ��� ���� - 
	CFieldMapProject *GetGardenFieldMapProjectPtr(BYTE i_byInfluence=INFLUENCE_TYPE_UNKNOWN, MapIndex_t i_nStartCityMapIdx=VCN_CITY_MAP_INDEX);


	void ProcessSPIPenaltyOnDead(BYTE i_nItemUpdateType=IUT_PENALTY_ON_DEAD);
	BOOL IsCityCurrentFieldMapChannel(void);

	// 2010-04-22 by cmkwon, �Ʒ��� �÷��� ���µ� ��ȿ�� ĳ���ͷ� ó�� - 
	BOOL IsValidCharacter(BOOL i_bLiveCheck=TRUE, BOOL i_bPermissionArenaState=FALSE);

	Err_t IsEnableWarp(int i_nMapInfluenceTy, BOOL i_bConflictAraeMap, int i_nRequetCompletionQuestIndex=0, BOOL i_bCancelTrade=FALSE, CFieldMapProject *i_pTargetFieldMapProject = NULL);	// 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý��� // 2010-02-01 by cmkwon, �ŷ��� �̼Ǹ��̵� ���� ���� ���� -
	Err_t IsEnableWarpToCityMap(void);

	Err_t ClearCharacterForWarp(void);	// 2009-02-05 by cmkwon, ����� ���� ���� ���� ó�� ���� - 

	// 2008-01-10 by cmkwon, IsCheckInfluenceTypeANDSocketType() ���� ���� - IsCheckInfluenceTypeANDSocketType() ���� �߰�
	// 2009-10-12 by cmkwon, ������ī ���� ��� ���� - IsCheckInfluenceTypeANDSocketType, �����߰�(i_nStartCityMapIdx)
	BOOL IsCheckInfluenceTypeANDSocketType(BYTE i_bySenderInfluenceTyMask, int i_nOnlySocketTy=ST_ALL, BOOL i_bGameMasterOK=TRUE, MapIndex_t i_nStartCityMapIdx=0);
	BOOL IsCheckLevel(BYTE i_byMinLevel, BYTE i_byMaxLevel);

// 2008-09-12 by cmkwon, "/��" ��ɾ� �߰� - 
//	void AddCharacterFame(int i_nAddFameValues=1);
	void AddCharacterFame(int i_nAddCharacterFameValues=1, int i_nAddGuildFameValues=1);
	UINT AddCharacterWPRewardByKill();
	UINT AddCharacterSPIRewardByKill();
	BOOL AddCredits(INT amount);	// 2015-08-04 Future, adding of credits

	BOOL IsGoToInfluenceCity(BYTE i_byCharInfluenceTy, INT i_nMapInfluenceTy);
	BOOL IsValidGuild(void);
	void StoreDB_QPARAM_CHARACTER_CHANGE_STAT(void);
	void StoreExperienceDelayStore(void);

	// ĳ���� �Ӽ� ����
	// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
	Experience_t ChangeExperience(Experience_t fChangeValue, BOOL i_bApplyPartner = FALSE, BOOL i_bApplyPlusRate=TRUE, BOOL i_bCheckValidGameUser=TRUE, INT nInSamePartyMember=1, CFieldMonster* nMonsterLevel = NULL);		// 2011-09-02 by hskim, ��Ʈ�� �ý��� 2��
	// end 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
	void ChangeLevel(int nLevelChangeValue, int nPercentageOfExp = 0);
	void CheckAndUpdateStatus();				// �ź� ��ȭ Ȯ���ϱ�
	void ChangeCurrentHP(float i_fChangeValHP, BYTE i_nDamageType= DAMAGE_BY_NA, ClientIndex_t i_SkillUseClientIdx=0);
	void ChangeCurrentDP(float i_fChangeValDP, ClientIndex_t i_SkillUseClientIdx=0);
	void ChangeCurrentSP(float i_fChangeValSP);
	void DecreaseCurrentSP(int i_nChangeValSP, INT i_nSkillBaseNum=0);
	void ChangeCurrentEP(float i_fChangeValEP);
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	void ChangeHP(UINT i_nHP);
	void ChangeDP(UINT i_nDP);
	UINT GetCharacterTotalHP(void);
	UINT GetCharacterTotalDP(void);
#else
	void ChangeHP(USHORT i_nHP);
	void ChangeDP(USHORT i_nDP);
	USHORT GetCharacterTotalHP(void);
	USHORT GetCharacterTotalDP(void);
#endif	
	void ChangeSP(USHORT i_nSP, BOOL i_bStore2DB=FALSE);
	void ChangeEP(USHORT i_nEP, BOOL i_bStore2DB=FALSE);
	USHORT GetCharacterTotalEP(void);
	USHORT GetCharacterTotalSP(void); //// 2010-08-26 by shcho&jsKim, �� ������ ����

	float DecreaseCharacterHPDP(float i_fChangeValue, BYTE i_enumDamageType, BOOL i_bCheckRecovery=TRUE
		, CFieldMonster *i_pAttackMonster=NULL, CFieldIOCPSocket *i_pAttackCharacter=NULL);

// 2010-03-05 by cmkwon, �ߵ��� �������α��� ���� - 
//	void CheckArmorState(void);					// �Ƹ� ���� ���ο� ���� ���� ���� Ȯ��
//	void DistributeBonusStatByAutoStatType(void);
//	void CheckArmor_SendTotalStat(BOOL i_bRecalculateComputerItem=FALSE);

	//void ApplyArmorParamFactor(void);			// �Ƹ��� ParamFactor ����
	void CheckComputerState(ITEM *i_pItemInfo, BOOL i_bAttach, BOOL i_bSendChangeInfo=TRUE);	// ��ǻ�� ������ ���� ���ο� ���� ���� ���� Ȯ��, i_bAttach�� TRUE�� ���� ��, FALSE ���� ��
	void CheckAllState(INT SataCount, BOOL i_bAttach, BOOL i_bSendChangeInfo=TRUE);		// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - CheckAllState() ����

	// 2010-03-17 by cmkwon, ����1�� �ߵ��� ������ ���� ���� ���� - ���� ����ŸƮ�ÿ� CurrentDP�� �Ƹ��� �߰� DP��ŭ ������ ���� ���� 
	void ApplyComputerGearStat(DestParam_t desParam, float paramValue, BOOL i_bSendInfo=TRUE);		// 2011-08-01 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ��� ���� (DestParameter - 255 -> 32767 ����)

	BOOL ChangeStat(BYTE i_nStatKind, Stat_t i_nChangeValue);
	Err_t ResetStat(int i_nStat);
	void SendStat_UpdateDB(BOOL i_bSendGearStat=TRUE, BOOL i_bSendTotalGearStat=TRUE, BOOL i_bUpdateDB=FALSE);
	void UpdateDBtoBonusStatPoint(void);
	BOOL ChangeCharacterInfluenceType(BYTE i_byInfluenceTy, BOOL i_bAdminCommand=FALSE);
	void ApplyParamFactorWithAccesoryTimeLimiteItem(ITEM *i_pItemInfo);
	void ApplyParamFactorWithAllUsingTimeLimitedOnlyCardItemW(void);
	void ArenaWarpSetHPDPSPEP();								// 2007-05-11 by dhjin, �Ʒ��� ������ �̵��� ó�� �ؾߵǴ� HP,DP,SP,EP
	void ArenaWarpCitySetHPDPSPEP(BOOL i_bSendClient=TRUE);		// 2007-05-11 by dhjin, �Ʒ��� �ʿ��� ������ �̵��� ó�� �ؾߵǴ� HP,DP,SP,EP
	void RearrangeCharacterTatalStat(BOOL i_bSendGearStat=TRUE, BOOL i_bSendTotalGearStat=TRUE, BOOL i_bUpdateDB=FALSE);		// 2010-03-05 by cmkwon, �ߵ��� �������α��� ���� - 

	// ��ü�� �׸��� ���� ���� loading
	// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - InitRenderInfo()�� �Լ��� ����
	//void UpdateRenderInfo();									// RenderInfo�� ��� ������
	void InitRenderInfo(void);
	BOOL UpdateItemRenderInfoByPOS(INT *o_pnItemNum, INT *o_pnShapeItemNum, INT *o_pnEffectItemNum, INT i_nPos, INT *o_pnPetLevel);		// 2012-12-05 by hskim, ��Ʈ�� �Ⱓ�� ���� ���� - // 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - �߰���.

	void UpdateItemRenderInfo(INT WinPos, BOOL bSendMsg);		// Item�� ���õ� RenderInfo���� ������

	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-18 by cmkwon, ���ͺ��� ���� - 
	BOOL UpdateItemRenderInfoForTransformer(INT i_nMonUnitKind, BOOL i_bAttachFlag, BOOL i_bSendToAround=TRUE);

	// body condition ����
	static void SetBodyConditionBit(BodyCond_t &VAR, BodyCond_t MASK);
	static void ClearBodyConditionBit(BodyCond_t &VAR, BodyCond_t MASK);
//	static BOOL CompareBodyConditionBit(UINT &VAR, UINT MASK);

	// NPC�� �� ����
	inline BOOL InitializeNPCMap(CMapProject *pmapProject, map<int, MONSTER> *mapMonsterParam);

	// Event ó��: WarpPoint�� �־����� �� ��ġ�� ó���Ѵ�.
	EventResult_t HandleEvent(EVENTINFO *pEventInfo, const ChannelIndex_t ChannelIndex = 0, BYTE i_byCharacterMode=FALSE);
	EventResult_t WarpToMap(const MAP_CHANNEL_INDEX &MapChannel);
	EventResult_t WarpToMap(const MAP_CHANNEL_INDEX &MapChannel, D3DXVECTOR3 *pPosition, BYTE i_byCharacterMode=FALSE);
	EventResult_t WarpToMap(const MAP_CHANNEL_INDEX &MapChannel, int TargetIndex);
	EventResult_t WarpToMap(const MapIndex_t MapIndex, const ChannelIndex_t ChannelIndex);
	EventResult_t WarpToMap(const MapIndex_t MapIndex, const ChannelIndex_t ChannelIndex, D3DXVECTOR3 *pPosition);
	EventResult_t WarpToMap(const MapIndex_t MapIndex, const ChannelIndex_t ChannelIndex, int TargetIndex);
	EventResult_t WarpToCityMap(void);
	EventResult_t WarpToQuestMap(MapIndex_t i_targetMapIndex);
	BOOL SendAllChannelState(MapIndex_t i_nMapIndex, EVENTINFO *i_pEventInfo);
	BOOL SendAllChannelState(CFieldMapProject *i_pFieldMapProject, EVENTINFO *i_pEventInfo);
	BOOL SendAllChannelState(MapIndex_t i_nMapIndex, INT i_nWarpTargetIndex);
	BOOL SendAllChannelState(CFieldMapProject *i_pFieldMapProject, INT i_nWarpTargetIndex);
	EventResult_t SetWarpTargetPosition(CFieldMapProject *i_pMapProject, EVENTINFO *i_pEventInfo, AVECTOR3 *o_pWarpTargetPosition);
	D3DXVECTOR3 GetWarpTargetPosition(CFieldMapProject *i_pMapProject, EVENTINFO *i_pEventInfo);

	void ApplyMapEntranceCondition(vectorMapEntranceCondition *pEntranceCondition);		// 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���

	
	// ����� ��ȿ�� �˻�
	BOOL CheckValidAttackTargetCharacter(CFieldMapChannel *i_pFMChann, ClientIndex_t i_targetIndex);	// ���� ��� ĳ������ ��ȿ�� �˻�
	BOOL CheckValidAttackTargetCharacter(CFieldMapChannel *i_pFMChann, ClientIndex_t i_targetIndex, CFieldIOCPSocket *&o_pTargetSocket);	// ���� ��� ĳ������ ��ȿ�� �˻�
	BOOL CheckValidAttackTargetCharacter(CFieldMapChannel *i_pFMChann, ClientIndex_t i_targetIndex, CFieldIOCPSocket *&o_pTargetSocket, CHARACTER *&o_pTargetCharacter);	// ���� ��� ĳ������ ��ȿ�� �˻�
	BOOL CheckValidAttackTargetMonster(BYTE i_byAttackCharInfluenceTy, ClientIndex_t i_nTargetIndex, ChannelIndex_t i_nChannelIndex, CFieldMonster *&o_pTargetMonster);	// ���� ��� ������ ��ȿ�� �˻�

	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-16 by cmkwon, ����2�� MtoM, MtoC Ÿ�� ���� ���� ���� - 
	BOOL CheckValidM2M(CFieldMapChannel *i_pFMChann, ClientIndex_t i_AttackerIdx, CFieldMonster **o_ppAttackMon, ClientIndex_t i_TargetIdx, CFieldMonster **o_ppTargetMon);	// M to M ���� ���� ��ȿ�� üũ

	static int CalcSplashDamageKindByMonsterAllAttack(ITEM *i_pAttackItem, float i_fDistance, int i_nAddExplosionRange);

	static void ProcessSplashDamageC2All(CFieldIOCPSocket *i_pAttFSock, SATTACK_PARAMETER *i_pAttParam, float i_fRadius, D3DXVECTOR3 *i_pVec3TargetPos);		// 2006-11-29 by dhjin, 
	static void ProcessSplashDamageMonsterAllAttack(MSG_FN_BATTLE_ATTACK_PRIMARY *i_pAttackPri, CFieldIOCPSocket *i_pAttackMonSock, CFieldMonster *pAttackMon, ITEM *pAttackItem);

	static float CalcDamageOfAttackNew(BYTE *o_pDamageKind, enumAttackToTarget i_attType, float i_fDamageWeight,
						  CFieldIOCPSocket *i_pAttackSocket, void* i_pAttackUnit, SATTACK_PARAMETER *i_pAttParam,
						  CFieldIOCPSocket *i_pTargetSocket, void *i_pTargetUnit, 
						  float fSkillAppliedAttackRange = 0.0f, float fActualAttackDistance = 0.0f);

	static void OnMonsterDead(CFieldIOCPSocket *pAttackSocket, CHARACTER *pAttackCharacter,
					  CFieldIOCPSocket *pTargetSocket, CFieldMonster *pTargetMonster,
					  ITEM *pAttackItem);	// C2M�� ��츸 ó����

	static void OnInfluenceBossMonsterCreated(CFieldMonster *i_pFMon, CFieldMapChannel *i_pFMapChann);
	static void OnInfluenceBossMonsterAutoDestroyed(CFieldMonster *i_pFMon, CFieldMapChannel *i_pFMapChann);
	static void OnInfluenceBossMonsterDead(CFieldMonster *i_pFMon, CFieldMapChannel *i_pFMapChann, CGuildDamageManager *i_pGDamageManager);

	static BOOL IsValidStrategyPointMonster(CFieldMonster *i_pFMon, CFieldMapChannel *i_pFMapChann);
	static void OnStrategyPointMonsterCreated(CFieldMonster *i_pFMon, CFieldMapChannel *i_pFMapChann);
	static void OnStrategyPointMonsterAutoDestroyed(CFieldMonster *i_pFMon, CFieldMapChannel *i_pFMapChann);
	static void OnStrategyPointMonsterDead(CFieldMonster *i_pFMon, CFieldMapChannel *i_pFMapChann);

	static void OnTeleportMonsterCreated(CFieldMonster *i_pFMon, CFieldMapChannel *i_pFMapChann);		// 2007-09-20 by cmkwon, �ڷ���Ʈ ��ȯ ���� ����
	static void OnTeleportMonsterDead(CFieldMonster *i_pFMon, CFieldMapChannel *i_pFMapChann);			// 2007-09-20 by cmkwon, �ڷ���Ʈ ��ȯ ���� ����
#ifdef NEMERIAN_NATION_BALANCE_BUFF_PENALITY
	static void ResetAllBuff(BYTE InflType);
#endif

	// 2006-12-04 by dhjin
	static BOOL IsValidCharacterEnemy(CFieldIOCPSocket *i_pAttFISoc, CFieldIOCPSocket *i_pTarFISoc, BOOL i_bTargetLiveCheck=TRUE);
	static BOOL IsValidCharacterFriend(CFieldIOCPSocket *i_pAttFISoc, CFieldIOCPSocket *i_pTarFISoc, BOOL i_bTargetLiveCheck=TRUE);

	// 2007-02-12 by cmkwon
	static BOOL IsAllSameMap(CFieldMapChannel *pFMChann, vector<CFieldIOCPSocket*> *i_pvectFISockList);

	BOOL OnPrimaryAttack( BYTE i_Atktype );					// 1�� ���� �߻�� ó���ؾ� �� �͵�, check: currentBulletCount ���ʿ��ϴٰ� �����Ǹ� ������ �����ϱ�, 20031203, kelovon
	
	int OnSecondaryAttack( int shotCount, BYTE i_atktype = NULL );	// 2�� ���� �߻�� ó���ؾ� �� �͵�: shotnum, multinum, reattacktime, HPȸ��������, ź�� �� ó��, currentBulletCount = -1�̸� ó�� �� ��, check: currentBulletCount ���ʿ��ϴٰ� �����Ǹ� ������ �����ϱ�, 20031203, kelovon
	int RechargeBullet(ITEM_GENERAL *pItemWeapon, BOOL bBulletConsumption = TRUE, BYTE i_byRechargeBulletTy=BULLET_RECHARGE_TYPE_NORMAL);		// 2007-08-07 by cmkwon, 1��/2�� ���� �Ѿ� ���� ������ ���� - ���� �߰�, // źâ ä��, returns(recharge�� �Ѿ��� ��, 0�̸� recharge �� ��)
	void SaveCurrentBulletCount(ITEM_GENERAL *pItemWeapon);				// ������ �Ѿ� ���� DB�� ������

	BYTE CalcDamageKind(float fDistance, float fDistanceVar = 0.0f);	// �������� ������ ���� fDistanceVar�� ������Ŵ

	int SupplyBullet(ITEM_GENERAL *i_pWeaponItem, ITEM *i_pBulletItem, int i_nSupplyCount);		// źâ ä��, returns(recharge�� �Ѿ��� ��, 0�̸� recharge �� ��)


	// virtual functions

	// 2008-03-11 by cmkwon, ���� �ñ� ���� ������ ���� - 
	BOOL PreSendAddData(BYTE *i_pbyData, int i_nSize, int i_nWriteBufCnts, vectSSendedOverlappedInfo *i_pSendedOverInfoList);
	void OnSendAddData(SSendedOverlappedInfo *i_pSendedOverInfo, int i_nSize, int i_nWriteBufCnts) override;
	
	///////////////////////////////////////////////////////////////////////////////
	// 2008-03-19 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ ��(lack) ���� ���� - 
	void SaveDataOfWriteBufList();
	
	BOOL OnRecvdPacket(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP = "", int nPeerPort = 0, SThreadInfo *i_pThreadInfo=NULL) override;
	BOOL OnRecvdPacketUDP(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP = "", int nPeerPort = 0, SThreadInfo *i_pThreadInfo=NULL);
	BOOL OnRecvdPacketTCP(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP = "", int nPeerPort = 0, SThreadInfo *i_pThreadInfo=NULL);
	
	void OnConnect() override;
	void OnClose(int reason) override;
	BOOL OnError(int errCode) override;


	BOOL ResCreateCharacter(CHARACTER *pCharacter, int nErr = 0, CHARACTER_RENDER_INFO *i_pRenderInfo=NULL);
	BOOL ResDeleteCharacter(MSG_FC_CHARACTER_DELETE *pMsgCharacterDelete, int nErr = 0);
	BOOL ResGetCharacterInfoByName(CHARACTER *pCharacter, int nErr = 0);
	BOOL ResGetAllCharacterInfoByID(MSG_FC_CONNECT_LOGIN_OK *pMsgConnectIDOK, SCASH_PREMEIUM_CARD_INFO *i_pCashCardInfo, BYTE i_bySelectableInfluenceMask, int nErr = 0);



	float GetCurrentLoadagePercentage(float i_fWeightPlus = 0.0f);					// ������ ��ȯ
	float GetTradeItemWeight();														// �ŷ� �������� ���� ��ȯ
	void SetSkillUseTime(UID64_t i_skillUID, ATUM_DATE_TIME i_atUseTime);			// 2007-02-07 by dhjin, ��ų ��� �ð� ����.



	BOOL AttachItem(int position, ITEM_GENERAL *pItem, BOOL bCheckWear = FALSE);

	void DetachItem(int position);

	Err_t CheckAttachableItem(INT NewPosition, ITEM_GENERAL *i_pItemGen);
	BOOL CheckItemWindowPosition(INT NewPosition, ITEM* itemInfo);
	void SwapItem(ITEM *pa, ITEM *pb);
	void SetWear(ITEM_GENERAL* pItem);
	void UpdateWearItemPointer(UID64_t MoveItemUniqueNumber, UID64_t DestItemUniqueNumber, int MovePos, int DestPos);

	// 2010-03-11 by cmkwon, ResetAllSkillAndEnchant�Լ� ȣ�� ���� ����(called OnAttach or OnDetach) - 
	void OnUpdateWearItem(UID64_t MoveItemUniqueNumber, UID64_t DestItemUniqueNumber, int MovePos, int DestPos);

	ITEM *GetAttachedItem(BYTE i_nPosition);
	void OnAttachItem(ITEM_GENERAL *i_pItemGeneral, BYTE i_nTargetItemWindowIndex);
	void OnDetachItem(ITEM_GENERAL *i_pItemGeneral, BYTE i_nSourceItemWindowIndex, BOOL i_bSendChangeInfo=TRUE, BOOL i_bDeleteUsingTimeLimitedItem=TRUE);
	ITEM_GENERAL *GetAttachItemGeneralByPosition(int i_position);

	// 2009-10-22 by cmkwon, �ð� �������� ������ �ý��� ���� - CFieldIOCPSocket::ProcessItemChangeWindowPosition# �����߰�(i_bNoValidCheck)
	ProcessResult ProcessItemChangeWindowPosition(MSG_FC_ITEM_CHANGE_WINDOW_POSITION *i_pItemChangeWindowPos, BOOL i_bDeleteUsingTimeLimitedItem=TRUE, BOOL i_bNoValidCheck=TRUE);

	Err_t ApplyCardItem(ITEM *i_pItemInfo, BOOL i_bUseFlag=FALSE, char *i_szChatMsg=NULL);	// 2007-08-09 by cmkwon, ��� ���¿� ä�� �����ϱ� - �����߰�(, char *i_szChatMsg=NULL)
	void ReleaseCardItem(ITEM *i_pItemInfo);

	// 2008-01-31 by cmkwon, �ð� ���� ������ ��� ���� �ý��� ���� - CFieldIOCPSocket::InsertTimeLimiteItem() ����
	BOOL InsertTimeLimiteItem(ITEM *i_pTimeLimiteItemInfo, BOOL i_bCheckAlreadyUsing8Apply=TRUE);

	// 2013-05-09 by hskim, ���� ����Ʈ ����
	bool GetStealthState() const { return m_bStealthState || m_bStealthStateBySkill; }

	BOOL ApplyBuffSkill(INT nItemNum);
	BOOL ReleaseBuffSkill(INT nItemNum);
	BOOL ReleaseBuffSkillByDesParam(DestParam_t desSame);

	BOOL ApplyBuffSkillByTurnAround();		// ������ ���� ���� (�ش��ڸ�)
	// 2013-05-09 by hskim, ���� ����Ʈ ����

	BOOL IsTransferItem(ITEM_GENERAL *i_pItemG, BOOL i_bBazaar=FALSE);
	BOOL IsTransferItem(ITEM *i_pItemInfo, BOOL i_bBazaar=FALSE);

	// ��� ������ ����
	void SendDropItems(CMapBlock *pBlock);				// check: DROPITEM�� DROPMINE�� �Բ� �Ѹ���
	void SendDropItemsAroundPosition(D3DXVECTOR3 &pos);

	// ���� ����
	void ClearAllCharacterMines();						// ĳ������ �ѷ����� ��� ������ �����Ѵ�
	BOOL IsValidDropMine(DROPMINE *i_pDropMine, CMapBlock *&o_pMapBlock);

	// ������ ���� ���� Ȯ��
	Err_t CheckItemRequirements(ITEM *i_pItemInfo);
	Err_t CheckGeneralItemRequirements(ITEM_GENERAL *i_pGItem);
	Err_t CheckSkillItemRequirements(ITEM *i_pSItemInfo);

	// ���λ� ���̻� ����
	static void ApplyRareItem(CFieldMonster *i_pMonster, MONSTER2ITEM *i_pMonster2Item, float i_fPlusRateDropRareItem, CFieldIOCP *i_pFieldIOCP, ITEM_GENERAL *io_pItem);

	// 2008-10-21 by cmkwon, �������� ���õ� ����� �ٽ� ������ �ʰ� ���� - �����߰�(, RARE_ITEM_INFO *i_pLastRareItemInfo/*=NULL*/)
	BOOL ApplyGambleRareFix(ITEM *i_pGambleItemInfo, ITEM_GENERAL *io_pItem, RARE_ITEM_INFO *i_pLastRareItemInfo=NULL);

	BOOL ApplyGambleRareFix(ITEM_GENERAL *io_pItemG, int i_n100KPrefixProbability, int i_n100KSuffixProbability);

	// ���� ĳ���� ���� �Լ�
	BOOL StopItemKindShield(ITEM *pItemShield);
	BOOL StopItemKindDecoy(ITEM *pItemDecoy);

	// 2007-12-04 by cmkwon, ���� �����ϱ� ���̺귯�� ���� - CashItemMoneyPay() ���� �߰�, UID32_t i_giveTargetCharUID=0
	// 2008-12-18 by cmkwon, �Ϻ� Arario �ܺ�����, ���� ��� �����ϱ� - ���� �߰�(, int i_nCurTotalMoney=0)
	Err_t CashItemMoneyPay(INT *o_pTotalMoney, INT *o_pMCash, INT *o_pGiftCard, ITEM *i_pItem = NULL, int itemPrice = 0, int i_nTotalPrice = 0, UID32_t i_giveTargetCharUID = 0, int i_nCurTotalMoney = 0);
	constexpr static bool IsChargeUser() { return true; }
	constexpr static bool  IsValidGameUser(Experience_t* o_pExptMaxEXP = nullptr, Experience_t i_CurEXP = 0.0f) { return true; }


	///////////////////////////////////////////////////////////////////////////////
	// 2008-04-25 by cmkwon, ���� ���/���� �߰��� �� �߰� �Ǿ�� �ϴ� ���� - [����-�ʼ�] ���� �Լ� ����

	// 2012-08-17 by jhseol, ���� BPSoft �����ý���
	Err_t CashItem_BPSOFT(INT *o_pTotalMoney, INT *o_pMCash, INT *o_pFreeCash, ITEM *i_pItem=NULL);
	Err_t GetCashPay_BPSOFT(INT *o_pTotalMoney, INT *o_pMCash, INT *o_pFreeCash);
	Err_t BuyCashItem_BPSOFT(INT *o_pTotalMoney, INT *o_pMCash, INT *o_pFreeCash, ITEM *i_pItem);
	void GetWebAccountNum(char* i_szAccountName, UID32_t* o_WebAccountNumber, int* o_nErrNum); // 2012-10-18 by jhseol, BPSoft ���� ��� - ����(=WEB)DB ���� ������ �ѹ� ��������
	// end 2012-08-17 by jhseol, ���� BPSoft �����ý���


#ifdef SHOP_PRICES_PER_BUILDING_NPC
	Err_t CashItem_Wikigames(INT *o_pTotalMoney, INT *o_pMCash, INT *o_pGiftCard, ITEM *i_pItem = NULL, int itemPrice = 0, int i_nTotalPrice = 0, UID32_t i_giveTargetCharUID = 0);
#else
	Err_t CashItem_Wikigames(INT *o_pTotalMoney, INT *o_pMCash, INT *o_pGiftCard, ITEM *i_pItem = NULL, int i_nTotalPrice = 0, UID32_t i_giveTargetCharUID = 0);
#endif // SHOP_PRICES_PER_BUILDING_NPC

	
	int CashItem_Wikigames_DirectConnect(SWIKIBILL_REQ_DATA *i_pReqData, SWIKIBILL_RET_DATA *o_pRetData);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-10-06 by cmkwon, ��Ʈ�� ���� ���� X-TRAP���� ���� - 
	static BOOL				ms_XTrapUsingFlag;


	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-01 by cmkwon, ����� ���� ���� �ִ� �ϼ� - ���󺰷� �ٸ���
	static INT GetMaxBuyableDayOfMembership(void);

	// 2006-02-09 by cmkwon, ų��ũ ����
	ITEM *GetGiveKillMarkItemPtr2Killer(void);
	void ResetGiveKillMarkItemPtr2Killer(void);

	// quest
protected:

public:
	Err_t CheckQuestStartRequirements(CQuest *pQuestInfo, MSG_FC_QUEST_REQUEST_START *i_pQuestStart, BOOL i_bProcessPartyMembers/*=TRUE*/);		// ����Ʈ ���� �ʿ� ���� Ȯ��
	CFieldCharacterQuest* GetCharacterQuestInProgress(void);			// �������� ����Ʈ ��ȯ.
	Err_t CheckQuestCompetionRequirements(CFieldCharacterQuest *i_pFCharQuest, MSG_FC_QUEST_REQUEST_SUCCESS *i_pQuestSuccess
											, MapIndex_t i_nMapIndex, INT i_nWarpTargetIndex, INT i_nAttMonster=0, BOOL i_bByUser=TRUE);	// ����Ʈ ���� ����(�Ϸ� ����) Ȯ��
	ProcessResult CheckQuestRequestSuccessAndSendResult(CFieldCharacterQuest *i_pFCharQuest, MSG_FC_QUEST_REQUEST_SUCCESS *i_pQuestSuccess
											, BOOL i_bSendErrorMessage=TRUE, MapIndex_t i_nMapIndex=0, INT i_nWarpTargetIndex=0
											, INT i_nAttMonster=0, BOOL i_bOnlyCompletionCheck=FALSE);
	void CheckSuccessAllQuestByWarpTargetIndex(MapIndex_t i_nMapIndex, INT i_nWarpTargetIndex);
	void CheckSuccessAllQuestByAttackMonsterNum(INT i_nAttackMonsterNum);
	
	// 2007-12-06 by cmkwon, �ð����� ����Ʈ �������� �ð� üũ�ϰ� ���� - CheckSuccessProgressQuestByTimeLimited() �߰�
	void CheckSuccessProgressQuestByTimeLimited(void);
	
	BOOL ProcessQuestResult(int *o_pnExpOfCompensation, CQuest *pQuestInfo, int i_nQuestResultParam=0
							, MSG_FC_QUEST_REQUEST_SUCCESS_RESULT * o_pQuestResult = NULL
							, QUEST_PAY_ITEM_INFO * o_QuestPayItemInfo = NULL);
	BOOL ProcessQuestStartAction(MapIndex_t *o_pWarpMapIndex, BOOL *o_pbPartyWarp, CQuest *pQuestInfo);
	BOOL InsertQuestPayItem(QUEST_PAY_ITEM *i_pQuestPayItem);
	BOOL IsExistQuestCityWar(void);
	BOOL CityWarQuestResult(INT i_questIdx, BOOL i_bSuccess);
	BOOL IsCheckQuestComplited(INT i_questIdx);
	BOOL GMQuestCompletion(int i_questIdx=-1);
	BOOL GMQuestCompletion(CQuest *i_pQuestInfo);
	BOOL GMQuestAdjustByInfluenceType(BYTE i_byInflTy);
	CFieldCharacterQuest * GetFieldCharacterQuestPtrByQuestIndex(INT i_questIdx);

	void OnGetAllQuest(mtmapQuestIndex2FieldCharacterQuest *i_pmtmapCharacterQuest);

	// 2009-01-13 by cmkwon, �̼� ������ ��Ƽ�� ��ο��� �ֱ� ��ɾ� �߰� - ���� �߰�(, BOOL i_bAttacker=TRUE)
	BOOL AddCharacterQuestMonsterCount(int i_monUniqueNumber, BOOL i_bTopAttacker, BOOL i_bAttacker=TRUE);
	BOOL IsQuestStateInProgressByQuestIndex(int i_quesIndex);
	BOOL CheckQuestCompetionRequirementsByQuestIndex(int i_quesIndex, MSG_FC_QUEST_REQUEST_SUCCESS *i_pRequestSuccess);
	Err_t CheckQuestStartByQuestIndex(int *o_pnErrParam1, MSG_FC_QUEST_REQUEST_START *i_pQuestRequestStart, BOOL i_bProcessPartyMembers=TRUE);
	
	// �ŷ�
	CFieldIOCPSocket* SendTradeCanceledAndReset(CFieldIOCPSocket *pPeerSocket, BOOL bSendToPeer);
	Err_t TradeCheckEnableInsertItems(CFieldIOCPSocket *i_pPeerFISock);
	void TradeInsertItems(CFieldIOCPSocket *i_pPeerFISock);
	// 2010-06-15 by shcho&hslee ��ý��� - �ŷ� �� �� ���� ó��
	void TradePetData(UID64_t i_tradeUniqeNum, CFieldIOCPSocket *i_pPeerFISock);
	// end 2010-06-15 by shcho&hslee ��ý��� - �ŷ� �� �� ���� ó��

	// PK
	CFieldIOCPSocket* SendP2PPKEndAndReset(CFieldIOCPSocket *pPeerSocket, BOOL bSendToPeer);
	static void OnP2PPKEnd(CFieldIOCPSocket *pWinnerSocket, CFieldIOCPSocket *pLoserSocket);	// �ϴ��� ���� ��� ó��, �������� ��� Reset��

	// 2010-03-10 by cmkwon, ���ǿ��� �ð����� �׼����� �߰� ���� ���� ���� - 
	BOOL InitCharacterInventoryItem(BOOL i_bStartFromArena=FALSE);
	BOOL ResetAllSkillAndEnchant();	// ��� skill & enchant & rare fix ����
//	BOOL InsertItemListByCharacterFirstGameStartFromDB(vectITEM_GENERALPtr *i_pvectItemGPtrList, vectENCHANT *i_pvectEnchantList, vectItemSkillPtr *i_pvectItemSkillPtrList);	// 2007-12-21 by dhjin, ���� ���� ��ƾ DB�� ������ Client���� �κ� �и�
	
	// 2010-06-15 by shcho&hslee ��ý��� - �� �������� ���� �޾ƿ��� ���� �߰�
	BOOL CFieldIOCPSocket::InsertItemListByCharacterFirstGameStartFromDB(vectITEM_GENERALPtr *i_pvectItemGPtrList, vectENCHANT *i_pvectEnchantList, vectItemSkillPtr *i_pvectItemSkillPtrList , vecPetCurrentInfo *i_vectPetCurInfo);	// 2007-12-21 by dhjin, ���� ���� ��ƾ DB�� ������ Client���� �κ� �и�

// 2008-01-31 by cmkwon, ������� �ʴ� �Լ� �ּ� ó��
// 	BOOL InsertItemListFromDB(MSG_FC_STORE_GET_ITEM* i_pStoreGetItem, vectITEM_GENERALPtr *i_pvectItemGPtrList, vectENCHANT *i_pvectEnchantList, vectItemSkillPtr *i_pvectItemSkillPtrList=NULL);
// 	BOOL InsertCharacterItemListFromDB(vectITEM_GENERALPtr *i_pvectItemGPtrList, vectENCHANT *i_pvectEnchantList);	// 2007-12-14 by dhjin, ���� ���� ��ƾ DB�� ������ Client���� �κ� �и�
	void SendCharacterItemList();			// 2007-12-14 by dhjin, ���� ���� ��ƾ DB�� ������ Client���� �κ� �и�

	// 2008-01-10 by cmkwon, ���� â�� ���� ���� - CFieldIOCPSocket::InsertStoreItemListFromDB() �� ���� �߰�
	BOOL InsertStoreItemListFromDB(vectITEM_GENERALPtr *i_pvectItemGPtrList, vectENCHANT *i_pvectEnchantList, UID32_t i_possessCharacterUID, vecPetCurrentInfo *i_vectPetCurInfo);		// 2012-01-14 by hskim, ��Ʈ�� �ý��� 2�� - â���� ���� �� Ŭ�� ƨ��� ���� ���� - // 2007-12-14 by dhjin, ���� ���� ��ƾ DB�� ������ Client���� �κ� �и�
	
	void SendStoreItemList();				// 2007-12-14 by dhjin, ���� ���� ��ƾ DB�� ������ Client���� �κ� �и�
	BOOL InsertSkillListFromDB(vectItemSkillPtr *i_pvectItemSkillPtrList);			// 2007-12-14 by dhjin, ���� ���� ��ƾ DB�� ������ Client���� �κ� �и�
	void SendSkillItemList();				// 2007-12-14 by dhjin, ���� ���� ��ƾ DB�� ������ Client���� �κ� �и�
	void SendQuestList();					// 2007-12-14 by dhjin, ���� ���� ��ƾ DB�� ������ Client���� �κ� �и�
	void SendPetItemList();					// 2010-06-15 by shcho&hslee ��ý��� - ó�� ���� ���۽� Ŭ���̾�Ʈ ��Ŷ ���� �Լ� �߰� 
	void SendInsertPetItemInfo(BYTE ItemInsertionType, tPET_CURRENTINFO* pcurrentPetInfo);			// 2010-06-15 by shcho&hslee ��ý��� - �� ������ �߰��� Ŭ���̾�Ʈ ��Ŷ ���� �Լ� �߰� 
	

	// enchant
	float GetEnchantValue(ITEM *pTargetItem, BYTE desParam); 

	BOOL ApplyEnchant(ITEM_GENERAL *pTargetItemG);			// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
	BOOL ApplyDestParam8Enchant8RareOfAttachedItems(void);	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
	BOOL ItemOptionInitialization(ITEM_GENERAL *pItemGeneral);	// 2010-11-12 by khkim �޸���:14(����) �α� ���� ����

	BOOL InsertEnchantToItem(ITEM_GENERAL *pTargetItem, ENCHANT &enchant);
	BOOL InsertEnchantToItem(ITEM_GENERAL *pTargetItem, vectENCHANT *i_pvectEnchant, BOOL bSendToClient=TRUE);
	BOOL DeleteAllEnchantToItem(UID64_t i_itemUID);
	void PrintEnchant(UID64_t ItemUniqueNumber = 0, BOOL i_bWriteSystemLog = FALSE);	// ItemUniqueNumber �� 0�̸� ��� ���
	BOOL PopAllEnchantByItemUID(vectENCHANT *o_pvectEnchat, UID64_t i_itemUID);

	vectENCHANT *GetVectEnchantListPtr(UID64_t i_itemUID);

	// skill
	BOOL SetParamFactor(DestParam_t desParam, float paramValue, ClientIndex_t i_SkillUseClientIdx=0);		// 2011-08-01 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ��� ���� (DestParameter - 255 -> 32767 ����)
	BOOL UnsetParamFactor(DestParam_t desParam, float paramValue);		// 2011-08-01 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ��� ���� (DestParameter - 255 -> 32767 ����)
	BOOL SetParamFactor(CParamFactor *i_pParamFactor, DestParam_t desParam, float paramValue, ClientIndex_t i_SkillUseClientIdx=0);		// 2011-08-01 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ��� ���� (DestParameter - 255 -> 32767 ����)
	BOOL UnsetParamFactor(CParamFactor *i_pParamFactor, DestParam_t desParam, float paramValue);		// 2011-08-01 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ��� ���� (DestParameter - 255 -> 32767 ����)
	
	const CParamFactor* GetParamFactor() const { return &m_ParamFactor; }

	bool CheckSKILLInvincible() const { return GetParamFactor()->pfb_BitFlag.pfb_SKILL_Invincible; }
	bool CheckSKILLBarrier() const { return GetParamFactor()->pfb_BitFlag.pfb_SKILL_Barrier; }
	bool CheckSKILLChainRolling() const { return GetParamFactor()->pfb_BitFlag.pfb_SKILL_ROLLING_TIME; }

	bool IsEnableRepairDP();
	float CalcSKILLBigBoom() const { return GetParamFactor()->pf_SKILL_Big_Boom_damage; }

	bool IsDamageDistributionToPartyMember() const { return GetParamFactor()->pfb_BitFlag.pfb_SKILL_DamageDistribution; }

	// 2007-12-13 by cmkwon, ������ �Ǽ��縮 ���߱�� ���� - Current ���� ���� ���ִ� ����� ���ŵ�
	BOOL SetParamFactor_POS_ATTACHMENT(BYTE desParam, float paramValue);	
	BOOL UnsetParamFactor_POS_ATTACHMENT(BYTE desParam, float paramValue);
	USHORT GetParamValue_POS_ATTACHMENT(BYTE desParam);

	// Rare Item�� Suffix & Prefix ����, ������ ������(����)�� ���ؼ��� �ش��
	BOOL ApplyRareFix(ITEM_GENERAL *pItemGeneral);

	// ��Ƽ
	void PartyGetMemberClientIdxListExcludeMe(vectorClientIndex *o_pVectClientIndex, ClientIndex_t i_excludeCltIdx=0, BOOL i_bOnlySameMapChannel=FALSE, BOOL i_bOnlyNoBodyConEventHandle=FALSE);
	UID32_t GetPartyMasterUniqueNumber();
	BOOL IsPartyMaster(void);
	BOOL IsSamePartyByClientIndex(ClientIndex_t clientIndex);
	BOOL IsSamePartyByCharacterUniqueNumber(UID32_t CharacterUniqueNumber);
	BOOL IsSameParty(CFieldIOCPSocket *pSocket);
	void OnPartyBattleMemberDead(BOOL bImediateLoss = FALSE);	// ��Ƽ������ ���� Ȥ�� ���� �����, ��� �й� ó���� �� �� ���� ����
	BOOL IsPartyBattleState(void);								// ��Ƽ���� �ϰ� �ִ°�?
	static void OnPartyBattleEnd(CFieldParty *pWinnerParty, CFieldParty *pLoserParty);

	// ���ǵ��� üũ
	void CheckSpeedHack(DWORD i_dwClientTickGap, float i_fServerDistaceGap);
	BOOL WriteLogMessageSpeedHack(BYTE i_bCheckType);

	// 2005-12-21 by cmkwon, �޸��� üũ
	void CheckMemoryHackWeapon(ITEM_GENERAL *i_pItemGen, MSG_FC_CHARACTER_GET_REAL_WEAPON_INFO_OK *i_pRealInfo);
	void CheckMemoryHackEngine(ITEM_GENERAL *i_pItemGen, MSG_FC_CHARACTER_GET_REAL_ENGINE_INFO_OK *i_pRealInfo);
	void CheckMemoryHackTotalWeight(ITEM_GENERAL *i_pItemGen, MSG_FC_CHARACTER_GET_REAL_TOTAL_WEIGHT_OK *i_pRealInfo);
	BOOL CheckMemoryHackByMemoryHackCheckTy(ITEM_GENERAL *i_pItemGen, int i_nMemoryHackCheckTy, float i_fValidValue, float i_fCurrentValue, int i_nParam1=0);

	// TargetMonsterIndex ����
	void InsertAttackTargetIndexVector(ClientIndex_t i_Index);
	void DeleteAttackTargetIndexVector(ClientIndex_t i_Index);
	void DeleteMeFromMonsterAttackedList(void);

	void IncrementInsertingStoreCounts(void);
	void DecrementInsertingStoreCounts(void);


	// 2005-12-28 by cmkwon, �������� ���� �Լ�
	void InfluenceWarCheckInvasion(void);
#ifdef NEMERIAN_PVP_AWARD_EFFORT
	void InfluenceWarBonus2Killer(CFieldIOCPSocket *i_pFISockDeaded, unsigned int killMarkQty = 0);
#else
	void InfluenceWarBonus2Killer(CFieldIOCPSocket *i_pFISockDeaded);
#endif
	///////////////////////////////////////////////////////////////////////////////
	// 2006-06-05 by cmkwon
	BOOL					m_Security_bCheckVersion;						// 2006-06-05 by cmkwon
// 2008-04-03 by cmkwon, �ٽ��� ���� ���� �ý��� ���� - ������� ����
//	unsigned char			m_Security_pbyGuidReqInfo[SIZEOF_GUIDREQINFO];	// 2006-06-05 by cmkwon
//
//	// 2008-03-24 by cmkwon, �ٽ��� 2.0 ���� - 
//	//unsigned long *			m_Security_plCRCInfo;							// 2006-06-05 by cmkwon
//	HSHIELD_CLIENT_CONTEXT	m_Security_ClientContext;		// 2008-03-24 by cmkwon, �ٽ��� 2.0 ���� - �߰���

	BOOL					m_Security_bSendCRCReqMsg;						// 2006-06-08 by cmkwon
// 2008-04-03 by cmkwon, �ٽ��� ���� ���� �ý��� ���� - ������� ����
//	unsigned char			m_Security_pbyReqInfo[SIZEOF_REQINFO];			// 2006-06-05 by cmkwon
//	unsigned long			m_Security_ulOption;
//	Err_t SecurityCheckVersion(MSG_FC_MOVE_HACKSHIELD_GuidAckMsg *i_pGuidAckMsg=NULL);
//	Err_t SecurityCheckClient_1(MSG_FC_MOVE_HACKSHIELD_CRCAckMsg *i_pCRCAckMsg=NULL);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-11-04 by cmkwon, �±� ���Ӱ��� Apex�� ���� - 
	static BOOL				ms_bUsingSecuritySystem;
	BOOL					m_bInitSecurityClient;
	BOOL GetInitSecurityClient(void);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-04-03 by cmkwon, �ٽ��� ���� ���� �ý��� ���� - 

	static AHNHS_SERVER_HANDLE	ms_hAhnHSServerHandle;
	static BOOL SecurityServerInit(void);
	static void SecurityServerClean(void);

	AHNHS_CLIENT_HANDLE			m_hAhnHSClientHandle;
	Err_t SecurityClientInit(void);
	void SecurityClientClean(void);
	// 2009-03-09 by cmkwon, �Ϻ� Arario nProtect�� CS���� �����ϱ� - �����߰�(, BOOL i_bCheckTerm=TRUE)
	Err_t SecurityClientCheck(MSG_FC_MOVE_HACKSHIELD_CRCAckMsg *i_pCRCAckMsg=NULL, BOOL i_bCheckTerm=TRUE);

	DWORD					m_dwSetTickSecurity_bSendCRCReqMsg;				// 2006-10-21 by cmkwon, �׽�Ʈ

	DWORD					m_dwReadyTickforSecurityCheck;					// 2009-03-09 by cmkwon, �Ϻ� Arario nProtect�� CS���� �����ϱ� -
	static int				ms_nPeriodMinutesforSecurityCheck;				// 2009-03-09 by cmkwon, �Ϻ� Arario nProtect�� CS���� �����ϱ� - 

	///////////////////////////////////////////////////////////////////////////////
	// 2006-07-21 by cmkwon, 
	mtvectActionInfo			m_mtvectActionInfoAggroList;
	void AddAggroList(ActionInfo *i_pActionInfoAggro);
	BOOL GetAggroList(vectActionInfo *o_pvectActionInfo);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-07-26 by cmkwon, ���� ����
	CBazaarManager					m_bazaarManager;				// 2006-07-26 by cmkwon
	BOOL MakeMSG_FC_BAZAAR_INFO_OKW(MSG_FC_BAZAAR_INFO_OK *o_pBazaarInfoOK);
	BOOL SendBazaarItemList(CFieldIOCPSocket *i_pFISock, BYTE i_byBazaarTy);
	void SendEnchatItemNumList(CFieldIOCPSocket *i_pFISock, vectUID64_t *i_pItemUIDList);
	BOOL BazaarInsertItem(CFieldIOCPSocket *i_pFromFISock, ITEM_GENERAL *i_pInsertItemGen, int i_nItemCnts, BYTE i_nItemUpdateType=IUT_BAZAAR_SELL);

	BOOL CheckCharacterUIDFormAccount(UID32_t i_CharactUID);		// 2006-09-15 by dhjin, ������ �ɸ��Ͱ� �����ϴ��� üũ
	BOOL DeleteCharacterUIDFormAccount(UID32_t i_CharactUID);		// 2006-09-15 by dhjin, ������ �ɸ��� ����.
	FC_CONNECT_LOGIN_INFO * GetFC_CONNECT_LOGIN_INFONoLock(UID32_t i_CharactUID);		// 2006-10-18 by cmkwon

	// START 2011-11-03 by shcho, yedang �˴ٿ��� ���� - üũ �Լ� �߰�
	BOOL CheckCharacterUIDFormSHOTDOWNMINS(UID32_t i_CharactUID);
	// END 2011-11-03 by shcho, yedang �˴ٿ��� ���� - üũ �Լ� �߰�

	///////////////////////////////////////////////////////////////////////////////
	// 2009-11-02 by cmkwon, ĳ��(�κ�/â�� Ȯ��) ������ �߰� ���� - 
	BYTE GetAddedPermanentInventoryCountOfOthersCharacter(UID32_t i_CharactUID);
	BOOL SetAddedPermanentInventoryCountOfOthersCharacter(UID32_t i_CharactUID, int i_nRacingPoint);


	///////////////////////////////////////////////////////////////////////////////
	// 2006-09-28 by cmkwon
	CHARACTER *GetCharacterCacheNoLock(UID32_t i_charUID);
	BOOL UpdateCharacterCacheLock(UID32_t i_charUID, CHARACTER *i_pCharac);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-10-09 by cmkwon
	BOOL InsertEventItem(int i_itemNum, int i_nCnts, BYTE i_nItemUpdateType, int i_nPrefixCodeNum = 0, int i_nSuffixCodeNum = 0, BYTE i_enumStorage=ITEM_IN_CHARACTER);	// 2013-04-22 by jhseol, ������ �̺�Ʈ - ����Ÿ�� �߰����� �ý��� ����

	///////////////////////////////////////////////////////////////////////////////
	// 2006-10-16 by cmkwon
	BOOL						m_bQuestRequestWarp;					// 2006-10-16 by cmkwon
	DWORD						m_dwLastQuestRequestWarpTick;			// 2006-10-16 by cmkwon
	MAP_CHANNEL_INDEX			m_beforeMapChannIdxQuestRequestWarp;	// 2006-10-16 by cmkwon, ��Ƽ ���� ������ MapChannelIndex

	///////////////////////////////////////////////////////////////////////////////
	// 2006-11-08 by cmkwon
	BOOL						m_bIsInvisibleCharacter;				// 2006-11-08 by cmkwon
	BOOL IsInvisibleCharacter(void);

	//////////////////////////////////////////////////////////////////////////
	// 2006-11-24 by dhjin
	BOOL						m_bNoDamage;

	//////////////////////////////////////////////////////////////////////////
	// 2012-06-14 by jhseol, �Ʒ��� �߰�����part2 - ���� : �Ʒ��� ������ �� ���� �ð�
	ATUM_DATE_TIME				m_atimeArenaNoDamage;
	BOOL						m_bArenaNoDamage;
	// end 2012-06-14 by jhseol, �Ʒ��� �߰�����part2 - ���� : �Ʒ��� ������ �� ���� �ð�

	///////////////////////////////////////////////////////////////////////////////
	// 2006-11-15 by cmkwon
	ATUM_DATE_TIME		m_atmeGameStartTime;						// 2006-11-15 by cmkwon, ���� ���� �ð�
	int					m_nGameContinueTimeInSecondOfToday;			// 2006-11-15 by cmkwon, ���� ���� ���� �ð�
	int					m_nGameContinueTimeInSecondOfToday4Viet;	// 2007-10-23 by cmkwon, ��Ʈ�� ���� ���� �ð� �ý���  ���� - ����â���� ���ٰ� ���� ���۽� ó�� ���ӽ� ���ӽð����� �����ϱ� ���� ����
	ATUM_DATE_TIME		m_atimeLastGameEndDate;						// 2006-11-15 by cmkwon, ������ ���� ���� ��¥
	DWORD				m_dwSendPlayTimeInfoLastTick;				// 2007-06-28 by cmkwon, �߱� ��������(���� �ð� �˸� ����) - 
	int	GetCurGameContinueTimeInSecondOfToday(void);
	void GetPlayTimeInfoString(char *o_szPlayTimeString);
	//BOOL GPGetApplyRateByGamePlayingTime(float *o_pfRetRate);		// 2007-08-13 by cmkwon, ���ڸ� ����(<--i_pfRetRate), // 2007-06-14 by cmkwon, ���� ���� �ð� ���� �⺻ �ý������� ����
	BOOL GPInitGameStartTime(void);									// 2007-06-19 by cmkwon, ���� ���� �ð� ���� - �߱� 5�ð� �� ���� ó��

	// 2007-06-27 by cmkwon, �߱� ����� �ý��� ���� - �̼����� ����
	BOOL				m_bIsAdult;			
	BOOL IsAdultsUser(void);
	BOOL SetAdults(ATUM_DATE_TIME i_atBirthday);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-12-04 by dhjin, ü�� ���� 
	mtvectFieldDummy		m_mtvectFieldDummy;
	UINT					m_uiDummyItemFieldIndex;
	BOOL					m_bDummyCheck;
	FIELD_DUMMY* FindFieldDummyNoLock(UINT i_ItemFieldIndex);
	BOOL DeleteFieldDummy(UINT i_ItemFieldIndex);
	void DeleteAllFieldDummy(void);
	BOOL ApplyFieldDummyDamage(float i_fChangeValue, BYTE i_enumDamageType);
	BOOL Make_MSG_FC_BATTLE_DROP_DUMMY_OK(BYTE *o_pSendBuf, int *o_pnSendByte, int i_nBufSize);

	//////////////////////////////////////////////////////////////////////////
	// 2007-03-28 by dhjin, ������ ����� ������ �������� üũ.
	BOOL ObserverTargetCharacterCheck(CFieldIOCPSocket *i_pOTFISock);
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-06-19 by dhjin, ������ ����� ��ȿ�� �������� üũ.
	BOOL WatchTargetCharacterCheck(CFieldIOCPSocket *i_pOTFISock);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-03-29 by cmkwon
	MSG_FC_MOVE_BIT_FLAG Get_MSG_FC_MOVE_BIT_FLAG(void);

	mtvectObersverCharacter m_mtvectObersverCharacter;		// 2007-03-27 by dhjin, �ڽ��� ���� �ִ� ��� ������ ���� ����.
	BOOL AddObserver(CFieldIOCPSocket *i_pObserverFISoc);
	BOOL PopObserver(CFieldIOCPSocket *i_pObserverFISoc);

	//////////////////////////////////////////////////////////////////////////
	// 2007-06-19 by dhjin, ������ ���� �߰�.
	mtvectWatchCharacter m_mtvectWatchCharacter;
	BOOL AddWatchUser(CFieldIOCPSocket *i_pObserverFISoc);
	BOOL PopWatchUser(CFieldIOCPSocket *i_pObserverFISoc);

	const char* GetCharacterLevelRank() const;							// 2007-04-24 by dhjin, �ɸ��� ���� ��ũ �������� �Լ�
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-06-04 by dhjin, �Ʒ��� ���� ������ ���޿� ���õ� �Լ� �߰�.
	BOOL InsertArenaItem(int i_itemNum, int i_nCnts, BYTE i_nItemUpdateType = IUT_ARENA_ITEM);	// 2007-06-04 by dhjin, �Ʒ��� ���� ������ ����

	///////////////////////////////////////////////////////////////////////////////
	// 2007-06-08 by cmkwon, 2������ ����Ȯ��,���ݷ�,�Ǿ�� ���� �ý��� ����
	mtvectSATTACK_PARAMETER		m_mtvectSAttackParameterList;		// 2007-06-08 by cmkwon, 2�� T_FC_BATTLE_ATTACK���� ����
	BOOL APInsertAttackParameter(SATTACK_PARAMETER *i_pAttParam);
	SATTACK_PARAMETER *APFindAttackParameterNoLock(UID16_t i_WeaponIndex);
	BOOL APPopAttackParameter(SATTACK_PARAMETER *o_pAttParam, UID16_t i_WeaponIndex);
	void APCalcAttckParameter(SATTACK_PARAMETER *o_pAttParam, ITEM *i_pWeaponItemInfo, UID16_t i_WeaponIndex, enumAttackToTarget eAttackToTarget, float i_PvPBuffPercent = 0.0f);	// 2013-08-01 by jhseol, ������ ���� ������ - i_PvPBuffPercent �߰�	// 2013-05-09 by hskim, ���� ����Ʈ ����

	// 2007-06-13 by cmkwon, ���� 7�� PC�� ���θ��(���̺�Ʈ ����) - �߰���
	BOOL WarriorPlaytimeEventCheck(int nCurrTick); //warriorsw 2015-05-14
	BOOL WarriorsUpdatePlayTime(int nAccID, int Value);//warriorsw 2015-05-14
	BOOL WarriorPlaytimeEvent(int nItemNum, int nItemCount, int nCurrTick); //warriorsw 2015-05-14
	void OnDoMinutelyWorkByTimerManager(void);

	// 2007-06-14 by cmkwon, ������ ���� ���� - 
	// 2009-01-22 by cmkwon, ���� ���� ���� �α� ���� - CFieldIOCPSocket::WPAddWarPoint()�� ����(, BYTE i_byWPUpdateTy=WPUT_GENERAL) �߰�
	void WPAddWarPoint(INT i_nAddWarPoint, BOOL i_bEventBonus, BOOL i_bSend2Client = TRUE, BYTE i_byWPUpdateTy = WPUT_GENERAL, BOOL i_bCalcWarPoint = TRUE);
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-06-15 by dhjin, ����
	void WatchReady();		// 2007-06-15 by dhjin, ���� ���� ���� �غ� ����.
	void WatchEnd();		// 2007-06-19 by dhjin, ���� ���� ��ɾ� �Է�.

	//////////////////////////////////////////////////////////////////////////
	// 2007-07-06 by dhjin, Tutorial
	mtvectTutorialList			m_mtvectTutorialList;						// 2007-07-06 by dhjin, Tutorial �Ϸ� ����
	void InitTutorial(vectTutorial * i_pvectSTutorialCompleteInfo);		// 2007-07-06 by dhjin, Tutorial �Ϸ� ������ �����Ѵ�.
	INT	MakeMSG_FC_TUTORIAL_START_OK(STUTORIAL_LIST_INFO * o_pTutorialListInfo);		// 2007-07-06 by dhjin, Tutorial �޼��� �����
	ITEM* GetTutorialWeaponInfo(BYTE i_byAttackType);			// 2007-07-16 by dhjin, �� ��� Ʃ�丮�� ���� ����
	STUTORIAL_LIST_INFO *GetTutorialDataNoLock(INT i_nTutorialNum);	// 2007-07-23 by cmkwon

	//////////////////////////////////////////////////////////////////////////
	// 2007-08-27 by dhjin, �ǰ��� ����
	static void SetExpediencyFundW(BYTE i_byInfluence, INT i_nSPICount);	// 2007-08-27 by dhjin, �ǰ��� ������Ʈ �Լ�

	///////////////////////////////////////////////////////////////////////////////
	// 2007-09-12 by cmkwon, ��Ʈ�� 2���н����� ���� - CFieldIOCPSocket �� ������� ����Լ� �߰�
	BOOL						m_bUnlockSecondaryPassword;								// 2007-09-12 by cmkwon, ��Ʈ�� 2���н����� ���� - CFieldIOCPSocket �� ��� ���� �߰�, ��� ����
	char						m_szSecondaryPassword[SIZE_MAX_PASSWORD_MD5_STRING];	// 2007-09-12 by cmkwon, ��Ʈ�� 2���н����� ���� - CFieldIOCPSocket �� ��� ���� �߰�, 2���н�����
	void SetSecondaryPassword(char *i_szSecPass);
	bool IsSettingSecondaryPassword() const { return *m_szSecondaryPassword != '\0'; }
	BOOL CompareSecondaryPassword(char *i_szSecPass);
	BOOL CheckSecondaryPasswordLock(void);

	//////////////////////////////////////////////////////////////////////////
	// 2007-09-14 by dhjin, WarPoint ��� 
	INT	 CalcWarPoint(INT i_nWarPoint, BOOL i_bEventBonus);				// 2007-09-14 by dhjin, WarPoint �̺�Ʈ ���� ���

	// 2007-11-06 by cmkwon, ���� �α� DB ���� ���� �����ϱ� - ���Ӽ��� �α���/�α׾ƿ� ó��, PreServer ���� �÷��� ���� �߰�
	bool GetPreServerAuthenticated() const { return m_bPreServerAuthenticated; }
	void SetPreServerAuthenticated(bool i_bFlag, bool i_bUpdateDB = false) { m_bPreServerAuthenticated = i_bFlag; }

	///////////////////////////////////////////////////////////////////////////////
	// 2007-11-13 by cmkwon, �����ϱ� ��� �߰� - 
	SGIVE_TARGET_CHARACTER		m_giveTargetCharacter;
	void OnCheckGiveTarget(SGIVE_TARGET_CHARACTER *i_pGiveTargetCharacter);			// 2007-11-13 by cmkwon, �����ϱ� ��� �߰� - CFieldIOCPSocket::OnCheckGiveTarget() �߰�
	Err_t GiveItem(ITEM *i_pItem, int i_nCnts, BYTE i_nItemUpdateType);	// 2007-11-13 by cmkwon, �����ϱ� ��� �߰� - 

	///////////////////////////////////////////////////////////////////////////////
	// 2007-11-28 by cmkwon, �����ý��� ���� -
	mtvectSNOTIFY_MSG			m_mtvectNotifyMsgList;					// 2007-11-28 by cmkwon, �����ý��� ���� -
	void OnLoadNotifyMsg(mtvectSNOTIFY_MSG *i_pmtvectNotifyMsgList);	// 2007-11-28 by cmkwon, �����ý��� ���� -
	BOOL CheckDeleteNotifyMsg(SNOTIFY_MSG *o_pNotifyMsg, UID64_t i_notifyMsgUID);				// 2007-11-28 by cmkwon, �����ý��� ���� -
	void InsertNotifyMsg(SNOTIFY_MSG *o_pNotifyMsg);					// 2007-12-20 by cmkwon, �����ý��� ���� ���� - �ڽ��� ��������Ʈ�� ���� �߰�
	static BOOL InsertNotifyMsg2Anyone(SNOTIFY_MSG *i_pNotifyMsg, CFieldIOCPSocket *i_pDoerFISoc);		// 2007-12-20 by cmkwon, �����ý��� ���� ���� - CFieldIOCPSocket::InsertNotifyMsg() �Լ��� ����, // 2007-11-28 by cmkwon, �����ý��� ���� -

	///////////////////////////////////////////////////////////////////////////////
	// 2008-01-10 by cmkwon, ������ �̺�Ʈ �ý��ۿ� �� ���� �ý��� �߰� - 
	mtvectQPARAM_CHECK_EVENTITEM	m_mtvectCouponEventList;
	BOOL FindCouponEvent(int i_nItemEventUID);
	BOOL PopCouponEvent(QPARAM_CHECK_EVENTITEM *o_pCouponEventInfo, int i_nItemEventUID);
	BOOL AddCouponEvent(QPARAM_CHECK_EVENTITEM *i_pCouponEventInfo);
	void SendCouponEvent();		// 2008-04-28 by dhjin, �Ʒ��� ���� - ���� ���� ���� 

	// 2008-01-03 by dhjin, �Ʒ��� ���� - MFS�� AFS �ɸ��� ���� ��Ī ����
	SAFS_NEED_MFSINFO	m_AfsNeedMfsInfo;
	BOOL SendMFSArenaStateChange(BYTE i_byArenaState);			// 2008-01-03 by dhjin, �Ʒ��� ���� - �ʵ弭���� �Ʒ��� ���°� ���� 
	void JobBeforeArenaPlay();			// 2008-01-04 by dhjin, �Ʒ��� ���� - �Ʒ��� ������ ���� �ʵ弭������ �ؾߵǴ� �� 
	BOOL SetDataArenaGameStart();		// 2008-01-09 by dhjin, �Ʒ��� ���� - �Ʒ��� ���� �� CFieldIOCPSocket�� ���� �ؾߵǴ� �� ó��
	BOOL SetDataReadyArenaGameStart();		// 2008-01-31 by dhjin, �Ʒ��� ���� - �Ʒ��� ���� �� CFieldIOCPSocket�� ���� �ؾߵǴ� �� ó��, Ŭ���̾�Ʈ ���� ������
	void SendArenaCharacterPay(BYTE i_byIssue, INT i_nPayWarPoint);		// 2008-01-10 by dhjin, �Ʒ��� ���� - �ʵ� ������ ���� ���� 
	void SendArenaCharacterDisConnect();						// 2008-01-15 by dhjin, �Ʒ��� ���� - �ʵ� ������ ���� ���� ����
	void OnCharacterReadyGameStartFromArenaToMainServer();				// 2008-01-31 by dhjin, �Ʒ��� ���� - �Ʒ��� ���� �� �ʵ� ���� ���� ó��, Ŭ���̾�Ʈ ���� �����ֱ�
	void OnCharacterGameStartFromArenaToMainServer();				// 2008-01-10 by dhjin, �Ʒ��� ���� - �Ʒ��� ���� �� �ʵ� ���� ���� ó��
	void ResetWearItem();										// 2008-01-29 by dhjin, �Ʒ��� ���� - ���� ������ ���� �ʱ�ȭ
	void ResetAllSkill();								// 2008-02-25 by dhjin, �Ʒ��� ���� - ��� ��ų �ʱ�ȭ
	void ResetTimeLimitSkill();							// 2008-02-25 by dhjin, �Ʒ��� ���� - �ð������� ��ų �ʱ�ȭ
	void ResetToggleSkill();							// 2008-02-25 by dhjin, �Ʒ��� ���� - ��� ��ų�� �ʱ�ȭ
	void ResetChargingSkill();							// 2008-02-25 by dhjin, �Ʒ��� ���� - ��¡ ��ų�� �ʱ�ȭ
	void ResetFieldDummySkill();						// 2008-02-25 by dhjin, �Ʒ��� ���� - ���� ��ų �ʱ�ȭ
	void InitUserInfoForArenaEnding();					// 2008-03-11 by dhjin, �Ʒ��� ���� - �Ʒ��� ����� �Ʒ��� ������ ���� ���� �ʱ�ȭ �ʿ��� �κ� ó��
	void SendUserInfoForArenaEnding();					// 2011-10-17 by hskim, ��Ʈ�� �ý��� 2�� - �Ʒ��� ����� �Ʒ��� �������� ���� ������ ������ ���� ó��
	//////////////////////////////////////////////////////////////////////////
	// 2008-04-03 by dhjin, ����, ������ ����â ��ȹ�� - 
	void SendMSWarInfoResult(vectSMSWAR_INFO_RESULT * i_pVectMSWarInfoResult);	// 2008-04-03 by dhjin, ����, ������ ����â ��ȹ�� - ���� ���� ����
	INT MakeMSG_FC_INFO_MSWARINFO_RESULT_OK(SMSWAR_INFO_RESULT *o_pMSWarInfoResult, vectSMSWAR_INFO_RESULT * i_pVectMSWarInfoResult);
	void SendSPWarInfoResult(vectSSPWAR_INFO_RESULT * i_pVectSPWarInfoResult);	// 2008-04-03 by dhjin, ����, ������ ����â ��ȹ�� - ������ ���� ����
#ifdef NEMERIAN_NATION_BALANCE_BUFF_PENALITY
	void SetDiffVictory(int DiffVictory);
#endif

	//////////////////////////////////////////////////////////////////////////
	// 2008-04-23 by dhjin, EP3 ���� �ý��� - 
	CLetterManager		m_LetterManager;
	void GetDBAllLetter();				// 2008-04-23 by dhjin, EP3 ���� �ý��� - DB���� ���� ��������
	
	void SendUserInfoBySecretOption(SUSER_INFO_OPTION * i_pUserInfoOption);		// 2008-06-23 by dhjin, EP3 ���������ɼ� - ���� ���� ����

	///////////////////////////////////////////////////////////////////////////////
	// 2008-06-20 by cmkwon, EP3 ��� �ý��� ����(�ʴ� ����� 2�� ���� �� �� �ִ�) - 
	mtmapMapIdx2MapIdx		m_mtmapBackMapIndex;		
	MapIndex_t GetBackMapIndex(CFieldMapProject *i_pCurFMPro);
	void UpdateBackMapIndex(MapIndex_t i_nCurMapIndex, MapIndex_t i_nBackMapIndex);

	//////////////////////////////////////////////////////////////////////////
	// 2008-08-18 by dhjin, 1�� �� �ý��� ��ȹ��
	DWORD	m_dwMSTermSystem;
	BOOL CheckOneSecondTermSystem();

	///////////////////////////////////////////////////////////////////////////////
	// 2008-10-21 by cmkwon, �������� ���õ� ����� �ٽ� ������ �ʰ� ���� - 
	// 2010-04-20 by cmkwon, �ű� ��Ű �ӽ� ���� - 
	RARE_ITEM_INFO		*m_pLastSelectedPrefixRareItemInfo;	// ����� ���������� ���õ� ���� ���� 
	RARE_ITEM_INFO		*m_pLastSelectedSuffixRareItemInfo;	// ����� ���������� ���õ� ���� ���� 

	//////////////////////////////////////////////////////////////////////////
	// 2008-11-10 by dhjin, ��Ű�ӽ�
	// 2010-04-20 by cmkwon, �ű� ��Ű �ӽ� ���� - ���� �ҽ� ���� - 
	Err_t LuckyItemWin(INT i_buildingIdx, ITEM * i_pItem, INT i_nMachineNum, BOOL * o_pbDeleteCoinItem);		// 2009-03-03 by dhjin, ��Ű�ӽ� ������ - i_nMachineNum�߰� 

	// 2009-05-27 by cmkwon, ��Ű�ӽ�/���Ǹӽ� �κ� �䱸 ���� �ٸ��� ó�� - �Լ� ������Ͽ� �߰�
	int GetNeededInventoryCount(ITEM * i_pItem, INT i_nMachineNum, UINT i_nBuildingIdx);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-11-28 by cmkwon, �븸 Netpower_Tpe XignCode(���Ӱ���) ���� - 
	static BOOL				ms_bUsingXignCode;
	static BOOL IsInitializedSecurityServer(void);
	
	//////////////////////////////////////////////////////////////////////////
	// 2008-12-08 by dhjin, �̼Ǹ�����
	BOOL CheckQuestComplete(INT i_nQuestIdx);
	BOOL m_bMissionMaster;
	void MissionMasterQuestResult(CQuest *pQuestInfo);		// 2008-12-09 by dhjin, �̼Ǹ����� - �̼Ǹ����� ���� ����

	///////////////////////////////////////////////////////////////////////////////
	// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - 
	mtvectCFieldIOCPSocket		m_mtvectCFISockPartyMemberList;		// 2008-12-30 by cmkwon, �ڽ��� ������ ��� ��Ƽ�� ����Ʈ
	static BOOL ArrangeAllPartyMember(vectCFieldIOCPSocket *i_pvectCFISockList);
	BOOL ArrangePartyMemberList(vectCFieldIOCPSocket *i_pvectCFISockList);
	BOOL GetPartyMemberList(vectCFieldIOCPSocket *o_pvectCFISockList);
	void CleanPartyMemberList(void);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-03-09 by cmkwon, �Ϻ� Arario nProtect�� CS���� �����ϱ� - CFieldIOCPSocket�� ��� ���� �߰�
	static BOOL				ms_bUsingNProtect;

	///////////////////////////////////////////////////////////////////////////////
	// 2009-08-03 by cmkwon, EP3-4 ��� ���� ��ų ���� - 
	int						m_nPartyFormationSkillNum;
	void SetPartyFormationSkillNum(int i_nSkillNum);
	int GetPartyFormationSkillNum(void);
	ITEM_SKILL * FindPartyFormationSkill(BYTE i_byFormationTy);
	void ChangePartyFormationSkill(vectCFieldIOCPSocket *i_pvectPartyMemList, BOOL i_bOnOff, BYTE i_byFormationTy);
	BOOL ApplyPartyFormationSkill(ITEM_SKILL *i_pSkill);
	BOOL ReleasePartyFormationSkill(int i_nTemp=0);
	void ApplyItemDesParam(ITEM *i_pItem);
	void ReleaseItemDesParam(ITEM *i_pItem);
	void DamageDistributionToPartyMember(float i_fChangeValue, BYTE i_enumDamageType, BOOL i_bCheckRecovery=TRUE
		, CFieldMonster *i_pAttackMonster=NULL, CFieldIOCPSocket *i_pAttackCharacter=NULL, MSG_FC_BATTLE_SHOW_DAMAGE *i_pShowDamage=NULL);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
	BOOL IsChangeItemNumMix(ITEM_GENERAL **o_ppMainItemG, ITEM_UNIQUE_NUMBER_W_COUNT *i_arrSourceItem, INT i_nSourceItemCnt, INT i_nMixCnts, ITEM *i_pTargetItemInfo);
	BOOL IsChangeShapeItemNumMix(ITEM_GENERAL **o_ppMainItemG, ITEM_GENERAL **o_ppSubItemG, ITEM_UNIQUE_NUMBER_W_COUNT *i_arrSourceItem, INT i_nSourceItemCnt, INT i_nMixCnts, ITEM *i_pTargetItemInfo);
	BOOL IsChangeEffectItemNumMix(ITEM_GENERAL **o_ppMainItemG, ITEM_GENERAL **o_ppSubItemG, ITEM_UNIQUE_NUMBER_W_COUNT *i_arrSourceItem, INT i_nSourceItemCnt, INT i_nMixCnts, ITEM *i_pTargetItemInfo);
	///////////////////////////////////////////////////////////////////////////////
	// 2009-10-01 by cmkwon, �׷��� ���ҽ� ���� ���� �ʱ�ȭ ��� ���� - �Ʒ��� 2�� �Լ� �߰�
	BOOL IsInitShapeItemNumMix(ITEM_GENERAL **o_ppMainItemG, ITEM_GENERAL **o_ppSubItemG, ITEM_UNIQUE_NUMBER_W_COUNT *i_arrSourceItem, INT i_nSourceItemCnt, INT i_nMixCnts, ITEM *i_pTargetItemInfo);	// 2009-10-01 by cmkwon, �׷��� ���ҽ� ���� ���� �ʱ�ȭ ��� ���� - 
	BOOL IsInitEffectItemNumMix(ITEM_GENERAL **o_ppMainItemG, ITEM_GENERAL **o_ppSubItemG, ITEM_UNIQUE_NUMBER_W_COUNT *i_arrSourceItem, INT i_nSourceItemCnt, INT i_nMixCnts, ITEM *i_pTargetItemInfo);	// 2009-10-01 by cmkwon, �׷��� ���ҽ� ���� ���� �ʱ�ȭ ��� ���� - 

	///////////////////////////////////////////////////////////////////////////////
	// 2010-02-11 by cmkwon, ���ս� ����,����Ʈ ���� �ý��� ���� - 
	BOOL IsKeepShape8EffectMix(ITEM_GENERAL **o_ppMainItemG, ITEM_UNIQUE_NUMBER_W_COUNT *i_arrSourceItem, INT i_nSourceItemCnt, INT i_nMixCnts, ITEM *i_pTargetItemInfo);

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 
	mt_vectorInfinityImpute		m_mtVecInfiImpute;						// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ -  ���Ǵ�Ƽ �ͼ� ����
	INFINITY_PLAYING_INFO		m_InfinityPlayingInfo;					// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ -  ���Ǵ�Ƽ ���϶� �ʿ��� ���� ����	
	void GetDBInfinityImpute();			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ͼ� ���� ��������
//	void SetInfinityImpute(InfiModeUID_t i_ninfiModeUID, ImputeUID_t i_nImputeUID);			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ͼ�
	BOOL SetDataReadyInfinityGameStart();		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� �� CFieldIOCPSocket�� ���� �ؾߵǴ� �� ó��, Ŭ���̾�Ʈ ���� ������
	BOOL SetDataInfinityGameStart();			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� �� CFieldIOCPSocket�� ���� �ؾߵǴ� �� ó��
	BOOL SetInfinityMap(MapIndex_t i_MapIdx, ChannelIndex_t i_ChannelIdx);						// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� �� �� ����
	void InfinityFin();					// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ����� Main������ ������ �κ� ����
	void ArenaFin();					// 2011-10-17 by hskim, ��Ʈ�� �ý��� 2�� - �Ʒ��� ����� Main������ ������ �κ� ����
	INT	m_InfinityUpdateAllItemCount;		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ����� Main�������� ������Ʈ �ؾߵ� ������ ��
	ClientIndex_t	m_AFSClientIndex;		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ArenaSvr ClientIdx ����
	BOOL InfinityResetImpute(InfiModeUID_t i_ninfiModeUID);		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ͼ� ���� ����
	void InfinityFinReloadAllItem();					// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� �� MainSvr�� ���� �����ϸ鼭 ������ ������ ������Ʈ ����� ��� ������ �ٽ� �ҷ�����!!
	Lv_t GetLvCheckRevision();							// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ��Ȳ���� üũ
	static float CalcDamageOfAttackMonsterToMonster(float i_fDamageWeight,
											CFieldIOCPSocket *i_pAttackSocket, void* i_pAttackUnit, SATTACK_PARAMETER *i_pAttParam,	void *i_pTargetUnit);// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���Ͱ� ����
	static void OnMonsterDeadByMonster(CFieldMonster *pTargetMonster, ITEM *pAttackItem);	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���Ϳ� ���� ���

	///////////////////////////////////////////////////////////////////////////////
	// 2009-12-11 by cmkwon, ������ ��׷η� Ÿ���� �����ϴ� ���� ���� - 
	ClientIndex_t				m_nLastMonsterIndexAttack2Me;			// ���� �����ϴ� ������ ������ MonsterIndex
	ClientIndex_t				m_nTargetMonsterIndex;					// ���� �����ϴ� MonsterIndex
	void SetLastMonsterIndexAttack2Me(ClientIndex_t i_MonIdx);			// 
	ClientIndex_t GetLastMonsterIndexAttack2Me(void);					// 
	void SetTargetMonsterIndex(ClientIndex_t i_MonIdx);					
	ClientIndex_t GetTargetMonsterIndex(void);					
	void GetAttackedMonsterIndex(setClientIndex_t *o_pMonsterIdxList);
 	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 
	BOOL MonsterAttackSecM2C(CFieldMonster * i_pAttackMonster, MSG_FN_BATTLE_ATTACK_SECONDARY * i_pAttackSecMsgInfo, ITEM * i_pAttackItem);	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� �Ϲ� ���� ó�� M->C
	BOOL MonsterAttackSecM2M(CFieldMonster * i_pAttackMonster, MSG_FN_BATTLE_ATTACK_SECONDARY * i_pAttackSecMsgInfo, ITEM * i_pAttackItem);	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� �Ϲ� ���� ó�� M->M

// 2010-03-16 by cmkwon, ����2�� MtoM, MtoC Ÿ�� ���� ���� ���� - CFieldIOCPSocket::ProcessSplashDamageMonsterAllAttack#�� ����
//	static void ProcessRangeDamageMonsterAllAttack(MSG_FN_BATTLE_ATTACK_PRIMARY *i_pAttackPri, CFieldIOCPSocket *i_pAttackMonSock, CFieldMonster *i_pAttackMon, ITEM *i_pAttackItem);	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� ����

	BOOL GetDelegateClientIdxByMonsterAttackSecM2M(CFieldMonster * i_pAttackMonster, ClientIndex_t * o_pDelegateClientIdx);			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� �� 2�� ���� �Ǵ� ���� Ŭ���̾�Ʈ ������ ã��

	// start 2011-04-18 by hskim, ���Ǵ�Ƽ 3�� - ���� ���� ��� ����
	BOOL GetDelegateClientIdx(CFieldMonster * i_pAttackMonster, ClientIndex_t * o_pDelegateClientIdx);
	// end 2011-04-18 by hskim, ���Ǵ�Ƽ 3�� - ���� ���� ��� ����

	BOOL IsApplyItemIgnoreDefence();			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� ������ ��� üũ	
	BOOL IsApplyItemIgnoreAvoid();				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ȸ�Ƿ� ���� ������ ��� üũ
	ParamValue_t GetParamResistance();			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ������ ��� üũ
	ParamValue_t GetParamOnceResistance();		// 2009-09-09 ~ 2010-01-19 by dhjin, ���Ǵ�Ƽ - �� ���� �����ϰ� �������� ���� ������ �߰�
	ParamValue_t GetParamAddAttack();			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���밪 Ÿ��ġ ������ ��� üũ
	ParamValue_t GetParamAddAttackSec();		// 2009-09-09 ~ 2010-01-15 by dhjin, ���Ǵ�Ƽ - ���밪 Ÿ��ġ ������ ��޹����(�⹫�� �з�)
	ParamValue_t GetParamReduceDamage();		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ������ ���밪 ���� ������ ��� üũ
	// 2009-09-09 ~ 2010-01-15 by dhjin, ���Ǵ�Ƽ - ���밪 Ÿ��ġ ������ ��޹����(�⹫�� �з�), �ذ� ���� ����
//	static void AddAttackDamage(CFieldIOCPSocket *i_pAttackSocket, void* i_pAttackUnit, void *i_pTargetUnit, SATTACK_PARAMETER *i_pAttParam);			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���밪 �߰� Ÿ��ġ ������ ����
	static void AddAttackDamage(CFieldIOCPSocket *i_pAttackSocket, void* i_pAttackUnit, void *i_pTargetUnit, ParamValue_t i_AddAttackDamage, SATTACK_PARAMETER *i_pAttParam);			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���밪 �߰� Ÿ��ġ ������ ����
	BOOL GetInfinityShopInfo(InfinityShopUID_t i_InfinityShopUID, INFINITY_SHOP_INFO * o_pInfinityShopItem);	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ����
	Err_t CheckBuyShopItemInsertNoLock(ItemNum_t i_ItemNum, UID64_t i_Amount);	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ����, �������� ������ ���� �� ���� �� ������ üũ ����
	Err_t CheckBuyShopItemDeleteNoLock(ItemNum_t i_ItemNum, UID64_t i_Amount);	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ����, �������� ������ ���� �� ���� �� ������ üũ ����
	Err_t CheckInfinityShopItem(INFINITY_SHOP_INFO * i_pInfinityShopItem, INT i_Amount);	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ����, ������ üũ ����
	ItemNum_t GetCurrentAttackItemNum() const { return m_CurrentAttackItemNum; }		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�
	ItemNum_t m_CurrentAttackItemNum;					// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�
	void InfinityFinCharacterRoutine();			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��, ���Ǵ�Ƽ���� ���� ���� �� ó�� �Ǿ�� �Ǵ� �͵�
	///////////////////////////////////////////////////////////////////////////////
	// 2010-01-18 by cmkwon, ������ ���� Parameter �ߺ� üũ �ý��� ���� - 
	ITEM *CheckItemParamOverlap(ITEM *i_pToUseItemInfo, INT i_nExcludeItemNum=0);
	ITEM *CheckItemParamOverlapOfAttachedItems(ITEM *i_pToUseItemInfo, INT i_nExcludeItemNum);

	BOOL SendMFSInfinityStateChange(eINFINITY_STATE i_byInfintiyState);	// 2009-09-09 ~ 2010-01-20 by dhjin, ���Ǵ�Ƽ - ���� ���� ���� ���μ����� �����Ѵ�.
	Err_t SendInfinityModeInfo();		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ, ��� ���� ���� ����(���� ���� ���� üũ�Ͽ� ����)

	///////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
	Err_t ApplyInvokingItem(ITEM_GENERAL * i_pItemG);	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ������������� ����
	Err_t EnchantApplyInvokingItem(ITEM_GENERAL * i_pTargetItemG, ITEM * i_pEnchantItem);	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ������������� ��þƮ ����
	void SendApplyDestParamInfo(ClientIndex_t i_ApplyClientIdx, INVOKING_ITEM_DESTPARAM_TYPE * i_pApplyDestParamInfo);	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������, �ߵ� ���� �����Ķ� ���� ���� ����
	void SendApplyDestParamListInfo(CFieldIOCPSocket * i_pApplyFISoc, InvokingItemDestParamTypeList * i_pInvokingItemDestParamTypeList); // 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������, �ߵ� ���� �����Ķ� ���� ���� ����
	Err_t ApplyInvokingItemByUse(ITEM_GENERAL *i_pItemG);	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ������������� ���
	void GameEndAllInvokingEquipItem();				// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������, ���� ����� ó��.
	void ApplyInvokingItemDrain(float i_fRetDamage, CFieldIOCPSocket *i_pAttackFISoc, CFieldIOCPSocket *i_pTargetFISoc=NULL, CFieldMonster * i_pTargetMonster=NULL);					// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������, �巹�� ó��
	void ApplyInvokingItemReflection(float i_fRetDamage, CFieldIOCPSocket *i_pTargetFISoc, CFieldIOCPSocket *i_pAttackFISoc=NULL, CFieldMonster * i_pAttackMonster=NULL);				// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������, ������ �ݻ� ó��
	void IncreaseCurrentHPDP(float i_fChangeVal);	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������, HP -> DP ȸ��
	BOOL CheckDestParamEnchantItemNoLock(ITEM_GENERAL * i_pEnchantItemG, ITEM_GENERAL * i_pTargetItemG);	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������, ��þƮ�� �����Ķ��� ��� �����ϴ��� üũ

	CInvokingItemManager			m_InvokingItemManager;		// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������

	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-23 by cmkwon, ���Ǵ�Ƽ ���� ĳ�� ������ ���� - 
	EntranceCount_t InfinityGetEntranceCount(InfiModeUID_t i_infiModeUID);
	void Check8SendAFSUpdateItem(ITEM_GENERAL * i_pItemG);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -
	void UserApplyByMonsterSkill(MSG_FN_BATTLE_ATTACK_SKILL * i_pRMsg, CFieldMonster * i_pFMonster, ITEM * i_pSkillItemDefault);				// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - ���Ͱ� ����� ��ų ó�� (������)
	void ProcessingHitInvokeSkill(ITEM * i_pAttackItem, ClientIndex_t i_AttackIdx);			// ���߽� �ߵ��Ǵ� ��ų ó��
	void ApplyHitInvokeSkill(ItemNum_t i_SkillNum, ClientIndex_t i_AttackIdx);					// ���߽� �ߵ��Ǵ� ��ų ����

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-05 by cmkwon, ����2�� M2M 2�� ���� ���� ó�� - 
	void ProcessM2MOverLifeTimeAttackParameter(CFieldMonster *i_pAttFMon, SATTACK_PARAMETER *i_pAttParam, CFieldMapChannel *i_pCurFMapChann);
	
	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-05 by cmkwon, ���� ������ ī�� ���� �ý��� ���� - 
	void InfinityInitImputeList(vectINFINITY_IMPUTE *i_pImputeList);
	void InfinityAddImpute(InfiModeUID_t i_infiModeUID, BOOL i_bUpdate2DB=FALSE);
	INFINITY_IMPUTE *InfinityGetImputeInfoNoLock(InfiModeUID_t i_infiModeUID);
	BOOL InfinitySendImputeList2AFS(void);
	BOOL InfinitySendAddDataFromAFS2MFS(BYTE * pData, int nSize);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-06 by cmkwon, ����2�� �߰� ���� - 
	void InfinityOnCharacterGameEndRoutine(void);
	void ArenaOnCharacterGameEndRoutine(void);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-20 by cmkwon, �ű� ��Ű �ӽ� ���� - 
	Err_t TryEnchantItem(BOOL *o_pbSuccessFlag, ITEM_GENERAL *i_pTargetItemG, int i_nEnchantCnt, ITEM_GENERAL *i_pEnchatItemG, ITEM_GENERAL *i_pPreventDestructionItemG, ITEM_GENERAL *i_pProbabilityIncreaseItemG);
	Err_t TryGambleItem(BOOL *o_pbSuccessFlag, ITEM_GENERAL *i_pTargetItemG, int i_nEnchantCnt, ITEM_GENERAL *i_pGambleCard1ItemG, ITEM_GENERAL *i_pGambleCard2ItemG);

	// 2013-07-01 by bckim, 10��æƮ �̸� ������ī��ó��, �α��߰�, ��� ����
	BOOL GetAllEnchantListByItemUID(vectENCHANT *o_pvectEnchat, UID64_t i_itemUID);
	BOOL IsHyperCard(ENCHANT_INFO *i_pEnchantInfo);			
	BOOL ExsistHyperCardIn10Lv(UID64_t i_nItemUID);			
	// End. 2013-07-01 by bckim, 10��æƮ �̸� ������ī��ó��, �α��߰�, ��� ����

	///////////////////////////////////////////////////////////////////////////////
	// 2010-05-12 by cmkwon, �ŷ� �̿��� ������ �޸� ���� ����(K0002953) - �ŷ����� ������ ���� ����
	BOOL CheckTradingItem(UID64_t i_itemUID);
	///////////////////////////////////////////////////////////////////////////////
	// 2010-06-01 by shcho, PC�� ���� ȹ��(ĳ��) ������ - 
	void SetHomepremium(BOOL i_bUse);	
	///////////////////////////////////////////////////////////////////////////////
	// 2010-08-27 by shcho&&jskim, WARPOINT ���� ������ ����
	BOOL SetWarPoint(INT i_nAddWarPoint);

	////////////////////////////////////////////
	// 2011-09-20 by hskim, ��Ʈ�� �ý��� 2��
	Err_t PetCancelSocketItem(ITEM *i_pItemInfo);

	///////////////////////////////////////////////////////////////////////////////
	// 2012-02-13 by hskim, ���� ������ ��� �׽�Ʈ ��ɾ�
	BOOL TestMonsterDropItems(int MonsterIndex, int TestCount);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-06-01 by shcho, GLogDB ���� -
	Experience_t		m_GameStartEXP;						// 2010-06-01 by shcho, GLogDB ���� -
	ATUM_DATE_TIME		m_StartStayedMapTime;				// 2010-06-01 by shcho, GLogDB ���� -
	vectStayedMapInfo	m_vectStayedMapInfo;				// 2010-06-01 by shcho, GLogDB ���� -
	
	MOSTSTAYED_MAP_INFO* FindStayedMapInfo(MapIndex_t i_MapIndex);
	const char * GetStayedMapName();			

	// 2010-08-05 by dhjin, ���׸� - 
	Err_t	CheckPossibleBurningMapWarp(MapIndex_t i_nMapIdx);		// ���׸� ������ �������� ���� üũ
	BOOL	IsGetOutBurningMap(MapIndex_t i_nMapIdx);				// ���׸ʿ��� ��������~!!

	//eCONNECT_PUBLISHER	m_eOtherPublisherConncect;				// 2010-11 by dhjin, �ƶ󸮿� ä�θ� �α���.

	// start 2012-01-16 by hskim, ��� - ȭ��
	SSTATISTICS_MONEY_UNIT m_StatisticsInfinityMoney;
	SSTATISTICS_MONEY_UNIT GetCurrentMoney();

	void StatisticsInfinityStart();								// ���Ǵ�Ƽ ���� ��� ���۾� - ���� ORG �� SPI ����
	SSTATISTICS_MONEY_UNIT StatisticsInfinityEnd();				// ���Ǵ�Ƽ ������   ��� ���۾� - �����ߴ� ORG �� SPI ���� ������ ����
	// end 2012-01-16 by hskim, ��� - ȭ��

	void ArenaPlayInfoUpdataToMF();	// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : �Ʒ��� �÷��� ���� ������Ʈ ������ ���� �ʵ弭������ �����ϱ�

	// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
	// 2012-10-19 by jhjang ���Ǿƿ� ����ġ ������ - ųī��Ʈ 300�϶� �ٷ� �����ϰ� ����

#if S_BONUS_KILL_SYSTEM_RENEWAL
	bool RefreshKillCount();
#else
	bool RefreshKillCount(CFieldMonster* pFieldMonster);
#endif // S_BONUS_KILL_SYSTEM_RENEWAL

	// end 2012-10-19 by jhjang ���Ǿƿ� ����ġ ������ - ųī��Ʈ 300�϶� �ٷ� �����ϰ� ����
	void SetKillCountEventItem(bool bIsPayKillCountBonusItem);
	// end 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������

	// 2013-03-13 by hskim, �� ĳ�� ����
	//void ResetCashAuthenticationKey()					{ memset(m_szAuthenticationKey, 0, sizeof(m_szAuthenticationKey)); }
	//void GenCashAuthenticationKey();
	//void SetCashAuthenticationKey(char *pAuthKey)		{ util::strncpy(m_szAuthenticationKey, pAuthKey, sizeof(m_szAuthenticationKey)); }
	//BOOL IsCashAuthenticationKey(char *pAuthKey)		{ return 0 == strncmp(m_szAuthenticationKey,pAuthKey, sizeof(m_szAuthenticationKey)); }
	//char *GetCashAuthenticationKey()					{ return m_szAuthenticationKey; }
	// end 2013-03-13 by hskim, �� ĳ�� ����

	// 2015-09-23 Future, added logging of MAC Address
	const char* GetMACAddress() { return m_szMACAddress; }

	// 2015-10-04 Future, added decking detection
#ifdef S_ANTI_DECK
	void SetLastHit(UID32_t attackerUID);
	void ResetLastHit();
#endif // S_ANTI_DECK

	// 2015-11-24 Future, Multiple IP Restriction
#ifdef S_IP_UNIQUE_CONNECTION
	void ResGetMultipleIpStatus(BOOL status, UID32_t kickIssuingCharacterID = 0);
#endif // S_IP_UNIQUE_CONNECTION

	// 2015-12-17 Future, Nation Change Card
	void ResSwitchNation(BOOL res, UID64_t itemNum);

	// 2016-01-03 Future, leader warp restriction
#ifdef SC_LEADER_WARP_RESTRICTION
	BOOL RestrictWarp(UINT forMinutes, BOOL townUser = TRUE);
	BOOL ReleaseWarpRestriction();
	BOOL IsWarpRestricted();
	void SendWarpRestrictionError();
#endif // SC_LEADER_WARP_RESTRICTION

	void PreventCityFlying();

public:
	void Send2MeCharacterOtherInfo(vectorClientIndex *i_pVectCltIndexList);
	void SendCharacterOtherInfo(int idx, BOOL i_bSendInRange = FALSE);
	void SendCharacterInfo(MessageType_t msgType);
	Err_t SendInfinityDifficulty_Info(void);

	// �ڽ��� ������ �޽��� ����
	// 2009-10-12 by cmkwon, ������ī ���� ��� ���� - CFieldIOCPSocket::SendInRangeMessageAroundMe#, �����߰�(i_nStartCityMapIdx)
	void SendInRangeMessageAroundMe(BYTE *pData, int Size, BOOL bIncludeMe, BYTE i_byInfluenceMask=INFLUENCE_TYPE_ALL_MASK, MapIndex_t i_nStartCityMapIdx=VCN_CITY_MAP_INDEX);
	void SendInRangeMessageExactAroundMe(BYTE *pData, int Size, int i_radius, BOOL bIncludeMe, BYTE i_byInfluenceMask=INFLUENCE_TYPE_ALL_MASK);

	template<MessageType_t msgtype>
	void SendInRangeMessageAroundMe(MessageData<msgtype>& msg, bool bIncludeMe, BYTE i_byInfluenceMask=INFLUENCE_TYPE_ALL_MASK, MapIndex_t i_nStartCityMapIdx=VCN_CITY_MAP_INDEX)
	{
		this->SendInRangeMessageExactAroundMe(reinterpret_cast<BYTE*>(&msg), msg.size(), bIncludeMe, i_byInfluenceMask, i_nStartCityMapIdx);
	}

	// �ٸ� �̿��� �ڽ��� ������ ����
	void SendParamFactorToOthers(BYTE i_nItemPosition, ClientIndex_t i_ToClientIndex, BOOL i_bSendInRange);

	void SendString128(BYTE string128_type, const char* pFormat, ...);
	void SendString128Static(BYTE string128_type, char* pFormat, ...);
	//Send Notification to User
	void SendNotification(BOOL bSpecialChat/*Write it into Leadechat or not?*/, char* pFormat, ...);
private:
	/*
	ProcessResult Process_FM_CONNECT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FM_GET_NUM_CLIENTS(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FM_SHUTDOWN(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FM_PING(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FM_PRINTMAP(const char* pPacket, int nLength, int &nBytesUsed);
	*/
	ProcessResult Process_FC_CONNECT_LOGIN(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CONNECT_NETWORK_CHECK(const char* pPacket, int nLength, int &nBytesUsed);		// 2008-02-15 by cmkwon, Client<->FieldServer �� ��Ʈ��ũ ���� üũ - 
	ProcessResult Process_FC_CONNECT_ARENASERVER_LOGIN(const char* pPacket, int nLength, int &nBytesUsed);		// 2007-12-28 by dhjin,  �Ʒ��� ���� -
	ProcessResult Process_FC_CONNECT_ARENASERVER_TO_IMSERVER(const char* pPacket, int nLength, int &nBytesUsed);		// 2008-03-03 by dhjin,  �Ʒ��� ���� -
	ProcessResult Process_FC_CHARACTER_GET_CHARACTER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_CREATE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_DELETE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_GET_OTHER_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_GET_OTHER_MOVE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_GET_OTHER_RENDER_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_GET_ACCOUNTUNIQUENUMBER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_APPLY_COLLISION_DAMAGE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_SHOW_EFFECT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_GET_OTHER_PARAMFACTOR(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_SEND_PARAMFACTOR_IN_RANGE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_SPEED_HACK_USER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_GET_CASH_MONEY_COUNT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_TUTORIAL_SKIP(const char* pPacket, int nLength, int &nBytesUsed);
	// 2005-07-26 by hblee : �����忡�� ĳ���� ��� ��ȯ.
	ProcessResult Process_FC_CHARACTER_CHANGE_CHARACTER_MODE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_FALLING_BY_FUEL_ALLIN(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_WARP_BY_AGEAR_LANDING_FUEL_ALLIN(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_GET_REAL_WEAPON_INFO_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_GET_REAL_ENGINE_INFO_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_GET_REAL_TOTAL_WEIGHT_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_MEMORY_HACK_USER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_UPDATE_SUBLEADER(const char* pPacket, int nLength, int &nBytesUsed);				// 2007-02-14 by dhjin
	// 2007-03-27 by dhjin, ������ ���
	ProcessResult Process_FC_CHARACTER_OBSERVER_START(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_OBSERVER_END(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_SHOW_MAP_EFFECT(const char* pPacket, int nLength, int &nBytesUsed);		// 2007-04-20 by cmkwon

	// 2008-01-10 by dhjin,  �Ʒ��� ���� - 
	ProcessResult Process_FC_CHARACTER_GAMESTART_FROM_ARENA_TO_MAINSERVER(const char* pPacket, int nLength, int &nBytesUsed);	
	ProcessResult Process_FC_CHARACTER_READY_GAMESTART_FROM_ARENA_TO_MAINSERVER(const char* pPacket, int nLength, int &nBytesUsed);	

	// 2008-06-23 by dhjin, EP3 ���������ɼ� -
	ProcessResult Process_FC_CHARACTER_GET_USER_INFO(const char* pPacket, int nLength, int &nBytesUsed);	
	ProcessResult Process_FC_CHARACTER_CHANGE_INFO_OPTION_SECRET(const char* pPacket, int nLength, int &nBytesUsed);	
	ProcessResult Process_FC_CHARACTER_CHANGE_NICKNAME(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 

	ProcessResult Process_FC_CHARACTER_CHANGE_START_CITY_MAPINDEX(const char* pPacket, int nLength, int &nBytesUsed);	// 2009-10-12 by cmkwon, ������ī ���� ��� ���� - 

	ProcessResult Process_FC_CHARACTER_DEBUFF_DOT_INFO(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - F -> C
	
	ProcessResult Process_FC_CHARACTER_GET_USER_ITEM_INFO(const char* pPacket, int nLength, int &nBytesUsed);	// 2012-06-05 by jhseol, ���۹̸����� - ���캼 �ɸ��� UID
	ProcessResult Process_FC_CHARACTER_GET_USER_ITEM_INFO_SECRET(const char* pPacket, int nLength, int &nBytesUsed);	// 2012-07-12 by jhseol, ���۹̸����� On / Off ��� - ���� ����
	
// 2004-12-16 by cmkwon, �ٸ� �ʵ弭������ ������ �����Ƿ� ������
//	ProcessResult Process_FC_EVENT_WARP_CONNECT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_EVENT_WARP_SAME_MAP_DONE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_EVENT_WARP_SAME_FIELD_SERVER_DONE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_EVENT_ENTER_BUILDING_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_EVENT_LEAVE_BUILDING(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_EVENT_REQUEST_WARP(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_EVENT_REQUEST_OBJECT_EVENT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_EVENT_SELECT_CHANNEL(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_EVENT_SELECT_CHANNEL_WITH_PARTY(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_EVENT_REQUEST_RACING_WARP(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_EVENT_GET_SHOP_WARP_TARGET_MAP_LIST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_EVENT_REQUEST_SHOP_WARP(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_EVENT_CHARACTERMODE_ENTER_BUILDING(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_EVENT_CALL_WARP_EVENT_REQUEST_ACK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_EVENT_CLICK_TELEPORT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_EVENT_NOTIFY_MSG_GET(const char* pPacket, int nLength, int &nBytesUsed);		// 2007-11-28 by cmkwon, �����ý��� ���� -
	ProcessResult Process_FC_EVENT_NOTIFY_MSG_DELETE(const char* pPacket, int nLength, int &nBytesUsed);	// 2007-11-28 by cmkwon, �����ý��� ���� -
	ProcessResult Process_FC_EVENT_COUPON_EVENT_USE_COUPON(const char* pPacket, int nLength, int &nBytesUsed);	// 2008-01-10 by cmkwon, ������ �̺�Ʈ �ý��ۿ� �� ���� �ý��� �߰� - 

	ProcessResult Process_FC_CHARACTER_GAMESTART(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_CONNECT_GAMESTART(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_DEAD_GAMESTART(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_GAMEEND(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_REPAIR(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_DOCKING(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_UNDOCKING(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_CHANGE_BODYCONDITION(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_CHANGE_BODYCONDITION_ALL(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_USE_BONUSSTAT(const char* pPacket, int nLength, int &nBytesUsed);

	ProcessResult Process_FC_MOVE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_MOVE_LOCKON(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_MOVE_UNLOCKON(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_MOVE_LANDING(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_MOVE_LANDING_DONE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_MOVE_TAKEOFF(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_MOVE_TARGET(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_MOVE_WEAPON_VEL(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_MOVE_ROLLING(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_MOVE_HACKSHIELD_GuidAckMsg(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_MOVE_HACKSHIELD_CRCAckMsg(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_MOVE_HACKSHIELD_HACKING_CLIENT(const char* pPacket, int nLength, int &nBytesUsed);
	
	ProcessResult Process_FC_BATTLE_ATTACK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BATTLE_ATTACK_FIND(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BATTLE_DROP_MINE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BATTLE_MINE_ATTACK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BATTLE_MINE_ATTACK_FIND(const char* pPacket, int nLength, int &nBytesUsed);

	ProcessResult Process_FC_BATTLE_CHANGE_TARGET(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BATTLE_DROP_DUMMY(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BATTLE_DROP_FIXER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BATTLE_TOGGLE_SHIELD(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BATTLE_TOGGLE_DECOY(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BATTLE_ATTACK_EVASION(const char* pPacket, int nLength, int &nBytesUsed);
//	ProcessResult Process_FC_BATTLE_DROP_BUNDLE(const char* pPacket, int nLength, int &nBytesUsed);
//	ProcessResult Process_FC_BATTLE_BUNDLE_ATTACK(const char* pPacket, int nLength, int &nBytesUsed);
//	ProcessResult Process_FC_BATTLE_BUNDLE_ATTACK_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
// 2004-12-27 by cmkwon, ������� ����
//	ProcessResult Process_FC_BATTLE_REQUEST_PK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BATTLE_REQUEST_P2P_PK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BATTLE_ACCEPT_REQUEST_P2P_PK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BATTLE_REJECT_REQUEST_P2P_PK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BATTLE_SURRENDER_P2P_PK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BATTLE_ACCEPT_SURRENDER_P2P_PK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BATTLE_REJECT_SURRENDER_P2P_PK(const char* pPacket, int nLength, int &nBytesUsed);

	ProcessResult Process_FC_STORE_GET_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_STORE_MOVE_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_STORE_EXPIRE_USINGITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_STORE_REQUEST_QUICKSLOT(const char* pPacket, int nLength, int &nBytesUsed);	// 2006-09-04 by dhjin
	ProcessResult Process_FC_STORE_SAVE_QUICKSLOT(const char* pPacket, int nLength, int &nBytesUsed);		// 2006-09-04 by dhjin
	ProcessResult Process_FC_STORE_LOG_GUILD_ITEM(const char* pPacket, int nLength, int &nBytesUsed);			// 2006-09-27 by dhjin, ���� �α� ��û ����

	ProcessResult Process_FC_ITEM_GET_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_DELETE_ITEM_ADMIN(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_DELETE_DROP_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_CHANGE_WINDOW_POSITION(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_UPDATE_WINDOW_ITEM_LIST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_THROW_AWAY_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_USE_ENERGY(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_USE_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_USE_ENCHANT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_MIX_ITEMS(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_USE_CARDITEM_GUILDSUMMON(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_USE_CARDITEM_RESTORE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_USE_CARDITEM_GUILD(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_USE_CARDITEM_MONSTERSUMMON(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_USE_CARDITEM_CHANGECHARACTERNAME(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_USE_CARDITEM_SKILLINITIALIZE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_USE_CARDITEM_CHANGEPILOTFACE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_USE_INFLUENCE_BUFF(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_USE_INFLUENCE_GAMEEVENT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_USE_RANDOMBOX(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_USE_SKILL_SUPPORT_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_USE_LUCKY_ITEM(const char* pPacket, int nLength, int &nBytesUsed);		// 2008-11-04 by dhjin, ��Ű�ӽ�
// 2010-04-20 by cmkwon, �ű� ��Ű �ӽ� ���� - ���� �ҽ� ���� - 
//	ProcessResult Process_FC_ITEM_USE_LUCKY_ITEM_WIN(const char* pPacket, int nLength, int &nBytesUsed);	// 2008-11-04 by dhjin, ��Ű�ӽ�
	// 2010-08-31 by shcho&jskim �����ۿ��� �ý��� - ���� �ý��� ��Ŷ ó��
	ProcessResult Process_FC_ITEM_USE_PET_SOCKET_ITEM(const char* pPacket, int nLength, int &nBytesUsed);		// 2011-09-20 by hskim, ��Ʈ�� �ý��� 2��
	ProcessResult Process_FC_ITEM_CANCEL_PET_SOCKET_ITEM(const char* pPacket, int nLength, int &nBytesUsed);	// 2011-09-20 by hskim, ��Ʈ�� �ý��� 2��
	ProcessResult Process_FC_DISSOLVED_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	// END 2010-08-31 by shcho&jskim �����ۿ��� �ý��� - ���� �ý��� ��Ŷ ó��

	// 2015-12-17 Future, Nation Change Card
	ProcessResult Process_FC_ITEM_USE_CARDITEM_CHANGE_NATION_REQUEST(const char* pPacket, int nLength, int& nBytesUsed);

	ProcessResult Process_FC_INFO_GET_MONSTER_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_GET_MAPOBJECT_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_GET_ITEM_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_GET_SIMPLE_ITEM_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_GET_RARE_ITEM_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_GET_BUILDINGNPC_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_GET_ENCHANT_COST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_GET_CURRENT_MAP_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_GET_SERVER_DATE_TIME(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO(const char* pPacket, int nLength, int &nBytesUsed);	// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - C->F
	ProcessResult Process_FC_INFO_CHECK_RESOBJ_CHECKSUM(const char* pPacket, int nLength, int &nBytesUsed);

	ProcessResult Process_FC_REQUEST_REQUEST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_REQUEST_ACCEPT_REQUEST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_REQUEST_REJECT_REQUEST(const char* pPacket, int nLength, int &nBytesUsed);

	ProcessResult Process_FC_CITY_GET_BUILDING_LIST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CITY_REQUEST_ENTER_BUILDING(const char* pPacket, int nLength, int &nBytesUsed);
// 2004-11-10 by cmkwon, Process_FC_EVENT_REQUEST_SHOP_WARP, Process_FC_EVENT_GET_SHOP_WARP_TARGET_MAP_LIST�� ��ü��
//	ProcessResult Process_FC_CITY_GET_WARP_TARGET_MAP_LIST(const char* pPacket, int nLength, int &nBytesUsed);
//	ProcessResult Process_FC_CITY_REQUEST_WARP(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CITY_CHECK_WARP_STATE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CITY_POLL_REG_LEADER_CANDIDATE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CITY_POLL_DELETE_LEADER_CANDIDATE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CITY_POLL_VOTE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CITY_POLL_REQUEST_POLL_DATE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CITY_WARINFO_INFLUENCE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CITY_WARINFO_OUTPOST(const char* pPacket, int nLength, int &nBytesUsed);

	ProcessResult Process_FC_QUEST_REQUEST_START(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_QUEST_ACCEPT_QUEST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_QUEST_CANCEL_QUEST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_QUEST_REQUEST_SUCCESS(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_QUEST_DISCARD_QUEST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_QUEST_MOVE_QUEST_MAP(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_QUEST_REQUEST_SUCCESS_CHECK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_QUEST_REQUEST_PARTY_WARP_ACK(const char* pPacket, int nLength, int &nBytesUsed);		// 2006-10-16 by cmkwon

	ProcessResult Process_FC_SHOP_GET_ITEMINFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SHOP_BUY_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SHOP_SELL_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SHOP_GET_USED_ITEM_PRICE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SHOP_GET_SHOP_ITEM_LIST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SHOP_REQUEST_REPAIR(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SHOP_BUY_CASH_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SHOP_BUY_COLOR_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SHOP_BUY_WARPOINT_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SHOP_CHECK_GIVE_TARGET(const char* pPacket, int nLength, int &nBytesUsed);	// 2007-11-13 by cmkwon, �����ϱ� ��� �߰� -
	ProcessResult Process_FC_SHOP_INFLUENCE_BUFF(const char* pPacket, int nLength, int &nBytesUsed);		// 2013-05-09 by hskim, ���� ����Ʈ ����

	// Party
	ProcessResult Process_FC_PARTY_REQUEST_INVITE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_PARTY_ACCEPT_INVITE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_PARTY_REJECT_INVITE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_PARTY_GET_MEMBER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_PARTY_GET_ALL_MEMBER(const char* pPacket, int nLength, int &nBytesUsed);	// check: �ʿ��� �� �����ϱ�, 20031005
	ProcessResult Process_FC_PARTY_REQUEST_PARTY_WARP(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_PARTY_REQUEST_PARTY_WARP_WITH_MAP_NAME(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_PARTY_REQUEST_PARTY_OBJECT_EVENT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_PARTY_GET_OTHER_MOVE(const char* pPacket, int nLength, int &nBytesUsed);

	// Guild
	ProcessResult Process_FC_GUILD_GET_MAP_OWNER_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_GUILD_REQUEST_GUILD_WAR(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_GUILD_GET_WAR_INFO(const char* pPacket, int nLength, int &nBytesUsed);
// 2004-12-10 by cmkwon, IM Server���� ó���Ѵ�, �������� ������
//	ProcessResult Process_FC_GUILD_SURRENDER_GUILD_WAR(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_GUILD_SUMMON_MEMBER_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_GUILD_DISMEMBER(const char* pPacket, int nLength, int &nBytesUsed);

	// Trade
	ProcessResult Process_FC_TRADE_REQUEST_TRADE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_TRADE_CANCEL_REQUEST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_TRADE_ACCEPT_TRADE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_TRADE_REJECT_TRADE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_TRADE_TRANS_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_TRADE_SEE_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_TRADE_OK_TRADE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_TRADE_CANCEL_TRADE(const char* pPacket, int nLength, int &nBytesUsed);

	// Countdown
	ProcessResult Process_FC_COUNTDOWN_DONE(const char* pPacket, int nLength, int &nBytesUsed);

	// Object
	ProcessResult Process_FC_OBJECT_CHANGE_BODYCONDITION(const char* pPacket, int nLength, int &nBytesUsed);

	// Auction
	ProcessResult Process_FC_AUCTION_REGISTER_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_AUCTION_CANCEL_REGISTER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_AUCTION_BUY_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_AUCTION_GET_ITEM_LIST(const char* pPacket, int nLength, int &nBytesUsed);

	// Skill
	ProcessResult Process_FC_SKILL_USE_SKILLPOINT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SKILL_SETUP_SKILL(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SKILL_USE_SKILL(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SKILL_CANCEL_SKILL(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SKILL_PREPARE_USE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SKILL_CANCEL_PREPARE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SKILL_CONFIRM_USE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SKILL_CONFIRM_USE_ACK(const char* pPacket, int nLength, int &nBytesUsed);

	// Timer
	ProcessResult Process_FC_TIMER_TIMEOUT(const char* pPacket, int nLength, int &nBytesUsed);

	// MONSTER
	ProcessResult Process_FC_MONSTER_SKILL_END_SKILL(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_MONSTER_SUMMON_MONSTER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_MONSTER_TUTORIAL_MONSTER_DELETE(const char* pPacket, int nLength, int &nBytesUsed);
	
	// T0_FC_RACING
	ProcessResult Process_FC_RACING_RACINGLIST_REQUEST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_RACING_RACINGINFO_REQUEST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_RACING_JOIN_REQUEST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_RACING_CHECKPOINT_CHECK(const char* pPacket, int nLength, int &nBytesUsed);

	// T0_FC_CITYWAR
	ProcessResult Process_FC_CITYWAR_GET_OCCUPYINFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CITYWAR_SET_SETTINGTIME(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CITYWAR_SET_TEX(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CITYWAR_SET_BRIEFING(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CITYWAR_BRING_SUMOFTEX(const char* pPacket, int nLength, int &nBytesUsed);

	ProcessResult Process_FC_WAR_REQ_SIGN_BOARD_STRING_LIST(const char* pPacket, int nLength, int &nBytesUsed);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-07-26 by cmkwon, ���� ����
	ProcessResult Process_FC_BAZAAR_SELL_PUT_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BAZAAR_SELL_CANCEL_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BAZAAR_SELL_START(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BAZAAR_SELL_REQUEST_ITEMLIST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BAZAAR_SELL_BUY_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BAZAAR_BUY_PUT_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BAZAAR_BUY_CANCEL_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BAZAAR_BUY_START(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BAZAAR_BUY_REQUEST_ITEMLIST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BAZAAR_BUY_SELL_ITEM(const char* pPacket, int nLength, int &nBytesUsed);

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-17 by dhjin, ARENA ����
	ProcessResult Process_FC_ARENA_PLAY_CHARACTER_TYPE(const char* pPacket, int nLength, int &nBytesUsed);		// 2012-06-08 by jhseol, �Ʒ��� �߰�����part2 - �ɸ��� : Ÿ�� ����
	ProcessResult Process_FC_ARENA_REQUEST_TEAM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ARENA_CREATE_TEAM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ARENA_ENTER_TEAM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ARENA_REAVE_TEAM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ARENA_TEAM_READY_FINISH(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ARENA_TEAM_READY_FINISH_CANCEL(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ARENA_ENTER_ROOM_WARP(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ARENA_FINISH_WARP(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ARENA_REQUEST_CREATE_TEAMINFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ARENA_REQUEST_OTHER_TEAM_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ARENA_QUICK_ENTER_TEAM(const char* pPacket, int nLength, int &nBytesUsed);
	// 2008-01-08 by dhjin, �Ʒ��� ���� - 	
	ProcessResult Process_FC_ARENA_CHARACTER_GAMESTART(const char* pPacket, int nLength, int &nBytesUsed);


	//////////////////////////////////////////////////////////////////////////
	// 2007-07-06 by dhjin, Tutorial
	ProcessResult Process_FC_TUTORIAL_START(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_TUTORIAL_COMPLETE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_TUTORIAL_END(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_TUTORIAL_WARP(const char* pPacket, int nLength, int &nBytesUsed);

	//////////////////////////////////////////////////////////////////////////
	// 2007-08-20 by dhjin, OutPost
//	ProcessResult Process_FC_OUTPOST_RESET_CHECK_START(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_OUTPOST_RESET_START(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_OUTPOST_NEXTWAR_INFO_REQUEST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_OUTPOST_NEXTWAR_SET_TIME(const char* pPacket, int nLength, int &nBytesUsed);

	//////////////////////////////////////////////////////////////////////////
	// 2007-08-22 by dhjin, Notice
	ProcessResult Process_FC_INFO_NOTICE_REQUEST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_NOTICE_REG(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_NOTICE_MODIFY(const char* pPacket, int nLength, int &nBytesUsed);

	//////////////////////////////////////////////////////////////////////////
	// 2007-08-22 by dhjin, �ǰ���
	ProcessResult Process_FC_INFO_EXPEDIENCYFUND_REQUEST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_EXPEDIENCYFUND_PAYBACK(const char* pPacket, int nLength, int &nBytesUsed);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-09-12 by cmkwon, ��Ʈ�� 2���н����� ���� - �߰��� �������� ó�� �Լ� �߰�
	ProcessResult Process_FC_INFO_SECONDARYPASSWORD_UPDATE_PASSWORD(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_SECONDARYPASSWORD_CHECK_PASSWORD(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_SECONDARYPASSWORD_LOCK(const char* pPacket, int nLength, int &nBytesUsed);
	
	ProcessResult Process_FC_INFO_GET_GUILDMARK(const char* pPacket, int nLength, int &nBytesUsed);

	// NPC
	ProcessResult Process_FN_MAPPROJECT_START(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MONSTER_CREATE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MONSTER_MOVE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_GET_CHARACTER_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MONSTER_DELETE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MONSTER_CHANGE_OK(const char* pPacket, int nLength, int &nBytesUsed);		// 2011-05-11 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ���� ��� �߰� - �ش� ��ä�� Ư�� ���� ���� ��� �߰�
	ProcessResult Process_FN_MISSILE_MOVE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MONSTER_HPRECOVERY(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MONSTER_HIDE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MONSTER_SHOW(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_BATTLE_ATTACK_PRIMARY(const char* pPacket, int nLength, int &nBytesUsed
		, MSG_FN_BATTLE_ATTACK_PRIMARY *pObjectMonsterAttackPrimary=NULL
		, CFieldMapProject *i_pFMapProject=NULL);
	ProcessResult Process_FN_BATTLE_ATTACK_SECONDARY(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_BATTLE_ATTACK_FIND(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_BATTLE_DROP_FIXER_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_BATTLE_ATTACK_HIDE_ITEM_W_KIND(const char* pPacket, int nLength, int &nBytesUsed);
	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 
	ProcessResult Process_FN_BATTLE_ATTACK_SKILL(const char* pPacket, int nLength, int &nBytesUsed);

	ProcessResult Process_FN_MONSTER_CHANGE_BODYCONDITION(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MONSTER_SKILL_USE_SKILL(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MONSTER_SKILL_END_SKILL(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MONSTER_AUTO_DESTROYED(const char* pPacket, int nLength, int &nBytesUsed);
	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 
	ProcessResult Process_FN_MONSTER_HPTALK(const char* pPacket, int nLength, int &nBytesUsed);
	// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -
	ProcessResult Process_FN_BATTLE_ATTACK_SKILL_CANCEL(const char* pPacket, int nLength, int &nBytesUsed);
	
	// Monitor	
	ProcessResult Process_FM_PRINTMAP(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FM_SAVE_MAPBLOCK_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FM_GET_MAP_USER_COUNTS(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FM_SET_SERVICE_STATE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FM_RELOAD_SERVER_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FM_INCREASE_CHANNEL(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FM_SET_CHANNEL_STATE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FM_CITYWAR_CHANGE_WAR_TIME(const char* pPacket, int nLength, int &nBytesUsed);

	// Admin
	ProcessResult Process_FC_ADMIN_GET_SERVER_STAT(const char* pPacket, int nLength, int &nBytesUsed);

	// Admin�� �Լ�
	BOOL CheckCommandExecutability(USHORT i_nAllowedRace, BOOL i_bAllowAdminTool = FALSE);
	BOOL HandleAdminCommands(const char *cmd);

	// Debug�� �Լ���
	ProcessResult Process_FM_PRINT_DEBUG_MSG(const char* pPacket, int nLength, int &nBytesUsed);

	// Client Report
	ProcessResult Process_FC_CLIENT_REPORT(const char* pPacket, int nLength, int &nBytesUsed);

	// 2007-12-27 by dhjin, �Ʒ��� ���� -	
	ProcessResult Process_FtoA_MFSINFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FtoA_AUTH_CHARACTER_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FtoA_ARENA_TEAM_MATCHING_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FtoA_ARENA_CHARACTER_RESET_RELOAD(const char* pPacket, int nLength, int &nBytesUsed);	// 2012-12-18 by jhseol, �Ʒ��� ĳ���� ���� �� ĳ���� ���� �ٽ� �ε��ϱ�.

	//////////////////////////////////////////////////////////////////////////
	// 2008-03-28 by dhjin, ���� ���� ǥ�� ��ȹ�� -
	ProcessResult Process_FC_INFO_MSWARINFO_DISPLAY_OPTION(const char* pPacket, int nLength, int &nBytesUsed);
	
	//////////////////////////////////////////////////////////////////////////
	// 2008-04-02 by dhjin, ����, ������ ����â ��ȹ�� - 
	ProcessResult Process_FC_INFO_MSWARINFO_RESULT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_SPWARINFO_RESULT(const char* pPacket, int nLength, int &nBytesUsed);

	//////////////////////////////////////////////////////////////////////////
	// 2008-04-29 by dhjin, EP3 ���� �ý��� -
	ProcessResult Process_FC_CHAT_LETTER_REQUEST_TITLE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHAT_LETTER_READ(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHAT_LETTER_DELETE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHAT_LETTER_SEND(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHAT_ALLLETTER_REQUEST_TITLE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHAT_ALLLETTER_READ(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHAT_ALLLETTER_DELETE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHAT_ALLLETTER_SEND(const char* pPacket, int nLength, int &nBytesUsed);

	//////////////////////////////////////////////////////////////////////////
	// 2008-12-08 by dhjin, �̼Ǹ�����
	ProcessResult Process_FC_QUEST_REQUEST_MISSIONMASTER_HELP(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_QUEST_MISSIONMASTER_HELP_INVITE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_QUEST_MISSIONMASTER_HELP_REJECT(const char* pPacket, int nLength, int &nBytesUsed);

	//////////////////////////////////////////////////////////////////////////
	// 2009-01-12 by dhjin, ���� ���� -	
	ProcessResult Process_FC_INFO_DECLARATION_MSWAR_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_DECLARATION_MSWAR_SET(const char* pPacket, int nLength, int &nBytesUsed);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 
	ProcessResult Process_FC_INFO_WRK_GET_SERVICE_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_WRK_GET_RANKER_LIST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_WRK_GET_SELF_RANKING(const char* pPacket, int nLength, int &nBytesUsed);

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 
	ProcessResult Process_FC_INFINITY_MODE_LIST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_READY_LIST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_CREATE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_JOIN(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_JOIN_REQUEST_MASTERUSER_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_MEMBER_INFO_LIST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_CHANGE_MASTER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_LEAVE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_BAN(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FtoA_INFINITY_IMPUTE_LIST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_READY(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_READY_CANCEL(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_START(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FtoA_INFINITY_START_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_MAP_LOADED(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_FIN_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FtoA_INFINITY_READY_FINISH_MAINSVR_START(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_TENDER_PUT_IN_TENDER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SHOP_INFINITY_BUY_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_JOIN_CANCEL(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_RESTART_BY_DISCONNECT(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��
	ProcessResult Process_FC_INFINITY_MAP_LOADED_RESTART_BY_DISCONNECT(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��
	ProcessResult Process_FC_ITEM_USE_INVOKING_WEAR_ITEM(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ������������� ���
	ProcessResult Process_FC_ITEM_EXPIRE_TIME_INVOKING_WEAR_ITEM(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ������������� ���� �ð� �Ϸ�
	ProcessResult Process_FC_ITEM_END_COOLINGTIME_ITEM(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - ��Ÿ�� ����
	ProcessResult Process_FC_ITEM_GET_COOLINGTIME_INFO(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������, ��Ÿ�� ���� ��û C->F

	ProcessResult Process_FtoA_INFINITY_START_CHECK_ACK(const char* pPacket, int nLength, int &nBytesUsed);		// 2010-03-23 by cmkwon, ���Ǵ�Ƽ ���� ĳ�� ������ ���� - 
	ProcessResult Process_FtoA_UPDATE_ITEM_NOTI(const char* pPacket, int nLength, int &nBytesUsed);				// 2010-03-31 by dhjin, ���Ǵ�Ƽ ���� ĳ�� ������ ���� -  
	
	//ProcessResult Process_FC_Difficulty_INFO(const char* pPacket, int nLength, int &nBytesUsed);				// 2010-05-13 by shcho, ���Ǵ�Ƽ ���̵� ���� - 

	ProcessResult Process_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL ( const char* pPacket , int nLength , int &nByteUsed );		// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ��Ƽ��Ƽ ���� ���̵� ���� ��û.

	ProcessResult Process_FC_INFINITY_SKIP_ENDINGCINEMA ( const char* pPacket , int nLength , int &nByteUsed );			// ���Ǵ�Ƽ ���� �ó׸� ��ŵ ��û C->F // 2010. 07. 27 by hsLee ���Ǵ�Ƽ 2�� ���� ��� �ó׸� ���� ��ŵ ó��.

	
	/////////////////////////////////////////////////////////////
	// start 2011-08-22 by hskim, ��Ʈ�� �ý��� 2�� - ��� ����

	ProcessResult Process_FC_ITEM_PET_SET_NAME(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_PET_SET_EXP_RATIO(const char* pPacket, int nLength, int &nBytesUsed);
	
	ProcessResult Process_FC_ITEM_PET_SET_SOCKET(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_PET_SET_KIT_SLOT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_PET_SET_AUTOSKILL_SLOT(const char* pPacket, int nLength, int &nBytesUsed);

	BOOL PetChangeLevel(int Level, float PercentageOfExp = NULL);
	BOOL PetChangeExperience(Experience_t PetExperience, BOOL bAbs = FALSE);
	void SendPetInfo(MessageType_t msgType, tPET_CURRENTINFO *pPetCurrentInfo);

	// end 2011-08-22 by hskim, ��Ʈ�� �ý��� 2�� - ��� ����
	//////////////////////////////////////////////////////////
	// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
	ProcessResult Process_FI_GUILD_BOUNUEXP_RATE(const char* pPacket, int nLength, int &nBytesUsed);


	ProcessResult Process_FC_COLLECTION_SHAPE_CHANGE(const char* pPacket, int nLength, int &nBytesUsed);	// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - ��Ŷ �ޱ�

	void SendToClient_KillCount();
	void SendToClient_RestAndKillCount();

	void SendToClient_BonusExpRate();
	// end 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������

	// ����׿�
	void PRINT_DEBUG_MESSAGE(char*& msg);
	mt_lock	PRINT_DEBUG_MESSAGE_LOCK;
	void DBGOUT2(BitFlag8_t bType, char* pFormat, ...);
	void StartTestDBG(int nParam1, int nParam2, int nParam3, const char *szParam1, const char *szParam2, const char *szParam3);

public:
	// Send Error Msg
	void SendErrorMessage(MessageType_t msgType, Err_t err, int errParam1 = 0, int errParam2 = 0, char* errMsg = NULL, BOOL bCloseConnection = FALSE, BOOL i_bWriteSystemLog=TRUE);
	void SendNetworkErrorMessage(int i_nWriteBufCounts, int i_nBadNetworkContinueTime);
	void SendErrorMessageFromFieldDB(EnumQueryType qType, Err_t err, int errParam1 = 0, int errParam2 = 0, char* errMsg = NULL, BOOL bCloseConnection = FALSE);

	void ADDString(string * o_pszString, int i_int)
	{// 2007-01-17 by dhjin, ��Ʈ�� �����ִ� �Լ�, �켱 FieldIOCPSocket�� �߰���
		char temp[1028];
		util::zero(temp,1024);

		itoa(i_int, temp, 10);
		*o_pszString += temp;
	}

	struct loopdata
	{
		bool running;
		bool pleaseStop;
		bool pleaseDel;

		unsigned interval;
		unsigned hpamount;
		unsigned dpamount;

		loopdata(unsigned interval, unsigned hpamount, unsigned dpamount) :
			running { false },
			pleaseStop { false },
			pleaseDel { false },
			interval { interval },
			hpamount { hpamount },
			dpamount { dpamount } { }
	};

	struct syncdata
	{
		loopdata* data;

		syncdata() : data { nullptr } { }

		~syncdata();

		bool running() const { return data && data->running; }
	};

	syncdata dploopdata { };

	bool CheckDPLoop();
};

// 2008-12-22 by dhjin, �̼Ǹ����� - ������� �ø�.
// typedef vector<CFieldIOCPSocket*>			vectCFieldIOCPSocket;					// 2007-02-12 by cmkwon



// CIOCPSocket�� �ν��Ͻ��� ��� �������� ���θ� ����, restrict checking by kelovon
BOOL CFieldIOCPSocket::IsUsingRestrict(ClientIndex_t ClientIndex, UID32_t CharacterUniqueNumber)
{
	return (m_bUsing
			&& (ClientIndex == m_character.ClientIndex)
			&& (CharacterUniqueNumber == m_character.CharacterUniqueNumber)
			&& (GetClientState() == CS_PLAYING));
}


void CFieldIOCPSocket::SetClientState(CS_State csState)
{
	m_CSClientState = csState;
}

// check: MapChannel ������, 20040324, kelovon, NPC���� ������ ä�θ��� �ϳ��� �ε��� �����ؾ� ��
BOOL CFieldIOCPSocket::InitializeNPCMap(CMapProject *pmapProject, map<int, MONSTER> *mapMonsterParam)
{
	m_pFieldMapProjectForNPC = reinterpret_cast<CFieldMapProject*>(pmapProject);
	m_pFieldMapProjectForNPC->SetIOCPSocketMapSession(this);
	return TRUE;
}

#endif // !defined(AFX_FIELDIOCPSOCKET_H__6FB6A18E_0C74_4E46_AB75_2ECA21C4204B__INCLUDED_)
