#ifndef _ATUM_MAP_PROJECT_H_
#define _ATUM_MAP_PROJECT_H_

#include "MapBlock.h"
#include "MapWorkspace.h"
#include "Monster.h"

#ifdef _ATUM_NPC_SERVER
#include "SkinnedMesh.h"
#endif

///////////////////////////////////////////////////////////////////////////////
// *.sma ���� ���� ��ȯ ������ ������ ��ȯ Ÿ��
#define MONSTER_CREATETYPE_BOSS					0
#define MONSTER_CREATETYPE_LARGE_SIZE			1
#define MONSTER_CREATETYPE_MIDDLE_SIZE			2
#define MONSTER_CREATETYPE_SMALL_SIZE			3
#define MONSTER_CREATETYPE_SUMMON				4


/// �� ����Ÿ ///
typedef struct _VERTEXINFO
{
    D3DXVECTOR3 pos;
	// 2010-04-14 by cmkwon, ���� �޸� ���� ���� ���� - �ʿ���� ��� ���� ����
	//D3DXVECTOR3 nor;
} VERTEXINFO;

// 2010-04-14 by cmkwon, ���� �޸� ���� ���� ���� - �ʵ�����(*.dat)���� �ε��� ���� ����ü�� �߰��� ����
struct VERTEXINFO4Loading
{
    D3DXVECTOR3 pos;
	D3DXVECTOR3 nor;
};


enum
{
	WATER_TYPE_NOTING				= 0,
	WATER_TYPE_SERVER_AND_CLIENT	= 1,
	WATER_TYPE_ONLY_SERVER			= 2,
	WATER_TYPE_ONLY_CLIENT			= 3
};

// Tile ����Ÿ

// 2010-04-14 by cmkwon, ���� �޸� ���� ���� ���� - �Ʒ��� ���� ����
// typedef struct _TILEINFO
// {
// 	// 2007-04-26 by cmkwon, ������� �ʴ� ������
// 	//	int			m_nMoveType;	// 0�̸� ���� �ִ� ����, 0�� 1�̸� üũ, 2�̸� ������ ����
// 	//	BYTE		m_bPKType;		// PK ���� ����
// 	BOOL		m_bEnableLand;	// ���� ���� ����
// 	DWORD		m_dwWaterType;	// 0(���� �ƴ�), 1(����/Ŭ���̾�Ʈ ��Ÿ��), 2(Only ���� ��Ÿ��), 3(Only Ŭ���̾�Ʈ ��Ÿ��)
// 	EVENTINFO	m_EventInfo;	// �̺�Ʈ ����
// } TILEINFO;
typedef struct _TILEINFO	// 2010-04-14 by cmkwon, ���� �޸� ���� ���� ���� - 
{
	EVENTINFO	*m_pEventInfo;
	BYTE		m_bEnableLand;	// ���� ���� ����
	BYTE		m_dwWaterType;	// 0(���� �ƴ�), 1(����/Ŭ���̾�Ʈ ��Ÿ��), 2(Only ���� ��Ÿ��), 3(Only Ŭ���̾�Ʈ ��Ÿ��)	
} TILEINFO;

typedef struct _MONSTER_DATA
{
	char		strRegionName[SIZE_MAX_MONSTER_REGION_NAME];
	UINT		sMonType;
	short		sStartx;
	short		sStartz;
	short		sEndx;
	short		sEndz;
	short		sMaxMon;
	short		sResNum;
	USHORT		sResTime;	// �������� ������ ��(minute)�̸� �������� ��(second)��
	BYTE		bMonType;	// 0: ����, 1: ����, 2: ����, 3: ����
} MONSTER_CREATE_REGION_INFO;

typedef struct _MAP_DATA_SERVER
{
	int				m_nVertexNumber;
	int				m_nObjectNumber;
} MapInfoServer;

typedef vector<CMapChannel*>					vectorMapChannelPtr;
typedef vector<MONSTER_CREATE_REGION_INFO>		vectorMONSTER_CREATE_REGION_INFO;
typedef vector<VERTEXINFO>						vectorVERTEXINFO;
typedef vector<D3DXVECTOR3>						vectD3DXVECTOR3;			// 2006-05-19 by cmkwon
float GGetVec3LengthPlane(D3DXVECTOR3 *i_pVec3);

