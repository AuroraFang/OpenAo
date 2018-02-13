// INFCommunityGuildIntroMe.cpp: implementation of the CINFCommunityGuildIntroMe class.
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
#include "INFWindow.h"
#include "IMSocketManager.h"
#include "INFCurselEditBox.h"		// 2008-10-29 by bhsohn ����Ʈ �ڽ� ���� ����

#include "INFCommunityGuildIntroMe.h"

#include "INFImageEx.h"
#include "INFGroupImage.h"
#include "INFGroupManager.h"							  // 2011. 10. 10 by jskim UI�ý��� ����
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
#define MEMBER_INTRO_EDIT_X						9
#define MEMBER_INTRO_EDIT_Y						26
//#define MEMBER_INTRO_EDIT_W						195
#define MEMBER_INTRO_EDIT_W						185
#define MEMBER_INTRO_FONT_LINE_HEIGHT			15
#define MEMBER_INTRO_FONT_MOUSE_HEIGHT			171
#define	MAX_MEMBER_INTRO_LINE					12


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFCommunityGuildIntroMe::CINFCommunityGuildIntroMe(CAtumNode* pParent)
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
	m_pCloseBtn = NULL;
	
}

CINFCommunityGuildIntroMe::~CINFCommunityGuildIntroMe()
{
	util::del(m_pFormat);	

	util::del(m_pRqBtn);			// ����
	util::del(m_pRqCancelBtn );				// ����

	util::del(m_pEditGuildIntro);	
	util::del(m_pScrollGuildIntro);	
	util::del(m_pCloseBtn);
}

HRESULT CINFCommunityGuildIntroMe::InitDeviceObjects()
{
	auto ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();
	
	// Ŀ�´�Ƽ �ɼǹ�	
	m_ptCommOpBk.x = ptBkPos.x + COMMUNITY_SEARCHRBK_X;
	m_ptCommOpBk.y = (g_pD3dApp->GetBackBufferDesc().Height/2) - COMMUNITY_SEARCHRBK_HEIGHT/2;
	
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����	
	{
		if(NULL == m_pFormat)
		{
 			m_pFormat = new CINFImageEx;
		}		
		auto pDataHeader = FindResource("c_mabk2");
 		m_pFormat->InitDeviceObjects(pDataHeader);
 	}
#else
	if(m_pFormat == NULL)
	{
		DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("B_int");	
		m_pFormat = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pFormat->InitDeviceObjects( g_pD3dApp->m_pImageList );
		m_pFormat->RestoreDeviceObjects();
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
		m_pRqBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
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
		m_pRqCancelBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
		
	
	{
		if(NULL == m_pEditGuildIntro)
		{
			// 2008-10-29 by bhsohn ����Ʈ �ڽ� ���� ����
			//m_pEditGuildIntro = new CINFEditBox;
			m_pEditGuildIntro = new CINFCurselEditBox;
			
		}
		POINT ptPos = {MEMBER_INTRO_EDIT_X, MEMBER_INTRO_EDIT_Y};		
		m_pEditGuildIntro->InitDeviceObjects(9, ptPos, MEMBER_INTRO_EDIT_W, TRUE, MEMBER_INTRO_FONT_LINE_HEIGHT, 
										FALSE, MEMBER_INTRO_FONT_MOUSE_HEIGHT);				
		m_pEditGuildIntro->SetStringMaxBuff(SIZE_MAX_NOTICE);		
		m_pEditGuildIntro->SetOnePageItemCnt(MAX_MEMBER_INTRO_LINE);	// 2008-10-29 by bhsohn ����Ʈ �ڽ� ���� ����
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

HRESULT CINFCommunityGuildIntroMe::RestoreDeviceObjects()
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
	if(m_pCloseBtn)
	{	
		m_pCloseBtn->RestoreDeviceObjects();		
	}

	// UI���� ���� 
	UpdateUIPos();	
	
	return S_OK ;
}

HRESULT CINFCommunityGuildIntroMe::DeleteDeviceObjects()
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
	if(m_pCloseBtn)
	{
		m_pCloseBtn->DeleteDeviceObjects();	
		util::del(m_pCloseBtn);
	}

	return S_OK ;
}

HRESULT CINFCommunityGuildIntroMe::InvalidateDeviceObjects()
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
	if(m_pCloseBtn)
	{
		m_pCloseBtn->InvalidateDeviceObjects();	
	}
	return S_OK ;
}

void CINFCommunityGuildIntroMe::Render()
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

	m_pCloseBtn->Render();				// �ݱ� â	
	
}

void CINFCommunityGuildIntroMe::Tick()
{
	if(!IsShowWindow())
	{
		return;
	}

}

