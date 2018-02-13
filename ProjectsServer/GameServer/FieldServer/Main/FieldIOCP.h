// FieldIOCP.h: interface for the CFieldIOCP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIELDIOCP_H__F0E40C2E_D76B_4426_955E_1B43B544085D__INCLUDED_)
#define AFX_FIELDIOCP_H__F0E40C2E_D76B_4426_955E_1B43B544085D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IOCP.h"
#include "AtumFieldDBManager.h"
#include "FieldIOCPSocket.h"
#include "FieldMapWorkspace.h"
#include "PreWinSocket.h"
#include "IMWinSocket.h"
#include "ArenaFieldWinSocket.h"	// 2007-12-26 by dhjin, �Ʒ��� ���� -
#include "AtumParam.h"
#include "NPCScripts.h"
#include "Quest.h"
#include "MapOwnership.h"
#include "GuildWar.h"
#include "DropItemManager.h"
#include "ServicePauseManager.h"
#include "GameEventManager.h"
#include "CityWarManager.h"
#include "InflWarManager.h"
#include "CallWarpEventManager.h"		// 2006-07-21 by cmkwon
#include "GiveItemManager.h"			// 2006-08-25 by dhjin
#include "ArenaManager.h"				// 2007-04-17 by dhjin
#include "ChatBlockManager.h"			// 2007-07-11 by cmkwon, �Ʒ����� �ý��� ���� - ������� �߰�
#include "OutPostManager.h"				// 2007-08-16 by dhjin, ��������
#include "CityLeader.h"					// 2007-08-21 by dhjin, CityLeader
//#include "MersenneTwister.h"					// 2007-12-10 by cmkwon, ���� �ý��� ����
#include "WarContributionCalculation.h"		// 2008-12-22 by dhjin, ���� ���� �߰���
#include "WRankingManager.h"				// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 
#include "QuestMatching.h"					// 2009-03-31 by cmkwon, �����ʱ�ȭ �ý��� ���� - 
#include "InflRateManager.h"				// 2009-09-16 by cmkwon, ���� �ʱ�ȭ�� ���¡ ���� ���� - 
#include "InfinityManager.h"			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ -
#include "InvokingItemInfo.h"	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
#include "PetDataManager.h"		// 2010-06-15 by shcho&hslee ��ý���
#include "DissolutionItemmanager.h" // 2010-08-31 by shcho&jskim �����ۿ��� �ý��� -
#include "StatisticsManager.h"			// 2012-01-16 by hskim, ��� - ȭ��
#include "MonthlyArmorManager.h"		// 2013-04-18 by jhseol,bckim �̴��� �Ƹ� - �ش� �߰�
#include "DGAEventManager.h"

#ifdef SC_LEADER_WARP_RESTRICTION
#include "WarpRestrictionManager.h"		// 2016-01-04 Future, Leader Warp Restrictions
#endif // SC_LEADER_WARP_RESTRICTION

#if S_BONUS_KILL_SYSTEM_RENEWAL
#include "BonusKillSystem.h"
#endif

#ifdef NEMERIAN_PVP_AWARD_EFFORT
#include "LastHitManager.h"
#endif // NEMERIAN_PVP_AWARD_EFFORT

class CLogWinSocket;
class CFieldParty;
class CFieldTickManager;
class CRacingDemon;
class CCityWar;
class CCouponManager;
class CHappyHourEventManager;
class CGiveItemManager;
class CFieldODBC;
class CAllGuildItemManager;
class CBossWarContributionCalculation;		// 2008-12-22 by dhjin, ���� ���� �߰���
class CInnovaLibrary;						// 2010-04-26 by cmkwon, ���þ� Innva ����/���� �ý��� ���� - 
//class CSecurityManager;						// 2011-06-22 by hskim, �缳 ���� ����
class CMapTriggerManager;					// 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
class CStatisticsManager;					// 2012-01-16 by hskim, ��� - ȭ��

typedef mt_map<UID32_t, CFieldIOCPSocket*>					mtmapUID2FieldIOCPSocket;
typedef mt_map<string, CFieldIOCPSocket*, greater<string> >	mtmapName2FieldIOCPSocket, mtmapAccountName2FieldIOCPSocket;	// 2012-10-05 by jhseol, NPlay pc�� ��� - AccountName���� ���� ���� �� �߰�
// 2007-05-04 by cmkwon, mt_stl.h�� �̵���
//typedef mt_set<string>										mtsetString;
typedef mt_map<PartyID_t, CFieldParty*>						mtmapPartyID2FieldParty;
typedef mt_map<string, INT>									mtmapUsedFameData;			// 2005-12-29 by cmkwon
typedef vector<ITEM*>										vectITEMPtr;				// 2006-08-10 by cmkwon
typedef mt_map<UID32_t, CGuildItemManager*>					mtmapAllGuildItem;
typedef vector<MONSTER2ITEM*>								vectMONSTER2ITEMPtr;		// 2006-11-21 by cmkwon
typedef mt_vector<CBossWarContributionCalculation*>			mtvectCBossWarContributionCalculation;		// 2008-12-22 by dhjin, ���� ���� �߰���
typedef mt_vector<CWarContributionCalculation*>				mtvectCOutPostWarContributionCalculation;		// 2008-12-22 by dhjin, ���� ���� �߰���
typedef mt_vector<CWarContributionCalculation*>				mtvectCStrategyPointWarContributionCalculation;		// 2008-12-22 by dhjin, ���� ���� �߰���

struct SARENA_USE_FIELD_SERVER_INFO
{// 2007-12-27 by dhjin, �Ʒ��� ���� - �Ʒ��� �������� ����ϴ� �ʵ� ���� ����
	//USHORT		MFS_ID;
	USHORT			MFS_Port;
	CHAR			MFS_IP[SIZE_MAX_IPADDRESS];
	CHAR			MFS_Name[SIZE_MAX_SERVER_NAME];
	CIOCPSocket	*	Socket;	
};
typedef mt_vector<SARENA_USE_FIELD_SERVER_INFO>			mtvectSARENA_USE_FIELD_SERVER_INFO;

#define IS_MAP_WARP_POSSIBLE_INDEX		20000	// 2012-11-29 by jhseol, OX�̺�Ʈ �� �������� - ������ ������ ������ ������ �迭 �˳��ϰ� 2����
#define SM_TICK_EVENT_SUB_ITEM_EVENT	0		// 2013-03-29 by jhseol, ������ �̺�Ʈ - ����Ÿ�� �߰������� ����

class CFieldIOCP : public CIOCP
{
public:
	CFieldIOCP(int nPort, char *szLocalIP = "127.0.0.1");
	virtual ~CFieldIOCP();

	inline CFieldMapWorkspace* GetMapWorkspace();

	inline CFieldIOCPSocket* GetFieldIOCPSocket(int idx);
	CFieldIOCPSocket* GetFieldIOCPSocketByCharacterUniqueNumber(UID32_t uniqueNumber, BOOL i_bPermissionArenaState = FALSE);	// 2013-04-22 by jhseol, ������ �̺�Ʈ - ����Ÿ�� �߰����� �ý��� ����
	CFieldIOCPSocket* GetFieldIOCPSocketByCharacterName(char* i_szCharacterName);
	CFieldIOCPSocket* GetFieldIOCPSocketByAccountName(char* i_szAccountName);			// 2012-10-05 by jhseol, NPlay pc�� ���, IP�� ���� ���� ã��
	CHARACTER* GetCharacterByArrayIndex(int idx);
	BOOL GetUnitPlanePositonByClientIndex(CFieldMapChannel *pCurMapChannel, int i_nIdx, float *o_pX, float *o_pZ);

