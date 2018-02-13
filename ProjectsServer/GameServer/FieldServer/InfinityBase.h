/// InfinityBase.h: interface for the CInfinityBase class.
/// \brief		���Ǵ�Ƽ - ���Ǵ�Ƽ Ŭ���� 
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFINITYBASE_H__679CAEA9_97E4_46FF_AD8F_2808F8C05E3E__INCLUDED_)
#define AFX_INFINITYBASE_H__679CAEA9_97E4_46FF_AD8F_2808F8C05E3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFieldIOCPSocket;
class CFieldMapChannel;
typedef vector<CFieldIOCPSocket*>		VectCFISoc;
typedef mt_vector<CFieldIOCPSocket*>	mtVectCFISoc;
struct KEY_MONSTER_INFO {
	MonIdx_t		KeyMonsterIdx;
	BOOL			KeyMonsterAlive;
	SummonCnt_t		KeyMonsterCount;		// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -
	CinemaOrder_t	KeyMonsterOrder;		// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -
};
typedef vector<KEY_MONSTER_INFO>				VectKeyMonsterInfo;

struct TENDER_USER_INFO {
	ClientIndex_t	UserClientIdx;
	UINT			ItemFieldIndex;
	DiceCnt_t		DiceCount;
	BOOL			GiveUp;
};
typedef vector<TENDER_USER_INFO>				VectTenderUserInfo;

///////////////////////////////////////////////////////////////////////////////
// 2010-04-09 by cmkwon, ����2�� �߰� ����(�ܰ躰 ���� �߰�) - 
struct BIDDER_INFO
{
	ClientIndex_t	UserClientIdx;
	DiceCnt_t		DiceCount;		// �ֻ��� ī��Ʈ(0: ������ ���� ����, 1~100: ���� ������ ��, ���� �ֻ��� ���� ���� ������ ������ ó��)
	BOOL			GiveUp;			// ���� ���� ����
};
typedef vector<BIDDER_INFO>				vectBIDDER_INFO;

class CTenderItemInfo
{
public:
	CTenderItemInfo();
	CTenderItemInfo(const CTenderItemInfo & i_tenderingInfo);
	
	BOOL CheckAllUserChoice(VectCFISoc *i_pUserList);
	BIDDER_INFO *GetBidderInfo(ClientIndex_t i_cliIdx);
	Err_t PlayBidding(DiceCnt_t *o_pDiceResult, ClientIndex_t i_cliIdx, BOOL i_bGiveUp);
	BOOL IsExistDiceValue(DiceCnt_t i_nDiceValue);
	BIDDER_INFO *FindSuccessfulBidder(void);
	BOOL PushBidder(BIDDER_INFO *i_pBidderInfo);
public:
	UINT				m_ItemFieldIndex;
	//DWORD				m_TenderStartTick;
	ATUM_DATE_TIME		m_TenderStartTime;
	D3DXVECTOR3			m_TenderItemPosition;
	ItemNum_t			m_TenderItemNum;
	INT					m_TenderItemCount;
	BOOL				m_IsTimeOver;			// TRUE: ������ �ð� ����� ó����, FALSE: ��� ������ ���� Ȥ�� ���⸦ ���������� ó����
	vectBIDDER_INFO		m_BidderList;			// 2010-04-09 by cmkwon, ����2�� �߰� ����(�ܰ躰 ���� �߰�) - 
};
typedef mt_vector<CTenderItemInfo>						mtvectTenderItemInfo;


class CFieldIOCP;		// 2010-03-26 by cmkwon, ����2�� ���� - CFieldIOCP �������� �߰�
class CInfinityBase  
{
	friend class CAtumLogSender;				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�
public:
	CInfinityBase();
	virtual ~CInfinityBase();

