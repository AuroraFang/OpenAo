#ifndef __ATUM_FIELD_DB_MANAGER_H__
#define __ATUM_FIELD_DB_MANAGER_H__

#include "AtumDBManager.h"
#include "AtumProtocol.h"

class CFieldIOCP;
class CShopInfo;

struct QPARAM_DELETESTOREITEM
{
	UID32_t		AccountUniqueNumber;
	UID64_t		ItemUniqueNumber;			// STORE DB�� index
};

struct QPARAM_BUFFER
{
	INT			nSize;
	char		*pBuffer;
	UINT		GeneralParam1;
	UINT		GeneralParam2;
};

struct QPARAM_UPDATEITEMCOUNT
{
	UID64_t		ItemUniqueNumber;
	INT			Count;
};

struct QPARAM_UPDATEITEMNUM			// 2006-06-14 by cmkwon
{
	UID64_t		ItemUniqueNumber;
	INT			ItemNum;
};

struct QPARAM_UPDATE_ENDURANCE
{
	UID64_t		ItemUniqueNumber;
	float		Endurance;
};

struct QPARAM_STORE_UPDATE
{// 2006-09-19 by dhjin, ���� �κ�, â�� ���� ����
	UID64_t		ItemUniqueNumber;
	UID32_t		CharacterUID;
	BYTE		ItemStorage;
};

struct QPARAM_STORE_UPDATE_COLORCODE
{
	UID64_t		ItemUniqueNumber;
	INT			ColorCode;
};

struct QPARAM_UPDATE_SHAPEITEMNUM		// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
{
	UID64_t		ItemUID;
	INT			nShapeItemNum;

	FIXED_TERM_INFO	FixedTermShape;		// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)
};

struct QPARAM_UPDATE_ITEM_USINGTIMESTAMP
{
	UID64_t		ItemUniqueNumber;
	INT			UsingTimeStamp1;
};

struct QPARAM_UPDATE_ITEM_RARE_FIX
{
	UID64_t	ItemUID;
	INT		PrefixCodeNum;	// ���λ�, ������ 0
	INT		SuffixCodeNum;	// ���̻�, ������ 0
};

struct QPARAM_INSERTDEFAULTITEMS
{
	UID32_t		CharacterUniqueNumber;
};

// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)
struct QPARAM_FIXED_TERM_SHAPE
{
	UID64_t			ItemUID;
	INT16			TermType;
	INT				AppliedItemNum;
	ATUM_DATE_TIME	StartDate;
	ATUM_DATE_TIME	EndDate;
};
// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)

// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - QPARAM_SHAPE_STATLEVEL ����ü �߰�
struct QPARAM_SHAPE_STATLEVEL
{
	BYTE			ItemKind;
	UID64_t			ItemUID;
	ItemNum_t		nStatShapeItemNum;	// �ҽ��ε���(�������ѹ�)
	INT				nStatLevel;			// ����
	QPARAM_SHAPE_STATLEVEL()
	{
		memset(this,0x00,sizeof(QPARAM_SHAPE_STATLEVEL));
	}
};
// end 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - QPARAM_SHAPE_STATLEVEL ����ü �߰�

class CFieldIOCPSocket;

struct QPARAM_EXECUTETRADE
{
	CFieldIOCPSocket *pPeerTraderSocket;
};
struct QPARAM_TRADE_MOVEITEM		// 2006-05-11 by cmkwon
{
	UID32_t		ToAccountUID;			
	UID32_t		ToCharacterUID;			
	UID32_t		FromCharacterUID;		
	UID64_t		MoveItemUID;				// �̵��� �������� UID
};

struct QPARAM_BUY_ITEM
{
	MSG_FC_SHOP_BUY_ITEM	MsgBuyItem;
	CShopInfo				*pShopInfo;
	SHOP_ITEM				*pShopItem;
	ITEM_GENERAL			*pMoney;
};

struct QPARAM_SELL_ITEM
{
	MSG_FC_SHOP_SELL_ITEM	MsgSellItem;
	ITEM_GENERAL			*pItemToSell;
	ITEM					*pItemInfoToSell;
	ITEM_GENERAL			*pMoney;
};

struct QPARAM_THROW_AWAY_ITEM
{
	MSG_FC_ITEM_THROW_AWAY_ITEM	MsgThrowAwayItem;
	ITEM_GENERAL				*pItemToThrowAway;
};

struct QPARAM_CHARACTER_CHANGE_UNITKIND
{
	UID32_t	CharacterUniqueNumber;
	USHORT	UnitKind;
};

struct QPARAM_CHARACTER_SAVE_CRITICAL_DATA
{
	UID32_t			CharacterUID;
	BYTE			Level;
	BYTE			CharacterMode0;		// 2005-10-06 by cmkwon, ĳ���͸�� ������ ����
	Experience_t	Experience;
	Experience_t	DownExperience;		// 2005-03-23 by cmkwon
	INT				DownSPIOnDeath0;	// 2006-04-10 by cmkwon
	BodyCond_t		BodyCondition;
	BYTE			PetLevel;
	Experience_t	PetExperience;
	float			Position_X;
	float			Position_Y;
	float			Position_Z;
	MapIndex_t		MapIndex;
	ChannelIndex_t	ChannelIndex;
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	INT		HP;
	INT		DP;
#else
	SHORT	HP;
	SHORT	DP;
#endif
	SHORT			SP;
	SHORT			EP;
	float			CurrentHP;
	float			CurrentDP;
	SHORT			CurrentSP;
	float			CurrentEP;
	LONGLONG		TotalPlayTime;
	INT				GameContinueTimeInSecondOfToday;		// 2006-11-15 by cmkwon
	ATUM_DATE_TIME	LastGameEndDate;						// 2006-11-15 by cmkwon
	LONGLONG		PCBangTotalPlayTime;					// 2007-06-07 by dhjin, PC�� �� �÷��� �ð�.
	INT				SecretInfoOption;				// 2008-06-23 by dhjin, EP3 ���������ɼ� -
};

struct QPARAM_CHARACTER_CHANGE_STAT
{
	UID32_t		CharacterUniqueNumber;
	BYTE		byAutoStatType;
	GEAR_STAT	GearStat1;		// ��� ����
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	INT		HP;
	INT		DP;
#else
	SHORT	HP;
	SHORT	DP;
#endif
	SHORT		SP;
	SHORT		EP;
	BYTE		BonusStat;
};


struct QPARAM_CHARACTER_CHANGE_BONUSSTATPOINT		// 2005-11-15 by cmkwon, �߰���
{
	UID32_t		CharacterUniqueNumber;
	BYTE		BonusStatPoint;
};

struct QPARAM_CHARACTER_CHANGE_GUILD
{
	UID32_t	CharacterUniqueNumber;
	char	Guild[SIZE_MAX_GUILD_NAME];
};

struct QPARAM_CHARACTER_CHANGE_LEVEL
{
	UID32_t	CharacterUniqueNumber;
	BYTE	Level;
	BYTE	BonusStat;
// 2005-11-15 by cmkwon, ������
//	BYTE	BonusSkillPoint;
};

struct QPARAM_CHARACTER_CHANGE_EXP
{
	UID32_t			CharacterUniqueNumber;
	Experience_t	Experience;						// �� ����ġ
};

struct QPARAM_CHARACTER_CHANGE_BODYCONDITION
{
	UID32_t		CharacterUniqueNumber;
	BodyCond_t	BodyCondition;					// ���� 32bit�� ��� <-- check: ��ų bodycon�� �������� �����ϱ�!
};

/*
struct QPARAM_CHARACTER_CHANGE_PROPENSITY
{
	UID32_t	CharacterUniqueNumber;
	INT		Propensity;
};
*/

struct QPARAM_CHARACTER_CHANGE_INFLUENCE_TYPE
{
	UID32_t		AccountUniqueNumber;				// 2005-12-07 by cmkwon, ���ð��ɼ��¸���Ʈ ������ ����
	UID32_t		CharacterUniqueNumber;
	BYTE		InfluenceType0;
	BYTE		SelectableInfluenceMask0;			// 2005-12-07 by cmkwon, ���ð��ɼ��¸���Ʈ ������ ����
};

struct QPARAM_CHARACTER_CHANGE_STATUS
{
	UID32_t	CharacterUniqueNumber;
	BYTE	Status;
};

struct QPARAM_CHARACTER_CHANGE_PKPOINT
{
	UID32_t	CharacterUniqueNumber;
	INT		Propensity;
	UINT	PKWinPoint;
	UINT	PKLossPoint;
};

struct QPARAM_CHARACTER_CHANGE_RACINGPOINT
{
	UID32_t	CharacterUniqueNumber;
	INT		RacingPoint;
};

struct QPARAM_CHARACTER_UPDATE_TOTAL_PLAY_TIME
{
	UID32_t		CharacterUniqueNumber;
	LONGLONG	TotalPlayTime;
};

struct QPARAM_CHARACTER_CHANGE_HPDPSPEP
{
	UID32_t	CharacterUniqueNumber;
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	INT		HP;
	INT		DP;
#else
	SHORT	HP;
	SHORT	DP;
#endif
	SHORT	SP;
	SHORT	EP;
	float	CurrentHP;
	float	CurrentDP;
	SHORT	CurrentSP;
	float	CurrentEP;
};

struct QPARAM_CHARACTER_CHANGE_CURRENTHPDPSPEP
{
	UID32_t	CharacterUniqueNumber;
	float	CurrentHP;
	float	CurrentDP;
	SHORT	CurrentSP;
	float	CurrentEP;
};

struct QPARAM_CHARACTER_CHANGE_MAPCHANNEL
{
	UID32_t				CharacterUniqueNumber;
	MAP_CHANNEL_INDEX	MapChannelIndex;
};

