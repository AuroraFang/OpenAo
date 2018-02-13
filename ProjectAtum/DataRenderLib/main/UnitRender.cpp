// UnitRender.cpp: implementation of the CUnitRender class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UnitRender.h"
#include "AtumApplication.h"
#include "SkinnedMesh.h"
#include "UnitData.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "EnemyData.h"
#include "SceneData.h"
#include "StoreData.h"
#include "ItemInfo.h"
#include "dxutil.h"

// 2007-08-29 by bhsohn ���� ���� ���� ����
#define		UNI_STOP_TICK_TIME		40.0f

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUnitRender::CUnitRender()
{
//	m_dwNormalStateBlock = 0;
//	m_dwAlphaBlendStateBlock = 0;
	m_nSelectStateUnitColor = 0;
}

CUnitRender::~CUnitRender()
{
	map<int,CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.begin();
	while( it != m_mapSkinnedMesh.end()) 
	{
		util::del(it->second);
		it++;
	}
	m_mapSkinnedMesh.clear();
}


HRESULT CUnitRender::InitDeviceObjects()
{
	return S_OK;
}

HRESULT CUnitRender::RestoreDeviceObjects()
{
	// 2005-07-27 by ispark
	map<int,CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.begin();
	while(it != m_mapSkinnedMesh.end())
	{
		it->second->RestoreDeviceObjects();
		it++;
	}
//	if(g_pShuttleChild)
//	{
//		g_pShuttleChild->RestoreShadow();
//	}
//	CMapEnemyIterator it = g_pScene->m_mapEnemyList.begin();
//	while( it != g_pScene->m_mapEnemyList.end() )
//	{
//		it->second->RestoreShadow();
//		it++;
//	}
	return S_OK;
}

HRESULT CUnitRender::InvalidateDeviceObjects()
{
	// 2005-07-27 by ispark
	map<int,CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.begin();
	while(it != m_mapSkinnedMesh.end())
	{
		it->second->InvalidateDeviceObjects();
		it++;
	}
	// 2005-01-03 by jschoi - �ּ�
//	if(m_dwNormalStateBlock)
//	{
//		g_pD3dDev->DeleteStateBlock(m_dwNormalStateBlock);
//		m_dwNormalStateBlock = 0;
//	}
//	if(m_dwAlphaBlendStateBlock)
//	{
//		g_pD3dDev->DeleteStateBlock(m_dwAlphaBlendStateBlock);
//		m_dwAlphaBlendStateBlock = 0;
//	}
//	if(g_pShuttleChild)
//	{
//		g_pShuttleChild->InvalidateShadow();
//	}
//	CMapEnemyIterator it = g_pScene->m_mapEnemyList.begin();
//	while( it != g_pScene->m_mapEnemyList.end() )
//	{
//		it->second->InvalidateShadow();
//		it++;
//	}
	return S_OK;
}

HRESULT CUnitRender::DeleteDeviceObjects()
{
	map<int,CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.begin();
	while( it != m_mapSkinnedMesh.end()) 
	{
		it->second->InvalidateDeviceObjects();
		it->second->DeleteDeviceObjects();
		util::del(it->second);
		it++;
	}
	m_mapSkinnedMesh.clear();
	return S_OK;
}
CSkinnedMesh* CUnitRender::GetUnitMesh(int nUnitNum)
{
	map<int, CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.find(nUnitNum);
	if( it == m_mapSkinnedMesh.end() )
	{
		m_vecLoadMeshIndex.push_back( nUnitNum );
		return NULL;
	}
	return it->second;	
}