	void ApplyBuffSkillToGuild(INT itemNum, INT guildUID);

	void SendInRangeCharacterMoveOK(CFieldIOCPSocket *pFIOCPSock, BYTE *pData, int Size, CFieldMapChannel *pFieldMapChannel, DWORD i_dwSendCounts);
	void SendInArenaCharacterMoveOK(CFieldIOCPSocket *pFIOCPSock, BYTE *pData, int Size, CFieldMapChannel *pFieldMapChannel, DWORD i_dwSendCounts);		// 2007-04-26 by dhjin, �Ʒ��� �ʿ����� �̵��� ���� ��, ������ ��� �������� ������
	void SendInRangeCharacterOtherInfo(CFieldIOCPSocket *i_pFISock, CFieldMapChannel *pFieldMapChannel);
//	void SendInRangeBattleAttackResult(ClientIndex_t idx, BYTE* pData, int Size, CFieldMapChannel *pFieldMapChannel);
//	void SendInRangeBattleAttackFindResult(ClientIndex_t idx, BYTE* pData, int Size, CFieldMapChannel *pFieldMapChannel);
	void SendInRangeChangeBodyCondition(ClientIndex_t idx, CFieldMapChannel *pFieldMapChannel, int i_nRangeSize);

	void SendInRangeMonsterMoveOK(CFieldMonster *i_pFMonster, BYTE *pData, int Size, CFieldMapChannel *pFieldMapChannel, USHORT i_usSendRange);

	// general-purpose function
	void SendInRangeMessageAroundPosition(D3DXVECTOR3 pos, BYTE *pData, int Size, CFieldMapChannel *pFieldMapChannel, ClientIndex_t clientIndexToExclude=0, BOOL i_bCharacterFlag=TRUE);
	void SendInRangeMessageExactAroundPosition(D3DXVECTOR3 pos, BYTE *pData, int Size, int i_fRadius, CFieldMapChannel *pFieldMapChannel, ClientIndex_t i_ExcludeCliIdx=0, BYTE i_byInfluenceMask=INFLUENCE_TYPE_ALL_MASK);

	// 2009-10-12 by cmkwon, ������ī ���� ��� ���� - CFieldIOCP::SendInRangeMessageAroundCharacter#, �����߰�(i_nStartCityMapIdx)
	void SendInRangeMessageAroundCharacter(ClientIndex_t idx, BYTE *pData, int Size, CFieldMapChannel *pFieldMapChannel, BOOL bIncludeMe, int i_nRangeSize, BYTE i_byInfluenceMask=INFLUENCE_TYPE_ALL_MASK, MapIndex_t i_nStartCityMapIdx=VCN_CITY_MAP_INDEX);

	// 
	// 2009-10-12 by cmkwon, ������ī ���� ��� ���� - SendInRangeClientIndexVector, �����߰�(i_nStartCityMapIdx)
	int SendInRangeClientIndexVector(vectorClientIndex *i_pVectorCIdx, BYTE *i_pData, int i_nDataLen, ClientIndex_t i_excludeIdx=0, BYTE i_byInfluenceMask=INFLUENCE_TYPE_ALL_MASK, MapIndex_t i_nStartCityMapIdx=VCN_CITY_MAP_INDEX);
	int SendInRangeClientIndexVectorAroundPosition(D3DXVECTOR3 pos, vectorClientIndex *i_pVectorCIdx, BYTE *i_pData, int i_nDataLen, CFieldMapChannel *pFieldMapChannel, int i_nRangeSize, BOOL *o_pbExistUnitInMapChannel=NULL);
	int SendString128ToClientIndexVector(vectorClientIndex *i_pVectorCIdx, BYTE string128_type, char *szString);

	// ��� Ŭ���̾�Ʈ���� Message ������
	
	// 2008-09-09 by cmkwon, /���¼�ȯ ��ɾ� ���� ����Ʈ�� ���Ÿ�� �߰� - 
	//void SendMessageToAllClients(BYTE *pData, int Size, BYTE i_byInfluenceMask=INFLUENCE_TYPE_ALL_MASK, BYTE i_byMinLevel=0, BYTE i_byMaxLevel=0, MAP_CHANNEL_INDEX *i_pExcludeMapChannel=NULL, BOOL i_bOnlyPCBangClient=FALSE, BOOL i_bArenaState=FALSE);
	void SendMessageToAllClients(BYTE *pData, int Size, BYTE i_byInfluenceMask=INFLUENCE_TYPE_ALL_MASK, BYTE i_byMinLevel=0, BYTE i_byMaxLevel=0, MAP_CHANNEL_INDEX *i_pExcludeMapChannel=NULL, BOOL i_bOnlyPCBangClient=FALSE, BOOL i_bArenaState=FALSE, USHORT i_usReqUnitKind=UNITKIND_ALL_MASK);

	void SendString128ToAllClients(BYTE string128_type, char *szString, BOOL i_bWriteLog=FALSE, BYTE i_byInfluenceMask= INFLUENCE_TYPE_ALL_MASK);
	//Send Notification to all players
	void SendWorldNotification(char* szString, BOOL bSpecialChat/*Write it into Leadechat or not?*/);
	int SendMessageToGuildMembers(BYTE *pData, int Size, UID32_t i_guildUID, UID32_t i_excludeCharUID);
	int SendGuildSummonMessage(BYTE *pData, int Size, UID32_t i_guildUID, UID32_t i_excludeCharUID, CFieldMapProject *i_pFMPro);
	void SendMessageToCityClients(BYTE *pData, int Size, BYTE i_byInflTy);

	// 2008-01-31 by cmkwon, ���� ��/���� ��ɾ�� ������ �ý��� ���� - 
	BOOL Send2PreServerW(BYTE *i_pData, int i_nDataLen);

	// 2005-12-03 by cmkwon, IMServer�� ������
	BOOL Send2IMServer(void *i_pData, int i_nDataLen);
	
	BOOL Send2NPCServerByTCP(BYTE *i_pbyData, int i_nDataLen);	// 2007-08-22 by cmkwon, �Լ��� ����(<-Send2NPCServer)

	// 2008-02-25 by cmkwon, Pre,IM Server �� Alive Packet �����ϴ� �Լ� �߰� - 
	void Send2PreAndIMOfAlivePacket(void);

	BOOL OpenUDPPortForOtherServer(void);
	BOOL CloseUDPPortForOtherServer(void);

	// �ð� ��ȯ
	void GetCurrentServerTimeInAtumDateTime(ATUM_DATE_TIME &o_AtumDateTime);

	// Item
	void LoadItemInfo(BOOL bReload = FALSE);
	void ReloadIteminfo();		// ������ ������ ������Ʈ��. �������� �߰��ǰų� ���ŵ� ���� ���� �� ��
	ITEM* GetItemInfo(int ItemNum);
	ITEM* GetBulletItemInfoByCaliber(BYTE i_byCaliber);
#ifdef NEMERIAN_NATION_BALANCE_BUFF_PENALITY
	void ResetAllBuffSkill(BYTE InflType);
#endif
	int InsertItemAllCharacter(INT i_nItemNum, INT i_nItemCount, BYTE i_byInflMask=INFLUENCE_TYPE_ALL_MASK, BOOL bExceptArenaMapUser = FALSE, BOOL bPCBang = FALSE);			// 2012-10-04 by hskim, �ѱ� ��ü ���� (����� IDC) - PC�� ���� ������ ���� �߰� 	// 2007-05-16 by dhjin, �Ʒ��� üũ ���� �߰�.
	int InsertTimeLimiteItemAllCharacterSameMapChannel(ITEM *i_pTimeLimiteItemInfo, MAP_CHANNEL_INDEX i_mapChanIdx, BYTE* i_pMsgData, int i_nMsgSize, BYTE i_byInflMask=INFLUENCE_TYPE_ALL_MASK);			// 2009-01-05 by dhjin, �̼Ǹ����� - ��� ���� ������ �߰�
	int InsertTimeLimiteItemPartyMember(ITEM *i_pTimeLimiteItemInfo, CFieldIOCPSocket *i_pUseItemFISoc, BYTE* i_pMsgData, int i_nMsgSize);			// 2009-01-05 by dhjin, �̼Ǹ����� - ��� ���� ������ �߰�
	ITEM * CheckAllItemInfo(void);
	BOOL CheckItemInfo(ITEM *i_pItemInfo);
	ITEM * GetRandomItemFromRandomBoxItemList(void);
	ITEM * GetRandomSkillSupportItem(void);

