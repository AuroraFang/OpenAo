#ifndef _ATUM_LOG_SENDER_H_
#define _ATUM_LOG_SENDER_H_

#include "FieldIOCP.h"

// 2012-10-16 by bckim, ĳ�������� �������� Log �߰�
enum ITEM_BUY_CASH_PRICE_TYPE{BUY_TYPE_SPI=1,BUY_TYPE_WARPOINT,BUY_TYPE_CASH,BUY_TYPE_INFINITY};
// end 2012-10-16 by bckim, ĳ�������� �������� Log �߰�

class CAtumLogSender  
{
public:
	CAtumLogSender();
	virtual ~CAtumLogSender();

	//static BOOL SendLogMessage(MessageType_t msgType, const char *szAction);

	// ���� �α�
	static BOOL SendLogMessageLOGIN(CFieldIOCPSocket *pCharacterSocket, UID32_t AccountUniqueNumber);
	static BOOL SendLogMessageLOGOUT(CFieldIOCPSocket *pCharacterSocket);

	// ĳ���� �α�
	static BOOL SendLogMessageLEVEL(CFieldIOCPSocket *pCharacterSocket, int fromLevel, int toLevel);
	static BOOL SendLogMessageEXP(CFieldIOCPSocket *pCharacterSocket, Experience_t changeExp, Experience_t CharacterExp);
	static BOOL SendLogMessageSTAT(CFieldIOCPSocket *pCharacterSocket, BYTE kind, BYTE amount = 1.0);
	static BOOL SendLogMessageGAMESTART(CFieldIOCPSocket *pCharacterSocket);
	static BOOL SendLogMessageGAMEEND(CFieldIOCPSocket *pCharacterSocket);
	static BOOL SendLogMessageQUESTCOMPLETION(CFieldIOCPSocket *pCharacterSocket, UID32_t QuestUniqueNumber);
	static BOOL SendLogMessageQUESTCOMPENSATION(CFieldIOCPSocket *pCharacterSocket, MSG_FL_LOG_QUEST_COMPENSATION *i_pMsgQuestCompensation, string &i_szCompensationItemList);
	static BOOL SendLogMessagePKWIN(CFieldIOCPSocket *pCharacterSocket, UID32_t PeerCharacterUniqueNumber);
	static BOOL SendLogMessagePKLOSS(CFieldIOCPSocket *pCharacterSocket, UID32_t PeerCharacterUniqueNumber);
	static BOOL SendLogMessagePvEWIN(CFieldIOCPSocket *pCharacterSocket, UID32_t PeerCharacterUniqueNumber);
	static BOOL SendLogMessagePvELOSS(CFieldIOCPSocket *pCharacterSocket, UID32_t PeerCharacterUniqueNumber);
	static BOOL SendLogMessageDEAD(CFieldIOCPSocket *pCharacterSocket, BYTE i_nDamageType, ItemNum_t i_ItemNum);		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�

	// 2007-11-30 by cmkwon, �����ʿ� ä���� �ٸ� ��쵵 ���� ó�� - SendLogMessageWARP() ���� �߰�
	static BOOL SendLogMessageWARP(CFieldIOCPSocket *pCharacterSocket, MapIndex_t CurrentMapIndex, ChannelIndex_t CurChannIdx, AVECTOR3 &CurrentMapPosition, MapIndex_t TargetMapIndex, ChannelIndex_t TargetChannIdx, AVECTOR3 &TargetMapPosition);
	static BOOL SendLogMessageSpeedHack(CFieldIOCPSocket *pCharacterSocket, BYTE i_byCheckType, INT i_nRecvMoveCounts, INT EngineItemNum,
										INT ServerPastTime, INT ClientPastTime, INT PastDistance, INT CurrentSpeed);
	static BOOL SendLogMessageMemoryHack(CFieldIOCPSocket *pCharacterSocket, INT i_itemKind, INT i_itemNum, UID64_t i_itemUID, INT i_MemHackCheckTy, float i_fValidValue, float i_fCurrentValue, int i_nParam1=0);
	static BOOL SendLogMessageCREATECHARACTER(CHARACTER *pCharacter);
	static BOOL SendLogMessageDELETECHARACTER(CHARACTER *pCharacter);

