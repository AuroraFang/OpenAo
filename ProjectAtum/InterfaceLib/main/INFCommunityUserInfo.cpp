// INFCommunityUserInfo.cpp: implementation of the CINFCommunityUserInfo class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "D3DHanFont.h"
#include "AtumSound.h"
#include "Interface.h"
#include "INFPilotFace.h"
#include "AtumDatabase.h"

// 2009-02-13 by bhsohn ���� ��ŷ �ý���
#include "WorldRankManager.h"
// end 2009-02-13 by bhsohn ���� ��ŷ �ý���

#include "INFCommunityUserInfo.h"

#include "INFImageEx.h"	/// 2011. 10. 10 by jskim UI�ý��� ����
#include "INFGroupManager.h"
#include "INFGroupImage.h"
#include "INFToolTip.h"

// Ŀ�´�Ƽ â �ɼ�
#define	COMMUNITY_OPENBK_X				230
#define	COMMUNITY_OPENBK_Y				0
#define	COMMUNITY_OPENBK_WIDTH			274
#define	COMMUNITY_OPENBK_HEIGHT			179
#define	COMMUNITY_OPENBK_CAPS_HEIGHT	20

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
#define	COMMUNITY_OPENE_CLOSE_X			254
#define	COMMUNITY_OPENE_CLOSE_Y			0
#else
// �ݱ�
#define	COMMUNITY_OPENE_CLOSE_X			257
#define	COMMUNITY_OPENE_CLOSE_Y			5
#endif

// ĳ����
#define	USER_INFO_CHARACTER_POS_X					166
#define	USER_INFO_CHARACTER_POS_Y					23
// ��ü
#define	USER_INFO_UNIT_POS_X					166
#define	USER_INFO_UNIT_POS_Y					44

// 2009-02-13 by bhsohn ���� ��ŷ �ý���
// ����
// #define	USER_INFO_GUILD_MARK_POS_X					166
// #define	USER_INFO_GUILD_MARK_POS_Y					67
// ����
// #define	USER_INFO_GUILD_POS_X					196
// #define	USER_INFO_GUILD_POS_Y					65
// ����
// #define	USER_INFO_CLASS_POS_X					166
// #define	USER_INFO_CLASS_POS_Y					86
// ��
// #define	USER_INFO_FAME_POS_X					166
// #define	USER_INFO_FAME_POS_Y					107
// ��ġ
// #define	USER_INFO_REGION_POS_X					96
// #define	USER_INFO_REGION_POS_Y					128
// ������ ��¥
// #define	USER_INFO_LOGIN_POS_X					96
// #define	USER_INFO_LOGIN_POS_Y					149
////////////////// ���� ��ŷ ��ǥ ////////////////////////////
// �г���
#define	USER_INFO_NICKNAME_POS_X					166
#define	USER_INFO_NICKNAME_POS_Y					67

// ����
#define	USER_INFO_GUILD_MARK_POS_X					166
#define	USER_INFO_GUILD_MARK_POS_Y					88
// ����
#define	USER_INFO_GUILD_POS_X					196
#define	USER_INFO_GUILD_POS_Y					86
// ����
#define	USER_INFO_CLASS_POS_X					166
#define	USER_INFO_CLASS_POS_Y					107
// ��
#define	USER_INFO_FAME_POS_X					96
#define	USER_INFO_FAME_POS_Y					128
// ��ġ
#define	USER_INFO_REGION_POS_X					96
#define	USER_INFO_REGION_POS_Y					149
// ������ ��¥
#define	USER_INFO_LOGIN_POS_X					96
#define	USER_INFO_LOGIN_POS_Y					170

