// NPCSkillManager.h: interface for the CNPCSkillManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCSKILLMANAGER_H__AE98A3CB_794B_4F11_9D7D_642240EE0D13__INCLUDED_)
#define AFX_NPCSKILLMANAGER_H__AE98A3CB_794B_4F11_9D7D_642240EE0D13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct MONSTER_SKILL_INFO
{
	ITEM	*	Skill;
	DWORD		dwSkillUseTick;

	float		fIncreasePowerRation;		// ȿ�� ������.	// 2010. 06. 08 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (�Ʊ� ���� �뷱�� ����.) - ���� ��ų �뷱�� ����.

};
typedef mt_vector<MONSTER_SKILL_INFO>		mtMonsterSkillInfoList;

class CNPCSkillManager  
{
public:
	CNPCSkillManager();
	virtual ~CNPCSkillManager();
	
	void InitSkill();

	void ApplyTimeLimitSkill(ITEM * i_UseSkill , const float a_fIncreasePowerRatio = 1.0f );
	ItemNum_t ReleaseTimeLimitSkill(ItemNum_t i_ExpireSkillNum);
	BOOL CheckExpireTimeLimitSkill(ItemNum_t i_CheckExpireSkillNum);
	void AllReleaseTimeLimitSkill(vector<ItemNum_t> * o_pReleaseSkillNumList);
	
protected:
	mtMonsterSkillInfoList			m_TimeLimitSkillList;

};

#endif // !defined(AFX_NPCSKILLMANAGER_H__AE98A3CB_794B_4F11_9D7D_642240EE0D13__INCLUDED_)