	// ������ ���� �α�
	static BOOL SendLogMessageMonsterBoss(MSG_FL_LOG_MONSTER_BOSS *i_pMsgBossMonLog, string &i_szDropItemList);
	static BOOL SendLogMessageMSWar(MSG_FL_LOG_MS_WAR *i_pMsgMSWarLog);		// 2008-04-01 by dhjin, ����, ������ ����â ��ȹ�� - 
	static BOOL SendLogMessageSPWar(MSG_FL_LOG_SP_WAR *i_pMsgSPWarLog);		// 2008-04-01 by dhjin, ����, ������ ����â ��ȹ�� - 

	// ������ �α�(���� �Ӵ� �α� ����)

	// 2008-02-15 by cmkwon, �κ�<->â�� �̵� �α� ���� - CAtumLogSender::SendLogMessageITEMCharacterITEM() ���� �߰�
	static BOOL SendLogMessageITEMCharacterITEM(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, int i_nChangeCnts);				// ���� ���� �� ��� �������� ����Ʈ�� ����

	static BOOL SendLogMessageITEMCharacterToFromGuildstoreResult(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, int i_nChangeCnts);	// 2012-12-17 by bckim, ���â�� ������ �̵� ���� �߰��α� 
		static BOOL SendLogMessageITEMGetWithSameAccount(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, UID32_t i_StoreCharacterUID);	// 2012-12-27 by bckim, ���ϰ���, ������ �ɸ��Ͱ��� ������ �̵��� �α��߰�
	
	// 2008-09-26 by cmkwon, ���ս� GameLog ���� - 
	//static BOOL SendLogMessageITEMGetITEM(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount);						// Drop Item ����
	static BOOL SendLogMessageITEMGetITEM(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, BYTE i_byItemUpdateType);			// Drop Item ����, ����Ʈ ���� ������ ����, 2���� �Ϸ� ������ ����
	#ifdef _ATUM_SERVER
	static BOOL SendLogMessageITEMDissolutionITEM(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, BYTE i_byItemUpdateType);	// 2013-08-05 by bckim, ���� �������� �α� �߰�
	#endif
	static BOOL SendLogMessageITEMTradeSend(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, CHARACTER *pToChar);		// ������ �ŷ�: �ֱ�
	static BOOL SendLogMessageITEMTradeRecv(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, CHARACTER *pFromChar);	// ������ �ŷ�: �ޱ�
	static BOOL SendLogMessageITEMThrowAwayITEM(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, vectENCHANT *i_pVectEnchantList);					// ������ ������
	static BOOL SendLogMessageITEMTryEnchantInfo(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, vectENCHANT *i_pVectEnchantList);		// 2013-07-01 by bckim, 10��æƮ �̸� ������ī��ó��, �α��߰�, ��� ����
	// 2010-01-26 by cmkwon, ĳ�� ������ �����Ǹ� �ý��� ���� - 
	static BOOL SendLogMessageITEMBuyITEM(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, INT RemainedMoney, BOOL bWarPoint = FALSE, INT i_nRemainCount=UNLIMITED_REMAIN_COUNT_FOR_LIMITED_EDITION, BOOL bGLog = TRUE);	// 2011-12-12 by hskim, GLog 2�� // ������ ����

	// 2012-10-16 by bckim, ĳ�������� �������� Log �߰�
	static BOOL SendLogMessageITEMBuyCashPrice(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nAmount, INT64 nSpend, BOOL bDummy , INT ntype, INFINITY_SHOP_INFO *pBuyInfinityItemInfo);
	// end 2012-10-16 by bckim, ĳ�������� �������� Log �߰�

	static BOOL SendLogMessageITEMSellITEM(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, INT RemainedMoney, vectENCHANT *i_pVectEnchantList=NULL);	// 2007-07-20 by cmkwon, �Ǹž����� �α� �߰� - �����߰�(, vectENCHANT *i_pVectEnchantList=NULL), // ������ �Ǹ�
	static BOOL SendLogMessageITEMUseITEM(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem);											// ������ ���
	static BOOL SendLogMessageITEMUseENERGY(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem);										// ������ ���

	// 2010-04-20 by cmkwon, �ű� ��Ű �ӽ� ���� - 
	// 2010-04-20 by cmkwon, �ű� ��Ű �ӽ� ���� - (�߰� ����) - 
	//static BOOL SendLogMessageITEMUseENCHANT(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, int i_enchantLogType, int i_nPrefixCodeNum=0, int i_nSuffixCodeNum=0);	// 2007-07-30 by cmkwon, ����/��æƮ �α׿� ����/���� ���� ���� - �����߰�(, int i_nPrefixCodeNum=0, int i_nSuffixCodeNum=0), // ��æƮ ���
	static BOOL SendLogMessageITEMUseENCHANT(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *i_pTargetItemG, ITEM_GENERAL *i_pEnchantItemG, int i_nEnchantCnt, BOOL i_bSuccessFlag, ITEM_GENERAL *i_pAddedItemG_1=NULL, ITEM_GENERAL *i_pAddedItemG_2=NULL);

	static BOOL SendLogMessageITEMAddItemByAdmin(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount);				// ����� ���� ����
	static BOOL SendLogMessageITEMDelItemByAdmin(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem);									// ����� ���� ����
	static BOOL SendLogMessageITEMAddItemByCommand(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount);				// ��ɾ ���� ����
	static BOOL SendLogMessageITEMInsertToStore(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, UID32_t i_StoreCharacterUID);					// â�� ����
	static BOOL SendLogMessageITEMDrawFromStore(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, UID32_t i_StoreCharacterUID);					// â���� ������
	//static BOOL SendLogMessageSKILL(const char *szAction);

	// 2008-02-15 by cmkwon, �κ�<->â�� �̵� �α� ���� - CAtumLogSender::SendLogMessageITEMStoreITEM() ���� �߰�
	static BOOL SendLogMessageITEMStoreITEM(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, int i_nChangeCnts);					// â�� ���� ������

	// 2008-09-26 by cmkwon, ���ս� GameLog ���� - 
	//static BOOL SendLogMessageITEMUseMix(CFieldIOCPSocket *pCharacterSocket, INT i_ItemNum, INT i_nCurrentCount, UID64_t i_64ItemUID=0);	// ����, CurrentCount( 0�̸� ���� 1�̸� ����)
	static BOOL SendLogMessageITEMUseMix(CFieldIOCPSocket *pCharacterSocket, BOOL i_bIsSuccess, ITEM_GENERAL *i_pTargetItemG, ITEM_UID_W_ITEMNUM_COUNT i_arrSourceItemList[COUNT_ITEM_MIXING_SOURCE]);

	static BOOL SendLogMessageITEMUseChangeCharacterName(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, char *i_szOriginCharName);	// ĳ���� ����, atum_backup_log_item_trade ���̺� ����
	static BOOL SendLogMessageITEMBazaarSend(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, CHARACTER *pToChar);	// ���λ��� �ŷ�: �ֱ�
	static BOOL SendLogMessageITEMBazaarRecv(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount, CHARACTER *pFromChar);	// ���λ��� �ŷ�: �ޱ�
	static BOOL SendLogMessageITEMInsertToGuildStore(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount);	// 2006-09-27 by dhjin, ���� â�� ����.
	static BOOL SendLogMessageITEMDrawFromGuildStore(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount);	// 2006-09-27 by dhjin, ���� â���� ������.
	static BOOL SendLogMessageITEMSPI(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, BYTE i_byItemUpdateTy, INT nChangeCount, INT RemainedMoney);	// 2006-10-27 by cmkwon, SPI �α�
	static BOOL SendLogMessageITEMGetItemByItemEvent(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nChangeCount);	// 2007-01-11 by cmkwon, �������̺�Ʈ�� ���� ������ �߰�

	// 2009-01-22 by cmkwon, ���� ���� ���� �α� ���� - CAtumLogSender::SendLogMessageGetWarPoint()�� ����(, BYTE i_byWPUpdateTy) �߰�
	static BOOL SendLogMessageGetWarPoint(CFieldIOCPSocket *pCharacterSocket, INT i_nChangeWarPoint, INT i_nTotalWarPoint, INT i_nCumulativeWarPoint, BYTE i_byWPUpdateTy);			// 2007-05-15 by dhjin, ������Ʈ ���� �α�
	static BOOL SendLogMessageArenaTeamMatch(BYTE i_nTeamMode, BYTE i_nTeamSize, BYTE i_nStartLevel, BYTE i_nEndLevel, MapIndex_t i_nArenaMap/**jhseol*/);		// 2007-06-11 by dhjin, �� ��Ī�� �α�		// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ������ �Ʒ��� �� �α� �߰�
	// 2007-07-06 by dhjin, Tutorial
	static BOOL SendLogMessageTutorialCOMPLETION(CFieldIOCPSocket *pCharacterSocket, INT TutorialNumber);