////////////////// ���� ��ŷ ��ǥ ////////////////////////////
// ����
#define	USER_INFO_NOWORLD_GUILD_MARK_POS_X					166
#define	USER_INFO_NOWORLD_GUILD_MARK_POS_Y					67
// ����
#define	USER_INFO_NOWORLD_GUILD_POS_X					196
#define	USER_INFO_NOWORLD_GUILD_POS_Y					65
// ����
#define	USER_INFO_NOWORLD_CLASS_POS_X					166
#define	USER_INFO_NOWORLD_CLASS_POS_Y					86
// ��
#define	USER_INFO_NOWORLD_FAME_POS_X					166
#define	USER_INFO_NOWORLD_FAME_POS_Y					107
// ��ġ
#define	USER_INFO_NOWORLD_REGION_POS_X					96
#define	USER_INFO_NOWORLD_REGION_POS_Y					128
// ������ ��¥
#define	USER_INFO_NOWORLD_LOGIN_POS_X					96
#define	USER_INFO_NOWORLD_LOGIN_POS_Y					149
// end 2009-02-13 by bhsohn ���� ��ŷ �ý���


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFCommunityUserInfo::CINFCommunityUserInfo(CAtumNode* pParent)
{
	m_pParent = pParent;
	
	m_bShow = FALSE;
	m_bMove = FALSE;

	m_pBkImage = NULL;
	m_ptCommOpBk.x = m_ptCommOpBk.y = 0;
	m_ptCommOpMouse.x = m_ptCommOpMouse.y = 0;

	m_pCloseBtn = NULL;	
	m_pFontText = NULL;
	m_pPilotFace			= NULL;
	m_pInfoGuildCase		= NULL;
	memset(&m_stUserInfo, 0x00, sizeof(MSG_FC_CHARACTER_GET_USER_INFO_OK));

	memset(m_strUserName, 0x00, 128);				// ���� ĳ���� �̸�
	memset(m_strUserGear, 0x00, 64);				// ���� ��ü ����
	memset(m_strGuild, 0x00, 256);				// ���� ��� ����
	memset(m_strClass, 0x00, 128);				// ���� ���
	memset(m_strFame, 0x00, 16);					// ���� ��
	memset(m_strLevel, 0x00, 16);					// ���� ����
	memset(m_strRegion, 0x00, 512);				// ������
	memset(m_strLogOutTime, 0x00, 512);				// �ð�

	// 2009-02-13 by bhsohn ���� ��ŷ �ý���
	ZERO_MEMORY(m_strNickName);
	// end 2009-02-13 by bhsohn ���� ��ŷ �ý���
	
}

CINFCommunityUserInfo::~CINFCommunityUserInfo()
{
	util::del(m_pBkImage);		
	util::del(m_pCloseBtn);	
	util::del(m_pFontText);	
	util::del(m_pPilotFace);
	util::del(m_pInfoGuildCase);
}

HRESULT CINFCommunityUserInfo::InitDeviceObjects()
{
	DataHeader	* pDataHeader= NULL;
	int nCnt = 0;

	// Ŀ�´�Ƽ �ɼǹ�	
	m_ptCommOpBk.x = (g_pD3dApp->GetBackBufferDesc().Width) - COMMUNITY_OPENBK_WIDTH;
	m_ptCommOpBk.y = (g_pD3dApp->GetBackBufferDesc().Height/2) - COMMUNITY_OPENBK_HEIGHT/2;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("C_ubk");	
	m_pBkImage = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
	m_pBkImage->InitDeviceObjects( g_pD3dApp->m_pImageList );

	{
		// �ݱ�
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "close");
		wsprintf(szDownBtn, "close");
		wsprintf(szSelBtn, "close");
		wsprintf(szDisBtn, "close");
		if(NULL == m_pCloseBtn)
		{
			m_pCloseBtn = new CINFImageBtn;
		}
		m_pCloseBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
#else
	{
		if(NULL == m_pBkImage)
		{
			m_pBkImage = new CINFImageEx;	
		}		
		pDataHeader = FindResource("c_ubk");
		m_pBkImage->InitDeviceObjects(pDataHeader );
	}		
	{
		// �ݱ�
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "xclose3");
		wsprintf(szDownBtn, "xclose1");
		wsprintf(szSelBtn, "xclose0");
		wsprintf(szDisBtn, "xclose2");
		if(NULL == m_pCloseBtn)
		{
			m_pCloseBtn = new CINFImageBtn;
		}
		m_pCloseBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
#endif
		

	{
		if(NULL == m_pFontText)
		{
			// 2008-10-22 by bhsohn ĳ���� ��Ʈ�� ©���� ���� ����
			//m_pFontText = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, FALSE,128,32);
			m_pFontText = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, FALSE,256,32);
			m_pFontText->InitDeviceObjects(g_pD3dDev);
		}
	}
	{
		if(NULL == m_pPilotFace)
		{
			m_pPilotFace = new CINFPilotFace;
			m_pPilotFace->InitDeviceObjects("face.tex");
		}
	}
	{
		if(NULL == m_pInfoGuildCase)
		{
			m_pInfoGuildCase = new CINFImageEx;
			pDataHeader = FindResource("GuilCase");
			m_pInfoGuildCase->InitDeviceObjects(pDataHeader ) ;
		}
	}
			
		
	return S_OK ;
}