struct QPARAM_CHARACTER_CHANGE_PETINFO
{
	UID32_t			CharacterUniqueNumber;
	BYTE			Level;
	Experience_t	Experience;						// �� ����ġ
};

struct QPARAM_CHARACTER_CHANGE_POSITION
{
	UID32_t		CharacterUniqueNumber;
	D3DXVECTOR3	PositionVector;		// ĳ���� ��ǥ
};

struct QPARAM_UPDATE_QUEST
{
	UID32_t		CharacterUniqueNumber;
	INT			QuestIndex;
	BYTE		QuestState;				// QUEST_STATE_XXX
	LONGLONG	QuestProgressTimeInSecond;		// 2006-08-31 by cmkwon, ����Ʈ �Ϸ�� ���� �ð�(��), ����Ʈ ���۽� TotalPlayTime
};

struct QPARAM_DELETE_QUEST
{
	UID32_t	CharacterUniqueNumber;
	INT		QuestIndex;
};

struct QPARAM_UPDATE_QUEST_MONSTER_COUNT
{
	UID32_t	CharacterUniqueNumber;
	INT		QuestIndex;
	INT		MonsterUniqueNumber;
	INT		Count;
};

struct QPARAM_DELETE_QUEST_MONSTER_COUNT
{
	UID32_t	CharacterUniqueNumber;
	INT		QuestIndex;
};

struct QPARAM_CHANGE_QUEST_INDEX		// 2009-03-31 by cmkwon, �����ʱ�ȭ �ý��� ���� - 
{
	UID32_t	CharacterUID;
	INT		FromQuestIndex;
	INT		ToQuestIndex;
};

struct QPARAM_GENERAL_EXEC
{
	char	QueryString[256];
};

struct QPARAM_CHANGE_ITEM_WINDOW_POSITION
{
	UID32_t	AccountUID;
	UID32_t	CharacterUID;
	UID64_t	ItemUID1;
	INT		ItemWindowIndex1;
	BYTE	ItemWear1;
	UID64_t	ItemUID2;
	INT		ItemWindowIndex2;
	BYTE	ItemWear2;
};

struct QPARAM_CASH_INSERT_PREMIUM_CARD
{
	UID32_t			nAccountUID;
	INT				nItemNum;
	ATUM_DATE_TIME	atumTimeCurrentTime;
	ATUM_DATE_TIME	atumTimeExpireTime;	
};

typedef QPARAM_CASH_INSERT_PREMIUM_CARD		QPARAM_CASH_UPDATE_PREMIUM_CARD;

struct QPARAM_CASH_DELETE_PREMIUM_CARD
{
	UID32_t			nAccountUID;
};

struct QPARAM_UPDATE_HAPPY_HOUR_EVENT_DATE
{
	BYTE			byInfluenceType0;		// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - QPARAM_UPDATE_HAPPY_HOUR_EVENT_DATE ����ü�� �ʵ� �߰� ��
	ATUM_DATE_TIME	atimeStartTime2;
	ATUM_DATE_TIME	atimeEndTime2;
};

struct QPARAM_UPDATE_INFLUENCE_WAR_DATA		// 2006-04-14 by cmkwon
{
	BYTE			InfluenceType0;
	//INT				MGameServerID0;
	BYTE			WartimeStage0;
	INT				ContributionPoint0;
};

// 2013-05-09 by hskim, ���� ����Ʈ ����
struct QPARAM_UPDATE_INFLUENCE_CONSECUTIVE_VICTORIES
{
	BYTE			InfluenceType0;
	//INT				MGameServerID0;
	SSHORT			ConsecutiveVictories;
};
// end 2013-05-09 by hskim, ���� ����Ʈ ����

struct QPARAM_UPDATE_OWNEROFCONFLICTAREA	// 2006-07-12 by cmkwon
{
	BYTE			InfluenceType0;			// ���� ����
	//INT				MGameServerID0;
};

struct FIELD_DB_QUERY
{
	CFieldIOCPSocket*	pFieldIOCPSocket;
	UID32_t				nCharacterUID;
	EnumQueryType		enumQuryType;
	void*				pQueryParam;
	void*				pGeneralParam;		// General-Purpose void* Parameter
	INT64				nGeneralParam1;		// General-Purpose INT   Parameter
	INT64				nGeneralParam2;		// General-Purpose INT   Parameter
	DWORD				dwProcessedTick;	// ������ ó���Ǵ� �ð�

	FIELD_DB_QUERY(const DB_QUERY& rhs)
	{
		this->pFieldIOCPSocket	= (CFieldIOCPSocket*)rhs.pIOCPSocket;
		this->nCharacterUID		= rhs.nCharacterUID;
		this->enumQuryType		= rhs.enumQueryType;
		this->pQueryParam		= rhs.pQueryParam;
		this->pGeneralParam		= rhs.pGeneralParam;
		this->nGeneralParam1	= rhs.nGeneralParam1;
		this->nGeneralParam2	= rhs.nGeneralParam2;
		this->dwProcessedTick	= rhs.dwProcessedTick;
	}
};
// 2011-08-25 by shcho, Ƚ���� ������ ���ޱ�� ���� - ���� ����ü �߰�
struct QPARAM_CHECK_LOGINEVENTITEM
{ // ���������� ����Ѵ�.
	int		EventUID;
	int		AccountUID;
	int		CompleteFlag; // check�� ���� ��쿡�� ������� �ʴ´�.	
};
// 2011-08-25 by shcho, Ƚ���� ������ ���ޱ�� ���� - ���� ����ü �߰�

// 2006-08-25 by dhjin, �̺�Ʈ ������ ��� �޾ƿ��� ���ν����� ����
struct QPARAM_CHECK_EVENTITEM
{
	int		AccountUID;
	int		ItemEventUID;
	int		ItemEventType;
	int		ItemNum;
	int		Count;
	int		PrefixCodeNum;			// 2007-07-25 by dhjin, ���� �ɼ�
	int		SuffixCodeNum;			// 2007-07-25 by dhjin, ���� �ɼ�
	INT		CharacterUID;			// 2009-11-19 by cmkwon, ������ �̺�Ʈ�� Account/Character üũ ��� �߰� -

	BOOL	UseFixedPeriod;			// 2013-02-28 by bckim, �������� �����߰�
	INT		FixedPeriod_DAY;		// 2013-02-28 by bckim, �������� �����߰�

	// 2013-04-22 by jhseol, ������ �̺�Ʈ - ����Ÿ�� �߰����� �ý��� ����
	BYTE	InsertStorage;		// �������� �߰��� ��ġ (ĳ���� or â��)

	QPARAM_CHECK_EVENTITEM()
	{
		memset(this, 0x00, sizeof(QPARAM_CHECK_EVENTITEM));
		InsertStorage = ITEM_IN_CHARACTER;	// �⺻���� ĳ���ͷ� ��´�.
	}
	// end 2013-04-22 by jhseol, ������ �̺�Ʈ - ����Ÿ�� �߰����� �ý��� ����
};
typedef mt_vector<QPARAM_CHECK_EVENTITEM>			mtvectQPARAM_CHECK_EVENTITEM;		// 2008-01-10 by cmkwon, ������ �̺�Ʈ �ý��ۿ� �� ���� �ý��� �߰� - 

// 2006-08-25 by dhjin, �̺�Ʈ ������ ���� ���� ����� �ϴ� ���ν����� ����
struct QPARAM_INSERT_EVENTITEM
{
	int				AccountUID;
	int			    CharacterUID;
	int				ItemEventUID;
	ATUM_DATE_TIME  InsertItemTime;		// 2007-12-18 by dhjin, �̺�Ʈ ������ ���� �ð� ���� ����
};

struct QPARAM_CheckCouponEvent			// 2008-01-10 by cmkwon, ������ �̺�Ʈ �ý��ۿ� �� ���� �ý��� �߰� - 
{
	char	AccName[SIZE_MAX_ACCOUNT_NAME];
	char	CouponNumber[SIZE_MAX_COUPON_NUMBER];
	int		ItemEventUID;
};

struct QPARAM_GET_QUICKSLOT
{// 2006-09-04 by dhjin, �� ���� ���� ������ ���ν����� ����
	int				AccountUID;
	int			    CharacterUID;
	int				SlotPurpose;		// 2012-06-21 by jhseol, �Ʒ��� �߰�����part2 - �Ʒ��� ���� ������
};

struct QPARAM_DELETE_QUICKSLOT
{// 2006-09-04 by dhjin, �� ���� ���� ���ν����� ����
	int				AccountUID;
	int			    CharacterUID;
	int				TabIdx;
	int				SlotPurpose;		// 2012-06-21 by jhseol, �Ʒ��� �߰�����part2 - �Ʒ��� ���� ������
};

struct QPARAM_UPDATE_QUICKSLOT
{// 2006-09-04 by dhjin, �� ���� ������Ʈ ���ν����� ����
	int				AccountUID;
	int			    CharacterUID;
	int				TabIdx;
	int				arrItemNum_[QUICKSLOTCOUNT];
	int				SlotPurpose;		// 2012-06-21 by jhseol, �Ʒ��� �߰�����part2 - �Ʒ��� ���� ������
};

struct QPARAM_UPDATE_GUILDSTORE
{// 2006-09-20 by dhjin, ���� â�� ������Ʈ ���ν��� QPARAM
	UID64_t			ItemUID;
	UID32_t			AccountUID;
	UID32_t			Possess;
	BYTE			ItemStorage;
	INT				Count;
};

struct QPARAM_INSERT_GUILDSTORE
{// 2006-09-20 by dhjin, ���� â�� �߰� ���ν��� QPARAM
	UID32_t			GuildUID;
	INT				ItemNum;
	INT				Count;
};

struct QPARAM_DELETE_GUILDSTORE
{// 2006-09-20 by dhjin, ���� â�� ���� ���ν��� QPARAM
	UID64_t			ItemUID;
	UID32_t			GuildUID;
	BYTE			byItemStorage;			// ITEM_IN_XXX
	BOOL			SendMSG;	
};

