// FieldMonster.h: interface for the CFieldMonster class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIELDMONSTER_H__4AF49C24_9155_4274_A7F2_E3557E7D59B9__INCLUDED_)
#define AFX_FIELDMONSTER_H__4AF49C24_9155_4274_A7F2_E3557E7D59B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Monster.h"
#include "FieldIOCP.h"


typedef mt_list<MONSTER_DROP_ITEM_GENERAL>		mtlistMonsterDropItem;

////////////////////////////////////////////////////////////////////////////////
// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 
struct BARRIER_SKILL_INFO {
	BOOL			bBarrierSkillHave;
	BOOL			bBarrierSkillUsing;
	HPHitRate_t		HitRate;
	DWORD			UseTick;
	DWORD			ReAttackTick;
	DWORD			dwLastTick;	
	INT				SkillNum;
};
struct IMMEDIATE_BARRIER_SKILL_INFO {
	// 2009-09-09 ~ 2010-01-15 by dhjin, ���Ǵ�Ƽ - ���� ��� ���� �踮�� ��ų(��޹��⸸�ߵ�)
	BOOL			bBarrierSkillUsing;
	DWORD			UseTick;
	DWORD			dwLastTick;	
	INT				SkillNum;
};

class CCityWar;
class CTriggerFunction;				// 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
class CTriggerFunctionCrystal;		// 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���

class CFieldMonster : public CMonster
{
	friend class CFieldMapChannel;
	friend class CFieldIOCPSocket;

public:
	CFieldMonster();
	virtual ~CFieldMonster();

	///////////////////////////////////////////////////////////////////////////
	// Property
	USHORT GetSendCountsMonsterMoveOK(void);
	void SetCityWarFieldMonster(CCityWar *i_pWar);

	///////////////////////////////////////////////////////////////////////////
	// Method
	// �� ��Ŷ ���� �ý��� ����
	USHORT IncreaseSendCountsMonsterMoveOK(void);

	float IncreaseMonsterCurrentHP(float i_fValue);
	// 2010-03-18 by dhjin, ���� �� ���� ó�� ��ƾ ���� - DecreaseMonsterCurrentHP �� ����
//	float DecreaseMonsterCurrentHPByCharacter(float i_fValue, CFieldIOCPSocket *i_AttackFISock, vectActionInfo *i_pAInfoAggroList);
//	float DecreaseMonsterCurrentHPByMonster(float i_fValue);	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���Ϳ��� ���� ����
//	float DecreaseMonsterCurrentHPByPenalty(float i_fValue);	// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - �г�Ƽ ������ ���Ͽ� ���� HP ����
	float DecreaseMonsterCurrentHP(float i_fValue, BOOL i_bPenalty=FALSE);	// 2010-03-18 by dhjin, ���� �� ���� ó�� ��ƾ ���� -
	void ProcessingDamagedMonsterByCharacter(float i_fValue, CFieldIOCPSocket *i_AttackFISock, vectActionInfo *i_pAInfoAggroList);		// 2010-03-18 by dhjin, ���� �� ���� ó�� ��ƾ ���� -

	// �̵� ����
	D3DXVECTOR3 GetCurrentPosition(void);

	ITEM * FindItemPtrWithItemNum(INT i_uiItemNum);
	ITEM * FindItemPtrByOrBitTyp(BYTE i_byOrBitType);

	// ���Ľ� item ó�� ����
	void LockDropItemList(void);
	void UnlockDropItemList(void);
// 2007-12-07 by cmkwon, ������� �ʴ� �Լ� ��
//	void CreateDropItem(CFieldIOCP *pFieldIOCP, CFieldIOCPSocket *i_pFISockGive2MonsterTopDamage);

	// 2006-11-07 by cmkwon
	BOOL SetAttackerCliIdx(ClientIndex_t i_cliIdx);
	ClientIndex_t GetAttackerCliIdx(void);
// 2009-12-11 by cmkwon, ������ ��׷η� Ÿ���� �����ϴ� ���� ���� - CMonster Ŭ������ �̵�
//	BOOL IsValidMonster(BOOL i_bLiveCheck=TRUE);

	// virtual
	virtual void ResetMonster(void);
	virtual void InserttoAttackedInfoList(ActionInfo *i_pAttackedInfo);
	
	// ���Ľ� item ó�� ����
	mtlistMonsterDropItem		m_mtlistDropItem;

	mtvectClientIndex_t			m_mtVectCltIdxForMonsterMoveOK;
	mtvectClientIndex_t			m_mtVectCltIdxForExcludeMonsterMoveOK;

