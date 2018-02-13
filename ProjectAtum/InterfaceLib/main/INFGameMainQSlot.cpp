// INFGameMainQSlot.cpp: implementation of the CINFGameMainQSlot class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFGameMainQSlot.h"
#include "AtumApplication.h"
#include "INFTrade.h"
#include "INFIcon.h"
#include "StoreData.h"
// 2008-09-22 by bhsohn EP3 ĳ���� â
//#include "INFCharacterInfo.h"
#include "INFCharacterInfoExtend.h"
#include "AtumDatabase.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "Interface.h"
#include "INFGameMain.h"
#include "Chat.h"
#include "GameDataLast.h"
#include "KeyBoardInput.h"
#include "ItemInfo.h"
#include "RangeTime.h"
#include "INFInven.h"
#include "INFImage.h"
#include "INFCityBase.h"
#include "Skill.h"
#include "D3DHanFont.h"
#include "SkillInfo.h"
#include "ClientParty.h"
#include "Skill.h"
#include "dxutil.h"
#include "INFGroupImage.h"
#include "INFImageEx.h"
#include "INFImageBtn.h"
#include "INFWindow.h"	// 2012-12-21 by bhsohn [�巹�θ��] ������ ���� ��� �ȵǰ� ����
		  
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
#define QSLOT_START_X			336 + ( ( g_pD3dApp->GetBackBufferDesc().Width - ( 330 + 335 + 336) ) / 2)		// c_nbar + mnbtnBG + mnQSlot
#define QSLOT_START_Y			(g_pD3dApp->GetBackBufferDesc().Height - 57)
#define QSLOT_GAP				(m_pBack->GetImgSize().y + 4)

#define REAL_TAB_NUMBER			3	// ���� ����Ǵ� �� ����

// 2007-07-09 by bhsohn ��ݰ� ���ڻ��� ���� ���� ������ ó��
#define	BAZAAR_CLICK_TIME		2.0f

#define QSLOT_BUTTON_UP_START_X			(m_nX + QSLOT_SIZE_X + 7)
#define QSLOT_BUTTON_UP_START_Y			(m_nY + 12)
#define QSLOT_BUTTON_DOWN_START_X		(m_nX + QSLOT_SIZE_X + 4)
#define QSLOT_BUTTON_DOWN_START_Y		(m_nY + 23)
#define QSLOT_BUTTON_SIZE_X				8
#define QSLOT_BUTTON_SIZE_Y				8
#else		 
#define QSLOT_START_X			((g_pD3dApp->GetBackBufferDesc().Width - QSLOT_SIZE_X)/2)
#define QSLOT_START_Y			(g_pD3dApp->GetBackBufferDesc().Height - 34)
#define REAL_TAB_NUMBER			3	// ���� ����Ǵ� �� ����

// 2007-07-09 by bhsohn ��ݰ� ���ڻ��� ���� ���� ������ ó��
#define	BAZAAR_CLICK_TIME		2.0f
#endif

// 2006-03-07 by ispark, �� ���� ��ġ ����
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn �±� ���� �߰�
#define FONTLINE_X				0
#define FONTLINE_Y				-1
#else
#define FONTLINE_X				1
#define FONTLINE_Y				1
#endif

// 2012-12-21 by bhsohn [�巹�θ��] ������ ���� ��� �ȵǰ� ����
#define		DRAIN_MODULE_ITEMNUM_0		7027200
#define		DRAIN_MODULE_ITEMNUM_1		7027220		// ��Ƽ �巹�� ���
#define		DRAIN_MODULE_ITEMNUM_2		7027230		// ��Ƽ �̷��� ���
// END 2012-12-21 by bhsohn [�巹�θ��] ������ ���� ��� �ȵǰ� ����


// 2013-07-01 by bhsohn QuickSlot����� �ð� üũ�Ͽ� �ٷ� �����ϰ� ����
#define		QUICKSLOT_SAVE_CHECK_TIME		(60.0f)
// END 2013-07-01 by bhsohn QuickSlot����� �ð� üũ�Ͽ� �ٷ� �����ϰ� ����

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFGameMainQSlot::CINFGameMainQSlot(CAtumNode* pParent)
{
	m_pParent = pParent;
	m_pBack = NULL;
	m_pNumber = NULL;
	m_pImgDisSkill = NULL;
         m_nRenderMoveIconIntervalWidth = 0;
         m_nRenderMoveIconIntervalHeight = 0;
	memset(m_pQSlotInfo, 0x00, sizeof(INVEN_DISPLAY_INFO)*QSLOT_NUMBER*QSLOT_TAB_NUMBER);
	m_nX = 0;
	m_nY = 0;
	m_nCurrentTab = 0;
	m_nSelectSlotNumber = -1;
	m_fQSlotTimer = 0;
	m_bRestored = FALSE;
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	memset(m_pImgTabButton, 0x00, sizeof(DWORD)*QSLOT_BUTTON_STATE_NUMBER*QSLOT_BUTTON_NUMBER);
	m_nButtonState[QSLOT_BUTTON_UP] = BUTTON_STATE_NORMAL;
	m_nButtonState[QSLOT_BUTTON_DOWN] = BUTTON_STATE_NORMAL;
#endif
	
	for(int i=0; i<8; i++)
		m_vecFontLine[i] = NULL;
	m_pSelectItem.pItem = NULL;
	m_nQSlotSwapTab = 0;
	m_nQSlotSwapNum = 0;
	m_bQSlotSwapFlag = FALSE;
	m_nItemType = QSLOT_ITEMTYPE_NONE;

	m_pQSlotMove = NULL;
	m_bLButtonDown = FALSE;
	m_pFontTabNum = NULL;

	// 2007-07-09 by bhsohn ��ݰ� ���ڻ��� ���� ���� ������ ó��
	m_fClickBazaar = -1;

	// 2008-11-13 by bhsohn ���̽�ƽ �۾�
	m_vecJoystikcSkillList.clear();
	m_fJoystikcSkillList = 0.0f;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pImgBlind = NULL;
	m_pIsSlotOpen = TRUE;
#endif

	m_fCheckQuiclSlotSave = 0.0f; // 2013-07-01 by bhsohn QuickSlot����� �ð� üũ�Ͽ� �ٷ� �����ϰ� ����

}

CINFGameMainQSlot::~CINFGameMainQSlot()
{
	int i;
	util::del(m_pBack);
	util::del(m_pNumber);
	util::del(m_pImgDisSkill);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	for(i=0;i<QSLOT_BUTTON_NUMBER;i++)
	{
		util::del(m_pImgTabButton[i]);
	}
#else
	for(i=0;i<QSLOT_BUTTON_STATE_NUMBER;i++)
	{
		util::del(m_pImgTabButton[QSLOT_BUTTON_UP][i]);
		util::del(m_pImgTabButton[QSLOT_BUTTON_DOWN][i]);
	}
#endif

	for(i=0; i<QSLOT_NUMBER; i++)
	{
		util::del(m_vecFontLine[i]);
	}
	util::del(m_pFontTabNum);

	for(i=0;i<QSLOT_TAB_NUMBER;i++)
	{
		for(int j=0;j<QSLOT_NUMBER;j++)
		{
			util::del(m_pQSlotInfo[i][j].pItem);
		}
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	util::del( m_pImgBlind );																	  
#endif
}

HRESULT CINFGameMainQSlot::InitDeviceObjects()
{
	FLOG( "CINFGameMainQSlot::InitDeviceObjects()" );
	DataHeader	* pDataHeader;
	
	int i;
	pDataHeader = FindResource("mnQSlot");
	m_pBack = new CINFImageEx;
	m_pBack->InitDeviceObjects(pDataHeader) ;
	
	m_pNumber = new CINFImageEx;
	pDataHeader = FindResource("mnQSlotN");
	m_pNumber->InitDeviceObjects(pDataHeader) ;
	
	m_pImgDisSkill = new CINFImageEx;
	pDataHeader = FindResource("diskill");
	m_pImgDisSkill->InitDeviceObjects(pDataHeader);
												
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pImgTabButton[QSLOT_BUTTON_UP] = new CINFImageBtn;
	m_pImgTabButton[QSLOT_BUTTON_UP]->InitDeviceObjects( "mnQup03", "mnQup01", "mnQup00", "mnQup02" );
		
	m_pImgTabButton[QSLOT_BUTTON_DOWN] = new CINFImageBtn;
	m_pImgTabButton[QSLOT_BUTTON_DOWN]->InitDeviceObjects( "mnQdn03", "mnQdn01", "mnQdn00", "mnQdn02" );
#else			
	for(i=0;i<QSLOT_BUTTON_STATE_NUMBER;i++)
	{
		char buf[64];
		wsprintf( buf, "mnQup%02d", i);
		pDataHeader = FindResource(buf);
		m_pImgTabButton[QSLOT_BUTTON_UP][i] = new CINFImage;
		m_pImgTabButton[QSLOT_BUTTON_UP][i]->InitDeviceObjects( pDataHeader->m_pData, pDataHeader->m_DataSize );
		wsprintf( buf, "mnQdn%02d", i);
		pDataHeader = FindResource(buf);
		m_pImgTabButton[QSLOT_BUTTON_DOWN][i] = new CINFImage;
		m_pImgTabButton[QSLOT_BUTTON_DOWN][i]->InitDeviceObjects( pDataHeader->m_pData, pDataHeader->m_DataSize );
	}
#endif

	for(i=0; i<QSLOT_NUMBER; i++)
	{
		m_vecFontLine[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),8, D3DFONT_ZENABLE,  TRUE,256,32);
		m_vecFontLine[i]->InitDeviceObjects(g_pD3dDev);
	}

	m_pFontTabNum = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),8, D3DFONT_ZENABLE,  TRUE,256,32);
	m_pFontTabNum->InitDeviceObjects(g_pD3dDev);

	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
    m_pImgBlind = new CINFImageEx;
	pDataHeader = FindResource("LM_inven");
	m_pImgBlind->InitDeviceObjects(pDataHeader);
#endif
	return S_OK ;
}

HRESULT CINFGameMainQSlot::RestoreDeviceObjects()
{
	FLOG( "CINFGameMainQSlot::RestoreDeviceObjects()" );
	if(!m_bRestored)
	{
		m_nX = QSLOT_START_X;
		m_nY = QSLOT_START_Y;
		m_pBack->RestoreDeviceObjects() ;
		m_pNumber->RestoreDeviceObjects() ;
		m_pImgDisSkill->RestoreDeviceObjects() ;
		// �� ��ư ����
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
		m_pImgTabButton[QSLOT_BUTTON_UP]->RestoreDeviceObjects();
		m_pImgTabButton[QSLOT_BUTTON_UP]->SetBtnPosition(QSLOT_BUTTON_UP_START_X,QSLOT_BUTTON_UP_START_Y);
		m_pImgTabButton[QSLOT_BUTTON_DOWN]->RestoreDeviceObjects();
		m_pImgTabButton[QSLOT_BUTTON_DOWN]->SetBtnPosition(QSLOT_BUTTON_UP_START_X,QSLOT_BUTTON_UP_START_Y);
#else 
		for(int i=0;i<QSLOT_BUTTON_STATE_NUMBER;i++)
		{
			m_pImgTabButton[QSLOT_BUTTON_UP][i]->RestoreDeviceObjects();
			m_pImgTabButton[QSLOT_BUTTON_DOWN][i]->RestoreDeviceObjects();
		}
#endif
		m_bRestored = TRUE;
	}
	
	for(int i=0; i<QSLOT_NUMBER; i++)
	{
		m_vecFontLine[i]->RestoreDeviceObjects() ;
	}
	m_pFontTabNum->RestoreDeviceObjects() ;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pImgBlind->RestoreDeviceObjects();														  
#endif
	return S_OK ;
}

HRESULT CINFGameMainQSlot::DeleteDeviceObjects()
{
	FLOG( "CINFGameMainQSlot::DeleteDeviceObjects()" );
	int i;

	m_pBack->DeleteDeviceObjects() ;
	util::del(m_pBack );	
	m_pImgDisSkill->DeleteDeviceObjects() ;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	util::del(m_pImgDisSkill );
	m_pNumber->DeleteDeviceObjects() ;
	util::del(m_pNumber );
	m_pImgTabButton[QSLOT_BUTTON_UP]->InvalidateDeviceObjects();
	m_pImgTabButton[QSLOT_BUTTON_DOWN]->InvalidateDeviceObjects();
	util::del(m_pImgTabButton[QSLOT_BUTTON_UP]);
	util::del(m_pImgTabButton[QSLOT_BUTTON_DOWN]);  
#else
	for(i=0;i<QSLOT_BUTTON_STATE_NUMBER;i++)
	{
		m_pImgTabButton[QSLOT_BUTTON_UP][i]->InvalidateDeviceObjects();
		m_pImgTabButton[QSLOT_BUTTON_DOWN][i]->InvalidateDeviceObjects();
		util::del(m_pImgTabButton[QSLOT_BUTTON_UP][i]);
		util::del(m_pImgTabButton[QSLOT_BUTTON_DOWN][i]);
	}
#endif
	for(i=0; i<QSLOT_NUMBER; i++)
	{
		m_vecFontLine[i]->DeleteDeviceObjects() ;
		util::del(m_vecFontLine[i]);
	}
	m_pFontTabNum->DeleteDeviceObjects() ;
	util::del(m_pFontTabNum);

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pImgBlind->DeleteDeviceObjects();
	util::del(m_pImgBlind);
#endif
	return S_OK ;
}


HRESULT CINFGameMainQSlot::InvalidateDeviceObjects()
{
	FLOG( "CINFGameMainQSlot::InvalidateDeviceObjects()" );
	if(m_bRestored)
	{
		m_pBack->InvalidateDeviceObjects() ;
		m_pNumber->InvalidateDeviceObjects() ;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����

		m_pImgDisSkill->InvalidateDeviceObjects() ;
		m_pImgTabButton[QSLOT_BUTTON_UP]->InvalidateDeviceObjects();
		m_pImgTabButton[QSLOT_BUTTON_DOWN]->InvalidateDeviceObjects();
		m_pImgBlind->InvalidateDeviceObjects();	
#else										   
		m_pImgDisSkill->InvalidateDeviceObjects() ;
		for(int i=0;i<QSLOT_BUTTON_STATE_NUMBER;i++)
		{
			m_pImgTabButton[QSLOT_BUTTON_UP][i]->InvalidateDeviceObjects();
			m_pImgTabButton[QSLOT_BUTTON_DOWN][i]->InvalidateDeviceObjects();
		}
#endif

		m_bRestored = FALSE;
	}

	for(int i=0; i<QSLOT_NUMBER; i++)
	{
		m_vecFontLine[i]->InvalidateDeviceObjects();
	}
	m_pFontTabNum->InvalidateDeviceObjects();

	return S_OK ;
}


void CINFGameMainQSlot::Tick()
{
	FLOG( "CINFGameMainQSlot::Tick()" );
	if(g_pShuttleChild->IsOperAndObser())
	{
		return;
	}
	
	// 2007-07-09 by bhsohn ��ݰ� ���ڻ��� ���� ���� ������ ó��
	if(m_fClickBazaar > 0)
	{
		m_fClickBazaar -= g_pD3dApp->GetCheckElapsedTime(); 
	}
	// end 2007-07-09 by bhsohn ��ݰ� ���ڻ��� ���� ���� ������ ó��
	if(m_fQSlotTimer>0)
	{
		m_fQSlotTimer -= g_pD3dApp->GetElapsedTime();
	}
	if( g_pD3dApp->m_dwGameState == _CITY &&
		g_pInterface->m_pCityBase->GetCurrentBuildingNPC() != NULL )
	{
		return;
	}

	if( m_fQSlotTimer <= 0 &&
		g_pInterface->m_pCityBase &&
		g_pInterface->m_pCityBase->GetCurrentBuildingNPC() == NULL &&
		!((CINFGameMain*)m_pParent)->m_pTrade->m_bTrading && 
		!g_pD3dApp->m_bChatMode &&
		g_pShuttleChild->CheckUnitState() == FLIGHT)					// 2006-09-19 by ispark, �����߿���
	{
		// 2008-12-15 by bhsohn ��ü�� �׾�����, ����Ŭ������ ��ų �ߵ� ���� ����
		BOOL bBreak=FALSE;
		for(int i=0;i<QSLOT_NUMBER;i++)
		{
			// 2008-12-15 by bhsohn ��ü�� �׾�����, ����Ŭ������ ��ų �ߵ� ���� ����
			if(bBreak)
			{
				break;
			}
			// end 2008-12-15 by bhsohn ��ü�� �׾�����, ����Ŭ������ ��ų �ߵ� ���� ����
			// 2008-11-13 by bhsohn ���̽�ƽ �۾�
//			if( g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_1+i) )
//			{
//				UseQuickSlot(m_nCurrentTab, i);
//			}
			if( g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_1+i) )
			{
				if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_1+i))
				{
					// Ű���� ��ų
					UseQuickSlot(m_nCurrentTab, i);
				}
				else
				{
					// ���̽�ƽ���� �ɸ���Ȳ
					AddJoystikcQuickSlotList(m_nCurrentTab, i);
				}
			}
		}		
	}
	// 2008-11-13 by bhsohn ���̽�ƽ �۾�
	TickJoysticSlot();
	// end 2008-11-13 by bhsohn ���̽�ƽ �۾�

	// 2013-07-01 by bhsohn QuickSlot����� �ð� üũ�Ͽ� �ٷ� �����ϰ� ����
	TickCheckQuiclSlotSave();
	// END 2013-07-01 by bhsohn QuickSlot����� �ð� üũ�Ͽ� �ٷ� �����ϰ� ����
}

