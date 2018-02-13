// ImportExcelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "atummonitor.h"
#include "ImportExcelDlg.h"
#include "AtumDBManager.h"
#include "atumparam.h"
#include "ODBCStatement.h"

#include "CheckDBData.h"      // 2011-07-15 by shcho - ĳ�������� üũ �ý��� ����
#include "CheckItemDialog.h"  // 2011-07-15 by shcho - ĳ�������� üũ �ý��� ����

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImportExcelDlg dialog


CImportExcelDlg::CImportExcelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImportExcelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImportExcelDlg)
	//}}AFX_DATA_INIT
	
	// 2009. 10. 27 by jskim ����� Ȯ��
	m_nMaxTotalCount		= 0;
	m_nCurrentTotalCount	= 0;
	//end 2009. 10. 27 by jskim ����� Ȯ��
}


void CImportExcelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImportExcelDlg)
	DDX_Control(pDX, IDC_SELECT_DB, m_ComboServerList);
	DDX_Control(pDX, IDC_DB_TABLES, m_DBTables);
	DDX_Control(pDX, IDC_SELECT_EXCELFILE, m_Select_ExcelFile);
	DDX_GridControl(pDX, IDC_EXCELFILE_TABLES, m_ExcelTables);
	// 2009. 10. 27 by jskim ����� Ȯ��
	DDX_Control(pDX, IDC_RECORDPROGRESS, m_RecordProgress);
	DDX_Control(pDX, IDC_TOTALPROGRESS, m_TotalProgress);
	DDX_Control(pDX, IDC_TABLENAME, m_TableName);
	//end 2009. 10. 27 by jskim ����� Ȯ��
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImportExcelDlg, CDialog)
	//{{AFX_MSG_MAP(CImportExcelDlg)
	ON_BN_CLICKED(IDC_INSERT_DB, OnInsertDb)
	ON_BN_CLICKED(IDC_EXCELFILE_SEARCH, OnExcelfileSearch)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_EXCELFILE_TABLES, OnGridExcelTablesClick)
	// 2009. 10. 27 by jskim ����� Ȯ��
	ON_MESSAGE(WM_PROGRESSBAR_RANGE, OnUpdateExcelloadProgressRange)
	ON_MESSAGE(WM_PROGRESSBAR_POS, OnUpdateExcelloadProgressPos)
	//end 2009. 10. 27 by jskim ����� Ȯ��
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImportExcelDlg message handlers

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CImportExcelDlg::OnInsertDb()
/// \brief		// 2009-07-09 by jwlee, ���� ���� ���̺� ������ DB�� �о�ֱ� �� ���� - 
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CImportExcelDlg::OnInsertDb()
{
	UpdateData();
	if(-1 == m_ComboServerList.GetCurSel())
	{
		AfxMessageBox("First, you have to select valid GameServerGroup !!");
		return;
	}

	// TODO: Add your control notification handler code here
	// 2009-07-09 by jwlee, ���� ���� ���̺� ������ DB�� �о�ֱ� �� ���� -
	int nObjects = 0;
	int nlen = 0;
	int i;
	CString szServerName;
	GAME_SERVER_INFO_FOR_ADMIN GameServerInfo;		util::zero(&GameServerInfo, sizeof(GameServerInfo));
	CString dbResultInfo;    //db �������
	CString szError;
	vector<CString> tmVectTables; // ����ڰ� ������ ���̺�

	// ������ ���� ���� üũ
	m_ComboServerList.GetLBText(m_ComboServerList.GetCurSel(), szServerName);

	for (i = 0; g_arrGameServers[i].ServerName !=NULL; i++)
	{
		if (szServerName == g_arrGameServers[i].ServerName)
		{
			GameServerInfo = g_arrGameServers[i];
			break;
		}
	}
	if(NULL == GameServerInfo.ServerName)
	{
		AfxMessageBox("First, you have to select valid GameServerGroup !!");
		return;
	}

	// ���� ���̺� üũ
	CGridCellBase *pGCell;
	for(i=1; i < m_ExcelTables.GetRowCount(); i++)
	{
		pGCell = m_ExcelTables.GetCell(i, GRID_COL_INDEX_EXCEL_TABLES);
		if(1 == pGCell->GetImage())
		{
			pGCell = m_ExcelTables.GetCell(i, 0);
			CString tmCheckTable = pGCell->GetText();
			tmVectTables.push_back(tmCheckTable);
		}
	}

	if(tmVectTables.size() == 0)
	{
		MessageBox("There aren't selected table !!");
		return;
	}
	
	if(FALSE == m_ExcelData.LoadExcelData(&tmVectTables))
	{
		MessageBox("LoadExcelData Fail!!");
		return;
	}


	const char *szDBIP = GameServerInfo.DBIP;
	const int nDBPort = GameServerInfo.DBPort;
	const char *szDatabaseName = GameServerInfo.DBName;
	const char *szDBUID = GameServerInfo.DBUID;
	const char *szDBPassword = GameServerInfo.DBPWD;

	CODBCStatement odbcStmt;
	BOOL bRet = odbcStmt.Init(szDBIP, nDBPort, szDatabaseName, szDBUID, szDBPassword, GetSafeHwnd());
	if (!bRet)
	{
		MessageBox("Can not connect to the DB Server!!");
		return;
	}

	// DB��� â �ʱ�ȭ
	m_DBTables.SetWindowText("");
	
	// 2009. 10. 27 by jskim ����� Ȯ��
	CButton* pBtn = (CButton*)GetDlgItem(IDC_EXCELFILE_SEARCH);
	CButton* pBtn1 = (CButton*)GetDlgItem(IDC_INSERT_DB);
	CButton* pBtn2 = (CButton*)GetDlgItem(IDOK);
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_SELECT_DB);
	CMenu* pMenu =(CMenu*)GetSystemMenu(FALSE);
	m_ExcelTables.EnableWindow(FALSE);
	
	pBtn->EnableWindow(FALSE);	
	pBtn1->EnableWindow(FALSE);
	pBtn2->EnableWindow(FALSE);
	pCombo->EnableWindow(FALSE);
	pMenu->EnableMenuItem(SC_CLOSE, MF_DISABLED);
	//end 2009. 10. 27 by jskim ����� Ȯ��

	int nSourCnts = 0;
	// MFC Grid Control ����
	// insert DBTable
	// 2009. 10. 27 by jskim ����� Ȯ��
	m_nMaxTotalCount		= 0;
	m_nCurrentTotalCount	= 0;
	for(i=0; i<tmVectTables.size(); i++)
	{
		if(stricmp(tmVectTables[i], TB_NAME_ti_Item) == 0)
		{	
			m_nMaxTotalCount += m_ExcelData.vectItem.size();
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_MonsterItem) == 0)
		{
			m_nMaxTotalCount += m_ExcelData.vectMonsterItem.size();
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_RareItemInfo) == 0)
		{
			m_nMaxTotalCount += m_ExcelData.vectRareItemInfo.size();
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_Shop) == 0)
		{
			m_nMaxTotalCount += m_ExcelData.vectShopInfo.size();
		}	
		else if(stricmp(tmVectTables[i], TB_NAME_ti_CityTargetWarpMap) == 0)
		{
			m_nMaxTotalCount += m_ExcelData.vectCityTargetWarpMap.size();
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_ItemMixingInfo) == 0)
		{
			m_nMaxTotalCount += m_ExcelData.vectItemMixingInfo.size();
		}
		// start 2012-02-20 by hskim, DB ����ȭ - ItemMixingElement
		else if(stricmp(tmVectTables[i], TB_NAME_ti_ItemMixingElement) == 0)
		{
			m_nMaxTotalCount += m_ExcelData.vectItemMixingElement.size();
		}
		// end 2012-02-20 by hskim, DB ����ȭ - ItemMixingInfo
		else if(stricmp(tmVectTables[i], TB_NAME_ti_EnchantInfo) == 0)
		{
			m_nMaxTotalCount += m_ExcelData.vectEnchantInfo.size();
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_StrategyPointSummonMapIndex) == 0)
		{
			m_nMaxTotalCount += m_ExcelData.vectStrategyPointSummonMapIndex.size();
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_ArenaInfo) == 0)
		{
			m_nMaxTotalCount += m_ExcelData.vectArenaInfo.size();
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_ArenaMapinfo) == 0)
		{
			m_nMaxTotalCount += m_ExcelData.vectArenaMapInfo.size();
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_MysteryItemDrop) == 0)
		{
			m_nMaxTotalCount += m_ExcelData.vectMysteryItem.size();
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_InfluenceWar) == 0)
		{
			m_nMaxTotalCount += m_ExcelData.vectInfluenceWarInfo.size();
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_OutPostNextWarTime) == 0)
		{
			m_nMaxTotalCount += m_ExcelData.vectOutPostInfo.size();
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_ActionByLevel) == 0)
		{
			m_nMaxTotalCount += m_ExcelData.vectActoinByLevel.size();
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_MapInfo) == 0)
		{
			m_nMaxTotalCount += m_ExcelData.vectMapInfo.size();			
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_BuildingNPC) == 0)
		{
			m_nMaxTotalCount += m_ExcelData.vectBuildingNpc.size();
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_Monster) == 0)
		{
			m_nMaxTotalCount += m_ExcelData.vectMonsterInfo.size();
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_TutorialInfo) == 0)
		{
			m_nMaxTotalCount += m_ExcelData.vectTutorialInfo.size();
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_MapObject) == 0)
		{
			m_nMaxTotalCount += m_ExcelData.vectMapObject.size();			
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_LuckyMachine) == 0)
		{
			m_nMaxTotalCount += m_ExcelData.vectLuckyMachine.size();
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_QuestMatching) == 0)
		{
			m_nMaxTotalCount += m_ExcelData.vectQuestMatching.size();			
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_ItemMatching) == 0)
		{
			m_nMaxTotalCount += m_ExcelData.vectItemMatching.size();
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_InfluenceRate) == 0)	
		{
			m_nMaxTotalCount += m_ExcelData.vectInfluenceRate.size();
		}
		////////////////////////////////////////////////////////////////////////////////
		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - Excel -> DB �б�
		else if(stricmp(tmVectTables[i], TB_NAME_ti_HPAction) == 0)	{
			m_nMaxTotalCount += m_ExcelData.HPActionList.size();
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_Cinema) == 0) {
			m_nMaxTotalCount += m_ExcelData.CinemaInfoList.size();
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_Revision) == 0)	{
			m_nMaxTotalCount += m_ExcelData.RevisionInfoList.size();
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_InfinityMonster) == 0) {
			m_nMaxTotalCount += m_ExcelData.InfinityMonsterInfoList.size();
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_InfinityMode) == 0)	{
			m_nMaxTotalCount += m_ExcelData.InfinityModeInfoList.size();
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_Tender) == 0)	{
			m_nMaxTotalCount += m_ExcelData.TenderInfoList.size();
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_InfinityShop) == 0)	{
			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ����
			m_nMaxTotalCount += m_ExcelData.InfinityShopInfoList.size();
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_ItemParamOverlap) == 0)
		{// 2010-01-18 by cmkwon, ������ ���� Parameter �ߺ� üũ �ý��� ���� - CImportExcelDlg::OnInsertDb#
			m_nMaxTotalCount += m_ExcelData.ItemParamOverlapList.size();
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_InvokingWearItemDPNum) == 0)	{
			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ����
			m_nMaxTotalCount += m_ExcelData.InvokingItemDPList.size();
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_InvokingWearItemDPNumByUse) == 0)	{
			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ����
			m_nMaxTotalCount += m_ExcelData.InvokingItemDPByUseList.size();
		}
		else if(stricmp(tmVectTables[i],TB_NAME_ti_LevelAdjustment)==0)
		{// 2010-05-24 by shcho, ���Ǵ�Ƽ ���̵� ���� - ���Ǵ�Ƽ ���̵� ������ ���� �� ������Ʈ
			m_nMaxTotalCount += m_ExcelData.LevelAdjustmentList.size();
		}
		else if(stricmp(tmVectTables[i],TB_NAME_ti_LevelAdjustmentReward)==0)
		{// 2010-05-24 by shcho, ���Ǵ�Ƽ ���̵� ���� - ���Ǵ�Ƽ ���̵� ������ ���� �� ������Ʈ
			m_nMaxTotalCount += m_ExcelData.LevelAdjustmentRewardList.size();
		}
		else if(stricmp(tmVectTables[i],TB_NAME_ti_BurningMap)==0)
		{// 2010-08-05 by dhjin, ���׸� - 
			m_nMaxTotalCount += m_ExcelData.BurningMapList.size();
		}
		else if(stricmp(tmVectTables[i],TB_NAME_ti_Pet)==0)
		{// 2010-06-15 by shcho&hslee ��ý��� - �� �⺻ ���� ���� �б�
			m_nMaxTotalCount += m_ExcelData.PetBaseData.size();
		}
		else if(stricmp(tmVectTables[i],TB_NAME_ti_PetLevel)==0)
		{// 2010-06-15 by shcho&hslee ��ý��� - �� �⺻ ���� ���� �б�
			m_nMaxTotalCount += m_ExcelData.PetLevelData.size();
		}
		else if(stricmp(tmVectTables[i],TB_NAME_ti_OperatorAction)==0)
		{// 2010-06-15 by shcho&hslee ��ý��� - DB �б� �߰� �۾� �� ���۷����� ����.
			m_nMaxTotalCount += m_ExcelData.OperatorDataList.size();
		}
		else if(stricmp(tmVectTables[i],TB_NAME_ti_DissolutionItem)==0)
		{// 2010-08-31 by shcho&jskim �����ۿ��� �ý��� - ���� ������ ����.
			m_nMaxTotalCount += m_ExcelData.DissolutionIteminfo.size();
		}
		else if(stricmp(tmVectTables[i],TB_NAME_ti_MonsterMultiTarget)==0)
		{// 2011-03-17 by hskim, ���Ǵ�Ƽ 3�� - ���� ��Ƽ Ÿ���� ��� �߰�
			m_nMaxTotalCount += m_ExcelData.MonsterMultiTargetList.size();
		}
		// start 2011-05-23 by hskim, ���Ǵ�Ƽ 3�� - ��������Ʈ ����
		else if(stricmp(tmVectTables[i],TB_NAME_ti_WayPoint)==0)
		{
			m_nMaxTotalCount += m_ExcelData.WayPointList.size();
		}
		else if(stricmp(tmVectTables[i],TB_NAME_ti_WayPointMove)==0)
		{
			m_nMaxTotalCount += m_ExcelData.WayPointMoveList.size();
		}
		// end 2011-05-23 by hskim, ���Ǵ�Ƽ 3�� - ��������Ʈ ����
		else if(stricmp(tmVectTables[i],TB_NAME_ti_MapBuff)==0)
		{	// 2011-10-10 by hskim, EP4 [Ʈ���� �ý���] - ȭ���� / �� ��ǳ
			m_nMaxTotalCount += m_ExcelData.TriggerMapBuffList.size();
		}
		// start 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
		else if(stricmp(tmVectTables[i],TB_NAME_ti_MapTrigger)==0)
		{
			m_nMaxTotalCount += m_ExcelData.m_vectTriggerMapList.size();
		}
		else if(stricmp(tmVectTables[i],TB_NAME_ti_TriggerFunctionCrystal)==0)
		{
			m_nMaxTotalCount += m_ExcelData.m_vectTriggerFunctionCrystalList.size();
		}
		else if(stricmp(tmVectTables[i],TB_NAME_ti_TriggerCrystalGroup)==0)
		{
			m_nMaxTotalCount += m_ExcelData.m_vectTriggerCrystalGroupList.size();
		}
		else if(stricmp(tmVectTables[i],TB_NAME_ti_TriggerCrystalDestroyGroup)==0)
		{
			m_nMaxTotalCount += m_ExcelData.m_vectTriggerCrystalDestroyGroupList.size();
		}
		// end 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
		// start 2011-11-07 by hskim, EP4 [Ʈ���� �ý���] - NCG ������ �ý���
		else if(stricmp(tmVectTables[i],TB_NAME_ti_TriggerFunctionNGCInflWar)==0)
		{
			m_nMaxTotalCount += m_ExcelData.m_vectTriggerFunctionNGCInflWarList.size();
		}
		else if(stricmp(tmVectTables[i],TB_NAME_ti_TriggerNGCInflWarMonsterGroup)==0)
		{
			m_nMaxTotalCount += m_ExcelData.m_vectTriggerNGCInflWarMonsterGroupList.size();
		}
		// end 2011-11-07 by hskim, EP4 [Ʈ���� �ý���] - NCG ������ �ý���
		// 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý��� - ����� �� DB���� insert