// 2006-02-17 by ispark, ���� �Է�
void CUnitRender::Render(CUnitData * pNode, int nAmorColor)
{
	if(pNode->m_dwPartType == _SHUTTLE)
	{
		int nUnitNum = g_pShuttleChild->GetUnitNum();
		map<int, CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.find(nUnitNum);
		if( it == m_mapSkinnedMesh.end() )
		{
			m_vecLoadMeshIndex.push_back( nUnitNum );
			return;
		}
		CSkinnedMesh* pSkinnedMesh = it->second;
		g_pD3dApp->SetUnitArmorColorMax(pSkinnedMesh->m_bTotalTextureNum - 1);
		
		g_pD3dDev->SetLight( 0, &g_pScene->m_light0 );
		g_pD3dDev->LightEnable( 0, TRUE ); /// 0�� ������ �Ҵ� 
		g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE ); /// ���������� �Ҵ�
		// 2010. 03. 18 by jskim ���ͺ��� ī��
		//g_pD3dDev->SetRenderState( D3DRS_SPECULARENABLE, TRUE );
		if(pNode->GetMonsterTransformer())
		{
			g_pD3dDev->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
		}
		else
		{
			g_pD3dDev->SetRenderState( D3DRS_SPECULARENABLE, TRUE );
		}		
		//end 2010. 03. 18 by jskim ���ͺ��� ī��
		g_pD3dDev->LightEnable( 2, FALSE );
		g_pD3dDev->LightEnable( 3, FALSE );
		g_pD3dDev->SetRenderState( D3DRS_FOGENABLE,  IsFogEnableMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) );
		g_pD3dDev->SetRenderState( D3DRS_ZENABLE,  TRUE );
		g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
		g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );		
		g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );		
		g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
		
		//*--------------------------------------------------------------------------*//
		// 2006-11-09 by ispark, ���� ������
		if(g_pShuttleChild->GetUnitAlphaState())
		{
			g_pD3dApp->SetAlphaRenderState(g_pShuttleChild->m_nAlphaValue);
		}
		//*--------------------------------------------------------------------------*//
		
		// �Ƹ��� Į�� ���� ����
		// 2006-02-17 by ispark
//		CItemInfo* pTempItem = g_pStoreData->FindItemInInventoryByWindowPos( POS_CENTER );
//		if(pTempItem)
//		{			
//			g_pD3dApp->SetUnitArmorColorNum(max(1, pTempItem->ColorCode%100));
//		}
		pSkinnedMesh->m_unTexSelectColor = nAmorColor;

		// ��Į ���� ������
		g_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE); 
		g_pD3dDev->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000001);
		g_pD3dDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	

		pSkinnedMesh->Tick(g_pShuttleChild->m_fCurrentTime);

		// 2010. 03. 18 by jskim ���ͺ��� ī��
		D3DXMATRIX tempScale;
		D3DXMatrixIdentity(&tempScale);
		if(pNode->m_MonsterTransformer && pNode->m_MonsterTransScale > 0)
		{
			D3DXMatrixScaling(&tempScale, pNode->m_MonsterTransScale, pNode->m_MonsterTransScale, pNode->m_MonsterTransScale);
			tempScale = tempScale * g_pShuttleChild->m_mMatrix;
		}
		else
		{
			tempScale = g_pShuttleChild->m_mMatrix;
		}
		//pSkinnedMesh->SetWorldMatrix(g_pShuttleChild->m_mMatrix);
		pSkinnedMesh->SetWorldMatrix(tempScale);
		//end 2010. 03. 18 by jskim ���ͺ��� ī��		
		pSkinnedMesh->AnotherTexture(1);


		// 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)
		//pSkinnedMesh->Render(FALSE, pNode->m_dwPartType);
		
		// �κ� �Ƹ� �ؽ��� 2�� ���
		// 2010. 03. 18 by jskim ���ͺ��� ī��
		//if( !pNode->IsRobotArmor())
		if (!pNode->IsRobotArmor() && !pNode->GetMonsterTransformer())
			//end 2010. 03. 18 by jskim ���ͺ��� ī��
			 pSkinnedMesh->Render(FALSE, _SHUTTLE);
		else
			pSkinnedMesh->Render(FALSE, _MONSTER);
		// end 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)


		pSkinnedMesh->SetDecalCount(0);

		g_pD3dDev->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
	}
	else if(pNode->m_dwPartType == _ENEMY)
	{
		CEnemyData* pEnemy = (CEnemyData*)pNode;
		int nUnitNum = pEnemy->GetUnitNum();

		map<int, CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.find(nUnitNum);
		if( it == m_mapSkinnedMesh.end() )
		{
			m_vecLoadMeshIndex.push_back( nUnitNum );
			return;
		}		
		CSkinnedMesh* pSkinnedMesh = it->second;		
		g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

		// ��Į ���� ������
		g_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE); 
		g_pD3dDev->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000001);
		g_pD3dDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

		//*--------------------------------------------------------------------------*//
		// 2006-11-09 by ispark, ���� ������
		if(pEnemy->m_infoCharacter.CharacterRenderInfo.RI_Invisible)
		{
			g_pD3dApp->SetAlphaRenderState(pEnemy->m_nAlphaValue);
		}
		//*--------------------------------------------------------------------------*//