	// 2013-05-09 by hskim, ���� ����Ʈ ����	
	BOOL DeleteBuffSkillAllCharacter(BYTE i_byInfluenceMask, DestParam_t desParam);
	BOOL InsertBuffSkillAllCharacter(BYTE i_byInfluenceMask, INT nItemNum);
	// end 2013-05-09 by hskim, ���� ����Ʈ ����

	// 2008-08-26 by cmkwon, ItemAttribute �߰� - ��Ű��(Package) ������, �Լ� ����, ���ϰ� ����
	//MYSTERY_ITEM_DROP * GetMysteryDropItem(CFieldIOCPSocket *i_pFISoc, INT i_nMysteryItemDropNum);
	BOOL GetMysteryDropItem(vectMYSTERY_ITEM_DROPPtr *o_pMysteryDropItemList, CFieldIOCPSocket *i_pFISoc, INT i_nMysteryItemDropNum, BOOL i_bPackageItem=FALSE);
	BOOL GetLuckyDropItem(CFieldIOCPSocket *i_pFISoc, INT i_nMysteryItemDropNum, LUCKYITEM_DROP_INFO * o_pLuckyItemInfo, MSG_FL_LOG_LUCKY_DROP * o_pLuckyDropLog);	// 2009-03-31 by dhjin, ��Ű�ӽ� �α� - �α׸� �����. // 2008-11-04 by dhjin, ��Ű�ӽ�
	void CheckLuckyItemPeriod(ATUM_DATE_TIME * i_pCurrentTime);		// 2008-11-04 by dhjin, ��Ű�ӽ�, �Ⱓ�� �������� üũ�Ͽ� ���ο� �Ⱓ�� �����Ѵ�.

	Err_t CheckMysteryItemDropRequirements(CFieldIOCPSocket *i_pFISoc, MYSTERY_ITEM_DROP *i_pMysteryItemDrop);


	// Rare Item - prefix or suffix
	RARE_ITEM_INFO* GetRareItemInfo(INT nCodeNum);
	void GetReqLevelApplyRare(int *o_pMinLevel, int *o_pMaxLevel, ITEM_GENERAL *i_pGItem);


	// Quest
	BOOL LoadQuestInfo(BOOL i_bReload = FALSE);
	void CleanQuestInfo();
	CQuest *GetQuestInfo(int i_nQuestIndex);
	BOOL CheckAndInitQuestInfo(CQuest *i_pQuestInfo);
	CQuest* GetFirstScenarioQuestInfoByInfluenceType(BYTE i_byInflTy);
	void SendAllQuestList(CFieldIOCPSocket *i_pTOFISock, BYTE i_byInflTy, char *i_szTxt=NULL);	// 2009-10-15 by cmkwon, ����Ʈ����Ʈ ������ÿ� ����Ʈ������ �˻� ��� �߰� - 


	// Monster
	MONSTER_INFO *GetMonsterInfo(int i_nMonsterUniqueNumber);
	MONSTER_INFO *GetMonsterInfo(char *i_szMonsterName);
	void LoadMonster2Item();
	void CleanMonster2Item();
	MONSTER2ITEM *GetMonster2Item(int MonsterUniqueNumber);

	// 2010-04-09 by cmkwon, ����2�� �߰� ����(�ܰ躰 ���� �߰�) - 
	void CreateDropItems(listMonsterDropItem *o_plistMonsterDropItem, CFieldMonster *i_pDeadFMon, CFieldIOCPSocket *i_pFISockTopDamage, float i_fPartyBonus, DropType_t i_dropType = DROP_TYPE_DEAD_MONSTER);
	void TestDropItems(MONSTER_INFO *pMonsterInfo, mapMonsterDropItemTest *o_pmapMonsterDropItemTest);		// 2012-02-13 by hskim, ���� ������ ��� �׽�Ʈ ��ɾ�

	void GetDropItemList(vectMONSTER2ITEMPtr *o_pVectDropItemList, int i_nMonsterUnitKind);

	CMapProject *GetMapProjectByMapIndex(MapIndex_t MapIndex);
	CFieldMapProject *GetFieldMapProjectByMapIndex(MapIndex_t MapIndex);
	CFieldMapChannel *GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX i_mapChannIdx);

	// 2009-10-12 by cmkwon, ������ī ���� ��� ���� - �����߰�(MapIndex_t i_nStartCityMapIdx)
	CFieldMapProject *GetCityFieldMapProjectPtrByInfluenceW(BYTE i_byInfluence, MapIndex_t i_nStartCityMapIdx=VCN_CITY_MAP_INDEX);

	// Log ���� �Լ�
	void SendLogMessageServerInfo(void);

	// map ����
/*
	inline char* GetDefaultMapName();
	inline CFieldMapProject* GetDefaultFieldMapProject();
*/
	// Item_Mixing_Info����
	// 2008-03-17 by cmkwon, ���� �ý��� ��� �߰� - �����߰�(INT i_nMixCnts)
	ITEM_MIXING_INFO* CheckAndGetItemMixingInfo(ITEM_W_COUNT_CHECK *i_arrItemWithCount, INT i_nItemWithCount, INT i_nMixCnts);
	ITEM_MIXING_INFO* CheckAndGetItemFixMixingInfo(ITEM_W_COUNT_CHECK *i_arrItemWithCount, INT i_nItemWithCount, INT i_nMixCnts, ITEM_MIXING_INFO *i_pItemMixingInfo);		// 2012-03-16 by hskim, DB ���ս� ���� (������ �������� ����)

	// ���� �������� �����͸� ��� ������
	void CharacterDataSaveRoutine();

	// 2004-03-22 by cmkwon, ��Ŷ ���� �ý��� ����
	BOOL CheckPacketPriority(DWORD i_dwSendCounts, DWORD i_dwModulusNumber);

	// 1�п� �ѹ��� ó���� �� ����(CFieldTickManager���� ȣ����) 
	void OnDoMinutelyWorkIOCP(ATUM_DATE_TIME *pDateTime);
	void OnDoSecondlyWorkFIOCP(ATUM_DATE_TIME *pDateTime);

	// 1�ð��� �ѹ��� ó���� �� ����(CFieldTickManager���� ȣ����) 
	void OnDoHourlyWorkIOCP(ATUM_DATE_TIME *pDateTime);

	// ��Ƽ����
	void DeleteFieldParty(CFieldParty *i_pFParty);
	void AdjustDeletedFieldParties(void);

	// ���������� ����
	void OnCityWarStartFieldIOCP(CCityWar *i_pCityWar);
	void OnCityWarEndFieldIOCP(CCityWar *i_pCityWar, vectCITYWAR_QUEST *i_pCityWarQuestResultList);
	void OnCityWarInfoFieldIOCP(CCityWar *i_pCityWar, int i_nRemainMinute);
	void OnCityWarQuestMonsterCreated(MapIndex_t i_mapIdx);
	void OnCityWarQuestMonsterDead(CCityWar *i_pCityWar);
	void SetCityWarTeamTypeByMapIndex(CCityWar *i_pCityWar, CFieldMapProject *i_pFMPro);

	// �� ����Ʈ ����
	BOOL GetP2PPKFamePoint(const char *szAttackCharName, const char *szDeadCharName);
	INT CountP2PPKFamePoint(const char *szAttackCharName, const char *szDeadCharName);
	void ResetAllP2PPKFamePoint(void);

	// 2006-02-08 by cmkwon, ������ ���� ����
	void AddInfluenceTypeCount(BYTE i_byInflType);
	int GetInfluenceTypeDistirbutionPercent(BYTE i_byInflType);
	float GetInfluenceTypeDistirbutionTexPercent(BYTE i_byInflType);
	int ChangeMapInfluenceType2AllConflictAreaW(BYTE i_byDeadMonsterBelligerence, BOOL i_bAdd);	// 2009-01-12 by dhjin, ���� ���� - ���� �߰� ��Ʈ

	// 2006-02-09 by cmkwon, ų��ũ ����
	ITEM * GetKillMarkItemPtr(BYTE i_byCharInflTy, int i_byCharLevel);

	// 2006-03-02 by cmkwon
	void InsertCharacterDropItem(BYTE i_byInflMask, USHORT i_UnitKindMask, CHARACTER2ITEM i_Character2Item);
	void InsertCharacter2Item(BYTE i_byInflty, USHORT i_UnitKindMask, CHARACTER2ITEM i_Character2Item);
	CHARACTER2ITEMLIST * FindCharacter2Item(BYTE i_byInflty, USHORT i_UnitKindMask);

	// 2006-04-17 by cmkwon, ������ �ý���
	BOOL SignBoardInsertString(MSG_SIGN_BOARD_STRING *i_pSignBoardString);
	BOOL SignBoardDeleteString(int i_nStringIdx, BOOL i_bInflLeader);
	void SendSignBoardStringList(CFieldIOCPSocket *i_pFISocket, int i_nReqStringIndex=-1);
	void SignBoardCheckExpireTime(ATUM_DATE_TIME *i_pCurATime);

	// 2006-04-20 by cmkwon, ����������
	void Send2LeaderANDSubLeader(BYTE i_byInflTy, BYTE *i_pData, int i_nDataLen, CFieldMapChannel *i_pExcludeFMChann);

// 2008-04-03 by cmkwon, �ٽ��� ���� ���� �ý��� ���� - ������� ����
//	// 2006-06-05 by cmkwon
//	BOOL SecurityInit(void);
//	void SecurityClean(void);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-07-21 by cmkwon
	BOOL AddCallWarpEventRequest(SCALL_WARP_EVENT *i_pCallWarpEvent);
	Err_t CheckCallWarpEventRequestAck(SCALL_WARP_EVENT *o_pCallWarpEvent, DWORD i_dwCallWarpID, BOOL i_bAddUserCount=FALSE);

	// 2006-08-22 by cmkwon
	// 2008-05-20 by cmkwon, ��� �̺�Ʈ(HappyHoure,MotherShip,Item) �׷� ���ÿ� �����ϰ� - SendGameEventMessage() ���� �߰�
	void SendGameEventMessage(CFieldIOCPSocket *pToFISock, BYTE i_byInflTy, MessageType_t i_msgTy=T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK);

	//////////////////////////////////////////////////////////////////////
	// 2012-10-05 by jhseol, NPlay PC�� ��� - ���Ǿƿ� ���� ��������
	void LoadGameEventMessage(CFieldIOCPSocket *pToFISock, MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK* msg);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-09-25 by cmkwon
	CAllGuildItemManager			*m_pAllGuildItemManager;		// 2006-09-20 by dhjin
	mtmapAllGuildItem * GetmtmapAllGuildItemPtrW(void);
	CGuildItemManager *GetGuildItemMangerNoLockW(UID32_t i_GuildUID);
	BOOL InsertGuildItemManagerNoLockW(CGuildItemManager *i_pGuildManager);

	// 2006-11-21 by cmkwon, ��������Ʈ ���� ����
	void InitStrategyPointOccupyW(BOOL i_bVCNMapInflTyForInit);

	// 2009-01-22 by cmkwon, ���� ���� ���� �α� ���� - CFieldIOCP::SendWarPointToInflClient()�� ����(, BYTE i_byWPUpdateTy=WPUT_MOTHERSHIPWAR) �߰�
	void SendWarPointToInflClient(BYTE i_byInflTy, int i_nWarPoint, BYTE i_byWPUpdateTy=WPUT_MOTHERSHIPWAR);		// 2007-05-15 by dhjin, WarPoint�� ������ �¸� ���¿��� �����Ѵ�.
	void SendWarPointToInflClientByDGA(int i_nWarPoint, BYTE i_bInfl = INFLUENCE_TYPE_ALL_MASK);

	// 2007-05-28 by cmkwon
	// 2009-10-06 by cmkwon, ��Ʈ�� ���� ���� X-TRAP���� ���� - CFieldIOCP::GetAllFileNameList# ����, ���� �߰� (i_szPrefixFileName)
	static BOOL GetAllFileNameList(vectString *i_pVectFileNameList, const char *i_szDir, char *i_szPrefixFileName=NULL);

	// 2008-09-08 by cmkwon, SCMonitor���� ReloadVersionInfo�ÿ� �Ϻ� üũ������(.\Res-Tex\*.*)�� ���ε��ϱ� - 
	//BOOL LoadResObjCheckList(void);
	BOOL LoadResObjCheckList(BOOL i_bReloadOnlyRexTexDirectory=FALSE);

	SRESOBJ_CHECKSUM * GetResObjCheckSum(char *i_szFileName);

	// 2007-07-23 by cmkwon, �߰���
	STUTORIAL_INFO *GetTutorialInfo(INT i_nTutorialUID);

	///////////////////////////////////////////////////////////////////////////////	
	// 2007-08-24 by cmkwon, ����Ŀ������ ��� ����/���� ���� ��� �߰� - �Լ� �߰�
	void SetUnusableSpeakerItem(BOOL i_bFlag);
	BOOL GetUnusableSpeakerItem(void);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-08-27 by cmkwon, �����ٿ��غ� ��ɾ� �߰�(SCAdminTool���� SCMonitor�� PrepareShutdown�� ���� �� �� �ְ�)
	void SetPrepareShutdown(BOOL i_bPrepareShutdown);

	//////////////////////////////////////////////////////////////////////////
	// 2007-08-24 by dhjin, OutPost
	void OnOutPostProtectorDestroyW(int i_nMapIndex);		// 2007-08-24 by dhjin, �������� ��ȣ�� �ı��� ó�� 
	void OnOutPostResetDestroyW(int i_nMapIndex);			// 2007-08-24 by dhjin, �������� �߾�ó����ġ �ı��� ó��

	//////////////////////////////////////////////////////////////////////////
	// 2007-10-31 by dhjin, ActionByLevel
	INT GetPollPointActionByLevel(BYTE i_nLevel);			// 2007-10-31 by dhjin, ������ ���� pollpoint ������

	void PayBossMonsterSuccessDefend(BYTE i_byInfluence);			// 2007-11-07 by dhjin, ���� ��� ���� �� ���� ���� �������� ���� �ִ� ��������Ʈ �� ��ŭ ���� �ϰ� ����

	///////////////////////////////////////////////////////////////////////////////
	// 2007-12-07 by cmkwon, ���� �ý��� ���� -
	// 2008-10-21 by cmkwon, �������� ���õ� ����� �ٽ� ������ �ʰ� ���� - ���� �߰�(, RARE_ITEM_INFO *i_nLastRareItemInfo/*=NULL*/)
	int GetRandomRareCode(ITEM *i_pTargetItemInfo, BOOL i_bIsPrefix, int i_nReqUseTypeMask, CFieldIOCPSocket *i_pFISoc=NULL, RARE_ITEM_INFO *i_pLastRareItemInfo=NULL);
	BOOL CheckFixRareCode(ITEM *i_pTargetItemInfo, BOOL i_bIsPrefix, int i_nFixedCodeNum);		// 2011-10-20 by hskim, EP4 [Free to play] - ����/���� ���� �ɼ�

	// 2007-12-10 by cmkwon, ���� �ý��� ���� - CFieldIOCP::GetRandInt32() �߰�
	UINT GetRandInt32(UINT i_uiMin, UINT i_uiMax);

	// 2007-12-27 by dhjin, �Ʒ��� ���� -
	BOOL InsertFieldServerInfoForArenaServer(SARENA_USE_FIELD_SERVER_INFO * i_pFieldServerInfo);	// 2007-12-27 by dhjin, �Ʒ��� ���� - �ʵ� ���� ������ �߰�
	CIOCPSocket* GetMFSSockForArenaServer() const { return m_SARENA_USE_FIELD_SERVER_INFO.Socket; }	// 2007-12-27 by dhjin, �Ʒ��� ���� - �ʵ� ���� ���� ��������
	void SendArenaServerPay(BYTE i_byInfluence, INT i_nPayInfluencePoint);					// 2008-01-10 by dhjin, �Ʒ��� ���� - �ʵ� ������ ���� ���� 
	void ConvertArenaRenderUserName(CHAR * i_pArenaCharacterName, CHAR * o_pSendArenaCharacterName);					// 2008-02-21 by dhjin, �Ʒ��� ���� - �Ʒ��� �������� ���� �̸� '\xxx' ��� �����ֱ�

	///////////////////////////////////////////////////////////////////////////////
	// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - 
	int								m_nLimitUserCount;						// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - 
	BOOL							m_bLockCreateCharacterForNewAccount;	// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - 

	void SendAllLetterByInfluence(BYTE i_byInfluence, SLETTER_INFO * i_pAllLetterInfo);	// 2008-05-09 by dhjin, EP3 ���� �ý��� - ������ �ִ� ���� ���� �������� ��ü ���� �߰�

	///////////////////////////////////////////////////////////////////////////////
	// 2008-10-28 by cmkwon, ��ȸ���� �ý��� ����(MotherShip ��ȯ�� ��� ���� ���ø����� �̵�) - 
	int WarpAllInfluenceUsersToCity(BYTE i_byInflTy, BOOL i_bIncludeDiedCharacter=FALSE, BOOL bExclusionMapByMSWar = FALSE);		// 2012-12-05 by hskim, ���� ���۽ÿ� ��� ���� ���ø����� ����
	BOOL IsWarpExclusionMapByMSWar(BYTE byInflTy, int nMapInfluenceType);		// 2012-12-05 by hskim, ���� ���۽ÿ� ��� ���� ���ø����� ����

	///////////////////////////////////////////////////////////////////////////////
	// 2009-03-31 by cmkwon, �����ʱ�ȭ �ý��� ���� - 
