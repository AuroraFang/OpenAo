// BumpMapping.cpp: implementation of the CBumpMapping class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "AtumApplication.h"
#include "DXUtil.h"
#include "d3dutil.h"
#include "dxerr9.h"
#include "BumpMapping.h"



//-------------------------------------------------------------
// ��������
//-------------------------------------------------------------
D3DVERTEXELEMENT9 decl[] =
{
	{0,  0, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	{0, 12, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,	0},
	{0, 24, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,	0},
	{0, 36, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
	D3DDECL_END()
};



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBumpMapping::CBumpMapping()
{
	m_pDecl						= NULL;
	m_pNormalMap				= NULL;

	m_pEffect					= NULL;
	m_hTechnique  				= NULL;
	m_hmWVP  					= NULL;
	m_hvLightDir  				= NULL;
	m_hvColor	  				= NULL;
	m_hvEyePos					= NULL;
	m_htDecaleTex  				= NULL;
	m_htNormalMap				= NULL;
}

CBumpMapping::~CBumpMapping()
{

}



HRESULT CBumpMapping::InitDeviceObjects()
{
    HRESULT hr;
    LPDIRECT3DTEXTURE9	pHeightTexture;
    D3DSURFACE_DESC desc;

	// ������ ����
    D3DUtil_CreateTexture( g_pD3dDev,// ���̸� �б�
		_T("height_test.bmp"), &pHeightTexture );
    pHeightTexture->GetLevelDesc(0,&desc);// �ؽ�ó ���� ���
    D3DXCreateTexture(g_pD3dDev, desc.Width, desc.Height, 0, 0, 
        D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &m_pNormalMap);// �ؽ�ó ����
    D3DXComputeNormalMap(m_pNormalMap,	// ������ ����
        pHeightTexture, NULL, 0, D3DX_CHANNEL_RED, 1.0f);
    SAFE_RELEASE( pHeightTexture );		// �ʿ������ ���ҽ� ����

	// �������� ������Ʈ ����
	if( FAILED( hr = g_pD3dDev->CreateVertexDeclaration(
										decl, &m_pDecl )))
		return DXTRACE_ERR ("CreateVertexDeclaration", hr);
	
	
	// ���̴� �б�
    LPD3DXBUFFER pErr=NULL;
    if( FAILED( hr = D3DXCreateEffectFromFile(
                g_pD3dDev, "hlsl.fx", NULL, NULL, 
                0 , NULL, &m_pEffect, &pErr ))){
        // ���̴� �б� ����
        MessageBox( NULL, (LPCTSTR)pErr->GetBufferPointer()
                    , "ERROR", MB_OK);
    }else{
		m_hTechnique = m_pEffect->GetTechniqueByName( "TShader" );
		m_hmWVP      = m_pEffect->GetParameterByName( NULL, "mWVP" );
		m_hvLightDir = m_pEffect->GetParameterByName( NULL, "vLightDir" );
		m_hvColor    = m_pEffect->GetParameterByName( NULL, "vColor" );
		m_hvEyePos   = m_pEffect->GetParameterByName( NULL, "vEyePos" );
		m_htDecaleTex= m_pEffect->GetParameterByName( NULL, "DecaleTex" );
		m_htNormalMap= m_pEffect->GetParameterByName( NULL, "NormalMap" );
    }
    SAFE_RELEASE(pErr);

    return S_OK;
}

HRESULT CBumpMapping::RestoreDeviceObjects()
{
	// ����Ʈ
	if(m_pEffect) m_pEffect->OnResetDevice();

	return S_OK;
}


HRESULT CBumpMapping::Render(LPDIRECT3DTEXTURE9 m_pTexture,int m_nTriangleNumber)
{
	if( m_pEffect != NULL ) 
	{
		g_pD3dDev->SetRenderState( D3DRS_ZENABLE,        TRUE );
		g_pD3dDev->SetRenderState( D3DRS_LIGHTING, FALSE );

		g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
		g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );

		//-------------------------------------------------
		// ���̴� ����
		//-------------------------------------------------
		m_pEffect->SetTechnique( m_hTechnique );
		m_pEffect->Begin( NULL, 0 );
		m_pEffect->Pass( 0 );
		

		//-------------------------------------------------
		// ������ ����
		//-------------------------------------------------
		D3DXMATRIX m,matWorld,matView,matProj;
		g_pD3dDev->GetTransform(D3DTS_WORLD		, &matWorld);
		g_pD3dDev->GetTransform(D3DTS_VIEW		, &matView);
		g_pD3dDev->GetTransform(D3DTS_PROJECTION, &matProj);
		
		// ����-���� ��ȯ���
		m = matWorld * matView * matProj;
		m_pEffect->SetMatrix( m_hmWVP, &m );

		// ������ ����(������ǥ��)
		D3DXVECTOR4 light_pos = D3DXVECTOR4( -0.577f, -0.577f, -0.577f,0);
		D3DXVECTOR4 v;
		D3DXMatrixInverse( &m, NULL, &matWorld);
		D3DXVec4Transform( &v, &light_pos, &m );
		D3DXVec3Normalize( (D3DXVECTOR3 *)&v, (D3DXVECTOR3 *)&v );
		v.w = -1.0f;		// ȯ�汤 ����
		m_pEffect->SetVector( m_hvLightDir, &v );
		
		// ����(������ǥ��)
		m = matWorld * matView;
		D3DXMatrixInverse( &m, NULL, &m);
		v = D3DXVECTOR4( 0, 0, 0, 1);
		D3DXVec4Transform( &v, &v, &m );
		m_pEffect->SetVector( m_hvEyePos, &v );

		// ������
		m_pEffect->SetTexture( m_htNormalMap, m_pNormalMap );
		// ��������
		g_pD3dDev->SetVertexDeclaration( m_pDecl );

		v.x = 0.5f;
		v.y = 0.5f;
		v.z = 0.5f;

		m_pEffect->SetVector( m_hvColor, &v );
		
		m_pEffect->SetTexture( m_htDecaleTex, m_pTexture );
		
			g_pD3dDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0,
				0, 
				((64 + 1) * (64 + 1)), 
				0, 
				m_nTriangleNumber );

		m_pEffect->End();
	}
    return S_OK;
}


HRESULT CBumpMapping::InvalidateDeviceObjects()
{
	// ���̴�
    if( m_pEffect != NULL ) m_pEffect->OnLostDevice();

    return S_OK;
}




//-------------------------------------------------------------
// Name: DeleteDeviceObjects()
// Desc: InitDeviceObjects() ���� ������ ������Ʈ ����
//-------------------------------------------------------------
HRESULT CBumpMapping::DeleteDeviceObjects()
{
	SAFE_RELEASE( m_pEffect );      // ���̴�
	SAFE_RELEASE( m_pDecl );		// ��������
	SAFE_RELEASE( m_pNormalMap );
	
    return S_OK;
}