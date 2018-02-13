// FieldMapWorkspace.cpp: implementation of the CFieldMapWorkspace class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FieldMapWorkspace.h"
#include "FieldMapProject.h"
#include "config.h"
#include "FieldIOCP.h"
#include "FieldGlobal.h"
#include "AtumLogSender.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFieldMapWorkspace::CFieldMapWorkspace(CIOCP* pIOCP, ENServerType serverType)
	: CMapWorkspace(pIOCP, SIZE_BLOCK_X, SIZE_BLOCK_Z)
{
	m_bLogFlagMapBandwidth			= FALSE;

// 2008-07-11 by cmkwon, MySQL ������ ���� ����(IP,Port �� DB ���� ����) - �Ʒ��� ���� ���� ��
//	BOOL bRet = m_odbcStmtForLoading.Init((LPCSTR)g_pFieldGlobal->GetODBCDSN(), (LPCSTR)g_pFieldGlobal->GetODBCUID(), (LPCSTR)g_pFieldGlobal->GetODBCPASSWORD());
	BOOL bRet = m_odbcStmtForLoading.Init(g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (char*)g_pGlobal->GetODBCUID(), (char*)g_pGlobal->GetODBCPASSWORD(), g_pGlobal->GetMainWndHandle());

	if (!bRet)
	{
		// 2008-09-05 by cmkwon, DBServer ���� ���� �ý��� �α� ����� - 
		g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] fail to connect DBServer(CFieldMapWorkspace::CFieldMapWorkspace_) !! %s,%d %s %s %s\r\n"
			, g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (CHAR*)g_pGlobal->GetODBCUID(), (CHAR*)g_pGlobal->GetODBCPASSWORD());

		MessageBox(NULL, "DB connect fail@CFieldMapWorkspace::CFieldMapWorkspace()", "ERROR - SpaceCowboy FieldServer", NULL);
	}	

	m_dwLastTickGLogSystem	= 0;	// 2010-06-01 by shcho, GLogDB ���� -

	m_vecTriggerMapBuff.clear();		// 2011-10-10 by hskim, EP4 [Ʈ���� �ý���] - ȭ���� / �� ��ǳ
}

CFieldMapWorkspace::~CFieldMapWorkspace()
{
	m_odbcStmtForLoading.Clean();

	m_vecTriggerMapBuff.clear();		// 2011-10-10 by hskim, EP4 [Ʈ���� �ý���] - ȭ���� / �� ��ǳ
}


//////////////////////////////////////////////////////////////////////
// Method
//////////////////////////////////////////////////////////////////////
CMapProject * CFieldMapWorkspace::CreateChildMapProject(PROJECTINFO * i_pProjectInfo)
{
	return new CFieldMapProject(this, i_pProjectInfo);
}