HRESULT CINFCommunityUserInfo::RestoreDeviceObjects()
{
	if(m_pBkImage )
	{
		m_pBkImage->RestoreDeviceObjects();
	}

	if(m_pCloseBtn)
	{
		m_pCloseBtn->RestoreDeviceObjects();						
	}
	if(m_pFontText)
	{
		m_pFontText->RestoreDeviceObjects();
	}
	if(m_pPilotFace)
	{
		m_pPilotFace->RestoreDeviceObjects();
	}
	if(m_pInfoGuildCase)
	{
		m_pInfoGuildCase->RestoreDeviceObjects();
	}
	
	// UI���� ���� 
	UpdateUIPos();

	
	return S_OK ;
}

HRESULT CINFCommunityUserInfo::DeleteDeviceObjects()
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
	if(m_pFontText)
	{
		m_pFontText->DeleteDeviceObjects();
		util::del(m_pFontText);
	}
	if(m_pPilotFace)
	{
		m_pPilotFace->DeleteDeviceObjects();
		util::del(m_pPilotFace);
	}
	if(m_pInfoGuildCase)
	{		
		m_pInfoGuildCase->DeleteDeviceObjects();
		util::del(m_pInfoGuildCase);
	}
	return S_OK ;
}

HRESULT CINFCommunityUserInfo::InvalidateDeviceObjects()
{
	if(m_pBkImage)
	{
		m_pBkImage->InvalidateDeviceObjects();
	}
	if(m_pCloseBtn)
	{		
		m_pCloseBtn->InvalidateDeviceObjects();		
	}
	if(m_pFontText)
	{
		m_pFontText->InvalidateDeviceObjects();
	}
	if(m_pPilotFace)
	{
		m_pPilotFace->InvalidateDeviceObjects();
	}
	if(m_pInfoGuildCase)
	{		
		m_pInfoGuildCase->InvalidateDeviceObjects();
	}
		
	return S_OK ;
}

