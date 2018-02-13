// CharacterRender.cpp: implementation of the CCharacterRender class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CharacterRender.h"
#include "AtumApplication.h"
#include "SkinnedMesh.h"
#include "UnitData.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "EnemyData.h"
#include "SceneData.h"
#include "dxutil.h"
#include "ShuttleChild.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCharacterRender::CCharacterRender()
{

}

CCharacterRender::~CCharacterRender()
{
	map<int,CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.begin();
	while( it != m_mapSkinnedMesh.end()) 
	{
		util::del(it->second);
		it++;
	}
	m_mapSkinnedMesh.clear();
}

HRESULT CCharacterRender::InitDeviceObjects()
{
	return S_OK;
}

HRESULT CCharacterRender::RestoreDeviceObjects()
{
	map<int,CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.begin();
	while(it != m_mapSkinnedMesh.end())
	{
		it->second->RestoreDeviceObjects();
		it++;
	}

	return S_OK;
}

HRESULT CCharacterRender::InvalidateDeviceObjects()
{
	map<int,CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.begin();
	while(it != m_mapSkinnedMesh.end())
	{
		it->second->InvalidateDeviceObjects();
		it++;
	}

	return S_OK;
}

HRESULT CCharacterRender::DeleteDeviceObjects()
{
	map<int,CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.begin();
	while( it != m_mapSkinnedMesh.end()) 
	{
		// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����	
		it->second->DeleteLoadingGameData();
		//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
//		it->second->InvalidateDeviceObjects();
		it->second->DeleteDeviceObjects();
		util::del(it->second);
		it++;
	}
	m_mapSkinnedMesh.clear();
	return S_OK;
}

void CCharacterRender::Render()
{
	// 2007-10-08 by dgwoo �Ƹ� �ε�.
	{
		int nAmorUnitNum = g_pShuttleChild->GetUnitNum();
		map<int, CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.find(nAmorUnitNum);
		if( it == m_mapSkinnedMesh.end() )
		{
			m_vecLoadMeshIndex.push_back( nAmorUnitNum );
			return;
		}
		g_pD3dApp->SetUnitArmorColorMax(it->second->m_bTotalTextureNum - 1);		
	}
	// 2007-10-08 by dgwoo �Ƹ� �ε�.

	// 2005-07-13 by ispark
	int nUnitNum = g_pCharacterChild->m_nUnitNum;
//	int nUnitNum = 20000000;			// ������ ����

	map<int, CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.find(nUnitNum);
	if( it == m_mapSkinnedMesh.end() )
	{
		m_vecLoadMeshIndex.push_back( nUnitNum );
		return;
	}
	CSkinnedMesh* pSkinnedMesh = it->second;	

	pSkinnedMesh->Tick(g_pCharacterChild->m_fCurrentTime);
	pSkinnedMesh->SetWorldMatrix(g_pCharacterChild->m_mMatrix);
	pSkinnedMesh->AnotherTexture(1);
	pSkinnedMesh->Render();
}

void CCharacterRender::Render(CUnitData * pNode)
{
	if(pNode->m_dwPartType == _ENEMY || pNode->m_dwPartType == _ADMIN)
	{
		CEnemyData* pEnemy = (CEnemyData*)pNode;
		int nUnitNum = pNode->GetPilotNum();

		map<int, CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.find(nUnitNum);
		if( it == m_mapSkinnedMesh.end() )
		{
			m_vecLoadMeshIndex.push_back( nUnitNum );
			return;
		}
		CSkinnedMesh* pSkinnedMesh = it->second;

		pSkinnedMesh->Tick(pEnemy->m_fCurrentTime);
		pSkinnedMesh->SetWorldMatrix(pEnemy->m_mMatrix);
		pSkinnedMesh->AnotherTexture(1);
		pSkinnedMesh->Render();
	}
}

void CCharacterRender::Render(GUIUnitRenderInfo& renderinfo)
{
	map<int,CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.find(renderinfo.rendertype);
	if( it == m_mapSkinnedMesh.end() )
	{
		m_vecLoadMeshIndex.push_back( renderinfo.rendertype );
		return;
	}
	CSkinnedMesh* pSkinnedMesh = it->second;
	pSkinnedMesh->Tick(0.0f);
	pSkinnedMesh->SetWorldMatrix(renderinfo.matrix);
	pSkinnedMesh->AnotherTexture(1);
	pSkinnedMesh->Render();
}

