// QuadGround.cpp: implementation of the CQuadGround class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Frustum.h"
#include "QuadGround.h"
#include "DXUtil.h"
#include "AtumApplication.h"
#include "SceneData.h"
#include "Background.h"
#include "Camera.h"
#include "GameDataLast.h"
#include "Water.h"
#include "ShuttleChild.h"


#define QUAD_BLOCK_SIZE		64
#define QUAD_BASE_DEGREE	0
#define QUAD_START_DEGREE	2
#define QUAD_LAST_DEGREE	8
#define QUAD_LAST_SIZE		1
#define QUAD_APPLY_DEGREE	5


#define GROUND_RENDER_NOT		0
#define GROUND_RENDER_NORMAL	1
#define GROUND_RENDER_TILE		2
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CQuadGround::CQuadGround(int nMax)
{	
//	FLOG("CQuadGround::CQuadGround(int nMax)");
	m_nMaxSize = nMax;
	m_pTexture = NULL;					
	m_pVBTest = NULL;
//	m_pVBTest1 = NULL;
	m_pIBTest = NULL;
	m_nSize = 0;
	m_nTriangleNumber = 0;
	m_pOriParent= this;
	m_pIdx = NULL;

	m_nMaxHeight = 0;
}

CQuadGround::CQuadGround(CQuadTree * pParent, 
						 CornerType nType, 
						 short sX, 
						 short sY, 
						 int nMax)
{
//	FLOG("CQuadGround::CQuadGround(CQuadTree * pParent,CornerType nType,short sX, short sY,int nMax)");
	for ( int nCheckNode = 0;nCheckNode < 4;nCheckNode++ )
	{
		m_pChild[nCheckNode] = NULL;				
		m_vPos[nCheckNode] = D3DXVECTOR3( 0,0,0 );
	}
	m_pParent = pParent;
	m_eCornerType = nType;
	m_nMaxSize = nMax;
	if ( m_pParent )
	{
		m_nDegree = m_pParent->m_nDegree + 1;
	}
	else
	{
		m_nDegree = QUAD_BASE_DEGREE;
	}
	m_sStartX = sX;	
	m_sStartY = sY;	
	m_pTexture = NULL;
	m_pVBTest = NULL;
//	m_pVBTest1 = NULL;
	m_pIBTest = NULL;
	m_nSize = 0;
	if ( m_nDegree <= QUAD_START_DEGREE )
	{
		m_pOriParent = this;
	}
	else
	{
		m_pOriParent = m_pParent->m_pOriParent;
	}
	m_nTriangleNumber = 0;

	m_nMaxHeight = 0;

}

