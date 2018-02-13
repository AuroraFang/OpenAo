// StatisticPage99Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "spacecowboydbeditortool.h"
#include "StatisticPage99Dlg.h"
#include "SpaceCowboyDBEditorToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage99Dlg property page

IMPLEMENT_DYNCREATE(CStatisticPage99Dlg, CPropertyPage)

CStatisticPage99Dlg::CStatisticPage99Dlg() : CPropertyPage(CStatisticPage99Dlg::IDD)
{
	//{{AFX_DATA_INIT(CStatisticPage99Dlg)
	m_ctl_nRankCount = 300;
	//}}AFX_DATA_INIT
	m_pMainDlg				= (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd();
	m_pODBCStmt				= m_pMainDlg->m_pODBCStmt;
}

CStatisticPage99Dlg::~CStatisticPage99Dlg()
{
}

void CStatisticPage99Dlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticPage99Dlg)
	DDX_Control(pDX, IDC_GRIDCTRL_VIEW_99, m_gridView99);
	DDX_Text(pDX, IDC_EDIT_RANK_COUNT, m_ctl_nRankCount);
	DDV_MinMaxInt(pDX, m_ctl_nRankCount, 1, 1000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticPage99Dlg, CPropertyPage)
	//{{AFX_MSG_MAP(CStatisticPage99Dlg)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnBtnSearch)
	ON_BN_CLICKED(IDC_BTN_SAVE_AS_FILE, OnBtnSaveAsFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage99Dlg message handlers

void CStatisticPage99Dlg::OnBtnSearch() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	m_ctl_nRankCount = max(1, m_ctl_nRankCount);
	m_ctl_nRankCount = min(1000, m_ctl_nRankCount);

	ViewIndividualFameRanking(m_ctl_nRankCount);
}