	///////////////////////////////////////////////////////////////////////////////
	// 2007-06-08 by cmkwon, 2������ ����Ȯ��,���ݷ�,�Ǿ�� ���� �ý��� ����
	mtvectSATTACK_PARAMETER		m_mtvectSAttackParameterList;		// 2007-06-08 by cmkwon, 2�� T_FC_BATTLE_ATTACK���� ����
	BOOL APInsertAttackParameter(SATTACK_PARAMETER *i_pAttParam);
	SATTACK_PARAMETER *APFindAttackParameterNoLock(UID16_t i_WeaponIndex);
	BOOL APPopAttackParameter(SATTACK_PARAMETER *o_pAttParam, UID16_t i_WeaponIndex);
	void APCalcAttckParameter(SATTACK_PARAMETER *o_pAttParam, ITEM *i_pWeaponItemInfo, UID16_t i_WeaponIndex);

	///////////////////////////////////////////////////////////////////////////////	
	// 2010-04-05 by cmkwon, ����2�� M2M 2�� ���� ���� ó�� - 
	BOOL APPopOverLifeTimeAttackParameterList(vectSATTACK_PARAMETER *o_pOverTimeAttParamList);

	//////////////////////////////////////////////////////////////////////////
	// 2008-03-28 by dhjin, ���� ���� ǥ�� ��ȹ�� -
	void HPRateCalcByMSWarInfoDisPlay(float i_fMonsterMaxHP, float i_fMonsterBeforeHP, float i_fMonsterAfterHP);			// 2008-03-28 by dhjin, ���� ���� ǥ�� ��ȹ�� - ���� ���� ǥ�ÿ� ���õ� ���� HPRate����Ͽ� ���� ����
	BOOL MSMonsterCheckWithoutMSBossMonster();			// 2008-03-28 by dhjin, ���� ���� ǥ�� ��ȹ�� - ���� ���� ǥ�ÿ� ���õ� ���� ���� üũ

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 
	void MonsterSkillApply(MSG_FN_BATTLE_ATTACK_SKILL * i_pMsg, ITEM * i_nSkill);
	void SetBarrier(ITEM * i_pSkill);
	BOOL CheckBarrierHave();	// �踮�� ��ų ������ �ִ��� üũ 
	BOOL CheckBarrierUsing();	// �踮�� ��ų ����� üũ
	BOOL BarrierUse();			// �踮�� ��ų ���
	void SetImmediateBarrier(ITEM * i_pSkill);		// 2009-09-09 ~ 2010-01-15 by dhjin, ���Ǵ�Ƽ - ���� ��� ���� �踮�� ��ų(��޹��⸸�ߵ�) ����
	BOOL CheckImmediateBarrierUsing();	// 2009-09-09 ~ 2010-01-15 by dhjin, ���Ǵ�Ƽ - ���� ��� ���� �踮�� ��ų(��޹��⸸�ߵ�) ����� üũ

	CFieldMapChannel * GetCurrentMapChannelMonster();	
	ItemNum_t GetCurrentAttackItemNum();		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�
	ItemNum_t m_CurrentAttackItemNum;					// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���ӷα� �߰�

	void ProcessingInserttoAttackedInfoList(float i_fValue, CFieldIOCPSocket *i_AttackFISock, vectActionInfo *i_pAInfoAggroList);	// 2010-03-18 by dhjin, ���� �� ���� ó�� ��ƾ ����
	
	// start 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
	void InsertTriggerFunction(CTriggerFunction *pTriggerFunction);
	BOOL IsTriggerFunction();
	// end 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���

protected:
	D3DXVECTOR3					m_BeforePosition;					//
	DWORD						m_dwTimeLastMoved;					// ������ ������ �̵� �ð�

	// �� ��Ŷ ���� �ý���
	USHORT						m_usSendCountsMonsterMoveOK;		// Move ��Ŷ ���� ī��Ʈ

	CFieldMapChannel			*m_pCurrentFieldMapChannelMonster;	// CFieldMapChannel�� �����ڿ��� �Ҵ��ؾ� ��

	// ������ ���� ó��
	DWORD						m_dwTimeCreatedTick;
	CCityWar					*m_pCityWar2;
	DWORD						m_dwLastTickInfluenceBossAttackedMsg;	// 2006-01-20 by cmkwon, ������ ���� ���Ͱ� ���ݴ��ϴ� �޽����� ���� ������ �ð�

	// 2006-11-07 by cmkwon
	ClientIndex_t				m_nAttackerCliIdx;						// 2006-11-07 by cmkwon
	DWORD						m_dwLastTickSetAttackerCliIdx;			// 2006-11-07 by cmkwon

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 
	BARRIER_SKILL_INFO			m_bBarrierInfo;							// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �踮��
	IMMEDIATE_BARRIER_SKILL_INFO	m_ImmediateBarrier;					// 2009-09-09 ~ 2010-01-15 by dhjin, ���Ǵ�Ƽ - ���� ��� ���� �踮�� ��ų(��޹��⸸�ߵ�)

	mt_vector<CTriggerFunction *>	m_mtVectTriggerFunctionPtr;			// 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���	
};

#endif // !defined(AFX_FIELDMONSTER_H__4AF49C24_9155_4274_A7F2_E3557E7D59B9__INCLUDED_)
