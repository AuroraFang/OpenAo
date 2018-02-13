// CharacterChild.h: interface for the CCharacterChild class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARACTERCHILD_H__3926B3F0_BE13_4B06_8B22_FA8FA691CD25__INCLUDED_)
#define AFX_CHARACTERCHILD_H__3926B3F0_BE13_4B06_8B22_FA8FA691CD25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UnitData.h"

class CSkill;
class CCharacterRender;
class CObjectChild;
class CPkNormalTimer;
class CChatMoveData;

class CCharacterChild : public CAtumData
{
public:
	CCharacterChild();
	virtual ~CCharacterChild();
	virtual void Tick(float fElapsedTime);
	virtual void Render();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();

	void WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void InitCharacterData();
	void FineObjectTakeOff();
	BOOL GetbPickMove() { return m_bPickMove; }
	BOOL CheckEnterShopNPC(POINT *pt, float fLength=50.0f, BOOL nFlag = FALSE);
	CObjectChild *GetEventObject(float fDistance);
	CObjectChild *GetPickingObj(){return m_pSelectObject;}
	CUnitData * GetPickingBazaar(){return m_pSelBazaarShop;}
	D3DXVECTOR3 GetVecPickDir() { return m_vPickDir; }
	BOOL GetbBazaarEvent() {return m_bBazaarEventPos;}

	void LoadCharacterEffect(char* strFileName);

	void DeleteChatMoveShop();

	float GetCharacterSpeed(){ return m_fCharacterSpeed; }

	void SetPartner(char* i_szPartner);											// 2010-06-08 by dgwoo, ��ý��� �߰�. 
	// 2010-11-29 by jskim, �� ������ �̵��ӵ� ����
	void SetPetWearItem( BOOL flag ) { m_bPetWearItem = flag; }
	BOOL GetPetWearItem() { return m_bPetWearItem; }
	void SetPetAddSpeed( BOOL flag ) { m_bSetSpeed = flag; }
	BOOL IsPetAddSpeed() { return m_bSetSpeed; }
	void SetCharacterAddSpeed( float fSpeed ) { m_fCharacterAddSpeed = fSpeed; }
	float GetCharacterAddSpeed() { return m_fCharacterAddSpeed; }
	// end 2010-11-29 by jskim, �� ������ �̵��ӵ� ����

	// 2013-06-10 by bhsohn ��������Ʈ ������ - ���� '�ɸ����̵��ӵ�' ����
	float	m_fAdminModeSpeed;
	// END 2013-06-10 by bhsohn ��������Ʈ ������ - ���� '�ɸ����̵��ӵ�' ����

protected:
	void CheckMoveRate(float fElapsedTime);
	void CheckShadowPos();
	void CheckAniTime(float fElapsedTime);
	void CheckState();
	void CheckEventTypeObject(float fElapsedTime);

	void InitEffect();
	void LoadCharacterEffect(CCharacterInfo** pCharacterInfo, char* strFileName);

	void Move_Character(float fElapsedTime);
	void CameraMoveTick();														// 2005-08-16 by ispark
	void SetMatrix_Move_Character(float fElapsedTime);
	void SetMatrix_Picking(D3DXVECTOR3 vPos, D3DXVECTOR3 vNorPos);
	D3DXVECTOR3 SetMoveVelByEventObject(BYTE bObjectType, float fDist);			// 2006-07-20 by ispark

	HRESULT RestoreShadow();
	void SendRequestEventObjectWarpIn(CObjectChild * pObj);						// 2006-07-19 by ispark

	void ChangeWearItemBodyConditionAllProcess(BodyCond_t i_nBodyCondition);
	
public:
	//////////////////////////////////////////////////////////////////////////
	// ĳ���� 
	int						m_nUnitNum;

//	D3DXVECTOR3				m_vPos;					// ������ ���� ��ǥ
//	D3DXVECTOR3				m_vNextPos;				
//	D3DXVECTOR3				m_vUp;
//	D3DXVECTOR3				m_vVel;
//  D3DXVECTOR3				m_vSideVel;				// ������ ����
// 
//	INT						m_nObjScreenX;			// ��ũ���� X ��ǥ
//	INT						m_nObjScreenY;			// ��ũ���� Y ��ǥ
//	INT						m_nObjScreenW;			// ��ũ���� W ��ǥ
//
//	CCharacterInfo*			m_pCharacterInfo;		// ĳ���� �ٵ� �����
	CCharacterInfo*			m_pPickingInfo;			// Picking ����Ʈ

