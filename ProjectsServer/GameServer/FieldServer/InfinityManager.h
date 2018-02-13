/// InfinityManager.h: interface for the CInfinityManager class.
/// \brief		���Ǵ�Ƽ - ���Ǵ�Ƽ ���� Ŭ����
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFINITYMANAGER_H__9E1738B9_2E9B_4A12_8CDE_FF761AAC14A8__INCLUDED_)
#define AFX_INFINITYMANAGER_H__9E1738B9_2E9B_4A12_8CDE_FF761AAC14A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumFieldDBManager.h"

#include "Cinema.h"
#include "Revision.h"
#include "InfinityMapManager.h"
#include "InfinityBase.h"
#include "InfinityBossrush.h"
#include "InfinityDefence.h"
#include "InfinityMShipBattle.h"			// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�

typedef mt_vector<CInfinityBossrush*>	mtvectInfiBossRush;
typedef mt_vector<CInfinityDefence*>	mtvectInfiDefence;
typedef mt_vector<CInfinityMShipBattle*>	mtvectInfiMShipBattle;			// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
typedef vector<CFieldIOCPSocket*>   vectCFieldIOCPSocket;

class CInfinityTickManager;
class CInfinityManager
{
public:
	CInfinityManager();
	virtual ~CInfinityManager();

	void InitInfinityManager();
	void SetInfinityMapManagerW(CFieldMapWorkspace * i_pFieldMapWorkspace);
	BOOL SetDBManager(CAtumFieldDBManager * i_pAtumDBManager);
	void SetCinemaInfoW(vectorCinemaInfo * i_pVectCinemaInfo);
	void SetRevisionInfoW(vectorRevisionInfo * i_pVectRevisionInfo);
	void SetDBInfinityModeInfo(vectorInfinityModeInfo * i_pVectInfiModeInfo);
	void SetDBInfinityMonsterInfo(vectorInfinityMonsterInfo * i_pVectInfiMonsterInfo);
	void GetInfinityModeInfo(vectorInfinityModeInfo * o_pCopyVectInfiModeInfo);
	INT MakeMsgInfinityPlayingList(INFINITY_READY_LIST * o_pInfinityPlayingList, MapIndex_t i_nInfinityMapIdx, eINFINITY_MODE i_nInfinityMode, BYTE i_byInfluenceType);
	BOOL CheckInfinityModeLevel(InfiModeUID_t i_nInfinityModeUID, Lv_t i_Lv);

	// 2010-03-23 by cmkwon, ���Ǵ�Ƽ ���� ĳ�� ������ ���� - 
	BOOL CheckEntranceCount(InfiModeUID_t i_nInfinityModeUID, EntranceCount_t i_EntranceCount, int i_nAddLimiteCnt=0);

	BOOL CreateInfinity(MSG_FC_INFINITY_CREATE * i_pInfinityCreateInfo, CFieldIOCPSocket * i_pFISoc, InfinityCreateUID_t * o_pCreateUID, INT * o_nInfinityDifficultyLevel );	
	Err_t JoinCheckInfinity(MSG_FC_INFINITY_JOIN * i_pInfinityJoinRequestInfo, CFieldIOCPSocket * i_pFISoc);		// ���� ���� �� üũ ���� 
	Err_t JoinInfinity(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, CFieldIOCPSocket * i_pFISoc);		// ���� ����
	void GetPlayerListW(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, vectCFieldIOCPSocket * o_pInfinityMemberList, ClientIndex_t * o_pMasterUserClientIdx);
	Err_t ChangeMasterUserW(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, CFieldIOCPSocket * i_pChangeMasterUserFISoc);
	Err_t LeaveInfinity(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, CFieldIOCPSocket * i_pFISoc);		// ���� Ż��
	Err_t BanInfinity(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, CFieldIOCPSocket * i_pBanFISoc);		// ���� �߹�
	Err_t StartInfinity(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, CFieldIOCPSocket * i_pFISoc);		// ���� ����
	BOOL GetRevisionInfoW(REVISIONINFO * o_pRevisionInfo, InfiModeUID_t i_InfinityModeUID, INT i_nUnitKind);		// ����ġ ��������
	Err_t UserMapLoadedComplete(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, MapIndex_t i_MapIndex);		// ���� �� ����
	Err_t ChoiceTenderItemW(DiceCnt_t *o_pDiceResult, MSG_FC_INFINITY_TENDER_PUT_IN_TENDER * i_pPutInTenderInfo, ClientIndex_t i_PlayerClientIdx);		// ���� ����
	void ProcessingInfinityPenalty(char * i_szCharacterName, eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID);	// 2009-09-09 ~ 2010-01-13 by dhjin, ���Ǵ�Ƽ - ���� ���� �̸� ���� ���� �߰�, // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ����� �г�Ƽ �߰�
	void SendInfinityTeamChatW(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, BYTE * i_pDATA, int i_nSize);	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ä��
	Err_t ImputeInfinityW(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID);		// 2010-04-05 by cmkwon, ���� ������ ī�� ���� �ý��� ���� - 

	// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - �濡 ���̵� ���� ����.
	Err_t ChangeInfinityDifficultyLevel ( const INT i_cst_ChangeDifficultyLevel , eINFINITY_MODE i_eInfiMode , InfinityCreateUID_t i_CreateUID , CFieldIOCPSocket * i_pMasterUserFISoc );

	// ���Ǵ�Ƽ ���� �ó׸� ���� ��ŵ Ȱ��ȭ. // 2010. 07. 27 by hsLee ���Ǵ�Ƽ 2�� ���� ��� �ó׸� ���� ��ŵ ó��.
	Err_t InfinitySkipEndingCinema ( eINFINITY_MODE i_eInfiMode , InfinityCreateUID_t i_CreateUID , CFieldIOCPSocket * i_pPlayerFISoc , const bool a_bNormalEnding = false );

	///////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��
	void SetDisConnectUserInfo(INFINITY_DISCONNECTUSER_INFO * i_pDisConnectUserInfo);		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��, ���� ƨ�� ������ ��� ���� ����.
	Err_t DisConnectUserReStart(char * i_DisConnectUserName, INFINITY_PLAYING_INFO * o_pInfinityPlayingInfo);		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��, ���� ƨ�� ���� ���� ó��
	Err_t CheckIsCreateInfinityUID(INFINITY_PLAYING_INFO * o_pInfinityPlayingInfo);				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��
	BOOL DeleteDisConnectUserInfo(char * i_DisConnectUserName);	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��, ���� ƨ�� ������ ��� ���� ����.
	void ReStartDisConnectUserW(InfinityCreateUID_t i_nInfinityCreateUID, eINFINITY_MODE i_eInfiMode, CFieldIOCPSocket * i_pUserFISoc);	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó�� ����

	// Tick����
	void StartTick();
	void DoSecondlyWorkInfinity(ATUM_DATE_TIME *pDateTime);


	void CreateKeyMonster_DeadForNextStepW(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx, MonIdx_t i_CreateMonsterIdx);

	// 2010-04-09 by cmkwon, ����2�� �߰� ����(�ܰ躰 ���� �߰�) - 
	void DeleteKeyMonster_DeadForNextStepW(BOOL *o_pbCompensationFlag, MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx, MonIdx_t i_DeadMonsterIdx);

	void CreateKeyMonster_AliveForGameClearW(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx, MonIdx_t i_CreateMonsterIdx);
	void DeleteKeyMonster_AliveForGameClearW(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx, MonIdx_t i_DeadMonsterIdx);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-23 by cmkwon, ���Ǵ�Ƽ ���� ĳ�� ������ ���� - 
	Err_t SendFtoA_INFINITY_START_CHECK_W(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID);
	CFieldIOCPSocket *GetMasterPlayerW(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID);
	BOOL CheckInfinityStartCheckAckW(eINFINITY_STATE *o_pInfiRoomState, MSG_FtoA_INFINITY_START_CHECK_ACK *i_pStartCheckAck);
	void SetAllPlayerStateW(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, eINFINITY_STATE i_InfiState);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-26 by cmkwon, ����2�� ���� - CFieldIOCP �������� �߰�
	CFieldIOCP					*m_pFieldIOCP21;

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-05 by cmkwon, ���� ������ ī�� ���� �ý��� ���� - 
	CInfinityBossrush *FindBossrushNoLock(InfinityCreateUID_t i_CreateUID);
	CInfinityDefence *FindDefenceNoLock(InfinityCreateUID_t i_CreateUID);
	CInfinityMShipBattle *FindMShipBattleNoLock(InfinityCreateUID_t i_CreateUID);			// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-09 by cmkwon, ����2�� �߰� ����(�ܰ躰 ���� �߰�) - 
	CInfinityBossrush *FindBossrushNoLock(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx);
	CInfinityDefence *FindDefenceNoLock(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx);
	CInfinityMShipBattle *FindMShipBattleNoLock(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx);			// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�

	Err_t PushTenderItemW(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx, CTenderItemInfo *i_pTenderItemInfo);

	///////////////////////////////////////////////////////////////////////////////	
	// 2010-04-06 by cmkwon, ����2�� �߰� ���� - 
	Err_t CheckInfinityAllPlayerStateW(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, eINFINITY_STATE i_infiState);

	///////////////////////////////////////////////////////////////////////////////	
	// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
	///////////////////////////////////////////////////////////////////////////////
	// 2010-05-06 by shcho, ���̵� ������ ���� ��������
	BOOL Get_Difficulty_BonusInfo_ListData(vectorInfinity_DifficultyInfo_Bonus *p_vecGetInfinity_DifficulytList);
	BOOL Set_Difficulty_BonusInfo_ListData(vectorInfinity_DifficultyInfo_Bonus *p_vecListInfo);

	const INFINITY_DIFFICULTY_BONUS_INFO *Get_Difficulty_BonusInfo ( const int a_iStep );
	///////////////////////////////////////////////////////////////////////////////
	// 2010-05-06 by shcho, ���̵� ������ ���� ����
	const INFINITY_DIFFICULTY_MONSTER_SETTING_INFO *Get_Difficulty_MonsterInfo (int iStep);

	BOOL Set_Difficulty_MonsterInfo_ListData(vectorInfinity_DifficultyInfo_Monster *p_vecListInfo);
	// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )

	void UpdateNextSceneProc ( eINFINITY_MODE i_eInfiMode , InfinityCreateUID_t i_CreateUID , CFieldIOCPSocket * i_pMasterFISoc , int nUpdateSceneCount = 1 );	// 2010. 06. 04 by hsLee ��Ƽ�Ǵ� �ʵ� 2�� ���̵� ����. (GM ��ɾ� �߰�. /nextscene(���� �ó׸� �� ȣ��.) ) - GM ���� ���׸� �ٷ� ȣ��.
	
protected:

	CAtumFieldDBManager			*m_pAtumDBManager;
	CInfinityTickManager		*m_pTickManager;
	CCinema						m_Cinema;
	CRevision					m_Revision;
	CInfinityMapManager			m_InfiMapManager;
	vectorInfinityModeInfo		m_vectInfiModeInfo;
	vectorInfinityMonsterInfo	m_vectInfiMonsterInfo; // ���Ǵ�Ƽ ���� info ����
	mtvectInfiBossRush			m_mtvectInfiBossRush;
	mtvectInfiDefence			m_mtvectInfiDefence;
	mtvectInfiMShipBattle		m_mtvectInfiMShipBattle;		// 2011-02-18 by hskim, ���Ǵ�Ƽ 3�� - �� ���� �۾�
	
	// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
	vectorInfinity_DifficultyInfo_Bonus			m_vecInfinityDifficultyBonusInfo;	// 2010-05-13 by shcho, ���Ǵ�Ƽ ���̵� ���� - �������� �����ִ� ���̵� ����
	vectorInfinity_DifficultyInfo_Monster		m_vecInfinityDifficultyMonsterInfo;	// 2010-05-13 by shcho, ���Ǵ�Ƽ ���̵� ���� - ���� ���� ����
	// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
	
	InfinityCreateUID_t				m_nInfinityCreateUID;
	mtvectorInfinityDisConnectUser  m_mtInfinityDisConnectUserList;		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��
	CRITICAL_SECTION				m_criticalSectionCreate;				// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
};

#endif // !defined(AFX_INFINITYMANAGER_H__9E1738B9_2E9B_4A12_8CDE_FF761AAC14A8__INCLUDED_)
