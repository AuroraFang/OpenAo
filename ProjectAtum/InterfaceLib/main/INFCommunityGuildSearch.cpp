// INFCommunityGuildSearch.cpp: implementation of the CINFCommunityGuildSearch class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "D3DHanFont.h"
#include "INFScrollBar.h"
#include "INFComboBox.h"
#include "INFGameMain.h"
#include "INFCommunity.h"
#include "GameDataLast.h"
#include "INFImageBtn.h"
#include "INFArenaScrollBar.h"
#include "AtumSound.h"
#include "INFEditBox.h"
#include "ShuttleChild.h"
#include "Chat.h"
#include "AtumDatabase.h"
#include "IMSocketManager.h"

#include "INFCommunityGuildSearch.h"

#include "INFImageEx.h"
#include "INFGroupImage.h"
#include "INFGroupManager.h"								   // 2011. 10. 10 by jskim UI�ý��� ����
#include "INFToolTip.h"			// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
#include "Interface.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Ŀ�´�Ƽ â �ɼ�
#define	COMMUNITY_SEARCHRBK_X				230
#define	COMMUNITY_SEARCHRBK_Y				0
#define	COMMUNITY_SEARCHRBK_WIDTH			348
#define	COMMUNITY_SEARCHRBK_HEIGHT			392
#define	COMMUNITY_SEARCHRBK_CAPS_HEIGHT	20

// ���� �Ұ�
#define MEMBER_INTRO_EDIT_X						19
#define MEMBER_INTRO_EDIT_Y						253
#define MEMBER_INTRO_EDIT_W						300
#define MEMBER_INTRO_FONT_LINE_HEIGHT			15
#define MEMBER_INTRO_FONT_MOUSE_HEIGHT			92
#define	MAX_MEMBER_INTRO_LINE					5


// ���� ����Ʈ ��ũ��
#define GUILD_LIST_EDIT_X						14
#define GUILD_LIST_EDIT_Y						52
#define GUILD_LIST_EDIT_W						312
#define GUILD_LIST_FONT_LINE_HEIGHT			17
#define	MAX_GUILD_LIST_LINE					10

// ����
#define GUILD_NAME_X						146
#define GUILD_NAME_Y						227



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFCommunityGuildSearch::CINFCommunityGuildSearch(CAtumNode* pParent)
{
	m_pParent = pParent;
	
	m_bShow = FALSE;
	m_bMove = FALSE;

	m_pFormat = NULL;
	m_ptCommOpBk.x = m_ptCommOpBk.y = 0;
	m_ptCommOpMouse.x = m_ptCommOpMouse.y = 0;	
	
	m_pRqBtn = NULL;			// ����
	m_pRqCancelBtn = NULL;				// ����
	
	m_pEditGuildIntro = NULL;	
	
	m_pScrollGuildIntro = NULL;
	m_pScrollGuildList = NULL;

	m_pImgSelect = NULL;
	m_nSelMemberList = -1;
	m_vecGuildSearchInfo.clear();	// ������ 

	m_pFontSel = NULL;	
	memset(&m_myShuttleIntroduceInfo, 0x00, sizeof(MSG_IC_GUILD_GET_SELF_INTRODUCTION_OK));

	m_pCloseBtn = NULL;
}

CINFCommunityGuildSearch::~CINFCommunityGuildSearch()
{
	util::del(m_pFormat);	

	util::del(m_pRqBtn);			// ����
	util::del(m_pRqCancelBtn );				// ����

	util::del(m_pEditGuildIntro);
	
	util::del(m_pScrollGuildIntro);
	util::del(m_pScrollGuildList);

	util::del(m_pImgSelect);	

	util::del(m_pFontSel);
	util::del(m_pCloseBtn);

	DelGuildSearchVector();	// ������ 	
}

void CINFCommunityGuildSearch::DelGuildSearchVector()
{
	vector<structGuildSearchInfo*>::iterator it = m_vecGuildSearchInfo.begin();	// ������ ����
	while(it != m_vecGuildSearchInfo.end())
	{
		util::del(*it);
		it++;
	}
	m_vecGuildSearchInfo.clear();

}

