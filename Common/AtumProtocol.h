#pragma once

/******************************************************************************
ATUM C/S Protocol ����

* protocol header ( size of data + ��ȣȭ ���� + seq. #), ������ 4 bytes
----------------------------------------------------
| size(2B) | encode_flag(1bit)+N(7bit)| seq. # (1B)|
----------------------------------------------------

* ��ȣȭ�� �������� ��� (�ڼ��� ������ �� ���� �Ʒ� �κ� ����)
<------------------- 4B --------------->
-----------------------------------------------------------------------
| size | encode_flag +   N    | seq. # | MSGs ... | dummy  | checksum |
| (2B) |   (1bit)    + (7bit) |  (1B)  |          | (0~3B) |   (1B)   |
-----------------------------------------------------------------------
<- XOR -><----------- XOR -------------->

* ��ȣȭ���� ���� �������� ���
---------------------------------------------------
| size | encode_flag +   N    | seq. # | MSGs ... |
| (2B) |   (1bit)    + (7bit) |  (1B)  |          |
---------------------------------------------------
<----------- 0x0000 ------------>

* message header
-------------------------
| Type1(1B) | Type2(1B) |
-------------------------

�ۼ���: ������(ywlee@webcallworld.com)
�ۼ���: 2002. 9. 23.

******************************************************************************/

#include "DefineGlobal.h"
#include "SocketHeader.h"
#include "AtumParam.h"
#include "ThreadCheck.h"

#if defined(_ATUM_CLIENT)
#include "HShield.h"
#else
#include "Security/AntiCpXSvr.h"
#endif

#include "MessageTypes.h"

constexpr auto PRE_SERVER_PORT = 15100;
constexpr auto IM_SERVER_PORT = 15101;
constexpr auto LOG_SERVER_PORT = 15102;
constexpr auto FIELD_SERVER_PORT = 15103;
constexpr auto NPC_SERVER_PORT = 15104;

const char* GetProtocolTypeString(MessageType_t msgType);
const char* GetGameLogTypeString(T1<T0::FL_LOG> msgType);
const char* GetItemUpdateTypeString(BYTE i_byItemUpdateTy);				// 2006-10-27 by cmkwon
const char* GetItemDeleteTypeString(BYTE i_byItemDeleteTy);				// 2008-01-23 by cmkwon, S_F, S_L: ����/�������� ���� �α׿� �߰� - GetItemDeleteTypeString() �߰�

void PrintExchangeMsg(BYTE SendOrRecv, MessageType_t nType, char *peerIP, ENServerType st = ST_INVALID_TYPE, BYTE printLevel = PRINTLEVEL_NO_MSG);

struct MSG_FP_CASH_CHANGE_CHARACTERNAME
{
	char		szAccName[SIZE_MAX_ACCOUNT_NAME];
	char		szChangedCharName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_FP_ADMIN_BLOCKACCOUNT		// 2008-01-31 by cmkwon, ���� ��/���� ��ɾ�� ������ �ý��� ���� - 
{
	SBLOCKED_ACCOUNT_INFO blockAccInfo;
};

struct MSG_FP_ADMIN_BLOCKACCOUNT_OK		// 2008-01-31 by cmkwon, ���� ��/���� ��ɾ�� ������ �ý��� ���� - 
{
	Err_t	ErrCode;		// ERR_NO_ERROR �̸� ����
	INT		AdminFieldServerClientIndex;		// 2008-01-31 by cmkwon, ���� ��/���� ��ɾ�� ������ �ý��� ���� - PreServer<->FieldServer ������ �����
	char	AdminAccName[SIZE_MAX_ACCOUNT_NAME];
	char	BlockedAccName[SIZE_MAX_ACCOUNT_NAME];
	ATUM_DATE_TIME	atimeEndTime;				// �� ���� �ð�
};

struct MSG_FP_ADMIN_UNBLOCKACCOUNT		// 2008-01-31 by cmkwon, ���� ��/���� ��ɾ�� ������ �ý��� ���� - 
{
	SBLOCKED_ACCOUNT_INFO blockAccInfo;
};

struct MSG_FP_ADMIN_UNBLOCKACCOUNT_OK	// 2008-01-31 by cmkwon, ���� ��/���� ��ɾ�� ������ �ý��� ���� - 
{
	Err_t	ErrCode;		// ERR_NO_ERROR �̸� ����
	INT		AdminFieldServerClientIndex;		// 2008-01-31 by cmkwon, ���� ��/���� ��ɾ�� ������ �ý��� ���� - PreServer<->FieldServer ������ �����
	char	AdminAccName[SIZE_MAX_ACCOUNT_NAME];
	char	UnblockedAccName[SIZE_MAX_ACCOUNT_NAME];
};

struct MSG_FC_CITYWAR_START_WAR
{
	MapIndex_t			CityWarMapIndex3;
	UID32_t				OccupyGuildUID3;
	char				szOccupyGuildName3[SIZE_MAX_GUILD_NAME];
	ATUM_DATE_TIME		atimeCityWarEndTime;
};

struct MSG_FC_CITYWAR_MONSTER_DEAD
{
	MapIndex_t			CityWarMapIndex3;
	UID32_t				OccupyGuildUID3;
	char				szOccupyGuildName3[SIZE_MAX_GUILD_NAME];
};

struct MSG_FC_CITYWAR_END_WAR
{
	MapIndex_t			CityWarMapIndex3;
	UID32_t				OccupyGuildUID3;
	char				szOccupyGuildName3[SIZE_MAX_GUILD_NAME];
	ATUM_DATE_TIME		atimeNextCityWarDefaultTime;
};

struct MSG_FC_CITYWAR_GET_OCCUPYINFO_OK
{
	MapIndex_t			CurrentMapIndex3;			// ���� MapIndex 
	MapIndex_t			CityWarMapIndex3;			// ���������� MapIndex
	MapIndex_t			CityWarCityMapIndex3;		// ���������� ���� MapIndex
	SCITY_OCCUPY_INFO	CityWarOccupyInfo3;			// ���������� ���� ����
};

struct MSG_FC_CITYWAR_SET_SETTINGTIME
{
	ATUM_DATE_TIME		atimeSetCityWarSettingTime;
};

struct MSG_FC_CITYWAR_SET_TEX
{
	float				fSetTex;
};

struct MSG_FC_CITYWAR_SET_BRIEFING
{
	char				szSetBriefing[SIZE_MAX_CITY_BRIEFING_LENGTH];
};

struct MSG_FC_CITYWAR_BRING_SUMOFTEX_OK
{
	int					nBroughtSumOfTex;
};

struct MSG_FC_WAR_NOTIFY_INVASION					// 2005-12-27 by cmkwon
{
	MAP_CHANNEL_INDEX	MapChannelIndex0;		// Ÿ ������ ħ���� MapChannelIndex
};

struct MSG_FC_WAR_NOTIFY_INFLUENCE_MONSTER_DEAD		// 2005-12-27 by cmkwon
{
	INT					MonsterUnitkind;		// ���� ����
	UID32_t				uidBestGuildUID;						// 2007-08-23 by cmkwon, �� ���Ľ� �ְ� ������ �� ���ܸ� ǥ�� - GuildUID
	char				szBestGuildName[SIZE_MAX_GUILD_NAME];	// 2007-08-23 by cmkwon, �� ���Ľ� �ְ� ������ �� ���ܸ� ǥ�� - GuildName 
};

struct MSG_FC_WAR_NOTIFY_INFLUENCE_MONSTER_INVASION		// 2006-01-20 by cmkwon
{
	INT					MonsterUnitkind;		// ���� �޴� ����
};

struct MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA				// 2006-04-14 by cmkwon
{
	INT					SummonMonsterUnitkind;		// ��ȯ �� MonsterUnitKind
	INT					RemainMinute;				// ���Ͱ� ��ȯ�Ǳ� ���� ���� �ð�(����:��)
	INT					ContributionPoint;			// 2008-04-01 by dhjin, ����, ������ ����â ��ȹ�� - 
	ATUM_DATE_TIME		SummonMonsterTime;			// 2007-02-06 by dhjin, ����(����) ���� ��ȯ�� �ð� 
	BYTE				BossStep;					// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - �� �ܰ�
	BYTE				BeforeWinCheck;				// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - �� �ܰ� ���� ����
};

struct MSG_FC_WAR_JACO_MONSTER_SUMMON				// 2006-04-19 by cmkwon
{
	int		nBelligerence0;
};

struct MSG_FC_WAR_STRATEGYPOINT_MONSTER_SUMMON
{// 2007-07-18 by dhjin,
	MapIndex_t		MapIndex;
	BYTE			InfluenceType;
	CHAR			MapName[SIZE_MAX_MAP_NAME];
	ATUM_DATE_TIME	SummonStrategyPointTime;
	FLOAT			HPRate;
};

struct MSG_FC_INFO_STRATEGICPOINTINFO_DISPLAY_LIST
{
	INT				StrategicPointInfoDisplayListCount;
	_ARRAY(SSTRATEGYPOINT_DISPLAY_INFO);
};

struct MSG_FC_INFO_STRATEGICPOINTINFO_DISPLAY : public SSTRATEGYPOINT_DISPLAY_INFO { };

struct MSG_SIGN_BOARD_STRING
{
	BYTE			InfluenceMask0;									// 2006-04-17 by cmkwon, ���� ����ũ
	bool			IsInfluenceLeader;								// 2006-04-17 by cmkwon, TRUE:����������, FALSE:��� Ȥ�� ������
	INT				StringIndex;									// ������ ��Ʈ�� �ε���
	ATUM_DATE_TIME	SignBoardExprieATime;							// ������ ��Ʈ�� ���� �ð�
	char			SingBoardString[SIZE_MAX_SIGN_BOARD_STRING];	// ������ ��Ʈ��
};

struct MSG_FC_WAR_SIGN_BOARD_INSERT_STRING				// 2006-04-17 by cmkwon
{
	bool			IsInfluenceLeader;								// 2006-04-17 by cmkwon, TRUE:����������, FALSE:��� Ȥ�� ������
	INT				StringIndex;									// ������ ��Ʈ�� �ε���
	ATUM_DATE_TIME	SignBoardExpiryTime;							// ������ ��Ʈ�� ���� �ð�
	char			SingBoardString[SIZE_MAX_SIGN_BOARD_STRING];	// ������ ��Ʈ��
};

struct MSG_FC_WAR_SIGN_BOARD_DELETE_STRING				// 2006-04-18 by cmkwon
{
	INT				DeleteStringIndex;						// ������ ��Ʈ�� �ε���
};

struct MSG_FC_WAR_REQ_SIGN_BOARD_STRING_LIST				// 2006-04-17 by cmkwon
{
	INT		nReqStringCount;									// ������ ��Ʈ�� ����
};

struct MSG_FC_WAR_REQ_SIGN_BOARD_STRING_LIST_OK				// 2006-04-17 by cmkwon
{
	INT		nStringCount;									// ������ ��Ʈ�� ����
	ARRAY_(MSG_FC_WAR_SIGN_BOARD_INSERT_STRING);			// ������ ����
};

struct MSG_FC_WAR_UPDATE_CONTRIBUTION_POINT_OK				// 2006-04-19 by cmkwon
{
	BYTE	byInfluenceType;
	int		nContributionPoint;
};

struct MSG_FC_WAR_INFLUENCE_DATA							// 2006-04-21 by cmkwon
{
	BYTE	byInfluenceType;
	float	fHPRepairRate;
	float	fDPRepairRate;
	float	fSPRepairRate;
};

struct MSG_FC_WAR_MONSTER_CREATED			// 2006-11-20 by cmkwon
{
	INT					MonsterUnitKind;
	MAP_CHANNEL_INDEX	MapChannIdx;
	ATUM_DATE_TIME		CreateTime;			// 2007-07-16 by dhjin, ���� �ð� �߰�
};

struct MSG_FC_WAR_MONSTER_AUTO_DESTROYED	// 2006-11-20 by cmkwon
{
	INT					MonsterUnitKind;
	MAP_CHANNEL_INDEX	MapChannIdx;
};

struct MSG_FC_WAR_MONSTER_DEAD				// 2006-11-20 by cmkwon
{
	INT					MonsterUnitKind;
	MAP_CHANNEL_INDEX	MapChannIdx;
};

struct SCONTRIBUTION_GUILD_INFO
{
	BYTE		order;
	UID32_t		GuildUID;
	char		GuildName[SIZE_MAX_GUILD_NAME];
	INT			GuildPay;
};

struct MSG_FC_WAR_BOSS_CONTRIBUTION_GUILD
{// 2008-12-29 by dhjin, ���� ���� �߰���
	SCONTRIBUTION_GUILD_INFO   ContributionGuldInfo[3];
};

struct MSG_FC_WAR_INFLUENCE_CONSECUTIVE_VICTORITES
{
	INT PointBCU;
	INT PointANI;
};

struct MSG_FC_WAR_INFLUENCE_CONSECUTIVE_POINT
{
	float PVPBuffPercent;
};

struct MSG_FC_BAZAAR_CUSTOMER_INFO_OK
{
	ClientIndex_t	clientIndex0;
};

struct MSG_FC_BAZAAR_INFO_OK
{
	ClientIndex_t	clientIndex0;
	BYTE			byBazaarType;
	char			szBazaarName[SIZE_MAX_BAZAAR_NAME];
};

struct MSG_FC_BAZAAR_SELL_PUT_ITEM
{
	UID64_t			itemUID;
	int				nAmount;
	int				nEachPrice;
};


struct MSG_FC_BAZAAR_SELL_PUT_ITEM_OK
{
	UID64_t			itemUID;
};

struct MSG_FC_BAZAAR_SELL_CANCEL_ITEM
{
	UID64_t			itemUID;
};

struct MSG_FC_BAZAAR_SELL_CANCEL_ITEM_OK
{
	UID64_t			itemUID;
};

struct MSG_FC_BAZAAR_SELL_START
{
	char			szBazaarName[SIZE_MAX_BAZAAR_NAME];
};

struct MSG_FC_BAZAAR_SELL_REQUEST_ITEMLIST
{
	ClientIndex_t	clientIndex0;
};

struct SBAZAAR_SELL_ITEM
{
	int				nSellItemNum0;
	int				nSellAmount0;
	int				nSellEachPrice0;
	UID64_t			itemUID;					// 2006-07-26 by cmkwon
	INT				PrefixCodeNum0;				// ���λ�, ������ 0
	INT				SuffixCodeNum0;				// ���̻�, ������ 0
												// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
												//	INT				ColorCode0;					// Ʃ�׽� �Ƹ��� ColorCode
	INT				ShapeItemNum0;			// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
	INT				EffectItemNum0;			// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
	INT				CoolingTime;			// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
	FIXED_TERM_INFO	FixedTermShape;				// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)
};

struct MSG_FC_BAZAAR_SELL_REQUEST_ITEMLIST_OK
{
	ClientIndex_t	clientIndex0;
	int				nItemListCnts0;
	_ARRAY(SBAZAAR_SELL_ITEM);
};

struct MSG_FC_BAZAAR_SELL_ITEM_ENCHANTLIST_OK
{
	UID64_t			itemUID;
	int				nEnchatCnts;
	_ARRAY(int EnchantItemNum);
};

struct MSG_FC_BAZAAR_SELL_BUY_ITEM
{
	ClientIndex_t	clientIndex0;
	UID64_t			itemUID;					// 2006-07-26 by cmkwon
	int				nAmount0;
};

struct MSG_FC_BAZAAR_SELL_BUY_ITEM_OK
{
	ClientIndex_t	clientIndex0;
	UID64_t			itemUID;					// 2006-07-26 by cmkwon
	int				nAmount0;
};

struct MSG_FC_BAZAAR_BUY_PUT_ITEM
{
	int				itemNum0;
	int				nAmount;
	int				nEachPrice;
};

struct MSG_FC_BAZAAR_BUY_PUT_ITEM_OK
{
	int				itemNum0;
	int				nBuyItemIndex0;
};

struct MSG_FC_BAZAAR_BUY_CANCEL_ITEM
{
	int				nBuyItemIndex0;
};

struct MSG_FC_BAZAAR_BUY_CANCEL_ITEM_OK
{
	int				nBuyItemIndex0;
};

struct MSG_FC_BAZAAR_BUY_START
{
	char			szBazaarName[SIZE_MAX_BAZAAR_NAME];
};

struct MSG_FC_BAZAAR_BUY_REQUEST_ITEMLIST
{
	ClientIndex_t	clientIndex0;
};

struct SBAZAAR_BUY_ITEM
{
	int				nBuyItemIndex0;
	int				nBuyItemNum0;
	int				nBuyAmount0;
	int				nBuyEachPrice0;
};
struct MSG_FC_BAZAAR_BUY_REQUEST_ITEMLIST_OK
{
	ClientIndex_t	clientIndex0;
	int				nItemListCnts0;
	_ARRAY(SBAZAAR_BUY_ITEM);
};

struct MSG_FC_BAZAAR_BUY_SELL_ITEM
{
	ClientIndex_t	clientIndex0;
	int				nBuyItemIndex0;
	UID64_t			itemUID;					// 2006-07-26 by cmkwon
	int				nAmount0;
};

struct MSG_FC_BAZAAR_BUY_SELL_ITEM_OK
{
	ClientIndex_t	clientIndex0;
	int				nBuyItemIndex0;
	UID64_t			itemUID;					// 2006-07-26 by cmkwon
	int				nAmount0;
};

struct MSG_FI_CASH_USING_GUILD
{
	UID32_t		guildUID;
	int			nIncreaseMemberCapacity;
	UID32_t		CashPrice;						// 2008-05-28 by dhjin, EP3 ���� ���� ���� - ���ܿ� ���� ĳ�� ������
};

struct MSG_FI_CASH_USING_CHANGE_CHARACTERNAME
{
	UID32_t		charUID;
	char		szChangedCharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_FI_CASH_PREMIUM_CARD_INFO
{// 2006-09-14 by dhjin, ���� �ʴ�� �ɹ��� ���� �ʿ�
	UID32_t			AccountUID;
	INT				nCardItemNum1;
	//	ATUM_DATE_TIME	atumTimeUpdatedTime1;	// ������ �ð�
	ATUM_DATE_TIME	atumTimeExpireTime1;	// ���� �ð�		// 2008-06-20 by dhjin, EP3 ���� ���� ���� - ���� �ð� �ʿ�
											//	float			fExpRate1;
											//	float			fDropRate1;
											//	float			fDropRareRate1;
											//	float			fExpRepairRate1;
};

struct MSG_FI_GUILD_RANK
{
	UID32_t		charUID;
	BYTE		GuildRank;
};

struct MSG_FN_CITYWAR_START_WAR
{
	ChannelIndex_t		ChannelIndex;
};

struct MSG_FN_CITYWAR_END_WAR
{
	ChannelIndex_t		ChannelIndex;
	UID32_t				OccupyGuildUID4;
};

struct MSG_FN_CITYWAR_CHANGE_OCCUPY_INFO
{
	ChannelIndex_t		ChannelIndex;
	UID32_t				OccupyGuildUID4;
};


// STRING_128 type
#define STRING_128_DEBUG_L1		0	// ����׿�, level 1
#define STRING_128_DEBUG_L2		1	// ����׿�, level 2
#define STRING_128_DEBUG_L3		2	// ����׿�, level 3
#define STRING_128_ADMIN_CMD	3	// ������ ��ɾ��
#define STRING_128_USER_ERR		4	// ����ڿ��� �ִ� ����
#define STRING_128_USER_NOTICE	5	// ����ڿ��� �ִ� �˸�
#define STRING_128_DEBUG_PRINT	6	// ����׿�, DBGOUT�� ���
#define STRING_128_SYSTEM_NOTICE	7	// ����ڿ��� �ִ� �˸�		// 2012-03-30 by hskim, EP4 Ʈ���� �ý��� �̺�Ʈ �˸�
#define STRING_128_USER_POPUP	8	// ����ڿ��� �ִ� �˾��˸�		// 2013-05-31 by jhseol,bckim �Ƹ� �÷���

// SendErrorMessage types
//#define T_PRE_IOCP							(MessageType_t)((T0_PRE<<8)|T1_PRE_IOCP)
//#define T_PRE_DB								(MessageType_t)((T0_PRE<<8)|T1_PRE_DB)
//#define T_IM_IOCP								(MessageType_t)((T0_IM<<8)|T1_IM_IOCP)
//#define T_IM_DB								(MessageType_t)((T0_IM<<8)|T1_IM_DB)
//#define T_FIELD_IOCP							(MessageType_t)((T0_FIELD<<8)|T1_FIELD_IOCP)
//#define T_FIELD_DB							(MessageType_t)((T0_FIELD<<8)|T1_FIELD_DB)
//#define T_NPC_IOCP							(MessageType_t)((T0_NPC<<8)|T1_NPC_IOCP)
//#define T_NPC_DB								(MessageType_t)((T0_NPC<<8)|T1_NPC_DB)
//#define T_TIMER								(MessageType_t)((T0_TIMER<<8)|T1_TIMER)
//#define T_DB									(MessageType_t)((T0_DB<<8)|T1_DB)
//#define T_NA									(MessageType_t)((T0_NA<<8)|T1_NA)		// NOT AVAILABLE PROTOCOL
//#define T_ERROR								(MessageType_t)((T0_ERROR<<8)|T1_ERROR)



constexpr bool IS_ALIVE_MSG(MessageType_t msgtype)
{
	return (HIBYTE(msgtype) >= unsigned(T0::PC_CONNECT) && HIBYTE(msgtype) <= unsigned(T0::NL_CONNECT))
		&& (LOBYTE(msgtype) == 0x03);
}

///////////////////////////////////////////////////////////////////////////////
// Structures for Data Exchange
///////////////////////////////////////////////////////////////////////////////

struct MSG_UNIT_INDEX
{
	ClientIndex_t	ClientIndex;
};

///////////////////////////////
// PC_DEFAULT_UPDATE
struct MSG_PC_DEFAULT_UPDATE_LAUNCHER_VERSION
{
	USHORT	Version[SIZE_MAX_VERSION];
};

struct MSG_PC_DEFAULT_UPDATE_LAUNCHER_UPDATE_INFO
{
	USHORT	UpdateVersion[SIZE_MAX_VERSION];
	char	FtpIP[SIZE_MAX_FTP_URL];
	USHORT	FtpPort;
	char	FtpAccountName[SIZE_MAX_ACCOUNT_NAME];
	char	FtpPassword[SIZE_MAX_PASSWORD];
	char	LauncherFileDownloadPath[SIZE_MAX_FTP_FILE_PATH];		// 2005-12-23 by cmkwon, ����
};

struct MSG_PC_DEFAULT_NEW_UPDATE_LAUNCHER_VERSION					// 2007-01-08 by cmkwon, C->P
{
	USHORT	Version[SIZE_MAX_VERSION];
};

struct MSG_PC_DEFAULT_NEW_UPDATE_LAUNCHER_UPDATE_INFO			// 2007-01-06 by cmkwon, �߰���
{
	USHORT	UpdateVersion[SIZE_MAX_VERSION];
	char	UpdateServerIP[SIZE_MAX_FTP_URL];
	USHORT	UpdateServerPort;
	char	AccountName[SIZE_MAX_ACCOUNT_NAME];
	char	Password[SIZE_MAX_PASSWORD];
	char	LauncherFileDownloadPath[SIZE_MAX_FTP_FILE_PATH];
};


///////////////////////////////
// PC_CONNECT

struct MSG_PC_CONNECT_VERSION
{
	USHORT	ClientVersion[SIZE_MAX_VERSION];
};

struct MSG_PC_CONNECT_UPDATE_INFO
{
	USHORT	OldVersion[SIZE_MAX_VERSION];		// ������Ʈ ���� ����
	USHORT	UpdateVersion[SIZE_MAX_VERSION];	// ������Ʈ �� ����
	char	FtpIP[SIZE_MAX_FTP_URL];
	USHORT	FtpPort;
	char	FtpAccountName[SIZE_MAX_ACCOUNT_NAME];
	char	FtpPassword[SIZE_MAX_PASSWORD];
	// 2005-12-23 by cmkwon
	//	char	FtpUpdateDir[SIZE_MAX_FTP_FILE_PATH];	// ������Ʈ�� ������ �ִ� ���
	char	FtpUpdateDownloadDir[SIZE_MAX_FTP_FILE_PATH];	// 2005-12-23 by cmkwon, ������Ʈ�� ���� �ٿ�ε� ���
};

struct MSG_PC_CONNECT_REINSTALL_CLIENT
{
	USHORT	LatestVersion[SIZE_MAX_VERSION];		// �ֽ� ����
};


#define LOGIN_TYPE_DIRECT	((BYTE)0)
#define LOGIN_TYPE_MGAME	((BYTE)1)

struct MSG_PC_CONNECT_LOGIN
{
	BYTE	LoginType;		// LOGIN_TYPE_XXX

							// 2008-10-08 by cmkwon, �븸 2�ܰ� ���� �ý��� ���� ����(email->uid) - 
							//	char	AccountName[SIZE_MAX_ACCOUNT_NAME];
	char	AccountName[SIZE_MAX_ORIGINAL_ACCOUNT_NAME];	// 2008-10-08 by cmkwon, �븸 2�ܰ� ���� �ý��� ���� ����(email->uid) - 

	BYTE	Password[SIZE_MAX_PASSWORD_MD5];
	char	FieldServerGroupName[SIZE_MAX_SERVER_NAME];
	char	PrivateIP[SIZE_MAX_IPADDRESS];
	//INT		MGameSEX;		// ���� - ��=0, ����=1, ����=2
	//INT		MGameYear;		// ����⵵(ex> 1976, 1981, 2000)
	char	WebLoginAuthKey[SIZE_MAX_WEBLOGIN_AUTHENTICATION_KEY];		// 2007-03-29 by cmkwon, �߰���
	char	ClientIP[SIZE_MAX_IPADDRESS];	// 2008-10-08 by cmkwon, �븸 Netpower_Tpe �ܺ����� ���� - PreServer�� ������ client IP
	char	SelectiveShutdownInfo[SIZE_MAX_SELECTIVE_SHUTDOWN_INFO];		// 2012-07-11 by hskim, ������ �˴ٿ�
	char	MACAddress[SIZE_MAX_MAC_ADDRESS];	// 2016-01-09 Future, MAC Blocking
};

struct MSG_PC_CONNECT_LOGIN_OK
{
	char	AccountName[SIZE_MAX_ACCOUNT_NAME];		// 2008-10-08 by cmkwon, �븸 2�ܰ� ���� �ý��� ���� ����(email->uid) - 2�� ������ ������

	char	FieldServerIP[SIZE_MAX_IPADDRESS];
	char	IMServerIP[SIZE_MAX_IPADDRESS];
	USHORT	FieldServerPort;
	USHORT	IMServerPort;
	bool	OpeningMoviePlay;		// 2011-12-21 by hskim, EP4 [������ 1ȸ ���]
};

struct MSG_PC_CONNECT_CLOSE
{
	INT		reason;
};

struct MSG_PC_CONNECT_SINGLE_FILE_VERSION_CHECK
{
	USHORT	DeleteFileListVersion[SIZE_MAX_VERSION];
	USHORT	NoticeVersion[SIZE_MAX_VERSION];
};		// single file�鿡 ���� ���� Ȯ��(deletefilelist.txt, notice.txt ��)

struct MSG_PC_CONNECT_SINGLE_FILE_UPDATE_INFO
{					// 2007-01-08 by cmkwon, AU_SERVER_TYPE_XXX
	USHORT	NewDeleteFileListVersion[SIZE_MAX_VERSION];
	USHORT	NewNoticeVersion[SIZE_MAX_VERSION];
	char	FtpIP[SIZE_MAX_FTP_URL];
	USHORT	FtpPort;
	char	FtpAccountName[SIZE_MAX_ACCOUNT_NAME];
	char	FtpPassword[SIZE_MAX_PASSWORD];
	char	DeleteFileListDownloadPath[SIZE_MAX_FTP_FILE_PATH];
	char	NoticeFileDownloadPath[SIZE_MAX_FTP_FILE_PATH];
};		// single file�鿡 ���� ������Ʈ ����(deletefilelist.txt, notice.txt ��)

#ifndef MGAME_MAX_PARAM_STRING_SIZE
#define MGAME_MAX_PARAM_STRING_SIZE			50
#endif

struct MEX_SERVER_GROUP_INFO_FOR_LAUNCHER
{
	char	ServerGroupName[SIZE_MAX_SERVER_NAME];
	int		Crowdedness;		// ȥ�⵵, 0% ~ 100%
};

struct MSG_PC_CONNECT_GET_SERVER_GROUP_LIST_OK
{
	size_t		NumOfServerGroup;
	ARRAY_(MEX_SERVER_GROUP_INFO_FOR_LAUNCHER);
};	// P->Launcher

struct SGAME_SERVER_GROUP_OLD		// 2007-09-05 by cmkwon, EXE_1�� �α��� ���� ���� �������̽� ���� - ���� ���� ȣȯ�� ���� ����ü
{
	char	szGameServerGroupName[SIZE_MAX_GAME_SERVER_GROUP_NAME];
	char	szPreServerIP0[SIZE_MAX_IPADDRESS];
	USHORT	usPreServerPort0;
};

struct MSG_PC_CONNECT_GET_GAME_SERVER_GROUP_LIST_OK		// 2007-05-02 by cmkwon, PreServer ����
{// 2007-05-15 by cmkwon, ����ü�� ����� SIZE_MAX_PACKET ���� �۾ƾ� �Ѵ�.
	SGAME_SERVER_GROUP_OLD arrGameServerGroupList[COUNT_MAX_GAME_SERVER_GROUP_LIST];		// 2007-09-05 by cmkwon, EXE_1�� �α��� ���� ���� �������̽� ���� - ���� ���� ȣȯ�� ���� ����ü ���
};

struct SGAME_SERVER_GROUP		// 2007-05-02 by cmkwon, PreServer ����
{
	char	szGameServerGroupName[SIZE_MAX_GAME_SERVER_GROUP_NAME];
	char	szPreServerIP0[SIZE_MAX_IPADDRESS];
	// 2007-09-05 by cmkwon, EXE_1�� �α��� ���� ���� �������̽� ���� - Port�� �⺻��Ʈ�� �����
	//	USHORT	usPreServerPort0;
	USHORT	usPreServerTab8OrderIndex;			// 2007-09-05 by cmkwon, EXE_1�� �α��� ���� ���� �������̽� ���� - �ʵ��߰�
};
using vectSGAME_SERVER_GROUP = vector<SGAME_SERVER_GROUP>;			// 2007-05-15 by cmkwon

struct MSG_PC_CONNECT_GET_NEW_GAME_SERVER_GROUP_LIST_OK		// 2007-09-05 by cmkwon, EXE_1�� �α��� ���� ���� �������̽� ���� - ���� �߰��� ����ü
{// 2007-05-15 by cmkwon, ����ü�� ����� SIZE_MAX_PACKET ���� �۾ƾ� �Ѵ�.
	SGAME_SERVER_GROUP arrGameServerGroupList[COUNT_MAX_GAME_SERVER_GROUP_LIST];
};

struct MSG_PC_CONNECT_NETWORK_CHECK			// 2007-06-18 by cmkwon, ��Ʈ��ũ ���� üũ 
{
	int		nCheckCount;
};

using MSG_PC_CONNECT_NETWORK_CHECK_OK = MSG_PC_CONNECT_NETWORK_CHECK;		// 2007-06-18 by cmkwon, ��Ʈ��ũ ���� üũ 


struct MSG_PC_CONNECT_LOGIN_BLOCKED
{
	// 2008-10-08 by cmkwon, �븸 2�ܰ� ���� �ý��� ���� ����(email->uid) -
	//	char			szAccountName[SIZE_MAX_ACCOUNT_NAME];				// 2007-01-10 by cmkwon
	char			szAccountName[SIZE_MAX_ORIGINAL_ACCOUNT_NAME];		// 2008-10-08 by cmkwon, �븸 2�ܰ� ���� �ý��� ���� ����(email->uid) -

	int				nBlockedType;
	ATUM_DATE_TIME	atimeStart;
	ATUM_DATE_TIME	atimeEnd;
	char			szBlockedReasonForUser[SIZE_MAX_BLOCKED_ACCOUNT_REASON];		// 2007-01-10 by cmkwon
};

///////////////////////////////
// FN_CONNECT

struct MSG_FN_CONNECT_MAP_INFO
{
	MapIndex_t	MapIndex;
};

struct MSG_FN_CONNECT_INCREASE_CHANNEL
{
	MapIndex_t	MapIndex;
	INT			TotalChannelCount;
	DWORD		Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, ���� ���� ���� - ���� ������ ȣȯ �ȵǵ��� ����ü ũ�� �ٲ�
};		// F->N

struct MSG_FN_CONNECT_SET_CHANNEL_STATE
{
	MAP_CHANNEL_INDEX	MapChannelIndex;
	bool				EnableChannel;	// TRUE: Enable, FALSE: Disable
	DWORD				Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, ���� ���� ���� - ���� ������ ȣȯ �ȵǵ��� ����ü ũ�� �ٲ�
};		// F->N

		///////////////////////////////
		// FC_CONNECT

struct MSG_FC_CONNECT_LOGIN
{
	char	AccountName[SIZE_MAX_ACCOUNT_NAME];
	char	Password[SIZE_MAX_PASSWORD_MD5_STRING];
	char	PrivateIP[SIZE_MAX_IPADDRESS];
	char	MACAddress[SIZE_MAX_MAC_ADDRESS];		// 2015-09-23 Future, added transmition of MAC Address
	DWORD	Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, ���� ���� ���� - ���� ������ ȣȯ �ȵǵ��� ����ü ũ�� �ٲ�
};

// 2005-12-08 by cmkwon
/////////////////////////////////////
//// ������ ���� ��ġ(POS_XXX)
//#define POS_PROW							((BYTE)0)	// ���̴�(���� ���)
//#define POS_PROWIN						((BYTE)1)	// ��ǻ��(�߾� ����)
//#define POS_PROWOUT						((BYTE)2)	// 1�� ����(���� ����)
//#define POS_WINGIN						((BYTE)3)	//		������(�߾� ����)
//#define POS_WINGOUT						((BYTE)4)	// 2�� ����(���� ����)
//#define POS_CENTER						((BYTE)5)	// �Ƹ�(�߾� ���)
//#define POS_REAR							((BYTE)6)	// ����(�Ĺ� ���)
//#define POS_ATTACHMENT					((BYTE)7)	// 2006-03-30 by cmkwon, ������ �Ǽ��縮 - ������(�Ĺ� ����-������ũ|�����̳ʰ迭)
//#define POS_PET							((BYTE)8)	// 2006-03-30 by cmkwon, �ð����� �Ǽ��縮(�Ĺ� ����)
struct CHARACTER_RENDER_INFO
{
	INT		RI_Prow;			// POS_PROW�� ItemNum, ���� (����Ʈ�迭 + ���迭 or ���̴�)
	INT		RI_ProwIn;			// 2005-03-17 by cmkwon (CPU ��ǻ��)
	INT		RI_ProwOut;			// POS_PROWOUT�� ItemNum, ������ �ٱ���, ���� (1������ - ���迭 or ĳ��迭)
	INT		RI_WingIn;			// 2005-03-17 by cmkwon (��ũ)
	INT		RI_WingOut;			// POS_WINGOUT�� ItemNum, ������ �ٱ���, ����(2������ - ���ϰ迭 or �̻��ϰ迭)
	INT		RI_Center;			// POS_CENTER�� ItemNum, �߾� (���迭 - �Ƹ�) ���
								// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - �ʿ� ����
								//	INT		RI_ArmorColorCode;	// 2005-12-08 by cmkwon, �Ƹ��� ����Į��
	INT		RI_Rear;			// POS_REAR�� ItemNum, �Ĺ� (�����迭)

								// 2010-06-15 by shcho&hslee ��ý���
								//INT		RI_Attachment;		// POS_ATTACHMENT�� ItemNum, ������ (�����̳ʰ迭<������ũ/��Ÿ�迭> or �������� ����)
	INT		RI_AccessoryUnLimited;

	// 2010-06-15 by shcho&hslee ��ý���
	//INT		RI_Pet;				// 2005-03-17 by cmkwon (�ð����� �Ǽ��縮)
	INT		RI_AccessoryTimeLimit;

	bool	RI_Invisible;		// 2006-11-27 by dhjin, ĳ���� ������ �ʴ� �÷���
	INT		RI_Prow_ShapeItemNum;			// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - ���̴� ShapeItemNum
	INT		RI_WingIn_ShapeItemNum;			// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - ��ũ ShapeItemNum
	INT		RI_Center_ShapeItemNum;			// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - �Ƹ� ShapeItemNum
	INT		RI_ProwOut_ShapeItemNum;		// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 1������ ShapeItemNum
	INT		RI_WingOut_ShapeItemNum;		// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 2������ ShapeItemNum
	INT		RI_ProwOut_EffectItemNum;		// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 1������ ź�� EffectItemNum
	INT		RI_WingOut_EffectItemNum;		// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 2������ ź�� EffectItemNum
	INT		RI_MonsterUnitKind_ForTransformer;	// 2010-03-18 by cmkwon, ���ͺ��� ���� - ���Ż����� ���� ����ī��

	INT		RI_Pet;							// 2010-06-15 by shcho&hslee ��ý��� - �� ������.
	INT		RI_Pet_ShapeItemNum;			// 2010-06-15 by shcho&hslee ��ý��� - ShapeItemNum.

											///////////////////////////////////////////////////////////////////////////////
											/// \fn			
											/// \brief		// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
											/// \author		cmkwon
											/// \date		2009-08-27 ~ 2009-08-27
											/// \warning	
											///
											/// \param		
											/// \return		
											///////////////////////////////////////////////////////////////////////////////
	bool SetRenderInfoWithPOS(INT i_nPos, INT i_nItemNum, INT i_nShapeItemNum, INT i_nEffectItemNum)
	{
		switch (i_nPos)
		{

		case POS_PROW:			// ���̴� - �������氡��
		{
			RI_Prow = i_nItemNum;
			RI_Prow_ShapeItemNum = i_nShapeItemNum;
		}
		break;

		case POS_PROWIN:		// CPU ��ǻ�� - 
		{
			RI_ProwIn = i_nItemNum;		// 2012-06-20 by isshin CPU ���� ���� ����
		}
		break;

		case POS_PROWOUT:		// 1�� ���� - �������氡�� + ź������Ʈ���氡��
		{
			RI_ProwOut = i_nItemNum;
			RI_ProwOut_ShapeItemNum = i_nShapeItemNum;
			RI_ProwOut_EffectItemNum = i_nEffectItemNum;
		}
		break;

		case POS_WINGIN:		// ��ũ - �������氡��
		{
			RI_WingIn = i_nItemNum;
			RI_WingIn_ShapeItemNum = i_nShapeItemNum;
		}
		break;

		case POS_WINGOUT:		// 2�� ���� - �������氡�� + ź������Ʈ���氡��
		{
			RI_WingOut = i_nItemNum;
			RI_WingOut_ShapeItemNum = i_nShapeItemNum;
			RI_WingOut_EffectItemNum = i_nEffectItemNum;
		}
		break;

		case POS_CENTER:		// �Ƹ� - �������氡��
		{
			RI_Center = i_nItemNum;
#ifdef _REWORKED_COLORSHOP
			if (i_nEffectItemNum)
			{
				RI_Center_ShapeItemNum = i_nEffectItemNum;
			}
			else
#endif
				RI_Center_ShapeItemNum = i_nShapeItemNum;
		}
		break;

		case POS_REAR:			// ���� - 
		{
			RI_Rear = i_nItemNum;
		}
		break;

		// 2010-06-15 by shcho&hslee ��ý��� - ���� ����.
		//case POS_ATTACHMENT:	// ������ũ or �������Ǳ��� - 
		case POS_ACCESSORY_UNLIMITED:
		{
			//RI_Attachment	= i_nItemNum;
			RI_AccessoryUnLimited = i_nItemNum;
		}
		break;

		// 2010-06-15 by shcho&hslee ��ý��� - ���� ����.
		//case POS_PET:						// �ð����� �Ǽ��縮 - 
		case POS_ACCESSORY_TIME_LIMIT:		// �ð����� �Ǽ��縮 - 
		{
			//RI_Pet			= i_nItemNum;
			RI_AccessoryTimeLimit = i_nItemNum;
		}
		break;

		// 2010-06-15 by shcho&hslee ��ý���
		case POS_PET:
		{
			RI_Pet = i_nItemNum;
			RI_Pet_ShapeItemNum = i_nShapeItemNum;
		}
		break;

		default:
		{
			return FALSE;
		}
		}
		return TRUE;
	};
};		// ��ü�� �׸��� ���� �ʿ��� ������ ������ ����

struct FC_CONNECT_LOGIN_INFO
{
	char					CharacterName[SIZE_MAX_CHARACTER_NAME];
	UID32_t					CharacterUniqueNumber;
	USHORT					Race;
	USHORT					UnitKind;
	BYTE					PilotFace;
	BYTE					Color;
	int						RacingPoint;		// 2009-11-02 by cmkwon, ĳ��(�κ�/â�� Ȯ��) ������ �߰� ���� - 
	CHARACTER_RENDER_INFO	CharacterRenderInfo;
	// START 2011-11-03 by shcho, yedang �˴ٿ��� ���� - ���������� �߰��� �޾ƿ´�.
	bool					ShutDownMINS;
	// END 2011-11-03 by shcho, yedang �˴ٿ��� ���� - ���������� �߰��� �޾ƿ´�.
	DWORD					Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, ���� ���� ���� - ���� ������ ȣȯ �ȵǵ��� ����ü ũ�� �ٲ�
};

struct MSG_FC_CONNECT_LOGIN_OK
{
	UID32_t				AccountUniqueNumber;
	BYTE				NumCharacters;
	FC_CONNECT_LOGIN_INFO	Characters[3];
	//char				VoIP1to1ServerIP[SIZE_MAX_IPADDRESS];
	//USHORT				VoIP1to1ServerPort;
	//char				VoIPNtoNServerIP[SIZE_MAX_IPADDRESS];
	//USHORT				VoIPNtoNServerPort;
	BYTE				bIsUseSecondaryPasswordSystem;			// 2007-09-13 by cmkwon, ��Ʈ�� 2���н����� ���� - MSG_FC_CONNECT_LOGIN_OK �� �ʵ� �߰�
	BYTE				bIsSetSecondaryPassword;				// 2007-09-13 by cmkwon, ��Ʈ�� 2���н����� ���� - MSG_FC_CONNECT_LOGIN_OK �� �ʵ� �߰�
	DWORD				Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, ���� ���� ���� - ���� ������ ȣȯ �ȵǵ��� ����ü ũ�� �ٲ�
	// todo : remove
	[[deprecated("unused functionality")]]
	bool				BIsTestServer;							// 2012-11-27 by khkim, �׽�Ʈ �������� �Ϲ� �������� ����
	BYTE				DBNum;									// 2012-11-28 by jhseol, �������� ĳ�� WEB ���� - ������ ����ϴ� ��ȣ
};

struct MSG_FC_CONNECT_CLOSE
{
	INT		reason;
};

struct MSG_FC_CONNECT_SYNC_TIME {
	UINT	CurrentTime;	// 4�ð� �������� ���� ��
};


struct MSG_FC_CONNECT_NETWORK_CHECK		// 2008-02-15 by cmkwon, Client<->FieldServer �� ��Ʈ��ũ ���� üũ - 
{
	int		nCheckCount;
	DWORD	dwClientTick;
};
// 2008-10-31 by cmkwon, ��Ʈ��ũ ���� üũ ���� ����(���۰����� ����) - �Ʒ��� ���� ���� ������
//typedef MSG_FC_CONNECT_NETWORK_CHECK	MSG_FC_CONNECT_NETWORK_CHECK_OK;	// 2008-02-15 by cmkwon, Client<->FieldServer �� ��Ʈ��ũ ���� üũ - 
struct MSG_FC_CONNECT_NETWORK_CHECK_OK		// 2008-10-31 by cmkwon, ��Ʈ��ũ ���� üũ ���� ����(���۰����� ����) - 
{
	int		nCheckCount;
	DWORD	dwClientTick;
	int		nWriteBufferSize;		// 2008-10-31 by cmkwon, ��Ʈ��ũ ���� üũ ���� ����(���۰����� ����) - 
};

struct MSG_FC_CONNECT_ARENASERVER_INFO
{// 2007-12-28 by dhjin, �Ʒ��� ���� - F -> C 
	CHAR		MainServerName[SIZE_MAX_SERVER_NAME];
	CHAR		ArenaServerName[SIZE_MAX_SERVER_NAME];
	USHORT		AFS_Port;
	USHORT		AIS_Port;
	CHAR		AFS_IP[SIZE_MAX_IPADDRESS];
	CHAR		AIS_IP[SIZE_MAX_IPADDRESS];
};

struct MSG_FC_CONNECT_ARENASERVER_LOGIN
{// 2007-12-28 by dhjin, �Ʒ��� ���� - AF -> C 
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIdx;
	CHAR			MFSName[SIZE_MAX_SERVER_NAME];
};

struct MSG_FC_CONNECT_ARENASERVER_LOGIN_OK
{// 2007-12-28 by dhjin, �Ʒ��� ���� - C -> AF
	CHARACTER		AFSCharacter;
	ATUM_DATE_TIME	atimeCurServerTime;		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��, ���� �Ʒ��� ���� ��¥ �ð�
};

// 2008-02-25 by dhjin, �Ʒ��� ���� - AF -> C
struct SSERVER_GROUP_FOR_CLIENT
{
	CHAR			MFS_ServerIDName[SIZE_MAX_SERVER_NAME];
	CHAR			MFS_Name[SIZE_MAX_SERVER_NAME];
};

struct MSG_FC_CONNECT_ARENASERVER_SSERVER_GROUP_FOR_CLIENT
{// �Ʒ��� ���� -
	SSERVER_GROUP_FOR_CLIENT		ServerGroupInfo[SIZE_MAX_SERVER_GROUP_COUNT];
};



///////////////////////////////
// FP_CONNECT

#define PRESERVER_AUTH_TYPE_LOGIN				0x00
// 2004-12-16 by cmkwon, �ٸ� �ʵ弭������ ������ �����Ƿ� ������
//#define PRESERVER_AUTH_TYPE_WARP_CONNECT		0x01
// 2005-07-21 by cmkwon, �ٸ� �ʵ弭������ GameStart�� �����Ƿ� ������
//#define PRESERVER_AUTH_TYPE_CONNECT_GAMESTART	0x02

struct MSG_FP_CONNECT_AUTH_USER
{
	char			AccountName[SIZE_MAX_ACCOUNT_NAME];
	SERVER_ID		FieldServerID;
	char			PrivateIP[SIZE_MAX_IPADDRESS];
	ClientIndex_t	ClientIndex;
	BYTE			AuthType;
	DWORD			Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, ���� ���� ���� - ���� ������ ȣȯ �ȵǵ��� ����ü ũ�� �ٲ�
};

struct MSG_FP_CONNECT_AUTH_USER_OK
{
	char			AccountName[SIZE_MAX_ACCOUNT_NAME];
	UID32_t			AccountUniqueNumber;
	ClientIndex_t	ClientIndex;
	BYTE			AuthType;
	//int			GalaNetAccountIDNum;							// 2006-06-01 by cmkwon, exteranl authentication DB accountID Number
	ATUM_DATE_TIME	AccountRegisteredDate;							// 2006-06-02 by cmkwon
	char			PasswordFromDB[SIZE_MAX_PASSWORD_MD5_STRING];	// 2006-06-02 by cmkwon
	int				GameContinueTimeInSecondOfToday;				// 2006-11-15 by cmkwon, ���� �Ϸ� ���� ���� �ð�
	ATUM_DATE_TIME	LastGameEndDate;								// 2006-11-15 by cmkwon, ������ ���� ���� �ð�
	ATUM_DATE_TIME	Birthday;										// 2007-06-28 by cmkwon, �߱� ��������(�������� FielServer�� ��������) - �������� ����
	char			SecondaryPassword[SIZE_MAX_PASSWORD_MD5_STRING];	// 2007-09-12 by cmkwon, ��Ʈ�� 2���н����� ���� - MSG_FP_CONNECT_AUTH_USER_OK �� �ʵ� �߰�
	USHORT			UserType;											// 2013-01-18 by khkim, GLog ���� ���� 
	DWORD			Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, ���� ���� ���� - ���� ������ ȣȯ �ȵǵ��� ����ü ũ�� �ٲ�
};

struct MSG_FP_CONNECT_FIELD_CONNECT
{
	char		FieldServerGroupName[SIZE_MAX_SERVER_NAME];
	SERVER_ID	FieldServerID;
	int			NumOfMapIndex;
	bool		ArenaFieldServerCheck;	// 2007-12-26 by dhjin, �Ʒ��� ���� - TRUE => �Ʒ��� �ʵ� ����
	SDBSERVER_GROUP		DBServerGroup;	// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - 
	ARRAY_(MapIndex_t);					// array of MapIndexes
};

struct MSG_FP_CONNECT_FIELD_CONNECT_OK
{
	DWORD			Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, ���� ���� ���� - ���� ������ ȣȯ �ȵǵ��� ����ü ũ�� �ٲ�
};

struct MSG_FP_CONNECT_NOTIFY_CLOSE
{
	char			AccountName[SIZE_MAX_ACCOUNT_NAME];
	ClientIndex_t	ClientIndex;
	DWORD			Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, ���� ���� ���� - ���� ������ ȣȯ �ȵǵ��� ����ü ũ�� �ٲ�
};

struct MSG_FP_CONNECT_NOTIFY_FIELDSERVER_CHANGE
{
	char			AccountName[SIZE_MAX_ACCOUNT_NAME];
	SERVER_ID		FieldServerID;
	ClientIndex_t	ClientIndex;
};

struct MSG_FP_CONNECT_NOTIFY_FIELDSERVER_CHANGE_OK
{
	ClientIndex_t	ClientIndex;
};


struct MSG_FP_CONNECT_UPDATE_DBSERVER_GROUP		// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - 
{
	SDBSERVER_GROUP DBServerGroup;
};

struct MSG_FP_CONNECT_CHECK_CONNECTABLE_ACCOUNT			// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - 
{
	char					AccountName[SIZE_MAX_ACCOUNT_NAME];
	MSG_PC_CONNECT_LOGIN_OK PCConnectLoginOK;
};

struct MSG_FP_CONNECT_CHECK_CONNECTABLE_ACCOUNT_OK		// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - 
{
	int						ErrorCode;	// 0�̸� No Error, 0�� �ƴϸ� �����ڵ��� 
	char					AccountName[SIZE_MAX_ACCOUNT_NAME];
	MSG_PC_CONNECT_LOGIN_OK PCConnectLoginOK;
};

// start 2011-12-12 by hskim, GLog 2��
//#define T_FP_GLOG_CONNECT_USER_NEW				(MessageType_t)((T0_FP_GLOG<<8)|T1_FP_GLOG_CONNECT_USER_NEW)
//#define T_FP_GLOG_CONNECT_USER_LOGIN			(MessageType_t)((T0_FP_GLOG<<8)|T1_FP_GLOG_CONNECT_USER_LOGIN)

struct MSG_FP_GLOG_CONNECT_USER_NEW
{
	char AccountName[SIZE_MAX_ACCOUNT_NAME];		// �ű� ���� ����
};

struct MSG_FP_GLOG_CONNECT_USER_LOGIN
{
	char AccountName[SIZE_MAX_ACCOUNT_NAME];		// ���� ���� ���� �߰�
};
// end 2011-12-12 by hskim, GLog 2��

///////////////////////////////
// IP_CONNECT

struct MSG_IP_CONNECT_IM_CONNECT
{
	char		ServerGroupName[SIZE_MAX_SERVER_NAME];
	SERVER_ID	IMServerID;								// 2006-05-10 by cmkwon
	DWORD		Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, ���� ���� ���� - ���� ������ ȣȯ �ȵǵ��� ����ü ũ�� �ٲ�
														// 2006-05-10 by cmkwon, IMServerID ������ ���� - IP ������ ����Ѵ�.
														//	int		IMServerListenPort;
};

struct MSG_IP_GET_SERVER_GROUP_INFO_ACK
{
	char	ServerGroupName[SIZE_MAX_SERVER_NAME];
	UINT	IMCurrentUserCounts;
	UINT	IMMaxUserCounts;
	DWORD	Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, ���� ���� ���� - ���� ������ ȣȯ �ȵǵ��� ����ü ũ�� �ٲ�
};

struct MSG_IP_ADMIN_PETITION_SET_PERIOD
{// 2007-11-19 by cmkwon, �����ý��� ������Ʈ - MSG_IP_ADMIN_PETITION_SET_PERIOD ����ü �߰�
	BYTE			byIsImmediatOn;		// 2007-11-20 by cmkwon, ��� ���� �÷���
	BYTE			byIsImmediatOff;	// 2007-11-20 by cmkwon, ��� ���� �÷���
	ATUM_DATE_TIME	atStart;			// 2007-11-20 by cmkwon, ���� ��¥�ð�
	ATUM_DATE_TIME	atEnd;				// 2007-11-20 by cmkwon, ���� ��¥�ð�
};

///////////////////////////////
// FI_CONNECT

struct MSG_FI_CONNECT
{
	SERVER_ID	FieldServerID;
	int			NumOfMapIndex;
	bool		ArenaServerCheck;		// 2008-02-28 by dhjin, �Ʒ��� ���� - 0:�Ϲ� ���Ӽ���, 1:�Ʒ������ռ���
	ARRAY_(MapIndex_t);					// array of MapIndexes
};

struct MSG_FI_CONNECT_NOTIFY_FIELDSERVER_IP
{
	UID32_t		CharacterUniqueNumber;
	SERVER_ID	FieldServerID;
	//char		IPAddress[SIZE_MAX_IPADDRESS];
	DWORD		Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, ���� ���� ���� - ���� ������ ȣȯ �ȵǵ��� ����ü ũ�� �ٲ�
};

struct MSG_FI_CONNECT_NOTIFY_GAMEEND
{
	UID32_t	CharacterUniqueNumber;
};


///////////////////////////////////////////////////////////////////////////////
struct MSG_PP_CONNECT		// 2008-02-22 by cmkwon, ServerPreServer->MasangPreServer �� ���� ���� ���� �ý��� �߰� - 
{
	int			nServiceUID;						// ����� ������� ����, ���� ó���� ���� �߰�
	int			nLanguageType;						// 
	char		szPreServerIP[SIZE_MAX_IPADDRESS];
	USHORT		nPreServerPort;
	BYTE		byIsOnlyChoiceServer;
	BYTE		byIsTestServer;
	BYTE		byIsUseExternalAuthentication;
	int			nPreServerGroupCnts;				// PreServer Count
	int			nEnableGameServerGroupCnts;			// Active ServerGroup Count
	char		szReserve[100];						// 
	char *GetWriteLogString(char *o_szLogString)
	{
		sprintf(o_szLogString, "%d|%d|%15s|%d|%d|%d|%d|%d|%d\r\n", nServiceUID, nLanguageType, szPreServerIP, nPreServerPort
			, byIsOnlyChoiceServer, byIsTestServer, byIsUseExternalAuthentication, nPreServerGroupCnts, nEnableGameServerGroupCnts);
		return o_szLogString;
	}
};

///////////////////////////////////////////////////////////////////////////////
// 2011-01-26 by hskim, ���� ���� ����
struct MSG_PATUM_CONNECT
{
	char		szGameName[SIZE_MAX_AUTH_GAMENAME];
	char		szServerIP[SIZE_MAX_IPADDRESS];
	USHORT		nServerPort;
	char		szCurrentVer[SIZE_MAX_AUTH_CURRENTVER];
	int			nLanguageType;
	BYTE		byTestServer;
	BYTE		byUseExternalAuthentication;
	int			nPreServerGroupCnts;
	int			nEnableGameServerGroupCnts;
	char		szReserve[50];
	char *GetWriteLogString(char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%s|%d|%s|%d|%d|%d|%d|%d\r\n", szGameName, szServerIP, nServerPort, szCurrentVer, nLanguageType,
			byTestServer, byUseExternalAuthentication, nPreServerGroupCnts, nEnableGameServerGroupCnts);

		return o_szLogString;
	}
};

struct MSG_PATUM_CONNECT_OK
{
	char		szReserve[100];
};

struct MSG_PATUM_CONNECT_FAIL
{
	char		szReserve[100];
};

// start 2011-06-22 by hskim, �缳 ���� ����
struct MSG_PATUM_CONNECT_SHUTDOWN
{
	char		szReserve[100];
};
// end 2011-06-22 by hskim, �缳 ���� ����

#//if defined(_ATUM_SERVER)	// 2008-02-26 by cmkwon, Ŭ���̾�Ʈ���� ������ ���� ���� �ذ�
struct MSG_PP_CONNECT_OK		// 2008-02-22 by cmkwon, ServerPreServer->MasangPreServer �� ���� ���� ���� �ý��� �߰� - 
{
	char		szPreServerODBCDSN[SIZE_MAX_ODBC_CONN_STRING];		// ODBC_DSN
	char		szPreServerODBCUID[SIZE_MAX_ODBC_CONN_STRING];		// ODBC_UID
	char		szPreServerODBCPASSWORD[SIZE_MAX_ODBC_CONN_STRING];	// ODBC_PASSWD	
	char		szReserve[100];										// 
};
//#endif

struct MSG_FI_EVENT_NOTIFY_WARP
{
	UID32_t	CharacterUniqueNumber;
	MAP_CHANNEL_INDEX	MapChannelIndex;	// ��ȭ�� ������ �� �� �̸�
	INT		nNumOfTimer;					// ���� �ʵ� �������� ������ ���� 0
	ARRAY_(TIMER_EVENT_4_EXCHANGE);
};					// ��ȭ�� �� �̸�, ���� Timer ����(���� �ʵ� �������� ������ ������ �ʴ´�), ���

struct MSG_FI_EVENT_CHAT_BLOCK				// 2008-12-30 by cmkwon, ������ ä�� ���� ī�� ���� - 
{
	char	szBlockedCharacterName[SIZE_MAX_CHARACTER_NAME];
	int		nBlockedMinutes;
};



// Just Declare
struct TIMER_EVENT;

struct TIMER_EVENT_4_EXCHANGE
{
	TimerEventType		Type;
	//	TimeUnit_t			StartTimeStamp;			// milli-seconds
	//	TimeUnit_t			ExpireTime;				// milli-seconds
	TimeUnit_t			TimeInterval;			// milli-seconds, (ExpireTime - StartTimeStamp)
												//	CFieldIOCPSocket	*pFieldIOCPSocket;
	UID32_t				CharacterUniqueNumber;	// event�� ������ charac�� ������ �ٸ� charac�� socket�� ����ϴ� ��츦 ���� ����, IsUsing()�� CharacterUniqueNumber�� ���ƾ� ��!
												//	ClientIndex_t		ClientIndex;			// event�� ������ charac�� ������ �ٸ� charac�� socket�� ����ϴ� ��츦 ���� ����, IsUsing()�� ClientIndex�� ���ƾ� ��!
	float				FloatParam1;			// delete_item��: ���� Endurance
	INT					IntParam1;				// delete_item��: ItemNum
												//	TIMER_EVENT_BUCKET	*pCurrentBucket;

												// operator overloading
	TIMER_EVENT_4_EXCHANGE& operator=(const TIMER_EVENT& rhs);
};

struct MSG_FI_EVENT_NOTIFY_WARP_OK
{
	UID32_t	CharacterUniqueNumber;
};			// �ٸ� �ʵ� �������� ������ ��츸 �޴´�

struct MSG_FI_EVENT_GET_WARP_INFO
{
	UID32_t			CharacterUniqueNumber;
};			// Party & TimerEvent����, �ٸ� �ʵ� �������� ������ ��� ���� ��û

struct MSG_FI_EVENT_GET_WARP_INFO_OK
{
	UID32_t			AccountUniqueNumber;		// �ٸ� �ʵ� �������� ������, ������ �ʿ���
	UID32_t			CharacterUniqueNumber;
	PartyID_t		PartyID;
	UID32_t			MasterCharacterUniqueNumber;
	USHORT			nPartyMembers;
	USHORT			nTimerEvents;
	ARRAY_(FI_PARTY_MEMBER_INFO);
	ARRAY_(TIMER_EVENT_4_EXCHANGE);
};		// Party & TimerEvent����, �ٸ� �ʵ� �������� ������ ��� ���� �ֱ�

struct MSG_FI_CONNECT_NOTIFY_DEAD
{
	UID32_t	CharacterUniqueNumber;
	UID32_t AttackerGuildUID;
};	// F->I, check: ����� DEAD�� ��Ƽ Ż�� ���ؼ� �������, 20030821, kelovon

struct MSG_FI_GET_FIELD_USER_COUNTS_ACK
{
	SERVER_ID	FieldServerID;
	UINT		AccumulatedFieldUserCounts;
	UINT		CurrentFieldUserCounts;
	UINT		MaxFieldUserCounts;
	UINT		AccumulatedFieldMonsterCounts;
	UINT		CurrentFieldMonsterCounts;
};

enum GameStartType
{
	GST_GAMESTART = 0,
	GST_CONN_GAMESTART = 1,
	GST_WARP_CONN_GAMESTART = 2,
	GST_WARP_SAME_MAP = 3,
	GST_WARP_SAME_FIELD_SERVER = 4
};

struct MSG_FI_CONNECT_NOTIFY_GAMESTART {
	UID32_t			CharacterUniqueNumber;
	GameStartType	FieldGameStartType;
	MAP_CHANNEL_INDEX	MapChannelIndex;
};				// F->I, ���� �������� �� IM Server�� �˸�, ��Ƽ ���� Ȯ�� ��û ��

struct MSG_FI_CONNECT_NOTIFY_DEAD_GAMESTART
{
	UID32_t			CharacterUniqueNumber;
};

struct MSG_FI_CONNECT_PREPARE_SHUTDOWN	// 2007-08-27 by cmkwon, �����ٿ��غ� ��ɾ� �߰�(SCAdminTool���� SCMonitor�� PrepareShutdown�� ���� �� �� �ְ�)
{
	bool			bPrepareShutdown;			// TURE�� �����ٿ� �غ� ����, FALSE�̸� �����ٿ� �غ� ����
};

///////////////////////////////
// IC_CONNECT

struct MSG_IC_CONNECT_LOGIN
{
	char	AccountName[SIZE_MAX_ACCOUNT_NAME];
	char	ServerName[SIZE_MAX_SERVER_NAME];
	char	CharacterName[SIZE_MAX_CHARACTER_NAME];
	char	Password[SIZE_MAX_PASSWORD_MD5_STRING];			// MD5
	BYTE	LoginType;						// see below
	DWORD	Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, ���� ���� ���� - ���� ������ ȣȯ �ȵǵ��� ����ü ũ�� �ٲ�
};

#define CHAT_LOGIN_TYPE_GAME		0x00
#define CHAT_LOGIN_TYPE_MESSENGER	0x01

struct MSG_IC_CONNECT_CLOSE
{
	INT		reason;
};

/*
// check: Ȯ�� �ʿ�
//typedef	struct
//{
//	BYTE					NumCharacters;
//	FC_CONNECT_LOGIN_INFO	Characters[3];
//} MSG_IC_CONNECT_LOGIN_OK;
//*/

///////////////////////////////
// FM_CONNECT
struct MSG_FM_CONNECT_OK
{
	char	ServerName[SIZE_MAX_SERVER_NAME];
	time_t	StartedTime;
	char	IPAddress[SIZE_MAX_IPADDRESS];
	USHORT	Port;
	USHORT	NumOfClients;
	bool	CalcBandwidth;
	USHORT	LoadedMapCounts;
};
using MSG_NM_CONNECT_OK = MSG_FM_CONNECT_OK;
using MSG_IM_CONNECT_OK = MSG_FM_CONNECT_OK;

///////////////////////////////
// PM_CONNECT
struct MSG_PM_CONNECT_OK
{
	char	ServerName[SIZE_MAX_SERVER_NAME];
	time_t	StartedTime;
	char	IPAddress[SIZE_MAX_IPADDRESS];
	USHORT	Port;
	USHORT	NumOfClients;
	bool	CalcBandwidth;
	USHORT	LoadedMapCounts;
};

///////////////////////////////
// IC_CHAT

struct MSG_IC_CHAT_BASE
{
	char	FromCharacterName[SIZE_MAX_CHARACTER_NAME];
	BYTE	NameColor;
	BYTE	MessageLength;
};

using MSG_IC_CHAT_MAP = MSG_IC_CHAT_BASE;
using MSG_IC_CHAT_REGION = MSG_IC_CHAT_BASE;
using MSG_IC_CHAT_PARTY = MSG_IC_CHAT_BASE;
using MSG_IC_CHAT_GUILD = MSG_IC_CHAT_BASE;
using MSG_IC_CHAT_ALL = MSG_IC_CHAT_BASE;
using MSG_IC_CHAT_CHATROOM = MSG_IC_CHAT_BASE;

#ifdef _SHOW_INFLUENCE_ON_GMNATION
struct MSG_IC_CHAT_WAR : MSG_IC_CHAT_BASE
{
	BYTE	InfluenceID;
};
#endif

/// Cross Nation Chat
struct MSG_IC_CHAT_CNC : MSG_IC_CHAT_BASE
{
	char	InfluenceOrStaffPrefix[SIZE_MAX_INFLUENCE_OR_STAFFPREFIX];	// Like BCU or CoMa
};

struct MSG_IC_CHAT_PTOP : MSG_IC_CHAT_BASE
{
	BYTE	FromInflTy;
	char	ToCharacterName[SIZE_MAX_CHARACTER_NAME];
};


struct MSG_IC_CHAT_GET_GUILD
{
	UID32_t	CharacterUniqueNumber;
};

struct MSG_IC_CHAT_GET_GUILD_OK
{
	UID32_t	CharacterUniqueNumber;
	char	CharacterName[SIZE_MAX_CHARACTER_NAME];
	BYTE	ConnectionCondition;			// ������,�޽���,����
};

struct MSG_IC_CHAT_CHANGE_GUILD
{
	UID32_t	CharacterUniqueNumber;
	char	CharacterName[SIZE_MAX_CHARACTER_NAME];
	char	Guild[SIZE_MAX_GUILD_NAME];
	BYTE	Joined;							// ���� ����
};

struct MSG_IC_CHAT_CHANGE_GUILD_OK
{
	UID32_t	CharacterUniqueNumber;
	char	CharacterName[SIZE_MAX_CHARACTER_NAME];
	char	Guild[SIZE_MAX_GUILD_NAME];
	BYTE	Joined;							// ���� ����
};

struct MSG_IC_CHAT_CHANGE_PARTY
{
	UID32_t	CharacterUniqueNumber;
	char	PartyName[20];
	BYTE	Joined;			// ����, Ż��
};

struct MSG_IC_CHAT_CHANGE_PARTY_OK
{
	UID32_t	CharacterUniqueNumber;
	char	PartyName[20];
	BYTE	Joined;			// ����, Ż��
};

struct MSG_IC_CHAT_CHANGE_CHAT_FLAG
{
	// 2008-05-15 by dhjin, EP3 - ä�� �ý��� ����
	//	BitFlag8_t	bitChatType;	// see below
	BitFlag16_t	bitChatType;	// see below
};

using MSG_IC_CHAT_SELL_ALL = MSG_IC_CHAT_BASE;
using MSG_IC_CHAT_CASH_ALL = MSG_IC_CHAT_BASE;
using MSG_IC_CHAT_INFLUENCE_ALL = MSG_IC_CHAT_BASE;			// 2006-04-21 by cmkwon
using MSG_IC_CHAT_ARENA = MSG_IC_CHAT_BASE;					// 2007-05-02 by dhjin
#ifndef _SHOW_INFLUENCE_ON_GMNATION
using MSG_IC_CHAT_WAR = MSG_IC_CHAT_BASE;					// 2008-05-19 by dhjin, EP3 - ä�� �ý��� ����, ���� ä��
#endif
using MSG_IC_CHAT_INFINITY = MSG_IC_CHAT_BASE;				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ä��
using MSG_FC_CHAT_CNC = MSG_IC_CHAT_CNC;

// 2008-05-15 by dhjin, EP3 - ä�� �ý��� ����
//	#define CHAT_TYPE_UNCONDITIONAL	(BitFlag8_t)0x01	// disable �Ұ�, (���� ����, ���� ��ü ä��, ����ä��)
//	#define CHAT_TYPE_SELL_ALL		(BitFlag8_t)0x02	// disable ����, �Ÿ� ��ü ä��
//	#define CHAT_TYPE_MAP			(BitFlag8_t)0x04	// disable ����, �� ä��
//	#define CHAT_TYPE_PTOP			(BitFlag8_t)0x08	// disable ����, 1:1 ä�� 
//	#define CHAT_TYPE_GUILD			(BitFlag8_t)0x10	// disable ����, ��� ä��
//	#define CHAT_TYPE_PARTY			(BitFlag8_t)0x20	// disable ����, ��Ƽ ä��
//	#define CHAT_TYPE_INFLUENCE		(BitFlag8_t)0x40	// disable �Ұ�
//	#define CHAT_TYPE_ARENA			(BitFlag8_t)0x80	// disable �Ұ�, ARENA ä��
#define CHAT_TYPE_UNCONDITIONAL	(BitFlag16_t)0x0001	// disable �Ұ�, (���� ����, ���� ��ü ä��, ����ä��)
#define CHAT_TYPE_SELL_ALL		(BitFlag16_t)0x0002	// disable ����, �Ÿ� ��ü ä��
#define CHAT_TYPE_MAP			(BitFlag16_t)0x0004	// disable ����, �� ä��
#define CHAT_TYPE_PTOP			(BitFlag16_t)0x0008	// disable ����, 1:1 ä�� 
#define CHAT_TYPE_GUILD			(BitFlag16_t)0x0010	// disable ����, ��� ä��
#define CHAT_TYPE_PARTY			(BitFlag16_t)0x0020	// disable ����, ��Ƽ ä��
#define CHAT_TYPE_INFLUENCE		(BitFlag16_t)0x0040	// disable �Ұ�
#define CHAT_TYPE_ARENA			(BitFlag16_t)0x0080	// disable �Ұ�, ARENA ä��
#define CHAT_TYPE_WAR			(BitFlag16_t)0x0100	// 2008-05-15 by dhjin, EP3 - ä�� �ý��� ����,	���� ä�� 
#define CHAT_TYPE_CHATROOM		(BitFlag16_t)0x0200	// 2008-05-15 by dhjin, EP3 - ä�� �ý��� ����, ä�ù� 
#define CHAT_TYPE_SYSTEM		(BitFlag16_t)0x0400	// 2008-05-15 by dhjin, EP3 - ä�� �ý��� ����, �ý��� 
#define CHAT_TYPE_HELPDESK		(BitFlag16_t)0x0800	// 2008-05-15 by dhjin, EP3 - ä�� �ý��� ����, ����
#define CHAT_TYPE_CNC			(BitFlag16_t)0x1000 // 2014-08-03 by Future, adding Cross Nation Chat

#define COMPARE_CHATTYPE_BIT(VAR, MASK)	(((VAR) & (MASK)) != 0)
#define CLEAR_CHATTYPE_BIT(VAR, MASK)	{ (VAR) &= ~(MASK); }
#define SET_CHATTYPE_BIT(VAR, MASK)		{ (VAR) |= (MASK); }

struct MSG_IC_CHAT_POSITION
{
	UID32_t		CharacterUniqueNumber;
	AVECTOR3	PosVector;
};

enum FRIEND_TYPE
{
	FRIEND_TYPE_FRIEND = 0,
	FRIEND_TYPE_REJECT = 1
};

struct DB_FRIEND_INFO
{
	char	CharacterName[SIZE_MAX_CHARACTER_NAME];
	char	FriendName[SIZE_MAX_CHARACTER_NAME];
	BYTE	FriendType;
	ATUM_DATE_TIME	RegDate;	// 2008-04-11 by dhjin, EP3 �źθ�� -
};

struct FRIENDINFO
{
	char		szCharacterName[SIZE_MAX_CHARACTER_NAME];
	BYTE		byIsOnline;
	ATUM_DATE_TIME	RegDate;	// 2008-04-11 by dhjin, EP3 �źθ�� -
};

struct MSG_IC_CHAT_FRIENDLIST_LOADING_OK
{
	int			nFriendListCounts;
	ARRAY_(FRIENDINFO);
};

struct MSG_IC_CHAT_FRIENDLIST_INSERT
{
	char szCharacterName[SIZE_MAX_CHARACTER_NAME];
};

using MSG_IC_CHAT_FRIENDLIST_INSERT_OK = FRIENDINFO;

using MSG_IC_CHAT_FRIENDLIST_DELETE = MSG_IC_CHAT_FRIENDLIST_INSERT;

using MSG_IC_CHAT_REJECTLIST_DELETE_OK = MSG_IC_CHAT_FRIENDLIST_INSERT;

struct MSG_IC_CHAT_FRIENDLIST_INSERT_NOTIFY		// 2009-01-13 by cmkwon, ģ�� ��Ͻ� ���濡�� �˸� �ý��� ���� - 
{
	char szDoerCharacName[SIZE_MAX_CHARACTER_NAME];	// ģ�� ��Ͻ� ������ �¶��� ������ ��� �˸�
};

using MSG_IC_CHAT_FRIENDLIST_REFRESH_OK = MSG_IC_CHAT_FRIENDLIST_LOADING_OK;

struct MSG_IC_CHAT_REJECTLIST_LOADING_OK
{
	int			nRejectListCounts;
	ARRAY_(FRIENDINFO);
};

using MSG_IC_CHAT_REJECTLIST_INSERT = MSG_IC_CHAT_FRIENDLIST_INSERT;

//////////////////////////////////////////////////////////////////////////
// 2008-04-11 by dhjin, EP3 �źθ�� - �ذ� ���� ����
// typedef MSG_IC_CHAT_FRIENDLIST_INSERT		MSG_IC_CHAT_REJECTLIST_INSERT_OK;
struct MSG_IC_CHAT_REJECTLIST_INSERT_OK
{
	char szCharacterName[SIZE_MAX_CHARACTER_NAME];
	ATUM_DATE_TIME	RegDate;
};

using MSG_IC_CHAT_REJECTLIST_DELETE = MSG_IC_CHAT_FRIENDLIST_INSERT;

using MSG_IC_CHAT_FRIENDLIST_DELETE_OK = MSG_IC_CHAT_FRIENDLIST_INSERT;



///////////////////////////////
// FC_CHAT
using MSG_FC_CHAT_MAP = MSG_IC_CHAT_BASE;
using MSG_FC_CHAT_REGION = MSG_IC_CHAT_REGION;
using MSG_FC_CHAT_CASH_ALL = MSG_IC_CHAT_CASH_ALL;
using MSG_FC_CHAT_ARENA = MSG_IC_CHAT_ARENA;
using MSG_FC_CHAT_OUTPOST_GUILD = MSG_IC_CHAT_BASE;	// 2007-10-06 by cmkwon, ���� ���� ������ ������ ���� ä�� ����
using MSG_FC_CHAT_INFINITY = MSG_IC_CHAT_INFINITY;		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ä��
using MSG_FI_CHAT_CNC = MSG_IC_CHAT_CNC;	// 2015-07-20 Future, CnC Costs Money

struct MSG_FC_CHAT_ALL_INFLUENCE		// 2007-08-09 by cmkwon, ��� ���¿� ä�� �����ϱ� - ����ü �߰�
{
	char	FromCharacterName[SIZE_MAX_CHARACTER_NAME];
	char	str256ChatMsg[SIZE_MAX_STRING_256];			// 2007-08-09 by cmkwon, ��� ���¿� ä�� �����ϱ� - 
#ifdef _SHOW_SPEAKER_IN_CHAT
	BYTE		nInfluence;
#endif
};

///////////////////////////////
// FI_CHAT
struct MSG_FI_CHAT_MAP
{
	UID32_t	CharacterUniqueNumber;
	BYTE	MessageLength;
	//�ڿ� �޽����� �ٿ��� ������
	//char*	ChatMessage;
};

using MSG_FI_CHAT_REGION = MSG_FI_CHAT_MAP;

struct MSG_FI_CHAT_CHANGE_CHAT_FLAG
{
	UID32_t		CharacterUniqueNumber;
	// 2008-05-15 by dhjin, EP3 - ä�� �ý��� ����
	//	BitFlag8_t	ChatFlag;				// ä�� ���� flag
	BitFlag16_t	ChatFlag;				// ä�� ���� flag
};

using MSG_FI_CHAT_CASH_ALL = MSG_FI_CHAT_MAP;
using MSG_FI_CHAT_ARENA = MSG_FI_CHAT_MAP;		// 2007-05-02 by dhjin
using MSG_FI_CHAT_OUTPOST_GUILD = MSG_FI_CHAT_MAP;		// 2007-10-06 by cmkwon, ���� ���� ������ ������ ���� ä�� ����
using MSG_FI_CHAT_INFINITY = MSG_FI_CHAT_MAP;		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ä��

struct MSG_FI_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT_OK		// 2006-07-18 by cmkwon
{
	UID32_t		CharacterUniqueNumber;
	int			OnlineFriendCnts;			// 2006-07-18 by cmkwon, ���� ����� �¶��� ģ�� ī��Ʈ	
};

struct MSG_IC_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT
{// 2008-07-11 by dhjin, EP3 ģ����� -
	int			OnlineFriendCnts;
};

///////////////////////////////
// FC_CHARACTER
struct MSG_FC_CHARACTER_GET_CHARACTER
{
	UID32_t		AccountUniqueNumber;
	UID32_t		CharacterUniqueNumber;
	// START 2011-11-03 by shcho, yedang �˴ٿ��� ���� - ���������� �߰��� �޾ƿ´�.
	bool     ShutDownMINS;
	// END 2011-11-03 by shcho, yedang �˴ٿ��� ���� - ���������� �߰��� �޾ƿ´�.
};

struct MSG_FC_CHARACTER_GET_CHARACTER_OK
{
	CHARACTER	Character;
	//char		FieldServerIP[SIZE_MAX_IPADDRESS];		// MSG_FC_CHARACTER_GAMESTART_OK�� �̵�
};

struct MSG_FC_CHARACTER_CREATE
{
	UID32_t		AccountUniqueNumber;
	char		CharacterName[SIZE_MAX_CHARACTER_NAME];
	USHORT		UnitKind;
	USHORT		Race;
	BYTE		AutoStatType1;
	GEAR_STAT	GearStat1;			// ��� ����
	BYTE		Color;
	BYTE		PilotFace;
	BYTE		InfluenceType0;		// 2005-06-23 by cmkwon, ���� Ÿ������ ���߿� ��� ���� �ʵ�
};

struct MSG_FC_CHARACTER_CREATE_OK
{
	CHARACTER				Character;
	CHARACTER_RENDER_INFO	CharacterRenderInfo;		// 2006-01-23 by cmkwon, �߰���
};

struct MSG_FC_CHARACTER_DELETE
{
	UID32_t	AccountUniqueNumber;
	UID32_t	CharacterUniqueNumber;
	char	CurrentSecPassword[SIZE_MAX_PASSWORD_MD5_STRING];	// 2007-09-13 by cmkwon, ��Ʈ�� 2���н����� ���� - MSG_FC_CHARACTER_DELETE �� �ʵ� �߰�
};

/* This message has NO body
//typedef struct
//{
//} MSG_FC_CHARACTER_DELETE_OK;
*/

struct MSG_FC_CHARACTER_REPAIR
{
	UID32_t	CharacterUniqueNumber;
};

struct MSG_FC_CHARACTER_REPAIR_OK
{
	INT	RepairCharge;
	INT	Remainder;
};


struct MSG_FC_CHARACTER_REPAIR_ERR
{
	BYTE	ErrCode;
};

// Repair Error Code
#define ERROR_REPAIR_NOERROR			0x00
#define ERROR_REPAIR_NOT_ENOUGH_MONEY	0x01

struct MSG_FC_CHARACTER_DOCKING
{
	UINT	ItemIndex;
};

struct MSG_FC_CHARACTER_UNDOCKING
{
	UINT	 ItemIndex;
};

struct MSG_FC_CHARACTER_DOCKING_ERR
{
	BYTE	 ErrCode;
};

// Docking Error Code
#define ERROR_DOCKING_NOERROR			0x00
#define ERROR_DOCKING_NO_SUCH_ITEM		0x01
#define ERROR_DOCKING_INVALID_POSITION	0x02

struct MSG_FC_CHARACTER_GAMESTART
{
	ClientIndex_t	ClientIndex;
	UID32_t			CharacterUniqueNumber;
};

struct MSG_FC_CHARACTER_GAMESTART_OK
{
	ClientIndex_t	ClientIndex;
	char			FieldServerIP[SIZE_MAX_IPADDRESS];
	int				FieldServerPort;
	BYTE			CharacterMode0;
	AVECTOR3		PositionVector;
	BitFlag16_t		MapWeather;
	bool			bMemberPCBang;			// 2007-01-25 by cmkwon, ���� PC�� �÷���
	char			ServerGroupName0[SIZE_MAX_SERVER_NAME];			// 2007-04-09 by cmkwon
	char			MainORTestServerName[SIZE_MAX_SERVER_NAME];		// 2007-04-09 by cmkwon
	char			GamePublisher[SIZE_MAX_GAME_PUBLISHER_NAME];	// 2007-04-09 by cmkwon
	MAP_CHANNEL_INDEX MapInfo;				// 2007-12-12 by dhjin, ������ �̸� ������
	BodyCond_t		BodyCondition;			// 2008-04-01 by cmkwon, ���øʿ��� ���� ������ ��� ��Ȱ���Ѽ� ĳ���� ���·� ó�� - MSG_FC_CHARACTER_GAMESTART_OK �� �߰�
	float			CurrentHP;				// 2008-04-01 by cmkwon, ���øʿ��� ���� ������ ��� ��Ȱ���Ѽ� ĳ���� ���·� ó�� - MSG_FC_CHARACTER_GAMESTART_OK �� �߰�
	float			CurrentDP;				// 2008-04-01 by cmkwon, ���øʿ��� ���� ������ ��� ��Ȱ���Ѽ� ĳ���� ���·� ó�� - MSG_FC_CHARACTER_GAMESTART_OK �� �߰�
	float			CurrentEP;				// 2008-04-01 by cmkwon, ���øʿ��� ���� ������ ��� ��Ȱ���Ѽ� ĳ���� ���·� ó�� - MSG_FC_CHARACTER_GAMESTART_OK �� �߰�
	float			CurrentSP;				// 2008-04-01 by cmkwon, ���øʿ��� ���� ������ ��� ��Ȱ���Ѽ� ĳ���� ���·� ó�� - MSG_FC_CHARACTER_GAMESTART_OK �� �߰�
	DWORD			Padding[SIZE_MAX_PACKET_PADDING];				// 2011-07-21 by hskim, ���� ���� ���� - ���� ������ ȣȯ �ȵǵ��� ����ü ũ�� �ٲ�
};

struct MSG_FC_CHARACTER_CONNECT_GAMESTART
{
	char	AccountName[SIZE_MAX_ACCOUNT_NAME];
	char	Password[SIZE_MAX_PASSWORD_MD5_STRING];
	char	PrivateIP[SIZE_MAX_IPADDRESS];
	UID32_t	AccountUniqueNumber;
	UID32_t	CharacterUniqueNumber;
	DWORD	Padding[SIZE_MAX_PACKET_PADDING];				// 2011-07-21 by hskim, ���� ���� ���� - ���� ������ ȣȯ �ȵǵ��� ����ü ũ�� �ٲ�
};

struct MSG_FC_CHARACTER_CONNECT_GAMESTART_OK
{
	ClientIndex_t	ClientIndex;
	AVECTOR3		PositionVector;
	BitFlag16_t		MapWeather;
};

struct MSG_FC_CHARACTER_DEAD_GAMESTART
{
	ClientIndex_t	ClientIndex;
	UID32_t			CharacterUniqueNumber;
	bool			bRebirthInCityMap;			// TRUE:���º����ø� ��Ȱ, FALSE:����ʿ��� ��Ȱ
};

struct MSG_FC_CHARACTER_OTHER_REVIVED {
	UID32_t			CharacterUniqueNumber;
};	// F -> C, �׾��� �ǻ�Ƴ��� �� �ٸ� ĳ����(����� ��Ƽ��)���� ����

struct MSG_FC_CHARACTER_GAMEEND
{
	ClientIndex_t	ClientIndex;
	bool			SelectCharacterView;		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ɸ��� ���� â���� �̵��̸� TRUE	
};

struct MSG_FC_CHARACTER_GAMEEND_OK
{
	ClientIndex_t	ClientIndex;
};

struct MSG_FC_CHARACTER_CHANGE_UNITKIND
{
	ClientIndex_t	ClientIndex;
	USHORT			UnitKind;
};

struct MSG_FC_CHARACTER_CHANGE_STAT
{
	ClientIndex_t	ClientIndex;
	GEAR_STAT		GearStat1;		// ��� ����
	BYTE			BonusStat;		// ���� ���ʽ� ����
};
struct MSG_FC_CHARACTER_CHANGE_TOTALGEAR_STAT
{
	ClientIndex_t	ClientIndex;
	BYTE			byAutoStatType;
	GEAR_STAT		GearStat1;		// ��� ����
};

struct MSG_FC_CHARACTER_CHANGE_GUILD
{
	ClientIndex_t	ClientIndex;
	char			Guild[SIZE_MAX_GUILD_NAME];
};

struct MSG_FC_CHARACTER_CHANGE_LEVEL
{
	ClientIndex_t	ClientIndex;
	BYTE			Level;
	BYTE			BonusStat;
	// 2005-11-15 by cmkwon, ������
	//	BYTE			BonusSkillPoint;
};

struct MSG_FC_CHARACTER_CHANGE_EXP
{
	ClientIndex_t	ClientIndex;
	Experience_t	Experience;						// �� ����ġ
};

struct MSG_FC_CHARACTER_CHANGE_BODYCONDITION
{
	ClientIndex_t	ClientIndex;
	BodyCond_t		BodyCondition;					// ���� 32bit�� ��� <-- check: ��ų bodycon�� �������� �����ϱ�!
};

struct MSG_FC_CHARACTER_CHANGE_BODYCONDITION_SKILL
{
	ClientIndex_t	ClientIndex;
	BodyCond_t		SkillBodyCondition;				// ���� 32bit�� ��� <-- check: ��ų bodycon�� �������� �����ϱ�!
};

struct MSG_FC_CHARACTER_CHANGE_INFLUENCE_TYPE
{
	ClientIndex_t	ClientIndex;
	BYTE			InfluenceType0;					// 
};

struct MSG_FC_CHARACTER_CHANGE_BODYCONDITION_ALL
{
	ClientIndex_t	ClientIndex;
	BodyCond_t		BodyCondition;					// ���� 32bit�� ��� <-- check: ��ų bodycon�� �������� �����ϱ�!
};		// C->F, ������ BodyCondition ���� ��û

struct MSG_FC_CHARACTER_CHANGE_PROPENSITY
{
	ClientIndex_t	ClientIndex;
	BYTE			byUpdateType;					// 2005-08-22 by cmkwon, ����� �ΰ�(IUT_PENALTY_ON_DEAD, IUT_PENALTY_AGEAR_FUEL_ALLIN) - IUT_XXX, ITEM_UPDATE_TYPE_XXX
	INT				Propensity;
};				// 2005-08-22 by cmkwon, 

struct MSG_FC_CHARACTER_CHANGE_STATUS
{
	ClientIndex_t	ClientIndex;
	BYTE			Status;
};

struct MSG_FC_CHARACTER_CHANGE_PKPOINT
{
	ClientIndex_t	ClientIndex;
	INT				Propensity;
	USHORT			PKWinPoint;
	USHORT			PKLossPoint;
};

struct MSG_FC_CHARACTER_CHANGE_HPDPSPEP
{
	ClientIndex_t	ClientIndex;
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

struct MSG_FC_CHARACTER_CHANGE_CURRENTHPDPSPEP
{
	ClientIndex_t	ClientIndex;
	float	CurrentHP;
	float	CurrentDP;
	SHORT	CurrentSP;
	float	CurrentEP;
};

struct MSG_FC_CHARACTER_CHANGE_CURRENTHP
{
	ClientIndex_t	ClientIndex;
	float			CurrentHP;
};

struct MSG_FC_CHARACTER_CHANGE_CURRENTDP
{
	ClientIndex_t	ClientIndex;
	float			CurrentDP;
};

struct MSG_FC_CHARACTER_CHANGE_CURRENTSP
{
	ClientIndex_t	ClientIndex;
	SHORT			CurrentSP;
};

struct MSG_FC_CHARACTER_CHANGE_CURRENTEP
{
	ClientIndex_t	ClientIndex;
	float			CurrentEP;
};

struct MSG_FC_CHARACTER_CHANGE_MAPNAME
{
	ClientIndex_t		ClientIndex;
	MAP_CHANNEL_INDEX	MapChannelIndex;
};

struct MSG_FC_CHARACTER_CHANGE_PETINFO
{
	ClientIndex_t	ClientIndex;
	BYTE			Level;
	Experience_t	Experience;
};

struct MSG_FC_CHARACTER_CHANGE_POSITION
{
	ClientIndex_t	ClientIndex;
	AVECTOR3		PositionVector;		// ĳ���� ��ǥ
};

struct MSG_FC_CHARACTER_USE_BONUSSTAT
{
	ClientIndex_t	ClientIndex;
	BYTE	KindOfStat;	// see below
};

struct MSG_FC_CHARACTER_USE_BONUSSTAT_OK		// 2006-09-18 by cmkwon
{
	BYTE	byReaminBonusStat;
};

struct MSG_FC_CHARACTER_DEAD_NOTIFY
{
	ClientIndex_t	ClientIndex;
	BYTE			byDamageKind;
	bool			bDeadByP2PPK;	// P2PPK ������ ����
};


#define COUNT_MAX_STAT_POINT			(CHARACTER_LEVEL_110_MAX_STAT_POINT)	// 2009-12-29 by cmkwon, ĳ���� �ִ� ���� ����(110����) - 100Lv�̻��� 340�� �ִ�

// Kind of Stat, STAT_XXX
#define STAT_ATTACK_PART				((BYTE)0)	// ���� ��Ʈ
#define STAT_DEFENSE_PART				((BYTE)1)	// ��� ��Ʈ
#define STAT_FUEL_PART					((BYTE)2)	// ���� ��Ʈ
#define STAT_SOUL_PART					((BYTE)3)	// ���� ��Ʈ
#define STAT_SHIELD_PART				((BYTE)4)	// ���� ��Ʈ
#define STAT_DODGE_PART					((BYTE)5)	// ȸ�� ��Ʈ
#define STAT_BONUS						((BYTE)6)	// ���ʽ��� �޴� stat
#define STAT_ALL_PART					((BYTE)7)	// ��� ��Ʈ
#define STAT_BONUS_STAT_POINT			((BYTE)8)	// 2007-06-20 by cmkwon, ���ʽ� ���� ���� �α� ����� - ���ʽ��� �޴� statPoint

///////////////////////////////////////////////////////////////////////////////
// ĳ�� ������ Level 20������ 
#define AUTOSTAT_TYPE_FREESTYLE					0	// ����� ������
#define AUTOSTAT_TYPE_BGEAR_ATTACK				1	// B-Gear ������
#define AUTOSTAT_TYPE_BGEAR_MULTI				2	// B-Gear ��Ƽ��
#define AUTOSTAT_TYPE_IGEAR_ATTACK				3	// I-Gear ������
#define AUTOSTAT_TYPE_IGEAR_DODGE				4	// I-Gear ȸ����
#define AUTOSTAT_TYPE_AGEAR_ATTACK				5	// A-Gear ������
#define AUTOSTAT_TYPE_AGEAR_SHIELD				6	// A-Gear ������
#define AUTOSTAT_TYPE_MGEAR_DEFENSE				7	// M-Gear �����
#define AUTOSTAT_TYPE_MGEAR_SUPPORT				8	// M-Gear ������

struct MSG_FC_CHARACTER_GET_OTHER_INFO
{
	ClientIndex_t		ClientIndex;
};

#define CITYWAR_TEAM_TYPE_NORMAL				0	// �������������� �ƴϰų� ������������ ���۾ȵ�
#define CITYWAR_TEAM_TYPE_ATTACKER				1	// ������, ������
#define CITYWAR_TEAM_TYPE_DEFENSER				2	// �����, 

struct MEX_OTHER_CHARACTER_INFO
{
	// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - �Ʒ��� ���� �ʵ� ������ �����Ѵ�.
	BodyCond_t		BodyCondition;
	INT				Propensity;			// 2005-12-27 by cmkwon, ����(��,��)�� �ƴϰ� ������ �����
	UID32_t			CharacterUniqueNumber;
	// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - ������� �����Ƿ� ����
	//	INT				RacingPoint;		// Racing ��� Point
	UID32_t			GuildUniqueNumber;
	MAP_CHANNEL_INDEX	MapChannelIndex;		// ĳ���Ͱ� ���� �� �� ä��, 2006-01-18 by cmkwon, �߰���
	AVECTOR3		PositionVector;
	AVECTOR3		TargetVector;
	AVECTOR3		UpVector;
	ClientIndex_t	ClientIndex;
	USHORT			Race;
	USHORT			UnitKind;
	USHORT			PKWinPoint;			// PK �¸� ��ġ
	USHORT			PKLossPoint;		// PK �й� ��ġ
	char			CharacterName[SIZE_MAX_CHARACTER_NAME];
	char			szCharacterMent[SIZE_STRING_32];
	BYTE			Color;				// false(0) : ��, true(1) : ��
	BYTE			PilotFace;			// ȭ�鿡 ��Ÿ���� �ι� ĳ����
	BYTE			CharacterMode0;		// 2005-07-13 by cmkwon, ���� ĳ���� ���� �÷���
	BYTE			InfluenceType;		// ���� Ÿ��, 2005-06-23 by cmkwon
	BYTE			Level1;				//
	BYTE			CityWarTeamType;	// 
	BYTE			Status;				// �ź�

	BYTE			UsingReturnItem;				// 2013-02-28 by bckim, �������� �����߰�

													// operator overloading
	MEX_OTHER_CHARACTER_INFO& operator=(const CHARACTER& rhs)
	{
		ClientIndex = rhs.ClientIndex;
		CharacterUniqueNumber = rhs.CharacterUniqueNumber;
		util::strncpy(CharacterName, rhs.CharacterName, SIZE_MAX_CHARACTER_NAME);
		Color = rhs.Color;
		PilotFace = rhs.PilotFace;
		CharacterMode0 = rhs.CharacterMode;
		InfluenceType = rhs.InfluenceType;
		Race = rhs.Race;
		UnitKind = rhs.UnitKind;
		Level1 = rhs.Level;
		Propensity = rhs.Propensity;
		Status = rhs.Status;
		PKWinPoint = rhs.PKWinPoint;
		PKLossPoint = rhs.PKLossPoint;
		// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - ������� �����Ƿ� ����
		//		RacingPoint				= rhs.RacingPoint;
		GuildUniqueNumber = rhs.GuildUniqueNumber;
		BodyCondition = rhs.BodyCondition;
		MapChannelIndex = rhs.MapChannelIndex;
		PositionVector = rhs.PositionVector;
		TargetVector = rhs.TargetVector*1000.0f;
		UpVector = rhs.UpVector;

		UsingReturnItem = rhs.bUsingReturnItem;		// 2013-02-28 by bckim, �������� �����߰�

		return *this;
	}
};

struct MSG_FC_CHARACTER_GET_OTHER_INFO_OK
{
	MEX_OTHER_CHARACTER_INFO	CharacterInfo;
	CHARACTER_RENDER_INFO		CharacterRenderInfo;
};

struct MSG_FC_CHARACTER_GET_MONSTER_INFO_OK
{
	ClientIndex_t	MonsterIndex;
	INT				CurrentHP;
	INT				MonsterUnitKind;
	SHORT			MonsterForm;
	AVECTOR3		PositionVector;
	AVECTOR3		TargetVector;
	BodyCond_t		BodyCondition;
	INT				MaxHP;
};

struct MSG_FC_CHARACTER_GET_ACCOUNTUNIQUENUMBER
{
	char		CharacterName[SIZE_MAX_CHARACTER_NAME];	// ��ȭ ���
};				// ��ȭ ��û �� �ʿ���

struct MSG_FC_CHARACTER_GET_ACCOUNTUNIQUENUMBER_OK
{
	char		CharacterName[SIZE_MAX_CHARACTER_NAME];	// ��ȭ ���
	UID32_t		AccountUniqueNumber;					// ��ȭ ���
};			// ��ȭ ��û �� �ʿ���

			// �浹 Ÿ��
#define COLLISION_TYPE_LAND			(BYTE)0x00	// ��, �� ���� �������� �浹
#define COLLISION_TYPE_BUILDING		(BYTE)0x01	// �ǹ����� �浹
#define COLLISION_TYPE_CHARACTER	(BYTE)0x02	// �ٸ� ĳ���Ϳ��� �浹
#define COLLISION_TYPE_MONSTER		(BYTE)0x03	// ���Ϳ��� �浹
struct MSG_FC_CHARACTER_APPLY_COLLISION_DAMAGE
{
	ClientIndex_t	UnitIndex;								// ���Ϳ� �浹�ô� �浹 ���� Index, ĳ���� �浹�ô� �浹 ĳ���� Index
	SHORT			SpeedOfCollision;						// �浹�� ���� ���ǵ�
	BYTE			CollisionType;							// �浹 Ÿ��, see below
};


struct MSG_FC_CHARACTER_GET_OTHER_MOVE
{
	ClientIndex_t	OtherClientIndex;
};			// C -> F, �ٸ� ������ MOVE ������ ��û��, ������ MSG_FC_MOVE

struct MSG_FC_CHARACTER_DELETE_OTHER_INFO
{
	ClientIndex_t	OtherClientIndex;
};		// F -> C,	Ŭ���̾�Ʈ���� �ٸ� ����(OTHER_INFO)�� ������ ��û��

struct MSG_FC_CHARACTER_GET_OTHER_RENDER_INFO {
	ClientIndex_t	ClientIndex;
};

struct MSG_FC_CHARACTER_GET_OTHER_RENDER_INFO_OK
{
	ClientIndex_t			ClientIndex;
	CHARACTER_RENDER_INFO	CharacterRenderInfo;
};

//typedef struct {
//	ClientIndex_t			ClientIndex;
//	USHORT					UnitState;
//} MSG_FC_CHARACTER_PUT_OTHER_EFFECT;		// F->C, �ٸ� ������ ���� ������ ����, ���� ����Ʈ � ���, check: ���ŵ�, 20040626, kelovon
//
//#define UNIT_EFFECT_NO_EFFECT		((USHORT)0)
//#define UNIT_EFFECT_HP_UP			((USHORT)1)
//#define UNIT_EFFECT_DP_UP			((USHORT)2)
//#define UNIT_EFFECT_SP_UP			((USHORT)3)
//#define UNIT_EFFECT_EP_UP			((USHORT)4)

struct MSG_FC_CHARACTER_SHOW_EFFECT
{
	UINT			EffectIndex;	// Ŭ���̾�Ʈ�� ���ǵ� effect�� index
};		// C->F, ������ �ڽ��� ����Ʈ ���� ��û

struct MSG_FC_CHARACTER_SHOW_EFFECT_OK
{
	ClientIndex_t	ClientIndex;
	UINT			EffectIndex;	// Ŭ���̾�Ʈ�� ���ǵ� effect�� index
};	// F->C, ������ ĳ���鿡�� ����

struct MSG_FC_CHARACTER_GET_OTHER_PARAMFACTOR
{
	ClientIndex_t	ClientIndex;
	BYTE			ItemPosition;				// POS_XXX
};		// C->F, �ش� ĳ������ ParamFactor ���� ��û

struct DES_PARAM_VALUE
{
	BYTE			DestParameter;				// ����Ķ����, DES_XXX
	float			ParameterValue;				// �����Ķ����
};

struct MSG_FC_CHARACTER_GET_OTHER_PARAMFACTOR_OK
{
	ClientIndex_t	ClientIndex;
	BYTE			ItemPosition;				// POS_XXX
	INT				ItemNum;
	INT				NumOfParamValues;
	ARRAY_(DES_PARAM_VALUE);
};	// F->C, �ش� ĳ������ ParamFactor ���� ��û ���

struct MSG_FC_CHARACTER_SEND_PARAMFACTOR_IN_RANGE
{
	BYTE			ItemPosition;				// POS_XXX
};	// C->F, �ڽ��� ParamFactor�� ������ �������� ��û

struct MSG_FC_CHARACTER_GET_OTHER_SKILL_INFO
{
	ClientIndex_t	ClientIndex;				// ������ ClientIndex
};		// C->F

struct MSG_FC_CHARACTER_GET_OTHER_SKILL_INFO_OK
{
	ClientIndex_t	ClientIndex;				// ������ ClientIndex
	INT				NumOfSkillInfos;
	ARRAY_(MEX_OTHER_SKILL_INFO);
};		// F->C

struct MSG_FC_CHARACTER_SPEED_HACK_USER
{
	int				nSendMoveCounts;			// ������ ������ Move ��Ŷ ī��Ʈ
	DWORD			dwTimeGap;					// �ð�(���� ms)
};

struct MSG_FC_CHARACTER_CHANGE_CHARACTER_MENT
{
	ClientIndex_t	ClientIdx;
	char			szCharacterMent1[SIZE_STRING_32];
};

struct MSG_FC_CHARACTER_GET_CASH_MONEY_COUNT_OK
{
	int				nMCash;						// MCash
	int				nGiftCard;					// ��ǰ��
};

struct MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO
{
	INT				nCardItemNum1;
	ATUM_DATE_TIME	atumTimeUpdatedTime1;	// ������ �ð�
	ATUM_DATE_TIME	atumTimeExpireTime1;	// ���� �ð�
	float			fExpRate1;
	float			fDropRate1;
	float			fDropRareRate1;
	float			fExpRepairRate1;
};

struct MSG_FC_CHARACTER_TUTORIAL_SKIP		// 2006-10-13 by cmkwon
{
	UID32_t				CharacterUniqueNumber;
};

struct MSG_FC_CHARACTER_TUTORIAL_SKIP_OK
{
	ClientIndex_t		ClientIndex;
	UID32_t				CharacterUniqueNumber;
	MAP_CHANNEL_INDEX	mapChannelIdx;
};

struct MEX_OTHER_SKILL_INFO
{
	INT		SkillItemNum;						// Skill�� ItemNum
};

// 2005-07-26 by hblee : �����忡�� ĳ���� ��� ��ȯ.
struct MSG_FC_CHARACTER_CHANGE_CHARACTER_MODE
{
	BYTE				CharacterMode0;
	AVECTOR3			PositionAVec3;
	AVECTOR3			TargetAVec3;
};

struct MSG_FC_CHARACTER_CHANGE_CHARACTER_MODE_OK
{
	ClientIndex_t		ClientIndex;
	BYTE				CharacterMode0;
	AVECTOR3			PositionAVec3;
	AVECTOR3			TargetAVec3;
};

struct MSG_FC_CHARACTER_GET_REAL_WEAPON_INFO_OK		// 2005-12-21 by cmkwon
{
	UID64_t		ItemUID0;
	float		ShotNum0;
	float		MultiNum0;
	float		ReattackTime0;
	float		RepeatTime0;
	float		Time0;
	float		RangeAngle0;
	float		BoosterAngle0;
	float		OrbitType0;
};

struct MSG_FC_CHARACTER_GET_REAL_ENGINE_INFO_OK		// 2005-12-21 by cmkwon
{
	UID64_t		ItemUID0;
	float		AbilityMin0;
	float		AbilityMax0;
	float		SpeedPenalty0;
	float		Time0;
	float		Range0;
	float		RangeAngle0;
	float		BoosterAngle0;
};

struct MSG_FC_CHARACTER_GET_REAL_TOTAL_WEIGHT_OK		// 2005-12-21 by cmkwon
{
	UID64_t		ItemUID0;				// �Ƹ��� ItemUID
	float		Transport0;				// ���� �ɷ�
	float		TotalWeight0;			// ���� ���� �ѷ�
};

struct MSG_FC_CHARACTER_MEMORY_HACK_USER				// 2005-12-22 by cmkwon
{
	UID64_t		ItemUID0;				// ������ ItemUID
	float		ValidMoveDistance;		// ���� ��ȿ �ִ� �̵� �Ÿ� - TickGap�ð� ���� ������ �ִ� �Ÿ�
	float		CurrentMoveDistance;	// �̵��� �Ÿ� - TickGap�ð� ���� �̵��� �Ÿ�
	int			TickGap;				// ����� �ð�(����:ms, ex> 1��= 1000, 0.5��= 500)
};

struct MSG_FC_CHARACTER_UPDATE_SUBLEADER
{// 2007-02-13 by dhjin, �������� ���� ���ν���, �� ��° ������������ üũ�� �ʿ�
	BYTE			InflType;
	BYTE			SubLeaderRank;		// 2007-10-06 by dhjin, �� ��° ������������
	char			CharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_FC_CHARACTER_UPDATE_SUBLEADER_OK
{// 2007-10-06 by dhjin, �������� ������ ������ Ŭ���̾�Ʈ�� ����
	BYTE			SubLeaderRank;		// 2007-10-06 by dhjin, �� ��° ������������
	char			CharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_FC_CHARACTER_OBSERVER_TARGET_CHARACTERINDEX
{// 2007-03-27 by dhjin, ������ ��� ���� �ε��� 
	ClientIndex_t		TargetClientIndex;
};
using MSG_FC_CHARACTER_OBSERVER_START = MSG_FC_CHARACTER_OBSERVER_TARGET_CHARACTERINDEX;
using MSG_FC_CHARACTER_OBSERVER_END = MSG_FC_CHARACTER_OBSERVER_TARGET_CHARACTERINDEX;


struct MSG_FC_CHARACTER_OBSERVER_INFO
{// 2007-03-27 by dhjin, ���������� �����Ǵ� ����
	ClientIndex_t		ClientIndex;
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	INT		HP;
	INT		DP;
#else
	SHORT	HP;
	SHORT	DP;
#endif
	SHORT				SP;
	SHORT				EP;
	float				CurrentHP;
	float				CurrentDP;
	SHORT				CurrentSP;
	float				CurrentEP;
};

struct MSG_FC_CHARACTER_OBSERVER_REG
{// 2007-03-27 by dhjin, ������ ������ ��� ���� ����ϴ� ����ü
	ClientIndex_t		ClientIndex;
	SHORT				nRegNum;
};

struct MSG_FC_CHARACTER_SHOW_MAP_EFFECT		// 2007-04-20 by cmkwon
{
	UINT			EffectIndex;			// Ŭ���̾�Ʈ�� ���ǵ� effect�� index
	AVECTOR3		avec3Up;				// effect�� Up Vector, Normalize �ؾ���
	AVECTOR3		avec3Target;			// effect�� Target Vector, Normalize �ؾ���
	AVECTOR3		avec3Position;			// effect�� Positon Vector
	INT				nLifetime;				// effect�� ��ȿ�ð�(����:ms) - 0 �����̸� ������� ���� effect ��
};

struct MSG_FC_CHARACTER_SHOW_MAP_EFFECT_OK		// 2007-04-20 by cmkwon
{
	ClientIndex_t	ClientIdx;
	UINT			EffectIndex;			// Ŭ���̾�Ʈ�� ���ǵ� effect�� index
	AVECTOR3		avec3Up;				// effect�� Up Vector, Normalize �ؾ���(=����UpVector*1000f)
	AVECTOR3		avec3Target;			// effect�� Target Vector, Normalize �ؾ���(=����TargetVector*1000f)
	AVECTOR3		avec3Position;			// effect�� Positon Vector
	INT				nLifetime;				// effect�� ��ȿ�ð�(����:ms) - 0 �����̸� ������� ���� effect ��
};

struct MSG_FC_CHARACTER_PAY_WARPOINT
{// 2007-05-16 by dhjin, WarPoint�� ���޵Ǿ� ���� �Ѵ�.
	INT				WarPoint;				// 2007-05-16 by dhjin, ���޵� WarPoint
	INT				TotalWarPoint;			// 2007-05-16 by dhjin, �� WarPoint
	INT				CumulativeWarPoint;		// 2007-05-28 by dhjin, ���� WarPoint
	bool			UseItemFlag;			// 2010-08-27 by shcho&&jskim, WARPOINT ���� ������ ���� - ������ ���ȹ��(TRUE)���� ���ӿ��� ȹ��(FALSE)���� ����
	bool			ShowSystemMessage;		// 2015-07-22 Future, make System message hideable
};

struct MSG_FC_CHARACTER_WATCH_INFO
{// 2007-06-19 by dhjin, �����ڿ��� �����Ǵ� ����
	ClientIndex_t		ClientIndex;
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	INT		HP;
	INT		DP;
#else
	SHORT	HP;
	SHORT	DP;
#endif
	float				CurrentHP;
	float				CurrentDP;
};

struct MSG_FC_CHARACTER_GAMESTART_FROM_ARENA_TO_MAINSERVER
{// 2008-01-11 by dhjin, �Ʒ��� ���� - 
	ClientIndex_t		ClientIndex;
};

struct MSG_FC_CHARACTER_READY_GAMESTART_FROM_ARENA_TO_MAINSERVER
{// 2008-01-31 by dhjin, �Ʒ��� ���� - 
	ClientIndex_t		ClientIndex;
};

struct MSG_FC_CHARACTER_GET_USER_INFO
{// 2008-06-20 by dhjin, EP3 ���������ɼ� -
	UID32_t				TargetCharcterUID;
};

struct MSG_FC_CHARACTER_GET_USER_INFO_OK
{// 2008-06-20 by dhjin, EP3 ���������ɼ� -
	BYTE				PilotFace;						// ȭ�鿡 ��Ÿ���� �ι� �ɸ���
	char				CharacterName[SIZE_MAX_CHARACTER_NAME];		// ����(ĳ����) �̸�
	USHORT				UnitKind;						// ������ ����	
	UID32_t				GuildUID;
	char				GuildName[SIZE_MAX_GUILD_NAME];	// ��� �̸�
	BYTE				Level;
	MAP_CHANNEL_INDEX	MapChannelIndex;
	INT					Propensity;						// ��ġ, ����(��, ��)
	ATUM_DATE_TIME		LastStartedTime;				// ���� ���� ���� �ð�
	char				NickName[SIZE_MAX_CHARACTER_NAME];			// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 
};

// 2012-07-12 by isshin ���۹̸����� On / Off ��� - ���� ����
struct MSG_FC_CHARACTER_GET_USER_ITEM_INFO_SECRET
{
	UID32_t				TargetCharcterUID;
};
struct MSG_FC_CHARACTER_GET_USER_ITEM_INFO_SECRET_OK
{
	bool				bItemInfoSecret;
};
// end 2012-07-12 by isshin ���۹̸����� On / Off ��� - ���� ����

// 2012-06-05 by jhseol, ���۹̸����� - Ŭ���û ��Ŷ �߰�
struct MSG_FC_CHARACTER_GET_USER_ITEM_INFO
{
	UID32_t				TargetCharcterUID;
};
// end 2012-06-05 by jhseol, ���۹̸����� - Ŭ���û ��Ŷ �߰�

struct MSG_FC_CHARACTER_CHANGE_INFO_OPTION_SECRET
{// 2008-06-20 by dhjin, EP3 ���������ɼ� -
	INT					SecretInfoOption;
};

struct MSG_FC_CHARACTER_CHANGE_NICKNAME		// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 
{
	UID32_t				CharacUID;
	char				NickName[SIZE_MAX_CHARACTER_NAME];
};
using MSG_FC_CHARACTER_CHANGE_NICKNAME_OK = MSG_FC_CHARACTER_CHANGE_NICKNAME;	// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 

struct MSG_FC_CHARACTER_CHANGE_START_CITY_MAPINDEX		// 2009-10-12 by cmkwon, ������ī ���� ��� ���� - 
{
	UID32_t				CharacUID;
	MapIndex_t			nStartCityMapIdx;
};
using MSG_FC_CHARACTER_CHANGE_START_CITY_MAPINDEX_OK = MSG_FC_CHARACTER_CHANGE_START_CITY_MAPINDEX;	// 2009-10-12 by cmkwon, ������ī ���� ��� ���� - 


struct MSG_FC_CHARACTER_CHANGE_ADDED_INVENTORY_COUNT	// 2009-11-02 by cmkwon, ĳ��(�κ�/â�� Ȯ��) ������ �߰� ���� - 
{
	UID32_t				CharacUID;
	int					nRacingPoint;
};

// 2012-10-10 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
struct MSG_FC_CHARACTER_KILLCOUNT
{
	INT nCount;
};		// C->F

		//#if S_BONUSEXPSYSTEM_RENEWAL
struct MSG_FC_CHARACTER_RESTCOUNT
{
	INT nKillCount;
	INT nRestCount;
};		// C->F
		//#endif // S_BONUSEXPSYSTEM_RENEWAL

struct MSG_FC_CHARACTER_BONUS_EXP_RATE
{
	INT nPartyRate;
	INT nFriendRate;
	INT nGuildRate;
};		// C->F
		// end 2012-10-10 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
		///////////////////////////////////////////////////////////////////////////////
		// FN CHARACTER
struct MSG_FN_CHARACTER_CHANGE_CHARACTER_MODE_OK
{
	ChannelIndex_t		ChannelIndex;
	ClientIndex_t		ClientIndex;
	BYTE				CharacterMode0;
};

struct MSG_FN_CHARACTER_CHANGE_INFLUENCE_TYPE
{
	ChannelIndex_t		ChannelIndex;
	ClientIndex_t		ClientIndex;
	BYTE				InfluenceType0;				// 
};

struct MSG_FN_CLIENT_GAMEEND_OK : public MSG_FC_CHARACTER_GAMEEND_OK
{
	ChannelIndex_t		ChannelIndex;
};

struct MSG_FN_CHARACTER_CHANGE_UNITKIND : public MSG_FC_CHARACTER_CHANGE_UNITKIND
{
	ChannelIndex_t		ChannelIndex;
};

struct MSG_FN_CHARACTER_CHANGE_BODYCONDITION : public MSG_FC_CHARACTER_CHANGE_BODYCONDITION
{
	ChannelIndex_t		ChannelIndex;
};

struct MSG_FN_CHARACTER_CHANGE_HPDPSPEP : public MSG_FC_CHARACTER_CHANGE_HPDPSPEP
{
	ChannelIndex_t		ChannelIndex;
};

struct MSG_FN_CHARACTER_CHANGE_CURRENTHPDPSPEP : public MSG_FC_CHARACTER_CHANGE_CURRENTHPDPSPEP
{
	ChannelIndex_t		ChannelIndex;
};

struct MSG_FN_CHARACTER_CHANGE_CURRENTHP : public MSG_FC_CHARACTER_CHANGE_CURRENTHP
{
	ChannelIndex_t		ChannelIndex;
};

struct MSG_FN_CHARACTER_CHANGE_CURRENTSP : public MSG_FC_CHARACTER_CHANGE_CURRENTSP
{
	ChannelIndex_t		ChannelIndex;
};		// check: �ʿ��Ѱ�? ö�ξ����� Ȯ�� ��!

struct MSG_FN_CHARACTER_CHANGE_CURRENTEP : public MSG_FC_CHARACTER_CHANGE_CURRENTEP
{
	ChannelIndex_t		ChannelIndex;
};		// check: �ʿ��Ѱ�? ö�ξ����� Ȯ�� ��!
struct MSG_FN_CHARACTER_CHANGE_MAPNAME : public MSG_FC_CHARACTER_CHANGE_MAPNAME
{
	ChannelIndex_t		ChannelIndex;
};
struct MSG_FN_CHARACTER_CHANGE_PETINFO : public MSG_FC_CHARACTER_CHANGE_PETINFO
{
	ChannelIndex_t		ChannelIndex;
};
struct MSG_FN_CHARACTER_CHANGE_POSITION : public MSG_FC_CHARACTER_CHANGE_POSITION
{
	ChannelIndex_t		ChannelIndex;
};

struct MSG_FN_CHARACTER_CHANGE_STEALTHSTATE
{
	ChannelIndex_t		ChannelIndex;
	ClientIndex_t		ClientIndex;
	bool				bStealthState2;
};

struct MSG_FN_CHARACTER_CHANGE_INVISIBLE
{
	ChannelIndex_t		ChannelIndex;
	ClientIndex_t		ClientIndex;
	bool				bInvisible;
};

///////////////////////////////////////////////////////////////////////////////
// FC_MOVE ( Field server <-> Client)
struct MSG_FC_MOVE
{
	ClientIndex_t	ClientIndex;
	USHORT			TimeGap;
	BYTE			DistanceGap;		// Client�� ���� ������ ������
	AVECTOR3		PositionVector;
	AVECTOR3		TargetVector;
	AVECTOR3		UpVector;
};

struct MSG_FC_MOVE_BIT_FLAG					// 2007-03-29 by cmkwon
{
	bool			CharacterMode0 : 1;		// 
	bool			Invisible0 : 1;			// �κ����� ��ų ��� ����
	bool			ChargingSkill : 1;		// 2007-04-02 by cmkwon, A-Gear ��¡ ��ų ��� ����
	bool			HyperShot : 1;			// A-Gear �����ۼ� ��ų ��� ����
											// 2012-12-06 by jhseol, ��ġ���� ���� ĳ���� �߽����� ����
	bool			bSearchEye_1 : 1;			// �Ϲ� ��ġ����
	bool			bSearchEye_2 : 1;			// ��ȭ ��ġ����
	bool			bUsingBarialSkill : 1;		// 2013-01-29 by jhseol, ������ ��ų ����Ʈ ����
	bool			bUsingInvicibleSkill : 1;	// 2013-02-06 by jhseol, M��� ������ų �̺�Ʈ ����

	MSG_FC_MOVE_BIT_FLAG() :
		CharacterMode0 { false },
		Invisible0 { false },
		ChargingSkill { false },
		HyperShot { false },
		bSearchEye_1 { false },
		bSearchEye_2 { false },
		bUsingBarialSkill { false },
		bUsingInvicibleSkill { false }
	{ }
};

// 2012-12-06 by jhseol, ��ġ���� ���� ĳ���� �߽����� ����
struct UsingItemBitFlag
{
	bool			bSearchEye_1;			// �Ϲ� ��ġ����
	bool			bSearchEye_2;			// ��ȭ ��ġ����
	bool			bUsingBarialSkill;		// 2013-01-29 by jhseol, ������ ��ų ����Ʈ ����
	bool			bUsingInvicibleSkill;	// 2013-02-06 by jhseol, M��� ������ų �̺�Ʈ ����
	UsingItemBitFlag()
	{
		bSearchEye_1 = FALSE;				// �ʱⰪ �̻������ ���� (FALSE : �̻��, TRUE : ���)
		bSearchEye_2 = FALSE;				// �ʱⰪ �̻������ ���� (FALSE : �̻��, TRUE : ���)
		bUsingBarialSkill = FALSE;			// 2013-01-29 by jhseol, ������ ��ų ����Ʈ ����
		bUsingInvicibleSkill = FALSE;		// 2013-02-06 by jhseol, M��� ������ų �̺�Ʈ ����
	}
};
// end 2012-12-06 by jhseol, ��ġ���� ���� ĳ���� �߽����� ����

struct MSG_FC_MOVE_OK
{
	ClientIndex_t	ClientIndex;
	// 2007-03-29 by cmkwon, �Ʒ��� ���� ��Ʈ�÷��� ������ �ϳ� �߰���
	//	BYTE			CharacterMode0;		// 2005-07-29 by cmkwon
	MSG_FC_MOVE_BIT_FLAG moveBitFlag;	// 2007-03-29 by cmkwon
	AVECTOR3		PositionVector;
	AVECTOR3		TargetVector;
	AVECTOR3		UpVector;
};

struct MSG_FC_MISSILE_MOVE_OK
{
	UINT			ItemNum;						// ���� ������ ����
	ClientIndex_t	MonsterIndex;					// ���� ������ �ε���
	INT				WeaponIndex;					// ���� ���� �ε���(���Ϳ� ���� ������)
	AVECTOR3		PositionVector;
	AVECTOR3		TargetVector;
};

struct MSG_FC_MOVE_LOCKON
{
	ClientIndex_t	AttackIndex;
	ClientIndex_t	TargetIndex;
};

struct MSG_FC_MOVE_LOCKON_OK
{
	char			AttackCharacterName[SIZE_MAX_CHARACTER_NAME];
	ClientIndex_t	AttackIndex;		// 2005-10-11 by cmkwon
	ClientIndex_t	TargetIndex;
};

struct MSG_FC_MOVE_UNLOCKON
{
	ClientIndex_t	AttackIndex;
	ClientIndex_t	TargetIndex;
};

struct MSG_FC_MOVE_UNLOCKON_OK
{
	char			AttackCharacterName[SIZE_MAX_CHARACTER_NAME];
	ClientIndex_t	AttackIndex;		// 2005-10-12 by cmkwon
	ClientIndex_t	TargetIndex;
};

struct MSG_FC_MOVE_LANDING
{
	ClientIndex_t	ClientIndex;
	AVECTOR3		Position;
};

struct MSG_FC_MOVE_LANDING_OK
{
	AVECTOR3	Position;
};

struct MSG_FC_MOVE_LANDING_DONE
{
	AVECTOR3	Position;
};		// C->F, ���� �ϷḦ �˸�

struct MSG_FC_MOVE_TAKEOFF
{
	ClientIndex_t		ClientIndex;
	AVECTOR3	Position;
};

struct MSG_FC_MOVE_TAKEOFF_OK
{
	AVECTOR3	Position;
};

using MSG_FC_MOVE_TARGET = MSG_UNIT_INDEX;

struct MSG_FC_MOVE_WEAPON_VEL
{
	D3DXVECTOR3		WeaponVel;
	D3DXVECTOR3		PetLeftVel;
	D3DXVECTOR3		PetRightVel;
};		// C->F, ������ ������ ������ ����

struct MSG_FC_MOVE_WEAPON_VEL_OK
{
	ClientIndex_t	ClientIndex;
	D3DXVECTOR3		WeaponVel;
	D3DXVECTOR3		PetLeftVel;
	D3DXVECTOR3		PetRightVel;
};	// F->C_in_range, ������ ������ ������ ����

struct MSG_FC_MOVE_ROLLING
{
	AVECTOR3		PositionAVec3;
	AVECTOR3		TargetAVec3;
	AVECTOR3		UpAVec3;
	BYTE			byLeftDirectionFlag;		// ���� �Ѹ� �÷���
};
struct MSG_FC_MOVE_ROLLING_OK
{
	ClientIndex_t	ClientIndex;
	AVECTOR3		PositionAVec3;
	AVECTOR3		TargetAVec3;
	AVECTOR3		UpAVec3;
	BYTE			byLeftDirectionFlag;		// ���� �Ѹ� �÷���
};

struct MSG_FC_MOVE_HACKSHIELD_CRCReqMsg			// 2008-04-03 by cmkwon, �ٽ��� ���� ���� �ý��� ���� - 
{
	_AHNHS_TRANS_BUFFER		stRequestBuf;
};
struct MSG_FC_MOVE_HACKSHIELD_CRCAckMsg			// 2008-04-03 by cmkwon, �ٽ��� ���� ���� �ý��� ���� - 
{
	_AHNHS_TRANS_BUFFER		stResponseBuf;
};

struct MSG_FC_MOVE_HACKSHIELD_HACKING_CLIENT	// 2006-06-05 by cmkwon
{
	long			lHackingClinetCode;			// ERR_AHNHS_XXXXXX	
	char			szErrString[SIZE_STRING_128];			// 2006-10-20 by cmkwon, �߰���(��ŷ ���α׷���)
};

struct MEX_TARGET_INFO
{
	AVECTOR3		TargetPosition;			// ���� Ŭ���̾�Ʈ�� ȭ�鿡���� Ÿ�� ������
	ClientIndex_t	TargetIndex;			// ���� ��� ClientIndex or MonterIndex, 0�̸� ItemFieldIndex�� ��ȿ
	UINT			TargetItemFieldIndex;	// ���� ��� ������ �������̸� TargetIndex ��ȿ, �ƴϸ� TargetIndex�� 0
	USHORT			MultiTargetIndex;		// 2011-03-21 by hskim, ���Ǵ�Ƽ 3�� - ���� ��Ƽ Ÿ���� ��� �߰�

public:
	void SetNullTarget()
	{
		TargetIndex = 0;
		TargetItemFieldIndex = 0;
		MultiTargetIndex = 0;				// 2011-03-21 by hskim, ���Ǵ�Ƽ 3�� - ���� ��Ƽ Ÿ���� ��� �߰�
	}
};

struct MSG_FC_BATTLE_ATTACK
{
	MEX_TARGET_INFO	TargetInfo;
	AVECTOR3		FirePosition;		// ������ �߻� ��ġ
	BYTE			AttackType;			// ���� Ÿ��, ATT_TYPE_XXX, see below
	UINT			SkillNum;			// ��ų ��� �� ���
};					// C->F

					// ATT_TYPE_XXX, 1~100: 1�� ����, 101~200: 2�� ����
#define	ATT_TYPE_NONE			((BYTE)0)	// Attack Type ����
					// 1�� ����
#define	ATT_TYPE_GENERAL_PRI		((BYTE)1)	// 1�� ���� �Ϲ� ����
#define	ATT_TYPE_GROUND_BOMBING_PRI	((BYTE)2)	// 1�� ���� ���� ���
#define	ATT_TYPE_AIR_BOMBING_PRI	((BYTE)3)	// 1�� ���� ���� ���
#define	ATT_TYPE_SEIGE_PRI			((BYTE)4)	// 1�� ���� ���
#define	ATT_TYPE_SPLASH_PRI			((BYTE)5)	// 1�� Splash ���� ���
#define	ATT_TYPE_END_PRI			((BYTE)100)	// 1�� ���� ���� ��, ������ �� ����
					// 2�� ����
#define ATT_TYPE_GENERAL_SEC		((BYTE)101)	// 2�� ���� �Ϲ� ����
#define ATT_TYPE_GROUND_BOMBING_SEC	((BYTE)102)	// 2�� ���� ���� ���
#define ATT_TYPE_AIR_BOMBING_SEC	((BYTE)103)	// 2�� ���� ���� ���
#define ATT_TYPE_SIEGE_SEC			((BYTE)104)	// 2�� ���� ���
#define	ATT_TYPE_END_SEC			((BYTE)200)	// 2�� ���� ���� ��, ������ �� ����

					// 2010-06-15 by shcho&hslee ��ý��� - �� ���� ó��
#define ATT_TYPE_PET_ATK			ATT_TYPE_PET_GENERAL
#define ATT_TYPE_PET_GENERAL		((BYTE)201) // �� ����(�Ϲ� ����)
#define ATT_TYPE_PET_END			((BYTE)300)	// �� ���� ��

#define IS_PET_ATT_TPYE(_ATT_TYPE)			(util::in_range(ATT_TYPE_PET_START, _ATT_TYPE, ATT_TYPE_PET_END))
					// END 2010-06-15 by shcho&hslee ��ý��� - �� ���� ó��
#define IS_PRIMARY_ATT_TYPE(_ATT_TYPE)		(util::in_range(ATT_TYPE_GENERAL_PRI, _ATT_TYPE, ATT_TYPE_END_PRI))
#define IS_SECONDARY_ATT_TYPE(_ATT_TYPE)	(util::in_range(ATT_TYPE_GENERAL_SEC, _ATT_TYPE, ATT_TYPE_END_SEC))

					///////////////////////////////////////////////////////////////////////////////
					// 2007-06-04 by cmkwon
struct SATTACK_PARAMETER
{
	UID16_t			WeaponIndex;					// ź�� ������ȣ - 2�� ���⸸ ó����
	DWORD			dwShotTick;						// 2007-06-08 by cmkwon, �߻� Tick
	ITEM			*pWeaponItemInfo;				// ���� ���� ������ ����
	float			fAttackProbability;				// ���� Ȯ��
	float			fPierceAttackProbability;		// ���� �Ǿ��
	float			fAttack;						// ���ݷ�
	float			fMaxAttack;						// �ִ���ݷ�	
	ClientIndex_t	TargetIndex;					// 2010-04-05 by cmkwon, ����2�� M2M 2�� ���� ���� ó�� - 
	BYTE			AttackType;						// 2010-04-05 by cmkwon, ����2�� M2M 2�� ���� ���� ó�� - 
	USHORT			MultiTargetIndex;				// 2011-04-04 by hskim, ���Ǵ�Ƽ 3�� - ���� ��Ƽ Ÿ���� ��� �߰�
};
using mtvectSATTACK_PARAMETER = mt_vector<SATTACK_PARAMETER>;		// 2007-06-07 by cmkwon
using vectSATTACK_PARAMETER = vector<SATTACK_PARAMETER>;			// 2010-04-05 by cmkwon, ����2�� M2M 2�� ���� ���� ó�� - 

struct MSG_FC_BATTLE_ATTACK_OK
{
	ClientIndex_t	AttackIndex;
	MEX_TARGET_INFO	TargetInfo;
	AVECTOR3		FirePosition;		// ������ �߻� ��ġ
	BYTE			AttackType;			// ���� Ÿ��, ATT_TYPE_XXX
	UID16_t			WeaponIndex;		// Ŭ���̾�Ʈ���� �߻�� �Ѿ��� �ε���, �������� ����, CUID16Generator ���
	UINT			ItemNum;			// ������ ItemNum
	USHORT			RemainedBulletFuel;	// ���� �Ѿ�(Ȥ�� Fuel)�� ��
	UINT			SkillNum;			// ��ų ��� �� ���
	ClientIndex_t	DelegateClientIdx;	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� �� 2�� ���� ���� ó�� �� ���� Ŭ���̾�Ʈ
};				// F->C_in_range

struct MSG_FC_BATTLE_ATTACK_FIND
{
	ClientIndex_t	AttackIndex;
	MEX_TARGET_INFO	TargetInfo;
	UINT			ItemNum;			// ������ ItemNum, ������ 2�� ���ݿ��� ����
	UID16_t			WeaponIndex;		// Ŭ���̾�Ʈ���� �߻�� �Ѿ��� �ε���, �������� ����, CUID16Generator ���
	BYTE			AttackType;			// ���� Ÿ��, ATT_TYPE_XXX
};			// C->F

struct MSG_FC_BATTLE_ATTACK_FIND_OK
{
	ClientIndex_t	AttackIndex;
	ClientIndex_t	TargetIndex;			// ���� ��� ClientIndex or MonterIndex, 0�̸� ItemFieldIndex�� ��ȿ
	UINT			TargetItemFieldIndex;	// ���� ��� ������ �������̸� TargetIndex ��ȿ, �ƴϸ� TargetIndex�� 0
	UID16_t			WeaponIndex;		// Ŭ���̾�Ʈ���� �߻�� �Ѿ��� �ε���, �������� ����, CUID16Generator ���
	BYTE			AttackType;			// ���� Ÿ��, ATT_TYPE_XXX
};			// F->C_in_range

			// ���η� ó��
struct MSG_FC_BATTLE_DROP_MINE
{
	BYTE			NumOfMines;
	ClientIndex_t	TargetIndex;		// Mine�� �� Target�� ������ �����ȴ�(������ Ŭ���̾�Ʈ�� ���޸� �ϸ��)
	ARRAY_(AVECTOR3);					// MINE�� ������ ��ġ
};				// C->F

struct MSG_FC_BATTLE_DROP_MINE_OK
{
	ClientIndex_t	AttackIndex;		// MINE ������
	ClientIndex_t	TargetIndex;		// Mine�� �� Target�� ������ �����ȴ�(������ Ŭ���̾�Ʈ�� ���޸� �ϸ��)
	UINT			ItemFieldIndex;		// ���� ������ ������ �ӽ÷� �����ϴ� ���� ��ȣ
	INT				ItemNum;			// Ŭ���̾�Ʈ�� �������� ������ �����ֱ� ���� ����
	AVECTOR3		DropPosition;		// MINE�� ������ ��ġ
	USHORT			RemainedBulletFuel;	// ���� �Ѿ�(Ȥ�� Fuel)�� ��
};			// F->C_in_range, ������ �����ֱ�

struct MSG_FC_BATTLE_MINE_ATTACK
{
	UINT			ItemFieldIndex;		// ������ �ӽ÷� �����ϴ� ���� ��ȣ
	MEX_TARGET_INFO	TargetInfo;			// �ǰ�����
};			// C->F

struct MSG_FC_BATTLE_MINE_ATTACK_OK
{
	UINT			ItemFieldIndex;		// ������ �ӽ÷� �����ϴ� ���� ��ȣ
	MEX_TARGET_INFO	TargetInfo;			// �ǰ�����
};			// F->C_in_range

struct MSG_FC_BATTLE_MINE_ATTACK_FIND
{
	UINT			ItemFieldIndex;		// ������ �ӽ÷� �����ϴ� ���� ��ȣ
	MEX_TARGET_INFO	TargetInfo;			// �ǰ�����
};		// C->F

struct MSG_FC_BATTLE_MINE_ATTACK_FIND_OK
{
	UINT			ItemFieldIndex;		// ������ �ӽ÷� �����ϴ� ���� ��ȣ
	MEX_TARGET_INFO	TargetInfo;			// �ǰ�����
};	// F->C_in_range

	// 2007-08-07 by cmkwon, 1��/2�� ���� �Ѿ� ���� ������ ���� - �Ʒ��� ���� ������
	//typedef struct  {
	//	USHORT			BulletCount;		// ������ reload�� �Ѿ��� ���� ����
	//} MSG_FC_BATTLE_PRI_BULLET_RELOADED;
	//
	//typedef struct  {
	//	USHORT			BulletCount;		// ������ reload�� �Ѿ��� ���� ����
	//} MSG_FC_BATTLE_SEC_BULLET_RELOADED;

	// 2007-08-07 by cmkwon, 1��/2�� ���� �Ѿ� ���� ������ ���� - BULLET_RECHARGE_TYPE_XXX ���� �߰�
#define BULLET_RECHARGE_TYPE_NORMAL			((BYTE)0)
#define BULLET_RECHARGE_TYPE_REPAIR_SHOP	((BYTE)1)
#define BULLET_RECHARGE_TYPE_BULLET_ITEM	((BYTE)2)
#define BULLET_RECHARGE_TYPE_ADMIN_COMMAND	((BYTE)3)
struct  MSG_FC_BATTLE_PRI_BULLET_RELOADED
{
	USHORT			BulletCount;		// ������ reload�� �Ѿ��� ���� ����
	USHORT			RechargeCount;		// 2007-08-07 by cmkwon, 1��/2�� ���� �Ѿ� ���� ������ ���� - �߰��� �ʵ�
	BYTE			RechargeType;		// 2007-08-07 by cmkwon, 1��/2�� ���� �Ѿ� ���� ������ ���� - �߰��� �ʵ�(BULLET_RECHARGE_TYPE_XXX)
};
using MSG_FC_BATTLE_SEC_BULLET_RELOADED = MSG_FC_BATTLE_PRI_BULLET_RELOADED;

// Kind of Damages: ����Ʈ�� ǥ���ϱ� ���� ����Ѵ�. check: �Ʒ��з��� �����ǵǾ�� �Ѵ�.
// DAMAGEKIND_XXX
#define DAMAGEKIND_NO_DAMAGE	(BYTE)0x00
#define DAMAGEKIND_NORMAL		(BYTE)0x01	// 0 < DAMAGE < 100
#define DAMAGEKIND_CRITICAL		(BYTE)0x02	// �ǰ������� ���� ����
#define DAMAGEKIND_ADD_DAMAGE	(BYTE)0x03	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���밪 �߰� Ÿ��ġ ������
#define DAMAGEKIND_REFLECTION	(BYTE)0x04	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - ������ �ݻ�
#define DAMAGEKIND_PET			(BYTE)0x05	// 2010-11-01 by jskim, �� ������ View ����

struct MSG_FC_BATTLE_ATTACK_EXPLODE_ITEM
{
	ClientIndex_t	TargetIndex;			// �ش� �������� ������ ClientIndex
	UINT			TargetItemFieldIndex;
};		// ��ü�� �ް� �ִ� ������(DUMMY �� ��)�� ������ ���

struct MSG_FC_BATTLE_ATTACK_HIDE_ITEM
{
	ClientIndex_t	TargetIndex;			// �ش� �������� ������ ClientIndex
	UINT			TargetItemFieldIndex;
};			// ��ü�� �ް� �ִ� ������(DUMMY �� ��)�� �׳� ������� ���(Time Out ��� ����...)

struct MSG_FC_BATTLE_ATTACK_EXPLODE_ITEM_W_KIND
{
	ClientIndex_t	TargetIndex;			// �ش� �������� ������ ClientIndex
	BYTE			ItemKind;
	UINT			TargetItemFieldIndex;
};	// With KIND, ��ü�� �ް� �ִ� ������(FIXER �� ��)�� ������ ���

struct MSG_FC_BATTLE_ATTACK_HIDE_ITEM_W_KIND
{
	ClientIndex_t	TargetIndex;			// �ش� �������� ������ ClientIndex
	BYTE			ItemKind;
	UINT			TargetItemFieldIndex;
};	// With KIND, ��ü�� �ް� �ִ� ������(FIXER �� ��)�� �׳� ������� ���(Time Out ��� ����...)

	// ����� ó��
struct MSG_FC_BATTLE_DROP_BUNDLE
{
	UID64_t			ItemUniqueNumber;
	ClientIndex_t	AttackClientIndex;
	ClientIndex_t	TargetClientIndex;
	AVECTOR3		DropPosition;
};

struct MSG_FC_BATTLE_DROP_BUNDLE_OK
{
	UID64_t			ItemUniqueNumber;
	ClientIndex_t	AttackClientIndex;
	ClientIndex_t	TargetClientIndex;
	INT				ItemNum;			// Ŭ���̾�Ʈ�� �������� ������ �����ֱ� ���� ����
	AVECTOR3		DropPosition;
	USHORT			NumOfBullet;		// �߻��� źü�� ����
};

struct MSG_FC_BATTLE_BUNDLE_ATTACK
{
	UID64_t			BundleItemUniqueNumber;	// ������ ItemUniqueNumber
	AVECTOR3		AttackPosition;
	ClientIndex_t	TargetIndex;
	AVECTOR3		TargetPosition;
};

struct MSG_FC_BATTLE_BUNDLE_ATTACK_RESULT
{
	INT				ItemNum;				// bundle�� ��ũ ������(�̻��� ��)�� ItemNum
	ClientIndex_t	AttackIndex;
	AVECTOR3		AttackPosition;
	ClientIndex_t	TargetIndex;
	AVECTOR3		TargetPosition;
};

struct MSG_FC_BATTLE_BUNDLE_ATTACK_ITEM
{
	UID64_t			BundleItemUniqueNumber;	// ������ ItemUniqueNumber
	AVECTOR3		AttackPosition;
	ClientIndex_t	TargetIndex;
	UINT			TargetItemFieldIndex;	// ��� ������
	AVECTOR3		TargetPosition;
};

struct MSG_FC_BATTLE_BUNDLE_ATTACK_ITEM_RESULT
{
	INT				ItemNum;				// bundle�� ��ũ ������(�̻��� ��)�� ItemNum
	ClientIndex_t	AttackIndex;
	AVECTOR3		AttackPosition;
	ClientIndex_t	TargetIndex;
	UINT			TargetItemFieldIndex;	// ��� ������
	AVECTOR3		TargetPosition;
};

// check: �ʿ��ϸ� �츲(���� NO BODY��), kelovon, 20030917
//typedef struct
//{
//} MSG_FC_BATTLE_TOGGLE_SHIELD;

struct MSG_FC_BATTLE_TOGGLE_SHIELD_RESULT
{
	ClientIndex_t	AttackIndex;		// shield�� ������ ĳ����
	BYTE			IsOn;				// 0(FALSE): off, 1(TRUE): on
	INT				ItemNum;
};	// F->C, SHIELD�� �������̸� MSG_FC_CHARACTER_GET_OTHER_INFO_OK���� �� �� MSG�� �ٿ�������.

	// check: �ʿ��ϸ� �츲(���� NO BODY��), kelovon, 20040517
	//typedef struct
	//{
	//} MSG_FC_BATTLE_TOGGLE_DECOY;

struct MSG_FC_BATTLE_TOGGLE_DECOY_OK
{
	ClientIndex_t	AttackIndex;	// decoy�� ������ ĳ����
	BYTE			IsOn;			// 0(FALSE): off, 1(TRUE): on
	INT				ItemNum;
};	// F->C, DECOY�� �������̸� MSG_FC_CHARACTER_GET_OTHER_INFO_OK���� �� �� MSG�� �ٿ�������.

struct MSG_FC_BATTLE_SHIELD_DAMAGE
{
	ClientIndex_t	TargetIndex;		// shield�� ������Ű�� �ִ� ĳ����
	AVECTOR3		CollisionPosition;	// �浹 ��ġ
};

// ����(DUMMY)��
struct MSG_FC_BATTLE_DROP_DUMMY
{
	UID64_t		ItemUniqueNumber;
	//	BYTE		NumOfDummies;			// check: �����. 20030930, kelovon with jinking
	//	ARRAY_(AVECTOR3);					// DUMMY�� ��ġ(��ü�� ���� ��� ��ǥ), check: �����. 20030930, kelovon with jinking
};				// ���̴� �ѹ��� �� ���.

				// 2007-06-21 by cmkwon, ü�� �ϳ��� �޽����� ��� ���� - �Ʒ��� ���� ����ü ����
				//typedef struct
				//{
				//	ClientIndex_t	AttackIndex;		// DUMMY�� �ߵ��� ��ü
				//	UINT			ItemFieldIndex;		// ���� ������ ������ �ӽ÷� �����ϴ� DUMMY ��ȣ
				//	INT				ItemNum;			// Ŭ���̾�Ʈ�� �������� ������ �����ֱ� ���� ����
				////	AVECTOR3		DropPosition;		// ��ü�� ���� ��� ��ǥ, check: �����. 20030930, kelovon with jinking
				//} MSG_FC_BATTLE_DROP_DUMMY_OK;			// ������ �����ֱ�,  DUMMY�� �������̸� MSG_FC_CHARACTER_GET_OTHER_INFO_OK���� �� �� MSG�� �ٿ�������.
struct MSG_FC_BATTLE_DROP_DUMMY_OK		// 2007-06-21 by cmkwon, ü�� �ϳ��� �޽����� ��� ���� -
{
	ClientIndex_t	AttackIndex;		// DUMMY�� �ߵ��� ��ü
	INT				ItemNum;			// Ŭ���̾�Ʈ�� �������� ������ �����ֱ� ���� ����
	INT				DummyCounts;		// 2007-06-21 by cmkwon, ü�� �ϳ��� �޽����� ��� ���� -
	_ARRAY(UINT ItemFieldIndex);		// 2007-06-21 by cmkwon, ü�� �ϳ��� �޽����� ��� ���� - DummyCounts ��ŭ �ٿ��� ����
};

// �ȼ�(FIXER)��
struct MSG_FC_BATTLE_DROP_FIXER
{
	UID64_t			ItemUniqueNumber;
	ClientIndex_t	TargetIndex;
};				// �� ���� ������ŭ �� ��

struct MSG_FC_BATTLE_DROP_FIXER_OK
{
	ClientIndex_t	AttackIndex;		// FIXER�� �� ��ü
	ClientIndex_t	TargetIndex;
	UINT			ItemFieldIndex;		// ���� ������ ������ �ӽ÷� �����ϴ� FIXER�� ��ȣ
	INT				ItemNum;			// Ŭ���̾�Ʈ�� �������� ������ �����ֱ� ���� ����
};			// ������ ���� ����, ������ �����ֱ�, FIXER�� �������̸� MSG_FC_CHARACTER_GET_OTHER_INFO_OK���� �� �� MSG�� �ٿ�������.

			// check: �ʿ��ϸ� �츲(���� NO BODY��), kelovon, 20030612
			//typedef struct
			//{
			//} MSG_FC_BATTLE_REQUEST_PK;			// C->F, client�� PK ��û

struct MSG_FC_BATTLE_REQUEST_PK_OK
{
	ClientIndex_t	ClientIndex;		// PK�� ��û�� client
};			// F->C, pk ��û �³�

			// check: �ʿ��ϸ� �츲(���� NO BODY��), kelovon, 20030612
			//typedef struct
			//{
			//} MSG_FC_BATTLE_CANCEL_PK;			// F->C, PK ����

struct MSG_FC_BATTLE_REQUEST_P2P_PK
{
	ClientIndex_t	TargetClientIndex;		// �ǿ�û��
	int				AdditionalParameter;	// 0 => 'normal' duel, 1 => with restrictions
};				// C->F, �ϴ��� PK ��û

struct MSG_FC_BATTLE_REQUEST_P2P_PK_OK
{
	ClientIndex_t	SourceClientIndex;		// ��û��
	int				AdditionalParameter;	// 0 => 'normal' duel, 1 => with restrictions
};			// F->C, �ϴ��� PK ��û

struct MSG_FC_BATTLE_ACCEPT_REQUEST_P2P_PK
{
	ClientIndex_t	SourceClientIndex;		// ��û��
};		// C->F, �ϴ��� PK �³�

struct MSG_FC_BATTLE_ACCEPT_REQUEST_P2P_PK_OK
{
	ClientIndex_t	PeerClientIndex;		// ����
};	// F->C, �ϴ��� PK �³�, �� ������ ����, Ŭ���̾�Ʈ�� ������ PK ����

struct MSG_FC_BATTLE_REJECT_REQUEST_P2P_PK
{
	ClientIndex_t	SourceClientIndex;		// ��û��
};		// C->F, �ϴ��� PK ����

struct MSG_FC_BATTLE_REJECT_REQUEST_P2P_PK_OK
{
	ClientIndex_t	TargetClientIndex;		// �ǿ�û��
};	// F->C, �ϴ��� PK ����

struct MSG_FC_BATTLE_SURRENDER_P2P_PK
{
	ClientIndex_t	TargetClientIndex;		// �ǿ�û��
};			// C->F, �ϴ��� PK �׺�

struct MSG_FC_BATTLE_SURRENDER_P2P_PK_OK
{
	ClientIndex_t	SourceClientIndex;		// ��û��
};		// F->C, �ϴ��� PK �׺�

struct MSG_FC_BATTLE_ACCEPT_SURRENDER_P2P_PK
{
	ClientIndex_t	SourceClientIndex;		// ��û��
};	// C->F, �ϴ��� PK �׺� �³�, �̿� ���� ������ MSG_FC_BATTLE_END_P2P_PK�� ó��

	/*
	struct MSG_FC_BATTLE_ACCEPT_SURRENDER_P2P_PK_OK
	{
	ClientIndex_t	TargetClientIndex;		// �ǿ�û��
	};	// F->C, �ϴ��� PK �׺� �³�
	*/

struct MSG_FC_BATTLE_REJECT_SURRENDER_P2P_PK
{
	ClientIndex_t	SourceClientIndex;		// ��û��
};	// C->F, �ϴ��� PK �׺� ����

struct MSG_FC_BATTLE_REJECT_SURRENDER_P2P_PK_OK
{
	ClientIndex_t	TargetClientIndex;		// �ǿ�û��
};	// F->C, �ϴ��� PK �׺� ����

struct MSG_FC_BATTLE_END_P2P_PK
{
	ClientIndex_t	PeerClientIndex;		// ������ ClientIndex
	USHORT			EndType;				// ���� ���� Ÿ��, BATTLE_END_XXX
};					// PK ����

					// ���� ���� Ÿ��, BATTLE_END_XXX
#define BATTLE_END_WIN			(USHORT)0x0000	// �¸�
#define BATTLE_END_DEFEAT		(USHORT)0x0001	// �й�
#define BATTLE_END_TIE			(USHORT)0x0002	// ���º�
#define BATTLE_END_END			(USHORT)0x0003	// �׳� ����Ǿ����ϴ�(�����ҹ�)
#define BATTLE_END_TIME_LIMITE	(USHORT)0x0004	// �ð�����
#define BATTLE_END_SURRENDER	(USHORT)0x0005	// �׺�

struct MSG_FC_BATTLE_SHOW_DAMAGE
{
	ClientIndex_t	TargetIndex;	// TargetIndex
	USHORT			AmountDamage;	// ������ ��
	BYTE			DamageKind;		// DAMAGEKIND_XXX
	BYTE			byIsPrimaryWeapon;		// 2008-12-03 by cmkwon, ������ ������ 1��,2�� ���� �߰� - 
	USHORT			MultiTargetIndex;		// 2011-03-21 by hskim, ���Ǵ�Ƽ 3�� - ���� ��Ƽ Ÿ���� ��� �߰�
};		// F->C, ���� ���������� ǥ����



struct MSG_FC_BATTLE_ATTACK_EVASION		// 2005-12-12 by cmkwon
{
	ClientIndex_t	AttackIndex;
	MEX_TARGET_INFO	TargetInfo;
	UINT			ItemNum;			// ������ ItemNum, ������ 2�� ���ݿ��� ����
	UID16_t			WeaponIndex;		// Ŭ���̾�Ʈ���� �߻�� �Ѿ��� �ε���, �������� ����, CUID16Generator ���
	BYTE			AttackType;			// ���� Ÿ��, ATT_TYPE_XXX
};

using MSG_FC_BATTLE_ATTACK_EVASION_OK = MSG_FC_BATTLE_ATTACK_EVASION;	// 2005-12-12 by cmkwon

struct MSG_FC_BATTLE_DELETE_DUMMY_OK		// 2006-12-04 by dhjin
{
	ClientIndex_t	AttackIndex;
	UINT			ItemFieldIndex;
};

struct MSG_FC_BATTLE_EXPLODE_DUMMY_OK		// 2006-12-04 by dhjin
{
	ClientIndex_t	AttackIndex;
	UINT			ItemFieldIndex;
};


///////////////////////////////////////////////////////////////////////////////
// FN_BATTLE (Field server <-> NPC server)

struct MSG_FN_BATTLE_ATTACK_PRIMARY
{
	ChannelIndex_t	ChannelIndex;
	ClientIndex_t	AttackIndex;
	ClientIndex_t	TargetIndex;
	UINT			WeaponItemNumber;	// ���� Ÿ��(�� ��������,��ų����)
	USHORT			WeaponIndex;		// ���� �Ѿ�(Ȥ�� Fuel)�� ��, Fuel�� ��쿡�� x10�Ͽ� �����
	AVECTOR3		TargetPosition;		// ���� Ŭ���̾�Ʈ�� ȭ�鿡���� Ÿ�� ������
	USHORT			MultiTargetIndex;	// 2011-03-21 by hskim, ���Ǵ�Ƽ 3�� - ���� ��Ƽ Ÿ���� ��� �߰�
};

struct MSG_FN_BATTLE_ATTACK_RESULT_PRIMARY
{
	ClientIndex_t	AttackIndex;
	ClientIndex_t	TargetIndex;
	UINT			WeaponItemNumber;	// ���� Ÿ��(�� ��������,��ų����)
	USHORT			WeaponIndex;		// ���� �Ѿ�(Ȥ�� Fuel)�� ��, Fuel�� ��쿡�� x10�Ͽ� �����
	AVECTOR3		TargetPosition;		// ���� Ŭ���̾�Ʈ�� ȭ�鿡���� Ÿ�� ������
	BYTE			DamageKind;
};

struct MSG_FN_BATTLE_ATTACK_SECONDARY
{
	ChannelIndex_t	ChannelIndex;
	ClientIndex_t	AttackIndex;
	ClientIndex_t	TargetIndex;
	UINT			WeaponItemNumber;	// ���� Ÿ��(�� ��������,��ų����)
	USHORT			WeaponIndex;		// ���� �Ѿ�(Ȥ�� Fuel)�� ��, Fuel�� ��쿡�� x10�Ͽ� �����
	AVECTOR3		TargetPosition;		// ���� Ŭ���̾�Ʈ�� ȭ�鿡���� Ÿ�� ������
	USHORT			MultiTargetIndex;	// 2011-03-21 by hskim, ���Ǵ�Ƽ 3�� - ���� ��Ƽ Ÿ���� ��� �߰�
	BYTE			Distance;
	BYTE			SecAttackType;		// 2�� ���� ���� Ÿ��: SEC_ATT_TYPE_XXX, see below
	AVECTOR3		AttackPosition;		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ��ġ
};

struct MSG_FN_BATTLE_ATTACK_RESULT_SECONDARY
{
	ClientIndex_t	AttackIndex;
	ClientIndex_t	TargetIndex;
	UINT			WeaponItemNumber;	// ���� Ÿ��(�� ��������,��ų����)
	USHORT			WeaponIndex;		// ���� �Ѿ�(Ȥ�� Fuel)�� ��, Fuel�� ��쿡�� x10�Ͽ� �����
	AVECTOR3		TargetPosition;		// ���� Ŭ���̾�Ʈ�� ȭ�鿡���� Ÿ�� ������
	BYTE			DamageKind;
	BYTE			Distance;
	BYTE			SecAttackType;		// 2�� ���� ���� Ÿ��: SEC_ATT_TYPE_XXX
};

struct MSG_FN_BATTLE_ATTACK_FIND
{
	ClientIndex_t	AttackIndex;
	ClientIndex_t	TargetIndex;
	AVECTOR3		TargetPosition;		// ���� Ŭ���̾�Ʈ�� ȭ�鿡���� Ÿ�� ������
	USHORT			WeaponIndex;		// Ŭ���̾�Ʈ���� �߻�� �Ѿ��� �ε���
	ChannelIndex_t	ChannelIndex;
};

struct MSG_FN_BATTLE_ATTACK_FIND_RESULT
{
	ClientIndex_t	AttackIndex;
	ClientIndex_t	TargetIndex;
	USHORT			WeaponIndex;		// Ŭ���̾�Ʈ���� �߻�� �Ѿ��� �ε���
	BYTE			DamageKind;
};

struct MSG_FN_BATTLE_DROP_FIXER
{
	ChannelIndex_t	ChannelIndex;		// check_cmkwon, �߰��ϱ�~, 20040330, kelovon
	ClientIndex_t	AttackIndex;		// Attack Character
	ClientIndex_t	TargetIndex;		// Target Monster
	INT				ItemNum;			// Fixer�� ItemNum
};				// F -> N

struct MSG_FN_BATTLE_DROP_FIXER_OK
{
	ChannelIndex_t	ChannelIndex;		// check_cmkwon, �߰��ϱ�~, 20040330, kelovon
	ClientIndex_t	AttackIndex;		// Attack Character
	ClientIndex_t	TargetIndex;		// Target Monster
	UINT			ItemFieldIndex;
	INT				ItemNum;			// Ŭ���̾�Ʈ�� �������� ������ �����ֱ� ���� ����
};			// N -> F

struct MSG_FN_BATTLE_ATTACK_HIDE_ITEM_W_KIND : public MSG_FC_BATTLE_ATTACK_HIDE_ITEM_W_KIND
{
	ChannelIndex_t	ChannelIndex;
};

struct MSG_FN_BATTLE_SET_ATTACK_CHARACTER
{
	ChannelIndex_t	ChannelIndex;
	ClientIndex_t	AttackIndex;
	ClientIndex_t	TargetIndex;
	float			DamageAmount;
	BYTE			ItemKind;
};

///////////////////////////////////////////////////////////////////////////////
// Party(���, ��Ƽ) ���� ��������
///////////////////////////////////////////////////////////////////////////////

// IMServer���� FieldServer�� �Ѱ��־�� �ϴ� ����
struct MEX_FIELD_PARTY_INFO
{
	INT		nTotalPartyMember;			// �� ��Ƽ�� ��
	BYTE	lowestMemberLevel;			// ���� ��Ƽ�� level, check: FieldServer�� IMServer�� ������� level ���� �� update�ϴ� �κ� �����ؾ� ��! 20031101, kelovon
};

// ��Ƽ ����
struct MSG_IC_PARTY_CREATE
{
	UID32_t		CharacterUniqueNumber;		// ������(��Ƽ��) ��ȣ
	SPARTY_INFO	PartyInfo;					// 2008-06-02 by dhjin, EP3 ��� ���� - ��� ����
	DWORD		Padding[SIZE_MAX_PACKET_PADDING];					// 2011-07-21 by hskim, ���� ���� ���� - ���� ������ ȣȯ �ȵǵ��� ����ü ũ�� �ٲ�
};

struct MSG_FI_PARTY_CREATE_OK
{
	PartyID_t				PartyID;
	UID32_t					CharacterUniqueNumber;	// ������(��Ƽ��) ��ȣ
	MEX_FIELD_PARTY_INFO	FieldPartyInfo;
	BYTE					ExpDistributeType;		// 2008-06-02 by dhjin, EP3 ��� ���� - ��� ����ġ �й� Ÿ��
	BYTE					ItemDistributeType;		// 2008-06-02 by dhjin, EP3 ��� ���� - ��� ������ �й� Ÿ��
};

struct MSG_FC_PARTY_CREATE_OK
{
	PartyID_t	PartyID;
};

#ifdef NEMERIAN_TEAMSPEAK
typedef struct
{
	unsigned long long ChannelID;
} MSG_FC_TEAMSPEAK_MOVE_CLIENT;
#endif

// ��Ƽ�� �ʴ�
struct MSG_FC_PARTY_REQUEST_INVITE
{
	char		CharacterName[SIZE_MAX_CHARACTER_NAME];		// �ʴ��� ����
};

struct MSG_FC_PARTY_REQUEST_INVITE_QUESTION
{
	PartyID_t	PartyID;
	char		MasterCharacterName[SIZE_MAX_CHARACTER_NAME];	// ��Ƽ��
};

using MSG_FC_PARTY_REQUEST_INVITE_QUESTION_THE_ARENA = MSG_FC_PARTY_REQUEST_INVITE_QUESTION;	// 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ڵ���Ƽ : ��Ƽ�� ���Խ�ų Ŭ�󿡰� ��Ƽ ���� �غ� ��Ŷ ������.

struct MSG_FC_PARTY_ACCEPT_INVITE
{
	PartyID_t	PartyID;
};

struct MSG_FI_PARTY_ACCEPT_INVITE_OK
{
	PartyID_t				PartyID;
	UID32_t					CharacterUniqueNumber;		// ���� ������ ��Ƽ��
	MEX_FIELD_PARTY_INFO	FieldPartyInfo;				// �� MSG�� F->I�� ���� ������
};						// F->I, I->F �� �� ����

						//////////////////////////////////////////////////////////////////////
						// 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ڵ���Ƽ : ����ȣ ������
struct MSG_FI_PARTY_ARENA_TEAM_NUM
{
	UID32_t					CharacterUniqueNumber;		/// 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ڵ���Ƽ : IM���� ����� ���� ã���� ���� CUID
	int						TeamNum;					/// 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ڵ���Ƽ : IM���� ���� ������ üũ�� �� ��ȣ
};
// end 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ڵ���Ƽ : ����ȣ ������


struct IMPartyMember;

struct IM_PARTY_MEMBER_INFO
{
	UID32_t		CharacterUniqueNumber;
	char		CharacterName[SIZE_MAX_CHARACTER_NAME];
	BYTE		NameColor;
	BYTE		PilotFace;
	USHORT		UnitKind;
	USHORT		Race;
	BYTE		Level;
	//char		MapName[SIZE_MAX_MAP_NAME];	// IS_VALID_MAP_NAME()�� FALSE�̸�, ��Ƽ �Ҽ��̳� �������� �ƴ� ĳ����(�׾ ���� ĳ���� ��)
	MAP_CHANNEL_INDEX	MapChannelIndex;	// IsValid()�� FALSE�̸�, ��Ƽ �Ҽ��̳� �������� �ƴ� ĳ����(�׾ ���� ĳ���� ��)
	EN_CHECK_TYPE	VoipType;		// 2008-07-16 by dhjin, EP3 - Voip ����

									// operator overloading
	IM_PARTY_MEMBER_INFO& operator=(const IMPartyMember& rhs);
	inline IM_PARTY_MEMBER_INFO& operator=(const CHARACTER& rhs)
	{
		this->CharacterUniqueNumber = rhs.CharacterUniqueNumber;
		this->PilotFace = rhs.PilotFace;
		this->UnitKind = rhs.UnitKind;
		this->Race = rhs.Race;
		this->Level = rhs.Level;
		util::strncpy(this->CharacterName, rhs.CharacterName, SIZE_MAX_CHARACTER_NAME);
		NameColor = rhs.Color;
		this->MapChannelIndex = rhs.MapChannelIndex;
		return *this;
	}
};										// IM Server�� �׻� �����ؾ� �ϴ� ����

struct MSG_IC_PARTY_ACCEPT_INVITE_OK
{
	PartyID_t				PartyID;
	IM_PARTY_MEMBER_INFO	IMPartyMemberInfo;
};

struct MSG_FC_PARTY_REJECT_INVITE
{
	PartyID_t	PartyID;
};

struct MSG_FC_PARTY_REJECT_INVITE_OK
{
	PartyID_t	PartyID;
	char		CharacterName[SIZE_MAX_CHARACTER_NAME];		// �ʴ��ߴ� ����
};

// ��Ƽ�� ����(from IM Server)
struct MSG_IC_PARTY_GET_MEMBER
{
	UID32_t		CharacterUniqueNumber;
};					// ���� ��Ƽ������ �������� Ȯ���Ѵ�

struct MSG_IC_PARTY_PUT_MEMBER
{
	IM_PARTY_MEMBER_INFO	IMPartyMemberInfo;
};

struct MSG_IC_PARTY_GET_ALL_MEMBER
{
	PartyID_t	PartyID;
};

struct MSG_IC_PARTY_PUT_ALL_MEMBER
{
	PartyID_t	PartyID;
	UID32_t		MasterUniqueNumber;			// ��Ƽ���� CharacterUniqueNumber
	UINT		nNumOfPartyMembers;
	ARRAY_(IM_PARTY_MEMBER_INFO);
};

// ��Ƽ�� ����(from Field Server)
struct FIELD_PARTY_MEMBER_INFO
{
	UID32_t			CharacterUniqueNumber;
	ClientIndex_t	ClientIndex;
	char			CharacterName[SIZE_MAX_CHARACTER_NAME];
	//char			MapName[SIZE_MAX_MAP_NAME];	// IS_VALID_MAP_NAME()�� FALSE�̸�, ��Ƽ �Ҽ��̳� �������� �ƴ� ĳ����(�׾ ���� ĳ���� ��)
	MAP_CHANNEL_INDEX	MapChannelIndex;	// IsValid()�� FALSE�̸�, ��Ƽ �Ҽ��̳� �������� �ƴ� ĳ����(�׾ ���� ĳ���� ��)
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	INT		HP;
	INT		DP;
#else
	SHORT	HP;
	SHORT	DP;
#endif
	float			CurrentHP;
	float			CurrentDP;
	SHORT			SP;
	SHORT			CurrentSP;
	SHORT			EP;
	float			CurrentEP;

	// operator overloading
	FIELD_PARTY_MEMBER_INFO& operator=(const CHARACTER& rhs)
	{
		this->CharacterUniqueNumber = rhs.CharacterUniqueNumber;
		this->ClientIndex = rhs.ClientIndex;
		this->HP = rhs.HP;
		this->CurrentHP = rhs.CurrentHP;
		this->DP = rhs.DP;
		this->CurrentDP = rhs.CurrentDP;
		this->SP = rhs.SP;
		this->CurrentSP = rhs.CurrentSP;
		this->EP = rhs.EP;
		this->CurrentEP = rhs.CurrentEP;
		util::strncpy(this->CharacterName, rhs.CharacterName, SIZE_MAX_CHARACTER_NAME);
		this->MapChannelIndex = rhs.MapChannelIndex;
		return *this;
	}
};

struct MSG_FC_PARTY_GET_MEMBER
{
	UID32_t		CharacterUniqueNumber;
};					// ���� ��Ƽ������ �������� Ȯ���Ѵ�

struct MSG_FC_PARTY_PUT_MEMBER
{
	FIELD_PARTY_MEMBER_INFO	FieldMemberInfo;
};

struct MSG_FC_PARTY_GET_ALL_MEMBER
{
	PartyID_t	PartyID;
};

struct MSG_FC_PARTY_PUT_ALL_MEMBER
{
	UID32_t		MasterUniqueNumber;			// ��Ƽ���� CharacterUniqueNumber
	UINT		nNumOfPartyMembers;
	ARRAY_(MSG_FC_PARTY_PUT_MEMBER);
};

// ��Ƽ�� ���� ������Ʈ
struct MSG_FC_PARTY_UPDATE_MEMBER_INFO_ALL
{
	UID32_t		CharacterUniqueNumber;
	BYTE		Level;
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	INT		HP;
	INT		DP;
#else
	SHORT	HP;
	SHORT	DP;
#endif
	float		CurrentHP;
	float		CurrentDP;
	SHORT		SP;
	SHORT		CurrentSP;
	SHORT		EP;
	float		CurrentEP;
	BodyCond_t	BodyCondition;
};

struct MSG_FC_PARTY_UPDATE_MEMBER_INFO_LEVEL
{
	UID32_t		CharacterUniqueNumber;
	BYTE		Level;
};

struct MSG_FC_PARTY_UPDATE_MEMBER_INFO_HP
{
	UID32_t		CharacterUniqueNumber;
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	INT		HP;
#else
	SHORT	HP;
#endif
	float		CurrentHP;
};

struct MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_HP
{
	UID32_t		CharacterUniqueNumber;
	float		CurrentHP;
};

struct MSG_FC_PARTY_UPDATE_MEMBER_INFO_DP
{
	UID32_t		CharacterUniqueNumber;
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	INT		DP;
#else
	SHORT	DP;
#endif
	float		CurrentDP;
};

struct MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_DP
{
	UID32_t		CharacterUniqueNumber;
	float		CurrentDP;
};

struct MSG_FC_PARTY_UPDATE_MEMBER_INFO_SP
{
	UID32_t		CharacterUniqueNumber;
	SHORT		SP;
	SHORT		CurrentSP;
};

struct MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_SP
{
	UID32_t		CharacterUniqueNumber;
	SHORT		CurrentSP;
};

struct MSG_FC_PARTY_UPDATE_MEMBER_INFO_EP
{
	UID32_t		CharacterUniqueNumber;
	SHORT		EP;
	float		CurrentEP;
};

struct MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_EP
{
	UID32_t		CharacterUniqueNumber;
	float		CurrentEP;
};

struct MSG_FC_PARTY_UPDATE_MEMBER_INFO_BODYCONDITION
{
	UID32_t		CharacterUniqueNumber;
	BodyCond_t	BodyCondition;
};

struct MSG_IC_PARTY_UPDATE_MEMBER_INFO_MAPNAME
{
	UID32_t		CharacterUniqueNumber;
	MAP_CHANNEL_INDEX	MapChannelIndex;
};		// ������ �� �̸� ������Ʈ

		// �߹�
struct MSG_IC_PARTY_BAN_MEMBER
{
	PartyID_t	PartyID;
	UID32_t		CharacterUniqueNumber;
};

struct MSG_IC_PARTY_BAN_MEMBER_OK
{
	PartyID_t	PartyID;
	UID32_t		CharacterUniqueNumber;
};

struct MSG_FI_PARTY_BAN_MEMBER_OK
{
	PartyID_t				PartyID;
	UID32_t					CharacterUniqueNumber;
	MEX_FIELD_PARTY_INFO	FieldPartyInfo;
};

// Ż��
struct MSG_IC_PARTY_LEAVE
{
	PartyID_t	PartyID;
};

struct MSG_IC_PARTY_LEAVE_OK
{
	PartyID_t	PartyID;
	UID32_t		CharacterUniqueNumber;
	bool		bMoveToArena;
};

struct MSG_FI_PARTY_LEAVE_OK
{
	PartyID_t				PartyID;
	UID32_t					CharacterUniqueNumber;
	MEX_FIELD_PARTY_INFO	FieldPartyInfo;
};

// ��Ƽ�� ���� �絵
struct MSG_IC_PARTY_TRANSFER_MASTER
{
	PartyID_t	PartyID;
	UID32_t		OldMasterCharacterUniqueNumber;
	UID32_t		NewMasterCharacterUniqueNumber;
};

struct MSG_IC_PARTY_TRANSFER_MASTER_OK
{
	PartyID_t	PartyID;
	UID32_t		OldMasterCharacterUniqueNumber;
	UID32_t		NewMasterCharacterUniqueNumber;
};

struct MSG_FI_PARTY_TRANSFER_MASTER_OK
{
	PartyID_t	PartyID;
	UID32_t		OldMasterCharacterUniqueNumber;
	UID32_t		NewMasterCharacterUniqueNumber;
};

// �ػ�
struct MSG_IC_PARTY_DISMEMBER
{
	PartyID_t	PartyID;
};

struct MSG_IC_PARTY_DISMEMBER_OK
{
	PartyID_t	PartyID;
};

struct MSG_FI_PARTY_DISMEMBER_OK
{
	PartyID_t	PartyID;
};

// ��� ���� ��û
struct MSG_IC_PARTY_CHANGE_FLIGHT_FORMATION
{
	PartyID_t	PartyID;
	BYTE		Formation;				// ��� ���� ����, see below
};	// Cm -> I

	// 2009-08-03 by cmkwon, EP3-4 ��� ���� ��ų ���� - AtumParam.h�� �ű�
	// #define FLIGHT_FORM_NONE				(BYTE)0	// ��� ���� �� ��
	// #define FLIGHT_FORM_2_COLUMN			(BYTE)1	// �̷� ����, �̷� ���� ������� �� �ٷ� ������ �� ����̴�
	// #define FLIGHT_FORM_2_LINE				(BYTE)2	// �̷� Ⱦ��, �̷� Ⱦ�� ������� �� �ٷ� ������ �� ����̴�
	// #define FLIGHT_FORM_TRIANGLE			(BYTE)3	// �ﰢ ���, �ﰢ�� ������� ��ܺ��� 1, 2, 3���� ������ ��ġ�Ѵ�
	// #define FLIGHT_FORM_INVERTED_TRIANGLE	(BYTE)4	// ���ﰢ ���, �� �ﰢ�� ������� ��ܺ��� 3, 2, 1���� ������ ��ġ�Ѵ�
	// #define FLIGHT_FORM_BELL				(BYTE)5	// �� ����, �� ������� ��ܺ��� 1, 3, 2���� ������ ��ġ�Ѵ�
	// #define FLIGHT_FORM_INVERTED_BELL		(BYTE)6	// ���� ����, ���� ������� ��ܺ��� 2, 3, 1���� ������ ��ġ�Ѵ�
	// #define FLIGHT_FORM_X					(BYTE)7 // X�� ����
	// #define FLIGHT_FORM_STAR				(BYTE)8	// �� ����

struct MSG_IC_PARTY_CHANGE_FLIGHT_FORMATION_OK
{
	PartyID_t	PartyID;
	BYTE		Formation;					// ��� ���� ����
};	// I -> C, ��Ƽ�忡�Ե� ����

struct MSG_FI_PARTY_CHANGE_FLIGHT_FORMATION_OK
{
	PartyID_t	PartyID;
	BYTE		Formation;					// ��� ���� ����
};	// I -> C, ��Ƽ�忡�Ե� ����

struct MSG_IC_PARTY_GET_FLIGHT_POSITION
{
	UID32_t		CharacterUniqueNumber;
};			// C -> I -> Cm

struct MSG_IC_PARTY_CHANGE_FLIGHT_POSITION
{
	UID32_t		CharacterUniqueNumber;
	BYTE		FlightPosition;				// �ڱ� �ڽ��� ��� ���� ��ġ
};		// Cm -> I -> C

struct MSG_FI_PARTY_CHANGE_FLIGHT_POSITION
{
	PartyID_t	PartyID;
	UID32_t		CharacterUniqueNumber;
	BYTE		FlightPosition;				// �ڱ� �ڽ��� ��� ���� ��ġ
};		// Cm -> I -> C

		// 2011-02-22 by shcho&hsSon, ������ ���� �ȵǴ� ���� ����
#define FORMATION_SKILL_NULL	0	// �����̼� ���� ������� ����
#define FORMATION_SKILL_ON		1	// �����̼� ���
#define FORMATION_SKILL_OFF		2	// �����̼� ��� ���� �ƴ�
		// END 2011-02-22 by shcho&hsSon, ������ ���� �ȵǴ� ���� ����

struct MSG_IC_PARTY_CANCEL_FLIGHT_POSITION
{
	// 2011-02-22 by shcho&hsSon, ������ ���� �ȵǴ� ���� ����
	bool		Formation_On_Off;
	// end 2011-02-22 by shcho&hsSon, ������ ���� �ȵǴ� ���� ����
	UID32_t		CharacterUniqueNumber;
};		// C -> I -> Cm

struct MSG_FI_PARTY_CANCEL_FLIGHT_POSITION
{
	PartyID_t	PartyID;
	UID32_t		CharacterUniqueNumber;
};		// C -> I -> Cm

struct MSG_IC_PARTY_MEMBER_INVALIDATED
{
	UID32_t		CharacterUniqueNumber;
};			// I -> C, ��Ƽ���� ������������ ���ӿ��� ƨ���� �� ����

struct MSG_IC_PARTY_MEMBER_REJOINED
{
	UID32_t				CharacterUniqueNumber;
	MAP_CHANNEL_INDEX	MapChannelIndex;
};				// I -> C, ��Ƽ���� �ٽ� ������ �����Ͽ��� �� ����, �ڽ��� ������

struct MSG_IC_PARTY_UPDATE_ITEM_POS {
	UID32_t			CharacterUniqueNumber;	// ������ â���� ���ŵ� ��Ƽ��
	BYTE			ItemPosition;			// POS_XXX
	INT				ItemNum;
	// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - ����
	//	INT				ColorCode;				// 2005-12-08 by cmkwon, �Ƹ� ���� Ʃ�� ����
	INT				nShapeItemNum;		// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
	INT				nEffectItemNum;		// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
};				// I->C, ��Ƽ���� ������ ������ �������� �� ����

struct MSG_IC_PARTY_ALL_FLIGHT_POSITION
{
	UID32_t			AllPartyMemberCharacterUniqueNumber[SIZE_MAX_PARTY_MEMBER - 1];
};

struct MSG_IC_PARTY_LIST_INFO
{// 2008-06-02 by dhjin, EP3 ��� ���� - ��� ���� ����Ʈ 
	PartyID_t		StartNum;
};

struct MSG_IC_PARTY_JOIN_FREE
{// 2008-06-03 by dhjin, EP3 ��� ���� - ��� ���� ����
	PartyID_t		PartyNum;
	CHAR			PartyPW[SIZE_MAX_TEAM_PW];
};

struct SPARTY_LIST_INFO
{// 2008-06-02 by dhjin, EP3 ��� ���� - ��� ���� ����Ʈ OK
	PartyID_t	PartyNum;
	CHAR		PartyName[SIZE_MAX_PARTY_NAME];
	BYTE		ExpDistributeType;
	BYTE		ItemDistributeType;
	bool		PartyLock;
	CHAR		PartyPW[SIZE_MAX_TEAM_PW];
	INT			MinLevel;
	INT			MaxLevel;
	CHAR		PartyMasterName[SIZE_MAX_CHARACTER_NAME];
	BYTE		Membercount;
	bool		VoipCheck; // 2017-01-16 panoskj todo : Remove
#ifdef C_RENDER_PARTY_MG_COUNT
	INT			MGCount;
#endif
};

struct MSG_IC_PARTY_LIST_INFO_OK
{// 2008-06-02 by dhjin, EP3 ��� ���� - ��� ���� ����Ʈ OK
	INT				PartyInfoTotalCount;	// 2008-06-02 by dhjin, ��Ƽ �ִ� ���
	INT				PartyInfoListCount;		// 2008-06-02 by dhjin,
	_ARRAY(SPARTY_LIST_INFO);
};

struct MSG_IC_PARTY_CHANGE_INFO
{// 2008-06-04 by dhjin, EP3 ��� ���� - ��� ���� ����
	SPARTY_INFO	PartyInfo;
};
using MSG_IC_PARTY_INFO = MSG_IC_PARTY_CHANGE_INFO;

struct SRECOMMENDATION_MEMBER_INFO
{
	char		CharacterName[SIZE_MAX_CHARACTER_NAME];
	UID32_t		CharacterUID;
};

struct MSG_IC_PARTY_RECOMMENDATION_MEMBER_OK
{// 2008-06-04 by dhjin, EP3 ��� ���� - ��õ �ɸ��� ��û
	UINT		Count;
	ARRAY_(SRECOMMENDATION_MEMBER_INFO);
};


// check: FI_EVENT_GET_WARP_INFO(��Ƽ ����+�̺�Ʈ ����)�� ��ü�Ѵ�. ���� ��Ƽ ������ �޾ƿ;��� �ʿ伺�� ����� �츰��. 20031006.
// �ٸ� �ʵ� ����(��Ƽ ������ ������ ����)���� ������, IM �����κ��� ������ �޾� ��Ƽ ���� ����
//typedef struct
//{
//	UID32_t			CharacterUniqueNumber;
//} MSG_FI_PARTY_GET_PARTY_INFO;
//
//typedef struct
//{
//	UID32_t			AccountUniqueNumber;		// �ٸ� �ʵ� �������� ������, ������ �ʿ���
//	UID32_t			CharacterUniqueNumber;
//	PartyID_t		PartyID;
//	UID32_t			MasterCharacterUniqueNumber;
//	int				NumOfPartyMembers;
//	ARRAY_(FI_PARTY_MEMBER_INFO);
//} MSG_FI_PARTY_GET_PARTY_INFO_OK;

struct FI_PARTY_MEMBER_INFO
{
	UID32_t			CharacterUniqueNumber;
	//	char			CurrentMapName[SIZE_MAX_MAP_NAME];	// check: �ٸ� �ʵ弭���� �ִ� ��Ƽ���� �������� �ʱ�� �ϸ鼭 ����, 20031010, Ȯ��&�����Ǹ� ���� ����
};	// Field Server�� IM Server ���̿� ����ϴ� ��Ƽ�� ����

	// ��� �������� ��Ƽ�� ����
struct MSG_FC_PARTY_REQUEST_PARTY_WARP
{
	int				nPartyMembers;				// ���� ������ ��Ƽ���� ��, ����� ����
	ARRAY_(UID32_t);							// ��Ƽ�� character uniquenumber�� array
};

struct MSG_FC_PARTY_REQUEST_PARTY_WARP_WITH_MAP_NAME
{
	MAP_CHANNEL_INDEX	MapChannelIndex;
	int					nPartyMembers;				// ���� ������ ��Ƽ���� ��, ����� ����
	ARRAY_(UID32_t);								// ��Ƽ�� character uniquenumber�� array
};

//typedef struct
//{
//	INT				ObjectIndex;				// Object�� Index
//	int				nPartyMembers;				// ���� ������ ��Ƽ���� ��, ����� ����
//	ARRAY_(UID32_t);							// ��Ƽ�� character uniquenumber�� array
//} MSG_FC_PARTY_REQUEST_PARTY_OBJECT_EVENT;
struct MSG_FC_PARTY_REQUEST_PARTY_OBJECT_EVENT
{
	DWORD			ObjectType;
	AVECTOR3		ObjectPosition;
	int				nPartyMembers;				// ���� ������ ��Ƽ���� ��, ����� ����
	ARRAY_(UID32_t);							// ��Ƽ�� character uniquenumber�� array
};

// ��Ƽ���� MOVE ���� ��û
struct MSG_FC_PARTY_GET_OTHER_MOVE
{
	UID32_t			OtherCharacterUniqueNumber;
};

// ���ӿ��� �����ٰ� ������ �� ��� ���� ����
struct MSG_IC_PARTY_PUT_LAST_PARTY_INFO
{
	PartyID_t	PartyID;				// �ֱٿ� ����Ҵ� ��Ƽ�� �α��Ͽ��� �� �����ϹǷ�, �׿� ���� �غ� ��û
};		// I -> C, ��Ƽ���� �ٽ� ������ �����Ͽ��� �� ����, �ڱ� �ڽſ��Ը� ����

		// ��Ƽ�� ����
struct MSG_FC_PARTY_BATTLE_START
{
	PartyID_t		PeerPartyID;				// ��� PartyID
	ClientIndex_t	PeerPartyMasterClientIndex;	// ��Ƽ���� ClientIndex
	SHORT			nPeerPartyMemberToBattle;	// ��Ƽ���� ������ ��� ��Ƽ���� ��
	ARRAY_(PEER_PARTY_MEMBER);
};					// F->C, ��� ��Ƽ�� ������ ������.

struct PEER_PARTY_MEMBER
{
	ClientIndex_t	ClientIndex;
	UID32_t			CharacterUniqueNumber;
};

struct MSG_FC_PARTY_BATTLE_END
{
	PartyID_t		PeerPartyID;				// ������ ClientIndex
	USHORT			EndType;					// ���� ���� Ÿ��, BATTLE_END_XXX
};						// F->C, ��Ƽ�� ���

struct MSG_FI_PARTY_NOTIFY_BATTLE_PARTY
{
	PartyID_t		PartyID1;					// ��Ƽ 1
	PartyID_t		PeerPartyID1;				// ��Ƽ 1�� ��� ��Ƽ
	PartyID_t		PartyID2;					// ��Ƽ 2
	PartyID_t		PeerPartyID2;				// ��Ƽ 2�� ��� ��Ƽ
};				// F->I, ��Ƽ���� �˸�

struct MSG_FI_PARTY_NOTIFY_BATTLE_PARTY_OK
{
	PartyID_t		PartyID1;					// ��Ƽ 1
	PartyID_t		PeerPartyID1;				// ��Ƽ 1�� ��� ��Ƽ
	PartyID_t		PartyID2;					// ��Ƽ 2
	PartyID_t		PeerPartyID2;				// ��Ƽ 2�� ��� ��Ƽ
};			// I->F, ��Ƽ���� �˸��� ���� ACK

struct MSG_FC_PARTY_PUT_ITEM_OTHER {
	INT				ItemNum;					// �������� ����
	USHORT			Amount;						// �������� ����
	ClientIndex_t	ClientIndex;
};					// F->C, �ٸ� ��Ƽ���� ������ ��� ���� ����

struct MSG_FI_PARTY_ADD_MEMBER {
	PartyID_t				PartyID;
	UID32_t					CharacterUniqueNumber;	// �߰��� ��Ƽ��
	MEX_FIELD_PARTY_INFO	FieldPartyInfo;
};						// I->F, ��Ƽ���� �߰��϶�� Field Server �˸�

struct MSG_FI_PARTY_DELETE_MEMBER {
	PartyID_t				PartyID;
	UID32_t					CharacterUniqueNumber;	// ������ ��Ƽ��
	MEX_FIELD_PARTY_INFO	FieldPartyInfo;
};					// I->F, ��Ƽ���� �����϶�� Field Server �˸�

struct MSG_FI_PARTY_UPDATE_ITEM_POS {
	UID32_t			CharacterUniqueNumber;	// ������ â���� ���ŵ� ��Ƽ��
	BYTE			ItemPosition;			// POS_XXX
	INT				ItemNum;
	// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - ����
	//	INT				ColorCode;				// 2005-12-08 by cmkwon, �Ƹ� ���� Ʃ�� ����
	INT				nShapeItemNum;		// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
	INT				nEffectItemNum;		// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
};				// F->I, ��Ƽ���� ������ ������ �������� �� ����

struct MSG_FI_PARTY_ALL_FLIGHT_POSITION
{
	PartyID_t		PartyID;
	UID32_t			AllPartyMemberCharacterUniqueNumber[SIZE_MAX_PARTY_MEMBER - 1];
};

struct MSG_FI_PARTY_UPDATE_PARTY_INFO
{
	PartyID_t				PartyID;
	MEX_FIELD_PARTY_INFO	FieldPartyInfo;
};	// I->F, ��Ƽ ������ ������Ʈ

struct MSG_FI_PARTY_CHANGE_EXP_DISTRIBUTE_TYPE
{// 2008-06-04 by dhjin, EP3 ��� ���� - ����ġ �й� ��� ���� 
	PartyID_t	PartyID;
	BYTE		ExpDistributeType;
};

struct MSG_FI_PARTY_CHANGE_ITEM_DISTRIBUTE_TYPE
{// 2008-06-04 by dhjin, EP3 ��� ���� - ������ �й� ��� ���� 
	PartyID_t	PartyID;
	BYTE		ItemDistributeType;
};

struct MSG_FI_PARTY_CHANGE_FORMATION_SKILL		// 2009-08-03 by cmkwon, EP3-4 ��� ���� ��ų ���� - 
{
	UID32_t		PartyMasterCharcUID;	// PartyMaster CharacterUID
	BYTE		Is_On_or_Off;			// TRUE is ON, FALSE is Off
	BYTE		FlightFormation;		// 
};

///////////////////////////////////////////////////////////////////////////////
// FI_CHARACTER
struct MSG_FI_CHARACTER_DELETE_CHARACTER
{
	UID32_t	AccountUniqueNumber;
	UID32_t	CharacterUniqueNumber;
	UID32_t	GuildUniqueNumber;
};

struct MSG_FI_CHARACTER_CHANGE_LEVEL
{
	UID32_t	CharacterUniqueNumber;
	BYTE	Level;
};

struct MSG_FI_CHARACTER_UPDATE_GUILD_INFO
{
	UID32_t	CharacterUniqueNumber;
	char	GuildName[SIZE_MAX_GUILD_NAME];	// ��� �̸�
	UID32_t	GuildUniqueNumber;				// ��� ��ȣ, 0�̸� ��� ����
	bool	GuildDelete;					// 2006-09-29 by dhjin, ��� ���� ����, 1->����
};


// 2007-10-06 by dhjin, �������ڰ� ���� ������ InflWarManager Ŭ������ �������� ������ ����.
struct MSG_FI_CREATE_GUILD_BY_SUBLEADER
{
	unsigned char	Influence;
	int		SubLeaderRank;
	UID32_t CharacterUID;
};

struct MSG_FI_CHARACTER_UPDATE_MAP_CHANNEL
{
	UID32_t				CharacterUniqueNumber;
	MAP_CHANNEL_INDEX	MapChannel;
};

struct MSG_FI_CHARACTER_CHANGE_INFLUENCE_TYPE
{
	UID32_t				CharacterUID;
	BYTE				InfluenceType0;					// 
};

struct MSG_FI_UPDATE_SUBLEADER
{// 2007-02-14 by dhjin, �������� ���� �� IM������ ���� ����.
	UID32_t				CharacterUID;
	BYTE				SubLeaderNum;
	BYTE				InfluenceType;
};

///////////////////////////////////////////////////////////////////////////////
// FC_GUILD (Cleint <-> Field Server)
struct MSG_FC_GUILD_GET_MAP_OWNER_INFO
{
	MapIndex_t	MapIndex;
};			// C->F, �� ���� ���� ��û

struct MSG_FC_GUILD_GET_MAP_OWNER_INFO_OK
{
	char	DefenderGuildName[SIZE_MAX_GUILD_NAME];	// ���� ��� �̸�
	BYTE	NumOfCallengerGuilds;					// ���� ��� ��
};		// F->C, �� ���� ���� ��û ���

struct MSG_FC_GUILD_REQUEST_GUILD_WAR
{
	char	GuildName[SIZE_MAX_GUILD_NAME];
};			// C->F, ������ ��û

struct MSG_FC_GUILD_REQUEST_GUILD_WAR_RESULT
{
	INT		Order;							// ����(1���� ����), 0 �����̸� ��û ����
};	// F->C, ������ ��û ���

struct MSG_FC_GUILD_GET_CHALLENGER_GUILD
{
	UID32_t	DefenderGuildUniqueNumber;		// ���� ��� ���� ��ȣ
};		// C->F, ������ ��û ��� ��� ����Ʈ ��û

struct MSG_FC_GUILD_GET_CHALLENGER_GUILD_OK
{
	BYTE	NumOfCallengerGuilds;			// ���� ��� �̸� ����(�켱 ���� ����)
	ARRAY_(char[SIZE_MAX_GUILD_NAME]);
};		// F->C, ������ ��û ��� ��� ����Ʈ ��û ���

struct MSG_FC_GUILD_GET_WAR_INFO
{
	UID32_t		GuildUID;				// �ڽ��� ���� ����� UID
};			// C->F, ������ ������ ��û

struct MSG_FC_GUILD_GET_WAR_INFO_OK
{
	MAP_CHANNEL_INDEX	MapChannel;		// �������� �Ͼ�� ��
	UID32_t				PeerGuildUID;	// ��� ����� ��� ��ȣ
};			// F->C, ������ ������ ����

struct MSG_FC_GUILD_SUMMON_MEMBER
{
	UID32_t				uidGuildUID;	// ��� UID
	MAP_CHANNEL_INDEX	MapChannel;		// �������� �ִ� ��
	AVECTOR3			PositionVector;	// �������� ��ǥ
};
struct MSG_FC_GUILD_SUMMON_MEMBER_OK
{
	UID32_t				uidGuildUID;	// ��� UID
	MAP_CHANNEL_INDEX	MapChannel;		// �������� �ִ� ��
	AVECTOR3			PositionVector;	// �������� ��ǥ
};

///////////////////////////////////////////////////////////////////////////////
// FI_GUILD (Field Server <-> IM Server)
struct MSG_FI_GUILD_NOTIFY_START_WAR
{
	UID32_t		DefenderGuildUID;
	UID32_t		ChallengerGuildUID;
	MAP_CHANNEL_INDEX	WarMapChannel;
};	// F->I

struct MSG_FI_GUILD_NOTIFY_END_WAR
{
	USHORT		WarEndType;			// BATTLE_END_XXX, BATTLE_END_WIN or BATTLE_END_DEFEAT�̸� ���� ����
	UID32_t		WinnerGuildUID;
	UID32_t		LoserGuildUID;
	MAP_CHANNEL_INDEX	WarMapChannel;
};		// F->I

struct MSG_FI_GUILD_DELETE_GUILD
{
	UID32_t		DeleteGuildUID;
};
using MSG_FI_GUILD_REG_DELETE_GUILD = MSG_FI_GUILD_DELETE_GUILD;		// 2007-11-09 by dhjin

struct MSG_FI_GUILD_ADD_GUILD_FAME	// 2005-12-27 by cmkwon
{
	UID32_t		guildUID;		// 
	int			addValues;		// 1 or 100
};

struct MSG_FI_GUILD_OUTPOST
{// 2008-05-21 by dhjin, EP3 - ���� ���� ���� - �������� ����
	UID32_t		GuildUID;
	MapIndex_t	MapIndex;
};

///////////////////////////////////////////////////////////////////////////////
// IC_GUILD (Cleint <-> IM Server)
struct MSG_IC_GUILD_CREATE
{
	char	GuildName[SIZE_MAX_GUILD_NAME];
};						// C->I, ��� ���� ��û

struct MSG_IC_GUILD_CREATE_OK
{
	char	GuildName[SIZE_MAX_GUILD_NAME];
	UID32_t	GuildUniqueNumber;				// ��� ������ȣ
	char	GuildCommanderUniqueNumber;		// ������
};					// I->C, ��� ���� ���

struct MSG_IC_GUILD_GET_GUILD_INFO
{
	UID32_t	GuildUniqueNumber;				// ��� ������ȣ
};				// C->I, ��� ���� ��û

				// GUILD_STATE_XXX
#define GUILD_STATE_NORMAL				((BYTE)0)	// ���� ��� ����
#define GUILD_STATE_DISMEMBER_READY		((BYTE)1)	// ��� ��ü ��� ����
#define GUILD_STATE_IN_GUILD_WAR		((BYTE)2)	// ����� ����
#define GUILD_STATE_CITYWAR				((BYTE)3)	// ���������� ����

				///////////////////////////////////////////////////////////////////////////////
				// 2007-08-02 by cmkwon, ���� ��ũ �ɻ� �ý��� ���� - GUILD_MARK_STATE_XXX ����
#define GUILD_MARK_STATE_NONE					((BYTE)0)	// ��ũ ���� ����
#define GUILD_MARK_STATE_WAITING_PERMISSION		((BYTE)1)	// ��ũ ����� ��ٸ��� ����, ���ӻ󿡼��� ��� �Ұ�
#define GUILD_MARK_STATE_NORMAL					((BYTE)2)	// ��ũ ��� ���� ����
const char *GetStringGuildMarkState(BYTE i_byGuildMarkState, bool i_bForUser = false);


struct MSG_IC_GUILD_GET_GUILD_INFO_OK
{
	char	GuildName[SIZE_MAX_GUILD_NAME];
	UID32_t	GuildUniqueNumber;				// ��� ������ȣ
	UID32_t	GuildCommanderUniqueNumber;		// ������
	INT		GuildMemberCapacity;			// ��� �ο� ����
	BYTE	NumOfGuildMemberInfo;
	BYTE	GuildState;						// GUILD_STATE_XXX
	UINT	GuildMarkVersion;
	INT		WarWinPoint;					// ����� �¼�
	INT		WarLossPoint;					// ����� �м�
	char    Notice[SIZE_MAX_NOTICE];		// 2008-06-05 by dhjin, EP3 - ���� ���� ����
	MapIndex_t GuildOutPostCityMapIndex;	// 2008-06-05 by dhjin, EP3 - ���� ���� ����
	INT		GuildTotalFame;					// 2008-06-05 by dhjin, EP3 - ���� ���� ����			
	INT		GuildMonthlyFame;				// 2008-06-05 by dhjin, EP3 - ���� ���� ����
	INT		GuildTotalFameRank;				// 2008-06-05 by dhjin, EP3 - ���� ���� ����	
	INT		GuildMonthlyFameRank;			// 2008-06-05 by dhjin, EP3 - ���� ���� ����
	bool    GuildMemberShip;					// 2008-06-20 by dhjin, EP3 - ���� ���� ����
	ATUM_DATE_TIME GuildMemberShipExpireTime;	// 2008-06-20 by dhjin, EP3 - ���� ���� ����
	ARRAY_(MEX_GUILD_MEMBER_INFO);
};			// I->C, ��� ���� ���

			// ���� ���, GUILD_RANK_XXX, BYTE
#define GUILD_RANK_PRIVATE_NULL			((BYTE)0)	// ���Ҽ� ����
#define GUILD_RANK_COMMANDER			((BYTE)1)	// ���࿩����, �����
#define GUILD_RANK_SUBCOMMANDER			((BYTE)2)	// �ο�����				// 2008-05-20 by dhjin, EP3 - ���� ���� ����
#define GUILD_RANK_SQUAD_LEADER_1		((BYTE)3)	// �� 1 ��������
#define GUILD_RANK_PRIVATE_1			((BYTE)4)	// �� 1 �������
#define GUILD_RANK_SQUAD_LEADER_2		((BYTE)5)	// �� 2 ��������
#define GUILD_RANK_PRIVATE_2			((BYTE)6)	// �� 2 �������
#define GUILD_RANK_SQUAD_LEADER_3		((BYTE)7)	// �� 3 ��������
#define GUILD_RANK_PRIVATE_3			((BYTE)8)	// �� 3 �������
#define GUILD_RANK_SQUAD_LEADER_4		((BYTE)9)	// �� 4 ��������
#define GUILD_RANK_PRIVATE_4			((BYTE)10)	// �� 4 �������
#define GUILD_RANK_SQUAD_LEADER_5		((BYTE)11)	// �� 5 ��������
#define GUILD_RANK_PRIVATE_5			((BYTE)12)	// �� 5 �������

			// check: MSG_IC_GUILD_GET_GUILD_INFO�� ����, �ʿ��ϸ� �츲, 20040520, kelovon
			//typedef struct
			//{
			//	UID32_t	GuildUniqueNumber;		// ��� ������ȣ
			//} MSG_IC_GUILD_GET_MEMBER;		// C->I, ���� ���� ��û

class CGuildMember;

struct MEX_GUILD_MEMBER_INFO
{
	char	MemberName[SIZE_MAX_CHARACTER_NAME];
	UID32_t	MemberUniqueNumber;
	BYTE	GuildRank;				// GUILD_RANK_XXX
	BYTE	IsOnline;				// 1: TRUE, 0: FALSE
	INT		UnitKind;				// 2008-05-20 by dhjin, EP3 - ���� ���� ����
	BYTE	MemberLevel;			// 2008-05-20 by dhjin, EP3 - ���� ���� ����	
	EN_CHECK_TYPE	VoipType;		// 2008-07-16 by dhjin, EP3 - Voip ����

#ifdef _ATUM_IM_SERVER
									// operator overloading
	MEX_GUILD_MEMBER_INFO& operator=(const CGuildMember& rhs);
#endif // _ATUM_IM_SERVER
};

// check: MSG_IC_GUILD_GET_GUILD_INFO_OK�� ����, �ʿ��ϸ� �츲, 20040520, kelovon
//typedef struct
//{
//	UID32_t	GuildUniqueNumber;		// ��� ������ȣ
//	BYTE	NumOfGuildMemberInfo;
//	ARRAY_(MEX_GUILD_MEMBER_INFO);
//} MSG_IC_GUILD_GET_MEMBER_OK;		// I->C, ���� ���� ���

class CGuild;

struct MEX_OTHER_GUILD_INFO
{
	char	GuildName[SIZE_MAX_GUILD_NAME];
	UID32_t	GuildUniqueNumber;
	UINT	GuildMarkVersion;

#ifdef _ATUM_IM_SERVER
	// operator overloading
	MEX_OTHER_GUILD_INFO& operator=(const CGuild& rhs);
#endif // _ATUM_IM_SERVER
};

struct MSG_IC_GUILD_GET_OTHER_GUILD_INFO
{
	UID32_t	GuildUniqueNumber;
};

struct MSG_IC_GUILD_GET_OTHER_GUILD_INFO_OK
{
	MEX_OTHER_GUILD_INFO		OtherGuildMemberInfo;
};

struct MSG_IC_GUILD_REQUEST_INVITE
{
	char	InviteeCharacterName[SIZE_MAX_CHARACTER_NAME];		// �ʴ��� ���� ���̵�
};		// C->I, ���� ����, ������ ��û

struct MSG_IC_GUILD_REQUEST_INVITE_QUESTION
{
	UID32_t	GuildUniqueNumber;										// ��� ���� ��ȣ
	char	GuildName[SIZE_MAX_GUILD_NAME];							// ��� �̸�
	char	GuildCommanderCharacterName[SIZE_MAX_CHARACTER_NAME];	// ����� ���̵�
};		// I->C, ���� ����, ����ڿ��� ����

struct MSG_IC_GUILD_ACCEPT_INVITE
{
	UID32_t	GuildUniqueNumber;		// ��� ���� ��ȣ, MSG_IC_GUILD_REQUEST_INVITE_QUESTION�� �� 
};		// C->I, ���� �³�, ������ ��û

struct MSG_IC_GUILD_ACCEPT_INVITE_OK
{
	MEX_GUILD_MEMBER_INFO	MemberInfo;		// �� ������ ����
};			// I->C, ���� �³�, ����� �� �������� ����

struct MSG_IC_GUILD_REJECT_INVITE
{
	UID32_t	GuildUniqueNumber;		// ��� ���� ��ȣ, MSG_IC_GUILD_REQUEST_INVITE_QUESTION�� ��
};		// C->I, ���� �ź�, ������ ��û

struct MSG_IC_GUILD_REJECT_INVITE_OK
{
	char	CharacterName[SIZE_MAX_CHARACTER_NAME];	// ���� �ź��� character�� �̸�
};	// I->C, ���� �ź�, ����ڿ��� ����

struct MSG_IC_GUILD_BAN_MEMBER
{
	UID32_t	MemberUniqueNumber;
};			// C->I, ���� �߹�

struct MSG_IC_GUILD_BAN_MEMBER_OK
{
	UID32_t	MemberUniqueNumber;
};		// I->C, ���� �߹� ���

struct MSG_IC_GUILD_LEAVE
{
	UID32_t	GuildUniqueNumber;
};				// C->I, ���� Ż��

struct MSG_IC_GUILD_LEAVE_OK
{
	UID32_t	MemberUniqueNumber;
	DWORD	Padding[SIZE_MAX_PACKET_PADDING];				// 2011-07-21 by hskim, ���� ���� ���� - ���� ������ ȣȯ �ȵǵ��� ����ü ũ�� �ٲ�
};			// I->C, ���� Ż�� ���

struct MSG_IC_GUILD_DISMEMBER
{
	UID32_t	GuildUniqueNumber;
};			// C->I, ���� ��ü
using MSG_FC_GUILD_DISMEMBER = MSG_IC_GUILD_DISMEMBER;
using MSG_FI_GUILD_DISMEMBER = MSG_IC_GUILD_DISMEMBER;


struct MSG_IC_GUILD_DISMEMBER_OK
{
	char	GuildName[SIZE_MAX_GUILD_NAME];
};		// I->C, ���� ��ü ���

		// GUILD_MEMBER_STATE_XXX
#define GUILD_MEMBER_STATE_OFFLINE		((BYTE)0)
#define GUILD_MEMBER_STATE_ONLINE		((BYTE)1)

struct MSG_IC_GUILD_SET_MEMBER_STATE
{
	UID32_t	MemberUniqueNumber;
	BYTE	GuildMemberState;		// GUILD_MEMBER_STATE_XXX
};	// I->C, ������ ���� ��ȭ(ONLINE, OFFLINE ��)

struct MSG_IC_GUILD_CANCEL_DISMEMBER
{
	UID32_t	GuildUniqueNumber;
};		// C->I, ���� ��ü ��� ��û

struct MSG_IC_GUILD_CANCEL_DISMEMBER_OK
{
	BYTE	GuildState;					// GUILD_STATE_XXX
};		// I->C, ���� ��ü ��� ���

struct MSG_IC_GUILD_GET_DISMEMBER_DATE
{
	UID32_t	GuildUniqueNumber;
};		// C->I, ��� �ػ� �ð� ��û

struct MSG_IC_GUILD_GET_DISMEMBER_DATE_OK
{
	ATUM_DATE_TIME	DismemberDateTime;
};	// C->I, ��� �ػ� �ð� ���

struct MSG_IC_GUILD_CHANGE_GUILD_NAME
{
	char	NewGuildName[SIZE_MAX_GUILD_NAME];
};		// C->I, ���� �̸� ���� ��û

struct MSG_IC_GUILD_CHANGE_GUILD_NAME_OK
{
	char	NewGuildName[SIZE_MAX_GUILD_NAME];
};	// I->C, ���� �̸� ���� ���

struct MSG_IC_GUILD_GET_GUILD_MARK
{
	UID32_t	GuildUniqueNumber;
};			// C->I, ���� ���� ��û

			// MSG_SEQUENCE_XXX
#define MSG_SEQUENCE_SINGLE		((BYTE)0)	// �ܵ� MSG
#define MSG_SEQUENCE_BEGIN		((BYTE)1)	// LONG MSG�� ����
#define MSG_SEQUENCE_MIDDLE		((BYTE)2)	// LONG MSG�� �߰�
#define MSG_SEQUENCE_END		((BYTE)3)	// LONG MSG�� ��

struct MSG_IC_GUILD_GET_GUILD_MARK_OK
{
	UID32_t	GuildUniqueNumber;
	UINT	GuildMarkVersion;
	UINT	SizeOfGuildMark;			// ��� ������ size <= SIZE_MAX_GUILD_MARK_IMAGE
	ARRAY_(char);						// ��� ����, image(bmp, gif, jpg, ...)
};		// I->C, ���� ���� ���

struct MSG_IC_GUILD_SET_GUILD_MARK
{
	UINT	SizeOfGuildMark;			// ��� ������ size <= SIZE_MAX_GUILD_MARK_IMAGE
	ARRAY_(char);						// ��� ����, image(bmp, gif, jpg, ...)
};			// C->I, ���� ���� ���� ��û

struct MSG_IC_GUILD_SET_GUILD_MARK_OK
{
	UINT	GuildMarkVersion;
	UINT	SizeOfGuildMark;			// ��� ������ size <= SIZE_MAX_GUILD_MARK_IMAGE
	ARRAY_(char);						// ��� ����, image(bmp, gif, jpg, ...)
};		// I->C, ���� ���� ���� ���

struct MSG_IC_GUILD_SET_RANK
{
	UID32_t	MemberUniqueNumber;
	BYTE	GuildRank;					// GUILD_RANK_XXX
};				// C->I, ��� ����

struct MSG_IC_GUILD_SET_RANK_OK
{
	UID32_t	MemberUniqueNumber;
	BYTE	GuildRank;					// GUILD_RANK_XXX
};				// I->C, ��� ���� ���

struct MSG_IC_GUILD_CHANGE_GUILD_STATE
{
	BYTE	GuildState;					// GUILD_STATE_XXX
};		// I->C, ���� ���� ����

struct MSG_IC_GUILD_LOADING_GUILD_DONE
{
	UID32_t	GuildUniqueNumber;
};		// I->C, ���������� ��� ���� �ε� �Ϸ� �˸�

struct MSG_IC_GUILD_WAR_READY
{
	UID32_t		PeerGuildUID;			// ��� ���
	INT			WaitingTime;			// ��� �ð�, ���� ��õ� �ð� �Ŀ� ������ ����
};				// I->C, ������ ���� ��� ���¸� �˸�

struct MSG_IC_GUILD_START_WAR
{
	UID32_t		PeerGuildUID;			// ��� ���
};				// I->C, ������ ������ �˸�

struct MSG_IC_GUILD_END_WAR
{
	USHORT		WarEndType;				// ���, BATTLE_END_XXX
	char		PeerGuildName[SIZE_MAX_GUILD_NAME];	// ��� ��� �̸�
};					// I->C, ������ ���Ḧ �˸�

struct MSG_IC_GUILD_UPDATE_WAR_POINT
{
	INT			WarWinPoint;			// ����� �¼�
	INT			WarLossPoint;			// ����� �м�
};

struct MSG_IC_GUILD_CHANGE_MEMBER_CAPACITY
{
	UID32_t		guildUID;				// ��� UID
	INT			nMemberCapacity;		// ���� ���� �ο�
	INT			IncreaseCapacity;		// 2008-05-28 by dhjin, EP3 ���� ���� ���� - ���ܿ� ���� ĳ�� ������
};

struct MSG_IC_GUILD_GET_GUILD_MEMBER_LIST_OK
{
	UID32_t	GuildUniqueNumber;				// ��� ������ȣ
	BYTE	NumOfGuildMemberInfo;
	ARRAY_(MEX_GUILD_MEMBER_INFO);
};

struct MSG_IC_GUILD_END_WAR_ADMIN_NOTIFY		// 2006-08-09 by cmkwon
{
	USHORT		WarEndType;								// ���, BATTLE_END_XXX
	char		WinerGuildName[SIZE_MAX_GUILD_NAME];	// �¸� ��� �̸�
	char		LoserGuildName[SIZE_MAX_GUILD_NAME];	// �й� ��� �̸�
};

struct MSG_IC_GUILD_MEMBER_LEVEL_UP
{// 2008-05-20 by dhjin, EP3 - ���� ���� ���� - ���ܿ� ������ ����
	UID32_t		CharacterUID;
	INT			Level;
};

struct MSG_IC_GUILD_NEW_COMMANDER
{// 2008-05-20 by dhjin, EP3 - ���� ���� ���� - ������ ����
	UID32_t		NewCommanderUID;
};

struct MSG_IC_GUILD_NOTICE_WRITE
{// 2008-05-20 by dhjin, EP3 - ���� ���� ���� - ���� ����
	char		Notice[SIZE_MAX_NOTICE];
};
using MSG_IC_GUILD_NOTICE_WRITE_OK = MSG_IC_GUILD_NOTICE_WRITE;

struct MSG_IC_GUILD_GET_INTRODUCTION_OK
{// 2008-05-27 by dhjin, EP3 - ���� ���� ���� - ���� ������ ����
	char		GuildIntroduction[SIZE_MAX_NOTICE];
};

struct MSG_IC_GUILD_GET_APPLICANT_OK
{// 2008-05-27 by dhjin, EP3 - ���� ���� ���� - ���� ������ ����
	UID32_t		CharacterUID;
	char		CharacterName[SIZE_MAX_CHARACTER_NAME];
	BYTE		UnitKind;
	INT			Level;
};

struct MSG_IC_GUILD_GET_SELF_INTRODUCTION
{// 2008-05-27 by dhjin, EP3 - ���� ���� ���� - ���� ������ �Ұ��� 
	UID32_t		CharacterUID;
};

struct MSG_IC_GUILD_GET_SELF_INTRODUCTION_OK
{// 2008-05-27 by dhjin, EP3 - ���� ���� ���� - ���� ������ �Ұ��� 
	char		GuildName[SIZE_MAX_GUILD_NAME];
	char		SelfIntroduction[SIZE_MAX_NOTICE];
};

struct MSG_IC_GUILD_SEARCH_INTRODUCTION_OK
{// 2008-05-27 by dhjin, EP3 - ���� ���� ���� - ���� �Ұ� �˻�
	UID32_t			GuildUID;
	char			GuildName[SIZE_MAX_GUILD_NAME];
	ATUM_DATE_TIME	WriteDate;
	char			GuildIntroduction[SIZE_MAX_NOTICE];
	char			CharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_IC_GUILD_UPDATE_INTRODUCTION
{// 2008-05-28 by dhjin, EP3 - ���� ���� ���� - ���� �Ұ� �ۼ� 
	char			GuildIntroduction[SIZE_MAX_NOTICE];
};

struct MSG_IC_GUILD_UPDATE_SELFINTRODUCTION
{// 2008-05-28 by dhjin, EP3 - ���� ���� ���� - �ڱ� �Ұ� �ۼ�
	UID32_t			TargetGuildUID;
	char			SelfIntroduction[SIZE_MAX_NOTICE];
};

struct MSG_IC_GUILD_CHANGE_FAME_RANK
{// 2008-06-10 by dhjin, EP3 - ���� ���� ���� - ���� �� ����
	INT		GuildTotalFameRank;
	INT		GuildMonthlyFameRank;
};

struct MSG_IC_GUILD_APPLICANT_INVITE
{// 2008-06-12 by dhjin, EP3 - ���� ���� ���� - ������ �������� ���ܿ� �ʴ�
	UID32_t		CharacterUID;
	char		CharacterName[SIZE_MAX_CHARACTER_NAME];
	BYTE		UnitKind;
	INT			Level;
};

struct MSG_IC_GUILD_APPLICANT_REJECT_INVITE
{// 2008-06-12 by dhjin, EP3 - ���� ���� ���� - ������ �������� ���ܿ� �ʴ� �ź� 
	UID32_t		CharacterUID;
	char		GuildName[SIZE_MAX_GUILD_NAME];
};

struct MSG_IC_GUILD_CHANGE_MEMBERSHIP
{// I->C, // 2008-06-20 by dhjin, EP3 - ���� ���� ���� - ������ �ɹ��� ���� ����
	bool			MemberShip;
	ATUM_DATE_TIME	MemberShipExpireTime;
};

///////////////////////////////////////////////////////////////////////////////
// FC_QUEST
struct MEX_QUEST_INFO						// Character�� ����Ʈ ����
{
	INT				QuestIndex;				// ����Ʈ ��ȣ
	BYTE			QuestState;				// �Ϸ�, ������
	LONGLONG		QuestPlayTimeStamp;		// ����Ʈ ���� ���� TotalPlayTime
	//char			szCityWarServerGroupName[SIZE_MAX_SERVER_NAME];
};

struct MEX_QUEST_MONSTER_COUNT						// Character�� ����Ʈ ����
{
	INT				QuestIndex;				// ����Ʈ ��ȣ
	INT				MonsterUniqueNumber;
	INT				Count;
};

struct MSG_FC_QUEST_REQUEST_START
{
	INT			QuestIndex;
	BYTE		QuestStartType;			// QUEST_START_TYPE_XXX
	BYTE		PassQuest;				// ����Ʈ�� �׳� �ѱ�, CQuest::IsDiscardable�� TRUE�� ���� ó����
	AVECTOR3	AVec3QuestPosition;		// ����Ʈ ���۽� ��ǥ, 2005-08-29 by cmkwon
	char		szCouponNumber[SIZE_MAX_COUPON_NUMBER];
};		// C->F, Quest ������ ��û

struct MSG_FC_QUEST_REQUEST_START_RESULT
{
	MEX_QUEST_INFO	MexQuestInfo;	// ����Ʈ ����
	float			fVCNInflDistributionPercent;	// 2006-02-08 by cmkwon, ���������� ���Ա� ���º��� - ���¼��� �̼ǽø� ����
	float			fANIInflDistributionPercent;	// 2006-02-08 by cmkwon, �˸��� ���Ա� - ���¼��� �̼ǽø� ����
};	// F->C, Quest ������ ����, Client�� �� MSG�� ������ Pre NPCTalk�� �ε��Ѵ�

struct MSG_FC_QUEST_ACCEPT_QUEST
{
	INT		QuestIndex;
};	// C->F, Quest�� �޾Ƶ���

struct MSG_FC_QUEST_CANCEL_QUEST
{
	INT		QuestIndex;
};	// C->F, Quest�� ������(������ �� ���� Quest�� ������)

	// check: no body, �ʿ��ϸ� �츲
	//typedef struct {
	//} MSG_FC_QUEST_LEAVE_NPC;		// C->F, NPC�� ����
	//
	//typedef struct {
	//} MSG_FC_QUEST_LEAVE_NPC_OK;	// F->C, NPC�� ������ ���� ACK

struct MSG_FC_QUEST_REQUEST_SUCCESS
{
	INT			QuestIndex;
	INT			QuestResult;			// ����Ʈ�� ���� Ȥ�� ����, 0�̸� ����, 10~13(A~D)�̸� �Ѱ��ֱ�, check: ����� ������� �Ѱ��ֱ�, 20040224, kelovon
	INT			SpentTimeInSeconds;		// ����Ʈ ���� �� ���� �ð�, 20041129, kelovon, ���������� Ŭ���̾�Ʈ �ð��� �������� ����
	AVECTOR3	AVec3QuestPosition;		// ����Ʈ �Ϸ�� ��ǥ, 2005-08-29 by cmkwon
};	// C->F, Quest ��� ������ ��û

struct QUEST_PAY_ITEM_INFO
{
	INT		ItemNum;					// 2007-07-09 by dhjin, ���� ������ ��ȣ 
	INT		ItemCount;					// 2007-07-09 by dhjin, ���� ������ ��
};

struct MSG_FC_QUEST_REQUEST_SUCCESS_RESULT
{
	INT		QuestIndex;
	bool	IsSuccessful;
	INT		QuestResult;		// ����Ʈ�� ���� Ȥ�� ����, 0�̸� ����, 10~13(A~D)�̸� �Ѱ��ֱ�, check: ����� ������� �Ѱ��ֱ�, 20040224, kelovon
	INT		ExpOfCompensation;			// 2007-03-06 by cmkwon, ����Ʈ �Ϸ�� ������ �߰��� ���� ����ġ
	BYTE	BonusStatOfCompensation;	// 2007-07-09 by dhjin, BonusStat ���� ���� 
	INT		QuestPayInfoListCount;		// 2007-07-09 by dhjin, ������ ���� ��
	ARRAY_(QUEST_PAY_ITEM_INFO);
};	// F->C, Quest ���(�Ϸ�)�� �˸�, Client�� �� MSG�� ������ quest�� �����ϰ� After NPCTalk�� �ε��Ѵ�

struct MSG_FC_QUEST_PUT_ALL_QUEST
{
	float			fVCNInflDistributionPercent;	// 2006-02-08 by cmkwon, ���������� ���Ա� ���º��� - ���¼��� �̼ǽø� ����
	float			fANIInflDistributionPercent;	// 2006-02-08 by cmkwon, �˸��� ���Ա� - ���¼��� �̼ǽø� ����
	INT		NumOfQuest;
	ARRAY_(MEX_QUEST_INFO);		// QuestIndex�� array, see below
};			// F->C, �Ϸ�ǰų� �������� ��� ����Ʈ�� ����, ���� ���� �� ������ ���� �� �̾ ����

struct MSG_FC_QUEST_DISCARD_QUEST
{
	INT		QuestIndex;
};		// C->F, �̹� ���۵� ����Ʈ�� ������

struct MSG_FC_QUEST_DISCARD_QUEST_OK
{
	INT		QuestIndex;
	BYTE	IsDiscarded;			// ����Ʈ ���� ���� ����, 1: TRUE, 0:FALSE
};	// F->C, �̹� ���۵� ����Ʈ�� �����Կ� ���� ���

struct MSG_FC_QUEST_MOVE_QUEST_MAP
{
	INT		QuestIndex0;
};

struct MSG_FC_QUEST_PUT_ALL_QUEST_MONSTER_COUNT
{
	INT		NumOfMonsterCount;
	ARRAY_(MEX_QUEST_MONSTER_COUNT);
};	// 2005-10-25 by cmkwon

using MSG_FC_QUEST_UPDATE_MONSTER_COUNT = MEX_QUEST_MONSTER_COUNT; 	// 2005-10-25 by cmkwon

using MSG_FC_QUEST_REQUEST_SUCCESS_CHECK = MSG_FC_QUEST_REQUEST_SUCCESS;			// 2006-03-24 by cmkwon

using MSG_FC_QUEST_REQUEST_SUCCESS_CHECK_RESULT = MSG_FC_QUEST_REQUEST_SUCCESS_RESULT;	// 2006-03-24 by cmkwon

struct MSG_FC_QUEST_REQUEST_PARTY_WARP			// 2006-10-16 by cmkwon
{
	UID32_t				callerCharacterUID;			// ȣ���� CharcterUID
	DWORD				dwRemainTime;				// ����:ms(ex> 1000 <== 1��)
	MAP_CHANNEL_INDEX	warpMapChannIndex;
};

struct MSG_FC_QUEST_REQUEST_PARTY_WARP_ACK		// 2006-10-16 by cmkwon
{
	UID32_t				callerCharacterUID;
	MAP_CHANNEL_INDEX	warpMapChannIndex;
};

///////////////////////////////////////////////////////////////////////////////
// FC_MONSTER (Client <-> Field server)

using MSG_FC_MONSTER_CREATED = MSG_FC_CHARACTER_GET_MONSTER_INFO_OK;

struct MSG_FC_MONSTER_MOVE_OK
{
	ClientIndex_t	MonsterIndex;
	ClientIndex_t	TargetIndex;
	AVECTOR3		PositionVector;
	AVECTOR3		TargetVector;
};

struct MSG_FC_MONSTER_HIDE
{
	ClientIndex_t	MonsterIndex;
};

using MSG_FC_MONSTER_SHOW = MSG_FC_MONSTER_HIDE;

struct MSG_FC_MONSTER_CHANGE_HP
{
	ClientIndex_t	MonsterIndex;
	INT				CurrentHP;
};					// F -> C, ������ ���� HP�� ������

struct MSG_FC_MONSTER_CHANGE_BODYCONDITION
{
	ClientIndex_t	MonsterIndex;
	BodyCond_t		BodyCondition;					// ���� 32bit�� ��� <-- check: ��ų bodycon�� �������� �����ϱ�!
};

struct MSG_FC_MONSTER_SKILL_USE_SKILL
{
	ClientIndex_t		MonsterIndex;				// ��ų�� ����� ����
	ClientIndex_t		ClientIndex;				// ���� ��ų�� ���Ḧ �˷��� Ŭ���̾�Ʈ �ε���
	INT					SkillItemNum;				// ��ų�� ItemNum
};

// start 2011-05-17 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ���� ��ü ���
struct MSG_FC_MONSTER_CHANGE_INDEX
{
	ClientIndex_t	MonsterIndex;
	INT				ChangeMonsterUnitKind;
};					// F -> C, ���� ��ü �޽��� ����
					// end 2011-05-17 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ���� ��ü ���

struct MSG_FC_MONSTER_SKILL_END_SKILL
{
	ClientIndex_t	MonsterIndex;
	AVECTOR3		PositionVector;
	AVECTOR3		TargetVector;
};

struct MSG_FC_MONSTER_SUMMON_MONSTER
{
	ClientIndex_t	clientIdx;
	INT				SummonMonsterUnitKind;			// ���� ������ȣ
	INT				nSummonCounts;
	BYTE			byMonsterTargetType2;
	int				nTargetTypeData2;
	AVECTOR3		PositionAVector;
};
struct MSG_FC_MONSTER_TUTORIAL_MONSTER_DEAD_NOTIFY
{
	ClientIndex_t	monsterIdx;
};
struct MSG_FC_MONSTER_TUTORIAL_MONSTER_DELETE
{
	ClientIndex_t	monsterIdx;
};

///////////////////////////////////////////////////////////////////////////////
// FN_MONSTER (Field server <-> NPC server)
struct MSG_FN_MAPPROJECT_START_OK
{
	USHORT		CreateChannelCounts;
	bool		bCityWarStarted;
	UID32_t		CityWarOccupyGuildUID;
	ARRAY_(MSG_FN_CONNECT_SET_CHANNEL_STATE);
};

struct MSG_FN_MONSTER_CREATE
{
	ChannelIndex_t			ChannelIndex;
	ClientIndex_t			MonsterIndex;				// �ε���
	INT						MonsterUnitKind;				// ���� ������ȣ
	BYTE					MonsterTargetType1;
	int						TargetTypeData1;
	ClientIndex_t			CltIdxForTargetType1;
	BodyCond_t				BodyCondition;					// ���� BodyCondition
	AVECTOR3				PositionVector;					// ���� ��ǥ
	AVECTOR3				TargetVector;					//
	BYTE					ObjectMonsterType;
	MONSTER_BALANCE_DATA	MonsterBalanceData;				// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ���� �뷱�� ���� �߰�.
};	// check: ���ʿ��� ������ ���־� ���� ������? Ȯ�� ���!

struct MSG_FN_MONSTER_CREATE_OK
{
	ChannelIndex_t	ChannelIndex;
	ClientIndex_t	MonsterIndex;
};

struct MSG_FN_MONSTER_DELETE
{
	ChannelIndex_t	ChannelIndex;
	ClientIndex_t	MonsterIndex;
	bool			CinemaDelete;		// 2011-05-30 by hskim, ���Ǵ�Ƽ 3�� - ���� ���� Ŭ���̾�Ʈ �ݿ� - �ó׸����� �����Ѱ�� TRUE
};

// start 2011-05-11 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ���� ��� �߰� - �ش� ��ä�� Ư�� ���� ���� ��� �߰�
struct MSG_FN_MONSTER_CHANGE_OK
{
	ChannelIndex_t	ChannelIndex;
	ClientIndex_t	MonsterIndex;
	INT				ChangeMonsterUnitKind;
};
// end 2011-05-11 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ���� ��� �߰� - �ش� ��ä�� Ư�� ���� ���� ��� �߰�

// start 2011-05-30 by hskim, ���Ǵ�Ƽ 3�� - ���� ���� Ŭ���̾�Ʈ �ݿ�
struct MSG_FC_MONSTER_CINEMA_DELETE_NOTIFY
{
	ClientIndex_t	MonsterIdx;
};
// end 2011-05-30 by hskim, ���Ǵ�Ƽ 3�� - ���� ���� Ŭ���̾�Ʈ �ݿ�

struct MSG_FN_MONSTER_MOVE
{
	ChannelIndex_t	ChannelIndex;
	ClientIndex_t	ClientIndex;
	ClientIndex_t	TargetIndex;
	AVECTOR3		PositionVector;
	AVECTOR3		TargetVector;
	USHORT			usSendRange;
};

struct MSG_FN_MOVE_OK
{
	ChannelIndex_t	ChannelIndex;
	ClientIndex_t	ClientIndex;
	AVECTOR3		PositionVector;
	AVECTOR3		TargetVector;
};

struct MEX_CHARACTER_1
{
	USHORT			Race;						// ����
	USHORT			UnitKind;
	BYTE			InfluenceType0;				// 2005-12-27 by cmkwon
	BYTE			CharacterMode0;				// 2005-07-15 by cmkwon
	UID32_t			CharacterUniqueNumber;
	UID32_t			GuildUID;
	BodyCond_t		BodyCondition;				// ����, bit flag ���
	float			CurrentHP;					// ������
	AVECTOR3		PositionVector;				// ĳ���� ��ǥ

	MEX_CHARACTER_1 &operator=(const CHARACTER &rhs)
	{
		Race = rhs.Race;
		UnitKind = rhs.UnitKind;
		InfluenceType0 = rhs.InfluenceType;
		CharacterMode0 = rhs.CharacterMode;
		CharacterUniqueNumber = rhs.CharacterUniqueNumber;
		GuildUID = rhs.GuildUniqueNumber;
		BodyCondition = rhs.BodyCondition;
		CurrentHP = rhs.CurrentHP;
		PositionVector = rhs.PositionVector;
		return *this;
	};
};
struct MSG_FN_CLIENT_GAMESTART_OK
{
	ChannelIndex_t	ChannelIndex;
	ClientIndex_t	ClientIndex;
	MEX_CHARACTER_1	mexCharacter;
	UID32_t			GuildMasterCharUID;
	bool			bStealthState1;
	bool			bInvisible;			// 2006-11-27 by dhjin, ĳ���� �Ⱥ��̱� �÷���
};

struct MSG_FN_GET_CHARACTER_INFO
{
	ChannelIndex_t	ChannelIndex;
	ClientIndex_t	ClientIndex;
};

using MSG_FN_GET_CHARACTER_INFO_OK = MSG_FN_CLIENT_GAMESTART_OK;

struct MSG_FN_ADMIN_SUMMON_MONSTER
{
	ChannelIndex_t			ChannelIndex;
	char					CharacterName[SIZE_MAX_CHARACTER_NAME];
	INT						MonsterUnitKind;
	BYTE					MonsterTargetType1;
	int						TargetTypeData1;
	ClientIndex_t			CltIdxForTargetType1;
	INT						NumOfMonster;
	AVECTOR3				Position;

	// 2010. 06. 08 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (�Ʊ� ���� �뷱�� ����.)
	MONSTER_BALANCE_DATA	MonsterBalanceData;

};

struct MSG_FN_MONSTER_CHANGE_HP
{
	ChannelIndex_t		ChannelIndex;
	ClientIndex_t		MonsterIndex;
	float				CurrentHP;
};

struct MSG_FN_MONSTER_CHANGE_BODYCONDITION : MSG_FC_CHARACTER_CHANGE_BODYCONDITION
{
	ChannelIndex_t		ChannelIndex;
};

struct MSG_FN_MONSTER_SKILL_USE_SKILL
{
	ChannelIndex_t		ChannelIndex;
	ClientIndex_t		MonsterIndex;				// ��ų�� ����� ����
	ClientIndex_t		ClientIndex;				// ���� ��ų�� ���Ḧ �˷��� Ŭ���̾�Ʈ �ε���
	INT					SkillItemNum;				// ��ų�� ItemNum
};

struct MSG_FN_MONSTER_SKILL_END_SKILL
{
	ChannelIndex_t		ChannelIndex;
	ClientIndex_t		MonsterIndex;
	AVECTOR3			PositionVector;
	AVECTOR3			TargetVector;
};

struct MSG_FN_MONSTER_AUTO_DESTROYED		// 2006-04-17 by cmkwon
{
	MAP_CHANNEL_INDEX	MapChannIdx;				// 2007-11-26 by cmkwon, ���� �ڵ����� �޽��� TCP�� ����(N->F) - MSG_FN_MONSTER_AUTO_DESTROYED �� �ʵ� �߰�
													// 2007-11-26 by cmkwon, ���� �ڵ����� �޽��� TCP�� ����(N->F) - MapChannIdx �� �߰�
													//	ChannelIndex_t		ChannelIndex;
	ClientIndex_t		MonsterIndex;
	BodyCond_t			BodyCondition;				// 2007-11-26 by cmkwon, ���� �ڵ����� �޽��� TCP�� ����(N->F) - MSG_FN_MONSTER_AUTO_DESTROYED �� �ʵ� �߰�
};

struct MSG_FN_MONSTER_STRATEGYPOINT_INIT		// 2006-11-20 by cmkwon
{
	ChannelIndex_t		ChannelIndex;
	int					bVCNMapInflTyforInit;	// 2007-09-16 by dhjin, Belligerence �˻����� �������� ���Ͽ� ������Ʈ Bell������ ���� // 2006-11-21 by cmkwon, �ʱ�ȭ �� ���� VCN���� ����(TRUE=IS_MAP_INFLUENCE_VCN, FALSE=IS_MAP_INFLUENCE_ANI)
	bool				bCreateFlag;			// �ʱ�ȭ �Ŀ� ��ȯ ���� �÷���
	bool				bInfluenceBoss;			// 2007-08-30 by dhjin, ������ ��������Ʈ ��ȯ���� üũ ��Ʈ 0 : FASLE, 1 : TRUE
};

struct MSG_FN_MONSTER_STRATEGYPOINT_SUMMON
{
	MapIndex_t			MapIndex;
};

struct MSG_FN_MONSTER_OUTPOST_INIT
{// 2007-08-24 by dhjin, �������� �� ���� �ʱ�ȭ �� ������ �°� ���͸� ��ȯ�ؾ��� 
	MAP_CHANNEL_INDEX	mapChann;
	int					bell1;			// �⺻���� -1 �̾�� ��
	int					bell2;			// �⺻���� -1 �̾�� ��
	int					bell3;			// �⺻���� -1 �̾�� ��
};

struct MSG_FN_MONSTER_OUTPOST_RESET_SUMMON
{// 2007-08-24 by dhjin, �������� ���� ��ȯ, ����~!! ��ȣ���� �ı��� ���� ������ ��ȯ�ؾ��Ѵ�.
	MAP_CHANNEL_INDEX	mapChann;
	int					bell1;			// �⺻���� -1 �̾�� ��
};
using MSG_FN_MONSTER_TELEPORT_SUMMON = MSG_FN_MONSTER_OUTPOST_RESET_SUMMON;

struct MSG_MONSTER_SUMMON_BY_BELL
{// 2007-09-19 by cmkwon, ����ü �߰�
	int					MonsterBell;	// �⺻���� -1 �̾�� ��
};



struct MSG_FN_NPCSERVER_SUMMON_JACO_MONSTER		// 2006-04-18 by cmkwon
{
	BYTE			Belligerence0;				// 
	bool			IsSummonJacoMonster;		// 
};

struct MSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL		// TCP:F->N, // 2007-08-22 by cmkwon, �ش� ��ä�� ���� ��� �����ϱ� ��� �߰�
{
	MAP_CHANNEL_INDEX	mapChann;
	bool				bAllFlag;		// FALSE�� �ƴϸ� �Ʒ��� �ʵ�鿡 ������� ��� ���Ͱ� ������
	int					bell1;			// �⺻���� -1 �̾�� ��
	int					bell2;			// �⺻���� -1 �̾�� ��
	int					excludeBell1;	// �⺻���� -1 �̾�� ��
	int					excludeBell2;	// �⺻���� -1 �̾�� ��
	bool				bNotCreateMonster;	// ���� ���� ���� 0 => False 1 => True
};

struct MSG_FN_MONSTER_CREATE_IN_MAPCHANNEL_BYVALUE		// TCP:F->N, // 2007-08-29 by dhjin, �ش� ��ä�� ���� ��� ���������ϰ� ����
{
	MAP_CHANNEL_INDEX	mapChann;
};


struct MSG_FN_MISSILE_MOVE
{
	ChannelIndex_t	ChannelIndex;
	UINT			ItemNum;						// ���� ������ ����
	ClientIndex_t	MonsterIndex;					// ���� ������ �ε���
	INT				WeaponIndex;					// ���� ���� �ε���(���Ϳ� ���� ������)
	AVECTOR3		PositionVector;
	AVECTOR3		TargetVector;
};

struct MSG_FN_MONSTER_HPRECOVERY
{
	ChannelIndex_t	ChannelIndex;
	ClientIndex_t	MonsterIndex;
	UINT			RecoveryHP;
};


struct MSG_FN_MONSTER_HIDE
{
	ChannelIndex_t	ChannelIndex;
	ClientIndex_t	ClientIndex;
};
using MSG_FN_MONSTER_SHOW = MSG_FN_MONSTER_HIDE;

///////////////////////////////////////////////////////////////////////////////
// FC_EVENT (Field server <-> Client)

// ���� Ÿ��: WARP_TYPE_XXX
#define WARP_TYPE_SAME_MAP				(BYTE)0x00	// ���� �������� ����
#define WARP_TYPE_SAME_FIELD_SERVER		(BYTE)0x01	// �ٸ� ��(���� �ʵ� ����)������ ����
#define WARP_TYPE_OTHER_FIELD_SERVER	(BYTE)0x02	// �ٸ� ��(�ٸ� �ʵ� ����)������ ����

struct MSG_FC_EVENT_WARP
{
	MAP_CHANNEL_INDEX	MapChannelIndex;
	SERVER_ID			FieldServerID;
	USHORT				WarpAreaIndex;
};			// F->C, �ٸ� �ʵ� ������ ������

struct MSG_FC_EVENT_WARP_SAME_MAP
{
	AVECTOR3			PositionVector;
	BYTE				CharacterMode0;		// 2005-07-27 by cmkwon
};				// F->C, ���� �ʵ� ���� & ���� ������ ������

struct MSG_FC_EVENT_WARP_SAME_FIELD_SERVER
{
	MAP_CHANNEL_INDEX	MapChannelIndex;
	BitFlag16_t			MapWeather;
	AVECTOR3			PositionVector;
	BYTE				CharacterMode0;		// 2005-07-27 by cmkwon
};		// F->C, ���� �ʵ� ���� & �ٸ� ������ ������

struct MSG_FC_EVENT_OTHER_WARPED
{
	ClientIndex_t		ClientIndex;
	MapIndex_t			WarpMapIndex;	// 2005-11-01 by cmkwon, �̵� �� MapIndex
};

struct MSG_FC_EVENT_WARP_CONNECT
{
	char	AccountName[SIZE_MAX_ACCOUNT_NAME];
	char	Password[SIZE_MAX_PASSWORD_MD5_STRING];		// MD5
	char	PrivateIP[SIZE_MAX_IPADDRESS];
	UID32_t	AccountUniqueNumber;
	UID32_t	CharacterUniqueNumber;
	UINT	PartyNumber;
	MAP_CHANNEL_INDEX	MapChannelIndex;
	USHORT	WarpAreaIndex;
};

struct MSG_FC_EVENT_WARP_CONNECT_OK
{
	ClientIndex_t	ClientIndex;
	AVECTOR3		PositionVector;
	AVECTOR3		TargetVector;
	BitFlag16_t		MapWeather;
};

struct MSG_FC_EVENT_ENTER_BUILDING
{
	BUILDINGNPC		BuildingNPCInfo;
	// 2006-02-08 by cmkwon
	//	float			fCityWarTexRate;				//
	float			fInflDistributionTexPercent;	// 2006-02-08 by cmkwon, ���º��� ����
};				// F->C

struct MSG_FC_EVENT_ENTER_BUILDING_OK
{
	UINT		BuildingIndex;
	bool		SendShopItemList;
};			// C->F

struct MSG_FC_EVENT_LEAVE_BUILDING
{
	UINT		BuildingIndex;
};				// C->F

struct MSG_FC_EVENT_LEAVE_BUILDING_OK
{
	AVECTOR3	PositionVector;
	AVECTOR3	TargetVector;
};			// F->C

struct MSG_FC_EVENT_REQUEST_WARP
{
	ClientIndex_t	ClientIndex;
};				// C->F, ��Ƽ�� ������ ��Ƽ�常 ��û ����, �������� ��û �Ұ�

struct MSG_FC_EVENT_REJECT_WARP
{
	Err_t			Reason;
};					// F->C, check: �ʿ��ϰ� �Ǹ� ��ü���� MSG ���� ���ϱ�!


					//typedef struct
					//{
					//	INT				ObjectIndex;			// Object�� Index
					//} MSG_FC_EVENT_REQUEST_OBJECT_EVENT;		// F->C, Object�� ���� Event�� ��û

struct MSG_FC_EVENT_REQUEST_OBJECT_EVENT
{
	DWORD			ObjectType;
	AVECTOR3		ObjectPosition;
};		// F->C, Object�� ���� Event�� ��û

struct MSG_FC_EVENT_CHANGE_WEATHER
{
	BitFlag16_t		MapWeather;
};

#define WEATHER_DEFAULT		(BitFlag16_t)0x0000	// �⺻
#define WEATHER_SUNNY		(BitFlag16_t)0x0001	// ����
#define WEATHER_RAINY		(BitFlag16_t)0x0002	// ��
#define WEATHER_SNOWY		(BitFlag16_t)0x0004	// ��
#define WEATHER_CLOUDY		(BitFlag16_t)0x0008	// �帲
#define WEATHER_FOGGY		(BitFlag16_t)0x0010	// �Ȱ�

struct MEX_CHANNEL_INFO
{
	ChannelIndex_t	ChannelIndex;
	INT				Crowdedness;		// ȥ�⵵, 0% ~ 100%??, check: ��Ȯ�� ��� �����ؾ� ��
};

struct  MSG_FC_EVENT_SUGGEST_CHANNELS
{
	MapIndex_t		MapIndex;
	INT				WarpTargetIndex;	// Ŭ���̾�Ʈ�� �� ������ MSG_FC_EVENT_SELECT_CHANNEL�� �״�� �����ؼ� ������ �Ѱ��־�� ��
	INT				NumOfChannels;		// ���� ������ ä���� ��
	ARRAY_(MEX_CHANNEL_INFO);
};		// F->C, ���� ������ ä���� ������

struct MSG_FC_EVENT_SELECT_CHANNEL {
	ClientIndex_t		ClientIndex;
	INT					WarpTargetIndex;	// Ŭ���̾�Ʈ�� �� ������ MSG_FC_EVENT_SUGGEST_CHANNELS�κ��� �״�� �����ؼ� ������ �Ѱ��־�� ��
	MAP_CHANNEL_INDEX	MapChannelIndex;
};				// C->F, ������ ä���� ������

struct MSG_FC_EVENT_SELECT_CHANNEL_WITH_PARTY
{
	ClientIndex_t		ClientIndex;
	INT					WarpTargetIndex;	// Ŭ���̾�Ʈ�� �� ������ MSG_FC_EVENT_SUGGEST_CHANNELS�κ��� �״�� �����ؼ� ������ �Ѱ��־�� ��
	MAP_CHANNEL_INDEX	MapChannelIndex;
	int					nPartyMembers;		// ���� ������ ��Ƽ���� ��, ����� ����
	ARRAY_(UID32_t);						// ��Ƽ�� character uniquenumber�� array
};	// C->F, ������ ä���� ������

struct MSG_FC_EVENT_REQUEST_RACING_WARP
{
	ClientIndex_t		ClientIndex;
	MAP_CHANNEL_INDEX	MapChannelIndex;
};	// C->F, ���̸̽������� ���� ��û


struct MSG_FC_EVENT_GET_SHOP_WARP_TARGET_MAP_LIST
{
	MAP_CHANNEL_INDEX	MapChannelIndex;
};

struct WARP_TARGET_MAP_INFO_4_EXCHANGE
{
	MapIndex_t	MapIndex;
	INT			TargetIndex;	// EVENTINFO�� �����ϴ� TargetIndex
	char		TargetName[SIZE_MAX_WARP_TARGET_NAME];	// Ÿ���� �̸�(ex: ���û��, �����ϴ�)
	INT			Fee;			// ���� ���
};

struct MSG_FC_EVENT_GET_SHOP_WARP_TARGET_MAP_LIST_OK
{
	INT		NumOfTargetMaps;
	ARRAY_(WARP_TARGET_MAP_INFO_4_EXCHANGE);
};

struct MSG_FC_EVENT_REQUEST_SHOP_WARP
{
	MapIndex_t	MapIndex;
	INT			TargetIndex;			// EVENTINFO�� �����ϴ� TargetIndex	
};

struct MSG_FC_EVENT_CHARACTERMODE_ENTER_BUILDING
{
	INT			nBuildingIndex0;
};

struct MSG_FC_EVENT_CALL_WARP_EVENT_REQUEST			// 2006-07-21 by cmkwon
{
	DWORD				dwCallWarpEventID0;
	DWORD				dwSupportTermTick0;
	char				szUserMessage[SIZE_STRING_128];
};

struct MSG_FC_EVENT_CALL_WARP_EVENT_REQUEST_ACK		// 2006-07-21 by cmkwon
{
	DWORD				dwCallWarpEventID0;
};


///////////////////////////////////////////////////////////////////////////////
// FN_EVENT (Field server <-> NPC server)
using MSG_FN_EVENT_OTHER_WARPED = MSG_FC_EVENT_OTHER_WARPED;

///////////////////////////////////////////////////////////////////////////////
// FP_EVENT (Field server <-> Pre server)
struct MSG_FP_EVENT_NOTIFY_WARP
{
	char			AccountName[SIZE_MAX_ACCOUNT_NAME];
	SERVER_ID		FieldServerID;
	UID32_t			CharacterUniqueNumber;
};		// F --> P

struct MSG_FP_EVENT_NOTIFY_WARP_OK
{
	UID32_t			CharacterUniqueNumber;
};		// P --> F

struct MSG_FP_EVENT_ENTER_SELECT_SCREEN
{
	char			AccountName[SIZE_MAX_ACCOUNT_NAME];
};		// F->P, Client�� ĳ���� ���� â�� ����

struct MSG_FP_EVENT_GAME_STARTED
{
	char				AccountName[SIZE_MAX_ACCOUNT_NAME];
	char				CharacterName[SIZE_MAX_CHARACTER_NAME];
	UID32_t				CharacterUniqueNumber;
	MAP_CHANNEL_INDEX	MapChannelIndex;
};			// F->P, Client�� ������ ������(������ ����)

struct MSG_FP_EVENT_MAP_CHANGED
{
	char				AccountName[SIZE_MAX_ACCOUNT_NAME];
	char				CharacterName[SIZE_MAX_CHARACTER_NAME];
	UID32_t				CharacterUniqueNumber;
	MAP_CHANNEL_INDEX	MapChannelIndex;
};				// F->P, Client�� ���� �̵���

				// T0_FP_MONITOR
//struct MSG_FP_MONITOR_SET_MGAME_EVENT_TYPE
//{
//	MGAME_EVENT_t		enMGameEvent;
//};



///////////////////////////////////////////////////////////////////////////////
// FC_STORE
struct MSG_FC_STORE_GET_ITEM
{
	UID32_t	AccountUniqueNumber;
	UID32_t	PossessCharacter;		// CharacterUniqueNumber 
	BYTE	ItemStorage0;			// 0(ITEM_IN_CHARACTER):ĳ�����κ�, 1(ITEM_IN_STORE):â��
	bool	bSendToClient;			// Client�� �������� ����, WARP_CONNECT�ô� ���� ���ʿ�
	bool	bMainGameStartFlag;		// 2012-10-08 by khkim, GLog - ���ӽ��۽� ������ Ȯ���� ����
};

struct MSG_FC_STORE_PUT_ITEM_HEADER
{
	UID32_t	PossessCharacter;		// CharacterUniqueNumber
	BYTE	ItemStorage0;			// 2006-01-05 by cmkwon, 0(ITEM_IN_CHARACTER):ĳ�����κ�, 1(ITEM_IN_STORE):â��
};

struct MSG_FC_STORE_PUT_ITEM
{
	int		BytesToRead;			// �ڿ� �ٴ� �������� �� ����Ʈ��, ���ǰ� �ʿ���
	ARRAY_(ITEM_XXX);
};

struct MSG_FC_STORE_PUT_ITEM_DONE
{
	UINT	NumOfItem;
	BYTE	ItemStorage0;			// 2006-01-05 by cmkwon, 0(ITEM_IN_CHARACTER):ĳ�����κ�, 1(ITEM_IN_STORE):â��
};

struct MSG_FC_CHARACTER_GET_USER_ITEM_INFO_OK_DONE	// 2012-06-05 by jhseol, ���۹̸����� - Ŭ���û ��Ŷ �߰�
{
	struct ItemEnahcntInfo
	{
		// 2013-01-24 by jhseol, ���۹̸����� ��Ŷ ������ �ʰ��� ���� ���� ����
		UID64_t		TargetItemUniqueNumber;
		INT			TargetItemNum;				// ��� �������� ItemNum
		INT			EnchantItemNum;				// enchant�� ���� �������� ItemNum
												// end 2013-01-24 by jhseol, ���۹̸����� ��Ŷ ������ �ʰ��� ���� ���� ����
		int			EnchantCount;
	};
	ItemEnahcntInfo ItemEnchant[6][8];
};

struct MSG_FC_STORE_MOVE_ITEM
{
	UID64_t		ItemUniqueNumber;		// STORE Talbe�� �ε���, DB paramemter�� (STORE_ITEM*)��
	BYTE		FromItemStorage;		// 0(ITEM_IN_CHARACTER):ĳ�����κ�, 1(ITEM_IN_STORE):â��
	BYTE		ToItemStorage;			// 0(ITEM_IN_CHARACTER):â��->ĳ�����κ�, 1(ITEM_IN_STORE):ĳ�����κ�->â��
	INT			Count;					// ���� �������� ����
};

struct MSG_FC_STORE_MOVE_ITEM_OK
{
	UID64_t		ItemUniqueNumber;
	UID32_t		FromPossessCharacter;	// CharacterUniqueNumber or 0
	UID32_t		ToPossessCharacter;		// CharacterUniqueNumber or 0
	INT			Count;					// ���� �������� ����
};

struct MSG_FC_STORE_LOG_GUILD_ITEM
{// 2006-09-27 by dhjin, C --> F ���� �α� ���� ��û
	UID32_t		AccountUID;
	UID32_t		CharacterUID;
};

struct MSG_FC_STORE_LOG_GUILD_ITEM_OK
{// 2006-09-27 by dhjin, F --> C ���� �α� ���� ����
	T1<T0::FL_LOG>	LogType;
	ATUM_DATE_TIME	Time;
	UID32_t			CharacterUID;
	CHAR			CharacterName[SIZE_MAX_CHARACTER_NAME];
	INT				ItemNum;
	INT				PrefixCodeNum;
	INT				SuffixCodeNum;
	INT				ItemChangeCount;
};

// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
struct MSG_FI_GUILD_BONUS_EXP_RATE
{
	INT				nBonusExpRate;
	UID32_t			CharacterUID; // 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
};
// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������


// 2008-01-23 by cmkwon, S_F, S_L: ����/�������� ���� �α׿� �߰� - IDT_XXX, ITEM_DELETE_TYPE_XXX
#define IDT_GENERAL						((BYTE)0)	// �Ϲ� �뵵
#define IDT_EXPIRE_TIME					((BYTE)1)	// ��ȿ �ð�/�Ⱓ ���
#define IDT_INVALID_ITEMNUM				((BYTE)2)	// ��ȿ���� ���� ItemNum		// 2009-11-20 by cmkwon, ���� ������ �ε��� ��ȿ���� ���� ������ ó�� ���� - 
#define IDT_COUNTABLEITEM_ZERO			((BYTE)3)	// ī���ͺ� ������ Count 0		// 2009-11-20 by cmkwon, ���� ������ �ε��� ��ȿ���� ���� ������ ó�� ���� - 


// IUT_XXX, ITEM_UPDATE_TYPE_XXX
#define IUT_GENERAL						((BYTE)0)	// �Ϲ� �뵵
#define IUT_DROP_ITEM					((BYTE)1)	// ��� ������
#define IUT_MIXING						((BYTE)2)	// ������ ����
#define IUT_TRADE						((BYTE)3)	// �ŷ�
#define IUT_SHOP						((BYTE)4)	// ���� ���� ������, �Ǹ� �ݾ� ��
#define IUT_QUEST						((BYTE)5)	// ����Ʈ�� ���� ����
#define IUT_ADMIN						((BYTE)6)	// ����, ���߿�
#define IUT_BULLET						((BYTE)7)	// �Ѿ� ������Ʈ
#define IUT_SKILL						((BYTE)8)	// ��ų ������
#define IUT_LOADING						((BYTE)9)	// ���� ���� �� �ε�
#define IUT_AUCTION						((BYTE)10)	// ��� ����
#define IUT_ENCHANT						((BYTE)11)	// ��æƮ
#define IUT_USE_ITEM					((BYTE)12)	// ������ ���
#define IUT_MGAME_EVENT					((BYTE)13)	// MGameEvent
#define IUT_USE_ENERGY					((BYTE)14)	// �������� ������ ���
#define IUT_EXPIRE_CARD_ITEM			((BYTE)15)	// �ð����� ������ ��� �ð� ����
#define IUT_PENALTY_ON_DEAD				((BYTE)16)	// ĳ���� ���� ���� �г�Ƽ
#define IUT_PENALTY_AGEAR_FUEL_ALLIN	((BYTE)17)	// ĳ���� ���� ���� �г�Ƽ
#define IUT_INFLUENCEWAR_KILLER_BONUS	((BYTE)18)	// ���������� ų�� ���ʽ�
#define IUT_BONUS_ITEM					((BYTE)19)	// 2006-04-26 by cmkwon, ���ʽ� ������
#define IUT_BAZAAR_SELL					((BYTE)20)	// ���� �Ǹ� ���� �ŷ�
#define IUT_BAZAAR_BUY					((BYTE)21)	// ���� ���� ���� �ŷ�
#define IUT_RANDOMBOX					((BYTE)22)	// 2006-08-10 by cmkwon, �����ڽ� ���
#define IUT_GIVEEVENTITEM				((BYTE)23)	// 2006-08-25 by dhjin, �̺�Ʈ ������ ���
#define IUT_GUILD_STORE					((BYTE)24)	// 2006-09-25 by cmkwon, ���� â��
#define IUT_EXPIRE_ITEM					((BYTE)25)	// 2006-09-29 by cmkwon, ������ ����
#define IUT_STORE						((BYTE)26)	// 2006-10-26 by cmkwon, â�� �̿�
#define IUT_STORE_FEE					((BYTE)27)	// 2006-10-26 by cmkwon, â�� �̿��
#define IUT_ARENA_ITEM					((BYTE)28)	// 2007-06-04 by dhjin, �Ʒ��� ���� ������
#define IUT_TUTORIAL_PAY_ITEM			((BYTE)29)	// 2007-07-24 by dhjin, Tutorial ���� ������
#define IUT_EXPEDIENCYFUND_PAYBACK		((BYTE)30)	// 2007-08-22 by dhjin, �ǰ��� ȯ��
#define IUT_GIVEEVENTITEM_COUPONEVENT	((BYTE)31)	// 2008-01-10 by cmkwon, ������ �̺�Ʈ �ý��ۿ� �� ���� �ý��� �߰� - 
#define IUT_LUCKY_ITEM					((BYTE)32)	// 2008-11-10 by dhjin, ��Ű�ӽ�
#define IUT_WAR_CONTRIBUTION			((BYTE)33)	// 2008-12-23 by dhjin, ���� ���� �߰���
#define IUT_WAR_CONTRIBUTION_LEADER		((BYTE)34)	// 2008-12-23 by dhjin, ���� ���� �߰��� - ������
#define IUT_WAR_CONTRIBUTION_GUILD		((BYTE)35)	// 2008-12-23 by dhjin, ���� ���� �߰��� - ���
#define IUT_DISSOLUTION_ITEM			((BYTE)36)	// 2010-08-31 by shcho&jskim �����ۿ��� �ý��� - ���� �ý��� ��Ŷ ó��
#define IUT_DEPENDENCY_ITEM				((BYTE)37)	// 2011-09-20 by hskim, ��Ʈ�� �ý��� 2�� - ������ ������ ó�� (�� ������ ���� ������ ���� ����)
#define IUT_WARP						((BYTE)38)	// 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý��� - ���� ���� ���ǿ� ���� ������ ����

// start 2012-11-21 by khkim, GLog 2��
#define GLOG_WARPOINTSHOP				((BYTE)39)	// ���� ���� 
#define GLOG_INFINITYOBSHOP				((BYTE)40)	// ����	����
#define GLOG_CASHSHOP					((BYTE)41)	// ĳ�� ����
#define GLOG_DEFAULT_ZERO				((BYTE)0) 	// ����Ʈ 0�� ����
// end 2012-11-21 by khkim, GLog 2��

#define IUT_CITYWAR_BRING_SUMOFTEX		((BYTE)100)	// ���������� ����

struct MSG_FC_STORE_INSERT_ITEM
{
	UID32_t			FromCharacterUniqueNumber;		// From Possess, Ȥ�� FromCharacterUniqueNumber
	BYTE			ItemInsertionType;				// ������ ���� Ÿ��, IUT_XXX
	ITEM_GENERAL	ItemGeneral;
};

struct MSG_FC_STORE_DELETE_ITEM
{
	UID64_t			ItemUniqueNumber;	// ���� ������
	BYTE			ItemDeletionType;	// ������ ���� Ÿ��, IUT_XXX
};

struct MSG_FC_STORE_UPDATE_ITEM_COUNT
{
	UID64_t			ItemUniqueNumber;	// Update�� ������
	INT				NewCount;			// Count�� �� ��
	BYTE			ItemUpdateType;		// ������ ���� Ÿ��, IUT_XXX
};

struct MSG_FC_STORE_UPDATE_ENDURANCE
{
	UID64_t			ItemUniqueNumber;
	float			Endurance;
};

struct MSG_FC_STORE_UPDATE_POSSESS
{
	UID64_t			ItemUniqueNumber;
	UID32_t			FromPossess;
	UID32_t			ToPossess;
};

struct MSG_FC_STORE_UPDATE_RARE_FIX
{
	UID64_t	ItemUID;
	INT		PrefixCodeNum;	// ���λ�, ������ 0
	INT		SuffixCodeNum;	// ���̻�, ������ 0
};

struct MSG_FC_STORE_INSERT_USINGITEM
{
	INT				ItemNum;
	UID64_t			ItemUID;						// 2006-04-24 by cmkwon
	BYTE			ItemInsertionType;				// ������ ���� Ÿ��, IUT_XXX
	INT				nRemainSecond;
};

struct MSG_FC_STORE_DELETE_USINGITEM
{
	INT				ItemNum;
	BYTE			ItemDeletionType;	// ������ ���� Ÿ��, IUT_XXX
};

struct MSG_FC_STORE_UPDATE_USINGITEM
{
	UID64_t			ItemUID0;
	INT				UsingTimeStamp0;	// ���ݱ��� ���� �ð�(����:��)
};

struct MSG_FC_STORE_EXPIRE_USINGITEM		// 2006-10-11 by cmkwon
{
	UID64_t			ItemUID0;
};

struct MSG_FC_STORE_UPDATE_ITEMSTORAGE
{
	//	UID32_t			PossessCharacter;		// 2006-09-18 by dhjin, ���õ� �ɸ���
	UID64_t			ItemUniqueNumber;
	UID32_t			FromItemStorage;
	UID32_t			ToItemStorage;
};

struct MSG_FC_STORE_UPDATE_ITEMNUM		// 2006-06-14 by cmkwon
{
	UID64_t			ItemUniqueNumber;
	INT				ItemNum;
	BYTE			ItemUpdateType;		// ������ ���� Ÿ��, IUT_XXX
};

///////////////////////////////////////////////////////////////////////////////
// 2006-09-04 by dhjin, �� ���� ���� 
#define QUICKTABCOUNT		3
#define QUICKSLOTCOUNT		10			// 2008-06-19 by dhjin, EP3 - 8 -> 10���� ����  

struct SQUICKSLOT_INFO
{
	bool		Delete;
	int			arrItemNum_[QUICKSLOTCOUNT];
};

struct MSG_FC_STORE_REQUEST_QUICKSLOT_OK
{
	SQUICKSLOT_INFO		POS[QUICKTABCOUNT];
};

struct MSG_FC_STORE_SAVE_QUICKSLOT
{
	SQUICKSLOT_INFO		POS[QUICKTABCOUNT];
	int					SlotPurpose;	// 2012-06-21 by jhseol, �Ʒ��� �߰�����part2 - �Ʒ��� ���� ������
};

///////////////////////////////////////////////////////////////////////////////
// FC_ITEM
struct MSG_FC_ITEM_SHOW_ITEM
{
	UINT			ItemFieldIndex;				// ���� ������ ������ �ӽ÷� �����ϴ� ��ȣ
	INT				ItemNum;					// Ŭ���̾�Ʈ�� �������� ������ �����ֱ� ���� ����
	UID32_t			FirstCharacterUID;			// ������ ���� ������ ù��° ĳ����
	INT				Amount;						// �������� ����
	AVECTOR3		Position;
	BYTE			DropItemType;				// ������ �������� ����(����� ������, ���ݿ�(���η�) ������ ���)
};

#define DROP_ITEM_TYPE_PICKUP		(BYTE)0x00		// ����� ������
#define DROP_ITEM_TYPE_ATTACK_MINE	(BYTE)0x01		// ���ݿ� ���η� ������

struct MSG_FC_ITEM_HIDE_ITEM
{
	UINT			ItemFieldIndex;				// ���� ������ ������ �ӽ÷� �����ϴ� ��ȣ
	ClientIndex_t	ClientIndex;				// �������� ���� character�� client index
};

struct MSG_FC_ITEM_GET_ITEM
{
	ClientIndex_t	ClientIndex;
	UINT			ItemFieldIndex;					// ���� ������ ������ �ӽ÷� �����ϴ� ��ȣ
};

struct MSG_FC_ITEM_GET_ITEM_OK
{
	ClientIndex_t	ClientIndex;
	UINT			ItemFieldIndex;				// ���� ������ ������ �ӽ÷� �����ϴ� ��ȣ
	INT				ItemNum;					// �������� ����
	INT				Amount;						// �������� ����
	INT				SizeOfItem;					// sizeof(�ش�Item)
	ARRAY_(ITEM_XXX);							// ITEM_XXX�� �´�
};

struct MSG_FC_ITEM_GET_ITEM_FAIL
{
	ClientIndex_t	ClientIndex;
	UINT			ItemFieldIndex;				// ���� ������ ������ �ӽ÷� �����ϴ� ��ȣ
};

struct MSG_FC_ITEM_PUT_ITEM
{
	UINT			ItemFieldIndex;				// ���� ������ ������ �ӽ÷� �����ϴ� ��ȣ
	STRUCT_(ITEM_XXX);							// ���Ŀ� �ش� ITEM_XXXX�� �´�
};

// 2005-11-15 by cmkwon, ������
//typedef struct
//{
//	ClientIndex_t	ClientIndex;
//	BYTE			KindOfSkill;				// check: �ǹ� �ľ� �� �̸� ���� �ʿ�
//} MSG_FC_ITEM_BONUSSKILLPOINT;
//
//typedef struct
//{
//	ClientIndex_t	ClientIndex;
//	BYTE			OldKindOfSkill;				// check: �ǹ� �ľ� �� �̸� ���� �ʿ�
//	BYTE			NewKindOfSkill;				// check: �ǹ� �ľ� �� �̸� ���� �ʿ�
//	BYTE			RemainedAmountOfSkill;		// check: ���� type Ȯ���ؾ� ��!
//} MSG_FC_ITEM_BONUSSKILLPOINT_OK;

struct MSG_FC_ITEM_CHANGE_WINDOW_POSITION
{
	UID32_t			CharacterUniqueNumber;
	UID64_t			FromItemUniqueNumber;	// ���� ������ �̵��� ������
	INT				FromItemWindowIndex;	// ���� ������ �̵��� �������� ���� ��ġ
	UID64_t			ToItemUniqueNumber;		// �̵� ��ġ(destination)�� ��� �ִ� ������(�����Ѵٸ�), ���ٸ� 0
	INT				ToItemWindowIndex;		// �̵� ��ġ(destination)�� ItemWindowIndex
};

struct MSG_FC_ITEM_CHANGE_WINDOW_POSITION_OK
{
	UID64_t			UniqueNumber;			// ���� ������ �̵��� ������
	INT				ItemWindowIndex;		// ���� ������ �̵��� �������� �� ��ġ
	BYTE			Wear;					// ���� ������ �̵��� �������� wear ����
	UID64_t			UniqueNumberDest;		// �̵� ��ġ(destination)�� ��� �ִ� ���� ������(�����Ѵٸ�), ���ٸ� 0
	INT				ItemWindowIndexDest;	// �̵� ��ġ(destination)�� ��� �ִ� ���� �������� �� ��ġ(�����Ѵٸ�), ���ٸ� POS_INVALID_POSITION
	BYTE			WearDest;				// �̵� ��ġ(destination)�� ��� �ִ� ���� ������ wear ����(�����Ѵٸ�), ���ٸ� 0
};

// start 2011-09-20 by hskim, ��Ʈ�� �ý��� 2��
struct MSG_FC_ITEM_NOTIFY_WINDOW_POSITION
{
	UID64_t			UniqueNumber;			// ������ �̵��� ������
	INT				ItemWindowIndex;		// ������ �̵��� �������� ��ġ
	BYTE			Wear;					// ������ �̵��� �������� wear ����

											// �������� �� �޽��� ���� CalcInventoryHiddenItemCounts() �Լ��� �� ȣ�����־���� (������ ������ ���� ����)

};		// F -> C, �������� ItemWindowIndex ����ɽÿ� Ŭ���̾�Ʈ���� �˷��ش�
		// end 2011-09-20 by hskim, ��Ʈ�� �ý��� 2��

struct MSG_FC_ITEM_UPDATE_WINDOW_ITEM_LIST
{
	INT				NumOfItems;
	ARRAY_(ITEM_WINDOW_INFO);
};

struct ITEM_WINDOW_INFO
{
	UID64_t			ItemUniqueNumber;
	BYTE			ItemKind;
	BYTE			Wear;
	INT				ItemWindowIndex;
};

struct MSG_FC_ITEM_THROW_AWAY_ITEM
{
	ClientIndex_t	ClientIndex;
	UID64_t			ItemUniqueNumber;
	INT				Amount;					// ������ ����
};

struct MSG_FC_ITEM_THROW_AWAY_ITEM_OK
{
	ClientIndex_t	ClientIndex;
	UID64_t			ItemUniqueNumber;
	INT				RemainedNumOfItem;		// ���� �� �����ִ� ����. 0 �̸� ������ ������.
};

struct MSG_FC_ITEM_USE_ENERGY
{
	ClientIndex_t	ClientIndex;
	UID64_t			ItemUniqueNumber;
	INT				nParam1;				//
};
struct MSG_FC_ITEM_USE_ENERGY_OK
{
	ClientIndex_t	ClientIndex;
	INT				ItemNum;
};

struct MSG_FC_ITEM_USE_ITEM
{
	ClientIndex_t	ClientIndex;
	UID64_t			ItemUniqueNumber;
	char			str256ChatMsg[SIZE_MAX_STRING_256];		// 2007-08-09 by cmkwon, ��� ���¿� ä�� �����ϱ� - 
};
struct MSG_FC_ITEM_USE_ITEM_OK
{
	ClientIndex_t	ClientIndex;
	INT				ItemNum;
};

struct MSG_FC_ITEM_DELETE_ITEM_ADMIN
{
	UINT			ItemFieldIndex;	// ���� ������ ������ �ӽ÷� �����ϴ� ��ȣ
	AVECTOR3		DropPosition;	// �������� ��ġ
};

struct MSG_FC_ITEM_DELETE_DROP_ITEM
{
	UINT			ItemFieldIndex;	// ���� ������ ������ �ӽ÷� �����ϴ� ��ȣ
	AVECTOR3		DropPosition;	// ������(����)�� ��ġ
};		// F->C, �ڽ��� �Ѹ� ������(���ε�)�� ���� �� ����

struct MSG_FC_ITEM_UPDATE_ITEM_POS {
	ClientIndex_t	ClientIndex;
	BYTE			ItemPosition;	// POS_XXX
	INT				ItemNum;
	// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - ����
	//	INT				ColorCode;		// 2005-12-08 by cmkwon, �Ƹ� ���� Ʃ�� ����
	INT				nShapeItemNum;		// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
	INT				nEffectItemNum;		// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
	INT				nPetLevel;		// 2012-12-05 by hskim, ��Ʈ�� �Ⱓ�� ���� ����
};		// F->C, ������ ����(��)�� ������, ������ ������ �ٲٸ� ������ ������

struct MSG_FC_ITEM_USE_ENCHANT
{
	UID64_t			EnchantItemUniqueNumber;	// ��æƮ ������
	UID64_t			TargetItemUniqueNumber;		// ��æƮ�� �ٸ� ��� ������
	UID64_t			AttachItemUniqueNumber;		// �߰� ������
	UID64_t			IncreaseProbabilityItemUID;	// 2009-01-19 by dhjin, ��þƮ Ȯ�� ����, 10��þ �Ĺ� ī�� - ��þƮ Ȯ�� ���� ī��
	UID64_t			EnchantItemUniqueNumber2;	// ��æƮ ������2 // 2010-04-20 by cmkwon, �ű� ��Ű �ӽ� ���� - SuffixCard ItemUID 
};

struct MSG_FC_ITEM_USE_ENCHANT_OK
{
	// 2010-04-20 by cmkwon, �ű� ��Ű �ӽ� ���� - (�����κ��߰�) - 
	//INT				ItemNum;					// ��æƮ ������
	INT				EnchantItemNum;			// 2010-04-20 by cmkwon, �ű� ��Ű �ӽ� ���� - (�����κ��߰�) - 
	INT				SuffixRareItemNum;		// 2010-04-20 by cmkwon, �ű� ��Ű �ӽ� ���� - (�����κ��߰�) - 
	bool			bSuccessFlag;			// 2010-04-20 by cmkwon, �ű� ��Ű �ӽ� ���� - (�����κ��߰�) - 
};					// ��æƮ �ϷḦ ����

struct MSG_FC_ITEM_PUT_ENCHANT
{
	ENCHANT			Enchant;
};

struct MSG_FC_ITEM_DELETE_ALL_ENCHANT
{
	UID64_t			ItemUniqueNumber;			// ��æƮ�� ������ ������	
};

struct MSG_FC_ITEM_MIX_ITEMS
{
	INT				NumOfItems;
	INT				nMixCounts;		// 2008-03-17 by cmkwon, ���� �ý��� ��� �߰� - 
	ARRAY_(ITEM_UNIQUE_NUMBER_W_COUNT);
};		// C->F, ������ �������� ����Ʈ�� ����

struct MSG_FC_ITEM_MIX_ITEMS_RESULT
{
	Err_t			Err;		// ERR_XXX, ERROR_NO_ERROR�̸� ����
};	// F->C, ������ ���� ����� ����

struct MSG_FC_ITEM_USE_CARDITEM_GUILDSUMMON
{
	UID64_t			ItemUniqueNumber;
	char			szCharacterName10[SIZE_MAX_CHARACTER_NAME];
};
struct MSG_FC_ITEM_USE_CARDITEM_GUILDSUMMON_NOTIFY
{
	ClientIndex_t	guildSummonClientIdx;
};

struct MSG_FC_ITEM_USE_CARDITEM_RESTORE
{
	UID64_t			ItemUniqueNumber;
	char			szCharacterName10[SIZE_MAX_CHARACTER_NAME];
};
struct MSG_FC_ITEM_USE_CARDITEM_RESTORE_NOTIFY
{
	ClientIndex_t	restoreClientIdx;
};

struct MSG_FC_ITEM_USE_CARDITEM_GUILD
{
	UID64_t			ItemUniqueNumber;
};
struct MSG_FC_ITEM_USE_CARDITEM_GUILD_NOTIFY
{
	ClientIndex_t	notifyClientIdx;
	UID32_t			guildUID;
};
struct MSG_FC_ITEM_USE_CARDITEM_MONSTERSUMMON
{
	UID64_t			ItemUniqueNumber;
};
struct MSG_FC_ITEM_USE_CARDITEM_MONSTERSUMMON_NOTIFY
{
	ClientIndex_t	notifyClientIdx;
	INT				notifyItemNum;
	INT				summonMonsterUnitKind;
};
struct MSG_FC_ITEM_USE_CARDITEM_CHANGECHARACTERNAME
{
	UID64_t			ItemUniqueNumber;
	char			szChangeCharacterName[SIZE_MAX_CHARACTER_NAME];
};
struct MSG_FC_ITEM_USE_CARDITEM_CHANGE_NATION_REQUEST	// 2015-12-17 Future, Nation Change Card
{
	UID64_t			ItemUniqueNumber;
};
struct MSG_FC_ITEM_USE_CARDITEM_CHANGECHARACTERNAME_NOTIFY
{
	ClientIndex_t	notifyClientIdx;
	char			szChangedCharacterName[SIZE_MAX_CHARACTER_NAME];
	char			szOriginCharacterName[SIZE_MAX_CHARACTER_NAME];
};
struct MSG_FC_ITEM_USE_CARDITEM_SKILLINITIALIZE
{
	UID64_t			ItemUniqueNumber;
	INT				skillItemNum;
};
struct MSG_FC_ITEM_USE_CARDITEM_SKILLINITIALIZE_NOTIFY
{
	ClientIndex_t	notifyClientIdx;
	INT				notifyItemNum;
};
struct MSG_FC_ITEM_USE_CARDITEM_CHANGEPILOTFACE
{
	UID64_t			ItemUniqueNumber;
	BYTE			pilotFace;
};
struct MSG_FC_ITEM_USE_CARDITEM_CHANGEPILOTFACE_NOTIFY
{
	ClientIndex_t	notifyClientIdx;
	INT				notifyItemNum;
	BYTE			pilotFace;
};

struct MSG_FC_ITEM_USE_INFLUENCE_BUFF				// 2006-04-21 by cmkwon
{
	UID64_t			ItemUniqueNumber;
};
struct MSG_FC_ITEM_USE_INFLUENCE_BUFF_OK			// 2006-04-21 by cmkwon
{
	// 2009-01-05 by dhjin, �̼Ǹ����� - ��� ���� ������ �߰� - �ذ� ���� ����
	//	UID64_t			ItemUniqueNumber;
	char			ItemUseCharacterName[SIZE_MAX_CHARACTER_NAME];
	INT				ItemNum;
};
struct MSG_FC_ITEM_USE_INFLUENCE_GAMEEVENT			// 2006-04-21 by cmkwon
{
	UID64_t			ItemUniqueNumber;
};
struct MSG_FC_ITEM_USE_INFLUENCE_GAMEEVENT_OK		// 2006-04-21 by cmkwon
{
	UID64_t			ItemUniqueNumber;
};

// 2006-08-10 by cmkwon
#define RANDOMBOX_RESULT_FAIL				0		// ����
#define RANDOMBOX_RESULT_ITEM				1		// ������
#define RANDOMBOX_RESULT_SPI				2		// Money(SPI)
#define RANDOMBOX_RESULT_EXP				3		// ����ġ
#define RANDOMBOX_RESULT_SKILL_SUPPORT_ITEM	4		// ��ų ���� ������, 2006-09-29 by cmkwon �߰���, ITEM_ATTR_SKILL_SUPPORT_ITEM
#define RANDOMBOX_RESULT_CREDITS			5		// 2015-08-03 Future, added credit Randombox
#define RANDOMBOX_RESULT_KIND_COUNT			6		// �����ڽ� ��� ���� ����

struct MSG_FC_ITEM_USE_RANDOMBOX		// 2006-08-10 by cmkwon
{
	UID64_t			ItemUID;
};
struct MSG_FC_ITEM_USE_RANDOMBOX_OK		// 2006-08-10 by cmkwon
{
	char			szCharacterName0[SIZE_MAX_CHARACTER_NAME];	// ����� CharacterName
	INT				nRandomBoxResult;							// RANDOMBOX_RESULT_XXX
	INT				nResultItemNum0;							// ������ ItemNum
	INT64			n64ResultCounts;							//
	INT				nPrefixCodeNum0;							// ���λ�, ������ 0
	INT				nSuffixCodeNum0;							// ���̻�, ������ 0
};

using MSG_FC_ITEM_USE_SKILL_SUPPORT_ITEM = MSG_FC_ITEM_USE_ITEM;		// 2006-09-29 by cmkwon
using MSG_FC_ITEM_USE_SKILL_SUPPORT_ITEM_OK = MSG_FC_ITEM_USE_ITEM_OK;	// 2006-09-29 by cmkwon

																		///////////////////////////////////////////////////////////////////////////////////////
																		// start 2011-09-20 by hskim, ��Ʈ�� �ý��� 2��

struct MSG_FC_ITEM_USE_PET_SOCKET_ITEM
{
	ClientIndex_t	ClientIndex;
	UID64_t			ItemUniqueNumber;
};

struct MSG_FC_ITEM_USE_PET_SOCKET_ITEM_OK
{
	ClientIndex_t	ClientIndex;
	UID64_t			ItemUniqueNumber;
};

struct MSG_FC_ITEM_CANCEL_PET_SOCKET_ITEM
{
	ClientIndex_t	ClientIndex;
	UID64_t			ItemUniqueNumber;
};

struct MSG_FC_ITEM_CANCEL_PET_SOCKET_ITEM_OK
{
	ClientIndex_t	ClientIndex;
	UID64_t			ItemUniqueNumber;
};

// end 2011-09-20 by hskim, ��Ʈ�� �ý��� 2��
///////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 2008-11-04 by dhjin, ��Ű�ӽ�
// 2009-03-03 by dhjin, ��Ű�ӽ� ������ - �ذ� ���� ����
// typedef MSG_FC_ITEM_USE_RANDOMBOX		MSG_FC_ITEM_USE_LUCKY_ITEM;
struct MSG_FC_ITEM_USE_LUCKY_ITEM
{
	UID64_t			ItemUID;
	INT				MachineNum;
};

struct LUCKY_ITEM_SIMPLEINFO
{
	SHORT			SlotNum;
	INT				MysteryItemDropNum;
	INT				Itemnum;
	INT64			n64ResultCounts;							//
	INT				nPrefixCodeNum0;							// ���λ�, ������ 0
	INT				nSuffixCodeNum0;							// ���̻�, ������ 0
};

struct MSG_FC_ITEM_USE_LUCKY_ITEM_OK
{
	LUCKY_ITEM_SIMPLEINFO	LuckyItemSimpleInfo[MaxPayLuckyMachineItem];
};


struct MSG_FC_ITEM_CHANGED_SHAPEITEMNUM			// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
{
	UINT64				nItemUID;
	INT					nShapeItemNum;

	FIXED_TERM_INFO		FixedTermShape;			// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)
};

struct MSG_FC_ITEM_CHANGED_EFFECTITEMNUM		// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
{
	UINT64			nItemUID;
	INT				nEffectItemNum;
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// FC_SHOP
struct MSG_FC_SHOP_PUT_ITEM_HEADER
{
	UINT		BuildingIndex;
};

struct MSG_FC_SHOP_PUT_ITEM
{
	USHORT		BytesToRead;			// �ڿ� �ٴ� �����۵��� �� ����Ʈ��
	ARRAY_(SHOP_ITEM);
};

#define UNLIMITED_REMAIN_COUNT_FOR_LIMITED_EDITION		-1		// 2010-01-26 by cmkwon, ĳ�� ������ �����Ǹ� �ý��� ���� - 
#define SOLD_OUT_REMAIN_COUNT_FOR_LIMITED_EDITION		0		// 2010-01-26 by cmkwon, ĳ�� ������ �����Ǹ� �ý��� ���� - 

struct SHOP_ITEM
{
	UINT		ItemNum;
	char		ItemName[SIZE_MAX_ITEM_NAME];
	USHORT		MinTradeQuantity;
	INT			Price;
	BYTE		ItemKind;
	BYTE		CashShopIndex;		// 2009-01-28 by cmkwon, ĳ���?����(��õ��,�Ż�ǰ �߰�) - SHOP_ITEM�� �߰�, ���� 4��Ʈ�� ���ε���, ���� 4��Ʈ�� ��Ʈ �÷��׷� ���
	INT			RemainCountForLimitedEdition;		// 2010-01-26 by cmkwon, ĳ�� ������ �����Ǹ� �ý��� ���� - 

													// 2010-01-26 by cmkwon, ĳ�� ������ �����Ǹ� �ý��� ���� - ��������
	bool IsSoldOutShopItem(void)
	{
		if (SOLD_OUT_REMAIN_COUNT_FOR_LIMITED_EDITION != RemainCountForLimitedEdition)
		{
			return FALSE;
		}
		return TRUE;
	}
	bool IsLimitedEditionShopItem(void)
	{
		if (SOLD_OUT_REMAIN_COUNT_FOR_LIMITED_EDITION > RemainCountForLimitedEdition)
		{
			return FALSE;
		}
		return TRUE;
	}

	// start 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���

	// ���� �߰��Ǵ°� ���θ� �� �� �ִ� 1ȸ�� ������ ���� �Ǹ� ���(DB ������ �ʿ����)�� �� (ũ����Ż �ý���)
	// ���� ���� ����� ���ÿ� ����ϴ� ���� �Ǹ� ����� �ʿ��ϴٸ� ����ȭ �� �߰� ����� ������

	bool IsAvailableShopItem(int Amount)
	{
		if (UNLIMITED_REMAIN_COUNT_FOR_LIMITED_EDITION == RemainCountForLimitedEdition)
		{
			return TRUE;
		}

		if (Amount <= RemainCountForLimitedEdition)
		{
			return TRUE;
		}

		return FALSE;
	}
	// end 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
};

struct MSG_FC_SHOP_PUT_ITEM_DONE
{
	UINT		NumOfItem;
};

struct MSG_FC_SHOP_GET_ITEMINFO
{
	INT			ItemNum;
};

struct MSG_FC_SHOP_GET_ITEMINFO_OK
{
	ITEM		ItemInfo;
};

struct MSG_FC_SHOP_BUY_ITEM
{
	UINT		BuildingIndex;
	UINT		ItemNum;
	INT			Amount;					// ������ �������� ����
};

struct MSG_FC_SHOP_BUY_ITEM_OK
{
	INT			RemainedMoney;			// ���� �� ���� ��
	INT			SizeOfItem;				// sizeof(�ش�Item)�� ����
	ARRAY_(ITEM_XXX);						// ITEM_XXX�� �´�
};

#ifdef S_WEB_CASHSHOP_SERVER_MODULE_HSKIM
// 2013-03-13 by hskim, �� ĳ�� ����
// TODO : remove referencing to this module
//#define T_FC_SHOP_AUTH_KEY		(MessageType_t)((T0_FC_SHOP<<8)|T1_FC_CHARACTER_XIGNCODE_S_ACK_PACKET)
#endif

struct MSG_FC_SHOP_AUTH_KEY
{
	char AuthenticationKey[SIZE_MAX_WEB_AUTHENTICATION_KEY];
	INT ServerID;
};
// end 2013-03-13 by hskim, �� ĳ�� ����

// 2011-12-26 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
struct MSG_FC_SHOP_BUY_ITEM_UPDATE
{
	UINT		BuildingIndex;
	UINT		ItemNum;
	INT			RemainCountForLimitedEdition;
};
// 2011-12-26 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���

struct MSG_FC_SHOP_SELL_ITEM
{
	UINT		BuildingIndex;
	UID64_t		ItemUniqueNumber;
	BYTE		ItemKind;
	INT			Amount;					// �Ǹ��� �������� ����
};

struct MSG_FC_SHOP_SELL_ITEM_OK
{
	UID64_t		ItemUniqueNumber;
	INT			RemainedNumOfItem;		// �Ǹ� �� ���� �ִ� ����(0�̸� ����, Countable Item �� ��� �ǹ� ����)
	INT			RemainedMoney;			// �Ǹ� �� ���� ��
};

struct MSG_FC_SHOP_GET_USED_ITEM_PRICE
{
	UID64_t		ItemUniqueNumber;
	INT			Amount;
};

struct MSG_FC_SHOP_GET_USED_ITEM_PRICE_OK
{
	UID64_t		ItemUniqueNumber;
	UINT		Price;
};

struct MSG_FC_SHOP_GET_SHOP_ITEM_LIST
{
	INT			BuildingIndex;
};	// C->F, �������� �Ĵ� �������� ����Ʈ�� ��û, ������ T_FC_SHOP_PUT_ITEM_XXX

struct MSG_FC_SHOP_REQUEST_REPAIR
{
	INT			DesParam;			// ������ �κ�(DES_HP, DES_DP, DES_EP, DES_SP, DES_BULLET_01, DES_BULLET_02)
	INT			Count;				// ��� �縸ŭ ������ ������
};		// C->F, ��ü ���� ��û

struct MSG_FC_SHOP_REQUEST_REPAIR_OK
{
	INT			DesParam;			// ������ �κ�(DES_HP, DES_DP, DES_EP, DES_SP, DES_BULLET_01, DES_BULLET_02)
	INT			Count;				// ������ ��
	INT			RepairCost;			// ���� ���
};

struct MSG_FC_SHOP_BUY_CASH_ITEM
{
	UINT		BuildingIndex;
	UINT		ItemNum;
	INT			Amount;					// ������ �������� ����
	UID32_t		GiveTargetCharacterUID;	// 2007-11-13 by cmkwon, �����ϱ� ��� �߰� - 0 �̸� �ڽ��� ����, 0�� �ƴϸ� �ش� ĳ���Ϳ��� �����ϱ�
};
struct MSG_FC_SHOP_BUY_CASH_ITEM_OK
{// 2007-11-13 by cmkwon, �����ϱ� ��� �߰� - MSG_FC_SHOP_BUY_CASH_ITEM_OK ����ü �߰�
	UINT		ItemNum;
	INT			Amount;					// ������ �������� ����
	UID32_t		GiveTargetCharacterUID;	// 2007-11-13 by cmkwon, �����ϱ� ��� �߰� - 0 �̸� �ڽ��� ����, 0�� �ƴϸ� �ش� ĳ���Ϳ��� �����ϱ�
	char		GiveTargetCharacterName[SIZE_MAX_CHARACTER_NAME];	// 2007-11-13 by cmkwon, �����ϱ� ��� �߰� - 
	INT			RemainCountForLimitedEdition;		// 2010-01-26 by cmkwon, ĳ�� ������ �����Ǹ� �ý��� ���� - 
};

struct MSG_FC_SHOP_BUY_COLOR_ITEM	// 2005-12-06 by cmkwon
{
	UINT		BuildingIndex;
	UINT		ItemNum;
};

struct MSG_FC_SHOP_BUY_COLOR_ITEM_OK	// 2005-12-06 by cmkwon
{
	UID64_t		ItemUID64;
	INT			ColorCode;
};

struct MSG_FC_SHOP_BUY_WARPOINT_ITEM
{// 2007-06-13 by dhjin, WarPoint �� 
	UINT		BuildingIndex;
	UINT		ItemNum;
	INT			Amount;					// ������ �������� ����
};

struct MSG_FC_SHOP_BUY_WARPOINT_ITEM_OK
{// 2007-06-13 by dhjin, WarPointItem �����ϰ� �� ����.
	INT				PayWarPoint;			// 2007-06-13 by dhjin, ������ WarPoint
	INT				WarPoint;				// 2007-06-13 by dhjin, �� WarPoint
};

struct MSG_FC_SHOP_CHECK_GIVE_TARGET
{// 2007-11-13 by cmkwon, �����ϱ� ��� �߰� - MSG_FC_SHOP_CHECK_GIVE_TARGET ����ü �߰�
	char			GiveTargetCharName[SIZE_MAX_CHARACTER_NAME];	// �����޴� ĳ���͸�
	INT				GiveItemNum;
};

struct MSG_FC_SHOP_CHECK_GIVE_TARGET_OK
{// 2007-11-13 by cmkwon, �����ϱ� ��� �߰� - MSG_FC_SHOP_CHECK_GIVE_TARGET_OK ����ü �߰�
	char			GiveTargetCharName[SIZE_MAX_CHARACTER_NAME];	// �����޴� ĳ���͸�
	UID32_t			GiveTargetCharUID;
	char			GiveTargetGuildName[SIZE_MAX_GUILD_NAME];
	USHORT			GiveTargetUnitKind;
	BYTE			GiveTargetLevel;
};

// start 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
struct MSG_FC_SHOP_CLOSE
{
	UINT BuildingIndex;
};
// end 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���

// 2013-05-09 by hskim, ���� ����Ʈ ����
struct MSG_FC_SHOP_INFLUENCE_BUFF
{
	INT	nConsecutiveVictories;		// ���� ����Ʈ
};

struct MSG_FC_SHOP_INFLUENCE_BUFF_OK
{
	INT	nLastSkillNum;
};
// end 2013-05-09 by hskim, ���� ����Ʈ ����

struct SGIVE_TARGET_CHARACTER
{// 2007-11-13 by cmkwon, �����ϱ� ��� �߰� - SGIVE_TARGET_CHARACTER ����ü �߰�
	char			AccountName0[SIZE_MAX_ACCOUNT_NAME];
	UID32_t			AccountUID0;
	INT				ConnectingServerGroupID;
	INT				MembershipItemNum;
	ATUM_DATE_TIME	MembershipExpireTime;
	char			CharacterName0[SIZE_MAX_CHARACTER_NAME];
	UID32_t			CharacterUID0;
	BYTE			InfluenceType0;
	char			GuildName0[SIZE_MAX_GUILD_NAME];
	USHORT			UnitKind0;
	BYTE			Level0;
	INT				BuyItemNum;								// ���� �� ItemNum
};


///////////////////////////////////////////////////////////////////////////////
// FC_TRADE
struct MSG_FC_TRADE_REQUEST_TRADE
{
	char			TradeTarget[SIZE_MAX_CHARACTER_NAME];	// �ǿ�û��
};

struct MSG_FC_TRADE_REQUEST_TRADE_OK
{
	char			TradeSource[SIZE_MAX_CHARACTER_NAME];	// ��û��
	UID32_t			TradeSourceCharacterUniqueNumber;		// ��û���� character unique number
};

struct MSG_FC_TRADE_CANCEL_REQUEST
{
	char			TradeTarget[SIZE_MAX_CHARACTER_NAME];	// �ǿ�û��
};

struct MSG_FC_TRADE_CANCEL_REQUEST_OK
{
	char			TradeSource[SIZE_MAX_CHARACTER_NAME];	// ��û��
	UID32_t			TradeSourceCharacterUniqueNumber;		// ��û���� character unique number
};

struct MSG_FC_TRADE_ACCEPT_TRADE
{
	UID32_t			TradeSourceCharacterUniqueNumber;		// ��û���� character unique number
};

struct MSG_FC_TRADE_ACCEPT_TRADE_OK
{
	char			TradeTarget[SIZE_MAX_CHARACTER_NAME];	// �ǿ�û��
	UID32_t			TradeTargetCharacterUniqueNumber;		// �ǿ�û���� character unique number
};

struct MSG_FC_TRADE_REJECT_TRADE
{
	UID32_t			TradeSourceCharacterUniqueNumber;		// ��û���� character unique number
};

struct MSG_FC_TRADE_REJECT_TRADE_OK
{
	char			TradeTarget[SIZE_MAX_CHARACTER_NAME];	// �ǿ�û��
	UID32_t			TradeTargetCharacterUniqueNumber;		// �ǿ�û���� character unique number
};

struct MSG_FC_TRADE_REJECT_TRADING
{
	char			TradeTarget[SIZE_MAX_CHARACTER_NAME];	// �ǿ�û��
};								// �ŷ����̹Ƿ� �ŷ��� �� ���� ����

struct MSG_FC_TRADE_SHOW_TRADE_WINDOW
{
	UID32_t			PeerTradeCharacterUniqueNumber;			// �ŷ� ����� character unique number
};

struct MSG_FC_TRADE_TRANS_ITEM
{
	UID32_t			CharacterUniqueNumber;					// ������ �ø� ���
	UID64_t			ItemUniqueNumber;						// �ø� ������
	INT				ItemNum;								// ������ ����
	INT				Amount;									// �ø� ����(���������� �ش�, �������� 1)
};

struct MSG_FC_TRADE_TRANS_ITEM_OK
{
	UID32_t			CharacterUniqueNumber;					// ������ �ø� ���
	ITEM_GENERAL	TradeItem;								// �� �������� count�� ���� ����
	INT				NumOfEnchants;							// �������� �� ��æƮ ����
	ARRAY_(INT);											// EnchantItemNum�� Array
};

struct MSG_FC_TRADE_SEE_ITEM
{
	UID32_t			CharacterUniqueNumber;					// ������ �ø� ���
	UID64_t			ItemUniqueNumber;						// �ø� ������
	INT				ItemNum;								// ������ ����
	INT				Amount;									// �ø� ����(countable item�� ���� ����, �������� 1)
};

struct MSG_FC_TRADE_SEE_ITEM_OK
{
	UID32_t			CharacterUniqueNumber;					// ������ �ø� ���
	STRUCT_(ITEM_XXX);										// �� �������� count�� ���� ����
};

struct MSG_FC_TRADE_OK_TRADE
{
	UID32_t			CharacterUniqueNumber;					// �ڽ�
};

struct MSG_FC_TRADE_OK_TRADE_OK
{
	UID32_t			CharacterUniqueNumber;					// ����
};

struct MSG_FC_TRADE_CANCEL_TRADE
{
	UID32_t			CharacterUniqueNumber;					// ����
};

struct MSG_FC_TRADE_CANCEL_TRADE_OK
{
	UID32_t			CharacterUniqueNumber;					// ����
};

using MSG_FC_TRADE_INSERT_ITEM = MSG_FC_STORE_INSERT_ITEM;
using MSG_FC_ENEMY_ITEM_INFO = MSG_FC_STORE_INSERT_ITEM;		// 2012-06-05 by jhseol, ���۹̸����� - Ŭ���û ��Ŷ �߰�
using MSG_FC_TRADE_DELETE_ITEM = MSG_FC_STORE_DELETE_ITEM;
using MSG_FC_TRADE_UPDATE_ITEM_COUNT = MSG_FC_STORE_UPDATE_ITEM_COUNT;

struct MSG_FC_TRADE_OK_TRADE_NOTIFY		// 2008-11-21 by cmkwon, �ŷ� ���� Ȯ�� �ý��� ���� - 
{
	UID32_t			CharacterUniqueNumber;					// �ŷ� ���� ��ư�� ������ ĳ����
};

enum COUNTDOWN_TYPE
{
	COUNTDOWN_TYPE_P2P_WAR = 0,
	COUNTDOWN_TYPE_PARTY_WAR = 1,
	COUNTDOWN_TYPE_GUILD_WAR = 2,
	COUNTDOWN_TYPE_RACING = 3,
	COUNTDOWN_TYPE_INVALID = 4
};
// T0_FC_COUNTDOWN
struct MSG_FC_COUNTDOWN_START
{
	int		nWaitingSecond;
	BYTE	byCountdownType;
};

struct MSG_FC_COUNTDOWN_DONE
{
	BYTE	byCountdownType;
};


// T0_FC_OBJECT
struct MSG_FC_OBJECT_CHANGE_BODYCONDITION
{
	INT			nObjectCodeNum;
	AVECTOR3	AVec3Position;
	BodyCond_t	bodyCondition;
};

using MSG_FC_OBJECT_CHANGE_BODYCONDITION_OK = MSG_FC_OBJECT_CHANGE_BODYCONDITION;

///////////////////////////////////////////////////////////////////////////////////////
// start 2011-08-17 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ� ���� ����

struct MSG_TRADE_PET_DATA
{
	UID64_t			CreatedPetUID;

	char			PetName[SIZE_MAX_PET_NAME];

	INT				PetIndex;
	INT				PetLevel;

	Experience_t	PetExp;

	// Ŭ�󿡼� �ʿ��� ���� �޾Ƽ� ó�� by hskim

};
// end 2011-08-17 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ� ���� ����
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// FC_AUCTION
struct MSG_FC_AUCTION_REGISTER_ITEM
{
	UID32_t		ItemUID;
	INT			Price;
};			// C->F, ��� ������ ���

struct MSG_FC_AUCTION_REGISTER_ITEM_OK
{
	UID32_t		ItemUID;
	INT			Price;
};		// F->C, ��� ������ ��� ���

struct MSG_FC_AUCTION_CANCEL_REGISTER
{
	UID32_t		ItemUID;
};		// C->F, ��� ������ ��� ���

struct MSG_FC_AUCTION_CANCEL_REGISTER_OK
{
	UID32_t		ItemUID;
};	// F->C, ��� ������ ��� ��� ���

struct MSG_FC_AUCTION_BUY_ITEM
{
	UID32_t		ItemUID;
};				// C->F, ��� ������ ����

struct MSG_FC_AUCTION_BUY_ITEM_OK
{
	UID32_t		ItemUID;
};			// F->C, ��� ������ ���� ���

struct MSG_FC_AUCTION_GET_ITEM_LIST
{
	UID32_t		CharacterUID;			// �ڽ��� ����� ��� �������� ��û�� ���� �ڽ��� CharacterUID, �ƴϸ� 0
	BYTE		AuctionKind;			// ��û�� ��� ������ ����, AUCTION_KIND_XXX
};			// C->F, ��� ������ ��� ��û

			// AUCTION_KIND_XXX
#define AUCTION_KIND_MY_ITEM	((BYTE)0)	// �ڽ��� ����� ������
#define AUCTION_KIND_ATTACH		((BYTE)1)	// ������ 0 ~ 17, 22, 25 ~ 26
#define AUCTION_KIND_CARD		((BYTE)2)	// ī��� 21, 27
#define AUCTION_KIND_ETC		((BYTE)3)	// ��Ÿ�� 18 ~ 20, 23 ~ 24

template<typename T> bool IS_AUCTION_KIND_ATTACH(T _ITEM_KIND)
{
	return IS_WEAPON(_ITEM_KIND)
		|| _ITEM_KIND == ITEMKIND_DEFENSE
		|| _ITEM_KIND == ITEMKIND_SUPPORT
		|| _ITEM_KIND == ITEMKIND_TANK
		|| _ITEM_KIND == ITEMKIND_RADAR
		|| _ITEM_KIND == ITEMKIND_COMPUTER;
}

template<typename T> bool IS_AUCTION_KIND_CARD(T _ITEM_KIND)
{
	return _ITEM_KIND == ITEMKIND_ENCHANT || _ITEM_KIND == ITEMKIND_GAMBLE;
}

template<typename T> bool IS_AUCTION_KIND_ETC(T _ITEM_KIND)
{
	return _ITEM_KIND == ITEMKIND_ENERGY
		|| _ITEM_KIND == ITEMKIND_INGOT
		|| _ITEM_KIND == ITEMKIND_CARD
		|| _ITEM_KIND == ITEMKIND_BULLET
		|| _ITEM_KIND == ITEMKIND_QUEST;
}

struct MSG_FC_AUCTION_INSERT_ITEM
{
	BYTE			AuctionKind;		// AUCTION_KIND_XXX
	char			SellerCharacterName[SIZE_MAX_CHARACTER_NAME];
	ITEM_GENERAL	AuctionItemGeneral;	// ��� ������
	int				AuctionPrice;		// ��� ����
	ATUM_DATE_TIME	AuctionStartDate;	// ��� ���� �ð�
};			// F->C, ��� ������ ���ۿ�

struct MSG_FC_AUCTION_PUT_ENCHANT
{
	BYTE			AuctionKind;		// AUCTION_KIND_XXX
	ENCHANT			AuctionItemEnchant;
};			// F->C, ��� �������� ��æƮ ���� ���ۿ�

			///////////////////////////////////////////////////////////////////////////////
			// FC_SKILL
struct MSG_FC_SKILL_USE_SKILL_POINT
{
	UINT		KindOfSkill;
};

struct MSG_FC_SKILL_USE_SKILL_POINT_OK
{
	UINT		KindOfSkill;
	BYTE		UsedPoints;
	BYTE		RemainedPoints;
};

struct MSG_FC_SKILL_SETUP_SKILL
{
	INT			ItemNum;
};

struct MSG_FC_SKILL_SETUP_SKILL_OK
{
	BYTE		ItemUpdateType;		// IUT_SHOP, IUT_LOADING, IUT_SKILL
	ITEM_SKILL	ItemSkill;
};

struct MSG_FC_SKILL_USE_SKILL
{
	ItemID_t		SkillItemID;
	ClientIndex_t	AttackIndex;
	ClientIndex_t	TargetIndex;	// target�� ���� skill�� ��� 0
	UID32_t			TargetCharUID;	// 2005-11-24 by cmkwon, target������ TargetIndex�� 0�϶� ����, TargetIndex�� ���� 1�� ��ȯ ��ų�� ����Ѵ�.
};

struct MSG_FC_SKILL_USE_SKILL_OK
{
	ItemID_t		SkillItemID;
	ClientIndex_t	AttackIndex;
	ClientIndex_t	TargetIndex;	// target�� ���� skill�� ��� 0
	ATUM_DATE_TIME	UseTime;		// 2006-11-17 by dhjin, 2�� ��ų ��� �ð�
#ifdef NEMERIAN_NATION_BALANCE_BUFF_PENALITY
	float			penality;
#endif
};

struct MSG_FC_SKILL_CANCEL_SKILL
{
	ItemID_t		SkillItemID;			// ����Ǵ� ��ų ����
	INT				AttackSkillItemNum0;	// 2006-12-12 by cmkwon, ���� ��ų�� ����ǰ� �ϴ� ���ݽ�ų �����۳ѹ� 
};

struct MSG_FC_SKILL_CANCEL_SKILL_OK
{
	ClientIndex_t	ClientIndex;
	ItemID_t		SkillItemID;
	INT				AttackSkillItemNum0;	// 2006-12-12 by cmkwon, ���� ��ų�� ����ǰ� �ϴ� ���ݽ�ų �����۳ѹ�
};

struct MSG_FC_SKILL_INVALIDATE_SKILL
{
	ItemID_t		SkillItemID;
	ClientIndex_t	ClientIndex;
};	// skill ��� ����(�ð� ������ ���� ���)

struct MSG_FC_SKILL_PREPARE_USE
{
	ItemID_t		SkillItemID;
};

struct MSG_FC_SKILL_PREPARE_USE_OK
{
	ClientIndex_t	ClientIndex;
	ItemID_t		SkillItemID;
};

struct MSG_FC_SKILL_CANCEL_PREPARE
{
	ItemID_t		SkillItemID;
};

struct MSG_FC_SKILL_CANCEL_PREPARE_OK
{
	ClientIndex_t	ClientIndex;
	ItemID_t		SkillItemID;
};

struct MSG_FC_SKILL_CONFIRM_USE			// 2005-12-02 by cmkwon
{
	char				szAttackCharacterName[SIZE_MAX_CHARACTER_NAME];	// ��ų ������� CharacterName;
	UID32_t				AttackCharacterUID;		// ��ų ����� CharacterUID
	UID32_t				TargetCharacterUID;		// ��ų Ÿ�� CharacterUID
	int					UsingSkillItemNum;		// ��� ��ų ItemNum
	MAP_CHANNEL_INDEX	MapChannelIndex;		// ��ų ������� MapChannelIndex
	int					SkillConfirmUseUID;		// 2009-04-06 by cmkwon, �ݿ��� ����� ��ų �ý��� ���� - �ش� ��û�� UID
};

struct MSG_FC_SKILL_CONFIRM_USE_ACK		// 2005-12-02 by cmkwon
{
	bool				bYesOrNo;				// ���� ����
	UID32_t				AttackCharacterUID;		// ��ų ����� CharacterUID
	UID32_t				TargetCharacterUID;		// ��ų Ÿ�� CharacterUID
	int					UsingSkillItemNum;		// ��� ��ų ItemNum
	int					SkillConfirmUseUID;		// 2009-04-06 by cmkwon, �ݿ��� ����� ��ų �ý��� ���� - �ش� ��û�� UID
};



///////////////////////////////////////////////////////////////////////////////
// FN_SKILL
using MSG_FN_SKILL_USE_SKILL = MSG_FC_SKILL_USE_SKILL;
struct MSG_FN_SKILL_USE_SKILL_OK : public MSG_FC_SKILL_USE_SKILL_OK
{
	ChannelIndex_t		ChannelIndex;
};

///////////////////////////////////////////////////////////////////////////////
// FC_SYNC
//typedef struct
//{
//	INT		ItemNum;
//} MSG_FC_SYNC_SKILL_REUSE_OK;

///////////////////////////////////////////////////////////////////////////////
// FC_INFO
struct MSG_FC_INFO_GET_MONSTER_INFO
{
	INT				MonsterUnitKind;	// ���� ���� ��ȣ
};

struct MEX_MONSTER_INFO
{
	INT				MonsterUnitKind;					// ���� ������ȣ
	char			MonsterName[SIZE_MAX_MONSTER_NAME];	// ���� �̸�
	BYTE			Level;								// ������ Level
	INT				HP;									// ����
	BYTE			Size;								// �ʵ� ������ SizeForClient�� loading,  NPC ������ SizeForServer�� loading
	BYTE			Belligerence;						// ȣ����, // 2005-12-28 by cmkwon �߰���
	BYTE			AlphaBlending;						// alpha blending ����, TRUE(1), FALSE(0), client�� ����� ���� �߰�, 20030616
	USHORT			RenderIndex;
	float			ScaleValue;
	BYTE			TextureIndex;
	UINT			SourceIndex;
	BitFlag64_t		MPOption;			// 2010-01-11 by cmkwon, ���� MPOption 64bit�� ���� - ����(BYTE)
	BYTE			ClickEvent;							// 2007-09-05 by dhjin, ���� Ŭ�� �̺�Ʈ �߰�
	char			PortraitFileName[SIZE_MAX_FILE_NAME];	// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - ���� �ʻ�ȭ ����

															// operator overloading
	MEX_MONSTER_INFO& operator=(const MONSTER_INFO& rhs)
	{
		this->MonsterUnitKind = rhs.MonsterUnitKind;
		util::strncpy(this->MonsterName, rhs.MonsterName, SIZE_MAX_MONSTER_NAME);
		this->Level = rhs.Level;
		this->HP = rhs.MonsterHP;
		this->Size = rhs.Size;
		this->Belligerence = rhs.Belligerence;				// ȣ����
		this->AlphaBlending = rhs.AlphaBlending;
		this->RenderIndex = rhs.RenderIndex;
		this->ScaleValue = rhs.ScaleValue;
		this->TextureIndex = rhs.TextureIndex;
		this->SourceIndex = rhs.SourceIndex;
		this->MPOption = rhs.MPOption;
		this->ClickEvent = rhs.ClickEvent;				// 2007-09-05 by dhjin
		util::strncpy(this->PortraitFileName, rhs.PortraitFileName, SIZE_MAX_FILE_NAME);	// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - ���� �ʻ�ȭ ����

		return *this;
	}
};

struct MSG_FC_INFO_GET_MONSTER_INFO_OK
{
	MEX_MONSTER_INFO	MonsterInfo;
};

struct MSG_FC_INFO_GET_MAPOBJECT_INFO
{
	INT				Code;
};

struct MSG_FC_INFO_GET_MAPOBJECT_INFO_OK
{
	MAPOBJECTINFO	MapObjectInfo;
};

struct MSG_FC_INFO_GET_ITEM_INFO
{
	INT				ItemNum;
};

struct MSG_FC_INFO_GET_ITEM_INFO_OK
{
	ITEM			ItemInfo;
};

struct MSG_FC_INFO_GET_RARE_ITEM_INFO
{
	INT				CodeNum;
};

struct MSG_FC_INFO_GET_RARE_ITEM_INFO_OK
{
	RARE_ITEM_INFO	RareItemInfo;
};

struct MSG_FC_INFO_GET_BUILDINGNPC_INFO
{
	INT				BuildingIndex;
};

struct MSG_FC_INFO_GET_BUILDINGNPC_INFO_OK
{
	BUILDINGNPC		BuildingNPCInfo;
};

struct MSG_FC_INFO_GET_SIMPLE_ITEM_INFO
{
	INT				ItemNum;
};

struct MEX_ITEM_INFO
{
	INT			ItemNum;						// ������ ������ȣ, ���� �������� �� (ITEM_BASE*)
	BYTE		Kind;							// ������ ����(�����, ��, ����, ��ų.....), ITEMKIND_XXX
	float		AbilityMin;						// �������ּҼ���
	float		AbilityMax;						// �������ִ뼺��
	USHORT		ReqUnitKind;					// �ʿ���������
	BYTE		ReqMinLevel;					// �ʿ� ���� ����
	BYTE		ReqMaxLevel;					// �ʿ� ���� ����
	float		HitRate;						// ����Ȯ��(0~255), // 2010-07-19 by dhjin, Ȯ�� ���� ����
	BYTE		Defense;						// ����
	BYTE		SpeedPenalty;					// ���ǵ����Ƽ, �̵��ӵ�����ġ�� ����(-:����)
	USHORT		Range;							// ���ݹ���, �������� ��쿡�� �ν��� ���� �� �ӵ�
#ifdef SHOP_PRICES_PER_BUILDING_NPC
	UINT		SellingPrice;
#else
	UINT		Price;							// �ּ� �ŷ� ������ ����
	UINT		CashPrice;						// �ּ� �ŷ� ������ ���� ����
#endif
												// 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - 
												// 	BYTE		DestParameter1;					// ����Ķ����1
												// 	float		ParameterValue1;				// �����Ķ����1
												// 	BYTE		DestParameter2;					// ����Ķ����2
												// 	float		ParameterValue2;				// �����Ķ����2
												// 	BYTE		DestParameter3;					// ����Ķ����3
												// 	float		ParameterValue3;				// �����Ķ����3
												// 	BYTE		DestParameter4;					// ����Ķ����4
												// 	float		ParameterValue4;				// �����Ķ����4
	DestParam_t	ArrDestParameter[SIZE_MAX_DESPARAM_COUNT_IN_ITEM];	// 2011-08-01 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ��� ���� (DestParameter - 255 -> 32767 ����)) - // 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - 
	float		ArrParameterValue[SIZE_MAX_DESPARAM_COUNT_IN_ITEM];	// 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - 

	UINT		ReAttacktime;					// �� ���ݽð�(ms)
	INT			Time;							// ���� �ð�(��ų�� ��)
	USHORT		RepeatTime;						// 2006-12-08 by cmkwon, �߰���(����������� ���� �Ѿ� ���� ���, �������� ����, �ð��� ��ų������ ���� �ð�, ������ ��ų�� ��� ����)
	float		RangeAngle;						// ��������(0 ~ PI)
	BYTE		MultiTarget;					// ���ÿ� ���� �� �ִ� Ÿ���� ��
	USHORT		ExplosionRange;					// ���߹ݰ�(���� �� �������� ������ ��ġ�� �ݰ�)
	USHORT		ReactionRange;					// �����ݰ�(���� ���� �����ϴ� �ݰ�)
	BYTE		ShotNum;						// ���� ��,	���� �� �߻� ���� ��Ÿ����.
	BYTE		MultiNum;						// ���� �߻� ź ��,	1�� �߻翡 ����� ���ÿ� ��������
	USHORT		AttackTime;						// ���ݽð�, ������ �ϱ� ���� �ʿ��� �ð�
	BYTE		ReqSP;							// SP �Ҹ�(��ų)
	BYTE		OrbitType;						// �̻���, ���� ���� ����
	BitFlag64_t	ItemAttribute;					// �������� �Ӽ�, ITEM_ATTR_XXX
	FLOAT		BoosterAngle;					// �ν��ͽÿ� ������ ȸ����, ����� �������� ���

												// operator overloading
	MEX_ITEM_INFO& operator=(const ITEM& rhs);
};
struct MSG_FC_INFO_GET_SIMPLE_ITEM_INFO_OK
{
	MEX_ITEM_INFO	ItemInfo;
};
struct MSG_FC_INFO_GET_ENCHANT_COST
{
	INT				EnchantItemNum;
};		// C->F, ��æƮ ������ ��û
struct MSG_FC_INFO_GET_ENCHANT_COST_OK
{
	INT				Cost;
};	// F->C, ��æƮ ������ ����

struct MSG_FC_INFO_GET_CURRENT_MAP_INFO		// 2007-04-06 by cmkwon, �߰���
{
	MAP_CHANNEL_INDEX	mapChannelIdx0;			// 2007-04-06 by cmkwon
												// 2009-05-29 by cmkwon, Hash�˰��� �߰�(SHA256) - 
												//UINT				checkSum0;				// 2007-04-06 by cmkwon, map�� checksum ���
	int					nFileSize;			// 2009-05-29 by cmkwon, Hash�˰��� �߰�(SHA256) - 
	BYTE				byDigest[32];		// 2009-05-29 by cmkwon, Hash�˰��� �߰�(SHA256) - 
};

struct MSG_FC_INFO_GET_CURRENT_MAP_INFO_OK
{
	BYTE			IsPKMap;				// 2005-02-15 by cmkwon, ���� ������� ����
	BYTE			IsCityWarStarted;		// ���������� ���� �÷���
	BYTE			byCityWarTeamType;		//
};

enum
{
	HAPPYEV_STATE_TYPE_END = 0,			// �Ϲ� ���Ǿƿ� �̺�Ʈ �����
	HAPPYEV_STATE_TYPE_START = 1,			// �Ϲ� ���Ǿƿ� �̺�Ʈ ���۵�
	HAPPYEV_STATE_TYPE_STARTING = 2,			// �Ϲ� ���Ǿƿ� �̺�Ʈ ������
	PCBANG_HAPPYEV_STATE_TYPE_END = 10,			// PCBang ���Ǿƿ� �̺�Ʈ �����
	PCBANG_HAPPYEV_STATE_TYPE_START = 11,			// PCBang ���Ǿƿ� �̺�Ʈ ���۵�
	PCBANG_HAPPYEV_STATE_TYPE_STARTING = 12,			// PCBang ���Ǿƿ� �̺�Ʈ ������
	GAME_EVENT_GROUP_MOTHERSHIP_END = 20,		// 2008-05-20 by cmkwon, ��� �̺�Ʈ(HappyHoure,MotherShip,Item) �׷� ���ÿ� �����ϰ� - 
	GAME_EVENT_GROUP_MOTHERSHIP_START = 21,		// 2008-05-20 by cmkwon, ��� �̺�Ʈ(HappyHoure,MotherShip,Item) �׷� ���ÿ� �����ϰ� - 
	GAME_EVENT_GROUP_MOTHERSHIP_STARTING = 22,		// 2008-05-20 by cmkwon, ��� �̺�Ʈ(HappyHoure,MotherShip,Item) �׷� ���ÿ� �����ϰ� - 
	GAME_EVENT_GROUP_ITEM_END = 30,		// 2008-05-20 by cmkwon, ��� �̺�Ʈ(HappyHoure,MotherShip,Item) �׷� ���ÿ� �����ϰ� - 
	GAME_EVENT_GROUP_ITEM_START = 31,		// 2008-05-20 by cmkwon, ��� �̺�Ʈ(HappyHoure,MotherShip,Item) �׷� ���ÿ� �����ϰ� - 
	GAME_EVENT_GROUP_ITEM_STARTING = 32,		// 2008-05-20 by cmkwon, ��� �̺�Ʈ(HappyHoure,MotherShip,Item) �׷� ���ÿ� �����ϰ� - 
	GAME_EVENT_GROUP_CRYSTAL_SYSTEM_END = 40,		// 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
	GAME_EVENT_GROUP_CRYSTAL_SYSTEM_START = 41,		// 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
	GAME_EVENT_GROUP_CRYSTAL_SYSTEM_STARTING = 42,		// 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
	GAME_EVENT_GROUP_CRYSTAL_SYSTEM_2ND_END = 50,		// 2012-05-08 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý��� 2��
	GAME_EVENT_GROUP_CRYSTAL_SYSTEM_2ND_START = 51,		// 2012-05-08 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý��� 2��
	GAME_EVENT_GROUP_CRYSTAL_SYSTEM_2ND_STARTING = 52		// 2012-05-08 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý��� 2��
};
struct MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK
{
	BYTE			byStateType;			// 0(�����), 1(���۵�), 2(������)
	BYTE			byInfluenceType4;		// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK ����ü�� �߰��� �ʵ�
	ATUM_DATE_TIME	StartATime;
	ATUM_DATE_TIME	EndATime;
	float			fEXPRate2;
	float			fSPIRate2;
	float			fEXPRepairRate2;
	float			fDropItemRate2;
	float			fDropRareRate2;
	float			fWarPointRate2;			// 2007-06-26 by dhjin, ������Ʈ �̺�Ʈ ���� �߰�
};
struct MSG_FC_INFO_GET_GAME_EVENT_INFO_OK
{
	BYTE			byStateType;			// 0(�����), 1(���۵�), 2(������)
	int				nGameEventType;			// �̺�Ʈ Ÿ��
	float			fRate;					// �̺�Ʈ Rate
	int				nRemainMinute;			// ���� �ð�(����:��)
};

struct MSG_FC_INFO_GET_SERVER_DATE_TIME_OK		// 2006-10-11 by cmkwon
{
	ATUM_DATE_TIME	atimeCurServerTime;		// 2006-10-11 by cmkwon, ���� ���� ��¥ �ð�
};

struct MSG_FC_INFO_CHECK_RESOBJ_CHECKSUM		// 2007-05-28 by cmkwon
{
	int			nItemNum0;
	char		szObjFileName[SIZE_MAX_RESOBJ_FILE_NAME];
	// 2009-05-29 by cmkwon, Hash�˰��� �߰�(SHA256) - 
	//UINT		uiCheckSum0;
	int			nFileSize0;
	BYTE		byDigest[32];		// 2009-05-29 by cmkwon, Hash�˰��� �߰�(SHA256) - 
};


///////////////////////////////////////////////////////////////////////////////
// FC_REQUEST - ĳ���Ͱ��� ��û, ����, ���� � ����, general-purpose
// REQUEST_TYPE_XXX
#define REQUEST_TYPE_NULL			((BYTE)0)
#define REQUEST_TYPE_TRADE			((BYTE)1)	// �ŷ�
#define REQUEST_TYPE_PARTY_INVITE	((BYTE)2)	// ��Ƽ �ʴ�
#define REQUEST_TYPE_GUILD_INVITE	((BYTE)3)	// ���� �ʴ�
#define REQUEST_TYPE_P2P_PK			((BYTE)4)	// 1�� 1 ����
#define REQUEST_TYPE_PARTY_BATTLE	((BYTE)5)	// ��Ƽ�� ����
#define REQUEST_TYPE_GUILD_WAR		((BYTE)6)	// ������ ��û, ���� �ʿ��� ������ ����

struct MSG_FC_REQUEST_REQUEST
{
	ClientIndex_t	TargetClientIndex;	// �ǿ�û��
	char			TargetCharacterName[SIZE_MAX_CHARACTER_NAME];	// ClientIndex�� 0�� ��� �� �ʵ� ����
	BYTE			RequestType;
};				// C->F, ��û

struct MSG_FC_REQUEST_REQUEST_OK
{
	ClientIndex_t	SourceClientIndex;	// ��û��
	char			SourceCharacterName[SIZE_MAX_CHARACTER_NAME];	// ClientIndex�� 0�� ��� �� �ʵ� ����
	BYTE			RequestType;
};			// F->C, ��û�� ����

struct MSG_FC_REQUEST_ACCEPT_REQUEST
{
	ClientIndex_t	SourceClientIndex;	// ��û��
	char			SourceCharacterName[SIZE_MAX_CHARACTER_NAME];	// ClientIndex�� 0�� ��� �� �ʵ� ����
	BYTE			RequestType;
};		// C->F, �³�

struct MSG_FC_REQUEST_ACCEPT_REQUEST_OK
{
	ClientIndex_t	PeerClientIndex;	// ����
	char			PeerCharacterName[SIZE_MAX_CHARACTER_NAME];	// ClientIndex�� 0�� ��� �� �ʵ� ����
	BYTE			RequestType;
};		// F->C, �³��� ����, ��(Ȥ�� ��) ������ ����

struct MSG_FC_REQUEST_REJECT_REQUEST
{
	ClientIndex_t	SourceClientIndex;	// ��û��
	char			SourceCharacterName[SIZE_MAX_CHARACTER_NAME];	// ClientIndex�� 0�� ��� �� �ʵ� ����
	BYTE			RequestType;
};		// C->F, ����

struct MSG_FC_REQUEST_REJECT_REQUEST_OK
{
	ClientIndex_t	PeerClientIndex;	// ����
	char			PeerCharacterName[SIZE_MAX_CHARACTER_NAME];	// ClientIndex�� 0�� ��� �� �ʵ� ����
	BYTE			RequestType;
};		// F->C, ������ ����, ��(Ȥ�� ��) ������ ����

struct MSG_FC_REQUEST_CANCEL_REQUEST
{
	ClientIndex_t	PeerClientIndex;	// ����
	char			PeerCharacterName[SIZE_MAX_CHARACTER_NAME];	// ClientIndex�� 0�� ��� �� �ʵ� ����
	BYTE			RequestType;
};		// C->F, ��û ��ҵ�, ��(Ȥ�� ��) ������ ����

		///////////////////////////////////////////////////////////////////////////////
		// FC_CITY - ���ÿ� ��������
struct MSG_FC_CITY_GET_BUILDING_LIST
{
	MAP_CHANNEL_INDEX	MapChannelIndex;
};

struct CITY_BUILDING_INFO_4_EXCHANGE
{
	UINT	BuildingIndex;	// �ǹ�(���� ��) ���� ��ȣ
	BYTE	BuildingKind;
};

struct MSG_FC_CITY_GET_BUILDING_LIST_OK
{
	INT		NumOfBuildings;
	ARRAY_(CITY_BUILDING_INFO_4_EXCHANGE);
};

struct MSG_FC_CITY_GET_WARP_TARGET_MAP_LIST
{
	MAP_CHANNEL_INDEX	MapChannelIndex;
};

// 2004-11-10 by cmkwon, ������ ������
//struct WARP_TARGET_MAP_INFO_4_EXCHANGE
//{
//	MapIndex_t	MapIndex;
//	INT			TargetIndex;	// EVENTINFO�� �����ϴ� TargetIndex
//	char		TargetName[SIZE_MAX_WARP_TARGET_NAME];	// Ÿ���� �̸�(ex: ���û��, �����ϴ�)
//	INT			Fee;			// ���� ���
//};

struct MSG_FC_CITY_GET_WARP_TARGET_MAP_LIST_OK
{
	INT		NumOfTargetMaps;
	ARRAY_(WARP_TARGET_MAP_INFO_4_EXCHANGE);
};

struct MSG_FC_CITY_REQUEST_ENTER_BUILDING
{
	INT		BuildingIndex;
};	// C->F, ���� �� ���� ��û

struct MSG_FC_CITY_REQUEST_ENTER_BUILDING_OK
{
	INT		BuildingIndex;
	// 2006-02-08 by cmkwon
	//	float	fCityWarTexRate;			//
	float	fInflDistributionTexPercent;	// 2006-02-08 by cmkwon, ���º��� ����
};	// F->C, ���� ���� �ϷḦ �˸�

struct MSG_FC_CITY_REQUEST_WARP
{
	MapIndex_t	MapIndex;
	INT			TargetIndex;			// EVENTINFO�� �����ϴ� TargetIndex	
};				// C->F, ���ÿ��� �����ؼ� ������ ���� ��û

struct MSG_FC_CITY_CHECK_WARP_STATE_OK
{
	Err_t		Err;				// ERR_NO_ERROR�̸� ����
};	// F->C, ���� ������ ���������� ���� ���

	///////////////////////////////////////////////////////////////////////////////
	// FC_TIMER - Ÿ�̸� ������ ��������
	///////////////////////////////////////////////////////////////////////////////

	// TimerEventType �� ����, TE_TYPE_XXX
#define TE_TYPE_TEST					(TimerEventType)0
	//#define TE_TYPE_SYNC_PRI				(TimerEventType)1
	//#define TE_TYPE_SYNC_SEC				(TimerEventType)2
	//#define TE_TYPE_SYNC_SKILL			(TimerEventType)3
	//#define TE_TYPE_END_SKILL				(TimerEventType)4		// check: Ŭ���̾�Ʈ�� ��û�� ���� ������, �ش� �ð� ��� �� Ŭ���̾�Ʈ���� CANCEL_SKILL ����, 20041005, kelovon with jschoi
	//#define TE_TYPE_DELETE_MINE			(TimerEventType)5	// check: ���� ������ Ŭ���̾�Ʈ���� ��� �����ϱ�� �ϸ鼭 ������, 20041118, kelovon with dhkwon
#define TE_TYPE_DELETE_DUMMY			(TimerEventType)6
#define TE_TYPE_DELETE_FIXER			(TimerEventType)7
#define TE_TYPE_DELETE_DECOY			(TimerEventType)8		// �����̴� TimeOut�߻� �� CurrentCount�� ���̰�, CurrentCount=0���� �ݺ���
#define TE_TYPE_GRADUAL_SHIELD_UP		(TimerEventType)9		// 2-2�� ���� ����, interval�� ������ 1000 ms
#define TE_TYPE_RECOVER_HP				(TimerEventType)10		// �������� ���� ä��(5�ʴ� 3)
#define TE_TYPE_RECOVER_DP				(TimerEventType)11		// �������� ���� ä��
#define TE_TYPE_RECOVER_SP				(TimerEventType)12
#define TE_TYPE_GRADUAL_HP_UP			(TimerEventType)13		// interval�� ������ 1000 ms, ITEM_GENERAL.Endurance�� 0�� �� ������ �ݺ���
#define TE_TYPE_GRADUAL_DP_UP			(TimerEventType)14		// interval�� ������ 1000 ms, ITEM_GENERAL.Endurance�� 0�� �� ������ �ݺ���
#define TE_TYPE_GRADUAL_SP_UP			(TimerEventType)15		// interval�� ������ 1000 ms, ITEM_GENERAL.Endurance�� 0�� �� ������ �ݺ���, not used yet: SP�� �ڵ����� ȸ����, TE_TYPE_RECOVER_SP�� ���
#define TE_TYPE_GRADUAL_EP_UP			(TimerEventType)16		// interval�� ������ 1000 ms, ITEM_GENERAL.Endurance�� 0�� �� ������ �ݺ���
#define TE_TYPE_IMMEDIATE_HP_UP			(TimerEventType)17		// ���� �ٷ� ä��� Time ���� ������ �ȵȴ�
#define TE_TYPE_IMMEDIATE_DP_UP			(TimerEventType)18		// ���� �ٷ� ä��� Time ���� ������ �ȵȴ�
#define TE_TYPE_IMMEDIATE_SP_UP			(TimerEventType)19		// ���� �ٷ� ä��� Time ���� ������ �ȵȴ�
#define TE_TYPE_IMMEDIATE_EP_UP			(TimerEventType)20		// ���� �ٷ� ä��� Time ���� ������ �ȵȴ�
#define TE_TYPE_REQUEST_TIMEOUT			(TimerEventType)21		// �ŷ�, ��Ƽ��, �ϴ��ϴ�� ���� ��û�� ���� TimeOut
#define TE_TYPE_DECREASE_SP				(TimerEventType)22		// TOGGLE�� ��ų ��� ��, SP�� ReqSP��ŭ �Ҹ�, SP ���� �� ��ų �ڵ� ���
#define TE_TYPE_DO_MINUTELY_WORK		(TimerEventType)23		// 1�п� �ѹ��� �����ϴ� Ÿ�̸�
	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 
#define TE_TYPE_DOTSKILL_STEALING		(TimerEventType)24		// Stealing ��ų - SP���� (SP�� ���� �Ǿ ��� ���� �Ǿ�� �Ѵ�.)
#define TE_TYPE_DOTSKILL_DRAIN			(TimerEventType)25		// Drain ��ų - �ǰݴ�� HP���ҽ��� ���ݴ�󿡰� ä���ش�.
#define TE_TYPE_DOTSKILL_TIME_BOMB		(TimerEventType)26		// TimeBomb ��ų - �� ��󿡰� �ð� ����� ���� �ǿ� %�� �������� ������.
#define TE_TYPE_FIXED_TERM_SHAPE		(TimerEventType)27		// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)


#define TIMER_DOT_INTERVAL					1000


#define TIMER_DECOY_TIME_CHECK_INTERVAL		1000
#define TIMER_GRADUAL_UP_INTERVAL			1000	// gradual up�� ���Ǵ� interval, DELETE_DECOY���� �����
#define _DEFAULT_TIME_GRANULARITY			100		// milli-seconds
#define _MAX_TIMER_INTERVAL_TIME			150000	// maximum 150 seconds, check: �����ؾ� ��
#define SIZE_MAX_TIMER_EVENT_MEMPOOL		1000
#define _TIMER_TICK							_DEFAULT_TIME_GRANULARITY/3
#define TIMER_REPEAT_INFINITE				MAX_INT_VALUE
#define TIMER_DECREASE_SP_INTERVAL			1000				// 1��
#define TIMER_DO_MINUTELY_WORK				60*1000				// 1��


#define MANAGE_MAX_TIME_FIXED_TERM_MINUTELY		10080				// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����) - 1�� 60�� * 24�ð� * 7�� (�Ⱓ�� ������ 7������ ���� �͸� ó��)

struct MEX_TIMER_EVENT
{
	TimerEventType		Type;
	UID32_t				TimerUID;				// Ÿ�̸� ���̵�
	TimeUnit_t			StartTimeStamp;			// milli-seconds
	TimeUnit_t			ExpireTime;				// milli-seconds
	TimeUnit_t			TimeInterval;			// milli-seconds, (ExpireTime - StartTimeStamp)
	UID32_t				CharacterUniqueNumber;	// event�� ������ charac�� ������ �ٸ� charac�� socket�� ����ϴ� ��츦 ���� ����, IsUsing()�� CharacterUniqueNumber�� ���ƾ� ��!
	ClientIndex_t		ClientIndex;			// event�� ������ charac�� ������ �ٸ� charac�� socket�� ����ϴ� ��츦 ���� ����, IsUsing()�� ClientIndex�� ���ƾ� ��!
	UINT				nGeneralParam1;			// end_skill: ITEM_SKILL* ,	mine: CMapBlock*,	delete_item��: ITEM_GENERAL*, GRADUAL_SHIELD_UP: HI(ItemUID), GRADUAL_XXX_UP: ITEM*, DELETE_DUMMY: FIELD_DUMMY*
	UINT				nGeneralParam2;			// end_skill: ,				mine: DROPMINE*,	delete_item��:              , GRADUAL_SHIELD_UP: LO(ItemUID)
	UINT				nGeneralParam3;			// end_skill: ,				mine: CFieldIOCP*,	delete_item��:              , GRADUAL_SHIELD_UP: ItemNum
	bool				bRepeatable;			// repeat�ؾ� �ϴ��� ����
	INT					nRemainedRepeatCount;	// ���� Ƚ��, ~0(0xFFFFFFFF): ���� �ݺ�, 0����: ����, 1�̻�: ȸ����ŭ �ݺ�
};

struct MSG_FC_TIMER_START_TIMER
{
	MEX_TIMER_EVENT		TimerEvent;
};			// F->C, TIMER_EVENT ����

struct MSG_FC_TIMER_STOP_TIMER
{
	UID32_t				TimerUID;
};			// F->C, TIMER_EVENT ����

struct MSG_FC_TIMER_UPDATE_TIMER
{
	MEX_TIMER_EVENT		TimerEvent;
};		// F->C, TIMER_EVENT ����(�ð� ����)

struct MSG_FC_TIMER_PAUSE_TIMER
{
	UID32_t				TimerUID;
};			// F->C, TIMER_EVENT �Ͻ� ����

struct MSG_FC_TIMER_CONTINUE_TIMER
{
	UID32_t				TimerUID;
};		// F->C, TIMER_EVENT �����

struct MSG_FC_TIMER_TIMEOUT
{
	MEX_TIMER_EVENT		TimerEvent;
};				// C->F, �ð��� �� ���� �˸�

				///////////////////////////////////////////////////////////////////////////////
				// FC_CLIENT_REPORT
struct MSG_FC_CLIENT_REPORT
{
	char			ClientPrivateIP[SIZE_MAX_IPADDRESS];	// �缳���� ��� private ip
	char			CharacterName[SIZE_MAX_CHARACTER_NAME];
	char			Message[SIZE_MAX_CLIENT_REPORT];
};		// C -> F

///////////////////////////////////////////////////////////////////////////////
// STRING128 - DEBUG Message
struct MSG_FC_STRING_128
{
	BYTE			PrintType;		// STR128_PRINT_TYPE_XXX
	char			String[128];
};
struct MSG_FC_STRING_256
{
	BYTE			PrintType;		// STR128_PRINT_TYPE_XXX
	char			String[256];
};
struct MSG_FC_STRING_512
{
	BYTE			PrintType;		// STR128_PRINT_TYPE_XXX
	char			String[512];
};

struct MSF_FC_WORLD_NOTIFICATION
{
	char	String[512];
	bool	Notification;
};

#define STR128_PRINT_TYPE_CHAT		(BYTE)0x00	// ä��â�� ���
#define STR128_PRINT_TYPE_STATIC	(BYTE)0x01	// ���� text�� ���
#define STR128_PRINT_TYPE_DBGOUT	(BYTE)0x02	// Ŭ���̾�Ʈ DBGOUT ��� - // 2012-02-13 by hskim, ���� ������ ��� �׽�Ʈ ��ɾ�
#define STR128_PRINT_TYPE_NOTICE	(BYTE)0x03	// Ŭ���̾�Ʈ ���� ��ũ�ѿ� �ø� - // 2012-03-30 by hskim, EP4 Ʈ���� �ý��� �̺�Ʈ �˸�
#define STR128_PRINT_TYPE_POPUP		(BYTE)0x04	// Ŭ���̾�Ʈ ���� �˾����� �ٿ� - // 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - STR128_PRINT_TYPE_POPUP �߰�

using MSG_IC_STRING_128 = MSG_FC_STRING_128;
using MSG_IC_STRING_256 = MSG_FC_STRING_256;
using MSG_IC_STRING_512 = MSG_FC_STRING_512;

///////////////////////////////////////////////////////////////////////////////
// FI_ADMIN
struct MSG_FI_ADMIN_GET_CHARACTER_INFO
{
	BYTE	Type;			// see below
	UID32_t	AdminCharacterUniqueNumber;
	UID32_t	CharacterUniqueNumber;
};

#define ADMIN_GET_CHARAC_TYPE_SEND	 (BYTE)0	// �׳� ���ۿ�
#define ADMIN_GET_CHARAC_TYPE_MOVETO (BYTE)1	// ������

struct MSG_FI_ADMIN_GET_CHARACTER_INFO_OK
{
	BYTE		Type;		// see above
	UID32_t		AdminCharacterUniqueNumber;
	char		AccountName[SIZE_MAX_ACCOUNT_NAME];
	char		CharacterName[SIZE_MAX_CHARACTER_NAME];
	MAP_CHANNEL_INDEX	MapChannelIndex;
	AVECTOR3	Position;
	BYTE		Level;
};

struct MSG_FI_ADMIN_CALL_CHARACTER
{
	UID32_t			CharacterUniqueNumber;
	MAP_CHANNEL_INDEX	MapChannelIndex;			// admin�� ���̸�
	AVECTOR3		Position;						// admin�� ��ǥ
};	// I -> F

struct MSG_FI_ADMIN_MOVETO_CHARACTER
{
	UID32_t			AdminCharacterUniqueNumber;
	MAP_CHANNEL_INDEX	MapChannelIndex;			// Target�� ���̸�
	AVECTOR3		Position;						// Target�� ��ǥ
};

struct MSG_FI_ADMIN_CHANGE_WEATHER
{
	MapIndex_t		MapIndex;		// MAP_INDEX_ALL �̸� ��� �� �ǹ�
	BitFlag16_t		MapWeather;
	bool			OnOff;
};

///////////////////////////////////////////////////////////////////////////////
// IC_ADMIN
struct MSG_IC_ADMIN_CALL_CHARACTER
{
	char				CharacterName[SIZE_MAX_CHARACTER_NAME];
	MAP_CHANNEL_INDEX	MapChannelIndex;				// admin�� ���̸�
	AVECTOR3			Position;						// admin�� ��ǥ
};	// I -> C

struct MSG_IC_ADMIN_GET_SERVER_STAT_OK
{
	INT					TotalUserCount;				// ������ ��ü�� ����� ��
	INT					BCUUserCount;
	INT					ANIUSerCount;
};					// I -> C

struct MSG_IC_ADMIN_CALL_GUILD
{
	char				GuildName[SIZE_MAX_GUILD_NAME];
	MAP_CHANNEL_INDEX	MapChannelIndex;				// admin�� ���̸�
	AVECTOR3			Position;						// admin�� ��ǥ
};	// I -> C

struct MSG_IC_ADMIN_CALLGM_INFO_OK		// 2006-05-08 by cmkwon
{
	int		nCallGMTotalCount;
};

#define SIZE_MAX_CALLGM_LIST					1000			// 2006-05-08 by cmkwon
#define SIZE_MAX_CALLGM_BRING_COUNT				10				// 2006-05-08 by cmkwon
struct SCALLGM			// 2006-05-08 by cmkwon
{
	UID32_t			CallerCharacterUID;
	BYTE			CallerInflTy;			// 2007-11-19 by cmkwon, �����ý��� ������Ʈ - SCALLGM ����ü�� ���� �ʵ� �߰�
	char			szCallerCharacterName[SIZE_MAX_CHARACTER_NAME];
	ATUM_DATE_TIME	atimeCallTime;
	char			szCallGMDescription[SIZE_MAX_CHAT_MESSAGE];
};
struct MSG_IC_ADMIN_CALLGM_VIEW_OK		// 2006-05-08 by cmkwon
{
	int		nCallGMCount;
	ARRAY_(SCALLGM);
};
using MSG_IC_ADMIN_CALLGM_BRING_OK = MSG_IC_ADMIN_CALLGM_VIEW_OK;		// 2006-05-08 by cmkwon


																		///////////////////////////////////////////////////////////////////////////////
																		// FC_ADMIN
struct MSG_FC_ADMIN_GET_SERVER_STAT_OK
{
	MAP_CHANNEL_INDEX	CurrentMapChannelIndex;		// ���� ��
	INT					CurrentUserCount;			// ���� ���� ����� ��
	INT					CurrentBCUUserCount;
	INT					CurrentANIUserCount;
};					// F -> C

					///////////////////////////////////////////////////////////////////////////////
					// ERROR
struct MSG_ERROR
{
	MessageType_t	MsgType;	// ������ �Ͼ�� �� ó�����̾��� Message Type
	Err_t			ErrorCode;			// 2 bytes
	bool			CloseConnection;	// Client���� ������ �������� ���� ����
	INT				ErrParam1;			// general-purpose err parameter 1
	INT				ErrParam2;			// general-purpose err parameter 2
	USHORT			StringLength;		// 2 bytes
										//char*	String;				// error string
};

///////////////////////////////////////////////////////////////////////////////
// Log Server Protocol (T0: 0x20~)
///////////////////////////////////////////////////////////////////////////////
/*
- Item:		����(�ŷ�,�ݴ°��), ������â->â��,â��->������â, �����Ǻн�(�ŷ��� �������, �ٴڿ� ����߸� ��� ��ǥ)
- Skill:	����, ������ �н�
- Level:	Up(n), down(-n)
- Exp:		Up(n), down(-n)
- Stat:		6��(AttackPart, DefensePart, FuelPart, SoulPart, ShieldPart, DodgePart), type,n
- EndGame:	���ӽð�, ����ð�, IP, PlayTime, ��ǥ
*/


// 2010-04-20 by cmkwon, �ű� ��Ű �ӽ� ���� - 
// enum EN_ENCHANT_LOGTYPE
// {
// 	ENCHANT_LOGTYPE_FAIL		= 0,
// 	ENCHANT_LOGTYPE_SUCCESS		= 1,
// 	ENCHANT_LOGTYPE_TARGETITEM	= 2,
// 	ENCHANT_LOGTYPE_ATTACHITEM	= 3,
// 	ENCHANT_LOGTYPE_INCREASE_PROBABILITY = 4		// 2009-01-19 by dhjin, ��þƮ Ȯ�� ����, 10��þ �Ĺ� ī��
// };
///////////////////////////////////////////////////////////////////////////////
// 2010-04-20 by cmkwon, �ű� ��Ű �ӽ� ���� - 
enum EN_ENCHANT_LOGTYPE
{
	ENCHANT_LOGTYPE_UNKNOWN = 0,		// 
	ENCHANT_LOGTYPE_ENCHANT_FAIL = 100,		// ��æƮ ����
	ENCHANT_LOGTYPE_ENCHANT_SUCCESS = 101,		// ��æƮ ����
	ENCHANT_LOGTYPE_ENCHANT_INIT_FAIL = 110,		// ��æƮ �ʱ�ȭ ����
	ENCHANT_LOGTYPE_ENCHANT_INIT_SUCCESS = 111,
	ENCHANT_LOGTYPE_GAMBLE_PREFIX_FAIL = 120,		// ���� �׺� ����
	ENCHANT_LOGTYPE_GAMBLE_PREFIX_SUCCESS = 121,
	ENCHANT_LOGTYPE_GAMBLE_SUFFIX_FAIL = 122,		// ���� �׺� ����
	ENCHANT_LOGTYPE_GAMBLE_SUFFIX_SUCCESS = 123,
	ENCHANT_LOGTYPE_GAMBLE_PREFIX_INIT_FAIL = 130,		// ���� �׺� �ʱ�ȭ ����
	ENCHANT_LOGTYPE_GAMBLE_PREFIX_INIT_SUCCESS = 131,
	ENCHANT_LOGTYPE_GAMBLE_SUFFIX_INIT_FAIL = 132,		// ���� �׺� �ʱ�ȭ ����
	ENCHANT_LOGTYPE_GAMBLE_SUFFIX_INIT_SUCCESS = 133
};
char *GetEnchatLogTypeString(int i_nEnchantLogTy);		// 2010-04-20 by cmkwon, �ű� ��Ű �ӽ� ���� - 
int GetEnchantLogType(ITEM *i_pEnchantItemInfo, bool i_bSuccessFlag);



// 2013-07-01 by bckim, 10��æƮ �̸� ������ī��ó��, �α��߰�, ��� ����
enum EN_TRY_ENCHANT_SUCCESS_LOGTYPE
{
	TRY_ENCHANT_FAIL = 0,
	TRY_ENCHANT_SUCCESS = 1,
	TRY_ENCHANT_PREVENT_5 = 5,
	TRY_ENCHANT_PREVENT_10 = 10
};
char *GetTryEnchantSuccessTypeString(int i_nSuccessType);
// End. 2013-07-01 by bckim, 10��æƮ �̸� ������ī��ó��, �α��߰�, ��� ����


// CONNECTION LOG(0x00~)
//#define T1_FL_LOG_LOGIN							0x00	// 0 - �α���
//#define T1_FL_LOG_LOGOUT						0x01	// 1 - �α׾ƿ�
// USER LOG(0x10~)
//#define T1_FL_LOG_LEVEL							0x10	// 16 - ���� ��ȭ
//#define T1_FL_LOG_EXP							0x11	// 17 - ����ġ ��ȭ
//#define T1_FL_LOG_STAT							0x12	// 18 - ���� ���
//#define T1_FL_LOG_GAME_START					0x13	// 19 - ���� ����
//#define T1_FL_LOG_GAME_END						0x14	// 20 - ���� ����
//#define T1_FL_LOG_QUEST_COMPLETION				0x15	// 21 - ����Ʈ �Ϸ�
//#define T1_FL_LOG_PKWIN							0x16	// 22 - 1:1 ���� �¸�
//#define T1_FL_LOG_PKLOSS						0x17	// 23 - 1:1 ���� �й�
//#define T1_FL_LOG_DEAD							0x18	// 24 - ��ü ����
//#define T1_FL_LOG_WARP							0x19	// 25 - ����
//#define T1_FL_LOG_SPEED_HACK					0x1A	// 26 - ���ǵ� ��
//#define T1_FL_LOG_CREATE_CHARACTER				0x1B	// 27 - ĳ���� ����
//#define T1_FL_LOG_DELETE_CHARACTER				0x1C	// 28 - ĳ���� ����
//#define T1_FL_LOG_MEMORY_HACK					0x1D	// 29 - �޸���, 2005-12-20 by cmkwon
//#define T1_FL_LOG_PvEWIN						0x1E	// 30 - ���������� ��� ���� ĳ���͸� ����, 2006-03-06 by cmkwon
//#define T1_FL_LOG_PvELOSS						0x1F	// 31 - ���������� ��� ���¿� ���� ����, 2006-03-06 by cmkwon
// ITEM LOG(0x20~)
//#define T1_FL_LOG_ITEM_CHARACTER_ITEM			0x20	// 32 - ��� ���� ������ ����
//#define T1_FL_LOG_ITEM_GET_ITEM					0x21	// 33 - Drop Item ����
//#define T1_FL_LOG_ITEM_TRADE_SEND				0x22	// 34 - ������ �ŷ�: �ֱ�
//#define T1_FL_LOG_ITEM_TRADE_RECV				0x23	// 35 - ������ �ŷ�: �ޱ�
//#define T1_FL_LOG_ITEM_THROWAWAY_ITEM			0x24	// 36 - ������ ������
//#define T1_FL_LOG_ITEM_BUY_ITEM					0x25	// 37 - ������ ����
//#define T1_FL_LOG_ITEM_SELL_ITEM				0x26	// 38 - ������ �Ǹ�
//#define T1_FL_LOG_ITEM_USE_ITEM					0x27	// 39 - ������ ���
//#define T1_FL_LOG_ITEM_USE_ENERGY				0x28	// 40 - �������� ������ ���
//#define T1_FL_LOG_ITEM_USE_ENCHANT				0x29	// 41 - ��æƮ�� ������ ���
//#define T1_FL_LOG_ITEM_ADD_ITEM_BY_ADMIN		0x2A	// 42 - ����� ���� ����
//#define T1_FL_LOG_ITEM_DEL_ITEM_BY_ADMIN		0x2B	// 43 - ����� ���� ����
//#define T1_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND		0x2C	// 44 - ��ɾ ���� ����
//#define T1_FL_LOG_ITEM_INSERT_TO_STORE			0x2D	// 45 - â�� ����
//#define T1_FL_LOG_ITEM_DRAW_FROM_STORE			0x2E	// 46 - â���� ������
//#define T1_FL_LOG_ITEM_STORE_ITEM				0x2F	// 47 - â�� ���� ������ ����, â�� ���� ������
//#define T1_FL_LOG_ITEM_USE_MIX					0x30	// 48 - ����
//#define T1_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN	0x31	// 49 - ĳ���� ����� �������� ĳ����
//#define T1_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_NEW	0x32	// 50 - ĳ���� ����� ���ο� ĳ����
//#define T1_FL_LOG_ITEM_BAZAAR_SEND				0x33	// 51 - ���λ��� �ŷ�: �ֱ�, 2006-07-27 by cmkwon
//#define T1_FL_LOG_ITEM_BAZAAR_RECV				0x34	// 52 - ���λ��� �ŷ�: �ޱ�, 2006-07-27 by cmkwon
//#define T1_FL_LOG_ITEM_INSERT_TO_GUILDSTORE		0x35	// 53 - ���� â�� ����, 2006-09-27 by dhjin
//#define T1_FL_LOG_ITEM_DRAW_FROM_GUILDSTORE		0x36	// 54 - ���� â���� ������, 2006-09-27 by dhjin
//#define T1_FL_LOG_ITEM_SPI						0x37	// 55 - ĳ���� ������ �ٿ� SPI Log, 2006-10-27 by cmkwon
//#define T1_FL_LOG_ITEM_GET_ITEM_BY_ITEMEVENT	0x38	// 56 - ItemEvent�� ���� ������ �߰�, 2007-01-11 by cmkwon
//#define T1_FL_LOG_QUEST_COMPENSATION			0x39	// 57 - ����Ʈ ����, 2007-01-16 by dhjin
//#define T1_FL_LOG_WARPOINT						0x3A	// 58 - WarPoint ����, 2007-05-15 by dhjin
//#define T1_FL_LOG_ARENA_TEAM_MATCH				0x3B	// 59 - Arena �� ��Ī, 2007-06-11 by dhjin
//#define T1_FL_LOG_TUTORIAL_COMPLETION			0x3C	// 60 - Tutorial, 2007-07-06 by dhjin
//#define T1_FL_LOG_OUTPOST_START					0x3D	// 61 - OutPost���� �α�, 2007-10-02 by dhjin
//#define T1_FL_LOG_OUTPOST_END					0x3E	// 62 - OutPost���� �α�, 2007-10-02 by dhjin
//#define T1_FL_LOG_EXPEDIENCYFUND				0x3F	// 63 - �ǰ��� ��/�� �α�, 2007-10-02 by dhjin

//#define T1_FL_LOG_ITEM_BUY_CASH_PRICE			0x46	// 70 - // 2012-10-16 by bckim, ĳ�������� �������� Log �߰�
//#define T1_FL_LOG_ITEM_CHARACTER_ITEM_GUILDSTORE_RESULT 0x49 // 73 - // 2012-12-17 by bckim, ���â�� ������ �̵� ���� �߰��α� 
// 2012-11-21 by bckim, �������߰�, ��æƮ���������ű��
//#define T1_FL_LOG_ITEM_ENCHANT_BY_ADMIN			0x47	// 71 - ������ ��æƮ ���� Log �߰�
//#define T1_FL_LOG_ITEM_DISENCHANT_BY_ADMIN		0x48	// 72 - ������ ��æƮ ���� Log �߰�
// 2012-11-21 by bckim, �������߰�, ��æƮ���������ű��. End 

//#define T1_FL_LOG_ITEM_TRY_ENCHANT_INFO			0x4B	// 75 - // 2013-07-01 by bckim, 10��æƮ �̸� ������ī��ó��, �α��߰�, ��� ����
//#define T1_FL_LOG_ITEM_DISSOLUTION_ITEM			0x4C	// 76 - // 2013-08-05 by bckim, ���� �������� �α� �߰�


// ETC LOG(0x40~)
//#define T1_FL_LOG_SERVER_INFO_MAP				0x40	// 64 - �ʺ� ������
//#define T1_FL_LOG_SERVER_INFO_TOTAL				0x41	// 65 - 
//#define T1_FL_LOG_HACKING_LOG					0x42	// 66 - 
//#define T1_FL_LOG_LIVE_DELETED_CHARACTER		0x43	// 67 - ������ �ɸ���, 2007-02-22 by dhjin
//#define T1_FL_LOG_EVENT_PARTICIPATION_RATE		0x44	// 68 - // 2010-06-01 by shcho, GLogDB ���� -

// MONSTER LOG(0x50~)
//#define T1_FL_LOG_MONSTER_BOSS					0x50	// 80 - ������ ����
//#define T1_FL_LOG_MS_WAR						0x51	// 81 - // 2008-04-01 by dhjin, ����, ������ ����â ��ȹ�� - 
//#define T1_FL_LOG_SP_WAR						0x52	// 82 - // 2008-04-01 by dhjin, ����, ������ ����â ��ȹ�� - 
//#define T1_FL_LOG_WAR_CONTRIBUTION				0x53	// 83 - // 2008-12-23 by dhjin, ���� ���� �߰���
//#define T1_FL_LOG_WAR_CONTRIBUTION_GEAR			0x54	// 84 - // 2008-12-23 by dhjin, ���� ���� �߰���

//
//#define T1_FL_LOG_SERVER_INTEGRAION							0x80	// 128 - ���� ���� ���� - ĳ���� ���� ó�� ��
//#define T1_FL_LOG_SERVER_INTEGRAION_GUILD_STORE_ITEMS		0x81	// 129 - ���� ���� ���� - ���� â�� ������ ���� ó����, // 2007-02-15 by cmkwon
//#define T1_FL_LOG_SERVER_INTEGRAION_ONLY_CASH_STORE_ITEMS	0x82	// 130 - ���� ���� ���� - ĳ�� �����۸� ���ռ����� DB �� �̵�, // 2008-02-28 by cmkwon, ���� ������ ���� ĳ�� �����۸� �̵��ϴ� ���ν���

//#define T1_FL_LOG_INFLWAR_START							0x90	// 144 - // 2007-10-16 by cmkwon, �α� �߰� - ������ �α�
//#define T1_FL_LOG_INFLWAR_END							0x91	// 145 - // 2007-10-16 by cmkwon, �α� �߰� - ������ �α�
//#define T1_FL_LOG_OUTPOSTWAR_RESET_START				0x95	// 149 - // 2007-10-16 by cmkwon, �α� �߰� - ���������� �α�
//#define T1_FL_LOG_OUTPOSTWAR_RESET_DESTROY				0x96	// 150 - // 2007-10-16 by cmkwon, �α� �߰� - ���������� �α�
//#define T1_FL_LOG_OUTPOSTWAR_RESET_COMPLETE				0x97	// 151 - // 2007-10-16 by cmkwon, �α� �߰� - ���������� �α�
//#define T1_FL_LOG_OUTPOSTWAR_PROTECTOR_DESTROY			0x98	// 152 - // 2007-10-16 by cmkwon, �α� �߰� - ���������� �α�
//#define T1_FL_LOG_OUTPOSTWAR_SET_NEXTTIME				0x99	// 153 - // 2007-10-16 by cmkwon, �α� �߰� - ���������� �α�

//#define T1_FL_LOG_POLL_VOTE								0xA0	// 160 - // 2007-11-08 by dhjin, ��ǥ �α� 
//#define T1_FL_LOG_POLL_DELETE_LEADERCANDIDATE			0xA1	// 161 - // 2007-11-08 by dhjin, ������ �⸶ Ż�� �α�

//#define T1_FL_LOG_DISMEMBER_GUILD						0xA2	// 162 - // 2007-11-09 by dhjin
//#define T1_FL_LOG_NOTIFY_MSG_DELETE						0xA3	// 163 - // 2007-11-28 by cmkwon, �����ý��� ���� -
//#define T1_FL_LOG_USE_COUPON							0xA4	// 164 - // 2008-01-23 by cmkwon, S_F, S_L: ���� ��� ���� �α׿� �߰� - ���� ���� �α�

//#define T1_FL_LOG_ITEM_GIFT_SEND						0xB0	// 176 - // 2007-11-13 by cmkwon, �����ϱ� ��� �߰� - �����ϱ� : �ֱ�
//#define T1_FL_LOG_ITEM_GIFT_RECV						0xB1	// 177 - // 2007-11-13 by cmkwon, �����ϱ� ��� �߰� - �����ϱ� : �ޱ�
//#define T1_FL_LOG_ITEM_REARRANGE_DELETE_ITEM			0xB2	// 178 - // 2007-11-29 by cmkwon, ī���ͺ������ �������� ���� �α� ����� - ������ ������
//#define T1_FL_LOG_ITEM_REARRANGE_ITEM					0xB3	// 179 - // 2007-11-29 by cmkwon, ī���ͺ������ �������� ���� �α� ����� - ������ ������
//#define T1_FL_LOG_ITEM_ATTACH_ITEM						0xB4	// 180 - // 2008-01-23 by cmkwon, S_F, S_L: ����/�������� ���� �α׿� �߰� - ����
//#define T1_FL_LOG_ITEM_DETACH_ITEM						0xB5	// 181 - // 2008-01-23 by cmkwon, S_F, S_L: ����/�������� ���� �α׿� �߰� - ��������
//#define T1_FL_LOG_ITEM_DELETE							0xB6	// 182 - // 2008-01-23 by cmkwon, S_F, S_L: ����/�������� ���� �α׿� �߰� - ������ ������(��ȿ�ð� ���)

//#define T1_FL_LOG_LUCKY_DROP							0xC0	// 192 - // 2009-03-31 by dhjin, ��Ű�ӽ� �α�
//#define T1_FL_LOG_INFINITY_START						0xC1	// 193 - // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� ���� ���� 
//#define T1_FL_LOG_INFINITY_START_ITEM					0xC2	// 194 - // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���۽� ������ ���� ����
//#define T1_FL_LOG_INFINITY_CINEMA						0xC3	// 195 - // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� �α�
//#define T1_FL_LOG_MONSTER_SKILL							0xC4	// 196 - // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� ��ų ����
//#define T1_FL_LOG_HPACTION_TALK							0xC5	// 197 - // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� ��ȭ ����
//#define T1_FL_LOG_INFINITY_FIN							0xC6	// 198 - // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� ���� ���� 
//#define T1_FL_LOG_INFINITY_FIN_ITEM						0xC7	// 199 - // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ����� ������ ���� ����
//#define T1_FL_LOG_INFINITY_FIN_ALIVE_KEY_MONSTER		0xC8	// 200 - // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ����� ����ִ� ���� ����
//#define T1_FL_LOG_INFINITY_LEAVE_ITEM					0xC9	// 201 - // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� Ż��� ������ ���� ����

//#define T1_FL_LOG_BLOCK_ACCOUNT							0xD0	// 208 - // 2008-01-30 by cmkwon, ���� �� �α� ����� ���� - ���� ��
//#define T1_FL_LOG_UNBLOCK_ACCOUNT						0xD1	// 209 - // 2008-01-30 by cmkwon, ���� �� �α� ����� ���� - ���� �� ����

//#define T1_FL_LOG_PET_LEVEL								0xD5	// 213 - // 2011-09-15 by hskim, ��Ʈ�� �ý��� 2�� - ��Ʈ�� ���� ����

//#define T1_FL_LOG_CONNECT_USER_NEW						0xD6	// 214 - // 2012-01-08 by hskim, GLog 2�� - ���� �ű� ����
//#define T1_FL_LOG_CONNECT_USER_LOGIN					0xD7	// 215 - // 2012-01-08 by hskim, GLog 2�� - ���� ����

//#define T1_FL_LOG_STATISTICS_MONEY						0xD8	// 216 - // 2012-01-16 by hskim, ��� - ȭ��

//#define T1_FL_LOG_FIXED_TERM_SHAPE_START				0xD9	// 217 - // 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)
//#define T1_FL_LOG_FIXED_TERM_SHAPE_END					0xDA	// 218 - // 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)

// 2013-1-02 by khkim, GLog
//#define T1_FL_LOG_ACCOUNTCONNECT						0xDB		 
//#define T1_FL_LOG_CHARDATA								0xDC	
//#define	T1_FL_LOG_ITEM									0xDD
//#define	T1_FL_LOG_ITEMSTATE								0xDE
//#define	T1_FL_LOG_SERVER								0xDF
// end 2013-1-02 by khkim, GLog


//#define T1_FL_LOG_ITEM_INITIALIZED_GUILD_STORE			0xE0	// 224 - // 2012-12-20 by bckim, �����ʱ�ȭ�� ���â��������� ����忡�� �Ű����� ������ �α�
//#define T1_FL_LOG_ITEM_GET_WITH_SAME_ACCOUNT			0xE1	// 225 - // 2012-12-27 by bckim, ���ϰ���, ������ �ɸ��Ͱ��� ������ �̵��� �α��߰�

//#define T1_FL_LOG_COLLECTION_STATE						0xE2	// 226 - // 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - GameLog �߰�
//#define T1_FL_LOG_COLLECTION_SHAPE_CHANGE				0xE3	// 227 - // 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - GameLog �߰�
//#define T1_FL_LOG_ACCOUNT_INFL_CHANGE_BY_ADMIN			0xE4	// 228 - // 2013-07-26 by jhseol, Ÿ ���� ���º��� - GameLog �߰�

//#define T1_FL_LOG_START_FIELD_SERVER			0xFF	// 255 - FieldServer Start



//#define T_FL_LOG_LOGIN					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_LOGIN)				// �α���
//#define T_FL_LOG_LOGOUT					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_LOGOUT)			// �α׾ƿ�
//#define T_FL_LOG_LEVEL					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_LEVEL)				// ���� ��ȭ
//#define T_FL_LOG_EXP					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_EXP)				// ����ġ ��ȭ
//#define T_FL_LOG_STAT					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_STAT)				// ���� ���
//#define T_FL_LOG_GAME_START				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_GAME_START)		// ���� ����
//#define T_FL_LOG_GAME_END				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_GAME_END)			// ���� ����
//#define T_FL_LOG_QUEST_COMPLETION		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_QUEST_COMPLETION)	// ����Ʈ �Ϸ�
//#define T_FL_LOG_QUEST_COMPENSATION		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_QUEST_COMPENSATION)// ����Ʈ ����
//#define T_FL_LOG_PKWIN					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_PKWIN)				// 1:1 ���� �¸�
//#define T_FL_LOG_PKLOSS					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_PKLOSS)			// 1:1 ���� �й�
//#define T_FL_LOG_DEAD					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_DEAD)				// ��ü ����
//#define T_FL_LOG_WARP					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_WARP)				// ����
//#define T_FL_LOG_SPEED_HACK				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_SPEED_HACK)		// ���ǵ� ��
//#define T_FL_LOG_CREATE_CHARACTER		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_CREATE_CHARACTER)	// ĳ���� ����
//#define T_FL_LOG_DELETE_CHARACTER		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_DELETE_CHARACTER)	// ĳ���� ����
//#define T_FL_LOG_MEMORY_HACK			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_MEMORY_HACK)		// 29 - �޸���, 2005-12-20 by cmkwon
//#define T_FL_LOG_PvEWIN					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_PvEWIN)			// 30 - ���������� ��� ���� ĳ���͸� ����, 2006-03-06 by cmkwon
//#define T_FL_LOG_PvELOSS				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_PvELOSS)			// 31 - ���������� ��� ���¿� ���� ����, 2006-03-06 by cmkwon

//#define T_FL_LOG_ITEM_CHARACTER_ITEM	(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_CHARACTER_ITEM)	// ��� ���� ������ ����
//#define T_FL_LOG_ITEM_GET_ITEM			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_GET_ITEM)		// Drop Item ����
//#define T_FL_LOG_ITEM_TRADE_SEND		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_TRADE_SEND)	// ������ �ŷ�: �ֱ�
//#define T_FL_LOG_ITEM_TRADE_RECV		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_TRADE_RECV)	// ������ �ŷ�: �ޱ�
//#define T_FL_LOG_ITEM_THROWAWAY_ITEM	(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_THROWAWAY_ITEM)	// ������ ������

//#define T_FL_LOG_ITEM_TRY_ENCHANT_INFO	(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_TRY_ENCHANT_INFO)	// 2013-07-01 by bckim, 10��æƮ �̸� ������ī��ó��, �α��߰�, ��� ����
//#define T_FL_LOG_ITEM_DISSOLUTION_ITEM	(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_DISSOLUTION_ITEM)	// 2013-08-05 by bckim, ���� �������� �α� �߰�

//#define T_FL_LOG_ITEM_BUY_ITEM			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_BUY_ITEM)		// ������ ����

//#define T_FL_LOG_ITEM_BUY_CASH_PRICE	(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_BUY_CASH_PRICE)	// 2012-10-16 by bckim, ĳ�������� �������� Log �߰�
//#define T_FL_LOG_ITEM_CHARACTER_ITEM_GUILDSTORE_RESULT	(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_CHARACTER_ITEM_GUILDSTORE_RESULT)	// 2012-12-17 by bckim, ���â�� ������ �̵� ���� �߰��α� 
//#define T_FL_LOG_ITEM_INITIALIZED_GUILD_STORE	(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_INITIALIZED_GUILD_STORE)	// 2012-12-20 by bckim, �����ʱ�ȭ�� ���â��������� ����忡�� �Ű����� ������ �α�
//#define T_FL_LOG_ITEM_GET_WITH_SAME_ACCOUNT		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_GET_WITH_SAME_ACCOUNT)	// 2012-12-27 by bckim, ���ϰ���, ������ �ɸ��Ͱ��� ������ �̵��� �α��߰�

//#define T_FL_LOG_ITEM_SELL_ITEM			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_SELL_ITEM)	// ������ �Ǹ�
//#define T_FL_LOG_ITEM_USE_ITEM			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_USE_ITEM)		// ������ ���
//#define T_FL_LOG_ITEM_USE_ENERGY		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_USE_ENERGY)	// �������� ������ ���
//#define T_FL_LOG_ITEM_USE_ENCHANT		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_USE_ENCHANT)	// ��æƮ�� ������ ���
//#define T_FL_LOG_ITEM_ADD_ITEM_BY_ADMIN	(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_ADD_ITEM_BY_ADMIN)		// ����� ���� ����
//#define T_FL_LOG_ITEM_DEL_ITEM_BY_ADMIN	(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_DEL_ITEM_BY_ADMIN)		// ����� ���� ����
//#define T_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND	(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND)	// ��ɾ ���� ����
//#define T_FL_LOG_ITEM_INSERT_TO_STORE	(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_INSERT_TO_STORE)	// â�� ����
//#define T_FL_LOG_ITEM_DRAW_FROM_STORE	(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_DRAW_FROM_STORE)	// â���� ������
//#define T_FL_LOG_ITEM_STORE_ITEM		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_STORE_ITEM)		// â�� ���� ������ ����, â�� ���� ������
//#define T_FL_LOG_ITEM_USE_MIX			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_USE_MIX)			// ����
//#define T_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN)	// ĳ������
//#define T_FL_LOG_ITEM_BAZAAR_SEND		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_BAZAAR_SEND)	// ���λ��� �ŷ�: �ֱ�
//#define T_FL_LOG_ITEM_BAZAAR_RECV		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_BAZAAR_RECV)	// ���λ��� �ŷ�: �ޱ�
//#define T_FL_LOG_ITEM_INSERT_TO_GUILDSTORE			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_INSERT_TO_GUILDSTORE)		// 2006-09-27 by dhjin, ���� â�� �α� �����
//#define T_FL_LOG_ITEM_DRAW_FROM_GUILDSTORE			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_DRAW_FROM_GUILDSTORE)		// 2006-09-27 by dhjin, ���� â�� �α� �����
//#define T_FL_LOG_ITEM_SPI							(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_SPI)							// 55 - SPI Log, 2006-10-27 by cmkwon
//#define T_FL_LOG_ITEM_GET_ITEM_BY_ITEMEVENT			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_GET_ITEM_BY_ITEMEVENT)		// 56 - ItemEvent�� ���� ������ �߰�, 2007-01-11 by cmkwon
//#define T_FL_LOG_WARPOINT							(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_WARPOINT)						// 58 - WarPoint ����, 2007-05-15 by dhjin
//#define T_FL_LOG_ARENA_TEAM_MATCH					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ARENA_TEAM_MATCH)				// 59 - Arena �� ��Ī, 2007-06-11 by dhjin
//#define T_FL_LOG_TUTORIAL_COMPLETION				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_TUTORIAL_COMPLETION)			// 60 - Tutorial, 2007-07-06 by dhjin
//#define T_FL_LOG_OUTPOST_START			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_OUTPOST_START)				// 61 - OutPost���� �α�, 2007-10-02 by dhjin
//#define T_FL_LOG_OUTPOST_END			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_OUTPOST_END)				// 62 - OutPost���� �α�, 2007-10-02 by dhjin
//#define T_FL_LOG_EXPEDIENCYFUND			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_EXPEDIENCYFUND)			// 63 - �ǰ��� ��/�� �α�, 2007-10-02 by dhjin

//#define T_FL_LOG_SERVER_INFO_MAP		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_SERVER_INFO_MAP)
//#define T_FL_LOG_SERVER_INFO_TOTAL		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_SERVER_INFO_TOTAL)
//#define T_FL_LOG_HACKING_LOG			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_HACKING_LOG)

//#define T_FL_LOG_MONSTER_BOSS			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_MONSTER_BOSS)			// ������ ����
//#define T_FL_LOG_MS_WAR					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_MS_WAR)			// 81 - // 2008-04-01 by dhjin, ����, ������ ����â ��ȹ�� - 
//#define T_FL_LOG_SP_WAR					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_SP_WAR)			// 82 - // 2008-04-01 by dhjin, ����, ������ ����â ��ȹ�� - 
//#define T_FL_LOG_WAR_CONTRIBUTION		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_WAR_CONTRIBUTION)			// 83  - // 2008-12-23 by dhjin, ���� ���� �߰���
//#define T_FL_LOG_WAR_CONTRIBUTION_GEAR	(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_WAR_CONTRIBUTION_GEAR)		// 84  - // 2008-12-23 by dhjin, ���� ���� �߰���

//#define T_FL_LOG_INFLWAR_START					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_INFLWAR_START)					// 144 - // 2007-10-16 by cmkwon, �α� �߰� - ������ �α�
//#define T_FL_LOG_INFLWAR_END					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_INFLWAR_END)					// 145 - // 2007-10-16 by cmkwon, �α� �߰� - ������ �α�
//#define T_FL_LOG_OUTPOSTWAR_RESET_START			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_OUTPOSTWAR_RESET_START)		// 151 - // 2007-10-16 by cmkwon, �α� �߰� - ���������� �α�
//#define T_FL_LOG_OUTPOSTWAR_RESET_DESTROY		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_OUTPOSTWAR_RESET_DESTROY)		// 154 - // 2007-10-16 by cmkwon, �α� �߰� - ���������� �α�
//#define T_FL_LOG_OUTPOSTWAR_RESET_COMPLETE		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_OUTPOSTWAR_RESET_COMPLETE)		// 152 - // 2007-10-16 by cmkwon, �α� �߰� - ���������� �α�
//#define T_FL_LOG_OUTPOSTWAR_PROTECTOR_DESTROY	(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_OUTPOSTWAR_PROTECTOR_DESTROY)	// 153 - // 2007-10-16 by cmkwon, �α� �߰� - ���������� �α�
//#define T_FL_LOG_OUTPOSTWAR_SET_NEXTTIME		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_OUTPOSTWAR_SET_NEXTTIME)		// 156 - // 2007-10-16 by cmkwon, �α� �߰� - ���������� �α�

//#define T_FL_LOG_POLL_VOTE						(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_POLL_VOTE)						// 160 - // 2007-11-08 by dhjin, ��ǥ �α�
//#define T_FL_LOG_POLL_DELETE_LEADERCANDIDATE	(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_POLL_DELETE_LEADERCANDIDATE)	// 161 - // 2007-11-08 by dhjin, ������ �⸶ Ż�� �α�

//#define T_FL_LOG_DISMEMBER_GUILD				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_DISMEMBER_GUILD)		// 162 - // 2007-11-09 by dhjin, ���� ��ü�α�
//#define T_FL_LOG_NOTIFY_MSG_DELETE				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_NOTIFY_MSG_DELETE)		// 163 - // 2007-11-28 by cmkwon, �����ý��� ���� -
//#define T_FL_LOG_USE_COUPON						(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_USE_COUPON)			// 164 - // 2008-01-23 by cmkwon, S_F, S_L: ���� ��� ���� �α׿� �߰� - ���� ���� �α�

//#define T_FL_LOG_ITEM_ATTACH_ITEM				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_ATTACH_ITEM)		// 180 - // 2008-01-23 by cmkwon, S_F, S_L: ����/�������� ���� �α׿� �߰� - ����
//#define T_FL_LOG_ITEM_DETACH_ITEM				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_DETACH_ITEM)		// 181 - // 2008-01-23 by cmkwon, S_F, S_L: ����/�������� ���� �α׿� �߰� - ��������
//#define T_FL_LOG_ITEM_DELETE					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_DELETE)			// 182 - // 2008-01-23 by cmkwon, S_F, S_L: ����/�������� ���� �α׿� �߰� - ������

//#define T_FL_LOG_LUCKY_DROP						(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_LUCKY_DROP)			// 192 - // 2009-03-31 by dhjin, ��Ű�ӽ� �α�

//#define T_FL_LOG_INFINITY_START					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_INFINITY_START)			// 193 - // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� ���� ���� 
//#define T_FL_LOG_INFINITY_START_ITEM			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_INFINITY_START_ITEM)		// 194 - // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���۽� ������ ���� ����
//#define T_FL_LOG_INFINITY_CINEMA				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_INFINITY_CINEMA)			// 195 - // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� �α�
//#define T_FL_LOG_MONSTER_SKILL					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_MONSTER_SKILL)				// 196 - // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� ��ų ����
//#define T_FL_LOG_HPACTION_TALK					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_HPACTION_TALK)				// 197 - // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� ��ȭ ����
//#define T_FL_LOG_INFINITY_FIN					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_INFINITY_FIN)				// 198 - // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� ���� ���� 
//#define T_FL_LOG_INFINITY_FIN_ITEM				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_INFINITY_FIN_ITEM)			// 199 - // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ����� ������ ���� ����
//#define T_FL_LOG_INFINITY_FIN_ALIVE_KEY_MONSTER		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_INFINITY_FIN_ALIVE_KEY_MONSTER)	// 200 - // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ����� ����ִ� ���� ����
//#define T_FL_LOG_INFINITY_LEAVE_ITEM			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_INFINITY_LEAVE_ITEM)		// 201 - // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� Ż��� ������ ���� ����

//#define T_FL_LOG_PET_LEVEL						(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_PET_LEVEL)					// 213 - // 2011-09-15 by hskim, ��Ʈ�� �ý��� 2�� - ��Ʈ�� ���� ����

//#define T_FL_LOG_CONNECT_USER_NEW				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_CONNECT_USER_NEW)			// 214 - // 2012-01-08 by hskim, GLog 2�� - ���� �ű� ����
//#define T_FL_LOG_CONNECT_USER_LOGIN				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_CONNECT_USER_LOGIN)		// 215 - // 2012-01-08 by hskim, GLog 2�� - ���� ����

//#define T_FL_LOG_STATISTICS_MONEY				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_STATISTICS_MONEY)			// 216 - // 2012-01-16 by hskim, ��� - ȭ��

//#define T_FL_LOG_FIXED_TERM_SHAPE_START			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_FIXED_TERM_SHAPE_START)	// 217 - // 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)
//#define T_FL_LOG_FIXED_TERM_SHAPE_END			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_FIXED_TERM_SHAPE_END)		// 218 - // 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)

//#define T_FL_LOG_START_FIELD_SERVER				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_START_FIELD_SERVER)		// FieldServer Start

// start 2012-10-08 by khkim, GLog
//#define T_FL_LOG_ACCOUNTCONNECT					(MessageType_t)((T0_FL_LOG<<8)| T1_FL_LOG_ACCOUNTCONNECT)			// 218		2012-10-08 by khkim, GLog
//#define T_FL_LOG_CHARDATA						(MessageType_t)((T0_FL_LOG<<8)| T1_FL_LOG_CHARDATA)					// 219		
//#define T_FL_LOG_ITEM							(MessageType_t)((T0_FL_LOG<<8)| T1_FL_LOG_ITEM)						// 220
//#define T_FL_LOG_ITEMSTATE						(MessageType_t)((T0_FL_LOG<<8)| T1_FL_LOG_ITEMSTATE)				// 221	
//#define T_FL_LOG_SERVER							(MessageType_t)((T0_FL_LOG<<8)| T1_FL_LOG_SERVER)					// 222
// end 2012-10-08 by khkim, GLog

//#define T_FL_LOG_COLLECTION_STATE				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_COLLECTION_STATE)			// 226		// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - GameLog �߰�
//#define T_FL_LOG_COLLECTION_SHAPE_CHANGE		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_COLLECTION_SHAPE_CHANGE)	// 227		// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - GameLog �߰�
//#define T_FL_LOG_ACCOUNT_INFL_CHANGE_BY_ADMIN		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ACCOUNT_INFL_CHANGE_BY_ADMIN)	// 228 - // 2013-07-26 by jhseol, Ÿ ���� ���º��� - GameLog �߰�

////////////////////////////////
// CONNECTION LOG(0x00~)
struct MSG_FL_LOG_LOGIN
{
	UID32_t	AccountUniqueNumber;
	CHAR	AccountName[SIZE_MAX_ACCOUNT_NAME];
	BYTE	IPAddress[4];
	CHAR	ServerGroupName[SIZE_MAX_SERVER_NAME];
	char	MACAddress[SIZE_MAX_MAC_ADDRESS];	// 2015-09-23 Future, added logging of MAC Address
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%u|%s|%u.%u.%u.%u|%s %s\r\n", GetGameLogTypeString(i_nLogType), AccountUniqueNumber, AccountName
			, IPAddress[0], IPAddress[1], IPAddress[2], IPAddress[3], ServerGroupName, MACAddress);
		return o_szLogString;
	}
};					// �α���

struct MSG_FL_LOG_LOGOUT
{
	UID32_t	AccountUniqueNumber;
	CHAR	AccountName[SIZE_MAX_ACCOUNT_NAME];
	BYTE	IPAddress[4];
	CHAR	ServerGroupName[SIZE_MAX_SERVER_NAME];
	char	MACAddress[SIZE_MAX_MAC_ADDRESS];	// 2015-09-23 Future, added logging of MAC Address
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%u|%s|%u.%u.%u.%u|%s %s\r\n", GetGameLogTypeString(i_nLogType), AccountUniqueNumber, AccountName
			, IPAddress[0], IPAddress[1], IPAddress[2], IPAddress[3], ServerGroupName, MACAddress);
		return o_szLogString;
	}
};				// �α׾ƿ�

				////////////////////////////////
				// USER LOG(0x10~)
struct FL_USER_LOG_BASE
{
	UID32_t		CharacterUniqueNumber;
	MapIndex_t	CurrentMapIndex;
	ChannelIndex_t	CurrentChannIdx;		// 2007-11-30 by cmkwon, �����ʿ� ä���� �ٸ� ��쵵 ���� ó�� - FL_USER_LOG_BASE �� �ʵ� �߰�
	AVECTOR3	CurrentPosition;
	char *GetLogBaseString(char *o_szLogString)
	{
		// 2007-11-30 by cmkwon, �����ʿ� ä���� �ٸ� ��쵵 ���� ó�� - ���� �α� ���� ����
		sprintf(o_szLogString, "%d|%d|%d|(%d, %d, %d)", CharacterUniqueNumber, CurrentMapIndex, CurrentChannIdx, CurrentPosition.x, CurrentPosition.y, CurrentPosition.z);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_LEVEL : public FL_USER_LOG_BASE
{
	BYTE		FromLevel;
	BYTE		ToLevel;
	LONGLONG	TotalPlayTime;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%d|%d|%I64d\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), FromLevel, ToLevel, TotalPlayTime);
		return o_szLogString;
	}
};					// ���� ��ȭ

struct MSG_FL_LOG_EXP : public FL_USER_LOG_BASE
{
	float		AcquiredExperience;	// ���� ����ġ
	LONGLONG	CharacterExperiece;	// ���� ����ġ, Experience_t�̳� DB���� BIGINT�� ����
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%.0f|%I64d\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), AcquiredExperience, CharacterExperiece);
		return o_szLogString;
	}
};					// ����ġ ��ȭ

struct MSG_FL_LOG_STAT : public FL_USER_LOG_BASE
{
	BYTE	StatKind;				// STAT_XXX, ������ ����
	BYTE	RemainedBonusStat;		// ���� ���� ���ʽ� ����
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%d|%d|0\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), StatKind, RemainedBonusStat);
		return o_szLogString;
	}
};					// ���� ���

struct MSG_FL_LOG_GAME_START : public FL_USER_LOG_BASE
{
	CHAR	CharacterName[SIZE_MAX_CHARACTER_NAME];
	UID32_t	AccountUniqueNumber;
	CHAR	AccountName[SIZE_MAX_ACCOUNT_NAME];
	LONGLONG	TotalPlayTime;		// �� ���� �ð�
	UID32_t		PCBangUID;			// 2007-01-22 by dhjin, ���� PCBang UID
	INT			GameContinueTimeOfToday;	// 2007-11-07 by cmkwon, ���ð������ӽð� ���ӷα׿� �߰� - �ʵ� �߰�
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		// 2007-11-07 by cmkwon, ���ð������ӽð� ���ӷα׿� �߰� - GameContinueTimeOfToday �߰�
		sprintf(o_szLogString, "%s|%s|%s|%u|%s|0|%I64d|%u|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), CharacterName
			, AccountUniqueNumber, AccountName, TotalPlayTime, PCBangUID, GameContinueTimeOfToday);
		return o_szLogString;
	}
};			// ���� ����

struct MSG_FL_LOG_GAME_END : public FL_USER_LOG_BASE
{
	CHAR	CharacterName[SIZE_MAX_CHARACTER_NAME];
	UID32_t	AccountUniqueNumber;
	CHAR	AccountName[SIZE_MAX_ACCOUNT_NAME];
	UINT	PlayTime;				// ���� ���� ���� �� ���� �ð�
	LONGLONG	TotalPlayTime;		// �� ���� �ð�
	UINT		PCBangPlayTime;		// 2006-12-18 by cmkwon, ���� PCBang���� ���ӽð�
	UID32_t		PCBangUID;			// 2007-01-22 by dhjin, ���� PCBang UID
	INT			GameContinueTimeOfToday;	// 2007-11-07 by cmkwon, ���ð������ӽð� ���ӷα׿� �߰� - �ʵ� �߰�
	ATUM_DATE_TIME	LoginDate;								// 2010-06-01 by shcho, GLogDB ���� -		
	CHAR			CharacterRace[SIZE_MAX_GLOG_CHARACTER_RACE_NAME];		// 2010-06-01 by shcho, GLogDB ���� -
	CHAR			CharacterClass[SIZE_MAX_GLOG_CHARACTER_CLASS_NAME];		// 2010-06-01 by shcho, GLogDB ���� -
	Experience_t	PlayExp;											// 2010-06-01 by shcho, GLogDB ���� -
	Experience_t	TotalExp;											// 2010-06-01 by shcho, GLogDB ���� -
	Lv_t			CharacterLevel;										// 2010-06-01 by shcho, GLogDB ���� -
	CHAR			MostStayedInZoneName[SIZE_MAX_MAP_NAME];			// 2010-06-01 by shcho, GLogDB ���� -	

	UID32_t			CharacterUniqueNumber;								// 2011-12-12 by hskim, GLog 2��
	USHORT			Race;												// 2011-12-12 by hskim, GLog 2��
	USHORT			UnitKind;											// 2011-12-12 by hskim, GLog 2��
	BYTE			InfluenceType;										// 2011-12-12 by hskim, GLog 2��

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		// 2007-11-07 by cmkwon, ���ð������ӽð� ���ӷα׿� �߰� - GameContinueTimeOfToday �߰�
		sprintf(o_szLogString, "%s|%s|%s|%u|%s|%u|%I64d|%u|%u|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), CharacterName
			, AccountUniqueNumber, AccountName, PlayTime, TotalPlayTime, PCBangPlayTime, PCBangUID, GameContinueTimeOfToday);
		return o_szLogString;
	}
};				// ���� ����

struct MSG_FL_LOG_QUEST_COMPLETION : public FL_USER_LOG_BASE
{
	INT		QuestIndex;				// ����Ʈ �ε���
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), QuestIndex);
		return o_szLogString;
	}
};		// ����Ʈ �Ϸ�


struct MSG_FL_LOG_QUEST_COMPENSATION : public FL_USER_LOG_BASE		// 2007-04-18 by cmkwon
{
	INT					QuestIndex;				// 2007-04-18 by cmkwon
	INT					ExpOfCompensation;
	INT					BonusStatOfCompensation;
	INT					LenOfCompensation;		// ���� ������ �� (����ġ, SPI����)
	ARRAY_(char);								// ���� ������
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString, char *i_szCompensationItemList)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%d|%d|%d|%s\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), QuestIndex, ExpOfCompensation, BonusStatOfCompensation, i_szCompensationItemList);
		return o_szLogString;
	}
};	// 2007-01-16 by dhjin, ���� ������ �α�


struct MSG_FL_LOG_PKWIN : public FL_USER_LOG_BASE
{
	UID32_t	PeerCharacterUniqueNumber;	// ��� ĳ����
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%u\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), PeerCharacterUniqueNumber);
		return o_szLogString;
	}
};						// 1:1 ���� �¸�

struct MSG_FL_LOG_PKLOSS : public FL_USER_LOG_BASE
{
	UID32_t	PeerCharacterUniqueNumber;	// ��� ĳ����
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%u\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), PeerCharacterUniqueNumber);
		return o_szLogString;
	}
};					// 1:1 ���� �й�

					// DAMAGE_BY_XXX
#define DAMAGE_BY_COLLISION		((BYTE)0)
#define DAMAGE_BY_MONSTER		((BYTE)1)
#define DAMAGE_BY_PK			((BYTE)2)
#define DAMAGE_BY_FUEL_ALLIN	((BYTE)3)
#define DAMAGE_BY_NA			((BYTE)10)	// �� �� ���ų� ���ʿ��� ���

struct MSG_FL_LOG_DEAD : public FL_USER_LOG_BASE
{
	INT			DamageKind;			// DAMAGE_BY_XXX, ���� ������ ����(PK, �浹, ���� ���� ��)
	ItemNum_t	ItemNum;			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� ������ ����
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%d|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), DamageKind, ItemNum);
		return o_szLogString;
	}
};					// ��ü ����

struct MSG_FL_LOG_WARP : public FL_USER_LOG_BASE
{
	MapIndex_t	TargetMapIndex;		// ������ ��
	ChannelIndex_t	TargetChannIdx;		// 2007-11-30 by cmkwon, �����ʿ� ä���� �ٸ� ��쵵 ���� ó�� - MSG_FL_LOG_WARP �� �ʵ� �߰�
	AVECTOR3	TargetPosition;		// ������ ���� ��ġ
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		// 2007-11-30 by cmkwon, �����ʿ� ä���� �ٸ� ��쵵 ���� ó�� - ���� �α� ����
		sprintf(o_szLogString, "%s|%s|%d|%d|(%d,%d,%d)\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), TargetMapIndex, TargetChannIdx, TargetPosition.x, TargetPosition.y, TargetPosition.z);
		return o_szLogString;
	}
};					// ����, ���� ���� �� �α� ����

enum SPEEDHACK_CHECK_TYPE
{
	SPEEDHACK_CHECK_TYPE_BY_SERVER = 0,
	SPEEDHACK_CHECK_TYPE_BY_CLIENT = 1
};

struct MSG_FL_LOG_SPEED_HACK : public FL_USER_LOG_BASE
{
	BYTE	CheckType;
	INT 	ReceivedMoveCounts;
	INT		EngineItemNum;
	INT		ServerPastTime;
	INT		ClientPastTime;
	INT		PastDistance;
	INT		CurrentSpeed;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%d|%d|%d|%d|%d|%d|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), CheckType, ReceivedMoveCounts
			, EngineItemNum, ServerPastTime, ClientPastTime, PastDistance, CurrentSpeed);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_CREATE_CHARACTER : public FL_USER_LOG_BASE
{
	CHAR	CharacterName[SIZE_MAX_CHARACTER_NAME];
	UID32_t	AccountUniqueNumber;
	CHAR	AccountName[SIZE_MAX_ACCOUNT_NAME];
	USHORT	UnitKind;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%s|%u|%s|%d|1\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), CharacterName, AccountUniqueNumber
			, AccountName, UnitKind);
		return o_szLogString;
	}
};		// ĳ���� ����

struct MSG_FL_LOG_DELETE_CHARACTER : public FL_USER_LOG_BASE
{
	CHAR	CharacterName[SIZE_MAX_CHARACTER_NAME];
	UID32_t	AccountUniqueNumber;
	CHAR	AccountName[SIZE_MAX_ACCOUNT_NAME];
	USHORT	UnitKind;
	USHORT	Level;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%s|%u|%s|%d|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), CharacterName, AccountUniqueNumber
			, AccountName, UnitKind, Level);
		return o_szLogString;
	}
};		// ĳ���� ����

struct MSG_FL_LOG_MEMORY_HACK : public FL_USER_LOG_BASE		// 2005-12-20 by cmkwon
{
	int		nItemKind;			// üũ ������ ����(1������, 2������, ����, �Ƹ�)
	int		nItemNum0;			// ItemNum
	UID64_t	uid64ItemUID;		// Item UniqueNumber
	int		nMemHackCheckType;	// üũ Ÿ��(CHECK_TYPE_XXXXXX)
	float	fValidValue;		// ��ȿ�� ��
	float	fCurrentValue;		// ���� ��
	int		nParam1;			// ü??Ÿ�Կ� ���� �ǹ̰� �ٸ���
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%d|%d|%I64d|%d|%5.2f|%5.2f|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), nItemKind, nItemNum0
			, uid64ItemUID, nMemHackCheckType, fValidValue, fCurrentValue, nParam1);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_PvEWIN : public FL_USER_LOG_BASE		// 2006-03-06 by cmkwon
{
	UID32_t	PeerCharacterUniqueNumber;	// ��� ĳ����
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%u\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), PeerCharacterUniqueNumber);
		return o_szLogString;
	}
};						// 1:1 ���� �¸�

struct MSG_FL_LOG_PvELOSS : public FL_USER_LOG_BASE		// 2006-03-06 by cmkwon
{
	UID32_t	PeerCharacterUniqueNumber;	// ��� ĳ����
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%u\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), PeerCharacterUniqueNumber);
		return o_szLogString;
	}
};					// 1:1 ���� �й�


					///////////////////////////////////////////////////////////////////////////////
					// ITEM LOG(0x20~)
struct FL_ITEM_LOG_BASE
{
	UID32_t		CharacterUniqueNumber;
	MapIndex_t	CurrentMapIndex;
	char *GetLogItemBaseString(char *o_szLogString)
	{
		sprintf(o_szLogString, "%u|%d", CharacterUniqueNumber, CurrentMapIndex);
		return o_szLogString;
	}
};

struct ITEM_FOR_LOG
{
	UID64_t		ItemUniqueNumber;
	INT			ItemNum;
	INT			CurrentCount;
	char *GetLogItemString(char *o_szLogString)
	{
		sprintf(o_szLogString, "%I64d|%d|%d", ItemUniqueNumber, ItemNum, CurrentCount);
		return o_szLogString;
	}

#ifdef _ATUM_SERVER
	// operator overloading
	ITEM_FOR_LOG& operator=(const ITEM_GENERAL& rhs)
	{
		this->ItemUniqueNumber = rhs.UniqueNumber;
		this->ItemNum = rhs.ItemNum;
		this->CurrentCount = rhs.CurrentCount;

		return *this;
	}
#endif // _ATUM_SERVER
};

struct MSG_FL_LOG_ITEM_CHARACTER_ITEM : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	INT				ChangeCount;	// ���� ����, 			// 2008-02-15 by cmkwon, �κ�<->â�� �̵� �α� ���� - MSG_FL_LOG_ITEM_STORE_ITEM �� �ʵ� �߰�
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		// 2008-02-15 by cmkwon, �κ�<->â�� �̵� �α� ���� - 
		//		sprintf(o_szLogString, "%s|%s|%s|-1\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem));
		sprintf(o_szLogString, "%s|%s|%s|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem), ChangeCount);
		return o_szLogString;
	}
};	// 2008-02-15 by cmkwon, �κ�<->â�� �̵� �α� ���� - â��-->�κ� ���� ī���ͺ� ������ �̵��� �κ��� �ش� ������ �߰� �� ���� ����� ����, // (���� ���� �� �)��� ���� ������ ����


	// 2012-12-17 by bckim, ���â�� ������ �̵� ���� �߰��α� 
struct MSG_FL_LOG_ITEM_CHARACTER_ITEM_GUILDSTORE_RESULT : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	INT				ChangeCount;
	UID32_t			GuildUID;	/// ��� UID
	CHAR			GuildName[SIZE_MAX_GUILD_NAME];	/// ��� �̸� 

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%d|%s|%s|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), GuildUID, GuildName, Item4Log.GetLogItemString(szItem), ChangeCount);
		return o_szLogString;
	}
};
// 2012-12-17 by bckim, ���â�� ������ �̵� ���� �߰��α�.End



struct MSG_FL_LOG_ITEM_GET_ITEM : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	UID32_t			StoreCharacterUID;			// 2006-11-01 by cmkwon
	INT				ChangeCount;
	BYTE			ItemUpdateType;				// 2008-09-26 by cmkwon, ���ս� GameLog ���� - IUT_XXX
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		// 2008-09-26 by cmkwon, ���ս� GameLog ���� - 
		//sprintf(o_szLogString, "%s|%s|%d|%s|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), StoreCharacterUID, Item4Log.GetLogItemString(szItem), ChangeCount);
		sprintf(o_szLogString, "%s|%s|%d|%s|%d|%s\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), StoreCharacterUID, Item4Log.GetLogItemString(szItem), ChangeCount, GetItemUpdateTypeString(ItemUpdateType));
		return o_szLogString;
	}
};			// Drop Item ����

			// 2013-08-05 by bckim, ���� �������� �α� �߰�
struct MSG_FL_LOG_ITEM_DISSOLUTION_ITEM : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	INT				ChangeCount;
	BYTE			ItemUpdateType;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%s|%d|%s\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem), ChangeCount, GetItemUpdateTypeString(ItemUpdateType));
		return o_szLogString;
	}
};
// End. 2013-08-05 by bckim, ���� �������� �α� �߰�

struct MSG_FL_LOG_ITEM_GET_ITEM_2 : public FL_ITEM_LOG_BASE		// 2008-09-26 by cmkwon, ���ս� GameLog ���� - ���� �߰���
{
	ITEM_FOR_LOG	Item4Log;
	UID32_t			StoreCharacterUID;			// 2006-11-01 by cmkwon
	INT				ChangeCount;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%d|%s|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), StoreCharacterUID, Item4Log.GetLogItemString(szItem), ChangeCount);
		return o_szLogString;
	}
};

// 2008-09-26 by cmkwon, ���ս� GameLog ���� - �Ʒ��� ���� MSG_FL_LOG_ITEM_GET_ITEM_2 �� ������ ��.
//typedef MSG_FL_LOG_ITEM_GET_ITEM		MSG_FL_LOG_ITEM_ADD_ITEM_BY_ADMIN;		// ����� ���� ����
//typedef MSG_FL_LOG_ITEM_GET_ITEM		MSG_FL_LOG_ITEM_DEL_ITEM_BY_ADMIN;		// ����� ���� ����
//typedef MSG_FL_LOG_ITEM_GET_ITEM		MSG_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND;	// ��ɾ ���� ����
//typedef MSG_FL_LOG_ITEM_GET_ITEM		MSG_FL_LOG_ITEM_INSERT_TO_STORE;		// â�� ����
//typedef MSG_FL_LOG_ITEM_GET_ITEM		MSG_FL_LOG_ITEM_DRAW_FROM_STORE;		// â���� ������
using MSG_FL_LOG_ITEM_ADD_ITEM_BY_ADMIN = MSG_FL_LOG_ITEM_GET_ITEM_2;		// ����� ���� ����
using MSG_FL_LOG_ITEM_DEL_ITEM_BY_ADMIN = MSG_FL_LOG_ITEM_GET_ITEM_2;		// ����� ���� ����
using MSG_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND = MSG_FL_LOG_ITEM_GET_ITEM_2;	// ��ɾ ���� ����
using MSG_FL_LOG_ITEM_INSERT_TO_STORE = MSG_FL_LOG_ITEM_GET_ITEM_2;		// â�� ����
using MSG_FL_LOG_ITEM_DRAW_FROM_STORE = MSG_FL_LOG_ITEM_GET_ITEM_2;		// â���� ������

using MSG_FL_LOG_ITEM_GET_WITH_SAME_ACCOUNT = MSG_FL_LOG_ITEM_GET_ITEM_2;		// 2012-12-27 by bckim, ���ϰ���, ������ �ɸ��Ͱ��� ������ �̵��� �α��߰�

struct MSG_FL_LOG_ITEM_TRADE_SEND : public FL_ITEM_LOG_BASE
{
	UID32_t			PeerCharacterUniqueNumber;	// �ŷ� ���
	char			PeerCharacterName[SIZE_MAX_CHARACTER_NAME];	// �ŷ� ���
	ITEM_FOR_LOG	Item4Log;					// �ŷ� ������
	INT				ChangeCount;				// �ŷ� ����
	BYTE			bGiftItem;					// 2007-11-13 by cmkwon, �����ϱ� ��� �߰� - �����ϱ� �α� �÷��� ����
	BYTE			bRearrangeItem;				// 2007-11-29 by cmkwon, ī���ͺ������ �������� ���� �α� ����� - 
	BYTE			bStoreItem;					// 2007-11-29 by cmkwon, ī���ͺ������ �������� ���� �α� ����� - FALSE �̸� �κ�, FALSE �� �ƴϸ� ĳ����â��
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		// 2007-11-13 by cmkwon, �����ϱ� ��� �߰� - �����ϱ� �α� �÷��� ����
		// 2007-11-29 by cmkwon, ī���ͺ������ �������� ���� �α� ����� - 
		sprintf(o_szLogString, "%s|%s|%u|%s|%s|%d|%d|%d|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), PeerCharacterUniqueNumber
			, PeerCharacterName, Item4Log.GetLogItemString(szItem), ChangeCount, bGiftItem, bRearrangeItem, bStoreItem);
		return o_szLogString;
	}
};					// ������ �ŷ�: �ֱ�

struct MSG_FL_LOG_ITEM_TRADE_RECV : public FL_ITEM_LOG_BASE
{
	UID32_t			PeerCharacterUniqueNumber;	// �ŷ� ���
	char			PeerCharacterName[SIZE_MAX_CHARACTER_NAME];	// �ŷ� ���
	ITEM_FOR_LOG	Item4Log;					// ������
	INT				ChangeCount;				// �ŷ� ����
	BYTE			bGiftItem;					// 2007-11-13 by cmkwon, �����ϱ� ��� �߰� - �����ϱ� �α� �÷��� ����
	BYTE			bRearrangeItem;				// 2007-11-29 by cmkwon, ī���ͺ������ �������� ���� �α� ����� - 
	BYTE			bStoreItem;					// 2007-11-29 by cmkwon, ī���ͺ������ �������� ���� �α� ����� - FALSE �̸� �κ�, FALSE �� �ƴϸ� ĳ����â��
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		// 2007-11-13 by cmkwon, �����ϱ� ��� �߰� - �����ϱ� �α� �÷��� ����
		// 2007-11-29 by cmkwon, ī���ͺ������ �������� ���� �α� ����� - 
		sprintf(o_szLogString, "%s|%s|%u|%s|%s|%d|%d|%d|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), PeerCharacterUniqueNumber
			, PeerCharacterName, Item4Log.GetLogItemString(szItem), ChangeCount, bGiftItem, bRearrangeItem, bStoreItem);
		return o_szLogString;
	}
};					// ������ �ŷ�: �ޱ�

					// 2007-06-05 by cmkwon, ��������/��æƮ������ �����ϱ�
struct MSG_FL_LOG_ITEM_THROWAWAY_ITEM : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	INT				ChangeCount;	// �ŷ� ����
	INT				PrefixCodeNum;			// 2007-06-05 by cmkwon
	INT				SuffixCodeNum;			// 2007-06-05 by cmkwon
	INT				EnchatItemNumCount;		// 2007-06-05 by cmkwon, EnchantItemNum List Count
	_ARRAY(INT);							// 2007-06-05 by cmkwon, EnchantItemNum List
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%s|%d|%d|%d", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem), ChangeCount
			, PrefixCodeNum, SuffixCodeNum);
		return o_szLogString;
	}
};	// ������ ������

	// 2013-07-01 by bckim, 10��æƮ �̸� ������ī��ó��, �α��߰�, ��� ����
struct MSG_FL_LOG_ITEM_TRY_ENCHANT_INFO : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	INT				Success;
	INT				PrefixCodeNum;
	INT				SuffixCodeNum;
	INT				EnchantCount;
	_ARRAY(INT);
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%s|%s|%d|%d|%d", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem),
			GetTryEnchantSuccessTypeString(Success), EnchantCount, PrefixCodeNum, SuffixCodeNum);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_ITEM_TRY_ENCHANT_INFO_STRING
{
	MSG_FL_LOG_ITEM_TRY_ENCHANT_INFO TryEnchantInfo;
	char szCardList[1024];
};
// End. 2013-07-01 by bckim, 10��æƮ �̸� ������ī��ó��, �α��߰�, ��� ����


struct MSG_FL_LOG_ITEM_BUY_ITEM : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	INT				ChangeCount;	// �ŷ� ����
	INT				RemainedMoney;	// ���� ��
	bool			IsCashItem;		// 2006-06-21 by cmkwon, ���� ������
	INT				nRemainCountForLimitedEdition;		// 2010-01-26 by cmkwon, ĳ�� ������ �����Ǹ� �ý��� ���� - 
	INT				CashItemPrice;							// 2010-06-01 by shcho, GLogDB ���� -
	CHAR			AccountName[SIZE_MAX_ACCOUNT_NAME];		// 2010-06-01 by shcho, GLogDB ���� -
	CHAR			CharacterName[SIZE_MAX_CHARACTER_NAME]; // 2010-06-01 by shcho, GLogDB ���� -
	INT				CharacterLevel;							// 2010-06-01 by shcho, GLogDB ���� - GLogDB�ʵ尡 int ���̹Ƿ� Int�� �޴´�.
	CHAR			ItemName[SIZE_MAX_ITEM_NAME];			// 2010-06-01 by shcho, GLogDB ���� -

	bool			GLog;									// 2011-12-12 by hskim, GLog 2��
	UID32_t			AccountUniqueNumber;					// 2011-12-12 by hskim, GLog 2��
	INT				ItemNum;								// 2011-12-12 by hskim, GLog 2��

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%s|%d|%d|%d|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem)
			, ChangeCount, RemainedMoney, IsCashItem, nRemainCountForLimitedEdition);
		return o_szLogString;
	}
};			// ������ ����


			// 2012-10-16 by bckim, ĳ�������� �������� Log �߰�
struct MSG_FL_LOG_ITEM_BUY_CASH_PRICE : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;								// ������ �����ѹ�, �����۹�ȣ
	UID32_t			AccountUniqueNumber;					// ���� ���� �ѹ� 
	INT				nAmount;								// ������ ���ż��� 
	INT				nType;									// ���� Ÿ��.

	INT64			nSPI;
	INT64			nWarPoint;
	INT64			nCash;
	INT64			nInfinity1;
	INT64			nInfinity2;
	INT64			nInfinity3;
	INT64			nInfinity4;
	INT64			nInfinity5;

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		//						%s|%s|%d|%d   |%d|%A|%T|%S   |%W   |%C   |%1   |%2   |%3   |%4   |%5|														
		sprintf(o_szLogString, "%s|%s|%d|%I64d|%d|%d|%d|%I64d|%I64d|%I64d|%I64d|%I64d|%I64d|%I64d|%I64d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp)
			, AccountUniqueNumber, Item4Log.ItemUniqueNumber, Item4Log.ItemNum, nAmount, nType
			, nSPI, nWarPoint, nCash, nInfinity1, nInfinity2, nInfinity3, nInfinity4, nInfinity5);
		return o_szLogString;
	}
};
// end 2012-10-16 by bckim, ĳ�������� �������� Log �߰�


// 2012-12-20 by bckim, �����ʱ�ȭ�� ���â��������� ����忡�� �Ű����� ������ �α�
struct MSG_FL_LOG_ITEM_INITIALIZED_GUILD_STORE
{
	// ����UID/�����̸�/�ɸ���UID/�ɸ����̸�/������UID/�����۳ѹ�/�����/����������/���ް���/���ѹ�/����̸�
	INT			AccountUniqueNumber;
	CHAR		AccountName[SIZE_MAX_ACCOUNT_NAME];
	INT			CharacterUniqueNumber;
	CHAR		CharacterName[SIZE_MAX_CHARACTER_NAME];

	INT64		ItemUniqueNumber;
	INT			ItemNum;
	INT			Kind;
	INT			CurrentCount;

	INT			GuildUniqueNumber;
	CHAR		GuildName[SIZE_MAX_GUILD_NAME];

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%s|%d|%s|%d|%s|%I64d|%d|%d\r\n", GetGameLogTypeString(i_nLogType)
			, GuildUniqueNumber, GuildName, AccountUniqueNumber, AccountName, CharacterUniqueNumber, CharacterName, ItemUniqueNumber, ItemNum, CurrentCount);
		return o_szLogString;
	}
};
// 2012-12-20 by bckim, �����ʱ�ȭ�� ���â��������� ����忡�� �Ű����� ������ �α�. End



struct MSG_FL_LOG_ITEM_SELL_ITEM : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	INT				ChangeCount;	// �ŷ� ����
	INT				RemainedMoney;	// ���� ��
	INT				PrefixCodeNum;			// 2007-07-20 by cmkwon, �Ǹž����� �α� �߰� - ����
	INT				SuffixCodeNum;			// 2007-07-20 by cmkwon, �Ǹž����� �α� �߰� - ����
	INT				EnchatItemNumCount;		// 2007-07-20 by cmkwon, �Ǹž����� �α� �߰� - EnchantItemNum List Count
	_ARRAY(INT);							// 2007-07-20 by cmkwon, �Ǹž����� �α� �߰� - EnchantItemNum List
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		// 2007-07-20 by cmkwon, �Ǹž����� �α� �߰� - ����/���� �߰�
		sprintf(o_szLogString, "%s|%s|%s|%d|%d|%d|%d", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem)
			, ChangeCount, RemainedMoney, PrefixCodeNum, SuffixCodeNum);
		return o_szLogString;
	}
};		// ������ �Ǹ�

struct MSG_FL_LOG_ITEM_USE_ITEM : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%s|-1\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem));
		return o_szLogString;
	}
};			// ������ ���

struct MSG_FL_LOG_ITEM_USE_ENERGY : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%s|-1\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem));
		return o_szLogString;
	}
};		// �������� ������ ���

struct MSG_FL_LOG_ITEM_USE_ENCHANT : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	INT				EnchantLogType;	// ENCHANT_LOGTYPE_FAIL, ENCHANT_LOGTYPE_SUCCESS, ENCHANT_LOGTYPE_TARGETITEM, ENCHANT_LOGTYPE_ATTACHITEM
	INT				nTargetItemPrefixCodeNum;	// 2007-07-30 by cmkwon, ����/��æƮ �α׿� ����/���� ���� ���� - �ʵ��߰�
	INT				nTargetItemSuffixCodeNum;	// 2007-07-30 by cmkwon, ����/��æƮ �α׿� ����/���� ���� ���� - �ʵ��߰�
	INT				nTargetItemEnchantCnt;		// 2010-04-20 by cmkwon, �ű� ��Ű �ӽ� ���� - 
	INT				nEnchantItemKind;			// ��æƮ �������� ItemKind // 2010-04-20 by cmkwon, �ű� ��Ű �ӽ� ���� - 
	INT				nEnchantItemNum;			// ��æƮ �������� ItemNum // 2010-04-20 by cmkwon, �ű� ��Ű �ӽ� ���� - 
	bool			bSuccessFlag;				// 2010-04-20 by cmkwon, �ű� ��Ű �ӽ� ���� - 
	UID64_t			arrAddedItemUID[2];			// 2010-04-20 by cmkwon, �ű� ��Ű �ӽ� ���� - (�߰� ����) - 
	INT				arrAddedItemNum[2];			// 2010-04-20 by cmkwon, �ű� ��Ű �ӽ� ���� - (�߰� ����) - 

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];

		// 2007-07-30 by cmkwon, ����/��æƮ �α׿� ����/���� ���� ���� - ���� �α� ������
		// 2010-04-20 by cmkwon, �ű� ��Ű �ӽ� ���� - 
		//sprintf(o_szLogString, "%s|%s|%s|%d|%d|%d\r\n"
		//	, GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem)
		//	, EnchantLogType, nTargetItemPrefixCodeNum, nTargetItemSuffixCodeNum);
		///////////////////////////////////////////////////////////////////////////////
		// 2010-04-20 by cmkwon, �ű� ��Ű �ӽ� ���� - 
		// 2010-04-20 by cmkwon, �ű� ��Ű �ӽ� ���� - (�߰� ����) - 
		sprintf(o_szLogString, "%s|%s|%s|%s|%d|%d|%d|%d|%d|%d|%I64d|%d|%I64d|%d\r\n"
			, GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem)
			, GetEnchatLogTypeString(EnchantLogType), nTargetItemPrefixCodeNum, nTargetItemSuffixCodeNum, nTargetItemEnchantCnt, nEnchantItemKind, nEnchantItemNum, bSuccessFlag
			, arrAddedItemUID[0], arrAddedItemNum[0], arrAddedItemUID[1], arrAddedItemNum[1]);
		return o_szLogString;
	}
};		// ��æƮ�� ������ ���

struct MSG_FL_LOG_ITEM_STORE_ITEM : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	INT				ChangeCount;	// ���� ����, 			// 2008-02-15 by cmkwon, �κ�<->â�� �̵� �α� ���� - MSG_FL_LOG_ITEM_STORE_ITEM �� �ʵ� �߰�
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		// 2008-02-15 by cmkwon, �κ�<->â�� �̵� �α� ���� - 
		//		sprintf(o_szLogString, "%s|%s|%s|-1\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem));
		sprintf(o_szLogString, "%s|%s|%s|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem), ChangeCount);
		return o_szLogString;
	}
};		// 2008-02-15 by cmkwon, �κ�<->â�� �̵� �α� ���� - �κ�-->â�� �� ī���ͺ� ������ �̵��� â�� �ش� ������ �߰� �� ���� ����� ����, // â�� ���� ������ ����, â�� ���� ������


struct MSG_FL_LOG_ITEM_USE_MIX : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	bool						IsSuccess;										// 2008-09-26 by cmkwon, ���ս� GameLog ���� - 
	ITEM_UID_W_ITEMNUM_COUNT	arrSourceItemList[COUNT_ITEM_MIXING_SOURCE];	// 2008-09-26 by cmkwon, ���ս� GameLog ���� - 
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		// 2008-09-26 by cmkwon, ���ս� GameLog ���� - �Ʒ��� ���� ���� ��
		//sprintf(o_szLogString, "%s|%s|%s|-1\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem));
		sprintf(o_szLogString, "%s|%s|%s|%s|%I64d|%d|%d|%I64d|%d|%d|%I64d|%d|%d|%I64d|%d|%d|%I64d|%d|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp),
			((IsSuccess) ? "SUCCESS" : "FAIL"), Item4Log.GetLogItemString(szItem)  // 2013-07-02 by bckim, ���շα� ���� 
			, arrSourceItemList[0].ItemUID, arrSourceItemList[0].ItemNum, arrSourceItemList[0].Count, arrSourceItemList[1].ItemUID, arrSourceItemList[1].ItemNum, arrSourceItemList[1].Count
			, arrSourceItemList[2].ItemUID, arrSourceItemList[2].ItemNum, arrSourceItemList[2].Count, arrSourceItemList[3].ItemUID, arrSourceItemList[3].ItemNum, arrSourceItemList[3].Count
			, arrSourceItemList[4].ItemUID, arrSourceItemList[4].ItemNum, arrSourceItemList[4].Count);
		return o_szLogString;
	}
};			// ���� ����, CurrentCount( 0�̸� ���� 1�̸� ����)

struct MSG_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	char			szOriginCharName[SIZE_MAX_CHARACTER_NAME];
	char			szNewCharName[SIZE_MAX_CHARACTER_NAME];
	DWORD			Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, ���� ���� ���� - ���� ������ ȣȯ �ȵǵ��� ����ü ũ�� �ٲ�
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%s|%s|%s\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem)
			, szOriginCharName, szNewCharName);
		return o_szLogString;
	}
};

using MSG_FL_LOG_ITEM_BAZAAR_SEND = MSG_FL_LOG_ITEM_TRADE_SEND;		// 2006-07-28 by cmkwon
using MSG_FL_LOG_ITEM_BAZAAR_RECV = MSG_FL_LOG_ITEM_TRADE_RECV;		// 2006-07-28 by cmkwon

struct MSG_FL_LOG_ITEM_SPI : public FL_ITEM_LOG_BASE			// 2006-10-27 by cmkwon
{
	ITEM_FOR_LOG	Item4Log;
	BYTE			ItemUpdateType;	// 2006-10-27 by cmkwon, IUT_XXXX
	INT				ChangeCount;	// ���� ����
	INT				RemainedMoney;	// ���� ��
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%s|%s|%d|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem)
			, GetItemUpdateTypeString(ItemUpdateType), ChangeCount, RemainedMoney);
		return o_szLogString;
	}
};

// 2008-09-26 by cmkwon, ���ս� GameLog ���� - 
//typedef MSG_FL_LOG_ITEM_GET_ITEM		MSG_FL_LOG_ITEM_GET_ITEM_BY_ITEMEVENT;		// 56 - ItemEvent�� ���� ������ �߰�, 2007-01-11 by cmkwon
using MSG_FL_LOG_ITEM_GET_ITEM_BY_ITEMEVENT = MSG_FL_LOG_ITEM_GET_ITEM_2;		// 56 - ItemEvent�� ���� ������ �߰�, 2007-01-11 by cmkwon

struct MSG_FL_LOG_WARPOINT
{// 58 - WarPoint ����, 2007-05-15 by dhjin
	UID32_t			AccountUID;
	CHAR			AccountName[SIZE_MAX_ACCOUNT_NAME];
	UID32_t			CharacterUID;
	CHAR			CharacterName[SIZE_MAX_CHARACTER_NAME];
	INT				ChangeWarPoint;
	INT				TotalWarPoint;
	INT				CumulativeWarPoint;
	BYTE			WPUpdateType;				// 2009-01-22 by cmkwon, ���� ���� ���� �α� ���� - MSG_FL_LOG_WARPOINT�� �߰�
	INT				ArenaPlayCount;				// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : �÷����� �Ʒ��� Ƚ�� �α� �߰�
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		// 2009-01-22 by cmkwon, ���� ���� ���� �α� ���� - �Ʒ��� ���� �߰���.
		//sprintf(o_szLogString, "%s|%d|%s|%d|%s|%d|%d|%d\r\n"
		//	, GetGameLogTypeString(i_nLogType), AccountUID, AccountName, CharacterUID, CharacterName, ChangeWarPoint, TotalWarPoint, CumulativeWarPoint);
		//////////////////////////////////////////////////////////////////////
		// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : �÷����� �Ʒ��� Ƚ�� �α� �߰�
		//sprintf(o_szLogString, "%s|%d|%s|%d|%s|%d|%d|%d|%s\r\n"
		//	, GetGameLogTypeString(i_nLogType), AccountUID, AccountName, CharacterUID, CharacterName, ChangeWarPoint, TotalWarPoint, CumulativeWarPoint, GetWPUTString(WPUpdateType));
		sprintf(o_szLogString, "%s|%d|%s|%d|%s|%d|%d|%d|%s|ArenaPlayCount(%d)\r\n"
			, GetGameLogTypeString(i_nLogType), AccountUID, AccountName, CharacterUID, CharacterName, ChangeWarPoint, TotalWarPoint, CumulativeWarPoint, GetWPUTString(WPUpdateType), ArenaPlayCount);
		// end 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : �÷����� �Ʒ��� Ƚ�� �α� �߰�
		return o_szLogString;
	}
};

struct MSG_FL_LOG_ARENA_TEAM_MATCH
{// 59 - Arena �� ��Ī, 2007-06-11 by dhjin
	BYTE			TeamMode;
	BYTE			TeamSize;
	BYTE			StartLevel;
	BYTE			EndLevel;
	MapIndex_t		ArenaMap;	// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ������ �Ʒ��� �� �α� �߰�
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		//////////////////////////////////////////////////////////////////////
		// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ������ �Ʒ��� �� �α� �߰�
		//		sprintf(o_szLogString, "%s|%d|%d|%d~%d\r\n", GetGameLogTypeString(i_nLogType), TeamMode, TeamSize, StartLevel, EndLevel);
		sprintf(o_szLogString, "%s|%d|%d|%d~%d|%d\r\n", GetGameLogTypeString(i_nLogType), TeamMode, TeamSize, StartLevel, EndLevel, ArenaMap);
		// end 2012-04-12 by jhseol, �Ʒ��� �߰����� - ������ �Ʒ��� �� �α� �߰�
		return o_szLogString;
	}
};

struct MSG_FL_LOG_TUTORIAL_COMPLETION
{// 60 - Tutorial, 2007-07-06 by dhjin
	UID32_t			AccountUID;
	CHAR			AccountName[SIZE_MAX_ACCOUNT_NAME];
	UID32_t			CharacterUID;
	CHAR			CharacterName[SIZE_MAX_CHARACTER_NAME];
	INT				TutorialNum;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%s|%d|%s|%d\r\n"
			, GetGameLogTypeString(i_nLogType), AccountUID, AccountName, CharacterUID, CharacterName, TutorialNum);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_GUILD_STORE
{// 2006-09-27 by dhjin, ���� â�� �α� ����� ����ü
	UID32_t			GuildUID;
	CHAR			GuildName[SIZE_MAX_GUILD_NAME];
	UID32_t			AccountUID;
	CHAR			AccountName[SIZE_MAX_ACCOUNT_NAME];
	UID32_t			CharacterUID;
	CHAR			CharacterName[SIZE_MAX_CHARACTER_NAME];
	UID64_t			ItemUID;
	INT				ItemNum;
	INT				PrefixCodeNum;
	INT				SuffixCodeNum;
	INT				ItemResultCount;
	INT				ItemChangeCount;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%s|%d|%s|%d|%s|%I64d|%d|%d|%d|%d|%d\r\n"
			, GetGameLogTypeString(i_nLogType), GuildUID, GuildName, AccountUID, AccountName, CharacterUID, CharacterName
			, ItemUID, ItemNum, PrefixCodeNum, SuffixCodeNum, ItemResultCount, ItemChangeCount);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_OUTPOST_START
{// 61 - OutPost���� �α�, 2007-10-02 by dhjin
	MapIndex_t			OutPostMapIdx;
	BYTE				PossessInflTy;
	UID32_t				PossessGuildUID;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%u\r\n", GetGameLogTypeString(i_nLogType), OutPostMapIdx, PossessInflTy, PossessGuildUID);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_OUTPOST_END
{// 62 - OutPost���� �α�, 2007-10-02 by dhjin
	MapIndex_t			OutPostMapIdx;
	BYTE				PossessInflTy;
	UID32_t				PossessGuildUID;
	CHAR				PossessGuildName[SIZE_MAX_GUILD_NAME];
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%u|%s\r\n"
			, GetGameLogTypeString(i_nLogType), OutPostMapIdx, PossessInflTy, PossessGuildUID, PossessGuildName);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_EXPEDIENCYFUND
{// 63 - �ǰ��� ��/�� �α�, 2007-10-02 by dhjin
	MapIndex_t		MapIndex;
	UID32_t			CharacterUID;	// 2007-10-16 by cmkwon, �α� �߰� - �߰�
	UID32_t			GuildUID;		// 2007-10-16 by cmkwon, �α� �߰� - �߰�
	bool			Increase;		// FALSE : ����, TRUE : ����
	INT				Count;
	INT				ExpediencyFundVRate;	// 2007-10-16 by cmkwon, �α� �߰� - EXPEDIENCYFUND_RATE_VALUE �� �������� ���� ¡����
	INT				TotalCount;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		// 2007-10-16 by cmkwon, �α� �߰� - ������
		sprintf(o_szLogString, "%s|%d|%u|%u|%d|%d|%.3f|%d\r\n"
			, GetGameLogTypeString(i_nLogType), MapIndex, CharacterUID, GuildUID, Increase, Count, ((float)ExpediencyFundVRate) / EXPEDIENCYFUND_RATE_VALUE, TotalCount);
		return o_szLogString;
	}
};

////////////////////////////////
// ETC LOG(0x40~)
struct MSG_FL_LOG_SERVER_INFO_MAP
{
	MapIndex_t		MapIndex;
	ChannelIndex_t	ChannelIndex;
	INT				ClientCount;
	INT				MonsterCount;
	char			MapName[SIZE_MAX_MAP_NAME];						// 2010-06-01 by shcho, GLogDB ���� -
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%d|%d\r\n", GetGameLogTypeString(i_nLogType), MapIndex, ChannelIndex, ClientCount, MonsterCount);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_SERVER_INFO_TOTAL
{
	INT				ClientCount;
	INT				MonsterCount;
	bool			bGlogUpdate;		// 2010-06-01 by shcho, GLogDB ���� -
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|0|0|%d|%d\r\n", GetGameLogTypeString(i_nLogType), ClientCount, MonsterCount);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_HACKING_LOG : public FL_USER_LOG_BASE
{
	char			AccountName0[SIZE_MAX_ACCOUNT_NAME];
	long			hackingCode;
	char			szErrString[SIZE_STRING_128];			// 2006-10-20 by cmkwon, �߰���(��ŷ ���α׷���)
	char			IPAddress[SIZE_MAX_IPADDRESS];
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%s|%d|%s|%s\r\n", GetGameLogTypeString(i_nLogType), AccountName0, GetLogBaseString(szTemp), hackingCode, IPAddress, szErrString);
		return o_szLogString;
	}
};

////////////////////////////////
// MONSTER LOG(0x50~)
struct MSG_FL_LOG_MONSTER_BOSS
{
	MAP_CHANNEL_INDEX	MapChannel;
	INT					MonsterUnitKind;
	ATUM_DATE_TIME		CreatedTime;
	ATUM_DATE_TIME		DeadTime;
	char				AttackUser[SIZE_MAX_USERLIST_ON_BOSS_MONSTER_DEAD][SIZE_MAX_CHARACTER_NAME];
	INT					LenOfDropItemList;		// Drop Item List string�� ����
	ARRAY_(char);								// Drop Item List string
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString, char *i_szDropItemList)
	{
		sprintf(o_szLogString, "%s|%d|%d|%d|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s\r\n", GetGameLogTypeString(i_nLogType), MapChannel.MapIndex, MapChannel.ChannelIndex, MonsterUnitKind
			, CreatedTime.GetDateTimeString().GetBuffer(), DeadTime.GetDateTimeString().GetBuffer()
			, AttackUser[0], AttackUser[1], AttackUser[2], AttackUser[3], AttackUser[4], AttackUser[5]
			, AttackUser[6], AttackUser[7], AttackUser[8], AttackUser[9], i_szDropItemList);
		return o_szLogString;
	}
};	// ������ ����



struct MSG_FL_LOG_INFLWAR_START
{// 2007-10-16 by cmkwon, �α� �߰� -
	MapIndex_t			MothershipCreatedMapIdx;
	BYTE				InfluenceType2;
	INT					MonsterUnitKind;
	BYTE				MonBell;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%d|%d\r\n", GetGameLogTypeString(i_nLogType), MothershipCreatedMapIdx, InfluenceType2, MonsterUnitKind, MonBell);
		return o_szLogString;
	}
};
struct MSG_FL_LOG_INFLWAR_END
{// 2007-10-16 by cmkwon, �α� �߰� -
	MapIndex_t			MothershipCreatedMapIdx;
	BYTE				InfluenceType2;
	INT					MonsterUnitKind;
	BYTE				MonBell;
	bool				DeadFlag;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%d|%d|%d\r\n", GetGameLogTypeString(i_nLogType), MothershipCreatedMapIdx, InfluenceType2, MonsterUnitKind, MonBell, DeadFlag);
		return o_szLogString;
	}
};
struct MSG_FL_LOG_OUTPOSTWAR_RESET_START
{// 2007-10-16 by cmkwon, �α� �߰� -
	MapIndex_t			OutPostMapIdx;
	BYTE				PossessInflTy;
	UID32_t				PossessGuildUID;
	BYTE				InflTy4Reset;
	UID32_t				GuildUID4Reset;
	UID32_t				ResetUserGuildUID;
	UID32_t				ResetUserCharacterUID;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%u|%d|%u|%u|%u\r\n", GetGameLogTypeString(i_nLogType), OutPostMapIdx, PossessInflTy, PossessGuildUID, InflTy4Reset, GuildUID4Reset, ResetUserGuildUID, ResetUserCharacterUID);
		return o_szLogString;
	}
};
struct MSG_FL_LOG_OUTPOSTWAR_RESET_DESTROY
{// 2007-10-16 by cmkwon, �α� �߰� -
	MapIndex_t			OutPostMapIdx;
	BYTE				PossessInflTy;
	UID32_t				PossessGuildUID;
	BYTE				InflTy4Reset;
	UID32_t				GuildUID4Reset;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%u|%d|%u\r\n", GetGameLogTypeString(i_nLogType), OutPostMapIdx, PossessInflTy, PossessGuildUID, InflTy4Reset, GuildUID4Reset);
		return o_szLogString;
	}
};
struct MSG_FL_LOG_OUTPOSTWAR_RESET_COMPLETE
{// 2007-10-16 by cmkwon, �α� �߰� -
	MapIndex_t			OutPostMapIdx;
	BYTE				PossessInflTy;
	UID32_t				PossessGuildUID;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%u\r\n", GetGameLogTypeString(i_nLogType), OutPostMapIdx, PossessInflTy, PossessGuildUID);
		return o_szLogString;
	}
};

#define SIZE_MAX_OUTPOSTWAR_DAMAGELIST			10		// 2007-10-16 by cmkwon, �α� �߰� - 
struct MSG_FL_LOG_OUTPOSTWAR_PROTECTOR_DESTROY
{// 2007-10-16 by cmkwon, �α� �߰� -
	MapIndex_t			OutPostMapIdx;
	BYTE				PossessInflTy;
	UID32_t				PossessGuildUID;
	BYTE				InflTy4Reset;
	UID32_t				GuildUID4Reset;
	UID32_t				ArrGuildUID[SIZE_MAX_OUTPOSTWAR_DAMAGELIST];
	float				ArrDamage[SIZE_MAX_OUTPOSTWAR_DAMAGELIST];
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		// 2007-10-16 by cmkwon, ������ ����Ʈ�� �ܺο��� ����� �̾ ���δ�
		sprintf(o_szLogString, "%s|%d|%d|%u|%d|%u", GetGameLogTypeString(i_nLogType), OutPostMapIdx, PossessInflTy, PossessGuildUID, InflTy4Reset, GuildUID4Reset);
		return o_szLogString;
	}
};
struct MSG_FL_LOG_OUTPOSTWAR_SET_NEXTTIME
{// 2007-10-16 by cmkwon, �α� �߰� -
	MapIndex_t			OutPostMapIdx;
	BYTE				PossessInflTy;
	UID32_t				PossessGuildUID;
	UID32_t				UserGuildUID;
	UID32_t				UserCharacterUID;
	ATUM_DATE_TIME		NextTime;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%u|%u|%u|%s\r\n", GetGameLogTypeString(i_nLogType), OutPostMapIdx, PossessInflTy, PossessGuildUID, UserGuildUID, UserCharacterUID, NextTime.GetDateTimeString().GetBuffer());
		return o_szLogString;
	}
};

struct MSG_FL_LOG_POLL_VOTE
{// 2007-11-08 by dhjin
	UID32_t				VoteCharacterUID;
	UID32_t				CharacterUID;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d\r\n", GetGameLogTypeString(i_nLogType), VoteCharacterUID, CharacterUID);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_POLL_DELETE_LEADERCANDIDATE
{// 2007-11-08 by dhjin
	UID32_t				CharacterUID;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d\r\n", GetGameLogTypeString(i_nLogType), CharacterUID);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_DISMEMBER_GUILD
{// 2007-11-09 by dhjin
	UID32_t				GuildUID;
	char				GuildName[SIZE_MAX_GUILD_NAME];
	UID32_t				GuildCommanderUID;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%s|%d\r\n", GetGameLogTypeString(i_nLogType), GuildUID, GuildName, GuildCommanderUID);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_NOTIFY_MSG_DELETE : public SNOTIFY_MSG		// 2007-11-28 by cmkwon, �����ý��� ���� -
{
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%I64d|%u|%d|%s|%u|%s|%s\r\n", GetGameLogTypeString(i_nLogType), NotifyMsgUID, CharacterUID, NotifyMsgType, NotifyMsgString, SenderCharacterUID, SenderCharacterName, CreateTime.GetDateTimeString().GetBuffer());
		return o_szLogString;
	}
};
struct MSG_FL_LOG_USE_COUPON : public FL_USER_LOG_BASE		// 2008-01-23 by cmkwon, S_F, S_L: ���� ��� ���� �α׿� �߰� - 
{
	char AccountName[SIZE_MAX_ACCOUNT_NAME];
	char CouponNumber[SIZE_MAX_COUPON_NUMBER];
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%s|%s\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), AccountName, CouponNumber);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_ITEM_ATTACH_ITEM : public FL_ITEM_LOG_BASE		// 2008-01-23 by cmkwon, S_F, S_L: ����/�������� ���� �α׿� �߰� - 
{
	ITEM_FOR_LOG	Item4Log;
	INT				ReaminTimeSec;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%s|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem)
			, ReaminTimeSec);
		return o_szLogString;
	}
};
struct MSG_FL_LOG_ITEM_DETACH_ITEM : public FL_ITEM_LOG_BASE		// 2008-01-23 by cmkwon, S_F, S_L: ����/�������� ���� �α׿� �߰� - 
{
	ITEM_FOR_LOG	Item4Log;
	INT				ReaminTimeSec;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%s|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem)
			, ReaminTimeSec);
		return o_szLogString;
	}
};
struct MSG_FL_LOG_ITEM_DELETE : public FL_ITEM_LOG_BASE		// 2008-01-23 by cmkwon, S_F, S_L: ����/�������� ���� �α׿� �߰� - 
{
	ITEM_FOR_LOG	Item4Log;
	INT				ItemDeleteType;
	ATUM_DATE_TIME	atCreatedTime;			// 2009-11-17 by cmkwon, �������� ���� ������ ������ �α� ���� �߰� - MSG_FL_LOG_ITEM_DELETE
	INT				nUsedTime;				// 2009-11-17 by cmkwon, �������� ���� ������ ������ �α� ���� �߰� - MSG_FL_LOG_ITEM_DELETE
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		// 2009-11-17 by cmkwon, �������� ���� ������ ������ �α� ���� �߰� - 
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%s|%s|%s|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem)
			, GetItemDeleteTypeString(ItemDeleteType), atCreatedTime.GetDateTimeString().GetBuffer(), nUsedTime);
		return o_szLogString;
	}
};

//////////////////////////////////////////////////////////////////////////
// 2008-04-01 by dhjin, ����, ������ ����â ��ȹ�� - 
struct MSG_FL_LOG_MS_WAR
{
	BYTE			AttInfluence;
	INT				MonsterUID;
	INT				ContributionPoint;
	ATUM_DATE_TIME	MSWarStartTime;
	BYTE			WinInfluence;

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%d|%s|%d\r\n", GetGameLogTypeString(i_nLogType), AttInfluence, MonsterUID, ContributionPoint, MSWarStartTime.GetDateTimeString().GetBuffer(), WinInfluence);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_SP_WAR
{
	BYTE			AttInfluence;
	INT				SPSummonMapIndex;
	ATUM_DATE_TIME	SPWarStartTime;
	BYTE			WinInfluence;

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%s|%d\r\n", GetGameLogTypeString(i_nLogType), AttInfluence, SPSummonMapIndex, SPWarStartTime.GetDateTimeString().GetBuffer(), WinInfluence);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_WAR_CONTRIBUTION
{
	UID32_t		CharacterUID;
	MapIndex_t	nMapIndex;
	UID64_t		nContribution;
	INT			nPay;
	BYTE		byPayType;

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%I64d|%d|%d\r\n", GetGameLogTypeString(i_nLogType), CharacterUID, nMapIndex, nContribution, nPay, byPayType);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_WAR_CONTRIBUTION_GEAR
{
	UID64_t			nTotalBGearContribution;
	UID64_t			nTotalIGearContribution;
	UID64_t			nTotalAGearContribution;
	UID64_t			nTotalMGearContribution;
	INT				nBGearCount;
	INT				nIGearCount;
	INT				nAGearCount;
	INT				nMGearCount;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%I64d|%I64d|%I64d|%I64d|%d|%d|%d|%d\r\n", GetGameLogTypeString(i_nLogType)
			, nTotalBGearContribution, nTotalIGearContribution, nTotalAGearContribution, nTotalMGearContribution
			, nBGearCount, nIGearCount, nAGearCount, nMGearCount);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_LUCKY_DROP
{// 2009-03-31 by dhjin, ��Ű�ӽ� �α�
	INT				MysteryItemDropNum;
	INT				DropItemNum;
	ATUM_DATE_TIME	Starttime;				// ��Ű�ӽ� ���� �ð�, 2008-11-04 by dhjin, ��Ű�ӽ�
	ATUM_DATE_TIME	Endtime;				// ��Ű�ӽ� ���� �ð�, 2008-11-04 by dhjin, ��Ű�ӽ�
	INT				CountPerPeriod;			// �Ⱓ���� ����� �ִ� ������ ��, 2008-11-04 by dhjin, ��Ű�ӽ�
	INT				DropCount;				// �Ⱓ���� ���� ����� ������ ��, 2008-11-04 by dhjin, ��Ű�ӽ�

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%s|%s|%d|%d\r\n", GetGameLogTypeString(i_nLogType)
			, MysteryItemDropNum, DropItemNum, Starttime.GetDateTimeString().GetBuffer(), Endtime.GetDateTimeString().GetBuffer()
			, CountPerPeriod, DropCount);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_INFINITY_START {
	// 193 - // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� ���� ���� 
	InfinityCreateUID_t		InfinityCreateUID;		// �ش� ���� ���� ��ȣ
	//USHORT					MainServerID;			// ������ ĳ������ ����
	BYTE					InfluenceType;			// ������ ĳ������ ����
	char					CharacterName[SIZE_MAX_CHARACTER_NAME];		//	ĳ���͸�
	UID32_t					CharacterUID;			// (ĳ���� UID)
	Lv_t					Level;					// ������ ĳ������ ����
	USHORT					UnitKind;				// ������ ĳ������ �������
	Experience_t			Experience;				// ����ġ��
	float					CurrentHP;				// HP
	float					CurrentDP;				// DP
	SHORT					CurrentSP;				// SP

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString) {
		sprintf(o_szLogString, "%s|%I64d|%d|%s(%d)|%d|%d|%f|%f|%f|%d\r\n", GetGameLogTypeString(i_nLogType)
			, InfinityCreateUID, InfluenceType, CharacterName, CharacterUID, Level, UnitKind, Experience, CurrentHP, CurrentDP, CurrentSP);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_INFINITY_START_ITEM : public FL_ITEM_LOG_BASE {
	// 194 - // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���۽� ������ ���� ����
	ITEM_FOR_LOG	Item4Log;
	INT				ReaminTimeMSec;
	UID64_t			MainSvrItemUID;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString) {
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%s|%d|%I64d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem)
			, ReaminTimeMSec, MainSvrItemUID);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_INFINITY_CINEMA {
	// 195 - // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� �α�
	InfinityCreateUID_t		InfinityCreateUID;		// �ش� ���� ���� ��ȣ
	MSec_t					StartTime;
	EffectIdx_t				EffectIdx;
	ObjectIdx_t				ObjectIdx;
	ObjectNum_t				ChangeObjectIdx;
	MonIdx_t				MonsterIdx;
	MSec_t					CameraTremble;
	CinemaTalk_t			CinemaTalk[SIZE_MAX_CINEMATALK_DESCRIPTION];
	QuestIndex_t			QuestIndex;		// 2011-03-09 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ����Ʈ �ε��� ���� �߰� -

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString) {
		sprintf(o_szLogString, "%s|%I64d|%d|%d|%d|%d|%d|%d|%s|%d\r\n", GetGameLogTypeString(i_nLogType)
			, InfinityCreateUID, StartTime, EffectIdx, ObjectIdx, ChangeObjectIdx, MonsterIdx, CameraTremble, CinemaTalk, QuestIndex);		// 2011-03-09 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ����Ʈ �ε��� ���� �߰�
		return o_szLogString;
	}
};

struct MSG_FL_LOG_MONSTER_SKILL {
	// 196 - // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� ��ų ����
	MonIdx_t				MonsterIdx;
	ItemNum_t				MonsterSkillNum;

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString) {
		sprintf(o_szLogString, "%s|%d|%d\r\n", GetGameLogTypeString(i_nLogType)
			, MonsterIdx, MonsterSkillNum);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_HPACTION_TALK {
	// 197 - // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� ��ȭ ����
	MonIdx_t				MonsterIdx;
	HPTalk_t				HPTalk[SIZE_MAX_HPTALK_DESCRIPTION];
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString) {
		sprintf(o_szLogString, "%s|%d|%s\r\n", GetGameLogTypeString(i_nLogType)
			, MonsterIdx, HPTalk);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_INFINITY_FIN {
	// 198 - // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� ���� ���� 
	InfinityCreateUID_t		InfinityCreateUID;		// �ش� ���� ���� ��ȣ
	//USHORT					MainServerID;			// ������ ĳ������ ����
	BYTE					InfluenceType;			// ������ ĳ������ ����
	char					CharacterName[SIZE_MAX_CHARACTER_NAME];		//	ĳ���͸�
	UID32_t					CharacterUID;			// (ĳ���� UID)
	Lv_t					Level;					// ������ ĳ������ ����
	USHORT					UnitKind;				// ������ ĳ������ �������
	Experience_t			Experience;				// ����ġ��
	float					CurrentHP;				// HP
	float					CurrentDP;				// DP
	SHORT					CurrentSP;				// SP
	bool					Clear;					// ���Ǵ�Ƽ �ʵ� Ŭ���� ����
	KillCnt_t				KillCount;				// ���� ����� �ϰ��� ��(��Ÿ Ƚ��)
	DeathCnt_t				DeathCount;				// Ŭ���� �ϱ���� ��� Ƚ��

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString) {
		sprintf(o_szLogString, "%s|%I64d|%d|%s(%d)|%d|%d|%f|%f|%f|%d|%d|%d|%d\r\n", GetGameLogTypeString(i_nLogType)
			, InfinityCreateUID, InfluenceType, CharacterName, CharacterUID, Level, UnitKind, Experience, CurrentHP, CurrentDP, CurrentSP, Clear, KillCount, DeathCount);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_INFINITY_FIN_ITEM : public FL_ITEM_LOG_BASE {
	// 199 - // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ����� ������ ���� ���� 
	ITEM_FOR_LOG	Item4Log;
	INT				ReaminTimeMSec;
	UID64_t			MainSvrItemUID;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString) {
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%s|%d|%I64d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem)
			, ReaminTimeMSec, MainSvrItemUID);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_INFINITY_FIN_ALIVE_KEY_MONSTER {
	// 200 - // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ����� ����ִ� ���� ����
	MonIdx_t				MonsterIdx;

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString) {
		sprintf(o_szLogString, "%s|%d\r\n", GetGameLogTypeString(i_nLogType)
			, MonsterIdx);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_PET_LEVEL : public FL_USER_LOG_BASE {
	// 213 - // 2011-09-15 by hskim, ��Ʈ�� �ý��� 2�� - ��Ʈ�� ���� ����
	BYTE		FromPetLevel;
	BYTE		ToPetLevel;
	LONGLONG	TotalPlayTime;
	// 2012-10-04 by hskim, �ѱ� ��ü ���� (����� IDC) - ��Ʈ�� �α� ����
	UID64_t		UniqueNumber;
	Experience_t	PetTotalExp;
	Experience_t	PetAcquisitionExp;
	// 2012-10-04 by hskim, �ѱ� ��ü ���� (����� IDC) - ��Ʈ�� �α� ����
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%I64d|%f|%f|%d|%d|%I64d\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), UniqueNumber, PetTotalExp, PetAcquisitionExp, FromPetLevel, ToPetLevel, TotalPlayTime);
		return o_szLogString;
	}
};					// ��Ʈ�� ���� ��ȭ

					// start 2011-12-12 by hskim, GLog 2��
struct MSG_FL_LOG_CONNECT_USER_NEW
{
	char AccountName[SIZE_MAX_ACCOUNT_NAME];

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%s\r\n", GetGameLogTypeString(i_nLogType), AccountName);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_CONNECT_USER_LOGIN
{
	char AccountName[SIZE_MAX_ACCOUNT_NAME];

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%s\r\n", GetGameLogTypeString(i_nLogType), AccountName);
		return o_szLogString;
	}
};
// end 2011-12-12 by hskim, GLog 2��

// start 2012-10-08 by khkim, GLog
struct MSG_FL_LOG_ACCOUNTCONNECT
{
	SHORT		LogType;
	UID64_t		UserSerial;
	UID64_t		IP;
	//int			GameServerID;

	MSG_FL_LOG_ACCOUNTCONNECT()
	{
		LogType = NULL;
		UserSerial = NULL;
		IP = NULL;
		//GameServerID = NULL;
	}
};

struct MSG_FL_LOG_CHARDATA
{
	SHORT		LogType;
	UID64_t		UserSerial;
	UID64_t		CharSerial;
	SHORT		Class;
	SHORT		Lv;
	UID64_t		Exp;
	UID64_t		GameMoney;
	UID64_t		Playtime;
	//int			GameServerID;

	MSG_FL_LOG_CHARDATA()
	{
		LogType = NULL;
		UserSerial = NULL;
		CharSerial = NULL;
		Class = NULL;
		Lv = NULL;
		Exp = NULL;
		GameMoney = NULL;
		Playtime = NULL;
		//GameServerID = NULL;
	}
};

struct MSG_FL_LOG_ITEM
{
	SHORT		LogType;
	UID64_t		ItemSerial;
	UID64_t		Count1;
	UID64_t		Count2;
	UID64_t		CharSerial;
	int			CharLv;

	MSG_FL_LOG_ITEM()
	{
		LogType = NULL;
		ItemSerial = NULL;
		Count1 = NULL;
		Count2 = NULL;
		CharSerial = NULL;
		CharLv = NULL;
//		GameServerID = NULL;
	}
};

struct MSG_FL_LOG_ITEMSTATE
{
	SHORT		LogType;
	UID64_t		ItemSerial;
	UID64_t		Count;
	UID64_t		CharSerial;
	int			CharLv;
	//int			GameServerID;

	MSG_FL_LOG_ITEMSTATE()
	{
		LogType = NULL;
		ItemSerial = NULL;
		Count = NULL;
		CharSerial = NULL;
		CharLv = NULL;
		//GameServerID = NULL;
	}
};

struct MSG_FL_LOG_SERVER
{
	SHORT		LogType;

	MSG_FL_LOG_SERVER() : LogType { 0 } { }
};
// end 2012-10-08 by khkim, GLog 2��

// start 2012-01-16 by hskim, ��� - ȭ��
struct MSG_FL_LOG_STATISTICS_MONEY
{
	BYTE					FunctionType;
	StatisticsMoneyUnit_t	SPI;
	StatisticsMoneyUnit_t	WarPoint;
	StatisticsMoneyUnit_t	SystemOrb;
	StatisticsMoneyUnit_t	QuasarOrb;
	StatisticsMoneyUnit_t	ClusterOrb;
	StatisticsMoneyUnit_t	GalaxyOrb;
	StatisticsMoneyUnit_t	UniverseOrb;

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|\r\n", GetGameLogTypeString(i_nLogType));
		return o_szLogString;
	}
};
// end 2012-01-16 by hskim, ��� - ȭ��

// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����) 
struct MSG_FL_LOG_FIXED_TERM_SHAPE_START : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	INT				ShapeItemNum;		// ���� ��ȣ
	INT				ApplyItemNum;		// ���� ���� ������ ��ȣ
	ATUM_DATE_TIME	Starttime;			// ���� ���� ���� �ð�
	ATUM_DATE_TIME	Endtime;			// ���� ���� ���� �ð�

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%s|%d|%d|%s|%s\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem)
			, ShapeItemNum, ApplyItemNum, Starttime.GetDateTimeString().GetBuffer(), Endtime.GetDateTimeString().GetBuffer());
		return o_szLogString;
	}
};

struct MSG_FL_LOG_FIXED_TERM_SHAPE_END : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	INT				ShapeItemNum;		// ���� ���� ������ ��ȣ
	INT				ApplyItemNum;		// ���� ���� ������ ��ȣ
	ATUM_DATE_TIME	Starttime;			// ���� ���� ���� �ð�
	ATUM_DATE_TIME	Endtime;			// ���� ���� ���� �ð�

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%s|%d|%d|%s|%s\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem)
			, ShapeItemNum, ApplyItemNum, Starttime.GetDateTimeString().GetBuffer(), Endtime.GetDateTimeString().GetBuffer());
		return o_szLogString;
	}
};
// end 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)

///////////////////////////////////////////////////////////////////////////////
// Monitor Server Protocol (T0: 0x30~)
///////////////////////////////////////////////////////////////////////////////

//////////////////////////////////
// T0_FM_MONITOR - Field Server Monitor
//#define T1_FM_GET_NUM_CLIENTS			0x00	// No Body
//#define T1_FM_GET_NUM_CLIENTS_OK		0x01
//#define T1_FM_SHUTDOWN					0x02	// No Body
//#define T1_FM_SHUTDOWN_OK				0x03	// No Body
//#define T1_FM_PING						0x04	// No Body
//#define T1_FM_PING_OK					0x05	// No Body
//#define T1_FM_PRINTMAP					0x06	// No Body
//#define T1_FM_SAVE_MAPBLOCK_INFO		0x07	// No Body
//#define T1_FM_PRINT_DEBUG_MSG			0x08	// No Body
//#define T1_FM_SEND_CHANNEL_INFO			0x09	// F->M
//#define T1_FM_CHANGE_BANDWIDTH_FLAG		0x0A	// No Body
//#define T1_FM_CHANGE_BANDWIDTH_FLAG_OK	0x0B	// ����� �÷��װ�
//#define T1_FM_SET_MSG_PRINT_LEVEL		0x0C
//#define T1_FM_GET_CHARACTER_INFO		0x0D
//#define T1_FM_GET_CHARACTER_INFO_OK		0x0E
//#define T1_FM_GET_MAP_USER_COUNTS		0x0F	// No Body
//#define T1_FM_GET_MAP_USER_COUNTS_ACK	0x10
//#define T1_FM_RELOAD_ITEM_INFO			0x11	// No Body
//#define T1_FM_RELOAD_ITEM_INFO_OK		0x12	// No Body
//#define T1_FM_RESET_NPC_SERVER			0x13	// No Body
//#define T1_FM_RESET_NPC_SERVER_OK		0x14	// No Body
//#define T1_FM_SERVER_THREAD_STATUS		0x15
//#define T1_FM_SET_SERVICE_STATE			0x16
//#define T1_FM_SET_SERVICE_STATE_OK		0x17	// No Body
//#define T1_FM_RELOAD_SERVER_INFO		0x18
//#define T1_FM_RELOAD_SERVER_INFO_OK		0x19	// No Body
//#define T1_FM_INCREASE_CHANNEL			0x1A	// M->F, Channel �� ����
//#define T1_FM_INCREASE_CHANNEL_OK		0x1B	// F->M, Channel �� ���� ���
//#define T1_FM_SET_CHANNEL_STATE			0x1C	// M->F, Channel Enable/Disable
//#define T1_FM_SET_CHANNEL_STATE_OK		0x1D	// F->M, Channel Enable/Disable ���
//#define T1_FM_CITYWAR_CHANGE_WAR_TIME		0x20		// M->F
//#define T1_FM_CITYWAR_CHANGED_OCCUPY_INFO	0x21	// F->M

//#define T_FM_GET_NUM_CLIENTS			(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_GET_NUM_CLIENTS)
//#define T_FM_GET_NUM_CLIENTS_OK			(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_GET_NUM_CLIENTS_OK)
//#define T_FM_SHUTDOWN					(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_SHUTDOWN)
//#define T_FM_SHUTDOWN_OK				(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_SHUTDOWN_OK)
//#define T_FM_PING						(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_PING)
//#define T_FM_PING_OK					(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_PING_OK)
//#define T_FM_PRINTMAP					(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_PRINTMAP)
//#define T_FM_SAVE_MAPBLOCK_INFO			(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_SAVE_MAPBLOCK_INFO)
//#define T_FM_PRINT_DEBUG_MSG			(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_PRINT_DEBUG_MSG)
//#define T_FM_SEND_CHANNEL_INFO			(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_SEND_CHANNEL_INFO)		// F->M
//#define T_FM_CHANGE_BANDWIDTH_FLAG		(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_CHANGE_BANDWIDTH_FLAG)
//#define T_FM_CHANGE_BANDWIDTH_FLAG_OK	(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_CHANGE_BANDWIDTH_FLAG_OK)
//#define T_FM_SET_MSG_PRINT_LEVEL		(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_SET_MSG_PRINT_LEVEL)
//#define T_FM_GET_CHARACTER_INFO			(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_GET_CHARACTER_INFO)
//#define T_FM_GET_CHARACTER_INFO_OK		(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_GET_CHARACTER_INFO_OK)
//#define T_FM_GET_MAP_USER_COUNTS		(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_GET_MAP_USER_COUNTS)
//#define T_FM_GET_MAP_USER_COUNTS_ACK	(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_GET_MAP_USER_COUNTS_ACK)
//#define T_FM_RELOAD_ITEM_INFO			(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_RELOAD_ITEM_INFO)
//#define T_FM_RELOAD_ITEM_INFO_OK		(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_RELOAD_ITEM_INFO_OK)
//#define T_FM_RESET_NPC_SERVER			(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_RESET_NPC_SERVER)
//#define T_FM_RESET_NPC_SERVER_OK		(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_RESET_NPC_SERVER_OK)
//#define T_FM_SERVER_THREAD_STATUS		(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_SERVER_THREAD_STATUS)
//#define T_FM_SET_SERVICE_STATE			(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_SET_SERVICE_STATE)
//#define T_FM_SET_SERVICE_STATE_OK		(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_SET_SERVICE_STATE_OK)	// No Body
//#define T_FM_RELOAD_SERVER_INFO			(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_RELOAD_SERVER_INFO)
//#define T_FM_RELOAD_SERVER_INFO_OK		(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_RELOAD_SERVER_INFO_OK)	// No Body
//#define T_FM_INCREASE_CHANNEL			(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_INCREASE_CHANNEL)		// M->F, Channel �� ����
//#define T_FM_INCREASE_CHANNEL_OK		(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_INCREASE_CHANNEL_OK)	// F->M, Channel �� ���� ���
//#define T_FM_SET_CHANNEL_STATE			(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_SET_CHANNEL_STATE)		// M->F, Channel Enable/Disable
//#define T_FM_SET_CHANNEL_STATE_OK		(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_SET_CHANNEL_STATE_OK)	// F->M, Channel Enable/Disable ���
//#define T_FM_CITYWAR_CHANGE_WAR_TIME		(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_CITYWAR_CHANGE_WAR_TIME)		// M->F
//#define T_FM_CITYWAR_CHANGED_OCCUPY_INFO	(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_CITYWAR_CHANGED_OCCUPY_INFO)	// F->M


struct MSG_FM_GET_NUM_CLIENTS_OK
{
	USHORT	NumOfClients;
};
using MSG_PM_GET_NUM_CLIENTS_OK = MSG_FM_GET_NUM_CLIENTS_OK;
using MSG_IM_GET_NUM_CLIENTS_OK = MSG_FM_GET_NUM_CLIENTS_OK;

struct MSG_PM_RELOAD_VERSION_INFO
{
	USHORT	LauncherVersion[4];
	USHORT	DeleteFileListVersion[4];
	USHORT	NoticeVersion[4];
	INT		NumOfClientVersions;
	ARRAY_(USHORT[8]);
};

struct MSG_FM_PRINTMAP
{
	MAP_CHANNEL_INDEX		MapChannelIndex;
};

using MSG_NM_PRINTMAP = MSG_FM_PRINTMAP;

struct MSG_FM_SAVE_MAPBLOCK_INFO
{
	MAP_CHANNEL_INDEX		MapChannelIndex;
};

using PMSG_FM_SAVE_MAPBLOCK_INFO = MSG_FM_SAVE_MAPBLOCK_INFO*;

using MSG_NM_SAVE_MAPBLOCK_INFO = MSG_FM_SAVE_MAPBLOCK_INFO;

struct MSG_FM_PRINT_DEBUG_MSG
{
	int		IntParam1;
	int		IntParam2;
	float	FloatParam1;
	float	FloatParam2;
	char	StringParam[100];
};
using MSG_IM_PRINT_DEBUG_MSG = MSG_FM_PRINT_DEBUG_MSG;

#ifndef _ATUM_CLIENT

struct MSG_FM_SEND_CHANNEL_INFO
{
	PROJECTINFO			projectInfo;
	bool				bCityWarMap;
	SCITY_OCCUPY_INFO	cityWarOccupyInfo;
	ChannelIndex_t		ChannelIndex;	// ä���� �ε���
	bool				IsEnabled;		// ä���� Ȱ��ȭ ���ο� ���� ����
};			// F->M

struct MSG_NM_SEND_CHANNEL_INFO
{
	PROJECTINFO		projectInfo;
	ChannelIndex_t	ChannelIndex;	// ä���� �ε���
	bool			IsEnabled;		// ä���� Ȱ��ȭ ���ο� ���� ����
	UINT			LimitMonsterCount;
};

#endif // _ATUM_CLIENT

struct MSG_FM_CHANGE_BANDWIDTH_FLAG_OK
{
	bool	bChagedFlag;
};

using PMSG_FM_CHANGE_BANDWIDTH_FLAG_OK = MSG_FM_CHANGE_BANDWIDTH_FLAG_OK*;

using MSG_PM_CHANGE_BANDWIDTH_FLAG_OK = MSG_FM_CHANGE_BANDWIDTH_FLAG_OK;
using MSG_NM_CHANGE_BANDWIDTH_FLAG_OK = MSG_FM_CHANGE_BANDWIDTH_FLAG_OK;
using MSG_IM_CHANGE_BANDWIDTH_FLAG_OK = MSG_FM_CHANGE_BANDWIDTH_FLAG_OK;

struct MSG_FM_SET_MSG_PRINT_LEVEL
{
	BYTE	Level;
};
using MSG_PM_SET_MSG_PRINT_LEVEL = MSG_FM_SET_MSG_PRINT_LEVEL;
using MSG_NM_SET_MSG_PRINT_LEVEL = MSG_FM_SET_MSG_PRINT_LEVEL;
using MSG_IM_SET_MSG_PRINT_LEVEL = MSG_FM_SET_MSG_PRINT_LEVEL;

struct MSG_FM_GET_CHARACTER_INFO
{
	char			CharacterName[SIZE_MAX_CHARACTER_NAME];
	UID32_t			CharacterUniqueNumber;
	ClientIndex_t	ClientIndex;
};

struct MSG_FM_GET_CHARACTER_INFO_OK
{
	CHARACTER		CharacterInfo;
};

struct MSG_FM_GET_MAP_USER_COUNTS
{
	MAP_CHANNEL_INDEX	MapChannIndex;
};

struct MSG_FM_GET_MAP_USER_COUNTS_ACK
{
	MAP_CHANNEL_INDEX	MapChannIndex;
	UINT		AccumulatedUserCounts;
	UINT		CurrentUserCounts;
	UINT		MaxUserCounts;
	UINT		AccumulatedMonsterCounts;
	UINT		CurrentMonsterCounts;
};

struct MSG_FM_SERVER_THREAD_STATUS
{
	DWORD	dwThreadId;
	char	szThreadComment[SIZE_MAX_THREAD_COMMENT];
	DWORD	dwTimeGap;
};

// SERVER_SERVICE_TYPE_XXX
#define SERVER_SERVICE_TYPE_QUEST		((BYTE)0)	// ����Ʈ ����

struct MSG_FM_SET_SERVICE_STATE
{
	BYTE	ServiceType;	// SERVER_SERVICE_TYPE_XXX
	bool	ServiceState;	// TRUE: Start Service, FALSE: Stop Service
};

// SERVER_INFO_TYPE_XXX
#define SERVER_INFO_TYPE_QUEST			((BYTE)0)	// ����Ʈ

struct MSG_FM_RELOAD_SERVER_INFO
{
	BYTE	InfoType;		// SERVER_INFO_TYPE_XXX
};

struct MSG_FM_INCREASE_CHANNEL
{
	MapIndex_t	MapIndex;
};	// M->F, Channel �� ����

struct MSG_FM_INCREASE_CHANNEL_OK
{
	MapIndex_t	MapIndex;
	INT			TotalChannelCount;
};	// F->M, Channel �� ���� ���

struct MSG_FM_SET_CHANNEL_STATE
{
	MAP_CHANNEL_INDEX	MapChannelIndex;
	bool				EnableChannel;		// TRUE: Enable, FALSE: Disable
};		// M->F, Channel Enable/Disable

struct MSG_FM_SET_CHANNEL_STATE_OK
{
	MAP_CHANNEL_INDEX	MapChannelIndex;
	bool				IsEnabled;			// TRUE: Enable, FALSE: Disable
};	// F->M, Channel Enable/Disable ���
struct MSG_FM_CITYWAR_CHANGE_WAR_TIME
{
	MAP_CHANNEL_INDEX	MapChannelIndex;
	ATUM_DATE_TIME		CityWarATimeDefault;
	ATUM_DATE_TIME		CityWarATimeSetting;
};
struct MSG_FM_CITYWAR_CHANGED_OCCUPY_INFO
{
	MAP_CHANNEL_INDEX	MapChannelIndex;
	SCITY_OCCUPY_INFO	cityWarOccupyInfo;
};
///////////////////////////////////////////////////////////////////////////////
// PRE Server Monitor
//#define T1_PM_GET_NUM_CLIENTS			0x00	// No Body
//#define T1_PM_GET_NUM_CLIENTS_OK		0x01	// # of client
//#define T1_PM_SHUTDOWN					0x02	// No Body
//#define T1_PM_SHUTDOWN_OK				0x03	// No Body
//#define T1_PM_CLOSE						0x04	// No Body
//#define T1_PM_CLOSE_OK					0x05	// No Body
//#define T1_PM_PING						0x06	// No Body
//#define T1_PM_PING_OK					0x07	// No Body
//#define T1_PM_RELOAD_VERSION_INFO		0x08	// M->P
//#define T1_PM_RELOAD_VERSION_INFO_OK	0x09	// No Body
//#define T1_PM_CHANGE_BANDWIDTH_FLAG		0x0A	// No Body
//#define T1_PM_CHANGE_BANDWIDTH_FLAG_OK	0x0B	// ����� �÷��װ�
//#define T1_PM_SET_MSG_PRINT_LEVEL		0x0C
//#define T1_PM_PAUSE_SERVICE				0x0D
//#define T1_PM_PAUSE_SERVICE_OK			0x0E
//#define T1_PM_START_SERVICE				0x0F
//#define T1_PM_START_SERVICE_OK			0x10
//#define T1_PM_GET_SERVER_GROUP_INFO		0x11
//#define T1_PM_GET_SERVER_GROUP_INFO_ACK	0x12
//#define T1_PM_SET_LIMIT_GROUP_USER_COUNTS	0x13
//#define T1_PM_SERVER_THREAD_STATUS			0x14
//#define T1_PM_RELOAD_BLOCKED_ACCOUNTS		0x15	// No Body
//#define T1_PM_RELOAD_BLOCKED_ACCOUNTS_OK	0x16	// No Body
//#define T1_PM_SET_MGAME_EVENT_TYPE			0x17
//#define T1_PM_CHANGE_ENABLE_SERVER_GROUP	0x18
//#define T1_PM_PREPARE_SHUTDOWN				0x19	// 2006-08-04 by cmkwon, 

//#define T1_PM_AUTO_UPDATE_FTP_SERVER_SETTING		0x20	// 2007-01-06 by cmkwon
//#define T1_PM_AUTO_UPDATE_HTTP_SERVER_SETTING		0x21	// 2007-01-06 by cmkwon
//#define T1_PM_RELOAD_VERSION_INFO_HEADER			0x22	// M->P
//#define T1_PM_RELOAD_VERSION_INFO_LIST				0x23	// M->P
//#define T1_PM_RELOAD_VERSION_INFO_DONE				0x24	// M->P

//#define T_PM_GET_NUM_CLIENTS			(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_GET_NUM_CLIENTS)			// No Body
//#define T_PM_GET_NUM_CLIENTS_OK			(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_GET_NUM_CLIENTS_OK)
//#define T_PM_SHUTDOWN					(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_SHUTDOWN)					// No Body
//#define T_PM_SHUTDOWN_OK				(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_SHUTDOWN_OK)				// No Body
//#define T_PM_CLOSE						(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_CLOSE)						// No Body
//#define T_PM_CLOSE_OK					(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_CLOSE_OK)					// No Body
//#define T_PM_PING						(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_PING)						// No Body
//#define T_PM_PING_OK					(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_PING_OK)					// No Body
//#define T_PM_RELOAD_VERSION_INFO		(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_RELOAD_VERSION_INFO)		// M->P
//#define T_PM_RELOAD_VERSION_INFO_OK		(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_RELOAD_VERSION_INFO_OK)	// No Body
//#define T_PM_CHANGE_BANDWIDTH_FLAG		(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_CHANGE_BANDWIDTH_FLAG)		// No Body
//#define T_PM_CHANGE_BANDWIDTH_FLAG_OK	(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_CHANGE_BANDWIDTH_FLAG_OK)
//#define T_PM_SET_MSG_PRINT_LEVEL		(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_SET_MSG_PRINT_LEVEL)
//#define T_PM_PAUSE_SERVICE				(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_PAUSE_SERVICE)
//#define T_PM_PAUSE_SERVICE_OK			(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_PAUSE_SERVICE_OK)
//#define T_PM_START_SERVICE				(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_START_SERVICE)
//#define T_PM_START_SERVICE_OK			(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_START_SERVICE_OK)
//#define T_PM_GET_SERVER_GROUP_INFO		(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_GET_SERVER_GROUP_INFO)
//#define T_PM_GET_SERVER_GROUP_INFO_ACK	(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_GET_SERVER_GROUP_INFO_ACK)
//#define T_PM_SET_LIMIT_GROUP_USER_COUNTS	(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_SET_LIMIT_GROUP_USER_COUNTS)
//#define T_PM_SERVER_THREAD_STATUS		(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_SERVER_THREAD_STATUS)
//#define T_PM_RELOAD_BLOCKED_ACCOUNTS	(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_RELOAD_BLOCKED_ACCOUNTS)		// No Body
//#define T_PM_RELOAD_BLOCKED_ACCOUNTS_OK	(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_RELOAD_BLOCKED_ACCOUNTS_OK)	// No Body
//#define T_PM_SET_MGAME_EVENT_TYPE		(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_SET_MGAME_EVENT_TYPE)
//#define T_PM_CHANGE_ENABLE_SERVER_GROUP	(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_CHANGE_ENABLE_SERVER_GROUP)
//#define T_PM_PREPARE_SHUTDOWN			(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_PREPARE_SHUTDOWN)		// 2006-08-04 by cmkwon

//#define T_PM_AUTO_UPDATE_FTP_SERVER_SETTING			(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_AUTO_UPDATE_FTP_SERVER_SETTING)	// 2007-01-06 by cmkwon, P->M
//#define T_PM_AUTO_UPDATE_HTTP_SERVER_SETTING		(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_AUTO_UPDATE_HTTP_SERVER_SETTING)	// 2007-01-06 by cmkwon, P->M
//#define T_PM_RELOAD_VERSION_INFO_HEADER				(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_RELOAD_VERSION_INFO_HEADER)		// 2007-01-09 by cmkwon, M->P
//#define T_PM_RELOAD_VERSION_INFO_LIST				(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_RELOAD_VERSION_INFO_LIST)			// 2007-01-09 by cmkwon, M->P
//#define T_PM_RELOAD_VERSION_INFO_DONE				(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_RELOAD_VERSION_INFO_DONE)			// 2007-01-09 by cmkwon, M->P

struct MSG_PM_GET_SERVER_GROUP_INFO
{
	char		ServerGroupName[SIZE_MAX_SERVER_NAME];
};

struct MSG_PM_GET_SERVER_GROUP_INFO_ACK
{
	char		ServerGroupName[SIZE_MAX_SERVER_NAME];
	bool		bEnableServerGroup;
	SERVER_ID	IMServerID;
	UINT		AccumulatedGroupUserCounts;
	UINT		CurrentGroupUserCounts;
	UINT		MaxGroupUserCounts;
	UINT		LimitGroupUserCounts;
};

struct MSG_PM_SET_LIMIT_GROUP_USER_COUNTS
{
	char		ServerGroupName[SIZE_MAX_SERVER_NAME];
	UINT		LimitGroupUserCounts;
};

using MSG_PM_SERVER_THREAD_STATUS = MSG_FM_SERVER_THREAD_STATUS;

//struct MSG_PM_SET_MGAME_EVENT_TYPE
//{
//	MGAME_EVENT_t		enMGameEvent;
//};

struct MSG_PM_CHANGE_ENABLE_SERVER_GROUP
{
	char	ServerGroupName[SIZE_MAX_SERVER_NAME];
	bool	bEnableServerGroup;
};

struct MSG_PM_PREPARE_SHUTDOWN				// 2006-08-04 by cmkwon
{
	char	ServerGroupName[SIZE_MAX_SERVER_NAME];
};

struct MSG_PM_AUTO_UPDATE_HTTP_SERVER_SETTING		// 2007-01-06 by cmkwon, Http Auto Update server ���� �߰�
{						// 2007-01-06 by cmkwon, AU_SERVER_TYPE_XXX
	char		szDownloadHttpServerIP[SIZE_MAX_FTP_URL];
	USHORT		nDownloadHttpServerPort;
	char		szDownloadHttpServerAccountName[SIZE_MAX_ACCOUNT_NAME];
	char		szDownloadHttpServerPassword[SIZE_MAX_PASSWORD];
	char		szClientHttpUpdateDownloadDir[SIZE_MAX_FTP_FILE_PATH];
	char		szLauncherFileDownloadHttpPath[SIZE_MAX_FTP_FILE_PATH];
	char		szDeleteFileListDownloadHttpPath[SIZE_MAX_FTP_FILE_PATH];
	char		szNoticeFileDownloadHttpPath[SIZE_MAX_FTP_FILE_PATH];
	char		szVersionListFileDownloadHttpPath[SIZE_MAX_FTP_FILE_PATH];
};

struct MSG_PM_RELOAD_VERSION_INFO_HEADER			// 2007-01-09 by cmkwon
{
	USHORT	LauncherVersion[4];
	USHORT	DeleteFileListVersion[4];
	USHORT	NoticeVersion[4];
};

struct MSG_PM_RELOAD_VERSION_INFO_LIST				// 2007-01-09 by cmkwon
{
	INT		NumOfClientVersions;
	ARRAY_(USHORT[8]);
};


//////////////////////////////////
// NPC Server Monitor
//#define T1_NM_GET_NUM_MONSTERS				0x00	// No Body
//#define T1_NM_GET_NUM_MONSTERS_OK			0x01
//#define T1_NM_SHUTDOWN						0x02	// No Body
//#define T1_NM_SHUTDOWN_OK					0x03	// No Body
//#define T1_NM_PING							0x04	// No Body
//#define T1_NM_PING_OK						0x05	// No Body
//#define T1_NM_PRINTMAP						0x06	// No Body
//#define T1_NM_SAVE_MAPBLOCK_INFO			0x07	// No Body
//#define T1_NM_SEND_CHANNEL_INFO				0x08	//
//#define T1_NM_CHANGE_BANDWIDTH_FLAG			0x0A	// No Body
//#define T1_NM_CHANGE_BANDWIDTH_FLAG_OK		0x0B	// ����� �÷��װ�
//#define T1_NM_SET_MSG_PRINT_LEVEL			0x0C
//#define T1_NM_GET_MAP_USER_COUNTS			0x0D	// No Body
//#define T1_NM_GET_MAP_USER_COUNTS_ACK		0x0E
//#define T1_NM_SET_LIMIT_MONSTER_COUNT		0x0F
//#define T1_NM_SET_LIMIT_MONSTER_COUNT_ACK	0x10
//#define T1_NM_SERVER_THREAD_STATUS			0x11
//#define T1_NM_SET_CHANNEL_STATE_OK			0x12	// N->M, Channel Enable/Disable ���

//#define T_NM_GET_NUM_MONSTERS				(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_GET_NUM_MONSTERS)
//#define T_NM_GET_NUM_MONSTERS_OK			(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_GET_NUM_MONSTERS_OK)
//#define T_NM_SHUTDOWN						(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_SHUTDOWN)
//#define T_NM_SHUTDOWN_OK					(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_SHUTDOWN_OK)
//#define T_NM_PING							(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_PING)
//#define T_NM_PING_OK						(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_PING_OK)
//#define T_NM_PRINTMAP						(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_PRINTMAP)
//#define T_NM_SAVE_MAPBLOCK_INFO				(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_SAVE_MAPBLOCK_INFO)
//#define T_NM_SEND_CHANNEL_INFO				(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_SEND_CHANNEL_INFO)
//#define T_NM_CHANGE_BANDWIDTH_FLAG			(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_CHANGE_BANDWIDTH_FLAG)
//#define T_NM_CHANGE_BANDWIDTH_FLAG_OK		(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_CHANGE_BANDWIDTH_FLAG_OK)
//#define T_NM_SET_MSG_PRINT_LEVEL			(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_SET_MSG_PRINT_LEVEL)
//#define T_NM_GET_MAP_USER_COUNTS			(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_GET_MAP_USER_COUNTS)
//#define T_NM_GET_MAP_USER_COUNTS_ACK		(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_GET_MAP_USER_COUNTS_ACK)
//#define T_NM_SET_LIMIT_MONSTER_COUNT		(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_SET_LIMIT_MONSTER_COUNT)
//#define T_NM_SET_LIMIT_MONSTER_COUNT_ACK	(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_SET_LIMIT_MONSTER_COUNT_ACK)
//#define T_NM_SERVER_THREAD_STATUS			(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_SERVER_THREAD_STATUS)
//#define T_NM_SET_CHANNEL_STATE_OK			(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_SET_CHANNEL_STATE_OK)	// N->M, Channel Enable/Disable ���

struct MSG_NM_GET_NUM_MONSTERS_OK
{
	USHORT	NumOfMonsters;
};

using MSG_NM_GET_MAP_USER_COUNTS = MSG_FM_GET_MAP_USER_COUNTS;

using MSG_NM_GET_MAP_USER_COUNTS_ACK = MSG_FM_GET_MAP_USER_COUNTS_ACK;

struct MSG_NM_SET_LIMIT_MONSTER_COUNT
{
	MAP_CHANNEL_INDEX	MapChannelIndex;
	UINT		SetLimitMonsterCount;
};

using MSG_NM_SET_LIMIT_MONSTER_COUNT_ACK = MSG_NM_SET_LIMIT_MONSTER_COUNT;
using MSG_NM_SERVER_THREAD_STATUS = MSG_FM_SERVER_THREAD_STATUS;
using MSG_NM_SET_CHANNEL_STATE_OK = MSG_FM_SET_CHANNEL_STATE_OK;

//////////////////////////////////////////////////////////////////////////
// IM Server Monitor
//#define T1_IM_GET_NUM_CLIENTS			0x00	// No Body
//#define T1_IM_GET_NUM_CLIENTS_OK		0x01
//#define T1_IM_SHUTDOWN					0x02	// No Body
//#define T1_IM_SHUTDOWN_OK				0x03	// No Body
//#define T1_IM_PING						0x04	// No Body
//#define T1_IM_PING_OK					0x05	// No Body
//#define T1_IM_PRINTMAP					0x06	// No Body
//#define T1_IM_PRINT_DEBUG_MSG			0x08	// No Body
//#define T1_IM_CHANGE_BANDWIDTH_FLAG		0x0A	// No Body
//#define T1_IM_CHANGE_BANDWIDTH_FLAG_OK	0x0B	// ����� �÷��װ�
//#define T1_IM_SET_MSG_PRINT_LEVEL		0x0C
//#define T1_IM_GET_TOTAL_USER_COUNTS		0x0D	// No Body
//#define T1_IM_GET_TOTAL_USER_COUNTS_ACK	0x0E
//#define T1_IM_SERVER_THREAD_STATUS		0x0F

//#define T_IM_GET_NUM_CLIENTS			(MessageType_t)((T0_IM_MONITOR<<8)|T1_IM_GET_NUM_CLIENTS)
//#define T_IM_GET_NUM_CLIENTS_OK			(MessageType_t)((T0_IM_MONITOR<<8)|T1_IM_GET_NUM_CLIENTS_OK)
//#define T_IM_SHUTDOWN					(MessageType_t)((T0_IM_MONITOR<<8)|T1_IM_SHUTDOWN)
//#define T_IM_SHUTDOWN_OK				(MessageType_t)((T0_IM_MONITOR<<8)|T1_IM_SHUTDOWN_OK)
//#define T_IM_PING						(MessageType_t)((T0_IM_MONITOR<<8)|T1_IM_PING)
//#define T_IM_PING_OK					(MessageType_t)((T0_IM_MONITOR<<8)|T1_IM_PING_OK)
//#define T_IM_PRINTMAP					(MessageType_t)((T0_IM_MONITOR<<8)|T1_IM_PRINTMAP)
//#define T_IM_PRINT_DEBUG_MSG			(MessageType_t)((T0_IM_MONITOR<<8)|T1_IM_PRINT_DEBUG_MSG)
//#define T_IM_CHANGE_BANDWIDTH_FLAG		(MessageType_t)((T0_IM_MONITOR<<8)|T1_IM_CHANGE_BANDWIDTH_FLAG)
//#define T_IM_CHANGE_BANDWIDTH_FLAG_OK	(MessageType_t)((T0_IM_MONITOR<<8)|T1_IM_CHANGE_BANDWIDTH_FLAG_OK)
//#define T_IM_SET_MSG_PRINT_LEVEL		(MessageType_t)((T0_IM_MONITOR<<8)|T1_IM_SET_MSG_PRINT_LEVEL)
//#define T_IM_GET_TOTAL_USER_COUNTS		(MessageType_t)((T0_IM_MONITOR<<8)|T1_IM_GET_TOTAL_USER_COUNTS)
//#define T_IM_GET_TOTAL_USER_COUNTS_ACK	(MessageType_t)((T0_IM_MONITOR<<8)|T1_IM_GET_TOTAL_USER_COUNTS_ACK)
//#define T_IM_SERVER_THREAD_STATUS		(MessageType_t)((T0_IM_MONITOR<<8)|T1_IM_SERVER_THREAD_STATUS)

struct MSG_FIELDSERVER_USERCOUNTS
{
	char	FieldIP[SIZE_MAX_IPADDRESS];
	USHORT	FieldListenPort;
	UINT	AccumulatedFieldUserCounts;
	UINT	CurrentFieldUserCounts;
	UINT	MaxFieldUserCounts;
	UINT	AccumulatedFieldMonsterCounts;
	UINT	CurrentFieldMonsterCounts;
};

struct MSG_IM_GET_TOTAL_USER_COUNTS_ACK
{
	UINT	AccumulatedTotalUserCounts;
	UINT	CurrentTotalUserCounts;
	UINT	MaxTotalUserCounts;
};

using MSG_IM_SERVER_THREAD_STATUS = MSG_FM_SERVER_THREAD_STATUS;

//////////////////////////////////////////////////////////////////////////
// PreServer <-> admin tool 
//#define T1_PA_ADMIN_CONNECT				0x00
//#define T1_PA_ADMIN_CONNECT_OK			0x01
//#define T1_PA_ADMIN_CHANGE_SERVER_STATE	0x02
//#define T1_PA_ADMIN_GET_ACCOUNT_INFO	0x03
//#define T1_PA_ADMIN_GET_ACCOUNT_INFO_OK	0x04
//#define T1_PA_ADMIN_DISCONNECT_USER		0x05
//#define T1_PA_ADMIN_DISCONNECT_USER_OK	0x06
//#define T1_PA_ADMIN_BLOCK_ACCOUNT		0x07
//#define T1_PA_ADMIN_BLOCK_ACCOUNT_OK	0x08
//#define T1_PA_ADMIN_UNBLOCK_ACCOUNT		0x09
//#define T1_PA_ADMIN_UNBLOCK_ACCOUNT_OK	0x0A
//#define T1_PA_ADMIN_GET_SERVER_INFO		0x0B
//#define T1_PA_ADMIN_GET_SERVER_INFO_OK	0x0C
//#define T1_PA_ADMIN_GET_ACCOUNT_LIST	0x0D
//#define T1_PA_ADMIN_GET_ACCOUNT_LIST_OK	0x0E
//#define T1_PA_ADMIN_RELOAD_HAPPYEV		0x0F		// A->P, No body, 2006-08-28 by cmkwon
//#define T1_PA_ADMIN_RELOAD_ITEMEV		0x10		// A->P, No body, 2006-08-31 by dhjin
//#define T1_PA_ADMIN_UPDATE_PCBANGLIST	0x11		// A->P, No body, 2007-01-22 by dhjin
//#define T1_PA_ADMIN_UPDATE_STRATEGYPOINT_NOTSUMMONTIME	0x12		// A->P, No body, 2007-03-06 by dhjin
//#define T1_PA_ADMIN_PETITION_SET_PERIOD	0x13			// 2007-11-19 by cmkwon, �����ý��� ������Ʈ - A->P
//#define T1_PA_ADMIN_SET_DBSERVER_GROUP		0x14			// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - A->P
//#define T1_PA_ADMIN_SET_DBSERVER_GROUP_OK	0x15			// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - P->A
//#define T1_PA_ADMIN_RELOAD_ADMIN_NOTICE_SYSTEM	0x16		// 2009-01-14 by cmkwon, ��� �ڵ� ���� �ý��� ���� - A->P, �� ������ �ڵ� ���� ���ε�
//#define T1_PA_ADMIN_RELOAD_WORLDRANKING			0x17		// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - A->P, 
//#define T1_PA_ADMIN_RELOAD_INFLUENCERATE		0x18		// A->P // 2009-09-16 by cmkwon, ���� �ʱ�ȭ�� ���¡ ���� ���� - 

// 2012-06-07 by hskim, ������ ���� ���� ��ȭ - ���� ���� ����� �α� ����
//#define T1_PA_ADMIN_LOG_ACCOUNT_ACCOUNTTYPE_CHANGE					0x1B		
//#define T1_PA_ADMIN_LOG_CHARACTER_RACEACCOUNTTYPE_CHANGE			0x1C
// end 2012-06-07 by hskim, ������ ���� ���� ��ȭ - ���� ���� ����� �α� ����
//#define T1_PA_ADMIN_STRATRGYPOINT_INFO_CHANGE						0x1D		// 2012-11-13 by jhseol, ���� �ý��� ������ - ������, ������ ���� �ǽð� ����

//#define T_PA_ADMIN_CONNECT				(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_CONNECT)				// A->P
//#define T_PA_ADMIN_CONNECT_OK			(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_CONNECT_OK)			// P->A
//#define T_PA_ADMIN_CHANGE_SERVER_STATE	(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_CHANGE_SERVER_STATE)	// P->A
//#define T_PA_ADMIN_GET_ACCOUNT_INFO		(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_GET_ACCOUNT_INFO)		// A->P
//#define T_PA_ADMIN_GET_ACCOUNT_INFO_OK	(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_GET_ACCOUNT_INFO_OK)	// P->A
//#define T_PA_ADMIN_DISCONNECT_USER		(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_DISCONNECT_USER)		// A->P
//#define T_PA_ADMIN_DISCONNECT_USER_OK	(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_DISCONNECT_USER_OK)	// P->A
//#define T_PA_ADMIN_BLOCK_ACCOUNT		(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_BLOCK_ACCOUNT)			// A->P
//#define T_PA_ADMIN_BLOCK_ACCOUNT_OK		(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_BLOCK_ACCOUNT_OK)		// P->A
//#define T_PA_ADMIN_UNBLOCK_ACCOUNT		(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_UNBLOCK_ACCOUNT)		// A->P
//#define T_PA_ADMIN_UNBLOCK_ACCOUNT_OK	(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_UNBLOCK_ACCOUNT_OK)	// P->A
//#define T_PA_ADMIN_GET_SERVER_INFO		(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_GET_SERVER_INFO)		// A->P, no body
//#define T_PA_ADMIN_GET_SERVER_INFO_OK	(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_GET_SERVER_INFO_OK)	// P->A
//#define T_PA_ADMIN_GET_ACCOUNT_LIST		(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_GET_ACCOUNT_LIST)		// A->P
//#define T_PA_ADMIN_GET_ACCOUNT_LIST_OK	(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_GET_ACCOUNT_LIST_OK)	// P->A
//#define T_PA_ADMIN_RELOAD_HAPPYEV		(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_RELOAD_HAPPYEV)		// A->P, No body, 2006-08-28 by cmkwon
//#define T_PA_ADMIN_RELOAD_ITEMEV		(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_RELOAD_ITEMEV)			// A->P, No body, 2006-08-31 by dhjin
//#define T_PA_ADMIN_UPDATE_PCBANGLIST	(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_UPDATE_PCBANGLIST)		// A->P, No body, 2007-01-22 by dhjin
//#define T_PA_ADMIN_UPDATE_STRATEGYPOINT_NOTSUMMONTIME	(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_UPDATE_STRATEGYPOINT_NOTSUMMONTIME)		// A->P, No body, 2007-01-22 by dhjin
//#define T_PA_ADMIN_PETITION_SET_PERIOD	(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_PETITION_SET_PERIOD)	// 2007-11-19 by cmkwon, �����ý��� ������Ʈ - A->P
//#define T_PA_ADMIN_SET_DBSERVER_GROUP		(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_SET_DBSERVER_GROUP)	// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - A->P
//#define T_PA_ADMIN_SET_DBSERVER_GROUP_OK	(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_SET_DBSERVER_GROUP_OK)	// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - P->A
//#define T_PA_ADMIN_RELOAD_ADMIN_NOTICE_SYSTEM	(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_RELOAD_ADMIN_NOTICE_SYSTEM)	// 2009-01-14 by cmkwon, ��� �ڵ� ���� �ý��� ���� - A->P, �� ������ �ڵ� ���� ���ε�
//#define T_PA_ADMIN_RELOAD_WORLDRANKING			(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_RELOAD_WORLDRANKING)			// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - A->P, 
//#define T_PA_ADMIN_RELOAD_INFLUENCERATE			(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_RELOAD_INFLUENCERATE)		// A->P // 2009-09-16 by cmkwon, ���� �ʱ�ȭ�� ���¡ ���� ���� - 

// 2012-06-07 by hskim, ������ ���� ���� ��ȭ - ���� ���� ����� �α� ����
//#define T_PA_ADMIN_LOG_ACCOUNT_ACCOUNTTYPE_CHANGE			(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_LOG_ACCOUNT_ACCOUNTTYPE_CHANGE)		// A->P
//#define T_PA_ADMIN_LOG_CHARACTER_RACEACCOUNTTYPE_CHANGE		(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_LOG_CHARACTER_RACEACCOUNTTYPE_CHANGE)	// A->P
// end 2012-06-07 by hskim, ������ ���� ���� ��ȭ - ���� ���� ����� �α� ����
//#define T_PA_ADMIN_STRATRGYPOINT_INFO_CHANGE				(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_STRATRGYPOINT_INFO_CHANGE)	// A->P	// 2012-11-13 by jhseol, ���� �ý��� ������ - ������

struct MSG_PA_ADMIN_CONNECT
{
	char	UID[SIZE_MAX_ACCOUNT_NAME];
	char	PWD[SIZE_MAX_PASSWORD];
	DWORD	Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, ���� ���� ���� - ���� ������ ȣȯ �ȵǵ��� ����ü ũ�� �ٲ�
};

struct MSG_PA_ADMIN_CONNECT_OK
{
	// 2006-04-15 by cmkwon, �ʵ� ����
	//	BYTE	AuthOK;				// 0: FALSE, 1:TRUE
	USHORT	AccountType0;		// ���� ����
	//DWORD	Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, ���� ���� ���� - ���� ������ ȣȯ �ȵǵ��� ����ü ũ�� �ٲ�
	//int		IsMasang;	// 2012-06-16 by jhseol, �Ʒ��� �߰�����part2 - ���� : ���� ���ΰ� �ƴϸ� ��� �Ұ�
};

struct MSG_PA_ADMIN_CHANGE_SERVER_STATE
{
	SERVER_ID	ServerID;
	INT			ServerType;		// ENServerType, ST_XXX
	BYTE		ServerState;	// SERVER_STATE_XXX
};

struct MSG_PA_ADMIN_GET_ACCOUNT_INFO
{
	char	AccountName[SIZE_MAX_ACCOUNT_NAME];
	char	CharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct CAccountInfo;

struct MEX_ACCOUNT_INFO_PRE_SERVER
{
	MEX_ACCOUNT_INFO_PRE_SERVER()
	{
		memset(this, 0x00, sizeof(MEX_ACCOUNT_INFO_PRE_SERVER));
	}

	char				AccountName[SIZE_MAX_ACCOUNT_NAME];
	UID32_t				AccountUniqueNumber;
	char				PublicClientIP[SIZE_MAX_IPADDRESS];
	char				PrivateClientIP[SIZE_MAX_IPADDRESS];
	char				CurrentCharacterName[SIZE_MAX_CHARACTER_NAME];
	UID32_t				CurrentCharacterUniqueNumber;
	char				CurrentServerGroup[SIZE_MAX_SERVER_NAME];
	SERVER_ID			CurrentFieldServerID;
	MAP_CHANNEL_INDEX	CurrentMapChannel;
	INT					ClientState;

#ifdef _ATUM_PRE_SERVER
	// operator overloading
	MEX_ACCOUNT_INFO_PRE_SERVER& operator=(const CAccountInfo& rhs);
#endif // _ATUM_IM_SERVER
};

struct MSG_PA_ADMIN_GET_ACCOUNT_INFO_OK
{
	BYTE	IsOnline;			// 0: FALSE, 1:TRUE
	BYTE	IsBlocked;			// 0: FALSE, 1:TRUE
	SBLOCKED_ACCOUNT_INFO		BlockedAccountInfo;
	MEX_ACCOUNT_INFO_PRE_SERVER	AccountInfo;
};

struct MSG_PA_ADMIN_DISCONNECT_USER
{
	char	AccountName[SIZE_MAX_ACCOUNT_NAME];
	char	CharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_PA_ADMIN_DISCONNECT_USER_OK
{
	char	AccountName[SIZE_MAX_ACCOUNT_NAME];
};

using MSG_PA_ADMIN_BLOCK_ACCOUNT = SBLOCKED_ACCOUNT_INFO;

using MSG_PA_ADMIN_BLOCK_ACCOUNT_OK = SBLOCKED_ACCOUNT_INFO;


// typedef struct
// {
// 	char	AccountName[SIZE_MAX_ACCOUNT_NAME];
// } MSG_PA_ADMIN_UNBLOCK_ACCOUNT;
using MSG_PA_ADMIN_UNBLOCK_ACCOUNT = SBLOCKED_ACCOUNT_INFO;	// 2008-01-30 by cmkwon, ���� �� �α� ����� ���� - 

struct MSG_PA_ADMIN_UNBLOCK_ACCOUNT_OK
{
	char	AccountName[SIZE_MAX_ACCOUNT_NAME];
};

// 2012-06-07 by hskim, ������ ���� ���� ��ȭ - ���� ���� ����� �α� ����
struct MSG_PA_ADMIN_LOG_ACCOUNT_ACCOUNTTYPE_CHANGE
{
	char	AccountName[SIZE_MAX_ACCOUNT_NAME];
	UID32_t	AccountUniqueNumber;
	USHORT	OldAccountType;
	USHORT	NewAccountType;
};

struct MSG_PA_ADMIN_LOG_CHARACTER_RACEACCOUNTTYPE_CHANGE
{
	char	AccountName[SIZE_MAX_ACCOUNT_NAME];
	char	CharacterName[SIZE_MAX_CHARACTER_NAME];
	UID32_t	AccountUniqueNumber;
	UID32_t	CharacterUniqueNumber;
	USHORT	OldAccountType;
	USHORT	NewAccountType;
};
// end 2012-06-07 by hskim, ������ ���� ���� ��ȭ - ���� ���� ����� �α� ����

//typedef struct
//{
//} MSG_PA_ADMIN_GET_SERVER_INFO;

#define SERVER_STATE_NOT_ACTIVE	((BYTE)0)
#define SERVER_STATE_NORMAL		((BYTE)1)
#define SERVER_STATE_ABNORMAL	((BYTE)2)

// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - MEX_SERVER_GROUP_INFO_FOR_ADMIN �� ��ü ��
// struct MEX_SERVER_INFO_FOR_ADMIN
// {
// 	char		ServerName[SIZE_MAX_SERVER_NAME];
// 	INT			ServerType;							// ENServerType, ST_XXX
// 	SERVER_ID	ServerID;
// 	INT			UserCount;						
// 	BYTE		ServerState;						// SERVER_STATE_XXX
// };

struct MEX_SERVER_GROUP_INFO_FOR_ADMIN				// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - 
{
	char		ServerName[SIZE_MAX_SERVER_NAME];
	SERVER_ID	IMServerID;
	INT			UserCount;
	BYTE		IMServerState;						// SERVER_STATE_XXX
	SERVER_ID	FieldServerID;
	BYTE		FieldServerState;					// SERVER_STATE_XXX
	int			ServerGroupID;						// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - 
	int			LimitUserCount;						// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - 
	bool		LockCreateCharacterForNewAccount;	// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - 
};

struct MSG_PA_ADMIN_GET_SERVER_INFO_OK
{
	// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - 
	//	INT		NumOfServers;
	//	ARRAY_(MEX_SERVER_INFO_FOR_ADMIN);
	INT		NumOfServerGroups;						// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - 
	ARRAY_(MEX_SERVER_GROUP_INFO_FOR_ADMIN);		// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - 
};

struct MSG_PA_ADMIN_GET_ACCOUNT_LIST
{
	char	ServerName[SIZE_MAX_SERVER_NAME];
};

struct MEX_SIMPLE_ACCOUNT_INFO_FOR_ADMIN
{
	char	AccountName[SIZE_MAX_ACCOUNT_NAME];
	char	CharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_PA_ADMIN_GET_ACCOUNT_LIST_OK
{
	size_t		NumOfAccounts;
	ARRAY_(MEX_SIMPLE_ACCOUNT_INFO_FOR_ADMIN);
};


using MSG_PA_ADMIN_PETITION_SET_PERIOD = MSG_IP_ADMIN_PETITION_SET_PERIOD;	// 2007-11-19 by cmkwon, �����ý��� ������Ʈ - 

struct MSG_PA_ADMIN_SET_DBSERVER_GROUP		// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - 
{
	char		ServerGroupName[SIZE_MAX_SERVER_NAME];
	int			LimitUserCount;
	bool		LockCreateCharacterForNewAccount;
};

using MSG_PA_ADMIN_SET_DBSERVER_GROUP_OK = MSG_PA_ADMIN_SET_DBSERVER_GROUP;		// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - 

struct MSG_PA_ADMIN_RELOAD_INFLUENCERATE	// 2009-09-16 by cmkwon, ���� �ʱ�ȭ�� ���¡ ���� ���� - 
{
	char		ServerGroupName[SIZE_MAX_SERVER_NAME];
};

//////////////////////////////////////////////////////////////////////////
// IMServer <-> admin tool 
//#define T1_IA_ADMIN_CONNECT				0x00
//#define T1_IA_ADMIN_CONNECT_OK			0x01
//#define T1_IA_ADMIN_CHANGE_SERVER_STATE	0x02
//#define T1_IA_ADMIN_GET_CHARACTER		0x03
//#define T1_IA_ADMIN_GET_CHARACTER_OK	0x04
//#define T1_IA_ADMIN_CHAT_ALL			0x05
//#define T1_IA_ADMIN_GET_SERVER_INFO		0x06
//#define T1_IA_ADMIN_GET_SERVER_INFO_OK	0x07

//#define T_IA_ADMIN_CONNECT				(MessageType_t)((T0_IA_ADMIN<<8)|T1_IA_ADMIN_CONNECT)				// A->I
//#define T_IA_ADMIN_CONNECT_OK			(MessageType_t)((T0_IA_ADMIN<<8)|T1_IA_ADMIN_CONNECT_OK)			// I->A
//#define T_IA_ADMIN_CHANGE_SERVER_STATE	(MessageType_t)((T0_IA_ADMIN<<8)|T1_IA_ADMIN_CHANGE_SERVER_STATE)	// I->A
//#define T_IA_ADMIN_GET_CHARACTER		(MessageType_t)((T0_IA_ADMIN<<8)|T1_IA_ADMIN_GET_CHARACTER)			// A->I
//#define T_IA_ADMIN_GET_CHARACTER_OK		(MessageType_t)((T0_IA_ADMIN<<8)|T1_IA_ADMIN_GET_CHARACTER_OK)		// I->A
//#define T_IA_ADMIN_CHAT_ALL				(MessageType_t)((T0_IA_ADMIN<<8)|T1_IA_ADMIN_CHAT_ALL)				// A->I
//#define T_IA_ADMIN_GET_SERVER_INFO		(MessageType_t)((T0_IA_ADMIN<<8)|T1_IA_ADMIN_GET_SERVER_INFO)		// A->I, No Body
//#define T_IA_ADMIN_GET_SERVER_INFO_OK	(MessageType_t)((T0_IA_ADMIN<<8)|T1_IA_ADMIN_GET_SERVER_INFO_OK)	// I->A

struct MSG_IA_ADMIN_CONNECT
{
	char	UID[SIZE_MAX_ACCOUNT_NAME];
	char	PWD[SIZE_MAX_PASSWORD];
};

struct MSG_IA_ADMIN_CONNECT_OK
{
	// 2006-04-15 by cmkwon, �ʵ� ����
	//	BYTE	AuthOK;				// 0: FALSE, 1:TRUE
	USHORT	AccountType0;		// ���� ����
};

using MSG_IA_ADMIN_CHANGE_SERVER_STATE = MSG_PA_ADMIN_CHANGE_SERVER_STATE;

struct MSG_IA_ADMIN_GET_CHARACTER
{
	char	CharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_IA_ADMIN_GET_CHARACTER_OK
{
	BYTE	IsOnline;			// 0: FALSE, 1:TRUE
};

using MSG_IA_ADMIN_CHAT_ALL = MSG_IC_CHAT_ALL;

struct MSG_IA_ADMIN_GET_SERVER_INFO_OK
{
	INT			UserCount;
	BYTE		ServerState;	// SERVER_STATE_XXX
};

//////////////////////////////////////////////////////////////////////////
// FieldServer <-> admin tool 
//#define T1_FA_ADMIN_CONNECT				0x00
//#define T1_FA_ADMIN_CONNECT_OK			0x01
//#define T1_FA_ADMIN_CHANGE_SERVER_STATE	0x02

//#define T_FA_ADMIN_CONNECT				(MessageType_t)((T0_FA_ADMIN<<8)|T1_FA_ADMIN_CONNECT)			// A->F
//#define T_FA_ADMIN_CONNECT_OK			(MessageType_t)((T0_FA_ADMIN<<8)|T1_FA_ADMIN_CONNECT_OK)		// F->A
//#define T_FA_ADMIN_CHANGE_SERVER_STATE	(MessageType_t)((T0_FA_ADMIN<<8)|T1_FA_ADMIN_CHANGE_SERVER_STATE)	// F->A

struct MSG_FA_ADMIN_CONNECT
{
	char	UID[SIZE_MAX_ACCOUNT_NAME];
	char	PWD[SIZE_MAX_PASSWORD];
};

struct MSG_FA_ADMIN_CONNECT_OK
{
	// 2006-04-15 by cmkwon, �ʵ� ����
	//	BYTE	AuthOK;				// 0: FALSE, 1:TRUE
	USHORT	AccountType0;		// ���� ����
};

using MSG_FA_ADMIN_CHANGE_SERVER_STATE = MSG_PA_ADMIN_CHANGE_SERVER_STATE;

// IMServer -> Client
using MSG_IC_COUNTDOWN_START = MSG_FC_COUNTDOWN_START;

// Client -> IMServer
using MSG_IC_COUNTDOWN_DONE = MSG_FC_COUNTDOWN_DONE;

//struct MSG_IC_VOIP_SET
//{// 2008-06-17 by dhjin, EP3 VOIP - ����
//	EN_CHECK_TYPE	VoipType;
//};
//
//struct MSG_IC_VOIP_SET_OK
//{// 2008-06-17 by dhjin, EP3 VOIP - ����
//	UID32_t	CharacterUID;
//	EN_CHECK_TYPE	VoipType;
//};

//////////////////////////////////////////////////////////////////////////
// 2008-06-16 by dhjin, EP3 ä�ù� -
struct MSG_IC_CHATROOM_CREATE
{// 2008-06-16 by dhjin, EP3 ä�ù� - ä�ù� ����
	CHAR	ChatRoomName[SIZE_MAX_CHATROOM_NAME];
	bool	ChatRoomLock;
	CHAR	ChatRoomPW[SIZE_MAX_TEAM_PW];
	BYTE	ChatRoomMaxCount;
};

struct MSG_IC_CHATROOM_CREATE_OK
{// 2008-06-16 by dhjin, EP3 ä�ù� - ä�ù� ����
	INT			ChatRoomNum;
};

struct MSG_IC_CHATROOM_LIST_INFO
{// 2008-06-16 by dhjin, EP3 ä�ù� - ä�ù� ����Ʈ 
	INT			StartNum;
};

struct SCHATROOM_LIST_INFO
{// 2008-06-16 by dhjin, EP3 ä�ù� - ä�ù� ����Ʈ  OK
	INT			ChatRoomNum;
	CHAR		ChatRoomName[SIZE_MAX_CHATROOM_NAME];
	bool		ChatRoomLock;
	CHAR		ChatRoomMasterName[SIZE_MAX_CHARACTER_NAME];
	BYTE		Membercount;
	BYTE		MaxMembercount;
};

struct MSG_IC_CHATROOM_LIST_INFO_OK
{// 2008-06-16 by dhjin, EP3 ä�ù� - ä�ù� ����Ʈ  OK
	INT			ChatRoomInfoTotalCount;	// 2008-06-17 by dhjin, ä�ù� �ִ� ���
	INT			ChatRoomInfoListCount;		// 2008-06-17 by dhjin,
	_ARRAY(SCHATROOM_LIST_INFO);
};

struct MSG_IC_CHATROOM_REQUEST_INVITE
{// 2008-06-17 by dhjin, EP3 ä�ù� - ä�ù� �ʴ� 
	INT		ChatRoomNum;
	char	InviteeCharacterName[SIZE_MAX_CHARACTER_NAME];		// �ʴ��� ���� ���̵�
};

struct MSG_IC_CHATROOM_REQUEST_INVITE_QUESTION
{// 2008-06-17 by dhjin, EP3 ä�ù� - ä�ù� �ʴ��ڿ��� ���� ����
	INT		ChatRoomNum;
	char	ChatRoomMasterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_IC_CHATROOM_ACCEPT_INVITE
{// 2008-06-17 by dhjin, EP3 ä�ù� - ä�ù� �ʴ� �¶�
	INT		ChatRoomNum;
};

struct MSG_IC_CHATROOM_ACCEPT_INVITE_OK
{// 2008-06-17 by dhjin, EP3 ä�ù� - ä�ù� �ʴ� �¶� ���� ���� ����
	UID32_t	CharacterUID;
	char	CharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_IC_CHATROOM_JOIN
{// 2008-06-18 by dhjin, EP3 ä�ù� - ä�ù� ���� ����
	INT		ChatRoomNum;
	bool    ChatRoomLock;
	char	ChatRoomPW[SIZE_MAX_TEAM_PW];
};

struct MSG_IC_CHATROOM_REJECT_INVITE
{// 2008-06-18 by dhjin, EP3 ä�ù� - ä�ù� �ʴ� ���
	INT		ChatRoomNum;
};

struct MSG_IC_CHATROOM_LEAVE
{// 2008-06-18 by dhjin, EP3 ä�ù� - ä�ù� ������
	INT		ChatRoomNum;
	char	LeaveCharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_IC_CHATROOM_LEAVE_OK
{// 2008-06-18 by dhjin, EP3 ä�ù� - ä�ù� ������
	UID32_t	CharacterUID;
	char	LeaveCharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_IC_CHATROOM_BAN
{// 2008-06-18 by dhjin, EP3 ä�ù� - ä�ù� �߹� 
	INT		ChatRoomNum;
	UID32_t	BanCharacterUID;
	char	BanCharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_IC_CHATROOM_BAN_OK
{// 2008-06-18 by dhjin, EP3 ä�ù� - ä�ù� �߹� 
	UID32_t	BanCharacterUID;
	char	BanCharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_IC_CHATROOM_CHANGE_NAME
{// 2008-06-18 by dhjin, EP3 ä�ù� - ä�ù� �̸� ���� 
	INT		ChatRoomNum;
	char	ChatRoomName[SIZE_MAX_CHATROOM_NAME];
};

struct MSG_IC_CHATROOM_CHANGE_NAME_OK
{// 2008-06-18 by dhjin, EP3 ä�ù� - ä�ù� �̸� ���� 
	char	ChatRoomName[SIZE_MAX_CHATROOM_NAME];
};

struct MSG_IC_CHATROOM_CHANGE_MASTER
{// 2008-06-18 by dhjin, EP3 ä�ù� - ä�ù� ���� ����
	INT		ChatRoomNum;
	UID32_t	NewCharacterUID;
};

struct MSG_IC_CHATROOM_CHANGE_MASTER_OK
{// 2008-06-18 by dhjin, EP3 ä�ù� - ä�ù� ���� ����
	UID32_t	NewCharacterUID;
	char	CharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_IC_CHATROOM_CHANGE_LOCK_PW
{// 2008-06-18 by dhjin, EP3 ä�ù� - ä�ù� ��� ���� 
	INT		ChatRoomNum;
	bool    ChatRoomLock;
	char	ChatRoomPW[SIZE_MAX_TEAM_PW];
};

struct MSG_IC_CHATROOM_CHANGE_LOCK_PW_OK
{// 2008-06-18 by dhjin, EP3 ä�ù� - ä�ù� ��� ����
	bool    ChatRoomLock;
};

struct MSG_IC_CHATROOM_CHANGE_MAX_MEMBER
{// 2008-06-18 by dhjin, EP3 ä�ù� - ä�ù� �ִ� �ο��� ����
	INT		ChatRoomNum;
	BYTE	ChatRoomMaxMember;
};

struct MSG_IC_CHATROOM_CHANGE_MAX_MEMBER_OK
{// 2008-06-18 by dhjin, EP3 ä�ù� - ä�ù� �ִ� �ο��� ����
	BYTE	ChatRoomMaxMember;
};

struct MSG_IC_CHATROOM_MEMBER_INFO
{// 2008-06-25 by dhjin, EP3 ä�ù� - ä�ù� �ɹ� ����
	INT		ChatRoomNum;
};

struct SCHATROOM_MEMBER_INFO
{/// 2008-06-25 by dhjin, EP3 ä�ù� - ä�ù� �ɹ� ����
	UID32_t		CharacterUID;
	char		CharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_IC_CHATROOM_MEMBER_INFO_OK
{/// 2008-06-25 by dhjin, EP3 ä�ù� - ä�ù� �ɹ� ����
	INT			ChatRoomNum;
	INT			ChatRoomInfoListCount;
	UID32_t		MasterCharacterUID;
	char		ChatRoomName[SIZE_MAX_CHATROOM_NAME];
	bool		ChatRoomLock;
	char		ChatRoomPW[SIZE_MAX_TEAM_PW];
	INT			ChatRoomMaxCount;
	_ARRAY(SCHATROOM_MEMBER_INFO);
};


///////////////////////////////////////////////////////////////////////////////
// T0_FC_RACING
struct MSG_FC_RACING_RACINGLIST_REQUEST_ACK
{
	BYTE		byRacingListCounts;
	ARRAY_(MAP_CHANNEL_INDEX);
};

struct MSG_FC_RACING_RACINGINFO_REQUEST
{
	MAP_CHANNEL_INDEX	mapChannIdx;
};

struct MSG_FC_RACING_RACINGINFO_REQUEST_ACK
{
	MAP_CHANNEL_INDEX	mapChannelIndex;
	BYTE				byMaxUserCounts;				// �ִ� �����ڼ�
	BYTE				byMinUserCounts;				// �ּ� �����ڼ�
	BYTE				byRacingGameNumber;				// ����Ǵ� ���� �ѹ�
	int					nEntryFee;						// ������(����:spi)
	int					nLimitTimeForRacing;			// Racing�� ����Ǵ� ���ѽð�(����:��)
	int					nRamainTimeForJoinRacing;		// ���� ���� �ð����� ���� �ð�(����:��)	
};

struct MSG_FC_RACING_RACINGNOTIFY
{
	MAP_CHANNEL_INDEX	mapChannIdx;
	DWORD				dwRamainTimeForJoinRacing;		// ���� ���� �ð����� ���� �ð�(����:��)
};

struct MSG_FC_RACING_JOIN_ENABLE
{
	MAP_CHANNEL_INDEX	mapChannIdx;
};

struct MSG_FC_RACING_JOIN_REQUEST
{
	MAP_CHANNEL_INDEX	mapChannIdx;
};

struct MSG_FC_RACING_JOIN_REQUEST_ACK
{
	BYTE				byJoinedFlag : 1;					// 1 bit ���, ���̽̿� ���� �Ǿ������� �÷���
	BYTE				byJoinedOrder : 7;				// 7 bit ���, ���̽̿� ������ ����(ex, 8���̸� 1���� 8����)
	MAP_CHANNEL_INDEX	mapChannIdx;
};

struct MSG_FC_RACING_COUNTDOWN
{
	BYTE				byCountDowns;
};

struct MSG_FC_RACING_CHECKPOINT_CHECK
{
	BYTE				byCheckPointIndex;
	DWORD				dwObjectType;
	AVECTOR3			PositionAVector3;
};

struct MSG_FC_RACING_CHECKPOINT_CHECK_ACK
{
	BYTE				byCheckedFlag : 1;
	BYTE				byCheckPointIndex : 7;
	BYTE				byRanking;
};

struct MSG_FC_RACING_FINISHED
{
	BYTE				byRanking;
};

struct MSG_FC_RACING_OTHER_FINISHED
{
	char				szCharacterName[SIZE_MAX_CHARACTER_NAME];
	BYTE				byRanking;
};

struct RankingInfo
{
	char				szCharacterName[SIZE_MAX_CHARACTER_NAME];
	BYTE				byRanking;
	DWORD				dwElapsedTime;
};

struct MSG_FC_RACING_FINALRANKING
{
	BYTE				byRankingInfoCounts;		// Ranking ���� ī��Ʈ
	ARRAY_(RankingInfo);
};

//////////////////////////////////////////////////////////////////////////
// 2007-04-17 by dhjin
// T0_FC_ARENA
//#define T_FC_ARENA_PLAY_CHARACTER_TYPE		(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_PLAY_CHARACTER_TYPE)			// c->f(a)	// 2012-06-08 by jhseol, �Ʒ��� �߰�����part2 - �ɸ��� : Ÿ�� ����
//#define T_FC_ARENA_REQUEST_TEAM				(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_REQUEST_TEAM)					// C->F
//#define T_FC_ARENA_REQUEST_TEAM_OK			(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_REQUEST_TEAM_OK)				// F->C
//#define T_FC_ARENA_CREATE_TEAM				(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_CREATE_TEAM)					// C->F
//#define T_FC_ARENA_CREATE_TEAM_OK			(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_CREATE_TEAM_OK)				// F->C
//#define T_FC_ARENA_ENTER_TEAM				(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_ENTER_TEAM)					// C->F
//#define T_FC_ARENA_ENTER_TEAM_OK			(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_ENTER_TEAM_OK)					// F->C
//#define T_FC_ARENA_TEAM_MEMBER_LIST			(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_TEAM_MEMBER_LIST)				// F->C(n)
//#define T_FC_ARENA_REAVE_TEAM				(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_REAVE_TEAM)					// C->F
//#define T_FC_ARENA_REAVE_TEAM_OK			(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_REAVE_TEAM_OK)					// F->C
//#define T_FC_ARENA_TEAM_READY				(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_TEAM_READY)					// F->C(n)
//#define T_FC_ARENA_TEAM_READY_FINISH		(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_TEAM_READY_FINISH)				// C->F
//#define T_FC_ARENA_TEAM_READY_FINISH_CANCEL	(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_TEAM_READY_FINISH_CANCEL)		// C->F
//#define T_FC_ARENA_TEAM_MATCHING			(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_TEAM_MATCHING)					// F->C(n)
//#define T_FC_ARENA_ENTER_ROOM				(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_ENTER_ROOM)					// F->C(n)
//#define T_FC_ARENA_ENTER_ROOM_WARP			(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_ENTER_ROOM_WARP)				// C->F
//#define T_FC_ARENA_ROOM_WAR_START			(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_ROOM_WAR_START)				// F->C(n)
//#define T_FC_ARENA_ROOM_WAR_INFO			(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_ROOM_WAR_INFO)					// F->C(n)	
//#define T_FC_ARENA_ROOM_WAR_FINISH_HEADER	(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_ROOM_WAR_FINISH_HEADER)		// F->C(n)
//#define T_FC_ARENA_ROOM_WAR_FINISH			(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_ROOM_WAR_FINISH)				// F->C(n)
//#define T_FC_ARENA_ROOM_WAR_FINISH_DONE		(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_ROOM_WAR_FINISH_DONE)			// F->C(n)
//#define T_FC_ARENA_ROOM_WAR_FINISH_DRAW		(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_ROOM_WAR_FINISH_DRAW)				// F->C(n)
//#define T_FC_ARENA_FINISH_WARP				(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_FINISH_WARP)					// C->F
//#define T_FC_ARENA_FINISH_WARP_OK			(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_FINISH_WARP_OK)				// F->C(n)
//#define T_FC_ARENA_REQUEST_CREATE_TEAMINFO  (MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_REQUEST_CREATE_TEAMINFO)		// C->F
//#define T_FC_ARENA_REQUEST_CREATE_TEAMINFO_OK  (MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_REQUEST_CREATE_TEAMINFO_OK)	// F->C
//#define T_FC_ARENA_BEFORE_ROOM_WAR_FINISH	(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_BEFORE_ROOM_WAR_FINISH)		// F->C
//#define T_FC_ARENA_REQUEST_OTHER_TEAM_INFO  (MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_REQUEST_OTHER_TEAM_INFO)	// C->F
//#define T_FC_ARENA_REQUEST_OTHER_TEAM_INFO_OK	(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_REQUEST_OTHER_TEAM_INFO_OK)	// F->C
//#define T_FC_ARENA_GM_COMMAND_INFO_HEADER	(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_GM_COMMAND_INFO_HEADER)	// F->C
//#define T_FC_ARENA_GM_COMMAND_INFO			(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_GM_COMMAND_INFO)	// F->C
//#define T_FC_ARENA_GM_COMMAND_INFO_DONE		(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_GM_COMMAND_INFO_DONE)	// F->C
//#define T_FC_ARENA_QUICK_ENTER_TEAM			(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_QUICK_ENTER_TEAM)	// C->F
//#define T_FC_ARENA_WATCH_READY				(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_WATCH_READY)		// F->C
//#define T_FC_ARENA_WATCH_START				(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_WATCH_START)		// F->C
//#define T_FC_ARENA_WATCH_END				(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_WATCH_END)			// F->C
//#define T_FC_ARENA_WATCH_REMOVE_USER		(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_WATCH_REMOVE_USER)			// F->C
// 2008-01-08 by dhjin, �Ʒ��� ���� - 
//#define T_FC_ARENA_POSSIBLE_GAMESTART		(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_POSSIBLE_GAMESTART)			// F->C
//#define T_FC_ARENA_CHARACTER_GAMESTART		(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_CHARACTER_GAMESTART)			// C->F
//#define T_FC_ARENA_USE_CITYWARPITEM			(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_USE_CITYWARPITEM)				// F->C



struct MSG_FC_ARENA_REQUEST_TEAM
{
	BYTE				ArenaMode;			// 2007-04-17 by dhjin, �Ʒ��� ���   1 : DeathMatch, 2 : Round, so on..
	BYTE				RequestTeamState;	// 2007-06-05 by dhjin, �� ���� ��û �÷��� 1 : ��� ����, 2 : �����. 3 : ����� ����
	ClientIndex_t		ClientIndex;		// 2007-04-17 by dhjin, ��û�� Ŭ���̾�Ʈ
	INT					StartTeamNum;		// 2007-04-27 by dhjin, �� ���� ��û�Ͽ� �����ִ� �� ���� ��ȣ ex)11 => 11��° ����� ARENA_MAX_REQUEST_TEAM���� ����
	MapIndex_t			ArenaMap;			// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ������ �Ʒ��� ��
};

struct SARENA_TEAM_INFO
{
	bool			TeamLock;				// 2007-04-17 by dhjin, �� ���� ����  0 : ����, 1: ����� 
	bool			TeamEvent;				// 2008-03-10 by dhjin, �Ʒ��� ���� - �̺�Ʈ������ 0: �Ϲ��� 1: �̺�Ʈ��
	BYTE			TeamSize;				// 2007-04-17 by dhjin, �� �ο� ���� 
	BYTE			CurrentTeamSize;		// 2007-04-25 by dhjin, ���� �� �ο�
	BYTE			ArenaMode;				// 2007-04-17 by dhjin, �Ʒ��� ���   1 : DeathMatch, 2 : Round, so on..
	BYTE			ArenaState;				// 2007-06-05 by dhjin, ���� �Ʒ��� ����
	BYTE			TeamStartLevel;			// 2007-06-05 by dhjin, ���� ���� ����
	BYTE			TeamEndLevel;			// 2007-06-05 by dhjin, ���� �� ����
	INT				TeamNum;				// 2007-04-17 by dhjin, �� ��ȣ
	CHAR			CharacterName[SIZE_MAX_CHARACTER_NAME];		// 2007-04-17 by dhjin, ���� �̸�
	MapIndex_t		ArenaMap;				// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ������ �Ʒ��� ��
};

struct MSG_FC_ARENA_REQUEST_TEAM_OK
{
	INT				ArenaTeamInfoTotalCount;	// 2007-04-27 by dhjin, �� �ִ� ���
	INT				ArenaTeamInfoListCount;		// 2007-04-19 by dhjin,
	BYTE			RequetTeamState;			// 2007-06-07 by dhjin, �� ���� ��û �÷��� 1 : ��� ����, 2 : �����. 3 : ����� ���� 
	MapIndex_t		ArenaMap;					// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ������ �Ʒ��� ��
	_ARRAY(SARENA_TEAM_INFO);
};

struct MSG_FC_ARENA_CREATE_TEAM
{
	bool			TeamLock;				// 2007-04-17 by dhjin, �� ���� ���� 0 : ����, 1: �����
	BYTE			ArenaMode;				// 2007-04-17 by dhjin, �Ʒ��� ���   1 : DeathMatch, 2 : Round, so on..
	BYTE			TeamSize;				// 2007-04-17 by dhjin, �� �ο� ���� 
	ClientIndex_t	ClientIndex;			// 2007-04-17 by dhjin, ��û�� Ŭ���̾�Ʈ
	CHAR			TeamPW[SIZE_MAX_TEAM_PW];	// 2007-04-17 by dhjin, �� ��� ��ȣ
	MapIndex_t		ArenaMap;				// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ������ �Ʒ��� ��
};

struct MSG_FC_ARENA_CREATE_TEAM_OK
{
	BYTE			ArenaMode;				// 2007-04-17 by dhjin, �Ʒ��� ���   1 : DeathMatch, 2 : Round, so on..
	BYTE			TeamSize;				// 2007-04-17 by dhjin, �� �ο� ���� 
	INT				TeamNum;				// 2007-04-17 by dhjin, ������ �� ��ȣ
	MapIndex_t		ArenaMap;				// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ������ �Ʒ��� ��
};

struct MSG_FC_ARENA_ENTER_TEAM
{
	BYTE			ArenaMode;				// 2007-04-17 by dhjin, �Ʒ��� ���   1 : DeathMatch, 2 : Round, so on..
	ClientIndex_t	ClientIndex;			// 2007-04-17 by dhjin, ��û�� Ŭ���̾�Ʈ
	CHAR			TeamPW[SIZE_MAX_TEAM_PW];	// 2007-04-17 by dhjin, �� ��� ��ȣ
	INT				TeamNum;				// 2007-04-17 by dhjin, �����Ϸ��� �� ��ȣ	
};

struct MSG_FC_ARENA_ENTER_TEAM_OK
{
	BYTE			ArenaMode;				// 2007-04-17 by dhjin, �Ʒ��� ���   1 : DeathMatch, 2 : Round, so on..
	BYTE			TeamSize;				// 2007-04-17 by dhjin, �� �ο� ���� 
	INT				TeamNum;				// 2007-04-17 by dhjin, ������ �� ��ȣ
};

struct SARENA_TEAM_CHARACTERINFO
{
	BYTE			ArenaState;				// 2007-04-17 by dhjin, ���� �Ʒ��� ����
	ClientIndex_t	ClientIndex;
	CHAR			CharacterName[SIZE_MAX_CHARACTER_NAME];	// 2007-04-17 by dhjin, �������� �̸�
	INT				PlayCharacterType;		// 2012-06-08 by jhseol, �Ʒ��� �߰�����part2 - �ɸ��� : Ÿ�� ����
};

struct MSG_FC_ARENA_TEAM_MEMBER_LIST
{
	BYTE			ArenaTeamState;						// 2007-05-07 by dhjin, �Ʒ��� �� ����
	BYTE			ArenaTeamCharacterNameListCount;	// 2007-04-19 by dhjin
	_ARRAY(SARENA_TEAM_CHARACTERINFO);
};

struct MSG_FC_ARENA_PLAY_CHARACTER_TYPE
{
	INT				PlayCharacterType;		// 2012-06-08 by jhseol, �Ʒ��� �߰�����part2 - �ɸ��� : Ÿ�� ����
};

struct MSG_FC_ARENA_REAVE_TEAM
{
	BYTE			ArenaMode;				// 2007-04-17 by dhjin, �Ʒ��� ���   1 : DeathMatch, 2 : Round, so on..
	ClientIndex_t	ClientIndex;			// 2007-04-17 by dhjin, ��û�� Ŭ���̾�Ʈ
	INT				TeamNum;				// 2007-04-17 by dhjin, Ż���Ϸ��� �� ��ȣ
};

struct MSG_FC_ARENA_TEAM_READY
{
	BYTE			ArenaMode;
	BYTE			ArenaState;				// 2007-04-17 by dhjin, ���� �Ʒ��� ����, 2 : ARENA_STATE_READY
	INT				TeamNum;				// 2007-04-17 by dhjin, �� ��ȣ
};

struct MSG_FC_ARENA_ENTER_ROOM
{
	BYTE			ArenaMode;
	BYTE			ArenaState;				// 2007-04-17 by dhjin, ���� �Ʒ��� ����, 3 : ARENA_STATE_WARREADY
	BYTE			VCNDeathCount;			// 2007-04-17 by dhjin, ���������� ������ġ ����Ʈ
	BYTE			ANIDeathCount;			// 2007-04-17 by dhjin, �˸��� ������ġ ����Ʈ
	BYTE			PlayLimitedTime;		// 2007-05-04 by dhjin, ��� ���� �ð�
	INT				TeamNum;				// 2007-04-17 by dhjin, �� ��ȣ
	ATUM_DATE_TIME	StartRoomTime;			// 2007-04-17 by dhjin, �� ���� �ð�
	ATUM_DATE_TIME	ArenaServerTime;		// 2009-09-10 by jwlee, �Ʒ��� ���� �ð� ���� ���� ���� - 
};

struct MSG_FC_ARENA_ENTER_ROOM_WARP
{// 2008-02-21 by dhjin, �Ʒ��� ���� - 
	bool			CheckCommand;			// 2008-02-21 by dhjin, '/�Ʒ����̵�' ��ɾ� ������� üũ
};

struct MSG_FC_ARENA_ROOM_WAR_START
{
	BYTE			ArenaMode;
	BYTE			ArenaState;				// 2007-04-17 by dhjin, ���� �Ʒ��� ����, 4 : ARENA_STATE_WARING
	ATUM_DATE_TIME	Time;					// 2007-04-17 by dhjin, ���� ���� �ð�
};

struct MSG_FC_ARENA_ROOM_WAR_INFO
{
	BYTE			ArenaMode;
	BYTE			VCNDeathCount;			// 2007-04-17 by dhjin, ���������� ������ġ ����Ʈ
	BYTE			ANIDeathCount;			// 2007-04-17 by dhjin, �˸��� ������ġ ����Ʈ
											//	ATUM_DATE_TIME	Time;					// 2007-04-17 by dhjin, ���� ���� �ð�
};

struct MSG_FC_ARENA_ROOM_WAR_FINISH
{
	BYTE			Influence;				// 2007-04-17 by dhjin, ���� 
	BYTE 			ShootingDown;			// 2007-04-17 by dhjin, ���߼� 
	BYTE 			SufferingAttack;		// 2007-04-17 by dhjin, �ǰݼ� 
	BYTE			LostPoint;				// 2007-06-01 by dhjin, LostPoint
	USHORT			UnitKind;				// 2007-04-17 by dhjin, ��� ���� 
	CHAR			CharacterLevelRank[SIZE_MAX_LEVELRANK];	// 2007-04-17 by dhjin, ���� ���� ���
	CHAR			CharacterName[SIZE_MAX_CHARACTER_NAME];	// 2007-04-17 by dhjin, ���� �̸�
	FLOAT			Average;				// 2012-06-08 by jhseol, �Ʒ��� �߰�����part2 - HP/DP ����
	INT				TeamAverage;			// 2012-06-08 by jhseol, �Ʒ��� �߰�����part2 - HP/DP ����
};

struct MSG_FC_ARENA_ROOM_WAR_FINISH_HEADER
{
	BYTE			ArenaMode;
	BYTE			ArenaState;				// 2007-04-17 by dhjin, ���� �Ʒ��� ����, 5 : ARENA_STATE_WARFINISH
	ATUM_DATE_TIME	FinishTime;				// 2007-04-17 by dhjin, ���� ���� �ð�
};

struct MSG_FC_ARENA_ROOM_WAR_FINISH_DONE
{
	BYTE			Issue;					// 2007-04-20 by dhjin, �Ʒ��� ���� ����, 1 : ARENA_ISSUE_WIN, 2 : ARENA_ISSUE_LOSE, 3 : ARENA_ISSUE_DRAW
	BYTE			EndState;				// 2007-05-16 by dhjin, �Ʒ��� ���� Ÿ��, 1 : ARENA_END_TYPE_POINT, 2 : ARENA_END_TYPE_TIME, 3 : ARENA_END_TYPE_GIVEUP
	INT				InflPoint;				// 2007-04-17 by dhjin, ���� ����Ʈ ����
	INT				WarPoint;				// 2007-04-17 by dhjin, WarPoint ����
	INT				TotalWarPoint;			// 2007-05-28 by dhjin, WP
	INT				CumulativeWarPoint;		// 2007-05-28 by dhjin, CumulativeWP
	INT				ArenaWin;				// 2007-06-07 by dhjin, �Ʒ��� ���� ���� ��
	INT				ArenaLose;				// 2007-06-07 by dhjin, �Ʒ��� ���� ���� ��
};

struct MSG_FC_ARENA_ROOM_WAR_FINISH_DRAW
{
	BYTE			ArenaMode;
	BYTE			ArenaState;				// 2007-04-17 by dhjin, ���� �Ʒ��� ����, 5 : ARENA_STATE_WARFINISH
	BYTE			Issue;					// 2007-04-20 by dhjin, �Ʒ��� ���� ����, 1 : ARENA_ISSUE_WIN, 2 : ARENA_ISSUE_LOSE, 3 : ARENA_ISSUE_DRAW
	BYTE			EndState;				// 2007-05-16 by dhjin, �Ʒ��� ���� Ÿ��, 1 : ARENA_END_TYPE_POINT, 2 : ARENA_END_TYPE_TIME, 3 : ARENA_END_TYPE_GIVEUP
	INT				InflPoint;				// 2007-04-17 by dhjin, ���� ����Ʈ ����
	INT				WarPoint;				// 2007-04-17 by dhjin, WarPoint ����
	ATUM_DATE_TIME	FinishTime;				// 2007-04-17 by dhjin, ���� ���� �ð�
};

//struct SARENA_ROOM_FINISH_INFO
//{
//	BYTE			Influence;				// 2007-04-17 by dhjin, ���� 
//// 	BYTE			Rank;					// 2007-04-17 by dhjin, ���߼��� ���� ��ŷ
//	BYTE 			ShootingDown;			// 2007-04-17 by dhjin, ���߼� 
//	BYTE 			SufferingAttack;		// 2007-04-17 by dhjin, �ǰݼ� 
//	USHORT			UnitKind;				// 2007-04-17 by dhjin, ��� ���� 
//	CHAR			CharacterLevelRank[SIZE_MAX_LEVELRANK];	// 2007-04-17 by dhjin, ���� ���� ���
//	CHAR			CharacterName[SIZE_MAX_CHARACTER_NAME];	// 2007-04-17 by dhjin, ���� �̸�
//};
//
//struct MSG_FC_ARENA_ROOM_WAR_FINISH
//{
//	BYTE			ArenaMode;
//	BYTE			ArenaState;				// 2007-04-17 by dhjin, ���� �Ʒ��� ����, 5 : ARENA_STATE_WARFINISH
//	BYTE			Issue;					// 2007-04-20 by dhjin, �Ʒ��� ���� ����, 1 : ARENA_ISSUE_WIN, 2 : ARENA_ISSUE_LOSE, 3 : ARENA_ISSUE_DRAW
//	INT				InflPoint;				// 2007-04-17 by dhjin, ���� ����Ʈ ����
//	INT				WarPoint;				// 2007-04-17 by dhjin, WarPoint ����
//	ATUM_DATE_TIME	FinishTime;				// 2007-04-17 by dhjin, ���� ���� �ð�
//	BYTE			ArenaRoomFinishInfoDeathMatchListCount; // 2007-04-19 by dhjin
//	_ARRAY(SARENA_ROOM_FINISH_INFO);
//};

struct MSG_FC_ARENA_FINISH_WARP_OK
{
	MAP_CHANNEL_INDEX	MapChannelIndex;	// 2007-04-17 by dhjin, �� ä�� ����
};

struct SARENA_REQUEST_CREATE_TEAMINFO
{
	BYTE 	ArenaMode;				// �Ʒ��� ��� 1 : DeathMatch, 2 : Round
	BYTE 	ReqMemberNum; 			// �� �ο� ����
	MapIndex_t	ArenaMap;			// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ������ �Ʒ��� ��
};

struct MSG_FC_ARENA_REQUEST_CREATE_TEAMINFO_OK
{
	BYTE	ArenaRequetCreateTeamInfoListCount; // 2007-04-26 by dhjin
	_ARRAY(SARENA_REQUEST_CREATE_TEAMINFO);
};

// 2007-05-23 by dhjin, ��� �� ���� ����
struct SARENA_REQUEST_OTHER_TEAM_INFO_OK
{
	BYTE	MemberCount;			// �� �ο���
	INT		TeamCount;				// �� ��
};

struct MSG_FC_ARENA_REQUEST_OTHER_TEAM_INFO_OK
{
	BYTE 	ArenaMode;				// �Ʒ��� ��� 1 : DeathMatch, 2 : Round
	BYTE	ArenaOtherTeamListCount;
	_ARRAY(SARENA_REQUEST_OTHER_TEAM_INFO_OK);
};

// 2007-05-25 by dhjin, GM��ɾ� ����� ���� ����ü
struct SARENA_GM_COMMAND_INFO
{
	BYTE 	ArenaMode;							// �Ʒ��� ��� 1 : DeathMatch, 2 : Round
	BYTE	Level;								// �� ����
	BYTE	MemberCount;						// �� �ο���
	INT		TeamCount;							// �� ��
};
using MSG_FC_ARENA_GM_COMMAND_INFO = SARENA_GM_COMMAND_INFO;

struct MSG_FC_ARENA_GM_COMMAND_INFO_DONE
{
	INT	ReadyUserTotalCount;
	INT	MatchIngUserTotalCount;
	INT	ArenaFightIngUserTotalCount;
};

struct MSG_FC_ARENA_WATCH_READY
{
	BYTE			ArenaState;			// 2007-06-18 by dhjin, ���� �Ʒ��� ����
};

struct SARENA_WATCH_START_USER_INFO
{
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	INT		HP;
	INT		DP;
#else
	SHORT	HP;
	SHORT	DP;
#endif
	float			CurrentHP;
	float			CurrentDP;
	BYTE			Influence;
	ClientIndex_t	ClientIndex;
	CHAR			CharacterName[SIZE_MAX_CHARACTER_NAME];		// 2007-06-15 by dhjin, ���� �̸�
};

struct MSG_FC_ARENA_WATCH_START
{
	BYTE			VCNDeathCount;			// 2007-06-18 by dhjin, ���������� ������ġ ����Ʈ
	BYTE			ANIDeathCount;			// 2007-06-18 by dhjin, �˸��� ������ġ ����Ʈ
	INT				Time;					// 2007-06-18 by dhjin, ���� ���� �ð� (��)
	INT				ArenaWatchStartUserInfoListCount;		// 2007-06-15 by dhjin,
	_ARRAY(SARENA_WATCH_START_USER_INFO);
};

struct MSG_FC_ARENA_WATCH_REMOVE_USER
{
	ClientIndex_t	ClientIndex;			// 2007-06-18 by dhjin, �Ʒ������� ���� ����
};

// 2008-01-08 by dhjin, �Ʒ��� ���� - 
struct MSG_FC_ARENA_POSSIBLE_GAMESTART
{// 2008-01-08 by dhjin, �Ʒ��� �ɸ��� ������ �����Ѵ�.
	BitFlag16_t		MapWeather;
	CHARACTER		AFSCharacter;
	char			ServerGroupName0[SIZE_MAX_SERVER_NAME];
	char			MainORTestServerName[SIZE_MAX_SERVER_NAME];
	char			GamePublisher[SIZE_MAX_GAME_PUBLISHER_NAME];
	bool			IsPCBangUser;			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 
};

struct MSG_FC_CHARACTER_READY_GAMESTART_FROM_ARENA_TO_MAINSERVER_OK
{// 2008-01-15 by dhjin, �Ʒ��� ��� ���� �� �ʵ� ���� ���� �� �̸� �ʿ��� ������ ���⼭ �����Ѵ�.
	BitFlag16_t		MapWeather;
	BYTE			CharacterMode0;
	bool			bMemberPCBang;
	CHARACTER		Character;
	char			ServerGroupName0[SIZE_MAX_SERVER_NAME];
	char			MainORTestServerName[SIZE_MAX_SERVER_NAME];
	char			GamePublisher[SIZE_MAX_GAME_PUBLISHER_NAME];
	BYTE			bIsLockSecondPW;			// 2008-08-26 by cmkwon, ��Ʈ�� VTC-Intecom_Viet 2�� �н����� �ý��� ���� ���� - 2�� �н����� ���� ����
};

//////////////////////////////////////////////////////////////////////////
// 2007-07-06 by dhjin, Tutorial
//#define T_FC_TUTORIAL_START				(MessageType_t)((T0_FC_TUTORIAL<<8)|T1_FC_TUTORIAL_START)			// C->F
//#define T_FC_TUTORIAL_START_OK			(MessageType_t)((T0_FC_TUTORIAL<<8)|T1_FC_TUTORIAL_START_OK)		// F->C
//#define T_FC_TUTORIAL_COMPLETE			(MessageType_t)((T0_FC_TUTORIAL<<8)|T1_FC_TUTORIAL_COMPLETE)		// C->F
//#define T_FC_TUTORIAL_COMPLETE_OK		(MessageType_t)((T0_FC_TUTORIAL<<8)|T1_FC_TUTORIAL_COMPLETE_OK)		// F->C
//#define T_FC_TUTORIAL_END				(MessageType_t)((T0_FC_TUTORIAL<<8)|T1_FC_TUTORIAL_END)				// C->F
//#define T_FC_TUTORIAL_END_OK			(MessageType_t)((T0_FC_TUTORIAL<<8)|T1_FC_TUTORIAL_END_OK)			// F->C
//#define T_FC_TUTORIAL_WARP				(MessageType_t)((T0_FC_TUTORIAL<<8)|T1_FC_TUTORIAL_WARP)			// C->F

struct STUTORIAL_LIST_INFO
{// 2007-07-06 by dhjin, Tutorial �Ϸ� ���� ���� ����ü
	INT		TutorialNum;				// 2007-07-06 by dhjin, Tutorial ��ȣ 
	BYTE 	TutorialComplete;			// 2007-07-06 by dhjin, Tutorial �Ϸ� ����, 0 : FALSE, 1 : TRUE
};

struct MSG_FC_TUTORIAL_START_OK
{// 2007-07-06 by dhjin, Tutorial �Ϸ� ���� �޼���
	INT		TutorialInfoListCount;		// 2007-07-06 by dhjin, Tutorial ���� ����Ʈ ��
	_ARRAY(STUTORIAL_LIST_INFO);
};

struct MSG_FC_TUTORIAL_COMPLETE
{// 2007-07-06 by dhjin, Tutorial �Ϸ� ��ȣ
	INT		TutorialNum;
};

struct MSG_FC_TUTORIAL_COMPLETE_OK
{// 2007-07-06 by dhjin, Tutorial �Ϸ� ��ȣ
	INT		TutorialNum;
	INT		TutorialItemNum1;
	INT		TutorialItemCount1;
	INT		TutorialItemNum2;
	INT		TutorialItemCount2;
	INT		TutorialItemNum3;
	INT		TutorialItemCount3;
};


//////////////////////////////////////////////////////////////////////////
// 2007-08-13 by dhjin, OUTPOST
//#define T_FC_OUTPOST_WAR_BEFORE				(MessageType_t)((T0_FC_OUTPOST<<8)|T1_FC_OUTPOST_WAR_BEFORE)			// F->C(N)
//#define T_FC_OUTPOST_WAR_START 				(MessageType_t)((T0_FC_OUTPOST<<8)|T1_FC_OUTPOST_WAR_START)				// F->C(N)
//#define T_FC_OUTPOST_PROTECTOR_DESTROY		(MessageType_t)((T0_FC_OUTPOST<<8)|T1_FC_OUTPOST_PROTECTOR_DESTROY)		// F->C(N)
//#define T_FC_OUTPOST_RESET_CHECK_START		(MessageType_t)((T0_FC_OUTPOST<<8)|T1_FC_OUTPOST_RESET_CHECK_START)		// C->F
//#define T_FC_OUTPOST_RESET_CHECK_START_OK	(MessageType_t)((T0_FC_OUTPOST<<8)|T1_FC_OUTPOST_RESET_CHECK_START_OK)	// F->C
//#define T_FC_OUTPOST_RESET_START			(MessageType_t)((T0_FC_OUTPOST<<8)|T1_FC_OUTPOST_RESET_START)			// C->F
//#define T_FC_OUTPOST_RESET_START_OK 		(MessageType_t)((T0_FC_OUTPOST<<8)|T1_FC_OUTPOST_RESET_START_OK)		// F->C(N)
//#define T_FC_OUTPOST_RESET_DESTROY	 		(MessageType_t)((T0_FC_OUTPOST<<8)|T1_FC_OUTPOST_RESET_DESTROY)			// F->C
//#define T_FC_OUTPOST_RESET_SUCCESS	 		(MessageType_t)((T0_FC_OUTPOST<<8)|T1_FC_OUTPOST_RESET_SUCCESS)			// F->C
//#define T_FC_OUTPOST_WAR_END		 		(MessageType_t)((T0_FC_OUTPOST<<8)|T1_FC_OUTPOST_WAR_END)				// F->C(N)
//#define T_FC_OUTPOST_NEXTWAR_INFO_REQUEST		(MessageType_t)((T0_FC_OUTPOST<<8)|T1_FC_OUTPOST_NEXTWAR_INFO_REQUEST)			// C->F
//#define T_FC_OUTPOST_NEXTWAR_INFO_REQUEST_OK 	(MessageType_t)((T0_FC_OUTPOST<<8)|T1_FC_OUTPOST_NEXTWAR_INFO_REQUEST_OK)		// F->C(N)
//#define T_FC_OUTPOST_NEXTWAR_SET_TIME 		(MessageType_t)((T0_FC_OUTPOST<<8)|T1_FC_OUTPOST_NEXTWAR_SET_TIME)			// C->F
//#define T_FC_OUTPOST_NEXTWAR_SET_TIME_OK	(MessageType_t)((T0_FC_OUTPOST<<8)|T1_FC_OUTPOST_NEXTWAR_SET_TIME_OK)			// F->C
//#define T_FC_OUTPOST_WAR_INFO				(MessageType_t)((T0_FC_OUTPOST<<8)|T1_FC_OUTPOST_WAR_INFO)					// F->C


struct MSG_FC_OUTPOST_WAR_BEFORE
{// 2007-08-13 by dhjin, OutPost ���� �� ���� �ð� ���� ����
	BYTE	RemainTime;
	BYTE	Influence;
	INT		MapIndex;
};

struct MSG_FC_OUTPOST_WAR_START
{// 2007-08-13 by dhjin, OutPost ���� ���� ����
	BYTE			OutPostState;
	BYTE			Influence;
	INT				MapIndex;
	ATUM_DATE_TIME	WarStartTime;
	ATUM_DATE_TIME	WarEndTime;
};

struct MSG_FC_OUTPOST_PROTECTOR_DESTROY
{// 2007-08-13 by dhjin, OutPost ��ȣ�� �ı� ����
	BYTE	Influence;
	INT		MapIndex;
	UID32_t	GuildUID;
	CHAR	GuildName[SIZE_MAX_GUILD_NAME];
};

struct MSG_FC_OUTPOST_RESET_START
{// 2007-08-16 by dhjin, �߾�ó����ġ ���� ����
	ClientIndex_t	TargetIndex;
};

struct MSG_FC_OUTPOST_RESET_START_OK
{// 2007-08-16 by dhjin, �߾�ó����ġ ���� ����
	BYTE	Influence;
	INT		MapIndex;
	UID32_t	GuildUID;
	CHAR	GuildName[SIZE_MAX_GUILD_NAME];
	ATUM_DATE_TIME	ResetStartTime;
	ATUM_DATE_TIME	ResetEndTime;
};

struct MSG_FC_OUTPOST_RESET_DESTROY
{// 2007-08-13 by dhjin, �߾�ó����ġ �ı���
	INT		MapIndex;
};

struct MSG_FC_OUTPOST_RESET_SUCCESS
{// 2007-08-13 by dhjin, �߾�ó����ġ ���� ���� 
	BYTE	Influence;
	INT		MapIndex;
	UID32_t	GuildUID;
	CHAR	GuildName[SIZE_MAX_GUILD_NAME];
};

struct MSG_FC_OUTPOST_WAR_END
{// 2007-08-13 by dhjin, ���������� ����
	BYTE	Influence;
	INT		MapIndex;
	UID32_t	GuildUID;
	CHAR	GuildName[SIZE_MAX_GUILD_NAME];
};

struct MSG_FC_OUTPOST_NEXTWAR_INFO_REQUEST_OK
{// 2007-08-13 by dhjin, ���������� �ð� ���� ���� ����
	ATUM_DATE_TIME	    OutPostNextWarTime;
	INT					OutPostNextWarSelectTimeChoice;
	INT					OutPostNextWarTimeListCount;
	_ARRAY(ATUM_DATE_TIME);
};

struct MSG_FC_OUTPOST_NEXTWAR_SET_TIME
{// 2007-08-13 by dhjin, ���������� ���� �ð� ����
	INT		OutPostNextWarSelectTimeChoice;
};

struct MSG_FC_OUTPOST_NEXTWAR_SET_TIME_OK
{// 2007-09-06 by dhjin, ���������� ���� �ð� ���� ������ ��� �������� �����ش�.
	MapIndex_t		MapIndex;
	ATUM_DATE_TIME	OutPostNextWarTime;
};

struct SOUTPOST_WAR_INFO
{// 2007-09-14 by dhjin, ���� ���� �� ������� ������ ���� ���̸� ������� ������ �������� �����ش�.
	BYTE			OutPostState;
	BYTE			OutPostResetIngInfluence;			// 2007-10-04 by dhjin, ���� ���� ����
	MapIndex_t		MapIndex;
	INT				OutPostWarResetRamainSecondTime;	// 2007-10-04 by dhjin, ���� �Ϸ���� ���� �ð�
	ATUM_DATE_TIME	OutPostWarStartTime;
	ATUM_DATE_TIME  OutPostWarEndTime;
	//	ATUM_DATE_TIME	OutPostWarResetStartTime;
	//	ATUM_DATE_TIME	OutPostWarResetEndTime;
};

struct MSG_FC_OUTPOST_WAR_INFO
{// 2007-09-14 by dhjin, ���� ���� �� ������� ������ ���� ���̸� ������� ������ �������� �����ش�.
	INT					OutPostWarInfoListCount;
	_ARRAY(SOUTPOST_WAR_INFO);
};


//////////////////////////////////////////////////////////////////////////
// 2007-08-13 by dhjin, �������� (����~! FC ���������� ���� ���� ���� �ʾƼ� �ǹ̰� ����� T_FC_INFO�� �켱 �Ҵ��Ͽ� ����)
//#define T_FC_INFO_NOTICE_REQUEST		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_NOTICE_REQUEST)			// C->F
//#define T_FC_INFO_NOTICE_REQUEST_OK		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_NOTICE_REQUEST_OK)		// F->C
//#define T_FC_INFO_NOTICE_REG 			(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_NOTICE_REG)			// C->F
//#define T_FC_INFO_NOTICE_REG_OK			(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_NOTICE_REG_OK)		// F->C
//#define T_FC_INFO_NOTICE_MODIFY 		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_NOTICE_MODIFY)			// C->F
//#define T_FC_INFO_NOTICE_MODIFY_OK		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_NOTICE_MODIFY_OK)		// F->C

struct MSG_FC_INFO_NOTICE_REQUEST
{// 2007-08-13 by dhjin, �������� ��û
	BYTE	NoticeState;
	INT		MapIndex;
};

struct MSG_FC_INFO_NOTICE_REQUEST_OK
{// 2007-08-13 by dhjin, �������� ��û ����
	CHAR	NoticeString[SIZE_MAX_NOTICE];
};

struct MSG_FC_INFO_NOTICE_REG
{// 2007-08-13 by dhjin, �������� ���
	UID32_t	GuildUID;
	CHAR	NoticeString[SIZE_MAX_NOTICE];
};

struct MSG_FC_INFO_NOTICE_MODIFY
{// 2007-08-13 by dhjin, �������� ����
	UID32_t	GuildUID;
	CHAR	NoticeString[SIZE_MAX_NOTICE];
};

//////////////////////////////////////////////////////////////////////////
// 2007-08-13 by dhjin, �ǰ��� (����~! FC ���������� ���� ���� ���� �ʾƼ� �ǹ̰� ����� T_FC_INFO�� �켱 �Ҵ��Ͽ� ����)
//#define T_FC_INFO_EXPEDIENCYFUND_REQUEST 		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_EXPEDIENCYFUND_REQUEST)			// C->F
//#define T_FC_INFO_EXPEDIENCYFUND_REQUEST_OK		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_EXPEDIENCYFUND_REQUEST_OK)		// F->C
//#define T_FC_INFO_EXPEDIENCYFUND_PAYBACK 		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_EXPEDIENCYFUND_PAYBACK)			// C->F
//#define T_FC_INFO_EXPEDIENCYFUND_PAYBACK_OK		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_EXPEDIENCYFUND_PAYBACK_OK)		// F->C
//#define T_FC_INFO_SECONDARYPASSWORD_UPDATE_PASSWORD		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_SECONDARYPASSWORD_UPDATE_PASSWORD)	// 2007-09-12 by cmkwon, ��Ʈ�� 2���н����� ���� - �������� �߰�, C->F
//#define T_FC_INFO_SECONDARYPASSWORD_UPDATE_PASSWORD_OK	(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_SECONDARYPASSWORD_UPDATE_PASSWORD_OK)	// 2007-09-12 by cmkwon, ��Ʈ�� 2���н����� ���� - �������� �߰�, F->C
//#define T_FC_INFO_SECONDARYPASSWORD_CHECK_PASSWORD		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_SECONDARYPASSWORD_CHECK_PASSWORD)	// 2007-09-12 by cmkwon, ��Ʈ�� 2���н����� ���� - �������� �߰�, C->F
//#define T_FC_INFO_SECONDARYPASSWORD_CHECK_PASSWORD_OK	(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_SECONDARYPASSWORD_CHECK_PASSWORD_OK)	// 2007-09-12 by cmkwon, ��Ʈ�� 2���н����� ���� - �������� �߰�, F->C
//#define T_FC_INFO_SECONDARYPASSWORD_LOCK				(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_SECONDARYPASSWORD_LOCK)				// 2007-09-12 by cmkwon, ��Ʈ�� 2���н����� ���� - �������� �߰�, C->F
//#define T_FC_INFO_SECONDARYPASSWORD_LOCK_OK				(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_SECONDARYPASSWORD_LOCK_OK)			// 2007-09-12 by cmkwon, ��Ʈ�� 2���н����� ���� - �������� �߰�, F->C

struct MSG_FC_INFO_EXPEDIENCYFUND_REQUEST
{// 2007-08-13 by dhjin, �ǰ��� ���� ��û
	INT 	MapIndex;
};

struct MSG_FC_INFO_EXPEDIENCYFUND_REQUEST_OK
{// 2007-08-13 by dhjin, �ǰ��� ���� ����
	INT		CumulativeExpediencyFund;
	INT		ExpediencyFund;
	INT		ExpediencyFundRate;
};

struct MSG_FC_INFO_EXPEDIENCYFUND_PAYBACK
{// 2007-08-13 by dhjin, �ǰ��� ��û ��û
	INT	 ExpediencyFundPayBackCount;
};

struct MSG_FC_INFO_EXPEDIENCYFUND_PAYBACK_OK
{// 2007-08-13 by dhjin, �ǰ��� ��û ó�� ���� ����
	INT		CumulativeExpediencyFund;
	INT		ExpediencyFund;
	INT		ExpediencyFundPayBackCount;
	INT		ExpediencyFundRate;
};

struct MSG_FC_INFO_SECONDARYPASSWORD_UPDATE_PASSWORD
{// 2007-09-12 by cmkwon, ��Ʈ�� 2���н����� ���� -
	char	CurrentSecPassword[SIZE_MAX_PASSWORD_MD5_STRING];
	char	NewSecPassword[SIZE_MAX_PASSWORD_MD5_STRING];
};
struct MSG_FC_INFO_SECONDARYPASSWORD_CHECK_PASSWORD
{// 2007-09-12 by cmkwon, ��Ʈ�� 2���н����� ���� -
	char	CurrentSecPassword[SIZE_MAX_PASSWORD_MD5_STRING];
};
struct MSG_FC_INFO_SECONDARYPASSWORD_LOCK
{// 2007-09-12 by cmkwon, ��Ʈ�� 2���н����� ���� -	
	BYTE	bIsUnlockFlag;		// FALSE�̸� Lock, FALSE�� �ƴϸ� Unlock
	char	CurrentSecPassword[SIZE_MAX_PASSWORD_MD5_STRING];
};
struct MSG_FC_INFO_SECONDARYPASSWORD_LOCK_OK
{// 2007-09-12 by cmkwon, ��Ʈ�� 2���н����� ���� -	
	BYTE	bIsUnlockFlag;		// FALSE�̸� Lock, FALSE�� �ƴϸ� Unlock
};


//////////////////////////////////////////////////////////////////////////
// 2007-09-06 by dhjin, �ڷ���Ʈ Ŭ���Ͽ� ������ �����Ѵ�.
//#define T_FC_EVENT_CLICK_TELEPORT			(MessageType_t)((T0_FC_EVENT<<8)|T1_FC_EVENT_CLICK_TELEPORT)		// 2007-09-06 by dhjin, C->F
//#define T_FC_EVENT_CLICK_TELEPORT_OK		(MessageType_t)((T0_FC_EVENT<<8)|T1_FC_EVENT_CLICK_TELEPORT_OK)		// 2008-04-22 by dhjin, F->C ���� ���� ǥ�� ��ȹ�� - �ڷ���Ʈ ���� �Ϸ� �ð�
//#define T_FC_EVENT_TELEPORT_BUILDCOMPLETE	(MessageType_t)((T0_FC_EVENT<<8)|T1_FC_EVENT_TELEPORT_BUILDCOMPLETE)		// 2007-09-19 by dhjin, F->C(n)
//#define T_FC_EVENT_TELEPORT_DESTROY			(MessageType_t)((T0_FC_EVENT<<8)|T1_FC_EVENT_TELEPORT_DESTROY)		// 2007-09-19 by dhjin, F->C(n)

struct MSG_FC_EVENT_CLICK_TELEPORT
{// 2007-09-06 by dhjin, �ڷ���Ʈ Ŭ��
	INT 			MapIndex;
	ClientIndex_t	TargetIndex;
};

struct MSG_FC_EVENT_CLICK_TELEPORT_OK
{// 2008-04-22 by dhjin, ���� ���� ǥ�� ��ȹ�� - �ڷ���Ʈ ���� �Ϸ� �ð�
	INT				MapIndex;
	ATUM_DATE_TIME 	StartTime;
	ATUM_DATE_TIME 	EndTime;
};

struct MSG_FC_EVENT_TELEPORT_BUILDCOMPLETE
{// 2007-09-06 by dhjin, �ڷ���Ʈ �ϼ�
	MapIndex_t 		MapIndex;
};

struct MSG_FC_EVENT_TELEPORT_DESTROY
{// 2007-09-20 by cmkwon, �ڷ���Ʈ ���� - 
	MapIndex_t 		MapIndex;
	bool			bInactivateByOtherTeleport;		// 2007-09-20 by cmkwon, �ڷ���Ʈ ���� - �ʵ��߰�, FALSE�̸� ���ݿ� ���� ����, TRUE�̸� �ٸ� Teleport �������� ���� ��Ȱ��ȭ
};

///////////////////////////////////////////////////////////////////////////////
// 2007-11-28 by cmkwon, �����ý��� ���� -
//#define T_FC_EVENT_NOTIFY_MSG_GET			(MessageType_t)((T0_FC_EVENT<<8)|T1_FC_EVENT_NOTIFY_MSG_GET)	// 2007-11-28 by cmkwon, �����ý��� ���� - C->F
//#define T_FC_EVENT_NOTIFY_MSG_GET_OK		(MessageType_t)((T0_FC_EVENT<<8)|T1_FC_EVENT_NOTIFY_MSG_GET_OK)	// 2007-11-28 by cmkwon, �����ý��� ���� - F->C
//#define T_FC_EVENT_NOTIFY_MSG_DELETE		(MessageType_t)((T0_FC_EVENT<<8)|T1_FC_EVENT_NOTIFY_MSG_DELETE)	// 2007-11-28 by cmkwon, �����ý��� ���� - C->F

struct MSG_FC_EVENT_NOTIFY_MSG_GET_OK		// 2007-11-28 by cmkwon, �����ý��� ���� -
{
	UID64_t			NotifyMsgUID;				//	
	UID32_t			CharacterUID;				// �����޽��� ���� ĳ���� UID, 0 �̸� ��� ĳ���Ϳ��� ����(0 �ΰ��� ������ ���� �� ����)
	BYTE			NotifyMsgType;				// NOTIFY_MSG_TYPE_XXX
	char			NotifyMsgString[SIZE_MAX_NOTIFY_MSG_STRING];
};

struct MSG_FC_EVENT_NOTIFY_MSG_DELETE		// 2007-11-28 by cmkwon, �����ý��� ���� -
{
	UID64_t			NotifyMsgUID;				//	
};

///////////////////////////////////////////////////////////////////////////////
// 2008-01-10 by cmkwon, ������ �̺�Ʈ �ý��ۿ� �� ���� �ý��� �߰� - 
//#define T_FC_EVENT_COUPON_EVENT_INFO			(MessageType_t)((T0_FC_EVENT<<8)|T1_FC_EVENT_COUPON_EVENT_INFO)				// 2008-01-10 by cmkwon, ������ �̺�Ʈ �ý��ۿ� �� ���� �ý��� �߰� - F->C
//#define T_FC_EVENT_COUPON_EVENT_USE_COUPON		(MessageType_t)((T0_FC_EVENT<<8)|T1_FC_EVENT_COUPON_EVENT_USE_COUPON)		// 2008-01-10 by cmkwon, ������ �̺�Ʈ �ý��ۿ� �� ���� �ý��� �߰� - C->F
//#define T_FC_EVENT_COUPON_EVENT_USE_COUPON_OK	(MessageType_t)((T0_FC_EVENT<<8)|T1_FC_EVENT_COUPON_EVENT_USE_COUPON_OK)	// 2008-01-10 by cmkwon, ������ �̺�Ʈ �ý��ۿ� �� ���� �ý��� �߰� - F->C
struct MSG_FC_EVENT_COUPON_EVENT_INFO				// 2008-01-10 by cmkwon, ������ �̺�Ʈ �ý��ۿ� �� ���� �ý��� �߰� - 
{
	int		ItemEventUID;
	int		ItemEventType;
};
struct MSG_FC_EVENT_COUPON_EVENT_USE_COUPON			// 2008-01-10 by cmkwon, ������ �̺�Ʈ �ý��ۿ� �� ���� �ý��� �߰� - 
{
	int		ItemEventUID;
	char	CouponNumber[SIZE_MAX_COUPON_NUMBER];
};
struct MSG_FC_EVENT_COUPON_EVENT_USE_COUPON_OK		// 2008-01-10 by cmkwon, ������ �̺�Ʈ �ý��ۿ� �� ���� �ý��� �߰� - 
{
	int		ItemEventUID;
};



//////////////////////////////////////////////////////////////////////////
// 2007-10-29 by dhjin, Poll�ý��� ����
//#define T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST			(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST)			// C->F, ������ �ĺ� ����Ʈ ��û
//#define T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK_HEADER	(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK_HEADER)		// F->C, ������ �ĺ� ����Ʈ ��û
//#define T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK			(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK)		// F->C, ������ �ĺ� ����Ʈ ��û
//#define T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK_DONE	(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK_DONE)		// F->C, ������ �ĺ� ����Ʈ ��û
//#define T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO			(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO)			// C->F, ������ �ĺ� ���� ��û
//#define T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK			(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK)		// F->C, ������ �ĺ� ���� ��û
//#define T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK_GUILDMARK			(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK_GUILDMARK)		// F->C, ������ �ĺ� ���� ��û
//#define T_FC_CITY_POLL_REG_LEADER_CANDIDATE						(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_REG_LEADER_CANDIDATE)					// C->F, ������ �ĺ� ���
//#define T_FC_CITY_POLL_REG_LEADER_CANDIDATE_OK					(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_REG_LEADER_CANDIDATE_OK)				// F->C, ������ �ĺ� ���
//#define T_FC_CITY_POLL_DELETE_LEADER_CANDIDATE					(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_DELETE_LEADER_CANDIDATE)				// C->F, ������ �ĺ� Ż��
//#define T_FC_CITY_POLL_DELETE_LEADER_CANDIDATE_OK				(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_DELETE_LEADER_CANDIDATE_OK)				// F->C, ������ �ĺ� Ż��
//#define T_FC_CITY_POLL_VOTE										(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_VOTE)									// C->F, ������ �ĺ����� ��ǥ
//#define T_FC_CITY_POLL_VOTE_OK									(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_VOTE_OK)								// F->C, ������ �ĺ����� ��ǥ
//#define T_FC_CITY_POLL_REQUEST_POLL_DATE						(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_REQUEST_POLL_DATE)						// C->F, ���� �Ⱓ ��û
//#define T_FC_CITY_POLL_REQUEST_POLL_DATE_OK						(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_REQUEST_POLL_DATE_OK)					// F->C, ���� �Ⱓ ��û
//#define T_FC_CITY_POLL_LEADER_ELECTION_INFO						(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_LEADER_ELECTION_INFO)					// F->C, ���� ��� ����

struct MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK_HEADER
{// 2007-10-29 by dhjin, ������ �ĺ� ����Ʈ ����
	bool			RegLeaderCandidate;								// ������ �ĺ� �⸶ �� ����, TURE = �⸶
};

struct MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK
{// 2007-10-29 by dhjin, ������ �ĺ� ����Ʈ ����
	INT				LeaderCandidateNum;							//	������ �ĺ� ��ȣ
	UID32_t			CharacterUID;								//	������ �ĺ� UID
	UID32_t			GuildUID;									//  ������ �ĺ� ��� UID
	bool			DeleteCandidate;							//  �̹� ���� �Ⱓ�� �ĺ� Ż�� �ߴ��� ����, TRUE = Ż��
	CHAR			CharacterName[SIZE_MAX_CHARACTER_NAME];		//  ������ �ĺ� �̸�
	CHAR			GuildName[SIZE_MAX_GUILD_NAME];				//  ������ �ĺ� ����
};

struct MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO
{// 2007-10-29 by dhjin, ������ �ĺ� ���� ����
	INT				LeaderCandidateNum;							//	������ �ĺ� ��ȣ
};

struct MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK
{// 2007-10-29 by dhjin, ������ �ĺ� ���� ����
	BYTE			PilotFace;									//  ������ �ĺ� ��
	BYTE			Level;										//	������ �ĺ� Level
	INT				GuildFame;									//	������ �ĺ� ���� ��
	UID32_t			AccountUID;									//	������ �ĺ� ���� UID
	UID32_t			CharacterUID;								//	������ �ĺ� UID
	CHAR			CharacterName[SIZE_MAX_CHARACTER_NAME];		//  ������ �ĺ� �̸�
	CHAR			GuildName[SIZE_MAX_GUILD_NAME];				//  ������ �ĺ� ����
	CHAR			CampaignPromises[SIZE_MAX_CAMPAIGNPROMISES];//  ������ �ĺ� ����
};

struct MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK_GUILDMARK
{// 2007-11-01 by dhjin, ������ �ĺ� ���� ���� ���� ��ũ
	INT				GuildMarkSize;								//  ������ �ĺ� ���� ��ũ ũ��
	UID32_t			GuildUID;									//  ������ �ĺ� ��� UID
	ARRAY_(char);												//	������ �ĺ� ���� ��ũ
};

struct MSG_FC_CITY_POLL_REG_LEADER_CANDIDATE
{// 2007-10-29 by dhjin, ������ �ĺ� ���
	UID32_t			CharacterUID;								//	������ �ĺ� UID
	UID32_t			GuildUID;									//  ������ �ĺ� ��� UID
	CHAR			CampaignPromises[SIZE_MAX_CAMPAIGNPROMISES];//  ������ �ĺ� ����
};

struct MSG_FC_CITY_POLL_REG_LEADER_CANDIDATE_OK
{// 2007-10-29 by dhjin, ������ �ĺ� ���
	INT				LeaderCandidateNum;							//	������ �ĺ� ��ȣ
};

struct MSG_FC_CITY_POLL_DELETE_LEADER_CANDIDATE
{// 2007-10-29 by dhjin, ������ �ĺ� Ż��
	UID32_t			CharacterUID;								//	������ �ĺ� UID
};

struct MSG_FC_CITY_POLL_VOTE
{// 2007-10-29 by dhjin, ��ǥ
	INT				LeaderCandidateNum;							//	������ �ĺ� ��ȣ
	UID32_t			CharacterUID;								//	������ �ĺ� UID
	UID32_t			VoteCharacterUID;							//	��ǥ�� UID
};

struct MSG_FC_CITY_POLL_REQUEST_POLL_DATE_OK
{// 2007-10-29 by dhjin, ���� ����
	ATUM_DATE_TIME	ApplicationStartDate;		// �ĺ� ��û ���� ��
	ATUM_DATE_TIME	ApplicationEndDate;			// �ĺ� ��û ������ ��
	ATUM_DATE_TIME	VoteStartDate;				// ��ǥ ���� ��
	ATUM_DATE_TIME	VoteEndDate;				// ��ǥ ������ ��
	ATUM_DATE_TIME	Election;					// ������
};

struct MSG_FC_CITY_POLL_LEADER_ELECTION_INFO
{// 2007-11-16 by dhjin, ���� ���
	INT				LeaderCandidateNum;							//	������ �ĺ� ��ȣ
	CHAR			CharacterName[SIZE_MAX_CHARACTER_NAME];		//  ������ �ĺ� �̸�
	CHAR			GuildName[SIZE_MAX_GUILD_NAME];				//  ������ �ĺ� ����
	INT				Percent;									//  ������ ��ǥ��
};

//////////////////////////////////////////////////////////////////////////
// 2007-12-03 by dhjin, ���� ���� 
//#define T_FC_CITY_WARINFO_INFLUENCE						(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_WARINFO_INFLUENCE)		// C->F, ���� ����
//#define T_FC_CITY_WARINFO_INFLUENCE_OK					(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_WARINFO_INFLUENCE_OK)	// F->C, ���� ����
//#define T_FC_CITY_WARINFO_OUTPOST						(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_WARINFO_OUTPOST)			// C->F, �������� ���� 
//#define T_FC_CITY_WARINFO_OUTPOST_OK					(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_WARINFO_OUTPOST_OK)		// F->C, �������� ���� 

struct MSG_FC_CITY_WARINFO_INFLUENCE_OK
{// 2007-12-03 by dhjin, ���� ���� 
	INT			VCNInfluencePoint;
	INT			ANIInfluencePoint;
	CHAR		VCNInfluenceLeader[SIZE_MAX_CHARACTER_NAME];
	CHAR		VCNInfluenceSubLeader1[SIZE_MAX_CHARACTER_NAME];
	CHAR		VCNInfluenceSubLeader2[SIZE_MAX_CHARACTER_NAME];
	CHAR		ANIInfluenceLeader[SIZE_MAX_CHARACTER_NAME];
	CHAR		ANIInfluenceSubLeader1[SIZE_MAX_CHARACTER_NAME];
	CHAR		ANIInfluenceSubLeader2[SIZE_MAX_CHARACTER_NAME];
};

struct SCITY_WARINFO_OUTPOST
{// 2007-12-03 by dhjin, �������� ���� 
	BYTE			Influence;
	MapIndex_t		MapIndex;
	UID32_t			GuildUID;
	CHAR			GuildCommander[SIZE_MAX_CHARACTER_NAME];
	CHAR			GuildName[SIZE_MAX_GUILD_NAME];
	ATUM_DATE_TIME	OutpostDate;
};

struct MSG_FC_CITY_WARINFO_OUTPOST_OK
{// 2007-12-03 by dhjin, ���������� ����
	INT					CityWarInfoOutpostListCount;
	_ARRAY(SCITY_WARINFO_OUTPOST);
};

//#define T_FC_INFO_GET_GUILDMARK				(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_GET_GUILDMARK)			// 2007-12-07 by dhjin, ���� ��ũ C->F
//#define T_FC_INFO_GET_GUILDMARK_OK			(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_GET_GUILDMARK_OK)		// 2007-12-07 by dhjin, ���� ��ũ F->C
struct MSG_FC_INFO_GET_GUILDMARK
{// 2007-12-07 by dhjin
	UID32_t			GuildUID;
};

struct MSG_FC_INFO_GET_GUILDMARK_OK
{// 2007-12-07 by dhjin, ���� ��ũ ����
	INT				GuildMarkSize;
	INT				GuildMarkVersion;
	UID32_t			GuildUID;
	ARRAY_(char);
};

// 2007-12-27 by dhjin, �Ʒ��� ���� - �Ʒ��� �������� MainField <=> ArenaField 
//#define T_FtoA_MFSINFO						(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_MFSINFO)			// 2007-12-27 by dhjin, �Ʒ��� ������ �ʵ� ���� ��� �� �ʵ� ���� ���� ����, MF->AF
//#define T_FtoA_MFSINFO_OK					(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_MFSINFO_OK)		// 2007-12-27 by dhjin, �Ʒ��� ������ �ʵ� ���� ��� �� �ʵ� ���� ���� ����, AF->MF
//#define T_FtoA_ALIVE						(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_ALIVE)				// 2007-12-27 by dhjin,
//#define T_FtoA_AUTH_CHARACTER				(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_AUTH_CHARACTER)	// 2007-12-27 by dhjin, �Ʒ��� �������� ��ȿ�� �������� �ʵ� ������ Ȯ�� ��û, AF->MF
//#define T_FtoA_AUTH_CHARACTER_OK			(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_AUTH_CHARACTER_OK)	// 2007-12-27 by dhjin, �Ʒ��� �������� ��ȿ�� �������� �ʵ� ������ Ȯ�� ��û, MF->AF
//#define T_FtoA_ARENA_STATE_CHANGE			(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_ARENA_STATE_CHANGE)	// 2008-01-03 by dhjin, �ʵ� ������ ���� �Ʒ��� ���°� ����, AF->MF
//#define T_FtoA_ARENA_TEAM_MATCHING			(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_ARENA_TEAM_MATCHING)	// 2008-01-04 by dhjin, �ʵ� ������ ��Ī ������ �����Ѵ�, AF->MF
//#define T_FtoA_ARENA_TEAM_MATCHING_OK		(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_ARENA_TEAM_MATCHING_OK)	// 2008-01-04 by dhjin, �ʵ� ������ ��Ī ������ �����Ѵ�, MF->AF
//#define T_FtoA_ARENA_SERVER_PAY				(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_ARENA_SERVER_PAY)		// 2008-01-10 by dhjin, �ʵ� ������ ���� �Ʒ��� ���� ����, AF->MF
//#define T_FtoA_ARENA_CHARACTER_PAY			(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_ARENA_CHARACTER_PAY)	// 2008-01-10 by dhjin, �ʵ� ������ �ɸ��� �Ʒ��� ���� ����, AF->MF
//#define T_FtoA_ARENA_CHARACTER_DISCONNECT	(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_ARENA_CHARACTER_DISCONNECT)	// 2008-01-15 by dhjin, �ʵ� ������ �ɸ��� �Ʒ��� ���� ����, AF->MF
//#define T_FtoA_ARENA_CHARACTERARENAINFO_UPDATA	(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_ARENA_CHARACTERARENAINFO_UPDATA)	// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : AFS -> MFS CharacterArena UUpdata ��û ��Ŷ
//#define T_FtoA_ARENA_CHARACTER_RESET_RELOAD	(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_ARENA_CHARACTER_RESET_RELOAD)	// 2012-12-18 by jhseol, �Ʒ��� ĳ���� ���� �� ĳ���� ���� �ٽ� �ε��ϱ�.
struct MSG_FtoA_MFSINFO
{
	USHORT		MFS_Port;
	CHAR		MFS_Name[SIZE_MAX_SERVER_NAME];
	CHAR		MFS_IP[SIZE_MAX_IPADDRESS];
};

struct MSG_FtoA_AUTH_CHARACTER
{
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	ClientIndex_t	AFSClientIndex;
};

struct MSG_FtoA_AUTH_CHARACTER_OK
{
	bool			ErrorCheck;		// TRUE -> Error
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	CHAR			MFSCharacterName[SIZE_MAX_CHARACTER_NAME];
	ClientIndex_t	MFSClientIndex;
	UID32_t			AFSCharacterUID;
	CHAR			AFSCharacterName[SIZE_MAX_CHARACTER_NAME];
	ClientIndex_t	AFSClientIndex;
	MessageType_t	ErrorMsgType;			// 2010-04-05 by cmkwon, ���� ������ ī�� ���� �ý��� ���� - 
	Err_t			ErrorCode;				// 2010-04-05 by cmkwon, ���� ������ ī�� ���� �ý��� ���� - 
	bool			ShutDownMins; // 2011-11-03 by shcho, yedang �˴ٿ��� ���� - �Ʒ������� �˴ٿ��� ������
	SArenaPlayInfo	ArenaPlayInfo;			// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : �Ʒ��� �÷��� ������ ������ ����ü ����.
};

struct MSG_FtoA_ARENA_STATE_CHANGE
{
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	BYTE			AFSArenaState;
	ClientIndex_t	AFSClientIndex;
};

struct MSG_FtoA_ARENA_TEAM_MATCHING
{
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	UID32_t			AFSCharacterUID;
	ClientIndex_t	AFSClientIndex;
};

struct MSG_FtoA_ARENA_TEAM_MATCHING_OK
{
	ClientIndex_t	AFSClientIndex;
	UID32_t			AccountUID;
	UID32_t			AFSCharacterUID;
};

struct MSG_FtoA_ARENA_SERVER_PAY
{
	BYTE			Influence;
	INT				PayInfluencePoint;
};

struct MSG_FtoA_ARENA_CHARACTER_PAY
{
	BYTE			Issue;					// 2007-04-20 by dhjin, �Ʒ��� ���� ����, 1 : ARENA_ISSUE_WIN, 2 : ARENA_ISSUE_LOSE, 3 : ARENA_ISSUE_DRAW
	INT				PayWarPoint;
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	ClientIndex_t	AFSClientIndex;
};

struct MSG_FtoA_ARENA_CHARACTER_DISCONNECT
{
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	UID32_t			ArenaCharacterUID;	// 2012-10-21 by jhseol, �Ʒ��� ���׼��� - �Ʒ��� ����� �ɸ��� ���� ����.
	ClientIndex_t	MFSClientIndex;
	ClientIndex_t	AFSClientIndex;
};

// 2012-12-18 by jhseol, �Ʒ��� ĳ���� ���� �� ĳ���� ���� �ٽ� �ε��ϱ�.
struct MSG_FtoA_ARENA_CHARACTER_RESET_RELOAD
{
	UID32_t			AccountUID;
	UID32_t			ArenaCharacterUID;
};
// end 2012-12-18 by jhseol, �Ʒ��� ĳ���� ���� �� ĳ���� ���� �ٽ� �ε��ϱ�.

//////////////////////////////////////////////////////////////////////
// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : MF CharacterArena ������Ʈ ��Ŷ
struct MSG_FtoA_ARENA_CHARACTERARENAINFO_UPDATA
{
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	ClientIndex_t	AFSClientIndex;
	SArenaPlayInfo	ArenaInfo;
};
// end 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : MF CharacterArena ������Ʈ ��Ŷ

//////////////////////////////////////////////////////////////////////////
// 2008-03-27 by dhjin, ���� ���� ǥ�� ��ȹ�� - 
//#define T_FC_INFO_MSWARINFO_DISPLAY					(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_MSWARINFO_DISPLAY)					// 2008-03-27 by dhjin, ���� ���� ǥ�� ��ȹ�� - ���� ���� ���� F->C
//#define T_FC_INFO_MSWARINFO_DISPLAY_OPTION			(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_MSWARINFO_DISPLAY_OPTION)			// 2008-03-27 by dhjin, ���� ���� ǥ�� ��ȹ�� - ���� ���� �ɼ� ���� C->F
//#define T_FC_INFO_MSWARINFO_DISPLAY_OPTION_OK		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_MSWARINFO_DISPLAY_OPTION_OK)			// 2008-03-27 by dhjin, ���� ���� ǥ�� ��ȹ�� - ���� ���� �ɼ� ���� F->C(n)
//#define T_FC_INFO_STRATEGICPOINTINFO_DISPLAY_LIST	(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_STRATEGICPOINTINFO_DISPLAY_LIST)		// 2015-05-01 by killburne
//#define T_FC_INFO_STRATEGICPOINTINFO_DISPLAY		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_STRATEGICPOINTINFO_DISPLAY)			// 2015-05-02 by killburne
struct SMSWARINFO_DISPLAY
{// 2008-03-26 by dhjin, ���� ���� ǥ�� ��ȹ�� - �������� �ʿ��� ���� ǥ�� ����Ÿ
	MapIndex_t		MapIndex;
	INT				MapInfluenceType;
	INT				HPRate;
	INT				TelePortState;
	ATUM_DATE_TIME	TelePortBuildingStartTime;
	ATUM_DATE_TIME	TelePortBuildingEndTime;
};

struct MSG_FC_INFO_MSWARINFO_DISPLAY
{// 2008-03-26 by dhjin, ���� ���� ǥ�� ��ȹ�� - �������� �ʿ��� ���� ǥ�� ����Ÿ
	INT				MSWarInfoDisPlayListCount;
	_ARRAY(SMSWARINFO_DISPLAY);
};

struct MSG_FC_INFO_MSWARINFO_DISPLAY_OPTION
{// 2008-03-26 by dhjin, ���� ���� ǥ�� ��ȹ�� - ���� ���� �ɼ�
	SHORT			MSWarOptionType;
	UID32_t			CharacterUID;
	ClientIndex_t	ClientIndex;
};

struct MSG_FC_INFO_MSWARINFO_DISPLAY_OPTION_OK
{// 2008-03-26 by dhjin, ���� ���� ǥ�� ��ȹ�� - ���� ���� �ɼ�
	SHORT			MSWarOptionType;
};

//////////////////////////////////////////////////////////////////////////
// 2008-04-02 by dhjin, ����, ������ ����â ��ȹ�� - 
//#define T_FC_INFO_MSWARINFO_RESULT				(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_MSWARINFO_RESULT)	// 2008-04-02 by dhjin, ����, ������ ����â ��ȹ�� - ���� ��� ���� C->F
//#define T_FC_INFO_MSWARINFO_RESULT_OK			(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_MSWARINFO_RESULT_OK)	// 2008-04-02 by dhjin, ����, ������ ����â ��ȹ�� - ���� ��� ���� F->C
//#define T_FC_INFO_SPWARINFO_RESULT				(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_SPWARINFO_RESULT)	// 2008-04-02 by dhjin, ����, ������ ����â ��ȹ�� - ������ ��� ���� C->F
//#define T_FC_INFO_SPWARINFO_RESULT_OK_HEADER	(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_SPWARINFO_RESULT_OK_HEADER)	// 2008-04-02 by dhjin, ����, ������ ����â ��ȹ�� - ������ ��� ���� F->C
//#define T_FC_INFO_SPWARINFO_RESULT_OK			(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_SPWARINFO_RESULT_OK)	// 2008-04-02 by dhjin, ����, ������ ����â ��ȹ�� - ������ ��� ���� F->C
//#define T_FC_INFO_SPWARINFO_RESULT_OK_DONE		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_SPWARINFO_RESULT_OK_DONE)	// 2008-04-02 by dhjin, ����, ������ ����â ��ȹ�� - ������ ��� ���� F->C
struct MSG_FC_INFO_MSWARINFO_RESULT_OK
{// 2008-04-02 by dhjin, ����, ������ ����â ��ȹ�� - ���� ��� ���� F->C
	INT				MSWarInfoResultListCount;
	_ARRAY(SMSWAR_INFO_RESULT);
};

//////////////////////////////////////////////////////////////////////////
// 2008-04-29 by dhjin, EP3 ���� �ý��� - 
//#define T_FC_CHAT_LETTER_REQUEST_TITLE			(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_LETTER_REQUEST_TITLE)			// 2008-04-29 by dhjin, EP3 ���� �ý��� - ���� ����Ʈ ��û C->F
//#define T_FC_CHAT_LETTER_REQUEST_TITLE_OK_HEADER (MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_LETTER_REQUEST_TITLE_OK_HEADER)	// 2008-04-29 by dhjin, EP3 ���� �ý��� - ���� ����Ʈ ���� F->C
//#define T_FC_CHAT_LETTER_REQUEST_TITLE_OK		(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_LETTER_REQUEST_TITLE_OK)			// 2008-04-29 by dhjin, EP3 ���� �ý��� - ���� ����Ʈ ���� F->C
//#define T_FC_CHAT_LETTER_REQUEST_TITLE_OK_DONE	(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_LETTER_REQUEST_TITLE_OK_DONE)	// 2008-04-29 by dhjin, EP3 ���� �ý��� - ���� ����Ʈ ���� F->C
//#define T_FC_CHAT_LETTER_READ					(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_LETTER_READ)						// 2008-04-29 by dhjin, EP3 ���� �ý��� - ���� �б� ��û C->F
//#define T_FC_CHAT_LETTER_READ_OK				(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_LETTER_READ_OK)					// 2008-04-29 by dhjin, EP3 ���� �ý��� - ���� �б� �Ϸ� F->C
//#define T_FC_CHAT_LETTER_DELETE					(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_LETTER_DELETE)					// 2008-04-29 by dhjin, EP3 ���� �ý��� - ���� ����� ��û C->F
//#define T_FC_CHAT_LETTER_DELETE_OK				(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_LETTER_DELETE_OK)				// 2008-04-29 by dhjin, EP3 ���� �ý��� - ���� ����� �Ϸ� F->C
//#define T_FC_CHAT_LETTER_SEND					(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_LETTER_SEND)						// 2008-05-08 by dhjin, EP3 ���� �ý��� - ���� ���� C->F
//#define T_FC_CHAT_LETTER_SEND_OK				(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_LETTER_SEND_OK)					// 2008-05-08 by dhjin, EP3 ���� �ý��� - ���� ���� �Ϸ� F->C 
//#define T_FC_CHAT_LETTER_RECEIVE				(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_LETTER_RECEIVE)					// 2008-05-08 by dhjin, EP3 ���� �ý��� - ���� ���� ���� F->C
//#define T_FC_CHAT_ALLLETTER_REQUEST_TITLE			(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_ALLLETTER_REQUEST_TITLE)				// 2008-05-09 by dhjin, EP3 ���� �ý��� - ��ü ���� ����Ʈ ��û C->F
//#define T_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK_HEADER (MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK_HEADER)	// 2008-05-09 by dhjin, EP3 ���� �ý��� - ��ü ���� ����Ʈ ���� F->C
//#define T_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK		(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK)			// 2008-05-09 by dhjin, EP3 ���� �ý��� - ��ü ���� ����Ʈ ���� F->C
//#define T_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK_DONE	(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK_DONE)		// 2008-05-09 by dhjin, EP3 ���� �ý��� - ��ü ���� ����Ʈ ���� F->C
//#define T_FC_CHAT_ALLLETTER_READ					(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_ALLLETTER_READ)						// 2008-05-09 by dhjin, EP3 ���� �ý��� - ��ü ���� �б� ��û C->F
//#define T_FC_CHAT_ALLLETTER_READ_OK					(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_ALLLETTER_READ_OK)					// 2008-05-09 by dhjin, EP3 ���� �ý��� - ��ü ���� �б� �Ϸ� F->C
//#define T_FC_CHAT_ALLLETTER_DELETE					(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_ALLLETTER_DELETE)					// 2008-05-09 by dhjin, EP3 ���� �ý��� - ��ü ���� ����� ��û C->F
//#define T_FC_CHAT_ALLLETTER_DELETE_OK				(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_ALLLETTER_DELETE_OK)					// 2008-05-09 by dhjin, EP3 ���� �ý��� - ��ü ���� ����� �Ϸ� F->C
//#define T_FC_CHAT_ALLLETTER_SEND					(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_ALLLETTER_SEND)						// 2008-05-09 by dhjin, EP3 ���� �ý��� - ��ü ���� ���� C->F
//#define T_FC_CHAT_ALLLETTER_SEND_OK					(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_ALLLETTER_SEND_OK)					// 2008-05-09 by dhjin, EP3 ���� �ý��� - ��ü ���� ���� �Ϸ� F->C 
//#define T_FC_CHAT_ALLLETTER_RECEIVE					(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_ALLLETTER_RECEIVE)					// 2008-05-09 by dhjin, EP3 ���� �ý��� - ��ü ���� ���� ���� F->C

struct MSG_FC_CHAT_LETTER_REQUEST_TITLE_OK
{// 2008-04-29 by dhjin, EP3 ���� �ý��� - ���� ����Ʈ ����
	UID64_t			LetterUID;
	CHAR			SendCharacterName[SIZE_MAX_CHARACTER_NAME];
	ATUM_DATE_TIME  SendDate;
	bool			CheckRead;
	CHAR			Title[SIZE_MAX_LETTER_TITLE];
};
using MSG_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK = MSG_FC_CHAT_LETTER_REQUEST_TITLE_OK;

struct MSG_FC_CHAT_LETTER_READ
{// 2008-04-29 by dhjin, EP3 ���� �ý��� - ���� ���� ��û
	UID64_t			LetterUID;
	bool			CheckRead;
};
using MSG_FC_CHAT_ALLLETTER_READ = MSG_FC_CHAT_LETTER_READ;

struct MSG_FC_CHAT_LETTER_READ_OK
{// 2008-04-29 by dhjin, EP3 ���� �ý��� - ���� ���� ����
	UID64_t			LetterUID;
	CHAR			Content[SIZE_MAX_LETTER_CONTENT];
};
using MSG_FC_CHAT_ALLLETTER_READ_OK = MSG_FC_CHAT_LETTER_READ_OK;

struct MSG_FC_CHAT_LETTER_DELETE
{// 2008-04-29 by dhjin, EP3 ���� �ý��� - ���� ����� 
	UID64_t			LetterUID;
};
using MSG_FC_CHAT_ALLLETTER_DELETE = MSG_FC_CHAT_LETTER_DELETE;

struct MSG_FC_CHAT_LETTER_DELETE_OK
{// 2008-04-29 by dhjin, EP3 ���� �ý��� - ���� ����� �Ϸ�
	UID64_t			LetterUID;
};
using MSG_FC_CHAT_ALLLETTER_DELETE_OK = MSG_FC_CHAT_LETTER_DELETE_OK;

struct MSG_FC_CHAT_LETTER_SEND
{// 2008-05-08 by dhjin, EP3 ���� �ý��� - ���� ������
	INT				SendCharacterUID;
	CHAR			RecvCharacterName[SIZE_MAX_CHARACTER_NAME];
	CHAR			Title[SIZE_MAX_LETTER_TITLE];
	CHAR			Content[SIZE_MAX_LETTER_CONTENT];
};
using MSG_FC_CHAT_ALLLETTER_SEND = MSG_FC_CHAT_LETTER_SEND;

struct MSG_FC_CHAT_LETTER_RECEIVE
{// 2008-05-08 by dhjin, EP3 ���� �ý��� - ���� ���� ����
	CHAR				SendCharacterName[SIZE_MAX_CHARACTER_NAME];
};
using MSG_FC_CHAT_ALLLETTER_RECEIVE = MSG_FC_CHAT_LETTER_RECEIVE;

//////////////////////////////////////////////////////////////////////////
// 2008-08-18 by dhjin, ���¸�ũ�̺�Ʈ 
//#define T_FC_EVENT_INFLUENCEMARK		(MessageType_t)((T0_FC_EVENT<<8)|T1_FC_EVENT_INFLUENCEMARK)		
//#define T_FC_EVENT_INFLUENCEMARKEND		(MessageType_t)((T0_FC_EVENT<<8)|T1_FC_EVENT_INFLUENCEMARKEND)
struct MSG_FC_EVENT_INFLUENCEMARK
{
	BYTE			Influence;		// ���¸�ũ�̺�Ʈ����
};

//////////////////////////////////////////////////////////////////////////
// 2008-12-02 by dhjin, �̼Ǹ�����
//#define T_FC_QUEST_REQUEST_MISSIONMASTER_HELP			(MessageType_t)((T0_FC_QUEST<<8)|T1_FC_QUEST_REQUEST_MISSIONMASTER_HELP)		// 2008-12-02 by dhjin, C -> F, �̼Ǹ����� ��û
//#define T_FC_QUEST_REQUEST_MISSIONMASTER_HELP_INVITE	(MessageType_t)((T0_FC_QUEST<<8)|T1_FC_QUEST_REQUEST_MISSIONMASTER_HELP_INVITE)	// 2008-12-02 by dhjin, F -> C(n), �������� ���� �̼Ǹ����Ϳ��� ��û
//#define T_FC_QUEST_MISSIONMASTER_HELP_INVITE			(MessageType_t)((T0_FC_QUEST<<8)|T1_FC_QUEST_MISSIONMASTER_HELP_INVITE)			// 2008-12-02 by dhjin, C -> F, �̼Ǹ����� ��û �¶� 
//#define T_FC_QUEST_MISSIONMASTER_HELP_INVITE_OK			(MessageType_t)((T0_FC_QUEST<<8)|T1_FC_QUEST_MISSIONMASTER_HELP_INVITE_OK)		// 2008-12-02 by dhjin, F -> C, �̼Ǹ����� ��û �¶�
//#define T_FC_QUEST_MISSIONMASTER_HELP_REJECT			(MessageType_t)((T0_FC_QUEST<<8)|T1_FC_QUEST_MISSIONMASTER_HELP_REJECT)			// 2008-12-02 by dhjin, C -> F, �̼Ǹ����� ��û ���� 
//#define T_FC_QUEST_MISSIONMASTER_HELP_REJECT_OK			(MessageType_t)((T0_FC_QUEST<<8)|T1_FC_QUEST_MISSIONMASTER_HELP_REJECT_OK)		// 2008-12-02 by dhjin, F -> C, �̼Ǹ����� ��û ����
struct MSG_FC_QUEST_REQUEST_MISSIONMASTER_HELP
{
	INT				QuestIdx;
};

struct MSG_FC_QUEST_REQUEST_MISSIONMASTER_HELP_INVITE
{
	ClientIndex_t	ClientIdx;
	UID32_t			CharacterUID;		// 2008-12-02 by dhjin, �̼Ǹ����� ��û�� �ɸ���
	char			CharacterName[SIZE_MAX_CHARACTER_NAME];
	PartyID_t		PartyID;
};

struct MSG_FC_QUEST_MISSIONMASTER_HELP_INVITE
{
	UID32_t			CharacterUID;		// 2008-12-02 by dhjin, �̼Ǹ����� ��û�� �ɸ���
};

struct MSG_FC_QUEST_MISSIONMASTER_HELP_INVITE_OK
{
	ClientIndex_t	ClientIdx;
	UID32_t			CharacterUID;		// 2008-12-02 by dhjin, �̼Ǹ����� �ɸ���
	char			CharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_FC_QUEST_MISSIONMASTER_HELP_REJECT
{
	UID32_t			CharacterUID;		// 2008-12-02 by dhjin, �̼Ǹ����� ��û�� �ɸ���
};

struct MSG_FC_QUEST_MISSIONMASTER_HELP_REJECT_OK
{
	ClientIndex_t	ClientIdx;
	UID32_t			CharacterUID;		// 2008-12-02 by dhjin, �̼Ǹ����� �ɸ���
	char			CharacterName[SIZE_MAX_CHARACTER_NAME];
};

//////////////////////////////////////////////////////////////////////////
// 2009-01-12 by dhjin, ���� ���� -
//#define T_FC_INFO_DECLARATION_MSWAR_INFO		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_DECLARATION_MSWAR_INFO)		// 2009-01-12 by dhjin, ���� ���� - �������� ���� ��û C->F
//#define T_FC_INFO_DECLARATION_MSWAR_INFO_OK		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_DECLARATION_MSWAR_INFO_OK)	// 2009-01-12 by dhjin, ���� ���� - �������� ���� ���� F->C
//#define T_FC_INFO_DECLARATION_MSWAR_SET			(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_DECLARATION_MSWAR_SET)	// 2009-01-12 by dhjin, ���� ���� - �������� �ð� �� ���� ���� C->F
//#define T_FI_INFO_DECLARATION_MSWAR_SET_OK		(MessageType_t)((T0_FI_INFO<<8)|T1_FI_INFO_DECLARATION_MSWAR_SET_OK)	// 2009-01-12 by dhjin, ���� ���� - �������� �ð� �� ���� ���� F->I
//#define T_IC_INFO_DECLARATION_MSWAR_SET_OK		(MessageType_t)((T0_IC_INFO<<8)|T1_IC_INFO_DECLARATION_MSWAR_SET_OK)	// 2009-01-12 by dhjin, ���� ���� - �������� �ð� �� ���� ���� I->C
struct MSG_FC_INFO_DECLARATION_MSWAR_INFO_OK
{// 2009-01-12 by dhjin, ���� ���� - �������� ���� ���� F->C
	INT								DeclarationOfWarListCount;
	SDECLARATION_OF_WAR_FORBID_TIME	ForbidTime;
	_ARRAY(SDECLARATION_OF_WAR);
};

struct MSG_FC_INFO_DECLARATION_MSWAR_SET
{// 2009-01-12 by dhjin, ���� ���� - �������� �ð� �� ���� ���� C->F
	BYTE			MSWarStep;
	ATUM_DATE_TIME	MSWarStartTime;
	bool			GiveUp;
};

struct MSG_FI_INFO_DECLARATION_MSWAR_SET_OK
{// 2009-01-12 by dhjin, ���� ���� - �������� �ð� �� ���� ���� F->C
	BYTE			Influence;		// ���� ������ ������ ����.
	ATUM_DATE_TIME	MSWarStartTime;
	bool			GiveUp;
	BYTE			SelectCount;
};
using MSG_IC_INFO_DECLARATION_MSWAR_SET_OK = MSG_FI_INFO_DECLARATION_MSWAR_SET_OK;


///////////////////////////////////////////////////////////////////////////////
// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 
//#define T_FC_INFO_WRK_GET_SERVICE_INFO					(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_WRK_GET_SERVICE_INFO)			// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - C->F
//#define T_FC_INFO_WRK_GET_SERVICE_INFO_OK				(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_WRK_GET_SERVICE_INFO_OK)			// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - F->C
//#define T_FC_INFO_WRK_GET_SERVICE_INFO_OK_IMAGE			(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_WRK_GET_SERVICE_INFO_OK_IMAGE)	// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - F->C
//#define T_FC_INFO_WRK_GET_SERVICE_INFO_OK_DONE			(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_WRK_GET_SERVICE_INFO_OK_DONE)	// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - F->C
//#define T_FC_INFO_WRK_GET_RANKER_LIST					(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_WRK_GET_RANKER_LIST)				// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - C->F
//#define T_FC_INFO_WRK_GET_LEVEL_RANKER_LIST_OK			(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_WRK_GET_LEVEL_RANKER_LIST_OK)	// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - F->C
//#define T_FC_INFO_WRK_GET_FAME_RANKER_LIST_OK			(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_WRK_GET_FAME_RANKER_LIST_OK)		// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - F->C
//#define T_FC_INFO_WRK_GET_PVP_RANKER_LIST_OK			(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_WRK_GET_PVP_RANKER_LIST_OK)		// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - F->C
//#define T_FC_INFO_WRK_GET_SELF_RANKING					(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_WRK_GET_SELF_RANKING)			// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - C->F
//#define T_FC_INFO_WRK_GET_SELF_RANKING_OK				(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_WRK_GET_SELF_RANKING_OK)			// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - F->C
struct MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK
{
	//INT		ServiceUID;
	char	ServiceName[SIZE_MAX_SERVICE_NAME];
	INT		SymbolImageSize;
};
struct MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK_IMAGE
{
	//INT		ServiceUID;
	BYTE	SymbolImageIndex;
	BYTE	SymbolImage[1000];
};
struct MSG_FC_INFO_WRK_GET_RANKER_LIST
{
	BYTE	byRankingType;			// RANKING_TYPE_XXX
	INT		UnitKind;
	BYTE	byScope;				// RANKING_SCOPE_XXX
	BYTE	byStartRank;
	BYTE	byReqRankerCnt;
};
struct MSG_FC_INFO_WRK_GET_LEVEL_RANKER_LIST_OK
{
	BYTE	byStartRank;
	BYTE	byRankerCnt;
	_ARRAY(SWRK_LEVEL);
};
struct MSG_FC_INFO_WRK_GET_FAME_RANKER_LIST_OK
{
	BYTE	byStartRank;
	BYTE	byRankerCnt;
	_ARRAY(SWRK_FAME);

};
struct MSG_FC_INFO_WRK_GET_PVP_RANKER_LIST_OK
{
	BYTE	byStartRank;
	BYTE	byRankerCnt;
	_ARRAY(SWRK_PVP);
};
struct MSG_FC_INFO_WRK_GET_SELF_RANKING_OK
{
	UID32_t	CharacUID;
	INT		LevelRanking;
	INT		FameRanking;
	INT		PvPRanking;
};



///////////////////////////////////////////////////////////////////////////////
// 2009-03-31 by cmkwon, �����ʱ�ȭ �ý��� ���� - 
//#define T_FC_QUEST_INSERT_QUEST			(MessageType_t)((T0_FC_QUEST<<8)|T1_FC_QUEST_INSERT_QUEST)	// 2009-03-31 by cmkwon, �����ʱ�ȭ �ý��� ���� - F->C
//#define T_FC_QUEST_DELETE_QUEST			(MessageType_t)((T0_FC_QUEST<<8)|T1_FC_QUEST_DELETE_QUEST)	// 2009-03-31 by cmkwon, �����ʱ�ȭ �ý��� ���� - F->C

struct MSG_FC_QUEST_INSERT_QUEST
{
	INT		QuestIndex;
	BYTE	QuestState;				// �Ϸ�, ������
};
struct MSG_FC_QUEST_DELETE_QUEST
{
	INT		QuestIndex;
};

////////////////////////////////////////////////////////////////////////////////
// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 
//#define T_FN_MONSTER_HPTALK							(MessageType_t)((T0_FN_MONSTER<<8)|T1_FN_MONSTER_HPTALK)	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - , N->F
struct MSG_FN_MONSTER_HPTALK {
	MAP_CHANNEL_INDEX				ChannelIndex;
	ClientIndex_t					MonsterIndex;				// �ε���
	INT								MonsterUnitKind;			// ���� ������ȣ
	HPValueRate_t					HPValueRate;
	eHPACTION_TALK_IMPORTANCE		HPTalkImportance;
	MSec_t							HPCameraTremble;
	ClientIndex_t					TargetIndex;				// ��� ���õ� ��簡 �ִٸ� �����Ѵ�.	
	HPTalk_t						HPTalk[SIZE_MAX_HPTALK_DESCRIPTION];
};

//#define T_FC_MONSTER_HPTALK							(MessageType_t)((T0_FC_MONSTER<<8)|T1_FC_MONSTER_HPTALK)	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - , F->C(n)
struct MSG_FC_MONSTER_HPTALK {
	ClientIndex_t					MonsterIndex;				// �ε���
	INT								MonsterUnitKind;			// ���� ������ȣ
	eHPACTION_TALK_IMPORTANCE		HPTalkImportance;
	MSec_t							HPCameraTremble;
	ClientIndex_t					TargetIndex;				// ��� ���õ� ��簡 �ִٸ� �����Ѵ�.
	HPTalk_t						HPTalk[SIZE_MAX_HPTALK_DESCRIPTION];
};

//#define T_FN_BATTLE_ATTACK_SKILL					(MessageType_t)((T0_FN_BATTLE<<8)|T1_FN_BATTLE_ATTACK_SKILL)				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - N -> F
struct MSG_FN_BATTLE_ATTACK_SKILL
{
	MAP_CHANNEL_INDEX	MapInfo;
	ClientIndex_t		MonsterIndex;				// ��ų�� ����� ����
	ClientIndex_t		ClientIndex;				// Ŭ���̾�Ʈ �ε���
	INT					SkillItemNum;				// ��ų�� ItemNum
	AVECTOR3			PositionVector;
	AVECTOR3			TargetVector;
	float				fIncreasePowerRatio;		// ��ų�� �Ŀ�(������ or ����/����� ���� ȿ��) ������.		// 2010. 06. 08 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (�Ʊ� ���� �뷱�� ����.) - ���� ��ų �뷱�� ����.
};

//#define T_FC_MONSTER_SKILL							(MessageType_t)((T0_FC_MONSTER<<8)|T1_FC_MONSTER_SKILL)	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - , F->C(n)
struct MSG_FC_MONSTER_SKILL {
	ClientIndex_t		MonsterIndex;				// ��ų�� ����� ����
	ClientIndex_t		ClientIndex;				// Ŭ���̾�Ʈ �ε���
	INT					SkillItemNum;				// ��ų�� ItemNum
	AVECTOR3			PositionVector;
	AVECTOR3			TargetVector;
};

//#define T_FC_BATTLE_MONSTER_BARRIER_USING		(MessageType_t)((T0_FC_BATTLE<<8)|T1_FC_BATTLE_MONSTER_BARRIER_USING)	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - F -> C
struct MSG_FC_BATTLE_MONSTER_BARRIER_USING {
	ClientIndex_t		MonsterIndex;				// ��ų�� ����� ����
};

//#define T_FC_BATTLE_MONSTER_BARRIER_USE			(MessageType_t)((T0_FC_BATTLE<<8)|T1_FC_BATTLE_MONSTER_BARRIER_USE)		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - F -> C(n)
struct MSG_FC_BATTLE_MONSTER_BARRIER_USE {
	ClientIndex_t		MonsterIndex;				// ��ų�� ����� ����
	INT					SkillItemNum;
};

//#define T_FC_CHARACTER_DEBUFF_DOT_INFO			(MessageType_t)((T0_FC_CHARACTER<<8)|T1_FC_CHARACTER_DEBUFF_DOT_INFO)			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - C -> F
//#define T_FC_CHARACTER_DEBUFF_DOT_INFO_OK		(MessageType_t)((T0_FC_CHARACTER<<8)|T1_FC_CHARACTER_DEBUFF_DOT_INFO_OK)		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - F -> C
//#define T_FC_CHARACTER_DEBUFF_DOT_APPLYING		(MessageType_t)((T0_FC_CHARACTER<<8)|T1_FC_CHARACTER_DEBUFF_DOT_APPLYING)		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - F -> C
//#define T_FC_CHARACTER_DEBUFF_DOT_RELEASE		(MessageType_t)((T0_FC_CHARACTER<<8)|T1_FC_CHARACTER_DEBUFF_DOT_RELEASE)		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - F -> C
struct MSG_FC_CHARACTER_DEBUFF_DOT_INFO {
	UID32_t			CharacterUID;
};

struct MSG_DEBUFF_INFO {
	INT					ItemNum;						// ������ ������ȣ, ���� �������� �� (ITEM_BASE*)
};

struct MSG_DOT_INFO {
	INT					ItemNum;						// ������ ������ȣ, ���� �������� �� (ITEM_BASE*)
};

struct MSG_FC_CHARACTER_DEBUFF_DOT_INFO_OK {
	UID32_t				CharacterUID;
	INT					DebuffCount;
	INT					DotCount;
	_ARRAY(MSG_DEBUFF_INFO);
	_ARRAY(MSG_DOT_INFO);
};
struct MSG_FC_CHARACTER_DEBUFF_DOT_APPLYING {
	UID32_t				CharacterUID;
	INT					SkillItemNum;
};

struct MSG_FC_CHARACTER_DEBUFF_DOT_RELEASE {
	UID32_t				CharacterUID;
	INT					SkillItemNum;
};


//#define T_FC_INFINITY_MODE_LIST					(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_MODE_LIST)				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ����Ʈ ��û, C -> F
//#define T_FC_INFINITY_MODE_LIST_OK				(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_MODE_LIST_OK)			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ����Ʈ ��û, F -> C
struct INFINITY_MODE_INFO {
	MapIndex_t		InfinityMapIdx;
	InfiModeUID_t	InfinityModeUID;
	eINFINITY_MODE	InfinityMode;
	Lv_t			MinLv;
	Lv_t			MaxLv;
	bool			Join;
	Minute_t		TimePenaltyValue;		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ����� �г�Ƽ �߰�
	INT				MinMemberCount;			// 2009-09-09 ~ 2010-01-22 by dhjin, ���Ǵ�Ƽ - �ּ�,�ִ��ο� ���� �߰�
	INT				MaxMemberCount;			// 2009-09-09 ~ 2010-01-22 by dhjin, ���Ǵ�Ƽ - �ּ�,�ִ��ο� ���� �߰�
	INT				InfinityDifficultyStep; // 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ������ ���̵� ���� �߰�.
	Penalty_t		HPPenaltyValue;			// 2011-06-14 by hskim, ���Ǵ�Ƽ 3�� - �г�Ƽ ��� �߰� (HP �� �ð� ���� ������ ����)
};
struct MSG_FC_INFINITY_MODE_LIST_OK {
	INT				InfinityModeListCount;
	_ARRAY(INFINITY_MODE_INFO);
};

//#define T_FC_INFINITY_READY_LIST					(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_READY_LIST)				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ����Ʈ ��û, C -> F
//#define T_FC_INFINITY_READY_LIST_OK					(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_READY_LIST_OK)				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ����Ʈ ��û, F -> C
struct MSG_FC_INFINITY_READY_LIST {
	MapIndex_t		InfinityMapIdx;
	eINFINITY_MODE	InfinityMode;
};

struct INFINITY_READY_LIST {
	InfinityCreateUID_t	InfinityCreateUID;
	CHAR				MasterName[SIZE_MAX_CHARACTER_NAME];
	CHAR				InfinityTeamName[SIZE_MAX_PARTY_NAME];
	INT					PlayingRoomMemberCount;
	INT					MaxMemberCount;
	INT					DifficultLevel;
};
struct MSG_FC_INFINITY_READY_LIST_OK {
	INT				InfinityPlayingListCount;
	_ARRAY(INFINITY_READY_LIST);
};

//#define T_FC_INFINITY_CREATE						(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_CREATE)				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� ��û, C -> F
//#define T_FC_INFINITY_CREATE_OK						(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_CREATE_OK)			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� ��û, F -> C
struct MSG_FC_INFINITY_CREATE {
	InfiModeUID_t	InfinityModeUID;
	eINFINITY_MODE	InfinityMode;
	MapIndex_t		MapIndex;
	CHAR			InfinityTeamName[SIZE_MAX_PARTY_NAME];
	INT				InfinityDifficultyLevel;
};
struct MSG_FC_INFINITY_CREATE_OK {
	InfinityCreateUID_t	InfinityCreateUID;

	// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
	INT					IntinityDifficultyLevel;
	// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )

};

//#define T_FC_INFINITY_JOIN							(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_JOIN)						// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� ��û, C -> F
//#define T_FC_INFINITY_JOIN_REQUEST_MASTERUSER		(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_JOIN_REQUEST_MASTERUSER)		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���忡�� ������ ���� ���� ��û, F -> C
//#define T_FC_INFINITY_JOIN_REQUEST_MASTERUSER_OK	(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_JOIN_REQUEST_MASTERUSER_OK)	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���忡�� ������ ���� ���� ��û, C -> F
//#define T_FC_INFINITY_JOIN_OK						(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_JOIN_OK)						// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� ��û, F -> C
//#define T_FC_INFINITY_MEMBER_INFO_LIST				(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_MEMBER_INFO_LIST)			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� �ɹ� ����, C -> F
//#define T_FC_INFINITY_MEMBER_INFO_LIST_OK			(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_MEMBER_INFO_LIST_OK)			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� �ɹ� ����, F -> C
struct MSG_FC_INFINITY_JOIN {
	eINFINITY_MODE		InfinityMode;
	InfinityCreateUID_t	InfinityCreateUID;
	InfiModeUID_t		InfinityModeUID;		// 2010-03-23 by cmkwon, ���Ǵ�Ƽ ���� ĳ�� ������ ���� - 
};
struct MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER {
	ClientIndex_t		ReQuestClientIdx;
	CHAR				ReQuestCharacterName[SIZE_MAX_CHARACTER_NAME];
	USHORT				Gear;
	BYTE				Lv;
};
struct MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER_OK {
	ClientIndex_t		ReQuestClientIdx;
	bool				bAccept;
	eINFINITY_MODE		InfinityMode;
	InfinityCreateUID_t	InfinityCreateUID;
};
struct MSG_FC_INFINITY_JOIN_OK {
	ClientIndex_t		JoinClientIdx;
	CHAR				JoinCharacterName[SIZE_MAX_CHARACTER_NAME];
	USHORT				Gear;
	BYTE				Lv;
	bool				bAccept;
};
struct INFINITY_MEMBER_INFO_LIST {
	ClientIndex_t		ClientIdx;
	CHAR				CharacterName[SIZE_MAX_CHARACTER_NAME];
	USHORT				Gear;
	BYTE				Lv;
	eINFINITY_STATE		State;
};
struct MSG_FC_INFINITY_MEMBER_INFO_LIST_OK {
	ClientIndex_t		MasterClientIdx;
	INT					InfinityMemberListCount;
	_ARRAY(INFINITY_MEMBER_INFO_LIST);
};

//#define T_FC_INFINITY_CHANGE_MASTER					(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_CHANGE_MASTER)			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ������ ����, C -> F
//#define T_FC_INFINITY_CHANGE_MASTER_OK				(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_CHANGE_MASTER_OK)		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ������ ����, F -> C
//#define T_FC_INFINITY_LEAVE							(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_LEAVE)					// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� Ż��, C -> F
//#define T_FC_INFINITY_LEAVE_OK						(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_LEAVE_OK)				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� Ż��, F -> C
//#define T_FC_INFINITY_BAN							(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_BAN)						// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� �߹�, C -> F
//#define T_FC_INFINITY_BAN_OK						(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_BAN_OK)					// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� �߹�, F -> C
struct MSG_FC_INFINITY_CHANGE_MASTER {
	ClientIndex_t		ChangeMasterClientIdx;
};
struct MSG_FC_INFINITY_CHANGE_MASTER_OK {
	ClientIndex_t		ChangeMasterClientIdx;
};
struct MSG_FC_INFINITY_LEAVE_OK {
	ClientIndex_t		LeaveClientIdx;
};
struct MSG_FC_INFINITY_BAN {
	ClientIndex_t		BanClientIdx;
};
struct MSG_FC_INFINITY_BAN_OK {
	ClientIndex_t		BanClientIdx;
};

//#define T_FtoA_INFINITY_IMPUTE_LIST					(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_IMPUTE_LIST)			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� �ͼ� ���� ����, MFS -> AFS
struct MSG_FtoA_INFINITY_IMPUTE_LIST {
	ClientIndex_t	AFSClientIndex;
	INT				InfinityImputeListCount;
	bool			bHaveReentryTicket;			// 2010-03-23 by cmkwon, ���Ǵ�Ƽ ���� ĳ�� ������ ���� - 
	_ARRAY(INFINITY_IMPUTE);
};

//#define T_FC_INFINITY_READY							(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_READY)			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� ��ư ����, C -> F
//#define T_FC_INFINITY_READY_OK						(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_READY_OK)		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� ��ư ����, F -> C
//#define T_FC_INFINITY_READY_CANCEL					(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_READY_CANCEL)	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ������� ��ư ����, C -> F
//#define T_FC_INFINITY_READY_CANCEL_OK				(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_READY_CANCEL_OK)	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ������� ��ư ����, F -> C
//#define T_FC_INFINITY_READY_CANCEL_ALL_OK			(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_READY_CANCEL_ALL_OK)	// // 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.) - ���Ǵ�Ƽ - ���̵� ����� ��� ���� ���. ( AFS -> C )
struct MSG_FC_INFINITY_READY_OK {
	ClientIndex_t		ReadyClientIdx;
};
struct MSG_FC_INFINITY_READY_CANCEL_OK {
	ClientIndex_t		ReadyCancelClientIdx;
};

// 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.) - ���Ǵ�Ƽ �� ���̵� ����� ��� ���� ��� ��Ŷ.
struct MSG_FC_INFINITY_READY_CANCEL_ALL_OK
{
	InfinityCreateUID_t	InfinityCreateUID;
};
// End 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.)

//#define T_FC_INFINITY_START							(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_START)			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� ��ư ����(���� ����), C -> F
//#define T_FtoA_INFINITY_START						(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_START)			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� MainFieldServer�� �˸�, AFS -> MFS
//#define T_FtoA_INFINITY_START_OK					(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_START_OK)			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� MainFieldSever�غ� �Ϸ� ����, MFS -> AFS
//#define T_FC_INFINITY_MAP_LOADING					(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_MAP_LOADING)		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� �� �ε� �������� �˸�, F -> C  <-- ����!!!! ���� �����!
//#define T_FC_INFINITY_MAP_LOADED					(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_MAP_LOADED)		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� �� �ε� �Ϸ�, C -> F
//#define T_FtoA_INFINITY_IMPUTE						(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_IMPUTE)			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� MainFieldServer�� �ͼ� ���� ����, AFS -> MFS
//#define T_FC_INFINITY_ENTER							(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_ENTER)			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� �� ���� �㰡, F -> C
struct MSG_FtoA_INFINITY_START {
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	UID32_t			AFSCharacterUID;
	ClientIndex_t	AFSClientIndex;
};
struct MSG_FtoA_INFINITY_START_OK {
	ClientIndex_t	AFSClientIndex;
	bool			bIsPCBangClient;
};
struct MSG_FC_INFINITY_ENTER {
	ChannelIndex_t	InfinityChannelIdx;
	D3DXVECTOR3		CharacterPosition;
	ATUM_DATE_TIME	StartTime;
	MSec_t			LimitTime;
};
struct MSG_FtoA_INFINITY_IMPUTE {
	InfiModeUID_t	InfinityModeUID;
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	ClientIndex_t	AFSClientIndex;
	bool			CheckExistImpute;
};


//#define T_FC_INFINITY_CINEMA						(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_CINEMA)			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ó׸� ���� ����, F -> C
using MSG_FC_INFINITY_CINEMA = CINEMAINFO;

struct MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE		// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -
{
	MAP_CHANNEL_INDEX		mapChann;
	INT						MonsterUnitKind;
	SummonCnt_t				MonsterSummonCount;			// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -	
	Distance_t				MaxRandomDistance;			// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -
	MONSTER_BALANCE_DATA	MonsterBalanceInfo;			// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ���� �뷱�� ����(Ȯ�� ���� ����).
};

// start 2011-04-28 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ���� ��� �߰� - �ش� ��ä�� Ư�� ���� ����/���� ��� �߰�
struct MSG_FN_NPCSERVER_CINEMA_MONSTER_DESTROY
{
	MAP_CHANNEL_INDEX		mapChann;
	INT						MonsterUnitKind;
};

struct MSG_FN_NPCSERVER_CINEMA_MONSTER_CHANGE
{
	MAP_CHANNEL_INDEX		mapChann;					// ä�� �� ��ȣ
	INT						MonsterUnitKind;			// ������ ���� ��ȣ
	INT						ChangeMonsterUnitKind;		// ����� ���� ��ȣ
};
// end 2011-04-28 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ���� ��� �߰� - �ش� ��ä�� Ư�� ���� ����/���� ��� �߰�

// start 2011-06-02 ���Ǵ�Ƽ 3�� - ���� 6 - �ֱ��� ��ȯ ��� ����
struct MSG_FN_NPCSERVER_CINEMA_MONSTER_REGEN
{
	MAP_CHANNEL_INDEX		mapChann;
	INT						iMonsterUnitKind;
	bool					bRegen;
	MONSTER_BALANCE_DATA	MonsterBalanceInfo;			// 2011-06-21 by hskim ���� �뷱�� ����
};
// end 2011-06-02 ���Ǵ�Ƽ 3�� - ���� 6 - �ֱ��� ��ȯ ��� ����

//#define T_FC_INFINITY_SUCCESS_FIN					(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_SUCCESS_FIN)			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ����, F -> C
//#define T_FC_INFINITY_FAIL_FIN						(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_FAIL_FIN)			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ����, F -> C
//#define T_FC_INFINITY_FIN_OK						(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_FIN_OK)				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� Ȯ�� �� ����(Main������ ���ư�), C -> F
//#define T_FC_INFINITY_READY_FINISH_MAINSVR_START	(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_READY_FINISH_MAINSVR_START)	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - Main������ ���ư� �غ� �Ϸ� ��, F -> C

//#define T_FtoA_INFINITY_UPDATE_ALL_ITEM_COUNT		(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_UPDATE_ALL_ITEM_COUNT)	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� MainFieldServer�� ������ ����ȭ ���� ����, AFS -> MFS
//#define T_FtoA_INFINITY_INSERT_ITEM_HEADER			(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_INSERT_ITEM_HEADER)		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� MainFieldServer�� ������ ����ȭ ���� ����, AFS -> MFS
//#define T_FtoA_INFINITY_INSERT_ITEM					(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_INSERT_ITEM)				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� MainFieldServer�� ������ ����ȭ ���� ����, AFS -> MFS
//#define T_FtoA_INFINITY_INSERT_ITEM_DONE			(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_INSERT_ITEM_DONE)			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� MainFieldServer�� ������ ����ȭ ���� ����, AFS -> MFS
//#define T_FtoA_INFINITY_UPDATE_ITEM_HEADER			(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_UPDATE_ITEM_HEADER)		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� MainFieldServer�� ������ ����ȭ ���� ����, AFS -> MFS
//#define T_FtoA_INFINITY_UPDATE_ITEM					(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_UPDATE_ITEM)				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� MainFieldServer�� ������ ����ȭ ���� ����, AFS -> MFS
//#define T_FtoA_INFINITY_UPDATE_ITEM_DONE			(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_UPDATE_ITEM_DONE)			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� MainFieldServer�� ������ ����ȭ ���� ����, AFS -> MFS
//#define T_FtoA_INFINITY_DELETE_ITEM					(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_DELETE_ITEM)				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� MainFieldServer�� ������ ����ȭ ���� ����, AFS -> MFS
//#define T_FtoA_INFINITY_UPDATE_USER_INFO			(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_UPDATE_USER_INFO)			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� MainFieldServer�� ���� ����ȭ ���� ����, AFS -> MFS
//#define T_FtoA_INFINITY_READY_FINISH_MAINSVR_START  (MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_READY_FINISH_MAINSVR_START)	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - Main������ ���ư� �غ� �Ϸ� ��, MFS -> AFS
//#define T_FtoA_INFINITY_LOG							(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_LOG)						// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� MainFieldServer�� �α� ����, AFS -> MFS
//#define T_FtoA_INFINITY_USING_TIME_LIMIT_ITEM		(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_USING_TIME_LIMIT_ITEM)				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� MainFieldServer�� ������ ����ȭ ���� ����, AFS -> MFS
//#define T_FtoA_INFINITY_COME_BACK_POSTWORK			(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_COME_BACK_POSTWORK)		// 2011-09-29 by hskim, ��Ʈ�� �ý��� 2�� - ���� MainFieldServer�� ���ư��� ���� �� �۾� ó�� ���, AFS -> MFS
//#define T_FtoA_INFINITY_COME_BACK_PREWORK			(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_COME_BACK_PREWORK)		// 2011-09-29 by hskim, ��Ʈ�� �ý��� 2�� - ���� MainFieldServer�� ���ư��� ���� ���� �۾� ó�� ���, AFS -> MFS	
//#define T_FtoA_INFINITY_UPDATE_ITEM_PET				(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_UPDATE_ITEM_PET)			// 2011-09-30 by hskim, ��Ʈ�� �ý��� 2�� - ���� MainFieldServer�� ��Ʈ�� ������ ����ȭ ���� ����, AFS -> MFS

struct MSG_FtoA_INFINITY_UPDATE_ALL_ITEM_COUNT {
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	INT				AllItemCount;
};
struct MSG_FtoA_INFINITY_INSERT_ITEM {
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	INT				ItemNum;
	INT				ItemWindowIndex;
	INT				CurrentCount;
	INT				PrefixCodeNum;
	INT				SuffixCodeNum;
	BYTE			Wear;
	INT				ShapeItemNum;
	INT				UsingTimeStamp;
	ATUM_DATE_TIME	CreatedTime;
	INT				CoolingTimeStamp;	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
};
struct MSG_FtoA_INFINITY_UPDATE_ITEM
{
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	UID64_t			MainSvrItemUID;
	INT				ItemWindowIndex;
	INT				CurrentCount;
	BYTE			Wear;
	INT				UsingTimeStamp;
	INT				CoolingTimeStamp;	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
};

// start 2011-09-30 by hskim, ��Ʈ�� �ý��� 2��
struct MSG_FtoA_INFINITY_UPDATE_ITEM_PET
{
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	UID64_t			MainSvrItemUID;

	tPET_CURRENTINFO	PetCurrentData;		// Arena -> Field �� ������ Field ���������� PetSocketItemUID �� pItemPetSocket �� �����ʹ� �������� �ʴ´�
};
// end 2011-09-30 by hskim, ��Ʈ�� �ý��� 2��

struct MSG_FtoA_INFINITY_DELETE_ITEM {
	//	INT InfinityDeleteItemInfoListCount;
	//	_ARRAY(INFINITY_DELETE_ITEM_INFO);
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	UID64_t			MainSvrItemUID;
};
struct MSG_FtoA_INFINITY_UPDATE_USER_INFO {
	UID32_t				AccountUID;
	UID32_t				MFSCharacterUID;
	ClientIndex_t		MFSClientIndex;
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
struct MSG_FtoA_INFINITY_READY_FINISH_MAINSVR_START {
	ClientIndex_t	AFSClientIndex;
};

struct MSG_FtoA_INFINITY_LOG {
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	ClientIndex_t	AFSClientIndex;
	InfiModeUID_t	InfinityModeUID;
	DeathCnt_t		DeathCount;
	KillCnt_t		KillCount;
	ATUM_DATE_TIME	StartTime;
	ATUM_DATE_TIME	EndTime;
	Experience_t	AllEXP;								// 2010-06-25 by shcho, ���Ǵ�Ƽ ���÷α� ��� - ����� �ִ� ����ġ �߰�
};

struct MSG_FtoA_INFINITY_USING_TIME_LIMIT_ITEM {
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	INT				ItemNum;
	INT				ItemWindowIndex;
	INT				CurrentCount;
	BYTE			Wear;
	INT				ShapeItemNum;
	INT				UsingTimeStamp;
	ATUM_DATE_TIME	CreatedTime;
	UID64_t			MainSvrItemUID;
	INT				CoolingTimeStamp;			// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
};

// start 2011-09-29 by hskim, ��Ʈ�� �ý��� 2��
struct MSG_FtoA_INFINITY_COME_BACK_PREWORK
{
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
};
// end 2011-09-29 by hskim, ��Ʈ�� �ý��� 2��

// start 2012-01-16 by hskim, ��� - ȭ��
struct MSG_FtoA_INFINITY_COME_BACK_POSTWORK
{
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;

	SSTATISTICS_MONEY_UNIT DeltaMoney;
};
// end 2012-01-16 by hskim, ��� - ȭ��

struct MSG_FN_NPCSERVER_NEW_CHANGE_OBJECT {
	MAP_CHANNEL_INDEX	mapChann;
	ObjectIdx_t			DeleteObjectUID;
	ObjectNum_t			NewObjectNum;
};
struct MSG_FN_NPCSERVER_RESET_CHANGE_OBJECT {
	MAP_CHANNEL_INDEX	mapChann;
};

//#define T_FC_INFINITY_TENDER_DROPITEM_INFO		(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_TENDER_DROPITEM_INFO)	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���Ϳ��Լ� ����� Tender ������, F -> C
//#define T_FC_INFINITY_TENDER_START				(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_TENDER_START)			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - Tender ����, F -> C
//#define T_FC_INFINITY_TENDER_PUT_IN_TENDER		(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_TENDER_PUT_IN_TENDER)	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� Tender ��ư ����, C -> F
//#define T_FC_INFINITY_TENDER_PUT_IN_TENDER_OK	(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_TENDER_PUT_IN_TENDER_OK)	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� Tender ��ư ���� ���, F -> C
//#define T_FC_INFINITY_TENDER_RESULT				(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_TENDER_RESULT)			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ������ ���, F -> C
//#define T_FC_INFINITY_TENDER_RESULT_TIMEOVER	(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_TENDER_TIMEOVER)			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - Tender �ð��� �������� �����ڰ� ����, F -> C
//#define T_FC_INFINITY_TENDER_RESULT_ALLGIVEUP	(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_TENDER_ALLGIVEUP)		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ������ ��� ����, F -> C
struct MSG_FC_INFINITY_TENDER_DROPITEM_INFO {
	UINT			ItemFieldIndex;			// ���� ������ ������ �ӽ÷� �����ϴ� ��ȣ
	ItemNum_t		TenderDropItemNum;
	INT				Count;
	INT				PrefixCodeNum;
	INT				SuffixCodeNum;
	ATUM_DATE_TIME	TenderStartTime;		// ���� ���� �ð�.
};
struct MSG_FC_INFINITY_TENDER_START {
	UINT			ItemFieldIndex;			// ���� ������ ������ �ӽ÷� �����ϴ� ��ȣ
	ItemNum_t		TenderItemNum;
	INT				Count;
};
struct MSG_FC_INFINITY_TENDER_PUT_IN_TENDER {
	ItemNum_t			TenderDropItemNum;	// 2010-06-25 by shcho, ���Ǵ�Ƽ ���÷α� ��� - �޼��� �߰�
	eINFINITY_MODE		InfinityMode;
	InfinityCreateUID_t	InfinityCreateUID;
	UINT				ItemFieldIndex;			// ���� ������ ������ �ӽ÷� �����ϴ� ��ȣ
	bool				GiveUp;
};
struct MSG_FC_INFINITY_TENDER_PUT_IN_TENDER_OK {
	UINT				ItemFieldIndex;			// ���� ������ ������ �ӽ÷� �����ϴ� ��ȣ
	ClientIndex_t		DiceUserIdx;			// ���� ����
	DiceCnt_t			DiceCount;				// �ֻ��� ��� ��.
};
struct MSG_FC_INFINITY_TENDER_RESULT {
	UINT				ItemFieldIndex;			// ���� ������ ������ �ӽ÷� �����ϴ� ��ȣ
	ClientIndex_t		DiceUserIdx;			// ���� ����
	DiceCnt_t			DiceCount;				// �ֻ��� ��� ��.
	ItemNum_t			TenderItemNum;
	INT					Count;
};
struct MSG_FC_INFINITY_TENDER_RESULT_TIMEOVER {
	UINT				ItemFieldIndex;			// ���� ������ ������ �ӽ÷� �����ϴ� ��ȣ
};
struct MSG_FC_INFINITY_TENDER_RESULT_ALLGIVEUP {
	UINT				ItemFieldIndex;			// ���� ������ ������ �ӽ÷� �����ϴ� ��ȣ
};

//#define T_FC_INFO_APPLY_RESISTANCE_ITEM	(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_APPLY_RESISTANCE_ITEM)	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ������ ���� ���� ����, F -> C(n)
struct MSG_FC_INFO_APPLY_RESISTANCE_ITEM {
	ClientIndex_t		ResistanceUserIdx;
	ItemNum_t			SkillNum;			// 2009-09-09 ~ 2010-01-19 by dhjin, ���Ǵ�Ƽ - ���׵� ��ų ���� �߰�
	bool				OnceResistanceItem;	// 2009-09-09 ~ 2010-01-19 by dhjin, ���Ǵ�Ƽ - �� ���� �����ϰ� �������� ���� ������ �߰�
};

//#define T_FC_SHOP_INFINITY_ITEM_HEADER		(MessageType_t)((T0_FC_SHOP<<8)|T1_FC_SHOP_INFINITY_ITEM_HEADER)	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ����
//#define T_FC_SHOP_INFINITY_ITEM				(MessageType_t)((T0_FC_SHOP<<8)|T1_FC_SHOP_INFINITY_ITEM)			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ����
//#define T_FC_SHOP_INFINITY_ITEM_DONE		(MessageType_t)((T0_FC_SHOP<<8)|T1_FC_SHOP_INFINITY_ITEM_DONE)		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ����
using MSG_FC_SHOP_INFINITY_ITEM = INFINITY_SHOP_INFO;
//#define T_FC_SHOP_INFINITY_BUY_ITEM			(MessageType_t)((T0_FC_SHOP<<8)|T1_FC_SHOP_INFINITY_BUY_ITEM)		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ����, ����
struct MSG_FC_SHOP_INFINITY_BUY_ITEM {
	InfinityShopUID_t		InfinityShopUID;
	int						Amount;
};

//#define T_FC_INFINITY_CHANGE_LIMITTIME				(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_CHANGE_LIMITTIME)		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ����� �г�Ƽ �߰�, F -> C
struct MSG_FC_INFINITY_CHANGE_LIMITTIME {
	char			DeadCharactarName[SIZE_MAX_CHARACTER_NAME];		// 2009-09-09 ~ 2010-01-13 by dhjin, ���Ǵ�Ƽ - ���� ���� �̸� ���� ���� �߰�
	MSec_t			LimitTime;
};

// start 2011-05-30 by hskim, ���Ǵ�Ƽ 3�� - �÷��� �ð� �缳�� ��� 
//#define T_FC_INFINITY_SET_LIMITTIME				(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_SET_LIMITTIME)		// 2011-05-30 by hskim, ���Ǵ�Ƽ 3�� - �÷��� �ð� �缳�� ���
struct MSG_FC_INFINITY_SET_LIMITTIME {
	MSec_t			LimitTime;		// ms
};
// end 2011-05-30 by hskim, ���Ǵ�Ƽ 3�� - �÷��� �ð� �缳�� ���

//#define T_FC_INFINITY_JOIN_CANCEL							(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_JOIN_CANCEL)							// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� ���
//#define T_FC_INFINITY_JOIN_CANCEL_REQUEST_MASTERUSER		(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_JOIN_CANCEL_REQUEST_MASTERUSER)		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� ���
struct MSG_FC_INFINITY_JOIN_CANCEL {
	eINFINITY_MODE		InfinityMode;
	InfinityCreateUID_t	InfinityCreateUID;
};
struct MSG_FC_INFINITY_JOIN_CANCEL_REQUEST_MASTERUSER {
	ClientIndex_t		JoinCancelClientIdx;
};

///////////////////////////////////////////////////////////////////////////////
// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��
//#define T_FC_INFINITY_REQUEST_RESTART_BY_DISCONNECT					(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_REQUEST_RESTART_BY_DISCONNECT)			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��, MFS -> C
//#define T_FC_INFINITY_RESTART_BY_DISCONNECT							(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_RESTART_BY_DISCONNECT)					// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��, C -> AFS
//#define T_FC_INFINITY_MAP_LOADED_RESTART_BY_DISCONNECT				(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_MAP_LOADED_RESTART_BY_DISCONNECT)		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��, C -> AFS
//#define T_FC_INFINITY_DELETED_CINEMA_HEADER							(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_DELETED_CINEMA_HEADER)					// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��
//#define T_FC_INFINITY_DELETED_CINEMA								(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_DELETED_CINEMA)							// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��
//#define T_FC_INFINITY_DELETED_CINEMA_DONE							(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_DELETED_CINEMA_DONE)						// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��
//#define T_FC_INFINITY_ENTER_BY_DISCONNECT							(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_ENTER_BY_DISCONNECT)						// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��
using MSG_FC_INFINITY_DELETED_CINEMA = CINEMAINFO;
struct MSG_FC_INFINITY_ENTER_BY_DISCONNECT {
	eINFINITY_MODE		InfinityMode;
	Minute_t			TimePenaltyValue;
	ChannelIndex_t		InfinityChannelIdx;
	D3DXVECTOR3			CharacterPosition;
	ATUM_DATE_TIME		StartTime;
	MSec_t				LimitTime;
	MSec_t				DecreaseTimeByDead;		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��
	InfinityCreateUID_t	InfinityCreateUID;	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��
	ClientIndex_t		MasterClientIdx;	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��
	Penalty_t			HPPenaltyValue;			// 2011-06-14 by hskim, ���Ǵ�Ƽ 3�� - �г�Ƽ ��� �߰� (HP �� �ð� ���� ������ ����)
};

//#define T_FtoA_INFINITY_STATE_CHANGE			(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_STATE_CHANGE)		// 2009-09-09 ~ 2010-01-20 by dhjin, ���Ǵ�Ƽ - ���� ���� ���� ���μ����� �����Ѵ�.
struct MSG_FtoA_INFINITY_STATE_CHANGE {
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	eINFINITY_STATE	AFSInfinityState;
	ClientIndex_t	AFSClientIndex;
};

// start 2011-05-17 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ����
//#define T_FC_INFINITY_MOVIE						(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_MOVIE)				// 2011-05-17 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ����, F -> C

struct MSG_FC_INFINITY_MOVIE
{
	INT			UniqueNumber;			// ���� ��ȣ
	USHORT		Command;				// MOVIE_COMMAND_START, MOVIE_COMMAND_END
};
// end 2011-05-17 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ����

//#define T_FI_PARTY_AUTO_CREATE						(MessageType_t)((T0_FI_PARTY<<8)|T1_FI_PARTY_AUTO_CREATE)					// 2009-09-09 ~ 2010-01-26 by dhjin, ���Ǵ�Ƽ - �ڵ� ��� ���� ��û, F -> I
//#define T_FI_PARTY_AUTO_CREATE_OK					(MessageType_t)((T0_FI_PARTY<<8)|T1_FI_PARTY_AUTO_CREATE_OK)				// 2009-09-09 ~ 2010-01-26 by dhjin, ���Ǵ�Ƽ - �ڵ� ��� ���� �Ϸ�, I -> F
//#define T_FC_PARTY_AUTO_CREATED						(MessageType_t)((T0_FC_PARTY<<8)|T1_FC_PARTY_AUTO_CREATED)					// 2009-09-09 ~ 2010-01-26 by dhjin, ���Ǵ�Ƽ - �ڵ� ��� ���� �Ϸ� �˸�, F -> C
//#define T_IC_PARTY_GET_AUTO_PARTY_INFO				(MessageType_t)((T0_IC_PARTY<<8)|T1_IC_PARTY_GET_AUTO_PARTY_INFO)			// 2009-09-09 ~ 2010-01-26 by dhjin, ���Ǵ�Ƽ - �ڵ� ��� ���� ��û, C -> I
//#define T_IC_PARTY_GET_AUTO_PARTY_INFO_OK			(MessageType_t)((T0_IC_PARTY<<8)|T1_IC_PARTY_GET_AUTO_PARTY_INFO_OK)		// 2009-09-09 ~ 2010-01-26 by dhjin, ���Ǵ�Ƽ - �ڵ� ��� ���� ����, I -> C
struct MSG_FI_PARTY_AUTO_CREATE {
	INT				PartyMemberListCount;
	UID32_t			MasterUID;
	_ARRAY(UID32_t);
};
struct MSG_FI_PARTY_AUTO_CREATE_OK {
	PartyID_t				PartyID;
	MEX_FIELD_PARTY_INFO	FieldPartyInfo;
	INT						PartyMemberListCount;
	UID32_t					MasterUID;
	_ARRAY(UID32_t);
};
struct MSG_FC_PARTY_AUTO_CREATED {
	PartyID_t				PartyID;
};
//////////////////////////////////////////////////////////////////////
// 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ڵ���Ƽ : �ڵ����� ���� ��Ƽ�� ����.
struct MSG_FC_PARTY_AUTO_CREATED_THE_ARENA {
	SPARTY_INFO				PartyInfo;
};
// end 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ڵ���Ƽ : �ڵ����� ���� ��Ƽ�� ����.

struct MSG_IC_PARTY_GET_AUTO_PARTY_INFO {
	PartyID_t				PartyID;
};
struct MSG_IC_PARTY_GET_AUTO_PARTY_INFO_OK {
	PartyID_t				PartyID;
	SPARTY_INFO				PartyInfo;
	UID32_t					MasterUniqueNumber;			// ��Ƽ���� CharacterUniqueNumber
	UINT					nNumOfPartyMembers;
	ARRAY_(IM_PARTY_MEMBER_INFO);
};

//#define T_FI_PARTY_DISCONNECT_LEAVE_OK		(MessageType_t)((T0_FI_PARTY<<8)|T1_FI_PARTY_DISCONNECT_LEAVE_OK)	// 2009-09-09 ~ 2010-01-27 by dhjin, ���Ǵ�Ƽ - ���� ���� ���� ��Ƽ���� Ż��ó��!
using MSG_FI_PARTY_DISCONNECT_LEAVE_OK = MSG_FI_PARTY_LEAVE_OK;

///////////////////////////////////////////////////////////////////////////////
// 2010-01-26 by cmkwon, ĳ�� ������ �����Ǹ� �ý��� ���� - 
using vectITEM_W_COUNT = vector<ITEM_W_COUNT>;
using mtvectITEM_W_COUNT = mt_vector<ITEM_W_COUNT>;

///////////////////////////////////////////////////////////////////////////////
// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
//#define T_FC_ITEM_USE_INVOKING_WEAR_ITEM	(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_USE_INVOKING_WEAR_ITEM)	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ������������� ���, C->F
struct MSG_FC_ITEM_USE_INVOKING_WEAR_ITEM {
	ItemUID_t				InvokingWearItemUID;
};

//#define T_FC_ITEM_EXPIRE_TIME_INVOKING_WEAR_ITEM	(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_EXPIRE_TIME_INVOKING_WEAR_ITEM)	// C->F, 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ������������� ���� �ð� �Ϸ�
struct MSG_FC_ITEM_EXPIRE_TIME_INVOKING_WEAR_ITEM {
	ItemUID_t				InvokingWearItemUID;
};

//#define T_FC_INFO_APPLY_DESTPARAM		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_APPLY_DESTPARAM)			// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������, DestParam ���� ���� ���� ����
struct MSG_FC_INFO_APPLY_DESTPARAM {
	ClientIndex_t			ApplyClientIdx;
	DestParam_t				ApplyDestParam;
	EffectIdx_t				ApplyEffectIdx;
	ItemUID_t				ApplyItemUID;
};

//#define T_FC_INFO_APPLY_DESTPARAM_LIST	(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_APPLY_DESTPARAM_LIST)			// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������, DestParam ���� ���� ���� ����
struct APPLY_DESTPARAM_INFO {
	DestParam_t				ApplyDestParam;
	EffectIdx_t				ApplyEffectIdx;
};
struct MSG_FC_INFO_APPLY_DESTPARAM_LIST {
	ClientIndex_t			ApplyClientIdx;
	ItemUID_t				ApplyItemUID;
	UINT					ApplyDestParamCnt;
	ARRAY_(APPLY_DESTPARAM_INFO);
};

//#define T_FC_ITEM_END_COOLINGTIME_ITEM		(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_END_COOLINGTIME_ITEM)	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������, ��Ÿ�� ���� C->F
struct MSG_FC_ITEM_END_COOLINGTIME_ITEM {
	ItemUID_t				ItemUID;
};

//#define T_FC_ITEM_END_COOLINGTIME_ITEM_OK		(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_END_COOLINGTIME_ITEM_OK)	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������, ��Ÿ�� ���� F->C
struct MSG_FC_ITEM_END_COOLINGTIME_ITEM_OK {
	ItemUID_t				ItemUID;
};

//#define T_FC_ITEM_GET_COOLINGTIME_INFO		(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_GET_COOLINGTIME_INFO)	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������, ��Ÿ�� ���� ��û C->F
//#define T_FC_ITEM_GET_COOLINGTIME_INFO_OK	(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_GET_COOLINGTIME_INFO_OK)	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������, ��Ÿ�� ���� ��û F->C
struct MSG_FC_ITEM_GET_COOLINGTIME_INFO_OK {
	UID64_t			ItemUID;
	INT				CoolingTimeStamp;		// ���ݱ��� ���� �ð�(����:��)
};

//#define T_FC_ITEM_USE_INVOKING_WEAR_ITEM_BUFF		(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_USE_INVOKING_WEAR_ITEM_BUFF)	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������, ���� �ø����� F->C
struct MSG_FC_ITEM_USE_INVOKING_WEAR_ITEM_BUFF {
	ItemUID_t				ItemUID;
};

///////////////////////////////////////////////////////////////////////////////
// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -
//#define T_FC_INFINITY_CHANGE_ALIVE_FOR_GAMECLEAR_MONSTERHP			(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_CHANGE_ALIVE_FOR_GAMECLEAR_MONSTERHP)		// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - ���� ����� �г�Ƽ, F -> C
struct MSG_FC_INFINITY_CHANGE_ALIVE_FOR_GAMECLEAR_MONSTERHP {
	char			DeadCharactarName[SIZE_MAX_CHARACTER_NAME];
	MonIdx_t		MonsterNum;
	ClientIndex_t	MonsterIdx;
	float			CurrentHP;
};

//#define T_FC_MONSTER_CREATED_ALIVE_FOR_GAMECLEAR					(MessageType_t)((T0_FC_MONSTER<<8)|T1_FC_MONSTER_CREATED_ALIVE_FOR_GAMECLEAR)		// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - F -> C
struct MSG_FC_MONSTER_CREATED_ALIVE_FOR_GAMECLEAR
{
	MonIdx_t		MonsterNum;
	ClientIndex_t	MonsterIdx;
	float			CurrentHP;

	// 2010. 06. 08 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (�Ʊ� ���� �뷱�� ����.) - MAX HP �߰�.
	float			MaxHP;
};


//#define T_FC_ITEM_UPDATE_TRANSFORMER_OK				(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_UPDATE_TRANSFORMER_OK)	// F->C(n), // 2010-03-18 by cmkwon, ���ͺ��� ���� - 
struct MSG_FC_ITEM_UPDATE_TRANSFORMER_OK		// 2010-03-18 by cmkwon, ���ͺ��� ���� - 
{
	UID32_t			CharacUID;
	ClientIndex_t	ClientIdx;
	INT				MonsterUnitKind;
};

//#define T_FI_PARTY_UPDATE_ITEM_TRANSFORMER_OK			(MessageType_t)((T0_FI_PARTY<<8)|T1_FI_PARTY_UPDATE_ITEM_TRANSFORMER_OK)	// F->I, // 2010-03-18 by cmkwon, ���ͺ��� ���� - 
using MSG_FI_PARTY_UPDATE_ITEM_TRANSFORMER_OK = MSG_FC_ITEM_UPDATE_TRANSFORMER_OK;	// 2010-03-18 by cmkwon, ���ͺ��� ���� - 

																					//#define T_IC_PARTY_UPDATE_ITEM_TRANSFORMER_OK			(MessageType_t)((T0_IC_PARTY<<8)|T1_IC_PARTY_UPDATE_ITEM_TRANSFORMER_OK)	// I -> C(n), // 2010-03-18 by cmkwon, ���ͺ��� ���� - 
using MSG_IC_PARTY_UPDATE_ITEM_TRANSFORMER_OK = MSG_FC_ITEM_UPDATE_TRANSFORMER_OK;	// 2010-03-18 by cmkwon, ���ͺ��� ���� - 

																					///////////////////////////////////////////////////////////////////////////////
																					// 2010-03-23 by cmkwon, ���Ǵ�Ƽ ���� ĳ�� ������ ���� - 
																					//#define T_FtoA_INFINITY_START_CHECK			(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_START_CHECK)		// AF->MF, // 2010-03-23 by cmkwon, ���Ǵ�Ƽ ���� ĳ�� ������ ���� - 
																					//#define T_FtoA_INFINITY_START_CHECK_ACK		(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_START_CHECK_ACK)	// MF->AF, // 2010-03-23 by cmkwon, ���Ǵ�Ƽ ���� ĳ�� ������ ���� - 
struct MSG_FtoA_INFINITY_START_CHECK		// 2010-03-23 by cmkwon, ���Ǵ�Ƽ ���� ĳ�� ������ ���� - 
{
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	ClientIndex_t	AFSClientIndex;
	bool			bCheckReentryTicket;	// 
	eINFINITY_MODE		InfinityMode;
	InfinityCreateUID_t	InfinityCreateUID;
};

struct MSG_FtoA_INFINITY_START_CHECK_ACK		// 2010-03-23 by cmkwon, ���Ǵ�Ƽ ���� ĳ�� ������ ���� - 
{
	Err_t			ErrorCode;
	UID32_t			AccountUID;
	ClientIndex_t	AFSClientIndex;
	eINFINITY_MODE		InfinityMode;
	InfinityCreateUID_t	InfinityCreateUID;
};

//#define T_FN_BATTLE_ATTACK_SKILL_CANCEL					(MessageType_t)((T0_FN_BATTLE<<8)|T1_FN_BATTLE_ATTACK_SKILL_CANCEL)				// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - N -> F
struct MSG_FN_BATTLE_ATTACK_SKILL_CANCEL
{
	MAP_CHANNEL_INDEX	MapInfo;
	ClientIndex_t		MonsterIndex;
	INT					SkillItemNum;				// ��ų�� ItemNum
};

//#define T_FC_MONSTER_SKILL_CANCEL							(MessageType_t)((T0_FC_MONSTER<<8)|T1_FC_MONSTER_SKILL_CANCEL)			// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - , F->C(n)
struct MSG_FC_MONSTER_SKILL_CANCEL {
	ClientIndex_t		MonsterIndex;
	INT					SkillItemNum;				// ��ų�� ItemNum
};

//#define T_FtoA_UPDATE_ITEM_NOTI			(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_UPDATE_ITEM_NOTI)		//	MFS -> AFS // 2010-03-31 by dhjin, ���Ǵ�Ƽ ���� ĳ�� ������ ���� - 
struct MSG_FtoA_UPDATE_ITEM_NOTI {
	ClientIndex_t		AFSClientIdx;
	ItemNum_t			ItemNum;
};

//#define T_FtoA_INFINITY_UPDATE_USER_MAP_INFO	(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_UPDATE_USER_MAP_INFO)	//	AFS -> MFS // 2010-04-06 by cmkwon, ����2�� �߰� ���� - 
struct MSG_FtoA_INFINITY_UPDATE_USER_MAP_INFO
{
	UID32_t				AccountUID;
	UID32_t				MFSCharacterUID;
	ClientIndex_t		MFSClientIndex;
	MapIndex_t			InfinityMapIndex;				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��
	ChannelIndex_t		InfinityChannelIndex;			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��
};


// 2010-05-04 by shcho, ���Ǵ�Ƽ ���̵� ���� start
//#define T_FC_INFINITY_DIFFICULTY_LIST		(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_DIFFICULTY_LIST) // C -> AFS // 2010-05-04 by shcho, ���̵� ���� ��� ��û
//#define T_FC_INFINITY_DIFFICULTY_LIST_OK		(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_DIFFICULTY_LIST_OK) // AFS -> C // 2010-05-04 by shcho, Ŭ���̾�Ʈ ���̵� ���� ��� ��û�� ����
struct MSG_INFINITY_DIFFICULTY_LIST_OK
{
	int Difficulty_List_count; //����Ʈ�� �� ������ ����.
	_ARRAY(INFINITY_DIFFICULTY_BONUS_INFO);
};
// 2010-05-04 by shcho, ���Ǵ�Ƽ ���̵� ���� end


// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
//#define T_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL		(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL)		// C -> AFS // 2010-05-24 by hsLee, ���̵� ���� ��û.
//#define T_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL_OK	(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL_OK)	// AFS -> C // 2010-05-24 by hsLee, ���̵� ���� ��û ���.

struct MSG_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL
{
	INT	InfinityDifficultyLevel;
};

// 2010. 06. 01 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (����Ʈ �ڽ� �߰� + ���̵� ���� ��� ��Ŷ ����.)
struct MSG_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL_OK
{
	bool bUpdate;									// Ŭ���̾�Ʈ �޽��� ó���� ���� Flag.
	INT InfinityDifficultyLevel;
};
// End 2010. 06. 01 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (����Ʈ �ڽ� �߰� + ���̵� ���� ��� ��Ŷ ����.)

// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )

// 2010-06-01 by shcho, GLogDB ���� -
//#define T_FL_LOG_EVENT_PARTICIPATION_RATE		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_EVENT_PARTICIPATION_RATE)			// F -> L // 2010-06-01 by shcho, GLogDB ���� -
struct MSG_FL_LOG_EVENT_PARTICIPATION_RATE
{
	ATUM_DATE_TIME		StartTime;
	ATUM_DATE_TIME		EndTime;
	INT					ParticipationCount;
	USHORT				ParticipationRate;
	CHAR				Description[SIZE_MAX_GLOG_EVENT_DESCRIPTION];
};

// 2010-06-01 by shcho, PC�� ���� ȹ��(ĳ��) ������ - 
//#define T_FC_ITEM_HOMEPREMIUM_INFO			(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_HOMEPREMIUM_INFO)	// F->C, // 2010-06-01 by shcho, PC�� ���� ȹ��(ĳ��) ������ -
struct MSG_FC_ITEM_HOMEPREMIUM_INFO
{
	bool bUse; // 0:FALSE  1:TRUE
};

///////////////////////////////////////////////////////////////////////////////////////
// start 2011-08-22 by hskim, ��Ʈ�� �ý��� 2�� - ��� ����
//
// ���� 
//
//
//#define T_FC_ITEM_PET_HEADER				(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_HEADER)			// 2010-06-15 by shcho&hslee ��ý��� - Ŭ���̾�Ʈ ��Ŷ ���� Ŀ�ǵ� �߰� 
//#define T_FC_ITEM_PET						(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET)					// 2010-06-15 by shcho&hslee ��ý��� - Ŭ���̾�Ʈ ��Ŷ ���� Ŀ�ǵ� �߰� 
//#define T_FC_ITEM_PET_BASEDATA_OK			(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_BASEDATA_OK)		// 2010-06-15 by shcho&hslee ��ý��� - Ŭ���̾�Ʈ ��Ŷ ���� Ŀ�ǵ� �߰� 
//#define T_FC_ITEM_PET_SKILLDATA_OK			(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_SKILLDATA_OK)	// 2010-06-15 by shcho&hslee ��ý��� - Ŭ���̾�Ʈ ��Ŷ ���� Ŀ�ǵ� �߰� 
//#define T_FC_ITEM_PET_SOCKETDATA_OK			(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_SOCKETDATA_OK)	// 2010-06-15 by shcho&hslee ��ý��� - Ŭ���̾�Ʈ ��Ŷ ���� Ŀ�ǵ� �߰� 
//#define T_FC_ITEM_PET_DONE					(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_DONE)			// 2010-06-15 by shcho&hslee ��ý��� - Ŭ���̾�Ʈ ��Ŷ ���� Ŀ�ǵ� �߰� 
//
// 2010-06-15 by shcho&hslee ��ý��� - �� �⺻ ��ġ ���� ����ü
// struct MSG_FC_ITEM_PET_BASEDATA
// {
// 	BYTE			ItemUpdateType;										// ������ ���� Ÿ��, IUT_XXX
// 
// 	UID64_t			CreatedPetUID;
// 	
// 	bool			EnableEditPetName;
// 	bool			EnableLevelUp;
// 	
// 	char			szPetName[SIZE_MAX_PET_NAME];
// 	
// 	INT				PetIndex;
// 	INT				PetLevel;
// 	
// 	Experience_t	PetExp;
// 	
// 	FLOAT			Stamina;
// 	
// 	INT				SourceIndex_Field;
// 	INT				SourceIndex_City;
// 
// 	ItemNum_t		ItemIndex_PetSkill[SIZE_MAX_PETSKILLITEM];
// 	
// //	ITEM			sItem_PetSkill[SIZE_MAX_PETSKILLITEM];
// //	ITEM			sItem_PetSocketItem[SIZE_MAX_PETSOCKETITEM];
// };
// 
// 
// // 2010-06-15 by shcho&hslee ��ý��� - �� ��ų ���� ����ü
// struct MSG_FC_ITEM_PET_SKILLDATA
// {
// 	UID64_t			CreatePetUID;
// 
// 	INT				iSlotIndex;
// 
// 	ITEM			sItem_PetSkill;
// };
// 
// 
// // 2010-06-15 by shcho&hslee ��ý��� - �� ���� ���� ����ü
// struct MSG_FC_ITEM_PET_SOCKETDATA
// {
// 	UID64_t			CreatePetUID;
// 	
// 	INT				iSlotIndex;
// 	
// 	ITEM			sItem_PetSocket;
// };

/////////
// ����

//#define T_FC_ITEM_PET_HEADER				(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_HEADER)
//#define T_FC_ITEM_PET						(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET)	
//#define T_FC_ITEM_PET_BASEDATA_OK			(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_BASEDATA_OK)
//#define T_FC_ITEM_PET_DONE					(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_DONE)

//#define T_FC_ITEM_PET_SET_NAME				(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_SET_NAME)
//#define T_FC_ITEM_PET_SET_NAME_OK			(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_SET_NAME_OK)
//#define T_FC_ITEM_PET_SET_EXP_RATIO			(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_SET_EXP_RATIO)
//#define T_FC_ITEM_PET_SET_EXP_RATIO_OK		(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_SET_EXP_RATIO_OK)
//#define T_FC_ITEM_PET_CHANGE_LEVEL			(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_CHANGE_LEVEL)
//#define T_FC_ITEM_PET_CHANGE_EXP			(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_CHANGE_EXP)

//#define T_FC_ITEM_PET_SET_SOCKET			(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_SET_SOCKET)
//#define T_FC_ITEM_PET_SET_SOCKET_OK			(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_SET_SOCKET_OK)
//#define T_FC_ITEM_PET_SET_KIT_SLOT			(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_SET_KIT_SLOT)
//#define T_FC_ITEM_PET_SET_KIT_SLOT_OK		(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_SET_KIT_SLOT_OK)
//#define T_FC_ITEM_PET_SET_AUTOSKILL_SLOT	(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_SET_KIT_AUTOSKILL_SLOT)
//#define T_FC_ITEM_PET_SET_AUTOSKILL_SLOT_OK	(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_SET_KIT_AUTOSKILL_SLOT_OK)

struct MSG_FC_ITEM_PET_BASEDATA
{
	BYTE			ItemUpdateType;										// ������ ���� Ÿ��, IUT_XXX
	UID64_t			CreatedPetUID;
	char			PetName[SIZE_MAX_PET_NAME];
	INT				PetIndex;
	INT				PetLevel;
	Experience_t	PetExp;
	BYTE			PetExpRatio;
	BYTE			PetEnableSocketCount;

	UID64_t			PetSocketItemUID[SIZE_MAX_PETSOCKET];

	SPET_KIT_SLOT_DATA			PetKitHP;
	SPET_KIT_SLOT_DATA			PetKitShield;
	SPET_KIT_SLOT_DATA			PetKitSP;

	SPET_AUTOSKILL_SLOT_DATA	PetAutoSkill;
};

// �̸� ����

struct MSG_FC_ITEM_PET_SET_NAME
{
	UID64_t			ItemUniqueNumber;					// ��Ʈ�� ������ UID
	char			PetName[SIZE_MAX_PET_NAME];			// ����� ��Ʈ�� �̸�
};

struct MSG_FC_ITEM_PET_SET_NAME_OK
{
	UID64_t			ItemUniqueNumber;					// ��Ʈ�� ������ UID
	char			PetName[SIZE_MAX_PET_NAME];			// ����� ��Ʈ�� �̸�
};

// ����ġ ȹ�� ���� ����

struct MSG_FC_ITEM_PET_SET_EXP_RATIO
{
	UID64_t			ItemUniqueNumber;					// ��Ʈ�� ������ UID
	BYTE			ExpRatio;							// ����ġ ȹ�� ����
};

struct MSG_FC_ITEM_PET_SET_EXP_RATIO_OK
{
	UID64_t			ItemUniqueNumber;					// ��Ʈ�� ������ UID
	BYTE			ExpRatio;							// ����ġ ȹ�� ����
};

// ����ġ ���� �� ���� ��

struct MSG_FC_ITEM_PET_CHANGE_LEVEL
{
	UID64_t			ItemUniqueNumber;					// ��Ʈ�� ������ UID
	BYTE			Level;								// ��Ʈ�� ���� ����
};

struct MSG_FC_ITEM_PET_CHANGE_EXP
{
	UID64_t			ItemUniqueNumber;					// ��Ʈ�� ������ UID
	Experience_t	Experience;							// �� ����ġ
};

// ���� ������ ����

struct MSG_FC_ITEM_PET_SET_SOCKET
{
	UID64_t			ItemUniqueNumber;					// ��Ʈ�� ������ UID
	INT				SocketIndex;						// ���� ��ȣ
	UID64_t			PetSocketItemUID;					// ���� ������ ����ũ ��ȣ
};

struct MSG_FC_ITEM_PET_SET_SOCKET_OK
{
	UID64_t			ItemUniqueNumber;					// ��Ʈ�� ������ UID
	INT				SocketIndex;						// ���� ��ȣ
	UID64_t			PetSocketItemUID;					// ���� ������ ����ũ ��ȣ
};

// ŰƮ ���� ����

struct MSG_FC_ITEM_PET_SET_KIT_SLOT
{
	UID64_t				ItemUniqueNumber;				// ��Ʈ�� ������ UID
	INT					SocketIndex;					// ���� ��ȣ
	UID64_t				PetSocketItemUID;				// ���� ������ UID
	SPET_KIT_SLOT_DATA	PetKitHP;						// HP ŰƮ ���� ����
	SPET_KIT_SLOT_DATA	PetKitShield;					// Shield ŰƮ ���� ����
	SPET_KIT_SLOT_DATA	PetKitSP;						// SP ŰƮ ���� ����
};

struct MSG_FC_ITEM_PET_SET_KIT_SLOT_OK
{
	UID64_t				ItemUniqueNumber;				// ��Ʈ�� ������ UID
	INT					SocketIndex;					// ���� ��ȣ
	UID64_t				PetSocketItemUID;				// ���� ������ UID
	SPET_KIT_SLOT_DATA	PetKitHP;						// HP ŰƮ ���� ����
	SPET_KIT_SLOT_DATA	PetKitShield;					// Shield ŰƮ ���� ����
	SPET_KIT_SLOT_DATA	PetKitSP;						// SP ŰƮ ���� ����
};

// ���� ��ų ���� ����

struct MSG_FC_ITEM_PET_SET_AUTOSKILL_SLOT
{
	UID64_t						ItemUniqueNumber;		// ��Ʈ�� ������ UID
	INT							SocketIndex;			// ���� ��ȣ
	UID64_t						PetSocketItemUID;		// ���� ������ UID
	SPET_AUTOSKILL_SLOT_DATA	PetAutoSkill;			// ���� ��ų ����
};

struct MSG_FC_ITEM_PET_SET_AUTOSKILL_SLOT_OK
{
	UID64_t						ItemUniqueNumber;		// ��Ʈ�� ������ UID
	INT							SocketIndex;			// ���� ��ȣ
	UID64_t						PetSocketItemUID;		// ���� ������ UID
	SPET_AUTOSKILL_SLOT_DATA	PetAutoSkill;			// ���� ��ų ����
};

// end 2011-08-22 by hskim, ��Ʈ�� �ý��� 2�� - ��� ����
///////////////////////////////////////////////////////////////////////////////////////

//#define T_FC_INFINITY_SKIP_ENDING_CINEMA			(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_SKIP_ENDING_CINEMA)		// ���Ǵ�Ƽ ���� ��� ���� �ó׸� ���� ��ŵ ��û. 2010. 07. 27 by hsLee.
//#define T_FC_INFINITY_SKIP_ENDING_CINEMA_OK			(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_SKIP_ENDING_CINEMA_OK)	// ���Ǵ�Ƽ ���� ��� ���� �ó׸� ���� ��ŵ ���. 2010. 07. 27 by hsLee.

struct MSG_FC_INFINITY_SKIP_ENDING					// ���Ǵ�Ƽ �ó׸� ���� ��ŵ ��Ŷ.
{
	InfinityCreateUID_t	InfinityCreateUID;			// ���Ǵ�Ƽ ��Ƽ UID.
	bool				bNormalEnding;				// ���� ���� ���� & ��ŵ ��û ���� ����.
};

struct MSG_FC_INFINITY_SKIP_ENDING_CINEMA_OK		// ���Ǵ�Ƽ �ó׸� ���� ��ŵ ��� ��Ŷ.
{
	bool			EnableSkip;								// ���� ��ŵ ���� ���� ����.
	bool			NormalEnding;							// ���� ���� ���� & ��ŵ ��û ���� ����.

	char			szCharName[SIZE_MAX_CHARACTER_NAME];	// ��ŵ�� ��� System Messageó���� ����� ��û�� ĳ���� �̸�.

	ATUM_DATE_TIME	sUpdateTenderItemStartTime;			// ���� �������� ���� ���� �ð� ���Ű�. (���� ��ŵ�� ����� �ð�).
};

// 2010-08-31 by shcho&jskim �����ۿ��� �ý��� - ���� �ý��� ��Ŷ ó��
//#define T_FC_DISSOLVED_ITEM			(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_DISSOLUTIONITEM)		// client -> field
//#define T_FC_DISSOLVED_ITEM_OK		(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_DISSOLUTIONITEM_OK)	// field  -> client

struct MSG_FC_DISSOLVED_ITEM // ���� �� ������ ���� ����ü client -> field
{
	UID64_t UniqueNumber;
	INT	Itemnum;
};

struct MSG_FC_DISSOLVED_SUCCESS_AND_FAILED // ���ص� ������ ó�� ���
{
	bool Item_Success;		// ���� ���� �Ǵ� ����
	bool Dissolved_success;
};
// END 2010-08-31 by shcho&jskim �����ۿ��� �ý��� - ���� �ý��� ��Ŷ ó��

// 2010-06-25 by shcho, ���Ǵ�Ƽ ���÷α� ��� - ���� ������ ���� DB����
//#define T_FtoA_LOG_INFINITY_USER_GET_TENDERITEM		(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_LOG_INFINITYUSER_GET_TENDERITEM) // Field <- Arena

struct MSG_FtoA_INFINITY_TENDER_ITEM
{
	INT			TenderItemNum;		// ȹ���� ������ �ѹ�
	DiceCnt_t	GDicecount;			// ȹ���� �ֻ��� ī��Ʈ
	UID64_t		AccountUID;			// ���� UID
	CHAR		AccountName[SIZE_MAX_ACCOUNT_NAME];	// ���� �̸�
	UID64_t		CharacterUID;						// �ɸ��� UID
	CHAR		CharacterName[SIZE_MAX_CHARACTER_NAME];	// �ɸ��� �̸�
	INT			MFSClientIdx;

};
// END 2010-06-25 by shcho, ���Ǵ�Ƽ ���÷α� ��� - ���� ������ ���� DB����

///////////////////////////////////////////////////////////////////////////////
// 2011-01-26 by hskim, ���� ���� ����
//#define T_PAUTH_CONNECT_LOGIN						(MessageType_t)((T0_PAUTH_CONNECT<<8)|T1_PAUTH_CONNECT_LOGIN)
//#define T_PAUTH_CONNECT_LOGIN_OK					(MessageType_t)((T0_PAUTH_CONNECT<<8)|T1_PAUTH_CONNECT_LOGIN_OK)
//#define T_PAUTH_CONNECT_LOGIN_FAIL					(MessageType_t)((T0_PAUTH_CONNECT<<8)|T1_PAUTH_CONNECT_LOGIN_FAIL)
//#define T_PAUTH_CONNECT_LOGIN_SHUTDOWN				(MessageType_t)((T0_PAUTH_CONNECT<<8)|T1_PAUTH_CONNECT_LOGIN_SHUTDOWN)		// 2011-06-22 by hskim, �缳 ���� ����

struct SSERVER_AUTHENTICATION_ACCOUNT
{
	UINT64	UniqueNumber;
	char	szGameName[SIZE_MAX_AUTH_GAMENAME];
	char	szServerIP[SIZE_MAX_IPADDRESS];
	char	szServerNetmask[SIZE_MAX_IPADDRESS];
	char	szPublisher[SIZE_MAX_AUTH_PUBLISHER];
	int		iAllow;
	int		iShutdown;
	char	szResourceFileName[SIZE_MAX_AUTH_RESOURCE_FILE_NAME];
};

struct QPARAM_AUTHENTICATION_ACCOUNT
{
	bool	bResult;
	char	szGameName[SIZE_MAX_AUTH_GAMENAME];
	char	szServerPublicIP[SIZE_MAX_IPADDRESS];
	char	szServerPrivateIP[SIZE_MAX_IPADDRESS];
	char	szCurrentVer[SIZE_MAX_AUTH_CURRENTVER];
	int		nLanguageType;
	BYTE	byTestServer;
	USHORT	nServerPort;
	BYTE	byUseExternalAuthentication;
	int		nPreServerGroupCnts;
	int		nEnableGameServerGroupCnts;

	bool	bAccept;
	bool	bShutdown;
	char	szAcceptComment[SIZE_MAX_AUTH_ACCEPT_COMMENT];
	SSERVER_AUTHENTICATION_ACCOUNT AuthAccount;
};

///////////////////////////////////////////////////////////////////////////////
// 2011-06-22 by hskim, �缳 ���� ����

// start 2011-06-22 by hskim, �缳 ���� ����
//#define T_IP_AUTHENTICATION_SHUTDOWN			(MessageType_t)((T0_IP_CONNECT<<8)|T1_IP_AUTHENTICATION_SHUTDOWN)	// PreServer �� Authentication ������ �������� �ʾҴٸ� ���� ����
//#define T_FP_AUTHENTICATION_SHUTDOWN			(MessageType_t)((T0_FP_CONNECT<<8)|T1_FP_AUTHENTICATION_SHUTDOWN)	// PreServer �� Authentication ������ �������� �ʾҴٸ� ���� ����

struct MSG_IP_AUTHENTICATION_SHUTDOWN
{
	bool	bFlag;
};

struct MSG_FP_AUTHENTICATION_SHUTDOWN
{
	bool	bFlag;
};
// end 2011-06-22 by hskim, �缳 ���� ����

// 2012-07-11 by hskim, ������ �˴ٿ�
//#define T_IP_SELECTIVE_SHUTDOWN_NOTICE	(MessageType_t)((T0_IP_CONNECT<<8)|T1_IP_SELECTIVE_SHUTDOWN_NOTICE)

struct MSG_IP_SELECTIVE_SHUTDOWN_NOTICE
{
	UID32_t AccountUID;
	char m_szAccountName[SIZE_MAX_ACCOUNT_NAME];
	int nRemainingTime;
};
// end 2012-07-11 by hskim, ������ �˴ٿ�

// start 2011-11-03 by shcho, yedang �˴ٿ��� ���� - 
//#define T_FC_SHUTDOWNMINS_USER_ALTER			(MessageType_t)((T0_FC_CONNECT<<8)|T1_FC_CONNECT_SHUTDOWNMINS_USER_ALTER)	// nobody
//#define T_FC_SHUTDOWNMINS_USER_ENDGAME			(MessageType_t)((T0_FC_CONNECT<<8)|T1_FC_CONNECT_SHUTDOWNMINS_USER_ENDGAME)	// nobody

// end 2011-11-03 by shcho, yedang �˴ٿ��� ���� - 

// start 2011-10-18 by hskim, EP4 [Ʈ���� �ý���] - ȭ���� / �� ��ǳ
//#define T_FC_TRIGGER_MAP_BUFF_SKILL				(MessageType_t)((T0_FC_TRIGGER<<8)|T1_FC_TRIGGER_MAP_BUFF_SKILL)

struct MSG_FC_TRIGGER_MAP_BUFF_SKILL
{
	ClientIndex_t		ClientIndex;				// Ŭ���̾�Ʈ �ε���
	INT					SkillItemNum;				// ��ų�� ItemNum
	INT					AddTime;					// ��ų ���� �ð��� �߰��Ǵ� �� (ms)
};
// end 2011-10-18 by hskim, EP4 [Ʈ���� �ý���] - ȭ���� / �� ��ǳ

// start 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
struct MSG_FC_OBJECT_CHANGE
{
	ObjectIdx_t			ObjectIdx;
	ObjectNum_t			ChangeObjectIdx;
};
using mtvectObjectChange = mt_vector<MSG_FC_OBJECT_CHANGE>;

//#define T_FC_SET_LIMITTIME						(MessageType_t)((T0_FC_TRIGGER<<8)|T1_FC_SET_LIMITTIME)

struct MSG_FC_SET_LIMITTIME
{
	MSec_t			LimitTime;		// ms
};

using mtvectCharacterUID = mt_vector<UID32_t>;
// end 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���



// start 2012-10-08 by khkim, GLog
#define GLOG_AC_LT_LOGIN						0x01	// 1 - �α���	
#define GLOG_AC_LT_LOGOUT						0x02	// 2 - �α׾ƿ� 

#define GLOG_CD_LT_GAMESTART					0x01	// 1 - ���� ����
#define GLOG_CD_LT_GAMEEND						0x02	// 2 - ���� ����
#define GLOG_CD_LT_CHARACTERCREATE				0x03	// 3 - ĳ���� ����
#define GLOG_CD_LT_CHARACTERDELETE				0x04	// 4 - ĳ���� ����
#define GLOG_CD_LT_CHARACTLEVELUP				0x05	// 5 - ������

#define GLOG_SL_LT_SERVERON						0x01	// 1 - ���� ON
#define GLOG_SL_LT_SERVEROFF					0x02	// 2 - ���� OFF

#define	GLOG_IL_LT_DROP							0x01	// 1 - ���
#define GLOG_IL_LT_QUEST						0x02	// 2 - ����Ʈ
#define GLOG_IL_LT_SHOP_BUY						0x03	// 3 - ����			(����)
#define GLOG_IL_LT_SHOP_SELL					0x04	// 4 - ����			(�Ǹ�)
#define GLOG_IL_LT_WARPOINTSHOP_BUY				0x05	// 5 - ��������		(����)
#define GLOG_IL_LT_INFINITYOBSHOP_BUY			0x06	// 6 - �������		(����)
#define GLOG_IL_LT_CASHSHOP_BUY					0x07	// 7 - ĳ������		(����)
#define GLOG_IL_LT_LUCKYMACHINE_GET				0x08	// 8 - ��Ű�ӽ�		(����)
#define GLOG_IL_LT_LUCKYMACHINE_USE				0x09	// 9 - ��Ű�ӽ�		(���)

#define GLOG_IL_LT_BAZAARBUY_GET				0x0A	// 10 - ���λ���BUY	(����)
#define GLOG_IL_LT_BAZAARBUY_GIVE				0x0B	// 11 - ���λ���BUY	(�ֱ�)
#define GLOG_IL_LT_BAZAARSELL_GET				0x0C	// 12 - ���λ���SELL(����)
#define GLOG_IL_LT_BAZAARSELL_GIVE				0x0D	// 13 - ���λ���SELL(�ֱ�)
#define GLOG_IL_LT_TREADGIVE					0x0E	// 14 - �ŷ�		(�ֱ�)
#define GLOG_IL_LT_TREADGET						0x0F	// 15 - �ŷ�		(����)
#define GLOG_IL_LT_THROWAWAY					0x10	// 16 - ������ 
#define GLOG_IL_LT_MIXING_GET					0x11	// 17 - ����		(����)
#define GLOG_IL_LT_MIXING_USE					0x12	// 18 - ����		(���)
#define GLOG_IL_LT_ENCHANT_UP					0x13	// 19 - ��þƮ		(����)
#define GLOG_IL_LT_ENCHANT_DWON					0x14	// 20 - ��þƮ		(����)
#define GLOG_IL_LT_ENCHANT_DESTROY				0x15	// 21 - ��þƮ		(�ı�)
#define GLOG_IL_LT_ENCHANT_BEGINNING			0x16    // 22 - ��þƮ		(�ʱ�ȭ) 
#define GLOG_IL_LT_ENCHANT_USEITEM				0x17	// 23 - ��þƮ�� ����� ������
#define GLOG_IL_LT_RANDOMBOX					0x18	// 24 - ĸ���� ������
#define GLOG_IL_LT_DISSOLUTION_GETITEM			0x19	// 25 - ����
#define GLOG_IL_LT_SKILL						0x1A	
#define GLOG_IL_LT_LOADING						0x1B
#define GLOG_IL_LT_AUCTION						0x1C
#define GLOG_IL_LT_USE_GETITEM					0x1D
#define GLOG_IL_LT_USE_USEITEM					0x1E
#define GLOG_IL_LT_MGAME_EVENT					0x1F
#define GLOG_IL_LT_USE_ENERGY					0x20
#define GLOG_IL_LT_EXPIRE_CARD_ITEM				0x21
#define GLOG_IL_LT_PENALTY_ON_DEAD				0x22
#define GLOG_IL_LT_PENALTY_AGEAR_FUEL_ALLIN		0x23
#define GLOG_IL_LT_INFLUENCEWAR_KILLER_BONUS	0x24
#define GLOG_IL_LT_BONUS_ITEM					0x25
#define GLOG_IL_LT_ADMIN						0x26
#define GLOG_IL_LT_GIVEEVENTITEM				0x27
#define GLOG_IL_LT_GUILD_STORE					0x28
#define GLOG_IL_LT_EXPIRE_ITEM					0x29
#define GLOG_IL_LT_STORE						0x2A
#define GLOG_IL_LT_STORE_FEE					0x2B
#define GLOG_IL_LT_ARENA_ITEM					0x2C
#define GLOG_IL_LT_TUTORIAL_PAY_ITEM			0x2D
#define GLOG_IL_LT_EXPEDIENCYFUND_PAYBACK		0x2E
#define GLOG_IL_LT_GIVEEVENTITEM_COUPONEVENT	0x2F
#define GLOG_IL_LT_LUCKY_ITEM					0x30
#define GLOG_IL_LT_WAR_CONTRIBUTION				0x31
#define GLOG_IL_LT_WAR_CONTRIBUTION_LEADER		0x32
#define GLOG_IL_LT_WAR_CONTRIBUTION_GUILD		0x33
#define GLOG_IL_LT_BULLET						0x34
#define GLOG_IL_LT_DISSOLUTION_USEITEM			0x35
#define GLOG_IL_LT_DEPENDENCY_ITEM				0x36
#define GLOG_IL_LT_WARP							0x37
#define GLOG_IL_LT_CITYWAR_BRING_SUMOFTEX		0x38

#define GLOG_ISL_GAMESTART						0x01	// 1 - ���� ����
#define GLOG_ISL_CHARACTLEVELUP					0x02	// 2 - ������
// end 2012-10-08 by khkim, GLog

struct MSG_FC_CHARACTER_XIGNCODE_C_ACK_CODE
{
	ULONG Code;
};
// END 2012-10-17 by bhsohn XignCode�۾�

////////////////////////////////////////////////////////////////////////////
// 2012-11-13 by jhseol, ���� �ý��� ������ - ������
struct MSG_PA_ADMIN_STRATRGYPOINT_INFO_CHANGE
{
	char DBName[20];
};
using MSG_FP_ADMIN_STRATRGYPOINT_INFO_CHANGE = MSG_PA_ADMIN_STRATRGYPOINT_INFO_CHANGE;
// end 2012-11-13 by jhseol, ���� �ý��� ������ - ������

#define TIMESECOND_USER_NOTICE_MSG_SEND_TIC		3	// 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý��� - ���� �˸� MSG ������ �ֱ� �ð� 3��

// 2013-04-18 by jhseol,bckim �̴��� �Ƹ� - �̴��� �Ƹ� ���� ���� ���� ��Ŷ
struct MSG_FC_STORE_UPDATE_ARMOR_ITEM_EVENT_INFO
{
	UID64_t			ItemUID;			// ������ UID
	ItemNum_t		nOptionItemNum;		// �ɼ� ������
	ATUM_DATE_TIME	atEndDate;			// �̺�Ʈ ���� �Ⱓ
};

//#define T_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INIT	(MessageType_t)((T0_FC_COLLECTION<<8)|T1_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INIT)	// F->C, // �÷��ǿ� ǥ���� �̴��ǾƸ� �̺�Ʈ ���� �ʱ�ȭ ��Ŷ
//#define T_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO	(MessageType_t)((T0_FC_COLLECTION<<8)|T1_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO)	// F->C, // �÷��ǿ� ǥ���� �̴��ǾƸ� �̺�Ʈ ���� ��Ŷ

struct MSG_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO
{
	ItemNum_t		CollectionShapeNum;			// ���� ��ȣ
	ItemNum_t		nOptionItemNum;				// �ɼ� ������
	ATUM_DATE_TIME	atEndDate;					// �̺�Ʈ ���� �Ⱓ
};
// end 2013-04-18 by jhseol,bckim �̴��� �Ƹ� - �̴��� �Ƹ� ���� ���� ���� ��Ŷ

// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - ��Ŷ �߰� : �޼���
// ��Ŷ ����κ�
//#define T_FC_COLLECTION_INFO					(MessageType_t)((T0_FC_COLLECTION<<8)|T1_FC_COLLECTION_INFO)			// F->C, // �÷��� ���� ��Ŷ
//#define T_FC_COLLECTION_SHAPE_CHANGE			(MessageType_t)((T0_FC_COLLECTION<<8)|T1_FC_COLLECTION_SHAPE_CHANGE)	// C->F, // ���� ���� ��û ��Ŷ

// MSG ����κ�
using MSG_FC_COLLECTION_INFO = COLLECTION_INFO;				// �÷��� ���� ��Ŷ
using MSG_FC_COLLECTION_SHAPE_CHANGE = COLLECTION_SHAPE_CHANGE;		// ���� ���� ��û ��Ŷ

#define GAMELOG_COLLECTION_STATE_ADD			1	// �÷��� ���
#define GAMELOG_COLLECTION_STATE_ENCHANT		2	// �÷��� ��þƮ
#define GAMELOG_COLLECTION_STATE_TIME_START		3	// �÷��� ����(���):Ÿ�̸� ON
#define GAMELOG_COLLECTION_STATE_TIME_END		4	// �÷��� ����(�������):Ÿ�̸� OFF
struct MSG_FL_LOG_COLLECTION_STATE
{
	BYTE				CollectionState;
	COLLECTION_INFO		CollectionInfo;

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%d|%d|%d(%d)|%d|%d(%s)\r\n", GetGameLogTypeString(i_nLogType), CollectionInfo.CollectionType, CollectionState, CollectionInfo.AccountUID, CollectionInfo.CharacterUID
			, CollectionInfo.ShapeNum, CollectionInfo.ShapeItemNum, CollectionInfo.EnchantLevel, CollectionInfo.RemainSeconds, CollectionInfo.EndTime.GetDateTimeString().GetBuffer());
		return o_szLogString;
	}
};

struct MSG_FL_LOG_COLLECTION_SHAPE_CHANGE
{
	BYTE				CollectionType;
	UID32_t				AccountUID;
	UID32_t				CharacterUID;
	UID64_t				ItemUID;
	ItemNum_t			BeforeShapeNum;
	ItemNum_t			AfterShapeNum;

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%d|%I64d|%d->%d\r\n", GetGameLogTypeString(i_nLogType), CollectionType, AccountUID, CharacterUID, ItemUID, BeforeShapeNum, AfterShapeNum);
		return o_szLogString;
	}
};
// end 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - ��Ŷ �߰� : �޼���

// 2013-07-26 by jhseol, Ÿ ���� ���º��� - GameLog �߰�
struct MSG_FL_LOG_ACCOUNT_INFL_CHANGE_BY_ADMIN
{
	UID32_t				AccountUID;
	UID32_t				CharacterUID;
	BYTE				BeforeInfl;
	BYTE				AfterInfl;
	UID32_t				AdminCharacterUID;

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%d->%d|%d\r\n", GetGameLogTypeString(i_nLogType), AccountUID, CharacterUID, BeforeInfl, AfterInfl, AdminCharacterUID);
		return o_szLogString;
	}
};
// end 2013-07-26 by jhseol, Ÿ ���� ���º��� - GameLog �߰�

#include "AtumProtocolX.h"