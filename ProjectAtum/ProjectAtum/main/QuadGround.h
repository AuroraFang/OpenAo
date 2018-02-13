// QuadGround.h: interface for the CQuadGround class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUADGROUND_H__BDB11296_38A9_4D09_8CBD_6AC3162A611A__INCLUDED_)
#define AFX_QUADGROUND_H__BDB11296_38A9_4D09_8CBD_6AC3162A611A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "QuadTree.h"

// 16��Ʈ �ε��� ������ ���ؽ� �ε����� ���� �� ����ü
struct MYINDEX
{
	WORD _0,_1,_2;
};

///////////////////////////////////////////////////////////////////////////////
/// \class		QuadGround
///
/// \brief		���� Ʈ���� ������ ���� ����Ÿ�� ���� Ŭ����
/// \version	1.0
/// \date		2004-03-13 ~ 2004-03-13
/// \warning	�ܰ迡 ���� ����Ÿ������� Ʋ����
///////////////////////////////////////////////////////////////////////////////
class CQuadGround : public CQuadTree  
{
public:
	CQuadGround( int nMax );
	// �θ� ������, �ڳ� Ÿ��, X ��ǥ ���� ����ġ, Z ��ǥ ���� ����ġ, �ִ� ũ��
	CQuadGround( CQuadTree * pParent,
		CornerType nType,
		short sX,
		short sY,
		int nMax );
	virtual ~CQuadGround();
	virtual HRESULT InitDeviceObjects();			// Init Device
	virtual HRESULT RestoreDeviceObjects();			// Restore Device
	virtual HRESULT InvalidateDeviceObjects();		// Invalidate Device
	virtual HRESULT DeleteDeviceObjects();			// Delete Device
	virtual void Tick();							// Frame Move
	virtual void Render();							// Rendering
	void SubDivide();								// ���� ��带 �����Ѵ�
	void SetIB();									// ������ �Ǿ��� ���� ����(IB)
	void GetMinMax(float fMax, float fMin);		// ���� �ִ�,�ּ�

	LPDIRECT3DTEXTURE9			m_pTexture;			// �ؽ���
	LPDIRECT3DVERTEXBUFFER9		m_pVBTest;			// ���ؽ� ����	
	LPDIRECT3DVERTEXBUFFER9		m_pVBTest1;			// ���ؽ� ���� Ÿ�� ��� �׽�Ʈ��
	LPDIRECT3DINDEXBUFFER9		m_pIBTest;			// �ε��� ����
	
	int							m_nSize;			// ���� ����� �� ������
	int							m_nTriangleNumber;	// ������ �Ǿ��� ���
	MYINDEX						*m_pIdx;			// ������ �Ǿ��� �ε��������� ��ġ�� �����ϱ� ���� ������
	vector<int>					m_vecRenderTileList;// ������ �Ǿ��� Ÿ�� �� ����Ʈ
	float						m_fRadius;			// �� ������ ������ (����ü �ø��� ���ؼ�.)
	
	int							m_nMaxHeight;		// �ִ� ����
};

#endif // !defined(AFX_QUADGROUND_H__BDB11296_38A9_4D09_8CBD_6AC3162A611A__INCLUDED_)
