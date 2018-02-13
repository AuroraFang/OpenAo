// Skill.h: interface for the CSkill class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILL_H__5A26A431_9BF7_4616_8ADB_BBBEF7A4D820__INCLUDED_)
#define AFX_SKILL_H__5A26A431_9BF7_4616_8ADB_BBBEF7A4D820__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSkillInfo;
class CAppEffectData;
class CUnitData;
class CSkill  
{
	friend class CINFSkill;
public:
	CSkill();
	virtual ~CSkill();

	void		Tick(float fElaspedTime);
	CSkillInfo*	PutSkill(ITEM_SKILL* pItemSkill);		// ��ų �߰�
	void		DeleteSkill(LONGLONG UniqueNumber);		// ��ų ����
	void		UseSkill(CSkillInfo* pItemSkill);		// ��ų ���
	void		ReleaseSkill(CSkillInfo* pItemSkill);	// ��ų ��� ��ü
	void		CancelSkill(int nSkillItemNum, BOOL i_bEnemyUsingSkillCancel);	
	void		CancelSkillByBaseNum(int nSkillItemNum, BOOL i_bEnemyUsingSkillCancel);	
	
	ITEM*		FindItem(int nSkillItemNumber);			// SkillItemNumber �� ���� ITEM* �� ã���ִ� �Լ�
	CSkillInfo* FindItemSkill(int nSkillItemNumber);	// SkillItemNumber �� ���� ITEM_SKILL* �� ã���ִ� �Լ�
	CSkillInfo* FindUsingSkillInfo(int nSkillItemNum);	// ���� ������� ��ų
	CSkillInfo* FindEnemyUsingSkillForMeInfo(int nSkillItemNum);	//���� ���� ������ ����� ��ų.
	CSkillInfo* FindEnemyUsingSkillForMeInfoByBaseNum(int nSkillItemNum);	//���� ���� ������ ����� ��ų.
	CSkillInfo* FindUsingSkillInfoByBaseNum(int nSkillItemNum);	// ���� ������� ��ų�� ���� BASENUM�� ���� ��ų
	CSkillInfo* FindUsingSkillExistInfoByBaseNum(int nSkillItemNum);		//���� �ߵ����� ��ų.
	int			FindItemSkillLevelByBaseNum(int nSkillBaseNum); // 0 : �������� �ʴ� ��ų
	void		DisableSkillEffect(CUnitData* pUnit, int nEffectNum);

	BOOL		DeleteUsingSkillInfo(int nSkillItemNum);
	BOOL		DeleteUsingSkillInfoByBaseNum(int nSkillItemNum);
	BOOL		DeleteEnemyUsingSkillInfo(int nSkillItemNum);
	BOOL		DeleteEnemyUsingSkillInfoByBaseNum(int nSkillItemNum);

	// 2007-02-07 by dgwoo ��ĵ ��ų����.
	BOOL		ISScanSkill()								{return m_bScan;}
	
	// 2007-04-19 by bhsohn ��ġ���� ������ �߰�
	//void		SetScanSkill(BOOL bScan)					{m_bScan = bScan;}
//	float		GetScanRange()								{return m_fScanRange;}
//	void		SetScanRange(float ScanRange)				{m_fScanRange = ScanRange;}	
	void		SetScanSkill(int nIdx, BOOL bScan);
	float		GetScanRange(int nIdx);
	void		SetScanRange(int nIdx, float ScanRange);

	void		SetScanPosition(int nIdx, D3DXVECTOR3	vScanPosition);
	BOOL		IsCanSeeInvisible(D3DXVECTOR3 vPos);						// �κ����� ����� ���� �ִ��� �Ǵ�

	
	


