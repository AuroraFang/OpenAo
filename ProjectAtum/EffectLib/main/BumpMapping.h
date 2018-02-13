// BumpMapping.h: interface for the CBumpMapping class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUMPMAPPING_H__CDB521E0_1471_4313_9D78_E945EE08ECF5__INCLUDED_)
#define AFX_BUMPMAPPING_H__CDB521E0_1471_4313_9D78_E945EE08ECF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBumpMapping  
{
public:
	CBumpMapping();
	virtual ~CBumpMapping();
	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT Render(LPDIRECT3DTEXTURE9 m_pTexture, int m_nTriangleNumber);
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();

public:
	LPDIRECT3DTEXTURE9		m_pNormalMap;	// ������
	LPDIRECT3DVERTEXDECLARATION9	m_pDecl;// ��������
	// ���̴�
	LPD3DXEFFECT		    m_pEffect;		// ����Ʈ
	D3DXHANDLE				m_hTechnique;	// ��ũ��
	D3DXHANDLE				m_hmWVP;		// ����-������ȯ���
	D3DXHANDLE				m_hvLightDir;	// ��������
	D3DXHANDLE				m_hvColor;		// ������
	D3DXHANDLE				m_hvEyePos;		// ������ ��ġ
	D3DXHANDLE				m_htDecaleTex;	// �����ؽ�ó
	D3DXHANDLE				m_htNormalMap;	// ������


};

#endif // !defined(AFX_BUMPMAPPING_H__CDB521E0_1471_4313_9D78_E945EE08ECF5__INCLUDED_)
