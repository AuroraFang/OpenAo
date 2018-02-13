// WeaponItemInfo.h: interface for the CWeaponItemInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEAPONITEMINFO_H__103FED3F_02AF_4D41_9EC6_B76719B779E3__INCLUDED_)
#define AFX_WEAPONITEMINFO_H__103FED3F_02AF_4D41_9EC6_B76719B779E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ItemInfo.h"

class CItemInfo;
class CWeaponMineData;
class CParamFactor;
class CAtumData;
class CItemData;
class CUnitData;
struct MINE_DATA
{
	vector<CWeaponMineData *>	m_vecMine;					// ���� �߻�� ����
	int							m_nMaxMineNumber;			// �ִ� ���� ����
};
// 2006-02-01 by ispark, Ÿ�� �Ÿ� ���ؼ� Ÿ�� ���� ����Ʈ

// 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���
struct TARGET_DATA
{
	int nTargetIndex;
	int nTargetMultiIndex;
};
// end 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���

struct stTargetList
{
	int		nTargetIndex;
	float	fTargetLength;
	// 2011. 03. 08 by jskim ����3�� ����
	int		nMultiIndex;   
    // end 2011. 03. 08 by jskim ����3�� ����

	stTargetList()
	{
		nTargetIndex = 0;
		fTargetLength = 0.0f;
        // 2011. 03. 08 by jskim ����3�� ����
		nMultiIndex	=	0;					 
        // end 2011. 03. 08 by jskim ����3�� ����
	}
};

struct sort_List
{
	bool operator()(stTargetList p1, stTargetList p2)
	{
		 return p1.fTargetLength < p2.fTargetLength; 
	}
};

///////////////////////////////////////////////////////////////////////////////
/// \class		CWeaponItemInfo
///
/// \brief		���� ���⿡ ���� ���
/// \author		dhkwon
/// \version	
/// \date		2004-07-20 ~ 2004-07-20
/// \warning	ShuttleChild�� �̴´�. UnitData�� �Ȼ̴´�.
///////////////////////////////////////////////////////////////////////////////
class CWeaponItemInfo  
{
public:
	CWeaponItemInfo(CItemInfo* pItemInfo, CParamFactor * pParamFactor, float fOverHeatTime=0, BOOL bOverHeat=FALSE);
	virtual ~CWeaponItemInfo();

	void	SetAttackMode(BYTE nAttackMode) { m_nAttackMode = nAttackMode; }
	BYTE	GetAttackMode() { return m_nAttackMode; }
//	void	SetServerSyncReattackOk(BOOL bOK);
	float	GetOverHeatRate() {	return m_fOverHeatCheckTime/CAtumSJ::GetOverheatTime(m_pItemInfo->GetRealItemInfo(),m_pCharacterParamFactor); }
	float	GetReattackTimeRate() { return m_fReattackCheckTime/CAtumSJ::GetShotCountReattackTime(m_pItemInfo->GetRealItemInfo(),m_pCharacterParamFactor); }
	float	GetOverHeatTime() { return CAtumSJ::GetOverheatTime(m_pItemInfo->GetRealItemInfo(),m_pCharacterParamFactor); }
	float	GetOverHeatCheckTime() { return m_fOverHeatCheckTime; }
	BOOL	IsOverHeat() { return m_bOverHeat; }
	void	Tick(float fElapsedTime, BOOL bUse);
	void	TickSecondaryWeapon2(float fElapsedTime, BOOL bUse);
	void	TickNormalWeapon(float fElapsedTime, BOOL bUse);
	// 2010-06-15 by shcho&hslee ��ý��� - �� ���� ó��
	void	TickPetWeapon(float fElapsedTime, BOOL bUse);
	//end 2010-06-15 by shcho&hslee ��ý��� - �� ���� ó��	

	//float	CheckPrimaryInNet(CUnitData* pTarget);
	float	CheckPrimaryInNet(CUnitData* pTarget, BOOL bEqualTarget2Enemy=FALSE);
	// 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���
    float	CheckPrimaryMultiInNet(CUnitData* pTarget, BOOL bEqualTarget2Enemy = FALSE, int nMultiIndex = NULL);
	BOOL	CheckMultiInNet( CUnitData* pTarget, int nMultiIndex );
	// end 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���

	BOOL	CheckSecondaryInNet(CUnitData* pTarget);
	void	SetWeaponCount(INT nCount);
	void	BulletReloaded(MSG_FC_BATTLE_PRI_BULLET_RELOADED* pMsg);
	ITEM_GENERAL* GetItemGeneral() { return m_pItemInfo; }
	ITEM*	GetRealItemInfo() { return m_pItemInfo->GetRealItemInfo(); } // ������ ��¥ ����(��æƮ,���� ���� ��)
	DWORD	GetAttackTick() { return m_dwLastAttackTick; }

	void	AddMine(CWeaponMineData *pMine);
	void	DeleteMine(CWeaponMineData *pMine);

	void	UsePrimary();
	void	UseSecondary();

	// 2010-06-15 by shcho&hslee ��ý��� - �� ���� ����
	void	UsePetAttack();
	// end 2010-06-15 by shcho&hslee ��ý��� - �� ���� ����
	void	SetUseToggleWeapon(BOOL bUseToggleWeapon) { m_bUseToggleWeapon = bUseToggleWeapon; }

	void	SetReattackCheckTime(float fReattackCheckTime) { m_fReattackCheckTime = fReattackCheckTime; }
	void	SetAttackCount(int nAttackCount) { m_nAttackCount = nAttackCount; }
	BOOL	IsUsingToggleWeapon() { return m_bUseToggleWeapon; }
	void	ReleaseAllUsingToggleWeapon();			// ��� ������� ��� ���⸦ �����Ѵ�.

