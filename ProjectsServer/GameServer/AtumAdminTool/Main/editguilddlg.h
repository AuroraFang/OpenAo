#if !defined(AFX_EDITGUILDDLG_H__D929A69D_72D0_420D_B8C5_0F44D45DFBBB__INCLUDED_)
#define AFX_EDITGUILDDLG_H__D929A69D_72D0_420D_B8C5_0F44D45DFBBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// editguilddlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditGuildDlg dialog
struct SGUILD		// 2006-03-07 by cmkwon
{
	UID32_t				GuildUID;
	char				GuildName[SIZE_MAX_GUILD_NAME];			// ����
	UID32_t				GuildMasterCharacterUID;				// ������ CharacterUniqueNumber
	INT					GuildMemberCapacity;					// ���� ������ ������ �ִ� ��
	BYTE				GuildState;
	ATUM_DATE_TIME		GuildDismemberDate;
	UINT				GuildMarkVersion;
	INT					GuildMarkSize;
	INT					WarWinPoint;							// ������ �¸� ��
	INT					WarLossPoint;							// ������ �й� ��
	INT					TotalFame;								// ���� �� ���� ��
	INT					MonthlyFame;							// ���� �Ѵ� ���� ��
	char				GuildMarkImage[SIZE_MAX_GUILD_MARK_IMAGE];	// 2007-07-26 by cmkwon, SCAdminTool�� GuildManager ���� - ���ܸ�ũ�� ��������
	BYTE				GuildMarkState;							// 2007-08-02 by cmkwon, ���� ��ũ �ɻ� �ý��� ���� - �߰� �ʵ� �˻�(, GuildMarkState)
};

class CEditGuildDlg : public CDialog
{
// Construction
public:
	CEditGuildDlg(SGUILD *i_pGuildInfo, BOOL i_bEnableEdit, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditGuildDlg)
	enum { IDD = IDD_DLG_EDIT_GUILD };
	CComboBox	m_ctlcomboBoxGuildMarkState;
	UINT	m_ctluiGuildUID;
	CString	m_ctlcsGuildName;
	//}}AFX_DATA

	SGUILD		m_guildInfo;
	BOOL			m_bEnableEdit1;			// 2007-10-02 by cmkwon, SCAdminTool ���� ���� ó�� -

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditGuildDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditGuildDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITGUILDDLG_H__D929A69D_72D0_420D_B8C5_0F44D45DFBBB__INCLUDED_)
