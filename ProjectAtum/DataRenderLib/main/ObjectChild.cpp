// ObjectChild.cpp: implementation of the CObjectChild class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ObjectChild.h"
//#include "ObjectRender.h"
#include "AtumApplication.h"
#include "Background.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "SceneData.h"
#include "Camera.h"
#include "AtumDatabase.h"
#include "ChatMoveData.h"
#include "ObjRender.h"
#include "Interface.h"
#include "dxutil.h"
#include "INFGameMain.h"		// 2008-10-15 by bhsohn ���ҽ� �޸� ��ȣ ��� �߰�

extern LPDIRECT3DDEVICE9	g_pD3dDev;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObjectChild::CObjectChild(OBJECTINFOCLIENT objInfo, OBJECTSCALEINFO ScaleInfo)
{
	FLOG( "CObjectChild(OBJECTINFOCLIENT objInfo, OBJECTSCALEINFO ScaleInfo)" );

	m_bShowNode = TRUE;	// 2013-05-07 by bhsohn ��������Ʈ ������ �ý���

	m_pCharacterInfo = NULL;
	m_nObjectCont = objInfo.nObjCount;
	m_pRenderer = g_pScene->m_pObjectRender;
	m_vPos = objInfo.vPos;
	m_vUp = objInfo.vUp;
	m_vScale = ScaleInfo.vObjScale;		// 2012-05-02 by isshin ���� ���� ������Ʈ ������ ����
	D3DXVec3Normalize(&m_vUp,&m_vUp);
	m_vVel = objInfo.vVel;
	m_nCode = objInfo.dwObjType;
	m_pObjectInfo = g_pDatabase->CheckObjectInfo(m_nCode);
	if(m_pObjectInfo == NULL && IS_OBJECT_MONSTER_OBJECT_CODE(m_nCode))
	{
		return;
	}

	m_dwObjectMonsterUniqueNumber = objInfo.dwObjectMonsterUniqueNumber;
	m_bObjectTexIndex = objInfo.bObjectTexIndex;
	m_nNextEventIndex = objInfo.nObjectSrcIndex;
	BodyCond_t temp = 1;
	temp <<= objInfo.bBodyCondition;
	m_BodyCondition = temp;
	m_bIsEvent = objInfo.bIsEvent;
	m_bEventType = objInfo.bEventType;
	m_sEventIndexFrom = objInfo.sEventIndexFrom;
	m_sEventIndexTo = objInfo.sEventIndexTo;
	m_sEventIndex3 = objInfo.sEventIndex3;
	memset(m_strEventParam, 0x00, sizeof(m_strEventParam));
	strcpy(m_strEventParam, objInfo.strEventParam);
	// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
	//m_pObjMesh = m_pRenderer->InitData(m_nCode);
	m_pObjMesh = m_pRenderer->InitData(m_nCode ,_OBJECT_TYPE, m_vScale);	// 2012-05-21 by isshin ���� ���� ������Ʈ ������ ����
	//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
	if(m_pObjMesh == NULL && !IS_OBJECT_MONSTER_OBJECT_CODE(m_nCode) )
	{
		// 2008-10-15 by bhsohn ���ҽ� �޸� ��ȣ ��� �߰�
//		DBGOUT("Map Object Loading Error ~ Fuck(%d)\n", m_nCode);
//		g_pD3dApp->NetworkErrorMsgBox(STRERR_C_RESOURCE_0001);
//		return;				
		DBGOUT("Map Object Loading Error ~(%d)\n", m_nCode);
		char ErrorMsgMissionList[256];
		wsprintf(ErrorMsgMissionList, "Map Object Loading Error ~ (%d)", m_nCode);		
		BOOL bOper = FALSE;
		if(g_pGameMain)
		{
			bOper = g_pGameMain->CreateChatChild_OperationMode(ErrorMsgMissionList, COLOR_ERROR);
		}		
		if(bOper)
		{
			// �����ڸ� �⺻ ����Ʈ ������
			m_nCode = NOT_HAVE_OBJECT;		// �⺻����Ʈ
			// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
			//m_pObjMesh = m_pRenderer->InitData(m_nCode);				// �⺻ ����Ʈ�� �����Ͷ�
			m_pObjMesh = m_pRenderer->InitData(m_nCode, _OBJECT_TYPE);				// �⺻ ����Ʈ�� �����Ͷ�
			//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
		}
		if(NULL == m_pObjMesh)
		{
			// �⺻����Ʈ�� ������ ����			
			g_pD3dApp->NetworkErrorMsgBox(STRERR_C_RESOURCE_0001);
			return;				
		}
		// end 2008-10-15 by bhsohn ���ҽ� �޸� ��ȣ ��� �߰�
	}

	m_vOriPos = m_vPos;
	m_ptBlockPos.x = (int)(m_vOriPos.x/MAP_BLOCK_SIZE);
	m_ptBlockPos.y = (int)(m_vOriPos.z/MAP_BLOCK_SIZE);

	m_bCheckAttack = FALSE;
	m_fCurrentTime = 0.0f;
	m_bCheckAni = TRUE;
	m_bCheckColl = FALSE;
	CheckShadowPos();

	D3DXMatrixLookAtLH( &m_mMatrix, &m_vPos, &(m_vPos+m_vVel), &m_vUp);
	D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );
	
	// 2012-05-02 by isshin ���� ���� ������Ʈ ������ ����
	D3DXMatrixIdentity(&m_mScaleMatrix);	
	D3DXMatrixScaling(&m_mScaleMatrix, m_vScale.x, m_vScale.y , m_vScale.z);
	//m_mMatrix = m_mScaleMatrix * m_mMatrix;
	D3DXMatrixMultiply(&m_mMatrix, &m_mScaleMatrix, &m_mMatrix );
	// end 2012-05-02 by isshin ���� ���� ������Ʈ ������ ����


// 2007-06-04 by dgwoo �ػ� Low�ϰ�� ����Ʈ�� �ִϸ��̼��� �۵��� �ȵȴ�.
//	if(g_pD3dApp->m_bDegree != 0)
	{
		char strFileName[128];
		wsprintf( strFileName, "%08d", m_nCode);
		LoadCharacterEffect(strFileName);

		if(m_pCharacterInfo)
		{
			// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
			m_pCharacterInfo->m_LoadingPriority = _NOTHING_STEP;
			//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
			// 2007-04-23 by dgwoo �ִϸ��̼��� �Ⱥ��ϰ�쿣 �ּ��� Ǯ���ش�.
			//if(m_nCode == ARENA_DOOR_OBJECT)
			if(m_pCharacterInfo->m_mapBodyCondition.size() == 4)
			{// �Ʒ��� �� ������Ʈ�� ���.
				m_pCharacterInfo->SetCharacterAnimationBodyConditionMask(BODYCON_ARENA_DOOR_STATE);
				ChangeBodyconditionEvent(BODYCON_DOOR_CLOSE_MASK);
			}
			else if(m_pCharacterInfo->m_mapBodyCondition.size() == 2 )
			{// ���� ������Ʈ.
				m_pCharacterInfo->SetCharacterAnimationBodyConditionMask(BODYCON_WARP_STATE);
			}else
			{// ����Ʈ ������Ʈ.
				m_pCharacterInfo->SetCharacterAnimationBodyConditionMask(BODYCON_LANDED_MASK);
			}
			m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
			m_pCharacterInfo->SetBodyCondition(m_BodyCondition);
		}

	}
	m_bCheckRenderDist = TRUE;

	// 2005-01-05 by jschoi - ���� ���� �޽� ��� ����.
