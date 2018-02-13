// WeaponMineData.h: interface for the CWeaponMineData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEAPONMINEDATA_H__56B4F3E3_3D5D_4175_8B3C_1CA9B8DC6EDC__INCLUDED_)
#define AFX_WEAPONMINEDATA_H__56B4F3E3_3D5D_4175_8B3C_1CA9B8DC6EDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Weapon.h"

class CCinema;
class CWeaponMineData : public CWeapon  
{
public:
	CWeaponMineData(MSG_FC_ITEM_SHOW_ITEM* pMsg);
	CWeaponMineData(MSG_FC_BATTLE_DROP_MINE_OK* pMsg);
	virtual ~CWeaponMineData();
	virtual void Tick();
	void CheckDeleteMineSendData();
	void SendBattleMineAttack(ClientIndex_t nClientIndex, D3DXVECTOR3 vTargetPos, UINT nItemIndex);
	void SendBattleMineAttackFind(ClientIndex_t nClientIndex, D3DXVECTOR3 vTargetPos, UINT nItemIndex);

	
	BOOL					m_bSetPosition;
	INT						m_nClientIndex;					// ���� �߻��� �����̸� Ŭ���̾�Ʈ �ε����� ���´�. �ƴϸ� 0
	INT						m_nTargetItemFieldIndex;
	INT						m_nTargetIndex;
	FLOAT					m_fTargetLength;				// �߻� ��ġ�� ������ ���� ��ġ ������ �Ÿ�
//	FLOAT					m_fSpeed;			
	INT						m_nFieldItemIndex;
	BOOL					m_bIsRender;
	INT						m_nObjScreenX;			// ��ũ���� X ��ǥ
	INT						m_nObjScreenY;			// ��ũ���� Y ��ǥ
	INT						m_nObjScreenW;			// ��ũ���� W ��ǥ
	FLOAT					m_fObjectSize;
	POINT					m_ptRegion;						// ������ ��ġ�� ��
	FLOAT					m_fSendAttackDataCheckTime;		// ������ Attack �������� ���� üũ �ð� - ������ ������ 1�ʸ��� �浹 ������.
	FLOAT					m_fCheckReactionRange;			// ������ ���� �ݰ�
	CCinema			*		m_pCinema;
};

#endif // !defined(AFX_WEAPONMINEDATA_H__56B4F3E3_3D5D_4175_8B3C_1CA9B8DC6EDC__INCLUDED_)
