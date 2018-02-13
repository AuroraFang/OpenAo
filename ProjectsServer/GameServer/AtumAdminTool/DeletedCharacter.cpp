// DeletedCharacter.cpp : implementation file
//

#include "stdafx.h"
#include "atumprotocol.h"
#include "atumadmintool.h"
#include "DeletedCharacter.h"
#include "ODBCStatement.h"
#include "AtumSJ.h"
#include "AtumDBLogWriter.h"
#include "SCUserAdminDlg.h"			// 2008-01-31 by cmkwon, ���� ��/���� ��ɾ�� ������ �ý��� ���� - 


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeletedCharacter dialog


CDeletedCharacter::CDeletedCharacter(char *i_szAccName, CODBCStatement *i_pODBCState, CWnd* pParent /*=NULL*/)
	: CDialog(CDeletedCharacter::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeletedCharacter)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pODBCStmt3	= i_pODBCState;
	util::strncpy(m_szAccountName1, i_szAccName, SIZE_MAX_ACCOUNT_NAME);
}


void CDeletedCharacter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeletedCharacter)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID_DELETED_CHARACTER_LIST, m_GridDeletedCharacterList);
}


BEGIN_MESSAGE_MAP(CDeletedCharacter, CDialog)
	//{{AFX_MSG_MAP(CDeletedCharacter)
	ON_COMMAND(IDR_CONMENU_SELECTED_UPDATE, OnConmenuSelectedUpdate)
	ON_COMMAND(IDR_CONMENU_SELECTED_DELETE_CHARACTER, OnConmenuSelectedDeleteCharacter)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_RCLICK, IDC_GRID_DELETED_CHARACTER_LIST, OnGridRClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeletedCharacter message handlers

BOOL CDeletedCharacter::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->InitGridDeletedCharacterList();
	this->DBLoadDeletedCharacterData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDeletedCharacter::InitGridDeletedCharacterList(void)
{
	m_GridDeletedCharacterList.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
	// 2008-01-17 by cmkwon, T_A: ���� ������ ĳ���� �� ����, ���� ������ ���� �����ֱ� �߰� - 
	//int m_nCols = 4;
	// 2008-03-03 by cmkwon, ���� ������ ĳ���͸���Ʈ ������ ���� �� ��¥ ���� �ֱ� - 
	//int m_nCols = 6;
	int m_nCols = 7;
	int m_nFixRows = 1;
	
	m_GridDeletedCharacterList.SetEditable(TRUE);
	m_GridDeletedCharacterList.SetListMode(TRUE);
	m_GridDeletedCharacterList.SetSingleRowSelection(TRUE);
	m_GridDeletedCharacterList.EnableSelection(TRUE);
	m_GridDeletedCharacterList.SetHeaderSort(FALSE);
	m_GridDeletedCharacterList.SetFrameFocusCell(FALSE);
	m_GridDeletedCharacterList.SetTrackFocusCell(FALSE);

	m_GridDeletedCharacterList.SetRowCount(m_nRows);
	m_GridDeletedCharacterList.SetColumnCount(m_nCols);
	m_GridDeletedCharacterList.SetFixedRowCount(m_nFixRows);

	// Į�� �����
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = ((DWORD)DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	Item.col = m_nCols++;
	Item.strText.Format(_T("AccountUID"));
	m_GridDeletedCharacterList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Account NAME"));
	m_GridDeletedCharacterList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("CharacterUID"));
	m_GridDeletedCharacterList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Character Name"));
	m_GridDeletedCharacterList.SetItem(&Item);

	// 2008-01-17 by cmkwon, T_A: ���� ������ ĳ���� �� ����, ���� ������ ���� �����ֱ� �߰� - 
	Item.col = m_nCols++;
	Item.strText.Format(_T("Influece"));
	m_GridDeletedCharacterList.SetItem(&Item);
	Item.col = m_nCols++;
	Item.strText.Format(_T("Selectable Influece"));
	m_GridDeletedCharacterList.SetItem(&Item);

	// 2008-03-03 by cmkwon, ���� ������ ĳ���͸���Ʈ ������ ���� �� ��¥ ���� �ֱ� - 
	Item.col = m_nCols++;
	Item.strText.Format(_T("deleted date"));
	m_GridDeletedCharacterList.SetItem(&Item);

	m_GridDeletedCharacterList.AutoSize();
	m_GridDeletedCharacterList.ExpandColumnsToFit();

	CCellRange tmpCellRange(1, 0, m_GridDeletedCharacterList.GetRowCount()-1, m_GridDeletedCharacterList.GetColumnCount()-1);
	m_GridDeletedCharacterList.ClearCells(tmpCellRange);
}