	FLOAT					m_fCurrentTime;
	D3DXMATRIX				m_mMatrix;
	LPDIRECT3DVERTEXBUFFER9 m_pVBShadow;			// �׸��� ����

	int						m_nStartEventType;		// ���� �̺�Ʈ ��ȣ
	int						m_nSelBazaarType;		// 2006-07-29 by ispark, ������ ���λ��� Ÿ��
	ClientIndex_t			m_ShopEnemyClientIndex;	// 2006-07-29 by ispark, ������ ���λ��� Ŭ���̾�Ʈ �ε���

	CUnitData *				m_pSelBazaarShop;		// ���� ���λ��� ����

	//////////////////////////////////////////////////////////////////////////
	// ���� ������
	CChatMoveData	*		m_pMoveChatShop;

protected:
	//////////////////////////////////////////////////////////////////////////
	// ĳ����
	CCharacterRender*		m_pCharacterRender;		// Character�� Rendering ó��

	float					m_fCharacterAddSpeed;
	float					m_fCharacterSpeed;
	DWORD					m_dwState;

	D3DXVECTOR3				m_vShadowPos[4];		// �׸���
	BOOL					m_bCharacterRender;		// ĳ���� ���� �ұ�?
	//////////////////////////////////////////////////////////////////////////
	// ���콺, Ű
	BOOL					m_bRButtonState;
	BOOL					m_bMouseMove;			// 2005-08-16 by ispark
	BOOL					m_bMouseWheel;			// 2005-08-17 by ispark
	POINT					m_pOldMousePoint;

	//////////////////////////////////////////////////////////////////////////
	// Picking
	D3DXMATRIX				m_mPickMatrix;			// Picking Matrix
	D3DXVECTOR3				m_vPicking;				// 2005-07-15 by ispark Picking�� ��ǥ
	D3DXVECTOR3				m_vPickDir;				// Picking�� ����
	BOOL					m_bPickMove;			// Picking Move ���ΰ�
//	int						m_nSelectObjectIndex;	// ���� ������Ʈ
	CObjectChild *			m_pSelectObject;		// ���� ������Ʈ
	CObjectChild *			m_pShopObject;
	BOOL					m_bEnterShopState;
	float					m_fOldLength;			// �� ���� �Ÿ�
	
	//////////////////////////////////////////////////////////////////////////
	// �ٵ� �����
	BodyCond_t				m_hyBodyCondition;

	//////////////////////////////////////////////////////////////////////////
	// ��Ÿ
	BOOL					m_bStartPositionEvent;	// 2006-07-20 by ispark, �� ��ġ�� �̺�Ʈ ������ ��ġ�̳�.(�ʱ� ������ ���� ���Ѱ�)
	BOOL					m_bBazaarEventPos;		// 2006-07-28 by ispark, ���� ������ �� �� �ִ� ��ġ�ΰ�

	float					m_fCharacterWarpErrTime; // 2006-08-07 by ispark, ĳ���� ������ �������� �ʱ� ���ؼ� ���� �ð����� ������ ������ ������ �ǵ����� ����
//	float					m_fCharacterMoveRate;	// 2006-11-02 by ispark, �׽�Ʈ��
	// 2010-11-29 by jskim, �� ������ �̵��ӵ� ����
private:
	BOOL					m_bPetWearItem;
	BOOL					m_bSetSpeed;
	// end 2010-11-29 by jskim, �� ������ �̵��ӵ� ����

	//2012-06-25 by jhahn, �� ������ �̵��ӵ� ���� ����߰�
	int						PetoptionSpeed ;
	//end 2012-06-25 by jhahn, �� ������ �̵��ӵ� ���� ����߰�
};

#endif // !defined(AFX_CHARACTERCHILD_H__3926B3F0_BE13_4B06_8B22_FA8FA691CD25__INCLUDED_)