//	m_bCheckBlend = FALSE;

	// 2004-11-29 by jschoi - ������Ʈ ���� ������Ʈ ���۽� Ȱ��ȭ ����
	if(IS_OBJECT_MONSTER_OBJECT_CODE(m_nCode))
	{
		m_bEnableObjectMonsterObject = TRUE;
	}
	else
	{
		m_bEnableObjectMonsterObject = FALSE;
	}

	if( strlen(objInfo.strEventParam) > 0 )
	{
		m_pMoveChat = new CChatMoveData(this, objInfo.strEventParam, COLOR_SKILL_USE, 9, 512, 32);
//		m_pMoveChat->InitDeviceObjects();
//		m_pMoveChat->RestoreDeviceObjects();
		AddChild(m_pMoveChat);
	}
	else
	{
		m_pMoveChat = NULL;
	}

//	if(m_bIsEvent)	// �̺�Ʈ ������Ʈ
//	{
//		DBGOUT("Map Event Object (%d) ----> Check\n", m_nCode);
//	}
//	else if(m_pObjectInfo)
//	{
//		DBGOUT("Map Object (%d), Type(%d) ----> Check\n", m_nCode, m_pObjectInfo->ObjectRenderType);
//	}
//	else
//	{
//		DBGOUT("Map Normal Object (%d), Type(%d) ----> Check\n", m_nCode);
//	}
}

CObjectChild::~CObjectChild()
{
	FLOG( "~CObjectChild()" );
//	SAFE_RELEASE(m_pVBShadow);
	m_pRenderer = NULL;
	m_pObjectInfo = NULL;
//ysw3_16
	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->InvalidateDeviceObjects();
		m_pCharacterInfo->DeleteDeviceObjects();	
	}
	util::del(m_pCharacterInfo) ;
//	if(m_pMoveChat)
//	{
//		m_pMoveChat->InvalidateDeviceObjects();
//		m_pMoveChat->DeleteDeviceObjects();
//		util::del(m_pMoveChat);
//	}
}