	char*		GetSkillName(int nSkillItemNumber);			// SkillItemNumber �� ��ų�� �̸��� ����
	void		PutSkillFromWearItem(BYTE nItemKind);	// ������ ��ü�� nImteKind�� ���õ� ��ų Ȱ��ȭ(�нú�)
	BOOL		GetSmartSpeedState() { return m_bSmartSpeed; }
	void		SetSmartSpeedState(BOOL bSmartSpeed) { m_bSmartSpeed = bSmartSpeed;	}
	int			GetSkillPatternType() { return m_nSkillPatternType; }
	void		SetSkillPatternType(int nSkillPatternType) { m_nSkillPatternType = nSkillPatternType; }

	// 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
	void		SetMonsterHoldState( const BOOL bHold) { m_bMonsterHold = bHold; }
	BOOL		GetMonsterHoldState( void ) { return m_bMonsterHold; }
	// end 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����

	void		DeleteSkillFromWearItem(BYTE nItemKind);// ������ ��ü�� nItemKind�� ���õ� ��ų ����
	void		ReleaseAllUsingSkillWithOutPermanent();	// Permanent�� �����ϰ� ������� ��� ��ų�� �����Ѵ�.
	void		ReleaseAllUsingToggleSkill();			// ������� ��� Toggle ��ų�� �����Ѵ�.
	void		ReleaseGroundSkill();					// ������� ��� ���󿡼� ���Ǵ� ��ų�� �����Ѵ�.
	void		ReleaseAllChargingSkill();				// ������� ��� Charging ��ų�� �����Ѵ�.
	void		ReleseSkill(int nType, INT nAttackSkillItemNum = 0);	// ��ų�� �����Ѵ�.

	BOOL		IsSatisfyReqItemKind(BYTE nReqItemKind, BYTE nItemKind);

	void		SendPrepareUseSkill(CSkillInfo* pItemInfo);	// ������ ��ų�� �ߵ����� �˷��ش�.
	void		SendPrepareCancelSkill();				// ������ �ߵ��� ��ų�� ��ҵ��� �˷��ش�.
	void		SetChargingShotState(int nChargingShotState) { m_nChargingShotState = nChargingShotState; }
	int			GetChargingShotState() { return m_nChargingShotState; }

	void		CheckChargingShotSkill();				// ���� ������� ��¡�� ��ų ��ȣ�� �����Ѵ�.
	UINT		GetChargingShotTypeSkillNum();			// 2006-12-06 by ispark, í¡�� Ÿ�� ��ų �ѹ� ��������
	void		SetPrimarySkill(CSkillInfo* pSkillInfo) { m_pPriSkillInfo = pSkillInfo; }	// ���� ������� 1������ Skill
	CSkillInfo*	GetPrimarySkill() { return m_pPriSkillInfo; }								// ���� ������� 1������ Skill
	void		SetSecondarySkill(CSkillInfo* pSkillInfo) { m_pSecSkillInfo = pSkillInfo; }	// ���� ������� 2������ Skill
	CSkillInfo*	GetSecondarySkill() { return m_pSecSkillInfo; }								// ���� ������� 2������ Skill

	void			SetSkillTargetState(BOOL bSkillTargetState) { m_bSkillTargetState = bSkillTargetState; }
	BOOL			GetSkillTargetState() { return m_bSkillTargetState; }
	void			SetTargetIndex(ClientIndex_t nTargetIndex) { m_nTargetIndex = nTargetIndex; }
	ClientIndex_t	GetTargetIndex() { return m_nTargetIndex; }
	void			SetCharacterUID(UID32_t nCharacterUID) { m_nCharacterUID = nCharacterUID; }
	UID32_t			GetCharacterUID() { return m_nCharacterUID; }

	BOOL		FindTargetForSkill();					// ���콺 �����͸� �������� ���� ����� �� �ֺ��� Enemy�� ���Ѵ�.(Ŭ��)
	void		ReleaseAllPrepareTargetOneSkill();		// �ߵ����� ��� TARGET TYPE ONE ��ų�� �����Ѵ�.
	void		ReleasePrepareTargetOneSkill(int nSkillNumber);			//Ÿ���� ��ų�� �����ϰ� �ʱ�ȭ�Ѵ�.
	void		SafeDeleteSkillInfo(CSkillInfo* pSkillInfo);	
	void		EraseUsingSkill(CSkillInfo* pSkillInfo, BOOL bEnemyDelete = FALSE);