int CINFCommunityGuildIntroMe::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
			
						
			if(TRUE == m_pEditGuildIntro->OnLButtonDown(pt))
			{				
				OnClickEditCtl(m_pEditGuildIntro);
				return INF_MSGPROC_BREAK;
			}
			// ��ü�� ���?�޴´�.
			OnClickEditCtl(NULL);

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
					OnClickRqGuild();

					// ��ư���� ���콺�� �ִ�.					
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
				}
			}
			{
				if(TRUE == m_pRqCancelBtn->OnLButtonUp(pt))
				{			
					OnClickRqCancelGuild();

					// ��ư���� ���콺�� �ִ�.					
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
				}
			}
			
			{
				if(TRUE == m_pCloseBtn->OnLButtonUp(pt))
				{					
					// ��ư Ŭ�� 	
					ShowWindow(FALSE); // �����
					return  INF_MSGPROC_BREAK;
				}
			}
			
			{
				BOOL bClick = m_pScrollGuildIntro->GetMouseMoveMode();
				if(bClick)
				{
					m_pScrollGuildIntro->SetMouseMoveMode(FALSE);
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
			
		}
		break;	
	case WM_IME_STARTCOMPOSITION:
//	case WM_IME_NOTIFY:
	case WM_IME_COMPOSITION:
	case WM_INPUTLANGCHANGE:	
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_SETCONTEXT:	
	case WM_CHAR:
	case WM_KEYDOWN:
	// 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
	case WM_KEYUP:
	// end 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
		{
			if(m_pEditGuildIntro->IsEditMode())
			{				
				int nLienCnt = -1;
				int nArrowScroll = 0;
				BOOL bRtn = FALSE;
				BOOL bStrCat =FALSE;
				bRtn = m_pEditGuildIntro->WndProc(uMsg, wParam, lParam, m_pScrollGuildIntro->GetScrollStep(), 
					&nLienCnt, &nArrowScroll, &bStrCat);
				
				if(nLienCnt < 0)
				{
					// �ƹ� ó���� ���ߴ�.
					return INF_MSGPROC_NORMAL;
				}			
				int nMaxStep = 0;			
				int nScrollStep = 0;
				switch(uMsg)
				{
				case WM_KEYDOWN:
					{
						switch(wParam)
						{
						case VK_LEFT:	// �������� �̵�
						case VK_UP:
						case VK_RIGHT:
						case VK_DOWN:
							{
								// ����Ű�� ������ ��ũ�� ��ġ ���� 
								if(nLienCnt > MAX_MEMBER_INTRO_LINE 
									&& nArrowScroll !=0)
								{
									nMaxStep = m_pScrollGuildIntro->GetMaxStepCnt();
									nScrollStep = m_pScrollGuildIntro->GetScrollStep() + nArrowScroll;
									nScrollStep = max(0, nScrollStep);						
									nScrollStep = min(nMaxStep, nScrollStep);						
									m_pScrollGuildIntro->SetScrollStep(nScrollStep);
								}
								
								return INF_MSGPROC_NORMAL;
							}
							break;
						case VK_RETURN:
							{
								return INF_MSGPROC_BREAK;
							}
							break;
						}
					}
					break;
				}
				
				nMaxStep = max(nLienCnt, MAX_MEMBER_INTRO_LINE);
				nScrollStep = nMaxStep - MAX_MEMBER_INTRO_LINE;
				if(nScrollStep < 0)
				{
					nScrollStep = 0;
				}						
				if(m_pEditGuildIntro->IsLastPos())
				{
					// ���� ������ �����̳�?					
					
					// �ִ� �������� �����ְ�
					m_pScrollGuildIntro->SetMaxItem(nMaxStep);					
					// ��Ʈ���� �ڿ� ���̴� ����� �ƴϴ�.
					// ��ũ�� ��ġ�� ���� �Ʒ�
					m_pScrollGuildIntro->SetScrollStep(nScrollStep);								
				}					
				else
				{
					m_pScrollGuildIntro->SetOnlyMaxItem(nMaxStep);
					m_pEditGuildIntro->RefreshEndCurselPos();	// ��������ġ ����
				}
				
				if(bRtn)
				{
					return INF_MSGPROC_BREAK;
				}					
				
			}
			
		}		
		break;
//		{
//			int nMaxLine = 10;
//			CINFEditBox* pEdit = NULL;
//			CINFArenaScrollBar*	 pScroll = NULL;
//			
//			if(m_pEditGuildIntro->IsEditMode())
//			{
//				pEdit = m_pEditGuildIntro;
//				pScroll = m_pScrollGuildIntro;
//				nMaxLine = MAX_MEMBER_INTRO_LINE;
//			}			
//			BOOL bRtn = FALSE;
//			if(pEdit && pScroll)
//			{
//				bRtn = OnIMEEvent(uMsg, wParam, lParam, nMaxLine, pEdit, pScroll);
//			}
//			if(bRtn)
//			{
//				return INF_MSGPROC_BREAK;
//			}			
//		}
//		break;
		// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	}

	return INF_MSGPROC_NORMAL;		
}

