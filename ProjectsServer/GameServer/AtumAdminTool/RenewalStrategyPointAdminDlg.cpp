// RenewalStrategyPointAdminDlg.cpp : implementation file
//

#include "stdafx.h"
#include "atumadmintool.h"
#include "RenewalStrategyPointAdminDlg.h"
#include "AtumAdminToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRenewalStrategyPointAdminDlg dialog

CRenewalStrategyPointAdminDlg::CRenewalStrategyPointAdminDlg(CWnd* pParent /*=NULL*/, CSCAdminPreWinSocket* i_pAdminPreSocket)
	: CDialog(CRenewalStrategyPointAdminDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRenewalStrategyPointAdminDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pMainDlg = (CAtumAdminToolDlg*)AfxGetMainWnd();
	m_pODBCStmt = new CODBCStatement;
	util::zero(m_nCountBCU, sizeof(m_nCountBCU));
	util::zero(m_nCountANI, sizeof(m_nCountANI));
	util::zero(m_StartTime, sizeof(m_StartTime));
	util::zero(m_EndTime, sizeof(m_EndTime));
	m_SStrategyPointMapInfluenceInfo.clear();
	m_nMaxStrategyPointCount = 0;
	m_vectStrategyPointSummonTimeInfo.clear();
	m_nListSelectMapindex = 0;
	m_pStrategyPointAdminPreSocket = i_pAdminPreSocket;
}


void CRenewalStrategyPointAdminDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRenewalStrategyPointAdminDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_MONSTARTTIME	, m_StartTime[0]);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_MONENDTIME	, m_EndTime[0]);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_TUESTARTTIME	, m_StartTime[1]);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_TUEENDTIME	, m_EndTime[1]);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_WEDSTARTTIME	, m_StartTime[2]);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_WEDENDTIME	, m_EndTime[2]);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_THUSTARTTIME	, m_StartTime[3]);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_THUENDTIME	, m_EndTime[3]);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_FRISTARTTIME	, m_StartTime[4]);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_FRIENDTIME	, m_EndTime[4]);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_SATSTARTTIME	, m_StartTime[5]);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_SATENDTIME	, m_EndTime[5]);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_SUNSTARTTIME	, m_StartTime[6]);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_SUNENDTIME	, m_EndTime[6]);
	DDX_Text(pDX, IDC_EDIT_MONBCU, m_nCountBCU[0]);
	DDX_Text(pDX, IDC_EDIT_MONANI, m_nCountANI[0]);
	DDX_Text(pDX, IDC_EDIT_TUEBCU, m_nCountBCU[1]);
	DDX_Text(pDX, IDC_EDIT_TUEANI, m_nCountANI[1]);
	DDX_Text(pDX, IDC_EDIT_WEDBCU, m_nCountBCU[2]);
	DDX_Text(pDX, IDC_EDIT_WEDANI, m_nCountANI[2]);
	DDX_Text(pDX, IDC_EDIT_THUBCU, m_nCountBCU[3]);
	DDX_Text(pDX, IDC_EDIT_THUANI, m_nCountANI[3]);
	DDX_Text(pDX, IDC_EDIT_FRIBCU, m_nCountBCU[4]);
	DDX_Text(pDX, IDC_EDIT_FRIANI, m_nCountANI[4]);
	DDX_Text(pDX, IDC_EDIT_SATBCU, m_nCountBCU[5]);
	DDX_Text(pDX, IDC_EDIT_SATANI, m_nCountANI[5]);
	DDX_Text(pDX, IDC_EDIT_SUNBCU, m_nCountBCU[6]);
	DDX_Text(pDX, IDC_EDIT_SUNANI, m_nCountANI[6]);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID_STRATEGYPOINT_TODAY_LIST, m_GridTodayStrategyPointListInfo);
}


BEGIN_MESSAGE_MAP(CRenewalStrategyPointAdminDlg, CDialog)
	//{{AFX_MSG_MAP(CRenewalStrategyPointAdminDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON_STRATEGYPOINT_APPLY, OnButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_STRATEGYPOINT_SAVE, OnButtonSave)
	ON_BN_CLICKED(ID_MENUITEM_SUMMON, OnButtonMenuSummon)
	ON_BN_CLICKED(ID_MENUITEM_CANCEL, OnButtonMenuCancel)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_GRID_STRATEGYPOINT_TODAY_LIST, OnGridClick)
	ON_NOTIFY(NM_RCLICK, IDC_GRID_STRATEGYPOINT_TODAY_LIST, OnGridRClick)
END_MESSAGE_MAP()

const char *g_arrGridTodayStrategyPointListInfoColumnNames[] = {
	"Map Name","Map Index","Summon Time","Influence","Creation",	NULL
};

BOOL CRenewalStrategyPointAdminDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

/*
	m_pStrategyPointAdminPreSocket = new CSCAdminPreWinSocket("CSCStrategyPointAdminDlg's PreServer Socket", this, GetSafeHwnd());
	if (!m_pStrategyPointAdminPreSocket->Connect(m_pMainDlg->m_pServerInfo4Admin->ServerIP, PRE_SERVER_PORT))
	{
		int err = GetLastError();
		MessageBox(STRERR_S_SCADMINTOOL_0005);
		util::del(m_pStrategyPointAdminPreSocket);
		EndDialog(-1);
		return FALSE;
	}
*/

	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
		m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
		)
	{
		MessageBox(STRERR_S_SCADMINTOOL_0013);
		EndDialog(-1);
		return FALSE;
	}
//	InitGridColumn();
	UpdateGridItem();
	LoadStrategyPointMapInfluenceInfo();
	LoadStrategyPointSummonTimeInfo();
	ListLoad();
	UpdateData(FALSE);
	return TRUE;
}

