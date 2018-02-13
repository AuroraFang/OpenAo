// SCUserAdminDlg.h : header file
//

#if !defined(AFX_SCUSERADMINDLG_H__F23BF76C_BE5C_4CA6_BB07_CBB949EC9806__INCLUDED_)
#define AFX_SCUSERADMINDLG_H__F23BF76C_BE5C_4CA6_BB07_CBB949EC9806__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GridCtrl/GridCtrl.h"
#include "ODBCStatement.h"
#include "AtumDBHelper.h"
#include "atumparam.h"
#include "./Main/SCAdminWinSocket.h"
#include "resource.h"

class CAtumAdminToolDlg;

struct ITEM_GENERAL_READABLE : public ITEM_GENERAL
{
	char			ItemName[SIZE_MAX_ITEM_NAME];
	char			AccountName[SIZE_MAX_ACCOUNT_NAME];
	char			CharacterName[SIZE_MAX_CHARACTER_NAME];
	int				EnchantCount;
// 2009-11-18 by cmkwon, ������� ���� ���� �ּ� ó�� - 
//	ATUM_DATE_TIME	CreatedTime;

	ITEM_GENERAL_READABLE() : ITEM_GENERAL(NULL)
	{	
	};
};
typedef vector<ITEM_GENERAL_READABLE>		vectITEM_GENERAL_READABLE;

// PreIOCP.h�� CP_State�� �����ؾ� ��
typedef enum
{
	CP_NOTCONNECTED					= 0,	// Client�� �������� ���� ����
	CP_CONNECTED					= 1,	// Client�� Socket ������ ����
	CP_SINGLE_FILE_VERSIONCHECKED	= 2,	// Client�� SINGLE FILE VERSION�� update�� ����
	CP_VERSIONCHECKED				= 3,	// Client�� VERSION�� update�� ����
	CP_LOGINED						= 4,	// Client�� AccountName�� ������ Login �� ����, Launcher�� ���� �α���
	CP_FIELD_LOGINED				= 5,	// Client�� �ʵ� ������ ���� �α��� �� ����, ĳ���� ���� ȭ�鿡 �ִ� ��Ȳ
	CP_GAMESTARTED					= 6		// Client�� ������ �����Ͽ� �ʿ� �ִ� ��Ȳ
} CP_State;

const char *GetCPState(int i_nCPState);

enum enumGetAccountQueryType
{
	GAQT_ACCOUNT_NAME	= 0,
	GAQT_CHARACTER_NAME	= 1,
	GAQT_ACCOUNT_UID	= 2,
	GAQT_CHARACTER_UID	= 3
};

/////////////////////////////////////////////////////////////////////////////
// CSCUserAdminDlg dialog
class CAtumAdminToolApp;
class CSCUserAdminDlg : public CDialog
{
friend class CSelectItemDlg;
friend class CModifyItemDlg;
friend class CCharacterInfoDlg;
friend class CPartnerDlg;				// 2012-03-15 by jhseol, ��Ʈ�� - ��Ʈ�� Ŭ���� ���

// Construction
public:
	CSCUserAdminDlg(CWnd* pParent = NULL);	// standard constructor
	~CSCUserAdminDlg();


	void OnTimerForSendAlivePacket(void);

// Dialog Data
	//{{AFX_DATA(CSCUserAdminDlg)
	enum { IDD = IDD_DIALOG_USER_ADMIN_TOOL };
	CString	m_szAccountNameInput;
	CString	m_szAccountName;
	CString	m_CharacterName1;
	CString	m_CharacterName2;
	CString	m_CharacterName3;
	CString	m_szUsetStatus;
	int		m_nAccCharac;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSCUserAdminDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	// ���� ����
	BOOL ConnectServer();

	// grid ����
	void InitGridItem();
	void InsertItemToGrid(ITEM_GENERAL_READABLE *i_pItemGeneral, int i_nRow);
	BOOL GetItemGeneralInRow(int i_nRow, ITEM_GENERAL_READABLE *o_pItem);