// 2013-07-01 by bhsohn QuickSlot����� �ð� üũ�Ͽ� �ٷ� �����ϰ� ����
void CINFGameMainQSlot::TickCheckQuiclSlotSave()
{	
	if(m_fCheckQuiclSlotSave <= 0.0f)
	{
		return;
	}
	if(g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING )
	{
		return;
	}
	m_fCheckQuiclSlotSave -= g_pD3dApp->GetCheckElapsedTime(); 

	if(m_fCheckQuiclSlotSave <= 0.0f && g_pD3dApp && g_pD3dApp->m_pInterface)
	{
		m_fCheckQuiclSlotSave = 0.0f;
		g_pD3dApp->m_pInterface->SaveCharacterFile(SLOT_PURPOSE_MAIN); 
	}
}
void CINFGameMainQSlot::SetCheckQuiclSlotSave(float fCheckQuiclSlotSave) 
{
	m_fCheckQuiclSlotSave = fCheckQuiclSlotSave;
}


// 2007-01-22 by bhsohn ��Ű ���� ���̽� ������
//#define QSLOT_BUTTON_UP_START_X			(m_nX + 260)
//#define QSLOT_BUTTON_UP_START_Y			(m_nY + 5)
//#define QSLOT_BUTTON_DOWN_START_X		(m_nX + 260)
//#define QSLOT_BUTTON_DOWN_START_Y		(m_nY + 18)
//#define QSLOT_BUTTON_SIZE_X				8
//#define QSLOT_BUTTON_SIZE_Y				8

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
#define QSLOT_BUTTON_UP_START_X			(m_nX + QSLOT_SIZE_X + 4)
#define QSLOT_BUTTON_UP_START_Y			(m_nY + 0)
#define QSLOT_BUTTON_DOWN_START_X		(m_nX + QSLOT_SIZE_X + 4)
#define QSLOT_BUTTON_DOWN_START_Y		(m_nY + 23)
#define QSLOT_BUTTON_SIZE_X				8
#define QSLOT_BUTTON_SIZE_Y				8
#endif
// end 2007-01-22 by bhsohn ��Ű ���� ���̽� ������

void CINFGameMainQSlot::Render()
{
	// 2007-03-27 by bhsohn ������ ���� �������̽� ����
	if(g_pShuttleChild->IsObserverMode())
	{
		return;
	}
 #ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	FLOG( "CINFGameMainQSlot::Render()" );
	char buf[64];

	if( m_pIsSlotOpen == FALSE )
	{
		for(int i=0; i < QSLOT_TAB_NUMBER; i++ )
		{
			m_pBack->Move(m_nX, m_nY +4 - i * (m_pBack->GetImgSize().y + 2 ) );
			m_pBack->Render();
		}
	}
	else
	{
		m_pBack->Move( m_nX, m_nY +4 );
		m_pBack->Render();
	}

	// ����Ű ������
	CINFIcon* pIconInfo = ((CINFGameMain*)m_pParent)->m_pIcon;
	
// 	for(int i=0;i<QSLOT_NUMBER;i++)
// 	{
// 		if(m_pQSlotInfo[m_nCurrentTab][i].pItem && IsValidQSlotInfo(m_nCurrentTab, i))
// 		{
// 			strcpy( buf, m_pQSlotInfo[m_nCurrentTab][i].IconName );
// 
// 			pIconInfo->SetIcon(buf, 
// 				m_nX + 8 + (pIconInfo->GetIconSize().x + 3) * i,
// 				m_nY+11, 1.0f);
// 			pIconInfo->Render();
// 			
// 			if(IS_SKILL_ITEM(m_pQSlotInfo[m_nCurrentTab][i].pItem->Kind) 
// 			 && FALSE == RenderDisableSkill(m_pQSlotInfo[m_nCurrentTab][i].pItem->ItemNum))
// 			{
// 				m_pImgDisSkill->Move(m_nX + 7 + (pIconInfo->GetIconSize().x + 3) * i, m_nY + 11);
// 				m_pImgDisSkill->Render();
// 			}
// 
// 			// 2005-11-22 by ispark
// 			// ��ų ��ߵ� �ð� ǥ��
// 			if(IS_SKILL_ITEM(m_pQSlotInfo[m_nCurrentTab][i].pItem->Kind))
// 			{
// 				RenderSkillReAttackTime(m_pQSlotInfo[m_nCurrentTab][i].pItem->ItemNum, i);
// 			}
// 			else if(ITEMKIND_CARD == m_pQSlotInfo[m_nCurrentTab][i].pItem->Kind
// 				&& COMPARE_BIT_FLAG(m_pQSlotInfo[m_nCurrentTab][i].pItem->ItemInfo->ItemAttribute, ITEM_ATTR_TIME_LIMITE)
// 				&& 0 < m_pQSlotInfo[m_nCurrentTab][i].pItem->ItemInfo->ReAttacktime)
// 			{
// 				RenderItemUsableReAttackTime(m_pQSlotInfo[m_nCurrentTab][i].pItem->ItemNum, i);
// 			}
// 			// 2008-11-26 by bhsohn ����ð� ���� ������ ����
// 			else if(ITEMKIND_CARD == m_pQSlotInfo[m_nCurrentTab][i].pItem->Kind
// 				&& COMPARE_BIT_FLAG(m_pQSlotInfo[m_nCurrentTab][i].pItem->ItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED)
// 				&& 0 < m_pQSlotInfo[m_nCurrentTab][i].pItem->ItemInfo->ReAttacktime)
// 			{
// 				RenderItemUsableReAttackTime(m_pQSlotInfo[m_nCurrentTab][i].pItem->ItemNum, i);
// 			}
// 			// end 2008-11-26 by bhsohn ����ð� ���� ������ ����
// 
// 			//���� ������.
// 			if( IS_COUNTABLE_ITEM(m_pQSlotInfo[m_nCurrentTab][i].pItem->Kind) )
// 			{
// 				CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( m_pQSlotInfo[m_nCurrentTab][i].pItem->UniqueNumber );
// 				if( pItemInfo->CurrentCount > 1 )
// 				{
// 					// ������ �����ش�.
// 					// 2006-09-08 by ispark, ��ġ ����
// 					wsprintf(buf, "%d",pItemInfo->CurrentCount);
// 					int len = strlen(buf) - 1;			// ����� �Ѱ� �̻� ���´ٴ� ���ǿ� -1�� �ߴ�.
// 					m_vecFontLine[i]->DrawText(m_nX+pIconInfo->GetIconSize().x*i+21 - len*6,m_nY-1,QSLOT_COUNTERBLE_NUMBER,buf, 0L);
// 				}
// 			}
// 			// 2010. 02. 11 by ckPark �ߵ��� ����������
// 			if(m_pQSlotInfo[m_nCurrentTab][i].pItem && IsValidQSlotInfo(m_nCurrentTab, i))
// 			{
// 				if( m_pQSlotInfo[m_nCurrentTab][i].pItem->ItemInfo->InvokingDestParamID
// 					|| m_pQSlotInfo[m_nCurrentTab][i].pItem->ItemInfo->InvokingDestParamIDByUse )
// 				{
// 					char buf[128];
// 					
// 					// �����Կ����� ������ �������� ��Ÿ�Ӹ� ǥ���Ѵ�
// 					CItemInfo* pItemInfo = g_pStoreData->FindItemInWearByItemNum(m_pQSlotInfo[m_nCurrentTab][i].pItem->ItemNum);
// 					if( pItemInfo && GetString_CoolTime( pItemInfo, buf ) )
// 					{
// 						int len = strlen(buf) - 1;
// 						
// 						int nFontPosX = m_nX + pIconInfo->GetIconSize().x * i + FONTLINE_X + 20 - len * 6; // ���⼭ 6�� ���� ���� �ؽ�Ʈ �����̴�.
// 						int nFontPosY = m_nY + FONTLINE_Y + 7;
// 						
// 						m_vecFontLine[i]->DrawText(nFontPosX,nFontPosY, QSLOT_COUNTERBLE_NUMBER,buf, 0L);
// 					}
// 				}
// 			}
// 			// end 2010. 02. 11 by ckPark �ߵ��� ����������
// 		}
// 	}
// �߰� ������
	int TabNum = 0;
	for(int j=0;j<QSLOT_TAB_NUMBER;j++)
	{
// 			if(j == m_nCurrentTab)
// 				continue;
		
		int nNum = ( m_nCurrentTab + TabNum )%QSLOT_TAB_NUMBER;
		for(int i=0;i<QSLOT_NUMBER;i++)
		{
			if(m_pQSlotInfo[nNum][i].pItem && IsValidQSlotInfo(nNum, i))
			{
				strcpy( buf, m_pQSlotInfo[nNum][i].IconName );	
				pIconInfo->SetIcon(buf, 
					m_nX + 8 + (pIconInfo->GetIconSize().x + 3) * i,
					m_nY +11 - TabNum * (pIconInfo->GetIconSize().y + 14
					), 1.0f);				
				pIconInfo->Render();

				if(IS_SKILL_ITEM(m_pQSlotInfo[nNum][i].pItem->Kind) 
					&& FALSE == RenderDisableSkill(m_pQSlotInfo[nNum][i].pItem->ItemNum))
				{
					m_pImgDisSkill->Move(m_nX + 7 + (pIconInfo->GetIconSize().x + 3) * i, m_nY +10 - TabNum * (pIconInfo->GetIconSize().y + 14) );
					m_pImgDisSkill->Render();
				}
				
				// 2005-11-22 by ispark
				// ��ų ��ߵ� �ð� ǥ��
				if(IS_SKILL_ITEM(m_pQSlotInfo[nNum][i].pItem->Kind))
				{
					RenderSkillReAttackTime(m_pQSlotInfo[nNum][i].pItem->ItemNum, i, TabNum, pIconInfo->GetIconSize().x, pIconInfo->GetIconSize().y );
				}
				else if(ITEMKIND_CARD == m_pQSlotInfo[nNum][i].pItem->Kind
					&& COMPARE_BIT_FLAG(m_pQSlotInfo[nNum][i].pItem->ItemInfo->ItemAttribute, ITEM_ATTR_TIME_LIMITE)
					&& 0 < m_pQSlotInfo[nNum][i].pItem->ItemInfo->ReAttacktime)
				{
					RenderItemUsableReAttackTime(m_pQSlotInfo[nNum][i].pItem->ItemNum, i, TabNum, pIconInfo->GetIconSize().x, pIconInfo->GetIconSize().y );
				}
				// 2008-11-26 by bhsohn ����ð� ���� ������ ����
				else if(ITEMKIND_CARD == m_pQSlotInfo[nNum][i].pItem->Kind
					&& COMPARE_BIT_FLAG(m_pQSlotInfo[nNum][i].pItem->ItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED)
					&& 0 < m_pQSlotInfo[nNum][i].pItem->ItemInfo->ReAttacktime)
				{
					RenderItemUsableReAttackTime(m_pQSlotInfo[nNum][i].pItem->ItemNum, i, TabNum, pIconInfo->GetIconSize().x, pIconInfo->GetIconSize().y );
				}
				// end 2008-11-26 by bhsohn ����ð� ���� ������ ����
				
				//���� ������.
				if( IS_COUNTABLE_ITEM(m_pQSlotInfo[nNum][i].pItem->Kind) )
				{
					CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( m_pQSlotInfo[nNum][i].pItem->UniqueNumber );
					if( pItemInfo->CurrentCount > 1 )
					{
						// ������ �����ش�.
						// 2006-09-08 by ispark, ��ġ ����
						wsprintf(buf, "%d",pItemInfo->CurrentCount);
						//int len = strlen(buf) - 1;			// ����� �Ѱ� �̻� ���´ٴ� ���ǿ� -1�� �ߴ�.
						SIZE nSize = m_vecFontLine[i]->GetStringSize( buf );
						m_vecFontLine[i]->DrawText(m_nX + 7 + ( pIconInfo->GetIconSize().x + 3) * i + ( pIconInfo->GetIconSize().x - nSize.cx ) , m_nY + 8 - TabNum * (pIconInfo->GetIconSize().y + 14),QSLOT_COUNTERBLE_NUMBER,buf, 0L);
					}
				}
				// 2010. 02. 11 by ckPark �ߵ��� ����������
				if(m_pQSlotInfo[nNum][i].pItem && IsValidQSlotInfo(nNum, i))
				{
					if( m_pQSlotInfo[nNum][i].pItem->ItemInfo->InvokingDestParamID
						|| m_pQSlotInfo[nNum][i].pItem->ItemInfo->InvokingDestParamIDByUse )
					{
						char buf[128];
						
						// �����Կ����� ������ �������� ��Ÿ�Ӹ� ǥ���Ѵ�
						CItemInfo* pItemInfo = g_pStoreData->FindItemInWearByItemNum(m_pQSlotInfo[nNum][i].pItem->ItemNum);
						if( pItemInfo && GetString_CoolTime( pItemInfo, buf ) )
						{
							SIZE len = m_vecFontLine[i]->GetStringSize(buf);
							
							int nFontPosX = m_nX + 7 + ( pIconInfo->GetIconSize().x + 3) * i + ( ( pIconInfo->GetIconSize().x - len.cx ) / 2 ) ; // ���⼭ 6�� ���� ���� �ؽ�Ʈ �����̴�.
							int nFontPosY = m_nY + 10 - TabNum * (pIconInfo->GetIconSize().y + 14) + ( ( pIconInfo->GetIconSize().y - len.cy ) /2 );
							
							m_vecFontLine[i]->DrawText(nFontPosX,nFontPosY, QSLOT_COUNTERBLE_NUMBER,buf, 0L);
						}
					}
				}
				if( TabNum != 0 )
				{
					m_pImgBlind->Move(m_nX + 8 + (pIconInfo->GetIconSize().x + 3) * i, m_nY +11 - TabNum * (pIconInfo->GetIconSize().y + 14) );
					m_pImgBlind->SetScale( pIconInfo->GetIconSize().x, pIconInfo->GetIconSize().y );
					m_pImgBlind->Render();
				}
				// end 2010. 02. 11 by ckPark �ߵ��� ����������
			}
		}
		if(m_pIsSlotOpen == TRUE)
			break;

		TabNum++;
	}

	


	// 2007-01-22 by bhsohn ��Ű ���� ���̽� ������
	wsprintf(buf, "%d",m_nCurrentTab+1);
	m_pFontTabNum->DrawText(QSLOT_BUTTON_UP_START_X + 1,
								QSLOT_BUTTON_UP_START_Y + QSLOT_BUTTON_SIZE_Y , 
								QSLOT_COUNTERBLE_NUMBER,buf, 0L);

	m_pNumber->Move(m_nX - 5, m_nY);
	m_pNumber->Render();

	// 2010. 02. 11 by ckPark �ߵ��� ����������
	// �� ��ư ����
	if( m_pIsSlotOpen == TRUE )
	{
		m_pImgTabButton[QSLOT_BUTTON_UP]->Render();
	}
	else
	{
		m_pImgTabButton[QSLOT_BUTTON_DOWN]->Render();
	}

	// ���õ� �������� �巡���� ���
	if(m_nSelectSlotNumber != -1 && 
		m_pQSlotInfo[m_nCurrentTab][m_nSelectSlotNumber].IconName[0])
	{
		POINT ptCursor;
		GetCursorPos( &ptCursor );
		ScreenToClient( g_pD3dApp->GetHwnd(), &ptCursor );
		CheckMouseReverse(&ptCursor);
		g_pGameMain->m_bQSlotIconFlag = TRUE;
		g_pGameMain->m_nQSlotPosX = ptCursor.x - m_nRenderMoveIconIntervalWidth;
		g_pGameMain->m_nQSlotPosY = ptCursor.y - m_nRenderMoveIconIntervalHeight;

//		pIconInfo->SetIcon(m_pQSlotInfo[m_nCurrentTab][m_nSelectSlotNumber].IconName, 
//			ptCursor.x - m_nRenderMoveIconIntervalWidth,
//			ptCursor.y - m_nRenderMoveIconIntervalHeight, 1.0f);
//		pIconInfo->Render();
	}
	else
	{
		g_pGameMain->m_bQSlotIconFlag = FALSE;
	}  
#else
	FLOG( "CINFGameMainQSlot::Render()" );
	char buf[64];
	m_pBack->Move(m_nX, m_nY);
	m_pBack->Render();

	// ����Ű ������
	CINFIcon* pIconInfo = ((CINFGameMain*)m_pParent)->m_pIcon;
	for(int i=0;i<QSLOT_NUMBER;i++)
	{
		if(m_pQSlotInfo[m_nCurrentTab][i].pItem && IsValidQSlotInfo(m_nCurrentTab, i))
		{
			// 2010. 04. 01 by ckPark ���ҽ� ���� �ý��۽� ���丮�� �����ҿ��� ������ ȸ���� �� ������ �ȵǴ� ���� �ذ�

// 			// 2005-11-28 by ispark, SourceIndex�� ����, ��ų�� SKILL_BASE_NUM���� �Ѵ�.
// 			if(IS_SKILL_ITEM(m_pQSlotInfo[m_nCurrentTab][i].pItem->Kind))
// 			{
// 				wsprintf(buf, "%08d", SKILL_BASE_NUM(m_pQSlotInfo[m_nCurrentTab][i].pItem->ItemInfo->SourceIndex));
// 			}
// 			else
// 			{
// 				wsprintf(buf, "%08d", m_pQSlotInfo[m_nCurrentTab][i].pItem->ItemInfo->SourceIndex);
// 			}

			strcpy( buf, m_pQSlotInfo[m_nCurrentTab][i].IconName );

			// end 2010. 04. 01 by ckPark ���ҽ� ���� �ý��۽� ���丮�� �����ҿ��� ������ ȸ���� �� ������ �ȵǴ� ���� �ذ�

			pIconInfo->SetIcon(buf, 
				m_nX+QSLOT_ICON_INTERVAL*i+1,
				m_nY+1, 1.0f);
			pIconInfo->Render();
			
			if(IS_SKILL_ITEM(m_pQSlotInfo[m_nCurrentTab][i].pItem->Kind) 
			 && FALSE == RenderDisableSkill(m_pQSlotInfo[m_nCurrentTab][i].pItem->ItemNum))
			{
				m_pImgDisSkill->Move(m_nX+QSLOT_ICON_INTERVAL*i, m_nY);
				m_pImgDisSkill->Render();
			}

			// 2005-11-22 by ispark
			// ��ų ��ߵ� �ð� ǥ��
			if(IS_SKILL_ITEM(m_pQSlotInfo[m_nCurrentTab][i].pItem->Kind))
			{
				RenderSkillReAttackTime(m_pQSlotInfo[m_nCurrentTab][i].pItem->ItemNum, i);
			}
			else if(ITEMKIND_CARD == m_pQSlotInfo[m_nCurrentTab][i].pItem->Kind
				&& COMPARE_BIT_FLAG(m_pQSlotInfo[m_nCurrentTab][i].pItem->ItemInfo->ItemAttribute, ITEM_ATTR_TIME_LIMITE)
				&& 0 < m_pQSlotInfo[m_nCurrentTab][i].pItem->ItemInfo->ReAttacktime)
			{
				RenderItemUsableReAttackTime(m_pQSlotInfo[m_nCurrentTab][i].pItem->ItemNum, i);
			}
			// 2008-11-26 by bhsohn ����ð� ���� ������ ����
			else if(ITEMKIND_CARD == m_pQSlotInfo[m_nCurrentTab][i].pItem->Kind
				&& COMPARE_BIT_FLAG(m_pQSlotInfo[m_nCurrentTab][i].pItem->ItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED)
				&& 0 < m_pQSlotInfo[m_nCurrentTab][i].pItem->ItemInfo->ReAttacktime)
			{
				RenderItemUsableReAttackTime(m_pQSlotInfo[m_nCurrentTab][i].pItem->ItemNum, i);
			}
			// end 2008-11-26 by bhsohn ����ð� ���� ������ ����

			//���� ������.
			if( IS_COUNTABLE_ITEM(m_pQSlotInfo[m_nCurrentTab][i].pItem->Kind) )
			{
				CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( m_pQSlotInfo[m_nCurrentTab][i].pItem->UniqueNumber );
				if( pItemInfo->CurrentCount > 1 )
				{
					// ������ �����ش�.
					// 2006-09-08 by ispark, ��ġ ����
					wsprintf(buf, "%d",pItemInfo->CurrentCount);
					int len = strlen(buf) - 1;			// ����� �Ѱ� �̻� ���´ٴ� ���ǿ� -1�� �ߴ�.
					m_vecFontLine[i]->DrawText(m_nX+QSLOT_ICON_INTERVAL*i+21 - len*6,m_nY-1,QSLOT_COUNTERBLE_NUMBER,buf, 0L);
				}
			}
		}
	}

	// 2007-01-22 by bhsohn ��Ű ���� ���̽� ������
	wsprintf(buf, "%d",m_nCurrentTab+1);
	m_pFontTabNum->DrawText(QSLOT_BUTTON_UP_START_X + 1,
								QSLOT_BUTTON_UP_START_Y + QSLOT_BUTTON_SIZE_Y , 
								QSLOT_COUNTERBLE_NUMBER,buf, 0L);

	m_pNumber->Move(m_nX, m_nY);
	m_pNumber->Render();

	// 2010. 02. 11 by ckPark �ߵ��� ����������
	for( i=0; i<QSLOT_NUMBER; ++i )
	{
		if(m_pQSlotInfo[m_nCurrentTab][i].pItem && IsValidQSlotInfo(m_nCurrentTab, i))
		{
			if( m_pQSlotInfo[m_nCurrentTab][i].pItem->ItemInfo->InvokingDestParamID
				|| m_pQSlotInfo[m_nCurrentTab][i].pItem->ItemInfo->InvokingDestParamIDByUse )
			{
				char buf[128];

				// �����Կ����� ������ �������� ��Ÿ�Ӹ� ǥ���Ѵ�
				CItemInfo* pItemInfo = g_pStoreData->FindItemInWearByItemNum(m_pQSlotInfo[m_nCurrentTab][i].pItem->ItemNum);
				if( pItemInfo && GetString_CoolTime( pItemInfo, buf ) )
				{
					int len = strlen(buf) - 1;

					int nFontPosX = m_nX + QSLOT_ICON_INTERVAL * i + FONTLINE_X + 20 - len * 6; // ���⼭ 6�� ���� ���� �ؽ�Ʈ �����̴�.
					int nFontPosY = m_nY + FONTLINE_Y + 7;

					m_vecFontLine[i]->DrawText(nFontPosX,nFontPosY, QSLOT_COUNTERBLE_NUMBER,buf, 0L);
				}
			}
		}
	}
	// end 2010. 02. 11 by ckPark �ߵ��� ����������


	// �� ��ư ����
	m_pImgTabButton[QSLOT_BUTTON_UP][m_nButtonState[QSLOT_BUTTON_UP]]->Move(QSLOT_BUTTON_UP_START_X,QSLOT_BUTTON_UP_START_Y);
	m_pImgTabButton[QSLOT_BUTTON_UP][m_nButtonState[QSLOT_BUTTON_UP]]->Render();
	m_pImgTabButton[QSLOT_BUTTON_DOWN][m_nButtonState[QSLOT_BUTTON_DOWN]]->Move(QSLOT_BUTTON_DOWN_START_X,QSLOT_BUTTON_DOWN_START_Y);
	m_pImgTabButton[QSLOT_BUTTON_DOWN][m_nButtonState[QSLOT_BUTTON_DOWN]]->Render();

	// ���õ� �������� �巡���� ���
	if(m_nSelectSlotNumber != -1 && 
		m_pQSlotInfo[m_nCurrentTab][m_nSelectSlotNumber].IconName[0])
	{
		POINT ptCursor;
		GetCursorPos( &ptCursor );
		ScreenToClient( g_pD3dApp->GetHwnd(), &ptCursor );
		CheckMouseReverse(&ptCursor);
		g_pGameMain->m_bQSlotIconFlag = TRUE;
		g_pGameMain->m_nQSlotPosX = ptCursor.x - m_nRenderMoveIconIntervalWidth;
		g_pGameMain->m_nQSlotPosY = ptCursor.y - m_nRenderMoveIconIntervalHeight;

//		pIconInfo->SetIcon(m_pQSlotInfo[m_nCurrentTab][m_nSelectSlotNumber].IconName, 
//			ptCursor.x - m_nRenderMoveIconIntervalWidth,
//			ptCursor.y - m_nRenderMoveIconIntervalHeight, 1.0f);
//		pIconInfo->Render();
	}
	else
	{
		g_pGameMain->m_bQSlotIconFlag = FALSE;
	}
#endif
}