void CRenewalStrategyPointAdminDlg::InitGridColumn()
{
	m_GridTodayStrategyPointListInfo.SetBkColor(0xFFFFFF);
	m_GridTodayStrategyPointListInfo.SetEditable(FALSE);
	m_GridTodayStrategyPointListInfo.SetListMode(TRUE);
	m_GridTodayStrategyPointListInfo.EnableSelection(TRUE);
	m_GridTodayStrategyPointListInfo.SetHeaderSort(FALSE);
	m_GridTodayStrategyPointListInfo.SetFrameFocusCell(FALSE);
	m_GridTodayStrategyPointListInfo.SetTrackFocusCell(FALSE);
	m_GridTodayStrategyPointListInfo.SetRowCount(1);
	m_GridTodayStrategyPointListInfo.SetColumnCount(5);
	m_GridTodayStrategyPointListInfo.SetFixedRowCount(1);
	
	int m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = GRID_CELL_FORMAT;
	
	Item.col = m_nCols++;
	Item.strText.Format(_T("Map Name"));
	m_GridTodayStrategyPointListInfo.SetItem(&Item);
	
	Item.col = m_nCols++;
	Item.strText.Format(_T("Map Index"));
	m_GridTodayStrategyPointListInfo.SetItem(&Item);
	
	Item.col = m_nCols++;
	Item.strText.Format(_T("Summun Time"));
	m_GridTodayStrategyPointListInfo.SetItem(&Item);
	
	Item.col = m_nCols++;
	Item.strText.Format(_T("Influence"));
	m_GridTodayStrategyPointListInfo.SetItem(&Item);
	
	Item.col = m_nCols++;
	Item.strText.Format(_T("Creation"));
	m_GridTodayStrategyPointListInfo.SetItem(&Item);
	
	m_GridTodayStrategyPointListInfo.AutoSize();
	m_GridTodayStrategyPointListInfo.ExpandColumnsToFit();
}

void CRenewalStrategyPointAdminDlg::LoadStrategyPointMapInfluenceInfo()
{
	/*[Stored Query Definition]************************************************
	=============================================
	-- Author:		atum_LoadStrategyPointMapInfluenceInfo
	-- Create date: // 2012-11-13 by jhseol, ���� �ý��� ������ - ������ �� �߰�
	-- Description:	�������� �������� ��������
	-- =============================================
	CREATE PROCEDURE atum_LoadStrategyPointMapInfluenceInfo
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
	AS
	BEGIN
		SELECT [MapIndex], [MapName], [MapInfluenceType]
		FROM [atum2_db_account].[dbo].[ti_MapInfo]
		WHERE [MapIndex] IN (SELECT [MapIndex] FROM [atum2_db_account].[dbo].[ti_StrategyPointSummonMapIndex])
	END
	GO
	**************************************************************************/
	RETCODE	ret = m_pODBCStmt->ExecuteQuery(PROCEDURE_121113_0004);
	if (FALSE == ret)
	{
		MessageBox("DB Execute Error !!(atum_LoadStrategyPointMapInfluenceInfo)");
		m_pODBCStmt->FreeStatement();
		return;
	}
	
	SQLINTEGER	arrCB[4] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	SSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO	tmStrategyPointMapInfluenceInfo;
	util::zero(&tmStrategyPointMapInfluenceInfo, sizeof(SSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO));
	
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &tmStrategyPointMapInfluenceInfo.MapIndex, 0, &arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_CHAR, tmStrategyPointMapInfluenceInfo.MapName, SIZE_MAX_MAP_NAME, &arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_SHORT, &tmStrategyPointMapInfluenceInfo.Influence, 0, &arrCB[3]);
	
	while ( (ret = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		m_SStrategyPointMapInfluenceInfo.push_back(tmStrategyPointMapInfluenceInfo);
		util::zero(&tmStrategyPointMapInfluenceInfo, sizeof(SSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO));
	}
	m_pODBCStmt->FreeStatement();

	int tmStrategyPointCountBCU = 0;
	int tmStrategyPointCountANI = 0;
	vectSSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO::iterator itr = m_SStrategyPointMapInfluenceInfo.begin();
	for ( ; itr < m_SStrategyPointMapInfluenceInfo.end() ; itr++ )
	{
		if( IS_MAP_INFLUENCE_VCN(itr->Influence) )
		{
			tmStrategyPointCountBCU++;
		}
		else
		{
			tmStrategyPointCountANI++;
		}
	}

	m_nMaxStrategyPointCount = min(tmStrategyPointCountBCU, tmStrategyPointCountANI);
}