#ifdef S_TRIGGER_OUTPOST_JHSEOL	// - ����� �� DB���� insert
		else if(stricmp(tmVectTables[i],TB_NAME_ti_TriggerFunctionNGCOutPost)==0)
		{
			m_nMaxTotalCount += m_ExcelData.m_vectTriggerFunctionNGCOutPostList.size();
		}
		else if(stricmp(tmVectTables[i],TB_NAME_ti_TriggerOutPostBossKill)==0)
		{
			m_nMaxTotalCount += m_ExcelData.m_vectTriggerOutPostBossKillInfoList.size();
		}
		else if(stricmp(tmVectTables[i],TB_NAME_ti_TriggerCrystalBuff)==0)
		{
			m_nMaxTotalCount += m_ExcelData.m_vectTriggerCrystalBuff.size();
		}
#endif	// #ifdef S_TRIGGER_OUTPOST_JHSEOL
		// end 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý��� - ����� �� DB���� insert
	}
	m_TotalProgress.SetRange32(0,m_nMaxTotalCount - 1);
	m_TotalProgress.SetPos(m_nCurrentTotalCount);
	//end 2009. 10. 27 by jskim ����� Ȯ��

	// start 2011-07-15 by shcho - ĳ�������� üũ �ý��� ����
	CCheckItemDialog dlg;
	int checknum = 0; // üũ�� ���� ��ȯ�� ����
	cashItemCheckData tmpItemCheckData;
	util::zero(&tmpItemCheckData,sizeof(cashItemCheckData));