// 2009-09-16 by cmkwon, ���� �ʱ�ȭ�� ���¡ ���� ���� - 
//	BOOL IsEnableSelectInfluence(BYTE i_byInflTy);
	BOOL IsSelectableInfluence(BYTE i_byInflTy, BYTE i_byLv);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-08-03 by cmkwon, EP3-4 ��� ���� ��ų ���� - 
	mapINT2ItemSkillPtr				m_PartyFormationItemSkillList;
	int ArrangePartyFormationSkill(mapINT2ItemSkillPtr *o_pPartyItemSkillList, ez_map<INT, ITEM*> *i_pItemInfoList);
	ITEM_SKILL * SearchPartyFormationSkill(INT i_nItemNum);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-09-16 by cmkwon, ���� �ʱ�ȭ�� ���¡ ���� ���� - 
	CInflRateManager				m_InflRateMan;

	///////////////////////////////////////////////////////////////////////////////
	// 2010-01-18 by cmkwon, ������ ���� Parameter �ߺ� üũ �ý��� ���� - 
	ezmapINT2vectINT				m_mapItemParamOverlapList;
	int ResetItemParamOverlapList(vectSITEM_PARAM_OVERLAP *i_pParamOverlapList);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-01-26 by cmkwon, ĳ�� ������ �����Ǹ� �ý��� ���� - 
	mtvectITEM_W_COUNT				m_LimitedEItemList;
	void LimitedEReset(void);
	ITEM_W_COUNT *LimitedEFindItemInfoNoLock(INT i_nItemNum);
	BOOL LimitedEGetItemWCount(vectITEM_W_COUNT *o_pItemCntList);
	void LimitedEUpdateItemCount(INT i_nItemNum, INT i_nRemainCnt);
	void LimitedEUpdateShopItemW(UINT i_nBuildIdx, CFieldMapProject *i_pFMapPro);
	BOOL IsCashShopItemNoLock(INT nItemNum);		// 2013-03-13 by hskim, �� ĳ�� ����

	void GetUserCountByInfluence(int &aniCount, int &bcuCount);
	// 2013-03-29 by jhseol, ������ �̺�Ʈ
	BOOL ItemEventSubTypeCheck(UID32_t i_nAUID, UID32_t i_nCUID, SITEM_EVENT_INFO i_SITEM_EVENT_INFO, DestParam_t i_SkipSubEventType = ITEM_EVENT_DES_EMPTY);
	void InsertTickEvent(INT			i_TickEventType,
		INT				i_nTimeIntervalInSeconds,
		INT				i_nTickEventParam1 = 0,
		INT				i_nTickEventParam2 = 0,
		void			*i_vpTickEventParam1 = NULL,
		void			*i_vpTickEventParam2 = NULL);
	// end 2013-03-29 by jhseol, ������ �̺�Ʈ

	//////////////////////////////////////////////////////////////////////////
	// 2015-11-25 Future, Multiple IP Restriction
#ifdef S_IP_UNIQUE_CONNECTION
	BOOL MultipleIPsRestricted();
	BOOL EnableMultipleIPRestriction(CFieldIOCPSocket* pCaller, BOOL enable = TRUE);
#endif // S_IP_UNIQUE_CONNECTION

protected:
	CFieldMapWorkspace				*m_pFieldMapWorkspace;
// 2007-02-12 by cmkwon, �ʿ����	vector<CFieldIOCPSocket*>		m_vecMapIOCPSocket;

	//////////////////////////////////////////////////////////////////////////
	// 2015-11-25 Future, Multiple IP Restriction
#ifdef S_IP_UNIQUE_CONNECTION
	BOOL							m_bMultipleIPRestriction;
#endif // S_IP_UNIQUE_CONNECTION