HRESULT CINFCommunityGuildSearch::InitDeviceObjects()
{
	DataHeader	* pDataHeader= NULL;
	
	POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();
	
	// Ŀ�´�Ƽ �ɼǹ�	
	m_ptCommOpBk.x = ptBkPos.x + COMMUNITY_SEARCHRBK_X;
	m_ptCommOpBk.y = (g_pD3dApp->GetBackBufferDesc().Height/2) - COMMUNITY_SEARCHRBK_HEIGHT/2;
	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	if(m_pFormat == NULL)
	{
		DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("B_sea");	
		m_pFormat = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pFormat->InitDeviceObjects( g_pD3dApp->m_pImageList );
		m_pFormat->RestoreDeviceObjects();
	} 
#else 
	{
		if(NULL == m_pFormat)
		{
			m_pFormat = new CINFImageEx;	
		}		
		pDataHeader = FindResource("c_sebk");
		m_pFormat->InitDeviceObjects(pDataHeader);
	}
#endif
		
	{
		// ����
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "c_jrq3");
		wsprintf(szDownBtn, "c_jrq1");
		wsprintf(szSelBtn, "c_jrq0");
		wsprintf(szDisBtn, "c_jrq2");
		if(NULL == m_pRqBtn)
		{
			m_pRqBtn = new CINFImageBtn;
		}
		// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
		//m_pRqBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
		m_pRqBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP62");
		// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
	}
	{
		// ����
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
		wsprintf(szUpBtn, "cans03");
		wsprintf(szDownBtn, "cans01");
		wsprintf(szSelBtn, "cans00");
		wsprintf(szDisBtn, "cans02");															  
#else		
		wsprintf(szUpBtn, "c_jrqc3");
		wsprintf(szDownBtn, "c_jrqc1");
		wsprintf(szSelBtn, "c_jrqc0");
		wsprintf(szDisBtn, "c_jrqc2");
#endif
		if(NULL == m_pRqCancelBtn)
		{
			m_pRqCancelBtn = new CINFImageBtn;
		}
		// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
		//m_pRqCancelBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
		m_pRqCancelBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP81");
		// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
	}
		
	
	{
		if(NULL == m_pEditGuildIntro)
		{
			m_pEditGuildIntro = new CINFEditBox;
		}
		POINT ptPos = {MEMBER_INTRO_EDIT_X, MEMBER_INTRO_EDIT_Y};		
		m_pEditGuildIntro->InitDeviceObjects(9, ptPos, MEMBER_INTRO_EDIT_W, TRUE, MEMBER_INTRO_FONT_LINE_HEIGHT, 
										FALSE, MEMBER_INTRO_FONT_MOUSE_HEIGHT);				
		m_pEditGuildIntro->SetStringMaxBuff(SIZE_MAX_NOTICE);		
	}	
	
	{
		// ���Ұ� ����Ʈ
		char  szScBall[30];	
		if(NULL == m_pScrollGuildIntro)
		{
			m_pScrollGuildIntro = new CINFArenaScrollBar;
		}		
		wsprintf(szScBall,"c_scrlb");		
		m_pScrollGuildIntro->InitDeviceObjects(MAX_MEMBER_INTRO_LINE, szScBall);
	}
	
	{
		// ���� ����Ʈ 
		char  szScBall[30];	
		if(NULL == m_pScrollGuildList)
		{
			m_pScrollGuildList = new CINFArenaScrollBar;
		}		
		wsprintf(szScBall,"c_scrlb");		
		m_pScrollGuildList->InitDeviceObjects(MAX_GUILD_LIST_LINE, szScBall);
	}
	{			
		pDataHeader = m_pGameData->Find("c_sesel");	
		if(NULL == m_pImgSelect)
		{
			m_pImgSelect = new CINFImageEx;
		}
		m_pImgSelect->InitDeviceObjects( pDataHeader );
	}
	if(NULL == m_pFontSel)
	{
		m_pFontSel = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, FALSE,128,32);
		m_pFontSel->InitDeviceObjects(g_pD3dDev);
	}	
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
		wsprintf(szUpBtn, "close");
		wsprintf(szDownBtn, "close");
		wsprintf(szSelBtn, "close");
		wsprintf(szDisBtn, "close");															  
#else							  
		wsprintf(szUpBtn, "xclose3");
		wsprintf(szDownBtn, "xclose1");
		wsprintf(szSelBtn, "xclose0");
		wsprintf(szDisBtn, "xclose2");
#endif
		if(NULL == m_pCloseBtn)
		{
			m_pCloseBtn = new CINFImageBtn;
		}
		m_pCloseBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}

	
	return S_OK ;
}

HRESULT CINFCommunityGuildSearch::RestoreDeviceObjects()
{
	if(m_pFormat )
	{
		m_pFormat->RestoreDeviceObjects();
	}		
	
	if(m_pRqBtn)
	{
		m_pRqBtn->RestoreDeviceObjects();						
	}
	if(m_pRqCancelBtn)
	{
		m_pRqCancelBtn->RestoreDeviceObjects();						
	}
		
	if(m_pEditGuildIntro)
	{
		m_pEditGuildIntro->RestoreDeviceObjects();		
	}	
	if(m_pScrollGuildIntro)
	{
		m_pScrollGuildIntro->RestoreDeviceObjects();		
	}
	if(m_pScrollGuildList)
	{
		m_pScrollGuildList->RestoreDeviceObjects();		
	}

	if(m_pImgSelect)
	{
		m_pImgSelect->RestoreDeviceObjects();
	}
	
	if(m_pFontSel)
	{
		m_pFontSel->RestoreDeviceObjects();
	}
	if(m_pCloseBtn)
	{	
		m_pCloseBtn->RestoreDeviceObjects();				
	}

	// UI���� ���� 
	UpdateUIPos();	
	
	return S_OK ;
}