void CDeletedCharacter::InsertDeletedCharacterData(SDELETED_CHARACTER_INFO *i_pDeletedCharacterData)
{
	int m_nCols = 0;

	m_GridDeletedCharacterList.SetRowCount(m_GridDeletedCharacterList.GetRowCount()+1);

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = m_GridDeletedCharacterList.GetRowCount()-1;
	Item.nFormat = ((DWORD)DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pDeletedCharacterData->AccountUID);
	m_GridDeletedCharacterList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%s", i_pDeletedCharacterData->AccountName);
	m_GridDeletedCharacterList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pDeletedCharacterData->CharacterUID);
	m_GridDeletedCharacterList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%s", i_pDeletedCharacterData->CharacterName);
	m_GridDeletedCharacterList.SetItem(&Item);

	// 2008-01-17 by cmkwon, T_A: ���� ������ ĳ���� �� ����, ���� ������ ���� �����ֱ� �߰� - �׸��忡 �߰�
	Item.col = m_nCols++;
	Item.strText.Format("%s", CAtumSJ::GetInfluenceTypeString(i_pDeletedCharacterData->InflueceType0));
	m_GridDeletedCharacterList.SetItem(&Item);
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pDeletedCharacterData->SelectableInflueceMask);
	m_GridDeletedCharacterList.SetItem(&Item);

	// 2008-03-03 by cmkwon, ���� ������ ĳ���͸���Ʈ ������ ���� �� ��¥ ���� �ֱ� - �׸��忡 �߰�
	Item.col = m_nCols++;
	Item.strText.Format("%s", i_pDeletedCharacterData->atDeletedDTime.GetDateTimeString().GetBuffer());
	m_GridDeletedCharacterList.SetItem(&Item);
}

