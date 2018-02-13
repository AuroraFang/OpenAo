// WeaponFastData.h: interface for the CWeaponFastData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEAPONFASTDATA_H__51B7BBE4_E0D4_4A92_82EA_CC3BC47A7EC5__INCLUDED_)
#define AFX_WEAPONFASTDATA_H__51B7BBE4_E0D4_4A92_82EA_CC3BC47A7EC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Weapon.h"

class CAtumData;
struct ATTACK_DATA;
struct ITEM;

class CWeaponFastData : public CWeapon  
{
public:

	// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
	// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
	//CWeaponFastData(CAtumData * pAttack, ITEM * pWeaponITEM, ATTACK_DATA & attackData);
	//CWeaponFastData( CAtumData* pAttack, ITEM* pWeaponITEM, ATTACK_DATA& attackData, ITEM* pEffectItem = NULL );
	// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
	CWeaponFastData( CAtumData* pAttack, ITEM* pWeaponITEM, ATTACK_DATA& attackData, ITEM* pEffectItem = NULL, int LoadingPriority = _NOTHING_STEP);
	//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����


//	CWeaponFastData(CAtumData * pAttack,CAtumData * pTarget,int type,MSG_FC_BATTLE_ATTACK_RESULT_PRIMARY* pMsg);//int wtType,D3DXVECTOR3 vPos);
//	CWeaponFastData(CAtumData * pAttack,CAtumData * pTarget,MSG_FC_BATTLE_ATTACK_RESULT_PRIMARY* pMsg);//int wtType,D3DXVECTOR3 vPos);
//	CWeaponFastData(CAtumData * pAttack,CAtumData * pTarget,int type,MSG_FC_BATTLE_ATTACK_ITEM_RESULT_PRIMARY* pMsg);//int wtType,D3DXVECTOR3 vPos);
//	CWeaponFastData(CAtumData * pAttack,CAtumData * pTarget,MSG_FC_BATTLE_ATTACK_ITEM_RESULT_PRIMARY* pMsg);//int wtType,D3DXVECTOR3 vPos);
	
	virtual ~CWeaponFastData();
	virtual void Tick();
	void CheckWeaponCollision(D3DXVECTOR3 vDistance, float fMovingDistance);

	FLOAT				m_fTargetDistance;			// ���� ���õ� Ÿ�� ��ġ���� �Ÿ�(�浹 üũ�� ���ؼ� �̸� ���س��´�)
	INT					m_nItemIndex;
	BYTE				m_bFireType;				// 0: �� 1: �� 2: ��
	UINT				m_nWeaponItemNumber;		// ���� Ÿ��(�� ��������,���ε� ��Ʈ������)
	BOOL				m_bNeedCheckCollision;		// �浹 üũ�� �ʿ��Ѱ�?

	D3DXMATRIX			m_matColl;
};

#endif // !defined(AFX_WEAPONFASTDATA_H__51B7BBE4_E0D4_4A92_82EA_CC3BC47A7EC5__INCLUDED_)
