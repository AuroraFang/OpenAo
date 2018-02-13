// INFCommunity.cpp: implementation of the CINFCommunity class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFCommunity.h"
#include "AtumApplication.h"
#include "INFCommunityParty.h"
#include "INFCommunityGuild.h"
#include "INFCommunityFriend.h"
#include "INFCommunityReject.h"
// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
#include "INFCommunityLetter.h"
#include "INFCommuPartyCre.h"
#include "INFCommuPartyInvite.h"
#include "INFCommunityGuildManager.h"
#include "INFCommunityGuildSearch.h"
#include "INFCommunityGuildIntroMe.h"
#include "INFCommunityChatRoom.h"
#include "INFCommunityUserOpenSetup.h"
#include "INFCommunityUserInfo.h"
#include "INFEnemyItemInfo.h"// 2012-06-14 by isshin ���۹̸�����
#include "ShuttleChild.h"
#include "ClientParty.h"
#include "INFWindow.h"
#include "INFImageBtn.h"
#include "IMSocketManager.h"
// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â

// 2009-04-14 by bhsohn ä�� ���� �Ʒ��� ������, ������ ����
#include "Chat.h"
// end 2009-04-14 by bhsohn ä�� ���� �Ʒ��� ������, ������ ����

#include "INFGameMain.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "Interface.h"
#include "dxutil.h"

#include "INFImageEx.h"									   // 2011. 10. 10 by jskim UI�ý��� ����

#include "INFGroupImage.h"
#include "INFGroupManager.h"
// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
#include "INFToolTip.h"
// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����

// 2013-01-03 by mspark, ���� �޴��� Ŭ���Ͽ��� ��� ���尡 ���ļ� �鸮�� ���� ����
#include "AtumSound.h"
// end 2013-01-03 by mspark, ���� �޴��� Ŭ���Ͽ��� ��� ���尡 ���ļ� �鸮�� ���� ����

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
/// Ŀ�´�Ƽâ ��� ��ġ
#define	COMMUNITY_BK_WIDTH		518
#define	COMMUNITY_BK_HEIGHT		404
#define	COMMUNITY_CAPS_HEIGHT	20

#define	COMMUNITY_BTN_POSX		26
#define	COMMUNITY_BTN_POSY		378

CINFCommunity::CINFCommunity(CAtumNode* pParent)
{
	m_pParent = pParent;
	//m_pLogo = NULL;
	m_pParty = NULL;
	m_pGuild = NULL;
	m_pFriend = NULL;
	m_pReject = NULL;
	
	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	m_pLetter = NULL;		// ����	
	m_pChatRoom = NULL;			// ä�ù�.
	// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â

	m_nCommunityType = COMMUNITY_PARTY;

	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	m_ptCommunityBk.x = m_ptCommunityBk.y = 0;	
	m_bShowWnd = FALSE;	
	m_pCommuPartyInvite = NULL;
	m_pCommuPartyCreate = NULL;
	m_pCommunityUserOpenSetup = NULL;
	m_pCommunityUserInfo = NULL;
	m_pEnemyItemInfo = NULL;			// 2012-06-14 by isshin ���۹̸�����
	m_pCommunityGuildManager = NULL;
	m_pCommunityGuildSearch = NULL;
	m_pCommunityGuildIntroMe = NULL;
	m_pCloseBtn = NULL;

	m_nRqMemInfoWnd = COMMUNITY_MEMINFO_MANAGER;

	// 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
	m_nLastRqSecretInfoOption = -1;

	m_bMove = FALSE;	
	m_ptCommOpMouse.x = m_ptCommOpMouse.y = 0;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pCommunityBKImage = NULL;
	for( int i=0; i < COMMUNITY_MAX; i++ )
	{
		m_pCommMenuBtn[i] = NULL;	
	}																							  
#endif
}

CINFCommunity::~CINFCommunity()
{
	//util::del( m_pLogo );
	util::del(m_pParty);
	util::del(m_pGuild);
	util::del(m_pFriend);
	util::del(m_pReject);

	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	util::del(m_pLetter);	// ����		
	util::del(m_pChatRoom);
	util::del(m_pCommuPartyCreate);		
	util::del(m_pCommunityUserOpenSetup);
	util::del(m_pCommunityUserInfo);
	util::del(m_pEnemyItemInfo);							// 2012-06-14 by isshin ���۹̸�����
	util::del(m_pCommuPartyInvite);
	util::del(m_pCommunityGuildManager);
	util::del(m_pCommunityGuildSearch);
	util::del(m_pCommunityGuildIntroMe);
	util::del(m_pCloseBtn);
	// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	util::del( m_pCommunityBKImage );
	for( int i=0; i < COMMUNITY_MAX; i++ )
	{
		util::del(m_pCommMenuBtn[i]);
	}																							  
#endif
}

HRESULT CINFCommunity::InitDeviceObjects()
{
	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
//	DataHeader* pDataHeader = FindResource("com_logo");
//	m_pLogo = new CINFImage;
//	m_pLogo->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize) ;	
	m_ptCommunityBk.x = (g_pD3dApp->GetBackBufferDesc().Width/2) - COMMUNITY_BK_WIDTH/2;
	m_ptCommunityBk.y = (g_pD3dApp->GetBackBufferDesc().Height/2) - COMMUNITY_BK_HEIGHT/2;	

	m_pParty = new CINFCommunityParty(this);
	m_pParty->SetGameData(m_pGameData);
	m_pParty->InitDeviceObjects();
	
	m_pGuild = new CINFCommunityGuild(this);
	m_pGuild->SetGameData(m_pGameData);
	m_pGuild->InitDeviceObjects();

	m_pFriend = new CINFCommunityFriend(this);
	m_pFriend->SetGameData(m_pGameData);
	m_pFriend->InitDeviceObjects();	
	
	m_pReject = new CINFCommunityReject(this);
	m_pReject->SetGameData(m_pGameData);
	m_pReject->InitDeviceObjects();

	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	// ������
	m_pLetter = new CINFCommunityLetter(this);
	m_pLetter->SetGameData(m_pGameData);
	m_pLetter->InitDeviceObjects();	
	
	// ä��
	m_pChatRoom = new CINFCommunityChatRoom(this);
	m_pChatRoom->SetGameData(m_pGameData);
	m_pChatRoom->InitDeviceObjects();

	{
		if(NULL == m_pCommuPartyCreate)
		{
			m_pCommuPartyCreate = new CINFCommuPartyCre(this);
			m_pCommuPartyCreate->SetGameData(m_pGameData);
			m_pCommuPartyCreate->InitDeviceObjects();
		}		
	}

	{
		if(NULL == m_pCommunityUserOpenSetup)
		{
			m_pCommunityUserOpenSetup = new CINFCommunityUserOpenSetup(this);
			m_pCommunityUserOpenSetup->SetGameData(m_pGameData);
			m_pCommunityUserOpenSetup->InitDeviceObjects();
		}		
	}
	
	{
		if(NULL == m_pCommunityUserInfo)
		{
			m_pCommunityUserInfo = new CINFCommunityUserInfo(this);
			m_pCommunityUserInfo->SetGameData(m_pGameData);
			m_pCommunityUserInfo->InitDeviceObjects();
		}		
	}
	
	// 2012-06-14 by isshin ���۹̸�����
	{
		if(NULL == m_pEnemyItemInfo)
		{
			m_pEnemyItemInfo = new CINFEnemyItemInfo(this);
			m_pEnemyItemInfo->SetGameData(m_pGameData);
			m_pEnemyItemInfo->InitDeviceObjects();
		}		
	}// end 2012-06-14 by isshin ���۹̸�����

	{
		if(NULL == m_pCommuPartyInvite)
		{
			m_pCommuPartyInvite = new CINFCommuPartyInvite(this);
			m_pCommuPartyInvite->SetGameData(m_pGameData);
			m_pCommuPartyInvite->InitDeviceObjects();
		}		
	}
	{
		if(NULL == m_pCommunityGuildManager)
		{
			m_pCommunityGuildManager = new CINFCommunityGuildManager(this);
			m_pCommunityGuildManager->SetGameData(m_pGameData);
			m_pCommunityGuildManager->InitDeviceObjects();
		}		
	}
	
	{
		if(NULL == m_pCommunityGuildSearch)
		{
			m_pCommunityGuildSearch = new CINFCommunityGuildSearch(this);
			m_pCommunityGuildSearch->SetGameData(m_pGameData);
			m_pCommunityGuildSearch->InitDeviceObjects();
		}		
	}
	
	{
		if(NULL == m_pCommunityGuildIntroMe)
		{
			m_pCommunityGuildIntroMe = new CINFCommunityGuildIntroMe(this);
			m_pCommunityGuildIntroMe->SetGameData(m_pGameData);
			m_pCommunityGuildIntroMe->InitDeviceObjects();
		}		
	}
	// �ݱ� â
	//
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
	// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("commuBG");	
	m_pCommunityBKImage = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
	m_pCommunityBKImage->InitDeviceObjects( g_pD3dApp->m_pImageList );

	for( int i=0; i < COMMUNITY_MAX; i++ )
	{
		m_pCommMenuBtn[i] = new CINFImageBtn;
	}	

	m_pCommMenuBtn[COMMUNITY_PARTY]->InitDeviceObjects("comFbtn0");
	m_pCommMenuBtn[COMMUNITY_FRIEND]->InitDeviceObjects("comFrbtn0");
	m_pCommMenuBtn[COMMUNITY_GUILD]->InitDeviceObjects("comBbtn0");
	m_pCommMenuBtn[COMMUNITY_CHATROOM]->InitDeviceObjects("comCbtn0");
	m_pCommMenuBtn[COMMUNITY_MAIL]->InitDeviceObjects("comLbtn0");
	m_pCommMenuBtn[COMMUNITY_REJECT]->InitDeviceObjects("comRbtn0");
#endif
	return S_OK;
}

