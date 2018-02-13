// WeaponAllAttackData.h: interface for the CWeaponAllAttackData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEAPONALLATTACKDATA_H__B778727D_7CCE_4F40_8464_FEB126E54B49__INCLUDED_)
#define AFX_WEAPONALLATTACKDATA_H__B778727D_7CCE_4F40_8464_FEB126E54B49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Weapon.h"
class CUnitData;
class CCharacterInfo;

///////////////////////////////////////////////////////////////////////////////
/// \class		CWeaponAllAttackData
///
/// \brief		��ü ���� ����Ʈ ó��(�����ڸ� �ְ�, Ÿ���� ����. Ÿ���� �������� ����� ��)
///				ITEM.Range : ���߹ݰ�(���� �� �������� ������ ��ġ�� �ݰ�)
///				ITEM.RangeAngle : �߻� ���� ����
///				ITEM.
///				�ӵ� : MAX_WEAPON_SPEED
/// \author		dhkwon
/// \version	
/// \date		2004-06-24 ~ 2004-06-24
/// \warning	
///////////////////////////////////////////////////////////////////////////////
class CWeaponAllAttackData : public CWeapon
{
public:

	// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
	// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
	//CWeaponAllAttackData(CAtumData * pAttackter, CAtumData * pTarget, BodyCond_t nBodyCondition, UINT nItemNum);
	//CWeaponAllAttackData( CAtumData * pAttackter, CAtumData * pTarget, BodyCond_t nBodyCondition, UINT nItemNum, ITEM* pEffectItem = NULL );
	CWeaponAllAttackData( CAtumData * pAttackter, CAtumData * pTarget, BodyCond_t nBodyCondition, UINT nItemNum, ITEM* pEffectItem = NULL ,int LoadingPriority = _NOTHING_STEP);
	// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����	
	//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����

	virtual ~CWeaponAllAttackData();
	virtual void Tick();
protected:
	void CreateHitEffect(CAtumData* pTarget);

protected:
//	D3DXMATRIX			m_mMatrix;
//	CUnitData *			m_pAttacker;
//	CAtumNode *			m_pTarget;
//	ITEM		*		m_pITEM;
//	BodyCond_t			m_hyBodyCondition;
//	CCharacterInfo*		m_pCharacterInfo;
//	D3DXVECTOR3			m_vPos;						// ����Ʈ�� ��ġ

	// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
	ITEM*	m_pEffectItem;
	// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
};

#endif // !defined(AFX_WEAPONALLATTACKDATA_H__B778727D_7CCE_4F40_8464_FEB126E54B49__INCLUDED_)
