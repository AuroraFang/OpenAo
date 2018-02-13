 #ifndef _ATUM_MAP_CHANNEL_H_
#define _ATUM_MAP_CHANNEL_H_

#include "mt_stl.h"
#include "MapProject.h"
#include "IOCPSocket.h"

#define MAX_MONSTER_COUNT_SPARE			200			// MaxMonsterCounts ���� �̰���ŭ ������ �ΰ� ���� ���͸� �����Ѵ�.

typedef list<ClientIndex_t>		listClientIndex;
typedef mt_set<ClientIndex_t>	mtsetClientIndex;

///////////////////////////////////////////////////////////////////////////////
/// \class		CMapChannel
///
/// \brief		Map�� ���� ����Ÿ ����
/// \author		kelovon
/// \version
/// \date		2004-03-24 ~ 2004-03-24
/// \warning
///////////////////////////////////////////////////////////////////////////////
class CMapChannel
{
	friend class CMapBlock;
	friend class CFieldIOCPSocket;
	friend class CNPCIOCPSocket;
	friend class CNPCMapProject;
public:
	CMapChannel(CMapWorkspace *i_pWorkspace, CMapProject *i_pProject, ChannelIndex_t i_nChannelIndex);
	virtual ~CMapChannel();

	// �ʱ�ȭ �Լ�
	virtual BOOL InitMapChannel(void);
	void ResetMapChannel(void);
	void ResetUserMapChannel(void);				// 2008-02-20 by dhjin, �Ʒ��� ���� - �� ä�ο� ���� ���� �ʱ�ȭ
	void ResetDropItemMapChannel(void);				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �� ä�ο� ��� ������ ���� �ʱ�ȭ

	MAP_CHANNEL_INDEX GetMapChannelIndex(void) { return m_MapChannelIndex; }
	int GetUserVisibleDiameterW(void);
	int GetMonsterVisibleDiameterW(void);
	EVENTINFO *GetEventInfoByEventAreaIndexW(int i_evAreaIdx);
	D3DXVECTOR3 GetCityWarpTargetPositionW(void);
	int GetMapInfluenceTypeW(void);

	// ä�� Ȱ��ȭ/��Ȱ��ȭ ����
	BOOL IsEnabled();
	BOOL SetChannelState(BOOL i_bEnableChannel);

	// Map Block ���� �Լ�
	inline CMapBlock* GetBlock(float x, float z);
	inline CMapBlock* GetBlockByIndex(short xIdx, short zIdx);
	float GetSizeMapXW(void);
	float GetSizeMapZW(void);
	float GetMapHeightIncludeWaterW(D3DXVECTOR3 *i_pVec3Pos);

	// Map Block ��ġ ���� �Լ�
	BOOL SetInitialPosition(float x, float z, ClientIndex_t clientIndex);
	BOOL UpdateBlockPosition(float oldX, float oldZ, float newX, float newZ, ClientIndex_t clientIndex);
	BOOL UpdateBlockPosition(D3DXVECTOR3 oldPosition, D3DXVECTOR3 newPosition, ClientIndex_t clientIndex);
	BOOL DeleteBlockPosition(float x, float z, ClientIndex_t clientIndex);

	// Ư�� �Ÿ� ���� Map Block Index�� �����´�
	inline void GetAdjacentBlocksInCircle(D3DXVECTOR3 &i_refPosition, float i_fDistance, vector<CMapBlock*> *o_pVectorMapBlock);

	// Ư�� �Ÿ� ���� Character�� Monster �˻� ���� �Լ�
	int GetAdjacentCharacterIndexes(D3DXVECTOR3 *i_pPosition, float fDistance, vector<ClientIndex_t> *pClientIndexVector, ClientIndex_t nClientIndexToExclude = INVALID_CLIENT_INDEX);
	int GetAdjacentCharacterIndexes(float x, float z, float fDistance, vector<ClientIndex_t> *pClientIndexVector, ClientIndex_t nClientIndexToExclude = INVALID_CLIENT_INDEX);
	int GetAdjacentCharacterIndexes(float xStart, float zStart, float xEnd, float zEnd, vector<ClientIndex_t> *pClientIndexVector, ClientIndex_t nClientIndexToExclude = INVALID_CLIENT_INDEX);
	int GetAdjacentMonsterIndexes(D3DXVECTOR3 *i_pPosition, float fDistance, vector<ClientIndex_t> *pClientIndexVector);
	int GetAdjacentMonsterIndexes(float x, float z, float fDistance, vector<ClientIndex_t> *pClientIndexVector);
	int GetAdjacentMonsterIndexes(float xStart, float zStart, float xEnd, float zEnd, vector<ClientIndex_t> *pClientIndexVector);
	
	// �� ���� ���� �Լ�
	inline int GetNumClients();
	inline int GetNumMonsters();
	inline int UpdateMaxUserCounts(int nCurUserCounts);

