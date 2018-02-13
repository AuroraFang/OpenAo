#if !defined(AFX_SCITEMEVENTDLG_H__F920BD39_1EE4_477C_941D_F963ABFE0BBB__INCLUDED_)
#define AFX_SCITEMEVENTDLG_H__F920BD39_1EE4_477C_941D_F963ABFE0BBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SCItemEventDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSCItemEventDlg dialog
#include "GridCtrl/GridCtrl.h" //For GridCtrl
#include "ODBCStatement.h" //For DB Access
#include "AtumDateTime.h"

typedef mt_vector<SITEM_EVENT_INFO>		mtvectITEM_EVENT_INFO;

class CAtumAdminToolDlg;

class CSCItemEventDlg : public CDialog
{
// Construction
public:
	SITEM_EVENT_INFO * FindItemEvent(int i_nItemEvUID);
	CSCItemEventDlg(CWnd* pParent = NULL);   // standard constructor
	~CSCItemEventDlg();
	void		GetItemEventInfo();	//DB �� ������
	void		InitGrid();			//�׸��� �ʱ� ����

	ez_map<INT, ITEM>		m_mapItemInfo;
	ez_map<string, ITEM>	m_mapItemInfoName;
	ez_map<INT, RARE_ITEM_INFO>		m_mapRareItemInfo;
	ez_map<string, RARE_ITEM_INFO>	m_mapRareItemInfoName;

	CGridCtrl					m_GridList;
	mtvectITEM_EVENT_INFO		m_mtvectITEM_EVENT_INFO;

	CODBCStatement				*m_pODBCStmt; //DB ���� Ŭ���� ������
	CAtumAdminToolDlg			*m_pMainDlg; //AtumAdminToll ���̾�α� Ŭ����

	// 2008-01-11 by cmkwon, ������ �̺�Ʈ �ý��ۿ� �� ���� �ý��� �߰� - ���� �̺�Ʈ�� �ϳ��� ������ �����ϴ�.
	BOOL CheckEnbaleInsertCouponEvent(int i_nItemEvTy, int i_nExcludeItemEventUID=0);

// Dialog Data
	//{{AFX_DATA(CSCItemEventDlg)
	enum { IDD = IDD_DIALOG_ITEM_EVENT_ADMIN };
	CComboBox	m_comboServerGroupID;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSCItemEventDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL InsetSubTypeInfo(SITEM_EVENT_INFO i_SITEM_EVENT_INFO);	// 2013-03-29 by jhseol, ������ �̺�Ʈ - ����Ÿ�� �߰������� ����

	// Generated message map functions
	//{{AFX_MSG(CSCItemEventDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnConmenuSelectedInsert();
	afx_msg void OnConmenuNotselectedInsert();
	afx_msg void OnConmenuSelectedUpdate();
	afx_msg void OnConmenuSelectedDelete();
	afx_msg void OnItemEventAdminToolButtonOk();
	afx_msg void OnReloadItemEventButton();
	afx_msg void OnLuckyMachineInitButton();
	//}}AFX_MSG
	afx_msg void OnGridRClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCITEMEVENTDLG_H__F920BD39_1EE4_477C_941D_F963ABFE0BBB__INCLUDED_)
