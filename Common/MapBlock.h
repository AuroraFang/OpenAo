#pragma once

#include "VMemPool.h"

/******************************************************************************
	ATUM Map ������ ���
	* Map > Block > Tile
******************************************************************************/
constexpr auto SIZE_MAP_TILE_SIZE = 40;
constexpr auto SIZE_MAP_BLOCK_SIDE = 960;		// �ϴ�, SIZE_VISIBLE_RADIUS/2 �׽�Ʈ�� ���� ������ �� ���ؾ� ��
constexpr auto SIZE_BLOCK_X = SIZE_MAP_BLOCK_SIDE;
constexpr auto SIZE_BLOCK_Z = SIZE_MAP_BLOCK_SIDE;
constexpr auto SIZE_CHARACTER_VISIBLE_SIDE = 2000;	// ĳ���� �þ�
constexpr auto SIZE_MONSTER_VISIBLE_SIDE = 1440;	// ���� �þ�

constexpr auto	SIZE_MAP_BLOCK_SIDE_FOR_NPCSERVER = 960;		// NPC Server���� ����ϴ� MapBlockSize
constexpr auto SIZE_BLOCK_X_FOR_NPCSERVER = SIZE_MAP_BLOCK_SIDE_FOR_NPCSERVER;
constexpr auto SIZE_BLOCK_Z_FOR_NPCSERVER = SIZE_MAP_BLOCK_SIDE_FOR_NPCSERVER;

constexpr auto SIZE_MONSTER_CREATION_RANGE = 180.0f;	// ���Ͱ� ������ �� �� �Ÿ� �ȿ� ĳ���Ͱ� ������ �������� ����
constexpr auto COUNT_MAX_ADMIN_SUMMON_MONSTER = 20;		// ���� ��ȯ�� �ѹ��� ��ȯ�Ҽ� �ִ� �ִ� ũ��
constexpr auto SIZE_TILE_ADMIN_SUMMON_MONSTER_REGION = 2;		// ���� ��ȯ�� �����ϴ� ������ Ÿ�� ��(2 + 1 + 2)

constexpr auto SIZE_MAX_EXPERIENCE_DIVISION_RADIUS = SIZE_MONSTER_VISIBLE_SIDE * 2;	// ����ġ �й�� ���͸� �������� �̰Ÿ� �ȿ� �־����

// �־��� ��ǥ�� Tile Index�� ��ȯ�Ѵ�
template<typename T> int CHANGE_TO_TILEINDEX(T x) { return (int)(x / SIZE_MAP_TILE_SIZE); }


//#define _MAP_BLOCK_LIST
typedef list<ClientIndex_t> ClientIndexList;
typedef vector<ClientIndex_t> ClientIndexVector;

constexpr auto MAX_DROPITEM_DELAY = 15000;	// miliseconds
constexpr auto MAX_DROPITEM_DELAY_ADMIN_COMMAND = 2000;	// miliseconds		// 2013-04-10 by hskim, ���� ��ɿ� ���� ��� �������� ��� �ð� 2�� ����
constexpr auto SIZE_MAX_DROP_ITEM_PRIORITY_INFO = 20;
constexpr auto DROP_ITEM_RADIUS = 80;

enum EnumPriorityInfoType
{
	PIT_NONE		= 0,
	PIT_CHARACTER	= 1,
	PIT_PARTY		= 2
};

struct PRIORITY_INFO
{
	EnumPriorityInfoType	ePrioryInfoType;			// ĳ���Ϳ� ���� ������, ��Ƽ�� ���� ������ ����
	UID32_t					CharacterUniqueNumber;		// PIT_CHARACTER�� �� ĳ���� UID
	PartyID_t				PartyID;					// PIT_PARTY�� �� PartyID
	DWORD					dwDelay;					// miliseconds
};

struct DROPITEM
{
	ITEM_GENERAL		*pItem;						// new�� �����Ǿ� �Ҵ��!
	PRIORITY_INFO		priorityInfoArray[SIZE_MAX_DROP_ITEM_PRIORITY_INFO];
	int					priorityInfoCount;
	DWORD				dwStartTick;				// miliseconds
	D3DXVECTOR3			Position;
	BOOL				bProcessing;
	UID32_t				FirstCharacterUID1;				// charcter UID
	BOOL				bTendering;					// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - TRUE = ��������				
	BOOL				bAdminCommand;				// 2013-04-10 by hskim, ���� ��ɿ� ���� ��� �������� ��� �ð� 2�� ����

	DROPITEM()
	{
		pItem				= nullptr;
		util::zero(priorityInfoArray, /*SIZE_MAX_DROP_ITEM_PRIORITY_INFO*sizeof(PRIORITY_INFO)*/ sizeof(priorityInfoArray));
		priorityInfoCount	= 0;
		dwStartTick			= 0;
		Position.x			= 0;
		Position.y			= 0;
		Position.z			= 0;
		bProcessing			= FALSE;
		FirstCharacterUID1	= 0;
		bTendering			= FALSE;			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - TRUE = ��������	
		bAdminCommand		= FALSE;			// 2013-04-10 by hskim, ���� ��ɿ� ���� ��� �������� ��� �ð� 2�� ����
	}

#ifdef _ATUM_SERVER
	void* operator new(size_t size) { return VMemPool::vmObjectNew(size); }
	void operator delete(void* p) { VMemPool::vmObjectDelete(p, sizeof(DROPITEM)); }
#endif
};

typedef mt_map<ITEM_GENERAL*, DROPITEM> mtmapDropItem;	// ITEM_GENERAL*�� �������� ���۽ô� UINT�� ��

/*
+ ���� ���� �ð� ���� ���
	- �������� �� �ð��� dwStartTick�� �����Ѵ�.
	- ������ ������ �ɷ��� ��
		(1) dwStartTick�� 0���� ũ��, currentTick�񱳸� �ؼ� ����ð��� �����Ѵ�.
			���� ���� �ð� ���ĸ� dwStartTick�� 0���� �ٲ� �� ó���Ѵ�.
		(2) dwStartTick�� 0�̰ų� ������ �ٷ� ���� ������ �Ѵ�.
+ ���� �ѹ��� DROPMINE�� pointer�� �Ѵ�.
+ �ڷᱸ���� set�� ���
+
*/
struct DROPMINE
{
	UID32_t				CharacterUniqueNumber;		// ����߸� character, ������
													// �̹� ���ĵ� ���� timer ó���� ���ؼ� INVAILD_UNIQUE_NUMBER�� �Ҵ�
													// ����ü�� ������ timer�� ����
	ClientIndex_t		ClientIndex;				// ����߸� character, ������
	ITEM*				pItemInfo;
	D3DXVECTOR3			Position;
	TimeUnit_t			dwStartTick;				// miliseconds, by GetTickCount()

	void * operator new(size_t size) { return VMemPool::vmObjectNew(size); }
	void operator delete(void* p) { VMemPool::vmObjectDelete(p, sizeof(DROPMINE)); }
};

struct RECTANGLE
{
	float fMinX;
	float fMinZ;
	float fMaxX;
	float fMaxZ;
};

struct TWO_BLOCK_INDEXES
{
	short sMinX;
	short sMinZ;
	short sMaxX;
	short sMaxZ;
};

struct BLOCK_INDEX
{
	short sX;
	short sZ;
};

// start 2011-03-23 by hskim, ���Ǵ�Ƽ 3�� - ���� ��Ƽ Ÿ���� ��� �߰�
struct MONSTER_MAPBLOCK_INFO
{
	ClientIndex_t	MonsterIndex;					// ���� ���� ��ȣ
	BLOCK_INDEX		Block;							// �� ��ǥ
	
	MONSTER_MAPBLOCK_INFO()		{ MonsterIndex = 0; Block.sX = 0; Block.sZ = 0; }
	MONSTER_MAPBLOCK_INFO(ClientIndex_t	Index, short x, short y)		{ MonsterIndex = Index; Block.sX = x; Block.sZ = y; }
	~MONSTER_MAPBLOCK_INFO()		{}
};

typedef mt_vector<MONSTER_MAPBLOCK_INFO>	mtvectorMTMonsterIndex;
// end 2011-03-23 by hskim, ���Ǵ�Ƽ 3�� - ���� ��Ƽ Ÿ���� ��� �߰�

class CSkinnedMesh;
struct OBJECTINFOSERVER
{
	DWORD			m_dwObjType;			// Object Ÿ��
	D3DXVECTOR3		m_vPos;
	D3DXVECTOR3		m_vVel;
	D3DXVECTOR3		m_vUp;
	EVENTINFO		m_EventInfo;
	CSkinnedMesh*	m_pSkinnedMesh;
	D3DXMATRIX		m_matrix;
	DWORD			dwObjBossMonResTime;		// 2006-11-22 by cmkwon, 
	MONSTER_INFO*	m_pMonsterInfo;			// 2007-08-18 by cmkwon, ������Ʈ ���� ��ȯ ������ MONSTER_INFO * �����ϱ�
	// start 2011-06-02 ���Ǵ�Ƽ 3�� - ���� 6 - �ֱ��� ��ȯ ��� ����
	BYTE			m_bNotCreateMonster;
	MONSTER_BALANCE_DATA	MonsterBalanceInfo;
	// end 2011-06-02 ���Ǵ�Ƽ 3�� - ���� 6 - �ֱ��� ��ȯ ��� ����
};

typedef vector<OBJECTINFOSERVER>	vectorObjectInfoServer;
typedef vector<OBJECTINFOSERVER*>	vectorObjectInfoServerPtr;

typedef mt_vector<ClientIndex_t>	mtlistUnitIndex_t;
typedef mt_vector<ClientIndex_t>	mtvectClientIndex_t;
typedef vector<ClientIndex_t>		vectClientIndex_t;			// 2006-07-24 by cmkwon
typedef mt_set<DROPMINE*>			mtsetDropMine;

////////////////////////////////////////////////////////////////////////////////
// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ������Ʈ�� ����!!!! �� �����...
typedef mt_vector<DWORD>				mtDeletedObjectInfoList;	// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
typedef mt_vector<OBJECTINFOSERVER>		mtNewObjectInfoList;		// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ

class CMapChannel;

class CMapBlock
{
public:
	CMapBlock();
	virtual ~CMapBlock();

	// ĳ���Ϳ� ���� ����
	inline BOOL InsertUnit(ClientIndex_t clientIndex);
	inline BOOL DeleteUnit(ClientIndex_t clientIndex);

	// ĳ����
	inline BOOL InsertCharacter(ClientIndex_t clientIndex);
	inline BOOL DeleteCharacter(ClientIndex_t clientIndex);
	inline void ResetCharacter();					// 2008-02-20 by dhjin, �Ʒ��� ���� -

	// ����
	inline BOOL InsertMonster(ClientIndex_t clientIndex);
	inline BOOL DeleteMonster(ClientIndex_t clientIndex);

	inline int GetNumCharachter();
	inline int GetNumMonster();
	inline int GetNumItem();

public:
	CMapChannel			*m_pMapChannel;

	mtlistUnitIndex_t	m_CharacterIndexMtlist;
	mtlistUnitIndex_t	m_MonsterIndexMtlist;

	///////////////////////////////////////////////////////////////////////////
	// item
	mtmapDropItem		m_DropItemMtmap;	// ������ �������� pointer�� ����

	inline void InsertDropItem(ITEM_GENERAL* pStoreItem, DROPITEM *pDropItem);
	inline int DeleteDropItem(ITEM_GENERAL* pStoreItem);
	void DeleteTimeoutDropItem(DWORD dwNowTick, vectorDeleteDropItem *pDeleteDropItem = NULL);		// 2012-03-05 by hskim, ��� ������ ���� �ð��� ����

	// 2010-04-09 by cmkwon, ����2�� �߰� ����(�ܰ躰 ���� �߰�) - 
	DROPITEM* GetDROPITEM_SetProcess(ITEM_GENERAL* pStoreItem, BOOL i_bPickupTenderItem=FALSE);

	void AllDeleteDropItem();		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 

	///////////////////////////////////////////////////////////////////////////
	// item
	mtsetDropMine		m_setMapBlockDropMine;	// ������ �������� pointer�� ����

	// map block index
	short				m_x;	// map block x index
	short				m_z;	// map block z index

	//! map block�� center position X
	float				m_CenterPositionX;
	//! map block�� center position Z
	float				m_CenterPositionZ;

	static int			ms_nSIZE_BLOCK;
};

void CMapBlock::InsertDropItem(ITEM_GENERAL* pStoreItem, DROPITEM *pDropItem)
{
	m_DropItemMtmap.insertLock(pStoreItem, *pDropItem);
}

