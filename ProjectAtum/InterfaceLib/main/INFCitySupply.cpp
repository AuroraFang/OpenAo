// INFCitySupply.cpp: implementation of the CINFCitySupply class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "INFCitySupply.h"
#include "INFCityBase.h"
#include "INFScrollbar.h"
#include "INFImage.h"
#include "D3dHanFont.h"
#include "ItemInfo.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "FieldWinSocket.h"
#include "GameDataLast.h"
#include "StoreData.h"
#include "WeaponItemInfo.h"
#include "INFWindow.h"
#include "INFGameMain.h"
#include "INFItemInfo.h"
#include "RangeTime.h"
#include "INFInven.h"
#include "INFIcon.h"
#include "AtumDatabase.h"
#include "dxutil.h"
#include "Chat.h"

#include "INFImageEx.h"						 // 2011. 10. 10 by jskim UI�ý��� ����
#include "INFImageRadioBtn.h"
#include "Interface.h"

//////////////////////////////////////////////////////////////////////////
// 2005-11-02 by ispark
// ��ũ�� �ڵ�� ������ ���ؼ� ���ܵ�

//////////////////////////////////////////////////////////////////////////

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
#define SUPPLY_SLOT_NUMBER		6
#define SUPPLY_ALL_NUMBER		6

#define SUPPLY_START_X			(CITY_BASE_NPC_BOX_START_X + 9)
#define SUPPLY_START_Y			(CITY_BASE_NPC_BOX_START_Y - SIZE_SUPPLY_WINDOW_Y + 25)

#define SUPPLY_SCROLL_START_X	(SUPPLY_START_X + 167)
#define SUPPLY_SCROLL_START_Y	(SUPPLY_START_Y + 33)
#define SUPPLY_SCROLL_END_Y		(SUPPLY_START_Y + 181)
#define SUPPLY_SCROLL_SIZE_X	11
#define SUPPLY_SCROLL_SIZE_Y	38
#define SUPPLY_UPDOWN_X			(SUPPLY_START_X + 167)
#define SUPPLY_UP_Y				(SUPPLY_START_Y + 18)
#define SUPPLY_DOWN_Y			(SUPPLY_START_Y + 184)
#define SUPPLY_SCROLL_LINE_LENGTH	(SUPPLY_DOWN_Y - SUPPLY_UP_Y - SUPPLY_SCROLL_SIZE_Y - 6)

#define SUPPLY_SLOT_START_X		(SUPPLY_START_X + 6)
#define SUPPLY_SLOT_START_Y		(SUPPLY_START_Y + 18)
#define SUPPLY_SLOT_SIZE_X		157
#define SUPPLY_SLOT_SIZE_Y		178
#define SUPPLY_SLOT_INTERVAL	38

#define SUPPLY_ITEM_SPI_X		(SUPPLY_START_X + 125)

#define SUPPLY_CASH_Y			(SUPPLY_START_Y + 206)

#define SUPPLY_OK_X				(SUPPLY_START_X + 330)
#define SUPPLY_OK_Y				(SUPPLY_START_Y + 163)
#define SUPPLY_CANCEL_X			(SUPPLY_START_X + 370)
#define SUPPLY_CANCEL_Y			(SUPPLY_START_Y + 163)

#define SUPPLY_ICON_X			(SUPPLY_START_X + 15)
#define SUPPLY_ICON_Y			(SUPPLY_START_Y - 92)

#define SUPPLY_RADIO_WH			14
#define SUPPLY_RADIO_X			(SUPPLY_START_X + 376)
#define SUPPLY_RADIO_Y			(SUPPLY_START_Y + 29)

#define SUPPLY_ALL_X			(SUPPLY_START_X + 380)
#define SUPPLY_ALL_Y			(SUPPLY_START_Y - 94)

#define SUPPLY_ALL2_X			(CITY_BASE_NPC_BOX_START_X + 318)
#define SUPPLY_ALL2_Y			(SUPPLY_ALL_Y + 38 )

#define SUPPLY_SLIDE_BAR_X		(SUPPLY_START_X + 160)
#define SUPPLY_SLIDE_BAR_Y		(SUPPLY_START_Y - 81)
#define SUPPLY_SLIDE_LENGTH		67

#define SUPPLY_RATE_X			(SUPPLY_START_X + 294)
#define SUPPLY_RATE_PRICE_X		(SUPPLY_START_X + 370)

// 2006-03-07 by ispark, �� ���� ��ġ ����
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn �±� ���� �߰�
#define SUPPLY_ITEM_SPI_Y		(SUPPLY_START_Y + 137)//253)
#define SUPPLY_RATE_Y			(SUPPLY_START_Y - 85)//35)
#define SUPPLY_RATE_PRICE_Y		(SUPPLY_START_Y - 85)//35)
#define SHOP_TEX_X				SUPPLY_START_X + 344
#define SHOP_TEX_Y				SUPPLY_START_Y + 202
#else
#define SUPPLY_ITEM_SPI_Y		(SUPPLY_START_Y + 137)//253)
#define SUPPLY_RATE_Y			(SUPPLY_START_Y - 85)//35)
#define SUPPLY_RATE_PRICE_Y		(SUPPLY_START_Y - 85)//35)
#define SHOP_TEX_X				SUPPLY_START_X + 344
#define SHOP_TEX_Y				SUPPLY_START_Y + 202
#endif	  
#else	
#define SUPPLY_SLOT_NUMBER		6
#define SUPPLY_ALL_NUMBER		6

#define SUPPLY_START_X			(CITY_BASE_NPC_BOX_START_X + 9)
#define SUPPLY_START_Y			(CITY_BASE_NPC_BOX_START_Y - SIZE_SUPPLY_WINDOW_Y + 25)

#define SUPPLY_SCROLL_START_X	(SUPPLY_START_X + 167)
#define SUPPLY_SCROLL_START_Y	(SUPPLY_START_Y + 33)
#define SUPPLY_SCROLL_END_Y		(SUPPLY_START_Y + 181)
#define SUPPLY_SCROLL_SIZE_X	11
#define SUPPLY_SCROLL_SIZE_Y	38
#define SUPPLY_UPDOWN_X			(SUPPLY_START_X + 167)
#define SUPPLY_UP_Y				(SUPPLY_START_Y + 18)
#define SUPPLY_DOWN_Y			(SUPPLY_START_Y + 184)
#define SUPPLY_SCROLL_LINE_LENGTH	(SUPPLY_DOWN_Y - SUPPLY_UP_Y - SUPPLY_SCROLL_SIZE_Y - 6)

#define SUPPLY_SLOT_START_X		(SUPPLY_START_X + 6)
#define SUPPLY_SLOT_START_Y		(SUPPLY_START_Y + 18)
#define SUPPLY_SLOT_SIZE_X		157
#define SUPPLY_SLOT_SIZE_Y		178
#define SUPPLY_SLOT_INTERVAL	36

#define SUPPLY_ITEM_SPI_X		(SUPPLY_START_X + 38)

#define SUPPLY_CASH_Y			(SUPPLY_START_Y + 206)

