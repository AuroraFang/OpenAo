// INFMissionSelInfluence.cpp: implementation of the CINFMissionSelInfluence class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AtumApplication.h"
#include "ShuttleChild.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "D3DHanFont.h"
#include "AtumSound.h"
#include "QuestData.h"
#include "StoreData.h"
#include "ItemInfo.h"
#include "Interface.h"
#include "INFMissionMain.h"
#include "INFWindow.h"		// 2009-04-08 by bhsohn ���� �ұ���, ���� �ʱ�ȭ �ý��� �߰� ����

#include "INFMissionSelInfluence.h"


#include "INFImageEx.h"
#include "INFGroupImage.h"
#include "INFGroupManager.h"

// ���� ���� â
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
#define	SEL_INFLUENCE_BCU_POS_X								133
#define	SEL_INFLUENCE_BCU_POS_Y								480
#define	SEL_INFLUENCE_ANI_POS_X								509
#define	SEL_INFLUENCE_ANI_POS_Y								480


// �ִ� ���μ�
#define		MAX_LINE_COUNT						18
#define		MISSION_FONT_HEIGHT_GAB				17

// ��ũ�� �� 
#define		SCROLL_BALL_SCROLL_CAP				100
#define		SCROLL_BALL_WIDTH					11
#define		SCROLL_HEIGHT						355

#define		SCROLL_POS1_X						377
#define		SCROLL_POS1_Y						106

#define		SCROLL_POS2_X						747
#define		SCROLL_POS2_Y						106
#else	   
#define	SEL_INFLUENCE_BCU_POS_X								133
#define	SEL_INFLUENCE_BCU_POS_Y								488
#define	SEL_INFLUENCE_ANI_POS_X								509
#define	SEL_INFLUENCE_ANI_POS_Y								488


// �ִ� ���μ�
#define		MAX_LINE_COUNT						18
#define		MISSION_FONT_HEIGHT_GAB				17

// ��ũ�� �� 
#define		SCROLL_BALL_SCROLL_CAP				100
#define		SCROLL_BALL_WIDTH					11
#define		SCROLL_HEIGHT						355

#define		SCROLL_POS1_X						378
#define		SCROLL_POS1_Y						86

#define		SCROLL_POS2_X						748
#define		SCROLL_POS2_Y						86
#endif


		

// �ִ� ���� �ȼ�
#define MISSION_FLUNCE_MAX_DESC_STRING		312//55

// �� ���� ��ġ ����
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn �±� ���� �߰�
	#define STRING_CULL ::StringCullingUserData_ToBlank
#else
	#define STRING_CULL ::StringCullingUserDataEx
#endif // _DEBUG_endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFMissionSelInfluence::CINFMissionSelInfluence(CINFMissionMain* i_pParent)
{
	m_pParent = i_pParent;
	
	m_fBackPosX = m_fBackPosY = 0;
	m_nInfluencePosX = m_nInfluencePosY = 0;
	m_pMissionImg = NULL;
	m_pINFAlertMsgBox = NULL;

	int nCnt = 0;	
	for(nCnt = 0;nCnt < MAX_DEC_FONT;nCnt++)
	{
		m_pMissionFontfluenceL[nCnt] = NULL;
		m_pMissionFontfluenceR[nCnt] = NULL;		
	}	
	for(nCnt = 0;nCnt < MAX_INFLUENCE;nCnt++)
	{
		m_pINFScrollBar[nCnt] = NULL;
		m_pSelInfluenceBtn[nCnt] = NULL;
	}
	m_vecVCUfluenceDesc.clear();		// �̼� ����
	m_vecANIfluenceDesc.clear();		// �̼� ����

	m_nInflChoiceOver = -1;
}

CINFMissionSelInfluence::~CINFMissionSelInfluence()
{
	if(m_pINFAlertMsgBox)
	{
		m_pINFAlertMsgBox->DeleteDeviceObjects();	
		util::del(m_pINFAlertMsgBox);
	}
	if(m_pMissionImg)
	{
		m_pMissionImg->DeleteDeviceObjects();	
		util::del(m_pMissionImg);
	}
	
	int nCnt = 0;	
	for(nCnt = 0;nCnt < MAX_DEC_FONT;nCnt++)
	{
		util::del(m_pMissionFontfluenceL[nCnt]);
		util::del(m_pMissionFontfluenceR[nCnt]);
	}

	for(nCnt = 0;nCnt < MAX_INFLUENCE;nCnt++)
	{
		if(m_pINFScrollBar[nCnt])
		{
			m_pINFScrollBar[nCnt]->DeleteDeviceObjects();	
			util::del(m_pINFScrollBar[nCnt]);	
		}
		
		if(m_pSelInfluenceBtn[nCnt])
		{
			m_pSelInfluenceBtn[nCnt]->DeleteDeviceObjects();	
			util::del(m_pSelInfluenceBtn[nCnt]);	
		}
	}
}

