// INFInvenItem.cpp: implementation of the CINFInvenItem class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "D3DHanFont.h"
#include "INFInvenExtend.h"
#include "INFIcon.h"
#include "StoreData.h"
#include "ItemInfo.h"
#include "ShuttleChild.h"
#include "AtumDatabase.h"
#include "Interface.h"
#include "INFCityBase.h"
#include "INFArenaScrollBar.h"
#include "INFTrade.h"
#include "Chat.h"
#include "INFWindow.h"
#include "Skill.h"
#include "INFImageBtn.h"
#include "INFCityStore.h"
#include "AtumSound.h"
#include "INFItemInfo.h"
#include "INFLuckyMachine.h"
#include "INFCityLab.h"
#include "INFCityBazaar.h"

#include "INFInvenItem.h"
// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
#include "INFOptionMachine.h"
//end 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����

#include "INFDissolution.h" // 2010-08-31 by shcho&&jskim, ���� �ý��� ����

#include "INFCharacterInfoExtend.h"			 //2011-10-06 by jhahn ��Ʈ�� ������ �ý���

#include "PetManager.h"						 //2011-10-06 by jhahn ��Ʈ�� ������ �ý���

#include "INFGroupImage.h"
#include "INFGroupManager.h"
#include "INFImageEx.h"

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define EXTEND_INVEN_SLOT_SIZE			30
#define EXTEND_INVEN_SLOT_INTERVAL		32
#define EXTEND_INVEN_ITEM_SLOT_START_X	25
#define EXTEND_INVEN_ITEM_SLOT_START_Y	254	

#define	EXTEND_INVEN_SCROLL_WIDTH	11
#define	EXTEND_INVEN_SCROLL_HEIGHT	241

#define EXTEND_INVEN_SCROLL_LINE_START_X		350
#define EXTEND_INVEN_SCROLL_LINE_START_Y		254

#define	EXTEND_INVEN_CAPS_HEIGHT	20		// ���� ������ �׵θ�

#define INVEN_SPI_START_X		135			// 2012-04-12 by mspark, �κ��丮 �ڵ� ���� - ���� 174���� 135�� ����
#define INVEN_SPI_START_Y		460
#define INVEN_WARPOINT_X		289			// 2012-04-12 by mspark, �κ��丮 �ڵ� ���� - ���� 328���� 289�� ����
#define INVEN_WARPOINT_Y		460

#define EXTEND_WEIGHT_START_X			340
#define EXTEND_WEIGHT_START_Y			237

#define INVEN_GARBAGE_START_X	72			// 2012-04-12 by mspark, �κ��丮 �ڵ� ���� - ���� 35���� 72�� ����
#define INVEN_GARBAGE_START_Y	45
#define INVEN_GARBAGE_SIZE		15

// 2012-04-12 by mspark, �κ��丮 �ڵ� ����
#define INVEN_SORT_START_X	42
#define INVEN_SORT_START_Y	45
#define INVEN_SORT_SIZE		15
// end 2012-04-12 by mspark, �κ��丮 �ڵ� ����

#else 
#define EXTEND_INVEN_SLOT_SIZE			30
#define EXTEND_INVEN_SLOT_INTERVAL		32
#define EXTEND_INVEN_ITEM_SLOT_START_X	51
#define EXTEND_INVEN_ITEM_SLOT_START_Y	30

#define	EXTEND_INVEN_SCROLL_WIDTH	11
#define	EXTEND_INVEN_SCROLL_HEIGHT	241

#define EXTEND_INVEN_SCROLL_LINE_START_X		376
#define EXTEND_INVEN_SCROLL_LINE_START_Y		30

#define	EXTEND_INVEN_CAPS_HEIGHT	20		// ���� ������ �׵θ�

#define INVEN_SPI_START_X		243
#define INVEN_SPI_START_Y		244
#define INVEN_WARPOINT_X		367
#define INVEN_WARPOINT_Y		244

#define EXTEND_WEIGHT_START_X			367
#define EXTEND_WEIGHT_START_Y			225

#define INVEN_GARBAGE_START_X	373
#define INVEN_GARBAGE_START_Y	232
#define INVEN_GARBAGE_SIZE		24

#endif
#define INVEN_SPI_WIDTH			90
#define INVEN_SPI_HEIGHT		18


CINFInvenItem::CINFInvenItem(CAtumNode* pParent)
{
	m_pParent = pParent;

	m_bShowWnd = FALSE;
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pInvenBase = NULL;
#endif
	m_pFontItemNum = NULL;
	m_ptBkPos.x = m_ptBkPos.y =0;

	m_pINFInvenScrollBar = NULL;	

	m_bMove = FALSE;	
	m_ptCommOpMouse.x = m_ptCommOpMouse.y = 0;
	
	m_pMultiItemSelImage = NULL;

	// 2010. 05. 10 by jskim �ű� ��Ű �ӽ� �߰���� - �������� ��� ������ �����۸� �����ֱ�
	m_pDisableItemImage = NULL;
	//end 2010. 05. 10 by jskim �ű� ��Ű �ӽ� �߰���� - �������� ��� ������ �����۸� �����ֱ�
	

 //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	m_pSelectPetkitItemImageHP = NULL;
	m_pSelectPetkitItemImageSheld	= NULL;
	m_pSelectPetkitItemImageSP	= NULL;

	m_pSelectPetSocketItemImage = NULL;	   
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pEqShow = NULL ;		// ����
#endif
	m_pCloseBtn = NULL;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pInvenBG = NULL;
#endif

	// 2013-02-19 by mspark, �κ��丮 ���� ��ư ���� �������� Ŭ�� ����
	m_bInvenSortBtnArea = false;
	// end 2013-02-19 by mspark, �κ��丮 ���� ��ư ���� �������� Ŭ�� ����
}

CINFInvenItem::~CINFInvenItem()
{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	util::del(m_pInvenBase);
    util::del(m_pEqShow);	
#endif
	util::del(m_pFontItemNum);
	util::del(m_pINFInvenScrollBar);
	// 2010. 05. 10 by jskim �ű� ��Ű �ӽ� �߰���� - �������� ��� ������ �����۸� �����ֱ�
	util::del(m_pDisableItemImage);
	//end 2010. 05. 10 by jskim �ű� ��Ű �ӽ� �߰���� - �������� ��� ������ �����۸� �����ֱ�
	util::del(m_pMultiItemSelImage);	
	
	util::del(m_pCloseBtn);
//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	util::del(m_pSelectPetkitItemImageHP);
	util::del(m_pSelectPetkitItemImageSheld);
	util::del(m_pSelectPetkitItemImageSP);

	util::del(m_pSelectPetSocketItemImage); 
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	util::del(m_pInvenBG);
#endif
}

HRESULT CINFInvenItem::InitDeviceObjects()
{
	DataHeader	* pDataHeader = NULL;
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	if(NULL == m_pInvenBase)
	{
 		m_pInvenBase = new CINFImageEx;
		pDataHeader = FindResource("w_wi12");
 		m_pInvenBase->InitDeviceObjects( pDataHeader ) ;	
	}
#endif
	if(NULL == m_pFontItemNum)
	{
		m_pFontItemNum = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),8, D3DFONT_ZENABLE,  TRUE,256,32);
		m_pFontItemNum->InitDeviceObjects(g_pD3dDev);
	}
	{
		char szScBk[30], szScBall[30];	
		if(NULL == m_pINFInvenScrollBar)
		{
			m_pINFInvenScrollBar = new CINFArenaScrollBar;
		}
		wsprintf(szScBk,"arescroll");
		wsprintf(szScBall,"c_scrlb");

		m_pINFInvenScrollBar->InitDeviceObjects(INVEN_Y_NUMBER, szScBall);
	}
	if(NULL == m_pMultiItemSelImage)
	{
		m_pMultiItemSelImage = new CINFImageEx;
		pDataHeader = FindResource("selicon");
		m_pMultiItemSelImage->InitDeviceObjects( pDataHeader );	
	}
	// 2010. 05. 10 by jskim �ű� ��Ű �ӽ� �߰���� - �������� ��� ������ �����۸� �����ֱ�
	if(NULL == m_pDisableItemImage)
	{
		m_pDisableItemImage = new CINFImageEx;
		pDataHeader = FindResource("LM_inven");
		m_pDisableItemImage->InitDeviceObjects( pDataHeader );	
	}
//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	if(NULL == m_pSelectPetkitItemImageHP)
	{
		m_pSelectPetkitItemImageHP = new CINFImageEx;
		pDataHeader = FindResource("PN_usehp");
		m_pSelectPetkitItemImageHP->InitDeviceObjects( pDataHeader );	
	}
	if(NULL == m_pSelectPetkitItemImageSheld)
	{
		m_pSelectPetkitItemImageSheld = new CINFImageEx;
		pDataHeader = FindResource("PN_usesh");
		m_pSelectPetkitItemImageSheld->InitDeviceObjects( pDataHeader );	
	}
	if(NULL == m_pSelectPetkitItemImageSP)
	{
		m_pSelectPetkitItemImageSP = new CINFImageEx;
		pDataHeader = FindResource("PN_usesp");
		m_pSelectPetkitItemImageSP->InitDeviceObjects( pDataHeader );	
	}
	if(NULL == m_pSelectPetSocketItemImage)
	{
		m_pSelectPetSocketItemImage = new CINFImageEx;
		pDataHeader = FindResource("PN_selsoc");
		m_pSelectPetSocketItemImage->InitDeviceObjects( pDataHeader );	
	}
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���	

	//end 2010. 05. 10 by jskim �ű� ��Ű �ӽ� �߰���� - �������� ��� ������ �����۸� �����ֱ�
	
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	{	
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];				
		wsprintf(szUpBtn, "ieg3");
		wsprintf(szDownBtn, "ieg1");
		wsprintf(szSelBtn, "ieg0");
		wsprintf(szDisBtn, "ieg2");
		if(NULL == m_pEqShow)
		{
			m_pEqShow = new CINFImageBtn;
			m_pEqShow->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
		}
		
	}
#endif
			
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
		wsprintf(szUpBtn, "close");
		wsprintf(szDownBtn, "close");
		wsprintf(szSelBtn, "close");
		wsprintf(szDisBtn, "close");															  
#else			 
		wsprintf(szUpBtn, "xclose");
		wsprintf(szDownBtn, "xclose");
		wsprintf(szSelBtn, "xclose");
		wsprintf(szDisBtn, "xclose");
#endif
		if(NULL == m_pCloseBtn)
		{
			m_pCloseBtn = new CINFImageBtn;
		}
		m_pCloseBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
		
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("inven");
	m_pInvenBG = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );		
	m_pInvenBG->InitDeviceObjects( g_pD3dApp->m_pImageList );
	m_pInvenBG->RestoreDeviceObjects();															  
