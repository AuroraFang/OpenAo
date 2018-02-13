// INFCommunityUserOpenSetup.cpp: implementation of the CINFCommunityUserOpenSetup class.
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
#include "INFCommunity.h"
#include "INFImageRadioBtn.h"
#include "INFListBox.h"
#include "INFSecuEditBox.h"
#include "INFNumEditBox.h"
#include "INFEditBox.h"
#include "ClientParty.h"

#include "INFCommunityUserOpenSetup.h"
#include "INFImageEx.h"								// 2011. 10. 10 by jskim UI�ý��� ����
#include "INFGroupImage.h"
#include "INFGroupManager.h"
#include "INFToolTip.h"								// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
// Ŀ�´�Ƽ â �ɼ�
#define	COMMUNITY_OPENBK_X				230
#define	COMMUNITY_OPENBK_Y				0
#define	COMMUNITY_OPENBK_WIDTH			471
#define	COMMUNITY_OPENBK_HEIGHT			238
#define	COMMUNITY_OPENBK_CAPS_HEIGHT	20
#define	COMMUNITY_OPENBK_INFO_X			695

// 2012-07-12 by isshin ���۹̸����� On / Off ��� - ���� ����
#ifdef	C_ENEMY_INFO_ISSHIN
// Ȯ�� 
	#define	COMMUNITY_OPENE_OK_X			216
	#ifdef	C_ITEM_VIEW_UNCONDITIONAL_ON
		#define	COMMUNITY_OPENE_OK_Y			211 //240 // 2012-10-17 by bhsohn ������ �̸����� ������ ON���� ���� 211
	#else
		#define	COMMUNITY_OPENE_OK_Y			240 
	#endif
// �ݱ�
	#define	COMMUNITY_OPENE_CLOSE_X			283
	#ifdef	C_ITEM_VIEW_UNCONDITIONAL_ON
		#define	COMMUNITY_OPENE_CLOSE_Y			211 // 240 // 2012-10-17 by bhsohn ������ �̸����� ������ ON���� ���� 211
	#else
		#define	COMMUNITY_OPENE_CLOSE_Y			240
	#endif

	// ���� ���� ����
	#define	RADIO_GUILD_POS_X				122
	#define	RADIO_GUILD_POS_Y				68
	// �Ϲ� ���� ����
	#define	RADIO_NORMAL_POS_X				366
	#define	RADIO_NORMAL_POS_Y				68
#else
	#define	COMMUNITY_OPENE_OK_X			169
	#define	COMMUNITY_OPENE_OK_Y			211
	#define	COMMUNITY_OPENE_CLOSE_X			236
#define	COMMUNITY_OPENE_CLOSE_Y			211
	#define	RADIO_GUILD_POS_X				92
	#define	RADIO_GUILD_POS_Y				68
	#define	RADIO_NORMAL_POS_X				309	
	#define	RADIO_NORMAL_POS_Y				68
#endif
// end 2012-07-12 by isshin ���۹̸����� On / Off ��� - ���� ����

#define	COMMUNITY_OPENE_XCLOSE_X			455
#define	COMMUNITY_OPENE_XCLOSE_Y			3

#define	RADIO_POS_CAP_X			66
#define	RADIO_POS_CAP_Y			25
#else
// Ŀ�´�Ƽ â �ɼ�
#define	COMMUNITY_OPENBK_X				230
#define	COMMUNITY_OPENBK_Y				0
#define	COMMUNITY_OPENBK_WIDTH			471
#define	COMMUNITY_OPENBK_HEIGHT			238
#define	COMMUNITY_OPENBK_CAPS_HEIGHT	20
#define	COMMUNITY_OPENBK_INFO_X			695

// Ȯ�� 
#define	COMMUNITY_OPENE_OK_X				169
#define	COMMUNITY_OPENE_OK_Y				211
// �ݱ�
#define	COMMUNITY_OPENE_CLOSE_X			236
#define	COMMUNITY_OPENE_CLOSE_Y			211

#define	COMMUNITY_OPENE_XCLOSE_X			458
#define	COMMUNITY_OPENE_XCLOSE_Y			5

// ���� ���� ����
#define	RADIO_GUILD_POS_X				92
#define	RADIO_GUILD_POS_Y				68
// �Ϲ� ���� ����
#define	RADIO_NORMAL_POS_X				309
#define	RADIO_NORMAL_POS_Y				68

