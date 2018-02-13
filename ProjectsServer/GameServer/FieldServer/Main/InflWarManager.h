// InflWarManager.h: interface for the CInflWarManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFLWARMANAGER_H__35B2A97C_7399_40C5_AAC7_E19160CDE41B__INCLUDED_)
#define AFX_INFLWARMANAGER_H__35B2A97C_7399_40C5_AAC7_E19160CDE41B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumProtocol.h"

typedef mt_vector<SINFLBOSS_MONSTER_SUMMON_DATA>	mtvectSummonMonsterData;
typedef	mt_vector<SSUMMON_STRATEGYPOINT_INFO>		mtvectorSSUMMON_STRATEGYPOINT_INFO;
typedef	mt_vector<SSTRATEGYPOINT_SUMMONTIME_INFO>	mtvectSSTRATEGYPOINT_SUMMONTIME_INFO;
typedef mt_vector<SSTRATEGYPOINT_DAY>				mtvectSSTRATEGYPOINT_DAY;
typedef	mt_vector<SSTRATEGYPOINT_NOT_SUMMON_TIME>	mtvectSSTRATEGYPOINT_NOT_SUMMON_TIME;
typedef	mt_vector<STELEPORT_INFO_BUILDING>			mtvectSTELEPORT_INFO_BUILDING;
typedef	mt_vector<MSWARINFO_DISPLAY>				mtvectMSWARINFO_DISPLAY;				// 2008-03-26 by dhjin, ���� ���� ǥ�� ��ȹ�� - 
typedef mt_vector<SDECLARATION_OF_WAR>				mtvectSDECLARATION_OF_WAR;				// 2009-01-12 by dhjin, ���� ����

//////////////////////////////////////////////////////////////////////////
// 2012-11-13 by jhseol, ���� �ý��� ������ - ������
typedef	mt_vector<SRENEWAL_STRATEGYPOINT_SUMMON_TIME>	mtvectSRENEWAL_STRATEGYPOINT_SUMMON_TIME;
typedef mt_vector<SSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO> mtvectSSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO;
// end 2012-11-13 by jhseol, ���� �ý��� ������ - ������

struct SSTRATEGYPOINT_SUMMONTIME_INFO_BY_SUMMONCOUNT: binary_function<SSTRATEGYPOINT_SUMMONTIME_INFO, SSTRATEGYPOINT_SUMMONTIME_INFO, bool>
{// 2009-03-23 by dhjin, ��������Ʈ �ð� ���� ���� -
	bool operator()(SSTRATEGYPOINT_SUMMONTIME_INFO param1, SSTRATEGYPOINT_SUMMONTIME_INFO param2)
	{
        return param1.SummonCount > param2.SummonCount;		// ���� ���� ������ ���� �ݴ�� ��
    };
};

class CFieldIOCP;
class CFieldIOCPSocket;
class CInflWarData:public SINFLUENCE_WAR_DATA
{
public:
	CInflWarData();
	~CInflWarData();

	mt_lock *GetmtlockPtr(void);
	void InitCInflWarData(SINFLUENCE_WAR_DATA *i_pInflWarData, vectDB_INFLUENCE_WAR_INFO *i_pInflWarInfoList);
	void SetCurrentStepInflWarData(INT i_nBossMonsterUID, byte i_byInfl);	// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - ���ڰ� ���� i_nWartimeStage => i_nBossMonsterUID	// 2009-01-12 by dhjin, ���� ���� - ���� �� �ܰ�� m_pCurInflWarInfo�� �����Ѵ�.

	int CalcWartimeStage(int i_nContibutionPoint);
	SDB_INFLUENCE_WAR_INFO *GetInflWarInfoByWartimeStage(int i_nWartimeStage);
	SDB_INFLUENCE_WAR_INFO *GetInflWarInfoByKillBossMonsterUID(int i_nBossMonsterUID, byte i_byInfl);		// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� 
	SDB_INFLUENCE_WAR_INFO *GetInflWarInfoByContributionPoint(int i_nContributionPoint);
	SDB_INFLUENCE_WAR_INFO *GetCurInflWarInfo(void);
	void SetWartimeStage(SDB_INFLUENCE_WAR_INFO *i_pInflWarInfo);
	BOOL IsSummonJacoMonster(void);
	void SetSummonJacoMonster(BOOL i_bSummon);

	float fInflHPRepairRate;			// 2009-03-31 by dhjin, ���� ���� - ������ �¸� �� ���� �Ǵ� �̺�Ʈ ����
	float fInflDPRepairRate;			// 2009-03-31 by dhjin, ���� ���� - ������ �¸� �� ���� �Ǵ� �̺�Ʈ ����
	float fInflSPRepairRate;			// 2009-03-31 by dhjin, ���� ���� - ������ �¸� �� ���� �Ǵ� �̺�Ʈ ����

protected:
	mt_lock			m_mtlock;
	vectDB_INFLUENCE_WAR_INFO	*m_pInflWarInfoList;
	SDB_INFLUENCE_WAR_INFO		*m_pCurInflWarInfo;

	BOOL			m_bSummonJacoMonster;				// 2006-04-20 by cmkwon
};

//////////////////////////////////////////////////////////////////////////
// 2008-03-26 by dhjin, ���� ���� ǥ�� ��ȹ�� - 
class CMSWarInfoDisPlay
{
public:
	CMSWarInfoDisPlay();
	~CMSWarInfoDisPlay();
	
	void InitMSWarInfoDisPlay();
	void InitMSWarOptionType(BYTE i_byLeaderInfluence);
	void MSWarInfoDisPlayClear(BYTE i_byMonsterInfluence);
	
	void AllGetMSWarInfoDisPlay(vectMSWARINFO_DISPLAY * o_VectMSWarInfoDisPlay);
	