void CINFCommunityUserInfo::Render()
{
	if(!IsShowWindow())
	{
		return;
	}
	POINT ptBkPos = m_ptCommOpBk;
	if(m_pBkImage)
	{		
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����		
		m_pBkImage->Move(ptBkPos.x - 1, ptBkPos.y - 4);
		m_pBkImage->Render();
#else
		m_pBkImage->Move(ptBkPos.x, ptBkPos.y);
		m_pBkImage->Render() ;
#endif
	}
	
	m_pCloseBtn->Render();			

	int nPosX, nPosY;
	nPosX = nPosY = 0;

	{	
		// ĳ����
		{
			nPosX = ptBkPos.x+USER_INFO_CHARACTER_POS_X;
			nPosY = ptBkPos.y+USER_INFO_CHARACTER_POS_Y;

			m_pFontText->DrawText(nPosX,nPosY, GUI_FONT_COLOR, m_strUserName);
		}

		// ��ü
		{
			nPosX = ptBkPos.x+USER_INFO_UNIT_POS_X;
			nPosY = ptBkPos.y+USER_INFO_UNIT_POS_Y;

			m_pFontText->DrawText(nPosX,nPosY, GUI_FONT_COLOR, m_strUserGear);
		}
		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
// 		// ����
// 		if(m_stUserInfo.GuildUID)
// 		{
// 			CINFImage* pMark = NULL;
// 			pMark = g_pDatabase->GetGuildMark(m_stUserInfo.GuildUID);
// 			if(pMark)
// 			{
// 				nPosX = ptBkPos.x+USER_INFO_GUILD_MARK_POS_X;
// 				nPosY = ptBkPos.y+USER_INFO_GUILD_MARK_POS_Y;
// 				m_pInfoGuildCase->Move( nPosX-1, nPosY-1 );
// 				m_pInfoGuildCase->Render();
// 				
// 				pMark->Move( nPosX, nPosY );
// 				pMark->Render();
// 				
// 				// ���ܸ�
// 				nPosX = ptBkPos.x+USER_INFO_GUILD_POS_X;
// 				nPosY = ptBkPos.y+USER_INFO_GUILD_POS_Y;
// 			}
// 			else
// 			{
// 				// ���ܸ�ũ�� ����.
// 				nPosX = ptBkPos.x+USER_INFO_GUILD_MARK_POS_X;
// 				nPosY = ptBkPos.y+USER_INFO_GUILD_POS_Y;
// 			}		
// 		}
// 		else
// 		{
// 			nPosX = ptBkPos.x+USER_INFO_GUILD_MARK_POS_X;
// 			nPosY = ptBkPos.y+USER_INFO_GUILD_POS_Y;			
// 		}
// 		m_pFontText->DrawText(nPosX,nPosY, GUI_FONT_COLOR, m_strGuild);
// 
// 		// ���
// 		{
// 			nPosX = ptBkPos.x+USER_INFO_CLASS_POS_X;
// 			nPosY = ptBkPos.y+USER_INFO_CLASS_POS_Y;
// 			
// 			m_pFontText->DrawText(nPosX,nPosY, GUI_FONT_COLOR, m_strClass);
// 		}
// 		// ��
// 		{
// 			nPosX = ptBkPos.x+USER_INFO_FAME_POS_X;
// 			nPosY = ptBkPos.y+USER_INFO_FAME_POS_Y;
// 			
// 			m_pFontText->DrawText(nPosX,nPosY, GUI_FONT_COLOR, m_strFame);
// 		}
// 		// ��ġ
// 		{
// 			nPosX = ptBkPos.x+USER_INFO_REGION_POS_X;
// 			nPosY = ptBkPos.y+USER_INFO_REGION_POS_Y;
// 			
// 			m_pFontText->DrawText(nPosX,nPosY, GUI_FONT_COLOR, m_strRegion);
// 		}
// 		// ����������
// 		if(strlen(m_strLogOutTime) > 0)
// 		{
//			nPosX = ptBkPos.x+USER_INFO_LOGIN_POS_X;
//			nPosY = ptBkPos.y+USER_INFO_LOGIN_POS_Y;
//			
//			m_pFontText->DrawText(nPosX,nPosY, GUI_FONT_COLOR, m_strLogOutTime);
//		}

		CWorldRankManager*	pWorldRankManager = g_pD3dApp->GetWorldRankManager();
		if(pWorldRankManager->GetUseWorldRanking())
		{
			nPosX = ptBkPos.x+USER_INFO_NICKNAME_POS_X;
			nPosY = ptBkPos.y+USER_INFO_NICKNAME_POS_Y;
			
			m_pFontText->DrawText(nPosX,nPosY, GUI_FONT_COLOR, m_strNickName);
		}
		POINT ptGMarkPos, ptGPos, ptClassPos, ptFamePos, ptRegionPos, ptLoginPos;
		ptGMarkPos.x = ptGPos.x = ptClassPos.x = ptFamePos.x = ptRegionPos.x = ptLoginPos.x = 0;
		ptGMarkPos.y = ptGPos.y = ptClassPos.y = ptFamePos.y = ptRegionPos.y = ptLoginPos.y = 0;
		GetUIPos(&ptGMarkPos, &ptGPos, &ptClassPos, &ptFamePos, &ptRegionPos, &ptLoginPos);
		
		// ����
		if(m_stUserInfo.GuildUID)
		{
			CINFImage* pMark = NULL;
			pMark = g_pDatabase->GetGuildMark(m_stUserInfo.GuildUID);
			if(pMark)
			{
// 				nPosX = ptBkPos.x+USER_INFO_GUILD_MARK_POS_X;
// 				nPosY = ptBkPos.y+USER_INFO_GUILD_MARK_POS_Y;
				nPosX = ptBkPos.x+ptGMarkPos.x;
				nPosY = ptBkPos.y+ptGMarkPos.y;

				m_pInfoGuildCase->Move( nPosX-1, nPosY-1 );
				m_pInfoGuildCase->Render();
				
				pMark->Move( nPosX, nPosY );
				pMark->Render();
				
				// ���ܸ�
// 				nPosX = ptBkPos.x+USER_INFO_GUILD_POS_X;
// 				nPosY = ptBkPos.y+USER_INFO_GUILD_POS_Y;
				nPosX = ptBkPos.x+ptGPos.x;
				nPosY = ptBkPos.y+ptGPos.y;
			}
			else
			{
				// ���ܸ�ũ�� ����.
// 				nPosX = ptBkPos.x+USER_INFO_GUILD_MARK_POS_X;
// 				nPosY = ptBkPos.y+USER_INFO_GUILD_POS_Y;
				nPosX = ptBkPos.x+ptGMarkPos.x;
				nPosY = ptBkPos.y+ptGPos.y;
			}		
		}
		else
		{
// 			nPosX = ptBkPos.x+USER_INFO_GUILD_MARK_POS_X;
// 			nPosY = ptBkPos.y+USER_INFO_GUILD_POS_Y;			
			nPosX = ptBkPos.x+ptGMarkPos.x;
			nPosY = ptBkPos.y+ptGPos.y;			
		}
		m_pFontText->DrawText(nPosX,nPosY, GUI_FONT_COLOR, m_strGuild);

		// ���
		{
// 			nPosX = ptBkPos.x+USER_INFO_CLASS_POS_X;
// 			nPosY = ptBkPos.y+USER_INFO_CLASS_POS_Y;
			nPosX = ptBkPos.x+ptClassPos.x;
			nPosY = ptBkPos.y+ptClassPos.y;

			m_pFontText->DrawText(nPosX,nPosY, GUI_FONT_COLOR, m_strClass);
		}
		// ��
		{
// 			nPosX = ptBkPos.x+USER_INFO_FAME_POS_X;
// 			nPosY = ptBkPos.y+USER_INFO_FAME_POS_Y;
			nPosX = ptBkPos.x+ptFamePos.x;
			nPosY = ptBkPos.y+ptFamePos.y;			

			m_pFontText->DrawText(nPosX,nPosY, GUI_FONT_COLOR, m_strFame);
		}
		// ��ġ
		{
// 			nPosX = ptBkPos.x+USER_INFO_REGION_POS_X;
// 			nPosY = ptBkPos.y+USER_INFO_REGION_POS_Y;
			nPosX = ptBkPos.x+ptRegionPos.x;
			nPosY = ptBkPos.y+ptRegionPos.y;

			m_pFontText->DrawText(nPosX,nPosY, GUI_FONT_COLOR, m_strRegion);
		}
		// ����������
		if(strlen(m_strLogOutTime) > 0)
		{
// 			nPosX = ptBkPos.x+USER_INFO_LOGIN_POS_X;
// 			nPosY = ptBkPos.y+USER_INFO_LOGIN_POS_Y;
			nPosX = ptBkPos.x+ptLoginPos.x;
			nPosY = ptBkPos.y+ptLoginPos.y;

			m_pFontText->DrawText(nPosX,nPosY, GUI_FONT_COLOR, m_strLogOutTime);
		}		
		// end 2009-02-13 by bhsohn ���� ��ŷ �ý���
	}
	CINFImage* pFace = m_pPilotFace->FindPilotImage(m_stUserInfo.PilotFace);	
	if(pFace)
	{
		nPosX = ptBkPos.x+8;
		nPosY = ptBkPos.y+23;

		pFace->Move(nPosX, nPosY);
		pFace->Render();
	}
	
}

