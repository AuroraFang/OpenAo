// INFFadeEffect.cpp: implementation of the CINFFadeEffect class.
//
//////////////////////////////////////////////////////////////////////

#include "INFFadeEffect.h"
#include "AtumApplication.h"
#include "dxutil.h"
#include "SpriteDrawFix.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFFadeEffect::CINFFadeEffect()
{
	// 2010-09-29 by jskim, SpriteDevice Lost �Ǿ��� �� ���� ����
	//m_pd3dxSprite	= NULL;
	// end 2010-09-29 by jskim, SpriteDevice Lost �Ǿ��� �� ���� ����
	m_pTexture		= NULL;
	m_fAlpha		= 0.0f;
	m_bFadeIn		= TRUE;
	m_nTime			= 0;
	m_dwColor		= (D3DCOLOR)0x00000000;
}

CINFFadeEffect::~CINFFadeEffect()
{
	// 2010-09-29 by jskim, SpriteDevice Lost �Ǿ��� �� ���� ����
	//SAFE_RELEASE(m_pd3dxSprite);
	// end 2010-09-29 by jskim, SpriteDevice Lost �Ǿ��� �� ���� ����
	SAFE_RELEASE(m_pTexture);
}

HRESULT CINFFadeEffect::InitDeviceObjects()
{
	return S_OK;
}

HRESULT CINFFadeEffect::RestoreDeviceObjects()
{
	// 2010-09-29 by jskim, SpriteDevice Lost �Ǿ��� �� ���� ����
	//D3DXCreateSprite(g_pD3dDev , &m_pd3dxSprite);
	// end 2010-09-29 by jskim, SpriteDevice Lost �Ǿ��� �� ���� ����
	g_pD3dDev->CreateTexture(1,1,1,0,D3DFMT_A4R4G4B4,D3DPOOL_MANAGED,&m_pTexture,NULL);
    D3DLOCKED_RECT rect;
    m_pTexture->LockRect( 0, &rect, 0, 0 );
	DWORD* pDword = (DWORD*)rect.pBits;
	*pDword = (D3DCOLOR)0xffffffff;
    m_pTexture->UnlockRect(0);
	return S_OK;	
}

HRESULT CINFFadeEffect::DeleteDeviceObjects()
{
	return S_OK;
}

HRESULT CINFFadeEffect::InvalidateDeviceObjects()
{
	// 2010-09-29 by jskim, SpriteDevice Lost �Ǿ��� �� ���� ����
	//SAFE_RELEASE(m_pd3dxSprite);
	// end 2010-09-29 by jskim, SpriteDevice Lost �Ǿ��� �� ���� ����
	SAFE_RELEASE(m_pTexture);
	return S_OK;
}

void CINFFadeEffect::Render()
{
	if(m_fAlpha>0.0f || m_bFadeIn == FALSE)
	{
		D3DXVECTOR2 v2Scaling(g_pD3dApp->GetBackBufferDesc().Width,g_pD3dApp->GetBackBufferDesc().Height);
		D3DCOLOR dwColor;
		if(m_bFadeIn)
		{
			dwColor = (D3DCOLOR)((int)(0xff*m_fAlpha) << 24 | m_dwColor);
		}
		else
		{
			if(m_fAlpha < 0.0f)
			{
				m_fAlpha = 0.0f;
			}
			dwColor = (D3DCOLOR)((int)(0xff*(1.0f-m_fAlpha)) << 24 | m_dwColor);
		}
		// 2010-09-29 by jskim, SpriteDevice Lost �Ǿ��� �� ���� ����
		//m_pd3dxSprite->Draw(m_pTexture,NULL,&v2Scaling,NULL,NULL,NULL,dwColor);
		//g_pD3dApp->GetDirectSprite()->Draw(m_pTexture, NULL, &v2Scaling, NULL, NULL, NULL, dwColor);
		SpriteDrawFix(g_pD3dApp->GetDirectSprite(), m_pTexture, NULL, &v2Scaling, NULL, NULL, NULL, dwColor);
		// end 2010-09-29 by jskim, SpriteDevice Lost �Ǿ��� �� ���� ����
	}
}

void CINFFadeEffect::Tick(float fElapsedTime)
{
	m_bFadIng = FALSE;
	if(m_fAlpha>0.0f && fElapsedTime < 0.3f)
	{
		m_fAlpha -= fElapsedTime/m_nTime;
		m_bFadIng = TRUE;
	}
}

void CINFFadeEffect::SetFadeEffect(BOOL bFadeIn, int nTime, D3DCOLOR dwColor)
{
	m_bFadeIn	= bFadeIn;
	m_nTime		= nTime;
	m_dwColor	= dwColor;
	m_fAlpha	= 1.0f;
}

void CINFFadeEffect::ReleaseFadeEffect()
{
	m_bFadeIn = TRUE;
	m_fAlpha = 0.0f;
}