void CObjectChild::Tick()
{
	FLOG( "CObjectChild::Tick()" );
//////////////////////////// 2004-06-18 by dhkwon
//	USHORT nRenderIndex = m_pObjectInfo->RenderIndex;
//	if(!m_pRenderer->m_pMeshNode[nRenderIndex] || m_pRenderer->m_pMeshNode[nRenderIndex]->m_nRenderCount==0)
//		m_pRenderer->ObjectInitData(nRenderIndex,m_pObjectInfo->Code);
//	else if(m_pRenderer->m_pMeshNode[nRenderIndex]->m_nRenderCount==1)
//		m_pRenderer->ObjectRestoreData(nRenderIndex);
//	else if(m_pRenderer->m_pMeshNode[nRenderIndex]->m_nRenderCount==2)
//		m_pRenderer->m_pMeshNode[nRenderIndex]->m_nRenderCount++;
//////////////////////////

// ���� ���� �ӽ� ���� 2004-09-02 jschoi

//	float fRadius = 0;
//	if(m_pObjMesh)
//	{
//		fRadius = m_pObjMesh->m_fRadius;
//	}
//
//	if(!m_pObjectInfo)
//	{
//		m_pObjectInfo = g_pDatabase->CheckObjectInfo(m_nCode);
//		if(!m_pObjectInfo)
//		{
//			return;
//		}
//	}

//	int nX = g_pShuttleChild->m_ptOldPoint.x - m_ptBlockPos.x;
//	int nZ = g_pShuttleChild->m_ptOldPoint.y - m_ptBlockPos.y;
//	if((nX < -40 || nX > 40 ) && (nZ < -40 || nZ > 40))
//	{
//		m_bCheckRenderDist = FALSE;
//		return;
//	}
//	m_bCheckAttack = FALSE;
//	m_bCheckBlend = FALSE;
//	m_bCheckColl = FALSE;

	// 2004-11-04 by jschoi
	// ������Ʈ �ִϸ��̼� �ð� ����
	
	// 2007-04-23 by dgwoo bodycondition test
	if(m_pCharacterInfo)
	{
		m_fCurrentTime = m_pCharacterInfo->GetCurrentCharacterAnimationTime();
	}
	else
	{
		m_fCurrentTime += 20.0f*g_pD3dApp->GetElapsedTime();
		if(m_fCurrentTime >= 10000.0f)
			m_fCurrentTime = 0.0f;
	}

//	D3DXVECTOR3 vDir = m_vOriPos - g_pShuttleChild->m_vPos;
//	D3DXVec3Normalize(&vDir,&vDir);
//
	// ��ũ������ ��ǥ

	if(m_pMoveChat)
	{
		g_pD3dApp->CalcObjectSourceScreenCoords(m_vOriPos, g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height, 
			m_nObjScreenX, m_nObjScreenY, m_nObjScreenW );
//		m_fDistanceCamera = D3DXVec3Length(&(g_pD3dApp->m_pCamera->GetEyePt()-m_vOriPos));// ī�޶���� �Ÿ�
	}
	// 2004-12-15 by jschoi - ī�޶���� �Ÿ��� ���ϱ� ����
	m_fDistanceCamera = D3DXVec3Length(&(g_pD3dApp->m_pCamera->GetEyePt()-m_vOriPos));
//	// ��Ʋ�� ���� üũ�� �Ұ��ΰ�?
//	D3DXVECTOR3 v1,v2;
//	D3DXVec3Normalize(&v1,&(g_pShuttleChild->m_vPos - g_pD3dApp->m_pCamera->GetEyePt()));
//	D3DXVec3Normalize(&v2,&(m_vOriPos - g_pD3dApp->m_pCamera->GetEyePt()));
//	float fDistance = D3DXVec3Length(&(g_pShuttleChild->m_vPos-m_vOriPos));

//	// ī�޶��� �պκ�(���¹���)�� ������ ��Ʋ���� ���ʿ� �ִٸ�
//	if(m_nObjScreenW > 0 && g_pShuttleChild->m_fDistanceCamera + fRadius > m_fDistanceCamera)
//	{
//		// ������Ʈ�� ũ�⿡ ���� ��Ʋ�� �������Ͱ��ٸ�
//		if(sin(ACOS(D3DXVec3Dot(&v1,&v2)))*m_fDistanceCamera <= fRadius)
//		{
//			m_bCheckBlend = TRUE;
////			g_pShuttleChild->m_bCheckBlend = TRUE;
//		}
//	}



//	if(m_pObjMesh && m_pObjectInfo && m_pObjectInfo->Collision)
//	{
//		// ��Ʋ�� �Ʒ��� �ִ��� �Ǵ�(x,z ��ǥ�� �Ǵ�)
//		D3DXVECTOR3 vPos1 = g_pShuttleChild->m_vPos;
//		D3DXVECTOR3 vPos2 = m_vOriPos;
//		float fDistance = g_pD3dApp->Distance3DTo2D(vPos2,vPos1);
//		float fRadius = m_pObjMesh->m_fRadius;
//		if(fDistance <= fRadius)
//			g_pShuttleChild->m_bOnObject = TRUE;

//		if(fDistance < fRadius+15.0f)
//		{
//			m_bCheckColl = TRUE;
//			g_pShuttleChild->m_bCollObjectCheck++;		// ��Ʋ���� �浹 üũ��ƾ�� ����� ����
//		}

//		D3DXVECTOR3 v2;
//		D3DXVec3Normalize(&v2,&(m_vOriPos - g_pShuttleChild->m_vPos));
//		if(sin(ACOS(D3DXVec3Dot(&g_pShuttleChild->m_vVel,&v2)))*D3DXVec3Length(&(g_pShuttleChild->m_vPos - m_vOriPos)) <= fRadius)
//		{
//			m_bCheckAttack = TRUE;
//			g_pShuttleChild->m_bCollAttackCheck++;	// ��Ʋ�� Attack �� �浹 üũ ��ƾ�� ����� ����
//		}	
// 	}

	// effect matrix & ticking

	if(m_pCharacterInfo)
	{
		float fElapsedTime = g_pD3dApp->GetElapsedTime();
		m_pCharacterInfo->SetAllBodyConditionMatrix( m_mMatrix );
		m_pCharacterInfo->Tick(fElapsedTime);
	}

//	CAtumNode::Tick();
}

