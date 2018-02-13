// Camera.h: interface for the CCamera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAMERA_H__A85A685E_A796_4B9D_B700_FA1F1B8282ED__INCLUDED_)
#define AFX_CAMERA_H__A85A685E_A796_4B9D_B700_FA1F1B8282ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "D3DUtil.h"
/*
#define CAM_NORMAL_SPEED				10
#define OBJECT_SCCREENX_MIN_RATE		0.15
#define OBJECT_SCCREENX_MAX_RATE		0.85
#define OBJECT_SCCREENY_MIN_RATE		0.55
#define OBJECT_SCCREENY_MAX_RATE		0.9
*/
class CEnemyData;
class CCamera : public CD3DCamera  
{
public:
	CCamera();
	virtual ~CCamera();
	void Init( D3DXVECTOR3 vPos, D3DXVECTOR3 vVel , FLOAT angle, FLOAT distance );
	void Init( D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd , 
			   D3DXVECTOR3 vTargetPos, FLOAT angle, FLOAT distance );
	void Init(D3DXVECTOR3 vPos, D3DXVECTOR3 vVel);								// 2005-08-10 by ispark
	void Init();																// 2007-08-07 by dgwoo
	void SetCamPos( FLOAT angle, FLOAT distance );
	void ObserveTick();
	void Tick();//float fElapsedTime, D3DXVECTOR3 vObjPos,D3DXVECTOR3 vObjFBVel, bool bDead);
	void Tick(D3DXVECTOR3 vObjPos,D3DXVECTOR3 vObjFBVel,D3DXVECTOR3 vObjUp, BYTE type);
	void Tick(D3DXVECTOR3 vObjPos,D3DXVECTOR3 vObjFBVel,D3DXVECTOR3 vObjUp);//D3DXMATRIX m);
	void SetDirection(D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd , D3DXVECTOR3 vTargetPos );
	void SetControlCamPos(int nDir);
	void SetControlCamAngle(SHORT sDir);
	void SetControlCamSideAngle(SHORT sDir);
	float CheckMap(D3DXVECTOR3 vPos);
	BYTE GetCamType()							{return m_bCamType;}
	void SetCamType(BYTE bType);
	VOID ChangeCamType();
	FLOAT GetCamDistance()						{return m_fDistance;}
	FLOAT GetCamHeight()						{return m_fHeight;}
	void SetCamDistance(float fDist)			{m_fDistance = fDist;}
	void SetCamHeight(float fHeight)			{m_fHeight = fHeight;}
	void SetCamCollType(BYTE bType)				{m_bCamCollType = bType;}

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	//void ChangeCamCollType();
	void ChangeCamCollType( int nType = -1, float fTime = 7.0f );
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���


	byte CheckCullCam(D3DXVECTOR3 vPos);
	void SetDetailGround(byte bType,bool bSet = false);

	void SetCityCamera();
	void SetCameraZoomInOut(int nDir);
	void SetCameraRotate(int x, int y);
	BOOL CheckCollForMap(D3DXVECTOR3 vPos);
	BOOL CheckCollMapObject(D3DXVECTOR3 &vPos);

	// 2004-08-17 jschoi
	float		GetDistanceToViewPlane(D3DXVECTOR3 vPos);	// ī�޶��� ���� �Ÿ����ϴ� �Լ�
	void		SetViewPlane();								// ī�޶��� ����� ����
	D3DXPLANE	GetViewPlane();								// ī�޶��� ����� ����
	void		SetSmoothMove();

	// 2005-07-13 by ispark
	BOOL RotationLocal3Y(float fangle);						// ī�޶� 3��Ī Y�� ȸ��
	BOOL RotationLocal3X(float fangle);						// ī�޶� 3��Ī X�� ȸ��

	// 2005-08-16 by ispark
	void CharacterCamTick();
	void SetAngleX(float fangleX) { m_fAngleX = fangleX; }
	void SetAngleY(float fangleY) { m_fAngleY = fangleY; }
	void SetCamMove(BOOL bRButton, BOOL bWhell, BOOL bWarp = FALSE);

	float CheckCollMapObject(D3DXVECTOR3 vPos, D3DXVECTOR3 vLookAt);

	float GetCollDistance() { return m_fCollDistance; }		// 2005-08-19 by ispark