///////////////////////////////////////////////////////////////////////////////
// class CMapProject
///////////////////////////////////////////////////////////////////////////////
class CMapWorkspace;
class CMapChannel;
class CMonster;
class CIOCPSocket;
class CMapProject : public PROJECTINFO
{
	friend class CMapWorkspace;
	friend class CFieldIOCP;
	friend class CFieldIOCPSocket;
	friend class CNPCMapWorkspace;
	friend class CFieldMapWorkspace;
	friend class CPreWinSocket;
	friend class CIMWinSocket;
	friend class CNPCIOCPSocket;

	friend class CMapChannel;
	friend class CNPCMapChannel;
	friend class CFieldMapChannel;

public:
	CMapProject(CMapWorkspace * i_pMapWorkspace, PROJECTINFO * i_pProjectInfo);
	virtual ~CMapProject();

	// �ʱ�ȭ ����
	BOOL LoadMap(BOOL bLoadJustEvent = FALSE);
	BOOL LoadMFile(void);
	BOOL BelongsToServer(void);

	int GetUserVisibleDiameter() const { return m_nUserVisibleDiameter; }
	int GetMonsterVisibleDiameter() const { return m_nMonsterVisibleDiameter; }
	const D3DXVECTOR3& GetCityWarpTargetPosition() const {	return m_CityWarpTargetPosition; }
	D3DXVECTOR3 GetRandomCityWarpTargetPosition(void);
	D3DXVECTOR3 GetRandomXZCityWarpTargetPosition(void);
	int GetMapInfluenceType(void);
	BOOL SetMapInfluenceType2ConflictArea(SHORT i_sMapInflTy, BOOL i_bAdd);	// 2009-01-12 by dhjin, ���� ���� - ���� �߰� ��Ʈ
	int GetOriginalMapInfluenceType(void);
	BOOL IsConflictAreaMap(void);
	int GetWarpObjectIndex(BYTE i_byCharInflTy);
	int GetTeleportWarpObjectIndex();				// 2007-09-15 by dhjin, �ڷ���Ʈ ���� ������Ʈ �ε����� ������


	// Tile ����
	inline TILEINFO* GetTileInfo(float x, float z);
	inline TILEINFO* GetTileInfo(const D3DXVECTOR3 *pPosVector3);
	inline TILEINFO* GetTileInfoByTileIndex(int TileX, int TileZ);
	inline int GetTileInfoIndex(float x, float z);
	BOOL IsValidTileForCreateMonster(D3DXVECTOR3 *pPosVector3);

	// Tile Event ����
	inline EVENTINFO* GetTileEventInfo(float x, float z);
	inline EVENTINFO* GetTileEventInfo(const D3DXVECTOR3 *pPosVector3);
	inline EVENTINFO* GetTileEventInfoByTileIndex(int TileX, int TileZ);
	inline D3DXVECTOR3 GetTileCenterPosition(int index);							// Tile�� Array Index�� ã�´�.
	inline D3DXVECTOR3 GetTileCenterPositionByTileIndex(int xIdx, int zIdx);		// Tile(x, z)�� ã�´�.
	inline D3DXVECTOR3 GetTileCenterPosition(float x, float z);						// x, z ��ǥ�� ã�´�.
	inline D3DXVECTOR3 GetTileRandomPosition(int xIdx, int zIdx);
	EVENTINFO *GetEventInfoByTileEventAreaIndex(int i_evAreaIdx);
	EVENTINFO *GetEventInfoByObjectEventAreaIndex(int i_evAreaIdx);
	EVENTINFO *GetEventInfoByEventAreaIndex(int i_evAreaIdx);

	// �� �ε����� �����´�
	inline void GetBlockAdjacentToPositionHalfDistance(float x, float z, float fDistance, TWO_BLOCK_INDEXES &blockIdx);
	inline void GetBlockAdjacentToPosition(float x, float z, float fRadius, TWO_BLOCK_INDEXES &blockIdx);
	
	inline BLOCK_INDEX GetPostionToBlock(float x, float z);		// 2011-03-24 by hskim, ���Ǵ�Ƽ 3�� - ���� ��Ƽ Ÿ���� ��� �߰�

	BOOL IsValidPosition(D3DXVECTOR3 * i_pVector3Pos);
	BOOL IsValidPosition(float i_fX, float i_fZ);
	BOOL IsValidTileIndex(int i_TileIdx_X, int i_TileIdx_Z);
	BOOL IsValidBlockIndex(int i_BlockIdx_X, int i_BlockIdx_Z);

	float GetDefaultFlyingHeight(void);

	// ���� ��ǥ���� ���� ���̸� ���ϱ����� �Լ�
	float GetMapHeightIncludeWater(const D3DXVECTOR3 *vPos);
	float GetMapHeightExcludeWater(const D3DXVECTOR3 *vPos);

	// map channel ����
	CMapChannel *GetMapChannelByIndex(int i_nIndex, BOOL i_bOnlyEnabledChannel=FALSE);
	CMapChannel *GetRandomMapChannel(BOOL i_bOnlyEnabledChannel);
	CMapChannel *GetFirstMapChannel(BOOL i_bOnlyEnabledChannel);
// 2004-12-07 by cmkwon, ä�� �ǽð� ������ ���� �ʿ���� �Լ�
//	CMapChannel *GetLastMapChannel(BOOL i_bOnlyEnabledChannel);
	INT GetNumberOfMapChannels(BOOL i_bOnlyEnabledChannel=FALSE);

	void SetIOCPSocketMapSession(CIOCPSocket *pSocket) {		m_pIOCPSocketMapSession = pSocket; }
	BOOL Send2Peer(BYTE *pData, int nSize);

	///////////////////////////////////////////////////////////////////////////
	// virtual
	// �ʱ�ȭ ����, 2004-03-26 by cmkwon
	virtual BOOL InitMapProject(void);

	// ä�� ����, added by kelovon, 20040324
	virtual BOOL InitializeMapChannels(int i_nChannel) = 0;
	virtual BOOL CleanMapChannels(void) = 0;
	virtual BOOL IncreaseMapChannels(int i_nChannelToAdd) = 0;

	static BOOL IntersectTriangle( const D3DXVECTOR3& orig, const D3DXVECTOR3& dir, D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2, FLOAT* t, FLOAT* u, FLOAT* v );

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ������Ʈ�� ����!!!!
	void CreateNewObject(ObjectIdx_t i_dwDeleteObjectUID, ObjectNum_t i_dwNewObjectNum, mtNewObjectInfoList * o_pNewObjectInfoList);	// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ

protected:
	// �� ���� ����
	float						m_fSizeMap_X;				// ���� X�� Size�� ������ ����������
	float						m_fSizeMap_Z;				// ���� Z�� Size�� ������ ����������
	int							m_NUM_BLOCK_X;
	int							m_NUM_BLOCK_Z;

	// Object ����
	vectorObjectInfoServer		m_vectorObjectInfo;					// �ʿ� ���� ��� Object ����
	vectorObjectInfoServer		m_vectorObjectMonsterInfo;

	// �ʿ� ������ ���� ���� ����
	vectorMONSTER_CREATE_REGION_INFO	m_vectorMONSTER_CREATE_REGION_INFO;

	// �ʿ� ���� ���� ī��Ʈ ����
	int							m_nMaxMonsterCount;					// Map�� �ִ� ���� ��ü��(����)

	// ��ü ���� ����
	ez_map<int, MONSTER_INFO>		*m_pMapMonsterParameter;

	// Warp ����
	int							m_DefaltWarpTargetIndex;
	map<int, vector<int>*>		m_WarpAreaMap;		// eventIndex -> array of tileIndex
// 2005-07-21 by hblee : m_WarpObjectMap��ſ� m_vectorObjectInfo�� �̿�.
//	map<int, vector<int>*>		m_WarpObjectMap;	// eventIndex -> array of objIndex

	// Tile ����
	vector<TILEINFO>			m_vTileInfo;
	vectorVERTEXINFO			m_vectorVERTEXINFO;

	// ä�� ����, added by kelovon, 20040324
	vectorMapChannelPtr			m_vectorMapChannelPtr;

	CMapWorkspace				*m_pMapWorkspace;
	CIOCPSocket					*m_pIOCPSocketMapSession;