	void InitInfinityBase();
	void DeleteNullFISocPtrNoLock();		// CFieldIOCPSocket NULL �����͸� �����Ѵ�. �� Lock�� ���� �����Ƿ� �Լ� ȣ�� �������� ��ƾ��Ѵ�.!
	void SetInfinityCreateUID(InfinityCreateUID_t i_nInfinityCreateUID);
	void InitMasterPlayer(CFieldIOCPSocket * i_pMasterPlayerSoc);
	void SetModeInfo(INFINITY_MODEINFO * i_pInfinityModeInfo);
	void SetCinemaInfo(vectorCinemaInfo * i_pVectCinemaInfo);
	void SetKeyMonsterList();
	BOOL SetKeyMonster_DeadForNextStep(KEY_MONSTER_INFO * i_pKeyMonsterInfo);	// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -
	void SetKeyMonster_AliveForGameClear(KEY_MONSTER_INFO * i_pKeyMonsterInfo);	// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -
	void CreateKeyMonster_DeadForNextStep(MonIdx_t i_CreateMonsterIdx);
	void CreateKeyMonster_AliveForGameClear(MonIdx_t i_CreateMonsterIdx);		// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -

	// 2010-04-09 by cmkwon, ����2�� �߰� ����(�ܰ躰 ���� �߰�) - 
	void DeleteKeyMonster_DeadForNextStep(BOOL *o_pbCompensationFlag, MonIdx_t i_DeadMonsterIdx);

	void DeleteKeyMonster_AliveForGameClear(MonIdx_t i_DeadMonsterIdx);			// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -
	BOOL CheckEmptyMonster_DeadForNextStepAlive();
	BOOL CheckEmptyMonster_AliveForGameClearAlive();									// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -
	BOOL CheckKeyMonster_DeadForNextStepAlive();								// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ����ִ� Ű���Ͱ� �ִ��� üũ�Ѵ�.	
	BOOL CheckKeyMonster_AliveForGameClearAlive();									// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -
	void SetInfinityTeamName(CHAR * i_pTeamName);
	void GetPlayerList(VectCFISoc * o_pVectPlayer, ClientIndex_t * o_pMasterUserClientIdx=NULL);	// 2010-04-06 by cmkwon, ����2�� �߰� ���� - 
	INT GetPlayerListSize();
	INT	GetMaxPlayerSize();
	CFieldIOCPSocket * GetMasterPlayer();
	ClientIndex_t GetMasterPlayerClientIdx();
	CHAR * GetMasterPlayerName();
	InfinityCreateUID_t GetInfinityCreateUID();
	eINFINITY_MODE GetInfinityMode();
	CHAR * GetInfinityTeamName();
	eINFINITY_STATE GetInfinityState();
	void SetInfinityState(eINFINITY_STATE i_infiState);		// 2010-04-05 by cmkwon, ���� ������ ī�� ���� �ý��� ���� - 
	BYTE GetInfluenceType();
	MapIndex_t GetInfinityMapIndex();
	ChannelIndex_t GetInfinityChannelIndex();
	Err_t JoinCheck(CFieldIOCPSocket * i_pJoinRequestPlayerSoc);		// ���� ������ �������� üũ
	Err_t Join(CFieldIOCPSocket * i_pJoinPlayerSoc);					// ���� ����
	Err_t ChangeMasterUser(CFieldIOCPSocket * i_pChangeMasterUserFISoc);
	Err_t Leave(CFieldIOCPSocket * i_pLeavePlayerSoc);					// ���� Ż��
	Err_t Ban(CFieldIOCPSocket * i_pBanPlayerSoc);						// ���� �߹�
	Err_t Start(CFieldIOCPSocket * i_pMasterPlayerSoc, CFieldMapChannel *i_pFMChann);					// ����
	Err_t CreateMap(CFieldMapChannel * i_pInfinityMapChannel);

	// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
	Err_t ChangeDifficultyLevel ( CFieldIOCPSocket * i_pMasterPlayerSoc , const INT a_ChangeDifficultyLevel );			// ���� ���̵� ����. 2010. 05. 24. by hsLee.

	BOOL SetDifficultyLevel ( INT DifficultyLevel );																	// ���� ���̵� �� ����. 2010. 05. 25. by hsLee.

	inline INT GetDifficultyLevel ( void ) { return m_InfinityDifficultyLevel; }										// ���� ���̵� �� ����.	2010. 05. 25. by hsLee.
	// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )

	// 2010. 07. 27 by hsLee ���Ǵ�Ƽ 2�� ���� ��� �ó׸� ���� ��ŵ ó��.
	Err_t SkipEndingCinema ( CFieldIOCPSocket * i_pPlayerSoc , const bool a_bNormalEnding = false );															// ���Ǵ�Ƽ ���� ���� ��ŵ ó��.
	// End 2010. 07. 27 by hsLee ���Ǵ�Ƽ 2�� ���� ��� �ó׸� ���� ��ŵ ó��.

	// 2010. 06. 04 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (���� - Ű ���� ����Ʈ ��� ������ ��ȯ ī��Ʈ���� �뷱�� ����.)
	void SetUpdate_KeyMonsterSummonCount( void );																		// ���� ��ϵ� Ű ���� ��ȯ Count Ƚ�� ���̵� ����. 
	// End 2010. 06. 04 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (���� - Ű ���� ����Ʈ ��� ������ ��ȯ ī��Ʈ���� �뷱�� ����.)

	void Fin(BOOL i_bSuccess, INFINITY_FIN_t i_finType);			// ����
	BOOL CheckDestory();				// �ı� üũ
	BOOL CheckAllReady();
	
	BOOL CheckTenderItemSize();			// Tender Item�� �ִ���.
	Err_t ChoiceTenderItem(DiceCnt_t *o_pDiceResult, ClientIndex_t i_UserClientIdx, UINT i_ItemFieldIndex, BOOL i_bGiveUp);	// ���� Tender
	void CalcLimitTimeByUserDeath(char * i_szCharacterName);		// 2009-09-09 ~ 2010-01-13 by dhjin, ���Ǵ�Ƽ - ���� ���� �̸� ���� ���� �߰�,	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ����� �г�Ƽ �߰�
	void CalcAliveForGameClearMonsterHPByUserDeath(char * i_szCharacterName);		// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -

	void LogInfinityFinAliveKeyMonster();										// 2009-09-09 ~ 2010-01-13 by dhjin, ���Ǵ�Ƽ - ���� ������ ���� ���� ���� ���� �α� ������ ���� ���� �� ���� ���� ����	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� ���� ���� 
	void LogInfinityFinUser(CFieldIOCPSocket * i_pPlayerSoc, BOOL i_bClear);	// 2009-09-09 ~ 2010-01-13 by dhjin, ���Ǵ�Ƽ - ���� ������ ���� ���� ���� ���� �α� ������ ���� ���� �� ���� ���� ���� // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� ���� ���� 
	void LogInfinityLeave(CFieldIOCPSocket * i_pPlayerSoc);							// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�, ���� Ż��� ������ ���� ���� 

	void SendInfinityTeamChat(BYTE * i_pDATA, int i_nSize);							// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ä��
	
	void ReStartDisConnectUser(CFieldIOCPSocket * i_pUserFISoc);	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó�� ����
	void SetDeleteCinemaInfo(CINEMAINFO * i_pDeleteCinemaInfo);		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��
	void SendDeleteCinemaInfoList(CFieldIOCPSocket * i_pReStartPlayerSoc);		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��

	void AutoPartyCreate();						// 2009-09-09 ~ 2010-01-26 by dhjin, ���Ǵ�Ƽ - �ڵ� ��� 
	void SendAddDataPlayerList(BYTE* pData, int nSize);		// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -
	void ProcessingCinemaFinSuccess();		// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - ���� ���� ���� ����
	void ProcessingCinemaFinFail();			// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - ���� ���� ���� ����
	BOOL DoSecondlyWorkInfinityBaseIsContinue();		// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -

	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-23 by cmkwon, ���Ǵ�Ƽ ���� ĳ�� ������ ���� - 
	Err_t SendFtoA_INFINITY_START_CHECK(void);
	BOOL CheckInfinityStartCheckAck(eINFINITY_STATE *o_pInfiRoomState, MSG_FtoA_INFINITY_START_CHECK_ACK *i_pStartCheckAck);
	BOOL CheckInfinityAllPlayerState(eINFINITY_STATE i_infiState);
	CFieldIOCPSocket *GetPlayerInVectPlayerByClientIndex(ClientIndex_t i_cliIdx);
	void SetAllPlayerState(eINFINITY_STATE i_infiState);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-26 by cmkwon, ����2�� ���� - CFieldIOCP �������� �߰�
	static CFieldIOCP		*ms_pFieldIOCP4;

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-05 by cmkwon, ���� ������ ī�� ���� �ý��� ���� - 
	BOOL ImputeInfinity(void);