#define	RADIO_POS_CAP_X			66
#define	RADIO_POS_CAP_Y			25
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFCommunityUserOpenSetup::CINFCommunityUserOpenSetup(CAtumNode* pParent)
{
	m_pParent = pParent;
	
	m_bShow = FALSE;
	m_bMove = FALSE;

	m_pBkImage = NULL;
	m_ptCommOpBk.x = m_ptCommOpBk.y = 0;
	m_ptCommOpMouse.x = m_ptCommOpMouse.y = 0;

	m_pCloseBtn = NULL;
	m_pXCloseBtn = NULL;
	m_pOkBtn = NULL;

	// ���ӿɼ�	
	int nCnt = 0;	
	int nMainId = 0;
	for(nMainId = 0;nMainId < MAX_OPENSETUP_INFO;nMainId++)
	{
		m_bGuildInfo[nMainId] = TRUE;
		m_bNormalInfo[nMainId] = TRUE;
		for(nCnt = 0;nCnt < MAX_OPENSETUP_CNT;nCnt++)
		{
			m_pRadioGuildInfo[nMainId][nCnt] = NULL;
			m_pRadioUserInfo[nMainId][nCnt] = NULL;
		}
	}	
}

CINFCommunityUserOpenSetup::~CINFCommunityUserOpenSetup()
{
	util::del(m_pBkImage);
		
	util::del(m_pCloseBtn);
	util::del(m_pXCloseBtn);
	util::del(m_pOkBtn);	
	int nCnt = 0;	
	int nMainId = 0;
	for(nMainId = 0;nMainId < MAX_OPENSETUP_INFO;nMainId++)
	{
		for(nCnt = 0;nCnt < MAX_OPENSETUP_CNT;nCnt++)
		{		
			util::del(m_pRadioGuildInfo[nMainId][nCnt]);		
			util::del(m_pRadioUserInfo[nMainId][nCnt]);		
		}
		
	}	
}

HRESULT CINFCommunityUserOpenSetup::InitDeviceObjects()
{
	DataHeader	* pDataHeader= NULL;
	int nCnt = 0;

	POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();

	// Ŀ�´�Ƽ �ɼǹ�	
	m_ptCommOpBk.x = ptBkPos.x + COMMUNITY_OPENBK_X;
	m_ptCommOpBk.y = (g_pD3dApp->GetBackBufferDesc().Height/2) - COMMUNITY_OPENBK_HEIGHT/2;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("open");	
	m_pBkImage = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
	m_pBkImage->InitDeviceObjects( g_pD3dApp->m_pImageList );

	{
		// Ȯ��
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "oks03");
		wsprintf(szDownBtn, "oks01");
		wsprintf(szSelBtn, "oks00");
		wsprintf(szDisBtn, "oks02");
		if(NULL == m_pOkBtn)
		{
			m_pOkBtn = new CINFImageBtn;
		}
		m_pOkBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	
	{
		// �ݱ�
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "cans03");
		wsprintf(szDownBtn, "cans01");
		wsprintf(szSelBtn, "cans00");
		wsprintf(szDisBtn, "cans02");
		if(NULL == m_pCloseBtn)
		{
			m_pCloseBtn = new CINFImageBtn;
		}
		m_pCloseBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	{
		// �ݱ�
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "close");
		wsprintf(szDownBtn, "close");
		wsprintf(szSelBtn, "close");
		wsprintf(szDisBtn, "close");
		if(NULL == m_pXCloseBtn)
		{
			m_pXCloseBtn = new CINFImageBtn;
		}
		m_pXCloseBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
#else
	{
		if(NULL == m_pBkImage)
		{
			m_pBkImage = new CINFImageEx;
		}		
		pDataHeader = FindResource("openbk");
		m_pBkImage->InitDeviceObjects(pDataHeader);
	}		
	{
		// Ȯ��
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "lr_ok3");
		wsprintf(szDownBtn, "lr_ok1");
		wsprintf(szSelBtn, "lr_ok0");
		wsprintf(szDisBtn, "lr_ok2");
		if(NULL == m_pOkBtn)
		{
			m_pOkBtn = new CINFImageBtn;
		}
		m_pOkBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}

	{
		// �ݱ�
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "lr_can3");
		wsprintf(szDownBtn, "lr_can1");
		wsprintf(szSelBtn, "lr_can0");
		wsprintf(szDisBtn, "lr_can2");
		if(NULL == m_pCloseBtn)
		{
			m_pCloseBtn = new CINFImageBtn;
		}
		m_pCloseBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	{
		// �ݱ�
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "xclose3");
		wsprintf(szDownBtn, "xclose1");
		wsprintf(szSelBtn, "xclose0");
		wsprintf(szDisBtn, "xclose2");
		if(NULL == m_pXCloseBtn)
		{
			m_pXCloseBtn = new CINFImageBtn;
		}
		m_pXCloseBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