	// 2013-03-27 by bhsohn DestParam �ڷ��� ����
//	BOOL		FindDestParamUseSkill(ITEM * pItem, BYTE byDestParameter);
	BOOL		FindDestParamUseSkill(ITEM * pItem, DestParam_t byDestParameter);
	// END 2013-03-27 by bhsohn DestParam �ڷ��� ����
	void		ReleaseAllUsingControlSkill(int i_nExcludeSkillNum);

	void		SetSkillConfirmData(MSG_FC_SKILL_CONFIRM_USE* pMsg);
	void		UseSkillConfirm(int i);
	BOOL		IsSkillOverBooster();
	CSkillInfo*	CheckConfirmSkillUse();

	CSkillInfo* FindItemEnemySkillInfo(int nSkillItemNumber);	// SkillItemNumber �� ���� ITEM_SKILL* �� ã���ִ� �Լ�

	// 2007-04-05 by bhsohn ��� ���� ó��
	INT			GetPreSkillItemNum();
	void		InitPreSkillItemNum();

	// 2007-10-22 by bhsohn Ÿ���� ��ų ������ ���� ó��
	void		ReleasePrePareSkill();

	BOOL		IsAttackPossibility();


	// 2009-03-30 by bhsohn ��¡�� ����Ʈ ���� ����
	BOOL		IsSkillUse(INT	ItemNum);
	// end 2009-03-30 by bhsohn ��¡�� ����Ʈ ���� ����

	// 2008-08-27 by bhsohn HP/DP���ÿ� ���� ������ �߰�
	BOOL		IsShieldParalyze();

	
	// 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����

	BOOL		IsExistDesParamSKill( BYTE nDesParam );

	// end 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����


	// 2008-10-23 by bhsohn �ڱ� �ڽ����� �� ����Ű �߰�
	BOOL		IsTargetToMyShuttle(BYTE	i_bySkillTargetType);
	BOOL		FindTargetForSkill_ToMe();	 
	
	// 2008. 12. 23 by ckPark ��ų ��� ���� ��� ����
	void		SendConfirmSkillUse(CSkillInfo* pSkillInfo, UID32_t TargetCharacterUniqueNumber);

	// 2008-12-26 by bhsohn ����� ��� ���� ����
	void		EraseLowLVUsingSkill(ClientIndex_t	i_AttackIndex, INT i_nSkillItemNum);

	// 2008-12-29 by bhsohn ��� �� ���� ��ų ���� �ý���
	void		ReleaseStatDEAD_AllUsingSkillWithOutPermanent();

	// 2013-05-07 by bhsohn ��������Ʈ ������ �ý���
	void		DisableTimeLimitSkill(INT ItemNum);

private:
	int			FindSkillItemNum(LONGLONG UniqueNumber);// UniqueNumber �� ���� SkillItemNumber �� ã���ִ� �Լ�
	BOOL		IsSingleSkill(int nSkillItemNum);		// nSkillItemNum �� �ش��ϴ� ��ų�� ����� ��ų,����,���� ��ų�ΰ�?			
	BOOL		IsExistSingleSkill();					// ��Ʈ�� ��ų,����,������ Single��ų�̴�.
	BOOL		IsExistTargetOneSkill(CSkillInfo *pItemSkill);				// Ÿ���� Ŭ���ؾ� �ߵ��ϴ� ��ų�� �ߵ����ΰ�?
	BOOL		FindListTargetForSkill(POINT pt);		// ��� ����Ʈ���� ��ų �ߵ�
	BOOL		IsExistMultyNGround();					// ��ƼŸ���̳� ���������� ������ Ȯ��
	BOOL		IsUseSkill(CSkillInfo* pItemSkill);
	BOOL		IsUseEventMap(CSkillInfo* pItemSkill);	// �̺�Ʈ �ʿ��� ��� ������ ��ų�ΰ�?
	BOOL		IsSkillPossibility();					// ��ų�� ��밡���Ѱ�?
	BOOL		IsSkillPossibilitySlience();		  // 2011-10110 by jhahn EP4 ħ�� ��ų �߰�
	BOOL		IsGroundSkill(CSkillInfo *pItemSkill);	// ���󿡼� ����ϴ� ��ų�̳�?
	BOOL		IsCancelSkill(CSkillInfo* pSkillInfo);		// ��ȭ ��ų ���� ������ ��ų�ΰ�?