void CStatisticPage99Dlg::OnBtnSaveAsFile() 
{
	// TODO: Add your control notification handler code here
	if(0 >= m_gridView99.GetRowCount())
	{
		return;
	}
	
	CString tempStr;
	for(int i = 0; i<m_gridView99.GetRowCount();i++)
	{
		tempStr.Empty();
		for(int j = 0; j< m_gridView99.GetColumnCount();j++)
		{
			tempStr += m_gridView99.GetItemText( i, j);
			tempStr += "|\t";
		}
		m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)tempStr, tempStr.GetLength(), m_pMainDlg, 1);
	}	
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CStatisticPage99Dlg::ViewIndividualFameRanking(int i_nRankCount)
/// \brief		
/// \author		cmkwon
/// \date		2006-01-23 ~ 2006-01-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStatisticPage99Dlg::ViewIndividualFameRanking(int i_nRankCount)
{
	m_gridView99.SetBkColor(0xFFFFFF);
	m_gridView99.SetHeaderSort();

	m_gridView99.SetEditable(FALSE);
	m_gridView99.SetListMode(TRUE);
	m_gridView99.SetSingleRowSelection(TRUE);
	m_gridView99.EnableSelection(TRUE);
	m_gridView99.SetFrameFocusCell(FALSE);
	m_gridView99.SetTrackFocusCell(FALSE);

	m_gridView99.SetRowCount(1);
	m_gridView99.SetColumnCount(9);
	m_gridView99.SetFixedRowCount(1);

	// Į�� �����
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	Item.row = 0;

	Item.col = 0;
	Item.strText.Format("Num");
	m_gridView99.SetItem(&Item);
	Item.col = 1;
	Item.strText.Format("Account Name");
	m_gridView99.SetItem(&Item);
	Item.col = 2;
	Item.strText.Format("Character Name");
	m_gridView99.SetItem(&Item);
	Item.col = 3;
	Item.strText.Format("Unitkind");
	m_gridView99.SetItem(&Item);
	Item.col = 4;
	Item.strText.Format("InfluenceType");
	m_gridView99.SetItem(&Item);
	Item.col = 5;
	Item.strText.Format("Level");
	m_gridView99.SetItem(&Item);
	Item.col = 6;
	Item.strText.Format("GuildName");
	m_gridView99.SetItem(&Item);
	Item.col = 7;
	Item.strText.Format("Individual Fame Point");
	m_gridView99.SetItem(&Item);
	Item.col = 8;
	Item.strText.Format("Total Play Time");
	m_gridView99.SetItem(&Item);

	CString szSQLQuery;

	// 2007-08-09 by cmkwon, ������ ĳ���� ó��(RACE_DELETED_CHARACTER) - �˻� ������� ���� �Ѵ�(0 = c.Race&%d AND )		
// 2008-07-02 by cmkwon, MySQL ���� ���� - 
// 	szSQLQuery.Format("SELECT TOP %d c.accountname, c.charactername, dbo.atum_GetGearString(c.unitkind), c.InfluenceType, c.level, c.GuildName, c.Propensity, c.TotalPlayTime \
// 		FROM td_character c WITH (NOLOCK) \
// 		WHERE 0 = c.Race&%d AND c.race < 128 \
// 		ORDER BY c.Propensity DESC", i_nRankCount, RACE_DELETED_CHARACTER);

// 2008-08-27 by cmkwon, �������� Procedure�� ���� - 
/////////////////////////////////////////////////////////////////////////////////	
//// 2008-07-08 by cmkwon, MySQL ���� ���� - 
//#if defined(DB_SERVER_MYSQL)
//	szSQLQuery.Format(QUERY_080702_0223, RACE_DELETED_CHARACTER, i_nRankCount);
//#else
//	szSQLQuery.Format(QUERY_080702_0223, i_nRankCount, RACE_DELETED_CHARACTER);
//#endif
	USHORT	tmRaceDeletedCharacter = RACE_DELETED_CHARACTER;
	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_nRankCount, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmRaceDeletedCharacter, 0, NULL);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0223));
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}

	SQLINTEGER arrCB[9]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	char szAccName[SIZE_MAX_ACCOUNT_NAME];
	char szCharName[SIZE_MAX_CHARACTER_NAME];
	char szUnitkind[10];
	BYTE byInfluenceType = 0;
	BYTE byLevel = 0;
	char szGuildName[SIZE_MAX_GUILD_NAME];
	int nFamePoint = 0;
	LONGLONG llTotalTime = 0;
	
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, szAccName, SIZE_MAX_ACCOUNT_NAME,		&arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_CHAR, szCharName, SIZE_MAX_CHARACTER_NAME,	&arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_CHAR, szUnitkind, 10,						&arrCB[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_TINYINT, &byInfluenceType, 0,					&arrCB[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_TINYINT, &byLevel, 0,							&arrCB[5]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_CHAR, szGuildName, SIZE_MAX_GUILD_NAME,		&arrCB[6]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 7, SQL_C_LONG, &nFamePoint, 0,							&arrCB[7]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 8, SQL_C_UBIGINT, &llTotalTime, 0,						&arrCB[8]);


	util::zero(szGuildName, SIZE_MAX_GUILD_NAME);
	nFamePoint	= 0;
	// DB�� ���� ���������� loop�� ����
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		int nNewRowIdx = m_gridView99.GetRowCount();
		m_gridView99.SetRowCount(nNewRowIdx+1);

		//select�� ���� GridDetail GridCtrl�� �־��ش�.
		Item.row		= nNewRowIdx;
		Item.col		= 0;
		Item.strText.Format("%d", nNewRowIdx);
		m_gridView99.SetItem(&Item);

		Item.col		= 1;
		Item.strText.Format("%s", szAccName);
		m_gridView99.SetItem(&Item);

		Item.col		= 2;
		Item.strText.Format("%s", szCharName);
		m_gridView99.SetItem(&Item);

		Item.col		= 3;
		Item.strText.Format("%s", szUnitkind);
		m_gridView99.SetItem(&Item);

		Item.col		= 4;
		Item.strText.Format("%s", CAtumSJ::GetInfluenceTypeString(byInfluenceType));
		m_gridView99.SetItem(&Item);

		Item.col		= 5;
		Item.strText.Format("%d", byLevel);
		m_gridView99.SetItem(&Item);

		Item.col		= 6;
		Item.strText.Format("%s", szGuildName);
		m_gridView99.SetItem(&Item);
		
		Item.col		= 7;
		Item.strText.Format("%d", nFamePoint);
		m_gridView99.SetItem(&Item);

		Item.col		= 8;
		Item.strText.Format("%-15s(%d sec)", ATUM_DATE_TIME::GetDateTimeStringFromSeconds(llTotalTime, string()), (int)llTotalTime);
		m_gridView99.SetItem(&Item);

		util::zero(szGuildName, SIZE_MAX_GUILD_NAME);
		nFamePoint	= 0;
	}
	m_gridView99.UpdateData();

	m_pODBCStmt->FreeStatement();	// clean up

	m_gridView99.AutoSize();
}