void CRenewalStrategyPointAdminDlg::LoadStrategyPointSummonTimeInfo()
{
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: atum_LoadRenewalStrategyPointSummonTime
	-- DESC      : ���Ϻ� �ð� �� ���� ���� �������� ���ν���
	-- // 2012-11-13 by jhseol, ���� �ý��� ������
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_LoadRenewalStrategyPointSummonTime
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
	AS
		SELECT DayOfWeek, StartTime, EndTime, CountBCU, CountANI FROM td_RenewalStrategyPointSummonTime WITH(NOLOCK)
	GO
	**************************************************************************/
	RETCODE	ret = m_pODBCStmt->ExecuteQuery(PROCEDURE_121113_0001);
	if (FALSE == ret)
	{
		MessageBox("DB Execute Error !!(atum_LoadRenewalStrategyPointSummonTime)");
		m_pODBCStmt->FreeStatement();
		return;
	}

	SQLINTEGER	arrCB[6] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	SRENEWAL_STRATEGYPOINT_SUMMON_TIME	tmStrategyPointNotSummonTime;
	util::zero(&tmStrategyPointNotSummonTime, sizeof(SRENEWAL_STRATEGYPOINT_SUMMON_TIME));
	SQL_TIMESTAMP_STRUCT	tmSTime;
	SQL_TIMESTAMP_STRUCT	tmETime;

	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_TINYINT, &tmStrategyPointNotSummonTime.DayOfWeek, 0, &arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_TIMESTAMP, &tmSTime, 0, &arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_TIMESTAMP, &tmETime, 0, &arrCB[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_TINYINT, &tmStrategyPointNotSummonTime.CountBCU, 0, &arrCB[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_TINYINT, &tmStrategyPointNotSummonTime.CountANI, 0, &arrCB[5]);
	
	while ( (ret = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		tmStrategyPointNotSummonTime.StartTime	=	tmSTime;
		tmStrategyPointNotSummonTime.EndTime	=	tmETime;

		///////////////////////////////////////////////////////////////////////////////
		// 2008-08-21 by cmkwon, �ð��� ��������� �⺻ ��¥�� ���� ��¥�� ������.
		ATUM_DATE_TIME atCur { true };
		tmStrategyPointNotSummonTime.StartTime.Year		= tmStrategyPointNotSummonTime.EndTime.Year		= atCur.Year;
		tmStrategyPointNotSummonTime.StartTime.Month	= tmStrategyPointNotSummonTime.EndTime.Month	= atCur.Month;
		tmStrategyPointNotSummonTime.StartTime.Day		= tmStrategyPointNotSummonTime.EndTime.Day		= atCur.Day;

		
		m_StartTime[tmStrategyPointNotSummonTime.DayOfWeek].SetDateTime(tmStrategyPointNotSummonTime.StartTime.Year, tmStrategyPointNotSummonTime.StartTime.Month, tmStrategyPointNotSummonTime.StartTime.Day,
																		tmStrategyPointNotSummonTime.StartTime.Hour, tmStrategyPointNotSummonTime.StartTime.Minute, 0);
		m_EndTime[tmStrategyPointNotSummonTime.DayOfWeek].SetDateTime(tmStrategyPointNotSummonTime.EndTime.Year, tmStrategyPointNotSummonTime.EndTime.Month, tmStrategyPointNotSummonTime.EndTime.Day,
																		tmStrategyPointNotSummonTime.EndTime.Hour, tmStrategyPointNotSummonTime.EndTime.Minute, 0);
		m_nCountBCU[tmStrategyPointNotSummonTime.DayOfWeek] = tmStrategyPointNotSummonTime.CountBCU;
		m_nCountANI[tmStrategyPointNotSummonTime.DayOfWeek] = tmStrategyPointNotSummonTime.CountANI;
		util::zero(&tmStrategyPointNotSummonTime, sizeof(SRENEWAL_STRATEGYPOINT_SUMMON_TIME));
	}

	m_pODBCStmt->FreeStatement();	
}

/////////////////////////////////////////////////////////////////////////////
// CRenewalStrategyPointAdminDlg message handlers
void CRenewalStrategyPointAdminDlg::OnButtonApply() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	ATUM_DATE_TIME CurrentTime;
	CurrentTime.SetCurrentDateTime();
	auto localStTime = tm(CurrentTime);
	SettingRandumSummonTime(BYTE(tm(CurrentTime).tm_wday));

	
	UpdateData(FALSE);
//	MessageBox("Save Success!!");
}

void CRenewalStrategyPointAdminDlg::SettingRandumSummonTime(BYTE i_nWeek)
{
	SummonInfoErrorCheck();
	//InitGridColumn();
	UpdateData(FALSE);

	ATUM_DATE_TIME StartTime { true };
	ATUM_DATE_TIME EndTime { true };

	////////////////////////////////////////////////////////////////////////
	// ���� ���Ͽ� �´� ���� �ð��� �����´�
	StartTime.SetDateTime(StartTime.Year, StartTime.Month, StartTime.Day, m_StartTime[i_nWeek].GetHour(), m_StartTime[i_nWeek].GetMinute(), 0);
	EndTime.SetDateTime(EndTime.Year, EndTime.Month, EndTime.Day, m_EndTime[i_nWeek].GetHour(), m_EndTime[i_nWeek].GetMinute(), 0);

	////////////////////////////////////////////////////////////////////////
	// ���� ���� ���� �ð����� ����
	int TotalSummonTime = EndTime.GetTimeDiffTimeInSeconds(StartTime);
	int MaxSummonTimeGap = TotalSummonTime;
	
	////////////////////////////////////////////////////////////////////////
	// ���� ���� ���� �ð��븦 ������ ������ ���� ���� 1���� ������ �����ؾ��� ��� �ð��� ����Ѵ�
	int SettingSummonStrategyPointCount = m_nCountBCU[i_nWeek] + m_nCountANI[i_nWeek];
	if( 0 < SettingSummonStrategyPointCount )
	{
		MaxSummonTimeGap = TotalSummonTime / SettingSummonStrategyPointCount;
	}

	////////////////////////////////////////////////////////////////////////
	// ��� �ð����� ���� �ּ� �� 30���� ���ְ� ���� ���� �ð������� ����Ѵ�. ���⼭ �� 30���� �Ŀ� �ٽ� �����ش�.
	MaxSummonTimeGap -= RenewalStrategyPointSummonTimeTermMin;		// 2013-03-18 by jhseol, ���� �����ð� ���� - ���� ������ ���۽ð� ���� ���� �ϱ� ���� �ּ����� ���ش�
	if( 0 > MaxSummonTimeGap )
	{
		MaxSummonTimeGap = 0;
	}

/*
	char str[1024];
	sprintf(str, "���� : %d \r\n ���� �ð� ���� ���� : 0~%d(��)", i_nWeek, MaxSummonTimeGap/60);
	MessageBox(str);
*/


	////////////////////////////////////////////////////////////////////////
	// ���� �����ð� �ӽ� ���� ����
	vector<ATUM_DATE_TIME> tmRandomSummonTime;
	tmRandomSummonTime.clear();

	////////////////////////////////////////////////////////////////////////
	// ������ ������ ����ŭ ������ �Ѵ�.
	int forloopindex = 0;
	for( ; forloopindex < SettingSummonStrategyPointCount ; forloopindex++ )
	{
		int RandomSummonTimeGap = RANDI(0, MaxSummonTimeGap);
		StartTime.AddDateTime(0,0,0,0,0,RandomSummonTimeGap);
		tmRandomSummonTime.push_back(StartTime);

		if (S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_SUMMON_TIME_VARIANCE_JHSEOL)
			
			StartTime.AddDateTime(0,0,0,0,0,RenewalStrategyPointSummonTimeTermMin+MaxSummonTimeGap-RandomSummonTimeGap);
			
		else StartTime.AddDateTime(0,0,0,0,0,RenewalStrategyPointSummonTimeTermMin);


	}
	
	////////////////////////////////////////////////////////////////////////
	// ���� ������ ����� ������ ���� ����ŭ ����Ʈ�� �߰��Ѵ�.
	m_vectStrategyPointSummonTimeInfo.clear();
	SSTRATEGYPOINT_SUMMONTIME_INFO tmSSTRATEGYPOINT_SUMMONTIME_INFO;
	util::zero(&tmSSTRATEGYPOINT_SUMMONTIME_INFO, sizeof(SSTRATEGYPOINT_SUMMONTIME_INFO));
	vectSSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO::iterator tmmap_itr = m_SStrategyPointMapInfluenceInfo.begin();
	for ( ; tmmap_itr < m_SStrategyPointMapInfluenceInfo.end() ; tmmap_itr++ )
	{
		tmSSTRATEGYPOINT_SUMMONTIME_INFO.MapIndex = tmmap_itr->MapIndex;
		m_vectStrategyPointSummonTimeInfo.push_back(tmSSTRATEGYPOINT_SUMMONTIME_INFO);
	}
	////////////////////////////////////////////////////////////////////////
	// ��������Ʈ���� ���� ���ؿ� ������� ���Ƿ� ���´�.
	random_shuffle(m_vectStrategyPointSummonTimeInfo.begin(), m_vectStrategyPointSummonTimeInfo.end());
	vectSSTRATEGYPOINT_SUMMONTIME_INFO::iterator shuffle_itr = m_vectStrategyPointSummonTimeInfo.begin();
	int tmRandomSummonTimeIndex = 0;		// �ӽ� ����� ���� ���� ������ �ε���
	int tmBCUSummonCount = 0;				// ���� �Ϸ�� BCU ������ ���� ��
	int tmANISummonCount = 0;				// ���� �Ϸ�� ANI ������ ���� ��
	for ( ; shuffle_itr < m_vectStrategyPointSummonTimeInfo.end() ; shuffle_itr++ )
	{
		////////////////////////////////////////////////////////////////////////
		// ������ �ٽ� ���� ������ ���� �ð��� ��ȯ ���θ� �ʱ�ȭ �Ѵ�.
		util::zero(&shuffle_itr->SummonTime, sizeof(ATUM_DATE_TIME));
		shuffle_itr->SummonAttribute = FALSE;
		shuffle_itr->SummonCount = 0;

		////////////////////////////////////////////////////////////////////////
		// ������ �ʹ�ȣ�� ������ ���ؼ� �� ���¿� ���õ� ������ ����ŭ�� ������ �ǵ��� �Ѵ�.
		vectSSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO::iterator map_itr = m_SStrategyPointMapInfluenceInfo.begin();
		for ( ; map_itr < m_SStrategyPointMapInfluenceInfo.end() ; map_itr++ )
		{
			if ( shuffle_itr->MapIndex == map_itr->MapIndex )
			{
				if( IS_MAP_INFLUENCE_VCN(map_itr->Influence) )
				{
					////////////////////////////////////////////////////////////////////////
					// BCU������ ���� �Ϸ� �� ���� ���� ������ ������ ���� ���Ѵ�.
					if( tmBCUSummonCount < m_nCountBCU[i_nWeek] )
					{
						////////////////////////////////////////////////////////////////////////
						// BCU���¿� ���� ���õ��� ���� ������ ������ �ǹ������� �����ϰ� ���� �Ϸ�� BCU ���� ������ ����
						shuffle_itr->SummonAttribute = TRUE;
						shuffle_itr->SummonCount = 1;
						tmBCUSummonCount++;
					}
				}
				else
				{
					////////////////////////////////////////////////////////////////////////
					// ANI������ ���� �Ϸ� �� ���� ���� ������ ������ ���� ���Ѵ�.
					if( tmANISummonCount < m_nCountANI[i_nWeek] )
					{
						////////////////////////////////////////////////////////////////////////
						// ANI���¿� ���� ���õ��� ���� ������ ������ �ǹ������� �����ϰ� ���� �Ϸ�� ANI ���� ������ ����
						shuffle_itr->SummonAttribute = TRUE;
						shuffle_itr->SummonCount = 1;
						tmANISummonCount++;
					}
				}
			}
		}
		if( 0 < shuffle_itr->SummonCount )
		{
			////////////////////////////////////////////////////////////////////////
			// ���� �Ϸ� �� ������ �� ���� �ӽ÷� ������ ������ ���� �ð��� ũ�⸦ ���Ѵ�.
			if( tmRandomSummonTimeIndex < tmRandomSummonTime.size() )
			{
				////////////////////////////////////////////////////////////////////////
				// ���� ���õ��� ���� ������ ������ �ǹ��Ѵ�.
				shuffle_itr->SummonTime = tmRandomSummonTime[tmRandomSummonTimeIndex];
				tmRandomSummonTimeIndex++;
			}
		}
//		InsertGridItem(*shuffle_itr);
	}
	UpdateGridItem();

	m_GridTodayStrategyPointListInfo.AutoSize();
	m_GridTodayStrategyPointListInfo.ExpandColumnsToFit();

}

void CRenewalStrategyPointAdminDlg::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	SummonInfoErrorCheck();
	DBUpdateWeekdayStrategyPointSummonTime();
	DBUpdateStrategyPointSummonInfo();

	INIT_MSG_WITH_BUFFER(MSG_PA_ADMIN_STRATRGYPOINT_INFO_CHANGE, T_PA_ADMIN_STRATRGYPOINT_INFO_CHANGE, pSendMsg, SendBuf);

	util::strncpy(pSendMsg->DBName, m_pMainDlg->m_pServerInfo4Admin->DBName, sizeof(pSendMsg->DBName));
	
	if( TRUE != m_pStrategyPointAdminPreSocket->Write(SendBuf, MSG_SIZE(MSG_PA_ADMIN_STRATRGYPOINT_INFO_CHANGE)) )
	{
		MessageBox("Save Fail!!");
	}
	else
	{
		MessageBox("Save Success!!");
	}
	CDialog::OnCancel();	// 2012-12-03 by jhseol, ���� �ý��� ������ - ������ ���� �� �� �ڵ� ����
}

