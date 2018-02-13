#ifndef _MGR_FIELD_ITEM_H_
#define _MGR_FIELD_ITEM_H_

#include "FieldDataManager.h"
#include "AtumProtocol.h"

struct SDelayStoreItemInfo
{
	SDelayStoreItemInfo(UID64_t i_n64ItemUID, int i_nItemCount)
		: n64ItemUID(i_n64ItemUID), nStoredItemCounts(i_nItemCount)
	{
	}
	UID64_t		n64ItemUID;
	int			nStoredItemCounts;
};

typedef mt_map<UID64_t, ITEM_GENERAL*>	mtmapUID2ItemGeneral;
typedef vector<ITEM_SKILL*>				vectItemSkillPtr;
typedef vector<SDelayStoreItemInfo>		vectDelayStoreItemInfo;
// 2006-09-25 by cmkwon, stdafx.h�� �ű�
//typedef vector<ITEM_GENERAL*>			vectITEM_GENERALPtr;

class CParamFactor;
class CFieldItemManager : public CFieldDataManager
{
public:
	CFieldItemManager();

	// ĳ������ ���� ���� �� ȣ��
	void ResetAllCharacterItems(void);
	void ResetAllItemGenerals(void);
	void ResetAllItemGeneralsInStore(BOOL i_bClearEnchat=TRUE);
	void ResetAllItemSkills(void);
	void ResetUsingGeneralItems(void);

	void ResetUsingSocketItems();		// 2011-09-29 by hskim, ��Ʈ�� �ý��� 2�� - ���Ǵ�Ƽ���� ���ƿ� �� ���

	void ResetPremiumCardInfo(void);

	// funtions
	ITEM_BASE* MakeNewItem(INT ItemNum,
							BYTE i_enumStorage = ITEM_IN_CHARACTER,
							ITEM *pItemInfo = NULL, UID64_t ItemUniqueNumber = 0);

	// 2010-06-15 by shcho&hslee ��ý��� - �� ������ ����.
	tPET_CURRENTINFO* MakeNewPetData ( UID64_t i_nPetUID , const tPET_BASEDATA *a_pPetBaseData , const tPET_LEVEL_DATA *a_pPetLevelData = NULL );


	ITEM_GENERAL* GetItemGeneralByUID(UID64_t i_nItemUID, BYTE i_enumStorage = ITEM_IN_CHARACTER);
	ITEM_GENERAL* GetFirstItemGeneralByItemNum(INT i_nItemNum, BYTE i_enumItemStorage = ITEM_IN_CHARACTER);
	ITEM_GENERAL* PopItemGeneralByUID(UID64_t i_nItemUID);

	BOOL InsertItemGeneralByPointer(ITEM_GENERAL *i_pItemGeneral
									, BYTE i_nItemUpdateType
									, BOOL i_bCheckGeneralItemCounts=FALSE
									, BYTE i_enumStorage=ITEM_IN_CHARACTER
									, BOOL i_bUpdateDB=TRUE
									, BOOL i_bSendMSG=TRUE
									, UID32_t i_tradeCharacterUID=INVALID_UNIQUE_NUMBER		// 2006-05-15 by cmkwon
									, BOOL i_bCheckGamePlayTime=TRUE						// 2008-03-28 by cmkwon, PlayTime ���� ���� ���� ���� - 
									, int i_nGLogItemtype=GLOG_DEFAULT_ZERO					//  2012-11-21 by khkim, GLog 2��
									);	// new�� ������ pItem�� ���ο��� ����(����, ���� ��)��, DB���� �����ؼ� Ŭ���̾�Ʈ���� �޽��� ���������

	BOOL InsertItemSkillByPointer(UID64_t i_SkillItemUID, ITEM_SKILL *i_pItemSkill);		// ��ų�ε���, ��ų���Խ� ����

	BOOL InsertStoreItemByPointer(UID64_t i_n64ItemUID, ITEM_GENERAL *i_pItemG);			// 

