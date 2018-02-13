// AtumDatabase.h: interface for the CAtumDatabase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATUMDATABASE_H__3842E2DD_C6C4_4D3B_9CFB_18F7EA62E851__INCLUDED_)
#define AFX_ATUMDATABASE_H__3842E2DD_C6C4_4D3B_9CFB_18F7EA62E851__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

// 2011. 03. 08 by jskim ����3�� ���� - ����Ʈ �ý���		
#include "AtumParam.h"

// typedef enum 
// {
// 	DB_ITEM,
// 	DB_MONSTER_INFO,
// 	DB_MAPOBJECT,
// 	DB_RARE_ITEM,
// 	DB_BUILDINGNPC,
// 	DB_GUILD_MARK,
// 	DB_MAP_INFO,
// 	DB_MIXING_INFO,
// 	DB_MYSTERY_ITEM_DROP,
// 	// 2009-03-04 by bhsohn ��Ű �ӽ� ������
// 	DB_LUCKYMACHINE,
// 	// end 2009-03-04 by bhsohn ��Ű �ӽ� ������
// 	// 2010. 02. 11 by ckPark �ߵ��� ����������
// 	DB_INVOKINGWEARITEM_DPNUM,			// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
// 	DB_INVOKINGWEARITEM_DPNUM_BY_USE,	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
// 	// end 2010. 02. 11 by ckPark �ߵ��� ����������
// 
// 	// 2010-08-10 by dgwoo ���׸� �ý���
// 	DB_BURNING_MAP,
// 	// 2010-08-10 by dgwoo ���׸� �ý���
// 
// 	// 2010-06-15 by shcho&hslee ��ý���
// 	DB_PET_BASEDATA,						// 2010-06-15 by shcho&hslee ��ý��� - �� �⺻ ����.
// 	DB_PET_LEVELDATA,						// 2010-06-15 by shcho&hslee ��ý��� - �� ������ ��������.
// 	DB_OPERATOR_ACTION,						// 2010-06-15 by shcho&hslee ��ý��� - ���۷����� ����
// 	// End 2010-06-15 by shcho&hslee ��ý���
// 	DB_DISSOLUTION_ITEM,						// 2010-08-31 by shcho&&jskim, ���� �ý��� ����
// 	DB_MONSTER_ADD_TARGET,	
// 
// } DB_TYPE;

// ITEM
// struct DATA_HEADER
// {
// 	DB_TYPE nType;
// 	int nDataCount;
// };

// end 2011. 03. 08 by jskim ����3�� ���� - ����Ʈ �ý���		

// GUILD_MARK_VERSION FILE STRUCT
//----------------------------------------------------------------------------------------------------
//	          8           |				   	  ARRAY_(GUILD_MARK_VERSION):nNumber                      |
//----------------------------------------------------------------------------------------------------
//	          	          |       4     |      4             |      4       |ARRAY_(char):nSizeOfImage|
//----------------------------------------------------------------------------------------------------
//    nType   |  nNumber  |    nVersion | nGuildUniqueNumber | nSizeOfImage |              char       |
//----------------------------------------------------------------------------------------------------

class CINFImageEx;                     // 2011. 10. 10 by jskim UI�ý��� ����
struct GUILD_MARK_VERSION
{
	int nVersion;
	int nGuildUniqueNumber;
	int nSizeOfImage;
	CINFImage* pImage;               // 2011. 10. 10 by jskim UI�ý��� ���� 
	char* pData;
};

typedef std::vector< OperatorInfo* > vectorPreOperatorInfo;

// 2013-02-26 by bhsohn �ΰ��� ���� �˻� ó��
struct structResultMixingInfo		// ����� �˻� ���� 
{
	ITEM_MIXING_INFO struMixingInfo;
	char itemname[SIZE_MAX_ITEM_NAME];
	INT LinkItem;		// �Ƹ� ���� ī�� �ҽ� �ε���
};

typedef map<INT,CVectorMixingInfo*>			 CMapMixingSearchTable;
typedef map<INT,CVectorMixingInfo*>::iterator CMapMixingSearchTableIterator;