struct QPARAM_GET_GUILDSTORE
{// 2006-09-20 by dhjin, ���� â�� ������ ���ν��� QPARAM
	UID32_t			GuildUID;
};

struct QPARAM_UPDATE_SKILLUSETIME
{// 2006-12-06 by dhjin, ��ų ��� �ð� ������Ʈ�ϴ� ���ν��� QPARAM
	UID64_t			ItemUID;
	ATUM_DATE_TIME	UseTime;
};

struct QPARAM_UPDATE_SUBLEADER
{// 2007-02-13 by dhjin, �������� ���� ���ν��� QPARAM
	BYTE			InflType;
	BYTE			SubLeaderRank;		// 2007-10-06 by dhjin, �� ��° ������������
	char			CharacterName[SIZE_MAX_CHARACTER_NAME];
	BYTE			OrderNormalRace;	 // 2008-08-21 by dhjin, �Ϲ�, Ư�� ������ �������� �Ӹ� ����
};

//////////////////////////////////////////////////////////////////////////
// 2007-02-28 by dhjin, ��������Ʈ ���� ���� 
struct QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_COUNT
{// 2007-02-28 by dhjin, ��������Ʈ ���� ���� ������Ʈ
	MapIndex_t		MapIndex;
	BYTE			SummonCount;
};

struct QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME
{// 2007-02-28 by dhjin, ��������Ʈ ���� ���� ������Ʈ
	MapIndex_t		MapIndex;
	ATUM_DATE_TIME	SummonTime;
	BOOL			SummonAttribute;		// 2008-04-08 by dhjin, ��ȯ ���� �ð� ���� - 
};

struct QPARAM_DELETE_INSERT_STRATEGYPOINTSUMMONINFO
{// 2007-02-28 by dhjin, ��������Ʈ ���� ���� ������Ʈ
	MapIndex_t		MapIndex;
};

struct QPARAM_JAMBOREE_ENTRANTS			// 2007-04-10 by cmkwon, ��ȸ������ DB(atum2_db_20)�� ĳ���� ���� ����
{
	char		szCharName0[SIZE_MAX_CHARACTER_NAME];
	BYTE		byInflTy;				// 2008-04-15 by cmkwon, ��ȸ����(JamboreeServer)�� �ý��� ���� - "/��ȸ����������" ��ɾ� ����
	MapIndex_t	MapIndex;				// 2008-04-15 by cmkwon, ��ȸ����(JamboreeServer)�� �ý��� ���� - "/��ȸ����������" ��ɾ� ����
};

struct QPARAM_UPDATE_WARPOINT
{// 2007-05-15 by dhjin, ����Ǵ� WarPoint ��
	INT			TotalWarPoint;
	INT			CumulativeWarPoint;
	UID32_t		AccountUID;
	UID32_t		CharacterUID;
};

struct QPARAM_UPDATE_ARENA_RESULT
{// 2007-06-07 by dhjin, Arena��� ������Ʈ
	INT			TotalWarPoint;
	INT			CumulativeWarPoint;
	INT			ArenaWin;
	INT			ArenaLose;
	UID32_t		AccountUID;
	UID32_t		CharacterUID;	
};

struct QPARAM_UPDATE_ARENA_DISCONNECT
{// 2007-06-07 by dhjin, Arena �������� ������Ʈ
	INT			ArenaDisConnect;
	UID32_t		AccountUID;
	UID32_t		CharacterUID;
};

///////////////////////////////////////////////////////////////////////////////
// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : AS���� ���� CharacterArena ���� ������Ʈ
struct QPARAM_UPDATE_ARENA_CHARACTERARENA
{
	UID32_t		AccountUID;
	UID32_t		CharacterUID;
	SArenaPlayInfo	ArenaInfo;
};
// end 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : AS���� ���� CharacterArena ���� ������Ʈ


//////////////////////////////////////////////////////////////////////////
// 2007-07-06 by dhjin, Tutorial
struct QPARAM_INSERT_TUTORIAL_COMPLETE
{// 2007-07-06 by dhjin, Tutorial �Ϸ� ����
	UID32_t		CharacterUID;		
	INT			TutorialUID;					// Tutorial ��ȣ 
};


///////////////////////////////////////////////////////////////////////////////
// 2007-08-30 by cmkwon, ȸ�Ƿ� �ý��� ���� - DB Query ����ü �߰�
struct QPARAM_Insert2WarpableUserList		// 2007-08-30 by cmkwon, ȸ�Ƿ� �ý��� ���� -
{
	INT			MapIndex;
	BYTE		byInfluenceType0;
	char		CharacterName0[SIZE_MAX_CHARACTER_NAME];
};

struct QPARAM_DeleteWarpableUser			// 2007-08-30 by cmkwon, ȸ�Ƿ� �ý��� ���� -
{
	INT			MapIndex;
	UID32_t		CharacterUID0;
};


//////////////////////////////////////////////////////////////////////////
// 2007-08-21 by dhjin, OutPost
struct QPARAM_SET_NPCPOSSESSION_OUTPOST
{// 2007-08-21 by dhjin, �������� NPC������ ����
	INT				MapIndex;
	ATUM_DATE_TIME	GetTime;
	ATUM_DATE_TIME	NextWarTime;
};

struct QPARAM_SET_GUILDPOSSESSION_OUTPOST
{// 2007-08-21 by dhjin, �������� NPC������ ����
	INT				MapIndex;
	BYTE			Influence;
	UID32_t			GuildUID;
	ATUM_DATE_TIME	GetTime;
	ATUM_DATE_TIME	NextWarTime;
};

struct QPARAM_SET_OUTPOST_NEXTWAR_TIME
{// 2007-08-21 by dhjin, �������� ���� ���� �ð� ����
	INT				MapIndex;
	BYTE			Influence;
	UID32_t			GuildUID;
	ATUM_DATE_TIME	NextWarTime;
};

//////////////////////////////////////////////////////////////////////////
// 2007-08-22 by dhjin, CityLeader
struct QPARAM_INIT_EXPEDIENCY_FUND
{// 2007-08-22 by dhjin, �ǰ��� �ʱ�ȭ ����
	INT				MapIndex;
	BYTE			Influence;
	UID32_t			CharacterUID;
	INT				ExpediencyFundCumulative;
	INT				ExpediencyFund;
	INT				ExpediencyRate;
};

struct QPARAM_SET_EXPEDIENCY_FUND
{// 2007-08-22 by dhjin, �ǰ��� ���� ����
	INT				MapIndex;
	INT				ExpediencyFundCumulative;
	INT				ExpediencyFund;
};

struct QPARAM_SET_EXPEDIENCY_FUNDPAYBACK
{// 2007-08-22 by dhjin, �ǰ��� ȯ�� ����
	INT				MapIndex;
	BYTE			Influence;
	UID32_t			CharacterUID;
	INT				ExpediencyFund;
};

struct QPARAM_SET_EXPEDIENCY_FUNDRATE
{// 2007-08-22 by dhjin, �ǰ����� ���� ����
	INT				MapIndex;
	BYTE			Influence;
	UID32_t			CharacterUID;
	INT				ExpediencyRate;
};

struct QPARAM_DELETE_CITYLEADER_INFO
{// 2007-08-22 by dhjin, ���� ���� ����
	INT				MapIndex;
	BYTE			Influence;
	UID32_t			CharacterUID;
};

struct QPARAM_REG_NOTICE
{// 2007-08-22 by dhjin, �������� ���
	INT				MapIndex;
	UID32_t			CharacterUID;
	CHAR			Notice[SIZE_MAX_NOTICE];
};
typedef QPARAM_REG_NOTICE		QPARAM_MODIFY_NOTICE;

struct QPARAM_UPDATE_SECONDARY_PASSWORD
{// 2007-09-12 by cmkwon, ��Ʈ�� 2���н����� ���� - ���ν��� ȣ���� ���� ����ü �߰�
	UID32_t			AccountUID;
	char			NewSecPassword[SIZE_MAX_PASSWORD_MD5_STRING];
};

//////////////////////////////////////////////////////////////////////////
// 2007-10-30 by dhjin, Poll����
struct QPARAM_INSERT_LEADER_CANDIDATE
{// 2007-10-30 by dhjin, �ĺ��� ���
	BYTE	Influence;
	INT		ReqGuildMemberNum;
	INT		ReqGuildFame;
	UID32_t	AccountUID;
	UID32_t	CharacterUID;
	UID32_t	GuildUID;
	CHAR	GuildName[SIZE_MAX_GUILD_NAME];
	CHAR	CharacterName[SIZE_MAX_CHARACTER_NAME];
	CHAR	CampaignPromises[SIZE_MAX_CAMPAIGNPROMISES];
};

struct QPARAM_UPDATE_LEADERPOLLCOUNT
{// 2007-11-06 by dhjin, ��ǥ ���� ������Ʈ
	INT		LeaderCandidateNum;				//	�ĺ��� ��ȣ
	BYTE	Influence;						//	�ĺ��� ����
	INT		PollCount;						//	��ǥ ����
};

struct QPARAM_INSERT_VOTER_LIST
{// 2007-10-31 by dhjin, ��ǥ�� ���
	UID32_t		AccountUID;						//	��ǥ�� ���� UID
	UID32_t		CharacterUID;					//	��ǥ�� UID
	INT			LeaderCandidateNum;				//	��ǥ�ڰ� ��ǥ�� �ĺ��� ��ȣ
};

struct QPARAM_UPDATE_ConnectingServerGroupID
{// 2007-11-06 by cmkwon, ���� �α� DB ���� ���� �����ϱ�
	UID32_t		AccountUID;						//	���� UID
	INT			ServerGroupID;					// 2007-11-06 by cmkwon, 0�̸� ���� ����
};

