#if !defined(AFX_MODIFYITEMDLG_H__CA46DF11_23EA_4860_B76C_F0627E552F84__INCLUDED_)
#define AFX_MODIFYITEMDLG_H__CA46DF11_23EA_4860_B76C_F0627E552F84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModifyItemDlg.h : header file
//

#include "SCUserAdminDlg.h"

#define SIZE_MAX_RARE_DESCRIPTION		1024
struct SRARE_DESCRIPTION
{
	char		szRareDescription[SIZE_MAX_RARE_DESCRIPTION];
	INT			nRareCodeNum;
};
typedef vector<SRARE_DESCRIPTION>		vectRARE_DESCRIPTION;

struct SENCHANT_DESCRIPTION
{
	char		szEnchantDescription[SIZE_MAX_RARE_DESCRIPTION];
	INT			nEnchantItemNum;
};
typedef vector<SENCHANT_DESCRIPTION>	vectENCHANT_DESCRIPTION;

struct SCOLOR_DESCRIPTION
{
	char		szDescription[SIZE_MAX_RARE_DESCRIPTION];
	INT			nColorItemNum;
};
typedef vector<SCOLOR_DESCRIPTION>		vectCOLOR_DESCRIPTION;

///////////////////////////////////////////////////////////////////////////////
// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
struct SITEM_DESC_FOR_ADTOOL
{
	ITEM		*pItemInfo;
	char		szItemDesc[SIZE_MAX_RARE_DESCRIPTION];
};
typedef vector<SITEM_DESC_FOR_ADTOOL>		vectSITEM_DESC_FOR_ADTOOL;

/////////////////////////////////////////////////////////////////////////////
// CModifyItemDlg dialog

class CModifyItemDlg : public CDialog
{
	friend class CSCUserAdminDlg;
// Construction
public:
	CModifyItemDlg(CSCUserAdminDlg *i_pUserAdminDlg, BOOL i_bNoEdit=FALSE, CWnd* pParent = NULL);   // standard constructor

	char *GetRareParameterDescription(char *o_szRareDesc, RARE_ITEM_INFO *i_pRareInfo);
	INT GetRareCodeNumByRareDescription(char *i_szDesc);

	char *GetEnchantParameterDescription(char *o_szEnchantDesc, ITEM *i_pEnchantItemInfo);
	INT GetEnchantItemNumByEnchantDescription(char *i_szDesc);

	char *SetColorItemDescription(char *o_szColorDesc, ITEM *i_pColorItemInfo);
	INT GetColorItemNumByColorItemDescription(char *i_szDesc);

	// 2013-07-01 by bckim, 10��æƮ �̸� ������ī��ó��, �α��߰�, ��� ����
	BOOL IsHyperCardByItemNum(INT o_nItemNum);
	BOOL ExsistHyperCardInEnchantList();
	// End. 2013-07-01 by bckim, 10��æƮ �̸� ������ī��ó��, �α��߰�, ��� ����

// Dialog Data
	//{{AFX_DATA(CModifyItemDlg)
	enum { IDD = IDD_DIALOG_MODIFY_ITEM };
	CString	m_szItemName;
	int		m_nItemCount;
	int		m_ctl_nEnchantCount;
	int		m_ctl_nInsertEnchatCount;
	int		m_ctl_nRemoveEnchantCount;	// 2012-11-21 by bckim, �������߰�, ��æƮ���������ű��
	//}}AFX_DATA

	INT		m_nItemNum;
	INT		m_nItemPrefixCodeNum;
	INT		m_nItemSuffixCodeNum;
	INT		m_nAddEnchantItemNum;

#ifdef S_ADMINTOOL_ENCHANT_DELETE_MODIFY_BCKIM		// 2012-11-21 by bckim, �������߰�, ��æƮ���������ű��
	INT		m_nOldItemEnchantCount;				//	������ ��æƮ ī��Ʈ ENCHANT			
	INT		m_nSubEnchantItemNum;
	INT		m_nOldItemDisEnchantCount;			//	������ ��æƮ ī��Ʈ DISENCHANT
	BOOL	m_bDisEnchantAll;
#endif	

	BYTE	m_nItemKind;
	INT		m_nArmorColorItemNum;
	BOOL						m_bNoEdit;							// 2005-12-12 by cmkwon, �����Ұ� Flag
	USHORT	m_usReqUnitKind;					// 2008-06-19 by cmkwon, AdminTool ����(�� ��æƮ �߰� �Ҽ� �ְ�) - 
	INT		m_nShapeItemNum;				// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - ShapeItemNum �ʵ� �߰�

	CSCUserAdminDlg *m_pUserAdminDlg;
	vectRARE_DESCRIPTION		m_vectRARE_DESCRIPTIONList;
	vectENCHANT_DESCRIPTION		m_vectENCHANT_DESCRIPTIONList;
	vectCOLOR_DESCRIPTION		m_vectCOLOR_DESCRIPTIONList;			// 2005-12-08 by cmkwon
	
	///////////////////////////////////////////////////////////////////////////////
	// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
	vectSITEM_DESC_FOR_ADTOOL	 m_vectShapeItemDescList;			// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
	vectSITEM_DESC_FOR_ADTOOL	 m_vectEffectItemDescList;			// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
	char* MakeItemDesc4ADTool(char *o_szItemDesc, ITEM *i_pItem);
	INT GetShapeItemNumByItemDesc(char *i_szDesc);
	INT GetEffectItemNumByItemDesc(char *i_szDesc);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-02-24 by cmkwon, AdminTool����:�ش� �������� ��æƮ ����Ʈ �����ֱ� - 
	vectINT						m_vectEnchantItemNumList;
	vectENCHANT					m_vectEnchantOrder;					// 2013-02-27 by bckim, ��æƮ ����Ʈ���� ������(�����۳ѹ�����->��æƮ�ȼ���)

	
	vectSUB_ENCHANT_INFO		m_vectSubEnchantItemInfo;			// 2012-11-21 by bckim, �������߰�, ��æƮ���������ű��


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModifyItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModifyItemDlg)
	afx_msg void OnOk();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODIFYITEMDLG_H__CA46DF11_23EA_4860_B76C_F0627E552F84__INCLUDED_)