#endif
	return S_OK;
}
HRESULT CINFInvenItem::RestoreDeviceObjects()
{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pInvenBase->RestoreDeviceObjects();
#endif
	m_pFontItemNum->RestoreDeviceObjects();
	
	{
		m_pINFInvenScrollBar->RestoreDeviceObjects();				
	}
	m_pMultiItemSelImage->RestoreDeviceObjects();

	// 2010. 05. 10 by jskim �ű� ��Ű �ӽ� �߰���� - �������� ��� ������ �����۸� �����ֱ�
	if(m_pDisableItemImage)
	{
		m_pDisableItemImage->RestoreDeviceObjects();
	}
	//end 2010. 05. 10 by jskim �ű� ��Ű �ӽ� �߰���� - �������� ��� ������ �����۸� �����ֱ�
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	if(m_pEqShow)
	{
		m_pEqShow->RestoreDeviceObjects();				
	}
#endif
	if(m_pCloseBtn)
	{
		m_pCloseBtn->RestoreDeviceObjects();			
	}
//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	if(m_pSelectPetkitItemImageHP)
	{
		m_pSelectPetkitItemImageHP->RestoreDeviceObjects();
	}
	if(m_pSelectPetkitItemImageSheld)
	{
		m_pSelectPetkitItemImageSheld->RestoreDeviceObjects();
	}
	if(m_pSelectPetkitItemImageSP)
	{
		m_pSelectPetkitItemImageSP->RestoreDeviceObjects();
	}
	if(m_pSelectPetSocketItemImage)
	{
		m_pSelectPetSocketItemImage->RestoreDeviceObjects();
	}
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pInvenBG->RestoreDeviceObjects();
#endif
	
	UpdateBtnPos();
		
	return S_OK;
}
HRESULT CINFInvenItem::DeleteDeviceObjects()
{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pInvenBase->DeleteDeviceObjects();
	util::del(m_pInvenBase);
#endif

	m_pFontItemNum->DeleteDeviceObjects();
	util::del(m_pFontItemNum);

	{
		m_pINFInvenScrollBar->DeleteDeviceObjects();	
		util::del(m_pINFInvenScrollBar);
	}
	m_pMultiItemSelImage->DeleteDeviceObjects();

	// 2010. 05. 10 by jskim �ű� ��Ű �ӽ� �߰���� - �������� ��� ������ �����۸� �����ֱ�
	if(m_pDisableItemImage)
	{
		m_pDisableItemImage->DeleteDeviceObjects();
	}
	//end 2010. 05. 10 by jskim �ű� ��Ű �ӽ� �߰���� - �������� ��� ������ �����۸� �����ֱ�
	util::del(m_pMultiItemSelImage );

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	if(m_pEqShow)
	{		
		m_pEqShow->DeleteDeviceObjects();
		util::del(m_pEqShow);
	}
#endif
	if(m_pCloseBtn)
	{
		m_pCloseBtn->DeleteDeviceObjects();	
		util::del(m_pCloseBtn);
	}
//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	if(m_pSelectPetkitItemImageHP)
	{
		m_pSelectPetkitItemImageHP->DeleteDeviceObjects();
		util::del(m_pSelectPetkitItemImageHP);
	}
	if(m_pSelectPetkitItemImageSheld)
	{
		m_pSelectPetkitItemImageSheld->DeleteDeviceObjects();
		util::del(m_pSelectPetkitItemImageSheld);
	}
	if(m_pSelectPetkitItemImageSP)
	{
		m_pSelectPetkitItemImageSP->DeleteDeviceObjects();
		util::del(m_pSelectPetkitItemImageSP);
	}
	if(m_pSelectPetSocketItemImage	)
	{
		m_pSelectPetSocketItemImage	->DeleteDeviceObjects();
		util::del(m_pSelectPetSocketItemImage	);
	}
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pInvenBG->DeleteDeviceObjects();
	util::del( m_pInvenBG	);		  
#endif
	return S_OK;
}
HRESULT CINFInvenItem::InvalidateDeviceObjects()
{		
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pInvenBase->InvalidateDeviceObjects();
#endif
	m_pFontItemNum->InvalidateDeviceObjects();
	{
		m_pINFInvenScrollBar->InvalidateDeviceObjects();
	}
	m_pMultiItemSelImage->InvalidateDeviceObjects();

	// 2010. 05. 10 by jskim �ű� ��Ű �ӽ� �߰���� - �������� ��� ������ �����۸� �����ֱ�
	if(m_pDisableItemImage)
	{
		m_pDisableItemImage->InvalidateDeviceObjects();
	}
	//end 2010. 05. 10 by jskim �ű� ��Ű �ӽ� �߰���� - �������� ��� ������ �����۸� �����ֱ�
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	if(m_pEqShow)
	{		
		m_pEqShow->InvalidateDeviceObjects();
	}
#endif
	if(m_pCloseBtn)
	{
		m_pCloseBtn->InvalidateDeviceObjects();		
	}
//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	if(m_pSelectPetkitItemImageHP)
	{
		m_pSelectPetkitItemImageHP->InvalidateDeviceObjects();		
	}
	if(m_pSelectPetkitItemImageSheld)
	{
		m_pSelectPetkitItemImageSheld->InvalidateDeviceObjects();		
	}
	if(m_pSelectPetkitItemImageSP)
	{
		m_pSelectPetkitItemImageSP->InvalidateDeviceObjects();		
	}
	if(m_pSelectPetSocketItemImage	)
	{
		m_pSelectPetSocketItemImage	->InvalidateDeviceObjects();		
	}
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���	
	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pInvenBG->InvalidateDeviceObjects();
#endif
	return S_OK;
}
void CINFInvenItem::Render()
{
	if(!IsShowWnd())
	{
		return;
	}
	CINFInvenExtend* pParent = (CINFInvenExtend*)m_pParent;
	
	int nWindowPosX = m_ptBkPos.x;
	int nWindowPosY = m_ptBkPos.y; 
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	{
		m_pInvenBG->Move( m_ptBkPos.x, m_ptBkPos.y );
		m_pInvenBG->Render();
	}	
#else  
	int nPosX, nPosY;
	nPosX = nPosY = 0;
	{
		nPosX = m_ptBkPos.x;
		nPosY = m_ptBkPos.y;
		m_pInvenBase->Move(nPosX, nPosY);
		m_pInvenBase->Render();
	}
#endif
	m_pINFInvenScrollBar->Render();

	RenderInvenItem();

	{
		CD3DHanFont* pFontSpi = pParent->GetFontSpi();
		if(pFontSpi)
		{
			char temp1[128];
			char temp2[128];
			SIZE size;
			
			int	nItemSpi = pParent->GetItemSpi(); // SPI��
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
			// SPI
			wsprintf( temp1, "%d", nItemSpi );
			MakeCurrencySeparator( temp2, temp1, 3, ',' );
			size = pFontSpi->GetStringSize(temp2);
			pFontSpi->DrawText(nWindowPosX + INVEN_SPI_START_X-size.cx, nWindowPosY + INVEN_SPI_START_Y, GUI_FONT_COLOR_W,temp2, 0L);
			
			// War Point
			wsprintf(temp1,"%d",g_pShuttleChild->m_myShuttleInfo.WarPoint);
			MakeCurrencySeparator(temp2,temp1,3,',');
			size = pFontSpi->GetStringSize(temp2);
			pFontSpi->DrawText(nWindowPosX + INVEN_WARPOINT_X-size.cx, nWindowPosY + INVEN_WARPOINT_Y, GUI_FONT_COLOR_W,temp2, 0L);
#else
			// SPI
			wsprintf( temp1, "%d", nItemSpi );
			MakeCurrencySeparator( temp2, temp1, 3, ',' );
			size = pFontSpi->GetStringSize(temp2);
			pFontSpi->DrawText(nWindowPosX + INVEN_SPI_START_X-size.cx, nWindowPosY + INVEN_SPI_START_Y, GUI_FONT_COLOR_BM,temp2, 0L);
			
			// War Point
			wsprintf(temp1,"%d",g_pShuttleChild->m_myShuttleInfo.WarPoint);
			MakeCurrencySeparator(temp2,temp1,3,',');
			size = pFontSpi->GetStringSize(temp2);
			pFontSpi->DrawText(nWindowPosX + INVEN_WARPOINT_X-size.cx, nWindowPosY + INVEN_WARPOINT_Y, GUI_FONT_COLOR_BM,temp2, 0L);
#endif
		}
	}
	{
		CD3DHanFont*  pFontWeight = pParent->GetFontWeight(0);
		char buff[256];
		// 2009. 11. 3 by jskim ĳ��(�κ�/â�� Ȯ��) ������ �߰� ����
		//CAtumSJ::GetMaxInventorySize((BOOL)g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1) - 1);	// 2006-06-23 by ispark, -1�� ���� �������� �����ϴ� ���̴�.
		// 2009. 12. 17 by jskim ĳ��(�κ�/â�� Ȯ��) ����� 
// 		CHARACTER* pMainInfo = g_pD3dApp->GetMFSMyShuttleInfo();
// 		wsprintf(buff, "%s %d/%d", STRMSG_C_INTERFACE_0026, 
// 				(int)(g_pStoreData->GetTotalUseInven()), 		
// 		CAtumSJ::GetMaxInventorySize((BOOL)g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1, pMainInfo->GetAddedPermanentInventoryCount()) - 1);	// 2006-06-23 by ispark, -1�� ���� �������� �����ϴ� ���̴�.
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
		wsprintf(buff, "%d/%d",								
#else
		wsprintf(buff, "%s %d/%d", STRMSG_C_INTERFACE_0026, 
#endif

				(int)(g_pStoreData->GetTotalUseInven()), 		
 		CAtumSJ::GetMaxInventorySize((BOOL)g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1, g_pShuttleChild->m_myShuttleInfo.GetAddedPermanentInventoryCount()) - 1);	// 2006-06-23 by ispark, -1�� ���� �������� �����ϴ� ���̴�.
		//end 2009. 12. 17 by jskim ĳ��(�κ�/â�� Ȯ��) ����� 
		//end 2009. 11. 3 by jskim ĳ��(�κ�/â�� Ȯ��) ������ �߰� ����
		
		SIZE size = pFontWeight->GetStringSize(buff);		
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
		pFontWeight->DrawText(nWindowPosX+EXTEND_WEIGHT_START_X-size.cx, 
								nWindowPosY+EXTEND_WEIGHT_START_Y, GUI_FONT_COLOR_W, buff, 0 );//"���緮"
#else
		pFontWeight->DrawText(nWindowPosX+EXTEND_WEIGHT_START_X-size.cx, 
								nWindowPosY+EXTEND_WEIGHT_START_Y, GUI_FONT_COLOR_BM, buff, 0 );//"���緮"		
#endif
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pEqShow->Render();	
#endif
	m_pCloseBtn->Render();

	// 2013-02-26 by bhsohn �ΰ��� ���� �˻� ó��
	if(pParent)
	{
		pParent->RenderItemMenuListWnd();
	}
	// END 2013-02-26 by bhsohn �ΰ��� ���� �˻� ó��
	

}

void CINFInvenItem::Tick()
{
	
}
BOOL CINFInvenItem::IsShowWnd()
{
	return m_bShowWnd;
}
void CINFInvenItem::ShowWnd(BOOL bShow, POINT *i_ptPos/*=NULL*/)
{
	m_bShowWnd = bShow;
	// 2009. 08. 19 by jsKim ���� �� �޴� ������ ��� Ŀ���� ������ �ʴ� ����
	if(bShow)
	{
		// 2012-12-17 by jhjang ������ ���콺 Ŀ���� ����� �������� �ʴ� ���� ����
		//g_INFCnt++;
		g_pD3dApp->AddINFCnt();
		// end 2012-12-17 by jhjang ������ ���콺 Ŀ���� ����� �������� �ʴ� ���� ����
		g_pGameMain->m_bChangeMousePoint = TRUE;
	}
	else
	{
		// 2012-12-17 by jhjang ������ ���콺 Ŀ���� ����� �������� �ʴ� ���� ����
		//if(g_INFCnt < 0)
		//	g_INFCnt--;
		//{
		//	g_INFCnt = 0;
		//}
		//if(g_INFCnt==0)
		if(g_pD3dApp->RemoveINFCnt() == 0)
		{
			g_pGameMain->m_bChangeMousePoint = FALSE;
		}
		// end 2012-12-17 by jhjang ������ ���콺 Ŀ���� ����� �������� �ʴ� ���� ����

		// 2013-02-26 by bhsohn �ΰ��� ���� �˻� ó��
		POINT pt ={0,0};
		CINFInvenExtend* pParent = (CINFInvenExtend*)m_pParent;
		pParent->OnClickItemMenuListWnd(FALSE, pt, 0, 0);
		// END 2013-02-26 by bhsohn �ΰ��� ���� �˻� ó��

	}
	// end 2009. 08. 19 by jsKim ���� �� �޴� ������ ��� Ŀ���� ������ �ʴ� ����
	
	if(i_ptPos)
	{
		m_ptBkPos = (*i_ptPos);
		UpdateBtnPos();
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-08-22 by bhsohn EP3 �κ��丮 ó��
/// \date		2008-08-22 ~ 2008-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInvenItem::RenderInvenItem()
{
	CINFInvenExtend* pParent = (CINFInvenExtend*)m_pParent;
	CINFIcon* pIconInfo = g_pGameMain->m_pIcon;
	int nPosX, nPosY;
	int nIdxPos = 0;
	nPosX = nPosY = 0;

	POINT ptBkPos = m_ptBkPos;

//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	for (int check = 0 ; check < 3 ; check++)
	{
		g_pShuttleChild->GetPetManager()->SetSelectingCheck(check,FALSE);
		g_pShuttleChild->GetPetManager()->SetSelectingCheckSocket(check,FALSE);
	}
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���			


	for(int i=0;i<INVEN_Y_NUMBER;i++)
	{
		for(int j=0;j<INVEN_X_NUMBER;j++)
		{			
			nIdxPos = i*INVEN_X_NUMBER+j;
			INVEN_DISPLAY_INFO*  pInvenDisplayInfo = pParent->GetInvenDisplayInfo(nIdxPos);
			if(pInvenDisplayInfo)
			{				
				BOOL bMultiSel = FALSE;
				if(g_pD3dApp->CheckMultItemSel(pInvenDisplayInfo->pItem->UniqueNumber))
				{
					// ���� ������ ������ ������
					bMultiSel = TRUE;
				}

				nPosX = ptBkPos.x + EXTEND_INVEN_ITEM_SLOT_START_X + EXTEND_INVEN_SLOT_INTERVAL*j + 1;
				nPosY = ptBkPos.y + EXTEND_INVEN_ITEM_SLOT_START_Y + EXTEND_INVEN_SLOT_INTERVAL*i + 1;

				char buf[64];

				// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

				//wsprintf(buf, "%08d", pInvenDisplayInfo->pItem->ItemInfo->SourceIndex);
				strcpy( buf, pInvenDisplayInfo->IconName );

				// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����


				
				pIconInfo->SetIcon(buf, 
					nPosX,
					nPosY, 1.0f);
				pIconInfo->Render();
				// ī���ͺ������ �����̱�
				if( IS_COUNTABLE_ITEM(pInvenDisplayInfo->pItem->Kind) )
				{
					CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( 
						pInvenDisplayInfo->pItem->UniqueNumber );
					if( pItemInfo->CurrentCount > 1 )
					{
						// ������ �����ش�.
						char buf[128];
						wsprintf(buf, "%d",pItemInfo->CurrentCount);
						int len = strlen(buf) - 1;			// ����� �Ѱ� �̻� ���´ٴ� ���ǿ� -1�� �ߴ�.

						
						int nFontPosX = ptBkPos.x + EXTEND_INVEN_ITEM_SLOT_START_X + EXTEND_INVEN_SLOT_INTERVAL*j + 21 - len*6; // ���⼭ 6�� ���� ���� �ؽ�Ʈ �����̴�.
						int nFontPosY = ptBkPos.y + EXTEND_INVEN_ITEM_SLOT_START_Y + EXTEND_INVEN_SLOT_INTERVAL*i - 1;
						
						
						m_pFontItemNum->DrawText(nFontPosX,nFontPosY, QSLOT_COUNTERBLE_NUMBER,buf, 0L);
					}
				}

				// 2010. 02. 11 by ckPark �ߵ��� ����������
				if( pInvenDisplayInfo->pItem->ItemInfo->InvokingDestParamID
					|| pInvenDisplayInfo->pItem->ItemInfo->InvokingDestParamIDByUse )
				{
					char buf[128];

					CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( pInvenDisplayInfo->pItem->UniqueNumber );
					if( pItemInfo && GetString_CoolTime( pItemInfo, buf ) )
					{
						int len = strlen(buf) - 1;

						int nFontPosX = ptBkPos.x + EXTEND_INVEN_ITEM_SLOT_START_X + EXTEND_INVEN_SLOT_INTERVAL * j + 21 - len * 6; // ���⼭ 6�� ���� ���� �ؽ�Ʈ �����̴�.
						int nFontPosY = ptBkPos.y + EXTEND_INVEN_ITEM_SLOT_START_Y + EXTEND_INVEN_SLOT_INTERVAL * i + 7;

						m_pFontItemNum->DrawText(nFontPosX,nFontPosY, QSLOT_COUNTERBLE_NUMBER,buf, 0L);
					}
				}
				// end 2010. 02. 11 by ckPark �ߵ��� ����������
				// 2010. 05. 10 by jskim �ű� ��Ű �ӽ� �߰���� - �������� ��� ������ �����۸� �����ֱ�

				CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( pInvenDisplayInfo->pItem->UniqueNumber );
				if(pItemInfo && g_pInterface->m_pCityBase->GetCurrentBuildingNPC())
				{
					if(pItemInfo && ShopIsDisableInvenItem(g_pInterface->m_pCityBase->GetCurrentBuildingNPC()->buildingInfo.BuildingKind, pItemInfo))
					{
						m_pDisableItemImage->Move(nPosX, nPosY);
						m_pDisableItemImage->SetScale(pIconInfo->GetIconSize().x,pIconInfo->GetIconSize().y);
						m_pDisableItemImage->Render();
					}
				}
				//end 2010. 05. 10 by jskim �ű� ��Ű �ӽ� �߰���� - �������� ��� ������ �����۸� �����ֱ�
//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
				if(pItemInfo && g_pGameMain->m_pCharacterInfo->GetPartnerState()  == CHARACTER_PARTNER && g_pGameMain->m_pCharacterInfo->IsShowWnd())
				{
					CItemInfo* pAllItem = g_pStoreData->FindItemInInventoryByWindowPos( POS_PET ); 	

					if (pAllItem)
					{
						tPET_CURRENTINFO * tempCurrentInfo = g_pShuttleChild->GetPetManager()->GetPtr_PetCurrentData( pAllItem->UniqueNumber );					
						tPET_LEVEL_DATA *psPetLevelData = g_pDatabase->GetPtr_PetLevelData ( tempCurrentInfo->PetIndex , tempCurrentInfo->PetLevel );
						
						for (int num = 0 ; num < tempCurrentInfo->PetEnableSocketCount ; num++)
						{
							if ((tempCurrentInfo->PetSocketItemUID[num] == NULL) &&
								(pItemInfo->ItemInfo->ArrDestParameter[0] == DES_PET_SOCKET_ITEM_AUTOKIT) ||
								(pItemInfo->ItemInfo->ArrDestParameter[0] == DES_PET_SOCKET_ITEM_AUTOSKILL) )
							{
								m_pSelectPetSocketItemImage->Move(nPosX- 10, nPosY- 10);
								m_pSelectPetSocketItemImage->Render();
								g_pShuttleChild->GetPetManager()->SetSelectingCheckSocket(num,TRUE);
								
							}
							
						}
						
					}
				
					if(g_pShuttleChild->GetPetManager()->GetSelectSocket() == SOKET_TYPE_ITEM && pAllItem )
					{
						tPET_CURRENTINFO * tempCurrentInfo = g_pShuttleChild->GetPetManager()->GetPtr_PetCurrentData( pAllItem->UniqueNumber );
						tPET_LEVEL_DATA *psPetLevelData = g_pDatabase->GetPtr_PetLevelData ( tempCurrentInfo->PetIndex , tempCurrentInfo->PetLevel );
					
						if ( tempCurrentInfo && 
							pItemInfo->ItemInfo->ArrDestParameter[1] == DES_PET_SLOT_ITEM_AUTOKIT_HP  &&
							psPetLevelData->KitLevelHP >= pItemInfo->ItemInfo->ArrParameterValue[1])
						{
							m_pSelectPetkitItemImageHP->Move(nPosX- 10, nPosY- 10);
							m_pSelectPetkitItemImageHP->Render();	
							g_pShuttleChild->GetPetManager()->SetSelectingCheck(0,TRUE);
						}
						
						else if (tempCurrentInfo && 
							pItemInfo->ItemInfo->ArrDestParameter[1] == DES_PET_SLOT_ITEM_AUTOKIT_SHIELD &&
							psPetLevelData->KitLevelShield >= pItemInfo->ItemInfo->ArrParameterValue[1] )
						{
							m_pSelectPetkitItemImageSheld->Move(nPosX - 10, nPosY- 10);						
							m_pSelectPetkitItemImageSheld->Render();
							g_pShuttleChild->GetPetManager()->SetSelectingCheck(1,TRUE);
						}
						
						else if (tempCurrentInfo && 
							pItemInfo->ItemInfo->ArrDestParameter[1] == DES_PET_SLOT_ITEM_AUTOKIT_SP &&
							psPetLevelData->KitLevelSP >= pItemInfo->ItemInfo->ArrParameterValue[1])
						{
							m_pSelectPetkitItemImageSP->Move(nPosX- 10, nPosY- 10);
							m_pSelectPetkitItemImageSP->Render();
							g_pShuttleChild->GetPetManager()->SetSelectingCheck(2,TRUE);
						}
					}					 
				}	
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
				if(bMultiSel)
				{					
					m_pMultiItemSelImage->Move(nPosX, nPosY);
					m_pMultiItemSelImage->Render();
					
				}
			}
		}
	}
}
int CINFInvenItem::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bShowInven)
{
	if(!IsShowWnd())
	{
		return INF_MSGPROC_NORMAL;
	}
	// 2013-02-26 by bhsohn �ΰ��� ���� �˻� ó��
	CINFInvenExtend* pParent = (CINFInvenExtend*)m_pParent;
	if(INF_MSGPROC_BREAK == pParent->WndProcItemMenuListWnd(uMsg, wParam, lParam))
	{
		return INF_MSGPROC_BREAK;
	}
	// END 2013-02-26 by bhsohn �ΰ��� ���� �˻� ó��
	switch(uMsg)
	{
	case WM_LBUTTONUP:
		{
			return OnLButtonUp(uMsg, wParam, lParam, bShowInven);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			return OnLButtonDown(uMsg, wParam, lParam, bShowInven);
		}
		break;
	case WM_MOUSEMOVE:
		{
			return OnMouseMove(uMsg, wParam, lParam, bShowInven);
		}
		break;
	case WM_MOUSEWHEEL:
		{
			return OnMouseWhell(uMsg, wParam, lParam, bShowInven);
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			return OnLButtonDB(uMsg, wParam, lParam, bShowInven);
		}
		break;
	case WM_RBUTTONDOWN:
		{
			if(!bShowInven)
			{
				// �κ��� �ƴϸ� �̿� ���Ѵ�.
				return INF_MSGPROC_NORMAL;
			}
			return OnRButtonDown(uMsg, wParam, lParam, bShowInven);
		}
		break;
	case WM_KEYDOWN:
		{	
			// 2013-02-26 by bhsohn �ΰ��� ���� �˻� ó��
// 			if(!bShowInven)
// 			{
// 				// �κ��� �ƴϸ� �̿� ���Ѵ�.
// 				return INF_MSGPROC_NORMAL;
// 			}
			// END 2013-02-26 by bhsohn �ΰ��� ���� �˻� ó��

			switch( wParam )
			{
			case VK_CONTROL:
				{
					g_pD3dApp->OnCtrlBtnClick(TRUE);					
				}
				break;
			}
			return INF_MSGPROC_NORMAL;	// 2013-02-26 by bhsohn �ΰ��� ���� �˻� ó��
		}
		break;
	case WM_KEYUP:
		{
			// 2013-02-26 by bhsohn �ΰ��� ���� �˻� ó��
// 			if(!bShowInven)
// 			{
// 				// �κ��� �ƴϸ� �̿� ���Ѵ�.
// 				return INF_MSGPROC_NORMAL;
// 			}
			// END 2013-02-26 by bhsohn �ΰ��� ���� �˻� ó��

			switch( wParam )
			{
			case VK_CONTROL:
				{					
					g_pD3dApp->OnCtrlBtnClick(FALSE);
				}
				break;
			}		
			return INF_MSGPROC_NORMAL;	// 2013-02-26 by bhsohn �ΰ��� ���� �˻� ó��
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

int CINFInvenItem::OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bShowInven)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);

	if(!IsWndRect(pt) && !m_bMove)
	{
		return INF_MSGPROC_NORMAL;
	}

	CINFInvenExtend* pParent = (CINFInvenExtend*)m_pParent;

	int nItemPosX, nItemPosY;
	nItemPosX = nItemPosY =-1;	

	// ���� ���� ó�� ����
	int nSelItemCnt = g_pD3dApp->GetMultiSelectItem();

	// ���� ���ʹ� ������ ���� 
	if(!IsInvenRect(pt, &nItemPosX, &nItemPosY))
	{
		// ������ ����.
		pParent->ClearMultiSeletItem();	// ���߼��� �ʱ�ȭ		
		return INF_MSGPROC_NORMAL;
	}	
	
	
	INVEN_DISPLAY_INFO*  pInvenDisplayInfo = pParent->GetInvenDisplayInfo(nItemPosY*INVEN_X_NUMBER+nItemPosX);
	if(NULL == pInvenDisplayInfo)
	{
		return INF_MSGPROC_NORMAL;
	}
	
	if(nSelItemCnt <= 0 )
	{
		pParent->SetMultiSelectItem(NULL);	// ���� ó�� ����							
		// �̱� ����
		pParent->SetSelectItem(pInvenDisplayInfo);
	}
	
	if(g_pGameMain->m_stSelectItem.pSelectItem &&
		g_pGameMain->m_stSelectItem.bySelectType == ITEM_INVEN_POS &&
		g_pInterface->m_pBazaarShop == NULL)
	{
		if(IS_ITEM_SHOP_TYPE(g_pInterface->m_pCityBase->GetCurrentBuildingNPC()->buildingInfo.BuildingKind))
		{
			CItemInfo* pItemInfo = (CItemInfo*)g_pGameMain->m_stSelectItem.pSelectItem->pItem;
			if(pItemInfo->Wear == WEAR_NOT_ATTACHED )
			{
				if( IS_COUNTABLE_ITEM(pItemInfo->Kind))
				{
					char buf[128];
					ITEM *pITEM = pItemInfo->GetItemInfo();
					if(pITEM)
					{
						char temp1[64];
						char temp2[64];
						wsprintf( temp1, "%d", CAtumSJ::GetItemSellingPriceAtShop(pITEM) );
						MakeCurrencySeparator( temp2, temp1, 3, ',' );
						wsprintf( buf, STRMSG_C_SHOP_0007, pITEM->ItemName, pITEM->MinTradeQuantity, temp2 );//"%s � �Ľðڽ��ϱ�?[����:%d���� %s(����)]"
						
						// 2005-05-09 by ydkim	�������� �޼����ڽ� ����
						if(g_pGameMain->m_pInfWindow->IsExistMsgBox(_Q_SHOP_SELL_ITEM))
						{
							g_pGameMain->m_pInfWindow->DeleteMsgBox(_Q_SHOP_SELL_ITEM);
						}
						else if(g_pGameMain->m_pInfWindow->IsExistMsgBox(_Q_SHOP_SELL_ENERGY))
						{
							g_pGameMain->m_pInfWindow->DeleteMsgBox(_Q_SHOP_SELL_ENERGY);
						}
						// 2007-02-12 by bhsohn Item ���� ���� ó��
						else if(g_pGameMain->m_pInfWindow->IsExistMsgBox(_Q_SHOP_MULTI_SELL_ITEM ))
						{
							g_pGameMain->m_pInfWindow->DeleteMsgBox(_Q_SHOP_MULTI_SELL_ITEM );
						}
						else if(g_pGameMain->m_pInfWindow->IsExistMsgBox(_Q_STORE_MULTI_PUT_ITEM ))
						{
							g_pGameMain->m_pInfWindow->DeleteMsgBox(_Q_STORE_MULTI_PUT_ITEM );
						}								
						else if(g_pGameMain->m_pInfWindow->IsExistMsgBox(_Q_STORE_MULTI_GET_ITEM ))
						{
							g_pGameMain->m_pInfWindow->DeleteMsgBox(_Q_STORE_MULTI_GET_ITEM );
						}																
						// end 2007-02-12 by bhsohn Item ���� ���� ó��								
						g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_SHOP_SELL_ENERGY,
							(DWORD)pItemInfo, 
							pItemInfo->CurrentCount);
					}
					else
					{
						g_pD3dApp->m_pChat->CreateChatChild( STRMSG_C_SERVER_0004, COLOR_SYSTEM );//"�����κ��� ���� �޴� ��... �ٽ� �õ� �ϼ���."
					}
				}
				else
				{
					char buf[128];
					ITEM *pITEM = pItemInfo->GetItemInfo();
					if(pITEM)
					{
						char temp1[64];
						char temp2[64];
						wsprintf( temp1, "%d", CAtumSJ::GetItemSellingPriceAtShop(pITEM) );
						MakeCurrencySeparator( temp2, temp1, 3, ',' );
						wsprintf( buf, STRMSG_C_SHOP_0009, pITEM->ItemName, temp2);//"%s �� �Ľðڽ��ϱ�?[����:%s(����)]"
						
						// 2005-05-09 by ydkim	�������� �޼����ڽ� ����
						if(g_pGameMain->m_pInfWindow->IsExistMsgBox(_Q_SHOP_SELL_ITEM))
						{
							g_pGameMain->m_pInfWindow->DeleteMsgBox(_Q_SHOP_SELL_ITEM);
						}
						else if(g_pGameMain->m_pInfWindow->IsExistMsgBox(_Q_SHOP_SELL_ENERGY))
						{
							g_pGameMain->m_pInfWindow->DeleteMsgBox(_Q_SHOP_SELL_ENERGY);
						}
						// 2007-02-12 by bhsohn Item ���� ���� ó��
						else if(g_pGameMain->m_pInfWindow->IsExistMsgBox(_Q_SHOP_MULTI_SELL_ITEM ))
						{
							g_pGameMain->m_pInfWindow->DeleteMsgBox(_Q_SHOP_MULTI_SELL_ITEM );
						}
						else if(g_pGameMain->m_pInfWindow->IsExistMsgBox(_Q_STORE_MULTI_PUT_ITEM ))
						{
							g_pGameMain->m_pInfWindow->DeleteMsgBox(_Q_STORE_MULTI_PUT_ITEM );
						}								
						else if(g_pGameMain->m_pInfWindow->IsExistMsgBox(_Q_STORE_MULTI_GET_ITEM ))
						{
							g_pGameMain->m_pInfWindow->DeleteMsgBox(_Q_STORE_MULTI_GET_ITEM );
						}																
						// end 2007-02-12 by bhsohn Item ���� ���� ó��								
						
						g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_SHOP_SELL_ITEM,
							(DWORD)pItemInfo);
					}
					else
					{
						g_pD3dApp->m_pChat->CreateChatChild( STRMSG_C_SHOP_0008, COLOR_SYSTEM );//"�����κ��� ���� �޴���...�ٽ� �õ��ϼ���."
					}
				}
			}
			else
			{
				g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_SHOP_0010, _MESSAGE );//"������ �������� �� �� �����ϴ�."
			}
		}
	}
	// 2007-02-12 by bhsohn Item ���� ���� ó��
	// ���� ������ ����
	else if(nSelItemCnt > 0)
	{			
		if(g_pD3dApp->CheckMultItemSel(pInvenDisplayInfo->pItem->UniqueNumber))
		{			
			// ���� ������ ������ ������
			// ���� ������ ���� â �˾� 
			g_pGameMain->PopupMultiItemSelect();				
		}
		else
		{
			pParent->SetMultiSelectItem(NULL);	// ���� ó�� ����
		}
		
	}
	// end 2007-02-12 by bhsohn Item ���� ���� ó��
	pParent->SetSelectItem(NULL);
	pParent->SetItemInfo(NULL, 0, 0);
	
	return INF_MSGPROC_NORMAL;
}
int CINFInvenItem::OnMouseWhell(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bShowInven)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
	CheckMouseReverse(&pt);			
	BOOL bClick = FALSE;			
	
	{
		bClick = m_pINFInvenScrollBar->IsMouseWhellPos(pt);
		if(bClick)		
		{	
			// 2011-08-17 by hsson �κ� ���콺�� �������� �� ���¿��� �����뿡 ������ �ٸ� ������ ���� ������ ����
			g_pD3dApp->m_pInterface->m_pGameMain->m_pInven->SetSelectItem(NULL);
			// end 2011-08-17 by hsson �κ� ���콺�� �������� �� ���¿��� �����뿡 ������ �ٸ� ������ ���� ������ ����		

			int nOldScroll = m_pINFInvenScrollBar->GetScrollStep();
			m_pINFInvenScrollBar->OnMouseWheel(wParam, lParam);					
			int nNewScroll = m_pINFInvenScrollBar->GetScrollStep();
			if(nOldScroll != nNewScroll)
			{
				UpdateInvenScroll(); // ��û
			}			
			return INF_MSGPROC_BREAK;
		}
		
	}
	if(!IsWndRect(pt))
	{
		return INF_MSGPROC_NORMAL;
	}
	return INF_MSGPROC_NORMAL;
}