HRESULT CINFCommunity::RestoreDeviceObjects()
{
	//m_pLogo->RestoreDeviceObjects();
	m_pParty->RestoreDeviceObjects();
	m_pGuild->RestoreDeviceObjects();
	m_pFriend->RestoreDeviceObjects();
	m_pReject->RestoreDeviceObjects();
	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	m_pLetter->RestoreDeviceObjects();		
	m_pChatRoom->RestoreDeviceObjects();
	if(m_pCommuPartyCreate)
	{
		m_pCommuPartyCreate->RestoreDeviceObjects();
	}
	if(m_pCommunityUserOpenSetup)
	{
		m_pCommunityUserOpenSetup->RestoreDeviceObjects();
	}
	if(m_pCommunityUserInfo)
	{
		m_pCommunityUserInfo->RestoreDeviceObjects();
	}
	// 2012-06-14 by isshin ���۹̸�����
	if(m_pEnemyItemInfo)
	{
		m_pEnemyItemInfo->RestoreDeviceObjects();
	}// end 2012-06-14 by isshin ���۹̸�����
	if(m_pCommuPartyInvite)
	{
		m_pCommuPartyInvite->RestoreDeviceObjects();
	}	
	if(m_pCommunityGuildManager)
	{
		m_pCommunityGuildManager->RestoreDeviceObjects();
	}
	if(m_pCommunityGuildSearch)
	{
		m_pCommunityGuildSearch->RestoreDeviceObjects();
	}
	if(m_pCommunityGuildIntroMe)
	{
		m_pCommunityGuildIntroMe->RestoreDeviceObjects();
	}
	{	
		m_pCloseBtn->RestoreDeviceObjects();		
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
		float fPosX = m_ptCommunityBk.x + COMMUNITY_BK_WIDTH-m_pCloseBtn->GetImgSize().x ;
		float fPosY = m_ptCommunityBk.y + 4;													  
#else  
		float fPosX = m_ptCommunityBk.x + COMMUNITY_BK_WIDTH-m_pCloseBtn->GetImgSize().x -2;
		float fPosY = m_ptCommunityBk.y + 21;		
#endif
		m_pCloseBtn->SetBtnPosition(fPosX, fPosY);
	}
	UpdateBtnPos();
	// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pCommunityBKImage->RestoreDeviceObjects();

	DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("commu_me");	
	m_pCommunityControl = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
	for( int i=0; i < COMMUNITY_MAX; i++ )
	{
		m_pCommMenuBtn[i]->RestoreDeviceObjects();
	}																							  
#endif

	return S_OK;
}

HRESULT CINFCommunity::InvalidateDeviceObjects()
{
	//m_pLogo->InvalidateDeviceObjects();
	m_pParty->InvalidateDeviceObjects();
	m_pGuild->InvalidateDeviceObjects();
	m_pFriend->InvalidateDeviceObjects();
	m_pReject->InvalidateDeviceObjects();
	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	m_pLetter->InvalidateDeviceObjects();		
	m_pChatRoom->InvalidateDeviceObjects();
	if(m_pCommuPartyCreate)
	{
		m_pCommuPartyCreate->InvalidateDeviceObjects();
	}
	if(m_pCommunityUserOpenSetup)
	{
		m_pCommunityUserOpenSetup->InvalidateDeviceObjects();
	}
	if(m_pCommunityUserInfo)
	{
		m_pCommunityUserInfo->InvalidateDeviceObjects();
	}
	// 2012-06-14 by isshin ���۹̸�����
	if(m_pEnemyItemInfo)
	{
		m_pEnemyItemInfo->InvalidateDeviceObjects();
	}// end 2012-06-14 by isshin ���۹̸�����
	if(m_pCommuPartyInvite)
	{
		m_pCommuPartyInvite->InvalidateDeviceObjects();
	}
	if(m_pCommunityGuildManager)
	{
		m_pCommunityGuildManager->InvalidateDeviceObjects();
	}
	if(m_pCommunityGuildSearch)
	{
		m_pCommunityGuildSearch->InvalidateDeviceObjects();
	}
	if(m_pCommunityGuildIntroMe)
	{
		m_pCommunityGuildIntroMe->InvalidateDeviceObjects();
	}
	if(m_pCloseBtn)
	{
		m_pCloseBtn->InvalidateDeviceObjects();	
	}

	// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pCommunityBKImage->InvalidateDeviceObjects();

	for( int i=0; i < COMMUNITY_MAX; i++ )
	{
		m_pCommMenuBtn[i]->InvalidateDeviceObjects();
	}																							  
#endif
	return S_OK;
}

HRESULT CINFCommunity::DeleteDeviceObjects()
{
	//m_pLogo->DeleteDeviceObjects();
	m_pParty->DeleteDeviceObjects();
	m_pGuild->DeleteDeviceObjects();
	m_pFriend->DeleteDeviceObjects();
	m_pReject->DeleteDeviceObjects();
	util::del(m_pParty);
	util::del(m_pGuild);
	util::del(m_pFriend);
	util::del(m_pReject);
	
	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	m_pLetter->DeleteDeviceObjects();
	util::del(m_pLetter);		
	m_pChatRoom->DeleteDeviceObjects();
	util::del(m_pChatRoom);
	if(m_pCommuPartyCreate)
	{
		m_pCommuPartyCreate->DeleteDeviceObjects();
		util::del(m_pCommuPartyCreate);
	}
	if(m_pCommunityUserOpenSetup)
	{
		m_pCommunityUserOpenSetup->DeleteDeviceObjects();
		util::del(m_pCommunityUserOpenSetup);
	}
	if(m_pCommunityUserInfo)
	{
		m_pCommunityUserInfo->DeleteDeviceObjects();
		util::del(m_pCommunityUserInfo);
	}
	// 2012-06-14 by isshin ���۹̸�����
	if(m_pEnemyItemInfo)
	{
		m_pEnemyItemInfo->DeleteDeviceObjects();		
		util::del(m_pEnemyItemInfo);
	}// end 2012-06-14 by isshin ���۹̸�����

	if(m_pCommuPartyInvite)
	{
		m_pCommuPartyInvite->DeleteDeviceObjects();
		util::del(m_pCommuPartyInvite);
	}
	if(m_pCommunityGuildManager)
	{
		m_pCommunityGuildManager->DeleteDeviceObjects();
		util::del(m_pCommunityGuildManager);
	}
	if(m_pCommunityGuildSearch)
	{
		m_pCommunityGuildSearch->DeleteDeviceObjects();
		util::del(m_pCommunityGuildSearch);
	}
	if(m_pCommunityGuildIntroMe)
	{
		m_pCommunityGuildIntroMe->DeleteDeviceObjects();
		util::del(m_pCommunityGuildIntroMe);
	}
	if(m_pCloseBtn)
	{
		m_pCloseBtn->DeleteDeviceObjects();	
		util::del(m_pCloseBtn);
	}

	// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	if( m_pCommunityBKImage )
	{
		m_pCommunityBKImage->DeleteDeviceObjects();
		util::del( m_pCommunityBKImage );
	}

	for( int i=0; i < COMMUNITY_MAX; i++ )
	{
		if(m_pCommMenuBtn[i])
		{
			m_pCommMenuBtn[i]->DeleteDeviceObjects();
			util::del( m_pCommMenuBtn[i] );
		}
	}																							  
#endif
	return S_OK;
}

