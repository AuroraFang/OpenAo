// QuadTree.h: interface for the CQuadTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUADTREE_H__825B456F_BA9B_42CB_B29F_066BD1728E80__INCLUDED_)
#define AFX_QUADTREE_H__825B456F_BA9B_42CB_B29F_066BD1728E80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define QUAD_CHILD_NUM		4

enum CornerType{ MONE = -1,CORNER_TL,CORNER_TR,CORNER_BL,CORNER_BR};

///////////////////////////////////////////////////////////////////////////////
/// \class		QuadTree
///
/// \brief		���� Ʈ���� �ۼ��ϱ� ���� ��� Ŭ����
/// \version	1.0
/// \date		2004-03-13 ~ 2004-03-13
/// \warning	
///////////////////////////////////////////////////////////////////////////////

class CQuadTree  
{
public:
	CQuadTree();
	// �θ� ������, �ڳ� Ÿ��, X ��ǥ ���� ����ġ, Z ��ǥ ���� ����ġ, �ִ� ũ��
	CQuadTree(CQuadTree * pParent
		, CornerType nType
		, short sX
		, short sY
		, int nMax);
	virtual ~CQuadTree();
	virtual HRESULT InitDeviceObjects();		// Init	Device
	virtual HRESULT RestoreDeviceObjects();		// Restore Device
	virtual HRESULT InvalidateDeviceObjects();	// Invalidate Device
	virtual HRESULT DeleteDeviceObjects();		// Delete Device
	virtual void Render();						// Rendering
	virtual void Tick();						// FrameMove
	void AddChild(CQuadTree * pNode);			// �ڽ� ��� �߰�

public:
	CQuadTree		*m_pOriParent;				// ����Ÿ�� ��� �ִ� �θ�(VB,IB)
	CQuadTree		*m_pChild[QUAD_CHILD_NUM];	// �ڽ� ������	0~3 -> CornerType
	CQuadTree		*m_pParent;					// �θ� ������
	D3DXVECTOR3		m_vPos[4];					// ������ 4���� ������
	CornerType		m_eCornerType;				// �ڳ� Ÿ��
	int				m_nDegree;					// ��� ���� �ܰ�
	int				m_nMaxSize;					// �ִ� ũ��
	short			m_sStartX;					// ���� ��ġ x
	short			m_sStartY;					// ���� ��ġ y
};

#endif // !defined(AFX_QUADTREE_H__825B456F_BA9B_42CB_B29F_066BD1728E80__INCLUDED_)
