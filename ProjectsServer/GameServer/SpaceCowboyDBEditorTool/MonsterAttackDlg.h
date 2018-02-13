#if !defined(AFX_MONSTERATTACKDLG_H__33D82391_DB14_4D77_8E72_220C2580D45F__INCLUDED_)
#define AFX_MONSTERATTACKDLG_H__33D82391_DB14_4D77_8E72_220C2580D45F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MonsterAttackDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMonsterAttackDlg dialog

class CMonsterAttackDlg : public CDialog
{
// Construction
public:
	void ChangeGridDate(CGridCtrl *oriGrid, CGridCtrl *newGrid, int type, int row); //�� GridCtrl���� 1row�� �����͸� �ű��
	void SetDetailDialog(int row); //���õ� ���� �������� �ڽ� ���̾�α׿� ���� �����Ѵ�.
	void InsertItemToGridCurrent(_TI_ITEM *i_pTi_Item, int i_nRow); //DB���� ������ 1row�� �����͸� Current GridCtrl�� �ݿ��Ѵ�.
	void GetCurrentList(void); //DB���� �����͸� ��� �ҷ��´�.
	void InitGridUpdate(); //Update GridCtrl �ʱ� ����
	void InitGridCurrent(); //Current GridCtrl �ʱ� ����
	CMonsterAttackDlg(MONSTER_ITEMLIST_TYPE i_enItemListTy, int i_arrItemNumList[ARRAY_SIZE_MONSTER_ITEM]=NULL, CWnd* pParent = NULL);   // standard constructor
	CMonsterAttackDlg::~CMonsterAttackDlg();


// Dialog Data
	//{{AFX_DATA(CMonsterAttackDlg)
	enum { IDD = IDD_DIALOG_MONSTERATTACK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CODBCStatement					*m_pODBCStmt; //DB ���� Ŭ���� ������
	CSpaceCowboyDBEditorToolDlg				*m_pMainDlg; //���� ���̾�α� Ŭ����
	CMonsterAttackDetailDlg				*m_pMonsterAttackDetailDlg; //�ڽ� ���̾�α� ������
	CGridCtrl		m_GridCurrent; //Current Grid Ctrl ����
	CGridCtrl		m_GridUpdate; //Update Grid Ctrl ����

	MONSTER_ITEMLIST_TYPE			m_enMonsterItemListType;								// 2005-12-23 by cmkwon, 
	int								m_arrMonsterItemNumList[ARRAY_SIZE_MONSTER_ITEM];		// 2005-12-23 by cmkwon, �ϳ��� ���� ������ ����Ʈ

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonsterAttackDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMonsterAttackDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonEnd();
	afx_msg void OnButtonView();
	afx_msg void OnButtonInsert();
	afx_msg void OnButtonUpdate();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonRefresh();
	afx_msg void OnButtonCancel();
	afx_msg void OnButtonServerupdate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONSTERATTACKDLG_H__33D82391_DB14_4D77_8E72_220C2580D45F__INCLUDED_)