//	if(IS_MASANG_IP(GameServerInfo.ServerIP))
	{
		checknum = dlg.m_CheckDBData.LoadScanOldCashitemDBData(&odbcStmt); // Db���� ���� ���� �ε��Ѵ�.
	}
	// end 2011-07-15 by shcho - ĳ�������� üũ �ý��� ����
	
	for(i=0; i<tmVectTables.size(); i++)
	{
		// 2009. 10. 27 by jskim ����� Ȯ��
		m_TableName.Clear();
		m_TableName.SetSel(0, -1);
		m_TableName.ReplaceSel(tmVectTables[i] );
		//end 2009. 10. 27 by jskim ����� Ȯ��
		///////////////////////////////////////////////////////////////////////////////
		// 2009-10-23 by cmkwon, Monitor �������� �о�ֱ�� üũ �߰� - CImportExcelDlg::OnInsertDb#, ���̺���� #define �Ѱ��� �����.
		if(stricmp(tmVectTables[i], TB_NAME_ti_Item) == 0)
		{	
			// item info
			nSourCnts = m_ExcelData.vectItem.size();
			// 2009. 10. 27 by jskim ����� Ȯ��
			
			// start 2011-07-15 by shcho - ĳ�������� üũ �ý��� ����
//			if(IS_MASANG_IP(GameServerInfo.ServerIP))
			{

				if(0 != checknum) // �ű� �������� ��쿡�� üũ�� �ǳ� �ڴ�.
				{
					for(int i=0; i < m_ExcelData.vectItem.size() ; i++)
					{
						// üũ�� �ʿ��� ���� ���� ���� �ϱ�
						tmpItemCheckData.ItemNumber		=	m_ExcelData.vectItem[i].ItemNum;
#ifndef SHOP_PRICES_PER_BUILDING_NPC
						tmpItemCheckData.CashPrice		=	m_ExcelData.vectItem[i].CashPrice;
						tmpItemCheckData.Price = m_ExcelData.vectItem[i].Price;
#endif // SHOP_PRICES_PER_BUILDING_NPC
						tmpItemCheckData.ItemAttribute	=	m_ExcelData.vectItem[i].ItemAttribute;
						tmpItemCheckData.Time			=	m_ExcelData.vectItem[i].Time;
						
						// ã��
						if(dlg.m_CheckDBData.CheckOldDBCashItems(tmpItemCheckData)) // �ε��� ������ üũ����.
						{
							dlg.m_CheckDBData.GetData(VIEW_NEW_DATA)->push_back(tmpItemCheckData); // ���ο� ������
						}
						
						// �ʱ�ȭ
						util::zero(&tmpItemCheckData,sizeof(cashItemCheckData)); // �ʱ�ȭ
					}

				}

				if(dlg.m_CheckDBData.GetData(VIEW_OLD_DATA)->size()) // ���� ������ ���� �Ȱ��� ���ų� ���� DB�� �ش��Ѵ�.
				{ // üũâ�� ����.
					if(dlg.DoModal() != IDOK)
					{ // OK�� �ƴҶ� �ߴ��Ѵ�.
						pBtn->EnableWindow(TRUE);	
						pBtn1->EnableWindow(TRUE);
						pBtn2->EnableWindow(TRUE);
						pBtn2->EnableWindow(TRUE);
						m_ExcelTables.EnableWindow(TRUE);
						pMenu->EnableMenuItem(SC_CLOSE, MF_ENABLED);
						return;
					}
				}
			}
			// end 2011-07-15 by shcho - ĳ�������� üũ �ý��� ����

			//nObjects = CAtumDBHelper::SaveItem(&odbcStmt, &m_ExcelData.vectItem);
			nObjects = CAtumDBHelper::SaveItem(&odbcStmt, &m_ExcelData.vectItem, m_hWnd);
			//end 2009. 10. 27 by jskim ����� Ȯ��
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_MonsterItem) == 0)
		{
			// monster item info
			nSourCnts = m_ExcelData.vectMonsterItem.size();
			// 2009. 10. 27 by jskim ����� Ȯ��
			//nObjects = CAtumDBHelper::SaveMonsterItem(&odbcStmt, &m_ExcelData.vectMonsterItem);
			nObjects = CAtumDBHelper::SaveMonsterItem(&odbcStmt, &m_ExcelData.vectMonsterItem, m_hWnd);
			//end 2009. 10. 27 by jskim ����� Ȯ��
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_RareItemInfo) == 0)
		{
			// rare item info
			nSourCnts = m_ExcelData.vectRareItemInfo.size();
			// 2009. 10. 27 by jskim ����� Ȯ��
			//nObjects = CAtumDBHelper::SaveRareItemInfo(&odbcStmt, &m_ExcelData.vectRareItemInfo);
			nObjects = CAtumDBHelper::SaveRareItemInfo(&odbcStmt, &m_ExcelData.vectRareItemInfo, m_hWnd);
			//end 2009. 10. 27 by jskim ����� Ȯ��
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_Shop) == 0)
		{
			// shop info
			nSourCnts = m_ExcelData.vectShopInfo.size();
			// 2009. 10. 27 by jskim ����� Ȯ��
			//nObjects = CAtumDBHelper::SaveShop(&odbcStmt, &m_ExcelData.vectShopInfo);
			nObjects = CAtumDBHelper::SaveShop(&odbcStmt, &m_ExcelData.vectShopInfo,  m_hWnd);
			//end 2009. 10. 27 by jskim ����� Ȯ��
		}	
		else if(stricmp(tmVectTables[i], TB_NAME_ti_CityTargetWarpMap) == 0)
		{
			// CityTargetWarpMap info
			nSourCnts = m_ExcelData.vectCityTargetWarpMap.size();
			// 2009. 10. 27 by jskim ����� Ȯ��
			//nObjects = CAtumDBHelper::SaveCityTargetWarpMap(&odbcStmt, &m_ExcelData.vectCityTargetWarpMap);
			nObjects = CAtumDBHelper::SaveCityTargetWarpMap(&odbcStmt, &m_ExcelData.vectCityTargetWarpMap, m_hWnd);
			//end 2009. 10. 27 by jskim ����� Ȯ��
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_ItemMixingInfo) == 0)
		{
			// mixing info
			nSourCnts = m_ExcelData.vectItemMixingInfo.size();
			// 2009. 10. 27 by jskim ����� Ȯ��
			//nObjects = CAtumDBHelper::SaveItemMixingInfo(&odbcStmt, &m_ExcelData.vectItemMixingInfo);
			nObjects = CAtumDBHelper::SaveItemMixingInfo(&odbcStmt, &m_ExcelData.vectItemMixingInfo, m_hWnd);
			//end 2009. 10. 27 by jskim ����� Ȯ��
		}
		// start 2012-02-20 by hskim, DB ����ȭ - ItemMixingInfo
		else if(stricmp(tmVectTables[i], TB_NAME_ti_ItemMixingElement) == 0)
		{
			nSourCnts = m_ExcelData.vectItemMixingElement.size();
			nObjects = CAtumDBHelper::SaveItemMixingElement(&odbcStmt, &m_ExcelData.vectItemMixingElement, m_hWnd);
		}
		// end 2012-02-20 by hskim, DB ����ȭ - ItemMixingInfo
		else if(stricmp(tmVectTables[i], TB_NAME_ti_EnchantInfo) == 0)
		{
			// Enchant Info
			nSourCnts = m_ExcelData.vectEnchantInfo.size();
			// 2009. 10. 27 by jskim ����� Ȯ��
			//nObjects = CAtumDBHelper::SaveEnchantInfo(&odbcStmt, &m_ExcelData.vectEnchantInfo);
			nObjects = CAtumDBHelper::SaveEnchantInfo(&odbcStmt, &m_ExcelData.vectEnchantInfo, m_hWnd);
			//end 2009. 10. 27 by jskim ����� Ȯ��
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_StrategyPointSummonMapIndex) == 0)
		{
			// StrategyPointSummonMapIndex info
			nSourCnts = m_ExcelData.vectStrategyPointSummonMapIndex.size();
			// 2009. 10. 27 by jskim ����� Ȯ��
			//nObjects = CAtumDBHelper::SaveStrategyPointSummonMapIndex(&odbcStmt, &m_ExcelData.vectStrategyPointSummonMapIndex);
			nObjects = CAtumDBHelper::SaveStrategyPointSummonMapIndex(&odbcStmt, &m_ExcelData.vectStrategyPointSummonMapIndex, m_hWnd);
			//end 2009. 10. 27 by jskim ����� Ȯ��
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_ArenaInfo) == 0)
		{
			// Arena Info
			nSourCnts = m_ExcelData.vectArenaInfo.size();
			// 2009. 10. 27 by jskim ����� Ȯ��
			//nObjects = CAtumDBHelper::SaveArenaInfo(&odbcStmt, &m_ExcelData.vectArenaInfo);
			nObjects = CAtumDBHelper::SaveArenaInfo(&odbcStmt, &m_ExcelData.vectArenaInfo, m_hWnd);
			//end 2009. 10. 27 by jskim ����� Ȯ��
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_ArenaMapinfo) == 0)
		{
			//ArenaMap Info
			nSourCnts = m_ExcelData.vectArenaMapInfo.size();
			// 2009. 10. 27 by jskim ����� Ȯ��
			//nObjects = CAtumDBHelper::SaveArenaMapinfo(&odbcStmt, &m_ExcelData.vectArenaMapInfo);	
			nObjects = CAtumDBHelper::SaveArenaMapinfo(&odbcStmt, &m_ExcelData.vectArenaMapInfo, m_hWnd);
			//end 2009. 10. 27 by jskim ����� Ȯ��
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_MysteryItemDrop) == 0)
		{
			// MysteryItemDrop info
			nSourCnts = m_ExcelData.vectMysteryItem.size();
			// 2009. 10. 27 by jskim ����� Ȯ��
			//nObjects = CAtumDBHelper::SaveMysteryItemDrop(&odbcStmt, &m_ExcelData.vectMysteryItem);
			nObjects = CAtumDBHelper::SaveMysteryItemDrop(&odbcStmt, &m_ExcelData.vectMysteryItem, m_hWnd);
			//end 2009. 10. 27 by jskim ����� Ȯ��
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_InfluenceWar) == 0)
		{
			// InfluenceWar info
			nSourCnts = m_ExcelData.vectInfluenceWarInfo.size();
			// 2009. 10. 27 by jskim ����� Ȯ��
			//nObjects = CAtumDBHelper::SaveInfluenceWar(&odbcStmt, &m_ExcelData.vectInfluenceWarInfo);
			nObjects = CAtumDBHelper::SaveInfluenceWar(&odbcStmt, &m_ExcelData.vectInfluenceWarInfo, m_hWnd);
			//end 2009. 10. 27 by jskim ����� Ȯ��
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_OutPostNextWarTime) == 0)
		{
			// OutPostNextWarTime info
			nSourCnts = m_ExcelData.vectOutPostInfo.size();
			// 2009. 10. 27 by jskim ����� Ȯ��
			//nObjects = CAtumDBHelper::SaveOutPostNextWarTime(&odbcStmt, &m_ExcelData.vectOutPostInfo);
			nObjects = CAtumDBHelper::SaveOutPostNextWarTime(&odbcStmt, &m_ExcelData.vectOutPostInfo, m_hWnd);
			//end 2009. 10. 27 by jskim ����� Ȯ��
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_ActionByLevel) == 0)
		{
			// ActionByLevel info
			nSourCnts = m_ExcelData.vectActoinByLevel.size();
			// 2009. 10. 27 by jskim ����� Ȯ��
			//nObjects = CAtumDBHelper::SaveActionByLevel(&odbcStmt, &m_ExcelData.vectActoinByLevel);
			nObjects = CAtumDBHelper::SaveActionByLevel(&odbcStmt, &m_ExcelData.vectActoinByLevel, m_hWnd);
			//end 2009. 10. 27 by jskim ����� Ȯ��
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_MapInfo) == 0)
		{
			// MapInfo
			nSourCnts = m_ExcelData.vectMapInfo.size();
			// 2009. 10. 27 by jskim ����� Ȯ��
			//nObjects = CAtumDBHelper::SaveMapInfo(&odbcStmt, &m_ExcelData.vectMapInfo);
			nObjects = CAtumDBHelper::SaveMapInfo(&odbcStmt, &m_ExcelData.vectMapInfo, m_hWnd);
			//end 2009. 10. 27 by jskim ����� Ȯ��
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_BuildingNPC) == 0)
		{
			// buildingnpc
			nSourCnts = m_ExcelData.vectBuildingNpc.size();
			// 2009. 10. 27 by jskim ����� Ȯ��
			//nObjects = CAtumDBHelper::SaveBuildingNPC(&odbcStmt, &m_ExcelData.vectBuildingNpc);
			nObjects = CAtumDBHelper::SaveBuildingNPC(&odbcStmt, &m_ExcelData.vectBuildingNpc, m_hWnd);
			//end 2009. 10. 27 by jskim ����� Ȯ��
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_Monster) == 0)
		{
			// monster info
			nSourCnts = m_ExcelData.vectMonsterInfo.size();
			// 2009. 10. 27 by jskim ����� Ȯ��
			//nObjects = CAtumDBHelper::SaveMonster(&odbcStmt, &m_ExcelData.vectMonsterInfo);
			nObjects = CAtumDBHelper::SaveMonster(&odbcStmt, &m_ExcelData.vectMonsterInfo, m_hWnd);
			//end 2009. 10. 27 by jskim ����� Ȯ��
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_TutorialInfo) == 0)
		{
			// TutorialInfo
			nSourCnts = m_ExcelData.vectTutorialInfo.size();
			// 2009. 10. 27 by jskim ����� Ȯ��
			//nObjects = CAtumDBHelper::SaveTutorialInfo(&odbcStmt, &m_ExcelData.vectTutorialInfo);
			nObjects = CAtumDBHelper::SaveTutorialInfo(&odbcStmt, &m_ExcelData.vectTutorialInfo, m_hWnd);
			//end 2009. 10. 27 by jskim ����� Ȯ��
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_MapObject) == 0)
		{
			// mapobjectinfo
			nSourCnts = m_ExcelData.vectMapObject.size();
			// 2009. 10. 27 by jskim ����� Ȯ��
			//nObjects = CAtumDBHelper::SaveMapObject(&odbcStmt, &m_ExcelData.vectMapObject);
			nObjects = CAtumDBHelper::SaveMapObject(&odbcStmt, &m_ExcelData.vectMapObject, m_hWnd);
			//end 2009. 10. 27 by jskim ����� Ȯ��
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_LuckyMachine) == 0)
		{
			// LuckyMachine info
			nSourCnts = m_ExcelData.vectLuckyMachine.size();
			// 2009. 10. 27 by jskim ����� Ȯ��
			//nObjects = CAtumDBHelper::SaveLuckyMachine(&odbcStmt, &m_ExcelData.vectLuckyMachine);
			nObjects = CAtumDBHelper::SaveLuckyMachine(&odbcStmt, &m_ExcelData.vectLuckyMachine, m_hWnd);
			//end 2009. 10. 27 by jskim ����� Ȯ��
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_QuestMatching) == 0)
		{
			// QuestMatching info
			nSourCnts = m_ExcelData.vectQuestMatching.size();
			// 2009. 10. 27 by jskim ����� Ȯ��
			//nObjects = CAtumDBHelper::SaveQuestMatching(&odbcStmt, &m_ExcelData.vectQuestMatching);
			nObjects = CAtumDBHelper::SaveQuestMatching(&odbcStmt, &m_ExcelData.vectQuestMatching, m_hWnd);
			//end 2009. 10. 27 by jskim ����� Ȯ��
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_ItemMatching) == 0)
		{
			// ItemMatching info
			nSourCnts = m_ExcelData.vectItemMatching.size();
			// 2009. 10. 27 by jskim ����� Ȯ��
			//nObjects = CAtumDBHelper::SaveItemMatching(&odbcStmt, &m_ExcelData.vectItemMatching);
			nObjects = CAtumDBHelper::SaveItemMatching(&odbcStmt, &m_ExcelData.vectItemMatching, m_hWnd);
			//end 2009. 10. 27 by jskim ����� Ȯ��
		}
