#pragma once

#include "INFBase.h"
#include "rangetime.h"

// Unit Warning and State
enum INFAttackDirection
{
	INF_AD_DIRECTION0,
	INF_AD_DIRECTION1,
	INF_AD_DIRECTION2,
	INF_AD_DIRECTION3,
	INF_AD_DIRECTION4,
	INF_AD_DIRECTION5,
	INF_AD_DIRECTION6,
	INF_AD_DIRECTION7,

	INF_AD_DIRECTION_MAX
	
};


/////////////////////////////////////////////////////////////////////////////////
///	\class 	CINFAttackDirection
/// \brief  ������ ���� ���ݴ������� ������ ���� ������ �˾ƺ��� ���� �Ϸ���
/////////////////////////////////////////////////////////////////////////////////
class CINFImage;
class CINFImageEx;								  // 2011. 10. 10 by jskim UI�ý��� ����
class CINFAttackDirection  : public CINFBase
{
public:
	CINFAttackDirection();
	explicit CINFAttackDirection(CAtumNode* pParent);
	virtual ~CINFAttackDirection();

	HRESULT InitDeviceObjects() override;
	HRESULT RestoreDeviceObjects() override;
	HRESULT DeleteDeviceObjects() override;
	HRESULT InvalidateDeviceObjects() override;

	// main function
	void Tick() override;
	void Render() override;

	// sub function
	void OnAttackDirection( DWORD dwShiftNum );					//
	void OffAttackDirection( DWORD dwShiftNum );
	BOOL IsSeted( DWORD dwDirection );

public:
	// init()
	bool m_bRestored;

	// image data
	CINFImageEx	*m_pImgAttackDirection[INF_AD_DIRECTION_MAX];	// ������ ���� �������� ���� ���� �̹���

	// tick(), render()
	DWORD m_dwAttackDirectionBits;								// ������⿡�� �¾Ҵ��� �V�õǴ� 32��Ʈ Flag
	CRemainTime tRemainTime[INF_AD_DIRECTION_MAX];				// �V�õ� �� m_dwAttackDirectionBits�� ���� �ð�
};