	void SetMSWarInfoDisPlay(MSWARINFO_DISPLAY * i_pMSWARINFO_DISPLAY);
	void SetMSWarInfoDisPlayByHPRate(MapIndex_t i_nMapindex, INT i_nHPRate);
	void SetMSWarInfoDisPlayByCreateTeleport(MapIndex_t i_nMapindex, INT i_nHPRate, INT i_nTelePortState, BYTE i_byBelligerence);
	void SetMSWarInfoDisPlayByChangeTeleportState(MapIndex_t i_nMapindex, INT i_nHPRate, INT i_nTelePortState, ATUM_DATE_TIME * i_pTelePortBuildingStartTime = NULL, ATUM_DATE_TIME * i_pTelePortBuildingEndTime = NULL);
	void SetMSWarOptionType(BYTE i_byLeaderInfluence, SHORT i_byMSWarOptionType);
	SHORT GetMSWarOptionType(BYTE i_byLeaderInfluence);

protected:
	mtvectMSWARINFO_DISPLAY		m_mtvectMSWARINFO_DISPLAY;
	SHORT						BCUMSWarOptionType;
	SHORT						ANIMSWarOptionType;
};

//////////////////////////////////////////////////////////////////////////
// 2015-05-01 by killburne
class CStrategicPointDisplay
{
public:
	CStrategicPointDisplay();
	~CStrategicPointDisplay();

	void InitStrategicPointDisplay();

	void GetAllStrategicPointDisplay(vector<SSTRATEGYPOINT_DISPLAY_INFO>* o_VectStrategicPointDisplay);
	bool GetStrategicPointDisplay(MapIndex_t mapIndex, SSTRATEGYPOINT_DISPLAY_INFO* o_StrategicPointDisplay);

	void RemoveStrategicPointDisplay(MapIndex_t i_nMapindex);
	void SetStrategicPointDisplay(SSTRATEGYPOINT_DISPLAY_INFO* i_pSTRATEGICPOINT_DISPLAY);
	void SetStrategicPointDisplay(SSUMMON_STRATEGYPOINT_INFO* i_pSTRATEGICPOINT_INFO);
	void SetStrategicPointDisplayHP(MapIndex_t i_nMapindex, FLOAT i_nHPRate);
protected:
	mt_vector<SSTRATEGYPOINT_DISPLAY_INFO>		m_mtvectSTRATEGICPOINT_DISPLAY;
};

//////////////////////////////////////////////////////////////////////////
// 2009-01-12 by dhjin, ���� ����
class CDeclarationOfWar
{
public:
	CDeclarationOfWar();
	~CDeclarationOfWar();

	void InitDeclarationOfWar();
	BOOL SetDBDeclarationOfWar(mtvectSDECLARATION_OF_WAR * i_pVectDeclarationOfWar, SDECLARATION_OF_WAR_FORBID_TIME * i_pForbidTime);
	void SetBeforeWarStartTime(INT i_nBCUBeforeMin, INT i_nANIBeforeMin);			// ����ܰ�� ���� ������ ���� �ð��� �����Ѵ�.
	BYTE GetCurrentMSWarStep(BYTE i_byInfl);			// ���� ���� �ܰ� �� ��������
	BYTE GetCurrentMSWarEndState(BYTE i_byInfl);			// ���� ���� ���� ���� �� ��������
	void GetBeforeMSWarEndInfo(BYTE i_byInfl, INT * o_nMSBossMonsterUID, BYTE * o_MSWarEndState);	// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - ���ֱ� �� ���� ��������
	INT GetCurrentMSUID(BYTE i_byInfl);		// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - ���� �ֱ� ��ȯ�� ��UID�� ��������
	void SetMSWarStepByMSWarStart(SDECLARATION_OF_WAR * i_pDeclarationOfWar);			// ���� ���� �ܰ� �� ����
	void SetMSWarEndState(BYTE i_byInfl, INT i_nEndState);		// MSWarEndState�� ����
	Err_t SetSelectWarTime(MSG_FC_INFO_DECLARATION_MSWAR_SET * i_pMsg, BYTE i_byInfl, BYTE * o_bySelect, ATUM_DATE_TIME * o_MSWarStartTime);		// �ð� ����
	void SetMSWarFinish(BYTE i_bySummonInfl, BYTE i_byWinInfl, ATUM_DATE_TIME * i_pEndTime);			// ���� ���� �� �� ����
	void SetNewMSWar();				// ���� ���� �� ����

	// 2009-07-02 by cmkwon, �������� ���� ���� ���� - ���� �߰�
	BOOL CheckNextMSWarStep(ATUM_DATE_TIME * i_pCurrentTime);		// ���� ���� �ֱ����� Ȯ��
	SDECLARATION_OF_WAR * FindNextMSWarStep(SDECLARATION_OF_WAR *i_pCurWarStep);		// 2009-07-02 by cmkwon, �������� ���� ���� ���� - 
	SDECLARATION_OF_WAR * FindMSWarStep(BYTE i_byInfl, BYTE i_byMSWarStep);				// 2009-07-02 by cmkwon, �������� ���� ���� ���� - 
	SDECLARATION_OF_WAR * FindMSWarStepByCurrentTime(BYTE i_byInfl);					// 2009-07-02 by cmkwon, �������� ���� ���� ���� - 
	BYTE GetMostMSWarStepOfWinStep(BYTE i_byInfl, int *o_pnBossMonNum);					// 2009-07-02 by cmkwon, �������� ���� ���� ���� - 

	BOOL CheckBCUMSWarBeforeStartTime(ATUM_DATE_TIME * i_pCurrentTime);		// ������ ���� �ð����� üũ
	BOOL CheckANIMSWarBeforeStartTime(ATUM_DATE_TIME * i_pCurrentTime);		// ������ ���� �ð����� üũ
	BOOL CheckBCUMSWarStartTime(ATUM_DATE_TIME * i_pCurrentTime);		// ���� ���� �ð����� üũ
	BOOL CheckANIMSWarStartTime(ATUM_DATE_TIME * i_pCurrentTime);		// ���� ���� �ð����� üũ
	BOOL CheckForbidTime(ATUM_DATE_TIME * i_pHopeTime);					// ���� ���� �ð����� �������� �ð����� üũ
	INT	 MakeMSG_FC_INFO_DECLARATION_MSWAR_INFO_OK(SDECLARATION_OF_WAR * o_pDeclarationOfWarInfo, SDECLARATION_OF_WAR_FORBID_TIME * o_pForbidTime);	// Ŭ���̾�Ʈ ���� �� ���� ���� ���� �����
	BOOL GetOwnerInflOfConflictArea(BYTE i_byInfl);						// ���� �������� ���� �������� üũ
	BYTE GetLastMSWinStep(BYTE i_byInfl);		// ���� ���� �¸� �ܰ踦 �����´�.
	BOOL CheckSPSummonPossibleTime();			// 2009-03-23 by dhjin, ��������Ʈ �ð� ���� ���� - �� �ð��� ��������Ʈ�� ��ȯ �Ǿ� ���� �ʰ� üũ

