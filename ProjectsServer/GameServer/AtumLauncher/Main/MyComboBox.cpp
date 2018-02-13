// MyComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "atumlauncher.h"
#include "MyComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyComboBox

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyComboBox::CMyComboBox
/// \brief	: ������ �⺻ ���� �����Ѵ�
/// \author	: CKPark
/// \date	: 5:12:2008   11:50
/// \warning: 
/// \return	: 
///////////////////////////////////////////////////////////////////////////////
CMyComboBox::CMyComboBox() : CComboBox()
{
	m_TextColor = RGB(254, 254, 254);
	m_BackColor = RGB(0, 0, 0);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyComboBox::~CMyComboBox
/// \brief	: �ı��� ����� ��Ʈ�� ����
/// \author	: CKPark
/// \date	: 5:12:2008   11:51
/// \warning: 
/// \return	: 
///////////////////////////////////////////////////////////////////////////////
CMyComboBox::~CMyComboBox()
{
	if(m_EditBitMap.m_hObject)
		m_EditBitMap.DeleteObject();
}


BEGIN_MESSAGE_MAP(CMyComboBox, CComboBox)
	//{{AFX_MSG_MAP(CMyComboBox)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_WM_PAINT()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyComboBox message handlers

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyComboBox::OnSelchange
/// \brief	: ������ �ٲ���� ��� ���� ��ȿȭ
/// \author	: CKPark
/// \date	: 5:12:2008   11:51
/// \warning: 
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void CMyComboBox::OnSelchange() 
{
	// TODO: Add your control notification handler code here
	Invalidate(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyComboBox::OnPaint
/// \brief	: ���� ��ȿȭ�� ������
/// \author	: CKPark
/// \date	: 5:12:2008   11:52
/// \warning: CComboBox::OnPaint()�� ȣ����ϴ� ��� CPaintDC dc(this)�� ����
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void CMyComboBox::OnPaint()
{	
	CPaintDC	dc(this);
	
	CDC*	pDC		= GetDC();
	CDC		memDC;
	
	if(!pDC)
		return;

	// ��Ʈ�� �ε��� �ȵ������� �ƹ��͵� �ȱ׸���
	if(!m_EditBitMap.m_hObject)
		return;

	// �޸� dc ����
	memDC.CreateCompatibleDC(NULL);

	// Ŭ���̾�Ʈ ����
	CRect	r;
	GetClientRect(&r);

	// �޸� dc�� ��Ʈ�� ����
	CBitmap*	pMemDCOld;
	pMemDCOld = memDC.SelectObject(&m_EditBitMap);

	// dc�� �޸� dc���� �����
	pDC->BitBlt(0, 0, r.Width(), r.Height(), &memDC, 0, 0, SRCCOPY);

	// �޸� dc ����
	memDC.SelectObject(pMemDCOld);
	memDC.DeleteDC();

	// ���� ���õ� �������� �׷���� �ϹǷ�
	// ���õ� ���ؽ� ����
	int		nIndex = GetCurSel();
	// �ƹ��͵� ������ �ȵ� ���°� �ƴ� ���
	if( nIndex != -1)
	{
		// ���� ����
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_TextColor);

		// ���ڸ� �׷��ش�(r.left + 3, r.top + 4 �� ������ ��ġ�� ���� ���̴�)
		// textRect.right - 20�� ������ ȭ��ǥ�� ©���� ������ ���̴�.
		CRect	textRect = r;
		textRect.left	+= 3;
		textRect.top	+= 4;
		textRect.right	-= 20;

		pDC->DrawText(reinterpret_cast<char*>( GetItemData(nIndex) ),
					  &textRect,
					  DT_LEFT | DT_VCENTER);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyComboBox::DrawItem
/// \brief	: ����Ʈ�ڽ� ������ �׸��� ȣ��
/// \author	: CKPark
/// \date	: 5:12:2008   11:56
/// \warning: ���� ���õ� �������� OnPaint()���� �׷��ֹǷ� �ǳʶڴ�
/// \param	: LPDRAWITEMSTRUCT lpDrawItemStruct
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void CMyComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	if(!pDC)
		return;

	// ���� ���õ� �������� OnPaint()���� �׷��ֹǷ� �ǳʶڴ�
	if(lpDrawItemStruct->itemState & ODS_COMBOBOXEDIT)
		return;

	// Ŭ���̾�Ʈ ����
	CRect r = lpDrawItemStruct->rcItem;

	// ��׶��� �÷��� ������ ä���
	pDC->FillSolidRect(r.left, r.top, r.Width(), r.Height(), m_BackColor);
	
	// �������� �׸���
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_TextColor);
	pDC->DrawText(reinterpret_cast<char*>(lpDrawItemStruct->itemData), &r, DT_LEFT | DT_VCENTER);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyComboBox::OnKillFocus
/// \brief	: ��Ŀ�� ���� �� ���� ��ȿȭ�� �ٽ� �׸���
/// \author	: CKPark
/// \date	: 5:12:2008   11:57
/// \warning: 
/// \param	: CWnd * pNewWnd
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void CMyComboBox::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);
	// TODO: Add your message handler code here

	Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyComboBox::SetBitMap
/// \brief	: ����Ʈ �ڽ��� �� ��Ʈ�� ����
/// \author	: CKPark
/// \date	: 5:12:2008   11:58
/// \warning: 
/// \param	: const int nEditBitMapID
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void CMyComboBox::SetBitMap(const int nEditBitMapID)
{
	m_EditBitMap.LoadBitmap(nEditBitMapID);
}

void CMyComboBox::SetTextColor(const COLORREF dwTextColor)
{
	m_TextColor	= dwTextColor;
}

void CMyComboBox::SetBackColor(const COLORREF dwBackColor)
{
	m_BackColor = dwBackColor;
}