void CINFCommunityGuildIntroMe::ShowWindow(BOOL bShow)
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
		m_pEditGuildIntro->BackupTxtString();

	}
	else
	{
		// ��ü�� ���?�޴´�.
		OnClickEditCtl(NULL);
		// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
		g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
		// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
	}
}

BOOL CINFCommunityGuildIntroMe::IsShowWindow()
{
	return m_bShow;
}

BOOL CINFCommunityGuildIntroMe::IsMouseCaps(POINT ptPos)
{
	if((ptPos.x >= m_ptCommOpBk.x && (ptPos.x <= m_ptCommOpBk.x+COMMUNITY_SEARCHRBK_WIDTH))
		&& (ptPos.y >= m_ptCommOpBk.y && (ptPos.y <= m_ptCommOpBk.y+COMMUNITY_SEARCHRBK_CAPS_HEIGHT)))
	{
		return TRUE;
	}
	return FALSE;

}

void CINFCommunityGuildIntroMe::UpdateUIPos()
{
	POINT ptBkPos = m_ptCommOpBk;

	int nPosX, nPosY;
	nPosX = nPosY = 0;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����			
	{
		nPosX = ptBkPos.x + 160;
		nPosY = ptBkPos.y + 216;		
		m_pRqBtn->SetBtnPosition(nPosX, nPosY);	
	}
	{
		nPosX = ptBkPos.x + 195;
		nPosY = ptBkPos.y + 216;		
		m_pRqCancelBtn->SetBtnPosition(nPosX, nPosY);	
	}
		
	{
		nPosX = ptBkPos.x + MEMBER_INTRO_EDIT_X;
		nPosY = ptBkPos.y + MEMBER_INTRO_EDIT_Y;		
		m_pEditGuildIntro->SetPos(nPosX, nPosY);	
	}
	
	{
		nPosX = ptBkPos.x + 215;
		nPosY = ptBkPos.y + 4;		
		m_pCloseBtn->SetBtnPosition(nPosX, nPosY);
	}

	{
		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptBkPos;

		ptScrollPos.x += 212;
		ptScrollPos.y += 27;

		// ��ũ�� x = ��ġ�� -5
		// ��ũ�� height = �̹��� ������ - 34
		m_pScrollGuildIntro->SetPosition(ptScrollPos.x ,ptScrollPos.y,14,149);
		rcMouseWhell.left		= ptScrollPos.x - 300;
		rcMouseWhell.top		= ptScrollPos.y - 30;
		rcMouseWhell.right		= ptScrollPos.x + 30;
		rcMouseWhell.bottom		= ptScrollPos.y + 143;
		m_pScrollGuildIntro->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= rcMousePos.left + 32;
		rcMousePos.bottom		= rcMousePos.top + 153;
		m_pScrollGuildIntro->SetMouseBallRect(rcMousePos);
	}  
#else 
	{
		nPosX = ptBkPos.x + 35;
		nPosY = ptBkPos.y + 216;		
		m_pRqBtn->SetBtnPosition(nPosX, nPosY);	
	}
	{
		nPosX = ptBkPos.x + 120;
		nPosY = ptBkPos.y + 216;		
		m_pRqCancelBtn->SetBtnPosition(nPosX, nPosY);	
	}
		
	{
		nPosX = ptBkPos.x + MEMBER_INTRO_EDIT_X;
		nPosY = ptBkPos.y + MEMBER_INTRO_EDIT_Y;		
		m_pEditGuildIntro->SetPos(nPosX, nPosY);	
	}
	
	{
		nPosX = ptBkPos.x + 212;
		nPosY = ptBkPos.y + 5;		
		m_pCloseBtn->SetBtnPosition(nPosX, nPosY);
	}

	{
		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptBkPos;

		ptScrollPos.x += 212;
		ptScrollPos.y += 27;

		// ��ũ�� x = ��ġ�� -5
		// ��ũ�� height = �̹��� ������ - 34
		m_pScrollGuildIntro->SetPosition(ptScrollPos.x ,ptScrollPos.y,14,136);
		rcMouseWhell.left		= ptScrollPos.x - 300;
		rcMouseWhell.top		= ptScrollPos.y - 30;
		rcMouseWhell.right		= ptScrollPos.x + 30;
		rcMouseWhell.bottom		= ptScrollPos.y + 143;
		m_pScrollGuildIntro->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= rcMousePos.left + 32;
		rcMousePos.bottom		= rcMousePos.top + 153;
		m_pScrollGuildIntro->SetMouseBallRect(rcMousePos);
	}
#endif
	
}
//BOOL CINFCommunityGuildIntroMe::OnIMEEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, int nMaxLine, CINFCurselEditBox* pEdit, CINFArenaScrollBar*	 pScroll)
//{
//	int nLienCnt = -1;
//	BOOL bRtn = FALSE;
//	//bRtn = pEdit->WndProc(uMsg, wParam, lParam, &nLienCnt, TRUE);
//	bRtn = pEdit->WndProc(uMsg, wParam, lParam, pScroll->GetScrollStep(),	&nLienCnt, TRUE);
//	if(nLienCnt < 0)
//	{
//		// �ƹ� ó���� ���ߴ�.
//		return FALSE;
//	}				
//	
//	int nMaxStep = max(nLienCnt, nMaxLine);			
//	
//	// �ִ� �������� �����ְ�
//	pScroll->SetMaxItem(nMaxStep);
//	
//	int nScrollStep = nMaxStep - nMaxLine;
//	if(nScrollStep < 0)
//	{
//		nScrollStep = 0;
//	}
//	
//	// ��ũ�� ��ġ�� ���� �Ʒ�
//	pScroll->SetScrollStep(nScrollStep);		
//	return bRtn;	
//}