#define SUPPLY_OK_X				(SUPPLY_START_X + 326)
#define SUPPLY_OK_Y				(SUPPLY_START_Y + 276)
#define SUPPLY_CANCEL_X			(SUPPLY_START_X + 366)
#define SUPPLY_CANCEL_Y			(SUPPLY_START_Y + 276)

#define SUPPLY_ICON_X			(SUPPLY_START_X + 10)
#define SUPPLY_ICON_Y			(SUPPLY_START_Y + 26)

#define SUPPLY_RADIO_WH			14
#define SUPPLY_RADIO_X			(SUPPLY_START_X + 376)
#define SUPPLY_RADIO_Y			(SUPPLY_START_Y + 29)

#define SUPPLY_ALL_X			(SUPPLY_START_X + 265)
#define SUPPLY_ALL_Y			(SUPPLY_START_Y + 33)

#define SUPPLY_ALL2_X			(CITY_BASE_NPC_BOX_START_X + 318)
#define SUPPLY_ALL2_Y			(CITY_BASE_NPC_BOX_START_Y - SIZE_SUPPLY_WINDOW_Y + 21)

#define SUPPLY_SLIDE_BAR_X		(SUPPLY_START_X + 131)
#define SUPPLY_SLIDE_BAR_Y		(SUPPLY_START_Y + 35)
#define SUPPLY_SLIDE_LENGTH		67

#define SUPPLY_RATE_X			(SUPPLY_START_X + 210)
#define SUPPLY_RATE_PRICE_X		(SUPPLY_START_X + 305)

// 2006-03-07 by ispark, �� ���� ��ġ ����
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn �±� ���� �߰�
#define SUPPLY_ITEM_SPI_Y		(SUPPLY_START_Y + 251)//253)
#define SUPPLY_RATE_Y			(SUPPLY_START_Y + 33)//35)
#define SUPPLY_RATE_PRICE_Y		(SUPPLY_START_Y + 33)//35)
#define SHOP_TEX_X				SUPPLY_START_X + 344
#define SHOP_TEX_Y				SUPPLY_START_Y - 22
#else
#define SUPPLY_ITEM_SPI_Y		(SUPPLY_START_Y + 251)//253)
#define SUPPLY_RATE_Y			(SUPPLY_START_Y + 33)//35)
#define SUPPLY_RATE_PRICE_Y		(SUPPLY_START_Y + 33)//35)
#define SHOP_TEX_X				SUPPLY_START_X + 344
#define SHOP_TEX_Y				SUPPLY_START_Y - 22
#endif
#endif
//////////////////////////////////////////////////////////////////////////

enum {NONE = -1, SUPPLY, CANCEL, ALL, ALL_SUPPLY, SLIDEBAR, SLIDE};
enum {NOR, UP, DOWN};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFCitySupply::CINFCitySupply(CAtumNode* pParent, BUILDINGNPC* pBuilding)
{
	m_pParent = pParent;	// CGameMain*
	m_pGameData = NULL;
	m_pBuildingInfo = pBuilding;

	m_stSupply[0].nItemNum = 7011440;
	m_stSupply[1].nItemNum = 7011450;
	m_stSupply[2].nItemNum = 7011460;
	m_stSupply[3].nItemNum = 7011480;
	m_stSupply[4].nItemNum = 7011490;
	m_stSupply[5].nItemNum = 7011470;
	
	m_stSupply[0].nItemIndexNum = DES_HP;
	m_stSupply[1].nItemIndexNum = DES_DP;
	m_stSupply[2].nItemIndexNum = DES_SP;
	m_stSupply[3].nItemIndexNum = DES_BULLET_01;
	m_stSupply[4].nItemIndexNum = DES_BULLET_02;
	m_stSupply[5].nItemIndexNum = DES_EP;

	wsprintf(m_stSupply[0].chItemName, "HP ����");
	wsprintf(m_stSupply[1].chItemName, "���� ����");
	wsprintf(m_stSupply[2].chItemName, "SP ����");
	wsprintf(m_stSupply[3].chItemName, "�⺻źȯ ����");
	wsprintf(m_stSupply[4].chItemName, "���źȯ ����");
	wsprintf(m_stSupply[5].chItemName, "���� ����");

	// ���� ���� ����Ÿ �ʱ�ȭ
	for(int i = 0; i < SUPPLY_ALL_NUMBER; i++)
	{
		m_stSupply[i].fSupplyRate = 0.0f;
		m_stSupply[i].fCurrentSupplyRate = 0.0f;
	}

	InitItemCheck();

	m_pInfluenceTex = NULL;
}

CINFCitySupply::~CINFCitySupply()
{
	util::del(m_pInfluenceTex);
}