// 2009-10-23 by cmkwon, Monitor �������� �о�ֱ�� üũ �߰� - CImportExcelDlg::OnInsertDb#, ������� �����Ƿ� ����
// 		else if(stricmp(tmVectTables[i], "ti_Item_Info") == 0)
// 		{
// 			// ItemInfo info
// 			nObjects = CAtumDBHelper::SaveItemInfo(&odbcStmt, &m_ExcelData.vectItemInfo);
// 		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_InfluenceRate) == 0)		// 2009-09-16 by cmkwon, ���� �ʱ�ȭ�� ���¡ ���� ���� - ti_InfluenceRate ���̺� �߰�, 
		{
			// ItemInfo info
			nSourCnts = m_ExcelData.vectInfluenceRate.size();
			// 2009. 10. 27 by jskim ����� Ȯ��
			//nObjects = CAtumDBHelper::SaveInfluenceRate(&odbcStmt, &m_ExcelData.vectInfluenceRate);
			nObjects = CAtumDBHelper::SaveInfluenceRate(&odbcStmt, &m_ExcelData.vectInfluenceRate, m_hWnd);
			//end 2009. 10. 27 by jskim ����� Ȯ��
		}
		////////////////////////////////////////////////////////////////////////////////
		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - Excel -> DB �б�
		else if(stricmp(tmVectTables[i], TB_NAME_ti_HPAction) == 0) {
			nSourCnts = m_ExcelData.HPActionList.size();
			nObjects = CAtumDBHelper::SaveHPAction(&odbcStmt, &m_ExcelData.HPActionList, m_hWnd);
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_Cinema) == 0) {
			nSourCnts = m_ExcelData.CinemaInfoList.size();
			nObjects = CAtumDBHelper::SaveCinema(&odbcStmt, &m_ExcelData.CinemaInfoList, m_hWnd);
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_Revision) == 0) {
			nSourCnts = m_ExcelData.RevisionInfoList.size();
			nObjects = CAtumDBHelper::SaveRevision(&odbcStmt, &m_ExcelData.RevisionInfoList, m_hWnd);
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_InfinityMonster) == 0) {
			nSourCnts = m_ExcelData.InfinityMonsterInfoList.size();
			nObjects = CAtumDBHelper::SaveInfinityMonster(&odbcStmt, &m_ExcelData.InfinityMonsterInfoList, m_hWnd);
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_InfinityMode) == 0) {
			nSourCnts = m_ExcelData.InfinityModeInfoList.size();
			nObjects = CAtumDBHelper::SaveInfinityMode(&odbcStmt, &m_ExcelData.InfinityModeInfoList, m_hWnd);
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_Tender) == 0) {
			nSourCnts = m_ExcelData.TenderInfoList.size();
			nObjects = CAtumDBHelper::SaveTender(&odbcStmt, &m_ExcelData.TenderInfoList, m_hWnd);
		}	
		else if(stricmp(tmVectTables[i], TB_NAME_ti_InfinityShop) == 0) {
			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ����
			nSourCnts = m_ExcelData.InfinityShopInfoList.size();
			nObjects = CAtumDBHelper::SaveInfinityShop(&odbcStmt, &m_ExcelData.InfinityShopInfoList, m_hWnd);
		}	
		else if(stricmp(tmVectTables[i], TB_NAME_ti_ItemParamOverlap) == 0)
		{// 2010-01-18 by cmkwon, ������ ���� Parameter �ߺ� üũ �ý��� ���� - CImportExcelDlg::OnInsertDb#
			nSourCnts = m_ExcelData.ItemParamOverlapList.size();
			nObjects = CAtumDBHelper::SaveItemParamOverlap(&odbcStmt, &m_ExcelData.ItemParamOverlapList, m_hWnd);
		}	
		else if(stricmp(tmVectTables[i], TB_NAME_ti_InvokingWearItemDPNum) == 0) {
			// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
			nSourCnts = m_ExcelData.InvokingItemDPList.size();
			nObjects = CAtumDBHelper::SaveInvokingWearItemDPNum(&odbcStmt, &m_ExcelData.InvokingItemDPList, m_hWnd);
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_InvokingWearItemDPNumByUse) == 0) {
			// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
			nSourCnts = m_ExcelData.InvokingItemDPByUseList.size();
			nObjects = CAtumDBHelper::SaveInvokingWearItemDPNumByUse(&odbcStmt, &m_ExcelData.InvokingItemDPByUseList, m_hWnd);
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_LevelAdjustment) == 0) 
		{
			// 2010-05-24 by shcho, ���Ǵ�Ƽ ���̵� ���� - ���Ǵ�Ƽ ���̵� ������ ���� �� ������Ʈ
			nSourCnts = m_ExcelData.LevelAdjustmentList.size();
			nObjects = CAtumDBHelper::SaveLevelAdjustmentList(&odbcStmt, &m_ExcelData.LevelAdjustmentList, m_hWnd);
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_LevelAdjustmentReward) == 0) 
		{
			// 2010-05-24 by shcho, ���Ǵ�Ƽ ���̵� ���� - ���Ǵ�Ƽ ���̵� ������ ���� �� ������Ʈ
			nSourCnts = m_ExcelData.LevelAdjustmentRewardList.size();
			nObjects = CAtumDBHelper::SaveLevelAdjustmentRewardList(&odbcStmt, &m_ExcelData.LevelAdjustmentRewardList, m_hWnd);
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_LevelAdjustmentReward) == 0) 
		{
			// 2010-05-24 by shcho, ���Ǵ�Ƽ ���̵� ���� - ���Ǵ�Ƽ ���̵� ������ ���� �� ������Ʈ
			nSourCnts = m_ExcelData.LevelAdjustmentRewardList.size();
			nObjects = CAtumDBHelper::SaveLevelAdjustmentRewardList(&odbcStmt, &m_ExcelData.LevelAdjustmentRewardList, m_hWnd);
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_BurningMap) == 0) 
		{
			// 2010-08-05 by dhjin, ���׸� - 
			nSourCnts = m_ExcelData.BurningMapList.size();
			nObjects = CAtumDBHelper::SaveBurningMapList(&odbcStmt, &m_ExcelData.BurningMapList, m_hWnd);
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_Pet) == 0)
		{
			// 2010-06-15 by shcho&hslee ��ý��� - �� �⺻ ���� ���� �б�
			nSourCnts = m_ExcelData.PetBaseData.size();
			nObjects = CAtumDBHelper::SavePetBaseData(&odbcStmt, &m_ExcelData.PetBaseData, m_hWnd);
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_PetLevel) == 0)
		{
			// 2010-06-15 by shcho&hslee ��ý��� - �� ���� ���� ���� �б�
			nSourCnts = m_ExcelData.PetLevelData.size();
			nObjects = CAtumDBHelper::SavePetLevelData(&odbcStmt, &m_ExcelData.PetLevelData, m_hWnd);
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_OperatorAction) == 0)
		{
			// 2010-06-15 by shcho&hslee ��ý��� - DB �б� �߰� �۾� �� ���۷����� ����.
			nSourCnts = m_ExcelData.OperatorDataList.size();
			nObjects = CAtumDBHelper::SaveOperatorData(&odbcStmt, &m_ExcelData.OperatorDataList, m_hWnd);
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_DissolutionItem) == 0)
		{
			// 2010-08-31 by shcho&jskim �����ۿ��� �ý��� - ���� ������ ����.
			nSourCnts = m_ExcelData.DissolutionIteminfo.size();
			nObjects = CAtumDBHelper::SaveDissolutionItemData(&odbcStmt, &m_ExcelData.DissolutionIteminfo, m_hWnd);
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_MonsterMultiTarget) == 0)
		{
			// 2011-03-17 by hskim, ���Ǵ�Ƽ 3�� - ���� ��Ƽ Ÿ���� ��� �߰�
			nSourCnts = m_ExcelData.MonsterMultiTargetList.size();
			nObjects = CAtumDBHelper::SaveMonsterMultiTargetData(&odbcStmt, &m_ExcelData.MonsterMultiTargetList, m_hWnd);
		}		
		// start 2011-05-23 by hskim, ���Ǵ�Ƽ 3�� - ��������Ʈ ����
		else if(stricmp(tmVectTables[i], TB_NAME_ti_WayPoint) == 0)
		{
			nSourCnts = m_ExcelData.WayPointList.size();
			nObjects = CAtumDBHelper::SaveWayPointData(&odbcStmt, &m_ExcelData.WayPointList, m_hWnd);
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_WayPointMove) == 0)
		{
			nSourCnts = m_ExcelData.WayPointMoveList.size();
			nObjects = CAtumDBHelper::SaveWayPointMoveData(&odbcStmt, &m_ExcelData.WayPointMoveList, m_hWnd);
		}
		// end 2011-05-23 by hskim, ���Ǵ�Ƽ 3�� - ��������Ʈ ����
		else if(stricmp(tmVectTables[i], TB_NAME_ti_MapBuff) == 0)
		{	// 2011-10-10 by hskim, EP4 [Ʈ���� �ý���] - ȭ���� / �� ��ǳ
			nSourCnts = m_ExcelData.TriggerMapBuffList.size();
			nObjects = CAtumDBHelper::SaveTriggerMapBuff(&odbcStmt, &m_ExcelData.TriggerMapBuffList, m_hWnd);
		}
		// start 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
		else if(stricmp(tmVectTables[i], TB_NAME_ti_MapTrigger) == 0)
		{
			nSourCnts = m_ExcelData.m_vectTriggerMapList.size();
			nObjects = CAtumDBHelper::SaveTriggerMap(&odbcStmt, &m_ExcelData.m_vectTriggerMapList, m_hWnd);
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_TriggerFunctionCrystal) == 0)
		{
			nSourCnts = m_ExcelData.m_vectTriggerFunctionCrystalList.size();
			nObjects = CAtumDBHelper::SaveTriggerFunctionCrystal(&odbcStmt, &m_ExcelData.m_vectTriggerFunctionCrystalList, m_hWnd);
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_TriggerCrystalGroup) == 0)
		{
			nSourCnts = m_ExcelData.m_vectTriggerCrystalGroupList.size();
			nObjects = CAtumDBHelper::SaveTriggerCrystalGroup(&odbcStmt, &m_ExcelData.m_vectTriggerCrystalGroupList, m_hWnd);
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_TriggerCrystalDestroyGroup) == 0)
		{
			nSourCnts = m_ExcelData.m_vectTriggerCrystalDestroyGroupList.size();
			nObjects = CAtumDBHelper::SaveTriggerCrystalDestroyGroup(&odbcStmt, &m_ExcelData.m_vectTriggerCrystalDestroyGroupList, m_hWnd);
		}
		// end 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
		// start 2011-11-07 by hskim, EP4 [Ʈ���� �ý���] - NCG ������ �ý���
		else if(stricmp(tmVectTables[i], TB_NAME_ti_TriggerFunctionNGCInflWar) == 0)
		{
			nSourCnts = m_ExcelData.m_vectTriggerFunctionNGCInflWarList.size();
			nObjects = CAtumDBHelper::SaveTriggerFunctionNGCInflWar(&odbcStmt, &m_ExcelData.m_vectTriggerFunctionNGCInflWarList, m_hWnd);
		}
		else if(stricmp(tmVectTables[i], TB_NAME_ti_TriggerNGCInflWarMonsterGroup) == 0)
		{
			nSourCnts = m_ExcelData.m_vectTriggerNGCInflWarMonsterGroupList.size();
			nObjects = CAtumDBHelper::SaveTriggerNGCInflWarMonsterGroup(&odbcStmt, &m_ExcelData.m_vectTriggerNGCInflWarMonsterGroupList, m_hWnd);
		}
		// end 2011-11-07 by hskim, EP4 [Ʈ���� �ý���] - NCG ������ �ý���
		// 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý��� - ����� �� DB���� insert