void CRenewalStrategyPointAdminDlg::DBUpdateWeekdayStrategyPointSummonTime()
{
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: atum_UpdateRenewalStrategyPointSummonTime
	-- DESC      : ���Ϻ� �ð� �� ���� ���� ������Ʈ ���ν���
	-- // 2012-11-13 by jhseol, ���� �ý��� ������
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_UpdateRenewalStrategyPointSummonTime
		@i_DayOfWeek	TINYINT,
		@i_StartTime	VARCHAR(30),
		@i_EndTime		VARCHAR(30),
		@i_CountBCU		TINYINT,
		@i_CountANI		TINYINT
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
	AS
		UPDATE td_RenewalStrategyPointSummonTime SET StartTime = @i_StartTime, EndTime = @i_EndTime, CountBCU = @i_CountBCU, CountANI = @i_CountANI WHERE DayOfWeek = @i_DayOfWeek
	GO
	**************************************************************************/
	char tmStartTime[RenewalStrategyPointWeekDay][SIZE_MAX_SQL_DATETIME_STRING];
	char tmEndTime[RenewalStrategyPointWeekDay][SIZE_MAX_SQL_DATETIME_STRING];
	BOOL tmSummonCheck[RenewalStrategyPointWeekDay];
	util::zero(tmStartTime, sizeof(tmStartTime));
	util::zero(tmEndTime, sizeof(tmEndTime));
	util::zero(tmSummonCheck, sizeof(tmSummonCheck));
	int forloopindex = 0;
	for ( ; forloopindex < RenewalStrategyPointWeekDay ; forloopindex++ )
	{
		sprintf(tmStartTime[forloopindex], "%02d:%02d:0.000", m_StartTime[forloopindex].GetHour(), m_StartTime[forloopindex].GetMinute());
		sprintf(tmEndTime[forloopindex], "%02d:%02d:0.000", m_EndTime[forloopindex].GetHour(), m_EndTime[forloopindex].GetMinute());
		
		SQLBindParameter(m_pODBCStmt->m_hstmt, 1	, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &forloopindex, 0, NULL);
		SQLBindParameter(m_pODBCStmt->m_hstmt, 2	, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmStartTime[forloopindex], 0, NULL);
		SQLBindParameter(m_pODBCStmt->m_hstmt, 3	, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmEndTime[forloopindex], 0, NULL);
		SQLBindParameter(m_pODBCStmt->m_hstmt, 4	, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &m_nCountBCU[forloopindex], 0, NULL);
		SQLBindParameter(m_pODBCStmt->m_hstmt, 5	, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &m_nCountANI[forloopindex], 0, NULL);

		/********************************************************************************
		--------------------------------------------------------------------------------
		-- PROCEDURE NAME	: atum_UpdateRenewalStrategyPointSummonTime
		-- DESC				: ���Ϻ� �ð� �� ���� ���� ������Ʈ ���ν���
		-- // 2012-11-13 by jhseol, ���� �ý��� ������
		--------------------------------------------------------------------------------
		CREATE PROCEDURE dbo.atum_UpdateRenewalStrategyPointSummonTime
		@i_DayOfWeek	TINYINT,
		@i_StartTime	VARCHAR(30),
		@i_EndTime		VARCHAR(30),
		@i_CountBCU		TINYINT,
		@i_CountANI		TINYINT
		-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
		AS
		UPDATE td_RenewalStrategyPointSummonTime SET StartTime = @i_StartTime, EndTime = @i_EndTime, CountBCU = @i_CountBCU, CountANI = @i_CountANI WHERE DayOfWeek = @i_DayOfWeek
		GO
		********************************************************************************/
		RETCODE	ret = m_pODBCStmt->ExecuteQuery(PROCEDURE_121113_0002);
		if (FALSE == ret)
		{
			MessageBox("DB Execute Error !!(atum_UpdateRenewalStrategyPointSummonTime)");
			m_pODBCStmt->FreeStatement();
			return;
		}
		m_pODBCStmt->FreeStatement();
	}

}

