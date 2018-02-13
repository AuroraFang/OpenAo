// ModifyItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ModifyItemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModifyItemDlg dialog


///////////////////////////////////////////////////////////////////////////////
// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
#define COMBO_TEXT_DELETE			"Delete"		

CModifyItemDlg::CModifyItemDlg(CSCUserAdminDlg *i_pUserAdminDlg, BOOL i_bNoEdit/*=FALSE*/, CWnd* pParent /*=NULL*/)
	: CDialog(CModifyItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModifyItemDlg)
	m_szItemName = _T("");
	m_nItemCount = 0;
	m_ctl_nEnchantCount = 0;
	m_ctl_nInsertEnchatCount = 1;		// 2010-02-24 by cmkwon, AdminTool����:��æƮ �߰� ���� ���� �� �� �ְ� - 
	m_ctl_nRemoveEnchantCount = 1;		// 2012-11-21 by bckim, �������߰�, ��æƮ���������ű��

	//}}AFX_DATA_INIT
	m_nItemNum				= 0;
	m_nAddEnchantItemNum	= 0;
	m_nArmorColorItemNum	= 0;

	m_pUserAdminDlg			= i_pUserAdminDlg;
	m_bNoEdit				= i_bNoEdit;

	m_nItemKind				= 0;
	m_usReqUnitKind			= 0;	// 2008-06-19 by cmkwon, AdminTool ����(�� ��æƮ �߰� �Ҽ� �ְ�) - 
	m_nShapeItemNum			= 0;	// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 

	
#ifdef S_ADMINTOOL_ENCHANT_DELETE_MODIFY_BCKIM		// 2012-11-21 by bckim, �������߰�, ��æƮ���������ű��
	m_nOldItemEnchantCount	=0;
	m_nSubEnchantItemNum	=0;
	m_nOldItemDisEnchantCount	=0;	
	m_bDisEnchantAll			= FALSE;
#endif

}


void CModifyItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModifyItemDlg)
	DDX_Text(pDX, IDC_EDIT_ITEM_NAME, m_szItemName);
	DDX_Text(pDX, IDC_EDIT_ITEM_COUNT, m_nItemCount);
	DDX_Text(pDX, IDC_EDIT_ENCHNT_COUNT, m_ctl_nEnchantCount);
	DDX_Text(pDX, IDC_EDIT_INSERT_ENCHANT_COUNT, m_ctl_nInsertEnchatCount);
	DDV_MinMaxInt(pDX, m_ctl_nInsertEnchatCount, 1, 20);
	DDX_Text(pDX, IDC_EDIT_REMOVE_ENCHANT_COUNT, m_ctl_nRemoveEnchantCount);	// 2012-11-21 by bckim, �������߰�, ��æƮ���������ű��
	DDV_MinMaxInt(pDX, m_ctl_nRemoveEnchantCount, 1, 40);	// 2012-11-21 by bckim, �������߰�, ��æƮ���������ű��
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModifyItemDlg, CDialog)
	//{{AFX_MSG_MAP(CModifyItemDlg)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModifyItemDlg message handlers

void CModifyItemDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if(m_bNoEdit)
	{// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
		OnCancel();
		return;
	}

	CComboBox *pComboPrefix = (CComboBox*)GetDlgItem(IDC_COMBO_PREFIX);
	CComboBox *pComboSuffix = (CComboBox*)GetDlgItem(IDC_COMBO_SUFFIX);

	CString szPrefixName, szSuffixName;

	if (pComboPrefix->GetCurSel() != -1)
	{
		pComboPrefix->GetLBText(pComboPrefix->GetCurSel(), szPrefixName);
		if (szPrefixName == STRMSG_S_SCADMINTOOL_0025)
		{
			m_nItemPrefixCodeNum = 0;
		}
		else
		{
//			m_nItemPrefixCodeNum = m_pUserAdminDlg->m_mapRareItemInfoName.findEZ_ptr((LPCSTR)szPrefixName)->CodeNum;
			m_nItemPrefixCodeNum = GetRareCodeNumByRareDescription((LPSTR)(LPCSTR)szPrefixName);
		}
	}
	if (pComboSuffix->GetCurSel() != -1)
	{
		pComboSuffix->GetLBText(pComboSuffix->GetCurSel(), szSuffixName);
		if (szSuffixName == STRMSG_S_SCADMINTOOL_0025)
		{
			m_nItemSuffixCodeNum = 0;
		}
		else
		{
//			m_nItemSuffixCodeNum = m_pUserAdminDlg->m_mapRareItemInfoName.findEZ_ptr((LPCSTR)szSuffixName)->CodeNum;
			m_nItemSuffixCodeNum = GetRareCodeNumByRareDescription((LPSTR)(LPCSTR)szSuffixName);
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2005-12-08 by cmkwon, ��æƮ �߰�
	CComboBox *pComboAddEnchant = (CComboBox*)GetDlgItem(IDC_COMBO_ADD_ENCHANT);
	if(-1 != pComboAddEnchant->GetCurSel()
		&& m_ctl_nEnchantCount < SIZE_MAX_ENCHANT_PER_ITEM)
	{
		m_nAddEnchantItemNum	= 0;
		CString strEnchantDes;
		pComboAddEnchant->GetLBText(pComboAddEnchant->GetCurSel(), strEnchantDes);
		if (strEnchantDes.Compare(STRMSG_S_SCADMINTOOL_0025))
		{
			m_nAddEnchantItemNum = GetEnchantItemNumByEnchantDescription((LPSTR)(LPCTSTR)strEnchantDes);
			// 2013-07-01 by bckim, 10��æƮ �̸� ������ī��ó��, �α��߰�, ��� ����
			if( m_vectSubEnchantItemInfo.size() < 10 && TRUE == IsHyperCardByItemNum(m_nAddEnchantItemNum))
			{
				AfxMessageBox("Hyper enchant cannot be applied below item enchant level 10!!");
				m_nAddEnchantItemNum = 0;
			}
			// End. 2013-07-01 by bckim, 10��æƮ �̸� ������ī��ó��, �α��߰�, ��� ����
		}		
	}

#ifdef S_ADMINTOOL_ENCHANT_DELETE_MODIFY_BCKIM		// 2012-11-21 by bckim, �������߰�, ��æƮ���������ű��
	CComboBox *pComboSubEnchant = (CComboBox*)GetDlgItem(IDC_COMBO_SUB_ENCHANT);
	m_nOldItemDisEnchantCount = m_ctl_nEnchantCount;
	if( 0 == pComboSubEnchant->GetCurSel() )
	{
		m_bDisEnchantAll = TRUE;
	}
	if(-1 != pComboSubEnchant->GetCurSel() && m_ctl_nEnchantCount && 0 != pComboSubEnchant->GetCurSel() )	
	{
		m_nSubEnchantItemNum	= 0;
		vectSUB_ENCHANT_INFO::iterator	itr_SubEnchant = m_vectSubEnchantItemInfo.begin();
		while( itr_SubEnchant != m_vectSubEnchantItemInfo.end() )
		{
			if( itr_SubEnchant->nNumber == pComboSubEnchant->GetCurSel() )
			{
				m_nSubEnchantItemNum = itr_SubEnchant->nSubEnchantItemNum;
				// 2013-07-01 by bckim, 10��æƮ �̸� ������ī��ó��, �α��߰�, ��� ����
				if( TRUE ==  ExsistHyperCardInEnchantList() && FALSE == IsHyperCardByItemNum(m_nSubEnchantItemNum) )
				{
					AfxMessageBox("You can only remove normal enchant card after removing hyper enchant card !!");
					m_nSubEnchantItemNum = 0;
				}
				// End. 2013-07-01 by bckim, 10��æƮ �̸� ������ī��ó��, �α��߰�, ��� ����
			}
			itr_SubEnchant++;
		}
	}
#endif



// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
// 	///////////////////////////////////////////////////////////////////////////////
// 	// 2005-12-08 by cmkwon, �Ƹ� Ʃ�� ����
// 	CComboBox *pComboColorCode = (CComboBox*)GetDlgItem(IDC_COMBO_COLORCODE);
// 	if(-1 != pComboColorCode->GetCurSel())
// 	{
// 		CString strColorDes;
// 		pComboColorCode->GetLBText(pComboColorCode->GetCurSel(), strColorDes);
// 		m_nArmorColorItemNum = GetColorItemNumByColorItemDescription((LPSTR)(LPCTSTR)strColorDes);
// 	}
	///////////////////////////////////////////////////////////////////////////////
	// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
 	CComboBox *pComboShapeItem = (CComboBox*)GetDlgItem(IDC_COMBO_SHAPEITEMNUM);
	if(-1 != pComboShapeItem->GetCurSel())
	{
 		CString csText;
 		pComboShapeItem->GetLBText(pComboShapeItem->GetCurSel(), csText);
 		
		if(0 == csText.Compare(COMBO_TEXT_DELETE))
		{
			m_nShapeItemNum		= 0;		// ����
		}
		else
		{
			ITEM *pShapeItem	= m_pUserAdminDlg->m_mapItemInfo.findEZ_ptr(GetShapeItemNumByItemDesc((LPSTR)(LPCTSTR)csText));
			if(NULL == pShapeItem)
			{
				AfxMessageBox("Cann't find shapeItem !!");
				CDialog::OnCancel();
				return;
			}
			m_nShapeItemNum		= pShapeItem->LinkItem;	
		}
	}

	// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
	CComboBox *pComboEffectItem = (CComboBox*)GetDlgItem(IDC_COMBO_COLORCODE);
	if(-1 != pComboEffectItem->GetCurSel())
	{
		CString csText;
		pComboEffectItem->GetLBText(pComboEffectItem->GetCurSel(), csText);

		if(0 == csText.Compare(COMBO_TEXT_DELETE))
		{
			m_nArmorColorItemNum = 0;		// ����
		}
		else
		{
			m_nArmorColorItemNum = GetEffectItemNumByItemDesc((LPSTR)(LPCTSTR)csText);	
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2010-02-24 by cmkwon, AdminTool����:��æƮ �߰� ���� ���� �� �� �ְ� - 
	if(m_nAddEnchantItemNum
		&& SIZE_MAX_ENCHANT_PER_ITEM < m_ctl_nEnchantCount + m_ctl_nInsertEnchatCount)
	{
		AfxMessageBox("You can't add enchant to this item (MaxEnchantCount:40)!!");
		CDialog::OnCancel();
		return;
	}

	CDialog::OnOK();
}

BOOL CModifyItemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
	ITEM *pMainItemInfo = m_pUserAdminDlg->m_mapItemInfo.findEZ_ptr(m_nItemNum);
	if(NULL == pMainItemInfo)
	{
		MessageBox("Invalid ItemNum !!");
		OnOk();
		return FALSE;
	}

	if(m_bNoEdit
		|| FALSE == IS_COUNTABLE_ITEM(m_nItemKind))
	{
		GetDlgItem(IDC_EDIT_ITEM_COUNT)->EnableWindow(FALSE);
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2005-12-08 by cmkwon, ���� ���� ����
	CComboBox *pComboPrefix = (CComboBox*)GetDlgItem(IDC_COMBO_PREFIX);
	CComboBox *pComboSuffix = (CComboBox*)GetDlgItem(IDC_COMBO_SUFFIX);
	pComboPrefix->AddString(STRMSG_S_SCADMINTOOL_0025);
	pComboSuffix->AddString(STRMSG_S_SCADMINTOOL_0025);	

	// 2010-04-20 by cmkwon, �ű� ��Ű �ӽ� ���� -
	//if(IS_WEAPON(m_nItemKind))
	if(IS_RARE_TARGET_ITEMKIND(m_nItemKind))
	{
		// RareFix ����Ʈ ����
		char szPrefixRareDesc[SIZE_MAX_RARE_DESCRIPTION];
		char szSuffixRareDesc[SIZE_MAX_RARE_DESCRIPTION];
		util::zero(szPrefixRareDesc, SIZE_MAX_RARE_DESCRIPTION);
		util::zero(szSuffixRareDesc, SIZE_MAX_RARE_DESCRIPTION);


		SRARE_DESCRIPTION tmDescrip;
		ez_map<INT, RARE_ITEM_INFO>::iterator itr = m_pUserAdminDlg->m_mapRareItemInfo.begin();
		while (m_pUserAdminDlg->m_mapRareItemInfo.end() != itr)
		{
			char szTemp[SIZE_MAX_RARE_DESCRIPTION];
			util::zero(szTemp, SIZE_MAX_RARE_DESCRIPTION);
			RARE_ITEM_INFO *pRareItemInfo = &(itr->second);
			memset(&tmDescrip, 0x00, sizeof(SRARE_DESCRIPTION));
			sprintf(tmDescrip.szRareDescription, "%s(%d)    [%s]"
				, pRareItemInfo->Name, pRareItemInfo->CodeNum
				, GetRareParameterDescription(szTemp, pRareItemInfo));
			tmDescrip.nRareCodeNum = pRareItemInfo->CodeNum;

			if (IS_RARE_PREFIX(pRareItemInfo->CodeNum))
			{
				if(COMPARE_ITEMKIND(pRareItemInfo->ReqItemKind, m_nItemKind))
				{
					m_vectRARE_DESCRIPTIONList.push_back(tmDescrip);
					pComboPrefix->AddString(tmDescrip.szRareDescription);
					if (m_nItemPrefixCodeNum == pRareItemInfo->CodeNum)
					{
						util::strncpy(szPrefixRareDesc, tmDescrip.szRareDescription, SIZE_MAX_RARE_DESCRIPTION);
					}
				}
			}
			else if (IS_RARE_SUFFIX(pRareItemInfo->CodeNum))
			{
				if(COMPARE_ITEMKIND(pRareItemInfo->ReqItemKind, m_nItemKind))
				{
					m_vectRARE_DESCRIPTIONList.push_back(tmDescrip);
					pComboSuffix->AddString(tmDescrip.szRareDescription);
					if (m_nItemSuffixCodeNum == pRareItemInfo->CodeNum)
					{
						util::strncpy(szSuffixRareDesc, tmDescrip.szRareDescription, SIZE_MAX_RARE_DESCRIPTION);
					}
				}
			}
			itr++;
		}

		if(0 < strlen(szPrefixRareDesc))
		{
			pComboPrefix->SetCurSel(pComboPrefix->FindString(0, szPrefixRareDesc));
		}
		if(0 < strlen(szSuffixRareDesc))
		{
			pComboSuffix->SetCurSel(pComboSuffix->FindString(0, szSuffixRareDesc));
		}
	}
	if(m_bNoEdit
// 2010-04-20 by cmkwon, �ű� ��Ű �ӽ� ���� -
//		|| FALSE == IS_WEAPON(m_nItemKind))
		|| FALSE == IS_RARE_TARGET_ITEMKIND(m_nItemKind))
	{
		pComboPrefix->EnableWindow(FALSE);
		pComboSuffix->EnableWindow(FALSE);
	}


	///////////////////////////////////////////////////////////////////////////////
	// 2005-12-08 by cmkwon
	CComboBox *pComboAddEnchant = (CComboBox*)GetDlgItem(IDC_COMBO_ADD_ENCHANT);
	pComboAddEnchant->AddString(STRMSG_S_SCADMINTOOL_0025);
	SENCHANT_DESCRIPTION tmEDescrip;
	ez_map<INT, ITEM>::iterator itr = m_pUserAdminDlg->m_mapItemInfo.begin();
	for(; itr != m_pUserAdminDlg->m_mapItemInfo.end(); itr++)
	{
		ITEM *pItemEnchant = &itr->second;
		if(ITEMKIND_ENCHANT != pItemEnchant->Kind)
		{
			continue;
		}
		
		if(pItemEnchant->IsExistDesParam(DES_ENCHANT_INITIALIZE))
		{// 2010-02-24 by cmkwon, AdminTool����:��æƮ �߰��� ��æƮ�ʱ�ȭ ī�� �߰� �ȵǰ� ó�� - 
			continue;
		}

		if (FALSE == COMPARE_ITEMKIND(pItemEnchant->ReqItemKind, m_nItemKind))
		{
			continue;
		}
		// 2008-06-19 by cmkwon, AdminTool ����(�� ��æƮ �߰� �Ҽ� �ְ�) - �Ʒ��� ���� ����
		//if(pItemEnchant->ReqUnitKind != UNITKIND_ALL_MASK
		//	&& pItemEnchant->ReqUnitKind != m_nItemKind)
		if( pItemEnchant->ReqUnitKind != UNITKIND_ALL_MASK
			&& pItemEnchant->ReqUnitKind != m_usReqUnitKind )	// 2008-06-19 by cmkwon, AdminTool ����(�� ��æƮ �߰� �Ҽ� �ְ�) - üũ
		{
			continue;
		}
		char szTemp[SIZE_MAX_RARE_DESCRIPTION];
		util::zero(szTemp, SIZE_MAX_RARE_DESCRIPTION);
		util::zero(&tmEDescrip, sizeof(SENCHANT_DESCRIPTION));

		tmEDescrip.nEnchantItemNum	= pItemEnchant->ItemNum;
		sprintf(tmEDescrip.szEnchantDescription, "%s(%d)    [%s]"
			, pItemEnchant->ItemName, pItemEnchant->ItemNum
			, GetEnchantParameterDescription(szTemp, pItemEnchant));
		m_vectENCHANT_DESCRIPTIONList.push_back(tmEDescrip);
		pComboAddEnchant->AddString(tmEDescrip.szEnchantDescription);
	}
	if(m_bNoEdit
			|| 1 >= pComboAddEnchant->GetCount())
	{
		pComboAddEnchant->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_INSERT_ENCHANT_COUNT)->EnableWindow(FALSE);	// 2010-02-24 by cmkwon, AdminTool����:��æƮ �߰� ���� ���� �� �� �ְ� - 
	}

#ifdef S_ADMINTOOL_ENCHANT_DELETE_MODIFY_BCKIM		// 2012-11-21 by bckim, �������߰�, ��æƮ���������ű��
	CComboBox *pComboSubEnchant = (CComboBox*)GetDlgItem(IDC_COMBO_SUB_ENCHANT);
	
	vectSUB_ENCHANT_INFO::iterator	itr_SubEnchant;
	SUB_ENCHANT_INFO tempSubEnchant;

	if(m_vectEnchantItemNumList.empty())
	{
		pComboSubEnchant->EnableWindow(FALSE);
	}
	else
	{
		vectINT::iterator itr(m_vectEnchantItemNumList.begin());
		for(; itr != m_vectEnchantItemNumList.end(); itr++)
		{

			INT enchItemNum = *itr;
			if( m_vectSubEnchantItemInfo.empty() )
			{
				tempSubEnchant.nNumber = m_vectSubEnchantItemInfo.size() + 1;
				tempSubEnchant.nSubEnchantItemNum = enchItemNum;
				tempSubEnchant.nEnchantCount = 1;
				m_vectSubEnchantItemInfo.push_back(tempSubEnchant);	
			}
			else
			{
				// 2013-07-01 by bckim, 10��æƮ �̸� ������ī��ó��, �α��߰�, ��� ����
				// �����ڵ� �ּ�ó�� 
// 				BOOL exist = FALSE;
// 				itr_SubEnchant = m_vectSubEnchantItemInfo.begin();
// 				while ( itr_SubEnchant != m_vectSubEnchantItemInfo.end() )
// 				{	
// 					if( itr_SubEnchant->nSubEnchantItemNum == enchItemNum)	
// 					{
// 						itr_SubEnchant->nEnchantCount++;
// 						exist = TRUE;
// 						break;
// 					}
// 					itr_SubEnchant++;
// 				}
// 				if( !exist )
// 				{
// 					tempSubEnchant.nNumber = m_vectSubEnchantItemInfo.size() + 1 ;
// 					tempSubEnchant.nSubEnchantItemNum = enchItemNum;
// 					tempSubEnchant.nEnchantCount = 1;
// 					m_vectSubEnchantItemInfo.push_back(tempSubEnchant);					
// 				}		

					tempSubEnchant.nNumber = m_vectSubEnchantItemInfo.size() + 1 ;
					tempSubEnchant.nSubEnchantItemNum = enchItemNum;
					tempSubEnchant.nEnchantCount = 1;
					m_vectSubEnchantItemInfo.push_back(tempSubEnchant);					
				// End. 2013-07-01 by bckim, 10��æƮ �̸� ������ī��ó��, �α��߰�, ��� ����
			}
		}	

		int nEnchantIdx = 1;
		char szAddString[1024];
		//pComboSubEnchant->AddString(STRMSG_S_SCADMINTOOL_0058);
		pComboSubEnchant->AddString("( ======== !! Completely DisEnchant !! ======= )");
		

		sort(m_vectSubEnchantItemInfo.begin(), m_vectSubEnchantItemInfo.end(), SUB_ENCHANT_INFO_SORT_BY_SEQUENCE_NUMBER());		// 2013-07-01 by bckim, 10��æƮ �̸� ������ī��ó��, �α��߰�, ��� ����
		
		itr_SubEnchant = m_vectSubEnchantItemInfo.begin();
		for(; itr_SubEnchant != m_vectSubEnchantItemInfo.end(); itr_SubEnchant++)
		{
			INT enchItemNum = itr_SubEnchant->nSubEnchantItemNum;
			util::zero(szAddString, 1024);
			ITEM *pEnchantInfo = m_pUserAdminDlg->m_mapItemInfo.findEZ_ptr(enchItemNum);
			if(NULL == pEnchantInfo)
			{
				sprintf(szAddString, "[%02d] Invalid ItemNum(%d)", nEnchantIdx, enchItemNum);
			}
			else
			{
				// 2013-07-01 by bckim, 10��æƮ �̸� ������ī��ó��, �α��߰�, ��� ����
				BOOL bError = FALSE;
				if( nEnchantIdx <= 10 && TRUE == IsHyperCardByItemNum(enchItemNum))
				{
					bError = TRUE;
				}
				//sprintf(szAddString, "[%02d] %s(%d)          Count: [%2d]", nEnchantIdx, pEnchantInfo->ItemName, pEnchantInfo->ItemNum, itr_SubEnchant->nEnchantCount );
				sprintf(szAddString, "[%02d : %s] %s(%d) %s", nEnchantIdx,(IsHyperCardByItemNum(enchItemNum))?"Hyper":"Nomal", pEnchantInfo->ItemName, pEnchantInfo->ItemNum,
					(bError)?" \t<<<<< Hyper-enchant can��t be applied below level 10 !!":" ");
				// End. 2013-07-01 by bckim, 10��æƮ �̸� ������ī��ó��, �α��߰�, ��� ����
			}
			nEnchantIdx++;
			pComboSubEnchant->AddString(szAddString);
		}
	}		
	// 2013-05-29 by bckim, ��� ��æƮ ������ �����߰�  // ���� �������� ���� ��æƮ ���� ���� ���� ���� 
	if(m_bNoEdit
		|| 1 >= pComboSubEnchant->GetCount())
	{
		pComboSubEnchant->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_SUB_ENCHANT)->EnableWindow(FALSE);	
		GetDlgItem(IDC_EDIT_REMOVE_ENCHANT_COUNT)->EnableWindow(FALSE);
	}
	// End. 2013-05-29 by bckim, ��� ��æƮ ������ �����߰� 


#else
	GetDlgItem(IDC_COMBO_SUB_ENCHANT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_REMOVE_ENCHANT_COUNT)->EnableWindow(FALSE);

#endif



// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 	
// 	///////////////////////////////////////////////////////////////////////////////
// 	// 2005-12-08 by cmkwon, �Ƹ� Ʃ�� ����
// 	CComboBox *pComboColorCode = (CComboBox*)GetDlgItem(IDC_COMBO_COLORCODE);
// 	if(ITEMKIND_DEFENSE == m_nItemKind)
// 	{
// 		ITEM *pArmorItem = m_pUserAdminDlg->m_mapItemInfo.findEZ_ptr(m_nItemNum);
// 		if(pArmorItem)
// 		{
// 			char szCurColorItemDesc[SIZE_MAX_RARE_DESCRIPTION];
// 			char szBaseColorItemDesc[SIZE_MAX_RARE_DESCRIPTION];
// 			util::zero(szCurColorItemDesc, SIZE_MAX_RARE_DESCRIPTION);
// 			util::zero(szBaseColorItemDesc, SIZE_MAX_RARE_DESCRIPTION);
// 
// 			SCOLOR_DESCRIPTION tmCDescrip;
// 			char szTemp[SIZE_MAX_RARE_DESCRIPTION];
// 			
// 			for(int i= 0; i < SIZE_MAX_ARMOR_COLOR_COUNT; i++)
// 			{
// 				util::zero(szTemp, SIZE_MAX_RARE_DESCRIPTION);
// 				util::zero(&tmCDescrip, sizeof(SENCHANT_DESCRIPTION));
// 
// 				ITEM *pColorItem = m_pUserAdminDlg->m_mapItemInfo.findEZ_ptr(ARMORSourceIndex_TO_BaseColorItemNum(pArmorItem->SourceIndex)+i);
// 				if(NULL == pColorItem)
// 				{
// 					continue;
// 				}
// 
// 				tmCDescrip.nColorItemNum = pColorItem->ItemNum;
// 				SetColorItemDescription(tmCDescrip.szDescription, pColorItem);
// 				m_vectCOLOR_DESCRIPTIONList.push_back(tmCDescrip);
// 				pComboColorCode->AddString(tmCDescrip.szDescription);
// 
// 				if(0 == i)
// 				{
// 					strcpy(szBaseColorItemDesc, tmCDescrip.szDescription);
// 				}
// 				if(m_nArmorColorItemNum == pColorItem->ItemNum)
// 				{
// 					strcpy(szCurColorItemDesc, tmCDescrip.szDescription);
// 				}
// 			}
// 			if(0 < strlen(szCurColorItemDesc))
// 			{
// 				pComboColorCode->SetCurSel(pComboColorCode->FindString(0, szCurColorItemDesc));
// 			}
// 			else if(0 < strlen(szBaseColorItemDesc))
// 			{
// 				pComboColorCode->SetCurSel(pComboColorCode->FindString(0, szBaseColorItemDesc));
// 			}
// 		}
//	}
// 
// 	if(m_bNoEdit
// 		|| 0 >= pComboColorCode->GetCount())
// 	{
// 		pComboColorCode->EnableWindow(FALSE);
// 	}
	///////////////////////////////////////////////////////////////////////////////
	// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
	SITEM_DESC_FOR_ADTOOL	tmItemName4AT, tmOldItemName;
	vectItemPtr				vectItemList;

	util::zero(&tmOldItemName, sizeof(tmOldItemName));
	vectItemList.clear();
 	CComboBox *pComboShapeItemNum = (CComboBox*)GetDlgItem(IDC_COMBO_SHAPEITEMNUM);
	m_pUserAdminDlg->GetItemInfoListByDesParam(&vectItemList, DES_SHAPE_ITEM);
	int i;
	for(i=0; i < vectItemList.size(); i++)
	{
		util::zero(&tmItemName4AT, sizeof(tmItemName4AT));
		tmItemName4AT.pItemInfo	= vectItemList[i];
		if(FALSE == COMPARE_ITEMKIND(tmItemName4AT.pItemInfo->ReqItemKind, pMainItemInfo->Kind))
		{
			continue;
		}
		this->MakeItemDesc4ADTool(tmItemName4AT.szItemDesc, tmItemName4AT.pItemInfo);
		if(m_nShapeItemNum == tmItemName4AT.pItemInfo->LinkItem)
		{
			tmOldItemName = tmItemName4AT;
		}
		m_vectShapeItemDescList.push_back(tmItemName4AT);

		pComboShapeItemNum->AddString(tmItemName4AT.szItemDesc);
	}
	if(tmOldItemName.pItemInfo)
	{
		pComboShapeItemNum->SetCurSel(pComboShapeItemNum->FindString(0, tmOldItemName.szItemDesc));
	}
	else if(0 != m_nShapeItemNum)
	{
		MessageBox("not matched ShapeItemNum !!");
	}
	if(m_bNoEdit || 0 >= pComboShapeItemNum->GetCount())
	{
		pComboShapeItemNum->EnableWindow(FALSE);
	}
	else
	{
		pComboShapeItemNum->AddString(COMBO_TEXT_DELETE);
	}

	// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
	util::zero(&tmOldItemName, sizeof(tmOldItemName));
	vectItemList.clear();
	CComboBox *pComboEffectItemNum = (CComboBox*)GetDlgItem(IDC_COMBO_COLORCODE);
	m_pUserAdminDlg->GetItemInfoListByDesParam(&vectItemList, DES_EFFECT_ITEM);
	for(i=0; i < vectItemList.size(); i++)
	{
		util::zero(&tmItemName4AT, sizeof(tmItemName4AT));
		tmItemName4AT.pItemInfo	= vectItemList[i];
		if(FALSE == COMPARE_ITEMKIND(tmItemName4AT.pItemInfo->ReqItemKind, pMainItemInfo->Kind))
		{
			continue;
		}
		this->MakeItemDesc4ADTool(tmItemName4AT.szItemDesc, tmItemName4AT.pItemInfo);
		if(m_nArmorColorItemNum == tmItemName4AT.pItemInfo->ItemNum)
		{
			tmOldItemName = tmItemName4AT;
		}
		m_vectEffectItemDescList.push_back(tmItemName4AT);
		
		pComboEffectItemNum->AddString(tmItemName4AT.szItemDesc);
	}
	if(tmOldItemName.pItemInfo)
	{
		pComboEffectItemNum->SetCurSel(pComboEffectItemNum->FindString(0, tmOldItemName.szItemDesc));
	}
	else if(0 != m_nArmorColorItemNum)
	{
		MessageBox("not matched EffectItemNum !!");
	}
	if(m_bNoEdit || 0 >= pComboEffectItemNum->GetCount())
	{
		pComboEffectItemNum->EnableWindow(FALSE);
	}
	else
	{
		pComboEffectItemNum->AddString(COMBO_TEXT_DELETE);
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2010-02-24 by cmkwon, AdminTool����:�ش� �������� ��æƮ ����Ʈ �����ֱ� - ��æƼ ����Ʈ�� �޺��ڽ��� �߰��Ѵ�.
	CComboBox *pComboCurEnchantList = (CComboBox*)GetDlgItem(IDC_COMBO_ENCHANT_ITEMNUM_LIST);
	if(m_vectEnchantItemNumList.empty())
	{
		pComboCurEnchantList->EnableWindow(FALSE);
	}
	else
	{
		// 2013-02-27 by bckim, ��æƮ ����Ʈ���� ������(�����۳ѹ�����->��æƮ�ȼ���)

// 		sort(m_vectEnchantItemNumList.begin(), m_vectEnchantItemNumList.end(), less<INT>());
// 
// 		int nEnchantIdx = 1;
// 		vectINT::iterator itr(m_vectEnchantItemNumList.begin());
// 		for(; itr != m_vectEnchantItemNumList.end(); itr++)
// 		{
// 			INT enchItemNum = *itr;

		sort(m_vectEnchantOrder.begin(), m_vectEnchantOrder.end(), ENCHANT_ITEM_SORT_BY_SEQUENCE_NUMBER());	
		int nEnchantIdx = 1;
		vectENCHANT::iterator itr(m_vectEnchantOrder.begin());
		for(; itr != m_vectEnchantOrder.end(); itr++)
		{
			INT enchItemNum = itr->EnchantItemNum;			
		// End. 2013-02-27 by bckim, ��æƮ ����Ʈ���� ������(�����۳ѹ�����->��æƮ�ȼ���)

			char szAddString[1024];
			util::zero(szAddString, 1024);

			ITEM *pEnchantInfo = m_pUserAdminDlg->m_mapItemInfo.findEZ_ptr(enchItemNum);
			if(NULL == pEnchantInfo)
			{
				sprintf(szAddString, "[%02d] Invalid ItemNum(%d)", nEnchantIdx, enchItemNum);
			}
			else
			{
				// 2013-07-01 by bckim, 10��æƮ �̸� ������ī��ó��, �α��߰�, ��� ����
				sprintf(szAddString, "[%02d:%c] %s(%d)", nEnchantIdx,(IsHyperCardByItemNum(enchItemNum))?'H':'L',pEnchantInfo->ItemName, pEnchantInfo->ItemNum);
			}
			nEnchantIdx++;
			pComboCurEnchantList->AddString(szAddString);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


char *CModifyItemDlg::GetRareParameterDescription(char *o_szRareDesc, RARE_ITEM_INFO *i_pRareInfo)
{
// 2005-11-25 by cmkwon
//	switch(i_ReqItemKind)
//	{
//	case ITEMKIND_ALL_ITEM:					return "ITEMKIND_ALL_ITEM";
//	case ITEMKIND_ALL_WEAPON:				return "ITEMKIND_ALL_WEAPON";	
//	case ITEMKIND_PRIMARY_WEAPON_ALL:		return "ITEMKIND_PRIMARY_WEAPON_ALL";	
//	case ITEMKIND_PRIMARY_WEAPON_1:			return "ITEMKIND_PRIMARY_WEAPON_1";	
//	case ITEMKIND_PRIMARY_WEAPON_2:			return "ITEMKIND_PRIMARY_WEAPON_2";	
//	case ITEMKIND_SECONDARY_WEAPON_ALL:		return "ITEMKIND_SECONDARY_WEAPON_ALL";	
//	case ITEMKIND_SECONDARY_WEAPON_1:		return "ITEMKIND_SECONDARY_WEAPON_1";	
//	case ITEMKIND_SECONDARY_WEAPON_2:		return "ITEMKIND_SECONDARY_WEAPON_2";	
//	}
//
//	return "ITEMKIND_UNKNOWN";

	for(int i=0; i<SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
	{
		if(0 != i_pRareInfo->DesParameter[i])
		{
			sprintf(&o_szRareDesc[strlen(o_szRareDesc)], "%s(%4.2f), "
				, GetItemDesParameter(i_pRareInfo->DesParameter[i]), i_pRareInfo->ParameterValue[i]);
		}
	}
	
	if(2 < strlen(o_szRareDesc))
	{
		util::zero(&o_szRareDesc[strlen(o_szRareDesc)]-2, 2);
	}
	return o_szRareDesc;
}


INT CModifyItemDlg::GetRareCodeNumByRareDescription(char *i_szDesc)
{
	for(int i=0; i < m_vectRARE_DESCRIPTIONList.size(); i++)
	{
		if(0 == strcmp(i_szDesc, m_vectRARE_DESCRIPTIONList[i].szRareDescription))
		{
			return m_vectRARE_DESCRIPTIONList[i].nRareCodeNum;
		}
	}

	return 0;
}



///////////////////////////////////////////////////////////////////////////////
/// \fn			char *CModifyItemDlg::GetEnchantParameterDescription(char *o_szEnchantDesc, ITEM *i_pEnchantItemInfo)
/// \brief		
/// \author		cmkwon
/// \date		2005-11-26 ~ 2005-11-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
char *CModifyItemDlg::GetEnchantParameterDescription(char *o_szEnchantDesc, ITEM *i_pEnchantItemInfo)
{
// 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - �Ʒ��� ���� ���� ��
// 	if(0 != i_pEnchantItemInfo->DestParameter1)
// 	{
// 		sprintf(&o_szEnchantDesc[strlen(o_szEnchantDesc)], "%s(%4.2f), "
// 			, GetItemDesParameter(i_pEnchantItemInfo->DestParameter1), i_pEnchantItemInfo->ParameterValue1);
// 	}
// 	if(0 != i_pEnchantItemInfo->DestParameter2)
// 	{
// 		sprintf(&o_szEnchantDesc[strlen(o_szEnchantDesc)], "%s(%4.2f), "
// 			, GetItemDesParameter(i_pEnchantItemInfo->DestParameter2), i_pEnchantItemInfo->ParameterValue2);
// 	}
// 	if(0 != i_pEnchantItemInfo->DestParameter3)
// 	{
// 		sprintf(&o_szEnchantDesc[strlen(o_szEnchantDesc)], "%s(%4.2f), "
// 			, GetItemDesParameter(i_pEnchantItemInfo->DestParameter3), i_pEnchantItemInfo->ParameterValue3);
// 	}
// 	if(0 != i_pEnchantItemInfo->DestParameter4)
// 	{
// 		sprintf(&o_szEnchantDesc[strlen(o_szEnchantDesc)], "%s(%4.2f), "
// 			, GetItemDesParameter(i_pEnchantItemInfo->DestParameter4), i_pEnchantItemInfo->ParameterValue4);
// 	}
	///////////////////////////////////////////////////////////////////////////////
	// 2009-04-21 by cmkwon, ITEM�� DesParam �ʵ� ���� 8���� �ø��� - 
	for(int i=0; i < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; i++)
	{
		if(0 != i_pEnchantItemInfo->ArrDestParameter[i])
		{
 			sprintf(&o_szEnchantDesc[strlen(o_szEnchantDesc)], "%s(%4.2f), "
 				, GetItemDesParameter(i_pEnchantItemInfo->ArrDestParameter[i]), i_pEnchantItemInfo->ArrParameterValue[i]);
		}
	}
	
	if(2 < strlen(o_szEnchantDesc))
	{
		util::zero(&o_szEnchantDesc[strlen(o_szEnchantDesc)]-2, 2);
	}
	return o_szEnchantDesc;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT CModifyItemDlg::GetEnchantItemNumByEnchantDescription(char *i_szDesc)
/// \brief		
/// \author		cmkwon
/// \date		2005-11-26 ~ 2005-11-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CModifyItemDlg::GetEnchantItemNumByEnchantDescription(char *i_szDesc)
{
	for(int i=0; i < m_vectENCHANT_DESCRIPTIONList.size(); i++)
	{
		if(0 == strcmp(i_szDesc, m_vectENCHANT_DESCRIPTIONList[i].szEnchantDescription))
		{
			return m_vectENCHANT_DESCRIPTIONList[i].nEnchantItemNum;
		}
	}

	return 0;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			char *CModifyItemDlg::SetColorItemDescription(char *o_szColorDesc, ITEM *i_pColorItemInfo)
/// \brief		
/// \author		cmkwon
/// \date		2005-12-08 ~ 2005-12-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
char *CModifyItemDlg::SetColorItemDescription(char *o_szColorDesc, ITEM *i_pColorItemInfo)
{
	sprintf(o_szColorDesc, "%s(%8d)", i_pColorItemInfo->ItemName, i_pColorItemInfo->ItemNum);
	return o_szColorDesc;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT CModifyItemDlg::GetColorItemNumByColorItemDescription(char *i_szDesc)
/// \brief		
/// \author		cmkwon
/// \date		2005-12-08 ~ 2005-12-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CModifyItemDlg::GetColorItemNumByColorItemDescription(char *i_szDesc)
{
	for(int i=0; i < m_vectCOLOR_DESCRIPTIONList.size(); i++)
	{
		if(0 == strcmp(i_szDesc, m_vectCOLOR_DESCRIPTIONList[i].szDescription))
		{
			return m_vectCOLOR_DESCRIPTIONList[i].nColorItemNum;
		}
	}

	return 0;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
/// \author		cmkwon
/// \date		2009-09-02 ~ 2009-09-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
char * CModifyItemDlg::MakeItemDesc4ADTool(char *o_szItemDesc, ITEM *i_pItem)
{
	sprintf(o_szItemDesc, "%d (%s)", i_pItem->ItemNum, i_pItem->ItemName);
	return o_szItemDesc;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
/// \author		cmkwon
/// \date		2009-09-02 ~ 2009-09-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CModifyItemDlg::GetShapeItemNumByItemDesc(char *i_szDesc)
{
	for(int i=0; i < m_vectShapeItemDescList.size(); i++)
	{
		if(0 == strcmp(i_szDesc, m_vectShapeItemDescList[i].szItemDesc))
		{
			return m_vectShapeItemDescList[i].pItemInfo->ItemNum;
		}
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
/// \author		cmkwon
/// \date		2009-09-02 ~ 2009-09-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CModifyItemDlg::GetEffectItemNumByItemDesc(char *i_szDesc)
{
	for(int i=0; i < m_vectEffectItemDescList.size(); i++)
	{
		if(0 == strcmp(i_szDesc, m_vectEffectItemDescList[i].szItemDesc))
		{
			return m_vectEffectItemDescList[i].pItemInfo->ItemNum;
		}
	}

	return 0;
}


// 2013-07-01 by bckim, 10��æƮ �̸� ������ī��ó��, �α��߰�, ��� ����
BOOL CModifyItemDlg::IsHyperCardByItemNum(INT o_nItemNum)
{
	ENCHANT_INFO *pTempEnchantInfo = m_pUserAdminDlg->m_mapEnchantInfo.findEZ_ptr(o_nItemNum);

	if( NULL == pTempEnchantInfo )
	{
		return FALSE;
	}

	int nTotalProb = 0;
	for( int i=0; i<10; i++)
	{
		nTotalProb += pTempEnchantInfo->ProbabilityPerLevel[i];
		if( nTotalProb > 0 )
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CModifyItemDlg::ExsistHyperCardInEnchantList()
{
	vectSUB_ENCHANT_INFO::iterator	itr_SubEnchant = m_vectSubEnchantItemInfo.begin();
	while( itr_SubEnchant != m_vectSubEnchantItemInfo.end() )
	{
		if( TRUE == IsHyperCardByItemNum(itr_SubEnchant->nSubEnchantItemNum))
		{
			return TRUE;		// ��æƮ ����Ʈ �� ������ī�尡 ����Ǿ�����. 
		}
		itr_SubEnchant++;
	}
	return FALSE;		// ��æƮ ����Ʈ �� ������ī�尡 ����Ǿ����� ����.
}
// End. 2013-07-01 by bckim, 10��æƮ �̸� ������ī��ó��, �α��߰�, ��� ����

