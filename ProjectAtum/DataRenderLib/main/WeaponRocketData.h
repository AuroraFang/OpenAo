// WeaponRocketData.h: interface for the CWeaponRocketData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEAPONROCKETDATA_H__8BB73832_2C69_4757_B2F1_B85BB2F5A93E__INCLUDED_)
#define AFX_WEAPONROCKETDATA_H__8BB73832_2C69_4757_B2F1_B85BB2F5A93E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WSlowData.h"

class CItemData;
class CCinema;
class CAtumData;
struct ITEM;
struct ATTACK_DATA;
class CPkNormalTimer;

class CWeaponRocketData : public CWSlowData  
{
public:
	
	// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
	// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
	//CWeaponRocketData(CAtumData * pAttack, ITEM * pWeaponITEM, ATTACK_DATA & attackData);
	//CWeaponRocketData( CAtumData * pAttack, ITEM * pWeaponITEM, ATTACK_DATA & attackData, ITEM* pEffectItem = NULL );
	// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
	CWeaponRocketData( CAtumData * pAttack, ITEM * pWeaponITEM, ATTACK_DATA & attackData, ITEM* pEffectItem = NULL, int LoadingPriority = _NOTHING_STEP);
	//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����

//	CWeaponRocketData(CAtumData * pAttack,CAtumData * pTarget,MSG_FC_BATTLE_ATTACK_RESULT_SECONDARY* pMsg);
//	CWeaponRocketData(CAtumData * pAttack,CAtumData * pTarget,MSG_FC_BATTLE_ATTACK_ITEM_RESULT_SECONDARY* pMsg);
//	CWeaponRocketData(MSG_FC_BATTLE_BUNDLE_ATTACK_RESULT* pMsg);
//	CWeaponRocketData(MSG_FC_BATTLE_BUNDLE_ATTACK_ITEM_RESULT* pMsg);

	virtual ~CWeaponRocketData();
	virtual void Tick();
	virtual void NormalTick();
	virtual void SkillTick();
//	void ChangeTarget(MSG_FC_BATTLE_CHANGE_TARGET_OK* pMsg);
	void InitData();
	void CheckTargetByBomb(float fMovingDistance);
	void CheckWeaponCollision(CItemData *pTargetItem,float fMovingDistance);

protected:
	void CheckTargetWarning();

public:
	INT					m_nTargetIndex;
	INT					m_nTargetItemFieldIndex;

	FLOAT				m_fTurnCheckTime;			// �׽�Ʈ : ���� �ð� ������ �������� �ֱ� ���� �׽�Ʈ ����
	BOOL				m_bSendData;				// Change Target �� ���� ��û�� ������ �ߴ���
	BYTE				m_bTraceColorRed;			// ���� ����(R)
	BYTE				m_bTraceColorGreen;			// ���� ����(G)
	BYTE				m_bTraceColorBlue;			// ���� ����(B)
	CCinema		*		m_pCinema;
	INT					m_nTargetMe;

	// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
	ITEM*				m_pEffectItem;
	// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
	// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
	int					m_LoadingPriority;
	//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
	// 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���
	int m_nMultiTargetIndex;
	// end 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���	 
};

#endif // !defined(AFX_WEAPONROCKETDATA_H__8BB73832_2C69_4757_B2F1_B85BB2F5A93E__INCLUDED_)