void CINFCommunityUserInfo::Tick()
{
	if(!IsShowWindow())
	{
		return;
	}	

}

int CINFCommunityUserInfo::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
					ShowWindow(FALSE);					
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
				}
			}					
		}
		break;		
	}

	return INF_MSGPROC_NORMAL;		
}

void CINFCommunityUserInfo::ShowWindow(BOOL bShow)
{
	m_bShow = bShow;

	m_bMove = FALSE;		// ��Ʈ���� �����δ�.
	if(bShow)
	{
		// Ŀ�´�Ƽ �ɼǹ�	
		m_ptCommOpBk.x = (g_pD3dApp->GetBackBufferDesc().Width) - COMMUNITY_OPENBK_WIDTH;
		m_ptCommOpBk.y = (g_pD3dApp->GetBackBufferDesc().Height/2) - COMMUNITY_OPENBK_HEIGHT/2;
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

BOOL CINFCommunityUserInfo::IsShowWindow()
{
	return m_bShow;
}

BOOL CINFCommunityUserInfo::IsMouseCaps(POINT ptPos)
{
	if((ptPos.x >= m_ptCommOpBk.x && (ptPos.x <= m_ptCommOpBk.x+COMMUNITY_OPENBK_WIDTH))
		&& (ptPos.y >= m_ptCommOpBk.y && (ptPos.y <= m_ptCommOpBk.y+COMMUNITY_OPENBK_CAPS_HEIGHT)))
	{
		return TRUE;
	}
	return FALSE;

}

void CINFCommunityUserInfo::UpdateUIPos()
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
	
}

void CINFCommunityUserInfo::ShowUserInfo(MSG_FC_CHARACTER_GET_USER_INFO_OK* pMsg)
{	
	memcpy(&m_stUserInfo, pMsg, sizeof(MSG_FC_CHARACTER_GET_USER_INFO_OK));
	
	// ĳ���� �̸�
	strncpy(m_strUserName, pMsg->CharacterName, SIZE_MAX_CHARACTER_NAME);				
	// �۾��� �ٲپ��ش�.
	g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), m_strUserName);

	// ��� ����
	strcpy(m_strUserGear, AtumSJ::GetUnitKindString(pMsg->UnitKind));
		
	// ���
	strncpy(m_strGuild, pMsg->GuildName, SIZE_MAX_GUILD_NAME);	

	// 2009-02-13 by bhsohn ���� ��ŷ �ý���
	strncpy(m_strNickName, pMsg->NickName, SIZE_MAX_CHARACTER_NAME);		
	// end 2009-02-13 by bhsohn ���� ��ŷ �ý���

	// ���	'\0' == NULL = 0 =  
	memset(m_strClass, 0x00, 128);				// ���� ���
	
	auto pClass = AtumSJ::GetStatusByLevel(pMsg->Level);	
	
	if (pClass) strncpy(m_strClass, pClass, 128);

	// ������
	memset(m_strRegion, 0x00, 512);
	if(pMsg->MapChannelIndex.MapIndex)
	{
		MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo(pMsg->MapChannelIndex.MapIndex);
		if(pMapInfo)
		{
			strncpy(m_strRegion, pMapInfo->MapName, SIZE_MAX_MAP_NAME);
		}	
	}

	// ��
	wsprintf(m_strFame, "%d", pMsg->Propensity);
	
	// ����
	wsprintf(m_strLevel, "[Lv.%dX]",pMsg->Level/10);

	// ������ �α��� �ð�	
	memset(m_strLogOutTime, 0x00, 512);				// �ð�
	if(pMsg->LastStartedTime.Year != 0)
	{
		string str{};
		wsprintf(m_strLogOutTime, "%s", pMsg->LastStartedTime.GetLocalDateTimeString(str, TRUE));	
	}
	
	ShowWindow(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
/// \date		2009-02-13 ~ 2009-02-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunityUserInfo::GetUIPos(POINT *ptGMarkPos, POINT *ptGPos, POINT *ptClassPos, 
									 POINT *ptFamePos, POINT *ptRegionPos, POINT *ptLoginPos)
{	
	(*ptGMarkPos).x = (*ptGPos).x = (*ptClassPos).x = (*ptFamePos).x = (*ptRegionPos).x = (*ptLoginPos).x = 0;
	(*ptGMarkPos).y = (*ptGPos).y = (*ptClassPos).y = (*ptFamePos).y = (*ptRegionPos).y = (*ptLoginPos).y = 0;
	
	CWorldRankManager*	pWorldRankManager = g_pD3dApp->GetWorldRankManager();	
	if(pWorldRankManager->GetUseWorldRanking())
	{
		(*ptGMarkPos).x = USER_INFO_GUILD_MARK_POS_X;
		(*ptGMarkPos).y = USER_INFO_GUILD_MARK_POS_Y;
		
		(*ptGPos).x = USER_INFO_GUILD_POS_X;
		(*ptGPos).y = USER_INFO_GUILD_POS_Y;
		
		(*ptClassPos).x = USER_INFO_CLASS_POS_X;
		(*ptClassPos).y = USER_INFO_CLASS_POS_Y;
		
		(*ptFamePos).x = USER_INFO_FAME_POS_X;
		(*ptFamePos).y = USER_INFO_FAME_POS_Y;
		
		(*ptRegionPos).x = USER_INFO_REGION_POS_X;
		(*ptRegionPos).y = USER_INFO_REGION_POS_Y;
		
		(*ptLoginPos).x = USER_INFO_LOGIN_POS_X;
		(*ptLoginPos).y = USER_INFO_LOGIN_POS_Y;
	}
	else
	{
		(*ptGMarkPos).x = USER_INFO_NOWORLD_GUILD_MARK_POS_X;
		(*ptGMarkPos).y = USER_INFO_NOWORLD_GUILD_MARK_POS_Y;
		
		(*ptGPos).x = USER_INFO_NOWORLD_GUILD_POS_X;
		(*ptGPos).y = USER_INFO_NOWORLD_GUILD_POS_Y;
		
		(*ptClassPos).x = USER_INFO_NOWORLD_CLASS_POS_X;
		(*ptClassPos).y = USER_INFO_NOWORLD_CLASS_POS_Y;
		
		(*ptFamePos).x = USER_INFO_NOWORLD_FAME_POS_X;
		(*ptFamePos).y = USER_INFO_NOWORLD_FAME_POS_Y;
		
		(*ptRegionPos).x = USER_INFO_NOWORLD_REGION_POS_X;
		(*ptRegionPos).y = USER_INFO_NOWORLD_REGION_POS_Y;
		
		(*ptLoginPos).x = USER_INFO_NOWORLD_LOGIN_POS_X;
		(*ptLoginPos).y = USER_INFO_NOWORLD_LOGIN_POS_Y;
	}
}

void CINFCommunityUserInfo::HideUserInfo()
{
	ShowWindow(FALSE);
	g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0, 0, 0), FALSE);
}
