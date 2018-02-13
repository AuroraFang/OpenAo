// EnemyData.h: interface for the CEnemyData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENEMYDATA_H__F5A0B417_10F7_42CF_AF08_22552909F46E__INCLUDED_)
#define AFX_ENEMYDATA_H__F5A0B417_10F7_42CF_AF08_22552909F46E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UnitData.h"

class CUnitRender;
class CCharacterRender;						// 2005-07-21 by ispark
class CShuttleRender;
class CPkNormalTimer;
class CChatMoveData;


class CEnemyData : public CUnitData  
{
public:
	CEnemyData(MSG_FC_CHARACTER_GET_OTHER_INFO_OK* pMsg);
	virtual ~CEnemyData();
	virtual void Tick();
	virtual void Render();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	void SetItemParamFactor( MSG_FC_CHARACTER_GET_OTHER_PARAMFACTOR_OK* pMsg );
	void SetPosition(MSG_FC_MOVE_OK* pMsg);
	void CheckShadowPos(BOOL bOnObject);
	float CheckOnObject();
	void Init();
	float GetObjectSize(int type);
	void InvalidateChat();
	void RestoreChat();
	void InitItemPoint();


	// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

	// 2012-11-29 by bhsohn ��Ʈ�� ���� ���� ��Ʈ�� ����
	//void ChangeItemPoint(BYTE bPos,INT nItemNum, INT nColorNum);
//	void	ChangeItemPoint( BYTE bPos, INT nItemNum, INT nShapeItemNum, INT nEffectItemNum );
	void	ChangeItemPoint( BYTE bPos, INT nItemNum, INT nShapeItemNum, INT nEffectItemNum, int nPetLevel=-1);
	// END 2012-11-29 by bhsohn ��Ʈ�� ���� ���� ��Ʈ�� ����

	// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����


	void MoveSkill();
//	void UseSkillFromServer(INT nSkillItemNumber, ClientIndex_t nTargetIndex);
	float RollingSystem(float fElapsedTime);
	void CheckRoll(MSG_FC_MOVE_ROLLING_OK *pMsg);

	// remodeling, 2004-03-21 by dhkwon
	void ChangeBodyConditionFromServer(BodyCond_t hyBodyCondition);
	virtual USHORT GetShiftWeaponBodyconditionByUnitKind();
	
	// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
	//virtual void CheckAttack(ATTACK_DATA &attackData, BOOL bReset, ITEM* pWeaponITEM=NULL);
	virtual void CheckAttack( ATTACK_DATA &attackData, BOOL bReset, ITEM* pWeaponITEM = nullptr, ITEM* pEffectItem = nullptr);
	// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

	virtual void CreateSecondaryShieldDamage(D3DXVECTOR3 vCollPos );

	// 2005-07-28 by ispark
	void SetPilotNumber(BYTE PilotFace);

	void DeleteChatMoveShop();

	// 2006-11-27 by ispark, ��ų ���� ����
	void UseSkillFromServer(INT ItemNum);
	void CancelUsingSkill(INT ItemNum);

	void SetAlphaData();
	void InitAlphaData();

	// 2007-05-17 by bhsohn ������Ʈ �ڿ� �������� �� ���� ó �˻� ó��
	virtual BOOL	IsCheckObjectShow();	
	virtual BOOL	IsCheckObjectColl();

	// 2007-06-13 by bhsohn �޸� ���� �����
	ENEMYINFO		GetEnemyInfo();

	// 2008-08-19 by bhsohn ���� ��ũ �ý��� �߰�
	void RefreshMarkItem();

	// 2013-03-06 by bhsohn ���� ���� �ý���
	// ���� �������� ����	
	virtual void SetRtnGameUser(BYTE byUsingReturnItem) {m_infoCharacter.CharacterInfo.UsingReturnItem = byUsingReturnItem;}
	virtual BYTE GetRtnGameUser() ;
	// END 2013-03-06 by bhsohn ���� ���� �ý���

protected:
	void CheckCharacterPos();								// 2005-08-04 by ispark
	void TickCharacter(float fElapsedTime);
	void TickUnit(float fElapsedTime);
	void CheckShadowPos();
	void LoadEnemyCharacterInfo(char *strFileName);