struct QPARAM_GiveStoreItem
{// 2007-11-27 by cmkwon, �����ϱ� �α� ���� - QPARAM_GiveStoreItem �߰�
	UID32_t			SenderCharUID;
	char			SenderCharacterName[SIZE_MAX_CHARACTER_NAME];	
	ITEM_GENERAL	itemG;
};

//////////////////////////////////////////////////////////////////////////
// 2007-12-28 by dhjin, �Ʒ��� ���� - 
struct QPARAM_ARENA_UPDATE_CHARACTERINFO
{// 2007-12-28 by dhjin, �Ʒ��� ���� - �ɸ��� ���� ������Ʈ
	CHAR			NewArenaCharacterName[SIZE_MAX_CHARACTER_NAME]; 
	ClientIndex_t	AFSClientIndex;
};

struct QPARAM_ARENA_COPY_DB_INFO
{// 2007-12-28 by dhjin, �Ʒ��� ���� - �Ʒ��� DB�� ���� ����
	UID32_t			ArenaCharacterUID;
	UID32_t			CharacterUID;
	UID32_t			AccountUID;
	ClientIndex_t	AFSClientIndex;
	BOOL			InfinityCopy;		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���Ǵ�Ƽ �̿����� üũ ��Ʈ 
};

typedef MSG_FC_SHOP_CHECK_GIVE_TARGET	QPARAM_CHECK_GiveTarget;	// 2007-11-13 by cmkwon, �����ϱ� ��� �߰� - 

//////////////////////////////////////////////////////////////////////////
// 2008-04-10 by dhjin, ���� ���� ǥ�� ��ȹ�� - 
struct QPARAM_UPDATE_MSWAR_OPTION_TYPE
{
	BYTE		InfluenceType;
	SHORT		MSWarOptionType;
};

struct QPARAM_INSERT_MSWAR_LOG
{// 2008-08-28 by dhjin, ���� ����, ����DB�� ���ܾ� Admintool�� �ʱ�ȭ�� �����ϴ�.
	BYTE			AttInfluence;
	INT				MonsterUID;
	INT				ContributionPoint;
	ATUM_DATE_TIME	MSWarStartTime;
	BYTE			WinInfluence;	
};

struct QPARAM_INSERT_SPWAR_LOG
{// 2008-08-28 by dhjin, ���� ����, ����DB�� ���ܾ� Admintool�� �ʱ�ȭ�� �����ϴ�.
	BYTE			AttInfluence;
	INT				SPSummonMapIndex;
	ATUM_DATE_TIME	SPWarStartTime;
	BYTE			WinInfluence;	
};

// 2008-11-04 by dhjin, ��Ű�ӽ�
struct QPARAM_UPDATE_LUCKYITEM_DROPCOUNT
{
	INT				MysteryItemDropNum;
	INT				DropItemNum;
	INT				DropCount;
};

struct QPARAM_UPDATE_LUCKYITEM_STARTTIME
{
	INT				MysteryItemDropNum;
	ATUM_DATE_TIME	StartTime;		
};

//////////////////////////////////////////////////////////////////////////
// 2009-01-12 by dhjin, ���� ����
struct QPARAM_UPDATE_START_DECLARATION_OF_WAR
{
	BYTE			Influence;
	BYTE			MSWarStep;
	INT				NCP;
	INT				MSNum;
	MapIndex_t		MSAppearanceMap;
};

struct QPARAM_UPDATE_END_DECLARATION_OF_WAR
{
	BYTE			Influence;
	BYTE			MSWarStep;
	ATUM_DATE_TIME	MSWarEndTime;
	BYTE			MSWarEndState;
};

struct QPARAM_UPDATE_MSWAR_START_TIME
{
	BYTE			Influence;
	BYTE			MSWarStep;
	ATUM_DATE_TIME	MSWarStartTime;
	BYTE			SelectCount;
	BOOL			GiveUp;
};

///////////////////////////////////////////////////////////////////////////////
// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - 
#define	DB_THREAD_INDEX_0				0
#define	DB_THREAD_INDEX_1				1	// 2008-04-30 by cmkwon, QT_UpdateDBServerGroup, QT_CheckConnectableAccount
#define	DB_THREAD_INDEX_2				2
#define	DB_THREAD_INDEX_3				3
#define	DB_THREAD_INDEX_4				4
#define	DB_THREAD_INDEX_5				5
#define	DB_THREAD_INDEX_6				6
#define	DB_THREAD_INDEX_7				7
#define	DB_THREAD_INDEX_8				8
#define	DB_THREAD_INDEX_9				9

//////////////////////////////////////////////////////////////////////////
// 2008-04-24 by dhjin, EP3 ���� �ý��� - 
struct QPARAM_READ_LETTER
{
	UID64_t		LetterUID;
	UID32_t		RecvCharacterUID;
};

typedef QPARAM_READ_LETTER		QPARAM_DELETE_LETTER;
typedef QPARAM_DELETE_LETTER	QPARAM_DELETE_READALLLETTER;
typedef QPARAM_READ_LETTER		QPARAM_READ_ALLLETTER;

struct QPARAM_SEND_LETTER
{
	CHAR		RecvCharacterName[SIZE_MAX_CHARACTER_NAME];
	CHAR		Title[SIZE_MAX_LETTER_TITLE];
	CHAR		Content[SIZE_MAX_LETTER_CONTENT];
};

struct QPARAM_DELETE_ALL_LETTER
{
	UID64_t		AllLetterUID;
};

struct QPARAM_SEND_ALLLETTER
{
	CHAR		Title[SIZE_MAX_LETTER_TITLE];
	CHAR		Content[SIZE_MAX_LETTER_CONTENT];
};

////////////////////////////////////////////////////////////////////////////////
// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 
struct QPARAM_INSERT_INFINITY {
	UID32_t			AccountUID;
	UID32_t			CharacterUID;
	InfiModeUID_t	InfinityModeUID;
	ATUM_DATE_TIME	StartTime;
};
struct QPARAM_UPDATE_INFINITY {
	UID32_t			AccountUID;
	UID32_t			CharacterUID;
	InfiModeUID_t	InfinityModeUID;
};

struct QPARAM_CHARACTER_SAVE_DATA_INFINITY_FIN {
	UID32_t				CharacterUID;
	GEAR_STAT			GearStat;						// ��� ����
	BYTE				Level;							//
	Experience_t		Experience;						//
	Experience_t		DownExperience;					// ĳ���� ������ ������ ����ġ
	INT					DownSPIOnDeath;					// 2006-04-10 by cmkwon, ĳ���� ������ ������ SPI
	BYTE				BonusStat;						// �߰� 2002.12.13
	BYTE				BonusStatPoint;					// 2005-11-15 by cmkwon, �������� �ƴ� �ٸ�������� ���� ���ʽ� ���� ����Ʈ, BonusSkillPoint�� ������ // �߰� 2002.12.13
	LONGLONG			TotalPlayTime;					// �ʴ���
	ATUM_DATE_TIME		LevelUpTime;					// 2006-12-18 by dhjin, ������ �ð�
	INT					WarPoint;						// 2007-04-17 by dhjin, WP
	INT					CumulativeWarPoint;				// 2007-05-28 by dhjin, ����WP
	LONGLONG			PCBangTotalPlayTime;			// 2007-06-07 by dhjin, PC�� �� �÷��� �ð�, �ʴ���
	INT					SecretInfoOption;				// 2008-06-23 by dhjin, EP3 ���������ɼ� -
	MapIndex_t			InfinityMapIndex;				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��
	ChannelIndex_t		InfinityChannelIndex;			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��
};

struct QPARAM_INFINITY_FIN_UPDATE_ITEM {
	UID64_t			MainSvrItemUID;
	INT				ItemWindowIndex;
	INT				CurrentCount;
	BYTE			Wear;
	INT				UsingTimeStamp;
	INT				CoolingTimeStamp;					// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
};

struct QPARAM_INFINITY_INSERT_LOG {
	InfiModeUID_t	InfinityModeUID;
	DeathCnt_t		DeathCount;
	KillCnt_t		KillCount;
	ATUM_DATE_TIME	StartTime;
	ATUM_DATE_TIME	EndTime;
	Experience_t	AllEXP;								// 2010-06-25 by shcho, ���Ǵ�Ƽ ���÷α� ��� - ����� �ִ� ����ġ �߰�
};

struct QPARAM_INFINITY_RESET_IMPUTE_SERVERSTART {
	InfiModeUID_t	InfinityModeUID;
	ATUM_DATE_TIME	StartTime;
};

struct QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP {	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
	UID64_t		ItemUniqueNumber;
	INT			CoolingTimeStamp;
};

struct QPARAM_INFINITY_UPDATE_USER_MAP_INFO		// 2010-04-06 by cmkwon, ����2�� �߰� ���� - 
{
	UID32_t				CharacterUID;
	MapIndex_t			InfinityMapIndex;
	ChannelIndex_t		InfinityChannelIndex;
};

// 2010-06-25 by shcho, ���Ǵ�Ƽ ���÷α� ��� - ���� ������ ���� DB����
struct QPARAM_INFINITY_LOG_USER_GET_TENDERITEM_INFO // üũ
{
	INT			TenderItemNum;		// ȹ���� ������ �ѹ�
	DiceCnt_t	GDicecount;			// ȹ���� �ֻ��� ī��Ʈ
	UID64_t		AccountUID;			// ���� UID
	CHAR		AccountName[SIZE_MAX_ACCOUNT_NAME];	// ���� �̸�
	UID64_t		CharacterUID;						// �ɸ��� UID
	CHAR		CharacterName[SIZE_MAX_CHARACTER_NAME];	// �ɸ��� �̸�
};								   
// END 2010-06-25 by shcho, ���Ǵ�Ƽ ���÷α� ��� - ���� ������ ���� DB����

