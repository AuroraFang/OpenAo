// SkillInfo.h: interface for the CSkillInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLINFO_H__7B90C53A_7DEA_4545_A5DC_CB8A64A154AD__INCLUDED_)
#define AFX_SKILLINFO_H__7B90C53A_7DEA_4545_A5DC_CB8A64A154AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumParam.h"


///////////////////////////////////////////////////////////////////////////////
/// \class		class CSkillInfo : public ITEM_SKILL 
///
/// \brief		CSkill �� ��ų ��ü�� �����ϴ� Ŭ�������, CSkillInfo�� ��ų �ϳ��� ���� ������ ����ϴ� Ŭ������.
///				��, CSkillInfo �� ��ų 1���� ���Ѵ�.
/// \author		jschoi
/// \version	
/// \date		2004-10-19 ~ 2004-10-19
/// \warning	
///////////////////////////////////////////////////////////////////////////////

class CSkillInfo : public ITEM_SKILL 
{
public:
	CSkillInfo(ITEM_SKILL* pItemSkill);
	virtual ~CSkillInfo();
	int		GetSkillState() { return m_dwState; }
	void	SetSkillState(int dwState) { m_dwState = dwState; }
	float	GetCheckEnableTime() { return m_fCheckEnableTime; }
	float	GetCheckPrepareTime() { return m_fCheckPrepareTime; }
	float	GetCheckReattackTime() {return m_fCheckReattackTime; }
	void	SetCheckEnableTime() { m_fCheckEnableTime = ItemInfo->Time; }
	void	SetCheckReattackTime() { m_fCheckReattackTime = ItemInfo->ReAttacktime; }
	void	SetReattackTime(float fReattackTime) { m_fCheckReattackTime = fReattackTime; }
	void	SetInitReattackTime() { m_fCheckReattackTime = 0.0f; }	// 2005-11-26 by ispark, ReattackTime �ʱ�ȭ
	void	ChangeSkillState(int dwState, int nTempSkillItemNum = 0); 
	// 2010. 06. 18 by jskim ���� ��� ��ų ��Ʈ�� ����
	//void	UseSkillFromServer();					// server���� use skill ���
	void	UseSkillFromServer(char* Name = NULL, BOOL isUser = TRUE);
	//end 2010. 06. 18 by jskim ���� ��� ��ų ��Ʈ�� ����
	void	PrepareSkillFromServer();														// 2006-12-20 by ispark, server���� prepare skill ���
	void	Tick(float fElapsedTime);
	void	Init();
	BOOL	IsEnableStateForReUsing();
	void	ReUsingSkill();
	void	SendUseSkill();
	void	SendCancelSkill(int nAttackSkillItemNum = 0);									// 2006-12-12 by ispark, ����
	void	SetParamFactor();
	void	ReleaseParamFactor();
	void	SetCheckAckWaitingTime(float fWaitTime) { m_fCheckAckWaitingTime = fWaitTime; } // 2005-12-05 by ispark
	void	SetbApprovalEnd(BOOL bApproval) { m_bApprovalEnd = bApproval; }					// 2006-07-30 by ispark
	BOOL	GetbApprovalEnd() { return m_bApprovalEnd; }									// 2006-07-30 by ispark
	float	GetCheckAckWaitingTime() { return m_fCheckAckWaitingTime; }						// 2006-10-26 by ispark

	void	SetClientIndex(ClientIndex_t AttackIndex, ClientIndex_t TargetIndex);			// 2006-09-14 by ispark
	ClientIndex_t GetAttackIndex() { return m_AttackIndex; }
	ClientIndex_t GetTargetIndex() { return m_TargetIndex; }
	BOOL	IsMustOneTargetSkill(BYTE i_bySkillTargetType);

	// 2008-12-26 by bhsohn ����� ��� ���� ����
	BOOL	IsSkillEnable();
	void	SetSkillEnable(BOOL i_bSkillEnable);
	// end 2008-12-26 by bhsohn ����� ��� ���� ����

	// 2013-05-07 by bhsohn ��������Ʈ ������ �ý���
	void DisableTimelimitSkill();
	// END 2013-05-07 by bhsohn ��������Ʈ ������ �ý���

	// 2010. 10. 11. ������ Ÿ�� ������ ��ų ���� ����.
public :

	inline void ResetClickTargetInfo ( void ) { m_ClickTargetIndex = 0; m_nClickTargetUID = 0; }

	inline void SetClickTagetIndex ( const ClientIndex_t a_UpdateClickTargetIndex ) { m_ClickTargetIndex = a_UpdateClickTargetIndex; }
	inline ClientIndex_t GetClickTargetIndex ( void ) { return m_ClickTargetIndex; }

	inline void SetClickTargetUID ( const UID32_t a_nUpdateClickTragetUID ) { m_nClickTargetUID = a_nUpdateClickTragetUID; }
	inline UID32_t GetClickTargetUID ( void ) { return m_nClickTargetUID; }

	// End 2010. 10. 11. ������ Ÿ�� ������ ��ų ���� ����.

protected:
	void	Clone(ITEM_SKILL *pItemSkill);
	void	TickPermanent(float fElapsedTime);
	void	TickClick(float fElapsedTime);
	void	TickTimelimit(float fElapsedTime);
	void	TickToggle(float fElapsedTime);
	void	TickCharging(float fElapsedTime);
	BOOL	CheckUsingSkillToMeAtPrepare();

protected:
	float	m_fCheckWaitingTime;		// ������ ������ ��ٸ��� �ð�
	float	m_fCheckWaitingPrepareTime;	// ������ ������ ��ٸ��� �ð�
	float	m_fCheckReattackTime;		// �� ���� ���� �ð�
	float	m_fCheckEnableTime;			// �ð� �� ��ų�� �����ȴ�.
	float	m_fCheckPrepareTime;		// �ߵ� �ð�
	float	m_fCheckAckWaitingTime;		// 2005-12-05 by ispark, ���ѵ� �ð� üũ(�ð��� ������ �ϸ� �ڵ� ��ų ���)
	int		m_dwState;					// ��ų ����
	BOOL	m_bSetParamFactor;			// ParamFactor�� ����Ǿ�����.
	BOOL	m_bApprovalEnd;				// 2006-07-30 by ispark, ���� ����

	ClientIndex_t	m_AttackIndex;
	ClientIndex_t	m_TargetIndex;		// target�� ���� skill�� ��� 0

	// 2008-12-26 by bhsohn ����� ��� ���� ����
	BOOL	m_bSkillEnable;

	// 2010. 10. 11. ������ Ÿ�� ������ ��ų ���� ����.
	ClientIndex_t	m_ClickTargetIndex;
	UID32_t			m_nClickTargetUID;

};

#endif // !defined(AFX_SKILLINFO_H__7B90C53A_7DEA_4545_A5DC_CB8A64A154AD__INCLUDED_)
