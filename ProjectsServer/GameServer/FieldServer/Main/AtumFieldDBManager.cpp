#include "stdafx.h"
#include "AtumFieldDBManager.h"
#include "FieldIOCPSocket.h"
#include "FieldIOCP.h"
#include "FieldGlobal.h"
#include "AtumLogSender.h"
#include "odbcss.h"
#include "AtumError.h"
#include "FieldParty.h"
#include "HappyHourEventManager.h"
#include "GiveItemManager.h"
#include "AllGuildItemManager.h"
#include "GuildItemManager.h"
#include "PCBangIPManager.h"
#include "OutPostManager.h"

CAtumFieldDBManager::CAtumFieldDBManager(CFieldIOCP *pFieldIOCP)
:CAtumDBManager(COUNT_FIELD_SERVER_DBWORKER)
{
	m_pFieldIOCP8 = pFieldIOCP;

	m_dwCountDBWorker	= 10;
}

void CAtumFieldDBManager::MakeAndEnqueueQuery(EnumQueryType type, CFieldIOCPSocket* pFieldIOCPSocket, UID32_t i_uidAccountUID
											  , void *pMsg, void* i_pGeneralParam /* = NULL */
											  , INT64 i_nGeneralParam1 /* = 0 */, INT64 i_nGeneralParam2 /* = 0 */)
{
	// make query, enqueue
	DB_QUERY dbquery;

	dbquery.pIOCPSocket		= pFieldIOCPSocket;
	if(pFieldIOCPSocket)
	{
		dbquery.nCharacterUID	= pFieldIOCPSocket->GetCharacter()->CharacterUniqueNumber;
	}
	dbquery.enumQueryType	= type;
	dbquery.pQueryParam		= (void*)pMsg;
	dbquery.pGeneralParam	= i_pGeneralParam;
	dbquery.nGeneralParam1	= i_nGeneralParam1;
	dbquery.nGeneralParam2	= i_nGeneralParam2;

	int nDBThreadIndex		= i_uidAccountUID%m_dwCountDBWorker;	// 1 ~ (m_dwCountDBWorker-1)	
	m_arrmtvectorInputDBQuery[nDBThreadIndex].lock();
	m_arrmtvectorInputDBQuery[nDBThreadIndex].push_back(dbquery);
	m_arrmtvectorInputDBQuery[nDBThreadIndex].unlock();
}

BOOL CAtumFieldDBManager::CheckClientState(CIOCPSocket *i_pSocket)
{
	CFieldIOCPSocket *pFieldSocket = (CFieldIOCPSocket*)i_pSocket;

	if ( (pFieldSocket == NULL)
		|| !pFieldSocket->IsUsing()
		|| (pFieldSocket->GetClientState() != CS_PLAYING)
	)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CAtumFieldDBManager::ProcessServerQuery(DB_QUERY dbquery, SQLHSTMT &hstmt, SQLHSTMT &hstmt_mc, SQLHSTMT &hstmt_extAuth, SQLHSTMT &hstmt_GLog)		// 2013-06-20 by jhseol,bckim GLog ����
{
	FIELD_DB_QUERY FieldDBQuery = dbquery;

	switch(dbquery.enumQueryType)
	{
	case QT_CreateCharacter:			QP_CreateCharacter(dbquery, hstmt);				break;
	case QT_DeleteCharacter:			QP_DeleteCharacter(dbquery, hstmt);				break;
	case QT_GetAccountUniqueNumber:		QP_GetAccountUniqueNumber(dbquery, hstmt);		break;
	case QT_GetCharacterInfoByName:		QP_GetCharacterInfoByName(dbquery, hstmt);		break;
// 2005-07-21 by cmkwon, �ٸ� �ʵ弭������ GameStart�� �����Ƿ� ������
//	case QT_ConnectGameStart:			QP_ConnectGameStart(dbquery, hstmt);			break;
	case QT_GetAllCharacterInfoByID:	QP_GetAllCharacterInfoByID(dbquery, hstmt);		break;
	case QT_SaveCharacterCriticalData:	QP_SaveCharacterCriticalData(dbquery, hstmt);	break;
	case QT_ChangeUnitKind:				QP_ChangeUnitKind(dbquery, hstmt);				break;
	case QT_ChangeStat:					QP_ChangeStat(dbquery, hstmt);					break;
	case QT_ChangeBonusStatPoint:		QP_ChangeBonusStatPoint(dbquery, hstmt);		break;
	case QT_ChangeGuild:				QP_ChangeGuild(dbquery, hstmt);					break;
	// deprecated
	case QT_ChangeFriendGroup:															break;
	case QT_ChangeExp:					QP_ChangeExp(dbquery, hstmt);					break;
	case QT_ChangeLevel:				QP_ChangeLevel(dbquery, hstmt);					break;
	case QT_ChangeBodyCondition:		QP_ChangeBodyCondition(dbquery, hstmt);			break;

//	case QT_ChangePropensity:			QP_ChangePropensity(dbquery, hstmt);			break;

	case QT_ChangeInfluenceType:		QP_ChangeInfluenceType(dbquery, hstmt);			break;
	case QT_ChangeStatus:				QP_ChangeStatus(dbquery, hstmt);				break;
	case QT_ChangePKPoint:				QP_ChangePKPoint(dbquery, hstmt);				break;
// 2009-11-02 by cmkwon, ĳ��(�κ�/â�� Ȯ��) ������ �߰� ���� - ������� �����Ƿ� ����
//	case QT_ChangeRacingPoint:			QP_ChangeRacingPoint(dbquery, hstmt);			break;
	case QT_UpdateTotalPlayTime:		QP_UpdateTotalPlayTime(dbquery, hstmt);			break;
	case QT_UpdateLastStartedTime:		QP_UpdateLastStartedTime(dbquery, hstmt);		break;
	case QT_ChangeHPDPSPEP:				QP_ChangeHPDPSPEP(dbquery, hstmt);				break;
	case QT_ChangeCurrentHPDPSPEP:		QP_ChangeCurrentHPDPSPEP(dbquery, hstmt);		break;
	case QT_ChangeMapChannel:			QP_ChangeMapChannel(dbquery, hstmt);			break;
	case QT_ChangePetInfo:				QP_ChangePetInfo(dbquery, hstmt);				break;
	// deprecated
	case QT_ChangeDockingItem:															break;
	case QT_ChangePosition:				QP_ChangePosition(dbquery, hstmt);				break;
// 2004-12-16 by cmkwon, �ٸ� �ʵ弭������ ������ �����Ƿ� ������
//	case QT_EventWarpConnect:			QP_EventWarpConnect(dbquery, hstmt);			break;

// store(item) table ���� �Ŀ� ������
//	case QT_ChangeDockingItem:			QP_ChangeDockingItem(dbquery, hstmt);			break;
	// Item ���� query��
// 2007-01-29 by cmkwon, QP_GetStoreItem()�Լ��� ���� �۾� ����
//	case QT_GetStoreItem:			QP_GetStoreItem(dbquery, hstmt);			break;
	case QT_GetStoreItem:			QP_NewGetStoreItem(dbquery, hstmt);			break;
	case QT_ReloadAllEnchant:		QP_ReloadAllEnchant(dbquery, hstmt);		break;
	case QT_UpdateItemPossess:		QP_UpdateItemPossess(dbquery, hstmt);		break;
	case QT_UpdateItemStorage:		QP_UpdateItemStorage(dbquery, hstmt);		break;
	case QT_LoadOneItem:			QP_LoadOneItem(dbquery, hstmt);				break;
	case QT_INSERTLOGINITEMEVENT:	QP_InsertLoginItemEvent(dbquery, hstmt);	break;		// 2011-08-25 by shcho, Ƚ���� ������ ���ޱ�� ����
	case QT_CheckEventItem:			QP_CheckEventItem(dbquery, hstmt);			break;
	case QT_InsertEventItem:		QP_InsertEventItem(dbquery, hstmt);			break;
	case QT_UpdateEventItemFixedPeriod:	QP_UpdateEventItemFixedPeriod(dbquery, hstmt);			break;	// 2013-02-28 by bckim, �������� �����߰�
	case QT_CheckCouponEvent:		QP_CheckCouponEvent(dbquery, hstmt);		break;		// 2008-01-10 by cmkwon, ������ �̺�Ʈ �ý��ۿ� �� ���� �ý��� �߰� - 
	case QT_GetGuildStoreItem:		QP_GetGuildStoreItem(dbquery, hstmt);		break;
	case QT_UpdateGuildStoreItem:	QP_UpdateGuildStoreItem(dbquery, hstmt);	break;
	case QT_InsertGuildStoreItem:	QP_InsertGuildStoreItem(dbquery, hstmt);	break;
	case QT_DeleteGuildStoreItem:	QP_DeleteGuildStoreItem(dbquery, hstmt);	break;
	case QT_GetLogGuildStoreItem:	QP_GetLogGuildStoreItem(dbquery, hstmt);			break;
	case QT_AllDeleteGuildStoreItem:	QP_AllDeleteGuildStoreItem(dbquery, hstmt);	break;
	case QT_Insert2WarpableUserList:	QP_Insert2WarpableUserList(dbquery, hstmt);	break;	// 2007-08-30 by cmkwon, ȸ�Ƿ� �ý��� ���� - �����㰡 �߰�	
	case QT_DeleteWarpableUser:			QP_DeleteWarpableUser(dbquery, hstmt);		break;	// 2007-08-30 by cmkwon, ȸ�Ƿ� �ý��� ���� - �����㰡 ����
	//case QT_UPDATE_ConnectingServerGroupID:		QP_UPDATE_ConnectingServerGroupID(dbquery, hstmt);		break;	// 2007-11-06 by cmkwon, ���� �α� DB ���� ���� �����ϱ� - �߰� ��
	case QT_GiveStoreItem:				QP_GiveStoreItem(dbquery, hstmt);		break;		// 2007-11-13 by cmkwon, �����ϱ� ��� �߰� - 
	case QT_GetLetter:					QP_GetLetter(dbquery, hstmt);		break;		// 2008-04-24 by dhjin, EP3 ���� �ý��� - DB���� ���� ��������
	case QT_ReadLetter:					QP_ReadLetter(dbquery, hstmt);			break;		// 2008-04-24 by dhjin, EP3 ���� �ý��� - ���� �б�
	case QT_DeleteLetter:				QP_DeleteLetter(dbquery, hstmt);		break;		// 2008-04-24 by dhjin, EP3 ���� �ý��� - ���� ����
	case QT_SendLetter:					QP_SendLetter(dbquery, hstmt);			break;		// 2008-05-08 by dhjin, EP3 ���� �ý��� - ���� ������
	case QT_GetAllLetter:				QP_GetAllLetter(dbquery, hstmt);		break;		// 2008-05-09 by dhjin, EP3 ���� �ý��� - DB���� ��ü ���� ��������
	case QT_SendAllLetter:				QP_SendAllLetter(dbquery, hstmt);		break;		// 2008-05-09 by dhjin, EP3 ���� �ý��� - ��ü ���� ������
	case QT_ReadAllLetter:				QP_ReadAllLetter(dbquery, hstmt);		break;		// 2008-05-09 by dhjin, EP3 ���� �ý��� - ��ü ���� �б�
	case QT_DeleteAllLetter:			QP_DeleteAllLetter(dbquery, hstmt);		break;		// 2008-05-09 by dhjin, EP3 ���� �ý��� - ��ü ���� ����
	case QT_DeleteOldAllLetter:			QP_DeleteOldAllLetter(dbquery, hstmt);		break;		// 2008-05-09 by dhjin, EP3 ���� �ý��� - ������ ��ü ���� ����

	case QT_DeleteStoreItem:		QP_DeleteStoreItem(dbquery, hstmt);			break;
	case QT_InsertStoreItem:		QP_InsertStoreItem(dbquery, hstmt);			break;
	case QT_InsertStoreItemSkill:	QP_InsertStoreItemSkill(dbquery, hstmt);	break;
	case QT_UpdateStoreItemSkillUseTime:	QP_UpdateStoreItemSkillUseTime(dbquery, hstmt);	break;
	case QT_UpdateItemCount:		QP_UpdateItemCount(dbquery, hstmt);			break;
	case QT_UpdateItemNum:			QP_UpdateItemNum(dbquery, hstmt);			break;
	case QT_UpdateEndurance:		QP_UpdateEndurance(dbquery, hstmt);			break;
	case QT_StoreUpdateColorCode:	QP_StoreUpdateColorCode(dbquery, hstmt);	break;
	case QT_UpdateShapeItemNum:		QP_UpdateShapeItemNum(dbquery, hstmt);		break;		// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 

	case QT_UpdateItemUsingTimeStamp:	QP_UpdateItemUsingTimeStamp(dbquery, hstmt);	break;
	case QT_UpdateItemRareFix:		QP_UpdateItemRareFix(dbquery, hstmt);		break;
	case QT_InsertDefaultItems:		QP_InsertDefaultItems(dbquery, hstmt);		break;
	case QT_ChangeItemPosition:		QP_ChangeItemPosition(dbquery, hstmt);		break;
	case QT_UpdateWindowItemList:	QP_UpdateWindowItemList(dbquery, hstmt);	break;

// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) - ������� �ʴ� �Լ�
//	case QT_ExecuteTrade:			QP_ExecuteTrade(dbquery, hstmt_mc);			break;

	case QT_TradeMoveItem:			QP_TradeMoveItem(dbquery, hstmt);			break;

	case QT_InsertEnchant:			QP_InsertEnchant(dbquery, hstmt);			break;
	case QT_DeleteAllEnchant:		QP_DeleteAllEnchant(dbquery, hstmt);		break;
// 2007-12-21 by dhjin, ���� ���� ��ƾ DB�� ������ Client���� �κ� �и�
//	case QT_GetAllQuest:			QP_GetAllQuest(dbquery, hstmt);				break;
	case QT_InsertQuest:			QP_InsertQuest(dbquery, hstmt);				break;
	case QT_UpdateQuestState:		QP_UpdateQuestState(dbquery, hstmt);		break;
	case QT_DeleteQuest:			QP_DeleteQuest(dbquery, hstmt);				break;
	case QT_UpdateQuestMonsterCount:	QP_UpdateQuestMonsterCount(dbquery, hstmt);				break;
	case QT_DeleteQuestMonsterCount:	QP_DeleteQuestMonsterCount(dbquery, hstmt);				break;

	case QT_AuctionGetItemList:		QP_AuctionGetItemList(dbquery, hstmt);		break;
	case QT_AuctionRegisterItem:	QP_AuctionRegisterItem(dbquery, hstmt);		break;
	case QT_AuctionCancelRegister:	QP_AuctionCancelRegister(dbquery, hstmt);	break;
	case QT_AuctionBuyItem:			QP_AuctionBuyItem(dbquery, hstmt);			break;
	case QT_GeneralExec:			QP_GeneralExec(dbquery, hstmt);				break;

	case QT_CashInsertPremiumCard:	QP_CashInsertPremiumCard(dbquery, hstmt);	break;
	case QT_CashUpdatePremiumCard:	QP_CashUpdatePremiumCard(dbquery, hstmt);	break;
	case QT_CashDeletePremiumCard:	QP_CashDeletePremiumCard(dbquery, hstmt);	break;

	case QT_LoadHappyHourEvent:		QP_LoadHappyHourEvent(dbquery, hstmt);		break;
	case QT_UpdateHappyHourEventDate:	QP_UpdateHappyHourEventDate(dbquery, hstmt);	break;

	case QT_LoadItemEvent:			QP_LoadItemEvent(dbquery, hstmt);			break;
// 2008-12-19 by cmkwon, QT_UpdatePCBangList->DQT_UpdatePCBangList ���� - 
//	case QT_UpdatePCBangList:		QP_UpdatePCBangList(dbquery, hstmt);		break;
	case QT_Get_QuickSlot:			QP_Get_QuickSlot(dbquery, hstmt);			break;			// 2006-09-04 by dhjin
	case QT_Delete_QuickSlot:		QP_Delete_QuickSlot(dbquery, hstmt);		break;			// 2006-09-04 by dhjin
	case QT_Update_QuickSlot:		QP_Update_QuickSlot(dbquery, hstmt);		break;			// 2006-09-04 by dhjin
	
	// 2007-02-28 by dhjin, ��������Ʈ ���� �ֱ� ���� ����.
	case QT_LoadStrategyPointSummonInfo:					QP_LoadStrategyPointSummonInfo(dbquery, hstmt);						break;
	case QT_UpdateStrategyPointSummonInfoBySummon:			QP_UpdateStrategyPointSummonInfoBySummon(dbquery, hstmt);			break;
	case QT_UpdateStrategyPointSummonInfoBySummonTime:		QP_UpdateStrategyPointSummonInfoBySummonTime(dbquery, hstmt);		break;
	case QT_UpdateStrategyPointSummonInfoByNewSummonRange:	QP_UpdateStrategyPointSummonInfoByNewSummonRange(dbquery, hstmt);	break;
	case QT_LoadStrategyPointSummonRange:					QP_LoadStrategyPointSummonRange(dbquery, hstmt);					break;
	case QT_LoadStrategyPointNotSummonTime:					QP_LoadStrategyPointNotSummonTime(dbquery, hstmt);					break;
	case QT_LoadStrategyPointNotSummonTimeByAdminTool:		QP_LoadStrategyPointNotSummonTimeByAdminTool(dbquery, hstmt);					break;
		
	case QT_UpdateInfluenceWarData:	QP_UpdateInfluenceWarData(dbquery, hstmt);	break;
	case QT_UpdateOwnerOfConflictArea:	QP_UpdateOwnerOfConflictArea(dbquery, hstmt);	break;
	case QT_UpdateSubleader:		QP_UpdateSubleader(dbquery, hstmt);			break;

	//case QT_JamboreeInit:						QP_JamboreeInit(dbquery, hstmt);			break;	// 2007-04-10 by cmkwon
	//case QT_JamboreeEntrants:					QP_JamboreeEntrants(dbquery, hstmt);		break;	// 2007-04-10 by cmkwon
		
	case QT_UpdateWarPoint:						QP_UpdateWarPoint(dbquery, hstmt);			break;	// 2007-04-25 by dhjin
	case QT_UpdateArenaResult:					QP_UpdateArenaResult(dbquery, hstmt);		break;	// 2007-06-07 by dhjin
	case QT_UpdateArenaDisConnect:				QP_UpdateArenaDisConnect(dbquery, hstmt);		break;	// 2007-06-07 by dhjin
	case QT_MF_Updata_CharacterArena:			QP_MF_Updata_CharacterArena(dbquery, hstmt);	break;	// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : AS���� ���� CharacterArena ���� ������Ʈ

	case QT_LoadTutorialComplete:				QP_LoadTutorialComplete(dbquery, hstmt);		break;	// 2007-07-06 by dhjin
	case QT_InsertTutorialComplete:				QP_InsertTutorialComplete(dbquery, hstmt);		break;	// 2007-07-06 by dhjin

	case QT_SetNPCPossessionOutPost:			QP_SetNPCPossessionOutPost(dbquery, hstmt);		break;	// 2007-08-21 by dhjin
	case QT_SetGuildPossessionOutPost:			QP_SetGuildPossessionOutPost(dbquery, hstmt);		break;	// 2007-08-21 by dhjin
	case QT_SetOutPostNextWarTime:				QP_SetOutPostNextWarTime(dbquery, hstmt);		break;	// 2007-08-21 by dhjin

	case QT_InitExpediencyFund:					QP_InitExpediencyFund(dbquery, hstmt);		break;	// 2007-08-22 by dhjin
	case QT_SetExpediencyFund:					QP_SetExpediencyFund(dbquery, hstmt);		break;	// 2007-08-22 by dhjin
	case QT_SetExpediencyFundPayBack:			QP_SetExpediencyFundPayBack(dbquery, hstmt);		break;	// 2007-08-22 by dhjin
	case QT_SetExpediencyFundRate:				QP_SetExpediencyFundRate(dbquery, hstmt);		break;	// 2007-08-22 by dhjin
	case QT_DeleteCityLeaderInfo:				QP_DeleteCityLeaderInfo(dbquery, hstmt);		break;	// 2007-08-22 by dhjin
	case QT_RegNotice:							QP_RegNotice(dbquery, hstmt);		break;	// 2007-08-22 by dhjin
	case QT_ModifyNotice:						QP_ModifyNotice(dbquery, hstmt);		break;	// 2007-08-22 by dhjin
	case QT_UpdateSecondaryPassword:			QP_UpdateSecondaryPassword(dbquery, hstmt);		break;	// 2007-09-12 by cmkwon

	case QT_LoadVoterList:						QP_LoadVoterList(dbquery, hstmt);		break;	// 2007-10-29 by dhjin
	case QT_LoadLeaderCandidate:				QP_LoadLeaderCandidate(dbquery, hstmt);		break;	// 2007-10-29 by dhjin
	case QT_SelectLeaderCandidateInfoByRealTimeVariable:	QP_SelectLeaderCandidateInfoByRealTimeVariable(dbquery, hstmt);		break;	// 2007-10-30 by dhjin
	case QT_InsertLeaderCandidate:				QP_InsertLeaderCandidate(dbquery, hstmt);		break;	// 2007-10-30 by dhjin
	case QT_UpdateLeaderDeleteCandidate:		QP_UpdateLeaderDeleteCandidate(dbquery, hstmt);		break;	// 2007-10-30 by dhjin
	case QT_UpdateLeaderPollCount:				QP_UpdateLeaderPollCount(dbquery, hstmt);		break;	// 2007-10-31 by dhjin
	case QT_InsertVoterList:					QP_InsertVoterList(dbquery, hstmt);		break;	// 2007-10-31 by dhjin
	case QT_CheckGiveTarget:					QP_CheckGiveTarget(dbquery, hstmt);		break;	// 2007-11-13 by cmkwon, �����ϱ� ��� �߰� -
	case QT_UpdatePilotFace:					QP_UpdatePilotFace(dbquery, hstmt);		break;	// 2007-11-21 by cmkwon, PilotFace ���� ī�� ���� - 
	
	case QT_InsertNotifyMsg:					QP_InsertNotifyMsg(dbquery, hstmt);		break;	// 2007-11-28 by cmkwon, �����ý��� ���� -
	case QT_GetNotifyMsg:						QP_GetNotifyMsg(dbquery, hstmt);		break;	// 2007-11-28 by cmkwon, �����ý��� ���� -
	case QT_DeleteNotifyMsg:					QP_DeleteNotifyMsg(dbquery, hstmt);		break;	// 2007-11-28 by cmkwon, �����ý��� ���� -

	case QT_GetGuildMark:						QP_GetGuildMark(dbquery, hstmt);		break;	// 2007-12-07 by dhjin, ���� ��ũ

	//////////////////////////////////////////////////////////////////////////
	// 2007-12-28 by dhjin, �Ʒ��� ���� - 
	case QT_ArenaUpdateCharacterInfo:			QP_ArenaUpdateCharacterInfo(dbquery, hstmt);		break;	
	case QT_ArenaGetCharacter:					QP_ArenaGetCharacter(dbquery, hstmt);		break;	
	case QT_ArenaCopyDBInfo:					QP_ArenaCopyDBInfo(dbquery, hstmt);			break;	
	case QT_ArenaStartGetCharacter:				QP_ArenaStartGetCharacter(dbquery, hstmt);		break;	
	/////////////////////////////////////////////////////////////
	// 2012-04-12 by jhseol, �Ʒ��� �߰����� - �Ʒ��� �÷��� ī��Ʈ Ȯ�� �Լ� ���� 
	case QT_ArenaPlayCount:						QP_ArenaPlayCount(dbquery, hstmt);		break;

	//////////////////////////////////////////////////////////////////////////
	// 2008-04-02 by dhjin,	����, ������ ����â ��ȹ�� -
	case QT_GetLogMSWarInfo:					QP_GetLogMSWarInfo(dbquery, hstmt);		break;
	case QT_GetLogSPWarInfo:					QP_GetLogSPWarInfo(dbquery, hstmt);		break;
#ifdef NEMERIAN_AUTO_ELECTION_AND_RESET
	case QT_GetWarVictoryDiff:					QP_GetWarVictoryDiff(dbquery, hstmt);		break;
#endif
	case QT_UpdateMSWarOptionType:				QP_UpdateMSWarOptionType(dbquery, hstmt);		break;
	case QT_InsertMSWarLog:						QP_InsertMSWarLog(dbquery, hstmt);				break;		// 2008-08-28 by dhjin, ���� ����, ����DB�� ���ܾ� Admintool�� �ʱ�ȭ�� �����ϴ�.
	case QT_InsertSPWarLog:						QP_InsertSPWarLog(dbquery, hstmt);				break;		// 2008-08-28 by dhjin, ���� ����, ����DB�� ���ܾ� Admintool�� �ʱ�ȭ�� �����ϴ�.

	// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - 
	case QT_UpdateDBServerGroup:				QP_UpdateDBServerGroup(dbquery, hstmt);			break;		// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - 
	case QT_CheckConnectableAccount:			QP_CheckConnectableAccount(dbquery, hstmt);		break;		// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - 

	case QT_GetUserInfo:						QP_GetUserInfo(dbquery, hstmt);		break;						// 2008-06-23 by dhjin, EP3 ���������ɼ� - �ٸ� ���� ���� ��û

// 2008-12-01 by cmkwon, �������� �����带 ����� ó���ϴ� �ý��� ���� - DQT_DailyJob �� ���� ��.
//	//////////////////////////////////////////////////////////////////////////
//	// 2008-08-19 by dhjin, MySQL���� ������ MySQL���� �������� �ʴ� MSSQL�� Job���� �۾��� ���⼭ ó���Ѵ�.		
//	case QT_DailyJob:							QP_DailyJob(dbquery, hstmt);		break;

	// 2008-11-04 by dhjin, ��Ű�ӽ�
	case QT_UpdateLuckyItemDropCount:			QP_UpdateLuckyItemDropCount(dbquery, hstmt);		break;
	case QT_UpdateLuckyItemStarttime:			QP_UpdateLuckyItemStarttime(dbquery, hstmt);		break;

	//////////////////////////////////////////////////////////////////////////
	// 2009-01-12 by dhjin, ���� ����
	case QT_UpdateStartDeclarationOfWar:		QP_UpdateStartDeclarationOfWar(dbquery, hstmt);		break;
	case QT_UpdateEndDeclarationOfWar:			QP_UpdateEndDeclarationOfWar(dbquery, hstmt);		break;
	case QT_UpdateMSWarStartTime:				QP_UpdateMSWarStartTime(dbquery, hstmt);			break;
	
	case QT_UpdateNickName:						QP_UpdateNickName(dbquery, hstmt);			break;		// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 
	case QT_GetSelfRanking:						QP_GetSelfRanking(dbquery, hstmt);			break;		// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 

	case QT_ChangeItemWithItemMatching:			QP_ChangeItemWithItemMatching(dbquery, hstmt);		break;		// 2009-03-31 by cmkwon, �����ʱ�ȭ �ý��� ���� - 

	case QT_ChangeStartCityMapIndex:			QP_ChangeStartCityMapIndex(dbquery, hstmt);			break;		// 2009-10-12 by cmkwon, ������ī ���� ��� ���� - 
	case QT_ChangeAddedInventoryCount:			QP_ChangeAddedInventoryCount(dbquery, hstmt);		break;		// 2009-11-02 by cmkwon, ĳ��(�κ�/â�� Ȯ��) ������ �߰� ���� - 

	case QT_LoadInfinityImpute:					QP_LoadInfinityImpute(dbquery, hstmt);			break;		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 	���� �ͼ� ���� ��������
	case QT_InsertInfinityImpute:				QP_InsertInfinityImpute(dbquery, hstmt);		break;		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 	���� �ͼ� ���� �߰�
	case QT_UpdateInfinityImpute:				QP_UpdateInfinityImpute(dbquery, hstmt);		break;		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 	���� �Ϸ�
	case QT_ResetInfinityImpute:				QP_ResetInfinityImpute(dbquery, hstmt);			break;		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 	�ش� ���� �������� ���� ����
	case QT_ArenaCopyInfinityDBInfo:			QP_ArenaCopyInfinityDBInfo(dbquery, hstmt);		break;		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �Ʒ���DB�� �����ϱ�
	case QT_CharacterSaveDataInfinityFin:		QP_CharacterSaveDataInfinityFin(dbquery, hstmt);	break;		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� �� MainSvr�� ���� �����ϸ鼭 ������ �ɸ��� ���� ����
	case QT_InfinityFinUpdateItem:				QP_InfinityFinUpdateItem(dbquery, hstmt);			break;		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� �� MainSvr�� ���� �����ϸ鼭 ������ ������ ���� ����
	case QT_InfinityFinInsertItem:				QP_InfinityFinInsertItem(dbquery, hstmt);			break;		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� �� MainSvr�� ���� �����ϸ鼭 ������ ������ �߰�
	case QT_InfinityInsertLog:					QP_InfinityInsertLog(dbquery, hstmt);			break;		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� �α�
	case QT_CharacterSaveDataInfinityFinByDisconnect:	QP_CharacterSaveDataInfinityFinByDisconnect(dbquery, hstmt);	break;		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� ���� �� MainSvr�� ���� �����ϸ鼭 ������ �ɸ��� ���� ����
	case QT_InfinityFinUpdateItemByDisconnect:			QP_InfinityFinUpdateItemByDisconnect(dbquery, hstmt);			break;				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� ���� �� MainSvr�� ���� �����ϸ鼭 ������ ������ ���� ����
	case QT_InfinityFinInsertItemByDisconnect:			QP_InfinityFinInsertItemByDisconnect(dbquery, hstmt);			break;				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� ���� �� MainSvr�� ���� �����ϸ鼭 ������ ������ �߰���
	case QT_ResetInfinityImputeByServerStart:			QP_ResetInfinityImputeByServerStart(dbquery, hstmt);			break;				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���۽� ���� ����
	case QT_InfinityComBackPostWork:					QP_InfinityComBackPostWork(dbquery, hstmt);						break;				// 2012-01-16 by hskim, ��� - ȭ��

// 2010-04-09 by cmkwon, ����2�� �߰� ����(�ܰ躰 ���� �߰�) - 
//	case QT_LoadTenderInfo:						QP_LoadTenderInfo(dbquery, hstmt);			break;				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - Tender

	case QT_CashLoadPremiumCard:				QP_CashLoadPremiumCard(dbquery, hstmt);			break;				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �����̾� ���� ��������
	case QT_LoadInfinityShopInfo:				QP_LoadInfinityShopInfo(dbquery, hstmt);			break;				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ����
	case QT_UpdateItemCoolingTimeStamp:			QP_UpdateItemCoolingTimeStamp(dbquery, hstmt);			break;				// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
	case QT_InfinityUpdateUserMapInfo:			QP_InfinityUpdateUserMapInfo(dbquery, hstmt);			break;		// 2010-04-06 by cmkwon, ����2�� �߰� ���� - 
	case QT_LoadBurningMap:						QP_LoadBurningMap(dbquery, hstmt);						break;		// 2010-08-05 by dhjin, ���׸� -
	case QT_Log_UserGetTenderItem:				QP_Log_UserGetTenderItem(dbquery, hstmt);				break;		// 2010-06-25 by shcho, ���Ǵ�Ƽ ���÷α� ��� - ���� ������ ���� DB����

	/////////////////////////////////////////////////////////////
	// start 2011-08-22 by hskim, ��Ʈ�� �ý��� 2�� - ��� ����

	case QT_PetUpdateInfinityFin:				QP_PetUpdateInfinityFin(dbquery, hstmt);		break;
	case QT_PetSetName:							QP_PetSetName(dbquery, hstmt);					break;
	case QT_PetSetExpRatio:						OP_PetSetExpRatio(dbquery, hstmt);				break;
	case QT_PetSetLevel:						QP_PetSetLevel(dbquery, hstmt);					break;
	case QT_PetSetExp:							OP_PetSetExp(dbquery, hstmt);					break;

	case QT_PetSetSocket:						OP_PetSetSocket(dbquery, hstmt);				break;
	case QT_PetSetKitSlot:						OP_PetSetKitSlot(dbquery, hstmt);				break;
	case QT_PetSetAutoSkillSlot:				OP_PetSetAutoSkillSlot(dbquery, hstmt);			break;
	case QT_PetChangeSocketOwner:				OP_PetChangeSocketOwner(dbquery, hstmt);		break;				// 2012-01-30 by hskim, ��Ʈ�� �ý��� 2�� - ���� â�� �̵�
	case QT_StoreGetItemOne:					OP_StoreGetItemOne(dbquery, hstmt);				break;				// 2012-01-30 by hskim, ��Ʈ�� �ý��� 2�� - ���� â�� �̵�

	case QT_GetEnchantCount:					QP_GetEnchantCount(dbquery, hstmt);				break;				// 2012-08-27 by hskim, ��Ʈ�� ��þƮ �� ���� �߰� ���� ó�� (�ڵ���� �������� �߰ߵ��� �ʾ����� ������ ��쿡�� �α׸� �����) (����ȭ DB ������ �õ��ؼ� ���� ���ϰ� ���� : �ʿ���ٰ� �ǴܵǸ� ���� ����)

#if S_BONUSEXPSYSTEM_RENEWAL
	case QT_GetAddExpRestExperienceCount:		QP_GetAddExpRestExperienceCount(dbquery, hstmt);				break;				// 2012-10-07 by hskim, �޽� ����ġ
	case QT_SetAddExpRestExperienceCount:		QP_SetAddExpRestExperienceCount(dbquery, hstmt);				break;				// 2012-10-07 by hskim, �޽� ����ġ
#endif // S_BONUSEXPSYSTEM_RENEWAL

//#if S_BONUS_KILL_SYSTEM_RENEWAL
//	case QT_GetAddKillRestKillCount:			QP_GetAddKillRestKillCount(dbquery, hstmt);		break;				// 2015-06-11 Future, Kill Count System
//	case QT_SetAddKillrestKillCount:			QP_SetAddKillRestKillCount(dbquery, hstmt);		break;				// 2015-06-11 Future, Kill Count System
//#endif // S_BONUS_KILL_SYSTEM_RENEWAL


	case QT_GetFixedTerm:						QP_GetFixedTerm(dbquery, hstmt);							break;				// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)
	case QT_InsertFixedTerm:					QP_InsertFixedTerm(dbquery, hstmt);							break;				// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)
	case QT_DeleteFixedTerm:					QP_DeleteFixedTerm(dbquery, hstmt);							break;				// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)

	// end 2011-08-22 by hskim, ��Ʈ�� �ý��� 2�� - ��� ����
	/////////////////////////////////////////////////////////////

	case QT_UpgradePet:							OP_UpgradPet(dbquery, hstmt);								break;				// 2015-06-22 Future, upgrading Pets with sockets

	case QT_ArenaCharacterReset:				QP_ArenaCharacterReset(dbquery, hstmt);				break;	// 2012-10-21 by jhseol, �Ʒ��� ���׼��� - �Ʒ��� ����� �ɸ��� ���� ����.

	//////////////////////////////////////////////////////////////////////////////
	// 2012-11-13 by jhseol, ���� �ý��� ������ - ������
	case QT_LoadRenewalStrategyPointSummonInfo:	QP_LoadRenewalStrategyPointSummonInfo(dbquery, hstmt);		break;
	case QT_UpdateStrategyPointSummonInfo:		QP_UpdateStrategyPointSummonInfo(dbquery, hstmt);			break;
	// end 2012-11-13 by jhseol, ���� �ý��� ������ - ������

	case QT_ArenaCharacterReload:				QP_ArenaCharacterReload(dbquery, hstmt);			break;	// 2012-12-18 by jhseol, �Ʒ��� ĳ���� ���� �� ĳ���� ���� �ٽ� �ε��ϱ�.

	case QT_LoadCashBuyDate:					QP_LoadCashBuyDate(dbquery, hstmt);			break;	// 2013-03-29 by jhseol, ������ �̺�Ʈ - �ſ� ù ����� ��������
	case QT_InsertCashBuyDate:					QP_InsertCashBuyDate(dbquery, hstmt);			break;	// 2013-03-29 by jhseol, ������ �̺�Ʈ - �ſ� ù ����� ��������

	case QT_GetShapeStatLevel:					QP_GetShapeStatLevel(dbquery, hstmt);							break;		// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - QP_GetShapeStatLevel ȣ��
	case QT_InsertShapeStatLevel:				QP_InsertShapeStatLevel(dbquery, hstmt);						break;		// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - QP_InsertShapeStatLevel ȣ��

	case QT_LoadMonthlyArmorEvent:				QP_LoadMonthlyArmorEvent(dbquery, hstmt);						break;		// 2013-04-18 by jhseol,bckim �̴��� �Ƹ� - �̴��� �Ƹ� �̺�Ʈ �ε�
	case QT_UpdateInfluenceConsecutiveVictorites:	QP_UpdateInfluenceConsecutiveVictorites(dbquery, hstmt);	break;		// 2013-05-09 by hskim, ���� ����Ʈ ����

	case QT_GetTemporarySystemInfomation:		QP_GetTemporarySystemInfomation(dbquery, hstmt);				break;		// 2013-05-09 by hskim, ���� ����Ʈ ����
	case QT_GetStoreExtension:					QP_GetStoreExtension(dbquery, hstmt);							break;		// 2013-05-09 by hskim, ���� ����Ʈ ����
	case QT_InsertStoreExtension:				QP_InsertStoreExtension(dbquery, hstmt);						break;		// 2013-05-09 by hskim, ���� ����Ʈ ����

	case QT_CollectionArmorListLoad:			QP_CollectionArmorListLoad(dbquery, hstmt);							break;		// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - �Ƹ� �÷��� �ε�
	case QT_CollectionArmorUpdate:				QP_CollectionArmorUpdate(dbquery, hstmt);							break;		// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - �Ƹ� �÷��� ����
	case QT_AccountInflChange:					QP_AccountInflChange(dbquery, hstmt);		break;		// 2013-07-26 by jhseol, Ÿ ���� ���º���

#ifdef S_IP_UNIQUE_CONNECTION
	case QT_GetCharacterMultipleIPStatus:		QP_GetCharacterMultipleIPStatus(dbquery, hstmt);		break;					// 2015-11-25 Future, Multiple IP Restriction System
#endif // S_IP_UNIQUE_CONNECTION

	case QT_NationTransfer:						QP_NationTransfer(dbquery, hstmt);				break;							// 2015-12-17 Future, Nation Change Card

	default:
		{
			// error: no such DB query type
			g_pGlobal->WriteSystemLogEX(TRUE, STRMSG_080904_0001, dbquery.enumQueryType, GetDBQueryTypeString(dbquery.enumQueryType));
			return FALSE;
		}
		break;
	}
//#ifdef _DEBUG
//	DBGOUT("  ProcessServerQuery(%s): %s\n", GetCharacterString(&FieldDBQuery.pFieldIOCPSocket->m_character, string()), GetDBQueryTypeString(FieldDBQuery.enumQueryType));
//#endif

	return TRUE;
}



///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumFieldDBManager::GetAttachedItems(CHARACTER_RENDER_INFO *io_pRenderInfo, SQLHSTMT i_hstmt, UID32_t i_characterUID)
/// \brief		
/// \author		cmkwon
/// \date		2006-01-23 ~ 2006-01-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumFieldDBManager::GetAttachedItems(CHARACTER_RENDER_INFO *io_pRenderInfo, SQLHSTMT i_hstmt, UID32_t i_characterUID)
{
	// 2006-01-23 by cmkwon, �ʱ�ȭ
	util::zero(io_pRenderInfo, sizeof(CHARACTER_RENDER_INFO));

	SQLINTEGER cb1, cb2, cb3, cb4;
	int	OutItemWindIdx = 0;
	int OutItemNum = 0;
	int OutColorCode	= 0;
	int OutShapeItemNum	= 0;	// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
	/**********************************************************************
	-- !!!!
	-- Name: dbo.atum_GetAttachedItems
	-- Desc:
	--			2005-12-08 by cmkwon, �ƸӸ� ���� colorcode�� �����´�.
	--			2007-01-19, #define ITEM_IN_CHARACTER				0	// ĳ�����κ�
	--						#define ITEM_IN_STORE					1	// â��
	--						#define ITEM_IN_GUILD_STORE             2   // 2006-09-14 by dhjin, ���� â�� 
	--
	--						#define POS_PROW						((BYTE)0)	// ���̴�(���� ���)	--> ���� 2��
	--						#define POS_PROWIN						((BYTE)1)	// ��ǻ��(�߾� ����)	--> ���� 2��
	--						#define POS_PROWOUT						((BYTE)2)	// 1�� ����(���� ����)	--> ���� 1��
	--						#define POS_WINGIN						((BYTE)3)	// 2006-07-20 by cmkwon, ����Ʈ ������ (�߾� ����)
	--						#define POS_WINGOUT						((BYTE)4)	// 2�� ����(���� ����)	--> ���� 1��
	--						#define POS_CENTER						((BYTE)5)	// �Ƹ�(�߾� ���)	--> ���� 4��
	--						#define POS_REAR						((BYTE)6)	// ����(�Ĺ� ���)	--> ���� 3��
	--						#define POS_ATTACHMENT					((BYTE)7)	// 2006-03-30 by cmkwon, ������ �Ǽ��縮 - ������(�Ĺ� ����-������ũ|�����̳ʰ迭) --> ���� 3��
	--						#define POS_PET							((BYTE)8)	// 2006-03-30 by cmkwon, �ð����� �Ǽ��縮(�Ĺ� ����)	--> ���� 4��
	--		-- // 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - ShapeItemNum �ʵ� �߰�, ��� �ʵ� 1�� �߰�
	--
	--		-- // 2010-06-15 by shcho&hslee ��ý��� - ���� ����.	#define POS_PET ->> #define POS_ACCESSORY
							#define POS_PET							((BYTE)9)	// 2010-06-15 by shcho&hslee ��ý��� - ��(��Ʈ��)
	-- ====
	**********************************************************************/
	SQLBindParameter(i_hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_characterUID, 0, NULL);
	RETCODE ret = SQLExecDirect(i_hstmt, PROCEDURE_080822_0031, SQL_NTS);
	if(ret == SQL_NO_DATA)
	{
		return TRUE;
	}
	else if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		return FALSE;
	}

	SQLBindCol(i_hstmt, 1, SQL_C_LONG, &OutItemWindIdx, 0, &cb1);
	SQLBindCol(i_hstmt, 2, SQL_C_LONG, &OutItemNum, 0, &cb2);
	SQLBindCol(i_hstmt, 3, SQL_C_LONG, &OutColorCode, 0, &cb3);
	SQLBindCol(i_hstmt, 4, SQL_C_LONG, &OutShapeItemNum, 0, &cb4);	// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - ShapeItemNum �ʵ� �߰�

	// reset
	OutItemWindIdx		= 0;
	OutItemNum			= 0;
	while ( (ret = SQLFetch(i_hstmt)) != SQL_NO_DATA)
	{
// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - �Ʒ��� ���� ���� ��.
// 		switch(OutItemWindIdx)
// 		{
// 		case POS_PROW:			io_pRenderInfo->RI_Prow			= OutItemNum;		break;
// 		case POS_PROWIN:		io_pRenderInfo->RI_ProwIn		= OutItemNum;		break;
// 		case POS_PROWOUT:		io_pRenderInfo->RI_ProwOut		= OutItemNum;		break;
// 		case POS_WINGIN:		io_pRenderInfo->RI_WingIn		= OutItemNum;		break;		// 2006-07-20 by cmkwon, ����Ʈ ������ (�߾� ����)
// 		case POS_WINGOUT:		io_pRenderInfo->RI_WingOut		= OutItemNum;		break;
// 		case POS_CENTER:
// 			{
// 				io_pRenderInfo->RI_Center			= OutItemNum;
// 				io_pRenderInfo->RI_ArmorColorCode	= OutColorCode;		// �Ƹ��� ���������� �����ؾ��Ѵ�.
// 			}
// 			break;
// 		case POS_REAR:			io_pRenderInfo->RI_Rear			= OutItemNum;		break;
// 		case POS_ATTACHMENT:	io_pRenderInfo->RI_Attachment	= OutItemNum;		break;		// 2006-03-30 by cmkwon, ������ �Ǽ��縮 - ������
// 		case POS_PET:			io_pRenderInfo->RI_Pet			= OutItemNum;		break;		// 2006-03-30 by cmkwon, �ð����� �Ǽ��縮
// 		}
		///////////////////////////////////////////////////////////////////////////////
		// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
		// 2009-10-22 by cmkwon, �ð� �������� ������ �ý��� ���� - 
		//if(POS_INVALID_POSITION > OutItemWindIdx)
		if(IS_ATTACHABLE_POSITION(OutItemWindIdx))
		{
			if(FALSE == io_pRenderInfo->SetRenderInfoWithPOS(OutItemWindIdx, OutItemNum, OutShapeItemNum, OutColorCode))
			{
				g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::GetAttachedItems# attached item error !! ItemWindIdx(%d) ItemNum(%d) ShapeItemNum(%d) EffectItemNum(%d) \r\n", OutItemWindIdx, OutItemNum, OutShapeItemNum, OutColorCode);
			}
		}

		// reset
		OutItemWindIdx	= 0;
		OutItemNum		= 0;
		OutShapeItemNum	= 0;
		OutColorCode	= 0;
	}
	SQLFreeStmt(i_hstmt, SQL_CLOSE);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumFieldDBManager::LoadInfluenceWarData(BYTE *o_pOwnerInflOfConflictArea, CODBCStatement *i_pODBCStmt, CInflWarManager *i_pInflWarManager)
/// \brief		
///				// 2006-07-12 by cmkwon, BYTE *o_pOwnerInflOfConflictArea �߰���
/// \author		cmkwon
/// \date		2006-04-14 ~ 2006-04-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumFieldDBManager::LoadInfluenceWarData(BYTE *o_pOwnerInflOfConflictArea, CODBCStatement *i_pODBCStmt, CInflWarManager *i_pInflWarManager)
{
	*o_pOwnerInflOfConflictArea		= INFLUENCE_TYPE_UNKNOWN;		// 2006-07-12 by cmkwon

	RETCODE					ret;
	SQLINTEGER				arrCB[14] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};	// 2013-05-09 by hskim, ���� ����Ʈ ���� - // 2008-04-10 by dhjin, ���� ���� ǥ�� ��ȹ�� - 
	SINFLUENCE_WAR_DATA		tmWarData;
	UID32_t					VCNLeaderCharUID	= 0;
	UID32_t					ANILeaderCharUID	= 0;
	UID32_t					VCNSub1LeaderCharUID	= 0;		// 2006-12-08 by dhjin
	UID32_t					ANISub1LeaderCharUID	= 0;		// 2006-12-08 by dhjin
	UID32_t					VCNSub2LeaderCharUID	= 0;		// 2006-12-08 by dhjin
	UID32_t					ANISub2LeaderCharUID	= 0;		// 2006-12-08 by dhjin
	BOOL					bVCNLoading	= FALSE;
	BOOL					bANILoading = FALSE;
	BYTE					byOwnerOfConflictArea = FALSE;

//	char szQuery[1024];
// 2006-05-03 by cmkwon	wsprintf(szQuery, "SELECT * FROM td_InfluenceWarData WHERE MGameServerID = %d", g_pFieldGlobal->GetMGameServerID());
	// 2007-12-03 by dhjin, ������, �������� �̸��� �������� ����.

// 2008-07-02 by cmkwon, MySQL ���� ���� - 
//	wsprintf(szQuery, "SELECT wd.InfluenceType, wd.MGameServerID, wd.WartimeStage, wd.ContributionPoint, wd.InflLeaderCharacterUID, c.CharacterName, wd.OwnerOfConflictArea, wd.InflSub1LeaderCharacterUID, c1.CharacterName, wd.InflSub2LeaderCharacterUID, c2.CharacterName, wd.MSWarOptionType \
//						FROM td_InfluenceWarData wd  LEFT OUTER JOIN td_character c \
//							ON wd.InflLeaderCharacterUID = c.uniquenumber	\
//							LEFT OUTER JOIN td_character c1	\
//							ON wd.InflSub1LeaderCharacterUID = c1.uniquenumber \
//							LEFT OUTER JOIN td_character c2 \
//							ON wd.InflSub2LeaderCharacterUID = c2.uniquenumber");	// 2008-04-10 by dhjin, ���� ���� ǥ�� ��ȹ�� - 

// 2008-08-27 by cmkwon, �������� Procedure�� ���� - 
//	wsprintf(szQuery, QUERY_080702_0075);
//	BOOL bRet = i_pODBCStmt->ExecuteQuery(szQuery);
	BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0075));
	if (!bRet)
	{
		// error
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}
	// Bind Columns
	int serverId;
	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_UTINYINT, &tmWarData.InfluenceType, 0,			&arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_LONG, &serverId,					 0,				&arrCB[2]); // won't use this
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_UTINYINT, &tmWarData.WartimeStage, 0,				&arrCB[3]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_LONG, &tmWarData.ContributionPoint, 0,			&arrCB[4]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_ULONG, &tmWarData.InflLeaderCharacterUID, 0,		&arrCB[5]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 6, SQL_C_CHAR, tmWarData.InflLeaderCharacterName, SIZE_MAX_CHARACTER_NAME,		&arrCB[6]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 7, SQL_C_UTINYINT, &byOwnerOfConflictArea, 0,				&arrCB[7]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 8, SQL_C_ULONG, &tmWarData.InflSub1LeaderCharacterUID, 0,	&arrCB[8]);		// 2006-12-08 by dhjin
	SQLBindCol(i_pODBCStmt->m_hstmt, 9, SQL_C_CHAR, tmWarData.InflSubLeader1CharacterName, SIZE_MAX_CHARACTER_NAME,		&arrCB[9]);	// 2007-12-03 by dhjin
	SQLBindCol(i_pODBCStmt->m_hstmt, 10, SQL_C_ULONG, &tmWarData.InflSub2LeaderCharacterUID, 0,	&arrCB[10]);		// 2006-12-08 by dhjin
	SQLBindCol(i_pODBCStmt->m_hstmt, 11, SQL_C_CHAR, tmWarData.InflSubLeader2CharacterName, SIZE_MAX_CHARACTER_NAME,		&arrCB[11]);	// 2007-12-03 by dhjin
	SQLBindCol(i_pODBCStmt->m_hstmt, 12, SQL_C_USHORT, &tmWarData.MSWarOptionType, 0,	&arrCB[12]);	// 2008-04-10 by dhjin, ���� ���� ǥ�� ��ȹ�� - 
	SQLBindCol(i_pODBCStmt->m_hstmt, 13, SQL_C_SHORT, &tmWarData.ConsecutiveVictories, 0,	&arrCB[13]);		// 2013-05-09 by hskim, ���� ����Ʈ ����

	util::zero(&tmWarData, sizeof(tmWarData));
	while ( (ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		if(INFLUENCE_TYPE_VCN == tmWarData.InfluenceType)
		{
			VCNLeaderCharUID		= tmWarData.InflLeaderCharacterUID;
			VCNSub1LeaderCharUID	= tmWarData.InflSub1LeaderCharacterUID;			// 2006-12-08 by dhjin
			VCNSub2LeaderCharUID	= tmWarData.InflSub2LeaderCharacterUID;			// 2006-12-08 by dhjin

			bVCNLoading		= TRUE;
			i_pInflWarManager->InitInflWarData(&tmWarData);
			if(byOwnerOfConflictArea)
			{// 2006-07-12 by cmkwon, ���������� ���������� ����
				*o_pOwnerInflOfConflictArea	= INFLUENCE_TYPE_VCN;
			}
			
		}
		else if(INFLUENCE_TYPE_ANI == tmWarData.InfluenceType)
		{
			ANILeaderCharUID		= tmWarData.InflLeaderCharacterUID;
			ANISub1LeaderCharUID	= tmWarData.InflSub1LeaderCharacterUID;			// 2006-12-08 by dhjin
			ANISub2LeaderCharUID	= tmWarData.InflSub2LeaderCharacterUID;			// 2006-12-08 by dhjin

			bANILoading		= TRUE;
			i_pInflWarManager->InitInflWarData(&tmWarData);
			if(byOwnerOfConflictArea)
			{// 2006-07-12 by cmkwon, ���������� ���������� ����
				*o_pOwnerInflOfConflictArea	= INFLUENCE_TYPE_ANI;
			}
			
		}
		util::zero(&tmWarData, sizeof(tmWarData));
		byOwnerOfConflictArea = FALSE;			// 2006-07-12 by cmkwon
	}
	i_pODBCStmt->FreeStatement();

	if(FALSE == bVCNLoading)
	{
		util::zero(&tmWarData, sizeof(tmWarData));
		tmWarData.InfluenceType				= INFLUENCE_TYPE_VCN;
		if(FALSE == InsertInfluenceWarData(i_pODBCStmt, &tmWarData))
		{
			return FALSE;
		}
		i_pInflWarManager->InitInflWarData(&tmWarData);
	}
	if(FALSE == bANILoading)
	{
		util::zero(&tmWarData, sizeof(tmWarData));
		tmWarData.InfluenceType				= INFLUENCE_TYPE_ANI;
		if(FALSE == InsertInfluenceWarData(i_pODBCStmt, &tmWarData))
		{
			return FALSE;
		}
		i_pInflWarManager->InitInflWarData(&tmWarData);
	}

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumFieldDBManager::InsertInfluenceWarData(CODBCStatement *i_pODBCStmt, SINFLUENCE_WAR_DATA *i_pSINFLUENCE_WAR_DATA)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-14 ~ 2006-04-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumFieldDBManager::InsertInfluenceWarData(CODBCStatement *i_pODBCStmt, SINFLUENCE_WAR_DATA *i_pSINFLUENCE_WAR_DATA)
{
	// We don't even have this proceedure

	return false;

	//SQLHSTMT hstmt = i_pODBCStmt->GetSTMTHandle();
	//SQLINTEGER arrCB2[5] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	//SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &(i_pSINFLUENCE_WAR_DATA->InfluenceType), 0,		&arrCB2[1]);
	//SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(i_pSINFLUENCE_WAR_DATA->MGameServerID), 0,			&arrCB2[2]);
	//SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &(i_pSINFLUENCE_WAR_DATA->WartimeStage), 0,		&arrCB2[3]);
	//SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(i_pSINFLUENCE_WAR_DATA->ContributionPoint), 0,		&arrCB2[4]);
	//BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0076));
	//if (!bRet)
	//{// error
	//	i_pODBCStmt->FreeStatement();
	//	return FALSE;
	//}
	//i_pODBCStmt->FreeStatement();
	//return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumFieldDBManager::LoadDeclarationOfWarInfo(CODBCStatement *i_pODBCStmt, CInflWarManager *i_pInflWarManager)
/// \brief		���� ����
/// \author		dhjin
/// \date		2009-01-14 ~ 2009-01-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumFieldDBManager::LoadDeclarationOfWarInfo(CODBCStatement *i_pODBCStmt, CInflWarManager *i_pInflWarManager)
{
	// 2009-03-05 by cmkwon, �������� �Ʒ��� ���� ó�� - 
	if(g_pFieldGlobal->IsArenaServer())
	{
		mtvectSDECLARATION_OF_WAR tmvectDeclarationOfWar;
		SDECLARATION_OF_WAR	tmDeclarationOfWar;
		util::zero(&tmDeclarationOfWar, sizeof(tmDeclarationOfWar));	// 2009-04-13 by cmkwon, �ʱ�ȭ �߰���.
		tmDeclarationOfWar.Influence		= INFLUENCE_TYPE_VCN;
		tmDeclarationOfWar.MSWarStepStartTime.SetCurrentDateTime();
		tmDeclarationOfWar.MSWarStepEndTime = tmDeclarationOfWar.MSWarStepStartTime;
		tmDeclarationOfWar.MSWarStepEndTime.AddDateTime(1,0,0);		// 2009-03-05 by cmkwon, 1�� �ķ� ���� ����
		tmDeclarationOfWar.MSWarStartTime	= tmDeclarationOfWar.MSWarStepEndTime;
		tmDeclarationOfWar.MSWarStep		= 1;
		tmvectDeclarationOfWar.push_back(tmDeclarationOfWar);

		tmDeclarationOfWar.Influence		= INFLUENCE_TYPE_ANI;
		tmDeclarationOfWar.MSWarStepStartTime.SetCurrentDateTime();
		tmDeclarationOfWar.MSWarStepEndTime = tmDeclarationOfWar.MSWarStepStartTime;
		tmDeclarationOfWar.MSWarStepEndTime.AddDateTime(1,0,0);		// 2009-03-05 by cmkwon, 1�� �ķ� ���� ����
		tmDeclarationOfWar.MSWarStartTime	= tmDeclarationOfWar.MSWarStepEndTime;
		tmDeclarationOfWar.MSWarStep		= 1;
		tmvectDeclarationOfWar.push_back(tmDeclarationOfWar);


		SDECLARATION_OF_WAR_FORBID_TIME	tmDeclarationOfWarForbidTime;
		util::zero(&tmDeclarationOfWarForbidTime, sizeof(tmDeclarationOfWarForbidTime));

		if(!i_pInflWarManager->m_DeclarationOfWar.SetDBDeclarationOfWar(&tmvectDeclarationOfWar, &tmDeclarationOfWarForbidTime))
		{
			return FALSE;
		}

// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - ������ �¸� �� ���� �Ǵ� �̺�Ʈ ���� - ��ġ �̵� CInflWarData::SetCurrentStepInflWarData���� �����Ѵ�.
//		i_pInflWarManager->SetInflEventRate();			// 2009-03-31 by dhjin, ���� ���� - ������ �¸� �� ���� �Ǵ� �̺�Ʈ ����
		
		i_pInflWarManager->SetCurrentStepInflWarDataW();	// ���� �� �ܰ�� m_pCurInflWarInfo�� ���� �� ������ð��� �����Ѵ�.
		return TRUE;
	}

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Load_DeclarationOfWarInfo
	-- DESC				: ���� ����
	-- 2009-01-14 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Load_DeclarationOfWarInfo
	AS
		SELECT Influence, MSWarStep, NCP, MSNum, MSAppearanceMap, MSWarStepStartTime, MSWarStepEndTime, MSWarStartTime,
				MSWarEndTime, SelectCount, GiveUp, MSWarEndState
		FROM dbo.td_DeclarationOfWar
	GO
	************************************************************************/
	RETCODE	ret = SQLExecDirect(i_pODBCStmt->m_hstmt, PROCEDURE_090114_0284, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		i_pODBCStmt->ProcessLogMessagesForStmt(PROCEDURE_090114_0284);	// 2009-03-05 by cmkwon, �ý��� �α� �߰�
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}
	
	SQLINTEGER	arrCB[13] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
							, SQL_NTS,SQL_NTS,SQL_NTS};

	SDECLARATION_OF_WAR	tmDeclarationOfWar;
	util::zero(&tmDeclarationOfWar, sizeof(SDECLARATION_OF_WAR));
	mtvectSDECLARATION_OF_WAR tmvectDeclarationOfWar;
	tmvectDeclarationOfWar.clear();
	SQL_TIMESTAMP_STRUCT	MSWarStepStartTime;
	SQL_TIMESTAMP_STRUCT	MSWarStepEndTime;
	SQL_TIMESTAMP_STRUCT	MSWarStartTime;
	SQL_TIMESTAMP_STRUCT	MSWarEndTime;
	util::zero(&MSWarStepStartTime, sizeof(SQL_TIMESTAMP_STRUCT));
	util::zero(&MSWarStepEndTime, sizeof(SQL_TIMESTAMP_STRUCT));
	util::zero(&MSWarStartTime, sizeof(SQL_TIMESTAMP_STRUCT));
	util::zero(&MSWarEndTime, sizeof(SQL_TIMESTAMP_STRUCT));
	
	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_TINYINT,	&tmDeclarationOfWar.Influence, 0, &arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_TINYINT,	&tmDeclarationOfWar.MSWarStep, 0, &arrCB[2]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_ULONG,	&tmDeclarationOfWar.NCP, 0, &arrCB[3]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_ULONG,	&tmDeclarationOfWar.MSNum, 0, &arrCB[4]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_ULONG,	&tmDeclarationOfWar.MSAppearanceMap, 0, &arrCB[5]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 6, SQL_C_TIMESTAMP,&MSWarStepStartTime, 0, &arrCB[6]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 7, SQL_C_TIMESTAMP,&MSWarStepEndTime, 0, &arrCB[7]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 8, SQL_C_TIMESTAMP,&MSWarStartTime, 0, &arrCB[8]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 9, SQL_C_TIMESTAMP,&MSWarEndTime, 0, &arrCB[9]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 10, SQL_C_TINYINT,	&tmDeclarationOfWar.SelectCount, 0, &arrCB[10]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 11, SQL_C_BIT,		&tmDeclarationOfWar.GiveUp, 0, &arrCB[11]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 12, SQL_C_TINYINT,	&tmDeclarationOfWar.MSWarEndState, 0, &arrCB[12]);
	
	while ( (ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		tmDeclarationOfWar.MSWarStepStartTime	= MSWarStepStartTime;
		tmDeclarationOfWar.MSWarStepEndTime		= MSWarStepEndTime;				
		tmDeclarationOfWar.MSWarStartTime		= MSWarStartTime; 
		tmDeclarationOfWar.MSWarEndTime			= MSWarEndTime;
		tmvectDeclarationOfWar.push_back(tmDeclarationOfWar);
		util::zero(&MSWarStepStartTime, sizeof(SQL_TIMESTAMP_STRUCT));
		util::zero(&MSWarStepEndTime, sizeof(SQL_TIMESTAMP_STRUCT));
		util::zero(&MSWarStartTime, sizeof(SQL_TIMESTAMP_STRUCT));
		util::zero(&MSWarEndTime, sizeof(SQL_TIMESTAMP_STRUCT));
		util::zero(&tmDeclarationOfWar, sizeof(SDECLARATION_OF_WAR));
	}
	i_pODBCStmt->FreeStatement();	

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Load_DeclarationOfWarForbidTimeInfo
	-- DESC				: ���� ���� �����ð� ��������
	-- 2009-01-14 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Load_DeclarationOfWarForbidTimeInfo
	AS
		SELECT DayOfWeek, ForbidStartTime, ForbidEndTime
		FROM atum2_db_account.dbo.td_DeclarationOfWarForbidTime
	GO
	************************************************************************/
	ret = SQLExecDirect(i_pODBCStmt->m_hstmt, PROCEDURE_090114_0285, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		i_pODBCStmt->ProcessLogMessagesForStmt(PROCEDURE_090114_0285);	// 2009-03-05 by cmkwon, �ý��� �α� �߰�
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}
	
	SQLINTEGER	arrCB2[4] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};

	SDECLARATION_OF_WAR_FORBID_TIME	tmDeclarationOfWarForbidTime;
	util::zero(&tmDeclarationOfWarForbidTime, sizeof(SDECLARATION_OF_WAR_FORBID_TIME));
	SQL_TIMESTAMP_STRUCT	ForbidStartTime;
	SQL_TIMESTAMP_STRUCT	ForbidEndTime;

	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_ULONG,		&tmDeclarationOfWarForbidTime.DayOfWeek, 0, &arrCB2[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_TIMESTAMP,	&ForbidStartTime, 0, &arrCB2[2]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_TIMESTAMP,	&ForbidEndTime, 0, &arrCB2[3]);
	
	while ( (ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		tmDeclarationOfWarForbidTime.ForbidStartTime	= ForbidStartTime;
		tmDeclarationOfWarForbidTime.ForbidEndTime		= ForbidEndTime;				
	}
	i_pODBCStmt->FreeStatement();	

	if(!i_pInflWarManager->m_DeclarationOfWar.SetDBDeclarationOfWar(&tmvectDeclarationOfWar, &tmDeclarationOfWarForbidTime))
	{
		return FALSE;
	}
// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - ������ �¸� �� ���� �Ǵ� �̺�Ʈ ���� - ��ġ �̵� CInflWarData::SetCurrentStepInflWarData���� �����Ѵ�.
//	i_pInflWarManager->SetInflEventRate();			// 2009-03-31 by dhjin, ���� ���� - ������ �¸� �� ���� �Ǵ� �̺�Ʈ ����
	
	i_pInflWarManager->SetCurrentStepInflWarDataW();	// ���� �� �ܰ�� m_pCurInflWarInfo�� ���� �� ������ð��� �����Ѵ�.
	i_pInflWarManager->SetCompensationForDecalationOfWarWin(INFLUENCE_TYPE_VCN);	// 2009-07-02 by cmkwon, �������� ���� ���� ���� - ���� ����� ����
	i_pInflWarManager->SetCompensationForDecalationOfWarWin(INFLUENCE_TYPE_ANI);	// 2009-07-02 by cmkwon, �������� ���� ���� ���� - ���� ����� ����

	return TRUE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumFieldDBManager::DBLoadArenaInfo(CODBCStatement *i_pODBCStmt, CArenaManager *i_pArenaManager)
/// \brief		
/// \author		dhjin
/// \date		2007-04-17 ~ 2007-04-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumFieldDBManager::DBLoadArenaInfo(CODBCStatement *i_pODBCStmt, CArenaManager *i_pArenaManager)
{
	//////////////////////////////////////////////////////////////////////////////
	// start 2012-05-23 by jhseol, �Ʒ��� �߰����� - �ּ� ���� ���� �ʵ� �߰�
	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME: dbo.atum_LoadArenaInfo
	-- DESC			 : �Ʒ��� ���� �ε�
	-- 2007-04-17 by dhjin,
	--> 2012-05-23 by jhseol, �Ʒ��� WP �⺻���޷� �ʵ� �߰�
	--------------------------------------------------------------------------------
	ALTER PROCEDURE [dbo].[atum_LoadArenaInfo]
	AS
		SELECT ArenaMode, ReqMemberNum, PayInfluencePointWIN, PayInfluencePointLOSE, PayWarPointWIN, PayWarPointLOSE, DefaultPayWarPointWIN, DefaultPayWarPointLOSE
		FROM atum2_db_account.dbo.ti_ArenaInfo WITH (NOLOCK)
	GO
	************************************************************************/
	// end 2012-05-23 by jhseol, �Ʒ��� �߰����� - �ּ� ���� ���� �ʵ� �߰�
	//////////////////////////////////////////////////////////////////////////////
	RETCODE	ret = SQLExecDirect(i_pODBCStmt->m_hstmt, PROCEDURE_080822_0032, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}
	
	SQLINTEGER	arrCB[7] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};

	SDBARENA_INFO	tmDBArenaInfo;
	util::zero(&tmDBArenaInfo, sizeof(SDBARENA_INFO));
	vectorSDBARENA_INFO	tmvectDBArenaInfo;
	tmvectDBArenaInfo.clear();

	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_TINYINT, &tmDBArenaInfo.ArenaMode, 0, &arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_TINYINT, &tmDBArenaInfo.ReqMemberNum, 0, &arrCB[2]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_LONG, &tmDBArenaInfo.PayInfluencePointWIN, 0, &arrCB[3]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_LONG, &tmDBArenaInfo.PayInfluencePointLOSE, 0, &arrCB[4]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_LONG, &tmDBArenaInfo.PayWarPointWIN, 0, &arrCB[5]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 6, SQL_C_LONG, &tmDBArenaInfo.PayWarPointLOSE, 0, &arrCB[6]);
	//////////////////////////////////////////////////////////////////////////////
	// 2012-05-23 by jhseol, �Ʒ��� �߰����� - �ּ� ���� ���� �ʵ� �߰�
	SQLBindCol(i_pODBCStmt->m_hstmt, 7, SQL_C_LONG, &tmDBArenaInfo.DefaultPayWarPointWIN, 0, NULL);
	SQLBindCol(i_pODBCStmt->m_hstmt, 8, SQL_C_LONG, &tmDBArenaInfo.DefaultPayWarPointLOSE, 0, NULL);
	// end 2012-05-23 by jhseol, �Ʒ��� �߰����� - �ּ� ���� ���� �ʵ� �߰�
	while ( (ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		tmvectDBArenaInfo.push_back(tmDBArenaInfo);
		util::zero(&tmDBArenaInfo, sizeof(SDBARENA_INFO));
	}
	i_pODBCStmt->FreeStatement();	

	i_pArenaManager->InitDBArenaInfo(&tmvectDBArenaInfo);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumFieldDBManager::QP_LoadArenaMapInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		
/// \author		dhjin
/// \date		2007-04-17 ~ 2007-04-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumFieldDBManager::DBLoadArenaMapInfo(CODBCStatement *i_pODBCStmt, CArenaManager *i_pArenaManager)
{
	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME: dbo.atum_LoadArenaMapInfo
	-- DESC			 : ArenaMapInfo ������
	-- 2007-04-25 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_LoadArenaMapInfo
	AS
		SELECT ArenaMapIndex, ArenaMode, ReqMemberNum, PlayLimitedTime, WINCondition, 
				LvDSupplyItemNum1, LvDSupplyItemCount1, LvDSupplyItemNum2, LvDSupplyItemCount2, LvDSupplyItemNum3, LvDSupplyItemCount3
		FROM atum2_db_account.dbo.ti_ArenaMapInfo WITH (NOLOCK)
	GO
	************************************************************************/
	RETCODE	ret = SQLExecDirect(i_pODBCStmt->m_hstmt, PROCEDURE_080822_0033, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}
	
	// 2012-09-14 by jhseol, �Ʒ��� �߰����� part3 - �Ʒ��� ���� ������ �߰� (�Ʒ��� ���� ź�� ������ ī��)
	SQLINTEGER	arrCB[16] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS
		, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS };
	// end 2012-09-14 by jhseol, �Ʒ��� �߰����� part3 - �Ʒ��� ���� ������ �߰� (�Ʒ��� ���� ź�� ������ ī��)

	SDBARENA_MAPINFO	tmDBArenaMapInfo;
	util::zero(&tmDBArenaMapInfo, sizeof(SDBARENA_MAPINFO));
	vectorSDBARENA_MAPINFO	tmvectDBArenaMapInfo;
	tmvectDBArenaMapInfo.clear();

	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &tmDBArenaMapInfo.ArenaMapIndex, 0, &arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_TINYINT, &tmDBArenaMapInfo.ArenaMode, 0, &arrCB[2]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_TINYINT, &tmDBArenaMapInfo.ReqMemberNum, 0, &arrCB[3]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_TINYINT, &tmDBArenaMapInfo.PlayLimitedTime, 0, &arrCB[4]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_TINYINT, &tmDBArenaMapInfo.WINCondition, 0, &arrCB[5]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 6, SQL_C_LONG, &tmDBArenaMapInfo.LvDSupplyItemNum1, 0, &arrCB[6]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 7, SQL_C_LONG, &tmDBArenaMapInfo.LvDSupplyItemCount1, 0, &arrCB[7]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 8, SQL_C_LONG, &tmDBArenaMapInfo.LvDSupplyItemNum2, 0, &arrCB[8]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 9, SQL_C_LONG, &tmDBArenaMapInfo.LvDSupplyItemCount2, 0, &arrCB[9]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 10, SQL_C_LONG, &tmDBArenaMapInfo.LvDSupplyItemNum3, 0, &arrCB[10]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 11, SQL_C_LONG, &tmDBArenaMapInfo.LvDSupplyItemCount3, 0, &arrCB[11]);	
	// 2012-09-14 by jhseol, �Ʒ��� �߰����� part3 - �Ʒ��� ���� ������ �߰� (�Ʒ��� ���� ź�� ������ ī��)
#ifdef _ARENA_ADDITIONAL_SUPPLY_ITEMS
	SQLBindCol(i_pODBCStmt->m_hstmt, 12, SQL_C_LONG, &tmDBArenaMapInfo.LvDSupplyItemNum4, 0, &arrCB[12]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 13, SQL_C_LONG, &tmDBArenaMapInfo.LvDSupplyItemCount4, 0, &arrCB[13]);	
	SQLBindCol(i_pODBCStmt->m_hstmt, 14, SQL_C_LONG, &tmDBArenaMapInfo.LvDSupplyItemNum5, 0, &arrCB[14]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 15, SQL_C_LONG, &tmDBArenaMapInfo.LvDSupplyItemCount5, 0, &arrCB[15]);
#endif
	// end 2012-09-14 by jhseol, �Ʒ��� �߰����� part3 - �Ʒ��� ���� ������ �߰� (�Ʒ��� ���� ź�� ������ ī��)

	MapIndex_t	tmMapIndex = 0;			// 2007-04-30 by dhjin, �Ʒ��� �� �� ������ ����
	INT			TotalMapIndex = 0;		// 2007-04-30 by dhjin, �Ʒ��� �� �� ������ ����
	while ( (ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		tmvectDBArenaMapInfo.push_back(tmDBArenaMapInfo);
		if(tmMapIndex != tmDBArenaMapInfo.ArenaMapIndex)
		{
			tmMapIndex = tmDBArenaMapInfo.ArenaMapIndex;
			TotalMapIndex++;
		}
		util::zero(&tmDBArenaMapInfo, sizeof(SDBARENA_MAPINFO));
	}
	i_pODBCStmt->FreeStatement();		
	
	if(0 >= TotalMapIndex)
	{	// 2009-06-11 by cmkwon, �Ʒ��� �� ���� üũ �߰�
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::DBLoadArenaMapInfo# TotalArenaMapCount(%d) \r\n", TotalMapIndex);
		return FALSE;
	}

	i_pArenaManager->SetTotalMapCount(TotalMapIndex);
	i_pArenaManager->InitDBArenaMapInfo(&tmvectDBArenaMapInfo);

	return TRUE;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumFieldDBManager::DBLoadTutorialInfo(CODBCStatement *i_pODBCStmt, vectSTutorialInfo *i_pvectTutorialInfo)
/// \brief		Tutorial ���� ������
/// \author		dhjin
/// \date		2007-07-06 ~ 2007-07-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumFieldDBManager::DBLoadTutorialInfo(CODBCStatement *i_pODBCStmt, vectSTutorialInfo *i_pvectTutorialInfo)
{
	/************************************************************************
	CREATE PROCEDURE dbo.atum_LoadTutorialInfo
	AS
		SELECT TutorialUID, TutorialPayItem1, TutorialPayItemCount1, TutorialPayItem2, TutorialPayItemCount2, TutorialPayItem3, TutorialPayItemCount3
		FROM atum2_db_account.dbo.ti_TutorialInfo
	GO
	************************************************************************/
	RETCODE	ret = SQLExecDirect(i_pODBCStmt->m_hstmt, PROCEDURE_080822_0034, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}

	STUTORIAL_INFO		tmTutorial;
	util::zero(&tmTutorial, sizeof(STUTORIAL_INFO));
	SQLINTEGER	arrCB[8] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &tmTutorial.TutorialUID, 0, &arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_LONG, &tmTutorial.TutorialPayItem1, 0, &arrCB[2]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_LONG, &tmTutorial.TutorialPayItemCount1, 0, &arrCB[3]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_LONG, &tmTutorial.TutorialPayItem2, 0, &arrCB[4]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_LONG, &tmTutorial.TutorialPayItemCount2, 0, &arrCB[5]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 6, SQL_C_LONG, &tmTutorial.TutorialPayItem3, 0, &arrCB[6]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 7, SQL_C_LONG, &tmTutorial.TutorialPayItemCount3, 0, &arrCB[7]);

	while ( (ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		i_pvectTutorialInfo->push_back(tmTutorial);
		util::zero(&tmTutorial, sizeof(STUTORIAL_INFO));
	}
	
	i_pODBCStmt->FreeStatement();
	return TRUE;		
}

// 2012-12-20 by bckim, �����ʱ�ȭ�� ���â��������� ����忡�� �Ű����� ������ �α�
BOOL CAtumFieldDBManager::DBLoadInitializedGuildList(CODBCStatement *i_pODBCStmt)
{
	RETCODE	ret = SQLExecDirect(i_pODBCStmt->m_hstmt, PROCEDURE_121220_0001, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}

	MSG_FL_LOG_ITEM_INITIALIZED_GUILD_STORE		tmInitializedGuildList;
	util::zero(&tmInitializedGuildList, sizeof(MSG_FL_LOG_ITEM_INITIALIZED_GUILD_STORE));
	SQLINTEGER	arrCB[11] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &tmInitializedGuildList.AccountUniqueNumber, 0, &arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_CHAR, &tmInitializedGuildList.AccountName,SIZE_MAX_ACCOUNT_NAME, &arrCB[2]);
 	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_LONG, &tmInitializedGuildList.CharacterUniqueNumber, 0, &arrCB[3]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_CHAR, &tmInitializedGuildList.CharacterName, SIZE_MAX_CHARACTER_NAME, &arrCB[4]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_LONG, &tmInitializedGuildList.ItemUniqueNumber, 0, &arrCB[5]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 6, SQL_C_LONG, &tmInitializedGuildList.ItemNum, 0, &arrCB[6]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 7, SQL_C_LONG, &tmInitializedGuildList.Kind,0, &arrCB[7]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 8, SQL_C_LONG, &tmInitializedGuildList.CurrentCount, 0, &arrCB[8]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 9, SQL_C_LONG, &tmInitializedGuildList.GuildUniqueNumber,0, &arrCB[9]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 10, SQL_C_CHAR, &tmInitializedGuildList.GuildName,SIZE_MAX_GUILD_NAME, &arrCB[10]);
	
	while ( (ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		INIT_MSG_WITH_BUFFER(MSG_FL_LOG_ITEM_INITIALIZED_GUILD_STORE, T_FL_LOG_ITEM_INITIALIZED_GUILD_STORE, msgItemBuy, msgItemBuyBuf);
		
		msgItemBuy->AccountUniqueNumber		= tmInitializedGuildList.AccountUniqueNumber;
		util::strncpy(msgItemBuy->AccountName, tmInitializedGuildList.AccountName, SIZE_MAX_ACCOUNT_NAME);
		msgItemBuy->CharacterUniqueNumber	= tmInitializedGuildList.CharacterUniqueNumber;
		util::strncpy(msgItemBuy->CharacterName, tmInitializedGuildList.CharacterName, SIZE_MAX_CHARACTER_NAME);

		msgItemBuy->ItemUniqueNumber		= tmInitializedGuildList.ItemUniqueNumber;
		msgItemBuy->ItemNum					= tmInitializedGuildList.ItemNum;

		if( !IS_COUNTABLE_ITEM(tmInitializedGuildList.Kind))
		{
			tmInitializedGuildList.CurrentCount = 1;
		}
		msgItemBuy->CurrentCount			= tmInitializedGuildList.CurrentCount;
		
		msgItemBuy->GuildUniqueNumber		= tmInitializedGuildList.GuildUniqueNumber;
		util::strncpy(msgItemBuy->GuildName, tmInitializedGuildList.GuildName, SIZE_MAX_GUILD_NAME);

		g_pFieldGlobal->SendLogFieldServer2LogServer(msgItemBuyBuf, MSG_SIZE(MSG_FL_LOG_ITEM_INITIALIZED_GUILD_STORE));
		util::zero(&tmInitializedGuildList, sizeof(MSG_FL_LOG_ITEM_INITIALIZED_GUILD_STORE));		
	}

	i_pODBCStmt->FreeStatement();

	// ��� ���� 
	//#define PROCEDURE_121220_0002 (UCHAR*)"{call dbo.atum_Delete_LoadInitializedGuildList}"
	RETCODE ret2 = SQLExecDirect(i_pODBCStmt->m_hstmt, PROCEDURE_121220_0002, SQL_NTS);
	if(SQL_SUCCESS != ret2 && SQL_SUCCESS_WITH_INFO != ret2 && SQL_NO_DATA != ret2)
	{
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}

	i_pODBCStmt->FreeStatement();
	return TRUE;		
}
// 2012-12-20 by bckim, �����ʱ�ȭ�� ���â��������� ����忡�� �Ű����� ������ �α�. End


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumFieldDBManager::DBLoadOutPostInfo(CODBCStatement *i_pODBCStmt, COutPostManager *i_pOutPostManager)
/// \brief		�������� ���� ������
/// \author		dhjin
/// \date		2007-08-16 ~ 2007-08-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumFieldDBManager::DBLoadOutPostInfo(CODBCStatement *i_pODBCStmt, COutPostManager *i_pOutPostManager)
{
	/************************************************************************
	CREATE PROCEDURE dbo.atum_LoadOutPostInfo
	AS
		SELECT O.OutPostMapIndex, O.OutPostCityMapIndex, O.OutPostGetTime, O.OutPostNextWarTime
				, O.OutPostNextWarTimeSet, O.OutPostGetInfl, O.OutPostGuildUID, G.GuildName
		FROM td_OutPostInfo AS O LEFT JOIN td_Guild AS G 
		ON O.OutPostGuildUID = G.GuildUniqueNumber
	GO
	************************************************************************/
	RETCODE	ret = SQLExecDirect(i_pODBCStmt->m_hstmt, PROCEDURE_080822_0017, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}
	
	SOUTPOST_INFO		tmOutPostInfo;
	util::zero(&tmOutPostInfo, sizeof(SOUTPOST_INFO));
	SQL_TIMESTAMP_STRUCT	GetTime;
	SQL_TIMESTAMP_STRUCT	NextWarTime;
	vectSOutPostInfo	tmVectDBOutPostInfo;
	tmVectDBOutPostInfo.clear();

	SQLINTEGER	arrCB[9] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tmOutPostInfo.OutPostMapIndex, 0, &arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_ULONG, &tmOutPostInfo.OutPostCityMapIndex, 0, &arrCB[2]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_TIMESTAMP, &GetTime, 0, &arrCB[3]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_TIMESTAMP, &NextWarTime, 0, &arrCB[4]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_TINYINT, &tmOutPostInfo.OutPostNextWarTimeSet, 0, &arrCB[5]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 6, SQL_C_TINYINT, &tmOutPostInfo.OutPostInfluence, 0, &arrCB[6]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 7, SQL_C_ULONG, &tmOutPostInfo.OutPostGuildUID, 0, &arrCB[7]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 8, SQL_C_CHAR, &tmOutPostInfo.OutPostGuildName, SIZE_MAX_GUILD_NAME, &arrCB[8]);

	while ( (ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		tmOutPostInfo.OutPostGetTime		=	GetTime;
		tmOutPostInfo.OutPostNextWarTime	=	NextWarTime;
		tmVectDBOutPostInfo.push_back(tmOutPostInfo);
		util::zero(&tmOutPostInfo, sizeof(SOUTPOST_INFO));
	}
	i_pODBCStmt->FreeStatement();
	
	// 2007-08-16 by dhjin, COutPostManager�� DB���� ���� �����Ѵ�.
	i_pOutPostManager->SetOutPostManagerByDBLoad(&tmVectDBOutPostInfo);
	return TRUE;		
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumFieldDBManager::DBLoadOutPostNextWarTimeInfo(CODBCStatement *i_pODBCStmt, COutPostManager *i_pOutPostManager)
/// \brief		�������� ���� �ð� ���� ������
/// \author		dhjin
/// \date		2007-08-21 ~ 2007-08-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumFieldDBManager::DBLoadOutPostNextWarTimeInfo(CODBCStatement *i_pODBCStmt, COutPostManager *i_pOutPostManager)
{
	/************************************************************************
	CREATE PROCEDURE dbo.atum_LoadOutPostNextWarTime
	AS
		SELECT OutPostWarStartTime
		FROM atum2_db_account.dbo.ti_OutPostNextWarTime WITH (NOLOCK)
	GO
	************************************************************************/
	RETCODE	ret = SQLExecDirect(i_pODBCStmt->m_hstmt, PROCEDURE_080822_0036, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}

	SQL_TIMESTAMP_STRUCT	OutPostWarStartTime;	
	ATUM_DATE_TIME		tmOutPostNextWarTimeInfo;
	tmOutPostNextWarTimeInfo.Reset();
	vectATUM_DATE_TIME	tmVectDBOutPostNextWarTimeInfo;
	tmVectDBOutPostNextWarTimeInfo.clear();

	SQLINTEGER	arrCB[2] = {SQL_NTS, SQL_NTS};
	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_TIMESTAMP, &OutPostWarStartTime, 0, &arrCB[1]);

	while ( (ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		tmOutPostNextWarTimeInfo			= OutPostWarStartTime;
		tmVectDBOutPostNextWarTimeInfo.push_back(tmOutPostNextWarTimeInfo);
		tmOutPostNextWarTimeInfo.Reset();
	}
	i_pODBCStmt->FreeStatement();
	
	// 2007-08-16 by dhjin, COutPostManager�� DB���� ���� �����Ѵ�.
	i_pOutPostManager->SetOutPostNextWarTimeInfoByDBLoad(&tmVectDBOutPostNextWarTimeInfo);
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumFieldDBManager::DBLoadCityLeaderInfo(CODBCStatement *i_pODBCStmt, CCityLeader *i_pCityLeader)
/// \brief		CityLeader ���� ������
/// \author		dhjin
/// \date		2007-08-21 ~ 2007-08-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumFieldDBManager::DBLoadCityLeaderInfo(CODBCStatement *i_pODBCStmt, CCityLeader *i_pCityLeader)
{
	/************************************************************************
	CREATE PROCEDURE dbo.atum_LoadCityLeaderInfo
	AS
		SELECT MapIndex, Influence, CharacterUID, ExpediencyFundCumulative, ExpediencyFund, ExpediencyFundRate, Notice
		FROM td_CityLeaderInfo WITH (NOLOCK)
	GO
	************************************************************************/
	RETCODE	ret = SQLExecDirect(i_pODBCStmt->m_hstmt, PROCEDURE_080822_0037, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}
	
	SCITYLEADER_INFO		tmCityLeaderInfo;
	util::zero(&tmCityLeaderInfo, sizeof(SCITYLEADER_INFO));
	vectSCityLeaderInfo		tmVectDBCityLeaderInfo;
	tmVectDBCityLeaderInfo.clear();

	SQLINTEGER	arrCB[8] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS,    SQL_NTS, SQL_NTS, SQL_NTS};
	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tmCityLeaderInfo.MapIndex, 0, &arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_UTINYINT, &tmCityLeaderInfo.Influence, 0, &arrCB[2]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_ULONG, &tmCityLeaderInfo.CharacterUID, 0, &arrCB[3]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_ULONG, &tmCityLeaderInfo.ExpediencyFundCumulative, 0, &arrCB[4]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_ULONG, &tmCityLeaderInfo.ExpediencyFund, 0, &arrCB[5]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 6, SQL_C_ULONG, &tmCityLeaderInfo.ExpediencyFundRate, 0, &arrCB[6]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 7, SQL_C_CHAR, &tmCityLeaderInfo.Notice, SIZE_MAX_NOTICE, &arrCB[7]);

	while ( (ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		tmVectDBCityLeaderInfo.push_back(tmCityLeaderInfo);
		util::zero(&tmCityLeaderInfo, sizeof(SCITYLEADER_INFO));
	}
	i_pODBCStmt->FreeStatement();
	
	// 2007-08-22 by dhjin, CCityLeader�� DB���� ���� �����Ѵ�.
	i_pCityLeader->SetCityLeaderByLoadDB(&tmVectDBCityLeaderInfo);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumFieldDBManager::DBLoadLuckyMachine(CODBCStatement *i_pODBCStmt, vectSLUCKY_MACHINE * o_pvectLuckyMachine)
/// \brief		
/// \author		dhjin
/// \date		2008-11-10 ~ 2008-11-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumFieldDBManager::DBLoadLuckyMachine(CODBCStatement *i_pODBCStmt, vectSLUCKY_MACHINE * o_pvectLuckyMachine)
{
	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME: dbo.atum_load_ti_LuckyMachine
	-- DESC			 : 2008-11-10 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_load_ti_LuckyMachine
	AS
		SELECT BuildingIndex,MachineKind,MachineNum,CoinItemNum,SlotNum,SlotProbability,MysteryItemDropNum
		FROM atum2_db_account.dbo.ti_LuckyMachine WITH(NOLOCK)
	GO
	************************************************************************/
	RETCODE	ret = SQLExecDirect(i_pODBCStmt->m_hstmt, PROCEDURE_081110_0274, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}
	
	SLUCKY_MACHINE		tmLuckyMachine;
	util::zero(&tmLuckyMachine, sizeof(SLUCKY_MACHINE));

	// 2009-03-03 by dhjin, ��Ű�ӽ� ������
	SQLINTEGER	arrCB[8] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};	
	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tmLuckyMachine.BuildingIndex, 0, &arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_UTINYINT, &tmLuckyMachine.MachineKind, 0, &arrCB[2]);	
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_ULONG, &tmLuckyMachine.MachineNum, 0, &arrCB[3]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_ULONG, &tmLuckyMachine.CoinItemNum, 0, &arrCB[4]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_UTINYINT, &tmLuckyMachine.SlotNum, 0, &arrCB[5]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 6, SQL_C_ULONG, &tmLuckyMachine.SlotProbability, 0, &arrCB[6]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 7, SQL_C_ULONG, &tmLuckyMachine.MysteryItemDropNum, 0, &arrCB[7]);

	while ( (ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		o_pvectLuckyMachine->push_back(tmLuckyMachine);
		util::zero(&tmLuckyMachine, sizeof(SLUCKY_MACHINE));
	}
	i_pODBCStmt->FreeStatement();
	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumFieldDBManager::DBLoadActionByLevel(CODBCStatement *i_pODBCStmt, vectSACTION_BY_LEVEL_DB * o_pvectSACTION_BY_LEVEL_DB)
/// \brief		
/// \author		dhjin
/// \date		2007-10-29 ~ 2007-10-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumFieldDBManager::DBLoadActionByLevel(CODBCStatement *i_pODBCStmt, vectSACTION_BY_LEVEL_DB * o_pvectSACTION_BY_LEVEL_DB)
{
	/************************************************************************
	CREATE PROCEDURE dbo.atum_LoadActionByLevel
	AS
		SELECT Level, PollPoint
		FROM atum2_db_account.dbo.ti_ActionByLevel
	GO
	************************************************************************/
	RETCODE	ret = SQLExecDirect(i_pODBCStmt->m_hstmt, PROCEDURE_080822_0038, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}
	
	SACTION_BY_LEVEL_DB		tmActionByLevel;
	util::zero(&tmActionByLevel, sizeof(SACTION_BY_LEVEL_DB));

	SQLINTEGER	arrCB[3] = {SQL_NTS, SQL_NTS, SQL_NTS};
	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_UTINYINT, &tmActionByLevel.Level, 0, &arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_ULONG, &tmActionByLevel.PollPoint, 0, &arrCB[2]);

	while ( (ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		o_pvectSACTION_BY_LEVEL_DB->push_back(tmActionByLevel);
		util::zero(&tmActionByLevel, sizeof(SACTION_BY_LEVEL_DB));
	}
	i_pODBCStmt->FreeStatement();
	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumFieldDBManager::DBLoadPollDate(CODBCStatement *i_pODBCStmt, CInflWarManager *i_pInflWarManager)
/// \brief		���� ����
/// \author		dhjin
/// \date		2007-10-29 ~ 2007-10-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumFieldDBManager::DBLoadPollDate(CODBCStatement *i_pODBCStmt, CInflWarManager *i_pInflWarManager)
{
	/************************************************************************
	CREATE PROCEDURE dbo.atum_LoadPollDate
	AS
		SELECT ApplicationStartDate, ApplicationEndDate, VoteStartDate, VoteEndDate, Election
		FROM atum2_db_account_test.dbo.td_PollDate
	GO
	************************************************************************/
	RETCODE	ret = SQLExecDirect(i_pODBCStmt->m_hstmt, PROCEDURE_080822_0013, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}
	
	SPOLLDATE_DB		tmPOLLDATE_DB;
	util::zero(&tmPOLLDATE_DB, sizeof(SPOLLDATE_DB));
	
	SQL_TIMESTAMP_STRUCT	tmApplicationStartDate;
	SQL_TIMESTAMP_STRUCT	tmApplicationEndDate;
	SQL_TIMESTAMP_STRUCT	tmVoteStartDate;
	SQL_TIMESTAMP_STRUCT	tmVoteEndDate;
	SQL_TIMESTAMP_STRUCT	tmElection;

	SQLINTEGER	arrCB[6] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS,    SQL_NTS};
	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_TIMESTAMP, &tmApplicationStartDate, 0, &arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_TIMESTAMP, &tmApplicationEndDate, 0, &arrCB[2]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_TIMESTAMP, &tmVoteStartDate, 0, &arrCB[3]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_TIMESTAMP, &tmVoteEndDate, 0, &arrCB[4]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_TIMESTAMP, &tmElection, 0, &arrCB[5]);
	
	if ( (ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		tmPOLLDATE_DB.ApplicationStartDate			= tmApplicationStartDate;
		tmPOLLDATE_DB.ApplicationEndDate			= tmApplicationEndDate;
		tmPOLLDATE_DB.VoteStartDate					= tmVoteStartDate;
		tmPOLLDATE_DB.VoteEndDate					= tmVoteEndDate;
		tmPOLLDATE_DB.Election						= tmElection;
	}
	i_pODBCStmt->FreeStatement();
	
	// 2007-10-29 by dhjin, ���� ���� ����
	i_pInflWarManager->SetPollDate(&tmPOLLDATE_DB);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumFieldDBManager::DBLoadDBServerGroup(CODBCStatement *i_pODBCStmt, SDBSERVER_GROUP *io_pDBServerGroup)
/// \brief		// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - CAtumFieldDBManager::DBLoadDBServerGroup() �߰�
/// \author		cmkwon
/// \date		2008-04-29 ~ 2008-04-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumFieldDBManager::DBLoadDBServerGroup(CODBCStatement *i_pODBCStmt, SDBSERVER_GROUP *io_pDBServerGroup)
{
	io_pDBServerGroup->LimitUserCount					= SIZE_MAX_INIT_LIMIT_USER_COUNT_PER_SERVER_GROUP;
	io_pDBServerGroup->LockCreateCharacterForNewAccount	= FALSE;

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_LoadServerGroupInfo
	-- DESC				: ServerGroup ���� ���� �ε�
	-- // 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - 
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_LoadServerGroupInfo
		@i_ServGroupID		INT
	AS
		SELECT ServerGroupName, LimitUserCount, LockCreateCharacterForNewAccount
		FROM td_serverGroup WITH(NOLOCK)
		WHERE ServerGroupID = @i_ServGroupID
	GO
	************************************************************************/
	//SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &io_pDBServerGroup->ServerGroupID, 0, NULL);
	SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_INTEGER, SIZE_MAX_SERVER_NAME, 0, &io_pDBServerGroup->ServerGroupName, 0, NULL);
	RETCODE	ret = SQLExecDirect(i_pODBCStmt->m_hstmt, PROCEDURE_080822_0040, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}

	SQLINTEGER	arrCB[4] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, io_pDBServerGroup->ServerGroupName, 0,						&arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_LONG, &io_pDBServerGroup->LimitUserCount, 0,						&arrCB[2]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_LONG, &io_pDBServerGroup->LockCreateCharacterForNewAccount, 0,	&arrCB[3]);
	ret = SQLFetch(i_pODBCStmt->m_hstmt);
	i_pODBCStmt->FreeStatement();
	if ( SQL_NO_DATA == ret
		|| (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret) )
	{
		return FALSE;
	}
	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumFieldDBManager::DBUpdateDBServerGroup(CODBCStatement *i_pODBCStmt, SDBSERVER_GROUP *i_pDBServerGroup)
/// \brief		// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - CAtumFieldDBManager::DBUpdateDBServerGroup() �߰�
/// \author		cmkwon
/// \date		2008-04-29 ~ 2008-04-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumFieldDBManager::DBUpdateDBServerGroup(CODBCStatement *i_pODBCStmt, SDBSERVER_GROUP *i_pDBServerGroup)
{
	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_LoadServerGroupInfo
	-- DESC				: ServerGroup ���� ���� �ε�
	-- // 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - 
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_UpdateServerGroupInfo
		@i_ServGroupID							INT,
		@i_ServGroupName						VARCHAR(20),
		@i_LimitUserCount						INT,
		@i_LockCreateCharacterForNewAccount		INT
	AS
		IF EXISTS(SELECT * FROM dbo.td_serverGroup WITH(NOLOCK) WHERE ServerGroupID = @i_ServGroupID)
		BEGIN
			UPDATE dbo.td_serverGroup
			SET ServerGroupName= @i_ServGroupName, LimitUserCount= @i_LimitUserCount, LockCreateCharacterForNewAccount= @i_LockCreateCharacterForNewAccount	
			WHERE ServerGroupID = @i_ServGroupID
		END
		ELSE
		BEGIN
			INSERT INTO dbo.td_serverGroup(ServerGroupID, ServerGroupName, LimitUserCount, LockCreateCharacterForNewAccount)
			VALUES(@i_ServGroupID, @i_ServGroupName, @i_LimitUserCount, @i_LockCreateCharacterForNewAccount)			
		END
			
	GO
	************************************************************************/
	SQLINTEGER	arrCB[5] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	//SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pDBServerGroup->ServerGroupID, 0,						&arrCB[1]);
	SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_INTEGER, SIZE_MAX_SERVER_NAME, 0, i_pDBServerGroup->ServerGroupName, 0,	&arrCB[1]);
	SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pDBServerGroup->LimitUserCount, 0,						&arrCB[2]);
	SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pDBServerGroup->LockCreateCharacterForNewAccount, 0,	&arrCB[3]);
	RETCODE	ret = SQLExecDirect(i_pODBCStmt->m_hstmt, PROCEDURE_080822_0041, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}

	ret = SQLFetch(i_pODBCStmt->m_hstmt);
	i_pODBCStmt->FreeStatement();	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumFieldDBManager::DBLoadInfinityInfo(CODBCStatement *i_pODBCStmt, vectorCinemaInfo * o_pvectCinemaInfo, CLocalization *i_pLocalization)	
/// \brief		���Ǵ�Ƽ - // 2009-09-09 ~ 2010-02-26 by dhjin, ���Ǵ�Ƽ - *.cfg���� �߰�
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumFieldDBManager::DBLoadCinemaInfo(CODBCStatement *i_pODBCStmt, vectorCinemaInfo * o_pvectCinemaInfo) {
	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Load_Cinema
	-- DESC				: -- // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� �ε� 
	--					  -- // 2010-03-29 by cmkwon, ����2�� �ó׸��� ȿ���� ���� ���� �߰� - SoundFileName �ʵ� �߰�
						  -- // 2011-04-28 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ���� ��� �߰� - �ش� ��ä�� Ư�� ���� ���� ��� �߰�
	--------------------------------------------------------------------------------
	CALL dbo.atum_Load_Cinema
	************************************************************************/
	RETCODE	ret = SQLExecDirect(i_pODBCStmt->m_hstmt, PROCEDURE_090909_0501, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret) {
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}
	
	CINEMAINFO		tmCinemaInfo;
	util::zero(&tmCinemaInfo, sizeof(CINEMAINFO));

	SQLINTEGER	arrCB[28] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS,	SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS
							, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS,	SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS
							, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS,  SQL_NTS, SQL_NTS, SQL_NTS };	// 2010-03-29 by cmkwon, ����2�� �ó׸��� ȿ���� ���� ���� �߰� - // 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - // 2011-03-09 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ����Ʈ �ε��� ���� �߰� - // 2011-04-28 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ���� ��� �߰�
	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tmCinemaInfo.CinemaNum,			0, &arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_ULONG, &tmCinemaInfo.CinemaOrder,			0, &arrCB[2]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_ULONG, &tmCinemaInfo.StartTime,			0, &arrCB[3]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_ULONG, &tmCinemaInfo.EffectIdx,			0, &arrCB[4]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_FLOAT, &tmCinemaInfo.EffectPosition_X,	0, &arrCB[5]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 6, SQL_C_FLOAT, &tmCinemaInfo.EffectPosition_Y,	0, &arrCB[6]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 7, SQL_C_FLOAT, &tmCinemaInfo.EffectPosition_Z,	0, &arrCB[7]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 8, SQL_C_ULONG, &tmCinemaInfo.EffectPlayTime,		0, &arrCB[8]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 9, SQL_C_ULONG, &tmCinemaInfo.EffectPlayCount,		0, &arrCB[9]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 10, SQL_C_USHORT, &tmCinemaInfo.ObjectIdx,			0, &arrCB[10]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 11, SQL_C_ULONG, &tmCinemaInfo.ChangeObjectIdx,	0, &arrCB[11]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 12, SQL_C_ULONG, &tmCinemaInfo.ChangeTime,			0, &arrCB[12]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 13, SQL_C_ULONG, &tmCinemaInfo.CameraTremble,		0, &arrCB[13]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 14, SQL_C_UTINYINT, &tmCinemaInfo.KeyMonster,			0, &arrCB[14]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 15, SQL_C_ULONG, &tmCinemaInfo.MonsterIdx,			0, &arrCB[15]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 16, SQL_C_ULONG, &tmCinemaInfo.SummonCount,		0, &arrCB[16]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 17, SQL_C_ULONG, &tmCinemaInfo.SummonRandomPos,	0, &arrCB[17]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 18, SQL_C_ULONG, &tmCinemaInfo.ObjectBodyConditionIdx,	0, &arrCB[18]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 19, SQL_C_UBIGINT, &tmCinemaInfo.BodyCondition,	0, &arrCB[19]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 20, SQL_C_CHAR, tmCinemaInfo.TipFileName,		SIZE_MAX_FILE_NAME, &arrCB[20]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 21, SQL_C_CHAR, tmCinemaInfo.SkyFileName,		SIZE_MAX_FILE_NAME, &arrCB[21]);	
	SQLBindCol(i_pODBCStmt->m_hstmt, 22, SQL_C_CHAR, tmCinemaInfo.CinemaTalk,		SIZE_MAX_CINEMATALK_DESCRIPTION,&arrCB[22]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 23, SQL_C_CHAR, tmCinemaInfo.SoundFileName,	SIZE_MAX_FILE_NAME, &arrCB[23]);	// 2010-03-29 by cmkwon, ����2�� �ó׸��� ȿ���� ���� ���� �߰� - 
	// start 2011-04-28 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ���� ��� �߰�
	SQLBindCol(i_pODBCStmt->m_hstmt, 24, SQL_C_ULONG, &tmCinemaInfo.QuestIndex,			0, &arrCB[24]);  // 2011-03-09 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ����Ʈ �ε��� ���� �߰�
	SQLBindCol(i_pODBCStmt->m_hstmt, 25, SQL_C_ULONG, &tmCinemaInfo.MonsterIdxType,		0, &arrCB[25]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 26, SQL_C_ULONG, &tmCinemaInfo.CustomIdx,			0, &arrCB[26]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 27, SQL_C_ULONG, &tmCinemaInfo.UpdateMonsterIdx,	0, &arrCB[27]);
	// end 2011-04-28 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ���� ��� �߰�

	while ( (ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA) {
		o_pvectCinemaInfo->push_back(tmCinemaInfo);
		util::zero(&tmCinemaInfo, sizeof(CINEMAINFO));
	}
	i_pODBCStmt->FreeStatement();
	
	return TRUE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumFieldDBManager::DBLoadRevisionInfo(CODBCStatement *i_pODBCStmt, vectorInfinityModeInfo * o_pvectInfinityModeIndo)	
/// \brief		���Ǵ�Ƽ - 
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumFieldDBManager::DBLoadRevisionInfo(CODBCStatement *i_pODBCStmt, vectorRevisionInfo * o_pvectRevisionInfo) {
	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Load_Revision
	-- DESC				: -- // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ����ġ �ε� 
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Load_Revision
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
	AS
		SELECT 	RevisionNum, UnitKind, RevisionLevel, RevisionHP, RevisionDP, RevisionStandardWeapon, RevisionAdvancedWeapon
		FROM atum2_db_account.dbo.ti_Revision WITH (NOLOCK);
	GO
	************************************************************************/
	RETCODE	ret = SQLExecDirect(i_pODBCStmt->m_hstmt, PROCEDURE_090909_0508, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret) {
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}
	
	REVISIONINFO		tmRevisionInfo;
	util::zero(&tmRevisionInfo, sizeof(REVISIONINFO));

	SQLINTEGER	arrCB[8] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS,	 SQL_NTS, SQL_NTS, SQL_NTS};
	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tmRevisionInfo.RevisionNum,				0, &arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_ULONG, &tmRevisionInfo.UnitKind,					0, &arrCB[2]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_UTINYINT, &tmRevisionInfo.RevisionLevel,			0, &arrCB[3]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_USHORT, &tmRevisionInfo.RevisionHP,				0, &arrCB[4]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_USHORT, &tmRevisionInfo.RevisionDP,				0, &arrCB[5]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 6, SQL_C_ULONG, &tmRevisionInfo.RevisionStandardWeapon,	0, &arrCB[6]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 7, SQL_C_ULONG, &tmRevisionInfo.RevisionAdvancedWeapon,	0, &arrCB[7]);

	while ( (ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA) {
		o_pvectRevisionInfo->push_back(tmRevisionInfo);
		util::zero(&tmRevisionInfo, sizeof(REVISIONINFO));
	}
	i_pODBCStmt->FreeStatement();
	
	return TRUE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumFieldDBManager::DBLoadInfinityMonsterInfo(CODBCStatement *i_pODBCStmt, vectorInfinityMonsterInfo * o_pvectInfinityMonsterInfo)
/// \brief		���Ǵ�Ƽ - 
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumFieldDBManager::DBLoadInfinityMonsterInfo(CODBCStatement *i_pODBCStmt, vectorInfinityMonsterInfo * o_pvectInfinityMonsterInfo) {
	/************************************************************************
	CREATE PROCEDURE dbo.atum_Load_InfinityMonster
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
	AS
		SELECT InfinityMonsterUID, InfinityMonsterIdx, MonsterIdx, TargetType, TargetCount
		FROM atum2_db_account.dbo.ti_InfinityMonster WITH (NOLOCK);
	GO
	************************************************************************/
	RETCODE	ret = SQLExecDirect(i_pODBCStmt->m_hstmt, PROCEDURE_090909_0503, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret) {
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}
	
	INFINITY_MONSTERINFO		tmInfinityMonsterInfo;
	util::zero(&tmInfinityMonsterInfo, sizeof(INFINITY_MONSTERINFO));

	SQLINTEGER	arrCB[6] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS,	 SQL_NTS};
	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tmInfinityMonsterInfo.InfinityMonsterUID,			0, &arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_ULONG, &tmInfinityMonsterInfo.InfinityMonsterIdx,			0, &arrCB[2]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_ULONG, &tmInfinityMonsterInfo.MonsterIdx,					0, &arrCB[3]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_UTINYINT, &tmInfinityMonsterInfo.TargetType,				0, &arrCB[4]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_UTINYINT, &tmInfinityMonsterInfo.TargetCount,				0, &arrCB[5]);
	
	while ( (ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA) {
		o_pvectInfinityMonsterInfo->push_back(tmInfinityMonsterInfo);
		util::zero(&tmInfinityMonsterInfo, sizeof(INFINITY_MONSTERINFO));
	}
	i_pODBCStmt->FreeStatement();
	
	return TRUE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumFieldDBManager::ProcessDynamicServerQuery(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC)
/// \brief		// 2008-12-01 by cmkwon, �������� �����带 ����� ó���ϴ� �ý��� ���� - 
/// \author		cmkwon
/// \date		2008-12-01 ~ 2008-12-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumFieldDBManager::ProcessDynamicServerQuery(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC)
{
	switch(i_qQuery->enumQueryType)
	{
	case DQT_DailyJob:				return DQP_DailyJob(i_qQuery, i_pODBC);
	case DQT_UpdatePCBangList:		return DQP_UpdatePCBangList(i_qQuery, i_pODBC);		// 2008-12-19 by cmkwon, QT_UpdatePCBangList->DQT_UpdatePCBangList ���� - 

	case DQT_ReloadWRKServiceList:	return DQP_ReloadWRKServiceList(i_qQuery, i_pODBC);	// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 
	case DQT_ReloadWRKLevel:		return DQP_ReloadWRKLevel(i_qQuery, i_pODBC);		// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 
	case DQT_ReloadWRKFame:			return DQP_ReloadWRKFame(i_qQuery, i_pODBC);		// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 
	case DQT_ReloadWRKPVP:			return DQP_ReloadWRKPVP(i_qQuery, i_pODBC);			// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 
	case DQT_LoadInfluenceRate:		return DQP_LoadInfluenceRate(i_qQuery, i_pODBC);	// 2009-09-16 by cmkwon, ���� �ʱ�ȭ�� ���¡ ���� ���� - 
	}
	
	// error: no such DB query type
	g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::ProcessDynamicServerQuery# no such DB Query Type !! %d(%s)\r\n", i_qQuery->enumQueryType, GetDBQueryTypeString(i_qQuery->enumQueryType));
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumFieldDBManager::DQP_DailyJob(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC)
/// \brief		// 2008-12-01 by cmkwon, �������� �����带 ����� ó���ϴ� �ý��� ���� - 
/// \author		cmkwon
/// \date		2008-12-01 ~ 2008-12-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumFieldDBManager::DQP_DailyJob(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC)
{
	/************************************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_Job_DeleteCharacter
	-- DESC      : 2007-02-22 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Job_DeleteCharacter
	AS
		-- // 2007-11-16 by cmkwon, ĳ���� ���� ���� �� ���� ������ �̾Ƽ� ó�� - dbo.atum_Job_DeleteCharacter ���ν��� ����
		DECLARE @KeepTermOfDay INT
		SET @KeepTermOfDay		= 14

		-- 1. ĳ���� ���� �������� ��æƮ ����	
		DELETE td_Enchant FROM td_Enchant e, td_Store s 
		WHERE s.ItemStorage <> 2 AND s.AccountUniqueNumber IN 
		(SELECT AccountUniqueNumber FROM td_Character WITH (NOLOCK) WHERE 0 <> Race & 0x4000 AND (DATEDIFF(dd, LastStartedTime, GETDATE()) > @KeepTermOfDay)) AND s.Possess IN 
		(SELECT UniqueNumber FROM td_Character WITH (NOLOCK) WHERE 0 <> Race & 0x4000 AND (DATEDIFF(dd, LastStartedTime, GETDATE()) > @KeepTermOfDay)) AND s.UniqueNumber = e.TargetItemUniqueNumber

		-- 2. ĳ���� ���� ������ ����
		DELETE FROM td_Store WHERE ItemStorage <> 2 AND AccountUniqueNumber IN 
		(SELECT AccountUniqueNumber FROM td_Character WITH (NOLOCK) WHERE 0 <> Race & 0x4000 AND (DATEDIFF(dd, LastStartedTime, GETDATE()) > @KeepTermOfDay)) AND possess IN 
		(SELECT UniqueNumber FROM td_Character WITH (NOLOCK) WHERE 0 <> Race & 0x4000 AND (DATEDIFF(dd, LastStartedTime, GETDATE()) > @KeepTermOfDay))

		-- 3. ĳ���� �� ���� ����
		DELETE FROM td_QuickSlot WHERE AccountUID IN 
		(SELECT AccountUniqueNumber FROM td_Character WITH (NOLOCK) WHERE 0 <> Race & 0x4000 AND (DATEDIFF(dd, LastStartedTime, GETDATE()) > @KeepTermOfDay)) AND CharacterUID IN 
		(SELECT UniqueNumber FROM td_Character WITH (NOLOCK) WHERE 0 <> Race & 0x4000 AND (DATEDIFF(dd, LastStartedTime, GETDATE()) > @KeepTermOfDay))

		-- 4. ĳ���� ���� ��ų ����
		DELETE FROM td_StoreSkill WHERE AccountUniqueNumber IN 
		(SELECT AccountUniqueNumber FROM td_Character WITH (NOLOCK) WHERE 0 <> Race & 0x4000 AND (DATEDIFF(dd, LastStartedTime, GETDATE()) > @KeepTermOfDay)) AND possess IN 
		(SELECT UniqueNumber FROM td_Character WITH (NOLOCK) WHERE 0 <> Race & 0x4000 AND (DATEDIFF(dd, LastStartedTime, GETDATE()) > @KeepTermOfDay))

		-- 5. ����Ʈ ���̺��� ����
		DELETE FROM td_CharacterQuest WHERE CharacterUniqueNumber IN 
		(SELECT UniqueNumber FROM td_Character WITH (NOLOCK) WHERE 0 <> Race & 0x4000 AND (DATEDIFF(dd, LastStartedTime, GETDATE()) > @KeepTermOfDay))

		-- 6. ����Ʈ ���� ���� ���̺��� ����
		DELETE FROM td_CharacterQuestMonsterCount WHERE CharacterUniqueNumber IN 
		(SELECT UniqueNumber FROM td_Character WITH (NOLOCK) WHERE 0 <> Race & 0x4000 AND (DATEDIFF(dd, LastStartedTime, GETDATE()) > @KeepTermOfDay))

		-- 7. ģ������Ʈ���� ����
		DELETE FROM td_FriendList WHERE CharacterName IN 
		(SELECT CharacterName FROM td_Character WITH (NOLOCK) WHERE 0 <> Race & 0x4000 AND (DATEDIFF(dd, LastStartedTime, GETDATE()) > @KeepTermOfDay)) OR FriendName IN 
		(SELECT CharacterName FROM td_Character WITH (NOLOCK) WHERE 0 <> Race & 0x4000 AND (DATEDIFF(dd, LastStartedTime, GETDATE()) > @KeepTermOfDay))

		--8. Ʃ�丮�� ����Ʈ���� ����
		DELETE FROM td_TutorialComplete WHERE CharacterUID IN 
		(SELECT UniqueNumber FROM td_Character WITH (NOLOCK) WHERE 0 <> Race & 0x4000 AND (DATEDIFF(dd, LastStartedTime, GETDATE()) > @KeepTermOfDay))

		-- last ĳ���� ���̺��� ����
		DELETE FROM td_Character WHERE UniqueNumber IN 
		(SELECT UniqueNumber FROM td_Character WITH (NOLOCK) WHERE 0 <> Race & 0x4000 AND (DATEDIFF(dd, LastStartedTime, GETDATE()) > @KeepTermOfDay))
	GO
	************************************************************************/

	BOOL bRet = i_pODBC->ExecuteQuery(PROCEDURE_080822_0183);
	if(FALSE == bRet)
	{
		i_pODBC->ProcessLogMessagesForStmt(PROCEDURE_080822_0183);
		i_pODBC->FreeStatement();
		return FALSE;
	}
	i_pODBC->FreeStatement();
	
	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Guild_Update_TotalFameRankByJob
	-- DESC				: �� ���� �� ����
	-- JOB				: ���� ��ħ�ð����� JOB�� �Ǵ�.
	-- // 2008-06-05 by dhjin, EP3 - ���� ���� ����
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Guild_Update_TotalFameRankByJob
	AS
		UPDATE dbo.td_Guild SET GuildTotalFameRank = FG2.GuildTotalFameRank
		FROM dbo.td_Guild AS FG1,  
			(SELECT G1.GuildCommanderUniqueNumber, COUNT(G2.GuildCommanderUniqueNumber) + 1 AS GuildTotalFameRank
			FROM dbo.td_Guild AS G1 LEFT OUTER JOIN dbo.td_Guild AS G2
				ON G1.TotalFame < G2.TotalFame
			GROUP BY G1.GuildCommanderUniqueNumber) AS FG2
		WHERE FG1.GuildCommanderUniqueNumber = FG2.GuildCommanderUniqueNumber
	GO
	************************************************************************/
	bRet = i_pODBC->ExecuteQuery(PROCEDURE_080822_0184);
	if(FALSE == bRet)
	{
		i_pODBC->ProcessLogMessagesForStmt(PROCEDURE_080822_0183);
		i_pODBC->FreeStatement();
		return FALSE;
	}
	i_pODBC->FreeStatement();
	
	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Guild_Update_MonthlyFameRankByJob
	-- DESC				: ���� ���� �� ���� 
	-- JOB				: ���� ��ħ�ð����� JOB�� �Ǵ�.
	-- // 2008-06-05 by dhjin, EP3 - ���� ���� ����
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Guild_Update_MonthlyFameRankByJob
	AS
		UPDATE dbo.td_Guild SET GuildMonthlyFameRank = FG2.GuildMonthlyFameRank
		FROM dbo.td_Guild AS FG1,  
			(SELECT G1.GuildCommanderUniqueNumber, COUNT(G2.GuildCommanderUniqueNumber) + 1 AS GuildMonthlyFameRank
			FROM dbo.td_Guild AS G1 LEFT OUTER JOIN dbo.td_Guild AS G2
				ON G1.MonthlyFame < G2.MonthlyFame
			GROUP BY G1.GuildCommanderUniqueNumber) AS FG2
		WHERE FG1.GuildCommanderUniqueNumber = FG2.GuildCommanderUniqueNumber
	GO
	**********************************************************************/
	bRet = i_pODBC->ExecuteQuery(PROCEDURE_080822_0184);
	if(FALSE == bRet)
	{
		i_pODBC->ProcessLogMessagesForStmt(PROCEDURE_080822_0183);
		i_pODBC->FreeStatement();
		return FALSE;
	}
	i_pODBC->FreeStatement();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
///// \fn			BOOL CAtumFieldDBManager::DQP_UpdatePCBangList(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC)
///// \brief		// 2008-12-19 by cmkwon, QT_UpdatePCBangList->DQT_UpdatePCBangList ���� - 
///// \author		cmkwon
///// \date		2008-12-19 ~ 2008-12-19
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//BOOL CAtumFieldDBManager::DQP_UpdatePCBangList(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC)
//{
//	/************************************************************************
//	CREATE PROCEDURE dbo.atum_GetPCBangIPList
//	AS
//		SELECT UID, ST_IP, ED_IP FROM td_PCBang with(NOLOCK)
//	GO
//	************************************************************************/
//	BOOL bRet = i_pODBC->ExecuteQuery(PROCEDURE_080822_0133);
//	if (FALSE == bRet)
//	{
//		i_pODBC->ProcessLogMessagesForStmt(PROCEDURE_080822_0183);
//		i_pODBC->FreeStatement();
//		return FALSE;
//	}
//
//	DWORD dwLoadingStartTick = timeGetTime();		// 2008-09-02 by cmkwon, �ð� ����ϱ� �߰� - 
//
//	PCBANG_INFO		TempPCBang_Info;
//	util::zero(&TempPCBang_Info, sizeof(PCBANG_INFO));
//
//	//////////////////////////////////////////////////////////////////////////
//	// 2007-08-03 by dhjin, PC�� ����Ʈ ����
//	SIPClassD				TempIPClassD;
//	util::zero(&TempIPClassD, sizeof(SIPClassD));
//	CPCBangIPManager		tmPCBangIPManager;
//	INT						tmPCBangUIDCount	= 0;
//
//	SQLINTEGER cb[4]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
//	SQLBindCol(i_pODBC->GetSTMTHandle(), 1, SQL_C_ULONG, &TempPCBang_Info.PCBangUID, 0,				&cb[1]);
//	SQLBindCol(i_pODBC->GetSTMTHandle(), 2, SQL_C_CHAR, TempPCBang_Info.ST_IP, SIZE_MAX_IPADDRESS,		&cb[2]);
//	SQLBindCol(i_pODBC->GetSTMTHandle(), 3, SQL_C_CHAR, TempPCBang_Info.ED_IP, 4,					&cb[3]);	
//	RETCODE ret = 0;
//	while ( (ret = SQLFetch(i_pODBC->GetSTMTHandle())) != SQL_NO_DATA)
//	{
//		char * pIPStart0 = TempPCBang_Info.ST_IP;
//		char * pIPEndD   = TempPCBang_Info.ED_IP;
//		
//		char * pIPClassA = strtok(pIPStart0,".");
//		if (pIPClassA == NULL)
//		{		
//			char	szError[1024];
//			sprintf(szError, "[Error] pIPClassA RANGE error \r\n");
//			g_pFieldGlobal->WriteSystemLog(szError);
//			DBGOUT(szError);
//			continue;
//		}
//		char * pIPClassB = strtok(NULL,".");
//		if (pIPClassB == NULL)
//		{
//			char	szError[1024];
//			sprintf(szError, "[Error] pIPClassB RANGE error \r\n");
//			g_pFieldGlobal->WriteSystemLog(szError);
//			DBGOUT(szError);
//			continue;
//		}
//		char * pIPClassC = strtok(NULL,".");
//		if (pIPClassC == NULL)
//		{
//			char	szError[1024];
//			sprintf(szError, "[Error] pIPClassC RANGE error \r\n");
//			g_pFieldGlobal->WriteSystemLog(szError);
//			DBGOUT(szError);
//			continue;
//		}
//		char * pIPClassD = strtok(NULL,"\0");
//		if (pIPClassD == NULL)
//		{		
//			char	szError[1024];
//			sprintf(szError, "[Error] pIPClassD RANGE error \r\n");
//			g_pFieldGlobal->WriteSystemLog(szError);
//			DBGOUT(szError);
//			continue;
//		}
//		
//		BYTE iIPStartA	 = atoi(pIPClassA);
//		BYTE iIPStartB	 = atoi(pIPClassB);
//		BYTE iIPStartC	 = atoi(pIPClassC);
//		BYTE iIPStartD	 = atoi(pIPClassD);
//		BYTE iIPEndD	 = atoi(pIPEndD);
//
//		if (FALSE == util::in_range(1, iIPStartA, 255) ||
//			FALSE == util::in_range(0, iIPStartB, 255) ||
//			FALSE == util::in_range(0, iIPStartC, 255) ||
//			FALSE == util::in_range(1, iIPStartD, 255)	||
//			FALSE == util::in_range(1, iIPEndD, 255))
//		{// 2007-01-22 by dhjin, IP ���� üũ
//			char	szError[1024];
//			sprintf(szError, "[Error] UpdatePCBangIPList RANGE error \r\n");
//			g_pFieldGlobal->WriteSystemLog(szError);
//			DBGOUT(szError);
//			continue;
//		}			
//		
//		BYTE IPClassDGap	 = (iIPEndD - iIPStartD);
//
//		if ( 0 > IPClassDGap)
//		{// 2007-01-22 by dhjin, ����IP�� ��IP���� ũ�� ����
//			char	szError[1024];
//			sprintf(szError, "[Error] UpdatePCBangIPList (StartIP - EndIp) error \r\n");
//			g_pFieldGlobal->WriteSystemLog(szError);
//			DBGOUT(szError);
//			continue;
//		}
//		
//		
//		// 2007-08-03 by dhjin, PC�� IP����Ʈ ����.
//		TempIPClassD.PCBangUID		=	TempPCBang_Info.PCBangUID;
//		TempIPClassD.IPClassDStart	=	iIPStartD;
//		TempIPClassD.IPClassDEnd	=	iIPEndD;
//		char szIP[SIZE_MAX_IPADDRESS];
//		sprintf(szIP, "%d.%d.%d.0", iIPStartA, iIPStartB, iIPStartC);
//		DWORD	dwIPClassC = inet_addr(szIP);
//		
//		tmPCBangIPManager.InsertPCBangIPList(dwIPClassC, &TempIPClassD);
//        tmPCBangUIDCount++;
//// 2007-08-07 by dhjin, ���� �Լ��� ����
////		for (int i=0 ; i <= IPClassDGap; i++)
////		{// 2007-01-22 by dhjin, PC�� IP ���� ��ŭ ����Ʈ�� �߰� �Ѵ�.			
////			char szIP[SIZE_MAX_IPADDRESS];
////			sprintf(szIP, "%d.%d.%d.%d", iIPStartA, iIPStartB, iIPStartC, iIPStartD+i);
////			mapPCBangIPList.insert(pair<string, UID32_t>(szIP, TempPCBang_Info.PCBangUID));
////		}
//		util::zero(&TempPCBang_Info, sizeof(PCBANG_INFO));
//		util::zero(&TempIPClassD, sizeof(SIPClassD));
//	}
//	i_pODBC->FreeStatement();
//
//	DWORD dwLoadingEndTick = timeGetTime();								// 2008-09-02 by cmkwon, �ð� ����ϱ� �߰� - 
//
//	g_pFieldGlobal->m_pPCBangIPManager->SetPCBangIPList(tmPCBangIPManager.GetPCBangIPList());
//	
//	DWORD dwUpdatedPCBangIPListTick = timeGetTime();					// 2008-09-02 by cmkwon, �ð� ����ϱ� �߰� - 
//	int nLoadingTime = dwLoadingEndTick - dwLoadingStartTick;			// 2008-09-02 by cmkwon, �ð� ����ϱ� �߰� - 
//	int nUpdatingTime = dwUpdatedPCBangIPListTick - dwLoadingEndTick;	// 2008-09-02 by cmkwon, �ð� ����ϱ� �߰� - 
//
//	// 2007-08-07 by dhjin, PC�� ����Ʈ UID ���� �����.
//	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] CAtumFieldDBManager::DQP_UpdatePCBangList# PCBangUID Count: %d, TotalTime(%dms) = DBLoadingTime(%dms) + UpdateTime(%dms)\r\n", tmPCBangUIDCount, nLoadingTime+nUpdatingTime, nLoadingTime, nUpdatingTime);
//	return TRUE;
//}

/////////////////////////////////////////////////////////////////////////////////
///// \fn			
///// \brief		// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 
///// \author		cmkwon
///// \date		2009-02-19 ~ 2009-02-19
///// \warning	
/////
///// \param		
///// \return		
///////////////////////////////////////////////////////////////////////////////
//BOOL CAtumFieldDBManager::DQP_ReloadWRKServiceList(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC)
//{// DQT_ReloadWRKServiceList
//	
//	// 2009-11-24 by cmkwon, ���巩ŷ �ý��� �α� �߰� - 
//	server::log(true, "  [Notify] WRK CAtumFieldDBManager::DQP_ReloadWRKServiceList# \r\n");
//
//
//	SQLHSTMT hstmt = i_pODBC->GetSTMTHandle();
//
//	///////////////////////////////////////////////////////////////////////////////	
//	// 2009-02-19 by cmkwon, 1. ���巩ŷ�� ���� Service ���� ��������
//	BOOL bRet = i_pODBC->ExecuteQuery(PROCEDURE_090219_0300);
//	if (FALSE == bRet)
//	{
//		// 2009-02-19 by cmkwon, ���巩ŷ ���� ���� ������ ���� ������ ��� ��ŷ ���� ó������ �ʴ´�.
//		i_pODBC->ProcessLogMessagesForStmt(PROCEDURE_090219_0300);
//		i_pODBC->FreeStatement();
//		return FALSE;
//	}
//
//	INT selfServiceUID	= 0;
//	char selfServiceName[SIZE_MAX_SERVICE_NAME];	util::zero(selfServiceName, SIZE_MAX_SERVICE_NAME);
//	char selfServGroupName[SIZE_MAX_SERVER_NAME];	util::zero(selfServGroupName, SIZE_MAX_SERVER_NAME);
//
//	SQLINTEGER cb[5]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
//	SQLBindCol(hstmt, 1, SQL_C_LONG, &selfServiceUID, 0,						&cb[1]);
//	SQLBindCol(hstmt, 2, SQL_C_CHAR, selfServiceName, SIZE_MAX_SERVICE_NAME,	&cb[2]);
//	SQLBindCol(hstmt, 3, SQL_C_CHAR, selfServGroupName, SIZE_MAX_SERVER_NAME,	&cb[3]);	
//	RETCODE ret = SQLFetch(hstmt);
//	i_pODBC->FreeStatement();
//	if(0 == selfServiceUID
//		|| 0 >= strlen(selfServiceName)
//		|| 0 >= strlen(selfServGroupName))
//	{
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] WRK DQP_ReloadWRKServiceList# self setting is invalid !! ServiceUID(%d) ServiceName(%s) ServerGroupName(%s) \r\n", selfServiceUID, selfServiceName, selfServGroupName);
//		return TRUE;
//	}
//
//	///////////////////////////////////////////////////////////////////////////////	
//	// 2009-02-19 by cmkwon, 2. 
//	bRet = i_pODBC->ExecuteQuery(PROCEDURE_090219_0310);
//	if (FALSE == bRet)
//	{
//		i_pODBC->FreeStatement();
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] WRK DQP_ReloadWRKServiceList# get all service info fail 1 !! ServiceUID(%d) ServiceName(%s) ServerGroupName(%s) \r\n", selfServiceUID, selfServiceName, selfServGroupName);
//		return TRUE;
//	}
//
//	mtvectSWRK_SERVICE_INFO tmServiceList;
//	SWRK_SERVICE_INFO		tmServInfo;
//
//	cb[1] = cb[2] = cb[3] = cb[4] = SQL_NTS;
//	SQLBindCol(hstmt, 1, SQL_C_LONG, &tmServInfo.ServiceUID, 0,											&cb[1]);
//	SQLBindCol(hstmt, 2, SQL_C_CHAR, &tmServInfo.ServiceName, SIZE_MAX_SERVICE_NAME,					&cb[2]);
//	SQLBindCol(hstmt, 3, SQL_C_BINARY, tmServInfo.ServiceSymbolIamge, SIZE_MAX_SERVICE_SYMBOL_IMAGE,	&cb[3]);	
//	SQLBindCol(hstmt, 4, SQL_C_LONG, &tmServInfo.SymbolImageSize, 0,									&cb[4]);
//
//	util::zero(&tmServInfo, sizeof(tmServInfo));
//	while (true)
//	{
//		auto ret = SQLFetch(hstmt);
//
//		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) break;
//		
//		tmServiceList.push_back(tmServInfo);
//
//		util::zero(tmServInfo);
//	}
//	i_pODBC->FreeStatement();
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 2009-02-19 by cmkwon, 3. 
//	m_pFieldIOCP8->m_WRankingManager.m_ServiceManager.InitWRankingServiceManager(&tmServiceList);
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 2010-01-14 by cmkwon, ���巩ŷ ����Ʈ ���ε� ���� ���� - 
//	this->MakeQueryWithNewThread(DQT_ReloadWRKLevel, NULL, NULL, NULL, FALSE);
//	this->MakeQueryWithNewThread(DQT_ReloadWRKFame, NULL, NULL, NULL, FALSE);
//	this->MakeQueryWithNewThread(DQT_ReloadWRKPVP, NULL, NULL, NULL, FALSE);	
//	return TRUE;
//}
//
/////////////////////////////////////////////////////////////////////////////////
///// \fn			
///// \brief		// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 
///// \author		cmkwon
///// \date		2009-02-18 ~ 2009-02-18
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//BOOL CAtumFieldDBManager::DQP_ReloadWRKLevel(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC)
//{// DQT_ReloadWRKLevel
//
//	// 2009-11-24 by cmkwon, ���巩ŷ �ý��� �α� �߰� - 
//	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] WRK CAtumFieldDBManager::DQP_ReloadWRKLevel# \r\n");
//	
//	///////////////////////////////////////////////////////////////////////////////
//	// 2009-02-24 by cmkwon, 1. Self ServiceInfo �� ServerGroupInfo ��������
//	SWRK_SERVICE_INFO tmServiInfo;		util::zero(&tmServiInfo, sizeof(tmServiInfo));
//	SWRK_SERVER_GROUP tmServGroup;		util::zero(&tmServGroup, sizeof(tmServGroup));
//
//	//tmServGroup.ServerGroupID	= g_pFieldGlobal->GetMGameServerID();
//	if(!WRK_LoadSelfServiceInfor8ServerGroupInfo(i_pODBC, &tmServiInfo, &tmServGroup))
//	{
//		server::log(true, "[ERROR] WRK DQP_ReloadWRKLevel# self setting is invalid !!  ServiceName(%s) ServerGroupName(%s) \r\n",
//			tmServiInfo.ServiceName, tmServGroup.ServerGroupNameforRK);
//		return TRUE;
//	}
//
//	if(i_qQuery->nGeneralParam1)
//	{// 2009-02-25 by cmkwon, ������ �÷���
//
//		///////////////////////////////////////////////////////////////////////////////
//		// 2009-02-24 by cmkwon, 2. ���� ���� ��ŷ�� �ٽ� �����Ѵ�.
//		if (FALSE == WRK_LevelArrangeRanking(i_pODBC, &tmServiInfo, &tmServGroup))
//		{
//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] WRK DQP_ReloadWRKLevel# arrange level ranking fail !! ServiceName(%s) ServerGroupName(%s) \r\n",
//				tmServiInfo.ServiceName, tmServGroup.ServerGroupNameforRK);
//			return TRUE;
//		}
//	}
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 2009-02-19 by cmkwon, 3. ���� ��ŷ ����Ʈ�� �����´�.
//	mtvectSWRK_LEVEL_FOR_DB tmRankerList;
//	BYTE byRankScope = RANKING_SCOPE_LOCAL;
//	if (FALSE == CAtumDBHelper::WRK_LevelLoadRanking(i_pODBC, 0, byRankScope, &tmRankerList))
//	{		
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] WRK DQP_ReloadWRKLevel# get level ranking fail 1 !! ServiceName(%s) ServerGroupName(%s) \r\n",
//			tmServiInfo.ServiceName, tmServGroup.ServerGroupNameforRK);
//		return TRUE;
//	}
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 2009-02-19 by cmkwon, 4. ���� ��ŷ ����Ʈ�� �����´�.
//	byRankScope = RANKING_SCOPE_WORLD;
//	if (FALSE == CAtumDBHelper::WRK_LevelLoadRanking(i_pODBC, 0, byRankScope, &tmRankerList))
//	{		
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] WRK DQP_ReloadWRKLevel# get level ranking fail 2 !! ServiceName(%s) ServerGroupName(%s) \r\n",
//			tmServiInfo.ServiceName, tmServGroup.ServerGroupNameforRK);
//		return TRUE;
//	}
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 2009-02-19 by cmkwon, 5. ���Ӽ����� ��ŷ ������ �ٽ� �����Ѵ�.
//	m_pFieldIOCP8->m_WRankingManager.m_LevelRankManager.InitEachWRankingManager(&tmRankerList);
//	return TRUE;
//}
//
/////////////////////////////////////////////////////////////////////////////////
///// \fn			
///// \brief		// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 
///// \author		cmkwon
///// \date		2009-02-18 ~ 2009-02-18
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//BOOL CAtumFieldDBManager::DQP_ReloadWRKFame(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC)
//{// DQT_ReloadWRKFame
//	
//	// 2009-11-24 by cmkwon, ���巩ŷ �ý��� �α� �߰� - 
//	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] WRK CAtumFieldDBManager::DQP_ReloadWRKFame# \r\n");
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 2009-02-24 by cmkwon, 1. Self ServiceInfo �� ServerGroupInfo ��������
//	SWRK_SERVICE_INFO tmServiInfo;		util::zero(&tmServiInfo, sizeof(tmServiInfo));
//	SWRK_SERVER_GROUP tmServGroup;		util::zero(&tmServGroup, sizeof(tmServGroup));	
//
//	//ServGroup.ServerGroupID	= g_pFieldGlobal->GetMGameServerID();
//	if(!WRK_LoadSelfServiceInfor8ServerGroupInfo(i_pODBC, &tmServiInfo, &tmServGroup))
//	{
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] WRK DQP_ReloadWRKLevel# self setting is invalid !! erviceName(%s) ServerGroupName(%s) \r\n",
//			tmServiInfo.ServiceName, tmServGroup.ServerGroupNameforRK);
//		return TRUE;
//	}
//
//	if(i_qQuery->nGeneralParam1)
//	{// 2009-02-25 by cmkwon, ������ �÷���
//		///////////////////////////////////////////////////////////////////////////////
//		// 2009-02-24 by cmkwon, 2. ���� ���� ��ŷ�� �ٽ� �����Ѵ�.
//		if (!WRK_FameArrangeRanking(i_pODBC, &tmServiInfo, &tmServGroup))
//		{
//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] WRK DQP_ReloadWRKFame# arrange level ranking fail !! ServiceName(%s) ServerGroupName(%s) \r\n",
//				tmServiInfo.ServiceName, tmServGroup.ServerGroupNameforRK);
//			return TRUE;
//		}
//	}
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 2009-02-19 by cmkwon, 3. ���� ��ŷ ����Ʈ�� �����´�.
//	mtvectSWRK_FAME_FOR_DB	tmRankerList;
//	BYTE byRankScope = RANKING_SCOPE_LOCAL;
//	if (FALSE == CAtumDBHelper::WRK_FameLoadRanking(i_pODBC, 0, byRankScope, &tmRankerList))
//	{		
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] WRK DQP_ReloadWRKFame# get level ranking fail 1 !! ServiceName(%s) ServerGroupName(%s) \r\n",
//			tmServiInfo.ServiceName, tmServGroup.ServerGroupNameforRK);
//		return TRUE;
//	}
//	
//	///////////////////////////////////////////////////////////////////////////////
//	// 2009-02-19 by cmkwon, 4. ���� ��ŷ ����Ʈ�� �����´�.
//	byRankScope = RANKING_SCOPE_WORLD;
//	if (FALSE == CAtumDBHelper::WRK_FameLoadRanking(i_pODBC, 0, byRankScope, &tmRankerList))
//	{		
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] WRK DQP_ReloadWRKFame# get level ranking fail 2 !! ServiceName(%s) ServerGroupName(%s) \r\n",
//			tmServiInfo.ServiceName, tmServGroup.ServerGroupNameforRK);
//		return TRUE;
//	}
//
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 2009-02-19 by cmkwon, 5. ���Ӽ����� ��ŷ ������ �ٽ� �����Ѵ�.
//	m_pFieldIOCP8->m_WRankingManager.m_FameRankManager.InitEachWRankingManager(&tmRankerList);
//	return TRUE;
//}
//
/////////////////////////////////////////////////////////////////////////////////
///// \fn			
///// \brief		// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 
///// \author		cmkwon
///// \date		2009-02-18 ~ 2009-02-18
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//BOOL CAtumFieldDBManager::DQP_ReloadWRKPVP(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC)
//{// DQT_ReloadWRKPVP
//	
//	// 2009-11-24 by cmkwon, ���巩ŷ �ý��� �α� �߰� - 
//	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] WRK CAtumFieldDBManager::DQP_ReloadWRKPVP# \r\n");
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 2009-02-24 by cmkwon, 1. Self ServiceInfo �� ServerGroupInfo ��������
//	SWRK_SERVICE_INFO tmServiInfo;		util::zero(&tmServiInfo, sizeof(tmServiInfo));
//	SWRK_SERVER_GROUP tmServGroup;		util::zero(&tmServGroup, sizeof(tmServGroup));	
//	
//	//tmServGroup.ServerGroupID	= g_pFieldGlobal->GetMGameServerID();
//	if(!WRK_LoadSelfServiceInfor8ServerGroupInfo(i_pODBC, &tmServiInfo, &tmServGroup))
//	{
//		server::log(true, "[ERROR] WRK DQP_ReloadWRKPVP# self setting is invalid !! ServiceName(%s) ServerGroupName(%s) \r\n",
//			tmServiInfo.ServiceName, tmServGroup.ServerGroupNameforRK);
//		return TRUE;
//	}
//	
//	if(i_qQuery->nGeneralParam1)
//	{// 2009-02-25 by cmkwon, ������ �÷���
//		///////////////////////////////////////////////////////////////////////////////
//		// 2009-02-24 by cmkwon, 2. ���� ���� ��ŷ�� �ٽ� �����Ѵ�.
//		if (!WRK_PVPArrangeRanking(i_pODBC, &tmServiInfo, &tmServGroup))
//		{
//			server::log(true, "[ERROR] WRK DQP_ReloadWRKPVP# arrange level ranking fail !! ServiceName(%s) ServerGroupName(%s) \r\n",
//				tmServiInfo.ServiceName, tmServGroup.ServerGroupNameforRK);
//			return TRUE;
//		}
//	}
//	
//	///////////////////////////////////////////////////////////////////////////////
//	// 2009-02-19 by cmkwon, 3. ���� ��ŷ ����Ʈ�� �����´�.
//	mtvectSWRK_PVP_FOR_DB	tmRankerList;
//	BYTE byRankScope = RANKING_SCOPE_LOCAL;
//	if (!WRK_PVPLoadRanking(i_pODBC, 0, byRankScope, &tmRankerList))
//	{		
//		server::log(true, "[ERROR] WRK DQP_ReloadWRKPVP# get level ranking fail 1 !! ServiceName(%s) ServerGroupName(%s) \r\n",
//			tmServiInfo.ServiceName, tmServGroup.ServerGroupNameforRK);
//		return TRUE;
//	}
//	
//	///////////////////////////////////////////////////////////////////////////////
//	// 2009-02-19 by cmkwon, 4. ���� ��ŷ ����Ʈ�� �����´�.
//	byRankScope = RANKING_SCOPE_WORLD;
//	if (!WRK_PVPLoadRanking(i_pODBC, 0, byRankScope, &tmRankerList))
//	{		
//		server::log(true, "[ERROR] WRK DQP_ReloadWRKPVP# get level ranking fail 2 !! ServiceName(%s) ServerGroupName(%s) \r\n",
//			tmServiInfo.ServiceName, tmServGroup.ServerGroupNameforRK);
//		return TRUE;
//	}
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 2009-02-19 by cmkwon, 5. ���Ӽ����� ��ŷ ������ �ٽ� �����Ѵ�.
//	m_pFieldIOCP8->m_WRankingManager.m_PVPRankManager.InitEachWRankingManager(&tmRankerList);
//	return TRUE;
//}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-16 by cmkwon, ���� �ʱ�ȭ�� ���¡ ���� ���� - 
/// \author		cmkwon
/// \date		2009-09-18 ~ 2009-09-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumFieldDBManager::DQP_LoadInfluenceRate(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC)
{// DQT_LoadInfluenceRate

	if(g_pFieldGlobal->IsArenaServer())
	{// 2009-09-22 by cmkwon, �Ʒ��� ������ �ε� �ʿ� ����
		return TRUE;
	}

	SQLHSTMT hstmt = i_pODBC->GetSTMTHandle();

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Rearrange_InfluenceRate
	-- DESC				: -- // 2009-09-16 by cmkwon, ���� �ʱ�ȭ�� ���¡ ���� ���� - 
	--						
	--------------------------------------------------------------------------------
	CALL dbo.atum_Rearrange_InfluenceRate

	************************************************************************/
	SQLINTEGER	arrCB[8] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS ,SQL_NTS,SQL_NTS,SQL_NTS};
	int nPeriodDay = (-1)*PERIOD_OF_DAY_FOR_VALID_INFLUENCE;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nPeriodDay, 0,	&arrCB[1]);
	
	BOOL bRet = i_pODBC->ExecuteQuery(PROCEDURE_090918_0400);
	if(FALSE == bRet)
	{
		i_pODBC->ProcessLogMessagesForStmt(PROCEDURE_090918_0400);
		i_pODBC->FreeStatement();
		return FALSE;
	}
	i_pODBC->FreeStatement();
	
	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Load_InfluenceRate
	-- DESC				: -- // 2009-09-16 by cmkwon, ���� �ʱ�ȭ�� ���¡ ���� ���� - 
	--						
	--------------------------------------------------------------------------------
	CALL dbo.atum_Load_InfluenceRate

	************************************************************************/
	mtvectSINFL_USER_COUNT	tmUserCountList;
	SINFL_USER_COUNT		tmUserCnt;
	SQL_TIMESTAMP_STRUCT	UpdatedTime;
	
	arrCB[1] = arrCB[2] = arrCB[3] = arrCB[4] = arrCB[5] = arrCB[6] = arrCB[7] = SQL_NTS;
	SQLBindCol(hstmt, 1, SQL_C_UTINYINT, &tmUserCnt.StartLevel, 0,			&arrCB[1]);
	SQLBindCol(hstmt, 2, SQL_C_UTINYINT, &tmUserCnt.EndLevel, 0,			&arrCB[2]);
	SQLBindCol(hstmt, 3, SQL_C_LONG, &tmUserCnt.AllowablePercent, 0,		&arrCB[3]);
	SQLBindCol(hstmt, 4, SQL_C_TIMESTAMP, &UpdatedTime, 0,					&arrCB[4]);
	SQLBindCol(hstmt, 5, SQL_C_LONG, &tmUserCnt.arrUserCount[0], 0,			&arrCB[5]);
	SQLBindCol(hstmt, 6, SQL_C_LONG, &tmUserCnt.arrUserCount[1], 0,			&arrCB[6]);
	SQLBindCol(hstmt, 7, SQL_C_LONG, &tmUserCnt.arrUserCount[2], 0,			&arrCB[7]);

	bRet = i_pODBC->ExecuteQuery(PROCEDURE_090918_0401);
	if(FALSE == bRet)
	{
		i_pODBC->ProcessLogMessagesForStmt(PROCEDURE_090918_0401);
		i_pODBC->FreeStatement();
		return FALSE;
	}

	util::zero(&tmUserCnt, sizeof(tmUserCnt));
	util::zero(&UpdatedTime, sizeof(UpdatedTime));
	while(TRUE)
	{
		SQLRETURN ret = SQLFetch(hstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			break;
		}
		tmUserCnt.UpdatedTime	= UpdatedTime;

		tmUserCountList.push_back(tmUserCnt);

		util::zero(&tmUserCnt, sizeof(tmUserCnt));
		util::zero(&UpdatedTime, sizeof(UpdatedTime));
	}
	i_pODBC->FreeStatement();

	m_pFieldIOCP8->m_InflRateMan.InitInflRateManager(&tmUserCountList);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-09 by cmkwon, ����2�� �߰� ����(�ܰ躰 ���� �߰�) - 
/// \author		cmkwon
/// \date		2010-04-12 ~ 2010-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumFieldDBManager::DBLoad_TenderList(CODBCStatement *i_pODBCStmt, vectorTenderInfo *o_pTenderItemList)
{
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Load_Tender
	-- DESC				: -- // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - Tender �ε� 
	--------------------------------------------------------------------------------
	CALL dbo.atum_Load_Tender
	**********************************************************************/
	if(FALSE == i_pODBCStmt->ExecuteQuery(PROCEDURE_090909_0527))
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DB ERROR] CAtumFieldDBManager::DBLoad_TenderList# call PROCEDURE_090909_0527 \r\n");
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}
	
	SQLINTEGER		arrCB[3] = {SQL_NTS,SQL_NTS,SQL_NTS};
	TENDER_INFO		tmTenderInfo;
	util::zero(&tmTenderInfo, sizeof(TENDER_INFO));

	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &tmTenderInfo.TenderUID, 0,			&arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_LONG, &tmTenderInfo.TenderItemNum, 0,		&arrCB[2]);	
	while (TRUE)
	{
		util::zero(&tmTenderInfo, sizeof(TENDER_INFO));
		RETCODE	ret = SQLFetch(i_pODBCStmt->m_hstmt);

		if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
		{
			break;
		}
		o_pTenderItemList->push_back(tmTenderInfo);
	}
	i_pODBCStmt->FreeStatement();

	return TRUE;
}


// QT_CreateCharacter
void CAtumFieldDBManager::QP_CreateCharacter(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	CHARACTER* pMsg = (CHARACTER*)q.pQueryParam;

	RETCODE ret;
	SDWORD pcbNTS = SQL_NTS;

	// �̹� �����ϴ��� Ȯ��!
	UID32_t	characUniqueNumber;

	char spbuf[SIZE_MAX_SQL_PATTERN_BUFFER];
// 2009-01-29 by cmkwon, ���ڿ� ���� �˻��� �ʿ���� LIKE ����� ���� - �Ʒ��� ���� ���� ��.
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_PATTERN_BUFFER, 0, GetSqlPattern(pMsg->CharacterName, spbuf), 0, NULL);
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pMsg->CharacterName, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0042, SQL_NTS);

	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"QP_GetCharacterByName Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pMsg);
		return;
	}

	SQLBindCol(hstmt, 1, SQL_C_ULONG, &characUniqueNumber, 0, NULL);
	ret = SQLFetch(hstmt);
	if(ret != SQL_NO_DATA)
	{
		///////////////////////////////////////////////////////////////////////////
		// ĳ���Ͱ� �̹� ������, Error ó��
		q.pFieldIOCPSocket->ResCreateCharacter(pMsg, ERR_COMMON_CHARACTER_ALREADY_EXISTS);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pMsg);

		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);

	// ����
	SQLBindParameter(hstmt, 1,SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pMsg->CharacterName, 0, &pcbNTS);
	SQLBindParameter(hstmt, 2,SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_PATTERN_BUFFER, 0, GetSqlPattern(pMsg->CharacterName, spbuf), 0, &pcbNTS);
	SQLBindParameter(hstmt, 3,SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, _strlwr(pMsg->AccountName), 0, &pcbNTS);
	SQLBindParameter(hstmt, 4,SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pMsg->AccountUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 5,SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pMsg->Color, 0, NULL);
	SQLBindParameter(hstmt, 6,SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pMsg->Race, 0, NULL);
	SQLBindParameter(hstmt, 7,SQL_PARAM_INPUT, SQL_C_USHORT, SQL_INTEGER, 0, 0, &pMsg->UnitKind, 0, NULL);	// DB���� INTERGER�� �Ǿ� ����
	SQLBindParameter(hstmt, 8,SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pMsg->InfluenceType, 0, NULL);
	SQLBindParameter(hstmt, 9,SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pMsg->SelectableInfluenceMask, 0, NULL);
	SQLBindParameter(hstmt, 10,SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pMsg->PilotFace, 0, NULL);
	SQLBindParameter(hstmt, 11,SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pMsg->CharacterMode, 0, NULL);
	SQLBindParameter(hstmt, 12,SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pMsg->AutoStatType, 0, NULL);
	SQLBindParameter(hstmt, 13,SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pMsg->GearStat.AttackPart, 0, NULL);
	SQLBindParameter(hstmt, 14,SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pMsg->GearStat.DefensePart, 0, NULL);
	SQLBindParameter(hstmt, 15,SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pMsg->GearStat.FuelPart, 0, NULL);
	SQLBindParameter(hstmt, 16,SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pMsg->GearStat.SoulPart, 0, NULL);
	SQLBindParameter(hstmt, 17,SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pMsg->GearStat.ShieldPart, 0, NULL);
	SQLBindParameter(hstmt, 18,SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pMsg->GearStat.DodgePart, 0, NULL);
	SQLBindParameter(hstmt, 19,SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_GUILD_NAME, 0, pMsg->GuildName, 0, &pcbNTS);
	SQLBindParameter(hstmt, 20,SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pMsg->GuildUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 21,SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pMsg->Level, 0, NULL);
	SQLBindParameter(hstmt, 22,SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_FLOAT, 0, 0, &pMsg->Experience, 0, NULL);
	SQLBindParameter(hstmt, 23,SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &pMsg->BodyCondition, 0, NULL);
	SQLBindParameter(hstmt, 24,SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pMsg->Propensity, 0, NULL);
	SQLBindParameter(hstmt, 25,SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pMsg->Status, 0, NULL);
	SQLBindParameter(hstmt, 26,SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pMsg->PKWinPoint, 0, NULL);
	SQLBindParameter(hstmt, 27,SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pMsg->PKLossPoint, 0, NULL);
	SQLBindParameter(hstmt, 28,SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pMsg->Material, 0, NULL);
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	SQLBindParameter(hstmt, 29, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pMsg->HP, 0, NULL);
	SQLBindParameter(hstmt, 30, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pMsg->DP, 0, NULL);
#else
	SQLBindParameter(hstmt, 29, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pMsg->HP, 0, NULL);
	SQLBindParameter(hstmt, 30, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pMsg->DP, 0, NULL);
#endif	
	SQLBindParameter(hstmt, 31,SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pMsg->SP, 0, NULL);
	SQLBindParameter(hstmt, 32,SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pMsg->EP, 0, NULL);
	SQLBindParameter(hstmt, 33,SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_PET_NAME, 0, pMsg->PetName, 0, &pcbNTS);
	SQLBindParameter(hstmt, 34,SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pMsg->PetLevel, 0, NULL);
	SQLBindParameter(hstmt, 35,SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_FLOAT, 0, 0, &pMsg->PetExperience, 0, NULL);
	SQLBindParameter(hstmt, 36,SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pMsg->PositionVector.x, 0, NULL);
	SQLBindParameter(hstmt, 37,SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pMsg->PositionVector.y, 0, NULL);
	SQLBindParameter(hstmt, 38,SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pMsg->PositionVector.z, 0, NULL);
	SQLBindParameter(hstmt, 39,SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pMsg->MapChannelIndex.MapIndex, 0, NULL);
	SQLBindParameter(hstmt, 40,SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pMsg->MapChannelIndex.ChannelIndex, 0, NULL);
	SQLBindParameter(hstmt, 41,SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pMsg->MaxLevel, 0, NULL);
	SQLBindParameter(hstmt, 42,SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pMsg->CurrentHP, 0, NULL);
	SQLBindParameter(hstmt, 43,SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pMsg->CurrentDP, 0, NULL);
	SQLBindParameter(hstmt, 44,SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pMsg->CurrentSP, 0, NULL);
	SQLBindParameter(hstmt, 45,SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pMsg->CurrentEP, 0, NULL);
	SQLBindParameter(hstmt, 46,SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pMsg->BonusStat, 0, NULL);
// 2005-11-15 by cmkwon, ������
//	SQLBindParameter(hstmt, 46,SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pMsg->BonusSkillPoint, 0, NULL);
	SQLBindParameter(hstmt, 47,SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pMsg->BonusStatPoint, 0, NULL);
	SQLBindParameter(hstmt, 48,SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &pMsg->LastPartyID, 0, NULL);
// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) - 
//	SQLBindParameter(hstmt, 49,SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &q.pFieldIOCPSocket->m_nNumCharacters, 0, NULL);
// 	ret = SQLExecDirect(hstmt, (UCHAR*)"{call atum_CreateCharacter(?,?,?,?,?, ?,?,?,?,?,\
// 																	?,?,?,?,?, ?,?,?,?,?,\
// 																	?,?,?,?,?, ?,?,?,?,?,\
// 																	?,?,?,?,?, ?,?,?,?,?,\
// 																	?,?,?,?,?, ?,?,?,?)}", SQL_NTS);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0043, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		UCHAR szSqlState[MAXDBMSGBUFLEN] = "",	plm_szErrorMsg[MAXDBMSGBUFLEN] = "";
		SDWORD plm_pfNativeError = 0L;
		SWORD plm_pcbErrorMsg = 0;
		RETCODE retcode = SQLGetDiagRec(SQL_HANDLE_STMT, hstmt,
				1, szSqlState, &plm_pfNativeError,
				plm_szErrorMsg, MAXDBMSGBUFLEN - 1, &plm_pcbErrorMsg);
		if (strcmp((char*)szSqlState, "23000") == 0 )
		{
			q.pFieldIOCPSocket->ResCreateCharacter(pMsg, ERR_COMMON_CHARACTER_ALREADY_EXISTS);
		}
		else
		{
			// 2009-01-29 by cmkwon, ���ڿ� ���� �˻��� �ʿ���� LIKE ����� ���� - ������ GetSqlPattern() ó������ �ʰ� ����
			//q.pFieldIOCPSocket->SendErrorMessage(T_FC_CHARACTER_CREATE, ERR_COMMON_UNKNOWN_ERROR, 0, 0, GetSqlPattern(pMsg->CharacterName, spbuf));
			q.pFieldIOCPSocket->SendErrorMessage(T_FC_CHARACTER_CREATE, ERR_COMMON_UNKNOWN_ERROR, 0, 0, pMsg->CharacterName);
			ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"QP_CreatCharacter Failed!\n", TRUE, q.pFieldIOCPSocket,	q);
		}

		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pMsg);

		return;
	}

// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) - �Ʒ��� ���� SELECT ����� �޴´�.
//	// Clear any result sets generated. For ouput parameter...
//	while ( ( ret = SQLMoreResults(hstmt) ) != SQL_NO_DATA );
	SQLBindCol(hstmt, 1, SQL_C_LONG, &(q.pFieldIOCPSocket->m_nNumCharacters), 0, NULL);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(hstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 �̸� ����� RowCount �̴� 
			ret = SQLMoreResults(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(hstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(hstmt);                 // 2008-07-25 by cmkwon, ���� Results �� �ִ����� �ѹ��� üũ�Ѵ�.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}
	ret = SQLFreeStmt(hstmt, SQL_CLOSE);

	// ������ ĳ���� ��ȯ
// 2009-01-29 by cmkwon, ���ڿ� ���� �˻��� �ʿ���� LIKE ����� ���� - 
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_PATTERN_BUFFER, 0, GetSqlPattern(pMsg->CharacterName, spbuf), 0, NULL);
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pMsg->CharacterName, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0042, SQL_NTS);

	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"QP_GetCharacterByName Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pMsg);
		return;
	}

	SQLBindCol(hstmt, 1, SQL_C_ULONG, &characUniqueNumber, 0, NULL);

	ret = SQLFetch(hstmt);

	if(ret == SQL_NO_DATA)
	{
		///////////////////////////////////////////////////////////////////////////
		// ĳ���Ͱ� ����, Error ó��
		q.pFieldIOCPSocket->ResCreateCharacter(pMsg, ERR_COMMON_NO_SUCH_CHARACTER);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pMsg);
		return;
	}
	else
	{
		pMsg->CharacterUniqueNumber = characUniqueNumber;
	}
	ret = SQLFreeStmt(hstmt, SQL_CLOSE);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-01-23 by cmkwon, �⺻ â�� ������ �����´�
	CHARACTER_RENDER_INFO tmRenderInfo;
	util::zero(&tmRenderInfo, sizeof(CHARACTER_RENDER_INFO));
	if(FALSE == this->GetAttachedItems(&tmRenderInfo, hstmt, characUniqueNumber))
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_GetAttachedItems2 Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
	}
	q.pFieldIOCPSocket->ResCreateCharacter(pMsg, 0, &tmRenderInfo);
	util::del(pMsg);
}

// QT_DeleteCharacter
void CAtumFieldDBManager::QP_DeleteCharacter(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FC_CHARACTER_DELETE *pMsg = (MSG_FC_CHARACTER_DELETE*)q.pQueryParam;

	RETCODE ret;
	SDWORD pcbNTS = SQL_NTS;

	//////////////////////////////////////////////////////////////////////////
	// 2007-02-21 by dhjin, ĳ���� ���� ��å���� �������� ������ ������ �ּ� ó����, dbo.atum_DeleteCharacter()���ν������� ó�� ���� 
	//	if (pMsg->CharacterUniqueNumber != 0)
	//	{
	//		///////////////////////////////////////////////////////////////////////////
	//		// delete items from store
	//		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,
	//						&pMsg->AccountUniqueNumber, 0, NULL);
	//		SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,
	//						&pMsg->CharacterUniqueNumber, 0, NULL);
	//		ret = SQLExecDirect(hstmt, (UCHAR*)"{call dbo.atum_DeleteAllCharacterItems(?, ?)}", SQL_NTS);
	//
	//		if (ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_NO_DATA)
	//		{
	//			ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_DeleteAllCharacterItems Failed!\n", TRUE, q.pFieldIOCPSocket, q);
	//			SQLFreeStmt(hstmt, SQL_CLOSE);
	//			util::del(pMsg);
	//			return;
	//		}
	//		ret = SQLFreeStmt(hstmt, SQL_CLOSE);
	//	}

	///////////////////////////////////////////////////////////////////////////////
	// ���� �Ϸ��� ĳ���Ͱ� ��������� üũ
	UID32_t retedGuildUID = 0;
	if(m_pFieldIOCP8->m_GuildWarManager.CheckGuildCommander(pMsg->CharacterUniqueNumber, &retedGuildUID))
	{
		//////////////////////////////////////////////////////////////////////////
		// 2008-10-20 by dhjin, �������� �ɸ��� ���� �Ұ��� ����
		q.pFieldIOCPSocket->SendErrorMessage(T_FC_CHARACTER_DELETE, ERR_DELETE_CHARACTER_GUILDCOMMANDER);	
		util::del(pMsg);
		return;
//
//		//////////////////////////////////////////////////////////////////////////
//		// 2007-11-09 by dhjin, ���������� ������ �����̾��ٸ� NPC������ ����
//		m_pFieldIOCP8->m_OutPostManager.OutPostPossessNPCByDeleteGuild(retedGuildUID);
//
//		INIT_MSG_WITH_BUFFER(MSG_FI_GUILD_DELETE_GUILD, T_FI_GUILD_DELETE_GUILD, pS2IMMsg, SendBuf);
//		pS2IMMsg->DeleteGuildUID = retedGuildUID;
//		m_pFieldIOCP8->m_pIMWinSocket->Write(SendBuf, MSG_SIZE(MSG_FI_GUILD_DELETE_GUILD));
//		Sleep(500);
//
//		if(m_pFieldIOCP8->m_InflWarManager.CheckLeaderCandidateUser(pMsg->AccountUniqueNumber, pMsg->CharacterUniqueNumber))
//		{// 2008-06-05 by dhjin, ������ �ĺ� ���� ����Ÿ �� ����, DB���� �������� ���� ����
//			m_pFieldIOCP8->m_InflWarManager.DeleteLeaderCandidate(pMsg->CharacterUniqueNumber);
//		}
	}
	
	int nBeforeNumCharacters = q.pFieldIOCPSocket->m_nNumCharacters;
	int nTmNumCharacters = 0;		// 2008-08-22 by cmkwon, MySQL ���� �ҽ� ���� - �ӽ� ���� ����

	///////////////////////////////////////////////////////////////////////////
	// delete character
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,	&pMsg->CharacterUniqueNumber, 0, NULL);
// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) - 
//	SQLBindParameter(hstmt, 2, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0,	&q.pFieldIOCPSocket->m_nNumCharacters, 0, NULL);
//	ret = SQLExecDirect(hstmt, (UCHAR*)"{call atum_DeleteCharacter(?, ?)}", SQL_NTS);
// 	// Clear any result sets generated. For ouput parameter...
// 	while ( ( ret = SQLMoreResults(hstmt) ) != SQL_NO_DATA );
// 	
// 	if(ret == SQL_NO_DATA)
// 	{
// 		if(nBeforeNumCharacters == q.pFieldIOCPSocket->m_nNumCharacters)
// 		{
// 			///////////////////////////////////////////////////////////////////////////
// 			// ĳ���Ͱ� ����, Error ó��
// 			q.pFieldIOCPSocket->ResDeleteCharacter(pMsg, ERR_COMMON_NO_SUCH_CHARACTER);
// 			SQLFreeStmt(hstmt, SQL_CLOSE);
// 			util::del(pMsg);
// 			return;
// 		}
// 		
// 		ret = SQL_SUCCESS;
// 	}
// 
// 	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO) {
// 		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"QP_DeleteCharacter Failed!\n", TRUE, q.pFieldIOCPSocket, q);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		util::del(pMsg);
// 		return;
// 	}

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) - SELECT �� ����� �޴´�.

	// todo : check following proc in db, was using WPs

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0045, SQL_NTS);
	if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
 		q.pFieldIOCPSocket->ResDeleteCharacter(pMsg, ERR_DB_EXECUTION_FAILED);
 		SQLFreeStmt(hstmt, SQL_CLOSE);
 		util::del(pMsg);
 		return;
	}
	SQLBindCol(hstmt, 1, SQL_C_LONG, &(nTmNumCharacters), 0, NULL);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(hstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 �̸� ����� RowCount �̴� 
			ret = SQLMoreResults(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(hstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(hstmt);                 // 2008-07-25 by cmkwon, ���� Results �� �ִ����� �ѹ��� üũ�Ѵ�.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}
	ret = SQLFreeStmt(hstmt, SQL_CLOSE);
	if(nBeforeNumCharacters <= nTmNumCharacters
		|| nTmNumCharacters == -1)		// 2008-08-22 by cmkwon, MySQL ���� �ҽ� ���� - ���� üũ
	{
		q.pFieldIOCPSocket->ResDeleteCharacter(pMsg, ERR_COMMON_NO_SUCH_CHARACTER);
		util::del(pMsg);
		return;
	}
	q.pFieldIOCPSocket->m_nNumCharacters	= nTmNumCharacters;		// 2008-08-22 by cmkwon, MySQL ���� �ҽ� ���� - �Ҵ�
	q.pFieldIOCPSocket->ResDeleteCharacter(pMsg, ERR_NO_ERROR);
	util::del(pMsg);
}

void CAtumFieldDBManager::QP_GetAccountUniqueNumber(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FC_CHARACTER_GET_ACCOUNTUNIQUENUMBER *pMsg = (MSG_FC_CHARACTER_GET_ACCOUNTUNIQUENUMBER*)q.pQueryParam;

	RETCODE	ret;
	SDWORD	pcbNTS = SQL_NTS;
	UID32_t	AccountUniqueNumber = INVALID_UNIQUE_NUMBER;

// 2009-01-29 by cmkwon, ���ڿ� ���� �˻��� �ʿ���� LIKE ����� ���� - �Ʒ��� ���� ������.
//	char spbuf[SIZE_MAX_SQL_PATTERN_BUFFER];
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_PATTERN_BUFFER, 0, GetSqlPattern(pMsg->CharacterName, spbuf), 0, &pcbNTS);
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pMsg->CharacterName, 0, &pcbNTS);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0046, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_NO_DATA) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_GetAccountUniqueNumber Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pMsg);
		return;
	}

	SQLBindCol(hstmt, 1, SQL_C_ULONG, &AccountUniqueNumber, 0, NULL);

	ret = SQLFetch(hstmt);
	if(ret == SQL_NO_DATA)
	{
		///////////////////////////////////////////////////////////////////////////
		// ĳ���Ͱ� ����, Error ó��
		q.pFieldIOCPSocket->SendErrorMessageFromFieldDB(QT_GetAccountUniqueNumber, ERR_COMMON_NO_SUCH_CHARACTER, 0, 0, pMsg->CharacterName);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pMsg);
		return;
	}
	else
	{
		INIT_MSG_WITH_BUFFER(MSG_FC_CHARACTER_GET_ACCOUNTUNIQUENUMBER_OK, T_FC_CHARACTER_GET_ACCOUNTUNIQUENUMBER_OK, pGetAccOK, SendBuf);
		util::strncpy(pGetAccOK->CharacterName, pMsg->CharacterName, SIZE_MAX_CHARACTER_NAME);
		pGetAccOK->AccountUniqueNumber = AccountUniqueNumber;
		q.pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_CHARACTER_GET_ACCOUNTUNIQUENUMBER_OK));
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pMsg);
}

// QT_GetCharacterInfoByName -> QP_GetCharacterInfoByName()
void CAtumFieldDBManager::QP_GetCharacterInfoByName(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FC_CHARACTER_GET_CHARACTER *pRMsg = (MSG_FC_CHARACTER_GET_CHARACTER*)q.pQueryParam;

	CHARACTER retCharacter;
	CHARACTER_DB_EX retCharacterDBEX;
	RETCODE ret;
	memset(&retCharacter, 0x00, sizeof(CHARACTER));

	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name:
	-- Desc:
	--====
	CREATE PROCEDURE dbo.atum_GetCharacter
		@acc	INT,
		@charac	INT
	AS
		-- // 2007-07-30 by cmkwon, SCAdminTool���� ���ܸ� ���� ��� �߰� - ĳ���� ������ �����ö� td_Guild ���̺��� �����Ͽ� ������Ʈ�� ���Ŀ� �����´�
		IF NOT EXISTS(SELECT c.UniqueNumber FROM td_character c, td_guild g, td_guildMember gm WITH(NOLOCK) WHERE c.UniqueNumber = @charac AND c.GuildUniqueNumber = g.GuildUniqueNumber AND gm.GuildUniqueNumber = c.GuildUniqueNumber AND gm.CharacterUniqueNumber = c.UniqueNumber)
		BEGIN
			UPDATE td_character
			SET GuildName = NULL, GuildUniqueNumber = 0
			FROM td_character c	WITH (NOLOCK)
			WHERE c.UniqueNumber = @charac
		END
		ELSE
		BEGIN
			UPDATE td_character
			SET GuildName = g.GuildName
			FROM td_character c, td_Guild g	WITH (NOLOCK)
			WHERE c.UniqueNumber = @charac AND c.GuildUniqueNumber = g.GuildUniqueNumber
		END

		SELECT *
		FROM td_Character
		WITH (NOLOCK)
		WHERE UniqueNumber=@charac
		-- // 2007-07-30 by cmkwon, ���� ���� CharacterUID�θ� �˻�
		--WHERE UniqueNumber=@charac AND AccountUniqueNumber = @acc
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,
					&pRMsg->AccountUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,
					&pRMsg->CharacterUniqueNumber, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0047, SQL_NTS);

	// 2007-07-30 by cmkwon, SCAdminTool���� ���ܸ� ���� ��� �߰� - �� �߰�( && ret!=SQL_NO_DATA)
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_NO_DATA) {

		// 2010-01-25 by cmkwon, �ý��� �α� �߰�
		g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::QP_GetCharacterInfoByName# 10000 (%d:%d) ret(%d) \r\n"
			, pRMsg->AccountUniqueNumber, pRMsg->CharacterUniqueNumber, ret);

		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"QP_GetCharacterInfoByName Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}

	SQLINTEGER	cb[CB_COUNT_CHARACTER];
	fill_n(cb, CB_COUNT_CHARACTER, SQL_NTS);

	// bind columns
	CAtumDBManager::BindColCharacter(hstmt, retCharacter, retCharacterDBEX, cb);

	// initialize to 0x00
	memset(&retCharacter, 0x00, sizeof(CHARACTER));

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-30 by cmkwon, MySQL ���� ����(Multi Result ó�� ����) - 
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(hstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 �̸� ����� RowCount �̴� 
			ret = SQLMoreResults(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(hstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(hstmt);                 // 2008-07-25 by cmkwon, ���� Results �� �ִ����� �ѹ��� üũ�Ѵ�.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}

		retCharacter				= retCharacterDBEX;
		_strlwr(retCharacter.AccountName);
		retCharacter.TotalGearStat	= retCharacter.GearStat;

		// 2012-10-17 by jhseol, ���� �̸����� ������ ON ��Ű��
#ifdef S_ITEM_VIEW_UNCONDITIONAL_ON
		retCharacter.SecretInfoOption = retCharacter.SecretInfoOption & USER_INFO_OPTION_ITEMINFO_DELETE_ALL_MASK;
#endif
		// end 2012-10-17 by jhseol, ���� �̸����� ������ ON ��Ű��
	}
	ret = SQLFreeStmt(hstmt, SQL_CLOSE);

// 2008-07-30 by cmkwon, MySQL ���� ����(Multi Result ó�� ����) - �Ʒ��� ���� ������	
// 	// 2007-07-30 by cmkwon, SCAdminTool���� ���ܸ� ���� ��� �߰� - �Ʒ��� ���� ������, ���� üũ
// 	//if(ret == SQL_NO_DATA)
// 	if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	if(FALSE == IS_VALID_UNIQUE_NUMBER(retCharacter.CharacterUniqueNumber))		// 2008-07-30 by cmkwon, MySQL ���� ����(Multi Result ó�� ����) - 
	{
		// 2010-01-25 by cmkwon, �ý��� �α� �߰�
		g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::QP_GetCharacterInfoByName# 20000 (%d:%d) FetchedCharacterUID(%d) \r\n"
			, pRMsg->AccountUniqueNumber, pRMsg->CharacterUniqueNumber, retCharacter.CharacterUniqueNumber);

		///////////////////////////////////////////////////////////////////////////
		// ĳ���Ͱ� ����, Error ó��
		DBGOUT("ERROR! No Character At QP_GetCharacterInfoByName(): A: %d, CH: %d\r\n", pRMsg->AccountUniqueNumber, pRMsg->CharacterUniqueNumber);
		q.pFieldIOCPSocket->ResGetCharacterInfoByName(NULL, ERR_COMMON_NO_SUCH_CHARACTER);
		util::del(pRMsg);
		return;
	}


	///////////////////////////////////////////////////////////////////////////
	// IOCP�� �־��ֱ� ���� �ʿ�! IOCP������ �� ���� �� delete�ϱ�!
	q.pFieldIOCPSocket->ResGetCharacterInfoByName(&retCharacter, 0);
	util::del(pRMsg);
}

// 2005-07-21 by cmkwon, �ٸ� �ʵ弭������ GameStart�� �����Ƿ� ������
//void CAtumFieldDBManager::QP_ConnectGameStart(FIELD_DB_QUERY q, SQLHSTMT hstmt)
//{
//	MSG_FP_CONNECT_AUTH_USER_OK *pRMsg = (MSG_FP_CONNECT_AUTH_USER_OK*)q.pQueryParam;
//
//	CFieldIOCPSocket *pFieldIOCPSocket = q.pFieldIOCPSocket;
//
//	CHARACTER retCharacter;
//	CHARACTER_DB_EX retCharacterDBEX;
//	RETCODE ret;
//
//	/*[Stored Query Definition]************************************************
//	CREATE PROCEDURE atum_GetCharacter
//		@acc	INT,
//		@charac	INT
//	AS
//	select *
//	from td_character
//	where UniqueNumber=@charac and AccountUniqueNumber = @acc
//
//	GO
//	**************************************************************************/
//	memset(&retCharacter, 0x00, sizeof(CHARACTER));
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,
//					&pRMsg->AccountUniqueNumber, 0, NULL);
//	UID32_t CharacterUniqueNumberTmp
//		= m_pFieldIOCP8->GetCharacterByArrayIndex(q.pFieldIOCPSocket->GetClientArrayIndex())->CharacterUniqueNumber;
//	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,
//					&CharacterUniqueNumberTmp , 0, NULL);
//
//	ret = SQLExecDirect(hstmt, (UCHAR*)"{call dbo.atum_GetCharacter(?, ?)}", SQL_NTS);
//
//	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO ) {
//		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"QP_ConnectGameStart Failed!\n", TRUE, q.pFieldIOCPSocket, q);
//		SQLFreeStmt(hstmt, SQL_CLOSE);
//		util::del(pRMsg);
//		return;
//	}
//
//	SQLINTEGER	cb[CB_COUNT_CHARACTER] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
//											,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
//											,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
//											,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
//											,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
//											,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
//											,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
//											,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
//											,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
//											,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
//											,SQL_NTS,SQL_NTS};
//
//	// bind columns
//	CAtumDBManager::BindColCharacter(hstmt, retCharacter, retCharacterDBEX, cb);
//
//	// initialize to 0x00
//	memset(&retCharacter, 0x00, sizeof(CHARACTER));
//	ret = SQLFetch(hstmt);
//	retCharacter				= retCharacterDBEX;
//	_strlwr(retCharacter.AccountName);
//	retCharacter.TotalGearStat	= retCharacter.GearStat;
//
//	if(ret == SQL_NO_DATA)
//	{
//		///////////////////////////////////////////////////////////////////////////
//		// ĳ���Ͱ� ����, Error ó��
//		q.pFieldIOCPSocket->ResConnectGameStart(NULL, ERR_COMMON_NO_SUCH_CHARACTER);
//		SQLFreeStmt(hstmt, SQL_CLOSE);
//		util::del(pRMsg);
//		return;
//	}
//	else
//	{
//		///////////////////////////////////////////////////////////////////////////
//		// IOCP�� �־��ֱ� ���� �ʿ�!
//		q.pFieldIOCPSocket->ResConnectGameStart(&retCharacter, 0);
//	}
//
//	SQLFreeStmt(hstmt, SQL_CLOSE);
//
//	util::del(pRMsg);
//}

// QT_GetAllCharacterInfoByID
void CAtumFieldDBManager::QP_GetAllCharacterInfoByID(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FP_CONNECT_AUTH_USER_OK *pRMsg = (MSG_FP_CONNECT_AUTH_USER_OK*)q.pQueryParam;
	int birthday = 0; // START 2011-11-03 by shcho, yedang �˴ٿ��� ���� - DB���� ���� �޾ƿ� ����

	///////////////////////////////////////////////////////////////////////////////
	// 2009-07-07 by cmkwon, �ε� ���ʷ����� ���� �ذ� - 
	if(FALSE == IS_VALID_UNIQUE_NUMBER(pRMsg->AccountUniqueNumber))
	{// 2009-07-07 by cmkwon, üũ �߰�

#if defined(_ATUM_LOAD_TEST)
		if(0 == strnicmp(pRMsg->AccountName, ATUM_LOAD_TEST_PREFIX_ACCOUNT_NAME, ATUM_LOAD_TEST_PREFIX_ACCOUNT_NAME_SIZE))
		{
			SQLINTEGER	pcbNTS = SQL_NTS;
			SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, pRMsg->AccountName , 0, &pcbNTS);
			SQLRETURN ret = SQLExecDirect(hstmt, PROCEDURE_090707_0396, SQL_NTS);
			if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_NO_DATA)
			{
				ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"PROCEDURE_090707_0396 Failed! \r\n", TRUE, q.pFieldIOCPSocket, q);
				SQLFreeStmt(hstmt, SQL_CLOSE);
				q.pFieldIOCPSocket->SendErrorMessage(T_FC_CONNECT_LOGIN, ERR_PROTOCOL_INVALID_ACCOUNT_UNIQUENUMBER, pRMsg->AccountUniqueNumber, 0, pRMsg->AccountName);
				util::del(pRMsg);
				return;
			}
			SQLBindCol(hstmt, 1, SQL_C_ULONG, &(pRMsg->AccountUniqueNumber), 0, NULL);
			SQLBindCol(hstmt, 2, SQL_C_ULONG, &birthday, 0, NULL);// START 2011-11-03 by shcho, yedang �˴ٿ��� ����
			ret = SQLFetch(hstmt);
			SQLFreeStmt(hstmt, SQL_CLOSE);
		}
#endif // END - #if defined(_ATUM_LOAD_TEST)

		if(FALSE == IS_VALID_UNIQUE_NUMBER(pRMsg->AccountUniqueNumber))
		{
			q.pFieldIOCPSocket->SendErrorMessage(T_FC_CONNECT_LOGIN, ERR_PROTOCOL_INVALID_ACCOUNT_UNIQUENUMBER, pRMsg->AccountUniqueNumber, 0, pRMsg->AccountName);
			util::del(pRMsg);
			return;
		}
	}

	// START 2011-11-03 by shcho, yedang �˴ٿ��� ���� - üũ ��¥ ��������
	BOOL tmpShutDownMINS;
	
#ifdef SC_SHUT_DOWNMIN_SHCHO
	ATUM_DATE_TIME tmpDate;
	ATUM_DATE_TIME currentDate;
	ShutdownUserData ShutDownData;
	currentDate.SetCurrentDateTime();	// ���� �ð��� ������
	
	SQLINTEGER	pcbNTS = SQL_NTS;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, pRMsg->AccountName , 0, &pcbNTS);
	SQLRETURN ret1 = SQLExecDirect(hstmt, PROCEDURE_090707_0396, SQL_NTS);
	if ( ret1!=SQL_SUCCESS && ret1!=SQL_SUCCESS_WITH_INFO && ret1!=SQL_NO_DATA)
	{
					ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"PROCEDURE_090707_0396 Failed! \r\n", TRUE, q.pFieldIOCPSocket, q);
					SQLFreeStmt(hstmt, SQL_CLOSE);
					q.pFieldIOCPSocket->SendErrorMessage(T_FC_CONNECT_LOGIN, ERR_PROTOCOL_INVALID_ACCOUNT_UNIQUENUMBER, pRMsg->AccountUniqueNumber, 0, pRMsg->AccountName);
					util::del(pRMsg);
					return;
	}
	SQLBindCol(hstmt, 1, SQL_C_ULONG, &(pRMsg->AccountUniqueNumber), 0, NULL);
	SQLBindCol(hstmt, 2, SQL_C_ULONG, &birthday, 0, NULL);
	ret1 = SQLFetch(hstmt);
	SQLFreeStmt(hstmt, SQL_CLOSE);

	DbgOut("[2-2]QP_GetAllCharacterInfoByID Running Checked Year:[%d] \r\n",birthday);
#endif // SC_SHUT_DOWNMIN_SHCHO
	// END 2011-11-03 by shcho, yedang �˴ٿ��� ���� -	üũ ��¥ ��������

	int nCharacterCount = 0;
	CHARACTER retCharacter;
	CHARACTER_DB_EX retCharacterDBEX;
	RETCODE ret;

	char buffer[SIZE_MAX_PACKET];
	MSG_FC_CONNECT_LOGIN_OK *pSMsg;

	pSMsg = (MSG_FC_CONNECT_LOGIN_OK*)buffer;
	memset(&retCharacter, 0x00, sizeof(CHARACTER));

	// set account unique number
	pSMsg->AccountUniqueNumber = pRMsg->AccountUniqueNumber;

	/**********************************************************************
	-- !!!!
	-- Name:
	-- Desc:
	-- ====
	-- // 2009-01-29 by cmkwon, ���ڿ� ���� �˻��� �ʿ���� LIKE ����� ���� - �Ʒ��� ���� ������.
	-- CREATE PROCEDURE dbo.atum_GetAllCharacterInfoByID
	-- 	@ID	VARCHAR(64)
	-- AS
	-- 	SELECT *
	-- 	FROM td_Character
	-- 	WITH (NOLOCK)
	-- 	WHERE td_Character.AccountName LIKE @ID AND 0 = Race & 0x4000	-- Race >= 16384 ������ �ɸ���
	-- GO
	CREATE PROCEDURE dbo.atum_GetAllCharacterInfoByID
		@i_AccUID		-- // 2009-01-29 by cmkwon, ���ڿ� ���� �˻��� �ʿ���� LIKE ����� ���� - AccountUID �� �˻����� ����
	AS
		SELECT *
		FROM dbo.td_Character
		WITH (NOLOCK)
		WHERE AccountUniqueNumber = @i_AccUID AND 0 = Race & 0x4000	-- Race >= 16384 ������ �ɸ���
	GO
	**********************************************************************/

	// ĳ���� ���� ������ ����
// 2009-01-29 by cmkwon, ���ڿ� ���� �˻��� �ʿ���� LIKE ����� ���� - 
//	char spbuf[SIZE_MAX_SQL_PATTERN_BUFFER];
//	_strlwr(pRMsg->AccountName);
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_PATTERN_BUFFER, 0,
//					GetSqlPattern(pRMsg->AccountName,spbuf), 0, NULL);

	// 2009-01-29 by cmkwon, ���ڿ� ���� �˻��� �ʿ���� LIKE ����� ���� - �Ʒ��� ���� AccountUID�� �˻��ϴ� ������ ����
	_strlwr(pRMsg->AccountName);
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->AccountUniqueNumber, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0048, SQL_NTS);

	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"QP_GetAllCharacterInfoByID Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}

	SQLINTEGER	cb[CB_COUNT_CHARACTER];
	fill_n(cb, CB_COUNT_CHARACTER, SQL_NTS);

	// bind columns
	CAtumDBManager::BindColCharacter(hstmt, retCharacter, retCharacterDBEX, cb);

	BYTE bySelectableInfluenceMask		= INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI;		// ���Լ���, �ݶ�����
	nCharacterCount = 0;
	// initialize to 0x00
	memset(&retCharacter, 0x00, sizeof(CHARACTER));
	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA)
	{
		retCharacter				= retCharacterDBEX;
		_strlwr(retCharacter.AccountName);
		retCharacter.TotalGearStat	= retCharacter.GearStat;
		bySelectableInfluenceMask	= retCharacter.SelectableInfluenceMask;

		// 2012-10-17 by jhseol, ���� �̸����� ������ ON ��Ű��
#ifdef S_ITEM_VIEW_UNCONDITIONAL_ON
		retCharacter.SecretInfoOption = retCharacter.SecretInfoOption & USER_INFO_OPTION_ITEMINFO_DELETE_ALL_MASK;
#endif
		// end 2012-10-17 by jhseol, ���� �̸����� ������ ON ��Ű��

		pSMsg->Characters[nCharacterCount].CharacterUniqueNumber	= retCharacter.CharacterUniqueNumber;
		pSMsg->Characters[nCharacterCount].Color					= retCharacter.Color;
		util::strncpy(pSMsg->Characters[nCharacterCount].CharacterName, retCharacter.CharacterName, SIZE_MAX_CHARACTER_NAME);
		pSMsg->Characters[nCharacterCount].CharacterName[SIZE_MAX_CHARACTER_NAME-1] = '\0';
		pSMsg->Characters[nCharacterCount].PilotFace				= retCharacter.PilotFace;
		pSMsg->Characters[nCharacterCount].Race						= retCharacter.Race;
		pSMsg->Characters[nCharacterCount].UnitKind					= retCharacter.UnitKind;
		pSMsg->Characters[nCharacterCount].RacingPoint				= retCharacter.RacingPoint;		// 2009-11-02 by cmkwon, ĳ��(�κ�/â�� Ȯ��) ������ �߰� ���� - CAtumFieldDBManager::QP_GetAllCharacterInfoByID#, 
		
		// START 2011-11-03 by shcho, yedang �˴ٿ��� ���� - ���������� �߰��� �޾ƿ´�.
		// ���⼭ ���� ��¥�� ���Ѵ�. ���ϱ� �̼����ڸ� �����Ͽ� �̼����ڸ� TRUE, �ƴϸ� FALSE�� �з��Ѵ�.

#ifdef SC_SHUT_DOWNMIN_SHCHO
		DbgOut("[3-1]QP_GetAllCharacterInfoByID Running AccountUniqueNumber:[%d] , Year:[%d] TRUE!!\r\n",pSMsg->AccountUniqueNumber, birthday);	 // START 2011-11-03 by shcho, yedang �˴ٿ��� ����
		if( g_pFieldGlobal->checkSHUTDOWNMINORS(birthday,currentDate) ) // �̼����ڴ�.
		{
			memset(&ShutDownData, 0x00, sizeof(ShutdownUserData));
			ShutDownData.Account_UniqueNumber	  = pSMsg->AccountUniqueNumber;
			ShutDownData.CharacterUniqueNumber[nCharacterCount] = retCharacter.CharacterUniqueNumber; 
			vectShutDownUserData::iterator iter(m_pFieldIOCP8->m_ShutDownData.begin());
			BOOL pushCheck = FALSE;
			for(;iter !=m_pFieldIOCP8->m_ShutDownData.end(); iter++ )
			{
				if(pSMsg->AccountUniqueNumber==iter->Account_UniqueNumber)
				{	// ���� ���� �ִ�.
					DbgOut("[3-2-1]QP_GetAllCharacterInfoByID PushBack False!! ");	 // START 2011-11-03 by shcho, yedang �˴ٿ��� ����
					pushCheck = TRUE;
					break;
				}
			}
			// ������ �־��ش�.
			if(FALSE == pushCheck)
			{
				DbgOut("[3-2-2]QP_GetAllCharacterInfoByID PushBack m_ShutDownData AccountUniqueNumber:[%d] Year:[%d] TRUE!!\r\n",pSMsg->AccountUniqueNumber, birthday);	 // START 2011-11-03 by shcho, yedang �˴ٿ��� ����
			m_pFieldIOCP8->m_ShutDownData.push_back(ShutDownData); // �̼����ڸ� ����Ʈ�� ����Ѵ�.
			}
			tmpShutDownMINS = TRUE;	// �ϴ� �⵵�� �̼������̹Ƿ�
		}
		else
#endif // SC_SHUT_DOWNMIN_SHCHO
		{
			DbgOut("[3-3]QP_GetAllCharacterInfoByID Running AccountUniqueNumber:[%d] , Year:[%d] FALSE!!\r\n", pSMsg->AccountUniqueNumber, birthday); // START 2011-11-03 by shcho, yedang �˴ٿ��� ����
			tmpShutDownMINS = FALSE;
		}
		pSMsg->Characters[nCharacterCount].ShutDownMINS				= tmpShutDownMINS;
		// END 2011-11-03 by shcho, yedang �˴ٿ��� ���� - ���������� �߰��� �޾ƿ´�.
	
		nCharacterCount++;
#ifdef _ATUM_LOAD_TEST
		pSMsg->AccountUniqueNumber									= retCharacter.AccountUniqueNumber;
#endif // END - _ATUM_LOAD_TEST

		memset(&retCharacter, 0x00, sizeof(CHARACTER));		// initialize to 0x00
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	
	// START 2011-11-03 by shcho, yedang �˴ٿ��� ���� - ���������� �߰��� �޾ƿ´�.
	// ���⼭ ���� ��¥�� ���Ѵ�. ���ϱ� �̼����ڸ� �����Ͽ� �̼����ڸ� TRUE, �ƴϸ� FALSE�� �з��Ѵ�.
	// �ɸ��Ͱ� ��� ������ �ؾ��Ѵ�. �ű������� ���� ó���� ���Ͽ�
#ifdef SC_SHUT_DOWNMIN_SHCHO
	DbgOut("[4-1]QP_GetAllCharacterInfoByID Running AccountUniqueNumber:[%d] , Year:[%d] TRUE!!\r\n",pSMsg->AccountUniqueNumber, birthday);	 // START 2011-11-03 by shcho, yedang �˴ٿ��� ����
	if( g_pFieldGlobal->checkSHUTDOWNMINORS(birthday,currentDate) ) // �̼����ڴ�.
	{
		memset(&ShutDownData, 0x00, sizeof(ShutdownUserData));
		ShutDownData.Account_UniqueNumber	  = pSMsg->AccountUniqueNumber;
		ShutDownData.CharacterUniqueNumber[0] = 0; 
		vectShutDownUserData::iterator iter(m_pFieldIOCP8->m_ShutDownData.begin());
		BOOL pushCheck = FALSE;
		for(;iter !=m_pFieldIOCP8->m_ShutDownData.end(); iter++ )
		{
			if(pSMsg->AccountUniqueNumber==iter->Account_UniqueNumber)
			{	// ���� ���� �ִ�.
				DbgOut("[4-2-1]QP_GetAllCharacterInfoByID PushBack False!! ");	 // START 2011-11-03 by shcho, yedang �˴ٿ��� ����
				pushCheck = TRUE;
				break;
			}
		}
		// ������ �־��ش�.
		if(FALSE == pushCheck)
		{
			pSMsg->Characters[0].ShutDownMINS = TRUE;	// �ϴ� �⵵�� �̼������̹Ƿ�

			DbgOut("[4-2-2]QP_GetAllCharacterInfoByID PushBack m_ShutDownData AccountUniqueNumber:[%d] Year:[%d] TRUE!!\r\n",pSMsg->AccountUniqueNumber, birthday);	 // START 2011-11-03 by shcho, yedang �˴ٿ��� ����
			m_pFieldIOCP8->m_ShutDownData.push_back(ShutDownData); // �̼����ڸ� ����Ʈ�� ����Ѵ�.
		}

	}
	else

	{
		pSMsg->Characters[0].ShutDownMINS = FALSE;	// �ϴ� �⵵�� �̼������̹Ƿ�

		DbgOut("[4-3]QP_GetAllCharacterInfoByID Running AccountUniqueNumber:[%d] , Year:[%d] FALSE!!\r\n", pSMsg->AccountUniqueNumber, birthday); // START 2011-11-03 by shcho, yedang �˴ٿ��� ����
	}

	// 2012-02-10 by shcho, yedang �˴ٿ��� ���� - �α� �߰� ���
	// ���ӽ� �˴ٿ� Ȯ���� ���� �̼���,���� ���� �ѹ� ��� �Ѵ�.
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[notify] QP_GetAllCharacterInfoByID ShutdownData! AccountUniqueNumber:[%d] , birthday:[%d], ShutDownMinsCheck[%d] 0:ADULT 1:Not ADULT(Minor)\r\n", pSMsg->AccountUniqueNumber, birthday, pSMsg->Characters[0].ShutDownMINS);
#endif // SC_SHUT_DOWNMIN_SHCHO
	///////////////////////////////////////////////////////////////////////////////
	// 2007-04-09 by cmkwon
	if(g_pFieldGlobal->GetIsJamboreeServer()
		&& 0 >= nCharacterCount)
	{
		q.pFieldIOCPSocket->SendErrorMessage(T_FC_CONNECT_LOGIN, ERR_JAMBOREE_NO_SUCH_CHARACTER);
		util::del(pRMsg);
		return;		
	}

	// ������ ������ �Ҵ��ϱ�
	for (int i = 0; i < nCharacterCount; i++)
	{
		BOOL bReted = GetAttachedItems(&(pSMsg->Characters[i].CharacterRenderInfo), hstmt, pSMsg->Characters[i].CharacterUniqueNumber);
		if(FALSE == bReted)
		{
			ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_GetAttachedItems Failed!\n", TRUE, q.pFieldIOCPSocket, q);
			SQLFreeStmt(hstmt, SQL_CLOSE);
			util::del(pRMsg);
			return;
		}
	}

	SCASH_PREMEIUM_CARD_INFO	tmCardInfo;
	tmCardInfo.ResetCASH_PREMEIUM_CARD_INFO();
	///////////////////////////////////////////////////////////////////////////////
	// ����ȭ �����̾� ī�� �ε�
	/********************************************************************************
	-- 2005-01-20 by cmkwon, ������ ���� �������� �����´�
	CREATE PROCEDURE atum_GetAccountCashStoreItem
		@i_accNum		INT					-- AccountUniqueNumber
	AS
		SELECT * FROM td_accountCashStore
			WHERE AccountUniqueNumber = @i_accNum
	GO
	********************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->AccountUniqueNumber, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0049, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_GetAccountCashStoreItem Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	
	
	SQL_TIMESTAMP_STRUCT		arrSqlTime[3];
	util::zero(arrSqlTime, sizeof(arrSqlTime[0])*3);

	SQLBindCol(hstmt, 1, SQL_C_SBIGINT, &tmCardInfo.n64UniqueNumber10, 0, &cb[0]);
	SQLBindCol(hstmt, 2, SQL_C_LONG, &tmCardInfo.nAccountUID10, 0, &cb[1]);
	SQLBindCol(hstmt, 3, SQL_C_LONG, &tmCardInfo.nCardItemNum, 0, &cb[2]);
	SQLBindCol(hstmt, 4, SQL_C_TIMESTAMP, &arrSqlTime[0], 0, &cb[3]);
	SQLBindCol(hstmt, 5, SQL_C_TIMESTAMP, &arrSqlTime[1], 0, &cb[4]);
	SQLBindCol(hstmt, 6, SQL_C_TIMESTAMP, &arrSqlTime[2], 0, &cb[5]);
	ret = SQLFetch(hstmt);
	SQLFreeStmt(hstmt, SQL_CLOSE);
	tmCardInfo.atumTimeCreatedTime	= arrSqlTime[0];
	tmCardInfo.atumTimeUpdatedTime	= arrSqlTime[1];
	tmCardInfo.atumTimeExpireTime	= arrSqlTime[2];

	if(tmCardInfo.n64UniqueNumber10)
	{// ����ȭ ī�尡 �����Ѵ� - �Ⱓ ���� üũ

		ATUM_DATE_TIME	tmCurATime;
		tmCurATime.SetCurrentDateTime();

		if(tmCardInfo.atumTimeExpireTime <= tmCurATime)
		{// ����� ������ - DB���� ī�� �����ؾ���

			QPARAM_CASH_DELETE_PREMIUM_CARD *pQParam = new QPARAM_CASH_DELETE_PREMIUM_CARD;
			util::zero(pQParam, sizeof(QPARAM_CASH_DELETE_PREMIUM_CARD));
			pQParam->nAccountUID			= tmCardInfo.nAccountUID10;
			this->MakeAndEnqueueQuery(QT_CashDeletePremiumCard, q.pFieldIOCPSocket, tmCardInfo.nAccountUID10, pQParam);

			///////////////////////////////////////////////////////////////////////////////
			// �ʱ�ȭ
			util::zero(&tmCardInfo, sizeof(tmCardInfo));
		}
	}
	// end_����ȭ �����̾� ī�� �ε�
	///////////////////////////////////////////////////////////////////////////////
	
	pSMsg->NumCharacters = nCharacterCount;
	//util::strncpy(pSMsg->VoIP1to1ServerIP, g_pFieldGlobal->GetIPVoIP1to1Server(), SIZE_MAX_IPADDRESS);
	//pSMsg->VoIP1to1ServerPort = g_pFieldGlobal->GetPortVoIP1to1Server();
	//util::strncpy(pSMsg->VoIPNtoNServerIP, g_pFieldGlobal->GetIPVoIPNtoNServer(), SIZE_MAX_IPADDRESS);
	//pSMsg->VoIPNtoNServerPort = g_pFieldGlobal->GetPortVoIPNtoNServer();
#ifdef S_WEB_CASHSHOP_JHSEOL
	pSMsg->BIsTestServer = g_pFieldGlobal->IsTestServer();					// 2012-11-27 by khkim, �׽�Ʈ �������� �Ϲ� �������� ����
	// 2012-11-28 by jhseol, �������� ĳ�� WEB ���� - ������ ����ϴ� ��ȣ
	char tmDBnum[2];
	char tmDBName[SIZE_MAX_ODBC_CONN_STRING];
	util::zero(tmDBnum, sizeof(tmDBnum));
	util::strncpy(tmDBName, g_pFieldGlobal->GetDBServerDatabaseName(), SIZE_MAX_ODBC_CONN_STRING);
	tmDBnum[0]=tmDBName[9];
	tmDBnum[1]=tmDBName[10];
	pSMsg->DBNum = atoi(tmDBnum);
	// end 2012-11-28 by jhseol, �������� ĳ�� WEB ���� - ������ ����ϴ� ��ȣ
#endif
	q.pFieldIOCPSocket->SetAccountInfo(pRMsg);
	q.pFieldIOCPSocket->ResGetAllCharacterInfoByID(pSMsg, &tmCardInfo, bySelectableInfluenceMask, 0);
	util::del(pRMsg);
}

// QT_SaveCharacterCriticalData
void CAtumFieldDBManager::QP_SaveCharacterCriticalData(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_CHARACTER_SAVE_CRITICAL_DATA *pMsgSaveData
		= (QPARAM_CHARACTER_SAVE_CRITICAL_DATA*)q.pQueryParam;

	RETCODE ret;

	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name:
	-- Desc:
	--		2006-04-10 by cmkwon, �Ķ���� �߰�(@i_DownSPIOnDeath	INT,			-- 2006-04-10 by cmkwon)
	--		2006-11-15 by cmkwon, ���� 2�� �߰�
	--						@i_GameContinueTimeInSecondOfToday	INT,			-- 2006-11-15 by cmkwon, ���� �Ϸ� ���� ���� �ð�
	--						@i_LastGameEndDate					VARCHAR(30)		-- 2006-11-15 by cmkwon, ������ ���� ���� ��¥�ð�
	--====
	CREATE PROCEDURE dbo.atum_SaveCharacterCriticalData
		@i_CharacterUID		INT,
		@i_Level			TINYINT,
		@i_CharacterMode	TINYINT,
		@i_Experience		FLOAT,
		@i_DownExperience	FLOAT,
		@i_DownSPIOnDeath	INT,			-- 2006-04-10 by cmkwon
		@i_BodyCondition	BIGINT,
		@i_PetLevel			TINYINT,
		@i_PetExperience	FLOAT,
		@i_Position_X		FLOAT,
		@i_Position_Y		FLOAT,
		@i_Position_Z		FLOAT,
		@i_MapIndex			SMALLINT,
		@i_ChannelIndex		SMALLINT,
		@i_HP				SMALLINT,
		@i_DP				SMALLINT,
		@i_SP				SMALLINT,
		@i_EP				SMALLINT,
		@i_currentHP		FLOAT,
		@i_currentDP		FLOAT,
		@i_currentSP		SMALLINT,
		@i_currentEP		FLOAT,
		@i_totalPlayTime	BIGINT,
		@i_GameContinueTimeInSecondOfToday	INT,			-- 2006-11-15 by cmkwon, ���� �Ϸ� ���� ���� �ð�
		@i_LastGameEndDate					VARCHAR(30),	-- 2006-11-15 by cmkwon, ������ ���� ���� ��¥�ð�
		@i_PCBangtotalPlayTime	BIGINT,						-- 2007-06-07 by dhjin, PC�� �� �÷��� �ð�
		@i_SecretInfoOption		INT				-- // 2008-06-23 by dhjin, EP3 ���������ɼ�
	AS
		UPDATE td_Character
		SET	Level			= @i_Level,
			CharacterMode	= @i_CharacterMode,
			Experience		= @i_Experience,
			DownExperience	= @i_DownExperience,
			DownSPIOnDeath	= @i_DownSPIOnDeath,		-- 2006-04-10 by cmkwon
			BodyCondition	= @i_BodyCondition,
			PetLevel		= @i_PetLevel,
			PetExperience	= @i_PetExperience,
			Position_X		= @i_Position_X,
			Position_Y		= @i_Position_Y,
			Position_Z		= @i_Position_Z,
			MapIndex		= @i_MapIndex,
			ChannelIndex	= @i_ChannelIndex,
			HP				= @i_HP,
			DP				= @i_DP,
			SP				= @i_SP,
			EP				= @i_EP,
			currentHP		= @i_currentHP,
			currentDP		= @i_currentDP,
			currentSP		= @i_currentSP,
			currentEP		= @i_currentEP,
			TotalPlayTime	= @i_totalPlayTime,
			PCBangTotalPlayTime = @i_PCBangtotalPlayTime,
			SecretInfoOption	= @i_SecretInfoOption				-- // 2008-06-23 by dhjin, EP3 ���������ɼ�
		WHERE UniqueNumber = @i_CharacterUID
		
		--------------------------------------------------------------------------------
		-- 2006-11-15 by cmkwon
		-- td_account ���̺� �������ӽð�, ������ ���� ����ð��� ������Ʈ �Ѵ�.
		UPDATE td_Account
			SET GameContinueTimeInSecondOfToday = @i_GameContinueTimeInSecondOfToday,
				LastGameEndDate = @i_LastGameEndDate
			FROM td_Account a, td_character c
			WHERE c.UniqueNumber = @i_CharacterUID AND a.AccountUniqueNumber = c.AccountUniqueNumber

	GO
	**************************************************************************/
	SQLINTEGER arrCB[28] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
							,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
							,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};		// 2008-06-23 by dhjin, EP3 ���������ɼ� -
	char tmpTimeBuf[SIZE_MAX_SQL_DATETIME_STRING];													// 2006-11-15 by cmkwon
	pMsgSaveData->LastGameEndDate.GetSQLDateTimeString(tmpTimeBuf, SIZE_MAX_SQL_DATETIME_STRING);	// 2006-11-15 by cmkwon

	SQLBindParameter(hstmt,  1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pMsgSaveData->CharacterUID, 0,		&arrCB[1]);
	SQLBindParameter(hstmt,  2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pMsgSaveData->Level, 0,			&arrCB[2]);
	SQLBindParameter(hstmt,  3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pMsgSaveData->CharacterMode0, 0,	&arrCB[3]);
	SQLBindParameter(hstmt,  4, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_FLOAT, 0, 0, &pMsgSaveData->Experience, 0,			&arrCB[4]);
	SQLBindParameter(hstmt,  5, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_FLOAT, 0, 0, &pMsgSaveData->DownExperience, 0,		&arrCB[5]);
	SQLBindParameter(hstmt,  6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pMsgSaveData->DownSPIOnDeath0, 0,		&arrCB[6]);		// 2006-04-10 by cmkwon
	SQLBindParameter(hstmt,  7, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &pMsgSaveData->BodyCondition, 0,		&arrCB[7]);
	SQLBindParameter(hstmt,  8, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pMsgSaveData->PetLevel, 0,			&arrCB[8]);
	SQLBindParameter(hstmt,  9, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_FLOAT, 0, 0, &pMsgSaveData->PetExperience, 0,		&arrCB[9]);
	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pMsgSaveData->Position_X, 0,			&arrCB[10]);
	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pMsgSaveData->Position_Y, 0,			&arrCB[11]);
	SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pMsgSaveData->Position_Z, 0,			&arrCB[12]);
	SQLBindParameter(hstmt, 13, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pMsgSaveData->MapIndex, 0,			&arrCB[13]);
	SQLBindParameter(hstmt, 14, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pMsgSaveData->ChannelIndex, 0,		&arrCB[14]);
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	SQLBindParameter(hstmt, 15, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pMsgSaveData->HP, 0, &arrCB[15]);
	SQLBindParameter(hstmt, 16, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pMsgSaveData->DP, 0, &arrCB[16]);
#else
	SQLBindParameter(hstmt, 15, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pMsgSaveData->HP, 0, &arrCB[15]);
	SQLBindParameter(hstmt, 16, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pMsgSaveData->DP, 0, &arrCB[16]);
#endif
	SQLBindParameter(hstmt, 17, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pMsgSaveData->SP, 0,				&arrCB[17]);
	SQLBindParameter(hstmt, 18, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pMsgSaveData->EP, 0,				&arrCB[18]);
	SQLBindParameter(hstmt, 19, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pMsgSaveData->CurrentHP, 0,				&arrCB[19]);
	SQLBindParameter(hstmt, 20, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pMsgSaveData->CurrentDP, 0,				&arrCB[20]);
	SQLBindParameter(hstmt, 21, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pMsgSaveData->CurrentSP, 0,			&arrCB[21]);
	SQLBindParameter(hstmt, 22, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pMsgSaveData->CurrentEP, 0,				&arrCB[22]);
	SQLBindParameter(hstmt, 23, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pMsgSaveData->TotalPlayTime, 0,		&arrCB[23]);
	SQLBindParameter(hstmt, 24, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pMsgSaveData->GameContinueTimeInSecondOfToday, 0, &arrCB[24]);	// 2006-11-15 by cmkwon	
	SQLBindParameter(hstmt, 25, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpTimeBuf, 0, &arrCB[25]);			// 2006-11-15 by cmkwon
	SQLBindParameter(hstmt, 26, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pMsgSaveData->PCBangTotalPlayTime, 0,		&arrCB[26]);	// 2007-06-07 by dhjin
	SQLBindParameter(hstmt, 27, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pMsgSaveData->SecretInfoOption, 0,		&arrCB[27]);	// 2008-06-23 by dhjin, EP3 ���������ɼ� -

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0050, SQL_NTS);	// 2008-06-23 by dhjin, EP3 ���������ɼ� -

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO ) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_SaveCharacterCriticalData Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pMsgSaveData);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pMsgSaveData);
}

void CAtumFieldDBManager::QP_ChangeUnitKind(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_CHARACTER_CHANGE_UNITKIND *pMsg
		= (QPARAM_CHARACTER_CHANGE_UNITKIND*)q.pQueryParam;

	RETCODE ret;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,
					&pMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0,
					&pMsg->UnitKind, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0051, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO ) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_ChangeUnitKind Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pMsg);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pMsg);
}

// QT_ChangeStat
void CAtumFieldDBManager::QP_ChangeStat(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_CHARACTER_CHANGE_STAT *pMsg	= (QPARAM_CHARACTER_CHANGE_STAT*)q.pQueryParam;

	RETCODE ret;

	if(FALSE == IS_VALID_UNIQUE_NUMBER(pMsg->CharacterUniqueNumber))
	{// 2009-04-20 by cmkwon, �ý��� �α� �߰� - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] Parameter error !! QP_ChangeStat# %s %d \r\n"
			, GetCharacterString(q.pFieldIOCPSocket->GetCharacter(), string()), pMsg->CharacterUniqueNumber);
		util::del(pMsg);
		return;
	}

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pMsg->byAutoStatType, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pMsg->GearStat1.AttackPart, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pMsg->GearStat1.DefensePart, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pMsg->GearStat1.FuelPart, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pMsg->GearStat1.SoulPart, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pMsg->GearStat1.ShieldPart, 0, NULL);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pMsg->GearStat1.DodgePart, 0, NULL);
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pMsg->HP, 0, NULL);
	SQLBindParameter(hstmt, 10,SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pMsg->DP, 0, NULL);
	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pMsg->EP, 0, NULL);
	SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pMsg->SP, 0, NULL);
	SQLBindParameter(hstmt, 13, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pMsg->BonusStat, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0052, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_ChangeStat Failed!\r\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pMsg);
}

// QT_ChangeBonusStatPoint
void CAtumFieldDBManager::QP_ChangeBonusStatPoint(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_CHARACTER_CHANGE_BONUSSTATPOINT *pMsg	= (QPARAM_CHARACTER_CHANGE_BONUSSTATPOINT*)q.pQueryParam;

	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name:
	-- Desc: 2005-11-15 by cmkwon, Level Up �̿��� ������� �߰��� ���ʽ� ���� ��ġ ���� ��
	--====
	CREATE PROCEDURE atum_ChangeBonusStatPoint
		@UniqueNumber		INT,
		@BonusStatPoint		TINYINT
	AS
		UPDATE td_Character
			SET BonusStatPoint = @BonusStatPoint
			WHERE  UniqueNumber = @UniqueNumber;
	GO
	**************************************************************************/
	RETCODE ret;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pMsg->BonusStatPoint, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0053, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_ChangeBonusStatPoint Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pMsg);
}

void CAtumFieldDBManager::QP_ChangeGuild(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_CHARACTER_CHANGE_GUILD *pMsg
		= (QPARAM_CHARACTER_CHANGE_GUILD*)q.pQueryParam;

	RETCODE ret;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,
					&pMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_GUILD_NAME, 0,
					pMsg->Guild, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0054, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO ) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_ChangeGuild Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pMsg);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pMsg);
}

// QT_ChangeExp
void CAtumFieldDBManager::QP_ChangeExp(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_CHARACTER_CHANGE_EXP *pMsg
		= (QPARAM_CHARACTER_CHANGE_EXP*)q.pQueryParam;

	RETCODE ret;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_FLOAT, 0, 0, &pMsg->Experience, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0055, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_ChangeExp Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pMsg);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pMsg);
}

void CAtumFieldDBManager::QP_ChangeLevel(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_CHARACTER_CHANGE_LEVEL *pMsg
		= (QPARAM_CHARACTER_CHANGE_LEVEL*)q.pQueryParam;

	RETCODE ret;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,
					&pMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0,
					&pMsg->Level, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0,
					&pMsg->BonusStat, 0, NULL);
// 2005-11-15 by cmkwon, ������
//	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0,
//					&pMsg->BonusSkillPoint, 0, NULL);
//	ret = SQLExecDirect(hstmt, (UCHAR*)"{call dbo.atum_ChangeLevel(?, ?, ?, ?)}", SQL_NTS);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0056, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO ) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_ChangeLevel Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pMsg);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pMsg);
}

void CAtumFieldDBManager::QP_ChangeBodyCondition(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_CHARACTER_CHANGE_BODYCONDITION *pMsg
		= (QPARAM_CHARACTER_CHANGE_BODYCONDITION*)q.pQueryParam;

	RETCODE ret;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,
					&pMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0,
					&pMsg->BodyCondition, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0057, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_NO_DATA) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_ChangeBodyCondition Failed!\n", TRUE, q.pFieldIOCPSocket, q);
#ifdef _DEBUG
		DBGOUT("  %s's BodyCondition: %ld\r\n", GetCharacterString(&q.pFieldIOCPSocket->m_character, string()), pMsg->BodyCondition);
#endif
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pMsg);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pMsg);
}

/*
// void CAtumFieldDBManager::QP_ChangePropensity(FIELD_DB_QUERY q, SQLHSTMT hstmt)
// {
// 	QPARAM_CHARACTER_CHANGE_PROPENSITY *pMsg
// 		= (QPARAM_CHARACTER_CHANGE_PROPENSITY*)q.pQueryParam;
// 
// 	RETCODE ret;
// 
// 	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,
// 					&pMsg->CharacterUniqueNumber, 0, NULL);
// 	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0,
// 					&pMsg->Propensity, 0, NULL);
// 	ret = SQLExecDirect(hstmt, (UCHAR*)"{call dbo.atum_ChangePropensity(?, ?)}", SQL_NTS);
// 
// 	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO ) {
// 		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_ChangePropensity Failed!\n", TRUE, q.pFieldIOCPSocket, q);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		util::del(pMsg);
// 		return;
// 	}
// 
// 	util::del(pMsg);
// }
*/

void CAtumFieldDBManager::QP_ChangeInfluenceType(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_CHARACTER_CHANGE_INFLUENCE_TYPE *pMsg
		= (QPARAM_CHARACTER_CHANGE_INFLUENCE_TYPE*)q.pQueryParam;

	RETCODE ret;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pMsg->AccountUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pMsg->InfluenceType0, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pMsg->SelectableInfluenceMask0, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0058, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO ) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_ChangeInfluenceType Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pMsg);
}

// QT_ChangeStatus
void CAtumFieldDBManager::QP_ChangeStatus(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_CHARACTER_CHANGE_STATUS *pMsg
		= (QPARAM_CHARACTER_CHANGE_STATUS*)q.pQueryParam;

	RETCODE ret;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pMsg->Status, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0059, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO ) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_ChangeStatus Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pMsg);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pMsg);
}

// QT_ChangePKPoint
void CAtumFieldDBManager::QP_ChangePKPoint(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_CHARACTER_CHANGE_PKPOINT *pMsg
		= (QPARAM_CHARACTER_CHANGE_PKPOINT*)q.pQueryParam;

	RETCODE ret;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,
					&pMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0,
					&pMsg->Propensity, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0,
					&pMsg->PKWinPoint, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0,
					&pMsg->PKLossPoint, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0060, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO ) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_ChangePKPoint Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pMsg);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pMsg);
}

// 2009-11-02 by cmkwon, ĳ��(�κ�/â�� Ȯ��) ������ �߰� ���� - ������� ����
// void CAtumFieldDBManager::QP_ChangeRacingPoint(FIELD_DB_QUERY q, SQLHSTMT hstmt)
// {
// 	QPARAM_CHARACTER_CHANGE_RACINGPOINT *pQPChangeRacingPoint
// 		= (QPARAM_CHARACTER_CHANGE_RACINGPOINT*)q.pQueryParam;
// 
// 	RETCODE ret;
// 
// 	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPChangeRacingPoint->CharacterUniqueNumber, 0, NULL);
// 	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pQPChangeRacingPoint->RacingPoint, 0, NULL);
// 	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0061, SQL_NTS);
// 
// 	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO ) {
// 		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_ChangeRacingPoint Failed!\n", TRUE, q.pFieldIOCPSocket, q);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		util::del(pQPChangeRacingPoint);
// 		return;
// 	}
// 
// 	SQLFreeStmt(hstmt, SQL_CLOSE);
// 	util::del(pQPChangeRacingPoint);
// }

void CAtumFieldDBManager::QP_UpdateTotalPlayTime(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_CHARACTER_UPDATE_TOTAL_PLAY_TIME *pQPTotalPlayTime
		= (QPARAM_CHARACTER_UPDATE_TOTAL_PLAY_TIME*)q.pQueryParam;

	RETCODE ret;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPTotalPlayTime->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pQPTotalPlayTime->TotalPlayTime, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0062, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO ) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateTotalPlayTime Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPTotalPlayTime);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPTotalPlayTime);
}

void CAtumFieldDBManager::QP_UpdateLastStartedTime(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	UID32_t nCharacterUID = q.nGeneralParam1;

	RETCODE ret;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &nCharacterUID, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0063, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA ) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateLastStartedTime Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
}

// QT_ChangeHPDPSPEP
void CAtumFieldDBManager::QP_ChangeHPDPSPEP(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_CHARACTER_CHANGE_HPDPSPEP *pMsgChangeHPDPSPEP
		= (QPARAM_CHARACTER_CHANGE_HPDPSPEP*)q.pQueryParam;

	RETCODE ret;

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_ChangeHPDPSPEP
		@i_CharacterUID		INT,
		@i_HP				SMALLINT,
		@i_DP				SMALLINT,
		@i_SP				SMALLINT,
		@i_EP				SMALLINT,
		@i_CurrentHP		FLOAT,
		@i_CurrentDP		FLOAT,
		@i_CurrentSP		SMALLINT,
		@i_CurrentEP		FLOAT
	AS
		UPDATE td_character
			SET HP = @i_HP, DP = @i_DP, SP = @i_SP, EP = @i_EP,
				CurrentHP = @i_CurrentHP, CurrentDP = @i_CurrentDP, CurrentSP = @i_CurrentSP, CurrentEP = @i_CurrentEP
			WHERE  UniqueNumber = @i_CharacterUID;
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pMsgChangeHPDPSPEP->CharacterUniqueNumber, 0, NULL);
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pMsgChangeHPDPSPEP->HP, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pMsgChangeHPDPSPEP->DP, 0, NULL);
#else
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pMsgChangeHPDPSPEP->HP, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pMsgChangeHPDPSPEP->DP, 0, NULL);
#endif
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pMsgChangeHPDPSPEP->SP, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pMsgChangeHPDPSPEP->EP, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pMsgChangeHPDPSPEP->CurrentHP, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pMsgChangeHPDPSPEP->CurrentDP, 0, NULL);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pMsgChangeHPDPSPEP->CurrentSP, 0, NULL);
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pMsgChangeHPDPSPEP->CurrentEP, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0064, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO  && ret!=SQL_NO_DATA) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_ChangeHPDPSPEP Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pMsgChangeHPDPSPEP);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pMsgChangeHPDPSPEP);
}

// QT_ChangeCurrentHPDPSPEP
void CAtumFieldDBManager::QP_ChangeCurrentHPDPSPEP(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_CHARACTER_CHANGE_CURRENTHPDPSPEP *pMsg
		= (QPARAM_CHARACTER_CHANGE_CURRENTHPDPSPEP*)q.pQueryParam;

	RETCODE ret;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pMsg->CurrentHP, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pMsg->CurrentDP, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pMsg->CurrentSP, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pMsg->CurrentEP, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0065, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO  && ret!=SQL_NO_DATA) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_ChangeCurrentHPDPSPEP Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pMsg);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pMsg);
}

void CAtumFieldDBManager::QP_ChangeMapChannel(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_CHARACTER_CHANGE_MAPCHANNEL *pMsg
		= (QPARAM_CHARACTER_CHANGE_MAPCHANNEL*)q.pQueryParam;

	RETCODE ret;

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_ChangeMapChannel
		@UniqueNumber	INT,
		@MapIndex		SMALLINT,
		@ChannelIndex	SMALLINT
	AS
	UPDATE td_character
		SET MapIndex = @MapIndex, ChannelIndex = @ChannelIndex
		WHERE  UniqueNumber = @UniqueNumber;
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,
					&pMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0,
					&pMsg->MapChannelIndex.MapIndex, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0,
					&pMsg->MapChannelIndex.ChannelIndex, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0066, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_NO_DATA) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_ChangeMapChannel Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pMsg);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pMsg);
}

void CAtumFieldDBManager::QP_ChangePetInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_CHARACTER_CHANGE_PETINFO *pMsg
		= (QPARAM_CHARACTER_CHANGE_PETINFO*)q.pQueryParam;

	RETCODE ret;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pMsg->Level, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_FLOAT, 0, 0, &pMsg->Experience, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0067, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO ) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_ChangePetInfo Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pMsg);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pMsg);
}

void CAtumFieldDBManager::QP_ChangePosition(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_CHARACTER_CHANGE_POSITION *pMsg
		= (QPARAM_CHARACTER_CHANGE_POSITION*)q.pQueryParam;

	RETCODE ret;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0,
					&pMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0,
					&pMsg->PositionVector.x , 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0,
					&pMsg->PositionVector.y, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0,
					&pMsg->PositionVector.z, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0068, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_NO_DATA) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_ChangePosition Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pMsg);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pMsg);
}

/*
// store(item) table ���� �Ŀ� ������
// void CAtumFieldDBManager::QP_ChangeDockingItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
// {
// 	QPARAM_CHARACTER_CHANGE_DOCKINGITEM *pMsg
// 		= (QPARAM_CHARACTER_CHANGE_DOCKINGITEM*)q.pQueryParam;
// 
// 	RETCODE ret;
// 
// 	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0,
// 					&pMsg->CharacterUniqueNumber, 0, NULL);
// 	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0,
// 					&pMsg->CurrentPrimaryWeapon, 0, NULL);
// 	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0,
// 					&pMsg->CurrentSecondaryWeapon, 0, NULL);
// 	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0,
// 					&pMsg->Armor, 0, NULL);
// 
// 	ret = SQLExecDirect(hstmt, (UCHAR*)"{call dbo.atum_ChangeDockingItem(?, ?, ?, ?)}", SQL_NTS);
// 
// 	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO ) {
// 		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_ChangeDockingItem Failed!\n", TRUE, q.pFieldIOCPSocket, q);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		util::del(pMsg);
// 		return;
// 	}
// 
// 	util::del(pMsg);
// }
*/



// QT_UpdateItemPossess
void CAtumFieldDBManager::QP_UpdateItemPossess(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	SQLRETURN ret;

	/*[Stored Query Definition]********************************************
	CREATE PROCEDURE atum_UpdateItemPossess
		@i_ItemUniqueNumber		BIGINT,
		@i_ToPossess			INT		-- CharacterUniqueNumber OR 0
	AS
		UPDATE td_store
		SET Possess = @i_ToPossess
		WHERE UniqueNumber = @i_ItemUniqueNumber
	GO
	**********************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &q.nGeneralParam2, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0069, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO ) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateItemPossess Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_UpdateItemStorage(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		QT_UpdateItemStorage
/// \author		cmkwon
/// \date		2005-12-07 ~ 2005-12-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_UpdateItemStorage(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	SQLRETURN ret;
	QPARAM_STORE_UPDATE *pParam = (QPARAM_STORE_UPDATE*)q.pQueryParam;
	
	if(NULL == q.pFieldIOCPSocket
		|| FALSE == q.pFieldIOCPSocket->IsValidCharacter(FALSE))
	{
		util::del(pParam);
		return;
	}

	/*[Stored Query Definition]********************************************
	--!!!!
	-- Name:
	-- Desc:	2005-12-07 by cmkwon, �������� ���� ��ġ ����(ĳ�����κ�->â��, â��->ĳ�����κ�)
	--====
	CREATE PROCEDURE atum_UpdateItemStorage
		@i_ItemUniqueNumber		BIGINT,
		@i_ItemCharacterUID     INT,		// 2006-09-19 by dhjin, ���õ� �ɸ���
		@i_ItemStorage			TINYINT		-- 0(ITEM_IN_CHARACTER):ĳ�����κ�, 1(ITEM_IN_STORE):â��
	AS
		UPDATE td_Store
			SET ItemStorage = @i_ItemStorage, Possess = @i_ItemCharacterUID
			WHERE UniqueNumber = @i_ItemUniqueNumber
	GO

	**********************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &pParam->ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pParam->CharacterUID, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pParam->ItemStorage, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0070, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO ) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateItemStorage Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pParam);		// 2012-10-14 by hskim, �޸� ���� ����
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pParam);		// 2012-10-14 by hskim, �޸� ���� ����
}

// QT_LoadOneItem
void CAtumFieldDBManager::QP_LoadOneItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	// 2013-05-22 by jhseol, �ҽ����� - �ּ�����
}

// QT_GetStoreItem -> CAtumFieldDBManager::QP_GetStoreItem()
// 1. ItemGereral or ItemGeneralInStore Loaing
// 2. ItemSkill Loading(�ʱ� �ε� �ÿ��� �ش��)
// 3. Enchant Loading
// 4. Quest Loading(�ʱ� �ε� �ÿ��� �ش��)

// start 2011-08-25 by shcho, Ƚ���� ������ ���ޱ�� ����
void CAtumFieldDBManager::QP_InsertLoginItemEvent(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	SQLINTEGER cb[4] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	QPARAM_CHECK_LOGINEVENTITEM *pParam = (QPARAM_CHECK_LOGINEVENTITEM*)q.pQueryParam;

	SQLRETURN ret = 0;
	ret = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pParam->EventUID,		0, &cb[0]);
	ret = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pParam->AccountUID,	0, &cb[1]);
	ret = SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pParam->CompleteFlag,	0, &cb[2]);

	BOOL bRet = SQLExecDirect(hstmt, PROCEDURE_LOGINITEMEVENT_110825_0001, SQL_NTS);
	if( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "atum_InsertLoginItemEvent Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pParam);
		return;
	}
	
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pParam);

}
// end 2011-08-25 by shcho, Ƚ���� ������ ���ޱ�� ����

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_CheckEventItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		
/// \author		dhjin
/// \date		2006-08-25 ~ 2006-08-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_CheckEventItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_CheckEventItem
	SQLINTEGER cb1 = SQL_NTS;
	QPARAM_CHECK_EVENTITEM *pParam = (QPARAM_CHECK_EVENTITEM*)q.pQueryParam;

	// 2013-04-22 by jhseol, ������ �̺�Ʈ - ����Ÿ�� �߰����� �ý��� ����
	BOOL bPermissionArenaState = FALSE;
#ifdef S_ITEM_EVENT_SUB_TYPE_JHSEOL
	if ( ITEM_IN_STORE == pParam->InsertStorage )
	{
		bPermissionArenaState = TRUE;
	}
#endif
	// end 2013-04-22 by jhseol, ������ �̺�Ʈ - ����Ÿ�� �߰����� �ý��� ����
	
	if(NULL == q.pFieldIOCPSocket
		|| FALSE == q.pFieldIOCPSocket->IsValidCharacter(FALSE, bPermissionArenaState))	// 2013-04-22 by jhseol, ������ �̺�Ʈ - ����Ÿ�� �߰����� �ý��� ����
	{
		util::del(pParam);
		return;
	}

	/*[Stored Query Definition]****************************************
	-------------------------------------------------------------------------------
	-- �̺�Ʈ ������ ���� ���� ���� �˻��ϴ� ���ν���
	-- // 2009-11-19 by cmkwon, ������ �̺�Ʈ�� Account/Character üũ ��� �߰� - 
	CALL dbo.atum_GetEventItemList
	******************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pParam->AccountUID, 0,	 NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pParam->ItemEventUID, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pParam->CharacterUID, 0, NULL);		// 2009-11-19 by cmkwon, ������ �̺�Ʈ�� Account/Character üũ ��� �߰� - 
	
	BOOL bRet = SQLExecDirect(hstmt,	PROCEDURE_080822_0071, SQL_NTS);	
	if ( bRet!=SQL_SUCCESS && bRet!=SQL_SUCCESS_WITH_INFO )
	{
		char szSysLog[1024];
		sprintf(szSysLog, "[DB Error]: QP_CheckEventItem error, AccountUID(%d) ItemEventUID(%I64d)\r\n", pParam->AccountUID, pParam->ItemEventUID);
		DbgOut(szSysLog);
		g_pFieldGlobal->WriteSystemLog(szSysLog);
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "atum_GetEventItemList Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pParam);
		return;
	}
	
	SQL_TIMESTAMP_STRUCT	GetTime0;
	SQLBindCol(hstmt, 1, SQL_C_TIMESTAMP, &GetTime0, 0, &cb1);
	SQLRETURN	sqlRet = SQLFetch(hstmt);
	if(SQL_SUCCESS != sqlRet && SQL_SUCCESS_WITH_INFO != sqlRet && SQL_NO_DATA != sqlRet)
	{
		char szSysLog[1024];
		sprintf(szSysLog, "[DB Error]: QP_CheckEventItem error, AccountUID(%d) ItemEventUID(%I64d)\r\n", pParam->AccountUID, pParam->ItemEventUID);
		DbgOut(szSysLog);
		g_pFieldGlobal->WriteSystemLog(szSysLog);
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "atum_GetEventItemList Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pParam);
		return;					// 2008-01-10 by cmkwon, ������ ����
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	// 2008-01-10 by cmkwon, �ѹ��� üũ �Ѵ�.

	if(NULL == q.pFieldIOCPSocket
		|| FALSE == q.pFieldIOCPSocket->IsValidCharacter(FALSE, bPermissionArenaState))	// 2013-04-22 by jhseol, ������ �̺�Ʈ - ����Ÿ�� �߰����� �ý��� ����
	{
		util::del(pParam);
		return;
	}


#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, �������� �����߰�
	if( SQL_NO_DATA != sqlRet && ITEM_EVENT_TYPE_ONLYONE == pParam->ItemEventType && pParam->UseFixedPeriod )
	{
		ATUM_DATE_TIME tmpGetTimeDB;
		tmpGetTimeDB.operator=(GetTime0); // ���� ��� ����� �ð��� ��ȯ 

		tmpGetTimeDB.AddDateTime(0,0, pParam->FixedPeriod_DAY,0,0,0);

		ATUM_DATE_TIME tmpCurrentDate;
		tmpCurrentDate.SetCurrentDateTime();
	
		if(  tmpGetTimeDB < tmpCurrentDate )	// �������� ���� �޾Ҿ, ������ �Ⱓ �̳��� ���� �ʾ����� ���� 
		{
			if(q.pFieldIOCPSocket->InsertEventItem(pParam->ItemNum, pParam->Count, IUT_GIVEEVENTITEM, pParam->PrefixCodeNum, pParam->SuffixCodeNum, pParam->InsertStorage))	// 2013-04-22 by jhseol, ������ �̺�Ʈ - ����Ÿ�� �߰����� �ý��� ����
			{
				ATUM_DATE_TIME CurrentTime;
				CurrentTime.SetCurrentDateTime();
				QPARAM_INSERT_EVENTITEM *pQParam = new QPARAM_INSERT_EVENTITEM;
				pQParam->AccountUID		= pParam->AccountUID;
				pQParam->CharacterUID	= q.pFieldIOCPSocket->GetCharacter()->CharacterUniqueNumber;
				pQParam->ItemEventUID	= pParam->ItemEventUID;
				pQParam->InsertItemTime	= CurrentTime;
				MakeAndEnqueueQuery(QT_UpdateEventItemFixedPeriod, q.pFieldIOCPSocket, q.pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);// ������ �����Ͱ� �����Ƿ� ������Ʈ(�ð���)
			}
			util::del(pParam);
			return;	
		}
	}
#endif

	///////////////////////////////////////////////////////////////////////////////
	// 2008-01-10 by cmkwon, ������ �̺�Ʈ �ý��ۿ� �� ���� �ý��� �߰� - ���� �̺�Ʈ ó��
	if(ITEM_EVENT_TYPE_COUPON_ONLYONE == pParam->ItemEventType
		|| ITEM_EVENT_TYPE_COUPON_ONEDAY == pParam->ItemEventType)
	{
		// 2008-01-10 by cmkwon, �ش� ���� �̺�Ʈ ������ �������� üũ
		if(SQL_NO_DATA != sqlRet)
		{
			ATUM_DATE_TIME		CurrentTime { true };
			if( ITEM_EVENT_TYPE_COUPON_ONLYONE == pParam->ItemEventType
				|| (CurrentTime.Year == GetTime0.year && CurrentTime.Month == GetTime0.month && CurrentTime.Day == GetTime0.day) )
			{
				// 2008-01-10 by cmkwon, ���� �̺�Ʈ�� �Ұ��� �ϴ�
				util::del(pParam);
				return;
			}
		}

		///////////////////////////////////////////////////////////////////////////////
		// 2008-01-10 by cmkwon, CFieldIOCPSocket �� ������ ���� �̺�Ʈ�� �߰��Ѱ� Ŭ���̾�Ʈ�� �����Ѵ�.
		if(FALSE == q.pFieldIOCPSocket->AddCouponEvent(pParam))
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::QP_CheckEventItem_ AddCouponEvent Error !!, %s CouponEventUID(%d)\r\n"
				, GetCharacterString(q.pFieldIOCPSocket->GetCharacter(), string()), pParam->ItemEventUID);
		}
		util::del(pParam);
		return;
	}

	if(SQL_NO_DATA == sqlRet)
	{
		if(q.pFieldIOCPSocket->InsertEventItem(pParam->ItemNum, pParam->Count, IUT_GIVEEVENTITEM, pParam->PrefixCodeNum, pParam->SuffixCodeNum, pParam->InsertStorage))	// 2013-04-22 by jhseol, ������ �̺�Ʈ - ����Ÿ�� �߰����� �ý��� ����
		{
			ATUM_DATE_TIME CurrentTime;
			CurrentTime.SetCurrentDateTime();
			QPARAM_INSERT_EVENTITEM *pQParam = new QPARAM_INSERT_EVENTITEM;
			pQParam->AccountUID		= pParam->AccountUID;
			pQParam->CharacterUID	= q.pFieldIOCPSocket->GetCharacter()->CharacterUniqueNumber;
			pQParam->ItemEventUID	= pParam->ItemEventUID;
			pQParam->InsertItemTime	= CurrentTime;
			MakeAndEnqueueQuery(QT_InsertEventItem, q.pFieldIOCPSocket, q.pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
		}
		util::del(pParam);
		return;
	}

	// 2013-03-29 by jhseol, ������ �̺�Ʈ - �ſ� ù ����� ��������
#ifdef S_MONTHL_CASH_BUY_GIFT_JHSEOL
	if ( ITEM_EVENT_TYPE_FIRST_PURCHASE == pParam->ItemEventType )
	{
		ATUM_DATE_TIME		CurrentTime(TRUE);
		if ( IS_MASANG_IP(q.pFieldIOCPSocket->GetPeerIP()) || (CurrentTime.Year > GetTime0.year) || (CurrentTime.Year == GetTime0.year && CurrentTime.Month > GetTime0.month) )
		{
			if(q.pFieldIOCPSocket->InsertEventItem(pParam->ItemNum, pParam->Count, IUT_GIVEEVENTITEM, pParam->PrefixCodeNum, pParam->SuffixCodeNum, pParam->InsertStorage))	// 2013-04-22 by jhseol, ������ �̺�Ʈ - ����Ÿ�� �߰����� �ý��� ����
			{
				QPARAM_INSERT_EVENTITEM *pQParam = new QPARAM_INSERT_EVENTITEM;
				pQParam->AccountUID		= pParam->AccountUID;
				pQParam->CharacterUID	= q.pFieldIOCPSocket->GetCharacter()->CharacterUniqueNumber;
				pQParam->ItemEventUID	= pParam->ItemEventUID;
				pQParam->InsertItemTime	= CurrentTime;
				MakeAndEnqueueQuery(QT_InsertEventItem, q.pFieldIOCPSocket, q.pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[S_MONTHL_CASH_BUY_GIFT_JHSEOL][Notify] CAtumFieldDBManager::QP_CheckEventItem,   AUID(%8d) CUID(%8d) Insert Event Item ItemNum(%d|%d)\r\n"
					, q.pFieldIOCPSocket->m_character.AccountUniqueNumber, q.pFieldIOCPSocket->m_character.CharacterUniqueNumber, pParam->ItemNum, pParam->Count);
			}
		}
		util::del(pParam);
		return;
	}
#endif
	// end 2013-03-29 by jhseol, ������ �̺�Ʈ - �ſ� ù ����� ��������
	
	// 2007-08-01 by cmkwon, �ڿ� �߰��� �̺�ƮŸ��(ITEM_EVENT_TYPE_INFLCHANGE, ITEM_EVENT_TYPE_LEVELUP)�� ������ �ѹ��� ó���Ǿ�� �Ѵ�.
	//if (ITEM_EVENT_TYPE_ONLYONE == pParam->ItemEventType)
	if(ITEM_EVENT_TYPE_ONEDAY != pParam->ItemEventType)
	{
		util::del(pParam);
		return;
	}
	
	ATUM_DATE_TIME CurrentTime { true };
	if ( (CurrentTime.Year != GetTime0.year) ||
		 (CurrentTime.Month != GetTime0.month) ||
		 (CurrentTime.Day != GetTime0.day) )
	{
		if(q.pFieldIOCPSocket->InsertEventItem(pParam->ItemNum, pParam->Count, IUT_GIVEEVENTITEM, pParam->PrefixCodeNum, pParam->SuffixCodeNum, pParam->InsertStorage))	// 2013-04-22 by jhseol, ������ �̺�Ʈ - ����Ÿ�� �߰����� �ý��� ����
		{
			ATUM_DATE_TIME CurrentTime;
			CurrentTime.SetCurrentDateTime();
			QPARAM_INSERT_EVENTITEM *pQParam = new QPARAM_INSERT_EVENTITEM;
			pQParam->AccountUID		= pParam->AccountUID;
			pQParam->CharacterUID	= q.pFieldIOCPSocket->GetCharacter()->CharacterUniqueNumber;
			pQParam->ItemEventUID	= pParam->ItemEventUID;
			pQParam->InsertItemTime	= CurrentTime;
			MakeAndEnqueueQuery(QT_InsertEventItem, q.pFieldIOCPSocket, q.pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
		}
	}
	util::del(pParam);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_InsertEventItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		
/// \author		dhjin
/// \date		2006-08-25 ~ 2006-08-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_InsertEventItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QP_InsertEventItem
	QPARAM_INSERT_EVENTITEM *pParam = (QPARAM_INSERT_EVENTITEM*)q.pQueryParam;
	/*[Stored Query Definition]****************************************
	-------------------------------------------------------------------------------
	-- �̺�Ʈ ������ ���� ���� ���� ����ϴ� ���ν���
	-- // 2009-11-19 by cmkwon, ������ �̺�Ʈ�� Account/Character üũ ��� �߰� - 
	CALL dbo.atum_insert_Log_ItemEvent
	******************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pParam->AccountUID, 0,	 NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pParam->CharacterUID, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pParam->ItemEventUID, 0, NULL);
	char tmpTimeString[SIZE_MAX_SQL_DATETIME_STRING];
	pParam->InsertItemTime.GetSQLDateTimeString(tmpTimeString, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, &tmpTimeString, 0, NULL);
	
	BOOL bRet = SQLExecDirect(hstmt, PROCEDURE_080822_0072, SQL_NTS);
	
	if ( bRet!=SQL_SUCCESS && bRet!=SQL_SUCCESS_WITH_INFO && bRet != SQL_NO_DATA)
	{
		char szSysLog[1024];
		sprintf(szSysLog, "[DB Error]: QP_InsertEventItem error, AccountUID(%d) CharacterUID(%d) ItemEventUID(%I64d)\r\n"
			, pParam->AccountUID, pParam->CharacterUID, pParam->ItemEventUID);
		DbgOut(szSysLog);
		g_pFieldGlobal->WriteSystemLog(szSysLog);
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "atum_insert_Log_ItemEvent Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pParam);
		return;
	}
	
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pParam);
}

// 2013-02-28 by bckim, �������� �����߰�
void CAtumFieldDBManager::QP_UpdateEventItemFixedPeriod(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_INSERT_EVENTITEM *pParam = (QPARAM_INSERT_EVENTITEM*)q.pQueryParam;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pParam->AccountUID, 0,	 NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pParam->CharacterUID, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pParam->ItemEventUID, 0, NULL);
	char tmpTimeString[SIZE_MAX_SQL_DATETIME_STRING];
	pParam->InsertItemTime.GetSQLDateTimeString(tmpTimeString, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, &tmpTimeString, 0, NULL);
	
	BOOL bRet = SQLExecDirect(hstmt, PROCEDURE_130228_0001, SQL_NTS);
	
	if ( bRet!=SQL_SUCCESS && bRet!=SQL_SUCCESS_WITH_INFO && bRet != SQL_NO_DATA)
	{
		char szSysLog[1024];
		sprintf(szSysLog, "[DB Error]: QT_UpdateEventItemFixedPeriod error, AccountUID(%d) CharacterUID(%d) ItemEventUID(%I64d)\r\n"
			, pParam->AccountUID, pParam->CharacterUID, pParam->ItemEventUID);
		DbgOut(szSysLog);
		g_pFieldGlobal->WriteSystemLog(szSysLog);
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "atum_update_Log_ItemEvent_fixed_period Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pParam);
		return;
	}
	
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pParam);
}
// end 2013-02-28 by bckim, �������� �����߰�

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_CheckCouponEvent(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		// 2008-01-10 by cmkwon, ������ �̺�Ʈ �ý��ۿ� �� ���� �ý��� �߰� - CAtumFieldDBManager::QP_CheckCouponEvent() �߰�
/// \author		cmkwon
/// \date		2008-01-10 ~ 2008-01-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_CheckCouponEvent(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_CheckCouponEvent *pParam = (QPARAM_CheckCouponEvent*)q.pQueryParam;

	if(NULL == q.pFieldIOCPSocket
		|| FALSE == q.pFieldIOCPSocket->IsValidCharacter(FALSE))
	{
		util::del(pParam);
		return;
	}

	if(FALSE == q.pFieldIOCPSocket->FindCouponEvent(pParam->ItemEventUID))
	{// 2008-01-10 by cmkwon, ��ȿ�� ItemEventUID üũ
		q.pFieldIOCPSocket->SendErrorMessage(T_FC_EVENT_COUPON_EVENT_USE_COUPON, ERR_INVALID_ITEMEVENT_UID, 500);
		util::del(pParam);
		return ;
	}

	/*[Stored Query Definition]****************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_UseCouponNumber
	-- DESC				: // 2008-01-10 by cmkwon, ������ �̺�Ʈ �ý��ۿ� �� ���� �ý��� �߰� - dbo.atum_UseCouponNumber �߰�
	--						���� ��ȣ�� ��밡���� ������ȣ ���� üũ�ϰ� �����ϸ� ���Ȱ����� ������Ʈ �Ѵ�.
	--						#define ERR_PROTOCOL_QUEST_COUPON_INVALID_NUMBER				0x2710		// ������ȣ�� DB�� �������� �ʰų�
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_UseCouponNumber
		@i_CouponNumber				VARCHAR(30),
		@i_AccName					VARCHAR(20),
		@i_UseTime					VARCHAR(30)
	AS
		DECLARE @CouponUID INT
		SET @CouponUID = (SELECT UniqueNumber FROM atum2_db_account.dbo.td_couponlist WITH(NOLOCK) WHERE CouponNumber = @i_CouponNumber AND UsedFlag = 0)
		IF @CouponUID IS NULL
		BEGIN
			-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
			--RETURN 0x2710
			--SELECT 0x2710; // 2008-10-07 by cmkwon, 0x2710�Ұ����Ͽ� 10000���� ����
			SELECT 10000;	
			RETURN;
		END

		UPDATE atum2_db_account.dbo.td_couponlist
		SET UsedFlag = 1, UsedAccountName = @i_AccName, UsedTime = @i_UseTime
		WHERE UniqueNumber = @CouponUID
		
		-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
		--RETURN 0
		SELECT 0;
	GO	
	******************************************************************/
	int nRetErrCode			= ERR_NO_ERROR;
	SQLINTEGER arrCB[5]		= {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
// 	SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nRetErrCode, 0,								&arrCB[1]);
// 	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_COUPON_NUMBER, 0, pParam->CouponNumber, 0,	&arrCB[2]);
// 	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, pParam->AccName, 0,			&arrCB[3]);
// 	ATUM_DATE_TIME tmCur(TRUE);
// 	char tmpTimeString[SIZE_MAX_SQL_DATETIME_STRING];
// 	tmCur.GetSQLDateTimeString(tmpTimeString, SIZE_MAX_SQL_DATETIME_STRING);
// 	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpTimeString, 0,		&arrCB[4]);
// 	
// 	// 2008-07-08 by cmkwon, MySQL �������� ����(���ν������� ����ó�� �Ұ��� ����) - 
// 	//SQLRETURN bRet = SQLExecDirect(hstmt, (UCHAR*)"{? = call atum_UseCouponNumber(?, ?, ?)}", SQL_NTS);	
// 	SQLRETURN bRet = SQLExecDirect(hstmt, (UCHAR*)"{call atum_UseCouponNumber(?, ?, ?, ?)}", SQL_NTS);	
// 	if ( bRet!=SQL_SUCCESS && bRet!=SQL_SUCCESS_WITH_INFO && bRet != SQL_NO_DATA)
// 	{
// 		q.pFieldIOCPSocket->SendErrorMessage(T_FC_EVENT_COUPON_EVENT_USE_COUPON, ERR_DB_EXECUTION_FAILED, 510);
// 		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "atum_UseCouponNumber_ Failed!\r\n", TRUE, q.pFieldIOCPSocket, q);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		util::del(pParam);
// 		return;
// 	}
// 	// Clear any result sets generated.
// 	while ( ( bRet = SQLMoreResults(hstmt) ) != SQL_NO_DATA );
// 	SQLFreeStmt(hstmt, SQL_CLOSE);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_COUPON_NUMBER, 0, pParam->CouponNumber, 0,	&arrCB[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, pParam->AccName, 0,			&arrCB[2]);
	ATUM_DATE_TIME tmCur { true };
	char tmpTimeString[SIZE_MAX_SQL_DATETIME_STRING];
	tmCur.GetSQLDateTimeString(tmpTimeString, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpTimeString, 0,		&arrCB[3]);
	
	SQLRETURN bRet = SQLExecDirect(hstmt, PROCEDURE_080822_0073, SQL_NTS);	
	if ( bRet!=SQL_SUCCESS && bRet!=SQL_SUCCESS_WITH_INFO && bRet != SQL_NO_DATA)
	{
		q.pFieldIOCPSocket->SendErrorMessage(T_FC_EVENT_COUPON_EVENT_USE_COUPON, ERR_DB_EXECUTION_FAILED, 510);
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "atum_UseCouponNumber_ Failed!\r\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pParam);
		return;
	}
	SQLBindCol(hstmt, 1, SQL_C_LONG, &(nRetErrCode), 0, NULL);

	RETCODE ret = 0;
	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(hstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 �̸� ����� RowCount �̴� 
			ret = SQLMoreResults(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(hstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(hstmt);                 // 2008-07-25 by cmkwon, ���� Results �� �ִ����� �ѹ��� üũ�Ѵ�.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	if(FALSE == q.pFieldIOCPSocket->IsValidCharacter(FALSE))
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::QP_CheckCouponEvent_ !!, AccountName(%s) CouponNumber(%s) ItemEventUID(%d)\r\n"
			, pParam->AccName, pParam->CouponNumber, pParam->ItemEventUID);
		util::del(pParam);
		return;
	}

	if(ERR_NO_ERROR != nRetErrCode)
	{
		q.pFieldIOCPSocket->SendErrorMessage(T_FC_EVENT_COUPON_EVENT_USE_COUPON, nRetErrCode, 520, 0, pParam->CouponNumber);
		util::del(pParam);
		return;
	}
	
	QPARAM_CHECK_EVENTITEM couponEv;		util::zero(&couponEv, sizeof(couponEv));	
	if(FALSE == q.pFieldIOCPSocket->PopCouponEvent(&couponEv, pParam->ItemEventUID))
	{// 2008-01-10 by cmkwon, ���� �̺�Ʈ ���� �������� �����ϱ�
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::QP_CheckCouponEvent_ PopCouponEvent error!!, AccountName(%s) CouponNumber(%s) ItemEventUID(%d) %s\r\n"
			, pParam->AccName, pParam->CouponNumber, pParam->ItemEventUID, GetCharacterString(q.pFieldIOCPSocket->GetCharacter(), string()));
		q.pFieldIOCPSocket->SendErrorMessage(T_FC_EVENT_COUPON_EVENT_USE_COUPON, ERR_PROTOCOL_QUEST_COUPON_INVALID_NUMBER, 530);
		util::del(pParam);
		return;
	}
	if(FALSE == q.pFieldIOCPSocket->InsertEventItem(couponEv.ItemNum, couponEv.Count, IUT_GIVEEVENTITEM_COUPONEVENT, couponEv.PrefixCodeNum, couponEv.SuffixCodeNum))
	{// 2008-01-10 by cmkwon, ���� �̺�Ʈ ������ �߰��ϱ�
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::QP_CheckCouponEvent_ InsertEventItem error!!, AccountName(%s) CouponNumber(%s) ItemEventUID(%d) %s\r\n"
			, pParam->AccName, pParam->CouponNumber, pParam->ItemEventUID, GetCharacterString(q.pFieldIOCPSocket->GetCharacter(), string()));
		q.pFieldIOCPSocket->SendErrorMessage(T_FC_EVENT_COUPON_EVENT_USE_COUPON, ERR_PROTOCOL_QUEST_COUPON_INVALID_NUMBER, 540);
		util::del(pParam);
		return;
	}


	// 2008-01-23 by cmkwon, S_F, S_L: ���� ��� ���� �α׿� �߰� - 
	CAtumLogSender::SendLogMessageUseCoupon(q.pFieldIOCPSocket, pParam->AccName, pParam->CouponNumber);	

	///////////////////////////////////////////////////////////////////////////////
	// 2008-01-10 by cmkwon, �������̺�Ʈ ó���� �����Ѵ�.
	ATUM_DATE_TIME CurrentTime;
	CurrentTime.SetCurrentDateTime();
	QPARAM_INSERT_EVENTITEM *pQParam = new QPARAM_INSERT_EVENTITEM;
	pQParam->AccountUID		= couponEv.AccountUID;
	pQParam->CharacterUID	= q.pFieldIOCPSocket->GetCharacter()->CharacterUniqueNumber;
	pQParam->ItemEventUID	= couponEv.ItemEventUID;
	pQParam->InsertItemTime	= CurrentTime;
	MakeAndEnqueueQuery(QT_InsertEventItem, q.pFieldIOCPSocket, q.pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-01-10 by cmkwon, ���� ��� ������ Ŭ���̾�Ʈ�� ����
	INIT_MSG_WITH_BUFFER(MSG_FC_EVENT_COUPON_EVENT_USE_COUPON_OK, T_FC_EVENT_COUPON_EVENT_USE_COUPON_OK, pSUseCouponOK, SendBuf);
	pSUseCouponOK->ItemEventUID		= couponEv.ItemEventUID;
	q.pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_EVENT_COUPON_EVENT_USE_COUPON_OK));
	
	util::del(pParam);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_GetGuildStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		���� â�� ������ ��� ����
/// \author		dhjin
/// \date		2006-09-20 ~ 2006-09-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_GetGuildStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{

	QPARAM_GET_GUILDSTORE *pRMsg = (QPARAM_GET_GUILDSTORE*)q.pQueryParam;
	/*[Stored Query Definition]****************************************
	-------------------------------------------------------------------------------
	--!!!!	2006-09-21 by cmkwon
	-- Name: atum_Get_GuildStore
	-- Desc: �ش� ���� â�� ������ ��������
	-- 		
	--		
	--====
	CREATE PROCEDURE atum_Get_GuildStore
		@i_GuildUID         INT,
		@i_ItemStorage		TINYINT		-- 2006-09-25 by cmkwon
	AS
		SELECT * FROM td_Store 
			WHERE Possess = @i_GuildUID AND ItemStorage = @i_ItemStorage
	GO
	******************************************************************/
	CFieldIOCPSocket		*pFISock		= q.pFieldIOCPSocket;
	BYTE					tmpItemStorage	= ITEM_IN_GUILD_STORE;

	ITEM_GENERAL	ItemGeneral(NULL);		// ��ų �̿��� �����ۿ�
	RETCODE			ret;
	SQLINTEGER		pcbNTS = SQL_NTS;
	SQLINTEGER		arrCB[19] = {	SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,	SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,
									SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,	SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS	};		// 2012-03-08 by hskim, ���� �״� ���� ����
	SQL_TIMESTAMP_STRUCT tmpTimeStamp;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,	&pRMsg->GuildUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &tmpItemStorage, 0, NULL);		// 2006-09-25 by cmkwon
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0074, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Get_GuildStore Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);

		// 2012-12-11 by hskim, ��Ʈ�� ������ ���� ���� ���� (DB ó���� ���������� ���� ��Ŷ�� ����)
		if( pFISock )
		{
			pFISock->m_bProcessingStoreGetItem = FALSE;	
		}
		// end 2012-12-11 by hskim, ��Ʈ�� ������ ���� ���� ���� (DB ó���� ���������� ���� ��Ŷ�� ����)

		util::del(pRMsg);
		return;
	}

	SQLBindCol(hstmt, 1, SQL_C_SBIGINT, &ItemGeneral.UniqueNumber, 0,		&arrCB[1]);
	SQLBindCol(hstmt, 2, SQL_C_ULONG, &ItemGeneral.AccountUniqueNumber, 0,	&arrCB[2]);
	SQLBindCol(hstmt, 3, SQL_C_ULONG, &ItemGeneral.Possess, 0,				&arrCB[3]);
	SQLBindCol(hstmt, 4, SQL_C_UTINYINT, &ItemGeneral.ItemStorage, 0,		&arrCB[4]);
	SQLBindCol(hstmt, 5, SQL_C_UTINYINT, &ItemGeneral.Wear, 0,				&arrCB[5]);
	SQLBindCol(hstmt, 6, SQL_C_SLONG, &ItemGeneral.CurrentCount, 0,			&arrCB[6]);
// 2009-08-25 by cmkwon, ������� �ʴ� �ʵ� ����(td_Store.ScarcityNumber) - �Ʒ� �ε��� ������.
//	SQLBindCol(hstmt, 7, SQL_C_SBIGINT, &ItemGeneral.ScarcityNumber, 0,		&arrCB[7]);
	SQLBindCol(hstmt, 7, SQL_C_SLONG, &ItemGeneral.ItemWindowIndex, 0,		&arrCB[7]);
	SQLBindCol(hstmt, 8, SQL_C_SLONG, &ItemGeneral.ItemNum, 0,				&arrCB[8]);
	SQLBindCol(hstmt, 9,  SQL_C_SSHORT, &ItemGeneral.NumOfEnchants, 0,		&arrCB[9]);
	SQLBindCol(hstmt, 10, SQL_C_LONG, &ItemGeneral.PrefixCodeNum, 0,		&arrCB[10]);
	SQLBindCol(hstmt, 11, SQL_C_LONG, &ItemGeneral.SuffixCodeNum, 0,		&arrCB[11]);
	SQLBindCol(hstmt, 12, SQL_C_FLOAT, &ItemGeneral.CurrentEndurance, 0,	&arrCB[12]);
	SQLBindCol(hstmt, 13, SQL_C_LONG, &ItemGeneral.ColorCode, 0,			&arrCB[13]);
	SQLBindCol(hstmt, 14, SQL_C_LONG, &ItemGeneral.UsingTimeStamp, 0,		&arrCB[14]);
	SQLBindCol(hstmt, 15, SQL_C_TIMESTAMP, &tmpTimeStamp, 0,				&arrCB[15]);
	SQLBindCol(hstmt, 16, SQL_C_LONG, &ItemGeneral.ShapeItemNum, 0,			&arrCB[16]);	// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - ShapeItemNum �ʵ� �߰�
	SQLBindCol(hstmt, 17, SQL_C_SBIGINT, &ItemGeneral.MainSvrItemUID, 0,	&arrCB[17]);	// 2012-03-08 by hskim, ���� �״� ���� ����
	SQLBindCol(hstmt, 18, SQL_C_SLONG, &ItemGeneral.CoolingTimeStamp, 0,	&arrCB[18]);	// 2012-03-08 by hskim, ���� �״� ���� ����

	if (NULL == pFISock || FALSE == pFISock->IsValidCharacter(FALSE))
	{
		SQLFreeStmt(hstmt, SQL_CLOSE);

		// 2012-12-11 by hskim, ��Ʈ�� ������ ���� ���� ���� (DB ó���� ���������� ���� ��Ŷ�� ����)
		if( pFISock )
		{
			pFISock->m_bProcessingStoreGetItem = FALSE;	
		}
		// end 2012-12-11 by hskim, ��Ʈ�� ������ ���� ���� ���� (DB ó���� ���������� ���� ��Ŷ�� ����)

		util::del(pRMsg);
		return;
	}

	vectITEM_GENERALPtr		tmVectGuildItem;
	vectENCHANT				tmVectEnchant;				// 2007-01-30 by cmkwon


	util::zero(&ItemGeneral, sizeof(ITEM_GENERAL));	// initialize to 0x00
	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA )
	{
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
		{
			ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Get_GuildStore Fetching Failed!\n", TRUE, q.pFieldIOCPSocket, q);
			break;
		}

		ITEM *pItemInfo = m_pFieldIOCP8->GetItemInfo(ItemGeneral.ItemNum);
		if (pItemInfo == NULL)
		{
			g_pGlobal->WriteSystemLogEX(TRUE, "  [Error]Invaild Item in DB in CAtumFieldDBManager::QP_GetGuildStoreItem() -> %10s, %d \r\n",
				pFISock->m_character.CharacterName, ItemGeneral.ItemNum);

			// 2009-11-20 by cmkwon, ���� ������ �ε��� ��ȿ���� ���� ������ ó�� ���� - CAtumFieldDBManager::QP_GetGuildStoreItem#, ���ӷα��߰�, DB������ ����
			CAtumLogSender::SendLogMessageITEMDelete(q.pFieldIOCPSocket, &ItemGeneral, IDT_INVALID_ITEMNUM);
			QPARAM_DELETE_GUILDSTORE *pQParam	= new QPARAM_DELETE_GUILDSTORE;
			pQParam->GuildUID				= pRMsg->GuildUID;
			pQParam->ItemUID				= ItemGeneral.UniqueNumber;
			pQParam->byItemStorage			= ITEM_IN_GUILD_STORE;
			pQParam->SendMSG				= FALSE;
			this->MakeAndEnqueueQuery(QT_DeleteGuildStoreItem, q.pFieldIOCPSocket, q.pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber, pQParam);

			util::zero(&ItemGeneral, sizeof(ITEM_GENERAL));			// initialize to 0x00
			continue;
		}
		ItemGeneral.ItemInfo			= pItemInfo;			// 2006-09-25 by cmkwon
		ItemGeneral.Kind				= pItemInfo->Kind;		// 2006-09-26 by cmkwon
		ItemGeneral.CreatedTime			= tmpTimeStamp;			// 2006-09-29 by cmkwon, �ڵ� ���� ������ �ý��� ����

		///////////////////////////////////////////////////////////////////////////////
		// 2006-09-29 by cmkwon, �ڵ� ���� ������ �ý��� ����
		ItemGeneral.CreatedTime			= tmpTimeStamp;			
		if(COMPARE_BIT_FLAG(pItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE))
		{// 2006-09-29 by cmkwon, �ڵ� ���� ������ ���� üũ
			ATUM_DATE_TIME expireATime;
			ATUM_DATE_TIME curATime { true };
			expireATime			= ItemGeneral.CreatedTime;
			// 2009-10-22 by cmkwon, �ð� �������� ������ �ý��� ���� - CAtumFieldDBManager::QP_GetGuildStoreItem#, 
			//expireATime.AddDateTime(0, 0, 0, pItemInfo->Endurance, 0, 0);
			expireATime.AddDateTime(0, 0, 0, 0, CAtumSJ::GetTimeMinuteByItemKind8ItemAttribute(pItemInfo, TIME_TERM_DELETE_ITEM));	// 2009-10-22 by cmkwon, �ð� �������� ������ �ý��� ���� - CAtumFieldDBManager::QP_GetGuildStoreItem#,

			if(curATime > expireATime)
			{// 2006-09-29 by cmkwon, �Ⱓ ����, �����ؾ���

				// 2009-11-20 by cmkwon, ���� ������ �ε��� ��ȿ���� ���� ������ ó�� ���� - CAtumFieldDBManager::QP_GetGuildStoreItem#, ���ӷα��߰�
				CAtumLogSender::SendLogMessageITEMDelete(q.pFieldIOCPSocket, &ItemGeneral, IDT_EXPIRE_TIME);

				QPARAM_DELETE_GUILDSTORE *pQParam	= new QPARAM_DELETE_GUILDSTORE;
				pQParam->GuildUID				= pRMsg->GuildUID;
				pQParam->ItemUID				= ItemGeneral.UniqueNumber;
				pQParam->byItemStorage			= ITEM_IN_GUILD_STORE;
				pQParam->SendMSG				= FALSE;
				this->MakeAndEnqueueQuery(QT_DeleteGuildStoreItem, q.pFieldIOCPSocket, q.pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber, pQParam);

				util::zero(&ItemGeneral, sizeof(ITEM_GENERAL));	// initialize to 0x00
				continue;
			}
		}

		if(IS_COUNTABLE_ITEM(pItemInfo->Kind)
			&& 0 >= ItemGeneral.CurrentCount
			&& MONEY_ITEM_NUMBER != ItemGeneral.ItemNum)		// 2006-04-04 by cmkwon, SPI�϶����� �����ϸ� �ȵȴ�
		{
			// 2009-11-20 by cmkwon, ���� ������ �ε��� ��ȿ���� ���� ������ ó�� ���� - CAtumFieldDBManager::QP_GetGuildStoreItem#, ���ӷα��߰�
			CAtumLogSender::SendLogMessageITEMDelete(q.pFieldIOCPSocket, &ItemGeneral, IDT_COUNTABLEITEM_ZERO);

			QPARAM_DELETE_GUILDSTORE *pQParam	= new QPARAM_DELETE_GUILDSTORE;
			pQParam->GuildUID				= pRMsg->GuildUID;
			pQParam->ItemUID				= ItemGeneral.UniqueNumber;
			pQParam->byItemStorage			= ITEM_IN_GUILD_STORE;
			pQParam->SendMSG				= FALSE;
			this->MakeAndEnqueueQuery(QT_DeleteGuildStoreItem, q.pFieldIOCPSocket, q.pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber, pQParam);

			util::zero(&ItemGeneral, sizeof(ITEM_GENERAL));	// initialize to 0x00
			continue;
		}

		// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)
		if( IS_ENABLE_CHANGE_ShapeItemNum(ItemGeneral.Kind) )
		{
			QPARAM_FIXED_TERM_SHAPE	*pQParamFT = new QPARAM_FIXED_TERM_SHAPE;
			pQParamFT->ItemUID = ItemGeneral.UniqueNumber;
			pQParamFT->TermType = FIXED_TERM_SHAPE;
			pQParamFT->AppliedItemNum = 0;
			pQParamFT->StartDate.Reset();
			pQParamFT->EndDate.Reset();

			this->ExecuteQuery(QT_GetFixedTerm, q.pFieldIOCPSocket, pQParamFT);
	
			if( 0 != pQParamFT->AppliedItemNum )
			{
				BOOL bDelete = TRUE;

				if( ItemGeneral.ShapeItemNum == pQParamFT->AppliedItemNum )
				{
					INT nCalcMin = 0;
					ATUM_DATE_TIME CurrentDate { true };
					
					nCalcMin = pQParamFT->EndDate.GetTimeDiffTimeInMinutes(CurrentDate);

					if( nCalcMin > 0 )
					{
						ItemGeneral.FixedTermShape.bActive = TRUE;
						ItemGeneral.FixedTermShape.StartDate = pQParamFT->StartDate;
						ItemGeneral.FixedTermShape.EndDate = pQParamFT->EndDate;
						ItemGeneral.FixedTermShape.TimerUID = 0;

						// ��� â������ ���� �̺�Ʈ Ÿ�̸� �������� ����
						//
						// if( ITEM_IN_CHARACTER == tmpItemStorage )
						// {
						//		ItemGeneral.FixedTermShape.TimerUID = pFieldIOCPSocket->m_TimerManager.StartTimerField(TE_TYPE_FIXED_TERM_SHAPE, nCalcMin * TIMER_DO_MINUTELY_WORK, 0, 0);
						// }

						bDelete = FALSE;
					}
				}

				if( TRUE == bDelete)
				{
					ItemGeneral.ShapeItemNum = 0;

					FIXED_TERM_INFO DelFixedTermShape;
					util::zero(&DelFixedTermShape, sizeof(FIXED_TERM_INFO));

					DelFixedTermShape.StartDate = pQParamFT->StartDate;
					DelFixedTermShape.EndDate = pQParamFT->EndDate;

					QPARAM_UPDATE_SHAPEITEMNUM *pQParam = new QPARAM_UPDATE_SHAPEITEMNUM;
					pQParam->ItemUID		= ItemGeneral.UniqueNumber;
					pQParam->nShapeItemNum	= ItemGeneral.ShapeItemNum;
					pQParam->FixedTermShape.bActive = FALSE;
					pQParam->FixedTermShape.StartDate.Reset();
					pQParam->FixedTermShape.EndDate.Reset();
					pQParam->FixedTermShape.TimerUID = 0;

					this->MakeAndEnqueueQuery(QT_UpdateShapeItemNum, q.pFieldIOCPSocket, q.pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber, pQParam);

					if( FALSE == g_pFieldGlobal->IsArenaServer() )
					{
						QPARAM_FIXED_TERM_SHAPE	*pQParamFT2 = new QPARAM_FIXED_TERM_SHAPE;
						pQParamFT2->ItemUID = ItemGeneral.UniqueNumber;
						pQParamFT2->TermType = FIXED_TERM_SHAPE;
						pQParamFT2->AppliedItemNum = 0;
						pQParamFT2->StartDate.Reset();
						pQParamFT2->EndDate.Reset();
						this->MakeAndEnqueueQuery(QT_DeleteFixedTerm, q.pFieldIOCPSocket, q.pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber, pQParamFT2);

						CAtumLogSender::SendLogMessageFixedTermShapeEnd(pFISock, &ItemGeneral, pQParamFT->AppliedItemNum, DelFixedTermShape);
					}
				}
			}
			util::del(pQParamFT);
		}
		// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)

		ITEM_GENERAL* pItemGeneral = new ITEM_GENERAL(ItemGeneral);
		tmVectGuildItem.push_back(pItemGeneral);
		util::zero(&ItemGeneral, sizeof(ITEM_GENERAL));	// initialize to 0x00
	}// end_while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA )
	SQLFreeStmt(hstmt, SQL_CLOSE);		// reset cursor

	///////////////////////////////////////////////////////////////////////////
	// Load All Enchant

// 2007-01-30 by cmkwon, ���ν��� ���� �۾���, �Ʒ��� ���� ������
//	ENCHANT tmpEnchant;
//
//	/*[Stored Query Definition]************************************************
//	--!!!!
//	-- Name: atum_GetAllEnchant2
//	-- Desc: 2006-09-24 by cmkwon
//	--====
//	CREATE PROCEDURE atum_GetAllEnchant2
//		@i_AccountUniqueNumber		INT,
//		@i_CharacterUniqueNumber	INT,
//		@i_ItemStorage				TINYINT
//	AS
//		SELECT e.TargetItemUniqueNumber, e.TargetItemNum, e.EnchantItemNum
//		FROM td_Enchant e, td_Store s 
//		WHERE s.AccountUniqueNumber = @i_AccountUniqueNumber AND s.ItemStorage = @i_ItemStorage
//				AND s.possess = @i_CharacterUniqueNumber AND e.TargetItemUniqueNumber = s.UniqueNumber
//	GO
//	**************************************************************************/
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->GuildUID, 0, NULL);
//	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->GuildUID, 0, NULL);
//	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &tmpItemStorage, 0, NULL);		// 2006-09-24 by cmkwon
//	ret = SQLExecDirect(hstmt, (UCHAR*)"{call dbo.atum_GetAllEnchant2(?, ?, ?)}", SQL_NTS);
//	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO ) {
//		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_GetAllEnchant2 Failed!\n", TRUE, q.pFieldIOCPSocket, q);
//		SQLFreeStmt(hstmt, SQL_CLOSE);
//		util::del(pRMsg);
//		return;
//	}
//
//	arrCB[1] = arrCB[2] = arrCB[3] = arrCB[4] = SQL_NTS;
//	SQLBindCol(hstmt, 1, SQL_C_SBIGINT, &tmpEnchant.TargetItemUniqueNumber, 0,	&arrCB[1]);
//	SQLBindCol(hstmt, 2, SQL_C_ULONG, &tmpEnchant.TargetItemNum, 0,				&arrCB[2]);
//	SQLBindCol(hstmt, 3, SQL_C_ULONG, &tmpEnchant.EnchantItemNum, 0,			&arrCB[3]);
//
//	vectENCHANT	tmVectEnchant;
//	util::zero(&tmpEnchant, sizeof(ENCHANT));
//	while(SQL_NO_DATA != (ret = SQLFetch(hstmt)))
//	{
//		tmVectEnchant.push_back(tmpEnchant);
//		util::zero(&tmpEnchant, sizeof(ENCHANT));
//	}
//	SQLFreeStmt(hstmt, SQL_CLOSE);

	// 2007-01-30 by cmkwon, 2. �������� ��æƮ �ε�
	int nItemCnt = tmVectGuildItem.size();
	if(0 < nItemCnt)
	{
		for(int i=0; i < nItemCnt; i++)
		{
			ITEM_GENERAL* pItemG = tmVectGuildItem[i];

// 2009-02-17 by cmkwon, ��æƮ �� �� �ִ� ������ ī�ε� ���� - �Ʒ��� ���� ����
// 			// 2007-01-29 by cmkwon, ��æƮ ���� üũ - ũ�� 2����(����, �Ƹ�)
// 			if(FALSE == IS_WEAPON(pItemG->Kind)
// 				&& ITEMKIND_DEFENSE != pItemG->Kind)
// 			{
// 				continue;
// 			}
			if(FALSE == IS_ENCHANT_TARGET_ITEMKIND(pItemG->Kind))	// 2009-02-17 by cmkwon, ��æƮ �� �� �ִ� ������ ī�ε� ���� - 
			{
				continue;
			}

			/*[Stored Query Definition]************************************************
			CREATE PROCEDURE atum_GetEnchantBYItemUID
				@i_ItemUID		BIGINT
			AS
				SELECT TargetItemNum, EnchantItemNum
				FROM td_enchant WITH(NOLOCK)
				WHERE @i_ItemUID = TargetItemUniqueNumber
			GO
			**************************************************************************/
			SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &pItemG->UniqueNumber, 0, NULL);
			ret = SQLExecDirect(hstmt, PROCEDURE_080822_0075, SQL_NTS);
			if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
			{
				ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"@atum_GetEnchantBYItemUID Failed!\r\n", TRUE, q.pFieldIOCPSocket, q);
				SQLFreeStmt(hstmt, SQL_CLOSE);
				for (auto x : tmVectGuildItem) delete x;
				tmVectGuildItem.clear();
				tmVectEnchant.clear();

				pFISock->m_bProcessingStoreGetItem = FALSE;			// 2012-12-11 by hskim, ��Ʈ�� ������ ���� ���� ���� (DB ó���� ���������� ���� ��Ŷ�� ����)

				util::del(pRMsg);				
				return;
			}

			if(SQL_NO_DATA == ret)
			{
				SQLFreeStmt(hstmt, SQL_CLOSE);
				continue;
			}

			ENCHANT tmpEnchant;
			util::zero(arrCB, sizeof(arrCB[0])*4);
			SQLBindCol(hstmt, 1, SQL_C_ULONG, &tmpEnchant.TargetItemNum, 0,				&arrCB[1]);
			SQLBindCol(hstmt, 2, SQL_C_ULONG, &tmpEnchant.EnchantItemNum, 0,			&arrCB[2]);
			SQLBindCol(hstmt, 3, SQL_C_SBIGINT, &tmpEnchant.SequenceNumber_DB, 0,		&arrCB[3]); // 2013-01-15 by bckim, ��æƮ������ �������ѹ� �߰�

			util::zero(&tmpEnchant, sizeof(ENCHANT));
			while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA )
			{
				tmpEnchant.TargetItemUniqueNumber	= pItemG->UniqueNumber;
				
				tmVectEnchant.push_back(tmpEnchant);

				util::zero(&tmpEnchant, sizeof(ENCHANT));
			}
			SQLFreeStmt(hstmt, SQL_CLOSE);
		}// END - for(int i=0; i < nItemCnt; i++)
	}// END - if(0 < nItemCnt)


	// ��æƮ �ε� ����
	///////////////////////////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////////////////////////////
	// 2006-09-25 by cmkwon, ������ �߰� �� Ŭ���̾�Ʈ�� ����	
	mt_auto_lock ggLock(m_pFieldIOCP8->GetmtmapAllGuildItemPtrW());
	CGuildItemManager *pGuildItemMan = m_pFieldIOCP8->GetGuildItemMangerNoLockW(pRMsg->GuildUID);
	if(NULL == pGuildItemMan)
	{
		pGuildItemMan	= new CGuildItemManager;
		pGuildItemMan->SetGuildUID(pRMsg->GuildUID);

		m_pFieldIOCP8->InsertGuildItemManagerNoLockW(pGuildItemMan);
	}

	pGuildItemMan->InitGuildItemManager(&tmVectGuildItem, &tmVectEnchant);	

	///////////////////////////////////////////////////////////////////////////////
	// 2006-09-25 by dhjin, �ߺ� ������ ����
	pGuildItemMan->RearrangeGuildCountableItems(pFISock);	

	///////////////////////////////////////////////////////////////////////////////
	// 2006-09-25 by cmkwon, Ŭ���̾�Ʈ�� ������ ����Ʈ�� �����Ѵ�.
	pGuildItemMan->SendGuildStoreItemList(pFISock);

	ggLock.auto_unlock_cancel();

	for (auto x : tmVectGuildItem) delete x;
	tmVectGuildItem.clear();
	tmVectEnchant.clear();

	pFISock->m_bProcessingStoreGetItem = FALSE;			// 2012-12-11 by hskim, ��Ʈ�� ������ ���� ���� ���� (DB ó���� ���������� ���� ��Ŷ�� ����)

	util::del(pRMsg);	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_UpdateGuildStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		���� â�� ������ ����
/// \author		dhjin
/// \date		2006-09-21 ~ 2006-09-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_UpdateGuildStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_UPDATE_GUILDSTORE *pParam = (QPARAM_UPDATE_GUILDSTORE*)q.pQueryParam;
	CFieldIOCPSocket *pFieldIOCPSocket = q.pFieldIOCPSocket;		// 2013-02-21 by hskim, ��Ʈ�� ������ ���� ���� ���� (DB ó���� ���������� ���� ��Ŷ�� ����)
	
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_Update_Guild_Store 
			@i_ItemUID                  INT,
			@i_AccountUID                INT,
			@i_Possess					 INT,
			@i_ItemStorage				 TINYINT,
			@i_Count                     INT
		AS
			UPDATE td_Store 
				SET 
				AccountUniqueNumber = @i_AccountUID, Possess = @i_Possess, 
				ItemStorage = @i_ItemStorage, CurrentCount = @i_Count
				WHERE 
				UniqueNumber = @i_ItemUID
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pParam->ItemUID, 0,	 NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pParam->AccountUID, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pParam->Possess, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &pParam->ItemStorage, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pParam->Count, 0, NULL);
	
	BOOL bRet = SQLExecDirect(hstmt, PROCEDURE_080822_0076, SQL_NTS);
	
	if ( bRet!=SQL_SUCCESS && bRet!=SQL_SUCCESS_WITH_INFO && bRet != SQL_NO_DATA)
	{
		char szSysLog[1024];
		sprintf(szSysLog, "[DB Error]: atum_Update_Guild_Store error, ItemUID(%I64d) AccountUID(%d) CharacterUID(%d) ItemStorage(%d) Count(%d)\r\n"
			, pParam->ItemUID, pParam->AccountUID, pParam->Possess, pParam->ItemStorage, pParam->Count);
		DbgOut(szSysLog);
		g_pFieldGlobal->WriteSystemLog(szSysLog);
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "atum_Update_Guild_Store Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);

		if( NULL != pFieldIOCPSocket ) pFieldIOCPSocket->m_bProcessingStoreGetItem = FALSE;			// 2013-02-21 by hskim, ��Ʈ�� ������ ���� ���� ���� (DB ó���� ���������� ���� ��Ŷ�� ����)

		util::del(pParam);
		return;
	}
	
	SQLFreeStmt(hstmt, SQL_CLOSE);

	if( NULL != pFieldIOCPSocket ) pFieldIOCPSocket->m_bProcessingStoreGetItem = FALSE;			// 2013-02-21 by hskim, ��Ʈ�� ������ ���� ���� ���� (DB ó���� ���������� ���� ��Ŷ�� ����)

	util::del(pParam);
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_InsertGuildStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		���� â�� ������ ����
/// \author		dhjin
/// \date		2006-09-21 ~ 2006-09-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_InsertGuildStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{	 
	// 2008-12-29 by dhjin, ���� ���� �߰��� - ���� ���� ���� �߰��� ����â�� �������� ���� ������Ʈ�ϱ� ������ FieldIOCPSocket�� NULL�̴� ����!!
	ITEM_GENERAL		*pItem = (ITEM_GENERAL*)q.pQueryParam;
	CFieldIOCPSocket	*pFieldIOCPSocket	= q.pFieldIOCPSocket;
	
	RETCODE ret;
	SDWORD pcbNTS = SQL_NTS;

	/*[Stored Query Definition]************************************************
	-- !!!!
	-- Name: dbo.atum_InsertStoreItem
	-- Desc:
	--		2005-12-07 by cmkwon, ������ġ �ʵ� �߰�
	--		-- // 2009-08-25 by cmkwon, ������� �ʴ� �ʵ� ����(td_Store.ScarcityNumber) - 
	--		-- // 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - td_Store.ShapeItemNum �ʵ� �߰�, �ʵ帮��Ʈ ���
	-- ====
	**************************************************************************/

// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) - �Ʒ��� ���� ���� ��
// 	SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pItem->UniqueNumber, 0, NULL);
// 	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItem->AccountUniqueNumber, 0, NULL);
// 	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItem->Possess, 0, NULL);
// 	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pItem->ItemStorage, 0, NULL);		// 2005-12-07 by cmkwon
// 	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pItem->Wear, 0, NULL);
// 	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->CurrentCount, 0, NULL);
// 	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pItem->ScarcityNumber, 0, NULL);
// 	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->ItemWindowIndex, 0, NULL);
// 	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->ItemNum, 0, NULL);
// 	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &pItem->NumOfEnchants, 0, NULL);
// 	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->PrefixCodeNum, 0, NULL);
// 	SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->SuffixCodeNum, 0, NULL);
// 	SQLBindParameter(hstmt, 13, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pItem->CurrentEndurance, 0, NULL);
// 	SQLBindParameter(hstmt, 14, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItem->ColorCode, 0, NULL);
// 	SQLBindParameter(hstmt, 15, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItem->UsingTimeStamp, 0, NULL);
// 
// // 2008-07-08 by cmkwon, MySQL �������� ����(���ν������� ����ó�� �Ұ��� ����) - 
// // 	ret = SQLExecDirect(hstmt,
// // 		(UCHAR*)"{ ? = call atum_InsertStoreItem(?,?,?,?,?, ?,?,?,?,?,\
// // 												 ?,?,?,?)}", SQL_NTS);
// 	ret = SQLExecDirect(hstmt, (UCHAR*)"{call atum_InsertStoreItem(?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?)}", SQL_NTS);
// 
// 	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
// 	{
// 		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_InsertStoreItem Failed!\n", TRUE, q.pFieldIOCPSocket, q);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		util::del(pItem);
// 
// 		pFieldIOCPSocket->DecrementInsertingStoreCounts();
// 		return;
// 	}
// 
// 	// Clear any result sets generated.
// 	while ( ( ret = SQLMoreResults(hstmt) ) != SQL_NO_DATA );
// 	SQLFreeStmt(hstmt, SQL_CLOSE);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) - 
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItem->AccountUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItem->Possess, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pItem->ItemStorage, 0, NULL);		// 2005-12-07 by cmkwon
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pItem->Wear, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->CurrentCount, 0, NULL);
// 2009-08-25 by cmkwon, ������� �ʴ� �ʵ� ����(td_Store.ScarcityNumber) - �Ʒ� �ε��� ����
//	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pItem->ScarcityNumber, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->ItemWindowIndex, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->ItemNum, 0, NULL);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &pItem->NumOfEnchants, 0, NULL);
	SQLBindParameter(hstmt, 9,  SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->PrefixCodeNum, 0, NULL);
	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->SuffixCodeNum, 0, NULL);
	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pItem->CurrentEndurance, 0, NULL);
	SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItem->ColorCode, 0, NULL);
	SQLBindParameter(hstmt, 13, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItem->UsingTimeStamp, 0, NULL);
	SQLBindParameter(hstmt, 14, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pItem->ShapeItemNum, 0, NULL);		// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - ShapeItemNum �ʵ� �߰�

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0077, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_InsertStoreItem@ Failed 1 !\r\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pItem);

		pFieldIOCPSocket->DecrementInsertingStoreCounts();
		return;
	}
	SQLBindCol(hstmt, 1, SQL_C_UBIGINT, &(pItem->UniqueNumber), 0, NULL);	

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(hstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 �̸� ����� RowCount �̴� 
			ret = SQLMoreResults(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(hstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(hstmt);                 // 2008-07-25 by cmkwon, ���� Results �� �ִ����� �ѹ��� üũ�Ѵ�.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);	
	if(FALSE == IS_VALID_UID64(pItem->UniqueNumber))
	{// 2008-07-24 by cmkwon, �߰��� �������� ItemUID üũ   
		if(NULL == pFieldIOCPSocket)
        {// 2008-12-29 by dhjin, ���� ���� �߰���
 		     g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] atum_InsertStoreItem@ error Direct Insert GuildStore !! %s, GuildUID(%d)\r\n"
			     ,GetItemGeneralString(pItem, string()), pItem->Possess);
		     util::del(pItem);
		     return;
        }	 

		if(pFieldIOCPSocket->IsValidCharacter(FALSE))
		{
			pFieldIOCPSocket->SendErrorMessage(T_FC_STORE_INSERT_ITEM, ERR_DB_EXECUTION_FAILED);
		}
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] atum_InsertStoreItem@ error 1 !! %s %s, GuildUID(%d)\r\n"
			, GetCharacterString(pFieldIOCPSocket->GetCharacter(), string()), GetItemGeneralString(pItem, string()), pItem->Possess);
		util::del(pItem);
		return;
	}

	mt_auto_lock igLock(m_pFieldIOCP8->GetmtmapAllGuildItemPtrW());

	///////////////////////////////////////////////////////////////////////////////
	// 2006-09-29 by cmkwon
	pItem->CreatedTime.SetCurrentDateTime(TRUE);

	// 2008-07-23 by cmkwon, ���� �α� ���� ���� - �׻� ���ܾ� ������ ����� �̵���.
	CAtumLogSender::SendLogMessageITEMInsertToGuildStore(pFieldIOCPSocket, pItem, pItem->CurrentCount);

	CGuildItemManager *pGItemManager = m_pFieldIOCP8->GetGuildItemMangerNoLockW(pItem->Possess);
	if(NULL == pGItemManager)
	{
		util::del(pItem);
		return;
	}
	
	if(FALSE == pGItemManager->PushBackGuildStoreItemByPointer(pItem))
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] PushBackGuildStoreItemByPointer error !! %s GuildUID(%d)\r\n", GetItemGeneralString(pItem, string()), pItem->Possess);
		util::del(pItem);
		return;
	}

	// 2008-07-23 by cmkwon, ���� �α� ���� ���� - �׻� ���ܾ� ������ ���� �̵���.
	//CAtumLogSender::SendLogMessageITEMInsertToGuildStore(pFieldIOCPSocket, pItem, pItem->CurrentCount);

	if(pFieldIOCPSocket
		&& pFieldIOCPSocket->IsValidCharacter(FALSE))
	{// 2008-07-23 by cmkwon, ���� �α� ���� ���� - üũ �߰�
		// Send MSG
		INIT_MSG_WITH_BUFFER(MSG_FC_STORE_INSERT_ITEM, T_FC_STORE_INSERT_ITEM, msgInsertItem, msgInsertItemBuf);
		msgInsertItem->FromCharacterUniqueNumber	= pFieldIOCPSocket->m_character.CharacterUniqueNumber;
		msgInsertItem->ItemInsertionType			= IUT_GUILD_STORE;
		msgInsertItem->ItemGeneral					= *pItem;
		pFieldIOCPSocket->SendAddData(msgInsertItemBuf, MSG_SIZE(MSG_FC_STORE_INSERT_ITEM));
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_DeleteGuildStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		���� â�� ������ ����
/// \author		dhjin
/// \date		2006-09-21 ~ 2006-09-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_DeleteGuildStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_DELETE_GUILDSTORE	*pQParam = (QPARAM_DELETE_GUILDSTORE*)q.pQueryParam;
	CFieldIOCPSocket			*pFieldIOCPSocket	= q.pFieldIOCPSocket;

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_Delete_Guild_Store
				@i_ItemUID                   INT,
				@i_GuildUID                  INT,
				@i_ItemStorage				 TINYINT
			AS
				DELETE FROM td_Store
					WHERE UniqueNumber = @i_ItemUID AND AccountUniqueNumber = @i_GuildUID AND Possess = @i_GuildUID
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQParam->ItemUID, 0,	 NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQParam->GuildUID, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &pQParam->byItemStorage, 0, NULL);
	
	BOOL bRet = SQLExecDirect(hstmt, PROCEDURE_080822_0078, SQL_NTS);
	
	if ( bRet!=SQL_SUCCESS && bRet!=SQL_SUCCESS_WITH_INFO && bRet != SQL_NO_DATA)
	{
		char szSysLog[1024];
		sprintf(szSysLog, "[DB Error]: atum_Delete_Guild_Store error, ItemUID(%I64d) GuildUID(%d)\r\n"
			, pQParam->ItemUID, pQParam->GuildUID);
		DbgOut(szSysLog);
		g_pFieldGlobal->WriteSystemLog(szSysLog);
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "atum_Delete_Guild_Store Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);

		if( NULL != pFieldIOCPSocket ) pFieldIOCPSocket->m_bProcessingStoreGetItem = FALSE;			// 2013-02-21 by hskim, ��Ʈ�� ������ ���� ���� ���� (DB ó���� ���������� ���� ��Ŷ�� ����)

		util::del(pQParam);
		return;
	}
	
	SQLFreeStmt(hstmt, SQL_CLOSE);
	
	if (pQParam->SendMSG)
	{
		INIT_MSG_WITH_BUFFER(MSG_FC_STORE_DELETE_ITEM, T_FC_STORE_DELETE_ITEM, msgDelete, msgDeleteBuf);
		msgDelete->ItemUniqueNumber = pQParam->ItemUID;
		msgDelete->ItemDeletionType = IUT_GENERAL;
		pFieldIOCPSocket->SendAddData(msgDeleteBuf, MSG_SIZE(MSG_FC_STORE_DELETE_ITEM));
	}

	if( NULL != pFieldIOCPSocket ) pFieldIOCPSocket->m_bProcessingStoreGetItem = FALSE;			// 2013-02-21 by hskim, ��Ʈ�� ������ ���� ���� ���� (DB ó���� ���������� ���� ��Ŷ�� ����)

	util::del(pQParam);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_AllDeleteGuildStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		
/// \author		dhjin
/// \date		2006-09-29 ~ 2006-09-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_AllDeleteGuildStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_DELETE_GUILDSTORE	*pQParam = (QPARAM_DELETE_GUILDSTORE*)q.pQueryParam;
	CFieldIOCPSocket			*pFieldIOCPSocket	= q.pFieldIOCPSocket;

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_AllDelete_Guild_Store
				@i_GuildUID                  INT,
				@i_ItemStorage				 TINYINT
			AS
				DELETE FROM td_Store
					WHERE AccountUniqueNumber = @i_GuildUID AND Possess = @i_GuildUID AND ItemStorage = @i_ItemStorage
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQParam->GuildUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &pQParam->byItemStorage, 0, NULL);
	
	BOOL bRet = SQLExecDirect(hstmt, PROCEDURE_080822_0079, SQL_NTS);
	
	if ( bRet!=SQL_SUCCESS && bRet!=SQL_SUCCESS_WITH_INFO && bRet != SQL_NO_DATA)
	{
		char szSysLog[1024];
		sprintf(szSysLog, "[DB Error]: atum_AllDelete_Guild_Store error, ItemUID(%I64d) GuildUID(%d)\r\n"
			, pQParam->ItemUID, pQParam->GuildUID);
		DbgOut(szSysLog);
		g_pFieldGlobal->WriteSystemLog(szSysLog);
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "atum_AllDelete_Guild_Store Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQParam);
		return;
	}
	
	SQLFreeStmt(hstmt, SQL_CLOSE);
	
	util::del(pQParam);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_Insert2WarpableUserList(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		// 2007-08-30 by cmkwon, ȸ�Ƿ� �ý��� ���� - �����㰡 �߰�
/// \author		cmkwon
/// \date		2007-08-30 ~ 2007-08-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_Insert2WarpableUserList(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_Insert2WarpableUserList	*pQParam	= (QPARAM_Insert2WarpableUserList*)q.pQueryParam;
	CFieldIOCPSocket				*pFISock	= q.pFieldIOCPSocket;

	CFieldMapProject *pFMPro = m_pFieldIOCP8->GetFieldMapProjectByMapIndex(pQParam->MapIndex);
	if(NULL == pFMPro)
	{
		pFISock->SendString128(STRING_128_USER_NOTICE, STRMSG_070830_0002, pQParam->MapIndex);
		util::del(pQParam);
		return;
	}


	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name:dbo.atum_getCharacterUIDByCharacterName
	-- Desc: // 2007-08-30 by cmkwon, ȸ�Ƿ� �ý��� ���� - CharacterUID��������
	--====
	CREATE PROCEDURE dbo.atum_getCharacterUIDByCharacterName
		@i_CharacterName		VARCHAR(20),	
		@i_CharInflMask			TINYINT
	AS
		-- #define RACE_DELETED_CHARACTER		(USHORT)0x4000	// ������ ĳ����, 16384 // 2007-02-21 by cmkwon
		SELECT UniqueNumber
		FROM td_character WITH(NOLOCK)
		WHERE CharacterName = @i_CharacterName AND 0 = Race & 0x4000 AND 0 <> InfluenceType & @i_CharInflMask
	GO
	**************************************************************************/
	SQLINTEGER		arrCB[4] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pQParam->CharacterName0, 0,	&arrCB[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &pQParam->byInfluenceType0, 0,					&arrCB[2]);
	
	SQLRETURN sqlRet = SQLExecDirect(hstmt, PROCEDURE_080822_0080, SQL_NTS);	
	if ( sqlRet!=SQL_SUCCESS && sqlRet!=SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "atum_getCharacterUIDByCharacterName_ Failed!\r\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQParam);
		return;
	}
	
	UID32_t CharUID = 0;
	arrCB[1] = arrCB[2] = SQL_NTS;
	SQLBindCol(hstmt, 1, SQL_C_ULONG, &CharUID, 0,		&arrCB[1]);
	sqlRet = SQLFetch(hstmt);
	SQLFreeStmt(hstmt, SQL_CLOSE);
	if(FALSE == IS_VALID_UNIQUE_NUMBER(CharUID))
	{
		pFISock->SendString128(STRING_128_USER_NOTICE, STRMSG_070830_0006, pQParam->CharacterName0);
		util::del(pQParam);
		return;
	}

	if(FALSE == pFMPro->InsertWarpableUserList(CharUID, pQParam->CharacterName0))
	{// 2007-08-31 by cmkwon,
		pFISock->SendString128(STRING_128_USER_NOTICE, STRMSG_070830_0007, pQParam->CharacterName0, SIZE_MAX_CONFERENCEROOM_USER);
		util::del(pQParam);
		return;
	}

	
	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name:dbo.atum_insert2WarpableUserList
	-- Desc: // 2007-08-30 by cmkwon, ȸ�Ƿ� �ý��� ���� - ���� �߰��ϱ�
	--			#define ERR_NO_ERROR									0x0000
	--			#define ERR_DB_INTEGRITY_CONSTRAINT_VIOLATION			0x2103		// integrity constraint violation, 23000
	--====
	CREATE PROCEDURE dbo.atum_insert2WarpableUserList
		-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
		--@i_RetErr				INT		OUTPUT,		-- 0�̸� No Error, �� �ܴ̿� ������
		@i_MapIndex				INT,
		@i_CharUID				INT
	AS
		INSERT INTO dbo.td_user_list_warpable_to_map
		VALUES(@i_MapIndex, @i_CharUID)

		IF (@@Error <> 0)
		BEGIN
			-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
			--SET @i_RetErr = 0x2103
			SELECT 0x2103;
		END
		ELSE
		BEGIN
			-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
			--SET @i_RetErr = 0
			SELECT 0;
		END

	GO
	**************************************************************************/
	arrCB[1] = arrCB[2] = arrCB[3] = SQL_NTS;
	INT nRetErr = 1;
// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) - 
// 	SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nRetErr, 0,			&arrCB[1]);
// 	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQParam->MapIndex, 0,	&arrCB[2]);
// 	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &CharUID, 0,			&arrCB[3]);
// 
// 	sqlRet = SQLExecDirect(hstmt, (UCHAR*)"{call atum_insert2WarpableUserList(?, ?, ?)}", SQL_NTS);	
// 	if ( sqlRet!=SQL_SUCCESS && sqlRet!=SQL_SUCCESS_WITH_INFO)
// 	{
// 		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "atum_insert2WarpableUserList_ Failed!\r\n", TRUE, q.pFieldIOCPSocket, q);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		util::del(pQParam);
// 		return;
// 	}
// 	sqlRet = SQLMoreResults(hstmt);
// 	SQLFreeStmt(hstmt, SQL_CLOSE);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) - 
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQParam->MapIndex, 0,	&arrCB[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &CharUID, 0,			&arrCB[2]);
	
	sqlRet = SQLExecDirect(hstmt, PROCEDURE_080822_0081, SQL_NTS);	
	if (sqlRet!=SQL_SUCCESS && sqlRet!=SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "atum_insert2WarpableUserList_ Failed!\r\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQParam);
		return;
	}
	SQLBindCol(hstmt, 1, SQL_C_LONG, &(nRetErr), 0, NULL);	
	
	SQLRETURN ret = 0;
	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(hstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 �̸� ����� RowCount �̴� 
			ret = SQLMoreResults(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(hstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(hstmt);                 // 2008-07-25 by cmkwon, ���� Results �� �ִ����� �ѹ��� üũ�Ѵ�.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	if(ERR_NO_ERROR != nRetErr)
	{// 2007-08-31 by cmkwon, ���� ���� ����
		pFISock->SendString128(STRING_128_USER_NOTICE, STRMSG_070830_0007, pQParam->CharacterName0, SIZE_MAX_CONFERENCEROOM_USER);
		util::del(pQParam);
		return;
	}

	pFISock->SendString128(STRING_128_USER_NOTICE, STRMSG_070830_0008, pQParam->CharacterName0);
	CFieldIOCPSocket *pTargetFISoc = m_pFieldIOCP8->GetFieldIOCPSocketByCharacterUniqueNumber(CharUID);
	if(pTargetFISoc
		&& pTargetFISoc->IsValidCharacter(FALSE))
	{
		pTargetFISoc->SendString128(STRING_128_USER_NOTICE, STRMSG_070830_0009);
	}
	util::del(pQParam);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_DeleteWarpableUser(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		// 2007-08-30 by cmkwon, ȸ�Ƿ� �ý��� ���� - �����㰡 ����
/// \author		cmkwon
/// \date		2007-08-30 ~ 2007-08-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_DeleteWarpableUser(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_DeleteWarpableUser	*pQParam	= (QPARAM_DeleteWarpableUser*)q.pQueryParam;
	CFieldIOCPSocket			*pFISock	= q.pFieldIOCPSocket;

	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name:dbo.atum_deleteWarpableUser
	-- Desc: // 2007-08-30 by cmkwon, ȸ�Ƿ� �ý��� ���� - ���� �߰��ϱ�
	--====
	CREATE PROCEDURE dbo.atum_deleteWarpableUser
		@i_MapIndex				INT,
		@i_CharUID				INT
	AS
		DELETE FROM dbo.td_user_list_warpable_to_map
		WHERE MapIndex = @i_MapIndex AND CharacterUID = @i_CharUID
	GO
	**************************************************************************/
	SQLINTEGER		arrCB[3] = {SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQParam->MapIndex, 0,		&arrCB[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQParam->CharacterUID0, 0,	&arrCB[2]);
	
	SQLRETURN sqlRet = SQLExecDirect(hstmt, PROCEDURE_080822_0082, SQL_NTS);	
	if ( sqlRet!=SQL_SUCCESS && sqlRet!=SQL_SUCCESS_WITH_INFO && sqlRet != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "atum_deleteWarpableUser_ Failed!\r\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQParam);
		return;
	}	
	SQLFreeStmt(hstmt, SQL_CLOSE);	
	util::del(pQParam);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_UPDATE_ConnectingServerGroupID(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		// 2007-11-06 by cmkwon, ���� �α� DB ���� ���� �����ϱ� - CAtumFieldDBManager::QP_UPDATE_ConnectingServerGroupID() �߰� ��
/// \author		cmkwon
/// \date		2007-11-06 ~ 2007-11-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
//void CAtumFieldDBManager::QP_UPDATE_ConnectingServerGroupID(FIELD_DB_QUERY q, SQLHSTMT hstmt)
//{
//	QPARAM_UPDATE_ConnectingServerGroupID	*pQParam	= (QPARAM_UPDATE_ConnectingServerGroupID*)q.pQueryParam;
//	CFieldIOCPSocket			*pFISock	= q.pFieldIOCPSocket;
//	if(FALSE == IS_VALID_UNIQUE_NUMBER(pQParam->AccountUID))
//	{
//		util::del(pQParam);
//		return;
//	}
//
//	/*[Stored Query Definition]************************************************
//	--------------------------------------------------------------------------------
//	-- PROCEDURE NAME	: dbo.atum_Update_ConnectingServerGroupID_Of_Account
//	-- DESC				: // 2007-11-06 by cmkwon, ���� �α� DB ���� ���� �����ϱ� - ���� ���� ������Ʈ
//	--------------------------------------------------------------------------------
//	CREATE PROCEDURE dbo.atum_Update_ConnectingServerGroupID_Of_Account
//		@i_AccUID				INT,
//		@i_ServerGroupID		INT
//	AS
//		UPDATE atum2_db_account.dbo.td_account
//		SET ConnectingServerGroupID = @i_ServerGroupID
//		WHERE AccountUniqueNumber = @i_AccUID
//	GO
//	**************************************************************************/
//	SQLINTEGER		arrCB[3] = {SQL_NTS,SQL_NTS,SQL_NTS};
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQParam->AccountUID, 0,		&arrCB[1]);
//	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQParam->ServerGroupID, 0,		&arrCB[2]);
//	
//	SQLRETURN sqlRet = SQLExecDirect(hstmt, PROCEDURE_080822_0083, SQL_NTS);	
//	if ( sqlRet!=SQL_SUCCESS && sqlRet!=SQL_SUCCESS_WITH_INFO && sqlRet != SQL_NO_DATA)
//	{
//		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "atum_Update_ConnectingServerGroupID_Of_Account_ Failed!\r\n", TRUE, q.pFieldIOCPSocket, q);
//		SQLFreeStmt(hstmt, SQL_CLOSE);
//		util::del(pQParam);
//		return;
//	}	
//	SQLFreeStmt(hstmt, SQL_CLOSE);	
//	util::del(pQParam);
//}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_GiveStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		// 2007-11-13 by cmkwon, �����ϱ� ��� �߰� - CAtumFieldDBManager::QP_GiveStoreItem() �߰�
/// \author		cmkwon
/// \date		2007-11-06 ~ 2007-11-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_GiveStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
// 2007-11-27 by cmkwon, �����ϱ� �α� ���� - �Ʒ��� ���� ������
//	ITEM_GENERAL		*pItem = (ITEM_GENERAL*)q.pQueryParam;
	QPARAM_GiveStoreItem *pQParam = (QPARAM_GiveStoreItem*)q.pQueryParam;
	ITEM_GENERAL		*pItem = &(pQParam->itemG);		// 2007-11-27 by cmkwon, �����ϱ� �α� ���� - 
	CFieldIOCPSocket	*pFieldIOCPSocket = q.pFieldIOCPSocket;
	BYTE				ItemInsertionType = (BYTE)q.nGeneralParam1;
	if (NULL == pFieldIOCPSocket)
	{// 2008-07-21 by dhjin, EP3 - Nullüũ �߰�
		util::del(pQParam);
		return;
	}

	RETCODE ret;
	SDWORD pcbNTS = SQL_NTS;

	/*[Stored Query Definition]************************************************
	-- !!!!
	-- Name: dbo.atum_InsertStoreItem
	-- Desc:
	--		2005-12-07 by cmkwon, ������ġ �ʵ� �߰�
	--		-- // 2009-08-25 by cmkwon, ������� �ʴ� �ʵ� ����(td_Store.ScarcityNumber) - 
	--		-- // 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - td_Store.ShapeItemNum �ʵ� �߰�, �ʵ帮��Ʈ ���
	-- ====
	**************************************************************************/

// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) - �Ʒ��� ���� ������
// 	SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pItem->UniqueNumber, 0, NULL);
// 	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItem->AccountUniqueNumber, 0, NULL);
// 	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItem->Possess, 0, NULL);
// 	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pItem->ItemStorage, 0, NULL);		// 2005-12-07 by cmkwon
// 	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pItem->Wear, 0, NULL);
// 	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->CurrentCount, 0, NULL);
// 	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pItem->ScarcityNumber, 0, NULL);
// 	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->ItemWindowIndex, 0, NULL);
// 	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->ItemNum, 0, NULL);
// 	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &pItem->NumOfEnchants, 0, NULL);
// 	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->PrefixCodeNum, 0, NULL);
// 	SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->SuffixCodeNum, 0, NULL);
// 	SQLBindParameter(hstmt, 13, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pItem->CurrentEndurance, 0, NULL);
// 	SQLBindParameter(hstmt, 14, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItem->ColorCode, 0, NULL);
// 	SQLBindParameter(hstmt, 15, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItem->UsingTimeStamp, 0, NULL);
// 
// 	// 2008-07-08 by cmkwon, MySQL �������� ����(���ν������� ����ó�� �Ұ��� ����) - 
// 	//ret = SQLExecDirect(hstmt, (UCHAR*)"{ ? = call atum_InsertStoreItem(?,?,?,?,?, ?,?,?,?,?, ?,?,?,?)}", SQL_NTS);
// 	ret = SQLExecDirect(hstmt, (UCHAR*)"{call atum_InsertStoreItem(?, ?,?,?,?,?, ?,?,?,?,?, ?,?,?,?)}", SQL_NTS);
// 	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
// 	{
// 		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_InsertStoreItem Failed!\n", TRUE, q.pFieldIOCPSocket, q);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		util::del(pQParam);		// 2007-11-27 by cmkwon, �����ϱ� �α� ���� - 
// 		return;
// 	}
// 
// 	// Clear any result sets generated.
// 	while ( ( ret = SQLMoreResults(hstmt) ) != SQL_NO_DATA );
// 	SQLFreeStmt(hstmt, SQL_CLOSE);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) - 
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItem->AccountUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItem->Possess, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pItem->ItemStorage, 0, NULL);		// 2005-12-07 by cmkwon
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pItem->Wear, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->CurrentCount, 0, NULL);
// 2009-08-25 by cmkwon, ������� �ʴ� �ʵ� ����(td_Store.ScarcityNumber) - �Ʒ� �ε��� ����
//	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pItem->ScarcityNumber, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->ItemWindowIndex, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->ItemNum, 0, NULL);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &pItem->NumOfEnchants, 0, NULL);
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->PrefixCodeNum, 0, NULL);
	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->SuffixCodeNum, 0, NULL);
	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pItem->CurrentEndurance, 0, NULL);
	SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItem->ColorCode, 0, NULL);
	SQLBindParameter(hstmt, 13, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItem->UsingTimeStamp, 0, NULL);
	SQLBindParameter(hstmt, 14, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pItem->ShapeItemNum, 0, NULL);		// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - ShapeItemNum �ʵ� �߰�

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0077, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_InsertStoreItem Failed 2 !\r\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQParam);		// 2007-11-27 by cmkwon, �����ϱ� �α� ���� - 
		return;
	}
	SQLBindCol(hstmt, 1, SQL_C_UBIGINT, &(pItem->UniqueNumber), 0, NULL);	

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(hstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 �̸� ����� RowCount �̴� 
			ret = SQLMoreResults(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(hstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(hstmt);                 // 2008-07-25 by cmkwon, ���� Results �� �ִ����� �ѹ��� üũ�Ѵ�.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);	
	if(FALSE == IS_VALID_UID64(pItem->UniqueNumber))
	{// 2008-07-24 by cmkwon, �߰��� �������� ItemUID üũ
		if(pFieldIOCPSocket->IsValidCharacter(FALSE))
		{
			pFieldIOCPSocket->SendErrorMessage(T_FC_SHOP_BUY_CASH_ITEM, ERR_DB_EXECUTION_FAILED);
		}
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] atum_InsertStoreItem@ error 2 !! %s %s, PeerCharacterUID(%d)\r\n"
			, GetCharacterString(pFieldIOCPSocket->GetCharacter(), string()), GetItemGeneralString(pItem, string()), pItem->Possess);
		util::del(pQParam);
		return;
	}

	///////////////////////////////////////////////////////////////////////////////	
	// 2007-11-27 by cmkwon, �����ϱ� �α� ���� - 
	int nItemCount = pItem->CurrentCount;
	if(FALSE == IS_COUNTABLE_ITEM(pItem->Kind))
	{
		nItemCount			= 1;
		pItem->CurrentCount	= 1;
	}
	CAtumLogSender::SendLogMessageITEMGiftRecv(pQParam->itemG.Possess, pItem, nItemCount, pQParam->SenderCharUID, pQParam->SenderCharacterName);

	util::del(pQParam);			// 2007-11-27 by cmkwon, �����ϱ� �α� ���� - 
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_GetLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		EP3 ���� �ý��� - DB���� ���� ��������
/// \author		dhjin
/// \date		2008-04-24 ~ 2008-04-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_GetLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_GetLetter
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_Delete_Letter_By_LetterPassedRangeDay
	-- DESC		 : ���� �Ⱓ ���� ���� �����ϱ�
	-- // 2008-05-13 by dhjin, EP3 �����ý��� - 
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Delete_Letter_By_LetterPassedRangeDay
		@i_RecvCharacterUID		INT,
		@i_LetterPassedRangeDay	INT
	AS
		DELETE FROM dbo.td_Letter WHERE RecvCharacterUID = @i_RecvCharacterUID AND (DATEDIFF(dd, SendDate, GETDATE()) > @i_LetterPassedRangeDay)
		DELETE FROM dbo.td_ReadAllLetter WHERE AllLetterUID IN (SELECT AllLetterUID FROM  dbo.td_AllLetter WITH (NOLOCK) WHERE (DATEDIFF(dd, SendDate, GETDATE()) > @i_LetterPassedRangeDay) )
		DELETE FROM dbo.td_AllLetter WHERE (DATEDIFF(dd, SendDate, GETDATE()) > @i_LetterPassedRangeDay)
	GO
	**************************************************************************/
	CFieldIOCPSocket	*pFieldIOCPSocket = q.pFieldIOCPSocket;
	if (NULL == pFieldIOCPSocket)
	{// 2008-07-21 by dhjin, EP3 - Nullüũ �߰�
		return;
	}
	INT LetterPassedRangeDay = LETTER_PASSED_RANGE_DAY;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pFieldIOCPSocket->m_character.CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &LetterPassedRangeDay, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0085, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Delete_Letter_By_LetterPassedRangeDay Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_Get_Letter
	-- DESC		 : ���� ��������
	-- // 2008-04-14 by dhjin, EP3 �����ý��� - 
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Get_Letter
		@i_RecvCharacterUID		INT,
		@i_LetterPassedRangeDay	INT
	AS
		SELECT LetterUID, SendCharacterName, SendDate, CheckRead, Title, Content FROM dbo.td_Letter WITH (NOLOCK) WHERE RecvCharacterUID = @i_RecvCharacterUID
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pFieldIOCPSocket->m_character.CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &LetterPassedRangeDay, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0086, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Get_Letter Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	
	// Clear any result sets generated.
	SLETTER_INFO		SLetterInfo;
	util::zero(&SLetterInfo, sizeof(SLETTER_INFO));	// initialize to 0x00
	vectSLETTER_INFO	vectLetterInfo;
	vectLetterInfo.clear();
	SQL_TIMESTAMP_STRUCT SendDate;

	SQLINTEGER		arrCB[7] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS};

	SQLBindCol(hstmt, 1, SQL_C_SBIGINT,		&SLetterInfo.LetterUID,			0,							&arrCB[1]);
	SQLBindCol(hstmt, 2, SQL_C_CHAR,		SLetterInfo.SendCharacterName,	SIZE_MAX_CHARACTER_NAME,	&arrCB[2]);
	SQLBindCol(hstmt, 3, SQL_C_TIMESTAMP,	&SendDate,						0,							&arrCB[3]);
	SQLBindCol(hstmt, 4, SQL_C_UTINYINT,	&SLetterInfo.CheckRead,			0,							&arrCB[4]);
	SQLBindCol(hstmt, 5, SQL_C_CHAR,		SLetterInfo.Title,				SIZE_MAX_LETTER_TITLE,		&arrCB[5]);
	SQLBindCol(hstmt, 6, SQL_C_CHAR,		SLetterInfo.Content,			SIZE_MAX_LETTER_CONTENT,	&arrCB[6]);
	
	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA )
	{
		SLetterInfo.SendDate	= SendDate;
		vectLetterInfo.push_back(SLetterInfo);
		util::zero(&SLetterInfo, sizeof(SLETTER_INFO));	// initialize to 0x00
	}

	pFieldIOCPSocket->m_LetterManager.InitLetter(&vectLetterInfo);
	
	SQLFreeStmt(hstmt, SQL_CLOSE);	
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_ReadLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		EP3 ���� �ý��� - ���� �б�
/// \author		dhjin
/// \date		2008-04-24 ~ 2008-04-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_ReadLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_ReadLetter
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_Read_Letter
	-- DESC		 : ���� �б�
	-- // 2008-04-14 by dhjin, EP3 �����ý��� - 
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Read_Letter
		@i_LetterUID			BIGINT,
		@i_RecvCharacterUID		INT
	AS
		UPDATE dbo.td_Letter SET CheckRead = 1 WHERE LetterUID = @i_LetterUID AND RecvCharacterUID = @i_RecvCharacterUID
	GO
	**************************************************************************/
	QPARAM_READ_LETTER *pRMsg = (QPARAM_READ_LETTER*)q.pQueryParam;
	CFieldIOCPSocket	*pFieldIOCPSocket = q.pFieldIOCPSocket;
	if (NULL == pFieldIOCPSocket)
	{// 2008-07-21 by dhjin, EP3 - Nullüũ �߰�
		util::del(pRMsg);
		return;
	}

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->LetterUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->RecvCharacterUID, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0087, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Read_Letter Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}

	pFieldIOCPSocket->m_LetterManager.ReadLetter(pRMsg->LetterUID);
	pFieldIOCPSocket->m_LetterManager.SendLetterInfo(pRMsg->LetterUID);

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_DeleteLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		EP3 ���� �ý��� - ���� ����
/// \author		dhjin
/// \date		2008-04-24 ~ 2008-04-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_DeleteLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_DeleteLetter
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_Delete_Letter
	-- DESC		 : ���� ����
	-- // 2008-04-14 by dhjin, EP3 �����ý��� - 
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Delete_Letter
		@i_LetterUID			BIGINT,
		@i_RecvCharacterUID		INT
	AS
		DELETE FROM dbo.td_Letter WHERE LetterUID = @i_LetterUID AND RecvCharacterUID = @i_RecvCharacterUID
	GO
	**************************************************************************/
	QPARAM_DELETE_LETTER *pRMsg = (QPARAM_DELETE_LETTER*)q.pQueryParam;
	CFieldIOCPSocket	*pFieldIOCPSocket = q.pFieldIOCPSocket;
	if (NULL == pFieldIOCPSocket)
	{// 2008-07-21 by dhjin, EP3 - Nullüũ �߰�
		util::del(pRMsg);
		return;
	}

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->LetterUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->RecvCharacterUID, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0088, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Delete_Letter Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_SendLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		EP3 ���� �ý��� - ���� ������
/// \author		dhjin
/// \date		2008-05-08 ~ 2008-05-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_SendLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_SendLetter
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_Send_Letter
	-- DESC		 : ���� ������
	-- // 2008-04-14 by dhjin, EP3 �����ý��� - 
	--				-- // 2008-08-22 by cmkwon, MySQL ���� �ҽ� ���� - Result Set ==> ErrorCode, RecvCharacterUID, LetterUID
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Send_Letter
		-- // 2008-08-22 by cmkwon, MySQL ���� �ҽ� ���� - 
		--@o_Err					TINYINT	OUTPUT,
		--@o_RecvCharacterUID		INT	OUTPUT,
		@i_RecvCharacterName	VARCHAR(20),
		@i_SendCharacterUID		INT,
		@i_SendCharacterName	VARCHAR(20),
		@i_SendDate				VARCHAR(30),
		@i_Title				VARCHAR(40),
		@i_Content				VARCHAR(1000)
	AS
		DECLARE @CheckCount		INT
		SET @CheckCount = 0
		
		-- // 2008-08-22 by cmkwon, MySQL ���� �ҽ� ���� - 
		--SET @o_Err = 0
		--SET @o_RecvCharacterUID = 0
		--
		---- �����ϴ� �������� üũ�Ѵ�.
		--SET @o_RecvCharacterUID = ( SELECT UniqueNumber FROM dbo.td_character WITH (NOLOCK) WHERE CharacterName = @i_RecvCharacterName )
		--IF(@o_RecvCharacterUID IS NULL)
		--BEGIN
		--	SET @o_Err = 1		-- 1 ������ �������� �ʴ´�.
		--	RETURN
		--END

		DECLARE @tmRecvCharacterUID INT
		-- �����ϴ� �������� üũ�Ѵ�.
		SET @tmRecvCharacterUID = (SELECT UniqueNumber FROM dbo.td_character WITH (NOLOCK) WHERE CharacterName = @i_RecvCharacterName )
		IF(@tmRecvCharacterUID IS NULL)
		BEGIN
			-- // 2008-08-22 by cmkwon, MySQL ���� �ҽ� ���� - 
			--SET @o_Err = 1		-- 1 ������ �������� �ʴ´�.
			SELECT 1, 0, 0;
			RETURN
		END

		-- ���� �������� üũ.
		DECLARE @SendCharacterInfl TINYINT
		DECLARE @RecvCharacterInfl TINYINT
		SET @SendCharacterInfl = 0
		SET @RecvCharacterInfl = 0
		SET @SendCharacterInfl = ( SELECT InfluenceType FROM dbo.td_character WITH (NOLOCK) WHERE UniqueNumber = @i_SendCharacterUID )
		-- // 2008-08-22 by cmkwon, MySQL ���� �ҽ� ���� - 
		--SET @RecvCharacterInfl = ( SELECT InfluenceType FROM dbo.td_character WITH (NOLOCK) WHERE UniqueNumber = @o_RecvCharacterUID )
		SET @RecvCharacterInfl = ( SELECT InfluenceType FROM dbo.td_character WITH (NOLOCK) WHERE UniqueNumber = @tmRecvCharacterUID )
		IF(@SendCharacterInfl <> @RecvCharacterInfl)
		BEGIN
			-- // 2008-08-22 by cmkwon, MySQL ���� �ҽ� ���� - 
			--SET @o_Err = 4		-- 4 ���� ������ �ƴϴ�.
			SELECT 4, 0, 0;
			RETURN
		END	
		
		-- �ڽ� �ź� üũ
		SET @CheckCount = (SELECT Count(*) FROM dbo.td_FriendList WITH (NOLOCK) 
							WHERE CharacterName = @i_RecvCharacterName AND FriendName = @i_SendCharacterName  AND Type = 1)
		IF(0 <> @CheckCount)
		BEGIN
			-- // 2008-08-22 by cmkwon, MySQL ���� �ҽ� ���� - 
			--SET @o_Err = 2		-- 2 �ź� ��Ͽ� �ִ� �����̴�.
			SELECT 2, 0, 0;
			RETURN
		END	

		-- ���� �ź� üũ
		SET @CheckCount = (SELECT Count(*) FROM dbo.td_FriendList WITH (NOLOCK) 
							WHERE CharacterName = @i_SendCharacterName AND FriendName = @i_RecvCharacterName  AND Type = 1)
		IF(0 <> @CheckCount)
		BEGIN
			-- // 2008-08-22 by cmkwon, MySQL ���� �ҽ� ���� - 
			--SET @o_Err = 3		-- 3 ������ ���� �ź��� �����̴�.
			SELECT 3, 0, 0;
			RETURN
		END	

		-- ���� �߰�
		-- // 2008-08-22 by cmkwon, MySQL ���� �ҽ� ���� - 
		--INSERT INTO dbo.td_Letter (RecvCharacterUID, SendCharacterName, SendDate, CheckRead, Title, Content)
		--	VALUES (@o_RecvCharacterUID, @i_SendCharacterName, @i_SendDate, 0, @i_Title, @i_Content)
		INSERT INTO dbo.td_Letter (RecvCharacterUID, SendCharacterName, SendDate, CheckRead, Title, Content)
			VALUES (@tmRecvCharacterUID, @i_SendCharacterName, @i_SendDate, 0, @i_Title, @i_Content)

		-- // 2008-08-22 by cmkwon, MySQL ���� �ҽ� ���� - 
		--RETURN SCOPE_IDENTITY();
		SELECT 0, @tmRecvCharacterUID, SCOPE_IDENTITY();	-- // 2008-08-22 by cmkwon, MySQL ���� �ҽ� ���� - 
	GO
	**************************************************************************/
	QPARAM_SEND_LETTER *pRMsg = (QPARAM_SEND_LETTER*)q.pQueryParam;
	CFieldIOCPSocket	*pFieldIOCPSocket = q.pFieldIOCPSocket;
	if (NULL == pFieldIOCPSocket)
	{
		util::del(pRMsg);
		return;
	}

	UID64_t		LetterUID = 0;
	UID32_t		RecvCharacterUID = 0;
	int			ErrNum = 0;		// 2008-08-22 by cmkwon, MySQL ���� �ҽ� ���� - �ڷ��� ���� BYTE�� int�� ���� ��
	ATUM_DATE_TIME	SendDate;
	SendDate.SetCurrentDateTime();
	char SendTime[SIZE_MAX_SQL_DATETIME_STRING];
	SendDate.GetSQLDateTimeString(SendTime, SIZE_MAX_SQL_DATETIME_STRING);

// 2008-08-22 by cmkwon, MySQL ���� �ҽ� ���� - 
// 	SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &LetterUID, 0, NULL);
// 	SQLBindParameter(hstmt, 2, SQL_PARAM_OUTPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &ErrNum, 0, NULL);
// 	SQLBindParameter(hstmt, 3, SQL_PARAM_OUTPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &RecvCharacterUID, 0, NULL);
// 	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pRMsg->RecvCharacterName, 0, NULL);
// 	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pFieldIOCPSocket->m_character.CharacterUniqueNumber, 0, NULL);
// 	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pFieldIOCPSocket->m_character.CharacterName, 0, NULL);
// 	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, SendTime, 0, NULL);
// 	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_LETTER_TITLE, 0, pRMsg->Title, 0, NULL);
// 	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_LETTER_CONTENT, 0, pRMsg->Content, 0, NULL);
// 
// 	RETCODE ret = SQLExecDirect(hstmt, (UCHAR*)"{ ? = call dbo.atum_Send_Letter(?,?,?,?,? ,?,?,?)}", SQL_NTS);
// 	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
// 	{
// 		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Send_Letter Failed!\n", TRUE, q.pFieldIOCPSocket, q);
// 		pFieldIOCPSocket->SendErrorMessage(T_FC_CHAT_LETTER_SEND, ERR_DB_EXECUTION_FAILED);	
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		util::del(pRMsg);
// 		return;
// 	}
// 
// 	while ( ( ret = SQLMoreResults(hstmt) ) != SQL_NO_DATA );

	///////////////////////////////////////////////////////////////////////////////
	// 2008-08-22 by cmkwon, MySQL ���� �ҽ� ���� - OUTPUT ���ڸ� Result Set �� ó��
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pRMsg->RecvCharacterName, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pFieldIOCPSocket->m_character.CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pFieldIOCPSocket->m_character.CharacterName, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, SendTime, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_LETTER_TITLE, 0, pRMsg->Title, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_LETTER_CONTENT, 0, pRMsg->Content, 0, NULL);

 	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0277, SQL_NTS);
 	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Send_Letter Failed!\n", TRUE, q.pFieldIOCPSocket, q);
 		pFieldIOCPSocket->SendErrorMessage(T_FC_CHAT_LETTER_SEND, ERR_DB_EXECUTION_FAILED);	
 		SQLFreeStmt(hstmt, SQL_CLOSE);
 		util::del(pRMsg);
 		return;
	}
	SQLBindCol(hstmt, 1, SQL_C_LONG, &(ErrNum), 0, NULL);
	SQLBindCol(hstmt, 2, SQL_C_ULONG, &(RecvCharacterUID), 0, NULL);
	SQLBindCol(hstmt, 3, SQL_C_SBIGINT, &(LetterUID), 0, NULL);
	
	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(hstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 �̸� ����� RowCount �̴� 
			ret = SQLMoreResults(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(hstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(hstmt);                 // 2008-07-25 by cmkwon, ���� Results �� �ִ����� �ѹ��� üũ�Ѵ�.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}

	if(1 == ErrNum)
	{// ������ �������� �ʴ´�.
		pFieldIOCPSocket->SendErrorMessage(T_FC_CHAT_LETTER_SEND, ERR_FRIEND_INVALID_CHARACTER_NAME);	
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);	
		return;
	}
	else if(2 == ErrNum || 3 == ErrNum)
	{// �ź� ��Ͽ� �ִ� �����̴�., ������ ���� �ź��� �����̴�.
		pFieldIOCPSocket->SendErrorMessage(T_FC_CHAT_LETTER_SEND, ERR_FRIEND_REGISTERED_PEER_REJECTLIST);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);	
		return;
	}
	else if(4 == ErrNum)
	{// ���� ������ �ƴϴ�.
		pFieldIOCPSocket->SendErrorMessage(T_FC_CHAT_LETTER_SEND, ERR_PROTOCOL_REQ_INFLUENCE_NOT_MATCHED);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);	
		return;
	}

	// 2008-08-22 by cmkwon, MySQL ���� �ҽ� ���� - üũ �߰� ��
	if(FALSE == IS_VALID_UID64(LetterUID)
		|| FALSE == IS_VALID_UNIQUE_NUMBER(RecvCharacterUID))
	{
		pFieldIOCPSocket->SendErrorMessage(T_FC_CHAT_LETTER_SEND, ERR_PROTOCOL_INVALID_CHARACTER_UNIQUENUMBER);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);	
		return;
	}

	CFieldIOCPSocket *pRecvSocket = m_pFieldIOCP8->GetFieldIOCPSocketByCharacterUniqueNumber(RecvCharacterUID);
	if(NULL != pRecvSocket)
	{
		SLETTER_INFO LetterInfo;
		util::zero(&LetterInfo, sizeof(SLETTER_INFO));
		LetterInfo.LetterUID		= LetterUID;
		LetterInfo.RecvCharacterUID	= RecvCharacterUID;
		LetterInfo.CheckRead		= 0;
		LetterInfo.SendDate			= SendDate;
		util::strncpy(LetterInfo.SendCharacterName, pFieldIOCPSocket->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
		util::strncpy(LetterInfo.Title, pRMsg->Title, SIZE_MAX_LETTER_TITLE);
		util::strncpy(LetterInfo.Content, pRMsg->Content, SIZE_MAX_LETTER_CONTENT);
		pRecvSocket->m_LetterManager.InsertLetter(&LetterInfo);

		INIT_MSG_WITH_BUFFER(MSG_FC_CHAT_LETTER_RECEIVE, T_FC_CHAT_LETTER_RECEIVE, pSMsg, SendBuf);
		util::strncpy(pSMsg->SendCharacterName, pFieldIOCPSocket->m_character.CharacterName,SIZE_MAX_CHARACTER_NAME);
		pRecvSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_CHAT_LETTER_RECEIVE));
	}
	
	pFieldIOCPSocket->SendAddMessageType(T_FC_CHAT_LETTER_SEND_OK);

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);		
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_GetAllLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		EP3 �����ý��� - ��ü ���� ��������
/// \author		dhjin
/// \date		2008-05-09 ~ 2008-05-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_GetAllLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_GetAllLetter
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_Get_AllLetter
	-- DESC		 : ��ü ���� ��������
	-- // 2008-05-09 by dhjin, EP3 �����ý��� - 
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Get_AllLetter
		@i_RecvCharacterUID		INT,
		@i_RecvCharacterInfl	TINYINT,
		@i_LetterPassedRangeDay	INT
	AS
		SELECT A.AllLetterUID, A.SendCharacterName, A.SendDate, A.Title, A.Content, 0 
			FROM dbo.td_AllLetter AS A WITH (NOLOCK) INNER JOIN dbo.td_ReadAllLetter AS R WITH (NOLOCK) 
			ON A.SendCharacterInfl = @i_RecvCharacterInfl AND A.AllLetterUID = R.AllLetterUID AND R.RecvCharacterUID <> @i_RecvCharacterUID
			-- ��ü ������ ���� ����
		UNION
		SELECT A.AllLetterUID, A.SendCharacterName, A.SendDate, A.Title, A.Content, 1 
			FROM dbo.td_AllLetter AS A WITH (NOLOCK) INNER JOIN dbo.td_ReadAllLetter AS R WITH (NOLOCK) 
			ON A.SendCharacterInfl = @i_RecvCharacterInfl AND R.RecvCharacterUID = @i_RecvCharacterUID AND A.AllLetterUID = R.AllLetterUID AND 0 = R.CheckDelete
			-- ��ü ������ �о����� ���������� ����
	GO
	**************************************************************************/
	CFieldIOCPSocket	*pFieldIOCPSocket = q.pFieldIOCPSocket;
	if (NULL == pFieldIOCPSocket)
	{// 2008-07-21 by dhjin, EP3 - Nullüũ �߰�
		return;
	}

	INT LetterPassedRangeDay = LETTER_PASSED_RANGE_DAY;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pFieldIOCPSocket->m_character.CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pFieldIOCPSocket->m_character.InfluenceType, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &LetterPassedRangeDay, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0089, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Get_AllLetter Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	// Clear any result sets generated.
	SLETTER_INFO		SLetterInfo;
	util::zero(&SLetterInfo, sizeof(SLETTER_INFO));	// initialize to 0x00
	vectSLETTER_INFO	vectAllLetterInfo;
	vectAllLetterInfo.clear();
	SQL_TIMESTAMP_STRUCT SendDate;

	SQLINTEGER		arrCB[7] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS};

	SQLBindCol(hstmt, 1, SQL_C_SBIGINT,		&SLetterInfo.LetterUID,			0,							&arrCB[1]);
	SQLBindCol(hstmt, 2, SQL_C_CHAR,		SLetterInfo.SendCharacterName,	SIZE_MAX_CHARACTER_NAME,	&arrCB[2]);
	SQLBindCol(hstmt, 3, SQL_C_TIMESTAMP,	&SendDate,						0,							&arrCB[3]);
	SQLBindCol(hstmt, 4, SQL_C_CHAR,		SLetterInfo.Title,				SIZE_MAX_LETTER_TITLE,		&arrCB[4]);
	SQLBindCol(hstmt, 5, SQL_C_CHAR,		SLetterInfo.Content,			SIZE_MAX_LETTER_CONTENT,	&arrCB[5]);
	SQLBindCol(hstmt, 6, SQL_C_UTINYINT,	&SLetterInfo.CheckRead,			0,							&arrCB[6]);

	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA )
	{
		SLetterInfo.SendDate			= SendDate;
		SLetterInfo.RecvCharacterUID	= pFieldIOCPSocket->m_character.CharacterUniqueNumber;
		vectAllLetterInfo.push_back(SLetterInfo);
		util::zero(&SLetterInfo, sizeof(SLETTER_INFO));	// initialize to 0x00
	}

	pFieldIOCPSocket->m_LetterManager.InitAllLetter(&vectAllLetterInfo);
	
	SQLFreeStmt(hstmt, SQL_CLOSE);	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_SendAllLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		EP3 ���� �ý��� - ��ü ���� ������
/// \author		dhjin
/// \date		2008-05-09 ~ 2008-05-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_SendAllLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_SendAllLetter
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_Send_AllLetter_ByLeader
	-- DESC		 : �����ο��� ���� �߼����� ���� ���� �߰�
	-- // 2008-04-14 by dhjin, EP3 �����ý��� - 
	--				-- // 2008-08-22 by cmkwon, MySQL ���� �ҽ� ���� - RETURN�� Result Set�� ����
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Send_AllLetter_ByLeader
		@i_SendCharacterInfl	TINYINT,
		@i_SendCharacterUID		INT,
		@i_SendCharacterName	VARCHAR(20),
		@i_SendDate				DATETIME,
		@i_Title				VARCHAR(40),
		@i_Content				VARCHAR(1000)
	AS
		-- ���� �߰�
		INSERT INTO dbo.td_AllLetter (SendCharacterInfl, SendCharacterUID, SendCharacterName, SendDate, Title, Content)
			VALUES (@i_SendCharacterInfl, @i_SendCharacterUID, @i_SendCharacterName, @i_SendDate, @i_Title, @i_Content)

		-- // 2008-08-22 by cmkwon, MySQL ���� �ҽ� ���� - 
		--RETURN SCOPE_IDENTITY();
		IF (@@ERROR <> 0)
		BEGIN
			SELECT 0;
			RETURN;
		END

		SELECT SCOPE_IDENTITY();	-- // 2008-08-22 by cmkwon, MySQL ���� �ҽ� ���� - 
	GO
	**************************************************************************/
	QPARAM_SEND_ALLLETTER *pRMsg = (QPARAM_SEND_ALLLETTER*)q.pQueryParam;
	CFieldIOCPSocket	*pFieldIOCPSocket = q.pFieldIOCPSocket;
	if (NULL == pFieldIOCPSocket)
	{
		util::del(pRMsg);
		return;
	}

	UID64_t		AllLetterUID = 0;
	ATUM_DATE_TIME	SendDate;
	SendDate.SetCurrentDateTime();
	char SendTime[SIZE_MAX_SQL_DATETIME_STRING];
	SendDate.GetSQLDateTimeString(SendTime, SIZE_MAX_SQL_DATETIME_STRING);
// 2008-08-22 by cmkwon, MySQL ���� �ҽ� ���� - 
// 	SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &AllLetterUID, 0, NULL);
// 	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pFieldIOCPSocket->m_character.InfluenceType, 0, NULL);
// 	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pFieldIOCPSocket->m_character.CharacterUniqueNumber, 0, NULL);
// 	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pFieldIOCPSocket->m_character.CharacterName, 0, NULL);
// 	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, SendTime, 0, NULL);
// 	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_LETTER_TITLE, 0, pRMsg->Title, 0, NULL);
// 	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_LETTER_CONTENT, 0, pRMsg->Content, 0, NULL);
// 
// 	RETCODE ret = SQLExecDirect(hstmt, (UCHAR*)"{ ? = call dbo.atum_Send_AllLetter_ByLeader(?,?,?,?,? ,?)}", SQL_NTS);
// 	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
// 	{
// 		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Send_AllLetter_ByLeader Failed!\n", TRUE, q.pFieldIOCPSocket, q);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		util::del(pRMsg);
// 		return;
// 	}
// 
// 	while ( ( ret = SQLMoreResults(hstmt) ) != SQL_NO_DATA );

	///////////////////////////////////////////////////////////////////////////////
	// 2008-08-22 by cmkwon, MySQL ���� �ҽ� ���� - 
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pFieldIOCPSocket->m_character.InfluenceType, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pFieldIOCPSocket->m_character.CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pFieldIOCPSocket->m_character.CharacterName, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, SendTime, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_LETTER_TITLE, 0, pRMsg->Title, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_LETTER_CONTENT, 0, pRMsg->Content, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0278, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Send_AllLetter_ByLeader Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLBindCol(hstmt, 1, SQL_C_SBIGINT, &(AllLetterUID), 0, NULL);
	
	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(hstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 �̸� ����� RowCount �̴� 
			ret = SQLMoreResults(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(hstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(hstmt);                 // 2008-07-25 by cmkwon, ���� Results �� �ִ����� �ѹ��� üũ�Ѵ�.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}
	// 2008-08-22 by cmkwon, MySQL ���� �ҽ� ���� - üũ �߰�
	if(FALSE == IS_VALID_UID64(AllLetterUID))
	{
		pFieldIOCPSocket->SendErrorMessage(T_FC_CHAT_ALLLETTER_SEND, ERR_DB_NO_SUCH_DATA);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}

	pFieldIOCPSocket->SendAddMessageType(T_FC_CHAT_ALLLETTER_SEND_OK);
	
	SLETTER_INFO LetterInfo;
	util::zero(&LetterInfo, sizeof(SLETTER_INFO));
	LetterInfo.LetterUID		= AllLetterUID;
	LetterInfo.CheckRead		= 0;
	LetterInfo.SendDate			= SendDate;
	util::strncpy(LetterInfo.SendCharacterName, pFieldIOCPSocket->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
	util::strncpy(LetterInfo.Title, pRMsg->Title, SIZE_MAX_LETTER_TITLE);
	util::strncpy(LetterInfo.Content, pRMsg->Content, SIZE_MAX_LETTER_CONTENT);
	m_pFieldIOCP8->SendAllLetterByInfluence(pFieldIOCPSocket->m_character.InfluenceType, &LetterInfo);

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);		
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_ReadAllLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		EP3 ���� �ý��� - ��ü ���� �б�
/// \author		dhjin
/// \date		2008-05-09 ~ 2008-05-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_ReadAllLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_ReadAllLetter
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_Insert_ReadAllLetter_ByRead
	-- DESC		 : �����ο��� ���� �߼� ����
	-- // 2008-05-08 by dhjin, EP3 �����ý��� - 
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Insert_ReadAllLetter_ByRead
		@i_AllLetterUID			BIGINT,
		@i_RecvCharacterUID		INT,
	AS
		INSERT INTO dbo.td_ReadAllLetter (AllLetterUID, RecvCharacterUID, CheckDelete)
			VALUES (@i_AllLetterUID, @i_RecvCharacterUID, 0)
	GO
	**************************************************************************/
	QPARAM_READ_ALLLETTER *pRMsg = (QPARAM_READ_ALLLETTER*)q.pQueryParam;
	CFieldIOCPSocket	*pFieldIOCPSocket = q.pFieldIOCPSocket;
	if (NULL == pFieldIOCPSocket)
	{// 2008-07-21 by dhjin, EP3 - Nullüũ �߰�
		util::del(pRMsg);
		return;
	}

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->LetterUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->RecvCharacterUID, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0090, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Insert_ReadAllLetter_ByRead Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}

	pFieldIOCPSocket->m_LetterManager.ReadAllLetter(pRMsg->LetterUID);
	pFieldIOCPSocket->m_LetterManager.SendAllLetterInfo(pRMsg->LetterUID);

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_DeleteAllLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		EP3 ���� �ý��� - ��ü ���� ����
/// \author		dhjin
/// \date		2008-05-09 ~ 2008-05-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_DeleteAllLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_DeleteAllLetter
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_Delete_ReadAllLetter
	-- DESC		 : �����ο��� ���� �߼� ����
	-- // 2008-05-08 by dhjin, EP3 �����ý��� - 
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Delete_ReadAllLetter
		@i_AllLetterUID			BIGINT,
		@i_RecvCharacterUID		INT,
	AS
		UPDATE dbo.td_ReadAllLetter SET CheckDelete = 1 WHERE AllLetterUID = @i_AllLetterUID AND RecvCharacterUID = @i_RecvCharacterUID
	GO
	**************************************************************************/
	QPARAM_DELETE_READALLLETTER *pRMsg = (QPARAM_DELETE_READALLLETTER*)q.pQueryParam;
	CFieldIOCPSocket	*pFieldIOCPSocket = q.pFieldIOCPSocket;
	if (NULL == pFieldIOCPSocket)
	{// 2008-07-21 by dhjin, EP3 - Nullüũ �߰�
		util::del(pRMsg);
		return;
	}

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->LetterUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->RecvCharacterUID, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0091, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Delete_ReadAllLetter Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_DeleteOldAllLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		EP3 ���� �ý��� - ������ ��ü ���� ����
/// \author		dhjin
/// \date		2008-05-09 ~ 2008-05-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_DeleteOldAllLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_DeleteOldAllLetter
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_Delete_AllLetter
	-- DESC		 : ��ü ���� ����
	-- // 2008-05-09 by dhjin, EP3 �����ý��� - 
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Delete_AllLetter
		@i_AllLetterUID			BIGINT,
	AS
		DELETE FROM dbo.td_ReadAllLetter WHERE AllLetterUID = @i_AllLetterUID
		DELETE FROM dbo.td_AllLetter WHERE AllLetterUID = @i_AllLetterUID
	GO
	**************************************************************************/
	QPARAM_DELETE_ALL_LETTER *pRMsg = (QPARAM_DELETE_ALL_LETTER*)q.pQueryParam;
	CFieldIOCPSocket	*pFieldIOCPSocket = q.pFieldIOCPSocket;
	if (NULL == pFieldIOCPSocket)
	{// 2008-07-21 by dhjin, EP3 - Nullüũ �߰�
		util::del(pRMsg);
		return;
	}

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->AllLetterUID, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0092, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Delete_AllLetter Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);	
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_GetLogGuildStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		
/// \author		dhjin
/// \date		2006-09-27 ~ 2006-09-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_GetLogGuildStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_GET_GUILDSTORE 	*pQParam				= (QPARAM_GET_GUILDSTORE*)q.pQueryParam;
	CFieldIOCPSocket		*pFISock				= q.pFieldIOCPSocket;

	///////////////////////////////////////////////////////////////////////////////
	// 2007-12-03 by cmkwon, ���� �α� DB ���� ���� �ϱ� ���� ���� - ����â�� �α״� LogDB �������� �˻� �Ѵ�
	CODBCStatement logDBOdbcStmt;
// 2008-07-11 by cmkwon, MySQL ������ ���� ����(IP,Port �� DB ���� ����) - 
//	BOOL bRet = logDBOdbcStmt.Init((LPCSTR)g_pFieldGlobal->GetLogDBODBCDSN(), (LPCSTR)g_pFieldGlobal->GetLogDBODBCUID(), (LPCSTR)g_pFieldGlobal->GetLogDBODBCPASSWORD());
	BOOL bRet = logDBOdbcStmt.Init(g_pGlobal->GetLogDBDBServerIP(), g_pGlobal->GetLogDBDBServerPort(), g_pGlobal->GetLogDBDBServerDatabaseName(), (char*)g_pGlobal->GetLogDBODBCUID(), (char*)g_pGlobal->GetLogDBODBCPASSWORD(), g_pGlobal->GetMainWndHandle());
	if (!bRet)
	{
		// 2008-09-05 by cmkwon, DBServer ���� ���� �ý��� �α� ����� - 
		g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] fail to connect DBServer(CAtumFieldDBManager::QP_GetLogGuildStoreItem_) !! %s,%d %s %s %s\r\n"
			, g_pGlobal->GetLogDBDBServerIP(), g_pGlobal->GetLogDBDBServerPort(), g_pGlobal->GetLogDBDBServerDatabaseName(), (char*)g_pGlobal->GetLogDBODBCUID(), (char*)g_pGlobal->GetLogDBODBCPASSWORD());
		util::del(pQParam);
		return;
	}

	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name: atum_log_Get_GuildStore
	-- Desc: 2006-09-27 by dhjin
	--====
	CREATE PROCEDURE atum_log_Get_GuildStore
		@i_GuildUID						INT
		AS
			SELECT   LogType, Time, CharacterUID, CharacterName, ItemNum, 
						PrefixCodeNum, SuffixCodeNum, ItemChangeCount
			FROM     atum_log_GuildItem
			WHERE	 (DATEDIFF(hh, Time, GETDATE()) < 48) AND (GuildUID = @i_GuildUID)
			ORDER BY Time
	GO
	**************************************************************************/
// 2007-12-03 by cmkwon, ���� �α� DB ���� ���� �ϱ� ���� ���� - �Ʒ��� ���� ������
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,	&pQParam->GuildUID, 0, NULL);
//	
//	RETCODE			ret;
//	ret = SQLExecDirect(hstmt, (UCHAR*)"{call dbo.atum_log_Get_GuildStore(?)}", SQL_NTS);
//
//	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA) {
//		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_log_Get_GuildStore Failed!\n", TRUE, q.pFieldIOCPSocket, q);
//		SQLFreeStmt(hstmt, SQL_CLOSE);
//		util::del(pQParam);
//		return;
//	}
//
//	SQLINTEGER		arrCB[9] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
//	SQL_TIMESTAMP_STRUCT tmpTimeStamp;
//	
//	MSG_FC_STORE_LOG_GUILD_ITEM_OK TempLogMsg;
//	vector<MSG_FC_STORE_LOG_GUILD_ITEM_OK>		vectorMSG_FC_STORE_LOG_GUILD_ITEM_OK0;
//
//	SQLBindCol(hstmt, 1, SQL_C_UTINYINT,	&TempLogMsg.LogType,			0,							&arrCB[1]);
//	SQLBindCol(hstmt, 2, SQL_C_TIMESTAMP,	&tmpTimeStamp,					0,							&arrCB[2]);
//	SQLBindCol(hstmt, 3, SQL_C_ULONG,		&TempLogMsg.CharacterUID,		0,							&arrCB[3]);
//	SQLBindCol(hstmt, 4, SQL_C_CHAR,		&TempLogMsg.CharacterName,		SIZE_MAX_CHARACTER_NAME,	&arrCB[4]);
//	SQLBindCol(hstmt, 5, SQL_C_ULONG,		&TempLogMsg.ItemNum,			0,							&arrCB[5]);
//	SQLBindCol(hstmt, 6, SQL_C_ULONG,		&TempLogMsg.PrefixCodeNum,		0,							&arrCB[6]);
//	SQLBindCol(hstmt, 7, SQL_C_ULONG,		&TempLogMsg.SuffixCodeNum,		0,							&arrCB[7]);
//	SQLBindCol(hstmt, 8, SQL_C_LONG,		&TempLogMsg.ItemChangeCount,	0,							&arrCB[8]);
//
//	if (NULL == pFISock || FALSE == pFISock->IsValidCharacter(FALSE))
//	{
//		SQLFreeStmt(hstmt, SQL_CLOSE);
//		util::del(pQParam);
//		return;
//	}
//	
//	util::zero(&TempLogMsg, sizeof(MSG_FC_STORE_LOG_GUILD_ITEM_OK));	// initialize to 0x00
//	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA )
//	{
//		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
//		{
//			ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_log_Get_GuildStore Fetching Failed!\n", TRUE, q.pFieldIOCPSocket, q);
//			break;
//		}
//		
//		TempLogMsg.Time	= tmpTimeStamp;
//		vectorMSG_FC_STORE_LOG_GUILD_ITEM_OK0.push_back(TempLogMsg);
//		util::zero(&TempLogMsg, sizeof(MSG_FC_STORE_LOG_GUILD_ITEM_OK));	// initialize to 0x00
//	}// end_while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA )
//	SQLFreeStmt(hstmt, SQL_CLOSE);		// reset cursor

	// 2007-12-03 by cmkwon, ���� �α� DB ���� ���� �ϱ� ���� ���� - 
	SQLHSTMT logDBstmt = logDBOdbcStmt.GetSTMTHandle();
	SQLBindParameter(logDBstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,	&pQParam->GuildUID, 0, NULL);
	
	RETCODE			ret;
	ret = SQLExecDirect(logDBstmt, PROCEDURE_080822_0093, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA) {
		logDBOdbcStmt.ProcessLogMessages(SQL_HANDLE_STMT, logDBstmt,"@atum_log_Get_GuildStore 1 Failed!\r\n", TRUE);
		logDBOdbcStmt.FreeStatement();
		util::del(pQParam);
		return;
	}

	SQLINTEGER		arrCB[9] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQL_TIMESTAMP_STRUCT tmpTimeStamp;
	
	MSG_FC_STORE_LOG_GUILD_ITEM_OK TempLogMsg;
	vector<MSG_FC_STORE_LOG_GUILD_ITEM_OK>		vectorMSG_FC_STORE_LOG_GUILD_ITEM_OK0;

	// 2007-12-03 by cmkwon, ���� �α� DB ���� ���� �ϱ� ���� ���� - 
	SQLBindCol(logDBstmt, 1, SQL_C_UTINYINT,	&TempLogMsg.LogType,			0,							&arrCB[1]);
	SQLBindCol(logDBstmt, 2, SQL_C_TIMESTAMP,	&tmpTimeStamp,					0,							&arrCB[2]);
	SQLBindCol(logDBstmt, 3, SQL_C_ULONG,		&TempLogMsg.CharacterUID,		0,							&arrCB[3]);
	SQLBindCol(logDBstmt, 4, SQL_C_CHAR,		&TempLogMsg.CharacterName,		SIZE_MAX_CHARACTER_NAME,	&arrCB[4]);
	SQLBindCol(logDBstmt, 5, SQL_C_ULONG,		&TempLogMsg.ItemNum,			0,							&arrCB[5]);
	SQLBindCol(logDBstmt, 6, SQL_C_ULONG,		&TempLogMsg.PrefixCodeNum,		0,							&arrCB[6]);
	SQLBindCol(logDBstmt, 7, SQL_C_ULONG,		&TempLogMsg.SuffixCodeNum,		0,							&arrCB[7]);
	SQLBindCol(logDBstmt, 8, SQL_C_LONG,		&TempLogMsg.ItemChangeCount,	0,							&arrCB[8]);

	if (NULL == pFISock || FALSE == pFISock->IsValidCharacter(FALSE))
	{
		logDBOdbcStmt.FreeStatement();
		util::del(pQParam);
		return;
	}
	
	util::zero(&TempLogMsg, sizeof(MSG_FC_STORE_LOG_GUILD_ITEM_OK));	// initialize to 0x00
	while ( (ret = SQLFetch(logDBstmt)) != SQL_NO_DATA )
	{
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
		{
			logDBOdbcStmt.ProcessLogMessages(SQL_HANDLE_STMT, logDBstmt,"@atum_log_Get_GuildStore 2 Failed!\r\n", TRUE);
			break;
		}
		
		TempLogMsg.Time	= tmpTimeStamp;
		vectorMSG_FC_STORE_LOG_GUILD_ITEM_OK0.push_back(TempLogMsg);
		util::zero(&TempLogMsg, sizeof(MSG_FC_STORE_LOG_GUILD_ITEM_OK));	// initialize to 0x00
	}// END - while ( (ret = SQLFetch(logDBstmt)) != SQL_NO_DATA )
	logDBOdbcStmt.FreeStatement();
	
	////////////////////////////
	BYTE SendBuf[SIZE_MAX_PACKET];
	int			nSendBytes		= 0;
	*(MessageType_t*)(SendBuf)	= T_FC_STORE_LOG_GUILD_ITEM_OK_HEADER;
	nSendBytes					= SIZE_FIELD_TYPE_HEADER;
		
	for (int i=0; i < vectorMSG_FC_STORE_LOG_GUILD_ITEM_OK0.size(); i++)
	{
		if(nSendBytes + MSG_SIZE(MSG_FC_STORE_LOG_GUILD_ITEM_OK) > SIZE_MAX_PACKET)
		{
			pFISock->SendAddData(SendBuf, nSendBytes);
			nSendBytes					= 0;
		}

		*((MessageType_t*)(SendBuf + nSendBytes))	= T_FC_STORE_LOG_GUILD_ITEM_OK;
		nSendBytes									+= SIZE_FIELD_TYPE_HEADER;
		MSG_FC_STORE_LOG_GUILD_ITEM_OK *pRMsg		= (MSG_FC_STORE_LOG_GUILD_ITEM_OK*)(SendBuf + nSendBytes);
		*pRMsg										= vectorMSG_FC_STORE_LOG_GUILD_ITEM_OK0[i];
		nSendBytes									+= sizeof(MSG_FC_STORE_LOG_GUILD_ITEM_OK);
	}

	if(nSendBytes > 0)
	{
		pFISock->SendAddData(SendBuf, nSendBytes);
		nSendBytes						= 0;
	}

	// STORE_LOG_GUILD_ITEM_OK_DONE ����
	pFISock->SendAddMessageType(T_FC_STORE_LOG_GUILD_ITEM_OK_DONE);
	util::del(pQParam);
	return;
}

// 2013-05-22 by jhseol, �ҽ����� - �ּ�����

// QT_GetStoreItem
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_NewGetStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		
/// \author		cmkwon
/// \date		2007-01-29 ~ 2007-01-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_NewGetStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FC_STORE_GET_ITEM	*pRMsg				= (MSG_FC_STORE_GET_ITEM*)q.pQueryParam;
	CFieldIOCPSocket		*pFieldIOCPSocket	= q.pFieldIOCPSocket;
	BYTE					tmpItemStorage		= pRMsg->ItemStorage0;

	if(NULL == pFieldIOCPSocket
		|| FALSE == pFieldIOCPSocket->IsValidCharacter(FALSE))
	{
		// 2012-12-11 by hskim, ��Ʈ�� ������ ���� ���� ���� (DB ó���� ���������� ���� ��Ŷ�� ����)
		if( pFieldIOCPSocket )
		{
			pFieldIOCPSocket->m_bProcessingStoreGetItem = FALSE;
		}
		// end 2012-12-11 by hskim, ��Ʈ�� ������ ���� ���� ���� (DB ó���� ���������� ���� ��Ŷ�� ����)

		util::del(pRMsg);

		return;
	}

	RETCODE			ret;
	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-29 by cmkwon, 1. ������ �ε�
	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name:
	-- Desc:
	--		2005-12-07 by cmkwon, ������ġ
	--====
	CREATE PROCEDURE atum_GetStoreItem
		@accNum			INT,
		@possessNum		INT,
		@ItemStorage	TINYINT				-- 2005-12-07 by cmkwon,
	AS
		SELECT *
		FROM td_Store
		WHERE AccountUniqueNumber = @accNum AND Possess = @possessNum AND ItemStorage = @ItemStorage
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,	&pRMsg->AccountUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,	&pRMsg->PossessCharacter, 0, NULL);
	SQLBindParameter(hstmt, 3,SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRMsg->ItemStorage0, 0, NULL);		// 2005-12-07 by cmkwon
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0094, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_GetStoreItem Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);

		pFieldIOCPSocket->m_bProcessingStoreGetItem = FALSE;		// 2012-12-11 by hskim, ��Ʈ�� ������ ���� ���� ���� (DB ó���� ���������� ���� ��Ŷ�� ����)

		util::del(pRMsg);
		return;
	}

	vectITEM_GENERALPtr	vectItemGPtrList;		// 2007-01-29 by cmkwon
	vectENCHANT			vectEnchantList;		// 2007-01-29 by cmkwon
	SQL_TIMESTAMP_STRUCT tmCreatedTimeStamp;

	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - Main���� ������ UID �߰�
	SQLINTEGER		arrCB[19] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,
								SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};

	if(SQL_NO_DATA != ret)
	{
		ITEM_GENERAL	itemG(NULL);		// ��ų �̿��� �����ۿ�
		SQLBindCol(hstmt, 1, SQL_C_SBIGINT, &itemG.UniqueNumber, 0,				&arrCB[1]);
		SQLBindCol(hstmt, 2, SQL_C_ULONG, &itemG.AccountUniqueNumber, 0,		&arrCB[2]);
		SQLBindCol(hstmt, 3, SQL_C_ULONG, &itemG.Possess, 0,					&arrCB[3]);
		SQLBindCol(hstmt, 4, SQL_C_UTINYINT, &itemG.ItemStorage, 0,				&arrCB[4]);
		SQLBindCol(hstmt, 5, SQL_C_UTINYINT, &itemG.Wear, 0,					&arrCB[5]);
		SQLBindCol(hstmt, 6, SQL_C_SLONG, &itemG.CurrentCount, 0,				&arrCB[6]);
// 2009-08-25 by cmkwon, ������� �ʴ� �ʵ� ����(td_Store.ScarcityNumber) - �Ʒ��� �ε��� ����
//		SQLBindCol(hstmt, 7, SQL_C_SBIGINT, &itemG.ScarcityNumber, 0,			&arrCB[7]);
		SQLBindCol(hstmt, 7, SQL_C_SLONG, &itemG.ItemWindowIndex, 0,			&arrCB[7]);
		SQLBindCol(hstmt, 8, SQL_C_SLONG, &itemG.ItemNum, 0,					&arrCB[8]);
		SQLBindCol(hstmt, 9, SQL_C_SSHORT, &itemG.NumOfEnchants, 0,				&arrCB[9]);
		SQLBindCol(hstmt, 10, SQL_C_LONG, &itemG.PrefixCodeNum, 0,				&arrCB[10]);
		SQLBindCol(hstmt, 11, SQL_C_LONG, &itemG.SuffixCodeNum, 0,				&arrCB[11]);
		SQLBindCol(hstmt, 12, SQL_C_FLOAT, &itemG.CurrentEndurance, 0,			&arrCB[12]);
		SQLBindCol(hstmt, 13, SQL_C_LONG, &itemG.ColorCode, 0,					&arrCB[13]);
		SQLBindCol(hstmt, 14, SQL_C_LONG, &itemG.UsingTimeStamp, 0,				&arrCB[14]);
		SQLBindCol(hstmt, 15, SQL_C_TIMESTAMP, &tmCreatedTimeStamp, 0,			&arrCB[15]);
		SQLBindCol(hstmt, 16, SQL_C_LONG, &itemG.ShapeItemNum, 0,				&arrCB[16]);		// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - ShapeItemNum �ʵ� �߰�
		SQLBindCol(hstmt, 17, SQL_C_SBIGINT, &itemG.MainSvrItemUID, 0,			&arrCB[17]);		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - Main���� ������ UID �߰�
		SQLBindCol(hstmt, 18, SQL_C_ULONG, &itemG.CoolingTimeStamp, 0,			&arrCB[18]);		// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������

		util::zero(&itemG, sizeof(ITEM_GENERAL));
		util::zero(&tmCreatedTimeStamp, sizeof(tmCreatedTimeStamp));
		UID64_t tmSPICurrentCount = 0;			// 2012-10-08 by khkim, GLog - SPI �ݾ��� ������ �Ӽ� ����
		while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA )
		{
			if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
			{
				ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_GetStoreItem Fetching Failed!\n", TRUE, q.pFieldIOCPSocket, q);
				break;
			}

			ITEM *pItemInfo = m_pFieldIOCP8->GetItemInfo(itemG.ItemNum);
			if (pItemInfo == NULL)
			{
				g_pGlobal->WriteSystemLogEX(TRUE, "  [Error]Invaild Item in DB in CAtumFieldDBManager::QP_GetStoreItem() -> %10s, %d \r\n",
					pFieldIOCPSocket->m_character.CharacterName, itemG.ItemNum);

				// 2009-11-20 by cmkwon, ���� ������ �ε��� ��ȿ���� ���� ������ ó�� ���� - CAtumFieldDBManager::QP_NewGetStoreItem#, ���ӷα��߰�, DB ������ ����
				CAtumLogSender::SendLogMessageITEMDelete(q.pFieldIOCPSocket, &itemG, IDT_INVALID_ITEMNUM);
				QPARAM_DELETESTOREITEM *pQParam	= new QPARAM_DELETESTOREITEM;
				pQParam->AccountUniqueNumber	= q.pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber;
				pQParam->ItemUniqueNumber		= itemG.UniqueNumber;
				this->MakeAndEnqueueQuery(QT_DeleteStoreItem, q.pFieldIOCPSocket, q.pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber, pQParam);

				util::zero(&itemG, sizeof(ITEM_GENERAL));
				util::zero(&tmCreatedTimeStamp, sizeof(tmCreatedTimeStamp));
				continue;
			}		
			itemG.CreatedTime			= tmCreatedTimeStamp;

			// 2007-01-29 by cmkwon, �ڵ� ���� ������ ���� üũ
			if(COMPARE_BIT_FLAG(pItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE))
			{
				ATUM_DATE_TIME expireATime;
				ATUM_DATE_TIME curATime { true };
				expireATime			= itemG.CreatedTime;
				// 2009-10-22 by cmkwon, �ð� �������� ������ �ý��� ���� - CAtumFieldDBManager::QP_NewGetStoreItem#, 
				//expireATime.AddDateTime(0, 0, 0, pItemInfo->Endurance, 0, 0);

				// 2013-07-24 by bckim, �������� ������ ������� ���� ���� 
				if( pItemInfo->IsExistDesParam(DES_BUFF_TO_RETURN_USER) && itemG.UsingTimeStamp > 0 )
				{
					expireATime.AddDateTime(0, 0, 0, 0, CAtumSJ::GetTimeMinuteByItemKind8ItemAttribute(pItemInfo, TIME_TERM_USING_ITEM));	
				}
				else
				{
					expireATime.AddDateTime(0, 0, 0, 0, CAtumSJ::GetTimeMinuteByItemKind8ItemAttribute(pItemInfo, TIME_TERM_DELETE_ITEM));
				}
				//expireATime.AddDateTime(0, 0, 0, 0, CAtumSJ::GetTimeMinuteByItemKind8ItemAttribute(pItemInfo, TIME_TERM_DELETE_ITEM));	// 2009-10-22 by cmkwon, �ð� �������� ������ �ý��� ���� - CAtumFieldDBManager::QP_NewGetStoreItem#,
				// End. 2013-07-24 by bckim, �������� ������ ������� ���� ���� 


				if(curATime > expireATime)
				{// 2006-09-29 by cmkwon, �Ⱓ ����, �����ؾ���

					// 2008-01-23 by cmkwon, S_F, S_L: ����/�������� ���� �α׿� �߰� - �Ⱓ ����� ������
					CAtumLogSender::SendLogMessageITEMDelete(q.pFieldIOCPSocket, &itemG, IDT_EXPIRE_TIME);
					
					QPARAM_DELETESTOREITEM *pQParam	= new QPARAM_DELETESTOREITEM;
					pQParam->AccountUniqueNumber	= q.pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber;
					pQParam->ItemUniqueNumber		= itemG.UniqueNumber;
					this->MakeAndEnqueueQuery(QT_DeleteStoreItem, q.pFieldIOCPSocket, q.pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber, pQParam);

					util::zero(&itemG, sizeof(ITEM_GENERAL));
					util::zero(&tmCreatedTimeStamp, sizeof(tmCreatedTimeStamp));
					continue;
				}
			}

			// 2007-01-29 by cmkwon, �ð����� ������ ����üũ - �κ� �����۸� üũ
			if(ITEM_IN_CHARACTER == tmpItemStorage
				&& COMPARE_BIT_FLAG(pItemInfo->ItemAttribute, ITEM_ATTR_TIME_LIMITE|ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED)	// 2008-11-26 by cmkwon, ��� �� �ð�(����ð�) ���� ������ ���� - 
				&& ITEM_NOT_USING != itemG.UsingTimeStamp)
			{
				// 2008-11-26 by cmkwon, ��� �� �ð�(����ð�) ���� ������ ���� - �ε��� ���� �ð����� ���� �ð��� �����Ѵ�.
				if(COMPARE_BIT_FLAG(pItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED))
				{
					ATUM_DATE_TIME atCur { true };
					itemG.UsingTimeStamp	= max(1, atCur.GetTimeDiffTimeInSeconds(itemG.CreatedTime));
				}

				// 2008-11-26 by cmkwon, ��� �� �ð�(����ð�) ���� ������ ���� - �Ʒ��� ���� ���� ��.
				//if(pItemInfo->Time <= 1000 * itemG.UsingTimeStamp)
				if(CAtumSJ::GetTimeSecondByItemKind8ItemAttribute(pItemInfo) <= itemG.UsingTimeStamp)
				{// 2006-10-11 by cmkwon, ��� ���� �ð� üũ

					// 2008-01-23 by cmkwon, S_F, S_L: ����/�������� ���� �α׿� �߰� - �Ⱓ ����� ������
					// 2009-11-17 by cmkwon, �������� ���� ������ ������ �α� ���� �߰� - 
					CAtumLogSender::SendLogMessageITEMDelete(q.pFieldIOCPSocket, &itemG, IDT_EXPIRE_TIME, itemG.UsingTimeStamp);

					QPARAM_DELETESTOREITEM *pQParam	= new QPARAM_DELETESTOREITEM;
					pQParam->AccountUniqueNumber	= q.pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber;
					pQParam->ItemUniqueNumber		= itemG.UniqueNumber;
					this->MakeAndEnqueueQuery(QT_DeleteStoreItem, q.pFieldIOCPSocket, q.pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber, pQParam);
					util::zero(&itemG, sizeof(ITEM_GENERAL));
					util::zero(&tmCreatedTimeStamp, sizeof(tmCreatedTimeStamp));
					continue;
				}

				// ���Ǵ�Ƽ ���� ��� ������ ���� ���� ó��. 2010. 08. 09. by hsLee.
				if ( false == g_pFieldGlobal->IsArenaServer()
					 && COMPARE_BIT_FLAG( pItemInfo->ItemAttribute , ITEM_ATTR_ONLY_USE_INFINITY ) )
				{
					QPARAM_DELETESTOREITEM *pQParam	= new QPARAM_DELETESTOREITEM;
					pQParam->AccountUniqueNumber	= q.pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber;
					pQParam->ItemUniqueNumber		= itemG.UniqueNumber;
					this->MakeAndEnqueueQuery(QT_DeleteStoreItem, q.pFieldIOCPSocket, q.pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber, pQParam);
					util::zero(&itemG, sizeof(ITEM_GENERAL));
					util::zero(&tmCreatedTimeStamp, sizeof(tmCreatedTimeStamp));
					continue;
				}

			}

			//////////////////////////////////////////////////////////////////////////
			// 2007-06-04 by dhjin, �Ʒ��� ���� ���� ������ ����
			if(COMPARE_BIT_FLAG(pItemInfo->ItemAttribute, ITEM_ATTR_ARENA_ITEM))
			{
				QPARAM_DELETESTOREITEM *pQParam	= new QPARAM_DELETESTOREITEM;
				pQParam->AccountUniqueNumber	= q.pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber;
				pQParam->ItemUniqueNumber		= itemG.UniqueNumber;
				this->MakeAndEnqueueQuery(QT_DeleteStoreItem, q.pFieldIOCPSocket, q.pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber, pQParam);

				util::zero(&itemG, sizeof(ITEM_GENERAL));
				util::zero(&tmCreatedTimeStamp, sizeof(tmCreatedTimeStamp));
				continue;
			}

			// 2007-01-29 by cmkwon, item kind ��ȿ�� Ȯ��
			if (IS_MONSTER_ITEM(pItemInfo->Kind))
			{			
				g_pGlobal->WriteSystemLogEX(TRUE, "FATAL [Error]: Invaild Item Kind in DB in CAtumFieldDBManager::QP_GetStoreItem() -> %s, %s %d %d\r\n",
					GetCharacterString(&pFieldIOCPSocket->m_character, string()), pItemInfo->ItemName, pItemInfo->ItemNum, pItemInfo->Kind);

				util::zero(&itemG, sizeof(ITEM_GENERAL));
				util::zero(&tmCreatedTimeStamp, sizeof(tmCreatedTimeStamp));
				continue;
			}

			// 2012-10-08 by khkim, GLog 2��
			// GameStart�� ������ ���� GLog ��ŶSend �κ�
			if(!COMPARE_RACE(pFieldIOCPSocket->m_character.Race, RACE_OPERATION|RACE_GAMEMASTER|RACE_MONITOR|RACE_GUEST|RACE_DEMO))		// 2013-01-18 by khkim, GLog (�Ϲ������� ���)	
			{
				if(FALSE == g_pFieldGlobal->IsArenaServer())
				{
					if(TRUE == pRMsg->bMainGameStartFlag)
					{
						if(ITEM_IN_CHARACTER == pRMsg->ItemStorage0)
						{
							if( IS_COUNTABLE_ITEM(pItemInfo->Kind))
							{
								MSG_FL_LOG_ITEMSTATE LogItemStateParameter;	
								util::zero(&LogItemStateParameter, sizeof(LogItemStateParameter));		// 2013-05-30 by jhseol, GLog �ý��� ���� - �ʱ�ȭ
								LogItemStateParameter.LogType			= GLOG_ISL_GAMESTART;
								LogItemStateParameter.ItemSerial		= (UID64_t)itemG.ItemNum;		// 2013-05-30 by jhseol, GLog �ý��� ���� - ĳ����
								LogItemStateParameter.Count				= (UID64_t)itemG.CurrentCount;	// 2013-05-30 by jhseol, GLog �ý��� ���� - ĳ����
								LogItemStateParameter.CharSerial		= (UID64_t)pFieldIOCPSocket->m_character.CharacterUniqueNumber;	// 2013-05-30 by jhseol, GLog �ý��� ���� - ĳ����
								LogItemStateParameter.CharLv			= (int)pFieldIOCPSocket->m_character.Level;	// 2013-05-30 by jhseol, GLog �ý��� ���� - ĳ����
								//LogItemStateParameter.GameServerID		= g_pFieldGlobal->GetMGameServerID();
								CAtumLogSender::SendLogItemState(&LogItemStateParameter);
							}
						}
					}
				}
			}
			// end 2012-10-08 by khkim, GLog
			
			// 2007-01-29 by cmkwon, ī���ͺ� �������̸鼭 CurrentCount�� 0���� ������ ���� ó��(SPI�� ����)
			if(IS_COUNTABLE_ITEM(pItemInfo->Kind)
				&& 0 >= itemG.CurrentCount)
			{
				if(MONEY_ITEM_NUMBER == itemG.ItemNum)
				{
					itemG.CurrentCount			= 0;
				}
				else
				{
					QPARAM_DELETESTOREITEM *pQParam	= new QPARAM_DELETESTOREITEM;
					pQParam->AccountUniqueNumber	= q.pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber;
					pQParam->ItemUniqueNumber		= itemG.UniqueNumber;
					this->MakeAndEnqueueQuery(QT_DeleteStoreItem, q.pFieldIOCPSocket, q.pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber, pQParam);

					util::zero(&itemG, sizeof(ITEM_GENERAL));
					util::zero(&tmCreatedTimeStamp, sizeof(tmCreatedTimeStamp));
					continue;
				}
			}

			// 2007-01-29 by cmkwon, ������ â�� ��ȿ�� ���� üũ
			// 2009-10-22 by cmkwon, �ð� �������� ������ �ý��� ���� - 
			//if (POS_INVALID_POSITION <= pItemInfo->Position)
			if (FALSE == IS_ATTACHABLE_ITEM(pItemInfo))
			{// 2007-01-29 by cmkwon, ���� �Ұ� ������
				itemG.Wear				= WEAR_NOT_ATTACHED;

				// 2009-10-22 by cmkwon, �ð� �������� ������ �ý��� ���� - 
				//if(POS_INVALID_POSITION > itemG.ItemWindowIndex)
				if(IS_ATTACHABLE_POSITION(itemG.ItemWindowIndex))
				{// 2007-01-29 by cmkwon, �����Ǿ� �ִ� ���� üũ
					itemG.Wear				= WEAR_NOT_ATTACHED;
					itemG.ItemWindowIndex	= POS_INVALID_POSITION;
				}
			}
			else if(WEAR_ATTACHED != itemG.Wear)
			{// 2007-01-29 by cmkwon, ���� ���� ������ - �������� ���� ����

				itemG.Wear				= WEAR_NOT_ATTACHED;
				// 2009-10-22 by cmkwon, �ð� �������� ������ �ý��� ���� - 
				//if(POS_INVALID_POSITION > itemG.ItemWindowIndex)
				if(IS_ATTACHABLE_POSITION(itemG.ItemWindowIndex))
				{// 2007-01-29 by cmkwon, �����Ǿ� �ִ� ���� üũ
					itemG.ItemWindowIndex	= POS_INVALID_POSITION;
				}
			}
			else
			{// 2007-01-29 by cmkwon, ���� ���� ������ - ������ ����
				
				// 2009-10-22 by cmkwon, �ð� �������� ������ �ý��� ���� - 
				//if(POS_INVALID_POSITION <= itemG.ItemWindowIndex)
				if(FALSE == IS_ATTACHABLE_POSITION(itemG.ItemWindowIndex))
				{// 2007-01-29 by cmkwon, �����Ǿ� ���� ���� ���� üũ
					itemG.Wear				= WEAR_NOT_ATTACHED;
					itemG.ItemWindowIndex	= POS_INVALID_POSITION;
				}
			}
			
			// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)
			if( IS_ENABLE_CHANGE_ShapeItemNum(itemG.Kind) )
			{
				QPARAM_FIXED_TERM_SHAPE	*pQParamFT = new QPARAM_FIXED_TERM_SHAPE;
				pQParamFT->ItemUID = itemG.UniqueNumber;
				pQParamFT->TermType = FIXED_TERM_SHAPE;
				pQParamFT->AppliedItemNum = 0;
				pQParamFT->StartDate.Reset();
				pQParamFT->EndDate.Reset();

				this->ExecuteQuery(QT_GetFixedTerm, q.pFieldIOCPSocket, pQParamFT);

				if( 0 != pQParamFT->AppliedItemNum )
				{
					BOOL bDelete = TRUE;

					if( itemG.ShapeItemNum == pQParamFT->AppliedItemNum )
					{
						INT nCalcMin = 0;
						ATUM_DATE_TIME CurrentDate { true };
						
						nCalcMin = pQParamFT->EndDate.GetTimeDiffTimeInMinutes(CurrentDate);

						if( nCalcMin > 0 )
						{
							itemG.FixedTermShape.bActive = TRUE;
							itemG.FixedTermShape.StartDate = pQParamFT->StartDate;
							itemG.FixedTermShape.EndDate = pQParamFT->EndDate;
							itemG.FixedTermShape.TimerUID = 0;
							// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - �Ʒ��������� �Ϲ� �Ⱓ�� ������ ����ǵ��� ����
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM	// - �Ʒ��������� �Ϲ� �Ⱓ�� ������ ����ǵ��� ����
							if ( TRUE == g_pFieldGlobal->IsArenaServer() )
							{
								itemG.FixedTermShape.bActive = FALSE;				// �Ʒ������� ������ ���������� ����
								nCalcMin = MANAGE_MAX_TIME_FIXED_TERM_MINUTELY*2;	// Ÿ�̸� ���� ���ϵ��� ������ ���� ����
							}
#endif
							// enn 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - �Ʒ��������� �Ϲ� �Ⱓ�� ������ ����ǵ��� ����
							if( ITEM_IN_CHARACTER == tmpItemStorage )
							{
								if( nCalcMin < MANAGE_MAX_TIME_FIXED_TERM_MINUTELY )
								{
									itemG.FixedTermShape.TimerUID = pFieldIOCPSocket->m_TimerManager.StartTimerField(TE_TYPE_FIXED_TERM_SHAPE, nCalcMin * TIMER_DO_MINUTELY_WORK, 0, 0);
								}
							}

							bDelete = FALSE;
						}
					}

					if( TRUE == bDelete)
					{
						itemG.ShapeItemNum = 0;

						FIXED_TERM_INFO DelFixedTermShape;
						util::zero(&DelFixedTermShape, sizeof(FIXED_TERM_INFO));
					
						DelFixedTermShape.StartDate = pQParamFT->StartDate;
						DelFixedTermShape.EndDate = pQParamFT->EndDate;

						QPARAM_UPDATE_SHAPEITEMNUM *pQParam = new QPARAM_UPDATE_SHAPEITEMNUM;
						pQParam->ItemUID		= itemG.UniqueNumber;
						pQParam->nShapeItemNum	= itemG.ShapeItemNum;
						pQParam->FixedTermShape.bActive = FALSE;
						pQParam->FixedTermShape.StartDate.Reset();
						pQParam->FixedTermShape.EndDate.Reset();
						pQParam->FixedTermShape.TimerUID = 0;
						this->MakeAndEnqueueQuery(QT_UpdateShapeItemNum, q.pFieldIOCPSocket, q.pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber, pQParam);

						if( FALSE == g_pFieldGlobal->IsArenaServer() )
						{
							QPARAM_FIXED_TERM_SHAPE	*pQParamFT2 = new QPARAM_FIXED_TERM_SHAPE;
							pQParamFT2->ItemUID = itemG.UniqueNumber;
							pQParamFT2->TermType = FIXED_TERM_SHAPE;
							pQParamFT2->AppliedItemNum = 0;
							pQParamFT2->StartDate.Reset();
							pQParamFT2->EndDate.Reset();
							this->MakeAndEnqueueQuery(QT_DeleteFixedTerm, q.pFieldIOCPSocket, q.pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber, pQParamFT2);

							CAtumLogSender::SendLogMessageFixedTermShapeEnd(pFieldIOCPSocket, &itemG, pQParamFT->AppliedItemNum, DelFixedTermShape);
						}
					}
				}
				// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - �Ƹ� ���� Ȯ��
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM	// - �Ƹ� ���� Ȯ��
				else
				{		// ���ʷ�ƾ���� ���Դٴ� ���� 1. ����� ������ ���ų�, 2. ������ ���� �̰ų�, 3. �Ƹ��÷������� ����� ������ ���Ѵ�.
					if( ITEMKIND_DEFENSE == pItemInfo->Kind && ITEM_IN_CHARACTER == tmpItemStorage )
					{		// �켱 ����� �Ƹ� �÷��� ������ �ִ��� Ȯ���� �ϰ�
						QPARAM_SHAPE_STATLEVEL	*pQParamSS = new QPARAM_SHAPE_STATLEVEL;
						pQParamSS->ItemUID = itemG.UniqueNumber;		
						this->ExecuteQuery(QT_GetShapeStatLevel, q.pFieldIOCPSocket, pQParamSS);
						itemG.FixedTermShape.nStatLevel			= pQParamSS->nStatLevel;
						itemG.FixedTermShape.nStatShapeItemNum	= pQParamSS->nStatShapeItemNum;
						util::del(pQParamSS);
					}

					if ( FALSE == g_pFieldGlobal->IsArenaServer() )		// �Ʒ��� �������� ���� ��ƾ�� �����Ͽ� ����� �÷��� ������ �ִ�, ���� ���������� ��� �ϵ��� �����Ѵ�.
					{
						if ( SHAPE_STAT_INIT_LEVEL < itemG.FixedTermShape.nStatLevel )
						{
							COLLECTION_INFO* tempPtr = pFieldIOCPSocket->m_ArmorCollection.GetCollectionInfoPtr(itemG.ShapeItemNum);
							if ( NULL != tempPtr )
							{
								BOOL bShapeActvied = FALSE;
								if ( SHAPE_STAT_FIXED_TIME_LIMITE_LEVEL <= tempPtr->EnchantLevel )	// ������ ����
								{
									bShapeActvied = TRUE;
								}
								else if ( 0 < tempPtr->RemainSeconds )	// �Ⱓ�� �����ִ�.
								{
									bShapeActvied = TRUE;
									itemG.FixedTermShape.bActive = TRUE;

									if( tempPtr->RemainSeconds < MANAGE_MAX_TIME_FIXED_TERM_MINUTELY*60 )
									{
										itemG.FixedTermShape.TimerUID = pFieldIOCPSocket->m_TimerManager.StartTimerField(TE_TYPE_FIXED_TERM_SHAPE, tempPtr->RemainSeconds * TIMER_DECREASE_SP_INTERVAL, 0, 0);
									}
								}
								else		// �÷��� �Ⱓ�� ������.
								{
									itemG.ShapeItemNum						= 0;							// ���� �ʱ�ȭ
									itemG.FixedTermShape.nStatLevel			= SHAPE_STAT_INIT_LEVEL;		// ���� �ʱ�ȭ
									itemG.FixedTermShape.nStatShapeItemNum	= 0;							// ���� �ʱ�ȭ

									QPARAM_SHAPE_STATLEVEL	*pQParamSS = new QPARAM_SHAPE_STATLEVEL;
									pQParamSS->ItemKind				= itemG.Kind;
									pQParamSS->ItemUID				= itemG.UniqueNumber;
									pQParamSS->nStatLevel			= itemG.FixedTermShape.nStatLevel;
									pQParamSS->nStatShapeItemNum	= itemG.FixedTermShape.nStatShapeItemNum;
									this->MakeAndEnqueueQuery(QT_InsertShapeStatLevel, q.pFieldIOCPSocket, q.pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber, pQParamSS);

									QPARAM_UPDATE_SHAPEITEMNUM *pQParam = new QPARAM_UPDATE_SHAPEITEMNUM;
									pQParam->ItemUID		= itemG.UniqueNumber;
									pQParam->nShapeItemNum	= itemG.ShapeItemNum;
									pQParam->FixedTermShape.bActive = FALSE;
									pQParam->FixedTermShape.StartDate.Reset();
									pQParam->FixedTermShape.EndDate.Reset();
									pQParam->FixedTermShape.TimerUID = 0;
									this->MakeAndEnqueueQuery(QT_UpdateShapeItemNum, q.pFieldIOCPSocket, q.pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber, pQParam);
								}
								
								if ( TRUE == bShapeActvied )
								{
									q.pFieldIOCPSocket->m_ArmorCollection.ChangeActivedCount(itemG.ShapeItemNum, TRUE);
									itemG.FixedTermShape.nStatLevel			= tempPtr->EnchantLevel;
									itemG.FixedTermShape.nStatShapeItemNum	= tempPtr->ShapeItemNum;
									itemG.FixedTermShape.EndDate			= tempPtr->EndTime;

									if ( 1 == tempPtr->ActivedCount )
									{
										INIT_MSG_WITH_BUFFER(MSG_FC_COLLECTION_INFO, T_FC_COLLECTION_INFO, pCollectionInfo, SendBuf);
										memcpy(pCollectionInfo, tempPtr, sizeof(COLLECTION_INFO));
										pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_COLLECTION_INFO));
									}
								}
							}	// if ( NULL != tempPtr )
						}	// if ( SHAPE_STAT_INIT_LEVEL < itemG.FixedTermShape.nStatLevel )
					}	// if ( FALSE == g_pFieldGlobal->IsArenaServer() )
				}
#endif
				// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - �Ƹ� ���� Ȯ��



				util::del(pQParamFT);

			}
			// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)

			// 2007-01-29 by cmkwon
			itemG.ItemInfo			= pItemInfo;
			itemG.Kind				= pItemInfo->Kind;

			// 2007-01-29 by cmkwon, �ӽ� ���Ϳ� �߰�
			ITEM_GENERAL* pItemG	= new ITEM_GENERAL(itemG);
			vectItemGPtrList.push_back(pItemG);

			// 2012-10-08 by khkim, GLog
			if( MONEY_ITEM_NUMBER == itemG.ItemNum )
			{
				tmSPICurrentCount = (UID64_t)itemG.CurrentCount;
			}
			// 2012-10-08 by khkim, GLog

			util::zero(&itemG, sizeof(ITEM_GENERAL));
			util::zero(&tmCreatedTimeStamp, sizeof(tmCreatedTimeStamp));
		}// END - while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA )

		// 2012-10-08 by khkim, GLog
		// GameStart GLog ��ŶSend �κ�
		if(!COMPARE_RACE(pFieldIOCPSocket->m_character.Race, RACE_OPERATION|RACE_GAMEMASTER|RACE_MONITOR|RACE_GUEST|RACE_DEMO))		// 2013-01-18 by khkim, GLog (�Ϲ������� ���)	
		{
			if( TRUE == pRMsg->bMainGameStartFlag )
			{
				MSG_FL_LOG_CHARDATA LogCharDataParameter;
				util::zero(&LogCharDataParameter, sizeof(LogCharDataParameter));		// 2013-05-30 by jhseol, GLog �ý��� ���� - �ʱ�ȭ
				LogCharDataParameter.LogType			= GLOG_CD_LT_GAMESTART;
				LogCharDataParameter.UserSerial			= (UID64_t)pFieldIOCPSocket->m_character.AccountUniqueNumber;
				LogCharDataParameter.CharSerial			= (UID64_t)pFieldIOCPSocket->m_character.CharacterUniqueNumber;
				LogCharDataParameter.Class				= (SHORT)pFieldIOCPSocket->m_character.UnitKind;
				LogCharDataParameter.Lv					= (SHORT)pFieldIOCPSocket->m_character.Level;
				LogCharDataParameter.Exp				= (UID64_t)pFieldIOCPSocket->m_character.Experience;
				LogCharDataParameter.GameMoney			= (UID64_t)tmSPICurrentCount;
				LogCharDataParameter.Playtime			= (UID64_t)pFieldIOCPSocket->m_character.TotalPlayTime;
				//LogCharDataParameter.GameServerID		= (int)g_pFieldGlobal->GetMGameServerID();
// 2013-01-03 by khkim, GLog (���� Ȯ�ο�)
#ifdef S_KOR_TEST_GLOG_CHARDATA
				if(LogCharDataParameter.UserSerial < 0 || LogCharDataParameter.UserSerial > 7000000 )
				{
					g_pFieldGlobal->WriteSystemLogEX(TRUE,"[DEBUG] GLog #001 [%d,%I64d,%I64d,%d,%d,%I64d,%I64d,%I64d,%d]\r\n ", 
					LogCharDataParameter.LogType,	LogCharDataParameter.UserSerial,	LogCharDataParameter.CharSerial,
					LogCharDataParameter.Class,		LogCharDataParameter.Lv,			LogCharDataParameter.Exp, 
					LogCharDataParameter.GameMoney, LogCharDataParameter.Playtime,		LogCharDataParameter.GameServerID);
				}
#endif 
// end 2013-01-03 by khkim, GLog (���� Ȯ�ο�)			
				CAtumLogSender::SendLogCharData(&LogCharDataParameter);
			}
		}
		// end 2012-10-08 by khkim, GLog
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);		// reset cursor

	// 2013-05-20 by hskim, [���� �ý���] ���������� ������� ������ �߰� ����
#ifdef S_DIRECT_DB_ITEM_INSERT_SECURITY_HSKIM
	if( FALSE == g_pFieldGlobal->IsArenaServer() )
	{
		vectITEM_GENERALPtr::iterator itr(vectItemGPtrList.begin());
		while(itr != vectItemGPtrList.end())
		{
			UID64_t CheckCheckSum = 0;
			ITEM_GENERAL*pItemG = *itr;

			if( m_pFieldIOCP8->GetItemSecurityMaximunUID() >= pItemG->UniqueNumber )
			{
				itr++;

				continue;
			}
		
			QPARAM_GET_STORE_EXTENSION QParamGSE;
			util::zero(&QParamGSE, sizeof(QPARAM_GET_STORE_EXTENSION));
			QParamGSE.ItemUID = pItemG->UniqueNumber;
			this->ExecuteQuery(QT_GetStoreExtension, q.pFieldIOCPSocket, &QParamGSE);

			CheckCheckSum = m_pFieldIOCP8->GeneratorItemSecurityCheckSum(pItemG->UniqueNumber, pItemG->CurrentCount);

			if( CheckCheckSum != QParamGSE.Gesture )
			{
				g_pGlobal->WriteSystemLogEX(TRUE, "[Error] Abnormal items -> %s, %s %d %d\r\n", GetCharacterString(&pFieldIOCPSocket->m_character, string()), pItemG->ItemInfo->ItemName, pItemG->ItemInfo->ItemNum, pItemG->ItemInfo->Kind);
				itr = vectItemGPtrList.erase(itr);

				continue;
			}

			itr++;
		}
	}
#endif
	// end 2013-05-20 by hskim, [���� �ý���] ���������� ������� ������ �߰� ����

	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-29 by cmkwon, 2. �������� ��æƮ �ε�
	int nItemCnt = vectItemGPtrList.size();
	if(0 < nItemCnt)
	{
		for(int i=0; i < nItemCnt; i++)
		{
			ITEM_GENERAL* pItemG = vectItemGPtrList[i];

// 2009-02-17 by cmkwon, ��æƮ �� �� �ִ� ������ ī�ε� ���� - �Ʒ��� ���� ����
// 			// 2007-01-29 by cmkwon, ��æƮ ���� üũ - ũ�� 2����(����, �Ƹ�)
// 			if(FALSE == IS_WEAPON(pItemG->Kind)
// 				&& ITEMKIND_DEFENSE != pItemG->Kind
// 				&& ITEMKIND_RADAR != pItemG->Kind			// 2008-09-23 by dhjin, �ű� ��þƮ	���̴� 
// 				&& ITEMKIND_SUPPORT != pItemG->Kind			// 2008-09-23 by dhjin, �ű� ��þƮ ����
// 				)
// 			{
// 				continue;
// 			}
			if(FALSE == IS_ENCHANT_TARGET_ITEMKIND(pItemG->Kind))	// 2009-02-17 by cmkwon, ��æƮ �� �� �ִ� ������ ī�ε� ���� - 
			{
				continue;
			}


			/*[Stored Query Definition]************************************************
			CREATE PROCEDURE atum_GetEnchantBYItemUID
				@i_ItemUID		BIGINT
			AS
				SELECT TargetItemNum, EnchantItemNum
				FROM td_enchant WITH(NOLOCK)
				WHERE @i_ItemUID = TargetItemUniqueNumber
			GO
			**************************************************************************/
			SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &pItemG->UniqueNumber, 0, NULL);
			ret = SQLExecDirect(hstmt, PROCEDURE_080822_0075, SQL_NTS);
			if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
			{
				ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_GetEnchantBYItemUID Failed!\r\n", TRUE, q.pFieldIOCPSocket, q);
				SQLFreeStmt(hstmt, SQL_CLOSE);
				for (auto x : vectItemGPtrList) delete x;
				vectItemGPtrList.clear();
				vectEnchantList.clear();

				pFieldIOCPSocket->m_bProcessingStoreGetItem = FALSE;		// 2012-12-11 by hskim, ��Ʈ�� ������ ���� ���� ���� (DB ó���� ���������� ���� ��Ŷ�� ����)

				util::del(pRMsg);	
				return;
			}

			if(SQL_NO_DATA == ret)
			{
				SQLFreeStmt(hstmt, SQL_CLOSE);
				continue;
			}
			
			ENCHANT tmpEnchant;
			util::zero(arrCB, sizeof(arrCB[0])*4);
			SQLBindCol(hstmt, 1, SQL_C_ULONG, &tmpEnchant.TargetItemNum, 0,				&arrCB[1]);
			SQLBindCol(hstmt, 2, SQL_C_ULONG, &tmpEnchant.EnchantItemNum, 0,			&arrCB[2]);
			SQLBindCol(hstmt, 3, SQL_C_SBIGINT, &tmpEnchant.SequenceNumber_DB, 0,		&arrCB[3]);	// 2013-01-15 by bckim, ��æƮ������ �������ѹ� �߰�

			util::zero(&tmpEnchant, sizeof(ENCHANT));
			while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA )
			{
				tmpEnchant.TargetItemUniqueNumber	= pItemG->UniqueNumber;
				
				vectEnchantList.push_back(tmpEnchant);

				util::zero(&tmpEnchant, sizeof(ENCHANT));
			}
			SQLFreeStmt(hstmt, SQL_CLOSE);
		}// END - for(int i=0; i < nItemCnt; i++)
	}// END - if(0 < nItemCnt)

	///////////////////////////////////////////////////////////////////////////////
	// 2013-04-18 by jhseol,bckim �̴��� �Ƹ� - ���� ���۽� �̺�Ʈ �Ƹ� üũ
#ifdef S_MONTHL_ARMOR_EVENT_JHSEOL_BCKIM		// ���� ���۽� �̺�Ʈ �Ƹ� üũ
	int nItemCount = vectItemGPtrList.size();
	if(0 < nItemCount)
	{
		if ( NULL != m_pFieldIOCP8->m_pMonthlyArmorManager )
		{
			for(int i=0; i < nItemCount; i++)
			{
				ITEM_GENERAL* pItemG = vectItemGPtrList[i];
				m_pFieldIOCP8->m_pMonthlyArmorManager->CheckMonthlyArmorEventItem(pItemG);	// ���⼱ �ܼ� ������ ������ �ε� ������ �ɼ� ���� �� �̺�Ʈ ������ ������ �˷��� �ʿ䰡 ����
			}
		}
	}
#endif
	// end 2013-04-18 by jhseol,bckim �̴��� �Ƹ� - ���� ���۽� �̺�Ʈ �Ƹ� üũ

	///////////////////////////////////////////////////////////////////////////////////////
	// start 2011-08-17 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ� ���� ����
	//
	// ���� 
	//

// 	///////////////////////////////////////////////////////////////////////////////
// 	// 2010-06-15 by shcho&hslee ��ý��� - ������ ���� ��� �߰� ���� ����.
// 	SQLINTEGER		arrCB1[23] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,
// 								SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};

	/////////
	// ����

	SQLINTEGER	arrCB1[25] = 
	{
		SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,		SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,
		SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,		SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,
		SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
	};
	// end 2011-08-17 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ� ���� ����
	///////////////////////////////////////////////////////////////////////////////////////


	tPET_CURRENTINFO	tmp_PetCurrentData(NULL);
		memset ( &tmp_PetCurrentData , 0x00 , sizeof( tPET_CURRENTINFO ) );

	vecPetCurrentInfo tmp_vecPetCurrentInfo;
	vectITEM_GENERALPtr::iterator ItemG_iter = vectItemGPtrList.begin();
	
	for(; ItemG_iter != vectItemGPtrList.end() ; ++ItemG_iter )
	{
		//������ ī�ε尡 �� �������̸� �ε��� �ʿ�
		//PetEXP,PetLevel,Stamina,PetName,PetRenameFlag,PetItemUID,Itemslot1,Itemslot2,Itemslot3,Itemslot4,Itemslot5,Itemslot6,
		//ItemSocket1,ItemSocket2,ItemSocket3,ItemSocket4,ItemSocket5,ItemSocket6 
		if(ITEMKIND_PET_ITEM == (*ItemG_iter)->ItemInfo->Kind)
		{
			ret = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &(*ItemG_iter)->UniqueNumber, 0, NULL);
			ret = SQLExecDirect(hstmt, PROCEDURE_100615_0573, SQL_NTS);
			if ( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO )
			{
				ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_GetStoreItemPet Failed!\n", TRUE, q.pFieldIOCPSocket, q);
				SQLFreeStmt ( hstmt, SQL_CLOSE );
				for (auto x : vectItemGPtrList) delete x;
				vectItemGPtrList.clear();
				vectEnchantList.clear();

				pFieldIOCPSocket->m_bProcessingStoreGetItem = FALSE;		// 2012-12-11 by hskim, ��Ʈ�� ������ ���� ���� ���� (DB ó���� ���������� ���� ��Ŷ�� ����)

				util::del(pRMsg);
				return;
			}

			memset ( &tmp_PetCurrentData , 0x00 , sizeof( tPET_CURRENTINFO ) );

			///////////////////////////////////////////////////////////////////////////////////////
			// start 2011-08-17 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ� ���� ����
			//
			// ���� 
			//
// 			//PetEXP,ItemUID, PetIndex, PetEXP, PetLevel, EnableLevel, PetName, EnableName, Stamina, CitySourceIndex, FieldSourceIndex, 
// 			//Skillslot1, Skillslot2, Skillslot3, Skillslot4,Skillslot5, Skillslot6,
// 			//ItemSocket1,ItemSocket2,ItemSocket3,ItemSocket4,ItemSocket5,ItemSocket6
// 			SQLBindCol(hstmt, 1, SQL_C_UBIGINT,	&tmp_PetCurrentData.CreatedPetUID,		0,					&arrCB1[1]);
// 			SQLBindCol(hstmt, 2, SQL_C_LONG,	&tmp_PetCurrentData.PetIndex,			0,					&arrCB1[2]);
// 			SQLBindCol(hstmt, 3, SQL_C_DOUBLE,	&tmp_PetCurrentData.PetExp,				0,					&arrCB1[3]);
// 			SQLBindCol(hstmt, 4, SQL_C_LONG,	&tmp_PetCurrentData.PetLevel,			0,					&arrCB1[4]);
// 			SQLBindCol(hstmt, 5, SQL_C_LONG,	&tmp_PetCurrentData.EnableLevelUp,		0,					&arrCB1[5]);
// 			SQLBindCol(hstmt, 6, SQL_C_CHAR,	&tmp_PetCurrentData.szPetName,			SIZE_MAX_PET_NAME,	&arrCB1[6]);
// 			SQLBindCol(hstmt, 7, SQL_C_LONG,	&tmp_PetCurrentData.EnableEditPetName,	0,					&arrCB1[7]);
// 			SQLBindCol(hstmt, 8, SQL_C_DOUBLE,	&tmp_PetCurrentData.Stamina,			0,					&arrCB1[8]);
// 			SQLBindCol(hstmt, 9, SQL_C_LONG,	&tmp_PetCurrentData.SourceIndex_City,	0,					&arrCB1[9]);
// 			SQLBindCol(hstmt, 10, SQL_C_LONG,	&tmp_PetCurrentData.SourceIndex_Field,	0,					&arrCB1[10]);
// 
// 			int iPet = 0;
// 			INT iSkillIndex[SIZE_MAX_PETSKILLITEM] = {0, };
// 			
// 			for( iPet = 0; iPet < SIZE_MAX_PETSKILLITEM ; ++iPet )
// 			{
// 				SQLBindCol(hstmt, 11+iPet, SQL_C_LONG, &iSkillIndex[iPet], 0, &arrCB1[11+iPet]);
// 			}
// 
// 			for( iPet = 0 ; iPet < SIZE_MAX_PETSOCKETITEM ; ++iPet )
// 			{
// 				//SQLBindCol(hstmt, 13+icnt, SQL_C_DOUBLE,	&tmp_PetCurrentData.pItem_PetSocketItem[icnt],	0,		NULL);
// 				SQLBindCol(hstmt, 17+iPet, SQL_C_LONG ,	0,	0,		&arrCB1[17+iPet]);
// 			}
// 
// 			if(ret = SQLFetch(hstmt) != SQL_NO_DATA)
// 			{
// 				tPET_CURRENTINFO *pNewPetCurInfo = new tPET_CURRENTINFO( &tmp_PetCurrentData );
// 				
// 				for(iPet = 0; iPet < SIZE_MAX_PETSKILLITEM ; ++iPet )
// 				{
// 					if ( iSkillIndex[iPet] != 0 )
// 					{
// 						const ITEM *pItemInfo = m_pFieldIOCP8->GetItemInfo( iSkillIndex[iPet] );
// 						if ( NULL == pItemInfo )
// 							continue;
// 						
// 						pNewPetCurInfo->pItem_PetSkill[iPet] = pItemInfo;
// 					}
// 				}
// 				
// 				tmp_vecPetCurrentInfo.push_back(pNewPetCurInfo);
// 			}

			/////////
			// ����

			int iPet = 0;
			int iCBCount = 0;
			
			SQLBindCol(hstmt, (iCBCount + 1), SQL_C_UBIGINT,		&tmp_PetCurrentData.CreatedPetUID,			0,					&arrCB1[iCBCount]); iCBCount++;
			SQLBindCol(hstmt, (iCBCount + 1), SQL_C_LONG,			&tmp_PetCurrentData.PetIndex,				0,					&arrCB1[iCBCount]); iCBCount++;
			SQLBindCol(hstmt, (iCBCount + 1), SQL_C_CHAR,			&tmp_PetCurrentData.PetName,				SIZE_MAX_PET_NAME,	&arrCB1[iCBCount]); iCBCount++;
			SQLBindCol(hstmt, (iCBCount + 1), SQL_C_LONG,			&tmp_PetCurrentData.PetLevel,				0,					&arrCB1[iCBCount]); iCBCount++;
			SQLBindCol(hstmt, (iCBCount + 1), SQL_C_DOUBLE,			&tmp_PetCurrentData.PetExp,					0,					&arrCB1[iCBCount]); iCBCount++;

			SQLBindCol(hstmt, (iCBCount + 1), SQL_C_UTINYINT,		&tmp_PetCurrentData.PetExpRatio,			0,					&arrCB1[iCBCount]); iCBCount++;
			SQLBindCol(hstmt, (iCBCount + 1), SQL_C_UTINYINT,		&tmp_PetCurrentData.PetEnableSocketCount,	0,					&arrCB1[iCBCount]); iCBCount++;
			
			for(iPet = 0; iPet < SIZE_MAX_PETSOCKET; iPet++)
			{
				SQLBindCol(hstmt, (iCBCount + 1), SQL_C_UBIGINT,	&tmp_PetCurrentData.PetSocketItemUID[iPet],	0,					&arrCB1[iCBCount]); iCBCount++;
			}

			SQLBindCol(hstmt, (iCBCount + 1), SQL_C_LONG,	&tmp_PetCurrentData.PetKitHP.ItemNum,			0,					&arrCB1[iCBCount]); iCBCount++;
			SQLBindCol(hstmt, (iCBCount + 1), SQL_C_FLOAT,	&tmp_PetCurrentData.PetKitHP.TriggerValue,		0,					&arrCB1[iCBCount]); iCBCount++;
			SQLBindCol(hstmt, (iCBCount + 1), SQL_C_LONG,	&tmp_PetCurrentData.PetKitShield.ItemNum,		0,					&arrCB1[iCBCount]); iCBCount++;
			SQLBindCol(hstmt, (iCBCount + 1), SQL_C_FLOAT,	&tmp_PetCurrentData.PetKitShield.TriggerValue,	0,					&arrCB1[iCBCount]); iCBCount++;
			SQLBindCol(hstmt, (iCBCount + 1), SQL_C_LONG,	&tmp_PetCurrentData.PetKitSP.ItemNum,			0,					&arrCB1[iCBCount]); iCBCount++;
			SQLBindCol(hstmt, (iCBCount + 1), SQL_C_FLOAT,	&tmp_PetCurrentData.PetKitSP.TriggerValue,		0,					&arrCB1[iCBCount]); iCBCount++;

			for(iPet = 0; iPet < SIZE_MAX_PETSOCKET_AUTOSKILL_SLOT; iPet++)
			{
				SQLBindCol(hstmt, (iCBCount + 1), SQL_C_LONG,	&tmp_PetCurrentData.PetAutoSkill.ItemNum[iPet],		0,			&arrCB1[iCBCount]); iCBCount++;
			}

			if(ret = SQLFetch(hstmt) != SQL_NO_DATA)
			{
				tPET_CURRENTINFO *pNewPetCurInfo = new tPET_CURRENTINFO( &tmp_PetCurrentData );
				
				tmp_vecPetCurrentInfo.push_back(pNewPetCurInfo);
			}
			// end 2011-08-17 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ� ���� ����
			///////////////////////////////////////////////////////////////////////////////////////	

			SQLFreeStmt(hstmt, SQL_CLOSE);
		}
	}
	// END 2010-06-15 by shcho&hslee ��ý��� - ������ ���� ��� �߰� ���� ����.
	///////////////////////////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-29 by cmkwon, ĳ���� ���� ���۽� �ε��� �ƴϸ� ���⼭ ����
	if(ITEM_IN_CHARACTER != tmpItemStorage)
	{
		///////////////////////////////////////////////////////////////////////////////		
		// 2007-01-29 by cmkwon, �ε��� �����۰� ��æƮ Ŭ���̾�Ʈ�� ����
		if(pFieldIOCPSocket->IsValidCharacter(FALSE))
		{
// 2007-12-17 by dhjin, ���� ���� ��ƾ DB�� ������ Client���� �κ� �и�			
//			pFieldIOCPSocket->InsertItemListFromDB(pRMsg, &vectItemGPtrList, &vectEnchantList);

			// 2008-01-10 by cmkwon, ���� â�� ���� ���� - �Ʒ��� ���� ������ CharacterUID �� ���ڷ� �־� �ش�
			//pFieldIOCPSocket->InsertStoreItemListFromDB(&vectItemGPtrList, &vectEnchantList);
			pFieldIOCPSocket->InsertStoreItemListFromDB(&vectItemGPtrList, &vectEnchantList, pRMsg->PossessCharacter, &tmp_vecPetCurrentInfo);		// 2012-01-14 by hskim, ��Ʈ�� �ý��� 2�� - â���� ���� �� Ŭ�� ƨ��� ���� ����

// 2007-12-26 by cmkwon, ��ġ �ű�
//			pFieldIOCPSocket->SendStoreItemList();
		}
		
		// start 2012-01-14 by hskim, ��Ʈ�� �ý��� 2�� - â���� ���� �� Ŭ�� ƨ��� ���� ����
		for (auto x : tmp_vecPetCurrentInfo) delete x;
		tmp_vecPetCurrentInfo.clear();
		// end 2012-01-14 by hskim, ��Ʈ�� �ý��� 2�� - â���� ���� �� Ŭ�� ƨ��� ���� ����

		for (auto x : vectItemGPtrList) delete x;
		vectItemGPtrList.clear();
		vectEnchantList.clear();

		pFieldIOCPSocket->m_bProcessingStoreGetItem = FALSE;		// 2012-12-11 by hskim, ��Ʈ�� ������ ���� ���� ���� (DB ó���� ���������� ���� ��Ŷ�� ����)

		util::del(pRMsg);
		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-29 by cmkwon, ���� ������ ĳ���Ͱ� ��ȿ���� üũ
	if(pFieldIOCPSocket->m_character.CharacterUniqueNumber != pRMsg->PossessCharacter)
	{
		// start 2012-01-14 by hskim, ��Ʈ�� �ý��� 2�� - â���� ���� �� Ŭ�� ƨ��� ���� ����
		for (auto x : tmp_vecPetCurrentInfo) delete x;
		tmp_vecPetCurrentInfo.clear();
		// end 2012-01-14 by hskim, ��Ʈ�� �ý��� 2�� - â���� ���� �� Ŭ�� ƨ��� ���� ����

		for (auto x : vectItemGPtrList) delete x;
		vectItemGPtrList.clear();
		vectEnchantList.clear();

		pFieldIOCPSocket->m_bProcessingStoreGetItem = FALSE;		// 2012-12-11 by hskim, ��Ʈ�� ������ ���� ���� ���� (DB ó���� ���������� ���� ��Ŷ�� ����)

		util::del(pRMsg);
		return;
	}
	
	vectItemSkillPtr	vectItemSkillPtrList;	
	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-29 by cmkwon, ��ų �ε�
	/*[Stored Query Definition]************************************************
	CREATE 	PROCEDURE atum_GetStoreItemSkill
		@accNum	INT,
		@possessNum INT
	AS
		SELECT UniqueNumber, AccountUniqueNumber, Possess, ItemWindowIndex, ItemNum, CreatedTime, UseTime
		FROM td_StoreSkill
		WHERE AccountUniqueNumber = @accNum AND Possess = @possessNum
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,	&pRMsg->AccountUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,	&pRMsg->PossessCharacter, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0096, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_GetStoreItemSkill Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);

		// start 2012-01-14 by hskim, ��Ʈ�� �ý��� 2�� - â���� ���� �� Ŭ�� ƨ��� ���� ����
		for (auto x : tmp_vecPetCurrentInfo) delete x;
		tmp_vecPetCurrentInfo.clear();
		// end 2012-01-14 by hskim, ��Ʈ�� �ý��� 2�� - â���� ���� �� Ŭ�� ƨ��� ���� ����

		for (auto x : vectItemGPtrList) delete x;
		vectItemGPtrList.clear();
		vectEnchantList.clear();

		pFieldIOCPSocket->m_bProcessingStoreGetItem = FALSE;		// 2012-12-11 by hskim, ��Ʈ�� ������ ���� ���� ���� (DB ó���� ���������� ���� ��Ŷ�� ����)

		util::del(pRMsg);
		return;
	}

	if(SQL_NO_DATA != ret)
	{
		ITEM_SKILL				itemSkill(NULL);
		SQL_TIMESTAMP_STRUCT	tmpUseSkillTimeStamp;

		util::zero(arrCB, sizeof(arrCB[0])*8);
		SQLBindCol(hstmt, 1, SQL_C_SBIGINT, &itemSkill.UniqueNumber, 0,			&arrCB[1]);
		SQLBindCol(hstmt, 2, SQL_C_ULONG, &itemSkill.AccountUniqueNumber, 0,	&arrCB[2]);
		SQLBindCol(hstmt, 3, SQL_C_ULONG, &itemSkill.Possess, 0,				&arrCB[3]);
		SQLBindCol(hstmt, 4, SQL_C_SLONG, &itemSkill.ItemWindowIndex, 0,		&arrCB[4]);
		SQLBindCol(hstmt, 5, SQL_C_SLONG, &itemSkill.ItemNum, 0,				&arrCB[5]);
		SQLBindCol(hstmt, 6, SQL_C_TIMESTAMP, &tmCreatedTimeStamp, 0,			&arrCB[6]);
		SQLBindCol(hstmt, 7, SQL_C_TIMESTAMP, &tmpUseSkillTimeStamp, 0,			&arrCB[7]);

		util::zero(&itemSkill, sizeof(itemSkill));
		util::zero(&tmCreatedTimeStamp, sizeof(tmCreatedTimeStamp));
		util::zero(&tmpUseSkillTimeStamp, sizeof(tmpUseSkillTimeStamp));

		while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA )
		{
			// ������ ���� ã��
			ITEM *pItemInfo = m_pFieldIOCP8->GetItemInfo(itemSkill.ItemNum);

			if (pItemInfo == NULL)
			{
				// 2009-11-20 by cmkwon, ���� ������ �ε��� ��ȿ���� ���� ������ ó�� ���� - �α� ����
				g_pGlobal->WriteSystemLogEX(TRUE, "  [Error]Invaild Item in DB in CAtumFieldDBManager::atum_GetStoreItemSkill() -> %10s, %d \r\n", pFieldIOCPSocket->m_character.CharacterName, itemSkill.ItemNum);

				util::zero(&itemSkill, sizeof(itemSkill));
				util::zero(&tmCreatedTimeStamp, sizeof(tmCreatedTimeStamp));
				util::zero(&tmpUseSkillTimeStamp, sizeof(tmpUseSkillTimeStamp));
				continue;
			}

			
			if (FALSE == IS_SKILL_ITEM(pItemInfo->Kind))
			{
				char szSystemLog[1024];
				sprintf(szSystemLog, "Fatal [Error]: Invaild Skill Item Kind in DB in CAtumFieldDBManager::atum_GetStoreItemSkill() -> %10s, %d \r\n",
					pFieldIOCPSocket->m_character.CharacterName, itemSkill.ItemNum);
				g_pGlobal->WriteSystemLog(szSystemLog);
				DbgOut(szSystemLog);

				util::zero(&itemSkill, sizeof(itemSkill));
				util::zero(&tmCreatedTimeStamp, sizeof(tmCreatedTimeStamp));
				util::zero(&tmpUseSkillTimeStamp, sizeof(tmpUseSkillTimeStamp));
				continue;				
			}

			itemSkill.ItemInfo	= pItemInfo;
			itemSkill.Kind		= pItemInfo->Kind;
			itemSkill.UseTime	= tmpUseSkillTimeStamp;			// 2006-11-17 by dhjin, 2�� ��ų�� ���� ��ų ��� �ð� �߰�

			///////////////////////////////////////////////////////////////////////////////
			// 2007-01-29 by cmkwon, �ӽ� ���� ����Ʈ�� �߰�
			ITEM_SKILL* pItemSkill = new ITEM_SKILL(itemSkill);
			vectItemSkillPtrList.push_back(pItemSkill);

			util::zero(&itemSkill, sizeof(itemSkill));
			util::zero(&tmCreatedTimeStamp, sizeof(tmCreatedTimeStamp));
			util::zero(&tmpUseSkillTimeStamp, sizeof(tmpUseSkillTimeStamp));
		}// END - while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA )
	}
	
	SQLFreeStmt(hstmt, SQL_CLOSE);
	
	if(pFieldIOCPSocket->IsValidCharacter(FALSE))
	{
// 2007-12-17 by dhjin, ���� ���� ��ƾ DB�� ������ Client���� �κ� �и�
//		pFieldIOCPSocket->InsertItemListFromDB(pRMsg, &vectItemGPtrList, &vectEnchantList, &vectItemSkillPtrList);
		//////////////////////////////////////////////////////////////////////////
		// 2008-01-09 by dhjin, �Ʒ��� ���� -
		if(FALSE == g_pFieldGlobal->IsArenaServer())
		{// 2007-12-27 by dhjin, �Ʒ��� ������ �ƴϸ� ����Ʈ �ε�
			this->GetAllQuest(pFieldIOCPSocket, hstmt);
		}

		// 2010-06-15 by shcho&hslee ��ý��� - ������ ���� ��� �߰� ���� ���� ���� �߰�
		pFieldIOCPSocket->InsertItemListByCharacterFirstGameStartFromDB(&vectItemGPtrList, &vectEnchantList, &vectItemSkillPtrList, &tmp_vecPetCurrentInfo);

#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, �������� �����߰�
		if ( NULL != pFieldIOCPSocket->m_pFieldParty )			// ��밡 ������ 
		{
			pFieldIOCPSocket->m_pFieldParty->SetReturnUserBuffSkill(pFieldIOCPSocket); // �ʱ� ���������� ������ ���� 
		}
#endif

#if S_BONUSEXPSYSTEM_RENEWAL
		// 2012-10-19 by jhjang ���� �ƿ� �ý��� ������ - ��ī�� ��� �������� ����ä�� �����ӽ� ųī��Ʈ�� ���̴� ���� ����
		if (NULL != pFieldIOCPSocket->m_ItemManager.FindGeneralItemByItemNum(KILLCOUNT_ITEMNUM))
		{
			pFieldIOCPSocket->SetKillCountEventItem(true);
		}
		else
		{
			pFieldIOCPSocket->SetKillCountEventItem(false);
		}
		// end 2012-10-19 by jhjang ���� �ƿ� �ý��� ������ - ��ī�� ��� �������� ����ä�� �����ӽ� ųī��Ʈ�� ���̴� ���� ����
#endif // S_BONUSEXPSYSTEM_RENEWAL


	}
	
	for (auto x : vectItemGPtrList) delete x;
	vectItemGPtrList.clear();
	vectEnchantList.clear();
	for (auto x : vectItemSkillPtrList) delete x;
	vectItemSkillPtrList.clear();

	// 2010-06-15 by shcho&hslee ��ý��� - �ӽ� ������ �޸� ����.
	for (auto x : tmp_vecPetCurrentInfo) delete x;
	tmp_vecPetCurrentInfo.clear();
	
	pFieldIOCPSocket->m_bProcessingStoreGetItem = FALSE;		// 2012-12-11 by hskim, ��Ʈ�� ������ ���� ���� ���� (DB ó���� ���������� ���� ��Ŷ�� ����)

	util::del(pRMsg);
}

// QT_ReloadAllEnchant
void CAtumFieldDBManager::QP_ReloadAllEnchant(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	///////////////////////////////////////////////////////////////////////////
	// Load All Enchant - ������ �ش�
	///////////////////////////////////////////////////////////////////////////
	SQLRETURN	ret;
	ENCHANT		tmpEnchant;
	SQLINTEGER	cb1, cb2, cb3;

	CFieldIOCPSocket *pFieldIOCPSocket = q.pFieldIOCPSocket;
	if(FALSE == pFieldIOCPSocket->IsValidCharacter(FALSE))
	{// 2006-12-18 by cmkwon
		return;
	}

	// reset���� �Ͻð�
	pFieldIOCPSocket->m_ParamFactor.Reset();
	pFieldIOCPSocket->m_mapEnchant.clearLock();

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_GetAllEnchant
		@i_AccountUniqueNumber		INT,
		@i_CharacterUniqueNumber	INT
	AS
		SELECT e.TargetItemUniqueNumber, e.TargetItemNum, e.EnchantItemNum
		FROM td_enchant e, td_store i 
		WHERE i.AccountUniqueNumber = @i_AccountUniqueNumber AND i.possess = @i_CharacterUniqueNumber
				AND e.TargetItemUniqueNumber = i.UniqueNumber
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &q.nGeneralParam2, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0097, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO ) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_GetAllEnchant Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	SQLBindCol(hstmt, 1, SQL_C_SBIGINT, &tmpEnchant.TargetItemUniqueNumber, 0, &cb1);
	SQLBindCol(hstmt, 2, SQL_C_ULONG, &tmpEnchant.TargetItemNum, 0, &cb2);
	SQLBindCol(hstmt, 3, SQL_C_ULONG, &tmpEnchant.EnchantItemNum, 0, &cb3);

	// lock m_mapItemGeneral, @QP_ReloadAllEnchant
	mt_auto_lock igLock(&pFieldIOCPSocket->m_ItemManager.m_mapItemGeneral);

	mt_auto_lock mtE(&pFieldIOCPSocket->m_mapEnchant);		// 2006-12-18 by cmkwon, AutoLock�� ��´�
	pFieldIOCPSocket->m_mapEnchant.clearLock();				// 2006-12-18 by cmkwon, ��æƮ ������ �ʱ�ȭ �Ѵ�.

	// initialize to 0x00
	memset(&tmpEnchant, 0x00, sizeof(ENCHANT));
	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA )
	{
		ITEM_GENERAL *pItemTarget
			= pFieldIOCPSocket->m_ItemManager.GetItemGeneralByUID(tmpEnchant.TargetItemUniqueNumber, ITEM_IN_CHARACTER);

		if (pItemTarget != NULL)
		{
			pFieldIOCPSocket->InsertEnchantToItem(pItemTarget, tmpEnchant);
		}

		// initialize to 0x00
		memset(&tmpEnchant, 0x00, sizeof(ENCHANT));
	}	// end of while(fetch)
	SQLFreeStmt(hstmt, SQL_CLOSE);	// clean up
	mtE.auto_unlock_cancel();								// 2006-12-18 by cmkwon, AutoLock ����

	if (pFieldIOCPSocket->IsValidCharacter(FALSE))
	{// 2006-12-18 by cmkwon
#ifdef _DEBUG
//		pFieldIOCPSocket->PrintEnchant();
#endif

		pFieldIOCPSocket->ResetAllSkillAndEnchant();	// called CAtumFieldDBManager::QP_ReloadAllEnchant#
	}

}

// QT_DeleteStoreItem
void CAtumFieldDBManager::QP_DeleteStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_DELETESTOREITEM *pRMsg = (QPARAM_DELETESTOREITEM*)q.pQueryParam;

	// 2010-06-15 by shcho&hslee ��ý���
	CFieldIOCPSocket *pFieldIOCPSocket = q.pFieldIOCPSocket;

	RETCODE ret;
	SDWORD pcbNTS = SQL_NTS;

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_DeleteStoreItem
		@accNum	INT,
		@uniqueNum BIGINT
	AS
	DELETE FROM td_store
	WHERE AccountUniqueNumber = @accNum AND UniqueNumber = @uniqueNum;
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,
					&pRMsg->AccountUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0,
					&pRMsg->ItemUniqueNumber, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0098, SQL_NTS);

	if (ret != SQL_SUCCESS 
		&& ret != SQL_SUCCESS_WITH_INFO
		&& ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_DeleteStoreItem Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

// QT_InsertStoreItem -> CAtumFieldDBManager::QP_InsertStoreItem
void CAtumFieldDBManager::QP_InsertStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	ITEM_GENERAL		*pItem = (ITEM_GENERAL*)q.pQueryParam;
	CFieldIOCPSocket	*pFieldIOCPSocket = q.pFieldIOCPSocket;
	BYTE				ItemInsertionType = (BYTE)q.nGeneralParam1;

	RETCODE ret;
	SDWORD pcbNTS = SQL_NTS;

	/*[Stored Query Definition]************************************************
	-- !!!!
	-- Name: dbo.atum_InsertStoreItem
	-- Desc:
	--		2005-12-07 by cmkwon, ������ġ �ʵ� �߰�
	--		-- // 2009-08-25 by cmkwon, ������� �ʴ� �ʵ� ����(td_Store.ScarcityNumber) - 
	--		-- // 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - td_Store.ShapeItemNum �ʵ� �߰�, �ʵ帮��Ʈ ���
	-- ====
	**************************************************************************/

	///////////////////////////////////////////////////////////////////////////////
	
	// 2010-06-15 by shcho&hslee ��ý��� - �� �������� ����� �߰� ���ν��� ȣ�� ����.
	switch ( pItem->ItemInfo->Kind )
	{
		case ITEMKIND_PET_ITEM :
			{
				///////////////////////////////////////////////////////////////////////////////////////
				// start 2011-08-17 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ� ���� ����
				int iCBCount = 0;
			
				SQLBindParameter(hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_ULONG,		SQL_INTEGER,	0, 0, &pItem->AccountUniqueNumber,	0, NULL); iCBCount++;
				SQLBindParameter(hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_ULONG,		SQL_INTEGER,	0, 0, &pItem->Possess,				0, NULL); iCBCount++;
				SQLBindParameter(hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_UTINYINT,	SQL_TINYINT,	0, 0, &pItem->ItemStorage,			0, NULL); iCBCount++;
				SQLBindParameter(hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_UTINYINT,	SQL_TINYINT,	0, 0, &pItem->Wear,					0, NULL); iCBCount++;
				SQLBindParameter(hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_SLONG,		SQL_INTEGER,	0, 0, &pItem->CurrentCount,			0, NULL); iCBCount++;
				SQLBindParameter(hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_SLONG,		SQL_INTEGER,	0, 0, &pItem->ItemWindowIndex,		0, NULL); iCBCount++;
				SQLBindParameter(hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_SLONG,		SQL_INTEGER,	0, 0, &pItem->ItemNum,				0, NULL); iCBCount++;
				SQLBindParameter(hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_SSHORT,		SQL_SMALLINT,	0, 0, &pItem->NumOfEnchants,		0, NULL); iCBCount++;
				SQLBindParameter(hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_SLONG,		SQL_INTEGER,	0, 0, &pItem->PrefixCodeNum,		0, NULL); iCBCount++;
				SQLBindParameter(hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_SLONG,		SQL_INTEGER,	0, 0, &pItem->SuffixCodeNum,		0, NULL); iCBCount++;
				SQLBindParameter(hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_FLOAT,		SQL_FLOAT,		0, 0, &pItem->CurrentEndurance,		0, NULL); iCBCount++;
				SQLBindParameter(hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_ULONG,		SQL_INTEGER,	0, 0, &pItem->ColorCode,			0, NULL); iCBCount++;
				SQLBindParameter(hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_ULONG,		SQL_INTEGER,	0, 0, &pItem->UsingTimeStamp,		0, NULL); iCBCount++;
				SQLBindParameter(hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_LONG,		SQL_INTEGER,	0, 0, &pItem->ItemInfo->LinkItem,	0, NULL); iCBCount++;
				// end 2011-08-17 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ� ���� ����
				///////////////////////////////////////////////////////////////////////////////////////

				ret = SQLExecDirect(hstmt,PROCEDURE_100615_0571, SQL_NTS);
				
				if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
				{
					ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_InsertStorePetItem Failed 3 !\r\n", TRUE, q.pFieldIOCPSocket, q);
					SQLFreeStmt(hstmt, SQL_CLOSE);
					util::del(pItem);
					
					if(pFieldIOCPSocket
						&& pFieldIOCPSocket->IsValidCharacter(FALSE))
					{
						pFieldIOCPSocket->DecrementInsertingStoreCounts();
						pFieldIOCPSocket->SendErrorMessage(T_FC_STORE_MOVE_ITEM, ERR_DB_EXECUTION_FAILED, ItemInsertionType);
					}
					return;
				}
				SQLBindCol(hstmt, 1, SQL_C_UBIGINT, &(pItem->UniqueNumber), 0, NULL);
			}
			break;
			
		default :

			// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) - 
			SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItem->AccountUniqueNumber, 0, NULL);
			SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItem->Possess, 0, NULL);
			SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pItem->ItemStorage, 0, NULL);		// 2005-12-07 by cmkwon
			SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pItem->Wear, 0, NULL);
			SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->CurrentCount, 0, NULL);
			// 2009-08-25 by cmkwon, ������� �ʴ� �ʵ� ����(td_Store.ScarcityNumber) - �Ʒ��� �ε��� ����
			//	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pItem->ScarcityNumber, 0, NULL);
			SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->ItemWindowIndex, 0, NULL);
			SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->ItemNum, 0, NULL);
			SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &pItem->NumOfEnchants, 0, NULL);
			SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->PrefixCodeNum, 0, NULL);
			SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->SuffixCodeNum, 0, NULL);
			SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pItem->CurrentEndurance, 0, NULL);
			SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItem->ColorCode, 0, NULL);
			SQLBindParameter(hstmt, 13, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItem->UsingTimeStamp, 0, NULL);
			SQLBindParameter(hstmt, 14, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pItem->ShapeItemNum, 0, NULL);		// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - ShapeItemNum �ʵ� �߰�

			ret = SQLExecDirect(hstmt,PROCEDURE_080822_0077, SQL_NTS);
			
			if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
			{
				ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_InsertStoreItem Failed 3 !\r\n", TRUE, q.pFieldIOCPSocket, q);
				SQLFreeStmt(hstmt, SQL_CLOSE);
				util::del(pItem);
				
				if(pFieldIOCPSocket
					&& pFieldIOCPSocket->IsValidCharacter(FALSE))
				{
					pFieldIOCPSocket->DecrementInsertingStoreCounts();
					pFieldIOCPSocket->SendErrorMessage(T_FC_STORE_MOVE_ITEM, ERR_DB_EXECUTION_FAILED, ItemInsertionType);
				}
				return;
			}
			SQLBindCol(hstmt, 1, SQL_C_UBIGINT, &(pItem->UniqueNumber), 0, NULL);

			break;
	}
	// End 2010-06-15 by shcho&hslee ��ý��� - �� �������� ����� �߰� ���ν��� ȣ�� ����.

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(hstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 �̸� ����� RowCount �̴� 
			ret = SQLMoreResults(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(hstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(hstmt);                 // 2008-07-25 by cmkwon, ���� Results �� �ִ����� �ѹ��� üũ�Ѵ�.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 2010-06-15 by shcho&hslee ��ý��� - ������ ���� ��� �߰� ���� ����.
	tPET_CURRENTINFO *pPetCurData = NULL;

	if ( pItem->ItemInfo->Kind == ITEMKIND_PET_ITEM )
	{

		pPetCurData = new tPET_CURRENTINFO(NULL);
		if ( NULL == pPetCurData )
		{
			ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"Assign PetCurrentData Failed!\n", TRUE, pFieldIOCPSocket, q);
			SQLFreeStmt(hstmt, SQL_CLOSE);
			util::del(pItem);
			return;
		}
		else
		{
			memset ( pPetCurData , 0x00 , sizeof( tPET_CURRENTINFO ) );
		}

		SQLBindParameter ( hstmt , 1 , SQL_PARAM_INPUT , SQL_C_UBIGINT , SQL_BIGINT , 0 , 0 , &pItem->UniqueNumber , 0 , NULL );

		ret = SQLExecDirect(hstmt, PROCEDURE_100615_0573, SQL_NTS);
		
		if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
		{
			ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Load_td_petInfo Failed!\n", TRUE, pFieldIOCPSocket, q);
			SQLFreeStmt(hstmt, SQL_CLOSE);
			util::del(pItem);
			return;
		}

		///////////////////////////////////////////////////////////////////////////////////////
		// start 2011-08-17 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ� ���� ����
		SQLINTEGER	arrCB[25] = 
		{
			SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,		SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,
			SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,		SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,
			SQL_NTS,SQL_NTS,SQL_NTS
		};

		int iPet = 0;
		int iCBCount = 0;

		SQLBindCol(hstmt, (iCBCount + 1), SQL_C_UBIGINT,	&pPetCurData->CreatedPetUID,					0,					&arrCB[iCBCount]); iCBCount++;
		SQLBindCol(hstmt, (iCBCount + 1), SQL_C_LONG,		&pPetCurData->PetIndex,							0,					&arrCB[iCBCount]); iCBCount++;
		SQLBindCol(hstmt, (iCBCount + 1), SQL_C_CHAR,		&pPetCurData->PetName,							SIZE_MAX_PET_NAME,	&arrCB[iCBCount]); iCBCount++;
		SQLBindCol(hstmt, (iCBCount + 1), SQL_C_LONG,		&pPetCurData->PetLevel,							0,					&arrCB[iCBCount]); iCBCount++;
		SQLBindCol(hstmt, (iCBCount + 1), SQL_C_DOUBLE,		&pPetCurData->PetExp,							0,					&arrCB[iCBCount]); iCBCount++;

		SQLBindCol(hstmt, (iCBCount + 1), SQL_C_UTINYINT,	&pPetCurData->PetExpRatio,						0,					&arrCB[iCBCount]); iCBCount++;
		SQLBindCol(hstmt, (iCBCount + 1), SQL_C_UTINYINT,	&pPetCurData->PetEnableSocketCount,				0,					&arrCB[iCBCount]); iCBCount++;

		for(iPet = 0; iPet < SIZE_MAX_PETSOCKET; iPet++)
		{
			SQLBindCol(hstmt, (iCBCount + 1), SQL_C_UBIGINT,	&pPetCurData->PetSocketItemUID[iPet],		0,					&arrCB[iCBCount]); iCBCount++;
		}

		SQLBindCol(hstmt, (iCBCount + 1), SQL_C_LONG,		&pPetCurData->PetKitHP.ItemNum,					0,					&arrCB[iCBCount]); iCBCount++;
		SQLBindCol(hstmt, (iCBCount + 1), SQL_C_FLOAT,		&pPetCurData->PetKitHP.TriggerValue,			0,					&arrCB[iCBCount]); iCBCount++;
		SQLBindCol(hstmt, (iCBCount + 1), SQL_C_LONG,		&pPetCurData->PetKitShield.ItemNum,				0,					&arrCB[iCBCount]); iCBCount++;
		SQLBindCol(hstmt, (iCBCount + 1), SQL_C_FLOAT,		&pPetCurData->PetKitShield.TriggerValue,		0,					&arrCB[iCBCount]); iCBCount++;
		SQLBindCol(hstmt, (iCBCount + 1), SQL_C_LONG,		&pPetCurData->PetKitSP.ItemNum,					0,					&arrCB[iCBCount]); iCBCount++;
		SQLBindCol(hstmt, (iCBCount + 1), SQL_C_FLOAT,		&pPetCurData->PetKitSP.TriggerValue,			0,					&arrCB[iCBCount]); iCBCount++;

		for(iPet = 0; iPet < SIZE_MAX_PETSOCKET_AUTOSKILL_SLOT; iPet++)
		{
			SQLBindCol(hstmt, (iCBCount + 1), SQL_C_LONG,	&pPetCurData->PetAutoSkill.ItemNum[iPet],		0,					&arrCB[iCBCount]); iCBCount++;
		}

		ret = SQLFetch(hstmt);

		if ( ( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) || ret == SQL_NO_DATA )
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] atum_Load_td_petInfo@ Failed 1 !! %s %s, Invalid PetBaseData\r\n"
				, GetCharacterString( pFieldIOCPSocket->GetCharacter(), string() ), GetItemGeneralString( pItem, string() ) );

			SQLFreeStmt(hstmt, SQL_CLOSE);

			util::del ( pPetCurData );
			util::del(pItem);
			return;
		}
		// end 2011-08-17 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ� ���� ����
		///////////////////////////////////////////////////////////////////////////////////////

	}
	// End 2010-06-15 by shcho&hslee ��ý��� - ������ ���� ��� �߰� ���� ����.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	SQLFreeStmt(hstmt, SQL_CLOSE);	

	if(FALSE == IS_VALID_UID64(pItem->UniqueNumber))
	{// 2008-07-24 by cmkwon, �߰��� �������� ItemUID üũ
		if(pFieldIOCPSocket->IsValidCharacter(FALSE))
		{
			pFieldIOCPSocket->SendErrorMessage(T_FC_STORE_MOVE_ITEM, ERR_DB_EXECUTION_FAILED, ItemInsertionType);			
		}
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] atum_InsertStoreItem@ error 3 !! %s %s, ItemInsertionType(%d)\r\n"
			, GetCharacterString(pFieldIOCPSocket->GetCharacter(), string()), GetItemGeneralString(pItem, string()), ItemInsertionType);
		util::del(pItem);

		// 2010-06-15 by shcho&hslee ��ý���
		util::del ( pPetCurData );

		return;
	}

	// 2013-05-20 by hskim, [���� �ý���] ���������� ������� ������ �߰� ����
#ifdef S_DIRECT_DB_ITEM_INSERT_SECURITY_HSKIM
	if( FALSE == g_pFieldGlobal->IsArenaServer() )	
	{
		QPARAM_INSERT_STORE_EXTENSION *pQParamISE = new QPARAM_INSERT_STORE_EXTENSION;
		//util::zero(pQParamISE, sizeof(QPARAM_INSERT_STORE_EXTENSION));
		pQParamISE->ItemUID = pItem->UniqueNumber;
		pQParamISE->Gesture = m_pFieldIOCP8->GeneratorItemSecurityCheckSum(pItem->UniqueNumber, pItem->CurrentCount);
		this->MakeAndEnqueueQuery(QT_InsertStoreExtension, q.pFieldIOCPSocket, q.pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber, pQParamISE);
	}
#endif
	// end 2013-05-20 by hskim, [���� �ý���] ���������� ������� ������ �߰� ����

	///////////////////////////////////////////////////////////////////////////////	
	// 2008-01-10 by cmkwon, �Ʒ����� ���� �α׸� ����� ���ؼ��� �α� �Լ� ���� pFieldIOCPSocket üũ�ϴ� ��ƾ�� �߰��ؾ���
	switch(ItemInsertionType)
	{
	case IUT_DROP_ITEM:
	case IUT_QUEST:			// 2007-10-30 by cmkwon, ����Ʈ ������ ���� ���� �α׿� �߰�
	case IUT_MIXING:		// 2008-09-26 by cmkwon, ���ս� GameLog ���� - ���սÿ��� ������ �߰��� �α׸� GameLog�� �����.
	case IUT_DISSOLUTION_ITEM : // 2010-08-31 by shcho&jskim �����ۿ��� �ý��� -
	case IUT_RANDOMBOX :	// 2013-06-10 by jhseol, GameLog �߰� - ĸ���� ��� �� ȹ���ϴ� �����۵� ���ӷα׸� ���⵵�� �߰�
		// 2008-09-26 by cmkwon, ���ս� GameLog ���� - 
		// 2012-08-30 by jhseol, Ǯ�α� �����
#ifdef S_FULL_LOG_JHSEOL
		//CAtumLogSender::SendLogMessageITEMGetITEM(pFieldIOCPSocket, pItem, pItem->CurrentCount);		// 2012-08-30 by jhseol, Ǯ�α� ����� - ���� ���� �α��Լ� ������ �ּ����� ����.
#endif	//#ifdef S_FULL_LOG_JHSEOL
		// end 2012-08-30 by jhseol, Ǯ�α� �����
		CAtumLogSender::SendLogMessageITEMGetITEM(pFieldIOCPSocket, pItem, pItem->CurrentCount, ItemInsertionType);		// 2008-09-26 by cmkwon, ���ս� GameLog ���� - 
		break;
	case IUT_GIVEEVENTITEM:		// 2006-08-28 by dhjin
	case IUT_BONUS_ITEM:		// 2007-09-11 by cmkwon, ��ɾ� �̺�Ʈ ������ ���� ���ӷα׿� �߰��ϱ�
	case IUT_GIVEEVENTITEM_COUPONEVENT:		// 2008-01-10 by cmkwon, ������ �̺�Ʈ �ý��ۿ� �� ���� �ý��� �߰� - 
		CAtumLogSender::SendLogMessageITEMGetItemByItemEvent(pFieldIOCPSocket, pItem, pItem->CurrentCount);		// 2007-01-11 by cmkwon, SendLogMessageITEMGetItemByItemEvent�� ������
		break;
	case IUT_TRADE:
		{
			CFieldIOCPSocket *pTradePeerFISock = m_pFieldIOCP8->GetFieldIOCPSocketByCharacterUniqueNumber(q.nGeneralParam2);
			if(pTradePeerFISock
				&& pTradePeerFISock->IsValidCharacter(FALSE))
			{
				CAtumLogSender::SendLogMessageITEMTradeRecv(pFieldIOCPSocket, pItem, pItem->CurrentCount, &pTradePeerFISock->m_character);
			}
			else
			{
				CHARACTER tmChar = *pFieldIOCPSocket->GetCharacter();
				strcpy(tmChar.CharacterName, "Unknown Character");
				tmChar.CharacterUniqueNumber	= q.nGeneralParam2;
				CAtumLogSender::SendLogMessageITEMTradeRecv(pFieldIOCPSocket, pItem, pItem->CurrentCount, &tmChar);
			}
		}
		break;
	case IUT_SHOP:
		{
			// 2008-01-10 by cmkwon, �������� ������ ������ �߰� �α� DB �� �߰� �Ŀ� ���ӷα� �߰��ϱ�
			ITEM_GENERAL tmpItemForLog	= *pItem;
			int nBuyCnts				= 1;
			if(IS_COUNTABLE_ITEM(pItem->Kind))
			{
				nBuyCnts				= pItem->CurrentCount;
			}
			CAtumLogSender::SendLogMessageITEMBuyITEM(pFieldIOCPSocket, &tmpItemForLog, nBuyCnts, -1, FALSE, UNLIMITED_REMAIN_COUNT_FOR_LIMITED_EDITION, FALSE);		// 2011-12-12 by hskim, GLog 2��
		}
		break;
	case IUT_STORE:		// 2008-02-15 by cmkwon, �κ�<->â�� �̵� �α� ���� - ������ �߰� �α� �����ϱ� �߰�
		{
			int nChangeCount = IS_COUNTABLE_ITEM(pItem->Kind) ? pItem->CurrentCount : 1;

			// 2012-12-27 by bckim, ���ϰ���, ������ �ɸ��Ͱ��� ������ �̵��� �α��߰�
			CFieldIOCPSocket tempCharacterSocket;
			tempCharacterSocket.m_character.CharacterUniqueNumber = pItem->Possess;
			tempCharacterSocket.m_character.MapChannelIndex.MapIndex = pFieldIOCPSocket->m_character.MapChannelIndex.MapIndex;
			// 2012-12-27 by bckim, ���ϰ���, ������ �ɸ��Ͱ��� ������ �̵��� �α��߰�.End		

			if(ITEM_IN_CHARACTER == pItem->ItemStorage)
			{
				CAtumLogSender::SendLogMessageITEMCharacterITEM(pFieldIOCPSocket, pItem, nChangeCount);
				// 2012-12-27 by bckim, ���ϰ���, ������ �ɸ��Ͱ��� ������ �̵��� �α��߰�
				if( pItem->Possess != pFieldIOCPSocket->m_ItemManager.m_uid32StoreCharacterUID )
				{
					CAtumLogSender::SendLogMessageITEMGetWithSameAccount(&tempCharacterSocket, pItem, nChangeCount, pFieldIOCPSocket->m_ItemManager.m_uid32StoreCharacterUID);
				}
				// 2012-12-27 by bckim, ���ϰ���, ������ �ɸ��Ͱ��� ������ �̵��� �α��߰�.End
			}
			else
			{
				CAtumLogSender::SendLogMessageITEMStoreITEM(pFieldIOCPSocket, pItem, nChangeCount);
				// 2012-12-27 by bckim, ���ϰ���, ������ �ɸ��Ͱ��� ������ �̵��� �α��߰�
				if( pItem->Possess != pFieldIOCPSocket->m_character.CharacterUniqueNumber )
				{
					CAtumLogSender::SendLogMessageITEMGetWithSameAccount(&tempCharacterSocket, pItem, nChangeCount, pFieldIOCPSocket->m_character.CharacterUniqueNumber);
				}
				// 2012-12-27 by bckim, ���ϰ���, ������ �ɸ��Ͱ��� ������ �̵��� �α��߰�.End			
			}
		}
		break;

	// 2008-09-08 by cmkwon, ���λ��� �α� ���� ���� �� �α� �߰� - ���� �������� ���� ��쵵 �α� �����.
	case IUT_BAZAAR_SELL:
		{
			int nChangeCount = IS_COUNTABLE_ITEM(pItem->Kind) ? pItem->CurrentCount : 1;
			CHARACTER tmCharac;
			util::zero(&tmCharac, sizeof(CHARACTER));

			CFieldIOCPSocket *pPeerFISock = m_pFieldIOCP8->GetFieldIOCPSocketByCharacterUniqueNumber(q.nGeneralParam2);
			if(pPeerFISock
				&& pPeerFISock->IsValidCharacter(FALSE))
			{
				tmCharac = *(pPeerFISock->GetCharacter());
			}
			else
			{
				strcpy(tmCharac.CharacterName, "Unknown Character");
				tmCharac.CharacterUniqueNumber	= q.nGeneralParam2;
			}
			CAtumLogSender::SendLogMessageITEMBazaarRecv(pFieldIOCPSocket, pItem, nChangeCount, &tmCharac);				
		}
		break;

	// 2008-09-08 by cmkwon, ���λ��� �α� ���� ���� �� �α� �߰� - ���� �������� ���� ��쵵 �α� �����.
	case IUT_BAZAAR_BUY:
		{
			int nChangeCount = IS_COUNTABLE_ITEM(pItem->Kind) ? pItem->CurrentCount : 1;
			CHARACTER tmCharac;
			util::zero(&tmCharac, sizeof(CHARACTER));
			
			CFieldIOCPSocket *pPeerFISock = m_pFieldIOCP8->GetFieldIOCPSocketByCharacterUniqueNumber(q.nGeneralParam2);
			if(pPeerFISock
				&& pPeerFISock->IsValidCharacter(FALSE))
			{
				tmCharac = *(pPeerFISock->GetCharacter());
			}
			else
			{
				strcpy(tmCharac.CharacterName, "Unknown Character");
				tmCharac.CharacterUniqueNumber	= q.nGeneralParam2;
			}
			CAtumLogSender::SendLogMessageITEMBazaarSend(pFieldIOCPSocket, pItem, nChangeCount, &tmCharac);
		}
		break;

	// 2012-12-17 by bckim, ���â�� ������ �̵� ���� �߰��α� 
	case IUT_GUILD_STORE:
		{
			CAtumLogSender::SendLogMessageITEMCharacterToFromGuildstoreResult(pFieldIOCPSocket, pItem, pItem->CurrentCount);
		}
		break;
	// 2012-12-17 by bckim, ���â�� ������ �̵� ���� �߰��α�.End
		// 2013-03-22 by jhseol, ������ �������� �α׸� ����� UID ������ �Ұ��� ������ �α׸� ����� ��ġ ����
	case IUT_ADMIN:
		{
			CAtumLogSender::SendLogMessageITEMAddItemByCommand(pFieldIOCPSocket, pItem, pItem->CurrentCount);
		}
		break;
		// end 2013-03-22 by jhseol, ������ �������� �α׸� ����� UID ������ �Ұ��� ������ �α׸� ����� ��ġ ����

	}
	
	///////////////////////////////////////////////////////////////////////////////	
	// �˻�
	// 2013-04-22 by jhseol, ������ �̺�Ʈ - ����Ÿ�� �߰����� �ý��� ����
	BOOL bPermissionArenaState = FALSE;
#ifdef S_ITEM_EVENT_SUB_TYPE_JHSEOL
	if ( IUT_GIVEEVENTITEM == ItemInsertionType )
	{
		bPermissionArenaState = TRUE;		// ������ �̺�Ʈ Ÿ�Կ����� �Ʒ��� ���µ� ��ȿ�Ѱ����� üũ
	}
#endif
	// end 2013-04-22 by jhseol, ������ �̺�Ʈ - ����Ÿ�� �߰����� �ý��� ����
	if (NULL == pFieldIOCPSocket
		|| FALSE == pFieldIOCPSocket->IsValidCharacter(FALSE, bPermissionArenaState))	// 2013-04-22 by jhseol, ������ �̺�Ʈ - ����Ÿ�� �߰����� �ý��� ����
	{
		util::del(pItem);

		// 2010-06-15 by shcho&hslee ��ý���
		util::del ( pPetCurData );

		return;
	}
	
	BOOL bSendInsertMsg = TRUE;			// 2008-02-18 by cmkwon, ī���ͺ� �������� ������ ����� �ν��Ͻ� �������Ǵ� ���� ���� - 

	mt_auto_lock igLock(&pFieldIOCPSocket->m_ItemManager.m_mapItemGeneral);	// lock m_mapItemGeneral, @QP_InsertStoreItem

	///////////////////////////////////////////////////////////////////////////////
	// 2006-09-29 by cmkwon
	pItem->CreatedTime.SetCurrentDateTime(TRUE);

	///////////////////////////////////////////////////////////////////////////////
	// 
	pFieldIOCPSocket->DecrementInsertingStoreCounts();

	// 2013-05-22 by jhseol, �ҽ����� - �ּ�����

	if (ITEM_IN_CHARACTER == pItem->ItemStorage)
	{// ĳ���� ������ ������ 

		if(IUT_USE_ITEM == ItemInsertionType
			// 2006-09-29 by cmkwon, ITEM_ATTR_TIME_LIMITE�� üũ�ϵ��� ���� && ITEMKIND_CARD == pItem->Kind
			&& COMPARE_BIT_FLAG(pItem->ItemInfo->ItemAttribute, ITEM_ATTR_TIME_LIMITE|ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED)		// 2008-11-26 by cmkwon, ��� �� �ð�(����ð�) ���� ������ ���� - 
			&& ITEM_NOT_USING != pItem->UsingTimeStamp)
		{// ĳ���Ͱ� ������� �����ۿ� �߰�

			///////////////////////////////////////////////////////////////////////////////
			// 1. ��� ���� ���� �ð��� �ٽ� �ѹ��� �ʱ�ȭ �Ѵ�
			pItem->UsingStartTime.SetCurrentDateTime();

			///////////////////////////////////////////////////////////////////////////////
			// 2008-01-31 by cmkwon, �ð����� ������ DB �� �߰� �� üũ - DB �� �߰� �� ���¿��� ItemNum �� �����Ѵ�.
			pFieldIOCPSocket->m_ItemManager.DeleteInsertingItemNumList2DB(pItem->ItemNum);

			///////////////////////////////////////////////////////////////////////////////
			// 2. m_vectUsingGeneralItem�� �����ϱ�
			// 2010-03-17 by cmkwon, ����1�� �ߵ��� ������ ���� ���� ���� - ���������� ������ �Ķ� ���� ó��
			pFieldIOCPSocket->m_ItemManager.Add2UsingTimeLimitedItemList(pItem, TRUE);

			
#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, �������� �����߰�
			if ( NULL !=  pFieldIOCPSocket->m_pFieldParty )		// ��밡 ������ 
			{
				pFieldIOCPSocket->m_pFieldParty->SetReturnUserBuffSkill(pFieldIOCPSocket); // �ʱ� ���������� ������ ���� 
			}
#endif

			///////////////////////////////////////////////////////////////////////////////
			// 3. ����� ���������� �߰��ϱ� ���� Ŭ���̾�Ʈ�� ����
			INIT_MSG_WITH_BUFFER(MSG_FC_STORE_INSERT_USINGITEM, T_FC_STORE_INSERT_USINGITEM, pSUsingItem, SendBuf);
			pSUsingItem->ItemNum				= pItem->ItemNum;
			pSUsingItem->ItemUID				= pItem->UniqueNumber;
			pSUsingItem->ItemInsertionType		= IUT_USE_ITEM;
			// 2008-11-26 by cmkwon, ��� �� �ð�(����ð�) ���� ������ ���� - �Ʒ��� ���� ���� ��.
			//pSUsingItem->nRemainSecond			= pItem->ItemInfo->Time/1000;			// �ʴ����� ��ȯ
			pSUsingItem->nRemainSecond			= CAtumSJ::GetTimeSecondByItemKind8ItemAttribute(pItem->ItemInfo);			// �ʴ����� ��ȯ
			pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_STORE_INSERT_USINGITEM));
			return;
		}


		///////////////////////////////////////////////////////////////////////////////
		// ĳ���� �κ��� �߰�
		pItem->Possess		= pFieldIOCPSocket->m_character.CharacterUniqueNumber;		// 2006-09-20 by dhjin, �ɸ��Ͱ� â�� ���� -> ���õ� �ɸ��� UID
		pFieldIOCPSocket->m_ItemManager.m_mapItemGeneral.insertLock(pItem->UniqueNumber, pItem);
		// 2013-06-25 by jhseol, ������ UID '0' ������ ����� �α�
#ifdef S_ITEMUID_ZERO_DEBUG_LOG_JHSEOL
		if (IS_COUNTABLE_ITEM(pItem->Kind))
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DEBUG] SPI ItemUID Zero : CAtumFieldDBManager::QP_InsertStoreItem - CUID(%d), ItemNum(%d), Count(%d), ItemUID(%d)\r\n"
				, pItem->Possess, pItem->ItemNum, pItem->CurrentCount, pItem->UniqueNumber);
		}
#endif
		// end 2013-06-25 by jhseol, ������ UID '0' ������ ����� �α�
		pFieldIOCPSocket->m_ItemManager.ItemInsertByInfinityPlaying(pItem);			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� �� ������ ����
		pFieldIOCPSocket->m_ItemManager.SetDelayStoredItemCounts(pItem);
		pFieldIOCPSocket->Check8SendAFSUpdateItem(pItem);			// 2010-03-31 by dhjin, ���Ǵ�Ƽ ���� ĳ�� ������ ���� - 

		// 2010-06-15 by shcho&hslee ��ý��� - ���� �� ���� ���.
		pFieldIOCPSocket->m_ItemManager.InsertItemPetByPointer ( pPetCurData );

		///////////////////////////////////////////////////////////////////////////////
		// 2008-02-18 by cmkwon, ī���ͺ� �������� ������ ����� �ν��Ͻ� �������Ǵ� ���� ���� - ī���ͺ� ������ �ߺ� üũ
		if(IS_COUNTABLE_ITEM(pItem->Kind))
		{
			ITEM_GENERAL *pFirstItemG = pFieldIOCPSocket->m_ItemManager.GetFirstItemGeneralByItemNum(pItem->ItemNum);
			if(pFirstItemG)
			{
				bSendInsertMsg	= FALSE;		// 2008-02-18 by cmkwon, ī���ͺ� �������� ������ ����� �ν��Ͻ� �������Ǵ� ���� ���� - �ߺ� ī���ͺ� �������� ���⼭ �̸� �����Ѵ�.

				// Send MSG, // 2008-02-18 by cmkwon, ī���ͺ� �������� ������ ����� �ν��Ͻ� �������Ǵ� ���� ���� - 
				// INIT_MSG_WITH_BUFFER(MSG_FC_STORE_INSERT_ITEM, T_FC_STORE_INSERT_ITEM, msgInsertItem, SendBuf);
				// msgInsertItem->FromCharacterUniqueNumber	= pFieldIOCPSocket->m_character.CharacterUniqueNumber;
				// msgInsertItem->ItemInsertionType			= ItemInsertionType;
				// msgInsertItem->ItemGeneral					= *pItem;
				// pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_STORE_INSERT_ITEM));
				
				// 2012-11-07 by jhseol, ų��ũ �� ī���ͺ� ������ ���� MSG �ߺ� ��� ���� ����
				//	�߰� �� ������ UID�� ������ �ִ� �������� UID�� ���ٸ�		=> �߰��� ī���ͺ� �������� ó�� ���޵��� �ǹ������� Inset ��Ŷ���� ������.
				//	�߰� �� ������ UID�� ������ �ִ� �������� UID�� ���� �ʴٸ�	=> �߰��� ī���ͺ� �������� �̹� ������������ �ǹ������� �Ʒ��� RearrangeCountableItems()�Լ����� Update�� ������.
				if( pFirstItemG->UniqueNumber == pItem->UniqueNumber )
				{
					// 2010-06-15 by shcho&hslee ��ý��� - ��ϵ� ������ ���� ���� �Լ��� ����
					SendItemData( pFieldIOCPSocket, ItemInsertionType, pItem, pPetCurData );
				}
				// end 2012-11-07 by jhseol, ų��ũ �� ī���ͺ� ������ ���� MSG �ߺ� ��� ���� ����

				// 2010. 08. 02 by hsLee. ��Ű �ӽ� ī���ͺ� ������ �� �������� ���� ����.
				//pFieldIOCPSocket->m_ItemManager.RearrangeCountableItems(ITEM_IN_CHARACTER, pItem->ItemNum );
				pFieldIOCPSocket->m_ItemManager.RearrangeCountableItems(ITEM_IN_CHARACTER, pItem->ItemNum , ItemInsertionType );
			}
		}
	}
	else if (ITEM_IN_STORE == pItem->ItemStorage)
	{// â�� �߰�
		// 2013-04-22 by jhseol, ������ �̺�Ʈ - ����Ÿ�� �߰����� �ý��� ����
#ifdef S_ITEM_EVENT_SUB_TYPE_JHSEOL
		if ( IUT_GIVEEVENTITEM == ItemInsertionType )
		{
			pFieldIOCPSocket->SendString128(STRING_128_USER_NOTICE, STRMSG_130422_0001, pItem->ItemInfo->ItemName, IS_COUNTABLE_ITEM(pItem->Kind) ? pItem->CurrentCount : 1);
			util::del(pItem);
			util::del(pPetCurData);
			return;
		}
#endif
		// 2013-04-22 by jhseol, ������ �̺�Ʈ - ����Ÿ�� �߰����� �ý��� ����

		if(FALSE == pFieldIOCPSocket->m_ItemManager.InsertStoreItemByPointer(pItem->UniqueNumber, pItem))
		{

			///////////////////////////////////////////////////////////////////////////////
			// 2006-10-26 by cmkwon, Ŭ���̾�Ʈ�� Error ����
			pFieldIOCPSocket->SendErrorMessage(T_FC_STORE_MOVE_ITEM, ERR_ALREADY_OPEN_OTHER_STORE);
			
			///////////////////////////////////////////////////////////////////////////////
			// 2006-10-25 by cmkwon, ���� �м��� ���ؼ�
			char szErr[1024];
			sprintf(szErr, "CAtumFieldDBManager::QP_InsertStoreItem error !!, CharacterUID(%d), StoreCharacterUID(%d), ItemUID(%I64d) ItemPossess(%d) ItemStorage(%d) ItemInsertionType(%d)\r\n"
				, pFieldIOCPSocket->m_character.CharacterUniqueNumber, pFieldIOCPSocket->m_ItemManager.m_uid32StoreCharacterUID
				, pItem->UniqueNumber, pItem->Possess, pItem->ItemStorage, ItemInsertionType);
			g_pFieldGlobal->WriteSystemLog(szErr);
			DBGOUT(szErr);
			util::del(pItem);

			// 2010-06-15 by shcho&hslee ��ý���
			util::del(pPetCurData);

			return;					// 2006-10-25 by cmkwon, �߰���
		}
	}
	else
	{
		util::del(pItem);

		// 2010-06-15 by shcho&hslee ��ý���
		util::del(pPetCurData);

		return;
	}

	if(bSendInsertMsg)
	{// 2008-02-18 by cmkwon, ī���ͺ� �������� ������ ����� �ν��Ͻ� �������Ǵ� ���� ���� - �ߺ� ī���ͺ� �������� ������ ���� ó����
		
		// Send MSG
// 		INIT_MSG_WITH_BUFFER(MSG_FC_STORE_INSERT_ITEM, T_FC_STORE_INSERT_ITEM, msgInsertItem, msgInsertItemBuf);
// 		msgInsertItem->FromCharacterUniqueNumber	= pFieldIOCPSocket->m_character.CharacterUniqueNumber;
// 		msgInsertItem->ItemInsertionType			= ItemInsertionType;
// 		msgInsertItem->ItemGeneral					= *pItem;
// 		pFieldIOCPSocket->SendAddData(msgInsertItemBuf, MSG_SIZE(MSG_FC_STORE_INSERT_ITEM));
		
		// 2010-06-15 by shcho&hslee ��ý��� - ��ϵ� ������ ���� ���� �Լ��� ����
		SendItemData(pFieldIOCPSocket,ItemInsertionType,pItem,pPetCurData);
	}
}

// QT_InsertStoreItemSkill -> CAtumFieldDBManager::QP_InsertStoreItemSkill
void CAtumFieldDBManager::QP_InsertStoreItemSkill(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	ITEM_SKILL			*pItemSkill = (ITEM_SKILL*)q.pQueryParam;
	CFieldIOCPSocket	*pFieldIOCPSocket = q.pFieldIOCPSocket;
	BYTE				ItemInsertionType = (BYTE)q.nGeneralParam1;

	RETCODE ret;
	SDWORD pcbNTS = SQL_NTS;

	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name:
	-- Desc:
	--====
	CREATE PROCEDURE dbo.atum_InsertStoreItemSkill
		@accNum			INT,
		@possess		INT,
		@ItemWindowIndex	INT,
		@ItemNum		INT
	AS
		DELETE FROM td_StoreSkill
		WHERE AccountUniqueNumber = @accNum AND Possess = @possess AND (ItemNum/10)*10 = (@ItemNum/10)*10

		INSERT INTO td_StoreSkill
		VALUES (@accNum, @possess, @ItemWindowIndex, @ItemNum, GetDate(), NULL)

		-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
		--RETURN SCOPE_IDENTITY();
		IF (@@ERROR <> 0)
		BEGIN
			SELECT 0;
			RETURN;
		END
		
		SELECT SCOPE_IDENTITY();
	GO
	**************************************************************************/

// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) - �Ʒ��� ���� ���� ��
// 	SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pItemSkill->UniqueNumber, 0, NULL);
// 	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItemSkill->AccountUniqueNumber, 0, NULL);
// 	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItemSkill->Possess, 0, NULL);
// 	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItemSkill->ItemWindowIndex, 0, NULL);
// 	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItemSkill->ItemNum, 0, NULL);
// 
// // 2008-07-08 by cmkwon, MySQL �������� ����(���ν������� ����ó�� �Ұ��� ����) - 
// //	ret = SQLExecDirect(hstmt,
// //		(UCHAR*)"{ ? = call atum_InsertStoreItemSkill(?, ?, ?, ?)}", SQL_NTS);
// 	ret = SQLExecDirect(hstmt, (UCHAR*)"{call atum_InsertStoreItemSkill(?, ?, ?, ?, ?)}", SQL_NTS);
// 
// 	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
// 	{
// 		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_InsertStoreItemSkill Failed!\n", TRUE, pFieldIOCPSocket, q);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		util::del(pItemSkill);
// 		return;
// 	}
// 
// 	// Clear any result sets generated.
// 	while ( ( ret = SQLMoreResults(hstmt) ) != SQL_NO_DATA );
//
//	// �˻�
//	if (!pFieldIOCPSocket->IsUsing())
//	{
//		SQLFreeStmt(hstmt, SQL_CLOSE);
//		util::del(pItemSkill);
//		return;
//	}
//	SQLFreeStmt(hstmt, SQL_CLOSE);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) - 
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItemSkill->AccountUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItemSkill->Possess, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItemSkill->ItemWindowIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItemSkill->ItemNum, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0100, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_InsertStoreItemSkill Failed!\n", TRUE, pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pItemSkill);
		return;
	}
	SQLBindCol(hstmt, 1, SQL_C_UBIGINT, &(pItemSkill->UniqueNumber), 0, NULL);	
	
	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(hstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 �̸� ����� RowCount �̴� 
			ret = SQLMoreResults(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(hstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(hstmt);                 // 2008-07-25 by cmkwon, ���� Results �� �ִ����� �ѹ��� üũ�Ѵ�.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);	
	if(FALSE == IS_VALID_UID64(pItemSkill->UniqueNumber))
	{// 2008-07-24 by cmkwon, �߰��� �������� ItemUID üũ
		if(pFieldIOCPSocket->IsValidCharacter(FALSE))
		{
			pFieldIOCPSocket->SendErrorMessage(T_FC_SKILL_SETUP_SKILL, ERR_DB_EXECUTION_FAILED);
		}
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] atum_InsertStoreItemSkill@ error !! %s %s\r\n"
			, GetCharacterString(pFieldIOCPSocket->GetCharacter(), string()), GetItemSkillString(pItemSkill, string()));
		util::del(pItemSkill);
		return;
	}

	// �˻�
	if (FALSE == pFieldIOCPSocket->IsValidCharacter(FALSE))
	{
		util::del(pItemSkill);
		return;
	}

	mt_auto_lock igLock(&pFieldIOCPSocket->m_ItemManager.m_mapItemGeneral);			// lock m_mapItemGeneral, @QP_GetStoreItem

	// ������ ��ų ����
	// 2013-03-12 by jhseol, ��ų �ѹ��� �ý��� ����
#ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
	pFieldIOCPSocket->m_ItemManager.DeleteItemSkillBySkillBaseNum(pItemSkill->ItemInfo->NextSkill, TRUE);
#else	// #ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
	pFieldIOCPSocket->m_ItemManager.DeleteItemSkillBySkillBaseNum(pItemSkill->ItemNum, TRUE);
#endif	// #ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
	// end 2013-03-12 by jhseol, ��ų �ѹ��� �ý��� ����

	// ĳ���Ϳ��� ����
	pItemSkill->UseTime.Reset();			// 2006-12-06 by cmkwon, ó�� ��ų �߰��ô� �ʱ�ȭ�� ������ �����Ѵ�.
	if(FALSE == pFieldIOCPSocket->m_ItemManager.InsertItemSkillByPointer(pItemSkill->UniqueNumber, pItemSkill))
	{
		///////////////////////////////////////////////////////////////////////////////
		// 2008-07-23 by cmkwon, ������ ���� ���� üũ �߰�
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] InsertItemSkillByPointer error !!, CharInfo(%s) SkillItemInfo(%s)\r\n"
			, GetCharacterString(pFieldIOCPSocket->GetCharacter(), string()), GetItemSkillString(pItemSkill, string()));
		util::del(pItemSkill);
		return;
	}

	// ����
	if (pItemSkill->ItemInfo->SkillType == SKILLTYPE_PERMANENT)
	{
		pFieldIOCPSocket->m_SkillManager.SetSkillParamFactor(pItemSkill->ItemInfo);
	}

	// ��ų �� enchant ���� ���� �ٷ� ����
	if (pFieldIOCPSocket->IsUsing())
	{
		pFieldIOCPSocket->ResetAllSkillAndEnchant();	// called CAtumFieldDBManager::QP_InsertStoreItemSkill#, skill and enchant ����
	}

	// Send MSG
	INIT_MSG_WITH_BUFFER(MSG_FC_SKILL_SETUP_SKILL_OK, T_FC_SKILL_SETUP_SKILL_OK, pSetupSkillOK, pSetupSkillOKBuf);
	pSetupSkillOK->ItemUpdateType = ItemInsertionType;
	pSetupSkillOK->ItemSkill = *pItemSkill;
	pFieldIOCPSocket->SendAddData(pSetupSkillOKBuf, MSG_SIZE(MSG_FC_SKILL_SETUP_SKILL_OK));

#ifdef _DEBUG
	pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_S_F2DBQUERY_0004, pItemSkill->ItemInfo->ItemName, pItemSkill->ItemInfo->ItemNum);
#endif
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_UpdateStoreItemSkill(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		
/// \author		dhjin
/// \date		2006-11-17 ~ 2006-11-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_UpdateStoreItemSkillUseTime(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_UPDATE_SKILLUSETIME	*pSkillUseTime = (QPARAM_UPDATE_SKILLUSETIME*)q.pQueryParam;
	CFieldIOCPSocket	*pFieldIOCPSocket = q.pFieldIOCPSocket;

	RETCODE ret;
	SDWORD pcbNTS = SQL_NTS;

	char UseTime[SIZE_MAX_SQL_DATETIME_STRING];
	pSkillUseTime->UseTime.GetSQLDateTimeString(UseTime, SIZE_MAX_SQL_DATETIME_STRING);

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_UpdateStoreItemSkillUseTime
		@ItemUID		BIGINT,
		@i_UseTime		VARCHAR(30)
	AS
		UPDATE td_storeSkill
		SET UseTime = @i_UseTime
		WHERE UniqueNumber = @ItemUID
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pSkillUseTime->ItemUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, UseTime, 0, NULL);
	
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0101, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateStoreItemSkillUseTime Failed!\n", TRUE, pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pSkillUseTime);
		return;
	}

	// Clear any result sets generated.
	while ( ( ret = SQLMoreResults(hstmt) ) != SQL_NO_DATA );

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pSkillUseTime);
}

// QT_UpdateItemCount
void CAtumFieldDBManager::QP_UpdateItemCount(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_UPDATEITEMCOUNT *pRMsg = (QPARAM_UPDATEITEMCOUNT*)q.pQueryParam;

	RETCODE ret;
	SDWORD pcbNTS = SQL_NTS;

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_UpdateItemCount
		@UniqueNumber	BIGINT,
		@Count			INT
	AS
	UPDATE td_store
	SET Count = @Count
	WHERE UniqueNumber = @UniqueNumber;
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0,
					&pRMsg->ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0,
					&pRMsg->Count, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0102, SQL_NTS);

	if(ret == SQL_NO_DATA)
	{
		///////////////////////////////////////////////////////////////////////////
		// �������� �������� ����
		q.pFieldIOCPSocket->SendErrorMessageFromFieldDB(QT_UpdateItemCount, ERR_DB_NO_SUCH_STORE_ITEM, pRMsg->ItemUniqueNumber, pRMsg->Count);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO ) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateItemCount Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}

	// 2013-05-20 by hskim, [���� �ý���] ���������� ������� ������ �߰� ����
#ifdef S_DIRECT_DB_ITEM_INSERT_SECURITY_HSKIM
	if( FALSE == g_pFieldGlobal->IsArenaServer() )	
	{
		QPARAM_INSERT_STORE_EXTENSION *pQParamISE = new QPARAM_INSERT_STORE_EXTENSION;
		pQParamISE->ItemUID = pRMsg->ItemUniqueNumber;
		pQParamISE->Gesture = m_pFieldIOCP8->GeneratorItemSecurityCheckSum(pRMsg->ItemUniqueNumber, pRMsg->Count);
		this->MakeAndEnqueueQuery(QT_InsertStoreExtension, q.pFieldIOCPSocket, q.pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber, pQParamISE);
	}
#endif
	// end 2013-05-20 by hskim, [���� �ý���] ���������� ������� ������ �߰� ����	

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_UpdateItemNum(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		
/// \author		cmkwon
/// \date		2006-06-14 ~ 2006-06-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_UpdateItemNum(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_UPDATEITEMNUM *pRMsg = (QPARAM_UPDATEITEMNUM*)q.pQueryParam;

	RETCODE ret;
	SDWORD pcbNTS = SQL_NTS;

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_UpdateItemNum
		@UniqueNumber	BIGINT,
		@ItemNum		INT
	AS
		UPDATE td_store
			SET ItemNum = @ItemNum
			WHERE UniqueNumber = @UniqueNumber
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0,
					&pRMsg->ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0,
					&pRMsg->ItemNum, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0103, SQL_NTS);
	if(ret == SQL_NO_DATA)
	{
		///////////////////////////////////////////////////////////////////////////
		// �������� �������� ����
		q.pFieldIOCPSocket->SendErrorMessageFromFieldDB(QT_UpdateItemNum, ERR_DB_NO_SUCH_STORE_ITEM, pRMsg->ItemUniqueNumber, pRMsg->ItemNum);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateItemNum Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumFieldDBManager::QP_UpdateEndurance(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_UPDATE_ENDURANCE *pParamEndurance = (QPARAM_UPDATE_ENDURANCE*)q.pQueryParam;

	RETCODE ret;
	SDWORD pcbNTS = SQL_NTS;

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_UpdateEndurance
		@UniqueNumber	BIGINT,
		@Endurance		FLOAT
	AS
	UPDATE td_store
	SET Endurance = @Endurance
	WHERE UniqueNumber = @UniqueNumber;
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0,
					&pParamEndurance->ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0,
					&pParamEndurance->Endurance, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0104, SQL_NTS);

	if(ret == SQL_NO_DATA)
	{
		///////////////////////////////////////////////////////////////////////////
		// �������� �������� ����
		q.pFieldIOCPSocket->SendErrorMessageFromFieldDB(QT_UpdateEndurance, ERR_DB_NO_SUCH_STORE_ITEM);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pParamEndurance);
		return;
	}

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO ) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateEndurance Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pParamEndurance);
		return;
	}

	INIT_MSG_WITH_BUFFER(MSG_FC_STORE_UPDATE_ENDURANCE, T_FC_STORE_UPDATE_ENDURANCE, msgUpdateEndurance, msgUpdateEnduranceBuf);
	msgUpdateEndurance->ItemUniqueNumber = pParamEndurance->ItemUniqueNumber;
	msgUpdateEndurance->Endurance = pParamEndurance->Endurance;
	q.pFieldIOCPSocket->SendAddData(msgUpdateEnduranceBuf, MSG_SIZE(MSG_FC_STORE_UPDATE_ENDURANCE));

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pParamEndurance);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_StoreUpdateColorCode(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		
/// \author		cmkwon
/// \date		2005-11-21 ~ 2005-11-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_StoreUpdateColorCode(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_STORE_UPDATE_COLORCODE *pUpdateColorCode = (QPARAM_STORE_UPDATE_COLORCODE*)q.pQueryParam;

	RETCODE ret;
	SDWORD pcbNTS = SQL_NTS;

	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name:
	-- Desc:2005-11-21 by cmkwon
	--		�Ƹ� �������� Į�� �ؽ��� �ڵ� ������Ʈ
	--====
	CREATE PROCEDURE atum_StoreUpdateColorCode
		@UniqueNumber	BIGINT,
		@ColorCode		INT
	AS
		UPDATE td_Store
		SET ColorCode = @ColorCode
		WHERE UniqueNumber = @UniqueNumber
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0,
					&pUpdateColorCode->ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0,
					&pUpdateColorCode->ColorCode, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0105, SQL_NTS);

	if(ret == SQL_NO_DATA)
	{// �������� �������� ����
		q.pFieldIOCPSocket->SendErrorMessageFromFieldDB(QT_StoreUpdateColorCode, ERR_DB_NO_SUCH_STORE_ITEM);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pUpdateColorCode);
		return;
	}

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO ) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_StoreUpdateColorCode Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pUpdateColorCode);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
	INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_CHANGED_EFFECTITEMNUM, T_FC_ITEM_CHANGED_EFFECTITEMNUM, pSMsg, SendBuf);
	pSMsg->nItemUID			= pUpdateColorCode->ItemUniqueNumber;
	pSMsg->nEffectItemNum	= pUpdateColorCode->ColorCode;
	q.pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ITEM_CHANGED_EFFECTITEMNUM));

	util::del(pUpdateColorCode);
}

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
void CAtumFieldDBManager::QP_UpdateShapeItemNum(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_UpdateShapeItemNum
	QPARAM_UPDATE_SHAPEITEMNUM *pUpdateShapeItemNum = (QPARAM_UPDATE_SHAPEITEMNUM*)q.pQueryParam;

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Update_ShapeItemNum
	-- DESC				: -- // 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
	--						
	--------------------------------------------------------------------------------
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pUpdateShapeItemNum->ItemUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0,	&pUpdateShapeItemNum->nShapeItemNum, 0, NULL);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_090826_0398, SQL_NTS);

	if(ret == SQL_NO_DATA)
	{// �������� �������� ����
		q.pFieldIOCPSocket->SendErrorMessageFromFieldDB(QT_UpdateShapeItemNum, ERR_DB_NO_SUCH_STORE_ITEM, pUpdateShapeItemNum->ItemUID, pUpdateShapeItemNum->nShapeItemNum);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pUpdateShapeItemNum);
		return;
	}

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO ) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"PROCEDURE_090826_0398$ Failed! \r\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pUpdateShapeItemNum);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
	INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_CHANGED_SHAPEITEMNUM, T_FC_ITEM_CHANGED_SHAPEITEMNUM, pSMsg, SendBuf);
	pSMsg->nItemUID			= pUpdateShapeItemNum->ItemUID;
	pSMsg->nShapeItemNum	= pUpdateShapeItemNum->nShapeItemNum;

	// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)
	pSMsg->FixedTermShape.bActive = pUpdateShapeItemNum->FixedTermShape.bActive;
	pSMsg->FixedTermShape.StartDate = pUpdateShapeItemNum->FixedTermShape.StartDate;
	pSMsg->FixedTermShape.EndDate = pUpdateShapeItemNum->FixedTermShape.EndDate;
	pSMsg->FixedTermShape.TimerUID = pUpdateShapeItemNum->FixedTermShape.TimerUID;
	// end 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)

	// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - QP_UpdateShapeItemNum, T_FC_ITEM_CHANGED_SHAPEITEMNUM ����
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM	// - QP_UpdateShapeItemNum, T_FC_ITEM_CHANGED_SHAPEITEMNUM ����
	pSMsg->FixedTermShape.nStatLevel		= 	pUpdateShapeItemNum->FixedTermShape.nStatLevel;
	pSMsg->FixedTermShape.nStatShapeItemNum	= 	pUpdateShapeItemNum->FixedTermShape.nStatShapeItemNum;
#endif
	// end 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - QP_UpdateShapeItemNum, T_FC_ITEM_CHANGED_SHAPEITEMNUM ����
	
	q.pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ITEM_CHANGED_SHAPEITEMNUM));

	q.pFieldIOCPSocket->UpdateItemRenderInfo(POS_CENTER, TRUE);		// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)

	util::del(pUpdateShapeItemNum);
}

void CAtumFieldDBManager::QP_UpdateItemUsingTimeStamp(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_UPDATE_ITEM_USINGTIMESTAMP *pItemUsingTimeStamp = (QPARAM_UPDATE_ITEM_USINGTIMESTAMP*)q.pQueryParam;

	RETCODE ret;
	SDWORD pcbNTS = SQL_NTS;

	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name:
	-- Desc:
	--====
	CREATE PROCEDURE atum_UpdateItemUsingTimeStamp
		@UniqueNumber		BIGINT,
		@UsingTimeStamp		INT
	AS
		UPDATE td_Store
		SET UsingTimeStamp = @UsingTimeStamp
		WHERE UniqueNumber = @UniqueNumber
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pItemUsingTimeStamp->ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pItemUsingTimeStamp->UsingTimeStamp1, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0106, SQL_NTS);

	if(ret == SQL_NO_DATA)
	{
		///////////////////////////////////////////////////////////////////////////
		// �������� �������� ����
		q.pFieldIOCPSocket->SendErrorMessageFromFieldDB(QT_UpdateItemUsingTimeStamp, ERR_DB_NO_SUCH_STORE_ITEM);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pItemUsingTimeStamp);
		return;
	}

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO ) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateItemUsingTimeStamp Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pItemUsingTimeStamp);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	
	util::del(pItemUsingTimeStamp);
}

// QT_UpdateItemRareFix
void CAtumFieldDBManager::QP_UpdateItemRareFix(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_UPDATE_ITEM_RARE_FIX *pParamUpdateRareFix = (QPARAM_UPDATE_ITEM_RARE_FIX*)q.pQueryParam;

	RETCODE ret;
	SDWORD pcbNTS = SQL_NTS;

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_UpdateItemRareFix
		@i_ItemUID			BIGINT,
		@i_PrefixCodeNum	INT,
		@i_SuffixCodeNum	INT
	AS
		UPDATE td_Store
		SET PrefixCodeNum = @i_PrefixCodeNum, SuffixCodeNum = @i_SuffixCodeNum
		WHERE UniqueNumber = @i_ItemUID
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &pParamUpdateRareFix->ItemUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pParamUpdateRareFix->PrefixCodeNum, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pParamUpdateRareFix->SuffixCodeNum, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0107, SQL_NTS);

	if(ret == SQL_NO_DATA)
	{
		///////////////////////////////////////////////////////////////////////////
		// �������� �������� ����
		q.pFieldIOCPSocket->SendErrorMessageFromFieldDB(QT_UpdateItemRareFix, ERR_DB_NO_SUCH_STORE_ITEM);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pParamUpdateRareFix);
		return;
	}

	if (ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateItemRareFix Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pParamUpdateRareFix);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pParamUpdateRareFix);
}

void CAtumFieldDBManager::QP_InsertDefaultItems(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_INSERTDEFAULTITEMS *pRMsg = (QPARAM_INSERTDEFAULTITEMS*)q.pQueryParam;

	RETCODE ret;
	SQLINTEGER cb1 = 0;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,
					&pRMsg->CharacterUniqueNumber, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0108, SQL_NTS);

	if(ret == SQL_NO_DATA)
	{
		///////////////////////////////////////////////////////////////////////////
		// �������� �������� ����
		q.pFieldIOCPSocket->SendErrorMessageFromFieldDB(QT_InsertDefaultItems, ERR_DB_NO_SUCH_STORE_ITEM);
		ret = SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO  && ret!=SQL_NO_DATA) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_InsertDefaultItems Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}

	ret = SQLFreeStmt(hstmt, SQL_CLOSE);

	util::del(pRMsg);
}

void CAtumFieldDBManager::QP_UpdateWindowItemList(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FC_ITEM_UPDATE_WINDOW_ITEM_LIST *pUpdateItem = (MSG_FC_ITEM_UPDATE_WINDOW_ITEM_LIST*)q.pQueryParam;
	int offset = sizeof(INT);	// sizeof(pUpdateItem->NumOfItems)

	for (int i = 0; i < pUpdateItem->NumOfItems; i++)
	{
		ITEM_WINDOW_INFO *pItemWndInfo = (ITEM_WINDOW_INFO*)(((char*)q.pQueryParam) + offset);
		offset += sizeof(ITEM_WINDOW_INFO);

		if (IS_SKILL_ITEM(pItemWndInfo->ItemKind))
		{
			// SKILL item�� ���

			// check: �����ؾ� ��!
		}
		else
		{
			RETCODE ret;
			SQLINTEGER cb1 = NULL, cb2 = NULL, cb3 = NULL;

			// SKILL �̿��� item�� ���
			SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0,
				&pItemWndInfo->ItemUniqueNumber, 0, &cb1);
			SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0,
				&pItemWndInfo->Wear, 0, &cb2);
			SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0,
				&pItemWndInfo->ItemWindowIndex, 0, &cb3);

			ret = SQLExecDirect(hstmt, PROCEDURE_080822_0109, SQL_NTS);

			if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_NO_DATA)
			{
				ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateItemWindowList Failed!\n", TRUE, q.pFieldIOCPSocket, q);
				SQLFreeStmt(hstmt, SQL_CLOSE);
				util::del(pUpdateItem);
				return;
			}
			SQLFreeStmt(hstmt, SQL_CLOSE);
		}
	}
	
	util::del(pUpdateItem);
}

// QT_ChangeItemPosition -> CAtumFieldDBManager::QP_ChangeItemPosition()
void CAtumFieldDBManager::QP_ChangeItemPosition(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_CHANGE_ITEM_WINDOW_POSITION *pChangeWinPos
		= (QPARAM_CHANGE_ITEM_WINDOW_POSITION*)q.pQueryParam;

	if(NULL == q.pFieldIOCPSocket
		|| FALSE == q.pFieldIOCPSocket->IsValidCharacter(FALSE)
		|| FALSE == IS_VALID_UNIQUE_NUMBER(pChangeWinPos->AccountUID)
		|| FALSE == IS_VALID_UNIQUE_NUMBER(pChangeWinPos->CharacterUID))
	{// 2009-04-20 by cmkwon, �ý��� �α� �߰� - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] Parameter error !! QP_ChangeItemPosition %s %d %d %I64d %I64d \r\n"
			, GetCharacterString(q.pFieldIOCPSocket->GetCharacter(), string()), pChangeWinPos->AccountUID, pChangeWinPos->CharacterUID, pChangeWinPos->ItemUID1, pChangeWinPos->ItemUID2);
		util::del(pChangeWinPos);
		return;
	}

	SQLRETURN ret;

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_ChangeWindowPosition
		@i_AccountUID			INT,
		@i_CharacterUID			INT,
		@i_ItemUID1				BIGINT,
		@i_ItemWindowIndex1		INT,
		@i_ItemWear1			TINYINT,
		@i_ItemUID2				BIGINT,
		@i_ItemWindowIndex2		INT,
		@i_ItemWear2			TINYINT
	AS
	UPDATE td_store
		SET		ItemWindowIndex = @i_ItemWindowIndex1, Wear = @i_ItemWear1
		WHERE	UniqueNumber = @i_ItemUID1 AND Possess = @i_CharacterUID AND AccountUniqueNumber = @i_AccountUID
	UPDATE td_store
		SET		ItemWindowIndex = @i_ItemWindowIndex2, Wear = @i_ItemWear2
		WHERE	UniqueNumber = @i_ItemUID2 AND Possess = @i_CharacterUID AND AccountUniqueNumber = @i_AccountUID
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pChangeWinPos->AccountUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pChangeWinPos->CharacterUID, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &pChangeWinPos->ItemUID1, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pChangeWinPos->ItemWindowIndex1, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pChangeWinPos->ItemWear1, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &pChangeWinPos->ItemUID2, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pChangeWinPos->ItemWindowIndex2, 0, NULL);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pChangeWinPos->ItemWear2, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0110, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_ChangeWindowPosition Failed!\r\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pChangeWinPos);
		return;
	}

	ret = SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pChangeWinPos);
}

// 2013-05-22 by jhseol, �ҽ����� - �ּ�����

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_TradeMoveItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		QT_TradeMoveItem ó��
/// \author		cmkwon
/// \date		2006-05-11 ~ 2006-05-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_TradeMoveItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_TRADE_MOVEITEM *pParam = (QPARAM_TRADE_MOVEITEM*)q.pQueryParam;
	/*[Stored Query Definition]****************************************
	CREATE PROCEDURE atum_TradeMoveItem
		@ToCharacter			INT,
		@ToAccount				INT,
		@FromCharacter			INT,
		@ItemUniqueNumber		BIGINT			-- 2006-05-11 by cmkwon, INT�� ������
	AS
		UPDATE td_Store
			SET AccountUniqueNumber = @ToAccount, Possess = @ToCharacter, Wear = 0, ItemWindowIndex = 99
			WHERE UniqueNumber = @ItemUniqueNumber AND Possess = @FromCharacter
	GO
	******************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pParam->ToCharacterUID, 0,	 NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pParam->ToAccountUID, 0,	 NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pParam->FromCharacterUID, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pParam->MoveItemUID, 0,	 NULL);

	BOOL bRet = SQLExecDirect(hstmt,	PROCEDURE_080822_0111, SQL_NTS);

	if ( bRet!=SQL_SUCCESS && bRet!=SQL_SUCCESS_WITH_INFO )
	{
		char szSysLog[1024];
		wsprintf(szSysLog, "[DB Error]: QP_TradeMoveItem_ error, MoveItmeUID(%I64d) ToCharacterUID(%d) ToAccountUID(%d) FromCharacterUID(%d)\r\n"
			, pParam->MoveItemUID, pParam->ToCharacterUID, pParam->ToAccountUID, pParam->FromCharacterUID);
		DbgOut(szSysLog);
		g_pFieldGlobal->WriteSystemLog(szSysLog);
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "atum_TradeMoveItem Failed!\r\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pParam);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pParam);
}

// QT_InsertEnchant
void CAtumFieldDBManager::QP_InsertEnchant(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	ENCHANT *pENCHANT = (ENCHANT*)q.pQueryParam;
	CFieldIOCPSocket *pFieldIOCPSocket = q.pFieldIOCPSocket;

	RETCODE ret;
	SDWORD pcbNTS = SQL_NTS;

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_InsertEnchant
		@i_TargetItemUniqueNumber	BIGINT,
		@i_TargetItemNum			INT,
		@i_EnchantItemNum			INT
	AS
		INSERT INTO td_enchant
		VALUES (@i_TargetItemUniqueNumber, @i_TargetItemNum, @i_EnchantItemNum)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pENCHANT->TargetItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pENCHANT->TargetItemNum, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pENCHANT->EnchantItemNum, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0112, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_InsertEnchant Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pENCHANT);
		return;
	}

	// 2013-01-15 by bckim, ��æƮ������ �������ѹ� �߰�
	ENCHANT tmpEnchant;
	util::zero(&tmpEnchant, sizeof(ENCHANT));
	ret = SQLBindCol(hstmt, 1, SQL_C_UBIGINT, &tmpEnchant.SequenceNumber_DB, 0, NULL);	
	
	SQLSMALLINT    nColCnts = 0;
	while( TRUE )
	{
		SQLNumResultCols(hstmt, &nColCnts);
		if(nColCnts <= 0)
		{
			ret = SQLMoreResults(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_InsertEnchant Get SequenceNumber Failed!\n", TRUE, q.pFieldIOCPSocket, q); // 2013-04-04 by bckim, ��æƮ ������ �ѹ� �������ϴ� ���� 
				break;
			}
			continue;
		}

		ret = SQLFetch(hstmt);
		// 2013-04-04 by bckim, ��æƮ ������ �ѹ� �������ϴ� ���� 
		if( 0 == tmpEnchant.SequenceNumber_DB )
		{
			 g_pFieldGlobal->WriteSystemLogEX(TRUE, "[GET_ENCHANT_SEQ_NUM_ERROR!!]\r\n");
		}
		// End 2013-04-04 by bckim, ��æƮ ������ �ѹ� �������ϴ� ���� 
		if( ret == SQL_SUCCESS && 0 < tmpEnchant.SequenceNumber_DB )
		{	
			if( NULL == pFieldIOCPSocket)
			{
				break;
			}

			pFieldIOCPSocket->m_mapEnchant.lock();
			mtmapUID2EnchantVector::iterator itrEnchantList = pFieldIOCPSocket->m_mapEnchant.find(pENCHANT->TargetItemUniqueNumber);
			if (itrEnchantList != pFieldIOCPSocket->m_mapEnchant.end())
			{
				vectENCHANT &vecEnchant =  itrEnchantList->second;
				vectENCHANT::iterator itrvectEnchant = vecEnchant.begin();
				int count = 0;
				while( itrvectEnchant != vecEnchant.end())
				{
					if( 0 == itrvectEnchant->SequenceNumber_DB || pENCHANT->SequenceNumber_DB == itrvectEnchant->SequenceNumber_DB )
					{
						g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ENCHANT_SEQ_NUM_BEFORE][%d] [%I64d][%d][%d][%I64d]\r\n",count,itrvectEnchant->TargetItemUniqueNumber,itrvectEnchant->TargetItemNum,itrvectEnchant->EnchantItemNum,itrvectEnchant->SequenceNumber_DB);
						itrvectEnchant->SequenceNumber_DB = tmpEnchant.SequenceNumber_DB;
						g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ENCHANT_SEQ_NUM_AFTER ][%d] [%I64d][%d][%d][%I64d]\r\n",count,itrvectEnchant->TargetItemUniqueNumber,itrvectEnchant->TargetItemNum,itrvectEnchant->EnchantItemNum,itrvectEnchant->SequenceNumber_DB);
						break;
					}
					itrvectEnchant++;
					count++;
				}
				pFieldIOCPSocket->m_mapEnchant.unlock();
				break;
			}
		}
		ret = SQLMoreResults(hstmt);
	}
	// End. 2013-01-15 by bckim, ��æƮ������ �������ѹ� �߰�

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pENCHANT);
}

void CAtumFieldDBManager::QP_DeleteAllEnchant(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	ENCHANT *pENCHANT					= (ENCHANT*)q.pQueryParam;
	CFieldIOCPSocket *pFieldIOCPSocket	= q.pFieldIOCPSocket;

	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name:
	-- Desc:
	--====
	CREATE PROCEDURE atum_DeleteAllEnchant
		@i_TargetItemUniqueNumber	BIGINT
	AS
		DELETE FROM td_Enchant
			WHERE TargetItemUniqueNumber = @i_TargetItemUniqueNumber
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pENCHANT->TargetItemUniqueNumber, 0, NULL);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0113, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_DeleteAllEnchant Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pENCHANT);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pENCHANT);
}

// 2007-12-21 by dhjin, ���� ���� ��ƾ DB�� ������ Client���� �κ� �и� - �ؿ� �Լ��� ����
// QT_GetAllQuest -> CAtumFieldDBManager::QP_GetAllQuest()
//void CAtumFieldDBManager::QP_GetAllQuest(FIELD_DB_QUERY q, SQLHSTMT hstmt)
//{
//	CFieldIOCPSocket *pFieldIOCPSocket = q.pFieldIOCPSocket;
//	// �˻�
//	if (NULL == pFieldIOCPSocket
//		|| FALSE == pFieldIOCPSocket->IsValidCharacter(FALSE))
//	{
//		return;
//	}
//
//	mtmapQuestIndex2FieldCharacterQuest	tmmapFCharacterQuest;
//	CFieldCharacterQuest				tmFCharQuest;
//	CharacterQuest						retQuest;
//	RETCODE								ret;
//	SQLINTEGER							arrCB[6]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS};
//	SQL_TIMESTAMP_STRUCT				QuestStartTime;
//	/*[Stored Query Definition]************************************************
//	CREATE PROCEDURE atum_GetAllQuest
//		@CharacterUniqueNumber	INT
//	AS
//		SELECT *
//		FROM td_CharacterQuest
//		WHERE CharacterUniqueNumber = @CharacterUniqueNumber
//	GO
//	**************************************************************************/
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pFieldIOCPSocket->m_character.CharacterUniqueNumber, 0, NULL);
//	ret = SQLExecDirect(hstmt, (UCHAR*)"{call dbo.atum_GetAllQuest(?)}", SQL_NTS);
//	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
//	{
//		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_GetAllQuest Failed!\n", TRUE, pFieldIOCPSocket, q);
//		SQLFreeStmt(hstmt, SQL_CLOSE);
//		return;
//	}
//
//	SQLBindCol(hstmt,  1, SQL_C_LONG   , &retQuest.CharacterUniqueNumber, 0,	&arrCB[1]);
//	SQLBindCol(hstmt,  2, SQL_C_LONG   , &retQuest.QuestIndex, 0,				&arrCB[2]);
//	SQLBindCol(hstmt,  3, SQL_C_TINYINT, &retQuest.QuestState, 0,				&arrCB[3]);
//	SQLBindCol(hstmt,  4, SQL_C_LONG   , &retQuest.QuestParam1, 0,				&arrCB[4]);
//	SQLBindCol(hstmt,  5, SQL_C_TIMESTAMP, &QuestStartTime, 0,					&arrCB[5]);
//	SQLBindCol(hstmt,  6, SQL_C_SBIGINT, &retQuest.QuestPlayTimeStamp, 0,		&arrCB[6]);
//
//	// initialize to 0x00
//	memset(&retQuest, 0x00, sizeof(CharacterQuest));
//	memset(&QuestStartTime, 0, sizeof(SQL_TIMESTAMP_STRUCT));
//	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA )
//	{
//
//		CQuest *pQuestInfo = m_pFieldIOCP8->m_mapQuestInfoFromScript.findEZ(retQuest.QuestIndex);
//		if (NULL == pQuestInfo)
//		{// ���� ����Ʈ ��ũ��Ʈ�� �������� �ʴ� ����Ʈ�ε����� �߰� ���� �ʴ´�
//			continue;
//		}
//		retQuest.QuestTimeStamp = QuestStartTime;		// ����Ʈ ���� �ð� �Ҵ�
//		tmFCharQuest.ResetFieldCharacterQuest(&retQuest, pQuestInfo);
//
//		tmmapFCharacterQuest.insertNoLock(tmFCharQuest.QuestIndex, tmFCharQuest);
//		
//		memset(&retQuest, 0x00, sizeof(CharacterQuest));
//		memset(&QuestStartTime, 0, sizeof(SQL_TIMESTAMP_STRUCT));
//	}
//	SQLFreeStmt(hstmt, SQL_CLOSE);
//	
//	if(FALSE == pFieldIOCPSocket->IsValidCharacter(FALSE)
//		|| tmmapFCharacterQuest.empty())
//	{// 2005-10-21 by cmkwon, �Ұ��� ����ǰų� �ε��� ����Ʈ�� ������ ����
//		return;
//	}
//	
//	///////////////////////////////////////////////////////////////////////////////
//	// 2005-10-19 by cmkwon, ����Ʈ ���� ī��Ʈ 
//	DBCharacterQuestMonsterCount	tmDBMonsterCount;
//	/*[Stored Query Definition]************************************************
//	--!!!!
//	-- Name:
//	-- Desc:
//	--====
//	CREATE PROCEDURE atum_GetAllQuestMonsterCount
//		@CharacterUniqueNumber		INT
//	AS
//		SELECT *
//		FROM td_CharacterQuestMonsterCount
//		WHERE CharacterUniqueNumber = @CharacterUniqueNumber
//	GO
//	**************************************************************************/
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pFieldIOCPSocket->m_character.CharacterUniqueNumber, 0, NULL);
//	ret = SQLExecDirect(hstmt, (UCHAR*)"{call dbo.atum_GetAllQuestMonsterCount(?)}", SQL_NTS);
//	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
//	{
//		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_GetAllQuestMonsterCount Failed!\n", TRUE, pFieldIOCPSocket, q);
//		SQLFreeStmt(hstmt, SQL_CLOSE);
//		return;
//	}
//	util::zero(arrCB, sizeof(arrCB[0])*6);
//	SQLBindCol(hstmt,  1, SQL_C_LONG   , &tmDBMonsterCount.CharacterUniqueNumber, 0,	&arrCB[1]);
//	SQLBindCol(hstmt,  2, SQL_C_LONG   , &tmDBMonsterCount.QuestIndex, 0,				&arrCB[2]);
//	SQLBindCol(hstmt,  3, SQL_C_LONG   , &tmDBMonsterCount.MonsterUniqueNumber, 0,		&arrCB[3]);
//	SQLBindCol(hstmt,  4, SQL_C_LONG   , &tmDBMonsterCount.Count, 0,					&arrCB[4]);
//
//	util::zero(&tmDBMonsterCount, sizeof(DBCharacterQuestMonsterCount));
//	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA )
//	{
//		CFieldCharacterQuest *pFCharQuest = tmmapFCharacterQuest.findLock_Ptr(tmDBMonsterCount.QuestIndex);
//		if(NULL == pFCharQuest
//			|| QUEST_STATE_IN_PROGRESS != pFCharQuest->QuestState)
//		{
//			// ����Ʈ ���� ���� ����
//			QPARAM_DELETE_QUEST_MONSTER_COUNT *pDelMonCount = new QPARAM_DELETE_QUEST_MONSTER_COUNT;
//			pDelMonCount->CharacterUniqueNumber		= tmDBMonsterCount.CharacterUniqueNumber;
//			pDelMonCount->QuestIndex				= tmDBMonsterCount.QuestIndex;
//			this->MakeAndEnqueueQuery(QT_DeleteQuestMonsterCount, pFieldIOCPSocket, pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber, pDelMonCount);
//			continue;
//		}
//		pFCharQuest->InsertMonsterCount(&tmDBMonsterCount);
//		
//		// �ʱ�ȭ
//		util::zero(&tmDBMonsterCount, sizeof(DBCharacterQuestMonsterCount));
//	}
//	SQLFreeStmt(hstmt, SQL_CLOSE);
//
//	pFieldIOCPSocket->OnGetAllQuest(&tmmapFCharacterQuest);
//}
void CAtumFieldDBManager::GetAllQuest(CFieldIOCPSocket *pIOCPSocket, SQLHSTMT hstmt)
{
	// �˻�
	if (NULL == pIOCPSocket
		|| FALSE == pIOCPSocket->IsValidCharacter(FALSE))
	{
		return;
	}

	mtmapQuestIndex2FieldCharacterQuest	tmmapFCharacterQuest;
	CFieldCharacterQuest				tmFCharQuest;
	CharacterQuest						retQuest;
	RETCODE								ret;
	SQLINTEGER							arrCB[6]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS};
	SQL_TIMESTAMP_STRUCT				QuestStartTime;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_GetAllQuest
		@CharacterUniqueNumber	INT
	AS
		SELECT *
		FROM td_CharacterQuest
		WHERE CharacterUniqueNumber = @CharacterUniqueNumber
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pIOCPSocket->m_character.CharacterUniqueNumber, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0114, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	SQLBindCol(hstmt,  1, SQL_C_LONG   , &retQuest.CharacterUniqueNumber, 0,	&arrCB[0]);
	SQLBindCol(hstmt,  2, SQL_C_LONG   , &retQuest.QuestIndex, 0,				&arrCB[1]);
	SQLBindCol(hstmt,  3, SQL_C_TINYINT, &retQuest.QuestState, 0,				&arrCB[2]);
	SQLBindCol(hstmt,  4, SQL_C_LONG   , &retQuest.QuestParam1, 0,				&arrCB[3]);
	SQLBindCol(hstmt,  5, SQL_C_TIMESTAMP, &QuestStartTime, 0,					&arrCB[4]);
	SQLBindCol(hstmt,  6, SQL_C_SBIGINT, &retQuest.QuestPlayTimeStamp, 0,		&arrCB[5]);

	// initialize to 0x00
	memset(&retQuest, 0x00, sizeof(CharacterQuest));
	memset(&QuestStartTime, 0, sizeof(SQL_TIMESTAMP_STRUCT));
	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA )
	{

		CQuest *pQuestInfo = m_pFieldIOCP8->m_mapQuestInfoFromScript.findEZ(retQuest.QuestIndex);
		if (NULL == pQuestInfo)
		{// ���� ����Ʈ ��ũ��Ʈ�� �������� �ʴ� ����Ʈ�ε����� �߰� ���� �ʴ´�
			continue;
		}
		retQuest.QuestTimeStamp = QuestStartTime;		// ����Ʈ ���� �ð� �Ҵ�
		tmFCharQuest.ResetFieldCharacterQuest(&retQuest, pQuestInfo);

		tmmapFCharacterQuest.insertNoLock(tmFCharQuest.QuestIndex, tmFCharQuest);
		
		memset(&retQuest, 0x00, sizeof(CharacterQuest));
		memset(&QuestStartTime, 0, sizeof(SQL_TIMESTAMP_STRUCT));
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	
	if(FALSE == pIOCPSocket->IsValidCharacter(FALSE)
		|| tmmapFCharacterQuest.empty())
	{// 2005-10-21 by cmkwon, �Ұ��� ����ǰų� �ε��� ����Ʈ�� ������ ����
		return;
	}
	
	///////////////////////////////////////////////////////////////////////////////
	// 2005-10-19 by cmkwon, ����Ʈ ���� ī��Ʈ 
	DBCharacterQuestMonsterCount	tmDBMonsterCount;
	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name:
	-- Desc:
	--====
	CREATE PROCEDURE atum_GetAllQuestMonsterCount
		@CharacterUniqueNumber		INT
	AS
		SELECT *
		FROM td_CharacterQuestMonsterCount
		WHERE CharacterUniqueNumber = @CharacterUniqueNumber
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pIOCPSocket->m_character.CharacterUniqueNumber, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0115, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	util::zero(arrCB, sizeof(arrCB[0])*6);
	SQLBindCol(hstmt,  1, SQL_C_LONG   , &tmDBMonsterCount.CharacterUniqueNumber, 0,	&arrCB[0]);
	SQLBindCol(hstmt,  2, SQL_C_LONG   , &tmDBMonsterCount.QuestIndex, 0,				&arrCB[1]);
	SQLBindCol(hstmt,  3, SQL_C_LONG   , &tmDBMonsterCount.MonsterUniqueNumber, 0,		&arrCB[2]);
	SQLBindCol(hstmt,  4, SQL_C_LONG   , &tmDBMonsterCount.Count, 0,					&arrCB[3]);

	util::zero(&tmDBMonsterCount, sizeof(DBCharacterQuestMonsterCount));
	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA )
	{
		CFieldCharacterQuest *pFCharQuest = tmmapFCharacterQuest.findLock_Ptr(tmDBMonsterCount.QuestIndex);
		if(NULL == pFCharQuest
			|| QUEST_STATE_IN_PROGRESS != pFCharQuest->QuestState)
		{
			// ����Ʈ ���� ���� ����
			QPARAM_DELETE_QUEST_MONSTER_COUNT *pDelMonCount = new QPARAM_DELETE_QUEST_MONSTER_COUNT;
			pDelMonCount->CharacterUniqueNumber		= tmDBMonsterCount.CharacterUniqueNumber;
			pDelMonCount->QuestIndex				= tmDBMonsterCount.QuestIndex;
			this->MakeAndEnqueueQuery(QT_DeleteQuestMonsterCount, pIOCPSocket, pIOCPSocket->GetCharacter()->AccountUniqueNumber, pDelMonCount);
			continue;
		}
		pFCharQuest->InsertMonsterCount(&tmDBMonsterCount);
		
		// �ʱ�ȭ
		util::zero(&tmDBMonsterCount, sizeof(DBCharacterQuestMonsterCount));
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	pIOCPSocket->OnGetAllQuest(&tmmapFCharacterQuest);
}

//	// Init MSG
//	INIT_MSG_WITH_BUFFER(MSG_FC_QUEST_PUT_ALL_QUEST, T_FC_QUEST_PUT_ALL_QUEST, pPutAllQuest,  pPutAllQuestBuf);
//	
//		if(MSG_SIZE(MSG_FC_QUEST_PUT_ALL_QUEST)+(nQuest+1)*sizeof(MEX_QUEST_INFO) >= SIZE_MAX_PACKET)
//		{
//			pPutAllQuest->NumOfQuest	= nQuest;
//			q.pFieldIOCPSocket->SendAddData(pPutAllQuestBuf, MSG_SIZE(MSG_FC_QUEST_PUT_ALL_QUEST) + nQuest*sizeof(MEX_QUEST_INFO));
//
//			pPutAllQuest->NumOfQuest	= 0;
//			nQuest						= 0;
//		}
//
//		CQuest *pQuestInfo = m_pFieldIOCP8->m_mapQuestInfoFromScript.findEZ(retQuest.QuestIndex);
//		if (NULL == pQuestInfo)
//		{
//			continue;
//		}
//
//		// �ð� �Ҵ�
//		retQuest.QuestTimeStamp = QuestStartTime;
//
//		// MSG�� ����
//		MEX_QUEST_INFO *pQuest4Exch		= (MEX_QUEST_INFO*)(pPutAllQuestBuf + MSG_SIZE(MSG_FC_QUEST_PUT_ALL_QUEST) + nQuest*sizeof(MEX_QUEST_INFO));
//		pQuest4Exch->QuestIndex			= retQuest.QuestIndex;
//		pQuest4Exch->QuestState			= retQuest.QuestState;
//		pQuest4Exch->QuestPlayTimeStamp	= retQuest.QuestPlayTimeStamp;
//		if(pQuestInfo->IsCityWar)
//		{
//			strncpy(pQuest4Exch->szCityWarServerGroupName, g_pFieldGlobal->GetServerGroupNameByMGameServerID(retQuest.QuestParam1), SIZE_MAX_SERVER_NAME);
//		}
//
//		// Quest ����Ʈ�� ����
//		q.pFieldIOCPSocket->m_mapQuest.insertLock(retQuest.QuestIndex, retQuest);
//		
//		nQuest++;		// count ����			
//
//		memset(&retQuest, 0x00, sizeof(CharacterQuest));
//		memset(&QuestStartTime, 0, sizeof(SQL_TIMESTAMP_STRUCT));
//	if (nQuest > 0)
//	{		
//		pPutAllQuest->NumOfQuest		= nQuest;		// ���� ����		
//		q.pFieldIOCPSocket->SendAddData(pPutAllQuestBuf, MSG_SIZE(MSG_FC_QUEST_PUT_ALL_QUEST) + nQuest*sizeof(MEX_QUEST_INFO));
//	}

// QT_InsertQuest
void CAtumFieldDBManager::QP_InsertQuest(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	CharacterQuest *pQuest = (CharacterQuest*)q.pQueryParam;
	CFieldIOCPSocket *pFieldIOCPSocket = q.pFieldIOCPSocket;

	// 2007-10-11 by cmkwon, CharacterUID ��ȿ�� üũ �߰�
	if(FALSE == IS_VALID_UNIQUE_NUMBER(pQuest->CharacterUniqueNumber)
		|| FALSE == pFieldIOCPSocket->IsValidCharacter(FALSE))			// 2007-11-13 by cmkwon, üũ �߰�
	{
		util::del(pQuest);
		return;
	}

	RETCODE ret;
	SDWORD pcbNTS = SQL_NTS;

	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name:
	-- Desc:
	--====
	--			// 2007-11-16 by cmkwon, ���̺� Unique Key ������ �� ������ �߰��� üũ �Ŀ� �߰� - td_characterQuest
	CREATE PROCEDURE dbo.atum_InsertQuest
		@i_CharacterUniqueNumber	INT,
		@i_QuestIndex				INT,
		@i_QuestState				TINYINT,
		@i_QuestParam1				INT,
		@i_QuestTimeStamp			VARCHAR(50),
		@i_QuestPlayTimeStamp		BIGINT
	AS
		DECLARE @QuestIdx INT
		SET @QuestIdx = (SELECT QuestIndex FROM td_CharacterQuest WHERE CharacterUniqueNumber = @i_CharacterUniqueNumber AND QuestIndex = @i_QuestIndex)
		IF @QuestIdx IS NULL
		BEGIN 
			INSERT INTO td_CharacterQuest
			VALUES (@i_CharacterUniqueNumber, @i_QuestIndex, @i_QuestState, @i_QuestParam1, @i_QuestTimeStamp, @i_QuestPlayTimeStamp)
		END
		ELSE
		BEGIN
			UPDATE td_CharacterQuest
			SET QuestState = @i_QuestState, QuestParam1 = @i_QuestParam1, QuestTimeStamp = @i_QuestTimeStamp, QuestPlayTimeStamp = @i_QuestPlayTimeStamp
			WHERE CharacterUniqueNumber = @i_CharacterUniqueNumber AND QuestIndex = @i_QuestIndex
		END
		
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQuest->CharacterUniqueNumber, 0, NULL);	// 2007-11-13 by cmkwon, ���� ����
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQuest->QuestIndex, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &pQuest->QuestState, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQuest->QuestParam1, 0, NULL);
	char tmpTimeString[SIZE_MAX_SQL_DATETIME_STRING];
	pQuest->QuestTimeStamp.GetSQLDateTimeString(tmpTimeString, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, &tmpTimeString, 0, &pcbNTS);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pQuest->QuestPlayTimeStamp, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0116, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_InsertQuest Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQuest);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQuest);
}

// QT_UpdateQuestState -> CAtumFieldDBManager::QP_UpdateQuestState()
void CAtumFieldDBManager::QP_UpdateQuestState(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	CFieldIOCPSocket *pFieldIOCPSocket = q.pFieldIOCPSocket;
	QPARAM_UPDATE_QUEST *pParamUpdateQuest = (QPARAM_UPDATE_QUEST*)q.pQueryParam;

	RETCODE ret;

	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name:
	-- Desc:
	--		2006-08-31 by cmkwon, @i_QuestPlayTimeStamp		BIGINT
	--====
	CREATE PROCEDURE atum_UpdateQuestState
		@CharacterUniqueNumber	INT,
		@QuestIndex				INT,
		@QuestState				TINYINT,
		@i_QuestPlayTimeStamp		BIGINT		-- 2006-08-31 by cmkwon, �߰���
	AS
		UPDATE td_CharacterQuest
		SET QuestState = @QuestState, QuestPlayTimeStamp = @i_QuestPlayTimeStamp
		WHERE CharacterUniqueNumber = @CharacterUniqueNumber AND QuestIndex = @QuestIndex
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pParamUpdateQuest->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pParamUpdateQuest->QuestIndex, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &pParamUpdateQuest->QuestState, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pParamUpdateQuest->QuestProgressTimeInSecond, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0117, SQL_NTS);

	if(ret == SQL_NO_DATA)
	{
		///////////////////////////////////////////////////////////////////////////
		// �������� �������� ����
		if(pFieldIOCPSocket)
		{
			pFieldIOCPSocket->SendErrorMessageFromFieldDB(QT_UpdateQuestState, ERR_PROTOCOL_NO_SUCH_CHARACTER_QUEST);
		}
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pParamUpdateQuest);
		return;
	}

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO ) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateQuestState Failed!\n", TRUE, pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pParamUpdateQuest);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pParamUpdateQuest);
}

// QT_DeleteQuest -> CAtumFieldDBManager::QP_DeleteQuest()
void CAtumFieldDBManager::QP_DeleteQuest(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	CFieldIOCPSocket *pFieldIOCPSocket = q.pFieldIOCPSocket;
	QPARAM_DELETE_QUEST *pParamDeleteQuest = (QPARAM_DELETE_QUEST*)q.pQueryParam;

	RETCODE ret;

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_DeleteQuest
		@CharacterUniqueNumber	INT,
		@QuestIndex				INT
	AS
		DELETE FROM td_CharacterQuest
		WHERE CharacterUniqueNumber = @CharacterUniqueNumber AND QuestIndex = @QuestIndex
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pParamDeleteQuest->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pParamDeleteQuest->QuestIndex, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0118, SQL_NTS);
	if(ret == SQL_NO_DATA)
	{
		///////////////////////////////////////////////////////////////////////////
		// �������� �������� ����
		if(pFieldIOCPSocket)
		{
			pFieldIOCPSocket->SendErrorMessageFromFieldDB(QT_DeleteQuest, ERR_PROTOCOL_NO_SUCH_CHARACTER_QUEST);
		}
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pParamDeleteQuest);
		return;
	}

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO ) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_DeleteQuest Failed!\n", TRUE, pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pParamDeleteQuest);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pParamDeleteQuest);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_UpdateQuestMonsterCount(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		���� ī��Ʈ �߰� �� ����
/// \author		cmkwon
/// \date		2005-10-25 ~ 2005-10-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_UpdateQuestMonsterCount(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	CFieldIOCPSocket *pFieldIOCPSocket = q.pFieldIOCPSocket;
	QPARAM_UPDATE_QUEST_MONSTER_COUNT *pUpdateMonCount = (QPARAM_UPDATE_QUEST_MONSTER_COUNT*)q.pQueryParam;

	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name:
	-- Desc:����Ʈ ���� ī��Ʈ�� �߰� Ȥ�� ����
	--====
	CREATE PROCEDURE atum_UpdateQuestMonsterCount
		@CharacterUniqueNumber		INT,
		@QuestIndex					INT,
		@MonsterUniqueNumber		INT,
		@Count
	AS
		DECLARE @nCurCount INT
		SET @nCurCount = (SELECT top 1 Count FROM td_CharacterQuestMonsterCount 
							WHERE CharacterUniqueNumber = @CharacterUniqueNumber AND QuestIndex = @QuestIndex AND MonsterUniqueNumber = @MonsterUniqueNumber)
		IF (@nCurCount IS NULL)
			BEGIN
				INSERT INTO td_CharacterQuestMonsterCount
				VALUES (@CharacterUniqueNumber, @QuestIndex, @MonsterUniqueNumber, @Count)
			END
		ELSE IF (@Count <= @nCurCount)
			BEGIN
				UPDATE td_CharacterQuestMonsterCount
				SET Count = @nCurCount + 1
				WHERE CharacterUniqueNumber = @CharacterUniqueNumber AND QuestIndex = @QuestIndex AND MonsterUniqueNumber = @MonsterUniqueNumber
			END
		ELSE
			BEGIN
				UPDATE td_CharacterQuestMonsterCount
				SET Count = @Count
				WHERE CharacterUniqueNumber = @CharacterUniqueNumber AND QuestIndex = @QuestIndex AND MonsterUniqueNumber = @MonsterUniqueNumber
			END
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pUpdateMonCount->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pUpdateMonCount->QuestIndex, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pUpdateMonCount->MonsterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pUpdateMonCount->Count, 0, NULL);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0119, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateQuestMonsterCount Failed!\n", TRUE, pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pUpdateMonCount);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pUpdateMonCount);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_DeleteQuestMonsterCount(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		���� ī��Ʈ �����
/// \author		cmkwon
/// \date		2005-10-25 ~ 2005-10-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_DeleteQuestMonsterCount(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	CFieldIOCPSocket *pFieldIOCPSocket = q.pFieldIOCPSocket;
	QPARAM_DELETE_QUEST_MONSTER_COUNT *pDeleteMonCount = (QPARAM_DELETE_QUEST_MONSTER_COUNT*)q.pQueryParam;
	
	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name:
	-- Desc:����Ʈ ���� ī��Ʈ ����
	--====
	CREATE PROCEDURE atum_DeleteQuestMonsterCount
		@CharacterUniqueNumber		INT,
		@QuestIndex					INT
	AS
		DELETE FROM td_CharacterQuestMonsterCount
		WHERE CharacterUniqueNumber = @CharacterUniqueNumber AND QuestIndex = @QuestIndex
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pDeleteMonCount->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pDeleteMonCount->QuestIndex, 0, NULL);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0120, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_NO_DATA)	// 2007-08-01 by cmkwon, �߰���(&& ret!=SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_DeleteQuestMonsterCount Failed!\n", TRUE, pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pDeleteMonCount);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pDeleteMonCount);
}



// QT_AuctionGetItemList -> CAtumFieldDBManager::QP_AuctionGetItemList()
void CAtumFieldDBManager::QP_AuctionGetItemList(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
//	BYTE	nAuctionKind	= q.nGeneralParam1;
//	UID32_t	nCharacterUID	= q.nGeneralParam2;
//
//	RETCODE		ret;
//
//	// �˻�
//	if (!q.pFieldIOCPSocket->IsUsing()
//		|| q.pFieldIOCPSocket->GetClientState() != CS_PLAYING)
//	{
//		return;
//	}
//
//	if (nAuctionKind == AUCTION_KIND_MY_ITEM)
//	{
//		// �ڽ��� ����� �������� ��û�ϴ� ���
//
//		/*[Stored Query Definition]************************************************
//		CREATE PROCEDURE atum_AuctionGetMyItem
//			@i_CharacterUID			INT			-- ���� ĳ���� �ε���, td_Store.Possess
//		AS
//			SELECT *
//			FROM td_Auction a
//			WHERE a.AuctionCharacterUID = @i_CharacterUID
//			
//			SELECT e.*
//			FROM td_Auction a, td_Enchant e
//			WHERE a.AuctionCharacterUID = @i_CharacterUID AND e.TargetItemUniqueNumber = a.AuctionItemUID
//		GO
//		**************************************************************************/
//		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &nCharacterUID, 0, NULL);
//
//		ret = SQLExecDirect(hstmt, (UCHAR*)"{call dbo.atum_AuctionGetMyItem(?)}", SQL_NTS);
//
//		if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
//		{
//			ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "atum_AuctionGetMyItem Failed!\n", TRUE, q.pFieldIOCPSocket, q);
//			SQLFreeStmt(hstmt, SQL_CLOSE);
//			return;
//		}
//	}
//	else
//	{
//		// ������ ������ �з��� ��ϵ� �������� ��û�ϴ� ���
//
//		/*[Stored Query Definition]************************************************
//		CREATE PROCEDURE atum_AuctionGetKindItem
//			@i_AuctionItemKind		TINYINT		-- ������ ����, AtumProtocol.h�� AUCTION_KIND_XXX ����
//		AS
//			SELECT *
//			FROM td_Auction a
//			WHERE a.AuctionItemKind = @i_AuctionItemKind
//
//			SELECT e.*
//			FROM td_Auction a, td_Enchant e
//			WHERE a.AuctionItemKind = @i_AuctionItemKind AND e.TargetItemUniqueNumber = a.AuctionItemUID
//		GO
//		**************************************************************************/
//		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &nAuctionKind, 0, NULL);
//
//		ret = SQLExecDirect(hstmt, (UCHAR*)"{call dbo.atum_AuctionGetKindItem(?)}", SQL_NTS);
//
//		if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
//		{
//			ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "atum_AuctionGetKindItem Failed!\n", TRUE, q.pFieldIOCPSocket, q);
//			SQLFreeStmt(hstmt, SQL_CLOSE);
//			return;
//		}
//	}
//
//	SQLINTEGER				cb1, cb2, cb3, cb4, cb5, cb6, cb7, cb8, cb9, cb10, cb11, cb12, cb13, cb14;
//	BYTE					AuctionKind;
//	INT						AuctionPrice;
//	SQL_TIMESTAMP_STRUCT	AuctionStartTime; memset(&AuctionStartTime, 0, sizeof(SQL_TIMESTAMP_STRUCT));
//	BYTE					AuctionState;					
//	char					SellerName[SIZE_MAX_CHARACTER_NAME];
//	ITEM_GENERAL			ItemGeneral(NULL);
//
//	// AUCTION binding
//	SQLBindCol(hstmt, 1, SQL_C_TINYINT, &AuctionKind, 0, &cb1);
//	SQLBindCol(hstmt, 2, SQL_C_LONG, &AuctionPrice, 0, &cb2);
//	SQLBindCol(hstmt, 3, SQL_C_TIMESTAMP, &AuctionStartTime, 0, &cb3);
//	SQLBindCol(hstmt, 4, SQL_C_TINYINT, &AuctionState, 0, &cb4);
//	cb5 = SQL_NTS;
//	SQLBindCol(hstmt, 5, SQL_C_CHAR, SellerName, SIZE_MAX_CHARACTER_NAME, &cb5);
//	// ITEM_GENERAL binding
//	SQLBindCol(hstmt, 6, SQL_C_SLONG, &ItemGeneral.ItemNum, 0, &cb6);
//	SQLBindCol(hstmt, 7, SQL_C_SBIGINT, &ItemGeneral.UniqueNumber, 0, &cb7);
//	SQLBindCol(hstmt, 8, SQL_C_ULONG, &ItemGeneral.AccountUniqueNumber, 0, &cb8);
//	SQLBindCol(hstmt, 9, SQL_C_ULONG, &ItemGeneral.Possess, 0, &cb9);
//	SQLBindCol(hstmt, 10, SQL_C_SLONG, &ItemGeneral.CurrentCount, 0, &cb10);
//	SQLBindCol(hstmt, 11, SQL_C_SSHORT, &ItemGeneral.NumOfEnchants, 0, &cb11);
//	SQLBindCol(hstmt, 12, SQL_C_LONG, &ItemGeneral.PrefixCodeNum, 0, &cb12);
//	SQLBindCol(hstmt, 13, SQL_C_LONG, &ItemGeneral.SuffixCodeNum, 0, &cb13);
//	SQLBindCol(hstmt, 14, SQL_C_FLOAT, &ItemGeneral.CurrentEndurance, 0, &cb14);
//
//	// Init MSG
//	INIT_MSG_OF_SIZE(MSG_FC_AUCTION_INSERT_ITEM, T_FC_AUCTION_INSERT_ITEM, pAuctionInsertItem,  pAuctionInsertItemBuf);
//
//	// count ����
//	int nAuctionItemCount = 0;
//
//	// initialize to 0x00
//	memset(&ItemGeneral, 0x00, sizeof(ITEM_GENERAL));
//	memset(&AuctionStartTime, 0, sizeof(SQL_TIMESTAMP_STRUCT));
//	AuctionPrice = 0;
//	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA )
//	{
//		pAuctionInsertItem->AuctionKind				= nAuctionKind;
//		strncpy(pAuctionInsertItem->SellerCharacterName, SellerName, SIZE_MAX_CHARACTER_NAME);
//		pAuctionInsertItem->AuctionItemGeneral		= ItemGeneral;
//		pAuctionInsertItem->AuctionPrice			= AuctionPrice;
//		pAuctionInsertItem->AuctionStartDate		= AuctionStartTime;
//
//		// send msg
//		q.pFieldIOCPSocket->SendAddData(pAuctionInsertItemBuf, MSG_SIZE(MSG_FC_AUCTION_INSERT_ITEM));
//
//		// count ����
//		nAuctionItemCount++;
//
//		// initialize to 0x00
//		memset(&ItemGeneral, 0x00, sizeof(ITEM_GENERAL));
//		memset(&AuctionStartTime, 0, sizeof(SQL_TIMESTAMP_STRUCT));
//		AuctionPrice = 0;
//	}
//
//	///////////////////////////////////////////////////////////////////////////
//	// ��æƮ �ε� �κ����� �Ѿ
//	ret = SQLMoreResults(hstmt);
//	// ��æƮ �ε� �κ����� �Ѿ
//	///////////////////////////////////////////////////////////////////////////
//
//	if (ret == SQL_NO_DATA)
//	{
//		SQLFreeStmt(hstmt, SQL_CLOSE);
//		return;
//	}
//	else if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
//	{
//		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_LoadOneItem 2 Failed!\n", TRUE, q.pFieldIOCPSocket, q);
//		SQLFreeStmt(hstmt, SQL_CLOSE);
//		return;
//	}
//
//	ENCHANT tmpEnchant;
//
//	SQLBindCol(hstmt, 1, SQL_C_SBIGINT, &tmpEnchant.TargetItemUniqueNumber, 0, &cb1);
//	SQLBindCol(hstmt, 2, SQL_C_ULONG, &tmpEnchant.TargetItemNum, 0, &cb2);
//	SQLBindCol(hstmt, 3, SQL_C_ULONG, &tmpEnchant.EnchantItemNum, 0, &cb3);
//
//	// Init MSG
//	INIT_MSG_OF_SIZE(MSG_FC_AUCTION_PUT_ENCHANT, T_FC_AUCTION_PUT_ENCHANT, pAuctionPutEnchant, pAuctionPutEnchantBuf);
//	pAuctionPutEnchant->AuctionKind = nAuctionKind;
//
//	// initialize to 0x00
//	memset(&tmpEnchant, 0x00, sizeof(ENCHANT));
//	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA )
//	{
//		// Send MSG
//		pAuctionPutEnchant->AuctionItemEnchant = tmpEnchant;
//		q.pFieldIOCPSocket->SendAddData(pAuctionPutEnchantBuf, MSG_SIZE(MSG_FC_AUCTION_PUT_ENCHANT));
//
//		// initialize to 0x00
//		memset(&tmpEnchant, 0x00, sizeof(ENCHANT));
//	}	// end of while(fetch)
//
//	// clean up
//	SQLFreeStmt(hstmt, SQL_CLOSE);
}

// QT_AuctionRegisterItem -> CAtumFieldDBManager::QP_AuctionRegisterItem()
void CAtumFieldDBManager::QP_AuctionRegisterItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
//	UID64_t	nItemUID	= q.nGeneralParam1;
//	INT		nPrice		= q.nGeneralParam2;
//
//	if (q.pFieldIOCPSocket == NULL || !q.pFieldIOCPSocket->IsUsing()
//		|| q.pFieldIOCPSocket->m_pCurrentBuildingNPC == NULL
//		|| q.pFieldIOCPSocket->m_pCurrentBuildingNPC->BuildingKind != BUILDINGKIND_AUCTION)
//	{
//		return;
//	}
//
//	// lock m_mapItemGeneral, @QP_GetStoreItem
//	mt_auto_lock igLock(&q.pFieldIOCPSocket->m_ItemManager.m_mapItemGeneral);
//
//	ITEM_GENERAL *pItemGeneralForAuction
//		= q.pFieldIOCPSocket->m_ItemManager.GetItemGeneralByUID(nItemUID);
//	if (pItemGeneralForAuction == NULL)
//	{
//		q.pFieldIOCPSocket->SendErrorMessage(T_FC_AUCTION_REGISTER_ITEM, ERR_PROTOCOL_NO_SUCH_ITEM, nItemUID);
//		return;
//	}
//
//	// Countable Item�� ������, 20041210, kelovon with sjmin
//	if (IS_COUNTABLE_ITEM(pItemGeneralForAuction->Kind))
//	{
//		q.pFieldIOCPSocket->SendString128(STRING_128_USER_ERR, "����� �� ���� ������ �����Դϴ�.");
//		return;
//	}
//
//	BYTE	nAuctionKind;
//	if (IS_AUCTION_KIND_ATTACH(pItemGeneralForAuction->Kind))
//	{
//		nAuctionKind = AUCTION_KIND_ATTACH;
//	}
//	else if (IS_AUCTION_KIND_CARD(pItemGeneralForAuction->Kind))
//	{
//		nAuctionKind = AUCTION_KIND_CARD;
//	}
//	else if (IS_AUCTION_KIND_ETC(pItemGeneralForAuction->Kind))
//	{
//		nAuctionKind = AUCTION_KIND_ETC;
//	}
//	else
//	{
//		// no such auction kind
//		return;
//	}
//
//	SQLRETURN ret;
//
//	///////////////////////////////////////////
//	// 5�� �̻� ��ϵǾ����� Ȯ��
//	///////////////////////////////////////////
//
//	/*[Stored Query Definition]************************************************
//	CREATE PROCEDURE atum_AuctionGetRegisterCount
//		@i_CharacterUID			INT
//	AS
//		SELECT count(*)
//		FROM td_Auction
//		WHERE AuctionCharacterUID = @i_CharacterUID
//	GO
//	**************************************************************************/
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &q.pFieldIOCPSocket->m_character.CharacterUniqueNumber, 0, NULL);
//
//	ret = SQLExecDirect(hstmt, (UCHAR*)"{call dbo.atum_AuctionGetRegisterCount(?)}", SQL_NTS);
//
//	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
//	{
//		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "atum_AuctionGetRegisterCount Failed!\n", TRUE, q.pFieldIOCPSocket, q);
//		SQLFreeStmt(hstmt, SQL_CLOSE);
//		return;
//	}
//
//	SQLINTEGER cb1;
//	int RegisteredItemCount = 0;
//
//	SQLBindCol(hstmt, 1, SQL_C_LONG, &RegisteredItemCount, 0, &cb1);
//
//	while ((ret = SQLFetch(hstmt)) != SQL_NO_DATA)
//	{
//		if (RegisteredItemCount >= 5)
//		{
//			q.pFieldIOCPSocket->SendString128(STRING_128_USER_ERR, "5�� ������ ����� �� �ֽ��ϴ�.");
//			SQLFreeStmt(hstmt, SQL_CLOSE);
//			return;
//		}
//	}
//
//	// clean up
//	SQLFreeStmt(hstmt, SQL_CLOSE);
//
//
//	///////////////////////////////////////////
//	// ������ ��� ���
//	///////////////////////////////////////////
//
//	/*[Stored Query Definition]************************************************
//	CREATE PROCEDURE atum_AuctionRegisterItem
//		@i_AuctionItemKind				TINYINT,	-- ������ ����, AtumProtocol.h�� AUCTION_KIND_XXX ����
//		@i_AuctionPrice					INT,		-- ��Ű�
//		@i_AuctionItemUID				BIGINT,		-- ������ ���� ��ȣ, td_Store.UniqueNumber
//		@i_AuctionAccountUID			INT,		-- ���� Account �ε���, td_Store.AccountUniqueNumber
//		@i_AuctionCharacterUID			INT,		-- ���� ĳ���� �ε���, td_Store.Possess
//		@i_SellerName					VARCHAR(20)
//	AS
//		INSERT INTO td_Auction
//			SELECT @i_AuctionItemKind, @i_AuctionPrice, GetDate(), 0, @i_SellerName,
//					ItemNum, UniqueNumber, AccountUniqueNumber, Possess,
//					CurrentCount, NumOfEnchants, PrefixCodeNum, SuffixCodeNum, CurrentEndurance
//			FROM td_Store
//			WHERE UniqueNumber = @i_AuctionItemUID AND AccountUniqueNumber = @i_AuctionAccountUID AND Possess = @i_AuctionCharacterUID
//
//		UPDATE td_Store
//		SET Possess = -1
//		WHERE UniqueNumber = @i_AuctionItemUID AND AccountUniqueNumber = @i_AuctionAccountUID AND Possess = @i_AuctionCharacterUID
//	GO
//	**************************************************************************/
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &nAuctionKind, 0, NULL);
//	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nPrice, 0, NULL);
//	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pItemGeneralForAuction->UniqueNumber, 0, NULL);
//	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItemGeneralForAuction->AccountUniqueNumber, 0, NULL);
//	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItemGeneralForAuction->Possess, 0, NULL);
//	SQLINTEGER pcb = SQL_NTS;
//	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, &q.pFieldIOCPSocket->m_character.CharacterName, 0, &pcb);
//
//	ret = SQLExecDirect(hstmt, (UCHAR*)"{call dbo.atum_AuctionRegisterItem(?, ?, ?, ?, ?, ?)}", SQL_NTS);
//
//	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
//	{
//		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "atum_AuctionRegisterItem Failed!\n", TRUE, q.pFieldIOCPSocket, q);
//		SQLFreeStmt(hstmt, SQL_CLOSE);
//		return;
//	}
//
//	// send msg
//	INIT_MSG_OF_SIZE(MSG_FC_AUCTION_REGISTER_ITEM_OK, T_FC_AUCTION_REGISTER_ITEM_OK, pMsgRegOK, pMsgRegOKBuf);
//	pMsgRegOK->ItemUID	= nItemUID;
//	pMsgRegOK->Price	= nPrice;
//	q.pFieldIOCPSocket->SendAddData(pMsgRegOKBuf, MSG_SIZE(MSG_FC_AUCTION_REGISTER_ITEM_OK));
//
//	// ������ ����Ʈ���� ���� + �޸� ����
//	q.pFieldIOCPSocket->m_ItemManager.m_mapItemGeneral.deleteLock(pItemGeneralForAuction->UniqueNumber);
//	util::del(pItemGeneralForAuction);
//
//	// enchant reloading
//	MakeAndEnqueueQuery(QT_ReloadAllEnchant, q.pFieldIOCPSocket, q.pFieldIOCPSocket->m_character.AccountUniqueNumber
//		, NULL, NULL, q.pFieldIOCPSocket->m_character.AccountUniqueNumber, q.pFieldIOCPSocket->m_character.CharacterUniqueNumber);
//
//	// clean up
//	SQLFreeStmt(hstmt, SQL_CLOSE);
}

// QT_AuctionCancelRegister -> CAtumFieldDBManager::QP_AuctionCancelRegister()
void CAtumFieldDBManager::QP_AuctionCancelRegister(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	UID64_t	nItemUID = q.nGeneralParam1;

	if (q.pFieldIOCPSocket == NULL || !q.pFieldIOCPSocket->IsUsing()
		|| q.pFieldIOCPSocket->m_pCurrentBuildingNPC == NULL
		|| q.pFieldIOCPSocket->m_pCurrentBuildingNPC->BuildingKind != BUILDINGKIND_AUCTION)
	{
		return;
	}

	SQLRETURN ret;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_AuctionCancelItem
		@i_ItemUID				BIGINT,		-- ������ ���� ��ȣ, td_Store.UniqueNumber
		@i_AccountUID			INT,		-- ���� Account �ε���, td_Store.AccountUniqueNumber
		@i_CharacterUID			INT			-- ���� ĳ���� �ε���, td_Store.Possess
	AS
		UPDATE td_Store
		SET Possess = @i_CharacterUID
		WHERE UniqueNumber = @i_ItemUID AND AccountUniqueNumber = @i_AccountUID AND Possess = -1

		DELETE FROM td_Auction
		WHERE AuctionItemUID = @i_ItemUID AND AuctionAccountUID = @i_AccountUID AND AuctionCharacterUID = @i_CharacterUID
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &nItemUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &q.pFieldIOCPSocket->m_character.AccountUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &q.pFieldIOCPSocket->m_character.CharacterUniqueNumber, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0121, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "atum_AuctionCancelItem Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	if (q.pFieldIOCPSocket->IsUsing() && q.pFieldIOCPSocket->GetClientState() == CS_PLAYING)
	{
		INIT_MSG_OF_SIZE(MSG_FC_AUCTION_CANCEL_REGISTER_OK, T_FC_AUCTION_CANCEL_REGISTER_OK, pCancelRegOK, pCancelRegOKBuf);
		pCancelRegOK->ItemUID = nItemUID;
		q.pFieldIOCPSocket->SendAddData(pCancelRegOKBuf, MSG_SIZE(MSG_FC_AUCTION_CANCEL_REGISTER_OK));

		// �ش� ������ �ε�
		this->MakeAndEnqueueQuery(QT_LoadOneItem, q.pFieldIOCPSocket, q.pFieldIOCPSocket->m_character.AccountUniqueNumber, NULL, NULL, nItemUID);
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
}

// QT_AuctionBuyItem -> CAtumFieldDBManager::QP_AuctionBuyItem()
void CAtumFieldDBManager::QP_AuctionBuyItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
// 2008-07-08 by cmkwon, ������� ����
// 	SQLRETURN	ret;
// 	INT			nItemPrice	= 0;
// 	UID32_t		nSellerAccountUID	= 0;
// 	UID32_t		nSellerCharacterUID	= 0;
// 	UID64_t		nItemUID	= q.nGeneralParam1;
// 
// 	if (q.pFieldIOCPSocket == NULL || !q.pFieldIOCPSocket->IsUsing()
// 		|| q.pFieldIOCPSocket->m_pCurrentBuildingNPC == NULL
// 		|| q.pFieldIOCPSocket->m_pCurrentBuildingNPC->BuildingKind != BUILDINGKIND_AUCTION)
// 	{
// 		return;
// 	}
// 
// 	// ??? ??? ? ?? ??
// 	char szQuery[SIZE_MAX_SQL_QUERY_STRING];
// 	// 2008-07-02 by cmkwon, MySQL ?? ?? - 
// 	//sprintf(szQuery, "SELECT AuctionPrice, AuctionAccountUID, AuctionCharacterUID FROM td_Auction WITH (NOLOCK) WHERE AuctionItemUID = %d", nItemUID);
// 	sprintf(szQuery, QUERY_080702_0245, nItemUID);
// 
// 	ret = SQLExecDirect(hstmt, (UCHAR*)szQuery, SQL_NTS);
// 
// 	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
// 	{
// 		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "GetAuctionItem Failed!\n", TRUE, q.pFieldIOCPSocket, q);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		return;
// 	}
// 
// 	SQLBindCol(hstmt, 1, SQL_C_LONG, &nItemPrice, 0, NULL);
// 	SQLBindCol(hstmt, 2, SQL_C_UID32, &nSellerAccountUID, 0, NULL);
// 	SQLBindCol(hstmt, 3, SQL_C_UID32, &nSellerCharacterUID, 0, NULL);
// 
// 	ret = SQLFetch(hstmt);
// 	if (ret == SQL_NO_DATA)
// 	{
// 		q.pFieldIOCPSocket->SendString128(STRING_128_USER_ERR, STRMSG_S_F2DBQUERY_0005);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		return;
// 	}
// 
// 	// clean up
// 	SQLFreeStmt(hstmt, SQL_CLOSE);
// 
// 	// lock m_mapItemGeneral, @QP_AuctionBuyItem
// 	mt_auto_lock igLock(&q.pFieldIOCPSocket->m_ItemManager.m_mapItemGeneral);
// 
// 	ITEM_GENERAL *pMoneyItemGeneral = q.pFieldIOCPSocket->m_ItemManager.GetFirstItemGeneralByItemNum(MONEY_ITEM_NUMBER);
// 	if (pMoneyItemGeneral == NULL || pMoneyItemGeneral->CurrentCount < nItemPrice)
// 	{// ?? ???
// 		if(NULL == pMoneyItemGeneral)
// 		{// 2006-10-16 by cmkwon, SPI ???? ?? ?? ??
// 			q.pFieldIOCPSocket->m_ItemManager.InsertItemBaseByItemNum(MONEY_ITEM_NUMBER, 0, IUT_GENERAL);
// 		}
// 		q.pFieldIOCPSocket->SendErrorMessage(T_FC_AUCTION_BUY_ITEM, ERR_PROTOCOL_NOT_ENOUGH_MONEY);
// 		return;
// 	}
// 
// 	/*[Stored Query Definition]************************************************
// 	CREATE PROCEDURE atum_AuctionBuyItem
// 		@i_AuctionItemUID					BIGINT,	-- ???
// 		@i_AuctionSellerAccountUID			INT,	-- ???? Account UID
// 		@i_AuctionSellerCharacterUID		INT,	-- ???? Character UID
// 		@i_AuctionBuyerAccountUID			INT,	-- ???? Account UID
// 		@i_AuctionBuyerCharacterUID			INT		-- ???? Character UID
// 	AS
// 		DECLARE @tmpItemPrice	INT
// 		SET @tmpItemPrice = (SELECT TOP 1 AuctionPrice FROM td_Auction WHERE AuctionItemUID = @i_AuctionItemUID)
// 
// 		-- ???? ??? return
// 		IF (@tmpItemPrice IS NULL) RETURN 0
// 
// 		-- ?? ??? ??
// 		DELETE FROM td_Auction WHERE AuctionItemUID = @i_AuctionItemUID
// 
// 		-- ???? ??? ??
// 		UPDATE td_Store
// 		SET Possess = @i_AuctionBuyerCharacterUID, AccountUniqueNumber = @i_AuctionBuyerAccountUID
// 		WHERE UniqueNumber = @i_AuctionItemUID
// 
// 		-- ???? ? ????
// 		IF (SELECT count(*) FROM td_Store WHERE ItemNum = 7000022 AND Possess = 0 AND AccountUniqueNumber = @i_AuctionSellerAccountUID) >= 1
// 			BEGIN
// 				-- ??? ? ???? ?? ??
// 				UPDATE td_Store
// 				SET CurrentCount = CurrentCount + @tmpItemPrice
// 				WHERE ItemNum = 7000022 AND Possess = 0 AND AccountUniqueNumber = @i_AuctionSellerAccountUID
// 			END
// 		ELSE
// 			BEGIN
// 				-- ??? ? ???? ?? ??
// 				EXEC atum_admin_InsertStoreItem_DB_by_CharacUniqueNum i_SellerAccountUID, 7000022, @tmpItemPrice
// 			END
// 
// 		RETURN 1
// 	GO
// 	**************************************************************************/
// 	int nReturnValue = 999;
// 	SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nReturnValue, 0, NULL);
// 	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &nItemUID, 0, NULL);
// 	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_UID32, SQL_INTEGER, 0, 0, &nSellerAccountUID, 0, NULL);
// 	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_UID32, SQL_INTEGER, 0, 0, &nSellerCharacterUID, 0, NULL);
// 	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_UID32, SQL_INTEGER, 0, 0, &q.pFieldIOCPSocket->m_character.AccountUniqueNumber, 0, NULL);
// 	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_UID32, SQL_INTEGER, 0, 0, &q.pFieldIOCPSocket->m_character.CharacterUniqueNumber, 0, NULL);
// 
// 	ret = SQLExecDirect(hstmt, (UCHAR*)"{ ? = call atum_AuctionBuyItem(?, ?, ?, ?, ?)}", SQL_NTS);
// 
// 	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
// 	{
// 		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "atum_AuctionBuyItem Failed!\n", TRUE, q.pFieldIOCPSocket, q);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		return;
// 	}
// 
// 	// Clear any result sets generated.
// 	while ( ( ret = SQLMoreResults(hstmt) ) != SQL_NO_DATA );
// 
// 	if (nReturnValue != 1)
// 	{
// 		// ?? ??
// 		q.pFieldIOCPSocket->SendString128(STRING_128_USER_ERR, STRMSG_S_F2DBQUERY_0006);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		return;
// 	}
// 
// 	// ? ????
// 	q.pFieldIOCPSocket->m_ItemManager.UpdateItemCountByPointer(pMoneyItemGeneral, -nItemPrice, IUT_AUCTION);
// 
// 	if (q.pFieldIOCPSocket->IsUsing() && q.pFieldIOCPSocket->GetClientState() == CS_PLAYING)
// 	{
// 		// send msg
// 		INIT_MSG_OF_SIZE(MSG_FC_AUCTION_BUY_ITEM_OK, T_FC_AUCTION_BUY_ITEM_OK, pAuctionBuyOK, pAuctionBuyOKBuf);
// 		pAuctionBuyOK->ItemUID = nItemUID;
// 		q.pFieldIOCPSocket->SendAddData(pAuctionBuyOKBuf, MSG_SIZE(MSG_FC_AUCTION_BUY_ITEM_OK));
// 
// 		// ?? ??? ??
// 		this->MakeAndEnqueueQuery(QT_LoadOneItem, q.pFieldIOCPSocket, q.pFieldIOCPSocket->m_character.AccountUniqueNumber, NULL, NULL, nItemUID);
// 	}
// 
// 	// clean up
// 	SQLFreeStmt(hstmt, SQL_CLOSE);
// 	return;
}

void CAtumFieldDBManager::QP_GeneralExec(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	CFieldIOCPSocket *pFieldIOCPSocket = q.pFieldIOCPSocket;
	QPARAM_GENERAL_EXEC *pParamGeneralExec = (QPARAM_GENERAL_EXEC*)q.pQueryParam;
	char *pQueryString = pParamGeneralExec->QueryString;

	RETCODE ret;

	ret = SQLExecDirect(hstmt, (UCHAR*)pQueryString, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO ) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"QP_GeneralExec Failed!\n", TRUE, pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pParamGeneralExec);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pParamGeneralExec);
}

void CAtumFieldDBManager::ProcessLogMessagesField(SQLSMALLINT plm_handle_type,
		SQLHANDLE &plm_handle, char *logstring
		, BOOL ConnInd, CFieldIOCPSocket *pIOCPSocket
		, FIELD_DB_QUERY q)
{
	if(pIOCPSocket)
	{
		g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] === %s's ProcessLogMessagesField_ =====================\r\n"
			, GetCharacterString(&pIOCPSocket->m_character, string()));
	}
	switch(q.enumQuryType)
	{
	case QT_InsertStoreItem:
		{
			ITEM_GENERAL		*pItem = (ITEM_GENERAL*)q.pQueryParam;
			g_pGlobal->WriteSystemLogEX(TRUE, "  %20s, AccountUID(%8d) Possess(%8d) ItemNum(%8d) CurrentCount(%4d) ItemStorage(%d)\r\n"
				, GetDBQueryTypeString(q.enumQuryType), pItem->AccountUniqueNumber
				, pItem->Possess, pItem->ItemNum, pItem->CurrentCount, pItem->ItemStorage);
		}		
		break;
	case QT_InsertStoreItemSkill:
		{
			ITEM_SKILL			*pItemSkill = (ITEM_SKILL*)q.pQueryParam;
			g_pGlobal->WriteSystemLogEX(TRUE, "  %20s, AccountUID(%8d) Possess(%8d) ItemNum(%8d) ItemWindowIndex(%4d)\r\n"
				, GetDBQueryTypeString(q.enumQuryType), pItemSkill->AccountUniqueNumber
				, pItemSkill->Possess, pItemSkill->ItemNum, pItemSkill->ItemWindowIndex);
		}
		break;
	}

	CAtumDBManager::ProcessLogMessages(plm_handle_type, plm_handle, logstring, ConnInd, pIOCPSocket);
}




void CAtumFieldDBManager::QP_CashInsertPremiumCard(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_CASH_INSERT_PREMIUM_CARD *pQPMsg = (QPARAM_CASH_INSERT_PREMIUM_CARD*)q.pQueryParam;
	/*******************************************************************************
	-- 2005-01-20 by cmkwon, Cash ī�带 �߰�
	CREATE PROCEDURE dbo.atum_insert_AccountCashStoreItem
		@i_accNum			INT,				-- AccountUniqueNumber
		@i_ItemNum			INT,				-- �߰� �� ItemNum
		@i_CurTime			VARCHAR(30),		-- ���� �ð�
		@i_ExpireTime		VARCHAR(30)			-- ��� ���� �ð�
	AS
		DECLARE @RetUniqueNum	BIGINT
		SET @RetUniqueNum = (SELECT uniqueNumber FROM td_accountCashStore WHERE AccountUniqueNumber = @i_accNum)
		IF (@RetUniqueNum IS NOT NULL)
		BEGIN
			-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
			--RETURN 0
			SELECT 0;
			RETURN;
		END
		
		INSERT INTO td_accountCashStore
			VALUES (@i_accNum, @i_ItemNum, @i_CurTime, @i_CurTime, @i_ExpireTime)
		
		SET @RetUniqueNum = (SELECT uniqueNumber FROM td_accountCashStore WHERE AccountUniqueNumber = @i_accNum)
		IF (@RetUniqueNum IS NULL)
		BEGIN
			-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
			--RETURN 0
			SELECT 0;
			RETURN;
		END
		
		-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
		--RETURN @RetUniqueNum
		SELECT @RetUniqueNum;
	GO
	*******************************************************************************/
	UID64_t nRetUniqueNumber = 0;
	char tmpCurTimeBuf[SIZE_MAX_SQL_DATETIME_STRING];
	char tmpExpireTimeBuf[SIZE_MAX_SQL_DATETIME_STRING];
// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) - �Ʒ��� ���� ���� ��.
// 	SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &nRetUniqueNumber, 0, NULL);
// 	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->nAccountUID, 0, NULL);
// 	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->nItemNum, 0, NULL);
// 	
// 	pQPMsg->atumTimeCurrentTime.GetSQLDateTimeString(tmpCurTimeBuf, SIZE_MAX_SQL_DATETIME_STRING);
// 	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpCurTimeBuf, 0, NULL);
// 	
// 	pQPMsg->atumTimeExpireTime.GetSQLDateTimeString(tmpExpireTimeBuf, SIZE_MAX_SQL_DATETIME_STRING);
// 	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpExpireTimeBuf, 0, NULL);
// 	// 2008-07-08 by cmkwon, MySQL �������� ����(���ν������� ����ó�� �Ұ��� ����) - 
// 	//RETCODE ret = SQLExecDirect(hstmt, (UCHAR*)"{? = call atum_insert_AccountCashStoreItem(?, ?, ?, ?)}", SQL_NTS);
// 	RETCODE ret = SQLExecDirect(hstmt, (UCHAR*)"{call atum_insert_AccountCashStoreItem(?, ?, ?, ?, ?)}", SQL_NTS);
// 	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
// 	{
// 		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_insert_AccountCashStoreItem Failed!\n", TRUE, q.pFieldIOCPSocket, q);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		util::del(pQPMsg);
// 		return;
// 	}
// 
// 	// Clear any result sets generated.
// 	while ( ( ret = SQLMoreResults(hstmt) ) != SQL_NO_DATA );
// 	SQLFreeStmt(hstmt, SQL_CLOSE);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->nAccountUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->nItemNum, 0, NULL);	
	pQPMsg->atumTimeCurrentTime.GetSQLDateTimeString(tmpCurTimeBuf, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpCurTimeBuf, 0, NULL);
	pQPMsg->atumTimeExpireTime.GetSQLDateTimeString(tmpExpireTimeBuf, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpExpireTimeBuf, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0122, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_insert_AccountCashStoreItem Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	SQLBindCol(hstmt, 1, SQL_C_SBIGINT, &(nRetUniqueNumber), 0, NULL);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(hstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 �̸� ����� RowCount �̴� 
			ret = SQLMoreResults(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(hstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(hstmt);                 // 2008-07-25 by cmkwon, ���� Results �� �ִ����� �ѹ��� üũ�Ѵ�.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	if(0 == nRetUniqueNumber)
	{// Stored Procedure���� ���� ó����

		util::del(pQPMsg);
		return;
	}
	
	if(q.pFieldIOCPSocket->IsUsing()
		&& pQPMsg->nAccountUID == q.pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber)
	{
		q.pFieldIOCPSocket->m_ItemManager.CashOnEventPremiumCardFromDB(q.enumQuryType, pQPMsg->nAccountUID, nRetUniqueNumber);
	}
	util::del(pQPMsg);
}

void CAtumFieldDBManager::QP_CashUpdatePremiumCard(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_CASH_UPDATE_PREMIUM_CARD *pQPMsg = (QPARAM_CASH_UPDATE_PREMIUM_CARD*)q.pQueryParam;
	/*******************************************************************************
	-- 2005-01-20 by cmkwon, Cash ī�� ������ Update
	CREATE PROCEDURE dbo.atum_update_AccountCashStoreItem
		@i_accNum			INT,				-- AccountUniqueNumber	
		@i_ItemNum			INT,				-- ã�� ����Ÿ�� ������ ItemNum	
		@i_CurTime			VARCHAR(30),		-- ���� �ð�
		@i_ExpireTime		VARCHAR(30)			-- ��� ���� �ð�
	AS
		DECLARE @RetUniqueNum	BIGINT
		SET @RetUniqueNum = (SELECT uniqueNumber FROM td_accountCashStore WHERE AccountUniqueNumber = @i_accNum)
		IF (@RetUniqueNum IS NULL)
		BEGIN
			-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
			--RETURN 0
			SELECT 0;
			RETURN;
		END
		
		UPDATE td_accountCashStore
			SET ItemNum = @i_ItemNum, UpdatedTime = @i_CurTime, ExpireTime = @i_ExpireTime
			WHERE uniqueNumber = @RetUniqueNum
				
		-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
		--RETURN @RetUniqueNum
		SELECT @RetUniqueNum;
	GO
	*******************************************************************************/
	UID64_t nRetUniqueNumber = 0;
	char tmpCurTimeBuf[SIZE_MAX_SQL_DATETIME_STRING];
	char tmpExpireTimeBuf[SIZE_MAX_SQL_DATETIME_STRING];

// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) - �Ʒ��� ���� ���� ��.
// 	SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &nRetUniqueNumber, 0, NULL);
// 	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->nAccountUID, 0, NULL);
// 	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->nItemNum, 0, NULL);
// 	
// 	pQPMsg->atumTimeCurrentTime.GetSQLDateTimeString(tmpCurTimeBuf, SIZE_MAX_SQL_DATETIME_STRING);
// 	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpCurTimeBuf, 0, NULL);
// 	
// 	pQPMsg->atumTimeExpireTime.GetSQLDateTimeString(tmpExpireTimeBuf, SIZE_MAX_SQL_DATETIME_STRING);
// 	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpExpireTimeBuf, 0, NULL);
// 	// 2008-07-08 by cmkwon, MySQL �������� ����(���ν������� ����ó�� �Ұ��� ����) - 
// 	//RETCODE ret = SQLExecDirect(hstmt, (UCHAR*)"{? = call atum_update_AccountCashStoreItem(?, ?, ?, ?)}", SQL_NTS);
// 	RETCODE ret = SQLExecDirect(hstmt, (UCHAR*)"{call atum_update_AccountCashStoreItem(?, ?, ?, ?, ?)}", SQL_NTS);
// 	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
// 	{
// 		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_update_AccountCashStoreItem Failed!\n", TRUE, q.pFieldIOCPSocket, q);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		util::del(pQPMsg);
// 		return;
// 	}
// 
// 	// Clear any result sets generated.
// 	while ( ( ret = SQLMoreResults(hstmt) ) != SQL_NO_DATA );
// 	SQLFreeStmt(hstmt, SQL_CLOSE);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) - 
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->nAccountUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->nItemNum, 0, NULL);
	pQPMsg->atumTimeCurrentTime.GetSQLDateTimeString(tmpCurTimeBuf, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpCurTimeBuf, 0, NULL);
	pQPMsg->atumTimeExpireTime.GetSQLDateTimeString(tmpExpireTimeBuf, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpExpireTimeBuf, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0123, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_update_AccountCashStoreItem Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	SQLBindCol(hstmt, 1, SQL_C_SBIGINT, &(nRetUniqueNumber), 0, NULL);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(hstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 �̸� ����� RowCount �̴� 
			ret = SQLMoreResults(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(hstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(hstmt);                 // 2008-07-25 by cmkwon, ���� Results �� �ִ����� �ѹ��� üũ�Ѵ�.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	if(0 == nRetUniqueNumber)
	{// Stored Procedure���� ���� ó����

		util::del(pQPMsg);
		return;
	}
	
	if(q.pFieldIOCPSocket->IsUsing()
		&& pQPMsg->nAccountUID == q.pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber)
	{
		q.pFieldIOCPSocket->m_ItemManager.CashOnEventPremiumCardFromDB(q.enumQuryType, pQPMsg->nAccountUID, nRetUniqueNumber);
	}
	util::del(pQPMsg);
}

void CAtumFieldDBManager::QP_CashDeletePremiumCard(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_CASH_DELETE_PREMIUM_CARD *pQPMsg = (QPARAM_CASH_DELETE_PREMIUM_CARD*)q.pQueryParam;
	/********************************************************************************
	-- 2005-01-20 by cmkwon, Cash ī�� ������ �����Ѵ�
	CREATE PROCEDURE atum_delete_AccountCashStoreItem
		@i_accNum			INT				-- AccountUniqueNumber
	AS
		DELETE FROM td_accountCashStore
			WHERE AccountUniqueNumber = @i_accNum
	GO
	********************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->nAccountUID, 0, NULL);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0124, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_delete_AccountCashStoreItem Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	
	if(q.pFieldIOCPSocket->IsUsing()
		&& pQPMsg->nAccountUID == q.pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber)
	{
		q.pFieldIOCPSocket->m_ItemManager.CashOnEventPremiumCardFromDB(q.enumQuryType, pQPMsg->nAccountUID, 0);
	}
	util::del(pQPMsg);
}

void CAtumFieldDBManager::QP_LoadHappyHourEvent(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/********************************************************************************
	-- ��� ServerGroup �̺�Ʈ, ���� ������ �̺�Ʈ ��������
	-- 2006-08-25 by cmkwon, @i_ServerGroupID		INT �߰���
	-- // 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - �ʵ� �߰�(InfluenceType)
	-- 2008-03-13 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� - MinLevel, MaxLevel �ʵ� �߰�
	CREATE PROCEDURE dbo.atum_LoadHappyHourEvent
		@i_ServerGroupID		INT				-- 2006-08-25 by cmkwon, 0(=AllServerGroup), 1(=10061), 2(10062), ...
	AS
		-- // 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - �ʵ� �߰�(InfluenceType)�� �Ʒ��� ���� �ʵ�� ��÷� ������
		--SELECT * 
		--	FROM ti_HappyHourEvent WITH (NOLOCK)
		--	WHERE ServerGroupID= 0 OR ServerGroupID= @i_ServerGroupID
		--	ORDER BY DayOfWeek, StartTime
		SELECT UniqueNumber, ServerGroupID, DayOfWeek, StartTime, EndTime, EXPRate, SPIRate, EXPRepairRate, DropItemRate, DropRareItemRate
				, WarPointRate, InfluenceType, MinLevel, MaxLevel
		FROM ti_HappyHourEvent WITH (NOLOCK)
		WHERE ServerGroupID= 0 OR ServerGroupID= @i_ServerGroupID
		ORDER BY InfluenceType, DayOfWeek, StartTime

	GO
	********************************************************************************/

	//int nServerGroupID = g_pFieldGlobal->GetMGameServerID();
	//SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nServerGroupID, 0, NULL);
	auto ret = SQLExecDirect(hstmt, PROCEDURE_080822_0125, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_LoadHappyHourEvent Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	mtvectHAPPY_HOUR_EVENT	vectHappyEventList;
	SHAPPY_HOUR_EVENT		tmHappyEvent;
	SQL_TIMESTAMP_STRUCT	arrSqlTime[2];
	SQLINTEGER arrCB[15]	= {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS ,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS 
								,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};		// 2008-03-13 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� - // 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - �ʵ� �߰���, // 2007-06-26 by dhjin, ������Ʈ �̺�Ʈ ���� �߰�
	util::zero(&tmHappyEvent, sizeof(SHAPPY_HOUR_EVENT));
	util::zero(arrSqlTime, sizeof(arrSqlTime[0])*2);
	SQLBindCol(hstmt, 1, SQL_C_LONG, &tmHappyEvent.EventUniqueNumber, 0,	&arrCB[1]);
	SQLBindCol(hstmt, 2, SQL_C_LONG, &tmHappyEvent.ServerGroupID, 0,		&arrCB[2]);
	SQLBindCol(hstmt, 3, SQL_C_ULONG, &tmHappyEvent.DayOfWeek, 0,			&arrCB[3]);
	SQLBindCol(hstmt, 4, SQL_C_TIMESTAMP, &arrSqlTime[0], 0,				&arrCB[4]);
	SQLBindCol(hstmt, 5, SQL_C_TIMESTAMP, &arrSqlTime[1], 0,				&arrCB[5]);
	SQLBindCol(hstmt, 6, SQL_C_FLOAT, &tmHappyEvent.fEXPRate2, 0,			&arrCB[6]);
	SQLBindCol(hstmt, 7, SQL_C_FLOAT, &tmHappyEvent.fSPIRate2, 0,			&arrCB[7]);
	SQLBindCol(hstmt, 8, SQL_C_FLOAT, &tmHappyEvent.fEXPRepairRate2, 0,		&arrCB[8]);
	SQLBindCol(hstmt, 9, SQL_C_FLOAT, &tmHappyEvent.fDropItemRate2, 0,		&arrCB[9]);
	SQLBindCol(hstmt, 10, SQL_C_FLOAT, &tmHappyEvent.fDropRareItemRate2, 0,	&arrCB[10]);
	SQLBindCol(hstmt, 11, SQL_C_FLOAT, &tmHappyEvent.fWarPointRate2	, 0,	&arrCB[11]);		// 2007-06-26 by dhjin, ������Ʈ �̺�Ʈ ���� �߰�
	SQLBindCol(hstmt, 12, SQL_C_TINYINT, &tmHappyEvent.InfluenceType3, 0,	&arrCB[12]);		// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - �߰��� �ʵ�
	SQLBindCol(hstmt, 13, SQL_C_LONG, &tmHappyEvent.MinLevel, 0,			&arrCB[13]);		// 2008-03-13 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� - 
	SQLBindCol(hstmt, 14, SQL_C_ULONG, &tmHappyEvent.MaxLevel, 0,			&arrCB[14]);		// 2008-03-13 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� - 

	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA)
	{
		tmHappyEvent.atimeStartTime2	= arrSqlTime[0];
		tmHappyEvent.atimeEndTime2		= arrSqlTime[1];
		vectHappyEventList.push_back(tmHappyEvent);
		util::zero(&tmHappyEvent, sizeof(SHAPPY_HOUR_EVENT));
		util::zero(arrSqlTime, sizeof(arrSqlTime[0])*2);
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	m_pFieldIOCP8->m_gameEventManager.EndAllGameEvent();
	m_pFieldIOCP8->m_gameEventManager.EndPCBangGameEvent();

// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - CHappyHourEventManager::ResetHappyHourEventManager() �Լ����� ó�� ��
//	if(m_pFieldIOCP8->m_pHappyHourEventManager->GetCurrentHappyHourEvent())
//	{
//		INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, pSHappy, SendBuf);
//		pSHappy->byStateType			= HAPPYEV_STATE_TYPE_END;
//		m_pFieldIOCP8->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK));
//	}
//
//	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, pSPCBang, PCBangSendBuf);
//	if(m_pFieldIOCP8->m_pHappyHourEventManager->GetPCBangHappyHourEventInfo(pSPCBang))
//	{
//		pSPCBang->byStateType			= PCBANG_HAPPYEV_STATE_TYPE_END;
//		m_pFieldIOCP8->SendMessageToAllClients(PCBangSendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK), INFLUENCE_TYPE_ALL_MASK, 0, 0, NULL, TRUE);
//	}

	///////////////////////////////////////////////////////////////////////////////
	// 2007-11-02 by cmkwon, �ý��� �α� �߰�
	g_pGlobal->WriteSystemLogEX(TRUE, "[Notify] HappyHourEvent reload !!\r\n");

	m_pFieldIOCP8->m_pHappyHourEventManager->ResetHappyHourEventManager(TRUE);	// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - �Լ� ���� �߰� ��
	m_pFieldIOCP8->m_pHappyHourEventManager->InitHappyHourEventManager(&vectHappyEventList);
}

void CAtumFieldDBManager::QP_UpdateHappyHourEventDate(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/********************************************************************************
	-- HappyHourEvnet ���� Update -- ����	- ��(0) ��(1) ȭ(2) ��(3) ��(4) ��(5) ��(6) HappyHourEvent�Ⱓ(7) PCBangHappyHourEvent�Ⱓ�׼���(8)
	-- 2006-08-25 by cmkwon, ti_HappyHourEvent ���̺� ServerGroupID �ʵ� �߰���
	-- // 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - ���� ���� �߰� ��
	CREATE PROCEDURE dbo.atum_UpdateHappyHourEventDate
		@i_StartTime		VARCHAR(30),		-- �̺�Ʈ ���� �ð�
		@i_EndTime			VARCHAR(30),		-- �̺�Ʈ ���� �ð�
		@i_InflTy			TINYINT				-- // 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - dbo.atum_UpdateHappyHourEventDate() ���ν����� �߰� �� ����
	AS
		DECLARE @searchUniqueNumber INT
		-- // 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - �ش� ������ �Ⱓ�� �˻��Ѵ�.
		SET @searchUniqueNumber = (SELECT UniqueNumber FROM ti_HappyHourEvent WHERE InfluenceType = @i_InflTy AND DayOfWeek = 7)

		IF (@searchUniqueNumber IS NULL)
			BEGIN
				INSERT INTO ti_HappyHourEvent
					VALUES(0, 7, @i_StartTime, @i_EndTime, 0, 0, 0, 0, 0, 0, @i_InflTy, 1, 100)
					-- 2007-08-31 by cmkwon, ����
					-- // 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - ���� �ʵ� �߰���
					-- // 2008-03-13 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� -
			END
		ELSE
			BEGIN
				UPDATE ti_HappyHourEvent
					SET StartTime = @i_StartTime, EndTime = @i_EndTime
					WHERE UniqueNumber = @searchUniqueNumber
			END	
	GO
	********************************************************************************/
	QPARAM_UPDATE_HAPPY_HOUR_EVENT_DATE *pQPMsg = (QPARAM_UPDATE_HAPPY_HOUR_EVENT_DATE*)q.pQueryParam;
	char szStartTime[SIZE_MAX_SQL_DATETIME_STRING];
	char szEndTime[SIZE_MAX_SQL_DATETIME_STRING];

	pQPMsg->atimeStartTime2.GetSQLDateTimeString(szStartTime, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szStartTime, 0, NULL);

	pQPMsg->atimeEndTime2.GetSQLDateTimeString(szEndTime, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szEndTime, 0, NULL);
	
	// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - �߰��� ����
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &pQPMsg->byInfluenceType0, 0, NULL);
	
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0126, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateHappyHourEventDate Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_UpdateInfluenceWarData(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-14 ~ 2006-04-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_UpdateInfluenceWarData(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/********************************************************************************
	--!!!!
	-- Name: atum_UpdateInfluenceWarData
	-- Desc: 2006-04-14 by cmkwon, 
	--====
	CREATE PROCEDURE atum_UpdateInfluenceWarData
		@i_InfluenceType			TINYINT,
		@i_MGameServerID			INT,				-- ������������ �߻��� ������ä�� ID
		@i_WartimeStage				TINYINT,
		@i_ContributionPoint		INT					-- 
	AS
		UPDATE td_InfluenceWarData
			SET WartimeStage = @i_WartimeStage, ContributionPoint = @i_ContributionPoint
			WHERE InfluenceType = @i_InfluenceType and MGameServerID = @i_MGameServerID
	GO
	********************************************************************************/
	QPARAM_UPDATE_INFLUENCE_WAR_DATA *pQPMsg = (QPARAM_UPDATE_INFLUENCE_WAR_DATA*)q.pQueryParam;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQPMsg->InfluenceType0, 0, NULL);
	//SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->MGameServerID0, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQPMsg->WartimeStage0, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->ContributionPoint0, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0127, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateInfluenceWarData Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);
}

// 2013-05-09 by hskim, ���� ����Ʈ ����
void CAtumFieldDBManager::QP_UpdateInfluenceConsecutiveVictorites(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/********************************************************************************
	--!!!!
	-- Name: atum_UpdateInfluenceConsecutiveVictorites
	-- Desc: // 2013-05-09 by hskim, ���� ����Ʈ ����
	--====
	CREATE PROCEDURE atum_UpdateInfluenceConsecutiveVictorites
		@i_InfluenceType			TINYINT,
		@i_MGameServerID			INT,	
		@i_ConsecutiveVictories		SMALLINT
	AS
	BEGIN
		UPDATE td_InfluenceWarData
			SET ConsecutiveVictories = @i_ConsecutiveVictories
			WHERE InfluenceType = @i_InfluenceType and MGameServerID = @i_MGameServerID
	END
	GO
	********************************************************************************/
	QPARAM_UPDATE_INFLUENCE_CONSECUTIVE_VICTORIES *pQPMsg = (QPARAM_UPDATE_INFLUENCE_CONSECUTIVE_VICTORIES*)q.pQueryParam;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQPMsg->InfluenceType0, 0, NULL);
	//SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->MGameServerID0, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &pQPMsg->ConsecutiveVictories, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_130510_0001, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateInfluenceConsecutiveVictorites Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);
}
// 2013-05-09 by hskim, ���� ����Ʈ ����

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_UpdateOwnerOfConflictArea(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-12 ~ 2006-07-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_UpdateOwnerOfConflictArea(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/********************************************************************************
	--!!!!
	-- Name: atum_UpdateOwnerOfConflictArea
	-- Desc: 2006-07-12 by cmkwon, 
	--====
	CREATE PROCEDURE atum_UpdateOwnerOfConflictArea
		@i_InfluenceType			TINYINT,			-- �������� ���� ����
		@i_MGameServerID			INT					-- ������������ �߻��� ������ä�� ID
	AS
		-- �ٸ� ������ ���� �ʵ带 0���� �����Ѵ�
		UPDATE td_InfluenceWarData
			SET OwnerOfConflictArea = 0
			WHERE InfluenceType <> @i_InfluenceType and MGameServerID = @i_MGameServerID

		-- ���� ���� ������ ���� �ʵ带 1�� �����Ѵ�.
		UPDATE td_InfluenceWarData
			SET OwnerOfConflictArea = 1
			WHERE InfluenceType = @i_InfluenceType and MGameServerID = @i_MGameServerID
	GO
	********************************************************************************/
	QPARAM_UPDATE_OWNEROFCONFLICTAREA *pQPMsg = (QPARAM_UPDATE_OWNEROFCONFLICTAREA*)q.pQueryParam;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQPMsg->InfluenceType0, 0, NULL);
	//SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->MGameServerID0, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0128, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateOwnerOfConflictArea Failed!\n", TRUE, q.pFieldIOCPSocket, q);
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_UpdateSubleader(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		
/// \author		dhjin
/// \date		2007-02-13 ~ 2007-02-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_UpdateSubleader(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/********************************************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_Update_SubLeader
	-- DESC      : 2007-02-13 by dhjin, �������� ���� ���ν���
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Update_SubLeader
		-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
		--@o_RetCode				INT OUTPUT,
		@i_CharacterName		varchar(20),
		@i_InflType				TINYINT,
		@i_SubLeaderRank		TINYINT,
		@i_OrderNormalRace		TINYINT
	AS
		DECLARE @SetSubLeaderNum	INT
		DECLARE @SetUID				INT
		DECLARE @CheckCount	INT

		IF(0 <> @i_OrderNormalRace)
		BEGIN
			SELECT @SetUID = UniqueNumber FROM dbo.td_character WITH(NOLOCK) where CharacterName = @i_CharacterName AND InfluenceType = @i_InflType
			IF(@SetUID IS NULL)
			BEGIN
				-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
				--SET @o_RetCode = 10						-- �������� �ɸ��� �˻� �̸��� ����.
				--RETURN
				SELECT 0, 10;
				RETURN;
			END
		END
		ELSE
		BEGIN
			SELECT @SetUID = UniqueNumber FROM dbo.td_character WITH(NOLOCK) where CharacterName = @i_CharacterName AND InfluenceType = @i_InflType AND Race = 2
			IF(@SetUID IS NULL)
			BEGIN
				-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
				--SET @o_RetCode = 10						-- �������� �ɸ��� �˻� �̸��� ����.
				--RETURN
				SELECT 0, 10;
				RETURN;
			END
		END

		
		SELECT @CheckCount = count(*)  FROM dbo.td_InfluenceWarData WITH(NOLOCK) 
			WHERE InfluenceType = @i_InflType  AND (InflSub1LeaderCharacterUID = @SetUID OR  InflSub2LeaderCharacterUID = @SetUID OR InflLeaderCharacterUID = @SetUID)
		
		IF(@CheckCount > 0)
		BEGIN
			-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
			--SET @o_RetCode = 0
			--RETURN
			SELECT 0, 0;
			RETURN;
		END

		IF(1 = @i_SubLeaderRank)
		BEGIN
			SELECT @SetSubLeaderNum = InflSub1LeaderCharacterUID FROM dbo.td_InfluenceWarData WITH(NOLOCK) 
					WHERE InfluenceType = @i_InflType AND  InflSub2LeaderCharacterUID <> @SetUID AND InflLeaderCharacterUID <> @SetUID
			IF(@SetSubLeaderNum = @SetUID)
			BEGIN
				-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
				--SET @o_RetCode = 20						-- �̹� �������ڷ� ���� �Ǿ��ִ�.
				--RETURN
				SELECT 0, 20;
				RETURN;
			END
			IF(@SetSubLeaderNum = 0 OR @SetSubLeaderNum IS NULL)
			BEGIN
				UPDATE dbo.td_InfluenceWarData SET InflSub1LeaderCharacterUID = @SetUID
				WHERE InfluenceType = @i_InflType

				-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
				--SET @o_RetCode = 1
				--RETURN	@SetUID							-- ��������1�� ����
				SELECT @SetUID, 1;
				RETURN;
			END
		END
		
		ELSE IF(2 = @i_SubLeaderRank)
		BEGIN
			SELECT @SetSubLeaderNum = InflSub2LeaderCharacterUID FROM dbo.td_InfluenceWarData WITH(NOLOCK) 
					WHERE InfluenceType = @i_InflType AND  InflSub1LeaderCharacterUID <> @SetUID AND InflLeaderCharacterUID <> @SetUID
			IF(@SetSubLeaderNum = @SetUID)
			BEGIN
				-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
				--SET @o_RetCode = 20						-- �̹� �������ڷ� ���� �Ǿ��ִ�.
				--RETURN
				SELECT 0, 20;
				RETURN;
			END
			IF(@SetSubLeaderNum = 0 OR @SetSubLeaderNum IS NULL)
			BEGIN
				UPDATE dbo.td_InfluenceWarData SET InflSub2LeaderCharacterUID = @SetUID
				WHERE InfluenceType = @i_InflType
				
				-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
				--SET @o_RetCode = 2
				--RETURN	@SetUID							-- ��������2�� ����
				SELECT @SetUID, 2;
				RETURN;
			END
		END
		
		-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
		--SET @o_RetCode = 0
		--RETURN										-- �������� ������ �̹� �Ǿ��־ ������ �ȵȴ�.
		SELECT 0,0;
	GO
	********************************************************************************/
	QPARAM_UPDATE_SUBLEADER		*pQPMsg = (QPARAM_UPDATE_SUBLEADER*)q.pQueryParam;
	RETCODE ret;
	INT RetCode = -1;
	UID32_t nReturnValue = 0;

// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) - 
// 	SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nReturnValue, 0, NULL);
// 	SQLBindParameter(hstmt, 2, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &RetCode, 0, NULL);
// 	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pQPMsg->CharacterName, 0, NULL);
// 	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQPMsg->InflType, 0, NULL);
// 	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQPMsg->SubLeaderRank, 0, NULL);
// 
// 	// 2008-07-08 by cmkwon, MySQL �������� ����(���ν������� ����ó�� �Ұ��� ����) - 
// 	//ret = SQLExecDirect(hstmt, (UCHAR*)"{? = call atum_Update_SubLeader(?,?,?,?)}", SQL_NTS);
// 	ret = SQLExecDirect(hstmt, (UCHAR*)"{call atum_Update_SubLeader(?, ?,?,?,?)}", SQL_NTS);
// 	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
// 	{
// 		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Update_SubLeader Failed!\n", TRUE, q.pFieldIOCPSocket, q);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		util::del(pQPMsg);
// 		return;
// 	}
// 	// Clear any result sets generated. For ouput parameter...
// 	while ( ( ret = SQLMoreResults(hstmt) ) != SQL_NO_DATA );

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pQPMsg->CharacterName, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQPMsg->InflType, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQPMsg->SubLeaderRank, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQPMsg->OrderNormalRace, 0, NULL);		 // 2008-08-21 by dhjin, �Ϲ�, Ư�� ������ �������� �Ӹ� ����
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0129, SQL_NTS);

	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Update_SubLeader Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	SQLBindCol(hstmt, 1, SQL_C_LONG, &(nReturnValue), 0, NULL);
	SQLBindCol(hstmt, 2, SQL_C_LONG, &(RetCode), 0, NULL);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(hstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 �̸� ����� RowCount �̴� 
			ret = SQLMoreResults(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(hstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(hstmt);                 // 2008-07-25 by cmkwon, ���� Results �� �ִ����� �ѹ��� üũ�Ѵ�.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}

	// 2007-10-06 by dhjin, Ŭ���̾�Ʈ���� ���� ����
	INIT_MSG_WITH_BUFFER(MSG_FC_CHARACTER_UPDATE_SUBLEADER_OK, T_FC_CHARACTER_UPDATE_SUBLEADER_OK, pSendMsg, SendBufFC);
	pSendMsg->SubLeaderRank		= pQPMsg->SubLeaderRank;
	util::strncpy(pSendMsg->CharacterName, pQPMsg->CharacterName, SIZE_MAX_CHARACTER_NAME);	
	
	if(0 == RetCode)
	{
		q.pFieldIOCPSocket->SendErrorMessage(T_FC_CHARACTER_UPDATE_SUBLEADER_OK, ERR_PROTOCOL_SUBLEADER_SET_FAIL);
	}
	else if(1 == RetCode)
	{
		q.pFieldIOCPSocket->ms_pFieldIOCP->m_InflWarManager.SetSubLeader(pQPMsg->InflType, nReturnValue, pQPMsg->CharacterName, 1);
		INIT_MSG_WITH_BUFFER(MSG_FI_UPDATE_SUBLEADER, T_FI_UPDATE_SUBLEADER, pMsgSubLeader, SendBuf);
		pMsgSubLeader->CharacterUID		= nReturnValue;
		pMsgSubLeader->InfluenceType	= pQPMsg->InflType;
		pMsgSubLeader->SubLeaderNum		= 1;
		q.pFieldIOCPSocket->ms_pFieldIOCP->Send2IMServer(SendBuf, MSG_SIZE(MSG_FI_UPDATE_SUBLEADER));
		// 2007-10-06 by dhjin, ���������� üũ�Ͽ� �������̸� InflWarManager�� �������� ������ �߰��Ѵ�.
		q.pFieldIOCPSocket->ms_pFieldIOCP->m_InflWarManager.SetSubLeaderGuildUID(1, pQPMsg->InflType, nReturnValue);
		
		// 2007-10-06 by dhjin, �������� ���� ����
		q.pFieldIOCPSocket->SendAddData(SendBufFC, MSG_SIZE(MSG_FC_CHARACTER_UPDATE_SUBLEADER_OK));
		CFieldIOCPSocket * PeerFISoc = q.pFieldIOCPSocket->ms_pFieldIOCP->GetFieldIOCPSocketByCharacterName(pQPMsg->CharacterName);
		if(NULL != PeerFISoc)
		{// 2007-10-06 by dhjin, ��� ������ �����ϸ� �޼��� ����
			PeerFISoc->SendAddData(SendBufFC, MSG_SIZE(MSG_FC_CHARACTER_UPDATE_SUBLEADER_OK));
		}
	}
	else if(2 == RetCode)
	{
		q.pFieldIOCPSocket->ms_pFieldIOCP->m_InflWarManager.SetSubLeader(pQPMsg->InflType, nReturnValue, pQPMsg->CharacterName, 2);
		INIT_MSG_WITH_BUFFER(MSG_FI_UPDATE_SUBLEADER, T_FI_UPDATE_SUBLEADER, pMsgSubLeader, SendBuf);
		pMsgSubLeader->CharacterUID		= nReturnValue;
		pMsgSubLeader->InfluenceType	= pQPMsg->InflType;
		pMsgSubLeader->SubLeaderNum		= 2;
		q.pFieldIOCPSocket->ms_pFieldIOCP->Send2IMServer(SendBuf, MSG_SIZE(MSG_FI_UPDATE_SUBLEADER));
		// 2007-10-06 by dhjin, ���������� üũ�Ͽ� �������̸� InflWarManager�� �������� ������ �߰��Ѵ�.
		q.pFieldIOCPSocket->ms_pFieldIOCP->m_InflWarManager.SetSubLeaderGuildUID(2, pQPMsg->InflType, nReturnValue);
				
		// 2007-10-06 by dhjin, �������� ���� ����
		q.pFieldIOCPSocket->SendAddData(SendBufFC, MSG_SIZE(MSG_FC_CHARACTER_UPDATE_SUBLEADER_OK));
		CFieldIOCPSocket * PeerFISoc = q.pFieldIOCPSocket->ms_pFieldIOCP->GetFieldIOCPSocketByCharacterName(pQPMsg->CharacterName);
		if(NULL != PeerFISoc)
		{// 2007-10-06 by dhjin, ��� ������ �����ϸ� �޼��� ����
			PeerFISoc->SendAddData(SendBufFC, MSG_SIZE(MSG_FC_CHARACTER_UPDATE_SUBLEADER_OK));
		}
	}
	else if(10 == RetCode)
	{
//		q.pFieldIOCPSocket->SendErrorMessage(T_FC_CHARACTER_UPDATE_SUBLEADER_OK, ERR_PROTOCOL_SUBLEADER_SET_FAIL);
		q.pFieldIOCPSocket->SendString128(STRING_128_USER_NOTICE, STRMSG_080821_0001); // 2008-08-21 by dhjin, �Ϲ�, Ư�� ������ �������� �Ӹ� ����
	}
	else if(20 == RetCode)
	{
		q.pFieldIOCPSocket->SendErrorMessage(T_FC_CHARACTER_UPDATE_SUBLEADER_OK, ERR_PROTOCOL_SUBLEADER_SET_ALEADY);
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);	
	util::del(pQPMsg);
	return;
}

/////////////////////////////////////////////////////////////////////////////////
///// \fn			void CAtumFieldDBManager::QP_JamboreeInit(FIELD_DB_QUERY q, SQLHSTMT hstmt)
///// \brief		
///// \author		cmkwon
///// \date		2007-04-10 ~ 2007-04-10
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//void CAtumFieldDBManager::QP_JamboreeInit(FIELD_DB_QUERY q, SQLHSTMT hstmt)
//{
//	/********************************************************************************
//	--!!!!
//	-- Name: jamboree_init_jamboree_db
//	-- Desc: ��ȸ���� �׷�� ���ν����� DB�� ĳ���͸� �����ϱ� ���� ���ν����̴�
//	--		 ���, �������� ĳ���͸� ������ ��� ĳ���Ϳ� ĳ���� ���� �����Ͱ� ������
//	--
//	--		#define RACE_OPERATION				(USHORT)0x0080	// ������, 128
//	--		#define RACE_GAMEMASTER				(USHORT)0x0100	// ���Ӹ�����, 256
//
//	-- warning:	������ ������ ���¿��� ó���Ǿ�� ��
//	--			
//	-- input:
//	-- output:
//	--====
//	CREATE PROCEDURE dbo.jamboree_init_jamboree_db	
//	AS
//		-- 1. ĳ���� ���� �������� ��æƮ ����	
//		DELETE atum2_db_20.dbo.td_Enchant FROM td_Enchant e, td_Store s 
//		WHERE s.AccountUniqueNumber IN 
//		(SELECT AccountUniqueNumber FROM atum2_db_20.dbo.td_Character WITH (NOLOCK) WHERE 0 = Race & 0x180) AND s.Possess IN 
//		(SELECT UniqueNumber FROM atum2_db_20.dbo.td_Character WITH (NOLOCK) WHERE 0 = Race & 0x180) AND s.UniqueNumber = e.TargetItemUniqueNumber
//
//		-- 2. ĳ���� ���� ������ ����
//		DELETE FROM atum2_db_20.dbo.td_Store WHERE AccountUniqueNumber IN 
//		(SELECT AccountUniqueNumber FROM atum2_db_20.dbo.td_Character WITH (NOLOCK) WHERE 0 = Race & 0x180) AND possess IN 
//		(SELECT UniqueNumber FROM atum2_db_20.dbo.td_Character WITH (NOLOCK) WHERE 0 = Race & 0x180)
//
//		-- 3. ĳ���� �� ���� ����
//		DELETE FROM atum2_db_20.dbo.td_QuickSlot WHERE AccountUID IN 
//		(SELECT AccountUniqueNumber FROM atum2_db_20.dbo.td_Character WITH (NOLOCK) WHERE 0 = Race & 0x180) AND CharacterUID IN 
//		(SELECT UniqueNumber FROM atum2_db_20.dbo.td_Character WITH (NOLOCK) WHERE 0 = Race & 0x180)
//
//		-- 4. ĳ���� ���� ��ų ����
//		DELETE FROM atum2_db_20.dbo.td_StoreSkill WHERE AccountUniqueNumber IN 
//		(SELECT AccountUniqueNumber FROM atum2_db_20.dbo.td_Character WITH (NOLOCK) WHERE 0 = Race & 0x180) AND possess IN 
//		(SELECT UniqueNumber FROM atum2_db_20.dbo.td_Character WITH (NOLOCK) WHERE 0 = Race & 0x180)
//
//		-- 5. ����Ʈ ���̺��� ����
//		DELETE FROM atum2_db_20.dbo.td_CharacterQuest WHERE CharacterUniqueNumber IN 
//		(SELECT UniqueNumber FROM atum2_db_20.dbo.td_Character WITH (NOLOCK) WHERE 0 = Race & 0x180)
//
//		-- 6. ģ������Ʈ���� ����
//		DELETE FROM atum2_db_20.dbo.td_FriendList WHERE CharacterName IN 
//		(SELECT CharacterName FROM atum2_db_20.dbo.td_Character WITH (NOLOCK) WHERE 0 = Race & 0x180) OR FriendName IN 
//		(SELECT CharacterName FROM atum2_db_20.dbo.td_Character WITH (NOLOCK) WHERE 0 = Race & 0x180)
//
//		-- last ĳ���� ���̺��� ����
//		DELETE FROM atum2_db_20.dbo.td_Character WHERE UniqueNumber IN 
//		(SELECT UniqueNumber FROM atum2_db_20.dbo.td_Character WITH (NOLOCK) WHERE 0 = Race & 0x180)
//
//		-- ��� ���, ������ ĳ������ ���� �ʱ�ȭ��
//		UPDATE atum2_db_20.dbo.td_character
//		SET GuildName = '', GuildUniqueNumber = 0
//
//		-- ��� ���� �ʱ�ȭ��, ��ȸ���� �ʱ�ȭ�� ��� ���� ������
//		TRUNCATE TABLE atum2_db_20.dbo.td_GuildMember
//		TRUNCATE TABLE atum2_db_20.dbo.td_Guild
//	GO
//	********************************************************************************/
//	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0130, SQL_NTS);
//	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
//	{
//		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"jamboree_init_jamboree_db@ Failed!\r\n", TRUE, q.pFieldIOCPSocket, q);
//		SQLFreeStmt(hstmt, SQL_CLOSE);
//		q.pFieldIOCPSocket->SendString128(STRING_128_USER_NOTICE, STRMSG_070410_0003);
//		return;
//	}
//	// Clear any result sets generated. For ouput parameter...
//	do {
//		ret = SQLMoreResults(hstmt);
//	} while (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);
//	SQLFreeStmt(hstmt, SQL_CLOSE);
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 2007-04-10 by cmkwon, �ʱ�ȭ ���� �˸�
//	q.pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_070410_0002);
//}

/////////////////////////////////////////////////////////////////////////////////
///// \fn			void CAtumFieldDBManager::QP_JamboreeEntrants(FIELD_DB_QUERY q, SQLHSTMT hstmt)
///// \brief		
///// \author		cmkwon
///// \date		2007-04-10 ~ 2007-04-10
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//void CAtumFieldDBManager::QP_JamboreeEntrants(FIELD_DB_QUERY q, SQLHSTMT hstmt)
//{
//	/********************************************************************************
//	--!!!!
//	-- Name: dbo.jamboree_copy_character_to_jamboree_db
//	-- Desc: ��ȸ���� �׷�� ���ν����� DB�� ĳ���͸� �����ϱ� ���� ���ν����̴�
//	--		 ���, �����ڸ� ������ ��� ĳ���Ϳ� ĳ���� ������ �����͸� �����Ѵ�.
//	--
//	--			2007-01-25, #define ITEM_IN_CHARACTER				0	// ĳ�����κ�
//	--						#define ITEM_IN_STORE					1	// â��
//	--						#define ITEM_IN_GUILD_STORE             2   // 2006-09-14 by dhjin, ���� â�� 
//	--			
//	-- parameter:
//	--		[1]	OUTPUT @o_RetCode:���� �ڵ�
//	--									0	==> ���� ����
//	--									20	==> �ش� ������ ĳ���Ͱ� ����
//	--									21	==> �����Ϸ��� �������� �ش� ������ ĳ���Ͱ� �����Ѵ�.
//	--									22	==> ���� ���������� ĳ���� ������ �˻����� ����, ĳ����
//	--									23	==> Ÿ�� �������� ĳ���� �߰��� ���� �߻���
//	--									24	==> Ÿ�� ������ td_Store���̺� ������ �߰��� ���� �߻���
//	--									25	==> Ÿ�� ������ td_Enchant���̺� ������ �߰��� ���� �߻���
//	--									26	==> Ÿ�� ������ td_StoreSkill���̺� ������ �߰��� ���� �߻���
//	--									27	==> Ÿ�� ������ td_CharacterQuest���̺� ������ �߰��� ���� �߻���
//	--									28	==> ���� ���������� ������ ������ �˻����� ����, td_Store
//	--									29	==> // 2007-11-05 by cmkwon, ��ȸ ���� ĳ���� ���� �ý��� ���� - Ÿ�� ������ DB �� ĳ���Ͱ� �̹� 3�� �̻� �����Ѵ�.
//	--									30	==> // 2007-11-05 by cmkwon, ��ȸ ���� ĳ���� ���� �ý��� ���� - Ÿ�� ������ DB �� ���� ĳ���͸��� �����Ѵ�.
//	--		[2] INPUT @i_CharName: ĳ���͸�
//	--		[3] INPUT i_PreAddString2CharacterName: ĳ���͸�տ� ���� �߰� ��Ʈ��
//	--									ex> '1_', '2_', '3_',....
//	--		// 2008-04-15 by cmkwon, ��ȸ����(JamboreeServer)�� �ý��� ���� - ���� 2�� �߰���
//	--		-- // 2009-08-25 by cmkwon, ������� �ʴ� �ʵ� ����(td_Store.ScarcityNumber) - 
//	--		-- // 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - td_Store.ShapeItemNum �ʵ� �߰�, ������ �о�ֱ�ÿ� �ʵ� �߰�
//	--====
//	********************************************************************************/
//
//
//	QPARAM_JAMBOREE_ENTRANTS *pQPMsg = (QPARAM_JAMBOREE_ENTRANTS*)q.pQueryParam;
//	if(FALSE == q.pFieldIOCPSocket->IsValidCharacter(FALSE))
//	{
//		util::del(pQPMsg);
//		return;
//	}
//
//	char szPreAddStr[SIZE_MAX_ADD_CHARACTER_NAME];
//	util::strncpy(szPreAddStr, CAtumSJ::GetJamboreePreAddCharacterString(g_pFieldGlobal->GetMGameServerID()-SERVERGROUP_START_ID+1), SIZE_MAX_ADD_CHARACTER_NAME);
//
//	int nRetCode = 0;
//	SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nRetCode, 0, NULL);
//	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pQPMsg->szCharName0, 0, NULL);
//	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ADD_CHARACTER_NAME, 0, szPreAddStr, 0, NULL);
//	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQPMsg->byInflTy, 0, NULL);				// 2008-04-15 by cmkwon, ��ȸ����(JamboreeServer)�� �ý��� ���� - ���� ���� �߰�	
//	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pQPMsg->MapIndex, 0, NULL);				// 2008-04-15 by cmkwon, ��ȸ����(JamboreeServer)�� �ý��� ���� - ���º� ���ø� ���� �߰�	
//	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0131, SQL_NTS);	// 2008-04-15 by cmkwon, ��ȸ����(JamboreeServer)�� �ý��� ���� - ���� 2�� �߰�
//	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
//	{
//		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"@jamboree_copy_character_to_jamboree_db Failed!\r\n", TRUE, q.pFieldIOCPSocket, q);
//		SQLFreeStmt(hstmt, SQL_CLOSE);
//		util::del(pQPMsg);			// 2008-10-25 by cmkwon, ����
//		return;
//	}
//
//	do 
//	{
//		ret = SQLMoreResults(hstmt);
//	} while (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);
//	SQLFreeStmt(hstmt, SQL_CLOSE);
//
//	if(0 == nRetCode)
//	{
//		q.pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_070410_0004, pQPMsg->szCharName0);
//	}
//	else
//	{
//		switch(nRetCode)
//		{
//		case 20:
//			{
//				q.pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_070410_0005, pQPMsg->szCharName0);
//			}
//			break;
//		case 21:
//			{
//				q.pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_070410_0006, pQPMsg->szCharName0);
//			}
//			break;
//		case 23:
//		case 25:
//		case 26:
//		case 27:
//		case 28:
//			{
//				q.pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_070410_0007, pQPMsg->szCharName0);
//			}
//			break;
//		default:
//			{
//				q.pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_070410_0008, pQPMsg->szCharName0, nRetCode);
//			}			
//		}
//	}
//
//	util::del(pQPMsg);			// 2008-10-25 by cmkwon, ����
//}





///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_LoadItemEvent(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		
/// \author		dhjin
/// \date		2006-08-31 ~ 2006-08-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_LoadItemEvent(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QP_LoadItemEvent
	
	/*-------------------------------------------------------------------------------
	-- �̺�Ʈ ������ �˻� �ϴ� ���ν���
	-- // 2008-02-01 by cmkwon, ItemEvent �� LastGameEndDate üũ ��ƾ �߰� - �ʵ� �߰� �� �� �˻� ����� �߰�
	-- // 2009-11-19 by cmkwon, ������ �̺�Ʈ�� Account/Character üũ ��� �߰� - �ʵ� �߰� �� �� �˻� ����� �߰�
	call dbo.atum_Load_ItemEvent
	-------------------------------------------------------------------------------*/

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0132, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Load_ItemEvent Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	// 2010-06-16 by jskim, ������ �̺�Ʈ üũ ��� -
// 	SQLINTEGER cb[18]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
// 						,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS};

	SQLINTEGER cb[25]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
						,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
						,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};

	//end 2010-06-16 by jskim, ������ �̺�Ʈ üũ ��� -
	vectItemEventInfo	EventItemList;
	EventItemList.clear();
	SITEM_EVENT_INFO	EventItem;
	util::zero(&EventItem, sizeof(SITEM_EVENT_INFO));
	ATUM_DATE_TIME		CurTime { true };
	SQL_TIMESTAMP_STRUCT   StartTime0;
	SQL_TIMESTAMP_STRUCT   EndTime0;
	SQL_TIMESTAMP_STRUCT   LastGameEndDate;		// 2008-02-01 by cmkwon, ItemEvent �� LastGameEndDate üũ ��ƾ �߰� - 
	
	SQLBindCol(hstmt, 1, SQL_C_ULONG, &EventItem.ItemEventUID, 0,				&cb[1]);
	SQLBindCol(hstmt, 2, SQL_C_ULONG, &EventItem.ItemEventType, 0,				&cb[2]);
	SQLBindCol(hstmt, 3, SQL_C_ULONG, &EventItem.OnlyPCBang, 0,					&cb[3]);
	SQLBindCol(hstmt, 4, SQL_C_UTINYINT, &EventItem.byInfluenceType, 0,			&cb[4]);
	SQLBindCol(hstmt, 5, SQL_C_ULONG, &EventItem.ItemNum, 0,					&cb[5]);
	SQLBindCol(hstmt, 6, SQL_C_ULONG, &EventItem.Count, 0,						&cb[6]);
	SQLBindCol(hstmt, 7, SQL_C_TIMESTAMP, &StartTime0, 0,						&cb[7]);
	SQLBindCol(hstmt, 8, SQL_C_TIMESTAMP, &EndTime0, 0,							&cb[8]);
	SQLBindCol(hstmt, 9, SQL_C_BIT, &EventItem.MemberShip, 0,                   &cb[9]);
	SQLBindCol(hstmt, 10, SQL_C_UTINYINT, &EventItem.ReqLevel, 0,			    &cb[10]);
	SQLBindCol(hstmt, 11, SQL_C_BIT, &EventItem.NewMember, 0,                   &cb[11]);
	SQLBindCol(hstmt, 12, SQL_C_ULONG, &EventItem.UnitKind, 0,					&cb[12]);
	SQLBindCol(hstmt, 13, SQL_C_LONG, &EventItem.PrefixCodeNum, 0,				&cb[13]);
	SQLBindCol(hstmt, 14, SQL_C_LONG, &EventItem.SuffixCodeNum, 0,				&cb[14]);
	SQLBindCol(hstmt, 15, SQL_C_ULONG, &EventItem.UseLastGameEndDate, 0,		&cb[15]);	// 2008-02-01 by cmkwon, ItemEvent �� LastGameEndDate üũ ��ƾ �߰� - 
	SQLBindCol(hstmt, 16, SQL_C_TIMESTAMP, &LastGameEndDate, 0,					&cb[16]);	// 2008-02-01 by cmkwon, ItemEvent �� LastGameEndDate üũ ��ƾ �߰� - 
	SQLBindCol(hstmt, 17, SQL_C_LONG, &EventItem.CheckWithCharacterUID, 0,		&cb[17]);	// 2009-11-19 by cmkwon, ������ �̺�Ʈ�� Account/Character üũ ��� �߰� - 
	// 2010-06-16 by jskim, ������ �̺�Ʈ üũ ��� -
	SQLBindCol(hstmt, 18, SQL_C_LONG, &EventItem.iLevelMin, 0,					&cb[18]);
	SQLBindCol(hstmt, 19, SQL_C_LONG, &EventItem.iLevelMax, 0,					&cb[19]);
	//end 2010-06-16 by jskim, ������ �̺�Ʈ üũ ��� -
	SQLBindCol(hstmt, 20, SQL_C_LONG, &EventItem.LoginCheckNumber, 0,			&cb[20]);	// 2011-08-25 by shcho, Ƚ���� ������ ���ޱ�� ���� - üũ�ѹ� �߰�
#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, �������� �����߰�
	SQLBindCol(hstmt, 21, SQL_C_LONG, &EventItem.UseFixedPeriod, 0,				&cb[21]);	
	SQLBindCol(hstmt, 22, SQL_C_LONG, &EventItem.FixedPeriod_DAY, 0,			&cb[22]);	
#endif

	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA)
	{
		EventItem.StartTime		= StartTime0;
		EventItem.EndTime		= EndTime0;
		EventItem.atLastGameEndDate	= LastGameEndDate;		// 2008-02-01 by cmkwon, ItemEvent �� LastGameEndDate üũ ��ƾ �߰� - 
// 2008-01-10 by cmkwon, ������ �̺�Ʈ �ý��ۿ� �� ���� �ý��� �߰� - �Ʒ��� ���� ������
//		if ( (EventItem.EndTime) > CurTime )
//		{
//			ITEM *pItemInfo = m_pFieldIOCP8->GetItemInfo(EventItem.ItemNum);
//			if(NULL == pItemInfo)
//			{
//				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::QP_LoadItemEvent ItemNum error !!, ItemEventUID(%d) ItemNum(%8d)\r\n"
//					, EventItem.ItemEventUID, EventItem.ItemNum);
//			}
//			else
//			{
//				EventItemList.push_back(EventItem);
//			}
//		}
		if ( (EventItem.EndTime) < CurTime)
		{// 2008-01-11 by cmkwon, �̺�Ʈ ���� �ð� üũ
			util::zero(&EventItem, sizeof(SITEM_EVENT_INFO));
			continue;
		}
		ITEM *pItemInfo = m_pFieldIOCP8->GetItemInfo(EventItem.ItemNum);
		if(NULL == pItemInfo)
		{// 2008-01-11 by cmkwon, ������ ��ȿ�� üũ
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::QP_LoadItemEvent ItemNum error !!, ItemEventUID(%d) ItemNum(%8d)\r\n"
				, EventItem.ItemEventUID, EventItem.ItemNum);
			util::zero(&EventItem, sizeof(SITEM_EVENT_INFO));
			continue;
		}

		///////////////////////////////////////////////////////////////////////////////
		// 2008-01-10 by cmkwon, ������ �̺�Ʈ �ý��ۿ� �� ���� �ý��� �߰� - ����� �ϳ��� ���� �̺�Ʈ�� �����ϴ�.
		if(ITEM_EVENT_TYPE_COUPON_ONLYONE == EventItem.ItemEventType || ITEM_EVENT_TYPE_COUPON_ONEDAY == EventItem.ItemEventType)
		{
			int nCouponItemEvUID = 0;
			vectItemEventInfo::iterator itr = EventItemList.begin();
			for(; itr != EventItemList.end(); itr++)
			{
				SITEM_EVENT_INFO *pEvInfo = &*itr;
				if(ITEM_EVENT_TYPE_COUPON_ONLYONE == pEvInfo->ItemEventType || ITEM_EVENT_TYPE_COUPON_ONEDAY == pEvInfo->ItemEventType)
				{
					nCouponItemEvUID = pEvInfo->ItemEventUID;
					break;
				}
			}
			if(0 < nCouponItemEvUID)
			{// 2008-01-11 by cmkwon, �̹� ���� �̺�Ʈ�� �ϳ� �����Ѵ�. ���̻� ���� �̺�Ʈ�� �߰����� �ʴ´�.
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::QP_LoadItemEvent_ CouponEvent overlapped error !!, overlapped Coupon ItemEventUID(%d, %d)\r\n"
					, nCouponItemEvUID, EventItem.ItemEventUID);
				util::zero(&EventItem, sizeof(SITEM_EVENT_INFO));
				continue;
			}
		}

		EventItemList.push_back(EventItem);			
		util::zero(&EventItem, sizeof(SITEM_EVENT_INFO));
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	// 2013-03-29 by jhseol, ������ �̺�Ʈ - ����Ÿ�� �߰������� ����
#ifdef S_ITEM_EVENT_SUB_TYPE_JHSEOL
	vectItemEventInfo::iterator itr = EventItemList.begin();
	for ( ; itr != EventItemList.end() ; itr++ )
	{
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &itr->ItemEventUID, 0, NULL);
		ret = SQLExecDirect(hstmt, PROCEDURE_130329_0005, SQL_NTS);
		if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
		{
			ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_GetItemEventSubType Failed!\n", TRUE, q.pFieldIOCPSocket, q);
			SQLFreeStmt(hstmt, SQL_CLOSE);
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::QP_LoadItemEvent_ \r\n");
			continue;
		}
		SQLBindCol(hstmt, 1, SQL_C_BIT,	&itr->SubEventType, 0,				&cb[1]);
		for (int i = 0 ; i < SIZE_MAX_DESPARAM_COUNT_IN_ITEM_EVENT ; i++ )
		{
			SQLBindCol(hstmt, 2+(2*i), SQL_C_USHORT, &itr->ArrDestParameter[i], 0,			&cb[2+(2*i)]);
			SQLBindCol(hstmt, 3+(2*i), SQL_C_ULONG, &itr->ArrParameterValue[i], 0,			&cb[3+(2*i)]);
		}
		SQLFetch(hstmt);
		SQLFreeStmt(hstmt, SQL_CLOSE);
	}
#endif
	// end 2013-03-29 by jhseol, ������ �̺�Ʈ - ����Ÿ�� �߰������� ����
	
	m_pFieldIOCP8->m_pGiveItemManager->InitItemEventManager(&EventItemList);
}

// 2008-12-19 by cmkwon, QT_UpdatePCBangList->DQT_UpdatePCBangList ���� - DQP_UpdatePCBangList() �Լ��� ����
// ///////////////////////////////////////////////////////////////////////////////
// /// \fn			void CAtumFieldDBManager::QP_UpdatePCBangList(FIELD_DB_QUERY q, SQLHSTMT hstmt)
// /// \brief		
// /// \author		dhjin
// /// \date		2007-01-22 ~ 2007-01-22
// /// \warning	
// ///
// /// \param		
// /// \return		
// ///////////////////////////////////////////////////////////////////////////////
// void CAtumFieldDBManager::QP_UpdatePCBangList(FIELD_DB_QUERY q, SQLHSTMT hstmt)
// {// QT_UpdatePCBangList
// 	/************************************************************************
// 	CREATE PROCEDURE dbo.atum_GetPCBangIPList
// 	AS
// 		SELECT UID, ST_IP, ED_IP FROM td_PCBang with(NOLOCK)
// 	GO
// 	************************************************************************/
// 	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0133, SQL_NTS);
// 	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
// 	{
// 		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_GetPCBangIPList Failed!\n", TRUE, q.pFieldIOCPSocket, q);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		return;
// 	}
// 
// 	DWORD dwLoadingStartTick = timeGetTime();		// 2008-09-02 by cmkwon, �ð� ����ϱ� �߰� - 
// 
// 	PCBANG_INFO		TempPCBang_Info;
// 	util::zero(&TempPCBang_Info, sizeof(PCBANG_INFO));
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	// 2007-08-03 by dhjin, PC�� ����Ʈ ����
// 	SIPClassD				TempIPClassD;
// 	util::zero(&TempIPClassD, sizeof(SIPClassD));
// 	CPCBangIPManager		tmPCBangIPManager;
// 	INT						tmPCBangUIDCount	= 0;
// 
// 	SQLINTEGER cb[4]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
// 	SQLBindCol(hstmt, 1, SQL_C_ULONG, &TempPCBang_Info.PCBangUID, 0,				&cb[1]);
// 	SQLBindCol(hstmt, 2, SQL_C_CHAR, TempPCBang_Info.ST_IP, SIZE_MAX_IPADDRESS,		&cb[2]);
// 	SQLBindCol(hstmt, 3, SQL_C_CHAR, TempPCBang_Info.ED_IP, 4,					&cb[3]);	
// 	
// 	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA)
// 	{
// 		char * pIPStart0 = TempPCBang_Info.ST_IP;
// 		char * pIPEndD   = TempPCBang_Info.ED_IP;
// 		
// 		char * pIPClassA = strtok(pIPStart0,".");
// 		if (pIPClassA == NULL)
// 		{		
// 			char	szError[1024];
// 			sprintf(szError, "[Error] pIPClassA RANGE error \r\n");
// 			g_pFieldGlobal->WriteSystemLog(szError);
// 			DBGOUT(szError);
// 			continue;
// 		}
// 		char * pIPClassB = strtok(NULL,".");
// 		if (pIPClassB == NULL)
// 		{
// 			char	szError[1024];
// 			sprintf(szError, "[Error] pIPClassB RANGE error \r\n");
// 			g_pFieldGlobal->WriteSystemLog(szError);
// 			DBGOUT(szError);
// 			continue;
// 		}
// 		char * pIPClassC = strtok(NULL,".");
// 		if (pIPClassC == NULL)
// 		{
// 			char	szError[1024];
// 			sprintf(szError, "[Error] pIPClassC RANGE error \r\n");
// 			g_pFieldGlobal->WriteSystemLog(szError);
// 			DBGOUT(szError);
// 			continue;
// 		}
// 		char * pIPClassD = strtok(NULL,"\0");
// 		if (pIPClassD == NULL)
// 		{		
// 			char	szError[1024];
// 			sprintf(szError, "[Error] pIPClassD RANGE error \r\n");
// 			g_pFieldGlobal->WriteSystemLog(szError);
// 			DBGOUT(szError);
// 			continue;
// 		}
// 		
// 		BYTE iIPStartA	 = atoi(pIPClassA);
// 		BYTE iIPStartB	 = atoi(pIPClassB);
// 		BYTE iIPStartC	 = atoi(pIPClassC);
// 		BYTE iIPStartD	 = atoi(pIPClassD);
// 		BYTE iIPEndD	 = atoi(pIPEndD);
// 
// 		if (FALSE == util::in_range(1, iIPStartA, 255) ||
// 			FALSE == util::in_range(0, iIPStartB, 255) ||
// 			FALSE == util::in_range(0, iIPStartC, 255) ||
// 			FALSE == util::in_range(1, iIPStartD, 255)	||
// 			FALSE == util::in_range(1, iIPEndD, 255))
// 		{// 2007-01-22 by dhjin, IP ���� üũ
// 			char	szError[1024];
// 			sprintf(szError, "[Error] UpdatePCBangIPList RANGE error \r\n");
// 			g_pFieldGlobal->WriteSystemLog(szError);
// 			DBGOUT(szError);
// 			continue;
// 		}			
// 		
// 		BYTE IPClassDGap	 = (iIPEndD - iIPStartD);
// 
// 		if ( 0 > IPClassDGap)
// 		{// 2007-01-22 by dhjin, ����IP�� ��IP���� ũ�� ����
// 			char	szError[1024];
// 			sprintf(szError, "[Error] UpdatePCBangIPList (StartIP - EndIp) error \r\n");
// 			g_pFieldGlobal->WriteSystemLog(szError);
// 			DBGOUT(szError);
// 			continue;
// 		}
// 		
// 		
// 		// 2007-08-03 by dhjin, PC�� IP����Ʈ ����.
// 		TempIPClassD.PCBangUID		=	TempPCBang_Info.PCBangUID;
// 		TempIPClassD.IPClassDStart	=	iIPStartD;
// 		TempIPClassD.IPClassDEnd	=	iIPEndD;
// 		char szIP[SIZE_MAX_IPADDRESS];
// 		sprintf(szIP, "%d.%d.%d.0", iIPStartA, iIPStartB, iIPStartC);
// 		DWORD	dwIPClassC = inet_addr(szIP);
// 		
// 		tmPCBangIPManager.InsertPCBangIPList(dwIPClassC, &TempIPClassD);
//         tmPCBangUIDCount++;
// // 2007-08-07 by dhjin, ���� �Լ��� ����
// //		for (int i=0 ; i <= IPClassDGap; i++)
// //		{// 2007-01-22 by dhjin, PC�� IP ���� ��ŭ ����Ʈ�� �߰� �Ѵ�.			
// //			char szIP[SIZE_MAX_IPADDRESS];
// //			sprintf(szIP, "%d.%d.%d.%d", iIPStartA, iIPStartB, iIPStartC, iIPStartD+i);
// //			mapPCBangIPList.insert(pair<string, UID32_t>(szIP, TempPCBang_Info.PCBangUID));
// //		}
// 		util::zero(&TempPCBang_Info, sizeof(PCBANG_INFO));
// 		util::zero(&TempIPClassD, sizeof(SIPClassD));
// 	}
// 	SQLFreeStmt(hstmt, SQL_CLOSE);	
// 
// 	DWORD dwLoadingEndTick = timeGetTime();								// 2008-09-02 by cmkwon, �ð� ����ϱ� �߰� - 
// 
// 	g_pFieldGlobal->m_pPCBangIPManager->SetPCBangIPList(tmPCBangIPManager.GetPCBangIPList());
// 	
// 	DWORD dwUpdatedPCBangIPListTick = timeGetTime();					// 2008-09-02 by cmkwon, �ð� ����ϱ� �߰� - 
// 	int nLoadingTime = dwLoadingEndTick - dwLoadingStartTick;			// 2008-09-02 by cmkwon, �ð� ����ϱ� �߰� - 
// 	int nUpdatingTime = dwUpdatedPCBangIPListTick - dwLoadingEndTick;	// 2008-09-02 by cmkwon, �ð� ����ϱ� �߰� - 
// 
// 	// 2007-08-07 by dhjin, PC�� ����Ʈ UID ���� �����.
// 	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] CAtumFieldDBManager::QP_UpdatePCBangList_ PCBangUID Count: %d, TotalTime(%dms) = DBLoadingTime(%dms) + UpdateTime(%dms)\r\n", tmPCBangUIDCount, nLoadingTime+nUpdatingTime, nLoadingTime, nUpdatingTime);
// }

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_Get_QuickSlot(FIELD_DB_QUERY q, SQLHSTMT hstmt)	
/// \brief		
/// \author		dhjin
/// \date		2006-09-04 ~ 2006-09-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_Get_QuickSlot(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/************************************************************************
	---------------------------------------------------------------------
	-- PROCEDURE NAME: [atum_GetQuickSlot]
	-- DESC: 2012-06-19 by jhseol, ��� ������ �´� ������ ������ �� �ֵ��� ����
	---------------------------------------------------------------------
	ALTER PROCEDURE [dbo].[atum_GetQuickSlot]
		@i_AccountUID                int,
		@i_CharacterUID             int
		,@i_SlotPurpose				int
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
		AS
		SELECT [AccountUID],[CharacterUID],[TabIdx],[ItemNum_1],[ItemNum_2],[ItemNum_3],[ItemNum_4],[ItemNum_5],[ItemNum_6],[ItemNum_7],[ItemNum_8],[ItemNum_9],[ItemNum_10]
		FROM td_QuickSlot WITH (NOLOCK)
		WHERE AccountUID = @i_AccountUID AND CharacterUID = @i_CharacterUID AND SlotPurpose = @i_SlotPurpose
	GO
	************************************************************************/
	
	QPARAM_GET_QUICKSLOT *pQPMsg = (QPARAM_GET_QUICKSLOT*)q.pQueryParam;

	if(NULL == q.pFieldIOCPSocket
		|| FALSE == q.pFieldIOCPSocket->IsValidCharacter(FALSE))
	{
		util::del(pQPMsg);
		return;
	}

	// 2012-06-21 by jhseol, �Ʒ��� �߰�����part2 - �Ʒ��� ���� ������ ( 0 = ����, 1 = �Ʒ��� )
	int SlotPurpose = 0;
#ifdef S_ARENA_EX_1ST_JHSEOL
	// 2012-09-14 by jhseol, �Ʒ��� �߰����� restore - ������ ���� ���¿��� �ڽ��� �ɸ��ͷθ� �÷��� �ϵ��� ����
#ifndef S_ARENA_EX_1ST_RESTORE_JHSEOL
	SlotPurpose = pQPMsg->SlotPurpose;
#endif	//#ifndef S_ARENA_EX_1ST_RESTORE_JHSEOL
	// end 2012-09-14 by jhseol, �Ʒ��� �߰����� restore - ������ ���� ���¿��� �ڽ��� �ɸ��ͷθ� �÷��� �ϵ��� ����
#endif
	// end 2012-06-21 by jhseol, �Ʒ��� �߰�����part2 - �Ʒ��� ���� ������ ( 0 = ����, 1 = �Ʒ��� )

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->AccountUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->CharacterUID, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &SlotPurpose, 0, NULL);		// 2012-06-21 by jhseol, �Ʒ��� �߰�����part2 - �Ʒ��� ���� ������ ( 0 = ����, 1 = �Ʒ��� )

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0134, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_GetQuickSlot Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	
	SQLINTEGER cb[14]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
						,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};		// 2008-06-19 by dhjin, EP3 - 8 -> 10���� ���� 
	SQUICKSLOT_INFO		QuickSlot0;
	int					AccountUID0		= 0;
	int					CharacterUID0	= 0;
	int					TabIdx0			= 0;
	util::zero(&QuickSlot0, sizeof(SQUICKSLOT_INFO));
	
	SQLBindCol(hstmt, 1, SQL_C_ULONG, &AccountUID0, 0,					&cb[1]);
	SQLBindCol(hstmt, 2, SQL_C_ULONG, &CharacterUID0, 0,				&cb[2]);
	SQLBindCol(hstmt, 3, SQL_C_ULONG, &TabIdx0, 0,						&cb[3]);
	SQLBindCol(hstmt, 4, SQL_C_ULONG, &QuickSlot0.arrItemNum_[0], 0,	&cb[4]);
	SQLBindCol(hstmt, 5, SQL_C_ULONG, &QuickSlot0.arrItemNum_[1], 0,	&cb[5]);
	SQLBindCol(hstmt, 6, SQL_C_ULONG, &QuickSlot0.arrItemNum_[2], 0,	&cb[6]);
	SQLBindCol(hstmt, 7, SQL_C_ULONG, &QuickSlot0.arrItemNum_[3], 0,	&cb[7]);
	SQLBindCol(hstmt, 8, SQL_C_ULONG, &QuickSlot0.arrItemNum_[4], 0,	&cb[8]);
	SQLBindCol(hstmt, 9, SQL_C_ULONG, &QuickSlot0.arrItemNum_[5], 0,	&cb[9]);
	SQLBindCol(hstmt, 10, SQL_C_ULONG, &QuickSlot0.arrItemNum_[6], 0,	&cb[10]);
	SQLBindCol(hstmt, 11, SQL_C_ULONG, &QuickSlot0.arrItemNum_[7], 0,	&cb[11]);
	SQLBindCol(hstmt, 12, SQL_C_ULONG, &QuickSlot0.arrItemNum_[8], 0,	&cb[12]);		// 2008-06-19 by dhjin, EP3 - 8 -> 10���� ���� 
	SQLBindCol(hstmt, 13, SQL_C_ULONG, &QuickSlot0.arrItemNum_[9], 0,	&cb[13]);		// 2008-06-19 by dhjin, EP3 - 8 -> 10���� ���� 

	INIT_MSG_WITH_BUFFER(MSG_FC_STORE_REQUEST_QUICKSLOT_OK,T_FC_STORE_REQUEST_QUICKSLOT_OK,pSQuickSlotOK,SendBuf);
	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA)
	{
		if(util::in_range(0, TabIdx0, QUICKTABCOUNT-1))
		{	
			pSQuickSlotOK->POS[TabIdx0].Delete			= FALSE;
			pSQuickSlotOK->POS[TabIdx0].arrItemNum_[0]	= QuickSlot0.arrItemNum_[0];
			pSQuickSlotOK->POS[TabIdx0].arrItemNum_[1]	= QuickSlot0.arrItemNum_[1];
			pSQuickSlotOK->POS[TabIdx0].arrItemNum_[2]	= QuickSlot0.arrItemNum_[2];
			pSQuickSlotOK->POS[TabIdx0].arrItemNum_[3]	= QuickSlot0.arrItemNum_[3];
			pSQuickSlotOK->POS[TabIdx0].arrItemNum_[4]	= QuickSlot0.arrItemNum_[4];
			pSQuickSlotOK->POS[TabIdx0].arrItemNum_[5]	= QuickSlot0.arrItemNum_[5];
			pSQuickSlotOK->POS[TabIdx0].arrItemNum_[6]	= QuickSlot0.arrItemNum_[6];
			pSQuickSlotOK->POS[TabIdx0].arrItemNum_[7]	= QuickSlot0.arrItemNum_[7];
			pSQuickSlotOK->POS[TabIdx0].arrItemNum_[8]	= QuickSlot0.arrItemNum_[8];		// 2008-06-19 by dhjin, EP3 - 8 -> 10���� ���� 
			pSQuickSlotOK->POS[TabIdx0].arrItemNum_[9]	= QuickSlot0.arrItemNum_[9];		// 2008-06-19 by dhjin, EP3 - 8 -> 10���� ���� 
		}
		TabIdx0		= 0;
		util::zero(&QuickSlot0, sizeof(SQUICKSLOT_INFO));
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	q.pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_STORE_REQUEST_QUICKSLOT_OK));
	util::del(pQPMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_Delete_QuickSlot(FIELD_DB_QUERY q, SQLHSTMT hstmt)	
/// \brief		
/// \author		dhjin
/// \date		2006-09-04 ~ 2006-09-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_Delete_QuickSlot(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/************************************************************************
	---------------------------------------------------------------------
	-- PROCEDURE NAME: [atum_GetQuickSlot]
	-- DESC: 2012-06-19 by jhseol, ��� ������ �´� ������ ���� �� �ֵ��� ����
	---------------------------------------------------------------------
	ALTER PROCEDURE [dbo].[atum_delete_QuickSlot]
		@i_AccountUID                int,
		@i_CharacterUID               int,
		@i_TabIdx                     int
		,@i_SlotPurpose				 int
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
	AS
		DELETE FROM td_QuickSlot 
		WHERE AccountUID = @i_AccountUID AND CharacterUID = @i_CharacterUID AND TabIdx = @i_TabIdx AND SlotPurpose = @i_SlotPurpose
	GO
	************************************************************************/

	QPARAM_DELETE_QUICKSLOT *pQPMsg = (QPARAM_DELETE_QUICKSLOT*)q.pQueryParam;
	if(NULL == q.pFieldIOCPSocket
		|| FALSE == q.pFieldIOCPSocket->IsValidCharacter(FALSE))
	{
		util::del(pQPMsg);
		return;
	}

	// 2012-06-21 by jhseol, �Ʒ��� �߰�����part2 - �Ʒ��� ���� ������ ( 0 = ����, 1 = �Ʒ��� )
	int SlotPurpose = 0;
#ifdef S_ARENA_EX_1ST_JHSEOL
	// 2012-09-14 by jhseol, �Ʒ��� �߰����� restore - ������ ���� ���¿��� �ڽ��� �ɸ��ͷθ� �÷��� �ϵ��� ����
#ifndef S_ARENA_EX_1ST_RESTORE_JHSEOL
	SlotPurpose = pQPMsg->SlotPurpose;
#endif	//#ifndef S_ARENA_EX_1ST_RESTORE_JHSEOL
	// end 2012-09-14 by jhseol, �Ʒ��� �߰����� restore - ������ ���� ���¿��� �ڽ��� �ɸ��ͷθ� �÷��� �ϵ��� ����
#endif
	// end 2012-06-21 by jhseol, �Ʒ��� �߰�����part2 - �Ʒ��� ���� ������ ( 0 = ����, 1 = �Ʒ��� )
	
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->AccountUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->CharacterUID, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->TabIdx, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &SlotPurpose, 0, NULL);		// 2012-06-21 by jhseol, �Ʒ��� �߰�����part2 - �Ʒ��� ���� ������

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0135, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_delete_QuickSlot Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}	
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_Update_QuickSlot(FIELD_DB_QUERY q, SQLHSTMT hstmt)	
/// \brief		
/// \author		dhjin
/// \date		2006-09-04 ~ 2006-09-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_Update_QuickSlot(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/************************************************************************
	---------------------------------------------------------------------
	-- PROCEDURE NAME: [atum_GetQuickSlot]
	-- DESC: 2012-06-19 by jhseol, ��� ������ �´� ������ ������Ʈ �� �� �ֵ��� ����
	---------------------------------------------------------------------
	ALTER PROCEDURE [dbo].[atum_Update_QuickSlot]
		@i_AccountUID                int,
		@i_CharacterUID               int,
		@i_TabIdx                     int,
		@i_ItemNum_1                  int,
		@i_ItemNum_2                  int,
		@i_ItemNum_3                  int,
		@i_ItemNum_4                  int,
		@i_ItemNum_5                  int,
		@i_ItemNum_6                  int,
		@i_ItemNum_7                  int,
		@i_ItemNum_8                  int,		--// 2008-06-19 by dhjin, EP3 - 8 -> 10���� ���� 
		@i_ItemNum_9                  int,		--// 2008-06-19 by dhjin, EP3 - 8 -> 10���� ���� 
		@i_ItemNum_10                 int		--// 2008-06-19 by dhjin, EP3 - 8 -> 10���� ���� 
		,@i_SlotPurpose				  int
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
	AS
	
	DECLARE @CheckNum  INT
	SET @CheckNum = (SELECT TabIdx FROM td_QuickSlot 
		WHERE AccountUID = @i_AccountUID AND CharacterUID = @i_CharacterUID 
			AND TabIdx = @i_TabIdx AND SlotPurpose = @i_SlotPurpose)
	IF (@CheckNum IS NOT NULL)
	BEGIN
		UPDATE td_QuickSlot
		SET ItemNum_1 = @i_ItemNum_1, ItemNum_2 = @i_ItemNum_2, 
		ItemNum_3 = @i_ItemNum_3, ItemNum_4 = @i_ItemNum_4, 
		ItemNum_5 = @i_ItemNum_5, ItemNum_6 = @i_ItemNum_6,
		ItemNum_7 = @i_ItemNum_7, ItemNum_8 = @i_ItemNum_8,
		ItemNum_9 = @i_ItemNum_9, ItemNum_10 = @i_ItemNum_10		--// 2008-06-19 by dhjin, EP3 - 8 -> 10���� ���� 
		WHERE AccountUID = @i_AccountUID AND CharacterUID = @i_CharacterUID 
			AND TabIdx = @i_TabIdx AND SlotPurpose = @i_SlotPurpose
	END
	ELSE
	BEGIN
		INSERT INTO td_QuickSlot 
		VALUES(@i_AccountUID, @i_CharacterUID, @i_TabIdx, @i_ItemNum_1,
		 @i_ItemNum_2, @i_ItemNum_3, @i_ItemNum_4, @i_ItemNum_5,
			@i_ItemNum_6, @i_ItemNum_7, @i_ItemNum_8, @i_ItemNum_9, @i_ItemNum_10, @i_SlotPurpose)		--// 2008-06-19 by dhjin, EP3 - 8 -> 10���� ���� 
	END
	************************************************************************/

	QPARAM_UPDATE_QUICKSLOT *pQPMsg = (QPARAM_UPDATE_QUICKSLOT*)q.pQueryParam;
	// 2013-07-29 by jhseol, �Ʒ��� ���Խ� ������ ���� ���ϴ� ���� ���� - �ּ�ó��
// 	if(NULL == q.pFieldIOCPSocket
// 		|| FALSE == q.pFieldIOCPSocket->IsValidCharacter(FALSE))
// 	{
// 		util::del(pQPMsg);
// 		return;
// 	}
	// end 2013-07-29 by jhseol, �Ʒ��� ���Խ� ������ ���� ���ϴ� ���� ���� - �ּ�ó��

	// 2012-06-21 by jhseol, �Ʒ��� �߰�����part2 - �Ʒ��� ���� ������ ( 0 = ����, 1 = �Ʒ��� )
	int SlotPurpose = 0;
#ifdef S_ARENA_EX_1ST_JHSEOL
	// 2012-09-14 by jhseol, �Ʒ��� �߰����� restore - ������ ���� ���¿��� �ڽ��� �ɸ��ͷθ� �÷��� �ϵ��� ����
#ifndef S_ARENA_EX_1ST_RESTORE_JHSEOL
	SlotPurpose = pQPMsg->SlotPurpose;
#endif	//#ifndef S_ARENA_EX_1ST_RESTORE_JHSEOL
	// end 2012-09-14 by jhseol, �Ʒ��� �߰����� restore - ������ ���� ���¿��� �ڽ��� �ɸ��ͷθ� �÷��� �ϵ��� ����
#endif
	// end 2012-06-21 by jhseol, �Ʒ��� �߰�����part2 - �Ʒ��� ���� ������ ( 0 = ����, 1 = �Ʒ��� )

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->AccountUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->CharacterUID, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->TabIdx, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->arrItemNum_[0], 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->arrItemNum_[1], 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->arrItemNum_[2], 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->arrItemNum_[3], 0, NULL);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->arrItemNum_[4], 0, NULL);
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->arrItemNum_[5], 0, NULL);
	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->arrItemNum_[6], 0, NULL);
	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->arrItemNum_[7], 0, NULL);
	SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->arrItemNum_[8], 0, NULL); 	// 2008-06-19 by dhjin, EP3 - 8 -> 10���� ���� 
	SQLBindParameter(hstmt, 13, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->arrItemNum_[9], 0, NULL);	// 2008-06-19 by dhjin, EP3 - 8 -> 10���� ���� 
	SQLBindParameter(hstmt, 14, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &SlotPurpose, 0, NULL);	// 2012-06-21 by jhseol, �Ʒ��� �߰�����part2 - �Ʒ��� ���� ������

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0136, SQL_NTS);	// 2008-06-19 by dhjin, EP3 - 8 -> 10���� ���� 
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Update_QuickSlot Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}	
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_LoadStrategyPointSummonInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		1. DB���� �� �ֱ⿡ ���� ������ �ε��Ѵ�. (td_StrategyPointSummonRange)
///				2. ��ȯ üũ ����(StrategyPointSummonCheck)�� TRUE�̸� ��������Ʈ ���� ������ �����Ѵ�.
///				3. ��������Ʈ ���� �� �� ���� td_StrategyPointSummonInfo�� ti_StrategyPointSummonMapIndex�� MapIndex�� �����ϰ� �Ѵ�.
///				4. �� �ֱ⿡ EndTime�� �������� ������ ���� ������Ʈ �ؾ��Ѵ�.
///             5. �����ؾ� �� ��������Ʈ ������ ���´�. (td_StrategyPointSummonInfo)
///				6. ���Ϻ� ���� �����Ǵ� �ð��� ���´�. (td_StrategyPointNotSummonTime)
///				7. ��������Ʈ ��ȯ �ð��� �����ϰ� �����Ѵ�.
/// \author		dhjin
/// \date		2007-03-12 ~ 2007-03-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_LoadStrategyPointSummonInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	// 2009-10-26 by cmkwon, ���� �ε��� �α� �߰� - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CAtumFieldDBManager::QP_LoadStrategyPointSummonInfo# 11000 \r\n");

	// 1. DB���� �� �ֱ⿡ ���� ������ �ε��Ѵ�. (td_StrategyPointSummonRange)	
	this->QP_LoadStrategyPointSummonRange(q, hstmt);
	
	// 2009-10-26 by cmkwon, ���� �ε��� �α� �߰� - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CAtumFieldDBManager::QP_LoadStrategyPointSummonInfo# 12000 \r\n");

	// 2. ��ȯ üũ ����(StrategyPointSummonCheck)�� TRUE�̸� ��������Ʈ ���� ������ �����Ѵ�.
	if(FALSE == m_pFieldIOCP8->m_InflWarManager.CheckStrategyPointSummon())
	{// 2007-03-02 by dhjin, ��ȯ���� �ʴ´ٸ� �ؿ��� �������� �ʴ´�.
		return;
	}

	// 2009-10-26 by cmkwon, ���� �ε��� �α� �߰� - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CAtumFieldDBManager::QP_LoadStrategyPointSummonInfo# 13000 \r\n");

	// 3. ��������Ʈ ���� �� �� ���� td_StrategyPointSummonInfo�� ti_StrategyPointSummonMapIndex�� MapIndex�� �����ϰ� �Ѵ�.
	/************************************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_UpdateStrategyPointSummonInfoByMapIndex
	-- DESC      : �����ؾ� �� ��������Ʈ �� ������ ti_StrategyPointSummonMapIndex�� �����ϰ� �ϴ� ���ν���.
	-- 2007-02-26 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_UpdateStrategyPointSummonInfoByMapIndex
	AS
		DELETE dbo.td_StrategyPointSummonInfo WHERE MapIndex NOT IN (SELECT MapIndex FROM atum2_db_account.dbo.ti_StrategyPointSummonMapIndex)
		INSERT INTO dbo.td_StrategyPointSummonInfo 
			SELECT MapIndex, 0, GetDATE(), NULL		-- // 2008-04-08 by dhjin, ��ȯ ���� �ð� ���� - 
			FROM atum2_db_account.dbo.ti_StrategyPointSummonMapIndex
			WHERE MapIndex NOT IN (SELECT MapIndex FROM dbo.td_StrategyPointSummonInfo)
	GO
	************************************************************************/
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0137, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateStrategyPointSummonInfoByMapIndex Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	
	// 2009-10-26 by cmkwon, ���� �ε��� �α� �߰� - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CAtumFieldDBManager::QP_LoadStrategyPointSummonInfo# 14000 \r\n");

	// 4. �� �ֱ⿡ EndTime�� �������� ������ ���� ������Ʈ �ؾ��Ѵ�.
	if(TRUE == m_pFieldIOCP8->m_InflWarManager.CompareStrategyPointSummonTime())
	{// 2007-03-02 by dhjin, �ֱ⸦ ���� ��� ���� ������Ʈ�� �����Ѵ�.
		this->QP_UpdateStrategyPointSummonInfoByNewSummonRange(q, hstmt);
		this->QP_LoadStrategyPointSummonRange(q, hstmt);
	}

	// 2009-10-26 by cmkwon, ���� �ε��� �α� �߰� - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CAtumFieldDBManager::QP_LoadStrategyPointSummonInfo# 15000 \r\n");

	// 5. �����ؾ� �� ��������Ʈ ������ ���´�. (td_StrategyPointSummonInfo)
	/************************************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_LoadStrategyPointSummonInfo
	-- DESC      : �����ؾ� �� ��������Ʈ ������ ������ ���ν���.
	-- 2007-02-26 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_LoadStrategyPointSummonInfo
	AS
		SELECT MapIndex, SummonCount, SummonTime, SummonAttribute FROM dbo.td_StrategyPointSummonInfo WITH (NOLOCK)
	GO
	************************************************************************/	
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0138, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_LoadStrategyPointSummonInfo Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
 
	
	SQLINTEGER	arrCB[5] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};		// 2008-04-08 by dhjin, ��ȯ ���� �ð� ���� - 
	SSTRATEGYPOINT_SUMMONTIME_INFO	tmStrategyPointSummonTimeInfo;
	vectSSTRATEGYPOINT_SUMMONTIME_INFO	tmvectStrategyPointSummonTimeInfo;
	util::zero(&tmStrategyPointSummonTimeInfo, sizeof(SSTRATEGYPOINT_SUMMONTIME_INFO));
	tmvectStrategyPointSummonTimeInfo.clear();
	SQL_TIMESTAMP_STRUCT SummonTime;
	
	SQLBindCol(hstmt, 1, SQL_C_LONG, &tmStrategyPointSummonTimeInfo.MapIndex, 0, &arrCB[1]);
	SQLBindCol(hstmt, 2, SQL_C_UTINYINT, &tmStrategyPointSummonTimeInfo.SummonCount, 0, &arrCB[2]);
	SQLBindCol(hstmt, 3, SQL_C_TIMESTAMP, &SummonTime, 0, &arrCB[3]);
	SQLBindCol(hstmt, 4, SQL_C_UTINYINT, &tmStrategyPointSummonTimeInfo.SummonAttribute, 0, &arrCB[4]);	// 2008-04-08 by dhjin, ��ȯ ���� �ð� ���� - 
	
	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA)
	{
		tmStrategyPointSummonTimeInfo.SummonTime	= SummonTime;
		tmvectStrategyPointSummonTimeInfo.push_back(tmStrategyPointSummonTimeInfo);
		util::zero(&tmStrategyPointSummonTimeInfo, sizeof(SSTRATEGYPOINT_SUMMONTIME_INFO));
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	// 2009-10-26 by cmkwon, ���� �ε��� �α� �߰� - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CAtumFieldDBManager::QP_LoadStrategyPointSummonInfo# 16000 \r\n");

	m_pFieldIOCP8->m_InflWarManager.InitStrategyPointSummonTimeInfo(&tmvectStrategyPointSummonTimeInfo);
	
	// 2009-10-26 by cmkwon, ���� �ε��� �α� �߰� - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CAtumFieldDBManager::QP_LoadStrategyPointSummonInfo# 17000 \r\n");

	// 6. ���Ϻ� ���� �����Ǵ� �ð��� ���´�. (td_StrategyPointNotSummonTime)
	this->QP_LoadStrategyPointNotSummonTime(q, hstmt);
	
	// 2009-10-26 by cmkwon, ���� �ε��� �α� �߰� - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CAtumFieldDBManager::QP_LoadStrategyPointSummonInfo# 18000 \r\n");

	// 7. ��������Ʈ ��ȯ �ð��� �����ϰ� �����Ѵ�.
// 2009-03-23 by dhjin, ��������Ʈ �ð� ���� ���� - ��������Ʈ ��ȯ ������ �ð��� ���� ����
//	m_pFieldIOCP8->m_InflWarManager.SetAllStrategyPointSummonTime();
	m_pFieldIOCP8->m_InflWarManager.SetAllStrategyPointSummonTimeNew();

	// 2009-10-26 by cmkwon, ���� �ε��� �α� �߰� - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CAtumFieldDBManager::QP_LoadStrategyPointSummonInfo# 19000 \r\n");
}

void CAtumFieldDBManager::QP_UpdateStrategyPointSummonInfoBySummon(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/************************************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_UpdateStrategyPointSummonInfoBySummon
	-- DESC      : ������ ��������Ʈ ���� ���ν���.
	-- 2007-02-26 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_UpdateStrategyPointSummonInfoBySummon
		@i_MapIndex			INT,
		@i_SummonCount		TINYINT
	AS
		UPDATE dbo.td_StrategyPointSummonInfo SET SummonCount = @i_SummonCount WHERE MapIndex = @i_MapIndex
	GO
	************************************************************************/

	QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_COUNT *pQPMsg = (QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_COUNT*)q.pQueryParam;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pQPMsg->MapIndex, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQPMsg->SummonCount, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0139, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateStrategyPointSummonInfoBySummon Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);	
}

void CAtumFieldDBManager::QP_UpdateStrategyPointSummonInfoBySummonTime(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/************************************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_UpdateStrategyPointSummonInfoBySummonTime
	-- DESC      : ��������Ʈ ���� �ð� ���� ���ν���.
	-- 2007-02-26 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_UpdateStrategyPointSummonInfoBySummonTime
		@i_MapIndex			INT,
		@i_SummonTime		VARCHAR(30),
		@i_SummonAttribute	TINYINT		-- // 2008-04-08 by dhjin, ��ȯ ���� �ð� ���� - 
	AS
		UPDATE dbo.td_StrategyPointSummonInfo SET SummonTime = @i_SummonTime, SummonAttribute = @i_SummonAttribute WHERE MapIndex = @i_MapIndex
	GO
	************************************************************************/
	QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME *pQPMsg = (QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME*)q.pQueryParam;

	char tmpSummonTimeBuf[SIZE_MAX_SQL_DATETIME_STRING];
	pQPMsg->SummonTime.GetSQLDateTimeString(tmpSummonTimeBuf, SIZE_MAX_SQL_DATETIME_STRING);

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pQPMsg->MapIndex, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpSummonTimeBuf, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQPMsg->SummonAttribute, 0, NULL);	// 2008-04-08 by dhjin, ��ȯ ���� �ð� ���� - 

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0140, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateStrategyPointSummonInfoBySummonTime Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);
}

void CAtumFieldDBManager::QP_UpdateStrategyPointSummonInfoByNewSummonRange(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/************************************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_UpdateStrategyPointSummonInfoByNewSummonRange
	-- DESC      : ������ ��������Ʈ �ð� �� ���� ���ν���.
	-- 2007-02-26 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_UpdateStrategyPointSummonInfoByNewSummonRange
	AS
		UPDATE dbo.td_StrategyPointSummonInfo SET SummonCount = SummonCount + 1
		DECLARE @tmNewSTime	DATETIME
		SET @tmNewSTime = (SELECT EndTime FROM dbo.td_StrategyPointSummonRangeTimeInfo)
		DECLARE @tmSummonRange SMALLINT
		SET @tmSummonRange = (SELECT SummonRange FROM atum2_db_account.dbo.td_StrategyPointSummonRange)
		DECLARE @tmStrategyPointSummonCheck TINYINT
		SET @tmStrategyPointSummonCheck = (SELECT StrategyPointSummonCheck FROM atum2_db_account.dbo.td_StrategyPointSummonRange)
		DECLARE @tmMaxSummonCountDaily TINYINT
		SET @tmMaxSummonCountDaily = (SELECT MaxSummonCountDaily FROM atum2_db_account.dbo.td_StrategyPointSummonRange)
		UPDATE dbo.td_StrategyPointSummonRangeTimeInfo SET StartTime = @tmNewSTime, EndTime = DateAdd(day, @tmSummonRange, EndTime), 
			   SummonRange = @tmSummonRange, StrategyPointSummonCheck = @tmStrategyPointSummonCheck, MaxSummonCountDaily = @tmMaxSummonCountDaily
	GO
	************************************************************************/
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0141, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateStrategyPointSummonInfoByNewSummonRange Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
}

void CAtumFieldDBManager::QP_LoadStrategyPointSummonRange(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_LoadStrategyPointSummonRangeTimeInfo
	-- DESC      : �����ؾ� �� ��������Ʈ ���� �ֱ� �ð� ������ ������ ���ν���.
	-- 2007-03-29 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_LoadStrategyPointSummonRangeTimeInfo
	AS
		SELECT StartTime, EndTime, SummonRange, StrategyPointSummonCheck, MaxSummonCountDaily FROM dbo.td_StrategyPointSummonRangeTimeInfo WITH (NOLOCK)
	GO
	**************************************************************************/
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0020, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_LoadStrategyPointSummonRangeTimeInfo Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	SQLINTEGER	arrCB[6] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	SSTRATEGYPOINT_SUMMON_RANGE		tmSStrategyPointSummonRange;
	util::zero(&tmSStrategyPointSummonRange, sizeof(SSTRATEGYPOINT_SUMMON_RANGE));

	SQL_TIMESTAMP_STRUCT	stTime;
	SQL_TIMESTAMP_STRUCT	EndTime;

	SQLBindCol(hstmt, 1, SQL_C_TIMESTAMP, &stTime, 0, &arrCB[1]);
	SQLBindCol(hstmt, 2, SQL_C_TIMESTAMP, &EndTime, 0, &arrCB[2]);
	SQLBindCol(hstmt, 3, SQL_C_ULONG, &tmSStrategyPointSummonRange.SummonRange, 0, &arrCB[3]);
	SQLBindCol(hstmt, 4, SQL_C_TINYINT, &tmSStrategyPointSummonRange.StrategyPointSummonCheck, 0, &arrCB[4]);
	SQLBindCol(hstmt, 5, SQL_C_TINYINT, &tmSStrategyPointSummonRange.MaxSummonCountDaily, 0, &arrCB[5]);

	ret = SQLFetch(hstmt);

	tmSStrategyPointSummonRange.StartTime	= stTime;
	tmSStrategyPointSummonRange.EndTime		= EndTime;

	SQLFreeStmt(hstmt, SQL_CLOSE);	

	m_pFieldIOCP8->m_InflWarManager.InitStrategyPointSummonRange(&tmSStrategyPointSummonRange);

}

void CAtumFieldDBManager::QP_LoadStrategyPointNotSummonTime(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/************************************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_LoadStrategyPointNotSummonTime
	-- DESC      : ��������Ʈ �����Ǹ� �ȵǴ� �ð� �������� ���ν���
	-- 2007-02-26 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_LoadStrategyPointNotSummonTime
	AS
		SELECT DayOfWeek, StartTime, EndTime, CheckSummon FROM atum2_db_account.dbo.td_StrategyPointNotSummonTime
	GO
	************************************************************************/
	
	RETCODE	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0022, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_LoadStrategyPointNotSummonTime Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	
	SQLINTEGER	arrCB[5] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};	// 2008-04-04 by dhjin, ��ȯ ���� �ð� ���� - 
	SSTRATEGYPOINT_NOT_SUMMON_TIME		tmSStrategyPointNotSummonTime;
	vectSSTRATEGYPOINT_NOT_SUMMON_TIME	tmvectSStrategyPointNotSummonTime;
	util::zero(&tmSStrategyPointNotSummonTime, sizeof(SSTRATEGYPOINT_NOT_SUMMON_TIME));
	SQL_TIMESTAMP_STRUCT	stTime;
	SQL_TIMESTAMP_STRUCT	EndTime;

	SQLBindCol(hstmt, 1, SQL_C_ULONG, &tmSStrategyPointNotSummonTime.DayOfWeek, 0, &arrCB[1]);
	SQLBindCol(hstmt, 2, SQL_C_TIMESTAMP, &stTime, 0, &arrCB[2]);
	SQLBindCol(hstmt, 3, SQL_C_TIMESTAMP, &EndTime, 0, &arrCB[3]);
	SQLBindCol(hstmt, 4, SQL_C_TINYINT, &tmSStrategyPointNotSummonTime.CheckSummon, 0, &arrCB[4]);	// 2008-04-04 by dhjin, ��ȯ ���� �ð� ���� - 

	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA)
	{
		tmSStrategyPointNotSummonTime.StartTime		= stTime;
		tmSStrategyPointNotSummonTime.EndTime		= EndTime;
		tmvectSStrategyPointNotSummonTime.push_back(tmSStrategyPointNotSummonTime);
		util::zero(&tmSStrategyPointNotSummonTime, sizeof(SSTRATEGYPOINT_NOT_SUMMON_TIME));
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);		

	m_pFieldIOCP8->m_InflWarManager.InitStrategyPointNotSummonTime(&tmvectSStrategyPointNotSummonTime);

}

void CAtumFieldDBManager::QP_LoadStrategyPointNotSummonTimeByAdminTool(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	this->QP_LoadStrategyPointNotSummonTime(q, hstmt);

// 2009-03-23 by dhjin, ��������Ʈ �ð� ���� ���� - ��������Ʈ ��ȯ ������ �ð��� ���� ����
//	m_pFieldIOCP8->m_InflWarManager.SetAllStrategyPointSummonTime();
	m_pFieldIOCP8->m_InflWarManager.SetAllStrategyPointSummonTimeNew();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_UpdateWarPoint(FIELD_DB_QUERY q, SQLHSTMT hstmt)	
/// \brief		
/// \author		dhjin
/// \date		2007-04-25 ~ 2007-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_UpdateWarPoint(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/************************************************************************
	CREATE PROCEDURE dbo.atum_UpdateCharacterWarPoint
		@i_CharacterUID		INT,
		@i_AccountUID		INT,
		@i_WP				INT,
		@i_CumulativeWP		INT
	AS
		UPDATE dbo.td_Character
			SET WarPoint = @i_WP, CumulativeWarPoint = @i_CumulativeWP
			WHERE UniqueNumber = @i_CharacterUID AND AccountUniqueNumber = @i_AccountUID
	GO
	************************************************************************/
	QPARAM_UPDATE_WARPOINT *pQPMsg = (QPARAM_UPDATE_WARPOINT*)q.pQueryParam;
	
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->CharacterUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->AccountUID, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->TotalWarPoint, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->CumulativeWarPoint, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0144, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateCharacterWarPoint Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	util::del(pQPMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_UpdateArenaResult(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		�Ʒ��� ��� ������Ʈ
/// \author		dhjin
/// \date		2007-06-07 ~ 2007-06-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_UpdateArenaResult(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/************************************************************************
	CREATE PROCEDURE dbo.atum_UpdateCharacterArenaResult
		@i_CharacterUID		INT,
		@i_AccountUID		INT,
		@i_WP				INT,
		@i_CumulativeWP		INT,
		@i_ArenaWin			INT,
		@i_ArenaLose		INT
	AS
		UPDATE dbo.td_Character
			SET WarPoint = @i_WP, CumulativeWarPoint = @i_CumulativeWP, ArenaWin = @i_ArenaWin, ArenaLose = @i_ArenaLose
			WHERE UniqueNumber = @i_CharacterUID AND AccountUniqueNumber = @i_AccountUID
	GO
	************************************************************************/
	QPARAM_UPDATE_ARENA_RESULT *pQPMsg = (QPARAM_UPDATE_ARENA_RESULT*)q.pQueryParam;
	
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->CharacterUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->AccountUID, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->TotalWarPoint, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->CumulativeWarPoint, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->ArenaWin, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->ArenaLose, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0145, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateCharacterArenaResult Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	util::del(pQPMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_UpdateArenaDisConnect(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		�Ʒ��� �������� ������Ʈ
/// \author		dhjin
/// \date		2007-06-07 ~ 2007-06-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_UpdateArenaDisConnect(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/************************************************************************
	CREATE PROCEDURE dbo.atum_UpdateCharacterArenaDisConnect
		@i_CharacterUID		INT,
		@i_AccountUID		INT,
		@i_ArenaDisConnect	INT
	AS
		UPDATE dbo.td_Character
			SET ArenaDisConnect = @i_ArenaDisConnect
			WHERE UniqueNumber = @i_CharacterUID AND AccountUniqueNumber = @i_AccountUID
	GO
	************************************************************************/
	QPARAM_UPDATE_ARENA_DISCONNECT *pQPMsg = (QPARAM_UPDATE_ARENA_DISCONNECT*)q.pQueryParam;
	
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->CharacterUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->AccountUID, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->ArenaDisConnect, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0146, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateCharacterArenaDisConnect Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);
}

/////////////////////////////////////////////////////////////
// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : AS���� ���� CharacterArena ���� ������Ʈ
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_MF_Updata_CharacterArena(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		�����ʵ� �Ʒ��� ���� ������Ʈ
/// \author		jhseol
/// \date		2012-04-12 ~ 2012-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_MF_Updata_CharacterArena(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/************************************************************************
	************************************************************************/
	QPARAM_UPDATE_ARENA_CHARACTERARENA *pQPMsg = (QPARAM_UPDATE_ARENA_CHARACTERARENA*)q.pQueryParam;
	SQL_TIMESTAMP_STRUCT LastPlayTime;				/// DB�� ����� ���� ĳ������ �ֱ� �Ʒ��� �÷��� �ð�
	util::zero(&LastPlayTime, sizeof(SQL_TIMESTAMP_STRUCT));
	LastPlayTime.year	= pQPMsg->ArenaInfo.atimeLastPlayTime.Year;
	LastPlayTime.month	= pQPMsg->ArenaInfo.atimeLastPlayTime.Month;
	LastPlayTime.day	= pQPMsg->ArenaInfo.atimeLastPlayTime.Day;
	LastPlayTime.hour	= pQPMsg->ArenaInfo.atimeLastPlayTime.Hour;
	LastPlayTime.minute = pQPMsg->ArenaInfo.atimeLastPlayTime.Minute;
	LastPlayTime.second = pQPMsg->ArenaInfo.atimeLastPlayTime.Second;
	/************************************************************************
	---------------------------------------------------------------------
	-- PROCEDURE NAME: dbo.atum_arena_CharacterArena_Updata(?,?,?,?)
	-- DESC: 2012-04-12 by jhseol, td_CharacterArena ������ ������Ʈ ���ν���
	---------------------------------------------------------------------
	CREATE PROCEDURE [dbo].[atum_arena_CharacterArena_Updata]
		@i_CharacterUID				INT,
		@i_AccountUID				INT,
		@i_PlayCount				INT,
		@i_LastPlayTime				DATETIME
	AS
	BEGIN
		UPDATE	[dbo].[td_CharacterArena]
		SET		[PlayCount] = @i_PlayCount, [LastPlayTime] = @i_LastPlayTime
		WHERE	[CharacterUID] = @i_CharacterUID AND [AccountUID] = @i_AccountUID
	END
	GO
	---------------------------------------------------------------------
	[�߰� ������]
	// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : SQLBindParameter(), DATETIME �Ѱ��ִ� ���
	SQL_TIMESTAMP_STRUCT ���� ���� & �ʱ�ȭ
	SQL_TIMESTAMP_STRUCT LastPlayTime;
	LastPlayTime.year     = 2011;
	LastPlayTime.month    = 7;
	LastPlayTime.day      = 1;
	LastPlayTime.hour     = 12;
	LastPlayTime.minute   = 10;
	LastPlayTime.second   = 30;
	LastPlayTime.fraction = 0;            // ����!> ������ 0 ���� �����Ѵ�.
	SQLLEN nParamLen = 0;
	SQLBindParameter(
	hSTMT,                      // ��� �ڵ�
	1,                          // �Ű����� ��ȣ
	SQL_PARAM_INPUT,            // INPUT / OUTPUT / INPUT_OUTPUT
	SQL_C_TYPE_TIMESTAMP,       // [����] �� Ÿ��(C Ÿ�� ID)
	SQL_TYPE_TIMESTAMP,         // [����] �Ķ���� Ÿ��(SQL Ÿ�� ID)
	SQL_TIMESTAMP_LEN,          // [����] �Ķ���� ũ��(�⺻��: 19, 'yyyy-mm-dd hh:mm:ss')
	3,                          // [����] �ð� ������ �� �и����� �ڸ���
	&tmTime,                    // �Ķ���Ϳ� ����� ���� ������
	sizeof(TIMESTAMP_STRUCT),   // [����] �Ķ���Ϳ� ����� ������ ũ��
	&nParamLen                  // ��ȯ��(����, ����)�� ����� ���� ������ (��������)
	);
	************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->CharacterUID,			0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->AccountUID,			0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->ArenaInfo.nPlayCount,	0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP, SQL_TIMESTAMP_LEN, 3, &LastPlayTime,	sizeof(SQL_TIMESTAMP_STRUCT), NULL);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_120412_0003, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_arena_CharacterArena_Updata Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);
}
// end 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : AS���� ���� CharacterArena ���� ������Ʈ

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_LoadTutorialComplete(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		������ Tutorial ���� ������
/// \author		dhjin
/// \date		2007-07-06 ~ 2007-07-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_LoadTutorialComplete(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// 2007-07-06 by dhjin, QT_LoadTutorialComplete
	/************************************************************************
	CREATE PROCEDURE dbo.atum_LoadTutorialComplete
		@i_CharacterUID          INT
	AS
		SELECT TutorialUID FROM dbo.td_TutorialComplete 
		WHERE CharacterUID = @i_CharacterUID
		ORDER BY TutorialUID DESC
	GO
	************************************************************************/
	if (q.pFieldIOCPSocket == NULL || !q.pFieldIOCPSocket->IsUsing()
		|| !q.pFieldIOCPSocket->IsValidCharacter(FALSE))
	{
		return;
	}

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &q.pFieldIOCPSocket->m_character.CharacterUniqueNumber, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0147, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_LoadTutorialComplete Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	SQLINTEGER	arrCB[2] = {SQL_NTS, SQL_NTS};
	vectTutorial		tmvectSTutorialCompleteInfo;
	tmvectSTutorialCompleteInfo.clear();
	INT			TutorialCompleteNum;

	SQLBindCol(hstmt, 1, SQL_C_LONG, &TutorialCompleteNum, 0, &arrCB[1]);

	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA)
	{
		tmvectSTutorialCompleteInfo.push_back(TutorialCompleteNum);
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	// 2007-07-06 by dhjin, Tutorial ���� �����
	q.pFieldIOCPSocket->InitTutorial(&tmvectSTutorialCompleteInfo);

	// 2007-07-06 by dhjin, �޼��� �����
	INIT_MSG_WITH_BUFFER(MSG_FC_TUTORIAL_START_OK, T_FC_TUTORIAL_START_OK, pSTutorialList, SendBuf);
	pSTutorialList->TutorialInfoListCount	= q.pFieldIOCPSocket->MakeMSG_FC_TUTORIAL_START_OK((STUTORIAL_LIST_INFO*)(SendBuf+MSG_SIZE(MSG_FC_TUTORIAL_START_OK)));
	if(0 > pSTutorialList->TutorialInfoListCount)
	{
		return;
	}
	
	// 2007-07-06 by dhjin, ���� ���� 
	q.pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_TUTORIAL_START_OK)+pSTutorialList->TutorialInfoListCount*sizeof(STUTORIAL_LIST_INFO));

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_InsertTutorialComplete(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		������ Tutorial �Ϸ� ���� ����
/// \author		dhjin
/// \date		2007-07-06 ~ 2007-07-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_InsertTutorialComplete(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// 2007-07-06 by dhjin, QT_InsertTutorialComplete
	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME: dbo.atum_InsertTutorialComplete
	-- DESC			 : 
	-- 2007-06-28 by dhjin,
	--					// 2007-11-16 by cmkwon, ���̺� Unique Key ������ �� ������ �߰��� üũ �Ŀ� �߰� - td_tutorialComplete
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_InsertTutorialComplete
		@i_CharacterUID          INT,
		@i_TutorialUID           INT
	AS
		DECLARE @TutorialUID INT
		SET @TutorialUID = (SELECT TutorialUID FROM dbo.td_TutorialComplete WITH(NOLOCK) WHERE CharacterUID = @i_CharacterUID AND TutorialUID = @i_TutorialUID)

		IF @TutorialUID IS NULL
		BEGIN
			INSERT INTO dbo.td_TutorialComplete VALUES(@i_CharacterUID, @i_TutorialUID, GETDATE())
		END
		ELSE
		BEGIN
			UPDATE dbo.td_TutorialComplete
			SET TutorialCompleteTime = GETDATE()
			WHERE CharacterUID = @i_CharacterUID AND TutorialUID = @i_TutorialUID		
		END
	GO
	************************************************************************/
	QPARAM_INSERT_TUTORIAL_COMPLETE *pQPMsg = (QPARAM_INSERT_TUTORIAL_COMPLETE*)q.pQueryParam;
	
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->CharacterUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->TutorialUID, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0148, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_InsertTutorialComplete Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 2007-08-21 by dhjin, OutPost

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_SetNPCPossessionOutPost(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		�������� NPC������ ����
/// \author		dhjin
/// \date		2007-08-21 ~ 2007-08-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_SetNPCPossessionOutPost(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_SetNPCPossessionOutPost
	/************************************************************************
	CREATE PROCEDURE dbo.atum_SetNPCPossessionOutPost
		@i_Mapindex					INT,
		@i_GetTime					DATETIME,
		@i_NextWarTime				DATETIME
	AS
		UPDATE td_OutPostInfo SET OutPostGetTime = @i_GetTime, OutPostNextWarTime = @i_NextWarTime, OutPostNextWarTimeSet = 1, OutPostGetInfl = 0, OutPostGuildUID = 0
		WHERE OutPostMapIndex = @i_Mapindex
	GO
	************************************************************************/
	QPARAM_SET_NPCPOSSESSION_OUTPOST *pQPMsg = (QPARAM_SET_NPCPOSSESSION_OUTPOST*)q.pQueryParam;
	
	char szGetTime[SIZE_MAX_SQL_DATETIME_STRING];
	char szNextWarTime[SIZE_MAX_SQL_DATETIME_STRING];

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->MapIndex, 0, NULL);
	pQPMsg->GetTime.GetSQLDateTimeString(szGetTime, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szGetTime, 0, NULL);
	pQPMsg->NextWarTime.GetSQLDateTimeString(szNextWarTime, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szNextWarTime, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0149, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_SetNPCPossessionOutPost Failed!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_SetGuildPossessionOutPost(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		�������� ���� ������ ����
/// \author		dhjin
/// \date		2007-08-21 ~ 2007-08-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_SetGuildPossessionOutPost(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_SetGuildPossessionOutPost
	/************************************************************************
	CREATE PROCEDURE dbo.atum_SetGuildPossessionOutPost
		@i_Mapindex					INT,
		@i_Infl						TINYINT,
		@i_GuildUID					INT,
		@i_GetTime					DATETIME,
		@i_NextWarTime				DATETIME
	AS
		UPDATE td_OutPostInfo 
		SET OutPostGetTime = @i_GetTime, OutPostNextWarTime = @i_NextWarTime, OutPostNextWarTimeSet = 0
							, OutPostGetInfl = @i_Infl, OutPostGuildUID = @i_GuildUID
		WHERE OutPostMapIndex = @i_Mapindex
	GO
	************************************************************************/
	QPARAM_SET_GUILDPOSSESSION_OUTPOST *pQPMsg = (QPARAM_SET_GUILDPOSSESSION_OUTPOST*)q.pQueryParam;
	
	char szGetTime[SIZE_MAX_SQL_DATETIME_STRING];
	char szNextWarTime[SIZE_MAX_SQL_DATETIME_STRING];

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->MapIndex, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQPMsg->Influence, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->GuildUID, 0, NULL);
	pQPMsg->GetTime.GetSQLDateTimeString(szGetTime, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szGetTime, 0, NULL);
	pQPMsg->NextWarTime.GetSQLDateTimeString(szNextWarTime, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szNextWarTime, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0150, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_SetGuildPossessionOutPost Failed!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_SetOutPostNextWarTime(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		���� ���������� �ð� ����
/// \author		dhjin
/// \date		2007-08-21 ~ 2007-08-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_SetOutPostNextWarTime(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_SetOutPostNextWarTime
	/************************************************************************
	CREATE PROCEDURE dbo.atum_SetOutPostNextWarTime
		@i_Mapindex					INT,
		@i_Infl						TINYINT,
		@i_GuildUID					INT,
		@i_NextWarTime				DATETIME
	AS
		UPDATE td_OutPostInfo 
		SET OutPostNextWarTime = @i_NextWarTime, OutPostNextWarTimeSet = 1
		WHERE OutPostCityMapIndex = @i_Mapindex AND OutPostGetInfl = @i_Infl AND OutPostGuildUID = @i_GuildUID
	GO
	************************************************************************/
	QPARAM_SET_OUTPOST_NEXTWAR_TIME *pQPMsg = (QPARAM_SET_OUTPOST_NEXTWAR_TIME*)q.pQueryParam;
	
	char szNextWarTime[SIZE_MAX_SQL_DATETIME_STRING];

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->MapIndex, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQPMsg->Influence, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->GuildUID, 0, NULL);
	pQPMsg->NextWarTime.GetSQLDateTimeString(szNextWarTime, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szNextWarTime, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0151, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_SetOutPostNextWarTime Failed!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_InitExpediencyFund(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		�ǰ��� ���� �߰�
/// \author		dhjin
/// \date		2007-08-27 ~ 2007-08-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_InitExpediencyFund(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_InitExpediencyFund
	/************************************************************************
	CREATE PROCEDURE dbo.atum_InitExpediencyFund
		@i_MapIndex						INT,
		@i_Influence					TINYINT,
		@i_CharacterUID					INT,
		@i_ExpediencyFundCumulative		INT,
		@i_ExpediencyFund				INT,
		@i_ExpediencyFundRate			INT
	AS
		DECLARE @tmCount INT
		SET @tmCount = (SELECT Count(*) FROM td_CityLeaderInfo WHERE MapIndex = @i_MapIndex)

		IF (0 = @tmCount)
		BEGIN
			INSERT INTO td_CityLeaderInfo VALUES(@i_MapIndex, @i_Influence, @i_CharacterUID, @i_ExpediencyFundCumulative
												, @i_ExpediencyFund, @i_ExpediencyFundRate, NULL)		
		END
		IF (1 = @tmCount)
		BEGIN
			UPDATE td_CityLeaderInfo
			SET	Influence = @i_Influence, CharacterUID = @i_CharacterUID, ExpediencyFundCumulative = @i_ExpediencyFundCumulative
				, ExpediencyFund = @i_ExpediencyFund, ExpediencyFundRate = @i_ExpediencyFundRate
			WHERE MapIndex = @i_MapIndex		
		END

	GO
	************************************************************************/
	QPARAM_INIT_EXPEDIENCY_FUND *pQPMsg = (QPARAM_INIT_EXPEDIENCY_FUND*)q.pQueryParam;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->MapIndex, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQPMsg->Influence, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->CharacterUID, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->ExpediencyFundCumulative, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->ExpediencyFund, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->ExpediencyRate, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0152, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_InitExpediencyFund Failed!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_SetExpediencyFund(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		�ǰ��� ���� ����
/// \author		dhjin
/// \date		2007-08-22 ~ 2007-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_SetExpediencyFund(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_SetExpediencyFund
	/************************************************************************
	CREATE PROCEDURE dbo.atum_SetExpediencyFund
		@i_MapIndex						INT,
		@i_ExpediencyFund				INT,
		@i_ExpediencyFundCumulative		INT
	AS
		UPDATE td_CityLeaderInfo
		SET	ExpediencyFundCumulative = @i_ExpediencyFundCumulative, ExpediencyFund = @i_ExpediencyFund
		WHERE MapIndex = @i_MapIndex
	GO
	************************************************************************/
	QPARAM_SET_EXPEDIENCY_FUND *pQPMsg = (QPARAM_SET_EXPEDIENCY_FUND*)q.pQueryParam;
	
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->MapIndex, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->ExpediencyFund, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->ExpediencyFundCumulative, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0153, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_SetExpediencyFund Failed!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_SetExpediencyFundPayBack(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		�ǰ��� ȯ�� ����
/// \author		dhjin
/// \date		2007-08-27 ~ 2007-08-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_SetExpediencyFundPayBack(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_SetExpediencyFundPayBack
	/************************************************************************
	CREATE PROCEDURE dbo.atum_SetExpediencyFundPayBack
		@i_MapIndex						INT,
		@i_Influence					TINYINT,
		@i_CharacterUID					INT,
		@i_ExpediencyFund				INT
	AS
		UPDATE td_CityLeaderInfo
		SET	ExpediencyFund = @i_ExpediencyFund
		WHERE MapIndex = @i_MapIndex AND Influence = @i_Influence AND CharacterUID = @i_CharacterUID
	GO
	************************************************************************/
	QPARAM_SET_EXPEDIENCY_FUNDPAYBACK *pQPMsg = (QPARAM_SET_EXPEDIENCY_FUNDPAYBACK*)q.pQueryParam;
	
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->MapIndex, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQPMsg->Influence, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->CharacterUID, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->ExpediencyFund, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0154, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_SetExpediencyFundPayBack Failed!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_SetExpediencyFundRate(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		�ǰ����� ���� ����
/// \author		dhjin
/// \date		2007-08-22 ~ 2007-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_SetExpediencyFundRate(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_SetExpediencyFundRate
	/************************************************************************
	CREATE PROCEDURE dbo.atum_SetExpediencyFundRate
		@i_MapIndex						INT,
		@i_Influence					TINYINT,
		@i_CharacterUID					INT,
		@i_ExpediencyFundRate			INT
	AS
		UPDATE td_CityLeaderInfo
		SET	ExpediencyFundRate = @i_ExpediencyFundRate
		WHERE MapIndex = @i_MapIndex AND Influence = @i_Influence AND CharacterUID = @i_CharacterUID
	GO
	************************************************************************/
	QPARAM_SET_EXPEDIENCY_FUNDRATE *pQPMsg = (QPARAM_SET_EXPEDIENCY_FUNDRATE*)q.pQueryParam;
	
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->MapIndex, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQPMsg->Influence, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->CharacterUID, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->ExpediencyRate, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0155, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DB Error] QP_SetExpediencyFundRate error !!, MapIdx(%d) Influence(%d) CharacterUID(%d) ExpediencyRate(%d)\r\n"
			, pQPMsg->MapIndex, pQPMsg->Influence, pQPMsg->CharacterUID, pQPMsg->ExpediencyRate);
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_SetExpediencyFundRate Failed!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_DeleteCityLeaderInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		���� ���� ����
/// \author		dhjin
/// \date		2007-08-22 ~ 2007-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_DeleteCityLeaderInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_DeleteCityLeaderInfo
	/************************************************************************
	CREATE PROCEDURE dbo.atum_DeleteCityLeaderInfo
		@i_MapIndex						INT,
		@i_Influence					TINYINT,
		@i_CharacterUID					INT
	AS
		DELETE FROM td_CityLeaderInfo
		WHERE MapIndex = @i_MapIndex AND Influence = @i_Influence AND CharacterUID = @i_CharacterUID
	GO
	************************************************************************/
	QPARAM_DELETE_CITYLEADER_INFO *pQPMsg = (QPARAM_DELETE_CITYLEADER_INFO*)q.pQueryParam;
	
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->MapIndex, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQPMsg->Influence, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->CharacterUID, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0156, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_DeleteCityLeaderInfo Failed!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_RegNotice(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		�������� ���
/// \author		dhjin
/// \date		2007-08-22 ~ 2007-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_RegNotice(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_RegNotice
	/************************************************************************
	CREATE PROCEDURE dbo.atum_RegNotice
		@i_MapIndex						INT,
		@i_CharacterUID					INT,
		@i_Notice						VARCHAR(512)
	AS
		DECLARE	@CheckCount INT
		SET @CheckCount = (SELECT Count(*) FROM td_CityLeaderInfo WHERE MapIndex = @i_MapIndex)

		-- �������� ���� ���� ���� ��츦 ����Ͽ� �߰��ϴ� ��ƾ�� �д�.
		IF (@CheckCount = 0)
		BEGIN
		--  2007-10-18 by dhjin, �߰� ���� �����Ѵ�.
		--	INSERT INTO	td_CityLeaderInfo VALUES(@i_MapIndex, @i_CharacterUID, 0,0,0,0, @i_Notice)		
			RETURN
		END

		IF (@CheckCount = 1)
		BEGIN
			UPDATE td_CityLeaderInfo SET Notice = @i_Notice
			WHERE MapIndex = @i_MapIndex AND CharacterUID = @i_CharacterUID
			RETURN
		END
	GO
	************************************************************************/
	QPARAM_REG_NOTICE *pQPMsg = (QPARAM_REG_NOTICE*)q.pQueryParam;
	
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->MapIndex, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->CharacterUID, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_NOTICE, 0, pQPMsg->Notice, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0157, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_RegNotice Failed!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_ModifyNotice(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		�������� ����.����
/// \author		dhjin
/// \date		2007-08-22 ~ 2007-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_ModifyNotice(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_ModifyNotice
	/************************************************************************
	CREATE PROCEDURE dbo.atum_ModifyNotice
		@i_MapIndex						INT,
		@i_CharacterUID					INT,
		@i_Notice						VARCHAR(512)
	AS
		UPDATE td_CityLeaderInfo SET Notice = @i_Notice
		WHERE MapIndex = @i_MapIndex AND CharacterUID = @i_CharacterUID
	GO
	************************************************************************/
	QPARAM_MODIFY_NOTICE *pQPMsg = (QPARAM_MODIFY_NOTICE*)q.pQueryParam;
	
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->MapIndex, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->CharacterUID, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_NOTICE, 0, pQPMsg->Notice, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0158, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_ModifyNotice Failed!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_UpdateSecondaryPassword(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		// 2007-09-12 by cmkwon, ��Ʈ�� 2���н����� ���� -
/// \author		cmkwon
/// \date		2007-09-12 ~ 2007-09-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_UpdateSecondaryPassword(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_UpdateSecondaryPassword
	
	/************************************************************************
	--!!!!
	-- Name:
	-- Desc: // 2007-09-12 by cmkwon, ��Ʈ�� 2���н����� ���� - 2���н����� ������Ʈ ���ν���
	--====
	CREATE PROCEDURE dbo.atum_UpdateSecondaryPassword
		@i_AccUID						INT,
		@i_SecPassword					VARCHAR(35)
	AS
		UPDATE td_Account
		SET SecondaryPassword = @i_SecPassword
		WHERE AccountUniqueNumber = @i_AccUID
	GO	
	************************************************************************/
	QPARAM_UPDATE_SECONDARY_PASSWORD *pQPMsg = (QPARAM_UPDATE_SECONDARY_PASSWORD*)q.pQueryParam;

	if(NULL == q.pFieldIOCPSocket
		|| FALSE == q.pFieldIOCPSocket->IsValidCharacter(FALSE))
	{
		util::del(pQPMsg);
		return;
	}
	
	SQLINTEGER	arrCB[3] = {SQL_NTS,SQL_NTS,SQL_NTS};

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->AccountUID, 0,									&arrCB[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_PASSWORD_MD5_STRING, 0, pQPMsg->NewSecPassword, 0,	&arrCB[2]);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0159, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"@atum_UpdateSecondaryPassword Failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	if(FALSE == q.pFieldIOCPSocket->IsValidCharacter(FALSE)
		|| q.pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber != pQPMsg->AccountUID)
	{
		util::del(pQPMsg);
		return;
	}

	q.pFieldIOCPSocket->SetSecondaryPassword(pQPMsg->NewSecPassword);
	q.pFieldIOCPSocket->SendAddMessageType(T_FC_INFO_SECONDARYPASSWORD_UPDATE_PASSWORD_OK);
	util::del(pQPMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_LoadVoterList(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		��ǥ�� ����Ʈ �ε�
/// \author		dhjin
/// \date		2007-10-29 ~ 2007-10-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_LoadVoterList(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_LoadVoterList
	-- DESC				: ��ǥ�� ����Ʈ �������� ���ν���
	-- 2007-10-24 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_LoadVoterList
	AS
		SELECT AccountUID, CharacterUID, LeaderCandidateNum
		FROM dbo.td_VoterList
	GO
	************************************************************************/
	
	RETCODE	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0160, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_LoadVoterList Failed!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	
	SQLINTEGER	arrCB[4] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};

	SVOTER_LIST			tmVoterList;
	util::zero(&tmVoterList, sizeof(SVOTER_LIST));
	mtvectSVOTER_LIST	tmmtvectSVOTER_LIST;
	tmmtvectSVOTER_LIST.clear();

	SQLBindCol(hstmt, 1, SQL_C_ULONG, &tmVoterList.AccountUID, 0, &arrCB[1]);
	SQLBindCol(hstmt, 2, SQL_C_ULONG, &tmVoterList.CharacterUID, 0, &arrCB[2]);
	SQLBindCol(hstmt, 3, SQL_C_ULONG, &tmVoterList.LeaderCandidateNum, 0, &arrCB[3]);

	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA)
	{
		tmmtvectSVOTER_LIST.push_back(tmVoterList);
		util::zero(&tmVoterList, sizeof(SSTRATEGYPOINT_NOT_SUMMON_TIME));
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);		

	m_pFieldIOCP8->m_InflWarManager.InitVoterListByDB(&tmmtvectSVOTER_LIST);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_LoadLeaderCandidate(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		������ �ĺ� ����Ʈ �ε�
/// \author		dhjin
/// \date		2007-10-29 ~ 2007-10-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_LoadLeaderCandidate(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_LoadLeaderCandidate
	-- DESC				: ������ �ĺ� ����Ʈ �������� ���ν���
	-- 2007-10-24 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_LoadLeaderCandidate
	AS
		SELECT LeaderCandidateNum, AccountUID, CharacterUID, CharacterName, GuildUID, GuildName, Influence, DeleteCandidate, PollCount, CampaignPromises
		FROM dbo.td_LeaderCandidate
	GO
	************************************************************************/
	
	RETCODE	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0015, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_LoadLeaderCandidate Failed!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	
	SQLINTEGER	arrCB[11] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS,	SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS
							, SQL_NTS};

	SLEADER_CANDIDATE			tmSLEADER_CANDIDATE;
	util::zero(&tmSLEADER_CANDIDATE, sizeof(SLEADER_CANDIDATE));
	mtvectSLEADER_CANDIDATE		tmmtvectSLEADER_CANDIDATE;
	tmmtvectSLEADER_CANDIDATE.clear();

	SQLBindCol(hstmt, 1, SQL_C_ULONG, &tmSLEADER_CANDIDATE.LeaderCandidateNum, 0, &arrCB[1]);
	SQLBindCol(hstmt, 2, SQL_C_ULONG, &tmSLEADER_CANDIDATE.AccountUID, 0, &arrCB[2]);
	SQLBindCol(hstmt, 3, SQL_C_ULONG, &tmSLEADER_CANDIDATE.CharacterUID, 0, &arrCB[3]);
	SQLBindCol(hstmt, 4, SQL_C_CHAR, tmSLEADER_CANDIDATE.CharacterName, SIZE_MAX_CHARACTER_NAME, &arrCB[4]);
	SQLBindCol(hstmt, 5, SQL_C_ULONG, &tmSLEADER_CANDIDATE.GuildUID, 0, &arrCB[5]);
	SQLBindCol(hstmt, 6, SQL_C_CHAR, tmSLEADER_CANDIDATE.GuildName, SIZE_MAX_GUILD_NAME, &arrCB[6]);
	SQLBindCol(hstmt, 7, SQL_C_TINYINT, &tmSLEADER_CANDIDATE.Influence, 0, &arrCB[7]);
	SQLBindCol(hstmt, 8, SQL_C_TINYINT, &tmSLEADER_CANDIDATE.DeleteCandidate, 0, &arrCB[8]);
	SQLBindCol(hstmt, 9, SQL_C_ULONG, &tmSLEADER_CANDIDATE.PollCount, 0, &arrCB[9]);
	SQLBindCol(hstmt, 10, SQL_C_CHAR, tmSLEADER_CANDIDATE.CampaignPromises, SIZE_MAX_CAMPAIGNPROMISES, &arrCB[10]);

	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA)
	{
		tmmtvectSLEADER_CANDIDATE.push_back(tmSLEADER_CANDIDATE);
		util::zero(&tmSLEADER_CANDIDATE, sizeof(SSTRATEGYPOINT_NOT_SUMMON_TIME));
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);		

	m_pFieldIOCP8->m_InflWarManager.InitLeaderCandidateByDB(&tmmtvectSLEADER_CANDIDATE);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_SelectLeaderCandidateInfoByRealTimeVariable(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		������ �ĺ� �ǽð����� ���ϴ� ���� DB���� ������ �������� �����Ѵ�.
/// \author		dhjin
/// \date		2007-10-30 ~ 2007-10-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_SelectLeaderCandidateInfoByRealTimeVariable(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_SelectLeaderCandidateInfoByRealTimeVariable
	-- DESC				: ������ �ĺ��� �ǽð����� ��ȭ �� �� �ִ� ������(level, guildfame���) ������ ���ν���
	-- 2007-10-30 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_SelectLeaderCandidateInfoByRealTimeVariable
		@i_AccountUID				INT,
		@i_CharacterUID				INT
	AS
		SELECT C.Level, C.PilotFace, G.MonthlyFame, G.GuildMarkImage, G.GuildMarkSize
		FROM td_character AS C INNER JOIN td_Guild AS G 
		ON C.UniqueNumber = G.GuildCommanderUniqueNumber
		WHERE C.UniqueNumber = @i_CharacterUID AND C.AccountUniqueNumber = @i_AccountUID
	GO
	************************************************************************/
	if (q.pFieldIOCPSocket == NULL || !q.pFieldIOCPSocket->IsUsing()
		|| !q.pFieldIOCPSocket->IsValidCharacter(FALSE))
	{
		return;
	}
	
	SLEADER_CANDIDATE	ChoiceLeaderInfo;
	util::zero(&ChoiceLeaderInfo, sizeof(SLEADER_CANDIDATE));
	if(FALSE == q.pFieldIOCPSocket->ms_pFieldIOCP->m_InflWarManager.GetLeaderCandidateInfo(q.nGeneralParam1, q.nGeneralParam2, &ChoiceLeaderInfo))
	{// 2007-10-30 by dhjin, ������ ������ �ĺ� ���� �����´�.
		q.pFieldIOCPSocket->SendErrorMessage(T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO, ERR_NO_SEARCH_CHARACTER);
	}

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &ChoiceLeaderInfo.AccountUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &ChoiceLeaderInfo.CharacterUID, 0, NULL);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0162, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_SelectLeaderCandidateInfoByRealTimeVariable Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	CHAR	tmGuildMarkImage[SIZE_MAX_GUILD_MARK_IMAGE];
	INIT_MSG_WITH_BUFFER(MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK, T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK, pSendMsg, SendBuf);
	INIT_MSG_WITH_BUFFER(MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK_GUILDMARK, T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK_GUILDMARK, pSendMsg2, SendBuf2);
	SQLINTEGER	arrCB[6] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS,  SQL_NTS};
	SQLBindCol(hstmt, 1, SQL_C_TINYINT, &pSendMsg->Level, 0, &arrCB[1]);
	SQLBindCol(hstmt, 2, SQL_C_TINYINT, &pSendMsg->PilotFace, 0, &arrCB[2]);
	SQLBindCol(hstmt, 3, SQL_C_ULONG, &pSendMsg->GuildFame, 0, &arrCB[3]);
	SQLBindCol(hstmt, 4, SQL_C_BINARY, tmGuildMarkImage, SIZE_MAX_GUILD_MARK_IMAGE, &arrCB[4]);
	SQLBindCol(hstmt, 5, SQL_C_ULONG, &pSendMsg2->GuildMarkSize, 0, &arrCB[5]);
	if ( (ret = SQLFetch(hstmt)) == SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_SelectLeaderCandidateInfoByRealTimeVariable Select Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		q.pFieldIOCPSocket->SendErrorMessage(T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO, ERR_NO_SEARCH_CHARACTER);
		return;		
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	pSendMsg->AccountUID		= ChoiceLeaderInfo.AccountUID;
	util::strncpy(pSendMsg->CampaignPromises, ChoiceLeaderInfo.CampaignPromises, SIZE_MAX_CAMPAIGNPROMISES);
	util::strncpy(pSendMsg->CharacterName, ChoiceLeaderInfo.CharacterName, SIZE_MAX_CHARACTER_NAME);
	pSendMsg->CharacterUID		= ChoiceLeaderInfo.CharacterUID;
	util::strncpy(pSendMsg->GuildName, ChoiceLeaderInfo.GuildName, SIZE_MAX_GUILD_NAME);

	q.pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK));
	
	if(0 <= pSendMsg2->GuildMarkSize
		&& 1300 >= pSendMsg2->GuildMarkSize)
	{// 2007-11-01 by dhjin, ���� �̹��� ī��
		memcpy(SendBuf2+MSG_SIZE(MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK_GUILDMARK), tmGuildMarkImage, pSendMsg2->GuildMarkSize);
		pSendMsg2->GuildUID			= ChoiceLeaderInfo.GuildUID;
		q.pFieldIOCPSocket->SendAddData(SendBuf2, MSG_SIZE(MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK_GUILDMARK)+pSendMsg2->GuildMarkSize);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_InsertLeaderCandidate(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		������ �ĺ� ���
/// \author		dhjin
/// \date		2007-10-30 ~ 2007-10-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_InsertLeaderCandidate(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_InsertLeaderCandidate
	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_InsertLeaderCandidate
	-- DESC				: ������ �ĺ� ��� ���ν���
	-- 2007-10-24 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_InsertLeaderCandidate
		-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
		--@o_RetCode					INT OUTPUT,
		--@o_NewLeaderCandidateNum	INT OUTPUT,
		@i_ReqGuildMemberNum		INT,
		@i_ReqGuildFame				INT,
		@i_AccountUID				INT,
		@i_CharacterUID				INT,
		@i_CharacterName			VARCHAR(20),
		@i_GuildUID					INT,
		@i_GuildName				VARCHAR(30),
		@i_Influence				TINYINT,
		@i_CampaignPromises			VARCHAR(512)
	AS
		DECLARE @CheckCount			INT
		DECLARE @ReqGuildMemberNum	INT
		DECLARE @GuildUID			INT
		DECLARE @NewLeaderCandidateNum	INT	-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 

		SET @CheckCount = (SELECT Count(*) FROM dbo.td_Guild WITH (NOLOCK) 
							WHERE GuildUniqueNumber = @i_GuildUID AND GuildCommanderUniqueNumber = @i_CharacterUID AND MonthlyFame >= @i_ReqGuildFame)
		IF(@CheckCount <> 1)
		BEGIN
			-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
			--SET @o_RetCode = 10		-- ������ �������� �ʰų� �䱸 ������ �۴�.
			--RETURN
			SELECT 10, 0;
			RETURN;
		END
		
		SET @ReqGuildMemberNum = (SELECT Count(*) FROM dbo.td_GuildMember WITH (NOLOCK) WHERE GuildUniqueNumber = @i_GuildUID)
		IF(@ReqGuildMemberNum < @i_ReqGuildMemberNum)
		BEGIN
			-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
			--SET @o_RetCode = 20		-- ������ �������� �ʰų� �䱸 ���� ������ �۴�
			--RETURN
			SELECT 20, 0;
			RETURN;
		END

		-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
		--SET @o_NewLeaderCandidateNum = (SELECT TOP 1 LeaderCandidateNum FROM dbo.td_LeaderCandidate WITH (NOLOCK) 
		--									WHERE Influence = @i_Influence ORDER BY LeaderCandidateNum DESC)
		--SET @o_NewLeaderCandidateNum = @o_NewLeaderCandidateNum + 1
		--INSERT INTO dbo.td_LeaderCandidate (LeaderCandidateNum, AccountUID, CharacterUID, CharacterName, GuildUID, GuildName, Influence, DeleteCandidate, PollCount, CampaignPromises) 
		--VALUES (@o_NewLeaderCandidateNum, @i_AccountUID, @i_CharacterUID, @i_CharacterName, @i_GuildUID, @i_GuildName, @i_Influence, 0, 0, @i_CampaignPromises)
		--
		--SET @o_RetCode = 0
		SET @NewLeaderCandidateNum = (SELECT TOP 1 LeaderCandidateNum FROM dbo.td_LeaderCandidate WITH (NOLOCK) 
											WHERE Influence = @i_Influence ORDER BY LeaderCandidateNum DESC)
		IF(@NewLeaderCandidateNum IS NULL)
		BEGIN
			SET @NewLeaderCandidateNum = 0
		END
		SET @NewLeaderCandidateNum = @NewLeaderCandidateNum + 1
		INSERT INTO dbo.td_LeaderCandidate (LeaderCandidateNum, AccountUID, CharacterUID, CharacterName, GuildUID, GuildName, Influence, DeleteCandidate, PollCount, CampaignPromises) 
		VALUES (@NewLeaderCandidateNum, @i_AccountUID, @i_CharacterUID, @i_CharacterName, @i_GuildUID, @i_GuildName, @i_Influence, 0, 0, @i_CampaignPromises)

		SELECT 0, @NewLeaderCandidateNum;
	GO
	************************************************************************/
	QPARAM_INSERT_LEADER_CANDIDATE		*pQPMsg = (QPARAM_INSERT_LEADER_CANDIDATE*)q.pQueryParam;
	if (q.pFieldIOCPSocket == NULL || !q.pFieldIOCPSocket->IsUsing()
		|| !q.pFieldIOCPSocket->IsValidCharacter(FALSE))
	{
		util::del(pQPMsg);
		return;
	}
	
	INT RetCode = 0;
	INT	NewLeaderCandidateNum = 0;
	SQLINTEGER	arrCB[12] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS};	
// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
// 	SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &RetCode, 0, &arrCB[1]);
// 	SQLBindParameter(hstmt, 2, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &NewLeaderCandidateNum, 0, &arrCB[2]);
// 	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->ReqGuildMemberNum, 0, &arrCB[3]);
// 	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->ReqGuildFame, 0, &arrCB[4]);
// 	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->AccountUID, 0, &arrCB[5]);
// 	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->CharacterUID, 0, &arrCB[6]);
// 	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pQPMsg->CharacterName, 0, &arrCB[7]);
// 	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->GuildUID, 0, &arrCB[8]);
// 	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_GUILD_NAME, 0, pQPMsg->GuildName, 0, &arrCB[9]);
// 	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &pQPMsg->Influence, 0, &arrCB[10]);
// 	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CAMPAIGNPROMISES, 0, pQPMsg->CampaignPromises, 0, &arrCB[11]);
// 	
// 	RETCODE ret = SQLExecDirect(hstmt, (UCHAR*)"{call atum_InsertLeaderCandidate(?,?,?,?,?, ?,?,?,?,?, ?)}", SQL_NTS);
// 	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
// 	{
// 		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_InsertLeaderCandidate Failed!\n", TRUE, q.pFieldIOCPSocket, q);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		util::del(pQPMsg);
// 		q.pFieldIOCPSocket->SendErrorMessage(T_FC_CITY_POLL_REG_LEADER_CANDIDATE, ERR_FAIL_REG_LEADER_CANDIDATE_BY_DB);
// 		return;
// 	}
// 
// 	// Clear any result sets generated. For ouput parameter...
// 	while ( ( ret = SQLMoreResults(hstmt) ) != SQL_NO_DATA );

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) - 
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->ReqGuildMemberNum, 0,					&arrCB[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->ReqGuildFame, 0,						&arrCB[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->AccountUID, 0,							&arrCB[3]);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->CharacterUID, 0,						&arrCB[4]);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pQPMsg->CharacterName, 0,	&arrCB[5]);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->GuildUID, 0,							&arrCB[6]);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_GUILD_NAME, 0, pQPMsg->GuildName, 0,			&arrCB[7]);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &pQPMsg->Influence, 0,						&arrCB[8]);
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CAMPAIGNPROMISES, 0, pQPMsg->CampaignPromises, 0, &arrCB[9]);
	
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0163, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_InsertLeaderCandidate Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		q.pFieldIOCPSocket->SendErrorMessage(T_FC_CITY_POLL_REG_LEADER_CANDIDATE, ERR_FAIL_REG_LEADER_CANDIDATE_BY_DB, 10000);
		return;
	}
	SQLBindCol(hstmt, 1, SQL_C_LONG, &(RetCode), 0, NULL);
	SQLBindCol(hstmt, 2, SQL_C_LONG, &(NewLeaderCandidateNum), 0, NULL);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(hstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 �̸� ����� RowCount �̴� 
			ret = SQLMoreResults(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(hstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(hstmt);                 // 2008-07-25 by cmkwon, ���� Results �� �ִ����� �ѹ��� üũ�Ѵ�.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}

	if(10 == RetCode || 20 == RetCode)
	{// 2007-10-30 by dhjin, �䱸 ������ �������� ���� ����
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		q.pFieldIOCPSocket->SendErrorMessage(T_FC_CITY_POLL_REG_LEADER_CANDIDATE, ERR_FAIL_REG_LEADER_CANDIDATE_BY_REQ, 11000, RetCode);
		return;
	}
	else if(0 == RetCode)
	{// 2007-10-30 by dhjin, ������ �ĺ� ��� ����
		SLEADER_CANDIDATE	NewLeaderCandidateInfo;
		util::zero(&NewLeaderCandidateInfo, sizeof(SLEADER_CANDIDATE));
		NewLeaderCandidateInfo.AccountUID			= pQPMsg->AccountUID;
		NewLeaderCandidateInfo.CharacterUID			= pQPMsg->CharacterUID;
		NewLeaderCandidateInfo.DeleteCandidate		= 0;
		NewLeaderCandidateInfo.GuildUID				= pQPMsg->GuildUID;
		NewLeaderCandidateInfo.Influence			= pQPMsg->Influence;
		NewLeaderCandidateInfo.LeaderCandidateNum	= NewLeaderCandidateNum;
		NewLeaderCandidateInfo.PollCount			= 0;
		util::strncpy(NewLeaderCandidateInfo.CampaignPromises, pQPMsg->CampaignPromises, SIZE_MAX_CAMPAIGNPROMISES);
		util::strncpy(NewLeaderCandidateInfo.CharacterName, pQPMsg->CharacterName, SIZE_MAX_CHARACTER_NAME);
		util::strncpy(NewLeaderCandidateInfo.GuildName, pQPMsg->GuildName, SIZE_MAX_GUILD_NAME);
		q.pFieldIOCPSocket->ms_pFieldIOCP->m_InflWarManager.InsertLeaderCandidate(&NewLeaderCandidateInfo);
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "CAtumFieldDBManager::atum_InsertLeaderCandidate Reg : CharacterUID = %d, GuildUID = %d, RegNum = %d\r\n", pQPMsg->CharacterUID, pQPMsg->GuildUID, NewLeaderCandidateNum);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);

		INIT_MSG_WITH_BUFFER(MSG_FC_CITY_POLL_REG_LEADER_CANDIDATE_OK, T_FC_CITY_POLL_REG_LEADER_CANDIDATE_OK, pSendMsg, SendBuf);
		pSendMsg->LeaderCandidateNum		=	NewLeaderCandidateNum;
		q.pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_CITY_POLL_REG_LEADER_CANDIDATE_OK));
		return;		
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);
	q.pFieldIOCPSocket->SendErrorMessage(T_FC_CITY_POLL_REG_LEADER_CANDIDATE, ERR_COMMON_UNKNOWN_ERROR, 12000);
	return;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_UpdateLeaderDeleteCandidate(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		������ �ĺ� Ż��
/// \author		dhjin
/// \date		2007-10-30 ~ 2007-10-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_UpdateLeaderDeleteCandidate(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_UpdateLeaderDeleteCandidate
	-- DESC				: ������ �ĺ� Ż�� ������Ʈ ���ν���
	-- 2007-10-24 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_UpdateLeaderDeleteCandidate
		@i_CharacterUID		INT
	AS
		UPDATE dbo.td_LeaderCandidate SET DeleteCandidate = 1 WHERE CharacterUID = @i_CharacterUID
	GO
	************************************************************************/
	if (q.pFieldIOCPSocket == NULL || !q.pFieldIOCPSocket->IsUsing()
		|| !q.pFieldIOCPSocket->IsValidCharacter(FALSE))
	{
		return;
	}

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &q.pFieldIOCPSocket->m_character.CharacterUniqueNumber, 0, NULL);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0164, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateLeaderDeleteCandidate Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	q.pFieldIOCPSocket->ms_pFieldIOCP->m_InflWarManager.UpdateDeleteLeaderCandidate(q.pFieldIOCPSocket->m_character.CharacterUniqueNumber);
	SQLFreeStmt(hstmt, SQL_CLOSE);
	q.pFieldIOCPSocket->SendAddMessageType(T_FC_CITY_POLL_DELETE_LEADER_CANDIDATE_OK);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_UpdateLeaderPollCount(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		��ǥ�� ȹ�� ǥ ������Ʈ
/// \author		dhjin
/// \date		2007-10-31 ~ 2007-10-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_UpdateLeaderPollCount(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_UpdateLeaderPollCount
	-- DESC				: ��ǥ�� ȹ�� ǥ ������Ʈ ���ν���
	-- 2007-10-24 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_UpdateLeaderPollCount
		@i_LeaderCandidateNum		INT,
		@i_Influence				TINYINT,
		@i_PollCount				INT
	AS
		UPDATE dbo.td_LeaderCandidate SET PollCount = @i_PollCount WHERE LeaderCandidateNum = @i_LeaderCandidateNum AND Influence = @i_Influence
	GO
	************************************************************************/
	QPARAM_UPDATE_LEADERPOLLCOUNT *pQPMsg = (QPARAM_UPDATE_LEADERPOLLCOUNT*)q.pQueryParam;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->LeaderCandidateNum, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &pQPMsg->Influence, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->PollCount, 0, NULL);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0165, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateLeaderPollCount Failed!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_InsertVoterList(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		��ǥ�� ��� ������Ʈ
/// \author		dhjin
/// \date		2007-10-31 ~ 2007-10-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_InsertVoterList(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_InsertVoterList
	-- DESC				: ��ǥ�� ��� ���ν���
	-- 2007-10-24 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_InsertVoterList
		@i_AccountUID			INT,
		@i_CharacterUID			INT,
		@i_LeaderCandidateNum	INT
	AS
		INSERT INTO dbo.td_VoterList (AccountUID, CharacterUID, LeaderCandidateNum, PollDate) 
		VALUES (@i_AccountUID, @i_CharacterUID, @i_LeaderCandidateNum, GetDate())
	GO
	************************************************************************/
	QPARAM_INSERT_VOTER_LIST		*pQPMsg = (QPARAM_INSERT_VOTER_LIST*)q.pQueryParam;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->AccountUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->CharacterUID, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->LeaderCandidateNum, 0, NULL);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0166, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_InsertVoterList Failed!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_CheckGiveTarget(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		// 2007-11-13 by cmkwon, �����ϱ� ��� �߰� - CAtumFieldDBManager::QP_CheckGiveTarget() �߰�
/// \author		cmkwon
/// \date		2007-11-13 ~ 2007-11-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_CheckGiveTarget(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_CheckGiveTarget
	QPARAM_CHECK_GiveTarget		*pQPMsg = (QPARAM_CHECK_GiveTarget*)q.pQueryParam;
	if(FALSE == q.pFieldIOCPSocket->IsValidCharacter(FALSE))
	{
		util::del(pQPMsg);
		return;
	}

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_GetGiveTargetCharacter
	-- DESC				: // 2007-11-13 by cmkwon, �����ϱ� ��� �߰� - ���� �޴� ĳ���� ���� ��������
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_GetGiveTargetCharacter
		@i_CharName				VARCHAR(20)
	AS
		--		#define RACE_DELETED_CHARACTER		(USHORT)0x4000	// ������ ĳ����, 16384 // 2007-02-21 by cmkwon
		SELECT a.AccountName, a.AccountUniqueNumber, a.ConnectingServerGroupID, am.ItemNum, am.ExpireTime, c.UniqueNumber, c.InfluenceType, c.GuildName, c.UnitKind, c.Level
		FROM td_account a WITH(NOLOCK) INNER JOIN td_character c WITH(NOLOCK) ON c.AccountUniqueNumber = a.AccountUniqueNumber
		LEFT OUTER JOIN td_accountCashStore am WITH(NOLOCK) ON a.AccountUniqueNumber = am.AccountUniqueNumber
		WHERE c.CharacterName = @i_CharName AND 0 = c.Race & 0x4000
	GO
	************************************************************************/

	SQLINTEGER	arrCB[11] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
								,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pQPMsg->GiveTargetCharName, 0, &arrCB[1]);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0167, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		if(q.pFieldIOCPSocket->IsValidCharacter(FALSE))
		{
			q.pFieldIOCPSocket->SendErrorMessage(T_FC_SHOP_CHECK_GIVE_TARGET, ERR_DB_EXECUTION_FAILED);
		}

		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_GetGiveTargetCharacter_ Failed!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}

	// 2007-11-13 by cmkwon, arrCB �ʱ�ȭ
	for(int i=0; i < 11; i++){			arrCB[i] = SQL_NTS;}

	SGIVE_TARGET_CHARACTER	tmGiveTargetChar;	util::zero(&tmGiveTargetChar, sizeof(tmGiveTargetChar));
	SQL_TIMESTAMP_STRUCT	tmSqlTime;			util::zero(&tmSqlTime, sizeof(tmSqlTime));

	SQLBindCol(hstmt, 1, SQL_C_CHAR, tmGiveTargetChar.AccountName0, SIZE_MAX_ACCOUNT_NAME,			&arrCB[1]);
	SQLBindCol(hstmt, 2, SQL_C_ULONG, &tmGiveTargetChar.AccountUID0, 0,								&arrCB[2]);
	SQLBindCol(hstmt, 3, SQL_C_LONG, &tmGiveTargetChar.ConnectingServerGroupID, 0,					&arrCB[3]);
	SQLBindCol(hstmt, 4, SQL_C_LONG, &tmGiveTargetChar.MembershipItemNum, 0,						&arrCB[4]);
	SQLBindCol(hstmt, 5, SQL_C_TIMESTAMP, &tmSqlTime, 0,											&arrCB[5]);
	SQLBindCol(hstmt, 6, SQL_C_ULONG, &tmGiveTargetChar.CharacterUID0, 0,							&arrCB[6]);
	SQLBindCol(hstmt, 7, SQL_C_TINYINT, &tmGiveTargetChar.InfluenceType0, 0,						&arrCB[7]);
	SQLBindCol(hstmt, 8, SQL_C_CHAR, tmGiveTargetChar.GuildName0, SIZE_MAX_GUILD_NAME,				&arrCB[8]);
	SQLBindCol(hstmt, 9, SQL_C_USHORT, &tmGiveTargetChar.UnitKind0, 0,								&arrCB[9]);
	SQLBindCol(hstmt, 10, SQL_C_LONG, &tmGiveTargetChar.Level0, 0,									&arrCB[10]);
	
	ret = SQLFetch(hstmt);
	tmGiveTargetChar.MembershipExpireTime	= tmSqlTime;
	util::strncpy(tmGiveTargetChar.CharacterName0, pQPMsg->GiveTargetCharName, SIZE_MAX_CHARACTER_NAME);
	tmGiveTargetChar.BuyItemNum				= pQPMsg->GiveItemNum;
	SQLFreeStmt(hstmt, SQL_CLOSE);

	if(FALSE == q.pFieldIOCPSocket->IsValidCharacter(FALSE))
	{// 2007-11-13 by cmkwon, 
		util::del(pQPMsg);
		return;
	}
	if(FALSE == IS_VALID_UNIQUE_NUMBER(tmGiveTargetChar.AccountUID0)
		|| FALSE == IS_VALID_UNIQUE_NUMBER(tmGiveTargetChar.CharacterUID0))
	{// 2007-11-13 by cmkwon, �˻��� ������ üũ
		
		q.pFieldIOCPSocket->SendErrorMessage(T_FC_SHOP_CHECK_GIVE_TARGET, ERR_INVALID_PEER_CHARACTER, 200);
		util::del(pQPMsg);
		return;
	}

	q.pFieldIOCPSocket->OnCheckGiveTarget(&tmGiveTargetChar);
	util::del(pQPMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_UpdatePilotFace(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		// 2007-11-21 by cmkwon, PilotFace ���� ī�� ���� - CAtumFieldDBManager::QP_UpdatePilotFace() �߰�
/// \author		cmkwon
/// \date		2007-11-21 ~ 2007-11-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_UpdatePilotFace(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	UID32_t						CharUID		= (UID32_t)q.nGeneralParam1;
	BYTE						PilotFace	= (BYTE)q.nGeneralParam2;

	if(FALSE == IS_VALID_UNIQUE_NUMBER(CharUID))
	{
		return;
	}

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_UpdatePilotFace
	-- DESC				: // 2007-11-21 by cmkwon, PilotFace ���� ī�� ���� - �ش� ĳ������ PilotFace�� �����Ѵ�.
	--
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_UpdatePilotFace
		@i_CharUID		INT,
		@i_PilotFace	TINYINT
	AS
		UPDATE dbo.td_character
		SET PilotFace = @i_PilotFace
		WHERE UniqueNumber = @i_CharacterUID
	GO
	************************************************************************/

	SQLINTEGER	arrCB[3] = {SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &CharUID, 0,	&arrCB[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &PilotFace, 0,	&arrCB[2]);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0168, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdatePilotFace_ Failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_InsertNotifyMsg(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		// 2007-11-28 by cmkwon, �����ý��� ���� - CAtumFieldDBManager::QP_InsertNotifyMsg() �߰�
/// \author		cmkwon
/// \date		2007-11-28 ~ 2007-11-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_InsertNotifyMsg(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_InsertNotifyMsg
	SNOTIFY_MSG						*pQPMsg = (SNOTIFY_MSG*)q.pQueryParam;

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_InsertNotifyMsg
	-- DESC				: // 2007-11-28 by cmkwon, �����ý��� ���� - dbo.atum_InsertNotifyMsg �߰�
	--					  // 2007-12-20 by cmkwon, �����ý��� ���� ���� - �߰��� NotifyMsgUID �� �����Ѵ�.
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_InsertNotifyMsg
		@i_CharacterUID				INT,							-- ���� ���� CharacterUID
		@i_NotifyMsgType			TINYINT,						-- ���� �޽��� Ÿ��(NOTIFY_MSG_TYPE_XXX)
		@i_NotifyMsgString			VARCHAR(512),					-- ���� �޽��� ��Ʈ��(512)
		@i_SenderCharacterUID		INT,							-- ������ ���� CharacterUID
		@i_SenderCharacterName		VARCHAR(20)						-- ������ ���� CharacterName
	AS
		INSERT INTO dbo.td_NotifyMsg(CharacterUID, NotifyMsgType, NotifyMsgString, SenderCharacterUID, SenderCharacterName)
		VALUES(@i_CharacterUID, @i_NotifyMsgType, @i_NotifyMsgString, @i_SenderCharacterUID, @i_SenderCharacterName)

		-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
		--RETURN SCOPE_IDENTITY();		-- // 2007-12-20 by cmkwon, �����ý��� ���� ���� - �߰��� NotifyMsgUID �� �����Ѵ�.
		IF (@@ERROR <> 0)
		BEGIN
			SELECT 0;
			RETURN;
		END

		SELECT SCOPE_IDENTITY();
	GO
	************************************************************************/

	// 2007-12-20 by cmkwon, �����ý��� ���� ���� - �߰��� NotifyMsgUID �� ���� �޾Ƽ� Ŭ���̾�Ʈ�� �����Ѵ�.
	SQLINTEGER	arrCB[7] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS};

// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
// 	SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &pQPMsg->NotifyMsgUID, 0,									&arrCB[1]);
// 	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->CharacterUID, 0,									&arrCB[2]);
// 	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &pQPMsg->NotifyMsgType, 0,								&arrCB[3]);
// 	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_NOTIFY_MSG_STRING, 0, pQPMsg->NotifyMsgString, 0,			&arrCB[4]);
// 	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->SenderCharacterUID, 0,								&arrCB[5]);
// 	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pQPMsg->SenderCharacterName, 0,		&arrCB[6]);
// 	// 2008-07-08 by cmkwon, MySQL �������� ����(���ν������� ����ó�� �Ұ��� ����) - 
// 	//RETCODE ret = SQLExecDirect(hstmt, (UCHAR*)"{? = call atum_InsertNotifyMsg(?,?,?,?,?)}", SQL_NTS);
// 	RETCODE ret = SQLExecDirect(hstmt, (UCHAR*)"{call atum_InsertNotifyMsg(?,?,?,?,?, ?)}", SQL_NTS);
// 	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
// 	{
// 		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_InsertNotifyMsg_ Failed!\r\n", TRUE, NULL, q);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		util::del(pQPMsg);
// 		return;
// 	}
// 	// Clear any result sets generated.
// 	while ( ( ret = SQLMoreResults(hstmt) ) != SQL_NO_DATA );		// 2007-12-20 by cmkwon, �����ý��� ���� ���� - 
// 	SQLFreeStmt(hstmt, SQL_CLOSE);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->CharacterUID, 0,									&arrCB[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &pQPMsg->NotifyMsgType, 0,								&arrCB[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_NOTIFY_MSG_STRING, 0, pQPMsg->NotifyMsgString, 0,			&arrCB[3]);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->SenderCharacterUID, 0,								&arrCB[4]);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pQPMsg->SenderCharacterName, 0,		&arrCB[5]);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0169, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_InsertNotifyMsg_ Failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	SQLBindCol(hstmt, 1, SQL_C_UBIGINT, &(pQPMsg->NotifyMsgUID), 0, NULL);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(hstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 �̸� ����� RowCount �̴� 
			ret = SQLMoreResults(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(hstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(hstmt);                 // 2008-07-25 by cmkwon, ���� Results �� �ִ����� �ѹ��� üũ�Ѵ�.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-11-28 by cmkwon, �����ý��� ���� - ���� �������� �������Դ� �ٷ� �����Ѵ�.
	CFieldIOCPSocket *pFISoc = m_pFieldIOCP8->GetFieldIOCPSocketByCharacterUniqueNumber(pQPMsg->CharacterUID);
	if(pFISoc)
	{
// 2007-12-20 by cmkwon, �����ý��� ���� ���� - �Ʒ��� ���� CFieldIOCPSocket::InsertNotifyMsg() �Լ����� ó��
//		INIT_MSG_WITH_BUFFER(MSG_FC_EVENT_NOTIFY_MSG_GET_OK, T_FC_EVENT_NOTIFY_MSG_GET_OK, pSMsg, SendBuf);
//		pSMsg->NotifyMsgUID		= pQPMsg->NotifyMsgUID;
//		pSMsg->CharacterUID		= pQPMsg->CharacterUID;
//		pSMsg->NotifyMsgType	= pQPMsg->NotifyMsgType;
//		util::strncpy(pSMsg->NotifyMsgString, pQPMsg->NotifyMsgString, SIZE_MAX_NOTIFY_MSG_STRING);
//		pFISoc->SendAddData(SendBuf, MSG_SIZE(MSG_FC_EVENT_NOTIFY_MSG_GET_OK));
		pFISoc->InsertNotifyMsg(pQPMsg);		// 2007-12-20 by cmkwon, �����ý��� ���� ���� - 
	}

	util::del(pQPMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_GetNotifyMsg(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		// 2007-11-28 by cmkwon, �����ý��� ���� -
/// \author		cmkwon
/// \date		2007-11-28 ~ 2007-11-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_GetNotifyMsg(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	UID32_t						CharUID		= (UID32_t)q.nGeneralParam1;

	if(FALSE == IS_VALID_UNIQUE_NUMBER(CharUID)
		|| FALSE == q.pFieldIOCPSocket->IsValidCharacter(FALSE))
	{
		return;
	}

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_GetNotifyMsg
	-- DESC				: // 2007-11-28 by cmkwon, �����ý��� ���� - dbo.atum_GetNotifyMsg �߰�
	--
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_GetNotifyMsg
		@i_CharacterUID				INT								-- ���� ���� CharacterUID
	AS
		SELECT NotifyMsgUID, CharacterUID, NotifyMsgType, NotifyMsgString, SenderCharacterUID, SenderCharacterName, CreatedTime
		FROM dbo.td_NotifyMsg WITH(NOLOCK)
		WHERE CharacterUID = 0 OR CharacterUID = @i_CharacterUID
	GO
	************************************************************************/

	SQLINTEGER	arrCB[8] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &CharUID, 0,	&arrCB[1]);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0170, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_GetNotifyMsg_ Failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	SNOTIFY_MSG				notifyMsg;		util::zero(&notifyMsg, sizeof(notifyMsg));
	SQL_TIMESTAMP_STRUCT	tmpTimeStamp;	util::zero(&tmpTimeStamp, sizeof(tmpTimeStamp));

	mtvectSNOTIFY_MSG tmNotifyMsgList;

	SQLBindCol(hstmt, 1, SQL_C_SBIGINT, &notifyMsg.NotifyMsgUID, 0,								&arrCB[1]);
	SQLBindCol(hstmt, 2, SQL_C_ULONG, &notifyMsg.CharacterUID, 0,								&arrCB[2]);
	SQLBindCol(hstmt, 3, SQL_C_UTINYINT, &notifyMsg.NotifyMsgType, 0,							&arrCB[3]);
	SQLBindCol(hstmt, 4, SQL_C_CHAR, notifyMsg.NotifyMsgString, SIZE_MAX_NOTIFY_MSG_STRING,		&arrCB[4]);
	SQLBindCol(hstmt, 5, SQL_C_ULONG, &notifyMsg.SenderCharacterUID, 0,							&arrCB[5]);
	SQLBindCol(hstmt, 6, SQL_C_CHAR, notifyMsg.SenderCharacterName, SIZE_MAX_CHARACTER_NAME,	&arrCB[6]);
	SQLBindCol(hstmt, 7, SQL_C_TIMESTAMP, &tmpTimeStamp, 0,										&arrCB[7]);

	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA )
	{
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
		{
			break;
		}
		notifyMsg.CreateTime	= tmpTimeStamp;
		tmNotifyMsgList.pushBackLock(notifyMsg);
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	if(FALSE == q.pFieldIOCPSocket->IsValidCharacter(FALSE))
	{
		return;
	}
	
	q.pFieldIOCPSocket->OnLoadNotifyMsg(&tmNotifyMsgList);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_DeleteNotifyMsg(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		// 2007-11-28 by cmkwon, �����ý��� ���� -
/// \author		cmkwon
/// \date		2007-11-28 ~ 2007-11-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_DeleteNotifyMsg(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	UID64_t						NotifyMsgUID = q.nGeneralParam1;

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_DeleteNotifyMsg
	-- DESC				: // 2007-11-28 by cmkwon, �����ý��� ���� - dbo.atum_DeleteNotifyMsg �߰�
	--
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_DeleteNotifyMsg
		@i_NotifyMsgUID				BIGINT
	AS
		DELETE dbo.td_NotifyMsg
		WHERE NotifyMsgUID = @i_NotifyMsgUID
	GO
	************************************************************************/

	SQLINTEGER	arrCB[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &NotifyMsgUID, 0,	&arrCB[1]);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0171, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_DeleteNotifyMsg_ Failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_GetGuildMark(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		���� ��ũ ������
/// \author		dhjin
/// \date		2007-12-07 ~ 2007-12-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_GetGuildMark(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/************************************************************************
	CREATE PROCEDURE dbo.atum_GetGuildMark
		@guildUniqueNumber		INT	-- UINT
	AS
		SELECT GuildMarkVersion, GuildMarkSize, GuildMarkImage
		FROM td_Guild WITH (NOLOCK)
		WHERE GuildUniqueNumber = @guildUniqueNumber
	GO
	************************************************************************/
	if (q.pFieldIOCPSocket == NULL || !q.pFieldIOCPSocket->IsUsing()
		|| !q.pFieldIOCPSocket->IsValidCharacter(FALSE))
	{
		return;
	}
	
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &q.nGeneralParam1, 0, NULL);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0172, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_GetGuildMark Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	CHAR	tmGuildMarkImage[SIZE_MAX_GUILD_MARK_IMAGE];
	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_GUILDMARK_OK, T_FC_INFO_GET_GUILDMARK_OK, pSendMsg, SendBuf);
	SQLINTEGER	arrCB[4] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	
	SQLBindCol(hstmt, 1, SQL_C_ULONG, &pSendMsg->GuildMarkVersion, 0, &arrCB[1]);
	SQLBindCol(hstmt, 2, SQL_C_ULONG, &pSendMsg->GuildMarkSize, 0, &arrCB[2]);
	SQLBindCol(hstmt, 3, SQL_C_BINARY, tmGuildMarkImage, SIZE_MAX_GUILD_MARK_IMAGE, &arrCB[3]);
	if ( (ret = SQLFetch(hstmt)) == SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_GetGuildMark Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;		
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	if(0 <= pSendMsg->GuildMarkSize
		&& 1300 >= pSendMsg->GuildMarkSize)
	{// 2007-11-01 by dhjin, ���� �̹��� ī��
		memcpy(SendBuf+MSG_SIZE(MSG_FC_INFO_GET_GUILDMARK_OK), tmGuildMarkImage, pSendMsg->GuildMarkSize);
		pSendMsg->GuildUID			= q.nGeneralParam1;
		q.pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_GUILDMARK_OK)+pSendMsg->GuildMarkSize);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_ArenaUpdateCharacterInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		�Ʒ��� ���� - �Ʒ��� �ɸ��� �̸� ������Ʈ, ���μ������� ȣ��
/// \author		dhjin
/// \date		2007-12-28 ~ 2007-12-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_ArenaUpdateCharacterInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_ArenaUpdateCharacterInfo
	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.arena_updateCharacterInfo
	-- DESC				: �Ʒ��� ���� - �ɸ��� ���� ������Ʈ
	-- 2007-12-28 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.arena_updateCharacterInfo
		-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
		--@o_NewCharacterUID			INT OUTPUT,
		@i_NewCharacterName			VARCHAR(20),
		@i_CharacterUID				INT,
		@i_AccountUniqueNumber		INT			
	AS
		-- // 
		IF EXISTS(SELECT * FROM atum2_db_arena.dbo.td_character WITH(NOLOCK) WHERE AccountUniqueNumber = @i_AccountUniqueNumber AND CharacterName = @i_NewCharacterName)
		BEGIN

			-- // ���� �����͸� ������Ʈ �Ѵ�.	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - LevelUpTime �߰�
			UPDATE atum2_db_arena.dbo.td_character
			SET	Race = sc.Race, UnitKind = sc.UnitKind, InfluenceType = sc.InfluenceType
				, PilotFace = sc.PilotFace, AttackPart = sc.AttackPart, DefensePart = sc.DefensePart, FuelPart = sc.FuelPart, SoulPart = sc.SoulPart
				, ShieldPart = sc.ShieldPart, DodgePart = sc.DodgePart, Level = sc.Level, BodyCondition = sc.BodyCondition, Propensity = sc.Propensity
				, Status = sc.Status, PKWinPoint = sc.PKWinPoint, PKLossPoint = sc.PKLossPoint, HP = sc.HP, DP = sc.DP, SP = sc.SP, EP = sc.EP
				, BonusStat = sc.BonusStat, BonusStatPoint = sc.BonusStatPoint, LevelUpTime = sc.LevelUpTime, WarPoint = sc.WarPoint, CumulativeWarPoint = sc.CumulativeWarPoint
				, ArenaWin = sc.ArenaWin,	ArenaLose = sc.ArenaLose, ArenaDisConnect = sc.ArenaDisConnect
			FROM dbo.td_character sc, atum2_db_arena.dbo.td_character tc
			WHERE tc.AccountUniqueNumber = @i_AccountUniqueNumber AND tc.CharacterName = @i_NewCharacterName AND sc.UniqueNumber = @i_CharacterUID
		END
		ELSE
		BEGIN

			-- // atum2_db_arena.dbo.td_character ���̺��� �ش� ĳ���͸����� ���� ó���Ѵ�. - ������ ĳ���͸��� ���� �� �� �ִ�. ������� �ʴ� �����̴�
			DELETE atum2_db_arena.dbo.td_character
			WHERE CharacterName = @i_NewCharacterName

			-- // ������ �߰��Ѵ�.	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - LevelUpTime �߰�
			INSERT INTO atum2_db_arena.dbo.td_character (CharacterName, AccountName, AccountUniqueNumber, Race, UnitKind, InfluenceType, PilotFace
						, AttackPart, DefensePart, FuelPart, SoulPart, ShieldPart, DodgePart, Level
						, BodyCondition, Propensity, Status, PKWinPoint, PKLossPoint, HP, DP, SP, EP, BonusStat
						, BonusStatPoint, LevelUpTime, WarPoint, CumulativeWarPoint, ArenaWin, ArenaLose, ArenaDisConnect)
				SELECT @i_NewCharacterName, AccountName, AccountUniqueNumber, Race, UnitKind, InfluenceType, PilotFace
					, AttackPart, DefensePart, FuelPart, SoulPart, ShieldPart, DodgePart, Level
					, BodyCondition, Propensity, Status, PKWinPoint, PKLossPoint, HP, DP, SP, EP
					, BonusStat, BonusStatPoint, LevelUpTime, WarPoint, CumulativeWarPoint, ArenaWin, ArenaLose, ArenaDisConnect
				FROM dbo.td_character
				WHERE UniqueNumber = @i_CharacterUID
		END

		-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
		--SET @o_NewCharacterUID = (SELECT UniqueNumber FROM atum2_db_arena.dbo.td_character WITH (NOLOCK) WHERE AccountUniqueNumber = @i_AccountUniqueNumber AND CharacterName = @i_NewCharacterName)
		SELECT UniqueNumber FROM atum2_db_arena.dbo.td_character WITH (NOLOCK) WHERE AccountUniqueNumber = @i_AccountUniqueNumber AND CharacterName = @i_NewCharacterName;
	GO
	************************************************************************/
	QPARAM_ARENA_UPDATE_CHARACTERINFO 	*pQParam	= (QPARAM_ARENA_UPDATE_CHARACTERINFO*)q.pQueryParam;
	CFieldIOCPSocket *pFISock						= q.pFieldIOCPSocket;
	if (NULL == pFISock || !q.pFieldIOCPSocket->IsValidCharacter(FALSE))
	{
		util::del(pQParam);
		return;
	}

	UID32_t	ArenaCharacterUID = 0;
	SQLINTEGER	arrCB[5] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};	
// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
// 	SQLBindParameter(hstmt, 1,SQL_PARAM_OUTPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &ArenaCharacterUID, 0, &arrCB[1]);
// 	SQLBindParameter(hstmt, 2,SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pQParam->NewArenaCharacterName, 0, &arrCB[2]);
// 	SQLBindParameter(hstmt, 3,SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pFISock->m_character.CharacterUniqueNumber, 0, &arrCB[3]);
// 	SQLBindParameter(hstmt, 4,SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pFISock->m_character.AccountUniqueNumber, 0, &arrCB[4]);
// 
// 	RETCODE ret = SQLExecDirect(hstmt, (UCHAR*)"{call arena_updateCharacterInfo(?,?,?,?)}", SQL_NTS);
// 	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
// 	{
// 		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::arena_updateCharacterInfo Fail!!\r\n");
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		INIT_MSG_WITH_BUFFER(MSG_FtoA_AUTH_CHARACTER_OK, T_FtoA_AUTH_CHARACTER_OK, pSendMsg, SendBuf);
// 		pSendMsg->ErrorCheck			= TRUE;
// 		pSendMsg->AccountUID			= pFISock->m_character.AccountUniqueNumber;
// 		pSendMsg->AFSClientIndex		= pQParam->AFSClientIndex;
// 		m_pFieldIOCP8->m_pArenaFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FtoA_AUTH_CHARACTER_OK));
// 		util::del(pQParam);
// 		return;
// 	}
// 
// 	while ( (ret = SQLMoreResults(hstmt)) != SQL_NO_DATA )
// 	{
// 
// 	}
// //	ret = SQLMoreResults(hstmt);
// //	ret = SQLFetch(hstmt);
// 	SQLFreeStmt(hstmt, SQL_CLOSE);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLBindParameter(hstmt, 1,SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pQParam->NewArenaCharacterName, 0, &arrCB[1]);
	SQLBindParameter(hstmt, 2,SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pFISock->m_character.CharacterUniqueNumber, 0, &arrCB[2]);
	SQLBindParameter(hstmt, 3,SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pFISock->m_character.AccountUniqueNumber, 0, &arrCB[3]);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0173, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::arena_updateCharacterInfo Fail!!\r\n");
		SQLFreeStmt(hstmt, SQL_CLOSE);
		INIT_MSG_WITH_BUFFER(MSG_FtoA_AUTH_CHARACTER_OK, T_FtoA_AUTH_CHARACTER_OK, pSendMsg, SendBuf);
		pSendMsg->ErrorCheck			= TRUE;
		pSendMsg->AccountUID			= pFISock->m_character.AccountUniqueNumber;
		pSendMsg->AFSClientIndex		= pQParam->AFSClientIndex;
		pSendMsg->ErrorMsgType			= T_FtoA_AUTH_CHARACTER;
		pSendMsg->ErrorCode				= ERR_DB_EXECUTION_FAILED;
		m_pFieldIOCP8->m_pArenaFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FtoA_AUTH_CHARACTER_OK));
		util::del(pQParam);
		return;
	}
	SQLBindCol(hstmt, 1, SQL_C_ULONG, &(ArenaCharacterUID), 0, NULL);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(hstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 �̸� ����� RowCount �̴� 
			ret = SQLMoreResults(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(hstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(hstmt);                 // 2008-07-25 by cmkwon, ���� Results �� �ִ����� �ѹ��� üũ�Ѵ�.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	////////////////////////////////////////////////////////////////////////////////
	// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : ĳ���� �Ʒ��� ���̺� ������ ����
	/////////////////////////////////////////////////////////////////////
	/// �ʿ��� ������ ����
	SArenaPlayInfo	ArenaPlayInfo;					/// ���� ĳ������ �Ʒ��� �÷��� ������ ���� ����ü
	ATUM_DATE_TIME	CurTime;						/// ���� �ð��� ������ ����
	SQL_TIMESTAMP_STRUCT LastPlayTime;				/// DB�� ����� ���� ĳ������ �ֱ� �Ʒ��� �÷��� �ð�
	int PlayCount = 0;								/// DB�� ����� ���� �ɸ��Ͱ� �÷����� �Ʒ��� PlayCount
	
	/////////////////////////////////////////////////////////////////////
	/// ������ �ʱ�ȭ
	util::zero(&ArenaPlayInfo, sizeof(SArenaPlayInfo));
	util::zero(&LastPlayTime, sizeof(SQL_TIMESTAMP_STRUCT));
	util::zero(&CurTime, sizeof(ATUM_DATE_TIME));
	CurTime.SetCurrentDateTime(FALSE);

#ifdef S_ARENA_EX_1ST_JHSEOL
	/************************************************************************
	---------------------------------------------------------------------
	-- PROCEDURE NAME: dbo.atum_arena_CharacterArena_Get(?,?)
	-- DESC: 2012-04-12 by jhseol, td_CharacterArena ������ �ε� ���ν���
	---------------------------------------------------------------------
	CREATE PROCEDURE [dbo].[atum_arena_CharacterArena_Get]
		@i_CharacterUID				INT,
		@i_AccountUID				INT			
	AS
	BEGIN
		SELECT	[PlayCount], [LastPlayTime]
		FROM	[dbo].[td_CharacterArena]
		WHERE	[CharacterUID] = @i_CharacterUID AND [AccountUID] = @i_AccountUID
	END
	GO
	************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pFISock->m_character.CharacterUniqueNumber,	0, &arrCB[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pFISock->m_character.AccountUniqueNumber,		0, &arrCB[2]);
	ret = SQLExecDirect(hstmt, PROCEDURE_120412_0002, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::arena_updateCharacterInfo In CharacterArena GET Fail!!\r\n");
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQParam);		// 2012-09-24 by jhseol, �ҽ����� - �޸� ���� ����
		return;
	}
	
	SQLBindCol(hstmt, 1, SQL_C_ULONG,		&PlayCount,		0, &arrCB[1]);
	SQLBindCol(hstmt, 2, SQL_C_TIMESTAMP,	&LastPlayTime,	0, &arrCB[2]);		// 2012-09-24 by jhseol, �ҽ����� - ��Ÿ����
	if( SQL_NO_DATA == (ret = SQLFetch(hstmt)) )
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] atum_arena_CharacterArena_Get SQLBindCol SQL_NO_DATA!! Arena Contents ON!! [CUID:%7d, AUID:%7d]\r\n", pFISock->m_character.CharacterUniqueNumber, pFISock->m_character.AccountUniqueNumber);	// 2012-12-13 by jhseol, �Ʒ��� ���� �����ڵ� �߰�
		// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : SQL_NO_DATA �̸� �Ʒ����� �ѹ��� �÷������� ���� �ɸ��������� ������ �߰��Ѵ�.
		SQLFreeStmt(hstmt, SQL_CLOSE);
		/************************************************************************
		---------------------------------------------------------------------
		-- PROCEDURE NAME: dbo.atum_arena_CharacterArena_Insert(?,?)
		-- DESC: 2012-04-12 by jhseol, td_CharacterArena ������ ��� ���ν���
		---------------------------------------------------------------------
		CREATE PROCEDURE [dbo].[atum_arena_CharacterArena_Insert]
			@i_CharacterUID				INT,
			@i_AccountUID				INT
		AS
		BEGIN
			INSERT INTO [dbo].[td_CharacterArena]([CharacterUID], [CharacterName], [AccountName], [AccountUID])
			SELECT UniqueNumber, CharacterName, AccountName, AccountUniqueNumber
			FROM td_Character
			WHERE UniqueNumber = @i_CharacterUID AND AccountUniqueNumber = @i_AccountUID
		END
		GO
		************************************************************************/
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pFISock->m_character.CharacterUniqueNumber,	0, &arrCB[1]);
		SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pFISock->m_character.AccountUniqueNumber,		0, &arrCB[2]);
		
		ret = SQLExecDirect(hstmt, PROCEDURE_120412_0001, SQL_NTS);
		if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::arena_updateCharacterInfo In CharacterArena INSERT Fail!!\r\n");
			SQLFreeStmt(hstmt, SQL_CLOSE);
			util::del(pQParam);		// 2012-09-24 by jhseol, �ҽ����� - �޸� ���� ����
			return;
		}
	}
	else
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] atum_arena_CharacterArena_Get SQLBindCol SQL_DATA!! [CUID:%7d, AUID:%7d]\r\n", pFISock->m_character.CharacterUniqueNumber, pFISock->m_character.AccountUniqueNumber);	// 2012-12-13 by jhseol, �Ʒ��� ���� �����ڵ� �߰�
		CurTime.Year	= (USHORT)LastPlayTime.year;
		CurTime.Month	= (BYTE)LastPlayTime.month;
		CurTime.Day		= (BYTE)LastPlayTime.day;
		CurTime.Hour	= (BYTE)LastPlayTime.hour;
		CurTime.Minute	= (BYTE)LastPlayTime.minute;
		CurTime.Second	= (BYTE)LastPlayTime.second;
		
		ArenaPlayInfo.nPlayCount = PlayCount;
		ArenaPlayInfo.atimeLastPlayTime = CurTime;

		pFISock->m_character.ArenaPlayInfo = ArenaPlayInfo;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
#endif
	// end  2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : ���� : ĳ���� �Ʒ��� ���̺� ������ ����
	//////////////////////////////////////////////////////////////////////////////////

	if(FALSE == IS_VALID_UNIQUE_NUMBER(ArenaCharacterUID))
	{// 2008-03-05 by cmkwon, üũ �߰�
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::arena_updateCharacterInfo Fail!!\r\n");
		INIT_MSG_WITH_BUFFER(MSG_FtoA_AUTH_CHARACTER_OK, T_FtoA_AUTH_CHARACTER_OK, pSendMsg, SendBuf);
		pSendMsg->ErrorCheck			= TRUE;
		pSendMsg->AccountUID			= pFISock->m_character.AccountUniqueNumber;
		pSendMsg->AFSClientIndex		= pQParam->AFSClientIndex;
		pSendMsg->ErrorMsgType			= T_FtoA_AUTH_CHARACTER;
		pSendMsg->ErrorCode				= ERR_PROTOCOL_INVALID_CHARACTER_UNIQUENUMBER;
		pSendMsg->ArenaPlayInfo			= pFISock->m_character.ArenaPlayInfo;	// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : ArenaPlayInfo �߰�
		m_pFieldIOCP8->m_pArenaFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FtoA_AUTH_CHARACTER_OK));
		util::del(pQParam);
		return;
	}

	// start 2011-11-03 by shcho, yedang �˴ٿ��� ���� - �Ʒ������� �˴ٿ��� ������
	BOOL Shutdown = FALSE;
	vectShutDownUserData::iterator itr(m_pFieldIOCP8->m_ShutDownData.begin());
	for(; itr != m_pFieldIOCP8->m_ShutDownData.end() ; itr++)
	{
		if(pFISock->m_character.AccountUniqueNumber == itr->Account_UniqueNumber)
		{
			Shutdown = TRUE;
			break;
		}
	}
	// üũ�� ���� �α׸� �����.
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify]SHUTDOWNMINS CHECKED!! MFAccountUID[ %d ] : MF_CLIENT_INDEX[ %d ]:AF_CLIENT_INDEX[ %d ] ShutdownFLAG[%d]\r\n", pFISock->m_character.AccountUniqueNumber, pFISock->m_character.ClientIndex, pQParam->AFSClientIndex, Shutdown);
	// end 2011-11-03 by shcho, yedang �˴ٿ��� ���� - �Ʒ������� �˴ٿ��� ������

	// 2007-12-28 by dhjin, �Ʒ��� ������ ��ȿ�� ������� ����� �����Ѵ�.
	INIT_MSG_WITH_BUFFER(MSG_FtoA_AUTH_CHARACTER_OK, T_FtoA_AUTH_CHARACTER_OK, pSendMsg, SendBuf);
	pSendMsg->AccountUID			= pFISock->m_character.AccountUniqueNumber;
	pSendMsg->AFSClientIndex		= pQParam->AFSClientIndex;
	pSendMsg->AFSCharacterUID		= ArenaCharacterUID;
	util::strncpy(pSendMsg->AFSCharacterName, pQParam->NewArenaCharacterName, SIZE_MAX_CHARACTER_NAME);
	pSendMsg->MFSClientIndex		= pFISock->m_character.ClientIndex;
	pSendMsg->MFSCharacterUID		= pFISock->m_character.CharacterUniqueNumber;
	util::strncpy(pSendMsg->MFSCharacterName, pFISock->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
	pSendMsg->ShutDownMins			= Shutdown; // 2011-11-03 by shcho, yedang �˴ٿ��� ���� - �Ʒ������� �˴ٿ��� ������ 
	pSendMsg->ArenaPlayInfo			= pFISock->m_character.ArenaPlayInfo;	// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : �Ʒ��� �÷��� ������ ������ ����ü ����.
	m_pFieldIOCP8->m_pArenaFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FtoA_AUTH_CHARACTER_OK));

// 2010-04-05 by cmkwon, ���� ������ ī�� ���� �ý��� ���� - 
// 	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ͼ� ������ �����Ѵ�.
// 	INIT_MSG_WITH_BUFFER(MSG_FtoA_INFINITY_IMPUTE_LIST, T_FtoA_INFINITY_IMPUTE_LIST, pSendImputeMsg, SendImputeBuf);
// 	pSendImputeMsg->AFSClientIndex		= pQParam->AFSClientIndex;
// 	pSendImputeMsg->bHaveReentryTicket	= (NULL == pFISock->m_ItemManager.GetItemGeneralByDestParam(DES_INFINITY_REENTRY_TICKET)) ? FALSE : TRUE;	// 2010-03-23 by cmkwon, ���Ǵ�Ƽ ���� ĳ�� ������ ���� - 
// 	int	InfinityImputelistCount = 0;
// 	INFINITY_IMPUTE * pInfinityImputeInfo = (INFINITY_IMPUTE*)(SendImputeBuf+MSG_SIZE(MSG_FtoA_INFINITY_IMPUTE_LIST));
// 	mt_auto_lock mtImpute(&pFISock->m_mtVecInfiImpute);
// 	mt_vectorInfinityImpute::iterator itr = pFISock->m_mtVecInfiImpute.begin();
// 	for(; itr != pFISock->m_mtVecInfiImpute.end(); itr++) {
// 		pInfinityImputeInfo[InfinityImputelistCount].InfinityModeUID	 = itr->InfinityModeUID;
// 		pInfinityImputeInfo[InfinityImputelistCount].EntranceCount		 = itr->EntranceCount;
// 		InfinityImputelistCount++;
// 	}
// 	mtImpute.auto_unlock_cancel();
// 
// 	pSendImputeMsg->InfinityImputeListCount = InfinityImputelistCount;
// 	if(0 < InfinityImputelistCount) {
// 		m_pFieldIOCP8->m_pArenaFieldWinSocket->Write(SendImputeBuf, MSG_SIZE(MSG_FtoA_INFINITY_IMPUTE_LIST) + InfinityImputelistCount * sizeof(INFINITY_IMPUTE));
// 	}
	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-05 by cmkwon, ���� ������ ī�� ���� �ý��� ���� - 
	pFISock->InfinitySendImputeList2AFS();
	
//	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENACHECK] arena_updateCharacterInfo ArenaCUID = %d, CUID = %d, AUID = %d\r\n", ArenaCharacterUID, pFISock->m_character.CharacterUniqueNumber, pFISock->m_character.AccountUniqueNumber);

	util::del(pQParam);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_ArenaGetCharacter(FIELD_DB_QUERY q, SQLHSTMT hstmt)	
/// \brief		�Ʒ��� ���� - �ɸ��� ���� �ε�, �Ʒ��� �������� ȣ��
/// \author		dhjin
/// \date		2007-12-29 ~ 2007-12-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_ArenaGetCharacter(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_ArenaGetCharacter

	CHARACTER retCharacter;
	memset(&retCharacter, 0x00, sizeof(CHARACTER));
	CHARACTER_DB_EX retCharacterDBEX;
	RETCODE ret;
	CFieldIOCPSocket *pFISock	= q.pFieldIOCPSocket;
	if (NULL == pFISock
		|| FALSE == pFISock->IsUsing())
	{
		// 2009-12-09 by cmkwon, �α� ���� �߰� - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] ArenaServer CAtumFieldDBManager::QP_ArenaGetCharacter# ClientIndex(%d) \r\n", pFISock->GetClientArrayIndex());
		return;
	}
	SArenaPlayInfo tempArenaInfo = q.pFieldIOCPSocket->m_character.ArenaPlayInfo;	// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : �Ʒ��� �÷��� ���� �ӽ�����
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE dbo.arena_GetCharacter
		@acc	INT,
		@charac	INT
	AS
		SELECT *
		FROM td_Character
		WITH (NOLOCK)
		WHERE UniqueNumber=@charac AND AccountUniqueNumber = @acc
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,
					&pFISock->m_character.AccountUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,
					&pFISock->m_character.CharacterUniqueNumber, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0174, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_NO_DATA) {
		// 2009-12-09 by cmkwon, �α� ���� �߰� - 
		//g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::arena_GetCharacter Fail!!\r\n");
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] ArenaServer CAtumFieldDBManager::arena_GetCharacter Fail!! ClientIndex(%d) \r\n", pFISock->GetClientArrayIndex());

		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	SQLINTEGER	cb[CB_COUNT_CHARACTER];
	fill_n(cb, CB_COUNT_CHARACTER, SQL_NTS);

	// bind columns
	CAtumDBManager::BindColCharacter(hstmt, retCharacter, retCharacterDBEX, cb);

	// initialize to 0x00
	memset(&retCharacter, 0x00, sizeof(CHARACTER));
	ret = SQLFetch(hstmt);
	SQLFreeStmt(hstmt, SQL_CLOSE);
	retCharacter				= retCharacterDBEX;
	_strlwr(retCharacter.AccountName);
	retCharacter.TotalGearStat	= retCharacter.GearStat;

	// 2012-10-17 by jhseol, ���� �̸����� ������ ON ��Ű��
#ifdef S_ITEM_VIEW_UNCONDITIONAL_ON
	retCharacter.SecretInfoOption = retCharacter.SecretInfoOption & USER_INFO_OPTION_ITEMINFO_DELETE_ALL_MASK;
#endif
	// end 2012-10-17 by jhseol, ���� �̸����� ������ ON ��Ű��

	// 2007-07-30 by cmkwon, SCAdminTool���� ���ܸ� ���� ��� �߰� - �Ʒ��� ���� ������, ���� üũ
	//if(ret == SQL_NO_DATA)
	if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		///////////////////////////////////////////////////////////////////////////
		// ĳ���Ͱ� ����, Error ó��
		// 2009-12-09 by cmkwon, �α� ���� �߰� - 
		//DBGOUT("ERROR! No Character At QT_ArenaGetCharacter(): A: %d, CH: %d\r\n", pFISock->m_character.AccountUniqueNumber, pFISock->m_character.CharacterUniqueNumber);
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] ArenaServer No Character At QT_ArenaGetCharacter(): A: %d, CH: %d\r\n", pFISock->m_character.AccountUniqueNumber, pFISock->m_character.CharacterUniqueNumber);
		pFISock->SendErrorMessage(T_FC_CONNECT_ARENASERVER_LOGIN, ERR_COMMON_NO_SUCH_CHARACTER);
		return;
	}

	pFISock->SetClientState(CS_GOTCHARACTER);				// 2007-12-29 by dhjin, �Ʒ��� ���� ���� �� �ɸ��� ����???
	util::zero(&pFISock->m_character, sizeof(CHARACTER));			// Character ����ü�� �ʱ�ȭ�Ѵ�.
	memcpy(&pFISock->m_character, &retCharacter, sizeof(CHARACTER));		// �Ʒ������� ����� Character
	pFISock->m_character.ClientIndex = pFISock->GetClientArrayIndex();	// Character ����ü�� �迭 �ε����� �����Ѵ�.
	pFISock->m_PeerSocketType = ST_CLIENT_TYPE;		
	pFISock->SetClientState(CS_PLAYING);
	memcpy(&pFISock->m_character.ArenaPlayInfo, &tempArenaInfo, sizeof(SArenaPlayInfo));	// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : �Ʒ��� �÷��� ���� �ӽ�����

	// 2008-01-16 by dhjin, ������ �� ������ �����Ѵ�.
	CFieldMapChannel *pFMChann = pFISock->GetCityFieldMapChannelPtr();
	if(NULL == pFMChann)
	{
		// 2009-12-09 by cmkwon, �α� ���� �߰� - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] ArenaServer No such CityFieldMapChann !! %s pFMChann(0x%X): CityMapIndex(%d) \r\n"
			, GetCharacterString(pFISock->GetCharacter(), string()), pFMChann, CAtumSJ::GetCityMapIndexByInfluence(retCharacter.InfluenceType, retCharacter.GetStartCityMapIndex()));
		return;
	}
	pFISock->m_character.MapChannelIndex	= pFMChann->GetMapChannelIndex();
	
	pFISock->SetCurrentFieldMapChannel(pFISock->GetFieldMapChannel(&pFISock->m_character.MapChannelIndex, TRUE, TRUE));
	if (pFISock->m_pCurrentFieldMapChannel == NULL)
	{
		// 2009-12-09 by cmkwon, �α� ���� �߰� - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] ArenaServer pCurrentFieldMapChannel !! %s MapChannelIdx(%d:%d) \r\n"
			, GetCharacterString(pFISock->GetCharacter(), string()), pFISock->GetCharacter()->MapChannelIndex.MapIndex, pFISock->GetCharacter()->MapChannelIndex.ChannelIndex);
		return;
	}
	
	m_pFieldIOCP8->m_mapCharacterUniqueNumber.insertLock(pFISock->m_character.CharacterUniqueNumber, pFISock);
	pFISock->m_pFieldParty			= NULL;			// party �ʱ�ȭ

	char tmCharacterName[SIZE_MAX_CHARACTER_NAME];
	m_pFieldIOCP8->m_mapCharacterName.insertLock(GGetLowerCase(tmCharacterName, pFISock->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME), pFISock);


	INIT_MSG_WITH_BUFFER(MSG_FC_CONNECT_ARENASERVER_LOGIN_OK, T_FC_CONNECT_ARENASERVER_LOGIN_OK, pSendMsg, SendBuf);
	memcpy(&pSendMsg->AFSCharacter, &pFISock->m_character, sizeof(CHARACTER));	// set character
	pSendMsg->atimeCurServerTime.SetCurrentDateTime();								// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��, ���� �Ʒ��� ���� ��¥ �ð�
	pFISock->SendAddData(SendBuf, MSG_SIZE(MSG_FC_CONNECT_ARENASERVER_LOGIN_OK));

	INIT_MSG_WITH_BUFFER(MSG_FC_CONNECT_ARENASERVER_SSERVER_GROUP_FOR_CLIENT, T_FC_CONNECT_ARENASERVER_SSERVER_GROUP_FOR_CLIENT, pServerGroupSendMsg, ServerGroupSendBuf);
	int	ServerGroupCount	= 0;


	char szAttachServerID[10];
	util::zero(szAttachServerID,10);
	CHAR ServerIDName[SIZE_MAX_CHARACTER_NAME];
	util::zero(ServerIDName, SIZE_MAX_CHARACTER_NAME);
	//strcat(ServerIDName, "\\");
	//strcat(ServerIDName, itoa(AttachServerID,szAttachServerID,10));
	util::strncpy(pServerGroupSendMsg->ServerGroupInfo[ServerGroupCount].MFS_ServerIDName, ServerIDName, SIZE_MAX_SERVER_NAME);
	util::strncpy(pServerGroupSendMsg->ServerGroupInfo[ServerGroupCount].MFS_Name, m_pFieldIOCP8->m_SARENA_USE_FIELD_SERVER_INFO.MFS_Name, SIZE_MAX_SERVER_NAME);
	ServerGroupCount++;


	q.pFieldIOCPSocket->SendAddData(ServerGroupSendBuf, MSG_SIZE(MSG_FC_CONNECT_ARENASERVER_SSERVER_GROUP_FOR_CLIENT));	

//	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENACHECK] arena_GetCharacter ArenaCUID = %d, CUID = %d, AUID = %d\r\n", pFISock->m_character.CharacterUniqueNumber, 0, pFISock->m_character.AccountUniqueNumber);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_ArenaCopyDBInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		�Ʒ��� ���� - �ɸ��� ���� �ε� , ���μ������� ȣ�� 
/// \author		dhjin
/// \date		2008-01-08 ~ 2008-01-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_ArenaCopyDBInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_ArenaCopyDBInfo
	/************************************************************************
	-------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.arena_CopyDBInfo
	-- DESC				: �Ʒ��� ���� - �Ʒ��� DB�� ����Ÿ ����
	-- 2008-01-07 by dhjin,
	--						-- // 2009-08-25 by cmkwon, ������� �ʴ� �ʵ� ����(td_Store.ScarcityNumber) - 
	--						-- // 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - td_Store.ShapeItemNum �ʵ� �߰�, �ʵ帮��Ʈ ���
	--						-- // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - Main���� ������ UID �߰�
	--						-- // 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
	--						-- // 2011-09-20 by hskim, ��Ʈ�� �ý��� 2�� - DB ���� ���� ���� ����
	--						-- // 2012-06-20 by jhseol, �Ʒ��� �߰�����part2 : �Ʒ��� ���� �ɸ��ͷ� ī�� �߰�
	--------------------------------------------------------------------------------
	************************************************************************/
	QPARAM_ARENA_COPY_DB_INFO 	*pQParam	= (QPARAM_ARENA_COPY_DB_INFO*)q.pQueryParam;
	CFieldIOCPSocket *pFISock				= q.pFieldIOCPSocket;
	if (NULL == pFISock 
		|| FALSE == pFISock->IsUsing() 
		|| ST_CLIENT_TYPE != pFISock->m_PeerSocketType
		|| CS_ARENASERVER_PLAYING != pFISock->GetClientState()		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - CS_ARENA_PLAYING => CS_ARENASERVER_PLAYING ����
		|| pFISock->GetCharacter()->AccountUniqueNumber != pQParam->AccountUID)
	{
		util::del(pQParam);
		return;
	}
// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -	
// 	INT	ErrCheck = 0;
// 	SQLINTEGER	arrCB[5] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
// 	SQLBindParameter(hstmt, 1,SQL_PARAM_OUTPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &ErrCheck, 0, &arrCB[1]);
// 	SQLBindParameter(hstmt, 2,SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQParam->ArenaCharacterUID, 0, &arrCB[2]);
// 	SQLBindParameter(hstmt, 3,SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQParam->CharacterUID, 0, &arrCB[3]);
// 	SQLBindParameter(hstmt, 4,SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQParam->AccountUID, 0, &arrCB[4]);
// 
// 	// 2008-02-29 by dhjin, �׽�Ʈ ��
// //	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENACHECK] arena_CopyDBInfo~S ArenaCUID = %d, CUID = %d, AUID = %d\r\n", pQParam->ArenaCharacterUID, pQParam->CharacterUID, pQParam->AccountUID);
// 	RETCODE ret = SQLExecDirect(hstmt, (UCHAR*)"{call arena_CopyDBInfo(?,?,?,?)}", SQL_NTS);
// 	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
// 	{
// 		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::arena_CopyDBInfo Fail!!\r\n");
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		INIT_MSG_WITH_BUFFER(MSG_FtoA_AUTH_CHARACTER_OK, T_FtoA_AUTH_CHARACTER_OK, pSendMsg, SendBuf);
// 		pSendMsg->ErrorCheck			= TRUE;
// 		pSendMsg->AccountUID			= pFISock->m_character.AccountUniqueNumber;
// 		pSendMsg->AFSClientIndex		= pQParam->AFSClientIndex;
// 		m_pFieldIOCP8->m_pArenaFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FtoA_AUTH_CHARACTER_OK));
// 		util::del(pQParam);
// 		return;
// 	}
// 	while ( (ret = SQLMoreResults(hstmt)) != SQL_NO_DATA ){}
// 	if(FALSE != ErrCheck)
// 	{
// 		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::arena_CopyDBInfo Fail!! ErrCount = %d, ArenaCharacterUID = %d, CharacterUID = %d, AccountUID = %d\r\n", ErrCheck, pQParam->ArenaCharacterUID, pQParam->CharacterUID, pQParam->AccountUID);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		INIT_MSG_WITH_BUFFER(MSG_FtoA_AUTH_CHARACTER_OK, T_FtoA_AUTH_CHARACTER_OK, pSendMsg, SendBuf);
// 		pSendMsg->ErrorCheck			= TRUE;
// 		pSendMsg->AccountUID			= pFISock->m_character.AccountUniqueNumber;
// 		pSendMsg->AFSClientIndex		= pQParam->AFSClientIndex;
// 		m_pFieldIOCP8->m_pArenaFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FtoA_AUTH_CHARACTER_OK));
// 		util::del(pQParam);
// 		return;
// 	}
// 	// 2008-02-29 by dhjin, �׽�Ʈ ��
// //	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENACHECK] arena_CopyDBInfo~E ArenaCUID = %d, CUID = %d, AUID = %d\r\n", pQParam->ArenaCharacterUID, pQParam->CharacterUID, pQParam->AccountUID);
// 	SQLFreeStmt(hstmt, SQL_CLOSE);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
 	INT	ErrCheck = 0;
 	SQLINTEGER	arrCB[6] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	// 2012-06-08 by jhseol, �Ʒ��� �߰�����part2 - �ɸ��� : Ÿ�� ����
	INT PLAYTYPE	= ARENA_PLAY_TYPE_NO;
#ifdef S_ARENA_EX_1ST_JHSEOL
	if(IS_BGEAR(pFISock->m_character.UnitKind))
	{
		PLAYTYPE	= pFISock->m_nPlayCharacterType + 0;
	}
	else if(IS_MGEAR(pFISock->m_character.UnitKind))
	{
		PLAYTYPE	= pFISock->m_nPlayCharacterType + 3;
	}
	else if(IS_IGEAR(pFISock->m_character.UnitKind))
	{
		PLAYTYPE	= pFISock->m_nPlayCharacterType + 6;
	}
	else if(IS_AGEAR(pFISock->m_character.UnitKind))
	{
		PLAYTYPE	= pFISock->m_nPlayCharacterType + 9;
	}
	// 2012-09-14 by jhseol, �Ʒ��� �߰����� restore - ������ ���� ���¿��� �ڽ��� �ɸ��ͷθ� �÷��� �ϵ��� ����
#ifdef S_ARENA_EX_1ST_RESTORE_JHSEOL
		PLAYTYPE = ARENA_PLAY_TYPE_NO;
#endif	//#ifdef S_ARENA_EX_1ST_RESTORE_JHSEOL
	// end 2012-09-14 by jhseol, �Ʒ��� �߰����� restore - ������ ���� ���¿��� �ڽ��� �ɸ��ͷθ� �÷��� �ϵ��� ����
#endif
	char SourceCharacterType[13][SIZE_MAX_CHARACTER_NAME] = { "",
		CHARACTER_TYPE_B_ATT,CHARACTER_TYPE_B_DEF,CHARACTER_TYPE_B_DOD,
		CHARACTER_TYPE_M_ATT,CHARACTER_TYPE_M_DEF,CHARACTER_TYPE_M_DOD,
		CHARACTER_TYPE_I_ATT,CHARACTER_TYPE_I_DEF,CHARACTER_TYPE_I_DOD,
		CHARACTER_TYPE_A_ATT,CHARACTER_TYPE_A_DEF,CHARACTER_TYPE_A_DOD};
	// end 2012-06-08 by jhseol, �Ʒ��� �߰�����part2 - �ɸ��� : Ÿ�� ����

	SQLBindParameter(hstmt, 1,SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQParam->ArenaCharacterUID, 0, &arrCB[1]);
	SQLBindParameter(hstmt, 2,SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQParam->CharacterUID, 0, &arrCB[2]);
	SQLBindParameter(hstmt, 3,SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQParam->AccountUID, 0, &arrCB[3]);
	// 2012-06-08 by jhseol, �Ʒ��� �߰�����part2 - �ɸ��� : Ÿ�� ����
	SQLBindParameter(hstmt, 4,SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &PLAYTYPE, 0, &arrCB[4]);
	SQLBindParameter(hstmt, 5,SQL_PARAM_INPUT, SQL_C_CHAR,  SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, SourceCharacterType[PLAYTYPE], 0, &arrCB[5]);
	// end 2012-06-08 by jhseol, �Ʒ��� �߰�����part2 - �ɸ��� : Ÿ�� ����
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0175, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::arena_CopyDBInfo Fail!!\r\n");
		SQLFreeStmt(hstmt, SQL_CLOSE);
		INIT_MSG_WITH_BUFFER(MSG_FtoA_AUTH_CHARACTER_OK, T_FtoA_AUTH_CHARACTER_OK, pSendMsg, SendBuf);
		pSendMsg->ErrorCheck			= TRUE;
		pSendMsg->AccountUID			= pFISock->m_character.AccountUniqueNumber;
		pSendMsg->AFSClientIndex		= pQParam->AFSClientIndex;
		pSendMsg->ErrorMsgType			= T_FtoA_AUTH_CHARACTER;
		pSendMsg->ErrorCode				= ERR_DB_EXECUTION_FAILED;
		m_pFieldIOCP8->m_pArenaFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FtoA_AUTH_CHARACTER_OK));
		util::del(pQParam);
		return;
	}
	SQLBindCol(hstmt, 1, SQL_C_LONG, &(ErrCheck), 0, NULL);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(hstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 �̸� ����� RowCount �̴� 
			ret = SQLMoreResults(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(hstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(hstmt);                 // 2008-07-25 by cmkwon, ���� Results �� �ִ����� �ѹ��� üũ�Ѵ�.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}
	if(FALSE != ErrCheck)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::arena_CopyDBInfo Fail!! ErrCount = %d, ArenaCharacterUID = %d, CharacterUID = %d, AccountUID = %d\r\n", ErrCheck, pQParam->ArenaCharacterUID, pQParam->CharacterUID, pQParam->AccountUID);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		INIT_MSG_WITH_BUFFER(MSG_FtoA_AUTH_CHARACTER_OK, T_FtoA_AUTH_CHARACTER_OK, pSendMsg, SendBuf);
		pSendMsg->ErrorCheck			= TRUE;
		pSendMsg->AccountUID			= pFISock->m_character.AccountUniqueNumber;
		pSendMsg->AFSClientIndex		= pQParam->AFSClientIndex;
		pSendMsg->ErrorMsgType			= T_FtoA_AUTH_CHARACTER;
		pSendMsg->ErrorCode				= ERR_PROTOCOL_INVALID_CHARACTER_UNIQUENUMBER;
		m_pFieldIOCP8->m_pArenaFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FtoA_AUTH_CHARACTER_OK));
		util::del(pQParam);
		return;
	}
	// 2008-02-29 by dhjin, �׽�Ʈ ��
	// 2012-09-21 by jhseol, �Ʒ��� �߰����� GF����
#ifdef S_ARENA_EX_1ST_JHSEOL
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] APPLY ARENA MODE [S_ARENA_EX_1ST_JHSEOL] [arenaCIUD:%7d, CUID:%7d, AUID:%7d]\r\n", pQParam->ArenaCharacterUID, pQParam->CharacterUID, pQParam->AccountUID);	// 2012-12-13 by jhseol, �Ʒ��� ���� �����ڵ� �߰�
#ifdef S_ARENA_EX_1ST_RESTORE_JHSEOL
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] APPLY ARENA MODE [S_ARENA_EX_1ST_RESTORE_JHSEOL]\r\n");
#else
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] DOES NOT APPLY ARENA MODE [S_ARENA_EX_1ST_RESTORE_JHSEOL]\r\n");
#endif	//#ifdef S_ARENA_EX_1ST_RESTORE_JHSEOL
#endif	//#ifdef S_ARENA_EX_1ST_JHSEOL
	// end 2012-09-21 by jhseol, �Ʒ��� �߰����� GF����
//	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENACHECK] arena_CopyDBInfo~E ArenaCUID = %d, CUID = %d, AUID = %d\r\n", pQParam->ArenaCharacterUID, pQParam->CharacterUID, pQParam->AccountUID);
	SQLFreeStmt(hstmt, SQL_CLOSE);

	// 2007-12-28 by dhjin, �Ʒ��� ������ ���簡 �Ϸ� �Ǿ����� �˷��ش�.
	INIT_MSG_WITH_BUFFER(MSG_FtoA_ARENA_TEAM_MATCHING_OK, T_FtoA_ARENA_TEAM_MATCHING_OK, pSendMsg, SendBuf);
	pSendMsg->AFSClientIndex		= pQParam->AFSClientIndex;
	pSendMsg->AccountUID			= pQParam->AccountUID;
	pSendMsg->AFSCharacterUID		= pQParam->ArenaCharacterUID;
	m_pFieldIOCP8->m_pArenaFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FtoA_ARENA_TEAM_MATCHING_OK));

	util::del(pQParam);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_ArenaStartGetCharacter(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		// 2008-01-09 by dhjin, �Ʒ��� ���� - �Ʒ��� ������ ���� �ɸ��� ���� �ٽ� �ε��Ͽ� Ŭ���̾�Ʈ���� ����, �Ʒ��� �������� ȣ�� 
/// \author		dhjin
/// \date		2008-01-09 ~ 2008-01-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_ArenaStartGetCharacter(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_ArenaStartGetCharacter

	CFieldIOCPSocket *pFISock	= q.pFieldIOCPSocket;
	if (NULL == pFISock)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] Socket error !! FISock(0x%X) \r\n", pFISock);
		return;
	}

	// 2012-12-18 by jhseol, �Ʒ��� ĳ���� ���� �� ĳ���� ���� �ٽ� �ε��ϱ�.
	if( TRUE == m_functionArenaCharacterLoad(pFISock, hstmt) )	// ������ �ִ� �ڵ带 �Լ��� ����.
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] CAtumFieldDBManager::QP_ArenaStartGetCharacter, Success !! AUID(%d) CUID(%d)\r\n", pFISock->m_character.AccountUniqueNumber, pFISock->m_character.CharacterUniqueNumber);
	}
	else
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::QP_ArenaStartGetCharacter, Fail !! AUID(%d) CUID(%d)\r\n", pFISock->m_character.AccountUniqueNumber, pFISock->m_character.CharacterUniqueNumber);
		return;
	}
	// end 2012-12-18 by jhseol, �Ʒ��� ĳ���� ���� �� ĳ���� ���� �ٽ� �ε��ϱ�.

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ذ� ���� ����
//	if(FALSE == pFISock->SetDataReadyArenaGameStart())
//	{
//		pFISock->SendErrorMessage(T_FC_ARENA_POSSIBLE_GAMESTART, ERR_INVALID_ARENASERVER_CHARACTER);
//		return;	
//	}
	if(INFINITY_STATE_READY <= pFISock->m_InfinityPlayingInfo.InfinityState)
	{
		// ���Ǵ�Ƽ ���� ���� ó��.
		if(FALSE == pFISock->SetDataReadyInfinityGameStart())
		{
			pFISock->SendErrorMessage(T_FC_ARENA_POSSIBLE_GAMESTART, ERR_INVALID_ARENASERVER_CHARACTER);
			return;
		}
	}
	else
	{
		if(FALSE == pFISock->SetDataReadyArenaGameStart())
		{
			pFISock->SendErrorMessage(T_FC_ARENA_POSSIBLE_GAMESTART, ERR_INVALID_ARENASERVER_CHARACTER);
			return;
		}
	}
	

//	// 2007-05-18 by cmkwon, �׽�Ʈ �α�
//	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENACHECK] SetDataReadyArenaGameStart ArenaCUID = %d, CUID = %d, AUID = %d\r\n", pFISock->m_character.CharacterUniqueNumber, 0, pFISock->m_character.AccountUniqueNumber);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		����, ������ ����â ��ȹ�� - 
/// \author		dhjin
/// \date		2008-04-02 ~ 2008-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_GetLogMSWarInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_GetLogMSWarInfo
	CFieldIOCPSocket *pFISock	= q.pFieldIOCPSocket;
	if (NULL == pFISock)
	{
		return;
	}

	SMSWAR_INFO_RESULT	MSWarInfoResult;
	util::zero(&MSWarInfoResult, sizeof(SMSWAR_INFO_RESULT));
	vectSMSWAR_INFO_RESULT	vectMSwarInfoResult;
	vectMSwarInfoResult.clear();

// 2008-08-28 by dhjin, ���� ����, ����DB�� ���ܾ� Admintool�� �ʱ�ȭ�� �����ϴ�.
//	CODBCStatement logDBOdbcStmt;
//	BOOL bRet = logDBOdbcStmt.Init((LPCSTR)g_pFieldGlobal->GetLogDBODBCDSN(), (LPCSTR)g_pFieldGlobal->GetLogDBODBCUID(), (LPCSTR)g_pFieldGlobal->GetLogDBODBCPASSWORD());
//	if (!bRet)
//	{
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::atum_log_Get_SPWar Can't connect to LogDBServer(%s, %s, %s) !!\r\n"
//			, (LPCSTR)g_pFieldGlobal->GetLogDBODBCDSN(), (LPCSTR)g_pFieldGlobal->GetLogDBODBCUID(), (LPCSTR)g_pFieldGlobal->GetLogDBODBCPASSWORD());
//		return;
//	}

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_log_Get_MSWar
	-- DESC				: ���� �α� ��������
	-- // 2008-04-01 by dhjin, ����, ������ ����â ��ȹ�� - 
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_log_Get_MSWar
	AS
		SELECT   AttInfluence, MonsterUID, ContributionPoint, MSWarStartTime, MSWarEndTime, WinInfluence
		FROM     dbo.atum_log_MSWar With (nolock)
	GO
	**************************************************************************/
//	SQLHSTMT logDBstmt = logDBOdbcStmt.GetSTMTHandle();
	RETCODE	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0177, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_log_Get_MSWar Failed!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	
	SQLINTEGER	cb[7] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS ,SQL_NTS,SQL_NTS};

	SQL_TIMESTAMP_STRUCT	MSWarStartTimeStamp;	util::zero(&MSWarStartTimeStamp, sizeof(MSWarStartTimeStamp));
	SQL_TIMESTAMP_STRUCT	MSWarEndTimeStamp;		util::zero(&MSWarEndTimeStamp, sizeof(MSWarEndTimeStamp));

	SQLBindCol(hstmt, 1, SQL_C_UTINYINT, &MSWarInfoResult.AttInfluence, 0,				&cb[1]);
	SQLBindCol(hstmt, 2, SQL_C_ULONG, &MSWarInfoResult.MonsterUID, 0,					&cb[2]);
	SQLBindCol(hstmt, 3, SQL_C_ULONG, &MSWarInfoResult.ContributionPoint, 0,			&cb[3]);
	SQLBindCol(hstmt, 4, SQL_C_TIMESTAMP, &MSWarStartTimeStamp, 0,						&cb[4]);
	SQLBindCol(hstmt, 5, SQL_C_TIMESTAMP, &MSWarEndTimeStamp, 0,						&cb[5]);
	SQLBindCol(hstmt, 6, SQL_C_UTINYINT, &MSWarInfoResult.WinInfluence, 0,				&cb[6]);

	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA)
	{
		MSWarInfoResult.MSWarStartTime	= MSWarStartTimeStamp;
		MSWarInfoResult.MSWarEndTime	= MSWarEndTimeStamp;
		vectMSwarInfoResult.push_back(MSWarInfoResult);
		util::zero(&MSWarInfoResult, sizeof(SMSWAR_INFO_RESULT));
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	pFISock->SendMSWarInfoResult(&vectMSwarInfoResult);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		����, ������ ����â ��ȹ�� - 
/// \author		dhjin
/// \date		2008-04-02 ~ 2008-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_GetLogSPWarInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_GetLogSPWarInfo
	CFieldIOCPSocket *pFISock	= q.pFieldIOCPSocket;
	if (NULL == pFISock)
	{
		return;
	}

	SSPWAR_INFO_RESULT	SPWarInfoResult;
	util::zero(&SPWarInfoResult, sizeof(SSPWAR_INFO_RESULT));
	vectSSPWAR_INFO_RESULT	vectSPwarInfoResult;
	vectSPwarInfoResult.clear();

// 2008-08-28 by dhjin, ���� ����, ����DB�� ���ܾ� Admintool�� �ʱ�ȭ�� �����ϴ�.
//	CODBCStatement logDBOdbcStmt;
//	BOOL bRet = logDBOdbcStmt.Init((LPCSTR)g_pFieldGlobal->GetLogDBODBCDSN(), (LPCSTR)g_pFieldGlobal->GetLogDBODBCUID(), (LPCSTR)g_pFieldGlobal->GetLogDBODBCPASSWORD());
//	if (!bRet)
//	{
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::atum_log_Get_SPWar Can't connect to LogDBServer(%s, %s, %s) !!\r\n"
//			, (LPCSTR)g_pFieldGlobal->GetLogDBODBCDSN(), (LPCSTR)g_pFieldGlobal->GetLogDBODBCUID(), (LPCSTR)g_pFieldGlobal->GetLogDBODBCPASSWORD());
//		return;
//	}

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_log_Get_SPWar
	-- DESC				: ������ �α� ��������
	-- // 2008-04-01 by dhjin, ����, ������ ����â ��ȹ�� - 
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_log_Get_SPWar
	AS
		SELECT   AttInfluence, SPSummonMapIndex, SPWarStartTime, SPWarEndTime, WinInfluence
		FROM     dbo.atum_log_SPWar With (nolock)
	GO
	**************************************************************************/
//	SQLHSTMT logDBstmt = logDBOdbcStmt.GetSTMTHandle();
	RETCODE	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0178, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_log_Get_SPWar Failed!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	SQLINTEGER	cb[6] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS ,SQL_NTS};

	SQL_TIMESTAMP_STRUCT	SPWarStartTimeStamp;	util::zero(&SPWarStartTimeStamp, sizeof(SPWarStartTimeStamp));
	SQL_TIMESTAMP_STRUCT	SPWarEndTimeStamp;		util::zero(&SPWarEndTimeStamp, sizeof(SPWarEndTimeStamp));

	SQLBindCol(hstmt, 1, SQL_C_UTINYINT, &SPWarInfoResult.AttInfluence, 0,			&cb[1]);
	SQLBindCol(hstmt, 2, SQL_C_ULONG, &SPWarInfoResult.SPSummonMapIndex, 0,			&cb[2]);
	SQLBindCol(hstmt, 3, SQL_C_TIMESTAMP, &SPWarStartTimeStamp, 0,					&cb[3]);
	SQLBindCol(hstmt, 4, SQL_C_TIMESTAMP, &SPWarEndTimeStamp, 0,					&cb[4]);
	SQLBindCol(hstmt, 5, SQL_C_UTINYINT, &SPWarInfoResult.WinInfluence, 0,			&cb[5]);

	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA)
	{
		SPWarInfoResult.SPWarStartTime	= SPWarStartTimeStamp;
		SPWarInfoResult.SPWarEndTime	= SPWarEndTimeStamp;
		vectSPwarInfoResult.push_back(SPWarInfoResult);
		util::zero(&SPWarInfoResult, sizeof(SSPWAR_INFO_RESULT));
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	
	// 2008-04-02 by dhjin, ����
	pFISock->SendSPWarInfoResult(&vectSPwarInfoResult);
}

#ifdef NEMERIAN_NATION_BALANCE_BUFF_PENALITY
void CAtumFieldDBManager::QP_GetWarVictoryDiff(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	SSPWAR_INFO_RESULT	SPWarInfoResult;
	util::zero(&SPWarInfoResult, sizeof(SSPWAR_INFO_RESULT));
	vectSSPWAR_INFO_RESULT	vectSPwarInfoResult;
	vectSPwarInfoResult.clear();
	RETCODE	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0178, SQL_NTS);
	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "atum_log_Get_SPWar Failed!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	SQLINTEGER	cb[6] = { SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS };

	SQL_TIMESTAMP_STRUCT	SPWarStartTimeStamp;	util::zero(&SPWarStartTimeStamp, sizeof(SPWarStartTimeStamp));
	SQL_TIMESTAMP_STRUCT	SPWarEndTimeStamp;		util::zero(&SPWarEndTimeStamp, sizeof(SPWarEndTimeStamp));

	SQLBindCol(hstmt, 1, SQL_C_UTINYINT, &SPWarInfoResult.AttInfluence, 0, &cb[1]);
	SQLBindCol(hstmt, 2, SQL_C_ULONG, &SPWarInfoResult.SPSummonMapIndex, 0, &cb[2]);
	SQLBindCol(hstmt, 3, SQL_C_TIMESTAMP, &SPWarStartTimeStamp, 0, &cb[3]);
	SQLBindCol(hstmt, 4, SQL_C_TIMESTAMP, &SPWarEndTimeStamp, 0, &cb[4]);
	SQLBindCol(hstmt, 5, SQL_C_UTINYINT, &SPWarInfoResult.WinInfluence, 0, &cb[5]);

	while ((ret = SQLFetch(hstmt)) != SQL_NO_DATA)
	{
		SPWarInfoResult.SPWarStartTime = SPWarStartTimeStamp;
		SPWarInfoResult.SPWarEndTime = SPWarEndTimeStamp;
		vectSPwarInfoResult.push_back(SPWarInfoResult);
		util::zero(&SPWarInfoResult, sizeof(SSPWAR_INFO_RESULT));
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	SMSWAR_INFO_RESULT	MSWarInfoResult;
	util::zero(&MSWarInfoResult, sizeof(SMSWAR_INFO_RESULT));
	vectSMSWAR_INFO_RESULT	vectMSwarInfoResult;
	vectMSwarInfoResult.clear();

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0177, SQL_NTS);
	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "atum_log_Get_MSWar Failed!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	SQLINTEGER	cb2[7] = { SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS };

	SQL_TIMESTAMP_STRUCT	MSWarStartTimeStamp;	util::zero(&MSWarStartTimeStamp, sizeof(MSWarStartTimeStamp));
	SQL_TIMESTAMP_STRUCT	MSWarEndTimeStamp;		util::zero(&MSWarEndTimeStamp, sizeof(MSWarEndTimeStamp));

	SQLBindCol(hstmt, 1, SQL_C_UTINYINT, &MSWarInfoResult.AttInfluence, 0, &cb2[1]);
	SQLBindCol(hstmt, 2, SQL_C_ULONG, &MSWarInfoResult.MonsterUID, 0, &cb2[2]);
	SQLBindCol(hstmt, 3, SQL_C_ULONG, &MSWarInfoResult.ContributionPoint, 0, &cb2[3]);
	SQLBindCol(hstmt, 4, SQL_C_TIMESTAMP, &MSWarStartTimeStamp, 0, &cb2[4]);
	SQLBindCol(hstmt, 5, SQL_C_TIMESTAMP, &MSWarEndTimeStamp, 0, &cb2[5]);
	SQLBindCol(hstmt, 6, SQL_C_UTINYINT, &MSWarInfoResult.WinInfluence, 0, &cb2[6]);

	while ((ret = SQLFetch(hstmt)) != SQL_NO_DATA)
	{
		MSWarInfoResult.MSWarStartTime = MSWarStartTimeStamp;
		MSWarInfoResult.MSWarEndTime = MSWarEndTimeStamp;
		vectMSwarInfoResult.push_back(MSWarInfoResult);
		util::zero(&MSWarInfoResult, sizeof(SMSWAR_INFO_RESULT));
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	//compute vicoty diff for sp and ms
	int diffvictory = 0;
	for (std::vector<SSPWAR_INFO_RESULT>::iterator it = vectSPwarInfoResult.begin(); it != vectSPwarInfoResult.end(); ++it)
	{
		if ((*it).WinInfluence == 2)
		{
			diffvictory++;
		}
		else if ((*it).WinInfluence == 4)
		{
			diffvictory--;
		}
	}
	for (std::vector<SMSWAR_INFO_RESULT>::iterator it = vectMSwarInfoResult.begin(); it != vectMSwarInfoResult.end(); ++it)
	{
		if ((*it).WinInfluence == 2)
		{
			diffvictory++;
		}
		else if ((*it).WinInfluence == 4)
		{
			diffvictory--;
		}
	}

	int diff = diffvictory;
	// when a nation get penality level we cancel active buff before change the victory diff used to compute penality 
	if ((diff % 22) == 0 && diffvictory > 0) {
		m_pFieldIOCP8->ResetAllBuffSkill(2);
	}
	else if ((diff % 22) == 0 && diffvictory < 0) {
		m_pFieldIOCP8->ResetAllBuffSkill(4);
	}
	g_diffVictoryWarByroo = diffvictory;
}
#endif
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_UpdateMSWarOptionType(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		
/// \author		dhjin
/// \date		2008-04-10 ~ 2008-04-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_UpdateMSWarOptionType(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_UpdateMSWarOptionType
	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Update_MSWarOptionType
	-- DESC				: ������ ����Ʈ �ʱ�ȭ ���ν���
	-- // 2008-04-10 by dhjin, ���� ���� ǥ�� ��ȹ�� - 
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Update_MSWarOptionType
		@i_InfluenceType		TINYINT,
		@i_MSWarOptionType		SMALLINT
	AS
		UPDATE dbo.td_InfluenceWarData SET MSWarOptionType = @i_MSWarOptionType WHERE InfluenceType = @i_InfluenceType
	GO
	************************************************************************/
	QPARAM_UPDATE_MSWAR_OPTION_TYPE		*pQPMsg = (QPARAM_UPDATE_MSWAR_OPTION_TYPE*)q.pQueryParam;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQPMsg->InfluenceType, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pQPMsg->MSWarOptionType, 0, NULL);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0179, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Update_MSWarOptionType Failed!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CAtumFieldDBManager::QP_InsertMSWarLog(FIELD_DB_QUERY q, SQLHSTMT hstmt)			
/// \brief		���� ����, ����DB�� ���ܾ� Admintool�� �ʱ�ȭ�� �����ϴ�.
/// \author		dhjin
/// \date		2008-08-28 ~ 2008-08-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_InsertMSWarLog(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_log_Insert_MSWar
	-- DESC				: ���� �α� �߰�
	-- // 2008-04-01 by dhjin, ����, ������ ����â ��ȹ�� - 
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_log_Insert_MSWar
		@i_LogType					TINYINT,
		@i_AttInfluence				TINYINT,
		@i_MonsterUID				INT,
		@i_ContributionPoint		INT,
		@i_MSWarStartTime			VARCHAR(30),
		@i_WinInfluence				TINYINT
	AS
		INSERT INTO dbo.atum_log_MSWar(LogType, AttInfluence, MonsterUID, ContributionPoint, MSWarStartTime, MSWarEndTime, WinInfluence)
		VALUES(@i_LogType, @i_AttInfluence, @i_MonsterUID, @i_ContributionPoint, @i_MSWarStartTime, GetDate(), @i_WinInfluence)
	GO
	**************************************************************************/
	QPARAM_INSERT_MSWAR_LOG *pRMsg = (QPARAM_INSERT_MSWAR_LOG*)q.pQueryParam;

	auto logType = T1_FL_LOG_MS_WAR;

	char tmMSWarStartTime[SIZE_MAX_SQL_DATETIME_STRING];

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &logType, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRMsg->AttInfluence, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->MonsterUID, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->ContributionPoint, 0, NULL);
	pRMsg->MSWarStartTime.GetSQLDateTimeString(tmMSWarStartTime, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmMSWarStartTime, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRMsg->WinInfluence, 0, NULL);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0263, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "dbo.atum_log_Insert_MSWar Failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_InsertSPWarLog(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		���� ����, ����DB�� ���ܾ� Admintool�� �ʱ�ȭ�� �����ϴ�.
/// \author		dhjin
/// \date		2008-08-28 ~ 2008-08-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_InsertSPWarLog(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_log_Insert_SPWar
	-- DESC				: ������ �α� �߰�
	-- // 2008-04-01 by dhjin, ����, ������ ����â ��ȹ�� - 
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_log_Insert_SPWar
		@i_LogType					TINYINT,
		@i_AttInfluence				TINYINT,
		@i_SPSummonMapIndex			INT,
		@i_SPWarStartTime			VARCHAR(30),
		@i_WinInfluence				TINYINT
	AS
		INSERT INTO dbo.atum_log_SPWar(LogType, AttInfluence, SPSummonMapIndex, SPWarStartTime, SPWarEndTime, WinInfluence)
		VALUES(@i_LogType, @i_AttInfluence, @i_SPSummonMapIndex, @i_SPWarStartTime, GetDate(), @i_WinInfluence)
	GO
	**************************************************************************/
	QPARAM_INSERT_SPWAR_LOG *pRMsg = (QPARAM_INSERT_SPWAR_LOG*)q.pQueryParam;

	auto logType = T1_FL_LOG_SP_WAR;

	char tmSPWarStartTime[SIZE_MAX_SQL_DATETIME_STRING];
	pRMsg->SPWarStartTime.GetSQLDateTimeString(tmSPWarStartTime, SIZE_MAX_SQL_DATETIME_STRING);
	
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &logType, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRMsg->AttInfluence, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->SPSummonMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmSPWarStartTime, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRMsg->WinInfluence, 0, NULL);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0264, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "dbo.atum_log_Insert_SPWar Failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_UpdateDBServerGroup(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - CAtumFieldDBManager::QP_UpdateDBServerGroup() �߰�
/// \author		cmkwon
/// \date		2008-04-29 ~ 2008-04-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_UpdateDBServerGroup(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// 2008-04-29 by cmkwon, QT_UpdateDBServerGroup

	SDBSERVER_GROUP		*pQPMsg		= (SDBSERVER_GROUP*)q.pQueryParam;

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_UpdateServerGroupInfo
	-- DESC				: ServerGroup ���� ���� �ε�
	-- // 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - 
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_UpdateServerGroupInfo
		@i_ServGroupID							INT,
		@i_ServGroupName						VARCHAR(20),
		@i_LimitUserCount						INT,
		@i_LockCreateCharacterForNewAccount		INT
	AS
		IF EXISTS(SELECT * FROM dbo.td_serverGroup WITH(NOLOCK) WHERE ServerGroupID = @i_ServGroupID)
		BEGIN
			UPDATE dbo.td_serverGroup
			SET ServerGroupName= @i_ServGroupName, LimitUserCount= @i_LimitUserCount, LockCreateCharacterForNewAccount= @i_LockCreateCharacterForNewAccount	
			WHERE ServerGroupID = @i_ServGroupID
		END
		ELSE
		BEGIN
			INSERT INTO dbo.td_serverGroup(ServerGroupID, ServerGroupName, LimitUserCount, LockCreateCharacterForNewAccount)
			VALUES(@i_ServGroupID, @i_ServGroupName, @i_LimitUserCount, @i_LockCreateCharacterForNewAccount)			
		END
			
	GO
	************************************************************************/
	SQLINTEGER	arrCB[5] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	//SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->ServerGroupID, 0,						&arrCB[1]);
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SERVER_NAME, 0, pQPMsg->ServerGroupName, 0,	&arrCB[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->LimitUserCount, 0,						&arrCB[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->LockCreateCharacterForNewAccount, 0,	&arrCB[3]);
	RETCODE	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0041, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateServerGroupInfo_ Failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	server::log(true, "[Notify] UpdateServerGroupInfo# success !!, %s, %d %d \r\n", 
		pQPMsg->ServerGroupName, pQPMsg->LimitUserCount, pQPMsg->LockCreateCharacterForNewAccount);

	util::del(pQPMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_CheckConnectableAccount(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - CAtumFieldDBManager::QP_CheckConnectableAccount() �߰�
/// \author		cmkwon
/// \date		2008-04-29 ~ 2008-04-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_CheckConnectableAccount(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// 2008-04-29 by cmkwon, QT_CheckConnectableAccount
	MSG_FP_CONNECT_CHECK_CONNECTABLE_ACCOUNT	*pQPMsg		= (MSG_FP_CONNECT_CHECK_CONNECTABLE_ACCOUNT*)q.pQueryParam;

	INIT_MSG_WITH_BUFFER(MSG_FP_CONNECT_CHECK_CONNECTABLE_ACCOUNT_OK, T_FP_CONNECT_CHECK_CONNECTABLE_ACCOUNT_OK, pSMsg, SendBuf);
	util::strncpy(pSMsg->AccountName, pQPMsg->AccountName, SIZE_MAX_ACCOUNT_NAME);
	pSMsg->PCConnectLoginOK		= pQPMsg->PCConnectLoginOK;

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_GetNumOfCharacter
	-- DESC				: ĳ������ ������ �����´�. ���� ������  ĳ���� ����.
	-- // 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - 
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_GetNumOfCharacter
		@i_AccName			VARCHAR(20)
	AS
		SELECT COUNT(*) 
		FROM dbo.td_character WITH(NOLOCK)
		WHERE AccountName = @i_AccName
	GO
	************************************************************************/
	SQLINTEGER	arrCB[2] = {SQL_NTS, SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, pQPMsg->AccountName, 0,	&arrCB[1]);
	RETCODE	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0181, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"dbo.atum_GetNumOfCharacter_ Failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		
		pSMsg->ErrorCode		= ERR_DB_EXECUTION_FAILED;
		// 2008-04-30 by cmkwon, PreServer �� ��� ����
		m_pFieldIOCP8->Send2PreServerW(SendBuf, MSG_SIZE(MSG_FP_CONNECT_CHECK_CONNECTABLE_ACCOUNT_OK));

		util::del(pQPMsg);
		return;
	}
	int nNumOfCharacter = 0;	// ���� ������ ĳ���� ����
	arrCB[1] = SQL_NTS;
	SQLBindCol(hstmt, 1, SQL_C_LONG, &nNumOfCharacter, 0,	&arrCB[1]);
	ret = SQLFetch(hstmt);
	SQLFreeStmt(hstmt, SQL_CLOSE);

	if(0 >= nNumOfCharacter)
	{
		pSMsg->ErrorCode		= ERR_NO_SEARCH_CHARACTER;
	}

	// 2008-04-30 by cmkwon, PreServer �� ��� ����
	m_pFieldIOCP8->Send2PreServerW(SendBuf, MSG_SIZE(MSG_FP_CONNECT_CHECK_CONNECTABLE_ACCOUNT_OK));

	util::del(pQPMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_GetUserInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		EP3 ���������ɼ� - �ٸ� ���� ���� ��û
/// \author		dhjin
/// \date		2008-06-24 ~ 2008-06-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_GetUserInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_GetUserInfo
	CFieldIOCPSocket *pFISock	= q.pFieldIOCPSocket;
	if (NULL == pFISock)
	{
		return;
	}

	UID32_t	TargetCharacterUID = (UID32_t)q.nGeneralParam1;
	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_GetUserInfo
	-- DESC				: ���� ���� ��������
	-- // 2008-06-20 by dhjin, EP3 ���������ɼ� -
	--						// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_GetUserInfo
		@i_CharacterUID		INT
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
	AS
		-- // 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - NickName �� �����´�.
		SELECT PilotFace, CharacterName, UnitKind, GuildName, GuildUniqueNumber, Level, MapIndex, ChannelIndex, Propensity, LastStartedTime, SecretInfoOption, NickName
		FROM dbo.td_character
		WHERE UniqueNumber = @i_CharacterUID
	GO
	************************************************************************/
	SQLINTEGER	arrCB[13] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS ,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS ,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &TargetCharacterUID, 0,	&arrCB[1]);
	RETCODE	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0182, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"dbo.atum_GetUserInfo Failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	arrCB[1] = SQL_NTS;
	SUSER_INFO_OPTION	UserInfoOption;
	util::zero(&UserInfoOption,sizeof(SUSER_INFO_OPTION));
	SQL_TIMESTAMP_STRUCT	LastStartedTime;		util::zero(&LastStartedTime, sizeof(LastStartedTime));
	SQLBindCol(hstmt, 1, SQL_C_UTINYINT, &UserInfoOption.PilotFace, 0,						&arrCB[1]);
	SQLBindCol(hstmt, 2, SQL_C_CHAR, UserInfoOption.CharacterName, SIZE_MAX_CHARACTER_NAME,	&arrCB[2]);
	SQLBindCol(hstmt, 3, SQL_C_USHORT, &UserInfoOption.UnitKind, 0,							&arrCB[3]);
	SQLBindCol(hstmt, 4, SQL_C_CHAR, UserInfoOption.GuildName, SIZE_MAX_GUILD_NAME,			&arrCB[4]);
	SQLBindCol(hstmt, 5, SQL_C_ULONG, &UserInfoOption.GuildUID, 0,							&arrCB[5]);
	SQLBindCol(hstmt, 6, SQL_C_UTINYINT, &UserInfoOption.Level, 0,							&arrCB[6]);
	SQLBindCol(hstmt, 7, SQL_C_USHORT, &UserInfoOption.MapChannelIndex.MapIndex, 0,			&arrCB[7]);
	SQLBindCol(hstmt, 8, SQL_C_USHORT, &UserInfoOption.MapChannelIndex.ChannelIndex, 0,		&arrCB[8]);
	SQLBindCol(hstmt, 9, SQL_C_SLONG, &UserInfoOption.Propensity, 0,						&arrCB[9]);
	SQLBindCol(hstmt, 10, SQL_C_TIMESTAMP, &LastStartedTime, 0,								&arrCB[10]);
	SQLBindCol(hstmt, 11, SQL_C_ULONG, &UserInfoOption.SecretInfoOption, 0,					&arrCB[11]);
	SQLBindCol(hstmt, 12, SQL_C_CHAR, UserInfoOption.NickName, SIZE_MAX_CHARACTER_NAME,		&arrCB[12]);	// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - NickName�� �����´�
	
	ret = SQLFetch(hstmt);
	UserInfoOption.LastStartedTime	= LastStartedTime;

	// 2012-10-17 by jhseol, ���� �̸����� ������ ON ��Ű��
#ifdef S_ITEM_VIEW_UNCONDITIONAL_ON
	UserInfoOption.SecretInfoOption = UserInfoOption.SecretInfoOption & USER_INFO_OPTION_ITEMINFO_DELETE_ALL_MASK;
#endif
	// end 2012-10-17 by jhseol, ���� �̸����� ������ ON ��Ű��

	SQLFreeStmt(hstmt, SQL_CLOSE);
	
	pFISock->SendUserInfoBySecretOption(&UserInfoOption);

}

// 2008-12-01 by cmkwon, �������� �����带 ����� ó���ϴ� �ý��� ���� - DQT_DailyJob �� ���� ��.
// ///////////////////////////////////////////////////////////////////////////////
// /// \fn			void CAtumFieldDBManager::QP_DailyJob(FIELD_DB_QUERY q, SQLHSTMT hstmt)
// /// \brief		MySQL���� ������ MySQL���� �������� �ʴ� MSSQL�� Job���� �۾��� ���⼭ ó���Ѵ�.
// /// \author		dhjin
// /// \date		2008-08-19 ~ 2008-08-19
// /// \warning	
// ///
// /// \param		
// /// \return		
// ///////////////////////////////////////////////////////////////////////////////
// void CAtumFieldDBManager::QP_DailyJob(FIELD_DB_QUERY q, SQLHSTMT hstmt)
// {// QT_DailyJob
// 	/************************************************************************
// 	--------------------------------------------------------------------------------
// 	-- TABLE NAME: dbo.atum_Job_DeleteCharacter
// 	-- DESC      : 2007-02-22 by dhjin,
// 	--------------------------------------------------------------------------------
// 	CREATE PROCEDURE dbo.atum_Job_DeleteCharacter
// 	AS
// 		-- // 2007-11-16 by cmkwon, ĳ���� ���� ���� �� ���� ������ �̾Ƽ� ó�� - dbo.atum_Job_DeleteCharacter ���ν��� ����
// 		DECLARE @KeepTermOfDay INT
// 		SET @KeepTermOfDay		= 14
// 
// 		-- 1. ĳ���� ���� �������� ��æƮ ����	
// 		DELETE td_Enchant FROM td_Enchant e, td_Store s 
// 		WHERE s.ItemStorage <> 2 AND s.AccountUniqueNumber IN 
// 		(SELECT AccountUniqueNumber FROM td_Character WITH (NOLOCK) WHERE 0 <> Race & 0x4000 AND (DATEDIFF(dd, LastStartedTime, GETDATE()) > @KeepTermOfDay)) AND s.Possess IN 
// 		(SELECT UniqueNumber FROM td_Character WITH (NOLOCK) WHERE 0 <> Race & 0x4000 AND (DATEDIFF(dd, LastStartedTime, GETDATE()) > @KeepTermOfDay)) AND s.UniqueNumber = e.TargetItemUniqueNumber
// 
// 		-- 2. ĳ���� ���� ������ ����
// 		DELETE FROM td_Store WHERE ItemStorage <> 2 AND AccountUniqueNumber IN 
// 		(SELECT AccountUniqueNumber FROM td_Character WITH (NOLOCK) WHERE 0 <> Race & 0x4000 AND (DATEDIFF(dd, LastStartedTime, GETDATE()) > @KeepTermOfDay)) AND possess IN 
// 		(SELECT UniqueNumber FROM td_Character WITH (NOLOCK) WHERE 0 <> Race & 0x4000 AND (DATEDIFF(dd, LastStartedTime, GETDATE()) > @KeepTermOfDay))
// 
// 		-- 3. ĳ���� �� ���� ����
// 		DELETE FROM td_QuickSlot WHERE AccountUID IN 
// 		(SELECT AccountUniqueNumber FROM td_Character WITH (NOLOCK) WHERE 0 <> Race & 0x4000 AND (DATEDIFF(dd, LastStartedTime, GETDATE()) > @KeepTermOfDay)) AND CharacterUID IN 
// 		(SELECT UniqueNumber FROM td_Character WITH (NOLOCK) WHERE 0 <> Race & 0x4000 AND (DATEDIFF(dd, LastStartedTime, GETDATE()) > @KeepTermOfDay))
// 
// 		-- 4. ĳ���� ���� ��ų ����
// 		DELETE FROM td_StoreSkill WHERE AccountUniqueNumber IN 
// 		(SELECT AccountUniqueNumber FROM td_Character WITH (NOLOCK) WHERE 0 <> Race & 0x4000 AND (DATEDIFF(dd, LastStartedTime, GETDATE()) > @KeepTermOfDay)) AND possess IN 
// 		(SELECT UniqueNumber FROM td_Character WITH (NOLOCK) WHERE 0 <> Race & 0x4000 AND (DATEDIFF(dd, LastStartedTime, GETDATE()) > @KeepTermOfDay))
// 
// 		-- 5. ����Ʈ ���̺��� ����
// 		DELETE FROM td_CharacterQuest WHERE CharacterUniqueNumber IN 
// 		(SELECT UniqueNumber FROM td_Character WITH (NOLOCK) WHERE 0 <> Race & 0x4000 AND (DATEDIFF(dd, LastStartedTime, GETDATE()) > @KeepTermOfDay))
// 
// 		-- 6. ����Ʈ ���� ���� ���̺��� ����
// 		DELETE FROM td_CharacterQuestMonsterCount WHERE CharacterUniqueNumber IN 
// 		(SELECT UniqueNumber FROM td_Character WITH (NOLOCK) WHERE 0 <> Race & 0x4000 AND (DATEDIFF(dd, LastStartedTime, GETDATE()) > @KeepTermOfDay))
// 
// 		-- 7. ģ������Ʈ���� ����
// 		DELETE FROM td_FriendList WHERE CharacterName IN 
// 		(SELECT CharacterName FROM td_Character WITH (NOLOCK) WHERE 0 <> Race & 0x4000 AND (DATEDIFF(dd, LastStartedTime, GETDATE()) > @KeepTermOfDay)) OR FriendName IN 
// 		(SELECT CharacterName FROM td_Character WITH (NOLOCK) WHERE 0 <> Race & 0x4000 AND (DATEDIFF(dd, LastStartedTime, GETDATE()) > @KeepTermOfDay))
// 
// 		--8. Ʃ�丮�� ����Ʈ���� ����
// 		DELETE FROM td_TutorialComplete WHERE CharacterUID IN 
// 		(SELECT UniqueNumber FROM td_Character WITH (NOLOCK) WHERE 0 <> Race & 0x4000 AND (DATEDIFF(dd, LastStartedTime, GETDATE()) > @KeepTermOfDay))
// 
// 		-- last ĳ���� ���̺��� ����
// 		DELETE FROM td_Character WHERE UniqueNumber IN 
// 		(SELECT UniqueNumber FROM td_Character WITH (NOLOCK) WHERE 0 <> Race & 0x4000 AND (DATEDIFF(dd, LastStartedTime, GETDATE()) > @KeepTermOfDay))
// 	GO
// 	************************************************************************/
// 	RETCODE	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0183, SQL_NTS);
// 	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
// 	{
// 		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"dbo.atum_Job_DeleteCharacter Failed!\r\n", TRUE, NULL, q);
// 	}
// 	else
// 	{
// 		g_pGlobal->WriteSystemLogEX(TRUE, "atum_Job_DeleteCharacter Successed! =====================\r\n");
// 	}
// 	SQLFreeStmt(hstmt, SQL_CLOSE);
// 	
// 	/************************************************************************
// 	--------------------------------------------------------------------------------
// 	-- PROCEDURE NAME	: dbo.atum_Guild_Update_TotalFameRankByJob
// 	-- DESC				: �� ���� �� ����
// 	-- JOB				: ���� ��ħ�ð����� JOB�� �Ǵ�.
// 	-- // 2008-06-05 by dhjin, EP3 - ���� ���� ����
// 	--------------------------------------------------------------------------------
// 	CREATE PROCEDURE dbo.atum_Guild_Update_TotalFameRankByJob
// 	AS
// 		UPDATE dbo.td_Guild SET GuildTotalFameRank = FG2.GuildTotalFameRank
// 		FROM dbo.td_Guild AS FG1,  
// 			(SELECT G1.GuildCommanderUniqueNumber, COUNT(G2.GuildCommanderUniqueNumber) + 1 AS GuildTotalFameRank
// 			FROM dbo.td_Guild AS G1 LEFT OUTER JOIN dbo.td_Guild AS G2
// 				ON G1.TotalFame < G2.TotalFame
// 			GROUP BY G1.GuildCommanderUniqueNumber) AS FG2
// 		WHERE FG1.GuildCommanderUniqueNumber = FG2.GuildCommanderUniqueNumber
// 	GO
// 	************************************************************************/
// 	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0184, SQL_NTS);
// 	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
// 	{
// 		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"dbo.atum_Guild_Update_TotalFameRankByJob Failed!\r\n", TRUE, NULL, q);
// 	}
// 	else
// 	{
// 		g_pGlobal->WriteSystemLogEX(TRUE, "atum_Guild_Update_TotalFameRankByJob Successed! =====================\r\n");
// 	}
// 	SQLFreeStmt(hstmt, SQL_CLOSE);
// 	
// 	/************************************************************************
// 	--------------------------------------------------------------------------------
// 	-- PROCEDURE NAME	: dbo.atum_Guild_Update_MonthlyFameRankByJob
// 	-- DESC				: ���� ���� �� ���� 
// 	-- JOB				: ���� ��ħ�ð����� JOB�� �Ǵ�.
// 	-- // 2008-06-05 by dhjin, EP3 - ���� ���� ����
// 	--------------------------------------------------------------------------------
// 	CREATE PROCEDURE dbo.atum_Guild_Update_MonthlyFameRankByJob
// 	AS
// 		UPDATE dbo.td_Guild SET GuildMonthlyFameRank = FG2.GuildMonthlyFameRank
// 		FROM dbo.td_Guild AS FG1,  
// 			(SELECT G1.GuildCommanderUniqueNumber, COUNT(G2.GuildCommanderUniqueNumber) + 1 AS GuildMonthlyFameRank
// 			FROM dbo.td_Guild AS G1 LEFT OUTER JOIN dbo.td_Guild AS G2
// 				ON G1.MonthlyFame < G2.MonthlyFame
// 			GROUP BY G1.GuildCommanderUniqueNumber) AS FG2
// 		WHERE FG1.GuildCommanderUniqueNumber = FG2.GuildCommanderUniqueNumber
// 	GO
// 	**********************************************************************/
// 	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0185, SQL_NTS);
// 	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
// 	{
// 		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"dbo.atum_Guild_Update_MonthlyFameRankByJob Failed!\r\n", TRUE, NULL, q);
// 	}
// 	else
// 	{
// 		g_pGlobal->WriteSystemLogEX(TRUE, "atum_Guild_Update_MonthlyFameRankByJob Successed! =====================\r\n");
// 	}
// 	SQLFreeStmt(hstmt, SQL_CLOSE);
// 
// }

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_UpdateLuckyItemDropCount(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		
/// \author		dhjin
/// \date		2008-11-04 ~ 2008-11-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_UpdateLuckyItemDropCount(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_UpdateLuckyItemDropCount
	QPARAM_UPDATE_LUCKYITEM_DROPCOUNT		*pQPMsg		= (QPARAM_UPDATE_LUCKYITEM_DROPCOUNT*)q.pQueryParam;

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Update_LuckyItem_DropCount
	-- DESC				: 
	-- 2008-11-04 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Update_LuckyItem_DropCount
		@i_MysteryItemDropNum	INT,
		@i_DropItemNum			INT,
		@i_ServerGroupID		INT,
		@i_DropCount			INT				
	AS
		DECLARE @CheckCount INT
		SET @CheckCount = (SELECT count(*) FROM atum2_db_account.dbo.td_MysteryItemDropCount WITH (NOLOCK) WHERE MysteryItemDropNum = @i_MysteryItemDropNum AND ServerGroupID = @i_ServerGroupID AND DropItemNum = @i_DropItemNum)
		IF (0 = @CheckCount)
		BEGIN
			INSERT INTO atum2_db_account.dbo.td_MysteryItemDropCount (MysteryItemDropNum, DropItemNum, ServerGroupID, DropCount) 
			VALUES (@i_MysteryItemDropNum, @i_DropItemNum, @i_ServerGroupID, @i_DropCount)
			RETURN;
		END

		UPDATE atum2_db_account.dbo.td_MysteryItemDropCount SET DropCount = DropCount + @i_DropCount 
		WHERE MysteryItemDropNum = @i_MysteryItemDropNum AND ServerGroupID = @i_ServerGroupID AND DropItemNum = @i_DropItemNum
	GO
	************************************************************************/
	//int ServerID = g_pGlobal->GetMGameServerID();
	SQLINTEGER	arrCB[5] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->MysteryItemDropNum, 0,	&arrCB[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->DropItemNum, 0,	&arrCB[2]);
	//SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &ServerID, 0,	&arrCB[3]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->DropCount, 0,	&arrCB[3]);
	RETCODE	ret = SQLExecDirect(hstmt, PROCEDURE_081104_0272, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Update_LuckyItem_DropCount Failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_UpdateLuckyItemStarttime(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		
/// \author		dhjin
/// \date		2008-11-04 ~ 2008-11-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_UpdateLuckyItemStarttime(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_UpdateLuckyItemStarttime
	QPARAM_UPDATE_LUCKYITEM_STARTTIME		*pQPMsg		= (QPARAM_UPDATE_LUCKYITEM_STARTTIME*)q.pQueryParam;

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Update_LuckyItem_STARTTIME
	-- DESC				:
	-- 2008-11-04 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Update_LuckyItem_STARTTIME
		@i_MysteryItemDropNum	INT,
		@i_StartTime			VARCHAR(30)				
	AS
		UPDATE atum2_db_account.dbo.ti_MysteryItemDrop SET starttime = @i_StartTime
		WHERE MysteryItemDropNum = @i_MysteryItemDropNum
	GO
	************************************************************************/
	SQLINTEGER	arrCB[3] = {SQL_NTS, SQL_NTS, SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->MysteryItemDropNum, 0,	&arrCB[1]);
	char tmpTimeString[SIZE_MAX_SQL_DATETIME_STRING];
	pQPMsg->StartTime.GetSQLDateTimeString(tmpTimeString, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpTimeString, 0,	&arrCB[2]);
	RETCODE	ret = SQLExecDirect(hstmt, PROCEDURE_081104_0273, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Update_LuckyItem_STARTTIME Failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Update_LuckyItem_DropCount_Reset
	-- DESC				: ��Ű�ӽ� ��������� �ʱ�ȭ
	-- 2008-11-04 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Update_LuckyItem_DropCount_Reset
		@i_MysteryItemDropNum	INT				
	AS
		UPDATE atum2_db_account.dbo.td_MysteryItemDropCount SET DropCount = 0
		WHERE MysteryItemDropNum = @i_MysteryItemDropNum
	GO
	**************************************************************************/
	SQLINTEGER	arrCBReset[2] = {SQL_NTS, SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->MysteryItemDropNum, 0,	&arrCBReset[1]);
	ret = SQLExecDirect(hstmt, PROCEDURE_081125_0275, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Update_LuckyItem_DropCount_Reset Failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_UpdateStartDeclarationOfWar(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		���� ���� - �������� ���� ���� �� ���� 
/// \author		dhjin
/// \date		2009-01-14 ~ 2009-01-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_UpdateStartDeclarationOfWar(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_UpdateStartDeclarationOfWar
	QPARAM_UPDATE_START_DECLARATION_OF_WAR	*pQPMsg	= (QPARAM_UPDATE_START_DECLARATION_OF_WAR*)q.pQueryParam;

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_UpdateStartDeclarationOfWar
	-- DESC				: ���� ���� ���� ���� ���� �� ����.
	-- 2009-01-14 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_UpdateStartDeclarationOfWar
		@i_Influence			TINYINT, 
		@i_MSWarStep			TINYINT, 
		@i_NCP					INT,     
		@i_MSNum				INT,     
		@i_MSAppearanceMap		INT
	AS
		UPDATE dbo.td_DeclarationOfWar SET NCP = @i_NCP, MSNum = @i_MSNum, MSAppearanceMap = @i_MSAppearanceMap
		WHERE Influence = @i_Influence AND MSWarStep = @i_MSWarStep
	GO
	************************************************************************/
	SQLINTEGER	arrCB[6] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQPMsg->Influence, 0,	&arrCB[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQPMsg->MSWarStep, 0,	&arrCB[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->NCP, 0,	&arrCB[3]);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->MSNum, 0,	&arrCB[4]);
	int nMapIndex = pQPMsg->MSAppearanceMap;
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nMapIndex, 0,	&arrCB[5]);
	RETCODE	ret = SQLExecDirect(hstmt, PROCEDURE_090114_0286, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateStartDeclarationOfWar Failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);	
}


void CAtumFieldDBManager::QP_UpdateEndDeclarationOfWar(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_UpdateEndDeclarationOfWar
	QPARAM_UPDATE_END_DECLARATION_OF_WAR	*pQPMsg	= (QPARAM_UPDATE_END_DECLARATION_OF_WAR*)q.pQueryParam;

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_UpdateEndDeclarationOfWar
	-- DESC				: ���� ���� ���� ���� ���� �� ����.
	-- 2009-01-14 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_UpdateEndDeclarationOfWar
		@i_Influence			TINYINT, 
		@i_MSWarStep			TINYINT, 
		@i_MSWarEndTime			VARCHAR(30),
		@i_MSWarEndState		TINYINT
	AS
		UPDATE dbo.td_DeclarationOfWar SET MSWarEndTime = @i_MSWarEndTime, MSWarEndState = @i_MSWarEndState
		WHERE Influence = @i_Influence AND MSWarStep = @i_MSWarStep
	GO
	************************************************************************/
	SQLINTEGER	arrCB[5] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQPMsg->Influence, 0,	&arrCB[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQPMsg->MSWarStep, 0,	&arrCB[2]);
	char tmpTimeString[SIZE_MAX_SQL_DATETIME_STRING];
	pQPMsg->MSWarEndTime.GetSQLDateTimeString(tmpTimeString, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpTimeString, 0,	&arrCB[3]);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQPMsg->MSWarEndState, 0,	&arrCB[4]);
	RETCODE	ret = SQLExecDirect(hstmt, PROCEDURE_090114_0287, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateEndDeclarationOfWar Failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_UpdateMSWarStartTime(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		���� ���� - ���� ���� ���� ���� �ð� ����
/// \author		dhjin
/// \date		2009-01-14 ~ 2009-01-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_UpdateMSWarStartTime(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_UpdateMSWarStartTime
	QPARAM_UPDATE_MSWAR_START_TIME	*pQPMsg	= (QPARAM_UPDATE_MSWAR_START_TIME*)q.pQueryParam;

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_UpdateMSWarStartTime
	-- DESC				: ���� ���� ���� ���� �ð� ����
	-- 2009-01-14 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_UpdateMSWarStartTime
		@i_Influence			TINYINT, 
		@i_MSWarStep			TINYINT,
		@i_MSWarStartTime		VARCHAR(30),
		@i_SelectCount			TINYINT,
		@i_GiveUp				BIT
	AS
		UPDATE dbo.td_DeclarationOfWar SET MSWarStartTime = @i_MSWarStartTime, SelectCount = @i_SelectCount, GiveUp = @i_GiveUp
		WHERE Influence = @i_Influence AND MSWarStep = @i_MSWarStep
	GO
	************************************************************************/
	SQLINTEGER	arrCB[6] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQPMsg->Influence, 0,	&arrCB[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQPMsg->MSWarStep, 0,	&arrCB[2]);
	char tmpTimeString[SIZE_MAX_SQL_DATETIME_STRING];
	pQPMsg->MSWarStartTime.GetSQLDateTimeString(tmpTimeString, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpTimeString, 0,	&arrCB[3]);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQPMsg->SelectCount, 0,	&arrCB[4]);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_BIT, SQL_BIT, 0, 0, &pQPMsg->GiveUp, 0,	&arrCB[5]);
	RETCODE	ret = SQLExecDirect(hstmt, PROCEDURE_090114_0288, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateMSWarStartTime Failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_UpdateNickName(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 
/// \author		cmkwon
/// \date		2009-02-19 ~ 2009-02-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_UpdateNickName(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_UpdateNickName

	MSG_FC_CHARACTER_CHANGE_NICKNAME *pQPMsg	= (MSG_FC_CHARACTER_CHANGE_NICKNAME*)q.pQueryParam;
	if(FALSE == q.pFieldIOCPSocket->IsValidCharacter(FALSE))
	{
		util::del(pQPMsg);
		return;
	}

	SQLINTEGER	arrCB[3] = {SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->CharacUID, 0,						&arrCB[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pQPMsg->NickName, 0,	&arrCB[2]);
	RETCODE	ret = SQLExecDirect(hstmt, PROCEDURE_090219_0311, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"PROCEDURE_090219_0311 Failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}

	int nErrorCode = 1;
	SQLBindCol(hstmt, 1, SQL_C_LONG, &nErrorCode, 0, NULL);
	
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(hstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 �̸� ����� RowCount �̴� 
			ret = SQLMoreResults(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(hstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(hstmt);                 // 2008-07-25 by cmkwon, ���� Results �� �ִ����� �ѹ��� üũ�Ѵ�.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}
	ret = SQLFreeStmt(hstmt, SQL_CLOSE);

	if(0 != nErrorCode)
	{
		q.pFieldIOCPSocket->SendErrorMessage(T_FC_CHARACTER_CHANGE_NICKNAME, ERR_COMMON_CHARACTER_ALREADY_EXISTS, 0, 0, pQPMsg->NickName);
	}
	else
	{
		INIT_MSG_WITH_BUFFER(MSG_FC_CHARACTER_CHANGE_NICKNAME_OK, T_FC_CHARACTER_CHANGE_NICKNAME_OK, pOK, SendBuf);
		pOK->CharacUID		= pQPMsg->CharacUID;
		util::strncpy(pOK->NickName, pQPMsg->NickName, SIZE_MAX_CHARACTER_NAME);
		q.pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_CHARACTER_CHANGE_NICKNAME_OK));
	}

	util::del(pQPMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 
/// \author		cmkwon
/// \date		2009-02-25 ~ 2009-02-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_GetSelfRanking(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_GetSelfRanking

	if(FALSE == q.pFieldIOCPSocket->IsValidCharacter(FALSE))
	{
		return;
	}

	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_WRK_GET_SELF_RANKING_OK, T_FC_INFO_WRK_GET_SELF_RANKING_OK, pSMsg, SendBuf);
	pSMsg->CharacUID		= q.nGeneralParam1;

	SQLINTEGER	cb = SQL_NTS;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pSMsg->CharacUID, 0,						&cb);
	RETCODE	ret = SQLExecDirect(hstmt, PROCEDURE_090219_0338, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"PROCEDURE_090219_0338 Failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		q.pFieldIOCPSocket->SendErrorMessage(T_FC_INFO_WRK_GET_SELF_RANKING, ERR_DB_EXECUTION_FAILED, 3);
		return;
	}
	cb = SQL_NTS;
	SQLBindCol(hstmt, 1, SQL_C_LONG, &pSMsg->LevelRanking, 0, &cb);
	ret = SQLFetch(hstmt);
	ret = SQLFreeStmt(hstmt, SQL_CLOSE);

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pSMsg->CharacUID, 0,						&cb);
	ret = SQLExecDirect(hstmt, PROCEDURE_090219_0339, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"PROCEDURE_090219_0339 Failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		q.pFieldIOCPSocket->SendErrorMessage(T_FC_INFO_WRK_GET_SELF_RANKING, ERR_DB_EXECUTION_FAILED, 3);
		return;
	}
	cb = SQL_NTS;
	SQLBindCol(hstmt, 1, SQL_C_LONG, &pSMsg->FameRanking, 0, &cb);
	ret = SQLFetch(hstmt);
	ret = SQLFreeStmt(hstmt, SQL_CLOSE);

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pSMsg->CharacUID, 0,						&cb);
	ret = SQLExecDirect(hstmt, PROCEDURE_090219_0340, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"PROCEDURE_090219_0340 Failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		q.pFieldIOCPSocket->SendErrorMessage(T_FC_INFO_WRK_GET_SELF_RANKING, ERR_DB_EXECUTION_FAILED, 3);
		return;
	}
	cb = SQL_NTS;
	SQLBindCol(hstmt, 1, SQL_C_LONG, &pSMsg->PvPRanking, 0, &cb);
	ret = SQLFetch(hstmt);
	ret = SQLFreeStmt(hstmt, SQL_CLOSE);

	q.pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_INFO_WRK_GET_SELF_RANKING_OK));
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-03-31 by cmkwon, �����ʱ�ȭ �ý��� ���� - 
/// \author		cmkwon
/// \date		2009-04-01 ~ 2009-04-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_ChangeItemWithItemMatching(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	CFieldIOCPSocket *pFieldIOCPSocket = q.pFieldIOCPSocket;
	UID32_t uidCharUID		= q.nGeneralParam1;
	BYTE	byInflTy		= q.nGeneralParam2;
	
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: atum_Load_QuestMaching
	-- DESC				: // 2009-03-31 by cmkwon, �����ʱ�ȭ �ý��� ���� - �ش� ĳ������ â�� �����۸� ó���Ѵ�.
	--						
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_ChangeItemWithItemMatching
		@i_CharcUID					INT,
		@i_InflTy					TINYINT
	AS
		IF (@i_InflTy = 2)
		BEGIN
			-- change Influence Type from ANI to BCU
			UPDATE dbo.td_Store
			SET ItemNum = im.ItemNumForBCU
			FROM dbo.td_Store s INNER JOIN atum2_db_account.dbo.ti_ItemMatching im ON s.ItemNum = im.ItemNumForANI
			WHERE s.Possess = @i_CharcUID AND s.ItemStorage = 1
		END
		ELSE IF (@i_InflTy = 4)
		BEGIN
			-- change Influence Type from BCU to ANI
			UPDATE dbo.td_Store
			SET ItemNum = im.ItemNumForANI
			FROM dbo.td_Store s INNER JOIN atum2_db_account.dbo.ti_ItemMatching im ON s.ItemNum = im.ItemNumForBCU
			WHERE s.Possess = @i_CharcUID AND s.ItemStorage = 1		
		END
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &uidCharUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &byInflTy, 0, NULL);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_090401_0346, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_NO_DATA)	// 2007-08-01 by cmkwon, �߰���(&& ret!=SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"PROCEDURE_090401_0346 Failed!\r\n", TRUE, pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-10-12 by cmkwon, ������ī ���� ��� ���� - CAtumFieldDBManager::QP_ChangeStartCityMapIndex# �߰�
/// \author		cmkwon
/// \date		2009-10-13 ~ 2009-10-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_ChangeStartCityMapIndex(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_ChangeStartCityMapIndex

	CFieldIOCPSocket *pFieldIOCPSocket = q.pFieldIOCPSocket;
	UID32_t		uidCharUID			= q.nGeneralParam1;
	MapIndex_t	nStartCityMapIdx	= q.nGeneralParam2;
	
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_change_StartCityMapIndex
	-- DESC				: // 2009-10-12 by cmkwon, ������ī ���� ��� ���� - �ش� ĳ������ StartCityMapIndex�� �����Ѵ�(Material �ʵ�)
	--						
	--------------------------------------------------------------------------------
	call dbo.atum_change_StartCityMapIndex
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &uidCharUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &nStartCityMapIdx, 0, NULL);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_091013_0406, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_NO_DATA)	// 2007-08-01 by cmkwon, �߰���(&& ret!=SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"PROCEDURE_091013_0406 Failed! \r\n", TRUE, pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);

		pFieldIOCPSocket->SendErrorMessage(T_FC_CHARACTER_CHANGE_START_CITY_MAPINDEX, ERR_DB_EXECUTION_FAILED, uidCharUID);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	if(pFieldIOCPSocket
		&& pFieldIOCPSocket->IsUsing())
	{
		INIT_MSG_WITH_BUFFER(MSG_FC_CHARACTER_CHANGE_START_CITY_MAPINDEX_OK, T_FC_CHARACTER_CHANGE_START_CITY_MAPINDEX_OK, pSMsg, SendBuf);
		pSMsg->CharacUID		= uidCharUID;
		pSMsg->nStartCityMapIdx	= nStartCityMapIdx;
		pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_CHARACTER_CHANGE_START_CITY_MAPINDEX_OK));
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-11-02 by cmkwon, ĳ��(�κ�/â�� Ȯ��) ������ �߰� ���� - 
/// \author		cmkwon
/// \date		2009-11-03 ~ 2009-11-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_ChangeAddedInventoryCount(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_ChangeAddedInventoryCount

	CFieldIOCPSocket *pFieldIOCPSocket = q.pFieldIOCPSocket;
	UID32_t		uidCharUID			= q.nGeneralParam1;
	int			nRacingPoint		= q.nGeneralParam2;
	
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_ChangeRacingPoint
	-- DESC				: // 2009-11-02 by cmkwon, ĳ��(�κ�/â�� Ȯ��) ������ �߰� ���� - 
	--						
	--------------------------------------------------------------------------------
	call dbo.atum_ChangeRacingPoint
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &uidCharUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nRacingPoint, 0, NULL);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0061, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_ChangeRacingPoint Failed! \r\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}	
	SQLFreeStmt(hstmt, SQL_CLOSE);

	if(pFieldIOCPSocket
		&& pFieldIOCPSocket->IsValidCharacter(FALSE))
	{
		INIT_MSG_WITH_BUFFER(MSG_FC_CHARACTER_CHANGE_ADDED_INVENTORY_COUNT, T_FC_CHARACTER_CHANGE_ADDED_INVENTORY_COUNT, pSMsg, SendBuf);
		pSMsg->CharacUID		= uidCharUID;
		pSMsg->nRacingPoint		= nRacingPoint;
		pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_CHARACTER_CHANGE_ADDED_INVENTORY_COUNT));
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_LoadInfinityImpute(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		���Ǵ�Ƽ - ���� �ͼ� ���� ��������
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_LoadInfinityImpute(FIELD_DB_QUERY q, SQLHSTMT hstmt) {
// QT_LoadInfinityImpute
	CFieldIOCPSocket *pFISock	= q.pFieldIOCPSocket;
	if (NULL == pFISock
		|| FALSE == pFISock->IsValidCharacter(FALSE)) {
		return;
	}

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Load_InfinityImpute
	-- DESC				: -- // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ͼ� ���� ��������
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Load_InfinityImpute
		@i_AccountUID			INT,
		@i_CharacterUID			INT
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
	AS
		SELECT InfinityModeUID, EntranceCount
		FROM dbo.td_InfinityImpute WITH (NOLOCK)
		WHERE AccountUID = @i_AccountUID AND CharacterUID = @i_CharacterUID
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pFISock->m_character.AccountUniqueNumber, 0,NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pFISock->m_character.CharacterUniqueNumber, 0,NULL);
	RETCODE	ret = SQLExecDirect(hstmt, PROCEDURE_090909_0504, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Load_InfinityImpute Failed!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	
	SQLINTEGER	cb[3] = {SQL_NTS,SQL_NTS,SQL_NTS};
	INFINITY_IMPUTE		tmInfinityImpute;
	vectINFINITY_IMPUTE	tmImputeList;		// 2010-04-05 by cmkwon, ���� ������ ī�� ���� �ý��� ���� - 
	util::zero(&tmInfinityImpute, sizeof(INFINITY_IMPUTE));

	SQLBindCol(hstmt, 1, SQL_C_ULONG, &tmInfinityImpute.InfinityModeUID, 0,			&cb[1]);
	SQLBindCol(hstmt, 2, SQL_C_UTINYINT, &tmInfinityImpute.EntranceCount, 0,		&cb[2]);
	
	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA) {
		// 2010-04-05 by cmkwon, ���� ������ ī�� ���� �ý��� ���� - 
		//pFISock->m_mtVecInfiImpute.pushBackLock(tmInfinityImpute);
		tmImputeList.push_back(tmInfinityImpute);	// 2010-04-05 by cmkwon, ���� ������ ī�� ���� �ý��� ���� - 

		util::zero(&tmInfinityImpute, sizeof(INFINITY_IMPUTE));
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	if(pFISock->IsValidCharacter(FALSE))
	{
		pFISock->InfinityInitImputeList(&tmImputeList);	// 2010-04-05 by cmkwon, ���� ������ ī�� ���� �ý��� ���� - 
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_InsertInfinityImpute(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		���Ǵ�Ƽ - ���� �ͼ� ���� �߰�
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_InsertInfinityImpute(FIELD_DB_QUERY q, SQLHSTMT hstmt) {
// QT_InsertInfinityImpute
	QPARAM_INSERT_INFINITY	*pQPMsg	= (QPARAM_INSERT_INFINITY*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Insert_InfinityImpute
	-- DESC				: -- // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���Ǵ�Ƽ�� �ͼӵ�
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Insert_InfinityImpute
		@i_AccountUID			INT,
		@i_CharacterUID			INT,
		@i_InfinityModeUID		INT,
		@i_StartTime			VARCHAR(30)
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
	AS
		INSERT INTO dbo.td_InfinityImpute(AccountUID, CharacterUID, InfinityModeUID, EntranceCount)
		VALUES (@i_AccountUID, @i_CharacterUID, @i_InfinityModeUID, 1)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->AccountUID, 0,NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->CharacterUID, 0,NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->InfinityModeUID, 0,NULL);
	char tmpTimeString[SIZE_MAX_SQL_DATETIME_STRING];
	pQPMsg->StartTime.GetSQLDateTimeString(tmpTimeString, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpTimeString, 0, NULL);
	RETCODE	ret = SQLExecDirect(hstmt, PROCEDURE_090909_0505, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Insert_InfinityImpute Failed!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
		return;
	}	
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);			// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_UpdateInfinityImpute(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		���Ǵ�Ƽ - ���� �Ϸ� 
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_UpdateInfinityImpute(FIELD_DB_QUERY q, SQLHSTMT hstmt) {
// QT_UpdateInfinityImpute
	QPARAM_UPDATE_INFINITY	*pQPMsg	= (QPARAM_UPDATE_INFINITY*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Update_InfinityImpute
	-- DESC				: -- // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���Ǵ�Ƽ �ͼ� ���� ����
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Update_InfinityImpute
		@i_AccountUID			INT,
		@i_CharacterUID			INT,
		@i_InfinityModeUID		INT
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
	AS
		UPDATE dbo.td_InfinityImpute
		SET EntranceCount = EntranceCount + 1
		WHERE AccountUID = @i_AccountUID AND CharacterUID = @i_CharacterUID AND InfinityModeUID = @i_InfinityModeUID
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->AccountUID, 0,NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->CharacterUID, 0,NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQPMsg->InfinityModeUID, 0,NULL);
	RETCODE	ret = SQLExecDirect(hstmt, PROCEDURE_090909_0506, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Done_InfinityImpute Failed!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
		return;
	}	
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQPMsg);		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_ResetInfinityImpute(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		���Ǵ�Ƽ - �ش� ���� �������� ���� ����
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_ResetInfinityImpute(FIELD_DB_QUERY q, SQLHSTMT hstmt) {
// QT_ResetInfinityImpute
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Reset_InfinityImpute
	-- DESC				: -- // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���Ǵ�Ƽ �ͼ� ���� ����
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Reset_InfinityImpute
		@i_InfinityModeUID		INT
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
	AS
		DELETE FROM dbo.td_InfinityImpute
		WHERE InfinityModeUID = @i_InfinityModeUID
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &q.nGeneralParam1, 0,NULL);
	RETCODE	ret = SQLExecDirect(hstmt, PROCEDURE_090909_0507, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Reset_InfinityImpute Failed!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_ArenaCopyInfinityDBInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		���Ǵ�Ƽ - �Ʒ���DB�� �����ϱ�
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_ArenaCopyInfinityDBInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_ArenaCopyInfinityDBInfo
	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.arena_CopyDBInfo_Infinity
	-- DESC				: -- // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �Ʒ���DB�� �����ϱ�
	--------------------------------------------------------------------------------
	************************************************************************/
	QPARAM_ARENA_COPY_DB_INFO 	*pQParam	= (QPARAM_ARENA_COPY_DB_INFO*)q.pQueryParam;
	CFieldIOCPSocket *pFISock				= q.pFieldIOCPSocket;
	if (NULL == pFISock 
		|| FALSE == pFISock->IsUsing() 
		|| ST_CLIENT_TYPE != pFISock->m_PeerSocketType
		|| CS_ARENASERVER_PLAYING != pFISock->GetClientState()		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - CS_ARENA_PLAYING => CS_ARENASERVER_PLAYING ����
		|| pFISock->GetCharacter()->AccountUniqueNumber != pQParam->AccountUID)
	{
		util::del(pQParam);
		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
 	INT	ErrCheck = 0;
 	SQLINTEGER	arrCB[5] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1,SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQParam->ArenaCharacterUID, 0, &arrCB[1]);
	SQLBindParameter(hstmt, 2,SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQParam->CharacterUID, 0, &arrCB[2]);
	SQLBindParameter(hstmt, 3,SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQParam->AccountUID, 0, &arrCB[3]);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_090909_0509, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::arena_CopyDBInfo_Infinity Fail!! ArenaCharcterUID(%d) CharacterUID(%d) AccUID(%d) \r\n", pQParam->ArenaCharacterUID, pQParam->CharacterUID, pQParam->AccountUID);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		INIT_MSG_WITH_BUFFER(MSG_FtoA_AUTH_CHARACTER_OK, T_FtoA_AUTH_CHARACTER_OK, pSendMsg, SendBuf);
		pSendMsg->ErrorCheck			= TRUE;
		pSendMsg->AccountUID			= pFISock->m_character.AccountUniqueNumber;
		pSendMsg->AFSClientIndex		= pQParam->AFSClientIndex;
		pSendMsg->ErrorMsgType			= T_FtoA_AUTH_CHARACTER;
		pSendMsg->ErrorCode				= ERR_DB_EXECUTION_FAILED;
		m_pFieldIOCP8->m_pArenaFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FtoA_AUTH_CHARACTER_OK));
		util::del(pQParam);
		return;
	}
	SQLBindCol(hstmt, 1, SQL_C_LONG, &(ErrCheck), 0, NULL);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(hstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 �̸� ����� RowCount �̴� 
			ret = SQLMoreResults(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(hstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(hstmt);                 // 2008-07-25 by cmkwon, ���� Results �� �ִ����� �ѹ��� üũ�Ѵ�.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}
	if(FALSE != ErrCheck)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::arena_CopyDBInfo_Infinity Fail!! ErrCount = %d, ArenaCharacterUID = %d, CharacterUID = %d, AccountUID = %d\r\n", ErrCheck, pQParam->ArenaCharacterUID, pQParam->CharacterUID, pQParam->AccountUID);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		INIT_MSG_WITH_BUFFER(MSG_FtoA_AUTH_CHARACTER_OK, T_FtoA_AUTH_CHARACTER_OK, pSendMsg, SendBuf);
		pSendMsg->ErrorCheck			= TRUE;
		pSendMsg->AccountUID			= pFISock->m_character.AccountUniqueNumber;
		pSendMsg->AFSClientIndex		= pQParam->AFSClientIndex;
		pSendMsg->ErrorMsgType			= T_FtoA_AUTH_CHARACTER;
		pSendMsg->ErrorCode				= ERR_PROTOCOL_INVALID_CHARACTER_UNIQUENUMBER;
		m_pFieldIOCP8->m_pArenaFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FtoA_AUTH_CHARACTER_OK));
		util::del(pQParam);
		return;
	}
	// 2008-02-29 by dhjin, �׽�Ʈ ��
//	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENACHECK] arena_CopyDBInfo~E ArenaCUID = %d, CUID = %d, AUID = %d\r\n", pQParam->ArenaCharacterUID, pQParam->CharacterUID, pQParam->AccountUID);
	SQLFreeStmt(hstmt, SQL_CLOSE);

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 
	INIT_MSG_WITH_BUFFER(MSG_FtoA_INFINITY_START_OK, T_FtoA_INFINITY_START_OK, pSendMsg, SendBuf);
	pSendMsg->AFSClientIndex		= pQParam->AFSClientIndex;
	pSendMsg->bIsPCBangClient = false; // pFISock->m_bIsPCBangClient;
	m_pFieldIOCP8->m_pArenaFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FtoA_INFINITY_START_OK));
	
	util::del(pQParam);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_CharacterSaveDataInfinityFin(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		���Ǵ�Ƽ - ���� ���� �� MainSvr�� ���� �����ϸ鼭 ������ �ɸ��� ���� ����
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_CharacterSaveDataInfinityFin(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_CharacterSaveDataInfinityFin
	QPARAM_CHARACTER_SAVE_DATA_INFINITY_FIN		*pQParam = (QPARAM_CHARACTER_SAVE_DATA_INFINITY_FIN*)q.pQueryParam;
	CFieldIOCPSocket	*pFieldIOCPSocket = q.pFieldIOCPSocket;
	if(NULL == pFieldIOCPSocket) {
		util::del(pQParam);
		return;
	}
	
	RETCODE ret;

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_CharacterSaveDataInfinityFin
	-- DESC				: -- // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� �� MainSvr�� ���� �����ϸ鼭 ������ �ɸ��� ���� ����
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_CharacterSaveDataInfinityFin
		@i_CharacterUID		INT,
		@i_Level			TINYINT,
		@i_Experience		FLOAT,
		@i_DownExperience	FLOAT,
		@i_DownSPIOnDeath	INT,			-- 2006-04-10 by cmkwon
		@i_totalPlayTime	BIGINT,
		@i_PCBangtotalPlayTime	BIGINT,						-- 2007-06-07 by dhjin, PC�� �� �÷��� �ð�
		@i_SecretInfoOption		INT,				-- // 2008-06-23 by dhjin, EP3 ���������ɼ�
		@i_AttackPart		SMALLINT,
		@i_DefensePart		SMALLINT,
		@i_FuelPart			SMALLINT,
		@i_SoulPart			SMALLINT,
		@i_ShieldPart		SMALLINT,
		@i_DodgePart		SMALLINT,
		@i_bstat			TINYINT,
		@i_bstatPoint		TINYINT,
		@i_LevelUpTime		VARCHAR(30),				// 2006-12-18 by dhjin, ������ �ð�
		@i_WarPoint			INT,						// 2007-04-17 by dhjin, WP
		@i_CumulativeWarPoint	INT						// 2007-05-28 by dhjin, ����WP
		-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
		AS
		UPDATE td_Character
		SET	Level			= @i_Level,
			Experience		= @i_Experience,
			DownExperience  = @i_DownExperience,
			DownSPIOnDeath	= @i_DownSPIOnDeath,			-- 2006-04-10 by cmkwon
			totalPlayTime	= @i_totalPlayTime,
			PCBangtotalPlayTime = @i_PCBangtotalPlayTime,						-- 2007-06-07 by dhjin, PC�� �� �÷��� �ð�
			SecretInfoOption	= @i_SecretInfoOption,				-- // 2008-06-23 by dhjin, EP3 ���������ɼ�
			AttackPart		= @i_AttackPart,
			DefensePart		= @i_DefensePart,
			FuelPart		= @i_FuelPart,
			SoulPart		= @i_SoulPart,
			ShieldPart		= @i_ShieldPart,
			DodgePart		= @i_DodgePart,
			BonusStat		= @i_bstat,
			BonusStatPoint	= @i_bstatPoint,
			LevelUpTime		= @i_LevelUpTime,				// 2006-12-18 by dhjin, ������ �ð�
			WarPoint		= @i_WarPoint,						// 2007-04-17 by dhjin, WP
			CumulativeWarPoint	= @i_CumulativeWarPoint						// 2007-05-28 by dhjin, ����WP
			WHERE UniqueNumber = @i_CharacterUID
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pFieldIOCPSocket->m_character.CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQParam->Level, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_FLOAT, 0, 0, &pQParam->Experience, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT,	SQL_C_DOUBLE, SQL_FLOAT, 0, 0, &pQParam->DownExperience, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pQParam->DownSPIOnDeath, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &pQParam->TotalPlayTime, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &pQParam->PCBangTotalPlayTime, 0, NULL);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pQParam->SecretInfoOption, 0, NULL);
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pQParam->GearStat.AttackPart, 0, NULL);
	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pQParam->GearStat.DefensePart, 0, NULL);
	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pQParam->GearStat.FuelPart, 0, NULL);
	SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pQParam->GearStat.SoulPart, 0, NULL);
	SQLBindParameter(hstmt, 13, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pQParam->GearStat.ShieldPart, 0, NULL);
	SQLBindParameter(hstmt, 14, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pQParam->GearStat.DodgePart, 0, NULL);
	SQLBindParameter(hstmt, 15, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQParam->BonusStat, 0, NULL);
	SQLBindParameter(hstmt, 16, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQParam->BonusStatPoint, 0, NULL);
	char tmpTimeString[SIZE_MAX_SQL_DATETIME_STRING];
	pQParam->LevelUpTime.GetSQLDateTimeString(tmpTimeString, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 17, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpTimeString, 0, NULL);
	SQLBindParameter(hstmt, 18, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pQParam->WarPoint, 0, NULL);
	SQLBindParameter(hstmt, 19, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pQParam->CumulativeWarPoint, 0, NULL);
	SQLBindParameter(hstmt, 20, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pQParam->InfinityMapIndex, 0, NULL);	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��
	SQLBindParameter(hstmt, 21, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pQParam->InfinityChannelIndex, 0, NULL);	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��

	ret = SQLExecDirect(hstmt,PROCEDURE_090909_0512, SQL_NTS);
	
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_CharacterSaveDataInfinityFin Failed 3 !\r\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQParam);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);	
	util::del(pQParam);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_CharacterSaveDataInfinityFinByDisconnect(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		���Ǵ�Ƽ - ���� ���� ���� �� MainSvr�� ���� �����ϸ鼭 ������ �ɸ��� ���� ����
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_CharacterSaveDataInfinityFinByDisconnect(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_CharacterSaveDataInfinityFinByDisconnect
	QPARAM_CHARACTER_SAVE_DATA_INFINITY_FIN		*pQParam = (QPARAM_CHARACTER_SAVE_DATA_INFINITY_FIN*)q.pQueryParam;
	RETCODE ret;

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_CharacterSaveDataInfinityFin
	-- DESC				: -- // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� �� MainSvr�� ���� �����ϸ鼭 ������ �ɸ��� ���� ����
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_CharacterSaveDataInfinityFin
		@i_CharacterUID		INT,
		@i_Level			TINYINT,
		@i_Experience		FLOAT,
		@i_DownExperience	FLOAT,
		@i_DownSPIOnDeath	INT,			-- 2006-04-10 by cmkwon
		@i_totalPlayTime	BIGINT,
		@i_PCBangtotalPlayTime	BIGINT,						-- 2007-06-07 by dhjin, PC�� �� �÷��� �ð�
		@i_SecretInfoOption		INT,				-- // 2008-06-23 by dhjin, EP3 ���������ɼ�
		@i_AttackPart		SMALLINT,
		@i_DefensePart		SMALLINT,
		@i_FuelPart			SMALLINT,
		@i_SoulPart			SMALLINT,
		@i_ShieldPart		SMALLINT,
		@i_DodgePart		SMALLINT,
		@i_bstat			TINYINT,
		@i_bstatPoint		TINYINT,
		@i_LevelUpTime		VARCHAR(30),				--// 2006-12-18 by dhjin, ������ �ð�
		@i_WarPoint			INT,					--	// 2007-04-17 by dhjin, WP
		@i_CumulativeWarPoint	INT,				--	// 2007-05-28 by dhjin, ����WP
		@i_MapIndex			SMALLINT,							--	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��
		@i_ChannelIndex		SMALLINT							--	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��
		-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
		AS
			UPDATE td_Character
			SET	Level			= @i_Level,
			Experience		= @i_Experience,
			DownExperience  = @i_DownExperience,
			DownSPIOnDeath	= @i_DownSPIOnDeath,			-- 2006-04-10 by cmkwon
			totalPlayTime	= @i_totalPlayTime,
			PCBangtotalPlayTime = @i_PCBangtotalPlayTime,						-- 2007-06-07 by dhjin, PC�� �� �÷��� �ð�
			SecretInfoOption	= @i_SecretInfoOption,				-- // 2008-06-23 by dhjin, EP3 ���������ɼ�
			AttackPart		= @i_AttackPart,
			DefensePart		= @i_DefensePart,
			FuelPart		= @i_FuelPart,
			SoulPart		= @i_SoulPart,
			ShieldPart		= @i_ShieldPart,
			DodgePart		= @i_DodgePart,
			BonusStat		= @i_bstat,
			BonusStatPoint	= @i_bstatPoint,
			LevelUpTime		= @i_LevelUpTime,			--	// 2006-12-18 by dhjin, ������ �ð�
			WarPoint		= @i_WarPoint,					--	// 2007-04-17 by dhjin, WP
			CumulativeWarPoint	= @i_CumulativeWarPoint,					--	// 2007-05-28 by dhjin, ����WP
			MapIndex		= @i_MapIndex,			--	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��
			ChannelIndex	= @i_ChannelIndex		--	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��
			WHERE UniqueNumber = @i_CharacterUID
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQParam->CharacterUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQParam->Level, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_FLOAT, 0, 0, &pQParam->Experience, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT,	SQL_C_DOUBLE, SQL_FLOAT, 0, 0, &pQParam->DownExperience, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pQParam->DownSPIOnDeath, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &pQParam->TotalPlayTime, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &pQParam->PCBangTotalPlayTime, 0, NULL);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pQParam->SecretInfoOption, 0, NULL);
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pQParam->GearStat.AttackPart, 0, NULL);
	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pQParam->GearStat.DefensePart, 0, NULL);
	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pQParam->GearStat.FuelPart, 0, NULL);
	SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pQParam->GearStat.SoulPart, 0, NULL);
	SQLBindParameter(hstmt, 13, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pQParam->GearStat.ShieldPart, 0, NULL);
	SQLBindParameter(hstmt, 14, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pQParam->GearStat.DodgePart, 0, NULL);
	SQLBindParameter(hstmt, 15, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQParam->BonusStat, 0, NULL);
	SQLBindParameter(hstmt, 16, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQParam->BonusStatPoint, 0, NULL);
	char tmpTimeString[SIZE_MAX_SQL_DATETIME_STRING];
	pQParam->LevelUpTime.GetSQLDateTimeString(tmpTimeString, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 17, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpTimeString, 0, NULL);
	SQLBindParameter(hstmt, 18, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pQParam->WarPoint, 0, NULL);
	SQLBindParameter(hstmt, 19, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pQParam->CumulativeWarPoint, 0, NULL);
	SQLBindParameter(hstmt, 20, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pQParam->InfinityMapIndex, 0, NULL);	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��
	SQLBindParameter(hstmt, 21, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pQParam->InfinityChannelIndex, 0, NULL);	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��
	
	ret = SQLExecDirect(hstmt,PROCEDURE_090909_0512, SQL_NTS);
	
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_CharacterSaveDataInfinityFin Failed 3 !\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQParam);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);	
	util::del(pQParam);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_InfinityFinUpdateItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		���Ǵ�Ƽ - ���� ���� �� MainSvr�� ���� �����ϸ鼭 ������ ������ ���� ����
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_InfinityFinUpdateItem(FIELD_DB_QUERY q, SQLHSTMT hstmt) {
	// QT_InfinityFinUpdateItem
	QPARAM_INFINITY_FIN_UPDATE_ITEM		*pItem = (QPARAM_INFINITY_FIN_UPDATE_ITEM*)q.pQueryParam;
	CFieldIOCPSocket	*pFieldIOCPSocket = q.pFieldIOCPSocket;
	if(NULL == pFieldIOCPSocket) {
		util::del(pItem);
		return;
	}

	RETCODE ret;

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_InfinityFinUpdateItem
	-- DESC				: -- // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� �� MainSvr�� ���� �����ϸ鼭 ������ ���� ����
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_InfinityFinUpdateItem
		@i_MainSvrItemUID	BIGINT,
		@i_wear				TINYINT,
		@i_CurrentCount		INT,
		@i_ItemWindowIndex	INT,
		@i_UsingTimeStamp	INT,
		@i_CoolingTime		INT					--	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
	AS
		UPDATE dbo.td_Store
		SET Wear = @i_wear, CurrentCount = @i_CurrentCount, ItemWindowIndex = @i_ItemWindowIndex, UsingTimeStamp = @i_UsingTimeStamp, CoolingTime = @i_CoolingTime
		WHERE UniqueNumber = @i_MainSvrItemUID
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &pItem->MainSvrItemUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pItem->Wear, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->CurrentCount, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->ItemWindowIndex, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->UsingTimeStamp, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->CoolingTimeStamp, 0, NULL);		// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������

	ret = SQLExecDirect(hstmt,PROCEDURE_090909_0511, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_InfinityFinUpdateItem Failed 3 !\r\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pItem);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);	
	util::del(pItem);

	if(NULL != pFieldIOCPSocket){
		pFieldIOCPSocket->InfinityFinReloadAllItem();
	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_InfinityFinUpdateItemByDisconnect(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		���Ǵ�Ƽ - ���� ���� ���� �� MainSvr�� ���� �����ϸ鼭 ������ ������ ���� ����
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_InfinityFinUpdateItemByDisconnect(FIELD_DB_QUERY q, SQLHSTMT hstmt) {
	// QT_InfinityFinUpdateItemByDisconnect
	QPARAM_INFINITY_FIN_UPDATE_ITEM		*pItem = (QPARAM_INFINITY_FIN_UPDATE_ITEM*)q.pQueryParam;
	RETCODE ret;

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_InfinityFinUpdateItem
	-- DESC				: -- // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� �� MainSvr�� ���� �����ϸ鼭 ������ ���� ����
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_InfinityFinUpdateItem
		@i_MainSvrItemUID	BIGINT,
		@i_wear				TINYINT,
		@i_CurrentCount		INT,
		@i_ItemWindowIndex	INT,
		@i_UsingTimeStamp	INT,
		@i_CoolingTime		INT					--	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
	AS
		UPDATE dbo.td_Store
		SET Wear = @i_wear, CurrentCount = @i_CurrentCount, ItemWindowIndex = @i_ItemWindowIndex, UsingTimeStamp = @i_UsingTimeStamp, CoolingTime = @i_CoolingTime
		WHERE UniqueNumber = @i_MainSvrItemUID
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &pItem->MainSvrItemUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pItem->Wear, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->CurrentCount, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->ItemWindowIndex, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->UsingTimeStamp, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->CoolingTimeStamp, 0, NULL);		// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������

	ret = SQLExecDirect(hstmt,PROCEDURE_090909_0511, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_InfinityFinUpdateItem Failed 3 !\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pItem);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);	
	util::del(pItem);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_InfinityFinInsertItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		���Ǵ�Ƽ - ���� ���� �� MainSvr�� ���� �����ϸ鼭 ������ ������ �߰�
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_InfinityFinInsertItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_InfinityFinInsertItem
	ITEM_GENERAL		*pItem = (ITEM_GENERAL*)q.pQueryParam;
	CFieldIOCPSocket	*pFieldIOCPSocket = q.pFieldIOCPSocket;
	if(NULL == pFieldIOCPSocket) {
		util::del(pItem);
		return;
	}

	RETCODE ret;

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_InfinityFinInsertItem
	-- DESC				: -- // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� �� MainSvr�� ���� �����ϸ鼭 ������ ������ �߰�
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_InfinityFinInsertItem
		@accNum				INT,
		@possess			INT,
		@ItemStorage		TINYINT,			-- 2005-12-07 by cmkwon
		@wear				TINYINT,
		@CurrentCount		INT,
		@ItemWindowIndex	INT,
		@ItemNum			INT,
		@NumOfEnchants		SMALLINT,
		@PrefixCodeNum		INT,
		@SufficCodeNum		INT,
		@CurrentEndurance	FLOAT,
		@ColorCode			INT,				-- 2005-11-21 by cmkwon, �Ƹ� Ʃ�׽� Į��
		@UsingTimeStamp		INT,
		@i_ShapeItemNum		INT,				-- // 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - td_Store.ShapeItemNum �ʵ� �߰�, �ʵ帮��Ʈ ���
		@i_GetDate			VARCHAR(30),
		@i_CoolingTime		INT					--	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
	AS
		INSERT INTO dbo.td_Store(AccountUniqueNumber,Possess,ItemStorage,Wear,CurrentCount,ItemWindowIndex,ItemNum,NumOfEnchants,PrefixCodeNum,SuffixCodeNum,CurrentEndurance,ColorCode,UsingTimeStamp,CreatedTime,ShapeItemNum,CoolingTime)
		VALUES (@accNum, @possess, @ItemStorage, @wear, @CurrentCount, @ItemWindowIndex,
				@ItemNum, @NumOfEnchants, @PrefixCodeNum, @SufficCodeNum, @CurrentEndurance, @ColorCode, @UsingTimeStamp, @i_GetDate, @i_ShapeItemNum, @i_CoolingTime)
		IF (@@ERROR <> 0)
		BEGIN
			SELECT 0;
			RETURN;
		END

	  SELECT SCOPE_IDENTITY();
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItem->AccountUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItem->Possess, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pItem->ItemStorage, 0, NULL);		// 2005-12-07 by cmkwon
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pItem->Wear, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->CurrentCount, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->ItemWindowIndex, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->ItemNum, 0, NULL);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &pItem->NumOfEnchants, 0, NULL);
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->PrefixCodeNum, 0, NULL);
	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->SuffixCodeNum, 0, NULL);
	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pItem->CurrentEndurance, 0, NULL);
	SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItem->ColorCode, 0, NULL);
	SQLBindParameter(hstmt, 13, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItem->UsingTimeStamp, 0, NULL);
	SQLBindParameter(hstmt, 14, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pItem->ShapeItemNum, 0, NULL);		// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - ShapeItemNum �ʵ� �߰�
	char tmpTimeString[SIZE_MAX_SQL_DATETIME_STRING];
	pItem->CreatedTime.GetSQLDateTimeString(tmpTimeString, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 15, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpTimeString, 0,	NULL);
	SQLBindParameter(hstmt, 16, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItem->CoolingTimeStamp, 0, NULL);	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������

	ret = SQLExecDirect(hstmt,PROCEDURE_090909_0510, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_InfinityFinInsertItem Failed 3 !\r\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pItem);

		if(pFieldIOCPSocket
			&& pFieldIOCPSocket->IsValidCharacter(FALSE))
		{
			pFieldIOCPSocket->DecrementInsertingStoreCounts();
			pFieldIOCPSocket->SendErrorMessage(T_FC_STORE_MOVE_ITEM, ERR_DB_EXECUTION_FAILED, IUT_DROP_ITEM);
		}
		return;
	}
	SQLBindCol(hstmt, 1, SQL_C_UBIGINT, &(pItem->UniqueNumber), 0, NULL);	

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(hstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 �̸� ����� RowCount �̴� 
			ret = SQLMoreResults(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(hstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(hstmt);                 // 2008-07-25 by cmkwon, ���� Results �� �ִ����� �ѹ��� üũ�Ѵ�.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);	

	if(FALSE == IS_VALID_UID64(pItem->UniqueNumber))
	{// 2008-07-24 by cmkwon, �߰��� �������� ItemUID üũ
		if(pFieldIOCPSocket->IsValidCharacter(FALSE))
		{
			pFieldIOCPSocket->SendErrorMessage(T_FC_STORE_MOVE_ITEM, ERR_DB_EXECUTION_FAILED, IUT_DROP_ITEM);			
		}
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] atum_InsertStoreItem@ error 3 !! %s %s, ItemInsertionType(%d)\r\n"
			, GetCharacterString(pFieldIOCPSocket->GetCharacter(), string()), GetItemGeneralString(pItem, string()), IUT_DROP_ITEM);
		util::del(pItem);
		return;
	}
	
	CAtumLogSender::SendLogMessageITEMGetITEM(pFieldIOCPSocket, pItem, pItem->CurrentCount, IUT_DROP_ITEM);
	
	///////////////////////////////////////////////////////////////////////////////	
	// �˻�
	if (NULL == pFieldIOCPSocket)
	{
		util::del(pItem);
		return;
	}
	
	BOOL bSendInsertMsg = TRUE;			// 2008-02-18 by cmkwon, ī���ͺ� �������� ������ ����� �ν��Ͻ� �������Ǵ� ���� ���� - 

	mt_auto_lock igLock(&pFieldIOCPSocket->m_ItemManager.m_mapItemGeneral);	// lock m_mapItemGeneral, @QP_InsertStoreItem

	///////////////////////////////////////////////////////////////////////////////
	// 
	pFieldIOCPSocket->DecrementInsertingStoreCounts();
	
	if(WEAR_ATTACHED == pItem->Wear) {
		BOOL bWearValid = pFieldIOCPSocket->CheckItemWindowPosition(pItem->ItemWindowIndex, pItem->ItemInfo); 
		if (bWearValid) {
// 2009-09-09 ~ 2010-02-23 by dhjin, ���Ǵ�Ƽ - ������ ���� ����ȭ ����.
//			bWearValid = pFieldIOCPSocket->AttachItem(pItem->ItemWindowIndex, pItem, TRUE);
			bWearValid = pFieldIOCPSocket->AttachItem(pItem->ItemWindowIndex, pItem, FALSE);
		}
		else {
			char szSystemLog[1024];
			sprintf(szSystemLog, "[Error] %s(%d, %d) Process_FtoA_INFINITY_UPDATE_ITEM Invalid Position: %s(%d) ItemPos(%d), ItemWindowIndex(%d)\r\n",
				pFieldIOCPSocket->m_character.CharacterName, pFieldIOCPSocket->m_character.CharacterUniqueNumber, pFieldIOCPSocket->m_character.ClientIndex,
				pItem->ItemInfo->ItemName, pItem->ItemInfo->ItemNum, pItem->ItemInfo->Position, pItem->ItemWindowIndex);
			g_pFieldGlobal->WriteSystemLog(szSystemLog);
			DbgOut(szSystemLog);
			// ���� ������ ��ȿ���� �����Ƿ� �ʱ�ȭ
			pItem->Wear				= WEAR_NOT_ATTACHED;
			pItem->ItemWindowIndex		= POS_INVALID_POSITION;
		}
	}
	
	// 2010-03-10 by cmkwon, ���ǿ��� �ð����� �׼����� �߰� ���� ���� ���� - 
	if(FALSE == IS_ATTACHABLE_ITEM(pItem->ItemInfo)
		&& COMPARE_BIT_FLAG(pItem->ItemInfo->ItemAttribute, ITEM_ATTR_TIME_LIMITE|ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED)		// 2008-11-26 by cmkwon, ��� �� �ð�(����ð�) ���� ������ ���� - 
		&& ITEM_NOT_USING != pItem->UsingTimeStamp)
	{// ĳ���Ͱ� ������� �����ۿ� �߰�
		
		///////////////////////////////////////////////////////////////////////////////
		// 1. ��� ���� ���� �ð��� �ٽ� �ѹ��� �ʱ�ȭ �Ѵ�
		pItem->UsingStartTime.SetCurrentDateTime();
		
		///////////////////////////////////////////////////////////////////////////////
		// 2008-01-31 by cmkwon, �ð����� ������ DB �� �߰� �� üũ - DB �� �߰� �� ���¿��� ItemNum �� �����Ѵ�.
		pFieldIOCPSocket->m_ItemManager.DeleteInsertingItemNumList2DB(pItem->ItemNum);
		
		///////////////////////////////////////////////////////////////////////////////
		// 2. m_vectUsingGeneralItem�� �����ϱ�
		// 2010-03-17 by cmkwon, ����1�� �ߵ��� ������ ���� ���� ���� - ��â�� ���� ������ �Ķ� ���� ó��
		pFieldIOCPSocket->m_ItemManager.Add2UsingTimeLimitedItemList(pItem, TRUE);


#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, �������� �����߰�
		if ( NULL !=  pFieldIOCPSocket->m_pFieldParty )		// ��밡 ������ 
		{
			pFieldIOCPSocket->m_pFieldParty->SetReturnUserBuffSkill(pFieldIOCPSocket); // ���Ǵ�Ƽ ������ ����  
		}
#endif

		if(FALSE == (pItem->ItemInfo->Kind == ITEMKIND_CARD && COMPARE_BIT_FLAG(pItem->ItemInfo->ItemAttribute, ITEM_ATTR_TIME_LIMITE|ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED)) && pItem->ItemInfo->Kind != ITEMKIND_PET_SOCKET_ITEM )		// 2011-09-29 by hskim, ��Ʈ�� �ý��� 2�� - ���� �������� ���� ���� �ʵ��� ���ǹ� �߰� 
		{
			// ��� ��� �����Ǵ� �������� �ƴϴ�. ex)���ڽ� ī�� -> ����, ������ ���� -> �κ� �߰�
			pItem->Possess		= pFieldIOCPSocket->m_character.CharacterUniqueNumber;		// 2006-09-20 by dhjin, �ɸ��Ͱ� â�� ���� -> ���õ� �ɸ��� UID
			pFieldIOCPSocket->m_ItemManager.m_mapItemGeneral.insertLock(pItem->UniqueNumber, pItem);
			// 2013-06-25 by jhseol, ������ UID '0' ������ ����� �α�
#ifdef S_ITEMUID_ZERO_DEBUG_LOG_JHSEOL
			if (IS_COUNTABLE_ITEM(pItem->Kind))
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DEBUG] SPI ItemUID Zero : CAtumFieldDBManager::QP_InfinityFinInsertItem_1 - CUID(%d), ItemNum(%d), Count(%d), ItemUID(%d)\r\n"
					, pItem->Possess, pItem->ItemNum, pItem->CurrentCount, pItem->UniqueNumber);
			}
#endif
			// end 2013-06-25 by jhseol, ������ UID '0' ������ ����� �α�
			pFieldIOCPSocket->m_ItemManager.SetDelayStoredItemCounts(pItem);
			pFieldIOCPSocket->InfinityFinReloadAllItem();
		}
		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// ĳ���� �κ��� �߰�
	pItem->Possess		= pFieldIOCPSocket->m_character.CharacterUniqueNumber;		// 2006-09-20 by dhjin, �ɸ��Ͱ� â�� ���� -> ���õ� �ɸ��� UID
	pFieldIOCPSocket->m_ItemManager.m_mapItemGeneral.insertLock(pItem->UniqueNumber, pItem);
	// 2013-06-25 by jhseol, ������ UID '0' ������ ����� �α�
#ifdef S_ITEMUID_ZERO_DEBUG_LOG_JHSEOL
	if (IS_COUNTABLE_ITEM(pItem->Kind))
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DEBUG] SPI ItemUID Zero : CAtumFieldDBManager::QP_InfinityFinInsertItem_2 - CUID(%d), ItemNum(%d), Count(%d), ItemUID(%d)\r\n"
			, pItem->Possess, pItem->ItemNum, pItem->CurrentCount, pItem->UniqueNumber);
	}
#endif
	// end 2013-06-25 by jhseol, ������ UID '0' ������ ����� �α�
	pFieldIOCPSocket->m_ItemManager.SetDelayStoredItemCounts(pItem);
	pFieldIOCPSocket->InfinityFinReloadAllItem();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_InfinityFinInsertItemByDisconnect(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		���Ǵ�Ƽ - ���� ���� �� MainSvr�� ���� �����ϸ鼭 ������ ������ �߰�
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_InfinityFinInsertItemByDisconnect(FIELD_DB_QUERY q, SQLHSTMT hstmt) {
	// QT_InfinityFinInsertItemByDisconnect
	ITEM_GENERAL		*pItem = (ITEM_GENERAL*)q.pQueryParam;
	RETCODE ret;

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_InfinityFinInsertItem
	-- DESC				: -- // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� �� MainSvr�� ���� �����ϸ鼭 ������ ������ �߰�
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_InfinityFinInsertItem
		@accNum				INT,
		@possess			INT,
		@ItemStorage		TINYINT,			-- 2005-12-07 by cmkwon
		@wear				TINYINT,
		@CurrentCount		INT,
		@ItemWindowIndex	INT,
		@ItemNum			INT,
		@NumOfEnchants		SMALLINT,
		@PrefixCodeNum		INT,
		@SufficCodeNum		INT,
		@CurrentEndurance	FLOAT,
		@ColorCode			INT,				-- 2005-11-21 by cmkwon, �Ƹ� Ʃ�׽� Į��
		@UsingTimeStamp		INT,
		@i_ShapeItemNum		INT,				-- // 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - td_Store.ShapeItemNum �ʵ� �߰�, �ʵ帮��Ʈ ���
		@i_GetDate			VARCHAR(30),
		@i_CoolingTime		INT					--	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
	AS
		INSERT INTO dbo.td_Store(AccountUniqueNumber,Possess,ItemStorage,Wear,CurrentCount,ItemWindowIndex,ItemNum,NumOfEnchants,PrefixCodeNum,SuffixCodeNum,CurrentEndurance,ColorCode,UsingTimeStamp,CreatedTime,ShapeItemNum,CoolingTime)
		VALUES (@accNum, @possess, @ItemStorage, @wear, @CurrentCount, @ItemWindowIndex,
				@ItemNum, @NumOfEnchants, @PrefixCodeNum, @SufficCodeNum, @CurrentEndurance, @ColorCode, @UsingTimeStamp, @i_GetDate, @i_ShapeItemNum, @i_CoolingTime)
		IF (@@ERROR <> 0)
		BEGIN
			SELECT 0;
			RETURN;
		END

	  SELECT SCOPE_IDENTITY();
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItem->AccountUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItem->Possess, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pItem->ItemStorage, 0, NULL);		// 2005-12-07 by cmkwon
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pItem->Wear, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->CurrentCount, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->ItemWindowIndex, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->ItemNum, 0, NULL);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &pItem->NumOfEnchants, 0, NULL);
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->PrefixCodeNum, 0, NULL);
	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pItem->SuffixCodeNum, 0, NULL);
	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pItem->CurrentEndurance, 0, NULL);
	SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItem->ColorCode, 0, NULL);
	SQLBindParameter(hstmt, 13, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItem->UsingTimeStamp, 0, NULL);
	SQLBindParameter(hstmt, 14, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pItem->ShapeItemNum, 0, NULL);		// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - ShapeItemNum �ʵ� �߰�
	char tmpTimeString[SIZE_MAX_SQL_DATETIME_STRING];
	pItem->CreatedTime.GetSQLDateTimeString(tmpTimeString, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 15, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpTimeString, 0,	NULL);
	SQLBindParameter(hstmt, 16, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItem->CoolingTimeStamp, 0, NULL);		// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������

	ret = SQLExecDirect(hstmt,PROCEDURE_090909_0510, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_InfinityFinInsertItem Failed 3 !\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pItem);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pItem);
	return;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_InfinityInsertLog(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		���Ǵ�Ƽ - ���� �α�
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_InfinityInsertLog(FIELD_DB_QUERY q, SQLHSTMT hstmt) {
	// QT_InfinityInsertLog
	QPARAM_INFINITY_INSERT_LOG		*pQParam = (QPARAM_INFINITY_INSERT_LOG*)q.pQueryParam;
	CFieldIOCPSocket	*pFieldIOCPSocket = q.pFieldIOCPSocket;
	if(NULL == pFieldIOCPSocket) {
		util::del(pQParam);
		return;
	}

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Insert_Log_Infinity
	-- DESC				: -- // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���Ǵ�Ƽ ���� ��� �α׷� �����
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Insert_Log_Infinity
		@i_AccountUID			INT,
		@i_CharacterUID			INT,
		@i_InfinityModeUID		INT,
		@i_StartTime			VARCHAR(30),
		@i_EndTime				VARCHAR(30),
		@i_DeathCount			INT,
		@i_KillCount			INT
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
	AS
		INSERT INTO dbo.atum_Log_Infinity(AccountUID, CharacterUID, InfinityModeUID, StartTime, EndTime, DeathCount, KillCount)
		VALUES (@i_AccountUID, @i_CharacterUID, @i_InfinityModeUID, @i_StartTime, @i_EndTime, @i_DeathCount, @i_KillCount)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pFieldIOCPSocket->m_character.AccountUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pFieldIOCPSocket->m_character.CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQParam->InfinityModeUID, 0, NULL);
	char StartTimeString[SIZE_MAX_SQL_DATETIME_STRING];
	pQParam->StartTime.GetSQLDateTimeString(StartTimeString, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, StartTimeString, 0, NULL);
	char EndTimeString[SIZE_MAX_SQL_DATETIME_STRING];
	pQParam->EndTime.GetSQLDateTimeString(EndTimeString, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, EndTimeString, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQParam->DeathCount, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQParam->KillCount, 0, NULL);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_FLOAT,  0, 0, &pQParam->AllEXP, 0, NULL); // 2010-06-25 by shcho, ���Ǵ�Ƽ ���÷α� ��� - ����� �ִ� ����ġ �߰�
	
	RETCODE ret = SQLExecDirect(hstmt,PROCEDURE_090909_0513, SQL_NTS);
	
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Insert_Log_Infinity Failed 3 !\r\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQParam);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);	
	util::del(pQParam);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_ResetInfinityImputeByServerStart(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		���Ǵ�Ƽ - �ش� ���� �������� ���� ����
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_ResetInfinityImputeByServerStart(FIELD_DB_QUERY q, SQLHSTMT hstmt) {
// QT_ResetInfinityImputeByServerStart
	QPARAM_INFINITY_RESET_IMPUTE_SERVERSTART		*pQParam = (QPARAM_INFINITY_RESET_IMPUTE_SERVERSTART*)q.pQueryParam;

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Reset_InfinityImputeByServerStart
	-- DESC				: -- // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���Ǵ�Ƽ �ͼ� ���� ����
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Reset_InfinityImputeByServerStart
		@i_InfinityModeUID		INT,
		@i_StartTime			VARCHAR(30)
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
	AS
		DELETE FROM dbo.td_InfinityImpute
		WHERE InfinityModeUID = @i_InfinityModeUID AND StartTime <= @i_StartTime
	GO
	**********************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQParam->InfinityModeUID, 0,NULL);
	char TimeString[SIZE_MAX_SQL_DATETIME_STRING];
	pQParam->StartTime.GetSQLDateTimeString(TimeString, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, TimeString, 0, NULL);
	RETCODE	ret = SQLExecDirect(hstmt, PROCEDURE_090909_0524, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Reset_InfinityImputeByServerStart Failed!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQParam);		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);	
	util::del(pQParam);
}

// 2010-04-09 by cmkwon, ����2�� �߰� ����(�ܰ躰 ���� �߰�) - 
// ///////////////////////////////////////////////////////////////////////////////
// /// \fn			void CAtumFieldDBManager::QP_LoadTenderInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt)
// /// \brief		���Ǵ�Ƽ - Tender
// /// \author		dhjin
// /// \date		2009-09-09 ~ 2010
// /// \warning	
// ///
// /// \param		
// /// \return		
// ///////////////////////////////////////////////////////////////////////////////
// void CAtumFieldDBManager::QP_LoadTenderInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt)
// {
// // QT_LoadTenderInfo
// 	/*[Stored Query Definition]************************************************
// 	--------------------------------------------------------------------------------
// 	-- PROCEDURE NAME	: dbo.atum_Load_Tender
// 	-- DESC				: -- // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - Tender �ε� 
// 	--------------------------------------------------------------------------------
// 	CREATE PROCEDURE dbo.atum_Load_Tender
// 	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
// 	AS
// 		SELECT TenderUID, TenderItemNum 
// 		FROM atum2_db_account.dbo.ti_Tender WITH (NOLOCK);
// 	GO
// 	**********************************************************************/
// 	RETCODE	ret = SQLExecDirect(hstmt, PROCEDURE_090909_0527, SQL_NTS);
// 	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret) {
// 		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Load_Tender Failed!\n", TRUE, NULL, q);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		return;
// 	}
// 	
// 	SQLINTEGER	cb[3] = {SQL_NTS,SQL_NTS,SQL_NTS};
// 	TENDER_INFO		tmTenderInfo;
// 	util::zero(&tmTenderInfo, sizeof(TENDER_INFO));
// 
// 	SQLBindCol(hstmt, 1, SQL_C_ULONG, &tmTenderInfo.TenderUID, 0,			&cb[1]);
// 	SQLBindCol(hstmt, 2, SQL_C_ULONG, &tmTenderInfo.TenderItemNum, 0,		&cb[2]);
// 	
// 	this->m_pFieldIOCP8->m_TenderInfoList.clear();
// 	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA) {
// 		this->m_pFieldIOCP8->m_TenderInfoList.push_back(tmTenderInfo);
// 		util::zero(&tmTenderInfo, sizeof(TENDER_INFO));
// 	}
// 	SQLFreeStmt(hstmt, SQL_CLOSE);
// }

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_CashLoadPremiumCard(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		���Ǵ�Ƽ - �����̾� ���� ��������
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_CashLoadPremiumCard(FIELD_DB_QUERY q, SQLHSTMT hstmt) {
	// QT_CashLoadPremiumCard
	CFieldIOCPSocket	*pFieldIOCPSocket = q.pFieldIOCPSocket;
	if(NULL == pFieldIOCPSocket) {
		return;
	}
	
	SCASH_PREMEIUM_CARD_INFO	tmCardInfo;
	tmCardInfo.ResetCASH_PREMEIUM_CARD_INFO();

	SQLINTEGER	cb[7] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS ,SQL_NTS,SQL_NTS};
	///////////////////////////////////////////////////////////////////////////////
	// ����ȭ �����̾� ī�� �ε�
	/********************************************************************************
	-- 2005-01-20 by cmkwon, ������ ���� �������� �����´�
	CREATE PROCEDURE atum_GetAccountCashStoreItem
	@i_accNum		INT					-- AccountUniqueNumber
	AS
	SELECT * FROM td_accountCashStore
	WHERE AccountUniqueNumber = @i_accNum
	GO
	********************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pFieldIOCPSocket->m_character.AccountUniqueNumber, 0, NULL);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0049, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_GetAccountCashStoreItem Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	
	
	SQL_TIMESTAMP_STRUCT		arrSqlTime[3];
	util::zero(arrSqlTime, sizeof(arrSqlTime[0])*3);
	
	SQLBindCol(hstmt, 1, SQL_C_SBIGINT, &tmCardInfo.n64UniqueNumber10, 0, &cb[0]);
	SQLBindCol(hstmt, 2, SQL_C_LONG, &tmCardInfo.nAccountUID10, 0, &cb[1]);
	SQLBindCol(hstmt, 3, SQL_C_LONG, &tmCardInfo.nCardItemNum, 0, &cb[2]);
	SQLBindCol(hstmt, 4, SQL_C_TIMESTAMP, &arrSqlTime[0], 0, &cb[3]);
	SQLBindCol(hstmt, 5, SQL_C_TIMESTAMP, &arrSqlTime[1], 0, &cb[4]);
	SQLBindCol(hstmt, 6, SQL_C_TIMESTAMP, &arrSqlTime[2], 0, &cb[5]);
	ret = SQLFetch(hstmt);
	SQLFreeStmt(hstmt, SQL_CLOSE);
	tmCardInfo.atumTimeCreatedTime	= arrSqlTime[0];
	tmCardInfo.atumTimeUpdatedTime	= arrSqlTime[1];
	tmCardInfo.atumTimeExpireTime	= arrSqlTime[2];
	
	if(tmCardInfo.n64UniqueNumber10)
	{// ����ȭ ī�尡 �����Ѵ� - �Ⱓ ���� üũ
		
		ATUM_DATE_TIME	tmCurATime;
		tmCurATime.SetCurrentDateTime();
		
		if(tmCardInfo.atumTimeExpireTime <= tmCurATime)
		{// ����� ������ - DB���� ī�� �����ؾ���
			
			QPARAM_CASH_DELETE_PREMIUM_CARD *pQParam = new QPARAM_CASH_DELETE_PREMIUM_CARD;
			util::zero(pQParam, sizeof(QPARAM_CASH_DELETE_PREMIUM_CARD));
			pQParam->nAccountUID			= tmCardInfo.nAccountUID10;
			this->MakeAndEnqueueQuery(QT_CashDeletePremiumCard, q.pFieldIOCPSocket, tmCardInfo.nAccountUID10, pQParam);
			
			///////////////////////////////////////////////////////////////////////////////
			// �ʱ�ȭ
			util::zero(&tmCardInfo, sizeof(tmCardInfo));
			return;
		}
	}
	// end_����ȭ �����̾� ī�� �ε�
	///////////////////////////////////////////////////////////////////////////////
	
	if(0 == tmCardInfo.nAccountUID10) {
		pFieldIOCPSocket->m_ItemManager.m_cashPremiumCardInfo.ResetCASH_PREMEIUM_CARD_INFO();
	}
	else {
		pFieldIOCPSocket->m_ItemManager.CashInitPremiumCardInfo(&tmCardInfo);
	}	
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_LoadInfinityShopInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		���Ǵ�Ƽ - ���� ����
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_LoadInfinityShopInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt) {
// QT_LoadInfinityShopInfo
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Load_InfinityShop
	-- DESC				: -- // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� �ε�
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Load_InfinityShop
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
	AS
		SELECT 	InfinityShopUID, BuyItemNum, BuyItemCount, TradeItemNum1, TradeItemCount1
		, TradeItemNum2, TradeItemCount2, TradeItemNum3, TradeItemCount3, TradeItemNum4, TradeItemCount4, TradeItemNum5, TradeItemCount5 
		FROM atum2_db_account.dbo.ti_InfinityShop WITH (NOLOCK)
		ORDER BY InfinityShopUID
	GO
	**********************************************************************/
	RETCODE	ret = SQLExecDirect(hstmt, PROCEDURE_090909_0530, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Load_InfinityShop Failed!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	
	SQLINTEGER	cb[14] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	INFINITY_SHOP_INFO		tmInfinityShopInfo;
	util::zero(&tmInfinityShopInfo, sizeof(INFINITY_SHOP_INFO));

	SQLBindCol(hstmt, 1, SQL_C_ULONG, &tmInfinityShopInfo.InfinityShopUID, 0,	&cb[1]);
	SQLBindCol(hstmt, 2, SQL_C_ULONG, &tmInfinityShopInfo.BuyItemNum, 0,		&cb[2]);
	SQLBindCol(hstmt, 3, SQL_C_ULONG, &tmInfinityShopInfo.BuyItemCount, 0,		&cb[3]);
	SQLBindCol(hstmt, 4, SQL_C_ULONG, &tmInfinityShopInfo.TradeItemNum1, 0,		&cb[4]);
	SQLBindCol(hstmt, 5, SQL_C_ULONG, &tmInfinityShopInfo.TradeItemCount1, 0,	&cb[5]);
	SQLBindCol(hstmt, 6, SQL_C_ULONG, &tmInfinityShopInfo.TradeItemNum2, 0,		&cb[6]);
	SQLBindCol(hstmt, 7, SQL_C_ULONG, &tmInfinityShopInfo.TradeItemCount2, 0,	&cb[7]);
	SQLBindCol(hstmt, 8, SQL_C_ULONG, &tmInfinityShopInfo.TradeItemNum3, 0,		&cb[8]);
	SQLBindCol(hstmt, 9, SQL_C_ULONG, &tmInfinityShopInfo.TradeItemCount3, 0,	&cb[9]);
	SQLBindCol(hstmt, 10, SQL_C_ULONG, &tmInfinityShopInfo.TradeItemNum4, 0,	&cb[10]);
	SQLBindCol(hstmt, 11, SQL_C_ULONG, &tmInfinityShopInfo.TradeItemCount4, 0,	&cb[11]);
	SQLBindCol(hstmt, 12, SQL_C_ULONG, &tmInfinityShopInfo.TradeItemNum5, 0,	&cb[12]);
	SQLBindCol(hstmt, 13, SQL_C_ULONG, &tmInfinityShopInfo.TradeItemCount5, 0,	&cb[13]);

	this->m_pFieldIOCP8->m_InfinityShopInfoList.clear();
	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA) {
		this->m_pFieldIOCP8->m_InfinityShopInfoList.push_back(tmInfinityShopInfo);
		util::zero(&tmInfinityShopInfo, sizeof(INFINITY_SHOP_INFO));
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
}

void CAtumFieldDBManager::QP_UpdateItemCoolingTimeStamp(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{	// QT_UpdateItemCoolingTimeStamp
	QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP *pItemCoolingTimeStamp = (QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP*)q.pQueryParam;

	RETCODE ret;
	SDWORD pcbNTS = SQL_NTS;

	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name:
	-- Desc:
	--====
	CREATE PROCEDURE atum_UpdateItemCoolingTimeStamp
		@UniqueNumber		BIGINT,
		@CoolingTime		INT
	AS
		UPDATE td_Store
		SET CoolingTime = @CoolingTime
		WHERE UniqueNumber = @UniqueNumber
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pItemCoolingTimeStamp->ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pItemCoolingTimeStamp->CoolingTimeStamp, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_100210_0556, SQL_NTS);

	if(ret == SQL_NO_DATA)	{
		///////////////////////////////////////////////////////////////////////////
		// �������� �������� ����
		q.pFieldIOCPSocket->SendErrorMessageFromFieldDB(QT_UpdateItemCoolingTimeStamp, ERR_DB_NO_SUCH_STORE_ITEM);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pItemCoolingTimeStamp);
		return;
	}

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO ) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_UpdateItemCoolingTimeStamp Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pItemCoolingTimeStamp);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	
	util::del(pItemCoolingTimeStamp);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-06 by cmkwon, ����2�� �߰� ���� - 
/// \author		cmkwon
/// \date		2010-04-06 ~ 2010-04-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_InfinityUpdateUserMapInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_InfinityUpdateUserMapInfo
	QPARAM_INFINITY_UPDATE_USER_MAP_INFO *pQParam = (QPARAM_INFINITY_UPDATE_USER_MAP_INFO*)q.pQueryParam;
	
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_InfinityUpdateUserMapInfo
	-- DESC				: -- // 2010-04-06 by cmkwon, ����2�� �߰� ���� - 
	--------------------------------------------------------------------------------
	CALL dbo.atum_InfinityUpdateUserMapInfo
	**************************************************************************/
	SQLINTEGER	arrCB[4] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQParam->CharacterUID, 0,			&arrCB[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pQParam->InfinityMapIndex, 0,		&arrCB[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pQParam->InfinityChannelIndex, 0, &arrCB[3]);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_100406_0560, SQL_NTS);

//	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity] CAtumFieldDBManager::QP_InfinityUpdateUserMapInfo# ! CharacterUID(%d) Map(%d:%d) ret(%d) \r\n"
//		, pQParam->CharacterUID, pQParam->InfinityMapIndex, pQParam->InfinityChannelIndex, ret);

	if(ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"PROCEDURE_100406_0560 Failed! \r\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQParam);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);	
	util::del(pQParam);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		2010-05-13 by shcho, ���Ǵ�Ƽ ���̵� ���� -
/// \author		shcho
/// \date		2010-05-13 ~ 
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumFieldDBManager::DBLoadInfinityDifficultInfo(CODBCStatement *i_pODBCStmt, vectorInfinity_DifficultyInfo_Bonus* o_vectorDifficultyInfo) 
{
	/************************************************************************
	CREATE PROCEDURE dbo.atum_Load_InfinityDifficult
	-- // 2010-05-06 by shcho					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
	AS
		SELECT IncreaseStep, IncreaseIncomeExp, IncreaseIncomeDropProbabillity, IncreaseIncomeDropCount
		FROM atum2_db_account.dbo.ti_LevelAdjustmentReward;
	GO
	************************************************************************/
	if(FALSE == i_pODBCStmt->ExecuteQuery(PROCEDURE_100506_0561))
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DB ERROR] CAtumFieldDBManager::DBLoadInfinityDifficultInfo call PROCEDURE_100506_0561 \r\n");
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}
	
	INFINITY_DIFFICULTY_BONUS_INFO	tmListInfo;
	util::zero(&tmListInfo, sizeof(INFINITY_DIFFICULTY_BONUS_INFO));

	SQLINTEGER	arrCB[4] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &tmListInfo.iIncreaseStep,			0, &arrCB[0]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_LONG, &tmListInfo.iExp,					0, &arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_LONG, &tmListInfo.iDrop_Item_Persent,		0, &arrCB[2]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_LONG, &tmListInfo.iItem_EA_persent,		0, &arrCB[3]);

	RETCODE	ret;
	while ( (ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA) 
	{
		o_vectorDifficultyInfo->push_back(tmListInfo);
		util::zero(&tmListInfo, sizeof(INFINITY_DIFFICULTY_BONUS_INFO));
	}
	i_pODBCStmt->FreeStatement();

	return TRUE;	
}

//2010-05-13 by shcho, ���Ǵ�Ƽ ���̵� ���� - ���� �������� ��������
BOOL CAtumFieldDBManager::DBLoadInfinityDifficultMonsterInfo(CODBCStatement *i_pODBCStmt, vectorInfinity_DifficultyInfo_Monster* o_vectorDifficultyMonsterInfo)
{
	if(FALSE == i_pODBCStmt->ExecuteQuery(PROCEDURE_100506_0562))
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DB ERROR] CAtumFieldDBManager::DBLoadInfinityDifficultMonsterInfo call PROCEDURE_100506_0562 \r\n");
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}
	
	INFINITY_DIFFICULTY_MONSTER_SETTING_INFO	tmListInfo;
	util::zero(&tmListInfo, sizeof(INFINITY_DIFFICULTY_MONSTER_SETTING_INFO));
	
	SQLINTEGER	arrCB[6] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &tmListInfo.iIncreaseStep,			0, &arrCB[0]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_LONG, &tmListInfo.iMonsterHP,				0, &arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_LONG, &tmListInfo.iMonsterDefense,		0, &arrCB[2]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_LONG, &tmListInfo.iMonsterEvasion,		0, &arrCB[3]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_LONG, &tmListInfo.iMonsterAttackItem,		0, &arrCB[4]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 6, SQL_C_LONG, &tmListInfo.iMonsterPopulation,		0, &arrCB[5]);
	
	RETCODE	ret;
	while ( (ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA) 
	{
		o_vectorDifficultyMonsterInfo->push_back(tmListInfo);
		util::zero(&tmListInfo, sizeof(INFINITY_DIFFICULTY_MONSTER_SETTING_INFO));
	}
	i_pODBCStmt->FreeStatement();
	
	return TRUE;	
}

// 2010-08-05 by dhjin, ���׸� -
void CAtumFieldDBManager::QP_LoadBurningMap(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{// QT_LoadBurningMap
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE dbo.atum_Load_BurningMap
	AS
		SELECT BuringMapUID, MapIndex, ReqUnitKind, ReqMinLv, ReqMaxLv
		FROM dbo.ti_BurningMap WITH(NOLOCK)
	GO
	**************************************************************************/
	RETCODE	ret = SQLExecDirect(hstmt, PROCEDURE_100805_0566, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Load_BurningMap Failed!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	
	SQLINTEGER	cb[6] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,  SQL_NTS};
	BURNING_MAP_INFO		tmBurningMapInfo;
	util::zero(&tmBurningMapInfo, sizeof(BURNING_MAP_INFO));
	
	SQLBindCol(hstmt, 1, SQL_C_ULONG, &tmBurningMapInfo.BurningMapUID, 0,	&cb[1]);
	SQLBindCol(hstmt, 2, SQL_C_ULONG, &tmBurningMapInfo.MapIndex, 0,		&cb[2]);
	SQLBindCol(hstmt, 3, SQL_C_USHORT, &tmBurningMapInfo.ReqUnitKind, 0,		&cb[3]);
	SQLBindCol(hstmt, 4, SQL_C_UTINYINT, &tmBurningMapInfo.ReqMinLv, 0,		&cb[4]);
	SQLBindCol(hstmt, 5, SQL_C_UTINYINT, &tmBurningMapInfo.ReqMaxLv, 0,	&cb[5]);
	
	this->m_pFieldIOCP8->m_BurningMapList.clear();
	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA) {
		this->m_pFieldIOCP8->m_BurningMapList.push_back(tmBurningMapInfo);
		util::zero(&tmBurningMapInfo, sizeof(BURNING_MAP_INFO));
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
}


// 2010-06-25 by shcho, ���Ǵ�Ƽ ���÷α� ��� - ���� ������ ���� DB����
void CAtumFieldDBManager::QP_Log_UserGetTenderItem(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{

	// QT_Log_UserGetTenderItem
	QPARAM_INFINITY_LOG_USER_GET_TENDERITEM_INFO	*pQParam = (QPARAM_INFINITY_LOG_USER_GET_TENDERITEM_INFO*)q.pQueryParam;
	CFieldIOCPSocket	*pFieldIOCPSocket = q.pFieldIOCPSocket;
	if(NULL == pFieldIOCPSocket) 
	{
		util::del(pQParam);  
		return;
	}

	/*[Stored Query Definition]************************************************
	-- PROCEDURE NAME	: dbo.atum_Insert_Log_Infinity_User_Get_TenderItem
	--------------------------------------------------------------------------------
	-- DESC				: -- // 2010-06-25 by shcho, ���Ǵ�Ƽ ���÷α� ��� - ���� ������ ���� DB����
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Insert_Log_Infinity_User_Get_TenderItem
		@i_AccountUID			INT,
		@i_AccountName			VARCHAR(30),
		@i_CharacterUID			INT,
		@i_CharacterName		VARCHAR(30),

		@i_DiceCount			TINYINT,
		@i_ItemNum				INT,
		@i_ItemCount			INT
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
	AS
		INSERT INTO dbo.atum_Log_Infinity_UserGetTenderItemInfo(AccountUID, AccountName, CharacterUID, CharacterName, DiceCount, ItemNum, ItemCount)
		VALUES (@i_AccountUID, @i_AccountName, @i_CharacterUID, @i_CharacterName, @i_DiceCount, @i_ItemNum, @i_ItemCount)
	GO
	**************************************************************************/
	// üũ - ���� ������
	RETCODE tret = 0;
	SQLINTEGER	arrCB[7] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	tret = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER,		0, 0, &pFieldIOCPSocket->m_character.AccountUniqueNumber, 0, &arrCB[1]);
	tret = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0,  &pFieldIOCPSocket->m_character.AccountName, 0, &arrCB[2]);
	tret = SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER,		0, 0, &pFieldIOCPSocket->m_character.CharacterUniqueNumber, 0, &arrCB[3]);
	tret = SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, &pFieldIOCPSocket->m_character.CharacterName, 0, &arrCB[4]);
	tret = SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT,	0, 0,	&pQParam->GDicecount, 0, &arrCB[5]);
	tret = SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER,		0, 0, &pQParam->TenderItemNum, 0, &arrCB[6]);
	
	//���� ���ν��� ����
	RETCODE ret = SQLExecDirect(hstmt,PROCEDURE_100914_0001, SQL_NTS);
	
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_Insert_Log_Infinity_User_Get_TenderItem Failed !\r\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQParam);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);	
	util::del(pQParam);
}
// END 2010-06-25 by shcho, ���Ǵ�Ƽ ���÷α� ��� - ���� ������ ���� DB����

// 2010-06-15 by shcho&hslee ��ý��� - ��ϵ� ������ ���� ���� �Լ� �߰� 
void CAtumFieldDBManager::SendItemData( CFieldIOCPSocket *pFieldIOCPSocket, BYTE ItemInsertionType, ITEM_GENERAL *pItem, tPET_CURRENTINFO *pcurrentData )
{

	//  �� �������̸� �����͸� �����Ѵ�.
	if( NULL != pcurrentData )
		pFieldIOCPSocket->SendInsertPetItemInfo(ItemInsertionType, pcurrentData);

	INIT_MSG_WITH_BUFFER(MSG_FC_STORE_INSERT_ITEM, T_FC_STORE_INSERT_ITEM, msgInsertItem, msgInsertItemBuf);
	msgInsertItem->FromCharacterUniqueNumber	= pFieldIOCPSocket->m_character.CharacterUniqueNumber;
	msgInsertItem->ItemInsertionType			= ItemInsertionType;
	msgInsertItem->ItemGeneral					= *pItem;
	pFieldIOCPSocket->SendAddData(msgInsertItemBuf, MSG_SIZE(MSG_FC_STORE_INSERT_ITEM));
}


/////////////////////////////////////////////////////////////
// start 2011-08-22 by hskim, ��Ʈ�� �ý��� 2�� - ��� ����

///////////////////////////////////////////////////////////////////////////////
/// \fn			CAtumFieldDBManager::QP_PetUpdateInfinityFin(FIELD_DB_QUERY q, SQLHSTMT hstmt)		
/// \brief		�� ���� ������Ʈ (���Ǵ�Ƽ ���� �� ���� ������Ʈ)
/// \author		hskim
/// \date		2011-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_PetUpdateInfinityFin(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/*[Query Definition]************************************************
	-------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Update_PetInfo
	-- DESC				: �� ���� ������Ʈ
	-- // 2011-09-30 by hskim, ��Ʈ�� �ý���
	-------------------------------------------------------------
	CREATE PROC [dbo].[atum_Update_PetInfo]
		@i_ItemUID					BIGINT,
		@i_Experience				FLOAT,
		@i_Level					INT,
		@i_Name						VARCHAR(40),
		@i_ExpRatio					TINYINT,
		@i_EnableSocketCount		TINYINT,
		@i_HPKitItemNum				INT,
		@i_HPKitTriggerValue		FLOAT,
		@i_ShieldKitItemNum			INT,
		@i_ShieldKitTriggerValue	FLOAT,
		@i_SPKitItemNum				INT,
		@i_SPKitTriggerValue		FLOAT,
		@i_AutoSkilItemNum1			INT,
		@i_AutoSkilItemNum2			INT,
		@i_AutoSkilItemNum3			INT,
		@i_AutoSkilItemNum4			INT,
		@i_AutoSkilItemNum5			INT,
		@i_AutoSkilItemNum6			INT
	AS
	BEGIN
		UPDATE [dbo].[td_pet] 
			SET Experience = @i_Experience, [Level] = @i_Level, Name = @i_Name, ExpRatio = @i_ExpRatio, 
				EnableSocketCount = @i_EnableSocketCount,
				HPKitItemNum = @i_HPKitItemNum, HPKitTriggerValue = @i_HPKitTriggerValue, 
				ShieldKitItemNum = @i_ShieldKitItemNum, ShieldKitTriggerValue = @i_ShieldKitTriggerValue,
				SPKitItemNum = @i_SPKitItemNum, SPKitTriggerValue = @i_SPKitTriggerValue,
				AutoSkilItemNum1 = @i_AutoSkilItemNum1, AutoSkilItemNum2 = @i_AutoSkilItemNum2,
				AutoSkilItemNum3 = @i_AutoSkilItemNum3, AutoSkilItemNum4 = @i_AutoSkilItemNum4,
				AutoSkilItemNum5 = @i_AutoSkilItemNum5, AutoSkilItemNum6 = @i_AutoSkilItemNum6
			WHERE [ItemUID] = @i_ItemUID;		
	END
	GO
	**************************************************************************/
	QPARAM_PET_UPDATE_INFINITY_FIN *pRMsg = (QPARAM_PET_UPDATE_INFINITY_FIN *)q.pQueryParam;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->UniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_FLOAT, 0, 0, &pRMsg->PetExp, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->PetLevel, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_PET_NAME, 0, pRMsg->PetName, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRMsg->PetExpRatio, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRMsg->PetEnableSocketCount, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->PetKitHP.ItemNum, 0, NULL);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pRMsg->PetKitHP.TriggerValue, 0, NULL);
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->PetKitShield.ItemNum, 0, NULL);
	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pRMsg->PetKitShield.TriggerValue, 0, NULL);
	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->PetKitSP.ItemNum, 0, NULL);
	SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pRMsg->PetKitSP.TriggerValue, 0, NULL);

	for(int a=0; a<SIZE_MAX_PETSOCKET_AUTOSKILL_SLOT; a++)
	{
		SQLBindParameter(hstmt, a + 13, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->PetAutoSkill.ItemNum[a], 0, NULL);
	}

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_110824_0008, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "dbo.atum_Update_PetInfo Failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CAtumFieldDBManager::QP_PetSetName(FIELD_DB_QUERY q, SQLHSTMT hstmt)		
/// \brief		�� �̸� ����
/// \author		hskim
/// \date		2011-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_PetSetName(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/*[Query Definition]************************************************
	-------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Update_PetName
	-- DESC				: �� �̸� ����
	-- // 2011-08-05 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ� ���� ����
	-------------------------------------------------------------
	CREATE PROC [dbo].[atum_Update_PetName]
		@i_ItemUID		BIGINT,
		@i_Name			VARCHAR(40)
	AS
	BEGIN
		UPDATE [dbo].[td_pet] SET [Name] = @i_Name WHERE [ItemUID] = @i_ItemUID;		
	END
	GO
	**************************************************************************/
	QPARAM_PET_SET_NAME *pRMsg = (QPARAM_PET_SET_NAME *)q.pQueryParam;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_PET_NAME, 0, pRMsg->PetName, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_110824_0001, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "dbo.atum_Update_PetName Failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CAtumFieldDBManager::OP_SetPetExpRatio(FIELD_DB_QUERY q, SQLHSTMT hstmt)		
/// \brief		�� ����ġ ȹ�� ���� ����
/// \author		hskim
/// \date		2011-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::OP_PetSetExpRatio(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_PET_SET_EXP_RATIO *pRMsg = (QPARAM_PET_SET_EXP_RATIO *)q.pQueryParam;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRMsg->ExpRatio, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_110824_0002, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "dbo.atum_Update_PetExpRatio Failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CAtumFieldDBManager::QP_PetSetLevel(FIELD_DB_QUERY q, SQLHSTMT hstmt)		
/// \brief		�� ���� ����
/// \author		hskim
/// \date		2011-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_PetSetLevel(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/*[Query Definition]************************************************
	-------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Update_PetLevel
	-- DESC				: �� �̸� ����
	-- // 2011-08-05 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ� ���� ����
	-------------------------------------------------------------
	CREATE PROC [dbo].[atum_Update_PetLevel]
		@i_ItemUID		BIGINT,
		@i_Level		INT
	AS
	BEGIN
		UPDATE [dbo].[td_pet] SET [Level] = @i_Level WHERE [ItemUID] = @i_ItemUID;		
	END
	GO
	**************************************************************************/
	QPARAM_PET_SET_LEVEL *pRMsg = (QPARAM_PET_SET_LEVEL *)q.pQueryParam;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Level, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_110824_0003, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "dbo.atum_Update_PetLevel Failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CAtumFieldDBManager::QP_PetSetExp(FIELD_DB_QUERY q, SQLHSTMT hstmt)		
/// \brief		�� ����ġ ����
/// \author		hskim
/// \date		2011-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::OP_PetSetExp(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/*[Query Definition]************************************************
	-------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Update_PetExp
	-- DESC				: �� ����ġ ����
	-- // 2011-08-05 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ� ���� ����
	-------------------------------------------------------------
	CREATE PROC [dbo].[atum_Update_PetExp]
		@i_ItemUID		BIGINT,
		@i_Exp			FLOAT
	AS
	BEGIN
		UPDATE [dbo].[td_pet] SET [Experience] = @i_Exp WHERE [ItemUID] = @i_ItemUID;
	END
	GO
	**************************************************************************/
	QPARAM_PET_SET_EXP *pRMsg = (QPARAM_PET_SET_EXP *)q.pQueryParam;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_FLOAT, 0, 0, &pRMsg->Exp, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_110824_0004, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "dbo.atum_Update_Exp Failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CAtumFieldDBManager::OP_PetSetSocket(FIELD_DB_QUERY q, SQLHSTMT hstmt)		
/// \brief		�� ���� ������ ����
/// \author		hskim
/// \date		2011-08-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::OP_PetSetSocket(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/*[Query Definition]************************************************
	-------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Update_PetSocket
	-- DESC				: �� ���� ������ ����
	-- // 2011-08-30 by hskim, ��Ʈ�� �ý��� 2��
	-------------------------------------------------------------
	CREATE PROC [dbo].[atum_Update_PetSocket]
		@i_ItemUID		BIGINT,
		@i_SocketItem1	INT,
		@i_SocketItem2	INT,
		@i_SocketItem3	INT,
		@i_SocketItem4	INT,
		@i_SocketItem5	INT,
		@i_SocketItem6	INT
	AS
	BEGIN
		UPDATE [dbo].[td_pet] 
			SET [SocketItem1] = @i_SocketItem1, [SocketItem2] = @i_SocketItem2, [SocketItem3] = @i_SocketItem3, 
				[SocketItem4] = @i_SocketItem4, [SocketItem5] = @i_SocketItem5, [SocketItem6] = @i_SocketItem6 WHERE [ItemUID] = @i_ItemUID;
	END
	GO
	**************************************************************************/
	QPARAM_PET_SET_SOCKET *pRMsg = (QPARAM_PET_SET_SOCKET *)q.pQueryParam;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->ItemUniqueNumber, 0, NULL);

	for(int a=0; a<SIZE_MAX_PETSOCKET; a++)
	{
		SQLBindParameter(hstmt, a + 2, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->PetSocketItemUID[a], 0, NULL);	
	}

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_110824_0005, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "dbo.atum_Update_PetSocket Failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CAtumFieldDBManager::OP_PetSetKitSlot(FIELD_DB_QUERY q, SQLHSTMT hstmt)		
/// \brief		�� ŰƮ ���� ����
/// \author		hskim
/// \date		2011-08-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::OP_PetSetKitSlot(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/*[Query Definition]************************************************
	-------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Update_PetKitSlot
	-- DESC				: �� ŰƮ ���� ����
	-- // 2011-08-30 by hskim, ��Ʈ�� �ý��� 2��
	-------------------------------------------------------------
	CREATE PROC [dbo].[atum_Update_PetKitSlot]
		@i_ItemUID					BIGINT,
		@i_HPKitItemNum				INT,
		@i_HPKitTriggerValue		FLOAT,
		@i_ShieldKitItemNum			INT,
		@i_ShieldKitTriggerValue	FLOAT,
		@i_SPKitItemNum				INT,
		@i_SPKitTriggerValue		FLOAT
	AS
	BEGIN
		UPDATE [dbo].[td_pet] 
			SET [HPKitItemNum] = @i_HPKitItemNum, [HPKitTriggerValue] = @i_HPKitTriggerValue, [ShieldKitItemNum] = @i_ShieldKitItemNum, 
				[ShieldKitTriggerValue] = @i_ShieldKitTriggerValue, [SPKitItemNum] = @i_SPKitItemNum, [SPKitTriggerValue] = @i_SPKitTriggerValue WHERE [ItemUID] = @i_ItemUID;
	END
	GO
	**************************************************************************/
	QPARAM_PET_SET_KIT_SLOT *pRMsg = (QPARAM_PET_SET_KIT_SLOT *)q.pQueryParam;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->PetKitHP.ItemNum, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pRMsg->PetKitHP.TriggerValue, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->PetKitShield.ItemNum, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pRMsg->PetKitShield.TriggerValue, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->PetKitSP.ItemNum, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pRMsg->PetKitSP.TriggerValue, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_110824_0006, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "dbo.atum_Update_PetKitSlot Failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CAtumFieldDBManager::OP_PetSetAutoSkillSlot(FIELD_DB_QUERY q, SQLHSTMT hstmt)		
/// \brief		�� ���� ��ų ���� ����
/// \author		hskim
/// \date		2011-08-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::OP_PetSetAutoSkillSlot(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/*[Query Definition]************************************************
	-------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Update_PetAutoSkillSlot
	-- DESC				: �� ���� ��ų ���� ����
	-- // 2011-08-30 by hskim, ��Ʈ�� �ý��� 2��
	-------------------------------------------------------------
	CREATE PROC [dbo].[atum_Update_PetAutoSkillSlot]
		@i_ItemUID				BIGINT,
		@i_AutoSkilItemNum1		INT,
		@i_AutoSkilItemNum2		INT,
		@i_AutoSkilItemNum3		INT,
		@i_AutoSkilItemNum4		INT,
		@i_AutoSkilItemNum5		INT,
		@i_AutoSkilItemNum6		INT
	AS
	BEGIN
		UPDATE [dbo].[td_pet] 
			SET [AutoSkilItemNum1] = @i_AutoSkilItemNum1, [AutoSkilItemNum2] = @i_AutoSkilItemNum2, [AutoSkilItemNum3] = @i_AutoSkilItemNum3, 
				[AutoSkilItemNum4] = @i_AutoSkilItemNum4, [AutoSkilItemNum5] = @i_AutoSkilItemNum5, [AutoSkilItemNum6] = @i_AutoSkilItemNum6 WHERE [ItemUID] = @i_ItemUID;
	END
	GO
	**************************************************************************/
	QPARAM_PET_SET_AUTOSKILL_SLOT *pRMsg = (QPARAM_PET_SET_AUTOSKILL_SLOT *)q.pQueryParam;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->ItemUniqueNumber, 0, NULL);

	for(int a=0; a<SIZE_MAX_PETSOCKET_AUTOSKILL_SLOT; a++)
	{
		SQLBindParameter(hstmt, a + 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->PetAutoSkill.ItemNum[a], 0, NULL);
	}

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_110824_0007, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "dbo.atum_Update_PetAutoSkillSlot Failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

// end 2011-08-22 by hskim, ��Ʈ�� �ý��� 2�� - ��� ����
/////////////////////////////////////////////////////////////

// start 2012-01-30 by hskim, ��Ʈ�� �ý��� 2�� - ���� â�� �̵�
///////////////////////////////////////////////////////////////////////////////
/// \fn			CAtumFieldDBManager::OP_PetChangeSocketOwner(FIELD_DB_QUERY q, SQLHSTMT hstmt)	
/// \brief		��Ʈ�� �ý��� 2�� - ���� â�� �̵�
/// \author		hskim
/// \date		2012-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::OP_PetChangeSocketOwner(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	SQLRETURN ret;
	QPARAM_PET_CHANGE_SOCKET_OWNER *pParam = (QPARAM_PET_CHANGE_SOCKET_OWNER*)q.pQueryParam;

	/*[Stored Query Definition]********************************************
	-------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Update_PetSocketOwner
	-- DESC				: ��Ʈ�� ���Ͼ������� ���� ĳ���� ����
	-- // 2012-01-30 by hskim, ��Ʈ�� �ý��� 2�� - ���� â�� �̵�
	-------------------------------------------------------------
	CREATE PROC [dbo].[atum_Update_PetSocketOwner]
		@i_SocketUN			BIGINT,
		@i_Possess			INT
	AS
	BEGIN
		UPDATE [dbo].[td_Store] SET [Possess] = @i_Possess WHERE [UniqueNumber] = @i_SocketUN
	END
	GO
	**********************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &pParam->ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pParam->CharacterUID, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_110824_0009, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO ) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"OP_PetChangeSocketOwner Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		util::del(pParam);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pParam);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CAtumFieldDBManager::OP_PetChangeSocketOwner(FIELD_DB_QUERY q, SQLHSTMT hstmt)	
/// \brief		��Ʈ�� �ý��� 2�� - ���� â�� �̵� (td_Store ���� ������ ������ �ϳ� �о�ͼ� Ŭ���̾�Ʈ�� ����)
/// \author		hskim
/// \date		2012-01-30
/// \warning	���� ���� üũ�� ���� ���� ó�� Ȯ���ϰ� �Ұ� (QP_NewGetStoreItem �Լ� ����)
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::OP_StoreGetItemOne(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	SQLRETURN ret;
	QPARAM_STORE_GET_ITEM_ONE *pParam	= (QPARAM_STORE_GET_ITEM_ONE*)q.pQueryParam;
	CFieldIOCPSocket *pFieldIOCPSocket	= q.pFieldIOCPSocket;

	if(NULL == pFieldIOCPSocket
		|| FALSE == pFieldIOCPSocket->IsValidCharacter(FALSE))
	{
		util::del(pParam);
		return;
	}

	/*[Stored Query Definition]********************************************

	**********************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &pParam->ItemUniqueNumber, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_110824_0010, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"OP_StoreGetItemOne Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		util::del(pParam);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	SQL_TIMESTAMP_STRUCT tmCreatedTimeStamp;
	SQLINTEGER		arrCB[19] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,
								SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};

	if( SQL_NO_DATA != ret )
	{
		ITEM_GENERAL	itemG(NULL);
		SQLBindCol(hstmt, 1, SQL_C_SBIGINT, &itemG.UniqueNumber, 0,				&arrCB[1]);
		SQLBindCol(hstmt, 2, SQL_C_ULONG, &itemG.AccountUniqueNumber, 0,		&arrCB[2]);
		SQLBindCol(hstmt, 3, SQL_C_ULONG, &itemG.Possess, 0,					&arrCB[3]);
		SQLBindCol(hstmt, 4, SQL_C_UTINYINT, &itemG.ItemStorage, 0,				&arrCB[4]);
		SQLBindCol(hstmt, 5, SQL_C_UTINYINT, &itemG.Wear, 0,					&arrCB[5]);
		SQLBindCol(hstmt, 6, SQL_C_SLONG, &itemG.CurrentCount, 0,				&arrCB[6]);
		SQLBindCol(hstmt, 7, SQL_C_SLONG, &itemG.ItemWindowIndex, 0,			&arrCB[7]);
		SQLBindCol(hstmt, 8, SQL_C_SLONG, &itemG.ItemNum, 0,					&arrCB[8]);
		SQLBindCol(hstmt, 9, SQL_C_SSHORT, &itemG.NumOfEnchants, 0,				&arrCB[9]);
		SQLBindCol(hstmt, 10, SQL_C_LONG, &itemG.PrefixCodeNum, 0,				&arrCB[10]);
		SQLBindCol(hstmt, 11, SQL_C_LONG, &itemG.SuffixCodeNum, 0,				&arrCB[11]);
		SQLBindCol(hstmt, 12, SQL_C_FLOAT, &itemG.CurrentEndurance, 0,			&arrCB[12]);
		SQLBindCol(hstmt, 13, SQL_C_LONG, &itemG.ColorCode, 0,					&arrCB[13]);
		SQLBindCol(hstmt, 14, SQL_C_LONG, &itemG.UsingTimeStamp, 0,				&arrCB[14]);
		SQLBindCol(hstmt, 15, SQL_C_TIMESTAMP, &tmCreatedTimeStamp, 0,			&arrCB[15]);
		SQLBindCol(hstmt, 16, SQL_C_LONG, &itemG.ShapeItemNum, 0,				&arrCB[16]);
		SQLBindCol(hstmt, 17, SQL_C_SBIGINT, &itemG.MainSvrItemUID, 0,			&arrCB[17]);
		SQLBindCol(hstmt, 18, SQL_C_ULONG, &itemG.CoolingTimeStamp, 0,			&arrCB[18]);

		util::zero(&itemG, sizeof(ITEM_GENERAL));
		util::zero(&tmCreatedTimeStamp, sizeof(tmCreatedTimeStamp));
		while( (ret = SQLFetch(hstmt)) != SQL_NO_DATA )
		{
			if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO )
			{
				ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_GetStoreItemOne Fetching Failed!\n", TRUE, q.pFieldIOCPSocket, q);
				break;
			}

			ITEM *pItemInfo = m_pFieldIOCP8->GetItemInfo(itemG.ItemNum);
			if( pItemInfo != NULL )
			{
				// ���⼭ ó��

				itemG.CreatedTime		= tmCreatedTimeStamp;
				itemG.ItemInfo			= pItemInfo;
				itemG.Kind				= pItemInfo->Kind;

				ITEM_GENERAL *pItemG	= new ITEM_GENERAL(itemG);

				if( FALSE == pFieldIOCPSocket->m_ItemManager.InsertItemGeneralByPointer(pItemG, IUT_DEPENDENCY_ITEM, FALSE, ITEM_IN_CHARACTER, FALSE, pParam->bSendToClient) )
				{
					util::del(pItemG);
				}
			}

			util::zero(&itemG, sizeof(ITEM_GENERAL));
			util::zero(&tmCreatedTimeStamp, sizeof(tmCreatedTimeStamp));
		}
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pParam);
}
// end 2012-01-30 by hskim, ��Ʈ�� �ý��� 2�� - ���� â�� �̵�

// start 2012-01-16 by hskim, ��� - ȭ��
void CAtumFieldDBManager::QP_InfinityComBackPostWork(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	// QT_InfinityComBackPostWork
	MSG_FtoA_INFINITY_COME_BACK_POSTWORK *pQParam = (MSG_FtoA_INFINITY_COME_BACK_POSTWORK*)q.pQueryParam;
	CFieldIOCPSocket *pFieldIOCPSocket	= q.pFieldIOCPSocket;

	if( NULL == pFieldIOCPSocket )
	{
		util::del(pQParam);
		return;
	}

	util::del(pQParam);
}
// end 2012-01-16 by hskim, ��� - ȭ��

///////////////////////////////////////////////////////////////////////////////
// 2012-04-12 by jhseol, �Ʒ��� �߰����� - �Ʒ��� �÷��� ī��Ʈ Ȯ�� �Լ� ���� 
///////////////////////////////////////////////////////////////////////////////
/// \fn			CAtumFieldDBManager::QP_ArenaPlayCount(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		�Ʒ��� �÷��� Ƚ�� Ȯ�� �� ������Ʈ �Լ�
/// \author		jhseol
/// \date		2012-04-12
/// \warning	���� ���� üũ�� ���� ���� ó�� Ȯ���ϰ� �Ұ� (QP_NewGetStoreItem �Լ� ����)
///
/// \param		
/// \return		
void CAtumFieldDBManager::QP_ArenaPlayCount(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	/////////////////////////////////////////////////////////////////////
	/// ������ �����ϰ� ��ȿ���� üũ
	CFieldIOCPSocket *pFieldIOCPSocket	= q.pFieldIOCPSocket;
	if(NULL == pFieldIOCPSocket
		|| FALSE == pFieldIOCPSocket->IsValidCharacter(FALSE))
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"QP_ArenaPlayCount in IsValidCharacter Error!\n", TRUE, q.pFieldIOCPSocket, q);
		return;
	}


	/////////////////////////////////////////////////////////////////////
	/// �ʿ��� ������ ����
	SArenaPlayInfo	ArenaPlayInfo;
	ATUM_DATE_TIME	CurTime, ResetStandardTime;		/// ���� �ð��� ���� Ÿ���� ������ ����
	vectorInfinityModeInfo tempResetInfo;			/// ����Ƽ���� ���� �ð��� ���������� ���Ǵ�Ƽ�� ������ ���� ����



	/////////////////////////////////////////////////////////////////////
	/// ������ �ʱ�ȭ
	util::zero(&ArenaPlayInfo, sizeof(SArenaPlayInfo));
	util::zero(&CurTime, sizeof(ATUM_DATE_TIME));
	util::zero(&ResetStandardTime, sizeof(ATUM_DATE_TIME));
	tempResetInfo.clear();


	/////////////////////////////////////////////////////////////////////
	/// ������������ ����ð� ����
	CurTime.SetCurrentDateTime();
	ResetStandardTime.SetCurrentDateTime();


	/////////////////////////////////////////////////////////////////////
	/// tempResetInfo�� ���Ǵ�Ƽ�� ������ ��������
	pFieldIOCPSocket->ms_pFieldIOCP->m_InfinityManager.GetInfinityModeInfo(&tempResetInfo);
	

	/////////////////////////////////////////////////////////////////////
	/// itr�� tempResetInfo�� ù��° ���������� ����
	vectorInfinityModeInfo::iterator itr = tempResetInfo.begin();


	/////////////////////////////////////////////////////////////////////
	/// itr�� ��ȿ�� üũ
	if(itr == tempResetInfo.end())
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"QP_ArenaPlayCount in Arena Reset Time Load Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	

	/////////////////////////////////////////////////////////////////////
	/// ���±��ؽð��� [��, ��, ��]�� �����ϰ� [��, ��, ��]�� ���Ǵ�Ƽ�� ���� �ð����� ����
	ResetStandardTime.Hour = itr->ResetCycle;
	ResetStandardTime.Minute = 0;
	ResetStandardTime.Second = 0;


	/////////////////////////////////////////////////////////////////////
	/// ���� [��, ��, ��]�� ���� �ð� ���̸� ���� �Ϸ簡 ������ �ʾ������� ���� �ð����� �Ϸ縦 ���ش�
	if( ! ( itr->ResetCycle <= CurTime.Hour && 0 <= CurTime.Minute && 0 <= CurTime.Second ) )
	{
		ResetStandardTime.AddDateTime(0,0,-1,0,0,0);
	}
	pFieldIOCPSocket->m_character.ArenaPlayInfo.atimeResetTime = ResetStandardTime;
//	pFieldIOCPSocket->ArenaPlayInfoUpdataToMF();

}
// end 2012-04-12 by jhseol, �Ʒ��� �߰����� - �Ʒ��� �÷��� ī��Ʈ Ȯ�� �Լ� ����

// start 2012-08-27 by hskim, ��Ʈ�� ��þƮ �� ���� �߰� ���� ó�� (�ڵ���� �������� �߰ߵ��� �ʾ����� ������ ��쿡�� �α׸� �����) (����ȭ DB ������ �õ��ؼ� ���� ���ϰ� ���� : �ʿ���ٰ� �ǴܵǸ� ���� ����)
void CAtumFieldDBManager::QP_GetEnchantCount(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	// �� �Լ��� ExecuteQuery �Լ� ȣ������� ���� (���� �Ұ�)

	QPARAM_GET_ENCHANT_COUNT *pQParam = (QPARAM_GET_ENCHANT_COUNT*)q.pQueryParam;
	CFieldIOCPSocket *pFieldIOCPSocket	= q.pFieldIOCPSocket;

	if( NULL == pFieldIOCPSocket )
	{
		// util::del(pQParam); <- pQParam �������� �Ҵ����� �ʾ���, �ش� �κ� ������, �ּ� �츮�� ����, ����� ��~~~~

		return;
	}

	SQLINTEGER cb = SQL_NTS;

	/********************************************************************************
	CREATE PROC [dbo].[atum_GetEnchantCountByItemUID]
		@i_TargetItemUniqueNumber		BIGINT
	AS
	BEGIN
		SELECT count(*) FROM [dbo].[td_Enchant] WHERE [TargetItemUniqueNumber] = @i_TargetItemUniqueNumber
	END
	GO
	********************************************************************************/

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &pQParam->ItemUniqueNumber, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_120827_0003, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_GetEnchantCountByItemUID Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	SQLBindCol(hstmt, 1, SQL_C_SBIGINT, &pQParam->EnchantCount, 0, &cb);
	ret = SQLFetch(hstmt);
	SQLFreeStmt(hstmt, SQL_CLOSE);

	// util::del(pQParam); <- pQParam �������� �Ҵ����� �ʾ���, �ش� �κ� ������, �ּ� �츮�� ����, ����� ��~~~~
}
// end 2012-08-27 by hskim, ��Ʈ�� ��þƮ �� ���� �߰� ���� ó�� (�ڵ���� �������� �߰ߵ��� �ʾ����� ������ ��쿡�� �α׸� �����) (����ȭ DB ������ �õ��ؼ� ���� ���ϰ� ���� : �ʿ���ٰ� �ǴܵǸ� ���� ����)

#if S_BONUSEXPSYSTEM_RENEWAL
// 2012-10-07 by hskim, �޽� ����ġ
void CAtumFieldDBManager::QP_GetAddExpRestExperienceCount(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_ADDEXP_REST_EXPERIENCE_COUNT *pQParam = (QPARAM_ADDEXP_REST_EXPERIENCE_COUNT*)q.pQueryParam;
	CFieldIOCPSocket *pFieldIOCPSocket = q.pFieldIOCPSocket;

	if (NULL == pFieldIOCPSocket || FALSE == pFieldIOCPSocket->IsValidCharacter(FALSE))
	{
		util::del(pQParam);

		return;
	}

	pFieldIOCPSocket->m_nRestExperienceCount = 0;

#ifdef S_ADDITIONAL_EXPERIENCE
#else
	util::del(pQParam);

	return;		// �޽� ����ġ ON / OFF ���
#endif

	if( TRUE == g_pFieldGlobal->IsArenaServer() )			// �Ʒ��������� ��� ����
	{
		util::del(pQParam);

		return;
	}

	SQLINTEGER cb = SQL_NTS;

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: [atum_Get_AddExp_RestExperienceCount]
	-- DESC				: // 2012-10-07 by hskim, �޽� ����ġ
	--------------------------------------------------------------------------------
	CREATE PROC [dbo].[atum_Get_AddExp_RestExperienceCount]
	@i_AccountUniqueNumber		INT
	AS
	-- YD ���� �ڵ�
	BEGIN
	SELECT RestExperienceCount FROM dbo.td_AdditionalExperience WITH (NOLOCK) WHERE AccountUniqueNumber = @i_AccountUniqueNumber
	END
	GO
	************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQParam->AccountUID, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_121008_0001, SQL_NTS);

	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "atum_Get_AddExp_RestExperienceCount Failed (1) !!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQParam);

		return;
	}

	SQLBindCol(hstmt, 1, SQL_C_ULONG, &pFieldIOCPSocket->m_nRestExperienceCount, 0, NULL);
	ret = SQLFetch(hstmt);

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQParam);

	// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
#if S_BONUSEXPSYSTEM_RENEWAL
	pFieldIOCPSocket->SendToClient_RestAndKillCount();
#endif
}

void CAtumFieldDBManager::QP_SetAddExpRestExperienceCount(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_ADDEXP_REST_EXPERIENCE_COUNT *pQParam = (QPARAM_ADDEXP_REST_EXPERIENCE_COUNT*)q.pQueryParam;
	CFieldIOCPSocket *pFieldIOCPSocket = q.pFieldIOCPSocket;

	SQLINTEGER cb = SQL_NTS;

	if (TRUE == g_pFieldGlobal->IsArenaServer())		// �Ʒ��������� ��� ����
	{
		util::del(pQParam);

		return;
	}

#ifdef S_ADDITIONAL_EXPERIENCE
#else
	util::del(pQParam);

	return;		// �޽� ����ġ ON / OFF ���
#endif

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: [atum_Set_AddExp_RestExperienceCount]
	-- DESC				: // 2012-10-07 by hskim, �޽� ����ġ
	--------------------------------------------------------------------------------
	CREATE PROC [dbo].[atum_Set_AddExp_RestExperienceCount]
	@i_AccountUniqueNumber		INT,
	@i_RestExperienceCount		INT
	AS
	-- YD ���� �ڵ�
	BEGIN
	DECLARE @RestExperienceCount	INT

	SELECT @RestExperienceCount = RestExperienceCount FROM dbo.td_AdditionalExperience WITH (NOLOCK) WHERE AccountUniqueNumber = @i_AccountUniqueNumber

	IF( @RestExperienceCount IS NULL )
	BEGIN
	INSERT INTO [dbo].[td_AdditionalExperience] ([AccountUniqueNumber], [RestExperienceCount]) VALUES (@i_AccountUniqueNumber, @i_RestExperienceCount)
	END
	ELSE
	BEGIN
	UPDATE dbo.td_AdditionalExperience SET RestExperienceCount = @i_RestExperienceCount WHERE AccountUniqueNumber = @i_AccountUniqueNumber
	END
	END
	GO
	************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQParam->AccountUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQParam->RestExperienceCount, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_121008_0002, SQL_NTS);

	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "atum_Set_AddExp_RestExperienceCount Failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQParam);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQParam);
}
// end 2012-10-07 by hskim, �޽� ����ġ
#endif // S_BONUSEXPSYSTEM_RENEWAL

// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)
void CAtumFieldDBManager::QP_GetFixedTerm(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_FIXED_TERM_SHAPE *pQParam = (QPARAM_FIXED_TERM_SHAPE*)q.pQueryParam;
	CFieldIOCPSocket *pFieldIOCPSocket	= q.pFieldIOCPSocket;

	if( NULL == pFieldIOCPSocket || FALSE == pFieldIOCPSocket->IsValidCharacter(FALSE) )
	{
		//util::del(pQParam);
		return;
	}

	RETCODE ret;
	SDWORD pcbNTS = SQL_NTS;

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME : [atum_Get_FixedTerm]
	--					// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)
	--------------------------------------------------------------------------------
	CREATE PROC [dbo].[atum_Get_FixedTerm]
		@i_ItemUID			BIGINT,
		@i_TermType			SMALLINT
	AS
	-- YD ���� �ڵ�
	BEGIN
		SELECT AppliedItemNum, StartDate, EndDate FROM [dbo].[td_FixedTerm] WHERE [ItemUID] = @i_ItemUID and [TermType] = @i_TermType
	END
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pQParam->ItemUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pQParam->TermType, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_121010_0001, SQL_NTS);

	if(ret == SQL_NO_DATA)
	{
		SQLFreeStmt(hstmt, SQL_CLOSE);
		//util::del(pQParam);
		return;
	}

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"QP_GetFixedTerm Failed!! (1)\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		//util::del(pQParam);
		return;
	}

	SQLINTEGER	arrCB[4] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	SQL_TIMESTAMP_STRUCT StartDate; memset(&StartDate, 0x00, sizeof(SQL_TIMESTAMP_STRUCT));
	SQL_TIMESTAMP_STRUCT EndDate; memset(&EndDate, 0x00, sizeof(SQL_TIMESTAMP_STRUCT));
	
	SQLBindCol(hstmt, 1, SQL_C_ULONG, &pQParam->AppliedItemNum, 0, &arrCB[1]);
	SQLBindCol(hstmt, 2, SQL_C_TIMESTAMP, &StartDate, 0, &arrCB[2]);
	SQLBindCol(hstmt, 3, SQL_C_TIMESTAMP, &EndDate, 0, &arrCB[3]);
	
	if( (ret = SQLFetch(hstmt)) == SQL_NO_DATA )
	{
		SQLFreeStmt(hstmt, SQL_CLOSE);
		//util::del(pQParam);
		return;		
	}

	pQParam->StartDate = StartDate;
	pQParam->EndDate = EndDate;

	SQLFreeStmt(hstmt, SQL_CLOSE);
	//util::del(pQParam);
}

void CAtumFieldDBManager::QP_InsertFixedTerm(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_FIXED_TERM_SHAPE *pQParam = (QPARAM_FIXED_TERM_SHAPE*)q.pQueryParam;
	CFieldIOCPSocket *pFieldIOCPSocket	= q.pFieldIOCPSocket;

	if( NULL == pFieldIOCPSocket || FALSE == pFieldIOCPSocket->IsValidCharacter(FALSE) )
	{
		util::del(pQParam);

		return;
	}

	SQLINTEGER cb = SQL_NTS;

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME :	[atum_Insert_FixedTerm]
	--					// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)
	--------------------------------------------------------------------------------
	CREATE PROC [dbo].[atum_Insert_FixedTerm]
		@i_ItemUID			BIGINT,
		@i_TermType			SMALLINT,
		@i_AppliedItemNum	INT,
		@i_StartDate		DATETIME,
		@i_EndDate			DATETIME
	AS
	-- YD ���� �ڵ�
	BEGIN
		INSERT INTO [dbo].[td_FixedTerm] ([ItemUID], [TermType], [AppliedItemNum], [StartDate], [EndDate])
		 VALUES (@i_ItemUID, @i_TermType, @i_AppliedItemNum, @i_StartDate, @i_EndDate)
	END
	GO
	************************************************************************/

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &pQParam->ItemUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pQParam->TermType, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,	&pQParam->AppliedItemNum, 0, NULL);

	char StartTimeString[SIZE_MAX_SQL_DATETIME_STRING];
	pQParam->StartDate.GetSQLDateTimeString(StartTimeString, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, StartTimeString, 0, NULL);

	char EndTimeString[SIZE_MAX_SQL_DATETIME_STRING];
	pQParam->EndDate.GetSQLDateTimeString(EndTimeString, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, EndTimeString, 0, NULL);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_121010_0002, SQL_NTS);

	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) 
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"QP_InsertFixedTerm Failed !!\n", TRUE, q.pFieldIOCPSocket, q);
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQParam);
}

// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - CAtumFieldDBManager::QP_ �Լ� ������
void CAtumFieldDBManager::QP_GetShapeStatLevel(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_SHAPE_STATLEVEL *pQParam = (QPARAM_SHAPE_STATLEVEL*)q.pQueryParam;
	CFieldIOCPSocket *pFieldIOCPSocket	= q.pFieldIOCPSocket;
	
	if( NULL == pFieldIOCPSocket || FALSE == pFieldIOCPSocket->IsValidCharacter(FALSE) )
	{
		return;
	}

	/************************************************************************
	--------------------------------------------------------------------------------
		-- CREATE PROCEDURE : [dbo].[atum_Get_ShapeStatLevel]
		-- ���ν��� ���� : 
	--------------------------------------------------------------------------------
		CREATE PROC [dbo].[atum_Get_ShapeStatLevel]
		@i_ItemUID			BIGINT
		--WITH EXECUTE AS 'proexe'
		AS
		
		BEGIN
			SELECT [ItemKind],[ItemUID],[ShapeItemNum],[AllStatLevel] FROM [dbo].[td_ShapeStatLevel] WITH(NOLOCK)
			WHERE [ItemUID] = @i_ItemUID
		END
	GO
	************************************************************************/

	SQLINTEGER cb = SQL_NTS;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &pQParam->ItemUID, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_130531_0003, SQL_NTS);
	
	if(ret == SQL_NO_DATA)
	{
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"QP_GetShapeStatLevel Failed!! (1)\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	
	SQLINTEGER	arrCB[5] = {SQL_NTS, SQL_NTS, SQL_NTS,SQL_NTS, SQL_NTS};				
	SQLBindCol(hstmt, 1, SQL_C_UTINYINT,&pQParam->ItemKind,			0, &arrCB[1]);
	SQLBindCol(hstmt, 2, SQL_C_UBIGINT, &pQParam->ItemUID,			0, &arrCB[2]);
	SQLBindCol(hstmt, 3, SQL_C_ULONG,	&pQParam->nStatShapeItemNum,0, &arrCB[3]);
	SQLBindCol(hstmt, 4, SQL_C_ULONG,	&pQParam->nStatLevel,		0, &arrCB[4]);
	
	
	if( (ret = SQLFetch(hstmt)) == SQL_NO_DATA )
	{
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;		
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	//util::del(pQParam);		// �Լ��� ȣ���� �ۿ��� ������ �Ѵ�.
}

void CAtumFieldDBManager::QP_InsertShapeStatLevel(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_SHAPE_STATLEVEL *pQParam = (QPARAM_SHAPE_STATLEVEL*)q.pQueryParam;
	CFieldIOCPSocket *pFieldIOCPSocket	= q.pFieldIOCPSocket;

	if( NULL == pFieldIOCPSocket || FALSE == pFieldIOCPSocket->IsValidCharacter(FALSE) )
	{
		util::del(pQParam);
		return;
	}

	/************************************************************************
	--------------------------------------------------------------------------------
		-- CREATE PROCEDURE : [dbo].[atum_Insert_ShapeStatLevel]
	--------------------------------------------------------------------------------
		CREATE PROC [dbo].[atum_Insert_ShapeStatLevel]
		@i_ItemKind			TINYINT,
		@i_ItemUID			BIGINT,
		@i_ShapeItemNum		INT,
		@i_AllStatLevel		INT
		--WITH EXECUTE AS 'proexe'
		AS
		
		BEGIN
		DECLARE	@ResultCnt	INT
		SELECT @ResultCnt = COUNT(*) FROM [dbo].[td_ShapeStatLevel]  WITH(NOLOCK) WHERE [ItemUID] = @i_ItemUID
		
		IF( @ResultCnt != 0 )
			BEGIN
				UPDATE [dbo].[td_ShapeStatLevel]  SET [ShapeItemNum] = @i_ShapeItemNum, [AllStatLevel] = @i_AllStatLevel
				WHERE  [ItemUID] = @i_ItemUID 
			END
		ELSE
			BEGIN
				INSERT INTO [dbo].[td_ShapeStatLevel] ([ItemKind],[ItemUID],[ShapeItemNum],[AllStatLevel]) 
				VALUES (@i_ItemKind, @i_ItemUID, @i_ShapeItemNum,@i_AllStatLevel)
			END
		END
	GO
	************************************************************************/

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQParam->ItemKind, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &pQParam->ItemUID, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,	&pQParam->nStatShapeItemNum, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,	&pQParam->nStatLevel, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_130531_0004, SQL_NTS);

	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) 
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"QP_InsertShapeStatLevel Failed !!\n", TRUE, q.pFieldIOCPSocket, q);
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQParam);
}
// end 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - CAtumFieldDBManager::QP_ �Լ� ������

void CAtumFieldDBManager::QP_DeleteFixedTerm(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_FIXED_TERM_SHAPE *pQParam = (QPARAM_FIXED_TERM_SHAPE*)q.pQueryParam;
	CFieldIOCPSocket *pFieldIOCPSocket	= q.pFieldIOCPSocket;

	if( NULL == pFieldIOCPSocket || FALSE == pFieldIOCPSocket->IsValidCharacter(FALSE) )
	{
		util::del(pQParam);

		return;
	}

	SQLINTEGER cb = SQL_NTS;

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME : [atum_Delete_FixedTerm]
	--					// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)
	--------------------------------------------------------------------------------
	CREATE PROC [dbo].[atum_Delete_FixedTerm]
		@i_ItemUID			BIGINT,
		@i_TermType			SMALLINT
	AS
	-- YD ���� �ڵ�
	BEGIN
		DELETE FROM [dbo].[td_FixedTerm] WHERE [ItemUID] = @i_ItemUID and [TermType] = @i_TermType
	END
	GO
	************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &pQParam->ItemUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pQParam->TermType, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_121010_0003, SQL_NTS);

	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) 
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"QP_DeleteFixedTerm Failed !!\n", TRUE, q.pFieldIOCPSocket, q);
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQParam);
}
// end 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)

// 2012-10-21 by jhseol, �Ʒ��� ���׼��� - �Ʒ��� ����� �ɸ��� ���� ����.
void CAtumFieldDBManager::QP_ArenaCharacterReset(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_ARENA_CHARACTER_RESET *pQParam = (QPARAM_ARENA_CHARACTER_RESET*)q.pQueryParam;

	SQLINTEGER arrCB[6] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	INT PLAYTYPE	= ARENA_PLAY_TYPE_NO;
	char SourceCharacterType[SIZE_MAX_CHARACTER_NAME] = {""};
	SQLBindParameter(hstmt, 1,SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQParam->ArenaCharacterUID, 0, &arrCB[1]);
	SQLBindParameter(hstmt, 2,SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQParam->CharacterUID, 0, &arrCB[2]);
	SQLBindParameter(hstmt, 3,SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQParam->AccountUID, 0, &arrCB[3]);
	SQLBindParameter(hstmt, 4,SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &PLAYTYPE, 0, &arrCB[4]);
	SQLBindParameter(hstmt, 5,SQL_PARAM_INPUT, SQL_C_CHAR,  SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, SourceCharacterType, 0, &arrCB[5]);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0175, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::QP_ArenaCharacterReset, arena_CopyDBInfo Fail!! ArenaCharacterUID = %d, CharacterUID = %d, AccountUID = %d\r\n", pQParam->ArenaCharacterUID, pQParam->CharacterUID, pQParam->AccountUID);
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	// 2012-12-18 by jhseol, �Ʒ��� ĳ���� ���� �� ĳ���� ���� �ٽ� �ε��ϱ�.
	INIT_MSG_WITH_BUFFER(MSG_FtoA_ARENA_CHARACTER_RESET_RELOAD, T_FtoA_ARENA_CHARACTER_RESET_RELOAD, pSendMsg, SendBuf);
	pSendMsg->AccountUID			= pQParam->AccountUID;
	pSendMsg->ArenaCharacterUID		= pQParam->ArenaCharacterUID;
	m_pFieldIOCP8->m_pArenaFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FtoA_ARENA_CHARACTER_RESET_RELOAD));
	// end 2012-12-18 by jhseol, �Ʒ��� ĳ���� ���� �� ĳ���� ���� �ٽ� �ε��ϱ�.

	util::del(pQParam);
}
// end 2012-10-21 by jhseol, �Ʒ��� ���׼��� - �Ʒ��� ����� �ɸ��� ���� ����.

///////////////////////////////////////////////////////////////////////////////
// 2012-11-13 by jhseol, ���� �ý��� ������ - ������
void CAtumFieldDBManager::QP_LoadRenewalStrategyPointSummonInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	server::log(TRUE, "  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Load & Init Step.1 - QP_LoadRenewalStrategyPointSummonInfo Call \r\n");
	m_functionLoadRenewalStrategyPointSummonTimeByDB(hstmt);
	m_functionLoadRenewalStrategyPointSummonMapInfoByDB(hstmt);
	m_functionLoadRenewalStrategyPointSummonSettingInfoByDB(hstmt);
}

void CAtumFieldDBManager::m_functionLoadRenewalStrategyPointSummonTimeByDB(SQLHSTMT hstmt)
{
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: atum_LoadRenewalStrategyPointSummonTime
	-- DESC      : ���Ϻ� �ð� �� ���� ���� �������� ���ν���
	-- // 2012-11-13 by jhseol, ���� �ý��� ������
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_LoadRenewalStrategyPointSummonTime
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
	AS
		SELECT DayOfWeek, StartTime, EndTime, CountBCU, CountANI FROM td_RenewalStrategyPointSummonTime WITH(NOLOCK)
	GO
	**************************************************************************/
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_121113_0001, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		server::log(TRUE, "  [ERROR] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Load & Init Step.2, atum_LoadRenewalStrategyPointSummonTime \r\n");
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	SQLINTEGER	arrCB[6] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	SRENEWAL_STRATEGYPOINT_SUMMON_TIME	tmStrategyPointNotSummonTime;
	vectSRENEWAL_STRATEGYPOINT_SUMMON_TIME	tmvectStrategyPointNotSummonTime;
	util::zero(&tmStrategyPointNotSummonTime, sizeof(SRENEWAL_STRATEGYPOINT_SUMMON_TIME));
	tmvectStrategyPointNotSummonTime.clear();
	SQL_TIMESTAMP_STRUCT	tmSTime;
	SQL_TIMESTAMP_STRUCT	tmETime;

	SQLBindCol(hstmt, 1, SQL_C_TINYINT, &tmStrategyPointNotSummonTime.DayOfWeek, 0, &arrCB[1]);
	SQLBindCol(hstmt, 2, SQL_C_TIMESTAMP, &tmSTime, 0, &arrCB[2]);
	SQLBindCol(hstmt, 3, SQL_C_TIMESTAMP, &tmETime, 0, &arrCB[3]);
	SQLBindCol(hstmt, 4, SQL_C_TINYINT, &tmStrategyPointNotSummonTime.CountBCU, 0, &arrCB[4]);
	SQLBindCol(hstmt, 5, SQL_C_TINYINT, &tmStrategyPointNotSummonTime.CountANI, 0, &arrCB[5]);
	
	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA)
	{
		tmStrategyPointNotSummonTime.StartTime	=	tmSTime;
		tmStrategyPointNotSummonTime.EndTime	=	tmETime;

		///////////////////////////////////////////////////////////////////////////////
		// 2008-08-21 by cmkwon, �ð��� ��������� �⺻ ��¥�� ���� ��¥�� ������.
		ATUM_DATE_TIME atCur { true };
		tmStrategyPointNotSummonTime.StartTime.Year		= tmStrategyPointNotSummonTime.EndTime.Year		= atCur.Year;
		tmStrategyPointNotSummonTime.StartTime.Month	= tmStrategyPointNotSummonTime.EndTime.Month	= atCur.Month;
		tmStrategyPointNotSummonTime.StartTime.Day		= tmStrategyPointNotSummonTime.EndTime.Day		= atCur.Day;
		tmvectStrategyPointNotSummonTime.push_back(tmStrategyPointNotSummonTime);
		util::zero(&tmStrategyPointNotSummonTime, sizeof(SRENEWAL_STRATEGYPOINT_SUMMON_TIME));
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	m_pFieldIOCP8->m_InflWarManager.InitRenewalStrategyPointSummonTime(&tmvectStrategyPointNotSummonTime);
	server::log(TRUE, "  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Load & Init Step.2 - Day of the week StrategyPoint SummonTime Loading Success \r\n");
}

void CAtumFieldDBManager::m_functionLoadRenewalStrategyPointSummonMapInfoByDB(SQLHSTMT hstmt)
{
	/*[Stored Query Definition]************************************************
	=============================================
	-- Author:		atum_LoadStrategyPointMapInfluenceInfo
	-- Create date: // 2012-11-13 by jhseol, ���� �ý��� ������ - ������ �� �߰�
	-- Description:	�������� �������� ��������
	-- =============================================
	CREATE PROCEDURE atum_LoadStrategyPointMapInfluenceInfo
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
	AS
	BEGIN
		SELECT [MapIndex], [MapName], [MapInfluenceType]
		FROM [atum2_db_account].[dbo].[ti_MapInfo]
		WHERE [MapIndex] IN (SELECT [MapIndex] FROM [atum2_db_account].[dbo].[ti_StrategyPointSummonMapIndex])
	END
	GO
	**************************************************************************/

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_121113_0004, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		server::log(TRUE, "  [ERROR] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Load & Init Step.3, atum_LoadStrategyPointMapInfluenceInfo \r\n");
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	SQLINTEGER	arrCB[4] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	SSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO	tmStrategyPointMapInfluenceInfo;
	vectSSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO	tmvectStrategyPointMapInfluenceInfo;
	util::zero(&tmStrategyPointMapInfluenceInfo, sizeof(SSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO));
	tmvectStrategyPointMapInfluenceInfo.clear();
	
	SQLBindCol(hstmt, 1, SQL_C_LONG, &tmStrategyPointMapInfluenceInfo.MapIndex, 0, &arrCB[1]);
	SQLBindCol(hstmt, 2, SQL_C_CHAR, tmStrategyPointMapInfluenceInfo.MapName, SIZE_MAX_MAP_NAME, &arrCB[2]);
	SQLBindCol(hstmt, 3, SQL_C_SHORT, &tmStrategyPointMapInfluenceInfo.Influence, 0, &arrCB[3]);
	
	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA)
	{
		tmvectStrategyPointMapInfluenceInfo.push_back(tmStrategyPointMapInfluenceInfo);
		util::zero(&tmStrategyPointMapInfluenceInfo, sizeof(SSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO));
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	m_pFieldIOCP8->m_InflWarManager.InitRenewalStrategyPointSummonMapInfo(&tmvectStrategyPointMapInfluenceInfo);
	server::log(TRUE, "  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Load & Init Step.3 - StrategyPoint MapInfluenceInfo Loading Success \r\n");
}


void CAtumFieldDBManager::m_functionLoadRenewalStrategyPointSummonSettingInfoByDB(SQLHSTMT hstmt)
{
	/************************************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_LoadStrategyPointSummonInfo
	-- DESC      : �����ؾ� �� ��������Ʈ ������ ������ ���ν���.
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_LoadStrategyPointSummonInfo
	AS
		SELECT MapIndex, SummonCount, SummonTime, SummonAttribute FROM dbo.td_StrategyPointSummonInfo WITH (NOLOCK)
	GO
	************************************************************************/	
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0138, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		server::log(TRUE, "  [ERROR] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Load & Init Step.Finish, atum_LoadStrategyPointSummonInfo \r\n");
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
 
	SQLINTEGER	arrCB[5] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	SSTRATEGYPOINT_SUMMONTIME_INFO	tmStrategyPointSummonTimeInfo;
	vectSSTRATEGYPOINT_SUMMONTIME_INFO	tmvectStrategyPointSummonTimeInfo;
	util::zero(&tmStrategyPointSummonTimeInfo, sizeof(SSTRATEGYPOINT_SUMMONTIME_INFO));
	tmvectStrategyPointSummonTimeInfo.clear();
	SQL_TIMESTAMP_STRUCT SummonTime;
	
	SQLBindCol(hstmt, 1, SQL_C_LONG, &tmStrategyPointSummonTimeInfo.MapIndex, 0, &arrCB[1]);
	SQLBindCol(hstmt, 2, SQL_C_UTINYINT, &tmStrategyPointSummonTimeInfo.SummonCount, 0, &arrCB[2]);
	SQLBindCol(hstmt, 3, SQL_C_TIMESTAMP, &SummonTime, 0, &arrCB[3]);
	SQLBindCol(hstmt, 4, SQL_C_UTINYINT, &tmStrategyPointSummonTimeInfo.SummonAttribute, 0, &arrCB[4]);
	
	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA)
	{
		tmStrategyPointSummonTimeInfo.SummonTime	= SummonTime;
		tmvectStrategyPointSummonTimeInfo.push_back(tmStrategyPointSummonTimeInfo);
		util::zero(&tmStrategyPointSummonTimeInfo, sizeof(SSTRATEGYPOINT_SUMMONTIME_INFO));
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	m_pFieldIOCP8->m_InflWarManager.InitStrategyPointSummonTimeInfo(&tmvectStrategyPointSummonTimeInfo);

	server::log(TRUE, "  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Load & Init Step.Finish - Today StrategyPoint SummonInfo Loading Success \r\n");
}

void CAtumFieldDBManager::QP_UpdateStrategyPointSummonInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_STRATEGYPOINT_SUMMON_INFO *pQParam = (QPARAM_STRATEGYPOINT_SUMMON_INFO*)q.pQueryParam;
	
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: atum_UpdateRenewalStrategyPointSummonInfo
	-- DESC      : ������ ���� ���� ������Ʈ
	-- // 2012-11-13 by jhseol, ���� �ý��� ������
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_UpdateRenewalStrategyPointSummonInfo
	@i_MapIndex			INT,
	@i_SummonCount		TINYINT,
	@i_SummonTime		VARCHAR(30),
	@i_SummonAttribute	TINYINT
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
	AS
		IF IS NULL (SELECT MapIndex FROM dbo.td_StrategyPointSummonInfo WHERE MapIndex = @i_MapIndex)
		BEGIN
			INSERT INTO dbo.td_StrategyPointSummonInfo(MapIndex, SummonCount, SummonTime, SummonAttribute)
			VALUES(@i_MapIndex, '19000101 00:00:00.000', 0, 0)
		END

		IF (@i_SummonTime like '0000%')
		BEGIN
			UPDATE dbo.td_StrategyPointSummonInfo SET SummonCount = @i_SummonCount, SummonTime = '19000101 00:00:00.000', SummonAttribute = @i_SummonAttribute WHERE MapIndex = @i_MapIndex
		END
		ELSE
		BEGIN
			UPDATE dbo.td_StrategyPointSummonInfo SET SummonCount = @i_SummonCount, SummonTime = @i_SummonTime, SummonAttribute = @i_SummonAttribute WHERE MapIndex = @i_MapIndex
		END
	GO
	**************************************************************************/
	SQLINTEGER arrCB[5] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	char tmpSummonTimeBuf[SIZE_MAX_SQL_DATETIME_STRING];
	pQParam->SummonTime.GetSQLDateTimeString(tmpSummonTimeBuf, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT,	0, 0, &pQParam->MapIndex, 0, &arrCB[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQParam->SummonCount, 0, &arrCB[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpSummonTimeBuf, 0, &arrCB[3]);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pQParam->SummonAttribute, 0, &arrCB[4]);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_121113_0003, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::QP_UpdateStrategyPointSummonInfo (MapIndex:%d, SummonCount:%d, SummonTime:%s, SummonAttribute:%d)\r\n",
			pQParam->MapIndex, pQParam->SummonCount, tmpSummonTimeBuf, pQParam->SummonAttribute);
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQParam);
}
// end 2012-11-13 by jhseol, ���� �ý��� ������ - ������

// 2012-12-18 by jhseol, �Ʒ��� ĳ���� ���� �� ĳ���� ���� �ٽ� �ε��ϱ�.
void CAtumFieldDBManager::QP_ArenaCharacterReload(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	CFieldIOCPSocket *pFISock	= q.pFieldIOCPSocket;
	if (NULL == pFISock)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::QP_ArenaCharacterReload, Socket error !! FISock(0x%X) \r\n", pFISock);
		return;
	}
	if( TRUE == m_functionArenaCharacterLoad(pFISock, hstmt) )
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] CAtumFieldDBManager::QP_ArenaCharacterReload, Success !! AUID(%d) CUID(%d)\r\n", pFISock->m_character.AccountUniqueNumber, pFISock->m_character.CharacterUniqueNumber);
	}
	else
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::QP_ArenaCharacterReload, Fail !! AUID(%d) CUID(%d)\r\n", pFISock->m_character.AccountUniqueNumber, pFISock->m_character.CharacterUniqueNumber);
	}
}

BOOL CAtumFieldDBManager::m_functionArenaCharacterLoad(CFieldIOCPSocket *i_pFISock, SQLHSTMT hstmt)
{
	// �Ʒ��� �������� �ɸ��� ������ �о���� �Լ�.
	if( NULL == i_pFISock )
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::m_functionArenaCharacterLoad, i_pFISock is NULL!!\r\n");
		return FALSE;
	}
	CHARACTER retCharacter;
	memset(&retCharacter, 0x00, sizeof(CHARACTER));
	CHARACTER_DB_EX retCharacterDBEX;

	///////////////////////////////////////////////////////////////////////////////////////////////////
	// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : �Ʒ��� �÷��� ���� ������Ʈ
	SArenaPlayInfo tempArenaInfo;
	util::zero(&tempArenaInfo, sizeof(SArenaPlayInfo));
	tempArenaInfo = i_pFISock->m_character.ArenaPlayInfo;			// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : �Ŀ� �ɸ��� ���� ���� ���� ���� �ʿ��� ���� ���
	// end 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : �Ʒ��� �÷��� ���� ������Ʈ
	
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE dbo.arena_GetCharacter
		@acc	INT,
		@charac	INT
	AS
		SELECT *
		FROM td_Character
		WITH (NOLOCK)
		WHERE UniqueNumber=@charac AND AccountUniqueNumber = @acc
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,	&i_pFISock->m_character.AccountUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,	&i_pFISock->m_character.CharacterUniqueNumber, 0, NULL);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0174, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_NO_DATA)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::m_functionArenaCharacterLoad, arena_GetCharacter Call Fail!! AUID=%d, CUID=%d\r\n", i_pFISock->m_character.AccountUniqueNumber, i_pFISock->m_character.CharacterUniqueNumber);
		i_pFISock->SendErrorMessage(T_FC_ARENA_POSSIBLE_GAMESTART, ERR_COMMON_NO_SUCH_CHARACTER);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return FALSE;
	}

	SQLINTEGER	cb[CB_COUNT_CHARACTER];
	fill_n(cb, CB_COUNT_CHARACTER, SQL_NTS);

	// bind columns
	CAtumDBManager::BindColCharacter(hstmt, retCharacter, retCharacterDBEX, cb);

	// initialize to 0x00
	memset(&retCharacter, 0x00, sizeof(CHARACTER));
	ret = SQLFetch(hstmt);
	SQLFreeStmt(hstmt, SQL_CLOSE);
	retCharacter				= retCharacterDBEX;
	_strlwr(retCharacter.AccountName);
	retCharacter.TotalGearStat	= retCharacter.GearStat;

	// 2012-10-17 by jhseol, ���� �̸����� ������ ON ��Ű��
#ifdef S_ITEM_VIEW_UNCONDITIONAL_ON
	retCharacter.SecretInfoOption = retCharacter.SecretInfoOption & USER_INFO_OPTION_ITEMINFO_DELETE_ALL_MASK;
#endif
	// end 2012-10-17 by jhseol, ���� �̸����� ������ ON ��Ű��

	if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		///////////////////////////////////////////////////////////////////////////
		// ĳ���Ͱ� ����, Error ó��
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::m_functionArenaCharacterLoad, No Character Data(): A: %d, CH: %d\r\n", i_pFISock->m_character.AccountUniqueNumber, i_pFISock->m_character.CharacterUniqueNumber);
		i_pFISock->SendErrorMessage(T_FC_ARENA_POSSIBLE_GAMESTART, ERR_COMMON_NO_SUCH_CHARACTER);
		return FALSE;
	}

	util::zero(&i_pFISock->m_character, sizeof(CHARACTER));			// Character ����ü�� �ʱ�ȭ�Ѵ�.
	memcpy(&i_pFISock->m_character, &retCharacter, sizeof(CHARACTER));		// �Ʒ������� ����� Character
	i_pFISock->m_character.ClientIndex = i_pFISock->GetClientArrayIndex();	// Character ����ü�� �迭 �ε����� �����Ѵ�.

	memcpy(&i_pFISock->m_character.ArenaPlayInfo, &tempArenaInfo, sizeof(SArenaPlayInfo));	/// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : �Ŀ� �ɸ��� ���� ���� ���� ���� �ʿ��� ���� ����

	return TRUE;
}
// end 2012-12-18 by jhseol, �Ʒ��� ĳ���� ���� �� ĳ���� ���� �ٽ� �ε��ϱ�.


// 2013-03-29 by jhseol, ������ �̺�Ʈ - �ſ� ù ����� ��������
void CAtumFieldDBManager::QP_LoadCashBuyDate(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	CFieldIOCPSocket *pFISock	= q.pFieldIOCPSocket;
	if (NULL == pFISock)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[S_MONTHL_CASH_BUY_GIFT_JHSEOL][ERROR] CAtumFieldDBManager::QP_LoadCashBuyDate, Socket is NULL CUID(%d)\r\n", q.nCharacterUID);
		return;
	}

	SQLINTEGER arrCB[2] = { SQL_NTS, SQL_NTS };

	/********************************************************************************
	CREATE PROCEDURE dbo.atum_GetCashBuyDateByAccountUID
		@i_AccountUID		INT
		--WITH EXECUTE AS 'proexe'
	AS
	BEGIN
		SELECT TOP 1 LastCashBuyDate
		FROM td_CashBuyDate WITH (NOLOCK)
		WHERE AccountUID = @i_AccountUID order by LastCashBuyDate desc
	END
	********************************************************************************/

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER,		0, 0, &pFISock->m_character.AccountUniqueNumber,		0, &arrCB[1]);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_130329_0001, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"[S_MONTHL_CASH_BUY_GIFT_JHSEOL]atum_GetCashBuyDateByAccountUID Failed!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	SQL_TIMESTAMP_STRUCT	LastCashBuyDate;
	util::zero(&LastCashBuyDate, sizeof(LastCashBuyDate));
	SQLBindCol(hstmt, 1, SQL_C_TIMESTAMP, &LastCashBuyDate, 0, &arrCB[1]);
	if ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA)
	{
		pFISock->m_atAccountLastBuyDate = LastCashBuyDate;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);


	/********************************************************************************
	CREATE PROCEDURE dbo.atum_GetCashBuyDateByCharacterUID
		@i_CharacterUID		INT
		--WITH EXECUTE AS 'proexe'
	AS
	BEGIN
		SELECT LastCashBuyDate
		FROM td_CashBuyDate WITH (NOLOCK)
		WHERE CharacterUID = @i_CharacterUID
	END
	********************************************************************************/

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER,		0, 0, &pFISock->m_character.CharacterUniqueNumber,		0, &arrCB[1]);
	ret = SQLExecDirect(hstmt, PROCEDURE_130329_0002, SQL_NTS);
	
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"[S_MONTHL_CASH_BUY_GIFT_JHSEOL]atum_GetCashBuyDateByCharacterUID Failed!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	
	util::zero(&LastCashBuyDate, sizeof(LastCashBuyDate));
	SQLBindCol(hstmt, 1, SQL_C_TIMESTAMP, &LastCashBuyDate, 0, &arrCB[1]);
	if ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA)
	{
		pFISock->m_atCharacterLastBuyDate = LastCashBuyDate;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	char tmpAccount[SIZE_MAX_SQL_DATETIME_STRING];
	pFISock->m_atAccountLastBuyDate.GetSQLDateTimeString(tmpAccount, SIZE_MAX_SQL_DATETIME_STRING);
	char tmpCharacter[SIZE_MAX_SQL_DATETIME_STRING];
	pFISock->m_atCharacterLastBuyDate.GetSQLDateTimeString(tmpCharacter, SIZE_MAX_SQL_DATETIME_STRING);
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[S_MONTHL_CASH_BUY_GIFT_JHSEOL][Notify] CAtumFieldDBManager::QP_LoadCashBuyDate,  AUID(%8d) CUID(%8d) A_Date(%s) C_Date(%s)\r\n"
		, pFISock->m_character.AccountUniqueNumber, pFISock->m_character.CharacterUniqueNumber, tmpAccount, tmpCharacter);
}

void CAtumFieldDBManager::QP_InsertCashBuyDate(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	SQLINTEGER arrCB[4] = { SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS };

	CFieldIOCPSocket *pFISock	= q.pFieldIOCPSocket;
	if (NULL == pFISock)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[S_MONTHL_CASH_BUY_GIFT_JHSEOL][ERROR] CAtumFieldDBManager::QP_InsertCashBuyDate, Socket is NULL CUID(%d)\r\n", q.nCharacterUID);
		return;
	}

	/********************************************************************************
	CREATE PROCEDURE dbo.atum_InsertCashBuyDate
		@i_AccountUID		INT,
		@i_CharacterUID		INT,
		@i_LastCashBuyDate	VARCHAR(30)
		--WITH EXECUTE AS 'proexe'
	AS
	BEGIN
		IF EXISTS(SELECT * FROM td_CashBuyDate WITH (NOLOCK) WHERE AccountUID = @i_AccountUID AND CharacterUID = @i_CharacterUID)
		BEGIN
			UPDATE td_CashBuyDate
			SET LastCashBuyDate = CONVERT(DATETIME, @i_LastCashBuyDate)
			WHERE AccountUID = @i_AccountUID AND CharacterUID = @i_CharacterUID
		END
		ELSE
		BEGIN
			INSERT INTO td_CashBuyDate(AccountUID, CharacterUID, LastCashBuyDate)
			VALUES(@i_AccountUID, @i_CharacterUID, CONVERT(DATETIME, @i_LastCashBuyDate))
		END
	END
	********************************************************************************/
	char tmpCurrent[SIZE_MAX_SQL_DATETIME_STRING] = { 0, };
	pFISock->m_atCharacterLastBuyDate.GetSQLDateTimeString(tmpCurrent, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pFISock->m_character.AccountUniqueNumber, 0, &arrCB[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pFISock->m_character.CharacterUniqueNumber, 0, &arrCB[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpCurrent, 0, &arrCB[3]);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_130329_0003, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA) {
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"[S_MONTHL_CASH_BUY_GIFT_JHSEOL]atum_InsertCashBuyDate Failed!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[S_MONTHL_CASH_BUY_GIFT_JHSEOL][Notify] CAtumFieldDBManager::QP_InsertCashBuyDate, AUID(%8d) CUID(%8d) LastBuyDate(%s)\r\n"
		, pFISock->m_character.AccountUniqueNumber, pFISock->m_character.CharacterUniqueNumber, tmpCurrent);
	SQLFreeStmt(hstmt, SQL_CLOSE);
}
// end 2013-03-29 by jhseol, ������ �̺�Ʈ - �ſ� ù ����� ��������

// 2013-04-18 by jhseol,bckim �̴��� �Ƹ� - �̴��� �Ƹ� �̺�Ʈ �ε�
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumFieldDBManager::QP_LoadMonthlyArmorEvent(FIELD_DB_QUERY q, SQLHSTMT hstmt)
/// \brief		
/// \author		jhseol
/// \date		2003-05-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumFieldDBManager::QP_LoadMonthlyArmorEvent(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_130503_0002_2, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_GetMonthlyArmorEventByServer Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	SQLINTEGER cb[6]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS	,SQL_NTS};

	vectMONTHLY_ARMOR_EVNET_INFO	MonthlyArmorEventList;
	MonthlyArmorEventList.clear();
	MONTHLY_ARMOR_EVNET_INFO	MonthlyArmorEventInfo;
	util::zero(&MonthlyArmorEventInfo, sizeof(MONTHLY_ARMOR_EVNET_INFO));
	ATUM_DATE_TIME		CurTime { true };
	SQL_TIMESTAMP_STRUCT   StartTime0;
	SQL_TIMESTAMP_STRUCT   EndTime0;

	SQLBindCol(hstmt, 1, SQL_C_ULONG, &MonthlyArmorEventInfo.nEventUID, 0,			&cb[1]);
	SQLBindCol(hstmt, 2, SQL_C_TIMESTAMP, &StartTime0, 0,							&cb[2]);
	SQLBindCol(hstmt, 3, SQL_C_TIMESTAMP, &EndTime0, 0,								&cb[3]);
	SQLBindCol(hstmt, 4, SQL_C_ULONG, &MonthlyArmorEventInfo.nArmorSourceIndex, 0,	&cb[4]);
	SQLBindCol(hstmt, 5, SQL_C_ULONG, &MonthlyArmorEventInfo.nOptionItemNum, 0,		&cb[5]);


	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA)
	{
		MonthlyArmorEventInfo.atStartDate	= StartTime0;
		MonthlyArmorEventInfo.atEndDate		= EndTime0;
		if ( (MonthlyArmorEventInfo.atEndDate) < CurTime)	// �̺�Ʈ ���� �ð� üũ
		{
			util::zero(&MonthlyArmorEventInfo, sizeof(MONTHLY_ARMOR_EVNET_INFO));
			continue;
		}
		ITEM *pItemInfo = m_pFieldIOCP8->GetItemInfo(MonthlyArmorEventInfo.nOptionItemNum);
		if(NULL == pItemInfo)	// ������ ��ȿ�� üũ
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumFieldDBManager::QP_LoadMonthlyArmorEvent Option ItemNum error !!, EventUID(%d) ItemNum(%8d)\r\n", MonthlyArmorEventInfo.nEventUID, MonthlyArmorEventInfo.nOptionItemNum);
			util::zero(&MonthlyArmorEventInfo, sizeof(MONTHLY_ARMOR_EVNET_INFO));
			continue;
		}
		
		MonthlyArmorEventList.push_back(MonthlyArmorEventInfo);			
		util::zero(&MonthlyArmorEventInfo, sizeof(MONTHLY_ARMOR_EVNET_INFO));
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	
	if ( NULL != m_pFieldIOCP8->m_pMonthlyArmorManager )
	{
		m_pFieldIOCP8->m_pMonthlyArmorManager->InitMonthlyArmorEventManager(&MonthlyArmorEventList);
	}
}
// end 2013-04-18 by jhseol,bckim �̴��� �Ƹ� - �̴��� �Ƹ� �̺�Ʈ �ε�

// 2013-05-20 by hskim, [���� �ý���] ���������� ������� ������ �߰� ����
void CAtumFieldDBManager::QP_GetTemporarySystemInfomation(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_TEMPORARY_SYSTEM_INFOMATION *pQParam = (QPARAM_TEMPORARY_SYSTEM_INFOMATION*)q.pQueryParam;

	RETCODE ret;

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME : [atum_GetTemporarySystemInfomation]
	--					// 2013-05-20 by hskim, [���� �ý���] ���������� ������� ������ �߰� ����
	--------------------------------------------------------------------------------
	ALTER PROCEDURE [dbo].[atum_GetTemporarySystemInfomation]
		@i_Type		INT
	-- YD ���� �ڵ�
	AS
	BEGIN
		SELECT [UID], [CreateDate], [Data1], [Data2], [Data3], [Data4] FROM [dbo].[atum_Temporary_SystemInfomation] WITH (NOLOCK) WHERE [Type] = @i_Type
	END
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQParam->Type, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_130520_0003, SQL_NTS);

	if(ret == SQL_NO_DATA)
	{
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQParam);
		return;
	}

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"QP_GetTemporarySystemInfomation Failed!!\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQParam);
		return;
	}

	SQLINTEGER	arrCB[7] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS };
	SQL_TIMESTAMP_STRUCT Date;
	util::zero(&Date, sizeof(SQL_TIMESTAMP_STRUCT));
	
	SQLBindCol(hstmt, 1, SQL_C_UBIGINT,		&pQParam->UID,		0, &arrCB[1]);
	SQLBindCol(hstmt, 2, SQL_C_TIMESTAMP,	&pQParam->Date,		0, &arrCB[2]);
	SQLBindCol(hstmt, 3, SQL_C_UBIGINT,		&pQParam->Data1,	0, &arrCB[3]);
	SQLBindCol(hstmt, 4, SQL_C_LONG,		&pQParam->Data2,	0, &arrCB[4]);
	SQLBindCol(hstmt, 5, SQL_C_CHAR,		pQParam->Data3,		SIZE_MAX_TEMP_SYSTEM_STRING, &arrCB[5]);
	SQLBindCol(hstmt, 6, SQL_C_CHAR,		pQParam->Data4,		SIZE_MAX_TEMP_SYSTEM_STRING, &arrCB[6]);
	
	if( (ret = SQLFetch(hstmt)) == SQL_NO_DATA )
	{
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQParam);
		return;		
	}

	////////////////////////////////////////////////////////////////////////
	if( TEMPORARY_SYSTEM_INFOMATION_INDEX_ITEMSECURITY == pQParam->Type )
	{
		m_pFieldIOCP8->SetItemSecurityMaximunUID(pQParam->Data1);
	}
	////////////////////////////////////////////////////////////////////////

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQParam);
}

void CAtumFieldDBManager::QP_GetStoreExtension(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_GET_STORE_EXTENSION *pQParam = (QPARAM_GET_STORE_EXTENSION*)q.pQueryParam;
	CFieldIOCPSocket *pFieldIOCPSocket	= q.pFieldIOCPSocket;

	if( NULL == pFieldIOCPSocket )
	{
		//util::del(pQParam);		<- pQParam �������� �Ҵ����� �ʾ���, �ش� �κ� ������, �ּ� �츮�� ����, ����� ��~~~~
		return;
	}

	RETCODE ret;
	SDWORD pcbNTS = SQL_NTS;

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME : [atum_GetStoreExtension]
	--					// 2013-05-20 by hskim, [���� �ý���] ���������� ������� ������ �߰� ����
	--------------------------------------------------------------------------------
	ALTER PROCEDURE [dbo].[atum_GetStoreExtension]
		@i_UniqueNumber		BIGINT
	-- YD ���� �ڵ�
	AS
	BEGIN
		SELECT [Gesture] FROM [dbo].[td_StoreExtension] WITH (NOLOCK) WHERE [UniqueNumber] = @i_UniqueNumber
	END
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &pQParam->ItemUID, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_130520_0002, SQL_NTS);

	if(ret == SQL_NO_DATA)
	{
		SQLFreeStmt(hstmt, SQL_CLOSE);
		//util::del(pQParam);		<- pQParam �������� �Ҵ����� �ʾ���, �ش� �κ� ������, �ּ� �츮�� ����, ����� ��~~~~
		return;
	}

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"QP_GetStoreExtension Failed!!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		//util::del(pQParam);		<- pQParam �������� �Ҵ����� �ʾ���, �ش� �κ� ������, �ּ� �츮�� ����, ����� ��~~~~
		return;
	}

	SQLINTEGER	arrCB[2] = {SQL_NTS, SQL_NTS };
	SQLBindCol(hstmt, 1, SQL_C_UBIGINT, &pQParam->Gesture, 0, &arrCB[1]);
	
	if( (ret = SQLFetch(hstmt)) == SQL_NO_DATA )
	{
		SQLFreeStmt(hstmt, SQL_CLOSE);
		//util::del(pQParam);		<- pQParam �������� �Ҵ����� �ʾ���, �ش� �κ� ������, �ּ� �츮�� ����, ����� ��~~~~
		return;		
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	//util::del(pQParam);		<- pQParam �������� �Ҵ����� �ʾ���, �ش� �κ� ������, �ּ� �츮�� ����, ����� ��~~~~
}

void CAtumFieldDBManager::QP_InsertStoreExtension(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_INSERT_STORE_EXTENSION *pQParam = (QPARAM_INSERT_STORE_EXTENSION*)q.pQueryParam;
	CFieldIOCPSocket *pFieldIOCPSocket	= q.pFieldIOCPSocket;

	if( NULL == pFieldIOCPSocket )
	{
		util::del(pQParam);
		return;
	}

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME : [atum_InsertStoreExtension]
	--					// 2013-05-20 by hskim, [���� �ý���] ���������� ������� ������ �߰� ����
	--------------------------------------------------------------------------------
	CREATE PROCEDURE [dbo].[atum_InsertStoreExtension]
		@i_UniqueNumber		BIGINT,
		@i_Gesture			BIGINT
	-- YD ���� �ڵ�
	AS
	BEGIN
		IF EXISTS(SELECT * FROM [td_StoreExtension] WITH (NOLOCK) WHERE [UniqueNumber] = @i_UniqueNumber)
		BEGIN
			-- �߰�
			INSERT INTO [dbo].[td_StoreExtension] ([UniqueNumber], [Gesture]) VALUES (@i_UniqueNumber, @i_Gesture)
		END
		ELSE
		BEGIN
			-- ����
			UPDATE [dbo].[td_StoreExtension] SET [Gesture] = @i_Gesture WHERE [UniqueNumber] = @i_UniqueNumber
		END
	END
	GO
	************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &pQParam->ItemUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &pQParam->Gesture, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_130520_0001, SQL_NTS);

	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) 
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"QP_InsertStoreExtension Failed !!\n", TRUE, q.pFieldIOCPSocket, q);
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQParam);
}
// end 2013-05-20 by hskim, [���� �ý���] ���������� ������� ������ �߰� ����

// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - CAtumFieldDBManager::QP_ �Լ� ������
void CAtumFieldDBManager::QP_CollectionArmorListLoad(FIELD_DB_QUERY q, SQLHSTMT hstmt)			// - �Ƹ� �÷��� �ε�
{
	CFieldIOCPSocket *pFieldIOCPSocket	= q.pFieldIOCPSocket;
	int nType = (int)q.nGeneralParam1;

	if(NULL == pFieldIOCPSocket
		|| FALSE == pFieldIOCPSocket->IsValidCharacter(FALSE))
	{
		return;
	}

	UID32_t nAccountUID = pFieldIOCPSocket->m_character.AccountUniqueNumber;
	UID32_t nCharacterUID = 0;
	switch(nType)
	{
	case 1:
		nCharacterUID = 0;
		break;
	default:
		nCharacterUID = pFieldIOCPSocket->m_character.CharacterUniqueNumber;
		break;
	}

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nType, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nAccountUID, 0, NULL);

    SQLRETURN	ret = SQLExecDirect(hstmt, PROCEDURE_130531_0001, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"QP_CollectionArmorListLoad Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	SQL_TIMESTAMP_STRUCT tmCreatedTimeStamp;
	SQLINTEGER		arrCB[8] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS};

	if( SQL_NO_DATA != ret )
	{

		COLLECTION_INFO tmpCollectionInfo;

		SQLBindCol(hstmt, 1, SQL_C_SLONG, &tmpCollectionInfo.CollectionType	, 0,&arrCB[1]);
		SQLBindCol(hstmt, 2, SQL_C_SLONG, &tmpCollectionInfo.AccountUID		, 0,&arrCB[2]);
		SQLBindCol(hstmt, 3, SQL_C_SLONG, &tmpCollectionInfo.CharacterUID	, 0,&arrCB[3]);
		SQLBindCol(hstmt, 4, SQL_C_SLONG, &tmpCollectionInfo.ShapeNum		, 0,&arrCB[4]);
		SQLBindCol(hstmt, 5, SQL_C_SLONG, &tmpCollectionInfo.ShapeItemNum	, 0,&arrCB[5]);
		SQLBindCol(hstmt, 6, SQL_C_UTINYINT, &tmpCollectionInfo.EnchantLevel	, 0,&arrCB[6]);
		SQLBindCol(hstmt, 7, SQL_C_SLONG, &tmpCollectionInfo.RemainSeconds	, 0,&arrCB[7]);

		while( (ret = SQLFetch(hstmt)) != SQL_NO_DATA )
		{
			if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO )
			{
				ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_GetCollectionList Fetching Failed!\n", TRUE, q.pFieldIOCPSocket, q);
				break;
			}

			if ( 0 == tmpCollectionInfo.CharacterUID || pFieldIOCPSocket->m_character.CharacterUniqueNumber == tmpCollectionInfo.CharacterUID)
			{
				pFieldIOCPSocket->m_ArmorCollection.CollectionAdd(&tmpCollectionInfo,FALSE); // ����Ʈ�� ���, ��� ������Ʈ ����.

				INIT_MSG_WITH_BUFFER(MSG_FC_COLLECTION_INFO, T_FC_COLLECTION_INFO, pCollectionInfo, SendBuf);
				memcpy(pCollectionInfo, &tmpCollectionInfo, sizeof(COLLECTION_INFO));
				pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_COLLECTION_INFO));

				// 2013-04-18 by jhseol,bckim �̴��� �Ƹ� - �ϳ��� �÷��ǿ� ���Ͽ� �̴��� ��� �̺�Ʈ ���� ������
#ifdef S_MONTHL_ARMOR_EVENT_JHSEOL_BCKIM	// - �ϳ��� �÷��ǿ� ���Ͽ� �̴��� ��� �̺�Ʈ ���� ������
				if ( NULL != m_pFieldIOCP8 )
				{
					m_pFieldIOCP8->m_pMonthlyArmorManager->CheckMonthlyArmorEventCollection(pFieldIOCPSocket, tmpCollectionInfo.ShapeNum);
				}
				
#endif
				// end 2013-04-18 by jhseol,bckim �̴��� �Ƹ� - �ϳ��� �÷��ǿ� ���Ͽ� �̴��� ��� �̺�Ʈ ���� ������
			}
		}
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
}

void CAtumFieldDBManager::QP_CollectionArmorUpdate(FIELD_DB_QUERY q, SQLHSTMT hstmt)				// - �Ƹ� �÷��� ����
{
	COLLECTION_INFO *tmAddCollectionInfo = (COLLECTION_INFO*)q.pQueryParam;
	CFieldIOCPSocket *pFieldIOCPSocket	= q.pFieldIOCPSocket;
	int nType = (int)q.nGeneralParam1;
	
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmAddCollectionInfo->CollectionType, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmAddCollectionInfo->AccountUID, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmAddCollectionInfo->CharacterUID, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmAddCollectionInfo->ShapeNum, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmAddCollectionInfo->ShapeItemNum, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmAddCollectionInfo->EnchantLevel, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmAddCollectionInfo->RemainSeconds, 0, NULL);
	
    SQLRETURN	ret = SQLExecDirect(hstmt, PROCEDURE_130531_0002, SQL_NTS);
	
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"QP_CollectionArmorUpdate Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(tmAddCollectionInfo);
		return;
	}

	if(NULL == pFieldIOCPSocket
		|| FALSE == pFieldIOCPSocket->IsValidCharacter(FALSE))
		
	{
		util::del(tmAddCollectionInfo);
		return;
	}
	
	INIT_MSG_WITH_BUFFER(MSG_FC_COLLECTION_INFO, T_FC_COLLECTION_INFO, pCollectionInfo, SendBuf);
	memcpy(pCollectionInfo, tmAddCollectionInfo, sizeof(COLLECTION_INFO));
	pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_COLLECTION_INFO));

	util::del(tmAddCollectionInfo);
	return;
}
// end 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - CAtumFieldDBManager::QP_ �Լ� ������

// 2013-07-26 by jhseol, Ÿ ���� ���º���
struct ChangeCharacterInfo
{
	UID32_t		CUID;
	BYTE		CLevel;
	INT64		CTotalPlayTime;
	
	ChangeCharacterInfo()
	{
		util::zero(this, sizeof(ChangeCharacterInfo));
	}
};

void CAtumFieldDBManager::QP_AccountInflChange(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	CFieldIOCPSocket *pFieldIOCPSocket	= q.pFieldIOCPSocket;
	UID32_t UserAccountUID = (UID32_t)q.nGeneralParam1;
	BYTE byChangeInfl = (BYTE)q.nGeneralParam2;
	CHAR UserAccountName[SIZE_MAX_ACCOUNT_NAME];

	if (NULL == pFieldIOCPSocket)
	{
		return;
	}

	SQLINTEGER		arrCB[8] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG,		SQL_INTEGER, 0, 0, &UserAccountUID, 0, NULL);
	SQLRETURN	ret = SQLExecDirect(hstmt, PROCEDURE_130726_0003, SQL_NTS);
	SQLBindCol(hstmt, 1, SQL_C_CHAR, UserAccountName, SIZE_MAX_ACCOUNT_NAME,		&arrCB[7]);
	
	if( (ret = SQLFetch(hstmt)) == SQL_NO_DATA )
	{
		pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_130726_0007);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	mt_auto_lock mtA(&m_pFieldIOCP8->m_mtsetAccountName);
	if(m_pFieldIOCP8->m_mtsetAccountName.end() != m_pFieldIOCP8->m_mtsetAccountName.find(UserAccountName))
	{
		pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_130726_0008);
		return;
	}
	mtA.auto_unlock_cancel();


	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG,		SQL_INTEGER, 0, 0, &UserAccountUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &byChangeInfl, 0, NULL);
	
	ret = SQLExecDirect(hstmt, PROCEDURE_130726_0001, SQL_NTS);
	
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_130726_0005, UserAccountUID, 0, byChangeInfl, 101);
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"QP_AccountInflChange PROCEDURE_130726_0001 Failed!\n", TRUE, q.pFieldIOCPSocket, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	int LoofCount = 0;
	BYTE ErrorCode = 0;
	ChangeCharacterInfo tmCharInfo;
	vector<ChangeCharacterInfo> vectCharInfo;
	vectCharInfo.clear();
	
	SQLBindCol(hstmt, 1, SQL_C_UTINYINT,	&ErrorCode, 0,					&arrCB[1]);
	SQLBindCol(hstmt, 2, SQL_C_ULONG,		&tmCharInfo.CUID, 0,			&arrCB[2]);
	SQLBindCol(hstmt, 3, SQL_C_UTINYINT,	&tmCharInfo.CLevel, 0,			&arrCB[3]);
	SQLBindCol(hstmt, 4, SQL_C_UBIGINT,		&tmCharInfo.CTotalPlayTime, 0,	&arrCB[4]);
	
	while( (ret = SQLFetch(hstmt)) != SQL_NO_DATA )
	{
		if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO )
		{
			pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_130726_0005, UserAccountUID, 0, byChangeInfl, 102);
			ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"atum_CheckInfluenceType_ByAccountUID Fetching Failed!\n", TRUE, q.pFieldIOCPSocket, q);
			SQLFreeStmt(hstmt, SQL_CLOSE);
			return;
		}
		if ( 100 != ErrorCode )
		{
			switch(ErrorCode)
			{
			case 1:	pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_130726_0001);	break;
			case 2:	pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_130726_0002);	break;
			case 3:	pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_130726_0003);	break;
			}
			SQLFreeStmt(hstmt, SQL_CLOSE);
			return;
		}

		LoofCount++;
		vectCharInfo.push_back(tmCharInfo);
	}
	if ( 0 == LoofCount )
	{
		pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_130726_0004);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	vector<ChangeCharacterInfo>::iterator itr = vectCharInfo.begin();
	for (; itr != vectCharInfo.end() ; itr++ )
	{
 		// 1. ���� ������ ĳ������ ����Ʈ ���� �ε�
		mtmapQuestIndex2FieldCharacterQuest	tmmapFCharacterQuest;
		CFieldCharacterQuest				tmFCharQuest;
		CharacterQuest						retQuest;
		util::zero(arrCB, sizeof(arrCB[0])*8);
		SQL_TIMESTAMP_STRUCT				QuestStartTime;
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &itr->CUID, 0, NULL);
		ret = SQLExecDirect(hstmt, PROCEDURE_080822_0114, SQL_NTS);
		if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_130726_0005, UserAccountUID, itr->CUID, byChangeInfl, 103);
			ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"QP_AccountInflChange PROCEDURE_080822_0114 Failed!\n", TRUE, q.pFieldIOCPSocket, q);
			SQLFreeStmt(hstmt, SQL_CLOSE);
			return;
		}
		
		SQLBindCol(hstmt,  1, SQL_C_LONG   , &retQuest.CharacterUniqueNumber, 0,	&arrCB[1]);
		SQLBindCol(hstmt,  2, SQL_C_LONG   , &retQuest.QuestIndex, 0,				&arrCB[2]);
		SQLBindCol(hstmt,  3, SQL_C_TINYINT, &retQuest.QuestState, 0,				&arrCB[3]);
		SQLBindCol(hstmt,  4, SQL_C_LONG   , &retQuest.QuestParam1, 0,				&arrCB[4]);
		SQLBindCol(hstmt,  5, SQL_C_TIMESTAMP, &QuestStartTime, 0,					&arrCB[5]);
		SQLBindCol(hstmt,  6, SQL_C_SBIGINT, &retQuest.QuestPlayTimeStamp, 0,		&arrCB[6]);
		
		// initialize to 0x00
		memset(&retQuest, 0x00, sizeof(CharacterQuest));
		memset(&QuestStartTime, 0, sizeof(SQL_TIMESTAMP_STRUCT));
		LoofCount = 0;
		while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA )
		{
			CQuest *pQuestInfo = m_pFieldIOCP8->m_mapQuestInfoFromScript.findEZ(retQuest.QuestIndex);
			if (NULL == pQuestInfo)
			{// ���� ����Ʈ ��ũ��Ʈ�� �������� �ʴ� ����Ʈ�ε����� �߰� ���� �ʴ´�
				continue;
			}
			if (QUEST_STATE_COMPLETED != retQuest.QuestState)
			{
				// �Ϸ���� ���� ����Ʈ�� �߰� ���� �ʴ´�.
				continue;
			}
			LoofCount++;
			retQuest.QuestTimeStamp = QuestStartTime;		// ����Ʈ ���� �ð� �Ҵ�
			tmFCharQuest.ResetFieldCharacterQuest(&retQuest, pQuestInfo);
			
			tmmapFCharacterQuest.insertNoLock(tmFCharQuest.QuestIndex, tmFCharQuest);
			
			memset(&retQuest, 0x00, sizeof(CharacterQuest));
			memset(&QuestStartTime, 0, sizeof(SQL_TIMESTAMP_STRUCT));
		}
		SQLFreeStmt(hstmt, SQL_CLOSE);
		if (0 < LoofCount)
		{
			///////////////////////////////////////////////////////////////////////////////
			// 2005-10-19 by cmkwon, ����Ʈ ���� ī��Ʈ 
			DBCharacterQuestMonsterCount	tmDBMonsterCount;
			SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &itr->CUID, 0, NULL);
			ret = SQLExecDirect(hstmt, PROCEDURE_080822_0115, SQL_NTS);
			if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_130726_0005, UserAccountUID, itr->CUID, byChangeInfl, 104);
				ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"QP_AccountInflChange PROCEDURE_080822_0115 Failed!\n", TRUE, q.pFieldIOCPSocket, q);
				SQLFreeStmt(hstmt, SQL_CLOSE);
				return;
			}
			util::zero(arrCB, sizeof(arrCB[0])*8);
			SQLBindCol(hstmt,  1, SQL_C_LONG   , &tmDBMonsterCount.CharacterUniqueNumber, 0,	&arrCB[1]);
			SQLBindCol(hstmt,  2, SQL_C_LONG   , &tmDBMonsterCount.QuestIndex, 0,				&arrCB[2]);
			SQLBindCol(hstmt,  3, SQL_C_LONG   , &tmDBMonsterCount.MonsterUniqueNumber, 0,		&arrCB[3]);
			SQLBindCol(hstmt,  4, SQL_C_LONG   , &tmDBMonsterCount.Count, 0,					&arrCB[4]);
			
			util::zero(&tmDBMonsterCount, sizeof(DBCharacterQuestMonsterCount));
			while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA )
			{
				CFieldCharacterQuest *pFCharQuest = tmmapFCharacterQuest.findLock_Ptr(tmDBMonsterCount.QuestIndex);
				if(NULL == pFCharQuest
					|| QUEST_STATE_IN_PROGRESS != pFCharQuest->QuestState)
				{
					// ����Ʈ ���� ���� ����
					QPARAM_DELETE_QUEST_MONSTER_COUNT *pDelMonCount = new QPARAM_DELETE_QUEST_MONSTER_COUNT;
					pDelMonCount->CharacterUniqueNumber		= tmDBMonsterCount.CharacterUniqueNumber;
					pDelMonCount->QuestIndex				= tmDBMonsterCount.QuestIndex;
					this->MakeAndEnqueueQuery(QT_DeleteQuestMonsterCount, NULL, 0, pDelMonCount);
					continue;
				}
				pFCharQuest->InsertMonsterCount(&tmDBMonsterCount);
				
				// �ʱ�ȭ
				util::zero(&tmDBMonsterCount, sizeof(DBCharacterQuestMonsterCount));
			}
			SQLFreeStmt(hstmt, SQL_CLOSE);
		}
		
		// 2. ������ ������ ����Ʈ�� ������ �ִ��� Ȯ��
		vectSQUEST_MATCHING::iterator	itrQuest(m_pFieldIOCP8->m_QuestMatching.m_vectQuestMatchingList.begin());
		for(; itrQuest != m_pFieldIOCP8->m_QuestMatching.m_vectQuestMatchingList.end(); itrQuest++)
		{
			SQUEST_MATCHING *pQuestMatching = &*itrQuest;
			
			// change influence from ANI to BCU
			SQUEST_MATCH_CHANGE tmQMatch;
			tmQMatch.FromQuestIndex			= pQuestMatching->QuestIndexForANI;
			tmQMatch.ToQuestIndex			= pQuestMatching->QuestIndexForBCU;
			BOOL QuestMatcingError = FALSE;
			if (INFLUENCE_TYPE_VCN == byChangeInfl)
			{
				if(0 != pQuestMatching->QuestIndexForBCU && tmmapFCharacterQuest.findNoLock_Ptr(pQuestMatching->QuestIndexForBCU))
				{
					QuestMatcingError = TRUE;	// ������ ������ BCU �ε� BCU ���� ����Ʈ�� ������ ������ ����
				}
			}
			else if (INFLUENCE_TYPE_ANI == byChangeInfl)
			{
				if(0 != pQuestMatching->QuestIndexForANI && tmmapFCharacterQuest.findNoLock_Ptr(pQuestMatching->QuestIndexForANI))
				{
					QuestMatcingError = TRUE;	// ������ ������ ANI �ε� ANI ���� ����Ʈ�� ������ ������ ����
				}
			}
			if (TRUE == QuestMatcingError)
			{
				pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_130726_0005, UserAccountUID, itr->CUID, byChangeInfl, 105);
				return;
			}
		}
		
		// 3. ������ ���¿� �´� ����Ʈ ����
		vectSQUEST_MATCH_CHANGE tmQuestMatchIndex;
		tmQuestMatchIndex.clear();
		tmQuestMatchIndex.reserve(m_pFieldIOCP8->m_QuestMatching.m_vectQuestMatchingList.size());

		vectSQUEST_MATCHING::iterator	itr_QM(m_pFieldIOCP8->m_QuestMatching.m_vectQuestMatchingList.begin());
		for(; itr_QM != m_pFieldIOCP8->m_QuestMatching.m_vectQuestMatchingList.end(); itr_QM++)
		{
			SQUEST_MATCH_CHANGE tmQMatch;
			if (INFLUENCE_TYPE_VCN == byChangeInfl)
			{
				tmQMatch.FromQuestIndex			= itr_QM->QuestIndexForANI;		// ������ ������ BCU�̸� ANI����Ʈ�� ����������
				tmQMatch.ToQuestIndex			= itr_QM->QuestIndexForBCU;		// BCU�� �����ķ�
			}
			else if (INFLUENCE_TYPE_ANI == byChangeInfl)
			{
				tmQMatch.FromQuestIndex			= itr_QM->QuestIndexForBCU;		// ������ ������ ANI�̸� BCU����Ʈ�� ����������
				tmQMatch.ToQuestIndex			= itr_QM->QuestIndexForANI;		// ANI�� �����ķ�
			}
			tmQuestMatchIndex.push_back(tmQMatch);
		}
		
		// 4. ����Ʈ ó��
		vectINT		tmQuetIndexListToDelete;		// ����Ʈ ó�� �� ���� �ؾ� �� ����Ʈ ����Ʈ 
		vectSQUEST_MATCH_CHANGE::iterator itrQM(tmQuestMatchIndex.begin());
		for(; itrQM != tmQuestMatchIndex.end(); itrQM++)
		{
			SQUEST_MATCH_CHANGE *pMatchIndex = &*itrQM;
			if(0 == pMatchIndex->FromQuestIndex)
			{// 2009-04-01 by cmkwon, �ҽ� QuestIndex�� ��� �ִٸ� �ش� ����Ʈ �䱸 ������ üũ�ؼ� �߰� Ȥ�� ó�� ����.
				
				CQuest *pQuestNew = m_pFieldIOCP8->GetQuestInfo(pMatchIndex->ToQuestIndex);
				if(NULL == pQuestNew)
				{
					continue;
				}
				
				if(pQuestNew->ReqLevel.Min > itr->CLevel)
				{// �䱸 ���� �̸��̸� �߰� �ʿ� ����
					continue;
				}
				SDWORD pcbNTS = SQL_NTS;
				CharacterQuest pQInsertQuest;
				pQInsertQuest.CharacterUniqueNumber	= itr->CUID;
				pQInsertQuest.QuestIndex				= pQuestNew->QuestIndex;
				pQInsertQuest.QuestState				= QUEST_STATE_COMPLETED;
				pQInsertQuest.QuestParam1				= 0;
				pQInsertQuest.QuestTimeStamp.SetCurrentDateTime();
				pQInsertQuest.QuestPlayTimeStamp		= (LONGLONG)itr->CTotalPlayTime;
				
				SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQInsertQuest.CharacterUniqueNumber, 0, NULL);	// 2007-11-13 by cmkwon, ���� ����
				SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQInsertQuest.QuestIndex, 0, NULL);
				SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &pQInsertQuest.QuestState, 0, NULL);
				SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQInsertQuest.QuestParam1, 0, NULL);
				char tmpTimeString[SIZE_MAX_SQL_DATETIME_STRING];
				pQInsertQuest.QuestTimeStamp.GetSQLDateTimeString(tmpTimeString, SIZE_MAX_SQL_DATETIME_STRING);
				SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, &tmpTimeString, 0, &pcbNTS);
				SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pQInsertQuest.QuestPlayTimeStamp, 0, NULL);
				
				ret = SQLExecDirect(hstmt, PROCEDURE_080822_0116, SQL_NTS);
				
				if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
				{
					pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_130726_0005, UserAccountUID, itr->CUID, byChangeInfl, 106);
					ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"QP_AccountInflChange in PROCEDURE_080822_0116 Failed!\n", TRUE, q.pFieldIOCPSocket, q);
					SQLFreeStmt(hstmt, SQL_CLOSE);
					return;
				}
				SQLFreeStmt(hstmt, SQL_CLOSE);
			}
			else
			{
				CFieldCharacterQuest *pFCharQuest = tmmapFCharacterQuest.findNoLock_Ptr(pMatchIndex->FromQuestIndex);
				if(NULL == pFCharQuest)
				{// 2009-04-01 by cmkwon, �ش� ����Ʈ�� ������ ���� ����Ʈ
					continue;
				}
				else if(pFCharQuest->QuestState == QUEST_STATE_IN_PROGRESS)
				{
					continue;
				}
				
				if(0 == pMatchIndex->ToQuestIndex)
				{// 2009-04-01 by cmkwon, Ÿ�� QuestIndex�� ���ٸ� �ش� ����Ʈ�� ����ó��
					
					tmQuetIndexListToDelete.push_back(pMatchIndex->FromQuestIndex);
				}
				else
				{// 2009-04-01 by cmkwon, �ҽ� QuestIndex�� Ÿ�� QuestIndex �Ѵ� �ִٸ� ���� ó��
					
					if(NULL != tmmapFCharacterQuest.findNoLock_Ptr(pMatchIndex->ToQuestIndex))
					{// 2009-04-02 by cmkwon, �ߺ����� ó���� ���ش�. �̹� �߰� �Ǿ� �����Ƿ� ���� ó�� �� �ʿ� ����
						continue;
					}
					
					CQuest *pQuestNew = m_pFieldIOCP8->GetQuestInfo(pMatchIndex->ToQuestIndex);
					if(NULL == pQuestNew)
					{
						continue;
					}
					
					SDWORD pcbNTS = SQL_NTS;
					CharacterQuest pQInsertQuest;
					pQInsertQuest.CharacterUniqueNumber	= itr->CUID;
					pQInsertQuest.QuestIndex				= pQuestNew->QuestIndex;
					pQInsertQuest.QuestState				= QUEST_STATE_COMPLETED;
					pQInsertQuest.QuestParam1				= 0;
					pQInsertQuest.QuestTimeStamp.SetCurrentDateTime();
					pQInsertQuest.QuestPlayTimeStamp		= (LONGLONG)itr->CTotalPlayTime;
					
					SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQInsertQuest.CharacterUniqueNumber, 0, NULL);	// 2007-11-13 by cmkwon, ���� ����
					SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQInsertQuest.QuestIndex, 0, NULL);
					SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &pQInsertQuest.QuestState, 0, NULL);
					SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pQInsertQuest.QuestParam1, 0, NULL);
					char tmpTimeString[SIZE_MAX_SQL_DATETIME_STRING];
					pQInsertQuest.QuestTimeStamp.GetSQLDateTimeString(tmpTimeString, SIZE_MAX_SQL_DATETIME_STRING);
					SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, &tmpTimeString, 0, &pcbNTS);
					SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pQInsertQuest.QuestPlayTimeStamp, 0, NULL);
					
					ret = SQLExecDirect(hstmt, PROCEDURE_080822_0116, SQL_NTS);
					
					if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
					{
						pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_130726_0005, UserAccountUID, itr->CUID, byChangeInfl, 107);
						ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"QP_AccountInflChange in PROCEDURE_080822_0116 Failed!\n", TRUE, q.pFieldIOCPSocket, q);
						SQLFreeStmt(hstmt, SQL_CLOSE);
						return;
					}
					SQLFreeStmt(hstmt, SQL_CLOSE);
					
					tmQuetIndexListToDelete.push_back(pMatchIndex->FromQuestIndex);					
				}
			}
		} // END - for(; itrQM != tmQuestMatchIndex.end(); itrQM++)
		vectINT::iterator	itrQID(tmQuetIndexListToDelete.begin());
		for(; itrQID != tmQuetIndexListToDelete.end(); itrQID++)
		{
			INT nDelQuestIndex = *itrQID;
			if(tmmapFCharacterQuest.findNoLock_Ptr(nDelQuestIndex))
			{
				// ����Ʈ ����� - DB
				SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &itr->CUID, 0, NULL);
				SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nDelQuestIndex, 0, NULL);
				ret = SQLExecDirect(hstmt, PROCEDURE_080822_0118, SQL_NTS);
				if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_NO_DATA )
				{
					pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_130726_0005, UserAccountUID, itr->CUID, byChangeInfl, 108);
					ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"QP_AccountInflChange in PROCEDURE_080822_0118 Failed!\n", TRUE, pFieldIOCPSocket, q);
					SQLFreeStmt(hstmt, SQL_CLOSE);
					return;
				}
				SQLFreeStmt(hstmt, SQL_CLOSE);
			}
		}
		
		// 5. ������ ó��
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &itr->CUID, 0, NULL);
		SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &byChangeInfl, 0, NULL);
		ret = SQLExecDirect(hstmt, PROCEDURE_130726_0002, SQL_NTS);
		if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_NO_DATA)	// 2007-08-01 by cmkwon, �߰���(&& ret!=SQL_NO_DATA)
		{
			pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_130726_0005, UserAccountUID, itr->CUID, byChangeInfl, 109);
			ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"QP_AccountInflChange in PROCEDURE_130726_0002 Failed!\r\n", TRUE, pFieldIOCPSocket, q);
			SQLFreeStmt(hstmt, SQL_CLOSE);
			return;
		}
		SQLFreeStmt(hstmt, SQL_CLOSE);
		
		// 6. �ɸ��� ���� ���� ����
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &UserAccountUID, 0, NULL);
		SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &itr->CUID, 0, NULL);
		SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &byChangeInfl, 0, NULL);
		SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &byChangeInfl, 0, NULL);
		ret = SQLExecDirect(hstmt, PROCEDURE_080822_0058, SQL_NTS);
		
		if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
		{
			pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_130726_0005, UserAccountUID, itr->CUID, byChangeInfl, 110);
			ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt,"QP_AccountInflChange PROCEDURE_080822_0058 Failed!\n", TRUE, q.pFieldIOCPSocket, q);
			SQLFreeStmt(hstmt, SQL_CLOSE);
			return;
		}
		SQLFreeStmt(hstmt, SQL_CLOSE);
		
		pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_130726_0006, UserAccountUID, itr->CUID, byChangeInfl);

		CAtumLogSender::SendLogMessageAccountInflChange_byAdmin(UserAccountUID, itr->CUID, (INFLUENCE_TYPE_VCN==byChangeInfl)?INFLUENCE_TYPE_ANI:INFLUENCE_TYPE_VCN, byChangeInfl, pFieldIOCPSocket->m_character.CharacterUniqueNumber);	
	}
}

void CAtumFieldDBManager::OP_UpgradPet(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	if (!q.pQueryParam || !q.pFieldIOCPSocket)
		return;
	QPARAM_UPGRADE_PET* upgradePetParams = (QPARAM_UPGRADE_PET*)q.pQueryParam;

	// Bind old and new Character Store Item num as parameters
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &upgradePetParams->OldItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &upgradePetParams->UpgradedPetItemUID, 0, NULL);

	// Execute Query
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_110824_0011, SQL_NTS);

	// Check for successful preperation
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "dbo.atum_Upgrade_Pet failed!\r\n", TRUE, NULL, q);
		// Free statements & cleanup Query
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(upgradePetParams);
		return;
	}

	// Bind Result
	SQLINTEGER cb[44];
	fill_n(cb, 44, SQL_NTS);

	// Bind result columns
	ITEM_GENERAL storeItem;
	SQL_TIMESTAMP_STRUCT createdTimeStamp;
	tPET_CURRENTINFO petInfo(NULL);

	int colIdx = 0;

	// Item General Params
	SQLBindCol(hstmt, ++colIdx, SQL_C_SBIGINT, &storeItem.UniqueNumber, 0, &cb[colIdx]);
	SQLBindCol(hstmt, ++colIdx, SQL_C_ULONG, &storeItem.AccountUniqueNumber, 0, &cb[colIdx]);
	SQLBindCol(hstmt, ++colIdx, SQL_C_ULONG, &storeItem.Possess, 0, &cb[colIdx]);
	SQLBindCol(hstmt, ++colIdx, SQL_C_UTINYINT, &storeItem.ItemStorage, 0, &cb[colIdx]);
	SQLBindCol(hstmt, ++colIdx, SQL_C_UTINYINT, &storeItem.Wear, 0, &cb[colIdx]);
	SQLBindCol(hstmt, ++colIdx, SQL_C_SLONG, &storeItem.CurrentCount, 0, &cb[colIdx]);
	SQLBindCol(hstmt, ++colIdx, SQL_C_SLONG, &storeItem.ItemWindowIndex, 0, &cb[colIdx]);
	SQLBindCol(hstmt, ++colIdx, SQL_C_SLONG, &storeItem.ItemNum, 0, &cb[colIdx]);
	SQLBindCol(hstmt, ++colIdx, SQL_C_SSHORT, &storeItem.NumOfEnchants, 0, &cb[colIdx]);
	SQLBindCol(hstmt, ++colIdx, SQL_C_LONG, &storeItem.PrefixCodeNum, 0, &cb[colIdx]);
	SQLBindCol(hstmt, ++colIdx, SQL_C_LONG, &storeItem.SuffixCodeNum, 0, &cb[colIdx]);
	SQLBindCol(hstmt, ++colIdx, SQL_C_FLOAT, &storeItem.CurrentEndurance, 0, &cb[colIdx]);
	SQLBindCol(hstmt, ++colIdx, SQL_C_LONG, &storeItem.ColorCode, 0, &cb[colIdx]);
	SQLBindCol(hstmt, ++colIdx, SQL_C_LONG, &storeItem.UsingTimeStamp, 0, &cb[colIdx]);
	SQLBindCol(hstmt, ++colIdx, SQL_C_TIMESTAMP, &createdTimeStamp, 0, &cb[colIdx]);
	SQLBindCol(hstmt, ++colIdx, SQL_C_LONG, &storeItem.ShapeItemNum, 0, &cb[colIdx]);
	SQLBindCol(hstmt, ++colIdx, SQL_C_SBIGINT, &storeItem.MainSvrItemUID, 0, &cb[colIdx]);
	SQLBindCol(hstmt, ++colIdx, SQL_C_ULONG, &storeItem.CoolingTimeStamp, 0, &cb[colIdx]);

	//Pet Params
	SQLBindCol(hstmt, ++colIdx, SQL_C_UBIGINT, &petInfo.CreatedPetUID, 0, &cb[colIdx]);
	SQLBindCol(hstmt, ++colIdx, SQL_C_LONG, &petInfo.PetIndex, 0, &cb[colIdx]);
	SQLBindCol(hstmt, ++colIdx, SQL_C_DOUBLE, &petInfo.PetExp, 0, &cb[colIdx]);
	SQLBindCol(hstmt, ++colIdx, SQL_C_LONG, &petInfo.PetLevel, 0, &cb[colIdx]);
	SQLBindCol(hstmt, ++colIdx, SQL_C_CHAR, &petInfo.PetName, SIZE_MAX_PET_NAME, &cb[colIdx]);
	SQLBindCol(hstmt, ++colIdx, SQL_C_UTINYINT, &petInfo.PetExpRatio, 0, &cb[colIdx]);
	SQLBindCol(hstmt, ++colIdx, SQL_C_UTINYINT, &petInfo.PetEnableSocketCount, 0, &cb[colIdx]);
	for (int iPet = 0; iPet < SIZE_MAX_PETSOCKET; iPet++)
	{
		SQLBindCol(hstmt, ++colIdx, SQL_C_UBIGINT, &petInfo.PetSocketItemUID[iPet], 0, &cb[colIdx]);
	}
	SQLBindCol(hstmt, ++colIdx, SQL_C_LONG, &petInfo.PetKitHP.ItemNum, 0, &cb[colIdx]);
	SQLBindCol(hstmt, ++colIdx, SQL_C_FLOAT, &petInfo.PetKitHP.TriggerValue, 0, &cb[colIdx]);
	SQLBindCol(hstmt, ++colIdx, SQL_C_LONG, &petInfo.PetKitShield.ItemNum, 0, &cb[colIdx]);
	SQLBindCol(hstmt, ++colIdx, SQL_C_FLOAT, &petInfo.PetKitShield.TriggerValue, 0, &cb[colIdx]);
	SQLBindCol(hstmt, ++colIdx, SQL_C_LONG, &petInfo.PetKitSP.ItemNum, 0, &cb[colIdx]);
	SQLBindCol(hstmt, ++colIdx, SQL_C_FLOAT, &petInfo.PetKitSP.TriggerValue, 0, &cb[colIdx]);
	for (int iPet = 0; iPet < SIZE_MAX_PETSOCKET_AUTOSKILL_SLOT; iPet++)
	{
		SQLBindCol(hstmt, ++colIdx, SQL_C_LONG, &petInfo.PetAutoSkill.ItemNum[iPet], 0, &cb[colIdx]);
	}

	// Set targets to zero before fetching the results
	util::zero(&storeItem, sizeof(ITEM_GENERAL));
	util::zero(&createdTimeStamp, sizeof(SQL_TIMESTAMP_STRUCT));
	util::zero(&petInfo, sizeof(tPET_CURRENTINFO));

	// Fetch the result
	ret = SQLFetch(hstmt);
	if (ret == SQL_NO_DATA
		|| (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO))
	{
		// Error during fetching of the result
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "dbo.atum_Upgrade_Pet failed!\r\n", TRUE, NULL, q);
		// Free statements & cleanup Query
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(upgradePetParams);
		return;
	}

	// Get / Validate Item info
	ITEM *pItemInfo = m_pFieldIOCP8->GetItemInfo(storeItem.ItemNum);
	if (pItemInfo == NULL)
	{
		g_pGlobal->WriteSystemLogEX(TRUE, "  [Error]Invaild Item in DB in CAtumFieldDBManager::QP_GetStoreItem() -> %10s, %d \r\n",
			q.pFieldIOCPSocket->m_character.CharacterName, storeItem.ItemNum);

		// 2009-11-20 by cmkwon, ���� ������ �ε��� ��ȿ���� ���� ������ ó�� ���� - CAtumFieldDBManager::QP_NewGetStoreItem#, ���ӷα��߰�, DB ������ ����
		CAtumLogSender::SendLogMessageITEMDelete(q.pFieldIOCPSocket, &storeItem, IDT_INVALID_ITEMNUM);
		QPARAM_DELETESTOREITEM *pQParam = new QPARAM_DELETESTOREITEM;
		pQParam->AccountUniqueNumber = q.pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber;
		pQParam->ItemUniqueNumber = storeItem.UniqueNumber;
		this->MakeAndEnqueueQuery(QT_DeleteStoreItem, q.pFieldIOCPSocket, q.pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber, pQParam);

		// Free statements & cleanup Query
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(upgradePetParams);
		return;
	}
	storeItem.ItemInfo = pItemInfo;
	storeItem.Kind = pItemInfo->Kind;

	// Convert SQL TimeStamp to AtumTimestamp
	storeItem.CreatedTime = createdTimeStamp;

	// Lock the Item Manager
	mt_auto_lock igLock(&q.pFieldIOCPSocket->m_ItemManager.m_mapItemGeneral);

	// Insert the new Pet Item
	q.pFieldIOCPSocket->m_ItemManager.InsertUpgradedPet(upgradePetParams->OldItemUniqueNumber, &storeItem, &petInfo);

	// Free statements & cleanup Query
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(upgradePetParams);
}

#ifdef S_IP_UNIQUE_CONNECTION
void CAtumFieldDBManager::QP_GetCharacterMultipleIPStatus(FIELD_DB_QUERY q, SQLHSTMT hstmt)
{
	if (!q.pQueryParam || !q.pFieldIOCPSocket)
		return;

	QPARAM_GET_CHARACTER_MULTIPLE_IP_STATUS* pCharacterParams = (QPARAM_GET_CHARACTER_MULTIPLE_IP_STATUS*)q.pQueryParam;

	// Bind Char UID
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, &pCharacterParams->AccountName, 0, NULL);

	// Exec proc
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_130727_0001, SQL_NTS);

	// Check for errors
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		q.pFieldIOCPSocket->ResGetMultipleIpStatus(FALSE, pCharacterParams->AdminIssuerCharacterUID);

		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "dbo.atum_GetMultipleIpStatusByCharacterId failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(q.pQueryParam);

		return;
	}

	// Bind result
	SQLINTEGER cb = SQL_NTS;
	BOOL isAllowedMultiIP = FALSE;
	SQLBindCol(hstmt, 1, SQL_C_BIT, &isAllowedMultiIP, 0, &cb);

	// Fetch the result
	ret = SQLFetch(hstmt);
	if (ret == SQL_NO_DATA
		|| (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO))
	{
		q.pFieldIOCPSocket->ResGetMultipleIpStatus(FALSE, pCharacterParams->AdminIssuerCharacterUID);

		// Error during fetching of the result
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "dbo.atum_GetMultipleIpStatusByCharacterId failed!\r\n", TRUE, NULL, q);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(q.pQueryParam);

		return;
	}

	// Call Field Socket with the result
	q.pFieldIOCPSocket->ResGetMultipleIpStatus(isAllowedMultiIP, pCharacterParams->AdminIssuerCharacterUID);

	// Free statements & cleanup Query
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(q.pQueryParam);
}
#endif // S_IP_UNIQUE_CONNECTION

// 2015-12-17 Future, Nation Change Card
void CAtumFieldDBManager::QP_NationTransfer(FIELD_DB_QUERY dbquery, SQLHSTMT hstmt)
{
	// Get params
	if (!dbquery.pQueryParam || !dbquery.pFieldIOCPSocket)
		return;
	QPARAM_NATION_RESET* params = (QPARAM_NATION_RESET*)dbquery.pQueryParam;

	//////////////////////////////////////////////////////////////////////////
	// Get the available Nation Xfers per nation
	BOOL xferAvailable = FALSE;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &params->Nation, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_130727_0003, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		// Switching of nation unsuccessful
		dbquery.pFieldIOCPSocket->ResSwitchNation(FALSE, params->ItemUID);

		// Error during execution of the stmt
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "dbo.atum_GetAvailalbeNationTransfers Failed!\r\n", TRUE, dbquery.pFieldIOCPSocket, dbquery);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(dbquery.pQueryParam);
		return;
	}

	SQLBindCol(hstmt, 1, SQL_C_LONG, &xferAvailable, 0, NULL);

	ret = SQLFetch(hstmt);
	if (ret == SQL_NO_DATA
		|| (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO))
	{
		// Switching of nation unsuccessful
		dbquery.pFieldIOCPSocket->ResSwitchNation(FALSE, params->ItemUID);

		// Error during fetching of the result
		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "dbo.atum_GetAvailalbeNationTransfers failed!\r\n", TRUE, dbquery.pFieldIOCPSocket, dbquery);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(dbquery.pQueryParam);

		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);

	// Check if the nation switch can be done
	if (!xferAvailable)
	{
		// Cannot switch because no more transfers are available
		dbquery.pFieldIOCPSocket->ResSwitchNation(FALSE, params->ItemUID);
		util::del(dbquery.pQueryParam);
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// Execute the nation switch procedure
	
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, &params->AccountName, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &params->Nation, 0, NULL);

	// Exec Proc
	ret = SQLExecDirect(hstmt, PROCEDURE_130727_0002, SQL_NTS);

	// Check for successful execution
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		dbquery.pFieldIOCPSocket->ResSwitchNation(FALSE, params->ItemUID);

		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "atum_Nation_Transfer Failed!\r\n", TRUE, dbquery.pFieldIOCPSocket, dbquery);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(dbquery.pQueryParam);
		return;
	}

	// Successful switch
	dbquery.pFieldIOCPSocket->ResSwitchNation(TRUE, params->ItemUID);

	// Free statements & cleanup Query
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(dbquery.pQueryParam);
}

//#if S_BONUS_KILL_SYSTEM_RENEWAL
//
//// 2015-06-11 Future, Kill Count System
//void CAtumFieldDBManager::QP_GetAddKillRestKillCount(FIELD_DB_QUERY q, SQLHSTMT hstmt)
//{
//	QPARAM_ADDKILL_REST_KILL_COUNT *pQParam = (QPARAM_ADDKILL_REST_KILL_COUNT*)q.pQueryParam;
//	CFieldIOCPSocket *pFieldIOCPSocket = q.pFieldIOCPSocket;
//
//	if (NULL == pFieldIOCPSocket || FALSE == pFieldIOCPSocket->IsValidCharacter(FALSE))
//	{
//		util::del(pQParam);
//
//		return;
//	}
//
//	pFieldIOCPSocket->m_nRestKillCount = 0;
//
//#ifndef S_BONUS_KILL_SYSTEM_RENEWAL
//	util::del(pQParam);
//
//	return;		// �޽� ����ġ ON / OFF ���
//#endif // !S_BONUS_KILL_SYSTEM_RENEWAL
//
//	if (TRUE == g_pFieldGlobal->IsArenaServer())			// �Ʒ��������� ��� ����
//	{
//		util::del(pQParam);
//
//		return;
//	}
//
//	SQLINTEGER cb = SQL_NTS;
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQParam->AccountUID, 0, NULL);
//
//	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_121008_0003, SQL_NTS);
//
//	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA) {
//		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "atum_Get_AddKills_RestKillCount Failed (1) !!\n", TRUE, q.pFieldIOCPSocket, q);
//		SQLFreeStmt(hstmt, SQL_CLOSE);
//		util::del(pQParam);
//
//		return;
//	}
//
//	SQLBindCol(hstmt, 1, SQL_C_ULONG, &pFieldIOCPSocket->m_nRestKillCount, 0, NULL);
//	ret = SQLFetch(hstmt);
//
//	SQLFreeStmt(hstmt, SQL_CLOSE);
//	util::del(pQParam);
//
//#if defined(S_BONUS_KILL_SYSTEM_RENEWAL)
//	pFieldIOCPSocket->SendToClient_RestAndKillCount();
//#endif S_BONUS_KILL_SYSTEM_RENEWAL
//}
//
//void CAtumFieldDBManager::QP_SetAddKillRestKillCount(FIELD_DB_QUERY q, SQLHSTMT hstmt)
//{
//	QPARAM_ADDKILL_REST_KILL_COUNT *pQParam = (QPARAM_ADDKILL_REST_KILL_COUNT*)q.pQueryParam;
//	CFieldIOCPSocket *pFieldIOCPSocket = q.pFieldIOCPSocket;
//
//	SQLINTEGER cb = SQL_NTS;
//
//	if (TRUE == g_pFieldGlobal->IsArenaServer())
//	{
//		util::del(pQParam);
//
//		return;
//	}
//
//#ifndef S_BONUS_KILL_SYSTEM_RENEWAL
//	util::del(pQParam);
//	return;
//#endif // !S_BONUS_KILL_SYSTEM_RENEWAL
//
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQParam->AccountUID, 0, NULL);
//	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQParam->RestKillCount, 0, NULL);
//
//	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_121008_0004, SQL_NTS);
//
//	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
//	{
//		ProcessLogMessagesField(SQL_HANDLE_STMT, hstmt, "atum_Set_AddKills_RestKillCount Failed!\r\n", TRUE, NULL, q);
//		SQLFreeStmt(hstmt, SQL_CLOSE);
//		util::del(pQParam);
//		return;
//	}
//
//	SQLFreeStmt(hstmt, SQL_CLOSE);
//	util::del(pQParam);
//}
//// end 2015-06-11 Future, Kill Count System
//#endif // S_BONUS_KILL_SYSTEM_RENEWAL