void CDeletedCharacter::DBLoadDeletedCharacterData(void)
{
	///////////////////////////////////////////////////////////////////////////////
	// 2007-05-15 by cmkwon, �˻� ���ڸ� AccountUID���� AccountName�� ����
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_admin_GetAllDeletedCharacter
	-- DESC      : 2007-05-15 by cmkwon,
	--				// 2008-01-17 by cmkwon, T_A: ���� ������ ĳ���� �� ����, ���� ������ ���� �����ֱ� �߰� - �������� �ʵ� �߰�(InfluenceType, SelectableInfluenceMask)
	--				// 2008-03-03 by cmkwon, ���� ������ ĳ���͸���Ʈ ������ ���� �� ��¥ ���� �ֱ� - 
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_admin_GetAllDeletedCharacter
		@i_AccName		VARCHAR(20)
	AS
		-- // 2008-01-17 by cmkwon, T_A: ���� ������ ĳ���� �� ����, ���� ������ ���� �����ֱ� �߰� - �������� �ʵ� �߰�(InfluenceType, SelectableInfluenceMask)
		-- // 2008-03-03 by cmkwon, ���� ������ ĳ���͸���Ʈ ������ ���� �� ��¥ ���� �ֱ� - ������ ��¥ �������� �߰�, ������ ��¥ ������ ���� �߰�
		SELECT AccountUniqueNumber, AccountName, UniqueNumber, CharacterName, InfluenceType, SelectableInfluenceMask, LastStartedTime FROM td_Character WITH (NOLOCK) 
		WHERE 0 <> Race & 0x4000 AND AccountName = @i_AccName
		ORDER BY LastStartedTime
	GO
	**************************************************************************/
	RETCODE	ret;
	
	SQLBindParameter(m_pODBCStmt3->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, m_szAccountName1, 0, NULL);		
	ret = SQLExecDirect(m_pODBCStmt3->m_hstmt, PROCEDURE_080822_0003, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		MessageBox("DB Execute Error !!(atum_admin_GetAllDeletedCharacter_)");
		m_pODBCStmt3->FreeStatement();
		return;
	}

	SDELETED_CHARACTER_INFO	tmDeletedCharacterInfo;
	util::zero(&tmDeletedCharacterInfo, sizeof(SDELETED_CHARACTER_INFO));
	SQL_TIMESTAMP_STRUCT tmDeletedTime;							// 2008-03-03 by cmkwon, ���� ������ ĳ���͸���Ʈ ������ ���� �� ��¥ ���� �ֱ� - 
	util::zero(&tmDeletedTime, sizeof(SQL_TIMESTAMP_STRUCT));	// 2008-03-03 by cmkwon, ���� ������ ĳ���͸���Ʈ ������ ���� �� ��¥ ���� �ֱ� - 

	SQLINTEGER	arrCB[8] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindCol(m_pODBCStmt3->m_hstmt, 1, SQL_C_ULONG, &tmDeletedCharacterInfo.AccountUID, 0,						&arrCB[1]);
	SQLBindCol(m_pODBCStmt3->m_hstmt, 2, SQL_C_CHAR, tmDeletedCharacterInfo.AccountName, SIZE_MAX_ACCOUNT_NAME,		&arrCB[2]);
	SQLBindCol(m_pODBCStmt3->m_hstmt, 3, SQL_C_ULONG, &tmDeletedCharacterInfo.CharacterUID, 0,						&arrCB[3]);
	SQLBindCol(m_pODBCStmt3->m_hstmt, 4, SQL_C_CHAR, tmDeletedCharacterInfo.CharacterName, SIZE_MAX_CHARACTER_NAME,	&arrCB[4]);
	SQLBindCol(m_pODBCStmt3->m_hstmt, 5, SQL_C_TINYINT, &tmDeletedCharacterInfo.InflueceType0, 0,					&arrCB[5]);	// 2008-01-17 by cmkwon, T_A: ���� ������ ĳ���� �� ����, ���� ������ ���� �����ֱ� �߰� - 
	SQLBindCol(m_pODBCStmt3->m_hstmt, 6, SQL_C_TINYINT, &tmDeletedCharacterInfo.SelectableInflueceMask, 0,			&arrCB[6]);	// 2008-01-17 by cmkwon, T_A: ���� ������ ĳ���� �� ����, ���� ������ ���� �����ֱ� �߰� - 
	SQLBindCol(m_pODBCStmt3->m_hstmt, 7, SQL_C_TIMESTAMP, &tmDeletedTime, 0,										&arrCB[7]);	// 2008-03-03 by cmkwon, ���� ������ ĳ���͸���Ʈ ������ ���� �� ��¥ ���� �ֱ� - 

	while ( (ret = SQLFetch(m_pODBCStmt3->m_hstmt)) != SQL_NO_DATA)
	{
		tmDeletedCharacterInfo.atDeletedDTime	= tmDeletedTime;	// 2008-03-03 by cmkwon, ���� ������ ĳ���͸���Ʈ ������ ���� �� ��¥ ���� �ֱ� - 
		this->InsertDeletedCharacterData(&tmDeletedCharacterInfo);
		util::zero(&tmDeletedCharacterInfo, sizeof(SDELETED_CHARACTER_INFO));
		util::zero(&tmDeletedTime, sizeof(SQL_TIMESTAMP_STRUCT));	// 2008-03-03 by cmkwon, ���� ������ ĳ���͸���Ʈ ������ ���� �� ��¥ ���� �ֱ� - 
	}

	m_pODBCStmt3->FreeStatement();
	m_GridDeletedCharacterList.Refresh();
}

void CDeletedCharacter::OnGridRClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	UpdateData();

    NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;
	if (pGridView->iRow == 0)
	{// 2006-08-25 by cmkwon, ������ ����
		return;
	}

	m_GridDeletedCharacterList.SetFocusCell(-1,-1);
	m_GridDeletedCharacterList.SetSelectedRange(pGridView->iRow, 0, pGridView->iRow, m_GridDeletedCharacterList.GetColumnCount()-1, TRUE, TRUE);
	m_GridDeletedCharacterList.SetFocusCell(pGridView->iRow, pGridView->iColumn);

	// show context menu
	CMenu muTemp, *pContextMenu;
	muTemp.LoadMenu(IDR_CONTEXT_MENU_DELETED_CHARACTER);
	CPoint point;
	GetCursorPos(&point);

	if(pGridView->iRow > 0)
	{// 2006-08-25 by cmkwon, ��ȿ�� ���� ����
		pContextMenu = muTemp.GetSubMenu(0);
	}
	else
	{// 2006-08-25 by cmkwon, ��ȿ���� ���� ���� ����
		return;
	}
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);	
}