void CINFCommunityGuildIntroMe::OnClickEditCtl(CINFCurselEditBox*	pSelEdit)
{	
	if(m_pEditGuildIntro->IsEditMode())
	{
		m_pEditGuildIntro->BackupTxtString();
	}

	m_pEditGuildIntro->EnableEdit(FALSE, FALSE);		// �ڱ�Ұ�
	if(pSelEdit)
	{
		pSelEdit->EnableEdit(TRUE, TRUE);
	}
}

// ���Խ�û
void CINFCommunityGuildIntroMe::OnClickRqGuild()
{
	char chBuff[256], chGuildName[SIZE_MAX_GUILD_NAME];
	memset(chBuff, 0x00, 256);
	memset(chGuildName, 0x00, SIZE_MAX_GUILD_NAME);
	if(!((CINFCommunity*)m_pParent)->GetGuildSearchSelGuidName(chGuildName))
	{
		return;
	}	
	wsprintf(chBuff,STRMSG_C_080527_0203, chGuildName);
	g_pGameMain->m_pInfWindow->AddMsgBox(chBuff, _Q_GUILD_REQUEST);	// "\\y%s\\y���ܿ� ���� ��û �Ͻðڽ��ϱ�?"
}

void CINFCommunityGuildIntroMe::RqGuildIntroMe()
{		
	MSG_IC_GUILD_UPDATE_SELFINTRODUCTION sMsg;
	memset(&sMsg,0x00,sizeof(MSG_IC_GUILD_UPDATE_SELFINTRODUCTION));	
	 
	if(!((CINFCommunity*)m_pParent)->GetGuildSearchSelGuidUID(&sMsg.TargetGuildUID))
	{
		return;
	}
	m_pEditGuildIntro->GetString(sMsg.SelfIntroduction , SIZE_MAX_NOTICE);
	g_pIMSocket->SendMsg(T_IC_GUILD_UPDATE_SELFINTRODUCTION, (char*)&sMsg, sizeof(sMsg) );

	// Lock
	{
		vector<MessageType_t> vecUnLockMsg;
		vecUnLockMsg.clear();
		vecUnLockMsg.push_back(T_IC_GUILD_UPDATE_SELFINTRODUCTION);		
		vecUnLockMsg.push_back(T_IC_GUILD_UPDATE_SELFINTRODUCTION_OK);		
		// �ϳ��� ��Ī �Ǿ ���� Ǯ����. 
		g_pD3dApp->EnterMultiLock(FALSE, T_IC_GUILD_UPDATE_SELFINTRODUCTION, vecUnLockMsg, MULTI_LOCK_ONE_MATCHING);
	}
	ShowWindow(FALSE);
	((CINFCommunity*)m_pParent)->ShowGuildSearch(FALSE);

}

// ���Խ�û ���
void CINFCommunityGuildIntroMe::OnClickRqCancelGuild()
{
	//((CINFCommunity*)m_pParent)->PopupRqCancelGuild();	
	ShowWindow(FALSE);
}


