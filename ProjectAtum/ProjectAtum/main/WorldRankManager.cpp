// WorldRankManager.cpp: implementation of the CWorldRankManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "Interface.h"
#include "INFBase.h"
#include "GameDataLast.h"
#include "ShuttleChild.h"

#include "WorldRankManager.h"
#include "INFImageEx.h"	// 2011. 10. 10 by jskim UI�ý��� ����

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// ���񽺻� �̹��� ũ��
#define			SERVICE_IMG_WIDTH				48
#define			SERVICE_IMG_HEIGHT				28

#define			MAX_SERVICE_IMAGEBUFF_SYMBOL_IMAGE	(SIZE_MAX_SERVICE_SYMBOL_IMAGE+10)

CWorldRankManager::CWorldRankManager() :
	m_ServiceInfo { },
	m_ServiceInfoStruct { nullptr }
{
	int nCnt = 0;
	int nGear = 0;
	for(nCnt = 0;nCnt < MAX_WORLDRANK_PAGE;nCnt++)
	{
		for(nGear = 0;nGear < MAX_WORLDRANK_GEAR;nGear++)
		{
			m_vecLocalRank_LV[nCnt][nGear].clear();			// ���� ��ŷ
			m_vecLocalRank_Fame[nCnt][nGear].clear();			// ���� ��ŷ
			m_vecLocalRank_PVP[nCnt][nGear].clear();			// ���� ��ŷ

			m_vecWorldRank_LV[nCnt][nGear].clear();			// ���� ��ŷ
			m_vecWorldRank_Fame[nCnt][nGear].clear();			// ���� ��ŷ
			m_vecWorldRank_PVP[nCnt][nGear].clear();			// ���� ��ŷ
		}
	}	
	
	// ��û�� ��� �ε���
	m_nRqGearIdx = WORLDRANK_GEAR_ALL;

	memset(&m_structMyRankInfo, 0x00, sizeof(MSG_FC_INFO_WRK_GET_SELF_RANKING_OK));

	// ���� ��ŷ �ý��� ��� ����
	m_bUseWorldRanking = FALSE;

}