void CObjectChild::Render()
{
	FLOG( "CObjectChild::Render()" );
	// 2013-05-07 by bhsohn ��������Ʈ ������ �ý���
 	if(!IsShowNode())
 	{
 		return;
 	}
	// END 2013-05-07 by bhsohn ��������Ʈ ������ �ý���
	g_pD3dDev->SetRenderState( D3DRS_ZENABLE,  TRUE );
	if(m_pObjectInfo && m_pRenderer)
	{
		if( m_pObjectInfo->ObjectRenderType != OBJECT_BIG_EFFECT)
		{
//			if( m_pObjMesh &&
//				(m_pObjMesh->m_fRadius > MIP_FILTER_DISTANCE*5 ||
//				 D3DXVec3Length(&(g_pCamera->GetEyePt() - m_vPos)) > 
//		 		 MIP_FILTER_DISTANCE+(m_pObjMesh==NULL?0:m_pObjMesh->m_fRadius)))
//			{
				// 2005-01-03 by jschoi
//				g_pD3dDev->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
//				g_pD3dDev->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );		
//				g_pD3dDev->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR );		
//				g_pD3dDev->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
//				g_pD3dDev->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
//				g_pD3dDev->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
//
//			}
//			else
//			{
				// 2005-01-03 by jschoi
//				g_pD3dDev->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_NONE );
//				g_pD3dDev->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_NONE );		
//				g_pD3dDev->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_NONE );	
//				g_pD3dDev->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_NONE);
//				g_pD3dDev->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_NONE);
//				g_pD3dDev->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_NONE);
//
//			}
			m_pRenderer->Render(this);
		}
//#ifdef _SCREENSHOT // ��ũ�� �� ���
//		if( g_pInterface->m_bShowInterface )
//		{
//#endif
//			if( m_pMoveChat && 
//				m_pObjMesh &&
//				m_fDistanceCamera > m_pObjMesh->m_fRadius &&
//				m_fDistanceCamera < g_pScene->m_fFogStartValue &&
//				m_nObjScreenW > 0)
//			{
//				m_pMoveChat->Render( m_nObjScreenX, m_nObjScreenY );
//			}
//#ifdef _SCREENSHOT
//		}
//#endif
	}

}