#endif
		

	
	{		
		char chRadioOff[30], chRadioOn[30];
		
		wsprintf(chRadioOff, "radio_17b");
		wsprintf(chRadioOn, "radio_17a");
				
		
		int nMainId = 0;
		for(nMainId = 0;nMainId < MAX_OPENSETUP_INFO;nMainId++)
		{
			for(nCnt = 0;nCnt < MAX_OPENSETUP_CNT;nCnt++)
			{	
				// ������
				if(NULL == m_pRadioGuildInfo[nMainId][nCnt])
				{		
					m_pRadioGuildInfo[nMainId][nCnt] = new CINFImageRadioBtn;
				}				
				m_pRadioGuildInfo[nMainId][nCnt]->InitDeviceObjects(chRadioOff, chRadioOn);

				// �Ϲ� ����
				if(NULL == m_pRadioUserInfo[nMainId][nCnt])
				{		
					m_pRadioUserInfo[nMainId][nCnt] = new CINFImageRadioBtn;
				}				
				m_pRadioUserInfo[nMainId][nCnt]->InitDeviceObjects(chRadioOff, chRadioOn);
			}
			
		}		
	}
		
		
	return S_OK ;
}

HRESULT CINFCommunityUserOpenSetup::RestoreDeviceObjects()
{
	if(m_pBkImage )
	{
		m_pBkImage->RestoreDeviceObjects();
	}

	if(m_pCloseBtn)
	{
		m_pCloseBtn->RestoreDeviceObjects();						
	}
	if(m_pXCloseBtn)
	{
		m_pXCloseBtn->RestoreDeviceObjects();						
	}
	if(m_pOkBtn)
	{
		m_pOkBtn->RestoreDeviceObjects();						
	}

	int nCnt = 0;
	int nMainId = 0;
	for(nMainId = 0;nMainId < MAX_OPENSETUP_INFO;nMainId++)
	{			
		for(nCnt = 0;nCnt < MAX_OPENSETUP_CNT;nCnt++)
		{	
			if(m_pRadioGuildInfo[nMainId][nCnt])
			{
				m_pRadioGuildInfo[nMainId][nCnt]->RestoreDeviceObjects();			
			}
			
			if(m_pRadioUserInfo[nMainId][nCnt])
			{
				m_pRadioUserInfo[nMainId][nCnt]->RestoreDeviceObjects();			
			}
		}			
	}	
	
	// UI���� ���� 
	UpdateUIPos();

	
	return S_OK ;
}

HRESULT CINFCommunityUserOpenSetup::DeleteDeviceObjects()
{
	if(m_pBkImage )
	{
		m_pBkImage->DeleteDeviceObjects();
		util::del(m_pBkImage );
	}
	if(m_pCloseBtn)
	{		
		m_pCloseBtn->DeleteDeviceObjects();
		util::del(m_pCloseBtn);
	}
	if(m_pXCloseBtn)
	{		
		m_pXCloseBtn->DeleteDeviceObjects();
		util::del(m_pXCloseBtn);
	}
	if(m_pOkBtn)
	{		
		m_pOkBtn->DeleteDeviceObjects();
		util::del(m_pOkBtn);
	}
	int nCnt = 0;	
	int nMainId = 0;
	for(nMainId = 0;nMainId < MAX_OPENSETUP_INFO;nMainId++)
	{
		for(nCnt = 0;nCnt < MAX_OPENSETUP_CNT;nCnt++)
		{		
			if(m_pRadioGuildInfo[nMainId][nCnt])
			{
				m_pRadioGuildInfo[nMainId][nCnt]->DeleteDeviceObjects();	
				util::del(m_pRadioGuildInfo[nMainId][nCnt]);
			}
			
			if(m_pRadioUserInfo[nMainId][nCnt])
			{
				m_pRadioUserInfo[nMainId][nCnt]->DeleteDeviceObjects();	
				util::del(m_pRadioUserInfo[nMainId][nCnt]);
			}
		}			
	}	

	return S_OK ;
}