public:
	SERVER_ID						m_FieldServerID;
	CAtumFieldDBManager				*m_pAtumDBManager;
	CFieldTickManager				*m_pFieldTickManager;			// Tick manager
	//CSecurityManager				*m_pSecurityManager;			// 2011-06-22 by hskim, �缳 ���� ����
	CStatisticsManager				*m_pStatisticsManager;			// 2012-01-16 by hskim, ��� - ȭ��
	CPreWinSocket					*m_pPreWinSocket;
	CIMWinSocket					*m_pIMWinSocket;
	CArenaFieldWinSocket			*m_pArenaFieldWinSocket;		// 2007-12-26 by dhjin, �Ʒ��� ���� - 
	ez_map<INT, ITEM*>				m_mapItemInfo;
	multimap<BYTE, ITEM*>			m_multiMapCaliber2Bullet;		// ����(Caliber) -> �Ѿ� ������(ITEMKIND_BULLET)
	ez_map<int, MONSTER_INFO>		m_mapMonsterInfo;
	multimap<int, MONSTER2ITEM*>	m_mapMonster2Item;
	ez_map<INT, CQuest*>			m_mapQuestInfoFromScript;
	ez_map<INT, ENCHANT_INFO>		m_mapEnchantInfo;
	vector<ITEM_MIXING_INFO>		m_vectorItemMixingInfo;
	ez_map<INT, RARE_ITEM_INFO>		m_mapRareItemInfo;
	ez_map<BYTE, vector<RARE_ITEM_INFO* > > m_mapLevel2RareItemInfo;

	mtmapUID2FieldIOCPSocket		m_mapCharacterUniqueNumber;
	mtmapName2FieldIOCPSocket		m_mapCharacterName;
	mtmapAccountName2FieldIOCPSocket	m_mapAccountName;			// 2012-10-05 by jhseol, NPlay pc�� ��� - AccountName���� ���� ���� ��
	mtsetString						m_mtsetAccountName;

	// Party
	mtmapPartyID2FieldParty			m_MapParties;
	mt_vector<CFieldParty*>			m_mtvectorDeletedFieldParties;

	// map ����
	MapIndex_t						m_DefaultMapIndex;		// ���� ��ü�� �⺻ ��, ĳ���� ���� �� �⺻ ��ġ
	CFieldMapProject*				m_pDefaultMapProject;	// ���� ��ü�� �⺻ ��, ĳ���� ���� �� �⺻ ��ġ

	// NPC Scripts ����
	ez_map<NPCID_t, NPCScript>		m_NpcScriptsMap;

	// ���η�
	float							SIZE_MAX_MINE_REACTION_RANGE;		// ���η��� MAX ���� ����

	UINT							m_uiAccumulatedUserCounts;
	UINT							m_uiMaxUserCounts;
	UINT							m_uiAccumulatedMonsterCounts;
	
	CFieldIOCPSocket				*m_pNPCTCPSocket;				// NPC���� TCP Connection	

// 2008-08-27 by cmkwon, �������� Procedure�� ���� - ������� �ʴ� �κ���
//	CMapOwnershipManager			m_MapOwnershipManager;			// ����(��) ������ ����, ����/���� ��	

	CGuildWarMananger				m_GuildWarManager;				// ����� ������	
	CDropItemManager				m_DropItemManager;				// ��� ������ ������	
	CServicePauseManager			m_ServicePauseManager;			// ���� ���� �Ͻ� ���� ������
	CRacingDemon					*m_pRacingDemon;				// ���̽� ������ ó���� ����	
	CGameEventManager				m_gameEventManager;				// ���� �̺�Ʈ ����

	CCityWarManager					m_cityWarManager;				// ���������� ����
// 2008-01-10 by cmkwon, ������ �̺�Ʈ �ý��ۿ� �� ���� �ý��� �߰� - ������
//	CCouponManager					*m_pCouponManager;				// ���� ������
	CHappyHourEventManager			*m_pHappyHourEventManager;		// Happy Hour Event ������

	CDGAEventManager				*m_pDGAEventManager;

	CGiveItemManager				*m_pGiveItemManager;			// 2006-08-24 by dhjin,

	CFieldODBC						*m_pFieldODBC;					// 

	mtmapUsedFameData				m_mtmapUsedFameDataList;			// 2005-12-29 by cmkwon, ���� �� ������ �Ϸ翡 �ѹ��� �ʱ�ȭ �ȴ�.

	// 2006-02-07 by cmkwon, ������ ���� ����
	int								m_nNormalInfluenceCount;		// 2006-02-07 by cmkwon, ���������� �Ϲݱ� ī��Ʈ - ���� �ʿ��� ��ġ�� �ƴϹǷ� ��Ȯ�� ���� �ƴϴ�.
	int								m_nVCNInfluenceCount;			// 2006-02-07 by cmkwon, ���������� ���Ա� ī��Ʈ
	int								m_nANIInfluenceCount;			// 2006-02-07 by cmkwon, �˸��� ���Ա� ī��Ʈ
	
	// 2006-02-09 by cmkwon, ų��ũ ����
	mtvectorItemPtr					m_mtvectItemPtrKillMarks;		// 2006-02-09 by cmkwon, ų��ũ ������ ����Ʈ

	vectCHARACTER2ITEMLIST			m_vectCharacter2ItemList;		// 2006-03-02 by cmkwon, 

	// 2006-04-12 by cmkwon, ������ ���� �ý���
	CInflWarManager					m_InflWarManager;

	// 2006-04-17 by cmkwon, ������ �ý���
	INT								m_nSignBoardStringIndex;		// 2006-04-17 by cmkwon
	mtvectMSG_SIGN_BOARD_STRINGList	m_mtvectSignBoardStringList;	// 2006-04-17 by cmkwon

	// 2006-06-29 by cmkwon, 
	BOOL							m_bInitHackShield;				// 2006-06-29 by cmkwon

	///////////////////////////////////////////////////////////////////////////////
	// 2006-07-21 by cmkwon
	CCallWarpEventManager			m_callWarpEventManager;			// 2006-07-21 by cmkwon

	BOOL							m_bPrepareShutDown;				// 2006-08-04 by cmkwon, ���� �ٿ� �������

	///////////////////////////////////////////////////////////////////////////////
	// 2006-08-10 by cmkwon, RandomBox
	vectITEMPtr						m_vectITEMPtr4RandomBox;		// 2006-08-10 by cmkwon, �����ڽ� �������� ���� ����
	vectITEMPtr						m_vectITEMPtrSkillSupportItems;	// 2006-09-29 by cmkwon, ��ų ���� ������ ����Ʈ

	///////////////////////////////////////////////////////////////////////////////
	// 2006-11-17 by cmkwon, �Ϸ� �ð� ���� �ý��� ����
	BOOL							m_bUseTimeLimiteSysteOfToday;	// 2006-11-17 by cmkwon

	///////////////////////////////////////////////////////////////////////////////
	// 2007-04-10 by cmkwon, ��ȸ���� DB �ʱ�ȭ ��ɾ� ��������
	int								m_nJamboreeInitCommandVerifyNumber;

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-17 by dhjin, �Ʒ��� ����
	CArenaManager					m_ArenaManager;


	///////////////////////////////////////////////////////////////////////////////
	// 2007-05-11 by cmkwon, �̽��׸�ĸ�� ������ �ý���
	//////////////////////////////////////////////////////////////////////////
	// 2008-11-10 by dhjin, ��Ű�ӽ� multimap -> mt_multimap���� ����
//	mmapINT2MYSTERY_ITEM_DROP	m_mmapMysteryItemDropList;
	mtmapINT2MYSTERY_ITEM_DROP	m_mtmapMysteryItemDropList;

	///////////////////////////////////////////////////////////////////////////////
	// 2007-05-28 by cmkwon
	mapstring2SRESOBJ_CHECKSUM		m_mapResObjCheckSumList;

	///////////////////////////////////////////////////////////////////////////////
	// 2007-07-11 by cmkwon, �Ʒ����� �ý��� ���� - m_arenaBlockManager �߰�
	CChatBlockManager				m_arenaBlockManager;

	//////////////////////////////////////////////////////////////////////////
	// 2007-07-11 by dhjin, Tutorial
	vectSTutorialInfo				m_vectTutorial;

	///////////////////////////////////////////////////////////////////////////////
	// 2007-08-24 by cmkwon, ����Ŀ������ ��� ����/���� ���� ��� �߰� - �����߰�
	BOOL							m_bUnusableSpeakerItem;

	//////////////////////////////////////////////////////////////////////////
	// 2007-08-16 by dhjin, ��������
	COutPostManager					m_OutPostManager;

	//////////////////////////////////////////////////////////////////////////
	// 2007-08-21 by dhjin, CityLeader
	CCityLeader						m_CityLeader;

	// 2007-10-29 by dhjin, ������ ���� ���� �ൿ�� ���� ��
	vectSACTION_BY_LEVEL_DB			m_vectSACTION_BY_LEVEL_DB;

	// 2007-12-10 by cmkwon, ���� �ý��� ����	
	//MTRand							m_mtRandManager;	// 2007-12-10 by cmkwon
	//mt_lock							m_lockRandManager;	// 2007-12-10 by cmkwon

	// 2007-12-27 by dhjin, �Ʒ��� ���� - �Ʒ��� �������� ����ϴ� �ʵ� ���� ����
	SARENA_USE_FIELD_SERVER_INFO	m_SARENA_USE_FIELD_SERVER_INFO;	

	//////////////////////////////////////////////////////////////////////////
	// 2008-08-25 by dhjin, �±� PC�� IP���� �ε�
	INT								m_nPCBangMinTick;
	INT								m_nMaxPCBangMinTick;

#ifdef NEMERIAN_PVP_AWARD_EFFORT
	LastHitManager *m_pLastHitManager;
#endif

	//////////////////////////////////////////////////////////////////////////
	// 2008-11-10 by dhjin, ��Ű�ӽ�
	vectSLUCKY_MACHINE				m_vectLuckyMachine;

	// 2010-04-20 by cmkwon, �ű� ��Ű �ӽ� ���� - ���� �ҽ� ���� - 
	int GetLuckyMachineItemList(vectSLUCKY_MACHINEPtr *o_pLuckyMachineItemList, INT i_buildingIdx, INT i_machineNum, INT i_coinItemNum);
	INT GetLuckyMachineKind(INT i_buildingIdx, INT i_machineNum, INT i_coinItemNum);

	//////////////////////////////////////////////////////////////////////////
	// 2008-12-08 by dhjin, �̼Ǹ�����
	void SearchMissionMaster(int i_nQuestIdx, CFieldIOCPSocket * i_pFISoc, vector<CFieldIOCPSocket*> * o_pVecMissionMasterIOCPSocket);
	void SendHelpMissionMaster(vector<CFieldIOCPSocket*> * i_pVecMissionMasterIOCPSocket, BYTE* i_pMsgData, int i_nMsgSize);

	//////////////////////////////////////////////////////////////////////////
	// 2008-12-22 by dhjin, ���� ���� �߰���
	mtvectCBossWarContributionCalculation			m_mtvectCBossWarContributionCalculation;
	mtvectCOutPostWarContributionCalculation		m_mtvectCOutPostWarContributionCalculation;		
	mtvectCStrategyPointWarContributionCalculation	m_mtvectCStrategyPointWarContributionCalculation;	
	void WarContributionCalculation(CFieldIOCPSocket * i_pFISoc, UID32_t i_nValue);		// 2008-12-23 by dhjin, ���� ���� �߰��� - �⿩�� ����Ͽ� �߰��Ѵ�.
	UID32_t WarContributionCalculationByGear(USHORT i_sUnitKind, UID32_t i_nValue);		// 2008-12-23 by dhjin, ���� ���� �߰��� - �� �� �⿩�� ���
	void WarDeathContributionCalculation(CFieldIOCPSocket * i_pFISoc);					// 2008-12-23 by dhjin, ���� ���� �߰��� - ���� ���� ī��Ʈ �߰�
	void WarOtherInflStayTime(CFieldIOCPSocket * i_pFISoc, BOOL i_bCheckStart);			// 2008-12-23 by dhjin, ���� ���� �߰��� - ��뼼�¿� �ӹ��ð�
	void WarStartOtherInflStayTimeForAllUser();											// 2009-01-06 by dhjin, ���� ���� �߰��� - ���� ���۽� ��뼼�¿� �̹� �ִ� ������ �ӹ��ð� ����
	void InsertCBossWarContributionCalculation(MapIndex_t i_nMapIndex, int i_nStartWarContributionPoint);
	void InsertCOutPostWarContributionCalculation(MapIndex_t i_nMapIndex);
	void InsertCStrategyPointWarContributionCalculation(MapIndex_t i_nMapIndex);
	void DeleteCBossWarContributionCalculation(MapIndex_t i_nMapIndex);
	void DeleteCOutPostWarContributionCalculation(MapIndex_t i_nMapIndex);
	void DeleteCStrategyPointWarContributionCalculation(MapIndex_t i_nMapIndex);
	void PayCBossWarContributionCalculation(MapIndex_t i_nMapIndex, BYTE i_byWinInfl);	// 2008-12-23 by dhjin, ���� ���� �߰��� - ���� �¸����¿��� ���� ����
	void PaySendBossWarContribution(CBossWarContributionCalculation * i_pBossWarContribution, BYTE i_byWinInfl);		// 2008-12-23 by dhjin, ���� ���� �߰��� - ���� ���� ����
	void PaySendBossWarContributionLeader(CBossWarContributionCalculation * i_pBossWarContribution, BYTE i_byWinInfl);		// 2008-12-23 by dhjin, ���� ���� �߰��� - ������ ����
	void PaySendBossWarContributionGuild(CBossWarContributionCalculation * i_pBossWarContribution, BYTE i_byWinInfl);		// 2008-12-23 by dhjin, ���� ���� �߰��� - ��� ����
	void PaySendBossWarContributionGuildProcess(UID32_t i_nGuildUID, UID32_t i_nPay, UID64_t i_nContribution, MapIndex_t i_nMapIndex, BYTE i_byPayGrade);		// 2008-12-23 by dhjin, ���� ���� �߰��� - ��� ���� ó��
	void PayCOutPostWarContributionCalculation(MapIndex_t i_nMapIndex, BYTE i_byWinInfl);	// 2008-12-23 by dhjin, ���� ���� �߰��� - �������� �¸����¿��� ���� ����
	void PayCStrategyPointWarContributionCalculation(MapIndex_t i_nMapIndex, BYTE i_byWinInfl);	// 2008-12-23 by dhjin, ���� ���� �߰��� - ��������Ʈ �¸����¿��� ���� ����
	void PaySendWarContribution(CWarContributionCalculation * i_pWarContribution, BYTE i_byWinInfl, int i_nDefinePayWarType, BYTE i_byWarType);		// 2008-12-23 by dhjin, ���� ���� �߰��� - ���� ����

	// 2010-06-01 by shcho, GLogDB ���� -
	void SendBossGLogEventParticipationRate(MapIndex_t i_nMapIndex, INT i_nCnt, ATUM_DATE_TIME * i_pWarStartTime);
	void SendStrategyPointGLogEventParticipationRate(MapIndex_t i_nMapIndex, INT i_nCnt, ATUM_DATE_TIME * i_pWarStartTime);
	void SendOutPostGLogEventParticipationRate(MapIndex_t i_nMapIndex, INT i_nCnt, ATUM_DATE_TIME * i_pWarStartTime);


	///////////////////////////////////////////////////////////////////////////////
	// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 
	CWRankingManager			m_WRankingManager;

	///////////////////////////////////////////////////////////////////////////////
	// 2009-03-31 by cmkwon, �����ʱ�ȭ �ý��� ���� - 
	CQuestMatching				m_QuestMatching;
	BOOL						m_bWarningStateForSelectInfluence;

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ -
	CInfinityManager			m_InfinityManager;
	void ResetInfinityByResetCycle(ATUM_DATE_TIME *i_pCurDateTime);
	void ResetInfinityByResetCycleByServerStart();

	// 2010-04-09 by cmkwon, ����2�� �߰� ����(�ܰ躰 ���� �߰�) - 
	//vectorTenderInfo			m_TenderInfoList;
	//BOOL CheckTenderItem(ItemNum_t i_CheckItemNum);

	vectorInfinityShopInfo		m_InfinityShopInfoList;		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ����
	CInvokingItemInfo			m_InvokingItemInfo;			// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-09 by cmkwon, ����2�� �߰� ����(�ܰ躰 ���� �߰�) - 
	BOOL SetTenderItemList(vectorTenderInfo *i_pTenderItemList);

	// 2010-08-05 by dhjin, ���׸� - 
	BurningMapInfoList			m_BurningMapList;
	BURNING_MAP_INFO * GetBurningMapInfo(MapIndex_t i_nMapIdx);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-06-15 by shcho&hslee ��ý���
	CPetDataManager		m_cPetDataManager;

	///////////////////////////////////////////////////////////////////////////////
	// 2010-08-31 by shcho&jskim �����ۿ��� �ý��� - ���� �ý��� Ŭ���� �߰�
	CDissolutionItemManager  m_cDissolutionItemManager;

	///////////////////////////////////////////////////////////////////////////////
	// 2011-11-03 by shcho, yedang �˴ٿ��� ���� - �˴ٿ� ��ų ���� ���� �����̳�
	vectShutDownUserData m_ShutDownData;
	void FindShutdownUserAndStartShutDownRoutin(ATUM_DATE_TIME *i_pCurDateTime); // �˴ٿ� ���� �Լ�

	// 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
	CMapTriggerManager		*m_pMapTriggerManager;

	// 2013-03-13 by hskim, �� ĳ�� ����
	BOOL					m_bServerActive;
	BOOL					IsServerOn()		{ return (TRUE == m_bServerActive && FALSE == m_bPrepareShutDown); }
	// end 2013-03-13 by hskim, �� ĳ�� ����

	// 2013-05-20 by hskim, [���� �ý���] ���������� ������� ������ �߰� ����
	UID64_t					m_MaximumItemUID;			// ���� ����� �ִ� ������ ��ȣ
	UID64_t					GetItemSecurityMaximunUID()		{ return m_MaximumItemUID; }
	void					SetItemSecurityMaximunUID(UID64_t MaximumItemUID)		{ m_MaximumItemUID = MaximumItemUID; }

	void					LoadItemSecurityMaximunUID();
	UID64_t					GeneratorItemSecurityCheckSum(UID64_t nUniqueNumber, INT ItemCount);
	// end 2013-05-20 by hskim, [���� �ý���] ���������� ������� ������ �߰� ����
	
	// 2015-11-24 Future, checks for IP availability
	bool					PeerIPAvailable(CFieldIOCPSocket* pCallingSocket);

	// 2016-01-04 Future, Leader Warp Restriction