void CRenewalStrategyPointAdminDlg::DBUpdateStrategyPointSummonInfo()
{
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: atum_UpdateRenewalStrategyPointSummonInfo
	-- DESC      : ������ ���� ���� ������Ʈ
	-- // 2012-11-13 by jhseol, ���� �ý��� ������
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_UpdateRenewalStrategyPointSummonInfo
	@i_MapIndex			INT,
	@i_SummonCount		TINYINT,
	@i_SummonTime		VARCHAR(30),
	@i_SummonAttribute	TINYINT
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
	AS
	IF IS NULL (SELECT MapIndex FROM dbo.td_StrategyPointSummonInfo WHERE MapIndex = @i_MapIndex)
	BEGIN
	INSERT INTO dbo.td_StrategyPointSummonInfo(MapIndex, SummonCount, SummonTime, SummonAttribute)
	VALUES(@i_MapIndex, '19000101 00:00:00.000', 0, 0)
	END
	
	  IF (@i_SummonTime like '0000%')
	  BEGIN
	  UPDATE dbo.td_StrategyPointSummonInfo SET SummonCount = @i_SummonCount, SummonTime = '19000101 00:00:00.000', SummonAttribute = @i_SummonAttribute WHERE MapIndex = @i_MapIndex
	  END
	  ELSE
	  BEGIN
	  UPDATE dbo.td_StrategyPointSummonInfo SET SummonCount = @i_SummonCount, SummonTime = @i_SummonTime, SummonAttribute = @i_SummonAttribute WHERE MapIndex = @i_MapIndex
	  END
	  GO
	**************************************************************************/
	vectSSTRATEGYPOINT_SUMMONTIME_INFO::iterator Insert_Date = m_vectStrategyPointSummonTimeInfo.begin();
	for ( ; Insert_Date < m_vectStrategyPointSummonTimeInfo.end() ; Insert_Date++ )
	{
		char tmpSummonTimeBuf[SIZE_MAX_SQL_DATETIME_STRING];
		Insert_Date->SummonTime.GetSQLDateTimeString(tmpSummonTimeBuf, SIZE_MAX_SQL_DATETIME_STRING);

		SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT,	0, 0, &Insert_Date->MapIndex, 0, NULL);
		SQLBindParameter(m_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &Insert_Date->SummonCount, 0, NULL);
		SQLBindParameter(m_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpSummonTimeBuf, 0, NULL);
		SQLBindParameter(m_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &Insert_Date->SummonAttribute, 0, NULL);

		/********************************************************************************
		--------------------------------------------------------------------------------
		-- PROCEDURE NAME	: atum_UpdateRenewalStrategyPointSummonInfo
		-- DESC				: ������ ���� ���� ������Ʈ
		-- // 2012-11-13 by jhseol, ���� �ý��� ������
		--------------------------------------------------------------------------------
		CREATE PROCEDURE dbo.atum_UpdateRenewalStrategyPointSummonInfo
		@i_MapIndex			INT,
		@i_SummonCount		TINYINT,
		@i_SummonTime		VARCHAR(30),
		@i_SummonAttribute	TINYINT
		-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
		AS
		IF (SELECT MapIndex FROM dbo.td_StrategyPointSummonInfo WHERE MapIndex = @i_MapIndex) IS NULL
		BEGIN
		INSERT INTO dbo.td_StrategyPointSummonInfo(MapIndex, SummonCount, SummonTime, SummonAttribute)
		VALUES(@i_MapIndex, '19000101 00:00:00.000', 0, 0)
		END
		
		  IF (@i_SummonTime like '0000%')
		  BEGIN
		  UPDATE dbo.td_StrategyPointSummonInfo SET SummonCount = @i_SummonCount, SummonTime = '19000101 00:00:00.000', SummonAttribute = @i_SummonAttribute WHERE MapIndex = @i_MapIndex
		  END
		  ELSE
		  BEGIN
		  UPDATE dbo.td_StrategyPointSummonInfo SET SummonCount = @i_SummonCount, SummonTime = @i_SummonTime, SummonAttribute = @i_SummonAttribute WHERE MapIndex = @i_MapIndex
		  END
		GO
		********************************************************************************/
		RETCODE	ret = m_pODBCStmt->ExecuteQuery(PROCEDURE_121113_0003);
		if (FALSE == ret)
		{
			MessageBox("DB Execute Error !!(atum_UpdateRenewalStrategyPointSummonInfo)");
			m_pODBCStmt->FreeStatement();
			return;
		}
		m_pODBCStmt->FreeStatement();
	}
}