HRESULT CINFCommunityUserOpenSetup::InvalidateDeviceObjects()
{
	if(m_pBkImage)
	{
		m_pBkImage->InvalidateDeviceObjects();
	}
	if(m_pCloseBtn)
	{		
		m_pCloseBtn->InvalidateDeviceObjects();		
	}
	if(m_pXCloseBtn)
	{
		m_pXCloseBtn->InvalidateDeviceObjects();		
	}
	if(m_pOkBtn)
	{		
		m_pOkBtn->InvalidateDeviceObjects();		
	}
	int nCnt = 0;	
	int nMainId = 0;
	for(nMainId = 0;nMainId < MAX_OPENSETUP_INFO;nMainId++)
	{
		for(nCnt = 0;nCnt < MAX_OPENSETUP_CNT;nCnt++)
		{		
			if(m_pRadioGuildInfo[nMainId][nCnt])
			{
				m_pRadioGuildInfo[nMainId][nCnt]->InvalidateDeviceObjects();
			}			
			if(m_pRadioUserInfo[nMainId][nCnt])
			{
				m_pRadioUserInfo[nMainId][nCnt]->InvalidateDeviceObjects();
			}
		}		
	}
	
	return S_OK ;
}

void CINFCommunityUserOpenSetup::Render()
{
	if(!IsShowWindow())
	{
		return;
	}
	POINT ptBkPos = m_ptCommOpBk;
	if(m_pBkImage)
	{		
		m_pBkImage->Move(ptBkPos.x, ptBkPos.y);
		m_pBkImage->Render() ;
	}
	
	m_pCloseBtn->Render();		
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
#else
	m_pXCloseBtn->Render();		
#endif
	m_pOkBtn->Render();		

	int nCnt =0;	
	int nMainId = 0;
	for(nMainId = 0;nMainId < MAX_OPENSETUP_INFO;nMainId++)
	{
		for(nCnt =0; nCnt < MAX_OPENSETUP_CNT; nCnt++)
		{		
			m_pRadioGuildInfo[nMainId][nCnt]->Render();
			m_pRadioUserInfo[nMainId][nCnt]->Render();
		}
		
	}	
	
}

void CINFCommunityUserOpenSetup::Tick()
{
	if(!IsShowWindow())
	{
		return;
	}	

}

int CINFCommunityUserOpenSetup::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
				if(TRUE == m_pXCloseBtn->OnLButtonDown(pt))
				{					
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
				if(TRUE == m_pOkBtn->OnLButtonDown(pt))
				{						
					return  INF_MSGPROC_BREAK;				
				}
			}
			{
				if(TRUE == m_pCloseBtn->OnLButtonDown(pt))
				{					
					return  INF_MSGPROC_BREAK;
				}		
			}			
			
			int nCnt = 0;
			int nMainId = 0;			
			for(nMainId = 0;nMainId < MAX_OPENSETUP_INFO;nMainId++)
			{		
				// ���� ��� ����		
				for(nCnt = 0;nCnt < MAX_OPENSETUP_CNT;nCnt++)
				{
					if(TRUE == m_pRadioGuildInfo[nMainId][nCnt]->OnLButtonDown(pt))
					{						
						OnClickGuildInfo(nMainId, nCnt);
						return  INF_MSGPROC_BREAK;
					}		
					if(TRUE == m_pRadioUserInfo[nMainId][nCnt]->OnLButtonDown(pt))
					{						
						OnClickNormalInfo(nMainId, nCnt);
						return  INF_MSGPROC_BREAK;
					}		
				}
			}
			
						
			// 		
			
			if((pt.x >= m_ptCommOpBk.x && (pt.x <= m_ptCommOpBk.x+COMMUNITY_OPENBK_WIDTH))
				&& (pt.y >= m_ptCommOpBk.y && (pt.y <= m_ptCommOpBk.y+COMMUNITY_OPENBK_HEIGHT)))
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
			int nCnt = 0;								

			if(m_bMove)
			{
				m_ptCommOpBk.x = pt.x - m_ptCommOpMouse.x;
				m_ptCommOpBk.y = pt.y - m_ptCommOpMouse.y;				
				// UI���� ���� 
				UpdateUIPos();
				return INF_MSGPROC_BREAK;
			}			
			m_pCloseBtn->OnMouseMove(pt);
			m_pXCloseBtn->OnMouseMove(pt);
			m_pOkBtn->OnMouseMove(pt);	
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
				if(TRUE == m_pCloseBtn->OnLButtonUp(pt))
				{			
					// ��ư���� ���콺�� �ִ�.
					ShowWindow(FALSE, FALSE);					
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
				}
			}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
