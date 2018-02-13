// INFFadeEffect.h: interface for the CINFFadeEffect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFFADEEFFECT_H__0FA467D2_D210_4B70_9F9C_9C8B51888502__INCLUDED_)
#define AFX_INFFADEEFFECT_H__0FA467D2_D210_4B70_9F9C_9C8B51888502__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"

///////////////////////////////////////////////////////////////////////////////
/// \class		CINFFadeEffect
///
/// \brief		Fade-In & Out ȿ���� ����.
/// \author		jschoi
/// \version	
/// \date		2004-12-13 ~ 2004-12-13
/// \warning	����� Fade-In �Ǵ� Fade-Out ȿ���� ���� ������, SetFadeEffect()�Լ��� ȣ���Ѵ�.
///				SetFadeEffect(	BOOL bFadeIn,		// Fade-Inȿ���� TRUE / Fade-Out ȿ���� FALSE �Է�
///								int nTime,			// ȿ���� ���ӽð�(��)
///								D3DCOLOR dwColor);	// ��
///////////////////////////////////////////////////////////////////////////////
class CINFFadeEffect 
{
public:
	CINFFadeEffect();
	virtual ~CINFFadeEffect();
	virtual HRESULT		InitDeviceObjects();
	virtual HRESULT		RestoreDeviceObjects();
	virtual HRESULT		DeleteDeviceObjects();
	virtual HRESULT		InvalidateDeviceObjects();
	virtual void		Render();
	virtual void		Tick(float fElapsedTime);
	void				SetFadeEffect(BOOL bFadeIn, int nTime, D3DCOLOR dwColor);
	void				ReleaseFadeEffect();
	float				GetCurrentAlpha() { return m_fAlpha; }
	BOOL				GetFadeIngFlag() { return m_bFadIng;}

private:
	//LPD3DXSPRITE		m_pd3dxSprite;
	LPDIRECT3DTEXTURE9	m_pTexture;
	float				m_fAlpha;
	BOOL				m_bFadeIn;
	int					m_nTime;
	D3DCOLOR			m_dwColor;
	BOOL				m_bFadIng;
};

#endif // !defined(AFX_INFFADEEFFECT_H__0FA467D2_D210_4B70_9F9C_9C8B51888502__INCLUDED_)