	///////////////////////////////////////////////////////////////////////////////	
	// 2010-04-06 by cmkwon, ����2�� �߰� ���� - 
	int GetPlayerStateCount(eINFINITY_STATE i_infiState);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-09 by cmkwon, ����2�� �߰� ����(�ܰ躰 ���� �߰�) - 
	KEY_MONSTER_INFO *GetKeyMonsterInfoFromKeyMonsterDeadForNextStep(MonIdx_t i_MonNum);
	BOOL DeleteKeyMonsterInfoFromKeyMonsterDeadForNextStep(MonIdx_t i_MonNum);

	CTenderItemInfo *GetTenderItemInfoNoLock(UINT i_ItemFieldIndex);
	BOOL PickupTenderItem(CTenderItemInfo *i_pTenderItemInfo, VectCFISoc *i_pPlayerList);
	BOOL CancelTenderItemInfo(CTenderItemInfo *i_pTenderItemInfo);
	Err_t PushTenderItem(CTenderItemInfo *i_pTenderItemInfo);
	void TenderProcessing(void);

	// ��ϵǾ� �ִ� ���� �����۵��� ���� ���� �ð��� �ϰ� ����.
	void UpdateTenderItemInfoAll_TenderStartTick ( ATUM_DATE_TIME sUpdateTime );

	// start 2011-04-08 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� �ܰ躰 ��� ����
	eINFINITY_STATE GetInfinityStatePlayingNext(eINFINITY_STATE Now);		// ���� Playing Step
	// end 2011-04-08 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� �ܰ躰 ��� ����

	void MoveAllPlayer(D3DXVECTOR3 vecPostion);		// 2011-06-01 ���Ǵ�Ƽ 3�� ���� 7 - ĳ���� ��ġ �̵�

	BOOL CheckInfinityMinAdmission();		// 2012-07-17 by hskim, ���Ǵ�Ƽ �ο��� �����Ҵ� START �Ұ�� ���� ó��

protected:
	// start 2011-05-30 by hskim, ���Ǵ�Ƽ 3�� - �÷��� �ð� �缳�� ��� 
	void EnablePenaltyApply();			// �г�Ƽ ����
	void DisablePenaltyApply();			// �г�Ƽ ���� ����
	// end 2011-05-30 by hskim, ���Ǵ�Ƽ 3�� - �÷��� �ð� �缳�� ��� 
	
	BOOL IsPenaltyApply();
	// end 2011-05-30 by hskim, ���Ǵ�Ƽ 3�� - �÷��� �ð� �缳�� ��� 

public :	// Virtual Process.

	// 2010. 06. 04 by hsLee ��Ƽ�Ǵ� �ʵ� 2�� ���̵� ����. (GM ��ɾ� �߰�. /nextscene(���� �ó׸� �� ȣ��.) )
	virtual void ProcessingCinema ( CFieldIOCPSocket * i_pMasterFISoc , int nUpdateScene = 1 ) {}

	// 2010. 07. 27 by hsLee ���Ǵ�Ƽ 2�� ���� ��� �ó׸� ���� ��ŵ ó��.
	virtual void ResetValue_SkipEndingCinema ( void );						// �ó׸� ��ŵ ���� ���� �ʱ�ȭ.
	virtual bool SetValue_SkipEndingCinema ( const CHAR *pszRqUserName , const bool a_bNormalEnding );			// �ó׸� ��ŵ Ȱ��ȭ & ��û �������� ����.
	// End 2010. 07. 27 by hsLee ���Ǵ�Ƽ 2�� ���� ��� �ó׸� ���� ��ŵ ó��.