	int							m_nOriginalMapInfluenceType;			// 2006-02-09 by cmkwon, ���� MapInfluenceType - ���� ���� �������� �߰���

#ifdef _ATUM_NPC_SERVER
	mapSkinnedMeshPtr			m_mapObjectSkinnedMeshPtr;
#endif

	vectD3DXVECTOR3				m_vectCityWarpTargetPositionList;		// 2006-05-19 by cmkwon

	///////////////////////////////////////////////////////////////////////////////
	// 2006-07-10 by cmkwon, UDP ������ ���� ���� �ý���
	BYTE			m_pbyDelaySendBuffer[SIZE_MAX_PACKET];
	int				m_nWritedSize;
	mt_lock			m_mtlockDelaySendBuffer;
	DWORD			m_dwLastSendedTick;							// 2006-07-25 by cmkwon

	///////////////////////////////////////////////////////////////////////////////
	// 2007-04-06 by cmkwon
	// 2009-05-29 by cmkwon, Hash�˰��� �߰�(SHA256) - 
	//UINT						m_uiMapCheckSum;
	SRESOBJ_CHECKSUM			m_mapCheckSum;		// 2009-05-29 by cmkwon, Hash�˰��� �߰�(SHA256) - 
};

////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : CMapProject::GetTileInfo
// ��ȯ�Ǵ� ��  : TILEINFO*
// �� �� �� ��  : float x
// �� �� �� ��  : float z
// �� �� �� ��  : ������ ��ǥ�� ���� Ÿ���� �̺�Ʈ ������ �����Ѵ�.
//					�̺�Ʈ ������ Ÿ�ϴ����� ó���Ǿ� ����
TILEINFO* CMapProject::GetTileInfo(float x, float z)
{
	int idx = ((int)x / SIZE_MAP_TILE_SIZE) * m_sYSize + ((int)z / SIZE_MAP_TILE_SIZE);

	if (idx >= m_vTileInfo.size() || idx < 0)
	{
		return NULL;
	}

	return &m_vTileInfo[idx];
}

TILEINFO* CMapProject::GetTileInfo(const D3DXVECTOR3 *pPosVector3)
{
	int idx = ((int)pPosVector3->x / SIZE_MAP_TILE_SIZE) * m_sYSize + ((int)pPosVector3->z / SIZE_MAP_TILE_SIZE);

	if (idx >= m_vTileInfo.size() || idx < 0)
	{
		return NULL;
	}

	return &m_vTileInfo[idx];
}

////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : CMapProject::GetTileInfoByTileIndex
// ��ȯ�Ǵ� ��  : TILEINFO*
// �� �� �� ��  : int TileX
// �� �� �� ��  : int TileZ
// �� �� �� ��  : ������ Ÿ���� �̺�Ʈ ������ �����Ѵ�.
//
TILEINFO* CMapProject::GetTileInfoByTileIndex(int TileX, int TileZ)
{
	int idx = TileX * m_sYSize + TileZ;

	if (idx >= m_vTileInfo.size() || idx < 0)
	{
		return NULL;
	}

	return &m_vTileInfo[idx];
}

EVENTINFO* CMapProject::GetTileEventInfo(float x, float z)
{
	int idx = ((int)x / SIZE_MAP_TILE_SIZE) * m_sYSize + ((int)z / SIZE_MAP_TILE_SIZE);

	if (idx >= m_vTileInfo.size() || idx < 0)
	{
		return NULL;
	}

	return m_vTileInfo[idx].m_pEventInfo;	// 2010-04-14 by cmkwon, ���� �޸� ���� ���� ���� - 
}

EVENTINFO* CMapProject::GetTileEventInfo(const D3DXVECTOR3 *pPosVector3)
{
	int idx = ((int)pPosVector3->x / SIZE_MAP_TILE_SIZE) * m_sYSize + ((int)pPosVector3->z / SIZE_MAP_TILE_SIZE);

	if (idx >= m_vTileInfo.size() || idx < 0)
	{
		return NULL;
	}

	return m_vTileInfo[idx].m_pEventInfo;	// 2010-04-14 by cmkwon, ���� �޸� ���� ���� ���� - 
}