HRESULT CINFMissionSelInfluence::InitDeviceObjects()
{
	DataHeader	* pDataHeader = NULL;
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
		
		pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("mfrc");
		m_pMissionImg = g_pGameMain->m_GruopImagemanager->GetGroupImage(pDataHeader);
		m_pMissionImg->InitDeviceObjects( g_pD3dApp->m_pImageList );				
#else 
	char buf[30] ;
		if(NULL == m_pMissionImg)
		{
			m_pMissionImg = new CINFImageEx;
		}
		
		wsprintf(buf,"mfrc_bk");
		pDataHeader = g_pGameMain->FindResource(buf);
		m_pMissionImg->InitDeviceObjects(pDataHeader);		
		
#endif
	
	}

	
	{
		m_pMissionFontfluenceL[DEC_FONT_TITLE] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,1024,32);
		m_pMissionFontfluenceL[DEC_FONT_TITLE]->InitDeviceObjects(g_pD3dDev) ;

		m_pMissionFontfluenceL[DEC_FONT_TXT] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,1024,32);
		m_pMissionFontfluenceL[DEC_FONT_TXT]->InitDeviceObjects(g_pD3dDev) ;

		m_pMissionFontfluenceR[DEC_FONT_TITLE] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,1024,32);
		m_pMissionFontfluenceR[DEC_FONT_TITLE]->InitDeviceObjects(g_pD3dDev) ;

		m_pMissionFontfluenceR[DEC_FONT_TXT] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,1024,32);
		m_pMissionFontfluenceR[DEC_FONT_TXT]->InitDeviceObjects(g_pD3dDev) ;
	}

	// ��ũ�ѹ�	
	{
		int nCnt = 0;
		char  szScBall[30];	

		for(nCnt=0;nCnt< MAX_INFLUENCE;nCnt++)
		{
			if(NULL == m_pINFScrollBar[nCnt])
			{
				m_pINFScrollBar[nCnt] = new CINFArenaScrollBar;
			}		
			wsprintf(szScBall,"c_scrlb");						
			
			m_pINFScrollBar[nCnt]->InitDeviceObjects(MAX_LINE_COUNT, szScBall);
		}
		
	}

	// ��ư 
	{		
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "frc_nr");
		wsprintf(szDownBtn, "frc_psh");
		wsprintf(szSelBtn, "frc_ov");
		wsprintf(szDisBtn, "frc_nr");

		int nCnt = 0;
		for(nCnt =0 ; nCnt < MAX_INFLUENCE;nCnt++)
		{
			if(NULL == m_pSelInfluenceBtn[nCnt])
			{
				m_pSelInfluenceBtn[nCnt] = new CINFImageBtn;
			}
			m_pSelInfluenceBtn[nCnt]->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
		}		
	}

	// �޽��� �ڽ�
	{
		if(NULL == m_pINFAlertMsgBox)
		{
			m_pINFAlertMsgBox = new CINFAlertMsgBox;
		}	
		m_pINFAlertMsgBox->InitDeviceObjects();
		
	}


	return S_OK ;
}
HRESULT CINFMissionSelInfluence::RestoreDeviceObjects()
{
	{	
		m_pMissionImg->RestoreDeviceObjects();		

		POINT ptBk = m_pMissionImg->GetImgSize();		
		
		if(g_pD3dApp->GetBackBufferDesc().Width > ptBk.x)
		{
			m_fBackPosX = (g_pD3dApp->GetBackBufferDesc().Width/2)-(ptBk.x/2);
		}
		if(g_pD3dApp->GetBackBufferDesc().Height > ptBk.y)
		{
			m_fBackPosY = (g_pD3dApp->GetBackBufferDesc().Height/2)-(ptBk.y/2);
		}	
	}
	
	
	int nCnt = 0;	
	for(nCnt = 0;nCnt < MAX_DEC_FONT;nCnt++)
	{
		m_pMissionFontfluenceL[nCnt]->RestoreDeviceObjects();
		m_pMissionFontfluenceR[nCnt]->RestoreDeviceObjects();	
	}

	for(nCnt = 0;nCnt < MAX_INFLUENCE;nCnt++)
	{
		m_pINFScrollBar[nCnt]->RestoreDeviceObjects();		
	}

	{				
		float fPosX[MAX_INFLUENCE] = 
		{
			m_fBackPosX + SEL_INFLUENCE_BCU_POS_X,
			m_fBackPosX + SEL_INFLUENCE_ANI_POS_X
		};
		float fPosY[MAX_INFLUENCE] = 
		{
			m_fBackPosY + SEL_INFLUENCE_BCU_POS_Y,
			m_fBackPosY + SEL_INFLUENCE_ANI_POS_Y		
		};

		int nCnt = 0;
		for(nCnt =0 ; nCnt < MAX_INFLUENCE;nCnt++)
		{
			m_pSelInfluenceBtn[nCnt]->RestoreDeviceObjects();		
			m_pSelInfluenceBtn[nCnt]->SetBtnPosition(fPosX[nCnt], fPosY[nCnt]);
		}
		
	}
	
	// �̼����� 
	InitInfluenceMission();

	m_pINFAlertMsgBox->RestoreDeviceObjects();

	return S_OK ;
}
HRESULT CINFMissionSelInfluence::DeleteDeviceObjects()
{
	{
		m_pMissionImg->DeleteDeviceObjects();	
		util::del(m_pMissionImg);		
	}

	{
		m_pINFAlertMsgBox->DeleteDeviceObjects();	
		util::del(m_pINFAlertMsgBox);		
	}	

	int nCnt = 0;	
	for(nCnt = 0;nCnt < MAX_DEC_FONT;nCnt++)	
	{
		m_pMissionFontfluenceL[nCnt]->DeleteDeviceObjects();
		util::del(m_pMissionFontfluenceL[nCnt]);
		m_pMissionFontfluenceR[nCnt]->DeleteDeviceObjects();
		util::del(m_pMissionFontfluenceR[nCnt]);	
	}

	for(nCnt = 0;nCnt < MAX_INFLUENCE;nCnt++)	
	{
		m_pINFScrollBar[nCnt]->DeleteDeviceObjects();	
		util::del(m_pINFScrollBar[nCnt]);
	}

	{
		int nCnt = 0;
		for(nCnt =0 ; nCnt < MAX_INFLUENCE;nCnt++)
		{
			m_pSelInfluenceBtn[nCnt]->DeleteDeviceObjects();	
			util::del(m_pSelInfluenceBtn[nCnt]);			
		}
		
	}
	
	return S_OK ;
}
HRESULT CINFMissionSelInfluence::InvalidateDeviceObjects()
{
	m_pMissionImg->InvalidateDeviceObjects();
	m_pINFAlertMsgBox->InvalidateDeviceObjects();
	
	
	int nCnt = 0;	
	for(nCnt = 0;nCnt < MAX_DEC_FONT;nCnt++)	
	{
		m_pMissionFontfluenceL[nCnt]->InvalidateDeviceObjects();
		m_pMissionFontfluenceR[nCnt]->InvalidateDeviceObjects();		
	}

	for(nCnt = 0;nCnt < MAX_INFLUENCE;nCnt++)	
	{
		m_pINFScrollBar[nCnt]->InvalidateDeviceObjects();
		m_pSelInfluenceBtn[nCnt]->InvalidateDeviceObjects();
	}
	
	
	
	return S_OK ;
}
void CINFMissionSelInfluence::Render()
{
	{
		m_pMissionImg->Move(m_fBackPosX,m_fBackPosY);
		m_pMissionImg->Render();
	}

	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_INFLUENCE;nCnt++)	
	{
		m_pSelInfluenceBtn[nCnt]->Render();
	}
	// �̼� ����
	RenderMissionInfo();

	m_pINFAlertMsgBox->Render();

}
void CINFMissionSelInfluence::Tick()
{
}