HRESULT CINFCommunityGuildSearch::DeleteDeviceObjects()
{
	if(m_pFormat )
	{
		m_pFormat->DeleteDeviceObjects();
		util::del(m_pFormat );
	}		
	if(m_pRqBtn)
	{		
		m_pRqBtn->DeleteDeviceObjects();
		util::del(m_pRqBtn);
	}
	if(m_pRqCancelBtn)
	{		
		m_pRqCancelBtn->DeleteDeviceObjects();
		util::del(m_pRqCancelBtn);
	}	
	
	if(m_pEditGuildIntro)
	{
		m_pEditGuildIntro->DeleteDeviceObjects();
		util::del(m_pEditGuildIntro);		
	}	
	if(m_pScrollGuildIntro)
	{
		m_pScrollGuildIntro->DeleteDeviceObjects();
		util::del(m_pScrollGuildIntro);		
	}
	if(m_pScrollGuildList)
	{
		m_pScrollGuildList->DeleteDeviceObjects();
		util::del(m_pScrollGuildList);		
	}
	if(m_pImgSelect)
	{
		m_pImgSelect->DeleteDeviceObjects();
		util::del(m_pImgSelect);
	}
	if(m_pFontSel)
	{
		m_pFontSel->DeleteDeviceObjects();	
		util::del(m_pFontSel);
	}
	if(m_pCloseBtn)
	{
		m_pCloseBtn->DeleteDeviceObjects();	
		util::del(m_pCloseBtn);
	}

	return S_OK ;
}

HRESULT CINFCommunityGuildSearch::InvalidateDeviceObjects()
{
	if(m_pFormat)
	{
		m_pFormat->InvalidateDeviceObjects();
	}		
	if(m_pRqBtn)
	{		
		m_pRqBtn->InvalidateDeviceObjects();		
	}
	if(m_pRqCancelBtn)
	{		
		m_pRqCancelBtn->InvalidateDeviceObjects();		
	}	
	
	if(m_pEditGuildIntro)
	{
		m_pEditGuildIntro->InvalidateDeviceObjects();
	}	
	if(m_pScrollGuildIntro)
	{
		m_pScrollGuildIntro->InvalidateDeviceObjects();
	}
	if(m_pScrollGuildList)
	{
		m_pScrollGuildList->InvalidateDeviceObjects();
	}
	if(m_pImgSelect)
	{
		m_pImgSelect->InvalidateDeviceObjects();
	}
	if(m_pFontSel)
	{
		m_pFontSel->InvalidateDeviceObjects();
	}
	if(m_pCloseBtn)
	{
		m_pCloseBtn->InvalidateDeviceObjects();	
	}

	return S_OK ;
}

void CINFCommunityGuildSearch::Render()
{
	if(!IsShowWindow())
	{
		return;
	}
	POINT ptBkPos = m_ptCommOpBk;
	if(m_pFormat)
	{		
		m_pFormat->Move(ptBkPos.x, ptBkPos.y);
		m_pFormat->Render() ;
	}	
	m_pRqBtn->Render();					// ����
	m_pRqCancelBtn->Render();					// ����

	m_pScrollGuildIntro->Render();	// �ڱ�Ұ� ��ũ��
	m_pEditGuildIntro->Tick();
	m_pEditGuildIntro->Render(m_pScrollGuildIntro->GetScrollStep(), MAX_MEMBER_INTRO_LINE);	

	// ������ ����
	{
		m_pScrollGuildList->Render();		
		RenderMemberList();
	}
// 2012-12-10 by jhjang ��� �˻� â �ݱ� ��ư �ٽ� ���.
//#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����		
//#else		
	m_pCloseBtn->Render();	
//#endif
// end 2012-12-10 by jhjang ��� �˻� â �ݱ� ��ư �ٽ� ���.
	
}

void CINFCommunityGuildSearch::Tick()
{
	if(!IsShowWindow())
	{
		return;
	}

}

