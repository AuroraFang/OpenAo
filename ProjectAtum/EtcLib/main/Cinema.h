// Cinema.h: interface for the CCinema class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CINEMA_H__DA0207CA_981F_45C0_867F_3F05E38B6EFA__INCLUDED_)
#define AFX_CINEMA_H__DA0207CA_981F_45C0_867F_3F05E38B6EFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _ATUM_CLIENT

#include <vector>
#include "dbgout_c.h"

#endif // _ATUM_CLIENT_endif �ó׸������Ϳ����� �����

typedef struct
{
	D3DXVECTOR3		vPosition;			/// ��ġ
	D3DXVECTOR3		vDirection;			/// ����
	D3DXVECTOR3		vUpVector;			/// UpVector
	D3DXVECTOR3		vTarget;			/// ������ Target
	FLOAT			fVelocity;			/// �ӷ�
	FLOAT			fCurvature;			/// ���
} EVENT_POINT;


// ��� ũ�� 128����Ʈ
struct CINEMA_HEADER
{
	int			nEventNum;
	int			nPatternType;
	char		strDescription[120];
};

class CD3DFont;
class CCinema
{
public:

	CCinema();
	virtual ~CCinema();
	void		InitDefaultData();

	// 2010. 03. 18 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ��ų �߰�)
	//BOOL		WeaponTick(D3DXVECTOR3 i_vTargetPosition, float fElapsedTime);
	BOOL		WeaponTick( D3DXVECTOR3 i_vTargetPosition, float fElapsedTime, float fDecSpeed = 0.0f );
	// end 2010. 03. 18 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ��ų �߰�)

	BOOL		Tick(D3DXVECTOR3 i_vTargetPosition);
	BOOL		SkillTick();
	void		InitData(char* i_pData);	// cin ���� ����
	BOOL		InsertEventPoint(int index, EVENT_POINT EventPoint);	/// index ��ġ�� EventPoint�� �����Ѵ�.
																		/// index ���� 0 �� �Է��ϸ� Default�� �Ǹ����� ���ҷ� ���Եȴ�.
	void		InitCinemaData(EVENT_POINT i_StartEventPoint, BOOL bLHMode=TRUE);
	void		InitWeaponCinemaData(EVENT_POINT i_StartEventPoint, BOOL bLHMode, int Speed, float Angle);
	EVENT_POINT	GetCurrentCinemaPoint();
	std::vector<EVENT_POINT>	m_vecEventPoint;	/// EventPoint ������ ���� ����Ʈ
	CINEMA_HEADER	GetHeader() { return m_header;}

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	// �ó׸� ������ �����°�
	BOOL		IsCinemaOver( void )
	{
		vector<EVENT_POINT>::size_type size = m_vecEventPoint.size();	// ���� ������ Size�� ���Ѵ�.

		return (m_nTargetPointIndex >= size);
	}

	// ���� �ó׸������� �ӵ��� �����Ѵ�
	void		SetCurrentCinemaPointVelocity( float fVelocity )
	{
		m_CurrentCinemaPoint.fVelocity	= fVelocity;
	}
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

private:

	EVENT_POINT			m_CurrentCinemaPoint;		/// ���� ��ġ, ����, �ӷ�
	EVENT_POINT			m_CurrentCameraPoint;		/// ���� ī�޶� 
	EVENT_POINT			m_StartEventPoint;			/// ���� ��ġ�� ������ (�ӷ� ������ ���� �ʿ�)
	EVENT_POINT			m_TargetEventPoint;			/// ���� ��ġ�� ��ǥ�� 
	int					m_nTargetPointIndex;		/// ��ǥ�� ��ȣ

	BOOL				m_bNewEventPoint;			/// ���ο� �̺�Ʈ ������ �ΰ�
	D3DXVECTOR3			m_vDifferenceDirection;		/// ���� ����� Ÿ�� ������ ���� ����
	D3DXVECTOR3			m_vDifferenceUpVector;		/// ���� UpVector(�������� ���� ����)
	D3DXVECTOR3			m_vDifferenceTarget;		/// ���� Ÿ�� ������ ���� ����
	float				m_fTotalDistance;			/// �̹� ������ ��ü �Ÿ� (StartPoint �� TargetPoint �� �Ÿ�)
//	float				m_fDistanceToTarget;		/// ��� �� �̵��� Target ���� �Ÿ�
//	float				m_fTatalTargetDistance;		/// �̹� �������� Ÿ���� ��ó �Ÿ�

	BOOL				m_bInRange;					/// ��ǥ ��ó�� �����ߴ°�?
	int					m_nMaxVelocity;				/// �ִ� �ӷ�
	int					m_nMinVelocity;				/// �ּ� �ӷ�
	BOOL				m_bTarget;					/// Ÿ���� �ִ°�?
	float				m_fDistance;				/// �Ÿ�


	int					m_nNumOfCinema;				/// CinemaPoint����
	int					m_nNumOfEvent;				/// EventPoint����
	LPDIRECT3DVERTEXBUFFER9		m_pCinemaVB;		/// CinemaVertex
	LPDIRECT3DVERTEXBUFFER9		m_pEventVB;			/// EventVertex
	LPDIRECT3DVERTEXBUFFER9		m_pSelectedVB;		/// SelectedVertex
	LPDIRECT3DVERTEXBUFFER9		m_pCinemaUpVectorVB;/// CinemaUpVectorVertex
	LPDIRECT3DVERTEXBUFFER9		m_pEventUpVectorVB;	/// EventUpVectorVertex
	LPDIRECT3DVERTEXBUFFER9		m_pTargetVertexVB;	/// TargetVertex
	LPDIRECT3DVERTEXBUFFER9		m_pTargetLineVB;	/// TargetLine
	float				m_fMinimumLength;			/// LengthPoint���� ���Ǵ� ���� ���� (�ּ� �Ÿ�)
	int					m_nSelectedPoint;			/// ���õ� ������ �ε���
	BOOL				m_bRender;					/// �׸���?
	int					m_nFrame;					/// �ּ������� ����� ���ؼ�..
	BOOL				m_bUpVectorRender;			/// UpVector������ �ұ�?
	BOOL				m_bSelectedType;			/// ���õ� ���� ���ΰ�?
	BOOL				m_bTargetRender;			/// Target �� ������ �Ұ��ΰ�?
	CINEMA_HEADER		m_header;					/// header

	// 2005-12-02 by ispark, ���� ����
	int					m_nWeaponSpeed;				// ����Ʈ
	float				m_fWeaponAngle;				// ��ȸ��

	BOOL				m_bDefaultData;				// ����Ʈ 

#ifndef _ATUM_CLIENT
public:
	BOOL		Tick();	
	HRESULT		Render();
	BOOL		Reset();					/// Cinema Point �� �ٽ� ���ϰ� �ٽ� �׸���.
	BOOL		RemoveEventPoint(int index);							/// index ��ġ�� EventPoint�� �����Ѵ�.
	HRESULT		InitDeviceObjects();
	HRESULT		RestoreDeviceObjects();
	HRESULT		InvalidateDeviceObjects();
	HRESULT		DeleteDeviceObjects();
	BOOL		SaveEventPoints();
	BOOL		LoadEventPoints();
	BOOL		SaveCameraPoints();
	BOOL		LoadCameraPoints();
	BOOL		LengthPoint(D3DXVECTOR3 vEyes, D3DXVECTOR3 vDir, D3DXVECTOR3 vPosition);
	CPoint		GetPixelFromPosition(D3DXVECTOR3 i_vPosition, DWORD i_dwViewType, float i_fZoom);
	int			PickUpPoint(CPoint i_pointMouse, DWORD i_dwViewType, float i_fZoom);		/// ���콺�� 3D ������ Ŭ���� ���� �����Ѵ�.
	BOOL		UpdatePoint();					/// ��ȭ�� �̷������ Update�Ѵ�.
	HRESULT		CreateVB();						/// m_pCinemaVB �� m_pEventVB�� ������ Ȱ��/���� 
	void		MovePoint(CPoint i_pointMouseMove, DWORD i_dwViewType, float i_fZoom);		/// ���õ� �̺�Ʈ ������ �̵�
	void		Save(char* strName);
	void		Open(char* strName);
	int			GetSelectedPoint();
	void		UpVectorRender();					/// UpVector�� ������ �� ���ΰ�?
	void		ChangeUpVector(CPoint i_pointMouseMove,DWORD i_dwViewType, float i_fZoom);	/// ���õ� �̺�Ʈ ����Ʈ�� UpVector ����
	int			GetTargetIndex();
	BOOL		IsRender();
	void		TargetRender();
	BOOL		IsTargetRender();					/// ���� Ÿ�� ���͸� �������ϰ� �ִ°�?
	void		Next();
private:


#endif // _ATUM_CLIENT_endif �ó׸������Ϳ����� �����


};

#endif // !defined(AFX_CINEMA_H__DA0207CA_981F_45C0_867F_3F05E38B6EFA__INCLUDED_)

