// LoadExcelFile.h: interface for the CLoadExcelFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOADEXCELFILE_H__C6CD4C45_00FE_48CF_BD8C_BA622A09307C__INCLUDED_)
#define AFX_LOADEXCELFILE_H__C6CD4C45_00FE_48CF_BD8C_BA622A09307C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumParam.h"
#include "AtumDBhelper.h"		// 2009-10-23 by cmkwon, Monitor �������� �о�ֱ�� üũ �߰� - 

#import "C:\Program Files\Common Files\System\ADO\msado15.dll" no_namespace rename("EOF", "EndOfFile")

#define MAX_TABLE_NAME	50

typedef struct{
	_bstr_t szTableName;
	int nRecordCount;
}TABLEINFO;

struct SGAME_INFO_TABEL_INFO		// 2009-10-23 by cmkwon, Monitor �������� �о�ֱ�� üũ �߰� - 
{
	char *	m_szTableName;
	int		m_nCBCount;
	char *	m_szFieldNames[SIZE_MAX_TB_FIELD_COUNT];
	BOOL CheckFieldNameList(vectstring *i_pvectFieldNameList, char *o_pszErrTxt);	// 2010-04-20 by cmkwon, SCMonitor ���� ���� �о�ֱ� ���� ���� �߰� - 
};
extern SGAME_INFO_TABEL_INFO *FindGameInfoTableInfoByTableName(char *i_szTableName);	// 2009-10-23 by cmkwon, Monitor �������� �о�ֱ�� üũ �߰� - 

// 2009-07-09 by jwlee, ���� ���� ���̺� ������ DB�� �о�ֱ� �� ���� -
class CLoadExcelFile  
{
///////////////////////////////////////////////////////////////////////////////
// public member functions
///////////////////////////////////////////////////////////////////////////////
public:
	CLoadExcelFile();
	virtual ~CLoadExcelFile();
	
	// Connect Excel
	BOOL ConnectExcel(char *i_szFileName);
	
	// Loading TableList and Record Count
	BOOL LoadTableList(void);

	// Load ExcelData
	BOOL LoadExcelData(vector<CString> *i_pvectTables);

///////////////////////////////////////////////////////////////////////////////
// private member functions
///////////////////////////////////////////////////////////////////////////////
private:
	// Loading ExcelTableData
	void LoadExcelItem(void);
	void LoadExcelMonsterItem(void);
	void LoadExcelRareItemInfo(void);
	void LoadExcelShop(void);
	void LoadExcelCityTargetWarpMap(void);
	void LoadExcelItemMixingInfo(void);
	void LoadExcelItemMixingElement(void);		// 2012-02-20 by hskim, DB ����ȭ - ItemMixingInfo
	void LoadExcelEnchantInfo(void);
	void LoadExcelStrategyPointSummonMapIndex(void);
	void LoadExcelArenaInfo(void);
	void LoadExcelArenaMapinfo(void);
	void LoadExcelMysteryItemDrop(void);
	void LoadExcelInfluenceWar(void);
	void LoadExcelOutPostNextWarTime(void);
	void LoadExcelActionByLevel(void);
	void LoadExcelMapInfo(void);
	void LoadExcelBuildingNPC(void);
	void LoadExcelMonster(void);
	void LoadExcelTutorialInfo(void);
	void LoadExcelMapObject(void);
	void LoadExcelLuckyMachine(void);
	void LoadExcelQuestMatching(void);
	void LoadExcelItemMatching(void);
	void LoadExcelItemInfo(void);
	void LoadExceInfluenceRate(void);		// 2009-09-16 by cmkwon, ���� �ʱ�ȭ�� ���¡ ���� ���� - ti_InfluenceRate ���̺� �߰�, 
	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - Excel -> DB �б�
	void LoadExcelHPAction(void);
	void LoadExcelCinema(void);
	void LoadExcelInfinityMonster(void);
	void LoadExcelRevision(void);
	void LoadExcelInfinityMode(void);
	void LoadExcelTender(void);	
	void LoadExcelInfinityShop(void);					// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ����
	void LoadExcelItemParamOverlap(void);				// 2010-01-18 by cmkwon, ������ ���� Parameter �ߺ� üũ �ý��� ���� - 
	void LoadExcelInvokingWearItemDPNum(void);				// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
	void LoadExcelInvokingWearItemDPNumByUse(void);			// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
	void LoadExeclLevelAdjustment(void);			// 2010-05-24 by shcho, ���Ǵ�Ƽ ���̵� ���� - ���Ǵ�Ƽ ���̵� ������ ���� �� ������Ʈ
	void LoadExeclLevelAdjustmentReward(void);		// 2010-05-24 by shcho, ���Ǵ�Ƽ ���̵� ���� - ���Ǵ�Ƽ ���̵� ������ ���� �� ������Ʈ
	void LoadExeclBurningMap(void);		// 2010-08-05 by dhjin, ���׸� - 
	void LoadExeclPetBaseData(void);		// 2010-06-15 by shcho&hslee ��ý��� - �� �⺻ ���� ���� �б�
	void LoadExeclPetLevelData(void);		// 2010-06-15 by shcho&hslee ��ý��� - �� ���� ���� ���� �б�
	void ExeclLoadOperatorData(void);		// 2010-06-15 by shcho&hslee ��ý��� - DB �б� �߰� �۾� �� ���۷����� ����.
	void LoadExeclDissolutionItemData(void);	// 2010-08-31 by shcho&jskim �����ۿ��� �ý��� - ���� ������ ����.
	void LoadExeclMonsterMultiTargetData(void);		// 2011-03-17 by hskim, ���Ǵ�Ƽ 3�� - ���� ��Ƽ Ÿ���� ��� �߰�

	// start 2011-05-23 by hskim, ���Ǵ�Ƽ 3�� - ��������Ʈ ����
	void LoadExeclWayPointData(void);
	void LoadExeclWayPointMoveData(void);
	// end 2011-05-23 by hskim, ���Ǵ�Ƽ 3�� - ��������Ʈ ����

	void LoadExeclTriggerMapBuffData(void);		// 2011-10-10 by hskim, EP4 [Ʈ���� �ý���] - ȭ���� / �� ��ǳ

	// start 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
	void LoadExeclMapTrigger(void);
	void LoadExeclTriggerFunctionCrystal(void);
	void LoadExeclTriggerCrystalGroup(void);
	void LoadExeclTriggerCrystalDestroyGroup(void);
	// end 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���

	// start 2011-11-07 by hskim, EP4 [Ʈ���� �ý���] - NCG ������ �ý���
	void LoadExeclTriggerFunctionNGCInflWar(void);
	void LoadExeclTriggerNGCInflWarMonsterGroup(void);
	// end 2011-11-07 by hskim, EP4 [Ʈ���� �ý���] - NCG ������ �ý���

	// 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý���
	void LoadExeclTriggerFunctionNGCOutPostList(void);
	void LoadExeclTriggerOutPostBossKillInfoList(void);
	void LoadExeclTriggerCrystalBuffList(void);
	// end 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý���

	// Delete ExcelTableData
	void DeleteExcelData(void);


	// 2009-10-23 by cmkwon, Monitor �������� �о�ֱ�� üũ �߰� - 
	BOOL GetFieldNameList(vectstring *o_pvectFieldNameList, _RecordsetPtr i_pRecordSet);
	
///////////////////////////////////////////////////////////////////////////////
// public member variables
///////////////////////////////////////////////////////////////////////////////
public:
	vector<ITEM>				vectItem;
	vector<MONSTER2ITEM>		vectMonsterItem;
	vector<RARE_ITEM_INFO>		vectRareItemInfo;
	vectSHOP_INFO				vectShopInfo;
	vectCITY_TARGET_WARPMAP		vectCityTargetWarpMap;
	vector<ITEM_MIXING_INFO_TOOL>		vectItemMixingInfo;			// 2012-02-20 by hskim, DB ����ȭ - ItemMixingInfo
	vector<ITEM_MIXING_ELEMENT_TOOL>	vectItemMixingElement;		// 2012-02-20 by hskim, DB ����ȭ - ItemMixingInfo
	vector<ENCHANT_INFO>		vectEnchantInfo;
	vectSTRATEGYPOINT_SUMMON_MAPINDEX	vectStrategyPointSummonMapIndex;