void CCharacterRender::Tick(float fElapsedTime)
{
	vector<int>::iterator it = m_vecLoadMeshIndex.begin();
	while(it != m_vecLoadMeshIndex.end())
	{
		// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
		//InitData( *it);
		InitData( *it , _CHARACTER_TYPE);
		// end 2008-10-15 by bhsohn ���ҽ� �޸� ��ȣ ��� �߰�
		//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
		it++;
	}
	m_vecLoadMeshIndex.clear();
}

void CCharacterRender::RenderShadow()
{
	int nUnitNum = g_pCharacterChild->m_nUnitNum;

	map<int,CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.find(nUnitNum);
	if(it != m_mapSkinnedMesh.end())
	{
		//Shadow
		if(g_pCharacterChild->m_pVBShadow)
		{
			D3DXMATRIX	matTemp;
			D3DXMatrixIdentity(&matTemp);
			// 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����
			//g_pD3dDev->SetTexture( 0, it->second->m_pTexture[it->second->m_bTotalTextureNum - 1] );

			// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
// 			LPDIRECT3DTEXTURE9	pShadowTex = it->second->GetShadowTex();
// 			g_pD3dDev->SetTexture( 0, pShadowTex );
// 			
// 			g_pD3dDev->SetTransform( D3DTS_WORLD, &matTemp );
// 			g_pD3dDev->SetFVF( D3DFVF_SPRITEVERTEX );
// 			g_pD3dDev->SetStreamSource( 0, g_pCharacterChild->m_pVBShadow,0, sizeof(SPRITEVERTEX) );
// 			g_pD3dDev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
			if(it->second->GetIsLoadingFlag() != TRUE)
			{
				//�ε� �� �϶��� �׸��ڸ� �׸��� �ʴ´�
				LPDIRECT3DTEXTURE9	pShadowTex = it->second->GetShadowTex();
				g_pD3dDev->SetTexture( 0, pShadowTex );

				g_pD3dDev->SetTransform( D3DTS_WORLD, &matTemp );
				g_pD3dDev->SetFVF( D3DFVF_SPRITEVERTEX );
				g_pD3dDev->SetStreamSource( 0, g_pCharacterChild->m_pVBShadow,0, sizeof(SPRITEVERTEX) );
				g_pD3dDev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
			}
			//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
		}
	}
}

void CCharacterRender::RenderShadow(CAtumNode * pNode)
{
	CUnitData * pData = (CUnitData *)pNode;
	int nUnitNum = pData->GetPilotNum();
	map<int,CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.find(nUnitNum);
	if(it != m_mapSkinnedMesh.end())
	{
		//Shadow
		if(pData->m_pVBShadow)
		{
			D3DXMATRIX	matTemp;
			D3DXMatrixIdentity(&matTemp);
			// 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����
			//g_pD3dDev->SetTexture( 0, it->second->m_pTexture[it->second->m_bTotalTextureNum - 1] );

			// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
// 			LPDIRECT3DTEXTURE9	pShadowTex = it->second->GetShadowTex();
// 			g_pD3dDev->SetTexture( 0, pShadowTex );
// 
// 			g_pD3dDev->SetTransform( D3DTS_WORLD, &matTemp );
// 			g_pD3dDev->SetFVF( D3DFVF_SPRITEVERTEX );
// 			g_pD3dDev->SetStreamSource( 0, pData->m_pVBShadow,0, sizeof(SPRITEVERTEX) );
// 			g_pD3dDev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

			if( it->second->GetIsLoadingFlag() != TRUE )
			{
				LPDIRECT3DTEXTURE9	pShadowTex = it->second->GetShadowTex();
				g_pD3dDev->SetTexture( 0, pShadowTex );
				
				g_pD3dDev->SetTransform( D3DTS_WORLD, &matTemp );
				g_pD3dDev->SetFVF( D3DFVF_SPRITEVERTEX );
				g_pD3dDev->SetStreamSource( 0, pData->m_pVBShadow,0, sizeof(SPRITEVERTEX) );
				g_pD3dDev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
			}
			// end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
		}
	}
}