#ifdef SC_LEADER_WARP_RESTRICTION
	CWarpRestrictionManager	m_WarpRestrictionManager;
#endif // SC_LEADER_WARP_RESTRICTION

public:
	int* GetUserLevelList(int i_UserLevelList[10]);
	int GetPCBangUserCount();										// 2007-01-25 by dhjin, ���� �÷��� �ϰ� �ִ� PC�� ������
	virtual BOOL IOCPInit(void);									// CAtumDBManager �� ���� �ʱ�ȭ �ϱ� ���� virtual�� ����
	virtual void IOCPClean(void);
	virtual SThreadInfo *CheckIOCPThread(DWORD i_dwThreadIdToExclude);
	virtual void CalcTotalTrafficInfo(void);
	BOOL LoadFieldServerDataDebug(void);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-08 by cmkwon, �������� �ʷε��� AlivePacket ���� �߰� - 
	virtual void SendAlivePacket2OtherServers(void);

	// 2012-11-29 by jhseol, OX�̺�Ʈ �� ��������
	BOOL m_bIsMapWarpPossible[IS_MAP_WARP_POSSIBLE_INDEX];		// DB�� ���� ���� �ʾ� ������ ���۵Ǹ� �ʱ�ȭ ��
	BYTE ReversalMapWarpPossible(int i_nMapindex);				// �ش���� ���� On/Off �� �������ִ� �Լ�
	BOOL IsMapWarpPossible(int i_nMapindex);					// �ش���� ���� ������ Ȯ���ϴ� �Լ�
	// end 2012-11-29 by jhseol, OX�̺�Ʈ �� ��������

	///////////////////////////////////////////////////////////////////////////////
	// 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý��� - ��������
	void SetNextMapWarpPossible(MapIndex_t i_nMapIndex, BOOL i_bWarpPossible);
	BOOL GetNextMapWarpPossible(MapIndex_t i_nMapIndex);
	BOOL m_bTriggerNextMapWarpPossible[IS_MAP_WARP_POSSIBLE_INDEX];		// �ش���� ���� ������ ������ �迭
	// end 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý��� - ��������

	// 2013-04-18 by jhseol,bckim �̴��� �Ƹ� - m_pMonthlyArmorManager ����
	CMonthlyArmorManager	*m_pMonthlyArmorManager;
	// end 2013-04-18 by jhseol,bckim �̴��� �Ƹ� - m_pMonthlyArmorManager ����

	// 2015-06-13 Future, Cashing of KillCount in case of User disconnect
#if S_BONUS_KILL_SYSTEM_RENEWAL
	mt_map<UID32_t, KILLCOUNT_CACHE_DATA>	m_mtmapCharacterKillCounts;
	void CacheCharacterKillCount(UID32_t characterUID, INT count);		// Caches Killcount of a Character
	INT GetCachedKillCount(UID32_t characterUID);						// Gets Cached Killcount for a Character
	void CleanKillCountCache();											// Cleans expired Cache Data
	void CleanKillCountCacheFromCharacter(UID32_t fromCharacterUID);	// 2015-12-26 Future, clean cached killcount from a specific character
#endif // S_BONUS_KILL_SYSTEM_RENEWAL
	// end 2015-06-13 Future, Cashing of KillCount in case of User disconnect
};

CFieldIOCPSocket* CFieldIOCP::GetFieldIOCPSocket(int idx)
{
	return (CFieldIOCPSocket*)GetIOCPSocket(idx);
}

CFieldMapWorkspace* CFieldIOCP::GetMapWorkspace()
{
	return m_pFieldMapWorkspace;
}

/*
char* CFieldIOCP::GetDefaultMapName()
{
	return m_DefaultMapName;
}

CFieldMapProject* CFieldIOCP::GetDefaultFieldMapProject()
{
	return m_pDefaultMapProject;
}
*/

#endif // !defined(AFX_FIELDIOCP_H__F0E40C2E_D76B_4426_955E_1B43B544085D__INCLUDED_)
