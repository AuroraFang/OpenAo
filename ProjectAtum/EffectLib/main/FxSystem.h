#pragma once

// FxSystem.h: interface for the CFxSystem class.
//////////////////////////////////////////////////////////////////////

#include "d3d9.h"
#include "SkinnedMesh.h"

// 2011-10-10 by jhahn EP4 Ʈ���� �ý���	ȭ���� ����Ʈ �߰�
#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <basetsd.h>
#include <math.h>
#include <stdio.h>
#include <d3dx9.h>
//#include <dxerr9.h>
#include <tchar.h>

//-------------------------------------------------------------
// ��������ü
//-------------------------------------------------------------
// �ؽ�ó�� 1�常 ���δ�
typedef struct {
	FLOAT       p[4];
	FLOAT       tu, tv;
} TVERTEX;
// �ؽ�ó�� 3�� ���δ�
typedef struct {
	FLOAT       p[4];
	FLOAT       t[3][2];
} T3VERTEX;
// �ؽ�ó�� 4�� ���δ�
typedef struct {
	FLOAT       p[3];
	FLOAT       t[4][2];
} T4VERTEX;

#define SMALL_WIDTH		((g_pD3dApp->GetBackBufferDesc().Width )/8)
#define SMALL_HEIGHT	((g_pD3dApp->GetBackBufferDesc().Height)/8)
//end 2011-10-10 by jhahn EP4 Ʈ���� �ý���	ȭ���� ����Ʈ �߰�
class CFxSystem  
{
public:
	CFxSystem();
	virtual ~CFxSystem();
	void	InitDeviceObjects();
	void	RestoreDeviceObjects();
	void	InvalidateDeviceObjects();
	void	DeleteDeviceObjects();
	void	Tick(float fElapsedTime);
	void	BeginTargetSurface();
	void	EndTargetSurface();
	LPDIRECT3DTEXTURE9	GetTargetSurface() { return m_pTargetTexture[0]; }
	void	SetSourceAlpha(BYTE byAlpha) { m_byAlpha = byAlpha; }
	BYTE	GetSourceAlpha() { return m_byAlpha; }
	BOOL	IsRenderBlurFilter() { return m_bSupportBlurFilter; }
	BOOL	GetMetarSurface(){return m_bSupportMetal;}
	BOOL	GetEnvSurface(){return m_bSupportEvn;}
	
	void	MakeTargetSurface();
	void	DrawTargetSurface();
	void	MakeGaussFilterSurface();
	void	DrawGaussFilterSurface();
// 2011-10-10 by jhahn EP4 Ʈ���� �ý���	ȭ���� ����Ʈ �߰�
	void	MakeSandStromSurface();
	void	SetingSandStromSurface( BOOL Setstorm){m_bStormShader = Setstorm;}
	BOOL	GetingSandStromSurface(){return m_bStormShader;}
	void	SetingMgamaSurface( BOOL Setmagma){m_bMagmaShader = Setmagma;}
	BOOL	GetingMgamaSurface(){return m_bMagmaShader;}
//end02 2011-10-10 by jhahn EP4 Ʈ���� �ý���	ȭ���� ����Ʈ �߰�

	

	// ��Ż
	void	DrawMetalFilterBegin(D3DXMATRIX mWorld, D3DXMATRIX mView, D3DXMATRIX mProj, UINT ipattr, DWORD nType=0);
	void	SetMeshFrame(SMeshContainer *pMesh){m_pframeCur = pMesh;}
	
	// ȯ�汤
	void	DrawEvnFilterBegin(D3DXMATRIX mWorld, D3DXMATRIX mView, D3DXMATRIX mProj, UINT ipattr, DWORD nType=0);

	BOOL	IsRenderGaussBlur()	{ return m_bSupportGaussBlur; }
// 2011-10-10 by jhahn EP4 Ʈ���� �ý���	ȭ���� ����Ʈ �߰�
	BOOL	IsRenderSandStromBlur(){ return TRUE; }
//end 2011-10-10 by jhahn EP4 Ʈ���� �ý���	ȭ���� ����Ʈ �߰�
	HRESULT CreatePSFromCompiledFile (LPDIRECT3DDEVICE9 pd3dDevice,TCHAR* strPSPath,LPDIRECT3DPIXELSHADER9& pPS);
	HRESULT CreateEffectFromCompiledFile(LPDIRECT3DDEVICE9 pd3dDevice,TCHAR* strFilePath,LPD3DXEFFECT& pEffect);

public:
	float				m_fEffectTime;