int CINFInvenItem::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bShowInven)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);

	{
		BOOL bClick = m_pINFInvenScrollBar->GetMouseMoveMode();
		if(bClick)
		{
			m_pINFInvenScrollBar->SetMouseMoveMode(FALSE);
			return INF_MSGPROC_BREAK;
		}				
	}
	{
		if(m_bMove)
		{
			m_bMove = FALSE;
			return INF_MSGPROC_BREAK;
		}		
	}
	CINFInvenExtend* pParent = (CINFInvenExtend*)m_pParent;
	{
		if(TRUE == m_pCloseBtn->OnLButtonUp(pt))
		{	
			GUI_BUILDINGNPC* pTempBase = g_pInterface->m_pCityBase->GetCurrentBuildingNPC();
			if(!pTempBase)
			{
				// ������ �������� �����.
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
                pParent->ShowInven(NULL, NULL, TRUE);
#else
				ShowWnd(FALSE, NULL);
#endif				
				// �����			
				// ��ư Ŭ�� 
				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
				return  INF_MSGPROC_BREAK;
			}
		}
	}

	// 2012-04-12 by mspark, �κ��丮 �ڵ� ����
	POINT ptBkSize;
	ptBkSize.x = m_pInvenBG->GetMaxPos().x - m_pInvenBG->GetMinPos().x;
	ptBkSize.y = m_pInvenBG->GetMaxPos().y - m_pInvenBG->GetMinPos().y;	
	
	if(true == m_bInvenSortBtnArea)		// 2013-02-19 by mspark, �κ��丮 ���� ��ư ���� �������� Ŭ�� ����
	{
	if(pt.x > m_ptBkPos.x + ptBkSize.x - INVEN_SORT_START_X && pt.x < m_ptBkPos.x + ptBkSize.x - INVEN_SORT_START_X + INVEN_SORT_SIZE &&
		pt.y > m_ptBkPos.y + ptBkSize.y - INVEN_SORT_START_Y && pt.y < m_ptBkPos.y + ptBkSize.y - INVEN_SORT_START_Y + INVEN_SORT_SIZE )
	{
		/*g_pStoreData->ResortingItemInInventorySort();
		pParent->ShowInven(NULL, NULL, TRUE);
		g_pGameMain->LeftWindowShow(TRUE, 1);*/
		g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_120813_0001, _Q_INVEN_ITEM_SORT);	// 2012-08-13 by mspark, �κ��丮 �ڵ� ���� Ȯ��â �߰� �۾�
			m_bInvenSortBtnArea = false;
		}
	}
	// end 2012-04-12 by mspark, �κ��丮 �ڵ� ����

	if(!IsWndRect(pt) && !m_bMove)
	{
		return INF_MSGPROC_NORMAL;
	}
	
	
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	{
		// 2008-12-02 by dgwoo ��Ű �ӽ�ó��.
		GUI_BUILDINGNPC* pTempBase = g_pInterface->m_pCityBase->GetCurrentBuildingNPC();
		BYTE  nBuildingNum = 0;
		if(pTempBase)
			nBuildingNum =  pTempBase->buildingInfo.BuildingKind;
		// 2008-12-02 by dgwoo ��Ű �ӽ�ó��.
		if(TRUE == m_pEqShow->OnLButtonUp(pt) &&
			BUILDINGKIND_LUCKY != nBuildingNum)
		{
			pParent->ShowEqInven();
			// ��ư Ŭ�� 						
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}
#endif
	
	int nWindowPosX = m_ptBkPos.x;
	int nWindowPosY = m_ptBkPos.y; 

	if( g_pGameMain->m_pQuickSlot->m_nItemType == QSLOT_ITEMTYPE_SKILL || 
				g_pGameMain->m_pQuickSlot->m_nItemType == QSLOT_ITEMTYPE_ITEM)
	{
		// ������ ���� �����ڵ�
		return INF_MSGPROC_NORMAL;
	}
	
	// �����Կ��� LBUTTONUP�� ��
	if(g_pGameMain->m_pQuickSlot->LButtonUpQuickSlot(pt))
	{
		// �����Կ����� SelectItem ���� ����
		return INF_MSGPROC_NORMAL;
	}

	CItemInfo* pSelectItem = NULL;
	if(g_pGameMain->m_stSelectItem.pSelectItem)
	{
		pSelectItem = (CItemInfo*)(g_pGameMain->m_stSelectItem.pSelectItem->pItem); 
	}
	else
	{
		// �κ��� ������ �������� ����.
		return INF_MSGPROC_NORMAL;
	}
	
	switch(g_pGameMain->m_stSelectItem.bySelectType)
	{
	case ITEM_INVEN_POS:
		{
			// �κ� ������ ����
			if(INF_MSGPROC_BREAK == OnLButtonUpInvenPosItem(pt, pSelectItem))
			{
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	case ITEM_STORE_POS:
		{
			// â�� ������ ����
			if(INF_MSGPROC_BREAK == OnLButtonUpStorePosItem(pt, pSelectItem))
			{
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	}	
	return INF_MSGPROC_NORMAL;
}

int CINFInvenItem::OnLButtonUpInvenPosItem(POINT pt, CItemInfo* pSelectItem)
{
	CINFInvenExtend* pParent = (CINFInvenExtend*)m_pParent;
	int nWindowPosX = m_ptBkPos.x;
	int nWindowPosY = m_ptBkPos.y; 
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	POINT ptBkSize;
	ptBkSize.x = m_pInvenBG->GetMaxPos().x - m_pInvenBG->GetMinPos().x;
	ptBkSize.y = m_pInvenBG->GetMaxPos().y - m_pInvenBG->GetMinPos().y;							  


	// ������ -> �ı�
	if(pSelectItem && pt.x > nWindowPosX + ptBkSize.x - INVEN_GARBAGE_START_X && pt.x < nWindowPosX + ptBkSize.x - INVEN_GARBAGE_START_X + INVEN_GARBAGE_SIZE &&
		pt.y > nWindowPosY + ptBkSize.y - INVEN_GARBAGE_START_Y && pt.y < nWindowPosY + ptBkSize.y - INVEN_GARBAGE_START_Y + INVEN_GARBAGE_SIZE )
#else
	if(pSelectItem && pt.x>nWindowPosX+INVEN_GARBAGE_START_X && pt.x<nWindowPosX+INVEN_GARBAGE_START_X+INVEN_GARBAGE_SIZE &&
		pt.y>nWindowPosY+INVEN_GARBAGE_START_Y && pt.y<nWindowPosY+INVEN_GARBAGE_START_Y + INVEN_GARBAGE_SIZE)
#endif
	{
		if( !g_pGameMain->m_pInfWindow->IsExistMsgBox(_Q_ITEM_DELETE) &&
			!g_pGameMain->m_pInfWindow->IsExistMsgBox(_Q_ITEM_DELETE_NUM))
		{
			char buf[256];
			ITEM *item = g_pDatabase->GetServerItemInfo(pSelectItem->ItemNum);


			// 2008-12-04 by dgwoo �̹� ����, �Ǵ� �ŷ�â�� �ö� �������� �����Ҽ�����.
			UID64_t nItemUID = 0;
			BOOL bPass = TRUE;
			BOOL bLuckyMechine = FALSE;
			GUI_BUILDINGNPC* pNpc = g_pInterface->m_pCityBase->GetCurrentBuildingNPC();
			if(pNpc)
			{
				if(pNpc->buildingInfo.BuildingKind == BUILDINGKIND_LUCKY)
				{// ��Ű �ӽ�.
					CMapCityShopIterator itLucky = g_pInterface->m_pCityBase->m_mapCityShop.find(BUILDINGKIND_LUCKY);
					if(itLucky != g_pInterface->m_pCityBase->m_mapCityShop.end()) 
					{
						//if(pSelectItem->UniqueNumber == ((CINFLuckyMachine*)itLucky->second)->GetSelUID())
						bPass = FALSE;
						bLuckyMechine = TRUE;
					}
				}
				if(pNpc->buildingInfo.BuildingKind == BUILDINGKIND_FACTORY)
				{// ���丮.
					CMapCityShopIterator it = g_pInterface->m_pCityBase->m_mapCityShop.find(BUILDINGKIND_FACTORY);
					
					if(it != g_pInterface->m_pCityBase->m_mapCityShop.end()) 
					{
						CINFCityLab* pCityLab = ((CINFCityLab*)it->second);
						if(pCityLab->FindItemFromSource(pSelectItem->UniqueNumber))
							bPass = FALSE;
					}
				}

				if(pNpc->buildingInfo.BuildingKind == BUILDINGKIND_LABORATORY)
				{// ������
					CMapCityShopIterator it = g_pInterface->m_pCityBase->m_mapCityShop.find(BUILDINGKIND_LABORATORY);
					
					if(it != g_pInterface->m_pCityBase->m_mapCityShop.end()) 
					{
						CINFCityLab* pCityLab = ((CINFCityLab*)it->second);
						if(pCityLab->FindItemFromSource(pSelectItem->UniqueNumber))
							bPass = FALSE;
					}
				}

				
			}
			if(g_pGameMain)
			{// �ŷ���..
				if(g_pGameMain->m_pTrade)
				{
					if(NULL != g_pGameMain->m_pTrade->FindTradeMyItem(pSelectItem->UniqueNumber))
					{
						bPass = FALSE;
					}
				}
			}
			if(g_pInterface)
			{
				if(g_pInterface->m_pBazaarShop)
				{//���� ����.
					if(g_pInterface->m_pBazaarShop->GetShopItemInfo(pSelectItem->UniqueNumber))
					{
						bPass = FALSE;	
					}

				}
			}
			if(!bPass)
			{
				if(bLuckyMechine)
				{
					//"\y��Ű�ӽ� �̿��߿��� �������� ���� �� �� �����ϴ�"
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_090324_0201,COLOR_ERROR);
				}
				else
				{
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081203_0101,COLOR_ERROR);
				}				
			}
			// 2008-12-04 by dgwoo �̹� ����, �Ǵ� �ŷ�â�� �ö� �������� �����Ҽ�����.

			if(item && bPass)
			{
				if(pSelectItem->ItemWindowIndex < POS_ITEMWINDOW_OFFSET)
				{
					g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_ITEM_0008, _MESSAGE);//"������ �������� ���� �� �����ϴ�."
					pSelectItem = NULL;
				}
				else
				{
					if(IS_COUNTABLE_ITEM(pSelectItem->Kind) && pSelectItem->CurrentCount>1)
					{
						wsprintf(buf, STRMSG_C_ITEM_0009, item->ItemName);//"������ %s ��� �����ðڽ��ϱ�?"
						g_pGameMain->m_pInfWindow->AddMsgBox(buf, 
							_Q_ITEM_DELETE_NUM, (DWORD)pSelectItem, pSelectItem->CurrentCount);
					}
					else
					{
					//	wsprintf(buf, STRMSG_C_ITEM_0010, item->ItemName);//"������ %s ��(��)  �����ðڽ��ϱ�?"
					//	g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_ITEM_DELETE);
				
						// 2013-04-23 by ssjung �߿� ������ ������ 2�� ���(����) �˾�â ����
						ImportantItemDeleteMsgBox(pSelectItem,buf,item);
					}
					
					// 2007-06-20 by bhsohn ������ ������, �޸� ���� ����
					//m_pDeleteItem = pSelectItem;
					pParent->SetDeleteItemInfo(pSelectItem);							
					return INF_MSGPROC_NORMAL;
				}				
			}
		}
	}
	int nItemPosX, nItemPosY;
	nItemPosX = nItemPosY= -1;

	if(!IsInvenRect(pt, &nItemPosX, &nItemPosY))
	{
		// ������ ����.
		return INF_MSGPROC_NORMAL;
	}

	int nScrollStep = m_pINFInvenScrollBar->GetScrollStep();	
	int nWindowPosition = (nItemPosY*INVEN_X_NUMBER)+nItemPosX+(nScrollStep*INVEN_X_NUMBER)+POS_ITEMWINDOW_OFFSET;
	
	if(	pSelectItem->ItemWindowIndex 
		&& nWindowPosition != pSelectItem->ItemWindowIndex 
		&& pSelectItem->ItemWindowIndex >= POS_ITEMWINDOW_OFFSET)
	{	// ������ -> ������
		if( pSelectItem->ItemWindowIndex < nWindowPosition )
		{
			INVEN_DISPLAY_INFO* pSelectInvenDisplayInfo = pParent->GetInvenDisplayInfo(pSelectItem->ItemWindowIndex);
			for(int i=pSelectItem->ItemWindowIndex+1; i<= nWindowPosition; i++)
			{
				CMapItemWindowInventoryIterator it = g_pStoreData->m_mapItemWindowPosition.find(i);
				if(it != g_pStoreData->m_mapItemWindowPosition.end())
				{
					CItemInfo* pSwapItem = it->second;
					pSwapItem->ItemWindowIndex = i-1;
					g_pStoreData->m_mapItemWindowPosition[i-1] = pSwapItem;
					pParent->SetSingleInvenIconInfo(pSwapItem);
				} else {
					pSelectItem->ItemWindowIndex = i-1;
					g_pStoreData->m_mapItemWindowPosition[i-1] = pSelectItem;
					pParent->SetAllIconInfo();
					pParent->SetSelectItem(NULL);
					return INF_MSGPROC_NORMAL;
				}
			}
			pSelectItem->ItemWindowIndex = nWindowPosition;
			g_pStoreData->m_mapItemWindowPosition[nWindowPosition] = pSelectItem;
			pParent->SetSingleInvenIconInfo(pSelectItem);
			pParent->SetAllIconInfo();
		}
		else if( pSelectItem->ItemWindowIndex > nWindowPosition )
		{
			INVEN_DISPLAY_INFO* pSelectInvenDisplayInfo = pParent->GetInvenDisplayInfo(pSelectItem->ItemWindowIndex);
			for(int i=pSelectItem->ItemWindowIndex-1; i >= nWindowPosition; i--)
			{
				CMapItemWindowInventoryIterator it = g_pStoreData->m_mapItemWindowPosition.find(i);
				if(it != g_pStoreData->m_mapItemWindowPosition.end())
				{
					CItemInfo* pSwapItem = it->second;
					pSwapItem->ItemWindowIndex = i+1;
					g_pStoreData->m_mapItemWindowPosition[i+1] = pSwapItem;
					pParent->SetSingleInvenIconInfo(pSwapItem);
				} else {
					DBGOUT("ERROR : CINFInven::WndProc() Item List crashed!!!!!\n");
					pParent->SetSelectItem(NULL);
					return INF_MSGPROC_NORMAL;// ������ ����Ʈ�� �������
				}
			}
			pSelectItem->ItemWindowIndex = nWindowPosition;
			g_pStoreData->m_mapItemWindowPosition[nWindowPosition] = pSelectItem;
			pParent->SetSingleInvenIconInfo(pSelectItem);
			pParent->SetAllIconInfo();
		}

		g_pGameMain->SetToolTip(NULL, 0, 0);
		pParent->SetItemInfo(NULL, 0, 0);
		pParent->SetMultiSelectItem(NULL);	// ���� ó�� ����
		pParent->SetSelectItem(NULL);
	}
	// ����â -> ������
	else if(pSelectItem->ItemWindowIndex >= 0 && pSelectItem->ItemWindowIndex < POS_ITEMWINDOW_OFFSET)	
	{
		// 2004-12-10 by jschoi
		if(pParent->IsAbleReleaseItem(pSelectItem,pSelectItem->ItemWindowIndex))
		{
			// send item windowNumber (socket)
			MSG_FC_ITEM_CHANGE_WINDOW_POSITION* pMsg;
			char buffer[SIZE_MAX_PACKET];
			*(MessageType_t*)buffer = T_FC_ITEM_CHANGE_WINDOW_POSITION;
			pMsg = (MSG_FC_ITEM_CHANGE_WINDOW_POSITION*)(buffer+SIZE_FIELD_TYPE_HEADER);
			pMsg->CharacterUniqueNumber = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber;
			pMsg->FromItemUniqueNumber = pSelectItem->UniqueNumber;
			pMsg->FromItemWindowIndex = pSelectItem->ItemWindowIndex;
			pMsg->ToItemUniqueNumber = 0;
			// ���ڸ� ��ȣ�� �����Ѵ�.			
			int i=POS_ITEMWINDOW_OFFSET+g_pStoreData->m_mapItemWindowPosition.size();//-count-1;
			pMsg->ToItemWindowIndex = i;
			
			// ���� �������� �κ��丮�� ������ �ش� ��ų�� ���⼭ ������ ��..���� ��û
			if(pSelectItem)
			{
				g_pShuttleChild->m_pSkill->DeleteSkillFromWearItem(pSelectItem->Kind);
			}
			g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_ITEM_CHANGE_WINDOW_POSITION));
			//						g_pD3dApp->m_bRequestEnable = FALSE;			// 2006-06-19 by ispark, �޼��� ������ ��ٸ���.
			
			g_pGameMain->SetToolTip(NULL, 0, 0);
			pParent->SetItemInfo(NULL, 0, 0);
			pParent->SetMultiSelectItem(NULL);	// ���� ó�� ����
			pParent->SetSelectItem(NULL);
			
			return INF_MSGPROC_BREAK;
		}
		else
		{
			char buf[128];
			wsprintf(buf,STRMSG_C_ITEM_0007);//"�䱸 �������� ���Ͽ� �ش� �������� ���� �� �� �����ϴ�."
			g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_ERROR);

			g_pGameMain->SetToolTip(NULL, 0, 0);
			pParent->SetItemInfo(NULL, 0, 0);
			pParent->SetMultiSelectItem(NULL);	// ���� ó�� ����
			pParent->SetSelectItem(NULL);
		}
	}

	return INF_MSGPROC_NORMAL;
}
// 2013-04-23 by ssjung �߿� ������ ������ 2�� ���(����) �˾�â ����
void CINFInvenItem::ImportantItemDeleteMsgBox(CItemInfo* pSelectItem, char* buf, ITEM *item)
{
	BOOL bShowWarningMsg = FALSE;
	char chItenName[256], chBuff[256];
	ZERO_MEMORY(chItenName);
	ZERO_MEMORY(chBuff);
	pSelectItem->GetItemFullName(chItenName);

	if(COMPARE_BIT_FLAG(pSelectItem->ItemInfo->ItemAttribute, ITEM_ATTR_UNIQUE_ITEM))
	{
	//	"[%s] �������� \\y����ũ\\y �������Դϴ�. ������ �����ðڽ��ϱ�?"
		bShowWarningMsg = TRUE;
		wsprintf(chBuff, STRMSG_C_130423_0001, chItenName);								
	}	
	else if(COMPARE_BIT_FLAG(pSelectItem->ItemInfo->ItemAttribute, ITEM_ATTR_LEGEND_ITEM))
	{
		//	"[%s] �������� \\m������\\m �������Դϴ�. ������ �����ðڽ��ϱ�?"
		bShowWarningMsg = TRUE;
		wsprintf(chBuff, STRMSG_C_130423_0003, chItenName);									
	}	
	else if(pSelectItem->GetEnchantNumber() > 0)
	{
		//	"[%s] �������� \\e��æƮ\\e �������Դϴ�. ������ �����ðڽ��ϱ�?"		
		bShowWarningMsg = TRUE;
		wsprintf(chBuff, STRMSG_C_130423_0002, chItenName);	
	}
	else if(0 < pSelectItem->PrefixCodeNum || 0 < pSelectItem->SuffixCodeNum)
	{
		//	"[%s] �������� \\g���� �Ǵ� ���̵�\\g �������Դϴ�. ������ �����ðڽ��ϱ�?"
		bShowWarningMsg = TRUE;
		RARE_ITEM_INFO* pPrefixRareinfo = pSelectItem->GetPrefixRareInfo();
		if(pPrefixRareinfo)
			wsprintf(chItenName, "\\g%s %s\\g",pPrefixRareinfo->Name, item->ItemName);
		RARE_ITEM_INFO* pSuffixRareinfo = pSelectItem->GetSuffixRareInfo();
		if(pSuffixRareinfo)
			wsprintf(chItenName, "\\g%s %s\\g",item->ItemName,pSuffixRareinfo->Name);

		if(0 < pSelectItem->PrefixCodeNum && 0 < pSelectItem->SuffixCodeNum)
			wsprintf(chItenName, "\\g%s %s %s\\g",pPrefixRareinfo->Name,item->ItemName,pSuffixRareinfo->Name);
			wsprintf(chBuff, STRMSG_C_130423_0004, chItenName);	
	}
	else
	{
		bShowWarningMsg = FALSE;
		wsprintf(buf, STRMSG_C_ITEM_0010, item->ItemName);//"������ %s ��(��)  �����ðڽ��ϱ�?"
	}
	if(!bShowWarningMsg)
	{
		g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_ITEM_DELETE);	
	}
	else
	{		
		g_pGameMain->m_pInfWindow->AddMsgBox(chBuff, _Q_INCHANT_ITEM_DELETE,0,0,0,pSelectItem->UniqueNumber,chItenName);
	}	
}
// end 2013-04-23 by ssjung �߿� ������ ������ 2�� ���(����) �˾�â ����

int CINFInvenItem::OnLButtonUpStorePosItem(POINT pt, CItemInfo* pSelectItem)
{
	CINFInvenExtend* pParent = (CINFInvenExtend*)m_pParent;
	int nWindowPosX = m_ptBkPos.x;
	int nWindowPosY = m_ptBkPos.y; 

	// �巹�� ������ �ʱ�ȭ
	// â�� -> �κ��丮
	GUI_BUILDINGNPC* pNpc = g_pInterface->m_pCityBase->GetCurrentBuildingNPC();
	
	if(!pNpc 
		|| !(IS_STORE_SHOP_TYPE(pNpc->buildingInfo.BuildingKind))
		|| !g_pGameMain->m_stSelectItem.pSelectItem)		
	{
		return INF_MSGPROC_NORMAL;
	}
	int nItemPosX, nItemPosY;
	nItemPosX = nItemPosY= -1;

	if(!IsInvenRect(pt, &nItemPosX, &nItemPosY))
	{
		// ������ ����.
		return INF_MSGPROC_NORMAL;
	}
	
	CItemInfo* pItemInfo = (CItemInfo*)g_pGameMain->m_stSelectItem.pSelectItem->pItem;
	
	int nStoreMultiItem = g_pGameMain->GetCityStoreMultiSelectItem();
	if(nStoreMultiItem <= 0)
	{
		if (IS_COUNTABLE_ITEM(g_pGameMain->m_stSelectItem.pSelectItem->pItem->Kind) && pItemInfo->CurrentCount > 1)
		{
			char buf[256];
			wsprintf( buf, STRMSG_C_STORE_0003, pItemInfo->ItemInfo->ItemName);//"%s ��� �������� ã���ðڽ��ϱ�?"
			g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_STORE_PUSH_ITEM,
				(DWORD)pItemInfo,pItemInfo->CurrentCount);
		}
		else
		{
			CINFCityStore* pStore = (CINFCityStore*)g_pInterface->m_pCityBase->FindBuildingShop(BUILDINGKIND_STORE);
			if(pStore)
			{
				pStore->FieldSocketSendItemToCharacter( pItemInfo->UniqueNumber, 1 );
			}
		}
	}
	else
	{
		g_pGameMain->PopupStoreMultiItemSelect();
	}
	
	pParent->SetSelectItem(NULL);	
	return  INF_MSGPROC_BREAK;	
}