	// ���� ����
	inline CMonster* GetMonster(ClientIndex_t i_Monsteridx);
	inline CMonster* GetMonsterByMonsterUnitKind(INT i_MonsterUnitKind);

	BOOL SaveUnitCountsPerBlock(void);

	BOOL GetAllClientIndexList(vectClientIndex_t *o_pvectCliIdxList);
	INT GetWarpObjectIndexW(BYTE i_byCharInflTy);		// 2006-12-08 by cmkwon
	INT GetTeleportWarpObjectIndexW();					// 2007-09-15 by dhjin

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-14 by cmkwon, ���� �޸� ���� ���� ���� - 
	TILEINFO* GetTileInfoW(float x, float z);
	TILEINFO* GetTileInfoW(const D3DXVECTOR3 *pPosVector3);
	TILEINFO* GetTileInfoByTileIndexW(int TileX, int TileZ);
	EVENTINFO* GetTileEventInfoW(float x, float z);
	EVENTINFO* GetTileEventInfoW(const D3DXVECTOR3 *pPosVector3);
	EVENTINFO* GetTileEventInfoByTileIndexW(int TileX, int TileZ);


#ifdef _DEBUG
	void printUnitCountsPerBlock(void);
#endif // _DEBUG_endif
protected:
	CMapBlock				**m_arrMapBlock;					// ���� ����Ÿ�� ����Ǵ� MapBlocks
	mtlistUnitIndex_t		m_mtClientIndexList;				// ���� �ʿ� ���� character���� client indexes

	int						m_nSizemtvectorMonsterPtr;			// CMonster Pointer�� �����Ҽ� �ִ� mt_vector Size
	mtvectorMonsterPtr		m_mtvectorMonsterPtr;				// CMonster Pointer�� �����Ҽ� �ִ� mt_vector
	listClientIndex			m_vectorUsableMonsterIndex;			// MonsterIndex List

	int						m_uiLimitMonsterCountsInChannel;
	int						m_nMaxMonsterCountInChannel;		// ä�ο� �ڵ� ���� ������ �ִ� ���� ��ü��(����)
	int						m_nCurMonsterCountInChannel;		// ä�ο� �����Ǿ��ִ� ���� ������ ��
	int						m_nTotalMonsterCountInChannel;		// ä�ο� ������ ��� ������ ��

	UINT					m_uiAccumulatedUserCountsInChannel;	// ä�ο� ������ ���� ĳ���� ��
	UINT					m_uiMaxUserCountsInChannel;			// ä�ο� ������ �ִ� �����ڼ�
	UINT					m_uiAccumulatedMonsterCountsInChannel;	// ä�ο� ������ ���� ���ͼ�

	CMapWorkspace			*m_pMapWorkspace;
	CMapProject				*m_pMapProject;

	MAP_CHANNEL_INDEX		m_MapChannelIndex;					// �ڽ��� MAP_CHANNEL_INDEX

	BOOL					m_bIsEnabled;						// ���� ä���� Ȱ��ȭ�Ǿ��ִ��� ����

public:
	// CityWar ����
	BOOL					m_bCityWarStarted;
};