EVENTINFO* CMapProject::GetTileEventInfoByTileIndex(int TileX, int TileZ)
{
	int idx = TileX * m_sYSize + TileZ;

	if (idx >= m_vTileInfo.size() || idx < 0)
	{
		return NULL;
	}

	return m_vTileInfo[idx].m_pEventInfo;	// 2010-04-14 by cmkwon, ���� �޸� ���� ���� ���� - 
}

////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : CMapProject::GetTileInfoIndex
// ��ȯ�Ǵ� ��  : int
// �� �� �� ��  : float x
// �� �� �� ��  : float z
// �� �� �� ��  : ������ ��ǥ�� �ش��ϴ� �̺�Ʈ ������ �ε����� �����Ѵ�.
//
int CMapProject::GetTileInfoIndex(float x, float z)
{
	int idx = ((int)x / SIZE_MAP_TILE_SIZE) * m_sYSize + ((int)z / SIZE_MAP_TILE_SIZE);
	return idx;
}

////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : CMapProject::GetTileCenterPosition
// ��ȯ�Ǵ� ��  : D3DXVECTOR3
// �� �� �� ��  : int index
// �� �� �� ��  : ���ڷ� �־��� �ε����� Ÿ���� �߽� ��ǥ�� �����Ѵ�.
//
D3DXVECTOR3 CMapProject::GetTileCenterPosition(int index)
{
	D3DXVECTOR3 vec;

	vec.x = (float)((index / m_sYSize) * SIZE_MAP_TILE_SIZE + SIZE_MAP_TILE_SIZE/2);
	vec.y = GetDefaultFlyingHeight();
	vec.z = (float)(index % m_sYSize* SIZE_MAP_TILE_SIZE + SIZE_MAP_TILE_SIZE/2);

	return vec;
}

////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : CMapProject::GetTileCenterPositionByTileIndex
// ��ȯ�Ǵ� ��  : D3DXVECTOR3
// �� �� �� ��  : int xIdx
// �� �� �� ��  : int zIdx
// �� �� �� ��  : ���ڷ� �־��� �ε����� Ÿ���� �߽� ��ǥ�� �����Ѵ�.
//
D3DXVECTOR3 CMapProject::GetTileCenterPositionByTileIndex(int xIdx, int zIdx)
{
	D3DXVECTOR3 vec;

	vec.x = (float)(xIdx * SIZE_MAP_TILE_SIZE + SIZE_MAP_TILE_SIZE/2);
	vec.y = GetDefaultFlyingHeight();
	vec.z = (float)(zIdx * SIZE_MAP_TILE_SIZE + SIZE_MAP_TILE_SIZE/2);

	return vec;
}

D3DXVECTOR3 CMapProject::GetTileCenterPosition(float x, float z)
{
	return GetTileCenterPosition(GetTileInfoIndex(x, z));
}