int CINFMissionSelInfluence::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	switch(uMsg)
	{
	case WM_MOUSEWHEEL:
		{
			return OnMouseWheel(wParam, lParam);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			return OnLButtonDown(wParam, lParam);			
		}
		break;
	case WM_MOUSEMOVE:
		{
			return OnMouseMove(wParam, lParam);
		}
	case WM_LBUTTONUP:
		{
			return OnLButtonUp(wParam, lParam);
		}
	}

	return INF_MSGPROC_NORMAL;	
}

void CINFMissionSelInfluence::InitInfluenceMission()
{
	m_vecVCUfluenceDesc.clear();		// �̼� ����
	m_vecANIfluenceDesc.clear();		// �̼� ����

	m_nInfluencePosX = (g_pD3dApp->GetBackBufferDesc().Width/2);
	m_nInfluencePosY = (g_pD3dApp->GetBackBufferDesc().Height/2);
	
	POINT pSize = m_pMissionImg->GetImgSize();

	m_nInfluencePosX = m_nInfluencePosX-(pSize.x/2);
	m_nInfluencePosY = m_nInfluencePosY-(pSize.y/2);

	m_vecVCUfluenceDesc.push_back(STRMSG_C_050818_0007);	// "[����]"
	STRING_CULL(STRMSG_C_050818_0001, MISSION_FLUNCE_MAX_DESC_STRING, &m_vecVCUfluenceDesc, m_pMissionFontfluenceL[DEC_FONT_TITLE]);	
	m_vecVCUfluenceDesc.push_back(STRMSG_C_050818_0008);	// "[���]"
	STRING_CULL(STRMSG_C_050818_0002, MISSION_FLUNCE_MAX_DESC_STRING, &m_vecVCUfluenceDesc, m_pMissionFontfluenceL[DEC_FONT_TITLE]);	
	m_vecVCUfluenceDesc.push_back(STRMSG_C_050818_0009);	//"[Aerial Cry]"
	STRING_CULL(STRMSG_C_050818_0003, MISSION_FLUNCE_MAX_DESC_STRING, &m_vecVCUfluenceDesc, m_pMissionFontfluenceL[DEC_FONT_TITLE]);	

	m_vecANIfluenceDesc.push_back(STRMSG_C_050818_0007);	// "[����]"
	STRING_CULL(STRMSG_C_050818_0004, MISSION_FLUNCE_MAX_DESC_STRING, &m_vecANIfluenceDesc, m_pMissionFontfluenceR[DEC_FONT_TITLE]);	
	m_vecANIfluenceDesc.push_back(STRMSG_C_050818_0008);	// "[���]"
	STRING_CULL(STRMSG_C_050818_0005, MISSION_FLUNCE_MAX_DESC_STRING, &m_vecANIfluenceDesc, m_pMissionFontfluenceR[DEC_FONT_TITLE]);		
	m_vecANIfluenceDesc.push_back(STRMSG_C_050818_0009);	//"[Aerial Cry]"
	STRING_CULL(STRMSG_C_050818_0006, MISSION_FLUNCE_MAX_DESC_STRING, &m_vecANIfluenceDesc, m_pMissionFontfluenceR[DEC_FONT_TITLE]);
	
	m_pINFScrollBar[INFLUENCE_BCU]->SetMaxItem(m_vecVCUfluenceDesc.size());
	m_pINFScrollBar[INFLUENCE_ANI]->SetMaxItem(m_vecANIfluenceDesc.size());
//	m_pScrollRightfluence->SetNumberOfData(m_vecANIfluenceDesc.size());	

	UpdateScrollPos();
}

