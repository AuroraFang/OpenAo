// MyCheck.cpp : implementation file
//

#include "stdafx.h"
#include "atumlauncher.h"
#include "MyCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyCheck

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyCheck::CMyCheck
/// \brief	: ������
/// \author	: CKPark
/// \date	: 5:12:2008   11:40
/// \warning: 
/// \return	: 
///////////////////////////////////////////////////////////////////////////////
CMyCheck::CMyCheck() : CButton(), m_bMyCheckState(FALSE)
{

}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyCheck::~CMyCheck
/// \brief	: �ı��� üũ/��üũ�� ����� ��Ʈ�� ����
/// \author	: CKPark
/// \date	: 5:12:2008   11:40
/// \warning: 
/// \return	: 
///////////////////////////////////////////////////////////////////////////////
CMyCheck::~CMyCheck()
{
	for(int i=0; i<CHECKBOX_TOTAL_STATE; ++i)
	{
		if(m_CheckStateBMP[i].m_hObject)
			m_CheckStateBMP[i].DeleteObject();
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyCheck::GetCheck
/// \brief	: üũ���� ������
/// \author	: CKPark
/// \date	: 5:12:2008   11:41
/// \warning: 
/// \return	: int
///////////////////////////////////////////////////////////////////////////////
int		CMyCheck::GetCheck()
{
	return m_bMyCheckState;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyCheck::SetCheck
/// \brief	: üũ���� ����
/// \author	: CKPark
/// \date	: 5:12:2008   11:41
/// \warning: !nCheck�� �����ϴ� ������ SetCheckȣ��� OnChange�� �ڵ����� ȣ��
//			  �Ǿ� ������ �ٽ� ������� �ٲٱ� ���ؼ��̴�
/// \param	: int nCheck
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void	CMyCheck::SetCheck(int nCheck)
{
	m_bMyCheckState = !nCheck;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyCheck::SetBitMap
/// \brief	: üũ, ��üũ�� ��Ʈ���� ���� �Ѵ�
/// \author	: CKPark
/// \date	: 5:12:2008   11:42
/// \warning: 
/// \param	: const int nUnCheckedBMPID
/// \param	: const int nCheckedBMPID
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void	CMyCheck::SetBitMap(const int nUnCheckedBMPID, const int nCheckedBMPID)
{
	m_CheckStateBMP[CHECKBOX_STATE_UNCHECKED].LoadBitmap(nUnCheckedBMPID);
	m_CheckStateBMP[CHECKBOX_STATE_CHECKED].LoadBitmap(nCheckedBMPID);
}


BEGIN_MESSAGE_MAP(CMyCheck, CButton)
 	//{{AFX_MSG_MAP(CMyCheck)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		ON_WM_MOUSEMOVE()
		ON_WM_PAINT()
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
		ON_WM_LBUTTONDBLCLK()
		ON_WM_ENABLE()
		ON_MESSAGE(BM_SETCHECK, OnChange)
 	//}}AFX_MSG_MAP
 END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyCheck message handlers


///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyCheck::OnPaint
/// \brief	: ����Ʈ �Լ�
/// \author	: CKPark
/// \date	: 5:12:2008   11:42
/// \warning: CButton::OnPaint()�� ȣ����ϴ� ��� CPaintDC dc(this)�� �����Ѵ�
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void CMyCheck::OnPaint()
{
	CPaintDC		dc(this);

	// ���� üũ/��üũ ��Ʈ���� �ε������� ��츸 ��Ʈ������ �׸���
	if(m_CheckStateBMP[CHECKBOX_STATE_UNCHECKED].m_hObject &&
	   m_CheckStateBMP[CHECKBOX_STATE_CHECKED].m_hObject)
	{
		CDC*		pDC	= GetDC();
		if(!pDC)
			return;

		// �Ÿ� dc����
		CDC			memDC;
		memDC.CreateCompatibleDC(NULL);
		
		
		// üũ/��üũ�� ���� ��Ʈ�� ����
		CBitmap*	pOld;
		if(m_bMyCheckState)
			pOld = memDC.SelectObject(&m_CheckStateBMP[CHECKBOX_STATE_CHECKED]);
		else
			pOld = memDC.SelectObject(&m_CheckStateBMP[CHECKBOX_STATE_UNCHECKED]);

		// Ŭ���̾�Ʈ ����
		CRect		r;
		GetClientRect(r);

 		// dc�� �޸� dc���� ����
		pDC->BitBlt(0, 0, r.Width(), r.Height(),
					&memDC, 0, 0, SRCCOPY);

		// ����� dc ����
		memDC.SelectObject(pOld);
		memDC.DeleteDC();
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyCheck::OnChange
/// \brief	: üũ, ��üũ�� �ٲ� ȣ���, m_bMyCheckState ����, ���� ��ȿȭ
/// \author	: CKPark
/// \date	: 5:12:2008   11:43
/// \warning: 
/// \param	: WPARAM wParam
/// \param	: LPARAM lParam
/// \return	: LRESULT
///////////////////////////////////////////////////////////////////////////////
LRESULT CMyCheck::OnChange(WPARAM wParam, LPARAM lParam) 
{
	m_bMyCheckState = !m_bMyCheckState;
	Invalidate();
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyCheck::OnMouseMove
/// \brief	: ���콺 �����϶� ���� ��ȿȭ
/// \author	: CKPark
/// \date	: 5:12:2008   11:44
/// \warning: 
/// \param	: UINT nFlags
/// \param	: CPoint point
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void CMyCheck::OnMouseMove(UINT nFlags, CPoint point)
{
	Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyCheck::OnLButtonDown
/// \brief	: ���콺 �ٿ�ɶ� ���� ��ȿȭ
/// \author	: CKPark
/// \date	: 5:12:2008   11:45
/// \warning: CButton::OnLButtonDown�� ȣ����ϸ� OnChange�� ȣ����� �ʴ´�
/// \param	: UINT nFlags
/// \param	: CPoint point
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void CMyCheck::OnLButtonDown(UINT nFlags, CPoint point)
{
	CButton::OnLButtonDown(nFlags, point);
	Invalidate();	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyCheck::OnLButtonUp
/// \brief	: ���콺 ���ɶ� ���� ��ȿȭ
/// \author	: CKPark
/// \date	: 5:12:2008   11:45
/// \warning: CButton::OnLButtonUp�� ȣ����ϸ� OnChange�� ȣ����� �ʴ´�
/// \param	: UINT nFlags
/// \param	: CPoint point
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void CMyCheck::OnLButtonUp(UINT nFlags, CPoint point)
{
	CButton::OnLButtonUp(nFlags, point);
	Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyCheck::OnLButtonDblClk
/// \brief	: ����Ŭ���� ���� ��ȿȭ
/// \author	: CKPark
/// \date	: 5:12:2008   11:46
/// \warning: 
/// \param	: UINT nFlags
/// \param	: CPoint point
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void CMyCheck::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyCheck::OnEnable
/// \brief	: ��Ʈ���� Ȱ��ȭ �ɶ� ���� ��ȿȭ
/// \author	: CKPark
/// \date	: 5:12:2008   11:46
/// \warning: 
/// \param	: BOOL bEnable
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void CMyCheck::OnEnable(BOOL bEnable)
{
	Invalidate();
}