	BOOL CheckMSWarTime(USHORT i_Year, BYTE i_Month, BYTE i_Day);	// 2012-11-13 by jhseol, ���� �ý��� ������ - ������, ���� ������ �ִ��� üũ
	ATUM_DATE_TIME GetMSWarStartTime(BYTE i_nInfluenceType);	// 2013-02-26 by jhseol, ���� �ý��� ������ �ΰ��ɼ� - ���� ���� �ð� ��������


private:
	mtvectSDECLARATION_OF_WAR	m_mtvectSDECLARATION_OF_WAR;
	SDECLARATION_OF_WAR	*		m_pCurrentBCUWarStepInfo;
	SDECLARATION_OF_WAR	*		m_pCurrentANIWarStepInfo;
	ATUM_DATE_TIME				m_pBeforeBCUWarStartTime;
	ATUM_DATE_TIME				m_pBeforeANIWarStartTime;
	SDECLARATION_OF_WAR_FORBID_TIME			m_ForbidTime;
};

class CInflWarManager  
{
	friend class CAtumLogSender;		// 2008-04-01 by dhjin, ����, ������ ����â ��ȹ�� - 
public:
	CInflWarManager();
	~CInflWarManager();

	void InitCInflWarManager(CFieldIOCP *i_pFIOCP);

	void OnMinutelySPManager(ATUM_DATE_TIME *pDateTime);

	void OnDoMinutelyWorkInflWarManager(ATUM_DATE_TIME *pDateTime);
	void OnInfluenceBossMonsterDeadInflWarManager(MONSTER_INFO *pMonInfo);
	void OnInfluenceBossMonsterAutoDestroyedInflWarManager(MONSTER_INFO *pMonInfo);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-13 by cmkwon, InfluenceWarInfo ����
	vectDB_INFLUENCE_WAR_INFO *GetInflWarInfoByInflType(BYTE i_byInflTy);
	vectDB_INFLUENCE_WAR_INFO *GetVCNInflWarInfo(void);
	vectDB_INFLUENCE_WAR_INFO *GetANIInflWarInfo(void);
	BOOL InsertInfluenceWarInfo(SDB_INFLUENCE_WAR_INFO *i_pInflWarInfo);


	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-13 by cmkwon, InfluenceWarData ����
	CInflWarData *GetInflWarDataByInflType(BYTE i_byInflTy);
	BOOL GetINFLUENCE_WAR_INFO_ByInflType(SDB_INFLUENCE_WAR_INFO **o_ppInflWarInfo, BYTE i_byInflTy);
	BOOL InitInflWarData(SINFLUENCE_WAR_DATA *i_pInflWarData);
	void SetCurrentStepInflWarDataW();				// 2009-01-12 by dhjin, ���� ���� - ���� �� �ܰ�� m_pCurInflWarInfo�� ���� �� ������ð��� �����Ѵ�.
	int GetContributionPoint(BYTE i_byInflTy);
	BOOL AddContributionPoint(BYTE i_byInflTy, INT i_nAddValue);
	BYTE GetDominatingInflByContributionPoints();	// 2015-12-17 Future, retrieval of the dominating nation using the NCP

	float GetHPRepairAddRateByInflTy(BYTE i_byInflTy);
	float GetDPRepairAddRateByInflTy(BYTE i_byInflTy);
	float GetSPRepairAddRateByInflTy(BYTE i_byInflTy);
	// 2007-10-06 by cmkwon, �������� 2���� ȣĪ�� �ٸ��� ���� - ���� ���� (, BOOL i_bSubLeader/*=FALSE*/)
	BOOL IsLeader(BYTE i_byInflTy, UID32_t i_charUID);
	void GetAllLeaderFISocket(BYTE i_byInflTy, CFieldIOCPSocket **i_ppLeaderFISoc, CFieldIOCPSocket **i_ppSubLeader1FISoc, CFieldIOCPSocket **i_ppSubLeader2FISoc);
	void SetSummonJacoMonsterW(BYTE i_byInflTy, BOOL i_bSummon);
	BOOL IsSubLeader1(BYTE i_byInflTy, UID32_t i_charUID);		// 2007-10-06 by cmkwon, �������� 2���� ȣĪ�� �ٸ��� ���� -
	BOOL IsSubLeader2(BYTE i_byInflTy, UID32_t i_charUID);		// 2007-10-06 by cmkwon, �������� 2���� ȣĪ�� �ٸ��� ���� -
	BOOL SetLeader8SubLeaderRACE(CHARACTER *io_pCharacter);		// 2007-10-06 by cmkwon, �������� 2���� ȣĪ�� �ٸ��� ���� - 

	BOOL GetSummonInflBossMonsterData(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA *o_pSummonData, BYTE i_byInflTy);
	BOOL GetVCNSummonedInflBossMonsterData(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA *o_pSummonData);
	BOOL GetANISummonedInflBossMonsterData(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA *o_pSummonData);
// 2007-05-17 by cmkwon, �Լ� �������� ��ġ ����
//	void OnInfluenceBossMonsterAutoDestroyedInflWarManager(BOOL i_bVCNInflTyForInit);
	
	BOOL SetSubLeader(BYTE i_byInflTy, UID32_t i_CharacterUID, CHAR * i_pCharacterName, BYTE SubLeaderNum);		// 2007-02-14 by dhjin, �������� ����
	