void CINFMissionSelInfluence::RenderMissionInfo()
{	
	int nCnt = 0;
	// ����String
	{
		int nLineCnt = 0;
		
		vector<string>::iterator itL = m_vecVCUfluenceDesc.begin();
		
		for(nCnt=0;nCnt < m_pINFScrollBar[INFLUENCE_BCU]->GetScrollStep();nCnt++)
		{
			if(itL == m_vecVCUfluenceDesc.end())
			{
				break;
			}
			itL++;
		}
		while(itL != m_vecVCUfluenceDesc.end())
		{		
			if(nLineCnt >= MAX_LINE_COUNT)
			{
				break;
			}		
			char* pLString = (char*)(*itL).c_str();
			m_pMissionFontfluenceL[DEC_FONT_TXT]->DrawText(m_nInfluencePosX+40,
				m_nInfluencePosY+130+(nLineCnt*MISSION_FONT_HEIGHT_GAB),
				GUI_FONT_COLOR_W,
				pLString);	
			
			nLineCnt++;
			itL++;
		}	
	}
	// �ݱ�String
	{
		int nLineCnt = 0;
		vector<string>::iterator itR = m_vecANIfluenceDesc.begin();		
		for(nCnt=0;nCnt < m_pINFScrollBar[INFLUENCE_ANI]->GetScrollStep();nCnt++)
		{
			if(itR == m_vecANIfluenceDesc.end())
			{
				break;
			}
			itR++;
		}

		while(itR != m_vecANIfluenceDesc.end())
		{		
			if(nLineCnt >= MAX_LINE_COUNT)
			{
				break;
			}		
			char* pRString = (char*)(*itR).c_str();
			m_pMissionFontfluenceR[DEC_FONT_TXT]->DrawText(m_nInfluencePosX+410,
				m_nInfluencePosY+130+(nLineCnt*MISSION_FONT_HEIGHT_GAB),
				GUI_FONT_COLOR_W,
				pRString);	
			
			nLineCnt++;
			itR++;
		}	
	}	

	{
		for(nCnt = 0; nCnt< MAX_INFLUENCE;nCnt++)
		{
			m_pINFScrollBar[nCnt]->Render();
		}
	}
	
}