///////////////////////////////////////////////////////////////////////////////
/// \fn			CMapBlock* CMapChannel::GetBlock(float x, float z)
/// \brief		������ ��ǥ�� ���ϴ� CMapBlock�� �����͸� �����Ѵ�.
/// \author		kelovon
/// \date		2004-03-24 ~ 2004-03-24
/// \warning
///
/// \param		x [in]
/// \param		z [in]
/// \return		CMapBlock*
///////////////////////////////////////////////////////////////////////////////
CMapBlock* CMapChannel::GetBlock(float x, float z)
{
	return GetBlockByIndex((short)(x / m_pMapWorkspace->ms_nSIZE_BLOCK_X),
							(short)(z / m_pMapWorkspace->ms_nSIZE_BLOCK_Z));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CMapBlock* CMapChannel::GetBlockByIndex(short xIdx, short zIdx)
/// \brief		������ block index�� �ش��ϴ� CMapBlock�� �����͸� �����Ѵ�.
/// \author		kelovon
/// \date		2004-03-24 ~ 2004-03-24
/// \warning
///
/// \param		xIdx [in] MapBlock�� x index
/// \param		zIdx [in] MapBlock�� z index
/// \return		CMapBlock*
///////////////////////////////////////////////////////////////////////////////
CMapBlock* CMapChannel::GetBlockByIndex(short xIdx, short zIdx)
{
	if (FALSE == m_pMapProject->IsValidBlockIndex(xIdx, zIdx))
	{
		return NULL;
	}

	return &m_arrMapBlock[xIdx][zIdx];
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CMapProject::GetAdjacentBlocksInCircle(D3DXVECTOR3 &i_refPosition,
///									float i_fDistance,
///									vector<CMapBlock> *o_pVectorMapBlock);
/// \brief		�־��� ��ǥ(x, z)�� �߽����� ���� �Ÿ�(i_fDistance) ���� �ִ�
///				block index�� vector�� ��ȯ��
/// \author		kelovon
/// \date		2004-03-15 ~ 2004-03-15
/// \warning	���̸� ������� �Ѱ� 2���� block�� ���ؼ��� ó����
///
/// \param		i_refPosition [in] ���� ��ǥ, x, z�� �����
/// \param		i_fDistance [in] ���� �Ÿ�
/// \param		o_pVectorMapBlock [out] ������ ���ϴ� block�� vector, TLSData�� ����ؾ� ��!
/// \return		����
///////////////////////////////////////////////////////////////////////////////
void CMapChannel::GetAdjacentBlocksInCircle(D3DXVECTOR3 &i_refPosition,
									float i_fDistance,
									vector<CMapBlock*> *o_pVectorMapBlock)
{
	TWO_BLOCK_INDEXES blockIdx;
	int i, j;

	int x = i_refPosition.x;
	int z = i_refPosition.z;

	m_pMapProject->GetBlockAdjacentToPosition(i_refPosition.x, i_refPosition.z, i_fDistance, blockIdx);

	i = blockIdx.sMinX;
	while(i <= blockIdx.sMaxX)
	{
		j = blockIdx.sMinZ;
		while(j <= blockIdx.sMaxZ)
		{
			CMapBlock *pBlock = GetBlockByIndex(i,j);
			// MapBlock �Ÿ� Ȯ���ϱ�
			if ( GGetLength(pBlock->m_CenterPositionX-i_refPosition.x,
					pBlock->m_CenterPositionZ-i_refPosition.z) < i_fDistance)
			{
				o_pVectorMapBlock->push_back(pBlock);
			}
			j++;
		}
		i++;
	}

	return;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int CMapChannel::GetNumClients()
/// \brief		���� �� ä�ο� �����ϴ� ĳ������ ������ ��ȯ
/// \author		kelovon
/// \date		2004-03-24 ~ 2004-03-24
/// \warning
///
/// \return		int
///////////////////////////////////////////////////////////////////////////////
int CMapChannel::GetNumClients()
{
	return m_mtClientIndexList.size();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int CMapChannel::GetNumMonsters()
/// \brief
/// \author		kelovon
/// \date		2004-03-24 ~ 2004-03-24
/// \warning
///
/// \param
/// \return		int
///////////////////////////////////////////////////////////////////////////////
int CMapChannel::GetNumMonsters()
{
	int nMonsters = 0;

	int i = 0;
	while(i < m_pMapProject->m_NUM_BLOCK_X)
	{
		int j = 0;
		while(j < m_pMapProject->m_NUM_BLOCK_Z)
		{
			nMonsters += (m_arrMapBlock[i][j]).GetNumMonster();
			j++;
		}
		i++;
	}

	return nMonsters;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int CMapChannel::UpdateMaxUserCounts(int nCurUserCounts)
/// \brief
/// \author		kelovon
/// \date		2004-03-24 ~ 2004-03-24
/// \warning
///
/// \param		nCurUserCounts [in]
/// \return		int
///////////////////////////////////////////////////////////////////////////////
int CMapChannel::UpdateMaxUserCounts(int nCurUserCounts)
{
	if(m_uiMaxUserCountsInChannel < nCurUserCounts)
	{
		m_uiMaxUserCountsInChannel = nCurUserCounts;
	}
	return m_uiMaxUserCountsInChannel;
}

////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : CMapProject::GetMonster
// ��ȯ�Ǵ� ��  : CMonster*
// �� �� �� ��  : ClientIndex_t idx
// �� �� �� ��  : idx���� MONSTER_CLIENT_INDEX_START_NUM�� �� �ε����� �ش��ϴ�
//					CMonster�� �����͸� �����ϴ� �Լ�
CMonster* CMapChannel::GetMonster(ClientIndex_t i_Monsteridx)
{
	int mIdx = i_Monsteridx - MONSTER_CLIENT_INDEX_START_NUM;
	
	if(mIdx < 0 || mIdx >= m_nSizemtvectorMonsterPtr) nullptr;

	return m_mtvectorMonsterPtr[mIdx];
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CMonster* CMapChannel::GetMonsterByMonsterUnitKind(INT i_MonsterUnitKind)
/// \brief		
/// \author		cmkwon
/// \date		2006-03-02 ~ 2006-03-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CMonster* CMapChannel::GetMonsterByMonsterUnitKind(INT i_MonsterUnitKind)
{
	for(int i=0; i < m_mtvectorMonsterPtr.size(); i++)
	{
		CMonster *pMon = m_mtvectorMonsterPtr[i];
		if(MS_PLAYING == pMon->m_enMonsterState
			&& pMon->MonsterInfoPtr
			&& i_MonsterUnitKind == pMon->MonsterInfoPtr->MonsterUnitKind)
		{
			return pMon;
		}
	}
	
	return NULL;
}
#endif //_ATUM_MAP_CHANNEL_H_
