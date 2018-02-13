// UnitData.h: interface for the CUnitData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNITDATA_H__2D386C99_DEB3_45E7_A7B5_A88B6CB2666D__INCLUDED_)
#define AFX_UNITDATA_H__2D386C99_DEB3_45E7_A7B5_A88B6CB2666D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumData.h"
// 2007-04-02 by bhsohn Move��Ŷ���� ���� ���� üũ
#define	UNIT_STATE_INVISIBLE	0
#define	UNIT_STATE_CHARINGSHOT	1
#define	UNIT_STATE_HYPERSHOT	2
// 2013-01-30 by mspark, A��� �踮�� ��ų �����Ŀ� ���濡�� ������ �ʴ� ���� ����
#define UNIT_STATE_BARRIER		3
// end 2013-01-30 by mspark, A��� �踮�� ��ų �����Ŀ� ���濡�� ������ �ʴ� ���� ����
// 2013-02-14 by mspark, M��� ���� ��ų �����Ŀ� ���濡�� ������ �ʴ� ���� ����
#define UNIT_STATE_INVINCIBLE	4
// end 2013-02-14 by mspark, M��� ���� ��ų �����Ŀ� ���濡�� ������ �ʴ� ���� ����
// 2012-11-29 by mspark, ��ġ���� ��ġ�� �̵��ϴ� ĳ������ ��ġ�� �����ϵ��� ����
#define UNIT_STATE_SEARCHEYE	5
#define UNIT_STATE_SEARCHEYE2	6
// end 2012-11-29 by mspark, ��ġ���� ��ġ�� �̵��ϴ� ĳ������ ��ġ�� �����ϵ��� ����
#define	UNIT_STATE_MAX			7

// 2013-01-30 by mspark, A��� �踮�� ��ų �����Ŀ� ���濡�� ������ �ʴ� ���� ����
#define	AGEAR_STATE_BARRIER		7821060		// �踮��
// end 2013-01-30 by mspark, A��� �踮�� ��ų �����Ŀ� ���濡�� ������ �ʴ� ���� ����

// 2013-02-14 by mspark, M��� ���� ��ų �����Ŀ� ���濡�� ������ �ʴ� ���� ����
#define	MGEAR_STATE_INVINCIBLE	7811100		// ����
// end 2013-02-14 by mspark, M��� ���� ��ų �����Ŀ� ���濡�� ������ �ʴ� ���� ����

#define	BGEAR_STATE_CHARINGSHOT		7800060		// ��¡�� 
#define	AGEAR_STATE_HYPERSHOT		7823060		// �����ۼ� 

// 2007-05-17 by bhsohn ������Ʈ �ڿ� �������� �� ���� ó �˻� ó��
#define	CHECK_OBJECT_TIME		1.0f

///////////////////////////////////////////////////////////////////////////////
/// \class		CUnitData
///
/// \brief		CShuttleChild, CEnemyData, CMonsterData�� �θ�μ�, ����κ� ó��
/// \author		dhkwon
/// \version	
/// \date		2004-03-20 ~ 2004-03-20
/// \warning	
///////////////////////////////////////////////////////////////////////////////
class CSkillEffect;
class CItemData;
class CPetManager;		// 2010-06-15 by shcho&hslee ��ý��� - Ŭ���� �߰�
class CUnitData : public CAtumData  
{
	friend class CAtumApplication;	// ���Ŀ� ���� ��.
	friend class CUnitRender;		// m_pVBShadow �� ����
	friend class CCharacterRender;	// 2005-07-13 by ispark
	friend class CMonsterRender;	// m_pVBShadow �� ����
public:
	CUnitData();
	virtual ~CUnitData();

	int		GetUnitNum() const { return m_nUnitNum; }
	int		GetPilotNum() const { return m_nPilotNum; }			// 2005-07-13 by ispark ���Ϸ�Ʈ �ѹ�
	HRESULT InitDeviceObjects() override;
	HRESULT RestoreDeviceObjects() override;
	HRESULT InvalidateDeviceObjects() override;
	HRESULT DeleteDeviceObjects() override;