void CINFMissionSelInfluence::UpdateScrollPos()
{		
	float fPosX, fPosY;
	fPosX = m_fBackPosX;
	fPosY = m_fBackPosY;			
	
	POINT ptScroll;
	RECT rcMouseWhell, rcMousePos;
	
	POINT pSize = m_pMissionImg->GetImgSize();

	int nWidth	= (int)(pSize.x/2);
	int nHeight = (int)(pSize.y/2);

	
	int nStartX[MAX_INFLUENCE] = 
	{
		SCROLL_POS1_X,
		SCROLL_POS2_X
	};
	int nStartY[MAX_INFLUENCE] = 
	{
		SCROLL_POS1_Y,
		SCROLL_POS2_Y
	};


	int nCnt = 0;	
	for(nCnt = 0;nCnt < MAX_INFLUENCE;nCnt++)
	{
		// �� ��ġ���� 
		rcMouseWhell.left		= fPosX+(nCnt*nWidth);
		rcMouseWhell.top		= fPosY;
		rcMouseWhell.right		= rcMouseWhell.left + nWidth;
		rcMouseWhell.bottom		= rcMouseWhell.top + pSize.y;	
		
		// ���콺 ��ġ ����
		rcMousePos.left			= fPosX + nStartX[nCnt];
		rcMousePos.top			= fPosY + nStartY[nCnt];
		rcMousePos.right		= rcMousePos.left;
		rcMousePos.bottom		= rcMousePos.top;
		
		// Resotre�� �ؾ����� �̹��� ũ�⸦ �˼� �ִ�. 
		m_pINFScrollBar[nCnt]->SetPosition(rcMousePos.left, rcMousePos.top, SCROLL_BALL_WIDTH, SCROLL_HEIGHT);
		m_pINFScrollBar[nCnt]->SetMouseWhellRect(rcMouseWhell);			
		
		ptScroll = m_pINFScrollBar[nCnt]->GetImgBkSize();
		
		rcMousePos.bottom		= rcMousePos.top + ptScroll.y + SCROLL_BALL_SCROLL_CAP;
		rcMousePos.top			-= SCROLL_BALL_SCROLL_CAP;
		rcMousePos.right		= rcMousePos.left + SCROLL_BALL_SCROLL_CAP;
		rcMousePos.left			-= SCROLL_BALL_SCROLL_CAP;					
		
		m_pINFScrollBar[nCnt]->SetMouseBallRect(rcMousePos);		
	}	  
}



