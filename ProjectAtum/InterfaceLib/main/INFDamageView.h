// INFDamageView.h: interface for the INFDamageView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFDAMAGEVIEW_H__7B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_)
#define AFX_INFDAMAGEVIEW_H__7B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#define NUMBER_PICTURE_DAMAGE	10

class CINFImage;
class CINFImageEx;
class DataHeader;
class CGameData;

typedef struct
{
	ClientIndex_t	TargetIndex;
	int				nDamageViewStep;	// ������ ��ܰ�
	float			fTimer;				// ������ ������ �ð�
	int				nDamageKind;
	vector<int>		vecDamageView;

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	float			fVerticalSpeed;
	float			fHorizontallySpeed;
	float			x, y;
	// 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���
	int				nMultiIndex;
	// end 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���
	
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

} DamageView_t;

class CINFDamageView : public CINFBase
{
public:
	CINFDamageView();
	virtual ~CINFDamageView();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();

public:
	// 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���
	//void RecvBattleShowDamage(ClientIndex_t	TIndex, int nDamage, int nDKind);
	void RecvBattleShowDamage(ClientIndex_t	TIndex, int nDamage, int nDKind, USHORT	MultiTargetIndex = NULL);
	// 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���
	
	vector<DamageView_t>	vecDamage;	

protected:	

	CINFImageEx			*	m_pNormal;
	CINFImageEx			*	m_pCritical;
	CINFImageEx			*	m_pMiss;
	// 2010-11-01 by jskim, �� ������ View ����
	CINFImageEx			*	m_pPet;
	// end 2010-11-01 by jskim, �� ������ View ����
	
	float					m_fTickTimer;						// �ð��� ���������� �������� ���� �ö󰣴� 5�ܰ谡 �Ǹ� ��������
//	D3DCAPS8				m_d3dCapsTest;
};

#endif