	HRESULT	RestoreShadow();		// CUnitRender, CMonsterRender�� RestoreDeviceObjects()���� ȣ���Ѵ�.
	HRESULT	InvalidateShadow();		// CUnitRender, CMonsterRender�� InvalidateDeviceObjects()���� ȣ���Ѵ�.

public:
// ���� ����
	void	ChangeUnitState( DWORD dwState );
	// 2010. 03. 18 by jskim ���ͺ��� ī��
	//void	ChangeUnitCharacterInfo(int nDefenseItemNum, int nUnitKind, BodyCond_t nBodyCon, BOOL bCharacter);
	void	ChangeUnitCharacterInfo(int nDefenseItemNum, int nUnitKind, BodyCond_t nBodyCon, BOOL bCharacter, BOOL bMonsterTransformer = FALSE);
	//end 2010. 03. 18 by jskim ���ͺ��� ī��
// �ٵ������
	void	ChangeUnitBodyCondition(BodyCond_t hyBodyCondition);
	void	ChangeSingleBodyCondition( BodyCond_t hySingleBodyCondition );
	void	ChangeKeepingBodyCondition( BodyCond_t hyBodyCondition );
	void	TurnSingleBodyCondition(BodyCond_t hySingleBodyCondition, BOOL bSet);
	float	GetCurrentAnimationTime(void);
	float	GetCurrentBodyConditionEndAnimationTime(void);
	BodyCond_t GetCurrentBodyCondition(void);
	void	ResetBodyCondition( BodyCond_t hyBody );
	void	ChangeBodyCondition( BodyCond_t hyBody );
	void	SetFlyBodyCondition( BodyCond_t hyBody );
	void	SetExBodyCondition( BodyCond_t hyBody );

// ����
	virtual void CreateSecondaryShieldDamage(D3DXVECTOR3 vCollPos) {};
	float	GetAutomaticAttackTime( BYTE nOrbitType );
	
	// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

// 	void	CreatePrimaryWeaponItem(ITEM* pItem);
// 	void	CreateSecondaryWeaponItem(ITEM* pItem);
	void	CreatePrimaryWeaponItem( ITEM* pItem, ITEM* pShapeItem );
	void	CreateSecondaryWeaponItem( ITEM* pItem, ITEM* pShapeItem );

	// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

	void	CreateWearItem( int nWearItemKind, int nEffectNum, BOOL bCharacter = FALSE);		// 2006-09-20 by ispark
	void	DeleteWearItem( int nWearItemKind );
	int		GetTotalShotNumPerReattackTime(ITEM* pWeaponITEM, CParamFactor *pParamFactor);
	int		GetMultiNum(ITEM* pWeaponITEM, CParamFactor *pParamFactor);

	void	CreateWeaponByFieldServer( MSG_FC_BATTLE_ATTACK_OK* pMsg);

	// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
	// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
	//void	CreateWeapon(ATTACK_DATA& attackData, ITEM* pWeaponITEM);
	//void	CreateWeapon( ATTACK_DATA& attackData, ITEM* pWeaponITEM, ITEM* pEffectItem = NULL );
	// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
	void	CreateWeapon( ATTACK_DATA& attackData, ITEM* pWeaponITEM, ITEM* pEffectItem = NULL, int LoadingPriority = _NOTHING_STEP);
	//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����

	// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
	//virtual void CheckAttack(ATTACK_DATA &attackData, BOOL bReset, ITEM* pWeaponITEM);
	virtual void CheckAttack(ATTACK_DATA &attackData, BOOL bReset, ITEM* pWeaponITEM = NULL, ITEM* pEffectItem = NULL);
	// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
	static CAppEffectData * CreateWearItemEffectAtSelectMenu( GUIUnitRenderInfo *pInfo, int nEffectNum, BodyCond_t nBodyCondition );

	void	ResetPrimaryWeaponBodyCondition( BodyCond_t hyBodyCon);
	void	ChangePrimaryWeaponBodyCondition( BodyCond_t hyBodyCon);
	void	ResetSecondaryWeaponBodyCondition( BodyCond_t hyBodyCon, BOOL i_bIsLeft=TRUE);
	void	ChangeSecondaryWeaponBodyCondition( BodyCond_t hyBodyCon);
	void	SetWeaponVelFromServer( D3DXVECTOR3 vWeaponVel );
	// 2010-06-15 by shcho&hslee ��ý��� - ���� ���
	void	ResetPetWeaponBodyCondition( BodyCond_t hyBodyCon, BOOL i_bIsLeft=TRUE);
	void	SetPetVelFromServer( D3DXVECTOR3 vPetLeftVel, D3DXVECTOR3 vPetRightVel );
	// end 2010-06-15 by shcho&hslee ��ý��� - ���� ���

	// 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)
	void	ShowWeapon( const BOOL bShow );
	void	CreateRobotAGearWeapon( ITEM* pAGearWeapon, const UINT nUnitType );
	void	DeleteRobotAGearWeapon( void );
	// end 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)

// PK
	void	SetPkState( DWORD dwPkFlag, BOOL bSet );
	BOOL	IsPkState( DWORD dwPkFlag );
	BOOL	IsPkEnable() { return (m_dwPkState != PK_NONE ? TRUE : FALSE); } // Enemy : ���� ���� ������ Enemy �ΰ�?, ShuttleChild : ������
	BOOL	IsPkAttackEnable() { return m_dwPkState != PK_NONE && m_dwPkState != PK_NORMAL; }
	DWORD	GetPkState() { return m_dwPkState; }
	BOOL	GetbPkAttack() { return m_bPkAttack; }
	void	SetbPkAttack(BOOL bAttack) { m_bPkAttack = bAttack; }
	int		GetUnitNumFromCharacter(int nDefenseItemNum, int nUnitKind, BodyCond_t nBodyCon, BOOL bCharacter, int nDegree);

	void	ResetPrimaryWeaponBodyCondition( BodyCond_t hyBodyCon, BOOL bZig);

	void	ChangeWearItemBodyCondition(int nWearItemKind, BodyCond_t hyBodyCondition);	

// ����
	BYTE	GetUnitAlphaState() { return m_bySkillStateFlag; }
	int		GetUnitAlphaValue() { return m_nAlphaValue; }
	void	SetUnitAlpha(BYTE bySkillState, int nAlphaValue);
	void	InitUnitAlpha();

// ü��
	void	AddChaffData(CAtumNode* pUnitData, MSG_FC_BATTLE_DROP_DUMMY_OK* pMsg);
	void	DelChaffData(UINT nItemIndex);
	void	DelChaffDataAll();
	CItemData* FindChaffDataByFieldIndex ( UINT nFieldIndex );
	void	ChaffDataTick();
	int		GetChaffDataCound();

	void	AddScanObject(CAtumNode* pUnitData,float fCheckTime);
	void	ScanDataTick();

	// 2007-04-02 by bhsohn Move��Ŷ���� ���� ���� üũ
	FLOAT	GetSkillChgTime(int nIdx);
	void	AddSkillChgTime(int nIdx, FLOAT fUnitStateChgCap);
	bool	IsUnitState(int nIdx);
	void	SetUnitSkillState(int nIdx, bool bOnOff);

	// 2007-05-17 by bhsohn ������Ʈ �ڿ� �������� �� ���� ó �˻� ó��
	virtual BOOL	IsCheckObjectShow();
	virtual BOOL	IsCheckObjectColl();
	// 2010. 03. 18 by jskim ���ͺ��� ī��
	void SetMonsterTransformer(int MonNum) { m_MonsterTransformer = MonNum; }
	int	 GetMonsterTransformer() { return m_MonsterTransformer; }
	void SetMonsterTransScale( float Scale ) { m_MonsterTransScale = Scale; }
	float GetMonsterTransScale() { return m_MonsterTransScale; }
	void SetMonsterTransPrimarySidePos( D3DXVECTOR3 sidePos ) { m_MonsterTransPrimarySidePos = sidePos; }
	void SetMonsterTransSecondarySidePos( D3DXVECTOR3 sidePos ) { m_MonsterTransSecondarySidePos = sidePos; }
	D3DXVECTOR3	GetMonsterTransPrimarySidePos() { return m_MonsterTransPrimarySidePos; }
	D3DXVECTOR3	GetMonsterTransSecondarySidePos() { return m_MonsterTransSecondarySidePos; }
	//end 2010. 03. 18 by jskim ���ͺ��� ī��

	// 2010-06-15 by shcho&hslee ��ý��� - �� ���� ó��
	CPetManager*		GetPetManager() { return m_pPetManager; }
	void				SetPetManager(CPetManager* i_PetManager) { m_pPetManager = i_PetManager; }
	void				HidePet( const BOOL bHide );
	//end 2010-06-15 by shcho&hslee ��ý��� - �� ���� ó��

	// 2013-03-06 by bhsohn ���� ���� �ý���
	// ���� �������� ����	
	virtual void SetRtnGameUser(BYTE byUsingReturnItem) {return ;}
	virtual BYTE GetRtnGameUser() {return 0;}
	// END 2013-03-06 by bhsohn ���� ���� �ý���

protected:
//���� ������
	void	ReloadWearItem();
	void	DeleteWearItemEffect( CAppEffectData * pEffect );
	CAppEffectData * CreateWearItemEffect( int nWearItemKind, int nEffectNum, D3DXVECTOR3 vPos, int nBodyCondition, int nWeaponPositionIndex = 0 );
	void	ResetWearItemBodyCondition( int nWearItemKind, CAppEffectData * pEffect, BodyCond_t hyBodyCon);
	void	DeleteItemPoint();
// ����
	virtual USHORT GetShiftWeaponBodyconditionByUnitKind();
	void	DelSecondWeapon();

	// 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)
public:
	// 2013-01-08 by jhjang �κ��Ƹӿ� ���� ���� Ŷ�� ��������� ������ �߻��ϴ� �κ� ����
	BOOL	IsRobotArmor_Old( void );// 2013-01-08 by jhjang ���� �κ� �Ƹ� üũ �Լ�
	BOOL	IsRobotArmor( void );	// 2013-01-08 by jhjang ���� ���� ���� �κ� �Ƹ� üũ �Լ�
	// end 2013-01-08 by jhjang �κ��Ƹӿ� ���� ���� Ŷ�� ��������� ������ �߻��ϴ� �κ� ����
	// end 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)

public:
	CParamFactor			m_paramFactor;			// ParamFactor( Enemy�� ��� 1���� ����,������Ÿ�Ӹ� ����)
	LPDIRECT3DVERTEXBUFFER9 m_pVBShadow;			// �׸��� ����

	//////////////////////////////////////////////////////////////////////////
	// ��ų
	vector<CItemData *>		m_pChaffData;			// 2006-12-04 by ispark
	CSkillEffect*			m_pSkillEffect;
	int						m_nAlphaValue;			// ���İ�
	BYTE					m_bySkillStateFlag;		// ��ų ���� (����, �κ�����...)
	ClientIndex_t			m_nTargetIndex;					// Enemy�� Ÿ�� �ε���
	
	// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
	int						m_LoadingPriority;
	//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����

protected:
	int						m_nUnitNum;				// ex> 12030100 ( 12 : degree, 03 : amor, 01 : unitKind )
	int						m_nPilotNum;			// 2005-07-13 by ispark	���Ϸ�Ʈ �ѹ�
	BOOL					m_bCharacter;			// 2005-07-13 by ispark (Unit : FALSE, Character : TRUE)
	DWORD					m_dwPkState;
	BOOL					m_bPkAttack;			// 2005-11-03 by ispark PK ���� �����Ѱ�, m_dwPkState�� Normal�̾ �̰��� TRUE�̸� ���� �� ����

	// 2007-04-02 by bhsohn Move��Ŷ���� ���� ���� üũ
	BYTE					m_byUnitState;
	FLOAT					m_fUnitStateChgCap[UNIT_STATE_MAX];		// ���� ���°� ����Ǿ��־��� �ð� 

	// 2007-05-17 by bhsohn ������Ʈ �ڿ� �������� �� ���� ó �˻� ó��
	BOOL					m_bCheckObjectEnemyShow;		// ������Ʈ �ڿ� �������� �� ���� ó �˻� ó��
	BOOL					m_bCheckObject;					// ������Ʈ üũ
	float					n_fCheckObjectTime;
	// 2010. 03. 18 by jskim ���ͺ��� ī��
	int						m_MonsterTransformer;
	D3DXVECTOR3				m_MonsterTransPrimarySidePos;
	D3DXVECTOR3				m_MonsterTransSecondarySidePos;
	float					m_MonsterTransScale;
	//end 2010. 03. 18 by jskim ���ͺ��� ī��

	CPetManager*			m_pPetManager;
};

#endif // !defined(AFX_UNITDATA_H__2D386C99_DEB3_45E7_A7B5_A88B6CB2666D__INCLUDED_)