int CINFMissionSelInfluence::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
	
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
	CheckMouseReverse(&pt);
	int nCnt = 0;
	BOOL bClick = FALSE;	

	// �޽��� �ڽ�
	if(m_pINFAlertMsgBox->IsShowPopup())
	{
		if(INF_MSGPROC_BREAK == m_pINFAlertMsgBox->OnMouseWheel(wParam, lParam))
		{
			return INF_MSGPROC_BREAK;
		}
		return INF_MSGPROC_NORMAL;
	}
	

	for(nCnt = 0; nCnt < MAX_INFLUENCE;nCnt++)
	{
		bClick = m_pINFScrollBar[nCnt]->IsMouseWhellPos(pt);
		if(bClick)		
		{
			m_pINFScrollBar[nCnt]->OnMouseWheel(wParam, lParam);
			return INF_MSGPROC_BREAK;
			
		}		
	}
	
	return INF_MSGPROC_NORMAL;	

}
int CINFMissionSelInfluence::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);

	// �޽��� �ڽ�
	if(m_pINFAlertMsgBox->IsShowPopup())
	{
		if(INF_MSGPROC_BREAK == m_pINFAlertMsgBox->OnLButtonDown(wParam, lParam))
		{
			return INF_MSGPROC_BREAK;
		}
		return INF_MSGPROC_NORMAL;
	}
	

	int nCnt = 0;
	BOOL bClick = FALSE;	
	
	for(nCnt = 0; nCnt < MAX_INFLUENCE;nCnt++)
	{
		bClick = m_pINFScrollBar[nCnt]->IsMouseBallPos(pt);
		if(bClick)		
		{
			m_pINFScrollBar[nCnt]->SetMouseMoveMode(TRUE);
			return INF_MSGPROC_BREAK;
		}		
	}
	{
		int nCnt = 0;
		for(nCnt = 0;nCnt < MAX_INFLUENCE;nCnt++)	
		{
			if(TRUE == m_pSelInfluenceBtn[nCnt]->OnLButtonDown(pt))
			{			
				// ��ư���� ���콺�� �ִ�.
				return  INF_MSGPROC_BREAK;
			}		
		}
		
	}

	

	return INF_MSGPROC_NORMAL;	

}
int CINFMissionSelInfluence::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
	CheckMouseReverse(&pt);	

	// �޽��� �ڽ�
	if(m_pINFAlertMsgBox->IsShowPopup())
	{
		if(INF_MSGPROC_BREAK == m_pINFAlertMsgBox->OnMouseMove(wParam, lParam))
		{
			return INF_MSGPROC_BREAK;
		}
		return INF_MSGPROC_NORMAL;
	}

	int nCnt = 0;
		
	for(nCnt = 0;nCnt < MAX_INFLUENCE;nCnt++)	
	{
		m_pSelInfluenceBtn[nCnt]->OnMouseMove(pt);
	}

	// ��ũ�� 
	for(nCnt = 0; nCnt < MAX_INFLUENCE;nCnt++)
	{
		if(m_pINFScrollBar[nCnt]->GetMouseMoveMode())
		{
			if(FALSE == m_pINFScrollBar[nCnt]->IsMouseScrollPos(pt))
			{
				m_pINFScrollBar[nCnt]->SetMouseMoveMode(FALSE);
				return INF_MSGPROC_NORMAL;			
			}
			m_pINFScrollBar[nCnt]->SetScrollPos(pt);
			return INF_MSGPROC_BREAK;			
		}
	}
	
	return INF_MSGPROC_NORMAL;	
	
}
int CINFMissionSelInfluence::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);

	int nCnt = 0;
	BOOL bClick= FALSE;

	// �޽��� �ڽ�
	if(m_pINFAlertMsgBox->IsShowPopup())
	{
		BOOL bOk = FALSE;
		
		if(INF_MSGPROC_BREAK == m_pINFAlertMsgBox->OnLButtonUp(wParam, lParam, &bOk))
		{
			OnAlerertMsgBox(bOk);
			return INF_MSGPROC_BREAK;
		}
		return INF_MSGPROC_NORMAL;	
	}


	for(nCnt = 0; nCnt < MAX_INFLUENCE;nCnt++)
	{
		bClick = m_pINFScrollBar[nCnt]->GetMouseMoveMode();
		if(bClick)		
		{
			m_pINFScrollBar[nCnt]->SetMouseMoveMode(FALSE);			
		}		
	}

	{
		int nCnt = 0;
		
		for(nCnt = 0;nCnt < MAX_INFLUENCE;nCnt++)	
		{
			if(TRUE == m_pSelInfluenceBtn[nCnt]->OnLButtonUp(pt))
			{					
				OnSelectInfluence(nCnt);
				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);				
				return  INF_MSGPROC_BREAK;
			}
		}
		
	}

	return INF_MSGPROC_NORMAL;	
	
}
void CINFMissionSelInfluence::OnSelectInfluence(int nInfluence)
{
	switch(nInfluence)
	{
	case INFLUENCE_BCU:
		{
			OnSelectVCN();
		}
		break;
	case INFLUENCE_ANI:
		{
			OnSelectANI();
		}
		break;
	}
	
}

void CINFMissionSelInfluence::OnSelectVCN()
{
	// ���� ����	
	float fVCNInflDistributionPercent = m_pParent->GetVCNInflDistributionPercent();

	// 2009-02-26 by bhsohn ���¼��ý�, ������ Ȯ��â �߰���
// 	if(fVCNInflDistributionPercent > 50.0f)
// 	{
// 		if(g_pD3dApp->m_pSound)
// 		{
// 			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_OPEN_MESSAGE_BOX, g_pShuttleChild->m_vPos, FALSE);
// 		}
// 		ShowMsgBox(INFLUENCE_TYPE_VCN);
// 	}
// 	else
// 	{
// 		CQuest *pQuestInfo = g_pQuestData->GetProgressMission();
// 		if(pQuestInfo)
// 		{
// 			g_pQuestData->SendFieldSocketQuestRequestSuccess(pQuestInfo->QuestIndex,INFLUENCE_TYPE_VCN);
// 		}
// 	}	
	BOOL bOverInfluenec = FALSE;
	if(fVCNInflDistributionPercent > 50.0f)
	{		
		// 2009. 06. 03 by ckPark Ŭ���̾�Ʈ���� ���� 50%�Ѿ �����ϵ��� ����
		
// 		// 2009-04-08 by bhsohn ���� �ұ���, ���� �ʱ�ȭ �ý��� �߰� ����
// 		if(fVCNInflDistributionPercent >= MAX_INFLUENCE_PERCENT)		
// 		{
// 			if(g_pD3dApp->m_pSound)
// 			{
// 				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_OPEN_MESSAGE_BOX, g_pShuttleChild->m_vPos, FALSE);
// 			}
// 			g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_090408_0104, _MESSAGE);	
// 			return;				
// 		}
// 		// end 2009-04-08 by bhsohn ���� �ұ���, ���� �ʱ�ȭ �ý��� �߰� ����

		// end 2009. 06. 03 by ckPark Ŭ���̾�Ʈ���� ���� 50%�Ѿ �����ϵ��� ����

		bOverInfluenec = TRUE;
	}
	if(g_pD3dApp->m_pSound)
	{
		g_pD3dApp->m_pSound->PlayD3DSound(SOUND_OPEN_MESSAGE_BOX, g_pShuttleChild->m_vPos, FALSE);
	}
	ShowMsgBox(INFLUENCE_TYPE_VCN, bOverInfluenec);
	// end 2009-02-26 by bhsohn ���¼��ý�, ������ Ȯ��â �߰���
}