	// 2008-01-23 by cmkwon, S_F, S_L: ����/�������� ���� �α׿� �߰� - 
	static BOOL SendLogMessageITEMAttachItem(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *i_pItemG);	// ������ ����
	static BOOL SendLogMessageITEMDetachItem(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *i_pItemG);	// ������ ��������
	static BOOL SendLogMessageITEMDelete(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *i_pItemG, BYTE i_byItemDeleteTy, INT i_nUsedTime=0);	// ������ ���� // 2009-11-17 by cmkwon, �������� ���� ������ ������ �α� ���� �߰� - �����߰�(, INT i_nUsedTime/*=0*/)

	// 2008-01-23 by cmkwon, S_F, S_L: ���� ��� ���� �α׿� �߰� - CAtumLogSender::SendLogMessageUseCouponNumber() �߰�
	static BOOL SendLogMessageUseCoupon(CFieldIOCPSocket *pCharacterSocket, char *i_szAccName, char *i_szCouponNumber);	// ���� ��� �α�

	// 2007-10-02 by dhjin, ��������, �ǰ��� ����
	// 2007-10-16 by cmkwon, �α� �߰� - ���� ����
	static BOOL SendLogMessageOutPostStart(MapIndex_t i_nOutPostMapIdx, BYTE i_byPossessInflTy, UID32_t i_possessGuildUID);
	// 2007-10-16 by cmkwon, �α� �߰� - ���� ����
	static BOOL SendLogMessageOutPostEnd(MapIndex_t i_nOutPostMapIdx, BYTE i_byPossessInflTy, UID32_t i_possessGuildUID, char *i_szPossessGuildName);

	static BOOL SendLogMessageInflWarStart(MapIndex_t i_nMothershipCreatedMapIdx, BYTE i_byInflTy, INT i_nMonsterUnitKind, BYTE i_byMonBell);					// 2007-10-16 by cmkwon, �α� �߰� -
	static BOOL SendLogMessageInflWarEnd(MapIndex_t i_nMothershipCreatedMapIdx, BYTE i_byInflTy, INT i_nMonsterUnitKind, BYTE i_byMonBell, BOOL i_bDeadFlag);	// 2007-10-16 by cmkwon, �α� �߰� -
	static BOOL SendLogMessageOutPostResetStart(MapIndex_t i_nOutPostMapIdx, BYTE i_byPossessInflTy, UID32_t i_possessGuildUID, BYTE i_byInflTy4Reset, UID32_t i_guildUID4Reset, UID32_t i_resetUserGuildUID, UID32_t i_resetUserCharacterUID);	// 2007-10-16 by cmkwon, �α� �߰� -
	static BOOL SendLogMessageOutPostResetDestroy(MapIndex_t i_nOutPostMapIdx, BYTE i_byPossessInflTy, UID32_t i_possessGuildUID, BYTE i_byInflTy4Reset, UID32_t i_guildUID4Reset);	// 2007-10-16 by cmkwon, �α� �߰� -
	static BOOL SendLogMessageOutPostResetComplete(MapIndex_t i_nOutPostMapIdx, BYTE i_byPossessInflTy, UID32_t i_possessGuildUID);													// 2007-10-16 by cmkwon, �α� �߰� -
	static BOOL SendLogMessageOutPostProtectorDestroy(MapIndex_t i_nOutPostMapIdx, BYTE i_byPossessInflTy, UID32_t i_possessGuildUID, BYTE i_byInflTy4Reset, UID32_t i_guildUID4Reset, mtvectSOutPostProtectorDamage *i_pvectDamageList);	// 2007-10-16 by cmkwon, �α� �߰� -
	static BOOL SendLogMessageOutPostSetNextTime(MapIndex_t i_nOutPostMapIdx, BYTE i_byPossessInflTy, UID32_t i_possessGuildUID, UID32_t i_userGuildUID, UID32_t i_userCharacterUID, ATUM_DATE_TIME *i_patNextTime);	// 2007-10-16 by cmkwon, �α� �߰� -