	// ���� ���� ����
	void GetAccountInfo(const char *i_szName, enumGetAccountQueryType i_nGAQT);
	void GetCharacterInfo(const char *i_szCharacterName);
	void GetItemInfo(const char *i_szCharacterName);
	BOOL DisplayCharacterInfo(CHARACTER *i_pCharacter);
	BOOL FindItemByItemNum(int i_nItemNum);
	BOOL GetInfluenceLeaderCharacterInfo(BYTE i_byInflType);
	
	// DB
	BOOL InsertItemToDB(const char *i_szCharacterName, int i_nItemNum, int i_nItemCount);
	BOOL GetAccountBlockState(const char *i_szAccountName);
	void SendBlockAccount(const char *i_szAccName, int i_nBlockTy, ATUM_DATE_TIME i_atimeS, ATUM_DATE_TIME i_atimeE, const char *i_szAdminName, const char *i_szReason, const char *i_szReasonForOnlyAdmin);
	//void SendUnblockAccount(const char *i_szAccName);
	void SendUnblockAccount(const char *i_szAccName, const char *i_szAdminName = NULL);		// 2012-06-05 by mspark, ���� �� ID�� ���� ��� ID ���� �۾�

	// control ����
	void EnableAllOuputControls(BOOL i_bEnable = TRUE);
	void ResetControls(BOOL i_bOnlyOutPut = FALSE);
	void EnableControls_GetAccountInfo ( const MSG_PA_ADMIN_GET_ACCOUNT_INFO_OK *a_pPacket_AccountInfo );				// ���� ���� �޾��� ���� �⺻ �޴� ��Ʈ�� ����.
	void UserManagementToolControls ();			// 2013-01-07 by khkim, ��Ʈ�� GM ���Ѽ��� 

	// character â ����
	void InitGridCharacter();
	BOOL AddLineToGridCharacter(const char *i_szKey, const char *i_szValue);
	BOOL AddLineToGridCharacter(const char *i_szKey, int i_nValue);
	BOOL AddLineToGridCharacter(const char *i_szKey, float i_fValue);

	// �������� �����ֱ�
	char *GetRareString(char *o_szRareString, char *i_szRareName, INT i_nRareCodeNum);
	int GetRareCodeNum(const char *i_szRareString);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-03-31 by cmkwon, �����ʱ�ȭ �ý��� ���� - 
	BOOL DBQ_InitializeInfluenceAllCharacter(void);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
	void GetItemInfoListByDesParam(vectItemPtr *o_pVectItemList, DestParam_t i_byDesParam);		// 2011-08-01 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ��� ���� (DestParameter - 255 -> 32767 ����)

	///////////////////////////////////////////////////////////////////////////////
	// 2010-02-24 by cmkwon, AdminTool����:�ش� �������� ��æƮ ����Ʈ �����ֱ� - 

	// 2013-02-27 by bckim, ��æƮ ����Ʈ���� ������(�����۳ѹ�����->��æƮ�ȼ���)
	//BOOL DBQ_LoadEnchantList(vectINT *o_pVecEnchantItemNumList, UID64_t i_ItemUID, BYTE i_byItemKind);
	BOOL DBQ_LoadEnchantList(vectINT *o_pVecEnchantItemNumList, UID64_t i_ItemUID, BYTE i_byItemKind, vectENCHANT* o_pEnchantInfo = NULL);
	// End. 2013-02-27 by bckim, ��æƮ ����Ʈ���� ������(�����۳ѹ�����->��æƮ�ȼ���)

	// 2012-06-07 by hskim, ������ ���� ���� ��ȭ - ���� ���� ����� �α� ����
	void SendLogByAccountTypeChange(USHORT i_nOldAccountType, USHORT i_nNewAccountType);
	void SendLogByRaceAccountTypeChange(USHORT i_nOldAccountType, USHORT i_nNewAccountType);
	// end 2012-06-07 by hskim, ������ ���� ���� ��ȭ - ���� ���� ����� �α� ����

public:
	BOOL IsEnabledEdit(void);
	