	LPDIRECT3DTEXTURE9	m_pTargetTexture[2];
	LPDIRECT3DSURFACE9	m_pTargetSurface[2];

	BYTE				m_byAlpha;
	BOOL				m_bSupportBlurFilter;
	SMeshContainer		*m_pframeCur;		// �޽� ������

	// ���콺 �� ����
	LPDIRECT3DTEXTURE9	m_pGaussFilterTexture[2];
	LPDIRECT3DSURFACE9	m_pGaussFilterSurface[2];

	// ���̴�
	LPD3DXEFFECT		    m_pEffect;		// ����Ʈ
	D3DXHANDLE				m_hTechnique;	// ��ũ��
	D3DXHANDLE				m_hafWeight;	// ����ġ �迭
	D3DXHANDLE				m_htSrcMap;		// �ؽ�ó		
	DWORD					m_dwGaussBlurNum;
	BOOL					m_bUseGaussBlur;				// ���콺�� �����
	BOOL					m_bSupportGaussBlur;			// �������̴� 1.1 + �ȼ����̴� 1.1 �̻� ����
	DWORD					m_dwShaderVersion;				// 1(1.1) 2(2.0)
// 2011-10-10 by jhahn EP4 Ʈ���� �ý���	ȭ���� ����Ʈ �߰�
	BOOL					m_bSupportFocusBlur;			// �������̴� 1.1 + �ȼ����̴� 1.1 �̻� ����


	//----------------------------------------------------------------------------------------------------------------------------------
	LPD3DXEFFECT		    m_pEffect2;		// ����Ʈ
	D3DXHANDLE				m_hTechnique2;	// ��ũ��
	D3DXHANDLE				m_hafWeight2;	// ����ġ�迭
	D3DXHANDLE				m_htSrcTex2;		// �ؽ�ó
	D3DXHANDLE				m_htBlurTex2;	// �ؽ�ó
	D3DXHANDLE				m_htBlendTex2;	// �ؽ�ó
	D3DXHANDLE				m_htDummyTex2;

	
	LPDIRECT3DTEXTURE9		m_pTex;
	LPDIRECT3DTEXTURE9		m_pDummyTex;
	

	// ������ Ÿ��
	LPDIRECT3DSURFACE9		m_pMapZ2;		// ���̹���
	LPDIRECT3DTEXTURE9		m_pOriginalTex2;	// �ؽ�ó
	LPDIRECT3DSURFACE9		m_pOriginalSurf2;// ǥ��
	LPDIRECT3DTEXTURE9		m_pPostTex2[2];	// �ؽ�ó
	LPDIRECT3DSURFACE9		m_pPostSurf2[2];	// ǥ��

	BOOL					m_bStormShader;
	BOOL					m_bMagmaShader;
	//----------------------------------------------------------------------------------------------------------------------------------
//end 2011-10-10 by jhahn EP4 Ʈ���� �ý���	ȭ���� ����Ʈ �߰�
	// ���̴� (�ݼӱ�)
	LPD3DXEFFECT		    m_pEffectMetal;		// ����Ʈ
	D3DXHANDLE				m_hTechniqueMetal;	// ��ũ��
	D3DXHANDLE				m_hmWVP;			// ����-������ȯ���
	D3DXHANDLE				m_hmW;				// ����-���庯ȯ���
	D3DXHANDLE				m_hvLightDir;		// ��������
	D3DXHANDLE				m_hvColor;			// ������
	D3DXHANDLE				m_hvEyePos;			// ������ ��ġ	
	BOOL					m_bSupportMetal;	// ��� ����

	// ���̴� (ȯ��)
	LPD3DXEFFECT			m_pEffectEvn;
    LPDIRECT3DTEXTURE9		m_pSphereMap;
	BOOL					m_bSupportEvn;	
};