void CRenewalStrategyPointAdminDlg::ListLoad() 
{
	//InitGridColumn();
	m_vectStrategyPointSummonTimeInfo.clear();
	// TODO: Add your control notification handler code here
		/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_LoadAllStrategyPointSummonInfo
	-- DESC      : �����ؾ� �� ��������Ʈ ���� ��ü �������� ���ν��� (SCAdminTool���� ���)
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_LoadAllStrategyPointSummonInfo
	AS
		SELECT MapIndex, SummonCount, SummonTime, SummonAttribute FROM dbo.td_StrategyPointSummonInfo WITH (NOLOCK) ORDER BY SummonTime
	GO
	**************************************************************************/
	RETCODE	ret = m_pODBCStmt->ExecuteQuery(PROCEDURE_080822_0018);
	if (FALSE == ret)
	{
		MessageBox("DB Execute Error !!(atum_LoadAllStrategyPointSummonInfo)");
		m_pODBCStmt->FreeStatement();
		return;
	}

	SQLINTEGER	arrCB[5] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	SSTRATEGYPOINT_SUMMONTIME_INFO	tmStrategyPointSummonTimeInfo;
	util::zero(&tmStrategyPointSummonTimeInfo, sizeof(SSTRATEGYPOINT_SUMMONTIME_INFO));
	SQL_TIMESTAMP_STRUCT	tmTime;

	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &tmStrategyPointSummonTimeInfo.MapIndex, 0, &arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_UTINYINT, &tmStrategyPointSummonTimeInfo.SummonCount, 0, &arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_TIMESTAMP, &tmTime, 0, &arrCB[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_UTINYINT, &tmStrategyPointSummonTimeInfo.SummonAttribute, 0, &arrCB[4]);
	
	while ( (ret = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		tmStrategyPointSummonTimeInfo.SummonTime	=	tmTime;
		m_vectStrategyPointSummonTimeInfo.push_back(tmStrategyPointSummonTimeInfo);
//		InsertGridItem(tmStrategyPointSummonTimeInfo);
		util::zero(&tmStrategyPointSummonTimeInfo, sizeof(SSTRATEGYPOINT_SUMMONTIME_INFO));
	}

	UpdateGridItem();

	m_pODBCStmt->FreeStatement();
	m_GridTodayStrategyPointListInfo.AutoSize();
	m_GridTodayStrategyPointListInfo.ExpandColumnsToFit();
}

void CRenewalStrategyPointAdminDlg::InsertGridItem(SSTRATEGYPOINT_SUMMONTIME_INFO i_item)
{
	int m_nCols = 0;
	char szTemp[50]={0x00,};
	char MapName[SIZE_MAX_MAP_NAME]={0x00,};
	char Influence[5]={0x00,};
	m_GridTodayStrategyPointListInfo.SetRowCount(m_GridTodayStrategyPointListInfo.GetRowCount()+1);
	vectSSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO::iterator itr = m_SStrategyPointMapInfluenceInfo.begin();
	for ( ; itr < m_SStrategyPointMapInfluenceInfo.end() ; itr++ )
	{
		if( i_item.MapIndex == itr->MapIndex )
		{
			util::strncpy(MapName, itr->MapName, sizeof(MapName));
			if( IS_MAP_INFLUENCE_VCN(itr->Influence) )
			{
				util::strncpy(Influence, "BCU", sizeof(Influence));
			}
			else
			{
				util::strncpy(Influence, "ANI", sizeof(Influence));
			}
			break;
		}
	}

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = m_GridTodayStrategyPointListInfo.GetRowCount()-1;
	Item.nFormat = ((DWORD)DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	
	Item.col = m_nCols++;
	Item.strText.Format("%s", MapName);
	m_GridTodayStrategyPointListInfo.SetItem(&Item);
	
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_item.MapIndex);
	m_GridTodayStrategyPointListInfo.SetItem(&Item);
	
	Item.col = m_nCols++;
	Item.strText.Format("%s", i_item.SummonTime.GetDateTimeString().GetBuffer());
	m_GridTodayStrategyPointListInfo.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%s", Influence);
	m_GridTodayStrategyPointListInfo.SetItem(&Item);

	Item.col = m_nCols++;
	if( 0 < i_item.SummonCount )
	{
		Item.strText = "Waiting";
	}
	else
	{
		Item.strText = "Finish";
	}
	m_GridTodayStrategyPointListInfo.SetItem(&Item);
}

void CRenewalStrategyPointAdminDlg::UpdateGridItem()
{
	InitGridColumn();
	vectSSTRATEGYPOINT_SUMMONTIME_INFO::iterator Insert_Date = m_vectStrategyPointSummonTimeInfo.begin();
	for ( ; Insert_Date < m_vectStrategyPointSummonTimeInfo.end() ; Insert_Date++ )
	{
		InsertGridItem(*Insert_Date);
	}
}

void CRenewalStrategyPointAdminDlg::OnGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	
	if (pItem->iRow != 0)
	{
		return;
	}
	
    if (pItem->iColumn == m_GridTodayStrategyPointListInfo.GetSortColumn())
	{
        m_GridTodayStrategyPointListInfo.SortTextItems(pItem->iColumn, !m_GridTodayStrategyPointListInfo.GetSortAscending());
	}
    else
	{
        m_GridTodayStrategyPointListInfo.SortTextItems(pItem->iColumn, TRUE);
	}
	
	m_GridTodayStrategyPointListInfo.AutoSize();
	m_GridTodayStrategyPointListInfo.ExpandColumnsToFit();
}