int CINFInvenItem::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bShowInven)
{
	POINT ptBkPos = m_ptBkPos;
	CINFInvenExtend* pParent = (CINFInvenExtend*)m_pParent;

	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);
	if(!IsWndRect(pt) && !m_bMove)
	{
		return INF_MSGPROC_NORMAL;
	}
	pParent->SetWndOrder(INVEN_ITEM_WND);

	{
		if(TRUE == m_pCloseBtn->OnLButtonDown(pt))
		{
			// ��ư���� ���콺�� �ִ�.
			return  INF_MSGPROC_BREAK;
		}		
	}

	// 2013-02-19 by mspark, �κ��丮 ���� ��ư ���� �������� Ŭ�� ����
	POINT ptBkSize;
	ptBkSize.x = m_pInvenBG->GetMaxPos().x - m_pInvenBG->GetMinPos().x;
	ptBkSize.y = m_pInvenBG->GetMaxPos().y - m_pInvenBG->GetMinPos().y;	
	
	if(pt.x > m_ptBkPos.x + ptBkSize.x - INVEN_SORT_START_X && pt.x < m_ptBkPos.x + ptBkSize.x - INVEN_SORT_START_X + INVEN_SORT_SIZE &&
		pt.y > m_ptBkPos.y + ptBkSize.y - INVEN_SORT_START_Y && pt.y < m_ptBkPos.y + ptBkSize.y - INVEN_SORT_START_Y + INVEN_SORT_SIZE )
	{
		m_bInvenSortBtnArea = true;
	}
	// end 2013-02-19 by mspark, �κ��丮 ���� ��ư ���� �������� Ŭ�� ����

	{
		GUI_BUILDINGNPC* pTempBase = g_pInterface->m_pCityBase->GetCurrentBuildingNPC();			

		if(IsMouseCaps(pt) && !pTempBase)
		{
			m_ptCommOpMouse.x = pt.x - m_ptBkPos.x;
			m_ptCommOpMouse.y = pt.y - m_ptBkPos.y;
			m_bMove = TRUE;

			g_pGameMain->SetToolTip(NULL, 0, 0);
			pParent->SetItemInfo(NULL, 0, 0);
			pParent->SetMultiSelectItem(NULL);	// ���� ó�� ����

			return INF_MSGPROC_BREAK;
		}
	}
	int nItemPosX, nItemPosY;
	nItemPosX = nItemPosY =-1;
	
	{
		BOOL bClick = m_pINFInvenScrollBar->IsMouseBallPos(pt);
		if(bClick)
		{
			m_pINFInvenScrollBar->SetMouseMoveMode(TRUE);
			return INF_MSGPROC_BREAK;
		}
	}
	
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����	
	{
		if(TRUE == m_pEqShow->OnLButtonDown(pt))
		{
			// ��ư���� ���콺�� �ִ�.
			return  INF_MSGPROC_BREAK;
		}		
	}