	void InsertSummonStrategyPointInfo(SSUMMON_STRATEGYPOINT_INFO *i_pSSUMMON_STRATEGYPOINT_INFO);	// 2007-02-27 by dhjin
	void DeleteSummonStrategyPointInfo(MAP_CHANNEL_INDEX i_MapChannel);								// 2007-02-27 by dhjin
	void DeleteSummonStrategyPointInfoByInfluenceType(BYTE i_InfluenceType);						// 2007-02-27 by dhjin
	BOOL CheckSummonStrategyPointInfoByMapIndex(INT i_nMapIndex);						// 2007-10-06 by dhjin, ���� ��������Ʈ�� �����ִ��� üũ
	void UpdateSummonStrategicPointInfoHP(MapIndex_t i_nMapindex, FLOAT i_nHPRate);
	void GetAllSummonStrategicPointInfo(vector<SSTRATEGYPOINT_DISPLAY_INFO>* o_VectStrategicPointDisplay);

	mtvectorSSUMMON_STRATEGYPOINT_INFO		m_mtvectSummonStrategyPointInfo;			// 2007-02-23 by dhjin, ��������
	
	//////////////////////////////////////////////////////////////////////////
	// 2012-11-13 by jhseol, ���� �ý��� ������ - ������
	void InitRenewalStrategyPointSummonTime(vectSRENEWAL_STRATEGYPOINT_SUMMON_TIME *i_pvectRenewalSStrategypointSummonTime);
	mtvectSRENEWAL_STRATEGYPOINT_SUMMON_TIME * GetRenewalStrategyPointSummonTime();
	void InitRenewalStrategyPointSummonMapInfo(vectSSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO *i_pvectStrategyPointMapInfluenceInfo);
	mtvectSSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO * GetRenewalStrategyPointSummonMapInfo();
	// end 2012-11-13 by jhseol, ���� �ý��� ������ - ������

	// 2012-11-26 by jhseol, ���� �ý��� ������ - ����� Ʈ���� ��������(�Ͻ�����), ������ ���������� Ȯ���ϴ� �Լ�
	void SetDoingInfluenceWar();
	BOOL IsDoingInfluenceWar();
#ifdef MS_TAKE_NO_DAMAGE
	BOOL IsDoingStrategyPointWar();
	UINT BlockDameMS();
	UINT SYS_H = 0;
	UINT SYS_M = 0;
	UINT MS_SAFE_TIME = 30;
	UINT BUFF_M = 0;
	UINT MSwar_H = 0;
	UINT MSwar_M = 0;
	UINT m_calculation(int new_h, int new_m, int old_h, int old_m);
#endif
	// end 2012-11-26 by jhseol, ���� �ý��� ������ - ����� Ʈ���� ��������(�Ͻ�����), ������ ���������� Ȯ���ϴ� �Լ�

	// 2007-02-28 by dhjin, ��������Ʈ ���� ���� �ֱ�
	mtvectSSTRATEGYPOINT_SUMMONTIME_INFO * GetStrategyPointSummonTimeInfo(void);
	SSTRATEGYPOINT_SUMMON_RANGE * GetStrategyPointSummonRange(void);
	mtvectSSTRATEGYPOINT_NOT_SUMMON_TIME * GetStrategyPointNotSummonTime(void);	
	void InitStrategyPointSummonTimeInfo(vectSSTRATEGYPOINT_SUMMONTIME_INFO *i_pvectSStrategypointSummonTimeInfo);
	void InitStrategyPointSummonRange(SSTRATEGYPOINT_SUMMON_RANGE *i_pStrategyPointSummonRange);
	void InitStrategyPointNotSummonTime(vectSSTRATEGYPOINT_NOT_SUMMON_TIME *i_pvectSStrategyPointNotSummonTime);
	void InitStrategyPoint();
	void LoadStrategyPointSummonInfo();
	void LoadStrategyPointSummonRange();
	void LoadStrategyPointNotSummonTime();
	BOOL CheckStrategyPointSummon();					// ��������Ʈ ��ȯ ���� üũ 0:��ȯ���� �ʴ´�, 1: ��ȯ ����
	BOOL CompareStrategyPointSummonTime();				// ��������Ʈ ��ȯ �ֱ� ������ �ð� üũ 
	void SetAllStrategyPointSummonTime();				// ��������Ʈ ��ȯ �ð� ���� ������Ʈ 
	void SetAllStrategyPointSummonTimeNew();			// 2009-03-23 by dhjin, ��������Ʈ �ð� ���� ���� - ��������Ʈ ��ȯ �ð� ���� ������Ʈ 
	INT SetSPPossibleTimeInfo(ATUM_DATE_TIME *i_pSTime, ATUM_DATE_TIME *i_pETime);						// 2009-03-23 by dhjin, ��������Ʈ �ð� ���� ���� - ��������Ʈ ��ȯ ������ �ð��� ���� ����
	void SetSPSummonCountInfo(BYTE *o_pMaxSPSummonCountByMapIndex, BYTE *o_pTotalSPSummonCount);		// 2009-03-23 by dhjin, ��������Ʈ �ð� ���� ���� - �� �ֱ⿡ ��ȯ �Ǿ�� �Ǵ� �� �� ���� �� ���� ����Ѵ�.
	void SortBySPSummonCount();		// 2009-03-23 by dhjin, ��������Ʈ �ð� ���� ���� - �ʴ� ��ȯ �Ǵ� ���� ���� ���� �� ������ �����Ѵ�.
	void CalcSPSummonTime(INT i_nSPStepMin, INT i_nSPPMin, ATUM_DATE_TIME *o_pSPTime, BOOL *o_bCheckSummon);		// 2009-03-23 by dhjin, ��������Ʈ �ð� ���� ���� - ��������Ʈ ��ȯ �ð� ���
	BOOL GetNextSPSummonTimeByALLSPSummonTimeInfo(INT i_nMapindex, ATUM_DATE_TIME *o_pSummonTime, BOOL *o_bCheckSummon);	// 2009-03-23 by dhjin, ��������Ʈ �ð� ���� ���� - ���� ��������Ʈ ��ȯ�Ǵ� ���� ������
	BOOL CheckSPSummonMapIndex(INT i_nMapIndex);	// 2009-03-23 by dhjin, ��������Ʈ �ð� ���� ���� - �ش� �ʿ� ��������Ʈ�� ���� �Ǿ� �ִ��� üũ		
	BOOL DeleteALLSPSummonTimeInfoByMapIndex(INT i_nMapindex);		// 2009-03-23 by dhjin, ��������Ʈ �ð� ���� ���� - ��ȯ�� ��������Ʈ ���� ����
	void SetStrategyPointSummonTime(SSTRATEGYPOINT_SUMMONTIME_INFO *i_pStrategyPointSummonTimeInfo, INT i_nSummonHourGap);	// ��������Ʈ ��ȯ �ð� ������Ʈ
	BOOL CheckStrategyPointNotSummonTime(ATUM_DATE_TIME *i_ptmTime, int i_nRange, BOOL *o_bSummonAttribute);		// 2008-04-08 by dhjin, ��ȯ ���� �ð� ���� -  ��ȯ���ɽð�üũ�����߰� // ��������Ʈ ��ȯ ���� �ð� üũ 
	void InitStrategyPointDay(ATUM_DATE_TIME *i_pSTime, ATUM_DATE_TIME *i_pETime);		// ��������Ʈ ���� üũ ���� �ʱ�ȭ
	BOOL CompareStrategyPointDay();					// ��������Ʈ Max ��ȯ ���� ���� �ʴ� ������ �ִ��� ���� üũ 
	void CurrentStrategyPointDay();					// ���� �ð����κ��� Max ��ȯ ���� ���� �ʴ� ���� ����. ���� ������ ����.
	void IncreaseStrategyPointDay(ATUM_DATE_TIME *i_pTime);		// ��������Ʈ ��ȯ ������ �������� ������ ���� ���� 1���� ��Ų��.
	BOOL CompareStrategyPointDayByOneDay(ATUM_DATE_TIME *i_pTime, int *o_nRangeTime);	// ��������Ʈ Max ��ȯ ���� ���� �ʴ� ������ �ִ��� üũ 


