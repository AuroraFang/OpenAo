// CGlobalImage.cpp: implementation of the CGlobalImage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GlobalImage.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "D3DHanFont.h"
#include "INFGameMain.h"
#include "QuestData.h"
#include "FieldWinSocket.h"
#include "dxutil.h"
#include "INFImageEx.h"	// 2011. 10. 10 by jskim UI�ý��� ����

#define GLOBAL_IMAGE_VIEW_TIME		0.0f

// 2007-03-20 by bhsohn ���� ��޹� ����ȸ ������ �߰�
// 2007-06-21 by bhsohn ���� ��޹� ����ȸ ������ ����
//#define GDWL_START_X				(g_pD3dApp->GetBackBufferDesc().Width-380)
#define GDWL_START_X				(g_pD3dApp->GetBackBufferDesc().Width-168)
#define GDWL_START_Y				23

#define MAX_GAP_TIME				3600.0f

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGlobalImage::CGlobalImage()
{
	FLOG( "CGlobalImage()" );
	
	for(int i=0; i<GLOBAL_IMAGE_MAX_COUNT; i++)
	{
		m_pImageGlobal[i]		= NULL;
	}
	
	m_pFontTitle	= NULL;
	m_fViewTime	= GLOBAL_IMAGE_VIEW_TIME;
	m_bRenderFlag = FALSE;
	m_dwAlpha = 0;
	m_nSelectImageIndex = -1;
	m_dwSelectType = 0;
	m_fTotalTime = 0.0f;
	m_fDelay = 0.0f;
	m_nRenderPosX = 0;
	m_nRenderPosY = 0;
	m_bMissionImg = FALSE;
	m_nMissionIndex = 0;

	// 2007-03-20 by bhsohn ���� ��޹� ����ȸ ������ �߰�
	m_pImageGdwl = NULL;	
	m_fGdwlViewTime = 0.0f;
	m_dwGdwlAlpha = 0 ;
	
	m_fGdwlCheckTotalTime = MAX_GAP_TIME - 10.0f;
	m_fGdwlLiveTotalTime = 3.0f;
	m_fGdwlDelay = 2.0f;		
	m_bGdwlRenderFlag = FALSE;
}

CGlobalImage::~CGlobalImage()
{
	FLOG( "~CGlobalImage()" );
	
	for(int i=0; i<GLOBAL_IMAGE_MAX_COUNT; i++)
	{
		util::del(m_pImageGlobal[i]);
	}
	
	util::del(m_pFontTitle);
//	util::del(m_pGameData);

	// 2007-03-20 by bhsohn ���� ��޹� ����ȸ ������ �߰�
	util::del(m_pImageGdwl);	 
}

HRESULT CGlobalImage::InitDeviceObjects()
{
	FLOG( "CGlobalImage::InitDeviceObjects(char* pData, int nSize)" );
	
//	char strPath[256];
//	g_pD3dApp->LoadPath( strPath, IDS_DIRECTORY_TEXTURE, strTexture);
//	SetResourceFile(strPath);
	DataHeader* pHeader;
	
	char buf[GLOBAL_IMAGE_MAX_COUNT][32];
	ZERO_MEMORY(buf);
	int nCount = 0;
	
	wsprintf( buf[MISSION_SUC],			"toimg0");
	wsprintf( buf[MISSION_FAI],			"toimg1");
	wsprintf( buf[MISSION_OUT],			"outmiss");
	wsprintf( buf[OUT_ZONE],			"outfly");
	
	for(nCount=0; nCount<GLOBAL_IMAGE_MAX_COUNT; nCount++)
	{
		pHeader = FindResource(buf[nCount]);
		if(pHeader)
		{
// 2011. 10. 10 by jskim UI�ý��� ����
			m_pImageGlobal[nCount] = new CINFImageEx;		
			m_pImageGlobal[nCount]->InitDeviceObjects( pHeader );
// ebd 2011. 10. 10 by jskim UI�ý��� ����
		}
	}

	m_pFontTitle = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE,  FALSE,256,32);
	m_pFontTitle->InitDeviceObjects(g_pD3dDev) ;
	
	// 2007-03-20 by bhsohn ���� ��޹� ����ȸ ������ �߰�
#ifdef KOR_GAME_RATINGS_HSSON
	pHeader = FindResource("gdwl");
	if(pHeader)
	{
// 2011. 10. 10 by jskim UI�ý��� ����
		m_pImageGdwl = new CINFImageEx;		
		m_pImageGdwl->InitDeviceObjects(pHeader) ;
// end 2011. 10. 10 by jskim UI�ý��� ����
	}	
#endif
	// end 2007-03-20 by bhsohn ���� ��޹� ����ȸ ������ �߰�
	
	return S_OK;
}

