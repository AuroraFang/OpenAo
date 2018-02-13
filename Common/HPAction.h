// HPAction.h: interface for the CHPAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HPACTION_H__6E1DDDE4_5068_41C9_BD4C_2032B4B4248E__INCLUDED_)
#define AFX_HPACTION_H__6E1DDDE4_5068_41C9_BD4C_2032B4B4248E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHPAction  
{
public:
	CHPAction();
	virtual ~CHPAction();

	void Init();
	void InitHPActionListByDB(vectHPAction * i_pvectHPActionDBValue);			// DB������ �ʱ�ȭ
	void InitHPActionAttackHPRateList(HPACTION * i_pHPAction);					// HP���� ���� ���� ���� ����Ʈ�� ���� �����.
	void InitHPActionTalkHPRateList(HPACTION * i_pHPAction);					// HP�� ���� ��ȭ ����Ʈ�� ���� �����.
	void InitHPActionTalkDamagedRandomList(HPACTION * i_pHPAction);				// �ǰݽ� ��ȭ ����Ʈ�� ���� �����.
	void InitHPActionTalkCreate(HPACTION * i_pHPAction);						// ������ ��ȭ ���� ���� �����.
	void InitHPActionTalkDead(HPACTION * i_pHPAction);							// ������ ��ȭ ���� ���� �����.
	void InitHPActionTalkTargetChange(HPACTION * i_pHPAction);					// Ÿ�� ���� �� ��ȭ ���� ���� �����.
	BOOL CheckValidSizeAttackItemIdx();
	BOOL CheckValidSizeTalkCreate();
	BOOL CheckValidSizeTalkDead();
	BOOL CheckValidSizeTalkHPRate();
	BOOL CheckValidSizeTalkDamagedRadom();
	BOOL CheckValidSizeTalkTargetChange();
	BOOL GetAttackItemIdxHPRate(MonHP_t i_CurrentMonHP, ItemIdx_t * o_pAttackItemIdx, INT * o_pSelectVectIdx);	// HP���� ���� ���� ���;����۹迭�ε��� ��������
	BOOL GetNextAttackItem(ItemIdx_t * o_pAttackItemIdx);								// ���� ���� ���� �ִ���
// 2009-09-09 ~ 2010-01-13 by dhjin, ���Ǵ�Ƽ - �� ���������� ���� ��� �����ϰ� ����, �ذ� ���� ���� HPActionItem ���� ��� ī��Ʈ�� ���δ�.
//	void SetSuccessAttackItemIdxHPRate(INT i_SelectVectIdx);							// HP���� ���� ���� ���� ó��
	void SetSuccessAttackItemIdxHPRate();												// HP���� ���� ���� ���� ó��
	BOOL GetTalkCreate(HPACTION_TALK_HPRATE * o_pTalkHPRate);							// ������ ��ȭ ��������
	BOOL GetTalkDead(HPACTION_TALK_HPRATE * o_pTalkHPRate);								// �׾��� ��� ��ȭ ��������
	BOOL GetTalkHPRate(MonHP_t i_CurrentMonHP, HPACTION_TALK_HPRATE * o_pTalkHPRate);	// HP���� ���� ��ȭ ��������
	BOOL GetTalkDamagedRandom(HPTalk_t * o_pTalk);										// �ǰݽ� ��ȭ ��������	
	BOOL GetTalkTargetChange(HPTalk_t * o_pTalk);										// Ÿ�� ���� �� ��ȭ ��������	
	void EraseHPActionByUseItemArrayIdx(ItemIdx_t i_UseItemArrayIdx);
	void SetHPTalkAttack(ItemIdx_t i_ItemArrayIdx, INT i_ItemNum);							// ���ݿ� ���� ��ȭ ����
	BOOL GetPreHPTalkAttack(ItemIdx_t i_AttackItemNum, HPTalk_t * o_pPreTalk, MSec_t * o_pPreHPCameraTremble);			// �غ� ���ݽ� ��ȭ �� ī�޶� ���� ��������
	BOOL GetHPTalkAttack(ItemIdx_t i_AttackItemNum , HPTalk_t * o_pTalk, MSec_t * o_pHPCameraTremble);					// ���ݽ� ��ȭ �� ī�޶� ���� ��������
	
protected:
	vectHPAction					m_vectHPAction;
	vectHPActionAttackHPRate		m_vectHPActionAttackHPRate;
	vectHPActionTalkHPRate			m_vectHPActionTalkHPRate;
	vectHPActionTalkDamagedRandom	m_vectHPActionTalkDamagedRandom;
	vectHPActionTalkAttack			m_vectHPActionTalkAttack;
	
	ItemIdx_t						m_NextAttackItemIdx;
	HPACTION_TALK_HPRATE			m_CreateTalk;
	HPACTION_TALK_HPRATE			m_DeadTalk;
	HPTalk_t						m_TargetChangeTalk[SIZE_MAX_HPTALK_DESCRIPTION];
	HPActionUID_t					m_HPActionAttackUID;			// 2009-09-09 ~ 2010-01-13 by dhjin, ���Ǵ�Ƽ - �� ���������� ���� ��� �����ϰ� ����
	HPActionUID_t					m_HPActionAttackTalkUID;		// 2009-09-09 ~ 2010-01-13 by dhjin, ���Ǵ�Ƽ - �� ���������� ���� ��� �����ϰ� ����
};

#endif // !defined(AFX_HPACTION_H__6E1DDDE4_5068_41C9_BD4C_2032B4B4248E__INCLUDED_)
