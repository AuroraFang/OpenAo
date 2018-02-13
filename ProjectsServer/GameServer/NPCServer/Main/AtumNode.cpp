// AtumNode.cpp: implementation of the CAtumNode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumNode.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAtumNode::CAtumNode()
{
	m_pParent = NULL;
    m_pNext = NULL;
    m_pPrev = NULL;
	m_pChild = NULL;
	m_bUsing = TRUE;
}

CAtumNode::CAtumNode(CAtumNode* pParent)
{
	m_pParent = pParent;
    m_pNext = NULL;
    m_pPrev = NULL;
	m_pChild = NULL;
	m_bUsing = TRUE;
}

//
// ~CCAtumNode - �Ҹ���
//
CAtumNode::~CAtumNode()
{
	CAtumNode* pChild = m_pChild;
	CAtumNode* pNext;
	while ( pChild ) {
		pNext = pChild->m_pNext;
		delete pChild;
		pChild = pNext;
	}
}

//
//  InitDeviceObjects - ����̽���ü �ʱ�ȭ
//
HRESULT CAtumNode::InitDeviceObjects()
{
	// ���ϵ��� ���� ȣ��
	if ( m_pChild ) 
		for( CAtumNode* pAtumNode = m_pChild; pAtumNode; pAtumNode = pAtumNode->m_pNext )
			pAtumNode->InitDeviceObjects();

	return S_OK;
}


//
//  RestoreDeviceObjects - ����̽� ��ü ���
//
HRESULT CAtumNode::RestoreDeviceObjects()
{
	// ���ϵ��� ���� ȣ��
	if ( m_pChild ) 
		for( CAtumNode* pAtumNode = m_pChild; pAtumNode; pAtumNode = pAtumNode->m_pNext )
			pAtumNode->RestoreDeviceObjects();

		return S_OK;
}

//
//  InvalidateDeviceObjects - ����̽� ��ȿȭ
//
HRESULT CAtumNode::InvalidateDeviceObjects()
{
	// ���ϵ��� ���� ȣ��
	if ( m_pChild ) 
		for( CAtumNode* pAtumNode = m_pChild; pAtumNode; pAtumNode = pAtumNode->m_pNext )
			pAtumNode->InvalidateDeviceObjects();

		return S_OK;
}

//
//  DeleteDeviceObjects - ����̽���ü ����
//
HRESULT CAtumNode::DeleteDeviceObjects()
{
	// ���ϵ��� ���� ȣ��
	if ( m_pChild ) 
		for( CAtumNode* pAtumNode = m_pChild; pAtumNode; pAtumNode = pAtumNode->m_pNext )
			pAtumNode->DeleteDeviceObjects();

		return S_OK;
}

//
//  Tick - ����
//
void CAtumNode::Tick()
{
	// ���ϵ��� ���� ȣ��
	CAtumNode* pAtumNode = m_pChild;
	CAtumNode* pNext;
	while( pAtumNode ) 
	{
		if(pAtumNode->m_bUsing)
			pAtumNode->Tick();
		pNext = pAtumNode->m_pNext;
		if(!pAtumNode->m_bUsing){
			if(pAtumNode->m_pPrev)
				pAtumNode->m_pPrev->m_pNext = pAtumNode->m_pNext;
			else m_pChild = pAtumNode->m_pNext;
			if(pAtumNode->m_pNext)
				pAtumNode->m_pNext->m_pPrev = pAtumNode->m_pPrev;
//			if(pAtumNode->m_dwPartType == 100 && (((CAppEffectData *)pAtumNode)->m_nType/1000000 == 7 || (((CAppEffectData *)pAtumNode)->m_nType/10000000 == 1)))
//			{
//				DBGOUT("CAtumNode::Tick(), Effect Delete Node : [type : %d]\n",((CAppEffectData *)pAtumNode)->m_nType,((CAppEffectData *)pAtumNode)->m_bodyCondition);
//			}
			delete pAtumNode;
		}
		pAtumNode = pNext;
	}
}

//
//  Render - ������
//
void CAtumNode::Render()
{
	// ���ϵ��� ���� ȣ��
	if ( m_pChild ) 
		for( CAtumNode* pAtumNode = m_pChild; pAtumNode; pAtumNode = pAtumNode->m_pNext )
			pAtumNode->Render();
}

//
//  AddChild - ���ϵ带 �߰�
//

CAtumNode* CAtumNode::AddChild( CAtumNode* pAtumNode )
{
  // ���ϵ尡 ������ �װ��� �տ��� �����ִ´�.
	if(pAtumNode->m_bUsing)
	{
		CAtumNode * pNode = m_pChild;
		m_pChild = pAtumNode;
		if(pNode)
		{
			m_pChild->m_pNext = pNode;
			pNode->m_pPrev = m_pChild;
		}
	}
	else
	{
		//DBGOUT("ERROR CAtumNode::AddChild( CAtumNode* pAtumNode ), pAtumNode->m_bUsing == FALSE )\n");
		delete pAtumNode; 
		pAtumNode = NULL;
	}
	return pAtumNode;
}

void CAtumNode::DeleteChild( CAtumNode* pAtumNode )
{
	CAtumNode * pChild = m_pChild;
	while(pChild)
	{
		if(pAtumNode == pChild)
		{
			if(pChild->m_pPrev)
				pChild->m_pPrev->m_pNext = pChild->m_pNext;
			else if(pChild->m_pNext)
			{
				m_pChild = pChild->m_pNext;
				pChild->m_pNext->m_pPrev = NULL;
			}
			else m_pChild = NULL;
			delete pChild;
			return;
		}
		pChild = pChild->m_pNext;
	}
}