	///////////////////////////////////////////////////////////////////////////////
	// 2010-02-11 by cmkwon, ���ս� ����,����Ʈ ���� �ý��� ���� - �����߰�(, INT i_nShapeItemNum=0, INT i_nEffectItemNum=0)
	BOOL InsertItemBaseByItemNum(INT i_nItemNum, INT i_nItemCount
								, BYTE i_nItemUpdateType
								, BOOL i_bCheckGeneralItemCounts=FALSE
								, BYTE i_enumStorage=ITEM_IN_CHARACTER
								, BOOL i_bUpdateDB=TRUE
								, BOOL i_bSendMSG=TRUE
								, INT i_nPrefixCodeNum=0
								, INT i_nSuffixCodeNum=0
								, INT i_nShapeItemNum=0
								, INT i_nEffectItemNum=0
								, int i_nGLogItemtype=GLOG_DEFAULT_ZERO				//  2012-11-21 by khkim, GLog 2��
								);			// 2007-07-25 by cmkwon, �̺�Ʈ �������� ����/���� ���� ���� - CFieldItemManager::InsertItemBaseByItemNum()�Լ� ���� �߰�(, int i_nPrefixCodeNum/*=0*/, int i_nSuffixCodeNum/*=0*/)

	BOOL InsertItemGeneralByItemNum(INT i_nItemNum, INT i_nItemCount, 
								BYTE i_nItemUpdateType, BYTE i_enumStorage = ITEM_IN_CHARACTER,
								BOOL i_bUpdateDB = TRUE, BOOL i_bSendMSG = TRUE
								, UID32_t i_tradeCharacterUID=INVALID_UNIQUE_NUMBER		// 2006-05-15 by cmkwon
								);

	BOOL UpdateItemCountByPointer(ITEM_GENERAL *i_pItemGeneral,
								INT i_nChangeCount,
								BYTE i_nItemUpdateType, BYTE i_enumStorage = ITEM_IN_CHARACTER,
								BOOL i_bUpdateDB = TRUE, BOOL i_bSendMSG = TRUE, BOOL i_bDeleteMemory = TRUE,
								int i_nGLogItemtype = GLOG_DEFAULT_ZERO );			//  2012-11-21 by khkim, GLog 2��
	BOOL UpdateItemNumByPointer(ITEM_GENERAL *i_pItemGeneral, ITEM *i_pChangeITEM, BYTE i_nItemUpdateType, BOOL i_bSendMSG = TRUE);

	BOOL DeleteItemFromCharacterByPointer(ITEM_GENERAL *i_pItemGeneral,
								BYTE i_nItemUpdateType, BYTE i_enumStorage = ITEM_IN_CHARACTER,
								BOOL i_bUpdateDB = TRUE, BOOL i_bSendMSG = TRUE, BOOL i_bDeleteMemory = TRUE,
								int	 i_nGLogItemtype = GLOG_DEFAULT_ZERO);			// 2012-11-21 by khkim, GLog 2��


	BOOL DeleteItemFromCharacterByUID(UID64_t i_nItemUID,
								BYTE i_nItemUpdateType, BYTE i_enumStorage = ITEM_IN_CHARACTER,
								BOOL i_bUpdateDB = TRUE, BOOL i_bSendMSG = TRUE, BOOL i_bDeleteMemory = TRUE,
								int i_nGLogItemtype = GLOG_DEFAULT_ZERO);			//  2012-11-21 by khkim, GLog 2��

	BOOL DeleteItemFromCharacterByItemNum(INT i_nItemNum,
								INT i_nDelCount,
								BYTE i_nItemUpdateType, BYTE i_enumStorage = ITEM_IN_CHARACTER,
								BOOL i_bUpdateDB = TRUE, BOOL i_bSendMSG = TRUE, BOOL i_bDeleteMemory = TRUE,
								int i_nGLogItemtype = GLOG_DEFAULT_ZERO);				//  2012-11-21 by khkim, GLog 2��

	// 2010-06-15 by shcho&hslee ��ý��� - ���� �� ���� �޸� ����.
	BOOL DeleteItemPetFromCharacterByUID ( UID64_t i_ItemUID , BOOL i_bSendMSG = TRUE , BOOL i_bDeleteMemory = TRUE );

	BOOL DeleteAllItemGeneralFromCharacter(BOOL i_bOnlyNotAttachedItems = TRUE);
	BOOL DeleteAllItemSkillFromCharacter();

	

	void DeleteAllArenaItem();			// 2007-06-04 by dhjin. �Ʒ��� ���� �������� �����Ѵ�.

	BOOL LoadEnchantItem(UID64_t i_nItemUID, BOOL i_bLoadServer, BOOL i_bSendClient);

	// 2008-02-18 by cmkwon, ī���ͺ� �������� ������ ����� �ν��Ͻ� �������Ǵ� ���� ���� - CFieldItemManager::RearrangeCountableItems() �Լ� ����
	//void RearrangeCountableItems(BYTE i_enumStorage, int i_nItemNum=0);
	void RearrangeCountableItems(BYTE i_enumStorage, int i_nItemNum=0 , BYTE i_byItemUpdateType = IUT_GENERAL );

