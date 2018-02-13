#if !defined(AFX_STATISTICPAGE100DLG_H__59458CCC_B329_4A51_83CB_BE5BA17AFA8E__INCLUDED_)
#define AFX_STATISTICPAGE100DLG_H__59458CCC_B329_4A51_83CB_BE5BA17AFA8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticPage100Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage100Dlg dialog

///////////////////////////////////////////////////////////////////////////////
/// \class	CStatisticPage100Dlg	
///
/// \brief		���� ü�� �ð� �α�
/// \author		dhjin
/// \version	
/// \date		2006-08-07 ~ 2006-08-07
/// \warning	
///////////////////////////////////////////////////////////////////////////////

class CStatisticPage100Dlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CStatisticPage100Dlg)

// Construction
public:
	void DisplayUserEachTime();		// 2006-08-07 by dhjin, ���� �ð��� �� ����Ʈ �ڽ��� ���.
	void DisplayUserStayDaily();	// 2006-08-07 by dhjin, �Ϻ� ��� ���� ü�� �ð� ���.
	void DisplayUserStayTime();		// 2006-08-07 by dhjin, ���� �ķ� �ð� ���.
	BOOL GetUserStayTimeData(BOOL i_bPCBangUser);		// 2006-08-07 by dhjin, DB���� ����Ÿ �� ������.
	void Szformat();				// 2007-01-23 by dhjin, DB ������ �´� ���ڰ����� ����
	CStatisticPage100Dlg();
	~CStatisticPage100Dlg();

	CGridCtrl m_GridStaUserStayTime;
	CGridCtrl m_GridStaUserStayDaily;
	CODBCStatement					*m_pODBCStmt; //DB ���� Ŭ���� ������
	CSpaceCowboyDBEditorToolDlg		*m_pMainDlg; 

	typedef struct _m_UserStayTime
	{
		CString		m_AccountName;
		CString		m_CharacterName;
		CString		m_StartTime;
		CString		m_EndTime;
		int			m_StayTime;
		int			m_Level;
		int			m_PCBangUID;
		CString		m_CharacterCreatedTime;		// 2007-07-12 by cmkwon, �˻� ����� ĳ���� �����ð� �߰���
	};
	list<_m_UserStayTime> m_lUserStayTime;

	typedef struct _m_UserStayDaily
	{
		CString		m_StartTime;
		int			m_AveStayTime;
		int			m_TotalStayTime;
	};
	list<_m_UserStayDaily> m_lUserStayDaily;

	typedef struct _DAILYDATE{
	int		Year;
	int		Month;
	int		Day;
	} CDAILYDATE;

	CString		szStartDate, szEndDate, szSido, szGuGun, szPCbangName;

// Dialog Data
	//{{AFX_DATA(CStatisticPage100Dlg)
	enum { IDD = IDD_STA_SHEET_100 };
	COleDateTime	m_ctl_CreatedDateStart;
	COleDateTime	m_ctl_CreatedTimeStart;
	COleDateTime	m_ctl_CreatedDateEnd;
	COleDateTime	m_ctl_CreatedTimeEnd;
	int		m_10Min;
	int		m_20Min;
	int		m_2Hour;
	int		m_30Min;
	int		m_3Hour;
	int		m_4Hour;
	int		m_5Hour;
	int		m_60Min;
	int		m_6Hour;
	int		m_6UpHour;
	int		m_90Min;
	CString	m_Sido;
	CString	m_GunGu;
	CString	m_PCBangName;
	BOOL	m_ctl_bPCBang;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStatisticPage100Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CStatisticPage100Dlg)
	afx_msg void OnButtonSt100Select();
	afx_msg void OnCheckPcbang();
	afx_msg void OnButtonSt100Save();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICPAGE100DLG_H__59458CCC_B329_4A51_83CB_BE5BA17AFA8E__INCLUDED_)
