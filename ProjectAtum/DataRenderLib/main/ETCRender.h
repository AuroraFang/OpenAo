// ETCRender.h: interface for the CETCRender class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ETCRENDER_H__34ED71FA_6125_49B2_80F1_C063122E993C__INCLUDED_)
#define AFX_ETCRENDER_H__34ED71FA_6125_49B2_80F1_C063122E993C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkinnedMesh.h"

class CETCRender : public CSkinnedMesh  
{
public:
	CETCRender();
	virtual ~CETCRender();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();

	void SkyRender();

	// 2010. 03. 15 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ó��)
	// ��ī�� �ڽ� �ٲٱ�( ��, �� �Ѵ� )
	void ChangeSkyBox( char* szSkyBoxName );
	// end 2010. 03. 15 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ó��)

	CSkinnedMesh *			m_pSky;
	CSkinnedMesh *			m_pSkyNight;
	D3DXMATRIX				m_mMatrix;
	CSkinnedMesh *			m_pCloud;
	CSkinnedMesh *			m_pCloudNight;
	float					m_fCloudAnimationTime;
};

#endif // !defined(AFX_ETCRENDER_H__34ED71FA_6125_49B2_80F1_C063122E993C__INCLUDED_)