void CRenewalStrategyPointAdminDlg::OnGridRClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
//	OutputDebugString("[SEOL Debuging Check] : Mouse R Click. \r\n");
    NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;
	if (pGridView->iRow == 0)
	{// 2006-08-25 by cmkwon, ������ ����
		return;
	}
	
	m_GridTodayStrategyPointListInfo.SetFocusCell(-1,-1);
	m_GridTodayStrategyPointListInfo.SetSelectedRange(pGridView->iRow, 0, pGridView->iRow, m_GridTodayStrategyPointListInfo.GetColumnCount()-1, TRUE, TRUE);
	m_GridTodayStrategyPointListInfo.SetFocusCell(pGridView->iRow, pGridView->iColumn);

	CMenu muTemp, *pContextMenu;
	muTemp.LoadMenu(IDR_CONTEXT_MENU_STRATEGYPOINT);
	CPoint point;
	GetCursorPos(&point);
	if(pGridView->iRow > 0)
	{// 2006-08-25 by cmkwon, ��ȿ�� ���� ����
		pContextMenu = muTemp.GetSubMenu(0);
		pContextMenu->EnableMenuItem(ID_MENUITEM_SUMMON, TRUE);
		pContextMenu->EnableMenuItem(ID_MENUITEM_CANCEL, TRUE);
	}
	else
	{// 2006-08-25 by cmkwon, ��ȿ���� ���� ���� ����
		return;
	}
	m_nListSelectMapindex = _ttoi(m_GridTodayStrategyPointListInfo.GetItemText(pGridView->iRow, 1));
	vectSSTRATEGYPOINT_SUMMONTIME_INFO::iterator itr = m_vectStrategyPointSummonTimeInfo.begin();
	for ( ; itr < m_vectStrategyPointSummonTimeInfo.end() ; itr++ )
	{
		if( m_nListSelectMapindex == itr->MapIndex )
		{
			ATUM_DATE_TIME CurrentTime { true };
			if ( CurrentTime < itr->SummonTime )
			{
				if( 0 < itr->SummonCount )
				{
					pContextMenu->EnableMenuItem(ID_MENUITEM_CANCEL, FALSE);
				}
				else
				{
					pContextMenu->EnableMenuItem(ID_MENUITEM_SUMMON, FALSE);
				}
			}
			break;
		}
	}
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}

void CRenewalStrategyPointAdminDlg::ChangeSummonState(BOOL i_bFlag)
{
	//InitGridColumn();
	vectSSTRATEGYPOINT_SUMMONTIME_INFO::iterator itr = m_vectStrategyPointSummonTimeInfo.begin();
	for ( ; itr < m_vectStrategyPointSummonTimeInfo.end() ; itr++ )
	{
		if( m_nListSelectMapindex == itr->MapIndex )
		{
			if( TRUE == i_bFlag )
			{
				itr->SummonCount = 1;
				itr->SummonAttribute = TRUE;
			}
			else
			{
				itr->SummonCount = 0;
				itr->SummonAttribute = FALSE;
			}
		}
//		InsertGridItem(*itr);
	}

	UpdateGridItem();

	m_nListSelectMapindex = 0;
}

void CRenewalStrategyPointAdminDlg::ApplySameChangesToAll(BYTE i_nWeek)
{
	UpdateData(TRUE);
	if (i_nWeek < 0 || i_nWeek > 6)
	{
		// �߸��� ���ڰ� �׳� �����Ѵ�
		return;
	}
	int forloopindex = 0;
	for(forloopindex = 0 ; forloopindex < RenewalStrategyPointWeekDay ; forloopindex++)
	{
		m_StartTime[forloopindex]	= m_StartTime[i_nWeek];
		m_EndTime[forloopindex]		= m_EndTime[i_nWeek];
		m_nCountBCU[forloopindex]	= m_nCountBCU[i_nWeek];
		m_nCountANI[forloopindex]	= m_nCountANI[i_nWeek];
	}
	SummonInfoErrorCheck();
	UpdateData(FALSE);
}