void CINFGameMainQSlot::SetToolTip(int x, int y, ITEM_BASE* pItem)
{
	if(pItem)
	{
		char buf[256];
		memset(buf, 0x00, sizeof(buf));
//		ITEM_GENERAL* pInvenItem = g_pStoreData->FindItemInInventoryByUniqueNumber(pItem->UniqueNumber);
//		ITEM* pItemInfo = g_pStoreData->GetItemInfoFromItemGeneral(pInvenItem);
		if(IS_SKILL_ITEM(pItem->Kind) == FALSE)
		{
			CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber(pItem->UniqueNumber);
			if(pItemInfo)
			{
				ITEM* pITEM = pItemInfo->GetItemInfo();
				if(pITEM)
				{
					if(IS_COUNTABLE_ITEM(pITEM->Kind) && pItemInfo)
					{
						wsprintf(buf, STRMSG_C_TOOLTIP_0013,pITEM->ItemName, pItemInfo->CurrentCount);//"%s (%d ��)"
					}
					else
					{
						wsprintf(buf, "%s",pITEM->ItemName);
					}
					int nLength = (strlen(buf)*6.5)+7;
					((CINFGameMain*)m_pParent)->SetToolTip(x-nLength, y,buf);
				}
			}
		}
		else
		{
			// SKILL
			ITEM* pSKILL = g_pShuttleChild->m_pSkill->FindItem(pItem->ItemNum);
			if(pSKILL)
			{
				int nLength = (strlen(pSKILL->ItemName)*6.5)+7;
				((CINFGameMain*)m_pParent)->SetToolTip(x-nLength, y,pSKILL->ItemName);
			}
		}
	}
	else
	{
		((CINFGameMain*)m_pParent)->SetToolTip(0, 0,NULL);
	}
}