	// 2007-10-16 by cmkwon, �α� �߰� - ���� �߰�(CFieldIOCPSocket *i_pCharFISoc, INT i_nExpediencyFundRate)
	static BOOL SendLogMessageExpediencyFund(CFieldIOCPSocket *i_pCharFISoc, MapIndex_t i_nMapIndex, BOOL i_bIncrease, INT i_nCount, INT i_nExpediencyFundRate, INT i_nTotalCount);

	// 2007-11-08 by dhjin, poll�α�
	static BOOL SendLogMessagePollVote(UID32_t i_nVoteCharacterUID, UID32_t i_nCharacterUID);				// 2007-11-08 by dhjin
	static BOOL SendLogMessageDeleteLeaderCandidate(UID32_t i_nCharacterUID);								// 2007-11-08 by dhjin

	// 2007-11-09 by dhjin, ���� ��ü �α�
	static BOOL SendLogMessageDisMemberGuild(CFieldIOCPSocket *i_pCharFISoc);

	// ���� �α�
	static BOOL SendLogMessageServerInfoTotal(INT i_nClientCount, INT i_nMonsterCount, BOOL i_bGlogUpdate);		// 2010-06-01 by shcho, GLogDB ���� -
	static BOOL SendLogMessageServerInfoMap(const MAP_CHANNEL_INDEX &i_MapChannelIndex, INT i_nClientCount, INT i_nMonsterCount);

	static BOOL SendLogMessageHackingLog(CFieldIOCPSocket *pCharFISock, long i_hackingCode, char *i_szErrString=NULL);

	// 2007-11-13 by cmkwon, �����ϱ� ��� �߰� - 
	static BOOL SendLogMessageITEMGiftSend(CFieldIOCPSocket *i_pFISoc, ITEM_GENERAL *i_pItemGen, INT i_nChangeCnts, SGIVE_TARGET_CHARACTER *i_pToChar);
	// 2007-11-27 by cmkwon, �����ϱ� �α� ���� - CAtumLogSender::SendLogMessageITEMGiftRecv() �Լ� ���� ����
	static BOOL SendLogMessageITEMGiftRecv(UID32_t i_targetCharUID, ITEM_GENERAL *i_pItemGen, INT i_nChangeCnts, UID32_t i_senderCharUID, char *i_senderCharacterName);

	// 2007-11-28 by cmkwon, �����ý��� ���� -
	static BOOL SendLogMessageNotifyMsgDelete(SNOTIFY_MSG *i_pNotifyMsg);

	// 2007-11-29 by cmkwon, ī���ͺ������ �������� ���� �α� ����� - 
	static BOOL SendLogMessageRearrangeDeleteItem(CFieldIOCPSocket *i_pFISoc, ITEM_GENERAL *i_pDelItemGen, INT i_nChangeCnts, BOOL i_bStoreItem=FALSE);
	static BOOL SendLogMessageRearrangeItem(CFieldIOCPSocket *i_pFISoc, ITEM_GENERAL *i_pItemGen, INT i_nChangeCnts, BOOL i_bStoreItem=FALSE);

	//////////////////////////////////////////////////////////////////////////
	// 2008-12-23 by dhjin, ���� ���� �߰���
	static BOOL SendLogMessageWarContribution(UID32_t i_nUID, MapIndex_t i_nMapIndex, UID64_t i_nContribution, INT i_nPay, BYTE i_byPayType);
	static BOOL SendLogMessageWarContributionGear(MSG_FL_LOG_WAR_CONTRIBUTION_GEAR * i_pContributionGear);