// 2013-06-14 by bhsohn �ΰ��� ���ս� ����
// typedef std::map<std::string, structResultMixingInfo*>	CMapResultMixingInfo;
// typedef std::map<std::string, structResultMixingInfo*>::iterator CMapResultMixingInfoIterator;
typedef vector<structResultMixingInfo*>					CVectorRInfo;					
typedef vector<structResultMixingInfo*>::iterator		CVectorRInfoIterator;

typedef std::map<std::string, CVectorRInfo>				CMapResultMixingInfo;
typedef std::map<std::string, CVectorRInfo>::iterator	CMapResultMixingInfoIterator;
// END 2013-06-14 by bhsohn �ΰ��� ���ս� ����
// END 2013-02-26 by bhsohn �ΰ��� ���� �˻� ó��

// 2013-04-11 by bhsohn �ΰ��� ����â ���� ���� ��þƮ �ý��� �˻�
enum
{
	LOOK_MIXING_NONE	= 0,					// ����
	LOOK_MIXING_PRIMARY_WEAPON_A,				// ���� ���� - �⺻ ���� A ���
	LOOK_MIXING_PRIMARY_WEAPON_BMI,				// ���� ���� - �⺻ ���� BMI ���
	LOOK_MIXING_SECOND_WEAPON_A,				// ���� ���� - ��� ���� A ���
	LOOK_MIXING_SECOND_WEAPON_BMI,				// ���� ���� - ��� ���� BMI ���
	LOOK_MIXING_EFFECT_PRIMARY_WEAPON,			// ���� ���� - �⺻ ���� ����Ʈ
	LOOK_MIXING_EFFECT_SECOND_WEAPON,			// ���� ���� - ��� ���� ����Ʈ
	LOOK_MIXING_DEFENSE_B,						// �Ƹ� ���� - B ���
	LOOK_MIXING_DEFENSE_M,						// �Ƹ� ���� - M ���
	LOOK_MIXING_DEFENSE_A,						// �Ƹ� ���� - A ���
	LOOK_MIXING_DEFENSE_I,						// �Ƹ� ���� - I ���	
	LOOK_MIXING_PET_ITEM,						// ��Ʈ�� ���� - ��Ʈ�� �Ⱓ�� ���� ����
////////////////////////////// �������� ī��� //////////////////////////////
	LOOK_MIXING_SHAPE_ITEM,						// ���� ī�� - 
	LOOK_MIXING_SHAPE_EFFECT,					// ����Ʈ ī��- 	
	LOOK_MIXING_SHAPE_PRIMARY_WEAPON_A,			// ���� ���� - �⺻ ���� A ���
	LOOK_MIXING_SHAPE_PRIMARY_WEAPON_BMI,		// ���� ���� - �⺻ ���� BMI ���
	LOOK_MIXING_SHAPE_SECOND_WEAPON_A,			// ���� ���� - ��� ���� A ���
	LOOK_MIXING_SHAPE_SECOND_WEAPON_BMI,		// ���� ���� - ��� ���� BMI ���
	LOOK_MIXING_SHAPE_EFFECT_PRIMARY_WEAPON,	// ���� ���� - �⺻ ���� ����Ʈ
	LOOK_MIXING_SHAPE_EFFECT_SECOND_WEAPON,		// ���� ���� - ��� ���� ����Ʈ
	LOOK_MIXING_SHAPE_DEFENSE_B,				// �Ƹ� ���� - B ���
	LOOK_MIXING_SHAPE_DEFENSE_M,				// �Ƹ� ���� - M ���
	LOOK_MIXING_SHAPE_DEFENSE_A,				// �Ƹ� ���� - A ���
	LOOK_MIXING_SHAPE_DEFENSE_I,				// �Ƹ� ���� - I ���
	LOOK_MIXING_SHAPE_DEFENSE_INIT,				// �Ƹ� ���� - �ʱ�ȭ
	LOOK_MIXING_SHAPE_SECOND_EFFECT_INIT,		// ����Ʈ ���� - ��� ���� �ʱ�ȭ 
	LOOK_MIXING_SHAPE_PRIMARY_EFFECT_INIT,		// ����Ʈ ���� - �⺻ ���� �ʱ�ȭ 
	LOOK_MIXING_SHAPE_PET_ITEM,					// ��Ʈ�� ���� - ��Ʈ�� �Ⱓ�� ���� ����
	MAX_LOOK_MIXING_ITEM						// ������ ��
};
// END 2013-04-11 by bhsohn �ΰ��� ����â ���� ���� ��þƮ �ý��� �˻�

class CAtumDatabase  
{
public:
	CAtumDatabase();
	virtual ~CAtumDatabase();
	
	void InitDeviceObjects();
	void RestoreDeviceObjects();
	void InvalidateDeviceObjects();
	void DeleteDeviceObjects();

	ITEM *	GetServerItemInfo(int nItemNum);
	void GetServerItemInfo(vectorItemInfoList * pItemList, int nKind);
	ITEM * UpdateItemInfo(ITEM &item);
	ITEM * UpdateSimpleItemInfo(MEX_ITEM_INFO &simpleInfo);
	ITEM * GetServerMonsterTransItemInfo(int nMonUintKind);

	MEX_MONSTER_INFO * CheckMonsterInfo(int nMonType);
	MEX_MONSTER_INFO * UpdateMonsterInfo(MEX_MONSTER_INFO* pMonsterInfo);

	void FindMysteryInfo(INT i_nItemNum,vector<MYSTERY_ITEM_DROP*> *pVecBuildingNPC);
	
	// 2008-12-29 by bhsohn ��Ű �ӽ� ���� ����Ʈ �߰�
	MYSTERY_ITEM_DROP* GetMysteryInfo(INT	i_MysteryItemDropNum, INT i_nItemNum);
	// end 2008-12-29 by bhsohn ��Ű �ӽ� ���� ����Ʈ �߰�

	MAPOBJECTINFO * CheckObjectInfo(int nObjType);
	MAPOBJECTINFO * UpdateObjectInfo(MAPOBJECTINFO & obj);

	RARE_ITEM_INFO * GetServerRareItemInfo(int nCodeNum);
	// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
	CMapRareItemInfoList GetServerRareItemList();
	//end 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
	RARE_ITEM_INFO * UpdateRareItemInfo(RARE_ITEM_INFO &rareItem);

	BUILDINGNPC * GetServerBuildingNPCInfoFromNPCIndex(int nNPCIndex);
	BUILDINGNPC * GetServerBuildingNPCInfo(int nBuildingIndex);
	BUILDINGNPC * UpdateBuildingNPCInfo(BUILDINGNPC & buildingInfo);

	// 2013-05-07 by bhsohn ��������Ʈ ������ �ý���
	BUILDINGNPC * GetBuildingNPCInfo(int nNPCIndex);
	// END 2013-05-07 by bhsohn ��������Ʈ ������ �ý���

	CINFImage*	GetGuildMark(UINT nGuildUniqueNumber);
	char* GetGuildName(UINT nGuildUniqueNumber);
	void CheckGuildMarkVersion(int nGuildUniqueNumber, int nGuildVersion );
	void UpdateGuildMark(UINT nGuildUniqueNumber, int nGuildVersion, char* pData, int nSize);

	MEX_OTHER_GUILD_INFO* GetOtherGuildInfo( int nGuildUniqueNumber );
	void UpdateOtherGuildInfo(MEX_OTHER_GUILD_INFO* pMsg);

	MAP_INFO * GetMapInfo(MapIndex_t nMapIndex);
	void FindBuildingNPCListOccupyShop(int nCityMapIndex, vector<BUILDINGNPC*> *pVecItem);

	int GetItemNameRareString(char* pOutFullName, int nItemNumber, int nPrefix, int nSuffix);

	void ReloadDatabase();

	// 2007-12-17 by bhsohn ���� ���� ǥ��
	// ���� ������ �˾ƿ´�.
	ITEM_MIXING_INFO* GetMixerPrice(ITEM_MIXING_INFO* pMixInfo);