	BOOL CheckSummonBossAndStrategyPoint();				// 2007-04-26 by dhjin, �����̳� ��������Ʈ�� ���� �Ǿ� �ִ��� üũ �Լ�
	BOOL CheckSummonBoss();								// 2008-03-28 by dhjin, ���� ���� ǥ�� ��ȹ�� -  
	BOOL CheckSummonOnlyStrategyPoint();				// 2008-12-23 by dhjin, ���� ���� �߰���
	BOOL CheckBossMonsterSummoningByAttackInfl(byte i_byAttackerInfluence);		// 2007-09-05 by dhjin, �������� �������� ��� ���� ������ ��ȯ �Ǿ� �ִ��� üũ
	BOOL CheckBossMonsterSummoningByMapInfluence(INT i_nMapInfluenceType);		// 2008-03-27 by dhjin, ���� ���� ǥ�� ��ȹ�� - �ʼ������� ���� ��ȯ �Ǿ� �ִ��� üũ
	BOOL CheckVCNBossMonsterSummoning();				// 2007-09-05 by dhjin, ���������� ���� ��ȯ �Ǿ� �ִ��� üũ
	BOOL CheckANIBossMonsterSummoning();				// 2007-09-05 by dhjin, �˸��� ���� ��ȯ �Ǿ� �ִ��� üũ

	void GameStartSetLeader_SubLeaderGuildUID();		// 2007-10-06 by dhjin, ������, �������� �������� ���� UID ���� 
	UID32_t GetVCNLeaderGuildUID();						// 2007-09-07 by dhjin, ���������� ������ ���� UID
	UID32_t GetANILeaderGuildUID();						// 2007-09-07 by dhjin, �˸��� ������ ���� UID
	UID32_t GetVCNSubLeader1GuildUID();					// 2007-10-06 by dhjin, ���������� ��������1 ����UID
	UID32_t GetVCNSubLeader2GuildUID();					// 2007-10-06 by dhjin, ���������� ��������2 ����UID
	UID32_t GetANISubLeader1GuildUID();					// 2007-10-06 by dhjin, �˸��� ��������1 ����UID
	UID32_t GetANISubLeader2GuildUID();					// 2007-10-06 by dhjin, �˸��� ��������2 ����UID	
	void SetSubLeaderGuildUID(int i_nSubLeaderRank, byte i_byInfluence, UID32_t i_nCharacterUID);		// 2007-10-06 by dhjin, �������� �������� ���� UID ����
	
	BOOL CheckLeader_SubLeaderGuildUID(UID32_t i_nGuildUID);		// 2007-09-07 by dhjin, ������ �������� üũ

	BOOL InsertTelePortInfoBuing(STELEPORT_INFO_BUILDING * i_pTeleportInfoBuilding);	// 2007-09-15 by dhjin, �ڷ���Ʈ�� ���� ���������� �����ϸ� ������ �����Ͽ� ���� �ð��� üũ�Ѵ�.
	void TelePortInfoBuingByTick(ATUM_DATE_TIME * i_pCurrnetTime);						// 2007-09-15 by dhjin, �ڷ���Ʈ ���� 1�и��� üũ�Ͽ� ���� ����
	void ClearTelePortByInfluenceWarEnd(BYTE i_byDefenceCharInfluence);		// 2009-03-25 by cmkwon, ������ �ڷ���Ʈ �ʱ�ȭ ���� ���� ���� - i_byCharInfluence => i_byDefenceCharInfluence �� ����										// 2007-09-16 by dhjin, ������ ����� �ڷ���Ʈ ������ �ʱ�ȭ�Ѵ�. 
	void DeleteTelePortInfoBuing(MapIndex_t i_mapIdx);		// 2007-09-20 by cmkwon, �ڷ���Ʈ ��ȯ ���� ���� - �ڷ���Ʈ ���� ������ �����Ѵ�.

	MapIndex_t GetTeleportMapIndex(BYTE i_byInfluence);				// 2007-09-16 by dhjin

