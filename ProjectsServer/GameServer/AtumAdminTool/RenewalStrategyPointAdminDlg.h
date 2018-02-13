#if !defined(AFX_RENEWALSTRATEGYPOINTADMINDLG_H__E4A85497_9F6F_4701_B4FC_8A150E4F244D__INCLUDED_)
#define AFX_RENEWALSTRATEGYPOINTADMINDLG_H__E4A85497_9F6F_4701_B4FC_8A150E4F244D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RenewalStrategyPointAdminDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRenewalStrategyPointAdminDlg dialog
#include "GridCtrl/GridCtrl.h" //For GridCtrl
#include "ODBCStatement.h" //For DB Access
#include "AtumDateTime.h"
#include "SCGridHelper.h"
#include "AtumDBHelper.h"
#include "atumparam.h"
#include "./Main/SCAdminWinSocket.h"

class CAtumAdminToolDlg;

class CRenewalStrategyPointAdminDlg : public CDialog
{
// Construction
public:
	CRenewalStrategyPointAdminDlg(CWnd* pParent = NULL, CSCAdminPreWinSocket* i_pAdminPreSocket = NULL);   // standard constructor
	CSCAdminPreWinSocket			*m_pStrategyPointAdminPreSocket;

	CAtumAdminToolDlg			*m_pMainDlg; //AtumAdminToll ���̾�α� Ŭ����
	CODBCStatement				*m_pODBCStmt; //DB ���� Ŭ���� ������
	CGridCtrl					m_GridTodayStrategyPointListInfo;
	vectSSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO			m_SStrategyPointMapInfluenceInfo;	// ������ ���� ���� ���������� ������ ���� DB���� �о��(ti_StrategyPointSummonMapIndex)
	vectSSTRATEGYPOINT_SUMMONTIME_INFO	m_vectStrategyPointSummonTimeInfo;	// ���� ���� ���� ���� ����(td_StrategyPointSummonInfo)
	int							m_nMaxStrategyPointCount;	// ������ ������ ������ ��
	int							m_nListSelectMapindex;		// ���̵忡�� ����(��Ŭ��)�� ����Ʈ�� �ɹ�ȣ

	void ApplySameChangesToAll(BYTE i_nWeek);
	void InitGridColumn();
	void InsertGridItem(SSTRATEGYPOINT_SUMMONTIME_INFO i_item);
	void UpdateGridItem();
	void LoadStrategyPointMapInfluenceInfo();
	void LoadStrategyPointSummonTimeInfo();
	void SettingRandumSummonTime(BYTE i_nWeek);
	void SummonInfoErrorCheck();
	void ChangeSummonState(BOOL i_bFlag);
	void ListLoad();
	void DBUpdateWeekdayStrategyPointSummonTime();
	void DBUpdateStrategyPointSummonInfo();

// Dialog Data
	//{{AFX_DATA(CRenewalStrategyPointAdminDlg)
	enum { IDD = IDD_DIALOG_RENEWAL_STRATEGYPOINT };
		// NOTE: the ClassWizard will add data members here
	COleDateTime	m_StartTime[7];		// ���Ϻ� �������� �ð��� ���۽ð�
	COleDateTime	m_EndTime[7];		// ���Ϻ� �������� �ð��� ����ð�
	BYTE			m_nCountBCU[7];		// BCU ������ ���Ϻ� �������� ��
	BYTE			m_nCountANI[7];		// ANI ������ ���Ϻ� �������� ��
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRenewalStrategyPointAdminDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRenewalStrategyPointAdminDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnButtonApply();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonMenuSummon();
	afx_msg void OnButtonMenuCancel();
	//}}AFX_MSG
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
    afx_msg void OnGridRClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RENEWALSTRATEGYPOINTADMINDLG_H__E4A85497_9F6F_4701_B4FC_8A150E4F244D__INCLUDED_)
