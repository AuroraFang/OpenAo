//#include "stdafx.h"
#include "Quest.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuest::CQuest()
{
	Reset();
}

void CQuest::Reset()
{
	// Basic Info
	QuestIndex = 0;
	memset(QuestName, 0x00, SIZE_MAX_QUEST_NAME);
	QuestEpisodeType				= QUEST_EPISODE_TYPE_1AND2;		// 2008-06-16 by cmkwon, EP3 ����Ʈ ���� ����Ʈ�� ���� - �⺻��
	QuestPartyType			= QUEST_PARTYTYPE_NONE;			// 2008-12-02 by dhjin, �̼Ǹ�����
	QuestKind			= QUEST_KIND_NORMAL;
	QuestType = 0;
	QuestStartType = 0;
	QuestEndType = 0;
	QuestPartyMembers				= 0;		// 2006-03-24 by cmkwon
	QuestNPCInfo.Reset();
	memset(QuestDescription, 0x00, SIZE_MAX_NPC_TALK_STRING);
	EventMapIndex = 0;
	NextQuestIndex = 0;
	PartyQuestMemberCount = 0;
	DropItemToAllAttackCharacter	= FALSE;
	TimeLimitInMinutes				= 0;
	IsDiscardable					= TRUE;		// 2006-07-20 by cmkwon, �⺻�� �̼� ���� ����
	IsCityWar						= FALSE;
	IsNeverCompletionByUser			= FALSE;
	IsDropItemToAllPartyMembers		= FALSE;	// 2009-01-13 by cmkwon, �̼� ������ ��Ƽ�� ��ο��� �ֱ� ��ɾ� �߰� - CQuest�� ��� ���� �ʱ�ȭ


	// Start Requirements
	ReqUnitKind = 0;
	ReqLevel.Reset();
	ReqExperience = 0;
	ReqAttackPart.Reset();
	ReqDefensePart.Reset();
	ReqFuelPart.Reset();
	ReqSoulPart.Reset();
	ReqShieldPart.Reset();
	ReqDodgePart.Reset();
	ReqQuestIndexVector.clear();
	ReqRecvQuestIndexVector.clear();
	ReqItemVector.clear();
	ReqGuild = 0;
	ReqInfluenceType		= INFLUENCE_TYPE_ALL_MASK;
	ReqStartArea.Reset();

	// Start Action
	StartPayItemVector.clear();
	StartDelItemVector.clear();
	StartMapIndex			= 0;
	StartPartyWarpFlag		= FALSE;

	// Completion Requirements
	TargetItemVector.clear();
	TargetMonsterItemVector.clear();
	TargetMeetNPCInfo.Reset();
	TargetMeetObjectIndex = 0;
	TargetMeetObjectMapArea.Reset();
	TargetDeadByMonsterUniquenumber		= 0;
	TargetMeetMapArea.Reset();
	TargetMonsterVector.clear();
	TargetMixItemTarget.Reset();				// 2007-04-02 by cmkwon

	// Completion Action
	EndDeleteItemVector.clear();
	EndPayItemVector.clear();
	EndMissionMasterPayItemVector.clear();		// 2008-12-02 by dhjin, �̼Ǹ�����
	EndRandomPayItemVector.clear();
	EndPayMixItemVector.clear();
	EndPayBonusStat = 0;
// 2005-11-15 by cmkwon, ������
//	EndPayBonusSkillPoint = 0;
	EndPayFame = 0;
	EndPayStatus = 0;
	EndPayLevel = 0;
	EndPayExperience = 0;
	EndPayWarpMapIndex = 0;
	EndPayPropensity = 0;
	EndCinemaNum = 0;
	EndInfluenceType		= 0;

	// Quest�� Drop Item List
	vecQuestDropItem.clear();

	vecQuestDropCharacter2Item.clear();

	StartPayItemWeight = 0.0f;
	EndPayItemWeight = 0.0f;
	EndMissionMasterPayItemWeight = 0.0f;	// 2008-12-02 by dhjin, �̼Ǹ�����
}

CQuest *CQuest::Clone()
{
	CQuest *pQuest = new CQuest;
	
	*pQuest = *this;

	return pQuest;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CQuest::IsExistTargetMonsterVector(INT i_monUniqueNumber)
/// \brief		
/// \author		cmkwon
/// \date		2005-10-25 ~ 2005-10-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CQuest::IsExistTargetMonsterVector(INT i_monUniqueNumber)
{
	if(false == TargetMonsterVector.empty())
	{
		vectMONSTER_W_COUNT::iterator itr(TargetMonsterVector.begin());
		for(; itr != TargetMonsterVector.end(); itr++)
		{
			if(itr->MonsterUniqueNumber == i_monUniqueNumber)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CQuest::IsPartyWarp(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-03-24 ~ 2006-03-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CQuest::IsPartyWarp(void)
{
	if(0 == StartMapIndex)
	{
		return FALSE;
	}

	if(StartPartyWarpFlag)
	{
		return TRUE;
	}

	return this->IsPartyQuest();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CQuest::IsPartyQuest(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-03-24 ~ 2006-03-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CQuest::IsPartyQuest(void)
{
	if(0 == QuestPartyMembers)
	{
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CQuest::IsFirstScenarioQuest(BYTE i_byInflTy)
/// \brief		
/// \author		cmkwon
/// \date		2006-08-07 ~ 2006-08-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CQuest::IsFirstScenarioQuest(BYTE i_byInflTy)
{
	if(FALSE == COMPARE_INFLUENCE(ReqInfluenceType, i_byInflTy))
	{
		return FALSE;
	}

	if(QUEST_KIND_SCENARIO != QuestKind)
	{
		return FALSE;
	}

	if(false == ReqQuestIndexVector.empty())
	{
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CQuest::IsEndTypeTimeOver(void)
/// \brief		// 2007-12-06 by cmkwon, �ð����� ����Ʈ �������� �ð� üũ�ϰ� ���� - CQuest::IsEndTypeTimeOver() �߰�
/// \author		cmkwon
/// \date		2007-12-06 ~ 2007-12-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CQuest::IsEndTypeTimeOver(void)
{
	if(QUEST_END_TYPE_TIME_OVER != this->QuestEndType)
	{
		return FALSE;
	}

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CQuest::IsCompleteEndTypeTimeOver(int i_nSpentTimeInSec)
/// \brief		// 2007-12-06 by cmkwon, �ð����� ����Ʈ �������� �ð� üũ�ϰ� ���� - CQuest::IsCompleteEndTypeTimeOver() �߰�
/// \author		cmkwon
/// \date		2007-12-06 ~ 2007-12-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CQuest::IsCompleteEndTypeTimeOver(int i_nSpentTimeInSec)
{
	if(FALSE == this->IsEndTypeTimeOver())
	{
		return FALSE;
	}

	if(this->TimeLimitInMinutes * 60 > i_nSpentTimeInSec)
	{
		return FALSE;
	}

	return TRUE;
}