	// registry
	void ReadProfile();
	void WriteProfile();

// Implementation
protected:
	CAtumAdminToolDlg				*m_pMainDlg;

	HICON m_hIcon;

	CSCAdminPreWinSocket			*m_pUserAdminPreSocket;
	CODBCStatement					*m_pODBCStmt;

	CGridCtrl						m_GridItem;
	CGridCtrl						m_GridCharacter;

	ez_map<INT, ITEM>				m_mapItemInfo;
	ez_map<string, ITEM>			m_mapItemInfoName;
	ez_map<INT, RARE_ITEM_INFO>		m_mapRareItemInfo;
	ez_map<string, RARE_ITEM_INFO>	m_mapRareItemInfoName;
	ez_map<INT, ENCHANT_INFO>		m_mapEnchantInfo;		// 2013-07-01 by bckim, 10��æƮ �̸� ������ī��ó��, �α��߰�, ��� ����

	CString							m_szCurrentCharacterName;	// ���� ���õ� ĳ���͸�
	CHARACTER						m_CurrentCharacter;			// ���� ���õ� ĳ����
	CHARACTER						m_InflLeaderCharacter;		// 2006-04-25 by cmkwon, ���������� ����(CharacterUniqueNumber, CharacterName�� ��ȿ��)
	CHARACTER						m_InflSub1LeaderCharacter;	// 2006-12-08 by dhjin, ��������1
	CHARACTER						m_InflSub2LeaderCharacter;	// 2006-12-08 by dhjin, ��������2
	BOOL							m_bCurrentCharacterOnline;	// TRUE�̸� NOT-Editable��
	BOOL							m_bCurrentAccountBlocked;	// FALSE�̸� NOT-Editable��
	SBLOCKED_ACCOUNT_INFO			m_blockedAccountInfo;
	
	BOOL							m_bIsValidAccountName;
	UID32_t							m_uidAccountUID;

	vectITEM_GENERAL_READABLE		m_vectCurrentCharacterGetItemList;		// 2012-03-15 by jhseol, ��Ʈ�� - ���õ� ĳ���Ͱ� ������ ������ ����Ʈ

	// Generated message map functions
	//{{AFX_MSG(CSCUserAdminDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOk();
	afx_msg void OnCmgInsertItem();
	afx_msg void OnCmgDeleteItem();
	afx_msg void OnCmgModifyItem();
	afx_msg void OnButtonGetUserInfo();
	afx_msg void OnButtonCharac1Ok();
	afx_msg void OnButtonCharac2Ok();
	afx_msg void OnButtonCharac3Ok();
	afx_msg void OnButtonReset();
	afx_msg void OnButtonReload();
	afx_msg void OnButtonEditCharacter();
	afx_msg void OnDestroy();
	virtual void OnCancel();
	afx_msg void OnButtonBlockAccount();
	afx_msg void OnButtonUnblockAccount();
	afx_msg void OnButtonEditAccount();
	afx_msg void OnButtonBlockEdit();
	afx_msg void OnButtonEditPremium();
	afx_msg void OnCmgItemInformation();
	afx_msg void OnBtnBlockedList();
	afx_msg void OnBtnInflwarList();
	afx_msg void OnButtonDeletedCharacter();
	afx_msg void OnBtnInitInfluence();
	afx_msg void OnBTNSETInfluenceRate();
	afx_msg void OnBtnInfinityResetImpute();
	//}}AFX_MSG
    afx_msg void OnGridRClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg LONG OnSocketNotifyPre(WPARAM wParam, LPARAM lParam);
//	afx_msg LONG OnSocketNotifyIM(WPARAM wParam, LPARAM lParam);
//	afx_msg LONG OnSocketNotifyField(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnAsyncSocketMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGridDClick(NMHDR *pNotifyStruct, LRESULT* pResult);		// 2012-03-15 by jhseol, ��Ʈ�� - ����Ŭ�� �̺�Ʈ �߰�
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCUSERADMINDLG_H__F23BF76C_BE5C_4CA6_BB07_CBB949EC9806__INCLUDED_)