	// 2010-06-25 by shcho, ���Ǵ�Ƽ ���÷α� ��� - ���� ������ ���� DB����
	BOOL SendInfinityItemTender( CFieldIOCPSocket* i_pBidderFISoc, DiceCnt_t i_DiceCount, INT i_ItemNum );
	// END 2010-06-25 by shcho, ���Ǵ�Ƽ ���÷α� ��� - ���� ������ ���� DB���� 

	void CheckInfinityPartyState();		// 2013-05-16 by jhseol, ���� ������ �� ��Ƽ����

protected:
	mtVectCFISoc			m_mtVectPlayer;
	CFieldIOCPSocket	*	m_MasterPlayerSoc;
	CFieldMapChannel	*	m_pInfinityMapChannel;
	VectKeyMonsterInfo		m_KeyMonsterDeadForNextStepList;	// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - �̸����� m_KeyMonsterList -> m_KeyMonsterDeadForNextStepList
	KEY_MONSTER_INFO		m_KeyMonsterAliveForGameClear;	// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - �ش� ���Ͱ� �� ������ ���� ����~

	BYTE					m_InfluenceType;
	eINFINITY_STATE			m_eInfinityState;
	INFINITY_MODEINFO		m_InfinityModeInfo;
	vectorCinemaInfo		m_CinemaInfoList;

	// start 2011-04-08 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� �ܰ躰 ��� ����
	vectorCinemaInfo		m_CinemaStepInfoList[CINEMA_PLAYING_STEP_MAX];

	DWORD					m_InfinityStepStartTick;	// 2011-04-11 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� �ܰ躰 ��� ���� - ���� ������ ���� �ð�
	INT						m_PlayingStepCount;			// 2011-04-11 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� �ܰ躰 ��� ���� - ���� �������� Playing Step
	// end 2011-04-08 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� �ܰ躰 ��� ����

	InfinityCreateUID_t		m_nInfinityCreateUID;
	CHAR					m_szTeamName[SIZE_MAX_PARTY_NAME];
	DWORD					m_InfinityCreatedTick;
	ATUM_DATE_TIME			m_InfinityCreatedTime;
	CinemaOrder_t			m_CinemaOrder;
	DWORD					m_dwForcedTick;
	mtvectTenderItemInfo	m_mtTenderItemList;
	VectTenderUserInfo		m_TenderUserInfoList;
	CRITICAL_SECTION		m_criticalLimitTime;		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ����� �г�Ƽ �߰�
	MSec_t					m_InfinityLimitTick;		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ����� �г�Ƽ �߰�
	vectorCinemaInfo		m_DeleteCinemaInfoList;		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ñ� ���� ������ ó��
	vectorCinemaInfo		m_FinSuccessCinemaInfoList; // 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -
	vectorCinemaInfo		m_FinFailCinemaInfoList;	// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -
	DWORD					m_dwTickChangedInfinityState;	// 2010-04-05 by cmkwon, ���� ������ ī�� ���� �ý��� ���� - 
	INT						m_InfinityDifficultyLevel;		// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ���� ���̵� ������.
	MONSTER_BALANCE_DATA	m_InfinityDifficultyData;		// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ���� ���̵� ����.
	eINFINITY_SKIP_CINEMA	m_eCinemaSkipState;											// �ó׸� ���� ��ŵ Ȱ��ȭ��. 2010. 07. 27 by hsLee ���Ǵ�Ƽ 2�� ���� ��� �ó׸� ���� ��ŵ ó��.
	CHAR					m_szRqSkipEndingCinema_UserName[SIZE_MAX_CHARACTER_NAME];	// �ó׸� ���� ��ŵ ��û�� ���� �̸�. 2010. 07. 27 by hsLee ���Ǵ�Ƽ 2�� ���� ��� �ó׸� ���� ��ŵ ó��.
	ATUM_DATE_TIME			m_sCinemaSkipTime;											// �ø޳� ���� ��ŵ�� �ð�.

	BOOL					m_bPenaltyApply;		// 2011-05-30 by hskim, ���Ǵ�Ƽ 3�� - �÷��� �ð� �缳�� ��� 
};

#endif // !defined(AFX_INFINITYBASE_H__679CAEA9_97E4_46FF_AD8F_2808F8C05E3E__INCLUDED_)