	void SetPollDate(SPOLLDATE_DB * i_pSPOLLDATE_DB);		// 2007-10-29 by dhjin, DB���� �ε��� ���� ���� ����
	void LoadVoterListByDB();								// 2007-10-29 by dhjin, DB���� �ε� ��ǥ�� ����Ʈ 
	void LoadLeaderCandidateByDB();							// 2007-10-29 by dhjin, DB���� �ε� ������ �ĺ� ����Ʈ
	void InitVoterListByDB(mtvectSVOTER_LIST * i_pvectSVOTER_LIST);			// 2007-10-29 by dhjin, DB���� �ε��� ��ǥ�� ����Ʈ ���� 
	void InitLeaderCandidateByDB(mtvectSLEADER_CANDIDATE * i_pvectSLEADER_CANDIDATE);			// 2007-10-29 by dhjin, DB���� �ε��� ������ �ĺ� ����Ʈ ���� 
	BOOL GetLeaderCandidateInfo(INT i_nLeaderCandidateNum, BYTE i_byInfluence, SLEADER_CANDIDATE *o_pSLEADER_CANDIDATE);		// 2007-10-30 by dhjin, ���õ� ������ �ĺ� ���� �� ��������
	BOOL CheckLeaderCandidateUser(UID32_t i_nAccountUID, UID32_t i_nCharacterUID);			// 2007-10-29 by dhjin, ������ �ĺ��� ��� �Ǿ� �ִ� �������� üũ
	void SendLeaderCandidateList(CFieldIOCPSocket *i_pFISoc);			// 2007-10-30 by dhjin, ������ �ĺ� ����Ʈ ����
	BOOL CheckPollDateByApplication(ATUM_DATE_TIME * i_pCurrentTime);	// 2007-10-30 by dhjin, ������ �ĺ� ��� �Ⱓ���� üũ
	BOOL CheckPollDateByVote(ATUM_DATE_TIME * i_pCurrentTime);			// 2007-10-30 by dhjin, ��ǥ �Ⱓ���� üũ
	BOOL CheckPollDateRange(ATUM_DATE_TIME * i_pCurrentTime);			// 2007-11-20 by dhjin, ���� �Ⱓ���� üũ
	BOOL CheckLeaderelectionDate(ATUM_DATE_TIME * i_pCurrentTime);		// 2007-11-16 by dhjin, ������ ���� �ð� üũ
#ifdef NEMERIAN_AUTO_ELECTION_AND_RESET
	BOOL ElectionDateIsInPast(ATUM_DATE_TIME * i_pCurrentTime);
#endif
	void SendLeaderelectionInfo();										// 2007-11-16 by dhjin, ������ ���� ���� ����

	void InsertLeaderCandidate(SLEADER_CANDIDATE * i_pSLEADER_CANDIDATE);	// 2007-10-30 by dhjin, ������ �ĺ� ���
	void UpdateDeleteLeaderCandidate(INT i_nCharacterUID);		// 2007-10-30 by dhjin, ������ �ĺ� Ż��
	void DeleteLeaderCandidate(INT i_nCharacterUID);			// 2008-06-05 by dhjin, ������ �ĺ� ���� ����Ÿ �� ����, DB���� �������� ���� ����
	BOOL CheckPossibleVoterUser(UID32_t i_nAccountUID);					// 2007-10-31 by dhjin, ��ǥ�� �������� üũ
	BOOL LeaderCandidateVote(INT i_nLeaderCandidateNum, BYTE i_byInfluence, INT i_nPollPoint);		// 2007-10-31 by dhjin, ������ �ĺ����� ��ǥ�� ���(��ǥ������)
	void InsertVoterUser(SVOTER_LIST *i_pSVOTER_LIST);					// 2007-10-31 by dhjin, ��ǥ�� ���� �߰�
	void SendPollDate(CFieldIOCPSocket *i_pFISoc);						// 2007-10-31 by dhjin, ���� ���� ����
	BOOL CheckPossibleDeleteLeaderCandidate(UID32_t i_nCharacterUID);	// 2007-11-02 by dhjin, ������ �ĺ� Ż�� ������ �������� üũ
	BOOL CheckRegLeaderCandidateGuild(UID32_t i_GuildUID);				// 2008-04-02 by dhjin, �������� �������� ������ ��꿡 ������ �ĺ� ���� ���� - ������ �ĺ� ����� �������� üũ

	INT  GetBossWarStrateyPointSummoningCountByInfl(BYTE i_byInfluence);	// 2007-11-06 by dhjin, ���� ����� �����ִ� ��������Ʈ �� üũ�Ͽ� ��ȯ

	void SendCityInfoInfluence(CFieldIOCPSocket *i_pFISoc);				// 2007-12-03 by dhjin, ���� ���� �� ���� ������ ������.

	//////////////////////////////////////////////////////////////////////////
	// 2008-03-26 by dhjin, ���� ���� ǥ�� ��ȹ�� -  
	void InsertMSWarInfoByMSWarStart(MapIndex_t i_nStrategypointMapindex, INT i_nStrategypointMapInfluence, BYTE i_byStrategypointBelligerence);		// 2008-03-26 by dhjin, ��������Ʈ ��ȯ�� ������ �����Ѵ�.
	void SendMSWarInfoForAllUser();					// 2008-03-27 by dhjin, ���� ���� ǥ�� ��ȹ�� - ���� ���� ���� 
	void SendMSWarInfoForUser(CFieldIOCPSocket *i_pFISoc);					// 2008-03-27 by dhjin, ���� ���� ǥ�� ��ȹ�� - ���� ���� ���� 
	void SendStrategicPointInfoForAllUser(MapIndex_t mapIndex);
	void SendStrategicPointInfoListForAllUser();
	void SendStrategicPointInfoForUser(CFieldIOCPSocket *i_pFISoc);
	INT MakeMSG_FC_INFO_MSWARINFO_DISPLAY(SMSWARINFO_DISPLAY *o_pMSWarInfoDisPlay, vectMSWARINFO_DISPLAY * i_pVectMSWarInfoDisPlay);		// 2008-03-27 by dhjin, ���� ���� ǥ�� ��ȹ�� - MSG_FC_INFO_MSWARINFO_DISPLAY
	INT MakeMSG_FC_INFO_STRATEGICPOINT_DISPLAY(SSTRATEGYPOINT_DISPLAY_INFO *o_StrategicPointInfoDisPlay, vector<SSTRATEGYPOINT_DISPLAY_INFO>& i_pVectStrategicPointInfoDisPlay);		// 2015-05-01 by killburne
	void SetMSWarInfo(MapIndex_t i_nMapindex, INT i_nMSWarInfoModifyType, INT i_nHPRate = STATE_ERROR, INT i_nTelePortState = STATE_ERROR, BYTE i_byBelligerence = STATE_ERROR);		// 2008-03-27 by dhjin, ���� ���� ǥ�� ��ȹ�� - ���� ����
	void InitMSWarOptionTypeW(BYTE i_byLeaderInfluence);		// 2008-03-28 by dhjin, ���� ���� ǥ�� ��ȹ�� -  ���� ���� �ɼ� �ʱ�ȭ 
	void SetMSWarOptionTypeW(BYTE i_byLeaderInfluence, SHORT i_byMSWarOptionType);		// 2008-03-28 by dhjin, ���� ���� ǥ�� ��ȹ�� - ���� ���� �ɼ� ���� 
	void SendMSWarOptionTypeForInfluenceAllUser(BYTE i_byLeaderInfluence);					// 2008-03-28 by dhjin, ���� ���� ǥ�� ��ȹ�� - ���� ���� �ɼ� ���¿� ��� �������� �����ϱ�
	void SendMSWarOptionTypeForInfluenceUser(CFieldIOCPSocket *i_pFISoc);					// 2008-03-28 by dhjin, ���� ���� ǥ�� ��ȹ�� - ���� ���� �ɼ� �������� �����ϱ�

	//////////////////////////////////////////////////////////////////////////
	// 2008-04-01 by dhjin, ����, ������ ����â ��ȹ�� - 
	vectSMSWAR_INFO_RESULT			m_vectMSWarInfoResult;		// 2008-04-01 by dhjin, ����, ������ ����â ��ȹ�� - ���� ��� ����
	vectSSPWAR_INFO_RESULT			m_vectSPWarInfoResult;		// 2008-04-01 by dhjin, ����, ������ ����â ��ȹ�� - ������ ��� ����

	void ResetMSWarInfoResult();
	void ResetSPWarInfoResult();
	void DeleteMSWarInfoResult(INT i_nMonsterUID);		
	void DeleteSPWarInfoResult(INT i_nSPSummonMapIndex);			
	void SetMSWarInfoResultByWarStart(BYTE i_byAttInfluence, INT i_nMonsterUID, INT i_nContributionPoint);			// 2008-04-01 by dhjin, ����, ������ ����â ��ȹ�� - ���� ���� �� ���� ����
	void SetMSWarInfoResultByWarEnd(INT i_nMonsterUID,BYTE i_byWinInfluence);										// 2008-04-01 by dhjin, ����, ������ ����â ��ȹ�� - ���� ���� �� ���� ����
	void SendLogMSWarInfoResult(INT i_nMonsterUID);															// 2008-04-01 by dhjin, ����, ������ ����â ��ȹ�� - ���� ���� �� ���� �α� ����
	void SetSPWarInfoResultByWarStart(BYTE i_byAttInfluence, INT i_nSPSummonMapIndex);			// 2008-04-01 by dhjin, ����, ������ ����â ��ȹ�� - ������ ���� �� ���� ����
	void SetSPWarInfoResultByWarEnd(INT i_nSPSummonMapIndex, BYTE i_byWinInfluence);					// 2008-04-01 by dhjin, ����, ������ ����â ��ȹ�� - ������ ���� �� ���� ����
	void SendLogSPWarInfoResult(INT i_nSPSummonMapIndex);												// 2008-04-01 by dhjin, ����, ������ ����â ��ȹ�� - ������ ���� �� ���� �α� ����
#ifdef NEMERIAN_AUTO_ELECTION_AND_RESET
	void setDiffVictory();
#endif
	INT SetMSWarInfoContributionPoint(INT i_nMonsterIdx);								// 2008-04-22 by dhjin, ����, ������ ����â ��ȹ�� - ���� ����Ʈ

	// 2009-12-04 by cmkwon, �������� ���/���2 ���� ��ȯ ó�� ���� ���� - 
	BOOL CheckCreateableMonsterInMap(INT i_nMapIdx);

	//////////////////////////////////////////////////////////////////////////
	// 2009-01-12 by dhjin, ���� ����
	void MSWarBeforeStart(BYTE i_byInflTy);			// 2009-01-12 by dhjin, ���� ���� - ���� ������ ������ ����
	void MSWarStart(BYTE i_byInflTy);				// 2009-01-12 by dhjin, ���� ���� - ���� ����
	CDeclarationOfWar			m_DeclarationOfWar;
	

// 2009-07-02 by cmkwon, �������� ���� ���� ���� - 
//	void SetInflEventRate();			// 2009-03-31 by dhjin, ���� ���� - ������ �¸� �� ���� �Ǵ� �̺�Ʈ ����
	void SetCompensationForDecalationOfWarWin(BYTE i_byInflTy, INT i_BossMonNum=0);		// 2009-07-02 by cmkwon, �������� ���� ���� ���� - 


	// 2009-03-10 by dhjin, �ܰ躰 �� �ý���
	INT GetMSBossSummonValue(BYTE i_byInflTy, BYTE * o_byBeforeWinCheck);		// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - ���� �ֱ⿡ ��ȯ�� �� ������ �������� ���� ��ȯ�Ǿ�� �Ǵ� ��UID�� ����Ѵ�.

	void LeaderChangeName(BYTE i_nInfluenceType, USHORT i_nLeaderType, char* i_stName);	// 2013-01-16 by jhseol, ������ ���ѿ��� �ɸ��� �̸� ����� ����ž ������ �ǽð� ����

	// 2013-05-09 by hskim, ���� ����Ʈ ����
	BOOL CalcContributionPoint(BYTE i_byWinnerInflTy, INT i_nWinnerValue, INT i_nLoserValue);
	BOOL CalcConsecutiveVictoriesPoint(BYTE i_byWinnerInFlTy);
	INT	GetConsecutiveVictoriesPoint(BYTE i_byWinnerInFlTy);
	INT GetTurnAroundPoint();		// + BCU Buff / - ANI Buff
	float GetPVPBuffPercent(int i_CurrentDiffContributionPoint);		// 2013-08-01 by jhseol, ������ ���� ������
	// end 2013-05-09 by hskim, ���� ����Ʈ ����

protected:
	CFieldIOCP						*m_pFieldIOCP14;
	mtvectSummonMonsterData			m_mtvectSummonMonsterDataList;

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-13 by cmkwon, InfluenceWarInfo ����
	vectDB_INFLUENCE_WAR_INFO		m_vectVCNInflWarInfoList;
	vectDB_INFLUENCE_WAR_INFO		m_vectANIInflWarInfoList;


	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-13 by cmkwon, InfluenceWarData ����
	CInflWarData					m_VCNInflWarData;
	CInflWarData					m_ANIInflWarData;

	SSUMMONBOSSMONSTER_INFO			m_SummonVCNBossMonsterInfo;		// 2007-02-06 by dhjin,
	SSUMMONBOSSMONSTER_INFO			m_SummonANIBossMonsterInfo;		// 2007-02-06 by dhjin,

	//////////////////////////////////////////////////////////////////////////
	// 2007-03-02 by dhjin, ��������Ʈ ���� ���� �ֱ� ����.
	SSTRATEGYPOINT_SUMMON_RANGE				m_StrategyPointSummonRange;		// 2007-02-28 by dhjin,
	CRITICAL_SECTION						m_criticallStrategyPointSummonRange;	// 2007-03-12 by dhjin
	mtvectSSTRATEGYPOINT_SUMMONTIME_INFO	m_mtvectStrategyPointSummonTimeInfo;	// 2007-02-28 by dhjin,
	mtvectSSTRATEGYPOINT_NOT_SUMMON_TIME	m_mtvectStrategyPointNotSummonTime;		// 2007-02-28 by dhjin,
	mtvectSSTRATEGYPOINT_DAY				m_mtvectStrategyPointDay;				// 2007-03-07 by dhjin,
	vectSSPPossibleTimeInfo					m_vectSSPPossibleTimeInfo;			// 2009-03-23 by dhjin, ��������Ʈ �ð� ���� ���� - ��������Ʈ ��ȯ ������ �ð��� ���� ����
	mtvectSSTRATEGYPOINT_SUMMONTIME_INFO	m_mtvectALLSPSummonTimeInfo;		// 2009-03-23 by dhjin, ��������Ʈ �ð� ���� ���� - ���� �ð� ���� ��������Ʈ�� ��ȯ�Ǵ� �ð��� ���� ����

	//////////////////////////////////////////////////////////////////////////
	// 2012-11-13 by jhseol, ���� �ý��� ������ - ������
	ATUM_DATE_TIME								m_atBeforeCheckTime;		// �Ϸ簡 �������� Ȯ���� �ð� ����
	mtvectSRENEWAL_STRATEGYPOINT_SUMMON_TIME	m_mtvectRenewalStrategyPointSummonTime;	// ���Ϻ� ������ �ð� �� ���� ���� �� ���� ����
	mtvectSSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO	m_mtvectStrategyPointMapInfluenceInfo;	// ������ ����Ʈ �� �� �������� ���� ����
	// end 2012-11-13 by jhseol, ���� �ý��� ������ - ������

	BOOL m_bIsDoingInfluenceWarCheck;	// 2012-11-26 by jhseol, ���� �ý��� ������ - ����� Ʈ���� ��������(�Ͻ�����), TRUE = ���� ���̴�, FALSE = ���� ���� �ƴϴ�.
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-09-07 by dhjin, �ڷ���Ʈ ��ġ�� �Ǿ� �ִ� ���� ����
	mtvectSTELEPORT_INFO_BUILDING			m_mtvectTeleportInfoBuilding;
	MapIndex_t								m_VCNTeleportMapIndex;				// 2007-09-16 by dhjin, ���������� ���� �ڷ���Ʈ�� ��ġ�� �� ��ȣ
	MapIndex_t								m_ANITeleportMapIndex;				// 2007-09-16 by dhjin, �˸��� ����  �ڷ���Ʈ�� ��ġ�� �� ��ȣ

	// 2007-09-07 by dhjin, ������ ���� UID����
	UID32_t		m_VCNLeaderGuildUID;				// 2007-09-07 by dhjin, ���������� ������ ����UID
	UID32_t		m_ANILeaderGuildUID;				// 2007-09-07 by dhjin, �˸��� ������ ���� UID

	// 2007-10-06 by dhjin, �������� ���� UID����, ����~!! �������� �������ڸ� ���� UID���� �ִ´�.
	UID32_t		m_VCNSubLeader1GuildUID;			// 2007-10-06 by dhjin, ���������� ��������1 ����UID
	UID32_t		m_VCNSubLeader2GuildUID;			// 2007-10-06 by dhjin, ���������� ��������2 ����UID
	UID32_t		m_ANISubLeader1GuildUID;			// 2007-10-06 by dhjin, �˸��� ��������1 ����UID
	UID32_t		m_ANISubLeader2GuildUID;			// 2007-10-06 by dhjin, �˸��� ��������2 ����UID

	SPOLLDATE_DB				m_SPOLLDATE_DB;				// 2007-10-29 by dhjin, ���� ����
	mtvectSVOTER_LIST			m_mtvectSVOTER_LIST;		// 2007-10-29 by dhjin, ��ǥ�� ����Ʈ
	mtvectSLEADER_CANDIDATE		m_mtvectSLEADER_CANDIDATE;	// 2007-10-29 by dhjin, ������ �ĺ���

	//////////////////////////////////////////////////////////////////////////
	// 2008-03-26 by dhjin, ���� ���� ǥ�� ��ȹ�� - 
	CMSWarInfoDisPlay			m_MSWarInfoDisPlay;

	// 2015-05-01 by killburne
	CStrategicPointDisplay		m_StrategicPointInfoDisplay;

protected:
	void OnInfluenceWarStart();	// 2015-09-14 Future, added events on infl war start & end
	void OnInfluenceWarEnd();	// 2015-09-14 Future, added events on infl war start & end
};

#endif // !defined(AFX_INFLWARMANAGER_H__35B2A97C_7399_40C5_AAC7_E19160CDE41B__INCLUDED_)
