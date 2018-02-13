#if !defined(AFX_STAISTICSMONEYDLG_H__C5289DF8_D11D_41B7_813F_6C026A5D3C5C__INCLUDED_)
#define AFX_STAISTICSMONEYDLG_H__C5289DF8_D11D_41B7_813F_6C026A5D3C5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StaisticsMoneyDlg.h : header file
//

#include "MyTabCtrl.h"
#include "ODBCStatement.h"
#include "AtumDateTime.h"
#include "SCGridHelper.h"
#include "AtumAdminToolDlg.h"
//#include "AtumProtocol.h"

#define SIZE_MAX_GAPTIME_YEAR		10
#define SIZE_MAX_GAPTIME_MONTH		11
#define SIZE_MAX_GAPTIME_DAY		31
#define SIZE_MAX_GAPTIME_HOUR		23
#define SIZE_MAX_GAPTIME_MINUTE		5

#define	TEXTPOS							3

#define	GRAPH_BACKGROUND_DRAW_X_MARGIN	35
#define	GRAPH_BACKGROUND_DRAW_Y_MARGIN	20
#define GRAPH_BACKGROUND_POS_X_START	16
#define GRAPH_BACKGROUND_POS_Y_START	436
#define GRAPH_BACKGROUND_POS_X_END		1234
#define GRAPH_BACKGROUND_POS_Y_END		800
#define GRAPH_BACKGROUND_WIDTH ((GRAPH_BACKGROUND_POS_X_END)-(GRAPH_BACKGROUND_POS_X_START))
#define GRAPH_BACKGROUND_HIGHT ((GRAPH_BACKGROUND_POS_Y_END)-(GRAPH_BACKGROUND_POS_Y_START))

#define	GRAPH_DRAW_MARGIN				5
#define GRAPH_DRAW_POS_X_START			GRAPH_BACKGROUND_POS_X_START+GRAPH_DRAW_MARGIN
#define GRAPH_DRAW_POS_Y_START			GRAPH_BACKGROUND_POS_Y_START+GRAPH_DRAW_MARGIN
#define GRAPH_DRAW_POS_X_END			GRAPH_BACKGROUND_POS_X_END-GRAPH_DRAW_MARGIN-GRAPH_BACKGROUND_DRAW_X_MARGIN
#define GRAPH_DRAW_POS_Y_END			GRAPH_BACKGROUND_POS_Y_END-GRAPH_DRAW_MARGIN-GRAPH_BACKGROUND_DRAW_Y_MARGIN
#define GRAPH_DRAW_POS_X_STANDARD_END	GRAPH_BACKGROUND_POS_X_END-GRAPH_DRAW_MARGIN
#define GRAPH_DRAW_WIDTH ((GRAPH_DRAW_POS_X_END)-(GRAPH_DRAW_POS_X_START))
#define GRAPH_DRAW_HIGHT ((GRAPH_DRAW_POS_Y_END)-(GRAPH_DRAW_POS_Y_START))

/////////////////////////////////////////////////////////////////////////////
// CStaisticsMoneyDlg dialog
class CAtumAdminToolDlg;
class CStaisticsMoneyDlg : public CDialog
{
// Construction
public:
	CStaisticsMoneyDlg(CAtumAdminToolDlg *i_pMainDlg, CWnd* pParent = NULL);   // standard constructor
	~CStaisticsMoneyDlg();
	CODBCStatement			*m_pODBCStmt;
	CAtumAdminToolDlg		*m_pMainDlg;
	CSCGridCtrl				m_GridStaisticsMoney;
	CDC						*m_pCurrentCD;
	vector<SSTAISTICSMONEY_INFO>		vectSStaisticsMoneyInfo_Search;	// start end 2012-02-27 by jhseol, ȭ������� DB ���� �˻���� ���� ����Ʈ
	vector<SSTAISTICSMONEY_INFO>		vectSStaisticsMoneyInfo_Result;	// start end 2012-02-27 by jhseol, ȭ������� DB ���� �� ����� �Ϸ�� ����Ʈ

// Dialog Data
	//{{AFX_DATA(CStaisticsMoneyDlg)
	enum { IDD = IDD_DIALOG_ATUM_ADMIN_TOOL };
	// NOTE: the ClassWizard will add data members here
	CTime			m_OleDateStart;
	CTime			m_OleTimeStart;
	CTime			m_OleDateEnd;
	CTime			m_OleTimeEnd;
//	COleDateTime	m_OleDateStart;
//	COleDateTime	m_OleTimeStart;
//	COleDateTime	m_OleDateEnd;
//	COleDateTime	m_OleTimeEnd;
	CComboBox		m_ComboGapTimeYear;
	CComboBox		m_ComboGapTimeMonth;
	CComboBox		m_ComboGapTimeDay;
	CComboBox		m_ComboGapTimeHour;
	CComboBox		m_ComboGapTimeMinute;
	BOOL			m_bCheckGapTimeYear;
	BOOL			m_bCheckGapTimeMonth;
	BOOL			m_bCheckGapTimeDay;
	BOOL			m_bCheckGapTimeHour;
	BOOL			m_bCheckGapTimeMinute;
	int				m_nIndex;						// start end 2012-02-27 by jhseol, ȭ������� DB �˻���� ����Ʈ�� �ε��� ����(���÷� ����)
	ATUM_DATE_TIME	m_GapTime;						// start end 2012-02-27 by jhseol, ȭ������� �˻� �ð��� ���� ���� ����
	SQL_TIMESTAMP_STRUCT StartTime, EndTime;		// start end 2012-02-27 by jhseol, ȭ������� DB ���ν��� ���� ����
	CProgressCtrl	m_Progress;
	CTabCtrl		m_GraphTab;
	CWnd			*m_pTab_show;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaisticsMoneyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:


	// Generated message map functions
	//{{AFX_MSG(CStaisticsMoneyDlg)
		// NOTE: the ClassWizard will add member functions here
	virtual BOOL OnInitDialog();
	void CheckBoxCheck();
	void OnCheckGapTimeYear();
	void OnCheckGapTimeMonth();
	void OnCheckGapTimeDay();
	void OnCheckGapTimeHour();
	void OnCheckGapTimeMinute();
	afx_msg void OnButtonSearch();
	afx_msg void OnButtonReset();
	afx_msg void OnButtonSaveFile();
	void DataArrangement();			// start end 2012-02-27 by jhseol, ȭ������� DB �˻���� ����Ʈ ���� �Լ�
	BOOL GetGapTime();
	afx_msg void OnGraphChange(NMHDR *pNMHDR, LRESULT *pResult);
	void DrawGraph(int SelectColumn);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STAISTICSMONEYDLG_H__C5289DF8_D11D_41B7_813F_6C026A5D3C5C__INCLUDED_)