#else			
			{
				if(TRUE == m_pXCloseBtn->OnLButtonUp(pt))
				{			
					// ��ư���� ���콺�� �ִ�.
					ShowWindow(FALSE, FALSE);					
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
				}
			}
#endif
			{
				if(TRUE == m_pOkBtn->OnLButtonUp(pt))
				{						
					OnApply();
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
				}
			}			
		}
		break;		
	case WM_KEYDOWN:
		{
			if(wParam == VK_ESCAPE)
			{
				ShowWindow(FALSE, FALSE);
				return  INF_MSGPROC_BREAK;				
			}
		}
	}

	return INF_MSGPROC_NORMAL;		
}

void CINFCommunityUserOpenSetup::ShowWindow(BOOL bShow, BOOL bCharacter)
{
	m_bShow = bShow;

	m_bMove = FALSE;		// ��Ʈ���� �����δ�.
	if(bShow)
	{
//		POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();

		// Ŀ�´�Ƽ �ɼǹ�	
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
		if(bCharacter)
		{
			m_ptCommOpBk.x = (g_pD3dApp->GetBackBufferDesc().Width / 2) - ( m_pBkImage->GetImgSize().x / 2 );
		}
		else
		{
			POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();
			m_ptCommOpBk.x = ptBkPos.x + COMMUNITY_OPENBK_X;
		}
#endif
		
		m_ptCommOpBk.y = (g_pD3dApp->GetBackBufferDesc().Height/2) - COMMUNITY_OPENBK_HEIGHT/2;

		InitControl();
		// UI���� ���� 
		UpdateUIPos();		
		
	}	
	// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
	else
	{
		g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
	}
	// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
}

BOOL CINFCommunityUserOpenSetup::IsShowWindow()
{
	return m_bShow;
}

BOOL CINFCommunityUserOpenSetup::IsMouseCaps(POINT ptPos)
{
	if((ptPos.x >= m_ptCommOpBk.x && (ptPos.x <= m_ptCommOpBk.x+COMMUNITY_OPENBK_WIDTH))
		&& (ptPos.y >= m_ptCommOpBk.y && (ptPos.y <= m_ptCommOpBk.y+COMMUNITY_OPENBK_CAPS_HEIGHT)))
	{
		return TRUE;
	}
	return FALSE;

}

void CINFCommunityUserOpenSetup::UpdateUIPos()
{
	POINT ptBkPos = m_ptCommOpBk;

	int nCnt = 0;						
	int nPosX, nPosY;
	nPosX = nPosY = 0;
	
	{
		nPosX = ptBkPos.x + COMMUNITY_OPENE_CLOSE_X;
		nPosY = ptBkPos.y + COMMUNITY_OPENE_CLOSE_Y;		
		m_pCloseBtn->SetBtnPosition(nPosX, nPosY);	
	}
	
	{
		nPosX = ptBkPos.x + COMMUNITY_OPENE_XCLOSE_X;
		nPosY = ptBkPos.y + COMMUNITY_OPENE_XCLOSE_Y;		
		m_pXCloseBtn->SetBtnPosition(nPosX, nPosY);	
	}
	{
		nPosX = ptBkPos.x + COMMUNITY_OPENE_OK_X;
		nPosY = ptBkPos.y + COMMUNITY_OPENE_OK_Y;		
		m_pOkBtn->SetBtnPosition(nPosX, nPosY);	
	}	

	int nMainId = 0;
	for(nMainId = 0;nMainId < MAX_OPENSETUP_INFO;nMainId++)
	{		
		// ���� ��� ����		
		for(nCnt = 0;nCnt < MAX_OPENSETUP_CNT;nCnt++)
		{
			nPosX = ptBkPos.x + RADIO_GUILD_POS_X+(nCnt*RADIO_POS_CAP_X);
			nPosY = ptBkPos.y + RADIO_GUILD_POS_Y+(nMainId*RADIO_POS_CAP_Y);		
			m_pRadioGuildInfo[nMainId][nCnt]->SetPosition(nPosX, nPosY, 60);		

			nPosX = ptBkPos.x + RADIO_NORMAL_POS_X+(nCnt*RADIO_POS_CAP_X);
			nPosY = ptBkPos.y + RADIO_NORMAL_POS_Y+(nMainId*RADIO_POS_CAP_Y);		
			m_pRadioUserInfo[nMainId][nCnt]->SetPosition(nPosX, nPosY, 60);		
		}		
	}		
	
	UpdateRadioBtn();
}