#endif
	
	// SPIâ
	if((pt.x > (ptBkPos.x+INVEN_SPI_START_X-100))
		&& (pt.x < ptBkPos.x+INVEN_SPI_START_X-100+ INVEN_SPI_WIDTH) 
		&& (pt.y > (ptBkPos.y+INVEN_SPI_START_Y))
		&&(pt.y < (ptBkPos.y+INVEN_SPI_START_Y+INVEN_SPI_HEIGHT)))
	{
		GUI_BUILDINGNPC* pBuilding = g_pInterface->m_pCityBase->GetCurrentBuildingNPC();	

		if( NULL == pBuilding && LEFT_WINDOW_TRANS == g_pGameMain->m_nLeftWindowInfo)		
		{
			// �ִ� ũ�� ���� ũ�� �����Ѵ�.
			if(!g_pGameMain->m_pInfWindow->IsExistMsgBox(_Q_TRADE_ITEM_NUMBER))
			{
				if(g_pGameMain->m_pInven->GetItemSpi()>0)
				{						
					g_pGameMain->m_pInfWindow->AddMsgBox(
						STRMSG_C_TRADE_0005, _Q_TRADE_ITEM_NUMBER, 0, g_pGameMain->m_pInven->GetItemSpi());//"�󸶸� �ø��ðڽ��ϱ�?"
					g_pInterface->SetWindowOrder(WNDInfWindow);
					return INF_MSGPROC_BREAK;
				}
			}
		}
		else if(pBuilding && IS_STORE_SHOP_TYPE(pBuilding->buildingInfo.BuildingKind) 
			&& (g_pGameMain->m_pInfWindow->IsExistMsgBox(_Q_PUT_ITEM_SPI) == FALSE)
			&& (g_pGameMain->m_pInven->GetItemSpi()>STORE_KEEPING_COST))
		{				
			char buf[256];
			wsprintf( buf, STRMSG_C_STORE_0001, STORE_KEEPING_COST);//"�󸶸� �ñ�ðڽ��ϱ�?[���� : %d ����]"
			g_pGameMain->m_pInfWindow->AddMsgBox(
				buf, _Q_PUT_ITEM_SPI, 0, g_pGameMain->m_pInven->GetItemSpi()-STORE_KEEPING_COST);
			return INF_MSGPROC_BREAK;
		}
		
	}		

	// ���� ���ʹ� ������ ���� 
	if(!IsInvenRect(pt, &nItemPosX, &nItemPosY))
	{
		// ������ ����.
		pParent->ClearMultiSeletItem();	// ���߼��� �ʱ�ȭ
		return INF_MSGPROC_BREAK;
	}	

	
	
	if(g_pGameMain->m_stSelectItem.pSelectItem)
	{
		// ������ �������� �ִ�.
		pParent->ClearMultiSeletItem();	// ���߼��� �ʱ�ȭ
		return INF_MSGPROC_BREAK;
	}

	INVEN_DISPLAY_INFO*  pInvenDisplayInfo = pParent->GetInvenDisplayInfo(nItemPosY*INVEN_X_NUMBER+nItemPosX);
	if(NULL == pInvenDisplayInfo)
	{
		// �޸� ����
		pParent->ClearMultiSeletItem();	// ���߼��� �ʱ�ȭ
		return INF_MSGPROC_BREAK;
	}
	POINT ptIconPos;
	
	ptIconPos.x = pt.x - (ptBkPos.x + EXTEND_INVEN_ITEM_SLOT_START_X + (EXTEND_INVEN_SLOT_INTERVAL*nItemPosX));
	ptIconPos.y = pt.y - (ptBkPos.y + EXTEND_INVEN_ITEM_SLOT_START_Y + (EXTEND_INVEN_SLOT_INTERVAL*nItemPosY));
	
	// 2007-02-12 by bhsohn Item ���� ���� ó��			
	BOOL bMuitiItemSel = FALSE;	
	
	if(g_pD3dApp->GetCtrlBtnClick())
	{							
		// ��Ʈ���� ������ Ŭ���߳�?
		GUI_BUILDINGNPC* pNpc = g_pInterface->m_pCityBase->GetCurrentBuildingNPC();
		if(pNpc)
		{
			if(IS_ITEM_SHOP_TYPE(pNpc->buildingInfo.BuildingKind)
				|| IS_STORE_SHOP_TYPE(pNpc->buildingInfo.BuildingKind)
				|| IS_WARPOINT_SHOP_TYPE(pNpc->buildingInfo.BuildingKind))
			{
				bMuitiItemSel = TRUE;
			}								
		}
		// 2013-02-26 by bhsohn �ΰ��� ���� �˻� ó��
		else
		{
			if(pInvenDisplayInfo && pInvenDisplayInfo->pItem)
			{
				// ������ �ƴϸ� �׳� �Ϲ� Ŭ��
				pParent->OnClickItemMenuListWnd(TRUE, pt, pInvenDisplayInfo->pItem->ItemNum, pInvenDisplayInfo->pItem->UniqueNumber);
				pParent->ClearMultiSeletItem();	// ���߼��� �ʱ�ȭ
				return INF_MSGPROC_BREAK;
			}
		}		
	}	
	if(!bMuitiItemSel)
	{
		int nSelItemCnt = g_pD3dApp->GetMultiSelectItem();				
		BOOL bMultiDragSel = FALSE;
		
		// ���� �������� �� �����߳�?
		if(g_pD3dApp->CheckMultItemSel(pInvenDisplayInfo->pItem->UniqueNumber))
		{
			bMultiDragSel = TRUE;
		}
		if(!bMultiDragSel)
		{
			pParent->SetMultiSelectItem(NULL);	// ���� ó�� ����
		}							
		
		// ������ ����
		if(g_pGameMain->m_stSelectItem.pSelectItem == NULL)						
		{
			pParent->SetSelectItem(pInvenDisplayInfo, &ptIconPos);			
			return INF_MSGPROC_BREAK;
		}
	}						
	else
	{
		// ���� ���� �߰�
		if(g_pGameMain->m_stSelectItem.pSelectItem == NULL)						
		{
			pParent->SetMultiSelectItem(pInvenDisplayInfo);
			pParent->SetItemInfo(NULL, 0, 0);
			return INF_MSGPROC_BREAK;
		}
		else
		{
			pParent->SetMultiSelectItem(NULL);	// ���� ó�� ����
		}
	}												
	// Item ���� ���� ó��	
	pParent->ClearMultiSeletItem();	// ���߼��� �ʱ�ȭ	

	return INF_MSGPROC_BREAK;
}

int CINFInvenItem::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bShowInven)
{	
	CINFInvenExtend* pParent = (CINFInvenExtend*)m_pParent;

	BOOL bSelectItem = FALSE;
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);

	g_pGameMain->SetToolTip(NULL, 0, 0);

	if(!IsWndRect(pt) && !m_bMove)
	{
		pParent->SetItemInfo(NULL, 0, 0);			
		return INF_MSGPROC_NORMAL;
	}	

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pEqShow->OnMouseMove(pt);
#endif
	m_pCloseBtn->OnMouseMove(pt);

	{
		if(m_bMove)
		{
			m_ptBkPos.x = pt.x - m_ptCommOpMouse.x;
			m_ptBkPos.y = pt.y - m_ptCommOpMouse.y;				
			// UI���� ���� 
			UpdateBtnPos();
			return INF_MSGPROC_BREAK;
		}
	}
	

	int nItemPosX, nItemPosY;
	nItemPosX = nItemPosY =-1;

	{
		if(m_pINFInvenScrollBar->GetMouseMoveMode())
		{
			if(FALSE == m_pINFInvenScrollBar->IsMouseScrollPos(pt))
			{
				m_pINFInvenScrollBar->SetMouseMoveMode(FALSE);
			}
			else
			{
				int nOldScroll = m_pINFInvenScrollBar->GetScrollStep();
				m_pINFInvenScrollBar->SetScrollPos(pt);
				int nNewScroll = m_pINFInvenScrollBar->GetScrollStep();
				if(nOldScroll != nNewScroll)
				{
					UpdateInvenScroll();			// ��û
				}
				
				return INF_MSGPROC_BREAK;
			}
		}
	}

	if(!IsInvenRect(pt, &nItemPosX, &nItemPosY))
	{
		// ������ ����.		
		pParent->SetWearPosition(POS_INVALID_POSITION);
		g_pShuttleChild->GetPetManager()->SetWearPetSocketPosition(POS_INVALID_POSITION);	 //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
		if(!pParent->m_pSelectItem)
		{				
			pParent->SetItemInfo(NULL, 0, 0);
		}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
		if(m_pEqShow->IsMouseOverlab(pt))
		{
			char chTmp[256];
			sprintf( chTmp, STRMSG_C_081014_0204);
			g_pGameMain->SetToolTip( pt.x, pt.y, chTmp);
			return INF_MSGPROC_BREAK;
		}
#endif
		// 2012-04-12 by mspark, �κ��丮 �ڵ� ����
		char buf[32] = { 0, };
		
		POINT ptBkSize;
		ptBkSize.x = m_pInvenBG->GetMaxPos().x - m_pInvenBG->GetMinPos().x;
		ptBkSize.y = m_pInvenBG->GetMaxPos().y - m_pInvenBG->GetMinPos().y;	
		
		if(pt.x > m_ptBkPos.x + ptBkSize.x - INVEN_SORT_START_X && pt.x < m_ptBkPos.x + ptBkSize.x - INVEN_SORT_START_X + INVEN_SORT_SIZE &&
			pt.y > m_ptBkPos.y + ptBkSize.y - INVEN_SORT_START_Y && pt.y < m_ptBkPos.y + ptBkSize.y - INVEN_SORT_START_Y + INVEN_SORT_SIZE )
		{
			wsprintf( buf, "%s", STRMSG_C_120412_0001);
			g_pGameMain->SetToolTip( pt.x, pt.y, buf);
			return INF_MSGPROC_BREAK;
		}
		// end 2012-04-12 by mspark, �κ��丮 �ڵ� ����

		return INF_MSGPROC_BREAK;
	}		
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	if(m_pEqShow->IsMouseOverlab(pt))
	{
		char chTmp[256];
		sprintf( chTmp, STRMSG_C_081014_0204);
		g_pGameMain->SetToolTip( pt.x, pt.y, chTmp);
		return INF_MSGPROC_BREAK;
	}