int CINFCommunityGuildSearch::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(!IsShowWindow())
	{
		return INF_MSGPROC_NORMAL;
	}
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);				

			{
				if(TRUE == m_pCloseBtn->OnLButtonDown(pt))
				{
					// ��ư���� ���콺�� �ִ�.
					return  INF_MSGPROC_BREAK;
				}		
			}			

			if(IsMouseCaps(pt))
			{
				m_ptCommOpMouse.x = pt.x - m_ptCommOpBk.x;
				m_ptCommOpMouse.y = pt.y - m_ptCommOpBk.y;
				m_bMove = TRUE;
				return INF_MSGPROC_BREAK;
			}						
			{
				if(TRUE == m_pRqBtn->OnLButtonDown(pt))
				{					
					return  INF_MSGPROC_BREAK;
				}		
			}
			{
				if(TRUE == m_pRqCancelBtn->OnLButtonDown(pt))
				{					
					return  INF_MSGPROC_BREAK;
				}		
			}			
						
			
			{
				BOOL bClick = m_pScrollGuildIntro->IsMouseBallPos(pt);
				if(bClick)
				{
					m_pScrollGuildIntro->SetMouseMoveMode(TRUE);
					return INF_MSGPROC_BREAK;
				}
			}
			
			{
				BOOL bClick = m_pScrollGuildList->IsMouseBallPos(pt);
				if(bClick)
				{
					m_pScrollGuildList->SetMouseMoveMode(TRUE);
					return INF_MSGPROC_BREAK;
				}
			}

			if(OnClickMemberList(pt))
			{
				return INF_MSGPROC_BREAK;
			}

			if((pt.x >= m_ptCommOpBk.x && (pt.x <= m_ptCommOpBk.x+COMMUNITY_SEARCHRBK_WIDTH))
				&& (pt.y >= m_ptCommOpBk.y && (pt.y <= m_ptCommOpBk.y+COMMUNITY_SEARCHRBK_HEIGHT)))
			{
				// ������ �������� �ִ�.
				return  INF_MSGPROC_BREAK;
			}
			
		}
		break;
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			if(m_bMove)
			{
				m_ptCommOpBk.x = pt.x - m_ptCommOpMouse.x;
				m_ptCommOpBk.y = pt.y - m_ptCommOpMouse.y;				
				// UI���� ���� 
				UpdateUIPos();
				return INF_MSGPROC_BREAK;
			}			
			
			m_pRqBtn->OnMouseMove(pt);				// ����
			m_pRqCancelBtn->OnMouseMove(pt);				// ����				
			m_pCloseBtn->OnMouseMove(pt);		
			
			{
				if(m_pScrollGuildIntro->GetMouseMoveMode())
				{
					if(FALSE == m_pScrollGuildIntro->IsMouseScrollPos(pt))
					{
						m_pScrollGuildIntro->SetMouseMoveMode(FALSE);
					}
					else
					{
						m_pScrollGuildIntro->SetScrollPos(pt);
						
						return INF_MSGPROC_BREAK;
					}
				}
			}
			
			{
				if(m_pScrollGuildList->GetMouseMoveMode())
				{
					if(FALSE == m_pScrollGuildList->IsMouseScrollPos(pt))
					{
						m_pScrollGuildList->SetMouseMoveMode(FALSE);
					}
					else
					{
						m_pScrollGuildList->SetScrollPos(pt);
						
						return INF_MSGPROC_BREAK;
					}
				}
			}

		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if(m_bMove)
			{
				m_bMove = FALSE;
				return INF_MSGPROC_BREAK;
			}									
						
			{
				if(TRUE == m_pRqBtn->OnLButtonUp(pt))
				{			
					OnClickGuildRq();

					// ��ư���� ���콺�� �ִ�.					
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
				}
			}
			{
				if(TRUE == m_pRqCancelBtn->OnLButtonUp(pt))
				{			
					OnClickGuildRqCancel();
					// ��ư���� ���콺�� �ִ�.					
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
				}
			}
// 2012-12-10 by jhjang ��� �˻� â �ݱ� ��ư �ٽ� ���.
//#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
//#else	// 2012-12-10 by jhjang ��� �˻� â �ݱ� ��ư �ٽ� ���.
			{
				if(TRUE == m_pCloseBtn->OnLButtonUp(pt))
				{					
					// ��ư Ŭ�� 	
					ShowWindow(FALSE); // �����
					return  INF_MSGPROC_BREAK;
				}
			}
//#endif
// end 2012-12-10 by jhjang ��� �˻� â �ݱ� ��ư �ٽ� ���.

			
			
			{
				BOOL bClick = m_pScrollGuildIntro->GetMouseMoveMode();
				if(bClick)
				{
					m_pScrollGuildIntro->SetMouseMoveMode(FALSE);
					return INF_MSGPROC_BREAK;
				}				
			}
			
			{
				BOOL bClick = m_pScrollGuildList->GetMouseMoveMode();
				if(bClick)
				{
					m_pScrollGuildList->SetMouseMoveMode(FALSE);
					return INF_MSGPROC_BREAK;
				}				
			}
		}
		break;
	case WM_MOUSEWHEEL:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);			
			BOOL bClick = FALSE;
						
			
			bClick = m_pScrollGuildIntro->IsMouseWhellPos(pt);
			if(bClick)		
			{			
				m_pScrollGuildIntro->OnMouseWheel(wParam, lParam);

				return INF_MSGPROC_BREAK;
			}
			
			{
				bClick = m_pScrollGuildList->IsMouseWhellPos(pt);
				if(bClick)		
				{			
					m_pScrollGuildList->OnMouseWheel(wParam, lParam);
					
					return INF_MSGPROC_BREAK;
				}
				
			}
		}
		break;		
		// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	}

	return INF_MSGPROC_NORMAL;		
}

void CINFCommunityGuildSearch::ShowWindow(BOOL bShow)
{
	m_bShow = bShow;

	m_bMove = FALSE;		// ��Ʈ���� �����δ�.
	if(bShow)
	{
		POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();

		// Ŀ�´�Ƽ �ɼǹ�	
		m_ptCommOpBk.x = ptBkPos.x + COMMUNITY_SEARCHRBK_X;
		m_ptCommOpBk.y = (g_pD3dApp->GetBackBufferDesc().Height/2) - COMMUNITY_SEARCHRBK_HEIGHT/2;
		// UI���� ���� 
		UpdateUIPos();		
		RqGuildSearchIntro();	// ���� �˻� ��û
	}
	// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
	else
	{
		g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
	}	
	// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
}

BOOL CINFCommunityGuildSearch::IsShowWindow()
{
	return m_bShow;
}

BOOL CINFCommunityGuildSearch::IsMouseCaps(POINT ptPos)
{
	if((ptPos.x >= m_ptCommOpBk.x && (ptPos.x <= m_ptCommOpBk.x+COMMUNITY_SEARCHRBK_WIDTH))
		&& (ptPos.y >= m_ptCommOpBk.y && (ptPos.y <= m_ptCommOpBk.y+COMMUNITY_SEARCHRBK_CAPS_HEIGHT)))
	{
		return TRUE;
	}
	return FALSE;

}

void CINFCommunityGuildSearch::UpdateUIPos()
{
	POINT ptBkPos = m_ptCommOpBk;

	int nPosX, nPosY;
	nPosX = nPosY = 0;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����			
	{
		nPosX = ptBkPos.x + 250;
		nPosY = ptBkPos.y + 365;		
		m_pRqBtn->SetBtnPosition(nPosX, nPosY);	
	}
	{
		nPosX = ptBkPos.x + 295;
		nPosY = ptBkPos.y + 365;
		m_pRqCancelBtn->SetBtnPosition(nPosX, nPosY);	
	}
		
	{
		nPosX = ptBkPos.x + MEMBER_INTRO_EDIT_X;
		nPosY = ptBkPos.y + MEMBER_INTRO_EDIT_Y;
		m_pEditGuildIntro->SetPos(nPosX, nPosY);
	}

	{
		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptBkPos;

		ptScrollPos.x += 330;
		ptScrollPos.y += 245;

		// ��ũ�� x = ��ġ�� -5
		// ��ũ�� height = �̹��� ������ - 34
		m_pScrollGuildIntro->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,85);
		rcMouseWhell.left		= ptScrollPos.x - 300;
		rcMouseWhell.top		= ptScrollPos.y - 30;
		rcMouseWhell.right		= ptScrollPos.x + 30;
		rcMouseWhell.bottom		= ptScrollPos.y + 78;
		m_pScrollGuildIntro->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= rcMousePos.left + 32;
		rcMousePos.bottom		= rcMousePos.top + 88;
		m_pScrollGuildIntro->SetMouseBallRect(rcMousePos);
	}
	
	{
		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptBkPos;
		
		ptScrollPos.x += 330;
		ptScrollPos.y += 51;
		
		// ��ũ�� x = ��ġ�� -5
		// ��ũ�� height = �̹��� ������ - 34
		m_pScrollGuildList->SetPosition(ptScrollPos.x ,ptScrollPos.y,11, 146);
		rcMouseWhell.left		= ptScrollPos.x - 300;
		rcMouseWhell.top		= ptScrollPos.y - 30;
		rcMouseWhell.right		= ptScrollPos.x + 30;
		rcMouseWhell.bottom		= ptScrollPos.y + 137;
		m_pScrollGuildList->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= rcMousePos.left + 32;
		rcMousePos.bottom		= rcMousePos.top + 147;
		m_pScrollGuildList->SetMouseBallRect(rcMousePos);
	}

	{
		nPosX = ptBkPos.x + 331;
		nPosY = ptBkPos.y + 3;		
		m_pCloseBtn->SetBtnPosition(nPosX, nPosY);
	}	 