HRESULT CINFCitySupply::InitDeviceObjects()
{
	DataHeader	* pDataHeader;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	char buf[16];

	for(int i = 0; i < 4 ; i++)
	{
		m_pSupplyButton[i] = new CINFImageEx;
		wsprintf(buf, "oks0%d",i);
		pDataHeader = FindResource(buf);
		m_pSupplyButton[i]->InitDeviceObjects( pDataHeader ) ;

		m_pCancelButton[i] = new CINFImageEx;
		wsprintf(buf, "cans0%d",i);
		pDataHeader = FindResource(buf);
		m_pCancelButton[i]->InitDeviceObjects( pDataHeader ) ;
	}

	m_pAllButton[0] = new CINFImageEx;
	wsprintf(buf, "Mallbtn_1");
	pDataHeader = FindResource(buf);
	m_pAllButton[0]->InitDeviceObjects( pDataHeader ) ;

	m_pAllButton[1] = new CINFImageEx;
	wsprintf(buf, "Mallbtn_1");
	pDataHeader = FindResource(buf);
	m_pAllButton[1]->InitDeviceObjects( pDataHeader ) ;

	m_pAllButton[2] = new CINFImageEx;
	wsprintf(buf, "Mallbtn_2");
	pDataHeader = FindResource(buf);
	m_pAllButton[2]->InitDeviceObjects( pDataHeader ) ;

 	m_pRadioButton = new CINFImageRadioBtn;
	m_pRadioButton->InitDeviceObjects("Mallbtn_1", "Mallbtn_2");

// 	wsprintf(buf, "radio_17a");
// 	pDataHeader = FindResource(buf);
// 	m_pRadioButton->InitDeviceObjects( pDataHeader ) ;

//  	m_pSupplyBack = new CINFImageEx;
//  	wsprintf(buf, "FS_bk");
//  	pDataHeader = FindResource(buf);
//  	m_pSupplyBack->InitDeviceObjects(pDataHeader ) ;
// 
// 	m_pSupplyTitle = new CINFImageEx;
// 	wsprintf(buf, "FS_ttl");
// 	pDataHeader = FindResource(buf);
// 	m_pSupplyTitle->InitDeviceObjects(pDataHeader ) ;

	m_pSlideBar = new CINFImageEx;
	wsprintf(buf, "scrl_b");
	pDataHeader = FindResource(buf);
	m_pSlideBar->InitDeviceObjects(pDataHeader ) ;
#else	  
	char buf[16];

	for(int i = 0; i < 4 ; i++)
	{
		m_pSupplyButton[i] = new CINFImageEx;
		wsprintf(buf, "Fb_%d",i);
		pDataHeader = FindResource(buf);
		m_pSupplyButton[i]->InitDeviceObjects(pDataHeader) ;

		m_pCancelButton[i] = new CINFImageEx;
		wsprintf(buf, "shmcan0%d",i);
		pDataHeader = FindResource(buf);
		m_pCancelButton[i]->InitDeviceObjects(pDataHeader) ;
	}

	m_pAllButton[0] = new CINFImageEx;
	wsprintf(buf, "all_nor");
	pDataHeader = FindResource(buf);
	m_pAllButton[0]->InitDeviceObjects(pDataHeader) ;

	m_pAllButton[1] = new CINFImageEx;
	wsprintf(buf, "all_over");
	pDataHeader = FindResource(buf);
	m_pAllButton[1]->InitDeviceObjects(pDataHeader) ;

	m_pAllButton[2] = new CINFImageEx;
	wsprintf(buf, "all_push");
	pDataHeader = FindResource(buf);
	m_pAllButton[2]->InitDeviceObjects(pDataHeader) ;

	m_pRadioButton = new CINFImageEx;
	wsprintf(buf, "radio_17a");
	pDataHeader = FindResource(buf);
	m_pRadioButton->InitDeviceObjects(pDataHeader) ;

	m_pSupplyBack = new CINFImageEx;
	wsprintf(buf, "FS_bk");
	pDataHeader = FindResource(buf);
	m_pSupplyBack->InitDeviceObjects(pDataHeader) ;

	m_pSupplyTitle = new CINFImageEx;
	wsprintf(buf, "FS_ttl");
	pDataHeader = FindResource(buf);
	m_pSupplyTitle->InitDeviceObjects(pDataHeader) ;

	m_pSlideBar = new CINFImageEx;
	wsprintf(buf, "scrl_b");
	pDataHeader = FindResource(buf);
	m_pSlideBar->InitDeviceObjects(pDataHeader) ;

#endif

//	m_pClose = new CINFImage;
//	wsprintf(buf, "xclose");
//	pDataHeader = FindResource(buf);
//	m_pClose->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize) ;
	
	m_pFontItemName = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,256,32);
	m_pFontItemName->InitDeviceObjects(g_pD3dDev);

	m_pInfluenceTex = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  TRUE,256,32);
	m_pInfluenceTex->InitDeviceObjects(g_pD3dDev);

	return S_OK;
}

HRESULT CINFCitySupply::RestoreDeviceObjects()
{
	int i;
	for(i = 0; i < 4; i++)
	{
		m_pSupplyButton[i]->RestoreDeviceObjects();
		m_pCancelButton[i]->RestoreDeviceObjects();
	}

	for(i = 0; i < 3; i++)
	{
		m_pAllButton[i]->RestoreDeviceObjects();
	}

	m_pRadioButton->RestoreDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pRadioButton->SetPosition( SUPPLY_ALL_X, SUPPLY_ALL_Y + SUPPLY_SLOT_INTERVAL * 6 - 2, 60 );
#else
	m_pSupplyBack->RestoreDeviceObjects();
	m_pSupplyBack->Move(SUPPLY_START_X, SUPPLY_START_Y);
	m_pSupplyTitle->RestoreDeviceObjects();
// 	m_pSupplyTitle->Move(CITY_BASE_NPC_BOX_START_X + 10, CITY_BASE_NPC_BOX_START_Y - SIZE_SUPPLY_WINDOW_Y + 6);
#endif

	m_pSlideBar->RestoreDeviceObjects();
//	m_pClose->RestoreDeviceObjects();
//	m_pClose->Move(CITY_BASE_NPC_BOX_START_X + 399, CITY_BASE_NPC_BOX_START_Y - SIZE_SUPPLY_WINDOW_Y + 6);
	m_pFontItemName->RestoreDeviceObjects();

	m_pInfluenceTex->RestoreDeviceObjects();

	return S_OK;
}

HRESULT CINFCitySupply::DeleteDeviceObjects()
{
	int i;
	for(i = 0; i < 4; i++)
	{
		m_pSupplyButton[i]->DeleteDeviceObjects();
		util::del(m_pSupplyButton[i]);
		m_pCancelButton[i]->DeleteDeviceObjects();
		util::del(m_pCancelButton[i]);
	}

	for(i = 0; i < 3; i++)
	{
		m_pAllButton[i]->DeleteDeviceObjects();
	}

	m_pRadioButton->DeleteDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	util::del( m_pRadioButton );
#else
	
	m_pSupplyBack->DeleteDeviceObjects();
	util::del(m_pSupplyBack);
	m_pSupplyTitle->DeleteDeviceObjects();
	util::del(m_pSupplyTitle);
#endif
	m_pSlideBar->DeleteDeviceObjects();
	util::del(m_pSlideBar);
//	m_pClose->DeleteDeviceObjects();
//	util::del(m_pClose);
	m_pFontItemName->DeleteDeviceObjects();
	util::del(m_pFontItemName);
	
	m_pInfluenceTex->DeleteDeviceObjects();
	util::del(m_pInfluenceTex);

	return S_OK;
}

HRESULT CINFCitySupply::InvalidateDeviceObjects()
{
	int i;
	for(i = 0; i < 4; i++)
	{
		m_pSupplyButton[i]->InvalidateDeviceObjects();
		m_pCancelButton[i]->InvalidateDeviceObjects();
	}

	for(i = 0; i < 3; i++)
	{
		m_pAllButton[i]->InvalidateDeviceObjects();
	}
	
	m_pRadioButton->InvalidateDeviceObjects();

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pSupplyBack->InvalidateDeviceObjects();
	m_pSupplyTitle->InvalidateDeviceObjects();
#endif
	m_pSlideBar->InvalidateDeviceObjects();
// 	m_pClose->InvalidateDeviceObjects();
	m_pFontItemName->InvalidateDeviceObjects();

	m_pInfluenceTex->InvalidateDeviceObjects();

	return S_OK;
}

