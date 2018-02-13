// InflWarManager.cpp: implementation of the CInflWarManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InflWarManager.h"
#include "FieldIOCP.h"
#include "FieldGlobal.h"
#include "AtumLogSender.h"			// 2008-04-01 by dhjin, ����, ������ ����â ��ȹ�� - 

///////////////////////////////////////////////////////////////////////////////
/// \fn			CInflWarData::CInflWarData()
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CInflWarData::CInflWarData()
{
	InfluenceType			= INFLUENCE_TYPE_UNKNOWN;
	//MGameServerID			= 0;
	WartimeStage			= 0;
	ContributionPoint		= 0;
	InflLeaderCharacterUID	= 0;
	InflSub1LeaderCharacterUID = 0;
	InflSub2LeaderCharacterUID = 0;
	
	m_pInflWarInfoList		= NULL;
	m_pCurInflWarInfo		= NULL;
	m_bSummonJacoMonster	= FALSE;


	fInflHPRepairRate			= 0.0f;		// 2009-08-20 by cmkwon, �Ʒ��� ȸ���� ���� ���� - �ʱ�ȭ �ؾ���
	fInflDPRepairRate			= 0.0f;		// 2009-08-20 by cmkwon, �Ʒ��� ȸ���� ���� ���� - �ʱ�ȭ �ؾ���
	fInflSPRepairRate			= 0.0f;		// 2009-08-20 by cmkwon, �Ʒ��� ȸ���� ���� ���� - �ʱ�ȭ �ؾ���

	ConsecutiveVictories		= 0;		// 2013-05-09 by hskim, ���� ����Ʈ ����
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CInflWarData::~CInflWarData()
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CInflWarData::~CInflWarData()
{
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			mt_auto_lock *CInflWarData::GetmtlockPtr(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
mt_lock *CInflWarData::GetmtlockPtr(void)
{
	return &m_mtlock;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarData::InitCInflWarData(SINFLUENCE_WAR_DATA *i_pInflWarData, vectDB_INFLUENCE_WAR_INFO *i_pInflWarInfoList)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarData::InitCInflWarData(SINFLUENCE_WAR_DATA *i_pInflWarData, vectDB_INFLUENCE_WAR_INFO *i_pInflWarInfoList)
{
	mt_auto_lock mtA(GetmtlockPtr());
	InfluenceType			= i_pInflWarData->InfluenceType;
	//MGameServerID			= i_pInflWarData->MGameServerID;
	WartimeStage			= i_pInflWarData->WartimeStage;
	ContributionPoint		= i_pInflWarData->ContributionPoint;
	InflLeaderCharacterUID	= i_pInflWarData->InflLeaderCharacterUID;
	InflSub1LeaderCharacterUID = i_pInflWarData->InflSub1LeaderCharacterUID;
	InflSub2LeaderCharacterUID = i_pInflWarData->InflSub2LeaderCharacterUID;
	util::strncpy(InflLeaderCharacterName, i_pInflWarData->InflLeaderCharacterName, SIZE_MAX_CHARACTER_NAME);	// 2007-12-03 by dhjin
	util::strncpy(InflSubLeader1CharacterName, i_pInflWarData->InflSubLeader1CharacterName, SIZE_MAX_CHARACTER_NAME);	// 2007-12-03 by dhjin
	util::strncpy(InflSubLeader2CharacterName, i_pInflWarData->InflSubLeader2CharacterName, SIZE_MAX_CHARACTER_NAME);	// 2007-12-03 by dhjin
	ConsecutiveVictories	= i_pInflWarData->ConsecutiveVictories;		// 2013-05-09 by hskim, ���� ����Ʈ ����

	m_pInflWarInfoList		= i_pInflWarInfoList;

	m_pCurInflWarInfo		= this->GetInflWarInfoByWartimeStage(WartimeStage);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarData::SetCurrentStepInflWarData(INT i_nBossMonsterUID, byte i_byInfl)
/// \brief		���� ���� - ���� �� �ܰ�� m_pCurInflWarInfo�� �����Ѵ�.
/// \author		dhjin
/// \date		2009-01-14 ~ 2009-01-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarData::SetCurrentStepInflWarData(INT i_nBossMonsterUID, byte i_byInfl)
{
	mt_auto_lock mtA(GetmtlockPtr());

	m_pCurInflWarInfo = this->GetInflWarInfoByKillBossMonsterUID(i_nBossMonsterUID, i_byInfl);

	// 2009-07-02 by cmkwon, ���� ����� ���� ���� �α� �߰� - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] DeclarationNoti CInflWarData::SetCurrentStepInflWarData# %s BossMonsterNum(%d) CurInflWarInfo(WartimeStage:%d) \r\n"
		, CAtumSJ::GetInfluenceTypeString(i_byInfl), i_nBossMonsterUID, m_pCurInflWarInfo->WartimeStage);

// 2009-07-02 by cmkwon, �������� ���� ���� ���� - SetCompensationForDecalationOfWarWin() �Լ����� ó��
//// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - ������ �¸� �� ���� �Ǵ� �̺�Ʈ ����
//	this->fInflHPRepairRate = m_pCurInflWarInfo->HPRepairRate;
//	this->fInflDPRepairRate = m_pCurInflWarInfo->DPRepairRate;
//	this->fInflSPRepairRate = m_pCurInflWarInfo->SPRepairRate;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CInflWarData::CalcWartimeStage(int i_nContibutionPoint)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CInflWarData::CalcWartimeStage(int i_nContibutionPoint)
{
	if(NULL == m_pInflWarInfoList){					return 0;}

	int nBeforeStage = 0;
	for(int i=0; i < m_pInflWarInfoList->size(); i++)		// 2006-04-13 by cmkwon, ������������ ���ĵǾ��ִ�
	{
		SDB_INFLUENCE_WAR_INFO *pInflWarInfo = &((*m_pInflWarInfoList)[i]);
		if(i_nContibutionPoint == pInflWarInfo->ReqContributionPoint)
		{
			return pInflWarInfo->WartimeStage;
		}
		else if(i_nContibutionPoint < pInflWarInfo->ReqContributionPoint)
		{
			return nBeforeStage;
		}
		nBeforeStage		= pInflWarInfo->WartimeStage;
	}

	return nBeforeStage;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SDB_INFLUENCE_WAR_INFO *CInflWarData::GetInflWarInfoByWartimeStage(int i_nWartimeStage)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SDB_INFLUENCE_WAR_INFO *CInflWarData::GetInflWarInfoByWartimeStage(int i_nWartimeStage)
{
	if(NULL == m_pInflWarInfoList){					return NULL;}

	for(int i=0; i < m_pInflWarInfoList->size(); i++)		// 2006-04-13 by cmkwon, ������������ ���ĵǾ��ִ�
	{
		SDB_INFLUENCE_WAR_INFO *pInflWarInfo = &((*m_pInflWarInfoList)[i]);
		if(i_nWartimeStage == pInflWarInfo->WartimeStage)
		{
			return pInflWarInfo;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SDB_INFLUENCE_WAR_INFO *CInflWarData::GetInflWarInfoByKillBossMonsterUID(int i_nBossMonsterUID, byte i_byInfl)
/// \brief		�ܰ躰 �� �ý��� 
/// \author		dhjin
/// \date		2009-03-10 ~ 2009-03-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SDB_INFLUENCE_WAR_INFO *CInflWarData::GetInflWarInfoByKillBossMonsterUID(int i_nBossMonsterUID, byte i_byInfl)
{
	if(NULL == m_pInflWarInfoList){					return NULL;}
	if(0 == i_nBossMonsterUID)
	{
		for(int i=0; i < m_pInflWarInfoList->size(); i++)		// 2006-04-13 by cmkwon, ������������ ���ĵǾ��ִ�
		{
			SDB_INFLUENCE_WAR_INFO *pInflWarInfo = &((*m_pInflWarInfoList)[i]);
			if(i_byInfl == pInflWarInfo->InfluenceType
				&& 5 == pInflWarInfo->WartimeStage)
			{
				return pInflWarInfo;
			}
		}			
	}

	for(int i=0; i < m_pInflWarInfoList->size(); i++)		// 2006-04-13 by cmkwon, ������������ ���ĵǾ��ִ�
	{
		SDB_INFLUENCE_WAR_INFO *pInflWarInfo = &((*m_pInflWarInfoList)[i]);
		if(i_nBossMonsterUID == pInflWarInfo->BossMonsterUnitKind)
		{
			return pInflWarInfo;
		}
	}
	
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SDB_INFLUENCE_WAR_INFO *CInflWarData::GetInflWarInfoByContributionPoint(int i_nContributionPoint)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-14 ~ 2006-04-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SDB_INFLUENCE_WAR_INFO *CInflWarData::GetInflWarInfoByContributionPoint(int i_nContributionPoint)
{
	if(NULL == m_pInflWarInfoList
		|| m_pInflWarInfoList->empty())
	{
		return NULL;
	}

	SDB_INFLUENCE_WAR_INFO *pBeforInfo = &((*m_pInflWarInfoList)[0]);
	for(int i=0; i < m_pInflWarInfoList->size(); i++)		// 2006-04-13 by cmkwon, ������������ ���ĵǾ��ִ�
	{
		SDB_INFLUENCE_WAR_INFO *pInflWarInfo = &((*m_pInflWarInfoList)[i]);
		if(i_nContributionPoint == pInflWarInfo->ReqContributionPoint)
		{
			return pInflWarInfo;
		}
		else if(i_nContributionPoint < pInflWarInfo->ReqContributionPoint)
		{
			return pBeforInfo;
		}
		pBeforInfo = pInflWarInfo;
	}

	return pBeforInfo;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SDB_INFLUENCE_WAR_INFO *CInflWarData::GetCurInflWarInfo(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SDB_INFLUENCE_WAR_INFO *CInflWarData::GetCurInflWarInfo(void)
{
	return m_pCurInflWarInfo;
}

void CInflWarData::SetWartimeStage(SDB_INFLUENCE_WAR_INFO *i_pInflWarInfo)
{
	WartimeStage		= i_pInflWarInfo->WartimeStage;
	m_pCurInflWarInfo	= i_pInflWarInfo;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarData::IsSummonJacoMonster(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-20 ~ 2006-04-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarData::IsSummonJacoMonster(void)
{
	return m_bSummonJacoMonster;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		cmkwon
/// \date		2006-04-20 ~ 2006-04-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarData::SetSummonJacoMonster(BOOL i_bSummon)
{
	m_bSummonJacoMonster	= i_bSummon;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 2008-03-26 by dhjin, ���� ���� ǥ�� ��ȹ�� - 
CMSWarInfoDisPlay::CMSWarInfoDisPlay()
{
	this->InitMSWarInfoDisPlay();
}

CMSWarInfoDisPlay::~CMSWarInfoDisPlay()
{

}

void CMSWarInfoDisPlay::InitMSWarInfoDisPlay()
{
	m_mtvectMSWARINFO_DISPLAY.clear();
}

void CMSWarInfoDisPlay::InitMSWarOptionType(BYTE i_byLeaderInfluence)
{
	if(IS_VCN_INFLUENCE_TYPE(i_byLeaderInfluence))
	{
		BCUMSWarOptionType = MS_WAR_OPTION_TYPE_UNKNOWN;
		BCUMSWarOptionType |= MS_WAR_OPTION_TYPE_ATT_ACTIVATED;
		BCUMSWarOptionType |= MS_WAR_OPTION_TYPE_ATT_ACTIVE;	
		BCUMSWarOptionType |= MS_WAR_OPTION_TYPE_ATT_ACTIVE_TIME;	
		BCUMSWarOptionType |= MS_WAR_OPTION_TYPE_ATT_TELEPORT_HP;	
		BCUMSWarOptionType |= MS_WAR_OPTION_TYPE_ATT_STRATEGYPOINT_LIVE;				
		BCUMSWarOptionType |= MS_WAR_OPTION_TYPE_DEF_STRATEGYPOINT_HP;
		BCUMSWarOptionType |= MS_WAR_OPTION_TYPE_DEF_STRATEGYPOINT_LIVE;		
		BCUMSWarOptionType |= MS_WAR_OPTION_TYPE_LEADER;		
	}
	else
	{
		ANIMSWarOptionType = MS_WAR_OPTION_TYPE_UNKNOWN;
		ANIMSWarOptionType |= MS_WAR_OPTION_TYPE_ATT_ACTIVATED;
		ANIMSWarOptionType |= MS_WAR_OPTION_TYPE_ATT_ACTIVE;	
		ANIMSWarOptionType |= MS_WAR_OPTION_TYPE_ATT_ACTIVE_TIME;	
		ANIMSWarOptionType |= MS_WAR_OPTION_TYPE_ATT_TELEPORT_HP;	
		ANIMSWarOptionType |= MS_WAR_OPTION_TYPE_ATT_STRATEGYPOINT_LIVE;				
		ANIMSWarOptionType |= MS_WAR_OPTION_TYPE_DEF_STRATEGYPOINT_HP;
		ANIMSWarOptionType |= MS_WAR_OPTION_TYPE_DEF_STRATEGYPOINT_LIVE;		
		ANIMSWarOptionType |= MS_WAR_OPTION_TYPE_LEADER;			
	}
}

void CMSWarInfoDisPlay::MSWarInfoDisPlayClear(BYTE i_byMonsterInfluence)
{
	mt_auto_lock mta(&m_mtvectMSWARINFO_DISPLAY);
	mtvectMSWARINFO_DISPLAY::iterator itr = m_mtvectMSWARINFO_DISPLAY.begin();
	while(itr != m_mtvectMSWARINFO_DISPLAY.end())
	{
// 2009-10-12 by cmkwon, ������ī ���� ��� ���� - CMSWarInfoDisPlay::MSWarInfoDisPlayClear#
//		if(IS_SAME_CHARACTER_8_MAP_INFLUENCE(i_byMonsterInfluence, itr->MapInfluenceType))
		if(CAtumSJ::IsSameCharacterInfluence8MapInfluence(i_byMonsterInfluence, itr->MapInfluenceType))		// 2009-10-12 by cmkwon, ������ī ���� ��� ���� - 
		{
			itr = m_mtvectMSWARINFO_DISPLAY.erase(itr);
			continue;
		}
		itr++;
	}
	
}

void CMSWarInfoDisPlay::AllGetMSWarInfoDisPlay(vectMSWARINFO_DISPLAY * o_VectMSWarInfoDisPlay)
{
	mt_auto_lock mta(&m_mtvectMSWARINFO_DISPLAY);

	o_VectMSWarInfoDisPlay->assign(m_mtvectMSWARINFO_DISPLAY.begin(), m_mtvectMSWARINFO_DISPLAY.end());
}

void CMSWarInfoDisPlay::SetMSWarInfoDisPlay(MSWARINFO_DISPLAY * i_pMSWARINFO_DISPLAY)
{
	mt_auto_lock mta(&m_mtvectMSWARINFO_DISPLAY);

	mtvectMSWARINFO_DISPLAY::iterator itr = m_mtvectMSWARINFO_DISPLAY.begin();
	for(; itr != m_mtvectMSWARINFO_DISPLAY.end(); itr++)
	{
		if(i_pMSWARINFO_DISPLAY->MapIndex == itr->MapIndex)
		{
			itr->Belligerence	= i_pMSWARINFO_DISPLAY->Belligerence;
			itr->HPRate			= i_pMSWARINFO_DISPLAY->HPRate;
			itr->TelePortState	= i_pMSWARINFO_DISPLAY->TelePortState;
			return;
		}
	}

	m_mtvectMSWARINFO_DISPLAY.push_back(*i_pMSWARINFO_DISPLAY);
}

void CMSWarInfoDisPlay::SetMSWarInfoDisPlayByHPRate(MapIndex_t i_nMapindex, INT i_nHPRate)
{
	mt_auto_lock mta(&m_mtvectMSWARINFO_DISPLAY);

	mtvectMSWARINFO_DISPLAY::iterator itr = m_mtvectMSWARINFO_DISPLAY.begin();
	for(; itr != m_mtvectMSWARINFO_DISPLAY.end(); itr++)
	{
		if(i_nMapindex == itr->MapIndex)
		{
			itr->HPRate			= i_nHPRate;
			return;
		}
	}
}

void CMSWarInfoDisPlay::SetMSWarInfoDisPlayByCreateTeleport(MapIndex_t i_nMapindex, INT i_nHPRate, INT i_nTelePortState, BYTE i_byBelligerence)
{
	mt_auto_lock mta(&m_mtvectMSWARINFO_DISPLAY);

	mtvectMSWARINFO_DISPLAY::iterator itr = m_mtvectMSWARINFO_DISPLAY.begin();
	for(; itr != m_mtvectMSWARINFO_DISPLAY.end(); itr++)
	{
		if(i_nMapindex == itr->MapIndex)
		{
			itr->HPRate			= i_nHPRate;
			itr->TelePortState	= i_nTelePortState;
			itr->Belligerence	= i_byBelligerence;
			return;
		}
	}
}

void CMSWarInfoDisPlay::SetMSWarInfoDisPlayByChangeTeleportState(MapIndex_t i_nMapindex, INT i_nHPRate, INT i_nTelePortState, ATUM_DATE_TIME * i_pTelePortBuildingStartTime /* = NULL*/, ATUM_DATE_TIME * i_pTelePortBuildingEndTime /* = NULL*/)
{
	mt_auto_lock mta(&m_mtvectMSWARINFO_DISPLAY);

	mtvectMSWARINFO_DISPLAY::iterator itr = m_mtvectMSWARINFO_DISPLAY.begin();
	for(; itr != m_mtvectMSWARINFO_DISPLAY.end(); itr++)
	{
		if(i_nMapindex == itr->MapIndex)
		{
			itr->HPRate			= i_nHPRate;
			itr->TelePortState	= i_nTelePortState;
			if(TELEPORT_STATE_BUILDING == i_nTelePortState)
			{
				itr->TelePortBuildingStartTime	= *i_pTelePortBuildingStartTime;
				itr->TelePortBuildingEndTime	= *i_pTelePortBuildingEndTime;
			}
			return;
		}
	}
}

void CMSWarInfoDisPlay::SetMSWarOptionType(BYTE i_byLeaderInfluence, SHORT i_byMSWarOptionType)
{
	if(IS_VCN_INFLUENCE_TYPE(i_byLeaderInfluence))
	{
		BCUMSWarOptionType = i_byMSWarOptionType;
	}
	else
	{
		ANIMSWarOptionType = i_byMSWarOptionType;
	}
}

SHORT CMSWarInfoDisPlay::GetMSWarOptionType(BYTE i_byLeaderInfluence)
{	
	if(IS_VCN_INFLUENCE_TYPE(i_byLeaderInfluence))
	{
		return BCUMSWarOptionType;
	}
	else
	{
		return ANIMSWarOptionType;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 2015-05-01 by killburne

CStrategicPointDisplay::CStrategicPointDisplay()
{
	InitStrategicPointDisplay();
}

CStrategicPointDisplay::~CStrategicPointDisplay()
{
	InitStrategicPointDisplay();
}

void CStrategicPointDisplay::InitStrategicPointDisplay()
{
	m_mtvectSTRATEGICPOINT_DISPLAY.clear();
}

void CStrategicPointDisplay::GetAllStrategicPointDisplay(vector<SSTRATEGYPOINT_DISPLAY_INFO>* o_VectStrategicPointDisplay)
{
	mt_auto_lock mta(&m_mtvectSTRATEGICPOINT_DISPLAY);

	char szSystemLog[256];
	sprintf(szSystemLog, "Getting All StrategicPointInfoDisplay Count(%d)\r\n", m_mtvectSTRATEGICPOINT_DISPLAY.size());
	g_pFieldGlobal->WriteSystemLog(szSystemLog);
	o_VectStrategicPointDisplay->assign(m_mtvectSTRATEGICPOINT_DISPLAY.begin(), m_mtvectSTRATEGICPOINT_DISPLAY.end());
}

bool CStrategicPointDisplay::GetStrategicPointDisplay(MapIndex_t mapIndex, SSTRATEGYPOINT_DISPLAY_INFO* o_StrategicPointDisplay)
{
	mt_auto_lock mta(&m_mtvectSTRATEGICPOINT_DISPLAY);

	for (auto &info : m_mtvectSTRATEGICPOINT_DISPLAY)
	{
		if (info.MapIndex == mapIndex)
		{
			CopyMemory(o_StrategicPointDisplay, &info, sizeof SSTRATEGYPOINT_DISPLAY_INFO);
			return true;
		}
	}

	return false;
}

void CStrategicPointDisplay::SetStrategicPointDisplay(SSTRATEGYPOINT_DISPLAY_INFO* i_pSTRATEGICPOINT_DISPLAY)
{
	mt_auto_lock mta(&m_mtvectSTRATEGICPOINT_DISPLAY);
	char szSystemLog[256];
	for (auto &strPoint : m_mtvectSTRATEGICPOINT_DISPLAY)
	{
		if (i_pSTRATEGICPOINT_DISPLAY->MapIndex == strPoint.MapIndex)
		{
			strPoint.HPRate = i_pSTRATEGICPOINT_DISPLAY->HPRate;
			
			sprintf(szSystemLog, "Updating StrategicPointInfoDisplay MapIndex(%d) InfluenceType(%d) HPRate(%d)\r\n", i_pSTRATEGICPOINT_DISPLAY->MapIndex, (INT)i_pSTRATEGICPOINT_DISPLAY->InfluenceType, (INT)i_pSTRATEGICPOINT_DISPLAY->HPRate);
			g_pFieldGlobal->WriteSystemLog(szSystemLog);
			return;
		}
	}

	sprintf(szSystemLog, "Adding StrategicPointInfoDisplay MapIndex(%d) InfluenceType(%d)\r\n", i_pSTRATEGICPOINT_DISPLAY->MapIndex, (INT)i_pSTRATEGICPOINT_DISPLAY->InfluenceType);
	g_pFieldGlobal->WriteSystemLog(szSystemLog);
	m_mtvectSTRATEGICPOINT_DISPLAY.push_back(*i_pSTRATEGICPOINT_DISPLAY);
}

void CStrategicPointDisplay::SetStrategicPointDisplay(SSUMMON_STRATEGYPOINT_INFO* i_pSTRATEGICPOINT_INFO)
{
	mt_auto_lock mta(&m_mtvectSTRATEGICPOINT_DISPLAY);

	for (auto &strPoint : m_mtvectSTRATEGICPOINT_DISPLAY)
	{
		if (i_pSTRATEGICPOINT_INFO->MapIndex == strPoint.MapIndex)
		{
			return;
		}
	}

	char szSystemLog[256];
	sprintf(szSystemLog, "Adding StrategicPointInfoDisplay MapIndex(%d) InfluenceType(%d)\r\n", i_pSTRATEGICPOINT_INFO->MapIndex, (INT)i_pSTRATEGICPOINT_INFO->InfluenceType);
	g_pFieldGlobal->WriteSystemLog(szSystemLog);
	SSTRATEGYPOINT_DISPLAY_INFO display;
	util::zero(&display, sizeof SSTRATEGYPOINT_DISPLAY_INFO);
	display.HPRate = MS_WAR_HP_RATE;
	display.InfluenceType = i_pSTRATEGICPOINT_INFO->InfluenceType;
	display.MapIndex = i_pSTRATEGICPOINT_INFO->MapIndex;
	display.SummonStrategyPointTime = i_pSTRATEGICPOINT_INFO->SummonStrategyPointTime;
	
	m_mtvectSTRATEGICPOINT_DISPLAY.push_back(display);
}

void CStrategicPointDisplay::SetStrategicPointDisplayHP(MapIndex_t i_nMapindex, FLOAT i_nHPRate)
{
	mt_auto_lock mta(&m_mtvectSTRATEGICPOINT_DISPLAY);

	for (auto &strPoint : m_mtvectSTRATEGICPOINT_DISPLAY)
	{
		if (strPoint.MapIndex == i_nMapindex)
		{
			strPoint.HPRate = i_nHPRate;
			char szSystemLog[256];
			sprintf(szSystemLog, "Updating StrategicPointInfoDisplayHP MapIndex(%d) HPRate(%d)\r\n", i_nMapindex, (INT)i_nHPRate);
			g_pFieldGlobal->WriteSystemLog(szSystemLog);
			break;
		}
	}
}

void CStrategicPointDisplay::RemoveStrategicPointDisplay(MapIndex_t i_nMapindex)
{
	mt_auto_lock mta(&m_mtvectSTRATEGICPOINT_DISPLAY);

	vector<SSTRATEGYPOINT_DISPLAY_INFO>::iterator it = find_if(m_mtvectSTRATEGICPOINT_DISPLAY.begin(), m_mtvectSTRATEGICPOINT_DISPLAY.end(), [i_nMapindex](const SSTRATEGYPOINT_DISPLAY_INFO &strPoint){
		return strPoint.MapIndex == i_nMapindex;
	});
	if (it == m_mtvectSTRATEGICPOINT_DISPLAY.end())
	{
		return;
	}
	char szSystemLog[256];
	sprintf(szSystemLog, "Removing StrategicPointInfoDisplay MapIndex(%d)\r\n", i_nMapindex);
	g_pFieldGlobal->WriteSystemLog(szSystemLog);
	m_mtvectSTRATEGICPOINT_DISPLAY.erase(it);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 2009-01-12 by dhjin, ���� ����
CDeclarationOfWar::CDeclarationOfWar()
{
	this->InitDeclarationOfWar();
}

CDeclarationOfWar::~CDeclarationOfWar()
{

}

void CDeclarationOfWar::InitDeclarationOfWar()
{
	m_mtvectSDECLARATION_OF_WAR.clear();
	m_pCurrentBCUWarStepInfo = NULL;
	m_pCurrentANIWarStepInfo = NULL;
	m_pBeforeBCUWarStartTime.Reset();
	m_pBeforeANIWarStartTime.Reset();
	util::zero(&m_ForbidTime, sizeof(SDECLARATION_OF_WAR_FORBID_TIME));
}

BOOL CDeclarationOfWar::SetDBDeclarationOfWar(mtvectSDECLARATION_OF_WAR * i_pVectDeclarationOfWar, SDECLARATION_OF_WAR_FORBID_TIME * i_pForbidTime)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);
	m_mtvectSDECLARATION_OF_WAR.clear();
	m_mtvectSDECLARATION_OF_WAR.assign(i_pVectDeclarationOfWar->begin(), i_pVectDeclarationOfWar->end());


	///////////////////////////////////////////////////////////////////////////////
	// 2009-07-02 by cmkwon, �������� ���� ���� ���� - ������ BCU ���� ������ ANI���µ� ������ �����Ѵ�.
	mtvectSDECLARATION_OF_WAR::iterator itr = m_mtvectSDECLARATION_OF_WAR.begin();
	for(; itr != m_mtvectSDECLARATION_OF_WAR.end(); itr++)
	{
		SDECLARATION_OF_WAR *pTmWar = &*itr;
		if(pTmWar->Influence == INFLUENCE_TYPE_ANI)
		{
			SDECLARATION_OF_WAR *pBCUWarStep = this->FindMSWarStep(INFLUENCE_TYPE_VCN, pTmWar->MSWarStep);
			if(NULL == pBCUWarStep)
			{
				// 2009-07-02 by cmkwon, ���� ����� ���� ���� �α� �߰� - 	
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] DeclarationNoti CDeclarationOfWar::SetDBDeclarationOfWar# Check error ANI Step(%d), BCU StepPtr(0x%X) \r\n", pTmWar->MSWarStep, pBCUWarStep);
				return FALSE;
			}
			pTmWar->MSWarStepStartTime		= pBCUWarStep->MSWarStepStartTime;
			pTmWar->MSWarStepEndTime		= pBCUWarStep->MSWarStepEndTime;
		}
	}

	m_pCurrentBCUWarStepInfo	= this->FindMSWarStepByCurrentTime(INFLUENCE_TYPE_VCN);
	m_pCurrentANIWarStepInfo	= this->FindMSWarStepByCurrentTime(INFLUENCE_TYPE_ANI);

// 2009-07-02 by cmkwon, �������� ���� ���� ���� - ���� ���� ���� ��
// 	ATUM_DATE_TIME CurrentTime;
// 	CurrentTime.SetCurrentDateTime();
// 
// 	bool CheckBCUTimeSet = FALSE;
// 	bool CheckANITimeSet = FALSE;
// 
// 	mtvectSDECLARATION_OF_WAR::iterator itr = m_mtvectSDECLARATION_OF_WAR.begin();
// 	for(; itr != m_mtvectSDECLARATION_OF_WAR.end(); itr++)
// 	{
// 		if(CheckBCUTimeSet
// 			&& CheckANITimeSet)
// 		{// 2009-02-09 by dhjin, ���� ���� - BCU, ANI �� �� ���� �Ǿ��ٸ� ������.
// 			break;
// 		}
// 		
// 		if(!CheckBCUTimeSet
// 			&& INFLUENCE_TYPE_VCN == itr->Influence
// 			&& itr->MSWarStepEndTime > CurrentTime
// 			)
// 		{// 2009-02-09 by dhjin, ���� ���� - ���� �ð��� ���� �ǰų� ���� �ð��� �����Ѵ�.
// 			m_pCurrentBCUWarStepInfo = itr;
// 			CheckBCUTimeSet = TRUE;
// 		}
// 		if(!CheckANITimeSet
// 			&& INFLUENCE_TYPE_ANI == itr->Influence
// 			&& itr->MSWarStepEndTime > CurrentTime
// 			)
// 		{
// 			m_pCurrentANIWarStepInfo = itr;
// 			CheckANITimeSet = TRUE;
// 		}
// 	}

	if(NULL == m_pCurrentBCUWarStepInfo
		|| NULL ==  m_pCurrentANIWarStepInfo)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Error] DeclarationOfWar : Can't set period of DeclarationOfWar !!, BUC(%s) ANI(%s) \r\n"
			, m_pCurrentBCUWarStepInfo==NULL?"NULL":m_pCurrentBCUWarStepInfo->MSWarStepStartTime.GetDateTimeString().GetBuffer()
			, m_pCurrentANIWarStepInfo==NULL?"NULL":m_pCurrentANIWarStepInfo->MSWarStepStartTime.GetDateTimeString().GetBuffer());
		return FALSE;	
	}

	// 2009-07-02 by cmkwon, ���� ����� ���� ���� �α� �߰� - 	
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] DeclarationNoti CDeclarationOfWar::SetDBDeclarationOfWar# 200 BUC(%s ~ %s, Step(%2d) MSNum(%d) EndState(%d)) \r\n"
		, m_pCurrentBCUWarStepInfo->MSWarStepStartTime.GetDateTimeString().GetBuffer(), m_pCurrentBCUWarStepInfo->MSWarStepEndTime.GetDateTimeString().GetBuffer()
		, m_pCurrentBCUWarStepInfo->MSWarStep, m_pCurrentBCUWarStepInfo->MSNum, m_pCurrentBCUWarStepInfo->MSWarEndState);
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] DeclarationNoti CDeclarationOfWar::SetDBDeclarationOfWar# 300 ANI(%s ~ %s Step(%2d) MSNum(%d) EndState(%d)) \r\n"
		, m_pCurrentANIWarStepInfo->MSWarStepStartTime.GetDateTimeString().GetBuffer(), m_pCurrentANIWarStepInfo->MSWarStepEndTime.GetDateTimeString().GetBuffer()
		, m_pCurrentANIWarStepInfo->MSWarStep, m_pCurrentANIWarStepInfo->MSNum, m_pCurrentANIWarStepInfo->MSWarEndState);

	util::zero(&m_ForbidTime, sizeof(SDECLARATION_OF_WAR_FORBID_TIME));
	m_ForbidTime.DayOfWeek			= i_pForbidTime->DayOfWeek;
	m_ForbidTime.ForbidStartTime	= i_pForbidTime->ForbidStartTime;
	m_ForbidTime.ForbidEndTime		= i_pForbidTime->ForbidEndTime;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CDeclarationOfWar::SetBeforeWarStartTime(INT i_nBCUBeforeMin, INT i_nANIBeforeMin)
/// \brief		����ܰ�� ���� ������ ���� �ð��� �����Ѵ�.
/// \author		dhjin
/// \date		2009-01-14 ~ 2009-01-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CDeclarationOfWar::SetBeforeWarStartTime(INT i_nBCUBeforeMin, INT i_nANIBeforeMin)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);
	m_pBeforeBCUWarStartTime = m_pCurrentBCUWarStepInfo->MSWarStartTime;
	m_pBeforeBCUWarStartTime.AddDateTime(0,0,0,0, -i_nBCUBeforeMin,0);
	m_pBeforeANIWarStartTime = m_pCurrentANIWarStepInfo->MSWarStartTime;
	m_pBeforeANIWarStartTime.AddDateTime(0,0,0,0, -i_nANIBeforeMin,0);

	// 2009-07-02 by cmkwon, �������� ���� ���� ���� - 
	if(m_pCurrentBCUWarStepInfo->MSWarStepStartTime > m_pBeforeBCUWarStartTime)
	{
		m_pBeforeBCUWarStartTime = m_pCurrentBCUWarStepInfo->MSWarStepStartTime;
	}
	if(m_pCurrentANIWarStepInfo->MSWarStepStartTime > m_pBeforeANIWarStartTime)
	{
		m_pBeforeANIWarStartTime = m_pCurrentANIWarStepInfo->MSWarStepStartTime;
	}
}

BYTE CDeclarationOfWar::GetCurrentMSWarStep(BYTE i_byInfl)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);

	if(INFLUENCE_TYPE_VCN == i_byInfl)
	{
		return m_pCurrentBCUWarStepInfo->MSWarStep;
	}
	else if(INFLUENCE_TYPE_ANI == i_byInfl)
	{
		return m_pCurrentANIWarStepInfo->MSWarStep;
	}

	return FALSE;
}

BYTE CDeclarationOfWar::GetCurrentMSWarEndState(BYTE i_byInfl)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);

	if(INFLUENCE_TYPE_VCN == i_byInfl)
	{
		return m_pCurrentBCUWarStepInfo->MSWarEndState;
	}
	else if(INFLUENCE_TYPE_ANI == i_byInfl)
	{
		return m_pCurrentANIWarStepInfo->MSWarEndState;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CDeclarationOfWar::GetBeforeMSWarEndInfo(BYTE i_byInfl, INT * o_nMSBossMonsterUID, BYTE * o_MSWarEndState)
/// \brief		
/// \author		dhjin
/// \date		2009-03-10 ~ 2009-03-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CDeclarationOfWar::GetBeforeMSWarEndInfo(BYTE i_byInfl, INT * o_nMSBossMonsterUID, BYTE * o_MSWarEndState)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);

	BYTE BeforeStep = 0;
	if(1 < m_pCurrentBCUWarStepInfo->MSWarStep
		&& MSWAR_NEXT_LEADER_SETP > m_pCurrentBCUWarStepInfo->MSWarStep)
	{
		BeforeStep = m_pCurrentBCUWarStepInfo->MSWarStep - 1;
	}
	else if(1 == m_pCurrentBCUWarStepInfo->MSWarStep)
	{
		BeforeStep = 1;
	}
	else
	{
		return;
	}

	mtvectSDECLARATION_OF_WAR::iterator itr = m_mtvectSDECLARATION_OF_WAR.begin();
	for(; itr != m_mtvectSDECLARATION_OF_WAR.end(); itr++)
	{
		if(i_byInfl == itr->Influence
			&& BeforeStep == itr->MSWarStep)
		{
			*o_nMSBossMonsterUID = itr->MSNum;
			*o_MSWarEndState = itr->MSWarEndState;
			return;
		}
	}	
	
	return;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT CDeclarationOfWar::GetCurrentMSUID(BYTE i_byInfl)
/// \brief		�ܰ躰 �� �ý��� - ���� �ֱ� ��ȯ�� ��UID�� ��������
/// \author		dhjin
/// \date		2009-03-10 ~ 2009-03-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CDeclarationOfWar::GetCurrentMSUID(BYTE i_byInfl)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);

	if(INFLUENCE_TYPE_VCN == i_byInfl)
	{
		return m_pCurrentBCUWarStepInfo->MSNum;
	}
	else if(INFLUENCE_TYPE_ANI == i_byInfl)
	{
		return m_pCurrentANIWarStepInfo->MSNum;
	}

	return FALSE;
}

void CDeclarationOfWar::SetMSWarStepByMSWarStart(SDECLARATION_OF_WAR * i_pDeclarationOfWar)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);
	if(m_pCurrentBCUWarStepInfo->Influence == i_pDeclarationOfWar->Influence)
	{
		m_pCurrentBCUWarStepInfo->MSAppearanceMap	= i_pDeclarationOfWar->MSAppearanceMap;
		m_pCurrentBCUWarStepInfo->MSNum				= i_pDeclarationOfWar->MSNum;
		m_pCurrentBCUWarStepInfo->MSWarEndState		= MSWARING;
		m_pCurrentBCUWarStepInfo->NCP				= i_pDeclarationOfWar->NCP;
	}
	else if(m_pCurrentANIWarStepInfo->Influence == i_pDeclarationOfWar->Influence)
	{
		m_pCurrentANIWarStepInfo->MSAppearanceMap	= i_pDeclarationOfWar->MSAppearanceMap;
		m_pCurrentANIWarStepInfo->MSNum				= i_pDeclarationOfWar->MSNum;
		m_pCurrentANIWarStepInfo->MSWarEndState		= MSWARING;
		m_pCurrentANIWarStepInfo->NCP				= i_pDeclarationOfWar->NCP;
	}
}

void CDeclarationOfWar::SetMSWarEndState(BYTE i_byInfl, INT i_nEndState)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);

	if(INFLUENCE_TYPE_VCN == i_byInfl)
	{
		m_pCurrentBCUWarStepInfo->MSWarEndState = i_nEndState;
	}
	else if(INFLUENCE_TYPE_ANI == i_byInfl)
	{
		m_pCurrentANIWarStepInfo->MSWarEndState = i_nEndState;
	}
}

Err_t CDeclarationOfWar::SetSelectWarTime(MSG_FC_INFO_DECLARATION_MSWAR_SET * i_pMsg, BYTE i_byInfl, BYTE * o_bySelect, ATUM_DATE_TIME * o_MSWarStartTime)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-07-02 by cmkwon, �������� ���� ���� ���� - �Ʒ��� ���� ���� ��.
	SDECLARATION_OF_WAR *pNextWarStep = (i_byInfl == INFLUENCE_TYPE_VCN) ? this->FindNextMSWarStep(m_pCurrentBCUWarStepInfo) : this->FindNextMSWarStep(m_pCurrentANIWarStepInfo);
	if(NULL == pNextWarStep
		|| pNextWarStep->MSWarStep != i_pMsg->MSWarStep)
	{
		return ERR_NO_SUCH_DECLARATION_MSWAR_INFO;
	}

	if(0 >= pNextWarStep->SelectCount)
	{
		return ERR_NO_MORE_DECLARATION_MSWAR_SET;
	}

	if(i_pMsg->GiveUp)
	{
		pNextWarStep->GiveUp			= TRUE;
	}
	else
	{
		ATUM_DATE_TIME tmLimitEndTime = pNextWarStep->MSWarStepEndTime;
		tmLimitEndTime.AddDateTime(0, 0, 0, 0, -180);
		if(CheckForbidTime(&(i_pMsg->MSWarStartTime))
			|| pNextWarStep->MSWarStepStartTime > i_pMsg->MSWarStartTime
			|| tmLimitEndTime < i_pMsg->MSWarStartTime)
		{// 2009-07-02 by cmkwon, �������� ������ ���� �ð� ���� ���� ���� - (���� ���� �ð�) ~ (���� ���� �ð� - 180��<3�ð�>), ���� �ð� �Ұ� <== ������ �ش� ���� �Ⱓ�ȿ� ���ᰡ �Ǿ�� �Ѵ�.
			return ERR_INVALID_DECLARATION_MSWAR_TIME;
		}
		pNextWarStep->MSWarStartTime	= i_pMsg->MSWarStartTime;				
		pNextWarStep->GiveUp			= FALSE;
	}
	pNextWarStep->SelectCount--;
	*o_MSWarStartTime	= pNextWarStep->MSWarStartTime;
	*o_bySelect			= pNextWarStep->SelectCount;	
	return ERR_NO_ERROR;

// 2009-07-02 by cmkwon, �������� ���� ���� ���� - ���� ���� ���� ��.
// 	BYTE BeforeMSWarStep = 0;
// 
// 	mtvectSDECLARATION_OF_WAR::iterator itr = m_mtvectSDECLARATION_OF_WAR.begin();
// 	for(; itr != m_mtvectSDECLARATION_OF_WAR.end(); itr++)
// 	{
// 		if(i_byInfl == itr->Influence 
// 			&& i_pMsg->MSWarStep == itr->MSWarStep
// 			)
// 		{
// 			if(0 == itr->SelectCount)
// 			{
// 				return ERR_NO_MORE_DECLARATION_MSWAR_SET;
// 			}
// 	
// 			if(i_pMsg->GiveUp)
// 			{
// 				itr->GiveUp = TRUE;
// 			}
// 			else
// 			{
// 				if(CheckForbidTime(&(i_pMsg->MSWarStartTime))
// 					|| itr->MSWarStepStartTime > i_pMsg->MSWarStartTime
// 					|| itr->MSWarStepEndTime < i_pMsg->MSWarStartTime
// 					|| BeforeMSWarStep != m_pCurrentBCUWarStepInfo->MSWarStep
// 					)
// 				{
// 					return ERR_INVALID_DECLARATION_MSWAR_TIME;
// 				}
// 				itr->MSWarStartTime = i_pMsg->MSWarStartTime;				
// 				itr->GiveUp = FALSE;
// 			}
// 			itr->SelectCount--;
// 			*o_MSWarStartTime	= itr->MSWarStartTime;
// 			*o_bySelect = itr->SelectCount;
// 
// 			return ERR_NO_ERROR;
// 		}
// 		BeforeMSWarStep = itr->MSWarStep;
// 	}	
// 
// 	return ERR_NO_SUCH_DECLARATION_MSWAR_INFO;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CDeclarationOfWar::SetMSWarFinish(BYTE i_bySummonInfl, BYTE i_byWinInfl, ATUM_DATE_TIME * i_pEndTime)
/// \brief		���� ���� �� �� ����
/// \author		dhjin
/// \date		2009-01-13 ~ 2009-01-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CDeclarationOfWar::SetMSWarFinish(BYTE i_bySummonInfl, BYTE i_byWinInfl, ATUM_DATE_TIME * i_pEndTime)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);

	if(INFLUENCE_TYPE_ANI == i_bySummonInfl)
	{// �˸��� ���̶�� �������������� ������ ���̴�.
		if(i_bySummonInfl != i_byWinInfl)
		{
			m_pCurrentBCUWarStepInfo->MSWarEndState = MSWAR_END_WIN;
		}
		else
		{
			m_pCurrentBCUWarStepInfo->MSWarEndState = MSWAR_END_LOSS;
		}
		m_pCurrentBCUWarStepInfo->MSWarEndTime = *i_pEndTime;
		
	}
	else if(INFLUENCE_TYPE_VCN == i_bySummonInfl)
	{
		if(i_bySummonInfl != i_byWinInfl)
		{
			m_pCurrentANIWarStepInfo->MSWarEndState = MSWAR_END_WIN;
		}
		else
		{
			m_pCurrentANIWarStepInfo->MSWarEndState = MSWAR_END_LOSS;
		}
		m_pCurrentANIWarStepInfo->MSWarEndTime = *i_pEndTime;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CDeclarationOfWar::CheckNextMSWarStep(ATUM_DATE_TIME * i_pCurrentTime)
/// \brief		���� ���� �ֱ����� Ȯ��
///				// 2009-07-02 by cmkwon, �������� ���� ���� ���� - 
/// \author		dhjin
/// \date		2009-01-14 ~ 2009-01-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CDeclarationOfWar::CheckNextMSWarStep(ATUM_DATE_TIME * i_pCurrentTime) 
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-07-02 by cmkwon, �������� ���� ���� ���� - �Ʒ��� ���� ������.
	if(m_pCurrentBCUWarStepInfo->MSWarStepEndTime > *i_pCurrentTime
		|| m_pCurrentBCUWarStepInfo->MSWarStep == MSWAR_NEXT_LEADER_SETP)
	{
		return FALSE;
	}

	SDECLARATION_OF_WAR *pNextBCUWarStep = this->FindNextMSWarStep(m_pCurrentBCUWarStepInfo);
	SDECLARATION_OF_WAR *pNextANIWarStep = this->FindNextMSWarStep(m_pCurrentANIWarStepInfo);
	if(NULL == pNextBCUWarStep
		|| NULL == pNextANIWarStep)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] DeclarationNoti CDeclarationOfWar::CheckNextMSWarStep# BUC(Next:0x%X Cur: %d) ANINextStep(Next:0x%X Cur: %d) \r\n"
			, pNextBCUWarStep, m_pCurrentBCUWarStepInfo->MSWarStep, pNextANIWarStep, m_pCurrentANIWarStepInfo->MSWarStep);
		return FALSE;
	}

	m_pCurrentBCUWarStepInfo	= pNextBCUWarStep;
	m_pCurrentANIWarStepInfo	= pNextANIWarStep;
	
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] DeclarationNoti CDeclarationOfWar::CheckNextMSWarStep# BUC(%s~%s %2d %8d) ANI(%s~%s %2d %8d) \r\n"
		, m_pCurrentBCUWarStepInfo->MSWarStepStartTime.GetDateTimeString().GetBuffer(), m_pCurrentBCUWarStepInfo->MSWarStepEndTime.GetDateTimeString().GetBuffer()
		, m_pCurrentBCUWarStepInfo->MSWarStep, m_pCurrentBCUWarStepInfo->MSNum
		, m_pCurrentANIWarStepInfo->MSWarStepStartTime.GetDateTimeString().GetBuffer(), m_pCurrentANIWarStepInfo->MSWarStepEndTime.GetDateTimeString().GetBuffer()
		, m_pCurrentANIWarStepInfo->MSWarStep, m_pCurrentANIWarStepInfo->MSNum);
	return TRUE;

// 2009-07-02 by cmkwon, �������� ���� ���� ���� - ���� ���� ���� ��.
//	ATUM_DATE_TIME CurrentTime;
//	CurrentTime.SetCurrentDateTime();
//	BOOL bCheckBCUUpdate = FALSE;
//	BOOL bCheckANIUpdate = FALSE;
//
// 	if(m_pCurrentBCUWarStepInfo->MSWarStepEndTime <= CurrentTime)
// 	{// �ֱⰡ �����ٸ� 
// 		mtvectSDECLARATION_OF_WAR::iterator itr = m_mtvectSDECLARATION_OF_WAR.begin();
// 		for(; itr != m_mtvectSDECLARATION_OF_WAR.end(); itr++)
// 		{
// 			if(MSWAR_NEXT_LEADER_SETP > itr->MSWarStep)
// 			{
// 				if(m_pCurrentBCUWarStepInfo->Influence == itr->Influence
// 					&& m_pCurrentBCUWarStepInfo->MSWarStep+1 == itr->MSWarStep
// 					&& FALSE == bCheckBCUUpdate
// 					)
// 				{// �����ֱ�� ������Ʈ
// 					m_pCurrentBCUWarStepInfo = itr;
// 					bCheckBCUUpdate = TRUE;
// 				}
// 				if(m_pCurrentANIWarStepInfo->Influence == itr->Influence
// 					&& m_pCurrentANIWarStepInfo->MSWarStep+1 == itr->MSWarStep
// 					&& FALSE == bCheckANIUpdate
// 					)
// 				{
// 					m_pCurrentANIWarStepInfo = itr;
// 					bCheckANIUpdate = TRUE;
// 				}
// 			}
// // 2009-03-16 by dhjin, ���� ���� - ���� �ذ� ���� ����
// //			else if(MSWAR_NEXT_LEADER_SETP == itr->MSWarStep
// //				&& MSWAR_FINAL_SETP == m_pCurrentBCUWarStepInfo->MSWarStep)
// //			{
// //				return FALSE;
// //			}
// 			else if(m_pCurrentBCUWarStepInfo->Influence == itr->Influence
// 				&& MSWAR_NEXT_LEADER_SETP == itr->MSWarStep
// 				&& FALSE == bCheckBCUUpdate)
// 			{
// 				m_pCurrentBCUWarStepInfo = itr;
// 				bCheckBCUUpdate = TRUE;
// 			}
// 			else if(m_pCurrentANIWarStepInfo->Influence == itr->Influence
// 				&& MSWAR_NEXT_LEADER_SETP == itr->MSWarStep
// 				&& FALSE == bCheckANIUpdate)
// 			{
// 				m_pCurrentANIWarStepInfo = itr;
// 				bCheckANIUpdate = TRUE;
// 			}
// 
// 			if(bCheckBCUUpdate
// 				&& bCheckANIUpdate)
// 			{
// 				return TRUE;
// 			}
// 		}
// 	}
// 	
// 	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-07-02 by cmkwon, �������� ���� ���� ���� - 
/// \author		cmkwon
/// \date		2009-07-02 ~ 2009-07-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SDECLARATION_OF_WAR * CDeclarationOfWar::FindNextMSWarStep(SDECLARATION_OF_WAR *i_pCurWarStep)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);
	
	SDECLARATION_OF_WAR *pLastWarStep = NULL;
	mtvectSDECLARATION_OF_WAR::iterator itr = m_mtvectSDECLARATION_OF_WAR.begin();
	for(; itr != m_mtvectSDECLARATION_OF_WAR.end(); itr++)
	{
		SDECLARATION_OF_WAR *pTmWar = &*itr;
		if(pTmWar->Influence != i_pCurWarStep->Influence)
		{// 2009-07-02 by cmkwon, ������ �ٸ��� continue
			continue;
		}

		if(pTmWar->MSWarStep == i_pCurWarStep->MSWarStep+1)
		{
			return pTmWar;
		}
		if(MSWAR_NEXT_LEADER_SETP == pTmWar->MSWarStep)
		{
			pLastWarStep = pTmWar;
		}
	}

	return pLastWarStep;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-07-02 by cmkwon, �������� ���� ���� ���� - 
/// \author		cmkwon
/// \date		2009-07-02 ~ 2009-07-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SDECLARATION_OF_WAR * CDeclarationOfWar::FindMSWarStep(BYTE i_byInfl, BYTE i_byMSWarStep)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);
	mtvectSDECLARATION_OF_WAR::iterator itr = m_mtvectSDECLARATION_OF_WAR.begin();
	for(; itr != m_mtvectSDECLARATION_OF_WAR.end(); itr++)
	{
		SDECLARATION_OF_WAR *pTmWar = &*itr;
		if(pTmWar->Influence == i_byInfl
			&& pTmWar->MSWarStep == i_byMSWarStep)
		{
			return pTmWar;
		}
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-07-02 by cmkwon, �������� ���� ���� ���� - 
/// \author		cmkwon
/// \date		2009-07-02 ~ 2009-07-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SDECLARATION_OF_WAR * CDeclarationOfWar::FindMSWarStepByCurrentTime(BYTE i_byInfl)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);
	ATUM_DATE_TIME atCurTime { true };

	mtvectSDECLARATION_OF_WAR::iterator itr = m_mtvectSDECLARATION_OF_WAR.begin();
	for(; itr != m_mtvectSDECLARATION_OF_WAR.end(); itr++)
	{
		SDECLARATION_OF_WAR *pTmWar = &*itr;
		if(pTmWar->Influence == i_byInfl
			&& pTmWar->MSWarStepEndTime > atCurTime)
		{
			return pTmWar;
		}
	}
	
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-07-02 by cmkwon, �������� ���� ���� ���� - 
/// \author		cmkwon
/// \date		2009-07-03 ~ 2009-07-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE CDeclarationOfWar::GetMostMSWarStepOfWinStep(BYTE i_byInfl, int *o_pnBossMonNum)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);

	SDECLARATION_OF_WAR *pMostStepOfWin = NULL;
	mtvectSDECLARATION_OF_WAR::iterator itr = m_mtvectSDECLARATION_OF_WAR.begin();
	for(; itr != m_mtvectSDECLARATION_OF_WAR.end(); itr++)
	{
		SDECLARATION_OF_WAR *pTmWar = &*itr;
		if(pTmWar->Influence != i_byInfl
			|| MSWAR_END_WIN != pTmWar->MSWarEndState)
		{
			continue;
		}

		if(NULL == pMostStepOfWin
			|| pMostStepOfWin->MSWarStep < pTmWar->MSWarStep)
		{
			pMostStepOfWin = pTmWar;
		}
	}
	
	if(NULL == pMostStepOfWin)
	{
		return 0;
	}

	*o_pnBossMonNum		= pMostStepOfWin->MSNum;
	return pMostStepOfWin->MSWarStep;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CDeclarationOfWar::CheckBCUMSWarBeforeStartTime(ATUM_DATE_TIME * i_pCurrentTime)
/// \brief		������ ���� �ð����� üũ
/// \author		dhjin
/// \date		2009-01-13 ~ 2009-01-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CDeclarationOfWar::CheckBCUMSWarBeforeStartTime(ATUM_DATE_TIME * i_pCurrentTime)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);

// 2009-07-02 by cmkwon, �������� ���� ���� ���� - 	
// 	if(MSWAR_NOT_START == m_pCurrentBCUWarStepInfo->MSWarEndState
// 		&& m_pBeforeBCUWarStartTime <= *i_pCurrentTime
// 		&& FALSE == m_pCurrentBCUWarStepInfo->GiveUp
// 		)
// 	{
// 		return TRUE;
// 	}
//	
//	return FALSE;
	///////////////////////////////////////////////////////////////////////////////
	// 2009-07-02 by cmkwon, �������� ���� ���� ���� - 
	if(MSWAR_NOT_START != m_pCurrentBCUWarStepInfo->MSWarEndState
		|| m_pCurrentBCUWarStepInfo->GiveUp)
	{// 2009-07-06 by cmkwon, 
		return FALSE;
	}

	if(m_pBeforeBCUWarStartTime > *i_pCurrentTime)
	{// 2009-07-06 by cmkwon, ���� �ð��� �����غ� �ð��� ��ġ�� �ʴ´�
		return FALSE;
	}
	
	if(m_pCurrentBCUWarStepInfo->MSWarStartTime < *i_pCurrentTime)
	{// 2009-07-06 by cmkwon, ���� �ð��� ���� ���۽ð� ���� ��� �Ǿ����� �̹� ������ ������ ������� �ʴ´�.
		return FALSE;
	}

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] DeclarationNoti CDeclarationOfWar::CheckBCUMSWarBeforeStartTime# BCU BeforeStartTime(%s) StartTime(%s) \r\n"
		, m_pBeforeBCUWarStartTime.GetDateTimeString().GetBuffer(), m_pCurrentBCUWarStepInfo->MSWarStartTime.GetDateTimeString().GetBuffer());
	return TRUE;	
}

BOOL CDeclarationOfWar::CheckANIMSWarBeforeStartTime(ATUM_DATE_TIME * i_pCurrentTime)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);
	
// 2009-07-02 by cmkwon, �������� ���� ���� ���� - 	
// 	if(MSWAR_NOT_START == m_pCurrentANIWarStepInfo->MSWarEndState
// 		&& m_pBeforeANIWarStartTime <= *i_pCurrentTime
// 		&& FALSE == m_pCurrentANIWarStepInfo->GiveUp
// 		)
// 	{
// 		return TRUE;
// 	}
// 	
// 	return FALSE;
	///////////////////////////////////////////////////////////////////////////////
	// 2009-07-02 by cmkwon, �������� ���� ���� ���� - 	
	if(MSWAR_NOT_START != m_pCurrentANIWarStepInfo->MSWarEndState
		|| m_pCurrentANIWarStepInfo->GiveUp)
	{// 2009-07-06 by cmkwon, 
		return FALSE;
	}
	
	if(m_pBeforeANIWarStartTime > *i_pCurrentTime)
	{// 2009-07-06 by cmkwon, ���� �ð��� �����غ� �ð��� ��ġ�� �ʴ´�
		return FALSE;
	}
	
	if(m_pCurrentANIWarStepInfo->MSWarStartTime < *i_pCurrentTime)
	{// 2009-07-06 by cmkwon, ���� �ð��� ���� ���۽ð� ���� ��� �Ǿ����� �̹� ������ ������ ������� �ʴ´�.
		return FALSE;
	}

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] DeclarationNoti CDeclarationOfWar::CheckBCUMSWarBeforeStartTime# ANI BeforeStartTime(%s) StartTime(%s) \r\n"
		, m_pBeforeANIWarStartTime.GetDateTimeString().GetBuffer(), m_pCurrentANIWarStepInfo->MSWarStartTime.GetDateTimeString().GetBuffer());
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CDeclarationOfWar::CheckBCUMSWarStartTime(ATUM_DATE_TIME * i_pCurrentTime)
/// \brief		���� ���� �ð����� üũ
/// \author		dhjin
/// \date		2009-01-12 ~ 2009-01-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CDeclarationOfWar::CheckBCUMSWarStartTime(ATUM_DATE_TIME * i_pCurrentTime)
{	
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);
	
	if(MSWARING_BEFORE == m_pCurrentBCUWarStepInfo->MSWarEndState
		&& m_pCurrentBCUWarStepInfo->MSWarStartTime <= *i_pCurrentTime 
		&& FALSE == m_pCurrentBCUWarStepInfo->GiveUp
		)
	{
		return TRUE;
	}
	
	return FALSE;
}

BOOL CDeclarationOfWar::CheckANIMSWarStartTime(ATUM_DATE_TIME * i_pCurrentTime)
{	
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);
	
	if(MSWARING_BEFORE == m_pCurrentANIWarStepInfo->MSWarEndState
		&& m_pCurrentANIWarStepInfo->MSWarStartTime <= *i_pCurrentTime 
		&& FALSE == m_pCurrentANIWarStepInfo->GiveUp
		)
	{
		return TRUE;
	}
	
	return FALSE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CDeclarationOfWar::CheckForbidTime(ATUM_DATE_TIME * i_pHopeTime)
/// \brief		���� ���� �ð����� �������� �ð����� üũ
/// \author		dhjin
/// \date		2009-01-13 ~ 2009-01-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CDeclarationOfWar::CheckForbidTime(ATUM_DATE_TIME * i_pHopeTime)
{
	auto localTime = tm(*i_pHopeTime);

	ATUM_DATE_TIME CurrentTime;
	CurrentTime.SetCurrentDateTime();
	if((m_ForbidTime.DayOfWeek == localTime.tm_wday
		&& m_ForbidTime.ForbidStartTime.Hour <= i_pHopeTime->Hour
		&& m_ForbidTime.ForbidEndTime.Hour > i_pHopeTime->Hour)
		|| MSWAR_FORBID_GAP_HOUR >= i_pHopeTime->GetTimeDiffTimeInHours(CurrentTime)
		)
	{
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT	CDeclarationOfWar::MakeMSG_FC_INFO_MSWARINFO_RESULT_OK(SDECLARATION_OF_WAR * o_pDeclarationOfWarInfo, SDECLARATION_OF_WAR_FORBID_TIME * o_pForbidTime)
/// \brief		Ŭ���̾�Ʈ ���� �� ���� ���� ���� �����
/// \author		dhjin
/// \date		2009-01-13 ~ 2009-01-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT	CDeclarationOfWar::MakeMSG_FC_INFO_DECLARATION_MSWAR_INFO_OK(SDECLARATION_OF_WAR * o_pDeclarationOfWarInfo, SDECLARATION_OF_WAR_FORBID_TIME * o_pForbidTime)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);
	
	int	ResultListCount = 0;

	mtvectSDECLARATION_OF_WAR::iterator itr = m_mtvectSDECLARATION_OF_WAR.begin();
	for(; itr != m_mtvectSDECLARATION_OF_WAR.end(); itr++)
	{
		o_pDeclarationOfWarInfo[ResultListCount].Influence			= itr->Influence;
		o_pDeclarationOfWarInfo[ResultListCount].MSWarStep			= itr->MSWarStep;
		o_pDeclarationOfWarInfo[ResultListCount].NCP				= itr->NCP;
		o_pDeclarationOfWarInfo[ResultListCount].MSNum				= itr->MSNum;
		o_pDeclarationOfWarInfo[ResultListCount].MSAppearanceMap	= itr->MSAppearanceMap;
		o_pDeclarationOfWarInfo[ResultListCount].MSWarStepStartTime	= itr->MSWarStepStartTime;
		o_pDeclarationOfWarInfo[ResultListCount].MSWarStepEndTime	= itr->MSWarStepEndTime;
		o_pDeclarationOfWarInfo[ResultListCount].MSWarStartTime		= itr->MSWarStartTime;
		o_pDeclarationOfWarInfo[ResultListCount].MSWarEndTime		= itr->MSWarEndTime;
		o_pDeclarationOfWarInfo[ResultListCount].SelectCount		= itr->SelectCount;
		o_pDeclarationOfWarInfo[ResultListCount].GiveUp				= itr->GiveUp;
		o_pDeclarationOfWarInfo[ResultListCount].MSWarEndState		= itr->MSWarEndState;
		ResultListCount++;
	}
	
	o_pForbidTime->DayOfWeek		=	m_ForbidTime.DayOfWeek;
	o_pForbidTime->ForbidStartTime	=	m_ForbidTime.ForbidStartTime;
	o_pForbidTime->ForbidEndTime	=	m_ForbidTime.ForbidEndTime;

	return ResultListCount;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CDeclarationOfWar::GetOwnerInflOfConflictArea(BYTE i_byInfl)
/// \brief		���� �������� ���� �������� üũ
/// \author		dhjin
/// \date		2009-01-13 ~ 2009-01-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CDeclarationOfWar::GetOwnerInflOfConflictArea(BYTE i_byInfl)
{
	ATUM_DATE_TIME CurrentTime;
	CurrentTime.SetCurrentDateTime();

	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);

	BOOL OwnerResult = FALSE; 

	mtvectSDECLARATION_OF_WAR::iterator itr = m_mtvectSDECLARATION_OF_WAR.begin();
	for(; itr != m_mtvectSDECLARATION_OF_WAR.end(); itr++)
	{
		if(i_byInfl == itr->Influence
			&& MSWAR_END_LOSS == itr->MSWarEndState)
		{
			OwnerResult = FALSE;
		}
		else if(i_byInfl == itr->Influence
				&& MSWAR_END_WIN == itr->MSWarEndState)
		{
			OwnerResult = TRUE;
		}
	}	

	return OwnerResult;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BYTE CDeclarationOfWar::GetLastMSWinStep(BYTE i_byInfl)
/// \brief		���� ���� �¸� �ܰ踦 �����´�.
/// \author		dhjin
/// \date		2009-03-31 ~ 2009-03-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE CDeclarationOfWar::GetLastMSWinStep(BYTE i_byInfl)
{
	mt_auto_lock mta(&m_mtvectSDECLARATION_OF_WAR);

	BOOL LastMSWinStep = 0; 

	mtvectSDECLARATION_OF_WAR::iterator itr = m_mtvectSDECLARATION_OF_WAR.begin();
	for(; itr != m_mtvectSDECLARATION_OF_WAR.end(); itr++)
	{
		if(i_byInfl == itr->Influence
			&& MSWAR_END_WIN == itr->MSWarEndState)
		{
			LastMSWinStep = itr->MSWarStep;
		}
	}	

	return LastMSWinStep;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CDeclarationOfWar::CheckSPSummonPossibleTime()
/// \brief		��������Ʈ �ð� ���� ���� - �� �ð��� ��������Ʈ�� ��ȯ �Ǿ� ���� �ʰ� üũ
/// \author		dhjin
/// \date		2009-03-30 ~ 2009-03-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CDeclarationOfWar::CheckSPSummonPossibleTime()
{
	mt_auto_lock mta { m_mtvectSDECLARATION_OF_WAR };

	ATUM_DATE_TIME CheckTime { true };

	CheckTime.AddDateTime(0,0,0, 0, (STRATEGYPOINT_SUMMONTIME_BY_INFLUENCEBOSS/2 + 3),0);
	
	// todo : improve this hardcoded part

	if(m_pCurrentBCUWarStepInfo->MSWarStartTime <= CheckTime
		&& CheckTime.GetTimeDiffTimeInMinutes(m_pCurrentBCUWarStepInfo->MSWarStartTime) < 150 // 2017-01-02 panoskj
		&& FALSE == m_pCurrentBCUWarStepInfo->GiveUp
		&& !(MSWAR_END_WIN == m_pCurrentBCUWarStepInfo->MSWarEndState
			|| MSWAR_END_LOSS == m_pCurrentBCUWarStepInfo->MSWarEndState)
		)
	{
		return FALSE;
	}
	if(m_pCurrentANIWarStepInfo->MSWarStartTime <= CheckTime 
		&& CheckTime.GetTimeDiffTimeInMinutes(m_pCurrentANIWarStepInfo->MSWarStartTime) < 150 // 2017-01-02 panoskj
		&& FALSE == m_pCurrentANIWarStepInfo->GiveUp
		&& !(MSWAR_END_WIN == m_pCurrentANIWarStepInfo->MSWarEndState
			|| MSWAR_END_LOSS == m_pCurrentANIWarStepInfo->MSWarEndState)
		)
	{
		return FALSE;
	}
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 2012-11-13 by jhseol, ���� �ý��� ������ - ������
BOOL CDeclarationOfWar::CheckMSWarTime(USHORT i_Year, BYTE i_Month, BYTE i_Day)
{
	if( i_Year	== m_pCurrentBCUWarStepInfo->MSWarStartTime.Year &&
		i_Month	== m_pCurrentBCUWarStepInfo->MSWarStartTime.Month &&
		i_Day	== m_pCurrentBCUWarStepInfo->MSWarStartTime.Day)
	{
		return TRUE;
	}
	if( i_Year	== m_pCurrentANIWarStepInfo->MSWarStartTime.Year &&
		i_Month	== m_pCurrentANIWarStepInfo->MSWarStartTime.Month &&
		i_Day	== m_pCurrentANIWarStepInfo->MSWarStartTime.Day)
	{
		return TRUE;
	}
	return FALSE;
}
// end 2012-11-13 by jhseol, ���� �ý��� ������ - ������

///////////////////////////////////////////////////////////////////////////////
// 2013-02-26 by jhseol, ���� �ý��� ������ �ΰ��ɼ� - ���� ���� �ð� ��������
ATUM_DATE_TIME CDeclarationOfWar::GetMSWarStartTime(BYTE i_nInfluenceType)
{
	if( INFLUENCE_TYPE_VCN == i_nInfluenceType )
	{
		return m_pCurrentBCUWarStepInfo->MSWarStartTime;
	}
	else if ( INFLUENCE_TYPE_ANI == i_nInfluenceType )
	{
		return m_pCurrentANIWarStepInfo->MSWarStartTime;
	}
	ATUM_DATE_TIME ErrorTime;
	ErrorTime.SetDateTime(1986,2,28,0,0,0);
	return ErrorTime;
}
// end 2013-02-26 by jhseol, ���� �ý��� ������ �ΰ��ɼ� - ���� ���� �ð� ��������
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// 2006-04-13 by cmkwon


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CInflWarManager::CInflWarManager()
{
	InitializeCriticalSection(&m_criticallStrategyPointSummonRange);
}

CInflWarManager::~CInflWarManager()
{
	DeleteCriticalSection(&m_criticallStrategyPointSummonRange);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::InitCInflWarManager(CFieldIOCP *i_pFIOCP)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::InitCInflWarManager(CFieldIOCP *i_pFIOCP)
{
	m_pFieldIOCP14		= i_pFIOCP;

	m_mtvectSummonMonsterDataList.clear();

	m_mtvectSummonStrategyPointInfo.clear();			// 2007-02-23 by dhjin,
	m_mtvectSummonStrategyPointInfo.reserve(12);		// 2007-02-23 by dhjin,	���� �� ���� 10�� 

	util::zero(&m_SummonVCNBossMonsterInfo, sizeof(SSUMMONBOSSMONSTER_INFO));
	util::zero(&m_SummonANIBossMonsterInfo, sizeof(SSUMMONBOSSMONSTER_INFO));

	this->InitStrategyPoint();

	m_VCNLeaderGuildUID	= 0;
	m_ANILeaderGuildUID = 0;

	m_VCNTeleportMapIndex	= 0;
	m_ANITeleportMapIndex	= 0;

	m_VCNSubLeader1GuildUID = 0;
	m_VCNSubLeader2GuildUID = 0;
	m_ANISubLeader1GuildUID = 0;
	m_ANISubLeader2GuildUID = 0;

	util::zero(&m_SPOLLDATE_DB, sizeof(SPOLLDATE_DB));
	this->LoadVoterListByDB();
	this->LoadLeaderCandidateByDB();

	this->ResetMSWarInfoResult();		// 2008-04-01 by dhjin, ����, ������ ����â ��ȹ�� - ���� ��� ����
	this->ResetSPWarInfoResult();       // 2008-04-01 by dhjin, ����, ������ ����â ��ȹ�� - ������ ��� ����
	
	//////////////////////////////////////////////////////////////////////////
	// 2009-01-12 by dhjin, ���� ����
	m_DeclarationOfWar.InitDeclarationOfWar();		

	m_bIsDoingInfluenceWarCheck = FALSE;	// 2012-11-26 by jhseol, ���� �ý��� ������ - ����� Ʈ���� ��������(�Ͻ�����), TRUE = ���� ���̴�, FALSE = ���� ���� �ƴϴ�.
#ifdef NEMERIAN_NATION_BALANCE_BUFF_PENALITY
	setDiffVictory();
#endif
}

void CInflWarManager::OnMinutelySPManager(ATUM_DATE_TIME * pDateTime)
{
	ATUM_DATE_TIME now { true };

	auto localStTime = tm(now);

	// calculate SP spawn times if it's a new day
	if (now > m_atBeforeCheckTime && now.Day != m_atBeforeCheckTime.Day)	// 2013-01-14 by jhseol, ���� �ý��� ������ - ������, ��¥ üũ ���ǹ� ����
	{
		// �Ϸ簡 �����ٸ� ���� ������ �ٽ� �����Ѵ�.
		server::log(true, "  It's a new day, resetting SP times, last check time(%s), current time(%s)\r\n",
			m_atBeforeCheckTime.GetDateTimeString().GetBuffer(),
			now.GetDateTimeString().GetBuffer());
		
		m_atBeforeCheckTime.SetCurrentDateTime();

		////////////////////////////////////////////////////////////////////////
		// ���� ���Ͽ� �´� ���� �ð��� �����´�
		mt_auto_lock mta { m_mtvectRenewalStrategyPointSummonTime };

		for (auto& spinfo : m_mtvectRenewalStrategyPointSummonTime)
		{
			if (SHORT(localStTime.tm_wday) == spinfo.DayOfWeek)		// ���ð� ���� ������ ������ �������� 
			{
				auto StartTime { now };
				auto EndTime { now };

				StartTime.Hour = spinfo.StartTime.Hour;
				StartTime.Minute = spinfo.StartTime.Minute;
				StartTime.Second = spinfo.StartTime.Second;
				EndTime.Hour = spinfo.EndTime.Hour;
				EndTime.Minute = spinfo.EndTime.Minute;
				EndTime.Second = spinfo.EndTime.Second;

				auto	tmTodayBCUSummonCount = spinfo.CountBCU;	// ������ �� ������ ���� �ӽú����� �� ����
				auto	tmTodayANISummonCount = spinfo.CountANI;	// ������ �� ������ ���� �ӽú����� �� ����
				mta.auto_unlock_cancel();	// ���ǹ��� �ɸ��� �ð����� �����Դٸ� ���̻� �ʿ� ������ �� ����. �Ŀ� break�� �ݺ��� Ż�� �ڵ� ����

											////////////////////////////////////////////////////////////////////////
											// ���� ���� ���� �ð����� ����
				auto TotalSummonTime = EndTime.GetTimeDiffTimeInSeconds(StartTime);
				auto MaxSummonTimeGap = TotalSummonTime;

				////////////////////////////////////////////////////////////////////////
				// ���� ���� ���� �ð��븦 ������ ������ ���� ���� 1���� ������ �����ؾ��� ��� �ð��� ����Ѵ�
				auto SettingSummonStrategyPointCount = tmTodayBCUSummonCount + tmTodayANISummonCount;
				if (0 < SettingSummonStrategyPointCount)
				{
					MaxSummonTimeGap = TotalSummonTime / SettingSummonStrategyPointCount;
				}

				////////////////////////////////////////////////////////////////////////
				// ��� �ð����� ���� �ּ� �� 30���� ���ְ� ���� ���� �ð������� ����Ѵ�. ���⼭ �� 30���� �Ŀ� �ٽ� �����ش�.
				MaxSummonTimeGap -= RenewalStrategyPointSummonTimeTermMin;		// 2013-03-18 by jhseol, ���� �����ð� ���� - ���� ������ ���۽ð� ���� ���� �ϱ� ���� �ּ����� ���ش�
				if (0 > MaxSummonTimeGap)
				{
					MaxSummonTimeGap = 0;
				}

				////////////////////////////////////////////////////////////////////////
				// ������ ���� ���� �α� ���
				server::log(TRUE, "  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Reset Step.2 - Today Strategy Point Info : Start Time(%s), End Time(%s), BCUCount(%d), ANICount(%d), MinSummonTimeGap(%d), MaxSummonTimeGap(%d)\r\n"
					, StartTime.GetDateTimeString().GetBuffer(), EndTime.GetDateTimeString().GetBuffer()
					, tmTodayBCUSummonCount, tmTodayANISummonCount, RenewalStrategyPointSummonTimeTermMin, MaxSummonTimeGap + RenewalStrategyPointSummonTimeTermMin);

				vector<ATUM_DATE_TIME> tmRandomSummonTime;
				tmRandomSummonTime.clear();

				////////////////////////////////////////////////////////////////////////
				// ������ ������ ����ŭ ������ �Ѵ�.
				for (int forloopindex = 0; forloopindex < SettingSummonStrategyPointCount; forloopindex++)
				{
					int RandomSummonTimeGap = RANDI(0, MaxSummonTimeGap);
					StartTime.AddDateTime(0, 0, 0, 0, 0, RandomSummonTimeGap);
					tmRandomSummonTime.push_back(StartTime);

					if (S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_SUMMON_TIME_VARIANCE_JHSEOL)

						StartTime.AddDateTime(0, 0, 0, 0, 0, RenewalStrategyPointSummonTimeTermMin + MaxSummonTimeGap - RandomSummonTimeGap);

					else StartTime.AddDateTime(0, 0, 0, 0, 0, RenewalStrategyPointSummonTimeTermMin);
				}

				server::log(TRUE, "  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Reset Step.3 - Summon Time Setting Success\r\n");

				////////////////////////////////////////////////////////////////////////
				// mtvect �Ӽ��� ������ �Ϲ� ���ͷ� ī��
				vectSSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO tmVectSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO;		// ������ �� ������ ������ �ӽ� ����
				tmVectSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO.clear();
				mt_auto_lock mtMap(&m_mtvectStrategyPointMapInfluenceInfo);
				mtvectSSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO::iterator copy_itr = m_mtvectStrategyPointMapInfluenceInfo.begin();
				for (; copy_itr < m_mtvectStrategyPointMapInfluenceInfo.end(); copy_itr++)
				{
					tmVectSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO.push_back(*copy_itr);
				}
				mtMap.auto_unlock_cancel();		// ���簡 �Ϸ� �Ǿ� ���̻� �ʿ� ������ �� ����

				server::log(TRUE, "  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Reset Step.4 - Map Influence Copy Success : Map Count(%d)\r\n", tmVectSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO.size());

				////////////////////////////////////////////////////////////////////////
				// ���� ������ ����� ������ ���� ����ŭ ����Ʈ�� �߰��Ѵ�.
				mt_auto_lock mtb(&m_mtvectStrategyPointSummonTimeInfo);
				m_mtvectStrategyPointSummonTimeInfo.clear();
				SSTRATEGYPOINT_SUMMONTIME_INFO tmSSTRATEGYPOINT_SUMMONTIME_INFO;
				util::zero(&tmSSTRATEGYPOINT_SUMMONTIME_INFO, sizeof(SSTRATEGYPOINT_SUMMONTIME_INFO));
				vectSSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO::iterator tmmap_itr = tmVectSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO.begin();
				for (; tmmap_itr < tmVectSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO.end(); tmmap_itr++)
				{
					tmSSTRATEGYPOINT_SUMMONTIME_INFO.MapIndex = tmmap_itr->MapIndex;
					m_mtvectStrategyPointSummonTimeInfo.push_back(tmSSTRATEGYPOINT_SUMMONTIME_INFO);
				}

				server::log(TRUE, "  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Reset Step.5 - Strategy Point Summon Time Info Init(Delete & New Insert) Success\r\n");

				////////////////////////////////////////////////////////////////////////
				// ��������Ʈ���� ���� ���ؿ� ������� ���Ƿ� ���´�.
				std::random_shuffle(m_mtvectStrategyPointSummonTimeInfo.begin(), m_mtvectStrategyPointSummonTimeInfo.end());
				mtvectSSTRATEGYPOINT_SUMMONTIME_INFO::iterator shuffle_itr = m_mtvectStrategyPointSummonTimeInfo.begin();
				int tmRandomSummonTimeIndex = 0;		// �ӽ� ����� ���� ���� ������ �ε���
				int tmBCUSummonCount = 0;				// ���� �Ϸ�� BCU ������ ���� ��
				int tmANISummonCount = 0;				// ���� �Ϸ�� ANI ������ ���� ��
				for (; shuffle_itr < m_mtvectStrategyPointSummonTimeInfo.end(); shuffle_itr++)
				{
					util::zero(&shuffle_itr->SummonTime, sizeof(ATUM_DATE_TIME));
					shuffle_itr->SummonAttribute = FALSE;
					shuffle_itr->SummonCount = 0;
					////////////////////////////////////////////////////////////////////////
					// ������ �ʹ�ȣ�� ������ ���ؼ� �� ���¿� ���õ� ������ ����ŭ�� ������ �ǵ��� �Ѵ�.
					vectSSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO::iterator map_itr = tmVectSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO.begin();
					for (; map_itr < tmVectSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO.end(); map_itr++)
					{
						if (shuffle_itr->MapIndex == map_itr->MapIndex)
						{
							if (IS_MAP_INFLUENCE_VCN(map_itr->Influence))
							{
								////////////////////////////////////////////////////////////////////////
								// BCU������ ���� �Ϸ� �� ���� ���� ������ ������ ���� ���Ѵ�.
								if (tmBCUSummonCount < tmTodayBCUSummonCount)
								{
									////////////////////////////////////////////////////////////////////////
									// BCU���¿� ���� ���õ��� ���� ������ ������ �ǹ������� �����ϰ� ���� �Ϸ�� BCU ���� ������ ����
									shuffle_itr->SummonAttribute = TRUE;
									shuffle_itr->SummonCount = 1;
									tmBCUSummonCount++;
								}
							}
							else
							{
								////////////////////////////////////////////////////////////////////////
								// ANI������ ���� �Ϸ� �� ���� ���� ������ ������ ���� ���Ѵ�.
								if (tmANISummonCount < tmTodayANISummonCount)
								{
									////////////////////////////////////////////////////////////////////////
									// ANI���¿� ���� ���õ��� ���� ������ ������ �ǹ������� �����ϰ� ���� �Ϸ�� ANI ���� ������ ����
									shuffle_itr->SummonAttribute = TRUE;
									shuffle_itr->SummonCount = 1;
									tmANISummonCount++;
								}
							}
						}
					}
					if (TRUE == shuffle_itr->SummonAttribute)
					{
						////////////////////////////////////////////////////////////////////////
						// ���� �Ϸ� �� ������ �� ���� �ӽ÷� ������ ������ ���� �ð��� ũ�⸦ ���Ѵ�.
						if (tmRandomSummonTimeIndex < tmRandomSummonTime.size())
						{
							////////////////////////////////////////////////////////////////////////
							// ���� ���õ��� ���� ������ ������ �ǹ��Ѵ�.
							shuffle_itr->SummonTime = tmRandomSummonTime[tmRandomSummonTimeIndex];
							tmRandomSummonTimeIndex++;
						}
					}
					if (NULL != m_pFieldIOCP14)
					{
						QPARAM_STRATEGYPOINT_SUMMON_INFO *pQparam = new QPARAM_STRATEGYPOINT_SUMMON_INFO;
						pQparam->MapIndex = shuffle_itr->MapIndex;
						pQparam->SummonCount = shuffle_itr->SummonCount;
						pQparam->SummonTime = shuffle_itr->SummonTime;
						pQparam->SummonAttribute = shuffle_itr->SummonAttribute;
						m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateStrategyPointSummonInfo, NULL, 0, pQparam);
						server::log(TRUE, "  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Reset Step.Finish - Strategy Point Setting Done : MapIndex(%d), SummonCount(%d), SummonTime(%s), SummonAttribute(%d)\r\n"
							, shuffle_itr->MapIndex, shuffle_itr->SummonCount, shuffle_itr->SummonTime.GetDateTimeString().GetBuffer(), shuffle_itr->SummonAttribute);
					}
					else
					{
						server::log(TRUE, "  [ERROR!] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Reset Step.Finish - m_pFieldIOCP14 is NULL : MapIndex(%d), SummonCount(%d), SummonTime(%s), SummonAttribute(%d)\r\n"
							, shuffle_itr->MapIndex, shuffle_itr->SummonCount, shuffle_itr->SummonTime.GetDateTimeString().GetBuffer(), shuffle_itr->SummonAttribute);
					}
				}
				break;
			}
		}
	}

	// this conditional is eliminated at compile time
	auto condition = S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_SUB_OPTION_JHSEOL ?
		m_DeclarationOfWar.CheckSPSummonPossibleTime() :
		!m_DeclarationOfWar.CheckMSWarTime(now.Year, now.Month, now.Day);

	if (condition)
	{
		// 2017-01-03 too much logging, panoskj
		// this conditional is eliminated at compile time
		//if (S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_SUB_OPTION_JHSEOL)
		//	server::log(true, "  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_SUB_OPTION_JHSEOL #Summon Step.1 - Summon Process Start\r\n");
		//else server::log(true, "  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Summon Step.1 - Summon Process Start\r\n");

		mt_auto_lock mta { m_mtvectRenewalStrategyPointSummonTime };

		for (auto& spnewinfo : m_mtvectRenewalStrategyPointSummonTime)
		{
			if (SHORT(localStTime.tm_wday) == spnewinfo.DayOfWeek)		// ���ð� ���� ������ ������ �������� 
			{
				auto tmStartTime { now };

				tmStartTime.Hour = spnewinfo.StartTime.Hour;
				tmStartTime.Minute = spnewinfo.StartTime.Minute;
				tmStartTime.Second = spnewinfo.StartTime.Second;

				auto tmEndTime { now };

				tmEndTime.Hour = spnewinfo.EndTime.Hour;
				tmEndTime.Minute = spnewinfo.EndTime.Minute;
				tmEndTime.Second = spnewinfo.EndTime.Second;

				mta.auto_unlock_cancel();		// ���ǹ��� �ɸ��� �ð����� �����Դٸ� ���̻� �ʿ� ������ �� ����. �Ŀ� break�� �ݺ��� Ż�� �ڵ� ����

				if (now >= tmStartTime && now < tmEndTime)
				{
					server::log("  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Summon Step.2 - CurrentTime in Today War Time : StartTime(%s), EndTime(%s)\r\n"
						, tmStartTime.GetDateTimeString().GetBuffer(), tmEndTime.GetDateTimeString().GetBuffer());

					mt_auto_lock mtb { m_mtvectStrategyPointSummonTimeInfo };
					
					for (auto& spinfo : m_mtvectStrategyPointSummonTimeInfo)
					{
						if (!spinfo.SummonAttribute || 0 >= spinfo.SummonCount)		// ��ȯ���� �ʴ� ���� skip

							continue;
						
						if (now < spinfo.SummonTime)								// ���� ��ȯ�� �ȵƴٸ� skip
						{
							server::log("  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Summon Step.3 - Still did not have time to summon. This Summon Strategy Point Pass : MapIndex(%d), SummonTime(%s)\r\n",
								spinfo.MapIndex, spinfo.SummonTime.GetDateTimeString().GetBuffer());
							
							continue;
						}

						if (300 > now.GetTimeDiffTimeInSeconds(spinfo.SummonTime))	// ���� �ð��� ��ȯ �ؾ��� �ð��� ���̰� 5�� �̳��� ��ȯ�� �Ѵ�.
						{
							if (NULL == m_pFieldIOCP14)
							{
								server::log("  [ERROR!] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Summon Step.3 - m_pFieldIOCP14 is NULL, Summon Info : MapIndex(%d), SummonTime(%s)\r\n"
									, spinfo.MapIndex, spinfo.SummonTime.GetDateTimeString().GetBuffer());
								
								continue;
							}
							server::log("  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Summon Step.3 - Summon Strategy Point : MapIndex(%d), SummonTime(%s)\r\n"
								, spinfo.MapIndex, spinfo.SummonTime.GetDateTimeString().GetBuffer());
							spinfo.SummonAttribute = FALSE;
							spinfo.SummonCount = 0;
							auto tmMapIndex = spinfo.MapIndex;			// ������ �� ������ ���� �ӽú����� �� ����
							mtb.auto_unlock_cancel();						// ���ǹ��� �ɸ��� �ʿ� ������ �������� ���̻� �ʿ� ������ �� ����. �Ŀ� break�� �ݺ��� Ż�� �ڵ� ����

							auto pQparam = new QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_COUNT;
							pQparam->MapIndex = tmMapIndex;
							pQparam->SummonCount = 0;
							m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateStrategyPointSummonInfoBySummon, NULL, 0, pQparam);

							// �̹� ��ȯ���� ���������� üũ�� ����
							auto bSummonCheck = false;	
							
							mt_auto_lock mtc { m_mtvectSummonStrategyPointInfo };
							
							// ������ ��ȯ�� ���� �ִٸ�
							for (auto& inf : m_mtvectSummonStrategyPointInfo)
								// ���� ��ȯ�� ���������� Ȯ��
								if (inf.MapIndex == tmMapIndex)			
									// ������ �̹� ������ ��ȯ���̴�.
									bSummonCheck = true;				
							
							// ���� ��ȸ�� �������� �� ����.
							mtc.auto_unlock_cancel();						

							if (!m_pFieldIOCP14->m_OutPostManager.CheckALLOutPostWaring() && !bSummonCheck)	// ������������ �ƴϰ� ���� ������ �����ϴٸ� ������ȯ
							{
								auto pFMapChann = m_pFieldIOCP14->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(tmMapIndex, 0));
								
								if (pFMapChann)
								{
									INIT_MSG_WITH_BUFFER(MSG_FN_MONSTER_STRATEGYPOINT_SUMMON, T_FN_MONSTER_STRATEGYPOINT_SUMMON, pSummon, SendBuf);
									pSummon->MapIndex = tmMapIndex;
									pFMapChann->Send2NPCServerW(SendBuf, MSG_SIZE(MSG_FN_MONSTER_STRATEGYPOINT_SUMMON));
									server::log("  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Summon Step.4 - Strategy Point Summon Done\r\n");
								}
								else
								{
									server::log("  [ERROR!] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Summon Step.4 - pFMapChann is NULL, MapIndex(%d)\r\n", tmMapIndex);
								}
							}
							else
							{
								server::log("  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Summon Step.4 - OutPost Start OR This map Strategy Point Summoning : OutPostState(%d), SummonCheck(%d)\r\n"
									, m_pFieldIOCP14->m_OutPostManager.CheckALLOutPostWaring(), bSummonCheck);
							}
							break;
						}
					}
					server::log("  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Summon Step.Finish - Do not have to be summoned anymore.\r\n");
				}

				else
				{
					server::log("  [Notify] Spawning SPs is disabled at this time, summon time (%s ~ %s)\r\n",
						tmStartTime.GetDateTimeString().GetBuffer(),
						tmEndTime.GetDateTimeString().GetBuffer());
				}

				break;
			}
		}
	}
	else
	{
		if (S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_SUB_OPTION_JHSEOL)
		{
			auto cBCUMswarStartTime = m_DeclarationOfWar.GetMSWarStartTime(INFLUENCE_TYPE_VCN).GetDateTimeString().GetBuffer();
			auto cANIMswarStartTime = m_DeclarationOfWar.GetMSWarStartTime(INFLUENCE_TYPE_ANI).GetDateTimeString().GetBuffer();

			server::log("  [Notify] Cannot spawn SPs, a MSWAR is underway, BCU(%s), ANI(%s)\r\n", cBCUMswarStartTime, cANIMswarStartTime);
		}

		else server::log("  [Notify] Cannot spawn SPs, a MSWAR is today\r\n");
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::OnDoMinutelyWorkInflWarManager(ATUM_DATE_TIME *pDateTime)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-14 ~ 2006-04-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::OnDoMinutelyWorkInflWarManager(ATUM_DATE_TIME *pDateTime)
{
	// 2009-03-05 by cmkwon, �������� �Ʒ��� ���� ó�� - �Ʒ��� ������ ������ ���� ó�� �ʿ� ����
	if (g_pFieldGlobal->IsArenaServer()) return;

	if (m_DeclarationOfWar.CheckNextMSWarStep(pDateTime)) SetCurrentStepInflWarDataW();
	
	// BCU
	if (m_DeclarationOfWar.CheckBCUMSWarBeforeStartTime(pDateTime)) MSWarBeforeStart(INFLUENCE_TYPE_VCN);
	
	// BCU ���� ����
	else if (m_DeclarationOfWar.CheckBCUMSWarStartTime(pDateTime)) MSWarStart(INFLUENCE_TYPE_VCN);
	
	// ANI
	if (m_DeclarationOfWar.CheckANIMSWarBeforeStartTime(pDateTime)) MSWarBeforeStart(INFLUENCE_TYPE_ANI);
	
	// ANI ���� ����
	else if (m_DeclarationOfWar.CheckANIMSWarStartTime(pDateTime)) MSWarStart(INFLUENCE_TYPE_ANI);
	

	// 2007-11-16 by dhjin, ������ ���� �ð� üũ�Ͽ� ������ ��ǥ ��� ����
	// 2008-09-10 by cmkwon, ���վƷ��� ���������� üũ �ʿ� ����
	if (CheckLeaderelectionDate(pDateTime))	SendLeaderelectionInfo();
		
	///////////////////////////////////////////////////////////////////////////////
	// 2012-11-13 by jhseol, ���� �ý��� ������ - ������
	if (S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL) OnMinutelySPManager(pDateTime);

	else
	{
		//////////////////////////////////////////////////////////////////////////
		// 2007-03-02 by dhjin, ��������Ʈ ���� ���� �ֱ� ����.
		mt_auto_lock mta(this->GetStrategyPointSummonTimeInfo());
		if(TRUE == this->CheckStrategyPointSummon())
		{
			if(TRUE == this->CompareStrategyPointSummonTime())
			{// 2007-03-02 by dhjin, �ֱⰡ �����ٸ� ���� �����Ѵ�.
				mta.auto_unlock_cancel();
				this->LoadStrategyPointSummonInfo();			
			}
			else
			{
				mtvectSSTRATEGYPOINT_SUMMONTIME_INFO::iterator itr = m_mtvectStrategyPointSummonTimeInfo.begin();
				while(itr != m_mtvectStrategyPointSummonTimeInfo.end())
				{	
					if(0 >= itr->SummonCount)
					{
						itr++;
						continue;
					}

					if(0 < itr->SummonTime.GetTimeDiffToCurrentTimeInSeconds())
					{// 2007-03-02 by dhjin, ��ȯ �ð��� �����ٸ� ��ȯ �Ѵ�.
						// 2009-03-23 by dhjin, ��������Ʈ �ð� ���� ���� - �ذ� ���� �� �ð��� üũ�Ͽ� ���� �� �ð��� �߻��Ѵٸ� ���� �ֱ�� �ѱ��.
						// �ذ� ���� �����Ѵ�.
						if(!m_DeclarationOfWar.CheckSPSummonPossibleTime()
							|| FALSE == m_pFieldIOCP14->m_OutPostManager.CheckOutPostWarTimeStrategyPoint(&(itr->SummonTime))
							|| CheckSPSummonMapIndex(itr->MapIndex))
						{
							// 2009-03-23 by dhjin, ��������Ʈ �ð� ���� ���� - m_mtvectALLSPSummonTimeInfo���� ��ȯ�� ���� ����
							this->DeleteALLSPSummonTimeInfoByMapIndex(itr->MapIndex);

							if (1 < itr->SummonCount)
							{// ���� ��ȯ ���� �ִٸ� �����Ѵ�.	
								this->GetNextSPSummonTimeByALLSPSummonTimeInfo(itr->MapIndex, &itr->SummonTime, &itr->SummonAttribute);
								QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME *pQparam = new QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME;
								pQparam->MapIndex		= itr->MapIndex;
								pQparam->SummonTime		= itr->SummonTime;
								pQparam->SummonAttribute	= itr->SummonAttribute;		// 2008-04-08 by dhjin, ��ȯ ���� �ð� ���� - 
								this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateStrategyPointSummonInfoBySummonTime, NULL, 0, pQparam);
								char szSysLog[1024];
								wsprintf(szSysLog, "  [Notify]: Strategypoint SummonTime Update By MS or Outpost or SP, MapIndex(%d), SummonTime(%s)\r\n"
									, itr->MapIndex, itr->SummonTime.GetDateTimeString().GetBuffer());
								g_pFieldGlobal->WriteSystemLog(szSysLog);
							}
							else
							{// ���� ��ȯ ���� ���ٸ� ���� �ֱ�� �ѱ��.
								itr->SummonTime.AddDateTime(1,0,0, 0,0,0);
								QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME *pQparam = new QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME;
								pQparam->MapIndex		= itr->MapIndex;
								pQparam->SummonTime		= itr->SummonTime;
								pQparam->SummonAttribute	= itr->SummonAttribute;		// 2008-04-08 by dhjin, ��ȯ ���� �ð� ���� - 
								this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateStrategyPointSummonInfoBySummonTime, NULL, 0, pQparam);
								char szSysLog[1024];
								wsprintf(szSysLog, "  [Notify]: Startegypoint SummonTime Update Next Period, MapIndex(%d)\r\n"
									, itr->MapIndex);
								g_pFieldGlobal->WriteSystemLog(szSysLog);
							}
						}
	//					if (TRUE == m_SummonVCNBossMonsterInfo.bSummonBossMonster
	//						|| TRUE == m_SummonANIBossMonsterInfo.bSummonBossMonster)
	//					{// 2007-03-07 by dhjin, ������ ��ȯ ���̶�� ��������Ʈ ��ȯ �ð��� �ٽ� �����Ѵ�.
	//						this->SetStrategyPointSummonTime(itr, STRATEGYPOINT_SUMMON_GAP_BY_INFLUENCEWAR);
	//					}
	//					else if(FALSE == m_pFieldIOCP14->m_OutPostManager.CheckOutPostWarTimeStrategyPoint(&(itr->SummonTime)))
	//					{// 2007-10-18 by cmkwon, else if�� ����// 2007-09-03 by dhjin, �������� ������ �߻��ϴ� �ð��̸� ��ȯ �ð��� �ٽ� �����Ѵ�.
	//						this->SetStrategyPointSummonTime(itr, STRATEGYPOINT_SUMMON_GAP_BY_OUTPOST);
	//					}
						else
						{
							if ( 0 < itr->SummonCount)
							{
								CFieldMapChannel *pFMapChann = m_pFieldIOCP14->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(itr->MapIndex, 0));
								if(pFMapChann)
								{
									INIT_MSG_WITH_BUFFER(MSG_FN_MONSTER_STRATEGYPOINT_SUMMON, T_FN_MONSTER_STRATEGYPOINT_SUMMON, pSummon, SendBuf);
									pSummon->MapIndex = itr->MapIndex;
									pFMapChann->Send2NPCServerW(SendBuf, MSG_SIZE(MSG_FN_MONSTER_STRATEGYPOINT_SUMMON));
								}
								
								QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_COUNT *pQparam = new QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_COUNT;
								pQparam->MapIndex		= itr->MapIndex;
								itr->SummonCount		-= 1;
								pQparam->SummonCount	= itr->SummonCount;
								this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateStrategyPointSummonInfoBySummon, NULL, 0, pQparam);
								
								// 2009-03-23 by dhjin, ��������Ʈ �ð� ���� ���� - m_mtvectALLSPSummonTimeInfo���� ��ȯ�� ���� ����
								this->DeleteALLSPSummonTimeInfoByMapIndex(itr->MapIndex);

								if (0 < itr->SummonCount)
								{
	// 2009-03-23 by dhjin, ��������Ʈ �ð� ���� ���� - m_mtvectALLSPSummonTimeInfo���� ������ �����ͼ� ��ȯ �ð��� �����Ѵ�.
	// 2007-03-05 by dhjin, ��ȯ �� ��ȯ �Ǿ�� �ϴ� ������ �ִٸ� ��ȯ �ð��� �ٽ� �����Ѵ�.
	//								this->SetStrategyPointSummonTime(itr, STRATEGYPOINT_SUMMON_GAP_BY_INFLUENCEWAR);
									this->GetNextSPSummonTimeByALLSPSummonTimeInfo(itr->MapIndex, &itr->SummonTime, &itr->SummonAttribute);
									QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME *pQparam = new QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME;
									pQparam->MapIndex		= itr->MapIndex;
									pQparam->SummonTime		= itr->SummonTime;
									pQparam->SummonAttribute	= itr->SummonAttribute;		// 2008-04-08 by dhjin, ��ȯ ���� �ð� ���� - 
									this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateStrategyPointSummonInfoBySummonTime, NULL, 0, pQparam);		
									char szSysLog[1024];
									wsprintf(szSysLog, "  [Notify]: Strategypoint SummonTime Update, MapIndex(%d), SummonTime(%s)\r\n"
									, itr->MapIndex, itr->SummonTime.GetDateTimeString().GetBuffer());
									g_pFieldGlobal->WriteSystemLog(szSysLog);
								}
							}
						}

					}
					itr++;
				}
				mta.auto_unlock_cancel();
			}
		}
	}
	// end 2012-11-13 by jhseol, ���� �ý��� ������ - ������

	// 2007-09-16 by dhjin, �ڷ���Ʈ ó��
	TelePortInfoBuingByTick(pDateTime);
	
	if (!m_mtvectSummonMonsterDataList.empty())
	{
		ATUM_DATE_TIME now { true };

		mt_auto_lock mtA { m_mtvectSummonMonsterDataList };

		auto itr = m_mtvectSummonMonsterDataList.begin();

		while (itr != m_mtvectSummonMonsterDataList.end())
		{
			auto pSummonData = &*itr;

			auto nRemainMinute = pSummonData->SummonTerm - now.GetTimeDiffTimeInMinutes(pSummonData->atimeInsertedTime);

			if (nRemainMinute <= 0)
			{
				///////////////////////////////////////////////////////////////////////////////
				// 2006-04-14 by cmkwon, ���� ��ȯ ó��
				auto pFMapChann = m_pFieldIOCP14->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(pSummonData->SummonMapIndex, 0));

				if (pFMapChann)
				{
					INIT_MSG_WITH_BUFFER(MSG_FN_ADMIN_SUMMON_MONSTER, T_FN_ADMIN_SUMMON_MONSTER, pSSummon, SendBuf);
					pSSummon->ChannelIndex = pFMapChann->GetMapChannelIndex().ChannelIndex;
					util::strncpy(pSSummon->CharacterName, "InfluenceWar", SIZE_MAX_CHARACTER_NAME);
					pSSummon->MonsterUnitKind = pSummonData->pMonsterInfo->MonsterUnitKind;
					pSSummon->NumOfMonster = pSummonData->SummonCount;
					pSSummon->Position = pSummonData->SummonPosition;

					// 2010. 07. 05 by hsLee. ���� ��ȯ ����. (�뷱�� ������ ���� ����.)
					pSSummon->MonsterBalanceData.Init(0);

					pFMapChann->Send2NPCServerW(SendBuf, MSG_SIZE(MSG_FN_ADMIN_SUMMON_MONSTER));
				}

				///////////////////////////////////////////////////////////////////////////////
				// 2006-04-14 by cmkwon, ������ ���� ���� ��ȯ ���� Ŭ���̾�Ʈ�� ����
				INIT_MSG_WITH_BUFFER(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA, T_FC_WAR_BOSS_MONSTER_SUMMON_DATA, pSWarData, SendBuf);

				pSWarData->SummonMonsterUnitkind = pSummonData->pMonsterInfo->MonsterUnitKind;
				pSWarData->RemainMinute = 0;
				pSWarData->SummonMonsterTime = now;
				pSWarData->ContributionPoint = SetMSWarInfoContributionPoint(pSummonData->pMonsterInfo->MonsterUnitKind);			// 2008-04-22 by dhjin, ����, ������ ����â ��ȹ�� - ��������Ʈ
				pSWarData->BossStep = pSummonData->BossStep;		// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - �� �ܰ�
				pSWarData->BeforeWinCheck = pSummonData->BeforeWinCheck;				// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - �� �ܰ� ���� ����
				m_pFieldIOCP14->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA), INFLUENCE_TYPE_VCN | INFLUENCE_TYPE_ANI);

				///////////////////////////////////////////////////////////////////////////////
				// 2006-04-18 by cmkwon, ���� ���� ��ȯ ��� - �������������� ��ȯ�ú��� ���� ���� ��ȯ �ȵ�
				SetSummonJacoMonsterW(GET_SAME_CHARACTER_INFL_BY_MONSTER_BELL(pSummonData->pMonsterInfo->Belligerence), FALSE);
				INIT_MSG(MSG_FN_NPCSERVER_SUMMON_JACO_MONSTER, T_FN_NPCSERVER_SUMMON_JACO_MONSTER, pSJacoMonster, SendBuf);
				pSJacoMonster->Belligerence0 = (BELL_INFLUENCE_VCN == pSummonData->pMonsterInfo->Belligerence) ? BELL_INFLUENCE_ANI : BELL_INFLUENCE_VCN;
				pSJacoMonster->IsSummonJacoMonster = FALSE;

				// 2007-08-22 by cmkwon, �Լ��� ����(<-Send2NPCServer)
				m_pFieldIOCP14->Send2NPCServerByTCP(SendBuf, MSG_SIZE(MSG_FN_NPCSERVER_SUMMON_JACO_MONSTER));

				if (BELL_INFLUENCE_VCN == pSummonData->pMonsterInfo->Belligerence)
				{// 2007-02-06 by dhjin, ��ȯ�� ��ȯ�� ������ ������.
					m_SummonVCNBossMonsterInfo.bSummonBossMonster = TRUE;
					m_SummonVCNBossMonsterInfo.SummonBossTime = now;
					m_SummonVCNBossMonsterInfo.SummonMonsterUnitkind = pSummonData->pMonsterInfo->MonsterUnitKind;
					m_SummonVCNBossMonsterInfo.ContributionPoint = SetMSWarInfoContributionPoint(pSummonData->pMonsterInfo->MonsterUnitKind);	// 2008-04-22 by dhjin, ����, ������ ����â ��ȹ�� - ��������Ʈ
					m_SummonVCNBossMonsterInfo.BossStep = pSummonData->BossStep;			// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - �� �ܰ�
					m_SummonVCNBossMonsterInfo.BeforeWinCheck = pSummonData->BeforeWinCheck;				// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - �� �ܰ� ���� ����
				}
				else
				{
					m_SummonANIBossMonsterInfo.bSummonBossMonster = TRUE;
					m_SummonANIBossMonsterInfo.SummonBossTime = now;
					m_SummonANIBossMonsterInfo.SummonMonsterUnitkind = pSummonData->pMonsterInfo->MonsterUnitKind;
					m_SummonANIBossMonsterInfo.ContributionPoint = SetMSWarInfoContributionPoint(pSummonData->pMonsterInfo->MonsterUnitKind);	// 2008-04-22 by dhjin, ����, ������ ����â ��ȹ�� - ��������Ʈ
					m_SummonANIBossMonsterInfo.BossStep = pSummonData->BossStep;			// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - �� �ܰ�
					m_SummonANIBossMonsterInfo.BeforeWinCheck = pSummonData->BeforeWinCheck;				// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - �� �ܰ� ���� ����
				}

				///////////////////////////////////////////////////////////////////////////////
				// 2006-04-14 by cmkwon
				itr = m_mtvectSummonMonsterDataList.erase(itr);

				// 2012-07-02 by hskim, ���� ���� - ���� ����� ������ ���� ���� �Ұ�
				auto pFMPro = reinterpret_cast<CFieldMapProject*>(m_pFieldIOCP14->GetMapWorkspace()->GetMapProjectByMapIndex(STONES_RUIN_MAP_INDEX));

				if (pFMPro)
				{
					auto m_MaxChannelPerMap = pFMPro->GetNumberOfMapChannels();

					for (auto ChannelCount = 0; ChannelCount < m_MaxChannelPerMap; ChannelCount++)
					{
						auto pTargetFieldMapChannel = pFMPro->GetFieldMapChannelByIndex(ChannelCount);

						if (pTargetFieldMapChannel)

							pTargetFieldMapChannel->WarpToCityMapFieldMapChannelByInfluence(INFLUENCE_TYPE_UNKNOWN);
					}
				}
				// 2012-07-02 by hskim, ���� ���� - ���� ����� ������ ���� ���� �Ұ�

				continue;
			}

			if (5 == nRemainMinute % 10 || 3 == nRemainMinute || 1 == nRemainMinute)
			{
				///////////////////////////////////////////////////////////////////////////////
				// 2006-04-14 by cmkwon, ������ ���� ���� ��ȯ ���� Ŭ���̾�Ʈ�� ����
				INIT_MSG_WITH_BUFFER(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA, T_FC_WAR_BOSS_MONSTER_SUMMON_DATA, pSWarData, SendBuf);
				pSWarData->SummonMonsterUnitkind = pSummonData->pMonsterInfo->MonsterUnitKind;
				pSWarData->RemainMinute = nRemainMinute;
				pSWarData->BossStep = pSummonData->BossStep;		// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - �� �ܰ�
				pSWarData->BeforeWinCheck = pSummonData->BeforeWinCheck;				// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - �� �ܰ� ���� ����
				m_pFieldIOCP14->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA), INFLUENCE_TYPE_VCN | INFLUENCE_TYPE_ANI);
			}

			itr++;
		}
		
		SetDoingInfluenceWar();	// 2012-11-26 by jhseol, ���� �ý��� ������ - ����� Ʈ���� ��������(�Ͻ�����), ������ ���������� Ȯ���ϴ� �Լ�.(���� üũ�� ����)
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::OnInfluenceBossMonsterDeadInflWarManager(MONSTER_INFO *pMonInfo)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-14 ~ 2006-04-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::OnInfluenceBossMonsterDeadInflWarManager(MONSTER_INFO *pMonInfo)
{
	if(NULL == pMonInfo){					return;}
	
	// 2007-08-23 by cmkwon, ���� ���� ���� ���� ó�� - �Ʒ��� ���� ������, �߰��� ������ ���
	//if(FALSE == IS_INFLWAR_MONSTER(pMonInfo->Belligerence))
	if(FALSE == IS_MOTHERSHIPWAR_MONSTER(pMonInfo->Belligerence))
	{// 2006-11-20 by cmkwon

		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2006-12-13 by cmkwon, ���Ϳ� ���� ������ InfluenceType�� ���Ѵ�.
	BYTE byInflTy = INFLUENCE_TYPE_UNKNOWN;
	BYTE byLoseInflTy = INFLUENCE_TYPE_UNKNOWN;		// 2008-01-07 by dhjin, ���� ���� ���� - 
	if(IS_BELL_VCN(pMonInfo->Belligerence))	
	{
		byInflTy	= INFLUENCE_TYPE_ANI;
		byLoseInflTy = INFLUENCE_TYPE_VCN;		// 2008-01-07 by dhjin, ���� ���� ���� - 
	}
	else
	{
		byInflTy	= INFLUENCE_TYPE_VCN;
		byLoseInflTy = INFLUENCE_TYPE_ANI;		// 2008-01-07 by dhjin, ���� ���� ���� - 
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-14 by cmkwon, 
	CInflWarData *pInflWarData = GetInflWarDataByInflType(byInflTy);
	if(NULL == pInflWarData)
	{
		return;
	}

	// 2007-02-06 by dhjin, ��ȯ�� ���� (����)�� ���� �Ǿ����Ƿ�  �ʱ�ȭ �Ѵ�.
	if(IS_BELL_VCN(pMonInfo->Belligerence))
	{
		util::zero(&m_SummonVCNBossMonsterInfo, sizeof(SSUMMONBOSSMONSTER_INFO));
	}
	else
	{
		util::zero(&m_SummonANIBossMonsterInfo, sizeof(SSUMMONBOSSMONSTER_INFO));
	}

	SetDoingInfluenceWar();	// 2012-11-26 by jhseol, ���� �ý��� ������ - ����� Ʈ���� ��������(�Ͻ�����), ���� ���Ḧ �ǽð����� Ȯ�� �ϱ� ���� �Լ�ȣ��(���� ����)

	mt_auto_lock mtA(pInflWarData->GetmtlockPtr());	

	// 2013-05-09 by hskim, ���� ����Ʈ ����
	SDB_INFLUENCE_WAR_INFO *pNewInfo = pInflWarData->GetCurInflWarInfo();		// ��ġ ����

#ifdef S_UPGRADE_INFLUENCE_POINT_HSKIM
	CalcContributionPoint(pNewInfo->InfluenceType, CONTRIBUTIONPOINT_BOSS_MONSTER_WINNER, CONTRIBUTIONPOINT_BOSS_MONSTER_LOSER);
	CalcConsecutiveVictoriesPoint(pNewInfo->InfluenceType);
#else
	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-14 by cmkwon, ������ �⿩�� ����
	pInflWarData->ContributionPoint += ADD_CONTRIBUTIONPOINT_BOSS_MONSTER;
	int nContributionPoint = pInflWarData->ContributionPoint;
#endif
// end 2013-05-09 by hskim, ���� ����Ʈ ����

//////////////////////////////////////////////////////////////////////////
// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - �ذ� ���� ���ôܰ踦 �����Ͽ� ������ �����Ѵ�.
//////////////////////////////////////////////////////////////////////////
// 2009-01-12 by dhjin, ���� ���� - �ذ� ���� ���� ���ôܰ�� ���⼭ �������� �ʴ´�. ���� �ֱ�� �ٲ� ������.
//	///////////////////////////////////////////////////////////////////////////////
//	// 2006-04-17 by cmkwon, ���� ���� ���߷� ���ôܰ� �ٽ� ������
//	SDB_INFLUENCE_WAR_INFO *pNewInfo = pInflWarData->GetInflWarInfoByContributionPoint(pInflWarData->ContributionPoint);
//	pInflWarData->SetWartimeStage(pNewInfo);
//	SDB_INFLUENCE_WAR_INFO *pNewInfo = pInflWarData->GetCurInflWarInfo();

// 2009-07-02 by cmkwon, �������� ���� ���� ���� - �Ʒ��� ���� ������.
// 	SDB_INFLUENCE_WAR_INFO *pNewInfo;
// 	if(INFLUENCE_TYPE_VCN == byInflTy)
// 	{
// 		pInflWarData->SetCurrentStepInflWarData(this->m_DeclarationOfWar.GetCurrentMSUID(INFLUENCE_TYPE_VCN), INFLUENCE_TYPE_VCN);
// 		pNewInfo = pInflWarData->GetCurInflWarInfo();
// //		CInflWarData *pBCUInflWarData = GetInflWarDataByInflType(INFLUENCE_TYPE_VCN);
// //		if(NULL == pBCUInflWarData)
// //		{
// //			return;
// //		}
// //		pBCUInflWarData->SetCurrentStepInflWarData(this->m_DeclarationOfWar.GetCurrentMSUID(INFLUENCE_TYPE_VCN), INFLUENCE_TYPE_VCN);
// //		pNewInfo = pBCUInflWarData->GetCurInflWarInfo();
// 	}
// 	else
// 	{
// 		pInflWarData->SetCurrentStepInflWarData(this->m_DeclarationOfWar.GetCurrentMSUID(INFLUENCE_TYPE_ANI), INFLUENCE_TYPE_ANI);
// 		pNewInfo = pInflWarData->GetCurInflWarInfo();		
// //		CInflWarData *pANIInflWarData = GetInflWarDataByInflType(INFLUENCE_TYPE_ANI);
// //		if(NULL == pANIInflWarData)
// //		{
// //			return;
// //		}
// //		pANIInflWarData->SetCurrentStepInflWarData(this->m_DeclarationOfWar.GetCurrentMSUID(INFLUENCE_TYPE_ANI), INFLUENCE_TYPE_ANI);
// //		pNewInfo = pANIInflWarData->GetCurInflWarInfo();		
// 	}
	///////////////////////////////////////////////////////////////////////////////
	// 2009-07-02 by cmkwon, �������� ���� ���� ���� - 	
	pInflWarData->SetCurrentStepInflWarData(pMonInfo->MonsterUnitKind, byInflTy);
	this->SetCompensationForDecalationOfWarWin(byInflTy, pMonInfo->MonsterUnitKind);			// 2009-07-02 by cmkwon, �������� ���� ���� ���� - 

// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - ������ �¸� �� ���� �Ǵ� �̺�Ʈ ���� - SetCurrentStepInflWarData���� �Ҵ��ϹǷ� �ּ�ó���Ѵ�.
//////////////////////////////////////////////////////////////////////////
// 2009-03-31 by dhjin, ���� ���� - ������ �¸� �� ���� �Ǵ� �̺�Ʈ ���� - ���� �̺�Ʈ�� �����ϰ� Ŭ���̾�Ʈ���� �����Ѵ�.
//	pInflWarData->fInflHPRepairRate = pNewInfo->HPRepairRate;
//	pInflWarData->fInflDPRepairRate = pNewInfo->DPRepairRate;
//	pInflWarData->fInflSPRepairRate = pNewInfo->SPRepairRate;
	INIT_MSG_WITH_BUFFER(MSG_FC_WAR_INFLUENCE_DATA, T_FC_WAR_INFLUENCE_DATA, pSInflData, SendBufInflData);
	pSInflData->byInfluenceType		= pNewInfo->InfluenceType;
	pSInflData->fHPRepairRate		= pInflWarData->fInflHPRepairRate;
	pSInflData->fDPRepairRate		= pInflWarData->fInflDPRepairRate;
	pSInflData->fSPRepairRate		= pInflWarData->fInflSPRepairRate;
	m_pFieldIOCP14->SendMessageToAllClients(SendBufInflData, MSG_SIZE(MSG_FC_WAR_INFLUENCE_DATA), pNewInfo->InfluenceType);
	mtA.auto_unlock_cancel();

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-27 by cmkwon
	char szSysLog[1024];
	wsprintf(szSysLog, "  [Notify]: InflWar OnInflBossMonsterDead InflTy[%d] CurWartimeStage(%d)\r\n"
		, pNewInfo->InfluenceType, pInflWarData->WartimeStage);
	g_pFieldGlobal->WriteSystemLog(szSysLog);
	DBGOUT(szSysLog);

// 2013-05-09 by hskim, ���� ����Ʈ ����
#ifdef S_UPGRADE_INFLUENCE_POINT_HSKIM
#else
	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-19 by cmkwon, ���ø� �������� �⿩�� ����
	INIT_MSG_WITH_BUFFER(MSG_FC_WAR_UPDATE_CONTRIBUTION_POINT_OK, T_FC_WAR_UPDATE_CONTRIBUTION_POINT_OK, pSConPoint, SendBuf);
	pSConPoint->byInfluenceType		= pNewInfo->InfluenceType;
	pSConPoint->nContributionPoint	= nContributionPoint;
	m_pFieldIOCP14->SendMessageToCityClients(SendBuf, MSG_SIZE(MSG_FC_WAR_UPDATE_CONTRIBUTION_POINT_OK), pSConPoint->byInfluenceType);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-25 by cmkwon, DB�� �⿩�� ����
	QPARAM_UPDATE_INFLUENCE_WAR_DATA *pS2DB = new QPARAM_UPDATE_INFLUENCE_WAR_DATA;
	pS2DB->InfluenceType0		= pNewInfo->InfluenceType;
	//pS2DB->MGameServerID0		= g_pFieldGlobal->GetMGameServerID();
	pS2DB->WartimeStage0		= pNewInfo->WartimeStage;
	pS2DB->ContributionPoint0	= nContributionPoint;
	m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateInfluenceWarData, NULL, 0, pS2DB);
#endif
// end 2013-05-09 by hskim, ���� ����Ʈ ����

//////////////////////////////////////////////////////////////////////////
// 2009-01-12 by dhjin, ���� ���� - �ذ� ���� �ּ� ó��(DB�� �� ������ �ʿ� ����.)
//	///////////////////////////////////////////////////////////////////////////////
//	// 2006-07-12 by cmkwon, ���� ���� ���� ���� ����
//	QPARAM_UPDATE_OWNEROFCONFLICTAREA *pOwner2DB = new QPARAM_UPDATE_OWNEROFCONFLICTAREA;
//	pOwner2DB->InfluenceType0	= pNewInfo->InfluenceType;
//	pOwner2DB->MGameServerID0	= g_pFieldGlobal->GetMGameServerID();
//	m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateOwnerOfConflictArea, NULL, 0, pOwner2DB);

// 2009-03-31 by dhjin, ���� ���� - ������ �¸� �� ���� �Ǵ� �̺�Ʈ ���� - ���� �ø�
//	///////////////////////////////////////////////////////////////////////////////
//	// 2006-04-21 by cmkwon
//	INIT_MSG(MSG_FC_WAR_INFLUENCE_DATA, T_FC_WAR_INFLUENCE_DATA, pSInflData, SendBuf);
//	pSInflData->byInfluenceType		= pNewInfo->InfluenceType;
//	pSInflData->fHPRepairRate		= pNewInfo->HPRepairRate;
//	pSInflData->fDPRepairRate		= pNewInfo->DPRepairRate;
//	pSInflData->fSPRepairRate		= pNewInfo->SPRepairRate;
//	m_pFieldIOCP14->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_WAR_INFLUENCE_DATA), pNewInfo->InfluenceType);

// 2008-05-20 by cmkwon, ��� �̺�Ʈ(HappyHoure,MotherShip,Item) �׷� ���ÿ� �����ϰ� - ���� �̺�Ʈ ����
// 	///////////////////////////////////////////////////////////////////////////////
// 	// 2006-04-14 by cmkwon,
// 	if(0 < pNewInfo->EventTime
// 		&& FALSE == m_pFieldIOCP14->m_gameEventManager.IsDoingAllGameEvent(INFLUENCE_TYPE_ALL_MASK))		// 2006-04-21 by cmkwon, ��ü �̺�Ʈ�� �������� �ƴҶ���
// 	{// 2006-04-17 by cmkwon, ���� �̺�Ʈ ó��
// 
// 		char szTemp[1024];
// 		if(m_pFieldIOCP14->m_gameEventManager.StartGameEvent(GAME_EVENT_TYPE_EXP, pNewInfo->EventExperience, pNewInfo->EventTime, 1, CHARACTER_MAX_LEVEL, byInflTy))	// 2008-03-14 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� -
// 		{
// 			sprintf(szTemp, STRMSG_S_F2NOTIFY_0116
// 				, CGameEventManager::GetGameEventTypeString(GAME_EVENT_TYPE_EXP), pNewInfo->EventExperience, pNewInfo->EventTime);
// 			m_pFieldIOCP14->SendString128ToAllClients(STRING_128_USER_NOTICE, szTemp, TRUE, byInflTy);
// 		}
// 		if(m_pFieldIOCP14->m_gameEventManager.StartGameEvent(GAME_EVENT_TYPE_SPI, pNewInfo->EventDropSPI, pNewInfo->EventTime, 1, CHARACTER_MAX_LEVEL, byInflTy))		// 2008-03-14 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� -
// 		{
// 			sprintf(szTemp, STRMSG_S_F2NOTIFY_0116
// 				, CGameEventManager::GetGameEventTypeString(GAME_EVENT_TYPE_SPI), pNewInfo->EventDropSPI, pNewInfo->EventTime);
// 			m_pFieldIOCP14->SendString128ToAllClients(STRING_128_USER_NOTICE, szTemp, TRUE, byInflTy);
// 		}
// 		if(m_pFieldIOCP14->m_gameEventManager.StartGameEvent(GAME_EVENT_TYPE_EXP_NO_DOWN, pNewInfo->EventDownExperience, pNewInfo->EventTime, 1, CHARACTER_MAX_LEVEL, byInflTy))		// 2008-03-14 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� -
// 		{
// 			sprintf(szTemp, STRMSG_S_F2NOTIFY_0116
// 				, CGameEventManager::GetGameEventTypeString(GAME_EVENT_TYPE_EXP_NO_DOWN), pNewInfo->EventDownExperience, pNewInfo->EventTime);
// 			m_pFieldIOCP14->SendString128ToAllClients(STRING_128_USER_NOTICE, szTemp, TRUE, byInflTy);
// 		}
// 		if(m_pFieldIOCP14->m_gameEventManager.StartGameEvent(GAME_EVENT_TYPE_DROP_ITEM, pNewInfo->EventDropItem, pNewInfo->EventTime, 1, CHARACTER_MAX_LEVEL, byInflTy))			// 2008-03-14 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� -
// 		{
// 			sprintf(szTemp, STRMSG_S_F2NOTIFY_0116
// 				, CGameEventManager::GetGameEventTypeString(GAME_EVENT_TYPE_DROP_ITEM), pNewInfo->EventDropItem, pNewInfo->EventTime);
// 			m_pFieldIOCP14->SendString128ToAllClients(STRING_128_USER_NOTICE, szTemp, TRUE, byInflTy);
// 		}
// 		if(m_pFieldIOCP14->m_gameEventManager.StartGameEvent(GAME_EVENT_TYPE_DROP_REAR_ITEM, pNewInfo->EventDropRareItem, pNewInfo->EventTime, 1, CHARACTER_MAX_LEVEL, byInflTy))		// 2008-03-14 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� -
// 		{
// 			sprintf(szTemp, STRMSG_S_F2NOTIFY_0116
// 				, CGameEventManager::GetGameEventTypeString(GAME_EVENT_TYPE_DROP_REAR_ITEM), pNewInfo->EventDropRareItem, pNewInfo->EventTime);
// 			m_pFieldIOCP14->SendString128ToAllClients(STRING_128_USER_NOTICE, szTemp, TRUE, byInflTy);
// 		}
// 	}
	///////////////////////////////////////////////////////////////////////////////	
	// 2008-05-20 by cmkwon, ��� �̺�Ʈ(HappyHoure,MotherShip,Item) �׷� ���ÿ� �����ϰ� - 
	if(pNewInfo->IsValidGameEvent())
	{
		if(FALSE == m_pFieldIOCP14->m_gameEventManager.IsProgressGameEventGroup(byInflTy, GAME_EVENT_GROUP_MOTHERSHIP))
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify]: GAME_EVENT_GROUP_MOTHERSIP start ! WarTimeStage(%d) InfluenceType(%d)\r\n", pNewInfo->WartimeStage, pNewInfo->InfluenceType);

			m_pFieldIOCP14->m_gameEventManager.StartGameEvent(GAME_EVENT_TYPE_EXP, pNewInfo->EventExperience, pNewInfo->EventTime, 1, CHARACTER_MAX_LEVEL, byInflTy, GAME_EVENT_GROUP_MOTHERSHIP);
 			m_pFieldIOCP14->m_gameEventManager.StartGameEvent(GAME_EVENT_TYPE_SPI, pNewInfo->EventDropSPI, pNewInfo->EventTime, 1, CHARACTER_MAX_LEVEL, byInflTy, GAME_EVENT_GROUP_MOTHERSHIP);
 			m_pFieldIOCP14->m_gameEventManager.StartGameEvent(GAME_EVENT_TYPE_EXP_NO_DOWN, pNewInfo->EventDownExperience, pNewInfo->EventTime, 1, CHARACTER_MAX_LEVEL, byInflTy, GAME_EVENT_GROUP_MOTHERSHIP);
 			m_pFieldIOCP14->m_gameEventManager.StartGameEvent(GAME_EVENT_TYPE_DROP_ITEM, pNewInfo->EventDropItem, pNewInfo->EventTime, 1, CHARACTER_MAX_LEVEL, byInflTy, GAME_EVENT_GROUP_MOTHERSHIP);
 			m_pFieldIOCP14->m_gameEventManager.StartGameEvent(GAME_EVENT_TYPE_DROP_REAR_ITEM, pNewInfo->EventDropRareItem, pNewInfo->EventTime, 1, CHARACTER_MAX_LEVEL, byInflTy, GAME_EVENT_GROUP_MOTHERSHIP);

			// 2008-05-20 by cmkwon, ��� �̺�Ʈ(HappyHoure,MotherShip,Item) �׷� ���ÿ� �����ϰ� - �ش� ���� �������� ����
			INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, pSHappy, SendBuf);		// 2013-05-09 by hskim, ���� ����Ʈ ����
			pSHappy->byStateType		= GAME_EVENT_GROUP_MOTHERSHIP_START;
			pSHappy->byInfluenceType4	= byInflTy;
			pSHappy->StartATime.SetCurrentDateTime(TRUE);
			pSHappy->EndATime			= pSHappy->StartATime;
			pSHappy->EndATime.AddDateTime(0, 0, 0, 0, pNewInfo->EventTime);
			pSHappy->fEXPRate2			= pNewInfo->EventExperience;
			pSHappy->fSPIRate2			= pNewInfo->EventDropSPI;
			pSHappy->fEXPRepairRate2	= pNewInfo->EventDownExperience;
			pSHappy->fDropItemRate2		= pNewInfo->EventDropItem;
			pSHappy->fDropRareRate2		= pNewInfo->EventDropRareItem;
			m_pFieldIOCP14->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK), pSHappy->byInfluenceType4);
		}
		else
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify]: GAME_EVENT_GROUP_MOTHERSIP error (progress other GameEvent)!! WarTimeStage(%d) InfluenceType(%d)\r\n", pNewInfo->WartimeStage, pNewInfo->InfluenceType);
		}
	}

// 2012-07-12 by hskim, ���� ����� ������ �߰� ���� - ������ ���� (7037060)
#if S_MS_WAR_FIN_SUPPLY_ITEM_ADD
	// ���� ����� ������ ���� (7037060) �ϰ� ����
	const int nItemNumAdd = 7037060;
	ITEM *pItemAdd = m_pFieldIOCP14->GetItemInfo(nItemNumAdd);
	if(pItemAdd)
	{
		m_pFieldIOCP14->InsertItemAllCharacter(nItemNumAdd, 1, pNewInfo->InfluenceType, TRUE);
	}
#endif
// end 2012-07-12 by hskim, ���� ����� ������ �߰� ���� - ������ ���� (7037060)

	if(0 != pNewInfo->SupplyItemNum
		&& 0 < pNewInfo->SupplyItemCount)
	{// 2006-04-17 by cmkwon, �ƾ��� ����
		ITEM *pItem = m_pFieldIOCP14->GetItemInfo(pNewInfo->SupplyItemNum);
		if(pItem)
		{
			m_pFieldIOCP14->InsertItemAllCharacter(pNewInfo->SupplyItemNum, pNewInfo->SupplyItemCount, pNewInfo->InfluenceType, TRUE);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-16 by dhjin, �߰��� 2��° ������ ������ �����ϸ� ����
	if(0 != pNewInfo->SupplyItemNum2
		&& 0 < pNewInfo->SupplyItemCount2)
	{// 2006-04-17 by cmkwon, �ƾ��� ����
		ITEM *pItem = m_pFieldIOCP14->GetItemInfo(pNewInfo->SupplyItemNum2);
		if(pItem)
		{
			m_pFieldIOCP14->InsertItemAllCharacter(pNewInfo->SupplyItemNum2, pNewInfo->SupplyItemCount2, pNewInfo->InfluenceType, TRUE);
		}
	}

	// 2006-04-17 by cmkwon, �߱޺��� ���� ��ȯ �̺�Ʈ
	for(int i=0; i<MAX_INFLUENCEWAR_EVENT_SUMMON_COUNT; i++)
	{
		if(0 >= pNewInfo->SummonMonsterUnitKind[i]
			|| 0 >= pNewInfo->SummonCount[i]
			|| 0 >= pNewInfo->SummonMapIndex[i])
		{
			continue;
		}
		
		MONSTER_INFO *pMonInfo = m_pFieldIOCP14->GetMonsterInfo(pNewInfo->SummonMonsterUnitKind[i]);
		if(NULL == pMonInfo)
		{
			continue;
		}
		CFieldMapProject *pFMapPro = m_pFieldIOCP14->GetFieldMapProjectByMapIndex(pNewInfo->SummonMapIndex[i]);
		if(pFMapPro)
		{// 2006-04-17 by cmkwon, ��� ä�ο� ���͸� ��ȯ�Ѵ�.
			MSG_FN_ADMIN_SUMMON_MONSTER tmSummon;
				util::zero(&tmSummon, sizeof(MSG_FN_ADMIN_SUMMON_MONSTER));

			tmSummon.ChannelIndex			= 0;
			util::strncpy(tmSummon.CharacterName, "InfluenceWar", SIZE_MAX_CHARACTER_NAME);
			tmSummon.MonsterUnitKind		= pNewInfo->SummonMonsterUnitKind[i];
			tmSummon.NumOfMonster			= pNewInfo->SummonCount[i];
			tmSummon.Position				= AVECTOR3(pNewInfo->SummonPositionX[i], pNewInfo->SummonPositionY[i], pNewInfo->SummonPositionZ[i]);

			// 2010. 07. 05 by hsLee. ���� ��ȯ ����. (�뷱�� ������ ���� ����.)
			tmSummon.MonsterBalanceData.Init(0);

			pFMapPro->SendSummonMessageToAllChannels(&tmSummon);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-15 by dhjin, WarPoint ���� (300��)
	m_pFieldIOCP14->SendWarPointToInflClient(byInflTy, ADD_WARPOINT_BOSS_MONSTER);
	m_pFieldIOCP14->SendWarPointToInflClient(byLoseInflTy, ADD_WARPOINT_BOSS_MONSTER_BY_LOSE);	// 2008-01-07 by dhjin, ���� ���� ���� - 
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::OnInfluenceBossMonsterAutoDestroyedInflWarManager(MONSTER_INFO *pMonInfo)
/// \brief		���� ���ÿ��� ������ �����Ѵ�. ���� ����Ʈ, ���� ����Ʈ, �̽��͸� ĸ��
/// \author		dhjin
/// \date		2007-05-15 ~ 2007-05-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::OnInfluenceBossMonsterAutoDestroyedInflWarManager(MONSTER_INFO *pMonInfo)
{
	if(NULL == pMonInfo){					return;}
	
	// 2007-08-23 by cmkwon, ���� ���� ���� ���� ó�� - �Ʒ��� ���� ������, �߰��� ������ ���
	//if(FALSE == IS_INFLWAR_MONSTER(pMonInfo->Belligerence))
	if(FALSE == IS_MOTHERSHIPWAR_MONSTER(pMonInfo->Belligerence))
	{// 2006-11-20 by cmkwon

		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2006-12-13 by cmkwon, ���Ϳ� ���� ������ InfluenceType�� ���Ѵ�.
	BYTE byInflTy = INFLUENCE_TYPE_UNKNOWN;
	BYTE byWinInflTy = INFLUENCE_TYPE_UNKNOWN;		// 2007-05-17 by dhjin, ������� ���� �� ������ ���� ��� ���°��� �ݴ� �����̹Ƿ� ���� ���� �� ���� �Ѵ�

	if(IS_BELL_VCN(pMonInfo->Belligerence))
	{
		byInflTy	= INFLUENCE_TYPE_ANI;
		byWinInflTy	= INFLUENCE_TYPE_VCN;
	}
	else
	{
		byInflTy	= INFLUENCE_TYPE_VCN;
		byWinInflTy = INFLUENCE_TYPE_ANI;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-14 by cmkwon, 
	CInflWarData *pInflWarData = GetInflWarDataByInflType(byInflTy);
	if(NULL == pInflWarData)
	{
		return;
	}
	CInflWarData *pWinInflWarData = GetInflWarDataByInflType(byWinInflTy);
	if(NULL == pWinInflWarData)
	{
		return;
	}

	// 2007-02-06 by dhjin, ��ȯ�� ���� (����)�� ���� �Ǿ����Ƿ�  �ʱ�ȭ �Ѵ�.
	if(IS_BELL_VCN(pMonInfo->Belligerence))
	{
		util::zero(&m_SummonVCNBossMonsterInfo, sizeof(SSUMMONBOSSMONSTER_INFO));
	}
	else
	{
		util::zero(&m_SummonANIBossMonsterInfo, sizeof(SSUMMONBOSSMONSTER_INFO));
	}
	
	SetDoingInfluenceWar();	// 2013-04-17 by jhseol, �� ����� Ʈ���� �̿� �Ұ��� ���� ���׼��� (�� �ڵ� �ı� �� ���� ���� üũ�� �ϵ��� �߰�)

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-17 by cmkwon, ���� ���� ������ ���´�.
	SDB_INFLUENCE_WAR_INFO *pNewInfo = pInflWarData->GetInflWarInfoByContributionPoint(pInflWarData->ContributionPoint);

// 2013-05-09 by hskim, ���� ����Ʈ ����
#ifdef S_UPGRADE_INFLUENCE_POINT_HSKIM
	CalcContributionPoint(pNewInfo->InfluenceType, CONTRIBUTIONPOINT_BOSS_MONSTER_WINNER, CONTRIBUTIONPOINT_BOSS_MONSTER_LOSER);
	CalcConsecutiveVictoriesPoint(pNewInfo->InfluenceType);		// 2013-05-09 by hskim, ���� ����Ʈ ����
#else
	mt_auto_lock mtA(pWinInflWarData->GetmtlockPtr());	
	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-14 by cmkwon, ������ �⿩�� ����
	pWinInflWarData->ContributionPoint += ADD_CONTRIBUTIONPOINT_BOSS_MONSTER;
	int nContributionPoint = pWinInflWarData->ContributionPoint;
	mtA.auto_unlock_cancel();
#endif
// end 2013-05-09 by hskim, ���� ����Ʈ ����

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-27 by cmkwon
	char szSysLog[1024];
	wsprintf(szSysLog, "  [Notify]: InflWar OnInflBossMonsterAutoDestroyed InflTy[%d] CurWartimeStage(%d)\r\n"
		, byWinInflTy, pWinInflWarData->WartimeStage);
	g_pFieldGlobal->WriteSystemLog(szSysLog);
	DBGOUT(szSysLog);

// 2013-05-09 by hskim, ���� ����Ʈ ����
#ifdef S_UPGRADE_INFLUENCE_POINT_HSKIM
#else
	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-19 by cmkwon, ���ø� �������� �⿩�� ����
	INIT_MSG_WITH_BUFFER(MSG_FC_WAR_UPDATE_CONTRIBUTION_POINT_OK, T_FC_WAR_UPDATE_CONTRIBUTION_POINT_OK, pSConPoint, SendBuf);
	pSConPoint->byInfluenceType		= byWinInflTy;
	pSConPoint->nContributionPoint	= nContributionPoint;
	m_pFieldIOCP14->SendMessageToCityClients(SendBuf, MSG_SIZE(MSG_FC_WAR_UPDATE_CONTRIBUTION_POINT_OK), byWinInflTy);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-25 by cmkwon, DB�� �⿩�� ����
	QPARAM_UPDATE_INFLUENCE_WAR_DATA *pS2DB = new QPARAM_UPDATE_INFLUENCE_WAR_DATA;
	pS2DB->InfluenceType0		= byWinInflTy;
	//pS2DB->MGameServerID0		= g_pFieldGlobal->GetMGameServerID();
	pS2DB->WartimeStage0		= pWinInflWarData->WartimeStage;
	pS2DB->ContributionPoint0	= nContributionPoint;
	m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateInfluenceWarData, NULL, 0, pS2DB);
#endif
// end 2013-05-09 by hskim, ���� ����Ʈ ����

	if(0 != pNewInfo->SupplyItemNum
		&& 0 < pNewInfo->SupplyItemCount)
	{// 2006-04-17 by cmkwon, �ƾ��� ����
		ITEM *pItem = m_pFieldIOCP14->GetItemInfo(pNewInfo->SupplyItemNum);
		if(pItem)
		{
			m_pFieldIOCP14->InsertItemAllCharacter(pNewInfo->SupplyItemNum, pNewInfo->SupplyItemCount, byWinInflTy, TRUE);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-16 by dhjin, �߰��� 2��° ������ ������ �����ϸ� ����
	if(0 != pNewInfo->SupplyItemNum2
		&& 0 < pNewInfo->SupplyItemCount2)
	{// 2006-04-17 by cmkwon, �ƾ��� ����
		ITEM *pItem = m_pFieldIOCP14->GetItemInfo(pNewInfo->SupplyItemNum2);
		if(pItem)
		{
			m_pFieldIOCP14->InsertItemAllCharacter(pNewInfo->SupplyItemNum2, pNewInfo->SupplyItemCount2, byWinInflTy, TRUE);
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-05-15 by dhjin, WarPoint ���� (300��)
	m_pFieldIOCP14->SendWarPointToInflClient(byWinInflTy, ADD_WARPOINT_BOSS_MONSTER);
	m_pFieldIOCP14->SendWarPointToInflClient(byInflTy, ADD_WARPOINT_BOSS_MONSTER_BY_LOSE);	// 2008-01-07 by dhjin, ���� ���� ���� - 
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			vectDB_INFLUENCE_WAR_INFO *CInflWarManager::GetInflWarInfoByInflType(BYTE i_byInflTy)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
vectDB_INFLUENCE_WAR_INFO *CInflWarManager::GetInflWarInfoByInflType(BYTE i_byInflTy)
{
	if(INFLUENCE_TYPE_VCN == i_byInflTy)
	{
		return &m_vectVCNInflWarInfoList;
	}
	if(INFLUENCE_TYPE_ANI == i_byInflTy)
	{
		return &m_vectANIInflWarInfoList;
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			vectDB_INFLUENCE_WAR_INFO *CInflWarManager::GetVCNInflWarInfo(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
vectDB_INFLUENCE_WAR_INFO *CInflWarManager::GetVCNInflWarInfo(void)
{
	return GetInflWarInfoByInflType(INFLUENCE_TYPE_VCN);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			vectDB_INFLUENCE_WAR_INFO *CInflWarManager::GetANIInflWarInfo(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
vectDB_INFLUENCE_WAR_INFO *CInflWarManager::GetANIInflWarInfo(void)
{
	return GetInflWarInfoByInflType(INFLUENCE_TYPE_ANI);
}

struct sort_SDB_INFLUENCE_WAR_INFO_byWartimeStage
{
	bool operator()(SDB_INFLUENCE_WAR_INFO op1, SDB_INFLUENCE_WAR_INFO op2)
	{
		return op1.WartimeStage < op2.WartimeStage;		// �������� ����
	}
};
///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::InsertInfluenceWarInfo(SDB_INFLUENCE_WAR_INFO *i_pInflWarInfo)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::InsertInfluenceWarInfo(SDB_INFLUENCE_WAR_INFO *i_pInflWarInfo)
{
	if(INFLUENCE_TYPE_VCN != i_pInflWarInfo->InfluenceType
		&& INFLUENCE_TYPE_ANI != i_pInflWarInfo->InfluenceType)
	{// 2006-04-13 by cmkwon, ���� üũ
		return FALSE;
	}

	vectDB_INFLUENCE_WAR_INFO *pInflWarInfoList = this->GetInflWarInfoByInflType(i_pInflWarInfo->InfluenceType);
	if(NULL == pInflWarInfoList)
	{
		return FALSE;
	}

	for(int i=0; i < pInflWarInfoList->size(); i++)
	{
		if(i_pInflWarInfo->WartimeStage == (*pInflWarInfoList)[i].WartimeStage)
		{// 2006-04-13 by cmkwon, �̹� WartimeStage�� ���� �����Ͱ� �����Ѵ�.
			return FALSE;
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-13 by cmkwon, �߰��ϱ�
	pInflWarInfoList->push_back(*i_pInflWarInfo);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-13 by cmkwon, ���ôܰ� ������������ �����Ѵ�.
	sort(pInflWarInfoList->begin(), pInflWarInfoList->end(), sort_SDB_INFLUENCE_WAR_INFO_byWartimeStage());
	return TRUE;
}





///////////////////////////////////////////////////////////////////////////////
/// \fn			CInflWarData *CInflWarManager::GetInflWarDataByInflType(BYTE i_byInflTy)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CInflWarData *CInflWarManager::GetInflWarDataByInflType(BYTE i_byInflTy)
{
	if(INFLUENCE_TYPE_VCN == i_byInflTy)
	{
		return &m_VCNInflWarData;
	}
	else if(INFLUENCE_TYPE_ANI == i_byInflTy)
	{
		return &m_ANIInflWarData;
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL *CInflWarManager::GetINFLUENCE_WAR_INFO_ByInflType(SDB_INFLUENCE_WAR_INFO **o_ppInflWarInfo, BYTE i_byInflTy)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-21 ~ 2006-04-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::GetINFLUENCE_WAR_INFO_ByInflType(SDB_INFLUENCE_WAR_INFO **o_ppInflWarInfo, BYTE i_byInflTy)
{
	*o_ppInflWarInfo = NULL;

	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byInflTy);
	if(NULL == pInflWarData)
	{
		return FALSE;
	}

	SDB_INFLUENCE_WAR_INFO *pInflWarInfo = pInflWarData->GetCurInflWarInfo();
	if(NULL == pInflWarInfo)
	{
		return FALSE;
	}
	if(0 >= pInflWarInfo->HPRepairRate
		&& 0 >= pInflWarInfo->DPRepairRate
		&& 0 >= pInflWarInfo->SPRepairRate)
	{
		return FALSE;
	}

	*o_ppInflWarInfo = pInflWarInfo;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::InitInflWarData(SINFLUENCE_WAR_DATA *i_pInflWarData)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::InitInflWarData(SINFLUENCE_WAR_DATA *i_pInflWarData)
{
	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_pInflWarData->InfluenceType);
	if(NULL == pInflWarData)
	{
		return FALSE;
	}

	this->SetMSWarOptionTypeW(i_pInflWarData->InfluenceType, i_pInflWarData->MSWarOptionType);	// 2008-04-10 by dhjin, ���� ���� ǥ�� ��ȹ�� - 

	pInflWarData->InitCInflWarData(i_pInflWarData, GetInflWarInfoByInflType(i_pInflWarData->InfluenceType));
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SetCurrentStepInflWarDataW()
/// \brief		���� ���� - ���� �� �ܰ�� m_pCurInflWarInfo�� �����Ѵ�.
/// \author		dhjin
/// \date		2009-01-14 ~ 2009-01-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SetCurrentStepInflWarDataW()
{
	CInflWarData *pBCUInflWarData = GetInflWarDataByInflType(INFLUENCE_TYPE_VCN);
	if(NULL == pBCUInflWarData)
	{
		return;
	}
	// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - �ذ� ���� ����
//	pBCUInflWarData->SetCurrentStepInflWarData(this->m_DeclarationOfWar.GetCurrentMSWarStep(INFLUENCE_TYPE_VCN));
	pBCUInflWarData->SetCurrentStepInflWarData(this->m_DeclarationOfWar.GetCurrentMSUID(INFLUENCE_TYPE_VCN), INFLUENCE_TYPE_VCN);

	CInflWarData *pANIInflWarData = GetInflWarDataByInflType(INFLUENCE_TYPE_ANI);
	if(NULL == pANIInflWarData)
	{
		return;
	}
	// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - �ذ� ���� ����
//	pANIInflWarData->SetCurrentStepInflWarData(this->m_DeclarationOfWar.GetCurrentMSWarStep(INFLUENCE_TYPE_ANI));
	pANIInflWarData->SetCurrentStepInflWarData(this->m_DeclarationOfWar.GetCurrentMSUID(INFLUENCE_TYPE_ANI), INFLUENCE_TYPE_ANI);

	SDB_INFLUENCE_WAR_INFO *pBCUCurInfoByWartimeStage = pBCUInflWarData->GetCurInflWarInfo();
	SDB_INFLUENCE_WAR_INFO *pANICurInfoByWartimeStage = pANIInflWarData->GetCurInflWarInfo();
	m_DeclarationOfWar.SetBeforeWarStartTime(pBCUCurInfoByWartimeStage->ReqContributionPoint, pANICurInfoByWartimeStage->ReqContributionPoint);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CInflWarManager::GetContributionPoint(BYTE i_byInflTy)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CInflWarManager::GetContributionPoint(BYTE i_byInflTy)
{
	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byInflTy);
	if(NULL == pInflWarData)
	{
		return 0;
	}

	return pInflWarData->ContributionPoint;
}

//MS War Steps , when this count of NCP is reached, MS will spawn
int ncpMSPoints[] = {
	50000,
	100000,
	150000,
	200000,
	250000,
	300000,
	350000,
	400000,
	500000,
	550000,
	600000,
	650000,
	700000,
	750000,
	800000,
	850000,
	900000,
	950000
};

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::AddContributionPoint(BYTE i_byInflTy, INT i_nAddValue)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-13 ~ 2006-04-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::AddContributionPoint(BYTE i_byInflTy, INT i_nAddValue)
{
	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byInflTy);
	if(NULL == pInflWarData)
	{
		return FALSE;
	}

//	char	szSysLog[1024];		// 2009-01-12 by dhjin, ���� ���� - �� ���� ��� �������� �ּ�ó��
	
	mt_auto_lock mtA(pInflWarData->GetmtlockPtr());
	SDB_INFLUENCE_WAR_INFO *pCurInfoByWartimeStage = pInflWarData->GetCurInflWarInfo();
//////////////////////////////////////////////////////////////////////////
// 2009-01-12 by dhjin, ���� ���� - �� ���� ��� �������� �ּ�ó��
//	SDB_INFLUENCE_WAR_INFO *pCurInfo = pInflWarData->GetInflWarInfoByContributionPoint(pInflWarData->ContributionPoint);
//	SDB_INFLUENCE_WAR_INFO *pNewInfo = pInflWarData->GetInflWarInfoByContributionPoint(pInflWarData->ContributionPoint + i_nAddValue);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-13 by cmkwon, �⿩���� ���Ѵ�.
	pInflWarData->ContributionPoint += i_nAddValue;
	int nContributionPoint = pInflWarData->ContributionPoint;

	mtA.auto_unlock_cancel();

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-19 by cmkwon, ���ø� �������� �⿩�� ����
	INIT_MSG_WITH_BUFFER(MSG_FC_WAR_UPDATE_CONTRIBUTION_POINT_OK, T_FC_WAR_UPDATE_CONTRIBUTION_POINT_OK, pSConPoint, SendBuf);
	pSConPoint->byInfluenceType		= pCurInfoByWartimeStage->InfluenceType;
	pSConPoint->nContributionPoint	= nContributionPoint;
	m_pFieldIOCP14->SendMessageToCityClients(SendBuf, MSG_SIZE(MSG_FC_WAR_UPDATE_CONTRIBUTION_POINT_OK), pSConPoint->byInfluenceType);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-14 by cmkwon, DB�� �⿩�� ����
	QPARAM_UPDATE_INFLUENCE_WAR_DATA *pS2DB = new QPARAM_UPDATE_INFLUENCE_WAR_DATA;
	pS2DB->InfluenceType0		= pCurInfoByWartimeStage->InfluenceType;
	//pS2DB->MGameServerID0		= g_pFieldGlobal->GetMGameServerID();
	pS2DB->WartimeStage0		= pCurInfoByWartimeStage->WartimeStage;
	pS2DB->ContributionPoint0	= nContributionPoint;
	m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateInfluenceWarData, NULL, 0, pS2DB);

#ifdef _NCP_BASED_MSWAR
	for (int i = 0; i < sizeof(ncpMSPoints); i++)
	{
		if (ncpMSPoints[i] == nContributionPoint 
			|| (ncpMSPoints[i] < nContributionPoint && ncpMSPoints[i] > (nContributionPoint - i_nAddValue))) //Make sure even if more than 1 ncp is added, the MS will spawn
		{
			if (COMPARE_INFLUENCE(pCurInfoByWartimeStage->InfluenceType, INFLUENCE_TYPE_VCN))
			{
				char buf[512];
				sprintf(buf, "\\eBCU\\r reached \\m%d NCP\\r. \\cHoros\\r Mothership War will start soon.", ncpMSPoints[i]);
				m_pFieldIOCP14->SendWorldNotification(buf, TRUE);
			}
			else if (COMPARE_INFLUENCE(pCurInfoByWartimeStage->InfluenceType, INFLUENCE_TYPE_ANI))
			{
				char buf[512];
				sprintf(buf, "\\cANI\\r reached \\m%d NCP\\r. \\eAnubis\\r Mothership War will start soon.", ncpMSPoints[i]);
				m_pFieldIOCP14->SendWorldNotification(buf, TRUE);
			}
			MSWarStart(pCurInfoByWartimeStage->InfluenceType);
		}
		else if (nContributionPoint == (ncpMSPoints[i] - 5000) || ((ncpMSPoints[i] - 5000) < nContributionPoint && (ncpMSPoints[i] - 5000) > (nContributionPoint - i_nAddValue))
			|| nContributionPoint == (ncpMSPoints[i] - 1000) || ((ncpMSPoints[i] - 1000) < nContributionPoint && (ncpMSPoints[i] - 1000) > (nContributionPoint - i_nAddValue))
			|| nContributionPoint == (ncpMSPoints[i] - 100) || ((ncpMSPoints[i] - 100) < nContributionPoint && (ncpMSPoints[i] - 100) > (nContributionPoint - i_nAddValue)))
		{
			if (COMPARE_INFLUENCE(pCurInfoByWartimeStage->InfluenceType, INFLUENCE_TYPE_VCN))
			{
				char buf[512];
				sprintf(buf, "\\y[\\rWARNING\\y] Only \\m%d NCP\\y left until the launch of \\cHoros\\y!", (ncpMSPoints[i] - nContributionPoint));
				m_pFieldIOCP14->SendWorldNotification(buf, TRUE);
			}
			else if (COMPARE_INFLUENCE(pCurInfoByWartimeStage->InfluenceType, INFLUENCE_TYPE_ANI))
			{
				char buf[512];
				sprintf(buf, "\\y[\\rWARNING\\y] Only \\m%d NCP\\y left until the launch of \\eAnubis\\y!", (ncpMSPoints[i] - nContributionPoint));
				m_pFieldIOCP14->SendWorldNotification(buf, TRUE);
			}
		}
	}
#endif

//////////////////////////////////////////////////////////////////////////
// 2009-01-12 by dhjin, ���� ���� - �� ���� ��� �������� �ּ�ó��
//	if(pCurInfo == pNewInfo)
//	{// 2006-04-14 by cmkwon, �������������� Ȥ�� ���ڸ��� ��ȯ ����
//		if(pCurInfo->IsSummonJacoMonster
//			&& FALSE == pInflWarData->IsSummonJacoMonster())
//		{
//
//			INIT_MSG(MSG_FN_NPCSERVER_SUMMON_JACO_MONSTER, T_FN_NPCSERVER_SUMMON_JACO_MONSTER, pSJacoMonster, SendBuf);
//			pSJacoMonster->Belligerence0	= GET_SAME_MONSTER_BELL_BY_CHARACTER_INFLTYPE(pNewInfo->InfluenceType);
//			pSJacoMonster->IsSummonJacoMonster	= TRUE;
//			// 2007-08-22 by cmkwon, �Լ��� ����(<-Send2NPCServer)
//			//if(m_pFieldIOCP14->Send2NPCServer(SendBuf, MSG_SIZE(MSG_FN_NPCSERVER_SUMMON_JACO_MONSTER)))
//			if(m_pFieldIOCP14->Send2NPCServerByTCP(SendBuf, MSG_SIZE(MSG_FN_NPCSERVER_SUMMON_JACO_MONSTER)))
//			{// 2006-04-25 by cmkwon, NPCServer�� ���� �����ÿ� 
//
//				pInflWarData->SetSummonJacoMonster(TRUE);
//
//				///////////////////////////////////////////////////////////////////////////////
//				// 2006-04-20 by cmkwon
//				INIT_MSG(MSG_FC_WAR_JACO_MONSTER_SUMMON, T_FC_WAR_JACO_MONSTER_SUMMON, pSJaco, SendBuf);
//				pSJaco->nBelligerence0			= GET_SAME_MONSTER_BELL_BY_CHARACTER_INFLTYPE(pNewInfo->InfluenceType);
//				m_pFieldIOCP14->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_WAR_JACO_MONSTER_SUMMON), INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);
//			}
//		}
//		return TRUE;
//	}
//
//	wsprintf(szSysLog, "  [Notify]: InflWar InflTy[%d] CurWartimeStage(%d) ChangeWartimeStage(%d) CurContributionPoint(%d) BossMonster(%8d) IsJacoSummon(%d)\r\n"
//		, pNewInfo->InfluenceType, pInflWarData->WartimeStage, pNewInfo->WartimeStage, nContributionPoint, pNewInfo->BossMonsterUnitKind, pNewInfo->IsSummonJacoMonster);
//	g_pFieldGlobal->WriteSystemLog(szSysLog);
//	DBGOUT(szSysLog);
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 2006-04-18 by cmkwon, ������ ���� ���� ��ȯ ó��
//	if(0 != pNewInfo->BossMonsterUnitKind)
//	{		
//		MONSTER_INFO *pBossMon = m_pFieldIOCP14->GetMonsterInfo(pNewInfo->BossMonsterUnitKind);
//		if(NULL == pBossMon
//			|| FALSE == IS_MOTHERSHIPWAR_MONSTER(pBossMon->Belligerence)
////			|| FALSE == IS_INFLWAR_MONSTER(pBossMon->Belligerence)			// 2007-08-21 by dhjin, ���� ���� ��, ��������Ʈ�� ���� �����ϰ� ����
//			|| FALSE == COMPARE_MPOPTION_BIT(pBossMon->MPOption, MPOPTION_BIT_BOSS_MONSTER))
//		{
//			char szErr[1024];
//			wsprintf(szErr, "CInflWarManager::AddContributionPoint_ Error, MonsterUnitKind(%8d) Belligerence(%d)"
//				, pNewInfo->BossMonsterUnitKind, (NULL==pBossMon)?0:pBossMon->Belligerence);
//			g_pFieldGlobal->WriteSystemLog(szErr);
//			DbgOut("%s\rn", szErr);
//			return TRUE;
//		}
//		
//		///////////////////////////////////////////////////////////////////////////////
//		// 2006-04-14 by cmkwon, ������ ���� ���� ��ȯ ���� �߰�
//		SINFLBOSS_MONSTER_SUMMON_DATA tmSummon;
//		tmSummon.pMonsterInfo		= pBossMon;
//		tmSummon.SummonCount		= pNewInfo->BossMonsterCount;
//		tmSummon.SummonTerm			= pNewInfo->BossMonsterSummonTerm;
//		tmSummon.atimeInsertedTime.SetCurrentDateTime(TRUE);
//		tmSummon.SummonMapIndex		= pNewInfo->BossMonsterSummonMapIndex;
//		tmSummon.SummonPosition.x	= pNewInfo->BossMonsterSummonPositionX;
//		tmSummon.SummonPosition.y	= pNewInfo->BossMonsterSummonPositionY;
//		tmSummon.SummonPosition.z	= pNewInfo->BossMonsterSummonPositionZ;
//		m_mtvectSummonMonsterDataList.pushBackLock(tmSummon);
//
//		///////////////////////////////////////////////////////////////////////////////
//		// 2006-04-14 by cmkwon, ������ ���� ���� ��ȯ ���� Ŭ���̾�Ʈ�� ����
//		INIT_MSG(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA, T_FC_WAR_BOSS_MONSTER_SUMMON_DATA, pSWarData, SendBuf);
//		pSWarData->SummonMonsterUnitkind	= pBossMon->MonsterUnitKind;
//		pSWarData->RemainMinute				= pNewInfo->BossMonsterSummonTerm;
//		m_pFieldIOCP14->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA), INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);
//
//		//////////////////////////////////////////////////////////////////////////
//		// 2008-04-01 by dhjin, ����, ������ ����â ��ȹ�� - 
//		this->SetMSWarInfoResultByWarStart(IS_VCN_INFLUENCE_TYPE(pNewInfo->InfluenceType) ? INFLUENCE_TYPE_VCN : INFLUENCE_TYPE_ANI, pNewInfo->BossMonsterUnitKind, pNewInfo->ReqContributionPoint);
//
//	}
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 2006-04-18 by cmkwon, ���ڸ��� ��ȯ ó��
//	if(FALSE != pNewInfo->IsSummonJacoMonster)
//	{
//		INIT_MSG(MSG_FN_NPCSERVER_SUMMON_JACO_MONSTER, T_FN_NPCSERVER_SUMMON_JACO_MONSTER, pSJacoMonster, SendBuf);
//		pSJacoMonster->Belligerence0	= GET_SAME_MONSTER_BELL_BY_CHARACTER_INFLTYPE(pNewInfo->InfluenceType);
//		pSJacoMonster->IsSummonJacoMonster	= TRUE;
//		// 2007-08-22 by cmkwon, �Լ��� ����(<-Send2NPCServer)
//		//if(m_pFieldIOCP14->Send2NPCServer(SendBuf, MSG_SIZE(MSG_FN_NPCSERVER_SUMMON_JACO_MONSTER)))
//		if(m_pFieldIOCP14->Send2NPCServerByTCP(SendBuf, MSG_SIZE(MSG_FN_NPCSERVER_SUMMON_JACO_MONSTER)))
//		{// 2006-04-25 by cmkwon, NPCServer�� ���� �����ÿ� 
//
//			pInflWarData->SetSummonJacoMonster(TRUE);
//
//			///////////////////////////////////////////////////////////////////////////////
//			// 2006-04-20 by cmkwon
//			INIT_MSG(MSG_FC_WAR_JACO_MONSTER_SUMMON, T_FC_WAR_JACO_MONSTER_SUMMON, pSJaco, SendBuf);
//			pSJaco->nBelligerence0			= GET_SAME_MONSTER_BELL_BY_CHARACTER_INFLTYPE(pNewInfo->InfluenceType);
//			m_pFieldIOCP14->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_WAR_JACO_MONSTER_SUMMON), INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);
//		}
//	}
	return TRUE;
}

// 2013-05-09 by hskim, ���� ����Ʈ ����
BOOL CInflWarManager::CalcContributionPoint(BYTE i_byWinnerInflTy, INT i_nWinnerValue, INT i_nLoserValue)
{
#ifdef S_UPGRADE_INFLUENCE_POINT_HSKIM
	BYTE byApplyInfl = INFLUENCE_TYPE_VCN;		// BCU ���� ����
	int CurrentDiffContributionPoint = 0;
	int nAbsDiff = 0;

	if( i_byWinnerInflTy != INFLUENCE_TYPE_VCN && i_byWinnerInflTy != INFLUENCE_TYPE_ANI )
	{
		return FALSE;
	}

	if( TRUE == COMPARE_INFLUENCE(i_byWinnerInflTy, INFLUENCE_TYPE_VCN) )
	{
		AddContributionPoint(INFLUENCE_TYPE_VCN, i_nWinnerValue);
		AddContributionPoint(INFLUENCE_TYPE_ANI, i_nLoserValue);
	}
	else
	{
		AddContributionPoint(INFLUENCE_TYPE_ANI, i_nWinnerValue);
		AddContributionPoint(INFLUENCE_TYPE_VCN, i_nLoserValue);
	}

	CurrentDiffContributionPoint = GetTurnAroundPoint();
	nAbsDiff = abs(CurrentDiffContributionPoint);

	if( CurrentDiffContributionPoint < 0 )
	{
		// ANI ���� ����
		byApplyInfl = INFLUENCE_TYPE_ANI;
	}

	m_pFieldIOCP14->DeleteBuffSkillAllCharacter(INFLUENCE_TYPE_VCN, DES_SKILLTYPE_TRUN_AROUND);
	m_pFieldIOCP14->DeleteBuffSkillAllCharacter(INFLUENCE_TYPE_ANI, DES_SKILLTYPE_TRUN_AROUND);

	// 2013-08-01 by jhseol, ������ ���� ������
	if( nAbsDiff > TURN_AROUND_BUFF_SKILL_NEW_THRESHOLD )
	{
		m_pFieldIOCP14->InsertBuffSkillAllCharacter(byApplyInfl, TURN_AROUND_BUFF_SKILL_1ST);
	}
	// �����ڵ� �ּ�����
// 	if( nAbsDiff >= TURN_AROUND_BUFF_SKILL_1ST_THRESHOLD )
// 	{
// 		if( nAbsDiff >= TURN_AROUND_BUFF_SKILL_2ND_THRESHOLD )
// 		{
// 			m_pFieldIOCP14->InsertBuffSkillAllCharacter(byApplyInfl, TURN_AROUND_BUFF_SKILL_2ND);
// 		}
// 		else
// 		{
// 			m_pFieldIOCP14->InsertBuffSkillAllCharacter(byApplyInfl, TURN_AROUND_BUFF_SKILL_1ST);
// 		}
// 	}
	// end 2013-08-01 by jhseol, ������ ���� ������

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] CalcContributionPoint :: WinnerInfluenceType [%d] WinnerContributionPoint [%d] LoserContributionPoint [%d]\r\n", 
		i_byWinnerInflTy, i_nWinnerValue, i_nLoserValue);
#endif

	return TRUE;
}

BOOL CInflWarManager::CalcConsecutiveVictoriesPoint(BYTE i_byWinnerInFlTy)
{
#ifdef S_UPGRADE_INFLUENCE_POINT_HSKIM
	// ���� üũ 
	if( i_byWinnerInFlTy != INFLUENCE_TYPE_VCN && i_byWinnerInFlTy != INFLUENCE_TYPE_ANI )
	{
		return FALSE;
	}	

	// ���� ����

	int ConsecutiveVictoriesBCU = 0;
	int ConsecutiveVictoriesANI = 0;

	int OldConsecutiveVictoriesBCU = 0;
	int OldConsecutiveVictoriesANI = 0;

	CInflWarData *pInflWarDataBCU = GetInflWarDataByInflType(INFLUENCE_TYPE_VCN);
	CInflWarData *pInflWarDataANI = GetInflWarDataByInflType(INFLUENCE_TYPE_ANI);

	if( NULL == pInflWarDataBCU || NULL == pInflWarDataANI )
	{
		return FALSE;
	}

	if( TRUE == COMPARE_INFLUENCE(i_byWinnerInFlTy, INFLUENCE_TYPE_VCN) )
	{
		mt_auto_lock mtA(pInflWarDataBCU->GetmtlockPtr());
		OldConsecutiveVictoriesBCU = pInflWarDataBCU->ConsecutiveVictories;
		pInflWarDataBCU->ConsecutiveVictories = min(pInflWarDataBCU->ConsecutiveVictories++, 8);
		ConsecutiveVictoriesBCU = pInflWarDataBCU->ConsecutiveVictories;
		mtA.auto_unlock_cancel();

		mt_auto_lock mtB(pInflWarDataANI->GetmtlockPtr());
		OldConsecutiveVictoriesANI = pInflWarDataANI->ConsecutiveVictories;
		pInflWarDataANI->ConsecutiveVictories = 0;
		ConsecutiveVictoriesANI = pInflWarDataANI->ConsecutiveVictories;
		mtB.auto_unlock_cancel();
	}

	if( TRUE == COMPARE_INFLUENCE(i_byWinnerInFlTy, INFLUENCE_TYPE_ANI) )
	{
		mt_auto_lock mtA(pInflWarDataBCU->GetmtlockPtr());
		OldConsecutiveVictoriesBCU = pInflWarDataBCU->ConsecutiveVictories;
		pInflWarDataBCU->ConsecutiveVictories = 0;
		ConsecutiveVictoriesBCU = pInflWarDataBCU->ConsecutiveVictories;
		mtA.auto_unlock_cancel();

		mt_auto_lock mtB(pInflWarDataANI->GetmtlockPtr());
		OldConsecutiveVictoriesANI = pInflWarDataANI->ConsecutiveVictories;
		pInflWarDataANI->ConsecutiveVictories = min(pInflWarDataANI->ConsecutiveVictories++, 8);
		ConsecutiveVictoriesANI = pInflWarDataANI->ConsecutiveVictories;
		mtB.auto_unlock_cancel();
	}

	// �׼� ����

	if( 0 != OldConsecutiveVictoriesBCU && 0 == ConsecutiveVictoriesBCU )
	{
		//BCU ���� �����
		m_pFieldIOCP14->DeleteBuffSkillAllCharacter(INFLUENCE_TYPE_VCN, DES_SKILLTYPE_CONSECUTIVE_VICTORIES);
	}

	if( 0 != OldConsecutiveVictoriesANI && 0 == ConsecutiveVictoriesANI )
	{
		//ANI ���� �����
		m_pFieldIOCP14->DeleteBuffSkillAllCharacter(INFLUENCE_TYPE_ANI, DES_SKILLTYPE_CONSECUTIVE_VICTORIES);
	}

	INIT_MSG_WITH_BUFFER(MSG_FC_WAR_INFLUENCE_CONSECUTIVE_VICTORITES, T_FC_WAR_INFLUENCE_CONSECUTIVE_VICTORITES, pInflVictory, pInflVictorySendBuf);
	pInflVictory->PointBCU	= ConsecutiveVictoriesBCU;
	pInflVictory->PointANI	= ConsecutiveVictoriesANI;
	m_pFieldIOCP14->SendMessageToAllClients(pInflVictorySendBuf, MSG_SIZE(MSG_FC_WAR_INFLUENCE_CONSECUTIVE_VICTORITES),INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);

	// DB ����

	QPARAM_UPDATE_INFLUENCE_CONSECUTIVE_VICTORIES *pBCUDB = new QPARAM_UPDATE_INFLUENCE_CONSECUTIVE_VICTORIES;
	pBCUDB->InfluenceType0		= INFLUENCE_TYPE_VCN;
	pBCUDB->MGameServerID0		= g_pFieldGlobal->GetMGameServerID();
	pBCUDB->ConsecutiveVictories		= ConsecutiveVictoriesBCU;
	m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateInfluenceConsecutiveVictorites, NULL, 0, pBCUDB);

	QPARAM_UPDATE_INFLUENCE_CONSECUTIVE_VICTORIES *pANIDB = new QPARAM_UPDATE_INFLUENCE_CONSECUTIVE_VICTORIES;
	pANIDB->InfluenceType0		= INFLUENCE_TYPE_ANI;
	pANIDB->MGameServerID0		= g_pFieldGlobal->GetMGameServerID();
	pANIDB->ConsecutiveVictories		= ConsecutiveVictoriesANI;
	m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateInfluenceConsecutiveVictorites, NULL, 0, pANIDB);

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] CalcConsecutiveVictoriesPoint :: WinnerInfluenceType [%d] ConsecutiveVictoriesBCU [%d] ConsecutiveVictoriesANI [%d]\r\n", 
		i_byWinnerInFlTy, ConsecutiveVictoriesBCU, ConsecutiveVictoriesANI);
#endif

	return TRUE;
}

int CInflWarManager::GetTurnAroundPoint()
{
#ifdef S_UPGRADE_INFLUENCE_POINT_HSKIM
	int CurrentContributionPointBCU = GetContributionPoint(INFLUENCE_TYPE_VCN);
	int CurrentContributionPointANI = GetContributionPoint(INFLUENCE_TYPE_ANI);

	return (CurrentContributionPointANI - CurrentContributionPointBCU);
#else
	return 0;
#endif
}

INT	CInflWarManager::GetConsecutiveVictoriesPoint(BYTE i_byWinnerInFlTy)
{
#ifdef S_UPGRADE_INFLUENCE_POINT_HSKIM
	int nPoint = 0;
	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byWinnerInFlTy);

	if( NULL == pInflWarData )
	{
		return 0;
	}

	mt_auto_lock mtA(pInflWarData->GetmtlockPtr());
	nPoint = pInflWarData->ConsecutiveVictories;
	mtA.auto_unlock_cancel();

	return nPoint;
#else
	return 0;
#endif
}
// end 2013-05-09 by hskim, ���� ����Ʈ ����
// 2013-08-01 by jhseol, ������ ���� ������
float CInflWarManager::GetPVPBuffPercent(int i_CurrentDiffContributionPoint)
{
	int nAbsDiff = abs(i_CurrentDiffContributionPoint);
	nAbsDiff -= TURN_AROUND_BUFF_SKILL_NEW_THRESHOLD;
	if ( 0 > nAbsDiff )
	{
		return 0.0f;
	}
	else
	{
		return (float)(min(nAbsDiff, TURN_AROUND_BUFF_SKILL_MAXIMUM_POINT_GAP));
	}
}
// end 2013-08-01 by jhseol, ������ ���� ������

///////////////////////////////////////////////////////////////////////////////
/// \fn			float CInflWarManager::GetHPRepairAddRateByInflTy(BYTE i_byInflTy)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-20 ~ 2006-04-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CInflWarManager::GetHPRepairAddRateByInflTy(BYTE i_byInflTy)
{
	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byInflTy);
	if(NULL == pInflWarData)
	{
		return 0.0f;
	}
// 2009-03-31 by dhjin, ���� ���� - ������ �¸� �� ���� �Ǵ� �̺�Ʈ ���� - �ذ� ���� ���� 
//	return pInflWarData->GetCurInflWarInfo()->HPRepairRate;
	return pInflWarData->fInflHPRepairRate;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			float CInflWarManager::GetDPRepairAddRateByInflTy(BYTE i_byInflTy)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-20 ~ 2006-04-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CInflWarManager::GetDPRepairAddRateByInflTy(BYTE i_byInflTy)
{
	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byInflTy);
	if(NULL == pInflWarData)
	{
		return 0.0f;
	}

// 2009-03-31 by dhjin, ���� ���� - ������ �¸� �� ���� �Ǵ� �̺�Ʈ ���� - �ذ� ���� ���� 
//	return pInflWarData->GetCurInflWarInfo()->DPRepairRate;
	return pInflWarData->fInflDPRepairRate;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			float CInflWarManager::GetSPRepairAddRateByInflTy(BYTE i_byInflTy)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-20 ~ 2006-04-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CInflWarManager::GetSPRepairAddRateByInflTy(BYTE i_byInflTy)
{
	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byInflTy);
	if(NULL == pInflWarData)
	{
		return 0.0f;
	}

// 2009-03-31 by dhjin, ���� ���� - ������ �¸� �� ���� �Ǵ� �̺�Ʈ ���� - �ذ� ���� ���� 
//	return pInflWarData->GetCurInflWarInfo()->SPRepairRate;
	return pInflWarData->fInflSPRepairRate;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::GetAllLeaderFISocket(BYTE i_byInflTy, CFieldIOCPSocket **i_ppLeaderFISoc, CFieldIOCPSocket **i_ppSubLeader1FISoc, CFieldIOCPSocket **i_ppSubLeader2FISoc)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-20 ~ 2006-04-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::GetAllLeaderFISocket(BYTE i_byInflTy, CFieldIOCPSocket **i_ppLeaderFISoc, CFieldIOCPSocket **i_ppSubLeader1FISoc, CFieldIOCPSocket **i_ppSubLeader2FISoc)
{
	*i_ppLeaderFISoc		= NULL;
	*i_ppSubLeader1FISoc	= NULL;
	*i_ppSubLeader2FISoc	= NULL;

	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byInflTy);
	if(NULL == pInflWarData)
	{
		return;
	}

	*i_ppLeaderFISoc		= m_pFieldIOCP14->GetFieldIOCPSocketByCharacterUniqueNumber(pInflWarData->InflLeaderCharacterUID);
	*i_ppSubLeader1FISoc	= m_pFieldIOCP14->GetFieldIOCPSocketByCharacterUniqueNumber(pInflWarData->InflSub1LeaderCharacterUID);
	*i_ppSubLeader2FISoc	= m_pFieldIOCP14->GetFieldIOCPSocketByCharacterUniqueNumber(pInflWarData->InflSub2LeaderCharacterUID);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::IsLeader(BYTE i_byInflTy, UID32_t i_charUID)
/// \brief		// 2007-10-06 by cmkwon, �������� 2���� ȣĪ�� �ٸ��� ���� - ���� ���� (, BOOL i_bSubLeader/*=FALSE*/)
/// \author		dhjin
/// \date		2006-12-08 ~ 2006-12-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::IsLeader(BYTE i_byInflTy, UID32_t i_charUID)
{
	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byInflTy);
	if(NULL == pInflWarData)
	{
		return FALSE;
	}

	return pInflWarData->InflLeaderCharacterUID == i_charUID;
// 2007-10-06 by cmkwon, �������� 2���� ȣĪ�� �ٸ��� ���� - ���� ���� ������ üũ
//	if(FALSE == i_bSubLeader)
//	{
//		return pInflWarData->InflLeaderCharacterUID == i_charUID;
//	}
//
//	if(i_charUID == pInflWarData->InflSub1LeaderCharacterUID
//		|| i_charUID == pInflWarData->InflSub2LeaderCharacterUID)
//	{
//		return TRUE;
//	}
//	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::IsSubLeader1(BYTE i_byInflTy, UID32_t i_charUID)
/// \brief		// 2007-10-06 by cmkwon, �������� 2���� ȣĪ�� �ٸ��� ���� -
/// \author		cmkwon
/// \date		2007-10-06 ~ 2007-10-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::IsSubLeader1(BYTE i_byInflTy, UID32_t i_charUID)
{
	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byInflTy);
	if(NULL == pInflWarData)
	{
		return FALSE;
	}

	return i_charUID == pInflWarData->InflSub1LeaderCharacterUID;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::IsSubLeader2(BYTE i_byInflTy, UID32_t i_charUID)
/// \brief		// 2007-10-06 by cmkwon, �������� 2���� ȣĪ�� �ٸ��� ���� -
/// \author		cmkwon
/// \date		2007-10-06 ~ 2007-10-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::IsSubLeader2(BYTE i_byInflTy, UID32_t i_charUID)
{
	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byInflTy);
	if(NULL == pInflWarData)
	{
		return FALSE;
	}

	return i_charUID == pInflWarData->InflSub2LeaderCharacterUID;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::SetLeader8SubLeaderRACE(CHARACTER *io_pCharacter)
/// \brief		// 2007-10-06 by cmkwon, �������� 2���� ȣĪ�� �ٸ��� ���� -
/// \author		cmkwon
/// \date		2007-10-06 ~ 2007-10-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::SetLeader8SubLeaderRACE(CHARACTER *io_pCharacter)
{
	if(INFLUENCE_TYPE_NORMAL == io_pCharacter->InfluenceType)
	{// 2007-10-06 by cmkwon, �Ϲ� ������ üũ �ʿ� ����
		return FALSE;
	}

	if(this->IsLeader(io_pCharacter->InfluenceType, io_pCharacter->CharacterUniqueNumber))
	{
		io_pCharacter->Race	|= RACE_INFLUENCE_LEADER;
		return TRUE;
	}

	if(this->IsSubLeader1(io_pCharacter->InfluenceType, io_pCharacter->CharacterUniqueNumber))
	{
		io_pCharacter->Race	|= RACE_INFLUENCE_SUBLEADER_1;
		return TRUE;
	}

	if(this->IsSubLeader2(io_pCharacter->InfluenceType, io_pCharacter->CharacterUniqueNumber))
	{
		io_pCharacter->Race	|= RACE_INFLUENCE_SUBLEADER_2;
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SetSummonJacoMonsterW(BYTE i_byInflTy, BOOL i_bSummon)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-20 ~ 2006-04-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SetSummonJacoMonsterW(BYTE i_byInflTy, BOOL i_bSummon)
{
	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byInflTy);
	if(NULL == pInflWarData)
	{
		return;
	}
	pInflWarData->SetSummonJacoMonster(i_bSummon);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::GetSummonInflBossMonsterData(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA *o_pSummonData, BYTE i_byInflTy)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-20 ~ 2006-04-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::GetSummonInflBossMonsterData(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA *o_pSummonData, BYTE i_byInflTy)
{
	if(FALSE == COMPARE_INFLUENCE(i_byInflTy, INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI))
	{
		return FALSE;
	}

	if(m_mtvectSummonMonsterDataList.empty())
	{
		return FALSE;
	}

	int				nMonsterBell = GET_SAME_MONSTER_BELL_BY_CHARACTER_INFLTYPE(i_byInflTy);
	ATUM_DATE_TIME	atumCur(true);
	mt_auto_lock mtA(&m_mtvectSummonMonsterDataList);

	mtvectSummonMonsterData::iterator itr(m_mtvectSummonMonsterDataList.begin());
	while(itr != m_mtvectSummonMonsterDataList.end())
	{
		SINFLBOSS_MONSTER_SUMMON_DATA *pSummonData = &*itr;
		if(nMonsterBell == pSummonData->pMonsterInfo->Belligerence)
		{
			o_pSummonData->SummonMonsterUnitkind	= pSummonData->pMonsterInfo->MonsterUnitKind;
			o_pSummonData->RemainMinute				= pSummonData->SummonTerm - atumCur.GetTimeDiffTimeInMinutes(pSummonData->atimeInsertedTime);
			o_pSummonData->BossStep					= pSummonData->BossStep;			// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - �� �ܰ�
			o_pSummonData->BeforeWinCheck			= pSummonData->BeforeWinCheck;				// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - �� �ܰ� ���� ����
			return TRUE;
		}
		itr++;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::GetVCNSummonedInflBossMonsterData(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA *o_pSummonData)
/// \brief		
/// \author		dhjin
/// \date		2007-02-06 ~ 2007-02-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::GetVCNSummonedInflBossMonsterData(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA *o_pSummonData)
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-02-06 by dhjin, ������ ��ȯ�ǰ� �� �� �����ϴ� �������� ������ �����ֱ� ���� �߰�
	if (TRUE == m_SummonVCNBossMonsterInfo.bSummonBossMonster)
	{
		o_pSummonData->SummonMonsterUnitkind	= m_SummonVCNBossMonsterInfo.SummonMonsterUnitkind;
		o_pSummonData->RemainMinute				= 0;
		o_pSummonData->SummonMonsterTime		= m_SummonVCNBossMonsterInfo.SummonBossTime;
		o_pSummonData->ContributionPoint		= m_SummonVCNBossMonsterInfo.ContributionPoint;			// 2008-04-22 by dhjin, ����, ������ ����â ��ȹ�� - ��������Ʈ
		o_pSummonData->BossStep					= m_SummonVCNBossMonsterInfo.BossStep;				// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - �� �ܰ�
		o_pSummonData->BeforeWinCheck			= m_SummonVCNBossMonsterInfo.BeforeWinCheck;				// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - �� �ܰ� ���� ����
		return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::GetANISummonedInflBossMonsterData(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA *o_pSummonData)
/// \brief		
/// \author		dhjin
/// \date		2007-02-06 ~ 2007-02-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::GetANISummonedInflBossMonsterData(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA *o_pSummonData)
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-02-06 by dhjin, ������ ��ȯ�ǰ� �� �� �����ϴ� �������� ������ �����ֱ� ���� �߰�
	if (TRUE == m_SummonANIBossMonsterInfo.bSummonBossMonster)
	{
		o_pSummonData->SummonMonsterUnitkind	= m_SummonANIBossMonsterInfo.SummonMonsterUnitkind;
		o_pSummonData->RemainMinute				= 0;
		o_pSummonData->SummonMonsterTime		= m_SummonANIBossMonsterInfo.SummonBossTime;
		o_pSummonData->ContributionPoint		= m_SummonANIBossMonsterInfo.ContributionPoint;		// 2008-04-22 by dhjin, ����, ������ ����â ��ȹ�� - ��������Ʈ
		o_pSummonData->BossStep					= m_SummonANIBossMonsterInfo.BossStep;				// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - �� �ܰ�
		o_pSummonData->BeforeWinCheck			= m_SummonANIBossMonsterInfo.BeforeWinCheck;				// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - �� �ܰ� ���� ����
		return TRUE;
	}
	return FALSE;
}

// 2007-05-17 by cmkwon, �Լ� ����
/////////////////////////////////////////////////////////////////////////////////
///// \fn			void CInflWarManager::OnInfluenceBossMonsterAutoDestroyedInflWarManager(BOOL i_bVCNInflTyForInit)
///// \brief		
///// \author		dhjin
///// \date		2007-02-06 ~ 2007-02-06
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//void CInflWarManager::OnInfluenceBossMonsterAutoDestroyedInflWarManager(BOOL i_bVCNInflTyForInit)
//{
//	if(TRUE == i_bVCNInflTyForInit)
//	{
//		util::zero(&m_SummonVCNBossMonsterInfo, sizeof(SSUMMONBOSSMONSTER_INFO));
//	}
//	else
//	{
//		util::zero(&m_SummonANIBossMonsterInfo, sizeof(SSUMMONBOSSMONSTER_INFO));
//	}
//}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::SetSubLeader(BYTE i_byInflTy, UID32_t i_CharacterUID, CHAR * i_pCharacterName, BYTE SubLeaderNum)
/// \brief		
/// \author		dhjin
/// \date		2007-02-14 ~ 2007-02-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::SetSubLeader(BYTE i_byInflTy, UID32_t i_CharacterUID, CHAR * i_pCharacterName, BYTE SubLeaderNum)
{
	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byInflTy);
	if(NULL == pInflWarData)
	{
		return FALSE;
	}

	mt_auto_lock mtA(pInflWarData->GetmtlockPtr());
	if(1 == SubLeaderNum)
	{
		pInflWarData->InflSub1LeaderCharacterUID = i_CharacterUID;
		util::strncpy(pInflWarData->InflSubLeader1CharacterName, i_pCharacterName, SIZE_MAX_CHARACTER_NAME);	// 2008-01-03 by dhjin, ���� ������ �������� �Ⱥ��̴� ���׷� ����
	}
	else if(2 == SubLeaderNum)
	{
		pInflWarData->InflSub2LeaderCharacterUID = i_CharacterUID;
		util::strncpy(pInflWarData->InflSubLeader2CharacterName, i_pCharacterName, SIZE_MAX_CHARACTER_NAME);	// 2008-01-03 by dhjin, ���� ������ �������� �Ⱥ��̴� ���׷� ����
	}
	else 
	{
		return FALSE;
	}

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::InsertSummonStrategyPointInfo(SSUMMON_STRATEGYPOINT_INFO *i_pSSUMMON_STRATEGYPOINT_INFO)
/// \brief		
/// \author		dhjin
/// \date		2007-02-27 ~ 2007-02-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::InsertSummonStrategyPointInfo(SSUMMON_STRATEGYPOINT_INFO *i_pSSUMMON_STRATEGYPOINT_INFO)
{
	m_mtvectSummonStrategyPointInfo.pushBackLock(*i_pSSUMMON_STRATEGYPOINT_INFO);
	if (!CheckSummonBoss())
	{
		m_StrategicPointInfoDisplay.SetStrategicPointDisplay(i_pSSUMMON_STRATEGYPOINT_INFO);
		SendStrategicPointInfoForAllUser(i_pSSUMMON_STRATEGYPOINT_INFO->MapIndex);
	}
	SetDoingInfluenceWar();	// 2012-11-26 by jhseol, ���� �ý��� ������ - ����� Ʈ���� ��������(�Ͻ�����), ������ ���������� Ȯ���ϴ� �Լ�.(������ üũ�� ����)
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::DeleteSummonStrategyPointInfo(MAP_CHANNEL_INDEX	i_MapChannel)
/// \brief		
/// \author		dhjin
/// \date		2007-02-27 ~ 2007-02-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::DeleteSummonStrategyPointInfo(MAP_CHANNEL_INDEX	i_MapChannel)
{
	mt_auto_lock mtA(&m_mtvectSummonStrategyPointInfo);

	mtvectorSSUMMON_STRATEGYPOINT_INFO::iterator	itr = m_mtvectSummonStrategyPointInfo.begin();
	while(itr != m_mtvectSummonStrategyPointInfo.end())
	{
		if(i_MapChannel.MapIndex == itr->MapIndex)
		{
			m_mtvectSummonStrategyPointInfo.erase(itr);
			SetDoingInfluenceWar();	// 2012-11-26 by jhseol, ���� �ý��� ������ - ����� Ʈ���� ��������(�Ͻ�����), ���� ���Ḧ �ǽð����� Ȯ�� �ϱ� ���� �Լ�ȣ��(������ ����)
			break;
		}
		itr++;
	}

	m_StrategicPointInfoDisplay.RemoveStrategicPointDisplay(i_MapChannel.MapIndex);
	SendStrategicPointInfoForAllUser(i_MapChannel.MapIndex);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::DeleteSummonStrategyPointInfoByInfluenceType(BYTE i_InfluenceType)
/// \brief		
/// \author		dhjin
/// \date		2007-02-27 ~ 2007-02-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::DeleteSummonStrategyPointInfoByInfluenceType(BYTE i_InfluenceType)
{
	mt_auto_lock mtA(&m_mtvectSummonStrategyPointInfo);
	
	mtvectorSSUMMON_STRATEGYPOINT_INFO::iterator	itr = m_mtvectSummonStrategyPointInfo.begin();
	while(itr != m_mtvectSummonStrategyPointInfo.end())
	{
		if(i_InfluenceType == itr->InfluenceType)
		{
			m_mtvectSummonStrategyPointInfo.erase(itr);
			SetDoingInfluenceWar();	// 2012-11-26 by jhseol, ���� �ý��� ������ - ����� Ʈ���� ��������(�Ͻ�����), ���� ���Ḧ �ǽð����� Ȯ�� �ϱ� ���� �Լ�ȣ��(������ ����)
			continue;
		}
		itr++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckSummonStrategyPointInfoByMapIndex(INT i_nMapIndex)
/// \brief		���� ��������Ʈ�� �����ִ��� üũ
/// \author		dhjin
/// \date		2007-10-06 ~ 2007-10-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckSummonStrategyPointInfoByMapIndex(INT i_nMapIndex)
{
	mt_auto_lock mtA(&m_mtvectSummonStrategyPointInfo);
	
	mtvectorSSUMMON_STRATEGYPOINT_INFO::iterator	itr = m_mtvectSummonStrategyPointInfo.begin();
	while(itr != m_mtvectSummonStrategyPointInfo.end())
	{
		if(i_nMapIndex == itr->MapIndex)
		{
			return TRUE;			
		}
		
		CFieldMapProject *pCurFMPro = m_pFieldIOCP14->GetFieldMapProjectByMapIndex(itr->MapIndex);
		if(pCurFMPro)
		{
			// 2008-06-20 by cmkwon, EP3 ��� �ý��� ����(�ʴ� ����� 2�� ���� �� �� �ִ�) - �Ʒ��� ���� ����
			//if(i_nMapIndex == pCurFMPro->m_nBeforeMapIndex)
			if(i_nMapIndex == pCurFMPro->m_nBeforeMapIndex
				|| i_nMapIndex == pCurFMPro->m_nBeforeMapIndex2)	// 2008-06-20 by cmkwon, EP3 ��� �ý��� ����(�ʴ� ����� 2�� ���� �� �� �ִ�) - 
			{
				return TRUE;
			}
		}
		itr++;
	}

	return FALSE;
}

void CInflWarManager::UpdateSummonStrategicPointInfoHP(MapIndex_t i_nMapindex, FLOAT i_nHPRate)
{
	m_StrategicPointInfoDisplay.SetStrategicPointDisplayHP(i_nMapindex, i_nHPRate);
	SendStrategicPointInfoForAllUser(i_nMapindex);
}

void CInflWarManager::GetAllSummonStrategicPointInfo(vector<SSTRATEGYPOINT_DISPLAY_INFO>* o_VectStrategicPointDisplay)
{
	m_StrategicPointInfoDisplay.GetAllStrategicPointDisplay(o_VectStrategicPointDisplay);
}

mtvectSSTRATEGYPOINT_SUMMONTIME_INFO * CInflWarManager::GetStrategyPointSummonTimeInfo(void)	
{
	return &m_mtvectStrategyPointSummonTimeInfo;
}

SSTRATEGYPOINT_SUMMON_RANGE * CInflWarManager::GetStrategyPointSummonRange(void)
{
	return &m_StrategyPointSummonRange;
}

mtvectSSTRATEGYPOINT_NOT_SUMMON_TIME * CInflWarManager::GetStrategyPointNotSummonTime(void)
{
	return &m_mtvectStrategyPointNotSummonTime;
}

void CInflWarManager::InitStrategyPointSummonTimeInfo(vectSSTRATEGYPOINT_SUMMONTIME_INFO *i_pvectSStrategypointSummonTimeInfo)
{
	mt_auto_lock mta(this->GetStrategyPointSummonTimeInfo());

	m_mtvectStrategyPointSummonTimeInfo.clear();

	m_mtvectStrategyPointSummonTimeInfo.assign(i_pvectSStrategypointSummonTimeInfo->begin(), i_pvectSStrategypointSummonTimeInfo->end());
}

//////////////////////////////////////////////////////////////////////////
// 2012-11-13 by jhseol, ���� �ý��� ������ - ������
void CInflWarManager::InitRenewalStrategyPointSummonTime(vectSRENEWAL_STRATEGYPOINT_SUMMON_TIME *i_pvectRenewalSStrategypointSummonTime)
{
	mt_auto_lock mta(this->GetRenewalStrategyPointSummonTime());
	
	m_mtvectRenewalStrategyPointSummonTime.clear();
	
	m_mtvectRenewalStrategyPointSummonTime.assign(i_pvectRenewalSStrategypointSummonTime->begin(), i_pvectRenewalSStrategypointSummonTime->end());
}

mtvectSRENEWAL_STRATEGYPOINT_SUMMON_TIME * CInflWarManager::GetRenewalStrategyPointSummonTime(void)	
{
	return &m_mtvectRenewalStrategyPointSummonTime;
}

void CInflWarManager::InitRenewalStrategyPointSummonMapInfo(vectSSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO *i_pvectStrategyPointMapInfluenceInfo)
{
	mt_auto_lock mta(this->GetRenewalStrategyPointSummonMapInfo());
	
	m_mtvectStrategyPointMapInfluenceInfo.clear();
	
	m_mtvectStrategyPointMapInfluenceInfo.assign(i_pvectStrategyPointMapInfluenceInfo->begin(), i_pvectStrategyPointMapInfluenceInfo->end());
}

mtvectSSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO * CInflWarManager::GetRenewalStrategyPointSummonMapInfo()
{
	return &m_mtvectStrategyPointMapInfluenceInfo;
}
// end 2012-11-13 by jhseol, ���� �ý��� ������ - ������

// 2012-11-26 by jhseol, ���� �ý��� ������ - ����� Ʈ���� ��������(�Ͻ�����), ������ ���������� Ȯ���ϴ� �Լ�
void CInflWarManager::SetDoingInfluenceWar()
{
	if (S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL)
	{
		BOOL IsDoingStrategyPointWar	= FALSE;
		BOOL IsDoingOutPostWar			= FALSE;
		BOOL IsDoingBossMonsterWar		= FALSE;

		if( !m_mtvectSummonStrategyPointInfo.empty() )	// ������ ��ȯ�� ���� �ִٸ�
		{
			IsDoingStrategyPointWar = TRUE;
		}
		
		if ( NULL != m_pFieldIOCP14 )
		{
			if(	TRUE == m_pFieldIOCP14->m_OutPostManager.CheckALLOutPostWaring() )	// ���������� ���� Ȯ��
			{
				IsDoingOutPostWar = TRUE;
			}
		}
			
		if (TRUE == m_SummonVCNBossMonsterInfo.bSummonBossMonster || TRUE == m_SummonANIBossMonsterInfo.bSummonBossMonster )	// ���� Ȯ��
		{
			IsDoingBossMonsterWar = TRUE;
		}

		if( IsDoingStrategyPointWar || IsDoingOutPostWar || IsDoingBossMonsterWar )
		{
			if (!m_bIsDoingInfluenceWarCheck)
				OnInfluenceWarStart();

			m_bIsDoingInfluenceWarCheck = TRUE;
		}
		else
		{
			if (m_bIsDoingInfluenceWarCheck)
				
				OnInfluenceWarEnd();

			m_bIsDoingInfluenceWarCheck = FALSE;
		}

		server::log(false, "  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL - CInflWarManager::SetDoingInfluenceWar, IsDoingStrategyPointWar(%d), IsDoingOutPostWar(%d), IsDoingBossMonsterWar(%d) = m_bIsDoingInfluenceWarCheck(%d)\r\n"
			, IsDoingStrategyPointWar, IsDoingOutPostWar, IsDoingBossMonsterWar, m_bIsDoingInfluenceWarCheck);

		// end 2013-03-25 by jhseol, ������� Ʈ���� �̿� �Ұ��� ������� �߰��α�
	}
}

#ifdef MS_TAKE_NO_DAMAGE 
BOOL CInflWarManager::IsDoingStrategyPointWar()
{
	if (TRUE != m_mtvectSummonStrategyPointInfo.empty())	// ������ ��ȯ�� ���� �ִٸ�
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
#endif

BOOL CInflWarManager::IsDoingInfluenceWar()
{
	return m_bIsDoingInfluenceWarCheck;
}
// end 2012-11-26 by jhseol, ���� �ý��� ������ - ����� Ʈ���� ��������(�Ͻ�����), ������ ���������� Ȯ���ϴ� �Լ�
void CInflWarManager::InitStrategyPointSummonRange(SSTRATEGYPOINT_SUMMON_RANGE *i_pStrategyPointSummonRange)
{
	EnterCriticalSection(&m_criticallStrategyPointSummonRange);
	util::zero(&m_StrategyPointSummonRange, sizeof(SSTRATEGYPOINT_SUMMON_RANGE));

	m_StrategyPointSummonRange = *i_pStrategyPointSummonRange;
	LeaveCriticalSection(&m_criticallStrategyPointSummonRange);
}
#ifdef MS_TAKE_NO_DAMAGE
UINT CInflWarManager::BlockDameMS()
{
	UINT _return = 0;
		if (CInflWarManager::MSwar_M == 0)
		{
			CInflWarManager::MSwar_M = CInflWarManager::SYS_M;
			CInflWarManager::MSwar_H = CInflWarManager::SYS_H;
		}
		if (CInflWarManager::m_calculation(CInflWarManager::SYS_H, CInflWarManager::SYS_M, CInflWarManager::MSwar_H, CInflWarManager::MSwar_M)< CInflWarManager::MS_SAFE_TIME)
		{
			_return = CInflWarManager::MS_SAFE_TIME-CInflWarManager::m_calculation(CInflWarManager::SYS_H, CInflWarManager::SYS_M, CInflWarManager::MSwar_H, CInflWarManager::MSwar_M);
		}
		else
		{
			_return = 0;
		}
	return _return;
}
#endif
void CInflWarManager::InitStrategyPointNotSummonTime(vectSSTRATEGYPOINT_NOT_SUMMON_TIME *i_pvectSStrategyPointNotSummonTime)
{
	mt_auto_lock mta(this->GetStrategyPointNotSummonTime());

	m_mtvectStrategyPointNotSummonTime.clear();

	m_mtvectStrategyPointNotSummonTime.assign(i_pvectSStrategyPointNotSummonTime->begin(), i_pvectSStrategyPointNotSummonTime->end());
}

void CInflWarManager::InitStrategyPoint()
{
	this->LoadStrategyPointSummonInfo();
	
}
#ifdef MS_TAKE_NO_DAMAGE
UINT CInflWarManager::m_calculation(int new_h, int new_m, int old_h, int old_m)
{
	int minutes = 0;
	if (new_h == old_h)
	{
		minutes = new_m - old_m;
	}
	else
	{
		minutes = (60 - old_m) + new_m;
	}
	return minutes;
}
#endif
void CInflWarManager::LoadStrategyPointSummonInfo()
{
	//////////////////////////////////////////////////////////////////////////
	// 2012-11-13 by jhseol, ���� �ý��� ������ - ������ ���� �ε�(���Ϻ� ���� ���� �� ���� ���� ����)
	if (S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL)
	{
		m_atBeforeCheckTime.SetCurrentDateTime();	// �ε��� �ð� ����
		this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_LoadRenewalStrategyPointSummonInfo, NULL, 0, NULL);
	}
	else this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_LoadStrategyPointSummonInfo, NULL, 0, NULL);
}

void CInflWarManager::LoadStrategyPointSummonRange()
{
	this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_LoadStrategyPointSummonRange, NULL, 0, NULL);
}

void CInflWarManager::LoadStrategyPointNotSummonTime()
{
	this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_LoadStrategyPointNotSummonTime, NULL, 0, NULL);
}

BOOL CInflWarManager::CheckStrategyPointSummon()
{
	EnterCriticalSection(&m_criticallStrategyPointSummonRange);
	if(TRUE == m_StrategyPointSummonRange.StrategyPointSummonCheck)
	{
		LeaveCriticalSection(&m_criticallStrategyPointSummonRange);
		return TRUE;
	}
	LeaveCriticalSection(&m_criticallStrategyPointSummonRange);

	return FALSE;
}

BOOL CInflWarManager::CompareStrategyPointSummonTime()
{
	EnterCriticalSection(&m_criticallStrategyPointSummonRange);
	if(0 < m_StrategyPointSummonRange.EndTime.GetTimeDiffToCurrentTimeInSeconds())
	{// 2007-03-02 by dhjin, ���� �ֱⰡ ������ �ʾҴ�.
		LeaveCriticalSection(&m_criticallStrategyPointSummonRange);
		return TRUE;
	}
	LeaveCriticalSection(&m_criticallStrategyPointSummonRange);
	
	return FALSE;
}

void CInflWarManager::SetAllStrategyPointSummonTime()
{
	mt_auto_lock mta(this->GetStrategyPointSummonTimeInfo());
	
	if(FALSE == m_StrategyPointSummonRange.StrategyPointSummonCheck)
	{// 2008-08-20 by dhjin, ���� �����ֱⰪ�� �ʱ�ȭ�� �ȵǾ��ִٸ� �����Ѵ�.
		return;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-03-12 by dhjin, 
	// mt4HourStartTime : �� �ֱ��� StartTime���� 4�ð��� ���Ѵ�. �ֱ� ������ ������ �յڷ� �ð����� �ξ� �ߺ������� ���Ѵ�.
	// mt4HourEndTime   : �� �ֱ��� EndTime���� 4�ð��� ���ش�.
	// mtCurTime		: ���� �ð��� ���ϰ� ���� �ð��� ���� �ð� ���� ���̸� ���� �ð��� ���� �ð����� �����Ѵ�.
	ATUM_DATE_TIME mt4HourStartTime;
	mt4HourStartTime = m_StrategyPointSummonRange.StartTime;
	mt4HourStartTime.AddDateTime(0,0,0,4,0,0);
	ATUM_DATE_TIME mt4HourEndTime;		
	mt4HourEndTime = m_StrategyPointSummonRange.EndTime;
	mt4HourEndTime.AddDateTime(0,0,0,-4,0,0);
	ATUM_DATE_TIME mtCurTime;			
	mtCurTime.SetCurrentDateTime();
	mtCurTime.AddDateTime(0,0,0,0,10,0);
	if(0 < mt4HourStartTime.GetTimeDiffTimeInSeconds(mtCurTime))
	{// 2007-03-05 by dhjin, ���� �ð��� ���� �ð� ���� ���̸� ���� �ð��� ���� �ð����� �����Ѵ�.
		mtCurTime = mt4HourStartTime;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-03-12 by dhjin, 
	// Range		: ���� �ð��� �ֱ� ������ �ð��� ���̳��� ���� ���Ͽ� ���� ���� ����(��¥)�� ����Ѵ�.
	// TimeRange	: ���� �ð��� �ֱ� ������ �ð��� ���̳��� ���� ���Ͽ� ���� ���� ����(��)�� ����Ѵ�.
	int Range = mt4HourEndTime.GetTimeDiffTimeInMinutes(mtCurTime);	
	int TimeRange = 0;												

	// 2009-10-26 by cmkwon, ���� �ε��� �α� �߰� - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CInflWarManager::SetAllStrategyPointSummonTime# 11000 \r\n");

	// 2007-03-12 by dhjin, ���Ϻ� ��ȯ �� �� �ִ� �ִ� ���� �����ϱ� ���� �ʱ�ȭ�� �Ѵ�.
	this->InitStrategyPointDay(&mtCurTime, &mt4HourEndTime);

	// 2009-10-26 by cmkwon, ���� �ε��� �α� �߰� - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CInflWarManager::SetAllStrategyPointSummonTime# 12000 \r\n");

	mtvectSSTRATEGYPOINT_SUMMONTIME_INFO::iterator itr = m_mtvectStrategyPointSummonTimeInfo.begin();
	for(; itr != m_mtvectStrategyPointSummonTimeInfo.end(); itr++)
	{
		if(FALSE == this->CompareStrategyPointDay()
			|| 0 >= Range)
		{// 2007-03-07 by dhjin, �� �̻� �Ҵ��� ������ ����. ���� �ֱ�� �ѱ��.
			itr->SummonTime.AddDateTime(1,0,0, 0,0,0);
			QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME *pQparam = new QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME;
			pQparam->MapIndex		= itr->MapIndex;
			pQparam->SummonTime		= itr->SummonTime;
			pQparam->SummonAttribute	= FALSE;		// 2008-04-08 by dhjin, ��ȯ ���� �ð� ���� - 
			this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateStrategyPointSummonInfoBySummonTime, NULL, 0, pQparam);
			continue;
		}
	
		// 2007-03-07 by dhjin, ���Ͽ� �Ҵ��Ѵ�.
		if(0 >= itr->SummonCount)
		{
			continue;
		}
		
		// 2007-03-12 by dhjin, itr->SummonCount = 0 �� �� �̻� ��ȯ �� �� �����Ƿ� �������� �����Ѵ�.
		// 2007-03-12 by dhjin, ��¥�� �����ϰ� �����Ѵ�.
		itr->SummonTime = mtCurTime;
		itr->SummonTime.AddDateTime(0,0,0, 0,(RANDI(1, Range)),0);
		ATUM_DATE_TIME tmMakeTime;
		tmMakeTime.SetDateTime(itr->SummonTime.Year, itr->SummonTime.Month, itr->SummonTime.Day, 0, 0, 0);

		//////////////////////////////////////////////////////////////////////////
		// 2007-03-12 by dhjin
		// tmCheckRangeTime : ���� �� ��¥�� ��� �ֱ��� ������ üũ�Ͽ� �� ��¥�� �����ϰ� ���� ������ ������ �ʸ� �����ϱ� ���� ����.
		// tmIncreaseDay	: ���� ���� ���̱� ���� ���� �� �������� ������ �����ϸ� ���ڸ� �ϳ��� ���Ͽ� ��ȯ ������ ���ڿ� �����Ѵ�.
		int				tmCheckRangeTime = 1;		
		int				tmIncreaseDay = 0;
		
		// 2009-10-26 by cmkwon, ���� �ε��� �α� �߰� - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CInflWarManager::SetAllStrategyPointSummonTime# 12100 tmMakeTime(%s) tmCheckRangeTime(%d) itrSummTime(%s) \r\n"
			, tmMakeTime.GetDateTimeString().GetBuffer(), tmCheckRangeTime, itr->SummonTime.GetDateTimeString().GetBuffer());

		while(FALSE == this->CompareStrategyPointDayByOneDay(&tmMakeTime, &tmCheckRangeTime))
		{// 2007-03-09 by dhjin, ��ȯ ������ ���ڰ� �ִ��� üũ�Ͽ� �� ���ڸ� �켱 �����Ѵ�.
			itr->SummonTime = mtCurTime;
			itr->SummonTime.AddDateTime(0,0,tmIncreaseDay, 0,0,0);
			tmIncreaseDay++;
			tmMakeTime.SetDateTime(itr->SummonTime.Year, itr->SummonTime.Month, itr->SummonTime.Day, 0, 0, 0);
		}

		// 2009-10-26 by cmkwon, ���� �ε��� �α� �߰� - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CInflWarManager::SetAllStrategyPointSummonTime# 12200 tmMakeTime(%s) tmCheckRangeTime(%d) itrSummTime(%s) \r\n"
			, tmMakeTime.GetDateTimeString().GetBuffer(), tmCheckRangeTime, itr->SummonTime.GetDateTimeString().GetBuffer());

		// 2007-03-12 by dhjin, ������ ��¥�� ��ȯ �� �� �ִ� count���� 1 ���� ��Ų��.
		this->IncreaseStrategyPointDay(&(itr->SummonTime));


		//////////////////////////////////////////////////////////////////////////
		// ���� ���� ���� ���� ���̰ų� �����̸� �����ϰ� ������ �ð� Range���� �����Ѵ�.
		if(1 == m_mtvectStrategyPointDay.size())
		{
			Range = mt4HourEndTime.GetTimeDiffTimeInMinutes(mtCurTime);	// 2008-04-04 by dhjin, ��ȯ ���� �ð� ���� - ���� ����, ù�� ��ȯ �ȵɶ� �������� ������ �Ǿ� ���������� ���̸� �ٽ� ����Ѵ�.	
			TimeRange = Range;
			tmMakeTime = mtCurTime;
		}
		else if(1 == tmCheckRangeTime)
		{
			ATUM_DATE_TIME	tmPlusOneDay;
			tmPlusOneDay.SetDateTime(mtCurTime.Year, mtCurTime.Month, mtCurTime.Day+1,0,0,0);
			TimeRange = tmPlusOneDay.GetTimeDiffTimeInMinutes(mtCurTime);
			tmMakeTime = mtCurTime;
		}
		else if(m_mtvectStrategyPointDay.size() == tmCheckRangeTime)
		{
			TimeRange = mt4HourEndTime.GetTimeDiffTimeInMinutes(tmMakeTime);
		}
		else
		{
			TimeRange = 24 * 60;
		}

		BOOL SummonAttribute = FALSE;		// 2008-04-08 by dhjin, ��ȯ ���� �ð� ���� - 


		// 2009-10-26 by cmkwon, ���� �ε��� �α� �߰� - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CInflWarManager::SetAllStrategyPointSummonTime# 12300 tmMakeTime(%s) TimeRange(%d) itrSummTime(%s) \r\n"
			, tmMakeTime.GetDateTimeString().GetBuffer(), TimeRange, itr->SummonTime.GetDateTimeString().GetBuffer());

		// 2007-03-12 by dhjin, �ð��� ���Ϻ� ���� �ð��� ���� �����ϰ� �����Ѵ�.
		if(FALSE == this->CheckStrategyPointNotSummonTime(&tmMakeTime, TimeRange, &SummonAttribute))
		{
			char szErr[1024];
			wsprintf(szErr, "CInflWarManager::Not_SummonTime Error");
			g_pFieldGlobal->WriteSystemLog(szErr);
			DbgOut("%s\rn", szErr);
			// 2007-03-29 by dhjin, �Ҵ����� ���ߴٸ� ���� �ֱ⿡ ��ȯ�Ѵ�.
			itr->SummonTime.AddDateTime(1,0,0, 0,0,0);
			QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME *pQparam = new QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME;
			pQparam->MapIndex		= itr->MapIndex;
			pQparam->SummonTime		= itr->SummonTime;
			pQparam->SummonAttribute	= FALSE;		// 2008-04-08 by dhjin, ��ȯ ���� �ð� ���� - 
			this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateStrategyPointSummonInfoBySummonTime, NULL, 0, pQparam);
			continue;
		}
		
		// 2009-10-26 by cmkwon, ���� �ε��� �α� �߰� - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CInflWarManager::SetAllStrategyPointSummonTime# 12400 tmMakeTime(%s) TimeRange(%d) itrSummTime(%s) \r\n"
			, tmMakeTime.GetDateTimeString().GetBuffer(), TimeRange, itr->SummonTime.GetDateTimeString().GetBuffer());

		// 2007-03-12 by dhjin, ������ ��ȯ�ð� ������ ����ü�� �Ҵ��Ѵ�.
		itr->SummonTime = tmMakeTime;
		// 2007-03-12 by dhjin, ������ ��ȯ�ð� ������ DB�� ������Ʈ �Ѵ�.
		QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME *pQparam = new QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME;
		pQparam->MapIndex		= itr->MapIndex;
		pQparam->SummonTime		= tmMakeTime;
		pQparam->SummonAttribute	= SummonAttribute;		// 2008-04-08 by dhjin, ��ȯ ���� �ð� ���� - 
		this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateStrategyPointSummonInfoBySummonTime, NULL, 0, pQparam);
	}

	// 2009-10-26 by cmkwon, ���� �ε��� �α� �߰� - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CInflWarManager::SetAllStrategyPointSummonTime# 13000 \r\n");

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SetAllStrategyPointSummonTimeNew()
/// \brief		��������Ʈ �ð� ���� ���� - ��������Ʈ ��ȯ �ð� ���� ������Ʈ 
/// \author		dhjin
/// \date		2009-03-23 ~ 2009-03-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SetAllStrategyPointSummonTimeNew()
{
	mt_auto_lock mta(this->GetStrategyPointSummonTimeInfo());

	// 2009-07-02 by cmkwon, ���� ����� ���� ���� �α� �߰� - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CInflWarManager::SetAllStrategyPointSummonTimeNew# SummonFlag(%d) SummonRange(%d) MaxSummonCntPerDaily(%d) Period(%s~%s) NewSummonRange(%d) NewMaxSummonCntPerDaily(%d) SummonTimeInfoListCnt(%d) \r\n"
		, m_StrategyPointSummonRange.StrategyPointSummonCheck, m_StrategyPointSummonRange.SummonRange, m_StrategyPointSummonRange.MaxSummonCountDaily
		, m_StrategyPointSummonRange.StartTime.GetDateTimeString().GetBuffer(), m_StrategyPointSummonRange.EndTime.GetDateTimeString().GetBuffer()
		, m_StrategyPointSummonRange.NewSummonRange, m_StrategyPointSummonRange.NewMaxSummonCountDaily, m_mtvectStrategyPointSummonTimeInfo.size());

	if(FALSE == m_StrategyPointSummonRange.StrategyPointSummonCheck
		|| 0 >= m_mtvectStrategyPointSummonTimeInfo.size()
		)
	{// 2008-08-20 by dhjin, ���� �����ֱⰪ�� �ʱ�ȭ�� �ȵǾ��ִٸ� �����Ѵ�.
		return;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-03-12 by dhjin, 
	// mt4HourStartTime : �� �ֱ��� StartTime���� 4�ð��� ���Ѵ�. �ֱ� ������ ������ �յڷ� �ð����� �ξ� �ߺ������� ���Ѵ�.
	// mt4HourEndTime   : �� �ֱ��� EndTime���� 4�ð��� ���ش�.
	// mtCurTime		: ���� �ð��� ���ϰ� ���� �ð��� ���� �ð� ���� ���̸� ���� �ð��� ���� �ð����� �����Ѵ�.
	ATUM_DATE_TIME mt4HourStartTime;
	mt4HourStartTime = m_StrategyPointSummonRange.StartTime;
	mt4HourStartTime.AddDateTime(0,0,0,4,0,0);
	ATUM_DATE_TIME mt4HourEndTime;		
	mt4HourEndTime = m_StrategyPointSummonRange.EndTime;
	mt4HourEndTime.AddDateTime(0,0,0,-4,0,0);
	ATUM_DATE_TIME mtCurTime;			
	mtCurTime.SetCurrentDateTime();
	mtCurTime.AddDateTime(0,0,0,0,10,0);
	if(0 < mt4HourStartTime.GetTimeDiffTimeInSeconds(mtCurTime))
	{// 2007-03-05 by dhjin, ���� �ð��� ���� �ð� ���� ���̸� ���� �ð��� ���� �ð����� �����Ѵ�.
		mtCurTime = mt4HourStartTime;
	}	

	//////////////////////////////////////////////////////////////////////////
	// 2007-03-12 by dhjin, 
	// Range		: ���� �ð��� �ֱ� ������ �ð��� ���̳��� ���� ���Ͽ� ���� ���� ����(��¥)�� ����Ѵ�.
	// TimeRange	: ���� �ð��� �ֱ� ������ �ð��� ���̳��� ���� ���Ͽ� ���� ���� ����(��)�� ����Ѵ�.
	int Range = mt4HourEndTime.GetTimeDiffTimeInMinutes(mtCurTime);	
	if(Range <= 0)
	{// 2009-03-23 by dhjin, �ֱ� ������ �ð� üũ ������ ����
		return;
	}
	int TimeRange = 0;
	// ��������Ʈ ���� ���� �ð� ������ �����ϸ� �� ���� ���� �ð��� ��´�.
	INT TotalSPPMin = this->SetSPPossibleTimeInfo(&mtCurTime, &mt4HourEndTime);
	BYTE MaxSPSummonCountByMapIndex = 0;
	BYTE TotalSPSummonCount = 0;
	// ��������Ʈ�� �� �ʿ� �ִ� �� ���� ��ȯ �Ǿ�� ���� üũ�ϸ� �� ��ȯ ���� �� ���� ���� �ð��� ���� ���� ���� ���� ���Ѵ�.
	this->SetSPSummonCountInfo(&MaxSPSummonCountByMapIndex, &TotalSPSummonCount);

	// 2009-07-02 by cmkwon, ���� ����� ���� ���� �α� �߰� - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CInflWarManager::SetAllStrategyPointSummonTimeNew# Period(%s ~ %s) Range(%d minutes) TotalSPPMin(%d) TotalSPSummonCount(%d) \r\n"
		, mt4HourStartTime.GetDateTimeString().GetBuffer(), mt4HourEndTime.GetDateTimeString().GetBuffer(), Range, TotalSPPMin, TotalSPSummonCount);
	if(0 >= TotalSPSummonCount)
	{// 2009-07-02 by cmkwon, �Ű��� ���� ���� ���� - 
		return;
	}

// 2010-01-07 by cmkwon, ������ �ֽ� �ð����� 10������ ���� - ���� 30��
//	INT SPPMin = max(30, TotalSPPMin/TotalSPSummonCount);		// 2009-11-11 by cmkwon, ��������Ʈ ��ȯ ���� ���� - �ּ� 30�� �̻� ���̰� ������
	INT SPPMin = max(10, TotalSPPMin/TotalSPSummonCount);		// 2009-11-11 by cmkwon, ��������Ʈ ��ȯ ���� ���� - �ּ� 30�� �̻� ���̰� ������
	INT SPStepMin = 0;

	// ��������Ʈ ��ȯ�� ���� �����ϰ� �����Ѵ�.
	std::random_shuffle(m_mtvectStrategyPointSummonTimeInfo.begin(), m_mtvectStrategyPointSummonTimeInfo.end());
	// �� �ʿ� ��ȯ�� ���� ������ �����Ѵ�.
	this->SortBySPSummonCount();

	m_mtvectALLSPSummonTimeInfo.clear();
	mt_auto_lock AllSP(&m_mtvectALLSPSummonTimeInfo);
	SSTRATEGYPOINT_SUMMONTIME_INFO tmSPSummonInfo;
	util::zero(&tmSPSummonInfo, sizeof(SSTRATEGYPOINT_SUMMONTIME_INFO));
	for(int SPSCountByMapindex = 1; SPSCountByMapindex <= MaxSPSummonCountByMapIndex; SPSCountByMapindex++)
	{
		mtvectSSTRATEGYPOINT_SUMMONTIME_INFO::iterator itr = m_mtvectStrategyPointSummonTimeInfo.begin();
		for(; itr != m_mtvectStrategyPointSummonTimeInfo.end(); itr++)
		{
			if(SPSCountByMapindex <= itr->SummonCount)
			{// �� �ֱ⿡ ��ȯ �Ǿ�� �ϴ� ������ ���� m_mtvectALLSPSummonTimeInfo �����Ѵ�.
				tmSPSummonInfo.MapIndex	= itr->MapIndex;
				this->CalcSPSummonTime(SPStepMin, SPPMin, &(tmSPSummonInfo.SummonTime), &(tmSPSummonInfo.SummonAttribute));
				if(!tmSPSummonInfo.SummonTime.IsValid())
				{
					tmSPSummonInfo.SummonTime.SetCurrentDateTime();
					tmSPSummonInfo.SummonTime.AddDateTime(1,0,0,0,0,0);
				}
				SPStepMin += SPPMin;
				m_mtvectALLSPSummonTimeInfo.push_back(tmSPSummonInfo);

				g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify]: AllStrategypoint Summon Info, MapIndex(%d), SummonTime(%s) SPStepMin(%4d) SPPMin(%4d)\r\n"
					, tmSPSummonInfo.MapIndex, tmSPSummonInfo.SummonTime.GetDateTimeString().GetBuffer(), SPStepMin, SPPMin);
				
				if(1 == SPSCountByMapindex)
				{// ù ������ ������Ʈ �Ѵ�. ���� ������ ��ȯ �Ǹ� m_mtvectALLSPSummonTimeInfo���� ������ ������ ������Ʈ�� ���ش�.
					itr->SummonTime			= tmSPSummonInfo.SummonTime;
					itr->SummonAttribute	= tmSPSummonInfo.SummonAttribute;
					QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME *pQparam = new QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME;
					pQparam->MapIndex		= itr->MapIndex;
					pQparam->SummonTime		= itr->SummonTime;
					pQparam->SummonAttribute	= itr->SummonAttribute;		// 2008-04-08 by dhjin, ��ȯ ���� �ð� ���� - 
					this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateStrategyPointSummonInfoBySummonTime, NULL, 0, pQparam);		
				}
				util::zero(&tmSPSummonInfo, sizeof(SSTRATEGYPOINT_SUMMONTIME_INFO));
			}
		}
	}
	AllSP.auto_unlock_cancel();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT CInflWarManager::SetSPPossibleTimeInfo(ATUM_DATE_TIME *i_pSTime, ATUM_DATE_TIME *i_pETime)
/// \brief		��������Ʈ �ð� ���� ���� - ��������Ʈ ��ȯ ������ �ð��� ���� ����
/// \author		dhjin
/// \date		2009-03-23 ~ 2009-03-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CInflWarManager::SetSPPossibleTimeInfo(ATUM_DATE_TIME *i_pSTime, ATUM_DATE_TIME *i_pETime)
{
	m_vectSSPPossibleTimeInfo.clear();
	SSPPossibleTimeInfo	SPPTimeInfo;
	INT TotalSPPossibleMin = 0;
	ATUM_DATE_TIME CheckSTime;
	CheckSTime	= *i_pSTime;
	CheckSTime.Hour = CheckSTime.Minute = CheckSTime.Second	= 0;	// 2009-11-11 by cmkwon, ��������Ʈ ��ȯ ���� ���� - 00:00:00 �� �ð� �ʱ�ȭ

	mt_auto_lock mta(this->GetStrategyPointNotSummonTime());

	for(; CheckSTime < *i_pETime; CheckSTime.AddDateTime(0,0,1,0,0,0))
	{// �ֱ� ������������ üũ
		util::zero(&SPPTimeInfo, sizeof(SSPPossibleTimeInfo));
		
		auto localStTime = tm(CheckSTime);
		
		vectSSTRATEGYPOINT_NOT_SUMMON_TIME::iterator itrSt = m_mtvectStrategyPointNotSummonTime.begin();
		while(itrSt != m_mtvectStrategyPointNotSummonTime.end())
		{// 2007-03-30 by dhjin, ù�� �����ð� ������
			if(localStTime.tm_wday == itrSt->DayOfWeek)
			{
				if(itrSt->CheckSummon)
				{// ��ȯ ���� �ð� ������ �Ϸ翡 �� ���� ���ü��ִ�.
					SPPTimeInfo.StartTime.SetDateTime(CheckSTime.Year, CheckSTime.Month, CheckSTime.Day
														, itrSt->StartTime.Hour, itrSt->StartTime.Minute, itrSt->StartTime.Second);
					SPPTimeInfo.EndTime.SetDateTime(CheckSTime.Year, CheckSTime.Month, CheckSTime.Day
														, itrSt->EndTime.Hour, itrSt->EndTime.Minute, itrSt->EndTime.Second);


					///////////////////////////////////////////////////////////////////////////////
					// 2010-01-28 by cmkwon, ������ ��ȯ �ð� ���� ����(������ ���� �ð� ���°� ����) - �Ⱓ �̿��� �ð� üũ
					if(*i_pSTime >= SPPTimeInfo.EndTime
						|| *i_pETime <= SPPTimeInfo.StartTime)
					{
						break;
					}

// 2010-01-28 by cmkwon, ������ ��ȯ �ð� ���� ����(������ ���� �ð� ���°� ����) - 
// 					if(SPPTimeInfo.EndTime > *i_pETime)
// 					{// 2009-11-11 by cmkwon, ��������Ʈ ��ȯ ���� ���� - �ֱ� ����ð� üũ
// 						SPPTimeInfo.EndTime = *i_pETime;
// 					}
// 
// 					if(*i_pSTime > SPPTimeInfo.StartTime
// 						&& *i_pSTime < SPPTimeInfo.EndTime)
// 					{// ������ �ð��� ���� �ð����� �� �ð��̸� ���� �ð��� ���� �ð����� ��´�.
// 						SPPTimeInfo.StartTime.SetDateTime(CheckSTime.Year, CheckSTime.Month, CheckSTime.Day
// 															, i_pSTime->Hour, i_pSTime->Minute, i_pSTime->Second);
// 					}
// 					else if(*i_pSTime > SPPTimeInfo.EndTime)
// 					{// ������ �ð��� ���ð��� ���� �ð����� �� �ð��̸� ������ �� �ִ� �ð��� ����.
// 						break;
// 					}
					///////////////////////////////////////////////////////////////////////////////
					// 2010-01-28 by cmkwon, ������ ��ȯ �ð� ���� ����(������ ���� �ð� ���°� ����) - 
					if(*i_pSTime > SPPTimeInfo.StartTime)
					{
 						SPPTimeInfo.StartTime.SetDateTime(CheckSTime.Year, CheckSTime.Month, CheckSTime.Day, i_pSTime->Hour, i_pSTime->Minute, i_pSTime->Second);
					}
					if(*i_pETime < SPPTimeInfo.EndTime)
					{
 						SPPTimeInfo.EndTime = *i_pETime;
					}
					
					SPPTimeInfo.PossibleMinRange = SPPTimeInfo.EndTime.GetTimeDiffTimeInMinutes(SPPTimeInfo.StartTime);
					SPPTimeInfo.CheckSummon = TRUE;
					m_vectSSPPossibleTimeInfo.push_back(SPPTimeInfo);		// 2010-01-28 by cmkwon, ������ ��ȯ �ð� ���� ����(������ ���� �ð� ���°� ����) - 
					TotalSPPossibleMin += SPPTimeInfo.PossibleMinRange;		// 2010-01-28 by cmkwon, ������ ��ȯ �ð� ���� ����(������ ���� �ð� ���°� ����) - 

// 2010-01-28 by cmkwon, ������ ��ȯ �ð� ���� ����(������ ���� �ð� ���°� ����) - 
// 					if(OUTPOST_WARTIME < SPPTimeInfo.PossibleMinRange)
// 					{// 2009-11-11 by cmkwon, ��������Ʈ ��ȯ ���� ���� - 
// 						SPPTimeInfo.EndTime.AddDateTime(0, 0, 0, 0, -OUTPOST_WARTIME);		// 2009-11-11 by cmkwon, ��������Ʈ ��ȯ ���� ���� - 
// 						SPPTimeInfo.PossibleMinRange	-= OUTPOST_WARTIME;					// 2009-11-11 by cmkwon, ��������Ʈ ��ȯ ���� ���� - 
// 						m_vectSSPPossibleTimeInfo.push_back(SPPTimeInfo);
// 						TotalSPPossibleMin += SPPTimeInfo.PossibleMinRange;
// 					}

				}
				else
				{// ��ȯ ���� �ð��� ��ȯ ���� �ð� ������ �Ϸ翡 �� �� ���´�. 
					SPPTimeInfo.StartTime.SetDateTime(CheckSTime.Year, CheckSTime.Month, CheckSTime.Day
														, 0, 0, 0);
					SPPTimeInfo.EndTime.SetDateTime(CheckSTime.Year, CheckSTime.Month, CheckSTime.Day
														, itrSt->StartTime.Hour, itrSt->StartTime.Minute, itrSt->StartTime.Second);

					if(SPPTimeInfo.EndTime > *i_pETime)
					{// 2009-11-11 by cmkwon, ��������Ʈ ��ȯ ���� ���� - �ֱ� ����ð� üũ
						SPPTimeInfo.EndTime = *i_pETime;
					}

					SPPTimeInfo.PossibleMinRange = SPPTimeInfo.EndTime.GetTimeDiffTimeInMinutes(SPPTimeInfo.StartTime);
					if(*i_pSTime > SPPTimeInfo.StartTime
						&& *i_pSTime < SPPTimeInfo.EndTime)
					{// ������ �ð��� ���� �ð����� �� �ð��̸� ���� �ð��� ���� �ð����� ��´�.
						SPPTimeInfo.StartTime.SetDateTime(CheckSTime.Year, CheckSTime.Month, CheckSTime.Day
															, i_pSTime->Hour, i_pSTime->Minute, i_pSTime->Second);
					}
					if(*i_pSTime < SPPTimeInfo.EndTime)
					{
						SPPTimeInfo.CheckSummon = FALSE;
						m_vectSSPPossibleTimeInfo.push_back(SPPTimeInfo);		// 2010-01-28 by cmkwon, ������ ��ȯ �ð� ���� ����(������ ���� �ð� ���°� ����) - 
						TotalSPPossibleMin += SPPTimeInfo.PossibleMinRange;		// 2010-01-28 by cmkwon, ������ ��ȯ �ð� ���� ����(������ ���� �ð� ���°� ����) - 

// 2010-01-28 by cmkwon, ������ ��ȯ �ð� ���� ����(������ ���� �ð� ���°� ����) - 
// 						if(OUTPOST_WARTIME < SPPTimeInfo.PossibleMinRange)
// 						{// 2009-11-11 by cmkwon, ��������Ʈ ��ȯ ���� ���� - 
// 							SPPTimeInfo.EndTime.AddDateTime(0, 0, 0, 0, -OUTPOST_WARTIME);		// 2009-11-11 by cmkwon, ��������Ʈ ��ȯ ���� ���� - 
// 							SPPTimeInfo.PossibleMinRange	-= OUTPOST_WARTIME;					// 2009-11-11 by cmkwon, ��������Ʈ ��ȯ ���� ���� - 
// 							m_vectSSPPossibleTimeInfo.push_back(SPPTimeInfo);
// 							TotalSPPossibleMin += SPPTimeInfo.PossibleMinRange;
// 						}
					}
					SPPTimeInfo.StartTime.SetDateTime(CheckSTime.Year, CheckSTime.Month, CheckSTime.Day, itrSt->EndTime.Hour, itrSt->EndTime.Minute, itrSt->EndTime.Second);
					SPPTimeInfo.EndTime.SetDateTime(CheckSTime.Year, CheckSTime.Month, CheckSTime.Day, 23, 59, 59);		// 2009-11-11 by cmkwon, ��������Ʈ ��ȯ ���� ���� - 23:59:59 �� ����

					if(SPPTimeInfo.EndTime > *i_pETime)
					{// 2009-11-11 by cmkwon, ��������Ʈ ��ȯ ���� ���� - �ֱ� ����ð� üũ
						SPPTimeInfo.EndTime = *i_pETime;
					}

					if(*i_pSTime > SPPTimeInfo.StartTime
						&& *i_pSTime < SPPTimeInfo.EndTime)
					{// ������ �ð��� ���� �ð����� �� �ð��̸� ���� �ð��� ���� �ð����� ��´�.
						SPPTimeInfo.StartTime.SetDateTime(CheckSTime.Year, CheckSTime.Month, CheckSTime.Day
															, i_pSTime->Hour, i_pSTime->Minute, i_pSTime->Second);
					}
					else if(*i_pSTime > SPPTimeInfo.EndTime)
					{// ������ �ð��� ���ð��� ���� �ð����� �� �ð��̸� ������ �� �ִ� �ð��� ����.
						break;
					}
					SPPTimeInfo.PossibleMinRange = SPPTimeInfo.EndTime.GetTimeDiffTimeInMinutes(SPPTimeInfo.StartTime);
					SPPTimeInfo.CheckSummon = FALSE;
					m_vectSSPPossibleTimeInfo.push_back(SPPTimeInfo);		// 2010-01-28 by cmkwon, ������ ��ȯ �ð� ���� ����(������ ���� �ð� ���°� ����) - 
					TotalSPPossibleMin += SPPTimeInfo.PossibleMinRange;		// 2010-01-28 by cmkwon, ������ ��ȯ �ð� ���� ����(������ ���� �ð� ���°� ����) - 

// 2010-01-28 by cmkwon, ������ ��ȯ �ð� ���� ����(������ ���� �ð� ���°� ����) - 
// 					if(OUTPOST_WARTIME < SPPTimeInfo.PossibleMinRange)
// 					{// 2009-11-11 by cmkwon, ��������Ʈ ��ȯ ���� ���� - 
// 						SPPTimeInfo.EndTime.AddDateTime(0, 0, 0, 0, -OUTPOST_WARTIME);		// 2009-11-11 by cmkwon, ��������Ʈ ��ȯ ���� ���� - 
// 						SPPTimeInfo.PossibleMinRange	-= OUTPOST_WARTIME;					// 2009-11-11 by cmkwon, ��������Ʈ ��ȯ ���� ���� - 
// 						m_vectSSPPossibleTimeInfo.push_back(SPPTimeInfo);
// 						TotalSPPossibleMin += SPPTimeInfo.PossibleMinRange;
// 					}

				}
				break;
			}
			itrSt++;
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2009-11-11 by cmkwon, ��������Ʈ ��ȯ ���� ���� - ��ȯ ������ �ð� �ý��� �α׿� �����
	{
		vectSSPPossibleTimeInfo::iterator itrSPPTInfo(m_vectSSPPossibleTimeInfo.begin());
		for(; itrSPPTInfo != m_vectSSPPossibleTimeInfo.end(); itrSPPTInfo++)
		{
			SSPPossibleTimeInfo *pPTInfo = &*itrSPPTInfo;

			g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CInflWarManager::SetSPPossibleTimeInfo# PossibleTime(%s ~ %s) TermMinutes(%4d) CheckSummon(%d) \r\n"
				, pPTInfo->StartTime.GetDateTimeString().GetBuffer(), pPTInfo->EndTime.GetDateTimeString().GetBuffer()
				, pPTInfo->PossibleMinRange, pPTInfo->CheckSummon);
		}
	}

	return TotalSPPossibleMin;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SetSPSummonCountInfo(BYTE *o_pMaxSPSummonCountByMapIndex, BYTE *o_pTotalSPSummonCount)
/// \brief		��������Ʈ �ð� ���� ���� - �� �ֱ⿡ ��ȯ �Ǿ�� �Ǵ� �� �� ���� �� ���� ����Ѵ�.
/// \author		dhjin
/// \date		2009-03-30 ~ 2009-03-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SetSPSummonCountInfo(BYTE *o_pMaxSPSummonCountByMapIndex, BYTE *o_pTotalSPSummonCount)
{
	mtvectSSTRATEGYPOINT_SUMMONTIME_INFO::iterator itr = m_mtvectStrategyPointSummonTimeInfo.begin();
	for(; itr != m_mtvectStrategyPointSummonTimeInfo.end(); itr++)
	{
		if(*o_pMaxSPSummonCountByMapIndex < itr->SummonCount)
		{
			*o_pMaxSPSummonCountByMapIndex = itr->SummonCount;
		}
		*o_pTotalSPSummonCount += itr->SummonCount;
	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SortBySPSummonCount()
/// \brief		�ʴ� ��ȯ �Ǵ� ���� ���� ���� �� ������ �����Ѵ�.
/// \author		dhjin
/// \date		2009-03-30 ~ 2009-03-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SortBySPSummonCount()
{
	sort(m_mtvectStrategyPointSummonTimeInfo.begin(), m_mtvectStrategyPointSummonTimeInfo.end(), SSTRATEGYPOINT_SUMMONTIME_INFO_BY_SUMMONCOUNT());
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::GetNextSPSummonTimeByALLSPSummonTimeInfo(INT i_nMapindex, ATUM_DATE_TIME *o_pSummonTime, BOOL *o_bCheckSummon)
/// \brief		��������Ʈ �ð� ���� ���� - ���� ��������Ʈ ��ȯ�Ǵ� ���� ������
/// \author		dhjin
/// \date		2009-03-30 ~ 2009-03-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::GetNextSPSummonTimeByALLSPSummonTimeInfo(INT i_nMapindex, ATUM_DATE_TIME *o_pSummonTime, BOOL *o_bCheckSummon)
{
	mtvectSSTRATEGYPOINT_SUMMONTIME_INFO::iterator itr = m_mtvectALLSPSummonTimeInfo.begin();
	for(; itr != m_mtvectALLSPSummonTimeInfo.end(); itr++)
	{
		if(i_nMapindex == itr->MapIndex)
		{
			*o_pSummonTime		= itr->SummonTime;
			*o_bCheckSummon		= itr->SummonAttribute;
			return TRUE;
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckSPSummonMapIndex(INT i_nMapIndex)
/// \brief		�ش� �ʿ� ��������Ʈ�� ���� �Ǿ� �ִ��� üũ
/// \author		dhjin
/// \date		2009-03-31 ~ 2009-03-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckSPSummonMapIndex(INT i_nMapIndex)
{
	mt_auto_lock mta(&m_mtvectSummonStrategyPointInfo);

	mtvectorSSUMMON_STRATEGYPOINT_INFO::iterator	itr = m_mtvectSummonStrategyPointInfo.begin();
	while(itr != m_mtvectSummonStrategyPointInfo.end())
	{
		if(i_nMapIndex == itr->MapIndex)
		{
			return TRUE;
		}
		itr++;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::DeleteALLSPSummonTimeInfoByMapIndex(INT i_nMapindex)
/// \brief		��������Ʈ �ð� ���� ���� - ��ȯ�� ��������Ʈ ���� ����
/// \author		dhjin
/// \date		2009-03-30 ~ 2009-03-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::DeleteALLSPSummonTimeInfoByMapIndex(INT i_nMapindex)
{
	mtvectSSTRATEGYPOINT_SUMMONTIME_INFO::iterator itr = m_mtvectALLSPSummonTimeInfo.begin();
	for(; itr != m_mtvectALLSPSummonTimeInfo.end(); itr++)
	{
		if(i_nMapindex == itr->MapIndex)
		{
			itr = m_mtvectALLSPSummonTimeInfo.erase(itr);
			return TRUE;
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::CalcSPSummonTime(INT i_nSPStepMin, INT i_nSPPMin, ATUM_DATE_TIME *o_pSPTime, BOOL *o_bCheckSummon)
/// \brief		��������Ʈ �ð� ���� ���� - ��������Ʈ ��ȯ �ð� ���
/// \author		dhjin
/// \date		2009-03-24 ~ 2009-03-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::CalcSPSummonTime(INT i_nSPStepMin, INT i_nSPPMin, ATUM_DATE_TIME *o_pSPTime, BOOL *o_bCheckSummon)
{
	INT RandomSPMin = RANDI(0, i_nSPPMin);
	RandomSPMin += i_nSPStepMin;
	vectSSPPossibleTimeInfo::iterator itr = m_vectSSPPossibleTimeInfo.begin();
	for(; itr != m_vectSSPPossibleTimeInfo.end(); itr++)
	{
		if(itr->PossibleMinRange >= RandomSPMin)
		{
			(*o_pSPTime).SetDateTime(itr->StartTime.Year, itr->StartTime.Month, itr->StartTime.Day, itr->StartTime.Hour, itr->StartTime.Minute, 0);
			(*o_pSPTime).AddDateTime(0,0,0,0,RandomSPMin,0);
			*o_bCheckSummon = itr->CheckSummon;
			return;
		}
		
		RandomSPMin -= itr->PossibleMinRange;
	}
}

void CInflWarManager::SetStrategyPointSummonTime(SSTRATEGYPOINT_SUMMONTIME_INFO *i_pStrategyPointSummonTimeInfo, INT i_nSummonHourGap)
{
	//////////////////////////////////////////////////////////////////////////
	// 2007-03-12 by dhjin, 
	// mt4HourStartTime : �� �ֱ��� StartTime���� 4�ð��� ���Ѵ�. �ֱ� ������ ������ �յڷ� �ð����� �ξ� �ߺ������� ���Ѵ�.
	// mt4HourEndTime   : �� �ֱ��� EndTime���� 4�ð��� ���ش�.
	// mtCurTime		: ���� �ð��� ���ϰ� ���� �ð��� ���� �ð� ���� ���̸� ���� �ð��� ���� �ð����� �����Ѵ�. 
	//					  ��������Ʈ�� ���� �ǰ� �� ���� ȣ��Ǵ� �κ��̱� ������ ���� �ð����κ��� i_nSummonHourGap�� ���Ѵ�.
	ATUM_DATE_TIME mt4HourStartTime;
	mt4HourStartTime = m_StrategyPointSummonRange.StartTime;
	mt4HourStartTime.AddDateTime(0,0,0,4,0,0);
	ATUM_DATE_TIME mt4HourEndTime;
	mt4HourEndTime = m_StrategyPointSummonRange.EndTime;
	mt4HourEndTime.AddDateTime(0,0,0,-4,0,0);
	ATUM_DATE_TIME mtCurTime;
	mtCurTime.SetCurrentDateTime();
	mtCurTime.AddDateTime(0,0,0,i_nSummonHourGap,0,0);
	if(0 < mt4HourStartTime.GetTimeDiffTimeInSeconds(mtCurTime))
	{// 2007-03-05 by dhjin, ���� �ð��� ���� �ð� ���� ���̸� ���� �ð��� ���� �ð����� �����Ѵ�.
		mtCurTime = mt4HourStartTime;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-03-12 by dhjin, 
	// Range		: ���� �ð��� �ֱ� ������ �ð��� ���̳��� �ʸ� ���Ͽ� ���� ���� ����(��¥)�� ����Ѵ�.
	// TimeRange	: ���� �ð��� �ֱ� ������ �ð��� ���̳��� �ʸ� ���Ͽ� ���� ���� ����(��)�� ����Ѵ�.
	int Range = mt4HourEndTime.GetTimeDiffTimeInMinutes(mtCurTime);
	int TimeRange = 0;

	if(FALSE == this->CompareStrategyPointDay()
		|| 0 >= Range)
	{// 2007-03-07 by dhjin, �� �̻� �Ҵ��� ������ ����. ���� �ֱ�� �ѱ��.
		i_pStrategyPointSummonTimeInfo->SummonTime.AddDateTime(1,0,0, 0,0,0);
		QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME *pQparam = new QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME;
		pQparam->MapIndex		= i_pStrategyPointSummonTimeInfo->MapIndex;
		pQparam->SummonTime		= i_pStrategyPointSummonTimeInfo->SummonTime;
		pQparam->SummonAttribute	= FALSE;		// 2008-04-08 by dhjin, ��ȯ ���� �ð� ���� - 
		this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateStrategyPointSummonInfoBySummonTime, NULL, 0, pQparam);
		return;
	}
	
	// 2007-03-07 by dhjin, ���Ͽ� �Ҵ��Ѵ�.
	i_pStrategyPointSummonTimeInfo->SummonTime = mtCurTime;
	i_pStrategyPointSummonTimeInfo->SummonTime.AddDateTime(0,0,0, 0,(RANDI(1, Range)),0);
	ATUM_DATE_TIME tmMakeTime;
	tmMakeTime.SetDateTime(i_pStrategyPointSummonTimeInfo->SummonTime.Year, i_pStrategyPointSummonTimeInfo->SummonTime.Month, i_pStrategyPointSummonTimeInfo->SummonTime.Day, 0, 0, 0);

	//////////////////////////////////////////////////////////////////////////
	// 2007-03-12 by dhjin
	// tmCheckRangeTime : ���� �� ��¥�� ��� �ֱ��� ������ üũ�Ͽ� �� ��¥�� �����ϰ� ���� ������ ������ �ʸ� �����ϱ� ���� ����.
	// tmIncreaseDay	: ���� ���� ���̱� ���� ���� �� �������� ������ �����ϸ� ���ڸ� �ϳ��� ���Ͽ� ��ȯ ������ ���ڿ� �����Ѵ�.
	int				tmCheckRangeTime = 1;		
	int				tmIncreaseDay = 0;		
	
	while(FALSE == this->CompareStrategyPointDayByOneDay(&tmMakeTime, &tmCheckRangeTime))
	{// 2007-03-09 by dhjin, ��ȯ ������ ���ڰ� �ִ��� üũ�Ͽ� �� ���ڸ� �켱 �����Ѵ�.
		i_pStrategyPointSummonTimeInfo->SummonTime = mtCurTime;
		i_pStrategyPointSummonTimeInfo->SummonTime.AddDateTime(0,0,tmIncreaseDay, 0,0,0);
		tmIncreaseDay++;
		tmMakeTime.SetDateTime(i_pStrategyPointSummonTimeInfo->SummonTime.Year, i_pStrategyPointSummonTimeInfo->SummonTime.Month, i_pStrategyPointSummonTimeInfo->SummonTime.Day, 0, 0, 0);
	}
	this->IncreaseStrategyPointDay(&(i_pStrategyPointSummonTimeInfo->SummonTime));

	//////////////////////////////////////////////////////////////////////////
	// ���� ���� ���� ���� ���̰ų� �����̸� �����ϰ� ������ �ð� Range���� �����Ѵ�.
	if(1 == m_mtvectStrategyPointDay.size())
	{
		TimeRange = Range;
		tmMakeTime = mtCurTime;
	}
	else if(1 == tmCheckRangeTime)
	{
		ATUM_DATE_TIME	tmPlusOneDay;
		tmPlusOneDay.SetDateTime(mtCurTime.Year, mtCurTime.Month, mtCurTime.Day+1,0,0,0);
		TimeRange = tmPlusOneDay.GetTimeDiffTimeInMinutes(mtCurTime);
		tmMakeTime = mtCurTime;
	}
	else if(m_mtvectStrategyPointDay.size() == tmCheckRangeTime)
	{
		TimeRange = mt4HourEndTime.GetTimeDiffTimeInMinutes(tmMakeTime);
	}
	else
	{
		TimeRange = 24 * 60;
	}

	BOOL SummonAttribute = FALSE;		// 2008-04-08 by dhjin, ��ȯ ���� �ð� ���� - 

	// 2007-03-12 by dhjin, �ð��� ���Ϻ� ���� �ð��� ���� �����ϰ� �����Ѵ�.
	if(FALSE == this->CheckStrategyPointNotSummonTime(&tmMakeTime, TimeRange, &SummonAttribute))
	{
		char szErr[1024];
		wsprintf(szErr, "CInflWarManager::Not_SummonTime Error");
		g_pFieldGlobal->WriteSystemLog(szErr);
		DbgOut("%s\rn", szErr);
		// 2007-03-29 by dhjin, �Ҵ����� ���ߴٸ� ���� �ֱ⿡ ��ȯ�Ѵ�.
		i_pStrategyPointSummonTimeInfo->SummonTime.AddDateTime(1,0,0, 0,0,0);
		QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME *pQparam = new QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME;
		pQparam->MapIndex		= i_pStrategyPointSummonTimeInfo->MapIndex;
		pQparam->SummonTime		= i_pStrategyPointSummonTimeInfo->SummonTime;
		pQparam->SummonAttribute	= FALSE;		// 2008-04-08 by dhjin, ��ȯ ���� �ð� ���� - 
		this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateStrategyPointSummonInfoBySummonTime, NULL, 0, pQparam);
		return;
	}

	// 2007-03-12 by dhjin, ������ ��ȯ�ð� ������ ����ü�� �Ҵ��Ѵ�.
	i_pStrategyPointSummonTimeInfo->SummonTime = tmMakeTime;
	// 2007-03-12 by dhjin, ������ ��ȯ�ð� ������ DB�� ������Ʈ �Ѵ�.
	QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME *pQparam = new QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME;
	pQparam->MapIndex		= i_pStrategyPointSummonTimeInfo->MapIndex;
	pQparam->SummonTime		= tmMakeTime;
	pQparam->SummonAttribute= SummonAttribute;		// 2008-04-08 by dhjin, ��ȯ ���� �ð� ���� - 
	this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateStrategyPointSummonInfoBySummonTime, NULL, 0, pQparam);

}

BOOL CInflWarManager::CheckStrategyPointNotSummonTime(ATUM_DATE_TIME *i_ptmTime, int i_nRange, BOOL *o_bSummonAttribute)
{
	auto localTime = tm(*i_ptmTime);
	
	mt_auto_lock mta(this->GetStrategyPointNotSummonTime());
	vectSSTRATEGYPOINT_NOT_SUMMON_TIME::iterator itr = m_mtvectStrategyPointNotSummonTime.begin();
	while(itr != m_mtvectStrategyPointNotSummonTime.end())
	{
		// 2009-10-26 by cmkwon, ���� �ε��� �α� �߰� - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CInflWarManager::CheckStrategyPointNotSummonTime 10000 Day(%d) Time(%s ~ %s) \r\n"
			, itr->DayOfWeek, itr->StartTime.GetDateTimeString().GetBuffer(), itr->EndTime.GetDateTimeString().GetBuffer());

		if(localTime.tm_wday == itr->DayOfWeek)
		{
			*o_bSummonAttribute = itr->CheckSummon;		// 2008-04-08 by dhjin, ��ȯ ���� �ð� ���� - 
			int tmStime = ((itr->StartTime.Hour) * 60 * 60) + ((itr->StartTime.Minute) * 60) + ((itr->StartTime.Second));
			int tmEtime = ((itr->EndTime.Hour) * 60 * 60) + ((itr->EndTime.Minute) * 60) + ((itr->EndTime.Second));
			ATUM_DATE_TIME	tmTime;
			tmTime	= *i_ptmTime;
			tmTime.AddDateTime(0,0,0, 0,(RANDI(0, i_nRange)),0);
			int tmTimeRange  = ((tmTime.Hour) * 60 * 60) + ((tmTime.Minute) * 60) + ((tmTime.Second));

			int nCount	= 0; // 2007-03-29 by dhjin, ���� ���� Ż���� ���� ����.

			//////////////////////////////////////////////////////////////////////////
			// 2008-04-04 by dhjin, ��ȯ ���� �ð� ���� - 
			if(itr->CheckSummon)
			{
				while(FALSE == ((tmStime <= tmTimeRange) && (tmEtime >= tmTimeRange)))
				{// 2008-04-04 by dhjin, ��ȯ ���� �ð��̸� �ٽ� ����.
					if(30 <= nCount)
					{// 2007-03-29 by dhjin, 30�� �������� ������ �ȵǸ� ���� ����
						if(23 > itr->EndTime.Hour)
						{// 2008-04-04 by dhjin, ��ȯ ���� ������ �ð��� 23�� �����̸� �����Ѵ�.
							tmTime.SetDateTime(i_ptmTime->Year, i_ptmTime->Month, i_ptmTime->Day, itr->EndTime.Hour, itr->EndTime.Minute, itr->EndTime.Second);
							*i_ptmTime = tmTime;	// 2007-03-29 by dhjin, �������� ������ �ȵǸ� ������ �����ð� ���Ŀ� ���� �ǵ��� ����.
							return TRUE;
						}
						if(1 <= itr->StartTime.Hour)
						{// 2008-04-04 by dhjin, ��ȯ ���� ���� �ð��� 1�� �����̸� �����Ѵ�.
							tmTime.SetDateTime(i_ptmTime->Year, i_ptmTime->Month, i_ptmTime->Day, itr->StartTime.Hour, itr->StartTime.Minute, itr->StartTime.Second);
							*i_ptmTime = tmTime;	// 2007-03-29 by dhjin, �������� ������ �ȵǸ� ������ �����ð� ���Ŀ� ���� �ǵ��� ����.
							return TRUE;
						}
						return FALSE;	// 2007-03-29 by dhjin, ���ʴ� ������ �ȵȴٸ� ���� �ֱ�� �ѱ��.
					}
					tmTime	= *i_ptmTime;
					tmTime.AddDateTime(0,0,0, 0,(RANDI(0, i_nRange)),0);
					tmTimeRange  = ((tmTime.Hour) * 60 * 60) + ((tmTime.Minute) * 60) + ((tmTime.Second));
					nCount++;
				}
			}
			else
			{
				while((tmStime <= tmTimeRange) && (tmEtime >= tmTimeRange))
				{// 2007-03-09 by dhjin, ���� �ð��̸� �ٽ� ����.
					if(30 <= nCount)
					{// 2007-03-29 by dhjin, 30�� �������� ������ �ȵǸ� ���� ����
						if(23 > itr->EndTime.Hour)
						{// 2007-03-29 by dhjin, ���� ������ �ð��� 23�� �����̸� �����Ѵ�.
							tmTime.SetDateTime(i_ptmTime->Year, i_ptmTime->Month, i_ptmTime->Day, itr->EndTime.Hour, itr->EndTime.Minute, itr->EndTime.Second);
							*i_ptmTime = tmTime;	// 2007-03-29 by dhjin, �������� ������ �ȵǸ� ������ �����ð� ���Ŀ� ���� �ǵ��� ����.
							return TRUE;
						}
						if(1 <= itr->StartTime.Hour)
						{// 2007-03-29 by dhjin, ���� ���� �ð��� 1�� �����̸� �����Ѵ�.
							tmTime.SetDateTime(i_ptmTime->Year, i_ptmTime->Month, i_ptmTime->Day, itr->StartTime.Hour, itr->StartTime.Minute, itr->StartTime.Second);
							*i_ptmTime = tmTime;	// 2007-03-29 by dhjin, �������� ������ �ȵǸ� ������ �����ð� ���Ŀ� ���� �ǵ��� ����.
							return TRUE;
						}
						return FALSE;	// 2007-03-29 by dhjin, ���ʴ� ������ �ȵȴٸ� ���� �ֱ�� �ѱ��.
					}
					tmTime	= *i_ptmTime;
					tmTime.AddDateTime(0,0,0, 0,(RANDI(0, i_nRange)),0);
					tmTimeRange  = ((tmTime.Hour) * 60 * 60) + ((tmTime.Minute) * 60) + ((tmTime.Second));
					nCount++;
				}
			}

			*i_ptmTime = tmTime;
			return TRUE;
		}
		itr++;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::InitStrategyPointDay(ATUM_DATE_TIME *i_pSTime, ATUM_DATE_TIME *i_pETime)
/// \brief		��������Ʈ ���� ������ ��¥�� Count ���� �ʱ�ȭ
/// \author		dhjin
/// \date		2007-03-12 ~ 2007-03-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::InitStrategyPointDay(ATUM_DATE_TIME *i_pSTime, ATUM_DATE_TIME *i_pETime)
{
	ATUM_DATE_TIME tmSTime;
	tmSTime.SetDateTime(i_pSTime->Year, i_pSTime->Month, i_pSTime->Day,0,0,0);
	ATUM_DATE_TIME StartTime;	// 2007-03-29 by dhjin, ���۳� �񱳰�
	StartTime	= tmSTime;
	ATUM_DATE_TIME EndTime;		// 2007-03-29 by dhjin, ���� �񱳰�
	EndTime.SetDateTime(i_pETime->Year, i_pETime->Month, i_pETime->Day,0,0,0);

	SSTRATEGYPOINT_DAY	tmStrategyPointDay;
	util::zero(&tmStrategyPointDay, sizeof(SSTRATEGYPOINT_DAY));
	m_mtvectStrategyPointDay.clear();
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-03-29 by dhjin, �����ð����� ���� �����Ǹ� �ȵǴ� ����, �������� �� ���ֱ�
	auto localStTime = tm(*i_pSTime);
	
	ATUM_DATE_TIME tmSNotSTime;
	ATUM_DATE_TIME tmSNotETime;

	//////////////////////////////////////////////////////////////////////////
	// 2008-04-04 by dhjin, ��ȯ ���� �ð� ���� - 
	BOOL CheckSummonForStart	= FALSE;
	BOOL CheckSummonForEnd		= FALSE;

	mt_auto_lock mta(this->GetStrategyPointNotSummonTime());
	vectSSTRATEGYPOINT_NOT_SUMMON_TIME::iterator itrSt = m_mtvectStrategyPointNotSummonTime.begin();
	while(itrSt != m_mtvectStrategyPointNotSummonTime.end())
	{// 2007-03-30 by dhjin, ù�� �����ð� ������
		if(localStTime.tm_wday == itrSt->DayOfWeek)
		{
			tmSNotSTime.SetDateTime(i_pSTime->Year, i_pSTime->Month, i_pSTime->Day, itrSt->StartTime.Hour, itrSt->StartTime.Minute, itrSt->StartTime.Second);
			tmSNotETime.SetDateTime(i_pSTime->Year, i_pSTime->Month, i_pSTime->Day, itrSt->EndTime.Hour, itrSt->EndTime.Minute, itrSt->EndTime.Second);
			if(itrSt->CheckSummon)
			{// 2008-04-04 by dhjin, ��ȯ ���� �ð� ���� - 
				CheckSummonForStart = TRUE;
			}

			break;
		}
		itrSt++;
	}

	auto localEtTime = tm(*i_pETime);

	ATUM_DATE_TIME tmENotSTime;
	ATUM_DATE_TIME tmENotETime;
	vectSSTRATEGYPOINT_NOT_SUMMON_TIME::iterator itrEt = m_mtvectStrategyPointNotSummonTime.begin();
	while(itrEt != m_mtvectStrategyPointNotSummonTime.end())
	{// 2007-03-30 by dhjin, �������� �����ð� ������
		if(localEtTime.tm_wday == itrEt->DayOfWeek)
		{
			tmENotSTime.SetDateTime(i_pETime->Year, i_pETime->Month, i_pETime->Day, itrEt->StartTime.Hour, itrEt->StartTime.Minute, itrEt->StartTime.Second);
			tmENotETime.SetDateTime(i_pETime->Year, i_pETime->Month, i_pETime->Day, itrEt->EndTime.Hour, itrEt->EndTime.Minute, itrEt->EndTime.Second);
			if(itrEt->CheckSummon)
			{// 2008-04-04 by dhjin, ��ȯ ���� �ð� ���� - 
				CheckSummonForEnd = TRUE;
			}
			
			break;
		}
		itrEt++;
	}
	mta.auto_unlock_cancel();
	
	while(tmSTime <= *i_pETime)
	{
		if(0 == tmSTime.GetTimeDiffTimeInHours(StartTime))
		{// 2007-03-29 by dhjin, ���۳� ���ؼ� �����ð����� ���� ������ �Ұ����ϸ� ���� ��¥��
			//////////////////////////////////////////////////////////////////////////
			// 2008-04-04 by dhjin, ��ȯ ���� �ð� ���� - 
			if(CheckSummonForStart)
			{
				if(0 > i_pSTime->GetTimeDiffTimeInSeconds(tmSNotSTime)
					|| 0 < i_pSTime->GetTimeDiffTimeInSeconds(tmSNotETime)
					|| 23 <= tmSNotETime.Hour)
				{
					tmSTime.AddDateTime(0,0,1,0,0,0);
					i_pSTime->SetDateTime(tmSTime.Year, tmSTime.Month, tmSTime.Day, 0, 0, 1);
					continue;
				}
			}
			else
			{
				if(0 < i_pSTime->GetTimeDiffTimeInSeconds(tmSNotSTime)
					&& 0 > i_pSTime->GetTimeDiffTimeInSeconds(tmSNotETime)
					&& 23 <= tmSNotETime.Hour)
				{
					tmSTime.AddDateTime(0,0,1,0,0,0);
					i_pSTime->SetDateTime(tmSTime.Year, tmSTime.Month, tmSTime.Day, 0, 0, 1);
					continue;
				}
			}

		}

		if(0 == tmSTime.GetTimeDiffTimeInHours(EndTime))
		{// 2007-03-29 by dhjin, ���� ���ؼ� �����ð����� ���� ������ �Ұ����ϸ� ���� ��¥��
			//////////////////////////////////////////////////////////////////////////
			// 2008-04-04 by dhjin, ��ȯ ���� �ð� ���� - 
			if(CheckSummonForEnd)
			{
				if(0 > i_pETime->GetTimeDiffTimeInSeconds(tmENotSTime)
					|| 0 < i_pETime->GetTimeDiffTimeInSeconds(tmENotETime)
					|| 1 >= tmENotSTime.Hour)
				{
					tmSTime.AddDateTime(0,0,1,0,0,0);
					continue;
				}
			
			}
			else
			{
				if(0 < i_pETime->GetTimeDiffTimeInSeconds(tmENotSTime)
					&& 0 > i_pETime->GetTimeDiffTimeInSeconds(tmENotETime)
					&& 1 >= tmENotSTime.Hour)
				{
					tmSTime.AddDateTime(0,0,1,0,0,0);
					continue;
				}
			}

		}

		tmStrategyPointDay.Year		= tmSTime.Year;
		tmStrategyPointDay.Month	= tmSTime.Month;
		tmStrategyPointDay.Day		= tmSTime.Day;
		tmStrategyPointDay.MaxCount = 0;
		m_mtvectStrategyPointDay.push_back(tmStrategyPointDay);

		util::zero(&tmStrategyPointDay, sizeof(SSTRATEGYPOINT_DAY));
		tmSTime.AddDateTime(0,0,1,0,0,0);
	}
	
	mtvectSSTRATEGYPOINT_SUMMONTIME_INFO::iterator itr = m_mtvectStrategyPointSummonTimeInfo.begin();
	while(itr != m_mtvectStrategyPointSummonTimeInfo.end())
	{// 2007-03-08 by dhjin, ��������Ʈ ��ȯ �ð����� �����ϱ� ���� ��ȯ�Ǹ� �ȵǴ� Max���� üũ�ϱ� ���� �̸� ��ȯ�� ������ ���Ͽ� ������Ʈ �Ѵ�.
		if(0 == itr->SummonCount)
		{
			ATUM_DATE_TIME tmTime;
			tmTime.SetDateTime(itr->SummonTime.Year, itr->SummonTime.Month, itr->SummonTime.Day, 0, 0, 0);
			ATUM_DATE_TIME tmITime;
			tmITime.SetDateTime(i_pSTime->Year, i_pSTime->Month, i_pSTime->Day, 0, 0, 0);
			if(0 == tmITime.GetTimeDiffTimeInHours(tmTime))
			{
				this->IncreaseStrategyPointDay(&tmTime);
			}
		}
		itr++;
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CompareStrategyPointDay()
/// \brief		��������Ʈ ������ ������ ��¥�� �ִ� �� üũ�Ѵ�.
/// \author		dhjin
/// \date		2007-03-12 ~ 2007-03-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CompareStrategyPointDay()
{
	this->CurrentStrategyPointDay();
	
	mtvectSSTRATEGYPOINT_DAY::iterator itr = m_mtvectStrategyPointDay.begin();
	while(itr != m_mtvectStrategyPointDay.end())
	{
		if(itr->MaxCount < m_StrategyPointSummonRange.MaxSummonCountDaily)
		{// 2007-03-08 by dhjin, �Ҵ��� �� �ִ� ������ �ִ�.
			return TRUE;
		}
		
		itr++;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::CurrentStrategyPointDay()
/// \brief		m_mtvectStrategyPointDay����ü���� ���� ��¥�� ���ʿ��� �����̹Ƿ� �����Ѵ�.
/// \author		dhjin
/// \date		2007-03-12 ~ 2007-03-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::CurrentStrategyPointDay()
{
	ATUM_DATE_TIME tmCurTime;
	tmCurTime.SetCurrentDateTime();
	tmCurTime.Hour = 0;
	tmCurTime.Minute = 0;
	tmCurTime.Second = 0;

	mtvectSSTRATEGYPOINT_DAY::iterator itr = m_mtvectStrategyPointDay.begin();
	while(itr != m_mtvectStrategyPointDay.end())
	{
		ATUM_DATE_TIME tmTime;
		tmTime.SetDateTime(itr->Year, itr->Month, itr->Day, 0, 0, 0);

		if(0 > tmTime.GetTimeDiffTimeInSeconds(tmCurTime))
		{// 2007-03-08 by dhjin, ���� �ð����� ���� �����̸� �����Ѵ�.
			itr = m_mtvectStrategyPointDay.erase(itr);
			continue;
		}
		itr++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::IncreaseStrategyPointDay(ATUM_DATE_TIME *i_pTime)
/// \brief		��������Ʈ�� ������ ��¥�� ��ȯ count���� 1 ������Ų��.
/// \author		dhjin
/// \date		2007-03-12 ~ 2007-03-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::IncreaseStrategyPointDay(ATUM_DATE_TIME *i_pTime)
{
	ATUM_DATE_TIME tmITime;
	tmITime.SetDateTime(i_pTime->Year, i_pTime->Month, i_pTime->Day, 0, 0, 0);

	mtvectSSTRATEGYPOINT_DAY::iterator itr = m_mtvectStrategyPointDay.begin();
	while(itr != m_mtvectStrategyPointDay.end())
	{
		ATUM_DATE_TIME tmTime;
		tmTime.SetDateTime(itr->Year, itr->Month, itr->Day, 0, 0, 0);
		if(0 == tmITime.GetTimeDiffTimeInHours(tmTime))
		{
			itr->MaxCount += 1;
			return;
		}
		itr++;
	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CompareStrategyPointDayByOneDay(ATUM_DATE_TIME *i_pTime, int *o_nRangeTime)
/// \brief		��������Ʈ�� ���� �������� �� ��¥�� �Է¹޾� üũ�Ͽ� �����Ѵ�.
/// \author		dhjin
/// \date		2007-03-12 ~ 2007-03-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CompareStrategyPointDayByOneDay(ATUM_DATE_TIME *i_pTime, int *o_nRangeTime)
{
	*o_nRangeTime = 1;
	mtvectSSTRATEGYPOINT_DAY::iterator itr = m_mtvectStrategyPointDay.begin();
	while(itr != m_mtvectStrategyPointDay.end())
	{
		// 2009-10-26 by cmkwon, ���� �ε��� �α� �߰� - 
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CInflWarManager::CompareStrategyPointDayByOneDay# 10000 itr(%d-%d-%d) MaxCount(%d) i_ptime(%s) RangeTime(%d)\r\n"
			, itr->Year, itr->Month, itr->Day, itr->MaxCount, i_pTime->GetDateTimeString().GetBuffer(), *o_nRangeTime);

		ATUM_DATE_TIME tmTime;
		tmTime.SetDateTime(itr->Year, itr->Month, itr->Day, 0, 0, 0);
		if(0 == i_pTime->GetTimeDiffTimeInHours(tmTime))
		{
			if(m_StrategyPointSummonRange.MaxSummonCountDaily > itr->MaxCount)
			{
				return TRUE;
			}
			return FALSE;
		}
		(*o_nRangeTime)++;
		itr++;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckSummonBossAndStrategyPoint()
/// \brief		
/// \author		dhjin
/// \date		2007-04-26 ~ 2007-04-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckSummonBossAndStrategyPoint()
{
	if(TRUE == m_SummonVCNBossMonsterInfo.bSummonBossMonster
		|| TRUE == m_SummonANIBossMonsterInfo.bSummonBossMonster
		|| FALSE == m_mtvectSummonStrategyPointInfo.empty())
	{
		return TRUE;
	}
	
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckSummonBoss()
/// \brief		���� ���� ǥ�� ��ȹ�� - 
/// \author		dhjin
/// \date		2008-03-28 ~ 2008-03-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckSummonBoss() 
{
	if(TRUE == m_SummonVCNBossMonsterInfo.bSummonBossMonster
		|| TRUE == m_SummonANIBossMonsterInfo.bSummonBossMonster)
	{
		return TRUE;
	}
	
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckSummonOnlyStrategyPoint()
/// \brief		���� ���� �߰���
/// \author		dhjin
/// \date		2008-12-24 ~ 2008-12-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckSummonOnlyStrategyPoint()
{
	if(FALSE == m_SummonVCNBossMonsterInfo.bSummonBossMonster
		&& FALSE == m_SummonANIBossMonsterInfo.bSummonBossMonster
		&& FALSE == m_mtvectSummonStrategyPointInfo.empty())
	{
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckBossMonsterSummoningByAttackInfl(byte i_byAttackerInfluence)
/// \brief		�������� �������� ��� ���� ������ ��ȯ �Ǿ� �ִ��� üũ
/// \author		dhjin
/// \date		2007-09-05 ~ 2007-09-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckBossMonsterSummoningByAttackInfl(byte i_byAttackerInfluence)
{
	if(INFLUENCE_TYPE_VCN == i_byAttackerInfluence)
	{
		return this->CheckANIBossMonsterSummoning();
	}

	if(INFLUENCE_TYPE_ANI == i_byAttackerInfluence)
	{
		return this->CheckVCNBossMonsterSummoning();
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckBossMonsterSummoningByMapInfluence(INT i_nMapInfluenceType)
/// \brief		���� ���� ǥ�� ��ȹ�� - �ʼ������� ���� ��ȯ �Ǿ� �ִ��� üũ
/// \author		dhjin
/// \date		2008-03-27 ~ 2008-03-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckBossMonsterSummoningByMapInfluence(INT i_nMapInfluenceType)
{
	if(IS_MAP_INFLUENCE_VCN(i_nMapInfluenceType))
	{
		return this->CheckVCNBossMonsterSummoning();
	}

	if(IS_MAP_INFLUENCE_ANI(i_nMapInfluenceType))
	{
		return this->CheckANIBossMonsterSummoning();
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckVCNBossMonsterSummoning()
/// \brief		���������� ���� ��ȯ �Ǿ� �ִ��� üũ
/// \author		dhjin
/// \date		2007-09-05 ~ 2007-09-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckVCNBossMonsterSummoning()
{
	if(m_SummonVCNBossMonsterInfo.bSummonBossMonster)
	{
		return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckANIBossMonsterSummoning()
/// \brief		�˸��� ���� ��ȯ �Ǿ� �ִ��� üũ
/// \author		dhjin
/// \date		2007-09-05 ~ 2007-09-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckANIBossMonsterSummoning()
{
	if(m_SummonANIBossMonsterInfo.bSummonBossMonster)
	{
		return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::GameStartSetLeader_SubLeaderGuildUID()
/// \brief		������, �������� �������� ���� UID ����  
/// \author		dhjin
/// \date		2007-09-07 ~ 2007-10-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::GameStartSetLeader_SubLeaderGuildUID()
{
	CInflWarData *pInflWarData = GetInflWarDataByInflType(INFLUENCE_TYPE_VCN);
	if(NULL == pInflWarData)
	{
		return;
	}
	// 2007-10-06 by dhjin, ���������� ������ ���� ���� 
	m_VCNLeaderGuildUID = m_pFieldIOCP14->m_GuildWarManager.GetLeaderGuildUID(pInflWarData->InflLeaderCharacterUID);
	// 2007-10-06 by dhjin, ���������� ��������1 ���� ����
	if(FALSE == m_pFieldIOCP14->m_GuildWarManager.CheckGuildCommander(pInflWarData->InflSub1LeaderCharacterUID, &m_VCNSubLeader1GuildUID))
	{
		m_VCNSubLeader1GuildUID = 0;
	}
	// 2007-10-06 by dhjin, ���������� ��������2 ���� ����
	if(FALSE == m_pFieldIOCP14->m_GuildWarManager.CheckGuildCommander(pInflWarData->InflSub2LeaderCharacterUID, &m_VCNSubLeader2GuildUID))
	{
		m_VCNSubLeader2GuildUID = 0;
	}

	pInflWarData = GetInflWarDataByInflType(INFLUENCE_TYPE_ANI);
	if(NULL == pInflWarData)
	{
		return;
	}
	// 2007-10-06 by dhjin, �˸��� ������ ���� ���� 
	m_ANILeaderGuildUID = m_pFieldIOCP14->m_GuildWarManager.GetLeaderGuildUID(pInflWarData->InflLeaderCharacterUID);
	// 2007-10-06 by dhjin, �˸��� ��������1 ���� ����
	if(FALSE == m_pFieldIOCP14->m_GuildWarManager.CheckGuildCommander(pInflWarData->InflSub1LeaderCharacterUID, &m_ANISubLeader1GuildUID))
	{
		m_ANISubLeader1GuildUID = 0;
	}
	// 2007-10-06 by dhjin, �˸��� ��������2 ���� ����
	if(FALSE == m_pFieldIOCP14->m_GuildWarManager.CheckGuildCommander(pInflWarData->InflSub1LeaderCharacterUID, &m_ANISubLeader2GuildUID))
	{
		m_ANISubLeader2GuildUID = 0;
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			UID32_t CInflWarManager::GetVCNLeaderGuildUID()
/// \brief		���������� ������ ���� UID
/// \author		dhjin
/// \date		2007-09-07 ~ 2007-09-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
UID32_t CInflWarManager::GetVCNLeaderGuildUID()
{
	return m_VCNLeaderGuildUID;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			UID32_t CInflWarManager::GetANILeaderGuildUID()
/// \brief		�˸��� ������ ���� UID
/// \author		dhjin
/// \date		2007-09-07 ~ 2007-09-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
UID32_t CInflWarManager::GetANILeaderGuildUID()
{
	return m_ANILeaderGuildUID;
}

UID32_t CInflWarManager::GetVCNSubLeader1GuildUID()
{
	return m_VCNSubLeader1GuildUID;
}

UID32_t CInflWarManager::GetVCNSubLeader2GuildUID()
{
	return m_VCNSubLeader2GuildUID;
}

UID32_t CInflWarManager::GetANISubLeader1GuildUID()
{
	return m_ANISubLeader1GuildUID;
}

UID32_t CInflWarManager::GetANISubLeader2GuildUID()
{
	return m_ANISubLeader2GuildUID;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SetSubLeaderGuildUID(int i_nSubLeaderRank, byte i_byInfluence, UID32_t i_nCharacterUID)
/// \brief		�������� �������� ���� UID ����
/// \author		dhjin
/// \date		2007-10-06 ~ 2007-10-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SetSubLeaderGuildUID(int i_nSubLeaderRank, byte i_byInfluence, UID32_t i_nCharacterUID)
{
	UID32_t tmSubLeaderGuildUID = 0;
	if(FALSE == m_pFieldIOCP14->m_GuildWarManager.CheckGuildCommander(i_nCharacterUID, &tmSubLeaderGuildUID))
	{// 2007-10-06 by dhjin, ���������� üũ�Ѵ�.
		return;
	}	

	if(1 == i_nSubLeaderRank
		&& INFLUENCE_TYPE_VCN == i_byInfluence)
	{
		m_VCNSubLeader1GuildUID = tmSubLeaderGuildUID;
	}
	else if(2 == i_nSubLeaderRank
		&& INFLUENCE_TYPE_VCN == i_byInfluence)
	{
		m_VCNSubLeader2GuildUID = tmSubLeaderGuildUID;
	}
	else if(1 == i_nSubLeaderRank
		&& INFLUENCE_TYPE_ANI == i_byInfluence)
	{
		m_ANISubLeader1GuildUID = tmSubLeaderGuildUID;
	}
	else if(2 == i_nSubLeaderRank
		&& INFLUENCE_TYPE_ANI == i_byInfluence)
	{
		m_ANISubLeader2GuildUID = tmSubLeaderGuildUID;
	}	
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckLeader_SubLeaderGuildUID(UID32_t i_nGuildUID)
/// \brief		������ �������� üũ
/// \author		dhjin
/// \date		2007-09-07 ~ 2007-09-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckLeader_SubLeaderGuildUID(UID32_t i_nGuildUID)
{
	if(i_nGuildUID == GetVCNLeaderGuildUID()
		|| i_nGuildUID == GetANILeaderGuildUID()
		|| i_nGuildUID == GetVCNSubLeader1GuildUID()
		|| i_nGuildUID == GetVCNSubLeader2GuildUID()
		|| i_nGuildUID == GetANISubLeader1GuildUID()
		|| i_nGuildUID == GetANISubLeader2GuildUID())
	{
		return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::InsertTelePortInfoBuing(STELEPORT_INFO_BUILDING * i_pTeleportInfoBuilding)
/// \brief		�ڷ���Ʈ�� ���� ���������� �����ϸ� ������ �����Ͽ� ���� �ð��� üũ�Ѵ�.
/// \author		dhjin
/// \date		2007-09-15 ~ 2007-09-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::InsertTelePortInfoBuing(STELEPORT_INFO_BUILDING * i_pTeleportInfoBuilding)
{
	mt_auto_lock mta(&m_mtvectTeleportInfoBuilding);

	for(int i=0; i < m_mtvectTeleportInfoBuilding.size(); i++)
	{
		if(m_mtvectTeleportInfoBuilding[i].MapIndex == i_pTeleportInfoBuilding->MapIndex)
		{
			return FALSE;
		}
	}

	m_mtvectTeleportInfoBuilding.push_back(*i_pTeleportInfoBuilding);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::TelePortInfoBuingByTick(ATUM_DATE_TIME * i_pCurrnetTime)
/// \brief		�ڷ���Ʈ ���� 1�и��� üũ�Ͽ� ���� ����
/// \author		dhjin
/// \date		2007-09-15 ~ 2007-09-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::TelePortInfoBuingByTick(ATUM_DATE_TIME * i_pCurrnetTime)
{
	mt_auto_lock mta(&m_mtvectTeleportInfoBuilding);

	mtvectSTELEPORT_INFO_BUILDING::iterator itr = m_mtvectTeleportInfoBuilding.begin();
	while(itr != m_mtvectTeleportInfoBuilding.end())
	{
		if(itr->bBuilded)
		{
			itr++;
			continue;
		}

		if(TELEPORT_BUILDING_TIME_SECOND <= i_pCurrnetTime->GetTimeDiffTimeInSeconds(itr->BuildStartTime))
		{// 2007-09-15 by dhjin, ������ �ϼ��Ǿ���. ������ ������ �����Ѵ�. (�ڷ���Ʈ ��ȯ�� �׻� �ϳ��� �����Ѵ�.)
			CFieldMapChannel *pFMChann = m_pFieldIOCP14->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(itr->MapIndex, 0));
			if(NULL == pFMChann)
			{
				itr = m_mtvectTeleportInfoBuilding.erase(itr);
				continue;
			}

			// 2007-09-20 by cmkwon, �ڷ���Ʈ ��ȯ ���� ����
			if(TELEPORT_STATE_BUILDING != pFMChann->GetTelePortState())
			{
				itr = m_mtvectTeleportInfoBuilding.erase(itr);
				continue;
			}

			// 2007-09-16 by dhjin, ���� BODYCON�� ����
			CFieldMonster	*pFMon = pFMChann->GetFieldMonster(pFMChann->m_ChangeBodyConMonsterIndex, 0);
			if(NULL == pFMon
				|| FALSE == pFMon->IsValidMonster()
				|| FALSE == IS_TELEPORT_MONSTER(pFMon->MonsterInfoPtr->Belligerence))
			{
				itr = m_mtvectTeleportInfoBuilding.erase(itr);
				continue;
			}

			MapIndex_t BeforeTeleportMapIdx = 0;
			if(IS_MAP_INFLUENCE_VCN(itr->MapInfluence))
			{
				BeforeTeleportMapIdx	= m_ANITeleportMapIndex;
				m_ANITeleportMapIndex	= itr->MapIndex;		// 2007-09-16 by dhjin, ���� �Ϸ� �� ��ȣ
			}
			else
			{
				BeforeTeleportMapIdx	= m_VCNTeleportMapIndex;
				m_VCNTeleportMapIndex	= itr->MapIndex;		// 2007-09-16 by dhjin, ���� �Ϸ� �� ��ȣ
			}

			if(0 != BeforeTeleportMapIdx)
			{
				///////////////////////////////////////////////////////////////////////////////
				// 2007-09-20 by cmkwon, �ڷ���Ʈ ���� - ���� �ڷ���Ʈ ó��
				CFieldMapChannel *pBeforeTeleportFMChann = m_pFieldIOCP14->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(BeforeTeleportMapIdx, 0));
				if(pBeforeTeleportFMChann)
				{
					CFieldMonster	*pBeforeTeleportFMon = pBeforeTeleportFMChann->GetFieldMonster(pBeforeTeleportFMChann->m_ChangeBodyConMonsterIndex, 0);
					if(pBeforeTeleportFMon
						&& pBeforeTeleportFMon->IsValidMonster()
						&& IS_TELEPORT_MONSTER(pBeforeTeleportFMon->MonsterInfoPtr->Belligerence))
					{
						CLEAR_BODYCON_BIT(pBeforeTeleportFMon->BodyCondition, BODYCON_MON_BUILD_MASK);		// 2007-09-20 by cmkwon, �ڷ���Ʈ ���� - 
						pBeforeTeleportFMChann->FieldSendMonsterChangeInfo(pBeforeTeleportFMon, T_FN_MONSTER_CHANGE_BODYCONDITION);
						pBeforeTeleportFMChann->FieldSendMonsterChangeInfo(pBeforeTeleportFMon, T_FC_MONSTER_CHANGE_BODYCONDITION);

						pBeforeTeleportFMChann->SetTelePortState(TELEPORT_STATE_READY);

						//////////////////////////////////////////////////////////////////////////
						// 2008-03-28 by dhjin, ���� ���� ǥ�� ��ȹ�� - 
						this->SetMSWarInfo(pBeforeTeleportFMChann->GetMapChannelIndex().MapIndex, T_MSWARINFO_MODIFY_CREATE_TELEPORT, MS_WAR_HP_RATE, TELEPORT_STATE_READY, pBeforeTeleportFMon->MonsterInfoPtr->Belligerence);
					}
					else
					{
						pBeforeTeleportFMChann->SetTelePortState(TELEPORT_STATE_NONE);
					}

					INIT_MSG_WITH_BUFFER(MSG_FC_EVENT_TELEPORT_DESTROY, T_FC_EVENT_TELEPORT_DESTROY, pSTeleport, SendBuf);
					pSTeleport->MapIndex					= BeforeTeleportMapIdx;
					pSTeleport->bInactivateByOtherTeleport	= TRUE;
					m_pFieldIOCP14->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_EVENT_TELEPORT_DESTROY),INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);
				}
			}

			pFMChann->SetTelePortState(TELEPORT_STATE_COMPLETE);
			itr->bBuilded	= TRUE;								// 2007-09-16 by dhjin, ���� �Ϸ� �÷��� 

			//////////////////////////////////////////////////////////////////////////
			// 2008-03-28 by dhjin, ���� ���� ǥ�� ��ȹ�� -
			// 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.)
			//this->SetMSWarInfo(itr->MapIndex, T_MSWARINFO_MODIFY_CHANGE_TELEPORT_STATE, CalcGradeByRate(pFMon->MonsterInfoPtr->MonsterHP, pFMon->CurrentHP, MS_WAR_HP_RATE), TELEPORT_STATE_COMPLETE, pFMon->MonsterInfoPtr->Belligerence);
			this->SetMSWarInfo(itr->MapIndex, T_MSWARINFO_MODIFY_CHANGE_TELEPORT_STATE, CalcGradeByRate(pFMon->MonsterInfoExtend.fMaxHP, pFMon->CurrentHP, MS_WAR_HP_RATE), TELEPORT_STATE_COMPLETE, pFMon->MonsterInfoPtr->Belligerence);
			// End 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.)

			CLEAR_BODYCON_BIT(pFMon->BodyCondition, BODYCON_MON_BUILDING);			// 2007-09-20 by cmkwon, �ڷ���Ʈ ���� - 
			SET_BODYCON_BIT(pFMon->BodyCondition,  BODYCON_MON_BUILDED);
			pFMChann->FieldSendMonsterChangeInfo(pFMon, T_FN_MONSTER_CHANGE_BODYCONDITION);
			pFMChann->FieldSendMonsterChangeInfo(pFMon, T_FC_MONSTER_CHANGE_BODYCONDITION);

			INIT_MSG_WITH_BUFFER(MSG_FC_EVENT_TELEPORT_BUILDCOMPLETE, T_FC_EVENT_TELEPORT_BUILDCOMPLETE, pSTeleport, SendBuf);
			pSTeleport->MapIndex			= itr->MapIndex;
			m_pFieldIOCP14->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_EVENT_TELEPORT_BUILDCOMPLETE),INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);
		}
		itr++;
	}

	// 2007-09-16 by dhjin, �ڷ���Ʈ ���� ������ �κ��� ������ �����Ѵ�.
	itr = m_mtvectTeleportInfoBuilding.begin();
	while(itr != m_mtvectTeleportInfoBuilding.end())
	{
		if(FALSE == itr->bBuilded)
		{
			itr++;
			continue;
		}

		if( (IS_MAP_INFLUENCE_VCN(itr->MapInfluence) && m_ANITeleportMapIndex != itr->MapIndex)
				|| (IS_MAP_INFLUENCE_ANI(itr->MapInfluence) && m_VCNTeleportMapIndex != itr->MapIndex) )
		{// 2007-09-16 by dhjin, �̹� ��ġ�� �ڷ���Ʈ ���Ͱ� ���� ����̸� �����Ѵ�.
			
			itr = m_mtvectTeleportInfoBuilding.erase(itr);
			continue;
		}		
		itr++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::ClearTelePortByInfluenceWarEnd(BYTE i_byDefenceCharInfluence)
/// \brief		������ ����� �ڷ���Ʈ ������ �ʱ�ȭ�Ѵ�. 
///				// 2009-03-25 by cmkwon, ������ �ڷ���Ʈ �ʱ�ȭ ���� ���� ���� - ������ i_byDefenceCharInfluence�� ����
/// \author		dhjin
/// \date		2007-09-16 ~ 2007-09-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::ClearTelePortByInfluenceWarEnd(BYTE i_byDefenceCharInfluence)
{
	mt_auto_lock mta(&m_mtvectTeleportInfoBuilding);

// 2009-03-25 by cmkwon, ������ �ڷ���Ʈ �ʱ�ȭ ���� ���� ���� - �Ʒ��� ���� ���� ��
// 	if(IS_VCN_INFLUENCE_TYPE(i_byCharInfluence))
// 	{
// 		m_VCNTeleportMapIndex	= 0;
// 	}
// 	if(IS_ANI_INFLUENCE_TYPE(i_byCharInfluence))
// 	{
// 		m_ANITeleportMapIndex	= 0;
// 	}
// 
// 	mtvectSTELEPORT_INFO_BUILDING::iterator itr = m_mtvectTeleportInfoBuilding.begin();
// 	while(itr != m_mtvectTeleportInfoBuilding.end())
// 	{	
// 		if(IS_SAME_CHARACTER_8_MAP_INFLUENCE(i_byCharInfluence, itr->MapInfluence))
// 		{
// 			CFieldMapChannel *pFMChann = m_pFieldIOCP14->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(itr->MapIndex, 0));
// 			if(NULL == pFMChann)
// 			{
// 				itr = m_mtvectTeleportInfoBuilding.erase(itr);
// 				continue;
// 			}						
// 
// 			pFMChann->SetTelePortState(TELEPORT_STATE_NOT_SUMMON);
// 			pFMChann->SetTelePortSummonFirst(FALSE);
// 			pFMChann->m_ChangeBodyConMonsterIndex = 0;
// 		}		
// 		itr++;
// 	}
// 
// 	if(IS_VCN_INFLUENCE_TYPE(i_byCharInfluence))
// 	{
// 		m_VCNTeleportMapIndex	= 0;
// 	}
// 	if(IS_ANI_INFLUENCE_TYPE(i_byCharInfluence))
// 	{
// 		m_ANITeleportMapIndex	= 0;
// 	}
	///////////////////////////////////////////////////////////////////////////////
	// 2009-03-25 by cmkwon, ������ �ڷ���Ʈ �ʱ�ȭ ���� ���� ���� - 
	if(IS_VCN_INFLUENCE_TYPE(i_byDefenceCharInfluence))
	{
 		m_ANITeleportMapIndex	= 0;
	}
	else
	{
		m_VCNTeleportMapIndex	= 0;
	}
	mta.auto_unlock_cancel();

	///////////////////////////////////////////////////////////////////////////////	
    // 2009-03-25 by cmkwon, ������ �ڷ���Ʈ �ʱ�ȭ ���� ���� ���� - �Ʒ��� ���� �ش� ��� ������ ��ä�ο� �ڷ���Ʈ ���� ������ �ʱ�ȭ �Ѵ�.
	mt_auto_lock mtSP(this->GetStrategyPointSummonTimeInfo());
	mtvectSSTRATEGYPOINT_SUMMONTIME_INFO::iterator itrSP = m_mtvectStrategyPointSummonTimeInfo.begin();
	for(; itrSP != m_mtvectStrategyPointSummonTimeInfo.end(); itrSP++)
	{
		CFieldMapChannel *pFMChann = m_pFieldIOCP14->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(itrSP->MapIndex, 0));
		if(pFMChann
// 2009-10-12 by cmkwon, ������ī ���� ��� ���� - CInflWarManager::ClearTelePortByInfluenceWarEnd#
//			&& IS_SAME_CHARACTER_8_MAP_INFLUENCE(i_byDefenceCharInfluence, pFMChann->GetMapInfluenceTypeW())
			&& CAtumSJ::IsSameCharacterInfluence8MapInfluence(i_byDefenceCharInfluence, pFMChann->GetMapInfluenceTypeW())	// 2009-10-12 by cmkwon, ������ī ���� ��� ���� -  
			)
		{
			pFMChann->SetTelePortState(TELEPORT_STATE_NOT_SUMMON);
			pFMChann->SetTelePortSummonFirst(FALSE);
			pFMChann->m_ChangeBodyConMonsterIndex = 0;
		}
	}
	mtSP.auto_unlock_cancel();

	//////////////////////////////////////////////////////////////////////////
	// 2008-03-28 by dhjin, ���� ���� ǥ�� ��ȹ�� - ���� ���� �� �����Ѵ�.
	this->m_MSWarInfoDisPlay.MSWarInfoDisPlayClear(i_byDefenceCharInfluence);		// 2009-03-25 by cmkwon, ������ �ڷ���Ʈ �ʱ�ȭ ���� ���� ���� - 
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::DeleteTelePortInfoBuing(MapIndex_t i_mapIdx)
/// \brief		// 2007-09-20 by cmkwon, �ڷ���Ʈ ��ȯ ���� ���� - 
/// \author		cmkwon
/// \date		2007-09-20 ~ 2007-09-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::DeleteTelePortInfoBuing(MapIndex_t i_mapIdx)
{
	mt_auto_lock mta(&m_mtvectTeleportInfoBuilding);

	if(m_VCNTeleportMapIndex == i_mapIdx)
	{
		m_VCNTeleportMapIndex = 0;
	}
	if(m_ANITeleportMapIndex == i_mapIdx)
	{
		m_ANITeleportMapIndex = 0;
	}

	mtvectSTELEPORT_INFO_BUILDING::iterator itr = m_mtvectTeleportInfoBuilding.begin();
	while(itr != m_mtvectTeleportInfoBuilding.end())
	{
		if(itr->MapIndex == i_mapIdx)
		{
			itr = m_mtvectTeleportInfoBuilding.erase(itr);
			continue;
		}
		itr++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			MapIndex_t CInflWarManager::GetTeleportMapIndex(BYTE i_byInfluence)
/// \brief		
/// \author		dhjin
/// \date		2007-09-16 ~ 2007-09-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
MapIndex_t CInflWarManager::GetTeleportMapIndex(BYTE i_byInfluence)
{
	if(IS_VCN_INFLUENCE_TYPE(i_byInfluence))
	{
		return m_VCNTeleportMapIndex;
	}
	else if(IS_ANI_INFLUENCE_TYPE(i_byInfluence))
	{
		return m_ANITeleportMapIndex;
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SetPollDate(SPOLLDATE_DB * i_pSPOLLDATE_DB)
/// \brief		DB���� �ε��� �� ����
/// \author		dhjin
/// \date		2007-10-29 ~ 2007-10-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SetPollDate(SPOLLDATE_DB * i_pSPOLLDATE_DB)
{
	if(NULL == i_pSPOLLDATE_DB)
	{
		return;
	}
	
	m_SPOLLDATE_DB.ApplicationStartDate		= i_pSPOLLDATE_DB->ApplicationStartDate;
	m_SPOLLDATE_DB.ApplicationEndDate		= i_pSPOLLDATE_DB->ApplicationEndDate;
	m_SPOLLDATE_DB.VoteStartDate			= i_pSPOLLDATE_DB->VoteStartDate;
	m_SPOLLDATE_DB.VoteEndDate				= i_pSPOLLDATE_DB->VoteEndDate;
	m_SPOLLDATE_DB.Election					= i_pSPOLLDATE_DB->Election;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::LoadVoterListByDB()
/// \brief		DB���� �ε� ��ǥ�� ����Ʈ 
/// \author		dhjin
/// \date		2007-10-29 ~ 2007-10-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::LoadVoterListByDB()
{
	this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_LoadVoterList, NULL, 0, NULL);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::LoadLeaderCandidateByDB()
/// \brief		DB���� �ε� ������ �ĺ� ����Ʈ
/// \author		dhjin
/// \date		2007-10-29 ~ 2007-10-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::LoadLeaderCandidateByDB()
{
	this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_LoadLeaderCandidate, NULL, 0, NULL);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::InitVoterListByDB(mtvectSVOTER_LIST * i_pvectSVOTER_LIST)
/// \brief		DB���� �ε��� ��ǥ�� ����Ʈ ���� 
/// \author		dhjin
/// \date		2007-10-29 ~ 2007-10-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::InitVoterListByDB(mtvectSVOTER_LIST * i_pvectSVOTER_LIST)
{
	mt_auto_lock mta(&m_mtvectSVOTER_LIST);
	
	m_mtvectSVOTER_LIST.clear();

	m_mtvectSVOTER_LIST.assign(i_pvectSVOTER_LIST->begin(), i_pvectSVOTER_LIST->end());

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::InitLeaderCandidateByDB(mtvectSLEADER_CANDIDATE * i_pvectSLEADER_CANDIDATE)
/// \brief		DB���� �ε��� ������ �ĺ� ����Ʈ ���� 
/// \author		dhjin
/// \date		2007-10-29 ~ 2007-10-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::InitLeaderCandidateByDB(mtvectSLEADER_CANDIDATE * i_pvectSLEADER_CANDIDATE)
{
	mt_auto_lock mta(&m_mtvectSLEADER_CANDIDATE);
	
	m_mtvectSLEADER_CANDIDATE.clear();

	m_mtvectSLEADER_CANDIDATE.assign(i_pvectSLEADER_CANDIDATE->begin(), i_pvectSLEADER_CANDIDATE->end());	

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::GetLeaderCandidateInfo(INT i_nLeaderCandidateNum, BYTE i_byInfluence, SLEADER_CANDIDATE *o_pSLEADER_CANDIDATE)
/// \brief		���õ� ������ �ĺ� ���� �� ��������
/// \author		dhjin
/// \date		2007-10-30 ~ 2007-10-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::GetLeaderCandidateInfo(INT i_nLeaderCandidateNum, BYTE i_byInfluence, SLEADER_CANDIDATE *o_pSLEADER_CANDIDATE)
{
	mt_auto_lock mta(&m_mtvectSLEADER_CANDIDATE);

	mtvectSLEADER_CANDIDATE::iterator itr = m_mtvectSLEADER_CANDIDATE.begin();
	for(; itr != m_mtvectSLEADER_CANDIDATE.end(); itr++)
	{
		if(i_nLeaderCandidateNum == itr->LeaderCandidateNum
			&& i_byInfluence == itr->Influence)
		{
			o_pSLEADER_CANDIDATE->AccountUID		= itr->AccountUID;
			util::strncpy(o_pSLEADER_CANDIDATE->CampaignPromises, itr->CampaignPromises, SIZE_MAX_CAMPAIGNPROMISES);
			util::strncpy(o_pSLEADER_CANDIDATE->CharacterName, itr->CharacterName, SIZE_MAX_CHARACTER_NAME);
			o_pSLEADER_CANDIDATE->CharacterUID		= itr->CharacterUID;
			o_pSLEADER_CANDIDATE->DeleteCandidate	= itr->DeleteCandidate;
			util::strncpy(o_pSLEADER_CANDIDATE->GuildName, itr->GuildName, SIZE_MAX_GUILD_NAME);
			o_pSLEADER_CANDIDATE->GuildUID			= itr->GuildUID;
			o_pSLEADER_CANDIDATE->Influence			= itr->Influence;
			o_pSLEADER_CANDIDATE->LeaderCandidateNum= itr->LeaderCandidateNum;
			o_pSLEADER_CANDIDATE->PollCount			= itr->PollCount;
			return TRUE;
		}
	}

	return FALSE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckLeaderCandidateUser(UID32_t i_nAccountUID, UID32_t i_nCharacterUID)
/// \brief		������ �ĺ��� ��� �Ǿ� �ִ� �������� üũ
/// \author		dhjin
/// \date		2007-10-29 ~ 2007-10-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckLeaderCandidateUser(UID32_t i_nAccountUID, UID32_t i_nCharacterUID)
{
	mt_auto_lock mta(&m_mtvectSLEADER_CANDIDATE);

	mtvectSLEADER_CANDIDATE::iterator itr = m_mtvectSLEADER_CANDIDATE.begin();
	for(; itr != m_mtvectSLEADER_CANDIDATE.end(); itr++)
	{
		if(i_nAccountUID == itr->AccountUID
			&& i_nCharacterUID == itr->CharacterUID)
		{
			return TRUE;
		}
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SendLeaderCandidateList(CFieldIOCPSocket *i_pFISoc)
/// \brief		������ �ĺ� ����Ʈ ����
/// \author		dhjin
/// \date		2007-10-30 ~ 2007-10-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SendLeaderCandidateList(CFieldIOCPSocket *i_pFISoc)
{

	BOOL	CheckLeaderCandidateUser	= FALSE;		// 2007-10-30 by dhjin, �ĺ� ����� �������� üũ ����
	
	// 2007-10-29 by dhjin, �ĺ� ��ϵ� �������� üũ
	if(this->CheckLeaderCandidateUser(i_pFISoc->m_character.AccountUniqueNumber, i_pFISoc->m_character.CharacterUniqueNumber))
	{// 2007-10-30 by dhjin, �ĺ� ����� �����̸� TRUE�� 
		CheckLeaderCandidateUser = TRUE;
	}

	// 2007-10-30 by dhjin, ����
	INT		nSendBytes = 0;
	INIT_MSG_WITH_BUFFER(MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK_HEADER, T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK_HEADER, pSendMsgHeader, SendBuf);
	pSendMsgHeader->RegLeaderCandidate		= CheckLeaderCandidateUser;
	nSendBytes = MSG_SIZE(MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK_HEADER);		
	
	mt_auto_lock mta(&m_mtvectSLEADER_CANDIDATE);

	mtvectSLEADER_CANDIDATE::iterator itr = m_mtvectSLEADER_CANDIDATE.begin();
	for(; itr != m_mtvectSLEADER_CANDIDATE.end(); itr++)
	{
		if(i_pFISoc->m_character.InfluenceType != itr->Influence
			|| TRUE == itr->DeleteCandidate)
		{// 2007-11-06 by dhjin, �ٸ� �����̰ų� ������ �ĺ��̸� ����Ѵ�.
			continue;
		}

		if(nSendBytes + MSG_SIZE(MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK) > SIZE_MAX_PACKET)
		{
			i_pFISoc->SendAddData(SendBuf, nSendBytes);
			nSendBytes	= 0;
		}

		*((MessageType_t*)(SendBuf + nSendBytes))					= T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK;		
		nSendBytes													+= SIZE_FIELD_TYPE_HEADER;
		MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK *pSendMsg	= (MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK*)(SendBuf + nSendBytes);
		pSendMsg->LeaderCandidateNum								= itr->LeaderCandidateNum;
		pSendMsg->CharacterUID										= itr->CharacterUID;
		pSendMsg->GuildUID											= itr->GuildUID;
		pSendMsg->DeleteCandidate									= itr->DeleteCandidate;
		util::strncpy(pSendMsg->CharacterName, itr->CharacterName, SIZE_MAX_CHARACTER_NAME);
		util::strncpy(pSendMsg->GuildName, itr->GuildName, SIZE_MAX_CHARACTER_NAME);
		nSendBytes									+= sizeof(MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK);
	}

	if(nSendBytes > 0)
	{
		i_pFISoc->SendAddData(SendBuf, nSendBytes);
		nSendBytes	= 0;
	}
	
	mta.auto_unlock_cancel();

	i_pFISoc->SendAddMessageType(T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK_DONE);
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckPollDateByApplication(ATUM_DATE_TIME * i_pCurrentTime)
/// \brief		������ �ĺ� ��� �Ⱓ���� üũ
/// \author		dhjin
/// \date		2007-10-30 ~ 2007-10-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckPollDateByApplication(ATUM_DATE_TIME * i_pCurrentTime)
{
	if(m_SPOLLDATE_DB.ApplicationStartDate <= *i_pCurrentTime
		&& m_SPOLLDATE_DB.ApplicationEndDate >= *i_pCurrentTime)
	{
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckPollDateByVote(ATUM_DATE_TIME * i_pCurrentTime)
/// \brief		��ǥ �Ⱓ���� üũ
/// \author		dhjin
/// \date		2007-10-30 ~ 2007-10-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckPollDateByVote(ATUM_DATE_TIME * i_pCurrentTime)
{
	if(m_SPOLLDATE_DB.VoteStartDate <= *i_pCurrentTime
		&& m_SPOLLDATE_DB.VoteEndDate >= *i_pCurrentTime)
	{
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckPollDateRange(ATUM_DATE_TIME * i_pCurrentTime)
/// \brief		���� �Ⱓ���� üũ
/// \author		dhjin
/// \date		2007-11-20 ~ 2007-11-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckPollDateRange(ATUM_DATE_TIME * i_pCurrentTime)
{
	if(m_SPOLLDATE_DB.ApplicationStartDate <= *i_pCurrentTime
		&& m_SPOLLDATE_DB.Election >= *i_pCurrentTime)
	{
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckLeaderelectionDate(ATUM_DATE_TIME * i_pCurrentTime)
/// \brief		������ ���� �ð� üũ
/// \author		dhjin
/// \date		2007-11-16 ~ 2007-11-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckLeaderelectionDate(ATUM_DATE_TIME * i_pCurrentTime)
{
	if(0 == m_SPOLLDATE_DB.Election.GetTimeDiffTimeInMinutes(*i_pCurrentTime))
	{
		return TRUE;
	}

	return FALSE;
}

#ifdef NEMERIAN_NATION_BALANCE_BUFF_PENALITY
BOOL CInflWarManager::ElectionDateIsInPast(ATUM_DATE_TIME * i_pCurrentTime)
{
	if (m_SPOLLDATE_DB.Election.GetTimeDiffTimeInMinutes(*i_pCurrentTime) > 0) {
		return TRUE;
	}

	return FALSE;
}
#endif

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SendLeaderelectionInfo()
/// \brief		������ ���� ���� ����
/// \author		dhjin
/// \date		2007-11-16 ~ 2007-11-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SendLeaderelectionInfo()
{
	INIT_MSG_WITH_BUFFER(MSG_FC_CITY_POLL_LEADER_ELECTION_INFO, T_FC_CITY_POLL_LEADER_ELECTION_INFO, pVCNSendMsg, VCNSendBuf);
	INIT_MSG_WITH_BUFFER(MSG_FC_CITY_POLL_LEADER_ELECTION_INFO, T_FC_CITY_POLL_LEADER_ELECTION_INFO, pANISendMsg, ANISendBuf);
	
	INT VCNPollCount = 0;			// ������ ��ǥ ��
	INT ANIPollCount = 0;
	INT TotalVCNPollCount = 0;
	INT TotalANIPollCount = 0;
	
	mt_auto_lock mta(&m_mtvectSLEADER_CANDIDATE);

	mtvectSLEADER_CANDIDATE::iterator itr = m_mtvectSLEADER_CANDIDATE.begin();
	for(; itr != m_mtvectSLEADER_CANDIDATE.end(); itr++)
	{
		if(COMPARE_INFLUENCE(itr->Influence, INFLUENCE_TYPE_VCN)
			&& FALSE == itr->DeleteCandidate)
		{
			if(VCNPollCount < itr->PollCount)
			{
				pVCNSendMsg->LeaderCandidateNum = itr->LeaderCandidateNum;
				util::strncpy(pVCNSendMsg->CharacterName, itr->CharacterName, SIZE_MAX_CHARACTER_NAME);
				util::strncpy(pVCNSendMsg->GuildName, itr->GuildName, SIZE_MAX_GUILD_NAME);
				VCNPollCount	= itr->PollCount;
			}
			TotalVCNPollCount	+= itr->PollCount;
		}
		else if(COMPARE_INFLUENCE(itr->Influence, INFLUENCE_TYPE_ANI)
				&& FALSE == itr->DeleteCandidate)
		{
			if(ANIPollCount < itr->PollCount)
			{
				pANISendMsg->LeaderCandidateNum = itr->LeaderCandidateNum;
				util::strncpy(pANISendMsg->CharacterName, itr->CharacterName, SIZE_MAX_CHARACTER_NAME);
				util::strncpy(pANISendMsg->GuildName, itr->GuildName, SIZE_MAX_GUILD_NAME);
				ANIPollCount	= itr->PollCount;
			}
			TotalANIPollCount	+= itr->PollCount;			
		}
	}
	mta.auto_unlock_cancel();

	if(0 != TotalVCNPollCount)
	{
		pVCNSendMsg->Percent	= (VCNPollCount * 100) / TotalVCNPollCount;
	}
	if(0 != TotalANIPollCount)
	{
		pANISendMsg->Percent	= (ANIPollCount * 100) / TotalANIPollCount;
	}

	m_pFieldIOCP14->SendMessageToAllClients(VCNSendBuf, MSG_SIZE(MSG_FC_CITY_POLL_LEADER_ELECTION_INFO), INFLUENCE_TYPE_VCN);
	m_pFieldIOCP14->SendMessageToAllClients(ANISendBuf, MSG_SIZE(MSG_FC_CITY_POLL_LEADER_ELECTION_INFO), INFLUENCE_TYPE_ANI);
	
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::InsertLeaderCandidate(SLEADER_CANDIDATE * i_pSLEADER_CANDIDATE)
/// \brief		������ �ĺ� ���
/// \author		dhjin
/// \date		2007-10-30 ~ 2007-10-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::InsertLeaderCandidate(SLEADER_CANDIDATE * i_pSLEADER_CANDIDATE)
{
	mt_auto_lock mta(&m_mtvectSLEADER_CANDIDATE);

	if(NULL != i_pSLEADER_CANDIDATE)
	{
		m_mtvectSLEADER_CANDIDATE.push_back(*i_pSLEADER_CANDIDATE);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::UpdateDeleteLeaderCandidate(INT i_nCharacterUID)
/// \brief		������ �ĺ� Ż��
/// \author		dhjin
/// \date		2007-10-30 ~ 2007-10-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::UpdateDeleteLeaderCandidate(INT i_nCharacterUID)
{
	mt_auto_lock mta(&m_mtvectSLEADER_CANDIDATE);

	mtvectSLEADER_CANDIDATE::iterator itr = m_mtvectSLEADER_CANDIDATE.begin();
	for(; itr != m_mtvectSLEADER_CANDIDATE.end(); itr++)
	{
		if(i_nCharacterUID == itr->CharacterUID)
		{
			itr->DeleteCandidate	= TRUE;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::DeleteLeaderCandidate(INT i_nCharacterUID)
/// \brief		������ �ĺ� ���� ����Ÿ �� ����, DB���� �������� ���� ����
/// \author		dhjin
/// \date		2008-06-05 ~ 2008-06-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::DeleteLeaderCandidate(INT i_nCharacterUID)
{
	mt_auto_lock mta(&m_mtvectSLEADER_CANDIDATE);

	mtvectSLEADER_CANDIDATE::iterator itr = m_mtvectSLEADER_CANDIDATE.begin();
	for(; itr != m_mtvectSLEADER_CANDIDATE.end(); itr++)
	{
		if(i_nCharacterUID == itr->CharacterUID)
		{
			itr = m_mtvectSLEADER_CANDIDATE.erase(itr);
			return;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckPossibleVoterUser(UID32_t i_nAccountUID)
/// \brief		��ǥ�� �������� üũ
/// \author		dhjin
/// \date		2007-10-31 ~ 2007-10-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckPossibleVoterUser(UID32_t i_nAccountUID)
{
	mt_auto_lock mta(&m_mtvectSVOTER_LIST);

	mtvectSVOTER_LIST::iterator itr = m_mtvectSVOTER_LIST.begin();
	for(; itr != m_mtvectSVOTER_LIST.end(); itr++)
	{
		if(i_nAccountUID == itr->AccountUID)
		{
			return FALSE;
		}
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::LeaderCandidateVote(INT i_nLeaderCandidateNum, BYTE i_byInfluence, INT i_nPollPoint)
/// \brief		������ �ĺ����� ��ǥ�� ���(��ǥ������)
/// \author		dhjin
/// \date		2007-10-31 ~ 2007-10-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::LeaderCandidateVote(INT i_nLeaderCandidateNum, BYTE i_byInfluence, INT i_nPollPoint)
{
	mt_auto_lock mta(&m_mtvectSLEADER_CANDIDATE);

	mtvectSLEADER_CANDIDATE::iterator itr = m_mtvectSLEADER_CANDIDATE.begin();
	for(; itr != m_mtvectSLEADER_CANDIDATE.end(); itr++)
	{
		if(i_nLeaderCandidateNum == itr->LeaderCandidateNum
			&& i_byInfluence	== itr->Influence
			&& FALSE == itr->DeleteCandidate)
		{
			itr->PollCount += i_nPollPoint;
			QPARAM_UPDATE_LEADERPOLLCOUNT *pQParam = new QPARAM_UPDATE_LEADERPOLLCOUNT;	
			pQParam->LeaderCandidateNum		= itr->LeaderCandidateNum;
			pQParam->Influence				= itr->Influence;
			pQParam->PollCount				= itr->PollCount;
			this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateLeaderPollCount, NULL, 0, pQParam);
			return TRUE;
		}
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::InsertVoterUser(SVOTER_LIST *i_pSVOTER_LIST)
/// \brief		��ǥ�� ���� �߰�
/// \author		dhjin
/// \date		2007-10-31 ~ 2007-10-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::InsertVoterUser(SVOTER_LIST *i_pSVOTER_LIST)
{
	mt_auto_lock mta(&m_mtvectSVOTER_LIST);

	if(NULL != i_pSVOTER_LIST)
	{
		QPARAM_INSERT_VOTER_LIST *pQParam = new QPARAM_INSERT_VOTER_LIST;
		pQParam->AccountUID			= i_pSVOTER_LIST->AccountUID;
		pQParam->CharacterUID		= i_pSVOTER_LIST->CharacterUID;
		pQParam->LeaderCandidateNum	= i_pSVOTER_LIST->LeaderCandidateNum;
		this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InsertVoterList, NULL, 0, pQParam);
		m_mtvectSVOTER_LIST.push_back(*i_pSVOTER_LIST);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SendPollDate(CFieldIOCPSocket *i_pFISoc)
/// \brief		���� ���� ����
/// \author		dhjin
/// \date		2007-10-31 ~ 2007-10-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SendPollDate(CFieldIOCPSocket *i_pFISoc)
{
	INIT_MSG_WITH_BUFFER(MSG_FC_CITY_POLL_REQUEST_POLL_DATE_OK, T_FC_CITY_POLL_REQUEST_POLL_DATE_OK, pSendMsg, SendBuf);
	pSendMsg->ApplicationStartDate		= m_SPOLLDATE_DB.ApplicationStartDate;
	pSendMsg->ApplicationEndDate		= m_SPOLLDATE_DB.ApplicationEndDate;
	pSendMsg->VoteStartDate				= m_SPOLLDATE_DB.VoteStartDate;
	pSendMsg->VoteEndDate				= m_SPOLLDATE_DB.VoteEndDate;
	pSendMsg->Election					= m_SPOLLDATE_DB.Election;
	if(NULL != i_pFISoc)
	{
		i_pFISoc->SendAddData(SendBuf, MSG_SIZE(MSG_FC_CITY_POLL_REQUEST_POLL_DATE_OK));
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckPossibleDeleteLeaderCandidate(UID32_t i_nCharacterUID)
/// \brief		������ �ĺ� Ż�� ������ �������� üũ
/// \author		dhjin
/// \date		2007-11-02 ~ 2007-11-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckPossibleDeleteLeaderCandidate(UID32_t i_nCharacterUID)
{
	mt_auto_lock mta(&m_mtvectSLEADER_CANDIDATE);

	mtvectSLEADER_CANDIDATE::iterator itr = m_mtvectSLEADER_CANDIDATE.begin();
	for(; itr != m_mtvectSLEADER_CANDIDATE.end(); itr++)
	{
		if(i_nCharacterUID == itr->CharacterUID
			&& FALSE == itr->DeleteCandidate)
		{
			return TRUE;
		}
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CInflWarManager::CheckRegLeaderCandidateGuild(UID32_t i_GuildUID)
/// \brief		�������� �������� ������ ��꿡 ������ �ĺ� ���� ���� - ������ �ĺ� ����� �������� üũ
/// \author		dhjin
/// \date		2008-04-02 ~ 2008-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckRegLeaderCandidateGuild(UID32_t i_GuildUID)
{
	mt_auto_lock mta(&m_mtvectSLEADER_CANDIDATE);

	mtvectSLEADER_CANDIDATE::iterator itr = m_mtvectSLEADER_CANDIDATE.begin();
	for(; itr != m_mtvectSLEADER_CANDIDATE.end(); itr++)
	{
		if(i_GuildUID == itr->GuildUID)
		{
			return TRUE;
		}
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT CInflWarManager::GetBossWarStrateyPointSummoningCountByInfl(BYTE i_byInfluence)
/// \brief		���� ����� �����ִ� ��������Ʈ �� üũ�Ͽ� ��ȯ
/// \author		dhjin
/// \date		2007-11-06 ~ 2007-11-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CInflWarManager::GetBossWarStrateyPointSummoningCountByInfl(BYTE i_byInfluence)
{
	mt_auto_lock mta(&m_mtvectSummonStrategyPointInfo);

	INT	tmStrategyPointSummoningCount = 0;
	mtvectorSSUMMON_STRATEGYPOINT_INFO::iterator	itr = m_mtvectSummonStrategyPointInfo.begin();
	while(itr != m_mtvectSummonStrategyPointInfo.end())
	{
		if(i_byInfluence == itr->InfluenceType)
		{
			tmStrategyPointSummoningCount++;
		}
		itr++;
	}

	return tmStrategyPointSummoningCount;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SendCityInfoInfluence(CFieldIOCPSocket *i_pFISoc)
/// \brief		���� ���� �� ���� ������ ������.
/// \author		dhjin
/// \date		2007-12-03 ~ 2007-12-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SendCityInfoInfluence(CFieldIOCPSocket *i_pFISoc)
{
	CInflWarData *pVCNInflWarData = GetInflWarDataByInflType(INFLUENCE_TYPE_VCN);
	mt_auto_lock mtV(pVCNInflWarData->GetmtlockPtr());
	INIT_MSG_WITH_BUFFER(MSG_FC_CITY_WARINFO_INFLUENCE_OK, T_FC_CITY_WARINFO_INFLUENCE_OK, pSendMsg, SendBuf);
	pSendMsg->VCNInfluencePoint			= pVCNInflWarData->ContributionPoint;
	util::strncpy(pSendMsg->VCNInfluenceLeader, pVCNInflWarData->InflLeaderCharacterName, SIZE_MAX_CHARACTER_NAME);
	util::strncpy(pSendMsg->VCNInfluenceSubLeader1, pVCNInflWarData->InflSubLeader1CharacterName, SIZE_MAX_CHARACTER_NAME);
	util::strncpy(pSendMsg->VCNInfluenceSubLeader2, pVCNInflWarData->InflSubLeader2CharacterName, SIZE_MAX_CHARACTER_NAME);
	mtV.auto_unlock_cancel();

	CInflWarData *pANIInflWarData = GetInflWarDataByInflType(INFLUENCE_TYPE_ANI);
	mt_auto_lock mtA(pANIInflWarData->GetmtlockPtr());
	pSendMsg->ANIInfluencePoint			= pANIInflWarData->ContributionPoint;
	util::strncpy(pSendMsg->ANIInfluenceLeader, pANIInflWarData->InflLeaderCharacterName, SIZE_MAX_CHARACTER_NAME);
	util::strncpy(pSendMsg->ANIInfluenceSubLeader1, pANIInflWarData->InflSubLeader1CharacterName, SIZE_MAX_CHARACTER_NAME);
	util::strncpy(pSendMsg->ANIInfluenceSubLeader2, pANIInflWarData->InflSubLeader2CharacterName, SIZE_MAX_CHARACTER_NAME);
	mtA.auto_unlock_cancel();

	if(NULL != i_pFISoc)
	{
		i_pFISoc->SendAddData(SendBuf, MSG_SIZE(MSG_FC_CITY_WARINFO_INFLUENCE_OK));
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::InsertMSWarInfoByMSWarStart(MapIndex_t i_nStrategypointMapindex, INT i_nStrategypointMapInfluence, BYTE i_byStrategypointBelligerence)
/// \brief		��������Ʈ ��ȯ�� ������ �����Ѵ�.
/// \author		dhjin
/// \date		2008-03-26 ~ 2008-03-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::InsertMSWarInfoByMSWarStart(MapIndex_t i_nStrategypointMapindex, INT i_nStrategypointMapInfluence, BYTE i_byStrategypointBelligerence)
{
	MSWARINFO_DISPLAY SMSWarInfoDisPlay;
	util::zero(&SMSWarInfoDisPlay, sizeof(MSWARINFO_DISPLAY));
	
	SMSWarInfoDisPlay.MapIndex			= i_nStrategypointMapindex;
	SMSWarInfoDisPlay.MapInfluenceType  = i_nStrategypointMapInfluence;
	SMSWarInfoDisPlay.HPRate			= MS_WAR_HP_RATE;
	SMSWarInfoDisPlay.TelePortState		= TELEPORT_STATE_NOT_SUMMON;
	SMSWarInfoDisPlay.Belligerence		= i_byStrategypointBelligerence;

	this->m_MSWarInfoDisPlay.SetMSWarInfoDisPlay(&SMSWarInfoDisPlay);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SendMSWarInfoForAllUser()
/// \brief		���� ���� ǥ�� ��ȹ�� - ���� ���� ����
/// \author		dhjin
/// \date		2008-03-27 ~ 2008-03-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SendMSWarInfoForAllUser()
{
	vectMSWARINFO_DISPLAY VectMSWarInfoDisPlay;
	VectMSWarInfoDisPlay.clear();
	
	this->m_MSWarInfoDisPlay.AllGetMSWarInfoDisPlay(&VectMSWarInfoDisPlay);
	
	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_MSWARINFO_DISPLAY, T_FC_INFO_MSWARINFO_DISPLAY, pMSWarInfoDisPlayInfo, MSWarInfoDisPlayInfoSendBuf);
	pMSWarInfoDisPlayInfo->MSWarInfoDisPlayListCount = this->MakeMSG_FC_INFO_MSWARINFO_DISPLAY((SMSWARINFO_DISPLAY*)(MSWarInfoDisPlayInfoSendBuf+MSG_SIZE(MSG_FC_INFO_MSWARINFO_DISPLAY)), &VectMSWarInfoDisPlay);
	if(0 >= pMSWarInfoDisPlayInfo->MSWarInfoDisPlayListCount)
	{
		return;
	}

	m_pFieldIOCP14->SendMessageToAllClients(MSWarInfoDisPlayInfoSendBuf, MSG_SIZE(MSG_FC_INFO_MSWARINFO_DISPLAY)+pMSWarInfoDisPlayInfo->MSWarInfoDisPlayListCount*sizeof(SMSWARINFO_DISPLAY), INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);
}

void CInflWarManager::SendStrategicPointInfoListForAllUser()
{
	vector<SSTRATEGYPOINT_DISPLAY_INFO> VectStrategicPointInfoDisplay;
	VectStrategicPointInfoDisplay.clear();
	m_StrategicPointInfoDisplay.GetAllStrategicPointDisplay(&VectStrategicPointInfoDisplay);

	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_STRATEGICPOINTINFO_DISPLAY_LIST, T_FC_INFO_STRATEGICPOINTINFO_DISPLAY_LIST, pStrategicPointInfoDisplayInfo, StrategicPointInfoDisplayInfoSendBuf);
	pStrategicPointInfoDisplayInfo->StrategicPointInfoDisplayListCount = this->MakeMSG_FC_INFO_STRATEGICPOINT_DISPLAY((SSTRATEGYPOINT_DISPLAY_INFO*)(StrategicPointInfoDisplayInfoSendBuf + MSG_SIZE(MSG_FC_INFO_STRATEGICPOINTINFO_DISPLAY_LIST)), VectStrategicPointInfoDisplay);
	if (0 >= pStrategicPointInfoDisplayInfo->StrategicPointInfoDisplayListCount)
	{
		return;
	}

	m_pFieldIOCP14->SendMessageToAllClients(StrategicPointInfoDisplayInfoSendBuf, MSG_SIZE(MSG_FC_INFO_STRATEGICPOINTINFO_DISPLAY_LIST) + pStrategicPointInfoDisplayInfo->StrategicPointInfoDisplayListCount*sizeof(SSTRATEGYPOINT_DISPLAY_INFO), INFLUENCE_TYPE_VCN | INFLUENCE_TYPE_ANI);
}

void CInflWarManager::SendStrategicPointInfoForAllUser(MapIndex_t mapIndex)
{
	SSTRATEGYPOINT_DISPLAY_INFO StrategicPointInfoDisplay;
	if (!m_StrategicPointInfoDisplay.GetStrategicPointDisplay(mapIndex, &StrategicPointInfoDisplay))
	{
		StrategicPointInfoDisplay.HPRate = -1;
		StrategicPointInfoDisplay.InfluenceType = INFLUENCE_TYPE_UNKNOWN;
		StrategicPointInfoDisplay.MapIndex = mapIndex;
		StrategicPointInfoDisplay.SummonStrategyPointTime.SetCurrentDateTime();
	}

	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_STRATEGICPOINTINFO_DISPLAY, T_FC_INFO_STRATEGICPOINTINFO_DISPLAY, pStrategicPointInfoDisplayInfo, StrategicPointInfoDisplayInfoSendBuf);
	CopyMemory((StrategicPointInfoDisplayInfoSendBuf + SIZE_FIELD_TYPE_HEADER), &StrategicPointInfoDisplay, sizeof SSTRATEGYPOINT_DISPLAY_INFO);

	m_pFieldIOCP14->SendMessageToAllClients(StrategicPointInfoDisplayInfoSendBuf, MSG_SIZE(MSG_FC_INFO_STRATEGICPOINTINFO_DISPLAY), INFLUENCE_TYPE_VCN | INFLUENCE_TYPE_ANI);
}


void CInflWarManager::SendStrategicPointInfoForUser(CFieldIOCPSocket *i_pFISoc)
{
	vector<SSTRATEGYPOINT_DISPLAY_INFO> VectStrategicPointInfoDisplay;
	VectStrategicPointInfoDisplay.clear();

	m_StrategicPointInfoDisplay.GetAllStrategicPointDisplay(&VectStrategicPointInfoDisplay);

	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_STRATEGICPOINTINFO_DISPLAY_LIST, T_FC_INFO_STRATEGICPOINTINFO_DISPLAY_LIST, pStrategicPointInfoDisplayInfo, StrategicPointInfoDisplayInfoSendBuf);
	pStrategicPointInfoDisplayInfo->StrategicPointInfoDisplayListCount = this->MakeMSG_FC_INFO_STRATEGICPOINT_DISPLAY((SSTRATEGYPOINT_DISPLAY_INFO*)(StrategicPointInfoDisplayInfoSendBuf + MSG_SIZE(MSG_FC_INFO_STRATEGICPOINTINFO_DISPLAY_LIST)), VectStrategicPointInfoDisplay);
	if (0 >= pStrategicPointInfoDisplayInfo->StrategicPointInfoDisplayListCount)
	{
		return;
	}

	i_pFISoc->SendAddData(StrategicPointInfoDisplayInfoSendBuf, MSG_SIZE(MSG_FC_INFO_STRATEGICPOINTINFO_DISPLAY_LIST) + pStrategicPointInfoDisplayInfo->StrategicPointInfoDisplayListCount*sizeof(SSTRATEGYPOINT_DISPLAY_INFO));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SendMSWarInfoForUser(CFieldIOCPSocket *i_pFISoc)
/// \brief		���� ���� ǥ�� ��ȹ�� - ���� ���� ����
/// \author		dhjin
/// \date		2008-03-27 ~ 2008-03-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SendMSWarInfoForUser(CFieldIOCPSocket *i_pFISoc)
{
	vectMSWARINFO_DISPLAY VectMSWarInfoDisPlay;
	VectMSWarInfoDisPlay.clear();
	
	this->m_MSWarInfoDisPlay.AllGetMSWarInfoDisPlay(&VectMSWarInfoDisPlay);
	
	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_MSWARINFO_DISPLAY, T_FC_INFO_MSWARINFO_DISPLAY, pMSWarInfoDisPlayInfo, MSWarInfoDisPlayInfoSendBuf);
	pMSWarInfoDisPlayInfo->MSWarInfoDisPlayListCount = this->MakeMSG_FC_INFO_MSWARINFO_DISPLAY((SMSWARINFO_DISPLAY*)(MSWarInfoDisPlayInfoSendBuf+MSG_SIZE(MSG_FC_INFO_MSWARINFO_DISPLAY)), &VectMSWarInfoDisPlay);
	if(0 >= pMSWarInfoDisPlayInfo->MSWarInfoDisPlayListCount)
	{
		return;
	}

	i_pFISoc->SendAddData(MSWarInfoDisPlayInfoSendBuf, MSG_SIZE(MSG_FC_INFO_MSWARINFO_DISPLAY)+pMSWarInfoDisPlayInfo->MSWarInfoDisPlayListCount*sizeof(SMSWARINFO_DISPLAY));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT CInflWarManager::MakeMSG_FC_INFO_MSWARINFO_DISPLAY(SMSWARINFO_DISPLAY *o_pMSWarInfoDisPlay, vectMSWARINFO_DISPLAY * i_pVectMSWarInfoDisPlay)
/// \brief		���� ���� ǥ�� ��ȹ�� - MSG_FC_INFO_MSWARINFO_DISPLAY
/// \author		dhjin
/// \date		2008-03-27 ~ 2008-03-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CInflWarManager::MakeMSG_FC_INFO_MSWARINFO_DISPLAY(SMSWARINFO_DISPLAY *o_pMSWarInfoDisPlay, vectMSWARINFO_DISPLAY * i_pVectMSWarInfoDisPlay)
{
	int	MSWarInfoDisPlayListCount = 0;

	vectMSWARINFO_DISPLAY::iterator itr = i_pVectMSWarInfoDisPlay->begin();
	for(; itr != i_pVectMSWarInfoDisPlay->end(); itr++)
	{
		o_pMSWarInfoDisPlay[MSWarInfoDisPlayListCount].MapIndex					= itr->MapIndex;
		o_pMSWarInfoDisPlay[MSWarInfoDisPlayListCount].MapInfluenceType			= itr->MapInfluenceType;
		o_pMSWarInfoDisPlay[MSWarInfoDisPlayListCount].HPRate					= itr->HPRate;
		o_pMSWarInfoDisPlay[MSWarInfoDisPlayListCount].TelePortState			= itr->TelePortState;
		o_pMSWarInfoDisPlay[MSWarInfoDisPlayListCount].TelePortBuildingStartTime= itr->TelePortBuildingStartTime;
		o_pMSWarInfoDisPlay[MSWarInfoDisPlayListCount].TelePortBuildingEndTime	= itr->TelePortBuildingEndTime;
		MSWarInfoDisPlayListCount++;
	}
	
	return MSWarInfoDisPlayListCount;
}

INT CInflWarManager::MakeMSG_FC_INFO_STRATEGICPOINT_DISPLAY(SSTRATEGYPOINT_DISPLAY_INFO *o_StrategicPointInfoDisPlay, vector<SSTRATEGYPOINT_DISPLAY_INFO>& i_pVectStrategicPointInfoDisPlay)
{
	int	StrategicPointInfoDisPlayListCount = 0;

	for (auto &itr : i_pVectStrategicPointInfoDisPlay)
	{
		o_StrategicPointInfoDisPlay[StrategicPointInfoDisPlayListCount].MapIndex = itr.MapIndex;
		o_StrategicPointInfoDisPlay[StrategicPointInfoDisPlayListCount].InfluenceType = itr.InfluenceType;
		o_StrategicPointInfoDisPlay[StrategicPointInfoDisPlayListCount].HPRate = itr.HPRate;
		o_StrategicPointInfoDisPlay[StrategicPointInfoDisPlayListCount].SummonStrategyPointTime = itr.SummonStrategyPointTime;
		StrategicPointInfoDisPlayListCount++;
	}

	return StrategicPointInfoDisPlayListCount;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SetMSWarInfo(MapIndex_t i_nMapindex, INT i_nMSWarInfoModifyType, INT i_nHPRate/*= STATE_ERROR*/, INT i_nTelePortState/*= STATE_ERROR*/, BYTE i_byBelligerence/*= STATE_ERROR*/)
/// \brief		���� ���� ǥ�� ��ȹ�� - ���� ����
/// \author		dhjin
/// \date		2008-03-27 ~ 2008-03-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////	
void CInflWarManager::SetMSWarInfo(MapIndex_t i_nMapindex, INT i_nMSWarInfoModifyType, INT i_nHPRate/*= STATE_ERROR*/, INT i_nTelePortState/*= STATE_ERROR*/, BYTE i_byBelligerence/*= STATE_ERROR*/)
{
	switch(i_nMSWarInfoModifyType)
	{
		case T_MSWARINFO_MODIFY_UNKNOWN:			return;			// �˼� ����
		case T_MSWARINFO_MODIFY_HPRATE:
			{// HP��� ����
				if(STATE_ERROR != i_nHPRate)
				{
					this->m_MSWarInfoDisPlay.SetMSWarInfoDisPlayByHPRate(i_nMapindex, i_nHPRate);
				}
			}
			break;
		case T_MSWARINFO_MODIFY_CREATE_TELEPORT:
			{// Teleport ����
				if(STATE_ERROR != i_nHPRate
					&& STATE_ERROR != i_nTelePortState
					&& IS_TELEPORT_MONSTER(i_byBelligerence)
					)
				{
					this->m_MSWarInfoDisPlay.SetMSWarInfoDisPlayByCreateTeleport(i_nMapindex, i_nHPRate, i_nTelePortState, i_byBelligerence);
				}
			}
			break;	
		case T_MSWARINFO_MODIFY_CHANGE_TELEPORT_STATE:
			{// Teleport ����
				if(STATE_ERROR != i_nHPRate
					&& STATE_ERROR != i_nTelePortState
					&& IS_TELEPORT_MONSTER(i_byBelligerence)
					)
				{
					//////////////////////////////////////////////////////////////////////////
					// 2008-04-22 by dhjin, ���� ���� ǥ�� ��ȹ�� - �ڷ���Ʈ ���� �Ϸ� �ð�
					if(TELEPORT_STATE_BUILDING == i_nTelePortState)
					{
						INIT_MSG_WITH_BUFFER(MSG_FC_EVENT_CLICK_TELEPORT_OK, T_FC_EVENT_CLICK_TELEPORT_OK, pMsg, SendBuf);
						pMsg->MapIndex			= i_nMapindex;
						pMsg->StartTime.SetCurrentDateTime();
						pMsg->EndTime			= pMsg->StartTime;
						pMsg->EndTime.Second = 0;
						pMsg->EndTime.AddDateTime(0,0,0,0,1,TELEPORT_BUILDING_TIME_SECOND);
						m_pFieldIOCP14->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_EVENT_CLICK_TELEPORT_OK));
						this->m_MSWarInfoDisPlay.SetMSWarInfoDisPlayByChangeTeleportState(i_nMapindex, i_nHPRate, i_nTelePortState, &pMsg->StartTime, &pMsg->EndTime);
					}	
					else
					{
						this->m_MSWarInfoDisPlay.SetMSWarInfoDisPlayByChangeTeleportState(i_nMapindex, i_nHPRate, i_nTelePortState);
					}
				}
			}
			break;
		default:
			{
				return;
			}
			break;
	}
	this->SendMSWarInfoForAllUser();
	return;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::InitMSWarOptionTypeW(BYTE i_byLeaderInfluence)
/// \brief		���� ���� ǥ�� ��ȹ�� -  ���� ���� �ɼ� �ʱ�ȭ 
/// \author		dhjin
/// \date		2008-03-28 ~ 2008-03-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::InitMSWarOptionTypeW(BYTE i_byLeaderInfluence)
{
	this->m_MSWarInfoDisPlay.InitMSWarOptionType(i_byLeaderInfluence);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SetMSWarOptionTypeW(BYTE i_byLeaderInfluence, SHORT i_byMSWarOptionType)
/// \brief		���� ���� ǥ�� ��ȹ�� - ���� ���� �ɼ� ���� 
/// \author		dhjin
/// \date		2008-03-28 ~ 2008-03-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SetMSWarOptionTypeW(BYTE i_byLeaderInfluence, SHORT i_byMSWarOptionType)
{
	this->m_MSWarInfoDisPlay.SetMSWarOptionType(i_byLeaderInfluence, i_byMSWarOptionType);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SendMSWarOptionTypeForInfluenceAllUser(BYTE i_byLeaderInfluence)
/// \brief		���� ���� ǥ�� ��ȹ�� - ���� ���� �ɼ� ���¿� ��� �������� �����ϱ�
/// \author		dhjin
/// \date		2008-03-28 ~ 2008-03-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SendMSWarOptionTypeForInfluenceAllUser(BYTE i_byLeaderInfluence)
{
	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_MSWARINFO_DISPLAY_OPTION_OK, T_FC_INFO_MSWARINFO_DISPLAY_OPTION_OK, pMSWarInfoDisPlayOption, MSWarInfoDisPlayOptionSendBuf);
	pMSWarInfoDisPlayOption->MSWarOptionType		= this->m_MSWarInfoDisPlay.GetMSWarOptionType(i_byLeaderInfluence);
	m_pFieldIOCP14->SendMessageToAllClients(MSWarInfoDisPlayOptionSendBuf, MSG_SIZE(MSG_FC_INFO_MSWARINFO_DISPLAY_OPTION_OK), i_byLeaderInfluence);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SendMSWarOptionTypeForInfluenceUser(CFieldIOCPSocket *i_pFISoc)
/// \brief		���� ���� ǥ�� ��ȹ�� - ���� ���� �ɼ� �������� �����ϱ�
/// \author		dhjin
/// \date		2008-03-28 ~ 2008-03-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SendMSWarOptionTypeForInfluenceUser(CFieldIOCPSocket *i_pFISoc)
{
	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_MSWARINFO_DISPLAY_OPTION_OK, T_FC_INFO_MSWARINFO_DISPLAY_OPTION_OK, pMSWarInfoDisPlayOption, MSWarInfoDisPlayOptionSendBuf);
	pMSWarInfoDisPlayOption->MSWarOptionType		= this->m_MSWarInfoDisPlay.GetMSWarOptionType(i_pFISoc->m_character.InfluenceType);
	i_pFISoc->SendAddData(MSWarInfoDisPlayOptionSendBuf, MSG_SIZE(MSG_FC_INFO_MSWARINFO_DISPLAY_OPTION_OK));
}

//////////////////////////////////////////////////////////////////////////
// 2008-04-01 by dhjin, ����, ������ ����â ��ȹ�� - ���� ��� ����
void CInflWarManager::ResetMSWarInfoResult()
{
	m_vectMSWarInfoResult.clear();
}

void CInflWarManager::ResetSPWarInfoResult()
{
	m_vectSPWarInfoResult.clear();
}

void CInflWarManager::DeleteMSWarInfoResult(INT i_nMonsterUID)
{
	vectSMSWAR_INFO_RESULT::iterator itr = m_vectMSWarInfoResult.begin();
	for(; itr != m_vectMSWarInfoResult.end(); itr++)
	{
		if(i_nMonsterUID == itr->MonsterUID)
		{
			itr = m_vectMSWarInfoResult.erase(itr);
			return;
		}
	}
}

void CInflWarManager::DeleteSPWarInfoResult(INT i_nSPSummonMapIndex)
{
	vectSSPWAR_INFO_RESULT::iterator itr = m_vectSPWarInfoResult.begin();
	for(; itr != m_vectSPWarInfoResult.end(); itr++)
	{
		if(i_nSPSummonMapIndex == itr->SPSummonMapIndex)
		{
			itr = m_vectSPWarInfoResult.erase(itr);
			return;
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SetMSWarInfoResultByWarStart(BYTE i_byAttInfluence, INT i_nMonsterUID, INT i_nContributionPoint)
/// \brief		����, ������ ����â ��ȹ�� - ���� ���� �� ���� ����
/// \author		dhjin
/// \date		2008-04-01 ~ 2008-04-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SetMSWarInfoResultByWarStart(BYTE i_byAttInfluence, INT i_nMonsterUID, INT i_nContributionPoint)
{
	SMSWAR_INFO_RESULT MSWarInfoResult;
	MSWarInfoResult.AttInfluence		= i_byAttInfluence;
	MSWarInfoResult.MonsterUID			= i_nMonsterUID;
	MSWarInfoResult.ContributionPoint	= i_nContributionPoint;
	MSWarInfoResult.MSWarStartTime.SetCurrentDateTime();
	
	vectSMSWAR_INFO_RESULT::iterator itr = m_vectMSWarInfoResult.begin();
	for(; itr != m_vectMSWarInfoResult.end(); itr++)
	{
		if(i_nMonsterUID == itr->MonsterUID)
		{
			return;
		}
	}

	m_vectMSWarInfoResult.push_back(MSWarInfoResult);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SetMSWarInfoResultByWarEnd(INT i_nMonsterUID, BYTE i_byWinInfluence)
/// \brief		����, ������ ����â ��ȹ�� - ���� ���� �� ���� ����
/// \author		dhjin
/// \date		2008-04-01 ~ 2008-04-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SetMSWarInfoResultByWarEnd(INT i_nMonsterUID, BYTE i_byWinInfluence)
{
	vectSMSWAR_INFO_RESULT::iterator itr = m_vectMSWarInfoResult.begin();
	for(; itr != m_vectMSWarInfoResult.end(); itr++)
	{
		if(i_nMonsterUID == itr->MonsterUID)
		{
			itr->WinInfluence		= i_byWinInfluence;
			return;
		}
	}
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SendLogMSWarInfoResult(INT i_nMonsterUID)
/// \brief		����, ������ ����â ��ȹ�� - ���� ���� �� ���� �α� ����
/// \author		dhjin
/// \date		2008-04-01 ~ 2008-04-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SendLogMSWarInfoResult(INT i_nMonsterUID)
{
	vectSMSWAR_INFO_RESULT::iterator itr = m_vectMSWarInfoResult.begin();
	for(; itr != m_vectMSWarInfoResult.end(); itr++)
	{
		if(i_nMonsterUID == itr->MonsterUID)
		{
			// 2012-08-30 by jhseol, Ǯ�α� �����
#ifdef S_FULL_LOG_JHSEOL
			// 2008-08-28 by dhjin, ���� ����, ����DB�� ���ܾ� Admintool�� �ʱ�ȭ�� �����ϴ�.
			MSG_FL_LOG_MS_WAR MSWarLog;
			MSWarLog.AttInfluence		= itr->AttInfluence;
			MSWarLog.ContributionPoint	= itr->ContributionPoint;
			MSWarLog.MonsterUID			= itr->MonsterUID;
			MSWarLog.WinInfluence		= itr->WinInfluence;
			MSWarLog.MSWarStartTime		= itr->MSWarStartTime;

			CAtumLogSender::SendLogMessageMSWar(&MSWarLog);
#endif	//#ifdef S_FULL_LOG_JHSEOL
			// end 2012-08-30 by jhseol, Ǯ�α� �����

			QPARAM_INSERT_MSWAR_LOG *pQParam = new QPARAM_INSERT_MSWAR_LOG;	
			pQParam->AttInfluence		= itr->AttInfluence;
			pQParam->ContributionPoint	= itr->ContributionPoint;
			pQParam->MonsterUID			= itr->MonsterUID;
			pQParam->WinInfluence		= itr->WinInfluence;
			pQParam->MSWarStartTime		= itr->MSWarStartTime;
			this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InsertMSWarLog, NULL, 0, pQParam);
	
			return;
		}
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SetSPWarInfoResultByWarStart(BYTE i_byAttInfluence, INT i_nSPSummonMapIndex)
/// \brief		����, ������ ����â ��ȹ�� - ������ ���� �� ���� ����
/// \author		dhjin
/// \date		2008-04-01 ~ 2008-04-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SetSPWarInfoResultByWarStart(BYTE i_byAttInfluence, INT i_nSPSummonMapIndex)
{
	SSPWAR_INFO_RESULT SPWarInfoResult;
	SPWarInfoResult.AttInfluence		= i_byAttInfluence;
	SPWarInfoResult.SPSummonMapIndex	= i_nSPSummonMapIndex;
	SPWarInfoResult.SPWarStartTime.SetCurrentDateTime();

	vectSSPWAR_INFO_RESULT::iterator itr = m_vectSPWarInfoResult.begin();
	for(; itr != m_vectSPWarInfoResult.end(); itr++)
	{
		// 2008-07-24 by dhjin, ���� �α� ��Ͽ� �ȳ��� ���� ����
//		if(i_nSPSummonMapIndex != itr->SPSummonMapIndex)
		if(i_nSPSummonMapIndex == itr->SPSummonMapIndex)
		{
			return;
		}
	}

	m_vectSPWarInfoResult.push_back(SPWarInfoResult);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SetSPWarInfoResultByWarEnd(BYTE i_byWinInfluence)
/// \brief		����, ������ ����â ��ȹ�� - ������ ���� �� ���� ����
/// \author		dhjin
/// \date		2008-04-01 ~ 2008-04-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SetSPWarInfoResultByWarEnd(INT i_nSPSummonMapIndex, BYTE i_byWinInfluence)
{
	vectSSPWAR_INFO_RESULT::iterator itr = m_vectSPWarInfoResult.begin();
	for(; itr != m_vectSPWarInfoResult.end(); itr++)
	{
		if(i_nSPSummonMapIndex == itr->SPSummonMapIndex)
		{
			itr->WinInfluence		= i_byWinInfluence;
			return;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SendLogSPWarInfoResult()
/// \brief		����, ������ ����â ��ȹ�� - ������ ���� �� ���� �α� ����
/// \author		dhjin
/// \date		2008-04-01 ~ 2008-04-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SendLogSPWarInfoResult(INT i_nSPSummonMapIndex)
{
	vectSSPWAR_INFO_RESULT::iterator itr = m_vectSPWarInfoResult.begin();
	for(; itr != m_vectSPWarInfoResult.end(); itr++)
	{
		if(i_nSPSummonMapIndex == itr->SPSummonMapIndex)
		{
			// 2012-08-30 by jhseol, Ǯ�α� �����
#ifdef S_FULL_LOG_JHSEOL
			// 2008-08-28 by dhjin, ���� ����, ����DB�� ���ܾ� Admintool�� �ʱ�ȭ�� �����ϴ�.
			MSG_FL_LOG_SP_WAR SPWarLog;
			SPWarLog.AttInfluence		= itr->AttInfluence;
			SPWarLog.SPSummonMapIndex   = itr->SPSummonMapIndex;
			SPWarLog.WinInfluence		= itr->WinInfluence;
			SPWarLog.SPWarStartTime		= itr->SPWarStartTime;

			//CAtumLogSender::SendLogMessageSPWar(&SPWarLog);	// 2012-12-14 by jhseol, ������ �α� �ι� ����� ���� ����. �ּ�ó����.
#endif	//#ifdef S_FULL_LOG_JHSEOL
			// end 2012-08-30 by jhseol, Ǯ�α� �����
			QPARAM_INSERT_SPWAR_LOG *pQParam = new QPARAM_INSERT_SPWAR_LOG;	
			pQParam->AttInfluence		= itr->AttInfluence;
			pQParam->SPSummonMapIndex	= itr->SPSummonMapIndex;
			pQParam->WinInfluence		= itr->WinInfluence;
			pQParam->SPWarStartTime		= itr->SPWarStartTime;
			this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InsertSPWarLog, NULL, 0, pQParam);
	
			return;
		}
	}
}

#ifdef NEMERIAN_NATION_BALANCE_BUFF_PENALITY
void CInflWarManager::setDiffVictory() {
	this->m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_GetWarVictoryDiff, m_pFieldIOCP14->GetFieldIOCPSocket(0), 0, NULL);
}
#endif

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::SetMSWarInfoContributionPoint(INT i_nMonsterIdx)
/// \brief		����, ������ ����â ��ȹ�� - ���� ����Ʈ
/// \author		dhjin
/// \date		2008-04-22 ~ 2008-04-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CInflWarManager::SetMSWarInfoContributionPoint(INT i_nMonsterIdx)
{
	vectSMSWAR_INFO_RESULT::iterator itr = m_vectMSWarInfoResult.begin();
	for(; itr != m_vectMSWarInfoResult.end(); itr++)
	{
		if(i_nMonsterIdx == itr->MonsterUID)
		{
			return itr->ContributionPoint;
		}
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-12-04 by cmkwon, �������� ���/���2 ���� ��ȯ ó�� ���� ���� - 
/// \author		cmkwon
/// \date		2009-12-04 ~ 2009-12-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CInflWarManager::CheckCreateableMonsterInMap(INT i_nMapIdx)
{
	vectSSPWAR_INFO_RESULT::iterator itr = m_vectSPWarInfoResult.begin();
	for(; itr != m_vectSPWarInfoResult.end(); itr++)
	{
		SSPWAR_INFO_RESULT *pSSPWarInfo = &*itr;
		if(i_nMapIdx == pSSPWarInfo->SPSummonMapIndex)
		{
			return FALSE;
		}

		const MAP_INFO *pMapInfo = CAtumSJ::GetMapInfo(pSSPWarInfo->SPSummonMapIndex);
		if(pMapInfo)
		{
			if(i_nMapIdx == pMapInfo->BeforeMapIndex
				|| i_nMapIdx == pMapInfo->BeforeMapIndex2)
			{
				return FALSE;
			}
		}
	}
	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::MSWarBeforeStart(BYTE i_byInflTy)
/// \brief		���� ���� - ���� ������ ������ ����	
/// \author		dhjin
/// \date		2009-01-13 ~ 2009-01-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::MSWarBeforeStart(BYTE i_byInflTy)
{
	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byInflTy);
	if(NULL == pInflWarData)
	{
		return;
	}

	mt_auto_lock mtA(pInflWarData->GetmtlockPtr());
	SDB_INFLUENCE_WAR_INFO *pCurInfoByWartimeStage = pInflWarData->GetCurInflWarInfo();
	
	if(FALSE != pCurInfoByWartimeStage->IsSummonJacoMonster)
	{
		INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_SUMMON_JACO_MONSTER, T_FN_NPCSERVER_SUMMON_JACO_MONSTER, pSJacoMonster, SendBuf);
		pSJacoMonster->Belligerence0	= GET_SAME_MONSTER_BELL_BY_CHARACTER_INFLTYPE(pCurInfoByWartimeStage->InfluenceType);
		pSJacoMonster->IsSummonJacoMonster	= TRUE;
		if(m_pFieldIOCP14->Send2NPCServerByTCP(SendBuf, MSG_SIZE(MSG_FN_NPCSERVER_SUMMON_JACO_MONSTER)))
		{// 2006-04-25 by cmkwon, NPCServer�� ���� �����ÿ� 

			pInflWarData->SetSummonJacoMonster(TRUE);

			///////////////////////////////////////////////////////////////////////////////
			// 2006-04-20 by cmkwon
			INIT_MSG_WITH_BUFFER(MSG_FC_WAR_JACO_MONSTER_SUMMON, T_FC_WAR_JACO_MONSTER_SUMMON, pSJaco, SendBuf);
			pSJaco->nBelligerence0			= GET_SAME_MONSTER_BELL_BY_CHARACTER_INFLTYPE(pCurInfoByWartimeStage->InfluenceType);
			m_pFieldIOCP14->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_WAR_JACO_MONSTER_SUMMON), INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);
		}
	}

	this->m_DeclarationOfWar.SetMSWarEndState(i_byInflTy, MSWARING_BEFORE);

	mtA.auto_unlock_cancel();			// 2009-07-02 by cmkwon, �������� ���� ���� ���� - ���⿡�� ������

	// 2009-07-02 by cmkwon, ���� ����� ���� ���� �α� �߰� - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] DeclarationNoti CInflWarManager::MSWarBeforeStart# %s WarTimeStage(%d) BossMonsterNum(%8d) IsSummonJacoMonster(%d) \r\n"
		, CAtumSJ::GetInfluenceTypeString(pCurInfoByWartimeStage->InfluenceType), pCurInfoByWartimeStage->WartimeStage, pCurInfoByWartimeStage->BossMonsterUnitKind, pCurInfoByWartimeStage->IsSummonJacoMonster);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::MSWarStart(BYTE i_byInflTy)
/// \brief		���� ���� - ���� ����
/// \author		dhjin
/// \date		2009-01-12 ~ 2009-01-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::MSWarStart(BYTE i_byInflTy)
{
	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byInflTy);
	if(NULL == pInflWarData)
	{
		return;
	}

	mt_auto_lock mtA(pInflWarData->GetmtlockPtr());
	SDB_INFLUENCE_WAR_INFO *pCurInfoByWartimeStage = pInflWarData->GetCurInflWarInfo();
	// 2009-03-10 by dhjin, �ܰ躰 �� �ý���
	BYTE BeforeWinCheck = 0;	// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - �� �ܰ� ���� ����
	BYTE BossSummonStep = 0;	// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - ���� �ܰ� ���̹Ƿ� pCurInfoByWartimeStage�� �ܰ�� ���ܰ谡 �ȴ� ���� ���� ���Ѵ�.
	INT SummonBossMonsterUID = GetMSBossSummonValue(i_byInflTy, &BeforeWinCheck);
	vectDB_INFLUENCE_WAR_INFO *pInflWarInfoList = this->GetInflWarInfoByInflType(i_byInflTy);
	if(NULL == pInflWarInfoList)
	{
		return;
	}
	for(int i=0; i < pInflWarInfoList->size(); i++)
	{
		SDB_INFLUENCE_WAR_INFO *pInflWarInfo = &((*pInflWarInfoList)[i]);
		if(SummonBossMonsterUID == pInflWarInfo->BossMonsterUnitKind
			&& i_byInflTy == pInflWarInfo->InfluenceType)
		{
			BossSummonStep = pInflWarInfo->WartimeStage;
			break;
		}
	}

	// 2009-07-02 by cmkwon, ���� ����� ���� ���� �α� �߰� - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] DeclarationNoti CInflWarManager::MSWarStart#       %s WarTimeStage(%d) BossMonsterNum(%8d) ==> RealBossMonsterNum(%8d) BeforeWinCheck(%d) \r\n"
		, CAtumSJ::GetInfluenceTypeString(pCurInfoByWartimeStage->InfluenceType), pCurInfoByWartimeStage->WartimeStage, pCurInfoByWartimeStage->BossMonsterUnitKind, SummonBossMonsterUID, BeforeWinCheck);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-18 by cmkwon, ������ ���� ���� ��ȯ ó��
	if(0 != SummonBossMonsterUID)	// 2009-03-10 by dhjin, �ܰ躰 �� �ý���
	{		
		MONSTER_INFO *pBossMon = m_pFieldIOCP14->GetMonsterInfo(SummonBossMonsterUID);	// 2009-03-10 by dhjin, �ܰ躰 �� �ý���
		if(NULL == pBossMon
			|| FALSE == IS_MOTHERSHIPWAR_MONSTER(pBossMon->Belligerence)
			|| FALSE == COMPARE_MPOPTION_BIT(pBossMon->MPOption, MPOPTION_BIT_BOSS_MONSTER))
		{
			char szErr[1024];
			wsprintf(szErr, "CInflWarManager::MSWarStart Error, MonsterUnitKind(%8d) Belligerence(%d)"
				, SummonBossMonsterUID, (NULL==pBossMon)?0:pBossMon->Belligerence);	// 2009-03-10 by dhjin, �ܰ躰 �� �ý���
			g_pFieldGlobal->WriteSystemLog(szErr);
			DbgOut("%s\rn", szErr);
			return;
		}
		
		///////////////////////////////////////////////////////////////////////////////
		// 2006-04-14 by cmkwon, ������ ���� ���� ��ȯ ���� �߰�
		SINFLBOSS_MONSTER_SUMMON_DATA tmSummon;
		tmSummon.pMonsterInfo		= pBossMon;
		tmSummon.SummonCount		= pCurInfoByWartimeStage->BossMonsterCount;
		tmSummon.SummonTerm			= pCurInfoByWartimeStage->BossMonsterSummonTerm;
		tmSummon.atimeInsertedTime.SetCurrentDateTime(TRUE);
		tmSummon.SummonMapIndex		= pCurInfoByWartimeStage->BossMonsterSummonMapIndex;
		tmSummon.SummonPosition.x	= pCurInfoByWartimeStage->BossMonsterSummonPositionX;
		tmSummon.SummonPosition.y	= pCurInfoByWartimeStage->BossMonsterSummonPositionY;
		tmSummon.SummonPosition.z	= pCurInfoByWartimeStage->BossMonsterSummonPositionZ;
		tmSummon.BossStep			= BossSummonStep;					// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - �� �ܰ�
		tmSummon.BeforeWinCheck		= BeforeWinCheck;			// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - �� �ܰ� ���� ����
		m_mtvectSummonMonsterDataList.pushBackLock(tmSummon);

		///////////////////////////////////////////////////////////////////////////////
		// 2006-04-14 by cmkwon, ������ ���� ���� ��ȯ ���� Ŭ���̾�Ʈ�� ����
		INIT_MSG_WITH_BUFFER(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA, T_FC_WAR_BOSS_MONSTER_SUMMON_DATA, pSWarData, SendBuf);
		pSWarData->SummonMonsterUnitkind	= pBossMon->MonsterUnitKind;
		pSWarData->RemainMinute				= pCurInfoByWartimeStage->BossMonsterSummonTerm;
		pSWarData->BossStep					= BossSummonStep;			// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - �� �ܰ�
		pSWarData->BeforeWinCheck			= BeforeWinCheck;				// 2009-03-10 by dhjin, �ܰ躰 �� �ý��� - �� �ܰ� ���� ����
		m_pFieldIOCP14->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA), INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI);

		//////////////////////////////////////////////////////////////////////////
		// 2008-04-01 by dhjin, ����, ������ ����â ��ȹ�� - 
		this->SetMSWarInfoResultByWarStart(IS_VCN_INFLUENCE_TYPE(pCurInfoByWartimeStage->InfluenceType) ? INFLUENCE_TYPE_VCN : INFLUENCE_TYPE_ANI, SummonBossMonsterUID, pInflWarData->ContributionPoint);		// 2009-03-10 by dhjin, �ܰ躰 �� �ý���

		// �� ���� ���� ����Ǿ�� �Ǵ� ��
		SDECLARATION_OF_WAR DeclarationOfWar;
		util::zero(&DeclarationOfWar, sizeof(SDECLARATION_OF_WAR));
		DeclarationOfWar.Influence			=	i_byInflTy;
		DeclarationOfWar.MSAppearanceMap	=	pCurInfoByWartimeStage->BossMonsterSummonMapIndex;
		DeclarationOfWar.MSNum				=	SummonBossMonsterUID;		// 2009-03-10 by dhjin, �ܰ躰 �� �ý���
		DeclarationOfWar.NCP				=	pInflWarData->ContributionPoint;
		this->m_DeclarationOfWar.SetMSWarStepByMSWarStart(&DeclarationOfWar);

		QPARAM_UPDATE_START_DECLARATION_OF_WAR *pQParam = new QPARAM_UPDATE_START_DECLARATION_OF_WAR;
		pQParam->Influence					=	i_byInflTy;	
		pQParam->MSWarStep					=	m_DeclarationOfWar.GetCurrentMSWarStep(i_byInflTy);
		pQParam->NCP						=	pInflWarData->ContributionPoint;
		pQParam->MSNum						=	SummonBossMonsterUID;		// 2009-03-10 by dhjin, �ܰ躰 �� �ý���
		pQParam->MSAppearanceMap			=	pCurInfoByWartimeStage->BossMonsterSummonMapIndex;
		m_pFieldIOCP14->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateStartDeclarationOfWar, NULL, 0, pQParam);		
	}

}
 
// 2009-07-02 by cmkwon, �������� ���� ���� ���� - 
/////////////////////////////////////////////////////////////////////////////////
///// \fn			void CInflWarManager::SetInflEventRate()
///// \brief		���� ���� - ������ �¸� �� ���� �Ǵ� �̺�Ʈ ����
///// \author		dhjin
///// \date		2009-03-31 ~ 2009-03-31
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
// void CInflWarManager::SetInflEventRate()
// {
// 	CInflWarData *pBCUInflWarData = GetInflWarDataByInflType(INFLUENCE_TYPE_VCN);
// 	if(pBCUInflWarData)
// 	{
// 		SDB_INFLUENCE_WAR_INFO		*m_pLastMSWinInflInfo;
// 		m_pLastMSWinInflInfo = pBCUInflWarData->GetInflWarInfoByWartimeStage(this->m_DeclarationOfWar.GetLastMSWinStep(INFLUENCE_TYPE_VCN));
// 		if(NULL == m_pLastMSWinInflInfo)
// 		{
// 			pBCUInflWarData->fInflHPRepairRate = 0;
// 			pBCUInflWarData->fInflDPRepairRate = 0;
// 			pBCUInflWarData->fInflSPRepairRate = 0;
// 		}
// 		else
// 		{
// 			pBCUInflWarData->fInflHPRepairRate = m_pLastMSWinInflInfo->HPRepairRate;
// 			pBCUInflWarData->fInflDPRepairRate = m_pLastMSWinInflInfo->DPRepairRate;
// 			pBCUInflWarData->fInflSPRepairRate = m_pLastMSWinInflInfo->SPRepairRate;
// 		}
// 	}
// 
// 	CInflWarData *pANIInflWarData = GetInflWarDataByInflType(INFLUENCE_TYPE_ANI);
// 	if(pANIInflWarData)
// 	{
// 		SDB_INFLUENCE_WAR_INFO		*m_pLastMSWinInflInfo;
// 		m_pLastMSWinInflInfo = pANIInflWarData->GetInflWarInfoByWartimeStage(this->m_DeclarationOfWar.GetLastMSWinStep(INFLUENCE_TYPE_ANI));
// 		if(NULL == m_pLastMSWinInflInfo)
// 		{
// 			pANIInflWarData->fInflHPRepairRate = 0;
// 			pANIInflWarData->fInflDPRepairRate = 0;
// 			pANIInflWarData->fInflSPRepairRate = 0;
// 		}
// 		else
// 		{
// 			pANIInflWarData->fInflHPRepairRate = m_pLastMSWinInflInfo->HPRepairRate;
// 			pANIInflWarData->fInflDPRepairRate = m_pLastMSWinInflInfo->DPRepairRate;
// 			pANIInflWarData->fInflSPRepairRate = m_pLastMSWinInflInfo->SPRepairRate;
// 		}
// 
// 	}
// }


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-07-02 by cmkwon, �������� ���� ���� ���� - 
/// \author		cmkwon
/// \date		2009-07-02 ~ 2009-07-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::SetCompensationForDecalationOfWarWin(BYTE i_byInflTy, INT i_BossMonNum/*=0*/)
{
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] DeclarationNoti CInflWarManager::SetCompensationForDecalationOfWarWin# 100 %s i_BossMonNum(%d) \r\n"
		, CAtumSJ::GetInfluenceTypeString(i_byInflTy), i_BossMonNum);

	CInflWarData *pInflWarData = GetInflWarDataByInflType(i_byInflTy);
	if(NULL == pInflWarData)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] DeclarationNoti CInflWarManager::SetCompensationForDecalationOfWarWin# 110 %s pInfoWarData(0x%X) \r\n", CAtumSJ::GetInfluenceTypeString(i_byInflTy), pInflWarData);
		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2009-07-03 by cmkwon, �¸��� �ְ� ������ ������ �����´�.
	INT		nBossMonNum	= i_BossMonNum;
	BYTE	byMSWarStep	= 0;
	if(0 >= nBossMonNum)
	{
		byMSWarStep = m_DeclarationOfWar.GetMostMSWarStepOfWinStep(i_byInflTy, &nBossMonNum);
	}
	if(0 >= nBossMonNum)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] DeclarationNoti CInflWarManager::SetCompensationForDecalationOfWarWin# 120 %s MostWinBossMonNum(%d) MostMSWarStep(%d) \r\n"
			, CAtumSJ::GetInfluenceTypeString(i_byInflTy), nBossMonNum, byMSWarStep);
		pInflWarData->fInflHPRepairRate		= 0.0f;
		pInflWarData->fInflDPRepairRate		= 0.0f;
		pInflWarData->fInflSPRepairRate		= 0.0f;
		return;
	}

	SDB_INFLUENCE_WAR_INFO *pLastWinWarInfo = pInflWarData->GetInflWarInfoByKillBossMonsterUID(nBossMonNum, i_byInflTy);
	if(NULL == pLastWinWarInfo)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] DeclarationNoti CInflWarManager::SetCompensationForDecalationOfWarWin# 130 %s MostWinBossMonNum(%d) MostMSWarStep(%d) \r\n"
			, CAtumSJ::GetInfluenceTypeString(i_byInflTy), nBossMonNum, byMSWarStep);
		
		pInflWarData->fInflHPRepairRate		= 0.0f;
		pInflWarData->fInflDPRepairRate		= 0.0f;
		pInflWarData->fInflSPRepairRate		= 0.0f;
		return;
	}
	
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] DeclarationNoti CInflWarManager::SetCompensationForDecalationOfWarWin# 200 %s MostWinBossMonNum(%d) MostMSWarStep(%d) WarTimeStage(%d) \r\n"
		, CAtumSJ::GetInfluenceTypeString(i_byInflTy), nBossMonNum, byMSWarStep, pLastWinWarInfo->WartimeStage);
	
	// 2009-08-20 by cmkwon, ���� ȸ���� ���� ���� - �Ʒ��� ���� ������ �¸� ������ ����
	//pInflWarData->fInflHPRepairRate		= pInflWarData->GetCurInflWarInfo()->HPRepairRate;
	//pInflWarData->fInflDPRepairRate		= pInflWarData->GetCurInflWarInfo()->DPRepairRate;
	//pInflWarData->fInflSPRepairRate		= pInflWarData->GetCurInflWarInfo()->SPRepairRate;
	pInflWarData->fInflHPRepairRate		= pLastWinWarInfo->HPRepairRate;
	pInflWarData->fInflDPRepairRate		= pLastWinWarInfo->DPRepairRate;
	pInflWarData->fInflSPRepairRate		= pLastWinWarInfo->SPRepairRate;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT CInflWarManager::GetMSBossSummonValue(BYTE i_byInflTy, BYTE * o_byBeforeWinCheck)
/// \brief		�ܰ躰 �� �ý��� - ���� �ֱ⿡ ��ȯ�� �� ������ �������� ���� ��ȯ�Ǿ�� �Ǵ� ��UID�� ����Ѵ�.
/// \author		dhjin
/// \date		2009-03-10 ~ 2009-03-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CInflWarManager::GetMSBossSummonValue(BYTE i_byInflTy, BYTE * o_byBeforeWinCheck)
{
	INT SummonBossMonsterUID = 0;
	BYTE EndState = 0;
	this->m_DeclarationOfWar.GetBeforeMSWarEndInfo(i_byInflTy, &SummonBossMonsterUID, &EndState); // ��ȯ�Ǿ�ߵǴ� �� UID
	vectDB_INFLUENCE_WAR_INFO *pInflWarInfoList = this->GetInflWarInfoByInflType(i_byInflTy);
	if(NULL == pInflWarInfoList)
	{
		return FALSE;
	}

	if(0 == SummonBossMonsterUID)
	{
		*o_byBeforeWinCheck = MSBOSSSTEP_UNKNOWN_BEFOREWAR;
		return (*pInflWarInfoList)[4].BossMonsterUnitKind;
	}
	
	for(int i=0; i < pInflWarInfoList->size(); i++)
	{
		if(SummonBossMonsterUID == (*pInflWarInfoList)[i].BossMonsterUnitKind
			&& MSWAR_END_WIN == EndState)
		{
			*o_byBeforeWinCheck = MSBOSSSTEP_WIN_BEFOREWAR;
			return (*pInflWarInfoList)[i].WinBossMonsterUnitKind;
		}
		if(SummonBossMonsterUID == (*pInflWarInfoList)[i].BossMonsterUnitKind
			&& MSWAR_END_LOSS == EndState)
		{
			*o_byBeforeWinCheck = MSBOSSSTEP_LOSS_BEFOREWAR;
			return (*pInflWarInfoList)[i].LossBossMonsterUnitKind;
		}
		if(SummonBossMonsterUID == (*pInflWarInfoList)[i].BossMonsterUnitKind
			&& MSWAR_END_WIN != EndState
			&& MSWAR_END_LOSS != EndState)
		{// �� ������ ���� �ٿ����� ���Ͽ� ���ᰡ ���� �ʾҴٸ� 
			*o_byBeforeWinCheck = MSBOSSSTEP_UNKNOWN_BEFOREWAR;
			return SummonBossMonsterUID;
		}
	}
	return FALSE;
}

// 2013-01-16 by jhseol, ������ ���ѿ��� �ɸ��� �̸� ����� ����ž ������ �ǽð� ����
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarManager::LeaderChangeName(BYTE i_nInfluenceType, USHORT i_nLeaderType, char* i_stName)
/// \brief		������ ���ѿ��� �ɸ��� �̸� ����� ����ž ������ �ǽð� ����
/// \author		jhseol
/// \date		2013-01-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarManager::LeaderChangeName(BYTE i_nInfluenceType, USHORT i_nLeaderType, char* i_stName)
{
	if( COMPARE_INFLUENCE(i_nInfluenceType, INFLUENCE_TYPE_VCN) )
	{
		mt_auto_lock mtBCU(m_VCNInflWarData.GetmtlockPtr());
		if( COMPARE_RACE(i_nLeaderType, RACE_INFLUENCE_LEADER) )
		{
			util::strncpy(m_VCNInflWarData.InflLeaderCharacterName, i_stName, SIZE_MAX_CHARACTER_NAME);
		}
		else if( COMPARE_RACE(i_nLeaderType, RACE_INFLUENCE_SUBLEADER_1) )
		{
			util::strncpy(m_VCNInflWarData.InflSubLeader1CharacterName, i_stName, SIZE_MAX_CHARACTER_NAME);
		}
		else if( COMPARE_RACE(i_nLeaderType, RACE_INFLUENCE_SUBLEADER_2) )
		{
			util::strncpy(m_VCNInflWarData.InflSubLeader2CharacterName, i_stName, SIZE_MAX_CHARACTER_NAME);
		}
	}
	else if( COMPARE_INFLUENCE(i_nInfluenceType, INFLUENCE_TYPE_ANI) )
	{
		mt_auto_lock mtANI(m_ANIInflWarData.GetmtlockPtr());
		if( COMPARE_RACE(i_nLeaderType, RACE_INFLUENCE_LEADER) )
		{
			util::strncpy(m_ANIInflWarData.InflLeaderCharacterName, i_stName, SIZE_MAX_CHARACTER_NAME);
		}
		else if( COMPARE_RACE(i_nLeaderType, RACE_INFLUENCE_SUBLEADER_1) )
		{
			util::strncpy(m_ANIInflWarData.InflSubLeader1CharacterName, i_stName, SIZE_MAX_CHARACTER_NAME);
		}
		else if( COMPARE_RACE(i_nLeaderType, RACE_INFLUENCE_SUBLEADER_2) )
		{
			util::strncpy(m_ANIInflWarData.InflSubLeader2CharacterName, i_stName, SIZE_MAX_CHARACTER_NAME);
		}
	}
}
// end 2013-01-16 by jhseol, ������ ���ѿ��� �ɸ��� �̸� ����� ����ž ������ �ǽð� ����

// 2015-09-14 Future, added events on infl war start & end
void CInflWarManager::OnInfluenceWarStart()
{
	// 2015-09-14 Future, disabled Monster Maps during Influence war
	/*if (!g_pFieldGlobal->GetInfluenceWarDisabledMonsterMaps().empty())
	{
		INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL, T_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL, pMonsterDelete, SendBuff);

		// Send Monster Delete Requests to the NPC server for each disabled map
		for (auto itr = g_pFieldGlobal->GetInfluenceWarDisabledMonsterMaps().begin()
			; itr != g_pFieldGlobal->GetInfluenceWarDisabledMonsterMaps().end()
			; itr++)
		{

			CFieldMapChannel* pMapChannel = m_pFieldIOCP14->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(*itr, 0));

			if (pMapChannel)
			{
				pMonsterDelete->mapChann = pMapChannel->GetMapChannelIndex();
				pMonsterDelete->bAllFlag = TRUE;
				pMonsterDelete->bell1 = -1;
				pMonsterDelete->bell2 = -1;
				pMonsterDelete->excludeBell1 = -1;
				pMonsterDelete->excludeBell2 = -1;
				pMonsterDelete->bNotCreateMonster = TRUE;

				m_pFieldIOCP14->Send2NPCServerByTCP(SendBuff, MSG_SIZE(MSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL));
			}
			else
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] User specified disabled Monster Map by Influence War (Index: %d) not found!", *itr);
			}
		}
	}*/
}

// 2015-09-14 Future, added events on infl war start & end
void CInflWarManager::OnInfluenceWarEnd()
{
	// 2015-09-14 Future, disabled Monster Maps during Influence war
	/*if (!g_pFieldGlobal->GetInfluenceWarDisabledMonsterMaps().empty())
	{
		INIT_MSG_WITH_BUFFER(MSG_FN_MONSTER_CREATE_IN_MAPCHANNEL_BYVALUE, T_FN_MONSTER_CREATE_IN_MAPCHANNEL_BYVALUE, pMonsterCreate, SendBuf);

		// Send Moster Create Request to the NPC Server for each disabled map
		for (auto& mapidx : g_pFieldGlobal->GetInfluenceWarDisabledMonsterMaps())
		{
			CFieldMapChannel* pMapChannel = m_pFieldIOCP14->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(mapidx, 0));

			if (pMapChannel)
			{
				pMonsterCreate->mapChann = pMapChannel->GetMapChannelIndex();
				pMapChannel->Send2NPCServerW(SendBuf, MSG_SIZE(MSG_FN_MONSTER_CREATE_IN_MAPCHANNEL_BYVALUE));
			}
			
			else server::log(" [ERROR] User specified disabled Monster Map by Influence War (Index: %d) not found!", mapidx);
			
		}
	}*/
}

// 2015-12-17 Future, retrieval of the dominating nation using the NCP
BYTE CInflWarManager::GetDominatingInflByContributionPoints()
{
	// Get NCP of both nations
	int bcuNCP = GetContributionPoint(INFLUENCE_TYPE_VCN);
	int aniNCP = GetContributionPoint(INFLUENCE_TYPE_ANI);

	if (bcuNCP > aniNCP)
		return INFLUENCE_TYPE_VCN;
	else
		return INFLUENCE_TYPE_ANI;
		
}