//		if(pEnemy->m_infoCharacter.CharacterRenderInfo.RI_ArmorColorCode == 0)
//			g_pD3dApp->SetUnitArmorColorNum(0);
//		else
//			g_pD3dApp->SetUnitArmorColorNum(max(1, pEnemy->m_infoCharacter.CharacterRenderInfo.RI_ArmorColorCode%100));
		pSkinnedMesh->m_unTexSelectColor = nAmorColor;
		
		pSkinnedMesh->Tick(pEnemy->m_fCurrentTime);


		// 2010. 03. 18 by jskim ���ͺ��� ī��
		D3DXMATRIX tempScale;
		D3DXMatrixIdentity(&tempScale);
		if(pNode->m_MonsterTransformer && pNode->m_MonsterTransScale > 0)
		{
			D3DXMatrixScaling(&tempScale, pNode->m_MonsterTransScale, pNode->m_MonsterTransScale, pNode->m_MonsterTransScale);
			tempScale = tempScale * pEnemy->m_mMatrix;
		}
		else
		{
			tempScale = pEnemy->m_mMatrix;
		}
		//pSkinnedMesh->SetWorldMatrix(pEnemy->m_mMatrix);
		pSkinnedMesh->SetWorldMatrix(tempScale);
		//end 2010. 03. 18 by jskim ���ͺ��� ī��
		pSkinnedMesh->AnotherTexture(1);


		// 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)
		//pSkinnedMesh->Render(FALSE, _ENEMY);

		// �κ� �Ƹ� �ؽ��� 2�� ���
		// 2010. 03. 18 by jskim ���ͺ��� ī��
		//if( !pNode->IsRobotArmor() )
		if( !pNode->IsRobotArmor() && !pNode->GetMonsterTransformer()){
		//end 2010. 03. 18 by jskim ���ͺ��� ī��
			pSkinnedMesh->Render(FALSE, _ENEMY);
		}
		else
			pSkinnedMesh->Render(FALSE, _MONSTER);
		// end 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)

		pSkinnedMesh->SetDecalCount(0);
	}
	else if(pNode->m_dwPartType == _ADMIN)
	{
		CEnemyData* pEnemy = (CEnemyData*)pNode;
		int nUnitNum = pEnemy->GetUnitNum();
		map<int, CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.find(nUnitNum);
		if( it == m_mapSkinnedMesh.end() )
		{
			m_vecLoadMeshIndex.push_back( nUnitNum );
			return;
		}
		CSkinnedMesh* pSkinnedMesh = it->second;		
		g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

		//*--------------------------------------------------------------------------*//
		// 2006-11-09 by ispark, ���� ������
		if(pEnemy->m_infoCharacter.CharacterRenderInfo.RI_Invisible)
		{
			g_pD3dApp->SetAlphaRenderState(pEnemy->m_nAlphaValue);
		}
		//*--------------------------------------------------------------------------*//

		pSkinnedMesh->Tick(pEnemy->m_fCurrentTime);

		// 2010. 03. 18 by jskim ���ͺ��� ī��
		D3DXMATRIX tempScale;
		D3DXMatrixIdentity(&tempScale);
		if(pNode->m_MonsterTransformer && pNode->m_MonsterTransScale > 0)
		{

			D3DXMatrixScaling(&tempScale, pNode->m_MonsterTransScale, pNode->m_MonsterTransScale, pNode->m_MonsterTransScale);
			tempScale = tempScale * pEnemy->m_mMatrix;
		}
		else
		{
			tempScale = pEnemy->m_mMatrix;
		}
		//pSkinnedMesh->SetWorldMatrix(pEnemy->m_mMatrix);
		pSkinnedMesh->SetWorldMatrix(tempScale);
		//end 2010. 03. 18 by jskim ���ͺ��� ī��
		pSkinnedMesh->AnotherTexture(1);
		pSkinnedMesh->Render();
	}
}