int CMapBlock::DeleteDropItem(ITEM_GENERAL* pStoreItem)
{
	m_DropItemMtmap.lock();
	auto ret = m_DropItemMtmap.erase(pStoreItem);
	m_DropItemMtmap.unlock();
	return ret;
}

////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : CMapBlock::InsertUnit
// ��ȯ�Ǵ� ��  : void
// �� �� �� ��  : ClientIndex_t clientIndex
// �� �� �� ��  : inline �Լ�
//					ĳ���Ϳ� ���͸� �����Ͽ� MapBlock�� �߰��ϴ� �Լ��� ȣ���Ѵ�.
BOOL CMapBlock::InsertUnit(ClientIndex_t clientIndex)
{
	if (clientIndex < MONSTER_CLIENT_INDEX_START_NUM)
		
		return InsertCharacter(clientIndex);
	
	return InsertMonster(clientIndex);
}


////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : CMapBlock::DeleteUnit
// ��ȯ�Ǵ� ��  : void
// �� �� �� ��  : ClientIndex_t clientIndex
// �� �� �� ��  : ĳ���Ϳ� ���͸� �����Ͽ� MapBlock���� �����ϴ� �Լ��� ȣ��
BOOL CMapBlock::DeleteUnit(ClientIndex_t clientIndex)
{
	if (clientIndex < MONSTER_CLIENT_INDEX_START_NUM)
		
		return DeleteCharacter(clientIndex);
	
	return DeleteMonster(clientIndex);
}