#else	
	{
		nPosX = ptBkPos.x + 159;
		nPosY = ptBkPos.y + 359;		
		m_pRqBtn->SetBtnPosition(nPosX, nPosY);	
	}
	{
		nPosX = ptBkPos.x + 244;
		nPosY = ptBkPos.y + 359;		
		m_pRqCancelBtn->SetBtnPosition(nPosX, nPosY);	
	}
		
	{
		nPosX = ptBkPos.x + MEMBER_INTRO_EDIT_X;
		nPosY = ptBkPos.y + MEMBER_INTRO_EDIT_Y;		
		m_pEditGuildIntro->SetPos(nPosX, nPosY);	
	}

	{
		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptBkPos;

		ptScrollPos.x += 328;
		ptScrollPos.y += 247;

		// ��ũ�� x = ��ġ�� -5
		// ��ũ�� height = �̹��� ������ - 34
		m_pScrollGuildIntro->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,68);
		rcMouseWhell.left		= ptScrollPos.x - 300;
		rcMouseWhell.top		= ptScrollPos.y - 30;
		rcMouseWhell.right		= ptScrollPos.x + 30;
		rcMouseWhell.bottom		= ptScrollPos.y + 78;
		m_pScrollGuildIntro->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= rcMousePos.left + 32;
		rcMousePos.bottom		= rcMousePos.top + 88;
		m_pScrollGuildIntro->SetMouseBallRect(rcMousePos);
	}
	
	{
		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptBkPos;
		
		ptScrollPos.x += 328;
		ptScrollPos.y += 55;
		
		// ��ũ�� x = ��ġ�� -5
		// ��ũ�� height = �̹��� ������ - 34
		m_pScrollGuildList->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,127);
		rcMouseWhell.left		= ptScrollPos.x - 300;
		rcMouseWhell.top		= ptScrollPos.y - 30;
		rcMouseWhell.right		= ptScrollPos.x + 30;
		rcMouseWhell.bottom		= ptScrollPos.y + 137;
		m_pScrollGuildList->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= rcMousePos.left + 32;
		rcMousePos.bottom		= rcMousePos.top + 147;
		m_pScrollGuildList->SetMouseBallRect(rcMousePos);
	}

	{
		nPosX = ptBkPos.x + 333;
		nPosY = ptBkPos.y + 5;		
		m_pCloseBtn->SetBtnPosition(nPosX, nPosY);
	}
#endif
}
BOOL CINFCommunityGuildSearch::OnIMEEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, int nMaxLine, CINFEditBox* pEdit, CINFArenaScrollBar*	 pScroll)
{
	int nLienCnt = -1;
	BOOL bRtn = FALSE;
	bRtn = pEdit->WndProc(uMsg, wParam, lParam, &nLienCnt, TRUE);
	if(nLienCnt < 0)
	{
		// �ƹ� ó���� ���ߴ�.
		return FALSE;
	}				
	
	int nMaxStep = max(nLienCnt, nMaxLine);			
	
	// �ִ� �������� �����ְ�
	pScroll->SetMaxItem(nMaxStep);
	
	int nScrollStep = nMaxStep - nMaxLine;
	if(nScrollStep < 0)
	{
		nScrollStep = 0;
	}
	
	// ��ũ�� ��ġ�� ���� �Ʒ�
	pScroll->SetScrollStep(nScrollStep);		
	return bRtn;	
}

BOOL CINFCommunityGuildSearch::OnClickMemberList(POINT pt)
{
	POINT ptBkPos = m_ptCommOpBk;
	int nStartX,nStartY;
	nStartX = ptBkPos.x + GUILD_LIST_EDIT_X;
	nStartY = 0;
	if(!((nStartX <= pt.x)
		&&(pt.x <= nStartX + GUILD_LIST_EDIT_W)))
	{		
		return FALSE; 
	}
	int nItemCnt = 0;
	int nSelMemberList = -1;
	for(nItemCnt = 0;nItemCnt < MAX_GUILD_LIST_LINE; nItemCnt++)
	{
		nStartY = ptBkPos.y + GUILD_LIST_EDIT_Y +(nItemCnt*GUILD_LIST_FONT_LINE_HEIGHT);
		if((nStartY <= pt.y)&&(pt.y <= nStartY + GUILD_LIST_FONT_LINE_HEIGHT))
		{
			nSelMemberList= nItemCnt;
			break;			
		}
	}	
	
	if(nSelMemberList >= 0)
	{
		int nItenLen = (int)m_vecGuildSearchInfo.size();
		int nSelItem = nSelMemberList+m_pScrollGuildList->GetScrollStep();
		if(nSelItem < nItenLen)
		{
			m_nSelMemberList = nSelItem;
			// ����Ʈ �ڽ� ����
			structGuildSearchInfo* pTmp = m_vecGuildSearchInfo[nSelItem];
			// 2008-10-29 by bhsohn ����Ʈ �ڽ� ���� ����
			//int nCullLine = m_pEditGuildIntro->SetString(pTmp->chNotice, SIZE_MAX_NOTICE);
			int nCullLine = m_pEditGuildIntro->SetString(pTmp->chNotice, SIZE_MAX_NOTICE, TRUE);

			m_pScrollGuildIntro->SetMaxItem(nCullLine);	// ��ũ�� ���μ�
			return TRUE; 
		}
	}	
	m_nSelMemberList = -1;
	return FALSE;
}