	// 2007-05-17 by bhsohn ������Ʈ �ڿ� �������� �� ���� ó �˻� ó��
	void TickCheckObjectTOEnemy();
	void UpdateCheckObjectTOEnemy(BOOL bTarget);
	void InitCheckInfo();
	
	// 2008-07-14 by bhsohn ĳ���� ���� ���� ����
	BOOL IsSameInfluence(BYTE	myInfluenceType, BYTE	emenyInfluenceType);

public:
	CAtumData		*		m_pTarget;
	///////////////// ���� �׽�Ʈ�� ���� ����Ʈ  /////////////////////
	BOOL					m_bUseMoveSkill;
	BYTE					m_bSkillMoveType;
	FLOAT					m_fSkillMoveTime;

	FLOAT					m_fTargetCheckTime;
//	DWORD				m_dwType;				// ĳ����, ���� ����

	D3DXVECTOR3				m_vNextUp;
	D3DXVECTOR3				m_vOriNextUp;
	D3DXVECTOR3				m_vNextVel;
	D3DXVECTOR3				m_vOriNextVel;
	D3DXVECTOR3				m_vExpectPos;		// NextPos�� �ι��� ����.

	D3DXVECTOR3				m_vMovepos;			// ������ ����.
	D3DXVECTOR3				m_vMoveUp;

	FLOAT					m_fLifeTime;
	BOOL					m_bCheckColl;
	BOOL					m_bIsRender;
	BOOL					m_bIsInit;

	BOOL					m_bRequestMove;
	POINT					m_ptCurRegion;					// ���� �� ������
	POINT					m_ptOldRegion;					// ���� ���� �� ������
	///////////////// Party  /////////////////////
	PartyID_t				m_nPartyID;
	
															// PK ����� FALSE ����
	BOOL					m_bZBuffer;						// �ν��Ϳ� �����ϱ� ����
	CUnitRender		*		m_pRender;						// Enemy �� Rendering �� ó��

	ENEMYINFO				m_infoCharacter;
	BOOL					m_bIsItemPointSet;				// ���������� ������ �����ߴ°�

	CAppEffectData	*		m_pEffectTarget;
	float					m_fCheckItemInfoTime;

	float					m_fRollStartTime;				// �Ѹ��� ������ �ð�
	float					m_fRollAngle;					// �Ѹ� ����
	BOOL					m_bRollStart;					// �Ѹ� ������
	BOOL					m_bRollLeft;					// �Ѹ� LEFT
	BOOL					m_bRollRight;					// �Ѹ� RIGHT
	D3DXVECTOR3				m_vRollStart;					// �Ѹ� ������ ��ġ

	BOOL					m_bRender;						// 2011-07-18 by jhahn ����3�� �ó׸� Ÿĳ���� �Ⱥ��̱�

	//////////////////////////////////////////////////////////////////////////
	// 2005-07-26 by ispark
	// �� ĳ����
	BOOL					m_bEnemyCharacter;				// �� ĳ���� // 2005-07-21 by ispark
	CCharacterInfo*			m_pEnemyCharacterInfo;			// �� ĳ���� ����
	CCharacterRender*		m_pCharacterRender;				// ĳ���� Rendering
	BodyCond_t				m_hyBodyCondition;

	// ���� ��ǥ : �ٵ�������� ���� ���� �Ѵ�.
	D3DXVECTOR3				m_vOldPos;						// �� ĳ���� ���� ��ǥ

	// 2005-11-03 by ispark
	///////////////// PK /////////////////////
	CPkNormalTimer*			m_pPkNormalTimer;				// PK Ÿ��
	

	BOOL					m_bSetPosition;

	//////////////////////////////////////////////////////////////////////////
	// ���� ������
	CChatMoveData	*		m_pMoveChatShop;
	BYTE					m_byBazaarType;
	char					m_szBazaarName[SIZE_MAX_BAZAAR_FULL_NAME];
	
#ifdef _DEBUG
	int						m_nDebugMoveCount;
	float					m_fCheckMoveTime;
#endif

};

#endif // !defined(AFX_ENEMYDATA_H__F5A0B417_10F7_42CF_AF08_22552909F46E__INCLUDED_)