	// 2007-11-02 by cmkwon, �߷� ���ϴ� �ý��� ���� - ���ϰ� �ڷ��� ����(<--int)
	float GetCurrentCarryingItemWeight(void);

// 2005-12-07 by cmkwon
//	BOOL UpdateItemPossess(ITEM_GENERAL *i_pItemGeneral, INT i_nFromPossess, INT i_nToPossess, BOOL i_bUpdateDB = TRUE, BOOL i_bSendMSG = TRUE);
	BOOL UpdateItemStorage(ITEM_GENERAL *i_pItemGeneral, BYTE i_enFromStorage, BYTE i_enToStorage);
	BOOL UpdateItemStorageSubItem(ITEM_GENERAL *i_pItemGeneral, BYTE i_enFromStorage, BYTE i_enToStorage);		// 2012-01-30 by hskim, ��Ʈ�� �ý��� 2�� - ���� â�� �̵�

	// ITEM_SKILL ����
	void DeleteItemSkillBySkillBaseNum(INT i_nSkillItemNum, BOOL i_bSendMsg);
	ITEM_SKILL *GetFirstSkillBySkillBaseNum(INT i_nSkillItemNum);
	ITEM_SKILL *GetItemSkillByUID(UID64_t i_nSkillUID);

	// 2010-06-15 by shcho&hslee ��ý���

	// ITEM_PET ����.
	void DeleteItemPetByUID ( UID64_t i_nPetUID );
	tPET_CURRENTINFO *GetItemPetByUID ( UID64_t i_nPetUID );

	// 2015-06-23 Future, Pet upgrades with sockets
	bool UpgradePet(UID64_t oldPetUID, ITEM* targetPetItem);	
	void InsertUpgradedPet(UID64_t oldPetUID, ITEM_GENERAL* newItemData, tPET_CURRENTINFO* newPetInfo);

	// End 2010-06-15 by shcho&hslee ��ý���

	BOOL IsBelongPetSocketItemByUID(UID64_t i_nPetID, UID64_t i_nSocketID);		// 2011-09-20 by hskim, ��Ʈ�� �ý��� 2��

	void CheckRequirementAllGeneralItems(void);

	INT GetInventoryItemCounts(void);		// 2011-10-21 by hskim, ��Ʈ�� �ý��� 2��

	// �ð� ���� ������ ���ð� ����
	// 2010-03-17 by cmkwon, ����1�� �ߵ��� ������ ���� ���� ���� - ���� ������ �Ķ� ���� ���� ���� ���� 
	BOOL Add2UsingTimeLimitedItemList(ITEM_GENERAL *i_pItemGen, BOOL i_bResetAllSkillAndEnchant=FALSE);

	ITEM_GENERAL * Stop8StoreFromUsingTimeLimitedItemList(ITEM_GENERAL *i_pItemGen, BOOL i_bDeleteUsingTimeLimitedItem);
	void ResetTimeStampUsingTimeLimitedItemList(BOOL i_bAfterArena = FALSE);			// 2008-02-01 by dhjin, �Ʒ��� ���� - �Ʒ��� ������ ���Ӽ������� ���۽� üũ ��Ʈ �߰�
	void CheckExpireUsingTimeLimitedItemList(UID64_t i_itemUID=INVALID_UID64);
	void StoreTimeStampUsingTimeLimitedIteList(void);
	BOOL IsExistAlreadyUsingItemByDestParameter1(DestParam_t i_DestParameter);		// 2011-08-01 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ��� ���� (DestParameter - 255 -> 32767 ����)
	BOOL IsExistAlreadyUsingItemByItemNum(int i_nItemNum);
	BOOL DeleteUsingTimeLimitedItemList(int i_nItemNum);
	void ApplyParamFactorWithAllUsingTimeLimitedOnlyCardItem(void);
	ITEM_GENERAL *FindUsingItemByItemNumNoLock(int i_nItemNum);		// 2007-06-15 by cmkwon, ��� �ð� ���� ������ �ý��� ����
	ITEM_GENERAL *FindUsingItemByDestParameterNoLock(DestParam_t i_DestParameter);		// 2011-08-01 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ��� ���� (DestParameter - 255 -> 32767 ����) - // 2010-04-05 by cmkwon, ���ͺ��Ű��� ���� ī�� ���� - 2010-04-16 by cmkwon, �������� �߰� ����
	BOOL DeleteUsingTimeLimitedItemListByDestParameter(DestParam_t i_DestParameter);	// 2011-08-01 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ��� ���� (DestParameter - 255 -> 32767 ����) - // 2010-04-05 by cmkwon, ���ͺ��Ű��� ���� ī�� ���� - 2010-04-16 by cmkwon, �������� �߰� ����
	BOOL SetReturnItemDesParamInParty(CFieldIOCPSocket *i_pUseItemFISoc, ITEM* i_pItemInfo); // 2013-02-28 by bckim, �������� �����߰�

	///////////////////////////////////////////////////////////////////////////////
	// 2010-01-18 by cmkwon, ������ ���� Parameter �ߺ� üũ �ý��� ���� - 
	ITEM *CheckItemParamOverlapOfUsingItem(ITEM *i_pToUseItemInfo, INT i_nExcludeItemNum);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-01-31 by cmkwon, �ð����� ������ DB �� �߰� �� üũ - 
	//			��� �� DB �� �߰� ���� ������ Item üũ�� ����, 2�� ����� �ȵǵ��� �ϱ� ���� üũ
	void InsertInsertingItemNumList2DB(int i_nItemNum);
	BOOL IsExistInsertingItemNumList2DB(int i_nItemNum);
	void DeleteInsertingItemNumList2DB(int i_nItemNum);

	// ī���ͺ������ DB�� ���� ����
	SDelayStoreItemInfo *FindDelayStoredItemInfoByItemUID(UID64_t i_n64ItemUID);
	BOOL SetDelayStoredItemCounts(ITEM_GENERAL *i_pItemG);
	BOOL DeleteDelayStoredItemInfoByItemUID(UID64_t i_n64ItemUID);
	BOOL StoreCountableItem(void);
		
	static BOOL IsDelayStoreItem(ITEM *i_pItem);

	// ����ȭ ������ ����
	BOOL IsExistPremiumCard(void);
	void CashInitPremiumCardInfo(SCASH_PREMEIUM_CARD_INFO *i_pCardInfo);
	Err_t CashInsertPremiumCard(ITEM *i_pItem, BOOL i_bOnlyCheck=FALSE);
	BOOL CashOnEventPremiumCardFromDB(int i_queryType, UID32_t i_AccUID, UID64_t i_n64CardUID, Err_t i_errNum=0);
	BOOL CashCheckPremiumCardExpire(BOOL i_bUnconditionalDeleteFlag=FALSE);
	void CashInsertBonusItemPremiumCard(ITEM *i_pItemInfo);
	constexpr static bool IsUsePremiumCardAdvantage() { return true; }

	void Make_MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO(MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO *o_pCardInfo);
	void Make_MSG_FI_CASH_PREMIUM_CARD_INFO(MSG_FI_CASH_PREMIUM_CARD_INFO *o_pCardInfo);

	// 2008-03-03 by dhjin, �Ʒ��� ���� - 
	void AllDetachTimeLimitItem();			// 2008-03-03 by dhjin, �Ʒ��� ���� - �������� �ð����� �������� ���´�.

	// debug��
	void DBG_PRINT_ITEM();

	///////////////////////////////////////////////////////////////////////////////
	// 2009-03-31 by cmkwon, �����ʱ�ȭ �ý��� ���� - 
	int ChangeItemNum(INT i_nFromItemNum, INT i_nToItemNum);

	// 2013-04-12 by jhseol, �ʵ�â�� ������� ������þƮ ���� ����
	BOOL m_bUsingStore;		// Fieldâ���̵� �Ϲ� â���̵� ��¶�� â�� �̿������� üũ�� �÷���
	BOOL GetUsingStore(void);
	void SetUsingStore(BOOL i_bFlag);
	// end 2013-04-12 by jhseol, �ʵ�â�� ������� ������þƮ ���� ����
	
	///////////////////////////////////////////////////////////////////////////////
	// 2009-09-23 by cmkwon, �ʵ�â�� ĳ�� ������ ���� - 
	BOOL						m_bUsingFieldStore;					// �ʵ�â�� �̿� �÷���
	BOOL GetUsingFieldStore(void);
	void SetUsingFieldStore(BOOL i_bFlag);
	BOOL IsUseableFieldStore(void);
	ITEM_GENERAL * GetItemGeneralByDestParam(DestParam_t i_byDestParam, BYTE i_enumStorage = ITEM_IN_CHARACTER);		// 2011-08-01 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ��� ���� (DestParameter - 255 -> 32767 ����)