	// 2008. 12. 23 by ckPark ��ų ��� ���� ��� ����
	//void		SendConfirmSkillUse(CSkillInfo* pSkillInfo, UID32_t TargetCharacterUniqueNumber);


	// 2007-05-23 by bhsohn ��ų �ȳ����� ���� ó��
	void		InitTargetState();

	// 2008-12-29 by bhsohn ��� �� ���� ��ų ���� �ý���
	BOOL	IsReleaseSkill_StatDEAD(CSkillInfo* i_pSkillInfo);

public:
	map<int, CSkillInfo*>	m_mapSkill;					// ��� ������ ��� ��ų (key:ItemNum, value:skill info)
	map<int, CSkillInfo*>	m_mapEnemySkillInfo;		// �ٸ� ����� ����� ��ų(key:ItemNum, value:skill info)
	vector<CSkillInfo*>		m_vecUsingSkill;			// �� �ڽſ��� ����� ��ų.
	vector<CSkillInfo*>		m_vecEnemyUsingSkillForMe;	// �ٸ� ������ ������ �� ��ų.
	// 2007-04-19 by bhsohn ��ġ���� ������ �߰�
	//D3DXVECTOR3				m_vScanPosition;			// ��ĵ�� �߽� ��ǥ.
	

private:
	BOOL					m_bSmartSpeed;				// SmartSpeed ��ų�� ������ΰ�?
	// 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
	BOOL					m_bMonsterHold;
	// end 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
	int						m_nSkillPatternType;		// ���� ������� ���� Ÿ��
	int						m_nChargingShotState;		// ��¡���� ����
	BOOL					m_bScan;					// ���� ��ĵ������ų�� �ް� �ִ� �����ΰ�?
	//float					m_fScanRange;
	CSkillInfo*				m_pPriSkillInfo;			// ���� ������� 1������ Skill �� �����ϱ����� �ӽú���
	CSkillInfo*				m_pSecSkillInfo;			// ���� ������� 2������ Skill �� �����ϱ����� �ӽú���
	BOOL					m_bSkillTargetState;		// ���� ���콺 ������ Ÿ�� Ŭ�� ����
	ClientIndex_t			m_nTargetIndex;				// Ÿ���� �ִ� ��ų�� ��� TargetIndex;
	UID32_t					m_nCharacterUID;			// Ŭ���̾�Ʈ �ε����� ��ȿ���� ���� �� ����ũ�ѹ��� �Է�
	MSG_FC_SKILL_CONFIRM_USE m_stSkillConfirm;

	// 2007-04-05 by bhsohn ��� ���� ó��
	INT						m_nPreSkillItemNum;			// ����� �õ����� ���

	// 2007-04-19 by bhsohn ��ġ���� ������ �߰�
	INT						m_nScanState;
	D3DXVECTOR3				m_vScanPosition[MAX_SCAN];			// ��ĵ�� �߽� ��ǥ.
	float					m_fScanRange[MAX_SCAN];				// ��ĵ ���� 

	// 2009. 04. 06 by ckPark �ݿ�������ν� �ٸ� Ÿ���� ��ų ������ ����
public:
	MSG_FC_SKILL_CONFIRM_USE GetSkillConfirm( void ) { return m_stSkillConfirm; }
	// end 2009. 04. 06 by ckPark �ݿ�������ν� �ٸ� Ÿ���� ��ų ������ ����
};

#endif // !defined(AFX_SKILL_H__5A26A431_9BF7_4616_8ADB_BBBEF7A4D820__INCLUDED_)
