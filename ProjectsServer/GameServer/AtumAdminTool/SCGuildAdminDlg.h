#if !defined(AFX_SCGUILDADMINDLG_H__6609BE4A_6DF3_4E16_A2B5_A4DC38C5A64D__INCLUDED_)
#define AFX_SCGUILDADMINDLG_H__6609BE4A_6DF3_4E16_A2B5_A4DC38C5A64D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SCGuildAdminDlg.h : header file
//
#include "AtumProtocol.h"
#include "GridCtrl/GridCtrl.h"
#include "SCUserAdminDlg.h"
#include "EditGuildDlg.h"

// 2007-07-31 by cmkwon, SCAdminTool�� ���ܸ� ���� ��� �߰� - EditGuildDlg.h ���Ϸ� �̵���
//struct SGUILD		// 2006-03-07 by cmkwon
//{
//	UID32_t				GuildUID;
//	char				GuildName[SIZE_MAX_GUILD_NAME];			// ����
//	UID32_t				GuildMasterCharacterUID;				// ������ CharacterUniqueNumber
//	INT					GuildMemberCapacity;					// ���� ������ ������ �ִ� ��
//	BYTE				GuildState;
//	ATUM_DATE_TIME		GuildDismemberDate;
//	UINT				GuildMarkVersion;
//	INT					GuildMarkSize;
//	INT					WarWinPoint;							// ������ �¸� ��
//	INT					WarLossPoint;							// ������ �й� ��
//	INT					TotalFame;								// ���� �� ���� ��
//	INT					MonthlyFame;							// ���� �Ѵ� ���� ��
//	char				GuildMarkImage[SIZE_MAX_GUILD_MARK_IMAGE];	// 2007-07-26 by cmkwon, SCAdminTool�� GuildManager ���� - ���ܸ�ũ�� ��������
//};
/////////////////////////////////////////////////////////////////////////////
// CSCGuildAdminDlg dialog
class CODBCStatement;
class CAtumAdminToolDlg;
class CSCGuildAdminDlg : public CDialog
{
// Construction
public:
	CSCGuildAdminDlg(CWnd* pParent = NULL);   // standard constructor
	~CSCGuildAdminDlg();

	void InitGridGuild(void);
	void InitGridGuildMembers(void);
	void InitGridGuildItems(void);				// 2006-09-27 by cmkwon

	BOOL AddLineToGridGuildMembers(const char *i_szKey, const char *i_szValue);
	BOOL AddLineToGridGuildMembers(const char *i_szKey, int i_nValue);
	BOOL AddLineToGridGuildMembers(const char *i_szKey, UINT i_uiValue);
	BOOL AddLineToGridGuildMembers(const char *i_szKey, float i_fValue);
	BOOL AddLineToGridGuildMembers(const char *i_szKey, CBitmap *i_pbmGuildMark);	// 2007-07-27 by cmkwon, SCAdminTool�� GuildManager ���� - AddLineToGridGuildMembers() �Լ� �� �߰�

	BOOL DisplayGuildInfo(SGUILD *i_pGUILD, UID32_t i_nGuildMemberCardCashPrice);		// 2008-05-27 by dhjin, EP3 ���� ���� ���� - ���ܿ� ���� ĳ�� ������
	BOOL GetGuildInfo(const char *i_szGuildName, UID32_t i_guildUID=INVALID_UID32);		// 2007-07-27 by cmkwon, SCAdminTool�� GuildManager ���� - �����߰�(, UID32_t i_guildUID=INVALID_UID32)
	BOOL GetGuildMembersInfo(SGUILD *i_pGUILD);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-09-27 by cmkwon
	BOOL GetGuildItems(SGUILD *i_pGuild);
	void InsertItemToGridGuildItems(ITEM_GENERAL_READABLE *i_pItemGeneral, int i_nRow);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-07-27 by cmkwon, SCAdminTool�� GuildManager ���� - LoadBMPFileFromMemory() �Լ� �߰�
	static BOOL LoadBMPFileFromMemory(CBitmap *o_pBitmap, char *i_pData, int i_nDataLen);
	
	///////////////////////////////////////////////////////////////////////////////
	// 2007-07-31 by cmkwon, SCAdminTool�� ���ܸ� ���� ��� �߰� - DB���� ���ܸ� �˻� �Լ� �߰�
	BOOL SearchGuildNameFromDB(char *i_szGuildName);
	BOOL UpdateGuild2DB(SGUILD *i_pGuild);
public:
	CGridCtrl				m_GridGuild;
	CGridCtrl				m_GridGuildMembers;
	CGridCtrl				m_GridGuildItems;				// 2006-09-27 by cmkwon
	
	CAtumAdminToolDlg		*m_pMainDlg;
	CODBCStatement			*m_pODBCStmt;

	SGUILD					m_CurGUILD; 

	ez_map<INT, ITEM>				m_mapItemInfo;
	ez_map<INT, RARE_ITEM_INFO>		m_mapRareItemInfo;

	CImageList				m_ilGuildMarkList;			// 2007-07-26 by cmkwon, SCAdminTool�� GuildManager ���� - ���� ��ũ ����Ʈ ��� ���� �߰�

// Dialog Data
	//{{AFX_DATA(CSCGuildAdminDlg)
	enum { IDD = IDD_DLG_GUILD_ADMIN };
	CString	m_ctlstrSearchGuildName;
	UINT	m_ctluiSearchGuildUID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSCGuildAdminDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSCGuildAdminDlg)
	afx_msg void OnBtnSearchguild();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnSearchguildByguid();
	afx_msg void OnPaint();
	afx_msg void OnBtnEditGuild();
	virtual void OnOK();
	afx_msg void OnBtnGuildMarkRecognition();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCGUILDADMINDLG_H__6609BE4A_6DF3_4E16_A2B5_A4DC38C5A64D__INCLUDED_)