// 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)
//void CUnitRender::Render(GUIUnitRenderInfo& renderinfo)
// 2010. 06. 09 by jskim ��� ����â �ִϸ��̼� ���� ����
//void CUnitRender::Render( GUIUnitRenderInfo& renderinfo, const BOOL bUseSubMtrl /* = FALSE */ )
void CUnitRender::Render( GUIUnitRenderInfo& renderinfo, float fTick, const BOOL bUseSubMtrl /* = FALSE */ )
// 2010. 06. 09 by jskim ��� ����â �ִϸ��̼� ���� ����
// end 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)
{
	map<int,CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.find(renderinfo.rendertype);
	if( it == m_mapSkinnedMesh.end() )
	{
		m_vecLoadMeshIndex.push_back( renderinfo.rendertype );
		return;
	}
	CSkinnedMesh* pSkinnedMesh = it->second;
	// ��Į ���� ������
	g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );	
	g_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE); 
	g_pD3dDev->SetRenderState(D3DRS_ALPHAREF, (DWORD)0x00000001);
	g_pD3dDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	
	// 2006-02-17 by ispark
//	g_pD3dApp->SetUnitArmorColorNum(m_nSelectStateUnitColor);
//	g_pD3dApp->SetUnitArmorColorNum(max(1, g_pSelect->m_guiUnitInfo[i].CharacterRenderInfo.RI_ArmorColorCode%100));
	
	pSkinnedMesh->m_unTexSelectColor = m_nSelectStateUnitColor;

	// 2007-08-29 by bhsohn ���� ���� ���� ����
	//pSkinnedMesh->Tick(0.0f);
	// 40 ms�� ��ü�� �ٸ��� ������  �ִ� �ð��� �̴�.
	// 2010. 06. 09 by jskim ��� ����â �ִϸ��̼� ���� ����
	pSkinnedMesh->Tick(fTick);
	//end 2010. 06. 09 by jskim ��� ����â �ִϸ��̼� ���� ����

	pSkinnedMesh->SetWorldMatrix(renderinfo.matrix);
	pSkinnedMesh->AnotherTexture(1);

	// 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)
	//pSkinnedMesh->Render(FALSE, _SHUTTLE);

	// �κ� �Ƹ� �ؽ��� 2�� ���
	if( !bUseSubMtrl)
		pSkinnedMesh->Render( FALSE, _SHUTTLE );
	else
		pSkinnedMesh->Render( FALSE, _MONSTER );
	// end 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)

	pSkinnedMesh->SetDecalCount(0);
}

void CUnitRender::Tick(float fElapsedTime)
{
	vector<int>::iterator it = m_vecLoadMeshIndex.begin();
	while(it != m_vecLoadMeshIndex.end())
	{
		// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
		//InitData( *it );
		InitData( *it , _UNIT_TYPE );	
		//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
		it++;
	}
	m_vecLoadMeshIndex.clear();
}