#endif
	
	if(g_pGameMain->m_stSelectItem.pSelectItem 
		&& g_pGameMain->m_stSelectItem.bySelectType == ITEM_INVEN_POS)
	{	
		return INF_MSGPROC_NORMAL;
	}	
	

	{
		INVEN_DISPLAY_INFO*  pInvenDisplayInfo = pParent->GetInvenDisplayInfo(nItemPosY*INVEN_X_NUMBER+nItemPosX);
		if(pInvenDisplayInfo)
		{
			char buf[256];
			ITEM *item = NULL;
			// set tooltip
			if (COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
			{
				if(IS_COUNTABLE_ITEM(pInvenDisplayInfo->pItem->Kind))
				{
					item = g_pDatabase->GetServerItemInfo(pInvenDisplayInfo->pItem->ItemNum);
					if(item)
						wsprintf(buf, STRMSG_C_TOOLTIP_0014,item->ItemName,pInvenDisplayInfo->pItem->ItemNum,(int)((ITEM_GENERAL*)pInvenDisplayInfo->pItem)->UniqueNumber,//"%s(%08d)(%08d)(%d ��)"
						((ITEM_GENERAL*)pInvenDisplayInfo->pItem)->CurrentCount );
				}
				else
				{
					item = g_pDatabase->GetServerItemInfo(pInvenDisplayInfo->pItem->ItemNum);
					if(item)
						wsprintf(buf, "%s(%8d)(%08d)",item->ItemName,pInvenDisplayInfo->pItem->ItemNum, (int)((ITEM_GENERAL*)pInvenDisplayInfo->pItem)->UniqueNumber);
				}
			}
			else
			{
				if(IS_COUNTABLE_ITEM(pInvenDisplayInfo->pItem->Kind))
				{
					item = g_pDatabase->GetServerItemInfo(pInvenDisplayInfo->pItem->ItemNum);
					if(item)
						wsprintf(buf, STRMSG_C_TOOLTIP_0013,item->ItemName,((ITEM_GENERAL*)pInvenDisplayInfo->pItem)->CurrentCount);//"%s (%d ��)"
				}
				else
				{
					item = g_pDatabase->GetServerItemInfo(pInvenDisplayInfo->pItem->ItemNum);
					if(item)
						wsprintf(buf, "%s",item->ItemName);
				}
			}
			
			//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
			if(item->ArrDestParameter[0] == DES_PET_SOCKET_ITEM_AUTOKIT || item->ArrDestParameter[0] == DES_PET_SOCKET_ITEM_AUTOSKILL )
			{
				g_pShuttleChild->GetPetManager()->SetWearPetSocketPosition(POS_HIDDEN_ITEM);
			}										  
            //end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
			else if( item )
			{
				pParent->SetWearPosition(item->Position);						
				g_pShuttleChild->GetPetManager()->SetWearPetSocketPosition(item->Position);		//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
			}
			else
			{
				pParent->SetWearPosition(POS_INVALID_POSITION);												
				g_pShuttleChild->GetPetManager()->SetWearPetSocketPosition(POS_INVALID_POSITION);	  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
			}					
			pParent->SetItemInfo(pInvenDisplayInfo, pt.x, pt.y);								
			return INF_MSGPROC_BREAK;
		}
		else
		{
			pParent->SetWearPosition(POS_INVALID_POSITION);			
			g_pShuttleChild->GetPetManager()->SetWearPetSocketPosition(POS_INVALID_POSITION);	 //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
			pParent->SetItemInfo(NULL, 0, 0);			
			// 2009-02-03 by bhsohn ���� ������ �� ����
			//g_pGameMain->m_pItemInfo->SetItemInfoNormal( NULL, 0 , 0 );
			g_pGameMain->SetItemInfoNormal( NULL, 0 , 0 );
			// end 2009-02-03 by bhsohn ���� ������ �� ����
		}
	}
	return INF_MSGPROC_BREAK;
}

BOOL CINFInvenItem::IsInvenRect(POINT pt, int *o_pPosX, int *o_pPosY)
{
	POINT ptBkPos = m_ptBkPos;
	(*o_pPosX) = (*o_pPosY) = -1;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	POINT ptBkSize;
	ptBkSize.x = m_pInvenBG->GetMaxPos().x - m_pInvenBG->GetMinPos().x;
	ptBkSize.y = m_pInvenBG->GetMaxPos().y - m_pInvenBG->GetMinPos().y;
#else 
	POINT ptBkSize = m_pInvenBase->GetImgSize();
#endif
	if((pt.x > (ptBkPos.x+ptBkSize.x))
		|| (pt.x < ptBkPos.x))
	{
		// �ִ� ũ�� ���� ũ�� �����Ѵ�.
		return FALSE;
	}
	else if((pt.y > (ptBkPos.y+ptBkSize.y))
		|| (pt.y < (ptBkPos.y+EXTEND_INVEN_CAPS_HEIGHT)))
	{
		// �ִ� ũ�� ���� ũ�� �����Ѵ�.
		return FALSE;
	}	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	if( pt.x > ptBkPos.x + EXTEND_INVEN_ITEM_SLOT_START_X &&
		pt.x < ptBkPos.x + ptBkSize.x - 35 &&
		pt.y > ptBkPos.y + EXTEND_INVEN_ITEM_SLOT_START_Y &&
		pt.y < ptBkPos.y + ptBkSize.y - 60 
		)
	{
	int nTmpItemPosX = (pt.x-ptBkPos.x-EXTEND_INVEN_ITEM_SLOT_START_X)/EXTEND_INVEN_SLOT_INTERVAL;		
	int nTmpItemPosY = (pt.y - ptBkPos.y - EXTEND_INVEN_ITEM_SLOT_START_Y)/EXTEND_INVEN_SLOT_INTERVAL;

	if( nTmpItemPosX >= 0 && nTmpItemPosX < INVEN_X_NUMBER)
	{
		if(nTmpItemPosY >= 0 && nTmpItemPosY < INVEN_Y_NUMBER)
		{	
			(*o_pPosX) = nTmpItemPosX;
			(*o_pPosY) = nTmpItemPosY;
			return TRUE;
		}
	}
	}
#else  
	int nTmpItemPosX = (pt.x-ptBkPos.x-EXTEND_INVEN_ITEM_SLOT_START_X)/EXTEND_INVEN_SLOT_INTERVAL;		
	int nTmpItemPosY = (pt.y - ptBkPos.y - EXTEND_INVEN_ITEM_SLOT_START_Y)/EXTEND_INVEN_SLOT_INTERVAL;

	if( nTmpItemPosX >= 0 && nTmpItemPosX < INVEN_X_NUMBER)
	{
		if(nTmpItemPosY >= 0 && nTmpItemPosY < INVEN_Y_NUMBER)
		{	
			(*o_pPosX) = nTmpItemPosX;
			(*o_pPosY) = nTmpItemPosY;
			return TRUE;
		}
	}
#endif
	return FALSE;
}

BOOL CINFInvenItem::IsWndRect(POINT ptPos)
{
	POINT ptBakPos = m_ptBkPos;	

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	POINT ptSize;
	ptSize.x = m_pInvenBG->GetMaxPos().x - m_pInvenBG->GetMinPos().x;
	ptSize.y = m_pInvenBG->GetMaxPos().y - m_pInvenBG->GetMinPos().y;							  
#else 
	POINT ptSize = m_pInvenBase->GetImgSize();
#endif

	if((ptPos.x >= ptBakPos.x && (ptPos.x <= ptBakPos.x+ptSize.x))
		&& (ptPos.y >= ptBakPos.y && (ptPos.y <= ptBakPos.y+ptSize.y)))
	{
		return TRUE;
	}
	return FALSE;
}
int CINFInvenItem::GetScrollStep()
{
	return m_pINFInvenScrollBar->GetScrollStep();
}
void CINFInvenItem::SetMaxScrollStep(int nStep)
{
	m_pINFInvenScrollBar->SetMaxItem(nStep);	

}

void CINFInvenItem::SetScrollEndLine()
{
	int nMaxStep = m_pINFInvenScrollBar->GetMaxStepCnt();
	
	int nScrollStep = nMaxStep - INVEN_Y_NUMBER;
	if(nScrollStep < 0)
	{
		nScrollStep = 0;
	}
	
	// ��ũ�� ��ġ�� ���� �Ʒ�
	m_pINFInvenScrollBar->SetScrollStep(nScrollStep);		
	
	
}
// ��û
void CINFInvenItem::UpdateInvenScroll()			// ��û
{
	CINFInvenExtend* pParent = (CINFInvenExtend*)m_pParent;
	pParent->SetAllIconInfo();
	
}

void CINFInvenItem::UpdateBtnPos()
{
	POINT ptBkPos = m_ptBkPos;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	POINT ptBkSize;
	ptBkSize.x = m_pInvenBG->GetMaxPos().x - m_pInvenBG->GetMinPos().x;
	ptBkSize.y = m_pInvenBG->GetMaxPos().y - m_pInvenBG->GetMinPos().y;							  
#else								  
	POINT ptBkSize = m_pInvenBase->GetImgSize();
#endif
	{
		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptBkPos;
		
		ptScrollPos.x += EXTEND_INVEN_SCROLL_LINE_START_X;
		ptScrollPos.y += EXTEND_INVEN_SCROLL_LINE_START_Y;
		
		// ��ũ�� x = ��ġ�� -5
		// ��ũ�� height = �̹��� ������ - 34
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
		m_pINFInvenScrollBar->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,165);					  
#else
		m_pINFInvenScrollBar->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,155);
#endif
		rcMouseWhell.left		= ptScrollPos.x - ptBkSize.x;
		rcMouseWhell.top		= ptScrollPos.y - 30;
		rcMouseWhell.right		= ptScrollPos.x + 60;
		rcMouseWhell.bottom		= ptScrollPos.y + 185;
		m_pINFInvenScrollBar->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= rcMousePos.left + 32;
		rcMousePos.bottom		= rcMousePos.top + 195;
		m_pINFInvenScrollBar->SetMouseBallRect(rcMousePos);
	}

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	{
		int nPosX, nPosY;
		nPosX = ptBkPos.x + 361;
		nPosY = ptBkPos.y + 6;		
		m_pCloseBtn->SetBtnPosition(nPosX, nPosY);	
	}	   
#else
	{
		int nPosX, nPosY;
		nPosX = ptBkPos.x + 50;
		nPosY = ptBkPos.y + 227;		
		m_pEqShow->SetBtnPosition(nPosX, nPosY);	
		
	}
	{
		int nPosX, nPosY;
		nPosX = ptBkPos.x + 405;
		nPosY = ptBkPos.y + 5;		
		m_pCloseBtn->SetBtnPosition(nPosX, nPosY);	
	}
#endif	
}

BOOL CINFInvenItem::IsMouseCaps(POINT ptPos)
{
	POINT ptBakPos = m_ptBkPos;	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	POINT ptBkSize;
	ptBkSize.x = m_pInvenBG->GetMaxPos().x - m_pInvenBG->GetMinPos().x;
	ptBkSize.y = m_pInvenBG->GetMaxPos().y - m_pInvenBG->GetMinPos().y;							  
#else		 
	POINT ptBkSize = m_pInvenBase->GetImgSize();
#endif
	if((ptPos.x >= ptBakPos.x && (ptPos.x <= ptBakPos.x+ptBkSize.x))
		&& (ptPos.y >= ptBakPos.y && (ptPos.y <= ptBakPos.y+EXTEND_INVEN_CAPS_HEIGHT)))
	{
		return TRUE;
	}
	return FALSE;

}