int CINFGameMainQSlot::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFGameMainQSlot::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			// 2007-03-27 by bhsohn ������ ���� �������̽� ����
			if(g_pShuttleChild->IsObserverMode())
			{
				return INF_MSGPROC_NORMAL;
			}
			// end 2007-03-27 by bhsohn ������ ���� �������̽� ����

			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if( m_pImgTabButton[QSLOT_BUTTON_DOWN]->OnMouseMove(pt) )
			{
				//return  INF_MSGPROC_BREAK;
			}

			if( m_pImgTabButton[QSLOT_BUTTON_UP]->OnMouseMove(pt) )
			{
				//return  INF_MSGPROC_BREAK;
			}
			
			// 2006-07-27 by ispark
			if(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem &&
				((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType == ITEM_QSLOT_POS)
			{
				m_bLButtonDown = TRUE;
			}
			// �� �� ���� �ֱ� 
		
			int TabNum = 0;
			CINFIcon* pIconInfo = ((CINFGameMain*)m_pParent)->m_pIcon;
			{
				for(int j=0;j<QSLOT_TAB_NUMBER;j++)
				{
					int nNum = ( m_nCurrentTab + TabNum )%QSLOT_TAB_NUMBER;
					for(int i=0;i< QSLOT_NUMBER;i++)
					{
						int nTempX = m_nX + 8 + ( pIconInfo->GetIconSize().x + 3) * i;
						int nTempY = m_nY + 11 - j * ( pIconInfo->GetIconSize().y + 14 );
						if( pt.y > nTempY &&
							pt.y < nTempY + pIconInfo->GetIconSize().y &&
							pt.x > nTempX &&
							pt.x < nTempX + pIconInfo->GetIconSize().x)
						{
							SetToolTip( pt.x - 10, pt.y + 13, m_pQSlotInfo[nNum][i].pItem );
							return INF_MSGPROC_BREAK;
						}
						else
						{
							((CINFGameMain*)m_pParent)->SetToolTip(0, 0,NULL);
						}					
					}
					if(m_pIsSlotOpen == TRUE)
						break;
					TabNum++;
				}
			}
// 			if( pt.y > m_nY &&
// 				pt.y < m_nY+QSLOT_SIZE_Y &&
// 				pt.x > m_nX &&
// 				pt.x < m_nX+QSLOT_SIZE_X)
// 			{
// 				int i = (pt.x - m_nX - 1)/QSLOT_ICON_INTERVAL;
// 
// 				if( m_pQSlotInfo[m_nCurrentTab][i].pItem && 
// 					i >= 0 && 
// 					i < QSLOT_NUMBER )
// 				{					
// //					g_pGameMain->SetItemInfo(m_pQSlotInfo[m_nCurrentTab][i].pItem->UniqueNumber, 
// //						m_pQSlotInfo[m_nCurrentTab][i].pItem->ItemNum, pt.x, pt.y);
// 					SetToolTip(pt.x-10, pt.y+13, m_pQSlotInfo[m_nCurrentTab][i].pItem);
// 					return INF_MSGPROC_BREAK;
// 				}
// 				else
// 				{
// 					((CINFGameMain*)m_pParent)->SetToolTip(0, 0,NULL);
// 				}
// 			}
// 			else
// 			{
// 				// 2007-01-22 by dgwoo ����â�������� ������ �Ѵ� ������ �ּ�ó���� �������̽��� 
// 				// ������ QSlot�� ������ �׳� ���� ���װ� �����.
// 				((CINFGameMain*)m_pParent)->SetToolTip(0, 0,NULL);
// 			}
			// current tab �̵� ��ư
// 			if(GetButtonStateOnMouse(pt, QSLOT_BUTTON_UP_START_X, QSLOT_BUTTON_UP_START_Y, QSLOT_BUTTON_SIZE_X, QSLOT_BUTTON_SIZE_Y))
// 			{
// 				m_nButtonState[QSLOT_BUTTON_UP] = BUTTON_STATE_UP;
// 				m_nButtonState[QSLOT_BUTTON_DOWN] = BUTTON_STATE_NORMAL;
// 			}
// 			else if(GetButtonStateOnMouse(pt, QSLOT_BUTTON_DOWN_START_X,QSLOT_BUTTON_DOWN_START_Y, QSLOT_BUTTON_SIZE_X, QSLOT_BUTTON_SIZE_Y))
// 			{
// 				m_nButtonState[QSLOT_BUTTON_UP] = BUTTON_STATE_NORMAL;
// 				m_nButtonState[QSLOT_BUTTON_DOWN] = BUTTON_STATE_UP;
// 			}
// 			else
// 			{
// 				m_nButtonState[QSLOT_BUTTON_UP] = BUTTON_STATE_NORMAL;
// 				m_nButtonState[QSLOT_BUTTON_DOWN] = BUTTON_STATE_NORMAL;
// 			}
		}
		break;
	case WM_RBUTTONDOWN:
		{
			// 2007-03-27 by bhsohn ������ ���� �������̽� ����
			if(g_pShuttleChild->IsObserverMode())
			{
				return INF_MSGPROC_NORMAL;
			}
			// end 2007-03-27 by bhsohn ������ ���� �������̽� ����

			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
// 			if( pt.y>m_nY &&
// 				pt.y<m_nY+QSLOT_SIZE_Y &&
// 				pt.x > m_nX &&
// 				pt.x < m_nX+QSLOT_SIZE_X)
// 			{
// 				// 2005-07-19 by ispark
// 				// I, V��ư ������ �������� ������ ���� ������ ���´�.
// 				// ���� �������� 0�̸� â�� ���� ���°� �ƴϴ�.
// 				if(!g_pGameMain->m_nLeftWindowInfo && !g_pGameMain->m_nRightWindowInfo)
// 				{
// 					break;
// 				}
// 
// 				int i = (pt.x - m_nX - 1)/QSLOT_ICON_INTERVAL;
// 				if( m_pQSlotInfo[m_nCurrentTab][i].pItem &&
// 					i >= 0 && 
// 					i < QSLOT_NUMBER ) 
// 				{
// 					m_pQSlotInfo[m_nCurrentTab][i].pItem = NULL;								
// 				}
// 			}
// 		}
			int TabNum = 0;
			CINFIcon* pIconInfo = ((CINFGameMain*)m_pParent)->m_pIcon;
			{
				for(int j=0;j<QSLOT_TAB_NUMBER;j++)
				{
					int nNum = ( m_nCurrentTab + TabNum )%QSLOT_TAB_NUMBER;
					for(int i=0;i< QSLOT_NUMBER;i++)
					{
						if(!g_pGameMain->m_nLeftWindowInfo && !g_pGameMain->m_nRightWindowInfo)
						{
							break;
						}		
						int nTempX = m_nX + 8 + ( pIconInfo->GetIconSize().x + 3) * i;
						int nTempY = m_nY + 11 - j * ( pIconInfo->GetIconSize().y + 14 );
						if( pt.y > nTempY &&
							pt.y < nTempY + pIconInfo->GetIconSize().y &&
							pt.x > nTempX &&
							pt.x < nTempX + pIconInfo->GetIconSize().x)
						{
							m_pQSlotInfo[nNum][i].pItem = NULL;	
							return INF_MSGPROC_BREAK;
						}
					}
					if(m_pIsSlotOpen == TRUE)
						break;
					TabNum++;
				}
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			// 2007-03-27 by bhsohn ������ ���� �������̽� ����
			if(g_pShuttleChild->IsObserverMode())
			{
				return INF_MSGPROC_NORMAL;
			}
			// end 2007-03-27 by bhsohn ������ ���� �������̽� ����

			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			if( m_pImgTabButton[QSLOT_BUTTON_DOWN]->OnLButtonDown(pt)  )
			{
			}	
			
			if( m_pImgTabButton[QSLOT_BUTTON_UP]->OnLButtonDown(pt) )
			{
			}
			
			int TabNum = 0;
			CINFIcon* pIconInfo = ((CINFGameMain*)m_pParent)->m_pIcon;
			for(int j=0;j<QSLOT_TAB_NUMBER;j++)
			{
				int nNum = ( m_nCurrentTab + TabNum )%QSLOT_TAB_NUMBER;
				for(int i=0;i< QSLOT_NUMBER;i++)
				{
					if(!g_pGameMain->m_nLeftWindowInfo && !g_pGameMain->m_nRightWindowInfo)
					{
						break;
					}
					int nTempX = m_nX + 8 + ( pIconInfo->GetIconSize().x + 3) * i;
					int nTempY = m_nY + 11 - j * ( pIconInfo->GetIconSize().y + 14 );
					if( pt.y > nTempY &&
						pt.y < nTempY + pIconInfo->GetIconSize().y &&
						pt.x > nTempX &&
						pt.x < nTempX + pIconInfo->GetIconSize().x)
					{
						if(NULL != m_pQSlotInfo[nNum][i].pItem &&
							IS_GENERAL_ITEM(m_pQSlotInfo[nNum][i].pItem->Kind) &&
							g_pInterface->m_pBazaarShop == NULL)
						{
							if(IS_COUNTABLE_ITEM(m_pQSlotInfo[nNum][i].pItem->Kind))
							{
								ITEM *pITEM = g_pDatabase->GetServerItemInfo(m_pQSlotInfo[nNum][i].pItem->ItemNum);
								if(pITEM)
								{
									CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber(((ITEM_GENERAL*)m_pQSlotInfo[nNum][i].pItem)->UniqueNumber);
									if(pItemInfo == NULL)
									{
										SetQSlotInfo(nNum,i,NULL);
										m_bLButtonDown = FALSE;
										return INF_MSGPROC_BREAK;
									}
								}
							}
						}					
					}
				// 2006-07-27 by ispark
					if(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem &&
						((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType == ITEM_QSLOT_POS)
					{
						m_bLButtonDown = TRUE;
						return INF_MSGPROC_BREAK;
					}

  					m_nItemType	= QSLOT_ITEMTYPE_NONE;
					
					if( pt.y > nTempY &&
						pt.y < nTempY + pIconInfo->GetIconSize().y &&
						pt.x > nTempX &&
						pt.x < nTempX + pIconInfo->GetIconSize().x)
					{
  						if( m_pQSlotInfo[nNum][i].pItem &&
  							i >= 0 && 
  							i < QSLOT_NUMBER ) 
  						{
  							if(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem == NULL)
  							{
								m_nRenderMoveIconIntervalWidth  = pt.x - nTempX; 
								m_nRenderMoveIconIntervalHeight = pt.y - nTempY;
  								if(IS_SKILL_ITEM(m_pQSlotInfo[nNum][i].pItem->Kind))
 								{
		 //							g_pGameMain->m_pCharacterInfo->m_pSelectSkill = (ITEM_SKILL*)m_pQSlotInfo[m_nCurrentTab][i].pItem;
 									SetSelectItem(&m_pQSlotInfo[nNum][i]);
 									m_nItemType				= QSLOT_ITEMTYPE_SKILL;
 								}
 								else
 								{
		 //							g_pGameMain->m_pInven->m_pSelectItem = (CItemInfo*)m_pQSlotInfo[m_nCurrentTab][i].pItem;
 									SetSelectItem(&m_pQSlotInfo[nNum][i]);
 									m_nItemType				= QSLOT_ITEMTYPE_ITEM;
 								}						
								m_bQSlotSwapFlag		= TRUE;
								m_nQSlotSwapTab			= nNum;
								m_nQSlotSwapNum			= i;

								m_pQSlotMove = m_pQSlotInfo[nNum][i].pItem;
								m_pQSlotInfo[nNum][i].pItem = NULL;
 							}
 							else
 							{
 								m_bQSlotSwapFlag		= FALSE;
 							}
 							
 							return INF_MSGPROC_BREAK;
  						}
 						else
  						{
  							if(m_bLButtonDown && 
  								((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType == ITEM_QSLOT_POS)
  							{
  								m_nSelectSlotNumber = -1;
  								m_nItemType = QSLOT_ITEMTYPE_NONE;
  								m_bLButtonDown = FALSE;
  								SetSelectItem(NULL);
  								return INF_MSGPROC_BREAK;
  							}
  						}
					}
				}
				if(m_pIsSlotOpen == TRUE)
					break;
				TabNum++;
			}
 		}
		break;
	case WM_LBUTTONUP:
		{
			// 2007-03-27 by bhsohn ������ ���� �������̽� ����
			if(g_pShuttleChild->IsObserverMode())
			{
				return INF_MSGPROC_NORMAL;
			}
			// end 2007-03-27 by bhsohn ������ ���� �������̽� ����
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if(g_pGameMain->m_pInven->m_bSelectWearItem) break;

			if(m_pIsSlotOpen == TRUE && m_pImgTabButton[QSLOT_BUTTON_DOWN]->OnLButtonUp(pt)  )
			{
				m_pIsSlotOpen = FALSE;
			}
			else if(m_pIsSlotOpen == FALSE && m_pImgTabButton[QSLOT_BUTTON_UP]->OnLButtonUp(pt) )
			{
				m_pIsSlotOpen = TRUE;
			}

			// 2006-07-27 by ispark
			ITEM_BASE* pSelectItem = NULL;
			if(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem)
			{
				pSelectItem = (ITEM_BASE*)(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem->pItem); 
			}
//			ITEM_BASE* pItem = NULL;
//			if(m_pSelectItem == NULL)
//			{
//				pItem = (ITEM_BASE*)((CINFGameMain*)m_pParent)->m_pInven->m_pSelectItem;
//				if(!pItem)
//				{
//					pItem = (ITEM_BASE*)((CINFGameMain*)m_pParent)->m_pCharacterInfo->m_pSelectSkill;
//				}
//			}
			int TabNum = 0;
			CINFIcon* pIconInfo = ((CINFGameMain*)m_pParent)->m_pIcon;
			int j;
			for(j=0;j<QSLOT_TAB_NUMBER;j++)
			{
				int nNum = ( m_nCurrentTab + TabNum )%QSLOT_TAB_NUMBER;
				for(int i=0;i< QSLOT_NUMBER;i++)
				{	
					if( pSelectItem && 
	    			  (m_bLButtonDown || ((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType != ITEM_QSLOT_POS))
					{
						int nTempX = m_nX + 8 + ( pIconInfo->GetIconSize().x + 3) * i;
						int nTempY = m_nY + 11 - j * ( pIconInfo->GetIconSize().y + 14 );
						if( pt.y > nTempY &&
							pt.y < nTempY + pIconInfo->GetIconSize().y &&
							pt.x > nTempX &&
							pt.x < nTempX + pIconInfo->GetIconSize().x)
						{
							SetQSlotInfo(nNum, i, pSelectItem);
							m_nSelectSlotNumber = -1;
							m_nItemType = QSLOT_ITEMTYPE_NONE;
							m_bLButtonDown = FALSE;
							SetSelectItem(NULL);
						}
					}
				}
				if(m_pIsSlotOpen == TRUE)
					break;
				TabNum++;
			}

			for(j=0;j<QSLOT_TAB_NUMBER;j++)
			{
				int nNum = ( m_nCurrentTab + TabNum )%QSLOT_TAB_NUMBER;
				for(int i=0;i< QSLOT_NUMBER;i++)
				{	
					if( m_nSelectSlotNumber>=0 )
					{
						int nTempX = m_nX + 8 + ( pIconInfo->GetIconSize().x + 3) * i;
						int nTempY = m_nY + 11 - j * ( pIconInfo->GetIconSize().y + 14 );
						if( pt.y > nTempY &&
							pt.y < nTempY + pIconInfo->GetIconSize().y &&
							pt.x > nTempX &&
							pt.x < nTempX + pIconInfo->GetIconSize().x)
						{
							m_nItemType = QSLOT_ITEMTYPE_NONE;
							SetQSlotInfo(m_nCurrentTab,m_nSelectSlotNumber,NULL);
							m_bLButtonDown = FALSE;
							SetSelectItem(NULL);
						}
					}
				}
				if(m_pIsSlotOpen == TRUE)
					break;
				TabNum++;
			}
			m_nSelectSlotNumber = -1;
//			if( (m_pSelectItem || pItem) && 
//  			if( (pSelectItem && 
//  				(m_bLButtonDown || ((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType != ITEM_QSLOT_POS)) &&
//  				pt.y>m_nY &&
//  				pt.y<m_nY+QSLOT_SIZE_Y)
//  			{
//  				int i = (pt.x - m_nX - 1)/QSLOT_ICON_INTERVAL;
//  				if( i >= 0 && 
//  					i < QSLOT_NUMBER ) 
//  				{
 //					if(m_pSelectItem)
 //						SetQSlotInfo(m_nCurrentTab,i, m_pSelectItem);
 //					else
 //					{
 //						SetQSlotInfo(m_nCurrentTab,i, pItem);
 //					}
 //					SetQSlotInfo(m_nCurrentTab,i, pSelectItem);
 					
// 					m_nSelectSlotNumber = -1;
// 					m_nItemType = QSLOT_ITEMTYPE_NONE;
// 					m_bLButtonDown = FALSE;
// //					g_pGameMain->m_pInven->m_pSelectItem = NULL;
// //					g_pGameMain->m_pInven->m_pSelectIcon = NULL;
// //					g_pGameMain->m_pCharacterInfo->m_pSelectSkill = NULL;
// //					g_pGameMain->m_pCharacterInfo->m_pSelectIcon = NULL;
// 					SetSelectItem(NULL);
// 					break;
// 				}
// 			}
// 			
// 			// Ŭ���� �����Ǵºκ� ��
// 			if( m_nSelectSlotNumber>=0 && 
// 				(pt.x < m_nX || 
// 				pt.x > m_nX+QSLOT_SIZE_X ||
// 				pt.y < m_nY || 
// 				pt.y > m_nY+QSLOT_SIZE_Y))
// 			{
// 				m_nItemType = QSLOT_ITEMTYPE_NONE;
// 				SetQSlotInfo(m_nCurrentTab,m_nSelectSlotNumber,NULL);
// 				m_bLButtonDown = FALSE;
// //				g_pGameMain->m_pInven->m_pSelectItem = NULL;
// //				g_pGameMain->m_pCharacterInfo->m_pSelectSkill = NULL;
// 				SetSelectItem(NULL);
// 			}
// 			m_nSelectSlotNumber = -1;
// 			
// 			// current tab �̵� ��ư
// 			if(GetButtonStateOnMouse(pt, QSLOT_BUTTON_UP_START_X, QSLOT_BUTTON_UP_START_Y, QSLOT_BUTTON_SIZE_X, QSLOT_BUTTON_SIZE_Y))
// 			{
// 				if(m_nButtonState[QSLOT_BUTTON_UP] == BUTTON_STATE_DOWN)
// 				{
// 					if(m_nCurrentTab > 0)
// 						m_nCurrentTab --;
// 				}
// 				m_nButtonState[QSLOT_BUTTON_UP] = BUTTON_STATE_UP;
// 				m_nButtonState[QSLOT_BUTTON_DOWN] = BUTTON_STATE_NORMAL;
// 			}
// 			else if(GetButtonStateOnMouse(pt, QSLOT_BUTTON_DOWN_START_X,QSLOT_BUTTON_DOWN_START_Y, QSLOT_BUTTON_SIZE_X, QSLOT_BUTTON_SIZE_Y))
// 			{
// 				if(m_nButtonState[QSLOT_BUTTON_DOWN] == BUTTON_STATE_DOWN)
// 				{
// //					if(m_nCurrentTab<QSLOT_TAB_NUMBER-2)
// //						m_nCurrentTab ++;
// 					if(m_nCurrentTab<REAL_TAB_NUMBER-1)
// 						m_nCurrentTab ++;
// 				}
// 				m_nButtonState[QSLOT_BUTTON_UP] = BUTTON_STATE_NORMAL;
// 				m_nButtonState[QSLOT_BUTTON_DOWN] = BUTTON_STATE_UP;
// 			}
// 			else
// 			{
// 				m_nButtonState[QSLOT_BUTTON_UP] = BUTTON_STATE_NORMAL;
// 				m_nButtonState[QSLOT_BUTTON_DOWN] = BUTTON_STATE_NORMAL;
// 			}

			// ������ ���� ��������� ����
			//g_pGameMain->m_pCharacterInfo->m_pSelectSkill = NULL;
			//g_pGameMain->m_pInven->m_pSelectItem = NULL;

		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			// 2007-03-27 by bhsohn ������ ���� �������̽� ����
			if(g_pShuttleChild->IsObserverMode())
			{
				return INF_MSGPROC_NORMAL;
			}
			// end 2007-03-27 by bhsohn ������ ���� �������̽� ����
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			// 2006-03-31 by ispark, ���� ����
// 			if( pt.x > m_nX && 
// 				pt.x < m_nX + QSLOT_SIZE_X &&
// 				pt.y>m_nY &&
// 				pt.y<m_nY+QSLOT_SIZE_Y)
// 			{
// 				int i = (pt.x - m_nX - 1)/QSLOT_ICON_INTERVAL;
// 				if( m_pQSlotInfo[m_nCurrentTab][i].pItem && 
// 					i >= 0 && 
// 					i < QSLOT_NUMBER )
// //					pt.x<m_nX+QSLOT_ICON_START_X+QSLOT_ICON_INTERVAL*i+QSLOT_ICON_SIZE)
// 				{
// 					UseQuickSlot(m_nCurrentTab, i);
// 				}
// 			}
			int TabNum = 0;
			CINFIcon* pIconInfo = ((CINFGameMain*)m_pParent)->m_pIcon;
// 			for(int j=0;j<QSLOT_TAB_NUMBER;j++)
// 			{
				int nNum = ( m_nCurrentTab + TabNum )%QSLOT_TAB_NUMBER;
				for(int i=0;i< QSLOT_NUMBER;i++)
				{
					int nTempX = m_nX + 8 + ( pIconInfo->GetIconSize().x + 3) * i;
					int nTempY = m_nY + 11 - 0 * ( pIconInfo->GetIconSize().y + 14 );
					if( pt.y > nTempY &&
						pt.y < nTempY + pIconInfo->GetIconSize().y &&
						pt.x > nTempX &&
						pt.x < nTempX + pIconInfo->GetIconSize().x)
					{
						UseQuickSlot(nNum, i);
					}
				}
// 				TabNum++;
// 			}
		}
		break;
	case WM_KEYDOWN:
		{
			// 2007-03-27 by bhsohn ������ ���� �������̽� ����
			if(g_pShuttleChild->IsObserverMode())
			{
				return INF_MSGPROC_NORMAL;
			}
			// end 2007-03-27 by bhsohn ������ ���� �������̽� ����

			if(wParam == VK_TAB)
			{
				if(m_nCurrentTab<REAL_TAB_NUMBER-1)
				{//���� ����.
					m_nCurrentTab ++;
					m_nSelectSlotNumber = -1;
//					g_pGameMain->m_pInven->m_pSelectItem = NULL;
//					g_pGameMain->m_pCharacterInfo->m_pSelectSkill = NULL;
//					g_pGameMain->m_pInven->m_pSelectIcon = NULL;
//					g_pGameMain->m_pCharacterInfo->m_pSelectIcon = NULL;
					SetSelectItem(NULL);
				}
				else
				{//���� �ʱ�ȭ.
					m_nCurrentTab = 0;
					m_nSelectSlotNumber = -1;
//					g_pGameMain->m_pInven->m_pSelectItem = NULL;
//					g_pGameMain->m_pCharacterInfo->m_pSelectSkill = NULL;
//					g_pGameMain->m_pInven->m_pSelectIcon = NULL;
//					g_pGameMain->m_pCharacterInfo->m_pSelectIcon = NULL;
					SetSelectItem(NULL);
				}
				m_nButtonState[QSLOT_BUTTON_DOWN] = BUTTON_STATE_DOWN;
			}
		}
		break;
	case WM_KEYUP:
		{
			// 2007-03-27 by bhsohn ������ ���� �������̽� ����
			if(g_pShuttleChild->IsObserverMode())
			{
				return INF_MSGPROC_NORMAL;
			}
			// end 2007-03-27 by bhsohn ������ ���� �������̽� ����
			
			if(wParam == VK_TAB)
			{
				m_nButtonState[QSLOT_BUTTON_DOWN] = BUTTON_STATE_NORMAL;
			}
		}
		break;
	}
	return INF_MSGPROC_NORMAL;	   
#else
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			// 2007-03-27 by bhsohn ������ ���� �������̽� ����
			if(g_pShuttleChild->IsObserverMode())
			{
				return INF_MSGPROC_NORMAL;
			}
			// end 2007-03-27 by bhsohn ������ ���� �������̽� ����

			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			// 2006-07-27 by ispark
			if(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem &&
				((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType == ITEM_QSLOT_POS)
			{
				m_bLButtonDown = TRUE;
			}

			if( pt.y > m_nY &&
				pt.y < m_nY+QSLOT_SIZE_Y &&
				pt.x > m_nX &&
				pt.x < m_nX+QSLOT_SIZE_X)
			{
				int i = (pt.x - m_nX - 1)/QSLOT_ICON_INTERVAL;

				if( m_pQSlotInfo[m_nCurrentTab][i].pItem && 
					i >= 0 && 
					i < QSLOT_NUMBER )
				{					
//					g_pGameMain->SetItemInfo(m_pQSlotInfo[m_nCurrentTab][i].pItem->UniqueNumber, 
//						m_pQSlotInfo[m_nCurrentTab][i].pItem->ItemNum, pt.x, pt.y);
					SetToolTip(pt.x-10, pt.y+13, m_pQSlotInfo[m_nCurrentTab][i].pItem);
					return INF_MSGPROC_BREAK;
				}
				else
				{
					((CINFGameMain*)m_pParent)->SetToolTip(0, 0,NULL);
				}
			}
			else
			{
				// 2007-01-22 by dgwoo ����â�������� ������ �Ѵ� ������ �ּ�ó���� �������̽��� 
				// ������ QSlot�� ������ �׳� ���� ���װ� �����.
				((CINFGameMain*)m_pParent)->SetToolTip(0, 0,NULL);
			}
			// current tab �̵� ��ư
			if(GetButtonStateOnMouse(pt, QSLOT_BUTTON_UP_START_X, QSLOT_BUTTON_UP_START_Y, QSLOT_BUTTON_SIZE_X, QSLOT_BUTTON_SIZE_Y))
			{
				m_nButtonState[QSLOT_BUTTON_UP] = BUTTON_STATE_UP;
				m_nButtonState[QSLOT_BUTTON_DOWN] = BUTTON_STATE_NORMAL;
			}
			else if(GetButtonStateOnMouse(pt, QSLOT_BUTTON_DOWN_START_X,QSLOT_BUTTON_DOWN_START_Y, QSLOT_BUTTON_SIZE_X, QSLOT_BUTTON_SIZE_Y))
			{
				m_nButtonState[QSLOT_BUTTON_UP] = BUTTON_STATE_NORMAL;
				m_nButtonState[QSLOT_BUTTON_DOWN] = BUTTON_STATE_UP;
			}
			else
			{
				m_nButtonState[QSLOT_BUTTON_UP] = BUTTON_STATE_NORMAL;
				m_nButtonState[QSLOT_BUTTON_DOWN] = BUTTON_STATE_NORMAL;
			}
		}
		break;
	case WM_RBUTTONDOWN:
		{
			// 2007-03-27 by bhsohn ������ ���� �������̽� ����
			if(g_pShuttleChild->IsObserverMode())
			{
				return INF_MSGPROC_NORMAL;
			}
			// end 2007-03-27 by bhsohn ������ ���� �������̽� ����

			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			if( pt.y>m_nY &&
				pt.y<m_nY+QSLOT_SIZE_Y &&
				pt.x > m_nX &&
				pt.x < m_nX+QSLOT_SIZE_X)
			{
				// 2005-07-19 by ispark
				// I, V��ư ������ �������� ������ ���� ������ ���´�.
				// ���� �������� 0�̸� â�� ���� ���°� �ƴϴ�.
				if(!g_pGameMain->m_nLeftWindowInfo && !g_pGameMain->m_nRightWindowInfo)
				{
					break;
				}

				int i = (pt.x - m_nX - 1)/QSLOT_ICON_INTERVAL;
				if( m_pQSlotInfo[m_nCurrentTab][i].pItem &&
					i >= 0 && 
					i < QSLOT_NUMBER ) 
				{
					m_pQSlotInfo[m_nCurrentTab][i].pItem = NULL;								
				}
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			// 2007-03-27 by bhsohn ������ ���� �������̽� ����
			if(g_pShuttleChild->IsObserverMode())
			{
				return INF_MSGPROC_NORMAL;
			}
			// end 2007-03-27 by bhsohn ������ ���� �������̽� ����

			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if( pt.y>m_nY &&
				pt.y<m_nY+QSLOT_SIZE_Y &&
				pt.x > m_nX &&
				pt.x < m_nX+QSLOT_SIZE_X)
			{
				// 2005-07-19 by ispark
				// I, V��ư ������ �������� ������ ���� Ŭ��(�̵�)�� ���´�.
				// ���� �������� 0�̸� â�� ���� ���°� �ƴϴ�.
				if(!g_pGameMain->m_nLeftWindowInfo && !g_pGameMain->m_nRightWindowInfo)
				{
					break;
				}
				int i = (pt.x - m_nX - 1)/QSLOT_ICON_INTERVAL;

				if(NULL != m_pQSlotInfo[m_nCurrentTab][i].pItem &&
					IS_GENERAL_ITEM(m_pQSlotInfo[m_nCurrentTab][i].pItem->Kind) &&
					g_pInterface->m_pBazaarShop == NULL)								// 2006-08-03 by ispark, ���λ����� ������ ������
				{

					if(IS_COUNTABLE_ITEM(m_pQSlotInfo[m_nCurrentTab][i].pItem->Kind))
					{
						ITEM *pITEM = g_pDatabase->GetServerItemInfo(m_pQSlotInfo[m_nCurrentTab][i].pItem->ItemNum);
						if(pITEM)
						{
							CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber(((ITEM_GENERAL*)m_pQSlotInfo[m_nCurrentTab][i].pItem)->UniqueNumber);
							if(pItemInfo == NULL)
							{
								SetQSlotInfo(m_nCurrentTab,i,NULL);
								m_bLButtonDown = FALSE;
								return INF_MSGPROC_BREAK;
							}
						}
					}
				}
				
				// 2006-07-27 by ispark
				if(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem &&
					((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType == ITEM_QSLOT_POS)
				{
					m_bLButtonDown = TRUE;
					return INF_MSGPROC_BREAK;
				}

				m_nItemType	= QSLOT_ITEMTYPE_NONE;
				if( m_pQSlotInfo[m_nCurrentTab][i].pItem &&
					i >= 0 && 
					i < QSLOT_NUMBER ) 
				{
//					if( g_pGameMain->m_pInven->m_pSelectItem == NULL &&
//						g_pGameMain->m_pCharacterInfo->m_pSelectSkill == NULL)
					if(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem == NULL)
					{
						m_nRenderMoveIconIntervalWidth  = (pt.x - m_nX) - i*QSLOT_ICON_INTERVAL + 1;
						m_nRenderMoveIconIntervalHeight = pt.y - m_nY + 1;
						m_nSelectSlotNumber = i;
						
						if(IS_SKILL_ITEM(m_pQSlotInfo[m_nCurrentTab][i].pItem->Kind))
						{
//							g_pGameMain->m_pCharacterInfo->m_pSelectSkill = (ITEM_SKILL*)m_pQSlotInfo[m_nCurrentTab][i].pItem;
							SetSelectItem(&m_pQSlotInfo[m_nCurrentTab][i]);
							m_nItemType				= QSLOT_ITEMTYPE_SKILL;
						}
						else
						{
//							g_pGameMain->m_pInven->m_pSelectItem = (CItemInfo*)m_pQSlotInfo[m_nCurrentTab][i].pItem;
							SetSelectItem(&m_pQSlotInfo[m_nCurrentTab][i]);
							m_nItemType				= QSLOT_ITEMTYPE_ITEM;
						}						
						m_bQSlotSwapFlag		= TRUE;
						m_nQSlotSwapTab			= m_nCurrentTab;
						m_nQSlotSwapNum			= i;

						m_pQSlotMove = m_pQSlotInfo[m_nCurrentTab][i].pItem;
						m_pQSlotInfo[m_nCurrentTab][i].pItem = NULL;
					}
					else
					{
						m_bQSlotSwapFlag		= FALSE;
					}
					
					return INF_MSGPROC_BREAK;
				}
			}
			else
			{
				if(m_bLButtonDown && 
					((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType == ITEM_QSLOT_POS)
				{
					m_nSelectSlotNumber = -1;
					m_nItemType = QSLOT_ITEMTYPE_NONE;
					m_bLButtonDown = FALSE;
					SetSelectItem(NULL);
					return INF_MSGPROC_BREAK;
				}
			}
			// current tab �̵� ��ư
			if(GetButtonStateOnMouse(pt, QSLOT_BUTTON_UP_START_X, QSLOT_BUTTON_UP_START_Y, QSLOT_BUTTON_SIZE_X, QSLOT_BUTTON_SIZE_Y))
			{
				m_nButtonState[QSLOT_BUTTON_UP] = BUTTON_STATE_DOWN;
				m_nButtonState[QSLOT_BUTTON_DOWN] = BUTTON_STATE_NORMAL;
			}
			else if(GetButtonStateOnMouse(pt, QSLOT_BUTTON_DOWN_START_X,QSLOT_BUTTON_DOWN_START_Y, QSLOT_BUTTON_SIZE_X, QSLOT_BUTTON_SIZE_Y))
			{
				m_nButtonState[QSLOT_BUTTON_UP] = BUTTON_STATE_NORMAL;
				m_nButtonState[QSLOT_BUTTON_DOWN] = BUTTON_STATE_DOWN;
			}
			else
			{
				m_nButtonState[QSLOT_BUTTON_UP] = BUTTON_STATE_NORMAL;
				m_nButtonState[QSLOT_BUTTON_DOWN] = BUTTON_STATE_NORMAL;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			// 2007-03-27 by bhsohn ������ ���� �������̽� ����
			if(g_pShuttleChild->IsObserverMode())
			{
				return INF_MSGPROC_NORMAL;
			}
			// end 2007-03-27 by bhsohn ������ ���� �������̽� ����
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if(g_pGameMain->m_pInven->m_bSelectWearItem) break;

			// 2006-07-27 by ispark
			ITEM_BASE* pSelectItem = NULL;
			if(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem)
			{
				pSelectItem = (ITEM_BASE*)(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem->pItem); 
			}
//			ITEM_BASE* pItem = NULL;
//			if(m_pSelectItem == NULL)
//			{
//				pItem = (ITEM_BASE*)((CINFGameMain*)m_pParent)->m_pInven->m_pSelectItem;
//				if(!pItem)
//				{
//					pItem = (ITEM_BASE*)((CINFGameMain*)m_pParent)->m_pCharacterInfo->m_pSelectSkill;
//				}
//			}

//			if( (m_pSelectItem || pItem) && 
			if( (pSelectItem && 
				(m_bLButtonDown || ((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType != ITEM_QSLOT_POS)) &&
				pt.y>m_nY &&
				pt.y<m_nY+QSLOT_SIZE_Y)
			{
				int i = (pt.x - m_nX - 1)/QSLOT_ICON_INTERVAL;
				if( i >= 0 && 
					i < QSLOT_NUMBER ) 
				{
//					if(m_pSelectItem)
//						SetQSlotInfo(m_nCurrentTab,i, m_pSelectItem);
//					else
//					{
//						SetQSlotInfo(m_nCurrentTab,i, pItem);
//					}
					SetQSlotInfo(m_nCurrentTab,i, pSelectItem);
					
					m_nSelectSlotNumber = -1;
					m_nItemType = QSLOT_ITEMTYPE_NONE;
					m_bLButtonDown = FALSE;
//					g_pGameMain->m_pInven->m_pSelectItem = NULL;
//					g_pGameMain->m_pInven->m_pSelectIcon = NULL;
//					g_pGameMain->m_pCharacterInfo->m_pSelectSkill = NULL;
//					g_pGameMain->m_pCharacterInfo->m_pSelectIcon = NULL;
					SetSelectItem(NULL);
					break;
				}
			}
			
			// Ŭ���� �����Ǵºκ� ��
			if( m_nSelectSlotNumber>=0 && 
				(pt.x < m_nX || 
				pt.x > m_nX+QSLOT_SIZE_X ||
				pt.y < m_nY || 
				pt.y > m_nY+QSLOT_SIZE_Y))
			{
				m_nItemType = QSLOT_ITEMTYPE_NONE;
				SetQSlotInfo(m_nCurrentTab,m_nSelectSlotNumber,NULL);
				m_bLButtonDown = FALSE;
//				g_pGameMain->m_pInven->m_pSelectItem = NULL;
//				g_pGameMain->m_pCharacterInfo->m_pSelectSkill = NULL;
				SetSelectItem(NULL);
			}
			m_nSelectSlotNumber = -1;
			
			// current tab �̵� ��ư
			if(GetButtonStateOnMouse(pt, QSLOT_BUTTON_UP_START_X, QSLOT_BUTTON_UP_START_Y, QSLOT_BUTTON_SIZE_X, QSLOT_BUTTON_SIZE_Y))
			{
				if(m_nButtonState[QSLOT_BUTTON_UP] == BUTTON_STATE_DOWN)
				{
					if(m_nCurrentTab > 0)
						m_nCurrentTab --;
				}
				m_nButtonState[QSLOT_BUTTON_UP] = BUTTON_STATE_UP;
				m_nButtonState[QSLOT_BUTTON_DOWN] = BUTTON_STATE_NORMAL;
			}
			else if(GetButtonStateOnMouse(pt, QSLOT_BUTTON_DOWN_START_X,QSLOT_BUTTON_DOWN_START_Y, QSLOT_BUTTON_SIZE_X, QSLOT_BUTTON_SIZE_Y))
			{
				if(m_nButtonState[QSLOT_BUTTON_DOWN] == BUTTON_STATE_DOWN)
				{
//					if(m_nCurrentTab<QSLOT_TAB_NUMBER-2)
//						m_nCurrentTab ++;
					if(m_nCurrentTab<REAL_TAB_NUMBER-1)
						m_nCurrentTab ++;
				}
				m_nButtonState[QSLOT_BUTTON_UP] = BUTTON_STATE_NORMAL;
				m_nButtonState[QSLOT_BUTTON_DOWN] = BUTTON_STATE_UP;
			}
			else
			{
				m_nButtonState[QSLOT_BUTTON_UP] = BUTTON_STATE_NORMAL;
				m_nButtonState[QSLOT_BUTTON_DOWN] = BUTTON_STATE_NORMAL;
			}

			// ������ ���� ��������� ����
			//g_pGameMain->m_pCharacterInfo->m_pSelectSkill = NULL;
			//g_pGameMain->m_pInven->m_pSelectItem = NULL;

		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			// 2007-03-27 by bhsohn ������ ���� �������̽� ����
			if(g_pShuttleChild->IsObserverMode())
			{
				return INF_MSGPROC_NORMAL;
			}
			// end 2007-03-27 by bhsohn ������ ���� �������̽� ����
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			// 2006-03-31 by ispark, ���� ����
			if( pt.x > m_nX && 
				pt.x < m_nX + QSLOT_SIZE_X &&
				pt.y>m_nY &&
				pt.y<m_nY+QSLOT_SIZE_Y)
			{
				int i = (pt.x - m_nX - 1)/QSLOT_ICON_INTERVAL;
				if( m_pQSlotInfo[m_nCurrentTab][i].pItem && 
					i >= 0 && 
					i < QSLOT_NUMBER )
//					pt.x<m_nX+QSLOT_ICON_START_X+QSLOT_ICON_INTERVAL*i+QSLOT_ICON_SIZE)
				{
					UseQuickSlot(m_nCurrentTab, i);
				}
			}
		}
		break;
	case WM_KEYDOWN:
		{
			// 2007-03-27 by bhsohn ������ ���� �������̽� ����
			if(g_pShuttleChild->IsObserverMode())
			{
				return INF_MSGPROC_NORMAL;
			}
			// end 2007-03-27 by bhsohn ������ ���� �������̽� ����

			if(wParam == VK_TAB)
			{
				if(m_nCurrentTab<REAL_TAB_NUMBER-1)
				{//���� ����.
					m_nCurrentTab ++;
					m_nSelectSlotNumber = -1;
//					g_pGameMain->m_pInven->m_pSelectItem = NULL;
//					g_pGameMain->m_pCharacterInfo->m_pSelectSkill = NULL;
//					g_pGameMain->m_pInven->m_pSelectIcon = NULL;
//					g_pGameMain->m_pCharacterInfo->m_pSelectIcon = NULL;
					SetSelectItem(NULL);
				}
				else
				{//���� �ʱ�ȭ.
					m_nCurrentTab = 0;
					m_nSelectSlotNumber = -1;
//					g_pGameMain->m_pInven->m_pSelectItem = NULL;
//					g_pGameMain->m_pCharacterInfo->m_pSelectSkill = NULL;
//					g_pGameMain->m_pInven->m_pSelectIcon = NULL;
//					g_pGameMain->m_pCharacterInfo->m_pSelectIcon = NULL;
					SetSelectItem(NULL);
				}
				m_nButtonState[QSLOT_BUTTON_DOWN] = BUTTON_STATE_DOWN;
			}
		}
		break;
	case WM_KEYUP:
		{
			// 2007-03-27 by bhsohn ������ ���� �������̽� ����
			if(g_pShuttleChild->IsObserverMode())
			{
				return INF_MSGPROC_NORMAL;
			}
			// end 2007-03-27 by bhsohn ������ ���� �������̽� ����
			
			if(wParam == VK_TAB)
			{
				m_nButtonState[QSLOT_BUTTON_DOWN] = BUTTON_STATE_NORMAL;
			}
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
#endif

}

BOOL CINFGameMainQSlot::UseQuickSlot(int nCurrentTab, int nSlotNumber)
{
	// 2008-12-15 by bhsohn ��ü�� �׾�����, ����Ŭ������ ��ų �ߵ� ���� ����
	BOOL bUseSkill = FALSE;
	// 2007-03-27 by bhsohn ������ ���� �������̽� ����
	if(g_pShuttleChild->IsObserverMode())
	{
		return bUseSkill;
	}	
	// end 2007-03-27 by bhsohn ������ ���� �������̽� ����
	// 2008-10-30 by bhsohn ���� ���� ���̵� �ƿ��� ���ý��� ����
	if(FALSE == g_pD3dApp->IsLockMode())	// FALSE :Lock  TRUE : UnLock	
	{
		return bUseSkill;
	}
	// end 2008-10-30 by bhsohn ���� ���� ���̵� �ƿ��� ���ý��� ����
	// 2008-12-15 by bhsohn ��ü�� �׾�����, ����Ŭ������ ��ų �ߵ� ���� ����
	else if( !IsPossibleJoysticSlot())
	{
		return bUseSkill;		
	}
	// end 2008-12-15 by bhsohn ��ü�� �׾�����, ����Ŭ������ ��ų �ߵ� ���� ����

	FLOG( "CINFGameMainQSlot::UseQuickSlot(int nCurrentTab, int nSlotNumber)" );
	if(IsValidQSlotInfo(nCurrentTab, nSlotNumber))
	{
		if(IS_GENERAL_ITEM(m_pQSlotInfo[nCurrentTab][nSlotNumber].pItem->Kind) &&
			g_pInterface->m_pBazaarShop == NULL)								// 2006-08-03 by ispark, ���λ����� ������ ������
		{
			char buf[256];
			memset(buf, 0x00, sizeof(buf));
			if(IS_COUNTABLE_ITEM(m_pQSlotInfo[nCurrentTab][nSlotNumber].pItem->Kind))
			{
				ITEM *pITEM = g_pDatabase->GetServerItemInfo(m_pQSlotInfo[nCurrentTab][nSlotNumber].pItem->ItemNum);
				if(pITEM)
				{
					CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber(((ITEM_GENERAL*)m_pQSlotInfo[nCurrentTab][nSlotNumber].pItem)->UniqueNumber);
					if(pItemInfo)
					{
						wsprintf(buf, STRMSG_C_TOOLTIP_0013,pITEM->ItemName,pItemInfo->CurrentCount);//"%s (%d ��)"
					}
				}
			}
			else
			{
				ITEM *pITEM = g_pDatabase->GetServerItemInfo(m_pQSlotInfo[nCurrentTab][nSlotNumber].pItem->ItemNum);
				if(pITEM)
					wsprintf(buf, "%s",pITEM->ItemName);
			}
			if(strlen(buf) <= 0 )
			{
				SetQSlotInfo( nCurrentTab, nSlotNumber, NULL );
				return bUseSkill;
			}
//			char msg[256];
//			wsprintf(msg, "%s�� ����մϴ�",buf);
//			g_pD3dApp->m_pChat->CreateChatChild(msg,COLOR_SKILL_USE);



			// 2008. 12. 16 by ckPark ������ ��� ����
			//((CINFGameMain*)m_pParent)->m_pInven->SendUseItem(m_pQSlotInfo[nCurrentTab][nSlotNumber].pItem);

			// ������ ���� ������
			// 2010. 02. 11 by ckPark �ߵ��� ����������
			//CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( m_pQSlotInfo[nCurrentTab][nSlotNumber].pItem->UniqueNumber );
			CItemInfo* pItemInfo;
			if( m_pQSlotInfo[nCurrentTab][nSlotNumber].pItem->ItemInfo->InvokingDestParamID
				|| m_pQSlotInfo[nCurrentTab][nSlotNumber].pItem->ItemInfo->InvokingDestParamIDByUse )
			{
				pItemInfo = g_pStoreData->FindItemInWearByItemNum( m_pQSlotInfo[nCurrentTab][nSlotNumber].pItem->ItemNum );
			}
			else
				pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( m_pQSlotInfo[nCurrentTab][nSlotNumber].pItem->UniqueNumber );
			// end 2010. 02. 11 by ckPark �ߵ��� ����������



			// ������ ������ ������ ���
			if(pItemInfo)
			{
				// AttackTime�� 0�̻��� �͵鸸 ������ ����Ʈ�� �߰��Ѵ�
				if(pItemInfo->GetItemInfo()->AttackTime > 0)
				{
					// ������ ����Ʈ�� ���������� �÷��� ���
					if(g_pGameMain->PushDelayItem(pItemInfo))
						// 2008-12-15 by bhsohn ��ü�� �׾�����, ����Ŭ������ ��ų �ߵ� ���� ����
						bUseSkill = TRUE;
				}
				else
				{
					// AttackTime�� 0������ �͵��� �ٷ� ��Ŷ�� ������

					// 2010. 02. 11 by ckPark �ߵ��� ����������
					//((CINFGameMain*)m_pParent)->m_pInven->SendUseItem(m_pQSlotInfo[nCurrentTab][nSlotNumber].pItem);
					((CINFGameMain*)m_pParent)->m_pInven->SendUseItem(pItemInfo);
					// end 2010. 02. 11 by ckPark �ߵ��� ����������

					bUseSkill = TRUE;
				}
 			}	 
            // ���� ��Ÿ�� ����
			m_fQSlotTimer = QSLOT_TIMER;
			// end 2008. 12. 16 by ckPark ������ ��� ����


		}
		else if(IS_SKILL_ITEM(m_pQSlotInfo[nCurrentTab][nSlotNumber].pItem->Kind) && 
//			g_pD3dApp->m_bCharacter == FALSE &&		// 2006-07-28 by ispark
			g_pD3dApp->m_bChatMode == FALSE)//&&	// 2005-12-03 by ispark, ä���߿� ��� ����
//			(g_pShuttleChild->m_bUnitStop == FALSE && g_pShuttleChild->m_bLandedMove == FALSE)&&		// 2005-08-08 by ispark ĳ������ ���� �������� ��ų ���� �ʱ�
//			g_pShuttleChild->m_bPenaltyUnitStop == FALSE)	// 2005-08-10 by ispark ���Ƽ�� �� ��ų ��� ����
		{
			// 2009-03-31 by bhsohn 1��Ī ���� ���� �����, ���� ����
			// 2007-03-23 by bhsohn �Ѹ��ϸ鼭 ���� ���� �ȵǴ� ���� �ذ�
// 			if(SKILL_BASE_NUM(m_pQSlotInfo[nCurrentTab][nSlotNumber].pItem->ItemNum) == AGEAR_SKILL_BASENUM_AIRSIEGEMODE
// 				&& g_pShuttleChild->IsMyShuttleRolling())
// 			{
// 				return bUseSkill;
// 			}
			if(SKILL_BASE_NUM(m_pQSlotInfo[nCurrentTab][nSlotNumber].pItem->ItemNum) == AGEAR_SKILL_BASENUM_AIRSIEGEMODE)
			{
				//if(g_pShuttleChild->IsMyShuttleRolling())
 				//{
 					//DbgOut("bUseSkill %d\n", bUseSkill);
				//	return bUseSkill;
 				//}
				if(g_pShuttleChild && g_pShuttleChild->m_bTurnCamera)
				{
					g_pShuttleChild->SetBackView(FALSE);
				}
			// end 2010.10.22 by jskim �Ѹ� �߿��� ������� ��밡���ϵ��� ����

			}
			// end 2007-03-23 by bhsohn �Ѹ��ϸ鼭 ���� ���� �ȵǴ� ���� �ذ�
			// end 2009-03-31 by bhsohn 1��Ī ���� ���� �����, ���� ����

			// 2007-05-30 by bhsohn ���̴� ���� ���� ��� �Ұ���
			if((NULL == g_pShuttleChild->m_pRadarItemInfo)
				&& (SKILL_BASE_NUM(m_pQSlotInfo[nCurrentTab][nSlotNumber].pItem->ItemNum) == BGEAR_SKILL_BASENUM_AIRBOMBINGMODE))
			{
				m_fQSlotTimer = QSLOT_TIMER;
				g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051229_0101, COLOR_SYSTEM);// "���̴ٰ� ���� �Ǿ� ���� �ʽ��ϴ�."
				return bUseSkill;
			}
			// end 2007-05-30 by bhsohn ���̴� ���� ���� ��� �Ұ���

			// 2007-07-09 by bhsohn ��ݰ� ���ڻ��� ���� ���� ������ ó��
			if(IS_BAZAAR_SKILL(m_pQSlotInfo[nCurrentTab][nSlotNumber].pItem->ItemInfo))
			{
				// 2008-11-3 by bhsohn ���� ���� �ߺ����� �� ������ ����
				if(g_pInterface->IsBazarOpen())
				{
					m_fQSlotTimer = QSLOT_TIMER;					
					return bUseSkill;
				}
				// end 2008-11-3 by bhsohn ���� ���� �ߺ����� �� ������ ����

				m_fClickBazaar = BAZAAR_CLICK_TIME;								
			}
			// end 2007-07-09 by bhsohn ��ݰ� ���ڻ��� ���� ���� ������ ó��
			
			((CINFGameMain*)m_pParent)->m_pCharacterInfo->SendUseSkill(m_pQSlotInfo[nCurrentTab][nSlotNumber].pItem);
			m_fQSlotTimer = QSLOT_TIMER;
			// 2008-12-15 by bhsohn ��ü�� �׾�����, ����Ŭ������ ��ų �ߵ� ���� ����
			bUseSkill = TRUE;
		}
	}	
	// 2008-12-15 by bhsohn ��ü�� �׾�����, ����Ŭ������ ��ų �ߵ� ���� ����
	return bUseSkill;
}

// 2007-07-09 by bhsohn ��ݰ� ���ڻ��� ���� ���� ������ ó��
BOOL CINFGameMainQSlot::IsBazarOpen()
{
	if(m_fClickBazaar > 0)
	{
		return TRUE;
	}	
	if(g_pInterface->m_pBazaarShop)
	{
		return TRUE;
	}
	return FALSE;
}


void CINFGameMainQSlot::SetQSlotInfo(int nTab, int nNumber, ITEM_BASE* pItem)
{
	FLOG( "CINFGameMainQSlot::SetQSlotInfo(int nTab, int nNumber, ITEM_BASE* pItem)" );

	if(pItem)
	{
		// 2012-12-21 by bhsohn [�巹�θ��] ������ ���� ��� �ȵǰ� ����		
		if(!IsPossibleSetQuickSlot(pItem->ItemNum))
		{
			if(g_pGameMain && g_pGameMain->m_pInfWindow)
			{
				g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_060728_0003, _MESSAGE);	// "����� �� �� ���� ������ �Դϴ�."
			}
			return;
		}
		// END 2012-12-21 by bhsohn [�巹�θ��] ������ ���� ��� �ȵǰ� ����
		if(m_bQSlotSwapFlag == TRUE)
		{
			strcpy(m_pQSlotInfo[m_nQSlotSwapTab][m_nQSlotSwapNum].Name, m_pQSlotInfo[nTab][nNumber].Name);
			strcpy(m_pQSlotInfo[m_nQSlotSwapTab][m_nQSlotSwapNum].IconName, m_pQSlotInfo[nTab][nNumber].IconName);		
			m_pQSlotInfo[m_nQSlotSwapTab][m_nQSlotSwapNum].pItem = m_pQSlotInfo[nTab][nNumber].pItem;

			// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
			// ������ ���Ұ��� ó��
			if(m_pQSlotInfo[m_nQSlotSwapTab][m_nQSlotSwapNum].pItem)
			{
				g_pSOptionCharacter->ItemNum[m_nQSlotSwapTab][m_nQSlotSwapNum] 
					= m_pQSlotInfo[m_nQSlotSwapTab][m_nQSlotSwapNum].pItem->ItemNum;
				g_pSOptionCharacter->UniqueNumber[m_nQSlotSwapTab][m_nQSlotSwapNum] 
					= m_pQSlotInfo[m_nQSlotSwapTab][m_nQSlotSwapNum].pItem->UniqueNumber;
			}
			else
			{
				g_pSOptionCharacter->ItemNum[m_nQSlotSwapTab][m_nQSlotSwapNum] =0;
				g_pSOptionCharacter->UniqueNumber[m_nQSlotSwapTab][m_nQSlotSwapNum]  =0;
			}
			// end 2007-11-22 by bhsohn �Ʒ��� ���ռ���

			m_bQSlotSwapFlag = FALSE;
			m_nQSlotSwapTab = 0;
			m_nQSlotSwapNum = 0;
 			m_pQSlotMove = NULL;
			m_pQSlotInfo[nTab][nNumber].pItem = NULL;
		}

		// 2005-11-25 by ispark, �ߺ� Ȯ��
		// 2006-10-12 by ispark, ����ũ �ѹ��� Ȯ��
		int i;
		for(i = 0; i < QSLOT_NUMBER; i++)
		{
			if(m_pQSlotInfo[nTab][i].pItem && (m_pQSlotInfo[nTab][i].pItem->UniqueNumber == pItem->UniqueNumber))
			{
				return;
			}
		}


		// 2010. 02. 11 by ckPark �ߵ��� ����������
		if( pItem->ItemInfo->InvokingDestParamID
			|| pItem->ItemInfo->InvokingDestParamIDByUse )
		{
			// ������ �ߵ��� ��ű� �������� �����Կ� �Ѱ��� �ø� �� �ִ�
			for( i=0; i<QSLOT_NUMBER; ++i )
			{
				if( m_pQSlotInfo[nTab][i].pItem && (pItem->ItemNum == m_pQSlotInfo[nTab][i].pItem->ItemNum) )
					return;
			}
		}
		// end 2010. 02. 11 by ckPark �ߵ��� ����������


		ITEM *item = g_pDatabase->GetServerItemInfo(pItem->ItemNum);
		if(item)
			strcpy(m_pQSlotInfo[nTab][nNumber].Name, item->ItemName);
//		wsprintf( m_pQSlotInfo[nTab][nNumber].IconName, "%08d", SKILL_BASE_NUM(pItem->ItemNum));	// 2005-08-23 by ispark

		// 2010. 04. 01 by ckPark ���ҽ� ���� �ý��۽� ���丮�� �����ҿ��� ������ ȸ���� �� ������ �ȵǴ� ���� �ذ�

		//wsprintf( m_pQSlotInfo[nTab][nNumber].IconName, "%08d", SKILL_BASE_NUM(pItem->ItemInfo->SourceIndex));
		CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( pItem->UniqueNumber );
		if( pItemInfo )
			SetIconName( pItemInfo, m_pQSlotInfo[nTab][nNumber].IconName );
		// 2013-03-13 by bhsohn ��ų ��ȣ ���� ����
		else
			sprintf( m_pQSlotInfo[nTab][nNumber].IconName, "%08d", SKILL_BASE_SOURCEINDEX( pItem->ItemInfo->SourceIndex ) );
// 		else
// 			sprintf( m_pQSlotInfo[nTab][nNumber].IconName, "%08d", SKILL_BASE_NUM( pItem->ItemInfo->SourceIndex ) );
		// END 2013-03-13 by bhsohn ��ų ��ȣ ���� ����

		// end 2010. 04. 01 by ckPark ���ҽ� ���� �ý��۽� ���丮�� �����ҿ��� ������ ȸ���� �� ������ �ȵǴ� ���� �ذ�


//		m_pQSlotInfo[nTab][nNumber].pItem = pItem;
		// 2006-05-29 by ispark, ������ �ּҰ��� �־�����, ������ ���ؼ� ���� �����Ѵ�. 
		// �� �����Կ� �������� �ִٸ� �����Ѵ�.
		util::del(m_pQSlotInfo[nTab][nNumber].pItem);
		m_pQSlotInfo[nTab][nNumber].pItem = new ITEM_BASE ;
		m_pQSlotInfo[nTab][nNumber].pItem->Kind = pItem->Kind ;
		m_pQSlotInfo[nTab][nNumber].pItem->ItemNum= pItem->ItemNum;
		m_pQSlotInfo[nTab][nNumber].pItem->ItemInfo = pItem->ItemInfo;
		m_pQSlotInfo[nTab][nNumber].pItem->UniqueNumber = pItem->UniqueNumber;
 		util::del(m_pQSlotMove);

		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���		
		{
			g_pSOptionCharacter->ItemNum[nTab][nNumber] = pItem->ItemNum;
			g_pSOptionCharacter->UniqueNumber[nTab][nNumber] = pItem->UniqueNumber;
		}
	}
	else
	{
		// 2006-05-29 by ispark, ����� �ڵ�
 		util::del(m_pQSlotMove);
		util::del(m_pQSlotInfo[nTab][nNumber].pItem);
		memset(&m_pQSlotInfo[nTab][nNumber], 0x00, sizeof(INVEN_DISPLAY_INFO));

		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
		{
			g_pSOptionCharacter->ItemNum[nTab][nNumber] = 0;
			g_pSOptionCharacter->UniqueNumber[nTab][nNumber] = 0;
		}
		// end 2007-11-22 by bhsohn �Ʒ��� ���ռ���
	}
//	g_pD3dApp->m_pInterface->SaveCharacterFile();			// 2006-09-05 by ispark
	
	// 2013-07-01 by bhsohn QuickSlot����� �ð� üũ�Ͽ� �ٷ� �����ϰ� ����
	// 1���� �ڵ� ���� 
	SetCheckQuiclSlotSave(QUICKSLOT_SAVE_CHECK_TIME); 
	// END 2013-07-01 by bhsohn QuickSlot����� �ð� üũ�Ͽ� �ٷ� �����ϰ� ����
}

BOOL CINFGameMainQSlot::IsValidQSlotInfo(int nTab, int nNumber)
{
	FLOG( "CINFGameMainQSlot::IsValidQSlotInfo(int nTab, int nNumber)" );
	if(m_pQSlotInfo[nTab][nNumber].pItem)
	{
		// ������ Ȯ��
		if(IS_GENERAL_ITEM(m_pQSlotInfo[nTab][nNumber].pItem->Kind))
		{
			CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber(((ITEM_GENERAL*)m_pQSlotInfo[nTab][nNumber].pItem)->UniqueNumber);
			if( pItemInfo )
			{
				return TRUE;
			}
		}
		// ��ų(UTC) Ȯ��
		else if(IS_SKILL_ITEM(m_pQSlotInfo[nTab][nNumber].pItem->Kind))
		{
//			map<int, ITEM_SKILL*>::iterator it = g_pShuttleChild->m_pSkill->m_mapEnableSkill.find(((ITEM_SKILL*)m_pQSlotInfo[nTab][nNumber].pItem)->ItemNum);
			map<int, CSkillInfo*>::iterator itSkillInfo = g_pShuttleChild->m_pSkill->m_mapSkill.find(((ITEM_SKILL*)m_pQSlotInfo[nTab][nNumber].pItem)->ItemNum);
			if(itSkillInfo != g_pShuttleChild->m_pSkill->m_mapSkill.end())
			{
				return TRUE;
			}
		}
	}
				
	return FALSE;
}			

void CINFGameMainQSlot::UpdateQick(int nItemNum) 
{	
	FLOG( "CINFGameMainQSlot::UpdateQick(int nItemNum)" );
	if(nItemNum)
	{
		for(int i=0;i<QSLOT_TAB_NUMBER;i++)
		{
			for(int j=0;j<QSLOT_NUMBER;j++)
			{
				
				if(g_pSOptionCharacter->ItemNum[i][j] == nItemNum)
				{
					ITEM *item = g_pDatabase->GetServerItemInfo(nItemNum);
					if(item && IS_GENERAL_ITEM(item->Kind))
					{
						// 2010. 04. 01 by ckPark ���ҽ� ���� �ý��۽� ���丮�� �����ҿ��� ������ ȸ���� �� ������ �ȵǴ� ���� �ذ�
						// char buff[128];
						// end 2010. 04. 01 by ckPark ���ҽ� ���� �ý��۽� ���丮�� �����ҿ��� ������ ȸ���� �� ������ �ȵǴ� ���� �ذ�
						util::del(m_pQSlotInfo[i][j].pItem);
						m_pQSlotInfo[i][j].pItem = new ITEM_BASE ;	
						m_pQSlotInfo[i][j].pItem->Kind = item->Kind;
						m_pQSlotInfo[i][j].pItem->UniqueNumber = g_pSOptionCharacter->UniqueNumber[i][j];
						m_pQSlotInfo[i][j].pItem->ItemNum = g_pSOptionCharacter->ItemNum[i][j];
						m_pQSlotInfo[i][j].pItem->ItemInfo = item;
						memset(m_pQSlotInfo[i][j].IconName,0x00,sizeof(m_pQSlotInfo[i][j].IconName));
//						wsprintf(buff, "%08d", SKILL_BASE_NUM(item->ItemNum)) ;	// 2005-08-23 by ispark

						// 2010. 04. 01 by ckPark ���ҽ� ���� �ý��۽� ���丮�� �����ҿ��� ������ ȸ���� �� ������ �ȵǴ� ���� �ذ�

// 						wsprintf(buff, "%08d", SKILL_BASE_NUM(item->SourceIndex)) ;
// 						strcpy(m_pQSlotInfo[i][j].IconName, buff) ; 
						CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( m_pQSlotInfo[i][j].pItem->UniqueNumber );
						if( pItemInfo )
							SetIconName( pItemInfo, m_pQSlotInfo[i][j].IconName );
						// 2013-03-13 by bhsohn ��ų ��ȣ ���� ����
						else
							sprintf( m_pQSlotInfo[i][j].IconName, "%08d", SKILL_BASE_SOURCEINDEX( item->SourceIndex ) );
// 						else
// 							sprintf( m_pQSlotInfo[i][j].IconName, "%08d", SKILL_BASE_NUM( item->SourceIndex ) );
						// END 2013-03-13 by bhsohn ��ų ��ȣ ���� ����

						// end 2010. 04. 01 by ckPark ���ҽ� ���� �ý��۽� ���丮�� �����ҿ��� ������ ȸ���� �� ������ �ȵǴ� ���� �ذ�
					}
					return;
				}
			}
		}
	}

}


void CINFGameMainQSlot::SetAllQSlotInfo()
{
	FLOG( "CINFGameMainQSlot::SetAllQSlotInfo()" );
//	InitQSlotItem() ;
		
	for(int i=0;i<QSLOT_TAB_NUMBER;i++)
	{
		for(int j=0;j<QSLOT_NUMBER;j++)
		{
			if( g_pSOptionCharacter->UniqueNumber[i][j] == 0 )
			{
				continue;
			}
			// 2012-12-21 by bhsohn [�巹�θ��] ������ ���� ��� �ȵǰ� ����
			if(!IsPossibleSetQuickSlot(g_pSOptionCharacter->ItemNum[i][j]))			
			{
				continue;
			}
			// END 2012-12-21 by bhsohn [�巹�θ��] ������ ���� ��� �ȵǰ� ����
			ITEM *pITEM = g_pDatabase->GetServerItemInfo(g_pSOptionCharacter->ItemNum[i][j]);
			if( pITEM == NULL )
			{
				// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
				util::del(m_pQSlotInfo[i][j].pItem);
				memset(&m_pQSlotInfo[i][j], 0x00, sizeof(INVEN_DISPLAY_INFO)) ;

				continue;
			}
			if(IS_SKILL_ITEM(pITEM->Kind))
			{

			}
			else
			{
				CItemInfo *pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( g_pSOptionCharacter->UniqueNumber[i][j] );
				if( pItemInfo == NULL )
				{
					if( IS_COUNTABLE_ITEM( pITEM->Kind ) == FALSE )
					{
						// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
						util::del(m_pQSlotInfo[i][j].pItem);
						memset(&m_pQSlotInfo[i][j], 0x00, sizeof(INVEN_DISPLAY_INFO)) ;

						continue;
					}
					pItemInfo = g_pStoreData->FindItemInInventoryByItemNum( g_pSOptionCharacter->ItemNum[i][j] );
					if( pItemInfo == NULL )
					{
						// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
						util::del(m_pQSlotInfo[i][j].pItem);
						memset(&m_pQSlotInfo[i][j], 0x00, sizeof(INVEN_DISPLAY_INFO)) ;

						continue;
					}
					g_pSOptionCharacter->UniqueNumber[i][j] = pItemInfo->UniqueNumber;
				}
			}
			util::del(m_pQSlotInfo[i][j].pItem);
			memset(&m_pQSlotInfo[i][j], 0x00, sizeof(INVEN_DISPLAY_INFO)) ;
			m_pQSlotInfo[i][j].pItem = new ITEM_BASE ;
			m_pQSlotInfo[i][j].pItem->Kind = pITEM->Kind ;
			m_pQSlotInfo[i][j].pItem->ItemNum= pITEM->ItemNum;
			m_pQSlotInfo[i][j].pItem->ItemInfo = pITEM;
			m_pQSlotInfo[i][j].pItem->UniqueNumber = g_pSOptionCharacter->UniqueNumber[i][j];
			//itoa(g_pSOption->ItemNum[i][j], m_pQSlotInfo[i][j].IconName, 10) ; 
//			wsprintf( m_pQSlotInfo[i][j].IconName, "%08d", SKILL_BASE_NUM(g_pSOptionCharacter->ItemNum[i][j]));

			// 2010. 04. 01 by ckPark ���ҽ� ���� �ý��۽� ���丮�� �����ҿ��� ������ ȸ���� �� ������ �ȵǴ� ���� �ذ�

			//wsprintf( m_pQSlotInfo[i][j].IconName, "%08d", SKILL_BASE_NUM(pITEM->SourceIndex)); // 2005-11-25 by ispark
			if ( IS_SKILL_ITEM(m_pQSlotInfo[i][j].pItem->Kind)  == FALSE) // 2011-11-09 by jhahn ������ ��ų ǥ�� ���� ����
			{
				CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( m_pQSlotInfo[i][j].pItem->UniqueNumber );
				if( pItemInfo )
					SetIconName( pItemInfo, m_pQSlotInfo[i][j].IconName );
			//	else
			//		sprintf( m_pQSlotInfo[i][j].IconName, "%08d", SKILL_BASE_NUM( pITEM->SourceIndex ) );
			}
			else
			{
				// 2013-03-13 by bhsohn ��ų ��ȣ ���� ����
//				sprintf( m_pQSlotInfo[i][j].IconName, "%08d", SKILL_BASE_NUM( pITEM->SourceIndex ) ); // 2011-11-09 by jhahn ������ ��ų ǥ�� ���� ����
				sprintf( m_pQSlotInfo[i][j].IconName, "%08d", SKILL_BASE_SOURCEINDEX( pITEM->SourceIndex ) ); // 2011-11-09 by jhahn ������ ��ų ǥ�� ���� ����
			}

			

			// end 2010. 04. 01 by ckPark ���ҽ� ���� �ý��۽� ���丮�� �����ҿ��� ������ ȸ���� �� ������ �ȵǴ� ���� �ذ�
		}
	}

}

BOOL CINFGameMainQSlot::RenderDisableSkill(int nSkillNum)
{
	CSkillInfo* pItemSkill = g_pShuttleChild->m_pSkill->FindItemSkill(nSkillNum);
	
//	if( pItemSkill->ItemInfo->SkillType != SKILLTYPE_PERMANENT &&
//		g_pD3dApp->m_dwGameState == _CITY )
//	{	// ���ÿ����� �нú� ��ų�� �����ϰ� �ٸ� ��ų�� ����� �� ����.		
//		return FALSE;
//	}	
//	
//	if( g_pShuttleChild->m_nCurrentPatternNumber == PATTERN_UNIT_WARP_OUT ||
//		g_pShuttleChild->m_nCurrentPatternNumber == PATTERN_UNIT_WARP_IN )
//	{	// �����߿��� ��ų�� ����� �� ����.
//		return FALSE;
//	}
//		
//	if((g_pShuttleChild->m_dwState != _NORMAL || (IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind) && g_pShuttleChild->m_bIsAir == FALSE)) &&
//		pItemSkill->ItemInfo->SkillType == SKILLTYPE_CLICK &&
//		SKILL_KIND(pItemSkill->ItemNum) == SKILL_KIND_CONTROL )		
//	{	// ������ �ִ� ��ų�� m_dwState�� _NORMAL ���¿��� ��밡���ϴ�.
//		return FALSE;
//	}

	if( pItemSkill->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ME &&
		pItemSkill->ItemInfo->ReqItemKind != ITEMKIND_ALL_ITEM &&
		g_pStoreData->IsWearItem( pItemSkill->ItemInfo->ReqItemKind ) == FALSE &&
		pItemSkill->ItemInfo->SkillType != SKILLTYPE_PERMANENT)
	{	// ��ų�� ������ ���Ⱑ �����Ǿ����� �ʽ��ϴ�		
		return FALSE;
	}

//	if(	g_pShuttleChild->m_bIsAir == TRUE &&
//		(SKILL_BASE_NUM(pItemSkill->ItemNum) == GROUND_ACCELERATOR ||
//		SKILL_BASE_NUM(pItemSkill->ItemNum) == FLASH_ACCELERATOR ||
//		SKILL_BASE_NUM(pItemSkill->ItemNum) == SIEGE_MODE ||
//		SKILL_BASE_NUM(pItemSkill->ItemNum) == SIEGE_DEFENSE_MODE) )
//	{	// ���󿡼��� ��밡���� ��ų�Դϴ�		
//		return FALSE;
//	}
//		
//	if( (pItemSkill->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_WITH_ME || 
//		pItemSkill->ItemInfo->SkillTargetType == SKILLTARGETTYPE_PARTY_WITHOUT_ME) &&
//		g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.size() == 0 )
//	{	// ��Ƽ �ÿ��� ��� ������ ��ų (���� �����ϰ� �ٸ� ��Ƽ���� ������ �� ��� ����)		
//		return FALSE;
//	}
//
//	if( g_pShuttleChild->m_pSkill->IsSingleSkill(pItemSkill->ItemNum) && g_pShuttleChild->m_pSkill->IsExistSingleSkill(FALSE))
//	{
//		return FALSE;
//	}
//	
//	if( pItemSkill->ItemInfo->SkillTargetType == SKILLTARGETTYPE_ONE &&
//		g_pShuttleChild->m_pSkill->IsExistTargetOneSkill())
//	{	// �ٸ� Ÿ���� ��ų�� �ߵ����Դϴ�		
//		return FALSE;
//	}
//	
//	if( pItemSkill->ItemInfo->ReqSP > g_pShuttleChild->m_myShuttleInfo.CurrentSP )
//	{	// ��ų����Ʈ�� �����մϴ�		
//		return FALSE;
//	}
//
//	if(pItemSkill->ItemInfo->SkillType != SKILLTYPE_PERMANENT)
//	{	// �ð��� ��ų ���� : �ð��� ��ų�� ��� ReattackTime�� ������ ������ �����ϴ�.
//		vector<CSkillInfo*>::iterator itvecSkillInfo = g_pShuttleChild->m_pSkill->m_vecUsingSkill.begin();
//		while(itvecSkillInfo != g_pShuttleChild->m_pSkill->m_vecUsingSkill.end())
//		{
//			if((*itvecSkillInfo)->ItemNum == pItemSkill->ItemNum) 
//			{
//				if((*itvecSkillInfo)->IsEnableStateForReUsing())
//				{					
//					(*itvecSkillInfo)->ReUsingSkill();
//				}
//				else
//				{
//					// g_pD3dApp->m_pChat->CreateChatChild("��ߵ� �ð��� ���ҽ��ϴ�.",COLOR_SKILL_CANCEL);								
//				}
//				return FALSE;
//			}
//			itvecSkillInfo++;
//		}
//	}

	return TRUE;
}
BOOL CINFGameMainQSlot::IsQSlotShowTime(ITEM *ItemInfo)
{
	// 2009-04-21 by bhsohn ������ DesParam�߰�
	//if(IS_EXIST_DES_PARAM(ItemInfo, DES_SKILL_HALLUCINATION))
	if(ItemInfo->IsExistDesParam(DES_SKILL_HALLUCINATION))
		return TRUE;
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFGameMainQSlot::StartReattackTime(ITEM *pItem)
/// \brief		�ð����� �������� ����ð��� �Լ��� ������������ �����Ѵ�.
/// \author		dgwoo
/// \date		2007-06-15 ~ 2007-06-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainQSlot::StartReattackTime(ITEM *pItem)
{
	CItemInfo * pItemInfo = g_pStoreData->FindItemInInventoryByItemNum(pItem->ItemNum);
	if(pItemInfo)
	{
		pItemInfo->UseItem();
	}
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFGameMainQSlot::RenderUsableReAttackTime(int nItemNum, int nRenderIndex)
/// \brief		�ð����� ������ ���� �����Կ� ����Ҽ��ִ� �����ð��� �����ش�.
/// \author		dgwoo
/// \date		2007-06-15 ~ 2007-06-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainQSlot::RenderItemUsableReAttackTime(int nItemNum, int nRenderIndex, int nLine /* =0 */, float fsizeX /* = 0.0f */, float fsizeY /* = 0.0f */)
{
	CItemInfo * pItemInfo = g_pStoreData->FindItemInInventoryByItemNum(nItemNum);
	float fElapsedTime = g_pD3dApp->GetElapsedTime();
	int nRemainedReattackTime = pItemInfo->TickReUsable(fElapsedTime);
	// 2007-06-21 by dgwoo (ms)�̱⶧���� �ʷ� ǥ���ҷ��� 1000�� ������.
	nRemainedReattackTime = (int)(nRemainedReattackTime/1000);
	
	if(nRemainedReattackTime > 0)
	{
		// �ߵ� ��ų�� ���� ���� �ð� ǥ��
		char strRemainedTime[32];
		BOOL bIsMinute = TRUE;
		int nOffSet;
		
		//nRemainedReattackTime++; // 2005-12-10 by ispark, 0�ʰ� ǥ�� �ǹǷ� 1�� ����
		
		if(nRemainedReattackTime >= 5940)
		{// ������ 99������ ǥ�� 60*99 = 5940
			nOffSet = -3;	// -1
			nRemainedReattackTime = 99;
		}
		else if(nRemainedReattackTime >= 60)
		{// ������ ǥ��
			nRemainedReattackTime /= 60;
			//nRemainedReattackTime++;			// 2006-12-12 by dgwoo 2�п��� 59�ʷ� �Ѿ�� �κ�.
		}
		else
		{// �ʷ� ǥ��
			bIsMinute = FALSE;
		}

		if(nRemainedReattackTime >= 10)
		{
			nOffSet = -3;	// -1
		}
		else
		{
			nOffSet = 0;	// 2
		}

		if(nRemainedReattackTime >= 0)
		{
			if(bIsMinute)
			{
				wsprintf(strRemainedTime, STRMSG_C_SKILL_0009, nRemainedReattackTime);
				m_vecFontLine[nRenderIndex]->DrawText(m_nX + QSLOT_ICON_INTERVAL * nRenderIndex + FONTLINE_X,
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
														m_nY + FONTLINE_Y - nLine * (fsizeY + 14),
#else		
														m_nY + FONTLINE_Y,
#endif
														D3DCOLOR_ARGB(0,0,255,255),
														strRemainedTime, 0L);
			}
			else
			{
				wsprintf(strRemainedTime, STRMSG_C_SKILL_0010, nRemainedReattackTime);//"%d��"
				m_vecFontLine[nRenderIndex]->DrawText(m_nX + QSLOT_ICON_INTERVAL * nRenderIndex + FONTLINE_X,
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
														m_nY + FONTLINE_Y - nLine * (fsizeY + 14) ,	   
#else 
														m_nY + FONTLINE_Y,
#endif
														D3DCOLOR_ARGB(0,255,255,255),
														strRemainedTime, 0L);
			}
		}
	}
	
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFGameMainQSlot::RenderSkillReAttackTime(int nItemNum)
/// \brief		��ų ��ߵ� �ð� üũ
/// \author		ispark
/// \date		2005-11-22 ~ 2005-11-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainQSlot::RenderSkillReAttackTime(int nItemNum, int nRenderIndex, int nLine /* = 0 */, float fsizeX /* = 0.0f */, float fsizeY /* = 0.0f */ )
{
	float fRemainedTime, fRemainedReattackTime;

	map<int, CSkillInfo*>::iterator itSkillInfo = g_pShuttleChild->m_pSkill->m_mapSkill.find(nItemNum);
	if(itSkillInfo != g_pShuttleChild->m_pSkill->m_mapSkill.end())
	{
		if(itSkillInfo->second->ItemInfo->SkillType == SKILLTYPE_TIMELIMIT || 
			itSkillInfo->second->ItemInfo->SkillType == SKILLTYPE_CLICK ||
			itSkillInfo->second->ItemInfo->SkillType == SKILLTYPE_CHARGING)
		{
			fRemainedReattackTime = itSkillInfo->second->GetCheckReattackTime();
			fRemainedTime = itSkillInfo->second->GetCheckEnableTime();
			// 2006-12-14 by dgwoo
			//if((IsQSlotShowTime(itSkillInfo->second->ItemInfo) || fRemainedTime <= 0) &&
			if(fRemainedReattackTime > 0)
			{
				// �ߵ� ��ų�� ���� ���� �ð� ǥ��
				char strRemainedTime[32];
				BOOL bIsMinute = TRUE;
				int nOffSet;
				
				int nRemainedReattackTime = (int)fRemainedReattackTime/1000;
				nRemainedReattackTime++; // 2005-12-10 by ispark, 0�ʰ� ǥ�� �ǹǷ� 1�� ����
				
				if(nRemainedReattackTime >= 5940)
				{// ������ 99������ ǥ�� 60*99 = 5940
					nOffSet = -3;	// -1
					nRemainedReattackTime = 99;
				}
				else if(nRemainedReattackTime >= 60)
				{// ������ ǥ��
					nRemainedReattackTime /= 60;
					//nRemainedReattackTime++;			// 2006-12-12 by dgwoo 2�п��� 59�ʷ� �Ѿ�� �κ�.
				}
				else
				{// �ʷ� ǥ��
					bIsMinute = FALSE;
				}

				if(nRemainedReattackTime >= 10)
				{
					nOffSet = -3;	// -1
				}
				else
				{
					nOffSet = 0;	// 2
				}

			
				if(nRemainedReattackTime >= 0)
				{
					if(bIsMinute)
					{
						wsprintf(strRemainedTime, STRMSG_C_SKILL_0009, nRemainedReattackTime);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
						SIZE len = m_vecFontLine[nRenderIndex]->GetStringSize( strRemainedTime );

						int nFontPosX = m_nX + 7 + ( fsizeX + 3) * nRenderIndex + ( ( fsizeX - len.cx ) / 2 ) ; // ���⼭ 6�� ���� ���� �ؽ�Ʈ �����̴�.
						int nFontPosY = m_nY + 10 - nLine * (fsizeX + 14) + ( ( fsizeX - len.cy ) /2 );
						m_vecFontLine[nRenderIndex]->DrawText(	nFontPosX,
																nFontPosY,
																D3DCOLOR_ARGB(0,0,255,255),
																strRemainedTime, 0L);			  
#else	   
						m_vecFontLine[nRenderIndex]->DrawText(m_nX + QSLOT_ICON_INTERVAL * nRenderIndex + FONTLINE_X,
																m_nY + FONTLINE_Y,
																D3DCOLOR_ARGB(0,0,255,255),
																strRemainedTime, 0L);
#endif
					}
					else
					{
						wsprintf(strRemainedTime, STRMSG_C_SKILL_0010, nRemainedReattackTime);//"%d��"
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
						SIZE len = m_vecFontLine[nRenderIndex]->GetStringSize( strRemainedTime );
						
						int nFontPosX = m_nX + 7 + ( fsizeX + 3) * nRenderIndex + ( ( fsizeX - len.cx ) / 2 ) ; // ���⼭ 6�� ���� ���� �ؽ�Ʈ �����̴�.
						int nFontPosY = m_nY + 10 - nLine * (fsizeX + 14) + ( ( fsizeX - len.cy ) /2 );
						m_vecFontLine[nRenderIndex]->DrawText(	nFontPosX,
																nFontPosY,
																D3DCOLOR_ARGB(0,255,255,255),
																strRemainedTime, 0L);			  
#else 

						m_vecFontLine[nRenderIndex]->DrawText(m_nX + QSLOT_ICON_INTERVAL * nRenderIndex + FONTLINE_X,
																m_nY + FONTLINE_Y,
																D3DCOLOR_ARGB(0,255,255,255),
																strRemainedTime, 0L);
#endif
					}
				}
			}
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFGameMainQSlot::LButtonUpQuickSlot(POINT pt)
/// \brief		�����Կ��� LButtonUp���� Ȯ��.(�κ��丮�� ��ųâ�� ������ �� ���)
/// \author		ispark
/// \date		2005-12-01 ~ 2005-12-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFGameMainQSlot::LButtonUpQuickSlot(POINT pt)
{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	int TabNum = 0;
	CINFIcon* pIconInfo = ((CINFGameMain*)m_pParent)->m_pIcon;
	{
		for(int j=0;j<QSLOT_TAB_NUMBER;j++)
		{
			int nNum = ( m_nCurrentTab + TabNum )%QSLOT_TAB_NUMBER;
			for(int i=0;i< QSLOT_NUMBER;i++)
			{
				int nTempX = m_nX + 8 + ( pIconInfo->GetIconSize().x + 3) * i;
				int nTempY = m_nY + 11 - j * ( pIconInfo->GetIconSize().y + 14 );
				if( pt.y > nTempY &&
					pt.y < nTempY + pIconInfo->GetIconSize().y &&
					pt.x > nTempX &&
					pt.x < nTempX + pIconInfo->GetIconSize().x)
				{
					return TRUE;
				}				
			}
			if(m_pIsSlotOpen == TRUE)
				break;
			TabNum++;
		}
	} 
#else	
	if( pt.y > m_nY &&
		pt.y < m_nY + QSLOT_SIZE_Y &&
		pt.x > m_nX &&
		pt.x < m_nX + QSLOT_SIZE_X)
	{
		return TRUE;
	}
#endif
// 	if( pt.y > m_nY &&
// 		pt.y < m_nY + QSLOT_SIZE_Y &&
// 		pt.x > m_nX &&
// 		pt.x < m_nX + QSLOT_SIZE_X)
// 	{
// 		return TRUE;
// 	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFGameMainQSlot::SetSelectItem(INVEN_DISPLAY_INFO *pDisplayInfo)
/// \brief		������ ����
/// \author		ispark
/// \date		2006-07-27 ~ 2006-07-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainQSlot::SetSelectItem(INVEN_DISPLAY_INFO *pDisplayInfo)
{
	POINT ptIcon;
	ptIcon.x = m_nRenderMoveIconIntervalWidth;
	ptIcon.y = m_nRenderMoveIconIntervalHeight;
	m_pSelectItem.pItem = NULL;

	if(pDisplayInfo)
	{
		m_pSelectItem.pItem = pDisplayInfo->pItem;
		strcpy(m_pSelectItem.IconName, pDisplayInfo->IconName);
		strcpy(m_pSelectItem.Name, pDisplayInfo->Name);

		((CINFGameMain*)m_pParent)->SetSelectItem(&m_pSelectItem, ptIcon, ITEM_QSLOT_POS);
	}
	else
	{
		((CINFGameMain*)m_pParent)->SetSelectItem(NULL, ptIcon, ITEM_QSLOT_POS);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void RefreshQSlotInfo()
/// \brief		������ ���� ����
/// \author		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
/// \date		2008-02-22 ~ 2008-02-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainQSlot::RefreshQSlotInfo()
{
	for(int i = 0; i < QUICKTABCOUNT; i++)
	{
		for(int j = 0; j < QUICKSLOTCOUNT; j++)
		{
			CItemInfo *pItemInfo = g_pStoreData->FindItemInInventoryByItemNum(g_pSOptionCharacter->ItemNum[i][j]);
			CSkillInfo *pSkillInfo = g_pShuttleChild->m_pSkill->FindItemSkill(g_pSOptionCharacter->ItemNum[i][j]);
			if(pItemInfo)
			{
				// ������
				g_pSOptionCharacter->UniqueNumber[i][j] = pItemInfo->UniqueNumber;
			}
			else if(pSkillInfo)
			{
				// ��ų
				g_pSOptionCharacter->UniqueNumber[i][j] = pSkillInfo->UniqueNumber;
			}		 
		}
	}
	// ������ ���� ���� 
	SetAllQSlotInfo();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void RefreshQSlotInfo()
/// \brief		������ �߰�
/// \author		// 2008-11-13 by bhsohn ���̽�ƽ �۾�
/// \date		2008-11-13 ~ 2008-11-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainQSlot::AddCurrentTab()
{
	if(m_nCurrentTab<REAL_TAB_NUMBER-1)
	{//���� ����.
		m_nCurrentTab ++;
		m_nSelectSlotNumber = -1;		

		SetSelectItem(NULL);
	}
	else
	{//���� �ʱ�ȭ.
		m_nCurrentTab = 0;
		m_nSelectSlotNumber = -1;
		
		SetSelectItem(NULL);
	}
	m_nButtonState[QSLOT_BUTTON_DOWN] = BUTTON_STATE_DOWN;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-11-13 by bhsohn ���̽�ƽ �۾�
/// \date		2008-11-13 ~ 2008-11-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainQSlot::AddJoystikcQuickSlotList(int nCurrentTab, int nSlotNumber)
{
	if(g_pGameMain && g_pGameMain->IsShowOpJoystick())
	{
		// ���̽�ƽ �ɼ�â�� ���ִ�.
		return;
	}

	DBGOUT("AddJoystikcQuickSlotList [%d][%d]\n", nCurrentTab, nSlotNumber);

	structJoystikcSkillList		struJoystikcSkillList;
	memset(&struJoystikcSkillList, 0x00, sizeof(structJoystikcSkillList));

	struJoystikcSkillList.nCurrentTab = nCurrentTab;
	struJoystikcSkillList.nSlotNumber = nSlotNumber;

	m_vecJoystikcSkillList.push_back(struJoystikcSkillList);

	m_fQSlotTimer = QSLOT_TIMER;
	m_fJoystikcSkillList = QSLOT_TIMER+(QSLOT_TIMER/3.0f);

}
	
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-11-13 by bhsohn ���̽�ƽ �۾�
/// \date		2008-11-13 ~ 2008-11-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainQSlot::TickJoysticSlot()
{
	CJoyStickInput *pJoyStick = g_pD3dApp->GetJoystickControl();
	
	if(!pJoyStick && !g_pD3dApp->IsUseJoyStick())
	{
		return;
	}
	else if(m_vecJoystikcSkillList.empty())
	{
		return;
	}

	m_fJoystikcSkillList -= g_pD3dApp->GetCheckElapsedTime(); 
	
	if(m_fJoystikcSkillList > 0.0f)
	{
		return;
	}
	m_fJoystikcSkillList = QSLOT_TIMER+(QSLOT_TIMER/3.0f);

	deque<structJoystikcSkillList>::iterator itSkillList = m_vecJoystikcSkillList.begin();
	if(itSkillList == m_vecJoystikcSkillList.end())
	{
		return;
	}

	structJoystikcSkillList stTmp = (*itSkillList);

	if(IsPossibleJoysticSlot())
	{
		UseQuickSlot(stTmp.nCurrentTab, stTmp.nSlotNumber);
		
		// ���� ù ����� �����Ѵ�.
		m_vecJoystikcSkillList.pop_front();	
	}

}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-12-15 by bhsohn ��ü�� �׾�����, ����Ŭ������ ��ų �ߵ� ���� ����
/// \date		2008-11-13 ~ 2008-11-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFGameMainQSlot::IsPossibleJoysticSlot()
{
	if(//m_fQSlotTimer <= 0 &&// 2009-01-21 by bhsohn ���� ���ÿ� ������ ����
		g_pInterface->m_pCityBase &&
		g_pInterface->m_pCityBase->GetCurrentBuildingNPC() == NULL &&
		!((CINFGameMain*)m_pParent)->m_pTrade->m_bTrading && 
		!g_pD3dApp->m_bChatMode &&
		g_pShuttleChild->CheckUnitState() == FLIGHT)
	{
		return TRUE;
	}
	return FALSE;
}


// 2010. 04. 01 by ckPark ���ҽ� ���� �ý��۽� ���丮�� �����ҿ��� ������ ȸ���� �� ������ �ȵǴ� ���� �ذ�
void	CINFGameMainQSlot::SetIconName( CItemInfo* pItemInfo, char* szName )
{
	// 2013-03-13 by bhsohn ��ų ��ȣ ���� ����
// 	if( IS_SKILL_ITEM( pItemInfo->ItemInfo->Kind ) )
// 		sprintf( szName, "%08d", SKILL_BASE_NUM( pItemInfo->ItemInfo->SourceIndex ) );
	if( IS_SKILL_ITEM( pItemInfo->ItemInfo->Kind ) )
		sprintf( szName, "%08d", SKILL_BASE_SOURCEINDEX( pItemInfo->ItemInfo->SourceIndex ) );
	else
	{
		if( pItemInfo->ItemInfo->ItemNum == pItemInfo->GetShapeItemNum() )
			sprintf( szName, "%08d", pItemInfo->ItemInfo->SourceIndex );
		else
		{
			ITEM* pShapeItem = g_pDatabase->GetServerItemInfo( pItemInfo->GetShapeItemNum() );
			if( pShapeItem )
				sprintf( szName, "%08d", pShapeItem->SourceIndex );
			else
				sprintf( szName, "%08d", pItemInfo->ItemInfo->SourceIndex );
		}
	}
}

void	CINFGameMainQSlot::UpdateIconName( CItemInfo* pItemInfo )
{
	for( int i=0; i<QSLOT_TAB_NUMBER; ++i )
	{
		for( int j=0; j<QSLOT_NUMBER; ++j )
		{
			if( m_pQSlotInfo[i][j].pItem
				&& (m_pQSlotInfo[i][j].pItem->UniqueNumber == pItemInfo->UniqueNumber) )
			{
				SetIconName( pItemInfo, m_pQSlotInfo[i][j].IconName );
				break;
			}
		}
	}
}
// end 2010. 04. 01 by ckPark ���ҽ� ���� �ý��۽� ���丮�� �����ҿ��� ������ ȸ���� �� ������ �ȵǴ� ���� �ذ�.


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2012-12-21 by bhsohn [�巹�θ��] ������ ���� ��� �ȵǰ� ����
/// \date		2012-12-27 ~ 2012-12-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFGameMainQSlot::IsPossibleSetQuickSlot(int nItemNum)
{
	switch(nItemNum)
	{
	case DRAIN_MODULE_ITEMNUM_0:
	case DRAIN_MODULE_ITEMNUM_1:	// ��Ƽ �巹�� ���
	case DRAIN_MODULE_ITEMNUM_2:	// ��Ƽ �̷��� ���
		{
			return FALSE;
		}
		break;
	}
	return TRUE;
}