void CRenewalStrategyPointAdminDlg::SummonInfoErrorCheck()
{
	int forloopindex = 0;
	for(forloopindex = 0 ; forloopindex < RenewalStrategyPointWeekDay ; forloopindex++)
	{
		m_StartTime[forloopindex].SetTime(m_StartTime[forloopindex].GetHour(), m_StartTime[forloopindex].GetMinute(), 0);
		m_EndTime[forloopindex].SetTime(m_EndTime[forloopindex].GetHour(), m_EndTime[forloopindex].GetMinute(), 0);
		if( m_StartTime[forloopindex] > m_EndTime[forloopindex] )	// �ð����� ���� ���� �ð��� ���� �ð����� ���ĸ� �������� 0���� ����
		{
			m_StartTime[forloopindex] = m_EndTime[forloopindex];
			m_nCountBCU[forloopindex] = 0;
			m_nCountANI[forloopindex] = 0;
		}
		else
		{
			ATUM_DATE_TIME tmStartTime { true };
			ATUM_DATE_TIME tmEndTime { true };
			tmStartTime.SetDateTime(tmStartTime.Year, tmStartTime.Month, tmStartTime.Day, m_StartTime[forloopindex].GetHour(), m_StartTime[forloopindex].GetMinute(), 0);
			tmEndTime.SetDateTime(tmEndTime.Year, tmEndTime.Month, tmEndTime.Day, m_EndTime[forloopindex].GetHour(), m_EndTime[forloopindex].GetMinute(), 0);
			//tmStartTime.SetDateTime(2012, 11, 14, m_StartTime[forloopindex].GetHour(), m_StartTime[forloopindex].GetMinute(), 0);
			//tmEndTime.SetDateTime(2012, 11, 14, m_EndTime[forloopindex].GetHour(), m_EndTime[forloopindex].GetMinute(), 0);

			int StartEndTimeGap = tmEndTime.GetTimeDiffTimeInSeconds(tmStartTime);
			int MaxSummonCountBySummonTime = 0;
			if( 0 < StartEndTimeGap )
			{
				MaxSummonCountBySummonTime = StartEndTimeGap / RenewalStrategyPointSummonTimeTermMin;	// ���� ������ �ּ� 30�з� ���������� ���� �ð��� ��ȯ ������ ������ ���� ���
				if( 0 < StartEndTimeGap % RenewalStrategyPointSummonTimeTermMin )
				{
					MaxSummonCountBySummonTime++;
				}
			}

			if( m_nCountBCU[forloopindex] + m_nCountANI[forloopindex] > MaxSummonCountBySummonTime )	// �缼�¿� ���õ� �������� �ִ� �������� �ʰ��ϸ� ������ �ʿ��ϴ�.
			{
				if( m_nCountBCU[forloopindex] == m_nCountANI[forloopindex] )							// �缼�� �������� ������ �������� ����
				{
					if (RANDBOOL())
					{
						m_nCountBCU[forloopindex] = MaxSummonCountBySummonTime/2;
						m_nCountANI[forloopindex] = MaxSummonCountBySummonTime - m_nCountBCU[forloopindex];
					}
					else
					{
						m_nCountANI[forloopindex] = MaxSummonCountBySummonTime/2;
						m_nCountBCU[forloopindex] = MaxSummonCountBySummonTime - m_nCountANI[forloopindex];
					}
				}
				else if( m_nCountBCU[forloopindex] > MaxSummonCountBySummonTime/2 && m_nCountANI[forloopindex] > MaxSummonCountBySummonTime/2 )	// �缼�� ��� ���� �������� �ִ������/2 �̻��̸� �缼�¸�� ����
				{
					if( m_nCountBCU[forloopindex] > m_nCountANI[forloopindex] )
					{
						m_nCountANI[forloopindex] = MaxSummonCountBySummonTime/2;
						m_nCountBCU[forloopindex] = MaxSummonCountBySummonTime-m_nCountANI[forloopindex];
					}
					else
					{
						m_nCountBCU[forloopindex] = MaxSummonCountBySummonTime/2;
						m_nCountANI[forloopindex] = MaxSummonCountBySummonTime-m_nCountBCU[forloopindex];
					}
				}
				else													// �缼�� ��� ���� �������� �ִ������/2 �̻��� �ƴϸ� �ʰ��� ������ �������� ����
				{
					if( m_nCountBCU[forloopindex] > m_nCountANI[forloopindex] )
					{
						m_nCountBCU[forloopindex] = MaxSummonCountBySummonTime-m_nCountANI[forloopindex];
					}
					else
					{
						m_nCountANI[forloopindex] = MaxSummonCountBySummonTime-m_nCountBCU[forloopindex];
					}
				}
			}
		}
		// ���������� ���õ� �������� ���������� ���� ���� ������ ũ�� �����Ѵ�. 
		if( m_nCountBCU[forloopindex] > m_nMaxStrategyPointCount )
		{
			m_nCountBCU[forloopindex] = m_nMaxStrategyPointCount;
		}
		if( m_nCountANI[forloopindex] > m_nMaxStrategyPointCount )
		{
			m_nCountANI[forloopindex] = m_nMaxStrategyPointCount;
		}
	}
}

void CRenewalStrategyPointAdminDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	ApplySameChangesToAll(0);
}

void CRenewalStrategyPointAdminDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	ApplySameChangesToAll(1);
}

void CRenewalStrategyPointAdminDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
	ApplySameChangesToAll(2);
}

void CRenewalStrategyPointAdminDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	ApplySameChangesToAll(3);
}

void CRenewalStrategyPointAdminDlg::OnButton5() 
{
	// TODO: Add your control notification handler code here
	ApplySameChangesToAll(4);
}

void CRenewalStrategyPointAdminDlg::OnButton6() 
{
	// TODO: Add your control notification handler code here
	ApplySameChangesToAll(5);
}

void CRenewalStrategyPointAdminDlg::OnButton7() 
{
	// TODO: Add your control notification handler code here
	ApplySameChangesToAll(6);
}

void CRenewalStrategyPointAdminDlg::OnButtonMenuSummon()
{
	ChangeSummonState(TRUE);
//	MessageBox("Summon!!");
}

void CRenewalStrategyPointAdminDlg::OnButtonMenuCancel()
{
	ChangeSummonState(FALSE);
//	MessageBox("Cancel!!");
}

BOOL CRenewalStrategyPointAdminDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
        // ���⿡ ����Ű ��� �ۼ�
		return TRUE;
    }
    else if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
        // ���⿡ ESCŰ ��� �ۼ�
		CDialog::OnCancel();
        return TRUE;
    }
    return CDialog::PreTranslateMessage(pMsg);
}