int CINFInvenItem::OnLButtonDB(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bShowInven)
{
	if(g_pGameMain->m_pTrade->m_bTrading)
	{
		return INF_MSGPROC_NORMAL;
	}
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);

	int nItemPosX, nItemPosY;
	nItemPosX = nItemPosY= -1;

	if(!IsWndRect(pt) && !m_bMove)
	{
		return INF_MSGPROC_NORMAL;
	}

	if(!IsInvenRect(pt, &nItemPosX, &nItemPosY))
	{
		// ������ ����.
		return INF_MSGPROC_NORMAL;
	}	
	CINFInvenExtend* pParent = (CINFInvenExtend*)m_pParent;

	int nItemIdx = nItemPosY*INVEN_X_NUMBER+nItemPosX;

	INVEN_DISPLAY_INFO*  pInvenDisplayInfo = pParent->GetInvenDisplayInfo(nItemIdx);
	UID64_t	 UniqueNumber = pParent->GetDeleteItemUID();
	
	if(0 == UniqueNumber && pInvenDisplayInfo)
	{
		// set tooltip
		ITEM_BASE* pItem = pInvenDisplayInfo->pItem;
		ITEM *item = g_pDatabase->GetServerItemInfo( pItem->ItemNum);
		if(item)
		{
			// 2009-04-02 by bhsohn ��Ű �ӽ� �߰� ��ȹ��
			GUI_BUILDINGNPC* pCurrentBuildingNPC = g_pInterface->m_pCityBase->GetCurrentBuildingNPC();
			// end 2009-04-02 by bhsohn ��Ű �ӽ� �߰� ��ȹ��

			// 2008-06-03 by dgwoo ������ �����ִ� ���¿��� ����Ŭ������ ������ ��ü�� ����.
			//if(g_pInterface->m_pCityBase->GetCurrentBuildingNPC())
			if(pCurrentBuildingNPC)
			{
				// 2009-04-02 by bhsohn ��Ű �ӽ� �߰� ��ȹ��
				//g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080603_0100,COLOR_ERROR);
				switch(pCurrentBuildingNPC->buildingInfo.BuildingKind)
				{
				case BUILDINGKIND_LUCKY:
					{
						// ��Ű �ӽſ����� ���� Ŭ��
						if(!OnLButtonDbClick_LuckyMechine(item, (ITEM_GENERAL*)pInvenDisplayInfo->pItem))
						{							
						}
					}
					break;
				// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
				case BUILDINGKIND_LUCKY_OPTION_MACHINE:
					{
						// ��Ű �ӽſ����� ���� Ŭ��
						if(!OnLButtonDbClick_OptionMechine(item, pInvenDisplayInfo->pItem->UniqueNumber))
						{							
						}
					}
					break;
				//end 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
				
				// 2010-08-31 by shcho&&jskim, ���� �ý��� ����
				case BUILDINGKIND_DISSOLUTION:
					{	
						if(!OnLButtonDbClick_Dissolution(item, pInvenDisplayInfo->pItem->UniqueNumber))
						{							
						}
					}
				break;
				// end 2010-08-31 by shcho&&jskim, ���� �ý��� ����
                // 2012-02-03 by jskim ���丮 ����Ŭ�� ���� 
				case BUILDINGKIND_LABORATORY:
					{
						OnLButtonDbClick_Laboratory(item, pInvenDisplayInfo->pItem->UniqueNumber);
					}
					break;
				case BUILDINGKIND_FACTORY:
					{
						OnLButtonDbClick_Factory(item, pInvenDisplayInfo->pItem->UniqueNumber);
					}
					break;
                // end 2012-02-03 by jskim ���丮 ����Ŭ�� ���� 
				default:
					{
						//"\\y���������� �̿� �� �� �����ϴ�."
						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080603_0100, COLOR_ERROR);
					}
					break;
				}
			}
			else
			{
				switch(item->Position)
				{
				case POS_INVALID_POSITION:
					{
						// 2007-10-17 by bhsohn ���� ������ ������ �����׼���
						// ���� ���� �׸� �ʱ�ȭ
						g_pD3dApp->DelMultiItemList(TRUE);
						if(g_pGameMain->m_pInfWindow->IsExistMsgBox( _Q_STORE_PUT_COUNTABLE_ITEM ))
						{
							break;
						}
						// end 2007-10-17 by bhsohn ���� ������ ������ �����׼���
						
						
												
						// 2008. 12. 16 by ckPark ������ ��� ����
						//pParent->SendUseItem(pItem);

						// ������ ���� ������
						CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( pItem->UniqueNumber );
						// ������ ������ ���
						if(pItemInfo)
						{
							// AttackTime�� 0�̻��� �͵��� ������ ����Ʈ�� �߰��Ѵ�
							if(pItemInfo->GetItemInfo()->AttackTime > 0)
								g_pGameMain->PushDelayItem( pItemInfo );
							else
							// �ƴ� �͵��� �ٷ� ��Ŷ ����
								pParent->SendUseItem(pItem);
						}
						// end 2008. 12. 16 by ckPark ������ ��� ����

					}
					break;
				case POS_PROWIN:
				case POS_PROWOUT:
				case POS_WINGIN:
				case POS_WINGOUT:
				case POS_PROW:
				case POS_CENTER:
				case POS_REAR:
				case POS_ACCESSORY_UNLIMITED :
				case POS_ACCESSORY_TIME_LIMIT :	// 2006-03-31 by ispark
				case POS_PET:					// 2010-06-15 by shcho&hslee ��ý���
					{
						pParent->SetSelectItem(pInvenDisplayInfo);
						pParent->SendChangeWearWindowPos(item->Position);
					}
					break;
//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
				case POS_HIDDEN_ITEM:
					{
						char buf[128];
						wsprintf(buf,STRMSG_C_ITEM_0011,pItem->ItemNum);//"item [ %08d ] �� ���� �� �� ���� �������Դϴ�."
						g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_ERROR);

					}

					break;
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
				default:
					{
						char buf[128];
						wsprintf(buf,STRMSG_C_ITEM_0011,pItem->ItemNum);//"item [ %08d ] �� ���� �� �� ���� �������Դϴ�."
						g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_ERROR);
					}
				}
			}
		}		
		pParent->SetSelectItem(NULL);
		pParent->SetItemInfo(NULL, 0, 0);
		g_pGameMain->SetToolTip(0,0,NULL);
		return INF_MSGPROC_BREAK;
	}
	return INF_MSGPROC_NORMAL;

}

POINT CINFInvenItem::GetBkPos()
{
	return m_ptBkPos;
}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
POINT CINFInvenItem::GetBkSize()
{
	POINT pPos;
	pPos.x = m_pInvenBG->GetMaxPos().x - m_pInvenBG->GetMinPos().x;
	pPos.y = m_pInvenBG->GetMaxPos().y - m_pInvenBG->GetMinPos().y;
	return pPos;
}
#endif
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// ��Ű �ӽſ����� ���� Ŭ��
/// \author		// 2009-04-02 by bhsohn ��Ű �ӽ� �߰� ��ȹ��
/// \date		2009-04-02 ~ 2009-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFInvenItem::OnLButtonDbClick_LuckyMechine(ITEM *pItem , ITEM_GENERAL* pItemGeneral)
{
	CMapCityShopIterator itLucky = g_pInterface->m_pCityBase->m_mapCityShop.find(BUILDINGKIND_LUCKY);
	if(itLucky == g_pInterface->m_pCityBase->m_mapCityShop.end())
	{
		return FALSE;
	}	
	CINFLuckyMachine* pINFLuckyMachine = ((CINFLuckyMachine*)itLucky->second);

	if(pINFLuckyMachine->GetConinItemInfo())
	{
		// �̹� ������ �ִ�.
		return TRUE;
	}

	if(!pINFLuckyMachine->IsPossibleUpLoadCoin(pItem))
	{
		return FALSE;
	}
	// ������ �ø���
	pINFLuckyMachine->UpLoadCoin(pItemGeneral);
	return TRUE;
}
// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
BOOL CINFInvenItem::OnLButtonDbClick_OptionMechine(ITEM *pItem , int UniqueNumber)
{
	CMapCityShopIterator itOptionMechine = g_pInterface->m_pCityBase->m_mapCityShop.find(BUILDINGKIND_LUCKY_OPTION_MACHINE);
 	if(itOptionMechine == g_pInterface->m_pCityBase->m_mapCityShop.end())
 	{
 		return FALSE;
	}	
 	CINFOptionMachine* pINFOptionMechine = ((CINFOptionMachine*)itOptionMechine->second);
 	
	int nPosition = pINFOptionMechine->PossibleUpLoadItemNum(pItem);
	if(pINFOptionMechine->IsPossibleUpLoadItem(pItem, nPosition) == FALSE)
	{
		return FALSE;
	}
	CItemInfo* pItemInfo =g_pStoreData->FindItemInInventoryByUniqueNumber(UniqueNumber);
	pINFOptionMechine->UpLoadItem(pItemInfo, nPosition);
	return TRUE;
}
//end 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
// 2010-08-31 by shcho&&jskim, ���� �ý��� ����
BOOL CINFInvenItem::OnLButtonDbClick_Dissolution(ITEM *pItem , int UniqueNumber)
{
	CMapCityShopIterator itDissolution = g_pInterface->m_pCityBase->m_mapCityShop.find(BUILDINGKIND_DISSOLUTION);
	if(itDissolution == g_pInterface->m_pCityBase->m_mapCityShop.end())
	{
		return FALSE;
	}	
	CINFDissolution* pINFitDissolution = ((CINFDissolution*)itDissolution->second);
	if(g_pDatabase->Is_DissolutionitemInfo(pItem->ItemNum) == FALSE)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_100421_0401,COLOR_ERROR);//"\\y�÷� ���� �� �����ϴ�.\\y"
		return FALSE;
	}
	CItemInfo* pItemInfo =g_pStoreData->FindItemInInventoryByUniqueNumber(UniqueNumber);
	pINFitDissolution->UpLoadItem(pItemInfo);
	return TRUE;
}
// end 2010-08-31 by shcho&&jskim, ���� �ý��� ����
// 2010. 05. 10 by jskim �ű� ��Ű �ӽ� �߰���� - �������� ��� ������ �����۸� �����ֱ�
BOOL CINFInvenItem::ShopIsDisableInvenItem(BYTE BuildingKind, CItemInfo* pItemInfo)
{
	if(BuildingKind == BUILDINGKIND_LUCKY_OPTION_MACHINE)
	{
		if(pItemInfo->ItemInfo->IsExistDesParam(DES_RARE_FIX_PREFIX_INITIALIZE) ||
			pItemInfo->ItemInfo->IsExistDesParam(DES_RARE_FIX_SUFFIX_INITIALIZE) ||
			pItemInfo->ItemInfo->IsExistDesParam(DES_RARE_FIX_PREFIX) ||
			pItemInfo->ItemInfo->IsExistDesParam(DES_RARE_FIX_SUFFIX) ||
			IS_WEAPON(pItemInfo->Kind) ||
			ITEMKIND_DEFENSE == pItemInfo->Kind)
		{
				return FALSE;
		}
	}
	else if(BuildingKind == BUILDINGKIND_LUCKY)
	{
		if(COMPARE_BIT_FLAG(pItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_LUCKY_ITEM))
		{
 			CINFLuckyMachine* pStore = (CINFLuckyMachine*)g_pInterface->m_pCityBase->FindBuildingShop(BUILDINGKIND_LUCKY);
 			int nMachineNum = pStore->GetLuckyMachineInfo().MachineNum;
 			int nLuckyMachineLen = g_pDatabase->GetLuckyMachineLen();
			
			for(int nCnt = 0; nCnt<nLuckyMachineLen; nCnt++)
			{
				LUCKY_MACHINE_OMI*  pLuckyMachine = g_pDatabase->GetLuckyMachineInfo(nCnt);
				if(NULL == pLuckyMachine)
				{
					break;
				}
				if(nMachineNum == pLuckyMachine->MachineNum)
				{
					if(pItemInfo->ItemInfo->ItemNum == pLuckyMachine->CoinItemNum )
					{
						return FALSE;
					}					
				}
			}
		}
	}
	// 2010-08-31 by shcho&&jskim, ���� �ý��� ����
	else if(BuildingKind == BUILDINGKIND_DISSOLUTION)	
	{
		if(g_pDatabase->Is_DissolutionitemInfo(pItemInfo->ItemNum))
 		{
	 		return FALSE;  // �׸��� ����
 		}
	else
		{
			return TRUE;   // �׸��� ����
		}
	}
	// end 2010-08-31 by shcho&&jskim, ���� �ý��� ����
	else
	{
 		return FALSE;
	}
	return TRUE;
}
//end 2010. 05. 10 by jskim �ű� ��Ű �ӽ� �߰���� - �������� ��� ������ �����۸� �����ֱ�

BOOL CINFInvenItem::OnLButtonDbClick_Factory(ITEM *pItem , int UniqueNumber)
{
	CMapCityShopIterator itFactory = g_pInterface->m_pCityBase->m_mapCityShop.find(BUILDINGKIND_FACTORY);
	if( itFactory == g_pInterface->m_pCityBase->m_mapCityShop.end())
	{
		return FALSE;
	}	
	CINFCityLab* pINFLab = ((CINFCityLab*)itFactory->second);

// 	if(g_pDatabase->Is_DissolutionitemInfo(pItem->ItemNum) == FALSE)
// 	{
// 		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_100421_0401,COLOR_ERROR);//"\\y�÷� ���� �� �����ϴ�.\\y"
// 		return FALSE;
// 	}
	CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber(UniqueNumber);
	pINFLab->UpLoadItem(pItemInfo);
	return TRUE;
}

BOOL CINFInvenItem::OnLButtonDbClick_Laboratory(ITEM *pItem , int UniqueNumber)
{
	CMapCityShopIterator itLaboratory = g_pInterface->m_pCityBase->m_mapCityShop.find(BUILDINGKIND_LABORATORY);
	if( itLaboratory == g_pInterface->m_pCityBase->m_mapCityShop.end())
	{
		return FALSE;
	}	
	CINFCityLab* pINFLab = ((CINFCityLab*)itLaboratory->second);

	// 	if(g_pDatabase->Is_DissolutionitemInfo(pItem->ItemNum) == FALSE)
	// 	{
	// 		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_100421_0401,COLOR_ERROR);//"\\y�÷� ���� �� �����ϴ�.\\y"
	// 		return FALSE;
	// 	}
	CItemInfo* pItemInfo =g_pStoreData->FindItemInInventoryByUniqueNumber(UniqueNumber);
	pINFLab->UpLoadItem(pItemInfo);
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2013-02-26 by bhsohn �ΰ��� ���� �˻� ó��
/// \date		2013-03-18 ~ 2013-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFInvenItem::SetBkPos(POINT ptBkPos)
{
	m_ptBkPos = ptBkPos;
	UpdateBtnPos();
}
