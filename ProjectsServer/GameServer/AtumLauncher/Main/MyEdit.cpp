#include "StdAfx.h"
#include "MyEdit.h"

CMyEdit::CMyEdit() : CEdit()
{
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyEdit::~CMyEdit
/// \brief	: �ı��� ����Ʈ �ڽ��� ����� ��Ʈ�� ����
/// \author	: CKPark
/// \date	: 5:12:2008   12:00
/// \warning: 
/// \return	: 
///////////////////////////////////////////////////////////////////////////////
CMyEdit::~CMyEdit()
{
	if(m_Bmp.m_hObject)
		m_Bmp.DeleteObject();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyEdit::SetBitMap
/// \brief	: ����Ʈ �ڽ��� ����� ��Ʈ�� ����
/// \author	: CKPark
/// \date	: 5:12:2008   12:01
/// \warning: 
/// \param	: const int nBitMapID
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void	CMyEdit::SetBitMap(const int nBitMapID)
{
	m_Bmp.LoadBitmap(nBitMapID);
}

BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
//{{AFX_MSG_MAP(CMyEdit)
ON_WM_PAINT()
ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
ON_CONTROL_REFLECT(EN_SETFOCUS, OnSetfocus)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyEdit message handlers

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyEdit::OnPaint
/// \brief	: ������ȿȭ�� ������
/// \author	: CKPark
/// \date	: 5:12:2008   12:01
/// \warning: CEdit::OnPaint()�� �������� ���� �� ȣ���Ѵ�. �׷��� �ʴٸ�
///			  ���� �ؽ�Ʈ�� �׷���� �Ѵ�
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void CMyEdit::OnPaint() 
{
	// ��Ʈ���� �ε����� ��츸 �׷���
	// ��Ʈ���� �ε����� �ʾ��� ���
	// CEdit::OnPaint()�� ���� �⺻ ����Ʈ �ڽ��� �׷�����
	if( m_Bmp.m_hObject)
	{
		CDC			*pDC	= GetDC();
		if(!pDC)
			return;

		// �޸� dc ����
		CDC			memDC;
		memDC.CreateCompatibleDC(pDC);

		// �޸� dc ��Ʈ�� ����
		CBitmap *pOld	= memDC.SelectObject(&m_Bmp);
		
		// Ŭ���̾�Ʈ ����
		CRect		r;
		GetClientRect(r);

		// ��Ʈ�� ���
		BITMAP bmp;
		m_Bmp.GetObject(sizeof(bmp), &bmp);

		// -3, -3, 5, 5�� ������ ������ ������
		pDC->StretchBlt(-3, -3, r.right + 5, r.bottom + 5,
						&memDC, 0, 0,
						bmp.bmWidth, bmp.bmHeight, SRCCOPY);

		// �޸� dc ����
		memDC.SelectObject(pOld);
		memDC.DeleteDC();
	}

	CEdit::OnPaint();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyEdit::OnChange
/// \brief	: ����Ʈ �ڽ� ������ �ٲ���� ��� ���� ��ȿȭ
/// \author	: CKPark
/// \date	: 5:12:2008   12:04
/// \warning: 
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void CMyEdit::OnChange() 
{
	Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyEdit::OnSetfocus
/// \brief	: ��Ŀ���� ����� �� ���� ��ȿȭ
/// \author	: CKPark
/// \date	: 5:12:2008   12:04
/// \warning: 
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void CMyEdit::OnSetfocus() 
{
	Invalidate();
}