	// 2009-03-31 by dhjin, ��Ű�ӽ� �α�
	static BOOL SendLogMessageLuckyDrop(MSG_FL_LOG_LUCKY_DROP * i_pLuckyDrop);

	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�
	static BOOL SendLogMessageInfinityStart(CFieldIOCPSocket * i_pPlayerSoc);											// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� ���� ���� 
	static BOOL SendLogMessageInfinityStartItem(CFieldIOCPSocket * i_pPlayerSoc, ITEM_GENERAL *i_pItemGen);				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���۽� ������ ���� ����
	static BOOL SendLogMessageInfinityCinema(CINEMAINFO * i_pCinemaInfo, InfinityCreateUID_t i_InfinityCreateUID);		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� �α�
	static BOOL SendLogMessageMonsterSkill(MonIdx_t i_MonIdx, ItemNum_t i_ItemNum);										// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� ��ų ����
	static BOOL SendLogMessageHPActionTalk(MonIdx_t i_MonIdx, char * i_pHPTalk);										// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� ��ȭ ����
	static BOOL SendLogMessageInfinityFin(CFieldIOCPSocket * i_pPlayerSoc, BOOL i_bClear);								// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� ���� ���� 
	static BOOL SendLogMessageInfinityFinItem(CFieldIOCPSocket * i_pPlayerSoc, ITEM_GENERAL *i_pItemGen);				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ����� ������ ���� ����
	static BOOL SendLogMessageInfinityFinAliveKeyMonster(MonIdx_t i_MonIdx);											// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ����� ����ִ� ���� ����
	static BOOL SendLogMessageInfinityLeaveItem(CFieldIOCPSocket * i_pPlayerSoc, ITEM_GENERAL *i_pItemGen);				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� Ż��� ������ ���� ����

	// 2010-06-01 by shcho, GLogDB ���� -
	static BOOL SendGLogEventParticipationRate(MSG_FL_LOG_EVENT_PARTICIPATION_RATE * i_pEventMSG);

	// 2011-09-15 by hskim, ��Ʈ�� �ý��� 2�� - ��Ʈ�� ���� ����
	static BOOL SendLogMessagePetLEVEL(CFieldIOCPSocket *pCharacterSocket, UID64_t nUniqueNumber, Experience_t PetTotalExp, Experience_t PetAcquisitionExp, int fromPetLevel, int toPetLevel);		// 2012-10-04 by hskim, �ѱ� ��ü ���� (����� IDC) - ��Ʈ�� �α� ����

	// start 2012-01-08 by hskim, GLog 2��
	static BOOL SendLogMessageConnectUserNew(char *pAccountName);
	static BOOL SendLogMessageConnectUserLogin(char *pAccountName);

	// start 2012-01-16 by hskim, ��� - ȭ��
	static BOOL SendLogMessageStatisticsMeney(MSG_FL_LOG_STATISTICS_MONEY *i_pSMoney);
	
	// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)
	static BOOL SendLogMessageFixedTermShapeStart(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, ItemNum_t ShapeItemNum);
	static BOOL SendLogMessageFixedTermShapeEnd(CFieldIOCPSocket *pCharacterSocket, ITEM_GENERAL *pItem, INT nShapeItemNum, FIXED_TERM_INFO FixedTermShape);
	// end 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)

	// start 2012-10-08 by khkim, GLog
	static BOOL SendLogAccountConnect(MSG_FL_LOG_ACCOUNTCONNECT *i_AccountParameter);
	static BOOL SendLogCharData(MSG_FL_LOG_CHARDATA *i_CharData);
	static BOOL SendLogItem(MSG_FL_LOG_ITEM *i_Item);
	static BOOL SendLogItemState(MSG_FL_LOG_ITEMSTATE *i_ItemState);		
	static BOOL SendLogServer(MSG_FL_LOG_SERVER *i_Server);
	// end 2012-10-08 by khkim, GLog	

	static BOOL SendLogMessageCollectionState(CFieldIOCPSocket *pCharacterSocket, COLLECTION_INFO *i_pCollection, BYTE i_nState);		// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - GameLog �߰�
	static BOOL SendLogMessageCollectionShapeChange(MSG_FL_LOG_COLLECTION_SHAPE_CHANGE *i_pShapeInfo);									// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - GameLog �߰�

	static BOOL SendLogMessageAccountInflChange_byAdmin(UID32_t i_nAccountUID, UID32_t i_nCharacterUID, BYTE i_nBeforeInfl, BYTE i_nAfterInfl, UID32_t i_nAdminCharacterUID);	// 2013-07-26 by jhseol, Ÿ ���� ���º��� - GameLog �߰�

public:
	CFieldIOCP		*m_pFieldIOCP;
};

#endif // _ATUM_LOG_SENDER_H_