void CINFCommunity::Tick()
{
	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â	
	if(!IsShowCommunityWindow())
	{		
		m_pCommuPartyCreate->Tick();// ��� ����
		m_pCommunityUserOpenSetup->Tick();// ���� ����
		m_pCommunityUserInfo->Tick();// ���� 
		m_pCommuPartyInvite->Tick();// ��� �ʴ�
		m_pCommunityGuildManager->Tick();// ������ ����
		m_pCommunityGuildSearch->Tick();// ���ܰ˻�
		m_pCommunityGuildIntroMe->Tick();// �ڱ� �Ұ�

// 2012-06-14 by isshin ���۹̸�����
#ifdef C_ENEMY_INFO_ISSHIN
		m_pEnemyItemInfo->Tick();
#endif
// end 2012-06-14 by isshin ���۹̸�����
		return;
	}
	// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	switch(m_nCommunityType)
	{
	case COMMUNITY_PARTY:
		{
			m_pParty->Tick();
		}
		break;
	case COMMUNITY_FRIEND:
		{
			m_pFriend->Tick();
		}
		break;
	case COMMUNITY_GUILD:
		{
			m_pGuild->Tick();
		}
		break;
	case COMMUNITY_REJECT:
		{
			m_pReject->Tick();
		}
		break;
		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	case COMMUNITY_MAIL:		// ����
		{
			m_pLetter->Tick();
		}
		break;
	case COMMUNITY_CHATROOM:
		{
			m_pChatRoom->Tick();
		}
		break;
		// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â	
	}	
	m_pCommuPartyCreate->Tick();// ��� ����
	m_pCommunityUserOpenSetup->Tick();// ���� ����
	m_pCommunityUserInfo->Tick();// ���� 
	m_pCommuPartyInvite->Tick();// ��� �ʴ�
	m_pCommunityGuildManager->Tick();// ������ ����
	m_pCommunityGuildSearch->Tick();// ���ܰ˻�
	m_pCommunityGuildIntroMe->Tick();// �ڱ� �Ұ�

// 2012-06-14 by isshin ���۹̸�����
#ifdef C_ENEMY_INFO_ISSHIN
	m_pEnemyItemInfo->Tick();
#endif
// end 2012-06-14 by isshin ���۹̸�����
}

#define LOGO_START_X			8
#define LOGO_START_Y			5

void CINFCommunity::Render()
{
	if(!IsShowCommunityWindow())
	{				
		m_pCommuPartyCreate->Render();// ��� ����
		m_pCommunityUserOpenSetup->Render();// ���� ����
		m_pCommunityUserInfo->Render();// ���� 
		m_pCommuPartyInvite->Render();// ��� �ʴ�
		m_pCommunityGuildManager->Render();// ������ ����
		m_pCommunityGuildSearch->Render();	// ���� �˻�
		m_pCommunityGuildIntroMe->Render();// �ڱ� �Ұ�

// 2012-06-14 by isshin ���۹̸�����
#ifdef C_ENEMY_INFO_ISSHIN
		m_pEnemyItemInfo->Render();
#endif
// end 2012-06-14 by isshin ���۹̸�����		
		return;
	}	
//	m_pLogo->Move(LOGO_START_X, g_pGameMain->m_nLeftWindowY + LOGO_START_Y);
//	m_pLogo->Render();

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pCommunityBKImage->Move(m_ptCommunityBk.x, m_ptCommunityBk.y);
	m_pCommunityBKImage->Render();

	{
		POINT pBkSize;
		pBkSize.x =  m_pCommunityBKImage->GetMaxPos().x - m_pCommunityBKImage->GetMinPos().x;
		pBkSize.y =  m_pCommunityBKImage->GetMaxPos().y - m_pCommunityBKImage->GetMinPos().y;
		float fPosX = m_ptCommunityBk.x + COMMUNITY_BTN_POSX;
		float fPosY = m_ptCommunityBk.y + COMMUNITY_BTN_POSY;
		
		POINT pPos = m_pCommunityControl->GetFindControlTargetofMinPos("comFbtn00");
		m_pCommMenuBtn[COMMUNITY_PARTY]->SetBtnPosition(fPosX + pPos.x, fPosY + pPos.y);
		
		pPos = m_pCommunityControl->GetFindControlTargetofMinPos("comFrbtn00");
		m_pCommMenuBtn[COMMUNITY_FRIEND]->SetBtnPosition(fPosX + pPos.x, fPosY + pPos.y);
		
		pPos = m_pCommunityControl->GetFindControlTargetofMinPos("comBbtn00");
		m_pCommMenuBtn[COMMUNITY_GUILD]->SetBtnPosition(fPosX + pPos.x, fPosY + pPos.y);
		
		pPos = m_pCommunityControl->GetFindControlTargetofMinPos("comCbtn00");
		m_pCommMenuBtn[COMMUNITY_CHATROOM]->SetBtnPosition(fPosX + pPos.x, fPosY + pPos.y);
		
		pPos = m_pCommunityControl->GetFindControlTargetofMinPos("comLbtn00");
		m_pCommMenuBtn[COMMUNITY_MAIL]->SetBtnPosition(fPosX + pPos.x, fPosY + pPos.y);
		
		pPos = m_pCommunityControl->GetFindControlTargetofMinPos("comRbtn");
		m_pCommMenuBtn[COMMUNITY_REJECT]->SetBtnPosition(fPosX + pPos.x, fPosY + pPos.y);
	}
	for(int i=0; i < COMMUNITY_MAX; i ++ )
	{
		m_pCommMenuBtn[i]->Render();
	}
#endif
	switch(m_nCommunityType)
	{
	case COMMUNITY_PARTY:
		{
			m_pParty->Render(m_ptCommunityBk);
		}
		break;
	case COMMUNITY_FRIEND:
		{
			m_pFriend->Render(m_ptCommunityBk);
		}
		break;
	case COMMUNITY_GUILD:
		{
			m_pGuild->Render(m_ptCommunityBk);
		}
		break;
	case COMMUNITY_CHATROOM:
		{
			m_pChatRoom->Render(m_ptCommunityBk);
		}
		break;
	case COMMUNITY_REJECT:
		{
			m_pReject->Render(m_ptCommunityBk);
		}
		break;
		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	case COMMUNITY_MAIL:		// ����
		{
			m_pLetter->Render(m_ptCommunityBk);
		}
		break;
		// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	}	
	m_pCloseBtn->Render();				// �ݱ� â	
		
	m_pCommuPartyCreate->Render();// ��� ����
	m_pCommunityUserOpenSetup->Render();// ���� ����
	m_pCommunityUserInfo->Render();// ���� 
	m_pCommuPartyInvite->Render();// ��� �ʴ�	
	m_pCommunityGuildManager->Render();// ������ ����
	m_pCommunityGuildSearch->Render();	// ���� �˻�
	m_pCommunityGuildIntroMe->Render();// �ڱ� �Ұ�

// 2012-06-14 by isshin ���۹̸�����
#ifdef C_ENEMY_INFO_ISSHIN
	m_pEnemyItemInfo->Render();
#endif
// end 2012-06-14 by isshin ���۹̸�����		
}


int CINFCommunity::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â		
	{
		if(INF_MSGPROC_BREAK == m_pCommunityGuildIntroMe->WndProc(uMsg, wParam, lParam))
		{
			// ��ư���� ���콺�� �ִ�.
			return  INF_MSGPROC_BREAK;
		}		
	}
	
	{
		if(INF_MSGPROC_BREAK == m_pCommunityGuildSearch->WndProc(uMsg, wParam, lParam))
		{
			// ��ư���� ���콺�� �ִ�.
			return  INF_MSGPROC_BREAK;
		}		
	}
	{
		if(INF_MSGPROC_BREAK == m_pCommunityGuildManager->WndProc(uMsg, wParam, lParam))
		{
			// ��ư���� ���콺�� �ִ�.
			return  INF_MSGPROC_BREAK;
		}		
	}
	{
		if(INF_MSGPROC_BREAK == m_pCommuPartyInvite->WndProc(uMsg, wParam, lParam))
		{
			// ��ư���� ���콺�� �ִ�.
			return  INF_MSGPROC_BREAK;
		}		
	}
	// ����
	{
		if(INF_MSGPROC_BREAK == m_pCommunityUserInfo->WndProc(uMsg, wParam, lParam))
		{
			// ��ư���� ���콺�� �ִ�.
			return  INF_MSGPROC_BREAK;
		}		
	}
	// 2012-06-14 by isshin ���۹̸�����
	{
		if(INF_MSGPROC_BREAK == m_pEnemyItemInfo->WndProc(uMsg, wParam, lParam))
		{
			// ��ư���� ���콺�� �ִ�.
			return  INF_MSGPROC_BREAK;
		}		
	}// end 2012-06-14 by isshin ���۹̸�����
	{
		if(INF_MSGPROC_BREAK == m_pCommunityUserOpenSetup->WndProc(uMsg, wParam, lParam))
		{
			// ��ư���� ���콺�� �ִ�.
			return  INF_MSGPROC_BREAK;
		}		
	}
	{
		if(INF_MSGPROC_BREAK == m_pCommuPartyCreate->WndProc(uMsg, wParam, lParam))
		{
			// ��ư���� ���콺�� �ִ�.
			return  INF_MSGPROC_BREAK;
		}		
	}	

	
	if(!IsShowCommunityWindow())
	{
		return INF_MSGPROC_NORMAL;
	}	

	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);
			m_pCloseBtn->OnMouseMove(pt);		
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
			for(int i=0; i < COMMUNITY_MAX; i++ )
			{
				if( m_nCommunityType != i )
				{
					if( m_pCommMenuBtn[i]->OnMouseMove( pt ) )
					{

					}
				}
 			} 