void CDeletedCharacter::OnConmenuSelectedUpdate() 
{
	// TODO: Add your command handler code here
	CCellID cellID = m_GridDeletedCharacterList.GetFocusCell();
	if(0 >= cellID.row)
	{
		return;
	}

	// 2008-01-31 by cmkwon, ���� ��/���� ��ɾ�� ������ �ý��� ���� - üũ �ؾ��Ѵ�, �������� ĳ���� ����
	if(FALSE == ((CSCUserAdminDlg*)this->GetParent())->IsEnabledEdit())
	{
		AfxMessageBox("Now, permission denied !!");
		return;
	}

	SDELETED_CHARACTER_INFO	tmDeletedCharacterInfo;
	util::zero(&tmDeletedCharacterInfo, sizeof(SDELETED_CHARACTER_INFO));

	tmDeletedCharacterInfo.AccountUID		= atoi(m_GridDeletedCharacterList.GetItemText(cellID.row, 0));
	CString		tmAccountName				= m_GridDeletedCharacterList.GetItemText(cellID.row, 1);
	tmDeletedCharacterInfo.CharacterUID		= atoi(m_GridDeletedCharacterList.GetItemText(cellID.row, 2));
	CString		tmCharacterName				= m_GridDeletedCharacterList.GetItemText(cellID.row, 3);
	util::strncpy(tmDeletedCharacterInfo.AccountName, tmAccountName, SIZE_MAX_ACCOUNT_NAME);
	util::strncpy(tmDeletedCharacterInfo.CharacterName, tmCharacterName, SIZE_MAX_CHARACTER_NAME);
	
	if(0 == this->DBLiveDeletedCharacterData(tmDeletedCharacterInfo.AccountUID, tmDeletedCharacterInfo.CharacterUID))
	{
		CAtumDBLogWriter::InserLog_Live_Deleted_Character(m_pODBCStmt3, T1_FL_LOG_LIVE_DELETED_CHARACTER, &tmDeletedCharacterInfo);
	}

	this->OnOK();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CDeletedCharacter::OnConmenuSelectedDeleteCharacter() 
/// \brief		// 2008-01-17 by cmkwon, T_A: ���� ������ ĳ���� ������ �����ϴ� ��� �߰� - 
/// \author		cmkwon
/// \date		2008-01-17 ~ 2008-01-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CDeletedCharacter::OnConmenuSelectedDeleteCharacter() 
{
	// TODO: Add your command handler code here
	CCellID cellID = m_GridDeletedCharacterList.GetFocusCell();
	if(0 >= cellID.row)
	{
		return;
	}

	SDELETED_CHARACTER_INFO	tmDeletedCharacterInfo;
	util::zero(&tmDeletedCharacterInfo, sizeof(SDELETED_CHARACTER_INFO));

	tmDeletedCharacterInfo.AccountUID		= atoi(m_GridDeletedCharacterList.GetItemText(cellID.row, 0));
	CString		tmAccountName				= m_GridDeletedCharacterList.GetItemText(cellID.row, 1);
	tmDeletedCharacterInfo.CharacterUID		= atoi(m_GridDeletedCharacterList.GetItemText(cellID.row, 2));
	CString		tmCharacterName				= m_GridDeletedCharacterList.GetItemText(cellID.row, 3);
	util::strncpy(tmDeletedCharacterInfo.AccountName, tmAccountName, SIZE_MAX_ACCOUNT_NAME);
	util::strncpy(tmDeletedCharacterInfo.CharacterName, tmCharacterName, SIZE_MAX_CHARACTER_NAME);

	// 2008-01-31 by cmkwon, ���� ��/���� ��ɾ�� ������ �ý��� ���� - üũ �ؾ��Ѵ�.
	if(FALSE == ((CSCUserAdminDlg*)this->GetParent())->IsEnabledEdit())
	{
		AfxMessageBox("Now, permission denied !!");
		return;
	}
	
	char szTemp[1024];
	sprintf(szTemp, "Are you sure you want to delete character(%s)?", tmCharacterName);
	if(IDYES != AfxMessageBox(szTemp, MB_YESNO))
	{
		return;
	}
	
	// 2008-01-17 by cmkwon, ���� ĳ���͸� �����Ѵ�.
	if(FALSE == this->DBDeletePermanentlyDeletedCharacter(&tmDeletedCharacterInfo))
	{
		return;
	}
	
	// 2008-01-17 by cmkwon, ĳ���� �����α׸� �����.
	MSG_FL_LOG_DELETE_CHARACTER logDeleteCharacter;
	util::zero(&logDeleteCharacter, sizeof(MSG_FL_LOG_DELETE_CHARACTER));
	logDeleteCharacter.CharacterUniqueNumber	= tmDeletedCharacterInfo.CharacterUID;
	util::strncpy(logDeleteCharacter.CharacterName, tmDeletedCharacterInfo.CharacterName, SIZE_MAX_CHARACTER_NAME);
	logDeleteCharacter.AccountUniqueNumber		= tmDeletedCharacterInfo.AccountUID;
	util::strncpy(logDeleteCharacter.AccountName, tmDeletedCharacterInfo.AccountName, SIZE_MAX_ACCOUNT_NAME);
	if(FALSE == CAtumDBLogWriter::InsertLog_User_Game_Start_End(m_pODBCStmt3, T1_FL_LOG_DELETE_CHARACTER, &logDeleteCharacter, logDeleteCharacter.CharacterName, logDeleteCharacter.AccountUniqueNumber, logDeleteCharacter.AccountName, 0, 0))
	{
		AfxMessageBox("DB Query error !!, CAtumDBLogWriter::InsertLog_User_Game_Start_End error ");
	}

	// 2008-01-17 by cmkwon
	this->OnOK();
}

INT CDeletedCharacter::DBLiveDeletedCharacterData(UID32_t TmSelectedAccountUID, UID32_t TmSelectedCharacterUID)
{
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_LiveDeletedCharacter
	-- DESC      : 2007-02-22 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_LiveDeletedCharacter
		@i_AccountUID		INT,
		@i_CharacterUID		INT
	AS
		DECLARE @LivedCheck		TINYINT
		SELECT @LivedCheck = count(*) FROM atum_log_Live_Deleted_Character WITH (NOLOCK) WHERE CharacterUID = @i_CharacterUID
		
		-- // 2008-02-20 by cmkwon, ���� ��û���� 2ȸ���� ���� �����ϴ�
		IF (@LivedCheck >= 2)
		BEGIN
			-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
			-- RETURN	10			-- ���� ������ �ִ� �ɸ���
			SELECT 10;
			RETURN;
		END
		
		DECLARE @CharacterNum		INT
		SELECT @CharacterNum = count(*) FROM td_Character WITH (NOLOCK) WHERE 0 = Race & 0x4000 AND AccountUniqueNumber = @i_AccountUID
			
		IF (@CharacterNum >= 3)
		BEGIN
			-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
			-- RETURN	20			-- �ɸ��Ͱ� 3���� �ֱ� ������ ���� �Ұ�.
			SELECT 20;
			RETURN;
		END
		ELSE
		BEGIN
			UPDATE td_Character SET Race = Race ^ 0x4000 WHERE AccountUniqueNumber = @i_AccountUID AND UniqueNumber = @i_CharacterUID
		END
			
		-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
		--RETURN 0
		SELECT 0;
	GO
	**************************************************************************/
	RETCODE	ret;
	
	INT		ReturnCheck = 0;
// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
// 	SQLBindParameter(m_pODBCStmt3->m_hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &ReturnCheck, 0, NULL);	
// 	SQLBindParameter(m_pODBCStmt3->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &TmSelectedAccountUID, 0, NULL);
// 	SQLBindParameter(m_pODBCStmt3->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &TmSelectedCharacterUID, 0, NULL);
// 
// // 2008-07-08 by cmkwon, MySQL �������� ����(���ν������� ����ó�� �Ұ��� ����) - 
// //	ret = SQLExecDirect(m_pODBCStmt3->m_hstmt, (UCHAR*)"{ ? = call atum_LiveDeletedCharacter(?,?)}", SQL_NTS);
// 	ret = SQLExecDirect(m_pODBCStmt3->m_hstmt, (UCHAR*)"{call atum_LiveDeletedCharacter(?,?,?)}", SQL_NTS);
// 	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
// 	{
// 		m_pODBCStmt3->ProcessLogMessagesForStmt("ddd");
// 		MessageBox("DB Execute Error !!(LiveDeletedCharacter)");
// 		m_pODBCStmt3->FreeStatement();
// 		return -1;
// 	}
// 
// 	while ( ( ret = SQLMoreResults(m_pODBCStmt3->m_hstmt) ) != SQL_NO_DATA );
// 	m_pODBCStmt3->FreeStatement();

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLBindParameter(m_pODBCStmt3->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &TmSelectedAccountUID, 0, NULL);
	SQLBindParameter(m_pODBCStmt3->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &TmSelectedCharacterUID, 0, NULL);

	ret = SQLExecDirect(m_pODBCStmt3->m_hstmt, PROCEDURE_080822_0004, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		m_pODBCStmt3->ProcessLogMessagesForStmt("ddd");
		MessageBox("DB Execute Error !!(LiveDeletedCharacter)");
		m_pODBCStmt3->FreeStatement();
		return -1;
	}
	SQLBindCol(m_pODBCStmt3->GetSTMTHandle(), 1, SQL_C_LONG, &(ReturnCheck), 0, NULL);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(m_pODBCStmt3->GetSTMTHandle(), &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 �̸� ����� RowCount �̴� 
			ret = SQLMoreResults(m_pODBCStmt3->GetSTMTHandle());
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(m_pODBCStmt3->GetSTMTHandle());
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(m_pODBCStmt3->GetSTMTHandle());                 // 2008-07-25 by cmkwon, ���� Results �� �ִ����� �ѹ��� üũ�Ѵ�.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}
	m_pODBCStmt3->FreeStatement();

	if (10 == ReturnCheck)
	{
		MessageBox("LiveDeletedCharacter Fail : DeletedCharacter Lived");
		return	10;
	}

	if (20 == ReturnCheck)
	{
		MessageBox("LiveDeletedCharacter Fail : Full Character");
		return	20;
	}


	MessageBox("LiveDeletedCharacter Success");
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CDeletedCharacter::DBDeletePermanentlyDeletedCharacter(SDELETED_CHARACTER_INFO *i_pDeletedCharacterData)
/// \brief		// 2008-01-17 by cmkwon, T_A: ���� ������ ĳ���� ������ �����ϴ� ��� �߰� - DBDeletePermanentlyDeletedCharacter() �߰�
/// \author		cmkwon
/// \date		2008-01-17 ~ 2008-01-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CDeletedCharacter::DBDeletePermanentlyDeletedCharacter(SDELETED_CHARACTER_INFO *i_pDeletedCharacterData)
{
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_admin_DeleteDeletedCharacter
	-- DESC      : // 2008-01-17 by cmkwon, T_A: ���� ������ ĳ���� ������ �����ϴ� ��� �߰� - 
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_admin_DeleteDeletedCharacter
		@i_CharName			VARCHAR(20),
		@i_CharacterUID		INT
	AS
		-- 2008-01-17 by cmkwon, �����Ⱦ��� ������ ĳ�������� üũ
		IF NOT EXISTS( SELECT * FROM dbo.td_Character WITH(NOLOCK) WHERE UniqueNumber = @i_CharacterUID AND 0 <> (Race & 0x4000) )
			BEGIN
				-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
				--RETURN	10			-- ������ ������ ĳ���Ͱ� �ƴϴ�
				SELECT 10;
				RETURN;
			END
		
		-- 1. ĳ���� ���� �������� ��æƮ ����	
		DELETE td_Enchant FROM td_Store s INNER JOIN td_Enchant e ON s.UniqueNumber = e.TargetItemUniqueNumber
		WHERE s.Possess = @i_CharacterUID AND s.ItemStorage <> 2

		-- 2. ĳ���� ���� ������ ����
		DELETE FROM td_Store WHERE Possess = @i_CharacterUID AND ItemStorage <> 2

		-- 3. ĳ���� �� ���� ����
		DELETE FROM td_QuickSlot WHERE CharacterUID = @i_CharacterUID

		-- 4. ĳ���� ���� ��ų ����
		DELETE FROM td_StoreSkill WHERE Possess = @i_CharacterUID 

		-- 5. ����Ʈ ���̺��� ����
		DELETE FROM td_CharacterQuest WHERE CharacterUniqueNumber = @i_CharacterUID

		-- 6. ����Ʈ ���� ���� ���̺��� ����
		DELETE FROM td_CharacterQuestMonsterCount WHERE CharacterUniqueNumber  = @i_CharacterUID

		-- 7. ģ������Ʈ���� ����
		DELETE FROM td_FriendList
		WHERE CharacterName = @i_CharName OR FriendName = @i_CharName

		--8. Ʃ�丮�� ����Ʈ���� ����
		DELETE FROM td_TutorialComplete WHERE CharacterUID = @i_CharacterUID

		-- last ĳ���� ���̺��� ����
		DELETE FROM td_Character WHERE UniqueNumber = @i_CharacterUID
		
		  
		-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
		--RETURN 0
		SELECT 0;
	GO
	**************************************************************************/
	RETCODE	ret;
	
	INT		ReturnCheck = 0;
// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
// 	SQLBindParameter(m_pODBCStmt3->m_hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &ReturnCheck, 0, NULL);	
// 	SQLBindParameter(m_pODBCStmt3->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, i_pDeletedCharacterData->CharacterName, 0, NULL);		
// 	SQLBindParameter(m_pODBCStmt3->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_pDeletedCharacterData->CharacterUID, 0, NULL);
// 
// 	// 2008-07-08 by cmkwon, MySQL �������� ����(���ν������� ����ó�� �Ұ��� ����) - 
// 	//ret = SQLExecDirect(m_pODBCStmt3->m_hstmt, (UCHAR*)"{ ? = call atum_admin_DeleteDeletedCharacter(?,?)}", SQL_NTS);
// 	ret = SQLExecDirect(m_pODBCStmt3->m_hstmt, (UCHAR*)"{call atum_admin_DeleteDeletedCharacter(?,?,?)}", SQL_NTS);
// 	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
// 	{
// 		m_pODBCStmt3->FreeStatement();
// 		MessageBox("Delete character error !!, DB Execute Error");
// 		return FALSE;
// 	}
// 
// 	while ( ( ret = SQLMoreResults(m_pODBCStmt3->m_hstmt) ) != SQL_NO_DATA );
// 	m_pODBCStmt3->FreeStatement();

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLBindParameter(m_pODBCStmt3->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, i_pDeletedCharacterData->CharacterName, 0, NULL);		
	SQLBindParameter(m_pODBCStmt3->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_pDeletedCharacterData->CharacterUID, 0, NULL);

	ret = SQLExecDirect(m_pODBCStmt3->m_hstmt, PROCEDURE_080822_0005, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		m_pODBCStmt3->FreeStatement();
		MessageBox("Delete character error !!, DB Execute Error");
		return FALSE;
	}
	SQLBindCol(m_pODBCStmt3->GetSTMTHandle(), 1, SQL_C_LONG, &(ReturnCheck), 0, NULL);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(m_pODBCStmt3->GetSTMTHandle(), &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 �̸� ����� RowCount �̴� 
			ret = SQLMoreResults(m_pODBCStmt3->GetSTMTHandle());
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(m_pODBCStmt3->GetSTMTHandle());
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(m_pODBCStmt3->GetSTMTHandle());                 // 2008-07-25 by cmkwon, ���� Results �� �ִ����� �ѹ��� üũ�Ѵ�.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}
	m_pODBCStmt3->FreeStatement();

	if (0 != ReturnCheck)
	{
		MessageBox("Delete character error !!, No such character");
		return	FALSE;
	}

	MessageBox("Permanently deleted character !!");
	return TRUE;
}
