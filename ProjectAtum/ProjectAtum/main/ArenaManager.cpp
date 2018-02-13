// ArenaManager.cpp: implementation of the CArenaManager class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "ArenaManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CArenaManager::CArenaManager()
{	
	m_nArenaState = ARENA_STATE_NONE;

	m_pInfoArenaServer = new MSG_FC_CONNECT_ARENASERVER_INFO;
	memset(m_pInfoArenaServer, 0x00, sizeof(MSG_FC_CONNECT_ARENASERVER_INFO));

	m_pAFSCharacter = new CHARACTER;
	memset(m_pAFSCharacter, 0x00, sizeof(CHARACTER));
	
	m_pMFSCharacter = new CHARACTER;
	memset(m_pMFSCharacter, 0x00, sizeof(CHARACTER));

	m_bGmModeConnect = FALSE;

	m_vecArenaServerInfo.clear();
	m_bForceCloseArenaSocket = FALSE;

	m_nInfPoint = 0;

}

CArenaManager::~CArenaManager()
{
	m_vecArenaServerInfo.clear();

	util::del(m_pInfoArenaServer);
	util::del(m_pAFSCharacter);
	util::del(m_pMFSCharacter);
	

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-11-20 by bhsohn �ʷε� �ϴ� ��� ����
/// \date		2007-11-20 ~ 2007-11-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::SetArenaState(int nState)
{
	DBGOUT("CArenaManager::SetArenaState [%d] \n", nState);
	m_nArenaState = nState;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� �Ʒ��� ������ �����´�. 
/// \author		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CArenaManager::GetArenaState()
{
	return m_nArenaState;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�Ʒ��� ���� ������ ���´�.
/// \author		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
MSG_FC_CONNECT_ARENASERVER_INFO* CArenaManager::GetArenaServerInfo()
{
	return m_pInfoArenaServer;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�Ʒ��� ���� ������ ������Ʈ �Ѵ�.
/// \author		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::SetArenaServerInfo(MSG_FC_CONNECT_ARENASERVER_INFO* pMsg)
{
	memcpy(m_pInfoArenaServer, pMsg, sizeof(MSG_FC_CONNECT_ARENASERVER_INFO));	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�Ʒ��� ���� ������ ������Ʈ �Ѵ�.
/// \author		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::SetArenaCharacterInfo(CHARACTER	 *pAFSCharacter)
{
	memcpy(m_pAFSCharacter, pAFSCharacter, sizeof(CHARACTER));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�Ʒ��� ���� ������ ���´�.
/// \author		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CHARACTER*	CArenaManager::GetArenaCharacterInfo()
{
	return m_pAFSCharacter;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���μ��� ������ ������Ʈ �Ѵ�.
/// \author		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::SetMFSMyShuttleInfo(CHARACTER *pMyShuttleInfo)
{
	memcpy(m_pMFSCharacter, pMyShuttleInfo, sizeof(CHARACTER));	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���μ��� ������ �����´�.
/// \author		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CHARACTER* CArenaManager::GetMFSMyShuttleInfo()
{
	return m_pMFSCharacter;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumApplication::ConevertArenaRenderUserName()
/// \brief		������ �Ʒ����ʿ��� �������� �� �̸� ����
/// \author		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::ConevertArenaRenderUserName(int nArenaState, char* pSrcDst)
{
	if((ARENA_STATE_ARENA_GAMING != nArenaState)
		&&(ARENA_STATE_ARENA_LOAD_GAME_INFO != nArenaState))
	{
		return;
	}
	char chFindChar[8];
	char chTmpBuff[128];
	//memset(chFindChar, 0x00, 8);
	//sprintf(chFindChar, "\\");	
	//memset(chTmpBuff, 0x00, 128);
	char* pFindChChatting = strstr(pSrcDst, chFindChar);

	if(pFindChChatting)
	{
//		BOOL bFindServerName = FALSE;
//		char chArenaServerId[SIZE_MAX_SERVER_NAME];		
//		util::strncpy(chArenaServerId, pFindChChatting, SIZE_MAX_SERVER_NAME);
//		bFindServerName = GetArenaId_To_ArenaServerName(chArenaServerId, chArenaServerId);
		
		int nPos = pFindChChatting - pSrcDst;
		strncpy(chTmpBuff, pSrcDst, nPos);
		util::strncpy(pSrcDst, chTmpBuff, nPos+1);
		
//		if(bFindServerName)
//		{
//			// ���� ��Ͽ� �̸��� �ִ�.
//			strcat(pSrcDst, "_");
//			strcat(pSrcDst, chArenaServerId);
//		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaManager::SetGmModeConnect(BOOL bGmModeConnect)
/// \brief		GM�� �Ʒ��� ������ ���ӽõ���?
/// \author		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::SetGmModeConnect(BOOL bGmModeConnect)
{
	m_bGmModeConnect = bGmModeConnect;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CArenaManager::IsGmModeConnect()
/// \brief		GM�� �Ʒ��� ������ ���ӽõ���?
/// \author		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CArenaManager::IsGmModeConnect()
{
	return m_bGmModeConnect ;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void FieldSocketArenaServerSServerGroupForClient(MSG_FC_CONNECT_ARENASERVER_SSERVER_GROUP_FOR_CLIENT* pMsg)
/// \brief		�Ʒ��� ���� ����� �˷��ش�.
/// \author		// 2008-02-25 by bhsohn �Ʒ��� ���� ���� ������ ǥ���ϰ� ����
/// \date		2008-02-25 ~ 2008-02-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::FieldSocketArenaServerSServerGroupForClient(MSG_FC_CONNECT_ARENASERVER_SSERVER_GROUP_FOR_CLIENT* pMsg)
{
	m_vecArenaServerInfo.clear();
	int nCnt = 0;
	for(nCnt = 0;nCnt < SIZE_MAX_SERVER_GROUP_COUNT; nCnt++)
	{
		m_vecArenaServerInfo.push_back(pMsg->ServerGroupInfo[nCnt]);
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void GetArenaId_To_ArenaServerName(char* pArenaId, char* pDstServerName)
/// \brief		�Ʒ��� ���� ��Ͽ��� ���� �̸��� ���´�.
/// \author		// 2008-02-25 by bhsohn �Ʒ��� ���� ���� ������ ǥ���ϰ� ����
/// \date		2008-02-25 ~ 2008-02-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CArenaManager::GetArenaId_To_ArenaServerName(char* pArenaId, char* pDstServerName)
{
	vector<SSERVER_GROUP_FOR_CLIENT>::iterator it = m_vecArenaServerInfo.begin();
	while(it != m_vecArenaServerInfo.end())
	{
		SSERVER_GROUP_FOR_CLIENT sMsg = (*it);
		if(0 == stricmp(sMsg.MFS_ServerIDName, pArenaId))
		{
			util::strncpy(pDstServerName, sMsg.MFS_Name, SIZE_MAX_SERVER_NAME);
			return TRUE;
		}
		it++;
	}
	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaManager::SetGmModeConnect(BOOL bGmModeConnect)
/// \brief		GM�� �Ʒ��� ������ ���ӽõ���?
/// \author		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::SetForceCloseArenaSocket(BOOL bForceCloseArenaSocket)
{	
	m_bForceCloseArenaSocket = bForceCloseArenaSocket;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CArenaManager::IsGmModeConnect()
/// \brief		GM�� �Ʒ��� ������ ���ӽõ���?
/// \author		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CArenaManager::IsForceCloseArenaSocket()
{
	return m_bForceCloseArenaSocket;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-15 by bhsohn �Ʒ��� ������Ʈ ��� �ý��� ����
/// \date		2008-04-15 ~ 2008-04-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::SetInflPoint(int nInflPoint)
{
	m_nInfPoint = nInflPoint;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-15 by bhsohn �Ʒ��� ������Ʈ ��� �ý��� ����
/// \date		2008-04-15 ~ 2008-04-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int  CArenaManager::GetInflPoint()
{
	return m_nInfPoint;
}