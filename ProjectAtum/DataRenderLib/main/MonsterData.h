// MonsterData.h: interface for the CMonsterData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONSTERDATA_H__F06CA2D1_F4C9_4EF2_B607_B83D503B400C__INCLUDED_)
#define AFX_MONSTERDATA_H__F06CA2D1_F4C9_4EF2_B607_B83D503B400C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UnitData.h"

class CCinema;
class CMonRender;
class CAppEffectData;
class CSkinnedMesh;
class CMonsterData : public CUnitData 
{
public:
	CMonsterData(MSG_FC_CHARACTER_GET_MONSTER_INFO_OK * pMsg);
	virtual ~CMonsterData();
	virtual void Tick();
	virtual void Render();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	void SetPosition(MSG_FC_MOVE_OK* pMsg);
	void SetMonPosition(MSG_FC_MONSTER_MOVE_OK* pMsg);
//	void SetMonsterTypePrimary(int nType){ m_nMonsterTypePrimary = nType; };
//	void SetMonsterTypeSecondary(int nType){ m_nMonsterTypeSecondary = nType; };
	void CheckShadowPos();
	void CheckAniTime(float fElapsedTime);
	void Init();
	void InvalidateChat();
	void RestoreChat();
	void ChangeBodyCondition(BodyCond_t hyBodyCondition);
	void RenderShadow();

	// remodeling, 2004-05-13 by dhkwon
	void ChangeBodyConditionFromServer(BodyCond_t hyBodyCondition);
	void MonsterChangeHPFromServer( INT CurrentHP );

	// ���� ��ų ���� �Լ�
	void UseSkillMove();
	void InitMonsterMovePattern(int nMonsterMoveType);

	// ������Ʈ ���� ������Ʈ ���� �Լ�
	void EnableObjectMonsterObject();
	void DisableObjectMonsterObject();

	// 2007-05-17 by bhsohn ������Ʈ �ڿ� �������� �� ���� ó �˻� ó��
	virtual BOOL	IsCheckObjectShow();	
	virtual BOOL	IsCheckObjectColl();

	// 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���
	D3DXVECTOR3 GetMultiPos(int nMultiIndex);
	void GetScreenPos(D3DXVECTOR3 vMultiPos, int &x, int &y, int& w);
	// end 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���	

	// 2011-05-17 by jhahn	����3�� ���� ��ü
	void ChangeMonster(int ChangeMonsterUnitKind);
	void ChangeMonsterInfo(int nUnitKind, BodyCond_t nBodyCon);
	//end 2011-05-17 by jhahn	����3�� ���� ��ü

	//2011-06-05 by jhahn	����3�� ���� ����
	void DeleteMonster();
	//end 2011-06-05 by jhahn	����3�� ���� ����
protected:
	D3DXVECTOR3 GetMapNormalVector(D3DXVECTOR3 vPos);

	// 2007-05-17 by bhsohn ������Ʈ �ڿ� �������� �� ���� ó �˻� ó��
	void TickObjectCheck();
	BOOL UpdateCheckObjectTOEnemy();		
	
public:
	CAtumData		*	m_pTarget;
	FLOAT				m_fTargetCheckTime;

	D3DXVECTOR3			m_vNextVel;
	D3DXVECTOR3			m_vOldPos;

	// ������ ��ų ���� ���� 2004.06.26 jschoi
	D3DXVECTOR3			m_vOldSkillEndPos;
	D3DXVECTOR3			m_vOldSkillEndVel;
	D3DXVECTOR3			m_vOldSkillEndUp;

	D3DXVECTOR3			m_vExpectPos;

	D3DXVECTOR3			m_vMovepos;
	D3DXVECTOR3			m_vMoveup;

	D3DXVECTOR3			m_vAttackVel;

	FLOAT				m_fLeanRate;				// �¿� ������ ����

//	BYTE				m_nMonsterTypePrimary;		// Ÿ�� ������ ���
//	BYTE				m_nMonsterTypeSecondary;	// Ÿ�� ������ ���
	BOOL				m_bRenderState;				// ������ ����(�������)
//	INT					m_nRenderType;
	FLOAT				m_fLifeTime;
	BOOL				m_bCheckColl;
	BOOL				m_bAlphaBlending;
//	BOOL				m_bIsRender;				// ȭ�� ������ ����� ������ ���� �ʴ���
	///////////////// �߶� Time /////////////////////
	FLOAT				m_fFallingTime;				// �߶��� Check Time
	D3DMATERIAL9		m_material;
	FLOAT				m_fMaterialCheckTime;

	FLOAT				m_fMaxHP;
	BOOL				m_bRenderHP;
	D3DXMATRIX			m_mScale;
	POINT				m_ptCurRegion;					// ���� �� ������
	POINT				m_ptOldRegion;					// ���� ���� �� ������

	INT					m_nUpDir;						// �ʺ� ����Ʈ �̼��� ���� ���� ����
	INT					m_nSideDir;						// �ʺ� ����Ʈ �̼��� ���� ���� ����

	CMonRender		*	m_pRenderer;

	CAppEffectData	*	m_pEffectTarget;
	
	MSG_FC_CHARACTER_GET_MONSTER_INFO_OK	m_info;
	MEX_MONSTER_INFO *	m_pMonsterInfo;
	MONSTERINFO			m_ResInfo;

	// ���� ��ų ���� 2004.06.25 jschoi
	int					m_nMonsterSkillState;			// ���Ͱ� ��ų�� ����
	BOOL				m_bSkillEnd;					// ��ų�� ���Ḧ ���� ������ �����ؾ��Ѵ°�?
	CCinema		*		m_pMonsterMovePattern;
	CSkinnedMesh*		m_pMonMesh;
	
	ClientIndex_t		m_nTargetIndex;					// ������ Ÿ�� �ε���
	// 2010. 03. 18 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ��ų �߰�)
	std::map<INT, ITEM*>	m_mapMonsterBuffSkill;

	void	InsertMonsterBuffSkill( INT nSkillNum, ITEM* pSkillItem );
	void	RemoveMonsterBuffSkill( INT nSkillNum );
	ITEM*	GetSkillItemWithDesParam( BYTE nDesParam );
	// end 2010. 03. 18 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ��ų �߰�)

	// 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���
	vector<MULTI_TARGET_DATA*>	m_vecvmultiData;
	vectorMonsterMultiTarget	m_vecMonMultiInfo;
	int							m_nindexSize;
	int							m_nMultiIndex;
	int							m_nSiegeIndex;
	BOOL						m_bSiegeMode;
	// end 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���
};

#endif // !defined(AFX_MONSTERDATA_H__F06CA2D1_F4C9_4EF2_B607_B83D503B400C__INCLUDED_)
