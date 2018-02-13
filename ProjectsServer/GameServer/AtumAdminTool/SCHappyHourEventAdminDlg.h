#if !defined(AFX_SCHAPPYHOUREVENTADMINDLG_H__B3B90992_2FE3_40EA_B5A5_55B319F25507__INCLUDED_)
#define AFX_SCHAPPYHOUREVENTADMINDLG_H__B3B90992_2FE3_40EA_B5A5_55B319F25507__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SCHappyHourEventAdminDlg.h : header file
//

#include "GridCtrl/GridCtrl.h" //For GridCtrl
#include "ODBCStatement.h" //For DB Access
#include "AtumDateTime.h"

typedef mt_vector<SHAPPY_HOUR_EVENT>		mtvectHAPPY_HOUR_EVENT;		// 2006-08-22 by cmkwon, vector<SHAPPY_HOUR_EVENT> --> mt_vector<SHAPPY_HOUR_EVENT>

/////////////////////////////////////////////////////////////////////////////
// CSCHappyHourEventAdminDlg dialog
class CAtumAdminToolDlg;

class CSCHappyHourEventAdminDlg : public CDialog
{
// Construction
public:
	CString DayofWeekToString(int nDayofweek); // 0~6 ���� Sunday~Saturday �� �������ش�
	void GetHappyHourEventDetailInfo(); // DB���� ���� ���úκ� ���� �����´�.
	void InsertItemToGridDetail(SHAPPY_HOUR_EVENT *i_pHappyEv, int i_nRow); // GridDetail GriCtrl�� ���� �־��ش�.
	CSCHappyHourEventAdminDlg(CWnd* pParent = NULL);   // standard constructor
	~CSCHappyHourEventAdminDlg(); // �Ҹ���

	void InitGridDetail(); // GridTotal GridCtrl�� �ʱ�ȭ

	CGridCtrl m_GridTotal; //Happy Hour Event ��ü ����(DayOfWeek = 7) �׸��� ��Ʈ��
	CGridCtrl m_GridDetail; //Happy Hour Event ���� ���� �׸��� ��Ʈ��

	CODBCStatement					*m_pODBCStmt; //DB ���� Ŭ���� ������
	CAtumAdminToolDlg				*m_pMainDlg; //AtumAdminToll ���̾�α� Ŭ����
	
	///////////////////////////////////////////////////////////////////////////////
	// 2006-08-25 by cmkwon
	mtvectHAPPY_HOUR_EVENT			m_mtvectHappyEventList;			// 2006-08-25 by cmkwon
// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - ���� ������ �ּ� ó���ϰ� �迭 ������ �߰���
//	int								m_nHappyEvUID;
//	ATUM_DATE_TIME					m_atiHappyEvStartDateTime;		// 2006-08-25 by cmkwon,  
//	ATUM_DATE_TIME					m_atiHappyEvEndDateTime;		// 2006-08-25 by cmkwon
	SHAPPY_HOUR_EVENT				m_PCBangHappyEv;				// 2006-08-25 by cmkwon

	SHAPPY_HOUR_EVENT * FindHappyHourEvent(int i_nHappyEvUID);
	BOOL CheckHappyHourEvent(SHAPPY_HOUR_EVENT *i_pHappyEv);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� -
	ATUM_DATE_TIME					m_arrHappyEvPeriodStart[INFLUENCE_TYPE_COUNT];
	ATUM_DATE_TIME					m_arrHappyEvPeriodEND[INFLUENCE_TYPE_COUNT];
	ATUM_DATE_TIME *GetHappyEvPeriodStartDateTime(BYTE i_byInflTy);
	ATUM_DATE_TIME *GetHappyEvPeriodEndDateTime(BYTE i_byInflTy);
	BOOL UpdateHappyHourEventPeriod(BYTE i_byInflTy, ATUM_DATE_TIME *i_pATStart, ATUM_DATE_TIME *i_pATEnd);

// Dialog Data
	//{{AFX_DATA(CSCHappyHourEventAdminDlg)
	enum { IDD = IDD_DIALOG_HAPPY_HOUR_EVENT_ADMIN };
	COleDateTime	m_valueDTPTotalSetStartDate;
	COleDateTime	m_valueDTPTotalSetEndDate;
	COleDateTime	m_valueDTPTotalSetStartTime;
	COleDateTime	m_valueDTPTotalSetEndTime;
	COleDateTime	m_OleDatePCBangStart;
	COleDateTime	m_OleTimePCBangStart;
	COleDateTime	m_OleDatePCBangEnd;
	COleDateTime	m_OleTimePCBangEnd;
	COleDateTime	m_ctlOleDateNormalStart;
	COleDateTime	m_ctlOleDateNormalEnd;
	COleDateTime	m_ctlOleDateVCNStart;
	COleDateTime	m_ctlOleDateVCNEnd;
	COleDateTime	m_ctlOleDateANIStart;
	COleDateTime	m_ctlOleDateANIEnd;
	COleDateTime	m_ctlOleTimeNormalStart;
	COleDateTime	m_ctlOleTimeNormalEnd;
	COleDateTime	m_ctlOleTimeVCNStart;
	COleDateTime	m_ctlOleTimeVCNEnd;
	COleDateTime	m_ctlOleTimeANIStart;
	COleDateTime	m_ctlOleTimeANIEnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSCHappyHourEventAdminDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSCHappyHourEventAdminDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonTotalUpdate();
	afx_msg void OnButtonEnd();
	afx_msg void OnConmenuSelectedInsert();
	afx_msg void OnConmenuSelectedUpdate();
	afx_msg void OnConmenuSelectedDelete();
	afx_msg void OnConmenuNotselectedInsert();
	afx_msg void OnBtnUpdatePcbangHappyev();
	afx_msg void OnBtnReloadHappyEv();
	afx_msg void OnBtnReloadPCbangList();
	afx_msg void OnBtnUpdatePeriodNormal();
	afx_msg void OnBtnUpdatePeriodVcn();
	afx_msg void OnBtnUpdatePeriodAni();
	//}}AFX_MSG
    afx_msg void OnGridRClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCHAPPYHOUREVENTADMINDLG_H__B3B90992_2FE3_40EA_B5A5_55B319F25507__INCLUDED_)