#ifdef S_TRIGGER_OUTPOST_JHSEOL	// - ����� �� DB���� insert
		else if (stricmp(tmVectTables[i], TB_NAME_ti_TriggerFunctionNGCOutPost) == 0)
		{
			nSourCnts = m_ExcelData.m_vectTriggerFunctionNGCOutPostList.size();
			nObjects = CAtumDBHelper::SaveTriggerFunctionNGCOutPost(&odbcStmt, &m_ExcelData.m_vectTriggerFunctionNGCOutPostList, m_hWnd);
		}
		else if (stricmp(tmVectTables[i], TB_NAME_ti_TriggerOutPostBossKill) == 0)
		{
			nSourCnts = m_ExcelData.m_vectTriggerOutPostBossKillInfoList.size();
			nObjects = CAtumDBHelper::SaveTriggerOutPostBossKill(&odbcStmt, &m_ExcelData.m_vectTriggerOutPostBossKillInfoList, m_hWnd);
		}
		else if (stricmp(tmVectTables[i], TB_NAME_ti_TriggerCrystalBuff) == 0)
		{
			nSourCnts = m_ExcelData.m_vectTriggerCrystalBuff.size();
			nObjects = CAtumDBHelper::SaveTriggerCrystalBuff(&odbcStmt, &m_ExcelData.m_vectTriggerCrystalBuff, m_hWnd);
		}