void CINFMissionSelInfluence::OnSelectANI()
{
	// ���� ����
	float fANIInflDistributionPercent = m_pParent->GetANIInflDistributionPercent();
	// 2009-02-26 by bhsohn ���¼��ý�, ������ Ȯ��â �߰���
// 	if(fANIInflDistributionPercent > 50.0f)
// 	{
// 		if(g_pD3dApp->m_pSound)
// 		{
// 			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_OPEN_MESSAGE_BOX, g_pShuttleChild->m_vPos, FALSE);
// 		}	
// 		ShowMsgBox(INFLUENCE_TYPE_ANI);
// 	}	
// 	else
// 	{
// 		CQuest *pQuestInfo = g_pQuestData->GetProgressMission();
// 		if(pQuestInfo)
// 		{
// 			g_pQuestData->SendFieldSocketQuestRequestSuccess(pQuestInfo->QuestIndex,INFLUENCE_TYPE_ANI);
// 		}
// 	}
	BOOL bOverInfluenec = FALSE;
	if(fANIInflDistributionPercent > 50.0f)
	{
		// 2009. 06. 03 by ckPark Ŭ���̾�Ʈ���� ���� 50%�Ѿ �����ϵ��� ����
		
// 		// 2009-04-08 by bhsohn ���� �ұ���, ���� �ʱ�ȭ �ý��� �߰� ����
// 		if(fANIInflDistributionPercent >= MAX_INFLUENCE_PERCENT)
// 		{		
// 			if(g_pD3dApp->m_pSound)
// 			{
// 				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_OPEN_MESSAGE_BOX, g_pShuttleChild->m_vPos, FALSE);
// 			}
// 			g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_090408_0104, _MESSAGE);	
// 			return;
// 		}
// 		// end 2009-04-08 by bhsohn ���� �ұ���, ���� �ʱ�ȭ �ý��� �߰� ����

		// end 2009. 06. 03 by ckPark Ŭ���̾�Ʈ���� ���� 50%�Ѿ �����ϵ��� ����

		bOverInfluenec = TRUE;		
	}	
	if(g_pD3dApp->m_pSound)
	{
		g_pD3dApp->m_pSound->PlayD3DSound(SOUND_OPEN_MESSAGE_BOX, g_pShuttleChild->m_vPos, FALSE);
	}	
	ShowMsgBox(INFLUENCE_TYPE_ANI, bOverInfluenec);	
	// end 2009-02-26 by bhsohn ���¼��ý�, ������ Ȯ��â �߰���
	
}

