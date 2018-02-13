// INFCityMission.cpp: implementation of the CINFCityMission class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFCityMission.h"
#include "AtumApplication.h"
#include "QuestData.h"
#include "INFImage.h"
#include "D3DHanFont.h"
#include "GameDataLast.h"
#include "AtumDatabase.h"
#include "INFGameMain.h"
#include "INFCityBase.h"
#include "INFWindow.h"
#include "INFScrollBar.h"
#include "dxutil.h"
#include "INFImageEx.h"	                    // 2011. 10. 10 by jskim UI�ý��� ����

#define CITY_MISSION_START_X				CITY_BASE_NPC_BOX_START_X//194
#define CITY_MISSION_START_Y				(CITY_BASE_NPC_BOX_START_Y-CITY_BASE_NPC_MISSION_SIZE_Y)//xx-275
#define CITY_MISSION_BACK_START_X			(CITY_MISSION_START_X+12)
#define CITY_MISSION_BACK_START_Y			(CITY_MISSION_START_Y+29)
#define CITY_MISSION_TITLE_START_X			(CITY_MISSION_START_X+10)
#define CITY_MISSION_TITLE_START_Y			(CITY_MISSION_START_Y+6)

#define CITY_MISSION_BUTTON_JOIN_START_X	(CITY_MISSION_START_X+340)
#define CITY_MISSION_BUTTON_JOIN_START_Y	(CITY_MISSION_START_Y+268)
#define CITY_MISSION_BUTTON_JOIN_SIZE_X		64
#define CITY_MISSION_BUTTON_JOIN_SIZE_Y		17
#define CITY_MISSION_BUTTON_CANCEL_START_X	(CITY_MISSION_START_X+406)
#define CITY_MISSION_BUTTON_CANCEL_START_Y	(CITY_MISSION_START_Y+268)
#define CITY_MISSION_BUTTON_CANCEL_SIZE_X	38
#define CITY_MISSION_BUTTON_CANCEL_SIZE_Y	17

#define CITY_MISSION_LIST_START_X			(CITY_MISSION_START_X+32)
#define CITY_MISSION_LIST_START_Y			(CITY_MISSION_START_Y+68)
#define CITY_MISSION_LIST_SIZE_X			171
#define CITY_MISSION_LIST_SIZE_Y			188
#define CITY_MISSION_LINE_INTERVAL			17
#define CITY_MISSION_LIST_SCROLL_START_X	(CITY_MISSION_START_X+210)
#define CITY_MISSION_LIST_SCROLL_START_Y	(CITY_MISSION_START_Y+67)
#define CITY_MISSION_LIST_SCROLL_SIZE_Y		188// scroll line length(long)

//#define CITY_MISSION_LIST_SCROLL_BAR_SIZE_X	11// scroll bar size(short)
//#define CITY_MISSION_LIST_SCROLL_BAR_SIZE_Y	30
//#define CITY_MISSION_LIST_SCROLL_BAR_LENGTH	(CITY_MISSION_LIST_SCROLL_SIZE_Y-CITY_MISSION_LIST_SCROLL_BAR_SIZE_Y)
//#define CITY_MISSION_LIST_SCROLL_BAR_INTERVAL(count)	(count==0 ? 0:(CITY_MISSION_LIST_SCROLL_BAR_LENGTH/count))

#define CITY_MISSION_DESC_START_X			(CITY_MISSION_START_X+234)
#define CITY_MISSION_DESC_START_Y			(CITY_MISSION_START_Y+76)
#define CITY_MISSION_DESC_INTERVAL			16
#define CITY_MISSION_DESC_SIZE_X			196
#define CITY_MISSION_DESC_SIZE_Y			187
#define CITY_MISSION_DESC_SCROLL_START_X	(CITY_MISSION_START_X+430)
#define CITY_MISSION_DESC_SCROLL_START_Y	(CITY_MISSION_START_Y+67)
#define CITY_MISSION_DESC_SCROLL_SIZE_Y		188// scroll line length(long)

//#define CITY_MISSION_DESC_SCROLL_BAR_SIZE_X	11// scroll bar size(short)
//#define CITY_MISSION_DESC_SCROLL_BAR_SIZE_Y	30
//#define CITY_MISSION_DESC_SCROLL_BAR_LENGTH	(CITY_MISSION_DESC_SCROLL_SIZE_Y-CITY_MISSION_DESC_SCROLL_BAR_SIZE_Y)
//#define CITY_MISSION_DESC_SCROLL_BAR_INTERVAL(count)	(count==0 ? 0:(CITY_MISSION_LIST_SCROLL_BAR_LENGTH/count))

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define NONE_SELECT_MISSION					-1

CINFCityMission::CINFCityMission(CAtumNode* pParent)
{
	m_pParent = pParent;
	m_bRestored = FALSE;
	int i;
	for(i=0;i<CITY_MISSION_BUTTON_NUMBER;i++)
	{
		m_pJoinButton[i] = NULL;
		m_pCompleteButton[i] = NULL;
		m_pCancelButton[i] = NULL;
	}

	m_pImgBack = NULL;
	m_pImgTitle = NULL;
//	m_pImgScroll = NULL;
	m_pImgHighLight = NULL;

	for(i=0;i<CITY_MISSION_LINE_NUMBER;i++)
	{
		m_pFontQuestList[i] = NULL;
		m_pFontQuestDesc[i] = NULL;
	}
	m_pListScroll = NULL;
	m_pDescScroll = NULL;
	m_nCancerSelect = 0;
	Reset();
}

CINFCityMission::~CINFCityMission()
{
	int i;
	for(i=0;i<CITY_MISSION_BUTTON_NUMBER;i++)
	{
		util::del(m_pJoinButton[i]);
		util::del(m_pCompleteButton[i]);
		util::del(m_pCancelButton[i]);
	}

	util::del(m_pImgBack);
	util::del(m_pImgTitle);
//	util::del(m_pImgScroll);
	util::del(m_pImgHighLight);

	for(i=0;i<CITY_MISSION_LINE_NUMBER;i++)
	{
		util::del(m_pFontQuestList[i]);
		util::del(m_pFontQuestDesc[i]);
	}
	util::del(m_pListScroll);
}

void CINFCityMission::Reset()
{
	m_nButtonState[CITY_MISSION_BUTTON_JOIN] = BUTTON_STATE_NORMAL;
	m_nButtonState[CITY_MISSION_BUTTON_CANCEL] = BUTTON_STATE_NORMAL;

	memset(m_strQuestList, 0x00, CITY_MISSION_MAX_NUMBER*CITY_MISSION_LINE_LENGTH);
	memset(m_strQuestDesc, 0x00, CITY_MISSION_MAX_NUMBER*CITY_MISSION_LINE_LENGTH);
	m_nCurrentSelectQuestList = NONE_SELECT_MISSION;
//	m_nCurrentQuestListScroll = 0;
//	m_bQuestListLock = FALSE;
//	m_nCurrentQuestDescScroll = 0;
//	m_bQuestDescLock = FALSE;

	m_nQuestDescLineNumber = 0;

	m_mapCityMission.clear();
	if(m_pListScroll)
	{
		m_pListScroll->Reset();
	}
	if(m_pDescScroll)
	{
		m_pDescScroll->Reset();
	}
}