void CINFCommunityGuildSearch::AddGuildList(UID32_t nGuildUniqueNumber, char *pGuildName, 
												char *pGuildCommanderCharacterName, ATUM_DATE_TIME	RegsterDateTime,
												char* pNotice)
{
	structGuildSearchInfo* struInfo = new structGuildSearchInfo;
	memset(struInfo, 0x00 ,sizeof(structGuildSearchInfo));
	
	struInfo->nGuildUniqueNumber = nGuildUniqueNumber;		// ��� �Ƶ�
	strncpy(struInfo->GuildName, pGuildName, SIZE_MAX_GUILD_NAME);	// ��� �̸�
	strncpy(struInfo->GuildCommanderCharacterName, pGuildCommanderCharacterName, SIZE_MAX_ARENA_FULL_NAME);	// ����� �̸�
	struInfo->RegsterDateTime = RegsterDateTime;		// ��� ����
	strncpy(struInfo->chNotice, pNotice, SIZE_MAX_NOTICE);

	m_vecGuildSearchInfo.push_back(struInfo);

	if(m_pScrollGuildList)
	{
		m_pScrollGuildList->SetMaxItem((int)m_vecGuildSearchInfo.size());
	}
}


void CINFCommunityGuildSearch::RenderMemberList()
{
	POINT ptBkPos = m_ptCommOpBk;
	int nStartX,nStartY;
	int nSelRenderItem = m_nSelMemberList- m_pScrollGuildList->GetScrollStep();
	nStartX = nStartY = 0;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	// ������ ������
	{
 		if(nSelRenderItem >= 0 && nSelRenderItem < MAX_GUILD_LIST_LINE )
 		{
			nStartX = ptBkPos.x + GUILD_LIST_EDIT_X - 3;
			nStartY = ptBkPos.y + GUILD_LIST_EDIT_Y +( nSelRenderItem * GUILD_LIST_FONT_LINE_HEIGHT ) -1;
			m_pImgSelect->Move(nStartX, nStartY);
			m_pImgSelect->Render();
 		}
 	}																							  
#else 
	// ������ ������
	{
		if(nSelRenderItem >= 0 && nSelRenderItem < MAX_GUILD_LIST_LINE )
		{
			nStartX = ptBkPos.x + GUILD_LIST_EDIT_X - 5;
			nStartY = ptBkPos.y + GUILD_LIST_EDIT_Y +(nSelRenderItem*GUILD_LIST_FONT_LINE_HEIGHT) -1;
			m_pImgSelect->Move(nStartX, nStartY);
			m_pImgSelect->Render();
		}
	}
#endif

	// �������� ����
	
	vector<structGuildSearchInfo*>::iterator it = m_vecGuildSearchInfo.begin();	// ������ ����
	int nCnt = 0;
	for(nCnt = 0;nCnt < m_pScrollGuildList->GetScrollStep();nCnt++)
	{
		if(it != m_vecGuildSearchInfo.end())
		{
			it++;
		}
	}
	
	char chBuff[128];
	memset(chBuff, 0x00, 128);
	
	for(nCnt = 0; nCnt < MAX_GUILD_LIST_LINE;nCnt++)
	{
		if(it == m_vecGuildSearchInfo.end())
		{
			break;
		}
		structGuildSearchInfo* stTmp = (*it);

		nStartX = ptBkPos.x + GUILD_LIST_EDIT_X;
		nStartY = ptBkPos.y + GUILD_LIST_EDIT_Y +(nCnt*GUILD_LIST_FONT_LINE_HEIGHT);

		// ��帶ũ
		nStartX += 4;
		CINFImage* pMark = g_pDatabase->GetGuildMark(stTmp->nGuildUniqueNumber);
		if(pMark)
		{	
			pMark->Move( nStartX, nStartY );
			pMark->Render();
		}


		// ����
		nStartX += 32;
		strncpy(chBuff, stTmp->GuildName, SIZE_MAX_GUILD_NAME);
		g_pGameMain->TextReduce(m_pFontSel, 90, chBuff);
		m_pFontSel->DrawText(nStartX, nStartY, GUI_FONT_COLOR, chBuff);

		// �����
		nStartX += 98;
		strncpy(chBuff, stTmp->GuildCommanderCharacterName, SIZE_MAX_ARENA_FULL_NAME);
		g_pGameMain->TextReduce(m_pFontSel, 80, chBuff);
		m_pFontSel->DrawText(nStartX, nStartY, GUI_FONT_COLOR, chBuff);

		// �����
		nStartX += 88;
		string szStrBuf;
		stTmp->RegsterDateTime.GetLocalString_YYYYMMDD(stTmp->RegsterDateTime.Year, 
														stTmp->RegsterDateTime.Month, 
														stTmp->RegsterDateTime.Day, szStrBuf);	

		m_pFontSel->DrawText(nStartX, nStartY, GUI_FONT_COLOR, (char*)szStrBuf.c_str());

		it++;
	}
	
	
	if(strlen(m_myShuttleIntroduceInfo.GuildName)>0)
	{		
		nStartX = ptBkPos.x + GUILD_NAME_X;
		nStartY = ptBkPos.y + GUILD_NAME_Y ;
		m_pFontSel->DrawText(nStartX, nStartY, GUI_FONT_COLOR, m_myShuttleIntroduceInfo.GuildName);
	}
}