void CObjectChild::ObjectNameRender()
{
	// 2013-05-07 by bhsohn ��������Ʈ ������ �ý���
 	if(!IsShowNode())
 	{
 		return;
 	}
	// END 2013-05-07 by bhsohn ��������Ʈ ������ �ý���

	g_pD3dDev->SetRenderState( D3DRS_ZENABLE,  FALSE );
//#ifdef _SCREENSHOT // ��ũ�� �� ���
	// 2007-11-28 by bhsohn ��ũ�� �� ��� �߰�
	//if( g_pInterface->m_bShowInterface )
	// 2007-12-29 by bhsohn ������Ʈ �̸� �ȳ����� ���� ����
	if( g_pInterface->m_bShowInterface && !g_pInterface->IsScreenShotMode())
	{
//#endif
		if( m_pMoveChat && 
			m_pObjMesh &&
			m_fDistanceCamera > m_pObjMesh->m_fRadius &&
			m_fDistanceCamera < g_pScene->m_fFogStartValue &&
			m_nObjScreenW > 0)
		{
			m_pMoveChat->Render( m_nObjScreenX, m_nObjScreenY );
		}
//#ifdef _SCREENSHOT
	}
//#endif
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2013-05-07 by bhsohn ��������Ʈ ������ �ý���
/// \date		2013-05-07 ~ 2013-05-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CObjectChild::IsShowNode()
{
	return m_bShowNode ;	
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2013-05-07 by bhsohn ��������Ʈ ������ �ý���
/// \date		2013-05-07 ~ 2013-05-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CObjectChild::SetShowNode(BOOL bShowNode)
{	
	m_bShowNode = bShowNode;	
}


void CObjectChild::CheckShadowPos()
{
	FLOG( "CObjectChild::CheckShadowPos()" );
	if( g_pSOption->sShadowState == FALSE )
	{
		return;
	}
	if(!m_pObjectInfo)
		return;
	D3DXVECTOR3 vSide,vUp,vVel;
	D3DXMATRIX	matTemp;
	vUp = D3DXVECTOR3(0,1,0);
	vVel = g_pScene->m_light0.Direction;

	D3DXMatrixRotationAxis( &matTemp, &vUp, (D3DX_PI*0.5f));
	D3DXVec3TransformCoord( &vSide, &vVel, &matTemp );
	D3DXVec3Normalize(&vSide, &vSide);

	vVel.y = 0;
	D3DXVec3Normalize(&vVel,&vVel);
//////////////////////////// 2004-06-18 by dhkwon
//	USHORT nRenderIndex = m_pObjectInfo->RenderIndex;
//	if(!m_pRenderer->m_pMeshNode[nRenderIndex] || m_pRenderer->m_pMeshNode[nRenderIndex]->m_nRenderCount==0)
//		m_pRenderer->ObjectInitData(nRenderIndex,m_pObjectInfo->Code);
//	else if(m_pRenderer->m_pMeshNode[nRenderIndex]->m_nRenderCount==1)
//		m_pRenderer->ObjectRestoreData(nRenderIndex);
//	else if(m_pRenderer->m_pMeshNode[nRenderIndex]->m_nRenderCount==2)
//		m_pRenderer->m_pMeshNode[nRenderIndex]->m_nRenderCount++;
//////////////////////////
	float fRadius = 0;
	if(m_pObjMesh)
	{
		fRadius = m_pObjMesh->m_fRadius;
	}
	m_vShadowPos1 = m_vPos+fRadius*(vVel-vSide);	
	m_vShadowPos2 = m_vPos+fRadius*(vVel+vSide);
	m_vShadowPos3 = m_vPos+fRadius*vSide;
	m_vShadowPos4 = m_vPos-fRadius*vSide;

	FLOAT fDist;
	int i,j;
	i = (int)(m_vShadowPos1.x/TILE_SIZE);
	j = (int)(m_vShadowPos1.z/TILE_SIZE);
	D3DXVECTOR3 v,orig,dir;//,v2;
	orig = m_vShadowPos1;
	dir = D3DXVECTOR3(0,-1,0);
	if(i >= 0 && i < g_pGround->m_projectInfo.sXSize && j >= 0 && j < g_pGround->m_projectInfo.sYSize)
	{
		if(g_pGround->IntersectTriangle(orig,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j].pos,
			g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
			&fDist))//, &fBary1, &fBary2 ))
		{
			m_vShadowPos1 = orig + dir*fDist;
		}
		else if(g_pGround->IntersectTriangle(orig,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
			&fDist))//, &fBary1, &fBary2 ))
		{
			m_vShadowPos1 = orig + dir*fDist;
		}
	}
	i = (int)(m_vShadowPos2.x/TILE_SIZE);
	j = (int)(m_vShadowPos2.z/TILE_SIZE);
	orig = m_vShadowPos2;
	dir = D3DXVECTOR3(0,-1,0);
	if(i >= 0 && i < g_pGround->m_projectInfo.sXSize && j >= 0 && j < g_pGround->m_projectInfo.sYSize)
	{
		if(g_pGround->IntersectTriangle(orig,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j].pos,
			g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
			&fDist))//, &fBary1, &fBary2 ))
		{
			m_vShadowPos2 = orig + dir*fDist;
		}
		else if(g_pGround->IntersectTriangle(orig,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
			&fDist))//, &fBary1, &fBary2 ))
		{
			m_vShadowPos2 = orig + dir*fDist;
		}
	}
	i = (int)(m_vShadowPos3.x/TILE_SIZE);
	j = (int)(m_vShadowPos3.z/TILE_SIZE);
	orig = m_vShadowPos3;
	dir = D3DXVECTOR3(0,-1,0);
	if(i >= 0 && i < g_pGround->m_projectInfo.sXSize && j >= 0 && j < g_pGround->m_projectInfo.sYSize)
	{
		if(g_pGround->IntersectTriangle(orig,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j].pos,
			g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
			&fDist))//, &fBary1, &fBary2 ))
		{
			m_vShadowPos3 = orig + dir*fDist;
		}
		else if(g_pGround->IntersectTriangle(orig,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
			&fDist))//, &fBary1, &fBary2 ))
		{
			m_vShadowPos3 = orig + dir*fDist;
		}
	}
	i = (int)(m_vShadowPos4.x/TILE_SIZE);
	j = (int)(m_vShadowPos4.z/TILE_SIZE);
	orig = m_vShadowPos4;
	dir = D3DXVECTOR3(0,-1,0);
	if(i >= 0 && i < g_pGround->m_projectInfo.sXSize && j >= 0 && j < g_pGround->m_projectInfo.sYSize)
	{
		if(g_pGround->IntersectTriangle(orig,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j].pos,
			g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
			&fDist))//, &fBary1, &fBary2 ))
		{
			m_vShadowPos4 = orig + dir*fDist;
		}
		else if(g_pGround->IntersectTriangle(orig,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
			g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
			&fDist))//, &fBary1, &fBary2 ))
		{
			m_vShadowPos4 = orig + dir*fDist;
		}
	}
}