void CINFCommunityUserOpenSetup::UpdateRadioBtn()
{
	// �ʴ���		 	
	int nMainId = 0;
	for(nMainId = 0;nMainId < MAX_OPENSETUP_INFO;nMainId++)
	{
		// ��� ����
		if(m_bGuildInfo[nMainId])
		{
			m_pRadioGuildInfo[nMainId][OPENSETUP_ON]->SetRadioBtn(TRUE);
			m_pRadioGuildInfo[nMainId][OPENSETUP_OFF]->SetRadioBtn(FALSE);
		}
		else
		{
			m_pRadioGuildInfo[nMainId][OPENSETUP_ON]->SetRadioBtn(FALSE);
			m_pRadioGuildInfo[nMainId][OPENSETUP_OFF]->SetRadioBtn(TRUE);
		}
		
		// �Ϲ� ����
		if(m_bNormalInfo[nMainId])
		{
			m_pRadioUserInfo[nMainId][OPENSETUP_ON]->SetRadioBtn(TRUE);
			m_pRadioUserInfo[nMainId][OPENSETUP_OFF]->SetRadioBtn(FALSE);
		}
		else
		{
			m_pRadioUserInfo[nMainId][OPENSETUP_ON]->SetRadioBtn(FALSE);
			m_pRadioUserInfo[nMainId][OPENSETUP_OFF]->SetRadioBtn(TRUE);
		}
	}

	// 2012-10-17 by bhsohn ������ �̸����� ������ ON���� ����
#ifdef	C_ENEMY_INFO_ISSHIN
	#ifdef	C_ITEM_VIEW_UNCONDITIONAL_ON
		{
			m_pRadioGuildInfo[OPENSETUP_INFO_PREITEMINFO][OPENSETUP_ON]->ShowWindow(FALSE);
			m_pRadioGuildInfo[OPENSETUP_INFO_PREITEMINFO][OPENSETUP_OFF]->ShowWindow(FALSE);
		}
		{
			m_pRadioUserInfo[OPENSETUP_INFO_PREITEMINFO][OPENSETUP_ON]->ShowWindow(FALSE);
			m_pRadioUserInfo[OPENSETUP_INFO_PREITEMINFO][OPENSETUP_OFF]->ShowWindow(FALSE);
		}
	#endif
#endif
	// END 2012-10-17 by bhsohn ������ �̸����� ������ ON���� ����
}

void CINFCommunityUserOpenSetup::OnClickGuildInfo(int nMainId, int nSubId)
{
	BOOL bCheck = FALSE;
	if(0 == nSubId)
	{
		bCheck = TRUE;
	}
	m_bGuildInfo[nMainId] = bCheck;
	// 2012-10-17 by bhsohn ������ �̸����� ������ ON���� ����
	#ifdef	C_ENEMY_INFO_ISSHIN
		#ifdef	C_ITEM_VIEW_UNCONDITIONAL_ON
			m_bGuildInfo[OPENSETUP_INFO_PREITEMINFO] = TRUE;
		#endif
	#endif
	// END 2012-10-17 by bhsohn ������ �̸����� ������ ON���� ����
	UpdateRadioBtn();
}

void CINFCommunityUserOpenSetup::OnClickNormalInfo(int nMainId, int nSubId)
{
	BOOL bCheck = FALSE;
	if(0 == nSubId)
	{
		bCheck = TRUE;
	}
	m_bNormalInfo[nMainId] = bCheck;

	// 2012-10-17 by bhsohn ������ �̸����� ������ ON���� ����
	#ifdef	C_ENEMY_INFO_ISSHIN
		#ifdef	C_ITEM_VIEW_UNCONDITIONAL_ON
			m_bGuildInfo[OPENSETUP_INFO_PREITEMINFO] = TRUE;
		#endif
	#endif
	// END 2012-10-17 by bhsohn ������ �̸����� ������ ON���� ����
	UpdateRadioBtn();
}

