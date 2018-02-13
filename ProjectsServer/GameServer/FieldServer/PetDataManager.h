//////////////////////////////////////////////////////////////////////
//	PetDataManager.h: interface for the CPetManager class.
//
//	Create Info : 2010. 06. 16. shCho & hsLee
//
//////////////////////////////////////////////////////////////////////

#ifndef __PET_DATA_MANAGER__
#define __PET_DATA_MANAGER__

//#pragma once

#include "AtumParam.h"
#include "AtumFieldDBManager.h"

class CPetDataManager
{
	public :
		friend CAtumDBHelper;

	public :
				 CPetDataManager();
		virtual ~CPetDataManager();

	void Clear(); // 2011-09-06 by hskim, ��Ʈ�� �ý��� 2��	
		void Initialize ( void );
		void Release ( void );

		BOOL LoadDBData ( CODBCStatement *i_odbcstmt, ez_map<INT, ITEM*> *pMapItemInfo);		// 2011-09-06 by hskim, ��Ʈ�� �ý��� 2��	
		BOOL LoadDB_PetBaseData ( CODBCStatement *i_odbcstmt );
		BOOL LoadDB_PetLevelData ( CODBCStatement *i_odbcstmt );

		const tPET_BASEDATA *GetPetBaseData ( const INT a_iPetIndex );
		const tPET_LEVEL_DATA *GetPetLevelData ( const INT a_iPetIndex , const INT a_Level );
		const tPET_BASE_ALL_DATA *GetPetData ( const INT a_iPetIndex );

		// 2010-06-15 by shcho&hslee ��ý��� - �� ���� ó��(�� ���� ������ �ѹ� ���ϴ� �Լ� �߰�)
		INT GetPetLevelDataFromAttackItemNum ( const INT a_iPetIndex , const INT a_iPetLevel );
		// END 2010-06-15 by shcho&hslee ��ý��� - �� ���� ó��(�� ���� ������ �ѹ� ���ϴ� �Լ� �߰�)

	///////////////////////////////////////////////////////////////////////////////////////
	// start 2011-08-17 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ� ���� ����

	BOOL CreateIndexKitLevel(ez_map<INT, ITEM*> *pMapItemInfo);
	BOOL CreateIndexAutoSkill(ez_map<INT, ITEM*> *pMapItemInfo);

	BOOL IsAvailableAutoKit(DestParam_t DestParam, INT iLevel, SPET_KIT_SLOT_DATA *pAutoKitSlot);
	BOOL IsAvailableAutoSkill(USHORT wUnitKind, SPET_AUTOSKILL_SLOT_DATA *pAutoSkill);

	tPET_LEVEL_DATA *GetPetLevel(INT a_iPetIndex, Experience_t PetExperience);		// ���� ����ġ�� �´� ������ �����ش�

	// end 2011-08-17 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ� ���� ����
	///////////////////////////////////////////////////////////////////////////////////////

	private :
		vectorPetDataInfo m_PetDataInfo;

	///////////////////////////////////////////////////////////////////////////////////////
	// start 2011-09-06 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ� ���� ����

	vectorPetKitLevel m_KitLevelHP;
	vectorPetKitLevel m_KitLevelShield;
	vectorPetKitLevel m_KitLevelSP;

	vectorPetAutoSkill m_KitLevelAGear;
	vectorPetAutoSkill m_KitLevelBGear;
	vectorPetAutoSkill m_KitLevelIGear;
	vectorPetAutoSkill m_KitLevelMGear;

	// end 2011-09-06 by hskim, ��Ʈ�� �ý��� 2�� - �ڷ� ���� ����
	///////////////////////////////////////////////////////////////////////////////////////

};

#endif