#endif	// #ifdef S_TRIGGER_OUTPOST_JHSEOL
		// end 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý��� - ����� �� DB���� insert
		
		// ������ ���� ���� üũ
		if(0 >= nObjects)
		{
			// 2010-01-13 by cmkwon, ���� �߰�
			szError.Format("Save %s error from DB!!, Please check DB Schema.(ErrorCode:%d)", tmVectTables[i], nObjects);
			MessageBox(szError);

			// 2009. 10. 27 by jskim ����� Ȯ��
			pBtn->EnableWindow(TRUE);	
			pBtn1->EnableWindow(TRUE);
			pBtn2->EnableWindow(TRUE);
			pBtn2->EnableWindow(TRUE);
			m_ExcelTables.EnableWindow(TRUE);
			pMenu->EnableMenuItem(SC_CLOSE, MF_ENABLED);
			//end 2009. 10. 27 by jskim ����� Ȯ��
			return;
		}

		m_nCurrentTotalCount += nObjects;	// 2009. 10. 27 by jskim ����� Ȯ��

		dbResultInfo.Format("%-30s  :  (%5d) ==> (%5d)\r\n", tmVectTables[i], nSourCnts, nObjects);
		dbResultInfo.Remove('$');	// '$'ǥ�� ����
		nlen = m_DBTables.GetWindowTextLength();
		m_DBTables.SetSel(nlen, nlen);
		m_DBTables.ReplaceSel(dbResultInfo);	
		nObjects = 0;
	}// end for
	// 2009. 10. 27 by jskim ����� Ȯ��
	m_TableName.SetWindowText("Work Completion");
	//end 2009. 10. 27 by jskim ����� Ȯ��
	MessageBox("Work Completion.");
	
	odbcStmt.Clean();

	///////////////////////////////////////////////////////////////////////////////
	// 2009-11-10 by cmkwon, SCMonitor�� ���� ���� �о�ֱ� ���� - 
	this->InitGridSelectedTables();


	// write profile
	CAtumMonitorApp *pMonitorApp		= (CAtumMonitorApp*)(AfxGetApp());
	pMonitorApp->m_strGameServerName	= GameServerInfo.ServerName;
	pMonitorApp->WriteProfile();
	// 2009. 10. 27 by jskim ����� Ȯ��
	pBtn->EnableWindow(TRUE);	
	pBtn1->EnableWindow(TRUE);
	pBtn2->EnableWindow(TRUE);
	pBtn2->EnableWindow(TRUE);
	m_ExcelTables.EnableWindow(TRUE);
	pMenu->EnableMenuItem(SC_CLOSE, MF_ENABLED);
	//end 2009. 10. 27 by jskim ����� Ȯ��
}