void CINFCommunityUserOpenSetup::InitControl()
{
	const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
	int nMainId = 0;
	for(nMainId = 0;nMainId < MAX_OPENSETUP_INFO;nMainId++)
	{
		m_bGuildInfo[nMainId] = TRUE;
		m_bNormalInfo[nMainId] = TRUE;
	}	
	// �Ϲ�
	{
		// ���
		if(USER_INFO_OPTION_SECRET_LEVEL & myShuttleInfo.SecretInfoOption)
		{
			m_bNormalInfo[OPENSETUP_INFO_DEGREE] = FALSE;
		}
		// ��ġ
		if(USER_INFO_OPTION_SECRET_POSITION & myShuttleInfo.SecretInfoOption)
		{
			m_bNormalInfo[OPENSETUP_INFO_POS] = FALSE;
		}
		// ��
		if(USER_INFO_OPTION_SECRET_PROPENSITY & myShuttleInfo.SecretInfoOption)
		{
			m_bNormalInfo[OPENSETUP_INFO_HONOR] = FALSE;
		}
		// �Ҽ�
		if(USER_INFO_OPTION_SECRET_GUILD & myShuttleInfo.SecretInfoOption)
		{
			m_bNormalInfo[OPENSETUP_INFO_BELONG] = FALSE;
		}
		// ���� ������
		if(USER_INFO_OPTION_SECRET_LASTLOGIN & myShuttleInfo.SecretInfoOption)
		{
			m_bNormalInfo[OPENSETUP_INFO_CONTIME] = FALSE;
		}
// ������ ����	// 2012-07-12 by isshin ���۹̸����� On / Off ���
#ifdef	C_ENEMY_INFO_ISSHIN	
		if(USER_INFO_OPTION_SECRET_ITEMINFO & myShuttleInfo.SecretInfoOption)
		{
			m_bNormalInfo[OPENSETUP_INFO_PREITEMINFO] = FALSE;
		}
		// 2012-10-17 by bhsohn ������ �̸����� ������ ON���� ����
		#ifdef	C_ITEM_VIEW_UNCONDITIONAL_ON
			m_bNormalInfo[OPENSETUP_INFO_PREITEMINFO] = TRUE;
		#endif
		// END 2012-10-17 by bhsohn ������ �̸����� ������ ON���� ����
#endif
// end 2012-07-12 by isshin ���۹̸����� On / Off ���
	}

	// ����
	{
		// ���
		if(USER_INFO_OPTION_SECRET_GUILD_LEVEL & myShuttleInfo.SecretInfoOption)
		{
			m_bGuildInfo[OPENSETUP_INFO_DEGREE] = FALSE;
		}
		// ��ġ
		if(USER_INFO_OPTION_SECRET_GUILD_POSITION & myShuttleInfo.SecretInfoOption)
		{
			m_bGuildInfo[OPENSETUP_INFO_POS] = FALSE;
		}
		// ��
		if(USER_INFO_OPTION_SECRET_GUILD_PROPENSITY & myShuttleInfo.SecretInfoOption)
		{
			m_bGuildInfo[OPENSETUP_INFO_HONOR] = FALSE;
		}
		// �Ҽ�
		if(USER_INFO_OPTION_SECRET_GUILD_GUILD & myShuttleInfo.SecretInfoOption)
		{
			m_bGuildInfo[OPENSETUP_INFO_BELONG] = FALSE;
		}
		// ���� ������
		if(USER_INFO_OPTION_SECRET_GUILD_LASTLOGIN & myShuttleInfo.SecretInfoOption)
		{
			m_bGuildInfo[OPENSETUP_INFO_CONTIME] = FALSE;
		}
// ������ ����	// 2012-07-12 by isshin ���۹̸����� On / Off ���
#ifdef	C_ENEMY_INFO_ISSHIN	
		if(USER_INFO_OPTION_SECRET_GUILD_ITEMINFO & myShuttleInfo.SecretInfoOption)
		{
			m_bGuildInfo[OPENSETUP_INFO_PREITEMINFO] = FALSE;
		}
		// 2012-10-17 by bhsohn ������ �̸����� ������ ON���� ����
		#ifdef	C_ITEM_VIEW_UNCONDITIONAL_ON
			m_bGuildInfo[OPENSETUP_INFO_PREITEMINFO] = TRUE;
		#endif
		// END 2012-10-17 by bhsohn ������ �̸����� ������ ON���� ����
#endif
// end 2012-07-12 by isshin ���۹̸����� On / Off ���

		
	}
}
int CINFCommunityUserOpenSetup::GetChangeOptionInfo()
{
	int nSecretInfoOption = 0;
	// �Ϲ�
	{
		// ���
		if(!m_bNormalInfo[OPENSETUP_INFO_DEGREE])
		{			 
			nSecretInfoOption |= USER_INFO_OPTION_SECRET_LEVEL;
		}
		// ��ġ
		if(!m_bNormalInfo[OPENSETUP_INFO_POS] )
		{
			nSecretInfoOption |= USER_INFO_OPTION_SECRET_POSITION;			 
		}
		// ��
		if(!m_bNormalInfo[OPENSETUP_INFO_HONOR])
		{			
			nSecretInfoOption |= USER_INFO_OPTION_SECRET_PROPENSITY;
		}
		// �Ҽ�
		if(!m_bNormalInfo[OPENSETUP_INFO_BELONG])
		{			 
			nSecretInfoOption |= USER_INFO_OPTION_SECRET_GUILD;
		}
		// ���� ������
		if(!m_bNormalInfo[OPENSETUP_INFO_CONTIME])
		{			 
			nSecretInfoOption |= USER_INFO_OPTION_SECRET_LASTLOGIN;
		}
// ������ ����	// 2012-07-12 by isshin ���۹̸����� On / Off ���
#ifdef	C_ENEMY_INFO_ISSHIN
		if(!m_bNormalInfo[OPENSETUP_INFO_PREITEMINFO])
		{			 
			// 2012-10-17 by bhsohn ������ �̸����� ������ ON���� ����
			#ifdef	C_ITEM_VIEW_UNCONDITIONAL_ON
			#else
				nSecretInfoOption |= USER_INFO_OPTION_SECRET_ITEMINFO;
			#endif
		}
#endif
// end 2012-07-12 by isshin ���۹̸����� On / Off ���		
	}

	// ����
	{
		// ���
		if(!m_bGuildInfo[OPENSETUP_INFO_DEGREE])
		{			
			nSecretInfoOption |= USER_INFO_OPTION_SECRET_GUILD_LEVEL;
		}
		// ��ġ
		if( !m_bGuildInfo[OPENSETUP_INFO_POS])
		{
			nSecretInfoOption |= USER_INFO_OPTION_SECRET_GUILD_POSITION;			
		}
		// ��
		if(!m_bGuildInfo[OPENSETUP_INFO_HONOR])
		{
			nSecretInfoOption |= USER_INFO_OPTION_SECRET_GUILD_PROPENSITY;			 
		}
		// �Ҽ�
		if(!m_bGuildInfo[OPENSETUP_INFO_BELONG])
		{
			nSecretInfoOption |= USER_INFO_OPTION_SECRET_GUILD_GUILD;
		}
		// ���� ������
		if(!m_bGuildInfo[OPENSETUP_INFO_CONTIME])
		{
			nSecretInfoOption |= USER_INFO_OPTION_SECRET_GUILD_LASTLOGIN;
		}
// ������ ����	// 2012-07-12 by isshin ���۹̸����� On / Off ���
#ifdef	C_ENEMY_INFO_ISSHIN
		if(!m_bGuildInfo[OPENSETUP_INFO_PREITEMINFO])
		{
			// 2012-10-17 by bhsohn ������ �̸����� ������ ON���� ����
			#ifdef	C_ITEM_VIEW_UNCONDITIONAL_ON
			#else
				nSecretInfoOption |= USER_INFO_OPTION_SECRET_GUILD_ITEMINFO;
			#endif
		}
#endif
// end 2012-07-12 by isshin ���۹̸����� On / Off ���
	}

	// 2009. 10. 15 by ckPark ĳ���� ����â ������ �̼Ǹ����Ͱ� �����Ǵ� ���� ����
	if( g_pShuttleChild->m_myShuttleInfo.SecretInfoOption & USER_INFO_OPTION_MISSIONMASTER )
		nSecretInfoOption |= USER_INFO_OPTION_MISSIONMASTER;
	// end 2009. 10. 15 by ckPark ĳ���� ����â ������ �̼Ǹ����Ͱ� �����Ǵ� ���� ����

	return nSecretInfoOption;

}
void CINFCommunityUserOpenSetup::OnApply()
{
	int nSecretInfoOption = GetChangeOptionInfo();
	((CINFCommunity*)m_pParent)->RqChangeOpenUserInfo(nSecretInfoOption);

}