void CObjectChild::LoadCharacterEffect(char* strFileName)
{
	FLOG( "CObjectChild::LoadCharacterEffect(char* strFileName)" );
	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->InvalidateDeviceObjects();
		m_pCharacterInfo->DeleteDeviceObjects();
		util::del(m_pCharacterInfo);
	}
	m_pCharacterInfo = new CCharacterInfo();
	if(m_pCharacterInfo->Load(strFileName))
	{
		m_pCharacterInfo->InitDeviceObjects();
		m_pCharacterInfo->RestoreDeviceObjects();
	}
	else
	{
		util::del(m_pCharacterInfo);
//		DBGOUT("CObjectChild::LoadCharacterEffect,ĳ���� ������ �����ϴ�.\n");
	}
}

void CObjectChild::ChangeBodycondition( BodyCond_t bodycon )
{
	// 2008-07-11 by dgwoo ��������Ʈ ����Ʈ �ȳ����� ���� ����.
	if(m_nCode == WARP_GATE_OBJECT_NUM_2 ||
		m_nCode == WARP_GATE_OBJECT_NUM		||
		m_nCode == WARP_GATE_OBJECT_NUM_3)// 2012-06-13 by jhahn ���� ����Ʈ �߰�2.
	{
		m_pCharacterInfo->ResetBodyCondition(bodycon);
		m_BodyCondition = bodycon;
		//DBGOUT("WarpGate(%d) : bodycondition(%x)\n",m_nCode, bodycon);
		return;
	}
	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->ChangeBodyCondition( m_BodyCondition | bodycon );
		m_BodyCondition |= bodycon;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void ChangeBodyconditionEvent(BodyCond_t bodycon)
/// \brief		�ٵ�������� �����ؾ� �ϴ� ������Ʈ�� ���.
/// \author		dgwoo
/// \date		2007-04-23 ~ 2007-04-23
/// \warning	single bodycondition �� ���.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CObjectChild::ChangeBodyconditionEvent(BodyCond_t bodycon)
{
	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->ChangeBodyCondition(bodycon);
		m_BodyCondition = bodycon;
	}
}