	///////////////////////////////////////////////////////////////////////////////
	// 2009-11-02 by cmkwon, ĳ��(�κ�/â�� Ȯ��) ������ �߰� ���� - 
	int GetMaxInventoryCount(BYTE i_enStorage=ITEM_IN_CHARACTER);

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - Main������ ������ ����ȭ�� ���� ó��
	void ItemInsertByInfinityPlaying(ITEM_GENERAL *i_pItemGeneral);			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� �� ������ ����	
	void ItemUpdateByInfinityPlaying(ITEM_GENERAL *i_pItemGeneral);			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� �� ������ ����
	void ItemDeleteByInfinityPlaying(ITEM_GENERAL *i_pItemGeneral);			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� �� ������ ����
	BOOL UpdateMainSvrUsingTimeLimitedItemByInfinity(MSG_FtoA_INFINITY_USING_TIME_LIMIT_ITEM *i_pItemInfo);			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���ǿ��� ������ �ð��� ������ ����!
	BOOL DeleteUsingTimeLimitedItemByInfinity(UID64_t i_MainSvrItemUID);
	void LogInfinityUsingItem();						// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�

	///////////////////////////////////////////////////////////////////////////////
	// 2010-01-26 by cmkwon, ĳ�� ������ �����Ǹ� �ý��� ���� - 
	BOOL CheckBuyLimitedEditionShopItem(INT *o_pRemainCount, INT i_nItemNum);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
	BOOL AddInvokingItemList(ItemUID_t i_ItemUID);
	BOOL CheckCoolingTimeInvokingItem(ItemUID_t i_ItemUID, BOOL i_bSendElapsedSec=FALSE);			// ��Ÿ���� ���� �ִ��� üũ
	void StopInvokingItemList(ITEM_GENERAL *i_pItemGen);			// ��Ÿ�� ����
	void SaveInvokingItemList(BOOL i_bSaveCoolingItem=TRUE);									// ��Ÿ�� ����
	ITEM_GENERAL *InvokingFindItemGNoLock(ItemUID_t i_ItemUID);
	BOOL InvokingDeleteItemGNoLock(ItemUID_t i_ItemUID);
	BOOL InvokingCheckExpire(void);									// ��Ÿ�� �Ϸ� üũ
	
	BOOL UpdateItemCountByPointer(DestParam_t i_byDestParam, BYTE i_enumStorage =ITEM_IN_CHARACTER); // 2011-08-01 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ��� ���� (DestParameter - 255 -> 32767 ����) - // 2010-03-23 by cmkwon, ���Ǵ�Ƽ ���� ĳ�� ������ ���� - 

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-02 by cmkwon, ����2�� �߰� ���� - 
	INFINITY_INSERT_ITEM_INFO *InfinityFindItemInfoFromInsertItemListNoLock(INT i_ItemNum);
	INFINITY_INSERT_ITEM_INFO *InfinityFindItemInfoFromInsertItemListNoLock(ItemUID_t i_ItemUID);
	BOOL InfinityDeleteItemInfoFromInsertItemList(ItemUID_t i_ItemUID);
	void InfinitySendInsertItemList(CFieldIOCPSocket *i_pFISoc4MFS);
	INFINITY_UPDATE_ITEM_INFO *InfinityFindItemInfoFromUpdateItemListNoLock(ItemUID_t i_ItemUID);
	BOOL InfinityDeleteItemInfoFromUpdateItemList(ItemUID_t i_ItemUID);
	void InfinitySendUpdateItemList(CFieldIOCPSocket *i_pFISoc4MFS);
	INFINITY_DELETE_ITEM_INFO *InfinityFindItemInfoFromDeleteItemListNoLock(ItemUID_t i_ItemUID4MFS);
	BOOL InfinityDeleteItemInfoFromDeleteItemList(ItemUID_t i_ItemUID4MFS);
	BOOL InfinityInsertItemInfoFromDeleteItemList(ItemUID_t i_ItemUID4MFS);
	void InfinitySendDeleteItemList(CFieldIOCPSocket *i_pFISoc4MFS);
	// 2010-06-15 by shcho&hslee ��ý��� - ���� �� ���� ����
	BOOL InsertItemPetByPointer( tPET_CURRENTINFO *i_pItemPet );
	BOOL InfinitySendPetItemInfo(CFieldIOCPSocket *i_pFISoc4MFS);		// 2011-09-30 by hskim, ��Ʈ�� �ý��� 2�� - ���� �����  Main������ ��Ʈ�� ����Ÿ ����

	// start 2011-10-21 by hskim, ��Ʈ�� �ý��� 2�� - ���� ������ �ִ� ������ ������ (POS_HIDDEN_ITEM) ����
	INT GetInventoryHiddenItemCounts(void);
	INT CalcInventoryHiddenItemCounts(void);
	// end 2011-10-21 by hskim, ��Ʈ�� �ý��� 2�� - ���� ������ �ִ� ������ ������ (POS_HIDDEN_ITEM) ����

	// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)
	BOOL RegisterTimerFixedTermShape(ITEM_GENERAL *i_pItemGeneral);
	BOOL RemoveTimerFixedTermShape(ITEM_GENERAL *i_pItemGeneral);
	BOOL DeleteFixedTermShapeByTimerUID(UID32_t TimerUID);
	// end 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)

	// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
	ITEM_GENERAL* FindGeneralItemByItemNum(INT i_nItemNum);
	// end 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������

	// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - ReturnShapeChangeKitItem �Լ� ����
	void ReturnShapeChangeKitItem(ITEM_GENERAL *i_pItemG);
	// end 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - ReturnShapeChangeKitItem �Լ� ����

	// 2013-05-30 by jhseol, GLog �ý��� ����
//	void CFieldItemManager::SetGLogData(BYTE i_nLogType, ITEM_GENERAL *i_pItemGeneral, BYTE i_nCharLv, int i_nChangeCount = 0, int i_nCharSerial = 0);	// 2012-11-21 by khkim, GLog 2��
	void SetGLogData(BYTE i_nLogType, ITEM_GENERAL *i_pItemGeneral, BYTE i_nCharLv = 0, int i_nChangeCount = 0, int i_nCharSerial = 0);
	// end 2013-05-30 by jhseol, GLog �ý��� ����
	
	// Item Data
public:
	mtmapUID2ItemGeneral		m_mapItemGeneral;				// ItemUID->ITEM_GENERAL*, check: reset�� �� item���� delete�ؾ� ��
	vectItemSkillPtr			m_vectItemSkillPtr;				// ItemUID->ITEM_SKILL*, check: reset�� �� item���� delete�ؾ� ��
	mtmapUID2ItemGeneral		m_mapItemInStore;				// â�� ����ִ� ������, â�� ���� ����, ���� �� ����, check: ����ϴ��� Ȯ���ϱ�
	vectDelayStoreItemInfo		m_vectDelayStoredItemInfo;		// ī���ͺ� ������ DB�� ����Ǿ��ִ� ����

	mtvectPetCurrentInfo		m_vectPetCurrentInfo;			// 2010-06-15 by shcho&hslee ��ý��� 

	SCASH_PREMEIUM_CARD_INFO	m_cashPremiumCardInfo;

	UID32_t						m_uid32StoreCharacterUID;		// 2006-09-18 by dhjin, ���� ���õ� â�� ĳ����

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - Main������ ������ ����ȭ�� ���� ó��
	mtvectorInfinityInsertItemInfo		m_mtInfinityInsertItemInfoList;		// �߿�! ���ǿ��� ������ �����ۿ� ���õǼ��� ó���Ǿ�ߵȴ�.
	mtvectorInfinityUpdateItemInfo		m_mtInfinityUpdateItemInfoList;		// �߿�! �������� �����ϴ� �����ۿ� ���ؼ��� ó���Ǿ�ߵȴ�.
	mtvectorInfinityDeleteItemInfo		m_mtInfinityDeleteItemInfoList;		// �߿�! �������� �����ϴ� �����ۿ� ���ؼ��� ó���Ǿ�ߵȴ�.

	INT							m_nInventoryHiddenItemCount;				// 2011-10-21 by hskim, ��Ʈ�� �ý��� 2�� - ���� ������ �ִ� ������ ������ (POS_HIDDEN_ITEM) ����

private:
	vectITEM_GENERALPtr			m_vectUsingGeneralItem;			// ������� ������
	DWORD						m_dwTimeLimiteCardItemCheckCounts;
	vectINT						m_vectInsertingItemNumList2DB;	// 2008-01-31 by cmkwon, �ð����� ������ DB �� �߰� �� üũ - ��� �� DB �� �߰� ������ ItemNum ����Ʈ

	///////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
	vectITEM_GENERALPtr			m_InvokingItemList;			// �ߵ��� ������ ��ߵ� �ð� ��� ���
};

#endif // _MGR_FIELD_ITEM_H_