void CUnitRender::RenderShadow(CAtumNode * pNode)
{
	CUnitData * pData = (CUnitData *)pNode;
	int nUnitNum = pData->GetUnitNum();
	map<int,CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.find(nUnitNum);
	if( it != m_mapSkinnedMesh.end() &&
		pData->m_bShadowIsRender &&  
		pData->m_dwState != _EXPLODED && 
		pData->m_dwState != _EXPLODING )
	{
		//Shadow
		if(pData->m_pVBShadow)
		{
			D3DXMATRIX	matTemp;
			D3DXMatrixIdentity(&matTemp);
			// 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����
			//g_pD3dDev->SetTexture( 0, it->second->m_pTexture[it->second->m_bTotalTextureNum - 1] );

			// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
			//LPDIRECT3DTEXTURE9	pShadowTex = it->second->GetShadowTex();
			//g_pD3dDev->SetTexture( 0, pShadowTex );			
			// end 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����
			//g_pD3dDev->SetTransform( D3DTS_WORLD, &matTemp );
			//g_pD3dDev->SetFVF( D3DFVF_SPRITEVERTEX );
			//g_pD3dDev->SetStreamSource( 0, pData->m_pVBShadow,0, sizeof(SPRITEVERTEX) );
			//g_pD3dDev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
			if(it->second->GetIsLoadingFlag() != TRUE)
			{
			//�ε� �� �϶��� �׸��ڸ� �׸��� �ʴ´�
			LPDIRECT3DTEXTURE9	pShadowTex = it->second->GetShadowTex();
			g_pD3dDev->SetTexture( 0, pShadowTex );			
			// end 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����
			
			g_pD3dDev->SetTransform( D3DTS_WORLD, &matTemp );
			g_pD3dDev->SetFVF( D3DFVF_SPRITEVERTEX );
			g_pD3dDev->SetStreamSource( 0, pData->m_pVBShadow,0, sizeof(SPRITEVERTEX) );
			g_pD3dDev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
			}
			//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
		}
	}
}

void CUnitRender::RenderDecalObj(CAtumNode * pNode){
//	CUnitData * pData = (CUnitData *)pNode;
//	int nUnitNum = pData->GetUnitNum();
//
//	// ��Į ���� ����
//	if(pData->m_nDecalObjNum == 0)
//		return;
//
//	//////////////////////////////////////////////////////////////////////////	
//	// CUnitData�� ��Į���� ���� ���� �ӽ÷� ����
//	// ��Į ���� �ʵ�
//	//
//	// 1. ��Į�� ������ FALSE
//	// 2. ������ ���س��� ��Į
//	// 3. ����� ���� ��Į
//	//////////////////////////////////////////////////////////////////////////
//
//	map<int, CSkinnedMesh*>::iterator it = m_mapSkinnedMesh.find(nUnitNum);
//	if( it == m_mapSkinnedMesh.end()/* &&pData->m_nDecalObjNum */)
//	{
//		m_vecLoadMeshIndex.push_back( nUnitNum );
//		return;
//	}
//	CSkinnedMesh* pSkinnedMesh = it->second;
//	
//	if()
//	g_pD3dDev->SetRenderState( D3DRS_SPECULARENABLE, TRUE );
//	g_pD3dDev->LightEnable( 2, FALSE );
//	g_pD3dDev->LightEnable( 3, FALSE );
//	g_pD3dDev->SetRenderState( D3DRS_FOGENABLE,  IsFogEnableMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) );
//	g_pD3dDev->SetRenderState( D3DRS_ZENABLE,  TRUE );
//	g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );		
//	g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
//	g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );		
//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );		
//	g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );	
//	
//	pSkinnedMesh->Tick(g_pShuttleChild->m_fCurrentTime);
//	pSkinnedMesh->SetWorldMatrix(g_pShuttleChild->m_mMatrix); //��ġ ���� �κ�
//	pSkinnedMesh->AnotherTexture(1);
//	pSkinnedMesh->Render(FALSE, pNode->m_dwPartType);
}