	void	DelTargetSecondary();					// 2006-09-08 by ispark, 2�� ���� Ÿ�� ����
	void	DelTargetIndex(int nTargetIndex);

	// 2008-09-22 by bhsohn EP3 ĳ���� â
	CItemInfo *	GetClassItemInfo();
	float GetStatPierce();	
	float GetPrimaryEnchantPierce();	
	float GetSecondaryEnchantPierce();
	float GetTotalPierce(BOOL bIsPrimary);

	// 2009-02-16 by bhsohn ����, ���� �Ǿ �� �Ⱥ��̴� ���� ó��
	float GetPreSufFixItemInfoRate(int nDesParameter);
	// end 2009-02-16 by bhsohn ����, ���� �Ǿ �� �Ⱥ��̴� ���� ó��


protected:
	void	Use();									// ���⸦ ����Ѵ�.
	// 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���
	//void	SendBattleAttackPrimary(int nTargetIndex, int nItemIndex, D3DXVECTOR3 vTargetPos);
	//void	SendBattleAttackSecondary(int nTargetIndex, int nItemIndex, D3DXVECTOR3 vTargetPos, D3DXVECTOR3 vFirePos);
	void	SendBattleAttackPrimary(int nTargetIndex, int nItemIndex, D3DXVECTOR3 vTargetPos, int nMultiIndex = 0);
	void	SendBattleAttackSecondary(int nTargetIndex, int nItemIndex, D3DXVECTOR3 vTargetPos, D3DXVECTOR3 vFirePos, int nMultiIndex = 0);
	void	SendBattleAttackPet(int nTargetIndex, int nItemIndex, D3DXVECTOR3 vTargetPos, int nMultiIndex = 0);
	// end 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���
	// 2010-06-15 by shcho&hslee ��ý��� - �� ���� ����
	//void	SendBattleAttackPet(int nTargetIndex, int nItemIndex, D3DXVECTOR3 vTargetPos);
	// end 2010-06-15 by shcho&hslee ��ý��� - �� ���� ����

//	void	SendBattleDropBundle();
	void	SendBattleDropMine();
	void	SendUseShield();
	void	SendUseDecoy();
	void	SendUseDummy();
	void	SendUseFixer();
	void	SendUseMissileAndRocket();

	void	SortTargetList();
	
	// 2007-05-16 by bhsohn 1������ Ÿ�� ó��
	void	PrimarySortTargetList();

public:
// ������� �߰� �������� ���߿� �ּ� �����..jschoi
	void	SendBattleAttack(MEX_TARGET_INFO TargetInfo,UINT nSkillNum, D3DXVECTOR3 FirePosition);




	void	CheckInNet();		// ȭ���� ���� Ÿ�� ����Ʈ ���� (multitarget, range, angle)
	void	ResetWeaponBodyCondition(BodyCond_t hyBody);

	BOOL	IsExistBullet();	// �Ѿ��� �����Ǿ� �ִ°�?
	
	// 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���
	//void	PutTargetIndex( int nRemainedTarget, int index );
	void	PutTargetIndex( int nRemainedTarget, int index, int MultiIndex = 0 );
	// end 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���

	BOOL	CheckWarpGateZone(D3DXVECTOR3 vPos);	// ����� �Ÿ��� ��������Ʈ�� ���°�?

public:
	// 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���
	//vector<INT>		m_vecTargetIndex;				// ClientIndex : 10000>=0 ? monster : enemy
	vector<TARGET_DATA>		m_vecTargetIndexData;				// ClientIndex : 10000>=0 ? monster : enemy
	// end 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���
	MINE_DATA *		m_pMineData;


protected:
	CItemInfo *		m_pItemInfo;
	CParamFactor *	m_pCharacterParamFactor;		// ĳ������ ParamFactor

	BOOL			m_bOverHeat;//m_bWeaponAction;				// ���� ����
	FLOAT			m_fOverHeatCheckTime;//m_fWeaponActionCheckTime;	// ���� üũ Ÿ��
	FLOAT			m_fReattackCheckTime;//m_fWeaponRACheckTime;		// ������ üũ Ÿ��
	FLOAT			m_fPrepareCheckTime;			// �ߵ� �غ� üũ Ÿ��
	FLOAT			m_fAttackCheckTime;				// ���� �ð� üũ Ÿ��
	int				m_nAttackCount;					// ���� ������ �Ѿ� ��
	BYTE			m_nAttackMode;					// ���� Ÿ��(0 : ���� ���� ���  1 : ���� ���� ���)


	DWORD			m_dwLastAttackTick;
//	BYTE			m_nServerSyncShotNum;			// �Ѿ��� ��ũ�� ���߱����� ������ ���� Ȯ�� �뺸�� �ް� �߻��ϰ� �ȴ�.
	vector<int>		m_vecSoundCheck;				// 2�� ���� ���� üũ��
	BOOL			m_bUseToggleWeapon;				// 2-2�� ����� ������ ��� ����
//	float			m_fAutoSyncReattackOkTime;		// �ð��� ������ �ڵ����� reattacktime * 2 ��ŭ ����

	vector<stTargetList> m_vecTargetOrderList;		// Ÿ�� �Ÿ��� ����Ʈ

	// 2007-05-16 by bhsohn 1������ Ÿ�� ó��
	deque<stTargetList> m_vecTmpTargetOrderList;		// 1������ ������ ���� �ӽ� ����

	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	BOOL HpCharge;
	BOOL ShieldCharge;
	BOOL SpCharge;							  
	//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
};

#endif // !defined(AFX_WEAPONITEMINFO_H__103FED3F_02AF_4D41_9EC6_B76719B779E3__INCLUDED_)