// 2008-09-10 by cmkwon, ���վƷ��� �ʼ���(map.cfg) ���� ���� - 
//BOOL CFieldMapWorkspace::LoadAllProject(const char* pFileName)
BOOL CFieldMapWorkspace::LoadAllProject(const char* pFileName, BOOL i_bIsArenaServer, vectMapIndexList *i_pvectArenaMapIndexList)
{
	// 2008-09-10 by cmkwon, ���վƷ��� �ʼ���(map.cfg) ���� ���� - 
	//BOOL bReted = CMapWorkspace::LoadAllProject(pFileName);
	BOOL bReted = CMapWorkspace::LoadAllProject(pFileName, i_bIsArenaServer, i_pvectArenaMapIndexList);
	if (FALSE == bReted) {					return FALSE;}

	///////////////////////////////////////////////////////////////////////////
	// IOCP Server�� DefaulMapIndex�� �����Ѵ�
	((CFieldIOCP*)m_pIOCPServer)->m_DefaultMapIndex = m_DefaultMapIndex1;

	///////////////////////////////////////////////////////////////////////////
	// Dead Default CFieldMapProject ����
	for (int k = 0; k < m_vectorPtrMapProject.size(); k++)
	{
		BOOL retTmp = ((CFieldMapProject*)m_vectorPtrMapProject[k])->SetDeadDefaultMap();
		if (!retTmp)
		{
			DBGOUT("ERROR: Cannot SetDeadDefaultMap!!!\r\n");
			return FALSE;
		}
//		DBGOUT("DefaltWarpTargetIndex(%s@%s(%d)): %d\n", ((CFieldMapProject*)m_vectorPtrMapProject[k])->m_strProjectName,
//			((CFieldMapProject*)m_vectorPtrMapProject[k])->m_strFieldIP,
//			((CFieldMapProject*)m_vectorPtrMapProject[k])->m_sFieldListenPort,
//			((CFieldMapProject*)m_vectorPtrMapProject[k])->m_DefaltWarpTargetIndex
//			);
	}

	///////////////////////////////////////////////////////////////////////////
	// WarpEvent�� üũ�Ѵ�.
// check: �ӽ÷� ���� �� ���� ����� �ϴ� �׽�Ʈ�� ���� ����, kelovon, 20040402
//	DBGOUT("\r\n\r\n!!!!! ���� ���� Ȯ�� ��ƾ �������� �ʰ� ���� !!!!!\r\n\r\n\r\n");
	char	szError[1024];
	sprintf(szError, "\r\n\r\n  CFieldMapWorkspace::CheckWarpInfo() =========================================================> Start\r\n\
		    Total map count: %d\r\n\r\n", m_vectorPtrMapProject.size());
	g_pFieldGlobal->WriteSystemLog(szError);
	DbgOut(szError);

	if (CheckWarpInfo() == FALSE)
	{
// 2005-09-28 by cmkwon, DBGOUT���� ��ü��
//		if(FALSE == g_pFieldGlobal->InMasangServer())
//		{
//			MessageBox(NULL, szError, "ERROR", MB_OK);
//			return FALSE;
//		}
	}
	sprintf(szError, "\r\n\r\n  CFieldMapWorkspace::CheckWarpInfo() =========================================================> End\r\n\r\n");
	g_pFieldGlobal->WriteSystemLog(szError);
	DbgOut(szError);

	return this->CheckWarpObjectIndexInfo();			// 2006-12-08 by cmkwon,
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-07 by cmkwon, ����2�� �߰� ���� - 
/// \author		cmkwon
/// \date		2010-04-07 ~ 2010-04-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CFieldMapChannel *CFieldMapWorkspace::GetFieldMapChannel(MAP_CHANNEL_INDEX i_mapChannIdx)
{
	CMapProject *pMapPro = GetMapProjectByMapIndex(i_mapChannIdx.MapIndex);
	if(NULL == pMapPro)
	{
		return NULL;
	}

	return (CFieldMapChannel*)(pMapPro->GetMapChannelByIndex(i_mapChannIdx.ChannelIndex));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-07 by cmkwon, ����2�� �߰� ���� - 
/// \author		cmkwon
/// \date		2010-04-07 ~ 2010-04-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CFieldMapChannel *CFieldMapWorkspace::GetFieldMapChannel(MapIndex_t i_mapIdx, ChannelIndex_t i_channelIdx)
{
	CMapProject *pMapPro = GetMapProjectByMapIndex(i_mapIdx);
	if(NULL == pMapPro)
	{
		return NULL;
	}
	
	return (CFieldMapChannel*)(pMapPro->GetMapChannelByIndex(i_channelIdx));
}

// 2010-06-01 by shcho, GLogDB ���� -
INT CFieldMapWorkspace::GetTotalUserCurrentCnt()
{
	CFieldMapChannel *pFMapChann = NULL;
	int			nTotalClientCount = 0;

	for (int i = 0; i < m_vectorPtrMapProject.size(); i++)
	{
		int nChannCounts = m_vectorPtrMapProject[i]->GetNumberOfMapChannels();
		for(int k = 0; k < nChannCounts; k++)
		{
			pFMapChann = (CFieldMapChannel*)m_vectorPtrMapProject[i]->GetMapChannelByIndex(k);
			if (NULL == pFMapChann)
			{
				continue;
			}
			
			nTotalClientCount += pFMapChann->GetNumClients();
		}
	}

	return nTotalClientCount;
}

// start 2011-10-10 by hskim, EP4 [Ʈ���� �ý���] - ȭ���� / �� ��ǳ
BOOL CFieldMapWorkspace::IsTriggerMapBuff(MapIndex_t MapIndex)
{
	for(int i=0; i<m_vecTriggerMapBuff.size(); i++)
	{
		if( m_vecTriggerMapBuff[i].MapIndex == MapIndex)
		{
			return TRUE;
		}
	}

	return FALSE;
}
// end 2011-10-10 by hskim, EP4 [Ʈ���� �ý���] - ȭ���� / �� ��ǳ

void CFieldMapWorkspace::SendLogServerInfo(void)
{
	int			nClientCount;
	int			nMonsterCount;
	int			nTotalClientCount = 0;
	int			nTotalMonsterCount = 0;
	CFieldMapChannel *pFMapChann = NULL;
	int			nMapClientCnt = 0;		// 2010-06-01 by shcho, GLogDB ���� -
	BOOL		GlogUpdate	= FALSE;	// 2010-06-01 by shcho, GLogDB ���� -
	if(0 >= m_dwLastTickGLogSystem)
	{// 2010-06-01 by shcho, GLogDB ���� -
		m_dwLastTickGLogSystem = GetTickCount();
	}
	else if((GetTickCount() - m_dwLastTickGLogSystem) >= TIMERGAP_GLOGSYSTEM)
	{// 2010-06-01 by shcho, GLogDB ���� -
		GlogUpdate = TRUE;
		m_dwLastTickGLogSystem = GetTickCount(); //����ϰ� ���� �ʱ�ȭ
	}

	for (int i = 0; i < m_vectorPtrMapProject.size(); i++)
	{
		int nChannCounts = m_vectorPtrMapProject[i]->GetNumberOfMapChannels();
		nMapClientCnt = 0;			// 2010-06-01 by shcho, GLogDB ���� -
		for(int k = 0; k < nChannCounts; k++)
		{
			pFMapChann = (CFieldMapChannel*)m_vectorPtrMapProject[i]->GetMapChannelByIndex(k);
			if (NULL == pFMapChann)
			{
				continue;
			}

			nClientCount = pFMapChann->GetNumClients();
			pFMapChann->UpdateMaxUserCounts(nClientCount);
			nTotalClientCount += nClientCount;
			nMonsterCount = pFMapChann->GetNumMonsters();
			nTotalMonsterCount += nMonsterCount;
			nMapClientCnt += nClientCount;	// 2010-06-01 by shcho, GLogDB ���� -
			// 2009-06-25 by cmkwon, �ʺ� ������,���ͼ� �α״� �α� ������ �����ϱ� �ȵ��� ���� - 
			// 2012-08-30 by jhseol, Ǯ�α� �����
#ifdef S_FULL_LOG_JHSEOL
			CAtumLogSender::SendLogMessageServerInfoMap(pFMapChann->GetMapChannelIndex(), nClientCount, nMonsterCount);
#endif	//#ifdef S_FULL_LOG_JHSEOL
			// end 2012-08-30 by jhseol, Ǯ�α� �����
		}
 		if(3 <= nMapClientCnt
 			&& TRUE == GlogUpdate)
 		{// 2010-06-01 by shcho, GLogDB ���� -
 			CAtumLogSender::SendLogMessageServerInfoMap(pFMapChann->GetMapChannelIndex(), nMapClientCnt, nMonsterCount);			// 2010-06-01 by shcho, GLogDB ���� -
 		}
	}

	if (((CFieldIOCP*)m_pIOCPServer)->m_uiMaxUserCounts < nTotalClientCount)
	{
		((CFieldIOCP*)m_pIOCPServer)->m_uiMaxUserCounts = nTotalClientCount;
	}

	CAtumLogSender::SendLogMessageServerInfoTotal(nTotalClientCount, nTotalMonsterCount, GlogUpdate);		// 2010-06-01 by shcho, GLogDB ���� - GlogUpdate ���ڰ� �߰� 
}

BOOL CFieldMapWorkspace::CheckWarpInfo(void)
{
	char szSysLog[1024];	util::zero(szSysLog, 1024);		// 2006-12-13 by cmkwon
	BOOL bError = FALSE;										// 2006-12-13 by cmkwon

	BOOL bRet = TRUE;
	int i, k = 0;
	CFieldMapProject *pSourceMap, *pTmpMap = NULL;
	map<int, vector<int>*>::iterator itrArea;

	for (i = 0; i < m_vectorPtrMapProject.size(); i++)
	{

		pSourceMap	= (CFieldMapProject*)m_vectorPtrMapProject[i];
		bError		= FALSE;										// 2006-12-13 by cmkwon
		util::zero(szSysLog, 1024);

		g_pFieldGlobal->WriteSystemLogEX(TRUE, "    Verifying Warp Info.: %04d		m_DefaltWarpTargetIndex: %d \r\n", pSourceMap->m_nMapIndex, pSourceMap->m_DefaltWarpTargetIndex);		
		if (pSourceMap->m_DeadDefaultMapIndex == -1)
		{
			return FALSE;
		}

		///////////////////////////////////////////////////////////////////////
		// WarpAreaMap
		itrArea = pSourceMap->m_WarpAreaMap.begin();
		while (itrArea != pSourceMap->m_WarpAreaMap.end())
		{
			bError		= FALSE;			// 2006-12-13 by cmkwon
			BOOL bTempRetWarpArea = TRUE;
			EVENTINFO e1 = *(pSourceMap->m_vTileInfo[(*(itrArea->second))[0]].m_pEventInfo);

			sprintf(&szSysLog[strlen(szSysLog)], "      Warp Area[%s,%3d��]: %04d[%1s%3d]"
				, (e1.m_bEventType==EVENT_TYPE_WARP?"SOURCE":"TARGET"), itrArea->second->size()
				, pSourceMap->m_nMapIndex, (e1.m_bEventType==EVENT_TYPE_WARP?"S":"T"), e1.m_EventwParam1);

			if (e1.m_bEventType == EVENT_TYPE_WARP)
			{// source�� ���
				sprintf(&szSysLog[strlen(szSysLog)], " -> %4d[T%3d], ERR INDEXES(" , e1.m_EventwParam3, e1.m_EventwParam2);
			}
			else if (e1.m_bEventType == EVENT_TYPE_WARP_TARGET)
			{// target�� ���
				sprintf(&szSysLog[strlen(szSysLog)],"              , ERR INDEXES("); 
			}

			for (k = 1; k < itrArea->second->size(); k++)
			{
				// 0��°�� ������ ����ϹǷ� 1���� ����
				EVENTINFO e2 = *(pSourceMap->m_vTileInfo[(*(itrArea->second))[k]].m_pEventInfo);
				if (e1.m_bEventType != e2.m_bEventType
					|| e1.m_EventwParam1 != e2.m_EventwParam1
					|| e1.m_EventwParam2 != e2.m_EventwParam2
					|| (e1.m_bEventType == EVENT_TYPE_WARP && e1.m_EventwParam3 != e2.m_EventwParam3)
				)
				{
					// ERROR
					sprintf(&szSysLog[strlen(szSysLog)], " %d", (*(itrArea->second))[k]);
// 2006-12-13 by cmkwon, ���� ���� ������
//					DBGOUT(" %d", (*(itrArea->second))[k]);
					bRet = FALSE;
					bTempRetWarpArea = FALSE;
				}
			}
			strcat(szSysLog, ")");
// 2006-12-13 by cmkwon, ���� ���� ������
//			DBGOUT(")");

			if (e1.m_bEventType == EVENT_TYPE_WARP)
			{
				BOOL bValidWARP = TRUE;
				// warp target ã��
				if ((pTmpMap = (CFieldMapProject*)GetMapProjectByMapIndex(e1.m_EventwParam3)) == NULL)
				{
					bValidWARP = FALSE;
					bRet = FALSE;
					bTempRetWarpArea = FALSE;
				}
				else
				{
					if (pTmpMap->GetEventInfoByWarpIndex(e1.m_EventwParam2) == NULL)
					{
						// error
						bValidWARP = FALSE;
						bRet = FALSE;
						bTempRetWarpArea = FALSE;
					}
				}

				if (bTempRetWarpArea)
				{
					strcat(szSysLog, " -> GOOD");
// 2006-12-13 by cmkwon, ���� ���� ������
//					DBGOUT(" -> GOOD");
				}
				else
				{
					bError	= TRUE;		// 2006-12-13 by cmkwon
					sprintf(&szSysLog[strlen(szSysLog)], " -> ERROR: no such target(%d, %d)", e1.m_EventwParam3, e1.m_EventwParam2);
// 2006-12-13 by cmkwon, ���� ���� ������
//					DBGOUT(" -> ERROR: no such target(%d, %d)", e1.m_EventwParam3, e1.m_EventwParam2);
				}
			}
			else if (e1.m_bEventType == EVENT_TYPE_WARP_TARGET)
			{
				// nothing to verify
			}

			strcat(szSysLog, "\r\n");
// 2006-12-13 by cmkwon, ���� ���� ������
//			DBGOUT("\n");

			///////////////////////////////////////////////////////////////////////////////		
			// 2006-12-13 by cmkwon
			g_pFieldGlobal->WriteSystemLog(szSysLog);
			if(bError)
			{
				DbgOut(szSysLog);
			}
			util::zero(szSysLog, 1024);

			itrArea++;
		}

		for(int j=0; j < pSourceMap->m_vectorObjectInfo.size(); j++)
		{
			bError		= FALSE;		// 2006-12-13 by cmkwon

			BOOL bTempRetWarpObject = TRUE;
			EVENTINFO e1 = pSourceMap->m_vectorObjectInfo[j].m_EventInfo;

			switch(e1.m_bEventType)
			{
			case EVENT_TYPE_WARP:
				{
					sprintf(&szSysLog[strlen(szSysLog)], "      Warp Obj[SOURCE  SourceMap(%4d, %4d) TargetMap(%4d, %4d)]"
						, pSourceMap->m_nMapIndex, e1.m_EventwParam1
						, e1.m_EventwParam3, e1.m_EventwParam2);
// 2006-12-13 by cmkwon, ���� ���� ������
//					DBGOUT("  Warp Obj[SOURCE  SourceMap(%4d, %4d) TargetMap(%4d, %4d)]"
//						, pSourceMap->m_nMapIndex, e1.m_EventwParam1
//						, e1.m_EventwParam3, e1.m_EventwParam2);
				}
				break;
			case EVENT_TYPE_WARP_TARGET:
				{
					sprintf(&szSysLog[strlen(szSysLog)], "      Warp Obj[TARGET			 TargetMap(%4d, %4d)]"
						, pSourceMap->m_nMapIndex, e1.m_EventwParam1);
// 2006-12-13 by cmkwon, ���� ���� ������
//					DBGOUT("  Warp Obj[TARGET			 TargetMap(%4d, %4d)]"
//						, pSourceMap->m_nMapIndex, e1.m_EventwParam1);
				}
				break;
			case EVENT_TYPE_PATTERN_POINT:
				{
					sprintf(&szSysLog[strlen(szSysLog)], "\t\t\t    Pattern Obj[Start(%4d) --> Next(%4d)]"
						, e1.m_EventwParam1, e1.m_NextEventIndex);
// 2006-12-13 by cmkwon, ���� ���� ������
//					DBGOUT("  \t\t\tPattern Obj[Start(%4d) --> Next(%4d)]"
//						, e1.m_EventwParam1, e1.m_NextEventIndex);
				}
				break;
			case EVENT_TYPE_OBJ_QUEST_OBJECT:
				{
					sprintf(&szSysLog[strlen(szSysLog)], "\t\t\t    Quest   Obj[    ObjectEventIndex(%4d)]"
						, e1.m_EventwParam1);
// 2006-12-13 by cmkwon, ���� ���� ������
//					DBGOUT("  \t\t\tQuest   Obj[    ObjectEventIndex(%4d)]"
//						, e1.m_EventwParam1);
				}
				break;
			case EVENT_TYPE_LANDING_PATTERN_START:
			case EVENT_TYPE_LANDING_PATTERN_END:
			case EVENT_TYPE_OBJ_ENTER_BUILDING:
			case EVENT_TYPE_TAKEOFF_PATTERN_START:
				{
					sprintf(&szSysLog[strlen(szSysLog)], "      ETC  Obj[%32s  EvIndex(%4d) NextEvIndex(%4d)]"
						, Get_EVENT_TYPE_String(e1.m_bEventType)
						, e1.m_EventwParam1, e1.m_NextEventIndex);
// 2006-12-13 by cmkwon, ���� ���� ������
//					DBGOUT("  ETC  Obj[%32s  EvIndex(%4d) NextEvIndex(%4d)]"
//						, Get_EVENT_TYPE_String(e1.m_bEventType)
//						, e1.m_EventwParam1, e1.m_NextEventIndex);
				}
				break;
			default:
				{
					continue;
				}
			}			

// 2010-04-14 by cmkwon, ���� �޸� ���� ���� ���� - �ʿ� ��� ���ŵ�
//			sprintf(&szSysLog[strlen(szSysLog)]," ObjIndex(%2d), Pos(% 6.0f, % 6.0f, % 6.0f)", e1.m_TileOrObjectIndex, e1.m_vPosition.x, e1.m_vPosition.y, e1.m_vPosition.z); 
// 2006-12-13 by cmkwon, ���� ���� ������
//			DBGOUT(" ObjIndex(%2d), Pos(% 5.0f, % 5.0f, % 5.0f)"
//				, e1.m_TileOrObjectIndex, e1.m_vPosition.x, e1.m_vPosition.y, e1.m_vPosition.z);
			///////////////////////////////////////////////////////////////////////////////
			// 2010-04-14 by cmkwon, ���� �޸� ���� ���� ���� - �ʿ� ��� ���ŵ�
			sprintf(&szSysLog[strlen(szSysLog)]," ObjIndex, Pos(% 6.0f, % 6.0f, % 6.0f)", e1.m_vPosition.x, e1.m_vPosition.y, e1.m_vPosition.z); 

			switch(e1.m_bEventType)
			{
			case EVENT_TYPE_WARP:
				{
					BOOL bValidWARP = TRUE;
					// warp target ã��
					if ((pTmpMap = (CFieldMapProject*)GetMapProjectByMapIndex(e1.m_EventwParam3)) == NULL)
					{
						bValidWARP			= FALSE;
						bRet				= FALSE;
						bTempRetWarpObject	= FALSE;
					}
					else
					{					
						if (pTmpMap->GetEventInfoByWarpIndex(e1.m_EventwParam2) == NULL)
						{
							// error
							bValidWARP = FALSE;
							bRet = FALSE;
							bTempRetWarpObject = FALSE;
						}
					}

					if (bTempRetWarpObject)
					{
						strcat(szSysLog, " ==> GOOD");
// 2006-12-13 by cmkwon, ���� ���� ������
//						DBGOUT(" ==> GOOD");
					}
					else
					{
						bError	= TRUE;		// 2006-12-13 by cmkwon
						strcat(szSysLog, " ==> ERROR: Warp Target Event Index error");
// 2006-12-13 by cmkwon, ���� ���� ������
//						DBGOUT(" ==> ERROR: Warp Target Event Index error");
					}
				}
				break;
			case EVENT_TYPE_WARP_TARGET:
			case EVENT_TYPE_OBJ_QUEST_OBJECT:
				{
					// nothing
				}
				break;
			default:
				{
					if(0 != e1.m_NextEventIndex)
					{
						if(pSourceMap->GetEventInfoByEventAreaIndex(e1.m_NextEventIndex))
						{
							strcat(szSysLog, " ==> GOOD");
// 2006-12-13 by cmkwon, ���� ���� ������
//							DBGOUT(" ==> GOOD");
						}
						else
						{
							bError	= TRUE;		// 2006-12-13 by cmkwon
							strcat(szSysLog, " ==> ERROR: Next Event Area Index error");
// 2006-12-13 by cmkwon, ���� ���� ������
//							DBGOUT(" ==> ERROR: Next Event Area Index error");
						}
					}
				}
			}// end_switch(e1.m_bEventType)
			
			strcat(szSysLog, "\r\n");
// 2006-12-13 by cmkwon, ���� ���� ������
//			DBGOUT("\n");

			///////////////////////////////////////////////////////////////////////////////		
			// 2006-12-13 by cmkwon
			g_pFieldGlobal->WriteSystemLog(szSysLog);
			if(bError)
			{
				DbgOut(szSysLog);
			}
			util::zero(szSysLog, 1024);


		}// end_for(int j; j < pSourceMap->m_vectorObjectInfo.size(); j++)

		strcat(szSysLog, "\r\n");
// 2006-12-13 by cmkwon, ���� ���� ������
//		DBGOUT("\n");

		///////////////////////////////////////////////////////////////////////////////		
		// 2006-12-13 by cmkwon
		g_pFieldGlobal->WriteSystemLog(szSysLog);
		if(bError)
		{
			DbgOut(szSysLog);
		}
		util::zero(szSysLog, 1024);

	}// end_for (i = 0; i < m_vectorPtrMapProject.size(); i++)

	return bRet;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldMapWorkspace::CheckWarpObjectIndexInfo(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-12-08 ~ 2006-12-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldMapWorkspace::CheckWarpObjectIndexInfo(void)
{
	BOOL bRet = TRUE;
	int i, k = 0;
	CFieldMapProject *pSourceMap, *pTmpMap = NULL;
	for (i = 0; i < m_vectorPtrMapProject.size(); i++)
	{
		pSourceMap = (CFieldMapProject*)m_vectorPtrMapProject[i];

		if(pSourceMap->m_VCNWarpObjectIndex)
		{
			OBJECTINFOSERVER *pObjInfoSvr = pSourceMap->GetRandomObjectInfoServerPtrByEventIndex(pSourceMap->m_VCNWarpObjectIndex);
			if(NULL == pObjInfoSvr
				|| (EVENT_TYPE_WARP_TARGET != pObjInfoSvr->m_EventInfo.m_bEventType && EVENT_TYPE_WARP != pObjInfoSvr->m_EventInfo.m_bEventType))
			{
				char szSysLog[1024];
				sprintf(szSysLog, "CFieldMapWorkspace::CheckWarpObjectIndexInfo_ VCNWarpObjectIndex Error, Map(%d) WapObjectIndex(%d)\r\n"
					, pSourceMap->m_nMapIndex, pSourceMap->m_VCNWarpObjectIndex);
				g_pGlobal->WriteSystemLog(szSysLog);
				DbgOut(szSysLog);
//				return FALSE;
			}
		}
		if(pSourceMap->m_ANIWarpObjectIndex)
		{
			OBJECTINFOSERVER *pObjInfoSvr = pSourceMap->GetRandomObjectInfoServerPtrByEventIndex(pSourceMap->m_ANIWarpObjectIndex);
			if(NULL == pObjInfoSvr
				|| (EVENT_TYPE_WARP_TARGET != pObjInfoSvr->m_EventInfo.m_bEventType && EVENT_TYPE_WARP != pObjInfoSvr->m_EventInfo.m_bEventType))
			{
				char szSysLog[1024];
				sprintf(szSysLog, "CFieldMapWorkspace::CheckWarpObjectIndexInfo_ ANIWarpObjectIndex Error, Map(%d) WapObjectIndex(%d)\r\n"
					, pSourceMap->m_nMapIndex, pSourceMap->m_ANIWarpObjectIndex);
				g_pGlobal->WriteSystemLog(szSysLog);
				DbgOut(szSysLog);
//				return FALSE;
			}
		}
	}// end_for (i = 0; i < m_vectorPtrMapProject.size(); i++)

	return TRUE;
}


// �� Bandwidth ���
void CFieldMapWorkspace::CalcAllMapTotalTraffic(void)
{
	if(FALSE == GetLogFlagMapBandwidth())
	{// 2008-03-13 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - FALSE �̸� �ٷ� �����Ѵ�.
		return;
	}

	if(m_vectorPtrMapProject.empty()){		return;}

	g_pGlobal->WriteSystemLogEX(TRUE, "Map Traffic\r\n");
	CFieldMapChannel * pFMapChann = NULL;
	for (int i = 0; i < m_vectorPtrMapProject.size(); i++)
	{
		CFieldMapProject *pFMapPro = (CFieldMapProject*)(this->GetMapProjectByIndex(i));
		if(NULL == pFMapPro)
		{
			continue;
		}

		int nChannCounts = pFMapPro->GetNumberOfMapChannels();
		for(int k=0; k < nChannCounts; k++)
		{
			CFieldMapChannel *pFMapChann = pFMapPro->GetFieldMapChannelByIndex(k);
			if(pFMapChann)
			{
				pFMapChann->CalcMapTotalTraffic();
			}
		}
	}
}


void CFieldMapWorkspace::OnDoMinutelyWorkFieldMapWorkspace(void)
{
	if(m_vectorPtrMapProject.empty()){			return;}

	for (int i = 0; i < m_vectorPtrMapProject.size(); i++)
	{		
		((CFieldMapProject*)m_vectorPtrMapProject[i])->OnDoMinutelyWorkFieldMapProject();
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldMapWorkspace::OnDoSecondlyWorkFieldMapWorkspace(ATUM_DATE_TIME *pDateTime)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-25 ~ 2006-07-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldMapWorkspace::OnDoSecondlyWorkFieldMapWorkspace(ATUM_DATE_TIME *pDateTime)
{
	if(m_vectorPtrMapProject.empty()){			return;}

	for (int i = 0; i < m_vectorPtrMapProject.size(); i++)
	{		
		((CFieldMapProject*)m_vectorPtrMapProject[i])->OnDoSecondlyWorkFieldMapProject(pDateTime);
	}
}

void CFieldMapWorkspace::SetCityWarFieldMapWorkspace(CCityWar *i_pCityWar)
{
	if(m_vectorPtrMapProject.empty()){			return;}

	for (int i = 0; i < m_vectorPtrMapProject.size(); i++)
	{		
		((CFieldMapProject*)m_vectorPtrMapProject[i])->SetCityWarFieldMapProject(i_pCityWar);
	}
}

// 2009-10-12 by cmkwon, ������ī ���� ��� ���� - �����߰�(MapIndex_t i_nStartCityMapIdx)
CFieldMapProject * CFieldMapWorkspace::GetCityFieldMapProjectPtrByInfluence(BYTE i_byInfluence, MapIndex_t i_nStartCityMapIdx/*=VCN_CITY_MAP_INDEX*/)
{
	CFieldMapProject *pRetFMPro = (CFieldMapProject*)this->GetMapProjectByMapIndex(CAtumSJ::GetCityMapIndexByInfluence(i_byInfluence, i_nStartCityMapIdx));
	if(pRetFMPro)
	{
		return pRetFMPro;
	}
	
	return (CFieldMapProject*)this->GetMapProjectByMapIndex(this->m_DefaultMapIndex1);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CFieldMapChannel* CFieldMapWorkspace::GetCityFieldMapChannelPtrByInfluence(BYTE i_byInfluence)
/// \brief		
///				// 2009-10-12 by cmkwon, ������ī ���� ��� ���� - �����߰�(MapIndex_t i_nStartCityMapIdx)
/// \author		cmkwon
/// \date		2006-04-19 ~ 2006-04-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CFieldMapChannel* CFieldMapWorkspace::GetCityFieldMapChannelPtrByInfluence(BYTE i_byInfluence, MapIndex_t i_nStartCityMapIdx/*=VCN_CITY_MAP_INDEX*/)
{
	return (CFieldMapChannel*)(this->GetCityFieldMapProjectPtrByInfluence(i_byInfluence, i_nStartCityMapIdx)->GetFirstMapChannel(FALSE));
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldMapWorkspace::InitStrategyPointOccupy(BOOL i_bVCNMapInflTyForInit)
/// \brief		
/// \author		cmkwon
/// \date		2006-11-21 ~ 2006-11-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldMapWorkspace::InitStrategyPointOccupy(BOOL i_bVCNMapInflTyForInit)
{
	for (int i = 0; i < m_vectorPtrMapProject.size(); i++)
	{
		CFieldMapProject *pFMapPro = (CFieldMapProject *)(m_vectorPtrMapProject[i]);
		if(i_bVCNMapInflTyForInit)
		{
			if(IS_MAP_INFLUENCE_VCN(pFMapPro->GetMapInfluenceType()))
			{
				(pFMapPro->GetFieldMapChannelByIndex(0))->SetLastTickStrategyPointMonsterDeaded(0);
			}
		}
		else
		{
			if(IS_MAP_INFLUENCE_ANI(pFMapPro->GetMapInfluenceType()))
			{
				(pFMapPro->GetFieldMapChannelByIndex(0))->SetLastTickStrategyPointMonsterDeaded(0);
			}
		}
	}
}