// 2009-02-26 by bhsohn ���¼��ý�, ������ Ȯ��â �߰���
void CINFMissionSelInfluence::ShowMsgBox(int nInflChoiceOver, BOOL bOverInfluence)
{
	if(NULL == m_pINFAlertMsgBox)
	{
		return;
	}
	char chInfluenceName[20];
	int nDistributionPercent = 0;
	float fVCNInflDistributionPercent = m_pParent->GetVCNInflDistributionPercent();
	float fANIInflDistributionPercent = m_pParent->GetANIInflDistributionPercent();
	if(nInflChoiceOver == INFLUENCE_TYPE_VCN)
	{
		sprintf(chInfluenceName, STRMSG_C_060210_0000);
		nDistributionPercent = (int)fVCNInflDistributionPercent;
	}
	else if(nInflChoiceOver == INFLUENCE_TYPE_ANI)
	{
		sprintf(chInfluenceName, STRMSG_C_060210_0001);
		nDistributionPercent = (int)fANIInflDistributionPercent;
	}
	else
	{
		return;
	}

	m_nInflChoiceOver = nInflChoiceOver;

	// ��ü �ؽ�Ʈ �����
	m_pINFAlertMsgBox->ResetContent();
		
	int nStartPosX = m_fBackPosX + 156;
	int nStartPosY = m_fBackPosY + 171;
	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����	
	int nPosX = nStartPosX + 28;
	int nPosY = nStartPosY + 67;
#else 
	int nPosX = nStartPosX + 48;
	int nPosY = nStartPosY + 77;
#endif

	int nCapY = 15;

	// 2009-02-26 by bhsohn ���¼��ý�, ������ Ȯ��â �߰���
// 	char MessageInfluenceWarning[1024];
// 	sprintf(MessageInfluenceWarning, STRMSG_C_060208_0001, (int)fVCNInflDistributionPercent, (int)fANIInflDistributionPercent); //"������ ���º��� ��Ȳ�� ������������ \\e%d\\e%%, �˸����� \\e%d\\e%%�Դϴ�."
// 	m_pINFAlertMsgBox->AddString(nPosX, nPosY, MessageInfluenceWarning);
// 	nPosY += nCapY;
// 	
// 	sprintf(MessageInfluenceWarning, STRMSG_C_060209_0000, chInfluenceName, (int)nDistributionPercent - 50); //"\\y%s\\y ������ �����̿��ݿ� \\r%d\\r%%�� �� ��� ��Ȳ�Դϴ�."
// 	m_pINFAlertMsgBox->AddString(nPosX, nPosY, MessageInfluenceWarning);
// 	nPosY += nCapY;
// 
// 	sprintf(MessageInfluenceWarning, STRMSG_C_060209_0001, chInfluenceName); //""�׷��� \\y%s\\y ������ �����Ͻðڽ��ϱ�?"
// 	m_pINFAlertMsgBox->AddString(nPosX, nPosY, MessageInfluenceWarning);
	char MessageInfluenceWarning[1024];
	if(bOverInfluence)
	{		
		sprintf(MessageInfluenceWarning, STRMSG_C_060208_0001, (int)fVCNInflDistributionPercent, (int)fANIInflDistributionPercent); //"������ ���º��� ��Ȳ�� ������������ \\e%d\\e%%, �˸����� \\e%d\\e%%�Դϴ�."
		m_pINFAlertMsgBox->AddString(nPosX, nPosY, MessageInfluenceWarning);
		nPosY += nCapY;
		
		sprintf(MessageInfluenceWarning, STRMSG_C_060209_0000, chInfluenceName, (int)nDistributionPercent - 50); //"\\y%s\\y ������ �����̿��ݿ� \\r%d\\r%%�� �� ��� ��Ȳ�Դϴ�."
		m_pINFAlertMsgBox->AddString(nPosX, nPosY, MessageInfluenceWarning);
		nPosY += nCapY;
		
		sprintf(MessageInfluenceWarning, STRMSG_C_060209_0001, chInfluenceName); //""�׷��� \\y%s\\y ������ �����Ͻðڽ��ϱ�?"
		m_pINFAlertMsgBox->AddString(nPosX, nPosY, MessageInfluenceWarning);
	}
	else
	{
		sprintf(MessageInfluenceWarning, STRMSG_C_090226_0201);	// "������ �ѹ� �����ϸ� ������ �Ұ��� �մϴ�."
		m_pINFAlertMsgBox->AddString(nPosX, nPosY, MessageInfluenceWarning);
		nPosY += nCapY;
		
		sprintf(MessageInfluenceWarning, STRMSG_C_090226_0202, chInfluenceName); //"������ \\y%s\\y �������� ���� �Ͻðڽ��ϱ�?"
		m_pINFAlertMsgBox->AddString(nPosX, nPosY, MessageInfluenceWarning);
	}
	// end 2009-02-26 by bhsohn ���¼��ý�, ������ Ȯ��â �߰���	
	m_pINFAlertMsgBox->ShowAlertWnd(TRUE, nStartPosX, nStartPosY);
	
}
void CINFMissionSelInfluence::OnAlerertMsgBox(BOOL bOk)
{	
	// ��� ��ư
	if(FALSE == bOk)
	{
		HideSelInfluence();
		return;
	}
	
	if( -1 == m_nInflChoiceOver )
	{
		return;
	}	

	CQuest *pQuestInfo = g_pQuestData->GetProgressMission();
	if(pQuestInfo)
	{
		g_pQuestData->SendFieldSocketQuestRequestSuccess(pQuestInfo->QuestIndex, m_nInflChoiceOver);
		// 2009-04-08 by bhsohn ���� �ұ���, ���� �ʱ�ȭ �ý��� �߰� ����
		HideSelInfluence();
		// end 2009-04-08 by bhsohn ���� �ұ���, ���� �ʱ�ȭ �ý��� �߰� ����
	}
	
}
void CINFMissionSelInfluence::HideSelInfluence()
{
	// ��ü �ؽ�Ʈ �����
	m_pINFAlertMsgBox->ResetContent();
	m_pINFAlertMsgBox->ShowAlertWnd(FALSE, 0, 0);
}