	vectorSDBARENA_INFO			vectArenaInfo;
	vectorSDBARENA_MAPINFO		vectArenaMapInfo;
	vectMYSTERY_ITEM_DROP		vectMysteryItem;
	vectDB_INFLUENCE_WAR_INFO	vectInfluenceWarInfo;
	vectSOutPostInfo			vectOutPostInfo;
	vectSACTION_BY_LEVEL_DB		vectActoinByLevel;
	vectorMAP_INFO				vectMapInfo;
	vector<BUILDINGNPC>			vectBuildingNpc;
	vectMONSTER_DATA			vectMonsterInfo;
	vectSTutorialInfo			vectTutorialInfo;
	vectMAPOBJECT				vectMapObject;
	vectLUCKY_MACHINE			vectLuckyMachine;
	vectSQUEST_MATCHING			vectQuestMatching;
	vectSITEM_MATCHING			vectItemMatching;
	vectItem_Info				vectItemInfo;
	vectSTI_INFLUENCERATE		vectInfluenceRate;		// 2009-09-16 by cmkwon, ���� �ʱ�ȭ�� ���¡ ���� ���� - ti_InfluenceRate ���̺� �߰�, 
	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - Excel -> DB �б�
	vectHPAction				HPActionList;
	vectorCinemaInfo			CinemaInfoList;
	vectorRevisionInfo			RevisionInfoList;
	vectorInfinityMonsterInfo	InfinityMonsterInfoList;
	vectorInfinityModeInfo		InfinityModeInfoList;
	vectorTenderInfo			TenderInfoList;
	vectorInfinityShopInfo		InfinityShopInfoList;		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ����
	vectSITEM_PARAM_OVERLAP		ItemParamOverlapList;		// 2010-01-18 by cmkwon, ������ ���� Parameter �ߺ� üũ �ý��� ���� - 
	InvokingWearItemDestParamList InvokingItemDPList;		// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
	InvokingWearItemDestParamList InvokingItemDPByUseList;		// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
	vectorInfinity_DifficultyInfo_Monster  LevelAdjustmentList;		// 2010-05-24 by shcho, ���Ǵ�Ƽ ���̵� ���� - ���Ǵ�Ƽ ���̵� ������ ���� �� ������Ʈ
	vectorInfinity_DifficultyInfo_Bonus  LevelAdjustmentRewardList;	// 2010-05-24 by shcho, ���Ǵ�Ƽ ���̵� ���� - ���Ǵ�Ƽ ���̵� ������ ���� �� ������Ʈ
	BurningMapInfoList			BurningMapList;			// 2010-08-05 by dhjin, ���׸� - 
	vectorPetBaseData			PetBaseData;			// 2010-06-15 by shcho&hslee ��ý��� - �� �⺻ ���� ���� �б�
	vectorPetLevelData			PetLevelData;			// 2010-06-15 by shcho&hslee ��ý��� - �� ���� ���� ���� �б�
	vectorOperatorInfo			OperatorDataList;		// 2010-06-15 by shcho&hslee ��ý��� - DB �б� �߰� �۾� �� ���۷����� ����.
	vectorDissolutionItemInfo	DissolutionIteminfo;	// 2010-08-31 by shcho&jskim �����ۿ��� �ý��� - ���� ������ ����.
	vectorMonsterMultiTarget	MonsterMultiTargetList;		// 2011-03-17 by hskim, ���Ǵ�Ƽ 3�� - ���� ��Ƽ Ÿ���� ��� �߰�
	// start 2011-05-23 by hskim, ���Ǵ�Ƽ 3�� - ��������Ʈ ����
	vectorWayPoint				WayPointList;
	vectorWayPointMove			WayPointMoveList;
	// end 2011-05-23 by hskim, ���Ǵ�Ƽ 3�� - ��������Ʈ ����
	vectorTriggerMapBuff		TriggerMapBuffList;		// 2011-10-10 by hskim, EP4 [Ʈ���� �ý���] - ȭ���� / �� ��ǳ
	vector<TABLEINFO>			m_vectTableInfo;
	// start 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
	vectorTriggerMap					m_vectTriggerMapList;
	vectorTriggerFunctionCrystal		m_vectTriggerFunctionCrystalList;
	vectorTriggerCrystalGroup			m_vectTriggerCrystalGroupList;
	vectorTriggerCrystalDestroyGroup	m_vectTriggerCrystalDestroyGroupList;
	// end 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
	// start 2011-11-07 by hskim, EP4 [Ʈ���� �ý���] - NCG ������ �ý���
	vectorTriggerFunctionNGCInflWar		m_vectTriggerFunctionNGCInflWarList;
	vectorTriggerNGCInflWarMonsterGroup	m_vectTriggerNGCInflWarMonsterGroupList;
	// end 2011-11-07 by hskim, EP4 [Ʈ���� �ý���] - NCG ������ �ý���
	// 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý���
	vectorTriggerFunctionNGCOutPost		m_vectTriggerFunctionNGCOutPostList;
	vectorTriggerOutPostBossKillInfo	m_vectTriggerOutPostBossKillInfoList;
	vectorTriggerCrystalBuff			m_vectTriggerCrystalBuff;

///////////////////////////////////////////////////////////////////////////////
// private member variables
///////////////////////////////////////////////////////////////////////////////
private:
	char szStrCon[MAX_PATH];
	char szFileName[MAX_PATH];
	char m_szStrTable[MAX_PATH];
	char m_szStrQuery[MAX_PATH];

	_ConnectionPtr	m_Connection;
	_RecordsetPtr	m_RecordSet;
	_RecordsetPtr	m_Sheetname;
};

#endif // !defined(AFX_LOADEXCELFILE_H__C6CD4C45_00FE_48CF_BD8C_BA622A09307C__INCLUDED_)
