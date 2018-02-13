// NPCSkillManager.cpp: implementation of the CNPCSkillManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NPCSkillManager.h"

#include <timeapi.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNPCSkillManager::CNPCSkillManager()
{
	this->InitSkill();
}

CNPCSkillManager::~CNPCSkillManager()
{

}

void CNPCSkillManager::InitSkill()
{
	m_TimeLimitSkillList.clear();
}


/**********************************************************************************************************
**
**	������ ���� ��ų ���ó��.
**
**	Create Info : ??. ??. ??
**
**	Update Info : 2010. 06. 08. by hsLee. - ��ų ȿ�� ������ ���� �߰�. ( a_fIncreasePowerRatio );
**
***********************************************************************************************************/
void CNPCSkillManager::ApplyTimeLimitSkill( ITEM * i_UseSkill , const float a_fIncreasePowerRatio /*= 1.0f*/ )
{
	if(NULL == i_UseSkill
		|| ORBIT_SKILL_DEBUFF_TIMELIMIT == i_UseSkill->OrbitType
		|| ORBIT_SKILL_DOT_TIMELIMIT == i_UseSkill->OrbitType)
	{	// ���� �ڽſ��� �ɸ��� Ÿ�� ������ �������ش�.
		return;
	}

	DWORD dwCurTick = timeGetTime();
	
	mt_auto_lock mta(&m_TimeLimitSkillList);
	mtMonsterSkillInfoList::iterator itr = m_TimeLimitSkillList.begin();
	for(; itr != m_TimeLimitSkillList.end(); itr++)
	{
		if(i_UseSkill->ItemNum == itr->Skill->ItemNum)
		{	// �̹� ������̸� �ð� �ٽ� �ʱ�ȭ ��Ų��.
			itr->dwSkillUseTick	= dwCurTick;

			// 2010. 06. 08 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (�Ʊ� ���� �뷱�� ����.) - ���� ��ų �뷱�� ����.
			itr->fIncreasePowerRation = a_fIncreasePowerRatio;

			return;
		}
	}
	mta.auto_unlock_cancel();

	MONSTER_SKILL_INFO ApplySkill;
	util::zero(&ApplySkill, sizeof(MONSTER_SKILL_INFO));
	ApplySkill.Skill				= i_UseSkill;
	ApplySkill.dwSkillUseTick		= dwCurTick;

	// 2010. 06. 08 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (�Ʊ� ���� �뷱�� ����.) - ���� ��ų �뷱�� ����.
	ApplySkill.fIncreasePowerRation = a_fIncreasePowerRatio;

	m_TimeLimitSkillList.pushBackLock(ApplySkill);
}

ItemNum_t CNPCSkillManager::ReleaseTimeLimitSkill(ItemNum_t i_ExpireSkillNum)
{
	mt_auto_lock mta(&m_TimeLimitSkillList);
	mtMonsterSkillInfoList::iterator itr = m_TimeLimitSkillList.begin();
	for(; itr != m_TimeLimitSkillList.end(); itr++)
	{
		if(i_ExpireSkillNum == itr->Skill->ItemNum)
		{
			m_TimeLimitSkillList.erase(itr);
			return i_ExpireSkillNum;
		}
	}

	return FALSE;
}

BOOL CNPCSkillManager::CheckExpireTimeLimitSkill(ItemNum_t i_CheckExpireSkillNum)
{
	DWORD dwCurTick = timeGetTime();

	mt_auto_lock mta(&m_TimeLimitSkillList);
	mtMonsterSkillInfoList::iterator itr = m_TimeLimitSkillList.begin();
	for(; itr != m_TimeLimitSkillList.end(); itr++)
	{
		if(i_CheckExpireSkillNum == itr->Skill->ItemNum
			&& (dwCurTick - itr->dwSkillUseTick) < itr->Skill->Time )
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CNPCSkillManager::AllReleaseTimeLimitSkill(vector<ItemNum_t> * o_pReleaseSkillNumList)
{
	DWORD dwCurTick = timeGetTime();
	
	mt_auto_lock mta(&m_TimeLimitSkillList);
	mtMonsterSkillInfoList::iterator itr = m_TimeLimitSkillList.begin();
	while(itr != m_TimeLimitSkillList.end())
	{
		if((dwCurTick - itr->dwSkillUseTick) > itr->Skill->Time)
		{
			o_pReleaseSkillNumList->push_back(itr->Skill->ItemNum);
			itr = m_TimeLimitSkillList.erase(itr);
			continue;
		}
		itr++;
	}
}