////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : CMapProject::GetTileRandomPosition
// ��ȯ�Ǵ� ��  : D3DXVECTOR3
// �� �� �� ��  : int xIdx
// �� �� �� ��  : int zIdx
// �� �� �� ��  : �־��� �ε��� Ÿ�Ͼ��� ������ ��ǥ�� �����Ѵ�.
//
D3DXVECTOR3 CMapProject::GetTileRandomPosition(int xIdx, int zIdx)
{
	D3DXVECTOR3 vec;

	vec.x = (float)(xIdx * SIZE_MAP_TILE_SIZE + 5 + (xIdx<<5)%(SIZE_MAP_TILE_SIZE - 10));
	vec.y = GetDefaultFlyingHeight();
	vec.z = (float)(zIdx * SIZE_MAP_TILE_SIZE + 5 + (zIdx<<5)%(SIZE_MAP_TILE_SIZE - 10));

	return vec;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CMapProject::GetBlockAdjacentToPositionHalfDistance(float x, float z, float fDistance, TWO_BLOCK_INDEXES &blockIdx)
/// \brief		������ ��ǥ�� �߽����� fDistance/2 �Ÿ����� ������ ���ϴ� Block �ε����� �����Ѵ�.
/// \author		kelovon
/// \date		2004-03-24 ~ 2004-03-24
/// \warning
///
/// \param		x [in]
/// \param		z [in]
/// \param		fDistance [in]
/// \param		blockIdx [out]
/// \return		void
///////////////////////////////////////////////////////////////////////////////
void CMapProject::GetBlockAdjacentToPositionHalfDistance(float x, float z, float fDistance, TWO_BLOCK_INDEXES &blockIdx)
{
	short sVal;
	sVal = (short)((x + fDistance/2) / m_pMapWorkspace->ms_nSIZE_BLOCK_X);
	blockIdx.sMaxX = (sVal >= m_NUM_BLOCK_X)?(m_NUM_BLOCK_X-1):sVal;

	sVal = (short)((z + fDistance/2) / m_pMapWorkspace->ms_nSIZE_BLOCK_Z);
	blockIdx.sMaxZ = (sVal >= m_NUM_BLOCK_Z)?(m_NUM_BLOCK_Z-1):sVal;

	sVal = (short)((x - fDistance/2) / m_pMapWorkspace->ms_nSIZE_BLOCK_X);
	blockIdx.sMinX = (sVal <= 0)?0:sVal;

	sVal = (short)((z - fDistance/2) / m_pMapWorkspace->ms_nSIZE_BLOCK_Z);
	blockIdx.sMinZ = (sVal <= 0)?0:sVal;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CMapChannel::GetBlockAdjacentToPositionHalfDistance(float x, float z, float fDistance, TWO_BLOCK_INDEXES &blockIdx)
/// \brief		������ ��ǥ�� �߽����� fRadius �Ÿ����� ������ ���ϴ� Block �ε����� �����Ѵ�.
/// \author		kelovon
/// \date		2004-03-24 ~ 2004-03-24
/// \warning
///
/// \param		x [in]
/// \param		z [in]
/// \param		fRadius [in]
/// \param		blockIdx [out]
/// \return		void
///////////////////////////////////////////////////////////////////////////////
void CMapProject::GetBlockAdjacentToPosition(float x, float z, float fRadius, TWO_BLOCK_INDEXES &blockIdx)
{
	short sVal;

	sVal = (short)((x + fRadius) / m_pMapWorkspace->ms_nSIZE_BLOCK_X);
	blockIdx.sMaxX = (sVal >= m_NUM_BLOCK_X)?(m_NUM_BLOCK_X-1):sVal;

	sVal = (short)((z + fRadius) / m_pMapWorkspace->ms_nSIZE_BLOCK_Z);
	blockIdx.sMaxZ = (sVal >= m_NUM_BLOCK_Z)?(m_NUM_BLOCK_Z-1):sVal;

	sVal = (short)((x - fRadius) / m_pMapWorkspace->ms_nSIZE_BLOCK_X);
	blockIdx.sMinX = (sVal <= 0)?0:sVal;

	sVal = (short)((z - fRadius) / m_pMapWorkspace->ms_nSIZE_BLOCK_Z);
	blockIdx.sMinZ = (sVal <= 0)?0:sVal;
}

// start 2011-03-24 by hskim, ���Ǵ�Ƽ 3�� - ���� ��Ƽ Ÿ���� ��� �߰�
///////////////////////////////////////////////////////////////////////////////
/// \fn			CMapChannel::GetPostionToBlock(float x, float z)
/// \brief		��ǥ�� �Ҽӵ� ���� ����Ѵ�.
/// \author		hskim
/// \date		2011-03-24
/// \warning
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BLOCK_INDEX CMapProject::GetPostionToBlock(float x, float z)
{
	BLOCK_INDEX Block;

	Block.sX = (short)((x) / m_pMapWorkspace->ms_nSIZE_BLOCK_X);
	Block.sX = (Block.sX >= m_NUM_BLOCK_X)?(m_NUM_BLOCK_X-1) : Block.sX;
	Block.sZ = (short)((z) / m_pMapWorkspace->ms_nSIZE_BLOCK_Z);
	Block.sZ = (Block.sZ >= m_NUM_BLOCK_Z)?(m_NUM_BLOCK_Z-1): Block.sZ;

	return Block;
}
// end 2011-03-24 by hskim, ���Ǵ�Ƽ 3�� - ���� ��Ƽ Ÿ���� ��� �߰�

#endif // _ATUM_MAP_PROJECT_H_