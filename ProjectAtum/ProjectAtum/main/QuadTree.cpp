// QuadTree.cpp: implementation of the CQuadTree class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuadTree.h"
#include "dxutil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// \fn			CQuadTree
/// \brief		QuadTree ������
/// \date		2004-03-15 ~ 2004-03-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CQuadTree::CQuadTree()
{
	FLOG("CQuadTree::CQuadTree()");
	for ( int nCheckChild = 0;nCheckChild < QUAD_CHILD_NUM;nCheckChild++ )
	{
		m_pChild[nCheckChild] = NULL;				
		m_vPos[nCheckChild] = D3DXVECTOR3( 0, 0, 0 );
	}
	m_pParent = NULL;					
	m_pOriParent = NULL;
	m_eCornerType = MONE;				
	m_nDegree = 0;						
	m_nMaxSize = 0;
	m_sStartX = 0;						
	m_sStartY = 0;						
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CQuadTree
/// \brief		QuadTree ������
/// \date		2004-03-15 ~ 2004-03-15
/// \warning	
///
/// \param		CQuadTree * pParent, CornerType nType, short sX, short sY, int nMax
/// \return		
///////////////////////////////////////////////////////////////////////////////
CQuadTree::CQuadTree(CQuadTree * pParent
					 , CornerType nType
					 , short sX
					 , short sY
					 , int nMax)
{
	FLOG("CQuadTree::CQuadTree(CQuadTree * pParent,CornerType nType,short sX, short sY,int nMax)");
	for(int nCheckChild = 0;nCheckChild < QUAD_CHILD_NUM;nCheckChild++)
	{
		m_pChild[nCheckChild] = NULL;				
		m_vPos[nCheckChild] = D3DXVECTOR3( 0, 0, 0 );
	}
	m_pParent = pParent;				
	m_eCornerType = nType;				
	m_nMaxSize = nMax;
	if(m_pParent)
	{
		m_nDegree = m_pParent->m_nDegree + 1;
	}
	else
	{
		m_nDegree = 0;
	}
	m_sStartX = sX;
	m_sStartY = sY;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			~CQuadTree
/// \brief		QuadTree �Ҹ���
/// \date		2004-03-15 ~ 2004-03-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CQuadTree::~CQuadTree()
{
	FLOG("CQuadTree::~CQuadTree()");
	for ( int nCheckChild = 0;nCheckChild < QUAD_CHILD_NUM;nCheckChild++ )
	{
		CQuadTree* pChild = m_pChild[nCheckChild];
		if ( pChild )
		{
			delete pChild;
			pChild = NULL;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			InitDeviceObjects
/// \brief		�ڽ� ��尡 ������ Init Device
/// \date		2004-03-15 ~ 2004-03-15
/// \warning	
///
/// \param		
/// \return		HRESULT
///////////////////////////////////////////////////////////////////////////////
HRESULT CQuadTree::InitDeviceObjects()
{
	FLOG("CQuadTree::InitDeviceObjects()");
	for ( int nCheckChild = 0;nCheckChild < QUAD_CHILD_NUM;nCheckChild++ )
	{
		if ( m_pChild[nCheckChild] )
		{
			m_pChild[nCheckChild]->InitDeviceObjects();
		}
	}
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			RestoreDeviceObjects
/// \brief		�ڽ� ��尡 ������ Restore Device
/// \date		2004-03-15 ~ 2004-03-15
/// \warning	
///
/// \param		
/// \return		HRESULT
///////////////////////////////////////////////////////////////////////////////
HRESULT CQuadTree::RestoreDeviceObjects()
{
	FLOG("CQuadTree::RestoreDeviceObjects()");
	for ( int nCheckChild = 0;nCheckChild < QUAD_CHILD_NUM;nCheckChild++ )
	{
		if ( m_pChild[nCheckChild] )
		{
			m_pChild[nCheckChild]->RestoreDeviceObjects();
		}
	}
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			InvalidateDeviceObjects
/// \brief		�ڽ� ��尡 ������ Invalidate Device
/// \date		2004-03-15 ~ 2004-03-15
/// \warning	
///
/// \param		
/// \return		HRESULT
///////////////////////////////////////////////////////////////////////////////
HRESULT CQuadTree::InvalidateDeviceObjects()
{
	FLOG("CQuadTree::InvalidateDeviceObjects()");
	for ( int nCheckChild = 0;nCheckChild < QUAD_CHILD_NUM;nCheckChild++ )
	{
		if ( m_pChild[nCheckChild] )
		{
			m_pChild[nCheckChild]->InvalidateDeviceObjects();
		}
	}
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			DeleteDeviceObjects
/// \brief		�ڽ� ��尡 ������ Delete Device
/// \date		2004-03-15 ~ 2004-03-15
/// \warning	
///
/// \param		
/// \return		HRESULT
///////////////////////////////////////////////////////////////////////////////
HRESULT CQuadTree::DeleteDeviceObjects()
{
	FLOG("CQuadTree::DeleteDeviceObjects()");
	for ( int nCheckChild = 0;nCheckChild < QUAD_CHILD_NUM;nCheckChild++ )
	{
		if ( m_pChild[nCheckChild] )
		{
			m_pChild[nCheckChild]->DeleteDeviceObjects();
			util::del( m_pChild[nCheckChild] );
		}
	}
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			Tick
/// \brief		�ڽ� ��尡 ������ Frame Move
/// \date		2004-03-15 ~ 2004-03-15
/// \warning	
///
/// \param		
/// \return		void
///////////////////////////////////////////////////////////////////////////////
void CQuadTree::Tick()
{
	FLOG("CQuadTree::Tick()");
	for ( int nCheckChild = 0;nCheckChild < QUAD_CHILD_NUM;nCheckChild++ )
	{
		if ( m_pChild[nCheckChild] )
		{
			m_pChild[nCheckChild]->Tick();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			Render
/// \brief		�ڽ� ��尡 ������ Rendering
/// \date		2004-03-15 ~ 2004-03-15
/// \warning	
///
/// \param		
/// \return		void
///////////////////////////////////////////////////////////////////////////////
void CQuadTree::Render()
{
	FLOG("CQuadTree::Render()");
	for ( int nCheckChild = 0;nCheckChild < QUAD_CHILD_NUM;nCheckChild++ )
	{
		if ( m_pChild[nCheckChild] )
		{
			m_pChild[nCheckChild]->Render();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			AddChild
/// \brief		�ڽ� ��� �߰�
/// \date		2004-03-13 ~ 2004-03-13
/// \warning	
///
/// \param		�ڽ� ���� �߰��� ����� ������
/// \return		void
///////////////////////////////////////////////////////////////////////////////
void CQuadTree::AddChild(CQuadTree * pNode)
{
	FLOG("CQuadTree::AddChild(CQuadTree * pNode)");
	m_pChild[pNode->m_eCornerType] = pNode;
}