void CINFCityMission::AddMission(CQuest* pQuest)
{
	if(g_pQuestData->IsQuestCompleted(pQuest->QuestIndex))
	{
		return;
	}
	int index = m_mapCityMission.size();
	m_mapCityMission[index] = pQuest;
	strcpy(m_strQuestList[index], pQuest->QuestName);
	if(g_pQuestData->IsQuestProgress(pQuest->QuestIndex))
	{
		m_bProgressMission[index] = TRUE;
		//strcat(m_strQuestList[index], " (������)");
	}
	m_pListScroll->SetNumberOfData( m_mapCityMission.size() );
}

HRESULT CINFCityMission::InitDeviceObjects()
{
	DataHeader	* pDataHeader ;
	char buf[32];
	int i;
	for(i=0;i<CITY_MISSION_BUTTON_NUMBER;i++)
	{
		m_pJoinButton[i] = new CINFImageEx;
		wsprintf( buf, "shmjoi0%d", i);
		pDataHeader = FindResource(buf);
		m_pJoinButton[i]->InitDeviceObjects(pDataHeader ) ;
		m_pCompleteButton[i] = new CINFImageEx;
		wsprintf( buf, "shmend0%d", i);
		pDataHeader = FindResource(buf);
		m_pCompleteButton[i]->InitDeviceObjects(pDataHeader ) ;
		m_pCancelButton[i] = new CINFImageEx;
		wsprintf( buf, "shmcan0%d", i);
		pDataHeader = FindResource(buf);
		m_pCancelButton[i]->InitDeviceObjects(pDataHeader ) ;
	}
	m_pImgBack = new CINFImageEx;
	pDataHeader = FindResource("shmback");
	m_pImgBack->InitDeviceObjects(pDataHeader ) ;
	m_pImgTitle = new CINFImageEx;
	pDataHeader = FindResource("shmtitle");
	m_pImgTitle->InitDeviceObjects(pDataHeader ) ;
//	m_pImgScroll = new CINFImage;
//	pDataHeader = FindResource("i_scrbar");
//	m_pImgScroll->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize) ;
	m_pImgHighLight = new CINFImageEx;
	pDataHeader = FindResource("shmline");
	m_pImgHighLight->InitDeviceObjects(pDataHeader ) ;

	for(i=0;i<CITY_MISSION_LINE_NUMBER;i++)
	{
		m_pFontQuestList[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
		m_pFontQuestList[i]->InitDeviceObjects(g_pD3dDev) ;
		m_pFontQuestDesc[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
		m_pFontQuestDesc[i]->InitDeviceObjects(g_pD3dDev) ;		
	}
	for(i=0;i<CITY_MISSION_MAX_NUMBER;i++)
	{
		m_bProgressMission[i] = FALSE; 
	}
	m_pListScroll = new CINFScrollBar(this,
								CITY_MISSION_LIST_SCROLL_START_X, 
								CITY_MISSION_LIST_SCROLL_START_Y, 
								CITY_MISSION_LIST_SCROLL_SIZE_Y,
								CITY_MISSION_LINE_NUMBER);
	m_pListScroll->SetGameData( m_pGameData );
	m_pListScroll->InitDeviceObjects();
	m_pListScroll->SetWheelRect(CITY_MISSION_LIST_START_X, 
		CITY_MISSION_LIST_START_Y,
		CITY_MISSION_LIST_START_X+CITY_MISSION_LIST_SIZE_X,
		CITY_MISSION_LIST_START_Y+CITY_MISSION_LIST_SIZE_Y);
	m_pDescScroll = new CINFScrollBar(this,
								CITY_MISSION_DESC_SCROLL_START_X, 
								CITY_MISSION_DESC_SCROLL_START_Y, 
								CITY_MISSION_DESC_SCROLL_SIZE_Y,
								CITY_MISSION_LINE_NUMBER);
	m_pDescScroll->SetGameData( m_pGameData );
	m_pDescScroll->InitDeviceObjects();
	m_pDescScroll->SetWheelRect(CITY_MISSION_DESC_START_X, 
		CITY_MISSION_DESC_START_Y,
		CITY_MISSION_DESC_START_X+CITY_MISSION_DESC_SIZE_X,
		CITY_MISSION_DESC_START_Y+CITY_MISSION_DESC_SIZE_Y);
	return S_OK;
}

HRESULT CINFCityMission::RestoreDeviceObjects()
{
	if(!m_bRestored)
	{
		int i;
		for(i=0;i<CITY_MISSION_BUTTON_NUMBER;i++)
		{
			m_pJoinButton[i]->RestoreDeviceObjects();
			m_pCompleteButton[i]->RestoreDeviceObjects();
			m_pCancelButton[i]->RestoreDeviceObjects();
		}

		m_pImgBack->RestoreDeviceObjects();
		m_pImgTitle->RestoreDeviceObjects();
//		m_pImgScroll->RestoreDeviceObjects();
		m_pImgHighLight->RestoreDeviceObjects();

		for(i=0;i<CITY_MISSION_LINE_NUMBER;i++)
		{
			m_pFontQuestList[i]->RestoreDeviceObjects();
			m_pFontQuestDesc[i]->RestoreDeviceObjects();
		}
		m_pListScroll->RestoreDeviceObjects();
		m_pDescScroll->RestoreDeviceObjects();
		m_bRestored = TRUE;
	}
	return S_OK;
}

HRESULT CINFCityMission::InvalidateDeviceObjects()
{
	if(m_bRestored)
	{
		int i;
		for(i=0;i<CITY_MISSION_BUTTON_NUMBER;i++)
		{
			m_pJoinButton[i]->InvalidateDeviceObjects();
			m_pCompleteButton[i]->InvalidateDeviceObjects();
			m_pCancelButton[i]->InvalidateDeviceObjects();
		}

		m_pImgBack->InvalidateDeviceObjects();
		m_pImgTitle->InvalidateDeviceObjects();
//		m_pImgScroll->InvalidateDeviceObjects();
		m_pImgHighLight->InvalidateDeviceObjects();

		for(i=0;i<CITY_MISSION_LINE_NUMBER;i++)
		{
			m_pFontQuestList[i]->InvalidateDeviceObjects();
			m_pFontQuestDesc[i]->InvalidateDeviceObjects();
		}
		m_pListScroll->InvalidateDeviceObjects();
		m_pDescScroll->InvalidateDeviceObjects();
		m_bRestored = FALSE;
	}
	return S_OK;
}

HRESULT CINFCityMission::DeleteDeviceObjects()
{
	int i;
	for(i=0;i<CITY_MISSION_BUTTON_NUMBER;i++)
	{
		m_pJoinButton[i]->DeleteDeviceObjects();
		m_pCompleteButton[i]->DeleteDeviceObjects();
		m_pCancelButton[i]->DeleteDeviceObjects();
		util::del(m_pJoinButton[i]);
		util::del(m_pCompleteButton[i]);
		util::del(m_pCancelButton[i]);
	}

	m_pImgBack->DeleteDeviceObjects();
	m_pImgTitle->DeleteDeviceObjects();
//	m_pImgScroll->DeleteDeviceObjects();
	m_pImgHighLight->DeleteDeviceObjects();
	util::del(m_pImgBack);
	util::del(m_pImgTitle);
//	util::del(m_pImgScroll);
	util::del(m_pImgHighLight);

	for(i=0;i<CITY_MISSION_LINE_NUMBER;i++)
	{
		m_pFontQuestList[i]->DeleteDeviceObjects();
		m_pFontQuestDesc[i]->DeleteDeviceObjects();
		util::del(m_pFontQuestList[i]);
		util::del(m_pFontQuestDesc[i]);
	}
	m_pListScroll->DeleteDeviceObjects();
	util::del(m_pListScroll);
	m_pDescScroll->DeleteDeviceObjects();
	util::del(m_pDescScroll);
	return S_OK;
}



void CINFCityMission::Render()
{
	m_pImgTitle->Move(CITY_MISSION_TITLE_START_X, CITY_MISSION_TITLE_START_Y);
	m_pImgTitle->Render();
	m_pImgBack->Move(CITY_MISSION_BACK_START_X, CITY_MISSION_BACK_START_Y);
	m_pImgBack->Render();
//	m_pJoinButton[m_nButtonState[CITY_MISSION_BUTTON_JOIN]]->Move(CITY_MISSION_BUTTON_JOIN_START_X, CITY_MISSION_BUTTON_JOIN_START_Y);
//	m_pJoinButton[m_nButtonState[CITY_MISSION_BUTTON_JOIN]]->Render();
	m_pCancelButton[m_nButtonState[CITY_MISSION_BUTTON_CANCEL]]->Move(CITY_MISSION_BUTTON_CANCEL_START_X, CITY_MISSION_BUTTON_CANCEL_START_Y);
	m_pCancelButton[m_nButtonState[CITY_MISSION_BUTTON_CANCEL]]->Render();

//	int nCurrentIndex = m_nCurrentSelectQuestList-m_nCurrentQuestListScroll;
//	if( nCurrentIndex != NONE_SELECT_MISSION)
//	{
//		m_pImgHighLight->Move(CITY_MISSION_LIST_START_X+1, CITY_MISSION_LIST_START_Y+nCurrentIndex*CITY_MISSION_LINE_INTERVAL+1);
//		m_pImgHighLight->Render();
//	}
	int nCurrentIndex = m_pListScroll->GetCurrentSelectWindowIndex();
	if( nCurrentIndex >= 0 &&
		nCurrentIndex < CITY_MISSION_LINE_NUMBER)
	{
		m_pImgHighLight->Move(CITY_MISSION_LIST_START_X+1, CITY_MISSION_LIST_START_Y+nCurrentIndex*CITY_MISSION_LINE_INTERVAL+1);
		m_pImgHighLight->Render();
	}
//	for(int i=0;i<CITY_MISSION_LINE_NUMBER;i++)
//	{
//		if(m_strQuestList[i+m_nCurrentQuestListScroll][0] == NULL)
//		{
//			break;
//		}
//		m_pFontQuestList[i]->DrawText(CITY_MISSION_LIST_START_X, 
//			CITY_MISSION_LIST_START_Y+CITY_MISSION_LINE_INTERVAL*i,
//			nCurrentIndex == i ? GUI_SELECT_FONT_COLOR : GUI_FONT_COLOR,
//			m_strQuestList[i+m_nCurrentQuestListScroll],0L);
//	}
	BOOL bJoinButton = TRUE;
	int i;
	for(i=0;i<CITY_MISSION_LINE_NUMBER;i++)
	{
		if(m_strQuestList[i+m_pListScroll->GetCurrentScrollIndex()][0] == NULL)
		{
			break;
		}
		
		if(m_bProgressMission[i+m_pListScroll->GetCurrentScrollIndex()])
		{
			m_pFontQuestList[i]->DrawText(CITY_MISSION_LIST_START_X, 
				CITY_MISSION_LIST_START_Y+CITY_MISSION_LINE_INTERVAL*i+1,
				nCurrentIndex == i ? GUI_SELECT_FONT_COLOR : GUI_FONT_COLOR_Y,
				m_strQuestList[i+m_pListScroll->GetCurrentScrollIndex()],0L);
		}
		else
		{
			m_pFontQuestList[i]->DrawText(CITY_MISSION_LIST_START_X, 
				CITY_MISSION_LIST_START_Y+CITY_MISSION_LINE_INTERVAL*i+1,
				nCurrentIndex == i ? GUI_SELECT_FONT_COLOR : RGB(128,128,255),
				m_strQuestList[i+m_pListScroll->GetCurrentScrollIndex()],0L);			
		}

		m_nCurrentSelectQuestList = m_pListScroll->GetCurrentSelectDataIndex();
		CMapQuestIterator it = m_mapCityMission.find(m_nCurrentSelectQuestList);
		if(it != m_mapCityMission.end())
		{
			GUI_BUILDINGNPC* pBuilding = ((CINFCityBase*)m_pParent)->GetCurrentBuildingNPC();
			if(pBuilding)
			{
				if(g_pQuestData->CheckEndQuestCondition( it->second, pBuilding->buildingInfo.NPCIndex))
				{
					m_pCompleteButton[m_nButtonState[CITY_MISSION_BUTTON_JOIN]]->Move(CITY_MISSION_BUTTON_JOIN_START_X, CITY_MISSION_BUTTON_JOIN_START_Y);
					m_pCompleteButton[m_nButtonState[CITY_MISSION_BUTTON_JOIN]]->Render();
				}
				else
				{
					m_pJoinButton[m_nButtonState[CITY_MISSION_BUTTON_JOIN]]->Move(CITY_MISSION_BUTTON_JOIN_START_X, CITY_MISSION_BUTTON_JOIN_START_Y);
					m_pJoinButton[m_nButtonState[CITY_MISSION_BUTTON_JOIN]]->Render();
				}
				bJoinButton = FALSE;
			}
		}
	}
	if(bJoinButton)
	{
		m_pJoinButton[m_nButtonState[CITY_MISSION_BUTTON_JOIN]]->Move(CITY_MISSION_BUTTON_JOIN_START_X, CITY_MISSION_BUTTON_JOIN_START_Y);
		m_pJoinButton[m_nButtonState[CITY_MISSION_BUTTON_JOIN]]->Render();
	}
	m_pListScroll->Render();
//	for(i=0;i<CITY_MISSION_LINE_NUMBER;i++)
//	{
//		if(m_strQuestDesc[i+m_nCurrentQuestDescScroll][0] == NULL)
//		{
//			break;
//		}
//		m_pFontQuestDesc[i]->DrawText(CITY_MISSION_DESC_START_X, 
//			CITY_MISSION_DESC_START_Y+CITY_MISSION_DESC_INTERVAL*i,
//			GUI_FONT_COLOR,
//			m_strQuestDesc[i+m_nCurrentQuestDescScroll],0L);
//	}
	for(i=0;i<CITY_MISSION_LINE_NUMBER;i++)
	{
//		if(m_strQuestDesc[i+m_pDescScroll->GetCurrentScrollIndex()][0] == NULL)
//		{
//			break;
//		}
		if(i+m_pDescScroll->GetCurrentScrollIndex() >= m_nQuestDescLineNumber)
		{
			break;
		}
		m_pFontQuestDesc[i]->DrawText(CITY_MISSION_DESC_START_X, 
			CITY_MISSION_DESC_START_Y+CITY_MISSION_DESC_INTERVAL*i,
			GUI_FONT_COLOR,
			m_strQuestDesc[i+m_pDescScroll->GetCurrentScrollIndex()],0L);
	}
	m_pDescScroll->Render();
}

void CINFCityMission::Tick()
{

}

#define CITY_MISSION_LINE_SIZE_X			169
#define CITY_MISSION_LINE_SIZE_Y			16
int CINFCityMission::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(m_pListScroll)
	{
		if(m_pListScroll->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
		{
			return INF_MSGPROC_BREAK;
		}
	}
	if( m_nCurrentSelectQuestList != m_pListScroll->GetCurrentSelectDataIndex() )
	{
		m_nCurrentSelectQuestList = m_pListScroll->GetCurrentSelectDataIndex();
		CMapQuestIterator it = m_mapCityMission.find(m_nCurrentSelectQuestList);
		if(it != m_mapCityMission.end())
		{
			SetMissionDetailInfo(it->second);
		}
	}
	if(m_pDescScroll)
	{
		if(m_pDescScroll->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
		{
			return INF_MSGPROC_BREAK;
		}
	}
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if(pt.y > CITY_MISSION_BUTTON_JOIN_START_Y && 
				pt.y < CITY_MISSION_BUTTON_JOIN_START_Y + CITY_MISSION_BUTTON_JOIN_SIZE_Y)
			{
				if( pt.x > CITY_MISSION_BUTTON_JOIN_START_X && 
					pt.x < CITY_MISSION_BUTTON_JOIN_START_X+CITY_MISSION_BUTTON_JOIN_SIZE_X)
				{
					if(m_nButtonState[CITY_MISSION_BUTTON_JOIN] != BUTTON_STATE_DOWN)
						m_nButtonState[CITY_MISSION_BUTTON_JOIN] = BUTTON_STATE_UP;
				}
				else 
				{
					m_nButtonState[CITY_MISSION_BUTTON_JOIN] = BUTTON_STATE_NORMAL;
				}
				if( pt.x > CITY_MISSION_BUTTON_CANCEL_START_X && 
					pt.x < CITY_MISSION_BUTTON_CANCEL_START_X+CITY_MISSION_BUTTON_CANCEL_SIZE_X)
				{
					if(m_nButtonState[CITY_MISSION_BUTTON_CANCEL] != BUTTON_STATE_DOWN)
						m_nButtonState[CITY_MISSION_BUTTON_CANCEL] = BUTTON_STATE_UP;
				}
				else 
				{
					m_nButtonState[CITY_MISSION_BUTTON_CANCEL] = BUTTON_STATE_NORMAL;
				}
			}
			else
			{
				m_nButtonState[CITY_MISSION_BUTTON_JOIN] = BUTTON_STATE_NORMAL;
				m_nButtonState[CITY_MISSION_BUTTON_CANCEL] = BUTTON_STATE_NORMAL;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if(pt.y > CITY_MISSION_BUTTON_JOIN_START_Y && 
				pt.y < CITY_MISSION_BUTTON_JOIN_START_Y + CITY_MISSION_BUTTON_JOIN_SIZE_Y)
			{
				if( pt.x > CITY_MISSION_BUTTON_JOIN_START_X && 
					pt.x < CITY_MISSION_BUTTON_JOIN_START_X+CITY_MISSION_BUTTON_JOIN_SIZE_X)
				{
					m_nButtonState[CITY_MISSION_BUTTON_JOIN] = BUTTON_STATE_DOWN;
				}
				else 
				{
					m_nButtonState[CITY_MISSION_BUTTON_JOIN] = BUTTON_STATE_NORMAL;
				}
				if( pt.x > CITY_MISSION_BUTTON_CANCEL_START_X && 
					pt.x < CITY_MISSION_BUTTON_CANCEL_START_X+CITY_MISSION_BUTTON_CANCEL_SIZE_X)
				{
					m_nButtonState[CITY_MISSION_BUTTON_CANCEL] = BUTTON_STATE_DOWN;
				}
				else 
				{
					m_nButtonState[CITY_MISSION_BUTTON_CANCEL] = BUTTON_STATE_NORMAL;
				}
			}
//			if( pt.x > CITY_MISSION_LIST_START_X && 
//				pt.x < CITY_MISSION_LIST_START_X + CITY_MISSION_LINE_SIZE_X &&
//				pt.y > CITY_MISSION_LIST_START_Y && 
//				pt.y < CITY_MISSION_LIST_START_Y + CITY_MISSION_LINE_INTERVAL*CITY_MISSION_LINE_NUMBER)
//			{
//				int i = (pt.y-CITY_MISSION_LIST_START_Y)/CITY_MISSION_LINE_INTERVAL;
//				if(i>=0 && i<CITY_MISSION_LINE_NUMBER)
//				{
//					if(m_mapCityMission.size() >= i + m_nCurrentQuestListScroll)
//					{
//						m_nCurrentSelectQuestList = i + m_nCurrentQuestListScroll;
//						CMapQuestIterator it = m_mapCityMission.find(m_nCurrentSelectQuestList);
//						if(it != m_mapCityMission.end())
//						{
//							SetMissionDetailInfo(it->second);
//						}
//					}
//				}
//			}
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if(pt.y > CITY_MISSION_BUTTON_JOIN_START_Y && 
				pt.y < CITY_MISSION_BUTTON_JOIN_START_Y + CITY_MISSION_BUTTON_JOIN_SIZE_Y)
			{
				if( pt.x > CITY_MISSION_BUTTON_JOIN_START_X && 
					pt.x < CITY_MISSION_BUTTON_JOIN_START_X+CITY_MISSION_BUTTON_JOIN_SIZE_X)
				{
					if(m_nButtonState[CITY_MISSION_BUTTON_JOIN] == BUTTON_STATE_DOWN)
					{
						OnButtonDown(CITY_MISSION_BUTTON_JOIN);
					}
					m_nButtonState[CITY_MISSION_BUTTON_JOIN] = BUTTON_STATE_UP;
				}
				else 
				{
					m_nButtonState[CITY_MISSION_BUTTON_JOIN] = BUTTON_STATE_NORMAL;
				}
				if( pt.x > CITY_MISSION_BUTTON_CANCEL_START_X && 
					pt.x < CITY_MISSION_BUTTON_CANCEL_START_X+CITY_MISSION_BUTTON_CANCEL_SIZE_X)
				{
					if(m_nButtonState[CITY_MISSION_BUTTON_CANCEL] == BUTTON_STATE_DOWN)
					{
						OnButtonDown(CITY_MISSION_BUTTON_CANCEL);
					}
					m_nButtonState[CITY_MISSION_BUTTON_CANCEL] = BUTTON_STATE_UP;
				}
				else 
				{
					m_nButtonState[CITY_MISSION_BUTTON_CANCEL] = BUTTON_STATE_NORMAL;
				}
			}
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

void CINFCityMission::OnButtonDown(int nButton)
{
	switch(nButton)
	{
	case CITY_MISSION_BUTTON_JOIN:
		{
			if(m_nCurrentSelectQuestList >= 0)
			{
				CMapQuestIterator it = m_mapCityMission.find(m_nCurrentSelectQuestList);
				// �������� �ƴ϶��
				if( it != m_mapCityMission.end())
				{
					if(g_pQuestData->CheckEndQuestCondition( it->second, 
						((CINFCityBase*)m_pParent)->GetCurrentBuildingNPC()->buildingInfo.NPCIndex))
					{
						g_pQuestData->SendFieldSocketQuestRequestSuccess( it->second->QuestIndex );
						m_nCancerSelect = (m_pListScroll->GetCurrentSelectWindowIndex()+m_pListScroll->GetCurrentScrollIndex());
						
						
					}
					else if(!g_pQuestData->IsQuestProgress(it->second->QuestIndex))
					{
						if(it->second->QuestStartType == QUEST_START_TYPE_COUPON/*���� ���*/)
						{
							g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_QUEST_0014,//"���� ��ȣ�� �Է��� �ֽʽÿ�."
								_Q_INPUT_SERIAL_NUM, it->second->QuestIndex);
						}
						else
						{
							g_pQuestData->SendFieldSocketQuestRequestStart(it->second->QuestIndex,0);
						}
					}
					else
					{
						g_pGameMain->m_pInfWindow->AddMsgBox(STRERR_C_QUEST_0003,_MESSAGE);//"�̹� ���۵� �̼��Դϴ�."
					}
				}
			}
		}
		break;
	case CITY_MISSION_BUTTON_CANCEL:
		{
//			((CINFCityBase*)m_pParent)->GetRefCurrentBuildingNPCState() = BUILDINGNPC_RENDER_TALKBOX;
			if(m_nCurrentSelectQuestList >= 0)
			{
				CMapQuestIterator it = m_mapCityMission.find(m_nCurrentSelectQuestList);
				// �������� �ƴ϶��
				if( it != m_mapCityMission.end())
				{
					if(g_pQuestData->IsQuestProgress(it->second->QuestIndex))
					{
						m_nCancerSelect = (m_pListScroll->GetCurrentSelectWindowIndex()+m_pListScroll->GetCurrentScrollIndex());
						g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_QUEST_0015,_Q_QUEST_DISCARD, it->second->QuestIndex);//"�ش� �̼��� �����Ͻðڽ��ϱ�?."
					}
				}
			}
		}
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFCityMission::SetMissionDetailInfo(CQuest* pQuest)
/// \brief		Mission�� ���� ������ �����ش�.(���� ���� ������ NPCTalk)
/// \author		dhkwon
/// \date		2004-04-30 ~ 2004-04-30
/// \warning	Missoion List���� ���ýÿ� ������
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCityMission::SetMissionDetailInfo(CQuest* pQuest)
{
	memset(m_strQuestDesc, 0x00, CITY_MISSION_MAX_NUMBER*CITY_MISSION_LINE_LENGTH);
	// ���� ���� ����
	m_nQuestDescLineNumber = 0;
	SetQuestDescQuestName(m_nQuestDescLineNumber++, pQuest->QuestName);
	if(pQuest->QuestType == QUEST_TYPE_REPEATABLE)
	{
		SetQuestDescQuestTypeRepeatable(m_nQuestDescLineNumber++);
	}
	if( pQuest->ReqLevel.Min > 1 || pQuest->ReqLevel.Max != MAX_INT_VALUE)
	{
		SetQuestDescRequstLevel(m_nQuestDescLineNumber++, pQuest);
	}
	if(pQuest->QuestType == QUEST_TYPE_SERIES && pQuest->NextQuestIndex != 0)
	{
		SetQuestDescNextQuestName(m_nQuestDescLineNumber++, pQuest->NextQuestIndex);
	}
	////////////////////
	// Req~�� ���� ����
	////////////////////
	if(!pQuest->ReqItemVector.empty())
	{
		SetQuestDescItemCountVector(STRMSG_C_QUEST_0016, pQuest->ReqItemVector);//"�غ� ����"
	}
	if(!pQuest->StartPayItemVector.empty())
	{
//		SetQuestDescItemCountVector("���۽� �����Ǵ� ������ :", pQuest->StartPayItemVector);
		SetQuestDescItemCountVector(STRMSG_C_QUEST_0017, pQuest->StartPayItemVector);//"���۽� ����"
	}
//	if(!pQuest->StartDelItemVector.empty())
//	{
//		SetQuestDescItemCountVector("���۽� �ʿ��� ������ :", pQuest->StartDelItemVector);
//	}
//	if(!pQuest->EndDeleteItemVector.empty())
//	{
//		SetQuestDescItemCountVector("�Ϸ�� �ʿ��� ������ :", pQuest->EndDeleteItemVector);
//	}
	if(!pQuest->TargetItemVector.empty())
	{
		SetQuestDescItemCountVector(STRMSG_C_QUEST_0018, pQuest->TargetItemVector);//"�Ϸ�� �ʿ��� ������ :"
	}
	if(!pQuest->EndPayItemVector.empty())
	{
//		SetQuestDescItemCountVector("�Ϸ�� �����Ǵ� ������ :", pQuest->EndPayItemVector);
		SetQuestDescItemCountVector(STRMSG_C_QUEST_0019, pQuest->EndPayItemVector);//"���� ������"
	}
	if(pQuest->EndRandomPayItemVector.size() > 0)
	{
		SetQuestDescEndRandomPayItemVector(STRMSG_C_QUEST_0020, pQuest->EndRandomPayItemVector );//"Ȯ�� ���� ������"
	}
	if(pQuest->EndPayMixItemVector.size() > 0)
	{
		SetQuestDescEndPayMixItemVector(STRMSG_C_QUEST_0021, pQuest->EndPayMixItemVector );//"���� ������ (����)"
	}
	if(pQuest->EndPayBonusStat != 0)
	{
		SetQuestDescEndPayBonusStat(m_nQuestDescLineNumber++, pQuest->EndPayBonusStat);
	}
//	if(pQuest->EndPayBonusSkillPoint != 0)	// 2005-11-16 by ispark
//	{
//		SetQuestDescEndPayBonusSkillPoint(m_nQuestDescLineNumber++, pQuest->EndPayBonusSkillPoint);
//	}
	if(pQuest->EndPayFame != 0)
	{
		SetQuestDescEndPayFame(m_nQuestDescLineNumber++, pQuest->EndPayFame);
	}
	if(pQuest->EndPayStatus != 0)
	{
		SetQuestDescEndPayStatus(m_nQuestDescLineNumber++, pQuest->EndPayStatus);
	}
	if(pQuest->EndPayLevel != 0)
	{
		SetQuestDescEndPayLevel(m_nQuestDescLineNumber++, pQuest->EndPayLevel);
	}
	if(pQuest->EndPayExperience != 0)
	{
		SetQuestDescEndPayExperience(m_nQuestDescLineNumber++, pQuest->EndPayExperience);
	}
	if(pQuest->EndPayPropensity != 0)
	{
		SetQuestDescEndPayPropensity(m_nQuestDescLineNumber++, pQuest->EndPayPropensity);
	}
/*//������ �������� �ʴ´�.
	if(pQuest->EndPayWarpMapIndex != 0)
	{
		SetQuestDescEndPayWarpMap(m_nQuestDescLineNumber++, pQuest->EndPayWarpMapIndex);
	}
*/
	m_pDescScroll->SetNumberOfData( m_nQuestDescLineNumber );
	// NPCTalk
	MEX_QUEST_INFO* pCharacterQuest = g_pQuestData->FindCharacterQuest(pQuest->QuestIndex);
	if(pCharacterQuest)
	{
		if(g_pQuestData->IsQuestProgress(pQuest->QuestIndex))
		{
			GUI_BUILDINGNPC* pBuilding = ((CINFCityBase*)m_pParent)->GetCurrentBuildingNPC();
			if(pBuilding == NULL)
			{
				return;
			}
			if( pQuest->QuestNPCInfo.NPCIndex == pBuilding->buildingInfo.NPCIndex )
			{
				((CINFCityBase*)m_pParent)->SetNPCTalkInfo(pQuest->QuestNPCInfo.MidTalk);
			}
			else if( pQuest->TargetMeetNPCInfo.NPCIndex == pBuilding->buildingInfo.NPCIndex )
			{
				((CINFCityBase*)m_pParent)->SetNPCTalkInfo(pQuest->TargetMeetNPCInfo.MidTalk);
			}
		}
		else if(g_pQuestData->IsQuestCompleted(pQuest->QuestIndex))
		{
			if( pQuest->QuestNPCInfo.NPCIndex == pQuest->TargetMeetNPCInfo.NPCIndex )
			{
				((CINFCityBase*)m_pParent)->SetNPCTalkInfo(pQuest->QuestNPCInfo.SuccessTalk);
			}
			else
			{
				((CINFCityBase*)m_pParent)->SetNPCTalkInfo(pQuest->TargetMeetNPCInfo.SuccessTalk);
			}
		}
	}
	else
	{
		((CINFCityBase*)m_pParent)->SetNPCTalkInfo(pQuest->QuestNPCInfo.PreTalk);
		((CINFCityBase*)m_pParent)->SetNPCTalkInfo(pQuest->QuestDescription, 1);
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void SetQuestDescString(int index, char* szString)
/// \brief		�Ϲ� ��Ʈ���� �ִ´�.(�Ʒ� ������ �Լ� ����� ���� ���ý� ���)
/// \author		dhkwon
/// \date		2004-04-30 ~ 2004-04-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCityMission::SetQuestDescString(int index, char* szString)
{
	strcpy(m_strQuestDesc[index], szString);
}
void CINFCityMission::SetQuestDescQuestName(int index, char* szName)
{
	strcpy(m_strQuestDesc[index], STRMSG_C_QUEST_0022);//"���� : "
	strcat(m_strQuestDesc[index], szName);
}
void CINFCityMission::SetQuestDescQuestTypeRepeatable(int index)//���� �ݺ� �̼��ΰ�츸 ǥ��
{
	strcpy(m_strQuestDesc[index], STRMSG_C_QUEST_0023);//"Ư�� : ���� �ݺ� �̼�"
}
void CINFCityMission::SetQuestDescRequstLevel(int index, CQuest* pQuest)//���� �ݺ� �̼��ΰ�츸 ǥ��
{
	if(pQuest->ReqLevel.Max > 100)
	{
		strcpy(m_strQuestDesc[index], STRMSG_C_QUEST_0024);//"���� ���� : "
		wsprintf(m_strQuestDesc[index], "%s%d ~",m_strQuestDesc[index] ,pQuest->ReqLevel.Min);
	}
	else
	{
		strcpy(m_strQuestDesc[index], STRMSG_C_QUEST_0024);//"���� ���� : "
		wsprintf(m_strQuestDesc[index], "%s%d ~ %d ",m_strQuestDesc[index] ,pQuest->ReqLevel.Min, pQuest->ReqLevel.Max);//sbuy3�� ����.
	}
}
void CINFCityMission::SetQuestDescNextQuestName(int index, int nNextQuestIndex)//���Ӽ��� ��츸 ǥ��
{
	CQuest* pQuest = g_pQuestData->FindQuest(nNextQuestIndex);
	if(pQuest)
	{
		strcpy(m_strQuestDesc[index], STRMSG_C_QUEST_0025);//"���� �̼� : "
		strcat(m_strQuestDesc[index], pQuest->QuestName);
	}
}
void CINFCityMission::SetQuestDescReqUnitKind(int index, USHORT nReqUnitKind)
{
	strcpy(m_strQuestDesc[index], STRMSG_C_QUEST_0026);//"���� ���� : "
	// 2007-03-29 by bhsohn China String
	char chGear[64];
	memset(chGear, 0x00, 64);
	if(IS_BT(nReqUnitKind))
	{
		//strcat(m_strQuestDesc[index], "B-Gear,");
		wsprintf(chGear, "%s," ,STRCMD_CS_UNITKIND_BGEAR);		
		strcat(m_strQuestDesc[index], chGear);
	}
	if(IS_ST(nReqUnitKind))
	{
		//strcat(m_strQuestDesc[index], "I-Gear,");		
		wsprintf(chGear, "%s," ,STRCMD_CS_UNITKIND_IGEAR);		
		strcat(m_strQuestDesc[index], chGear);
	}
	if(IS_OT(nReqUnitKind))
	{
		//strcat(m_strQuestDesc[index], "M-Gear,");		
		wsprintf(chGear, "%s," ,STRCMD_CS_UNITKIND_MGEAR);		
		strcat(m_strQuestDesc[index], chGear);
	}
	if(IS_DT(nReqUnitKind))
	{
		//strcat(m_strQuestDesc[index], "A-Gear,");		
		wsprintf(chGear, "%s," ,STRCMD_CS_UNITKIND_AGEAR);		
		strcat(m_strQuestDesc[index], chGear);
	}
	// end 2007-03-29 by bhsohn China String
}

void CINFCityMission::SetQuestDescReqLevel(int index, INT_RANGE nReqLevel)
{
	wsprintf(m_strQuestDesc[index], STRMSG_C_QUEST_0027, nReqLevel.Min, nReqLevel.Min );//"���� ���� : %d ~ %d"
}
void CINFCityMission::SetQuestDescReqAttackPart(int index, INT_RANGE nReqAttackPart)
{
	wsprintf(m_strQuestDesc[index], STRMSG_C_QUEST_0028, nReqAttackPart.Min, nReqAttackPart.Min );//"�ʿ� ������Ʈ : %d ~ %d"
}
void CINFCityMission::SetQuestDescReqDefensePart(int index, INT_RANGE nReqDefensePart)// ����->���
{
	wsprintf(m_strQuestDesc[index], STRMSG_C_QUEST_0029, nReqDefensePart.Min, nReqDefensePart.Min );//"�ʿ� �����Ʈ : %d ~ %d"
}
void CINFCityMission::SetQuestDescReqFuelPart(int index, INT_RANGE nReqFuelPart)
{
	wsprintf(m_strQuestDesc[index], STRMSG_C_QUEST_0031, nReqFuelPart.Min, nReqFuelPart.Min );//"�ʿ� ������Ʈ : %d ~ %d"
}
void CINFCityMission::SetQuestDescReqSoulPart(int index, INT_RANGE nReqSoulPart)
{
	wsprintf(m_strQuestDesc[index], STRMSG_C_QUEST_0032, nReqSoulPart.Min, nReqSoulPart.Min );//"�ʿ� ������Ʈ : %d ~ %d"
}
void CINFCityMission::SetQuestDescReqShieldPart(int index, INT_RANGE nReqShieldPart)// ���->����
{
	wsprintf(m_strQuestDesc[index], STRMSG_C_QUEST_0033, nReqShieldPart.Min, nReqShieldPart.Min );//"�ʿ� ������Ʈ : %d ~ %d"
}
void CINFCityMission::SetQuestDescReqDodgePart(int index, INT_RANGE nReqDodgePart)
{
	wsprintf(m_strQuestDesc[index], STRMSG_C_QUEST_0034, nReqDodgePart.Min, nReqDodgePart.Min );//"�ʿ� ȸ����Ʈ : %d ~ %d"
}
void CINFCityMission::SetQuestDescReqQuest(int index, int QuestIndex)// ����Ʈ ������ŭ �ݺ��Ͽ� �ִ´�.
{
	CQuest* pQuest = g_pQuestData->FindQuest(QuestIndex);
	if(pQuest)
	{
		strcpy(m_strQuestDesc[index], "   ");
		strcat(m_strQuestDesc[index], pQuest->QuestName);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFCityMission::SetQuestDescItemCount(int index, ITEM_W_COUNT itemCount)
/// \brief		vector�� ������� ������ �ϳ��� �׸���.
/// \author		dhkwon
/// \date		2004-04-30 ~ 2004-04-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCityMission::SetQuestDescItemCount(int index, ITEM_W_COUNT itemCount)// �Ʒ� �ּ� �Լ� ��ǥ
{
	ITEM* pItem = g_pDatabase->GetServerItemInfo(itemCount.ItemNum);
	if(pItem)
	{
		if(pItem->ItemNum == MONEY_ITEM_NUMBER)
		{
			wsprintf(m_strQuestDesc[index], STRMSG_C_QUEST_0035, itemCount.Count );//"   %d(����)"	
		}
		else
		{
			wsprintf(m_strQuestDesc[index], STRMSG_C_QUEST_0036, pItem->ItemName, itemCount.Count );//"   %s : %d��"
		}
	}
}
void CINFCityMission::SetQuestDescItemCount(int& index, QUEST_PAY_ITEM itemCount)// �Ʒ� �ּ� �Լ� ��ǥ
{
	ITEM* pITEMPerItem = NULL;
	if(itemCount.PerItem != 0)
	{
		pITEMPerItem = g_pDatabase->GetServerItemInfo( itemCount.PerItem );
	}
	if(itemCount.ToUnitKind == UNITKIND_ALL_MASK)
	{
		ITEM* pItem = g_pDatabase->GetServerItemInfo(itemCount.ItemNum);
		if(pItem)
		{
			if(pITEMPerItem == NULL)
			{
				if(pItem->ItemNum == MONEY_ITEM_NUMBER)
				{
					wsprintf(m_strQuestDesc[index], STRMSG_C_QUEST_0035, itemCount.Count );//"   %d(����)"
				}
				else
				{
					wsprintf(m_strQuestDesc[index], STRMSG_C_QUEST_0036, pItem->ItemName, itemCount.Count );//"   %s : %d��"
				}
			}
			else
			{
				if(pItem->ItemNum == MONEY_ITEM_NUMBER)
				{
					wsprintf(m_strQuestDesc[index], STRMSG_C_QUEST_0037, itemCount.Count, pITEMPerItem->ItemName );//"   %d(����)(%s 1����)"
				}
				else
				{
					wsprintf(m_strQuestDesc[index], STRMSG_C_QUEST_0038, pItem->ItemName, itemCount.Count, pITEMPerItem->ItemName );//"   %s : %d��(%s 1����)"
				}
			}
		}
	}
	else
	{
		char buf[64];
		memset( buf, 0x00, sizeof(buf) );
		// 2007-03-29 by bhsohn China String 
		if(IS_BT(itemCount.ToUnitKind))
		{
			//strcat(buf,"B-GEAR");
			strcat(buf,STRCMD_CS_UNITKIND_BGEAR);
		}
		if(IS_OT(itemCount.ToUnitKind))
		{
			//strcat(buf,"M-GEAR");
			strcat(buf,STRCMD_CS_UNITKIND_MGEAR);
		}
		if(IS_DT(itemCount.ToUnitKind))
		{
			//strcat(buf,"A-GEAR");
			strcat(buf,STRCMD_CS_UNITKIND_AGEAR);
		}
		if(IS_ST(itemCount.ToUnitKind))
		{
			//strcat(buf,"I-GEAR");
			strcat(buf,STRCMD_CS_UNITKIND_IGEAR);
		}
		// end 2007-03-29 by bhsohn China String 
		ITEM* pItem = g_pDatabase->GetServerItemInfo(itemCount.ItemNum);
		if(pItem)
		{
			if(pITEMPerItem == NULL)
			{
				if(pItem->ItemNum == MONEY_ITEM_NUMBER)
				{
					wsprintf(m_strQuestDesc[index], STRMSG_C_QUEST_0039, buf, itemCount.Count );//"   [%s] : %d(����)"
				}
				else
				{
//					wsprintf(m_strQuestDesc[index], STRMSG_C_QUEST_0040, buf, pItem->ItemName, itemCount.Count );//"   [%s] %s : %d��"
					wsprintf(m_strQuestDesc[index++], "   [%s]",buf);
					wsprintf(m_strQuestDesc[index],STRMSG_C_050510_0001, pItem->ItemName, itemCount.Count);	//"    %s: %d��"
				}
			}
			else
			{
				if(pItem->ItemNum == MONEY_ITEM_NUMBER)
				{
					wsprintf(m_strQuestDesc[index], STRMSG_C_QUEST_0041, buf,  itemCount.Count, pITEMPerItem->ItemName );//"   [%s] %d(����)(%s 1����)"
				}
				else
				{
//					wsprintf(m_strQuestDesc[index], STRMSG_C_QUEST_0042, buf, pItem->ItemName, itemCount.Count, pITEMPerItem->ItemName );//"   [%s] %s : %d��(%s 1����)"
					wsprintf(m_strQuestDesc[index++], "   [%s]", buf);
					wsprintf(m_strQuestDesc[index], STRMSG_C_050510_0002, pItem->ItemName, itemCount.Count, pITEMPerItem->ItemName );//"    %s: %d��(%s 1����)"
				}
			}
		}
	}
}

void CINFCityMission::SetQuestDescItemCount(int index, ITEM_W_COUNT_PROB itemCount)// �Ʒ� �ּ� �Լ� ��ǥ
{
	ITEM* pITEM = g_pDatabase->GetServerItemInfo(itemCount.ItemNum);
	if(pITEM)
	{
		if(pITEM->ItemNum == MONEY_ITEM_NUMBER)
		{
			wsprintf(m_strQuestDesc[index], STRMSG_C_QUEST_0043, itemCount.Count, itemCount.Prob100 );//"   %d(����)(Ȯ��:%d%%)"
		}
		else
		{
			wsprintf(m_strQuestDesc[index], STRMSG_C_QUEST_0044, pITEM->ItemName, itemCount.Count, itemCount.Prob100 );//"   %s:%d��(Ȯ��:%d%%)"
		}
	}
}

void CINFCityMission::SetQuestDescEndPayBonusStat(int index, BYTE nBonusStat)
{
	wsprintf(m_strQuestDesc[index], STRMSG_C_QUEST_0045, nBonusStat );//"���� ���ʽ����� : %d"
}
void CINFCityMission::SetQuestDescEndPayBonusSkillPoint(int index, BYTE nBonusSkillPoint)
{
	wsprintf(m_strQuestDesc[index], STRMSG_C_QUEST_0046, nBonusSkillPoint );//"���� ���ʽ���ų����Ʈ : %d"
}
void CINFCityMission::SetQuestDescEndPayFame(int index, int nFame)
{
	wsprintf(m_strQuestDesc[index], STRMSG_C_QUEST_0047, nFame );//"���� �� ��� : %d"
}
void CINFCityMission::SetQuestDescEndPayStatus(int index, int nStatus)
{
	wsprintf(m_strQuestDesc[index], STRMSG_C_QUEST_0048, nStatus );//"���� �ź� ��� : %d"

}
void CINFCityMission::SetQuestDescEndPayLevel(int index, BYTE nLevel)
{
	wsprintf(m_strQuestDesc[index], STRMSG_C_QUEST_0049, nLevel );//"���� ���� ��� : %d"
}
void CINFCityMission::SetQuestDescEndPayExperience(int index, int nExperience)
{
	wsprintf(m_strQuestDesc[index], STRMSG_C_QUEST_0050, nExperience );//"���� ����ġ ��� : %d"
}
void CINFCityMission::SetQuestDescEndPayPropensity(int index, int nPropensity)
{
	wsprintf(m_strQuestDesc[index], STRMSG_C_QUEST_0051, nPropensity );//"���� �ź� ��ġ ��� : %d"
}
void CINFCityMission::SetQuestDescEndPayWarpMap(int index, MapIndex_t nWrapMapIndex)
{
	wsprintf(m_strQuestDesc[index], STRMSG_C_QUEST_0052, nWrapMapIndex );//"���̵� : %d"
}


void CINFCityMission::SetQuestDescItemCountVector(char * strName, vector<ITEM_W_COUNT> &vecItem)
{
	SetQuestDescString(m_nQuestDescLineNumber++, strName);
	vector<ITEM_W_COUNT>::iterator itItemCount = vecItem.begin();
	while(itItemCount != vecItem.end())
	{
		SetQuestDescItemCount(m_nQuestDescLineNumber++, *itItemCount );
		itItemCount++;
	}

}

void CINFCityMission::SetQuestDescItemCountVector(char * strName, vector<QUEST_PAY_ITEM> &vecItem)
{
	SetQuestDescString(m_nQuestDescLineNumber++, strName);
	vector<QUEST_PAY_ITEM>::iterator itItemCount = vecItem.begin();
	while(itItemCount != vecItem.end())
	{
		SetQuestDescItemCount(m_nQuestDescLineNumber, *itItemCount );
		m_nQuestDescLineNumber++;
		itItemCount++;
	}

}

void CINFCityMission::SetQuestDescEndRandomPayItemVector(char* strName, vector<QUEST_RANDOM_PAY_ITEM> &vecItem )
{
	SetQuestDescString(m_nQuestDescLineNumber++, strName );
	vector<QUEST_RANDOM_PAY_ITEM>::iterator it = vecItem.begin();
	while(it != vecItem.end() )
	{
		char buf[128];
		ITEM* pITEM = g_pDatabase->GetServerItemInfo( (*it).PerItemNum );
		if(pITEM == NULL)
		{
			DBGOUT("Non Existent Mission Item Information (ItemNum:%d).\n", (*it).PerItemNum);
			it++;
			continue;
		}
		wsprintf( buf, STRMSG_C_QUEST_0053, pITEM->ItemName );//" * %s 1����"
		SetQuestDescString(m_nQuestDescLineNumber++, buf );
		for(int i=0;i<(*it).NumOfItems;i++)
		{
			SetQuestDescItemCount( m_nQuestDescLineNumber++, (*it).ItemWithCountProb[i] );
		}
		it++;
	}
}

void CINFCityMission::SetQuestDescEndPayMixItemVector(char* strName, vector<ITEM_MIXING_INFO> &vecItem )
{
	SetQuestDescString(m_nQuestDescLineNumber++, strName );

	for (auto& item : vecItem)
	{
		auto pITEM = g_pDatabase->GetServerItemInfo(item.TargetItemNum);
		
		if (!pITEM)
		{
			DBGOUT("Non Existent Mission Item Information.(ItemNum:%d).\n", item.TargetItemNum);
			continue;
		}

		char buf[128];

		wsprintf(buf, STRMSG_C_QUEST_0054, pITEM->ItemName );//" * ��� ������ : %s"
		SetQuestDescString(m_nQuestDescLineNumber++, buf );

		if (item.MixingProbability != MAX_RAND10K_VALUE + 1) // Ȯ�� 100%�� �ƴ� ��츸 ���δ�.
		{
			wsprintf(buf, STRMSG_C_QUEST_0055, int(item.MixingProbability/100));//"   [���� Ȯ�� : %d%%]"
			SetQuestDescString(m_nQuestDescLineNumber++, buf);
		}

		strcpy(buf, STRMSG_C_QUEST_0056);//"   [��� ������]"
		SetQuestDescString(m_nQuestDescLineNumber++, buf);
		
		for (auto i = 0; i < item.NumOfSourceItems; i++) SetQuestDescItemCount(m_nQuestDescLineNumber++, item.SourceItem[i]);
	}
}