	// 2009-03-04 by bhsohn ��Ű �ӽ� ������
	int GetLuckyMachineLen();
	LUCKY_MACHINE_OMI * GetLuckyMachineInfo(int nIdx);
	// end 2009-03-04 by bhsohn ��Ű �ӽ� ������

	// 2010. 02. 11 by ckPark �ߵ��� ����������
	// �ߵ��� �����Ķ� ������
	void	GetInvokingWearItemDPList( CVectorInvokingWearItemDP* pVec, InvokingDestParamID_t nIdx );
	// ���� �����Ķ� ������
	void	GetInvokingWearItemDPByUseList( CVectorInvokingWearItemDP* pVec, InvokingDestParamID_t nIdx );
	// end 2010. 02. 11 by ckPark �ߵ��� ����������

	// 2009. 06. 09 by ckPark OMI ���� ���۽� üũ�� ����ϵ��� ����
	void	GetOMICheckSum( const char* szFilePath, BYTE omiCheckSum[32], long* pFileSize );
	// end 2009. 06. 09 by ckPark OMI ���� ���۽� üũ�� ����ϵ��� ����

	// 2010-06-15 by shcho&hslee ��ý���
	// �� ��ü ���� ������ ����. ( Pet Index ����. ) Public.
	tPET_BASE_ALL_DATA *GetPtr_PetAllDataByIndex ( const INT a_PetIndex );

	// �� ���� ���� ������ ����.
	tPET_LEVEL_DATA *GetPtr_PetLevelData ( const INT a_PetIndex , const INT a_PetLevel );

	OperatorInfo* GetPtr_OperatorAction( const INT FunctionIndex );
	// End 2010-06-15 by shcho&hslee ��ý���

	// 2010-08-31 by shcho&&jskim, ���� �ý��� ����
	tDissolutionItemInfo Get_DissolutionitemInfo( const INT itemNum );
	BOOL Is_DissolutionitemInfo( const INT itemNum );
	// end 2010-08-31 by shcho&&jskim, ���� �ý��� ����

	// 2010-08-10 by dgwoo ���׸� �ý���
	BURNING_MAP_INFO *GetPtr_BurningMapInfo(const MapIndex_t i_Index);
	// 2010-08-10 by dgwoo ���׸� �ý���

	// 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���
	int GetMultiTargetIndexSize(int nMonsterIndex);
	MONSTER_MULTI_TARGET GetMultiTargetPosion(int nMonsterIndex, int nPosion);
	vectorMonsterMultiTarget GetvecMultiTargetInfo(int nMonsterIndex);
	// end 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���
	// 2011. 11. 17 by jskim EP4 UI ����
	void DeleteGuildInfo();
	// end 2011. 11. 17 by jskim EP4 UI ����

	// 2013-01-24 by jhjang �ΰ��� ���ս� �ӽ÷� Ŀ�ǵ�� ä��â�� ����Ҽ� �ְ� ����
	CVectorMixingInfo* GetMixingItemList(int nItemNum);
	void InitMixingItemSearchTable();
	void ReleaseMixingItemSearchTable();
	// end 2013-01-24 by jhjang �ΰ��� ���ս� �ӽ÷� Ŀ�ǵ�� ä��â�� ����Ҽ� �ְ� ����

	// 2013-02-26 by bhsohn �ΰ��� ���� �˻� ó��

	// 2013-04-11 by bhsohn �ΰ��� ����â ���� ���� ��þƮ �ý��� �˻�
	int GetMaterialMixingItemList(int nItemCnt, char* pSearchName, vector<structResultMixingInfo> *o_pVecItemList);
	int GetMaterialMixingItemListIndex(INT nItemCnt, INT nSearchItemNum, vector<structResultMixingInfo> *o_pVecItemList);	
	int AddMaterialShapeItemList(int nItemCnt, ITEM* pItem, vector<structResultMixingInfo> *o_pVecItemList); 
	// END 2013-04-11 by bhsohn �ΰ��� ����â ���� ���� ��þƮ �ý��� �˻�

	// ������ ����� �˻�
	void InitResultMixingItemSearchTable();
	void ReleaseResultMixingItemSearchTable();		
	int  AddResultShapeItemList(int nItemCnt, ITEM* pItem, vector<structResultMixingInfo> *o_pVecItemList); // 2013-04-11 by bhsohn �ΰ��� ����â ���� ���� ��þƮ �ý��� �˻�
	int  GetResultMixingItemList(int nItemCnt, char* pItemName, vector<structResultMixingInfo> *o_pVecItemList);	
	int  GetResultMixingItemListIndex(int nItemCnt, INT nItemNum, vector<structResultMixingInfo> *o_pVecItemList);

	// ���� ���� ���� ������ ��ü Ÿ�Ե�
	void InitMixingItemListIndex();
	void AddMixingItemListIndex(ITEM* pAddItem, int nItemShapeType);
	void GetMixingItemListIndex(INT nItemNum, vector<structResultMixingInfo> *o_pVecItemList);
	void ReleaseMixingItemListIndex();	
	
	int GetFindLookChangeIdx(ITEM* pItem);	
	void Find_ItemType_To_FindIdx(int nShapeIdx, vector<INT> *pvecShape);

	ITEM* GetItemInfoLoadItemData(INT nItemNum);
//	ITEM_MIXING_INFO* GetResultMixingItemSearchTable(char* pItemName);
	
	// �ڵ��ϼ��� �ش�Ǵ� ������ �̸��� �˷��ش�.
	void GetSearchItemList(char* pSearchItem, std::map<INT, std::string> *o_pVecSearchList); 
	void GetSearchItemList_LookChangeInfo(char* pSearchItem, std::map<INT, std::string> *o_pVecSearchList);

	void SearchStringCull(char* pItemName, vector<string> *o_pVecItemList); // 2013-04-11 by bhsohn �ΰ��� ����â ���� ���� ��þƮ �ý��� �˻�

	// END 2013-02-26 by bhsohn �ΰ��� ���� �˻� ó��

	// 2013-03-13 by bhsohn ��ų ��ȣ ���� ����
#ifdef C_SKILL_NUMBERING_RENEWAL_BHSOHN
	int GetSkillBaseNum(int nItemNum);		// #define SKILL_BASE_NUM(x)	((int)x/10)*10	 // ��ų�� ItemNum���� BaseNum�� ����
	int GetSkillLevel(int nItemNum);		// #define SKILL_LEVEL(x)		(x - ((int)x/10)*10)				// ��ų�� ItemNum���� SkillLevel�� ����
	int GetSkillKind(int nItemNum);			// #define SKILL_KIND(x)		((int)x/1000) - ((int)x/10000)*10	// Skill ����(����: 0, ���: 1, ��Ʈ��: 2, �Ӽ�|����: 3)	
#endif
	int GetSkillKindIndex(int nItemNum);
	int GetSkillBaseSourceIndex(int nSourceIndex);
	// END 2013-03-13 by bhsohn ��ų ��ȣ ���� ����

	// 2013-05-28 by bhsohn �Ƹ� �÷��� �ý���
	void InitArmorCollectionInfo();
	// END 2013-05-28 by bhsohn �Ƹ� �÷��� �ý���

#ifdef _REWORKED_COLORSHOP
	std::vector<ITEM*> GetColourItemsByReqUnitKind(int ReqUnitKind);
#endif
		
protected:
	BOOL LoadItemData(FILE* fd, int nCount);
	BOOL LoadMonsterInfo(FILE* fd, int nCount);
	BOOL LoadMapObjectInfo(FILE* fd, int nCount);
	BOOL LoadRareItemData(FILE* fd, int nCount);
	BOOL LoadBuildingNPC(FILE* fd, int nCount);
	BOOL LoadGuildMark(FILE* fd, int nCount);
	BOOL LoadMapInfo(FILE* fd, int nCount);
	BOOL LoadMixingInfo(FILE* fd, int nCount);
	BOOL LoadMysteryInfo(FILE* fd, int nCount);
	// 2009-03-04 by bhsohn ��Ű �ӽ� ������
	BOOL LoadLuckyMachineInfo(FILE* fd, int nCount);
	// end 2009-03-04 by bhsohn ��Ű �ӽ� ������

	// 2010. 02. 11 by ckPark �ߵ��� ����������
	BOOL LoadInvokingWearItemDPNum( FILE* fd, int nCount );
	BOOL LoadInvokingWearItemDPNumByUse( FILE* fd, int nCount );
	// end 2010. 02. 11 by ckPark �ߵ��� ����������

	// 2010-06-15 by shcho&hslee ��ý���
	BOOL LoadPetBaseData ( FILE* fd , int nCount );
	BOOL LoadPetLevelData ( FILE* fd , int nCount );
	// 2010-08-10 by dgwoo ���׸� �ý���
	BOOL LoadBurningMapData ( FILE* fd , int nCount );
	BOOL LoadOperatorData ( FILE* fd , int nCount );
	// 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���
	BOOL LoadMultiTargetIndexData( FILE* fd , int nCount );
	// end 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���
	tPET_BASE_ALL_DATA *GetPtr_PetAllData ( const INT a_PetUID );

	void DeletePetData ( void );
	// End 2010-06-15 by shcho&hslee ��ý���
	// 2010-08-31 by shcho&&jskim, ���� �ý��� ����
	BOOL LoadDissolutionItemData( FILE* fd , int nCount );
	// end 2010-08-31 by shcho&&jskim, ���� �ý��� ����

	BOOL SaveItemData(FILE* fd);
	BOOL SaveMonsterInfo(FILE* fd);
	BOOL SaveMapObjectInfo(FILE* fd);
	BOOL SaveRareItemData(FILE* fd);
	BOOL SaveBuildingNPC(FILE* fd);
	BOOL SaveGuildMark(FILE* fd);
	BOOL SaveMapInfo(FILE* fd);
	BOOL SaveMixingInfo(FILE* fd);

	// 2010-06-15 by shcho&hslee ��ý���
	BOOL SavePetBaseData ( FILE* fd );
	BOOL SavePetLevelData ( FILE* fd );
	// End 2010-06-15 by shcho&hslee ��ý���

protected:
	CMapItemInfoList			m_mapItemInfo;					// �� Item Data
	CMapItemInfoList			m_mapItemInfoTemp;				// �� Item Data �ӽ�
	CMapIsLoadingList			m_mapItemGetInfo;				// �� Item Data - ������ ��û�ߴ��� 
	CMapRareItemInfoList		m_mapRareItemInfo;				// �� Rare Item Data
	CMapRareItemInfoList		m_mapRareItemInfoTemp;			// �� Rare Item Data �ӽ�
	CMapIsLoadingList			m_mapRareItemGetInfo;			// �� Rare Item Data - ������ ��û�ߴ��� 
	CMapMonsterInfoList			m_mapMonsterInfo;				// �� Monster Data
	CMapMonsterInfoList			m_mapMonsterInfoTemp;			// �� Monster Data �ӽ�
	CMapIsLoadingList			m_mapMonsterGetInfo;			// �� Monster Data - ������ ��û�ߴ��� 
	CMapObjectInfoList			m_mapObjectInfo;				// �� Object Data
	CMapObjectInfoList			m_mapObjectInfoTemp;			// �� Object Data �ӽ�
	CMapIsLoadingList			m_mapObjectGetInfo;				// �� Object Data - ������ ��û�ߴ��� 
	CMapBuildingNPCList			m_mapBuildingNPCInfo;			// �� BUILDINGNPC
	CMapBuildingNPCList			m_mapBuildingNPCInfoTemp;		// �� BUILDINGNPC �ӽ�
	CMapIsLoadingList			m_mapBuildingNPCGetInfo;		// �� BUILDINGNPC - ������ ��û�ߴ���
	CMapGuildMark				m_mapGuildMark;					// ��� ���� ����
	CMapIsLoadingList			m_mapGuildMarkGetInfo;			// ��� ���� ���� - ������ ��û�ߴ���
	CMapOtherGuildInfo			m_mapOtherGuildInfo;			// �ٸ� ĳ�� ��� ����
	CMapIsLoadingList			m_mapOtherGuildInfoGetInfo;		// �ٸ� ĳ�� ��� ����
	CMapMapInfo					m_mapMapInfo;					// �� ����(���̸�)
	CVectorMysteryItemDrop		m_VecMysteryItem;				// �̽��͸� ������, ��Ű �ӽ� ������.
	CVectorMixingInfo			m_VecMixingInfo;				// ���� ����
	// 2009-03-04 by bhsohn ��Ű �ӽ� ������
	CVectorLuckyMachine			m_VecLuckyMachine;				// ��Ű �ӽ� ������ ������
	// end 2009-03-04 by bhsohn ��Ű �ӽ� ������

	// 2013-02-26 by bhsohn �ΰ��� ���� �˻� ó��
	CMapResultMixingInfo*		m_pMapMapResultMixingInfo;	
	// END 2013-02-26 by bhsohn �ΰ��� ���� �˻� ó��


	// 2013-01-24 by jhjang �ΰ��� ���ս� �ӽ÷� Ŀ�ǵ�� ä��â�� ����Ҽ� �ְ� ����
	CMapMixingSearchTable*		m_pMapMixingSearchTable;			//�ΰ��� ���ս� �˻��� ����.
	// end 2013-01-24 by jhjang �ΰ��� ���ս� �ӽ÷� Ŀ�ǵ�� ä��â�� ����Ҽ� �ְ� ����

	// 2010. 02. 11 by ckPark �ߵ��� ����������
	CVectorInvokingWearItemDP	m_VecInvokingWearItemDPNum;
	CVectorInvokingWearItemDP	m_VecInvokingWearItemDPNumByUse;
	// end 2010. 02. 11 by ckPark �ߵ��� ����������

	// 2013-04-11 by bhsohn �ΰ��� ����â ���� ���� ��þƮ �ý��� �˻�	
	CMapItemInfoList*		m_pMapItemShapeType[MAX_LOOK_MIXING_ITEM];			
	// END 2013-04-11 by bhsohn �ΰ��� ����â ���� ���� ��þƮ �ý��� �˻�

	// 2009. 01. 12 by ckPark MapInfo ���� ���� ���� ���� �ȶ���

	MAP_INFO					m_DefMapInfo;

	// end 2009. 01. 12 by ckPark MapInfo ���� ���� ���� ���� �ȶ���

	// 2010-06-15 by shcho&hslee ��ý���
	CVectorPetAllData			m_VecPetAllData;	
	vectorPreOperatorInfo		m_vecOptratorAction;
	// End 2010-06-15 by shcho&hslee ��ý���

	// 2010-08-10 by dgwoo ���׸� �ý���
	CVectorBurningData			m_VecBurningMapData;

	// 2010-08-31 by shcho&&jskim, ���� �ý��� ����
	vectorDissolutionItemInfo	m_vecDissolutionItemInfo;
	// end 2010-08-31 by shcho&&jskim, ���� �ý��� ����
	// 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���
	map<INT,vectorMonsterMultiTarget>	m_mapMultiTagetInfo;
	// end 2011. 03. 08 by jskim ����3�� ���� - �� Ÿ�� �ý���

};

// 2013-03-13 by bhsohn ��ų ��ȣ ���� ����
#ifdef C_SKILL_NUMBERING_RENEWAL_BHSOHN
	int SKILL_BASE_NUM(int nItemNum);
	int SKILL_LEVEL(int nItemNum);
	int SKILL_KIND(int nItemNum);	
#endif
	int SKILL_KIND_INDEX(int nItemNum);
	int SKILL_BASE_SOURCEINDEX(int nSourceIndex);
// END 2013-03-13 by bhsohn ��ų ��ȣ ���� ����

#endif // !defined(AFX_ATUMDATABASE_H__3842E2DD_C6C4_4D3B_9CFB_18F7EA62E851__INCLUDED_)