// Block�� list���� ���� �Ѱ�
////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : CMapBlock::InsertCharacter
// ��ȯ�Ǵ� ��  : void
// �� �� �� ��  : ClientIndex_t clientIndex ==> MapBlock�� �߰��� ĳ���� �ε���
// �� �� �� ��  : MapBlock�� ĳ������ �ε��� ����Ʈ�� clientIndex�� �߰��Ѵ�.
BOOL CMapBlock::InsertCharacter(ClientIndex_t clientIndex)
{	
	m_CharacterIndexMtlist.lock();
	if(0 == m_CharacterIndexMtlist.capacity())
	{
		// 2007-07-16 by cmkwon, �⺻ ������ 10���� ������
		//m_CharacterIndexMtlist.reserve(ms_nSIZE_BLOCK/8);
		m_CharacterIndexMtlist.reserve(10);
	}

#ifdef _DEBUG
	assert(clientIndex < MONSTER_CLIENT_INDEX_START_NUM);
	mtlistUnitIndex_t::iterator itr = find(m_CharacterIndexMtlist.begin(), m_CharacterIndexMtlist.end(), clientIndex);
	if(itr != m_CharacterIndexMtlist.end())
	{
		char szSystemLog[256];
		sprintf(szSystemLog, "====> InsertCharacter Error Index[%4d]\r\n", clientIndex);
		g_pGlobal->WriteSystemLog(szSystemLog);
		DBGOUT(szSystemLog);
		m_CharacterIndexMtlist.unlock();
		return FALSE;
	}
	m_CharacterIndexMtlist.push_back(clientIndex);
#else
	m_CharacterIndexMtlist.push_back(clientIndex);
#endif
	m_CharacterIndexMtlist.unlock();

	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : CMapBlock::DeleteCharacter
// ��ȯ�Ǵ� ��  : void
// �� �� �� ��  : ClientIndex_t clientIndex
// �� �� �� ��  : MapBlock�� ĳ���� �ε��� ����Ʈ���� clientIndex�� �����Ѵ�.
BOOL CMapBlock::DeleteCharacter(ClientIndex_t clientIndex)
{	
	m_CharacterIndexMtlist.lock();
#ifdef _DEBUG
	assert(clientIndex < MONSTER_CLIENT_INDEX_START_NUM);
	mtlistUnitIndex_t::iterator itr = find(m_CharacterIndexMtlist.begin(), m_CharacterIndexMtlist.end(), clientIndex);
	if(itr == m_CharacterIndexMtlist.end())
	{
		char szSystemLog[256];
		sprintf(szSystemLog, "====> DeleteCharacter Error Index[%4d]\r\n", clientIndex);
		g_pGlobal->WriteSystemLog(szSystemLog);
		DBGOUT(szSystemLog);
		m_CharacterIndexMtlist.unlock();
		return FALSE;
	}
	m_CharacterIndexMtlist.erase(remove(m_CharacterIndexMtlist.begin(), m_CharacterIndexMtlist.end(), clientIndex)
		, m_CharacterIndexMtlist.end());
#else
	m_CharacterIndexMtlist.erase(remove(m_CharacterIndexMtlist.begin(), m_CharacterIndexMtlist.end(), clientIndex)
		, m_CharacterIndexMtlist.end());
#endif
	m_CharacterIndexMtlist.unlock();
	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CMapBlock::ResetCharacter()
/// \brief		�Ʒ��� ���� -
/// \author		dhjin
/// \date		2008-02-20 ~ 2008-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CMapBlock::ResetCharacter()
{
	m_CharacterIndexMtlist.clearLock();
}

////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : CMapBlock::InsertMonster
// ��ȯ�Ǵ� ��  : void
// �� �� �� ��  : ClientIndex_t clientIndex
// �� �� �� ��  : MabBlock�� ���� �ε��� ����Ʈ�� clientIndex�� �߰��Ѵ�.
BOOL CMapBlock::InsertMonster(ClientIndex_t clientIndex)
{
	m_MonsterIndexMtlist.lock();
	if(0 == m_MonsterIndexMtlist.capacity())
	{
		// 2007-07-16 by cmkwon, �⺻ ������ 10���� ������
		//m_MonsterIndexMtlist.reserve(ms_nSIZE_BLOCK/8);
		m_MonsterIndexMtlist.reserve(10);
	}
#ifdef _DEBUG
	assert(clientIndex >= MONSTER_CLIENT_INDEX_START_NUM);
	mtlistUnitIndex_t::iterator itr = find(m_MonsterIndexMtlist.begin(), m_MonsterIndexMtlist.end(), clientIndex);
	if(itr != m_MonsterIndexMtlist.end())
	{
		char szSystemLog[256];
		sprintf(szSystemLog, "====> InsertMonster Error Index[%5d]\r\n", clientIndex);
		g_pGlobal->WriteSystemLog(szSystemLog);
		DBGOUT(szSystemLog);
		m_MonsterIndexMtlist.unlock();
		return FALSE;
	}
	m_MonsterIndexMtlist.push_back(clientIndex);
#else
	m_MonsterIndexMtlist.push_back(clientIndex);
#endif
	m_MonsterIndexMtlist.unlock();

	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : CMapBlock::DeleteMonster
// ��ȯ�Ǵ� ��  : void
// �� �� �� ��  : ClientIndex_t clientIndex
// �� �� �� ��  : MapBlock�� ���� �ε��� ����Ʈ���� clientIndex�� �����Ѵ�.
BOOL CMapBlock::DeleteMonster(ClientIndex_t clientIndex)
{	
#ifdef _DEBUG
	assert(clientIndex >= MONSTER_CLIENT_INDEX_START_NUM);
	m_MonsterIndexMtlist.lock();
	mtlistUnitIndex_t::iterator itr = find(m_MonsterIndexMtlist.begin(), m_MonsterIndexMtlist.end(), clientIndex);
	if(itr == m_MonsterIndexMtlist.end())
	{
		char szSystemLog[256];
		sprintf(szSystemLog, "====> DeleteMonster Error Index[%5d]\r\n", clientIndex);
		g_pGlobal->WriteSystemLog(szSystemLog);
		DBGOUT(szSystemLog);
		m_MonsterIndexMtlist.unlock();
		return FALSE;
	}
	m_MonsterIndexMtlist.erase(remove(m_MonsterIndexMtlist.begin(), m_MonsterIndexMtlist.end(), clientIndex)
		, m_MonsterIndexMtlist.end());
	m_MonsterIndexMtlist.unlock();
#else
	m_MonsterIndexMtlist.lock();
	m_MonsterIndexMtlist.erase(remove(m_MonsterIndexMtlist.begin(), m_MonsterIndexMtlist.end(), clientIndex)
		, m_MonsterIndexMtlist.end());
	m_MonsterIndexMtlist.unlock();
#endif
	
	return TRUE;
}

int CMapBlock::GetNumCharachter()
{
	return m_CharacterIndexMtlist.size();
}

int CMapBlock::GetNumMonster()
{
	return m_MonsterIndexMtlist.size();
}

int CMapBlock::GetNumItem()
{
	return m_DropItemMtmap.size();
}