void CINFCitySupply::Render()
{
	m_nTotalPrice = 0;

	char strtemp1[64];
	char strtemp2[64];

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pSupplyBack->Render();
	m_pSupplyTitle->Render();
#endif
//	m_pClose->Render();
	
	// ������ 
	int nStartIndex = (int)(m_ptScroll.y - SUPPLY_SCROLL_START_Y) / m_nItemLength;
	if(nStartIndex < 0)
		nStartIndex = 0;

	for(int i = 0; i < SUPPLY_SLOT_NUMBER; i++)
	{
		if(SUPPLY_ALL_NUMBER - (nStartIndex + i) > 0)
		{
			// ������
			ITEM* pItem = g_pDatabase->GetServerItemInfo(m_stSupply[nStartIndex + i].nItemNum);
			if(pItem)
			{
				// ������
				char buf[10];
				wsprintf(buf, "%08d", pItem->SourceIndex);
				g_pGameMain->m_pIcon->SetIcon(buf, SUPPLY_ICON_X, SUPPLY_ICON_Y + SUPPLY_SLOT_INTERVAL * i, 1.0f);
				g_pGameMain->m_pIcon->Render();
			}

			// SLIDE BAR
			m_pSlideBar->Move(SUPPLY_SLIDE_BAR_X + m_stSupply[i].nSlideMoveRate, 
								SUPPLY_SLIDE_BAR_Y + SUPPLY_SLOT_INTERVAL * i);
			m_pSlideBar->Render();

			// ALL ��ư
			m_pAllButton[m_stSupply[nStartIndex + i].nAllBtState]->Move(SUPPLY_ALL_X, SUPPLY_ALL_Y + SUPPLY_SLOT_INTERVAL * i);
			m_pAllButton[m_stSupply[nStartIndex + i].nAllBtState]->Render();

			// ALL Check
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
			if(m_stSupply[nStartIndex + i].bCheck)
			{
				m_pRadioButton->Move(SUPPLY_RADIO_X, SUPPLY_RADIO_Y + SUPPLY_SLOT_INTERVAL * i);
				m_pRadioButton->Render();
			}
#endif

			//////////////////////////////////////////////////////////////////////////
			// ���
			DrawSupplyPrice(nStartIndex + i);
		}
	}

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����	
	m_pRadioButton->Render();
#endif


	// ALL ��ü
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	if(m_bAllSupply)
	{
		m_pRadioButton->Move(SUPPLY_RADIO_X, SUPPLY_ALL2_Y + 1);
		m_pRadioButton->Render();
	}
#endif

	// SPI
	int nItemSpi;
	nItemSpi = g_pGameMain->m_pInven->GetItemSpi();
	wsprintf(strtemp1, "%d", nItemSpi);
	MakeCurrencySeparator( strtemp2, strtemp1, 3, ',' );
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	SIZE sSize = m_pFontItemName->GetStringSize( strtemp2 );
	m_pFontItemName->DrawText(SUPPLY_ITEM_SPI_X - sSize.cx,	
#else														
	m_pFontItemName->DrawText(SUPPLY_ITEM_SPI_X,
#endif
							  SUPPLY_ITEM_SPI_Y, 
							  GUI_FONT_COLOR, strtemp2, 0L);


	// �� ����
	wsprintf(strtemp1, "%d", m_nTotalPrice);
	MakeCurrencySeparator( strtemp2, strtemp1, 3, ',' );
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	sSize = m_pFontItemName->GetStringSize( strtemp2 );
	m_pFontItemName->DrawText(SUPPLY_START_X + 370- sSize.cx ,									  
#else				 
	m_pFontItemName->DrawText(SUPPLY_START_X + 287,
#endif
							  SUPPLY_ITEM_SPI_Y, 
							  GUI_FONT_COLOR, strtemp2, 0L);
	

	// ����, ��� ��ư
	m_pSupplyButton[m_nSupplyButtonState]->Move(SUPPLY_OK_X, SUPPLY_OK_Y);
	m_pSupplyButton[m_nSupplyButtonState]->Render();
	m_pCancelButton[m_nCancelButtonState]->Move(SUPPLY_CANCEL_X, SUPPLY_CANCEL_Y);
	m_pCancelButton[m_nCancelButtonState]->Render();

	// 2006-02-08 by ispark, ���� ����
	char chTexbuf[30] = {0,};
	sprintf(chTexbuf, STRMSG_C_060208_0000, m_fTexRate);
	m_pInfluenceTex->DrawText(SHOP_TEX_X, SHOP_TEX_Y, GUI_FONT_COLOR, chTexbuf, 0L);
}

void CINFCitySupply::Tick()
{

}

int CINFCitySupply::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
			m_nSupplyButtonState = 3;
#else 
			m_nSupplyButtonState = 0;

#endif
			m_nCancelButtonState = 3;

			if(pt.x >= SUPPLY_OK_X && pt.x <= SUPPLY_OK_X + 38 &&
				pt.y >= SUPPLY_OK_Y && pt.y <= SUPPLY_OK_Y + 17)
			{
				// ����
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
				m_nSupplyButtonState = 0;
#else									 
				m_nSupplyButtonState = 2;
#endif
				return INF_MSGPROC_BREAK;
			}
			else if(pt.x >= SUPPLY_CANCEL_X && pt.x <= SUPPLY_CANCEL_X + 38 &&
				pt.y >= SUPPLY_CANCEL_Y && pt.y <= SUPPLY_CANCEL_Y + 17)
			{
				// ���
				m_nCancelButtonState = 0;
				return INF_MSGPROC_BREAK;
			}

			// ALL ����
			int i;
			for(i = 0; i < SUPPLY_SLOT_NUMBER; i++)
			{
				if(m_stSupply[i].nAllBtState != DOWN)
					m_stSupply[i].nAllBtState = NOR;
				
				if(pt.x >= SUPPLY_ALL_X && pt.x <= SUPPLY_ALL_X + 38)
				{
					if(pt.y >= SUPPLY_ALL_Y + (SUPPLY_SLOT_INTERVAL * i) && 
						pt.y <= SUPPLY_ALL_Y + (SUPPLY_SLOT_INTERVAL * i) + 17 &&
						m_stSupply[i].nAllBtState != DOWN)
					{
						m_stSupply[i].nAllBtState = UP;
						return INF_MSGPROC_BREAK;
					}
				}
			}
			
			for(i = 0; i < SUPPLY_SLOT_NUMBER; i++)
			{
				if(m_stSupply[i].bSlideBar)
				{
//					if(pt.x >= SUPPLY_SLIDE_BAR_X - 2 && 
//						pt.x <= SUPPLY_SLIDE_BAR_X + SUPPLY_SLIDE_LENGTH + 2 &&
					if(pt.y >= SUPPLY_SLIDE_BAR_Y + (SUPPLY_SLOT_INTERVAL * i) - 4 && 
						pt.y <= SUPPLY_SLIDE_BAR_Y + (SUPPLY_SLOT_INTERVAL * i) + 15)
					{
						int nMove = pt.x - m_ptTempSlideBar.x;
						m_ptTempSlideBar = pt;

						if(pt.x <= SUPPLY_SLIDE_BAR_X)
						{
							m_stSupply[i].nSlideMoveRate = 0;
							m_ptTempSlideBar.x = SUPPLY_SLIDE_BAR_X;
						}
						else if(pt.x >= SUPPLY_SLIDE_BAR_X + SUPPLY_SLIDE_LENGTH)
						{
							m_stSupply[i].nSlideMoveRate = SUPPLY_SLIDE_LENGTH;
							m_ptTempSlideBar.x = SUPPLY_SLIDE_BAR_X + SUPPLY_SLIDE_LENGTH;
						}
						else
						{
							m_stSupply[i].nSlideMoveRate += nMove;
						}

						// ��ü ������ ���� ��� ��Ұ� ��
						if(m_stSupply[i].bCheck == TRUE)
						{
							// ���
							m_stSupply[i].nAllBtState = UP;
							m_stSupply[i].bCheck = FALSE;
							m_bAllSupply = FALSE;				// ��ü ���� ���
						}

						return INF_MSGPROC_BREAK;
					}
				}
			}
// 2005-11-02 by ispark
// ��ũ�� �ڵ�� ������ ���ؼ� ���ܵ�
//			if(m_bLButtonDown && m_bScrollLock)
//			{
//				// ��ũ�ѹٷ� �̵���
//				m_ptScroll.y += pt.y - m_ptTempScroll.y;
//				m_ptTempScroll = pt;
//
//				if(m_ptScroll.y <= SUPPLY_SCROLL_START_Y)
//				{
//					m_ptScroll.y = SUPPLY_SCROLL_START_Y;
//					return INF_MSGPROC_BREAK;
//				}
//				else if(m_ptScroll.y >= SUPPLY_DOWN_Y - SUPPLY_SCROLL_SIZE_Y - 3)
//				{
//					m_ptScroll.y = SUPPLY_DOWN_Y - SUPPLY_SCROLL_SIZE_Y - 3;
//					return INF_MSGPROC_BREAK;
//				}
//				
//				return INF_MSGPROC_BREAK;
//			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			m_bScrollLock = FALSE;
			m_nLButtonDownState = NONE;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����			
			if(m_pRadioButton->OnLButtonDown(pt))
			{

			}
#endif

			if(pt.x >= SUPPLY_OK_X && pt.x <= SUPPLY_OK_X + 38 &&
				pt.y >= SUPPLY_OK_Y && pt.y <= SUPPLY_OK_Y + 17)
			{
				// ����
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
				m_nSupplyButtonState = 1;														  
#else
				m_nSupplyButtonState = 3;
#endif
				m_bLButtonDown = TRUE;
				m_nLButtonDownState = SUPPLY;
				return INF_MSGPROC_BREAK;
			}
			else if(pt.x >= SUPPLY_CANCEL_X && pt.x <= SUPPLY_CANCEL_X + 38 &&
				pt.y >= SUPPLY_CANCEL_Y && pt.y <= SUPPLY_CANCEL_Y + 17)
			{
				// ���
				m_nCancelButtonState = 1;
				m_bLButtonDown = TRUE;
				m_nLButtonDownState = CANCEL;
				return INF_MSGPROC_BREAK;
			}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
			else if(pt.x >= SUPPLY_ALL2_X && pt.x <= SUPPLY_ALL2_X + 88 &&
					pt.y >= SUPPLY_ALL2_Y && pt.y <= SUPPLY_ALL2_Y + 15)
			{
				// ALL ��ü
				m_bLButtonDown = TRUE;
				m_nLButtonDownState = ALL_SUPPLY;
			}
#endif
			else if(pt.x >= CITY_BASE_NPC_BOX_START_X + 399 && pt.x <= CITY_BASE_NPC_BOX_START_X + 409 &&
				pt.y >= CITY_BASE_NPC_BOX_START_Y - SIZE_SUPPLY_WINDOW_Y + 6 &&
				pt.y <= CITY_BASE_NPC_BOX_START_Y - SIZE_SUPPLY_WINDOW_Y + 15)
			{
				// Close
				return INF_MSGPROC_BREAK;				
			}

			if(pt.x >= SUPPLY_ALL_X && pt.x <= SUPPLY_ALL_X + 38)
			{			
				for(int i = 0; i < SUPPLY_SLOT_NUMBER; i++)
				{
					if(pt.y >= SUPPLY_ALL_Y + (SUPPLY_SLOT_INTERVAL * i) && 
						pt.y <= SUPPLY_ALL_Y + (SUPPLY_SLOT_INTERVAL * i) + 17)
					{
						// ALL ��ư�� �Ѱ��� ���� �� �ִٰ� �����Ͽ� ��
						m_stSupply[i].nAllBtState = DOWN;
						m_bLButtonDown = TRUE;
						m_nLButtonDownState = ALL;
						return INF_MSGPROC_BREAK;
					}
				}
			}

			for(int i = 0; i < SUPPLY_SLOT_NUMBER; i++)
			{
				// �����̵� ��
				if(pt.x >= SUPPLY_SLIDE_BAR_X + m_stSupply[i].nSlideMoveRate && 
					pt.x <= SUPPLY_SLIDE_BAR_X + m_stSupply[i].nSlideMoveRate + 6)
				{
					if(pt.y >= SUPPLY_SLIDE_BAR_Y + (SUPPLY_SLOT_INTERVAL * i) && 
						pt.y <= SUPPLY_SLIDE_BAR_Y + (SUPPLY_SLOT_INTERVAL * i) + 11)
					{
						// �����̵� �� �Ѱ��� ���� �� �ִٰ� �����Ͽ� ��
						m_ptTempSlideBar = pt;
						m_bLButtonDown = TRUE;
						m_stSupply[i].bSlideBar = TRUE;
						m_nLButtonDownState = SLIDEBAR;
						return INF_MSGPROC_BREAK;
					}
				}
				else if(pt.x >= SUPPLY_SLIDE_BAR_X && pt.x <= SUPPLY_SLIDE_BAR_X + SUPPLY_SLIDE_LENGTH &&
						pt.y >= SUPPLY_SLIDE_BAR_Y + (SUPPLY_SLOT_INTERVAL * i) &&
						pt.y <= SUPPLY_SLIDE_BAR_Y + (SUPPLY_SLOT_INTERVAL * i) + 11)
				{
					// �����̵�
					m_bLButtonDown = TRUE;
					m_nLButtonDownState = SLIDE;
				}
			}
// 2005-11-02 by ispark
// ��ũ�� �ڵ�� ������ ���ؼ� ���ܵ�
//			else if(pt.x >= SUPPLY_UPDOWN_X && pt.x <= SUPPLY_UPDOWN_X + 12 &&
//					pt.y >= SUPPLY_UP_Y && pt.y <= SUPPLY_UP_Y + 12)
//			{
//				// UP
//				m_bLButtonDown = TRUE;
//				m_ptScroll.y -= m_nItemLength;
//				if(m_ptScroll.y <= SUPPLY_SCROLL_START_Y)
//				{
//					m_ptScroll.y = SUPPLY_SCROLL_START_Y;
//				}
//				return INF_MSGPROC_BREAK;
//			}
//			else if(pt.x >= SUPPLY_UPDOWN_X && pt.x <= SUPPLY_UPDOWN_X + 12 &&
//					pt.y >= SUPPLY_DOWN_Y && pt.y <= SUPPLY_DOWN_Y + 12)
//			{
//				// DOWN
//				m_bLButtonDown = TRUE;
//				m_ptScroll.y += m_nItemLength;
//				if(m_ptScroll.y >= SUPPLY_DOWN_Y - SUPPLY_SCROLL_SIZE_Y - 3)
//				{
//					m_ptScroll.y = SUPPLY_DOWN_Y - SUPPLY_SCROLL_SIZE_Y - 3;
//				}
//				return INF_MSGPROC_BREAK;
//			}
//			else if(pt.x >= SUPPLY_START_X && pt.x <= SUPPLY_START_X + 164 &&
//				pt.y >= SUPPLY_START_Y && pt.y <= SUPPLY_START_Y + 201)
//			{
//				// �簢�����ȿ� ������
//				int nStartIndex = (int)(m_ptScroll.y - SUPPLY_SCROLL_START_Y) / m_nItemLength;
//				for(int i = 0; i < 5; i ++)
//				{
//					if(SUPPLY_ALL_NUMBER - (nStartIndex + i) > 0)
//					{
//						if(pt.x >= SUPPLY_RADIO_X && pt.x <= SUPPLY_RADIO_X + SUPPLY_RADIO_WH &&
//							pt.y >= SUPPLY_RADIO_Y + SUPPLY_SLOT_INTERVAL * i && 
//							pt.y <= SUPPLY_RADIO_Y + SUPPLY_SLOT_INTERVAL * i + SUPPLY_RADIO_WH)
//						{
//							// ���� ��ư�� ������
//							if(nStartIndex + i == 0)
//							{
//								// ��� ���� ��ư�� ������
//								BOOL bCheck = m_stSupply[0].bCheck;
//								for(int j = 0; j < SUPPLY_ALL_NUMBER; j++)
//								{
//									if(bCheck == FALSE)
//										m_stSupply[j].bCheck = TRUE;
//									else
//										m_stSupply[j].bCheck = FALSE;
//								}
//							}
//							else
//							{
//								// ��Ÿ �ϳ��� ������ ������
//								m_stSupply[nStartIndex + i].bCheck = !m_stSupply[nStartIndex + i].bCheck;
//
//								// ��� ���� ��ư�� ������ �ִٸ� üũ ���
//								if(m_stSupply[0].bCheck == TRUE)
//									m_stSupply[0].bCheck = FALSE;
//							}
//						}
//					}
//				}
//				m_bLButtonDown = TRUE;
//				return INF_MSGPROC_BREAK;
//			}
//			else if(pt.x >= m_ptScroll.x && pt.x <= m_ptScroll.x + SUPPLY_SCROLL_SIZE_X &&
//				pt.y >= m_ptScroll.y && pt.y <= m_ptScroll.y + SUPPLY_SCROLL_SIZE_Y)
//			{
//				// ��ũ�ѹٸ� ������
//				m_bScrollLock = TRUE;
//				m_bLButtonDown = TRUE;
//				m_ptTempScroll = pt;
//				return INF_MSGPROC_BREAK;
// 			}

			// �簢 ������ ������ Ŭ�� ����
			if(pt.x >= CITY_BASE_NPC_BOX_START_X && pt.x <= CITY_BASE_NPC_BOX_START_X + 423 &&
				pt.y >= CITY_BASE_NPC_BOX_START_Y - SIZE_SUPPLY_WINDOW_Y && pt.y <= CITY_BASE_NPC_BOX_START_Y - SIZE_SUPPLY_WINDOW_Y + 336)
			{
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	case WM_MOUSEWHEEL:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);

// 2005-11-02 by ispark
// ��ũ�� �ڵ�� ������ ���ؼ� ���ܵ�
//			if(m_bScrollLock)
//			{
//				// ��ũ�� ���� �ɷ� �ִٸ� ���콺 ���� ����
//				if((int)wParam<0)
//				{
//					m_ptScroll.y += m_nItemLength;
//					if(m_ptScroll.y >= SUPPLY_DOWN_Y - SUPPLY_SCROLL_SIZE_Y - 3)
//					{
//						m_ptScroll.y = SUPPLY_DOWN_Y - SUPPLY_SCROLL_SIZE_Y - 3;
//					}
//				}
//				else
//				{
//					m_ptScroll.y -= m_nItemLength;
//					if(m_ptScroll.y <= SUPPLY_SCROLL_START_Y)
//					{
//						m_ptScroll.y = SUPPLY_SCROLL_START_Y;
//					}
//				}
//
//				return INF_MSGPROC_BREAK;
//			}
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
			if(m_pRadioButton->OnLButtonDown(pt))
			{
				m_bLButtonDown ^= TRUE;
				m_pRadioButton->SetRadioBtn(m_bLButtonDown);
				
				for(int i = 0; i < SUPPLY_SLOT_NUMBER; i++)
				{
			if(m_bLButtonDown)
					{
						m_stSupply[i].nAllBtState = DOWN;
						m_stSupply[i].nSlideMoveRate = SUPPLY_SLIDE_LENGTH;
					}
					else
					{
						m_stSupply[i].nAllBtState = NOR;
						m_stSupply[i].nSlideMoveRate = 0;
					}
					
					m_stSupply[i].bCheck = m_bAllSupply;
				}
				return INF_MSGPROC_BREAK;
			}
#endif
			if(m_bLButtonDown)
			{
				m_bLButtonDown = FALSE;

// 2005-11-02 by ispark
// ��ũ�� �ڵ�� ������ ���ؼ� ���ܵ�
//				if(pt.x >= SUPPLY_START_X && pt.x <= SUPPLY_START_X + 164 &&
//					pt.y >= SUPPLY_START_Y && pt.y <= SUPPLY_START_Y + 201)
//				{
//					m_bScrollLock = TRUE;
// 				}
				if(pt.x >= SUPPLY_OK_X && pt.x <= SUPPLY_OK_X + 38 &&
					pt.y >= SUPPLY_OK_Y && pt.y <= SUPPLY_OK_Y + 17 &&
					m_nLButtonDownState == SUPPLY)
				{
					// ����
					m_nLButtonDownState = NONE;
					SendShopRepair();
					InitItemCheck();
					return INF_MSGPROC_BREAK;
				}
				else if(pt.x >= SUPPLY_CANCEL_X && pt.x <= SUPPLY_CANCEL_X + 38 &&
					pt.y >= SUPPLY_CANCEL_Y && pt.y <= SUPPLY_CANCEL_Y + 17 &&
					m_nLButtonDownState == CANCEL)
				{
					// ���
					m_nLButtonDownState = NONE;
					g_pInterface->m_pCityBase->OnCityNPCButtonDown(CITY_NPC_BUTTON_CLOSE);
					return INF_MSGPROC_BREAK;
				}
				else if(pt.x >= CITY_BASE_NPC_BOX_START_X + 399 && pt.x <= CITY_BASE_NPC_BOX_START_X + 409 &&
					pt.y >= CITY_BASE_NPC_BOX_START_Y - SIZE_SUPPLY_WINDOW_Y + 6 &&
					pt.y <= CITY_BASE_NPC_BOX_START_Y - SIZE_SUPPLY_WINDOW_Y + 15)
				{
					// Close
					return INF_MSGPROC_BREAK;				
				}
				else if(pt.x >= SUPPLY_ALL2_X && pt.x <= SUPPLY_ALL2_X + 88 &&
						pt.y >= SUPPLY_ALL2_Y && pt.y <= SUPPLY_ALL2_Y + 15 &&
						m_nLButtonDownState == ALL_SUPPLY)
				{
					m_nLButtonDownState = NONE;
					m_bAllSupply = !m_bAllSupply;

					// ALL ��ü
					for(int i = 0; i < SUPPLY_SLOT_NUMBER; i++)
					{
						if(m_bAllSupply)
						{
							m_stSupply[i].nAllBtState = DOWN;
							m_stSupply[i].nSlideMoveRate = SUPPLY_SLIDE_LENGTH;
						}
						else
						{
							m_stSupply[i].nAllBtState = NOR;
							m_stSupply[i].nSlideMoveRate = 0;
						}

						m_stSupply[i].bCheck = m_bAllSupply;
					}

					return INF_MSGPROC_BREAK;
				}
				
				if(pt.x >= SUPPLY_ALL_X && pt.x <= SUPPLY_ALL_X + 38 && m_nLButtonDownState == ALL)
				{
					for(int i = 0; i < SUPPLY_SLOT_NUMBER; i++)
					{
						if(pt.y >= SUPPLY_ALL_Y + SUPPLY_SLOT_INTERVAL * i && 
							pt.y <= SUPPLY_ALL_Y + 17 + SUPPLY_SLOT_INTERVAL * i)
						{
							// ALL ��ư�� �Ѱ��� ���� �� �ִٰ� �����Ͽ� ��
							m_nLButtonDownState = NONE;
							if(m_stSupply[i].bCheck == TRUE)
							{
								// ���
								m_stSupply[i].nAllBtState = UP;
								m_stSupply[i].bCheck = FALSE;
								m_stSupply[i].nSlideMoveRate = 0;
								m_bAllSupply = FALSE;				// ��ü ���
							}
							else
							{
								m_stSupply[i].bCheck = TRUE;
								m_stSupply[i].nSlideMoveRate = SUPPLY_SLIDE_LENGTH;
							}
							return INF_MSGPROC_BREAK;
						}
						else if(m_stSupply[i].nAllBtState != DOWN)
						{
							m_stSupply[i].nAllBtState = NOR;
						}
					}

					return INF_MSGPROC_BREAK;
				}

				for(int i = 0; i < SUPPLY_SLOT_NUMBER; i++)
				{
					m_stSupply[i].bSlideBar = FALSE;
					if(pt.x >= SUPPLY_SLIDE_BAR_X && pt.x <= SUPPLY_SLIDE_BAR_X + SUPPLY_SLIDE_LENGTH &&
						pt.y >= SUPPLY_SLIDE_BAR_Y + (SUPPLY_SLOT_INTERVAL * i) &&
						pt.y <= SUPPLY_SLIDE_BAR_Y + (SUPPLY_SLOT_INTERVAL * i) + 11 &&
						m_nLButtonDownState == SLIDE)
					{
						// �����̵�
						m_nLButtonDownState = NONE;
						m_stSupply[i].nSlideMoveRate = pt.x - SUPPLY_SLIDE_BAR_X;
						m_ptTempSlideBar = pt;

						return INF_MSGPROC_BREAK;
					}
				}
			}
			
			m_nLButtonDownState = NONE;
		}
		break;
	case WM_CHAR:
		{

		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

void CINFCitySupply::InitItemCheck()
{
	// ���� ���� �޴� �ʱ�ȭ
	for(int i = 0; i < SUPPLY_ALL_NUMBER; i++)
	{
		m_stSupply[i].nAllBtState = 0;
		m_stSupply[i].bCheck = FALSE;
		m_stSupply[i].bSlideBar = FALSE;
		m_stSupply[i].nSlideMoveRate = 0;
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_nSupplyButtonState = 3;																	  
#else			   
	m_nSupplyButtonState = 0;
#endif
	m_nCancelButtonState = 3;
	
	m_bLButtonDown = FALSE;
	m_bScrollLock = FALSE;
	m_bAllSupply = FALSE;
	m_bSlideBarLock = FALSE;

	m_ptScroll.x = SUPPLY_SCROLL_START_X;
	m_ptScroll.y = SUPPLY_SCROLL_START_Y;

	m_nItemLength = SUPPLY_SCROLL_LINE_LENGTH / SUPPLY_ALL_NUMBER;

	m_nTotalPrice = 0;

	m_ptTempSlideBar.x = SUPPLY_START_X; // �����̵���� ó�� ��ġ	
}

void CINFCitySupply::InitData(BOOL bSupply)
{
	if(bSupply)
	{
		m_stSupply[0].fSupplyRate = (float)g_pShuttleChild->m_myShuttleInfo.HP - (float)g_pShuttleChild->m_fNextHP;
		m_stSupply[1].fSupplyRate = (float)g_pShuttleChild->m_myShuttleInfo.DP - (float)g_pShuttleChild->m_fNextDP;
		m_stSupply[2].fSupplyRate = (float)g_pShuttleChild->m_myShuttleInfo.SP - (float)g_pShuttleChild->m_fNextSP;
		m_stSupply[5].fSupplyRate = (float)g_pShuttleChild->m_myShuttleInfo.EP - (float)g_pShuttleChild->m_fNextEP;
	}
	else
	{
		m_stSupply[0].fSupplyRate = (float)g_pShuttleChild->m_myShuttleInfo.HP - (float)g_pShuttleChild->m_myShuttleInfo.CurrentHP;
		m_stSupply[1].fSupplyRate = (float)g_pShuttleChild->m_myShuttleInfo.DP - (float)g_pShuttleChild->m_myShuttleInfo.CurrentDP;
		m_stSupply[2].fSupplyRate = (float)g_pShuttleChild->m_myShuttleInfo.SP - (float)g_pShuttleChild->m_myShuttleInfo.CurrentSP;
		m_stSupply[5].fSupplyRate = (float)g_pShuttleChild->m_myShuttleInfo.EP - (float)g_pShuttleChild->m_myShuttleInfo.CurrentEP;
	}

	m_stSupply[3].fSupplyRate = 0;
	m_stSupply[4].fSupplyRate = 0;

	// 1��
	if(g_pShuttleChild->m_pPrimaryWeapon)
	{
		// 2007-09-06 by bhsohn �����̾� ������ �Ѿ˼� 2��
		//m_stSupply[3].fSupplyRate = g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo()->Charging - g_pShuttleChild->m_pPrimaryWeapon->GetItemGeneral()->CurrentCount;
		int nMaxCharge = g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo()->Charging;

//#ifndef LANGUAGE_VIETNAM
//		if(g_pD3dApp->IsPrimiumUser())
//		{
//			nMaxCharge = (2*g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo()->Charging);
//		}
//#endif
		// 2007-10-15 by bhsohn �Ѿ� ������ �߰� ó��
		nMaxCharge = g_pGameMain->GetMaxBulletItem(g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo()->Charging);
		// end 2007-10-15 by bhsohn �Ѿ� ������ �߰� ó��

		
		m_stSupply[3].fSupplyRate = nMaxCharge - g_pShuttleChild->m_pPrimaryWeapon->GetItemGeneral()->CurrentCount;
		// end 2007-09-06 by bhsohn �����̾� ������ �Ѿ˼� 2��		

		if(m_stSupply[3].fSupplyRate <= 0)
			m_stSupply[3].fSupplyRate = 0;
	}

	// 2��
	if(g_pShuttleChild->m_pSecondaryWeapon)
	{
		// 2007-09-06 by bhsohn �����̾� ������ �Ѿ˼� 2��
		//m_stSupply[4].fSupplyRate = g_pShuttleChild->m_pSecondaryWeapon->GetRealItemInfo()->Charging - g_pShuttleChild->m_pSecondaryWeapon->GetItemGeneral()->CurrentCount;
		int nMaxCharge = g_pShuttleChild->m_pSecondaryWeapon->GetRealItemInfo()->Charging;

		// 2007-10-15 by bhsohn �Ѿ� ������ �߰� ó��
		nMaxCharge = g_pGameMain->GetMaxBulletItem(g_pShuttleChild->m_pSecondaryWeapon->GetRealItemInfo()->Charging);
		// end 2007-10-15 by bhsohn �Ѿ� ������ �߰� ó��	
//#ifndef LANGUAGE_VIETNAM
//		if(g_pD3dApp->IsPrimiumUser())
//		{
//			nMaxCharge = (2*g_pShuttleChild->m_pSecondaryWeapon->GetRealItemInfo()->Charging);
//		}
//#endif
		m_stSupply[4].fSupplyRate = nMaxCharge - g_pShuttleChild->m_pSecondaryWeapon->GetItemGeneral()->CurrentCount;
		// end 2007-09-06 by bhsohn �����̾� ������ �Ѿ˼� 2��

		if(m_stSupply[4].fSupplyRate <= 0)
			m_stSupply[4].fSupplyRate = 0;
	}
}


void CINFCitySupply::SendShopRepair()
{
	if(m_nTotalPrice >= g_pGameMain->m_pInven->GetItemSpi())
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051108_0001, COLOR_ERROR);//"SPI(����)�� �����Ͽ� ������ �� ���� �����ϴ�."
		return;
	}

	for(int i = 0; i < SUPPLY_ALL_NUMBER; i++)
	{
		if(m_stSupply[i].fCurrentSupplyRate > 0)
		{
			MSG_FC_SHOP_REQUEST_REPAIR sMsg;
			memset(&sMsg,0x00,sizeof(sMsg));
			sMsg.DesParam = m_stSupply[i].nItemIndexNum;
			sMsg.Count = m_stSupply[i].fCurrentSupplyRate;
			g_pFieldWinSocket->SendMsg(T_FC_SHOP_REQUEST_REPAIR, (char*)&sMsg, sizeof(sMsg));
		}
	}
}

void CINFCitySupply::DrawSupplyPrice(int nIndex)
{
	char strtemp1[64];
	char strtemp2[64];

	int nItemRate = 0;
	int nItemRemain = 0;
	int nItemRatePrice = 0;
	int nRealAmountToRepair = 0;

	nItemRate = m_stSupply[nIndex].nSlideMoveRate * (m_stSupply[nIndex].fSupplyRate / SUPPLY_SLIDE_LENGTH);
	nItemRemain = m_stSupply[nIndex].fSupplyRate - nItemRate;	// ���� ���� ����

	if(m_stSupply[nIndex].nItemIndexNum == DES_BULLET_01 &&
		g_pShuttleChild->m_pPrimaryWeapon)
	{
		// 1�� ���� ���
		ITEM* pItem = g_pStoreData->FindBulletItemToDB(g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo()->Caliber);
		if( pItem != NULL )
		{
			CAtumSJ::GetRepairCost(&nItemRatePrice, &nRealAmountToRepair, 
									m_stSupply[nIndex].nItemIndexNum, 
									nItemRate,
									pItem);

			if(m_stSupply[nIndex].fSupplyRate != 0)
			{
				// ������ ���� ���� ���
				int nPackCount = (((int)m_stSupply[nIndex].fSupplyRate / pItem->MinTradeQuantity) + 1) * pItem->MinTradeQuantity;
				nItemRemain = nPackCount - nRealAmountToRepair; // ���� ����
				nItemRate = nRealAmountToRepair;				// �� ����
			}
		}
	}
	else if(m_stSupply[nIndex].nItemIndexNum == DES_BULLET_02 &&
			g_pShuttleChild->m_pSecondaryWeapon)
	{
		// 2�� ���� ���
		ITEM* pItem = g_pStoreData->FindBulletItemToDB(g_pShuttleChild->m_pSecondaryWeapon->GetRealItemInfo()->Caliber);
		if( pItem != NULL )
		{
			CAtumSJ::GetRepairCost(&nItemRatePrice, &nRealAmountToRepair, 
									m_stSupply[nIndex].nItemIndexNum, 
									nItemRate,
									pItem);
			
			if(m_stSupply[nIndex].fSupplyRate != 0)
			{
				// ������ ���� ���� ���
				int nPackCount = (((int)m_stSupply[nIndex].fSupplyRate / pItem->MinTradeQuantity) + 1) * pItem->MinTradeQuantity;
				nItemRemain = nPackCount - nRealAmountToRepair; // ���� ����
				nItemRate = nRealAmountToRepair;				// �� ����
			}
		}
	}
	else if(m_stSupply[nIndex].nItemIndexNum != DES_BULLET_01 &&
		m_stSupply[nIndex].nItemIndexNum != DES_BULLET_02)
	{
		// HP, DP, SP, EP
		CAtumSJ::GetRepairCost(&nItemRatePrice, &nRealAmountToRepair, 
								m_stSupply[nIndex].nItemIndexNum, 
								nItemRate);
	}

	// ��� �� ����
	m_stSupply[nIndex].fCurrentSupplyRate = nItemRate;

	// ���� ����
	wsprintf(strtemp1, "%d", nItemRemain);

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	SIZE sSize = m_pFontItemName->GetStringSize(strtemp1);	
	MakeCurrencySeparator( strtemp2, strtemp1, 3, ',' );
	m_pFontItemName->DrawText(SUPPLY_RATE_X - sSize.cx,											  
#else							  
	MakeCurrencySeparator( strtemp2, strtemp1, 3, ',' );
	m_pFontItemName->DrawText(SUPPLY_RATE_X,
#endif
							  SUPPLY_RATE_Y + SUPPLY_SLOT_INTERVAL * (nIndex), 
							  GUI_FONT_COLOR, strtemp2, 0L);

	// ���� ���� ����
	nItemRatePrice = CAtumSJ::GetCityWarTex(nItemRatePrice, m_fTexRate)+nItemRatePrice;
	wsprintf(strtemp1, "%d", nItemRatePrice);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	sSize = m_pFontItemName->GetStringSize(strtemp1);
	MakeCurrencySeparator( strtemp2, strtemp1, 3, ',' );
	m_pFontItemName->DrawText(SUPPLY_RATE_PRICE_X - sSize.cx,									  
#else 
	MakeCurrencySeparator( strtemp2, strtemp1, 3, ',' );
	m_pFontItemName->DrawText(SUPPLY_RATE_PRICE_X,
#endif
							  SUPPLY_RATE_PRICE_Y + SUPPLY_SLOT_INTERVAL * (nIndex), 
							  GUI_FONT_COLOR, strtemp2, 0L);
	// �� ���� ���
	m_nTotalPrice += nItemRatePrice;

}