CQuadGround::~CQuadGround()
{

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			InitDeviceObjects
/// \brief		Init Device
/// \date		2004-03-13 ~ 2004-03-13
/// \warning	1�ܰ� ������ �ؽ��ĸ� �ε��Ѵ�
///
/// \param		
/// \return		HRESULT
///////////////////////////////////////////////////////////////////////////////
char* strmap[16] = {"map15","map11","map14","map10","map07","map03","map06","map02","map13","map09",
					"map12","map08","map05","map01","map04","map00" };

int g_count = 0;
int g_loop = 0;
HRESULT CQuadGround::InitDeviceObjects()
{
//	FLOG("CQuadGround::InitDeviceObjects()");
	if ( m_nDegree == QUAD_START_DEGREE )
	{
		if( g_loop == 4 || g_loop == 8 || g_loop == 12 )
			g_count += 4;

		DataHeader*  pHeader;
		char buf[256];
		if ( m_eCornerType == CORNER_TL )
		{
			wsprintf( buf,strmap[g_count] );
			g_loop++;
		}
		else if ( m_eCornerType == CORNER_TR )
		{
			wsprintf( buf,strmap[g_count+1] );
			g_loop++;
		}
		else if ( m_eCornerType == CORNER_BL )
		{
			wsprintf( buf,strmap[g_count+2] );
			g_loop++;
		}
		else if ( m_eCornerType == CORNER_BR )
		{
			wsprintf( buf,strmap[g_count+3] );
			g_loop++;
		}
		if(g_pGround->m_pTextureData)
		{
			// 2010. 03. 05 by jskim �� �ε� �� ������ ȭ�� ��ȯ�� ���� ������� ���� ����
			if(m_pTexture)
			{
				SAFE_RELEASE(m_pTexture);
			}
			//end 2010. 03. 05 by jskim �� �ε� �� ������ ȭ�� ��ȯ�� ���� ������� ���� ����
			pHeader = g_pGround->m_pTextureData->Find( buf );
			if ( pHeader )
			{
				if( FAILED( D3DXCreateTextureFromFileInMemory( g_pD3dDev
					, pHeader->m_pData
					, pHeader->m_DataSize
					, &m_pTexture ) ) )
					return E_FAIL;
			}
		}
		else
		{
			return E_FAIL;
		}
		return S_OK;
	}
	return CQuadTree::InitDeviceObjects();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			RestoreDeviceObjects
/// \brief		Restore Device
/// \date		2004-03-13 ~ 2004-03-13
/// \warning	
///
/// \param		
/// \return		HRESULT
///////////////////////////////////////////////////////////////////////////////
HRESULT CQuadGround::RestoreDeviceObjects()
{
//	FLOG("CQuadGround::RestoreDeviceObjects()");
	CBackground * pGround = NULL;
	pGround = g_pGround;
	if ( !pGround )
	{
		return E_FAIL;
	}
	if ( m_nDegree > QUAD_BASE_DEGREE )
	{
		int nValue = 1;
		// �ܰ踦 ���� �Ͽ� ����� ���
		for ( int nCheckDegree = 0;nCheckDegree < m_nDegree;nCheckDegree++ )
		{
			nValue *=2;
		}
		nValue = m_nMaxSize/nValue;
		m_nSize = nValue;
		// �� ���� ���� ��ǥ�� ���
		m_vPos[0] = pGround->m_pTileVertexArray[(m_sStartX) * (m_nMaxSize + 1) + (m_sStartY)].pos;
		m_vPos[1] = pGround->m_pTileVertexArray[(nValue + m_sStartX) * (m_nMaxSize + 1) + (m_sStartY)].pos;
		m_vPos[2] = pGround->m_pTileVertexArray[(m_sStartX) * (m_nMaxSize + 1) + (nValue + m_sStartY)].pos;
		m_vPos[3] = pGround->m_pTileVertexArray[(nValue + m_sStartX) * (m_nMaxSize + 1) + (nValue + m_sStartY)].pos;
		// ���ؽ� ���۸� �����Ͽ� ����ǥ�� ����, �ε��� ���۸� �����ŭ ����
		if ( m_nDegree == QUAD_START_DEGREE )
		{
			GROUNDVERTEX *v;
//			GROUNDVERTEX *v1;
			g_pD3dDev->CreateVertexBuffer( (nValue + 1) * (nValue + 1) * sizeof(GROUNDVERTEX), 
				D3DUSAGE_WRITEONLY, 
				D3DFVF_GROUNDVERTEX, 
				D3DPOOL_MANAGED, 
				&m_pVBTest,
				NULL);
//			g_pD3dDev->CreateVertexBuffer( (nValue + 1) * (nValue + 1) * sizeof(GROUNDVERTEX), 
//				D3DUSAGE_WRITEONLY, 
//				D3DFVF_GROUNDVERTEX, 
//				D3DPOOL_MANAGED, 
//				&m_pVBTest1,
//				NULL);
			m_pVBTest->Lock( 0, 0, (void**)&v, 0 );
//			m_pVBTest1->Lock( 0, 0, (void**)&v1, 0 );
			int nStartX;
			int nStartY;

			//
			if ( m_sStartX >= QUAD_BLOCK_SIZE*3 )
			{
				nStartX = m_sStartX - QUAD_BLOCK_SIZE*3;
			}
			else if( m_sStartX >= QUAD_BLOCK_SIZE*2)
			{
				nStartX = m_sStartX - QUAD_BLOCK_SIZE*2;
			}
			else if( m_sStartX >= QUAD_BLOCK_SIZE )
			{
				nStartX = m_sStartX - QUAD_BLOCK_SIZE;
			}
			else
			{
				nStartX = m_sStartX;
			}

			//
			if ( m_sStartY >= QUAD_BLOCK_SIZE*3 )
			{
				nStartY = m_sStartY - QUAD_BLOCK_SIZE*3;
			}
			else if( m_sStartY >= QUAD_BLOCK_SIZE*2 )
			{
				nStartY = m_sStartY - QUAD_BLOCK_SIZE*2;
			}
			else if( m_sStartY >= QUAD_BLOCK_SIZE*1 )
			{
				nStartY = m_sStartY - QUAD_BLOCK_SIZE*1;
			}
			else
			{
				nStartY = m_sStartY;
			}
			for ( int nCheckX = 0 ;nCheckX < nValue + 1;nCheckX++ )
			{
				for ( int nCheckY = 0;nCheckY < nValue+1;nCheckY++ )
				{
					v[nCheckX * (nValue + 1) + nCheckY].p = 
						pGround->m_pTileVertexArray[(nCheckX + m_sStartX) * (m_nMaxSize + 1) 
						+ (nCheckY + m_sStartY)].pos;
					v[nCheckX * (nValue + 1) + nCheckY].tu = (nStartX + nCheckX) / (float)QUAD_BLOCK_SIZE;
					v[nCheckX * (nValue + 1) + nCheckY].tv = 1.0f - ((nStartY + nCheckY) / (float)QUAD_BLOCK_SIZE);
					v[nCheckX * (nValue + 1) + nCheckY].n = 
						pGround->m_pTileVertexArray[(nCheckX + m_sStartX) * (m_nMaxSize + 1) 
						+ (nCheckY + m_sStartY)].nor;

					// 2005-10-28 by ispark
					// ���� ���� ���� �Է�, ������ �ʿ� ���� ���ؼ�
					m_nMaxHeight = max(m_nMaxHeight, pGround->m_pTileVertexArray[(nCheckX + m_sStartX) * (m_nMaxSize + 1) 
								+ (nCheckY + m_sStartY)].pos.y);
				}
			}
			m_pVBTest->Unlock();
			g_pD3dDev->CreateIndexBuffer( (nValue * nValue) * 2 * sizeof( MYINDEX ), 
				D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 
				D3DFMT_INDEX16, 
				D3DPOOL_DEFAULT, 
				&m_pIBTest,
				NULL);
		}
	}
	return CQuadTree::RestoreDeviceObjects();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			InvalidateDeviceObjects
/// \brief		Invalidate Device
/// \date		2004-03-13 ~ 2004-03-13
/// \warning	1�ܰ� ������ ����
///
/// \param		
/// \return		HRESULT
///////////////////////////////////////////////////////////////////////////////
HRESULT CQuadGround::InvalidateDeviceObjects()
{
//	FLOG("CQuadGround::InvalidateDeviceObjects()");
//	DBGOUT("CQuadGround::InvalidateDeviceObjects()\n");
	// 1�ܰ� ������ VB, IB ������
	if ( m_nDegree == QUAD_START_DEGREE )
	{
		SAFE_RELEASE( m_pVBTest );				
//		SAFE_RELEASE( m_pVBTest1 );	
		SAFE_RELEASE( m_pIBTest );
		return S_OK;
	}

	m_nMaxHeight = 0;

	// 2010-10-20 by jskim, �� �ε� �� �ؽ��� �߸� �ҷ����� ���� ����
	g_count = 0;
	g_loop = 0;
	// end 2010-10-20 by jskim, �� �ε� �� �ؽ��� �߸� �ҷ����� ���� ����

	return CQuadTree::InvalidateDeviceObjects();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			DeleteDeviceObjects
/// \brief		Delete Device
/// \date		2004-03-13 ~ 2004-03-13
/// \warning	1�ܰ� ������ ����
///
/// \param		
/// \return		HRESULT
///////////////////////////////////////////////////////////////////////////////
HRESULT CQuadGround::DeleteDeviceObjects()
{
//	FLOG("CQuadGround::DeleteDeviceObjects()");
//	DBGOUT("CQuadGround::DeleteDeviceObjects()\n");
	if ( m_nDegree == QUAD_START_DEGREE )
	{
		SAFE_RELEASE( m_pTexture );
		return S_OK;
	}
	// 2010-10-20 by jskim, �� �ε� �� �ؽ��� �߸� �ҷ����� ���� ����
	//g_count = 0;
	//g_loop = 0;
	// end 2010-10-20 by jskim, �� �ε� �� �ؽ��� �߸� �ҷ����� ���� ����
	return CQuadTree::DeleteDeviceObjects();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SetIB
/// \brief		������ �Ǿ��� VB ����Ʈ�� IB�� ���
/// \date		2004-03-13 ~ 2004-03-13
/// \warning	
///
/// \param		
/// \return		void
///////////////////////////////////////////////////////////////////////////////
void CQuadGround::SetIB()
{
//	FLOG("CQuadGround::SetIB()");
	MYINDEX idx;
	int nStartX;
	int nStartY;

	if ( m_sStartX >= QUAD_BLOCK_SIZE*3 )
	{
		nStartX = m_sStartX - QUAD_BLOCK_SIZE*3;
	}
	else if( m_sStartX >= QUAD_BLOCK_SIZE*2)
	{
		nStartX = m_sStartX - QUAD_BLOCK_SIZE*2;
	}
	else if( m_sStartX >= QUAD_BLOCK_SIZE )
	{
		nStartX = m_sStartX - QUAD_BLOCK_SIZE;
	}
	else
	{
		nStartX = m_sStartX;
	}
	
	//
	if ( m_sStartY >= QUAD_BLOCK_SIZE*3 )
	{
		nStartY = m_sStartY - QUAD_BLOCK_SIZE*3;
	}
	else if( m_sStartY >= QUAD_BLOCK_SIZE*2 )
	{
		nStartY = m_sStartY - QUAD_BLOCK_SIZE*2;
	}
	else if( m_sStartY >= QUAD_BLOCK_SIZE )
	{
		nStartY = m_sStartY - QUAD_BLOCK_SIZE*1;
	}
	else
	{
		nStartY = m_sStartY;
	}
	for ( int nCheckX = nStartX;nCheckX < nStartX + m_nSize;nCheckX++ )
	{
		for ( int nCheckY = nStartY;nCheckY < nStartY + m_nSize;nCheckY++ )
		{
			idx._0 = nCheckX * (QUAD_BLOCK_SIZE + 1) + nCheckY;
			idx._1 = nCheckX * (QUAD_BLOCK_SIZE + 1) + nCheckY + 1;
			idx._2 = (nCheckX + 1)*(QUAD_BLOCK_SIZE+1) + nCheckY + 1;
			*((CQuadGround *)m_pOriParent)->m_pIdx++ = idx;
			((CQuadGround *)m_pOriParent)->m_nTriangleNumber++;

			idx._0 = nCheckX * (QUAD_BLOCK_SIZE + 1) + nCheckY;
			idx._1 = (nCheckX + 1) * (QUAD_BLOCK_SIZE + 1) + nCheckY + 1;
			idx._2 = (nCheckX + 1) * (QUAD_BLOCK_SIZE + 1) + nCheckY;
			*((CQuadGround *)m_pOriParent)->m_pIdx++ = idx;
			((CQuadGround *)m_pOriParent)->m_nTriangleNumber++;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			Tick
/// \brief		FrameMove
/// \date		2004-03-13 ~ 2004-03-13
/// \warning	
///
/// \param		void
/// \return		
///////////////////////////////////////////////////////////////////////////////

/// 2004.06.08 syjun, jschoi
/// ����Ʈ�� ����

void CQuadGround::Tick()
{
//	FLOG("CQuadGround::Tick()");

	D3DXVECTOR3 vCenter;

//	if( m_nDegree == QUAD_BASE_DEGREE )
	if( m_nDegree < QUAD_START_DEGREE )
	{
		CQuadTree::Tick();
	}

	else if ( m_nDegree == QUAD_APPLY_DEGREE )
	{
		vCenter = (m_vPos[0] + m_vPos[3]) / 2;
//		fRadius = D3DXVec3Length(&(vCenter-m_vPos[0]));

//		fRadius = D3DXVec3Length(&(m_vPos[1]-m_vPos[0]));

		BOOL bIn = g_pFrustum->CheckSphere(vCenter.x,vCenter.y,vCenter.z,m_fRadius);
		float fDistance = D3DXVec3Length( & ( vCenter - ( g_pCamera->m_vCamCurrentPos ) ) );
		if( ( (fDistance-m_fRadius) < g_pScene->m_fFogEndValue ) && bIn)
		{
			SetIB();
		}
		else
		{
			vCenter = (m_vPos[1] + m_vPos[2]) / 2;
			bIn = g_pFrustum->CheckSphere(vCenter.x,vCenter.y,vCenter.z,m_fRadius);
			fDistance = D3DXVec3Length( & ( vCenter - ( g_pCamera->m_vCamCurrentPos ) ) );
			if( ( (fDistance-m_fRadius) < g_pScene->m_fFogEndValue ) && bIn )
			{
				SetIB();
			}
		}
		return;
	}

	else if ( m_nDegree == QUAD_START_DEGREE )
	{
		vCenter = (m_vPos[0] + m_vPos[3]) / 2;
//		fRadius = D3DXVec3Length(&(vCenter-m_vPos[0]));
//		fRadius = D3DXVec3Length(&(m_vPos[1]-m_vPos[0]));

		BOOL bIn = g_pFrustum->CheckSphere(vCenter.x,vCenter.y,vCenter.z,m_fRadius);
		float fDistance = D3DXVec3Length( & ( vCenter - ( g_pCamera->m_vCamCurrentPos ) ) );
		if( ( (fDistance-m_fRadius) < g_pScene->m_fFogEndValue ) && bIn)
		{
			m_pIBTest->Lock( 0, (QUAD_BLOCK_SIZE*QUAD_BLOCK_SIZE) * 2 * sizeof( MYINDEX ), (void**)&m_pIdx, 0 );
			CQuadTree::Tick();
			m_pIBTest->Unlock();	
		}
		else
		{
			vCenter = (m_vPos[1] + m_vPos[2]) / 2;
			bIn = g_pFrustum->CheckSphere(vCenter.x,vCenter.y,vCenter.z,m_fRadius);
			fDistance = D3DXVec3Length( & ( vCenter - ( g_pCamera->m_vCamCurrentPos ) ) );
			if( ( (fDistance-m_fRadius) < g_pScene->m_fFogEndValue ) && bIn )
			{
				m_pIBTest->Lock( 0, (QUAD_BLOCK_SIZE*QUAD_BLOCK_SIZE) * 2 * sizeof( MYINDEX ), (void**)&m_pIdx, 0 );
				CQuadTree::Tick();
				m_pIBTest->Unlock();	
			}
		}
		return;
	}

	else if ( m_nDegree > QUAD_START_DEGREE )
	{
		
		vCenter = (m_vPos[0] + m_vPos[3]) / 2;
//		fRadius = D3DXVec3Length(&(vCenter-m_vPos[0]));
//		fRadius = D3DXVec3Length(&(m_vPos[1]-m_vPos[0]));

		BOOL bIn = g_pFrustum->CheckSphere(vCenter.x,vCenter.y,vCenter.z,m_fRadius);
		if( bIn )
		{
			CQuadTree::Tick();
		}
		else
		{
			vCenter = (m_vPos[1] + m_vPos[2]) / 2;
			bIn = g_pFrustum->CheckSphere(vCenter.x,vCenter.y,vCenter.z,m_fRadius);
			if( bIn )
			{
				CQuadTree::Tick();
			}
		}
		return;
	}
}
	

/*
	CBackground * pGround;
	pGround = g_pGround;
	CCamera * pCam = NULL;
	pCam = g_pD3dApp->m_pCamera;
	if ( !pGround || !pCam )
	{// Error
		return;
	}
	// �ڽ� �������� �˻� �� IB �ۼ�, Ÿ�� �� ����Ʈ �ۼ��� �Ѵ�
	// IB �� ��� ���� ��忡�� Lock �ϰ� �ڽ��� Tick�� ��� �����Ѵ��� �������� Unlock�ϰ� �ȴ�
	if ( m_nDegree == QUAD_START_DEGREE )
	{
		m_vecRenderTileList.clear();
		m_pIBTest->Lock( 0, (QUAD_BLOCK_SIZE*QUAD_BLOCK_SIZE) * 2 * sizeof( MYINDEX ), (BYTE**)&m_pIdx, 0 );
	}

	if ( m_nDegree != QUAD_BASE_DEGREE )
	{
		if ( m_vPos[0].x < pCam->m_fMinX 
			&& m_vPos[1].x < pCam->m_fMinX 
			&& m_vPos[2].x < pCam->m_fMinX 
			&& m_vPos[3].x < pCam->m_fMinX )
		{
			if ( m_nDegree == QUAD_START_DEGREE )
			{
				m_pIBTest->Unlock();
			}
			return;
		}
		else if ( m_vPos[0].x > pCam->m_fMaxX 
			&& m_vPos[1].x > pCam->m_fMaxX 
			&& m_vPos[2].x > pCam->m_fMaxX 
			&& m_vPos[3].x > pCam->m_fMaxX )
		{
			if ( m_nDegree == QUAD_START_DEGREE )
			{
				m_pIBTest->Unlock();
			}
			return;
		}
		else if ( m_vPos[0].z < pCam->m_fMinY 
			&& m_vPos[1].z < pCam->m_fMinY 
			&& m_vPos[2].z < pCam->m_fMinY 
			&& m_vPos[3].z < pCam->m_fMinY )
		{
			if ( m_nDegree == QUAD_START_DEGREE )
			{
				m_pIBTest->Unlock();
			}
			return;
		}
		else if ( m_vPos[0].z > pCam->m_fMaxY 
			&& m_vPos[1].z > pCam->m_fMaxY 
			&& m_vPos[2].z > pCam->m_fMaxY 
			&& m_vPos[3].z > pCam->m_fMaxY )
		{
			if ( m_nDegree == QUAD_START_DEGREE )
			{
				m_pIBTest->Unlock();
			}
			return;
		}
		byte byCheck1, byCheck2, byCheck3, byCheck4;
		byCheck1 = pCam->CheckCullCam( m_vPos[0] );
		byCheck2 = pCam->CheckCullCam( m_vPos[1] );
		byCheck3 = pCam->CheckCullCam( m_vPos[2] );
		byCheck4 = pCam->CheckCullCam( m_vPos[3] );
		if ( byCheck1 
			&& byCheck2 
			&& byCheck3 
			&& byCheck4)
		{// ��ü�� ��������
			if ( m_nSize == QUAD_LAST_SIZE )
			{// ������ ����ΰ� 
				if ( !(byCheck1 == GROUND_RENDER_TILE 
					|| byCheck2 == GROUND_RENDER_TILE 
					|| byCheck3 == GROUND_RENDER_TILE 
					|| byCheck4 == GROUND_RENDER_TILE) )
				{
					SetIB();
					if ( m_nDegree == QUAD_START_DEGREE )
					{
						m_pIBTest->Unlock();
					}
					return;
				}
				else
				{
					int nTileNum = m_sStartX * pGround->m_projectInfo.sYSize + m_sStartY;
					((CQuadGround *)m_pOriParent)->m_vecRenderTileList.push_back( nTileNum );
					if ( m_nDegree == QUAD_START_DEGREE )
					{
						m_pIBTest->Unlock();
					}
					return;
				}
			}
			else
			{
				if ( !(byCheck1 == GROUND_RENDER_TILE 
					|| byCheck2 == GROUND_RENDER_TILE 
					|| byCheck3 == GROUND_RENDER_TILE 
					|| byCheck4 == GROUND_RENDER_TILE) )
				{
					SetIB();
					if ( m_nDegree == QUAD_START_DEGREE )
					{
						m_pIBTest->Unlock();
					}
					return;
				}
			}
		}
		else
		{// �Ϻκ��� ��������
			if ( m_nSize == QUAD_LAST_SIZE )
			{
				if ( !(byCheck1 == GROUND_RENDER_TILE 
					|| byCheck2 == GROUND_RENDER_TILE 
					|| byCheck3 == GROUND_RENDER_TILE 
					|| byCheck4 == GROUND_RENDER_TILE) )
				{
					SetIB();
					if ( m_nDegree == QUAD_START_DEGREE )
					{
						m_pIBTest->Unlock();
					}
					return;
				}
				else
				{
					int nTileNum = m_sStartX * pGround->m_projectInfo.sYSize + m_sStartY;
					((CQuadGround *)m_pOriParent)->m_vecRenderTileList.push_back( nTileNum );
					if ( m_nDegree == QUAD_START_DEGREE )
					{
						m_pIBTest->Unlock();
					}
					return;
				}
			}
		}
	}
		
	CQuadTree::Tick();

	if ( m_nDegree == QUAD_START_DEGREE )
	{
		m_pIBTest->Unlock();
	}

}
*/

///////////////////////////////////////////////////////////////////////////////
/// \fn			Render
/// \brief		Rendering
/// \date		2004-03-13 ~ 2004-03-13
/// \warning	1�ܰ�(Start Node) ������ ����
///
/// \param		
/// \return		void
///////////////////////////////////////////////////////////////////////////////
void CQuadGround::Render()
{// ī�޶��� ������� ����� ���� ��ü�� ���ϸ� �������ϰ� ���� �׷��� ������ �ڽ��� �������Ѵ�.
//	FLOG("CQuadGround::Render()");
	CBackground * pGround;
	pGround = g_pGround;
	if ( !pGround )
	{// Error
		return;
	}

	if ( m_nDegree == QUAD_START_DEGREE )
	{


		// 2009. 01. 06 by ckPark ���� ����ȭ
		
// 		D3DXMATRIX mat;
// 		D3DXMatrixIdentity( &mat );
// 		g_pD3dDev->SetTransform( D3DTS_WORLD, &mat );
// 		g_pD3dDev->SetFVF( D3DFVF_GROUNDVERTEX );

		// end 2009. 01. 06 by ckPark ���� ����ȭ


		if ( m_nTriangleNumber )
		{
//			g_pD3dDev->SetTexture( 0, NULL );
			g_pD3dDev->SetTexture( 0, m_pTexture );

			// ������ �� �ڵ�
			if(g_pGround->m_pDetailMap && g_pShuttleChild->m_vPos.y < m_nMaxHeight + 600)	// m_nMaxHeight + 600�� ������ �� ��� �����̴�
			{
				
				

				// 2009. 01. 06 by ckPark ���� ����ȭ

// 				D3DXMATRIXA16	matWorld;
// 				D3DXMatrixIdentity( &matWorld );
// 				
// 				matWorld._11 = 5;
// 				matWorld._22 = 5;
// 				
// 				g_pD3dDev->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 );
// 				g_pD3dDev->SetTransform( D3DTS_TEXTURE1, &matWorld );

				// end 2009. 01. 06 by ckPark ���� ����ȭ


				g_pD3dDev->SetTexture(1 , g_pGround->m_pDetailMap);



				// 2009. 01. 06 by ckPark ���� ����ȭ

// 				g_pD3dDev->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
// 	//			g_pD3dDev->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
// 				g_pD3dDev->SetSamplerState(1,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
// 				g_pD3dDev->SetSamplerState(1,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
// 				g_pD3dDev->SetSamplerState(1,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
// 				g_pD3dDev->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );		// 0�� �ؽ�ó : 0�� �ؽ�ó �ε��� ���
// 				g_pD3dDev->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 0 );		// 1�� �ؽ�ó : 0�� �ؽ�ó �ε��� ���
// 				g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
// 				g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
// 				g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

				// end 2009. 01. 06 by ckPark ���� ����ȭ



				g_pD3dDev->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_ADDSIGNED );



				// 2009. 01. 06 by ckPark ���� ����ȭ

// 				g_pD3dDev->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
// 				g_pD3dDev->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );

				// end 2009. 01. 06 by ckPark ���� ����ȭ



			}
			
			g_pD3dDev->SetStreamSource( 0,m_pVBTest,0,sizeof(GROUNDVERTEX) );
//			g_pD3dDev->SetStreamSource( 0,m_pVBTest1,0,sizeof(GROUNDVERTEX) );

//			g_pD3dDev->SetTexture( 0, m_pTexture );

			// 2005-01-04 by jschoi
			g_pD3dDev->SetIndices( m_pIBTest );

			g_pD3dDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0,
				0, 
				((QUAD_BLOCK_SIZE + 1) * (QUAD_BLOCK_SIZE + 1)), 
				0, 
				m_nTriangleNumber );
			m_nTriangleNumber = 0;
			g_pD3dDev->SetTexture(1 , NULL);
			g_pD3dDev->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE  );
		}
/*		vector<int>::iterator itTile = m_vecRenderTileList.begin();
		while ( itTile != m_vecRenderTileList.end() )
		{
			int nCheckTileNum;
			int nCheckX;
			int nCheckY;
			nCheckTileNum = *itTile;
			nCheckX = nCheckTileNum / pGround->m_projectInfo.sYSize;
			nCheckY = nCheckTileNum % pGround->m_projectInfo.sYSize;
			if ( nCheckX >= 0 
				&& nCheckX < pGround->m_projectInfo.sXSize 
				&& nCheckY >= 0 
				&& nCheckY < pGround->m_projectInfo.sYSize )
			{
				g_pD3dDev->SetStreamSource( 0, pGround->m_pVBRender[nCheckX], sizeof( GROUNDVERTEX ) );
				if ( g_pD3dApp->m_pScene->m_pCreateTexture[pGround->m_pTileInfo[nCheckTileNum].useTexNumber] )
				{// �ؽ��İ� ������
					g_pD3dDev->SetTexture( 0, 
						g_pD3dApp->m_pScene->m_pCreateTexture[pGround->m_pTileInfo[nCheckTileNum].useTexNumber] );
				}
				else
				{// �ؽ��İ� ������ - �ؽ��ĸ� �ε��Ѵ�
					if ( g_pD3dApp->m_pScene->m_pData )
					{
						char szFilename[256];
						wsprintf( szFilename, "05%06d", pGround->m_pTileInfo[nCheckTileNum].useTexNumber );
						DataHeader * pHeader = g_pD3dApp->m_pScene->m_pData->Find( szFilename );
						if ( pHeader )
						{
							D3DXCreateTextureFromFileInMemory( g_pD3dDev, 
								pHeader->m_pData, 
								pHeader->m_DataSize, 
								&g_pD3dApp->m_pScene->m_pCreateTexture[pGround->m_pTileInfo[nCheckTileNum].useTexNumber]);
							if ( g_pD3dApp->m_pScene->m_pCreateTexture[pGround->m_pTileInfo[nCheckTileNum].useTexNumber] )
							{// �ε��� �ؽ��İ� ������
								g_pD3dDev->SetTexture( 0, 
									g_pD3dApp->m_pScene->m_pCreateTexture[pGround->m_pTileInfo[nCheckTileNum].useTexNumber] );
							}
							else
							{// �ε��� �ؽ��İ� ������ - Error
							}
						}
					}
				}
				g_pD3dDev->DrawPrimitive( D3DPT_TRIANGLESTRIP, nCheckY * 4, 2 );
			}
			itTile++;
		}
*/
		return;
	}
	CQuadTree::Render();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SubDivide
/// \brief		���� ��� ����
/// \date		2004-03-13 ~ 2004-03-13
/// \warning	
///
/// \param		
/// \return		void
///////////////////////////////////////////////////////////////////////////////
void CQuadGround::SubDivide()
{
//	FLOG("CQuadGround::SubDivide()");
	int nValue = 1;
	for ( int nCheckDegree = 0;nCheckDegree < m_nDegree+1;nCheckDegree++ )
	{
		nValue *= 2;
	}
	nValue = m_nMaxSize / nValue;
	if(m_nDegree < QUAD_LAST_DEGREE)
	{
		CQuadGround * pTree = NULL;
		pTree = new CQuadGround( this, CORNER_TL, m_sStartX + nValue, m_sStartY, m_nMaxSize );
		AddChild( pTree );
		pTree = new CQuadGround( this, CORNER_TR, m_sStartX + nValue, m_sStartY + nValue, m_nMaxSize );
		AddChild( pTree );
		pTree = new CQuadGround( this, CORNER_BL, m_sStartX, m_sStartY, m_nMaxSize );
		AddChild( pTree );
		pTree = new CQuadGround( this, CORNER_BR, m_sStartX, m_sStartY + nValue, m_nMaxSize );
		AddChild( pTree );
		for ( int nCheckChild = 0;nCheckChild < 4;nCheckChild++ )
		{
			((CQuadGround *)m_pChild[nCheckChild])->SubDivide();
		}

	}

}

void CQuadGround::GetMinMax(float fMax, float fMin)
{
	fMax = max(max(max(m_vPos[0].y, m_vPos[1].y), max(m_vPos[2].y, m_vPos[3].y)), fMax);
	fMin = min(min(min(m_vPos[0].y, m_vPos[1].y), min(m_vPos[2].y, m_vPos[3].y)), fMin);
	if(m_nDegree < QUAD_APPLY_DEGREE)
	{
		for ( int i = 0;i < 4;i++ )
		{
			((CQuadGround *)m_pChild[i])->GetMinMax( fMax, fMin );
		}
	}
//	m_fMax = fMax;
//	m_fMin = fMin;
	m_fRadius = fMax - fMin;
	float fRadius = max(max(max(D3DXVec3Length(&(m_vPos[0] - m_vPos[3]))/2,D3DXVec3Length(&(m_vPos[1] - m_vPos[2]))/2),
						max(D3DXVec3Length(&(m_vPos[0] - m_vPos[2]))/2,D3DXVec3Length(&(m_vPos[2] - m_vPos[3]))/2)),
						max(D3DXVec3Length(&(m_vPos[0] - m_vPos[1]))/2,D3DXVec3Length(&(m_vPos[1] - m_vPos[3]))/2));
	if(m_fRadius < fRadius)
	{
		m_fRadius  = fRadius;
	}
//	DBGOUT("������ : %.2f\n",m_fRadius);
}