#endif
			if(m_bMove)
			{
				m_ptCommunityBk.x = pt.x - m_ptCommOpMouse.x;
				m_ptCommunityBk.y = pt.y - m_ptCommOpMouse.y;				
				// UI���� ���� 
				UpdateBtnPos();
				return INF_MSGPROC_BREAK;
			}
		}
		break;
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
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
			for(int i=0; i < COMMUNITY_MAX; i++ )
			{
			
				if( m_pCommMenuBtn[i]->OnMouseMove( pt ) )
				{
					SetBtnClick( i );
					m_pCommMenuBtn[i]->SetBtnState( BTN_STATUS_SEL );
					continue;
				}
				m_pCommMenuBtn[i]->SetBtnState( BTN_STATUS_UP );				
			}	
#endif		
			if(IsMouseCaps(pt))
			{
				m_ptCommOpMouse.x = pt.x - m_ptCommunityBk.x;
				m_ptCommOpMouse.y = pt.y - m_ptCommunityBk.y;
				m_bMove = TRUE;
				g_pInterface->SetWindowOrder(WNDCommunityWnd);
				return INF_MSGPROC_BREAK;
			}
			int nWindowPosY = m_ptCommunityBk.y;
			// �켱 ���� ����
			// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
//			if( pt.x > 1 && pt.x <219 && pt.y > g_pGameMain->m_nLeftWindowY && pt.y <g_pGameMain->m_nLeftWindowY +275)
//				g_pInterface->SetWindowOrder(WNDLeftRightWindow);

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
			if(pt.y>nWindowPosY + 25 && pt.y<nWindowPosY + 44)
			{
				if(pt.x>=(m_ptCommunityBk.x+20) && pt.x<(m_ptCommunityBk.x+79))
				{
					SetCommunityType(COMMUNITY_PARTY);					
					g_pInterface->SetWindowOrder(WNDCommunityWnd);
					return INF_MSGPROC_BREAK;
				}
				else if(pt.x>=(m_ptCommunityBk.x+79) && pt.x<(m_ptCommunityBk.x+138))
				{
					SetCommunityType(COMMUNITY_FRIEND);			
					g_pInterface->SetWindowOrder(WNDCommunityWnd);
					return INF_MSGPROC_BREAK;
				}
				else if(pt.x>=(m_ptCommunityBk.x+138) && pt.x<(m_ptCommunityBk.x+197) )
				{
					SetCommunityType(COMMUNITY_GUILD);			
					g_pInterface->SetWindowOrder(WNDCommunityWnd);
					return INF_MSGPROC_BREAK;
				}
				else if(pt.x>=(m_ptCommunityBk.x+197) && pt.x<(m_ptCommunityBk.x+256) )
				{				
					// 2009-04-14 by bhsohn ä�� ���� �Ʒ��� ������, ������ ����
					if(g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING )
					{
						// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
						
// 						//"�Ʒ��������� �̿��� �� �����ϴ�."
// 						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203,COLOR_ERROR);//"�Ʒ��������� �̿��� �� �����ϴ�."

						if( g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_ARENA )
							g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203,COLOR_ERROR);//"�Ʒ��������� �̿��� �� �����ϴ�."
						else if( g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_INFINITY )
							g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_091103_0322,COLOR_ERROR);//"\y���Ǵ�Ƽ �ʵ忡���� ��� �� �� �����ϴ�\y"

						// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

						return INF_MSGPROC_BREAK;
					}	
					// end 2009-04-14 by bhsohn ä�� ���� �Ʒ��� ������, ������ ����
					SetCommunityType(COMMUNITY_CHATROOM);
					g_pInterface->SetWindowOrder(WNDCommunityWnd);
					return INF_MSGPROC_BREAK;
				}
				else if(pt.x>=(m_ptCommunityBk.x+256) && pt.x<(m_ptCommunityBk.x+315) )
				{	
					SetCommunityType(COMMUNITY_MAIL);			
					g_pInterface->SetWindowOrder(WNDCommunityWnd);
					return INF_MSGPROC_BREAK;
				}
				else if(pt.x>=(m_ptCommunityBk.x+315) && pt.x<(m_ptCommunityBk.x+374))
				{
					SetCommunityType(COMMUNITY_REJECT);			
					g_pInterface->SetWindowOrder(WNDCommunityWnd);
					return INF_MSGPROC_BREAK;
				}
			}			