void CINFCommunityGuildSearch::TestDB()
{
	int nCnt =0;
	ATUM_DATE_TIME	RegsterDateTime;
	char chUser[20];					// ���� ����	
	memset(chUser, 0x00, 20);	

	RegsterDateTime.Year = 2008;
	RegsterDateTime.Month = 6;
	RegsterDateTime.Day = 6;
	
	for(nCnt =0;nCnt < 20;nCnt++)
	{
		wsprintf(chUser, "User%d", nCnt);
		//AddGuildList(1, chUser,chUser, RegsterDateTime);
	}
	

}
//��� ��û
void CINFCommunityGuildSearch::OnClickGuildRq()
{	
	if(m_nSelMemberList == -1)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080611_0202,COLOR_ERROR);//"\\y��û ������ �������� �ʾҽ��ϴ�."
		return;
	}
	else if(m_nSelMemberList >= m_vecGuildSearchInfo.size())
	{
		return;
	}
	
	((CINFCommunity*)m_pParent)->OnPopupIntroMe(); // �˾�
}

void CINFCommunityGuildSearch::RqGuildSearchIntro()
{	
	DelGuildSearchVector(); // ���� �ʱ�ȭ
	// Lock
	{
		vector<MessageType_t> vecUnLockMsg;
		vecUnLockMsg.clear();
		vecUnLockMsg.push_back(T_IC_GUILD_SEARCH_INTRODUCTION);		
		vecUnLockMsg.push_back(T_IC_GUILD_SEARCH_INTRODUCTION_OK_DONE);		
		// �ϳ��� ��Ī �Ǿ ���� Ǯ����. 
		g_pD3dApp->EnterMultiLock(FALSE, T_IC_GUILD_SEARCH_INTRODUCTION, vecUnLockMsg, MULTI_LOCK_ONE_MATCHING);
	}
	// ���� ����Ʈ ���� ��û
	g_pIMSocket->SendMsg(T_IC_GUILD_SEARCH_INTRODUCTION, NULL, NULL);

}

void CINFCommunityGuildSearch::OnClickGuildRqCancel()
{
	((CINFCommunity*)m_pParent)->PopupRqCancelGuild();
}

BOOL CINFCommunityGuildSearch::GetGuildSearchSelGuidUID(UID32_t *pUID)
{
	if(m_nSelMemberList == -1)
	{
		return FALSE;
	}
	else if(m_nSelMemberList >= (int)m_vecGuildSearchInfo.size())
	{
		return FALSE;
	}
	structGuildSearchInfo* pInfo = m_vecGuildSearchInfo[m_nSelMemberList];
	(*pUID) = pInfo->nGuildUniqueNumber;

	return TRUE;

}

BOOL CINFCommunityGuildSearch::GetGuildSearchSelGuidName(char* pGuildName)
{
	if(m_nSelMemberList == -1)
	{
		return FALSE;
	}
	else if(m_nSelMemberList >= (int)m_vecGuildSearchInfo.size())
	{
		return FALSE;
	}
	structGuildSearchInfo* pInfo = m_vecGuildSearchInfo[m_nSelMemberList];
	strncpy(pGuildName, pInfo->GuildName, SIZE_MAX_GUILD_NAME);

	return TRUE;

}

void CINFCommunityGuildSearch::IMSocketGuildSearchIntroDone()
{
	// ��� �˻������� �� ������ ���� ������ �ٽ� ��û�ϳ�.
	const auto& myShuttle = g_pShuttleChild->GetMyShuttleInfo();
	((CINFCommunity*)m_pParent)->RqMemberInfo(COMMUNITY_MEMINFO_SEARCH, myShuttle.CharacterUniqueNumber);
}

void CINFCommunityGuildSearch::IMSocketGetSelfIntroductionOK(MSG_IC_GUILD_GET_SELF_INTRODUCTION_OK* pMsg)
{
	memcpy(&m_myShuttleIntroduceInfo, pMsg, sizeof(MSG_IC_GUILD_GET_SELF_INTRODUCTION_OK));
	UpdateBtnState();
}
MSG_IC_GUILD_GET_SELF_INTRODUCTION_OK* CINFCommunityGuildSearch::GetMySelfIntro()
{
	return &m_myShuttleIntroduceInfo;
}

void CINFCommunityGuildSearch::UpdateBtnState()
{
	if(strlen(m_myShuttleIntroduceInfo.GuildName) > 0)
	{
		m_pRqBtn->EnableBtn(FALSE);
		m_pRqCancelBtn->EnableBtn(TRUE);
	}
	else
	{
		m_pRqBtn->EnableBtn(TRUE);
		m_pRqCancelBtn->EnableBtn(FALSE);
	}

}