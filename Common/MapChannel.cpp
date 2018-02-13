#include "StdAfx.h"
#include "MapChannel.h"
#include <TIME.H>

///////////////////////////////////////////////////////////////////////////////
/// \fn			CMapChannel::CMapChannel()
/// \brief		constructor
/// \author		kelovon
/// \date		2004-03-24 ~ 2004-03-24
/// \warning
///////////////////////////////////////////////////////////////////////////////
CMapChannel::CMapChannel(CMapWorkspace *i_pWorkspace,
						 CMapProject *i_pProject,
						 ChannelIndex_t i_nChannelIndex)
{
	m_pMapWorkspace					= i_pWorkspace;
	m_pMapProject					= i_pProject;
	m_MapChannelIndex.ChannelIndex	= i_nChannelIndex;
	m_MapChannelIndex.MapIndex		= m_pMapProject->m_nMapIndex;
	m_nMaxMonsterCountInChannel		= i_pProject->m_nMaxMonsterCount;
	m_nSizemtvectorMonsterPtr		= m_nMaxMonsterCountInChannel + MAX_MONSTER_COUNT_SPARE;

	m_arrMapBlock					= NULL;
	m_mtClientIndexList.reserve(200);

	m_mtvectorMonsterPtr.lock();
	m_mtvectorMonsterPtr.clear();
	m_vectorUsableMonsterIndex.clear();
	m_mtvectorMonsterPtr.unlock();

	m_uiLimitMonsterCountsInChannel			= m_nMaxMonsterCountInChannel;
	m_nCurMonsterCountInChannel				= 0;
	m_nTotalMonsterCountInChannel			= 0;

	m_uiAccumulatedUserCountsInChannel		= 0;
	m_uiMaxUserCountsInChannel				= 0;
	m_uiAccumulatedMonsterCountsInChannel	= 0;

	m_bCityWarStarted						= FALSE;

	m_bIsEnabled							= TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CMapChannel::~CMapChannel()
/// \brief		destructor
/// \author		kelovon
/// \date		2004-03-24 ~ 2004-03-24
///////////////////////////////////////////////////////////////////////////////
CMapChannel::~CMapChannel()
{
	if (m_arrMapBlock != NULL)
	{
		util::del_array(m_arrMapBlock[0]);
		util::del_array(m_arrMapBlock);
	}
	m_mtClientIndexList.clear();

	m_mtvectorMonsterPtr.lock();
	m_mtvectorMonsterPtr.clear();
	m_vectorUsableMonsterIndex.clear();
	m_mtvectorMonsterPtr.unlock();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CMapChannel::InitMapChannel(void)
/// \brief		����Ÿ ������ ���� MapBlock�� array�� ����
/// \author		kelovon
/// \date		2004-03-24 ~ 2004-03-24
/// \warning	CMapChannel�� Init() ���Ŀ� ȣ��Ǿ�� �Ѵ�
///
/// \return		BOOL
///////////////////////////////////////////////////////////////////////////////
BOOL CMapChannel::InitMapChannel(void)
{
	m_arrMapBlock = 0;

	CMapBlock::ms_nSIZE_BLOCK = m_pMapWorkspace->ms_nSIZE_BLOCK_X;
	m_arrMapBlock = new CMapBlock*[m_pMapProject->m_NUM_BLOCK_X];
	CMapBlock *arr = new CMapBlock[m_pMapProject->m_NUM_BLOCK_X * m_pMapProject->m_NUM_BLOCK_Z];
	for (int i = 0; i < m_pMapProject->m_NUM_BLOCK_X; i++)
	{
		m_arrMapBlock[i] = arr + m_pMapProject->m_NUM_BLOCK_Z * i;
	}

	int x = 0;
	while(x < m_pMapProject->m_NUM_BLOCK_X)
	{
		int z = 0;
		while(z < m_pMapProject->m_NUM_BLOCK_Z)
		{
			CMapBlock *pBlock = &m_arrMapBlock[x][z];
			pBlock->m_pMapChannel = this;
			pBlock->m_x = x;
			pBlock->m_z = z;
			pBlock->m_CenterPositionX = (pBlock->m_x+0.5f)*m_pMapWorkspace->ms_nSIZE_BLOCK_X;
			if (pBlock->m_CenterPositionX > m_pMapProject->m_sXSize*SIZE_MAP_TILE_SIZE)
			{
				pBlock->m_CenterPositionX
					= (pBlock->m_x)*m_pMapWorkspace->ms_nSIZE_BLOCK_X
					+ (m_pMapProject->m_sXSize*SIZE_MAP_TILE_SIZE - (pBlock->m_x)*m_pMapWorkspace->ms_nSIZE_BLOCK_X)/2;
			}
			pBlock->m_CenterPositionZ = (pBlock->m_z+0.5f)*m_pMapWorkspace->ms_nSIZE_BLOCK_X;
			if (pBlock->m_CenterPositionZ > m_pMapProject->m_sYSize*SIZE_MAP_TILE_SIZE)
			{
				pBlock->m_CenterPositionZ
					= (pBlock->m_z)*m_pMapWorkspace->ms_nSIZE_BLOCK_X
					+ (m_pMapProject->m_sYSize*SIZE_MAP_TILE_SIZE - (pBlock->m_z)*m_pMapWorkspace->ms_nSIZE_BLOCK_X)/2;
			}
			z++;
		}
		x++;
	}

	return TRUE;
}

void CMapChannel::ResetMapChannel(void)
{
	m_mtvectorMonsterPtr.lock();
	if(m_mtvectorMonsterPtr.empty() == false)
	{
		m_vectorUsableMonsterIndex.clear();
		for (int i = 0; i < m_nSizemtvectorMonsterPtr; i++)
		{
			m_mtvectorMonsterPtr[i]->ResetMonster();
			m_vectorUsableMonsterIndex.push_back(i + MONSTER_CLIENT_INDEX_START_NUM);
		}
	}
	m_mtvectorMonsterPtr.unlock();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CMapChannel::ResetUserMapChannel(void)
/// \brief		�Ʒ��� ���� - �� ä�ο� ���� ���� �ʱ�ȭ
/// \author		dhjin
/// \date		2008-02-20 ~ 2008-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CMapChannel::ResetUserMapChannel(void)
{
	if (NULL == m_arrMapBlock || NULL == m_pMapProject)
	{
		return;
	}
	/*
	CMapBlock::ms_nSIZE_BLOCK = m_pMapWorkspace->ms_nSIZE_BLOCK_X;
	m_arrMapBlock = new CMapBlock*[m_pMapProject->m_NUM_BLOCK_X];
	CMapBlock *arr = new CMapBlock[m_pMapProject->m_NUM_BLOCK_X * m_pMapProject->m_NUM_BLOCK_Z];
	*/
	m_mtClientIndexList.clearLock();
	
	int x = 0;
	while(x < m_pMapProject->m_NUM_BLOCK_X)
	{
		int z = 0;
		while(z < m_pMapProject->m_NUM_BLOCK_Z)
		{
			CMapBlock *pBlock = &m_arrMapBlock[x][z];
			pBlock->ResetCharacter();
			z++;
		}
		x++;
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CMapChannel::ResetDropItemMapChannel(void)
/// \brief		���Ǵ�Ƽ - �� ä�ο� ��� ������ ���� �ʱ�ȭ 
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CMapChannel::ResetDropItemMapChannel(void)
{
	if (NULL == m_arrMapBlock || NULL == m_pMapProject)
	{
		return;
	}

	int x = 0;
	while(x < m_pMapProject->m_NUM_BLOCK_X)
	{
		int z = 0;
		while(z < m_pMapProject->m_NUM_BLOCK_Z)
		{
			CMapBlock *pBlock = &m_arrMapBlock[x][z];
			pBlock->AllDeleteDropItem();
			z++;
		}
		x++;
	}
}

int CMapChannel::GetUserVisibleDiameterW(void)
{
	return m_pMapProject->GetUserVisibleDiameter();
}
int CMapChannel::GetMonsterVisibleDiameterW(void)
{
	return m_pMapProject->GetMonsterVisibleDiameter();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		cmkwon
/// \date		2005-07-22 ~ 2005-07-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
EVENTINFO *CMapChannel::GetEventInfoByEventAreaIndexW(int i_evAreaIdx)
{
	return m_pMapProject->GetEventInfoByEventAreaIndex(i_evAreaIdx);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			D3DXVECTOR3 CMapChannel::GetCityWarpTargetPositionW(void)
/// \brief		
/// \author		cmkwon
/// \date		2005-07-27 ~ 2005-07-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
D3DXVECTOR3 CMapChannel::GetCityWarpTargetPositionW(void)
{
	return m_pMapProject->GetCityWarpTargetPosition();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int CMapChannel::GetMapInfluenceTypeW(void)
/// \brief		
/// \author		cmkwon
/// \date		2005-12-28 ~ 2005-12-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CMapChannel::GetMapInfluenceTypeW(void)
{
	return m_pMapProject->GetMapInfluenceType();
}



BOOL CMapChannel::IsEnabled()
{
	return m_bIsEnabled;
}

BOOL CMapChannel::SetChannelState(BOOL i_bEnableChannel)
{
	if(0 == m_MapChannelIndex.ChannelIndex
		&& FALSE == i_bEnableChannel)
	{// 2006-06-02 by cmkwon, 0�� ä���� ��Ȱ��ȭ ��ų�� ����.
		return FALSE;
	}

	m_bIsEnabled = i_bEnableChannel;
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			float CMapChannel::GetSizeMapXW(void)
/// \brief		
/// \author		cmkwon
/// \date		2005-11-03 ~ 2005-11-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CMapChannel::GetSizeMapXW(void)
{
	return m_pMapProject->m_fSizeMap_X;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			float CMapChannel::GetSizeMapZW(void)
/// \brief		
/// \author		cmkwon
/// \date		2005-11-03 ~ 2005-11-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CMapChannel::GetSizeMapZW(void)
{
	return m_pMapProject->m_fSizeMap_Z;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			float CMapChannel::GetMapHeightIncludeWaterW(D3DXVECTOR3 *i_pVec3Pos)
/// \brief		
/// \author		cmkwon
/// \date		2005-11-03 ~ 2005-11-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CMapChannel::GetMapHeightIncludeWaterW(D3DXVECTOR3 *i_pVec3Pos)
{
	return m_pMapProject->GetMapHeightIncludeWater(i_pVec3Pos);
}


////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : CMapChannel::SetInitialPosition
// ��ȯ�Ǵ� ��  : void
// �� �� �� ��  : float x
// �� �� �� ��  : float z
// �� �� �� ��  : ClientIndex_t clientIndex
// �� �� �� ��  : ��ü�� �ʱ� ��ġ�� �����ϴ� �Լ�
//					��ǥ�� �ش��ϴ� MapBlock�� �ε��� ����Ʈ�� ��ü�� �ε���(clientIndex)�� �߰��Ѵ�.
BOOL CMapChannel::SetInitialPosition(float x, float z, ClientIndex_t clientIndex)
{
	if (FALSE == m_pMapProject->IsValidPosition(x, z)
		|| clientIndex < CLIENT_INDEX_START_NUM)
	{
		return FALSE;
	}

	BLOCK_INDEX	idx		= CMapWorkspace::SelectBlockIndexWithPosition(x, z);
	BOOL		bRet	= m_arrMapBlock[idx.sX][idx.sZ].InsertUnit(clientIndex);

	// ���� �ʿ� ���� client���� �����ϱ� ���� �߰���
	if (clientIndex < MONSTER_CLIENT_INDEX_START_NUM)
	{
		m_mtClientIndexList.pushBackLock(clientIndex);
		m_uiAccumulatedUserCountsInChannel++;
	}
	else
	{
		m_uiAccumulatedMonsterCountsInChannel++;
	}

	if(FALSE == bRet)
	{
		char szSystemLog[512];
		sprintf(szSystemLog, "[Error] SetInitialPosition_1 Error, MapChannel(%s) UnitIndex(%5d) BlockXZ(%3d, %3d) PosXZ(%5.0f, %5.0f)\r\n"
			, GET_MAP_STRING(this->m_MapChannelIndex), clientIndex
			, idx.sX, idx.sZ, x, z);
		g_pGlobal->WriteSystemLog(szSystemLog);
		DBGOUT(szSystemLog);
	}

	return bRet;
}


////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : CMapChannel::DeleteBlockPosition
// ��ȯ�Ǵ� ��  : void
// �� �� �� ��  : float x
// �� �� �� ��  : float z
// �� �� �� ��  : ClientIndex_t clientIndex
// �� �� �� ��  : ���Ͱ� �����ϰų� ĳ���Ͱ� ������ �����Ҷ� �ش� ���ڷ� ���� ��ǥ�� �ش��ϴ� MapBlock��
//					�ε��� ����Ʈ���� ��ü�� clientIndex�� �����ϴ� �Լ�
BOOL CMapChannel::DeleteBlockPosition(float x, float z, ClientIndex_t clientIndex)
{
	if (FALSE == m_pMapProject->IsValidPosition(x, z)
		|| clientIndex < CLIENT_INDEX_START_NUM)
	{
		return FALSE;
	}

	BLOCK_INDEX	idx		= CMapWorkspace::SelectBlockIndexWithPosition(x, z);
	BOOL		bRet	= m_arrMapBlock[idx.sX][idx.sZ].DeleteUnit(clientIndex);
	
	// ���� �ʿ� ���� client���� �����ϱ� ���� �߰���
	if (clientIndex < MONSTER_CLIENT_INDEX_START_NUM)
	{
		m_mtClientIndexList.lock();
		m_mtClientIndexList.erase(remove(m_mtClientIndexList.begin(), m_mtClientIndexList.end(), clientIndex)
			, m_mtClientIndexList.end());
		m_mtClientIndexList.unlock();
	}

	if(FALSE == bRet)
	{
		char szSystemLog[512];
		sprintf(szSystemLog, "[Error] DeleteBlockPosition_1 Error, MapChannel(%s) UnitIndex(%5d) BlockXZ(%3d, %3d) PosXZ(%5.0f, %5.0f)\r\n"
			, GET_MAP_STRING(this->m_MapChannelIndex), clientIndex
			, idx.sX, idx.sZ, x, z);
		g_pGlobal->WriteSystemLog(szSystemLog);
		DBGOUT(szSystemLog);
	}

	return bRet;
}

BOOL CMapChannel::UpdateBlockPosition(D3DXVECTOR3 oldPosition, D3DXVECTOR3 newPosition, ClientIndex_t clientIndex)
{
	return UpdateBlockPosition(oldPosition.x, oldPosition.z, newPosition.x, newPosition.z, clientIndex);
}

////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : CMapChannel::UpdateBlockPosition
// ��ȯ�Ǵ� ��  : BOOL
// �� �� �� ��  : float oldX
// �� �� �� ��  : float oldZ
// �� �� �� ��  : float newX
// �� �� �� ��  : float newZ
// �� �� �� ��  : ClientIndex_t clientIndex
// �� �� �� ��  : ĳ���� Ȥ�� ������ Position�� ����Ǹ� ȣ��Ǵ� �Լ�, MapBlock�� ����� ��쿡��
//					�� MapBlock���� ��ü�� clientIndex�� �����ϰ� ���� MapBlock�� clientIndex�� �߰��Ѵ� �Լ�
BOOL CMapChannel::UpdateBlockPosition(float oldX, float oldZ, float newX, float newZ, ClientIndex_t clientIndex)
{
	if (FALSE == m_pMapProject->IsValidPosition(oldX, oldZ)
		|| FALSE == m_pMapProject->IsValidPosition(newX, newZ)
		|| clientIndex < CLIENT_INDEX_START_NUM)
	{
		return FALSE;
	}

	BLOCK_INDEX oldIdx = CMapWorkspace::SelectBlockIndexWithPosition(oldX, oldZ);
	BLOCK_INDEX newIdx = CMapWorkspace::SelectBlockIndexWithPosition(newX, newZ);
	if (oldIdx.sX != newIdx.sX || oldIdx.sZ != newIdx.sZ)
	{
		if(FALSE == m_arrMapBlock[oldIdx.sX][oldIdx.sZ].DeleteUnit(clientIndex))
		{
			char szTemp[256];
			sprintf(szTemp, "[Error] DeleteUnit_1 Error, MapChannel(%s) UnitIndex(%5d) BlockXZ(%3d, %3d) PosXZ(%5.0f, %5.0f)\r\n"
				, GET_MAP_STRING(this->m_MapChannelIndex), clientIndex
				, oldIdx.sX, oldIdx.sZ, oldX, oldZ);
			DBGOUT(szTemp);
			g_pGlobal->WriteSystemLog(szTemp);
		}
		if(FALSE == m_arrMapBlock[newIdx.sX][newIdx.sZ].InsertUnit(clientIndex))
		{
			char szTemp[256];
			sprintf(szTemp, "[Error] InsertUnit_1 Error, MapChannel(%s) UnitIndex(%5d) BlockXZ(%3d, %3d) PosXZ(%5.0f, %5.0f)\r\n"
				, GET_MAP_STRING(this->m_MapChannelIndex), clientIndex
				, newIdx.sX, newIdx.sZ, newX, newZ);
			DBGOUT(szTemp);
			g_pGlobal->WriteSystemLog(szTemp);
		}

		return TRUE;
	}

	return FALSE;
}

int CMapChannel::GetAdjacentCharacterIndexes(D3DXVECTOR3 *i_pPosition, float fDistance, vector<ClientIndex_t> *pClientIndexVector, ClientIndex_t nClientIndexToExclude)
{
	return GetAdjacentCharacterIndexes(i_pPosition->x, i_pPosition->z, fDistance, pClientIndexVector, nClientIndexToExclude);
}

int CMapChannel::GetAdjacentMonsterIndexes(D3DXVECTOR3 *i_pPosition, float fDistance, vector<ClientIndex_t> *pClientIndexVector)
{
	return GetAdjacentMonsterIndexes(i_pPosition->x, i_pPosition->z, fDistance, pClientIndexVector);
}

////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : CMapChannel::GetAdjacentCharacterIndexes
// ��ȯ�Ǵ� ��  : int ==> �������� ĳ���� �ε��� ī��Ʈ
// �� �� �� ��  : float x
// �� �� �� ��  : float z
// �� �� �� ��  : float fDistance ==> ��ǥ�� ������ �Ÿ��� 2�� ����� ����
// �� �� �� ��  : vector<ClientIndex_t> *pClientIndexVector
// �� �� �� ��  : ������ ��ǥ�� �߽����� fDistance/2 �Ÿ����� Block �����ȿ��ִ� ĳ���� �ε����� ���Ѵ�
//
int CMapChannel::GetAdjacentCharacterIndexes(float x, float z, float fDistance, vector<ClientIndex_t> *pClientIndexVector, ClientIndex_t nClientIndexToExclude)
{
	TWO_BLOCK_INDEXES blockIdx;
	int i, j;

//#ifdef _DEBUG
//	int tmpCap = pClientIndexVector->capacity();
//	pClientIndexVector->clear();
//	int tmpCap2 = pClientIndexVector->capacity();
//	assert( tmpCap == tmpCap2 );
//#endif

	m_pMapProject->GetBlockAdjacentToPositionHalfDistance(x, z, fDistance, blockIdx);			//
	pClientIndexVector->clear();
	i = blockIdx.sMinX;
	while(i <= blockIdx.sMaxX)
	{
		j = blockIdx.sMinZ;
		while(j <= blockIdx.sMaxZ)
		{
			CMapBlock *pMapBlock = GetBlockByIndex(i, j);
			if (pMapBlock == NULL)
			{
				j++; continue;
			}

// 2005-12-13 by cmkwon, �� ũ�Ⱑ Ŀ���� �߽��� ������ ����ϸ� ������ ����. �׷��� ������
////#ifdef _DEBUG
//			int llen = GGetLength(pMapBlock->m_CenterPositionX - x,
//					pMapBlock->m_CenterPositionZ - z);
//#endif
//			// MapBlock �Ÿ� Ȯ���ϱ�
//			if ( GGetLength(pMapBlock->m_CenterPositionX - x,
//					pMapBlock->m_CenterPositionZ - z) > fDistance/2 )
//			{
//				j++;
//				continue;
//			}

			// character�� ������ �Ѿ
			if (!pMapBlock->m_CharacterIndexMtlist.empty())
			{
				pMapBlock->m_CharacterIndexMtlist.lock();
				pClientIndexVector->insert(pClientIndexVector->end()
					, pMapBlock->m_CharacterIndexMtlist.begin(), pMapBlock->m_CharacterIndexMtlist.end());
				pClientIndexVector->erase(remove(pClientIndexVector->begin(), pClientIndexVector->end(), nClientIndexToExclude), pClientIndexVector->end());
				pMapBlock->m_CharacterIndexMtlist.unlock();
			}
			j++;
		}
		i++;
	}

	return pClientIndexVector->size();
}

////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : CMapChannel::GetAdjacentCharacterIndexes
// ��ȯ�Ǵ� ��  : int ==> �������� ĳ���� �ε��� ī��Ʈ
// �� �� �� ��  : float xStart
// �� �� �� ��  : float zStart
// �� �� �� ��  : float xEnd
// �� �� �� ��  : float zEnd
// �� �� �� ��  : vector<ClientIndex_t> *pClientIndexVector
// �� �� �� ��  : ���� ��ǥ�� ���� ��ǥ�� ���ԵǴ� Block �������� ĳ���� �ε����� ���Ѵ�.
//
int CMapChannel::GetAdjacentCharacterIndexes(float xStart, float zStart, float xEnd, float zEnd, vector<ClientIndex_t> *pClientIndexVector, ClientIndex_t nClientIndexToExclude)
{
	BLOCK_INDEX			bIdxStart, bIdxEnd;
	TWO_BLOCK_INDEXES	blockIdx;
	int i, j;

	bIdxStart = CMapWorkspace::SelectBlockIndexWithPosition(xStart, zStart);
	bIdxEnd = CMapWorkspace::SelectBlockIndexWithPosition(xEnd, zEnd);

	if(bIdxStart.sX < bIdxEnd.sX)
	{
		blockIdx.sMinX = bIdxStart.sX;
		blockIdx.sMaxX = bIdxEnd.sX;
	}
	else
	{
		blockIdx.sMinX = bIdxEnd.sX;
		blockIdx.sMaxX = bIdxStart.sX;
	}
	if(bIdxStart.sZ < bIdxEnd.sZ)
	{
		blockIdx.sMinZ = bIdxStart.sZ;
		blockIdx.sMaxZ = bIdxEnd.sZ;
	}
	else
	{
		blockIdx.sMinZ = bIdxEnd.sZ;
		blockIdx.sMaxZ = bIdxStart.sZ;
	}

	pClientIndexVector->clear();
	i = blockIdx.sMinX;
	while(i <= blockIdx.sMaxX)
	{
		j = blockIdx.sMinZ;
		while(j <= blockIdx.sMaxZ)
		{
			CMapBlock *pMapBlock = &(m_arrMapBlock[i][j]);
			// character�� ������ �Ѿ

			if (!pMapBlock->m_CharacterIndexMtlist.empty())
			{
				pMapBlock->m_CharacterIndexMtlist.lock();
				pClientIndexVector->insert(pClientIndexVector->end()
					, pMapBlock->m_CharacterIndexMtlist.begin(), pMapBlock->m_CharacterIndexMtlist.end());
				pMapBlock->m_CharacterIndexMtlist.unlock();
			}
			j++;
		}
		i++;
	}

	return pClientIndexVector->size();
}

////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : CMapChannel::GetAdjacentMonsterIndexes
// ��ȯ�Ǵ� ��  : int ==> �������� ���� �ε��� ī��Ʈ
// �� �� �� ��  : float x
// �� �� �� ��  : float z
// �� �� �� ��  : float fDistance
// �� �� �� ��  : vector<ClientIndex_t> *pClientIndexVector
// �� �� �� ��  : ������ ��ǥ�� �߽����� fDistance/2 �Ÿ����� Block �����ȿ��ִ� ���� �ε����� ���Ѵ�
//
int CMapChannel::GetAdjacentMonsterIndexes(float x, float z, float fDistance, vector<ClientIndex_t> *pClientIndexVector)
{
	TWO_BLOCK_INDEXES blockIdx;
	int i, j;

	m_pMapProject->GetBlockAdjacentToPositionHalfDistance(x, z, fDistance, blockIdx);			//

	pClientIndexVector->clear();
	i = blockIdx.sMinX;
	while(i <= blockIdx.sMaxX)
	{
		j = blockIdx.sMinZ;
		while(j <= blockIdx.sMaxZ)
		{
			CMapBlock *pMapBlock = &(m_arrMapBlock[i][j]);

// 2005-12-13 by cmkwon, �� ũ�Ⱑ Ŀ���� �߽��� ������ ����ϸ� ������ ����. �׷��� ������
//			// MapBlock �Ÿ� Ȯ���ϱ�
//			if ( GGetLength(pMapBlock->m_CenterPositionX - x,
//					pMapBlock->m_CenterPositionZ - z) > fDistance/2 )
//			{
//				j++;
//				continue;
//			}

			// ���Ͱ� ������ �Ѿ
			if (!pMapBlock->m_MonsterIndexMtlist.empty())
			{
				pMapBlock->m_MonsterIndexMtlist.lock();
				pClientIndexVector->insert(pClientIndexVector->end()
					, pMapBlock->m_MonsterIndexMtlist.begin(), pMapBlock->m_MonsterIndexMtlist.end());
				pMapBlock->m_MonsterIndexMtlist.unlock();
			}
			j++;
		}
		i++;
	}

	return pClientIndexVector->size();
}

////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : CMapChannel::GetAdjacentMonsterIndexes
// ��ȯ�Ǵ� ��  : int ==> �������� ���� �ε��� ī��Ʈ
// �� �� �� ��  : float xStart
// �� �� �� ��  : float zStart
// �� �� �� ��  : float xEnd
// �� �� �� ��  : float zEnd
// �� �� �� ��  : vector<ClientIndex_t> *pClientIndexVector
// �� �� �� ��  : ���� ��ǥ�� ���� ��ǥ�� ���ԵǴ� Block �������� ���� �ε����� ���Ѵ�.
//
int CMapChannel::GetAdjacentMonsterIndexes(float xStart, float zStart, float xEnd, float zEnd, vector<ClientIndex_t> *pClientIndexVector)
{
	BLOCK_INDEX			bIdxStart, bIdxEnd;
	TWO_BLOCK_INDEXES	blockIdx;
	int i, j;

	bIdxStart = CMapWorkspace::SelectBlockIndexWithPosition(xStart, zStart);
	bIdxEnd = CMapWorkspace::SelectBlockIndexWithPosition(xEnd, zEnd);

	if(bIdxStart.sX < bIdxEnd.sX)
	{
		blockIdx.sMinX = bIdxStart.sX;
		blockIdx.sMaxX = bIdxEnd.sX;
	}
	else
	{
		blockIdx.sMinX = bIdxEnd.sX;
		blockIdx.sMaxX = bIdxStart.sX;
	}
	if(bIdxStart.sZ < bIdxEnd.sZ)
	{
		blockIdx.sMinZ = bIdxStart.sZ;
		blockIdx.sMaxZ = bIdxEnd.sZ;
	}
	else
	{
		blockIdx.sMinZ = bIdxEnd.sZ;
		blockIdx.sMaxZ = bIdxStart.sZ;
	}

	pClientIndexVector->clear();
	i = blockIdx.sMinX;
	while(i <= blockIdx.sMaxX)
	{
		j = blockIdx.sMinZ;
		while(j <= blockIdx.sMaxZ)
		{
			CMapBlock *pMapBlock = &(m_arrMapBlock[i][j]);
	
			// Monster�� ������ �Ѿ
			if (!pMapBlock->m_MonsterIndexMtlist.empty())
			{
				pMapBlock->m_MonsterIndexMtlist.lock();
				pClientIndexVector->insert(pClientIndexVector->end()
					, pMapBlock->m_MonsterIndexMtlist.begin(), pMapBlock->m_MonsterIndexMtlist.end());
				pMapBlock->m_MonsterIndexMtlist.unlock();
			}
			j++;
		}
		i++;
	}

	return pClientIndexVector->size();
}

BOOL CMapChannel::SaveUnitCountsPerBlock(void)
{
	char		szTemp[1024];
	char		szTime[256];
	time_t		ltime;
	struct tm	*today = NULL;
	HANDLE		hFile;
	DWORD		dwWritten;
	int			x, z;
	CMapBlock	*pBlock = NULL;

	sprintf(szTemp, "%s_", GET_MAP_STRING(m_MapChannelIndex));
	time(&ltime);
	today = localtime(&ltime);
	strftime(szTime, 128, "%m%d_%H%M%S.log", today);
	strcat(szTemp, szTime);

	hFile = CreateFile(szTemp, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	CMapBlock *pArrMap = this->m_arrMapBlock[0];
	int count = m_pMapProject->m_NUM_BLOCK_X * m_pMapProject->m_NUM_BLOCK_Z;
	int nTotalNum;
	int nNum;

	sprintf(szTemp, "[%04d] MapTileSize(%d, %d) MapBlockSize(%d, %d)\r\n\r\n Character",
		m_pMapProject->m_nMapIndex, m_pMapProject->m_sXSize, m_pMapProject->m_sYSize
		, m_pMapProject->m_NUM_BLOCK_X, m_pMapProject->m_NUM_BLOCK_Z);

	//////////////////////////////////////////////////////////////////////
	// Save Character Info
	nTotalNum = 0;
	for(z=m_pMapProject->m_NUM_BLOCK_Z-1 ; z>=0 ; z--)
	{
		strcat(szTemp, "\r\n");
		for(x=0 ; x < m_pMapProject->m_NUM_BLOCK_X ; x++)
		{
			if(strlen(szTemp) > 1000)
			{
				WriteFile(hFile, szTemp, strlen(szTemp), &dwWritten, NULL);
				memset(szTemp, 0x00, 1024);
			}
			pBlock = GetBlockByIndex(x, z);
			nNum = pBlock->GetNumCharachter();
			if(nNum == 0)
			{
				strcat(szTemp, ". ");
			}
			else
			{
				sprintf(&szTemp[strlen(szTemp)], "%d ", nNum);
				nTotalNum += nNum;
			}
		}
	}
	WriteFile(hFile, szTemp, strlen(szTemp), &dwWritten, NULL);
	memset(szTemp, 0x00, 1024);
	sprintf(szTemp, "\r\n====>  total: %d\r\n\r\n Monster MaxCount[%d] CurrentCount[%d]",
		nTotalNum, m_nMaxMonsterCountInChannel, m_nCurMonsterCountInChannel);

	//////////////////////////////////////////////////////////////////////
	// Save Monster Info
	nTotalNum = 0;
	for(z=m_pMapProject->m_NUM_BLOCK_Z-1 ; z>=0 ; z--)
	{
		strcat(szTemp, "\r\n");
		for(x=0 ; x < m_pMapProject->m_NUM_BLOCK_X ; x++)
		{
			if(strlen(szTemp) > 1000)
			{
				WriteFile(hFile, szTemp, strlen(szTemp), &dwWritten, NULL);
				memset(szTemp, 0x00, 1024);
			}
			pBlock = GetBlockByIndex(x, z);
			nNum = pBlock->GetNumMonster();
			if(nNum == 0)
			{
				strcat(szTemp, ". ");
			}
			else
			{
				sprintf(&szTemp[strlen(szTemp)], "%d ", nNum);
				nTotalNum += nNum;
			}
		}
	}
	WriteFile(hFile, szTemp, strlen(szTemp), &dwWritten, NULL);
	memset(szTemp, 0x00, 1024);
	sprintf(szTemp, "\r\n====>  total: %d\r\n\r\n Item", nTotalNum);

	//////////////////////////////////////////////////////////////////////
	// Save Item Info
	nTotalNum = 0;
	for(z=m_pMapProject->m_NUM_BLOCK_Z-1 ; z>=0 ; z--)
	{
		strcat(szTemp, "\r\n");
		for(x=0 ; x < m_pMapProject->m_NUM_BLOCK_X ; x++)
		{
			if(strlen(szTemp) > 1000)
			{
				WriteFile(hFile, szTemp, strlen(szTemp), &dwWritten, NULL);
				memset(szTemp, 0x00, 1024);
			}
			pBlock = GetBlockByIndex(x, z);
			nNum = pBlock->GetNumItem();
			if(nNum == 0)
			{
				strcat(szTemp, ". ");
			}
			else
			{
				sprintf(&szTemp[strlen(szTemp)], "%d ", nNum);
				nTotalNum += nNum;
			}
		}
	}
	WriteFile(hFile, szTemp, strlen(szTemp), &dwWritten, NULL);
	sprintf(szTemp, "\r\n====>  total: %d", nTotalNum);
	WriteFile(hFile, szTemp, strlen(szTemp), &dwWritten, NULL);
	CloseHandle(hFile);
	hFile = NULL;
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CMapChannel::GetAllClientIndexList(vectClientIndex_t *o_pvectCliIdxList)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-24 ~ 2006-07-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CMapChannel::GetAllClientIndexList(vectClientIndex_t *o_pvectCliIdxList)
{
	if(m_mtClientIndexList.empty())
	{
		return FALSE;
	}

	mt_auto_lock mtA(&m_mtClientIndexList);

	o_pvectCliIdxList->reserve(m_mtClientIndexList.size());
	*o_pvectCliIdxList = m_mtClientIndexList;
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			INT CMapChannel::GetWarpObjectIndexW(BYTE i_byCharInflTy)
/// \brief		
/// \author		cmkwon
/// \date		2006-12-08 ~ 2006-12-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CMapChannel::GetWarpObjectIndexW(BYTE i_byCharInflTy)
{
	return this->m_pMapProject->GetWarpObjectIndex(i_byCharInflTy);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT CMapChannel::GetTeleportWarpObjectIndexW()
/// \brief		�ڷ���Ʈ ���� ������Ʈ�� ������
/// \author		dhjin
/// \date		2007-09-15 ~ 2007-09-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CMapChannel::GetTeleportWarpObjectIndexW()
{
	return this->m_pMapProject->GetTeleportWarpObjectIndex();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-14 by cmkwon, ���� �޸� ���� ���� ���� - 
/// \author		cmkwon
/// \date		2010-04-15 ~ 2010-04-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
TILEINFO* CMapChannel::GetTileInfoW(float x, float z)
{
	return m_pMapProject->GetTileInfo(x, z);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-14 by cmkwon, ���� �޸� ���� ���� ���� - 
/// \author		cmkwon
/// \date		2010-04-15 ~ 2010-04-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
TILEINFO* CMapChannel::GetTileInfoW(const D3DXVECTOR3 *pPosVector3)
{
	return m_pMapProject->GetTileInfo(pPosVector3);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-14 by cmkwon, ���� �޸� ���� ���� ���� - 
/// \author		cmkwon
/// \date		2010-04-15 ~ 2010-04-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
TILEINFO* CMapChannel::GetTileInfoByTileIndexW(int TileX, int TileZ)
{
	return m_pMapProject->GetTileInfoByTileIndex(TileX, TileZ);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-14 by cmkwon, ���� �޸� ���� ���� ���� - 
/// \author		cmkwon
/// \date		2010-04-15 ~ 2010-04-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
EVENTINFO* CMapChannel::GetTileEventInfoW(float x, float z)
{
	return m_pMapProject->GetTileEventInfo(x, z);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-14 by cmkwon, ���� �޸� ���� ���� ���� - 
/// \author		cmkwon
/// \date		2010-04-15 ~ 2010-04-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
EVENTINFO* CMapChannel::GetTileEventInfoW(const D3DXVECTOR3 *pPosVector3)
{
	return m_pMapProject->GetTileEventInfo(pPosVector3);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-14 by cmkwon, ���� �޸� ���� ���� ���� - 
/// \author		cmkwon
/// \date		2010-04-15 ~ 2010-04-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
EVENTINFO* CMapChannel::GetTileEventInfoByTileIndexW(int TileX, int TileZ)
{
	return m_pMapProject->GetTileEventInfoByTileIndex(TileX, TileZ);
}

#ifdef _DEBUG
void CMapChannel::printUnitCountsPerBlock(void)
{
	int			nMonsters = 0;
	int			nCharacters = 0;
	CMapBlock	*pMapBlock = NULL;
	char		szTemp[256];
	char		szSpace[100];
	char		szSystemLog[1024];

	memset(szSpace, 0x20, sizeof(szSpace));

	sprintf(szSystemLog, "  Print MapBlock [%s]\r\n", GET_MAP_STRING(m_MapChannelIndex));
	g_pGlobal->WriteSystemLog(szSystemLog);
	DBGOUT(szSystemLog);
	for (int i = 0; i < m_pMapProject->m_NUM_BLOCK_X;  i++)
	{
		for (int j = 0; j < m_pMapProject->m_NUM_BLOCK_Z;  j++)
		{
			memset(szSystemLog, 0x00, sizeof(szSystemLog));
			pMapBlock = &m_arrMapBlock[i][j];

			// Block�� list�� ����
			if(pMapBlock->GetNumCharachter() != 0
				|| pMapBlock->GetNumMonster() != 0)
			{
				sprintf(szTemp, "    MapBlock[%2d][%2d] :", i, j);
				strcat(szSystemLog, szTemp);
			}

			///////////////////////////////////////////////////////////////////
			// print characters
			if(pMapBlock->GetNumCharachter() != 0)
			{
				pMapBlock->m_CharacterIndexMtlist.lock();

				mtlistUnitIndex_t::iterator itrC = pMapBlock->m_CharacterIndexMtlist.begin();
				while(itrC != pMapBlock->m_CharacterIndexMtlist.end())
				{
					sprintf(szTemp, "%5d ", *itrC);
					strcat(szSystemLog, szTemp);
					nCharacters++;
					itrC++;
				}
				pMapBlock->m_CharacterIndexMtlist.unlock();

				if(pMapBlock->GetNumMonster() != 0)
				{
					strcat(szSystemLog, "\r\n");
				}
			}

			///////////////////////////////////////////////////////////////////
			// print monsters
			if(pMapBlock->GetNumMonster() != 0)
			{
				if(strlen(szSystemLog) < 70)
				{
					memcpy(szSystemLog + strlen(szSystemLog), szSpace, 70 - strlen(szSystemLog));
				}

				pMapBlock->m_MonsterIndexMtlist.lock();
				mtlistUnitIndex_t::iterator itrM = pMapBlock->m_MonsterIndexMtlist.begin();
				while(itrM != pMapBlock->m_MonsterIndexMtlist.end())
				{
					sprintf(szTemp, "%5d ", *itrM);
					strcat(szSystemLog, szTemp);
					nMonsters++;
					itrM++;
				}
				pMapBlock->m_MonsterIndexMtlist.unlock();

				strcat(szSystemLog, "\r\n");
			}
			if(strcmp(szSystemLog, "") != 0)
			{
				g_pGlobal->WriteSystemLog(szSystemLog, FALSE);
				DBGOUT(szSystemLog);
			}
		}
	}
	sprintf(szSystemLog, "  Block TotalCount ==> Characters[%3d], Monsters[%3d]\r\n", nCharacters, nMonsters);
	g_pGlobal->WriteSystemLog(szSystemLog, FALSE);
	DBGOUT(szSystemLog);
	sprintf(szSystemLog, "        TotalCount ==>                  Monsters[%3d]\r\n", m_nCurMonsterCountInChannel);
	g_pGlobal->WriteSystemLog(szSystemLog, FALSE);
	DBGOUT(szSystemLog);
}
#endif // _DEBUG_endif