HRESULT CGlobalImage::RestoreDeviceObjects()
{
	FLOG( "CGlobalImage::RestoreDeviceObjects()" );

	for(int i=0; i<GLOBAL_IMAGE_MAX_COUNT; i++)
	{
		m_pImageGlobal[i]->RestoreDeviceObjects();
	}
	m_pFontTitle->RestoreDeviceObjects();
	
	// 2007-03-20 by bhsohn ���� ��޹� ����ȸ ������ �߰�
	if(m_pImageGdwl)
	{
		m_pImageGdwl->RestoreDeviceObjects();
	}
	// end 2007-03-20 by bhsohn ���� ��޹� ����ȸ ������ �߰�
	
	
	return S_OK;
}

//DataHeader* CGlobalImage::FindResource(char* szRcName)
//{
//	DataHeader* pHeader = NULL;
//	if(m_pGameData)
//	{
//		pHeader = m_pGameData->Find(szRcName);
//	}
//	return pHeader;
//}

HRESULT CGlobalImage::InvalidateDeviceObjects()
{
	FLOG( "CGlobalImage::InvalidateDeviceObjects()" );

	for(int i=0; i<GLOBAL_IMAGE_MAX_COUNT; i++)
	{
		m_pImageGlobal[i]->InvalidateDeviceObjects();
	}
	m_pFontTitle->InvalidateDeviceObjects();
	
	// 2007-03-20 by bhsohn ���� ��޹� ����ȸ ������ �߰�
	if(m_pImageGdwl)
	{
		m_pImageGdwl->InvalidateDeviceObjects();
	}	
	
	return S_OK;
}

HRESULT CGlobalImage::DeleteDeviceObjects()
{
	FLOG( "CGlobalImage::DeleteDeviceObjects()" );
	
	for(int i=0; i<GLOBAL_IMAGE_MAX_COUNT; i++)
	{	
		m_pImageGlobal[i]->DeleteDeviceObjects();
		util::del(m_pImageGlobal[i]);
	}

	m_pFontTitle->DeleteDeviceObjects();
	util::del(m_pFontTitle);

	// 2007-03-20 by bhsohn ���� ��޹� ����ȸ ������ �߰�
	if(m_pImageGdwl)
	{
		m_pImageGdwl->DeleteDeviceObjects();
		util::del(m_pImageGdwl);	
	}	
	// end 2007-03-20 by bhsohn ���� ��޹� ����ȸ ������ �߰�

	return S_OK;
}

void CGlobalImage::Render()
{
	FLOG( "CGlobalImage::Render()" );
	if(TRUE == m_bRenderFlag)
	{
		m_pImageGlobal[m_nSelectImageIndex]->SetColor(m_dwAlpha);
		m_pImageGlobal[m_nSelectImageIndex]->Move(m_nRenderPosX, m_nRenderPosY);
		m_pImageGlobal[m_nSelectImageIndex]->Render();
	}

	// 2007-03-20 by bhsohn ���� ��޹� ����ȸ ������ �߰�
#ifdef KOR_GAME_RATINGS_HSSON
	RederGdwl();
#endif
}

