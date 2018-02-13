#pragma once

#include "INFBase.h"
#include "RangeTime.h"

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
// 2011. 10. 10 by jskim UI�ý��� ����
enum
{
	FADE_IN,
	FADE_OUT,
	FADE_NORMAL
};																								  
#endif

class CINFImage;
class CINFImageEx;		// 2011. 10. 10 by jskim UI�ý��� ����

class CINFUnitCreateInfo : public CINFBase
{
public:
	explicit CINFUnitCreateInfo(CAtumNode* pParent);
	virtual ~CINFUnitCreateInfo();


	HRESULT InitDeviceObjects() override;
	HRESULT RestoreDeviceObjects() override;
	HRESULT DeleteDeviceObjects() override;
	HRESULT InvalidateDeviceObjects() override;


	void Render(int nUnitKind);
	virtual void Tick(float fElapedTime);
	void StartView();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	void SetFadeMode(int nMode);		// 2011. 10. 10 by jskim UI�ý��� ����
	void SetBkAlpha(DWORD dwColor) { m_dwBkAlpha = dwColor; }		// 2011. 10. 10 by jskim UI�ý��� ����
	void SetSelGear(int num) { m_nSelGear = num; }		// 2011. 10. 10 by jskim UI�ý��� ����
#endif

protected:
	BOOL			m_bRestored;
	//img data
	CINFImageEx*	m_pBGear;		// 2011. 10. 10 by jskim UI�ý��� ����
	CINFImageEx*	m_pIGear;		// 2011. 10. 10 by jskim UI�ý��� ����
	CINFImageEx*	m_pMGear;		// 2011. 10. 10 by jskim UI�ý��� ����
	CINFImageEx*	m_pAGear;		// 2011. 10. 10 by jskim UI�ý��� ����

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFImageEx*	m_pBack;		// 2011. 10. 10 by jskim UI�ý��� ����
	CINFImageEx*	m_pGearInfo[4];		// 2011. 10. 10 by jskim UI�ý��� ����				   
#endif

	//tick data
	CRemainTime     m_tRemainTime;
	float			m_fIncreaseViewingTime;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
// 2011. 10. 10 by jskim UI�ý��� ����
	int				m_nFadeMode;		
	float			m_fFadeInTime;
	DWORD			m_dwBkAlpha;

	int				m_nSelGear;
// end 2011. 10. 10 by jskim UI�ý��� ����														  
#endif
};