CWorldRankManager::~CWorldRankManager()
{
	DeleteServerImageInfo();
	int nCnt = 0;
	int nGear = 0;
	for(nCnt = 0;nCnt < MAX_WORLDRANK_PAGE;nCnt++)
	{
		for(nGear = 0;nGear < MAX_WORLDRANK_GEAR;nGear++)
		{
			m_vecLocalRank_LV[nCnt][nGear].clear();			// ���� ��ŷ
			m_vecLocalRank_Fame[nCnt][nGear].clear();			// ���� ��ŷ
			m_vecLocalRank_PVP[nCnt][nGear].clear();			// ���� ��ŷ

			m_vecWorldRank_LV[nCnt][nGear].clear();			// ���� ��ŷ
			m_vecWorldRank_Fame[nCnt][nGear].clear();			// ���� ��ŷ
			m_vecWorldRank_PVP[nCnt][nGear].clear();			// ���� ��ŷ
		}
	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		������ �߰��Ѵ�.
/// \author		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWorldRankManager::AddWorldRank(int i_nLocalWorldIdx,			// �����̳�? �����?
									 int i_nInfoIdx,			// ����, ��ġ, PVP
									 int i_nGearIdx,			// ��� �ε���
										structRankInfo*		pstruRankInfo)			// ������
{
	if((i_nLocalWorldIdx >= MAX_WORLDRANK_SERVICE)
		||(i_nInfoIdx >= MAX_WORLDRANK_INFO)
		|| (i_nGearIdx >= MAX_WORLDRANK_GEAR))
	{
		return;
	}

	/*
	structRankInfo struRankInfo;
	memcpy(&struRankInfo, pstruRankInfo, sizeof(structRankInfo));	

	switch(i_nLocalWorldIdx)
	{
	case WORLDRANK_SERVICE_LOCAL:
		{
			m_vecLocalRank_LV[i_nInfoIdx][i_nGearIdx].push_back(struRankInfo);
		}
		break;
	case WORLDRANK_SERVICE_WORLD:
		{
			m_vecWorldRank[i_nInfoIdx][i_nGearIdx].push_back(struRankInfo);
		}
		break;
	}		
	*/
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		��ü ����� �˾ƿ´�
/// \author		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CWorldRankManager::GetWorldRankSize(int i_nLocalWorldIdx,			// �����̳�? �����?
										int i_nInfoIdx,			// ����, ��ġ, PVP
										int i_nGearIdx,			// ��� �ε���
										int i_nPage)			// ������ ��ġ
{
	if((i_nLocalWorldIdx >= MAX_WORLDRANK_SERVICE)
		||(i_nInfoIdx >= MAX_WORLDRANK_INFO)
		|| (i_nGearIdx >= MAX_WORLDRANK_GEAR)
		|| (i_nPage >= MAX_WORLDRANK_PAGE))
	{
		return 0;
	}
	
	switch(i_nLocalWorldIdx)
	{
	case WORLDRANK_SERVICE_LOCAL:
		{
			switch(i_nInfoIdx)
			{
			case WORLDRANK_INFO_LV:		// ����
				{
					return m_vecLocalRank_LV[i_nPage][i_nGearIdx].size();
				}
				break;
			case WORLDRANK_INFO_HONOR:	// ��ġ
				{
					return m_vecLocalRank_Fame[i_nPage][i_nGearIdx].size();
				}
				break;
			case WORLDRANK_INFO_PVP:	// Pvp
				{
					return m_vecLocalRank_PVP[i_nPage][i_nGearIdx].size();
				}
				break;
			}			
		}
		break;
	case WORLDRANK_SERVICE_WORLD:
		{
			switch(i_nInfoIdx)
			{
			case WORLDRANK_INFO_LV:		// ����
				{
					return m_vecWorldRank_LV[i_nPage][i_nGearIdx].size();
				}
				break;
			case WORLDRANK_INFO_HONOR:	// ��ġ
				{
					return m_vecWorldRank_Fame[i_nPage][i_nGearIdx].size();
				}
				break;
			case WORLDRANK_INFO_PVP:	// Pvp
				{
					return m_vecWorldRank_PVP[i_nPage][i_nGearIdx].size();
				}
				break;
			}		
		}
		break;
	}	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		��ŷ ������ ���´�.
/// \author		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
structRankInfo *CWorldRankManager::GetWorldRankInfo(int i_nLocalWorldIdx,			// �����̳�? �����?
										int i_nInfoIdx,			// ����, ��ġ, PVP
										int i_nGearIdx,			// ��� �ε���
										int i_nSelectPage,
										int i_nIdx)			
{
	if((i_nLocalWorldIdx >= MAX_WORLDRANK_SERVICE)
		||(i_nInfoIdx >= MAX_WORLDRANK_INFO)
		|| (i_nGearIdx >= MAX_WORLDRANK_GEAR)
		||(i_nSelectPage >= MAX_WORLDRANK_PAGE))
	{
		return NULL;
	}
	vector<structRankInfo>*	pSelRank = NULL;
	switch(i_nLocalWorldIdx)
	{
	case WORLDRANK_SERVICE_LOCAL:
		{
			switch(i_nInfoIdx)
			{
			case WORLDRANK_INFO_LV:		// ����
				{
					if(m_vecLocalRank_LV[i_nSelectPage][i_nGearIdx].size() <= i_nIdx)
					{
						return NULL;
					}
					return &m_vecLocalRank_LV[i_nSelectPage][i_nGearIdx][i_nIdx];
				}
				break;
			case WORLDRANK_INFO_HONOR:	// ��ġ
				{
					if(m_vecLocalRank_Fame[i_nSelectPage][i_nGearIdx].size() <= i_nIdx)
					{
						return NULL;
					}
					return &m_vecLocalRank_Fame[i_nSelectPage][i_nGearIdx][i_nIdx];
				}
				break;
			case WORLDRANK_INFO_PVP:	// Pvp
				{
					if(m_vecLocalRank_PVP[i_nSelectPage][i_nGearIdx].size() <= i_nIdx)
					{
						return NULL;
					}
					return &m_vecLocalRank_PVP[i_nSelectPage][i_nGearIdx][i_nIdx];
				}
				break;
			}
		}
		break;
	case WORLDRANK_SERVICE_WORLD:
		{
			switch(i_nInfoIdx)
			{
			case WORLDRANK_INFO_LV:		// ����
				{
					if(m_vecWorldRank_LV[i_nSelectPage][i_nGearIdx].size() <= i_nIdx)
					{
						return NULL;
					}
					return &m_vecWorldRank_LV[i_nSelectPage][i_nGearIdx][i_nIdx];
				}
				break;
			case WORLDRANK_INFO_HONOR:	// ��ġ
				{
					if(m_vecWorldRank_Fame[i_nSelectPage][i_nGearIdx].size() <= i_nIdx)
					{
						return NULL;
					}
					return &m_vecWorldRank_Fame[i_nSelectPage][i_nGearIdx][i_nIdx];
				}
				break;
			case WORLDRANK_INFO_PVP:	// Pvp
				{
					if(m_vecWorldRank_PVP[i_nSelectPage][i_nGearIdx].size() <= i_nIdx)
					{
						return NULL;
					}
					return &m_vecWorldRank_PVP[i_nSelectPage][i_nGearIdx][i_nIdx];
				}
				break;
			}
		}
		break;	
	}
	
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� ��ŷ ������ ���´�.
/// \author		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CWorldRankManager::GetMyRankInfo(int i_nInfoIdx)// ����, ��ġ, PVP)
{
	switch(i_nInfoIdx)
	{
	case WORLDRANK_INFO_LV:
		{
			return m_structMyRankInfo.LevelRanking;
		}
		break;
	case WORLDRANK_INFO_HONOR:
		{
			return m_structMyRankInfo.FameRanking;
		}
		break;
	case WORLDRANK_INFO_PVP:
		{
			return m_structMyRankInfo.PvPRanking;
		}
		break;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� ��ŷ ������ ���´�.
/// \author		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CWorldRankManager::RqGetServiceInfo()
{
	if(!GetUseWorldRanking())
	{
		// ���� ��ŷ �ý��� ��� ����
		return FALSE;
	}
	if(m_ServiceInfo.ServiceName[0] == '\0')
	{
		// � ������ ������ ����
		return FALSE;
	}
	g_pFieldWinSocket->SendMsg(T_FC_INFO_WRK_GET_SERVICE_INFO,NULL,NULL);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� ��ŷ ������ ���´�.
/// \author		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CWorldRankManager::RqGetMyRankInfo()
{
	if(!GetUseWorldRanking())
	{
		// ���� ��ŷ �ý��� ��� ����
		return FALSE;
	}

// 	if(m_structMyRankInfo.CharacUID != 0)
// 	{
// 		// � ������ ������ ����
// 		return FALSE;
// 	}
	g_pFieldWinSocket->SendMsg(T_FC_INFO_WRK_GET_SELF_RANKING,NULL,NULL);
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� ��ŷ ������ ���´�.
/// \author		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWorldRankManager::SetMyRankInfo(MSG_FC_INFO_WRK_GET_SELF_RANKING_OK	*pstructMyRankInfo)
{
	memcpy(&m_structMyRankInfo, pstructMyRankInfo, sizeof(MSG_FC_INFO_WRK_GET_SELF_RANKING_OK));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWorldRankManager::AddServiceInfo(MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK* pMsg)
{
	MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK sMsg;

	util::zero(sMsg);

	m_ServiceInfo = sMsg;

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK CWorldRankManager::GetServiceInfo()
{
	return m_ServiceInfo;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CWorldRankManager::GetServiceStringInfo(char* o_pServer)
{
	util::strncpy(o_pServer, m_ServiceInfo.ServiceName, SIZE_MAX_SERVICE_NAME);

	return m_ServiceInfo.ServiceName[0] != '\0';
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� �� �̹����� �����Ѵ�.
/// \author		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWorldRankManager::AddServiceInfoImage(MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK_IMAGE* pMsg)
{	
	auto pServiceInfo = m_ServiceInfoStruct;

	if (!pServiceInfo)
	{
		// ������ ���� ����
		pServiceInfo = new structServiceInfo;		
		pServiceInfo->nDataPos = 0;		
		pServiceInfo->bImageLoadDone = FALSE;		// �̹��� �ε� ����		
		pServiceInfo->pImage = NULL;		
		pServiceInfo->bRestored = FALSE;	// �ʱ�ȭ ���� ������

		// 4096�� �ò����� �׷��� �ణ �� ����
		pServiceInfo->pData = new char[MAX_SERVICE_IMAGEBUFF_SYMBOL_IMAGE];
		memset(pServiceInfo->pData, 0x00, MAX_SERVICE_IMAGEBUFF_SYMBOL_IMAGE);		
	}

	// �̹����� 4096�� �ò��� �׷��Ƿ� �޸� �����÷ΰ� �����ִ�.
	// ���� ������ �ε����� 96�̴� ->1000+1000+1000+1000+96 = 4096
	BOOL bReceiveImageDone = FALSE;
	int nCopySize = 1000;// BYTE	SymbolImage[1000]; 
	if((1 == pMsg->SymbolImageIndex)
		||((pServiceInfo->nDataPos + nCopySize) >= MAX_SERVICE_IMAGEBUFF_SYMBOL_IMAGE))
	{
		bReceiveImageDone = TRUE;		
	}
	
	if((pServiceInfo->nDataPos+nCopySize) < MAX_SERVICE_IMAGEBUFF_SYMBOL_IMAGE)
	{
		memcpy(&pServiceInfo->pData[pServiceInfo->nDataPos], pMsg->SymbolImage, nCopySize);		
	}

	if(bReceiveImageDone && !pServiceInfo->bImageLoadDone)
	{		
		pServiceInfo->bImageLoadDone = TRUE;
// 		pServiceInfo->pImage = new CINFImage;		
// 		pServiceInfo->pImage->InitDeviceObjects( pServiceInfo->pData, 4096 );		
// 		pServiceInfo->pImage->RestoreDeviceObjects();
// 		pServiceInfo->bRestored = TRUE;
	}		
	
	pServiceInfo->nDataPos = pServiceInfo->nDataPos + nCopySize;
	
	m_ServiceInfoStruct = pServiceInfo;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� �� �̹����� �����´�.
/// \author		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CINFImage* CWorldRankManager::GetServiceInfoImage()
{	 
	if (m_ServiceInfoStruct) return m_ServiceInfoStruct->pImage;
	
	return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� �� �̹����� �����Ѵ�.
/// \author		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWorldRankManager::InitDeviceObjects_ServiceInfoImage()
{	
	auto pServiceInfo = m_ServiceInfoStruct;
	if(pServiceInfo && !pServiceInfo->pImage && pServiceInfo->bImageLoadDone)
	{
		MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK  stServerInfo = GetServiceInfo();
        // 2011. 10. 10 by jskim UI�ý��� ����
		pServiceInfo->pImage = new CINFImageEx;	
		DataHeader* pHeader = new DataHeader;
		pHeader->m_pData = pServiceInfo->pData;
		pHeader->m_DataSize = stServerInfo.SymbolImageSize;

		pServiceInfo->pImage->InitDeviceObjects( pHeader );	
        // end 2011. 10. 10 by jskim UI�ý��� ����

		pServiceInfo->bRestored = FALSE;
	}				
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� �� �̹����� �����Ѵ�.
/// \author		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWorldRankManager::RestoreDeviceObjects_ServiceInfoImage()
{
	auto pServiceInfo = m_ServiceInfoStruct;

	if(pServiceInfo && pServiceInfo->pImage && pServiceInfo->bImageLoadDone && !pServiceInfo->bRestored)
	{
		pServiceInfo->pImage->RestoreDeviceObjects();
		pServiceInfo->pImage->SetRect(0,0,SERVICE_IMG_WIDTH,SERVICE_IMG_HEIGHT);

		pServiceInfo->bRestored = TRUE;
	}				
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� �� �̹����� �����Ѵ�.
/// \author		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWorldRankManager::InvalidateDeviceObjects_ServiceInfoImage()
{
	auto pServiceInfo = m_ServiceInfoStruct;

	if(pServiceInfo && pServiceInfo->pImage && pServiceInfo->bImageLoadDone && pServiceInfo->bRestored)
	{
		pServiceInfo->pImage->InvalidateDeviceObjects();
		pServiceInfo->bRestored = FALSE;
	}				
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� �� �̹����� �����Ѵ�.
/// \author		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWorldRankManager::DeleteDeviceObjects_ServiceInfoImage()
{
	auto pServiceInfo = m_ServiceInfoStruct;

	if (m_ServiceInfoStruct)
	{
		pServiceInfo->bRestored = FALSE;

		if (pServiceInfo->pImage)
		{
			pServiceInfo->pImage->DeleteDeviceObjects();
			util::del(pServiceInfo->pImage);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� �縦 ���´�.
/// \author		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWorldRankManager::DeleteServerImageInfo()
{
	auto it = m_ServiceInfoStruct;

	if (it)
	{
		if (it->pImage)
		{
			it->pImage->InvalidateDeviceObjects();
			it->pImage->DeleteDeviceObjects();
			util::del(it->pImage);
		}
		util::del(it->pData);
		util::del(it);
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		��� ��ŷ ������ ���´�.
/// \author		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CWorldRankManager::RqWorldRankInfo(int i_nLocalWorldIdx,			// �����̳�? �����?
										int i_nInfoIdx,			// ����, ��ġ, PVP
										int i_nGearIdx,			// ��� �ε���
										int i_nIdx,
										int i_nRqItemCnt,
										BOOL bCtrlRq)	// Ŭ���ؼ� ��û�Ѱų�?
{
	if(!GetUseWorldRanking())
	{
		// ���� ��ŷ �ý��� ��� ����
		return TRUE;
	}

	if((i_nLocalWorldIdx >= MAX_WORLDRANK_SERVICE)
		||(i_nInfoIdx >= MAX_WORLDRANK_INFO)
		|| (i_nGearIdx >= MAX_WORLDRANK_GEAR))
	{
		return TRUE;
	}

	if(!IsRqWorldRankInfo(i_nLocalWorldIdx, i_nInfoIdx, i_nGearIdx, i_nIdx))
	{
		return TRUE;
	}
	
	MSG_FC_INFO_WRK_GET_RANKER_LIST sMsg;
	memset(&sMsg, 0x00, sizeof(MSG_FC_INFO_WRK_GET_RANKER_LIST));

	// ����? ����?
	switch(i_nLocalWorldIdx)
	{
	case WORLDRANK_SERVICE_LOCAL:
		{
			sMsg.byScope = RANKING_SCOPE_LOCAL;
		}
		break;
	case WORLDRANK_SERVICE_WORLD:
		{
			sMsg.byScope = RANKING_SCOPE_WORLD;
		}
		break;
	}
	// ��� �ε���
	m_nRqGearIdx = i_nGearIdx;	// ��û�� ��� �ε���
	switch(i_nGearIdx)
	{
	case WORLDRANK_GEAR_ALL:
		{
			// ��ü ��� ��û
			sMsg.UnitKind = UNITKIND_ALL_MASK;			
		}
		break;
	case WORLDRANK_GEAR_B:
		{
			sMsg.UnitKind = UNITKIND_BGEAR_MASK;
		}
		break;
	case WORLDRANK_GEAR_M:
		{
			sMsg.UnitKind = UNITKIND_MGEAR_MASK;
		}
		break;
	case WORLDRANK_GEAR_A:
		{
			sMsg.UnitKind = UNITKIND_AGEAR_MASK;
		}
		break;
	case WORLDRANK_GEAR_I:
		{
			sMsg.UnitKind = UNITKIND_IGEAR_MASK;
		}
		break;
	}
	// ����, ��ġ, PVP
	switch(i_nInfoIdx)
	{
	case WORLDRANK_INFO_LV:
		{
			sMsg.byRankingType = RANKING_TYPE_LEVEL;
		}
		break;
	case WORLDRANK_INFO_HONOR:
		{
			sMsg.byRankingType = RANKING_TYPE_FAME;
		}
		break;
	case WORLDRANK_INFO_PVP:
		{
			sMsg.byRankingType = RANKING_TYPE_PVP;
		}
		break;
	}

	// ���� �ε���
	sMsg.byStartRank = i_nIdx;
	
	// ������
	sMsg.byReqRankerCnt = i_nRqItemCnt;
	
	g_pFieldWinSocket->SendMsg(T_FC_INFO_WRK_GET_RANKER_LIST, (char*)&sMsg, sizeof(sMsg));

	return TRUE;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�̹� �ִ� ���� ���� üũ�ؼ� ��û���� ���� �Ǵ�
/// \author		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CWorldRankManager::IsRqWorldRankInfo(int i_nLocalWorldIdx,			// �����̳�? �����?
										int i_nInfoIdx,			// ����, ��ġ, PVP
										int i_nGearIdx,			// ��� �ε���
										int i_nStartIdx)		// ���� ������
{
	int nIdx = (i_nStartIdx /MAX_WORLDRANK_PAGE);
	// �̹� �ִ� ���� ���� üũ
	if(GetWorldRankSize(i_nLocalWorldIdx,i_nInfoIdx, i_nGearIdx, nIdx) > 0)
	{
		return FALSE;
	}
	return TRUE;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� ��ŷ ���� �߰�
/// \author		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWorldRankManager::AddWorldRank_Lv(int nStartRank, SWRK_LEVEL* pSwrk)
{
	structRankInfo struRankInfo;
	memset(&struRankInfo, 0x00, sizeof(struRankInfo));	

	int nPage = (nStartRank-1)/MAX_WORLDRANK_ONEPAGE;	// �������� �ִ������ ���?

	int nGear = m_nRqGearIdx;

	if(nPage < 0)
	{
		nPage = 0;
	}
	else if(nPage >= MAX_WORLDRANK_PAGE)
	{
		nPage = MAX_WORLDRANK_PAGE-1;
	}

	// ���񽺸�
	GetServiceStringInfo(struRankInfo.chServiceName);
	
	strcpy(struRankInfo.chGearType, AtumSJ::GetUnitKindString(pSwrk->UnitKind));	
	

	// ������
	util::strncpy(struRankInfo.chServerName, pSwrk->ServerGroupNameforRK, SIZE_MAX_SERVER_NAME);
	// �г���
	util::strncpy(struRankInfo.chNickName, pSwrk->NameforRK, SIZE_MAX_NAME_FOR_RANKING);

	// ���¸�
	if(IS_VCN_INFLUENCE_TYPE(pSwrk->InfluenceType))
	{
		wsprintf(struRankInfo.chInfluence, INFLUENCE_NAME_BCU);
	}	
	else if(IS_ANI_INFLUENCE_TYPE(pSwrk->InfluenceType))
	{
		wsprintf(struRankInfo.chInfluence, INFLUENCE_NAME_ANI);
	}
	//����
	struRankInfo.nLevel = pSwrk->Level;
	
	// � ��ŷ�̳�?
	switch(pSwrk->RankingScope)		// RANKING_SCOPE_XXX
	{
	case RANKING_SCOPE_LOCAL:		// ���� ���� ���� ��ŷ, Name �� NickName
		{
			m_vecLocalRank_LV[nPage][nGear].push_back(struRankInfo);			// ���� ��ŷ_����

		}
		break;
	case RANKING_SCOPE_WORLD:		// ���� ��ŷ ����, Name �� CharacterName
		{
			m_vecWorldRank_LV[nPage][nGear].push_back(struRankInfo);			// ���� ��ŷ_����			
		}
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		��ġ ��ŷ ���� �߰�
/// \author		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWorldRankManager::AddWorldRank_Fame(int nStartRank, SWRK_FAME* pSwrk)
{
	structRankInfo struRankInfo;
	memset(&struRankInfo, 0x00, sizeof(struRankInfo));	
	
	int nPage = (nStartRank-1)/MAX_WORLDRANK_ONEPAGE;	// �������� �ִ������ ���?
	
	int nGear = m_nRqGearIdx;
	
	if(nPage < 0)
	{
		nPage = 0;
	}
	else if(nPage >= MAX_WORLDRANK_PAGE)
	{
		nPage = MAX_WORLDRANK_PAGE-1;
	}
	
	// ���񽺸�
	GetServiceStringInfo(struRankInfo.chServiceName);
	// ���ָ�
	{
		strcpy(struRankInfo.chGearType, AtumSJ::GetUnitKindString(pSwrk->UnitKind));	
	}
	
	// ������
	util::strncpy(struRankInfo.chServerName, pSwrk->ServerGroupNameforRK, SIZE_MAX_SERVER_NAME);
	// �г���
	util::strncpy(struRankInfo.chNickName, pSwrk->NameforRK, SIZE_MAX_NAME_FOR_RANKING);
	
	// ���¸�
	if(IS_VCN_INFLUENCE_TYPE(pSwrk->InfluenceType))
	{
		wsprintf(struRankInfo.chInfluence, INFLUENCE_NAME_BCU);
	}	
	else if(IS_ANI_INFLUENCE_TYPE(pSwrk->InfluenceType))
	{
		wsprintf(struRankInfo.chInfluence, INFLUENCE_NAME_ANI);
	}
	//����
	struRankInfo.nLevel = pSwrk->Level;
	// ���� ��
	struRankInfo.nHonor = pSwrk->Fame;
	
	// � ��ŷ�̳�?
	switch(pSwrk->RankingScope)		// RANKING_SCOPE_XXX
	{
	case RANKING_SCOPE_LOCAL:		// ���� ���� ���� ��ŷ, Name �� NickName
		{
			m_vecLocalRank_Fame[nPage][nGear].push_back(struRankInfo);			// ���� ��ŷ_����
			
		}
		break;
	case RANKING_SCOPE_WORLD:		// ���� ��ŷ ����, Name �� CharacterName
		{
			m_vecWorldRank_Fame[nPage][nGear].push_back(struRankInfo);			// ���� ��ŷ_����			
		}
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		PVP ��ŷ ���� �߰�
/// \author		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWorldRankManager::AddWorldRank_PVP(int nStartRank, SWRK_PVP* pSwrk)
{
	structRankInfo struRankInfo;
	memset(&struRankInfo, 0x00, sizeof(struRankInfo));	
	
	int nPage = (nStartRank-1)/MAX_WORLDRANK_ONEPAGE;	// �������� �ִ������ ���?
	
	int nGear = m_nRqGearIdx;
	
	if(nPage < 0)
	{
		nPage = 0;
	}
	else if(nPage >= MAX_WORLDRANK_PAGE)
	{
		nPage = MAX_WORLDRANK_PAGE-1;
	}
	
	// ���񽺸�
	GetServiceStringInfo(struRankInfo.chServiceName);
	// ���ָ�
	{
		strcpy(struRankInfo.chGearType, AtumSJ::GetUnitKindString(pSwrk->UnitKind));	
	}
	
	// ������
	util::strncpy(struRankInfo.chServerName, pSwrk->ServerGroupNameforRK, SIZE_MAX_SERVER_NAME);
	// �г���
	util::strncpy(struRankInfo.chNickName, pSwrk->NameforRK, SIZE_MAX_NAME_FOR_RANKING);
	
	// ���¸�
	if(IS_VCN_INFLUENCE_TYPE(pSwrk->InfluenceType))
	{
		wsprintf(struRankInfo.chInfluence, INFLUENCE_NAME_BCU);
	}	
	else if(IS_ANI_INFLUENCE_TYPE(pSwrk->InfluenceType))
	{
		wsprintf(struRankInfo.chInfluence, INFLUENCE_NAME_ANI);
	}
	//����
	struRankInfo.nLevel = pSwrk->Level;
	// ���ھ�
	struRankInfo.nScore = pSwrk->Score;
	// ��
	struRankInfo.nWinNum = pSwrk->WinPoint;
	// ��
	struRankInfo.nLoseNum = pSwrk->LossPoint;
	
	// � ��ŷ�̳�?
	switch(pSwrk->RankingScope)		// RANKING_SCOPE_XXX
	{
	case RANKING_SCOPE_LOCAL:		// ���� ���� ���� ��ŷ, Name �� NickName
		{
			m_vecLocalRank_PVP[nPage][nGear].push_back(struRankInfo);			// ���� ��ŷ_����
			
		}
		break;
	case RANKING_SCOPE_WORLD:		// ���� ��ŷ ����, Name �� CharacterName
		{
			m_vecWorldRank_PVP[nPage][nGear].push_back(struRankInfo);			// ���� ��ŷ_����			
		}
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�г��� �߰�
/// \author		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWorldRankManager::RqCreateMyNickName(char* i_pNickName, UID32_t	i_myCharacterUniqueNumber)
{
	if(!GetUseWorldRanking())
	{
		// ���� ��ŷ �ý��� ��� ����
		return;
	}
	MSG_FC_CHARACTER_CHANGE_NICKNAME sMsg;
	memset(&sMsg, 0x00, sizeof(MSG_FC_CHARACTER_CHANGE_NICKNAME));

	sMsg.CharacUID  = i_myCharacterUniqueNumber;
	strncpy(sMsg.NickName, i_pNickName, 10);	// �г����� 10���ڷ� 
	
	{
		vector<MessageType_t> vecUnLockMsg;
		vecUnLockMsg.clear();
		vecUnLockMsg.push_back(T_FC_CHARACTER_CHANGE_NICKNAME);		
		vecUnLockMsg.push_back(T_FC_CHARACTER_CHANGE_NICKNAME_OK);		
		// �ϳ��� ��Ī �Ǿ ���� Ǯ����. 
		g_pD3dApp->EnterMultiLock(TRUE, T_FC_CHARACTER_CHANGE_NICKNAME, vecUnLockMsg, MULTI_LOCK_ONE_MATCHING);
	}

	g_pFieldWinSocket->SendMsg(T_FC_CHARACTER_CHANGE_NICKNAME, (char*)&sMsg, sizeof(sMsg));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�г��� �߰�
/// \author		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CWorldRankManager::IsValidNickName(char* i_pCharater)
{
	if(0 == strlen(i_pCharater))
	{
		return FALSE;
	}
	int nCnt = 0;
	for(nCnt = 0;nCnt < strlen(i_pCharater)+1;nCnt++)
	{
		char	*pStr = CharNext(&i_pCharater[nCnt]);
		if(NULL == pStr)
		{
			break;
		}
		int nStrPointGap = pStr - &i_pCharater[nCnt];
		if(nStrPointGap > 1)
		{
			// ���� ���ڰ� �ƴ� �۾��� �ִ�.
			return FALSE;
		}
		if( ('a' <= i_pCharater[nCnt]) &&(i_pCharater[nCnt] <= 'z'))
		{
			int a=1;

		}
		else if( ('A' <= i_pCharater[nCnt]) &&(i_pCharater[nCnt] <= 'Z'))
		{
			int a=2;

		}
		else if( ('0' <= i_pCharater[nCnt]) &&(i_pCharater[nCnt] <= '9' ))
		{
			int a=3;
		}
		else if(NULL == i_pCharater[nCnt])
		{
			int a=4;
		}
		else
		{
			// �����Ǵ� ���ڰ��ִ�.
			return FALSE;
		}
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		ó�� ���� ��ŷ ������ ��û�Ѵ�.
/// \author		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWorldRankManager::RqInitWorldRank()
{
	if(!GetUseWorldRanking())
	{
		// ���� ��ŷ �ý��� ��� ����
		return;
	}
	const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
	if(IS_CITY_MAP_INDEX(myShuttleInfo.MapChannelIndex.MapIndex))
	{
		// �����ϋ��� ��û		
		if(!RqGetServiceInfo())	// ���� ����Ʈ ��û
		{
			// ��ü 1���� ��Ŀ���� ������ ��û�Ѵ�.
			g_pD3dApp->RqAllWorldRankInfo();
		}
		RqGetMyRankInfo(); // ���� ��ŷ ���� ��û
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWorldRankManager::SetUseWorldRanking(BOOL bUseWorldRanking)
{
	m_bUseWorldRanking = bUseWorldRanking;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// ���� ��ŷ �ý��� ��� ����
/// \author		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
/// \date		2009-02-20 ~ 2009-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CWorldRankManager::GetUseWorldRanking()
{
	return m_bUseWorldRanking;		

}