// Start 2011-08-25 by shcho, Ƚ���� ������ ���ޱ�� ���� - �α��� �̺�Ʈ ���� ����
struct QPARAM_ITEM_LOGIN_EVENT
{
	INT EventUniqueNumber;
	INT AccountUniqueNumber; 
	INT CompleteFlag;
};
// End 2011-08-25 by shcho, Ƚ���� ������ ���ޱ�� ���� - �α��� �̺�Ʈ ���� ����

/////////////////////////////////////////////////////////////
// start 2011-08-22 by hskim, ��Ʈ�� �ý��� 2�� - ��� ����

struct QPARAM_PET_UPDATE_INFINITY_FIN
{
	UID64_t			UniqueNumber;
	char			PetName[SIZE_MAX_PET_NAME];
	INT				PetLevel;
	Experience_t	PetExp;
	BYTE			PetExpRatio;
	BYTE			PetEnableSocketCount;

	SPET_KIT_SLOT_DATA	PetKitHP;
	SPET_KIT_SLOT_DATA	PetKitShield;
	SPET_KIT_SLOT_DATA	PetKitSP;

	SPET_AUTOSKILL_SLOT_DATA	PetAutoSkill;
};

struct QPARAM_PET_SET_NAME
{
	UID64_t			ItemUniqueNumber;
	char			PetName[SIZE_MAX_PET_NAME];
};

struct QPARAM_PET_SET_EXP_RATIO
{
	UID64_t			ItemUniqueNumber;
	BYTE			ExpRatio;
};

struct QPARAM_PET_SET_LEVEL
{
	UID64_t			ItemUniqueNumber;
	INT				Level;
};

struct QPARAM_PET_SET_EXP
{
	UID64_t			ItemUniqueNumber;
	Experience_t	Exp;
};

struct QPARAM_PET_SET_SOCKET
{
	UID64_t			ItemUniqueNumber;
	UID64_t			PetSocketItemUID[SIZE_MAX_PETSOCKET];
};

// 2015-06-22 Future, upgrading Pets with sockets
struct QPARAM_UPGRADE_PET
{
	UID64_t			OldItemUniqueNumber;
	UID32_t			UpgradedPetItemUID;
};

struct QPARAM_PET_SET_KIT_SLOT
{
	UID64_t				ItemUniqueNumber;
	SPET_KIT_SLOT_DATA	PetKitHP;
	SPET_KIT_SLOT_DATA	PetKitShield;
	SPET_KIT_SLOT_DATA	PetKitSP;
};

struct QPARAM_PET_SET_AUTOSKILL_SLOT
{
	UID64_t						ItemUniqueNumber;
	SPET_AUTOSKILL_SLOT_DATA	PetAutoSkill;
};

struct QPARAM_PET_CHANGE_SOCKET_OWNER
{
	UID64_t		ItemUniqueNumber;
	UID32_t		CharacterUID;
};

// end 2011-08-22 by hskim, ��Ʈ�� �ý��� 2�� - ��� ����
/////////////////////////////////////////////////////////////

// start 2012-01-30 by hskim, ��Ʈ�� �ý��� 2�� - ���� â�� �̵�
struct QPARAM_STORE_GET_ITEM_ONE
{
	UID64_t		ItemUniqueNumber;
	BOOL		bSendToClient;			// Client Inventory �� �������� ���� (���� Ȯ��� Type ������ �����ؼ� ó��)
};
// end 2012-01-30 by hskim, ��Ʈ�� �ý��� 2�� - ���� â�� �̵�

// start 2012-08-27 by hskim, ��Ʈ�� ��þƮ �� ���� �߰� ���� ó�� (�ڵ���� �������� �߰ߵ��� �ʾ����� ������ ��쿡�� �α׸� �����) (����ȭ DB ������ �õ��ؼ� ���� ���ϰ� ���� : �ʿ���ٰ� �ǴܵǸ� ���� ����)
struct QPARAM_GET_ENCHANT_COUNT
{
	UID64_t		ItemUniqueNumber;
	INT			EnchantCount;
};
// end 2012-08-27 by hskim, ��Ʈ�� ��þƮ �� ���� �߰� ���� ó�� (�ڵ���� �������� �߰ߵ��� �ʾ����� ������ ��쿡�� �α׸� �����) (����ȭ DB ������ �õ��ؼ� ���� ���ϰ� ���� : �ʿ���ٰ� �ǴܵǸ� ���� ����)

#if S_BONUSEXPSYSTEM_RENEWAL
// 2012-10-07 by hskim, �޽� ����ġ
struct QPARAM_ADDEXP_REST_EXPERIENCE_COUNT
{
	UID32_t		AccountUID;
	INT			RestExperienceCount;
};
// end 2012-10-07 by hskim, �޽� ����ġ
#endif // S_BONUSEXPSYSTEM_RENEWAL

#if S_BONUS_KILL_SYSTEM_RENEWAL
// 2015-06-11 Future, Kill Count System
struct QPARAM_ADDKILL_REST_KILL_COUNT
{
	UID32_t		AccountUID;
	INT			RestKillCount;
};
// end 2015-06-11 Future, Kill Count System
#endif // S_BONUS_KILL_SYSTEM_RENEWAL

// 2012-10-21 by jhseol, �Ʒ��� ���׼��� - �Ʒ��� ����� �ɸ��� ���� ����.
struct QPARAM_ARENA_CHARACTER_RESET
{
	UID32_t			AccountUID;
	UID32_t			CharacterUID;
	UID32_t			ArenaCharacterUID;
};
// end 2012-10-21 by jhseol, �Ʒ��� ���׼��� - �Ʒ��� ����� �ɸ��� ���� ����.

///////////////////////////////////////////////////////////////////////////////
// 2012-11-13 by jhseol, ���� �ý��� ������ - ������
struct QPARAM_STRATEGYPOINT_SUMMON_INFO
{
	MapIndex_t		MapIndex;
	BYTE			SummonCount;
	ATUM_DATE_TIME	SummonTime;
	BOOL			SummonAttribute;
};
// end 2012-11-13 by jhseol, ���� �ý��� ������ - ������

// 2013-05-20 by hskim, [���� �ý���] ���������� ������� ������ �߰� ����
struct QPARAM_TEMPORARY_SYSTEM_INFOMATION
{
	INT				Type;
	UID64_t			UID;
	ATUM_DATE_TIME	Date;
	UID64_t			Data1;
	INT				Data2;
	CHAR			Data3[SIZE_MAX_TEMP_SYSTEM_STRING];
	CHAR			Data4[SIZE_MAX_TEMP_SYSTEM_STRING];
};

struct QPARAM_GET_STORE_EXTENSION
{
	UID64_t			ItemUID;
	UID64_t			Gesture;
};

struct QPARAM_INSERT_STORE_EXTENSION
{
	UID64_t			ItemUID;
	UID64_t			Gesture;
};
// 2013-05-20 by hskim, [���� �ý���] ���������� ������� ������ �߰� ����

#ifdef S_IP_UNIQUE_CONNECTION
struct QPARAM_GET_CHARACTER_MULTIPLE_IP_STATUS
{
	char			AccountName[SIZE_MAX_ACCOUNT_NAME];
	UID32_t			AdminIssuerCharacterUID;
};
#endif // S_IP_UNIQUE_CONNECTION

struct QPARAM_NATION_RESET
{
	UID64_t			ItemUID;
	char			AccountName[SIZE_MAX_ACCOUNT_NAME];
	BYTE			Nation;
};


class CInflWarManager;
class CInflWarData;
class CGiveItemManager;
class CGuildItemManager;
class CArenaManager;
class CPCBangIPManager;
class COutPostManager;
class CCityLeader;
class CAtumFieldDBManager : public CAtumDBManager
{
public:
	CAtumFieldDBManager(CFieldIOCP *pFieldIOCP);

	void MakeAndEnqueueQuery(EnumQueryType type, CFieldIOCPSocket* pFieldIOCPSocket, UID32_t i_uidAccountUID, void *pMsg, void* i_pGeneralParam = NULL, INT64 i_nGeneralParam1 = 0, INT64 i_nGeneralParam2 = 0);	// Asynchronous Excution

	BOOL CheckClientState(CIOCPSocket *i_pSocket);	// DB Query Processing ������ Ŭ���̾�Ʈ�� ���°� �ٸ����� Ȯ��!
	BOOL ProcessServerQuery(DB_QUERY dbquery, SQLHSTMT &hstmt, SQLHSTMT &hstmt_mc, SQLHSTMT &hstmt_extAuth, SQLHSTMT &hstmt_GLog);		// 2013-06-20 by jhseol,bckim GLog ����

	static BOOL LoadInfluenceWarData(BYTE *o_pOwnerInflOfConflictArea, CODBCStatement *i_pODBCStmt, CInflWarManager *i_pInflWarManager);
	static BOOL InsertInfluenceWarData(CODBCStatement *i_pODBCStmt, SINFLUENCE_WAR_DATA *i_pSINFLUENCE_WAR_DATA);
	static BOOL LoadDeclarationOfWarInfo(CODBCStatement *i_pODBCStmt, CInflWarManager *i_pInflWarManager);			// 2009-01-12 by dhjin, ���� ����
	
	// 2007-04-25 by dhjin, ARENA ����
	static BOOL DBLoadArenaInfo(CODBCStatement *i_pODBCStmt, CArenaManager *i_pArenaManager);
	static BOOL DBLoadArenaMapInfo(CODBCStatement *i_pODBCStmt, CArenaManager *i_pArenaManager);

	// 2007-07-06 by dhjin, Tutorial
	static BOOL DBLoadTutorialInfo(CODBCStatement *i_pODBCStmt, vectSTutorialInfo *i_pvectTutorialInfo);			// 2007-07-06 by dhjin, Tutorial ���� ������

	static BOOL DBLoadInitializedGuildList(CODBCStatement *i_pODBCStmt);	// 2012-12-20 by bckim, �����ʱ�ȭ�� ���â��������� ����忡�� �Ű����� ������ �α�
	
	// 2007-08-16 by dhjin, �������� ����
	static BOOL DBLoadOutPostInfo(CODBCStatement *i_pODBCStmt, COutPostManager *i_pOutPostManager);					// 2007-08-16 by dhjin, �������� ���� ������
	static BOOL DBLoadOutPostNextWarTimeInfo(CODBCStatement *i_pODBCStmt, COutPostManager *i_pOutPostManager);					// 2007-08-21 by dhjin, �������� ���� �ð� ���� ������
	
	// 2007-08-21 by dhjin, CityLeader ����
	static BOOL DBLoadCityLeaderInfo(CODBCStatement *i_pODBCStmt, CCityLeader *i_pCityLeader);		// 2007-08-21 by dhjin, CityLeader ���� ������

	// 2008-11-10 by dhjin, ��Ű�ӽ�
	static BOOL DBLoadLuckyMachine(CODBCStatement *i_pODBCStmt, vectSLUCKY_MACHINE * o_pvectLuckyMachine);
	
	// 2007-10-29 by dhjin, ������ ���� ���� �ൿ��
	static BOOL DBLoadActionByLevel(CODBCStatement *i_pODBCStmt, vectSACTION_BY_LEVEL_DB * i_pvectSACTION_BY_LEVEL_DB);
	
	// 2007-10-29 by dhjin, ���� ����
	static BOOL	DBLoadPollDate(CODBCStatement *i_pODBCStmt, CInflWarManager *i_pInflWarManager);

	// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - 
	static BOOL DBLoadDBServerGroup(CODBCStatement *i_pODBCStmt, SDBSERVER_GROUP *io_pDBServerGroup);
	static BOOL DBUpdateDBServerGroup(CODBCStatement *i_pODBCStmt, SDBSERVER_GROUP *i_pDBServerGroup);

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 	
	static BOOL DBLoadCinemaInfo(CODBCStatement *i_pODBCStmt, vectorCinemaInfo * o_pvectCinemaInfo);	// 2009-09-09 ~ 2010-02-26 by dhjin, ���Ǵ�Ƽ - *.cfg���� �߰�
	static BOOL DBLoadRevisionInfo(CODBCStatement *i_pODBCStmt, vectorRevisionInfo * o_pvectRevisionInfo);
	static BOOL DBLoadInfinityMonsterInfo(CODBCStatement *i_pODBCStmt, vectorInfinityMonsterInfo * o_pvectInfinityMonsterInfo);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-12-01 by cmkwon, �������� �����带 ����� ó���ϴ� �ý��� ���� - 
	virtual BOOL ProcessDynamicServerQuery(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC);

	BOOL DQP_DailyJob(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC);		// 2008-12-01 by cmkwon, �������� �����带 ����� ó���ϴ� �ý��� ���� - 
	constexpr static bool DQP_UpdatePCBangList(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC) { return false; }		// 2008-12-19 by cmkwon, QT_UpdatePCBangList->DQT_UpdatePCBangList ���� - 
	constexpr static bool DQP_ReloadWRKServiceList(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC) { return false; }	// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 
	constexpr static bool DQP_ReloadWRKLevel(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC) { return false; }	// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 
	constexpr static bool DQP_ReloadWRKFame(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC) { return false; }	// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 
	constexpr static bool DQP_ReloadWRKPVP(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC) { return false; }		// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 
	BOOL DQP_LoadInfluenceRate(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC);					// 2009-09-16 by cmkwon, ���� �ʱ�ȭ�� ���¡ ���� ���� - 

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-09 by cmkwon, ����2�� �߰� ����(�ܰ躰 ���� �߰�) - 
	static BOOL DBLoad_TenderList(CODBCStatement *i_pODBCStmt, vectorTenderInfo *o_pTenderItemList);

	///////////////////////////////////////////////////////////////////////////////
	// // 2010-05-13 by shcho, ���Ǵ�Ƽ ���̵� ���� -
	static BOOL DBLoadInfinityDifficultInfo(CODBCStatement *i_pODBCStmt, vectorInfinity_DifficultyInfo_Bonus* o_vectorDifficultyInfo);
	static BOOL DBLoadInfinityDifficultMonsterInfo(CODBCStatement *i_pODBCStmt, vectorInfinity_DifficultyInfo_Monster* o_vectorDifficultyMonsterInfo);


private:

	BOOL GetAttachedItems( CHARACTER_RENDER_INFO *io_pRenderInfo, SQLHSTMT i_hstmt, UID32_t i_characterUID );
	// 2010-06-15 by shcho&hslee ��ý��� - ��ϵ� ������ ���� ���� �Լ� �߰� 		
	void SendItemData( CFieldIOCPSocket *pFieldIOCPSocket, BYTE ItemInsertionType, ITEM_GENERAL	*pItem, tPET_CURRENTINFO *pcurrentData = NULL );
	