	// 2009-02-19 by bhsohn ī�޶� �ü� ������, ���� ��� ��� ����
	float GetOriDistance() {return m_fOriDistance;}// ����Ʈ ī�޶��� ��Ʋ�κ��� �Ÿ�
	// end 2009-02-19 by bhsohn ī�޶� �ü� ������, ���� ��� ��� ����

public:
	D3DXVECTOR3	m_vCamNextPos;		// ī�޶� ���� ��ġ
	D3DXVECTOR3	m_vNextCamVel;		// ī�޶� �ӵ�
	D3DXVECTOR3	m_vObjectPos;		// ȭ�鿡 ������ ��Ÿ���� �ϴ� object position
	D3DXVECTOR3	m_vNextTargetPos;	// ī�޶� �ٶ󺸴� �ʿ����� ��ġ
	D3DXVECTOR3 m_vDistance;
	D3DXVECTOR3 m_vHeight;
	D3DXVECTOR3 m_vObjOldPos;
	D3DXVECTOR3 m_vCollCamPos;		// �浹�� Ÿ�ݽ� ī�޶� ���� ���� ����

	FLOAT		m_fOriDistance;		// ����Ʈ ī�޶��� ��Ʋ�κ��� �Ÿ�
	FLOAT		m_fOriHeight;		// ����Ʈ ī�޶��� ��Ʋ�κ��� ����
	FLOAT		m_fAngle;
	FLOAT		m_fSideAngle;
	BOOL		m_bTurning;
	FLOAT		m_fCheckCollTime;	// ī�޶� ���� �ð� ����
	BYTE		m_bCamCollType;		// ī�޶� ���� Ÿ��

	BYTE		m_bCamType;
public:
	D3DXVECTOR3	m_vCamCurrentPos;	// ī�޶� ���� ��ġ
	D3DXVECTOR3	m_vTargetStart;		// ���� ����
	D3DXVECTOR3	m_vTargetEnd;		// ���� ��
	D3DXVECTOR3 m_vCurrentTargetPos;

	FLOAT		m_fDistance;		// ���� ī�޶��� ��Ʋ�κ��� �Ÿ�
	FLOAT		m_fHeight;			// ���� ī�޶��� ��Ʋ�κ��� ����

	FLOAT		m_fAngleX;			// 2005-08-16 by ispark
	FLOAT		m_fAngleY;			

	// 2004.06.23 jschoi
	D3DXVECTOR3 m_vOldCamEyePos;	// ���� ī�޶� �� ��ġ
	FLOAT		m_fOldHeight;		// ���� Height
	FLOAT		m_fOldDistance;		// ���� Distance
	FLOAT		m_fOldSideAngle;	// ���� ȸ�� ��

	BOOL		m_bIsCamControl;	// ī�޶� ��Ʈ�� ���ΰ�
	FLOAT		m_fCamControlCheckTime;	// ī�޶� ��Ʈ�ѽ� Ÿ�� üũ�Ͽ� Ÿ�� ������ �ε巴���Ѵ�.
	FLOAT		m_fCheckSpeedRate;		// ī�޶��� ���󰡴� �ӵ�

	// ���� �ø��� ���� ������
	D3DXPLANE	m_pl[4];				// ���� �ø��� ���� 4���� ���
	FLOAT		m_fMinX;				// �ø��� ���� �ּ� X
	FLOAT		m_fMaxX;				// �ø��� ���� �ִ� X
	FLOAT		m_fMinY;				// �ø��� ���� �ּ� Y
	FLOAT		m_fMaxY;				// �ø��� ���� �ִ� Y
	FLOAT		m_fDetailRateGround;	// �������� ���� (������ ���� Ÿ�� ���� ����ȴ�)
	FLOAT		m_fRenderDistance;		// ������ ���� (�Ÿ�)

	// ������ ���� ������ȯ 
	D3DXVECTOR3 m_vCamSlowNextPos;
	FLOAT		m_fFrameLerp;

	// 2004-08-17 jschoi
	D3DXPLANE	m_planeView;			// ī�޶� ���
	BOOL		m_bSmoothMove;			// ī�޶� ���� õõ�� ���ư�����
	float		m_fSmoothTime;			// �ε巯�� ī�޶� �ִ� ���ӽð�

//	// 2005-07-20 by ispark
//	// ĳ���� �϶� ���� ������
//	D3DXVECTOR3		m_vChaVel;			// ĳ���͸� �Ĵ� ���� ����. ó���� �ѹ� ���� �� ȸ������ ���⿡ ���Ԥ�

	// 2005-08-18 by ispark
	float		m_fTotalAngleX;			// ������ ����
	float		m_fAngleError;			// ���� ���� : ó�� ī�޶� ��ġ ���ÿ� ���� ī�޶� ��,�� ������ �α� ���ؼ�
										//			   ���������� ������ �����Ѵ�. �� ������ ���� ���� Define�� ���δ�.
	// 2005-08-19 by ispark
	float		m_fCollDistance;		// �浹�� ī�޶��� ĳ���ͷκ��� �Ÿ�


};

#endif // !defined(AFX_CAMERA_H__A85A685E_A796_4B9D_B700_FA1F1B8282ED__INCLUDED_)