void CGlobalImage::Tick()
{
	FLOG( "CGlobalImage::Tick()" );

	if(TRUE == m_bRenderFlag)
	{	// ���̸� �������..
		m_dwAlpha = g_pGameMain->GetCurrentColor(m_fViewTime, m_dwSelectType, m_fTotalTime,m_fDelay);
		m_fViewTime += g_pD3dApp->GetElapsedTime();
		if(m_fViewTime > m_fTotalTime)
		{
			m_fViewTime = m_dwSelectType;
			m_bRenderFlag = FALSE;
			if(m_bMissionImg == TRUE)
			{
				if(m_nSelectImageIndex == MISSION_SUC)				
					g_pD3dApp->SendMissionComplete(m_nMissionIndex);
				else if(m_nSelectImageIndex == MISSION_FAI)
				{
					CQuest *pQuestInfo = g_pQuestData->GetProgressMission();

					MSG_FC_QUEST_DISCARD_QUEST sMsg;
					sMsg.QuestIndex = pQuestInfo->QuestIndex;
					g_pFieldWinSocket->SendMsg( T_FC_QUEST_DISCARD_QUEST, (char*)&sMsg, sizeof(sMsg) );
				}
			}
		}
	}
	// 2007-03-20 by bhsohn ���� ��޹� ����ȸ ������ �߰�
#ifdef KOR_GAME_RATINGS_HSSON
	TickGdwl();	
#endif
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� ��� ����ȸ �ΰ� ǥ��
/// \author		// 2007-03-20 by bhsohn ���� ��޹� ����ȸ ������ �߰�
/// \date		2007-03-20 ~ 2007-03-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CGlobalImage::TickGdwl()
{
	if(NULL == m_pImageGdwl)
	{
		return;
	}

	m_fGdwlCheckTotalTime += g_pD3dApp->GetCheckElapsedTime();
	
	// 1�ð��� ������ �׋����� ����ش�. 
	if(m_fGdwlCheckTotalTime < MAX_GAP_TIME)
	{
		return;
	}
	m_fGdwlViewTime += g_pD3dApp->GetCheckElapsedTime();	

	// 8�ʵ��� ����ְ� �����.
	if(m_fGdwlViewTime > m_fGdwlLiveTotalTime)
	{
		m_fGdwlCheckTotalTime = 0.0f;
		m_fGdwlViewTime = 0.0f;
		m_dwGdwlAlpha = 0;		
		m_bGdwlRenderFlag = FALSE;
		return;
	}
	m_bGdwlRenderFlag = TRUE;	
	// ���̸� �������..
	m_dwGdwlAlpha = g_pGameMain->GetCurrentColor(m_fGdwlViewTime, 
													ALPHA_DELAY_WAVE, 
													m_fGdwlLiveTotalTime,
													m_fGdwlDelay);	
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� ��� ����ȸ �ΰ� ǥ��
/// \author		// 2007-03-20 by bhsohn ���� ��޹� ����ȸ ������ �߰�
/// \date		2007-03-20 ~ 2007-03-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CGlobalImage::RederGdwl()
{
	if(NULL == m_pImageGdwl)
	{
		return;
	}
	
	if(FALSE == m_bGdwlRenderFlag)
	{
		return;
	}	
	
	m_pImageGdwl->SetColor(m_dwGdwlAlpha);
	m_pImageGdwl->Move(GDWL_START_X, GDWL_START_Y);
	m_pImageGdwl->Render();
	

}


void CGlobalImage::SetImageInfo(int nImageIndex, BOOL bMissionImg, int nQuestIndex, int x, int y, DWORD dwType, float fTotalTime, float fDelay)
{
	FLOG( "CGlobalImage::SetImageInfo(char *strImageName, DWORD dwType=ALPHA_DELAY_WAVE, int nQuestIndex = 0, float fTotalTime, float fDelay)" );
	
	if(nImageIndex <= -1 && m_nSelectImageIndex > GLOBAL_IMAGE_MAX_COUNT)
		return;
	if(m_bRenderFlag == TRUE && nImageIndex != MISSION_SUC && nImageIndex != MISSION_FAI)
		return;

	if(nImageIndex == MISSION_SUC || nImageIndex == MISSION_FAI)
	{
		g_pGameMain->m_bQuestLimitTimeView = FALSE;			// 2006-09-14 by ispark ���� �Ϸ� ���̸� �ð� ���� ����
	}
	
	// 2008-03-27 by bhsohn �̼Ǽ���â�� ���ִ� ���¿����� ������ �ȿ����� ����	
	if(MISSION_SUC == nImageIndex)
	{
		// ������ ���� ����Ʈ�Ϸ� �Ǵ� ����Ʈ�� ������ �ٷ� ������ �ʴ´�. 
		// ������ Ŭ���̾�Ʈ Tick�� ���� �Ϸᰡ �Ǵ� ����Ʈ�� ������ �ݴ´�.
		// ������ ���� �Ϸ�Ǵ� ����Ʈ ������ ������ ����.
		// 1. ���̵� �Ϸ���, �Ϸ� �Ǵ� ����Ʈ
		// 2. Ư�� ���Ϳ� ���� ������ ���ؾ߸� �Ϸ�Ǵ� ����Ʈ
		g_pGameMain->SetForceCloseShop(bMissionImg);		
	}	

	// end 2008-03-27 by bhsohn �̼Ǽ���â�� ���ִ� ���¿����� ������ �ȿ����� ����

	m_nSelectImageIndex = nImageIndex;
	m_dwSelectType = dwType;
	m_fTotalTime = fTotalTime;
	m_fDelay = fDelay;
	m_nRenderPosX = x;
	m_nRenderPosY = y;
	m_bMissionImg = bMissionImg;
	m_nMissionIndex = nQuestIndex;

	if(m_nRenderPosX == 0 && m_nRenderPosY ==0)
	{
		POINT pPos = m_pImageGlobal[m_nSelectImageIndex]->GetImgSize();
		m_nRenderPosX = (g_pD3dApp->GetBackBufferDesc().Width/2)-(pPos.x/2);
		m_nRenderPosY = (g_pD3dApp->GetBackBufferDesc().Height/3)-(pPos.y/2);
	}
	
	m_bRenderFlag = TRUE;
}

//HRESULT CGlobalImage::SetResourceFile(char* szFileName)
//{
//	FLOG( "CGlobalImage::SetResourceFile(char* szFileName)" );
//	util::del(m_pGameData);
//	m_pGameData = new CGameData;
//	m_pGameData->SetFile(szFileName, FALSE, NULL, 0);
//
//	return S_OK;
//}

int CGlobalImage::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CGlobalImage::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
		}
		break;
	case WM_LBUTTONDOWN:
		{
		}
		break;
	case WM_LBUTTONUP:
		{
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-03-27 by bhsohn �̼Ǽ���â�� ���ִ� ���¿����� ������ �ȿ����� ����
/// \date		2008-03-27 ~ 2008-03-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CGlobalImage::GetSelectImageIndex()
{
	return m_nSelectImageIndex;
}