/////////////////////////////////////////////////////////////////////////////
// CImportExcelDlg message handlers

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CImportExcelDlg::OnExcelfileSearch()
/// \brief		// 2009-07-09 by jwlee, ���� ���� ���̺� ������ DB�� �о�ֱ� �� ���� - 
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CImportExcelDlg::OnExcelfileSearch()
{
	// TODO: Add your control notification handler code here
	// 2009-07-09 by jwlee, ���� ���� ���̺� ������ DB�� �о�ֱ� �� ���� -
	CFileDialog dlg(TRUE);
	dlg.m_ofn.Flags |= OFN_NOCHANGEDIR;
	if(dlg.DoModal() != IDOK){
		return;
	}

	util::strncpy(m_FileName, dlg.GetPathName(), MAX_PATH);
	
	CString csExt = dlg.GetFileExt();
	if(0 != csExt.CompareNoCase("xls")
		&& 0 != csExt.CompareNoCase("xlsx"))
	{
		MessageBox("Not ExcelFile");
		return;
	}
	
	// ��� â �ʱ�ȭ
	this->InitAllWindows();

	m_Select_ExcelFile.SetWindowText(m_FileName);
	if(!m_ExcelData.ConnectExcel(m_FileName))
	{
		MessageBox("Can't Connect ExcelFile!!");
		return;
	}
	
	if(!m_ExcelData.LoadTableList())
	{
		MessageBox("Don't have GameTable!! Or Break File!!");
		return;
	}

	// ���� ���� ���� ���
	this->ViewGridExcelTables();
}