	///////////////////////////////////////////////////////////////////////////
	// Query Processing Functions
	///////////////////////////////////////////////////////////////////////////
	void QP_CreateCharacter(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_DeleteCharacter(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_GetAccountUniqueNumber(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_GetCharacterInfoByName(FIELD_DB_QUERY q, SQLHSTMT hstmt);
// 2005-07-21 by cmkwon, �ٸ� �ʵ弭������ GameStart�� �����Ƿ� ������
//	void QP_ConnectGameStart(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_GetAllCharacterInfoByID(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_SaveCharacterCriticalData(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ChangeUnitKind(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ChangeStat(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ChangeBonusStatPoint(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ChangeGuild(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ChangeExp(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ChangeBodyCondition(FIELD_DB_QUERY q, SQLHSTMT hstmt);
//	void QP_ChangePropensity(FIELD_DB_QUERY q, SQLHSTMT hstmt);

	void QP_ChangeInfluenceType(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ChangeStatus(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ChangePKPoint(FIELD_DB_QUERY q, SQLHSTMT hstmt);
// 2009-11-02 by cmkwon, ĳ��(�κ�/â�� Ȯ��) ������ �߰� ���� - ������� �����Ƿ� ����
//	void QP_ChangeRacingPoint(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateTotalPlayTime(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateLastStartedTime(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ChangeHPDPSPEP(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ChangeCurrentHPDPSPEP(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ChangeMapChannel(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ChangePetInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ChangePosition(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ChangeLevel(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	// store(item) table ���� �Ŀ� ������
	//void QP_ChangeDockingItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);

	// For Event
// 2004-12-16 by cmkwon, �ٸ� �ʵ弭������ ������ �����Ƿ� ������
//	void QP_EventWarpConnect(FIELD_DB_QUERY q, SQLHSTMT hstmt);

	// For Item
	void QP_GetStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_NewGetStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateItemPossess(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateItemStorage(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_DeleteStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_InsertStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_InsertStoreItemSkill(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateStoreItemSkillUseTime(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2006-11-17 by dhjin
	void QP_UpdateItemCount(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateItemNum(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2006-06-14 by cmkwon
	void QP_UpdateEndurance(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_StoreUpdateColorCode(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateShapeItemNum(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 

	void QP_UpdateItemUsingTimeStamp(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateItemRareFix(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_InsertDefaultItems(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ChangeItemPosition(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateWindowItemList(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ReloadAllEnchant(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_LoadOneItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_InsertLoginItemEvent(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2011-08-25 by shcho, Ƚ���� ������ ���ޱ�� ����
	void QP_CheckEventItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_InsertEventItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateEventItemFixedPeriod(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2013-02-28 by bckim, �������� �����߰�
	void QP_CheckCouponEvent(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2008-01-10 by cmkwon, ������ �̺�Ʈ �ý��ۿ� �� ���� �ý��� �߰� - 
	void QP_GetGuildStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateGuildStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_InsertGuildStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_DeleteGuildStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_GetLogGuildStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_AllDeleteGuildStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_Insert2WarpableUserList(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2007-08-30 by cmkwon, ȸ�Ƿ� �ý��� ���� - �����㰡 �߰�
	void QP_DeleteWarpableUser(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-08-30 by cmkwon, ȸ�Ƿ� �ý��� ���� - �����㰡 ����
	//void QP_UPDATE_ConnectingServerGroupID(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2007-11-06 by cmkwon, ���� �α� DB ���� ���� �����ϱ� - �߰� ��
	void QP_GiveStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2007-11-13 by cmkwon, �����ϱ� ��� �߰� - 
	void QP_GetLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2008-04-24 by dhjin, EP3 ���� �ý��� - DB���� ���� ��������
	void QP_ReadLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2008-04-24 by dhjin, EP3 ���� �ý��� - ���� �б�
	void QP_DeleteLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2008-04-24 by dhjin, EP3 ���� �ý��� - ���� ����
	void QP_SendLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2008-05-08 by dhjin, EP3 ���� �ý��� - ���� ������
	void QP_GetAllLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2008-05-09 by dhjin, EP3 ���� �ý��� - DB���� ��ü ���� ��������
	void QP_SendAllLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2008-05-09 by dhjin, EP3 ���� �ý��� - ��ü ���� ������
	void QP_ReadAllLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2008-05-09 by dhjin, EP3 ���� �ý��� - ��ü ���� �б�
	void QP_DeleteAllLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2008-05-09 by dhjin, EP3 ���� �ý��� - ��ü ���� ����
	void QP_DeleteOldAllLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2008-05-09 by dhjin, EP3 ���� �ý��� - ������ ��ü ���� ����

	// For Enchant
	void QP_InsertEnchant(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_DeleteAllEnchant(FIELD_DB_QUERY q, SQLHSTMT hstmt);

	// For Quest
// 2007-12-21 by dhjin, ���� ���� ��ƾ DB�� ������ Client���� �κ� �и� - �ؿ� �Լ��� ����
//	void QP_GetAllQuest(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// ������� �������̰ų� �Ϸ�� ��� quest�� loading
	void GetAllQuest(CFieldIOCPSocket *pIOCPSocket, SQLHSTMT hstmt);		// ������� �������̰ų� �Ϸ�� ��� quest�� loading
	void QP_InsertQuest(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// ������� quest�� ����
	void QP_UpdateQuestState(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// Quest ���� ���� ������Ʈ
	void QP_DeleteQuest(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// Quest �����
	void QP_UpdateQuestMonsterCount(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// ���� ī��Ʈ �߰� �� ����
	void QP_DeleteQuestMonsterCount(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// ���� ī��Ʈ �����

	// For Auction
	void QP_AuctionGetItemList(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// ��� ������ ��� ��û
	void QP_AuctionRegisterItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// ��� ������ ���
	void QP_AuctionCancelRegister(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// ��� ������ ��� ���
	void QP_AuctionBuyItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// ��� ������ ����

	// For Trade
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) - ������� �ʴ� �Լ�
	//void QP_ExecuteTrade(FIELD_DB_QUERY q, SQLHSTMT hstmt_mc);

	void QP_TradeMoveItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);

	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) - ������� �ʴ� �Լ�
	//BOOL MoveTradeItemDB(CFieldIOCPSocket *pFromSocket, CFieldIOCPSocket *pToSocket, SQLHSTMT hstmt_mc, QPARAM_EXECUTETRADE *&pParam, FIELD_DB_QUERY q);
	//BOOL MoveTradeItemField(CFieldIOCPSocket *pFromSocket, CFieldIOCPSocket *pToSocket, QPARAM_EXECUTETRADE *&pParam);

	// General Execution
	void QP_GeneralExec(FIELD_DB_QUERY q, SQLHSTMT hstmt);

	// ����ȭ �����̾� ī��
	void QP_CashInsertPremiumCard(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_CashUpdatePremiumCard(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_CashDeletePremiumCard(FIELD_DB_QUERY q, SQLHSTMT hstmt);

	// Happy Hour Evnet
	void QP_LoadHappyHourEvent(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateHappyHourEventDate(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateInfluenceWarData(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateOwnerOfConflictArea(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateSubleader(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2007-02-13 by dhjin, ��������

	//void QP_JamboreeInit(FIELD_DB_QUERY q, SQLHSTMT hstmt);					// 2007-04-10 by cmkwon

	// 2006-08-31 by dhjin,  ItemEvent
	void QP_LoadItemEvent(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	//void QP_UpdatePCBangList(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2007-01-22 by dhjin, PCBang

	// 2006-09-04 by dhjin, �� ����
	void QP_Get_QuickSlot(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_Delete_QuickSlot(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_Update_QuickSlot(FIELD_DB_QUERY q, SQLHSTMT hstmt);

	// 2007-02-28 by dhjin, ��������Ʈ �� �ֱ⿡ ���� ����
	void QP_LoadStrategyPointSummonInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateStrategyPointSummonInfoBySummon(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateStrategyPointSummonInfoBySummonTime(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateStrategyPointSummonInfoByNewSummonRange(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_LoadStrategyPointSummonRange(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_LoadStrategyPointNotSummonTime(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_LoadStrategyPointNotSummonTimeByAdminTool(FIELD_DB_QUERY q, SQLHSTMT hstmt);

	//////////////////////////////////////////////////////////////////////////
	// 2007-06-07 by dhjin, �Ʒ��� ���� 
	void QP_UpdateWarPoint(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-04-25 by dhjin, WarPoint����
	void QP_UpdateArenaResult(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2007-06-07 by dhjin, �Ʒ��� ��� ������Ʈ
	void QP_UpdateArenaDisConnect(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2007-06-07 by dhjin, �Ʒ��� �������� ������Ʈ
	void QP_MF_Updata_CharacterArena(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : AS���� ���� CharacterArena ���� ������Ʈ
	

	//////////////////////////////////////////////////////////////////////////
	// 2007-07-06 by dhjin, Tutorial
	void QP_LoadTutorialComplete(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-07-06 by dhjin, ������ Tutorial ���� ������
	void QP_InsertTutorialComplete(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-07-06 by dhjin, ������ Tutorial �Ϸ� ���� ����

	//////////////////////////////////////////////////////////////////////////
	// 2007-08-21 by dhjin, OutPost
	void QP_SetNPCPossessionOutPost(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-08-21 by dhjin, �������� NPC������ ����
	void QP_SetGuildPossessionOutPost(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-08-21 by dhjin, �������� ���� ������ ����
	void QP_SetOutPostNextWarTime(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-08-21 by dhjin, ���� ���������� �ð� ����
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-08-22 by dhjin, CityLeader
	void QP_InitExpediencyFund(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-08-22 by dhjin, �ǰ��� ���� �߰�
	void QP_SetExpediencyFund(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-08-22 by dhjin, �ǰ��� ���� ����
	void QP_SetExpediencyFundPayBack(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2007-08-22 by dhjin, �ǰ��� ȯ�� ����
	void QP_SetExpediencyFundRate(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2007-08-22 by dhjin, �ǰ����� ���� ����
	void QP_DeleteCityLeaderInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-08-22 by dhjin, ���� ���� ����
	void QP_RegNotice(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-08-22 by dhjin, �������� ���
	void QP_ModifyNotice(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-08-22 by dhjin, �������� ����.����
	void QP_UpdateSecondaryPassword(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2007-09-12 by cmkwon, ��Ʈ�� 2���н����� ���� - 2���н����� ������Ʈ ���ν��� ȣ�� �Լ�

	//////////////////////////////////////////////////////////////////////////
	// 2007-10-29 by dhjin, poll
	void QP_LoadVoterList(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2007-10-29 by dhjin, ��ǥ�� ����Ʈ �ε�
	void QP_LoadLeaderCandidate(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2007-10-29 by dhjin, ������ �ĺ� ����Ʈ �ε�
	void QP_SelectLeaderCandidateInfoByRealTimeVariable(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2007-10-30 by dhjin, ������ �ĺ� �ǽð����� ���ϴ� ���� DB���� ������ �������� �����Ѵ�.
	void QP_InsertLeaderCandidate(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2007-10-30 by dhjin, ������ �ĺ� ���
	void QP_UpdateLeaderDeleteCandidate(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2007-10-30 by dhjin, ������ �ĺ� Ż��
	void QP_UpdateLeaderPollCount(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2007-10-31 by dhjin, ��ǥ�� ȹ�� ǥ ������Ʈ
	void QP_InsertVoterList(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-10-31 by dhjin, ��ǥ�� ��� ������Ʈ

	void QP_CheckGiveTarget(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-11-13 by cmkwon, �����ϱ� ��� �߰� -
	void QP_UpdatePilotFace(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-11-21 by cmkwon, PilotFace ���� ī�� ���� - 

	void QP_InsertNotifyMsg(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-11-28 by cmkwon, �����ý��� ���� -
	void QP_GetNotifyMsg(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-11-28 by cmkwon, �����ý��� ���� -
	void QP_DeleteNotifyMsg(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-11-28 by cmkwon, �����ý��� ���� -

	void QP_GetGuildMark(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-12-07 by dhjin, ���� ��ũ ������

	//////////////////////////////////////////////////////////////////////////
	// 2007-12-28 by dhjin, �Ʒ��� ���� - 
	void QP_ArenaUpdateCharacterInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ArenaGetCharacter(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ArenaCopyDBInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ArenaStartGetCharacter(FIELD_DB_QUERY q, SQLHSTMT hstmt);

	//////////////////////////////////////////////////////////////////////////
	// 2008-04-02 by dhjin,	����, ������ ����â ��ȹ�� -
	void QP_GetLogMSWarInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_GetLogSPWarInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt);
#ifdef NEMERIAN_NATION_BALANCE_BUFF_PENALITY
	void QP_GetWarVictoryDiff(FIELD_DB_QUERY q, SQLHSTMT hstmt);
#endif
	void QP_UpdateMSWarOptionType(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_InsertMSWarLog(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2008-08-28 by dhjin, ���� ����, ����DB�� ���ܾ� Admintool�� �ʱ�ȭ�� �����ϴ�.
	void QP_InsertSPWarLog(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2008-08-28 by dhjin, ���� ����, ����DB�� ���ܾ� Admintool�� �ʱ�ȭ�� �����ϴ�.

	///////////////////////////////////////////////////////////////////////////////
	// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - 
	void QP_UpdateDBServerGroup(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_CheckConnectableAccount(FIELD_DB_QUERY q, SQLHSTMT hstmt);

	void QP_GetUserInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt);					// 2008-06-23 by dhjin, EP3 ���������ɼ� - �ٸ� ���� ���� ��û

	//////////////////////////////////////////////////////////////////////////
	// 2008-08-19 by dhjin, MySQL���� ������ MySQL���� �������� �ʴ� MSSQL�� Job���� �۾��� ���⼭ ó���Ѵ�.
	void QP_DailyJob(FIELD_DB_QUERY q, SQLHSTMT hstmt);

	// 2008-11-04 by dhjin, ��Ű�ӽ�
	void QP_UpdateLuckyItemDropCount(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateLuckyItemStarttime(FIELD_DB_QUERY q, SQLHSTMT hstmt);

	// 2009-01-12 by dhjin, ���� ����
	void QP_UpdateStartDeclarationOfWar(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateEndDeclarationOfWar(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateMSWarStartTime(FIELD_DB_QUERY q, SQLHSTMT hstmt);

	void QP_UpdateNickName(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 
	void QP_GetSelfRanking(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 

	void QP_ChangeItemWithItemMatching(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2009-03-31 by cmkwon, �����ʱ�ȭ �ý��� ���� - 

	void QP_ChangeStartCityMapIndex(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2009-10-12 by cmkwon, ������ī ���� ��� ���� - 
	void QP_ChangeAddedInventoryCount(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2009-11-02 by cmkwon, ĳ��(�κ�/â�� Ȯ��) ������ �߰� ���� - 

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 
	void QP_LoadInfinityImpute(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 	���� �ͼ� ���� ��������
	void QP_InsertInfinityImpute(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 	���� �ͼ� ���� �߰�
	void QP_UpdateInfinityImpute(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 	���� �Ϸ�
	void QP_ResetInfinityImpute(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 	�ش� ���� �������� ���� ����
	void QP_ArenaCopyInfinityDBInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �Ʒ���DB�� �����ϱ�
	void QP_CharacterSaveDataInfinityFin(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� �� MainSvr�� ���� �����ϸ鼭 ������ �ɸ��� ���� ����
	void QP_InfinityFinUpdateItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� �� MainSvr�� ���� �����ϸ鼭 ������ ������ ���� ����
	void QP_InfinityFinInsertItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� �� MainSvr�� ���� �����ϸ鼭 ������ ������ �߰�
	void QP_InfinityInsertLog(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� �α�
	void QP_CharacterSaveDataInfinityFinByDisconnect(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� ���� �� MainSvr�� ���� �����ϸ鼭 ������ �ɸ��� ���� ����
	void QP_InfinityFinUpdateItemByDisconnect(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� ���� �� MainSvr�� ���� �����ϸ鼭 ������ ������ ���� ����
	void QP_InfinityFinInsertItemByDisconnect(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� ���� �� MainSvr�� ���� �����ϸ鼭 ������ ������ �߰���
	void QP_ResetInfinityImputeByServerStart(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���۽� ���� ����
	void QP_InfinityComBackPostWork(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2012-01-16 by hskim, ��� - ȭ��

// 2010-04-09 by cmkwon, ����2�� �߰� ����(�ܰ躰 ���� �߰�) - 
//	void QP_LoadTenderInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt);					// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - Tender

	void QP_CashLoadPremiumCard(FIELD_DB_QUERY q, SQLHSTMT hstmt);					// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �����̾� ���� ��������
	void QP_LoadInfinityShopInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ����
	void QP_UpdateItemCoolingTimeStamp(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
	void QP_InfinityUpdateUserMapInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2010-04-06 by cmkwon, ����2�� �߰� ���� - 
	void QP_LoadBurningMap(FIELD_DB_QUERY q, SQLHSTMT hstmt);					// 2010-08-05 by dhjin, ���׸� -
	void QP_Log_UserGetTenderItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2010-06-25 by shcho, ���Ǵ�Ƽ ���÷α� ��� - ���� ������ ���� DB����	

	/////////////////////////////////////////////////////////////
	// start 2011-08-22 by hskim, ��Ʈ�� �ý��� 2�� - ��� ����

	void QP_PetUpdateInfinityFin(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_PetSetName(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void OP_PetSetExpRatio(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_PetSetLevel(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void OP_PetSetExp(FIELD_DB_QUERY q, SQLHSTMT hstmt);

	void OP_PetSetSocket(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void OP_PetSetKitSlot(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void OP_PetSetAutoSkillSlot(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void OP_PetChangeSocketOwner(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2012-01-30 by hskim, ��Ʈ�� �ý��� 2�� - ���� â�� �̵�
	void OP_StoreGetItemOne(FIELD_DB_QUERY q, SQLHSTMT hstmt);					// 2012-01-30 by hskim, ��Ʈ�� �ý��� 2�� - ���� â�� �̵�

	// end 2011-08-22 by hskim, ��Ʈ�� �ý��� 2�� - ��� ����
	/////////////////////////////////////////////////////////////

	void OP_UpgradPet(FIELD_DB_QUERY q, SQLHSTMT hstmt);						// 2015-06-22 Future, upgrading of Pets with sockets

	/////////////////////////////////////////////////////////////
	// 2012-04-12 by jhseol, �Ʒ��� �߰����� - �Ʒ��� �÷��� ī��Ʈ Ȯ�� �Լ� ���� 
	void QP_ArenaPlayCount(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	// end 2012-04-12 by jhseol, �Ʒ��� �߰����� - �Ʒ��� �÷��� ī��Ʈ Ȯ�� �Լ� ���� 

	// start 2012-08-27 by hskim, ��Ʈ�� ��þƮ �� ���� �߰� ���� ó�� (�ڵ���� �������� �߰ߵ��� �ʾ����� ������ ��쿡�� �α׸� �����) (����ȭ DB ������ �õ��ؼ� ���� ���ϰ� ���� : �ʿ���ٰ� �ǴܵǸ� ���� ����)
	void QP_GetEnchantCount(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	// end 2012-08-27 by hskim, ��Ʈ�� ��þƮ �� ���� �߰� ���� ó�� (�ڵ���� �������� �߰ߵ��� �ʾ����� ������ ��쿡�� �α׸� �����) (����ȭ DB ������ �õ��ؼ� ���� ���ϰ� ���� : �ʿ���ٰ� �ǴܵǸ� ���� ����)

#if S_BONUSEXPSYSTEM_RENEWAL
	// 2012-10-07 by hskim, �޽� ����ġ
	void QP_GetAddExpRestExperienceCount(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_SetAddExpRestExperienceCount(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	// end 2012-10-07 by hskim, �޽� ����ġ
#endif // S_BONUS_KILL_SYSTEM_RENEWAL

//#if S_BONUS_KILL_SYSTEM_RENEWAL
//	// 2015-06-11 Future, Kill Count System
//	void QP_GetAddKillRestKillCount(FIELD_DB_QUERY q, SQLHSTMT hstmt);
//	void QP_SetAddKillRestKillCount(FIELD_DB_QUERY q, SQLHSTMT hstmt);
//	// end 2015-06-11 Future, Kill Count System
//#endif // S_BONUS_KILL_SYSTEM_RENEWAL

	// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)
	void QP_GetFixedTerm(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_InsertFixedTerm(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_DeleteFixedTerm(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	// end 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)

	void CAtumFieldDBManager::QP_GetShapeStatLevel(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - QP_GetShapeStatLevel �߰�
	void CAtumFieldDBManager::QP_InsertShapeStatLevel(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - QP_InsertShapeStatLevel �߰�

	void QP_ArenaCharacterReset(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2012-10-21 by jhseol, �Ʒ��� ���׼��� - �Ʒ��� ����� �ɸ��� ���� ����.

	/////////////////////////////////////////////////////////////
	// 2012-11-13 by jhseol, ���� �ý��� ������ - ������
	void QP_LoadRenewalStrategyPointSummonInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void m_functionLoadRenewalStrategyPointSummonTimeByDB(SQLHSTMT hstmt);
	void m_functionLoadRenewalStrategyPointSummonMapInfoByDB(SQLHSTMT hstmt);
	void m_functionLoadRenewalStrategyPointSummonSettingInfoByDB(SQLHSTMT hstmt);
	void QP_UpdateStrategyPointSummonInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	// end 2012-11-13 by jhseol, ���� �ý��� ������ - ������

	// 2012-12-18 by jhseol, �Ʒ��� ĳ���� ���� �� ĳ���� ���� �ٽ� �ε��ϱ�.
	void QP_ArenaCharacterReload(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	BOOL m_functionArenaCharacterLoad(CFieldIOCPSocket *i_pFISock, SQLHSTMT hstmt);	// �Ʒ��� �������� �ɸ��� ������ �о���� �Լ�. ���� ȣ��Ǵ� ����̱⿡ �Լ��� ������
	// end 2012-12-18 by jhseol, �Ʒ��� ĳ���� ���� �� ĳ���� ���� �ٽ� �ε��ϱ�.


	void QP_LoadCashBuyDate(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2013-03-29 by jhseol, ������ �̺�Ʈ - �ſ� ù ����� ��������
	void QP_InsertCashBuyDate(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2013-03-29 by jhseol, ������ �̺�Ʈ - �ſ� ù ����� ��������

	void QP_LoadMonthlyArmorEvent(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2013-04-18 by jhseol,bckim �̴��� �Ƹ� - �̴��� �Ƹ� �̺�Ʈ �ε�
	void QP_UpdateInfluenceConsecutiveVictorites(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2013-05-09 by hskim, ���� ����Ʈ ����

	void QP_GetTemporarySystemInfomation(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2013-05-20 by hskim, [���� �ý���] ���������� ������� ������ �߰� ����
	void QP_GetStoreExtension(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2013-05-20 by hskim, [���� �ý���] ���������� ������� ������ �߰� ����
	void QP_InsertStoreExtension(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2013-05-20 by hskim, [���� �ý���] ���������� ������� ������ �߰� ����

	void QP_CollectionArmorListLoad(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - �Ƹ� �÷��� �ε�
	void QP_CollectionArmorUpdate(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - �Ƹ� �÷��� ����

	void QP_AccountInflChange(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2013-07-26 by jhseol, Ÿ ���� ���º���

#ifdef S_IP_UNIQUE_CONNECTION
	void QP_GetCharacterMultipleIPStatus(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2015-11-24 Future, Multiple IP Restriction
#endif // S_IP_UNIQUE_CONNECTION

	void QP_NationTransfer(FIELD_DB_QUERY dbquery, SQLHSTMT hstmt);				// 2015-12-17 Future, Nation Change Card

	void ProcessLogMessagesField(SQLSMALLINT plm_handle_type, SQLHANDLE &plm_handle, char *logstring, int ConnInd, CFieldIOCPSocket *pIOCPSocket, FIELD_DB_QUERY q);
public:
	CFieldIOCP	*m_pFieldIOCP8;
};

#endif