#endif		

		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			{
				if(TRUE == m_pCloseBtn->OnLButtonUp(pt))
				{					
					// ��ư Ŭ�� 	
					ShowCommunityWindow(FALSE); // �����
					return  INF_MSGPROC_BREAK;
				}
			}
			if(m_bMove)
			{
				m_bMove = FALSE;
				return INF_MSGPROC_BREAK;
			}
		}
		break;	
	case WM_KEYDOWN:
		{
			if(wParam == VK_ESCAPE)
			{	
				if(m_pCommunityGuildIntroMe->IsShowWindow())
				{
					m_pCommunityGuildIntroMe->ShowWindow(FALSE);
				}
				else if(m_pCommunityGuildSearch->IsShowWindow())
				{
					m_pCommunityGuildSearch->ShowWindow(FALSE);
				}
				else if(m_pCommunityGuildManager->IsShowWindow())
				{
					m_pCommunityGuildManager->ShowWindow(FALSE);
				}
				else if(m_pCommuPartyCreate->IsShowWindow())
				{
					m_pCommuPartyCreate->ShowWindow(FALSE);
				}
				else if(m_pCommunityUserInfo->IsShowWindow())
				{
					m_pCommunityUserInfo->ShowWindow(FALSE);
				}
				// 2012-06-14 by isshin ���۹̸�����
				else if(m_pEnemyItemInfo->IsShowWindow())
				{
					m_pEnemyItemInfo->ShowWindow(FALSE);
				}// end 2012-06-14 by isshin ���۹̸�����
				else if(m_pCommunityUserOpenSetup->IsShowWindow())
				{
					m_pCommunityUserOpenSetup->ShowWindow(FALSE,FALSE);
				}
				else if(m_pCommuPartyInvite->IsShowWindow())
				{
					m_pCommuPartyInvite->ShowWindow(FALSE);
				}
				else
				{
					ShowCommunityWindow(FALSE);
				}				
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	}
	// 2008-07-03 by bhsohn EP3 ���� ����Ʈ ����
	if(WM_LBUTTONDOWN == uMsg && IsShowWnds())
	{
		return INF_MSGPROC_NORMAL;
	}
	// end 2008-07-03 by bhsohn EP3 ���� ����Ʈ ����

	switch(m_nCommunityType)
	{
	case COMMUNITY_PARTY:
		{
			if(m_pParty->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
			{
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	case COMMUNITY_FRIEND:
		{
			if(m_pFriend->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
			{
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	case COMMUNITY_GUILD:
		{
			if(m_pGuild->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
			{
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	case COMMUNITY_REJECT:
		{
			if(m_pReject->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
			{
				return INF_MSGPROC_BREAK;
			}
		}
		break;
		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	case COMMUNITY_MAIL:		// ����
		{			
			if(m_pLetter->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
			{
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	case COMMUNITY_CHATROOM:
		{
			if(m_pChatRoom->WndProc(uMsg,wParam,lParam) == INF_MSGPROC_BREAK)
			{
				return INF_MSGPROC_BREAK;
			}
		}
		break;
		// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	}
	
	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â	
	if((WM_LBUTTONDOWN == uMsg )
		||(WM_RBUTTONDOWN == uMsg))
	{	
		POINT ptBkPos = GetCommunityBkPos();

		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		CheckMouseReverse(&pt);

		// â�ȿ� ���콺 Ŭ���� ��ȿ
		// â�ȿ� ���콺 Ŭ���� ��ȿ			
		if((pt.y >= ptBkPos.y )
			&&(pt.y <= (ptBkPos.y + COMMUNITY_BK_HEIGHT))
			&&(pt.x >= ptBkPos.x)
			&&(pt.x <= (ptBkPos.x+COMMUNITY_BK_WIDTH)))
		{
			g_pInterface->SetWindowOrder(WNDCommunityWnd);

			return INF_MSGPROC_BREAK;
		}
	}
	// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	return INF_MSGPROC_NORMAL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
/// \date		2008-04-04 ~ 2008-04-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunity::ShowCommunityWindow(BOOL bShow, int nSubTyepe/*=COMMUNITY_PARTY*/)
{
	m_bShowWnd = bShow;	

	m_bMove = FALSE;		// ��Ʈ���� �����δ�.

	// ����Ʈ ��Ʈ�� �ʱ�ȭ
	m_pReject->InitEditBox();
	if(bShow)
	{	
		// 2009. 08. 19 by jsKim ���� �� �޴� ������ ��� Ŀ���� ������ �ʴ� ����
		// 2012-12-17 by jhjang ������ ���콺 Ŀ���� ����� �������� �ʴ� ���� ����
		//g_INFCnt++;
		g_pD3dApp->AddINFCnt();
		// end 2012-12-17 by jhjang ������ ���콺 Ŀ���� ����� �������� �ʴ� ���� ����
		g_pGameMain->m_bChangeMousePoint = TRUE;
		// end 2009. 08. 19 by jsKim ���� �� �޴� ������ ��� Ŀ���� ������ �ʴ� ����
		m_ptCommunityBk.x = (g_pD3dApp->GetBackBufferDesc().Width/2) - COMMUNITY_BK_WIDTH/2;
		m_ptCommunityBk.y = (g_pD3dApp->GetBackBufferDesc().Height/2) - COMMUNITY_BK_HEIGHT/2;	

		g_pInterface->SetWindowOrder(WNDCommunityWnd);
		// ���ϸ� ����Ʈ�� ���â����
		SetCommunityType(nSubTyepe);		

		UpdateBtnPos();

		// 2013-01-03 by mspark, ���� �޴��� Ŭ���Ͽ��� ��� ���尡 ���ļ� �鸮�� ���� ����
		g_pD3dApp->m_pSound->PlayD3DSound(SOUND_OPEN_MENU, g_pShuttleChild->m_vPos, FALSE);
		// end 2013-01-03 by mspark, ���� �޴��� Ŭ���Ͽ��� ��� ���尡 ���ļ� �鸮�� ���� ����
	}
	else
	{
		// â�� �����.
		// 2009. 08. 19 by jsKim ���� �� �޴� ������ ��� Ŀ���� ������ �ʴ� ����
		// 2012-12-17 by jhjang ������ ���콺 Ŀ���� ����� �������� �ʴ� ���� ����
		//g_INFCnt--;
		//if(g_INFCnt==0)
		if(g_pD3dApp->RemoveINFCnt() == 0)
		{
			g_pGameMain->m_bChangeMousePoint = FALSE;
		}
		// end 2012-12-17 by jhjang ������ ���콺 Ŀ���� ����� �������� �ʴ� ���� ����
		// end 2009. 08. 19 by jsKim ���� �� �޴� ������ ��� Ŀ���� ������ �ʴ� ����
		m_pGuild->CloseGuildWnd();

		g_pD3dApp->EnableChatMode(FALSE, TRUE);
		// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
		g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
		// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����

		// 2013-01-03 by mspark, ���� �޴��� Ŭ���Ͽ��� ��� ���尡 ���ļ� �鸮�� ���� ����
		g_pD3dApp->m_pSound->PlayD3DSound(SOUND_CLOSE_MENU, g_pShuttleChild->m_vPos, FALSE);
		// end 2013-01-03 by mspark, ���� �޴��� Ŭ���Ͽ��� ��� ���尡 ���ļ� �鸮�� ���� ����
	}
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
/// \date		2008-04-04 ~ 2008-04-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFCommunity::IsShowCommunityWindow()
{
	return m_bShowWnd;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
/// \date		2008-04-04 ~ 2008-04-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
POINT CINFCommunity::GetCommunityBkPos()
{
	return m_ptCommunityBk;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
/// \date		2008-04-04 ~ 2008-04-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunity::SetCommunityType(int nType)
{ 
	m_nCommunityType = nType; 
	// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
	g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
	// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	switch(m_nCommunityType)
	{
	case COMMUNITY_PARTY:
		{				
			m_pParty->ShowPartyFrame();
		}
		break;
		// 2009-01-14 by bhsohn ģ�� ����Ʈ �ӼӸ� ��� �߰�
	case COMMUNITY_FRIEND:
		{
			m_pFriend->RefreshFriendInfo();
		}
		break;
		// end 2009-01-14 by bhsohn ģ�� ����Ʈ �ӼӸ� ��� �߰�
	case COMMUNITY_MAIL:		// ����
		{
			m_pLetter->RefreshLetterInfo();
		}
		break;	
	case COMMUNITY_GUILD:// ��� 
		{
			m_pGuild->RefreshGuildInfo();
		}
		break;
	case COMMUNITY_CHATROOM:
		{
			if(g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING )
			{
				// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
				if( g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_ARENA )
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203,COLOR_ERROR);//"�Ʒ��������� �̿��� �� �����ϴ�."
				else if( g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_INFINITY )
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_091103_0322,COLOR_ERROR);//"\y���Ǵ�Ƽ �ʵ忡���� ��� �� �� �����ϴ�\y"				
				// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
				break;
			}
			m_pChatRoom->InitSetting();
		}
		break;
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� ��� Ŭ��
/// \author		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
/// \date		2008-05-13 ~ 2008-05-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunity::OnClickCurrentParty()
{
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// ��� ����
/// \author		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
/// \date		2008-05-13 ~ 2008-05-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunity::OnPopupCreateParty()
{
	BOOL bShow = m_pCommuPartyCreate->IsShowWindow();
	bShow ^= TRUE;
	m_pCommuPartyCreate->ShowWindow(bShow);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// ��� ����
/// \author		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
/// \date		2008-05-13 ~ 2008-05-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunity::OnPopupInviteParty()
{
	BOOL bShow = m_pCommuPartyInvite->IsShowWindow();
	bShow ^= TRUE;
	m_pCommuPartyInvite->ShowWindow(bShow);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// ��� ����
/// \author		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
/// \date		2008-05-13 ~ 2008-05-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunity::OnClickCreateParty()
{
	// ��� ����
	if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _NOPARTY &&
					!g_pGameMain->m_pInfWindow->IsExistMsgBox(_Q_PARTY_CREATE))
	{
		g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_PARTY_0023, _Q_PARTY_CREATE);//"�������� �غ��Ͻðڽ��ϱ�?"
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFCommunity::SetAutoCreateParty()
/// \brief		/// 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ڵ���Ƽ : �ڵ����� ��Ƽ�����.
/// \author		jhseol
/// \date		2012-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunity::SetAutoCreateParty(SPARTY_INFO i_PartyInfo)
{/// 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ڵ���Ƽ : �ڵ����� ��Ƽ�����.
	m_pCommuPartyCreate->SetAutoCreateParty(i_PartyInfo);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// ���� �ʴ�
/// \author		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
/// \date		2008-05-13 ~ 2008-05-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunity::PartySendInviteUser(char* pUserName)
{
	util::strncpy(g_pGameMain->m_pCommunity->GetParty()->m_strInviteUser, pUserName, 20);
	g_pGameMain->m_pCommunity->GetParty()->PartySendInviteUser();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// ��ƼŻ��
/// \author		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
/// \date		2008-05-13 ~ 2008-05-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunity::OnClickSecedeParty()
{
	if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType != _NOPARTY &&
				!g_pGameMain->m_pInfWindow->IsExistMsgBox(_Q_PARTY_INVITE_USER))
	{
		g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_PARTY_0025, _Q_PARTY_SECEDE);//"��뿡�� Ż���Ͻðڽ��ϱ�?"
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// ���� ����
/// \author		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
/// \date		2008-05-13 ~ 2008-05-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunity::PartySendBanUser()
{
	FLOG( "CINFCommunityParty::PartySendBanUser()" );
	if(g_pShuttleChild->m_pClientParty->GetPartyInfo().PartyID != 0)
	{
		UID32_t nPartyBanCharacterUniqueNumber = m_pParty->GetPartyBanCharacterUniqueNumber();
		g_pShuttleChild->m_pClientParty->ISendPartyBanMember(nPartyBanCharacterUniqueNumber);
		m_pParty->SetPartyBanCharacterUniqueNumber(0);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// ���� ����
/// \author		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
/// \date		2008-05-13 ~ 2008-05-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunity::PartySendTransferMaster()
{
	FLOG( "CINFCommunityParty::PartySendTransferMaster()" );
	if(g_pShuttleChild->m_pClientParty->GetPartyInfo().PartyID != 0)
	{
		UID32_t nPartyTransferMasterCharacterUniqueNumber = m_pParty->GetPartyTransferMasterCharacterUniqueNumber();
		g_pShuttleChild->m_pClientParty->ISendPartyTransferMaster(nPartyTransferMasterCharacterUniqueNumber);
		m_pParty->SetPartyTransferMasterCharacterUniqueNumber(0);
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// ���� ������
/// \author		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
/// \date		2008-05-13 ~ 2008-05-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunity::OnClickLetterWrite(char* pUser)
{
	SetCommunityType(COMMUNITY_MAIL);
	m_pLetter->ChangeLetterMode(LETTER_MODE_WRITE, 0, pUser, FALSE, 0);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// ��� ����â
/// \author		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
/// \date		2008-05-13 ~ 2008-05-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunity::OnPopupGuildManager()
{
	if(m_pCommunityGuildSearch->IsShowWindow())
	{
		m_pCommunityGuildSearch->ShowWindow(FALSE);
	}

	BOOL bShow = m_pCommunityGuildManager->IsShowWindow();
	bShow ^= TRUE;
	m_pCommunityGuildManager->ShowWindow(bShow);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// ��� �˻�â
/// \author		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
/// \date		2008-05-13 ~ 2008-05-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunity::OnPopupGuildSearch()
{	
	if(m_pCommunityGuildManager->IsShowWindow())
	{
		m_pCommunityGuildManager->ShowWindow(FALSE);
	}

	BOOL bShow = m_pCommunityGuildSearch->IsShowWindow();
	bShow ^= TRUE;
	m_pCommunityGuildSearch->ShowWindow(bShow);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// �ڱ� �Ұ�â
/// \author		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
/// \date		2008-05-13 ~ 2008-05-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunity::OnPopupIntroMe()
{	
	BOOL bShow = m_pCommunityGuildIntroMe->IsShowWindow();
	bShow ^= TRUE;
	m_pCommunityGuildIntroMe->ShowWindow(bShow);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// ��� ����â
/// \author		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
/// \date		2008-05-13 ~ 2008-05-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunity::GetUnitKindString(USHORT i_nUnitKind,char* pTxt)
{
	switch(i_nUnitKind)
	{
	case UNITKIND_BT01:
	case UNITKIND_BT02:
	case UNITKIND_BT03:
	case UNITKIND_BT04:
		wsprintf(pTxt, "B");		
		break;
	case UNITKIND_DT01:
	case UNITKIND_DT02:
	case UNITKIND_DT03:
	case UNITKIND_DT04:
		wsprintf(pTxt, "A");
		break;
	case UNITKIND_ST01:
	case UNITKIND_ST02:
	case UNITKIND_ST03:
	case UNITKIND_ST04:
		wsprintf(pTxt, "I");
		break;
	case UNITKIND_OT01:
	case UNITKIND_OT02:
	case UNITKIND_OT03:
	case UNITKIND_OT04:
		wsprintf(pTxt, "M");
		break;
	default:
		{
			wsprintf(pTxt, " ");
		}
		break;
	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// ������ ���� UID�� ���´�.
/// \author		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
/// \date		2008-05-13 ~ 2008-05-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFCommunity::GetGuildSearchSelGuidUID(UID32_t *pUID)
{
	return m_pCommunityGuildSearch->GetGuildSearchSelGuidUID(pUID);
}

char* CINFCommunity::GetGuildName(UINT nGuildUniqueNumber)
{
	return g_pDatabase->GetGuildName(nGuildUniqueNumber);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// ������ ���� UID�� ���´�.
/// \author		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
/// \date		2008-05-13 ~ 2008-05-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFCommunity::GetGuildSearchSelGuidName(char* pGuildName)
{
	return m_pCommunityGuildSearch->GetGuildSearchSelGuidName(pGuildName);
}

void CINFCommunity::GetFormatString(int nSelTxt, char* pDst)
{
//	switch(nSelTxt)
//	{
//	case FLIGHT_FORM_NONE:	// ��� ���� �� ��
//		{
//			wsprintf(pDst, STRMSG_C_080603_0204);
//		}
//		break;
//	case FLIGHT_FORM_2_COLUMN:	// �̷� ����, �̷� ���� ������� �� �ٷ� ������ �� ����̴�
//		{
//			wsprintf(pDst, STRMSG_C_080603_0205);
//		}
//		break;
//	case FLIGHT_FORM_2_LINE:	// �̷� Ⱦ��, �̷� Ⱦ�� ������� �� �ٷ� ������ �� ����̴�
//		{
//			wsprintf(pDst, STRMSG_C_080603_0206);
//		}
//		break;
//	case FLIGHT_FORM_TRIANGLE:	// �ﰢ ���, �ﰢ�� ������� ��ܺ��� 1, 2, 3���� ������ ��ġ�Ѵ�
//		{
//			wsprintf(pDst, STRMSG_C_080603_0207);
//		}
//		break;
//	case FLIGHT_FORM_INVERTED_TRIANGLE:	// ���ﰢ ���, �� �ﰢ�� ������� ��ܺ��� 3, 2, 1���� ������ ��ġ�Ѵ�
//		{
//			wsprintf(pDst, STRMSG_C_080603_0208);
//		}
//		break;
//	case FLIGHT_FORM_BELL:			// �� ����, �� ������� ��ܺ��� 1, 3, 2���� ������ ��ġ�Ѵ�
//		{
//			wsprintf(pDst, STRMSG_C_080603_0209);
//		}
//		break;
//	case FLIGHT_FORM_INVERTED_BELL:	// ���� ����, ���� ������� ��ܺ��� 2, 3, 1���� ������ ��ġ�Ѵ�
//		{
//			wsprintf(pDst, STRMSG_C_080603_0210);
//		}
//		break;
//	case FLIGHT_FORM_X:				// X�� ����
//		{
//			wsprintf(pDst, STRMSG_C_080603_0211);
//		}
//		break;
//	case FLIGHT_FORM_STAR:			// �� ����
//		{
//			wsprintf(pDst, STRMSG_C_080603_0212);
//		}
//		break;		
//	default:
//		{
//			wsprintf(pDst, " ");
//		}
//		break;
//	}
	wsprintf(pDst, " ");
}

void CINFCommunity::RqMemberInfo(int nWndIdx, UID32_t	CharacterUID)
{
	m_nRqMemInfoWnd = nWndIdx;

	// Lock
	{
		vector<MessageType_t> vecUnLockMsg;
		vecUnLockMsg.clear();
		vecUnLockMsg.push_back(T_IC_GUILD_GET_SELF_INTRODUCTION);		
		vecUnLockMsg.push_back(T_IC_GUILD_GET_SELF_INTRODUCTION_OK);		
		// �ϳ��� ��Ī �Ǿ ���� Ǯ����. 
		g_pD3dApp->EnterMultiLock(FALSE, T_IC_GUILD_GET_SELF_INTRODUCTION, vecUnLockMsg, MULTI_LOCK_ONE_MATCHING);
	}
	MSG_IC_GUILD_GET_SELF_INTRODUCTION sMsg;
	memset(&sMsg, 0x00, sizeof(MSG_IC_GUILD_GET_SELF_INTRODUCTION));
	sMsg.CharacterUID = CharacterUID;
	g_pIMSocket->SendMsg(T_IC_GUILD_GET_SELF_INTRODUCTION, (char*)&sMsg, sizeof(sMsg));

}

void CINFCommunity::IMSocketGetSelfIntroductionOK(MSG_IC_GUILD_GET_SELF_INTRODUCTION_OK* pMsg)
{
	switch(m_nRqMemInfoWnd)
	{
	case COMMUNITY_MEMINFO_MANAGER:
		{
			m_pCommunityGuildManager->IMSocketGetSelfIntroductionOK(pMsg);
		}
		break;
	case COMMUNITY_MEMINFO_SEARCH:
		{
			m_pCommunityGuildSearch->IMSocketGetSelfIntroductionOK(pMsg);
		}
		break;
	}
	
}
MSG_IC_GUILD_GET_SELF_INTRODUCTION_OK* CINFCommunity::GetMySelfIntro()
{
	return m_pCommunityGuildSearch->GetMySelfIntro();
}

void CINFCommunity::PopupRqCancelGuild()
{
	char chBuff[256];
	memset(chBuff, 0x00, 256);
	
	MSG_IC_GUILD_GET_SELF_INTRODUCTION_OK* pInfo = GetMySelfIntro();
	wsprintf(chBuff,STRMSG_C_080527_0204, pInfo->GuildName);	
	g_pGameMain->m_pInfWindow->AddMsgBox(chBuff, _Q_GUILD_REQUEST_CANCEL);	// "\\y%s\\y���ܿ� ���� ��û�� ����Ͻðڽ��ϱ�?"
}

void CINFCommunity::RqCancelMyGuildIntro()
{	
	// Lock
	{
		vector<MessageType_t> vecUnLockMsg;
		vecUnLockMsg.clear();
		vecUnLockMsg.push_back(T_IC_GUILD_DELETE_SELFINTRODUCTION);		
		vecUnLockMsg.push_back(T_IC_GUILD_DELETE_SELFINTRODUCTION_OK);		
		// �ϳ��� ��Ī �Ǿ ���� Ǯ����. 
		g_pD3dApp->EnterMultiLock(FALSE, T_IC_GUILD_DELETE_SELFINTRODUCTION, vecUnLockMsg, MULTI_LOCK_ONE_MATCHING);
	}
	g_pIMSocket->SendMsg(T_IC_GUILD_DELETE_SELFINTRODUCTION, NULL, NULL);
	
	ShowGuildSearch(FALSE);

}
void CINFCommunity::ShowGuildSearch(BOOL bShow)
{
	m_pCommunityGuildSearch->ShowWindow(bShow);

}	
INT CINFCommunity::GetChatRoomNum()
{
	return m_pChatRoom->m_stMyRoomInfo.ChatRoomNum;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� ���� ����
/// \author		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
/// \date		2008-05-13 ~ 2008-05-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunity::OnPopupUserOpenSetup(BOOL bCharcterWnd)
{
	BOOL bShow = m_pCommunityUserOpenSetup->IsShowWindow();
	bShow ^= TRUE;
	m_pCommunityUserOpenSetup->ShowWindow(bShow,bCharcterWnd);
	if(bShow)
	{
		g_pInterface->SetWindowOrder(WNDCommunityWnd);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� ���� ����
/// \author		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
/// \date		2008-05-13 ~ 2008-05-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunity::ShowUserInfo(MSG_FC_CHARACTER_GET_USER_INFO_OK* pMsg)
{	
	m_pCommunityUserInfo->ShowUserInfo(pMsg);
}

// 2012-06-14 by isshin ���۹̸�����
///////////////////////////////////////////////////////////////////////////////
/// \brief		Ÿ���� ������ ����
///////////////////////////////////////////////////////////////////////////////
void CINFCommunity::ShowEnemyItemInfo(MSG_FC_CHARACTER_GET_USER_ITEM_INFO_OK_DONE* pMsg)
{	
	m_pEnemyItemInfo->InitEnemyEquipItemInfo();
	m_pEnemyItemInfo->Set_bItemInfoEnable(FALSE);
	m_pEnemyItemInfo->ShowEnemyItemInfo();
	m_pEnemyItemInfo->SetEnchantInfo(pMsg);
}

void CINFCommunity::UpdateEnemyItemInfo(BOOL bUpdatEnemyItemInfo, ClientIndex_t ClientIndex)
{
	m_pEnemyItemInfo->Set_bChangeEquipItem(bUpdatEnemyItemInfo, ClientIndex);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief		Ÿ���� ������ ���� ��û
///////////////////////////////////////////////////////////////////////////////
void CINFCommunity::RqCharacterUserItemInfo(UID32_t	TargetCharcterUID, ClientIndex_t TargetCharcterCID)
{
	m_pEnemyItemInfo->RqCharacterUserItemInfo(TargetCharcterUID, TargetCharcterCID);
}
// end 2012-06-14 by isshin ���۹̸�����

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� ���� ��û
/// \author		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
/// \date		2008-05-13 ~ 2008-05-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunity::RqCharacterUserInfo(UID32_t	TargetCharcterUID)
{
	{
		vector<MessageType_t> vecUnLockMsg;
		vecUnLockMsg.clear();
		vecUnLockMsg.push_back(T_FC_CHARACTER_GET_USER_INFO);		
		vecUnLockMsg.push_back(T_FC_CHARACTER_GET_USER_INFO_OK);		
		// �ϳ��� ��Ī �Ǿ ���� Ǯ����. 
		g_pD3dApp->EnterMultiLock(TRUE, T_FC_CHARACTER_GET_USER_INFO, vecUnLockMsg, MULTI_LOCK_ONE_MATCHING);
	}

	MSG_FC_CHARACTER_GET_USER_INFO sMsg;
	memset(&sMsg,0x00,sizeof(MSG_FC_CHARACTER_GET_USER_INFO));	

	sMsg.TargetCharcterUID = TargetCharcterUID;
	
	g_pFieldWinSocket->SendMsg( T_FC_CHARACTER_GET_USER_INFO, (char*)&sMsg, sizeof(sMsg) );
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� ���� ���� 
/// \author		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
/// \date		2008-05-13 ~ 2008-05-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunity::RqChangeOpenUserInfo(INT SecretInfoOption)
{
	{
		vector<MessageType_t> vecUnLockMsg;
		vecUnLockMsg.clear();
		vecUnLockMsg.push_back(T_FC_CHARACTER_CHANGE_INFO_OPTION_SECRET);		
		vecUnLockMsg.push_back(T_FC_CHARACTER_CHANGE_INFO_OPTION_SECRET_OK);		
		// �ϳ��� ��Ī �Ǿ ���� Ǯ����. 
		g_pD3dApp->EnterMultiLock(TRUE, T_FC_CHARACTER_CHANGE_INFO_OPTION_SECRET, vecUnLockMsg, MULTI_LOCK_ONE_MATCHING);
	}

	MSG_FC_CHARACTER_CHANGE_INFO_OPTION_SECRET sMsg;
	memset(&sMsg,0x00,sizeof(MSG_FC_CHARACTER_CHANGE_INFO_OPTION_SECRET));	

	sMsg.SecretInfoOption = SecretInfoOption;
	
	g_pFieldWinSocket->SendMsg( T_FC_CHARACTER_CHANGE_INFO_OPTION_SECRET, (char*)&sMsg, sizeof(sMsg) );	
	m_nLastRqSecretInfoOption = SecretInfoOption;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		T_FC_CHARACTER_CHANGE_INFO_OPTION_SECRET�� ���� ����
/// \author		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
/// \date		2008-05-13 ~ 2008-05-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunity::FieldSocketCharacterChangeInfoOptionSecetOk()
{
	if(m_nLastRqSecretInfoOption>= 0)
	{
		g_pShuttleChild->SetSecretInfoOption(m_nLastRqSecretInfoOption);
	}
	m_nLastRqSecretInfoOption = -1;

}
// 2008-12-09 by dgwoo �̼Ǹ�����.
void CINFCommunity::SetMissionMasterOption(BOOL i_bMissionMaster)
{
	const auto& ShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
	int nSecretInfoOption = ShuttleInfo.SecretInfoOption;
	//int nTempSecretInfoOption = 0;
	if(i_bMissionMaster)
	{
		//nTempSecretInfoOption = nSecretInfoOption;
		nSecretInfoOption |= USER_INFO_OPTION_MISSIONMASTER;
	}
	else
	{
		//nTempSecretInfoOption =0xFFFFFFFF;
		nSecretInfoOption &= ~USER_INFO_OPTION_MISSIONMASTER;
	}
	
	RqChangeOpenUserInfo(nSecretInfoOption);
	m_nLastRqSecretInfoOption = nSecretInfoOption;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
/// \date		2008-05-13 ~ 2008-05-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFCommunity::IsShowWnds()
{
	if(m_pCommunityGuildIntroMe->IsShowWindow())
	{
		// ��ư���� ���콺�� �ִ�.
		return  TRUE;
	}		
	else if(m_pCommunityGuildSearch->IsShowWindow())
	{
		// ��ư���� ���콺�� �ִ�.
		return  TRUE;
	}			
	else if(m_pCommunityGuildManager->IsShowWindow())
	{
		// ��ư���� ���콺�� �ִ�.
		return  TRUE;
	}	
	else if(m_pCommuPartyInvite->IsShowWindow())
	{
		// ��ư���� ���콺�� �ִ�.
		return  TRUE;
	}			
// 2009. 12. 04 by jskim �ٸ� ������ ���� ������ ���� ������ community-window�� ���� ���� ���� �ذ�
// 	else if(m_pCommunityUserInfo->IsShowWindow())
// 	{
// 		// ��ư���� ���콺�� �ִ�.
// 		return  TRUE;
// 	}		
//end 2009. 12. 04 by jskim �ٸ� ������ ���� ������ ���� ������ community-window�� ���� ���� ���� �ذ�
	else if(m_pCommunityUserOpenSetup->IsShowWindow())
	{
		// ��ư���� ���콺�� �ִ�.
		return  TRUE;
	}		
	else if(m_pCommuPartyCreate->IsShowWindow())
	{
		// ��ư���� ���콺�� �ִ�.
		return  TRUE;
	}	
		
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-07-14 by bhsohn EP3 ���� ó��
/// \date		2008-07-14 ~ 2008-07-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunity::UpdateBtnPos()
{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	float fPosX = m_ptCommunityBk.x + COMMUNITY_BK_WIDTH-m_pCloseBtn->GetImgSize().x;
	float fPosY = m_ptCommunityBk.y + 6;		
#else	 
	float fPosX = m_ptCommunityBk.x + COMMUNITY_BK_WIDTH-m_pCloseBtn->GetImgSize().x -2;
	float fPosY = m_ptCommunityBk.y + 21;		
#endif
	m_pCloseBtn->SetBtnPosition(fPosX, fPosY);

	m_pParty->UpdateBtnPos();
	m_pFriend->UpdateBtnPos();
	m_pGuild->UpdateBtnPos();
	m_pReject->UpdateBtnPos();
	m_pLetter->UpdateBtnPos();
	m_pChatRoom->UpdateBtnPos();	

}

BOOL CINFCommunity::IsMouseCaps(POINT ptPos)
{
	POINT ptBakPos = GetCommunityBkPos();
	if((ptPos.x >= ptBakPos.x && (ptPos.x <= ptBakPos.x+COMMUNITY_BK_WIDTH))
		&& (ptPos.y >= ptBakPos.y && (ptPos.y <= ptBakPos.y+COMMUNITY_CAPS_HEIGHT)))
	{
		return TRUE;
	}
	return FALSE;

}

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
int CINFCommunity::SetBtnClick( int nNum )
{
	switch(nNum)
	{
	case COMMUNITY_PARTY:
		{
			SetCommunityType(COMMUNITY_PARTY);					
			g_pInterface->SetWindowOrder(WNDCommunityWnd);
			return INF_MSGPROC_BREAK;
		}
		break;
	case COMMUNITY_FRIEND:
		{
			SetCommunityType(COMMUNITY_FRIEND);			
			g_pInterface->SetWindowOrder(WNDCommunityWnd);
			return INF_MSGPROC_BREAK;
		}
		break;
	case COMMUNITY_GUILD:
		{
			SetCommunityType(COMMUNITY_GUILD);			
			g_pInterface->SetWindowOrder(WNDCommunityWnd);
			
		}
		break;
	case COMMUNITY_CHATROOM:
		{
				SetCommunityType(COMMUNITY_CHATROOM);
			g_pInterface->SetWindowOrder(WNDCommunityWnd);
			return INF_MSGPROC_BREAK;
		}
		break;
	case COMMUNITY_MAIL:
		{
			SetCommunityType(COMMUNITY_MAIL);			
			g_pInterface->SetWindowOrder(WNDCommunityWnd);
			return INF_MSGPROC_BREAK;
		}
		break;
	case COMMUNITY_REJECT:
		{
			SetCommunityType(COMMUNITY_REJECT);			
			g_pInterface->SetWindowOrder(WNDCommunityWnd);
			return INF_MSGPROC_BREAK;
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

// 2015-06-27 Future, hides opened User Info again
void CINFCommunity::HideUserInfo()
{
	m_pCommunityUserInfo->HideUserInfo();
}

#endif