BOOL CImportExcelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int nComboSelIndex = 0;

	CAtumMonitorApp *pMonitorApp = (CAtumMonitorApp*)(AfxGetApp());
	for (int i = 0; g_arrGameServers[i].ServerName !=NULL; i++)
	{
		m_ComboServerList.AddString(g_arrGameServers[i].ServerName);
		if (0 == pMonitorApp->m_strGameServerName.Compare(g_arrGameServers[i].ServerName))
		{
			nComboSelIndex = i;
		}
	}
	m_ComboServerList.SetCurSel(nComboSelIndex);

	this->InitAllWindows();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// CImportExcelDlg message handlers

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CImportExcelDlg::InitGridExcelTables()
/// \brief		�׸��� ��Ʈ�� �ʱ�ȭ 
/// \author		jwlee
/// \date		2009-07-29 ~ 2009-07-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CImportExcelDlg::InitGridExcelTables()
{
	m_ilExcelTables2.DeleteImageList();
	m_ilExcelTables2.Create(24, 12, ILC_COLOR32, 2, 2);
	

	CBitmap bmCheckImage;
	bmCheckImage.LoadBitmap(IDB_BITMAP_CHECK_IMAGE);
	m_ilExcelTables2.Add(&bmCheckImage, RGB(0,0,0));
	m_ExcelTables.SetImageList(&m_ilExcelTables2);
	
	m_ExcelTables.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
	int m_nCols = 3;

	m_ExcelTables.SetEditable(FALSE);
	m_ExcelTables.SetListMode(TRUE);
	
	m_ExcelTables.EnableSelection(TRUE);
	m_ExcelTables.SetFrameFocusCell(FALSE);
	m_ExcelTables.SetTrackFocusCell(FALSE);

	m_ExcelTables.SetRowCount(m_nRows);
	m_ExcelTables.SetColumnCount(m_nCols);
	m_ExcelTables.SetFixedRowCount(1);

	// Į�� �����
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = GRID_CELL_FORMAT;

	Item.col = m_nCols++;
	Item.strText.Format("Table List");
	m_ExcelTables.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Record Count");
	m_ExcelTables.SetItem(&Item);

	Item.col = m_nCols++;
	Item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_IMAGE;
	Item.strText.Format("ALL");
	Item.iImage		= 0;
	Item.crBkClr	= RGB(200, 200, 200);
	m_ExcelTables.SetItem(&Item);

	// arrange grid
	m_ExcelTables.AutoSize();
	m_ExcelTables.ExpandColumnsToFit();

	// clean all cells
	CCellRange tmpCellRange(1, 0, m_ExcelTables.GetRowCount()-1, m_ExcelTables.GetColumnCount()-1);
	m_ExcelTables.ClearCells(tmpCellRange);
}

/////////////////////////////////////////////////////////////////////////////
// CImportExcelDlg message handlers

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CImportExcelDlg::ViewGridExcelTables()
/// \brief		���� ���� ���̺� ��� ��� 
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CImportExcelDlg::ViewGridExcelTables()
{
	GV_ITEM Item;

	int nSize = m_ExcelData.m_vectTableInfo.size();
	for(int i=0; i < nSize; i++)
	{
		TABLEINFO *pTable = &(m_ExcelData.m_vectTableInfo[i]);
		Item.mask		= GVIF_TEXT|GVIF_FORMAT;
		Item.nFormat	= DT_LEFT|DT_VCENTER|DT_SINGLELINE;

		int nNewRowIdx = m_ExcelTables.GetRowCount();
		m_ExcelTables.SetRowCount(nNewRowIdx+1);

		//select�� ���� GridDetail GridCtrl�� �־��ش�.
		Item.row		= nNewRowIdx;
		Item.col		= 0;
		Item.strText.Format("%s", (char*)pTable->szTableName);
		Item.strText.Remove('$');
		m_ExcelTables.SetItem(&Item);

		Item.col++;
		Item.strText.Format("%d", pTable->nRecordCount);
		m_ExcelTables.SetItem(&Item);

		Item.mask		= GVIF_FORMAT|GVIF_IMAGE|GVIF_BKCLR;
		Item.col++;
		Item.iImage		= 0;
		Item.crBkClr	= RGB(200, 200, 200);
		m_ExcelTables.SetItem(&Item);
	}
	
	m_ExcelTables.UpdateData();
}

/////////////////////////////////////////////////////////////////////////////
// CImportExcelDlg message handlers

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CImportExcelDlg::OnGridExcelTablesClick(NMHDR *pNotifyStruct, LRESULT* pResult)
/// \brief		����� ���̺� ���� üũ
/// \author		jwlee
/// \date		2009-07-29 ~ 2009-07-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CImportExcelDlg::OnGridExcelTablesClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	
	if(0 > pItem->iRow)
	{
		return;
	}

	if (GRID_COL_INDEX_EXCEL_TABLES != pItem->iColumn)
	{
		return;
	}

	// all Ŭ�� �� ��ü ����
	if(0 == pItem->iRow)
	{
		int nImageIndex = 0;
		CGridCellBase *pGCell = m_ExcelTables.GetCell(0, pItem->iColumn);
		if(0 == pGCell->GetImage())
		{
			nImageIndex	= 1;
		}

		for(int i=0; i < m_ExcelTables.GetRowCount(); i++)
		{
			pGCell = m_ExcelTables.GetCell(i, pItem->iColumn);
			pGCell->SetImage(nImageIndex);
			m_ExcelTables.Invalidate();
		}
		return;
	}

	// �κ� ����
	CGridCellBase *pGCell = m_ExcelTables.GetCell(pItem->iRow, pItem->iColumn);
	if(0 == pGCell->GetImage())
	{
		pGCell->SetImage(1);
	}
	else
	{
		pGCell->SetImage(0);
	}

	m_ExcelTables.Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// CImportExcelDlg message handlers

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CImportExcelDlg::InitAllWindows()
/// \brief		���ϸ� edit, db ���â, �׸��� â �ʱ�ȭ 
/// \author		jwlee
/// \date		2009-07-29 ~ 2009-07-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CImportExcelDlg::InitAllWindows()
{
	m_Select_ExcelFile.SetWindowText("");
	this->InitGridExcelTables();
	m_DBTables.SetWindowText("");
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-11-10 by cmkwon, SCMonitor�� ���� ���� �о�ֱ� ���� - 
/// \author		cmkwon
/// \date		2009-11-10 ~ 2009-11-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CImportExcelDlg::InitGridSelectedTables(void)
{
	for(int i=0; i < m_ExcelTables.GetRowCount(); i++)
	{
		CGridCellBase *pGCell = m_ExcelTables.GetCell(i, GRID_COL_INDEX_EXCEL_TABLES);
		pGCell->SetImage(0);
		m_ExcelTables.Invalidate();
	}
	m_ExcelTables.Invalidate();
}

// 2009. 10. 27 by jskim ����� Ȯ��
LONG CImportExcelDlg::OnUpdateExcelloadProgressPos(WPARAM wParam, LPARAM lParam)
{
	m_TotalProgress.SetPos(m_nCurrentTotalCount + lParam);
	m_RecordProgress.SetPos(lParam);
	return TRUE;
}

LONG CImportExcelDlg::OnUpdateExcelloadProgressRange(WPARAM wParam, LPARAM lParam)
{
	m_RecordProgress.SetRange32(0,lParam);
	return TRUE;
}
//end 2009. 10. 27 by jskim ����� Ȯ��