// AtumNode.cpp: implementation of the CAtumNode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumNode.h"
#include "AppEffectData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAtumNode::CAtumNode(CAtumNode* pParent)
{
	m_pParent = pParent;
    m_pNext = nullptr;
    m_pPrev = nullptr;
	m_pChild = nullptr;
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
			pAtumNode->InvalidateDeviceObjects(); // 2016-01-08 exception at this point

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
	auto pAtumNode = m_pChild;

	while (pAtumNode)
	{
		auto pNext = pAtumNode->m_pNext;

		if (!pAtumNode->m_bUsing)
		{
			if (pAtumNode->m_pPrev)
				pAtumNode->m_pPrev->m_pNext = pAtumNode->m_pNext;

			else m_pChild = pAtumNode->m_pNext;

			if (pAtumNode->m_pNext)
				pAtumNode->m_pNext->m_pPrev = pAtumNode->m_pPrev;

//			if(pAtumNode->m_dwPartType == 100 && (((CAppEffectData *)pAtumNode)->m_nType/1000000 == 7 || (((CAppEffectData *)pAtumNode)->m_nType/10000000 == 1)))
//			{
//				DBGOUT("CAtumNode::Tick(), Effect Delete Node : [type : %d]\n",((CAppEffectData *)pAtumNode)->m_nType,((CAppEffectData *)pAtumNode)->m_bodyCondition);
//			}

			delete pAtumNode;
		}
		else pAtumNode->Tick();

		pAtumNode = pNext;
	}
}

// use to improve bottlenecks
void CAtumNode::TickParallel()
{
	vector<CAtumNode*> temp;

	for (auto pAtumNode = m_pChild; pAtumNode; )
	{
		auto pNext = pAtumNode->m_pNext;

		if (!pAtumNode->m_bUsing)
		{
			if (pAtumNode->m_pPrev)
				pAtumNode->m_pPrev->m_pNext = pAtumNode->m_pNext;

			else m_pChild = pAtumNode->m_pNext;

			if (pAtumNode->m_pNext)
				pAtumNode->m_pNext->m_pPrev = pAtumNode->m_pPrev;

			delete pAtumNode;
		}
		else temp.push_back(pAtumNode);

		pAtumNode = pNext;
	}

	auto arr = temp.data();
	auto size = temp.size();

	if (size == 0) return;
#ifdef PARRALELLE_WEAPON_EFFET_PROCESSING
#pragma omp parallel for schedule(dynamic, PARRALELLE_WEAPON_EFFET_PROCESSING)
#else
#pragma omp parallel for schedule(dynamic, 8)
#endif
	for (auto i = 0; i < size; ++i) arr[i]->Tick();
}

//
//  Render - ������
//
void CAtumNode::Render()
{
	// ���ϵ��� ���� ȣ��
	if (m_pChild)
		for (auto pAtumNode = m_pChild; pAtumNode; pAtumNode = pAtumNode->m_pNext)
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
	// traverse 
	for (auto pChild = m_pChild; pChild; pChild = pChild->m_pNext)
	{
		// find child
		if (pAtumNode == pChild)
		{
			// remove it from list
			if (pChild->m_pPrev)

				pChild->m_pPrev->m_pNext = pChild->m_pNext;

			else if (pChild->m_pNext)
			{
				m_pChild = pChild->m_pNext;
				pChild->m_pNext->m_pPrev = nullptr;
			}

			else m_pChild = nullptr;

			// delete it
			delete pChild;

			// we are done
			break;
		}
	}
}
